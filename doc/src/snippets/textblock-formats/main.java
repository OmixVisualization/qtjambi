import org.qtjambi.qt.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.core.*;

import java.util.*;

public class main
{

    static String tr(String text)
    {
        return QApplication.translate(text, text);
    }

    public static void main(String args[])
    {
        QApplication.initialize(args);

    //! [0]
        QTextEdit editor = new QTextEdit();
        QTextCursor cursor = new QTextCursor(editor.textCursor());
    //! [0]
        cursor.movePosition(QTextCursor.MoveOperation.Start);

        QTextBlockFormat blockFormat = cursor.blockFormat();
        blockFormat.setTopMargin(4);
        blockFormat.setLeftMargin(4);
        blockFormat.setRightMargin(4);
        blockFormat.setBottomMargin(4);

        cursor.setBlockFormat(blockFormat);
        cursor.insertText(tr("This contains plain text inside a " +
                             "text block with margins to keep it separate " +
                             "from other parts of the document."));

        cursor.insertBlock();

    //! [1]
        QTextBlockFormat backgroundFormat = blockFormat;
        backgroundFormat.setBackground(new QBrush(new QColor("lightGray")));

        cursor.setBlockFormat(backgroundFormat);
    //! [1]
        cursor.insertText(tr("The background color of a text block can be " +
                             "changed to highlight text."));

        cursor.insertBlock();

        QTextBlockFormat rightAlignedFormat = blockFormat;
        rightAlignedFormat.setAlignment(Qt.AlignmentFlag.AlignRight);

        cursor.setBlockFormat(rightAlignedFormat);
        cursor.insertText(tr("The alignment of the text within a block is " +
                             "controlled by the alignment properties of " +
                             "the block itself. This text block is " +
                             "right-aligned."));

        cursor.insertBlock();

        QTextBlockFormat paragraphFormat = blockFormat;
        paragraphFormat.setAlignment(Qt.AlignmentFlag.AlignJustify);
        paragraphFormat.setTextIndent(32);

        cursor.setBlockFormat(paragraphFormat);
        cursor.insertText(tr("Text can be formatted so that the first " +
                             "line in a paragraph has its own margin. " +
                             "This makes the text more readable."));

        cursor.insertBlock();

        QTextBlockFormat reverseFormat = blockFormat;
        reverseFormat.setAlignment(Qt.AlignmentFlag.AlignJustify);
        reverseFormat.setTextIndent(32);

        cursor.setBlockFormat(reverseFormat);
        cursor.insertText(tr("The direction of the text can be reversed. " +
                             "This is useful for right-to-left " +
                             "languages."));

        editor.setWindowTitle(tr("Text Block Formats"));
        editor.resize(480, 480);
        editor.show();

        QApplication.execStatic();
        QApplication.shutdown();
    }
}
