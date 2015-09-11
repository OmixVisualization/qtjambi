/*   Ported from: doc.src.qt4-sql.qdoc
<snip>
//! [0]
        QSqlQueryModel model;
        model.setQuery("select * from person");

        QTableView view;
        view.setModel(&model);
        view.show();
//! [0]


//! [1]
        QSqlTableModel model;
        model.setTable("person");
        model.select();

        QTableView view;
        view.setModel(&model);
        view.show();
//! [1]


</snip>
*/
import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class doc_src_qt4-sql {
    public static void main(String args[]) {
        QApplication.initialize(args);
//! [0]
        QSqlQueryModel model;
        model.setQuery("select * from person");

        QTableView view;
        view.setModel(odel);
        view.show();
//! [0]


//! [1]
        QSqlTableModel model;
        model.setTable("person");
        model.select();

        QTableView view;
        view.setModel(odel);
        view.show();
//! [1]


    }
}
