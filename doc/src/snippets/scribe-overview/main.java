import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import com.trolletch.qt.gui.*;

import java.util.*;

public class main
{
    void mergeFormat(QTextEdit edit)
    {
    //! [0]
        QTextDocument document = edit.document();
        QTextCursor cursor = new QTextCursor(document);

        cursor.movePosition(QTextCursor.CursorPosition.Start);
        cursor.movePosition(QTextCursor.CursorPosition.EndOfLine,
                            QTextCursor.CursorPosition.KeepAnchor);

        QTextCharFormat format = new QTextCharFormat();
        format.setFontWeight(QFont.Weight.Bold.value());

        cursor.mergeCharFormat(format);
    //! [0]
    }

    public static void main(String args[])
    {
        QApplication.initialize(args);

        QWidget parent = null;
        String aStringContainingHTMLtext =
            "<h1>Scribe Overview</h1>";

    //! [1]
        QTextEdit editor = new QTextEdit(parent);
        editor.setHtml(aStringContainingHTMLtext);
        editor.show();
    //! [1]

        QApplication.execStatic();
        QApplication.shutdown();
    }
}
