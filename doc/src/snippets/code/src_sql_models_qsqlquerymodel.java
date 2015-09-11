import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class src_sql_models_qsqlquerymodel {
    public static void main(String args[]) {
        QApplication.initialize(args);

    {
    QSqlQueryModel myModel = new QSqlQueryModel();

//! [0]
    while (myModel.canFetchMore(null))
        myModel.fetchMore(null);
//! [0]
    }
    {
//! [1]
    QSqlQueryModel model = new QSqlQueryModel();
    model.setQuery("select * from MyTable");
    if (model.lastError().isValid())
        System.out.println(model.lastError());
//! [1]
    }

    }
}
