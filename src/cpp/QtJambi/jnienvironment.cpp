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

#include <QtCore/QObject>
#include <QtCore/QAbstractEventDispatcher>
#include "jnienvironment.h"
#include "java_p.h"
#include "coreapi.h"
#include "threadutils_p.h"
#include "qtjambilink_p.h"
#include "qtjambi_cast.h"

#ifndef JNI_VERSION_1_8
#define JNI_VERSION_1_8 JNI_VERSION_1_6
#endif

#if QT_VERSION >= QT_VERSION_CHECK(6, 6, 0)
#define qAsConst std::as_const
#endif

#define QTJAMBI_INTERNAL_METHOD_CALL(methodname)\
DebugAPI::MethodPrint __debug_method_print(DebugAPI::MethodPrint::Internal, methodname, __FILE__, __LINE__, Q_FUNC_INFO);

Q_GLOBAL_STATIC_WITH_ARGS(std::atomic<JavaVM *>, gJavaVM, (nullptr))

Q_GLOBAL_STATIC_WITH_ARGS(std::atomic<bool>, gIsJVMLoaded, (false))

typedef QHash<ResettableBoolFlag*,const char*> ResettableFlagsHash;
Q_GLOBAL_STATIC(ResettableFlagsHash, gResettableFlags)
Q_GLOBAL_STATIC(QMutex, gResettableFlagsMutex)

std::atomic<bool>* getJVMLoaded(){
    return gIsJVMLoaded();
}

std::atomic<JavaVM *>* getJVM(){
    return gJavaVM();
}

void registerResettableFlag(ResettableBoolFlag* flag, const char* property){
    QMutexLocker locker(gResettableFlagsMutex());
    (*gResettableFlags)[flag] = property;
}

void unregisterResettableFlag(ResettableBoolFlag* flag){
    if(!gResettableFlagsMutex.isDestroyed()){
        QMutexLocker locker(gResettableFlagsMutex());
        gResettableFlags->remove(flag);
    }
}

void clearResettableFlags(){
    ResettableFlagsHash envPointers;
    if(!gResettableFlagsMutex.isDestroyed()){
        QMutexLocker locker(gResettableFlagsMutex());
        envPointers.swap(*gResettableFlags);
    }
    for(auto iter = envPointers.keyValueBegin(), end = envPointers.keyValueEnd(); iter!=end; ++iter){
        iter->first->value = false;
    }
}

void reinitializeResettableFlag(JNIEnv * env, const char* property){
    ResettableFlagsHash envPointers;
    if(!gResettableFlagsMutex.isDestroyed()){
        QMutexLocker locker(gResettableFlagsMutex());
        envPointers = *gResettableFlags;
    }
    for(auto iter = envPointers.keyValueBegin(), end = envPointers.keyValueEnd(); iter!=end; ++iter){
        if(qstrcmp(property, iter->second)==0)
            iter->first->reinitialize(env, iter->second);
    }
}

#ifdef Q_OS_ANDROID
void reinitializeResettableFlags(JNIEnv * env){
    ResettableFlagsHash envPointers;
    if(!gResettableFlagsMutex.isDestroyed()){
        QMutexLocker locker(gResettableFlagsMutex());
        envPointers = *gResettableFlags;
    }
    for(auto iter = envPointers.keyValueBegin(), end = envPointers.keyValueEnd(); iter!=end; ++iter){
        iter->first->reinitialize(env, iter->second);
    }
}
#endif

