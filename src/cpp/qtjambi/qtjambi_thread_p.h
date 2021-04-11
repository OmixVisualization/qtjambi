/****************************************************************************
**
** Copyright (C) 2009-2021 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
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

#ifndef QTJAMBI_THREAD_P_H
#define QTJAMBI_THREAD_P_H

#include "qtjambi_core.h"
#include "qtjambi_application.h"
#include "qtjambi_thread.h"
#include "qtjambi_jobjectwrapper.h"
#include <QtCore/QPointer>
#include <QtCore/QThreadStorage>
#include <QtCore/QThread>
#include <QtCore/private/qthread_p.h>
#include <QtCore/private/qcoreapplication_p.h>
#include <functional>
#ifdef Q_OS_WIN
#include <Windows.h>
#endif

#ifndef QT_QTJAMBI_PORT
class ObjectDeleter{
public:
    ObjectDeleter(QObject* object);
    ~ObjectDeleter();
private:
    QPointer<QObject> m_object;
};

class EventDispatcherCheck{
public:
    EventDispatcherCheck(QThread* thread, std::function<void(QPointer<QThread>&,QList<std::function<void()>>&&)> _cleaner = std::function<void(QPointer<QThread>&,QList<std::function<void()>>&&)>());
    ~EventDispatcherCheck();
    
    static QThreadStorage<QSharedPointer<EventDispatcherCheck>> storage;
private:
    QPointer<QThread> m_thread;
    QList<std::function<void()>> m_finalActions;
    QMutex m_mutex;
    std::function<void(QPointer<QThread>&,QList<std::function<void()>>&&)> cleaner;
};
#endif

#ifdef QT_QTJAMBI_PORT
class QThreadInterrupterUserData : public QtJambiObjectData
{
public:
    QThreadInterrupterUserData(const QSharedPointer<QtJambiLink>& link);
    ~QThreadInterrupterUserData() override;
    QTJAMBI_OBJECTUSERDATA_ID_DECL
private:
    QWeakPointer<QtJambiLink> m_link;
};

void __qt_QThread_connect_signals(const QSharedPointer<QtJambiLink>& link, QMap<QString,QMetaObject::Connection>* connections);
#endif

class QThreadUserData : public QtJambiObjectData
{
public:
    QThreadUserData();
#ifndef QT_QTJAMBI_PORT
    QThreadUserData(JNIEnv *env, jobject threadGroup);
#endif
    ~QThreadUserData() override;
    QTJAMBI_OBJECTUSERDATA_ID_DECL
    void deleteAtThreadEnd(QObject* object);
    void doAtThreadEnd(const std::function<void()>& action);
#ifndef QT_QTJAMBI_PORT
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
#endif
private:
    QList<std::function<void()>>* m_finalActions;
    QMutex* m_mutex;
#ifndef QT_QTJAMBI_PORT
    JObjectWrapper m_threadGroup;
    bool m_isDaemon;
    QByteArray m_name;
    JObjectWrapper m_uncaughtExceptionHandler;
    JObjectWrapper m_contextClassLoader;
#endif
    friend EventDispatcherCheck;
};

#ifdef QT_QTJAMBI_PORT
const QSharedPointer<QtJambiLink>&
#else
void
#endif
qtjambi_adopt_thread(JNIEnv *env, jobject java_thread, jobject java_qthread, QThread *qt_thread, bool created_by_java);

class SelfDeletingThread : public QThread
{
    Q_DECLARE_PRIVATE(QThread)
public:
    bool deleteLaterIfIsInFinish();
};

#ifdef QT_QTJAMBI_PORT
class AdoptedThread : public QThread
{
    Q_DECLARE_PRIVATE(QThread)
public:
    AdoptedThread(JNIEnv *env, jobject thread, bool running);

    ~AdoptedThread() override;

    void onStart();

    bool event(QEvent *event) override;
    bool isAdopted() const;
private:
    void run() override {}

    const QWeakPointer<QtJambiLink> m_link;
};
#endif

#endif // QTJAMBI_THREAD_P_H
