/****************************************************************************
 **
 **  (C) 1992-2009 Nokia. All rights reserved.
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

import org.qtjambi.qt.core.Qt.AlignmentFlag;
import org.qtjambi.qt.gui.QContextMenuEvent;
import org.qtjambi.qt.gui.QFont;
import org.qtjambi.qt.gui.QIcon;
import org.qtjambi.qt.widgets.QAction;
import org.qtjambi.qt.widgets.QActionGroup;
import org.qtjambi.qt.widgets.QApplication;
import org.qtjambi.qt.widgets.QFrame;
import org.qtjambi.qt.widgets.QLabel;
import org.qtjambi.qt.widgets.QMainWindow;
import org.qtjambi.qt.widgets.QMenu;
import org.qtjambi.qt.widgets.QMessageBox;
import org.qtjambi.qt.widgets.QSizePolicy;
import org.qtjambi.qt.widgets.QVBoxLayout;
import org.qtjambi.qt.widgets.QWidget;

@QtJambiExample(name = "Menus")
//! [0]
public class Menus extends QMainWindow {
//! [0]

//! [1]
    public static void main(String args[]) {
        QApplication.initialize(args);
        Menus menus = new Menus();
        menus.show();
        QApplication.execStatic();
        QApplication.shutdown();
    }
//! [1]

//! [2]
    QMenu fileMenu;
    QMenu editMenu;
    QMenu formatMenu;
    QMenu helpMenu;
    QActionGroup alignmentGroup;
    QAction newAct;
    QAction openAct;
    QAction saveAct;
    QAction printAct;
    QAction exitAct;
    QAction undoAct;
    QAction redoAct;
    QAction cutAct;
    QAction copyAct;
    QAction pasteAct;
    QAction boldAct;
    QAction italicAct;
    QAction leftAlignAct;
    QAction rightAlignAct;
    QAction justifyAct;
    QAction centerAct;
    QAction setLineSpacingAct;
    QAction setParagraphSpacingAct;
    QAction aboutAct;
    QAction aboutQtAct;
    QAction aboutQtJambiAct;
    QLabel infoLabel;
//! [2]

//! [3]
    public Menus() {
        QWidget widget = new QWidget();
        setCentralWidget(widget);
//! [3]

//! [4]
        QWidget topFiller = new QWidget();
        topFiller.setSizePolicy(QSizePolicy.Policy.Expanding,
                                QSizePolicy.Policy.Expanding);

        infoLabel = new QLabel(tr("<i>Choose a menu option, "
                                  + "or right-click to invoke "
                                  + "a context menu</i>"));
        infoLabel.setFrameStyle(QFrame.Shape.StyledPanel.value()
                                | QFrame.Shadow.Sunken.value());
        infoLabel.setAlignment(AlignmentFlag.AlignCenter);

        QWidget bottomFiller = new QWidget();
        bottomFiller.setSizePolicy(QSizePolicy.Policy.Expanding,
                                   QSizePolicy.Policy.Expanding);

        QVBoxLayout layout = new QVBoxLayout();
        layout.setMargin(5);
        layout.addWidget(topFiller);
        layout.addWidget(infoLabel);
        layout.addWidget(bottomFiller);
        widget.setLayout(layout);
//! [4]

//! [5]
        createActions();
        createMenus();

        statusBar().showMessage(tr("A context menu is available by "
                                   + "right-clicking"));
//! [5]

//! [6]
        setWindowTitle(tr("Menus"));
        setMinimumSize(160, 160);
        resize(480, 320);
        setWindowIcon(new QIcon("classpath:org/qtjambi/images/qt-logo.png"));
    }
//! [6]

    @Override
//! [7]
    public void contextMenuEvent(QContextMenuEvent event) {
        QMenu menu = new QMenu(this);
        menu.addAction(cutAct);
        menu.addAction(copyAct);
        menu.addAction(pasteAct);
        menu.exec(event.globalPos());
    }
//! [7]

    void newFile() {
        infoLabel.setText(tr("Invoked <b>File|New</b>"));
    }

    void open() {
        infoLabel.setText(tr("Invoked <b>File|Open</b>"));
    }

    void save() {
        infoLabel.setText(tr("Invoked <b>File|Save</b>"));
    }

    void print() {
        infoLabel.setText(tr("Invoked <b>File|Print</b>"));
    }

    void undo() {
        infoLabel.setText(tr("Invoked <b>Edit|Undo</b>"));
    }

    void redo() {
        infoLabel.setText(tr("Invoked <b>Edit|Redo</b>"));
    }

    void cut() {
        infoLabel.setText(tr("Invoked <b>Edit|Cut</b>"));
    }

    void copy() {
        infoLabel.setText(tr("Invoked <b>Edit|Copy</b>"));
    }

    void paste() {
        infoLabel.setText(tr("Invoked <b>Edit|Paste</b>"));
    }

    void bold() {
        infoLabel.setText(tr("Invoked <b>Edit|Format|Bold</b>"));
    }

    void italic() {
        infoLabel.setText(tr("Invoked <b>Edit|Format|Italic</b>"));
    }

    void leftAlign() {
        infoLabel.setText(tr("Invoked <b>Edit|Format|Left Align</b>"));
    }

    void rightAlign() {
        infoLabel.setText(tr("Invoked <b>Edit|Format|Right Align</b>"));
    }

    void justify() {
        infoLabel.setText(tr("Invoked <b>Edit|Format|Justify</b>"));
    }

    void center() {
        infoLabel.setText(tr("Invoked <b>Edit|Format|Center</b>"));
    }

    void setLineSpacing() {
        infoLabel.setText(tr("Invoked <b>Edit|Format|Set Line Spacing</b>"));
    }

    void setParagraphSpacing() {
        infoLabel.setText(tr("Invoked <b>Edit|Format|"
                             + "Set Paragraph Spacing</b>"));
    }

    void about() {
        infoLabel.setText(tr("Invoked <b>Help|About</b>"));
        QMessageBox.about(this, tr("About Menu"),
                          tr("The <b>Menu</b> example shows how to create "
                             + "menu-bar menus and context menus."));
    }

    void aboutQt() {
        infoLabel.setText(tr("Invoked <b>Help|About Qt</b>"));
    }

    void aboutQtJambi() {
        infoLabel.setText(tr("Invoked <b>Help|About Qt Jambi</b>"));
    }

//! [8]
    void createActions() {
        newAct = new QAction(tr("&New"), this);
        newAct.setShortcut(tr("Ctrl+N"));
        newAct.setStatusTip(tr("Create a new file"));
        newAct.triggered.connect(this, "newFile()");
//! [8]

        openAct = new QAction(tr("&Open..."), this);
        openAct.setShortcut(tr("Ctrl+O"));
        openAct.setStatusTip(tr("Open an existing file"));
        openAct.triggered.connect(this, "open()");

        saveAct = new QAction(tr("&Save"), this);
        saveAct.setShortcut(tr("Ctrl+S"));
        saveAct.setStatusTip(tr("Save the document to disk"));
        saveAct.triggered.connect(this, "save()");

        printAct = new QAction(tr("&Print..."), this);
        printAct.setShortcut(tr("Ctrl+P"));
        printAct.setStatusTip(tr("Print the document"));
        printAct.triggered.connect(this, "print()");

        exitAct = new QAction(tr("E&xit"), this);
        exitAct.setShortcut(tr("Ctrl+Q"));
        exitAct.setStatusTip(tr("Exit the application"));
        exitAct.triggered.connect(this, "close()");

        undoAct = new QAction(tr("&Undo"), this);
        undoAct.setShortcut(tr("Ctrl+Z"));
        undoAct.setStatusTip(tr("Undo the last operation"));
        undoAct.triggered.connect(this, "undo()");

        redoAct = new QAction(tr("&Redo"), this);
        redoAct.setShortcut(tr("Ctrl+Y"));
        redoAct.setStatusTip(tr("Redo the last operation"));
        redoAct.triggered.connect(this, "redo()");

        cutAct = new QAction(tr("Cu&t"), this);
        cutAct.setShortcut(tr("Ctrl+X"));
        cutAct.setStatusTip(tr("Cut the current selection's contents to the clipboard"));
        cutAct.triggered.connect(this, "cut()");

        copyAct = new QAction(tr("&Copy"), this);
        copyAct.setShortcut(tr("Ctrl+C"));
        copyAct.setStatusTip(tr("Copy the current selection's contents to the clipboard"));
        copyAct.triggered.connect(this, "copy()");

        pasteAct = new QAction(tr("&Paste"), this);
        pasteAct.setShortcut(tr("Ctrl+V"));
        pasteAct.setStatusTip(tr("Paste the clipboard's contents into the current selection"));
        pasteAct.triggered.connect(this, "paste()");

        boldAct = new QAction(tr("&Bold"), this);
        boldAct.setCheckable(true);
        boldAct.setShortcut(tr("Ctrl+B"));
        boldAct.setStatusTip(tr("Make the text bold"));
        boldAct.triggered.connect(this, "bold()");

        QFont boldFont = boldAct.font();
        boldFont.setBold(true);
        boldAct.setFont(boldFont);

        italicAct = new QAction(tr("&Italic"), this);
        italicAct.setCheckable(true);
        italicAct.setShortcut(tr("Ctrl+I"));
        italicAct.setStatusTip(tr("Make the text italic"));
        italicAct.triggered.connect(this, "italic()");

        QFont italicFont = italicAct.font();
        italicFont.setItalic(true);
        italicAct.setFont(italicFont);

        setLineSpacingAct = new QAction(tr("Set &Line Spacing..."), this);
        setLineSpacingAct.setStatusTip(tr("Change the gap between the lines of a paragraph"));
        setLineSpacingAct.triggered.connect(this, "setLineSpacing()");

        setParagraphSpacingAct = new QAction(tr("Set &Paragraph Spacing..."),
                                             this);
        setLineSpacingAct.setStatusTip(tr("Change the gap between paragraphs"));
        setParagraphSpacingAct.triggered.connect(this, "setParagraphSpacing()");

        aboutAct = new QAction(tr("&About"), this);
        aboutAct.setStatusTip(tr("Show the application's About box"));
        aboutAct.triggered.connect(this, "about()");

        aboutQtJambiAct = new QAction(tr("About &Qt Jambi"), this);
        aboutQtJambiAct.setStatusTip(tr("Show the Qt Jambi library's About box"));
        aboutQtJambiAct.triggered.connect(QApplication.instance(), "aboutQtJambi()");
        aboutQtJambiAct.triggered.connect(this, "aboutQtJambi()");

        aboutQtAct = new QAction(tr("About Q&t"), this);
        aboutQtAct.setStatusTip(tr("Show the Qt library's About box"));
        aboutQtAct.triggered.connect(QApplication.instance(), "aboutQt()");
        aboutQtAct.triggered.connect(this, "aboutQt()");

        leftAlignAct = new QAction(tr("&Left Align"), this);
        leftAlignAct.setCheckable(true);
        leftAlignAct.setShortcut(tr("Ctrl+L"));
        leftAlignAct.setStatusTip(tr("Left align the selected text"));
        leftAlignAct.triggered.connect(this, "leftAlign()");

        rightAlignAct = new QAction(tr("&Right Align"), this);
        rightAlignAct.setCheckable(true);
        rightAlignAct.setShortcut(tr("Ctrl+R"));
        rightAlignAct.setStatusTip(tr("Right align the selected text"));
        rightAlignAct.triggered.connect(this, "rightAlign()");

        justifyAct = new QAction(tr("&Justify"), this);
        justifyAct.setCheckable(true);
        justifyAct.setShortcut(tr("Ctrl+J"));
        justifyAct.setStatusTip(tr("Justify the selected text"));
        justifyAct.triggered.connect(this, "justify()");

        centerAct = new QAction(tr("&Center"), this);
        centerAct.setCheckable(true);
        centerAct.setShortcut(tr("Ctrl+E"));
        centerAct.setStatusTip(tr("Center the selected text"));
        centerAct.triggered.connect(this, "center()");

//! [9]
        alignmentGroup = new QActionGroup(this);
        alignmentGroup.addAction(leftAlignAct);
        alignmentGroup.addAction(rightAlignAct);
        alignmentGroup.addAction(justifyAct);
        alignmentGroup.addAction(centerAct);
//! [9]
        leftAlignAct.setChecked(true);
//! [10]
    }
//! [10]

//! [11]
    void createMenus() {
        fileMenu = menuBar().addMenu(tr("&File"));
        fileMenu.addAction(newAct);
        fileMenu.addAction(openAct);
        fileMenu.addAction(saveAct);
        fileMenu.addAction(printAct);
        fileMenu.addSeparator();
        fileMenu.addAction(exitAct);

        editMenu = menuBar().addMenu(tr("&Edit"));
        editMenu.addAction(undoAct);
        editMenu.addAction(redoAct);
        editMenu.addSeparator();
        editMenu.addAction(cutAct);
        editMenu.addAction(copyAct);
        editMenu.addAction(pasteAct);
        editMenu.addSeparator();

        helpMenu = menuBar().addMenu(tr("&Help"));
        helpMenu.addAction(aboutAct);
        helpMenu.addSeparator();
        helpMenu.addAction(aboutQtJambiAct);
        helpMenu.addAction(aboutQtAct);
//! [11]

//! [12]
        formatMenu = editMenu.addMenu(tr("&Format"));
        formatMenu.addAction(boldAct);
        formatMenu.addAction(italicAct);
        formatMenu.addSeparator().setText(tr("Alignment"));
        formatMenu.addAction(leftAlignAct);
        formatMenu.addAction(rightAlignAct);
        formatMenu.addAction(justifyAct);
        formatMenu.addAction(centerAct);
        formatMenu.addSeparator();
        formatMenu.addAction(setLineSpacingAct);
        formatMenu.addAction(setParagraphSpacingAct);
    }
//! [12] //! [13]
}
//! [13]
