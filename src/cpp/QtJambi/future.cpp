/****************************************************************************
**
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#define QFUTURE_TEST
#include "qtjambiapi.h"
#include "java_p.h"
#include <QtCore/QFutureInterfaceBase>
#include <QtCore/QFutureWatcherBase>
#include <QtCore/private/qfutureinterface_p.h>
#include "coreapi.h"
#include "registryutil_p.h"
#include "qtjambilink_p.h"

struct FutureCallOut;

struct ReverseFutureCallOut final : public QFutureCallOutInterface{
    ReverseFutureCallOut(FutureCallOut* futureCallOut) : QFutureCallOutInterface(),
                                                         m_futureCallOut(futureCallOut) { }
    void postCallOutEvent(const QFutureCallOutEvent &callOutEvent) override;
    void callOutInterfaceDisconnected() override {}
    FutureCallOut* m_futureCallOut;
};

struct FutureCallOut final : public QFutureCallOutInterface{
    FutureCallOut(QSharedPointer<QFutureInterfaceBase>&& sourceFuture, QSharedPointer<QFutureInterfaceBase>&& targetFuture, QtJambiAPI::ResultTranslator resultTranslator, QtJambiAPI::ResultTranslator resultRetranslator);
    void initialize();
    void postCallOutEvent(const QFutureCallOutEvent &callOutEvent) override;
    void callOutInterfaceDisconnected() override {}
    void disconnectFromOutputInterface(bool isReverseEvent);

    QSharedPointer<QFutureInterfaceBase> m_sourceFuture;
    QSharedPointer<QFutureInterfaceBase> m_targetFuture;
    QtJambiAPI::ResultTranslator m_resultTranslator;
    QtJambiAPI::ResultTranslator m_resultRetranslator;
    QAtomicInteger<char> m_blocked;
    ReverseFutureCallOut m_reverseFutureCallOut;
};

FutureCallOut::FutureCallOut(QSharedPointer<QFutureInterfaceBase>&& sourceFuture, QSharedPointer<QFutureInterfaceBase>&& targetFuture, QtJambiAPI::ResultTranslator resultTranslator, QtJambiAPI::ResultTranslator resultRetranslator)
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
#if QT_VERSION < QT_VERSION_CHECK(6, 3, 0)
        m_targetFuture->d->manualProgress = m_sourceFuture->d->manualProgress;
#else
        if(m_sourceFuture->d->m_progress){
            m_targetFuture->d->m_progress.reset(new QFutureInterfaceBasePrivate::ProgressData);
            m_targetFuture->d->m_progress->minimum = m_sourceFuture->d->m_progress->minimum;
            m_targetFuture->d->m_progress->maximum = m_sourceFuture->d->m_progress->maximum;
            m_targetFuture->d->m_progress->text = m_sourceFuture->d->m_progress->text;
        }
#endif
        const auto currentState = m_sourceFuture->d->state.loadRelaxed();
        try{
            if (currentState & QFutureInterfaceBase::Started) {
                postCallOutEvent(QFutureCallOutEvent(QFutureCallOutEvent::Started));
#if QT_VERSION < QT_VERSION_CHECK(6, 3, 0)
                postCallOutEvent(QFutureCallOutEvent(QFutureCallOutEvent::ProgressRange,
                                                                m_sourceFuture->d->m_progressMinimum,
                                                                m_sourceFuture->d->m_progressMaximum));
                postCallOutEvent(QFutureCallOutEvent(QFutureCallOutEvent::Progress,
                                                                m_sourceFuture->d->m_progressValue,
                                                                m_sourceFuture->d->m_progressText));
#else
                if(m_sourceFuture->d->m_progress){
                    postCallOutEvent(QFutureCallOutEvent(QFutureCallOutEvent::ProgressRange,
                                                                    m_sourceFuture->d->m_progress->minimum,
                                                                    m_sourceFuture->d->m_progress->maximum));
                    postCallOutEvent(QFutureCallOutEvent(QFutureCallOutEvent::Progress,
                                                                    m_sourceFuture->d->m_progressValue,
                                                                    m_sourceFuture->d->m_progress->text));
                }else{
                    postCallOutEvent(QFutureCallOutEvent(QFutureCallOutEvent::Progress,
                                                                    m_sourceFuture->d->m_progressValue,
                                                                    QString()));
                }
#endif
            }

#if QT_VERSION < QT_VERSION_CHECK(6, 3, 0)
            QtPrivate::ResultIteratorBase it = m_sourceFuture->d->m_results.begin();
            while (it != m_sourceFuture->d->m_results.end()) {
                const int begin = it.resultIndex();
                const int end = begin + it.batchSize();
                    postCallOutEvent(QFutureCallOutEvent(QFutureCallOutEvent::ResultsReady,
                                                                    begin,
                                                                    end));

                it.batchedAdvance();
            }
#else
            if(m_sourceFuture->d->hasException){
                m_targetFuture->d->data.setException(m_sourceFuture->d->data.m_exceptionStore.exception());
            }else{
                QtPrivate::ResultIteratorBase it = m_sourceFuture->d->data.m_results.begin();
                while (it != m_sourceFuture->d->data.m_results.end()) {
                    const int begin = it.resultIndex();
                    const int end = begin + it.batchSize();
                        postCallOutEvent(QFutureCallOutEvent(QFutureCallOutEvent::ResultsReady,
                                                                        begin,
                                                                        end));

                    it.batchedAdvance();
                }
            }
#endif

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
            m_targetFuture->d->state.storeRelaxed(currentState);
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
#if QT_VERSION >= QT_VERSION_CHECK(6, 3, 0)
            if(m_sourceFuture->hasException()){
                m_targetFuture->reportException(m_sourceFuture->exceptionStore().exception());
#elif QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
            if(std::exception_ptr exn = m_sourceFuture->exceptionStore().exception()){
                m_targetFuture->reportException(exn);
#else
            if(QException* exn = m_sourceFuture->exceptionStore().exception().exception()){
                m_targetFuture->reportException(*exn);
#endif
            }else{
                m_targetFuture->cancel();
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
            if(JniEnvironment env{200}){
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
#if QT_VERSION >= QT_VERSION_CHECK(6, 3, 0)
            if(m_futureCallOut->m_targetFuture->hasException()){
                m_futureCallOut->m_sourceFuture->reportException(m_futureCallOut->m_targetFuture->exceptionStore().exception());
#elif QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
            if(std::exception_ptr exn = m_futureCallOut->m_targetFuture->exceptionStore().exception()){
                m_futureCallOut->m_sourceFuture->reportException(exn);
#else
            if(QException* exn = m_futureCallOut->m_targetFuture->exceptionStore().exception().exception()){
                m_futureCallOut->m_sourceFuture->reportException(*exn);
#endif
            }else{
                m_futureCallOut->m_sourceFuture->cancel();
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
                if(JniEnvironment env{200}){
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

jobject QtJambiAPI::convertQFutureWatcherToJavaObject(JNIEnv* env, const QFutureWatcherBase* futureWatcher,
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

jobject QtJambiAPI::createQFutureFromQFutureInterface(JNIEnv* env, jobject futureInterface){
    return Java::QtCore::QFuture::newInstance(env, futureInterface, true);
}

jobject QtJambiAPI::getQFutureInterfaceFromQFuture(JNIEnv* env, jobject future){
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
QFutureInterfaceBase& QtJambiAPI::getQFutureInterfaceFromQFuture(const QFuture<void>& future){
    return future.d;
}
#endif

bool isVariantFuture(QSharedPointer<QFutureInterfaceBase>& sourceFuture){
    if(dynamic_cast<QFutureInterface<QVariant>*>(sourceFuture.get()))
        return true;
#if defined(Q_OS_ANDROID)
    QFutureInterfaceBase* sourceObject = sourceFuture.get();
    QByteArray sourceFutureTypeName = QtJambiAPI::typeName(typeid(*sourceObject));
    if(sourceFutureTypeName=="QFutureInterface<QVariant>"
            || sourceFutureTypeName=="QFutureInterface_shell<QVariant>")
        return true;
#endif
    return false;
}

QFutureInterfaceBase* QtJambiAPI::translateQFutureInterface(QSharedPointer<QFutureInterfaceBase>&& sourceFuture, QSharedPointer<QFutureInterfaceBase>&& targetFuture, const char* translatedType, ResultTranslator resultTranslator, ResultTranslator resultRetranslator){
    if(sourceFuture->d){
        if(isVariantFuture(sourceFuture)){
            QMutexLocker lock(&sourceFuture->d->m_mutex);
            if(!sourceFuture->d->outputConnections.isEmpty()){
                for(int i=0; i<sourceFuture->d->outputConnections.size(); ++i){
                    if(ReverseFutureCallOut* rco = dynamic_cast<ReverseFutureCallOut*>(sourceFuture->d->outputConnections[i])){
                        QFutureInterfaceBase* sourceObject = rco->m_futureCallOut->m_sourceFuture.get();
                        QFutureInterfaceBase* targetObject = targetFuture.get();
                        const std::type_info& sourceType = typeid(*sourceObject);
                        const std::type_info& targetType = typeid(*targetObject);
                        if(unique_id(targetType)==unique_id(sourceType)){
                            return sourceObject;
                        }else{
                            if(JniEnvironment env{200}){
                                QByteArray baseType = QtJambiAPI::typeName(sourceType);
                                QByteArray requiredType = QtJambiAPI::typeName(targetType);
                                auto idx = requiredType.indexOf('<');
                                if(idx>0){
                                    requiredType = translatedType + requiredType.mid(idx);
                                }else{
                                    requiredType = translatedType;
                                }
                                if(baseType=="QFutureInterfaceBase"
                                        || baseType=="QFutureInterfaceBase_shell"){
                                    Java::Runtime::IllegalArgumentException::throwNew(env, QStringLiteral("Cannot cast %1<void> to %2.").arg(translatedType, requiredType) QTJAMBI_STACKTRACEINFO );
                                }else if(baseType.startsWith("QFutureInterface<")){
                                    Java::Runtime::IllegalArgumentException::throwNew(env, QStringLiteral("Cannot cast %1%2 to %3.").arg(translatedType, baseType.mid(16), requiredType) QTJAMBI_STACKTRACEINFO );
                                }else if(baseType.startsWith("QFutureInterface_shell<")){
                                    Java::Runtime::IllegalArgumentException::throwNew(env, QStringLiteral("Cannot cast %1%2 to %3.").arg(translatedType, baseType.mid(22), requiredType) QTJAMBI_STACKTRACEINFO );
                                }
                            }
                        }
                    }else if(FutureCallOut* rco = dynamic_cast<FutureCallOut*>(sourceFuture->d->outputConnections[i])){
                        QFutureInterfaceBase* sourceObject = rco->m_targetFuture.get();
                        QFutureInterfaceBase* targetObject = targetFuture.get();
                        const std::type_info& sourceType = typeid(*sourceObject);
                        const std::type_info& targetType = typeid(*targetObject);
                        if(unique_id(targetType)==unique_id(sourceType)){
                            return sourceObject;
                        }else{
                            if(JniEnvironment env{200}){
                                QByteArray baseType = QtJambiAPI::typeName(sourceType);
                                QByteArray requiredType = QtJambiAPI::typeName(targetType);
                                auto idx = requiredType.indexOf('<');
                                if(idx>0){
                                    requiredType = translatedType + requiredType.mid(idx);
                                }else{
                                    requiredType = translatedType;
                                }
                                if(baseType=="QFutureInterfaceBase"
                                        || baseType=="QFutureInterfaceBase_shell"){
                                    Java::Runtime::IllegalArgumentException::throwNew(env, QStringLiteral("Cannot cast %1<void> to %2.").arg(translatedType, requiredType) QTJAMBI_STACKTRACEINFO );
                                }else if(baseType.startsWith("QFutureInterface<")){
                                    Java::Runtime::IllegalArgumentException::throwNew(env, QStringLiteral("Cannot cast %1%2 to %3.").arg(translatedType, baseType.mid(16), requiredType) QTJAMBI_STACKTRACEINFO );
                                }else if(baseType.startsWith("QFutureInterface_shell<")){
                                    Java::Runtime::IllegalArgumentException::throwNew(env, QStringLiteral("Cannot cast %1%2 to %3.").arg(translatedType, baseType.mid(22), requiredType) QTJAMBI_STACKTRACEINFO );
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

#if QT_VERSION >= QT_VERSION_CHECK(6, 4, 0) && QT_VERSION < QT_VERSION_CHECK(6, 5, 0)
void CoreAPI::cleanContinuation(JNIEnv*, QFutureInterfaceBase* b)
{
    if (!b->d)
        return;
    QMutexLocker lock(&b->d->continuationMutex);
    b->d->continuation = nullptr;
#if QT_VERSION > QT_VERSION_CHECK(6, 4, 0)
    b->d->continuationState = QFutureInterfaceBasePrivate::Cleaned;
#endif
}
#endif

void CoreAPI::invokeAndCatch(JNIEnv *__jni_env, void* ptr, void(*expression)(void*)){
    Q_ASSERT(expression);
    try{
        expression(ptr);
    }catch(const JavaException& exn){
        exn.raise();
    }catch(const QUnhandledException& exn){
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
        if(exn.exception()){
            invokeAndCatch(__jni_env, const_cast<QUnhandledException*>(&exn), [](void* ptr){
                QUnhandledException* exn = reinterpret_cast<QUnhandledException*>(ptr);
                std::rethrow_exception(exn->exception());
            });
        }
#else
        Q_UNUSED(exn)
#endif
        Java::QtCore::QUnhandledException::throwNew(__jni_env, "An exception has been thrown in native code." QTJAMBI_STACKTRACEINFO );
    }catch(const QException& exn){
        if(unique_id(typeid(exn))==unique_id(typeid(QException))){
            Java::QtCore::QException::throwNew(__jni_env, "An exception has been thrown in native code." QTJAMBI_STACKTRACEINFO );
        }else{
            QByteArray exceptionName(QtJambiAPI::typeName(typeid(exn)));
            const char* what = exn.what();
            const char* original_what = exn.std::exception::what();
            if(what && QLatin1String(what)!=QLatin1String(original_what) && exceptionName!=what){
                Java::QtCore::QException::throwNew(__jni_env, QStringLiteral("An exception (%1) has been thrown in native code: %2").arg(QLatin1String(exceptionName), QLatin1String(what)) QTJAMBI_STACKTRACEINFO );
            }else{
                Java::QtCore::QException::throwNew(__jni_env, QStringLiteral("An exception (%1) has been thrown in native code.").arg(QLatin1String(exceptionName)) QTJAMBI_STACKTRACEINFO );
            }
        }
    }catch(const std::exception& exn){
        QByteArray exceptionName(QtJambiAPI::typeName(typeid(exn)));
        if(exn.what() && exceptionName!=exn.what()){
            Java::QtCore::QUnhandledException::throwNew(__jni_env, QStringLiteral("An exception (%1) has been thrown in native code: %2").arg(QLatin1String(exceptionName), QLatin1String(exn.what())) QTJAMBI_STACKTRACEINFO );
        }else{
            Java::QtCore::QUnhandledException::throwNew(__jni_env, QStringLiteral("An exception (%1) has been thrown in native code.").arg(QLatin1String(exceptionName)) QTJAMBI_STACKTRACEINFO );
        }
    }catch(...){
        Java::QtCore::QUnhandledException::throwNew(__jni_env, "An exception has been thrown in native code." QTJAMBI_STACKTRACEINFO );
    }
}
