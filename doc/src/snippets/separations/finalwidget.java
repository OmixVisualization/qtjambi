import org.qtjambi.qt.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.core.*;

import java.util.*;

public class finalwidget extends QFrame
{
    boolean hasImage;
    QImage originalImage = new QImage();
    QLabel imageLabel;
    QLabel nameLabel;
    QPoint dragStartPosition = new QPoint();

    public finalwidget(QWidget parent, String name,
                       QSize labelSize)
    {
        super(parent);

        hasImage = false;
        imageLabel = new QLabel();
        imageLabel.setFrameShadow(QFrame.Shadow.Sunken);
        imageLabel.setFrameShape(QFrame.Shape.StyledPanel);
        imageLabel.setMinimumSize(labelSize);
        nameLabel = new QLabel(name);

        QVBoxLayout layout = new QVBoxLayout();
        layout.addWidget(imageLabel, 1);
        layout.addWidget(nameLabel, 0);
        setLayout(layout);
    }

    protected void mouseMoveEvent(QMouseEvent event)
    {
        if (!(event.buttons().isSet(Qt.MouseButton.LeftButton)))
            return;

        if ((event.pos().subtract(dragStartPosition)).manhattanLength()
             < QApplication.startDragDistance())
            return;

        if (!hasImage)
            return;

        QDrag drag = new QDrag(this);
        QMimeData mimeData = new QMimeData();

    //! [0]
        QByteArray output = new QByteArray();
        QBuffer outputBuffer = new QBuffer(output);
        outputBuffer.open(QIODevice.OpenModeFlag.WriteOnly);
        imageLabel.pixmap().toImage().save(outputBuffer, "PNG");
        mimeData.setData("image/png", output);
    //! [0]

    //! [1]
        mimeData.setImageData(imageLabel.pixmap());
    //! [1]

        drag.setMimeData(mimeData);
        drag.setPixmap(imageLabel.pixmap().scaled(64, 64, Qt.AspectRatioMode.KeepAspectRatio));
    //! [2]
        drag.setHotSpot(new QPoint(drag.pixmap().width()/2,
                                   drag.pixmap().height()));
    //! [2]

        drag.exec();
    }

    protected void mousePressEvent(QMouseEvent event)
    {
        if (event.button().equals(Qt.MouseButton.LeftButton))
            dragStartPosition = event.pos();
    }

    public QPixmap pixmap()
    {
        return imageLabel.pixmap();
    }

    public void setPixmap(QPixmap pixmap)
    {
        imageLabel.setPixmap(pixmap);
        hasImage = true;
    }
}
