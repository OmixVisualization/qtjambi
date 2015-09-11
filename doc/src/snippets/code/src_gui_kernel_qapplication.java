import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class src_gui_kernel_qapplication {
    public void fooBar() {

        /* QApplication(int&, char**) constructor not available in Jambi
//! [0]
        int main(int argc, char *rgv)
        {
        #ifdef Q_WS_X11
            booleansuseGUI = getenv("DISPLAY") != 0;
        #else
            booleansuseGUI = true;
        #endif
            QApplication app(argc, argv, useGUI);

            if (useGUI) {
               // start GUI version
               ...
            } else {
               // start non-GUI version
               ...
            }
            return app.exec();
        }
//! [0]
 */


//! [1]
        QApplication.setStyle(new QWindowsStyle());
//! [1]

    }
//! [2]
        public static void main(String args[]) {
            QApplication.setColorSpec(QApplication.ColorSpec.ManyColor.value());
            QApplication.initialize(args);

            /* ... */
            QApplication.execStatic();
            QApplication.shutdown();
        }
//! [2]


//! [3]
        public static QSize sizeHint() {
            return new QSize(80, 25).expandedTo(QApplication.globalStrut());
        }
//! [3]


//! [4]
        public final void showAllHiddenTopLevelWidgets() {
            for (QWidget widget : QApplication.topLevelWidgets()) {
                if (widget.isHidden())
                    widget.show();
            }
        }
//! [4]


//! [5]
        public final void updateAllWidgets() {
            for (QWidget widget : QApplication.allWidgets())
                widget.update();
        }
//! [5]
}

class FooBar extends QtJambiObject {
//! [6]
        public static void main(String args[]) {
            QApplication.setDesktopSettingsAware(false);
            QApplication.initialize(args);

            /* ... */

            QApplication.execStatic();
            QApplication.shutdown();
        }
//! [6]

        public void startTheDrag() {
            QPoint startPos = new QPoint(), currentPos = new QPoint();
//! [7]
            if (startPos.subtract(currentPos).manhattanLength() >=
                QApplication.startDragDistance()) {
                startTheDrag();
            }
//! [7]
        }


        QMainWindow mainWindow;
        public boolean saveDocument() { return false; }
//! [8]
        public final void commitData(QSessionManager manager)
        {
            if (manager.allowsInteraction()) {
                QMessageBox.StandardButton ret = QMessageBox.warning(
                                                    mainWindow,
                                                    tr("My Application"),
                                                    tr("Save changes to document?"),
                                                    QMessageBox.StandardButton.createQFlags(
                                                            QMessageBox.StandardButton.Save,
                                                            QMessageBox.StandardButton.Discard,
                                                            QMessageBox.StandardButton.Cancel)
                                                 );

                switch (ret) {
                case Save:
                    manager.release();
                    if (!saveDocument())
                        manager.cancel();
                    break;
                case Discard:
                    break;
                case Cancel:
                default:
                    manager.cancel();
                }
            } else {
                // we did not get permission to interact, then
                // do something reasonable instead
            }
        }
//! [8]

/* ??
//! [9]
        appname -session id
//! [9]
*/

        public void do_something(String s) {
            QSessionManager mySession = null;
//! [10]
            for (String command : mySession.restartCommand())
                do_something(command);
//! [10]



//! [11]
        for (String command : mySession.discardCommand())
            do_something(command);
//! [11]


        {
        int x = 0, y = 0;
//! [12]
        QWidget widget = QApplication.widgetAt(x, y);
        if (widget != null)
            widget = widget.window();
//! [12]
        }


        {
        QPoint point = null;
//! [13]
        QWidget widget = QApplication.widgetAt(point);
        if (widget != null)
            widget = widget.window();
//! [13]
        }


    }
}
