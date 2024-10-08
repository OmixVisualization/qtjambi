#ifndef THREADFACTORY_H
#define THREADFACTORY_H

#include <QThread>
#include <QRunnable>

class ThreadFactory
{
public:
    ThreadFactory() = delete;
    ~ThreadFactory() = delete;
    static void startNativeWorkerThread(QRunnable* runnable);
    static QThread* startQWorkerThread(QRunnable* runnable);
    static QThread* startQLoopThread();
    static QThread* createQWorkerThread(QRunnable* runnable);
    static QThread* createQLoopThread();
};

#endif // THREADFACTORY_H
