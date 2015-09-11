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

import java.util.Vector;

import org.qtjambi.qt.core.QFile;
import org.qtjambi.qt.core.QRegExp;
import org.qtjambi.qt.core.Qt;
import org.qtjambi.qt.gui.QBrush;
import org.qtjambi.qt.gui.QColor;
import org.qtjambi.qt.gui.QFont;
import org.qtjambi.qt.gui.QIcon;
import org.qtjambi.qt.gui.QSyntaxHighlighter;
import org.qtjambi.qt.gui.QTextCharFormat;
import org.qtjambi.qt.gui.QTextDocument;
import org.qtjambi.qt.widgets.QAction;
import org.qtjambi.qt.widgets.QApplication;
import org.qtjambi.qt.widgets.QFileDialog;
import org.qtjambi.qt.widgets.QMainWindow;
import org.qtjambi.qt.widgets.QMenu;
import org.qtjambi.qt.widgets.QMessageBox;
import org.qtjambi.qt.widgets.QTextEdit;

@QtJambiExample(name = "Syntax Highlighter")
//! [0]
public class SyntaxHighlighter extends QMainWindow {

    private QTextEdit editor;
//! [0]

//! [1]
    public static void main(String args[]) {
        QApplication.initialize(args);

        SyntaxHighlighter syntaxHighlighter = new SyntaxHighlighter();
        syntaxHighlighter.show();

        QApplication.execStatic();
        QApplication.shutdown();
    }
//! [1]

//! [2]
    public SyntaxHighlighter() {
        setupFileMenu();
        setupHelpMenu();
        setupEditor();

        setCentralWidget(editor);
        resize(640, 480);
        setWindowTitle(tr("Syntax Highlighter"));
        setWindowIcon(new QIcon(
                      "classpath:org/qtjambi/images/qt-logo.png"));
    }
//! [2]

    public void about() {
        QMessageBox.about(this, tr("About Syntax Highlighter"),
                tr("<p>The <b>Syntax Highlighter</b> example shows how "
                        + "to perform simple syntax highlighting by subclassing "
                        + "the QSyntaxHighlighter class and describing "
                        + "highlighting rules using regular expressions.</p>"));
    }

    public void newFile() {
        editor.clear();
    }

    public void openFile() {
        openFile("");
    }

    public void openFile(String fileName) {
        if (fileName.equals(""))
            fileName = QFileDialog
                    .getOpenFileName(this, tr("Open File"), "", new QFileDialog.Filter("Java Files (*.java)"));

        if (!fileName.equals("")) {
            QFile file = new QFile(fileName);
            if (file.open(new QFile.OpenMode(QFile.OpenModeFlag.ReadOnly, QFile.OpenModeFlag.Text)))
                editor.setPlainText(file.readAll().toString());
        }
    }

//! [3]
    private void setupEditor() {
        QFont font = new QFont();
        font.setFamily("Courier");
        font.setFixedPitch(true);
        font.setPointSize(10);

        editor = new QTextEdit();
        editor.setLineWrapMode(QTextEdit.LineWrapMode.NoWrap);
        editor.setFont(font);

        new Highlighter(editor.document());

        QFile file = new QFile(
               "classpath:org/qtjambi/examples/SyntaxHighlighter.java");

        if (file.open(new QFile.OpenMode(QFile.OpenModeFlag.ReadOnly,
                                         QFile.OpenModeFlag.Text)))
            editor.setPlainText(file.readAll().toString());
    }
//! [3]

