
import org.qtjambi.qt.gui.*;

import java.util.*;


public class src_gui_itemviews_qtreewidget {
    public static void main(String args[]) {
        QApplication.initialize(args);
//! [0]
        QTreeWidget treeWidget = new QTreeWidget();
        treeWidget.setColumnCount(1);
        List<QTreeWidgetItem> items = new ArrayList<QTreeWidgetItem>();
        for (int i = 0; i < 10; ++i) {
            List<String> lst = new ArrayList<String>();
            lst.add("item: " + i);
            items.add(new QTreeWidgetItem((QTreeWidgetItem) null, lst));
        }
        treeWidget.insertTopLevelItems(0, items);
//! [0]


    }
}
