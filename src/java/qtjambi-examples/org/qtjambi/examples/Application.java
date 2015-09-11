/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
** 
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
** 
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
** 
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
** 
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
** $END_LICENSE$

**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

package org.qtjambi.examples;

//! [0]
import org.qtjambi.qt.QVariant;
import org.qtjambi.qt.core.QFile;
import org.qtjambi.qt.core.QFileInfo;
import org.qtjambi.qt.core.QPoint;
import org.qtjambi.qt.core.QSettings;
import org.qtjambi.qt.core.QSize;
import org.qtjambi.qt.core.QTextStream;
import org.qtjambi.qt.core.Qt;
//! [0]
import org.qtjambi.qt.gui.QCloseEvent;
import org.qtjambi.qt.gui.QCursor;
import org.qtjambi.qt.gui.QIcon;
import org.qtjambi.qt.gui.QKeySequence;
import org.qtjambi.qt.widgets.QAction;
import org.qtjambi.qt.widgets.QApplication;
import org.qtjambi.qt.widgets.QFileDialog;
import org.qtjambi.qt.widgets.QMainWindow;
import org.qtjambi.qt.widgets.QMenu;
import org.qtjambi.qt.widgets.QMenuBar;
import org.qtjambi.qt.widgets.QMessageBox;
import org.qtjambi.qt.widgets.QTextEdit;
import org.qtjambi.qt.widgets.QToolBar;

@QtJambiExample(name = "Application")
//! [1]
public class Application extends QMainWindow {

    private String curFile;
    private QTextEdit textEdit;
    private QMenu fileMenu;
    private QMenu editMenu;
    private QMenu helpMenu;

    private QToolBar fileToolBar;
    private QToolBar editToolBar;

    private QAction newAct;
    private QAction openAct;
    private QAction saveAct;
    private QAction saveAsAct;
    private QAction exitAct;
    private QAction cutAct;
    private QAction copyAct;
    private QAction pasteAct;
    private QAction aboutAct;
    private QAction aboutQtJambiAct;
    private QAction aboutQtAct;

    private String rsrcPath = "classpath:org/qtjambi/images";
//! [1]

//! [2]
    public Application()
    {
        QMenuBar menuBar = new QMenuBar();
        setMenuBar(menuBar);
//! [2]

        setWindowIcon(new QIcon("classpath:org/qtjambi/images/qt-logo.png"));

        textEdit = new QTextEdit();
        setCentralWidget(textEdit);

//! [3]
        try {
            createActions();
        } catch (Exception e) {
            e.printStackTrace();
        }
        createMenus();
        createToolBars();
        createStatusBar();
//! [3]

        readSettings();

//! [4]
        textEdit.document().contentsChanged.connect(this, "documentWasModified()");

        setCurrentFile("");
    }
//! [4]

