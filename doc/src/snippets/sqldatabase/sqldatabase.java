import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.sql.*;

import java.util.*;

public class sqldatabase
{

    static String tr(String text)
    {
        return QApplication.translate(text, text);
    }

    static void QSqlDatabase_snippets()
    {
        if (true)
        {
    //! [0]
        QSqlDatabase db = QSqlDatabase.addDatabase("QPSQL");
        db.setHostName("apollo");
        db.setDatabaseName("customdb");
        db.setUserName("mojito");
        db.setPassword("J0a1m8");
        boolean ok = db.open();
    //! [0]
        }

        if (true)
        {
    //! [1]
        QSqlDatabase db = QSqlDatabase.database();
    //! [1]
        }
    }

    static void QSqlField_snippets()
    {
        if (false)
        {
    //! [2]
        QSqlField field = new QSqlField("age");
        field.setValue(new QPixmap());  // WRONG
    //! [2]
        }

        if (true)
        {
    //! [3]
        QSqlField field = new QSqlField("age");
        field.setValue("123");  // casts String to int
    //! [3]
        }

        if (true)
        {
    //! [4]
        QSqlQuery query = new QSqlQuery();
    //! [4] //! [5]
        QSqlRecord record = query.record();
    //! [5] //! [6]
        QSqlField field = record.field("country");
    //! [6]
        }
    }

    static void doSomething(String str)
    {
    }

    static void QSqlQuery_snippets()
    {
        if (true)
        {
        // typical loop
    //! [7]
        QSqlQuery query = new QSqlQuery("SELECT * country FROM artist");
        while (query.next()) {
            String country = query.value(0).toString();
            doSomething(country);
        }
    //! [7]
        }

        if (true)
        {
        // field index lookup
    //! [8]
        QSqlQuery query = new QSqlQuery("SELECT * FROM artist");
        int fieldNo = query.record().indexOf("country");
        while (query.next()) {
            String country = query.value(fieldNo).toString();
            doSomething(country);
        }
    //! [8]
        }

        if (true)
        {
        // named with named
    //! [9]
        QSqlQuery query = new QSqlQuery();
        query.prepare("INSERT INTO person (id, forename, surname) " +
                      "VALUES (:id, :forename, :surname)");
        query.bindValue(":id", 1001);
        query.bindValue(":forename", "Bart");
        query.bindValue(":surname", "Simpson");
        query.exec();
    //! [9]
        }

        if (true)
        {
        // positional with named
    //! [10]
        QSqlQuery query = new QSqlQuery();
        query.prepare("INSERT INTO person (id, forename, surname) " +
                      "VALUES (:id, :forename, :surname)");
        query.bindValue(0, 1001);
        query.bindValue(1, "Bart");
        query.bindValue(2, "Simpson");
        query.exec();
    //! [10]
        }

        if (true)
        {
        // positional 1
    //! [11]
        QSqlQuery query = new QSqlQuery();
        query.prepare("INSERT INTO person (id, forename, surname) " +
                      "VALUES (?, ?, ?)");
        query.bindValue(0, 1001);
        query.bindValue(1, "Bart");
        query.bindValue(2, "Simpson");
        query.exec();
    //! [11]
        }

        if (true)
        {
        // positional 2
    //! [12]
        QSqlQuery query = new QSqlQuery();
        query.prepare("INSERT INTO person (id, forename, surname) " +
                      "VALUES (?, ?, ?)");
        query.addBindValue(1001);
        query.addBindValue("Bart");
        query.addBindValue("Simpson");
        query.exec();
    //! [12]
        }

        if (true)
        {
        // stored
    //! [13]
        QSqlQuery query = new QSqlQuery();
        query.prepare("CALL AsciiToInt(?, ?)");
        query.bindValue(0, "A");
        query.bindValue(1, 0, QSql.ParamTypeFlag.Out);
        query.exec();
        int i = query.boundValue(1).toInt(); // i is 65
    //! [13]
        Q_UNUSED(i);
        }

        QSqlQuery query = new QSqlQuery();

        if (true)
        {
        // examine with named binding
    //! [14]
        Map<String, Object> map = query.boundValues();

        for (String key : map.keySet()) {
            Object value = map.get(key);
            System.out.println(i.key().toAscii().data() << ": "
                 << i.value().toString().toAscii().data());
        }
    //! [14]
        }

        if (true)
        {
        // examine with positional binding
    //! [15]
        int i = 0;
        for (Object value : query.boundValues().values())
            System.err.println(String.valueOf(i++) + ": " + value);
    //! [15]
        }
    }

