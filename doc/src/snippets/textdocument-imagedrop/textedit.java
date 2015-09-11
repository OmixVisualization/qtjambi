import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;

public class textedit extends QTextEdit
{

    public textedit(QWidget parent)
    {
        super(parent);
    }

    //! [0]
    public boolean canInsertFromMimeData(QMimeData source)
    {
        if (source.hasImage())
            return true;
        else
            return super.canInsertFromMimeData(source);
    }
    //! [0]

    //! [1]
    public void insertFromMimeData(QMimeData source)
    {
        if (source.hasImage())
        {
            QImage image = (QImage) source.imageData();
            QTextCursor cursor = this.textCursor();
            QTextDocument document = this.document();
            document.addResource(QTextDocument.ResourceType.ImageResource.value(),
                                 new QUrl("image"), image);
            cursor.insertImage("image");
        }
    }
    //! [1]

    public static void main(String args[])
    {
        QApplication.initialize(args);

        new textedit(null).show();

        QApplication.execStatic();
        QApplication.shutdown();
    }
}
