import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class src_gui_dialogs_qfontdialog extends QWidget {
    public static void main(String args[]) {
        QApplication.initialize(args);

        }
    public void doFontDialog() {

//! [0]
    QFont font;
    QFontDialog.Result fontResult  = QFontDialog.getFont(new QFont("Helvetica [Cronyx]", 10), this);
    if (fontResult.ok) {
        // the user clicked OK and font is set to the font the user selected
        font = fontResult.font;
    } else {
        // the user canceled the dialog; font is set to the initial
        // value, in this case Helvetica [Cronyx], 10
    }
//! [0]

    QWidget myWidget = new QWidget(this);

//! [1]
    myWidget.setFont(QFontDialog.getFont(myWidget.font()).font);
//! [1]


//! [2]
    QFont f;
    QFontDialog.Result fr = QFontDialog.getFont(new QFont("Times", 12), this);
    if (fr.ok) {
        // font is set to the font the user selected
        f = fr.font;
    } else {
        // the user canceled the dialog; font is set to the initial
        // value, in this case Times, 12.
    }
//! [2]


//! [3]
    myWidget.setFont(QFontDialog.getFont(myWidget.font()).font);
//! [3]


//! [4]
    QFont fnt;
    QFontDialog.Result fntResult= QFontDialog.getFont(this);
    if (fntResult.ok) {
        // font is set to the font the user selected
        fnt = fntResult.font;
    } else {
        // the user canceled the dialog; font is set to the default
        // application font, QApplication.font()
    }
//! [4]


    }
}
