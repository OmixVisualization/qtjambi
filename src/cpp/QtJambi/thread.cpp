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
#include <QtCore/QSemaphore>
#include <stdio.h>
#include <string.h>

#define QTJAMBI_INTERNAL_METHOD_CALL(methodname)\
DebugAPI::MethodPrint __debug_method_print(DebugAPI::MethodPrint::Internal, methodname, __FILE__, __LINE__, Q_FUNC_INFO);

#if defined(Q_OS_LINUX)
#include <sys/syscall.h>
#endif

#include "qtjambi_cast.h"

bool threadRequiresCoreApplication(QThread* thread){
#if QT_VERSION < QT_VERSION_CHECK(6, 9, 0)
    Q_UNUSED(thread)
    return QCoreApplicationPrivate::threadRequiresCoreApplication();
#else
    QThreadData *data = QThreadData::get2(thread);
    if (Q_UNLIKELY(!data))
        return true;    // default setting
    return data->requiresCoreApplication;
#endif
}

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

struct QThreadUserDataPrivate : public QSharedData{
    QScopedPointer<QObject> m_threadDeleter{new Deleter};
    QList<QPointer<QObject>> m_objectsForDeletion;
    QWeakPointer<QList<QtJambiUtils::Runnable>> m_finalActions;
    QSharedPointer<QMutex> m_mutex{new QMutex()};
    void cleanup(bool isInDestructor);
};

QThreadUserData::QThreadUserData():
    QtJambiObjectData(),
    p(new QThreadUserDataPrivate),
    m_threadType(DefaultThread)
{
}

QThreadUserData::~QThreadUserData(){
    QTJAMBI_INTERNAL_METHOD_CALL("QThreadUserData::~QThreadUserData()")
    switch(m_threadType){
    case ProcessMainThread:
        p->cleanup(true);
        break;
    default:
        if(m_finishedConnection){
            QObject::disconnect(m_finishedConnection);
            p->cleanup(true);
        }
        break;
    }
}

QObject* QThreadUserData::threadDeleter() const {return p->m_threadDeleter.get();}

void QThreadUserData::initializeDefault(QThread* thread){
    p->m_threadDeleter->moveToThread(thread);
    m_threadType = QThreadData::get2(thread)->isAdopted ? AdoptedThread : DefaultThread;
    m_finishedConnection = QObject::connect(thread, &QThread::finished, thread, [p = this->p]() {
        QTJAMBI_INTERNAL_METHOD_CALL("QThread::finished received")
        p->cleanup(false);
    }, Qt::DirectConnection);
}

void QThreadUserData::initializeAdopted(QThread* thread){
    m_isJavaLaunched = true;
    p->m_threadDeleter->moveToThread(thread);
    m_threadType = AdoptedThread;
    m_finishedConnection = QObject::connect(thread, &QThread::finished, thread, [p = this->p]() {
        QTJAMBI_INTERNAL_METHOD_CALL("QThread::finished received")
        p->cleanup(false);
    }, Qt::DirectConnection);
}

void QThreadUserData::reinitializeMain(QThread* thread){
#if defined(Q_OS_LINUX)
    m_threadType = quintptr(syscall(SYS_gettid)) == quintptr(getpid()) ? ProcessMainThread : VirtualMainThread;
#elif defined(Q_OS_MACOS)
    m_threadType = pthread_main_np() ? ProcessMainThread : VirtualMainThread;
#elif defined(Q_OS_WIN)
    m_threadType = ProcessMainThread;
#endif
    QObject::disconnect(m_finishedConnection);
    switch(m_threadType){
    case ProcessMainThread:
        if(m_finishedConnection){
            QObject::disconnect(m_finishedConnection);
            m_finishedConnection = QMetaObject::Connection();
        }
        break;
    case VirtualMainThread:
        m_finishedConnection = QObject::connect(thread, &QThread::finished, thread, [p = this->p]() {
            QCoreApplicationPrivate::theMainThread.storeRelaxed(nullptr);
#if QT_VERSION >= QT_VERSION_CHECK(6, 8, 0)
            QCoreApplicationPrivate::theMainThreadId.storeRelaxed(nullptr);
#else
            QThreadUserData::theMainThreadId.storeRelaxed(nullptr);
#endif
            QTJAMBI_INTERNAL_METHOD_CALL("QThread::finished received")
            p->cleanup(false);
        }, Qt::DirectConnection);
        break;
    default:
        break;
    }
}

void QThreadUserDataPrivate::cleanup(bool isInDestructor){
    QTJAMBI_INTERNAL_METHOD_CALL("QThreadUserData::cleanup()")
    QList<QPointer<QObject>> objectsForDeletion;
    {
        MutexLocker locker(m_mutex.data());
        objectsForDeletion.swap(m_objectsForDeletion);
    }
    for(const QPointer<QObject>& ptr : objectsForDeletion){
        if(ptr)
            delete ptr.data();
    }
    Q_UNUSED(isInDestructor)
    m_threadDeleter.reset();
}

#if QT_VERSION < QT_VERSION_CHECK(6, 8, 0)
QBasicAtomicPointer<void> QThreadUserData::theMainThreadId = Q_BASIC_ATOMIC_INITIALIZER(nullptr);
#endif

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
        threadData->initializeDefault(thread);
    return threadData;
}

