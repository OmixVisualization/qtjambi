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

package org.qtjambi.demos;

import org.qtjambi.examples.QtJambiExample;
import org.qtjambi.qt.core.QByteArray;
import org.qtjambi.qt.core.QFile;
import org.qtjambi.qt.core.QFileInfo;
import org.qtjambi.qt.core.QTextCodec;
import org.qtjambi.qt.core.QTextStream;
import org.qtjambi.qt.core.Qt;
import org.qtjambi.qt.gui.QCloseEvent;
import org.qtjambi.qt.gui.QColor;
import org.qtjambi.qt.gui.QFont;
import org.qtjambi.qt.gui.QFontDatabase;
import org.qtjambi.qt.gui.QIcon;
import org.qtjambi.qt.gui.QKeySequence;
import org.qtjambi.qt.gui.QKeySequence.StandardKey;
import org.qtjambi.qt.gui.QPixmap;
import org.qtjambi.qt.gui.QTextBlockFormat;
import org.qtjambi.qt.gui.QTextCharFormat;
import org.qtjambi.qt.gui.QTextCursor;
import org.qtjambi.qt.gui.QTextListFormat;
import org.qtjambi.qt.printsupport.QPrintDialog;
import org.qtjambi.qt.printsupport.QPrinter;
import org.qtjambi.qt.widgets.QAction;
import org.qtjambi.qt.widgets.QActionGroup;
import org.qtjambi.qt.widgets.QApplication;
import org.qtjambi.qt.widgets.QColorDialog;
import org.qtjambi.qt.widgets.QComboBox;
import org.qtjambi.qt.widgets.QDialog;
import org.qtjambi.qt.widgets.QFileDialog;
import org.qtjambi.qt.widgets.QMainWindow;
import org.qtjambi.qt.widgets.QMenu;
import org.qtjambi.qt.widgets.QMessageBox;
import org.qtjambi.qt.widgets.QTextEdit;
import org.qtjambi.qt.widgets.QToolBar;

@QtJambiExample(name = "Text Edit")
public class TextEdit extends QMainWindow {

    public TextEdit() {
        init();                    
    }

    private void init() {
        setWindowIcon(new QIcon("classpath:org/qtjambi/images/qt-logo.png"));

        setupFileActions();
        setupEditActions();
        setupTextActions();

        textEdit = new QTextEdit(this);
        textEdit.currentCharFormatChanged.connect(this,
                "currentCharFormatChanged(QTextCharFormat)");

        setCentralWidget(textEdit);
        textEdit.setFocus();
        setCurrentFileName("");

        fontChanged(textEdit.font());
        colorChanged(textEdit.textColor());
        alignmentChanged(textEdit.alignment());

        textEdit.document().modificationChanged.connect(actionSave,
                "setEnabled(boolean)");
        textEdit.document().modificationChanged.connect(this,
                "setWindowModified(boolean)");
        textEdit.document().undoAvailable.connect(actionUndo,
                "setEnabled(boolean)");
        textEdit.document().redoAvailable.connect(actionRedo,
                "setEnabled(boolean)");

        setWindowModified(textEdit.document().isModified());
        actionSave.setEnabled(textEdit.document().isModified());
        actionUndo.setEnabled(textEdit.document().isUndoAvailable());
        actionRedo.setEnabled(textEdit.document().isRedoAvailable());

        actionUndo.triggered.connect(textEdit.document(), "undo()");
        actionRedo.triggered.connect(textEdit.document(), "redo()");

        actionCut.setEnabled(false);
        actionCopy.setEnabled(false);

        actionCut.triggered.connect(textEdit, "cut()");
        actionCopy.triggered.connect(textEdit, "copy()");
        actionPaste.triggered.connect(textEdit, "paste()");

        textEdit.copyAvailable.connect(actionCut, "setEnabled(boolean)");
        textEdit.copyAvailable.connect(actionCopy, "setEnabled(boolean)");

        QApplication.clipboard().dataChanged.connect(this,
                "clipboardDataChanged()");

        String initialFile = "classpath:org/qtjambi/demos/example.html";

        if (!load(initialFile))
            fileNew();

        resize(640, 800);
    }

