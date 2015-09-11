import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;

import java.util.*;

public class main
{
    public static void main(String args[])
    {
        QApplication.initialize(args);
        QObject parent = QApplication.instance();

        List<String> numbers = new Vector<String>();
        numbers.add("One");
        numbers.add("Two");
        numbers.add("Three");
        numbers.add("Four");
        numbers.add("Five");

        QAbstractItemModel stringListModel = new QStringListModel(numbers, parent);

    //! [0]
        QSortFilterProxyModel filterModel = new QSortFilterProxyModel(parent);
        filterModel.setSourceModel(stringListModel);
    //! [0]

        QWidget window = new QWidget();

    //! [1]
        QListView filteredView = new QListView();
        filteredView.setModel(filterModel);
    //! [1]
        filteredView.setWindowTitle("Filtered view onto a string list model");

        QLineEdit patternEditor = new QLineEdit();
        patternEditor.textChanged.connect(filterModel, "setFilterModel(String)");

        QVBoxLayout layout = new QVBoxLayout(window);
        layout.addWidget(filteredView);
        layout.addWidget(patternEditor);

        window.show();

        QApplication.execStatic();
        QApplication.shutdown();
    }
}
