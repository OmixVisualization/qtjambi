import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;

import java.util.*;

public class window extends QMainWindow
{
    public window()
    {
        setWindowTitle("Selected Items in a Table Model");

        model = new TableModel(8, 4, this);

        table = new QTableView(this);
        table.setModel(model);

        QMenu actionMenu = new QMenu(tr("Actions"), this);
        QAction fillAction = actionMenu.addAction(tr("Fill Selection"));
        QAction clearAction = actionMenu.addAction(tr("Clear Selection"));
        QAction selectAllAction = actionMenu.addAction(tr("Select All"));
        menuBar().addMenu(actionMenu);

        fillAction.triggered.connect(this, "fillSelection()");
        clearAction.triggered.connect(this, "clearSelection()");
        selectAllAction.triggered.connect(this, "selectAll()");

        selectionModel = table.selectionModel();

        statusBar();
        setCentralWidget(table);
    }

    void fillSelection()
    {
    //! [0]
        List<QModelIndex> indexes = selectionModel.selectedIndexes();

        for (QModelIndex index : indexes) {
            String text = "(" + index.row() + ", " + index.column() + ")";
            model.setData(index, text);
        }
    //! [0]
    }

    void clearSelection()
    {
        List<QModelIndex> indexes = selectionModel.selectedIndexes();

        for (QModelIndex index : indexes)
            model.setData(index, "");
    }

    void selectAll()
    {
    //! [1]
        QModelIndex parent = null;
    //! [1] //! [2]
        QModelIndex topLeft = model.index(0, 0, parent);
        QModelIndex bottomRight = model.index(model.rowCount(parent) - 1,
            model.columnCount(parent) - 1, parent);
    //! [2]

    //! [3]
        QItemSelection selection = new QItemSelection(topLeft, bottomRight);
        selectionModel.select(selection, QItemSelectionModel.SelectionFlag.Select);
    //! [3]
    }

    QAbstractItemModel model;
    QItemSelectionModel selectionModel;
    QTableView table;

    public static void main(String args[])
    {
        QApplication.initialize(args);

        new window().show();

        QApplication.execStatic();
        QApplication.shutdown();
    }
}
