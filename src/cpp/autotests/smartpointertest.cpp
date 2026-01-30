/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
**
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "smartpointertest.h"
#include <QtJambi/QtJambiAPI>
#include <QtJambi/jobjectwrapper.h>

QT_WARNING_DISABLE_DEPRECATED

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

SmartPointerTest::SmartPointerTest(QObject *parent) : QObject(parent),
    m_deletedSharedObjectName("")
{

}

SmartPointerTest::~SmartPointerTest()
{

}

QSharedPointer<QObject> SmartPointerTest::asSharedPointer(QSharedPointer<QObject> object){
    return object;
}

QSharedPointer<QObject> SmartPointerTest::createSharedObject1(){
    QSharedPointer<QObject> result(new QObject());
    result->setObjectName("SharedObject1");
    m_deletedSharedObjectName = "";
    QObject::connect(result.data(), SIGNAL(destroyed(QObject*)), this, SLOT(onDestroyed(QObject*)));
    return result;
}

const QSharedPointer<QGraphicsItem> SmartPointerTest::createSharedObject2(){
    QGraphicsTextItem* item = new QGraphicsTextItem();
    QSharedPointer<QGraphicsItem> result(item);
    item->setObjectName("SharedObject2");
    m_deletedSharedObjectName = "";
    QObject::connect(item, SIGNAL(destroyed(QObject*)), this, SLOT(onDestroyed(QObject*)));
    return result;
}

QSharedPointer<QEvent> SmartPointerTest::createSharedObject3(){
    m_deletedSharedObjectName = "";
    return QSharedPointer<QEvent>(new SharedEvent(&m_deletedSharedObjectName));
}

const QSharedPointer<QLayoutItem> SmartPointerTest::createSharedObject4(){
    QLayout* item = new QGridLayout();
    QSharedPointer<QLayoutItem> result(item);
    item->setObjectName("SharedObject4");
    m_deletedSharedObjectName = "";
    QObject::connect(item, SIGNAL(destroyed(QObject*)), this, SLOT(onDestroyed(QObject*)));
    return result;
}

const QSharedPointer<QLayoutItem> SmartPointerTest::createSharedObject5(){
    QSharedPointer<QLayoutItem> result(new SpacerItem(&m_deletedSharedObjectName));
    return result;
}

const QSharedPointer<QLayoutItem> SmartPointerTest::createSharedObject6(){
    QWidget* widget = new QWidget();
    widget->setObjectName("Widget");
    QObject::connect(widget, SIGNAL(destroyed(QObject*)), this, SLOT(onDestroyed(QObject*)));
    QWidgetItem* item = new WidgetItem(widget, &m_deletedSharedObjectName);
    QSharedPointer<QLayoutItem> result(item);
    return result;
}

QSharedPointer<QTemporaryFile> SmartPointerTest::createSharedObject7(){
    QSharedPointer<QTemporaryFile> result(new QTemporaryFile("test"));
    result->setObjectName("SharedObject7");
    m_deletedSharedObjectName = "";
    QObject::connect(result.data(), SIGNAL(destroyed(QObject*)), this, SLOT(onDestroyed(QObject*)));
    return result;
}

QSharedPointer<QList<QString> > SmartPointerTest::createSharedObject8(){
    QSharedPointer<QList<QString> > result(new QList<QString>());
    *result << "string content";
    m_deletedSharedObjectName = "";
    return result;
}

std::shared_ptr<QObject> SmartPointerTest::asStdSharedPointer(std::shared_ptr<QObject> object){
    return object;
}

std::shared_ptr<QObject> SmartPointerTest::createStdSharedObject1(){
    std::shared_ptr<QObject> result(new QObject());
    result->setObjectName("SharedObject1");
    m_deletedSharedObjectName = "";
    QObject::connect(result.get(), SIGNAL(destroyed(QObject*)), this, SLOT(onDestroyed(QObject*)));
    return result;
}

const std::shared_ptr<QGraphicsItem> SmartPointerTest::createStdSharedObject2(){
    QGraphicsTextItem* item = new QGraphicsTextItem();
    std::shared_ptr<QGraphicsItem> result(item);
    item->setObjectName("SharedObject2");
    m_deletedSharedObjectName = "";
    QObject::connect(item, SIGNAL(destroyed(QObject*)), this, SLOT(onDestroyed(QObject*)));
    return result;
}

std::shared_ptr<QEvent> SmartPointerTest::createStdSharedObject3(){
    m_deletedSharedObjectName = "";
    return std::shared_ptr<QEvent>(new SharedEvent(&m_deletedSharedObjectName));
}

