/****************************************************************************
**
** Copyright (C) 2009-2024 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
**
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
**
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "metainfo.h"
#include <QtCore/QAbstractEventDispatcher>
#include <QtCore/QRegularExpression>
#include <QtCore/private/qcoreapplication_p.h>
#if (defined(Q_OS_LINUX) || defined(Q_OS_MACOS) || defined(Q_OS_FREEBSD) || defined(Q_OS_NETBSD) || defined(Q_OS_OPENBSD) || defined(Q_OS_SOLARIS)) && !defined(Q_OS_ANDROID)
#include <signal.h>
#include <stdio.h>
#include <map>
#include <initializer_list>
#include "threadapi.h"
#endif
#include "java_p.h"
#include "utils_p.h"
#include "qtjambilink_p.h"
#if QT_VERSION < QT_VERSION_CHECK(6, 8, 0)
#include "threadutils_p.h"
#endif
#include "qtjambi_cast.h"

void shutdown(JNIEnv * env, bool regular);

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambi_1LibraryUtilities_shutdown)
(JNIEnv * env, jclass)
{
    QTJAMBI_NATIVE_METHOD_CALL("QtJambi_LibraryUtilities::shutdown()")
    try{
        bool regular = true;
#if QT_VERSION >= QT_VERSION_CHECK(6, 8, 0)
        if(auto mainThreadId = QCoreApplicationPrivate::theMainThreadId.loadRelaxed()){
            if(mainThreadId==QThread::currentThreadId()){
                if(QCoreApplication::instance() && !Java::Runtime::Boolean::getBoolean(env, env->NewStringUTF("io.qt.no-app-deletion"))){
                    delete QCoreApplication::instance();
                }
            }else if(QThread* mainThread = QCoreApplicationPrivate::theMainThread.loadRelaxed()){
                if(mainThread->isRunning())
                    regular = false;
            }
        }
#else
        if(QThread* mainThread = QCoreApplicationPrivate::theMainThread.loadRelaxed()){
            QThreadData* td = QThreadData::get2(mainThread);
            QThreadUserData::theMainThreadId.storeRelaxed(nullptr);
            if(td && td->threadId.loadRelaxed()==QThread::currentThreadId()){
                if(QCoreApplication::instance() && !Java::Runtime::Boolean::getBoolean(env, env->NewStringUTF("io.qt.no-app-deletion"))){
                    delete QCoreApplication::instance();
                }
            }else{
                if(mainThread->isRunning())
                    regular = false;
            }
        }
#endif
        shutdown(env, regular);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QtJambi_1LibraryUtilities_internalAccess)(JNIEnv *env, jclass cls){
    try{
        return MetaInfoAPI::internalAccess(env, cls);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_ExceptionUtility_convertNativeException)
(JNIEnv *env,
 jclass,
 jlong exception)
{
    if(exception){
        const std::exception* exn = reinterpret_cast<const std::exception*>(exception);
        if(typeid_equals(typeid(*exn), typeid(JavaException))){
            const JavaException* jexn = reinterpret_cast<const JavaException*>(exception);
            return jexn->throwable(env);
        }else{
            return Java::Runtime::RuntimeException::newInstanceWithMessage(env, exn->what());
        }
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT jboolean JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QtUtilities_putenv)
    (JNIEnv *env, jclass, jstring varName, jstring value)
{
    try{
        J2CStringBuffer _varName(env, varName);
        J2CStringBuffer _value(env, value);
        return qputenv(_varName.data(), _value.data());
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return false;
}

extern "C" Q_DECL_EXPORT jboolean JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_ReferenceUtility_needsReferenceCounting)
    (JNIEnv *env, jclass, QtJambiNativeID objId)
{
    try{
        if (QSharedPointer<QtJambiLink> link = QtJambiLink::fromNativeId(objId)) {
            return link->needsReferenceCounting() || link->ownership()==QtJambiLink::Ownership::Java;
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return false;
}

extern "C" Q_DECL_EXPORT jint JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_LibraryUtility_qtJambiVersion)(JNIEnv *,jclass)
{
    return QT_VERSION_CHECK(QT_VERSION_MAJOR, QT_VERSION_MINOR, QTJAMBI_PATCH_VERSION);
}

extern "C" Q_DECL_EXPORT jstring JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_LibraryUtility_libraryFilePath)
(JNIEnv *env,
 jclass)
{
    try{
        QString path = getFunctionLibraryPath(reinterpret_cast<QFunctionPointer>(&Java_io_qt_internal_LibraryUtility_libraryFilePath));
        return qtjambi_cast<jstring>(env, path);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT jclass JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_AccessUtility_getClass)
    (JNIEnv * env,
     jobject, jobject e)
{
    return e ? env->GetObjectClass(e) : nullptr;
}

extern "C" Q_DECL_EXPORT jboolean JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_ClassAnalyzerUtility_isGeneratedClass)
    (JNIEnv *env,
     jclass,
     jstring className)
{
    try{
        QString cn = qtjambi_cast<QString>(env, className).replace(".", "/");
        return getTypeByJavaName(cn)!=nullptr || isJavaNameNamespace(cn);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return false;
}

extern "C" Q_DECL_EXPORT jboolean JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QtUtilities_unsetenv)
    (JNIEnv *env, jclass, jstring varName)
{
    try{
        J2CStringBuffer _varName(env, varName);
        return qunsetenv(_varName.data());
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return false;
}

extern "C" Q_DECL_EXPORT jstring JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QtUtilities_getenv)
    (JNIEnv *env, jclass, jstring varName)
{
    try{
        J2CStringBuffer _varName(env, varName);
        if(qEnvironmentVariableIsSet(_varName.constData())){
            QString value = qEnvironmentVariable(_varName.constData());
            return qtjambi_cast<jstring>(env, value);
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT bool JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QtUtilities_reinstallEventNotifyCallback)(JNIEnv * env)
{
    try{
        if(QThread* mainThread = QCoreApplicationPrivate::theMainThread.loadRelaxed()){
            if(QThread::currentThread()!=mainThread){
                Java::Runtime::IllegalStateException::throwNew(env, "Unable to reinstall event notify callback from outside the main thread." QTJAMBI_STACKTRACEINFO );
            }
            if(mainThread->loopLevel()>0){
                Java::Runtime::IllegalStateException::throwNew(env, "Unable to reinstall event notify callback with running application event loop." QTJAMBI_STACKTRACEINFO );
            }
        }else{
            Java::Runtime::IllegalStateException::throwNew(env, "Unable to reinstall event notify callback without main thread." QTJAMBI_STACKTRACEINFO );
        }
        if(Java::Runtime::Boolean::getBoolean(env, env->NewStringUTF("io.qt.enable-event-thread-affinity-check"))){
            QInternal::unregisterCallback(QInternal::EventNotifyCallback, &threadAffineEventNotify);
            QInternal::unregisterCallback(QInternal::EventNotifyCallback, &simpleEventNotify);
            return QInternal::registerCallback(QInternal::EventNotifyCallback, &threadAffineEventNotify);
        }else{
            QInternal::unregisterCallback(QInternal::EventNotifyCallback, &threadAffineEventNotify);
            QInternal::unregisterCallback(QInternal::EventNotifyCallback, &simpleEventNotify);
            return QInternal::registerCallback(QInternal::EventNotifyCallback, &simpleEventNotify);
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return false;
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QtUtilities_uiThreadCheck)(JNIEnv *env, jclass)
{
    try{
        QtJambiAPI::checkThreadUI(env, typeid(void));
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QtUtilities_threadCheck)(JNIEnv *env, jclass, jobject object)
{
    try{
        checkThreadOnQObject(env, object);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

class SelectiveEventFilter : public QObject{
    QObject* m_eventFilter;
    bool(&predicate)(const void*,QObject *, QEvent *);
    void(&deleter)(void*);
    void* data;
    SelectiveEventFilter(QObject* _eventFilter, bool(&_predicate)(const void*,QObject *, QEvent *), void(&_deleter)(void*), void* _data)
        : QObject(_eventFilter->parent()),
        m_eventFilter(_eventFilter),
        predicate(_predicate),
        deleter(_deleter),
        data(_data)
    {
        m_eventFilter->setParent(this);
    }
    template<typename Predicate>
    static bool test(const void* data,QObject *watched, QEvent *event){
        return (*reinterpret_cast<const Predicate*>(data))(watched, event);
    }
    template<typename Predicate>
    static void destroy(void* data){
        delete reinterpret_cast<Predicate*>(data);
    }
public:
    template<typename Predicate>
    SelectiveEventFilter(QObject* _eventFilter, Predicate&& predicate)
        : SelectiveEventFilter(_eventFilter, SelectiveEventFilter::test<Predicate>,
                               SelectiveEventFilter::destroy<Predicate>,
                               new Predicate(std::move(predicate)))
    {
    }
    ~SelectiveEventFilter(){
        deleter(data);
    }
    bool eventFilter(QObject *watched, QEvent *event) final override{
        if(predicate(data, watched, event)){
            return m_eventFilter->eventFilter(watched, event);
        }
        return false;
    }
};

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QtUtilities_asSelectiveEventFilterEventTypes)(JNIEnv *env, jclass, QtJambiNativeID objectId, jobject firstType, jobjectArray types)
{
    try{
        QObject* eventFilter = QtJambiAPI::objectFromNativeId<QObject>(objectId);
        QtJambiAPI::checkThread(env, eventFilter);
        QSet<QEvent::Type> typeSet;
        typeSet.insert(qtjambi_cast<QEvent::Type>(env, firstType));
        for(jsize i=0, l = types ? env->GetArrayLength(types) : 0; i<l; ++i){
            typeSet.insert(qtjambi_cast<QEvent::Type>(env, env->GetObjectArrayElement(types, i)));
        }
        eventFilter = new SelectiveEventFilter(eventFilter, [typeSet = std::move(typeSet)](QObject *, QEvent *event){
            return typeSet.contains(event->type());
        });
        jobject result = qtjambi_cast<jobject>(env, eventFilter);
        QtJambiAPI::setJavaOwnershipForTopLevelObject(env, eventFilter);
        return result;
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

enum StringComparison{
    Equal,
    NotEqual,
    LessThan,
    GreaterThan,
    LessThanOrEqual,
    GreaterThanOrEqual,
    Contains,
    ContainsNot,
    StartsWith,
    StartsNotWith,
    EndsWith,
    EndsNotWith
};

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QtUtilities_asSelectiveEventFilterObjectNames)(JNIEnv *env, jclass, QtJambiNativeID objectId, jint stringComparisonType, jint caseSensitivity, jstring objectName, jobjectArray objectNames)
{
    try{
        QObject* eventFilter = QtJambiAPI::objectFromNativeId<QObject>(objectId);
        QtJambiAPI::checkThread(env, eventFilter);
        QList<QString> objectNameList;
        objectNameList << qtjambi_cast<QString>(env, objectName);
        for(jsize i=0, l = objectNames ? env->GetArrayLength(objectNames) : 0; i<l; ++i){
            objectNameList << qtjambi_cast<QString>(env, env->GetObjectArrayElement(objectNames, i));
        }
        switch(stringComparisonType){
        case Equal:
            eventFilter = new SelectiveEventFilter(eventFilter, [caseSensitivity = Qt::CaseSensitivity(caseSensitivity),
                                                                 objectNameList = std::move(objectNameList)](QObject *watched, QEvent *){
                QString watchedObjectName = watched->objectName();
                for(const QString& objectName : objectNameList){
                    if(watchedObjectName.compare(objectName, caseSensitivity)==0)
                        return true;
                }
                return false;
            });
            break;
        case NotEqual:
            eventFilter = new SelectiveEventFilter(eventFilter, [caseSensitivity = Qt::CaseSensitivity(caseSensitivity),
                                                                 objectNameList = std::move(objectNameList)](QObject *watched, QEvent *){
                QString watchedObjectName = watched->objectName();
                for(const QString& objectName : objectNameList){
                    if(watchedObjectName.compare(objectName, caseSensitivity)!=0)
                        return true;
                }
                return false;
            });
            break;
        case LessThan:
            eventFilter = new SelectiveEventFilter(eventFilter, [caseSensitivity = Qt::CaseSensitivity(caseSensitivity),
                                                                 objectNameList = std::move(objectNameList)](QObject *watched, QEvent *){
                QString watchedObjectName = watched->objectName();
                for(const QString& objectName : objectNameList){
                    if(watchedObjectName.compare(objectName, caseSensitivity)<0)
                        return true;
                }
                return false;
            });
            break;
        case GreaterThan:
            eventFilter = new SelectiveEventFilter(eventFilter, [caseSensitivity = Qt::CaseSensitivity(caseSensitivity),
                                                                 objectNameList = std::move(objectNameList)](QObject *watched, QEvent *){
                QString watchedObjectName = watched->objectName();
                for(const QString& objectName : objectNameList){
                    if(watchedObjectName.compare(objectName, caseSensitivity)>0)
                        return true;
                }
                return false;
            });
            break;
        case LessThanOrEqual:
            eventFilter = new SelectiveEventFilter(eventFilter, [caseSensitivity = Qt::CaseSensitivity(caseSensitivity),
                                                                 objectNameList = std::move(objectNameList)](QObject *watched, QEvent *){
                QString watchedObjectName = watched->objectName();
                for(const QString& objectName : objectNameList){
                    if(watchedObjectName.compare(objectName, caseSensitivity)<=0)
                        return true;
                }
                return false;
            });
            break;
        case GreaterThanOrEqual:
            eventFilter = new SelectiveEventFilter(eventFilter, [caseSensitivity = Qt::CaseSensitivity(caseSensitivity),
                                                                 objectNameList = std::move(objectNameList)](QObject *watched, QEvent *){
                QString watchedObjectName = watched->objectName();
                for(const QString& objectName : objectNameList){
                    if(watchedObjectName.compare(objectName, caseSensitivity)>=0)
                        return true;
                }
                return false;
            });
            break;
        case Contains:
            eventFilter = new SelectiveEventFilter(eventFilter, [caseSensitivity = Qt::CaseSensitivity(caseSensitivity),
                                                                 objectNameList = std::move(objectNameList)](QObject *watched, QEvent *){
                QString watchedObjectName = watched->objectName();
                for(const QString& objectName : objectNameList){
                    if(watchedObjectName.contains(objectName, caseSensitivity))
                        return true;
                }
                return false;
            });
            break;
        case ContainsNot:
            eventFilter = new SelectiveEventFilter(eventFilter, [caseSensitivity = Qt::CaseSensitivity(caseSensitivity),
                                                                 objectNameList = std::move(objectNameList)](QObject *watched, QEvent *){
                QString watchedObjectName = watched->objectName();
                for(const QString& objectName : objectNameList){
                    if(!watchedObjectName.contains(objectName, caseSensitivity))
                        return true;
                }
                return false;
            });
            break;
        case StartsWith:
            eventFilter = new SelectiveEventFilter(eventFilter, [caseSensitivity = Qt::CaseSensitivity(caseSensitivity),
                                                                 objectNameList = std::move(objectNameList)](QObject *watched, QEvent *){
                QString watchedObjectName = watched->objectName();
                for(const QString& objectName : objectNameList){
                    if(watchedObjectName.startsWith(objectName, caseSensitivity))
                        return true;
                }
                return false;
            });
            break;
        case StartsNotWith:
            eventFilter = new SelectiveEventFilter(eventFilter, [caseSensitivity = Qt::CaseSensitivity(caseSensitivity),
                                                                 objectNameList = std::move(objectNameList)](QObject *watched, QEvent *){
                QString watchedObjectName = watched->objectName();
                for(const QString& objectName : objectNameList){
                    if(!watchedObjectName.startsWith(objectName, caseSensitivity))
                        return true;
                }
                return false;
            });
            break;
        case EndsWith:
            eventFilter = new SelectiveEventFilter(eventFilter, [caseSensitivity = Qt::CaseSensitivity(caseSensitivity),
                                                                 objectNameList = std::move(objectNameList)](QObject *watched, QEvent *){
                QString watchedObjectName = watched->objectName();
                for(const QString& objectName : objectNameList){
                    if(watchedObjectName.endsWith(objectName, caseSensitivity))
                        return true;
                }
                return false;
            });
            break;
        case EndsNotWith:
            eventFilter = new SelectiveEventFilter(eventFilter, [caseSensitivity = Qt::CaseSensitivity(caseSensitivity),
                                                                 objectNameList = std::move(objectNameList)](QObject *watched, QEvent *){
                QString watchedObjectName = watched->objectName();
                for(const QString& objectName : objectNameList){
                    if(!watchedObjectName.endsWith(objectName, caseSensitivity))
                        return true;
                }
                return false;
            });
            break;
        default:
            eventFilter = nullptr;
            break;
        }
        jobject result = qtjambi_cast<jobject>(env, eventFilter);
        QtJambiAPI::setJavaOwnershipForTopLevelObject(env, eventFilter);
        return result;
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QtUtilities_asSelectiveEventFilterMetaObjects)(JNIEnv *env, jclass, QtJambiNativeID objectId, jobject firstMetaObject, jobjectArray metaObjects)
{
    try{
        QObject* eventFilter = QtJambiAPI::objectFromNativeId<QObject>(objectId);
        QtJambiAPI::checkThread(env, eventFilter);
        QList<const QMetaObject*> metaObjectList;
        metaObjectList << &qtjambi_cast<const QMetaObject&>(env, firstMetaObject);
        for(jsize i=0, l = metaObjects ? env->GetArrayLength(metaObjects) : 0; i<l; ++i){
            metaObjectList << &qtjambi_cast<const QMetaObject&>(env, env->GetObjectArrayElement(metaObjects, i));
        }
        eventFilter = new SelectiveEventFilter(eventFilter, [metaObjectList = std::move(metaObjectList)](QObject *watched, QEvent *){
            const QMetaObject* watchedMetaObject = watched->metaObject();
            for(const QMetaObject* metaObject : metaObjectList){
                if(watchedMetaObject->inherits(metaObject))
                    return true;
            }
            return false;
        });
        jobject result = qtjambi_cast<jobject>(env, eventFilter);
        QtJambiAPI::setJavaOwnershipForTopLevelObject(env, eventFilter);
        return result;
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QtUtilities_asSelectiveEventFilterObjectNameMatches)(JNIEnv *env, jclass, QtJambiNativeID objectId, QtJambiNativeID regexpId, jlong _offset, jint _matchType, jint _matchOptions)
{
    try{
        QObject* eventFilter = QtJambiAPI::objectFromNativeId<QObject>(objectId);
        QtJambiAPI::checkThread(env, eventFilter);
        eventFilter = new SelectiveEventFilter(eventFilter, [regexp = QtJambiAPI::valueFromNativeId<QRegularExpression>(regexpId),
                                                             offset = qsizetype(_offset),
                                                             matchType = QRegularExpression::MatchType(_matchType),
                                                             matchOptions = QRegularExpression::MatchOptions(_matchOptions)](QObject *watched, QEvent *){
            QString objectName = watched->objectName();
            if(regexp.match(objectName, offset, matchType, matchOptions).hasMatch())
                return true;
            return false;
        });
        jobject result = qtjambi_cast<jobject>(env, eventFilter);
        QtJambiAPI::setJavaOwnershipForTopLevelObject(env, eventFilter);
        return result;
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

#if (defined(Q_OS_LINUX) || defined(Q_OS_MACOS) || defined(Q_OS_FREEBSD) || defined(Q_OS_NETBSD) || defined(Q_OS_OPENBSD) || defined(Q_OS_SOLARIS)) && !defined(Q_OS_ANDROID)
#if defined(signals)
#undef signals
#endif

struct SignalCache : public QtJambiObjectData{
    SignalCache() = default;
    ~SignalCache() override = default;
    QTJAMBI_OBJECTUSERDATA_ID_IMPL(static,)
    Q_DISABLE_COPY_MOVE(SignalCache)
    static std::vector<int> signals(){
        return std::vector<int>{SIGSEGV,SIGBUS,SIGFPE,SIGPIPE,SIGILL,SIGQUIT,SIGUSR2,SIGFPE};
    }
    std::map<int,std::pair<int,void(*)(int)>> handlers;
};
#endif

extern "C" Q_DECL_EXPORT bool JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QtUtilities_saveUnixSignalHandlers)(JNIEnv *env, jclass)
{
#if (defined(Q_OS_LINUX) || defined(Q_OS_MACOS) || defined(Q_OS_FREEBSD) || defined(Q_OS_NETBSD) || defined(Q_OS_OPENBSD) || defined(Q_OS_SOLARIS)) && !defined(Q_OS_ANDROID)
    try{
        QThread* mainThread = QCoreApplicationPrivate::theMainThread.loadRelaxed();
        if(!mainThread){
            ThreadAPI::initializeMainThread(env);
            mainThread = QCoreApplicationPrivate::theMainThread.loadRelaxed();
        }
        if(mainThread){
            std::unique_ptr<SignalCache> cache{new SignalCache()};
            for(int s : SignalCache::signals()){
                struct sigaction sa;
                memset(&sa, 0, sizeof(sa));
                if(sigaction(s, nullptr, &sa)==0){
                    qCDebug(DebugAPI::internalCategory, "Signalhandler for %d saved.", s);
                    cache->handlers[s] = {sa.sa_flags, sa.sa_handler};
                }else
                    qCDebug(DebugAPI::internalCategory, "Unable to save signalhandler for %d.", s);
            }
            QWriteLocker locker(QtJambiLinkUserData::lock());
            QTJAMBI_SET_OBJECTUSERDATA(SignalCache, mainThread, cache.release());
            return true;
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
#else
    Q_UNUSED(env)
#endif
    return false;
}

extern "C" Q_DECL_EXPORT bool JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QtUtilities_restoreUnixSignalHandlers)(JNIEnv *env, jclass)
{
#if (defined(Q_OS_LINUX) || defined(Q_OS_MACOS) || defined(Q_OS_FREEBSD) || defined(Q_OS_NETBSD) || defined(Q_OS_OPENBSD) || defined(Q_OS_SOLARIS)) && !defined(Q_OS_ANDROID)
    try{
        if(QThread* mainThread = QCoreApplicationPrivate::theMainThread.loadRelaxed()){
            std::unique_ptr<SignalCache> cache{nullptr};
            {
                QWriteLocker locker(QtJambiLinkUserData::lock());
                cache.reset(QTJAMBI_GET_OBJECTUSERDATA(SignalCache, mainThread));
                QTJAMBI_SET_OBJECTUSERDATA(SignalCache, mainThread, nullptr);
            }
            if(cache){
                for(int s : SignalCache::signals()){
                    struct sigaction sa;
                    struct sigaction sa2;
                    memset(&sa, 0, sizeof(sa));
                    memset(&sa2, 0, sizeof(sa2));
                    sa.sa_flags = cache->handlers[s].first;
                    sa.sa_handler = cache->handlers[s].second;
                    if(sigaction(s, &sa, &sa2)==0){
                        if(sa.sa_handler!=sa2.sa_handler)
                            qCDebug(DebugAPI::internalCategory, "Signalhandler for %d restored.", s);
                        else
                            qCDebug(DebugAPI::internalCategory, "Signalhandler for %d unchanged.", s);
                    }else
                        qCDebug(DebugAPI::internalCategory, "Unable to restore signalhandler for %d.", s);
                }
                return true;
            }
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
#else
    Q_UNUSED(env)
#endif
    return false;
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QtUtilities_setDanglingPointerCheckEnabled)(JNIEnv *env, jclass, jboolean enabled)
{
    try{
        const char* property = "io.qt.enable-dangling-pointer-check";
        Java::Runtime::System::setProperty(env, env->NewStringUTF(property), env->NewStringUTF(enabled ? "true" : "false"));
        reinitializeResettableFlag(env, property);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QtUtilities_setMethodLogsEnabled)(JNIEnv *env, jclass, jboolean enabled)
{
    try{
        const char* property = "io.qt.enable-method-logs";
        Java::Runtime::System::setProperty(env, env->NewStringUTF(property), env->NewStringUTF(enabled ? "true" : "false"));
        reinitializeResettableFlag(env, property);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QtUtilities_setEventLogsEnabled)(JNIEnv *env, jclass, jboolean enabled)
{
    try{
        const char* property = "io.qt.enable-event-logs";
        Java::Runtime::System::setProperty(env, env->NewStringUTF(property), env->NewStringUTF(enabled ? "true" : "false"));
        reinitializeResettableFlag(env, property);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QtUtilities_setThreadAffinityCheckEnabled)(JNIEnv *env, jclass, jboolean enabled)
{
    try{
        const char* property = "io.qt.enable-thread-affinity-check";
        Java::Runtime::System::setProperty(env, env->NewStringUTF(property), env->NewStringUTF(enabled ? "true" : "false"));
        enableThreadAffinity(enabled);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QtUtilities_setSignalEmitThreadCheckEnabled)(JNIEnv *env, jclass, jboolean enabled)
{
    try{
        const char* property = "io.qt.enable-signal-emit-thread-check";
        Java::Runtime::System::setProperty(env, env->NewStringUTF(property), env->NewStringUTF(enabled ? "true" : "false"));
        enableSignalEmitThreadCheck(enabled);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QtUtilities_setNoExceptionForwardingFromMetaCallsEnabled)(JNIEnv *env, jclass, jboolean enabled)
{
    try{
        const char* property = "io.qt.no-exception-forwarding-from-meta-calls";
        Java::Runtime::System::setProperty(env, env->NewStringUTF(property), env->NewStringUTF(enabled ? "true" : "false"));
        reinitializeResettableFlag(env, property);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QtUtilities_setNoExceptionForwardingFromVirtualCallsEnabled)(JNIEnv *env, jclass, jboolean enabled)
{
    try{
        const char* property = "io.qt.no-exception-forwarding-from-virtual-calls";
        Java::Runtime::System::setProperty(env, env->NewStringUTF(property), env->NewStringUTF(enabled ? "true" : "false"));
        reinitializeResettableFlag(env, property);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

void installSignalEmitThreadCheckHandler(JNIEnv *env, jobject handler);

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QtUtilities_installSignalEmitThreadCheckHandler)(JNIEnv *env, jclass, jobject handler)
{
    try{
        installSignalEmitThreadCheckHandler(env, handler);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QtUtilities_setEventThreadAffinityCheckEnabled)(JNIEnv *env, jclass, jboolean enabled)
{
    try{
        const char* property = "io.qt.enable-event-thread-affinity-check";
        Java::Runtime::System::setProperty(env, env->NewStringUTF(property), env->NewStringUTF(enabled ? "true" : "false"));
        if(enabled){
            QInternal::unregisterCallback(QInternal::EventNotifyCallback, &simpleEventNotify);
            QInternal::unregisterCallback(QInternal::EventNotifyCallback, &threadAffineEventNotify);
            QInternal::registerCallback(QInternal::EventNotifyCallback, &threadAffineEventNotify);
        }else{
            QInternal::unregisterCallback(QInternal::EventNotifyCallback, &simpleEventNotify);
            QInternal::unregisterCallback(QInternal::EventNotifyCallback, &threadAffineEventNotify);
            QInternal::registerCallback(QInternal::EventNotifyCallback, &simpleEventNotify);
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}
