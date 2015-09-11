import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.core.*;

public class src_gui_itemviews_qitemselectionmodel {
    public static void main(String args[]) {
        QApplication.initialize(args);
        QModelIndex topLeft = null;
        QModelIndex bottomRight = null;
        {
//! [0]
    QItemSelection selection = new QItemSelection(topLeft, bottomRight);
//! [0]
        }

        {
//! [1]
    QItemSelection selection = new QItemSelection();
    // ...
    selection.select(topLeft, bottomRight);
//! [1]
        }


    }
}
