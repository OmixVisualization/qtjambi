import java.util.*;

import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class src_sql_kernel_qsqlresult {
    public static void main(String args[]) {
        QApplication.initialize(args);
//! [0]
    QSqlQuery q = new QSqlQuery();
    q.prepare("insert into test (i1, i2, s) values (?, ?, ?)");

    List<Integer> col1 = new ArrayList<Integer>();
    List<Integer> col2 = new ArrayList<Integer>();
    List<String>  col3 = new ArrayList<String>();

    col1.add(1);
    col1.add(3);
    col2.add(2);
    col2.add(4);
    col3.add("hello");
    col3.add("world");

    q.bindValue(0, col1);
    q.bindValue(1, col2);
    q.bindValue(2, col3);

    if (!q.execBatch())
        System.out.println(q.lastError());
//! [0]

/* Not relevant in Java
//! [1]
    QSqlQuery query = ...;
    QVariant v = query.result().handle();
    if (v.isValid() && qstrcmp(v.typeName(), "sqlite3_stmt*")) {
        // v.data() returns a pointer to the handle
        sqlite3_stmt andle = tatic_cast<sqlite3_stmt **>(v.data());
        if (handle != 0) { // check that it is not NULL
            ...
        }
    }
//! [1]


//! [2]
    if (v.typeName() == "PGresult*") {
        PGresult andle = tatic_cast<PGresult **>(v.data());
        if (handle != 0) ...
    }

    if (v.typeName() == "MYSQL_STMT*") {
        MYSQL_STMT andle = tatic_cast<MYSQL_STMT **>(v.data());
        if (handle != 0) ...
    }
//! [2]
*/

    }
}
