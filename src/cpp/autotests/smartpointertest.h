#ifndef SMARTPOINTERTEST_H
#define SMARTPOINTERTEST_H

#ifndef QTJAMBI_GENERATOR_RUNNING
#include <QtJambi/global.h>
#endif
#include <QtCore/QtCore>
#ifndef QTJAMBI_NO_WIDGETS
#include <QtWidgets/QtWidgets>
#endif

class JObjectWrapper;

class SmartPointerTest : public QObject
{
    Q_OBJECT
public:
    explicit SmartPointerTest(QObject *parent = nullptr);
    ~SmartPointerTest();

    QSharedPointer<QObject> createSharedObject1();
    const QSharedPointer<QGraphicsItem> createSharedObject2();
    QSharedPointer<QEvent> createSharedObject3();
    const QSharedPointer<QLayoutItem> createSharedObject4();
    const QSharedPointer<QLayoutItem> createSharedObject5();
    const QSharedPointer<QLayoutItem> createSharedObject6();
    QSharedPointer<QTemporaryFile> createSharedObject7();
    QSharedPointer<QList<QString> > createSharedObject8();
    static QSharedPointer<QObject> asSharedPointer(QSharedPointer<QObject> object);
    std::shared_ptr<QObject> createStdSharedObject1();
    const std::shared_ptr<QGraphicsItem> createStdSharedObject2();
    std::shared_ptr<QEvent> createStdSharedObject3();
    const std::shared_ptr<QLayoutItem> createStdSharedObject4();
    const std::shared_ptr<QLayoutItem> createStdSharedObject5();
    const std::shared_ptr<QLayoutItem> createStdSharedObject6();
    std::shared_ptr<QTemporaryFile> createStdSharedObject7();
    std::shared_ptr<QList<QString> > createStdSharedObject8();
    static std::shared_ptr<QObject> asStdSharedPointer(std::shared_ptr<QObject> object);
    static std::unique_ptr<QObject> asUniquePointer(std::unique_ptr<QObject> object);
    const QString& deletedSharedObjectName();
    void resetSharedObjectName();
    virtual std::unique_ptr<QObject> createUniqueObject();
    void useUniqueObject();
    virtual std::unique_ptr<QEvent> createUniqueEvent();
    virtual void analyze(QObject* obj);
    virtual void analyze(QEvent* evt);
    void useUniqueEvent();
    virtual const char* virtualString();
    QByteArray returnVirtualString();
    typedef std::function<const char*()> StringSupplier;
    QByteArray returnString(StringSupplier supplier);
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    static QScopedPointer<QObject> asScopedPointer(QScopedPointer<QObject> object);
    virtual QScopedPointer<QObject> createScopedObject();
    void useScopedObject();
    virtual QScopedPointer<QEvent> createScopedEvent();
    void useScopedEvent();
#endif
signals:

public slots:
    void onDestroyed(QObject* object);
private:
    QString m_deletedSharedObjectName;
};

#endif // SMARTPOINTERTEST_H
