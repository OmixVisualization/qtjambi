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

#include <QtCore/qcompilerdetection.h>

QT_WARNING_DISABLE_DEPRECATED

#include "qtjambiapi.h"
#include "java_p.h"
#include "qtjambilink_p.h"
#include "threadapi.h"
#include "threadutils_p.h"
#include <QtCore/private/qthread_p.h>
#include <QtCore/private/qobject_p.h>

#include <qglobal.h>

#include <QtCore/QAbstractItemModel>
#include <QtCore/QCoreApplication>
#include <QtCore/private/qcoreapplication_p.h>
#include <QtCore/QDir>
#include <QtCore/QEvent>
#include <QtCore/QFileInfo>
#include <QtCore/QLibrary>
#include <QtCore/QMetaMethod>
#include <QtCore/QMetaObject>
#include <QtCore/QSettings>
#include <QtCore/QStringList>
#include <QtCore/QThread>
#include <QtCore/private/qthread_p.h>
#include <QtCore/QVariant>
#include <QtCore/QTextStream>
#include <QtCore/QMutex>
#include <QtCore/QAbstractEventDispatcher>
#include <stdio.h>
#include <string.h>

#define QTJAMBI_INTERNAL_METHOD_CALL(methodname)\
DebugAPI::MethodPrint __debug_method_print(DebugAPI::MethodPrint::Internal, methodname, __FILE__, __LINE__, Q_FUNC_INFO);

#if defined(Q_OS_LINUX)
#include <sys/syscall.h>
#endif

#include "qtjambi_cast.h"

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#define MutexLocker QMutexLocker
#else
#define MutexLocker QMutexLocker<QMutex>
#endif

AbstractThreadEvent::AbstractThreadEvent()
    : QEvent(QEvent::DeferredDelete)
{
}

AbstractThreadEvent::AbstractThreadEvent(const AbstractThreadEvent& clone)
    : QEvent(QEvent::DeferredDelete)
{
    Q_UNUSED(clone)
    Q_UNUSED(reserved1)
    Q_UNUSED(reserved2)
}

bool AbstractThreadEvent::execute(QEvent* event){
    if(AbstractThreadEvent* devent = static_cast<AbstractThreadEvent*>(event)){
        devent->execute();
        return true;
    }
    return false;
}

class Deleter : public QObject{
public:
    using QObject::QObject;
    bool event(QEvent* event) override;
    void deleteLater();
};

template<typename T>
class DestructorEvent : public AbstractThreadEvent
{
public:
    DestructorEvent(T *pointer);
protected:
    DestructorEvent(const DestructorEvent& clone);
    DestructorEvent* clone() const override;
private:
    void execute() override;
    T *m_pointer;
};

template<typename T>
DestructorEvent<T>::DestructorEvent(T *pointer)
    : AbstractThreadEvent(),
    m_pointer(pointer)
{}

template<typename T>
DestructorEvent<T>::DestructorEvent(const DestructorEvent& clone)
    : AbstractThreadEvent(),
    m_pointer(clone.m_pointer)
{}

template<typename T>
DestructorEvent<T>* DestructorEvent<T>::clone() const{
    return new DestructorEvent(*this);
}

template<typename T>
void DestructorEvent<T>::execute() {
    T *pointer = m_pointer;
    m_pointer = nullptr;
    delete pointer;
}

template<typename Fun>
class FunctorEvent : public AbstractThreadEvent
{
public:
    FunctorEvent(Fun&& functor);
    FunctorEvent(const FunctorEvent& clone);
    void execute() override;
    FunctorEvent* clone() const override;
private:
    Fun m_functor;
};

template<typename Fun>
FunctorEvent<Fun>::FunctorEvent(Fun&& functor)
    : AbstractThreadEvent(),
    m_functor(std::move(functor))
{}

template<typename Fun>
FunctorEvent<Fun>::FunctorEvent(const FunctorEvent& clone)
    : AbstractThreadEvent(),
    m_functor(clone.m_functor)
{}

template<typename Fun>
FunctorEvent<Fun>* FunctorEvent<Fun>::clone() const{
    return new FunctorEvent(*this);
}

template<typename Fun>
void FunctorEvent<Fun>::execute() {
    m_functor();
}