    @Override
//! [5]
    public void closeEvent(QCloseEvent event)
    {
        if (maybeSave()) {
            writeSettings();
            event.accept();
        } else {
            event.ignore();
//! [5] //! [6]
        }
//! [6] //! [7]
    }
//! [7]

//! [8]
    public void newFile()
    {
        if (maybeSave()) {
            textEdit.clear();
            setCurrentFile("");
        }
//! [8] //! [9]
    }
//! [9]

//! [10]
    public void open()
    {
        if (maybeSave()) {
            String fileName = QFileDialog.getOpenFileName(this);
            if (fileName.length() != 0)
                loadFile(fileName);
        }
//! [10] //! [11]
    }
//! [11]

//! [12]
    public boolean save()
    {
        if (curFile.length() == 0) {
            return saveAs();
        } else {
            return saveFile(curFile);
//! [12] //! [13]
        }
//! [13] //! [14]
    }
//! [14]

//! [15]
    public boolean saveAs()
    {
        String fileName = QFileDialog.getSaveFileName(this);
        if (fileName.length() == 0)
            return false;

        return saveFile(fileName);
    }
//! [15]

//! [16]
    public void about()
    {
        QMessageBox.about(this,
                         tr("About Application"),
                         tr("The <b>Application</b> example demonstrates how to " +
                            "write modern GUI applications using Qt, with a menu bar, " +
                            "toolbars, and a status bar."));
    }
//! [16]

//! [17]
    public void documentWasModified()
    {
        setWindowModified(textEdit.document().isModified());
    }
//! [17]

//! [18]
    private void createActions()
    {
        newAct = new QAction(new QIcon(rsrcPath + "/new.png"), tr("&New"), this);
        newAct.setShortcut(new QKeySequence(tr("Ctrl+N")));
        newAct.setStatusTip(tr("Create a new file"));
        newAct.triggered.connect(this, "newFile()");

        openAct = new QAction(new QIcon(rsrcPath + "/open.png"), tr("&Open..."), this);
        openAct.setShortcut(tr("Ctrl+O"));
        openAct.setStatusTip(tr("Open an existing file"));
        openAct.triggered.connect(this, "open()");
//! [18]

        saveAct = new QAction(new QIcon(rsrcPath + "/save.png"), tr("&Save"), this);
        saveAct.setShortcut(tr("Ctrl+S"));
        saveAct.setStatusTip(tr("Save the document to disk"));
        saveAct.triggered.connect(this, "save()");

        saveAsAct = new QAction(tr("Save &As..."), this);
        saveAsAct.setStatusTip(tr("Save the document under a new name"));
        saveAsAct.triggered.connect(this, "saveAs()");

        exitAct = new QAction(tr("E&xit"), this);
        exitAct.setShortcut(tr("Ctrl+Q"));
        exitAct.setStatusTip(tr("Exit the application"));
        exitAct.triggered.connect(this, "close()");

        cutAct = new QAction(new QIcon(rsrcPath + "/cut.png"), tr("Cu&t"), this);
        cutAct.setShortcut(tr("Ctrl+X"));
        cutAct.setStatusTip(tr("Cut the current selection's contents to the clipboard"));
        cutAct.triggered.connect(textEdit, "cut()");

        copyAct = new QAction(new QIcon(rsrcPath + "/copy.png"), tr("&Copy"), this);
        copyAct.setShortcut(tr("Ctrl+C"));
        copyAct.setStatusTip(tr("Copy the current selection's contents to the clipboard"));
        copyAct.triggered.connect(textEdit, "copy()");

        pasteAct = new QAction(new QIcon(rsrcPath + "/paste.png"), tr("&Paste"), this);
        pasteAct.setShortcut(tr("Ctrl+V"));
        pasteAct.setStatusTip(tr("Paste the clipboard's contents into the current selection"));
        pasteAct.triggered.connect(textEdit, "paste()");

        aboutAct = new QAction(tr("&About"), this);
        aboutAct.setStatusTip(tr("Show the application's About box"));
        aboutAct.triggered.connect(this, "about()");

        aboutQtJambiAct = new QAction(tr("About &Qt Jambi"), this);
        aboutQtJambiAct.setStatusTip(tr("Show the Qt Jambi library's About box"));
        aboutQtJambiAct.triggered.connect(QApplication.instance(), "aboutQtJambi()");

//! [19]
        aboutQtAct = new QAction(tr("About Q&t"), this);
        aboutQtAct.setStatusTip(tr("Show the Qt library's About box"));
        aboutQtAct.triggered.connect(QApplication.instance(), "aboutQt()");
//! [19]


//! [20]
        cutAct.setEnabled(false);
        copyAct.setEnabled(false);
        textEdit.copyAvailable.connect(cutAct, "setEnabled(boolean)");
        textEdit.copyAvailable.connect(copyAct, "setEnabled(boolean)");
    }
//! [20]

//! [21]
    private void createMenus()
    {
        fileMenu = menuBar().addMenu(tr("&File"));
        fileMenu.addAction(newAct);
        fileMenu.addAction(openAct);
        fileMenu.addAction(saveAct);
        fileMenu.addAction(saveAsAct);
        fileMenu.addSeparator();
        fileMenu.addAction(exitAct);

        editMenu = menuBar().addMenu(tr("&Edit"));
        editMenu.addAction(cutAct);
        editMenu.addAction(copyAct);
        editMenu.addAction(pasteAct);

        menuBar().addSeparator();

        helpMenu = menuBar().addMenu(tr("&Help"));
        helpMenu.addAction(aboutAct);
        helpMenu.addSeparator();
        helpMenu.addAction(aboutQtJambiAct);
        helpMenu.addAction(aboutQtAct);
    }
//! [21]

//! [22]
    private void createToolBars()
    {
        fileToolBar = addToolBar(tr("File"));
        fileToolBar.addAction(newAct);
        fileToolBar.addAction(openAct);
        fileToolBar.addAction(saveAct);

        editToolBar = addToolBar(tr("Edit"));
        editToolBar.addAction(cutAct);
        editToolBar.addAction(copyAct);
        editToolBar.addAction(pasteAct);
    }
//! [22]

//! [23]
    private void createStatusBar()
    {
        statusBar().showMessage(tr("Ready"));
    }
//! [23]

