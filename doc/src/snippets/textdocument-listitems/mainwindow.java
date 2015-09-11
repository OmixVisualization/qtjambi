import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;

import java.util.*;

public class mainwindow extends QMainWindow
{
    public mainwindow()
    {
        QMenu fileMenu = new QMenu(tr("File"));

        fileMenu.addAction(tr("Exit"), this, "close()",
            new QKeySequence(tr("Ctrl+Q", "File|Exit")));

        QMenu actionsMenu = new QMenu(tr("Actions"));
        actionsMenu.addAction(tr("Highlight List Items"),
                              this, "highlightListItems()");
        actionsMenu.addAction(tr("Show Current List"), this, "showList()");

        QMenu insertMenu = new QMenu(tr("Insert"));

        insertMenu.addAction(tr("List"), this, "insertList()",
            new QKeySequence(tr("Ctrl+L", "Insert|List")));

        menuBar().addMenu(fileMenu);
        menuBar().addMenu(insertMenu);
        menuBar().addMenu(actionsMenu);

        editor = new QTextEdit(this);
        document = new QTextDocument(this);
        editor.setDocument(document);

        setCentralWidget(editor);
        setWindowTitle(tr("Text Document List Items"));
    }

    public void highlightListItems()
    {
        QTextCursor cursor = editor.textCursor();
        QTextList list = cursor.currentList();

        if (list == null)
            return;

        cursor.beginEditBlock();
    //! [0]
        for (int index = 0; index < list.count(); ++index) {
            QTextBlock listItem = list.item(index);
    //! [0]
            QTextBlockFormat newBlockFormat = listItem.blockFormat();
            newBlockFormat.setBackground(new QBrush(new QColor(Qt.GlobalColor.lightGray)));
            QTextCursor itemCursor = new QTextCursor(cursor);
            itemCursor.setPosition(listItem.position());
            //itemCursor.movePosition(QTextCursor.StartOfBlock);
            itemCursor.movePosition(QTextCursor.MoveOperation.EndOfBlock,
                                    QTextCursor.MoveMode.KeepAnchor);
            itemCursor.setBlockFormat(newBlockFormat);
            /*
    //! [1]
            processListItem(listItem);
    //! [1]
            */
    //! [2]
        }
    //! [2]
        cursor.endEditBlock();
    }

    public void showList()
    {
        QTextCursor cursor = editor.textCursor();
        QTextFrame frame = cursor.currentFrame();

        if (frame == null)
            return;

        QTreeWidget treeWidget = new QTreeWidget();
        treeWidget.setColumnCount(1);
        List<String> headerLabels = new Vector<String>();
        headerLabels.add(tr("Lists"));
        treeWidget.setHeaderLabels(headerLabels);

        QTreeWidgetItem parentItem = null;
        QTreeWidgetItem item = new QTreeWidgetItem();
        QTreeWidgetItem lastItem = null;
        parentItems.clear();
        previousItems.clear();

    //! [3]
        QTextFrame_iterator it;
        for (it = frame.begin(); !(it.atEnd()); it.next()) {
            QTextBlock block = it.currentBlock();

            if (block.isValid()) {

                QTextList list = block.textList();

                if (list != null) {
                    int index = list.itemNumber(block);
    //! [3]
                    if (index == 0) {
                        parentItems.add(parentItem);
                        previousItems.add(lastItem);
                        listStructures.add(list);
                        parentItem = lastItem;
                        lastItem = null;

                        if (parentItem != null)
                            item = new QTreeWidgetItem(parentItem, lastItem);
                        else
                            item = new QTreeWidgetItem(treeWidget, lastItem);

                    } else {
                        while (parentItem != null  &&
                              !listStructures.get(listStructures.size()-1).equals(list)) {
                            listStructures.remove(listStructures.size() - 1);
                            parentItem = parentItems.remove(parentItems.size() - 1);
                            lastItem = previousItems.remove(previousItems.size() - 1);
                        }
                        if (parentItem != null)
                            item = new QTreeWidgetItem(parentItem, lastItem);
                        else
                            item = new QTreeWidgetItem(treeWidget, lastItem);
                    }
                    item.setText(0, block.text());
                    lastItem = new QTreeWidgetItem(item);
                    /*
    //! [4]
                    processListItem(list, index);
    //! [4]
                    */
    //! [5]
                }
    //! [5] //! [6]
            }
    //! [6] //! [7]
        }
    //! [7]

        treeWidget.setWindowTitle(tr("List Contents"));
        treeWidget.show();
    }

    public void insertList()
    {
        QTextCursor cursor = editor.textCursor();
        cursor.beginEditBlock();

        QTextList list = cursor.currentList();
        QTextListFormat listFormat = new QTextListFormat();
        if (list != null)
            listFormat = (QTextListFormat) list.format();

        listFormat.setStyle(QTextListFormat.Style.ListDisc);
        listFormat.setIndent(listFormat.indent() + 1);
        cursor.insertList(listFormat);

        cursor.endEditBlock();
    }

    String currentFile = "";
    QTextEdit editor = new QTextEdit();
    QTextDocument document = new QTextDocument();
    List<QTextList> listStructures = new Vector<QTextList>();
    List<QTreeWidgetItem> previousItems = new Vector<QTreeWidgetItem>();
    List<QTreeWidgetItem> parentItems = new Vector<QTreeWidgetItem>();

    public static void main(String args[])
    {
        QApplication.initialize(args);

        new mainwindow().show();

        QApplication.execStatic();
        QApplication.shutdown();
    }
}
