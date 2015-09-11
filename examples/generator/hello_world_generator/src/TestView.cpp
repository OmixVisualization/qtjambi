
#include "TestView.h"
#include <QDebug>        
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QWidget>

TestView::TestView( QGraphicsScene *scene, QWidget *parent ) : QGraphicsView(scene, parent) {
#ifdef QTJAMBI_CPP_EXTEN_DEBUG
  qDebug() << "TestView::TestView(scene=" << scene << ", parent=" << parent << ") on " << this;
#endif
  item = new TestItem(scene);
}

TestItem*
TestView::getItem() {
#ifdef QTJAMBI_CPP_EXTEN_DEBUG
  qDebug() << "TestView::getItem() on " << this;
#endif
  return item;
}

