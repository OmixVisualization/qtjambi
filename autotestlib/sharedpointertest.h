#ifndef SHAREDPOINTERTEST_H
#define SHAREDPOINTERTEST_H

#include <QtCore>
#include <QtWidgets>
class JObjectWrapper;

class SharedPointerTest : public QObject
{
    Q_OBJECT
public:
    explicit SharedPointerTest(QObject *parent = nullptr);
    ~SharedPointerTest();

    QSharedPointer<QObject> createSharedObject1();
    const QSharedPointer<QGraphicsItem> createSharedObject2();
    QSharedPointer<QEvent> createSharedObject3();
    const QSharedPointer<QLayoutItem> createSharedObject4();
    const QSharedPointer<QLayoutItem> createSharedObject5();
    const QSharedPointer<QLayoutItem> createSharedObject6();
    QSharedPointer<QTemporaryFile> createSharedObject7();
    QSharedPointer<QList<QString> > createSharedObject8();
    static const QSharedPointer<QObject>& asSharedPointer(const QSharedPointer<QObject>& object);
    const QString& deletedSharedObjectName();
    void resetSharedObjectName();
signals:

public slots:
    void onDestroyed(QObject* object);
private:
    QString m_deletedSharedObjectName;
};

#endif // SHAREDPOINTERTEST_H
