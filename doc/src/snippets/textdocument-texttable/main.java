import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;

public class main
{
    public static void main(String args[])
    {
        int rows = 6;
        int columns = 2;

        QApplication.initialize(args);
        QTextEdit textEdit = new QTextEdit();
        QTextCursor cursor = new QTextCursor(textEdit.textCursor());
        cursor.movePosition(QTextCursor.MoveOperation.Start);

        QTextTableFormat tableFormat = new QTextTableFormat();
        tableFormat.setAlignment(
            new Qt.Alignment(Qt.AlignmentFlag.AlignHCenter));
        tableFormat.setCellPadding(2);
        tableFormat.setCellSpacing(2);
        QTextTable table = cursor.insertTable(rows, columns);
        table.setFormat(tableFormat);

        QTextCharFormat boldFormat = new QTextCharFormat();
        boldFormat.setFontWeight(QFont.Weight.Bold.value());

        QTextBlockFormat centerFormat = new QTextBlockFormat();
        centerFormat.setAlignment(
            new Qt.Alignment(Qt.AlignmentFlag.AlignHCenter));
        cursor.mergeBlockFormat(centerFormat);

        cursor = table.cellAt(0, 0).firstCursorPosition();
        cursor.insertText(("Details"), boldFormat);

        cursor = table.cellAt(1, 0).firstCursorPosition();
        cursor.insertText("Alan");

        cursor = table.cellAt(1, 1).firstCursorPosition();
        cursor.insertText("5, Pickety Street");

    //! [0]
        table.mergeCells(0, 0, 1, 2);
    //! [0] //! [1]
        table.splitCell(0, 0, 1, 1);
    //! [1]

        textEdit.show();

        QApplication.execStatic();
        QApplication.shutdown();
    }
}
