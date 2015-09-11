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
        removeAction.setEnabled(false);
        itemsMenu.addSeparator();
        ascendingAction = itemsMenu.addAction(tr("Sort in Ascending Order"));
        descendingAction = itemsMenu.addAction(tr("Sort in Descending Order"));
        autoSortAction = itemsMenu.addAction(tr("Automatically Sort Items"));
        autoSortAction.setCheckable(true);
        itemsMenu.addSeparator();
        QAction findItemsAction = itemsMenu.addAction(tr("Find Items"));
        findItemsAction.setShortcut(tr("Ctrl+F"));

        menuBar().addMenu(fileMenu);
        menuBar().addMenu(itemsMenu);

    //  For convenient quoting:
    //! [0]
        QTreeWidget treeWidget = new QTreeWidget(this);
    //! [0]

        treeWidget = new QTreeWidget(this);
    //! [1]
        treeWidget.setColumnCount(2);
    //! [1] //! [2]
        List<String> headers = new Vector<String>();
        headers.add(tr("Subject"));
        headers.add(tr("Default"));
        treeWidget.setHeaderLabels(headers);
    //! [2]

        /*connect(quitAction, SIGNAL(triggered()), this, SLOT(close()));
        connect(ascendingAction, SIGNAL(triggered()), this, SLOT(sortAscending()));
        connect(autoSortAction, SIGNAL(triggered()), this, SLOT(updateSortItems()));
        connect(descendingAction, SIGNAL(triggered()), this, SLOT(sortDescending()));
        connect(findItemsAction, SIGNAL(triggered()), this, SLOT(findItems()));
        connect(insertAction, SIGNAL(triggered()), this, SLOT(insertItem()));
        connect(removeAction, SIGNAL(triggered()), this, SLOT(removeItem()));
        connect(treeWidget,
                SIGNAL(currentItemChanged(QTreeWidgetItem , QTreeWidgetItem )),
                this, SLOT(updateMenus(QTreeWidgetItem )));*/

        setupTreeItems();
        updateMenus(treeWidget.currentItem());

        setCentralWidget(treeWidget);
        setWindowTitle(tr("Tree Widget"));
    }

    void setupTreeItems()
    {
    //! [3]
        QTreeWidgetItem cities = new QTreeWidgetItem(treeWidget);
        cities.setText(0, tr("Cities"));
        QTreeWidgetItem osloItem = new QTreeWidgetItem(cities);
        osloItem.setText(0, tr("Oslo"));
        osloItem.setText(1, tr("Yes"));
    //! [3]

        (new QTreeWidgetItem(cities)).setText(0, tr("Stockholm"));
        (new QTreeWidgetItem(cities)).setText(0, tr("Helsinki"));
        (new QTreeWidgetItem(cities)).setText(0, tr("Copenhagen"));

    //! [4] //! [5]
        QTreeWidgetItem planets = new QTreeWidgetItem(treeWidget, cities);
    //! [4]
        planets.setText(0, tr("Planets"));
    //! [5]
        (new QTreeWidgetItem(planets)).setText(0, tr("Mercury"));
        (new QTreeWidgetItem(planets)).setText(0, tr("Venus"));

        QTreeWidgetItem earthItem = new QTreeWidgetItem(planets);
        earthItem.setText(0, tr("Earth"));
        earthItem.setText(1, tr("Yes"));

        (new QTreeWidgetItem(planets)).setText(0, tr("Mars"));
        (new QTreeWidgetItem(planets)).setText(0, tr("Jupiter"));
        (new QTreeWidgetItem(planets)).setText(0, tr("Saturn"));
        (new QTreeWidgetItem(planets)).setText(0, tr("Uranus"));
        (new QTreeWidgetItem(planets)).setText(0, tr("Neptune"));
        (new QTreeWidgetItem(planets)).setText(0, tr("Pluto"));
    }

    void findItems()
    {
        String itemText = QInputDialog.getText(this, tr("Find Items"),
            tr("Text to find (including wildcards):"));

        if (itemText.length() < 1)
            return;

    //! [6]
        //QTreeWidgetItem item = new QTreeWidgetItem();
    //! [6]
        for (QTreeWidgetItem item : treeWidget.selectedItems())
            item.setSelected(true);

    //! [7]
        List<QTreeWidgetItem> found = treeWidget.findItems(
            itemText, new Qt.MatchFlags(Qt.MatchFlag.MatchWildcard));

        for (QTreeWidgetItem item : found) {
            item.setSelected(true);
            // Show the item.text(0) for each item.
        }
    //! [7]
    }

    void insertItem()
    {
        QTreeWidgetItem currentItem = treeWidget.currentItem();

        if (currentItem == null)
            return;

        String itemText = QInputDialog.getText(this, tr("Insert Item"),
            tr("Input text for the new item:"));

        if (itemText.length() < 1)
            return;

    //! [8]
        QTreeWidgetItem parent = currentItem.parent();
        QTreeWidgetItem newItem = new QTreeWidgetItem();
        if (parent != null)
            newItem = new QTreeWidgetItem(parent, treeWidget.currentItem());
        else
    //! [8] //! [9]
            newItem = new QTreeWidgetItem(treeWidget, treeWidget.currentItem());
    //! [9]

        newItem.setText(0, itemText);
    }

    void removeItem()
    {
        QTreeWidgetItem currentItem = treeWidget.currentItem();

        if (currentItem == null)
            return;

    //! [10]
        QTreeWidgetItem parent = currentItem.parent();
        int index;

        if (parent != null) {
            index = parent.indexOfChild(treeWidget.currentItem());

        } else {
            index = treeWidget.indexOfTopLevelItem(treeWidget.currentItem());

    //! [10] //! [11]
        }
    //! [11]
    }

    void sortAscending()
    {
        treeWidget.sortItems(0, Qt.SortOrder.AscendingOrder);
    }

    void sortDescending()
    {
        treeWidget.sortItems(0, Qt.SortOrder.DescendingOrder);
    }

    void updateMenus(QTreeWidgetItem current)
    {
        insertAction.setEnabled(current != null);
        removeAction.setEnabled(current != null);
    }

    void updateSortItems()
    {
        ascendingAction.setEnabled(!autoSortAction.isChecked());
        descendingAction.setEnabled(!autoSortAction.isChecked());

        treeWidget.setSortingEnabled(autoSortAction.isChecked());
    }

    private QAction ascendingAction;
    private QAction autoSortAction;
    private QAction descendingAction;
    private QAction insertAction;
    private QAction removeAction;
    private QTreeWidget treeWidget;

    public static void main(String args[])
    {
        QApplication.initialize(args);

        new mainwindow().show();

        QApplication.execStatic();
        QApplication.shutdown();
    }
}
