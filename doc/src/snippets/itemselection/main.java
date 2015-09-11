import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;

import java.util.*;

public class main
{
    //static QAbstractTableModel model;

    public static void main(String args[])
    {
        QApplication.initialize(args);

    //! [0]
        TableModel model = new TableModel(8, 4, app);


        QTableView table = new QTableView();
        table.setModel(model);

        QItemSelectionModel selectionModel = table.selectionModel();
    //! [0] //! [1]
        QModelIndex topLeft;
        QModelIndex bottomRight;

        topLeft = model.index(0, 0, null);
        bottomRight = model.index(5, 2, null);
    //! [1]

    //! [2]
        QItemSelection selection = new QItemSelection(topLeft, bottomRight);
        selectionModel.select(selection, QItemSelectionModel.SelectionFlag.Select);
    //! [2]

    //! [3]
        QItemSelection toggleSelection = new QItemSelection();

        topLeft = model.index(2, 1, null);
        bottomRight = model.index(7, 3, null);
        toggleSelection.select(topLeft, bottomRight);

        selectionModel.select(toggleSelection, QItemSelectionModel.SelectionFlag.Toggle);
    //! [3]

    //! [4]
        QItemSelection columnSelection = new QItemSelection();

        topLeft = model.index(0, 1, null);
        bottomRight = model.index(0, 2, null);

        columnSelection.select(topLeft, bottomRight);

        selectionModel.select(columnSelection,
                              QItemSelectionModel.SelectionFlag.Select,
                              QItemSelectionModel.SelectionFlag.Columns);

        QItemSelection rowSelection = new QItemSelection();

        topLeft = model.index(0, 0, null);
        bottomRight = model.index(1, 0, null);

        rowSelection.select(topLeft, bottomRight);

        selectionModel.select(rowSelection,
            QItemSelectionModel.SelectionFlag.Select, QItemSelectionModel.SelectionFlag.Rows);
    //! [4]

        table.setWindowTitle("Selected items in a table model");
        table.show();
        table.resize(460, 280);
        QApplication.execStatic();
        QApplication.shutdown();
    }
}
