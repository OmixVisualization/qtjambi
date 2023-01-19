/****************************************************************************
**
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

class EventDispatcherCheck{
public:
    typedef void (*CheckerFunction)(QPointer<QThread>&,JObjectWrapper&&,QWeakPointer<QtJambiLink>&&,QList<std::function<void()>>&&);
    EventDispatcherCheck(JObjectWrapper&& jthreadObjectWrapper, QThread* thread, QWeakPointer<QtJambiLink>&& wlink, CheckerFunction&& _cleaner = nullptr);
    ~EventDispatcherCheck();
    
    static QThreadStorage<QSharedPointer<EventDispatcherCheck>> storage;
private:
    JObjectWrapper m_jthreadObjectWrapper;
    QPointer<QThread> m_thread;
    QWeakPointer<QtJambiLink> m_wlink;
    QList<std::function<void()>> m_finalActions;
    QMutex m_mutex;
    CheckerFunction cleaner;
};

class QThreadUserData : public QtJambiObjectData
{
public:
    QThreadUserData(QThread* thread);
    QThreadUserData(JNIEnv *env, jobject threadGroup, QThread* thread);
    ~QThreadUserData() override;
    QTJAMBI_OBJECTUSERDATA_ID_DECL
    void deleteAtThreadEnd(QObject* object);
    void doAtThreadEnd(const std::function<void()>& action);
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
private:
    void cleanup();
    QList<QPointer<QObject>> m_objectsForDeletion;
    QList<std::function<void()>>* m_finalActions;
    QMutex* m_mutex;
    JObjectWrapper m_threadGroup;
    bool m_isDaemon;
    QByteArray m_name;
    JObjectWrapper m_uncaughtExceptionHandler;
    JObjectWrapper m_contextClassLoader;
    QMetaObject::Connection m_finishedConnection;

    enum ThreadType{
        ProcessMainThread,
        VirtualMainThread,
        NoMainThread
    };

    ThreadType m_threadType;

    friend EventDispatcherCheck;
};

class SelfDeletingThread : public QThread
{
    Q_DECLARE_PRIVATE(QThread)
public:
    bool deleteLaterIfIsInFinish();
};

#endif // THREADUTILS_P_H
