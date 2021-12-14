#define QFUTURE_TEST
#include "qtjambi_core.h"
#include "qtjambi_jobjectwrapper.h"
#include "qtjambi_repository_p.h"
#include "qtjambilink_p.h"
#include <QtCore/QFutureInterfaceBase>
#include <QtCore/QFutureWatcherBase>
#include <QtCore/private/qfutureinterface_p.h>

struct FutureCallOut;

struct ReverseFutureCallOut final : public QFutureCallOutInterface{
    ReverseFutureCallOut(FutureCallOut* futureCallOut) : QFutureCallOutInterface(),
                                                         m_futureCallOut(futureCallOut) { }
    void postCallOutEvent(const QFutureCallOutEvent &callOutEvent) override;
    void callOutInterfaceDisconnected() override {}
    FutureCallOut* m_futureCallOut;
};

struct FutureCallOut final : public QFutureCallOutInterface{
    FutureCallOut(QSharedPointer<QFutureInterfaceBase>&& sourceFuture, QSharedPointer<QFutureInterfaceBase>&& targetFuture, ResultTranslator resultTranslator, ResultTranslator resultRetranslator);
    void initialize();
    void postCallOutEvent(const QFutureCallOutEvent &callOutEvent) override;
    void callOutInterfaceDisconnected() override {}
    void disconnectFromOutputInterface(bool isReverseEvent);

    QSharedPointer<QFutureInterfaceBase> m_sourceFuture;
    QSharedPointer<QFutureInterfaceBase> m_targetFuture;
    ResultTranslator m_resultTranslator;
    ResultTranslator m_resultRetranslator;
    QAtomicInteger<char> m_blocked;
    ReverseFutureCallOut m_reverseFutureCallOut;
};

FutureCallOut::FutureCallOut(QSharedPointer<QFutureInterfaceBase>&& sourceFuture, QSharedPointer<QFutureInterfaceBase>&& targetFuture, ResultTranslator resultTranslator, ResultTranslator resultRetranslator)
    : QFutureCallOutInterface(),
      m_sourceFuture(std::move(sourceFuture)),
      m_targetFuture(std::move(targetFuture)),
      m_resultTranslator(resultTranslator),
      m_resultRetranslator(resultRetranslator),
      m_reverseFutureCallOut(this) {}

void FutureCallOut::initialize()
{
    Q_ASSERT(!m_sourceFuture.isNull());
    Q_ASSERT(!m_targetFuture.isNull());
    if(m_sourceFuture->d){
        QMutexLocker locker(&m_sourceFuture->d->m_mutex);
        m_targetFuture->d->manualProgress = m_sourceFuture->d->manualProgress;

        const auto currentState = m_sourceFuture->d->state.loadRelaxed();
        try{
            if (currentState & QFutureInterfaceBase::Started) {
                postCallOutEvent(QFutureCallOutEvent(QFutureCallOutEvent::Started));
                postCallOutEvent(QFutureCallOutEvent(QFutureCallOutEvent::ProgressRange,
                                                                m_sourceFuture->d->m_progressMinimum,
                                                                m_sourceFuture->d->m_progressMaximum));
                postCallOutEvent(QFutureCallOutEvent(QFutureCallOutEvent::Progress,
                                                                m_sourceFuture->d->m_progressValue,
                                                                m_sourceFuture->d->m_progressText));
            }

            QtPrivate::ResultIteratorBase it = m_sourceFuture->d->m_results.begin();
            while (it != m_sourceFuture->d->m_results.end()) {
                const int begin = it.resultIndex();
                const int end = begin + it.batchSize();
                    postCallOutEvent(QFutureCallOutEvent(QFutureCallOutEvent::ResultsReady,
                                                                    begin,
                                                                    end));

                it.batchedAdvance();
            }

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            if (currentState & QFutureInterfaceBase::Paused)
                postCallOutEvent(QFutureCallOutEvent(QFutureCallOutEvent::Paused));
#else
            if (currentState & QFutureInterfaceBase::Suspended)
                postCallOutEvent(QFutureCallOutEvent(QFutureCallOutEvent::Suspended));
            else if (currentState & QFutureInterfaceBase::Suspending)
                postCallOutEvent(QFutureCallOutEvent(QFutureCallOutEvent::Suspending));
#endif

            if (currentState & QFutureInterfaceBase::Canceled)
                postCallOutEvent(QFutureCallOutEvent(QFutureCallOutEvent::Canceled));
        }catch(...){
            delete this;
            throw;
        }
        if (currentState & QFutureInterfaceBase::Finished){
            postCallOutEvent(QFutureCallOutEvent(QFutureCallOutEvent::Finished));
        }else{
            m_sourceFuture->d->outputConnections.append(this);
            QMutexLocker locker2(&m_targetFuture->d->m_mutex);
            m_targetFuture->d->outputConnections.append(&m_reverseFutureCallOut);
        }
    }
}

