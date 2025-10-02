/****************************************************************************
**
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
#include "qtjambimetaobject_p.h"
#include "qtjambishell_p.h"
#if QT_VERSION < QT_VERSION_CHECK(6, 8, 0) \
               || ( (defined(Q_OS_LINUX) \
                    || defined(Q_OS_MACOS) \
                    || defined(Q_OS_FREEBSD) \
                    || defined(Q_OS_NETBSD) \
                    || defined(Q_OS_OPENBSD) \
                    || defined(Q_OS_SOLARIS) ) \
               && !defined(Q_OS_ANDROID))
#include "threadutils_p.h"
#endif
#ifdef Q_OS_ANDROID
#include "androidapi.h"
#endif
#include "qtjambi_cast.h"

void shutdown(JNIEnv * env, bool regular);

extern "C" JNIEXPORT void JNICALL Java_io_qt_internal_QtJambi_1LibraryUtilities_shutdown(JNIEnv * env, jclass)
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

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_QtJambi_1LibraryUtilities_internalAccess(JNIEnv *env, jclass cls){
    try{
        return MetaInfoAPI::internalAccess(env, cls);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_internal_ExceptionUtility_convertNativeException(JNIEnv *env, jclass, jlong exception){
    if(exception){
        const std::exception* exn = reinterpret_cast<const std::exception*>(exception);
        const std::type_info* ti = QtJambiPrivate::CheckPointer<std::exception>::trySupplyType(exn);
        if(ti && typeid_equals(*ti, typeid(JavaException))){
            const JavaException* jexn = reinterpret_cast<const JavaException*>(exception);
            return jexn->throwable(env);
        }else{
            return Java::Runtime::RuntimeException::newInstanceWithMessage(env, exn->what());
        }
    }
    return nullptr;
}

extern "C" JNIEXPORT jboolean JNICALL Java_io_qt_QtUtilities_putenv(JNIEnv *env, jclass, jstring varName, jstring value){
    try{
        J2CStringBuffer _varName(env, varName);
        J2CStringBuffer _value(env, value);
        return qputenv(_varName.data(), _value.data());
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return false;
}

extern "C" JNIEXPORT jboolean JNICALL Java_io_qt_internal_ReferenceUtility_needsReferenceCounting(JNIEnv *env, jclass, QtJambiNativeID objId){
    try{
        if (QSharedPointer<QtJambiLink> link = QtJambiLink::fromNativeId(objId)) {
            return link->needsReferenceCounting() || link->ownership()==QtJambiLink::Ownership::Java;
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return false;
}

void addClassPath(const QString& path, bool isDirectory);
void removeResource(const QString& path);
void insertJarFileResources(JNIEnv *env, jobject entryPaths, jstring _jarFileName);
void ensureHandler(JNIEnv* env, jstring qtJambiConfFile);

extern "C" JNIEXPORT void JNICALL Java_io_qt_internal_ResourceUtility_ensureHandler(JNIEnv *env,jclass, jstring qtJambiConfFile){
    try{
        ensureHandler(env, qtJambiConfFile);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

extern "C" JNIEXPORT jstring JNICALL Java_io_qt_internal_ResourceUtility_androidBundlePath(JNIEnv *env,jclass){
    try{
#ifdef Q_OS_ANDROID
        if(jobject context = Java::Android::QtNative::getContext(env)){
            if(jobject appInfo = Java::Android::Context::getApplicationInfo(env, context)){
                return Java::Android::ApplicationInfo::sourceDir(env, appInfo);
            }
        }
#endif
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT void JNICALL Java_io_qt_internal_ResourceUtility_addClassPath(JNIEnv *env,jclass, jstring path, jboolean isDirectory){
    try{
        addClassPath(qtjambi_cast<QString>(env, path), isDirectory);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

extern "C" JNIEXPORT void JNICALL Java_io_qt_internal_ResourceUtility_insertJarFileResources(JNIEnv *env,jclass, jobject entryPaths, jstring jarFileName){
    try{
        insertJarFileResources(env, entryPaths, jarFileName);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

extern "C" JNIEXPORT void JNICALL Java_io_qt_internal_ResourceUtility_removeResource(JNIEnv *env,jclass, jstring path){
    try{
        removeResource(qtjambi_cast<QString>(env, path));
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

extern "C" JNIEXPORT jint JNICALL Java_io_qt_internal_LibraryUtility_qtJambiVersion(JNIEnv *,jclass){
    return QT_VERSION_CHECK(QT_VERSION_MAJOR, QT_VERSION_MINOR, QTJAMBI_PATCH_VERSION);
}

extern "C" JNIEXPORT jstring JNICALL Java_io_qt_internal_LibraryUtility_libraryFilePath(JNIEnv *env, jclass){
    try{
        QString path = getFunctionLibraryPath(reinterpret_cast<QFunctionPointer>(&Java_io_qt_internal_LibraryUtility_libraryFilePath));
        return qtjambi_cast<jstring>(env, path);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jclass JNICALL Java_io_qt_internal_AccessUtility_getClass(JNIEnv * env, jobject, jobject e){
    return e ? env->GetObjectClass(e) : nullptr;
}

extern "C" JNIEXPORT jboolean JNICALL Java_io_qt_internal_ClassAnalyzerUtility_isGeneratedClass(JNIEnv *env, jclass, jstring className){
    try{
        QString cn = qtjambi_cast<QString>(env, className).replace(".", "/");
        return getTypeByJavaName(cn)!=nullptr || isJavaNameNamespace(cn);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return false;
}

extern "C" JNIEXPORT jboolean JNICALL Java_io_qt_QtUtilities_unsetenv(JNIEnv *env, jclass, jstring varName){
    try{
        J2CStringBuffer _varName(env, varName);
        return qunsetenv(_varName.data());
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return false;
}

extern "C" JNIEXPORT jstring JNICALL Java_io_qt_QtUtilities_getenv(JNIEnv *env, jclass, jstring varName){
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

extern "C" JNIEXPORT void JNICALL Java_io_qt_QtConstructInPlace_initialize(JNIEnv *env, jclass, jlong initializer, jobject object){
    try{
        if(!initializer)
            Java::QtJambi::QNoNativeResourcesException::throwNew(env, "in-place constructor disposed" QTJAMBI_STACKTRACEINFO );
        QtJambiShellImpl::initializeNativeInterface(env, object, reinterpret_cast<InPlaceInitializer*>(initializer));
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

extern "C" JNIEXPORT void JNICALL Java_io_qt_QtConstructInPlace_initWithArguments(JNIEnv *env, jclass, jlong initializer, jobject object, jintArray indexes){
    try{
        if(!initializer){
            Java::QtJambi::QNoNativeResourcesException::throwNew(env, "in-place constructor disposed" QTJAMBI_STACKTRACEINFO );
            return;
        }
        QtJambiScope scope;
        SuperInitializer* si = reinterpret_cast<InPlaceInitializer*>(initializer)->asArguments(env, qtjambi_cast<std::initializer_list<int>>(env, scope, indexes), false);
        QtJambiShellImpl::initializeNativeInterface(env, object, si);
        reinterpret_cast<InPlaceInitializer*>(initializer)->reset(env);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

extern "C" JNIEXPORT void JNICALL Java_io_qt_QtConstructInPlace_initFromStream(JNIEnv *env, jclass, jlong initializer, jobject object, jobject arguments){
    try{
        if(!initializer){
            Java::QtJambi::QNoNativeResourcesException::throwNew(env, "in-place constructor disposed" QTJAMBI_STACKTRACEINFO );
            return;
        }
        QtJambiScope scope;
        QtJambiShellImpl::initializeNativeInterface(env, object, reinterpret_cast<InPlaceInitializer*>(initializer), arguments);
        reinterpret_cast<InPlaceInitializer*>(initializer)->reset(env);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

extern "C" JNIEXPORT jint JNICALL Java_io_qt_QtConstructInPlace_parameterCount(JNIEnv *env, jclass, jlong ptr){
    try{
        if(!ptr)
            Java::QtJambi::QNoNativeResourcesException::throwNew(env, "in-place constructor disposed" QTJAMBI_STACKTRACEINFO );
        else return reinterpret_cast<InPlaceInitializer*>(ptr)->parameterCount();
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return 0;
}

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_QtConstructInPlace_argumentAt(JNIEnv *env, jclass, jlong ptr, jint index){
    try{
        if(!ptr)
            Java::QtJambi::QNoNativeResourcesException::throwNew(env, "in-place constructor disposed" QTJAMBI_STACKTRACEINFO );
        else return reinterpret_cast<InPlaceInitializer*>(ptr)->argumentAt(env, index);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jclass JNICALL Java_io_qt_QtConstructInPlace_parameterTypeAt(JNIEnv *env, jclass, jlong ptr, jint index){
    try{
        if(!ptr)
            Java::QtJambi::QNoNativeResourcesException::throwNew(env, "in-place constructor disposed" QTJAMBI_STACKTRACEINFO );
        else return reinterpret_cast<InPlaceInitializer*>(ptr)->parameterTypeAt(env, index);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_QtConstructInPlace_asArguments(JNIEnv *env, jclass, jlong ptr, jintArray indexes){
    try{
        if(!ptr)
            Java::QtJambi::QNoNativeResourcesException::throwNew(env, "in-place constructor disposed" QTJAMBI_STACKTRACEINFO );
        else{
            QtJambiScope scope;
            SuperInitializer* si = reinterpret_cast<InPlaceInitializer*>(ptr)->asArguments(env, qtjambi_cast<std::initializer_list<int>>(env, scope, indexes));
            return si ? si->inPlaceObject(env) : nullptr;
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT bool JNICALL Java_io_qt_QtUtilities_reinstallEventNotifyCallback(JNIEnv * env){
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

extern "C" JNIEXPORT void JNICALL Java_io_qt_QtUtilities_uiThreadCheck(JNIEnv *env, jclass){
    try{
        QtJambiAPI::checkMainThread(env, typeid(void));
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

extern "C" JNIEXPORT void JNICALL Java_io_qt_QtUtilities_threadCheck(JNIEnv *env, jclass, jobject object){
    try{
        checkThreadOnQObject(env, object);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
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

extern "C" JNIEXPORT bool JNICALL Java_io_qt_QtUtilities_saveUnixSignalHandlers(JNIEnv *env, jclass){
#if (defined(Q_OS_LINUX) || defined(Q_OS_MACOS) || defined(Q_OS_FREEBSD) || defined(Q_OS_NETBSD) || defined(Q_OS_OPENBSD) || defined(Q_OS_SOLARIS)) && !defined(Q_OS_ANDROID)
    try{
        QThread* mainThread = QCoreApplicationPrivate::theMainThread.loadRelaxed();
        if(!mainThread){
            ThreadPrivate::initializeMainThread(env);
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

extern "C" JNIEXPORT bool JNICALL Java_io_qt_QtUtilities_restoreUnixSignalHandlers(JNIEnv *env, jclass){
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

extern "C" JNIEXPORT void JNICALL Java_io_qt_QtUtilities_setDanglingPointerCheckEnabled(JNIEnv *env, jclass, jboolean enabled){
    try{
        const char* property = "io.qt.enable-dangling-pointer-check";
        Java::Runtime::System::setProperty(env, env->NewStringUTF(property), env->NewStringUTF(enabled ? "true" : "false"));
        reinitializeResettableFlag(env, property);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

extern "C" JNIEXPORT void JNICALL Java_io_qt_QtUtilities_setMethodLogsEnabled(JNIEnv *env, jclass, jboolean enabled){
    try{
        const char* property = "io.qt.enable-method-logs";
        Java::Runtime::System::setProperty(env, env->NewStringUTF(property), env->NewStringUTF(enabled ? "true" : "false"));
        reinitializeResettableFlag(env, property);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

extern "C" JNIEXPORT void JNICALL Java_io_qt_QtUtilities_setEventLogsEnabled(JNIEnv *env, jclass, jboolean enabled){
    try{
        const char* property = "io.qt.enable-event-logs";
        Java::Runtime::System::setProperty(env, env->NewStringUTF(property), env->NewStringUTF(enabled ? "true" : "false"));
        reinitializeResettableFlag(env, property);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

extern "C" JNIEXPORT void JNICALL Java_io_qt_QtUtilities_setThreadAffinityCheckEnabled(JNIEnv *env, jclass, jboolean enabled){
    try{
        const char* property = "io.qt.enable-thread-affinity-check";
        Java::Runtime::System::setProperty(env, env->NewStringUTF(property), env->NewStringUTF(enabled ? "true" : "false"));
        enableThreadAffinity(enabled);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

extern "C" JNIEXPORT void JNICALL Java_io_qt_QtUtilities_setSignalEmitThreadCheckEnabled(JNIEnv *env, jclass, jboolean enabled){
    try{
        const char* property = "io.qt.enable-signal-emit-thread-check";
        Java::Runtime::System::setProperty(env, env->NewStringUTF(property), env->NewStringUTF(enabled ? "true" : "false"));
        enableSignalEmitThreadCheck(enabled);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

extern "C" JNIEXPORT void JNICALL Java_io_qt_QtUtilities_setNoExceptionForwardingFromMetaCallsEnabled(JNIEnv *env, jclass, jboolean enabled){
    try{
        const char* property = "io.qt.no-exception-forwarding-from-meta-calls";
        Java::Runtime::System::setProperty(env, env->NewStringUTF(property), env->NewStringUTF(enabled ? "true" : "false"));
        reinitializeResettableFlag(env, property);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

extern "C" JNIEXPORT void JNICALL Java_io_qt_QtUtilities_setNoExceptionForwardingFromVirtualCallsEnabled(JNIEnv *env, jclass, jboolean enabled){
    try{
        const char* property = "io.qt.no-exception-forwarding-from-virtual-calls";
        Java::Runtime::System::setProperty(env, env->NewStringUTF(property), env->NewStringUTF(enabled ? "true" : "false"));
        reinitializeResettableFlag(env, property);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

void installSignalEmitThreadCheckHandler(JNIEnv *env, jobject handler);

extern "C" JNIEXPORT void JNICALL Java_io_qt_QtUtilities_installSignalEmitThreadCheckHandler(JNIEnv *env, jclass, jobject handler){
    try{
        installSignalEmitThreadCheckHandler(env, handler);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

extern "C" JNIEXPORT void JNICALL Java_io_qt_QtUtilities_setEventThreadAffinityCheckEnabled(JNIEnv *env, jclass, jboolean enabled){
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
