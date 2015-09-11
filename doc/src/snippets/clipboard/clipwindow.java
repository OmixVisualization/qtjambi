import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;

import java.util.*;

public class clipwindow extends QMainWindow
{
    int currentItem;
    QClipboard clipboard;
    QComboBox mimeTypeCombo;
    QLabel dataInfoLabel;
    QListWidget previousItems;

    public clipwindow()
    {
        clipboard = QApplication.clipboard();

        QWidget centralWidget = new QWidget(this);
        QWidget currentItem = new QWidget(centralWidget);
        QLabel mimeTypeLabel = new QLabel(tr("MIME types:"), currentItem);
        mimeTypeCombo = new QComboBox(currentItem);
        QLabel dataLabel = new QLabel(tr("Data:"), currentItem);
        dataInfoLabel = new QLabel("", currentItem);

        previousItems = new QListWidget(centralWidget);

    //! [0]
        clipboard.dataChanged.connect(this, "updateClipboard()");
    //! [0]
        mimeTypeCombo.activated.connect(this, "updateData(String)");

        QVBoxLayout currentLayout = new QVBoxLayout(currentItem);
        currentLayout.addWidget(mimeTypeLabel);
        currentLayout.addWidget(mimeTypeCombo);
        currentLayout.addWidget(dataLabel);
        currentLayout.addWidget(dataInfoLabel);
        currentLayout.addStretch(1);

        QHBoxLayout mainLayout = new QHBoxLayout(centralWidget);
        mainLayout.addWidget(currentItem, 1);
        mainLayout.addWidget(previousItems);

        setCentralWidget(centralWidget);
        setWindowTitle(tr("Clipboard"));
    }

    String format = "";
    //! [1]
    public void updateClipboard()
    {
        List<String> formats = clipboard.mimeData().formats();
        QByteArray data = clipboard.mimeData().data(format);
    //! [1]

        mimeTypeCombo.clear();
        mimeTypeCombo.addItems(formats);

        int size = clipboard.mimeData().data(formats.get(0)).size();
        QListWidgetItem newItem = new QListWidgetItem(previousItems);
        newItem.setText(formats.get(0) + " (" + size + " bytes)");

        updateData(formats.get(0));
    //! [2]
    }

    public void updateData(String format)
    {
        QByteArray data = clipboard.mimeData().data(format);
        dataInfoLabel.setText(String.valueOf(data.size()) + " bytes");
    }
}
