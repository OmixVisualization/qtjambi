/****************************************************************************
**
** Copyright (C) 2009-2024 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

typedef QExplicitlySharedDataPointer<EventDispatcherCheck> EventDispatcherCheckPointer;

class EventDispatcherCheck : public QSharedData {
public:
    struct Data{
        jweak m_jthreadObject;
        QPointer<QThread> m_thread;
        bool m_detach;
        int m_associationHashcode;
        QWeakPointer<QtJambiLink> m_wlink;
        QList<QtJambiUtils::Runnable> m_finalActions;
        ~Data();
    };

    typedef void (*CleanupFunction)(Data*);
    static void adoptThread(JNIEnv *env, jobject jthreadObject, QThread* thread, bool detach, jint associationHashcode, QWeakPointer<QtJambiLink>&& wlink, CleanupFunction _cleaner = nullptr);
    static void detach();
    static bool isAlive();
    ~EventDispatcherCheck();
private:
    static QThreadStorage<EventDispatcherCheckPointer> Instance;
    EventDispatcherCheck(Data& data, CleanupFunction _cleaner);
    Data* m_data;
    QMutex m_mutex;
    CleanupFunction cleaner;
};

class QThreadUserData : public QtJambiObjectData
{
public:
    enum ThreadType{
        DefaultThread,
        ProcessMainThread,
        VirtualMainThread,
        AdoptedThread
    };
    QThreadUserData();
    QThreadUserData(JNIEnv *env, jobject threadGroup);
    ~QThreadUserData() override;
    QTJAMBI_OBJECTUSERDATA_ID_DECL
    void initialize(QThread* thread, bool isAdopted = false);
    void deleteAtThreadEnd(QObject* object);
    void doAtThreadEnd(QtJambiUtils::Runnable&& action);
    inline bool isDaemon() const { return m_isDaemon; }
    inline void setDaemon(bool isDaemon) { m_isDaemon = isDaemon; }
    inline QByteArray getName() const { return m_name; }
    inline void setName(const QByteArray& name) { m_name = name; }
    inline jobject getThreadGroup() const { return m_threadGroup.object(); }
    inline jobject getUncaughtExceptionHandler() const { return m_uncaughtExceptionHandler.object(); }
    inline void setUncaughtExceptionHandler(JNIEnv *env, jobject uncaughtExceptionHandler) { m_uncaughtExceptionHandler = JObjectWrapper(env, uncaughtExceptionHandler); }
    inline jobject getContextClassLoader() const { return m_contextClassLoader.object(); }
    inline void setContextClassLoader(JNIEnv *env, jobject contextClassLoader) { m_contextClassLoader = JObjectWrapper(env, contextClassLoader); }
    inline void clearContextClassLoader(JNIEnv *env) { m_contextClassLoader.clear(env); }
    inline void clearThreadGroup(JNIEnv *env){ m_threadGroup.clear(env); }
    inline void clearUncaughtExceptionHandler(JNIEnv *env){ m_uncaughtExceptionHandler.clear(env); }
    inline bool purgeOnExit() const {return m_threadType!=ProcessMainThread;}
    inline QObject* threadDeleter() const {return m_threadDeleter.get();}
    inline ThreadType threadType() const { return m_threadType; }
    static QThreadUserData* ensureThreadUserData(QThread* thread);
    struct Result{
        QThreadUserData* threadUserData;
        bool initRequired;
    };
    static Result ensureThreadUserDataLocked(QThread* thread);
private:
    void cleanup(bool isInDestructor);
    QSharedPointer<QObject> m_threadDeleter;
    QList<QPointer<QObject>> m_objectsForDeletion;
    QList<QtJambiUtils::Runnable>* m_finalActions;
    QMutex* m_mutex;
    JObjectWrapper m_threadGroup;
    bool m_isDaemon;
    QByteArray m_name;
    JObjectWrapper m_uncaughtExceptionHandler;
    JObjectWrapper m_contextClassLoader;
    QMetaObject::Connection m_finishedConnection;

    ThreadType m_threadType;

    friend EventDispatcherCheck;
};

struct QThreadAdoptionUserData : public QtJambiObjectData{
    JObjectWrapper m_java_object;
    QThreadAdoptionUserData(JNIEnv * env, jobject java_qthread);
    void clear(JNIEnv* env);
    jobject getLocalRef(JNIEnv* env);
    ~QThreadAdoptionUserData() override;
    QTJAMBI_OBJECTUSERDATA_ID_DECL
private:
    Q_DISABLE_COPY_MOVE(QThreadAdoptionUserData)
};

class AbstractThreadEvent : public QEvent
{
#if QT_VERSION < 0x060000
public:
    virtual AbstractThreadEvent* clone() const = 0;
#endif
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

#endif // THREADUTILS_P_H