void FutureCallOut::postCallOutEvent(const QFutureCallOutEvent &callOutEvent){
    if(!m_blocked.testAndSetRelaxed(0, 1))
        return;
    try{
        switch(callOutEvent.callOutType){
        case QFutureCallOutEvent::Started:
            m_targetFuture->reportStarted();
            break;
        case QFutureCallOutEvent::Finished:
            m_targetFuture->reportFinished();
            disconnectFromOutputInterface(false);
            return;
        case QFutureCallOutEvent::Canceled:
            try{
                m_sourceFuture->exceptionStore().throwPossibleException();
                m_targetFuture->cancel();
            }catch(const QException& exn){
                m_targetFuture->reportException(exn);
            }catch(...){
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                m_targetFuture->reportException(std::current_exception());
#endif
            }
            break;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        case QFutureCallOutEvent::Suspending:
            m_targetFuture->setSuspended(true);
            break;
        case QFutureCallOutEvent::Suspended:
            m_targetFuture->reportSuspended();
            break;
        case QFutureCallOutEvent::Resumed:
            m_targetFuture->setSuspended(false);
            break;
#else
        case QFutureCallOutEvent::Paused:
            m_targetFuture->setPaused(true);
            break;
        case QFutureCallOutEvent::Resumed:
            m_targetFuture->setPaused(false);
            break;
#endif
        case QFutureCallOutEvent::Progress:
            m_targetFuture->setProgressValueAndText(callOutEvent.index1, callOutEvent.text);
            break;
        case QFutureCallOutEvent::ProgressRange:
            m_targetFuture->setProgressRange(callOutEvent.index1, callOutEvent.index2);
            break;
        case QFutureCallOutEvent::ResultsReady:
            if(JNIEnv* env = qtjambi_current_environment()){
                m_resultTranslator(env, m_sourceFuture, m_targetFuture, callOutEvent.index1, callOutEvent.index2-callOutEvent.index1);
            }
            break;
        }
    }catch(...){
        m_blocked.storeRelaxed(0);
        throw;
    }
    m_blocked.storeRelaxed(0);
}

void FutureCallOut::disconnectFromOutputInterface(bool isReverseEvent)
{
    class InactiveFutureCallOut final : public QFutureCallOutInterface{
        void postCallOutEvent(const QFutureCallOutEvent &) override{}
        void callOutInterfaceDisconnected() override{}
    };
    static InactiveFutureCallOut inactiveFutureCallOut;

    if(m_sourceFuture->d){
        if(isReverseEvent){
            QMutexLocker lock(&m_sourceFuture->d->m_mutex);
            auto index = m_sourceFuture->d->outputConnections.indexOf(this);
            if (index > -1)
                m_sourceFuture->d->outputConnections[index] = &inactiveFutureCallOut;
        }else{
            auto index = m_sourceFuture->d->outputConnections.indexOf(this);
            if (index > -1)
                m_sourceFuture->d->outputConnections[index] = &inactiveFutureCallOut;
        }
    }
    if(m_targetFuture->d){
        if(isReverseEvent){
            auto index = m_targetFuture->d->outputConnections.indexOf(&m_reverseFutureCallOut);
            if (index > -1)
                m_targetFuture->d->outputConnections[index] = &inactiveFutureCallOut;
        }else{
            QMutexLocker lock(&m_targetFuture->d->m_mutex);
            auto index = m_targetFuture->d->outputConnections.indexOf(&m_reverseFutureCallOut);
            if (index > -1)
                m_targetFuture->d->outputConnections[index] = &inactiveFutureCallOut;
        }
    }
    delete this;
}

