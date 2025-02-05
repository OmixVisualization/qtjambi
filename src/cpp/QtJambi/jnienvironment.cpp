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

#include <QtCore/QObject>
#include <QtCore/QAbstractEventDispatcher>
#include <thread>
#include <QtCore/QSemaphore>
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

Q_GLOBAL_STATIC(QMutex, gResettableFlagsMutex)
typedef SecureContainer<QHash<ResettableBoolFlag*,const char*>,gResettableFlagsMutex> ResettableFlagsHash;
Q_GLOBAL_STATIC(ResettableFlagsHash, gResettableFlags)

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
    if(Q_LIKELY(!gResettableFlagsMutex.isDestroyed())){
        QMutexLocker locker(gResettableFlagsMutex());
        gResettableFlags->remove(flag);
    }
}

void clearResettableFlags(){
    QHash<ResettableBoolFlag*,const char*> envPointers;
    if(Q_LIKELY(!gResettableFlagsMutex.isDestroyed())){
        QMutexLocker locker(gResettableFlagsMutex());
        envPointers.swap(*gResettableFlags);
    }
    for(auto iter = envPointers.keyValueBegin(), end = envPointers.keyValueEnd(); iter!=end; ++iter){
        iter->first->value = false;
    }
}

void reinitializeResettableFlag(JNIEnv * env, const char* property){
    QHash<ResettableBoolFlag*,const char*> envPointers;
    if(Q_LIKELY(!gResettableFlagsMutex.isDestroyed())){
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
    QHash<ResettableBoolFlag*,const char*> envPointers;
    if(Q_LIKELY(!gResettableFlagsMutex.isDestroyed())){
        QMutexLocker locker(gResettableFlagsMutex());
        envPointers = *gResettableFlags;
    }
    for(auto iter = envPointers.keyValueBegin(), end = envPointers.keyValueEnd(); iter!=end; ++iter){
        iter->first->reinitialize(env, iter->second);
    }
}
#endif

bool avoidJNIGlobalRefs(JNIEnv * env){
#ifdef Q_OS_ANDROID
    static ResettableBoolFlag flag(env, "io.qt.use-jni-global-references");
    return !flag;
#else
    static ResettableBoolFlag flag(env, "io.qt.avoid-jni-global-references");
    return flag;
#endif
}

void clearPurgeThreadAtShutdown(JNIEnv *env);
void initializePurgeThread(JNIEnv *env);

namespace ThreadPrivate{
class PurgeThread{
public:
    PurgeThread();
    ~PurgeThread();
    bool isRunning() const;
    void push(JNIInvokable&& invoker);
private:
    void run();
    void start(JNIEnv *env);
    void finish(JNIEnv *env);
    std::thread m_thread;
    QSemaphore m_semaphore;
    std::atomic_bool m_exit;
    std::atomic<std::list<JNIInvokable>*> m_tasks;
    friend void ::clearPurgeThreadAtShutdown(JNIEnv *env);
    friend void ::initializePurgeThread(JNIEnv *env);
};
}// namespace ThreadPrivate

Q_GLOBAL_STATIC(ThreadPrivate::PurgeThread, gPurgeThread)

void clearPurgeThreadAtShutdown(JNIEnv *env){
    ThreadPrivate::PurgeThread* instance = gPurgeThread();
    if(Q_LIKELY(instance))
        instance->finish(env);
}

void initializePurgeThread(JNIEnv *env){
    ThreadPrivate::PurgeThread* instance = gPurgeThread();
    if(Q_LIKELY(instance))
        instance->start(env);
}

namespace ThreadPrivate{

PurgeThread::PurgeThread()
    : m_thread(), m_semaphore(), m_exit(false), m_tasks(nullptr)
{
}

PurgeThread::~PurgeThread(){
    if(m_thread.joinable()){
        m_exit.store(true, std::memory_order_release);
        m_semaphore.release();
        try{
            m_thread.join();
        }catch(...){}
    }
}

void PurgeThread::start(JNIEnv *env){
    if(!env || !Java::Runtime::Boolean::getBoolean(env, env->NewStringUTF("io.qt.disable-thread-purger"))){
        if(!m_thread.joinable()){
            m_thread = std::thread([this](){run();});
        }
    }
}

void PurgeThread::finish(JNIEnv *env){
    std::list<ThreadPrivate::JNIInvokable>* tasks = m_tasks.exchange(nullptr, std::memory_order_release);
    if(m_thread.joinable()){
        m_exit.store(true, std::memory_order_release);
        m_semaphore.release();
        try{
            m_thread.join();
        }catch(...){}
    }
    if(tasks){
        if(env){
            JniLocalFrame frame(env, jsize(16+4*tasks->size()));
            for(const ThreadPrivate::JNIInvokable& runnable : qAsConst(*tasks)){
                try{
                    runnable(env);
                }catch(const JavaException& exn){
                    if(env)
                        exn.report(env);
                    else
                        printf("QtJambi: exception pending in native code: %s\n", exn.what());
                }
            }
        }
        delete tasks;
    }
}

bool PurgeThread::isRunning() const{
    return m_thread.joinable();
}

void PurgeThread::push(JNIInvokable&& invoker){
    std::list<JNIInvokable>* tasks = m_tasks.exchange(nullptr, std::memory_order_release);
    if(!tasks)
        tasks = new std::list<JNIInvokable>();
    tasks->emplace_back(JNIInvokable{std::move(invoker)});
    std::list<JNIInvokable>* tasks2 = m_tasks.exchange(tasks, std::memory_order_release);
    if(tasks2)
        tasks->merge(std::move(*tasks2));
    m_semaphore.release();
}

void PurgeThread::run(){
    try{
        JNIEnv *env = nullptr;
        bool detach = false;
        while(true){
            bool exited{false};
            std::list<JNIInvokable>* tasks = nullptr;
            {
                m_semaphore.acquire();
                exited = m_exit.load(std::memory_order_acquire);
                if(!exited)
                    tasks = m_tasks.exchange(nullptr, std::memory_order_acquire);
            }
            if(tasks){
                if(!env){
                    JavaVM *vm = nullptr;
                    if(std::atomic<JavaVM *>* atm = gJavaVM)
                        vm = atm->load();
                    if (!vm)
                        return;
                    JavaVMAttachArgs args;
                    args.version = JNI_VERSION_1_8;
                    QByteArray name("QtJambiPurgeThread");
                    args.name = name.isEmpty() ? nullptr : name.data();
                    args.group = nullptr;
#ifdef Q_OS_ANDROID
                    int result = vm->AttachCurrentThreadAsDaemon(&env, &args);
#else
                    int result = vm->AttachCurrentThreadAsDaemon(reinterpret_cast<void **>(&env), &args);
#endif
                    if (result != JNI_OK || !env)
                        return;
                    detach = true;
                }
                JniLocalFrame frame(env, jsize(16+4*tasks->size()));
                for(const JNIInvokable& runnable : qAsConst(*tasks)){
                    try{
                        runnable(env);
                    }catch(const JavaException& exn){
                        if(env)
                            exn.report(env);
                        else
                            printf("QtJambi: exception pending in native code: %s\n", exn.what());
                    }
                }
                delete tasks;
            }
            if(Q_UNLIKELY(exited))
                break;
        }
        if(detach){
            JavaVM *vm = nullptr;
            if(std::atomic<JavaVM *>* atm = gJavaVM)
                vm = atm->load();
            if (!vm)
                return;
            vm->DetachCurrentThread();
        }
    }catch(const std::exception& exn){
        printf("QtJambi: exception pending in native code: %s\n", exn.what());
    }catch(...){
        printf("QtJambi: exception pending in native code\n");
    }
}

void purgeLink(QSharedPointer<QtJambiLink>&& link){
    if(gPurgeThread.isDestroyed())
        return;
    PurgeThread* instance = gPurgeThread();
    if(QThread* thread = QCoreApplicationPrivate::theMainThread.loadRelaxed()){
        QThreadUserData* mainThreadData{nullptr};
        if(thread->loopLevel()>0 || !instance || !instance->isRunning()){
            QReadLocker locker(QtJambiLinkUserData::lock());
            mainThreadData = QTJAMBI_GET_OBJECTUSERDATA(QThreadUserData, thread);
        }
        if(mainThreadData && mainThreadData->threadDeleter()){
            QCoreApplication::postEvent(mainThreadData->threadDeleter(), new LinkInvalidator(std::move(link)));
            return;
        }else if(Q_UNLIKELY(!instance)){
            class LinkInvalidator : public QAbstractMetaCallEvent{
            public:
                LinkInvalidator(QSharedPointer<QtJambiLink>&& link)
                    : QAbstractMetaCallEvent(nullptr, -1), m_link(std::move(link)) {}
            private:
                void placeMetaCall(QObject *) override{
                    if(JniEnvironment env{200}){
                        m_link->invalidate(env);
                    }
                }
                QSharedPointer<QtJambiLink> m_link;
            };
            QCoreApplication::postEvent(thread, new LinkInvalidator(std::move(link)));
            return;
        }
    }
    if(Q_LIKELY(instance))
        instance->push([link = std::move(link)](JNIEnv* env){
            link->invalidate(env);
        });
}

void doInPurgeThread(JNIInvokable &&fun){
    if(gPurgeThread.isDestroyed())
        return;
    PurgeThread* instance = gPurgeThread();
    if(QThread* thread = QCoreApplicationPrivate::theMainThread.loadRelaxed()){
        QThreadUserData* mainThreadData{nullptr};
        if(thread->loopLevel()>0 || !instance || !instance->isRunning()){
            QReadLocker locker(QtJambiLinkUserData::lock());
            mainThreadData = QTJAMBI_GET_OBJECTUSERDATA(QThreadUserData, thread);
        }
        if(mainThreadData && mainThreadData->threadDeleter()){
            class Executer : public AbstractThreadEvent{
            public:
                Executer(JNIInvokable&& fun)
                    : AbstractThreadEvent(), m_fun(std::move(fun)) {}
                Executer* clone() const override {return new Executer(*this);}
            private:
                void execute() override{
                    if(JniEnvironment env{200}){
                        m_fun(env);
                    }
                }
                Executer(const Executer& other) : AbstractThreadEvent(other), m_fun(other.m_fun) {}
                JNIInvokable m_fun;
            };
            QCoreApplication::postEvent(mainThreadData->threadDeleter(), new Executer(std::move(fun)));
            return;
        }else if(Q_UNLIKELY(!instance)){
            class Executer : public QAbstractMetaCallEvent{
            public:
                Executer(JNIInvokable&& fun)
                    : QAbstractMetaCallEvent(nullptr, -1), m_fun(std::move(fun)) {}
            private:
                void placeMetaCall(QObject *) override{
                    if(JniEnvironment env{200}){
                        m_fun(env);
                    }
                }
                JNIInvokable m_fun;
            };
            QCoreApplication::postEvent(thread, new Executer(std::move(fun)));
            return;
        }
    }
    if(Q_LIKELY(instance))
        instance->push(std::move(fun));
}

struct ThreadDetacher : public QtJambiObjectData {
    EventDispatcherCheck::Data* m_threadData;
    ThreadDetacher(EventDispatcherCheck::Data* threadData)
        : QtJambiObjectData(),
        m_threadData(threadData)
    {
    }
    static void finish(JNIEnv *env, EventDispatcherCheck::Data* threadData){
        QTJAMBI_INTERNAL_METHOD_CALL("ThreadDetacher::finish()")
        JavaVM *vm = nullptr;
        if(std::atomic<JavaVM *>* atm = gJavaVM)
            vm = atm->load();
        if (Q_LIKELY(vm)) {
            QSharedPointer<QtJambiLink> link;
            {
                QReadLocker locker(QtJambiLinkUserData::lock());
                link = threadData->m_wlink.toStrongRef();
                threadData->m_wlink = QWeakPointer<QtJambiLink>{};
            }
            if(link || threadData->m_jthreadObject){
                int result = JNI_OK;
                if(!env)
                    result = vm->GetEnv( reinterpret_cast<void **>(&env), JNI_VERSION_1_8);
                if(result==JNI_OK && env){
                    JniLocalFrame __jniLocalFrame(env, 200);
                    if(threadData->m_jthreadObject){
                        jobject jthreadObject = threadData->m_jthreadObject;
                        threadData->m_jthreadObject = nullptr;
                        if(!env->IsSameObject(jthreadObject, nullptr)){
                            try{
                                Java::QtJambi::ThreadUtility::setThreadInterruptible(env, nullptr, jthreadObject, false);
                            } catch (const JavaException& exn) {
                                exn.report(env);
                            }
                        }
                        env->DeleteWeakGlobalRef(jthreadObject);
                    }
                }else{
                    QThreadUserData* mainThreadData{nullptr};
                    if(QThread* thread = QCoreApplicationPrivate::theMainThread.loadRelaxed()){
                        QReadLocker locker(QtJambiLinkUserData::lock());
                        mainThreadData = QTJAMBI_GET_OBJECTUSERDATA(QThreadUserData, thread);
                    }
                    if(mainThreadData){
                        threadData->m_wlink = QWeakPointer<QtJambiLink>{};
                        jweak jthreadObject = threadData->m_jthreadObject;
                        threadData->m_jthreadObject = nullptr;

                        class ThreadPurger : public AbstractThreadEvent{
                            QSharedPointer<QtJambiLink> m_link;
                            jweak m_jthreadObject;
                            int m_associationHashcode;
                        public:
                            ThreadPurger(QSharedPointer<QtJambiLink> &&link, jweak jthreadObject, int associationHashcode)
                                : AbstractThreadEvent(),
                                m_link(std::move(link)),
                                m_jthreadObject(jthreadObject),
                                m_associationHashcode(associationHashcode)
                            {
                                QTJAMBI_INTERNAL_METHOD_CALL("ThreadPurger::ThreadPurger()")
                            }
                            ThreadPurger(const ThreadPurger& other)
                                : AbstractThreadEvent(other),
                                m_link(other.m_link),
                                m_jthreadObject(other.m_jthreadObject),
                                m_associationHashcode(other.m_associationHashcode)
                            {
                                QTJAMBI_INTERNAL_METHOD_CALL("ThreadPurger::ThreadPurger()")
                            }
                            ThreadPurger* clone() const override
                            {
                                return new ThreadPurger(*this);
                            }
                            void execute() override{
                                QTJAMBI_INTERNAL_METHOD_CALL("ThreadPurger::~ThreadPurger()")
                                if(JniEnvironment env{200}){
                                    if(m_link){
                                        Q_ASSERT(m_link->isQObject());
                                        if(!m_link->qobject() || !m_link->qobject()->parent())
                                            m_link->setJavaOwnership(env);
                                    }
                                    if(m_jthreadObject){
                                        jobject jthreadObject = m_jthreadObject;
                                        m_jthreadObject = nullptr;
                                        if(!env->IsSameObject(jthreadObject, nullptr)){
                                            try{
                                                Java::QtJambi::ThreadUtility::setThreadInterruptible(env, nullptr, jthreadObject, false);
                                            } catch (const JavaException& exn) {
                                                exn.report(env);
                                            }
                                        }
                                        env->DeleteWeakGlobalRef(jthreadObject);
                                    }
                                }else{
                                    QTJAMBI_DEBUG_PRINT("Unable to cleanup thread due to missing JNI")
                                }
                            }
                        };
                        QCoreApplication::postEvent(mainThreadData->threadDeleter(), new ThreadPurger(std::move(link), jthreadObject, threadData->m_associationHashcode));
                    }
                }
            }
            while(!threadData->m_finalActions.isEmpty()){
                threadData->m_finalActions.takeFirst()();
            }
            QThreadUserData* data{nullptr};
            if(QThread* thread = threadData->m_thread.data()){
                {
                    QReadLocker locker(QtJambiLinkUserData::lock());
                    data = QTJAMBI_GET_OBJECTUSERDATA(QThreadUserData, thread);
                }
                if(data && data->purgeOnExit()){
                    QWriteLocker locker(QtJambiLinkUserData::lock());
                    QTJAMBI_SET_OBJECTUSERDATA(QThreadUserData, thread, nullptr);
                }
            }
            if(data && data->purgeOnExit())
                delete data;
            vm->DetachCurrentThread();
        }else{
            while(!threadData->m_finalActions.isEmpty()){
                threadData->m_finalActions.takeFirst()();
            }

            QThreadUserData* data{nullptr};
            if(QThread* thread = threadData->m_thread.data()){
                {
                    QReadLocker locker(QtJambiLinkUserData::lock());
                    data = QTJAMBI_GET_OBJECTUSERDATA(QThreadUserData, thread);
                }
                if(data && data->purgeOnExit()){
                    QWriteLocker locker(QtJambiLinkUserData::lock());
                    QTJAMBI_SET_OBJECTUSERDATA(QThreadUserData, thread, nullptr);
                }
            }
            if(data && data->purgeOnExit())
                delete data;
        }
        delete threadData;
    }
    ~ThreadDetacher() override {
        finish(nullptr, m_threadData);
    }
    QTJAMBI_OBJECTUSERDATA_ID_IMPL(static,)
};

void thread_detacher_cleaner(JNIEnv *env, EventDispatcherCheck::Data* threadData) {
#if !defined(Q_OS_ANDROID) //&& (!defined(Q_OS_MACOS) || QT_VERSION < QT_VERSION_CHECK(6, 8, 2))
    if(!threadData->m_thread || QThreadData::get2(threadData->m_thread)->isAdopted){
        ThreadDetacher::finish(env, threadData);
        return;
    }
    QAbstractEventDispatcher * eventDispatcher = nullptr;
    if(threadData->m_thread && !(eventDispatcher = threadData->m_thread->eventDispatcher())){
        if (QThread::currentThread()==threadData->m_thread && (QCoreApplication::instance() || !threadRequiresCoreApplication(threadData->m_thread))) {
            QEventLoop loop;
            eventDispatcher = threadData->m_thread->eventDispatcher();
        }
    }
    if(eventDispatcher){
        QWriteLocker locker(QtJambiLinkUserData::lock());
        QTJAMBI_SET_OBJECTUSERDATA_ID(QTJAMBI_OBJECTUSERDATA_TYPE_ID(ThreadDetacher), eventDispatcher, new ThreadDetacher(threadData));
    }else
#endif
        ThreadDetacher::finish(env, threadData);
}

} // namespace ThreadPrivate

JNIEnv *currentJNIEnvironment(JavaVM *vm, bool& requiresDetach, JniEnvironmentFlags flags = JniEnvironmentFlag::Default){
    JNIEnv *env = nullptr;
    if(flags & JniEnvironmentFlag::NoGetEnv)
        return env;
    int result = vm->GetEnv( reinterpret_cast<void **>(&env), JNI_VERSION_1_8);
    if (result == JNI_EDETACHED) {
        if(std::atomic<bool>* atm = gIsJVMLoaded){
            if(!atm->load())
                return nullptr;
        }else return nullptr;
        if(flags & JniEnvironmentFlag::NoAttachCurrentThread || EventDispatcherCheck::isAlive())
            return nullptr;
        if(QThread* currentThread = QThread::currentThread()){
            QThreadUserData* threadData;
            QThreadAdoptionUserData* threadAdoptionData;
            QThreadInitializationUserData* threadInitializationData;
            {
                QReadLocker locker(QtJambiLinkUserData::lock());
                threadData = QTJAMBI_GET_OBJECTUSERDATA(QThreadUserData, currentThread);
                threadAdoptionData = QTJAMBI_GET_OBJECTUSERDATA(QThreadAdoptionUserData, currentThread);
                threadInitializationData = QTJAMBI_GET_OBJECTUSERDATA(QThreadInitializationUserData, currentThread);
            }
            if(threadData){
                //if(JNIEnv* env = threadData->posessedEnvironment()){
                //    return env;
                //}
                if(threadData->isJavaLaunched() || (threadData->threadType()==QThreadUserData::AdoptedThread && !threadData->threadDeleter())){
                    // Java thread is already terminated. Attaching leads to deadlock.
                    return nullptr;
                }
            }
            bool isDaemon = true;
            QByteArray name;
            JavaVMAttachArgs args;
            args.version = JNI_VERSION_1_8;
            if(threadInitializationData){
                isDaemon = threadInitializationData->isDaemon();
                args.group = threadInitializationData->getThreadGroup();
                name = threadInitializationData->getName();
            }else{
                args.group = nullptr;
                if(currentThread->objectName().isEmpty()){
                    name = currentThread->metaObject()->className();
                }else{
                    name = currentThread->objectName().toUtf8();
                }
            }
            args.name = name.isEmpty() ? nullptr : name.data();
            JavaVMAttachArgs* _args = args.name && args.group ? &args : nullptr;
            if(isDaemon)
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
            if (Q_UNLIKELY(result != JNI_OK)) {
                if(QThread* mainThread = QCoreApplicationPrivate::theMainThread.loadRelaxed()){
                    if(mainThread!=currentThread)
                        qCWarning(DebugAPI::internalCategory, "Failed attaching current thread (%i).", result);
                }else{
                    qCWarning(DebugAPI::internalCategory, "Failed attaching current thread (%i).", result);
                }
                env = nullptr;
            }else{
                bool init = false;
                if(!threadData){
                    // we strictly need to set threadData prior to creating the Java wrapper!
                    QWriteLocker locker(QtJambiLinkUserData::lock());
                    threadData = QTJAMBI_GET_OBJECTUSERDATA(QThreadUserData, currentThread);
                    if(!threadData){
                        threadData = new QThreadUserData();
                        init = true;
                        QTJAMBI_SET_OBJECTUSERDATA(QThreadUserData, currentThread, threadData);
                    }
                }
                if(init)
                    threadData->initializeDefault(currentThread);
                if(Q_LIKELY(env && env->functions)){
                    QThreadData* td = QThreadData::get2(currentThread);
                    requiresDetach = td->isAdopted;
                    if(!(flags & JniEnvironmentFlag::NoInitializeJavaThread)){
                        JniLocalFrame __jniLocalFrame(env, 200);
                        jobject jthread = Java::Runtime::Thread::currentThread(env);
                        if(!name.isEmpty())
                            Java::Runtime::Thread::setName(env, jthread, env->NewStringUTF(name.constData()));
                        if(threadInitializationData || threadAdoptionData){
                            {
                                QWriteLocker locker(QtJambiLinkUserData::lock());
                                if(threadAdoptionData)
                                    QTJAMBI_SET_OBJECTUSERDATA(QThreadAdoptionUserData, currentThread, nullptr);
                                if(threadInitializationData)
                                    QTJAMBI_SET_OBJECTUSERDATA(QThreadInitializationUserData, currentThread, nullptr);
                            }
                            if(threadInitializationData){
                                jobject uncaughtExceptionHandler = threadInitializationData->getUncaughtExceptionHandler(env);
                                if(uncaughtExceptionHandler){
                                    Java::Runtime::Thread::setUncaughtExceptionHandler(env, jthread, uncaughtExceptionHandler);
                                }
                                if(jobject contextClassLoader = threadInitializationData->getContextClassLoader(env)){
                                    Java::Runtime::Thread::setContextClassLoader(env, jthread, contextClassLoader);
                                }
                                threadInitializationData->clear(env);
                                delete threadInitializationData;
                            }
                            if(threadAdoptionData){
                                threadAdoptionData->clear(env);
                                delete threadAdoptionData;
                            }
                        }
                        if(jobject qthreadObject = qtjambi_cast<jobject>(env, currentThread)){
                            QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForQObject(currentThread);
                            link->setAdoptedThread(true);
                            jint associationHashcode = 0;
                            jobject jthrd;
                            if(!(jthrd = Java::QtCore::QThread::javaThread(env, qthreadObject)) || !env->IsSameObject(jthrd, jthread)){
                                /*if(jthrd){
                                    try{
                                        Java::QtJambi::NativeUtility::deleteAssociation(env, jthrd);
                                    }catch(const JavaException& exn){
                                        exn.report(env);
                                    }
                                }*/
                                Java::QtCore::QThread::set_javaThread(env, qthreadObject, nullptr);
                                Java::QtCore::Internal::QThread::setJavaThreadReference(env, qthreadObject, jthread);
                                associationHashcode = Java::QtJambi::NativeUtility::createAssociation(env, jthread, qthreadObject);
                                Java::QtJambi::ThreadUtility::setThreadInterruptible(env, qthreadObject, jthread, true);
                            }
                            EventDispatcherCheck::adoptThread(env, jthread, currentThread, threadData, associationHashcode, link, &ThreadPrivate::thread_detacher_cleaner);
                        }
                    }else{
                        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForQObject(currentThread)){
                            link->setAdoptedThread(true);
                            EventDispatcherCheck::adoptThread(env, nullptr, currentThread, threadData, 0, link, &ThreadPrivate::thread_detacher_cleaner);
                        }else{
                            EventDispatcherCheck::adoptThread(env, nullptr, currentThread, threadData, 0, {}, &ThreadPrivate::thread_detacher_cleaner);
                        }
                        if(threadInitializationData || threadAdoptionData){
                            {
                                QWriteLocker locker(QtJambiLinkUserData::lock());
                                if(threadAdoptionData)
                                    QTJAMBI_SET_OBJECTUSERDATA(QThreadAdoptionUserData, currentThread, nullptr);
                                if(threadInitializationData)
                                    QTJAMBI_SET_OBJECTUSERDATA(QThreadInitializationUserData, currentThread, nullptr);
                            }
                            JniLocalFrame __jniLocalFrame(env, 200);
                            if(threadInitializationData){
                                threadInitializationData->clear(env);
                                delete threadInitializationData;
                            }
                            if(threadAdoptionData){
                                threadAdoptionData->clear(env);
                                delete threadAdoptionData;
                            }
                        }
                    }
                }
            }
        }else{
            // this is possible during thread destruction.
#if 1
#ifdef Q_OS_ANDROID
            result = vm->AttachCurrentThreadAsDaemon(&env, nullptr);
#else
            result = vm->AttachCurrentThreadAsDaemon(reinterpret_cast<void **>(&env), nullptr);
#endif
            if (Q_UNLIKELY(result != JNI_OK)) {
                qCWarning(DebugAPI::internalCategory, "Failed attaching current thread (%i).", result);
                env = nullptr;
            }else{
                requiresDetach = true;
                EventDispatcherCheck::adoptThread(env, nullptr, nullptr, nullptr, 0, {}, &ThreadPrivate::thread_detacher_cleaner);
            }
#endif
        }
    } else {
        Q_ASSERT(result == JNI_OK);
    }
    if(Q_UNLIKELY(env && !env->functions))
        env = nullptr;
    return env;
}

