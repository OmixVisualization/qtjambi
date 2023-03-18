/****************************************************************************
**
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
#include <QtCore/private/qcoreapplication_p.h>
#if (defined(Q_OS_LINUX) || defined(Q_OS_MACOS)) && !defined(Q_OS_ANDROID)
#include <signal.h>
#include <stdio.h>
#include <map>
#include <initializer_list>
#include "threadapi.h"
#endif
#include "java_p.h"
#include "utils_p.h"
#include "qtjambilink_p.h"
#include "qtjambi_cast.h"

void shutdown(JNIEnv * env);

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambi_1LibraryUtilities_shutdown)
(JNIEnv * env, jclass)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QtJambi_LibraryUtilities::shutdown()")
    try{
        if(QThread* mainThread = QCoreApplicationPrivate::theMainThread.loadRelaxed()){
            QThread* currentThread = QThread::currentThread();
            if(mainThread==currentThread){
                if(QCoreApplication::instance() && !Java::Runtime::Boolean::getBoolean(env, env->NewStringUTF("io.qt.no-app-deletion"))){
                    delete QCoreApplication::instance();
                }
            }
        }
        shutdown(env);
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

#ifdef Q_OS_ANDROID
#define unique_id(id) qHash(QLatin1String((id).name()))
#else
#define unique_id(id) (id).hash_code()
#endif

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_ExceptionUtility_convertNativeException)
(JNIEnv *env,
 jclass,
 jlong exception)
{
    if(exception){
        const std::exception* exn = reinterpret_cast<const std::exception*>(exception);
        if(unique_id(typeid(*exn))==unique_id(typeid(JavaException))){
            const JavaException* jexn = reinterpret_cast<const JavaException*>(exception);
            return jexn->object();
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
            return link->createdByJava() || link->ownership()==QtJambiLink::Ownership::Java;
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return false;
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
            if(QThread::currentThread()->loopLevel()>0){
                Java::Runtime::IllegalStateException::throwNew(env, "Unable to reinstall event notify callback with running application event loop." QTJAMBI_STACKTRACEINFO );
            }
        }else{
            Java::Runtime::IllegalStateException::throwNew(env, "Unable to reinstall event notify callback without main thread." QTJAMBI_STACKTRACEINFO );
        }
        if(QInternal::unregisterCallback(QInternal::EventNotifyCallback, &simpleEventNotify)){
            return QInternal::registerCallback(QInternal::EventNotifyCallback, &simpleEventNotify);
        }else if(QInternal::unregisterCallback(QInternal::EventNotifyCallback, &threadAffineEventNotify)){
            return QInternal::registerCallback(QInternal::EventNotifyCallback, &threadAffineEventNotify);
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
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QtUtilities_threadCheck)(JNIEnv *env, jclass, QtJambiNativeID objectId)
{
    try{
        QtJambiAPI::checkThread(env, QtJambiAPI::objectFromNativeId<QObject>(objectId));
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

#if (defined(Q_OS_LINUX) || defined(Q_OS_MACOS)) && !defined(Q_OS_ANDROID)
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
#if (defined(Q_OS_LINUX) || defined(Q_OS_MACOS)) && !defined(Q_OS_ANDROID)
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
                    qDebug("Signalhandler for %d saved.", s);
                    cache->handlers[s] = {sa.sa_flags, sa.sa_handler};
                }else
                    qDebug("Unable to save signalhandler for %d.", s);
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
#if (defined(Q_OS_LINUX) || defined(Q_OS_MACOS)) && !defined(Q_OS_ANDROID)
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
                            qDebug("Signalhandler for %d restored.", s);
                        else
                            qDebug("Signalhandler for %d unchanged.", s);
                    }else
                        qDebug("Unable to restore signalhandler for %d.", s);
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