namespace ThreadPrivate{

void thread_detacher_cleaner(EventDispatcherCheck::Data* threadData) {
     struct ThreadDetacher : public QtJambiObjectData {
         EventDispatcherCheck::Data* m_threadData;
         ThreadDetacher(EventDispatcherCheck::Data* threadData)
             : QtJambiObjectData(),
               m_threadData(threadData)
         {
             QTJAMBI_INTERNAL_METHOD_CALL("ThreadDetacher::ThreadDetacher()")
         }
         ~ThreadDetacher() override {
             QTJAMBI_INTERNAL_METHOD_CALL("ThreadDetacher::~ThreadDetacher()")
             JavaVM *vm = nullptr;
             if(std::atomic<JavaVM *>* atm = gJavaVM)
                 vm = atm->load();
             if (vm) {
                  QSharedPointer<QtJambiLink> link;
                  {
                      QReadLocker locker(QtJambiLinkUserData::lock());
                      link = m_threadData->m_wlink.toStrongRef();
                      m_threadData->m_wlink = QWeakPointer<QtJambiLink>{};
                  }
                  if(link || m_threadData->m_jthreadObject){
                        JNIEnv* env = nullptr;
                        int result = vm->GetEnv( reinterpret_cast<void **>(&env), JNI_VERSION_1_8);
                        if(result==JNI_OK &&  env){
                            JniLocalFrame __jniLocalFrame(env, 200);
                            if(link){
                                Q_ASSERT(link->isQObject());
                                if(!link->qobject() || !link->qobject()->parent())
                                   link->setJavaOwnership(env);
                            }
                            if(m_threadData->m_jthreadObject){
                                jobject jthreadObject = env->NewLocalRef(m_threadData->m_jthreadObject);
                                env->DeleteWeakGlobalRef(m_threadData->m_jthreadObject);
                                m_threadData->m_jthreadObject = nullptr;
                                if(jthreadObject){
                                    try{
                                        Java::QtJambi::ThreadUtility::setThreadInterruptible(env, nullptr, jthreadObject, false);
                                    } catch (const JavaException& exn) {
                                        exn.report(env);
                                    }
                                    try {
                                        Java::QtJambi::NativeUtility::deleteAssociation(env, jthreadObject);
                                    } catch (const JavaException& exn) {
                                        exn.report(env);
                                    }
                                    env->DeleteLocalRef(jthreadObject);
                                }
                            }
                        }else{
                            class ThreadPurger : public QObject{
                                QSharedPointer<QtJambiLink> m_link;
                                jweak m_jthreadObject;
                            public:
                                ThreadPurger(QSharedPointer<QtJambiLink> &&link, jweak jthreadObject)
                                    : QObject(),
                                    m_link(std::move(link)),
                                    m_jthreadObject(jthreadObject){
                                    QTJAMBI_INTERNAL_METHOD_CALL("ThreadPurger::ThreadPurger()")
                                }
                                ~ThreadPurger() override{
                                    QTJAMBI_INTERNAL_METHOD_CALL("ThreadPurger::~ThreadPurger()")
                                    if(JniEnvironment env{200}){
                                        if(m_link){
                                            Q_ASSERT(m_link->isQObject());
                                            if(!m_link->qobject() || !m_link->qobject()->parent())
                                                m_link->setJavaOwnership(env);
                                        }
                                        if(m_jthreadObject){
                                            jobject jthreadObject = env->NewLocalRef(m_jthreadObject);
                                            env->DeleteWeakGlobalRef(m_jthreadObject);
                                            m_jthreadObject = nullptr;
                                            if(jthreadObject){
                                                try{
                                                    Java::QtJambi::ThreadUtility::setThreadInterruptible(env, nullptr, jthreadObject, false);
                                                } catch (const JavaException& exn) {
                                                    exn.report(env);
                                                }
                                                try {
                                                    Java::QtJambi::NativeUtility::deleteAssociation(env, jthreadObject);
                                                } catch (const JavaException& exn) {
                                                    exn.report(env);
                                                }
                                                env->DeleteLocalRef(jthreadObject);
                                            }
                                        }
                                    }else{
                                        QTJAMBI_DEBUG_PRINT("Unable to cleanup thread due to missing JNI")
                                    }
                                }
                            };
                            ThreadPurger* referenceDeleter = new ThreadPurger(std::move(link), m_threadData->m_jthreadObject);
                            referenceDeleter->moveToThread(QCoreApplicationPrivate::mainThread());
                            referenceDeleter->deleteLater();
                            m_threadData->m_wlink = QWeakPointer<QtJambiLink>{};
                            m_threadData->m_jthreadObject = nullptr;
                        }
                  }
                  while(!m_threadData->m_finalActions.isEmpty()){
                      m_threadData->m_finalActions.takeFirst()();
                  }
                  {
                      QWriteLocker locker(QtJambiLinkUserData::lock());
                      if(QThreadUserData* data = QTJAMBI_GET_OBJECTUSERDATA(QThreadUserData, m_threadData->m_thread.data())){
                          if(data->purgeOnExit()){
                              QTJAMBI_SET_OBJECTUSERDATA(QThreadUserData, m_threadData->m_thread.data(), nullptr);
                              locker.unlock();
                              delete data;
                              locker.relock();
                          }
                      }
                  }
                  vm->DetachCurrentThread();
             }else{
                 while(!m_threadData->m_finalActions.isEmpty()){
                     m_threadData->m_finalActions.takeFirst()();
                 }

                 {
                      QWriteLocker locker(QtJambiLinkUserData::lock());
                      if(QThreadUserData* data = QTJAMBI_GET_OBJECTUSERDATA(QThreadUserData, m_threadData->m_thread.data())){
                          if(data->purgeOnExit()){
                              QTJAMBI_SET_OBJECTUSERDATA(QThreadUserData, m_threadData->m_thread.data(), nullptr);
                              locker.unlock();
                              delete data;
                              locker.relock();
                          }
                      }
                 }
             }
             delete m_threadData;
         }
         QTJAMBI_OBJECTUSERDATA_ID_IMPL(static,)
     };
     QAbstractEventDispatcher * eventDispatcher = nullptr;
     if(threadData->m_thread && !(eventDispatcher = threadData->m_thread->eventDispatcher())){
          if (QThread::currentThread()==threadData->m_thread && (QCoreApplication::instance() || !QCoreApplicationPrivate::threadRequiresCoreApplication())) {
              QEventLoop loop;
              eventDispatcher = threadData->m_thread->eventDispatcher();
          }
     }
     if(eventDispatcher){
           QWriteLocker locker(QtJambiLinkUserData::lock());
           QTJAMBI_SET_OBJECTUSERDATA_ID(QTJAMBI_OBJECTUSERDATA_TYPE_ID(ThreadDetacher), eventDispatcher, new ThreadDetacher(threadData));
     }else{
           ThreadDetacher releaser(threadData);
           Q_UNUSED(releaser)
     }
}

} // namespace ThreadPrivate

