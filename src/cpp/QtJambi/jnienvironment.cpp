/****************************************************************************
**
** Copyright (C) 2009-2022 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

Q_GLOBAL_STATIC_WITH_ARGS(std::atomic<JavaVM *>, gJavaVM, (nullptr));

Q_GLOBAL_STATIC_WITH_ARGS(std::atomic<bool>, gIsJVMLoaded, (false))

std::atomic<bool>* getJVMLoaded(){
    return gIsJVMLoaded();
}

std::atomic<JavaVM *>* getJVM(){
    return gJavaVM();
}

void thread_detacher_cleaner(QPointer<QThread>& thread, JObjectWrapper&& jthreadObjectWrapper, QWeakPointer<QtJambiLink>&& wlink, QList<std::function<void()>>&& finalActions) {
     struct ThreadDetacher : public QtJambiObjectData {
         JObjectWrapper m_jthreadObjectWrapper;
         QPointer<QThread> m_thread;
         QWeakPointer<QtJambiLink> m_wlink;
         QList<std::function<void()>> m_finalActions;
         ThreadDetacher(JObjectWrapper&& jthreadObjectWrapper,
                        QPointer<QThread>& thread,
                        QWeakPointer<QtJambiLink>&& wlink,
                        QList<std::function<void()>>&& finalActions)
             : QtJambiObjectData(),
               m_jthreadObjectWrapper(std::move(jthreadObjectWrapper)),
               m_thread(),
               m_wlink(std::move(wlink)),
               m_finalActions(std::move(finalActions))
         {
             QTJAMBI_DEBUG_METHOD_PRINT("native", "ThreadDetacher::ThreadDetacher()")
             m_thread.swap(thread);
         }
         ~ThreadDetacher() override {
             QTJAMBI_DEBUG_METHOD_PRINT("native", "ThreadDetacher::~ThreadDetacher()")
             JavaVM *vm = nullptr;
             if(std::atomic<JavaVM *>* atm = gJavaVM)
                 vm = atm->load();
             if (vm) {
                  QSharedPointer<QtJambiLink> link;
                  {
                      QReadLocker locker(QtJambiLinkUserData::lock());
                      link = m_wlink.toStrongRef();
                  }
                  if(link || m_jthreadObjectWrapper.object()){
                        JNIEnv* env = nullptr;
                        int result = vm->GetEnv( reinterpret_cast<void **>(&env), JNI_VERSION_1_8);
                        if(result==JNI_OK &&  env){
                            JniLocalFrame __jniLocalFrame(env, 200);
                            Q_ASSERT(link->isQObject());
                            if(!link->qobject() || !link->qobject()->parent())
                               link->setJavaOwnership(env);
                            jobject javaThread = env->NewLocalRef(m_jthreadObjectWrapper.object());
                            m_jthreadObjectWrapper.clear(env);
                            if(javaThread){
                                try{
                                    Java::QtJambi::ThreadUtility::setThreadInterruptible(env, nullptr, javaThread, false);
                                } catch (const JavaException& exn) {
                                    exn.report(env);
                                }
                                try {
                                    Java::QtJambi::NativeUtility::deleteAssociation(env, javaThread);
                                } catch (const JavaException& exn) {
                                    exn.report(env);
                                }
                            }
                        }
                  }
                  while(!m_finalActions.isEmpty()){
                      m_finalActions.takeFirst()();
                  }
                  {
                      QWriteLocker locker(QtJambiLinkUserData::lock());
                      if(QThreadUserData* data = QTJAMBI_GET_OBJECTUSERDATA(QThreadUserData, m_thread.data())){
                          if(data->purgeOnExit()){
                              QTJAMBI_SET_OBJECTUSERDATA(QThreadUserData, m_thread.data(), nullptr);
                              locker.unlock();
                              delete data;
                              locker.relock();
                          }
                      }
                  }
                  vm->DetachCurrentThread();
             }else{
                 while(!m_finalActions.isEmpty()){
                     m_finalActions.takeFirst()();
                 }

                 {
                      QWriteLocker locker(QtJambiLinkUserData::lock());
                      if(QThreadUserData* data = QTJAMBI_GET_OBJECTUSERDATA(QThreadUserData, m_thread.data())){
                          if(data->purgeOnExit()){
                              QTJAMBI_SET_OBJECTUSERDATA(QThreadUserData, m_thread.data(), nullptr);
                              locker.unlock();
                              delete data;
                              locker.relock();
                          }
                      }
                 }
             }
         }
         QTJAMBI_OBJECTUSERDATA_ID_IMPL(static,)
     };
     QAbstractEventDispatcher * eventDispatcher = nullptr;
     if(thread && !(eventDispatcher = thread->eventDispatcher())){
          if (QThread::currentThread()==thread && (QCoreApplication::instance() || !QCoreApplicationPrivate::threadRequiresCoreApplication())) {
              QEventLoop loop;
              eventDispatcher = thread->eventDispatcher();
          }
     }
     if(eventDispatcher){
           QWriteLocker locker(QtJambiLinkUserData::lock());
           QTJAMBI_SET_OBJECTUSERDATA_ID(QTJAMBI_OBJECTUSERDATA_TYPE_ID(ThreadDetacher), eventDispatcher, new ThreadDetacher(std::move(jthreadObjectWrapper), thread, std::move(wlink), std::move(finalActions)));
     }else{
           ThreadDetacher releaser(std::move(jthreadObjectWrapper), thread, std::move(wlink), std::move(finalActions));
           Q_UNUSED(releaser)
     }
}

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
                    qWarning("Failed attaching current thread (%i).", result);
            }else{
                qWarning("Failed attaching current thread (%i).", result);
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
                QSharedPointer<QtJambiLink> link;
                if(jobject qthreadObject = qtjambi_cast<jobject>(env, currentThread)){
                    link = QtJambiLink::findLinkForJavaObject(env, qthreadObject);
                    if(jobject jthrd = Java::QtCore::QThread::javaThread(env, qthreadObject)){
                        env->DeleteLocalRef(jthrd);
                    }else{
                        Java::QtCore::QThread::set_javaThread(env, qthreadObject, jthread);
                        Java::QtJambi::NativeUtility::createAssociation(env, jthread, qthreadObject);
                        Java::QtJambi::ThreadUtility::setThreadInterruptible(env, qthreadObject, jthread, true);
                        if(!currentThread->parent()){
                            QtJambiAPI::setJavaOwnership(env, qthreadObject);
                        }

                        // detach the QThread from Java
                        // After emitting QThread::finished the QThreadStorage data is cleared.
                        // Thereafter, the event dispatcher is deleted if available.
                        // Let QThreadStorage-deletion check if an event dispatcher is available and create an empty event dispatcher otherwise.
                        // During deletion of the event dispatcher (which is the latest action in a thread) the java thread is detached.
                        if(!EventDispatcherCheck::storage.hasLocalData() || !EventDispatcherCheck::storage.localData()){
                            EventDispatcherCheck::storage.setLocalData(QSharedPointer<EventDispatcherCheck>(
                                 new EventDispatcherCheck(
                                     JObjectWrapper(env, jthread),
                                     currentThread,
                                     link,
                                     &thread_detacher_cleaner)
                                ));
                        }
                    }
                    if(env->GetObjectRefType(qthreadObject)==jobjectRefType::JNILocalRefType){
                        env->DeleteLocalRef(qthreadObject);
                        qthreadObject = nullptr;
                    }
                }
                env->DeleteLocalRef(jthread);
                jthread = nullptr;
            }
        }
        {
            QReadLocker locker(QtJambiLinkUserData::lock());
            threadData = QTJAMBI_GET_OBJECTUSERDATA(QThreadUserData, currentThread);
        }
        if(!threadData){
            threadData = new QThreadUserData(currentThread);
            QWriteLocker locker(QtJambiLinkUserData::lock());
            QTJAMBI_SET_OBJECTUSERDATA(QThreadUserData, currentThread, threadData);
        }
    } else {
        Q_ASSERT(result == JNI_OK);
    }
    if(env && !env->functions)
        env = nullptr;
    return env;
}

JNIEnv *currentJNIEnvironment(bool initializeJavaThread = true){
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

JniEnvironment::JniEnvironment(bool b,int capacity)
    : m_env(currentJNIEnvironment(b)), m_hasLocalFrames(false)
{
    if(m_env && capacity>0){
        m_hasLocalFrames = m_env->PushLocalFrame(capacity)==0;
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

DefaultJniEnvironment::DefaultJniEnvironment(int capacity)
    : JniEnvironment(false, capacity){}

JniLocalFrame::JniLocalFrame(JNIEnv *env, int capacity) : m_env(env){
    m_env->PushLocalFrame(capacity);
}

JniLocalFrame::~JniLocalFrame(){
    m_env->PopLocalFrame(nullptr);
}