    @Override
    protected void closeEvent(QCloseEvent e) {
        if (maybeSave())
            e.accept();
        else
            e.ignore();
    }

    private QAction action(String name, String image, Object shortcut, String slot,
                           QMenu menu, QToolBar toolBar) {
        QAction a = new QAction(name, this);

        if (image != null) a.setIcon(new QIcon(rsrcPath + "/" + image + ".png"));
        if (menu != null) menu.addAction(a);
        if (toolBar != null) toolBar.addAction(a);
        if (slot != null) a.triggered.connect(this, slot);

        if (shortcut instanceof String)
            a.setShortcut((String) shortcut);
        else if (shortcut instanceof QKeySequence.StandardKey)
            a.setShortcuts((QKeySequence.StandardKey) shortcut);

        return a;
    }

    private void setupFileActions() {
        QToolBar tb = new QToolBar(this);
        tb.setWindowTitle(tr("File Actions"));
        addToolBar(tb);

        QMenu menu = new QMenu(tr("&File"), this);
        menuBar().addMenu(menu);

        action(tr("&New"), "filenew", StandardKey.New, "fileNew()", menu, null);
        action(tr("Open"), "fileopen", StandardKey.Open, "fileOpen()", menu, tb);
        menu.addSeparator();
        actionSave = action(tr("&Save"), "filesave", StandardKey.Save, "fileSave()", menu, tb);
        action(tr("Save &As..."), null, null, "fileSaveAs()", menu, null);
        menu.addSeparator();
        action(tr("&Print"), "fileprint", StandardKey.Print, "filePrint()", menu, tb);
        action(tr("&Export PDF..."), "exportpdf", null, "filePrintPdf()", menu, tb);
        menu.addSeparator();
        action(tr("&Quit"), null, "Ctrl+Q", "close()", menu, null);
    }

    private void setupEditActions() {
        QToolBar b = new QToolBar(this);
        b.setWindowTitle(tr("Edit Actions"));
        addToolBar(b);

        QMenu m = new QMenu(tr("&Edit"), this);
        menuBar().addMenu(m);

        actionUndo = action(tr("&Undo"), "editundo", StandardKey.Undo, null, m, b);
        actionRedo = action(tr("&Redo"), "editredo", StandardKey.Redo, null, m, b);
        actionCut = action(tr("Cu&t"), "editcut", StandardKey.Cut, null, m, b);
        actionCopy = action(tr("&Copy"), "editcopy", StandardKey.Copy, null, m, b);
        actionPaste = action(tr("&Paste"), "editpaste", StandardKey.Paste, null, m, b);

        actionPaste.setEnabled(QApplication.clipboard().text().length() > 0);
    }

    private void updateActionVisuals(QAction a, boolean bold, boolean underline, boolean italic, boolean checkable) {
        QFont font = new QFont();
        font.setBold(bold);
        font.setUnderline(underline);
        font.setItalic(italic);
        a.setFont(font);
        a.setCheckable(checkable);
    }

    private QAction actionGroupEntry(String name, String image, String shortcut, QActionGroup grp) {
        QAction action = new QAction(name, grp);
        action.setIcon(new QIcon(rsrcPath + "/" + image + ".png"));
        action.setCheckable(true);
        return action;
    }

