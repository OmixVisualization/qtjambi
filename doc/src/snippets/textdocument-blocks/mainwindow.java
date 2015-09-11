import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;

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

        QMenu insertMenu = new QMenu(tr("Insert"));

        QAction calendarAction = insertMenu.addAction(tr("Calendar"));
        calendarAction.setShortcut(tr("Ctrl+I"));

        menuBar().addMenu(fileMenu);
        menuBar().addMenu(insertMenu);

    //! [0]
        editor = new QTextEdit(this);
    //! [0]

        saveAction.triggered.connect(this, "saveFile()");
        quitAction.triggered.connect(this, "close()");
        calendarAction.triggered.connect(this, "insertCalendar()");

        setCentralWidget(editor);
        setWindowTitle(tr("Text Document Writer"));
    }

    void saveFile()
    {
        String fileName = QFileDialog.getSaveFileName(this,
            tr("Save document as:"), "",
            new QFileDialog.Filter(tr("XML (.xml)")));

        if (fileName.length() > 1) {
            if (writeXml(fileName))
                setWindowTitle(fileName);
            else
                QMessageBox.warning(this, tr("Warning"),
                    tr("Failed to save the document."),
                    new QMessageBox.StandardButtons(QMessageBox.StandardButton.Cancel));
        }
    }

    void insertCalendar()
    {
    //! [1]
        QTextCursor cursor = new QTextCursor(editor.textCursor());
        cursor.movePosition(QTextCursor.MoveOperation.Start);

        QTextCharFormat format = cursor.charFormat();
        format.setFontFamily("Courier");

        QTextCharFormat boldFormat = format;
        boldFormat.setFontWeight(QFont.Weight.Bold.value());

        cursor.insertBlock();
        cursor.insertText(" ", boldFormat);

        QDate date = QDate.currentDate();
        int year = date.year();
        int month = date.month();

        for (int weekDay = 1; weekDay <= 7; ++weekDay) {
            cursor.insertText(QDate.shortDayName(weekDay),
                boldFormat);
        }

        cursor.insertBlock();
        cursor.insertText(" ", format);

        for (int column = 1; column < new QDate(year, month, 1).dayOfWeek(); ++column) {
            cursor.insertText("    ", format);
        }

        for (int day = 1; day <= date.daysInMonth(); ++day) {
    //! [1] //! [2]
            int weekDay = new QDate(year, month, day).dayOfWeek();

            if (new QDate(year, month, day).equals(date))
                cursor.insertText(String.valueOf(day), boldFormat);
            else
                cursor.insertText(String.valueOf(day), format);

            if (weekDay == 7) {
                cursor.insertBlock();
                cursor.insertText(" ", format);
            }
    //! [2] //! [3]
        }
    //! [3]
    }

    boolean writeXml(String fileName)
    {
        /*XmlWriter documentWriter = new XmlWriter(editor.document());

        QDomDocument domDocument = documentWriter.toXml();
        QFile file = new QFile(fileName);

        if (file.open(QIODevice.OpenModeFlag.WriteOnly)) {
            QTextStream textStream = new QTextStream(file);
            textStream.setCodec(QTextCodec.codecForName("UTF-8"));

            textStream.writeString(domDocument.toString(1).toUtf8());
            file.close();
            return true;
        }
        else
            return false;*/
        return true;
    }
}
