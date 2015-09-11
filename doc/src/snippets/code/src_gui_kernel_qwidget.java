import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class src_gui_kernel_qwidget extends QWidget{
    public static void main(String args[]) {
        QApplication.initialize(args);

        if (true)
        {
        QWidget w = new QWidget();
//! [0]
        Qt.WindowStates state = w.windowState();

        if (w.windowState().isSet(Qt.WindowState.WindowFullScreen))
            state.set(Qt.WindowState.WindowFullScreen);
        else
            state.clear(Qt.WindowState.WindowFullScreen);

        w.setWindowState(state);
//! [0]
        }


        if (true)
        {
            QWidget w = new QWidget();
//! [1]
        Qt.WindowStates state= w.windowState();
        state.setValue(state.value() & ~Qt.WindowState.WindowMinimized.value() |
                                      Qt.WindowState.WindowActive.value());
        w.setWindowState(state);
//! [1]
        }


        if (true)
        {
                QWidget aWidget = new QWidget();
//! [3]
        aWidget.window().setWindowTitle("New Window Title");
//! [3]
        }

        if (true)
        {
                QWidget w = new QWidget();
                QFont font = new QFont();
//! [4]
        QFont fonti = new QFont("Helvetica", 12, QFont.Weight.Bold.value());
        w.setFont(font);
//! [4]
        }

        if (true)
        {
                QWidget w = new QWidget();
//! [5]
        QFont font = new QFont();
        font.setBold(false);
        w.setFont(font);
//! [5]
        }

        if (true)
        {
            QWidget w = new QWidget();
//! [6]
        w.setCursor(new QCursor(Qt.CursorShape.IBeamCursor));
//! [6]
        }

        if (true)
        {
                QWidget widget = new QWidget();
//! [7]
        QPixmap pixmap = new QPixmap(widget.size());
        widget.render(pixmap);
//! [7]
        }

    if (true)
    {
            QWidget myWidget = new QWidget();
//! [12]
        QSettings settings = new QSettings("MyCompany", "MyApp");
        myWidget.restoreGeometry((QByteArray) settings.value("myWidget/geometry"));
//! [12]
    }

/*
//! [14]
    This code is not relevant for Qt Jambi
    ...
    extern void qt_x11_set_global_double_buffer(bool);
    qt_x11_set_global_double_buffer(false);
    ...
//! [14]
*/

    }
    void tull()
    {
        QWidget a = null, b = null, c = null, d = null;

        if (true)
        {
                QWidget myWidget = null;
//! [8]
        QPainter painter = new QPainter(this);
        //...
        painter.end();
        myWidget.render(this);
//! [8]

//! [9]
        setTabOrder(a, b); // a to b
        setTabOrder(b, c); // a to b to c
        setTabOrder(c, d); // a to b to c to d
//! [9]


//! [10]
        // WRONG
        setTabOrder(c, d); // c to d
        setTabOrder(a, b); // a to b AND c to d
        setTabOrder(b, c); // a to b to c, but not c to d
//! [10]

//! [13]
        setUpdatesEnabled(false);
        bigVisualChanges();
        setUpdatesEnabled(true);
//! [13]
        if (true)
        {
                int width, height, i = 0, j = 0;
//! [2]
        width = baseSize().width() + i * sizeIncrement().width();
        height = baseSize().height() + j * sizeIncrement().height();
//! [2]
        }

        }
    }
//! [11]
    protected void closeEvent(QCloseEvent event)
    {
        QSettings settings = new QSettings("MyCompany", "MyApp");
        settings.setValue("geometry", saveGeometry());
        super.closeEvent(event);
    }
//! [11]

    static void bigVisualChanges() {}

}
