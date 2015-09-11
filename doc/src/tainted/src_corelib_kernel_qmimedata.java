/*   Ported from: src.corelib.kernel.qmimedata.cpp
<snip>
//! [0]
        void MyWidget::dragEnterEvent(QDragEnterEvent *event)
        {
            if (event->mimeData()->hasUrls())
                event->acceptProposedAction();
        }

        void MyWidget::dropEvent(QDropEvent *event)
        {
            if (event->mimeData()->hasUrls()) {
                foreach (QUrl url, event->mimeData()->urls()) {
                    ...
                }
            }
        }
//! [0]


//! [1]
            QByteArray csvData = ...;

            QMimeData *mimeData = new QMimeData;
            mimeData->setData("text/csv", csvData);
//! [1]


//! [2]
        void MyWidget::dropEvent(QDropEvent *event)
        {
            const MyMimeData *myData =
                    qobject_cast<const MyMimeData *>(event->mimeData());
            if (myData) {
                // access myData's data directly (not through QMimeData's API)
            }
        }
//! [2]


//! [3]
    application/x-qt-windows-mime;value="<custom type>"
//! [3]


//! [4]
    application/x-qt-windows-mime;value="FileGroupDescriptor"
    application/x-qt-windows-mime;value="FileContents"
//! [4]


//! [5]
        if (event->mimeData()->hasImage()) {
            QImage image = qvariant_cast<QImage>(event->mimeData()->imageData());
            ...
        }
//! [5]


//! [6]
        mimeData->setImageData(QImage("beautifulfjord.png"));
//! [6]


//! [7]
        if (event->mimeData()->hasColor()) {
            QColor color = qvariant_cast<QColor>(event->mimeData()->colorData());
            ...
        }
//! [7]


</snip>
*/
import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class src_corelib_kernel_qmimedata {
    public static void main(String args[]) {
        QApplication.initialize(args);
//! [0]
        void MyWidget.dragEnterEvent(QDragEnterEvent vent)
        {
            if (event.mimeData().hasUrls())
                event.acceptProposedAction();
        }

        void MyWidget.dropEvent(QDropEvent vent)
        {
            if (event.mimeData().hasUrls()) {
                for (QUrl url, event.mimeData().urls()) {
                    ...
                }
            }
        }
//! [0]


//! [1]
            QByteArray csvData = ...;

            QMimeData imeData = new QMimeData;
            mimeData.setData("text/csv", csvData);
//! [1]


//! [2]
        void MyWidget.dropEvent(QDropEvent vent)
        {
            MyMimeData yData =
                    qobject_cast<MyMimeData *>(event.mimeData());
            if (myData) {
                // access myData's data directly (not through QMimeData's API)
            }
        }
//! [2]


//! [3]
    application/x-qt-windows-mime;value="<custom type>"
//! [3]


//! [4]
    application/x-qt-windows-mime;value="FileGroupDescriptor"
    application/x-qt-windows-mime;value="FileContents"
//! [4]


//! [5]
        if (event.mimeData().hasImage()) {
            QImage image = qvariant_cast<QImage>(event.mimeData().imageData());
            ...
        }
//! [5]


//! [6]
        mimeData.setImageData(QImage("beautifulfjord.png"));
//! [6]


//! [7]
        if (event.mimeData().hasColor()) {
            QColor color = qvariant_cast<QColor>(event.mimeData().colorData());
            ...
        }
//! [7]


    }
}