JNIEnv *currentJNIEnvironment(JavaVM *vm, bool initializeJavaThread = true){
    JNIEnv *env = nullptr;
    int result = vm->GetEnv( reinterpret_cast<void **>(&env), JNI_VERSION_1_8);
    if (result == JNI_EDETACHED) {
        if(std::atomic<bool>* atm = gIsJVMLoaded){
            if(!atm->load())
                return nullptr;
        }else return nullptr;
        QThread* currentThread = QThread::currentThread();
        if(!currentThread)// this is possible during thread destruction.
            return nullptr;
        jobject uncaughtExceptionHandler = nullptr;
        jobject contextClassLoader = nullptr;
        QThreadUserData* threadData;
        {
            QReadLocker locker(QtJambiLinkUserData::lock());
            threadData = QTJAMBI_GET_OBJECTUSERDATA(QThreadUserData, currentThread);
        }
        if(threadData){
            uncaughtExceptionHandler = threadData->getUncaughtExceptionHandler();
            contextClassLoader = threadData->getContextClassLoader();
            QByteArray name(threadData->getName());
            JavaVMAttachArgs args;
            args.version = JNI_VERSION_1_8;
            args.name = name.isEmpty() ? nullptr : name.data();
            args.group = threadData->getThreadGroup();
            JavaVMAttachArgs* _args = args.name && args.group ? &args : nullptr;
            if(threadData->isDaemon())
#ifdef Q_OS_ANDROID
                result = vm->AttachCurrentThreadAsDaemon(&env, _args);
            else
                result = vm->AttachCurrentThread(&env, _args);
#else
                result = vm->AttachCurrentThreadAsDaemon(reinterpret_cast<void **>(&env), _args);
            else
                result = vm->AttachCurrentThread(reinterpret_cast<void **>(&env), _args);
#endif
            if (result != JNI_OK && _args) {
#ifdef Q_OS_ANDROID
              result = vm->AttachCurrentThreadAsDaemon(&env, nullptr);
#else
              result = vm->AttachCurrentThreadAsDaemon(reinterpret_cast<void **>(&env), nullptr);
#endif
            }
        }else{
#ifdef Q_OS_ANDROID
            result = vm->AttachCurrentThreadAsDaemon(&env, nullptr);
#else
            result = vm->AttachCurrentThreadAsDaemon(reinterpret_cast<void **>(&env), nullptr);
#endif
        }
        if (result != JNI_OK) {
            if(QThread* mainThread = QCoreApplicationPrivate::theMainThread.loadRelaxed()){
                if(mainThread!=currentThread)
                    qCWarning(DebugAPI::internalCategory, "Failed attaching current thread (%i).", result);
            }else{
                qCWarning(DebugAPI::internalCategory, "Failed attaching current thread (%i).", result);
            }
        }
        if(env && env->functions){
            if(threadData)
                threadData->clearThreadGroup(env);
            if(initializeJavaThread){
                jobject jthread = Java::Runtime::Thread::currentThread(env);
                if(uncaughtExceptionHandler){
                    Java::Runtime::Thread::setUncaughtExceptionHandler(env, jthread, uncaughtExceptionHandler);
                    threadData->clearUncaughtExceptionHandler(env);
                    uncaughtExceptionHandler = nullptr;
                }
                if(contextClassLoader){
                    Java::Runtime::Thread::setContextClassLoader(env, jthread, contextClassLoader);
                    threadData->clearContextClassLoader(env);
                    contextClassLoader = nullptr;
                }
                if(jobject qthreadObject = qtjambi_cast<jobject>(env, currentThread)){
                    QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, qthreadObject);
                    link->setNoThreadInitializationOnPurge(true);
                    if(jobject jthrd = Java::QtCore::QThread::javaThread(env, qthreadObject)){
                        env->DeleteLocalRef(jthrd);
                    }else{
                        Java::QtCore::QThread::set_javaThread(env, qthreadObject, jthread);
                        Java::QtJambi::NativeUtility::createAssociation(env, jthread, qthreadObject);
                        Java::QtJambi::ThreadUtility::setThreadInterruptible(env, qthreadObject, jthread, true);
                        if(!currentThread->parent()){
                            link->setJavaOwnership(env);
                        }

                        //ThreadCleanup::instance.initialize(env, jthread, currentThread, link, false);
                        // detach the QThread from Java
                        // After emitting QThread::finished the QThreadStorage data is cleared.
                        // Thereafter, the event dispatcher is deleted if available.
                        // Let QThreadStorage-deletion check if an event dispatcher is available and create an empty event dispatcher otherwise.
                        // During deletion of the event dispatcher (which is the latest action in a thread) the java thread is detached.
                        new EventDispatcherCheck(
                            env, jthread,
                            currentThread,
                            link,
                            &ThreadPrivate::thread_detacher_cleaner);
                    }
                    if(env->GetObjectRefType(qthreadObject)==jobjectRefType::JNILocalRefType){
                        env->DeleteLocalRef(qthreadObject);
                        qthreadObject = nullptr;
                    }
                }
                env->DeleteLocalRef(jthread);
                jthread = nullptr;
            }else if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForQObject(currentThread)){
                link->setNoThreadInitializationOnPurge(true);
            }
        }
        bool init = false;
        if(!threadData){
            QWriteLocker locker(QtJambiLinkUserData::lock());
            threadData = QTJAMBI_GET_OBJECTUSERDATA(QThreadUserData, currentThread);
            if(!threadData){
                threadData = new QThreadUserData();
                init = true;
                QTJAMBI_SET_OBJECTUSERDATA(QThreadUserData, currentThread, threadData);
            }
        }
        if(init)
            threadData->initialize(currentThread);
    } else {
        Q_ASSERT(result == JNI_OK);
    }
    if(env && !env->functions)
        env = nullptr;
    return env;
}

