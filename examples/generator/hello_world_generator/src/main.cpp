
#include <QtGui/QApplication>
#include "TestItem.h"
#include "TestView.h"
#include <QGraphicsView>
#include <QGraphicsScene>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QGraphicsScene *scene = new QGraphicsScene();
    TestView *view = new TestView(scene, 0);
    TestItem *item = view->getItem();

    qDebug("ID: %d", item->getId());

    view->show();

    a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
    return a.exec();

}

