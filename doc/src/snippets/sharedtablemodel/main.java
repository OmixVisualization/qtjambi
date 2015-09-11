import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;

import java.util.*;

public class main
{

    // I just want this to compile
    static class TableModel extends QAbstractTableModel
    {
        public TableModel(int i, int j, QApplication app) {}

        @Override
        public QModelIndex index(int i, int j, QModelIndex index)
        {
            return null;
        }

        public Object data(QModelIndex index, int role) { return null; }
        public int columnCount(QModelIndex parent) { return 0; }
        public int rowCount(QModelIndex parent) { return 0; }
    }

    public static void main(String args[])
    {
        QApplication.initialize(args);

        TableModel model = new TableModel(4, 2, QApplication.instance());

    //! [0]
        QTableView firstTableView = new QTableView();
        QTableView secondTableView = new QTableView();
    //! [0]

    //! [1]
        firstTableView.setModel(model);
        secondTableView.setModel(model);
    //! [1]

        firstTableView.horizontalHeader().setModel(model);

        for (int row = 0; row < 4; ++row) {
            for (int column = 0; column < 2; ++column) {
                QModelIndex index = model.index(row, column, null);
                model.setData(index, "(" + row + ", " + column +  ")");
            }
        }

    //! [2]
        secondTableView.setSelectionModel(firstTableView.selectionModel());
    //! [2]

        firstTableView.setWindowTitle("First table view");
        secondTableView.setWindowTitle("Second table view");
        firstTableView.show();
        secondTableView.show();

        QApplication.execStatic();
        QApplication.shutdown();
    }
}