void ReverseFutureCallOut::postCallOutEvent(const QFutureCallOutEvent &callOutEvent){
    if(!m_futureCallOut->m_blocked.testAndSetRelaxed(0, -1))
        return;
    try{
        switch(callOutEvent.callOutType){
        case QFutureCallOutEvent::Started:
            m_futureCallOut->m_sourceFuture->reportStarted();
            break;
        case QFutureCallOutEvent::Finished:
            m_futureCallOut->m_sourceFuture->reportFinished();
            m_futureCallOut->disconnectFromOutputInterface(true);
            return;
        case QFutureCallOutEvent::Canceled:
            try{
                m_futureCallOut->m_targetFuture->exceptionStore().throwPossibleException();
                m_futureCallOut->m_sourceFuture->cancel();
            }catch(const QException& exn){
                m_futureCallOut->m_sourceFuture->reportException(exn);
            }catch(...){
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
               m_futureCallOut->m_sourceFuture->reportException(std::current_exception());
#endif
           }
           break;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        case QFutureCallOutEvent::Suspending:
            m_futureCallOut->m_sourceFuture->setSuspended(true);
            break;
        case QFutureCallOutEvent::Suspended:
            m_futureCallOut->m_sourceFuture->reportSuspended();
            break;
        case QFutureCallOutEvent::Resumed:
            m_futureCallOut->m_sourceFuture->setSuspended(false);
            break;
#else
        case QFutureCallOutEvent::Paused:
            m_futureCallOut->m_sourceFuture->setPaused(true);
            break;
        case QFutureCallOutEvent::Resumed:
            m_futureCallOut->m_sourceFuture->setPaused(false);
            break;
#endif
        case QFutureCallOutEvent::Progress:
            m_futureCallOut->m_sourceFuture->setProgressValueAndText(callOutEvent.index1, callOutEvent.text);
            break;
        case QFutureCallOutEvent::ProgressRange:
            m_futureCallOut->m_sourceFuture->setProgressRange(callOutEvent.index1, callOutEvent.index2);
            break;
        case QFutureCallOutEvent::ResultsReady:
            if(m_futureCallOut->m_resultRetranslator){
                if(JNIEnv* env = qtjambi_current_environment()){
                    m_futureCallOut->m_resultRetranslator(env, m_futureCallOut->m_sourceFuture, m_futureCallOut->m_targetFuture, callOutEvent.index1, callOutEvent.index2-callOutEvent.index1);
                }
            }
            break;
        }
    }catch(...){
        m_futureCallOut->m_blocked.storeRelaxed(0);
        throw;
    }
    m_futureCallOut->m_blocked.storeRelaxed(0);
}

QFutureInterfaceBase* qtjambi_translate_futureinterfaces(QSharedPointer<QFutureInterfaceBase>&& sourceFuture, QSharedPointer<QFutureInterfaceBase>&& targetFuture, const char* translatedType, ResultTranslator resultTranslator, ResultTranslator resultRetranslator){
    if(sourceFuture->d){
        if(dynamic_cast<QFutureInterface<QVariant>*>(sourceFuture.get())
//                && !dynamic_cast<QFutureInterface<QVariant>*>(targetFuture.get())
//                && !dynamic_cast<QFutureInterface<void>*>(targetFuture.get())
            ){
            QMutexLocker lock(&sourceFuture->d->m_mutex);
            if(!sourceFuture->d->outputConnections.isEmpty()){
                for(int i=0; i<sourceFuture->d->outputConnections.size(); ++i){
                    if(ReverseFutureCallOut* rco = dynamic_cast<ReverseFutureCallOut*>(sourceFuture->d->outputConnections[i])){
                        QFutureInterfaceBase* sourceObject = rco->m_futureCallOut->m_sourceFuture.get();
                        QFutureInterfaceBase* targetObject = targetFuture.get();
                        const std::type_info& sourceType = typeid(*sourceObject);
                        const std::type_info& targetType = typeid(*targetObject);
                        if(targetType==sourceType){
                            return sourceObject;
                        }else{
                            if(JNIEnv* env = qtjambi_current_environment()){
                                QString baseType = QLatin1String(qtjambi_type_name(sourceType));
                                QString requiredType = QLatin1String(qtjambi_type_name(targetType));
                                auto idx = requiredType.indexOf('<');
                                if(idx>0){
                                    requiredType = QLatin1String(translatedType) + requiredType.mid(idx);
                                }else{
                                    requiredType = QLatin1String(translatedType);
                                }
                                if(baseType==QLatin1String("QFutureInterfaceBase")
                                        || baseType==QLatin1String("QFutureInterfaceBase_shell")){
                                    JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Cannot cast %1<void> to %2.").arg(QLatin1String(translatedType)).arg(requiredType)) QTJAMBI_STACKTRACEINFO );
                                }else if(baseType.startsWith(QLatin1String("QFutureInterface<"))){
                                    JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Cannot cast %1%2 to %3.").arg(QLatin1String(translatedType)).arg(baseType.mid(16)).arg(requiredType)) QTJAMBI_STACKTRACEINFO );
                                }else if(baseType.startsWith(QLatin1String("QFutureInterface_shell<"))){
                                    JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Cannot cast %1%2 to %3.").arg(QLatin1String(translatedType)).arg(baseType.mid(22)).arg(requiredType)) QTJAMBI_STACKTRACEINFO );
                                }
                            }
                        }
                    }
                }
            }
        }
        (new FutureCallOut(std::move(sourceFuture), std::move(targetFuture), resultTranslator, resultRetranslator))->initialize();
    }
    return nullptr;
}

