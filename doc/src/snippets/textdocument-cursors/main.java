import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;

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

        QTextEdit editor = new QTextEdit();

    //! [0]
        QTextDocument document = editor.document();
        QTextCursor redCursor = new QTextCursor(document);
    //! [0] //! [1]
        QTextCursor blueCursor = new QTextCursor(document);
    //! [1]

        QTextCharFormat redFormat = redCursor.charFormat();
        redFormat.setForeground(new QBrush(new QColor(Qt.GlobalColor.red)));
        QTextCharFormat blueFormat = blueCursor.charFormat();
        blueFormat.setForeground(new QBrush(new QColor(Qt.GlobalColor.blue)));

        redCursor.setCharFormat(redFormat);
        blueCursor.setCharFormat(blueFormat);

        for (int i = 0; i < 20; ++i) {
            if (i % 2 == 0)
                redCursor.insertText(String.valueOf(i), redFormat);
            if (i % 5 == 0)
                blueCursor.insertText(String.valueOf(i), blueFormat);
        }

        editor.setWindowTitle(tr("Text Document Cursors"));
        editor.resize(320, 480);
        editor.show();

        QApplication.execStatic();
        QApplication.shutdown();
    }
}
