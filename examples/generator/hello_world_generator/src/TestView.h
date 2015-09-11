#ifndef __TESTVIEW_H__
#define __TESTVIEW_H__

#include <QtCore/QDebug>
#include <QtGui/QGraphicsView>
#include <QtGui/QGraphicsScene>
#include <QtGui/QWidget>
#include "TestItem.h"

class TestView : public QGraphicsView {

    private:
        TestItem* item;

    public:
        TestView(QGraphicsScene *scene, QWidget *parent = 0);
        ~TestView() {
#ifdef QTJAMBI_CPP_EXTEN_DEBUG
            qDebug() << "~TestView() on " << this;
#endif
        }
        TestItem* getItem();

};

#endif