struct JNIEnvKeeper{
    JNIEnvKeeper();
    ~JNIEnvKeeper();
    JNIEnv *env = nullptr;
};

JNIEnvKeeper::JNIEnvKeeper(){
    JavaVM *vm = nullptr;
    if(std::atomic<JavaVM *>* atm = gJavaVM)
        vm = atm->load();
    if (vm)
        env = currentJNIEnvironment(vm, true);
}

JNIEnvKeeper::~JNIEnvKeeper(){
}

JNIEnv *fastJNIEnvironment(){
    static thread_local JNIEnvKeeper sEnvironment;
    return sEnvironment.env;
}

ResettableBoolFlag::operator bool() { return value; }

ResettableBoolFlag::ResettableBoolFlag(const char* property) : value(0)
{
    registerResettableFlag(this, property);
    JavaVM *vm = nullptr;
    if(std::atomic<JavaVM *>* atm = gJavaVM)
        vm = atm->load();
    if (vm){
        JNIEnv* env = currentJNIEnvironment(vm, true);
        if(env){
            JniLocalFrame frame(env, 24);
            value = Java::Runtime::Boolean::getBoolean(env, env->NewStringUTF(property));
        }
    }
}

ResettableBoolFlag::ResettableBoolFlag(JNIEnv * env, const char* property) : value(0)
{
    registerResettableFlag(this, property);
    if(env){
        JniLocalFrame frame(env, 24);
        value = Java::Runtime::Boolean::getBoolean(env, env->NewStringUTF(property));
    }else if(JniEnvironment _env{200})
        value = Java::Runtime::Boolean::getBoolean(_env, _env->NewStringUTF(property));
}

