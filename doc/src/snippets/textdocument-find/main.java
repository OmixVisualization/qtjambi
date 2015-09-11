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

        QTextCursor cursor = new QTextCursor(editor.textCursor());
        cursor.movePosition(QTextCursor.MoveOperation.Start);

        QTextCharFormat plainFormat = cursor.charFormat();
        QTextCharFormat colorFormat = cursor.charFormat();
        colorFormat.setForeground(new QBrush(new QColor(Qt.GlobalColor.red)));

        cursor.insertText(tr("Text can be displayed in a variety of " +
                             "different character " +
                             "formats. "), plainFormat);
        cursor.insertText(tr("We can emphasize text by making it "));
        cursor.insertText(tr("italic, give it a different color "));
        cursor.insertText(tr("to the default text color, underline it, "));
        cursor.insertText(tr("and use many other effects."));

        String searchString = tr("text");

        QTextDocument document = editor.document();
    //! [0]
        QTextCursor newCursor = new QTextCursor(document);

        while (!newCursor.isNull() && !newCursor.atEnd()) {
            newCursor = document.find(searchString, newCursor);

            if (!newCursor.isNull()) {
                newCursor.movePosition(QTextCursor.MoveOperation.WordRight,
                                       QTextCursor.MoveMode.KeepAnchor);

                newCursor.mergeCharFormat(colorFormat);
            }
    //! [0] //! [1]
        }
    //! [1]

        editor.setWindowTitle(tr("Text Document Find"));
        editor.resize(320, 480);
        editor.show();

        QApplication.execStatic();
        QApplication.shutdown();
    }
}
