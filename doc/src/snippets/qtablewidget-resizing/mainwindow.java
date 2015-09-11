import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;

import java.util.*;

public class mainwindow extends QMainWindow
{
    QTableWidget tableWidget;

    public mainwindow()
    {
        QMenu fileMenu = new QMenu(tr("File"));

        QAction quitAction = fileMenu.addAction(tr("Exit"));
        quitAction.setShortcut(tr("Ctrl+Q"));

        QMenu tableMenu = new QMenu(tr("Table"));

        QAction tableWidthAction = tableMenu.addAction(tr("Change Table Width"));
        QAction tableHeightAction = tableMenu.addAction(tr("Change Table Height"));

        menuBar().addMenu(fileMenu);
        menuBar().addMenu(tableMenu);

    //! [0]
        tableWidget = new QTableWidget(this);
    //! [0]
        tableWidget.setSelectionMode(QAbstractItemView.SelectionMode.ExtendedSelection);

        quitAction.triggered.connect(this, "close()");
        tableWidthAction.triggered.connect(this, "changeWidth()");
        tableHeightAction.triggered.connect(this, "changeHeight()");

        setupTableItems();

        setCentralWidget(tableWidget);
        setWindowTitle(tr("Table Widget Resizing"));
    }

    void setupTableItems()
    {
    //! [1]
        tableWidget.setRowCount(10);
        tableWidget.setColumnCount(5);
    //! [1]

        for (int row = 0; row < tableWidget.rowCount(); ++row) {
            for (int column = 0; column < tableWidget.columnCount(); ++column) {
    //! [2]
        QTableWidgetItem newItem = new QTableWidgetItem("" + (row+1) + ", " + (column+1));
        tableWidget.setItem(row, column, newItem);
    //! [2]
            }
        }
    }

    void changeWidth()
    {
        Integer newWidth = QInputDialog.getInteger(this, tr("Change table width"),
            tr("Input the number of columns required (1-20):"),
            tableWidget.columnCount(), 1, 20, 1);

        if (newWidth != null)
            tableWidget.setColumnCount(newWidth);
    }

    void changeHeight()
    {
        Integer newHeight = QInputDialog.getInteger(this, tr("Change table height"),
            tr("Input the number of rows required (1-20):"),
            tableWidget.rowCount(), 1, 20, 1);

        if (newHeight != null)
            tableWidget.setRowCount(newHeight);
    }

    public static void main(String args[])
    {
        QApplication.initialize(args);

        new mainwindow().show();

        QApplication.execStatic();
        QApplication.shutdown();
    }
}
