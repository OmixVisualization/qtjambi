import org.qtjambi.qt.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.core.*;

import java.util.*;

public class splitter extends QWidget
{

    static void processSize(int i)
    {
    }

    public static void main(String args[])
    {
        QWidget parent = null;

    //! [0]
        QSplitter splitter = new QSplitter(parent);
        QListView listview = new QListView();
        QTreeView treeview = new QTreeView();
        QTextEdit textedit = new QTextEdit();
        splitter.addWidget(listview);
        splitter.addWidget(treeview);
        splitter.addWidget(textedit);
    //! [0]

        if (true)
        {
        // SAVE STATE
    //! [1]
        QSettings settings = new QSettings();
        settings.setValue("splitterSizes", splitter.saveState());
    //! [1]
        }

        if (true)
        {
        // RESTORE STATE
    //! [2]
        QSettings settings = new QSettings();
        splitter.restoreState((QByteArray) settings.value("splitterSizes"));
    //! [2]
        }

    //! [3]
        List<Integer> sizes = splitter.sizes();
        for (Integer size : sizes)
            processSize(size);
    //! [3]
    }

}
