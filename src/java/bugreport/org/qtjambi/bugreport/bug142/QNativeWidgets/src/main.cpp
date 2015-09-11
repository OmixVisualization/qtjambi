
#include <QtGui/QApplication>
#include "qnativewidgettest.h"
#include <QGraphicsView>
#include <QGraphicsScene>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QNativeWidgetTest widget;
    widget.setLabel("Foobar"); 
    widget.show();

    a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
    return a.exec();

}

