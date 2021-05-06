#ifndef SHAREDPOINTERTEST_H
#define SHAREDPOINTERTEST_H

#ifndef QT_JAMBI_RUN
#include <qtjambi/qtjambi_global.h>
#endif
#include <QtCore/QtCore>
#ifndef QTJAMBI_NO_WIDGETS
#include <QtWidgets/QtWidgets>
#endif

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
