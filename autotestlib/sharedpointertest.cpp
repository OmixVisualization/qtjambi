#include "sharedpointertest.h"
#include <qtjambi/qtjambi_core.h>
#include <qtjambi/qtjambi_jobjectwrapper.h>

SharedPointerTest::SharedPointerTest(QObject *parent) : QObject(parent),
    m_deletedSharedObjectName("")
{

}

SharedPointerTest::~SharedPointerTest()
{

}

const QSharedPointer<QObject>& SharedPointerTest::asSharedPointer(const QSharedPointer<QObject>& object){
    return object;
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

const QSharedPointer<QLayoutItem> SharedPointerTest::createSharedObject4(){
    QLayout* item = new QGridLayout();
    QSharedPointer<QLayoutItem> result(item);
    item->setObjectName("SharedObject4");
    m_deletedSharedObjectName = "";
    QObject::connect(item, SIGNAL(destroyed(QObject *)), this, SLOT(onDestroyed(QObject*)));
    return result;
}

class SpacerItem : public QSpacerItem{
public:
    SpacerItem(QString* deletedSharedObjectName):
        QSpacerItem(5,20),
        m_deletedSharedObjectName(deletedSharedObjectName) {}
    virtual ~SpacerItem(){
        m_deletedSharedObjectName->operator =("SpacerItem");
    }

private:
    QString* m_deletedSharedObjectName;
};

const QSharedPointer<QLayoutItem> SharedPointerTest::createSharedObject5(){
    QSharedPointer<QLayoutItem> result(new SpacerItem(&m_deletedSharedObjectName));
    return result;
}

class WidgetItem : public QWidgetItem{
public:
    WidgetItem(QWidget* widget, QString* deletedSharedObjectName):
        QWidgetItem(widget),
        m_deletedSharedObjectName(deletedSharedObjectName) {}
    virtual ~WidgetItem(){
        m_deletedSharedObjectName->operator =("WidgetItem");
    }

private:
    QString* m_deletedSharedObjectName;
};

const QSharedPointer<QLayoutItem> SharedPointerTest::createSharedObject6(){
    QWidget* widget = new QWidget();
    widget->setObjectName("Widget");
    QObject::connect(widget, SIGNAL(destroyed(QObject *)), this, SLOT(onDestroyed(QObject*)));
    QWidgetItem* item = new WidgetItem(widget, &m_deletedSharedObjectName);
    QSharedPointer<QLayoutItem> result(item);
    return result;
}

QSharedPointer<QTemporaryFile> SharedPointerTest::createSharedObject7(){
    QSharedPointer<QTemporaryFile> result(new QTemporaryFile("test"));
    result->setObjectName("SharedObject7");
    m_deletedSharedObjectName = "";
    QObject::connect(result.data(), SIGNAL(destroyed(QObject *)), this, SLOT(onDestroyed(QObject*)));
    return result;
}

QSharedPointer<QList<QString> > SharedPointerTest::createSharedObject8(){
    QSharedPointer<QList<QString> > result(new QList<QString>());
    *result << "string content";
    m_deletedSharedObjectName = "";
    return result;
}

class SharedEvent : public QEvent{
public:
    SharedEvent(QString* deletedSharedObjectName):
        QEvent(QEvent::User),
        m_deletedSharedObjectName(deletedSharedObjectName) {}
    virtual ~SharedEvent(){
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

void SharedPointerTest::resetSharedObjectName(){
    m_deletedSharedObjectName = "";
}
