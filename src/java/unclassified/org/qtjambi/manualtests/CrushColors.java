package org.qtjambi.manualtests;

import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.core.*;

public class CrushColors {

    static public class Crusher extends Thread {
        public void run() {
            QImage image = new QImage(100, 100, QImage.Format.Format_ARGB32_Premultiplied);

            int counter = 0;

            while (1 + 1 == 2) {
                QPainter painter = new QPainter(image);
                QColor color = new QColor(237, 233, 227);

                painter.setPen(color.darker());
                painter.drawLine(0, 0, image.width() - 1, 0);
                painter.drawLine(0, 0, 0, image.height() - 1);
                painter.end();

                if (++counter == 10000) {
                    counter = 0;
                    System.out.println("lots of passes on thread: " + getId());
                }
            }
        }
    }


    public static void main(String args[]) {
        QApplication.initialize(args);

        for (int i=0; i<4; ++i) {
            Thread t = new Crusher();
            t.setDaemon(false);
            t.start();
        }
    }

}

