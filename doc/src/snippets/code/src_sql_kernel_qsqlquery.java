import java.util.*;

import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class src_sql_kernel_qsqlquery {
    public static void main(String args[]) {
        QApplication.initialize(args);
/*
//! [0]
        SELECT forename, surname FROM people;
//! [0]
*/
    {
//! [1]
    QSqlQuery q = new QSqlQuery("select * from employees");
    QSqlRecord rec = q.record();

    System.out.println("Number of columns: " + rec.count());

    int nameCol = rec.indexOf("name"); // index of the field "name"
    while (q.next())
        System.out.println(q.value(nameCol).toString()); // output all names
//! [1]
    }
    {
//! [2]
        QSqlQuery q = new QSqlQuery();
        q.prepare("insert into myTable values (?, ?)");

    List<Integer> ints = new ArrayList<Integer>();
        ints.add(1);
    ints.add(2);
    ints.add(3);
    ints.add(4);
        q.addBindValue(ints);

    List<String> names = new ArrayList<String>();
    names.add("Harald");
    names.add("Boris");
    names.add("Trond");
    names.add(null);
        q.addBindValue(names);

        if (!q.execBatch())
            System.out.println(q.lastError());
//! [2]
    }
/*
//! [3]
    1  Harald
    2  Boris
    3  Trond
        4  NULL
//! [3]
*/

    }
}