void Deleter::deleteLater(){
    QCoreApplication::postEvent(this, new DestructorEvent<Deleter>(this));
}

bool Deleter::event(QEvent* event) {
    switch(event->type()){
    case QEvent::DeferredDelete:
        if(AbstractThreadEvent::execute(event))
            return true;
        break;
    default: break;
    }
    return QObject::event(event);
}

QThreadUserData::QThreadUserData():
    QtJambiObjectData(),
    m_threadDeleter(QSharedPointer<Deleter>::create()),
    m_finalActions(nullptr),
    m_mutex(nullptr),
    m_threadGroup(),
    m_isDaemon(true),
    m_name(),
    m_uncaughtExceptionHandler(),
    m_threadType(DefaultThread)
{
}

QThreadUserData::QThreadUserData(JNIEnv *env, jobject threadGroup):
    QThreadUserData()
{
    m_threadGroup = JObjectWrapper(env, threadGroup);
}

QThreadUserData::~QThreadUserData(){
    QTJAMBI_INTERNAL_METHOD_CALL("QThreadUserData::~QThreadUserData()")
    switch(m_threadType){
    case ProcessMainThread:
        cleanup(true);
        break;
    default:
        if(m_finishedConnection){
            QObject::disconnect(m_finishedConnection);
            cleanup(true);
        }
        break;
    }
}

void QThreadUserData::initialize(QThread* thread, bool isAdopted){
    m_threadDeleter->moveToThread(thread);
    if(QCoreApplicationPrivate::theMainThread.loadRelaxed() == thread){
#if defined(Q_OS_LINUX)
        m_threadType = quintptr(syscall(SYS_gettid)) == quintptr(getpid()) ? ProcessMainThread : VirtualMainThread;
#elif defined(Q_OS_MACOS)
        m_threadType = pthread_main_np() ? ProcessMainThread : VirtualMainThread;
#elif defined(Q_OS_WIN)
        m_threadType = ProcessMainThread;
#endif
    }else if(isAdopted){
        m_threadType = AdoptedThread;
    }
    switch(m_threadType){
    case VirtualMainThread:
        m_finishedConnection = QObject::connect(thread, &QThread::finished, thread, [this]() {
                QCoreApplicationPrivate::theMainThread.storeRelaxed(nullptr);
                QTJAMBI_INTERNAL_METHOD_CALL("QThread::finished received")
                this->cleanup(false);
                QObject::disconnect(this->m_finishedConnection);
            }, Qt::DirectConnection);
        break;
    case AdoptedThread:
    case DefaultThread:
        m_finishedConnection = QObject::connect(thread, &QThread::finished, thread, [this]() {
                QTJAMBI_INTERNAL_METHOD_CALL("QThread::finished received")
                this->cleanup(false);
                QObject::disconnect(this->m_finishedConnection);
            }, Qt::DirectConnection);
        break;
    default:
        break;
    }
}

void QThreadUserData::cleanup(bool isInDestructor){
    QTJAMBI_INTERNAL_METHOD_CALL("QThreadUserData::cleanup()")
    QList<QPointer<QObject>> objectsForDeletion;
    {
        MutexLocker locker(m_mutex);
        objectsForDeletion.swap(m_objectsForDeletion);
    }
    for(const QPointer<QObject>& ptr : objectsForDeletion){
        if(ptr)
            delete ptr.data();
    }
    Q_UNUSED(isInDestructor)
    m_threadDeleter.reset();
}

QThreadUserData::Result QThreadUserData::ensureThreadUserDataLocked(QThread* thread){
    QThreadUserData* threadData = QTJAMBI_GET_OBJECTUSERDATA(QThreadUserData, thread);
    bool init = false;
    if(!threadData){
        threadData = QTJAMBI_GET_OBJECTUSERDATA(QThreadUserData, thread);
        if(!threadData){
            threadData = new QThreadUserData();
            init = true;
            QTJAMBI_SET_OBJECTUSERDATA(QThreadUserData, thread, threadData);
        }
    }
    return {threadData, init};
}

