import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;

public class main
{

    static String tr(String text)
    {
        return QApplication.translate(text, text);
    }

    public static void main(String args[])
    {
        QApplication.initialize(args);

        QTextEdit editor = new QTextEdit();

    //! [0]
        QTextDocument document = new QTextDocument(editor);
        QTextCursor cursor = new QTextCursor(document);
    //! [0]

    //! [1]
        QTextImageFormat imageFormat = new QTextImageFormat();
        imageFormat.setName("classpath:images/advert.png");
        cursor.insertImage(imageFormat);
    //! [1]

        cursor.insertBlock();
        cursor.insertText("Code less. Create more.");

        editor.setDocument(document);
        editor.setWindowTitle(tr("Text Document Images"));
        editor.resize(320, 480);
        editor.show();

        QApplication.execStatic();
        QApplication.shutdown();
    }

}
