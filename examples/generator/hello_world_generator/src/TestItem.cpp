
#include "TestItem.h"
#include <QDebug>
#include <QGraphicsScene>
#include <QLineF>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QWidget>

TestItem::TestItem(QGraphicsScene *scene) : TestSuper(scene) {
#ifdef QTJAMBI_CPP_EXTEN_DEBUG
  qDebug() << "TestItem::TestItem(scene=" << scene << ") on " << this;
#endif
}

int TestItem::getId() {
#ifdef QTJAMBI_CPP_EXTEN_DEBUG
  qDebug() << "TestItem::getId() on " << this;
#endif
  return -1;

}


