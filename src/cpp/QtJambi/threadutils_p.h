/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#ifndef THREADUTILS_P_H
#define THREADUTILS_P_H

#include "qtjambiapi.h"
#include "coreapi.h"
#include "jobjectwrapper.h"
#include <QtCore/QPointer>
#include <QtCore/QThreadStorage>
#include <QtCore/QThread>
#include <QtCore/private/qthread_p.h>
#include <QtCore/private/qcoreapplication_p.h>
#include <functional>

class QtJambiLink;
class EventDispatcherCheck;
class QThreadObjectData;

typedef QExplicitlySharedDataPointer<EventDispatcherCheck> EventDispatcherCheckPointer;

class EventDispatcherCheck : public QSharedData {
public:
    struct Data{
        jweak m_jthreadObject;
        QPointer<QThread> m_thread;
        Qt::HANDLE m_threadId;
        bool m_isAdoptedQThread;
        int m_associationHashcode;
        QWeakPointer<QtJambiLink> m_wlink;
        QSharedPointer<QMutex> m_mutex;
        QSharedPointer<QList<QtJambiUtils::Runnable>> m_finalActions{new QList<QtJambiUtils::Runnable>};
        ~Data();
    };

    typedef void (*CleanupFunction)(JNIEnv *env, Data*);
    static void adoptThread(JNIEnv *env, jobject jthreadObject, QThread* thread, QThreadObjectData* threadData, jint associationHashcode, QWeakPointer<QtJambiLink>&& wlink, CleanupFunction _cleaner = nullptr);
    static void detach(JNIEnv *env);
    static bool isAlive();
private:
    ~EventDispatcherCheck();
    void finalize(JNIEnv *env);
    static QThreadStorage<EventDispatcherCheckPointer> Instance;
    EventDispatcherCheck(Data& data, CleanupFunction _cleaner);
    Data* m_data;
    CleanupFunction m_cleaner;
    friend EventDispatcherCheckPointer;
};

class QThreadObjectData : public QtJambiObjectData
{
public:
    enum ThreadType{
        DefaultThread,
        ProcessMainThread,
        VirtualMainThread,
        AdoptedThread
    };
    QThreadObjectData();
    ~QThreadObjectData() override;
    void initializeDefault(QThread* thread);
    void initializeAdopted(QThread* thread);
    void reinitializeMain(QThread* thread);
    void deleteAtThreadEnd(QObject* object);
    void doAtThreadEnd(QtJambiUtils::Runnable&& action);
    inline bool isJavaLaunched() const {return m_isJavaLaunched;}
    inline bool purgeOnExit() const {return m_threadType!=ProcessMainThread;}
    QObject* threadDeleter() const;
    inline ThreadType threadType() const { return m_threadType; }
    static QThreadObjectData* ensureThreadUserData(QThread* thread);
    struct Result{
        QThreadObjectData* threadUserData;
        bool initRequired;
    };
    static Result ensureThreadUserDataLocked(QThread* thread);
#if QT_VERSION < QT_VERSION_CHECK(6, 8, 0)
    static QBasicAtomicPointer<void> theMainThreadId;
#endif
private:
    QExplicitlySharedDataPointer<struct QThreadObjectDataPrivate> p;
    ThreadType m_threadType;
    bool m_isJavaLaunched = false;

    friend EventDispatcherCheck;
};

class QThreadInitializationObjectData : public QtJambiObjectData{
public:
    QThreadInitializationObjectData();
    QThreadInitializationObjectData(JNIEnv *env, jobject threadGroup);
    ~QThreadInitializationObjectData() override;
    jobject getThreadGroup() const;
    jobject getUncaughtExceptionHandler(JNIEnv *env) const;
    void setUncaughtExceptionHandler(JNIEnv *env, jobject uncaughtExceptionHandler);
    jobject getContextClassLoader(JNIEnv *env) const;
    inline bool isDaemon() const { return m_isDaemon; }
    inline void setDaemon(bool isDaemon) { m_isDaemon = isDaemon; }
    inline QByteArray getName() const { return m_name; }
    inline void setName(const QByteArray& name) { m_name = name; }
    void setContextClassLoader(JNIEnv *env, jobject contextClassLoader);
    void clear(JNIEnv *env);
private:
    jobject m_threadGroup = nullptr;
    JObjectWrapper m_uncaughtExceptionHandler;
    JObjectWrapper m_contextClassLoader;
    bool m_isDaemon = true;
    QByteArray m_name;
};

