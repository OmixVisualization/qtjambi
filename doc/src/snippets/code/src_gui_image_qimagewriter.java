import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.core.*;


public class src_gui_image_qimagewriter {
    public static void main(String args[]) {
        QApplication.initialize(args);
        {
//! [0]
        QImageWriter writer = new QImageWriter();
        writer.setFormat(new QByteArray("png")); // same as writer.setFormat("PNG");
//! [0]
        }


        {
//! [1]
        QImage image = new QImage("some/image.jpeg");
        QImageWriter writer = new QImageWriter("images/outimage.png", new QByteArray("png"));
        writer.setText("Author", "John Smith");
        writer.write(image);
//! [1]
        }

        {
            String fileName = null;
//! [2]
        QImageWriter writer = new QImageWriter(fileName);
        if (writer.supportsOption(QImageIOHandler.ImageOption.Description))
            writer.setText("Author", "John Smith");
//! [2]
        }


    }
}