QThreadUserData* QThreadUserData::ensureThreadUserData(QThread* thread){
    QThreadUserData* threadData{nullptr};
    {
        QReadLocker locker(QtJambiLinkUserData::lock());
        threadData = QTJAMBI_GET_OBJECTUSERDATA(QThreadUserData, thread);
    }
    bool init = false;
    if(!threadData){
        QWriteLocker locker(QtJambiLinkUserData::lock());
        threadData = QTJAMBI_GET_OBJECTUSERDATA(QThreadUserData, thread);
        if(!threadData){
            threadData = new QThreadUserData();
            init = true;
            QTJAMBI_SET_OBJECTUSERDATA(QThreadUserData, thread, threadData);
        }
    }
    if(init)
        threadData->initialize(thread);
    return threadData;
}

QTJAMBI_OBJECTUSERDATA_ID_IMPL(,QThreadUserData::)

namespace ThreadPrivate{
void purgeThread(JNIEnv* env, EventDispatcherCheck::Data* threadData){
    QSharedPointer<QtJambiLink> link;
    {
        QReadLocker locker(QtJambiLinkUserData::lock());
        link = threadData->m_wlink.toStrongRef();
        threadData->m_wlink = QWeakPointer<QtJambiLink>{};
    }
    if(!QThreadData::get2(threadData->m_thread.data())->isAdopted
        && link
        && link->qobject()
        && !link->qobject()->parent()
        && link->qobject()!=QCoreApplicationPrivate::mainThread()){
        link->setJavaOwnership(env);
    }
    jobject jthreadObject = env->NewLocalRef(threadData->m_jthreadObject);
    env->DeleteWeakGlobalRef(threadData->m_jthreadObject);
    threadData->m_jthreadObject = nullptr;
    if(jthreadObject){
        try{
            Java::QtJambi::NativeUtility::deleteAssociation(env, jthreadObject);
        }catch(const JavaException& exn){
            exn.report(env);
        }
        env->DeleteLocalRef(jthreadObject);
        jthreadObject = nullptr;
    }
    if(env->ExceptionCheck()){
        env->ExceptionDescribe();
        env->ExceptionClear();
    }
}

class ThreadPurger : public QObject{
    EventDispatcherCheck::Data* m_threadData;
public:
    ThreadPurger(EventDispatcherCheck::Data* threadData)
        : QObject(),
        m_threadData(threadData){
        QTJAMBI_INTERNAL_METHOD_CALL("ThreadPurger::ThreadPurger()")
    }
    ~ThreadPurger() override{
        QTJAMBI_INTERNAL_METHOD_CALL("ThreadPurger::~ThreadPurger()")
        if(m_threadData){
            if(JniEnvironment env{200}){
                ThreadPrivate::purgeThread(env, m_threadData);
            }else{
                QTJAMBI_DEBUG_PRINT("Unable to cleanup thread due to missing JNI")
            }
            delete m_threadData;
            m_threadData = nullptr;
        }
    }
};

void eventless_thread_cleaner(EventDispatcherCheck::Data* threadData){
    bool noDeletion = false;
    if(threadData->m_thread.data()==QThread::currentThread()){
        if(JniEnvironment env{200})
            ThreadPrivate::purgeThread(env, threadData);
        else{
            ThreadPurger* referenceDeleter = new ThreadPurger(threadData);
            referenceDeleter->moveToThread(QCoreApplicationPrivate::mainThread());
            referenceDeleter->deleteLater();
            noDeletion = true;
        }
    }else{
        if(DefaultJniEnvironment env{200})
            ThreadPrivate::purgeThread(env, threadData);
        else{
            ThreadPurger* referenceDeleter = new ThreadPurger(threadData);
            referenceDeleter->moveToThread(QCoreApplicationPrivate::mainThread());
            referenceDeleter->deleteLater();
            noDeletion = true;
        }
    }
    while(!threadData->m_finalActions.isEmpty()){
        threadData->m_finalActions.takeFirst()();
    }
    threadData->m_finalActions.clear();
    {
        QWriteLocker locker(QtJambiLinkUserData::lock());
        if(QThreadUserData* data = QTJAMBI_GET_OBJECTUSERDATA(QThreadUserData, threadData->m_thread.data())){
            if(data->purgeOnExit()){
                QTJAMBI_SET_OBJECTUSERDATA(QThreadUserData, threadData->m_thread.data(), nullptr);
                locker.unlock();
                delete data;
                locker.relock();
            }
        }
    }
    if(!noDeletion)
        delete threadData;
}

void thread_cleaner(EventDispatcherCheck::Data* threadData){
     struct ObjectReleaser : public QtJambiObjectData {
        EventDispatcherCheck::Data* m_threadData;
        ObjectReleaser(EventDispatcherCheck::Data* threadData)
            : QtJambiObjectData(),
              m_threadData(threadData) {
            QTJAMBI_INTERNAL_METHOD_CALL("ObjectReleaser::ObjectReleaser()")
        }
        ~ObjectReleaser() override {
            bool noDeletion = false;
            QTJAMBI_INTERNAL_METHOD_CALL("ObjectReleaser::~ObjectReleaser()")
            if(m_threadData->m_thread.data()==QThread::currentThread()){
                if(JniEnvironment env{200})
                    ThreadPrivate::purgeThread(env, m_threadData);
                else{
                    ThreadPurger* referenceDeleter = new ThreadPurger(m_threadData);
                    referenceDeleter->moveToThread(QCoreApplicationPrivate::mainThread());
                    referenceDeleter->deleteLater();
                    noDeletion = true;
                }
            }else{
                if(DefaultJniEnvironment env{200})
                    ThreadPrivate::purgeThread(env, m_threadData);
                else{
                    ThreadPurger* referenceDeleter = new ThreadPurger(m_threadData);
                    referenceDeleter->moveToThread(QCoreApplicationPrivate::mainThread());
                    referenceDeleter->deleteLater();
                    noDeletion = true;
                }
            }
            while(!m_threadData->m_finalActions.isEmpty()){
                m_threadData->m_finalActions.takeFirst()();
            }
            m_threadData->m_finalActions.clear();
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
            if(!noDeletion)
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
          QTJAMBI_SET_OBJECTUSERDATA_ID(QTJAMBI_OBJECTUSERDATA_TYPE_ID(ObjectReleaser), eventDispatcher, new ObjectReleaser(threadData));
     }else{
         eventless_thread_cleaner(threadData);
     }
}

void adoptThread(JNIEnv *env, jobject java_thread, jobject java_qthread, QThread *qt_thread)
{
    QString threadName = qtjambi_cast<QString>(env, Java::Runtime::Thread::getName(env, java_thread));
    qt_thread->setObjectName(threadName);
    jobject javaQThread = java_qthread ? env->NewLocalRef(java_qthread) : nullptr;
    if(!javaQThread){
        javaQThread = qtjambi_cast<jobject>(env, qt_thread);
    }
    if(javaQThread){
        QThreadUserData* threadData;
        {
            QReadLocker locker(QtJambiLinkUserData::lock());
            threadData = QTJAMBI_GET_OBJECTUSERDATA(QThreadUserData, qt_thread);
        }
        bool init = false;
        if(threadData){
            if(threadData->getUncaughtExceptionHandler()){
                Java::Runtime::Thread::setUncaughtExceptionHandler(env, java_thread, threadData->getUncaughtExceptionHandler());
                threadData->clearUncaughtExceptionHandler(env);
            }
            if(threadData->getContextClassLoader()){
                Java::Runtime::Thread::setContextClassLoader(env, java_thread, threadData->getContextClassLoader());
                threadData->clearContextClassLoader(env);
            }
            threadData->clearThreadGroup(env);
        }else{
            QWriteLocker locker(QtJambiLinkUserData::lock());
            threadData = new QThreadUserData();
            init = true;
            QTJAMBI_SET_OBJECTUSERDATA(QThreadUserData, qt_thread, threadData);
        }
        if(init)
            threadData->initialize(qt_thread, true);
        jobject jthread = Java::QtCore::QThread::javaThread(env, javaQThread);
        if(!jthread){
            Java::QtCore::QThread::set_javaThread(env, javaQThread, java_thread);
            if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForQObject(qt_thread)){
                link->setNoThreadInitializationOnPurge(true);
                Java::QtJambi::NativeUtility::createAssociation(env, java_thread, javaQThread);
                if(!qt_thread->parent()){
                    if(QThreadData::get2(qt_thread)->isAdopted || link->qobject()==QCoreApplicationPrivate::mainThread()){
                        link->setCppOwnership(env);
                    }else{
                        link->setJavaOwnership(env);
                    }
                }
                EventDispatcherCheck::CleanupFunction cleaner = &thread_cleaner;
                new EventDispatcherCheck(env, java_thread, qt_thread, link, cleaner);
            }
        }else{
            env->DeleteLocalRef(jthread);
        }
        env->DeleteLocalRef(javaQThread);
        javaQThread = nullptr;
    }
}

jobject fromQThread(JNIEnv * env, jobject java_qthread, QThread *thread)
{
    if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForQObject(thread)){
        link->setNoThreadInitializationOnPurge(true);
    }
    if(thread == QThread::currentThread()){
        jobject currentThread = Java::Runtime::Thread::currentThread(env);
        adoptThread(env, currentThread, java_qthread, thread);
        return currentThread;
    }else{
        if(thread->eventDispatcher()){
            QThreadUserData* qthreadData = QThreadUserData::ensureThreadUserData(thread);
            JObjectWrapper _java_qthread(env, java_qthread);
            auto fun = [_java_qthread](){
                if(JniEnvironment env{500}){
                    jobject currentThread = Java::Runtime::Thread::currentThread(env);
                    adoptThread(env, currentThread, _java_qthread.object(), QThread::currentThread());
                }
            };
            QCoreApplication::postEvent(qthreadData->threadDeleter(), new FunctorEvent<decltype(fun)>(std::move(fun)), INT_MAX);
        }
    }
    return nullptr;
}

} // namespace ThreadPrivate