    static void QSqlQueryModel_snippets()
    {
        if (true)
        {
    //! [16]
        QSqlQueryModel model = new QSqlQueryModel();
        model.setQuery("SELECT name, salary FROM employee");
        model.setHeaderData(0, Qt.Orientation.Horizontal, tr("Name"));
        model.setHeaderData(1, Qt.Orientation.Horizontal, tr("Salary"));

    //! [17]
        QTableView view = new QTableView();
    //! [17] //! [18]
        view.setModel(model);
    //! [18] //! [19]
        view.show();
    //! [16] //! [19] //! [20]
        view.setEditTriggers(QAbstractItemView.EditTrigger.NoEditTriggers);
    //! [20]
        }

        if (true)
        {
    //! [21]
        QSqlQueryModel model = new QSqlQueryModel();
        model.setQuery("SELECT * FROM employee");
        int salary = ((Integer) model.record(4).value("salary")).intValue();
    //! [21]
        }

        if (true)
        {
        QSqlQueryModel model = new QSqlQueryModel();
    //! [22]
        int salary = ((Integer) model.data(model.index(4, 2))).intValue();
    //! [22]
        }

        /*
        for (int row = 0; row < model.rowCount(); ++row) {
            for (int col = 0; col < model.columnCount(); ++col) {
                qDebug() << model.data(model.index(row, col));
            }
        }*/
    }

    static class MyModel extends QSqlQueryModel
    {
        public Object data(QModelIndex item, int role)
        {
            return null;
        }

        int m_specialColumnNo;

        //! [23]
        public Object data(QModelIndex item, int role)
        {
            if (item.column() == m_specialColumnNo) {
                // handle column separately
            }
            //return QSqlQueryModel.data(item, role);
            return null;
        }
        //! [23]
    }

    static void QSqlTableModel_snippets()
    {
        if (true)
        {
    //! [24]
        QSqlTableModel model = new QSqlTableModel();
        model.setTable("employee");
        model.setEditStrategy(QSqlTableModel.EditStrategy.OnManualSubmit);
        model.select();
        model.removeColumn(0); // don't show the ID
        model.setHeaderData(0, Qt.Orientation.Horizontal, tr("Name"));
        model.setHeaderData(1, Qt.Orientation.Horizontal, tr("Salary"));

        QTableView view = new QTableView();
        view.setModel(model);
        view.show();
    //! [24]
        }


        if (true)
        {
    //! [25]
        QSqlTableModel model = new QSqlTableModel();
        model.setTable("employee");
        String name = model.record(4).value("name").toString();
    //! [25]
        }
    }

