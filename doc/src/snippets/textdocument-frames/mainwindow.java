import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;

import java.util.*;

public class mainwindow extends QMainWindow
{
    QTextEdit editor;

    public mainwindow()
    {
        QMenu fileMenu = new QMenu(tr("File"));

        QAction saveAction = fileMenu.addAction(tr("Save..."));
        saveAction.setShortcut(tr("Ctrl+S"));

        QAction quitAction = fileMenu.addAction(tr("Exit"));
        quitAction.setShortcut(tr("Ctrl+Q"));

        menuBar().addMenu(fileMenu);
        editor = new QTextEdit();

        QTextCursor cursor = editor.textCursor();
        cursor.movePosition(QTextCursor.MoveOperation.Start);

        //QTextFrame mainFrame = cursor.currentFrame();

        QTextCharFormat plainCharFormat = new QTextCharFormat();
        QTextCharFormat boldCharFormat = new QTextCharFormat();
        boldCharFormat.setFontWeight(QFont.Weight.Bold.value());
    // main farme
    //! [0]
        QTextFrame mainFrame = cursor.currentFrame();
        cursor.insertText("...");
    //! [0]

        cursor.insertText("Text documents are represented by the " +
                          "QTextDocument class, rather than by String objects. " +
                          "Each QTextDocument object contains information about " +
                          "the document's internal representation, its structure, " +
                          "and keeps track of modifications to provide undo/redo " +
                          "facilities. This approach allows features such as the " +
                          "layout management to be delegated to specialized " +
                          "classes, but also provides a focus for the framework.",
                          plainCharFormat);

    //! [1]
        QTextFrameFormat frameFormat = new QTextFrameFormat();
        frameFormat.setMargin(32);
        frameFormat.setPadding(8);
        frameFormat.setBorder(4);
    //! [1]
        cursor.insertFrame(frameFormat);

    //  insert frame
    //! [2]
        cursor.insertFrame(frameFormat);
        cursor.insertText("...");
    //! [2]

        cursor.insertText("Documents are either converted from external sources " +
                          "or created from scratch using Qt. The creation process " +
                          "can done by an editor widget, such as QTextEdit, or by " +
                          "explicit calls to the Scribe API.", boldCharFormat);

        cursor = mainFrame.lastCursorPosition();
    //  last cursor
    //! [3]
        cursor = mainFrame.lastCursorPosition();
        cursor.insertText("...");
    //! [3]

        cursor.insertText("There are two complementary ways to visualize the " +
                          "contents of a document: as a linear buffer that is " +
                          "used by editors to modify the contents, and as an " +
                          "object hierarchy containing structural information " +
                          "that is useful to layout engines. In the hierarchical " +
                          "model, the objects generally correspond to visual " +
                          "elements such as frames, tables, and lists. At a lower " +
                          "level, these elements describe properties such as the " +
                          "style of text used and its alignment. The linear " +
                          "representation of the document is used for editing and " +
                          "manipulation of the document's contents.",
                          plainCharFormat);

        saveAction.triggered.connect(this, "saveFile()");
        quitAction.triggered.connect(this, "close()");

        setCentralWidget(editor);
        setWindowTitle(tr("Text Document Frames"));
    }

    public void saveFile()
    {
        String fileName = QFileDialog.getSaveFileName(this,
            tr("Save document as:"), "", new QFileDialog.Filter(tr("XML (.xml)")));

        if (fileName.length() < 1) {
            if (writeXml(fileName))
                setWindowTitle(fileName);
            else
                QMessageBox.warning(this, tr("Warning"),
                    tr("Failed to save the document."),
                    new QMessageBox.StandardButtons(QMessageBox.StandardButton.Cancel));
        }
    }

    private boolean writeXml(String fileName)
    {
        xmlwriter documentWriter = new xmlwriter(editor.document());

        QDomDocument domDocument = documentWriter.toXml();
        QFile file = new QFile(fileName);

        if (file.open(QIODevice.OpenModeFlag.WriteOnly)) {
            QTextStream textStream = new QTextStream(file);
            textStream.setCodec(QTextCodec.codecForName("UTF-8"));

            textStream.writeString(domDocument.toString(1));
            file.close();
            return true;
        }
        else
            return false;
    }

    public static void main(String args[])
    {
        QApplication.initialize(args);

        new mainwindow().show();

        QApplication.execStatic();
        QApplication.shutdown();
    }
}