ResettableBoolFlag::~ResettableBoolFlag(){
    unregisterResettableFlag(this);
}

void ResettableBoolFlag::reinitialize(JNIEnv * env, const char* property){
    value = Java::Runtime::Boolean::getBoolean(env, env->NewStringUTF(property));
}

JNIEnv *currentJNIEnvironment(bool initializeJavaThread = true){
    static ResettableBoolFlag useFastEnv("io.qt.experimental.fast-jni");
    if(useFastEnv && initializeJavaThread){
        if(JNIEnv *env = fastJNIEnvironment())
            return env;
    }
    JavaVM *vm = nullptr;
    if(std::atomic<JavaVM *>* atm = gJavaVM)
        vm = atm->load();
    if (!vm)
        return nullptr;
    else return currentJNIEnvironment(vm, initializeJavaThread);
}

JniEnvironment::JniEnvironment(JniEnvironment&& other)
    : m_env(other.m_env), m_hasLocalFrames(other.m_hasLocalFrames)
{
    other.m_env = nullptr;
    other.m_hasLocalFrames = false;
}

JniEnvironment::JniEnvironment(bool)
    : m_env(nullptr), m_hasLocalFrames(false)
{}

JniEnvironment::JniEnvironment(bool b,int capacity)
    : m_env(currentJNIEnvironment(b)), m_hasLocalFrames(false)
{
    if(m_env && capacity>0){
        m_hasLocalFrames = m_env->PushLocalFrame(capacity)==0;
    }
}

void JniEnvironment::initialize(JNIEnv *env, int capacity)
{
    if(!m_env){
        m_env = env;
        if(m_env && capacity>0){
            m_hasLocalFrames = m_env->PushLocalFrame(capacity)==0;
        }
    }
}

JniEnvironment::JniEnvironment(int capacity)
    : m_env(currentJNIEnvironment()), m_hasLocalFrames(false)
{
    if(m_env && capacity>0){
        m_hasLocalFrames = m_env->PushLocalFrame(capacity)==0;
    }
}

JniEnvironment::~JniEnvironment(){
    if(m_hasLocalFrames){
        m_env->PopLocalFrame(nullptr);
    }
}

JniEnvironment::operator bool() const{
    return m_env;
}

JniEnvironment::operator JNIEnv *() const{
    return m_env;
}

JNIEnv * JniEnvironment::environment() const{
    return m_env;
}

JNIEnv *JniEnvironment::operator->() const{
    return m_env;
}

#ifdef QTJAMBI_STACKTRACE
void JniEnvironment::checkException(){
    if(m_env->ExceptionCheck()){
        jthrowable t = m_env->ExceptionOccurred();
        m_env->ExceptionClear();
        if(t){
            throw JavaException(m_env, t);
        }
    }
}
#define QTJAMBI_STACKTRACEINFO_DECL_USE(env, t) env, t, methodName, fileName, lineNumber
#define raiseThrowable QtJambiPrivate::raiseJavaException
#else
#define QTJAMBI_STACKTRACEINFO_DECL_USE(env, t) env, t
#define raiseThrowable throw JavaException
#endif