jobject ThreadAPI::findJThreadForQThread(JNIEnv * env, jobject qt_thread)
{
    QThread* thread = QtJambiAPI::convertJavaObjectToQObject<QThread>(env, qt_thread);
    QtJambiAPI::checkNullPointer(env, thread);
    return ThreadPrivate::fromQThread(env, qt_thread, thread);
}

QThreadStorage<EventDispatcherCheckPointer> EventDispatcherCheck::Instance;

EventDispatcherCheck::EventDispatcherCheck(JNIEnv *env, jobject jthreadObject, QThread* thread, QWeakPointer<QtJambiLink>&& wlink, CleanupFunction _cleaner)
    : QSharedData(),
      m_data(new Data{env->NewWeakGlobalRef(jthreadObject),
                      Java::Runtime::System::identityHashCode(env, jthreadObject),
                      thread,
                      std::move(wlink), {}}),
      cleaner(_cleaner) {
    QTJAMBI_INTERNAL_METHOD_CALL("EventDispatcherCheck::EventDispatcherCheck()")
    QThreadUserData* data;
    {
        QReadLocker locker(QtJambiLinkUserData::lock());
        data = QTJAMBI_GET_OBJECTUSERDATA(QThreadUserData, thread);
    }
    if(data){
        data->m_finalActions = &m_data->m_finalActions;
        data->m_mutex = &m_mutex;
    }
    EventDispatcherCheck::Instance.localData() = this;
}