    private void setupTextActions() {
        QToolBar b = new QToolBar(this);
        b.setWindowTitle(tr("Format Actions"));
        addToolBar(b);

        QMenu m = new QMenu(tr("F&ormat"), this);
        menuBar().addMenu(m);

        actionTextBold = action(tr("&Bold"), "textbold", "Ctrl+B", "textBold()", m, b);
        actionTextItalic = action(tr("&Italic"), "textitalic", "Ctrl+I", "textItalic()", m, b);
        actionTextUnderline = action(tr("&Underline"), "textunder", "Ctrl+U", "textUnderline()", m, b);
        m.addSeparator();

        updateActionVisuals(actionTextBold, true, false, false, true);
        updateActionVisuals(actionTextItalic, false, true, false, true);
        updateActionVisuals(actionTextUnderline, false, false, true, true);

        QActionGroup grp = new QActionGroup(this);
        grp.triggered.connect(this, "textAlign(QAction)");

        actionAlignLeft = actionGroupEntry(tr("&Left"), "textleft", "Ctrl+L", grp);
        actionAlignCenter = actionGroupEntry(tr("C&enter"), "textcenter", "Ctrl+E", grp);
        actionAlignRight = actionGroupEntry(tr("&Right"), "textright", "Ctrl+R", grp);
        actionAlignJustify = actionGroupEntry(tr("&Justify"), "textjustify", "Ctrl+J", grp);
        b.addActions(grp.actions());
        m.addActions(grp.actions());

        m.addSeparator();

        QPixmap pix = new QPixmap(16, 16);
        pix.fill(new QColor(org.qtjambi.qt.core.Qt.GlobalColor.black));
        actionTextColor = new QAction(new QIcon(pix), "&Color...", this);
        actionTextColor.triggered.connect(this, "textColor()");
        b.addAction(actionTextColor);
        m.addAction(actionTextColor);

        b = new QToolBar(this);
        b.setAllowedAreas(new Qt.ToolBarAreas(Qt.ToolBarArea.TopToolBarArea, Qt.ToolBarArea.BottomToolBarArea));
        b.setWindowTitle(tr("Format Actions"));
        addToolBarBreak(Qt.ToolBarArea.TopToolBarArea);
        addToolBar(b);

        comboStyle = new QComboBox(b);
        b.addWidget(comboStyle);
        comboStyle.addItem(tr("Standard"), null);
        comboStyle.addItem(tr("Bullet List (Disc)"), null);
        comboStyle.addItem(tr("Bullet List (Circle)"), null);
        comboStyle.addItem(tr("Bullet List (Square)"), null);
        comboStyle.addItem(tr("Ordered List (Decimal)"), null);
        comboStyle.addItem(tr("Ordered List (Alpha lower)"), null);
        comboStyle.addItem(tr("Ordered List (Alpha upper)"), null);
        comboStyle.activatedIndex.connect(this, "textStyle(int)");

        comboFont = new QComboBox(b);
        b.addWidget(comboFont);
        comboFont.setEditable(true);
        QFontDatabase db = new QFontDatabase();
        comboFont.addItems(db.families());
        comboFont.activated.connect(this, "textFamily(String)");
        comboFont.setCurrentIndex(comboFont.findText(QApplication.font()
                .family(), new Qt.MatchFlags(Qt.MatchFlag.MatchExactly, Qt.MatchFlag.MatchCaseSensitive)));

        comboSize = new QComboBox(b);
        comboSize.setObjectName("comboSize");
        b.addWidget(comboSize);
        comboSize.setEditable(true);

        for (int i = 0; i < QFontDatabase.standardSizes().size(); ++i) {
            int size = QFontDatabase.standardSizes().get(i);
            comboSize.addItem("" + size, null);
        }

        comboSize.activated.connect(this, "textSize(String)");
        comboSize.setCurrentIndex(comboSize.findText(""
                + QApplication.font().pointSize(),
                new Qt.MatchFlags(Qt.MatchFlag.MatchExactly, Qt.MatchFlag.MatchCaseSensitive)));
    }

    public boolean load(String f) {
        if (!QFile.exists(f))
            return false;
        QFile file = new QFile(f);
        if (!file.open(new QFile.OpenMode(QFile.OpenModeFlag.ReadOnly)))
            return false;

        QByteArray data = file.readAll();
        file.dispose();
        QTextCodec codec = QTextCodec.codecForHtml(data);
        String str = codec.toUnicode(data);

        if (str.toLowerCase().indexOf("<html") >= 0)
            textEdit.setHtml(str);
        else
            textEdit.setPlainText(str);

        setCurrentFileName(f);
        return true;
    }

    public boolean maybeSave() {
        if (!textEdit.document().isModified())
            return true;
        QMessageBox.StandardButton ret = QMessageBox.warning(this,
                                      tr("Application"),
                                      tr("The document has been modified.\nSave your changes?"),
                                      new QMessageBox.StandardButtons(QMessageBox.StandardButton.Ok, QMessageBox.StandardButton.Discard, QMessageBox.StandardButton.Cancel));
        if (ret == QMessageBox.StandardButton.Yes)
            return fileSave();
        else if (ret == QMessageBox.StandardButton.Cancel)
            return false;
        return true;
    }

