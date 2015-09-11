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

        QTextDocument document = new QTextDocument(editor);
        QTextCursor cursor = new QTextCursor(document);

        QTextImageFormat imageFormat = new QTextImageFormat();
        imageFormat.setName("classpath:images/advert.png");
        cursor.insertImage(imageFormat);

        QTextBlock block = cursor.block();
        QTextFragment fragment = new QTextFragment();
        QTextBlock_iterator it;

        for (it = block.begin(); !(it.atEnd()); it.next()) {
            fragment = it.fragment();

            if (fragment.contains(cursor.position()))
                break;
        }

    //! [0]
        if (fragment.isValid()) {
            QTextImageFormat newImageFormat = fragment.charFormat().toImageFormat();

            if (newImageFormat.isValid()) {
                newImageFormat.setName("classpath:images/newimage.png");
                QTextCursor helper = new QTextCursor(cursor);

                helper.setPosition(fragment.position());
                helper.setPosition(fragment.position() + fragment.length(),
                                   QTextCursor.MoveMode.KeepAnchor);
                helper.setCharFormat(newImageFormat);
    //! [0] //! [1]
            }
    //! [1] //! [2]
        }
    //! [2]

        cursor.insertBlock();
        cursor.insertText("Code less. Create more.");

        editor.setDocument(document);
        editor.setWindowTitle(tr("Text Document Image Format"));
        editor.resize(320, 480);
        editor.show();

        QApplication.execStatic();
        QApplication.shutdown();
    }
}
