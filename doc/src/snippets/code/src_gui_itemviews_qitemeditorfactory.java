import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class src_gui_itemviews_qitemeditorfactory {

    QColor color;

//! [0]
    @QtPropertyReader
    @QtPropertyUser
    public QColor color() { return color; }

    @QtPropertyWriter
    public void setColor(QColor color) { this.color = color; }
//! [0]

    public static void main(String args[]) {
        QApplication.initialize(args);


        /*
//! [1]
    QItemEditorCreator itemCreator =
        new QItemEditorCreator<MyEditor>("color");

    QItemEditorFactory factory = new QItemEditorFactory;
//! [1]


//! [2]
    QItemEditorFactory editorFactory = new QItemEditorFactory;
    QItemEditorCreatorBase creator = new QStandardItemEditorCreator<MyFancyDateTimeEdit>();
    editorFactory.registerEditor(QVariant.DateType, creator);
//! [2]


//! [3]
    @QtPropertyReader
    @QtPropertyUser
    public QColor color() { return color; }

    @QtPropertyWriter
    public void setColor(QColor color) { this.color = color); }
//! [3]

*/

    }
}