    public void setCurrentFileName(String fileName) {
        this.fileName = fileName;
        textEdit.document().setModified(false);

        String shownName;
        if (fileName.length() == 0)
            shownName = "untitled.txt";
        else
            shownName = new QFileInfo(fileName).fileName();

        setWindowTitle(shownName + "[*] - Rich Text");
        setWindowModified(false);
    }

    public void fileNew() {
        if (maybeSave()) {
            textEdit.clear();
            setCurrentFileName("");
        }
    }

    public void fileOpen() {
        String fn = QFileDialog.getOpenFileName(this, tr("Open File..."), "",
                new QFileDialog.Filter(tr("HTML-Files (*.htm *.html);;All Files (*)")));
        if (fn.length() != 0)
            load(fn);
    }

    public boolean fileSave() {
        if (fileName.length() == 0)
            return fileSaveAs();

        QFile file = new QFile(fileName);
        if (!file.open(new QFile.OpenMode(QFile.OpenModeFlag.WriteOnly)))
            return false;
        QTextStream ts = new QTextStream(file);
        QByteArray codecName = new QByteArray("UTF-8");
        ts.setCodec(QTextCodec.codecForName(codecName));
        ts.writeString(textEdit.document().toHtml(codecName));
        textEdit.document().setModified(false);
        file.close();
        file.dispose();
        return true;
    }

    public boolean fileSaveAs() {
        String fn = QFileDialog.getSaveFileName(this, tr("Save as..."), "",
                new QFileDialog.Filter(tr("HTML-Files (*.htm *.html);;All Files (*)")));
        if (fn.length() == 0)
            return false;
        setCurrentFileName(fn);
        return fileSave();
    }

    public void filePrint() {
        QPrinter printer = new QPrinter(QPrinter.PrinterMode.HighResolution);
        printer.setFullPage(true);
        QPrintDialog dlg = new QPrintDialog(printer, this);
        if (dlg.exec() == QDialog.DialogCode.Accepted.value()) {
            textEdit.document().print(printer);
        }
    }

    public void filePrintPdf() {
        String fileName = QFileDialog.getSaveFileName(this, tr("Export PDF"), "",
                new QFileDialog.Filter("*.pdf"));
        if (fileName.length() == 0)
            return;
        QPrinter printer = new QPrinter(QPrinter.PrinterMode.HighResolution);
        printer.setOutputFormat(QPrinter.OutputFormat.PdfFormat);
        printer.setOutputFileName(fileName);
        textEdit.document().print(printer);
    }

    public void textBold() {
        textEdit.setFontWeight(actionTextBold.isChecked()
                ? QFont.Weight.Bold.value()
                : QFont.Weight.Normal.value());
    }

    public void textUnderline() {
        textEdit.setFontUnderline(actionTextUnderline.isChecked());
    }

    public void textItalic() {
        textEdit.setFontItalic(actionTextItalic.isChecked());
    }

    public void textFamily(String f) {
        textEdit.setFontFamily(f);
    }

    public void textSize(String p) {
        textEdit.setFontPointSize(new Float(p));
    }

    public void textStyle(int styleIndex) {
        QTextCursor cursor = textEdit.textCursor();

        if (styleIndex != 0) {
            QTextListFormat.Style style = QTextListFormat.Style.ListDisc;

            switch (styleIndex) {
            default:
            case 1:
                style = QTextListFormat.Style.ListDisc;
                break;
            case 2:
                style = QTextListFormat.Style.ListCircle;
                break;
            case 3:
                style = QTextListFormat.Style.ListSquare;
                break;
            case 4:
                style = QTextListFormat.Style.ListDecimal;
                break;
            case 5:
                style = QTextListFormat.Style.ListLowerAlpha;
                break;
            case 6:
                style = QTextListFormat.Style.ListUpperAlpha;
                break;
            }

            cursor.beginEditBlock();

            QTextBlockFormat blockFmt = cursor.blockFormat();

            QTextListFormat listFmt = new QTextListFormat();

            if (cursor.currentList() != null) {
                listFmt = cursor.currentList().textListFormat();
            } else {
                listFmt.setIndent(blockFmt.indent() + 1);
                blockFmt.setIndent(0);
                cursor.setBlockFormat(blockFmt);
            }

            listFmt.setStyle(style);

            cursor.createList(listFmt);

            cursor.endEditBlock();
        } else {
            QTextBlockFormat bfmt = new QTextBlockFormat();
            bfmt.setObjectIndex(-1);
            cursor.mergeBlockFormat(bfmt);
        }
    }

