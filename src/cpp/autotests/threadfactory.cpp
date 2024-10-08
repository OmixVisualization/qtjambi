#include "threadfactory.h"
#include <thread>

void ThreadFactory::startNativeWorkerThread(QRunnable* runnable){
    if(runnable){
        std::thread myThread([runnable] {
            try{
                runnable->run();
            }catch(...){}
            if(runnable->autoDelete())
                delete runnable;
        });
        myThread.detach();
    }
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