    private void setupFileMenu() {
        QMenu fileMenu = new QMenu(tr("&File"), this);
        menuBar().addMenu(fileMenu);

        QAction newAct = new QAction(tr("&New"), this);
        newAct.triggered.connect(this, "newFile()");
        fileMenu.addAction(newAct);

        QAction openAct = new QAction(tr("&Open..."), this);
        openAct.triggered.connect(this, "openFile()");
        fileMenu.addAction(openAct);

        QAction quitAct = new QAction(tr("E&xit"), this);
        quitAct.triggered.connect(this, "close()");
        fileMenu.addAction(quitAct);
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

//! [4]
    private class Highlighter extends QSyntaxHighlighter {
//! [4]

//! [5]
        public class HighlightingRule {
            public QRegExp pattern;
            public QTextCharFormat format;

            public HighlightingRule(QRegExp pattern, QTextCharFormat format) {
                this.pattern = pattern;
                this.format = format;
            }
//! [5] //! [6]
        }
//! [6]

//! [7]
        Vector<HighlightingRule> highlightingRules = new Vector<HighlightingRule>();
//! [7]

//! [8]
        QRegExp commentStartExpression;
        QRegExp commentEndExpression;

        QTextCharFormat keywordFormat = new QTextCharFormat();
        QTextCharFormat classFormat = new QTextCharFormat();
        QTextCharFormat commentFormat = new QTextCharFormat();
        QTextCharFormat quotationFormat = new QTextCharFormat();
        QTextCharFormat functionFormat = new QTextCharFormat();
//! [8]

//! [9]
        public Highlighter(QTextDocument parent) {

            super(parent);

            HighlightingRule rule;
            QBrush brush;
            QRegExp pattern;
//! [9]

//! [10]
            brush = new QBrush(new QColor(org.qtjambi.qt.core.Qt.GlobalColor.darkBlue),Qt.BrushStyle.SolidPattern);
            keywordFormat.setForeground(brush);
            keywordFormat.setFontWeight(QFont.Weight.Bold.value());

//! [10]
            // All the java keywords
//! [11]
            String[] keywords = { "abstract", "continue", "for", "new",
                                  "switch", "assert", "default", "goto",
                                  "package", "synchronized", "boolean",
                                  "do", "if", "private", "this", "break",
                                  "double", "implements", "protected",
                                  "throw", "byte", "else", "import",
                                  "public", "throws", "case", "enum",
                                  "instanceof", "return", "transient",
                                  "catch", "extends", "int", "short",
                                  "try", "char", "final", "interface",
                                  "static", "void", "class", "finally",
                                  "long", "strictfp", "volatile", "const",
                                  "float", "native", "super", "while" };

            for (String keyword : keywords) {
                pattern = new QRegExp("\\b" + keyword + "\\b");
                rule = new HighlightingRule(pattern, keywordFormat);
                highlightingRules.add(rule);
            }

//! [11]
            // Any word starting with Q
//! [12]
            brush = new QBrush(new QColor(org.qtjambi.qt.core.Qt.GlobalColor.darkMagenta));
            pattern = new QRegExp("\\bQ[A-Za-z]+\\b");
            classFormat.setForeground(brush);
            classFormat.setFontWeight(QFont.Weight.Bold.value());
            rule = new HighlightingRule(pattern, classFormat);
            highlightingRules.add(rule);
//! [12]

            // Comment starting with //
//! [13]
            brush = new QBrush(new QColor(org.qtjambi.qt.core.Qt.GlobalColor.gray), Qt.BrushStyle.SolidPattern);
            pattern = new QRegExp("//[^\n]*");
            commentFormat.setForeground(brush);
            rule = new HighlightingRule(pattern, commentFormat);
            highlightingRules.add(rule);
//! [13]

            // String
//! [14]
            brush = new QBrush(new QColor(org.qtjambi.qt.core.Qt.GlobalColor.blue), Qt.BrushStyle.SolidPattern);
            pattern = new QRegExp("\".*\"");
            pattern.setMinimal(true);
            quotationFormat.setForeground(brush);
            rule = new HighlightingRule(pattern, quotationFormat);
            highlightingRules.add(rule);
//! [14]

            // Function
            brush = new QBrush(new QColor(org.qtjambi.qt.core.Qt.GlobalColor.darkGreen), Qt.BrushStyle.SolidPattern);
            pattern = new QRegExp("\\b[A-Za-z0-9_]+(?=\\()");
            functionFormat.setForeground(brush);
            functionFormat.setFontItalic(true);
            rule = new HighlightingRule(pattern, functionFormat);
            highlightingRules.add(rule);

            // Block comment
//! [15]
            commentStartExpression = new QRegExp("/\\*");
            commentEndExpression = new QRegExp("\\*/");
        }
//! [15]

        @Override
//! [16]
        public void highlightBlock(String text) {

            for (HighlightingRule rule : highlightingRules) {
                QRegExp expression = rule.pattern;
                int index = expression.indexIn(text);
                while (index >= 0) {
                    int length = expression.matchedLength();
                    setFormat(index, length, rule.format);
                    index = expression.indexIn(text, index + length);
                }
            }
//! [16] //! [17]
            setCurrentBlockState(0);
//! [17]

//! [18]
            int startIndex = 0;
            if (previousBlockState() != 1)
                startIndex = commentStartExpression.indexIn(text);

//! [18] //! [19]
            while (startIndex >= 0) {
//! [19] //! [20]
                int endIndex = commentEndExpression.indexIn(text, startIndex);
                int commentLength;
                if (endIndex == -1) {
                    setCurrentBlockState(1);
                    commentLength = text.length() - startIndex;
                } else {
//! [20] //! [21]
                    commentLength = endIndex - startIndex + commentEndExpression.matchedLength();
                }
//! [21] //! [22]
                setFormat(startIndex, commentLength, commentFormat);
                startIndex = commentStartExpression.indexIn(text, startIndex + commentLength);
            }
//! [22] //! [23]
        }
//! [23]
    }
//! [24]
}
//! [24]
