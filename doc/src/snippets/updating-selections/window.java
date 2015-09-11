import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;

import java.util.*;


public class window extends QWidget
{
    public static void main(String args[])
    {
        QApplication.initialize(args);

        new MainWindow().show();

        QApplication.execStatic();
        QApplication.shutdown();
    }
}

class MainWindow extends QMainWindow
{

    public MainWindow()
    {
        setWindowTitle("Selected items in a table model");

        //model = new TableModel(8, 4, this);

        table = new QTableView(this);
        table.setModel(model);

        selectionModel = table.selectionModel();
        /*connect(selectionModel,
            SIGNAL(selectionChanged(QItemSelection , QItemSelection )),
            this, SLOT(updateSelection(QItemSelection , QItemSelection )));
        connect(selectionModel,
            SIGNAL(currentChanged(QModelIndex , QModelIndex )),
            this, SLOT(changeCurrent(QModelIndex , QModelIndex )));*/

        statusBar();
        setCentralWidget(table);
    }

    //! [0]
    public void updateSelection(QItemSelection selected,
        QItemSelection deselected)
    {
        List<QModelIndex> items = selected.indexes();

        for (QModelIndex index : items) {
            String text = "(" + index.row() + ", " + index.column() +")";
            model.setData(index, text);
    //! [0] //! [1]
        }
    //! [1]

    //! [2]
        items = deselected.indexes();

        for (QModelIndex index : items)
            model.setData(index, "");
    }
    //! [2]

    //! [3]
    public void changeCurrent(QModelIndex current, QModelIndex previous)
    {
        statusBar().showMessage(
            "Moved from (" + previous.row() + ", " +previous.column() + " to "
            + "(" + current.row() + ", " +current.column() +")");
    }
    //! [3]

    QAbstractItemModel model;
    QItemSelectionModel selectionModel;
    QTableView table;
}