const std::shared_ptr<QLayoutItem> SmartPointerTest::createStdSharedObject4(){
    QLayout* item = new QGridLayout();
    std::shared_ptr<QLayoutItem> result(item);
    item->setObjectName("SharedObject4");
    m_deletedSharedObjectName = "";
    QObject::connect(item, SIGNAL(destroyed(QObject*)), this, SLOT(onDestroyed(QObject*)));
    return result;
}

const std::shared_ptr<QLayoutItem> SmartPointerTest::createStdSharedObject5(){
    std::shared_ptr<QLayoutItem> result(new SpacerItem(&m_deletedSharedObjectName));
    return result;
}

const std::shared_ptr<QLayoutItem> SmartPointerTest::createStdSharedObject6(){
    QWidget* widget = new QWidget();
    widget->setObjectName("Widget");
    QObject::connect(widget, SIGNAL(destroyed(QObject*)), this, SLOT(onDestroyed(QObject*)));
    QWidgetItem* item = new WidgetItem(widget, &m_deletedSharedObjectName);
    std::shared_ptr<QLayoutItem> result(item);
    return result;
}

std::shared_ptr<QTemporaryFile> SmartPointerTest::createStdSharedObject7(){
    std::shared_ptr<QTemporaryFile> result(new QTemporaryFile("test"));
    result->setObjectName("SharedObject7");
    m_deletedSharedObjectName = "";
    QObject::connect(result.get(), SIGNAL(destroyed(QObject*)), this, SLOT(onDestroyed(QObject*)));
    return result;
}

std::shared_ptr<QList<QString> > SmartPointerTest::createStdSharedObject8(){
    std::shared_ptr<QList<QString> > result(new QList<QString>());
    *result << "string content";
    m_deletedSharedObjectName = "";
    return result;
}

std::unique_ptr<QObject> SmartPointerTest::createUniqueObject(){
    QObject* uobj = new QStandardItemModel();
    uobj->setObjectName("UniqueModel");
    m_deletedSharedObjectName = "";
    QObject::connect(uobj, SIGNAL(destroyed(QObject*)), this, SLOT(onDestroyed(QObject*)));
    return std::unique_ptr<QObject>(uobj);
}

void SmartPointerTest::useUniqueObject(){
    std::unique_ptr<QObject> uobj = createUniqueObject();
    analyze(uobj.get());
    uobj.reset();
}

std::unique_ptr<QEvent> SmartPointerTest::createUniqueEvent(){
    QEvent* uobj = new QDynamicPropertyChangeEvent("UniqueEvent");
    return std::unique_ptr<QEvent>(uobj);
}

void SmartPointerTest::useUniqueEvent(){
    std::unique_ptr<QEvent> uobj = createUniqueEvent();
    analyze(uobj.get());
    uobj.reset();
}

void SmartPointerTest::analyze(QObject*){}
void SmartPointerTest::analyze(QEvent*){}

std::unique_ptr<QObject> SmartPointerTest::asUniquePointer(std::unique_ptr<QObject> object){
    return object;
}

void SmartPointerTest::onDestroyed(QObject* value){
    m_deletedSharedObjectName = value->objectName();
}

const QString& SmartPointerTest::deletedSharedObjectName(){
    return m_deletedSharedObjectName;
}

void SmartPointerTest::resetSharedObjectName(){
    m_deletedSharedObjectName = "";
}

const char* SmartPointerTest::virtualString(){
    return nullptr;
}

QByteArray SmartPointerTest::returnVirtualString(){
    return virtualString();
}

QByteArray SmartPointerTest::returnString(StringSupplier supplier){
    return supplier ? QByteArray{supplier()} : QByteArray{};
}

QScopedPointer<QObject> SmartPointerTest::createScopedObject(){
    QObject* uobj = new QStandardItemModel();
    uobj->setObjectName("ScopedModel");
    m_deletedSharedObjectName = "";
    QObject::connect(uobj, SIGNAL(destroyed(QObject*)), this, SLOT(onDestroyed(QObject*)));
    return QScopedPointer<QObject>(uobj);
}

void SmartPointerTest::useScopedObject(){
    QScopedPointer<QObject> uobj = createScopedObject();
    analyze(uobj.get());
    uobj.reset();
}

QScopedPointer<QEvent> SmartPointerTest::createScopedEvent(){
    QEvent* uobj = new QDynamicPropertyChangeEvent("ScopedEvent");
    return QScopedPointer<QEvent>(uobj);
}

void SmartPointerTest::useScopedEvent(){
    QScopedPointer<QEvent> uobj = createScopedEvent();
    analyze(uobj.get());
    uobj.reset();
}

QScopedPointer<QObject> SmartPointerTest::asScopedPointer(QScopedPointer<QObject> object){
    return QScopedPointer<QObject>(object.take());
}
