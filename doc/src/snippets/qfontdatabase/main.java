import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;

import java.util.*;

public class main
{
    public static void main(String args[])
    {
        QApplication.initialize(args);

    //! [0]
        QFontDatabase database = new QFontDatabase();
        QTreeWidget fontTree = new QTreeWidget();
        fontTree.setColumnCount(2);
        List<String> headers = new Vector<String>();
        headers.add("Font");
        headers.add("Smooth Sizes");

        fontTree.setHeaderLabels(headers);

        for (String family : database.families()) {
            QTreeWidgetItem familyItem = new QTreeWidgetItem(fontTree);
            familyItem.setText(0, family);

            for (String style : database.styles(family)) {
                QTreeWidgetItem styleItem = new QTreeWidgetItem(familyItem);
                styleItem.setText(0, style);

                String sizes = "";
                for (int points : database.smoothSizes(family, style))
                    sizes += String.valueOf(points) + " ";

                styleItem.setText(1, sizes.trim());
            }
    //! [0] //! [1]
        }
    //! [1]
        fontTree.resizeColumnToContents(0);
        fontTree.show();

        QApplication.execStatic();
        QApplication.shutdown();
    }
}
