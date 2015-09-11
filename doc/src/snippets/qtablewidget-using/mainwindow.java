import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;

import java.util.*;

public class mainwindow extends QMainWindow
{

    public mainwindow()
    {
        QMenu fileMenu = new QMenu(tr("File"));

        QAction quitAction = fileMenu.addAction(tr("Exit"));
        quitAction.setShortcut(tr("Ctrl+Q"));

        QMenu itemsMenu = new QMenu(tr("Items"));

        QAction sumItemsAction = itemsMenu.addAction(tr("Sum Items"));
        QAction averageItemsAction = itemsMenu.addAction(tr("Average Items"));

        menuBar().addMenu(fileMenu);
        menuBar().addMenu(itemsMenu);

    //! [0]
        tableWidget = new QTableWidget(12, 3, this);
    //! [0]
        tableWidget.setSelectionMode(QAbstractItemView.SelectionMode.ExtendedSelection);

    //! [1]
        QTableWidgetItem valuesHeaderItem = new QTableWidgetItem(tr("Values"));
        tableWidget.setHorizontalHeaderItem(0, valuesHeaderItem);
    //! [1]
        valuesHeaderItem.setTextAlignment(Qt.AlignmentFlag.AlignVCenter.value());
        QTableWidgetItem squaresHeaderItem = new QTableWidgetItem(tr("Squares"));
        squaresHeaderItem.setIcon(new QIcon(new QPixmap("classpath:images/squared.png")));
        squaresHeaderItem.setTextAlignment(Qt.AlignmentFlag.AlignVCenter.value());
    //! [2]
        QTableWidgetItem cubesHeaderItem = new QTableWidgetItem(tr("Cubes"));
        cubesHeaderItem.setIcon(new QIcon(new QPixmap("classpath:images/cubed.png")));
        cubesHeaderItem.setTextAlignment(Qt.AlignmentFlag.AlignVCenter.value());
    //! [2]
        tableWidget.setHorizontalHeaderItem(1, squaresHeaderItem);
        tableWidget.setHorizontalHeaderItem(2, cubesHeaderItem);

        quitAction.triggered.connect(this, "close()");
        sumItemsAction.triggered.connect(this, "sumItems()");
        averageItemsAction.triggered.connect(this, "averageItems()");

        setupTableItems();

        setCentralWidget(tableWidget);
        setWindowTitle(tr("Table Widget"));
    }

    void setupTableItems()
    {
        for (int row = 0; row < tableWidget.rowCount()-1; ++row) {
            for (int column = 0; column < tableWidget.columnCount(); ++column) {
    //! [3]
        QTableWidgetItem newItem =
            new QTableWidgetItem(String.valueOf(Math.pow(row, column+1)));
        tableWidget.setItem(row, column, newItem);
    //! [3]
            }
        }
        for (int column = 0; column < tableWidget.columnCount(); ++column) {
            QTableWidgetItem newItem = new QTableWidgetItem();
            newItem.setFlags(new Qt.ItemFlags(Qt.ItemFlag.ItemIsEnabled));
            tableWidget.setItem(tableWidget.rowCount()-1, column, newItem);
        }
    }

    void averageItems()
    {
        List<QTableWidgetItem > selected = tableWidget.selectedItems();
        int number = 0;
        double total = 0;

        for (QTableWidgetItem item : selected) {
            boolean ok;
            double value = Double.parseDouble(item.text());

            if (!item.text().equals("")) {
                total += value;
                number++;
            }
        }
        if (number > 0)
            tableWidget.currentItem().setText(String.valueOf(total/number));
    }

    void sumItems()
    {
    //! [4]
        List<QTableWidgetItem> selected = tableWidget.selectedItems();
        int number = 0;
        double total = 0;

        for (QTableWidgetItem item : selected) {
            double value = Double.parseDouble(item.text());

            if (!item.text().equals("")) {
                total += value;
                ++number;
            }
        }
    //! [4]
        if (number > 0)
            tableWidget.currentItem().setText(String.valueOf(total));
    }

    QAction removeAction;
    QTableWidget tableWidget;
}
