import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class src_gui_itemviews_qstandarditemmodel {
    public static void main(String args[]) {
        QApplication.initialize(args);
    }
    public void snippet0(QStandardItem item) {
//! [0]
            QStandardItemModel model = new QStandardItemModel(4, 4);
            for (int row = 0; row < 4; ++row) {
                for (int column = 0; column < 4; ++column) {
                    QStandardItem tem = new QStandardItem("row " + row +", column " + column);
                    model.setItem(row, column, item);
                }
            }
//! [0]
    }

    public void snippet1() {
//! [1]
            QStandardItemModel model = new QStandardItemModel();
            QStandardItem parentItem = model.invisibleRootItem();
            for (int i = 0; i < 4; ++i) {
                QStandardItem item = new QStandardItem("item " + i);
                parentItem.appendRow(item);
                parentItem = item;
            }
//! [1]
    }

class MyWidget extends QWidget {
    public void snippet2(QStandardItemModel myStandardItemModel) {
//! [2]
        QTreeView treeView = new QTreeView(this);
        treeView.setModel(myStandardItemModel);
        treeView.clicked.connect(this, "clicked(QModelIndex)");
//! [2]
    }

    QStandardItemModel myStandardItemModel;
//! [3]
        protected void clicked(QModelIndex index)
        {
            QStandardItem tem = myStandardItemModel.itemFromIndex(index);
            // Do stuff with the item ...
        }
//! [3]

    public void snippet3(QTreeView treeView, QStandardItem item) {
//! [4]
        treeView.scrollTo(item.index());
//! [4]
    }
}
}
