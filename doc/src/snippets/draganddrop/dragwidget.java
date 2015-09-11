import org.qtjambi.qt.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.core.*;

import java.util.*;

public class dragwidget extends QFrame
{
    QByteArray data = new QByteArray();
    QLabel dragDropLabel;
    QPoint dragStartPosition = new QPoint();
    String mimeType = "";

    Signal1<String> dragResult = new Signal1<String>();
    Signal1<List<String>> mimeTypes = new Signal1<List<String>>();

    public dragwidget(QWidget parent)
    {
        super(parent);

        setFrameStyle(QFrame.Shape.StyledPanel.value() | QFrame.Shadow.Sunken.value());
        dragDropLabel = new QLabel("", this);
        dragDropLabel.setAlignment(new Qt.Alignment(Qt.AlignmentFlag.AlignHCenter));

        QHBoxLayout layout = new QHBoxLayout(this);
        layout.addStretch(0);
        layout.addWidget(dragDropLabel);
        layout.addStretch(0);

        setAcceptDrops(true);
    }

    // Accept all actions, but deal with them separately later.
    //! [0]
    protected void dragEnterEvent(QDragEnterEvent event)
    {
        event.acceptProposedAction();
    }
    //! [0]

    //! [1]
    protected void dropEvent(QDropEvent event)
    {
        if (event.source() == this &&
            event.possibleActions().isSet(Qt.DropAction.MoveAction))
            return;
    //! [1]

    //! [2]
        if (event.proposedAction().equals(Qt.DropAction.MoveAction)) {
            event.acceptProposedAction();
            // Process the data from the event.
    //! [2]
            dragResult.emit(tr("The data was moved here."));
    //! [3]
        } else if (event.proposedAction().equals(Qt.DropAction.CopyAction)) {
            event.acceptProposedAction();
            // Process the data from the event.
    //! [3]
            dragResult.emit(tr("The data was copied here."));
    //! [4]
        } else {
            // Ignore the drop.
            return;
        }
    //! [4]
        // End of quote

        mimeTypes.emit(event.mimeData().formats());
        setData(event.mimeData().formats().get(0),
                event.mimeData().data(event.mimeData().formats().get(0)));
    //! [5]
    }
    //! [5]

    //! [6]
    protected void mousePressEvent(QMouseEvent event)
    {
        if (event.button().equals(Qt.MouseButton.LeftButton))
            dragStartPosition = event.pos();
    }
    //! [6]

    //! [7]
    protected void mouseMoveEvent(QMouseEvent event)
    {
        if (!(event.buttons().isSet(Qt.MouseButton.LeftButton)))
            return;
        if ((event.pos().subtract(dragStartPosition)).manhattanLength()
             < QApplication.startDragDistance())
            return;

        QDrag drag = new QDrag(this);
        QMimeData mimeData = new QMimeData();

        mimeData.setData(mimeType, data);
        drag.setMimeData(mimeData);

        Qt.DropAction dropAction = drag.exec(Qt.DropAction.CopyAction,
                                             Qt.DropAction.MoveAction);
    //! [7]

        switch (dropAction) {
            case CopyAction:
                dragResult.emit(tr("The text was copied."));
                break;
            case MoveAction:
                dragResult.emit(tr("The text was moved."));
                break;
            default:
                dragResult.emit(tr("Unknown action."));
                break;
        }
    //! [8]
    }
    //! [8]

    void setData(String mimetype, QByteArray newData)
    {
        mimeType = mimetype;
        data = new QByteArray(newData);

        dragDropLabel.setText(String.valueOf(data.size()) + " bytes.");

        List<String> formats = new Vector<String>();
        formats.add(mimetype);
        mimeTypes.emit(formats);
    }

}
