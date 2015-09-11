#include "sharedpointertest.h"

SharedPointerTest::SharedPointerTest(QObject *parent) : QObject(parent),
    m_deletedSharedObjectName("")
{

}

SharedPointerTest::~SharedPointerTest()
{

}

QSharedPointer<QObject> SharedPointerTest::createSharedObject1(){
    QSharedPointer<QObject> result(new QObject());
    result->setObjectName("SharedObject1");
    m_deletedSharedObjectName = "";
    QObject::connect(result.data(), SIGNAL(destroyed(QObject *)), this, SLOT(onDestroyed(QObject*)));
    return result;
}

const QSharedPointer<QGraphicsItem> SharedPointerTest::createSharedObject2(){
    QGraphicsTextItem* item = new QGraphicsTextItem();
    QSharedPointer<QGraphicsItem> result(item);
    item->setObjectName("SharedObject2");
    m_deletedSharedObjectName = "";
    QObject::connect(item, SIGNAL(destroyed(QObject *)), this, SLOT(onDestroyed(QObject*)));
    return result;
}

class SharedEvent : public QEvent{
public:
    SharedEvent(QString* deletedSharedObjectName):
        QEvent(QEvent::User),
        m_deletedSharedObjectName(deletedSharedObjectName) {}
    ~SharedEvent(){
        m_deletedSharedObjectName->operator =("SharedEvent");
    }

private:
    QString* m_deletedSharedObjectName;
};

QSharedPointer<QEvent> SharedPointerTest::createSharedObject3(){
    m_deletedSharedObjectName = "";
    return QSharedPointer<QEvent>(new SharedEvent(&m_deletedSharedObjectName));
}

void SharedPointerTest::onDestroyed(QObject* value){
    m_deletedSharedObjectName = value->objectName();
}

const QString& SharedPointerTest::deletedSharedObjectName(){
    return m_deletedSharedObjectName;
}
