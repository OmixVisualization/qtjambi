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

#include "threadfactory.h"
#include <QSharedPointer>
#include <QSemaphore>
#include <QEventLoop>
#include <thread>

QThread* ThreadFactory::startNativeWorkerThread(QRunnable* runnable){
    QThread* result{nullptr};
    if(runnable){
        QSharedPointer<QSemaphore> semaphore(new QSemaphore);
        std::thread myThread([runnable, semaphore, threadOut = &result] {
            *threadOut = QThread::currentThread();
            semaphore->release();
            try{
                runnable->run();
            }catch(...){}
            if(runnable->autoDelete())
                delete runnable;
        });
        semaphore->acquire();
        myThread.detach();
    }
    return result;
}

QThread* ThreadFactory::startNativeLoopThread(){
    QThread* result{nullptr};
    QSharedPointer<QSemaphore> semaphore(new QSemaphore);
    std::thread myThread([semaphore, threadOut = &result] {
        struct ThreadAccess : QThread{
            int execThread(){
                return exec();
            }
        };
        QThread* currentThread = QThread::currentThread();
        *threadOut = currentThread;
        semaphore->release();
        static_cast<ThreadAccess*>(currentThread)->execThread();
    });
    semaphore->acquire();
    myThread.detach();
    return result;
}

QThread* ThreadFactory::startQWorkerThread(QRunnable* runnable){
    QThread* result = createQWorkerThread(runnable);
    if(result){
        QObject::connect(result, &QThread::finished, result, &QObject::deleteLater, Qt::DirectConnection);
        result->start();
    }
    return result;
}

QThread* ThreadFactory::startQLoopThread(){
    QThread* result = createQLoopThread();
    QObject::connect(result, &QThread::finished, result, &QObject::deleteLater, Qt::DirectConnection);
    result->start();
    return result;
}

QThread* ThreadFactory::createQWorkerThread(QRunnable* runnable){
    if(runnable){
        class Thread : public QThread{
            QRunnable* runnable;
        public:
            Thread(QRunnable* _runnable) : QThread(), runnable(_runnable){}
        protected:
            void run() override {
                try{
                    runnable->run();
                }catch(...){}
                if(runnable->autoDelete())
                    delete runnable;
            }
        };
        return new Thread(runnable);
        /*return QThread::create([](QRunnable* runnable){
            try{
                runnable->run();
            }catch(...){}
            if(runnable->autoDelete())
                delete runnable;
        }, runnable);*/
    }else{
        return nullptr;
    }
}

QThread* ThreadFactory::createQLoopThread(){
    return new QThread;
}

void SignalReceiver::receiveSignal(){
    setReceived(true);
}

bool SignalReceiver::received() const
{
    QReadLocker locker(&m_lock);
    return m_received;
}

void SignalReceiver::setReceived(bool newReceived)
{
    {
        QWriteLocker locker(&m_lock);
        if (m_received == newReceived)
            return;
        m_received = newReceived;
    }
    emit receivedChanged();
}

