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

#ifndef THREADFACTORY_H
#define THREADFACTORY_H

#include <QThread>
#include <QRunnable>
#include <QReadWriteLock>

class ThreadFactory
{
public:
    ThreadFactory() = delete;
    ~ThreadFactory() = delete;
    static QThread* startNativeLoopThread();
    static QThread* startNativeWorkerThread(QRunnable* runnable);
    static QThread* startQWorkerThread(QRunnable* runnable);
    static QThread* startQLoopThread();
    static QThread* createQWorkerThread(QRunnable* runnable);
    static QThread* createQLoopThread();
};

class SignalReceiver : public QObject{
    Q_OBJECT
public:
    using QObject::QObject;
    ~SignalReceiver() override = default;
    bool received() const;
    void setReceived(bool newReceived);

public slots:
    void receiveSignal();
signals:
    void receivedChanged();

private:
    mutable QReadWriteLock m_lock;
    bool m_received = false;
    Q_PROPERTY(bool received READ received WRITE setReceived NOTIFY receivedChanged FINAL)
};

#endif // THREADFACTORY_H