QTJAMBI_OBJECTUSERDATA_ID_IMPL(,QThreadUserData::)

QThreadInitializationUserData::QThreadInitializationUserData(JNIEnv *env, jobject threadGroup)
    : QtJambiObjectData(),
    m_threadGroup(env->NewGlobalRef(threadGroup)),
    m_isDaemon(true){
}

QThreadInitializationUserData::QThreadInitializationUserData()
    : QtJambiObjectData(),
    m_threadGroup(nullptr),
    m_isDaemon(true){
}

QThreadInitializationUserData::~QThreadInitializationUserData()
{
    if(!m_uncaughtExceptionHandler.isNull() || m_threadGroup || !m_contextClassLoader.isNull()){
        if(DefaultJniEnvironment env{200}){
            m_contextClassLoader.clear(env);
            m_uncaughtExceptionHandler.clear(env);
            if(m_threadGroup){
                env->DeleteGlobalRef(m_threadGroup);
                m_threadGroup = nullptr;
            }
        }
    }
}

jobject QThreadInitializationUserData::getThreadGroup() const { return m_threadGroup; }

jobject QThreadInitializationUserData::getUncaughtExceptionHandler(JNIEnv *env) const { return m_uncaughtExceptionHandler.object(env); }

void QThreadInitializationUserData::setUncaughtExceptionHandler(JNIEnv *env, jobject uncaughtExceptionHandler) {
    m_uncaughtExceptionHandler.assign(env, uncaughtExceptionHandler);
}

jobject QThreadInitializationUserData::getContextClassLoader(JNIEnv *env) const { return m_contextClassLoader.object(env); }

void QThreadInitializationUserData::setContextClassLoader(JNIEnv *env, jobject contextClassLoader) {
    m_contextClassLoader.assign(env, contextClassLoader);
}

void QThreadInitializationUserData::clear(JNIEnv *env) {
    m_contextClassLoader.clear(env);
    if(m_threadGroup){
        env->DeleteGlobalRef(m_threadGroup);
        m_threadGroup = nullptr;
    }
    m_uncaughtExceptionHandler.clear(env);
}

QTJAMBI_OBJECTUSERDATA_ID_IMPL(,QThreadInitializationUserData::)

QThreadAdoptionUserData::QThreadAdoptionUserData(JNIEnv * env, jobject java_qthread) : QtJambiObjectData(), m_java_object(env, java_qthread){}

void QThreadAdoptionUserData::clear(JNIEnv* env){
    m_java_object.clear(env);
}
jobject QThreadAdoptionUserData::getLocalRef(JNIEnv* env){
    return m_java_object.object(env);
}

QThreadAdoptionUserData::~QThreadAdoptionUserData(){}

QTJAMBI_OBJECTUSERDATA_ID_IMPL(,QThreadAdoptionUserData::)

