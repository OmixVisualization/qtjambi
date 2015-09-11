#ifndef SHAREDPOINTERTEST_H
#define SHAREDPOINTERTEST_H

#include <QtCore>
#include <QtWidgets>

class SharedPointerTest : public QObject
{
    Q_OBJECT
public:
    explicit SharedPointerTest(QObject *parent = 0);
    ~SharedPointerTest();

    QSharedPointer<QObject> createSharedObject1();
    const QSharedPointer<QGraphicsItem> createSharedObject2();
    QSharedPointer<QEvent> createSharedObject3();
    const QString& deletedSharedObjectName();
signals:

public slots:
    void onDestroyed(QObject* object);
private:
    QString m_deletedSharedObjectName;
};

#endif // SHAREDPOINTERTEST_H
