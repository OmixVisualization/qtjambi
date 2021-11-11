#ifndef BINDABLEOWNER_H
#define BINDABLEOWNER_H

#ifndef QT_JAMBI_RUN
#include <qtjambi/qtjambi_global.h>
#endif
#include <QtCore/QMetaProperty>
#include <QtCore/QObject>
#include <QtCore/QFuture>
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <QtCore/QProperty>
#include <QtCore/QPromise>

#ifdef QT_JAMBI_RUN
#undef Q_OBJECT_BINDABLE_PROPERTY
#define Q_OBJECT_BINDABLE_PROPERTY(...)
#else
#include <qtjambi_core/qtjambi_stringutil.h>
#endif

class BindableOwner : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int x READ x WRITE setX NOTIFY xChanged BINDABLE bindableX)
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged BINDABLE bindableText)
public:
    explicit BindableOwner(QObject *parent = nullptr);

    int x() const { return xProp; }
    void setX(int x) { xProp = x; }
    QBindable<int> bindableX() { return QBindable<int>(&xProp); }

    const QString& text() const { return textProp; }
    void setText(const QString& text) { textProp = text; }
    QBindable<QString> bindableText() { return QBindable<QString>(&textProp); }

    static bool bind(QObject* targetObject, QStringView targetPropertyName, QObject* sourceObject, QStringView sourcePropertyName);
signals:
    void xChanged();
    void textChanged();

private:
    Q_OBJECT_BINDABLE_PROPERTY(BindableOwner, int, xProp, &BindableOwner::xChanged)
    Q_OBJECT_BINDABLE_PROPERTY(BindableOwner, QString, textProp, &BindableOwner::textChanged)
};

#endif //QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)

class FutureHandler{
private:
    FutureHandler() = delete;
public:
    static QFuture<QString> forward(QFuture<QString> future);
    static QFuture<int> forwardInt(QFuture<int> future);
    static QFuture<QObject*> forwardObject(QFuture<QObject*> future);
    static QFutureWatcher<QString>* watcherString();
    static QFutureWatcher<int>* watcherInt();
    static QFutureWatcher<QObject*>* watcherObject();
    static QFutureWatcher<void>* watcherVoid();
    static QFutureInterface<QString> interfaceString();
    static QFutureInterface<int> interfaceInt();
    static QFutureInterface<QObject*> interfaceObject();
    static QFutureInterface<void> interfaceVoid();
    static QString checkWatcherString(QFutureWatcher<QString>* watcher);
    static void checkWatcherVoid(QFutureWatcher<void>* watcher);
    static int checkWatcherInt(QFutureWatcher<int>* watcher);
    static QObject* checkWatcherObject(QFutureWatcher<QObject*>* watcher);
    static QList<int> returnInTheFutureInt(QFuture<int> future);
    static QFuture<QString> returnInTheFuture(const QStringList& strings, ulong delay);
    static QStringList returnInTheFuture(QFuture<QString> future);
    static QStringList returnSequentialInTheFuture(QFuture<QString> future);
    static void suspendInTheFuture(QFuture<QString> future);
    static void resumeInTheFuture(QFuture<QString> future);
    static void cancelInTheFuture(QFuture<QString> future);
    static QFuture<void> finishInTheFuture(ulong delay);
    static QFuture<void> throwInTheFuture(ulong delay, int exceptionType = 0);
    static QFuture<QString> throwInTheFuture(const QStringList& strings, ulong delay, int exceptionType = 0);
    static QStringList throwInTheFuture(QFuture<QString> future);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    static void fillStringPromise(QPromise<QString>& promise);
    static void fillIntPromise(QPromise<int>& promise);
    static void fillObjectPromise(QPromise<QObject*>& promise);
    static void fillVariantPromise(QPromise<QVariant>& promise);
#endif //QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
};

#endif // BINDABLEOWNER_H
