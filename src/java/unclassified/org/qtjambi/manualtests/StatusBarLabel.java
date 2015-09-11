package org.qtjambi.manualtests;

import org.qtjambi.qt.core.Qt;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.QThread;

/**
 * @author Boerge Nygaard Austvold
 */
public class StatusBarLabel extends QLabel {

    public StatusBarLabel() {
    }

    public StatusBarLabel(QWidget parent) {
        super(parent);
    }

    public StatusBarLabel(String text) {
        super(text);
    }

    public StatusBarLabel(QPrivateConstructor p) {
        super(p);
    }

    public StatusBarLabel(QWidget parent, Qt.WindowFlags f) {
        super(parent, f);
    }

    public StatusBarLabel(String text, QWidget parent) {
        super(text, parent);
    }

    public StatusBarLabel(String text, QWidget parent, Qt.WindowFlags f) {
        super(text, parent, f);
    }

    protected void paintEvent(QPaintEvent arg__1) {
        super.paintEvent(arg__1);

        QPainter painter = new QPainter(this);

        // TODO: Read color from style
        QColor color = new QColor(237, 233, 227);

        painter.setPen(color.darker());
        painter.drawLine(0, 0, width() - 1, 0);
        painter.drawLine(0, 0, 0, height() - 1);

        painter.setPen(color.lighter());
        painter.drawLine(width() - 1, 0, width() - 1, height() - 1);
        painter.drawLine(0, height() - 1, width() - 1, height() - 1);

        abc = !abc;
    }

    private static boolean abc;
    public static void main(String args[]) {
        QApplication.initialize(args);

        final StatusBarLabel label = new StatusBarLabel("hello");
        label.show();

        QThread t = new QThread(new Runnable() {

            public void run() {
                while (true) {
                    QApplication.invokeLater(new Runnable() {
                        public void run() {
                            label.setText("hello" + abc);
                            label.update();                            
                        }
                    });

                    try {
                        Thread.sleep(1L);
                    } catch (Exception sarestupid) {
                        
                    }
                }
            }

        });
        t.start();

        QApplication.execStatic();
        QApplication.shutdown();
    }
    
}
