import org.qtjambi.qt.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.core.*;

import java.util.*;

public class picture
{

    public static void myProcessing(String blah)
    {
    }

    public static void  main(String args[])
    {
        QWidget myWidget = new QWidget();
            // RECORD
        if (true)
        {
    //! [0]
            QPicture picture = new QPicture();
            QPainter painter = new QPainter();
            painter.begin(picture);           // paint in picture
            painter.drawEllipse(10,20, 80,70); // draw an ellipse
            painter.end();                     // painting done
            picture.save("drawing.pic");       // save picture
    //! [0]
        }

        if (true)
        {
            // REPLAY
    //! [1]
            QPicture picture = new QPicture();
            picture.load("drawing.pic");           // load picture
            QPainter painter = new QPainter();
            painter.begin(myWidget);              // paint in myWidget
            painter.drawPicture(0, 0, picture);    // draw the picture at (0,0)
            painter.end();                         // painting done
    //! [1]
        }

        QPicture myPicture = new QPicture();

        if (true)
        {
            // FORMATS
    //! [2]
            List<QByteArray> list = QPictureIO.inputFormats();
            for (QByteArray string : list)
                myProcessing(new String(string.toByteArray()));
    //! [2]
        }

        if (true)
        {
            // OUTPUT
    //! [3]
            List<QByteArray> list = QPictureIO.outputFormats();
            for (QByteArray string : list)
                myProcessing(new String(string.toByteArray()));
    //! [3]
        }

        if (true)
        {
            // PIC READ
    //! [4]
            QPictureIO iio = new QPictureIO();
            QPixmap  pixmap = new QPixmap();
            iio.setFileName("vegeburger.pic");
            if (iio.read()) {        // OK
                QPicture picture = iio.picture();
                QPainter painter = new QPainter(pixmap);
                painter.drawPicture(0, 0, picture);
            }
    //! [4]
        }

        if (true)
        {
            QPixmap pixmap = new QPixmap();
            // PIC WRITE
    //! [5]
            QPictureIO iio = new QPictureIO();
            QPicture   picture = new QPicture();
            QPainter painter = new QPainter(picture);
            painter.drawPixmap(0, 0, pixmap);
            iio.setPicture(picture);
            iio.setFileName("vegeburger.pic");
            iio.setFormat("PIC");
            if (iio.write())
                return;
    //! [5]
        }
    }

    // SVG READ
    //! [6]
    public static void readSVG(QPictureIO picture)
    {
        // read the picture using the picture.ioDevice()
    }
    //! [6]

    // SVG WRITE
    //! [7]
    public static void writeSVG(QPictureIO picture)
    {
        // write the picture using the picture.ioDevice()
    }
    //! [7]

    // USE SVG
    public static void foo() {

    //! [8]
        // add the SVG picture handler
        // ...
    //! [8]
        //QPictureIO.defineIOHandler("SVG", 0, 0, readSVG, writeSVG);
        // ...

    }
}
