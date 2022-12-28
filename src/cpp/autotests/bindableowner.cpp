#include "bindableowner.h"
#include <QtConcurrent>
#include <QtJambi/QtJambiAPI>

struct FutureException : public QException {
    void raise() const override {
        FutureException e = *this;
        throw e;
    }
    FutureException* clone() const override {
        return new FutureException(*this);
    }
};
struct FutureError : public QException {
    void raise() const override {
        FutureError e = *this;
        throw e;
    }
    FutureError* clone() const override {
        return new FutureError(*this);
    }
};

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)

BindableOwner::BindableOwner(QObject *parent) : QObject(parent)
{

}

bool BindableOwner::bind(QObject* targetObject, QStringView targetPropertyName, QObject* sourceObject, QStringView sourcePropertyName){
    QUntypedBindable target(targetObject->metaObject()->property(targetObject->metaObject()->indexOfProperty(targetPropertyName.toLatin1())).bindable(targetObject));
    QUntypedBindable source(sourceObject->metaObject()->property(sourceObject->metaObject()->indexOfProperty(sourcePropertyName.toLatin1())).bindable(sourceObject));

    if(target.isValid() && source.isValid()){
        return target.setBinding(source.makeBinding());
    }
    return false;
}

QFuture<QString> FutureHandler::returnInTheFuture(const QStringList& strings, ulong delay){
    return QtConcurrent::run([](QPromise<QString>& promise, ulong delay, QStringList strings){
        QThread::msleep(delay);
        for(const QString& strg : strings)
            promise.addResult(strg);
    }, delay, strings);
}

QFuture<QString> FutureHandler::throwInTheFuture(const QStringList& strings, ulong delay, int exceptionType){
    return QtConcurrent::run([exceptionType](QPromise<QString>& promise, ulong delay, QStringList strings){
        QThread::msleep(delay);
        switch(exceptionType){
        case 1: FutureException().raise();
        case 2: FutureError().raise();
        case 3: throw std::invalid_argument("X");
        case 4: throw std::underflow_error("X");
        case 5: throw std::bad_alloc();
        case 6: throw "Char string exception";
        case 7: throw 12345;
        case 8: throw QString("QString exception");
        default:
            QException().raise();
        }
        for(const QString& strg : strings)
            promise.addResult(strg);
    }, delay, strings);
}

#else

QFuture<QString> FutureHandler::returnInTheFuture(const QStringList& strings, ulong delay){
    QFutureInterface<QString> promise;
    promise.reportStarted();
    QtConcurrent::run([promise](ulong delay, QStringList strings) mutable {
        QThread::msleep(delay);
        for(const QString& strg : strings)
            promise.reportResult(strg);
        promise.reportFinished();
    }, delay, strings);
    return promise.future();
}

QFuture<QString> FutureHandler::throwInTheFuture(const QStringList& strings, ulong delay, int exceptionType){
    QFutureInterface<QString> promise;
    promise.reportStarted();
    QtConcurrent::run([promise, exceptionType](ulong delay, QStringList strings) mutable {
        QThread::msleep(delay);
        try{
            switch(exceptionType){
            case 1: FutureException().raise();
            case 2: FutureError().raise();
            case 3: throw std::invalid_argument("X");
            case 4: throw std::underflow_error("X");
            case 5: throw std::bad_alloc();
            case 6: throw "Char string exception";
            case 7: throw 12345;
            case 8: throw QString("QString exception");
            default:
                QException().raise();
            }
            for(const QString& strg : strings)
                promise.reportResult(strg);
        }catch(const QException& exn){
            promise.reportException(exn);
        }
        promise.reportFinished();
    }, delay, strings);
    return promise.future();
}

#endif //QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)

QFuture<QString> FutureHandler::forward(QFuture<QString> future){
    return future;
}

QFuture<int> FutureHandler::forwardInt(QFuture<int> future){
    return future;
}

QFuture<QObject*> FutureHandler::forwardObject(QFuture<QObject*> future){
    return future;
}

void FutureHandler::checkWatcherVoid(QFutureWatcher<void>* watcher){
    if(watcher){
        watcher->waitForFinished();
    }else{
        if(JniEnvironment env{16}){
            JavaException::raiseNullPointerException(env, nullptr QTJAMBI_STACKTRACEINFO );
        }
    }
}

QString FutureHandler::checkWatcherString(QFutureWatcher<QString>* watcher){
    if(watcher){
        watcher->waitForFinished();
        return watcher->result();
    }else{
        if(JniEnvironment env{16}){
            JavaException::raiseNullPointerException(env, nullptr QTJAMBI_STACKTRACEINFO );
        }
        return nullptr;
    }
}

