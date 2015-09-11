
import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;

/*!
    Create a default directory model and, using the index-based interface to
    the model and some QLabel widgets, populate the window's layout with the
    names of objects in the directory.

    Note that we only want to read the filenames in the highest level of the
    directory, so we supply a default (invalid) QModelIndex to the model in
    order to indicate that we want top-level items.
*/

public class main
{

    public static void main(String args[])
    {
        QApplication.initialize(args);

        QWidget window = new QWidget();
        QVBoxLayout layout = new QVBoxLayout(window);
        QLabel title = new QLabel("Some items from the directory model", window);
        title.setBackgroundRole(QPalette.ColorRole.Base);
        title.setMargin(8);
        layout.addWidget(title);

    //! [0]
        QDirModel model = new QDirModel();
        QModelIndex parentIndex = model.index(QDir.currentPath());
        int numRows = model.rowCount(parentIndex);
    //! [0]

    //! [1]
        for (int row = 0; row < numRows; ++row) {
            QModelIndex index = model.index(row, 0, parentIndex);
    //! [1]

    //! [2]
            String text = model.data(index, Qt.ItemDataRole.DisplayRole).toString();
            // Display the text in a widget.
    //! [2]

            QLabel label = new QLabel(text, window);
            layout.addWidget(label);
    //! [3]
        }
    //! [3]

        window.setWindowTitle("A simple model example");
        window.show();

        QApplication.execStatic();
        QApplication.shutdown();
    }
}
