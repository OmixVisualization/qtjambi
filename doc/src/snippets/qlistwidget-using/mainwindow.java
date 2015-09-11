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

        insertAction = itemsMenu.addAction(tr("Insert Item"));
        removeAction = itemsMenu.addAction(tr("Remove Item"));
        QAction ascendingAction = itemsMenu.addAction(tr("Sort in Ascending Order"));
        QAction descendingAction = itemsMenu.addAction(tr("Sort in Descending Order"));

        menuBar().addMenu(fileMenu);
        menuBar().addMenu(itemsMenu);


    //! [0]
        QListWidget listWidget = new QListWidget(this);
    //! [0]

        //listWidget = new QListWidget(this);
        listWidget.setSelectionMode(QAbstractItemView.SelectionMode.SingleSelection);

        quitAction.triggered.connect(this, "close()");
        ascendingAction.triggered.connect(this, "sortAscending()");
        descendingAction.triggered.connect(this, "sortDescending()");
        insertAction.triggered.connect(this, "insertItem()");
        removeAction.triggered.connect(this, "removeItem()");
        listWidget.currentItemChanged.connect(this, "updateMenus(QListWidgetItem)");

        setupListItems();
        updateMenus(listWidget.currentItem());

        setCentralWidget(listWidget);
        setWindowTitle(tr("List Widget"));
    }

    void setupListItems()
    {
    //! [1]
        new QListWidgetItem(tr("Oak"), listWidget);
        new QListWidgetItem(tr("Fir"), listWidget);
        new QListWidgetItem(tr("Pine"), listWidget);
    //! [1]
        new QListWidgetItem(tr("Birch"), listWidget);
    //! [2]
        new QListWidgetItem(tr("Hazel"), listWidget);
    //! [2]
        new QListWidgetItem(tr("Redwood"), listWidget);
    //! [3]
        new QListWidgetItem(tr("Sycamore"), listWidget);
        new QListWidgetItem(tr("Chestnut"), listWidget);
        new QListWidgetItem(tr("Mahogany"), listWidget);
    //! [3]
    }

    void sortAscending()
    {
    //! [4]
        listWidget.sortItems(Qt.SortOrder.AscendingOrder);
    //! [4]
    }

    void sortDescending()
    {
    //! [5]
        listWidget.sortItems(Qt.SortOrder.DescendingOrder);
    //! [5]
    }

    void insertItem()
    {
        if (listWidget.currentItem() == null)
            return;

        String itemText = QInputDialog.getText(this, tr("Insert Item"),
            tr("Input text for the new item:"));

        if (itemText == null)
            return;

    //! [6]
        QListWidgetItem newItem = new QListWidgetItem();
        newItem.setText(itemText);
    //! [6]
        int row = listWidget.row(listWidget.currentItem());
    //! [7]
        listWidget.insertItem(row, newItem);
    //! [7]

        String toolTipText = tr("Tooltip:") + itemText;
        String statusTipText = tr("Status tip:") + itemText;
        String swhatsThisText = tr("What's This?:") + itemText;
    //! [8]
        newItem.setToolTip(toolTipText);
        newItem.setStatusTip(toolTipText);
        newItem.setWhatsThis(whatsThisText);
    //! [8]
    }

    void removeItem()
    {
        listWidget.takeItem(listWidget.row(listWidget.currentItem()));
    }

    void updateMenus(QListWidgetItem current)
    {
        insertAction.setEnabled(current != null);
        removeAction.setEnabled(current != null);
    }

    QAction insertAction;
    QAction removeAction;
    QListWidget listWidget;
    String whatsThisText = null;

    public static void main(String args[])
    {
        QApplication.initialize(args);

        new mainwindow().show();

        QApplication.execStatic();
        QApplication.shutdown();
    }
}
