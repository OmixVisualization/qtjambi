import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;

import java.util.*;

public class window extends QWidget
{
    QComboBox mimeTypeCombo;
    QFrame dropFrame;
    QLabel dataLabel;
    String oldText;
    List<String> oldMimeTypes = new Vector<String>();

    public window()
    {
        QLabel textLabel = new QLabel(tr("Data:"), this);
        dataLabel = new QLabel(this);
        dataLabel.setFixedSize(200, 200);

        QVBoxLayout layout = new QVBoxLayout(this);
        layout.addWidget(textLabel);
        layout.addWidget(dataLabel);

        setAcceptDrops(true);
        setWindowTitle(tr("Drop Events"));
    }

    //! [0]
    protected void dragEnterEvent(QDragEnterEvent event)
    {
        if (event.mimeData().hasText() || event.mimeData().hasImage())
            event.acceptProposedAction();
    }
    //! [0]

    //! [1]
    protected void dropEvent(QDropEvent event)
    {
        if (event.mimeData().hasText())
            dataLabel.setText(event.mimeData().text());
        else if (event.mimeData().hasImage()) {
            Object imageData = event.mimeData().imageData();
            dataLabel.setPixmap((QPixmap) imageData);
        }
        event.acceptProposedAction();
    }
    //! [1]

    //! [2]
    protected void mousePressEvent(QMouseEvent event)
    {
    //! [2]
        String text = dataLabel.text();
        QPixmap iconPixmap = new QPixmap(32, 32);
        iconPixmap.fill(new QColor(255, 0, 0, 127));
        QImage image = new QImage(100, 100, QImage.Format.Format_RGB32);
        image.fill(new QColor(0, 0, 255).rgb());

    //! [3]
        if (event.button().equals(Qt.MouseButton.LeftButton)) {
            QDrag drag = new QDrag(this);
            QMimeData mimeData = new QMimeData();

            mimeData.setText(text);
            mimeData.setImageData(image);
            drag.setMimeData(mimeData);
            drag.setPixmap(iconPixmap);

            Qt.DropAction dropAction = drag.exec();
    //! [3]
            // ...
    //! [4]
            event.accept();
        }
    //! [4]
        else if (event.button().equals(Qt.MouseButton.MidButton)) {

            QDrag drag = new QDrag(this);
            QMimeData mimeData = new QMimeData();

            mimeData.setImageData(image);
            drag.setMimeData(mimeData);
            drag.setPixmap(iconPixmap);

            Qt.DropAction dropAction = drag.exec();
            // ...
            event.accept();
        }
    //! [5]
    }
    //! [5]
}
