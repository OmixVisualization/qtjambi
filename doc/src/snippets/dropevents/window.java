import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;

import java.util.*;

public class window extends QWidget
{

//! [0]
public window(QWidget parent)
{
    super(parent);
//! [0]
    QLabel textLabel = new QLabel(tr("Data:"), this);
    textBrowser = new QTextBrowser(this);

    QLabel mimeTypeLabel = new QLabel(tr("MIME types:"), this);
    mimeTypeCombo = new QComboBox(this);

    QVBoxLayout layout = new QVBoxLayout(this);
    layout.addWidget(textLabel);
    layout.addWidget(textBrowser);
    layout.addWidget(mimeTypeLabel);
    layout.addWidget(mimeTypeCombo);

//! [1]
    setAcceptDrops(true);
//! [1]
    setWindowTitle(tr("Drop Events"));
//! [2]
}
//! [2]

//! [3]
protected void dragEnterEvent(QDragEnterEvent event)
{
    if (event.mimeData().hasFormat("text/plain"))
        event.acceptProposedAction();
}
//! [3]

//! [4]
protected void dropEvent(QDropEvent event)
{
    textBrowser.setPlainText(event.mimeData().text());
    mimeTypeCombo.clear();
    mimeTypeCombo.addItems(event.mimeData().formats());

    event.acceptProposedAction();
}
//! [4]

    QComboBox mimeTypeCombo;
    QFrame dropFrame;
    QTextBrowser textBrowser;
    String oldText;
    List<String> oldMimeTypes;
}