EventDispatcherCheck::Data::~Data(){
}

EventDispatcherCheck::~EventDispatcherCheck(){
    QTJAMBI_INTERNAL_METHOD_CALL("EventDispatcherCheck::~EventDispatcherCheck()")
    QThreadUserData* data;
    {
        QReadLocker locker(QtJambiLinkUserData::lock());
        data = QTJAMBI_GET_OBJECTUSERDATA(QThreadUserData, m_data->m_thread);
    }
    if(data){
        data->m_finalActions = nullptr;
        data->m_mutex = nullptr;
    }
    try{
        if(!QCoreApplicationPrivate::is_app_closing && cleaner){
            cleaner(m_data);
            m_data = nullptr;
            cleaner = nullptr;
        }else{
            while(!m_data->m_finalActions.isEmpty()){
                m_data->m_finalActions.takeFirst()();
            }
            m_data->m_finalActions.clear();
            if(data && !data->purgeOnExit()){
                QWriteLocker locker(QtJambiLinkUserData::lock());
                QTJAMBI_SET_OBJECTUSERDATA(QThreadUserData, m_data->m_thread, nullptr);
                delete data;
            }
            delete m_data;
            m_data = nullptr;
        }
    } catch (const std::exception& e) {
        qCWarning(DebugAPI::internalCategory, "%s", e.what());
    } catch (...) {
    }
}