    private void readSettings()
    {
        QSettings settings = new QSettings("Nokia", "Application Example");
        QPoint pos = QVariant.toPoint(settings.value("pos", new QPoint(200, 200)));
        QSize size = QVariant.toSize(settings.value("size", new QSize(400, 400)));
        resize(size);
        move(pos);
    }

    private void writeSettings()
    {
        QSettings settings = new QSettings("Nokia", "Application Example");
        settings.setValue("pos", pos());
        settings.setValue("size", size());
    }

//! [24]
    private boolean maybeSave()
    {
        if (textEdit.document().isModified()) {
            QMessageBox.StandardButton ret = QMessageBox.warning(this, tr("Application"),
                                                                 tr("The document has been modified.\n" +
                                                                    "Save your changes?"),
                                                                 new QMessageBox.StandardButtons(QMessageBox.StandardButton.Ok,
                                                                                                 QMessageBox.StandardButton.Discard,
                                                                                                 QMessageBox.StandardButton.Cancel));
            if (ret == QMessageBox.StandardButton.Ok) {
                return save();
            } else if (ret == QMessageBox.StandardButton.Cancel) {
                return false;
            }
        }
        return true;
//! [24] //! [25]
    }
//! [25]

//! [26]
    public void loadFile(String fileName)
    {
        QFile file = new QFile(fileName);
        if (!file.open(new QFile.OpenMode(QFile.OpenModeFlag.ReadOnly, QFile.OpenModeFlag.Text))) {
            QMessageBox.warning(this, tr("Application"), String.format(tr("Cannot read file %1$s:\n%2$s."), fileName, file.errorString()));
            return;
        }
//! [26] //! [27]

        QTextStream in = new QTextStream(file);
        QApplication.setOverrideCursor(new QCursor(Qt.CursorShape.WaitCursor));
        textEdit.setPlainText(in.readAll());
        QApplication.restoreOverrideCursor();

        setCurrentFile(fileName);
        statusBar().showMessage(tr("File loaded"), 2000);
    }
//! [27]

//! [28]
    public boolean saveFile(String fileName)
    {
        QFile file = new QFile(fileName);
        if (!file.open(new QFile.OpenMode(QFile.OpenModeFlag.WriteOnly, QFile.OpenModeFlag.Text))) {
            QMessageBox.warning(this, tr("Application"), String.format(tr("Cannot write file %1$s:\n%2$s."), fileName, file.errorString()));
            return false;
        }

        QTextStream out = new QTextStream(file);
        QApplication.setOverrideCursor(new QCursor(Qt.CursorShape.WaitCursor));
        out.writeString(textEdit.toPlainText());
        QApplication.restoreOverrideCursor();

        setCurrentFile(fileName);
        statusBar().showMessage(tr("File saved"), 2000);
        file.close();
        return true;
//! [28] //! [29]
    }
//! [29]

//! [30]
    public void setCurrentFile(String fileName)
    {
        curFile = fileName;
        textEdit.document().setModified(false);
        setWindowModified(false);

        String shownName;
        if (curFile.length() == 0)
            shownName = "untitled.txt";
        else
            shownName = strippedName(curFile);

        setWindowTitle(String.format(tr("%1$s[*] - %2$s"), shownName, tr("Application")));
    }
//! [30]

//! [31]
    private static String strippedName(String fullFileName)
    {
        return new QFileInfo(fullFileName).fileName();
    }
//! [31]

//! [32]
    public static void main(String[] args) {
        QApplication.initialize(args);

        Application application = new Application();
        application.show();

        QApplication.execStatic();
        QApplication.shutdown();
    }
//! [32]

}