    public void textColor() {
        QColor col = QColorDialog.getColor(textEdit.textColor(), this);
        if (!col.isValid())
            return;
        textEdit.setTextColor(col);
        colorChanged(col);
    }

    public void textAlign(QAction a) {
        if (a == actionAlignLeft)
            textEdit.setAlignment(new Qt.Alignment(Qt.AlignmentFlag.AlignLeft));
        else if (a == actionAlignCenter)
            textEdit.setAlignment(new Qt.Alignment(Qt.AlignmentFlag.AlignHCenter));
        else if (a == actionAlignRight)
            textEdit.setAlignment(new Qt.Alignment(Qt.AlignmentFlag.AlignRight));
        else if (a == actionAlignJustify)
            textEdit.setAlignment(new Qt.Alignment(Qt.AlignmentFlag.AlignJustify));
    }

    public void currentCharFormatChanged(QTextCharFormat format) {
        fontChanged(format.font());
        colorChanged(format.foreground().color());
        alignmentChanged(textEdit.alignment());
    }

    public void clipboardDataChanged() {
        actionPaste.setEnabled(QApplication.clipboard().text().length() > 0);
    }

    public void fontChanged(QFont f) {
        comboFont.setCurrentIndex(comboFont.findText(f.family(),
                new Qt.MatchFlags(Qt.MatchFlag.MatchExactly, Qt.MatchFlag.MatchCaseSensitive)));
        comboSize.setCurrentIndex(comboSize.findText(Integer.toString(f.pointSize()),
                new Qt.MatchFlags(Qt.MatchFlag.MatchExactly, Qt.MatchFlag.MatchCaseSensitive)));
        actionTextBold.setChecked(f.bold());
        actionTextItalic.setChecked(f.italic());
        actionTextUnderline.setChecked(f.underline());
    }

    public void colorChanged(QColor c) {
        QPixmap pix = new QPixmap(16, 16);
        pix.fill(c);
        actionTextColor.setIcon(new QIcon(pix));
    }

    public void alignmentChanged(Qt.Alignment a) {
        if (a.isSet(Qt.AlignmentFlag.AlignLeft))
            actionAlignLeft.setChecked(true);
        else if (a.isSet(Qt.AlignmentFlag.AlignHCenter))
            actionAlignCenter.setChecked(true);
        else if (a.isSet(Qt.AlignmentFlag.AlignRight))
            actionAlignRight.setChecked(true);
        else if (a.isSet(Qt.AlignmentFlag.AlignJustify))
            actionAlignJustify.setChecked(true);
    }

    private String fileName;
    private QTextEdit textEdit;
    private QAction actionSave;
    private QAction actionUndo;
    private QAction actionRedo;
    private QAction actionCut;
    private QAction actionCopy;
    private QAction actionPaste;
    private QAction actionTextBold;
    private QAction actionTextItalic;
    private QAction actionTextUnderline;
    private QAction actionTextColor;
    private QAction actionAlignLeft;
    private QAction actionAlignRight;
    private QAction actionAlignCenter;
    private QAction actionAlignJustify;
    private QComboBox comboStyle;
    private QComboBox comboFont;
    private QComboBox comboSize;
    private String rsrcPath = "classpath:org/qtjambi/images/textedit/win";

    static public void main(String args[]) {
        QApplication.initialize(args);

        TextEdit mw = new TextEdit();
        if (args.length > 0) {
            mw.load(args[0]);
        }
        mw.show();

        QApplication.execStatic();
        QApplication.shutdown();
    }
}