void QThreadUserData::deleteAtThreadEnd(QObject* object){
    MutexLocker locker(m_mutex);
    Q_UNUSED(locker)
    m_objectsForDeletion.append(QPointer<QObject>(object));
}


void QThreadUserData::doAtThreadEnd(QtJambiUtils::Runnable&& action){
    if(m_finalActions){
        MutexLocker locker(m_mutex);
        Q_UNUSED(locker)
        m_finalActions->append(std::move(action));
    }
}

void ThreadAPI::initializeThread(JNIEnv *env, jobject jthread, jobject threadGroup)
{
    QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, jthread);
    Q_ASSERT(link);
    link->setNoThreadInitializationOnPurge(true);
    QThread* thread = reinterpret_cast<QThread*>(link->pointer());
    Q_ASSERT(thread);
    QThreadUserData* threadData = QTJAMBI_GET_OBJECTUSERDATA(QThreadUserData, thread);
    if(!threadData){
        threadData = new QThreadUserData(env, threadGroup);
        threadData->initialize(thread);
        QTJAMBI_SET_OBJECTUSERDATA(QThreadUserData, thread, threadData);
    }
    jobject current_jthread = Java::Runtime::Thread::currentThread(env);
    jobject contextClassLoader = Java::Runtime::Thread::getContextClassLoader(env, current_jthread);
    if(!contextClassLoader){
        contextClassLoader = Java::Runtime::Class::getClassLoader(env, Java::QtCore::QThread::getClass(env));
    }
    threadData->setContextClassLoader(env, contextClassLoader);
    threadData->setDaemon(Java::Runtime::Thread::isDaemon(env, current_jthread));
    QSharedPointer<QMetaObject::Connection> connection(new QMetaObject::Connection());
    *connection = QObject::connect(thread, &QThread::started, [connection](){
        JniEnvironment env;
        QObject::disconnect(*connection);
    });
}

void ThreadAPI::setDaemon(JNIEnv *__jni_env, jobject jthread, bool daemon)
{
    QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(__jni_env, jthread);
    Q_ASSERT(link);
    QThread* thread = reinterpret_cast<QThread*>(link->pointer());
    Q_ASSERT(thread);
    QThreadUserData* threadData;
    bool init = false;
    {
        QWriteLocker locker(QtJambiLinkUserData::lock());
        threadData = QTJAMBI_GET_OBJECTUSERDATA(QThreadUserData, thread);
        if(!threadData){
            threadData = new QThreadUserData();
            init = true;
            QTJAMBI_SET_OBJECTUSERDATA(QThreadUserData, thread, threadData);
        }
    }
    if(init)
        threadData->initialize(thread);
    threadData->setDaemon(daemon);
}

bool ThreadAPI::isDaemon(QThread* thread)
{
    QReadLocker locker(QtJambiLinkUserData::lock());
    if(QThreadUserData* threadData = QTJAMBI_GET_OBJECTUSERDATA(QThreadUserData, thread)){
        return threadData->isDaemon();
    }
    return false;
}

void ThreadAPI::setName(JNIEnv *__jni_env, jobject jthread, jstring name)
{
    QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(__jni_env, jthread);
    Q_ASSERT(link);
    QThread* thread = reinterpret_cast<QThread*>(link->pointer());
    Q_ASSERT(thread);
    QThreadUserData* threadData;
    bool init = false;
    {
        QWriteLocker locker(QtJambiLinkUserData::lock());
        threadData = QTJAMBI_GET_OBJECTUSERDATA(QThreadUserData, thread);
        if(!threadData){
            threadData = new QThreadUserData();
            init = true;
            QTJAMBI_SET_OBJECTUSERDATA(QThreadUserData, thread, threadData);
        }
    }
    if(init)
        threadData->initialize(thread);
    QString _name = qtjambi_cast<QString>(__jni_env, name);
    threadData->setName(_name.toUtf8());
}

jstring ThreadAPI::getName(JNIEnv *__jni_env, QThread* thread)
{
    QReadLocker locker(QtJambiLinkUserData::lock());
    if(QThreadUserData* threadData = QTJAMBI_GET_OBJECTUSERDATA(QThreadUserData, thread)){
        return qtjambi_cast<jstring>(__jni_env, QLatin1String(threadData->getName()));
    }
    return nullptr;
}

