/*   Ported from: src.gui.text.qtextcursor.cpp
*/
import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class src_gui_text_qtextcursor extends QWidget {
    static void main(String args[]) {
        QApplication.initialize(args);

        QTextEdit textEdit = new QTextEdit();
        textEdit.setPlainText("Hello, my name is Tommy and I am 4 years old.");

        QTextDocument textDocument = textEdit.document();
        QTextCursor cursor = new QTextCursor(textDocument);

        String searchText = "Hello";
        cursor = textDocument.find(searchText, cursor, QTextDocument.FindFlag.FindWholeWords);

        if (!cursor.isNull()) {
            cursor.movePosition(QTextCursor.MoveOperation.WordRight, QTextCursor.MoveMode.KeepAnchor);
            insertHelloWorld(cursor);
            insertImage(textDocument, cursor);

            //clearDocument();
            insertMoreText(textDocument);
            //insertEvenMoreText(textDocument);
        }
    }

    public static void insertHelloWorld(QTextCursor cursor) {
        //! [0]
        cursor.clearSelection();
        cursor.movePosition(QTextCursor.MoveOperation.NextWord, QTextCursor.MoveMode.KeepAnchor);
        cursor.insertText("Hello World");
        //! [0]
    }

    public static void insertImage(QTextDocument textDocument, QTextCursor cursor) {
        //! [1]
        QImage img = new QImage();
        textDocument.addResource(QTextDocument.ResourceType.ImageResource.value(), new QUrl("myimage"), img);
        cursor.insertImage("myimage");
        //! [1]
    }

    public static void clearDocument(QTextDocument textDocument) {
        textDocument.clear();
    }

    public static void insertMoreText(QTextDocument textDocument){
        //! [2]
        QTextCursor cursor = new QTextCursor(textDocument);
        cursor.beginEditBlock();
        cursor.insertText("Hello");
        cursor.insertText("World");
        cursor.endEditBlock();

        textDocument.undo();
        //! [2]
    }

    public static void insertEvenMoreText(QTextDocument textDocument) {
        //! [3]
        QTextCursor cursor = new QTextCursor(textDocument);
        cursor.beginEditBlock();
        cursor.insertText("Hello");
        cursor.insertText("World");
        cursor.endEditBlock();

        // ...

        cursor.joinPreviousEditBlock();
        cursor.insertText("Hey");
        cursor.endEditBlock();

        textDocument.undo();
        //! [3]
    }
}
