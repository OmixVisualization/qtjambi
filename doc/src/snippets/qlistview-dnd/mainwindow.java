import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;

import java.util.*;

public class mainwindow extends QMainWindow
{

    public mainwindow()
    {
        QMenu fileMenu = new QMenu(tr("File"));

        QAction quitAction = fileMenu.addAction(tr("Exit"));
        quitAction.setShortcut(tr("Ctrl+Q"));

        menuBar().addMenu(fileMenu);

    //  For convenient quoting:
    //! [0]
    QListView listView = new QListView(this);
    listView.setSelectionMode(QAbstractItemView.SelectionMode.ExtendedSelection);
    listView.setDragEnabled(true);
    listView.setAcceptDrops(true);
    listView.setDropIndicatorShown(true);
    //! [0]

        this.listView = listView;

        quitAction.triggered.connect(this, "close()");

        setupListItems();

        setCentralWidget(listView);
        setWindowTitle(tr("List View"));
    }

    void setupListItems()
    {
        List<String> items;
        items.add("Oak");
        items.add("Fir");
        items.add("Pine");
        items.add("Birch");

        DragDropListModel model = new DragDropListModel(items, this);
        listView.setModel(model);
    }

    QListView listView;

    public static void main(String args[])
    {
        QApplication.initialize(args);

        new mainwindow().show();

        QApplication.execStatic();
        QApplication.shutdown();
    }
}
