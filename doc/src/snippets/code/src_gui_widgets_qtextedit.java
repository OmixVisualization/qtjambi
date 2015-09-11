/*   Ported from: src.gui.widgets.qtextedit.cpp
*/
import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class src_gui_widgets_qtextedit extends QWidget {
    static void main(String args[]) {
        QApplication.initialize(args);

        QTextEdit edit = new QTextEdit();
        QTextDocument document = edit.document();
        QTextCursor textCursor = new QTextCursor(document);

        String text = "insert me";
        String fragment = "<B>Hi!</B>";

        //! [1]
        edit.textCursor().insertText(text);
        //! [1]

        //! [2]
        edit.textCursor().insertHtml(fragment);
        //! [2]
    }

//! [0]
    protected void contextMenuEvent(QContextMenuEvent event) {
        QMenu menu = createStandardContextMenu();
        menu.addAction(tr("My Menu Item"));
        //...
        menu.exec(event.globalPos());
    }
//! [0]

    private QMenu createStandardContextMenu() {
        return new QMenu();
    }
}