    static void sql_intro_snippets()
    {
        if (true)
        {
    //! [26]
        QSqlDatabase db = QSqlDatabase.addDatabase("QMYSQL");
        db.setHostName("bigblue");
        db.setDatabaseName("flightdb");
        db.setUserName("acarlson");
        db.setPassword("1uTbSbAs");
        boolean ok = db.open();
    //! [26]
        }

        if (true)
        {
    //! [27]
        QSqlDatabase firstDB = QSqlDatabase.addDatabase("QMYSQL", "first");
        QSqlDatabase secondDB = QSqlDatabase.addDatabase("QMYSQL", "second");
    //! [27]
        }

        if (true)
        {
    //! [28]
        QSqlDatabase defaultDB = QSqlDatabase.database();
    //! [28] //! [29]
        QSqlDatabase firstDB = QSqlDatabase.database("first");
    //! [29] //! [30]
        QSqlDatabase secondDB = QSqlDatabase.database("second");
    //! [30]
        }

        if (true)
        {
        // SELECT1
    //! [31]
        QSqlQuery query = new QSqlQuery();
        query.exec("SELECT name, salary FROM employee WHERE salary > 50000");
    //! [31]

    //! [32]
        while (query.next()) {
            String name = query.value(0).toString();
            int salary = ((Integer) query.value(1)).intValue();
            System.err.println(name + " " +salary);
        }
    //! [32]
        }

        if (true)
        {
        // FEATURE
    //! [33]
        QSqlQuery query = new QSqlQuery();
        int numRows;
        query.exec("SELECT name, salary FROM employee WHERE salary > 50000");

        QSqlDatabase defaultDB = QSqlDatabase.database();
        if (defaultDB.driver().hasFeature(QSqlDriver.DriverFeature.QuerySize)) {
            numRows = query.size();
        } else {
            // this can be very slow
            query.last();
            numRows = query.at() + 1;
        }
    //! [33]
        }

        if (true)
        {
        // INSERT1
    //! [34]
        QSqlQuery query = new QSqlQuery();
        query.exec("INSERT INTO employee (id, name, salary) " +
                   "VALUES (1001, 'Thad Beaumont', 65000)");
    //! [34]
        }

        if (true)
        {
        // NAMED BINDING
    //! [35]
        QSqlQuery query = new QSqlQuery();
        query.prepare("INSERT INTO employee (id, name, salary) " +
                      "VALUES (:id, :name, :salary)");
        query.bindValue(":id", 1001);
        query.bindValue(":name", "Thad Beaumont");
        query.bindValue(":salary", 65000);
        query.exec();
    //! [35]
        }

        if (true)
        {
        // POSITIONAL BINDING
    //! [36]
        QSqlQuery query = new QSqlQuery();
        query.prepare("INSERT INTO employee (id, name, salary) " +
                      "VALUES (?, ?, ?)");
        query.addBindValue(1001);
        query.addBindValue("Thad Beaumont");
        query.addBindValue(65000);
        query.exec();
    //! [36]
        }

        if (true)
        {
        // UPDATE1
    //! [37]
        QSqlQuery query = new QSqlQuery();
        query.exec("UPDATE employee SET salary = 70000 WHERE id = 1003");
    //! [37]
        }

        if (true)
        {
        // DELETE1
    //! [38]
        QSqlQuery query = new QSqlQuery();
        query.exec("DELETE FROM employee WHERE id = 1007");
    //! [38]
        }

        if (true)
        {
        // TRANSACTION
    //! [39]
        QSqlDatabase.database().transaction();
        QSqlQuery query = new QSqlQuery();
        query.exec("SELECT id FROM employee WHERE name = 'Torild Halvorsen'");
        if (query.next()) {
            int employeeId = ((Integer) query.value(0)).intValue();
            query.exec("INSERT INTO project (id, name, ownerid) " +
                       "VALUES (201, 'Manhattan Project', "
                       + String.valueOf(employeeId) + ")");
        }
        QSqlDatabase.database().commit();
    //! [39]
        }

        if (true)
        {
        // SQLQUERYMODEL1
    //! [40]
        QSqlQueryModel model = new QSqlQueryModel();
        model.setQuery("SELECT * FROM employee");

        for (int i = 0; i < model.rowCount(); ++i) {
            int id = ((Integer) model.record(i).value("id")).intValue();
            String name = model.record(i).value("name").toString();
            System.out.println(String.valueOf(id) + " " + name);
        }
    //! [40]
        }

        if (true)
        {
        // SQLTABLEMODEL1
    //! [41]
        QSqlTableModel model = new QSqlTableModel();
        model.setTable("employee");
        model.setFilter("salary > 50000");
        model.setSort(2, Qt.SortOrder.DescendingOrder);
        model.select();

        for (int i = 0; i < model.rowCount(); ++i) {
            String name = model.record(i).value("name").toString();
            int salary = ((Integer) model.record(i).value("salary")).intValue();
            System.out.println(name + " " + salary);
        }
    //! [41]
        }

        if (true)
        {
        // SQLTABLEMODEL2
        QSqlTableModel model = new QSqlTableModel();
        model.setTable("employee");

    //! [42]
        for (int i = 0; i < model.rowCount(); ++i) {
            QSqlRecord record = model.record(i);
            double salary = ((Double) record.value("salary")).doubleValue();
            salary = 1.1;
            record.setValue("salary", salary);
            model.setRecord(i, record);
        }
        model.submitAll();
    //! [42]

        // SQLTABLEMODEL3
        int row = 1;
        int column = 2;
    //! [43]
        model.setData(model.index(row, column), 75000);
        model.submitAll();
    //! [43]

        // SQLTABLEMODEL4
    //! [44]
        model.insertRows(row, 1);
        model.setData(model.index(row, 0), 1013);
        model.setData(model.index(row, 1), "Peter Gordon");
        model.setData(model.index(row, 2), 68500);
        model.submitAll();
    //! [44]

    //! [45]
        model.removeRows(row, 5);
    //! [45] //! [46]
        model.submitAll();
    //! [46]
        }
    }

    //! [47]
    static class XyzResult extends QSqlResult
    {

        public XyzResult(QSqlDriver driver)
        { super(driver); }

        protected Object data(int index) { return new Object(); }
        protected boolean isNull(int index ) { return false; }
        protected boolean reset(String query ) { return false; }
        protected boolean fetch(int index) { return false; }
        protected boolean fetchFirst() { return false; }
        protected boolean fetchLast() { return false; }
        protected int size() { return 0; }
        protected int numRowsAffected() { return 0; }
        protected QSqlRecord record() { return new QSqlRecord(); }
    };
    //! [47]

    //! [48]
    static class XyzDriver extends QSqlDriver
    {

        public XyzDriver() {}

        public boolean hasFeature(DriverFeature feature ) { return false; }
        public boolean open(String  db, String user,
                  String password, String host,
                  int port, String options)
            { return false; }
        public void close() {}
        public QSqlResult createResult() { return new XyzResult(this); }
    };
    //! [48]

    public static void main(String args[])
    {
        QApplication.initialize(args);

        QSqlDatabase_snippets();
        QSqlField_snippets();
        QSqlQuery_snippets();
        QSqlQueryModel_snippets();
        QSqlTableModel_snippets();

        XyzDriver driver = new XyzDriver();
        XyzResult result = new XyzResult(driver);
    }
}
