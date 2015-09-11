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

        QTextCharFormat headingFormat = cursor.charFormat();
        headingFormat.setFontWeight(QFont.Weight.Bold.value());
        headingFormat.setFontPointSize(16);

        QTextCharFormat emphasisFormat = cursor.charFormat();
        emphasisFormat.setFontItalic(true);

        QTextCharFormat qtFormat = cursor.charFormat();
        qtFormat.setForeground(new QBrush(new QColor("#990000")));

        QTextCharFormat underlineFormat = cursor.charFormat();
        underlineFormat.setFontUnderline(true);

    //! [0]
        cursor.insertText(tr("Character formats"),
                          headingFormat);

        cursor.insertBlock();

        cursor.insertText(tr("Text can be displayed in a variety of " +
                             "different character formats. "), plainFormat);
        cursor.insertText(tr("We can emphasize text by "));
        cursor.insertText(tr("making it italic"), emphasisFormat);
    //! [0]
        cursor.insertText(tr(", give it a "), plainFormat);
        cursor.insertText(tr("different color "), qtFormat);
        cursor.insertText(tr("to the default text color, "), plainFormat);
        cursor.insertText(tr("underline it"), underlineFormat);
        cursor.insertText(tr(", and use many other effects."), plainFormat);

        editor.setWindowTitle(tr("Text Document Character Formats"));
        editor.resize(320, 480);
        editor.show();

        QApplication.execStatic();
        QApplication.shutdown();
    }
}