namespace ThreadPrivate{

QThread* currentThread(){
    /*if(!QCoreApplicationPrivate::theMainThread.loadRelaxed()){
        QCoreApplicationPrivate::theMainThread.storeRelease(reinterpret_cast<QThread*>(1));
#if QT_VERSION >= QT_VERSION_CHECK(6, 8, 0)
        QCoreApplicationPrivate::theMainThreadId.storeRelaxed(reinterpret_cast<void*>(1));
#endif
        QThread* currentQThread = QThread::currentThread();
        QCoreApplicationPrivate::theMainThread.storeRelease(currentQThread);
#if QT_VERSION >= QT_VERSION_CHECK(6, 8, 0)
        QCoreApplicationPrivate::theMainThreadId.storeRelaxed(QThreadData::get2(currentQThread)->threadId.loadRelaxed());
#else
        QThreadUserData::theMainThreadId.storeRelaxed(QThreadData::get2(currentQThread)->threadId.loadRelaxed());
#endif
        return currentQThread;
    }else
    */{
        return QThread::currentThread();
    }
}

void purgeThread(JNIEnv* env, EventDispatcherCheck::Data* threadData){
    QSharedPointer<QtJambiLink> link;
    {
        QReadLocker locker(QtJambiLinkUserData::lock());
        link = threadData->m_wlink.toStrongRef();
        threadData->m_wlink = QWeakPointer<QtJambiLink>{};
    }
    if(!threadData->m_isAdoptedQThread
        && link
        && link->qobject()
        && !link->qobject()->parent()
        && link->qobject()!=QCoreApplicationPrivate::theMainThread.loadRelaxed()){
        link->setJavaOwnership(env);
    }
    if(threadData->m_jthreadObject){
        jobject jthreadObject = threadData->m_jthreadObject;
        threadData->m_jthreadObject = nullptr;
        if(!env->IsSameObject(jthreadObject, nullptr)){
            try{
                Java::QtJambi::ThreadUtility::setThreadInterruptible(env, nullptr, jthreadObject, false);
            } catch (const JavaException& exn) {
                exn.report(env);
            }
            env->DeleteWeakGlobalRef(jthreadObject);
            jthreadObject = nullptr;
        }
    }
    if(env->ExceptionCheck()){
        env->ExceptionDescribe();
        env->ExceptionClear();
    }
}

class ThreadPurger : public AbstractThreadEvent{
    EventDispatcherCheck::Data* m_threadData;
public:
    ThreadPurger(EventDispatcherCheck::Data* threadData)
        : AbstractThreadEvent(),
        m_threadData(threadData){
        QTJAMBI_INTERNAL_METHOD_CALL("ThreadPurger::ThreadPurger()")
    }
    ThreadPurger(const ThreadPurger& other) : AbstractThreadEvent(other), m_threadData(other.m_threadData) {}
    ThreadPurger* clone() const override
    {
        return new ThreadPurger(*this);
    }
    void execute() override{
        QTJAMBI_INTERNAL_METHOD_CALL("ThreadPurger::~ThreadPurger()")
        if(m_threadData){
            if(JniEnvironment env{200}){
                try{
                    ThreadPrivate::purgeThread(env, m_threadData);
                }catch(...){}
            }else{
                QTJAMBI_DEBUG_PRINT("Unable to cleanup thread due to missing JNI")
            }
            delete m_threadData;
            m_threadData = nullptr;
        }
    }
};

void eventless_thread_cleaner(JNIEnv *env, EventDispatcherCheck::Data* threadData){
    bool noDeletion = false;
    if(env){
        try{
            ThreadPrivate::purgeThread(env, threadData);
        }catch(...){}
    }else if(threadData->m_threadId==QThread::currentThreadId()){
        if(JniEnvironment env{200}){
            try{
                ThreadPrivate::purgeThread(env, threadData);
            }catch(...){}
        }else{
            QThreadUserData* mainThreadData{nullptr};
            if(QThread* thread = QCoreApplicationPrivate::theMainThread.loadRelaxed()){
                QReadLocker locker(QtJambiLinkUserData::lock());
                mainThreadData = QTJAMBI_GET_OBJECTUSERDATA(QThreadUserData, thread);
            }
            if(mainThreadData){
                QCoreApplication::postEvent(mainThreadData->threadDeleter(), new ThreadPurger(threadData));
                noDeletion = true;
            }
        }
    }else{
        if(DefaultJniEnvironment env{200})
            ThreadPrivate::purgeThread(env, threadData);
        else{
            QThreadUserData* mainThreadData{nullptr};
            if(QThread* thread = QCoreApplicationPrivate::theMainThread.loadRelaxed()){
                QReadLocker locker(QtJambiLinkUserData::lock());
                mainThreadData = QTJAMBI_GET_OBJECTUSERDATA(QThreadUserData, thread);
            }
            if(mainThreadData){
                QCoreApplication::postEvent(mainThreadData->threadDeleter(), new ThreadPurger(threadData));
                noDeletion = true;
            }
        }
    }
    QList<QtJambiUtils::Runnable> finalActions;
    {
        MutexLocker locker(threadData->m_mutex.data());
        threadData->m_finalActions->swap(finalActions);
    }
    while(!finalActions.isEmpty()){
        finalActions.takeFirst()();
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
    if(!noDeletion)
        delete threadData;
}
struct ThreadDataReleaser : public QtJambiObjectData {
    EventDispatcherCheck::Data* m_threadData;
    ThreadDataReleaser(EventDispatcherCheck::Data* threadData)
        : QtJambiObjectData(),
        m_threadData(threadData) {
        QTJAMBI_INTERNAL_METHOD_CALL("ThreadDataReleaser::ThreadDataReleaser()")
    }
    typedef void(*Finisher)(EventDispatcherCheck::Data* threadData, Qt::HANDLE currentThread);
    ~ThreadDataReleaser() override {
        Qt::HANDLE currentThread = QThread::currentThreadId();
        bool noDeletion = false;
        QTJAMBI_INTERNAL_METHOD_CALL("ThreadDataReleaser::~ThreadDataReleaser()")
        if(m_threadData->m_threadId==currentThread){
            if(JniEnvironment env{200}){
                try{
                    ThreadPrivate::purgeThread(env, m_threadData);
                }catch(...){}
            }else{
                QThreadUserData* mainThreadData{nullptr};
                if(QThread* thread = QCoreApplicationPrivate::theMainThread.loadRelaxed()){
                    QReadLocker locker(QtJambiLinkUserData::lock());
                    mainThreadData = QTJAMBI_GET_OBJECTUSERDATA(QThreadUserData, thread);
                }
                if(mainThreadData){
                    QCoreApplication::postEvent(mainThreadData->threadDeleter(), new ThreadPurger(m_threadData));
                    noDeletion = true;
                }
            }
        }else{
            if(DefaultJniEnvironment env{200})
                try{
                    ThreadPrivate::purgeThread(env, m_threadData);
                }catch(...){}
            else{
                QThreadUserData* mainThreadData{nullptr};
                if(QThread* thread = QCoreApplicationPrivate::theMainThread.loadRelaxed()){
                    QReadLocker locker(QtJambiLinkUserData::lock());
                    mainThreadData = QTJAMBI_GET_OBJECTUSERDATA(QThreadUserData, thread);
                }
                if(mainThreadData){
                    QCoreApplication::postEvent(mainThreadData->threadDeleter(), new ThreadPurger(m_threadData));
                    noDeletion = true;
                }
            }
        }
        QList<QtJambiUtils::Runnable> finalActions;
        {
            MutexLocker locker(m_threadData->m_mutex.data());
            m_threadData->m_finalActions->swap(finalActions);
        }
        while(!finalActions.isEmpty()){
            finalActions.takeFirst()();
        }
        QThreadUserData* data{nullptr};
        if(QThread* thread = m_threadData->m_thread.data()){
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
        if(!noDeletion)
            delete m_threadData;
    }
    QTJAMBI_OBJECTUSERDATA_ID_IMPL(static,)
};

void thread_cleaner(JNIEnv *env, EventDispatcherCheck::Data* threadData){
#ifndef Q_OS_ANDROID
    QAbstractEventDispatcher * eventDispatcher = nullptr;
    if(QThread* thread = threadData->m_thread.data()){
        if(!(eventDispatcher = thread->eventDispatcher())){
            if (QThread::currentThreadId()==threadData->m_threadId && (QCoreApplication::instance() || !threadRequiresCoreApplication(thread))) {
                QEventLoop loop;
                eventDispatcher = thread->eventDispatcher();
            }
        }
    }
    if(eventDispatcher){
        QWriteLocker locker(QtJambiLinkUserData::lock());
        QTJAMBI_SET_OBJECTUSERDATA_ID(QTJAMBI_OBJECTUSERDATA_TYPE_ID(ThreadDataReleaser), eventDispatcher, new ThreadDataReleaser(threadData));
    }else
#endif
        eventless_thread_cleaner(env, threadData);
}

jobject adoptCurrentJavaThread(JNIEnv *env, QThread *qt_thread, QSharedPointer<QtJambiLink> link)
{
    jobject java_thread = Java::Runtime::Thread::currentThread(env);
    QString threadName = qtjambi_cast<QString>(env, Java::Runtime::Thread::getName(env, java_thread));
#if QT_VERSION >= QT_VERSION_CHECK(6, 5, 0)
    {
        QObjectPrivate* opvt = QObjectPrivate::get(qt_thread);
#if QT_VERSION >= QT_VERSION_CHECK(6, 9, 0)
        opvt->setObjectNameWithoutBindings(threadName);
#else
        opvt->ensureExtraData();
        opvt->extraData->objectName.setValueBypassingBindings(threadName);
#endif
    }
#else
    qt_thread->setObjectName(threadName);
#endif
    Q_ASSERT(link);
    if(jobject javaQThread = link->getJavaObjectLocalRef(env)){
        QThreadUserData* threadData;
        QThreadInitializationUserData* threadInitializationData;
        {
            QReadLocker locker(QtJambiLinkUserData::lock());
            threadData = QTJAMBI_GET_OBJECTUSERDATA(QThreadUserData, qt_thread);
            threadInitializationData = QTJAMBI_GET_OBJECTUSERDATA(QThreadInitializationUserData, qt_thread);
        }
        bool init = false;
        if(threadInitializationData){
            if(jobject uncaughtExceptionHandler = threadInitializationData->getUncaughtExceptionHandler(env)){
                Java::Runtime::Thread::setUncaughtExceptionHandler(env, java_thread, uncaughtExceptionHandler);
            }
            if(jobject contextClassLoader = threadInitializationData->getContextClassLoader(env)){
                Java::Runtime::Thread::setContextClassLoader(env, java_thread, contextClassLoader);
            }
            threadInitializationData->clear(env);
        }
        if(threadData){
            if(threadInitializationData){
                QWriteLocker locker(QtJambiLinkUserData::lock());
                QTJAMBI_SET_OBJECTUSERDATA(QThreadInitializationUserData, qt_thread, nullptr);
            }
        }else{
            QWriteLocker locker(QtJambiLinkUserData::lock());
            threadData = new QThreadUserData();
            init = true;
            QTJAMBI_SET_OBJECTUSERDATA(QThreadUserData, qt_thread, threadData);
            if(threadInitializationData)
                QTJAMBI_SET_OBJECTUSERDATA(QThreadInitializationUserData, qt_thread, nullptr);
        }
        if(init)
            threadData->initializeAdopted(qt_thread);
        Java::QtCore::Internal::QThread::setJavaThreadReference(env, javaQThread, java_thread);
        jint associationHashcode = Java::QtJambi::NativeUtility::createAssociation(env, java_thread, javaQThread);
        Java::QtJambi::ThreadUtility::setThreadInterruptible(env, javaQThread, java_thread, true);
        EventDispatcherCheck::adoptThread(env, java_thread, qt_thread, threadData, associationHashcode, link, &thread_cleaner);
        env->DeleteLocalRef(javaQThread);
    }
    return java_thread;
}

jobject fromQThread(JNIEnv * env, jobject java_qthread, QThread *thread, const QSharedPointer<QtJambiLink>& link)
{
    Q_ASSERT(link);
    QThreadData* threadData = QThreadData::get2(thread);
#if QT_VERSION >= QT_VERSION_CHECK(6, 8, 0)
    if(thread->isCurrentThread()){
#else
    if(threadData->threadId.loadRelaxed() == QThread::currentThreadId()){
#endif
        return adoptCurrentJavaThread(env, thread, link);
    }else{
        if(thread->isRunning()){
            if(!threadData->isAdopted || thread->eventDispatcher()){
                QObject* initializer = new QObject();
                QObject::connect(initializer, &QObject::destroyed, initializer, [wrapper = JObjectWrapper(env, java_qthread)]() mutable {
                    if(JniEnvironment env{0}){
                        try{
                            wrapper.clear(env);
                        }catch(...){}
                    }
                }, Qt::DirectConnection);
                initializer->moveToThread(thread);
                initializer->deleteLater();
            }else if(link->ownership()==QtJambiLink::Ownership::Java){
                QWriteLocker locker(QtJambiLinkUserData::lock());
                QTJAMBI_SET_OBJECTUSERDATA(QThreadAdoptionUserData, thread, new QThreadAdoptionUserData(env, java_qthread));
            }
        }else if(!thread->isFinished()){
            if(!threadData->isAdopted){
                QObject::connect(thread, &QThread::started, thread, [wrapper = JObjectWrapper(env, java_qthread)]() mutable {
                    if(JniEnvironment env{0}){
                        try{
                            wrapper.clear(env);
                        }catch(...){}
                    }
                }, Qt::DirectConnection);
            }else if(link->ownership()==QtJambiLink::Ownership::Java){
                QWriteLocker locker(QtJambiLinkUserData::lock());
                QTJAMBI_SET_OBJECTUSERDATA(QThreadAdoptionUserData, thread, new QThreadAdoptionUserData(env, java_qthread));
            }
        }
    }
    return nullptr;
}

} // namespace ThreadPrivate

jobject ThreadAPI::findJThreadForQThread(JNIEnv * env, QtJambiNativeID thread_nid)
{
    QSharedPointer<QtJambiLink> link = QtJambiLink::fromNativeId(thread_nid);
    QThread* thread = link ? static_cast<QThread*>(link->qobject()) : nullptr;
    QtJambiAPI::checkNullPointer(env, thread);
    return ThreadPrivate::fromQThread(env, link->getJavaObjectLocalRef(env), thread, link);
}

QThreadStorage<EventDispatcherCheckPointer> EventDispatcherCheck::Instance;

void EventDispatcherCheck::adoptThread(JNIEnv *env, jobject jthreadObject, QThread* thread, QThreadUserData* tdata, jint associationHashcode, QWeakPointer<QtJambiLink>&& wlink, CleanupFunction _cleaner){
    Qt::HANDLE threadId = nullptr;
    bool isAdoptedQThread = false;
    if(thread){
        if(QThreadData* td = QThreadData::get2(thread)){
            threadId = td->threadId.loadRelaxed();
            isAdoptedQThread = td->isAdopted;
        }
    }else{
        threadId = QThread::currentThreadId();
    }
    EventDispatcherCheck* instance = new EventDispatcherCheck(*new Data{jthreadObject ? env->NewWeakGlobalRef(jthreadObject) : nullptr,
                                                                        thread,
                                                                        threadId,
                                                                        isAdoptedQThread, associationHashcode,
                                                                        std::move(wlink), {}}, _cleaner);
    QTJAMBI_INTERNAL_METHOD_CALL("EventDispatcherCheck::EventDispatcherCheck()")
    if(thread && !tdata){
        QReadLocker locker(QtJambiLinkUserData::lock());
        tdata = QTJAMBI_GET_OBJECTUSERDATA(QThreadUserData, thread);
    }
    if(tdata){
        tdata->p->m_finalActions = instance->m_data->m_finalActions;
        instance->m_data->m_mutex = tdata->p->m_mutex;
    }else{
        instance->m_data->m_mutex.reset(new QMutex());
    }
    EventDispatcherCheck::Instance.localData() = instance;
}

void EventDispatcherCheck::detach(JNIEnv *env){
    EventDispatcherCheckPointer instance;
    instance.swap(EventDispatcherCheck::Instance.localData());
    if(instance){
        instance->finalize(env);
    }
}

bool EventDispatcherCheck::isAlive(){
    return EventDispatcherCheck::Instance.localData();
}

EventDispatcherCheck::EventDispatcherCheck(Data& data, CleanupFunction _cleaner)
    : QSharedData(),
    m_data(&data),
    m_cleaner(_cleaner) {}

EventDispatcherCheck::Data::~Data(){
}

EventDispatcherCheck::~EventDispatcherCheck(){
    finalize(nullptr);
}

void EventDispatcherCheck::finalize(JNIEnv *env){
    if(m_cleaner || m_data){
        Data* data = m_data;
        CleanupFunction cleaner = m_cleaner;
        m_data = nullptr;
        m_cleaner = nullptr;
        QThreadUserData* threadUserData{nullptr};
        if(QThread* thread = data ? data->m_thread.data() : nullptr){
            QReadLocker locker(QtJambiLinkUserData::lock());
            threadUserData = QTJAMBI_GET_OBJECTUSERDATA(QThreadUserData, thread);
        }
        try{
            if(!QCoreApplicationPrivate::is_app_closing && cleaner){
                if(threadUserData)
                    threadUserData->p->m_finalActions.clear();
                cleaner(env, data);
            }else if(data){
                QList<QtJambiUtils::Runnable> finalActions;
                if(threadUserData)
                    threadUserData->p->m_finalActions.clear();
                {
                    MutexLocker locker(data->m_mutex.data());
                    data->m_finalActions->swap(finalActions);
                }
                while(!finalActions.isEmpty()){
                    finalActions.takeFirst()();
                }
                if(threadUserData && !threadUserData->purgeOnExit()){
                    if(QThread* thread = data->m_thread.data()){
                        QWriteLocker locker(QtJambiLinkUserData::lock());
                        QTJAMBI_SET_OBJECTUSERDATA(QThreadUserData, thread, nullptr);
                    }
                    delete threadUserData;
                }
                delete data;
            }
        } catch (const std::exception& e) {
            qCWarning(DebugAPI::internalCategory, "%s", e.what());
        } catch (...) {
        }
    }
}

LinkInvalidator::LinkInvalidator(QSharedPointer<QtJambiLink>&& link) : AbstractThreadEvent(), m_link(std::move(link)) {}
LinkInvalidator::LinkInvalidator(const LinkInvalidator& other) : AbstractThreadEvent(other), m_link(other.m_link) {}
LinkInvalidator* LinkInvalidator::clone() const
{
    return new LinkInvalidator(*this);
}
void LinkInvalidator::execute() {
    if(JniEnvironment env{200}){
        m_link->invalidate(env);
    }
    m_link.clear();
}

void QThreadUserData::deleteAtThreadEnd(QObject* object){
    MutexLocker locker(p->m_mutex.data());
    Q_UNUSED(locker)
    p->m_objectsForDeletion.append(QPointer<QObject>(object));
}


void QThreadUserData::doAtThreadEnd(QtJambiUtils::Runnable&& action){
    if(QSharedPointer<QList<QtJambiUtils::Runnable>> finalActions = p->m_finalActions){
        MutexLocker locker(p->m_mutex.data());
        finalActions->append(std::move(action));
    }
}

void ThreadAPI::initializeThread(JNIEnv *env, QtJambiNativeID thread_nid, jobject threadGroup)
{
    QSharedPointer<QtJambiLink> link = QtJambiLink::fromNativeId(thread_nid);
    QThread* thread = link ? reinterpret_cast<QThread*>(link->pointer()) : nullptr;
    QtJambiAPI::checkNullPointer(env, thread);
    QThreadUserData* threadData = QTJAMBI_GET_OBJECTUSERDATA(QThreadUserData, thread);
    QThreadInitializationUserData* threadInitializationData = QTJAMBI_GET_OBJECTUSERDATA(QThreadInitializationUserData, thread);
    if(!threadData){
        threadData = new QThreadUserData();
        threadData->initializeDefault(thread);
        QTJAMBI_SET_OBJECTUSERDATA(QThreadUserData, thread, threadData);
    }
    if(!threadInitializationData){
        threadInitializationData = new QThreadInitializationUserData(env, threadGroup);
        QTJAMBI_SET_OBJECTUSERDATA(QThreadInitializationUserData, thread, threadInitializationData);
    }
    jobject current_jthread = Java::Runtime::Thread::currentThread(env);
    jobject contextClassLoader = Java::Runtime::Thread::getContextClassLoader(env, current_jthread);
    if(!contextClassLoader){
        contextClassLoader = Java::Runtime::Class::getClassLoader(env, Java::QtCore::QThread::getClass(env));
    }
    threadInitializationData->setContextClassLoader(env, contextClassLoader);
    threadInitializationData->setDaemon(Java::Runtime::Thread::isDaemon(env, current_jthread));
    QSharedPointer<QMetaObject::Connection> connection(new QMetaObject::Connection());
    *connection = QObject::connect(thread, &QThread::started, [connection](){
        JniEnvironment env;
        QObject::disconnect(*connection);
    });
}

void ThreadAPI::setDaemon(JNIEnv *env, QtJambiNativeID thread_nid, bool daemon)
{
    QSharedPointer<QtJambiLink> link = QtJambiLink::fromNativeId(thread_nid);
    QThread* thread = link ? reinterpret_cast<QThread*>(link->pointer()) : nullptr;
    QtJambiAPI::checkNullPointer(env, thread);
    QThreadInitializationUserData* threadInitializationData;
    {
        QReadLocker locker(QtJambiLinkUserData::lock());
        threadInitializationData = QTJAMBI_GET_OBJECTUSERDATA(QThreadInitializationUserData, thread);
    }
    if(!threadInitializationData){
        threadInitializationData = new QThreadInitializationUserData();
        QWriteLocker locker(QtJambiLinkUserData::lock());
        QTJAMBI_SET_OBJECTUSERDATA(QThreadInitializationUserData, thread, threadInitializationData);
    }
    threadInitializationData->setDaemon(daemon);
}

bool ThreadAPI::isDaemon(JNIEnv *__jni_env, QtJambiNativeID thread_nid)
{
    QThread* thread = QtJambiAPI::objectFromNativeId<QThread>(thread_nid);
    QtJambiAPI::checkNullPointer(__jni_env, thread);
    QThreadInitializationUserData* threadInitializationData;
    {
        QReadLocker locker(QtJambiLinkUserData::lock());
        threadInitializationData = QTJAMBI_GET_OBJECTUSERDATA(QThreadInitializationUserData, thread);
    }
    if(threadInitializationData){
        return threadInitializationData->isDaemon();
    }
    return true;
}

void ThreadAPI::setName(JNIEnv *__jni_env, QtJambiNativeID thread_nid, jstring name)
{
    QSharedPointer<QtJambiLink> link = QtJambiLink::fromNativeId(thread_nid);
    QThread* thread = link ? reinterpret_cast<QThread*>(link->pointer()) : nullptr;
    QtJambiAPI::checkNullPointer(__jni_env, thread);
    QThreadInitializationUserData* threadInitializationData;
    {
        QReadLocker locker(QtJambiLinkUserData::lock());
        threadInitializationData = QTJAMBI_GET_OBJECTUSERDATA(QThreadInitializationUserData, thread);
    }
    if(!threadInitializationData){
        threadInitializationData = new QThreadInitializationUserData();
        QWriteLocker locker(QtJambiLinkUserData::lock());
        QTJAMBI_SET_OBJECTUSERDATA(QThreadInitializationUserData, thread, threadInitializationData);
    }
    QString _name = qtjambi_cast<QString>(__jni_env, name);
    threadInitializationData->setName(_name.toUtf8());
}

jstring ThreadAPI::getName(JNIEnv *__jni_env, QtJambiNativeID thread_nid)
{
    QThread* thread = QtJambiAPI::objectFromNativeId<QThread>(thread_nid);
    QtJambiAPI::checkNullPointer(__jni_env, thread);
    QThreadInitializationUserData* threadInitializationData;
    {
        QReadLocker locker(QtJambiLinkUserData::lock());
        threadInitializationData = QTJAMBI_GET_OBJECTUSERDATA(QThreadInitializationUserData, thread);
    }
    if(threadInitializationData){
        return qtjambi_cast<jstring>(__jni_env, QLatin1String(threadInitializationData->getName()));
    }
    return nullptr;
}

jobject ThreadAPI::getThreadGroup(JNIEnv *__jni_env, QtJambiNativeID thread_nid)
{
    QThread* thread = QtJambiAPI::objectFromNativeId<QThread>(thread_nid);
    QtJambiAPI::checkNullPointer(__jni_env, thread);
    QThreadInitializationUserData* threadInitializationData;
    {
        QReadLocker locker(QtJambiLinkUserData::lock());
        threadInitializationData = QTJAMBI_GET_OBJECTUSERDATA(QThreadInitializationUserData, thread);
    }
    if(threadInitializationData){
        return __jni_env->NewLocalRef(threadInitializationData->getThreadGroup());
    }
    return nullptr;
}

void ThreadAPI::setUncaughtExceptionHandler(JNIEnv *__jni_env, QtJambiNativeID thread_nid, jobject handler)
{
    QSharedPointer<QtJambiLink> link = QtJambiLink::fromNativeId(thread_nid);
    QThread* thread = link ? reinterpret_cast<QThread*>(link->pointer()) : nullptr;
    QtJambiAPI::checkNullPointer(__jni_env, thread);
    QThreadInitializationUserData* threadInitializationData;
    {
        QReadLocker locker(QtJambiLinkUserData::lock());
        threadInitializationData = QTJAMBI_GET_OBJECTUSERDATA(QThreadInitializationUserData, thread);
    }
    if(!threadInitializationData){
        threadInitializationData = new QThreadInitializationUserData();
        QWriteLocker locker(QtJambiLinkUserData::lock());
        QTJAMBI_SET_OBJECTUSERDATA(QThreadInitializationUserData, thread, threadInitializationData);
    }
    threadInitializationData->setUncaughtExceptionHandler(__jni_env, handler);
}

jobject ThreadAPI::getUncaughtExceptionHandler(JNIEnv *__jni_env, QtJambiNativeID thread_nid)
{
    QThread* thread = QtJambiAPI::objectFromNativeId<QThread>(thread_nid);
    QtJambiAPI::checkNullPointer(__jni_env, thread);
    QThreadInitializationUserData* threadInitializationData;
    {
        QReadLocker locker(QtJambiLinkUserData::lock());
        threadInitializationData = QTJAMBI_GET_OBJECTUSERDATA(QThreadInitializationUserData, thread);
    }
    if(threadInitializationData){
        return threadInitializationData->getUncaughtExceptionHandler(__jni_env);
    }
    return nullptr;
}

void ThreadAPI::setContextClassLoader(JNIEnv *__jni_env, QtJambiNativeID thread_nid, jobject cl)
{
    QSharedPointer<QtJambiLink> link = QtJambiLink::fromNativeId(thread_nid);
    QThread* thread = link ? reinterpret_cast<QThread*>(link->pointer()) : nullptr;
    QtJambiAPI::checkNullPointer(__jni_env, thread);
    QThreadInitializationUserData* threadInitializationData;
    {
        QReadLocker locker(QtJambiLinkUserData::lock());
        threadInitializationData = QTJAMBI_GET_OBJECTUSERDATA(QThreadInitializationUserData, thread);
    }
    if(!threadInitializationData){
        threadInitializationData = new QThreadInitializationUserData();
        QWriteLocker locker(QtJambiLinkUserData::lock());
        QTJAMBI_SET_OBJECTUSERDATA(QThreadInitializationUserData, thread, threadInitializationData);
    }
    threadInitializationData->setContextClassLoader(__jni_env, cl);
}

jobject ThreadAPI::getContextClassLoader(JNIEnv *__jni_env, QtJambiNativeID thread_nid)
{
    QThread* thread = QtJambiAPI::objectFromNativeId<QThread>(thread_nid);
    QtJambiAPI::checkNullPointer(__jni_env, thread);
    QThreadInitializationUserData* threadInitializationData;
    {
        QReadLocker locker(QtJambiLinkUserData::lock());
        threadInitializationData = QTJAMBI_GET_OBJECTUSERDATA(QThreadInitializationUserData, thread);
    }
    if(threadInitializationData){
        return threadInitializationData->getContextClassLoader(__jni_env);
    }
    return nullptr;
}

void ThreadAPI::initializeCurrentThread(JNIEnv *env)
{
    if(QThread* currentQThread = ThreadPrivate::currentThread()){
        QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForQObject(currentQThread);
        if(!link){
            jobject javaQThread = Java::QtCore::Internal::QThread::newInstance(env, nullptr);
            link = QtJambiLink::createLinkForNativeQObject(env, javaQThread, currentQThread);
            link->setCppOwnership(env);
        }
        ThreadPrivate::adoptCurrentJavaThread(env, currentQThread, link);
    }else{
        fprintf(stderr, "QThread::currentThread() returning nullptr");
    }
}

void ThreadPrivate::initializeMainThread(JNIEnv *__jni_env)
{
    static bool initialized = false;
    if(!initialized){
        initialized = true;
        QCoreApplicationPrivate::theMainThread.storeRelease(reinterpret_cast<QThread*>(1));
#if QT_VERSION >= QT_VERSION_CHECK(6, 8, 0)
        QCoreApplicationPrivate::theMainThreadId.storeRelaxed(reinterpret_cast<void*>(1));
#endif
        QThread* currentQThread = QThread::currentThread();
        QThreadUserData* threadData;
        {
            QReadLocker locker(QtJambiLinkUserData::lock());
            threadData = QTJAMBI_GET_OBJECTUSERDATA(QThreadUserData, currentQThread);
        }
        if(threadData)
            threadData->reinitializeMain(currentQThread);
        QCoreApplicationPrivate::theMainThread.storeRelease(currentQThread);
#if QT_VERSION >= QT_VERSION_CHECK(6, 8, 0)
        QCoreApplicationPrivate::theMainThreadId.storeRelaxed(QThreadData::get2(currentQThread)->threadId.loadRelaxed());
#else
        QThreadUserData::theMainThreadId.storeRelaxed(QThreadData::get2(currentQThread)->threadId.loadRelaxed());
#endif
        qtjambi_cast<jobject>(__jni_env, currentQThread);
    }
}

struct JNIInvokablePrivate : QSharedData{
    void* data = nullptr;
    ThreadPrivate::JNIInvokable::Invoker invoker;
    ThreadPrivate::JNIInvokable::Deleter deleter;
    JNIInvokablePrivate(void* _data, ThreadPrivate::JNIInvokable::Invoker _invoker, ThreadPrivate::JNIInvokable::Deleter _deleter = nullptr)
        : QSharedData(), data(_data), invoker(_invoker), deleter(_deleter){
        Q_ASSERT(_data);
        Q_ASSERT(_invoker);
    }
    ~JNIInvokablePrivate(){ if(deleter) deleter(data); }
};

namespace ThreadPrivate{

JNIInvokable::JNIInvokable(FunctionPointer functor) noexcept
    : d(!functor ? nullptr : new JNIInvokablePrivate(
                                 reinterpret_cast<void*>(functor),
                                 [](void* data, JNIEnv * env){
                                     FunctionPointer task = reinterpret_cast<FunctionPointer>(data);
                                     (*task)(env);
                                 })){}

JNIInvokable::JNIInvokable() noexcept : d(){}
JNIInvokable::~JNIInvokable() noexcept {}
JNIInvokable::JNIInvokable(const JNIInvokable& other) noexcept : d(other.d) {}
JNIInvokable::JNIInvokable(JNIInvokable&& other) noexcept : d(std::move(other.d)) {}
JNIInvokable::JNIInvokable(void* data, Invoker invoker, Deleter deleter) noexcept
    : d(!data ? nullptr : new JNIInvokablePrivate(data, invoker, deleter)) {}
JNIInvokable& JNIInvokable::operator=(const JNIInvokable& other) noexcept { d = other.d; return *this; }
JNIInvokable& JNIInvokable::operator=(JNIInvokable&& other) noexcept { d = std::move(other.d); return *this; }
bool JNIInvokable::operator==(const JNIInvokable& other) const noexcept { return d == other.d; }

JNIInvokable::operator bool() const noexcept{
    return d;
}

bool JNIInvokable::operator !() const noexcept{
    return !d;
}

void JNIInvokable::operator()(JNIEnv * env) const{
    if(d)
        d->invoker(d->data, env);
}

} // namespace QtJambiUtils
