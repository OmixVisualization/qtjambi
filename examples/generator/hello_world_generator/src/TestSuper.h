#ifndef __TESTSUPER_H__
#define __TESTSUPER_H__

#include <QtCore/QDebug>
#include <QtCore/QRectF>
#include <QtGui/QWidget>
#include <QtGui/QPainter>
#include <QtGui/QGraphicsObject>
#include <QtGui/QGraphicsScene>
#include <QtGui/QStyleOptionGraphicsItem>

class TestSuper : public QGraphicsObject {

    Q_OBJECT

    public:
        TestSuper(QGraphicsScene *scene);
#ifdef QTJAMBI_CPP_EXTEN_DEBUG
        ~TestSuper() {
            qDebug() << "~TestSuper() on " << this;
        }
#endif

    protected:
        virtual QRectF boundingRect() const;
        virtual void paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* );

};
#endif