jobject ThreadAPI::getThreadGroup(JNIEnv *__jni_env, QThread* thread)
{
    QReadLocker locker(QtJambiLinkUserData::lock());
    if(QThreadUserData* threadData = QTJAMBI_GET_OBJECTUSERDATA(QThreadUserData, thread)){
        return __jni_env->NewLocalRef(threadData->getThreadGroup());
    }
    return nullptr;
}

void ThreadAPI::setUncaughtExceptionHandler(JNIEnv *__jni_env, jobject jthread, jobject handler)
{
    QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(__jni_env, jthread);
    Q_ASSERT(link);
    QThread* thread = reinterpret_cast<QThread*>(link->pointer());
    Q_ASSERT(thread);
    bool init = false;
    QThreadUserData* threadData;
    {
        QWriteLocker locker(QtJambiLinkUserData::lock());
        threadData = QTJAMBI_GET_OBJECTUSERDATA(QThreadUserData, thread);
        if(!threadData){
            threadData = new QThreadUserData();
            init = true;
            QTJAMBI_SET_OBJECTUSERDATA(QThreadUserData, thread, threadData);
        }
    }
    if(init)
        threadData->initialize(thread);
    threadData->setUncaughtExceptionHandler(__jni_env, handler);
}

jobject ThreadAPI::getUncaughtExceptionHandler(JNIEnv *__jni_env, QThread* thread)
{
    QReadLocker locker(QtJambiLinkUserData::lock());
    if(QThreadUserData* threadData = QTJAMBI_GET_OBJECTUSERDATA(QThreadUserData, thread)){
        return __jni_env->NewLocalRef(threadData->getUncaughtExceptionHandler());
    }
    return nullptr;
}

void ThreadAPI::setContextClassLoader(JNIEnv *__jni_env, jobject jthread, jobject cl)
{
    QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(__jni_env, jthread);
    Q_ASSERT(link);
    QThread* thread = reinterpret_cast<QThread*>(link->pointer());
    Q_ASSERT(thread);
    bool init = false;
    QThreadUserData* threadData;
    {
        QWriteLocker locker(QtJambiLinkUserData::lock());
        threadData = QTJAMBI_GET_OBJECTUSERDATA(QThreadUserData, thread);
        if(!threadData){
            threadData = new QThreadUserData();
            init = true;
            QTJAMBI_SET_OBJECTUSERDATA(QThreadUserData, thread, threadData);
        }
    }
    if(init)
        threadData->initialize(thread);
    threadData->setContextClassLoader(__jni_env, cl);
}

jobject ThreadAPI::getContextClassLoader(JNIEnv *__jni_env, QThread* thread)
{
    QReadLocker locker(QtJambiLinkUserData::lock());
    if(QThreadUserData* threadData = QTJAMBI_GET_OBJECTUSERDATA(QThreadUserData, thread)){
        return __jni_env->NewLocalRef(threadData->getContextClassLoader());
    }
    return nullptr;
}

jobject ThreadAPI::findQThreadForJThread(JNIEnv *env, jobject thread){
    jobject associatedObject = Java::QtJambi::NativeUtility::findAssociation(env, thread);
    if(Java::QtCore::QThread::isInstanceOf(env, associatedObject)){
        return associatedObject;
    }
    return nullptr;
}

void ThreadAPI::initializeCurrentThread(JNIEnv *env)
{
    if(QThread* currentQThread = QThread::currentThread()){
        QReadLocker locker(QtJambiLinkUserData::lock());
        if(!QTJAMBI_GET_OBJECTUSERDATA(QThreadUserData, currentQThread)){
            locker.unlock();
            qtjambi_cast<jobject>(env, currentQThread);
            locker.relock();
        }
    }else{
        fprintf(stderr, "QThread::currentThread() returning nullptr");
    }
}

void ThreadAPI::initializeMainThread(JNIEnv *__jni_env)
{
    {
        QCoreApplicationPrivate::theMainThread = reinterpret_cast<QThread*>(1);
        QThread* currentQThread = QThread::currentThread();
        QCoreApplicationPrivate::theMainThread = currentQThread;
        qtjambi_cast<jobject>(__jni_env, currentQThread);
    }
}
