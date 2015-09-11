#ifndef __TESTITEM_H__
#define __TESTITEM_H__

#include <QtCore/QDebug>
#include <QtCore/QObject>
#include <QtGui/QGraphicsScene>
#include "TestSuper.h"

class TestItem : public TestSuper {

    Q_OBJECT

    public:
        TestItem(QGraphicsScene *scene);
#ifdef QTJAMBI_CPP_EXTEN_DEBUG
        ~TestItem() {
            qDebug() << "~TestItem() on " << this;
        }
#endif
        int getId();

};
#endif
