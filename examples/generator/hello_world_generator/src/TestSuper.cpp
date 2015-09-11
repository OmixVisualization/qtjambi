
#include "TestSuper.h"
#include <QDebug>        
#include <QGraphicsScene>
#include <QLineF>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QWidget>

TestSuper::TestSuper(QGraphicsScene *scene) : QGraphicsObject() {
#ifdef QTJAMBI_CPP_EXTEN_DEBUG
  qDebug() << "TestSuper::TestSuper(scene=" << scene << ") on " << this;
#endif
  if (scene) {
    scene->addItem(this);
  }
}

QRectF TestSuper::boundingRect() const {
#ifdef QTJAMBI_CPP_EXTEN_DEBUG
  qDebug() << "TestSuper::boundingRect() on " << this;
#endif
  return QRectF(10, 10, 10, 10);
}

void TestSuper::paint( QPainter* painter, const QStyleOptionGraphicsItem*, QWidget* widget) {
#ifdef QTJAMBI_CPP_EXTEN_DEBUG
  qDebug() << "TestSuper::paint(painter=" << painter << ", item=???, widget=" << widget << ") on " << this;
#endif

  QLineF line(0, 0, 10, 10);
  painter->drawLine(line);
  QLineF line2(10, 0, 0, 10);
  painter->drawLine(line2);

}

