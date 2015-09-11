package org.qtjambi.manualtests;

import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.QVariant;

import java.util.List;
import java.util.Arrays;
import java.util.ArrayList;

public class TestJDBC {

    // create table person (id integer primary key, name varchar(100), lastname varchar(100), countryid integer);
    // create table country (id integer primary key, name varchar(100));

    private enum Driver {
        PostgreSQL,
        Derby,
        MySQL,
        H2
    }

    static private String driverNames[] = {
        "org.postgresql.Driver",
        "org.apache.derby.jdbc.EmbeddedDriver",
        "com.mysql.jdbc.Driver",
        "org.h2.Driver"
    };

    static private String dbNames[] = {
        "jdbc:postgresql://127.0.0.1/main",
        "jdbc:derby:firstdb", // only usable locally, since no server is set up. Install derby and make the
                              // database on your local machine to run this test.
        "jdbc:mysql://qfwfq.europe.nokia.com/main",
        "jdbc:h2:~/test" // Ask Eskil nicely to start this up.
    };

    static private String personTable[] = {
            "person",
            "PERSON",
            "person",
            "PERSON"
    };

    static private String countryTable[] = {
        "country",
        "COUNTRY",
        "country",
        "COUNTRY"
    };

    static private String userNames[] = {
            "qt",
            "", // login not required
            "qt",
            "SA"
    };

    static private String passwords[] = {
            "qqqqqq",
            "",
            "qqqqqq",
            "",
    };

    static private String hostNames[] = {
            "",
            "",
            "",
            "qfwfq.europe.nokia.com"
    };


    static {
        checkSize(driverNames, "driverNames");
        checkSize(dbNames, "dbNames");
        checkSize(personTable, "personTable");
        checkSize(countryTable, "countryTable");
        checkSize(userNames, "userNames");
        checkSize(passwords, "passwords");
        checkSize(hostNames, "hostNames");
    }

    private static void checkSize(Object array[], String name) {
        if (array.length < Driver.values().length) {
            String missingNames = "";
            for (int i=array.length; i<Driver.values().length; ++i) {
                if (missingNames.length() > 0)
                    missingNames += ", ";
                missingNames += Driver.values()[i].name();
            }

            throw new RuntimeException("Array '" + name + "' does not contain any entry for the following drivers: "
                    + missingNames);
        }
    }

    private static void makeRelationalTable(QTabWidget topLevel, QSqlDatabase db)  {
        QSqlRelationalTableModel model = new QSqlRelationalTableModel(null, db);
        model.setTable(personTable[ordinal]);
        model.setEditStrategy(QSqlTableModel.EditStrategy.OnFieldChange);

        int columnCount = model.columnCount();
        int columnIdx = -1;
        for (int i=0; i<columnCount; ++i) {
           String columnName = QVariant.toString(model.headerData(i, Qt.Orientation.Horizontal));
           if (columnName.toLowerCase().equals("countryid")) {
               columnIdx = i;
               break;
           }
        }

        if (columnIdx < 0)
            return;

        model.setRelation(columnIdx, new QSqlRelation(countryTable[ordinal], "id", "name"));
        if (!model.select()) {
            System.err.println(model.lastError().text());
            return;
        }

        QTableView view = new QTableView();
        view.setWindowTitle("QSqlRelationalTableModel");
        view.setItemDelegate(new QSqlRelationalDelegate(view));
        view.setModel(model);

        topLevel.addTab(view, "QSqlRelationalTableModel");
    }

    private static void makeRegularPersonTable(QTabWidget topLevel, QSqlDatabase db)  {
        QSqlTableModel model = new QSqlTableModel(null, db);
        model.setTable(personTable[ordinal]);
        if (!model.select()) {
            System.err.println(model.lastError().text());
            return ;
        }

        QTableView view = new QTableView(topLevel);
        view.setModel(model);
        topLevel.addTab(view, "person");
    }

    private static void makeRegularCountryTable(QTabWidget topLevel, QSqlDatabase db) {
        QSqlTableModel model = new QSqlTableModel(null, db);
        model.setTable(countryTable[ordinal]);
        if (!model.select()) {
            System.err.println(model.lastError().text());
            return;
        }
        QTableView view = new QTableView(topLevel);
        view.setModel(model);
        topLevel.addTab(view, "country");
    }

    // Test some things known to cause difficulties with different drivers
    private static void testScrollCompliance(QTabWidget topLevel, QSqlDatabase db) {
        QSqlQuery query = new QSqlQuery("SELECT name FROM " + personTable[ordinal], db);

        QListWidget listWidget = new QListWidget();

        listWidget.addItem(new QListWidgetItem("query.isSelect(): " + query.isSelect()));
        listWidget.addItem(new QListWidgetItem("query.isActive(): " + query.isActive()));

        query.first();
        listWidget.addItem(new QListWidgetItem("query.first(): " + query.at() + ", name: " + query.value(0)));

        query.last();
        listWidget.addItem(new QListWidgetItem("query.last(): " + query.at() + ", name:" + query.value(0)));

        query.first();
        listWidget.addItem(new QListWidgetItem("query.first() again: " + query.at() + ", name:" + query.value(0)));

        query.seek(1);
        listWidget.addItem(new QListWidgetItem("query.seek(1): " + query.at() + ", name:" + query.value(0)));

        query.seek(0);
        listWidget.addItem(new QListWidgetItem("query.seek(0): " + query.at() + ", name:" + query.value(0)));

        query.next();
        listWidget.addItem(new QListWidgetItem("query.next(): " + query.at() + ", name:" + query.value(0)));

        query.previous();
        listWidget.addItem(new QListWidgetItem("query.previous(): " + query.at() + ", name:" + query.value(0)));


        topLevel.addTab(listWidget, "Scroll compliance");
    }

    private static int ordinal = -1;
    public static void main(String args[])  {
        QApplication.initialize(args);

        org.qtjambi.qt.sql.QJdbc.initialize();

        List<String> items = new ArrayList<String>();
        for (Driver driver : Driver.values())
            items.add(driver.name());

        String selectedDriver = QInputDialog.getItem(null, "Select a driver", "Select a driver", items);

        for (Driver driver : Driver.values()) {
            if (driver.name().equals(selectedDriver))
                ordinal = driver.ordinal();
        }

        if (ordinal < 0)
            return;

        try {
            System.err.println("Loading driver '" + driverNames[ordinal] + "'");
            Class.forName(driverNames[ordinal]).newInstance();
        } catch (Exception ex) { System.err.println(ex); return; }

        QSqlDatabase db = QSqlDatabase.addDatabase("QJDBC");
        System.err.println("Selecting database '" + dbNames[ordinal] + "'");
        db.setDatabaseName(dbNames[ordinal]);
        if (hostNames[ordinal].length() > 0) {
            System.err.println("Using hostname '" + hostNames[ordinal] + "'");
            db.setHostName(hostNames[ordinal]);
        }
        if (userNames[ordinal].length() > 0) {
            db.setUserName(userNames[ordinal]);
            db.setPassword(passwords[ordinal]);
        }
        if (db.open()) {
            System.err.println("Connected!");
        } else {
            System.err.println("Connection Failed!");
            System.err.println(db.lastError().text());
            return;
        }

        QTabWidget topLevel = new QTabWidget();
        topLevel.setWindowTitle("Tables");

        makeRegularPersonTable(topLevel, db);
        makeRegularCountryTable(topLevel, db);
        makeRelationalTable(topLevel, db);
        testScrollCompliance(topLevel, db);

        topLevel.show();

        QApplication.execStatic();
        QApplication.shutdown();

        db.close();
    }
}