struct JNIEnvKeeper{
    JNIEnvKeeper();
    ~JNIEnvKeeper();
    JNIEnv *env = nullptr;
    bool requiresDetach = false;
};

JNIEnvKeeper::JNIEnvKeeper(){
    JavaVM *vm = nullptr;
    if(std::atomic<JavaVM *>* atm = gJavaVM)
        vm = atm->load();
    if (vm)
        env = currentJNIEnvironment(vm, requiresDetach, JniEnvironmentFlag::Default);
}

JNIEnvKeeper::~JNIEnvKeeper(){
    if(requiresDetach)
        EventDispatcherCheck::detach(env);
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
    if (Q_LIKELY(vm)){
        JNIEnv *env = nullptr;
        vm->GetEnv( reinterpret_cast<void **>(&env), JNI_VERSION_1_8);
        if (env) {
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
    }else if(JniEnvironment _env{24})
        value = Java::Runtime::Boolean::getBoolean(_env, _env->NewStringUTF(property));
}

ResettableBoolFlag::~ResettableBoolFlag(){
    unregisterResettableFlag(this);
}

void ResettableBoolFlag::reinitialize(JNIEnv * env, const char* property){
    value = Java::Runtime::Boolean::getBoolean(env, env->NewStringUTF(property));
}

bool useFastEnv(JNIEnv *env = nullptr){
    static ResettableBoolFlag b(env, "io.qt.experimental.fast-jni");
    return b;
}

JNIEnv *currentJNIEnvironment(bool& requiresDetach, JniEnvironmentFlags flags = JniEnvironmentFlag::Default){
    if(useFastEnv() && !(flags & (int(JniEnvironmentFlag::NoGetEnv) | int(JniEnvironmentFlag::NoInitializeJavaThread)))){
        if(JNIEnv *env = fastJNIEnvironment())
            return env;
    }
    JavaVM *vm = nullptr;
    if(std::atomic<JavaVM *>* atm = gJavaVM)
        vm = atm->load();
    if (!vm)
        return nullptr;
    else return currentJNIEnvironment(vm, requiresDetach, flags);
}

enum Flags : quint8{
    HasLocalFrames = 0x01,
    RequiresDetach = 0x10
};

JniEnvironment::JniEnvironment(JniEnvironment&& other)
    : m_env(other.m_env), m_flags(other.m_flags)
{
    other.m_env = nullptr;
    other.m_flags = 0;
}

JniEnvironment::JniEnvironment(bool flag, int capacity)
    : JniEnvironment(int(flag ? JniEnvironmentFlag::Default : JniEnvironmentFlag::NoInitializeJavaThread), capacity)
{}

JniEnvironment::JniEnvironment(int flag, int capacity)
    : m_env(nullptr), m_flags(0)
{
    bool requiresDetach = false;
    m_env = currentJNIEnvironment(requiresDetach, JniEnvironmentFlags(flag));
    if(Q_LIKELY(m_env)){
        if(capacity>0){
            try{
                if(m_env->PushLocalFrame(capacity)==0)
                    m_flags |= HasLocalFrames;
            }catch(...){
                m_env = nullptr;
                return;
            }
        }
        if(requiresDetach)
            m_flags |= RequiresDetach;
    }
}

void JniEnvironment::initialize(JNIEnv *env, bool requiresDetach, int capacity)
{
    if(Q_LIKELY(!m_env)){
        m_env = env;
        m_flags = 0;
        if(Q_LIKELY(m_env)){
            if(capacity>0){
                try{
                    if(m_env->PushLocalFrame(capacity)==0)
                        m_flags |= HasLocalFrames;
                }catch(...){
                    m_env = nullptr;
                    return;
                }
            }
            if(requiresDetach)
                m_flags |= RequiresDetach;
        }
    }
}

JniEnvironment::JniEnvironment(int capacity)
    : JniEnvironment(int(JniEnvironmentFlag::Default), capacity)
{
}

JniEnvironment::~JniEnvironment(){
    if(m_flags & HasLocalFrames)
        m_env->PopLocalFrame(nullptr);
    if(m_flags & RequiresDetach)
        EventDispatcherCheck::detach(m_env);
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
    if(Q_UNLIKELY(m_env && m_env->ExceptionCheck())){
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
    if(Q_UNLIKELY(m_env && m_env->ExceptionCheck())){
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
    : JniEnvironment(int(JniEnvironmentFlag::NoInitializeJavaThread), capacity){}

JniLocalFrame::JniLocalFrame(JNIEnv *env, int capacity) : m_env(env){
    m_env->PushLocalFrame(capacity);
}

JniLocalFrame::~JniLocalFrame(){
    m_env->PopLocalFrame(nullptr);
}

