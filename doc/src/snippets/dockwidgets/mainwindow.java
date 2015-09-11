import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;

import java.util.*;

public class mainwindow extends QMainWindow
{

    public mainwindow()
    {
        setWindowTitle(tr("Dock Widgets"));

        setupDockWindow();
        setupContents();
        setupMenus();

        textBrowser = new QTextBrowser(this);

        headingList.itemClicked.connect(this, "updateText(QListWidgetItem)");

        updateText(headingList.item(0));
        headingList.setCurrentRow(0);
        setCentralWidget(textBrowser);
    }

    void setupContents()
    {
        QFile titlesFile = new QFile("classpath:Resources/titles.txt");
        titlesFile.open(QIODevice.OpenModeFlag.ReadOnly);
        int chapter = 0;

        do {
            String line = new String(titlesFile.readLine().toByteArray()).trim();
            String parts[] = line.split("\t+");
            if (parts.length != 2)
                break;

            String chapterTitle = parts[0];
            String fileName = parts[1];

            QFile chapterFile = new QFile(fileName);

            chapterFile.open(QIODevice.OpenModeFlag.ReadOnly);
            QListWidgetItem item = new QListWidgetItem(chapterTitle, headingList);
            item.setData(Qt.ItemDataRole.DisplayRole, chapterTitle);
            item.setData(Qt.ItemDataRole.UserRole, chapterFile.readAll());
            item.setFlags(new Qt.ItemFlags(Qt.ItemFlag.ItemIsEnabled, Qt.ItemFlag.ItemIsSelectable));
            chapterFile.close();

            ++chapter;
        } while (titlesFile.isOpen());

        titlesFile.close();
    }

    void setupDockWindow()
    {
    //! [0]
        contentsWindow = new QDockWidget(tr("Table of Contents"), this);
        contentsWindow.setAllowedAreas(new Qt.DockWidgetAreas(Qt.DockWidgetArea.LeftDockWidgetArea
                                      , Qt.DockWidgetArea.RightDockWidgetArea));
        addDockWidget(Qt.DockWidgetArea.LeftDockWidgetArea, contentsWindow);

        headingList = new QListWidget(contentsWindow);
        contentsWindow.setWidget(headingList);
    //! [0]
    }

    void setupMenus()
    {
        QAction exitAct = new QAction(tr("Exit"), this);
        exitAct.setShortcut(tr("Ctrl+Q"));
        exitAct.setStatusTip(tr("Exit the application"));
        exitAct.triggered.connect(QApplication.instance(), "closeAllWindows()");

        QMenu fileMenu = menuBar().addMenu(tr("File"));
        fileMenu.addAction(exitAct);
    }

    void updateText(QListWidgetItem item)
    {
        String text = item.data(Qt.ItemDataRole.UserRole).toString();
        textBrowser.setHtml(text);
    }

    private QDockWidget contentsWindow;
    private QListWidget headingList;
    private QTextBrowser textBrowser;

    public static void main(String args[])
    {
        QApplication.initialize(args);

        new mainwindow().show();

        QApplication.execStatic();
        QApplication.shutdown();
    }
}