int FutureHandler::checkWatcherInt(QFutureWatcher<int>* watcher){
    if(watcher){
        watcher->waitForFinished();
        return watcher->result();
    }else{
        if(JniEnvironment env{16}){
            JavaException::raiseNullPointerException(env, nullptr QTJAMBI_STACKTRACEINFO );
        }
        return 0;
    }
}

QObject* FutureHandler::checkWatcherObject(QFutureWatcher<QObject*>* watcher){
    if(watcher){
        watcher->waitForFinished();
        return watcher->result();
    }else{
        if(JniEnvironment env{16}){
            JavaException::raiseNullPointerException(env, nullptr QTJAMBI_STACKTRACEINFO );
        }
        return {};
    }
}

QFutureWatcher<void>* FutureHandler::watcherVoid(){
    QFutureWatcher<void>* result = new QFutureWatcher<void>;
    return result;
}

QFutureWatcher<QString>* FutureHandler::watcherString(){
    QFutureWatcher<QString>* result = new QFutureWatcher<QString>;
    return result;
}

QFutureWatcher<int>* FutureHandler::watcherInt(){
    QFutureWatcher<int>* result = new QFutureWatcher<int>;
    return result;
}

QFutureWatcher<QObject*>* FutureHandler::watcherObject(){
    QFutureWatcher<QObject*>* result = new QFutureWatcher<QObject*>;
    return result;
}

QFuture<void> FutureHandler::finishInTheFuture(ulong delay){
    return QtConcurrent::run([](ulong delay){
        QThread::msleep(delay);
    }, delay);
}

QList<int> FutureHandler::returnInTheFutureInt(QFuture<int> future){
    future.waitForFinished();
    return future.results();
}

QStringList FutureHandler::returnInTheFuture(QFuture<QString> future){
    future.waitForFinished();
    return future.results();
}

QStringList FutureHandler::returnSequentialInTheFuture(QFuture<QString> future){
    QStringList results;
    for(int i = 0; i<5; ++i){
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        future.suspend();
#else
        future.pause();
#endif
        QThread::msleep(500);
        future.resume();
        printf("progress: %i - \"%s\"\n", future.progressValue(), qPrintable(future.progressText()));
    }
    future.waitForFinished();
    return future.results();
}

void FutureHandler::suspendInTheFuture(QFuture<QString> future){
    QThread::msleep(200);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    future.suspend();
#else
    future.pause();
#endif
    future.waitForFinished();
}

void FutureHandler::cancelInTheFuture(QFuture<QString> future){
    QThread::msleep(200);
    future.cancel();
    future.waitForFinished();
}

void FutureHandler::resumeInTheFuture(QFuture<QString> future){
    QThread::msleep(200);
    future.resume();
    future.waitForFinished();
}

QFuture<void> FutureHandler::throwInTheFuture(ulong delay, int exceptionType){
    return QtConcurrent::run([exceptionType](ulong delay){
        QThread::msleep(delay);
        switch(exceptionType){
        case 1: FutureException().raise();
        case 2: FutureError().raise();
        case 3: throw std::invalid_argument("X");
        case 4: throw std::underflow_error("X");
        case 5: throw std::bad_alloc();
        case 6: throw "Char string exception";
        case 7: throw 12345;
        case 8: throw QString("QString exception");
        default:
            QException().raise();
        }
    }, delay);
}

QStringList FutureHandler::throwInTheFuture(QFuture<QString> future){
    future.waitForFinished();
    return future.results();
}

QFutureInterface<QString> FutureHandler::interfaceString() { return QFutureInterface<QString>(); }

QFutureInterface<int> FutureHandler::interfaceInt() { return QFutureInterface<int>(); }

QFutureInterface<QObject*> FutureHandler::interfaceObject() { return QFutureInterface<QObject*>(); }

QFutureInterface<void> FutureHandler::interfaceVoid() { return QFutureInterface<void>(); }

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
void FutureHandler::fillStringPromise(QPromise<QString>& promise){
    promise.addResult("A");
    promise.addResult("B");
    promise.addResult("C");
    promise.addResult("D");
}
void FutureHandler::fillIntPromise(QPromise<int>& promise){
    promise.addResult(345);
    promise.addResult(678);
}

void FutureHandler::fillObjectPromise(QPromise<QObject*>& promise){
    promise.addResult(QCoreApplication::instance());
    promise.addResult(QCoreApplication::instance()->thread());
    promise.addResult(QCoreApplication::instance()->thread()->eventDispatcher());
}

void FutureHandler::fillVariantPromise(QPromise<QVariant>& promise){
    promise.addResult("123");
    promise.addResult("456");
    promise.addResult("789");
}

#endif // QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
