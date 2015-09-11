import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;

import java.util.*;

public class main
{

static class MyItemModel extends QStandardItemModel
{

public MyItemModel(QWidget parent)
{ super(parent); }

}

static class Widget extends QWidget
{

public Widget(QWidget parent)
{
    super(parent);
//! [0] //! [1]
        QTreeView treeView = new QTreeView();
//! [0]
        MyItemModel model = new MyItemModel(this);

        treeView.setModel(model);
//! [1]

//! [2]
        MyItemModel sourceModel = new MyItemModel(this);
        QSortFilterProxyModel proxyModel = new QSortFilterProxyModel(this);

        proxyModel.setSourceModel(sourceModel);
        treeView.setModel(proxyModel);
//! [2]

//! [3]
        treeView.setSortingEnabled(true);
//! [3]

//! [4]
        proxyModel.sort(2, Qt.SortOrder.AscendingOrder);
//! [4] //! [5]
        proxyModel.setFilterRegExp(new QRegExp(".png", Qt.CaseSensitivity.CaseInsensitive,
                                            QRegExp.PatternSyntax.FixedString));
        proxyModel.setFilterKeyColumn(1);
//! [5]
}

}

public static void main(String args[])
{
    QApplication.initialize(args);
    Widget widget = new Widget(null);
    widget.show();
    QApplication.execStatic();
    QApplication.shutdown();
}

}
