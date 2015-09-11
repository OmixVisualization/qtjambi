import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;

import java.util.*;

public class image
{

    public static void main(String args[])
    {
        int x, y;

        /*if(true)
        {
            // BIT ACCESS
            QImage image = new QImage();
            // sets bit at (x, y) to 1
            if (image.format() == QImage.Format_MonoLSB)
                image.scanLine(y)[x >> 3] |= 1 << (x  7);
            else
                image.scanLine(y)[x >> 3] |= 1 << (7 - (x  7));
        }

        {
            // 8-BIT ACCESS
            QImage image;
            // set entry 19 in the color table to yellow
            image.setColor(19, qRgb(255, 255, 0));

            // set 8 bit pixel at (x,y) to value yellow (in color table)
            image.scanLine(y)[x] = 19;
        }

        {
            // 32-BIT
            QImage image;
            // sets 32 bit pixel at (x,y) to yellow.
            uint ptr = reinterpret_cast<uint >(image.scanLine(y)) + x;
            ptr = qRgb(255, 255, 0);
        }*/

        if (true)
        {
            // SAVE
    //! [0]
            QImage image = new QImage();
            QByteArray ba = new QByteArray();
            QBuffer buffer = new QBuffer(ba);
            buffer.open(QIODevice.OpenModeFlag.WriteOnly);
            image.save(buffer, "PNG"); // writes image into ba in PNG format
    //! [0]
        }

        QByteArray bytes = new QByteArray();
        if (true)
        {
            // PIX SAVE
    //! [1]
            QPixmap pixmap = new QPixmap();
            QByteArray bytesi = new QByteArray();
            QBuffer buffer = new QBuffer(bytes);
            buffer.open(QIODevice.OpenModeFlag.WriteOnly);
            pixmap.save(buffer, "PNG"); // writes pixmap into bytes in PNG format
    //! [1]
        }

        if (true)
        {
            // MASK
    //! [2]
            QPixmap alpha = new QPixmap("image-with-alpha.png");
            QPixmap alphacopy = alpha;
            alphacopy.setMask(alphacopy.mask());
    //! [2]
        }
    }

}
