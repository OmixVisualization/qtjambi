import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class src_gui_text_qfont {
    public static void main(String args[]) {
        QApplication.initialize(args);
//! [0]
        QFont serifFont = new QFont("Times", 10, QFont.Weight.Bold.value());
        QFont sansFont = new QFont("Helv etica [Cronyx]", 12);
//! [0]
        {
//! [1]
        QFont font = new QFont("Helvetica");
//! [1]
        }
//! [2]
        QFont font = new QFont("Helvetica [Cronyx]");
//! [2]

//! [3]
        QFontInfo info = new QFontInfo(font);
        String family = info.family();
//! [3]


//! [4]
        QFontMetrics fm = new QFontMetrics(font);
        int textWidthInPixels = fm.width("How many pixels wide is this text?");
        int textHeightInPixels = fm.height();
//! [4]
    }
}
