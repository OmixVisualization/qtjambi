/****************************************************************************
**
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#include "pch_p.h"
#include <QtCore/private/qfutureinterface_p.h>

template<>
class QPromise<std::nullptr_t>{
public:
    inline static const QFutureInterfaceBasePrivate* d_ptr(const QFutureInterfaceBase& future){ return future.d; }
    inline static QFutureInterfaceBasePrivate* d_ptr(QFutureInterfaceBase& future){ return future.d; }
    inline static const QFutureInterfaceBasePrivate* d_ptr(const QFutureInterfaceBase* future){ return future ? future->d : nullptr; }
    inline static QFutureInterfaceBasePrivate* d_ptr(QFutureInterfaceBase* future){ return future ? future->d : nullptr; }
    inline static QFutureInterfaceBasePrivate* d_ptr(const QSharedPointer<QFutureInterfaceBase>& future){ return future ? future->d : nullptr; }
};

template<>
class QFuture<std::nullptr_t>{
public:
    inline static const QFutureInterfaceBase& d_ptr(const QFuture<QVariant>& future){ return future.d; }

    inline static const QFutureInterfaceBase& d_ptr(const QFuture<void>& future){ return future.d; }

    inline static QFutureInterfaceBase& d_ptr(QFuture<QVariant>& future){ return future.d; }

    inline static QFutureInterfaceBase& d_ptr(QFuture<void>& future){ return future.d; }

    inline static const QFutureInterfaceBase* d_ptr(const QFuture<QVariant>* future){ return future ? &future->d : nullptr; }

    inline static const QFutureInterfaceBase* d_ptr(const QFuture<void>* future){ return future ? &future->d : nullptr; }

    inline static QFutureInterfaceBase* d_ptr(QFuture<QVariant>* future){ return future ? &future->d : nullptr; }

    inline static QFutureInterfaceBase* d_ptr(QFuture<void>* future){ return future ? &future->d : nullptr; }
};

const QFutureInterfaceBasePrivate* d_ptr(const QFutureInterfaceBase& future){ return QPromise<std::nullptr_t>::d_ptr(future); }
QFutureInterfaceBasePrivate* d_ptr(QFutureInterfaceBase& future){ return QPromise<std::nullptr_t>::d_ptr(future); }
const QFutureInterfaceBasePrivate* d_ptr(const QFutureInterfaceBase* future){ return QPromise<std::nullptr_t>::d_ptr(future); }
QFutureInterfaceBasePrivate* d_ptr(QFutureInterfaceBase* future){ return QPromise<std::nullptr_t>::d_ptr(future); }
QFutureInterfaceBasePrivate* d_ptr(const QSharedPointer<QFutureInterfaceBase>& future){ return QPromise<std::nullptr_t>::d_ptr(future); }

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
    bool m_topLevel;
};

FutureCallOut::FutureCallOut(QSharedPointer<QFutureInterfaceBase>&& sourceFuture, QSharedPointer<QFutureInterfaceBase>&& targetFuture, QtJambiAPI::ResultTranslator resultTranslator, QtJambiAPI::ResultTranslator resultRetranslator)
    : QFutureCallOutInterface(),
      m_sourceFuture(std::move(sourceFuture)),
      m_targetFuture(std::move(targetFuture)),
      m_resultTranslator(resultTranslator),
      m_resultRetranslator(resultRetranslator),
    m_reverseFutureCallOut(this), m_topLevel(false) {}

void FutureCallOut::initialize()
{
    Q_ASSERT(!m_sourceFuture.isNull());
    Q_ASSERT(!m_targetFuture.isNull());
    if(QFutureInterfaceBasePrivate* source_d = d_ptr(m_sourceFuture)){
        if(QFutureInterfaceBasePrivate* target_d = d_ptr(m_targetFuture)){
            QMutexLocker locker(&source_d->m_mutex);
#if QT_VERSION < QT_VERSION_CHECK(6, 3, 0)
            target_d->manualProgress = source_d->manualProgress;
#else
            if(source_d->m_progress){
                target_d->m_progress.reset(new QFutureInterfaceBasePrivate::ProgressData);
                target_d->m_progress->minimum = source_d->m_progress->minimum;
                target_d->m_progress->maximum = source_d->m_progress->maximum;
                target_d->m_progress->text = source_d->m_progress->text;
            }
#endif
            const auto currentState = source_d->state.loadRelaxed();
            try{
                if (currentState & QFutureInterfaceBase::Started) {
                    postCallOutEvent(QFutureCallOutEvent(QFutureCallOutEvent::Started));
#if QT_VERSION < QT_VERSION_CHECK(6, 3, 0)
                    postCallOutEvent(QFutureCallOutEvent(QFutureCallOutEvent::ProgressRange,
                                                                    source_d->m_progressMinimum,
                                                                    source_d->m_progressMaximum));
                    postCallOutEvent(QFutureCallOutEvent(QFutureCallOutEvent::Progress,
                                                                    source_d->m_progressValue,
                                                                    source_d->m_progressText));
#else
                    if(source_d->m_progress){
                        postCallOutEvent(QFutureCallOutEvent(QFutureCallOutEvent::ProgressRange,
                                                                        source_d->m_progress->minimum,
                                                                        source_d->m_progress->maximum));
                        postCallOutEvent(QFutureCallOutEvent(QFutureCallOutEvent::Progress,
                                                                        source_d->m_progressValue,
                                                                        source_d->m_progress->text));
                    }else{
                        postCallOutEvent(QFutureCallOutEvent(QFutureCallOutEvent::Progress,
                                                                        source_d->m_progressValue,
                                                                        QString()));
                    }
#endif
                }

#if QT_VERSION < QT_VERSION_CHECK(6, 3, 0)
                QtPrivate::ResultIteratorBase it = source_d->m_results.begin();
                while (it != source_d->m_results.end()) {
                    const int begin = it.resultIndex();
                    const int end = begin + it.batchSize();
                        postCallOutEvent(QFutureCallOutEvent(QFutureCallOutEvent::ResultsReady,
                                                                        begin,
                                                                        end));

                    it.batchedAdvance();
                }
#else
                if(source_d->hasException){
                    target_d->data.setException(source_d->data.m_exceptionStore.exception());
                }else{
                    QtPrivate::ResultIteratorBase it = source_d->data.m_results.begin();
                    while (it != source_d->data.m_results.end()) {
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
                // has been deleted!
            }else{
                m_topLevel = source_d->outputConnections.isEmpty();
                if(!m_topLevel){
                    m_topLevel = true;
                    for(int i=0; i<source_d->outputConnections.size(); ++i){
                        if(dynamic_cast<ReverseFutureCallOut*>(source_d->outputConnections[i])
                            || dynamic_cast<FutureCallOut*>(source_d->outputConnections[i])){
                            m_topLevel = false;
                            break;
                        }
                    }
                }
                source_d->outputConnections.append(this);
                QMutexLocker locker2(&target_d->m_mutex);
                target_d->outputConnections.append(&m_reverseFutureCallOut);
                target_d->state.storeRelaxed(currentState);
            }
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
            m_resultTranslator(m_sourceFuture.get(), m_targetFuture.get(), callOutEvent.index1, callOutEvent.index2-callOutEvent.index1);
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

    if(QFutureInterfaceBasePrivate* source_d = d_ptr(m_sourceFuture)){
        if(isReverseEvent){
            QMutexLocker lock(&source_d->m_mutex);
            auto index = source_d->outputConnections.indexOf(this);
            if (index > -1)
                source_d->outputConnections[index] = &inactiveFutureCallOut;
        }else{
            auto index = source_d->outputConnections.indexOf(this);
            if (index > -1)
                source_d->outputConnections[index] = &inactiveFutureCallOut;
        }
    }
    if(QFutureInterfaceBasePrivate* target_d = d_ptr(m_targetFuture)){
        if(isReverseEvent){
            auto index = target_d->outputConnections.indexOf(&m_reverseFutureCallOut);
            if (index > -1)
                target_d->outputConnections[index] = &inactiveFutureCallOut;
        }else{
            QMutexLocker lock(&target_d->m_mutex);
            auto index = target_d->outputConnections.indexOf(&m_reverseFutureCallOut);
            if (index > -1)
                target_d->outputConnections[index] = &inactiveFutureCallOut;
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
                m_futureCallOut->m_resultRetranslator(m_futureCallOut->m_targetFuture.get(), m_futureCallOut->m_sourceFuture.get(), callOutEvent.index1, callOutEvent.index2-callOutEvent.index1);
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
                                    FutureSetter futureSetter, FutureResult futureResult, FutureGetter futureGetter, FutureInterfaceGetter futureInterfaceGetter){
    if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForQObject(const_cast<QFutureWatcherBase*>(futureWatcher))){
        jobject result = link->getJavaObjectLocalRef(env);
        if(Java::QtCore::QFutureWatcher::isInstanceOf(env, result))
            return result;
    }
    jobject result = Java::QtCore::QFutureWatcher::newInstance(env, nullptr);
    Java::QtCore::QFutureWatcher::set_futureGetter(env, result, jlong(futureGetter));
    Java::QtCore::QFutureWatcher::set_futureSetter(env, result, jlong(futureSetter));
    Java::QtCore::QFutureWatcher::set_futureResult(env, result, jlong(futureResult));
    Java::QtCore::QFutureWatcher::set_futureInterfaceGetter(env, result, jlong(futureInterfaceGetter));
    QtJambiLink::createLinkForNativeQObject(env, result, const_cast<QFutureWatcherBase*>(futureWatcher));
    return result;
}

bool QtJambiAPI::isVariantFutureInterface(const QFutureInterfaceBase* base){
    if(!dynamic_cast<const QFutureInterface<QVariant>*>(base)){
#if defined(Q_OS_ANDROID) || defined(Q_OS_FREEBSD)
        if(base){
            QString baseType = QLatin1String(QtJambiAPI::typeName(QtJambiPrivate::CheckPointer<QFutureInterfaceBase>::trySupplyType(base)));
            return baseType==QLatin1String("QFutureInterface_shell")
                   || baseType==QLatin1String("QFutureInterface<QVariant>");
        }
#endif
        return false;
    }
    else return true;
};

bool QtJambiAPI::isVoidFutureInterface(const QFutureInterfaceBase* base){
    if(!dynamic_cast<const QFutureInterface<void>*>(base)){
#if defined(Q_OS_ANDROID) || defined(Q_OS_FREEBSD)
        if(base){
            QString baseType = QLatin1String(QtJambiAPI::typeName(QtJambiPrivate::CheckPointer<QFutureInterfaceBase>::trySupplyType(base)));
            return baseType==QLatin1String("QFutureInterface_vshell")
                   || baseType==QLatin1String("QFutureInterface<void>");
        }
#endif
        return false;
    }
    else return true;
};

const QFutureInterface<QVariant>* QtJambiAPI::asVariantFutureInterface(const QFutureInterfaceBase* base){
    if(const QFutureInterface<QVariant>* fiv = dynamic_cast<const QFutureInterface<QVariant>*>(base)){
        return fiv;
#if defined(Q_OS_ANDROID) || defined(Q_OS_FREEBSD)
    }else if(base){
        QString baseType = QLatin1String(QtJambiAPI::typeName(QtJambiPrivate::CheckPointer<QFutureInterfaceBase>::trySupplyType(base)));
        if(baseType==QLatin1String("QFutureInterface_shell")
            || baseType==QLatin1String("QFutureInterface<QVariant>")){
            return static_cast<const QFutureInterface<QVariant>*>(base);
        }
#endif
    }
    return nullptr;
}
QFutureInterface<QVariant>* QtJambiAPI::asVariantFutureInterface(QFutureInterfaceBase* base){
    if(QFutureInterface<QVariant>* fiv = dynamic_cast<QFutureInterface<QVariant>*>(base)){
        return fiv;
#if defined(Q_OS_ANDROID) || defined(Q_OS_FREEBSD)
    }else if(base){
        QString baseType = QLatin1String(QtJambiAPI::typeName(QtJambiPrivate::CheckPointer<QFutureInterfaceBase>::trySupplyType(base)));
        if(baseType==QLatin1String("QFutureInterface_shell")
            || baseType==QLatin1String("QFutureInterface<QVariant>")){
            return static_cast<QFutureInterface<QVariant>*>(base);
        }
#endif
    }
    return nullptr;
}

const QFutureInterface<void>* QtJambiAPI::asVoidFutureInterface(const QFutureInterfaceBase* base){
    if(const QFutureInterface<void>* fiv = dynamic_cast<const QFutureInterface<void>*>(base)){
        return fiv;
#if defined(Q_OS_ANDROID) || defined(Q_OS_FREEBSD)
    }else if(base){
        QString baseType = QLatin1String(QtJambiAPI::typeName(QtJambiPrivate::CheckPointer<QFutureInterfaceBase>::trySupplyType(base)));
        if(baseType==QLatin1String("QFutureInterface_vshell")
            || baseType==QLatin1String("QFutureInterface<void>")){
            return static_cast<const QFutureInterface<void>*>(base);
        }
#endif
    }
    return nullptr;
}
QFutureInterface<void>* QtJambiAPI::asVoidFutureInterface(QFutureInterfaceBase* base){
    if(QFutureInterface<void>* fiv = dynamic_cast<QFutureInterface<void>*>(base)){
        return fiv;
#if defined(Q_OS_ANDROID) || defined(Q_OS_FREEBSD)
    }else if(base){
        QString baseType = QLatin1String(QtJambiAPI::typeName(QtJambiPrivate::CheckPointer<QFutureInterfaceBase>::trySupplyType(base)));
        if(baseType==QLatin1String("QFutureInterface_vshell")
            || baseType==QLatin1String("QFutureInterface<void>")){
            return static_cast<QFutureInterface<void>*>(base);
        }
#endif
    }
    return nullptr;
}

std::unique_ptr<QFutureInterfaceBase> QtJambiAPI::getQFutureInterfaceFromQFutureWatcher(JNIEnv* env, jobject future){
    if(FutureInterfaceGetter futureInterfaceGetter = FutureInterfaceGetter(Java::QtCore::QFutureWatcher::futureInterfaceGetter(env, future))){
        QFutureWatcherBase* ft = QtJambiAPI::convertJavaObjectToNative<QFutureWatcherBase>(env, future);
        return futureInterfaceGetter(ft);
    }else{
        QFuture<QVariant>* _future = QtJambiAPI::convertJavaObjectToNative<QFuture<QVariant>>(env, Java::QtCore::QFutureWatcher::future(env, future));
        const QFutureInterfaceBase* base = QtPrivate::Continuation<QVariant,bool,bool>::sourceFuture(_future);
        if(const QFutureInterface<QVariant>* fiv = asVariantFutureInterface(base)){
            return std::unique_ptr<QFutureInterfaceBase>{new QFutureInterface<QVariant>(*fiv)};
        }else{
            return std::unique_ptr<QFutureInterfaceBase>{new QFutureInterfaceBase(*base)};
        }
    }
}

void findParents(QList<QFutureInterfaceBase*>& parents, QFutureInterfaceBasePrivate* source_d){
    QList<QFutureInterfaceBase*> _parents;
    {
        QMutexLocker lock(&source_d->m_mutex);
        if(!source_d->outputConnections.isEmpty()){
            for(int i=0; i<source_d->outputConnections.size(); ++i){
                if(ReverseFutureCallOut* rco = dynamic_cast<ReverseFutureCallOut*>(source_d->outputConnections[i])){
                    if(rco->m_futureCallOut->m_topLevel)
                        parents[0] = rco->m_futureCallOut->m_sourceFuture.get();
                    else
                        _parents << rco->m_futureCallOut->m_sourceFuture.get();
                }else if(FutureCallOut* rco = dynamic_cast<FutureCallOut*>(source_d->outputConnections[i])){
                    if(rco->m_topLevel)
                        parents[0] = rco->m_sourceFuture.get();
                    else
                        _parents << rco->m_targetFuture.get();
                }
            }
        }
    }
    for(QFutureInterfaceBase* base : std::as_const(_parents)){
        if(!parents.contains(base)){
            if(QFutureInterfaceBasePrivate* base_d = d_ptr(base)){
                parents << base;
                findParents(parents, base_d);
            }
        }
    }
}

QFutureInterfaceBase* QtJambiAPI::translateQFutureInterface(QSharedPointer<QFutureInterfaceBase>&& sourceFuture, QSharedPointer<QFutureInterfaceBase>&& targetFuture, ResultTranslator resultTranslator, ResultTranslator resultRetranslator, FutureInterfaceTypeTest futureInterfaceTypeTest){
    if(QFutureInterfaceBasePrivate* source_d = d_ptr(sourceFuture)){
        // in case of QFuture<QVariant> -> QFuture<T1> -> QFuture<QVariant>
        // don't use top-level base because QFuture<QVariant> could represent QFuture<T2>
        if(!isVariantFutureInterface(targetFuture.get())){
            QList<QFutureInterfaceBase*> parents;
            parents << nullptr; // representing the top level element
            parents << sourceFuture.get();
            findParents(parents, source_d);
            parents.takeAt(1); // taking sourceFuture
            QFutureInterfaceBase* topLevel = parents.takeAt(0);
            if(topLevel && futureInterfaceTypeTest(topLevel))
                return topLevel;
            for(QFutureInterfaceBase* base : std::as_const(parents)){
                if(futureInterfaceTypeTest(base)){
                    return base;
                }
            }
            if(topLevel && topLevel!=sourceFuture.get() && !isVariantFutureInterface(targetFuture.get())){
                if(!isVariantFutureInterface(topLevel)){
                    // take the top level element
                    return topLevel;
                }
                if(isVariantFutureInterface(sourceFuture.get())){
                    if(QFutureInterface<QVariant>* topLevelVariant = asVariantFutureInterface(topLevel)){
                        sourceFuture = QSharedPointer<QFutureInterfaceBase>(new QFutureInterface<QVariant>(*topLevelVariant));
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

const QFutureInterfaceBase& CoreAPI::futureInterface(const QFuture<QVariant>& future){
    return QFuture<std::nullptr_t>::d_ptr(future);
}

const QFutureInterfaceBase& CoreAPI::futureInterface(const QFuture<void>& future){
    return QFuture<std::nullptr_t>::d_ptr(future);
}

QFutureInterfaceBase& CoreAPI::futureInterface(QFuture<QVariant>& future){
    return QFuture<std::nullptr_t>::d_ptr(future);
}

QFutureInterfaceBase& CoreAPI::futureInterface(QFuture<void>& future){
    return QFuture<std::nullptr_t>::d_ptr(future);
}

const QFutureInterfaceBase* CoreAPI::futureInterface(const QFuture<QVariant>* future){
    return QFuture<std::nullptr_t>::d_ptr(future);
}

const QFutureInterfaceBase* CoreAPI::futureInterface(const QFuture<void>* future){
    return QFuture<std::nullptr_t>::d_ptr(future);
}

QFutureInterfaceBase* CoreAPI::futureInterface(QFuture<QVariant>* future){
    return QFuture<std::nullptr_t>::d_ptr(future);
}

QFutureInterfaceBase* CoreAPI::futureInterface(QFuture<void>* future){
    return QFuture<std::nullptr_t>::d_ptr(future);
}

void * CoreAPI::futurePrivate(const QFutureInterfaceBase * base){
    return d_ptr(const_cast<QFutureInterfaceBase*>(base));
}

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
        if(typeid_equals(typeid(exn), typeid(QException))){
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