struct QThreadAdoptionObjectData : public QtJambiObjectData{
    JObjectWrapper m_java_object;
    QThreadAdoptionObjectData(JNIEnv * env, jobject java_qthread);
    void clear(JNIEnv* env);
    jobject getLocalRef(JNIEnv* env);
    ~QThreadAdoptionObjectData() override;
private:
    Q_DISABLE_COPY_MOVE(QThreadAdoptionObjectData)
};

class AbstractThreadEvent : public QEvent
{
protected:
    AbstractThreadEvent();
    AbstractThreadEvent(const AbstractThreadEvent& clone);
private:
    static bool execute(QEvent* event);
    virtual void execute() = 0;
    int reserved1 = 0;
    int reserved2 = 0;
    friend class Deleter;
};

class LinkInvalidator : public AbstractThreadEvent{
public:
    LinkInvalidator(QSharedPointer<QtJambiLink>&& link);
    LinkInvalidator* clone() const override;
private:
    void execute() override;
    LinkInvalidator(const LinkInvalidator& other);
    QSharedPointer<QtJambiLink> m_link;
};

bool threadRequiresCoreApplication(QThread* thread);

struct JNIInvokablePrivate;

namespace ThreadPrivate{
class JNIInvokable{
    typedef void(*Deleter)(void*);
    typedef void(*Invoker)(void*, JNIEnv *);
public:
    typedef void(*FunctionPointer)(JNIEnv *);

private:
    explicit JNIInvokable(void* data, Invoker invoker, Deleter deleter) noexcept;
public:
    JNIInvokable() noexcept;
    ~JNIInvokable() noexcept;
    JNIInvokable(const JNIInvokable& other) noexcept;
    JNIInvokable(JNIInvokable&& other) noexcept;
    JNIInvokable(FunctionPointer functor) noexcept;
    inline JNIInvokable(std::nullptr_t) noexcept : JNIInvokable(FunctionPointer(nullptr)) {}

    JNIInvokable& operator=(const JNIInvokable& other) noexcept;
    JNIInvokable& operator=(JNIInvokable&& other) noexcept;

    template<typename Functor, typename std::enable_if<!std::is_pointer<Functor>::value, bool>::type = true
             , typename std::enable_if<!std::is_same<typename std::remove_reference<typename std::remove_cv<Functor>::type>::type, JNIInvokable>::value, bool>::type = true
             , typename std::enable_if<!std::is_null_pointer<typename std::remove_reference<typename std::remove_cv<Functor>::type>::type>::value, bool>::type = true
             , typename std::enable_if<!std::is_same<typename std::remove_reference<typename std::remove_cv<Functor>::type>::type, FunctionPointer>::value, bool>::type = true
             , typename std::enable_if<std::is_invocable<Functor, JNIEnv *>::value, bool>::type = true
             >
    JNIInvokable(Functor&& functor) noexcept
        : JNIInvokable(
              new typename std::remove_reference<typename std::remove_cv<Functor>::type>::type(std::move(functor)),
              [](void* data, JNIEnv *env){
                  typename std::remove_reference<typename std::remove_cv<Functor>::type>::type* fct = reinterpret_cast<typename std::remove_reference<typename std::remove_cv<Functor>::type>::type*>(data);
                  (*fct)(env);
              },
              [](void* data){
                  delete reinterpret_cast<typename std::remove_reference<typename std::remove_cv<Functor>::type>::type*>(data);
              }
              ){}
    bool operator==(const JNIInvokable& other) const noexcept;
    void operator()(JNIEnv * env) const;
    operator bool() const noexcept;
    bool operator !() const noexcept;
    template<typename T>
    static JNIInvokable deleter(T* t){
        return JNIInvokable(
            reinterpret_cast<void*>(t),
            [](void* data){
                T* ptr = reinterpret_cast<T*>(data);
                delete ptr;
            }, nullptr);
    }
    template<typename T>
    static JNIInvokable arrayDeleter(T* t){
        return JNIInvokable(
            reinterpret_cast<void*>(t),
            [](void* data){
                T* ptr = reinterpret_cast<T*>(data);
                delete[] ptr;
            }, nullptr);
    }
private:
    friend JNIInvokablePrivate;
    QExplicitlySharedDataPointer<JNIInvokablePrivate> d;
};

QThread* currentThread();
jobject fromQThread(JNIEnv * env, jobject java_qthread, QThread *thread, const QSharedPointer<QtJambiLink>& link);
void initializeMainThread(JNIEnv *__jni_env);
void doInPurgeThread(JNIInvokable &&fun);
}

#endif // THREADUTILS_P_H
