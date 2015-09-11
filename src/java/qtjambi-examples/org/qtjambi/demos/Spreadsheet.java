/****************************************************************************
 **
 ** Copyright (C) 2007-2009 Nokia. All rights reserved.
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

import java.util.Enumeration;
import java.util.Hashtable;
import java.util.Iterator;
import java.util.List;

import org.qtjambi.examples.QtJambiExample;
import org.qtjambi.qt.core.QAbstractTableModel;
import org.qtjambi.qt.core.QFile;
import org.qtjambi.qt.core.QFileInfo;
import org.qtjambi.qt.core.QIODevice.OpenMode;
import org.qtjambi.qt.core.QIODevice.OpenModeFlag;
import org.qtjambi.qt.core.QItemSelectionModel;
import org.qtjambi.qt.core.QModelIndex;
import org.qtjambi.qt.core.QRect;
import org.qtjambi.qt.core.QXmlStreamAttribute;
import org.qtjambi.qt.core.QXmlStreamAttributes;
import org.qtjambi.qt.core.QXmlStreamReader;
import org.qtjambi.qt.core.QXmlStreamReader.TokenType;
import org.qtjambi.qt.core.QXmlStreamWriter;
import org.qtjambi.qt.core.Qt;
import org.qtjambi.qt.core.Qt.ItemFlag;
import org.qtjambi.qt.core.Qt.ItemFlags;
import org.qtjambi.qt.core.Qt.Orientation;
import org.qtjambi.qt.gui.QColor;
import org.qtjambi.qt.gui.QFont;
import org.qtjambi.qt.gui.QFontDatabase;
import org.qtjambi.qt.gui.QIcon;
import org.qtjambi.qt.gui.QKeySequence;
import org.qtjambi.qt.gui.QKeySequence.StandardKey;
import org.qtjambi.qt.gui.QMouseEvent;
import org.qtjambi.qt.gui.QPainter;
import org.qtjambi.qt.gui.QPen;
import org.qtjambi.qt.gui.QPixmap;
import org.qtjambi.qt.printsupport.QPrintDialog;
import org.qtjambi.qt.printsupport.QPrinter;
import org.qtjambi.qt.widgets.QAction;
import org.qtjambi.qt.widgets.QActionGroup;
import org.qtjambi.qt.widgets.QApplication;
import org.qtjambi.qt.widgets.QColorDialog;
import org.qtjambi.qt.widgets.QComboBox;
import org.qtjambi.qt.widgets.QDialog;
import org.qtjambi.qt.widgets.QFileDialog;
import org.qtjambi.qt.widgets.QItemDelegate;
import org.qtjambi.qt.widgets.QLabel;
import org.qtjambi.qt.widgets.QLineEdit;
import org.qtjambi.qt.widgets.QMainWindow;
import org.qtjambi.qt.widgets.QMenu;
import org.qtjambi.qt.widgets.QMessageBox;
import org.qtjambi.qt.widgets.QStyleOptionViewItem;
import org.qtjambi.qt.widgets.QTableView;
import org.qtjambi.qt.widgets.QToolBar;
import org.qtjambi.qt.widgets.QWidget;

@QtJambiExample(name = "Spreadsheet")
public class Spreadsheet extends QMainWindow {

    public static void main(String[] args) {
        QApplication.initialize(args);

        Spreadsheet spread = new Spreadsheet();
        spread.show();

        QApplication.execStatic();
        QApplication.shutdown();
    }

    private TableView view;
    private TableModel model;
    private QLabel cellLabel = new QLabel("Cell:   ");
    private QLineEdit cellEdit = new QLineEdit();
    private QAction actionTextBold;
    private QAction actionTextItalic;
    private QAction actionTextUnderline;

    private QAction actionAlignLeft;
    private QAction actionAlignRight;
    private QAction actionAlignCenter;
    private QAction actionAlignJustify;

    private QAction actionTextColor;
    private QAction actionCellColor;

    private String fileName;

    public Spreadsheet() {
        setWindowIcon(new QIcon("classpath:org/qtjambi/images/qt-logo.png"));
        QToolBar toolBar = new QToolBar(this);
        toolBar.addWidget(cellLabel);
        toolBar.addWidget(cellEdit);
        addToolBar(toolBar);

        setupFileActions();
        setupCellActions();
        setupHelpMenu();

        view = new TableView(this);
        view.setEnabled(true);
        cellEdit.editingFinished.connect(view, "updateFromCellEdit()");
        model = new TableModel();
        view.setModel(model);
        setCentralWidget(view);
        statusBar().showMessage("Ready");
        statusBar().messageChanged.connect(this, "updateStatusBar(String)");

        fileOpen("classpath:org/qtjambi/demos/spreadsheet/demo.xml");
        // fileNew();
    }

    private QAction action(String name, String image, Object shortcut, String slot, QMenu menu, QToolBar toolBar) {
        QAction a = new QAction(name, this);

        if (image != null) {
            if (image.startsWith("border"))
                a.setIcon(new QIcon("classpath:org/qtjambi/demos/spreadsheet/" + image + ".png"));
            else
                a.setIcon(new QIcon("classpath:org/qtjambi/images/textedit/win/" + image + ".png"));
        }
        if (menu != null)
            menu.addAction(a);
        if (toolBar != null)
            toolBar.addAction(a);
        if (slot != null)
            a.triggered.connect(this, slot);

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
        action(tr("&Save"), "filesave", StandardKey.Save, "fileSave()", menu, tb);
        action(tr("Save &As..."), null, null, "fileSaveAs()", menu, null);
        menu.addSeparator();
        action(tr("&Print"), "fileprint", StandardKey.Print, "filePrint()", menu, tb);
        action(tr("&Export PDF..."), "exportpdf", null, "filePrintPdf()", menu, tb);
        menu.addSeparator();
        action(tr("&Quit"), null, "Ctrl+Q", "close()", menu, null);
    }

    private void setupHelpMenu() {
        QMenu helpMenu = new QMenu(tr("&Help"), this);
        menuBar().addMenu(helpMenu);

        QAction aboutAct = new QAction(tr("&About"), this);
        aboutAct.triggered.connect(this, "about()");
        helpMenu.addAction(aboutAct);

        QAction aboutQtJambiAct = new QAction(tr("About &Qt Jambi"), this);
        aboutQtJambiAct.triggered.connect(QApplication.instance(), "aboutQtJambi()");
        helpMenu.addSeparator();
        helpMenu.addAction(aboutQtJambiAct);

        QAction aboutQtAct = new QAction(tr("About Q&t"), this);
        aboutQtAct.triggered.connect(QApplication.instance(), "aboutQt()");
        helpMenu.addAction(aboutQtAct);
    }

    public void about() {
        QMessageBox.about(this, tr("About Spreadsheet Demo"), tr("<p>The <b>Spreadsheet Demo</b> shows how a simple spreadsheet application can be done "
                + "using Qt Jambi.</p>"));
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
        action.setIcon(new QIcon("classpath:org/qtjambi/images/textedit/win/" + image + ".png"));
        action.setCheckable(true);
        return action;
    }

    private void setupCellActions() {
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

        actionTextColor = new QAction(createPixmapForButton(tr("T"), new QColor(org.qtjambi.qt.core.Qt.GlobalColor.black), new QColor(org.qtjambi.qt.core.Qt.GlobalColor.transparent)), "&Color...", this);
        actionTextColor.triggered.connect(this, "textColor()");
        b.addAction(actionTextColor);
        m.addAction(actionTextColor);

        actionCellColor = new QAction(createPixmapForButton(tr("T"), new QColor(org.qtjambi.qt.core.Qt.GlobalColor.white), new QColor(org.qtjambi.qt.core.Qt.GlobalColor.black)), "&Color...", this);
        actionCellColor.triggered.connect(this, "backgroundColor()");
        b.addAction(actionCellColor);
        m.addAction(actionCellColor);

        b = new QToolBar(this);
        b.setAllowedAreas(new Qt.ToolBarAreas(Qt.ToolBarArea.TopToolBarArea, Qt.ToolBarArea.BottomToolBarArea));
        b.setWindowTitle(tr("Border Actions"));
        addToolBarBreak(Qt.ToolBarArea.TopToolBarArea);
        addToolBar(b);

        action(tr("&Around"), "borderaround", "Ctrl+A", "borderAround()", m, b);
        action(tr("Border &Clear"), "borderclear", "Ctrl+C", "borderClear()", m, b);
        action(tr("&Left"), "borderleft", "Ctrl+L", "borderLeft()", m, b);
        action(tr("&Right"), "borderright", "Ctrl+R", "borderRight()", m, b);
        action(tr("&Top"), "bordertop", "Ctrl+T", "borderTop()", m, b);
        action(tr("&Bottom"), "borderbottom", "Ctrl+B", "borderBottom()", m, b);

        m.addSeparator();

        b = new QToolBar(this);
        b.setAllowedAreas(new Qt.ToolBarAreas(Qt.ToolBarArea.TopToolBarArea, Qt.ToolBarArea.BottomToolBarArea));
        b.setWindowTitle(tr("Format Actions"));
        addToolBar(b);

        QComboBox comboFont = new QComboBox(b);
        b.addWidget(comboFont);
        comboFont.setEditable(true);
        QFontDatabase db = new QFontDatabase();
        comboFont.addItems(db.families());
        comboFont.activated.connect(this, "textFamily(String)");
        comboFont.setCurrentIndex(comboFont.findText(QApplication.font().family(),
                new Qt.MatchFlags(Qt.MatchFlag.MatchExactly, Qt.MatchFlag.MatchCaseSensitive)));

        QComboBox comboSize = new QComboBox(b);
        comboSize.setObjectName("comboSize");
        b.addWidget(comboSize);
        comboSize.setEditable(true);

        for (int i = 0; i < QFontDatabase.standardSizes().size(); ++i) {
            int size = QFontDatabase.standardSizes().get(i);
            comboSize.addItem("" + size, null);
        }

        comboSize.activated.connect(this, "textSize(String)");
        comboSize.setCurrentIndex(comboSize.findText("" + QApplication.font().pointSize(), new Qt.MatchFlags(Qt.MatchFlag.MatchExactly,
                Qt.MatchFlag.MatchCaseSensitive)));

        m.addSeparator();

        b = new QToolBar(this);
        b.setAllowedAreas(new Qt.ToolBarAreas(Qt.ToolBarArea.TopToolBarArea, Qt.ToolBarArea.BottomToolBarArea));
        b.setWindowTitle(tr("Function Actions"));
        addToolBar(b);

        action(tr("&Sum"), null, "", "sum()", m, b);
    }

    private QIcon createPixmapForButton(String text, QColor front, QColor back) {
        QPixmap pix = new QPixmap(32, 32);
        pix.fill(back);
        QPainter painter = new QPainter(pix);
        QFont font = new QFont();
        font.setPixelSize(32);
        font.setBold(true);
        painter.setFont(font);
        painter.setPen(front);
        painter.drawText(4, 28, tr(text));
        painter.end();
        return new QIcon(pix);
    }

    protected void applyToCurrentSelection(Cell.Property prop, Object value) {
        List<QModelIndex> selectedList = view.selectionModel().selectedIndexes();
        for (Iterator<QModelIndex> iterator = selectedList.iterator(); iterator.hasNext();) {
            applyToCell(iterator.next(), prop, value);
        }
    }

    protected void applyToCell(QModelIndex index, Cell.Property prop, Object value) {
        Cell cell = model.get(index);
        if (cell == null)
            cell = model.put(index, "");

        cell.apply(prop, value);
        view.update(index);
    }

    protected void applyBorderToCurrentSelection(Cell.Property prop, Object value) {
        QItemSelectionModel selection = view.selectionModel();
        List<QModelIndex> selectedList = selection.selectedIndexes();
        for (Iterator<QModelIndex> iterator = selectedList.iterator(); iterator.hasNext();) {
            QModelIndex index = iterator.next();
            int r = index.row();
            int c = index.column();

            boolean left = selectedList.contains(model.index(r, c - 1));
            boolean right = selectedList.contains(model.index(r, c + 1));
            boolean top = selectedList.contains(model.index(r - 1, c));
            boolean bottom = selectedList.contains(model.index(r + 1, c));

            boolean all = prop == Cell.Property.BorderAround;
            if (!left && (all || prop == Cell.Property.BorderLeft))
                applyToCell(index, Cell.Property.BorderLeft, value);
            if (!right && (all || prop == Cell.Property.BorderRight))
                applyToCell(index, Cell.Property.BorderRight, value);
            if (!top && (all || prop == Cell.Property.BorderTop))
                applyToCell(index, Cell.Property.BorderTop, value);
            if (!bottom && (all || prop == Cell.Property.BorderBottom))
                applyToCell(index, Cell.Property.BorderBottom, value);
        }
    }

    protected void textAlign(QAction a) {
        Qt.AlignmentFlag flag = null;
        if (a == actionAlignCenter) {
            flag = Qt.AlignmentFlag.AlignHCenter;
        } else if (a == actionAlignLeft) {
            flag = Qt.AlignmentFlag.AlignLeft;
        } else if (a == actionAlignRight) {
            flag = Qt.AlignmentFlag.AlignRight;
        } else if (a == actionAlignJustify) {
            flag = Qt.AlignmentFlag.AlignJustify;
        }
        applyToCurrentSelection(Cell.Property.Align, flag);
    }

    protected void borderAround() {
        applyBorderToCurrentSelection(Cell.Property.BorderLeft, true);
        applyBorderToCurrentSelection(Cell.Property.BorderRight, true);
        applyBorderToCurrentSelection(Cell.Property.BorderTop, true);
        applyBorderToCurrentSelection(Cell.Property.BorderBottom, true);
    }

    protected void borderClear() {
        applyBorderToCurrentSelection(Cell.Property.BorderLeft, false);
        applyBorderToCurrentSelection(Cell.Property.BorderRight, false);
        applyBorderToCurrentSelection(Cell.Property.BorderTop, false);
        applyBorderToCurrentSelection(Cell.Property.BorderBottom, false);
    }

    protected void borderLeft() {
        applyBorderToCurrentSelection(Cell.Property.BorderLeft, true);
    }

    protected void borderRight() {
        applyBorderToCurrentSelection(Cell.Property.BorderRight, true);
    }

    protected void borderTop() {
        applyBorderToCurrentSelection(Cell.Property.BorderTop, true);
    }

    protected void borderBottom() {
        applyBorderToCurrentSelection(Cell.Property.BorderBottom, true);
    }

    protected void textFamily(String font) {
        applyToCurrentSelection(Cell.Property.FontName, font);
    }

    protected void sum() {
        view.selectionFunction("sum");
    }

    protected void fileOpen() {
        fileOpen(QFileDialog.getOpenFileName(this, tr("Open File..."), "", new QFileDialog.Filter(tr("XML-Files (*.xml);;All Files (*)"))));
    }

    protected void fileOpen(String name) {
        if (name.length() == 0) {
            return;
        }
        model.data.clear();
        QFile file = new QFile(name);
        if (file.open(new OpenMode(OpenModeFlag.ReadOnly))) {
            QXmlStreamReader xml = new QXmlStreamReader(file);

            while (!xml.atEnd()) {
                TokenType type = xml.readNext();
                switch (type) {
                case StartDocument:
                case EndDocument:
                    break;

                case StartElement:
                    if (xml.name().equalsIgnoreCase(Cell.class.getSimpleName())) {
                        Cell cell = new Cell(xml);
                        model.put(cell);
                    } else if (xml.name().equalsIgnoreCase("doc")) {

                    } else {
                        throw new RuntimeException("unknown element found");
                    }
                    break;

                default:
                    break;
                }

            }
            if (xml.hasError()) {
                throw new RuntimeException(xml.errorString());
            }
            file.close();
            setCurrentFileName(name);
        } else {
            QMessageBox.warning(this, tr("Open failed"), tr("Could not open file: " + name));
        }
    }

    protected void fileNew() {
        model.data.clear();
        setCurrentFileName("");
    }

    protected boolean fileSave() {
        if (fileName.length() == 0)
            return fileSaveAs();

        QFile file = new QFile(fileName);

        if (file.open(new OpenMode(OpenModeFlag.WriteOnly))) {
            QXmlStreamWriter xml = new QXmlStreamWriter(file);
            xml.setAutoFormatting(true);

            xml.writeStartDocument();
            xml.writeStartElement("doc");
            Enumeration<Cell> e = model.data.elements();
            while (e.hasMoreElements()) {
                e.nextElement().writeXml(xml);
            }
            xml.writeEndElement();
            xml.writeEndDocument();

            file.close();
            statusBar().showMessage(tr("saved file..."), 5000);
            return true;
        } else {
            QMessageBox.warning(this, tr("Save failed"), tr("Could not open file for writing."));
            return false;
        }
    }

    protected boolean fileSaveAs() {
        String fn = QFileDialog.getSaveFileName(this, tr("Save as..."), "", new QFileDialog.Filter(tr("XML-Files (*.xml);;All Files (*)")));
        if (fn.length() == 0)
            return false;
        setCurrentFileName(fn);
        return fileSave();
    }

    public void setCurrentFileName(String fileName) {
        this.fileName = fileName;

        String shownName;
        if (fileName.length() == 0)
            shownName = "untitled.txt";
        else
            shownName = new QFileInfo(fileName).fileName();

        setWindowTitle(shownName);
    }

    protected void filePrint() {
        QPrinter printer = new QPrinter(QPrinter.PrinterMode.ScreenResolution);
        printer.setFullPage(true);
        QPrintDialog dlg = new QPrintDialog(printer, this);
        if (dlg.exec() == QDialog.DialogCode.Accepted.value()) {
            view.viewport().render(printer);
        }
    }

    protected void filePrintPdf() {
        String fileName = QFileDialog.getSaveFileName(this, tr("Export PDF"), "", new QFileDialog.Filter("*.pdf"));
        if (fileName.length() == 0)
            return;
        QPrinter printer = new QPrinter(QPrinter.PrinterMode.ScreenResolution);
        printer.setOutputFormat(QPrinter.OutputFormat.PdfFormat);
        printer.setOutputFileName(fileName);
        view.viewport().render(printer);
    }

    protected void textColor() {
        Cell cell = model.get(view.currentIndex());

        QColor col = QColorDialog.getColor(cell != null ? cell.textColor : null, this);
        if (!col.isValid())
            return;
        applyToCurrentSelection(Cell.Property.TextColor, col);
    }

    protected void backgroundColor() {
        Cell cell = model.get(view.currentIndex());

        QColor col = QColorDialog.getColor(cell != null ? cell.textColor : null, this);
        if (!col.isValid())
            return;
        applyToCurrentSelection(Cell.Property.BackgroundColor, col);
    }

    protected void textBold() {
        applyToCurrentSelection(Cell.Property.Bold, actionTextBold.isChecked());
    }

    protected void textUnderline() {
        applyToCurrentSelection(Cell.Property.Underscore, actionTextUnderline.isChecked());
    }

    protected void textItalic() {
        applyToCurrentSelection(Cell.Property.Italic, actionTextItalic.isChecked());
    }

    protected void textSize(String sizeString) {
        try {
            int size = Integer.parseInt(sizeString);
            applyToCurrentSelection(Cell.Property.FontSize, size);
        } catch (NumberFormatException e) {
        }
    }

    protected void updateStatusBar(String message) {
        if (message.equals("")) {
            statusBar().showMessage("Ready");
        }
    }

    static class Cell {

        enum Property {
            TextColor, BackgroundColor, Bold, Italic, Underscore, FontName, FontSize, Align, BorderLeft, BorderTop, BorderRight, BorderBottom, BorderAround
        };

        int col;
        int row;
        boolean borderTop;
        boolean borderLeft;
        boolean borderBottom;
        boolean borderRight;

        Object value = "";
        Qt.AlignmentFlag align;
        QColor backgroundColor;
        QColor textColor;
        private QFont font;

        Cell(Object value) {
            this.value = value;
        }

        Cell(QXmlStreamReader xml) {
            {
                QXmlStreamAttributes attributes = xml.attributes();
                for (int i = 0; i < attributes.size(); i++) {
                    QXmlStreamAttribute attrib = attributes.at(i);
                    String name = attrib.name();
                    String value = attrib.value();

                    if (name.equalsIgnoreCase("col")) {
                        col = Integer.parseInt(value);
                    } else if (name.equalsIgnoreCase("row")) {
                        row = Integer.parseInt(value);
                    } else if (name.equalsIgnoreCase("align")) {
                        if (value.equalsIgnoreCase(Qt.AlignmentFlag.AlignLeft.name()))
                            align = Qt.AlignmentFlag.AlignLeft;
                        else if (value.equalsIgnoreCase(Qt.AlignmentFlag.AlignRight.name()))
                            align = Qt.AlignmentFlag.AlignRight;
                        else if (value.equalsIgnoreCase(Qt.AlignmentFlag.AlignHCenter.name()))
                            align = Qt.AlignmentFlag.AlignHCenter;
                        else if (value.equalsIgnoreCase(Qt.AlignmentFlag.AlignJustify.name()))
                            align = Qt.AlignmentFlag.AlignJustify;

                    } else if (name.equalsIgnoreCase("font")) {
                        font = new QFont();
                        font.fromString(value);
                    } else if (name.equalsIgnoreCase("border")) {
                        borderTop = value.contains("top");
                        borderBottom = value.contains("bottom");
                        borderLeft = value.contains("left");
                        borderRight = value.contains("right");
                    }
                }
            }

            while (!xml.atEnd()) {
                TokenType type = xml.readNext();
                switch (type) {

                case Characters:
                    if (xml.isCDATA())
                        value = value + xml.text();
                    break;

                case StartElement:
                    if (xml.name().equalsIgnoreCase(Property.TextColor.name()) || xml.name().equalsIgnoreCase(Property.BackgroundColor.name())) {
                        QColor color = new QColor();
                        QXmlStreamAttributes attributes = xml.attributes();
                        for (int i = 0; i < attributes.size(); i++) {
                            QXmlStreamAttribute attrib = attributes.at(i);
                            String name = attrib.name();
                            String value = attrib.value();
                            if (name.equalsIgnoreCase("r")) {
                                color.setRed(Integer.parseInt(value));
                            } else if (name.equalsIgnoreCase("g")) {
                                color.setGreen(Integer.parseInt(value));
                            } else if (name.equalsIgnoreCase("b")) {
                                color.setBlue(Integer.parseInt(value));
                            } else if (name.equalsIgnoreCase("a")) {
                                color.setAlpha(Integer.parseInt(value));
                            }
                        }
                        if (xml.name().equalsIgnoreCase(Property.TextColor.name()))
                            this.textColor = color;
                        else
                            this.backgroundColor = color;

                    } else {
                        throw new RuntimeException("unknown element found: " + xml.name());
                    }
                    break;

                case EndElement:
                    if (xml.name().equalsIgnoreCase(Cell.class.getSimpleName())) {
                        return;
                    }
                    break;

                default:
                    break;
                }

            }
            if (xml.hasError()) {
                throw new RuntimeException(xml.errorString());
            }

        }

        public void apply(Property prop, Object value) {
            switch (prop) {
            case TextColor:
                textColor = (QColor) value;
                break;

            case BackgroundColor:
                backgroundColor = (QColor) value;
                break;

            case Bold:
                font().setBold((Boolean) value);
                break;

            case Italic:
                font().setItalic((Boolean) value);
                break;

            case Underscore:
                font().setUnderline((Boolean) value);
                break;

            case FontName:
                font().setFamily((String) value);
                break;

            case FontSize:
                font().setPointSize((Integer) value);
                break;

            case Align:
                align = ((Qt.AlignmentFlag) value);
                break;

            case BorderTop:
                borderTop = ((Boolean) value);
                break;

            case BorderLeft:
                borderLeft = ((Boolean) value);
                break;

            case BorderRight:
                borderRight = ((Boolean) value);
                break;

            case BorderBottom:
                borderBottom = ((Boolean) value);
                break;

            default:
                break;
            }
        }

        @Override
        public String toString() {
            return value.toString();
        }

        public QFont font() {
            if (font == null)
                font = new QFont();
            return font;
        }

        protected void writeXml(QXmlStreamWriter xml) {
            xml.writeStartElement(Cell.class.getSimpleName());
            xml.writeAttribute("col", col + "");
            xml.writeAttribute("row", row + "");
            if (align != null) {
                xml.writeAttribute("align", align.name());
            }
            if (font != null) {
                xml.writeAttribute("font", font.toString());
            }
            if (borderBottom || borderTop || borderLeft || borderRight) {
                xml.writeAttribute("border", (borderBottom ? "bottom " : "") + (borderTop ? "top " : "") + (borderRight ? "right " : "")
                        + (borderLeft ? "left " : ""));
            }
            if (textColor != null) {
                writeXmlColor(xml, Property.TextColor.name(), textColor);
            }
            if (backgroundColor != null) {
                writeXmlColor(xml, Property.BackgroundColor.name(), backgroundColor);
            }
            xml.writeCDATA(value.toString());
            xml.writeEndElement();
        }

        private void writeXmlColor(QXmlStreamWriter xml, String name, QColor color) {
            xml.writeStartElement(name);
            xml.writeAttribute("r", color.red() + "");
            xml.writeAttribute("g", color.green() + "");
            xml.writeAttribute("b", color.blue() + "");
            xml.writeAttribute("a", color.alpha() + "");
            xml.writeEndElement();
        }
    }

    class CellDelegate extends QItemDelegate {

        QPen penBorder = new QPen();
        QPen penNormal = new QPen();

        public CellDelegate() {
            penBorder.setWidth(2);
            penBorder.setColor(new QColor(org.qtjambi.qt.core.Qt.GlobalColor.darkBlue));

            penNormal.setColor(new QColor(org.qtjambi.qt.core.Qt.GlobalColor.lightGray));
        }

        @Override
        public void paint(QPainter painter, QStyleOptionViewItem option, QModelIndex index) {
            painter.save();
            super.paint(painter, option, index);
            QRect rect = option.rect();
            Cell cell = model.get(index);
            painter.setPen(penNormal);
            painter.drawRect(rect);
            rect.adjust(1, 1, -1, -1);

            if (cell != null) {

                painter.setPen(penBorder);
                if (cell.borderLeft)
                    painter.drawLine(rect.topLeft(), rect.bottomLeft());
                if (cell.borderTop)
                    painter.drawLine(rect.topLeft(), rect.topRight());
                if (cell.borderRight)
                    painter.drawLine(rect.topRight(), rect.bottomRight());
                if (cell.borderBottom)
                    painter.drawLine(rect.bottomLeft(), rect.bottomRight());
            }
            painter.restore();
        }
    }

    class TableView extends QTableView {
        private CellDelegate cellDelegate = new CellDelegate();

        TableView(QWidget parent) {
            super.parent();
            setItemDelegate(cellDelegate);
            setShowGrid(false);
        }

        public void updateFromCellEdit() {
            QModelIndex index = currentIndex();
            if (index != null) {
                model.setData(index, cellEdit.text());
                update(index);
            }
        }

        @Override
        protected void mouseReleaseEvent(QMouseEvent event) {
            if (selectState) {
                selectState = false;

            }
            super.mouseReleaseEvent(event);
        }

        boolean selectState;
        QModelIndex selectStateCurrent;
        String selectStateFunction;

        public void selectionFunction(String fun) {
            if (selectState)
                return;
            selectStateCurrent = view.currentIndex();
            if (selectStateCurrent == null)
                return;
            selectState = true;
            selectStateFunction = fun;
        }

        @Override
        protected void currentChanged(QModelIndex current, QModelIndex previous) {
            if (selectState) {
                QModelIndex topLeft = null;
                QModelIndex bottomRight = null;
                for (Iterator<QModelIndex> iterator = selectedIndexes().iterator(); iterator.hasNext();) {
                    QModelIndex index = (QModelIndex) iterator.next();
                    if (index.equals(selectStateCurrent))
                        continue;
                    if (topLeft == null) {
                        topLeft = index;
                        bottomRight = index;
                    } else {
                        if (topLeft.column() > index.column() || (topLeft.column() == index.column() && topLeft.row() > index.row()))
                            topLeft = index;
                        else if (bottomRight.column() < index.column() || (bottomRight.column() == index.column() && bottomRight.row() < index.row()))
                            bottomRight = index;
                    }
                }
                if (topLeft != null) {
                    model.put(selectStateCurrent, selectStateFunction + "(" + Util.convertIndex(topLeft) + " " + Util.convertIndex(bottomRight) + ")");
                }
                view.update(selectStateCurrent);
            }

            cellLabel.setText("Cell: " + Util.convertIndex(current) + "  ");

            boolean enabled = false;
            Cell cell = null;
            if (current != null) {
                cell = model.get(current);
                if (cell != null) {
                    enabled = true;
                }
            }
            /*
             * actionAlignCenter.setEnabled(enabled);
             * actionAlignLeft.setEnabled(enabled);
             * actionAlignRight.setEnabled(enabled);
             * actionAlignJustify.setEnabled(enabled);
             * actionTextBold.setEnabled(enabled);
             * actionTextUnderline.setEnabled(enabled);
             * actionTextItalic.setEnabled(enabled);
             */
            if (enabled) {
                cellEdit.setText(cell.toString());
                actionAlignCenter.setChecked(cell.align != null && cell.align == Qt.AlignmentFlag.AlignCenter);
                actionAlignLeft.setChecked(cell.align != null && cell.align == Qt.AlignmentFlag.AlignLeft);
                actionAlignRight.setChecked(cell.align != null && cell.align == Qt.AlignmentFlag.AlignRight);
                actionAlignJustify.setChecked(cell.align != null && cell.align == Qt.AlignmentFlag.AlignJustify);
                QFont font = cell.font;
                actionTextBold.setChecked(font != null && font.bold());
                actionTextUnderline.setChecked(font != null && font.underline());
                actionTextItalic.setChecked(font != null && font.italic());

                actionTextColor.setIcon(createPixmapForButton(tr("T"), cell.textColor != null ? cell.textColor : new QColor(org.qtjambi.qt.core.Qt.GlobalColor.black), new QColor(org.qtjambi.qt.core.Qt.GlobalColor.transparent)));
                actionCellColor.setIcon(createPixmapForButton(tr("T"), new QColor(org.qtjambi.qt.core.Qt.GlobalColor.white), cell.backgroundColor != null ? cell.backgroundColor : new QColor(org.qtjambi.qt.core.Qt.GlobalColor.black)));
            } else {
                cellEdit.setText("");
            }
        }

    }

    class TableModel extends QAbstractTableModel {

        private SpreadsheetInterpreter interpreter = new SpreadsheetInterpreter();

        public Hashtable<String, Cell> data = new Hashtable<String, Cell>();

        public Object get(int col, int row) {
            return data.get(col + "x" + row);
        }

        public Cell get(QModelIndex i) {
            return data.get(i.column() + "x" + i.row());
        }

        public Cell put(QModelIndex i, Object value) {
            Cell cell = get(i);
            if (cell == null) {
                cell = new Cell(value);
                cell.col = i.column();
                cell.row = i.row();
                data.put(i.column() + "x" + i.row(), cell);
            } else {
                cell.value = value;
            }
            return cell;
        }

        public void put(Cell cell) {
            data.put(cell.col + "x" + cell.row, cell);
            QModelIndex index = index(cell.col, cell.row);
            dataChanged.emit(index, index);
        }

        @Override
        public int columnCount(QModelIndex parent) {
            return 100;
        }

        @Override
        public int rowCount(QModelIndex parent) {
            return 100;
        }

        @Override
        public Object data(final QModelIndex index, int role) {
            Cell cell = get(index);
            if (cell == null)
                return null;
            boolean empty = cell.value.equals("");

            switch (role) {
            case Qt.ItemDataRole.DisplayRole:
                try {
                    if (!empty)
                        return interpreter.parseAndEvaluate(get(index).value);
                    return null;
                } catch (final Interpreter.ParseException e) {
                    QApplication.invokeLater(new Runnable() {
                        public void run() {
                            statusBar().showMessage("Error (" + Util.convertIndex(index) + "): " + e.getMessage(), 5000);
                        }
                    });
                    return "Error:" + e.getMessage();
                }

            case Qt.ItemDataRole.EditRole:
                if (!empty)
                    return get(index).value;

            case Qt.ItemDataRole.FontRole:
                if (!empty)
                    return cell.font;
                return null;

            case Qt.ItemDataRole.BackgroundRole:
                return cell.backgroundColor;  // we're already checked that cell != null

            case Qt.ItemDataRole.TextAlignmentRole:
                if (!empty)
                    return cell.align;

            case Qt.ItemDataRole.ForegroundRole:
                if (!empty)
                    return cell.textColor;
                return null;

            case Qt.ItemDataRole.ToolTipRole:
                return get(index);

            default:
                break;
            }
            return null;
        }

        @Override
        public Object headerData(int section, Orientation orientation, int role) {
            switch (role) {
            case Qt.ItemDataRole.DisplayRole:
                if (orientation == Orientation.Horizontal)
                    return Util.convertColumn(section);
                else
                    return section + 1;
            }
            return null;
        }

        @Override
        public boolean setData(QModelIndex index, Object value, int role) {
            try {
                put(index, Double.parseDouble(value.toString()));
                return true;
            } catch (Exception e) {
            }
            put(index, value);
            return true;
        }

        @Override
        public ItemFlags flags(QModelIndex index) {
            return new ItemFlags(ItemFlag.ItemIsEditable, ItemFlag.ItemIsSelectable, ItemFlag.ItemIsEnabled);
        }
    }

    class SpreadsheetInterpreter extends Interpreter {
        private SpreadsheetInterpreter() {
            Function function = new Function("sum") {
                @Override
                public Object result(Object[] args) throws ParseException {
                    checkNumberOfArguments(2, args);
                    int[] cell1 = Util.parseCell(args[0].toString());
                    int[] cell2 = Util.parseCell(args[1].toString());

                    double res = 0;

                    for (int col = cell1[0]; col <= cell2[0]; col++) {
                        for (int row = cell1[1]; row <= cell2[1]; row++) {
                            Object cell = model.get(col, row);
                            if (cell != null) {
                                Object cellValue = evaluate(parse(cell.toString()));
                                if (cellValue != null) {
                                    try {
                                        res += Double.parseDouble(cellValue.toString());
                                    } catch (NumberFormatException e) {
                                        throw new ParseException("Could not convert all cells in sum to numbers.");
                                    }
                                }
                            }
                        }
                    }

                    return res;
                }
            };
            functions.put(function.getName(), function);

            function = new Function("value") {
                @Override
                public Object result(Object[] args) throws ParseException {
                    checkNumberOfArguments(1, args);
                    int[] cell = Util.parseCell(args[0].toString());
                    return evaluate(parse(model.get(cell[0], cell[1]).toString())).toString();
                }
            };
            functions.put(function.getName(), function);
        }
    }

    static class Util {
        static int[] parseCell(String s) {
            s = s.trim().toLowerCase();

            int[] res = new int[2];
            int splitt = 0;
            for (; splitt < s.length(); splitt++) {
                char c = s.charAt(splitt);
                if ('a' <= c && c <= 'z')
                    continue;
                else
                    break;
            }
            res[0] = convertColumn(s.subSequence(0, splitt).toString());
            res[1] = Integer.parseInt(s.subSequence(splitt, s.length()).toString()) - 1;

            return res;
        }

        static int convertColumn(String number) {
            char[] column = number.toCharArray();
            int pos = 0;
            int res = 0;
            int mult = 'z' - 'a' + 1;
            for (int i = column.length - 1; i >= 0; i--) {
                if (pos == 0)
                    res += (column[i] - 'a');
                else
                    res += (column[i] - 'a') * pos * mult;
                pos++;
            }
            return res;
        }

        static String convertColumn(int number) {
            char[] chars = Integer.toString(number, 'z' - 'a' + 1).toCharArray();
            for (int i = 0; i < chars.length; i++) {
                char c = chars[i];
                if (Character.isDigit(c))
                    chars[i] = (char) (c - '0' + 'a');
                else
                    chars[i] = (char) (c - 'a' + 'k');
            }
            return String.valueOf(chars);
        }

        static String convertIndex(QModelIndex index) {
            return (convertColumn(index.column()) + (index.row() + 1)).toUpperCase();
        }
    }
}
