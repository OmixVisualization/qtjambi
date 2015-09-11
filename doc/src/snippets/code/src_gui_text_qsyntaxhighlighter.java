import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;
import java.util.*;


public class src_gui_text_qsyntaxhighlighter  {
    public static void main(String args[]) {
        QApplication.initialize(args);
    }

class MyHighlighter extends QSyntaxHighlighter {
    MyHighlighter(QTextDocument document)
    {
        super(document);
    }

//! [1]
        protected void highlightBlock(String text)
        {
            QTextCharFormat myClassFormat = new QTextCharFormat();
            myClassFormat.setFontWeight(QFont.Weight.Bold.value());
            myClassFormat.setForeground(new QBrush(QColor.darkMagenta));
            String pattern = "\\bMy[A-Za-z]+\\b";

            QRegExp expression = new QRegExp(pattern);
            int index = expression.indexIn(text);
            while (index >= 0) {
                int length = expression.matchedLength();
                setFormat(index, length, myClassFormat);
                index = expression.indexIn(text, index + length);
             }
         }
//! [1]

    public void snippet2(String text) {
//! [2]
        QTextCharFormat multiLineCommentFormat = new QTextCharFormat();
        multiLineCommentFormat.setForeground(new QBrush(QColor.red));

        QRegExp startExpression = new QRegExp("/\\*");
        QRegExp endExpression = new QRegExp("\\* /");

        setCurrentBlockState(0);

        int startIndex = 0;
        if (previousBlockState() != 1)
            startIndex = startExpression.indexIn(text);

        while (startIndex >= 0) {
           int endIndex = endExpression.indexIn(text, startIndex);
           int commentLength;
           if (endIndex == -1) {
               setCurrentBlockState(1);
               commentLength = text.length() - startIndex;
           } else {
               commentLength = endIndex - startIndex
                               + endExpression.matchedLength();
           }
           setFormat(startIndex, commentLength, multiLineCommentFormat);
           startIndex = startExpression.indexIn(text,
                                     startIndex + commentLength);
        }
//! [2]
    }
}

class MyHighlighter2 extends QSyntaxHighlighter {
    MyHighlighter2(QTextDocument document)
    {
        super(document);
    }
//! [3]
        protected void highlightBlock(String text)
        {
            QTextCharFormat myClassFormat = new QTextCharFormat();
            myClassFormat.setFontWeight(QFont.Weight.Bold.value());
            myClassFormat.setForeground(new QBrush(QColor.darkMagenta));
            String pattern = "\\bMy[A-Za-z]+\\b";

            QRegExp expression = new QRegExp(pattern);
            int index = expression.indexIn(text);
            while (index >= 0) {
                int length = expression.matchedLength();
                setFormat(index, length, myClassFormat);
                index = expression.indexIn(text, index + length);
             }
         }
//! [3]


//! [4]
        class ParenthesisInfo
        {
            char character;
            int position;
        }

        class BlockData extends QTextBlockUserData
        {
            List<ParenthesisInfo> parentheses;
        }
//! [4]


    public void snippet0() {
//! [0]
           QTextEdit editor = new QTextEdit();
           MyHighlighter highlighter = new MyHighlighter(editor.document());
//! [0]
   }
}
}

