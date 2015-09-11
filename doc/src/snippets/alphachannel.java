#include <qapplication.h>
#include <qlabel.h>
#include <qpixmap.h>
#include <qprinter.h>
#include <qpainter.h>
#include <qfile.h>
#include <qdir.h>
#include <qfileinfo.h>

#if (QT_VERSION) >= 0x040000
#include <QtGui>
#include <QtCore>
#endif

class MyClass : public QWidget
{
public:
     MyClass(QWidget *parent = 0) : QWidget(parent) { }

protected:
     void paintEvent(QPaintEvent *e)
     {
        /*QRadialGradient rg(50, 50, 50, 50, 50);
         rg.setColorAt(0, QColor::fromRgbF(1, 0, 0, 1));
         rg.setColorAt(1, QColor::fromRgbF(0, 0, 0, 0));
         QPainter pmp(&pm);
         pmp.fillRect(0, 0, 100, 100, rg);
         pmp.end();*/

    createImage();

    QPainter p(this);
    p.fillRect(rect(), Qt::white);

    p.drawPixmap(0, 0, pixmap);

    p.drawPixmap(100, 0, channelImage);
     }

    void createImage()
    {
//! [0]
    pixmap = QPixmap(100, 100);
    pixmap.fill(Qt::transparent);

    QRadialGradient gradient(50, 50, 50, 50, 50);
    gradient.setColorAt(0, QColor::fromRgbF(1, 0, 0, 1));
    gradient.setColorAt(1, QColor::fromRgbF(0, 0, 0, 0));
    QPainter painter(&pixmap);
    painter.fillRect(0, 0, 100, 100, gradient);

    channelImage = pixmap.alphaChannel();
    update();
//! [0]
    }

    QPixmap channelImage, pixmap;
    QSize sizeHint() const { return QSize(500, 500); }
};



int main(int argc, char **argv)
{
     QApplication app(argc, argv);

     MyClass cl;
     cl.show();
     QObject::connect(&app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()));

     int ret = app.exec();
     return ret;
}
