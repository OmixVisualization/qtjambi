import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class src_gui_dialogs_qmessagebox extends QWidget {
    public static void main(String args[]) {
        QApplication.initialize(args);
    }

    private void snippetWrapper0() {

//! [0]
        QMessageBox.StandardButtons buttons = new QMessageBox.StandardButtons();
        buttons.set(QMessageBox.StandardButton.Save);
        buttons.set(QMessageBox.StandardButton.Discard);
        buttons.set(QMessageBox.StandardButton.Cancel);
        QMessageBox.StandardButton ret = QMessageBox.warning(this, tr("My Application"),
                          tr("The document has been modified.\n"
                             + "Do you want to save your changes?"),
                             buttons, QMessageBox.StandardButton.Save);
//! [0]
    }


    private void snippetWrapper1() {
//! [1]
        QMessageBox msgBox = new QMessageBox();
        msgBox.setStandardButtons(QMessageBox.StandardButton.Yes, QMessageBox.StandardButton.No);
        switch (QMessageBox.StandardButton.resolve(msgBox.exec())) {
        case Yes:
            // yes was clicked
            break;
        case No:
            // no was clicked
            break;
        default:
            // should never be reached
            break;
        }
//! [1]
    }

    private void snippetWrapper2() {
//! [2]
        QMessageBox msgBox = new QMessageBox();
        QPushButton connectButton = msgBox.addButton(tr("Connect"), QMessageBox.ButtonRole.ActionRole);
        QPushButton abortButton = msgBox.addButton(QMessageBox.StandardButton.Abort);

        msgBox.exec();

        if (msgBox.clickedButton() == connectButton) {
            // connect
        } else if (msgBox.clickedButton() == abortButton) {
            // abort
        }
//! [2]
    }

    private void snippetWrapper3() {
//! [3]
        QMessageBox messageBox = new QMessageBox(this);
        QAbstractButton disconnectButton =
              messageBox.addButton(tr("Disconnect"), QMessageBox.ButtonRole.ActionRole);
        // ...
        messageBox.exec();
        if (messageBox.clickedButton() == disconnectButton) {
           // ...
        }
//! [3]
    }

/*
//! [4]
        #include <QApplication>
        #include <QMessageBox>

        int main(int argc, char rgv[])
        {
            QT_REQUIRE_VERSION(argc, argv, "4.0.2")

            QApplication app(argc, argv);
            //...
            return app.exec();
        }
//! [4]
*/
}
