import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;

import java.util.*;

public class mainwindow extends QMainWindow
{
    QLabel iconLabel;
    QLineEdit nameEdit;
    QPixmap iconPixmap = new QPixmap();
    QPoint dragStartPosition = new QPoint();
    QTextEdit commentEdit;

    public mainwindow()
    {
        QFrame centralFrame = new QFrame(this);

        QLabel nameLabel = new QLabel(tr("Comment:"), centralFrame);
        commentEdit = new QTextEdit(centralFrame);
        QLabel dragLabel = new QLabel(tr("<p>Drag the icon to a filer " +
                                          "window or the desktop background:</p>"),
                                          centralFrame);
        iconLabel = new QLabel(centralFrame);
        iconPixmap.load("classpath:/images/file.png");
        iconLabel.setPixmap(iconPixmap);

        QGridLayout grid = new QGridLayout(centralFrame);
        grid.addWidget(nameLabel, 0, 0);
        grid.addWidget(commentEdit, 1, 0, 1, 2);
        grid.addWidget(dragLabel, 2, 0);
        grid.addWidget(iconLabel, 2, 1);

        statusBar();
        setCentralWidget(centralFrame);
        setWindowTitle(tr("Dragging"));
    }

    //! [0]
    protected void mousePressEvent(QMouseEvent event)
    {
        if (event.button().equals(Qt.MouseButton.LeftButton)
            && iconLabel.geometry().contains(event.pos())) {

    //! [1]
            QDrag drag = new QDrag(this);
            QMimeData mimeData = new QMimeData();

            mimeData.setText(commentEdit.toPlainText());
            drag.setMimeData(mimeData);
    //! [1]
            drag.setPixmap(iconPixmap);

            Qt.DropAction dropAction = drag.exec();
    //! [0]

            String actionText = "";
            switch (dropAction) {
                case CopyAction:
                    actionText = tr("The text was copied.");
                    break;
                case MoveAction:
                    actionText = tr("The text was moved.");
                    break;
                case LinkAction:
                    actionText = tr("The text was linked.");
                    break;
                case IgnoreAction:
                    actionText = tr("The drag was ignored.");
                    break;
                default:
                    actionText = tr("Unknown action.");
                    break;
            }
            statusBar().showMessage(actionText);
    //! [2]
        }
    }
    //! [2]
}