void JniEnvironment::checkException(QTJAMBI_STACKTRACEINFO_DECL_NOENV){
    if(m_env->ExceptionCheck()){
        jthrowable t = m_env->ExceptionOccurred();
        m_env->ExceptionClear();
        if(t)
            raiseThrowable( QTJAMBI_STACKTRACEINFO_DECL_USE(m_env, t) );
    }
}

JniEnvironmentScope::JniEnvironmentScope(QtJambiShell* shell, int capacity)
    : JniEnvironment(capacity),
    m_scope(shell) {}
JniEnvironmentScope::~JniEnvironmentScope(){}
QtJambiScope& JniEnvironmentScope::scope() { return m_scope; }

namespace QtJambiPrivate{
jobject getJavaObjectLocalRef(JNIEnv *env, QtJambiScope& scope);
}

jobject JniEnvironmentScope::getJavaObjectLocalRef(){
    return QtJambiPrivate::getJavaObjectLocalRef(environment(), m_scope);
}

JniEnvironmentScopeExceptionHandler::JniEnvironmentScopeExceptionHandler(QtJambiShell* shell, int capacity)
    : JniEnvironmentExceptionHandler(capacity),
    m_scope(shell) {}
JniEnvironmentScopeExceptionHandler::~JniEnvironmentScopeExceptionHandler(){}
QtJambiScope& JniEnvironmentScopeExceptionHandler::scope() { return m_scope; }
jobject JniEnvironmentScopeExceptionHandler::getJavaObjectLocalRef(){
    return QtJambiPrivate::getJavaObjectLocalRef(environment(), m_scope);
}

JniEnvironmentScopeExceptionInhibitor::JniEnvironmentScopeExceptionInhibitor(QtJambiShell* shell, int capacity)
    : JniEnvironmentExceptionInhibitor(capacity),
    m_scope(shell) {}
JniEnvironmentScopeExceptionInhibitor::~JniEnvironmentScopeExceptionInhibitor(){}
QtJambiScope& JniEnvironmentScopeExceptionInhibitor::scope() { return m_scope; }
jobject JniEnvironmentScopeExceptionInhibitor::getJavaObjectLocalRef(){
    return QtJambiPrivate::getJavaObjectLocalRef(environment(), m_scope);
}

JniEnvironmentScopeExceptionHandlerAndBlocker::JniEnvironmentScopeExceptionHandlerAndBlocker(QtJambiShell* shell, int capacity)
    : JniEnvironmentExceptionHandlerAndBlocker(capacity),
    m_scope(shell) {}
JniEnvironmentScopeExceptionHandlerAndBlocker::~JniEnvironmentScopeExceptionHandlerAndBlocker(){}
QtJambiScope& JniEnvironmentScopeExceptionHandlerAndBlocker::scope() { return m_scope; }
jobject JniEnvironmentScopeExceptionHandlerAndBlocker::getJavaObjectLocalRef(){
    return QtJambiPrivate::getJavaObjectLocalRef(environment(), m_scope);
}

JniEnvironmentScopeExceptionInhibitorAndBlocker::JniEnvironmentScopeExceptionInhibitorAndBlocker(QtJambiShell* shell, int capacity)
    : JniEnvironmentExceptionInhibitorAndBlocker(capacity),
    m_scope(shell) {}
JniEnvironmentScopeExceptionInhibitorAndBlocker::~JniEnvironmentScopeExceptionInhibitorAndBlocker(){}
QtJambiScope& JniEnvironmentScopeExceptionInhibitorAndBlocker::scope() { return m_scope; }
jobject JniEnvironmentScopeExceptionInhibitorAndBlocker::getJavaObjectLocalRef(){
    return QtJambiPrivate::getJavaObjectLocalRef(environment(), m_scope);
}

DefaultJniEnvironment::DefaultJniEnvironment(int capacity)
    : JniEnvironment(false, capacity){}

JniLocalFrame::JniLocalFrame(JNIEnv *env, int capacity) : m_env(env){
    m_env->PushLocalFrame(capacity);
}

JniLocalFrame::~JniLocalFrame(){
    m_env->PopLocalFrame(nullptr);
}

