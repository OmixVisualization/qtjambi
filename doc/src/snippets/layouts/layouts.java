import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;


public class layouts
{
    public static void main(String args[])
    {
        QApplication.initialize(args);

        if (true)
        {
    //! [0]
        QWidget window = new QWidget();
    //! [0] //! [1]
        QPushButton button1 = new QPushButton("One");
    //! [1] //! [2]
        QPushButton button2 = new QPushButton("Two");
        QPushButton button3 = new QPushButton("Three");
        QPushButton button4 = new QPushButton("Four");
        QPushButton button5 = new QPushButton("Five");
    //! [2]

    //! [3]
        QHBoxLayout layout = new QHBoxLayout();
    //! [3] //! [4]
        layout.addWidget(button1);
        layout.addWidget(button2);
        layout.addWidget(button3);
        layout.addWidget(button4);
        layout.addWidget(button5);

        window.setLayout(layout);
    //! [4] //! [5]
        window.show();
    //! [5]
        }

        if (true)
        {
    //! [6]
        QWidget window = new QWidget();
    //! [6] //! [7]
        QPushButton button1 = new QPushButton("One");
    //! [7] //! [8]
        QPushButton button2 = new QPushButton("Two");
        QPushButton button3 = new QPushButton("Three");
        QPushButton button4 = new QPushButton("Four");
        QPushButton button5 = new QPushButton("Five");
    //! [8]

    //! [9]
        QVBoxLayout layout = new QVBoxLayout();
    //! [9] //! [10]
        layout.addWidget(button1);
        layout.addWidget(button2);
        layout.addWidget(button3);
        layout.addWidget(button4);
        layout.addWidget(button5);

        window.setLayout(layout);
    //! [10] //! [11]
        window.show();
    //! [11]
        }

        if (true)
        {
    //! [12]
        QWidget window = new QWidget();
    //! [12] //! [13]
        QPushButton button1 = new QPushButton("One");
    //! [13] //! [14]
        QPushButton button2 = new QPushButton("Two");
        QPushButton button3 = new QPushButton("Three");
        QPushButton button4 = new QPushButton("Four");
        QPushButton button5 = new QPushButton("Five");
    //! [14]

    //! [15]
        QGridLayout layout = new QGridLayout();
    //! [15] //! [16]
        layout.addWidget(button1, 0, 0);
        layout.addWidget(button2, 0, 1);
        layout.addWidget(button3, 1, 0, 1, 2);
        layout.addWidget(button4, 2, 0);
        layout.addWidget(button5, 2, 1);

        window.setLayout(layout);
    //! [16] //! [17]
        window.show();
    //! [17]
        }

        QApplication.execStatic();
        QApplication.shutdown();
    }
}