jobject qtjambi_from_QFutureWatcher(JNIEnv* env, const QFutureWatcherBase* futureWatcher,
                                    FutureSetter futureSetter, FutureResult futureResult, FutureGetter futureGetter){
    if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForQObject(const_cast<QFutureWatcherBase*>(futureWatcher))){
        jobject result = link->getJavaObjectLocalRef(env);
        if(Java::QtCore::QFutureWatcher::isInstanceOf(env, result))
            return result;
    }
    jobject result = Java::QtCore::QFutureWatcher::newInstance(env, jlong(futureSetter), jlong(futureResult), jlong(futureGetter));
    QtJambiLink::createLinkForQObject(env, result, const_cast<QFutureWatcherBase*>(futureWatcher), false, false);
    return result;
}

jobject qtjambi_create_QFuture(JNIEnv* env, jobject futureInterface){
    return Java::QtCore::QFuture::newInstance(env, futureInterface, true);
}

jobject qtjambi_get_QFutureInterface(JNIEnv* env, jobject future){
    if(Java::QtCore::QFuture::isInstanceOf(env, future)){
        return Java::QtCore::QFuture::d(env, future);
    }else if(Java::QtCore::QFutureWatcher::isInstanceOf(env, future)){
        jobject _future = Java::QtCore::QFutureWatcher::future(env, future);
        return Java::QtCore::QFuture::d(env, _future);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    }else if(Java::QtCore::QPromise::isInstanceOf(env, future)){
        return Java::QtCore::QPromise::d(env, future);
#endif
    }else return nullptr;
}

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
QFutureInterfaceBase& qtjambi_get_QFutureInterface(const QFuture<void>& future){
    return future.d;
}
#else
jobject qtjambi_create_QPromise(JNIEnv* env, jobject futureInterface, const void* promise, QtJambiScope* scope){
    for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(promise)){
        if(link){
            jobject ni = link->getJavaObjectLocalRef(env);
            if(Java::QtCore::QPromise$NativeInstance::isInstanceOf(env, ni)){
                jobject result = Java::QtCore::QPromise$NativeInstance::promise(env, ni);
                if(result)
                    return result;
            }
        }
    }
    jobject result = Java::QtCore::QPromise::newInstance(env, futureInterface, true);
    jobject ni = qtjambi_from_object(env, promise, Java::QtCore::QPromise$NativeInstance::getClass(env), false, false);
    if(scope){
        JObjectWrapper _result(env, result);
        scope->addFinalAction([env, _result](){
            jobject ni = Java::QtCore::QPromise::nativeInstance(env, _result.object());
            Java::QtCore::QPromise::set_nativeInstance(env, _result.object(), nullptr);
            qtjambi_invalidate_object(env, ni);
        });
    }
    Java::QtCore::QPromise$NativeInstance::set_promise(env, ni, result);
    Java::QtCore::QPromise::set_nativeInstance(env, result, ni);
    return result;
}

void* qtjambi_get_native_QPromise(JNIEnv* env, jobject promise){
    jobject ni = Java::QtCore::QPromise::nativeInstance(env, promise);
    return ni ? qtjambi_to_object(env, ni) : nullptr;
}
#endif
