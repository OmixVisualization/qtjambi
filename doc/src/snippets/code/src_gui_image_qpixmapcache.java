import org.qtjambi.qt.gui.*;


public class src_gui_image_qpixmapcache {
    public static void main(String args[]) {
        QApplication.initialize(args);

        QPixmap pixmap = null;
        QPainter painter = new QPainter();
//! [0]
        if (!QPixmapCache.find("my_big_image", pixmap)) {
            pixmap = new QPixmap("bigimage.png");
            QPixmapCache.insert("my_big_image", pixmap);
        }

        painter.drawPixmap(0, 0, pixmap);
//! [0]

        {
//! [1]
        if (!QPixmapCache.find("my_big_image", pixmap)) {
            pixmap = new QPixmap("bigimage.png");
            QPixmapCache.insert("my_big_image", pixmap);
        }
        painter.drawPixmap(0, 0, pixmap);
//! [1]
        }


    }
}
