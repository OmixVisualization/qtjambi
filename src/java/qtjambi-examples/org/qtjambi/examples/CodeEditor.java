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

import java.util.List;
import java.util.Vector;

import org.qtjambi.qt.core.QRect;
import org.qtjambi.qt.core.QSize;
import org.qtjambi.qt.core.Qt;
import org.qtjambi.qt.gui.QBrush;
import org.qtjambi.qt.gui.QColor;
import org.qtjambi.qt.gui.QIcon;
import org.qtjambi.qt.gui.QPaintEvent;
import org.qtjambi.qt.gui.QPainter;
import org.qtjambi.qt.gui.QPen;
import org.qtjambi.qt.gui.QResizeEvent;
import org.qtjambi.qt.gui.QTextBlock;
import org.qtjambi.qt.gui.QTextCharFormat;
import org.qtjambi.qt.gui.QTextCursor;
import org.qtjambi.qt.gui.QTextFormat;
import org.qtjambi.qt.widgets.QApplication;
import org.qtjambi.qt.widgets.QPlainTextEdit;
import org.qtjambi.qt.widgets.QTextEdit;
import org.qtjambi.qt.widgets.QWidget;

@QtJambiExample(name = "Code Editor")
public class CodeEditor extends QPlainTextEdit
{
    
//![constructor]
    public CodeEditor()
    {
        lineNumberArea = new LineNumberArea(this);

        blockCountChanged.connect(this, "updateLineNumberAreaWidth(Integer)");
        updateRequest.connect(this, "updateLineNumberArea(QRect,Integer)");
        cursorPositionChanged.connect(this, "highlightCurrentLine()");

        updateLineNumberAreaWidth(0);
        highlightCurrentLine();

        setWindowTitle("Code Editor Example");
        setWindowIcon(new QIcon("classpath:org/qtjambi/images/qt-logo.png"));
    }
//![constructor]

//![extraAreaWidth]
    public int lineNumberAreaWidth()
    {
        int digits = 1;
        int max = Math.max(1, blockCount());
        while (max >= 10) {
            max /= 10;
            ++digits;
        }

        int space = 3 + fontMetrics().width('9') * digits;

        return space;
    }
//![extraAreaWidth]

//![slotUpdateExtraAreaWidth]
    public void updateLineNumberAreaWidth(Integer newBlockCount)
    {
        setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
    }
//![slotUpdateExtraAreaWidth]

//![slotUpdateRequest]
    public void updateLineNumberArea(QRect rect, Integer dy)
    {
        if (dy > 0)
            lineNumberArea.scroll(0, dy);
        else
            lineNumberArea.update(0, rect.y(), lineNumberArea.width(),
                                  rect.height());

        if (rect.contains(viewport().rect()))
            updateLineNumberAreaWidth(0);
    }
//![slotUpdateRequest]

//![resizeEvent]
    @Override
    protected void resizeEvent(QResizeEvent e)
    {
        super.resizeEvent(e);

        QRect cr = contentsRect();
        lineNumberArea.setGeometry(new QRect(cr.left(), cr.top(),
                                   lineNumberAreaWidth(), cr.height()));
    }
//![resizeEvent]

//![cursorPositionChanged]
    private void highlightCurrentLine()
    {
        List<QTextEdit.ExtraSelection> extraSelections =
            new Vector<QTextEdit.ExtraSelection>();

        if (!isReadOnly()) {
            QTextEdit.ExtraSelection selection =
                new QTextEdit.ExtraSelection();
            
            QColor lineColor = new QColor(org.qtjambi.qt.core.Qt.GlobalColor.yellow).lighter(160);

            QTextCharFormat format = selection.format();
            format.setBackground(new QBrush(lineColor));
            format.setProperty(QTextFormat.Property.FullWidthSelection.value(), new Boolean(true));
            selection.setFormat(format);
            QTextCursor cursor = textCursor();
            cursor.clearSelection();
            selection.setCursor(cursor);
            extraSelections.add(selection);
        }

        setExtraSelections(extraSelections);
    }
//![cursorPositionChanged]

//![extraAreaPaintEvent_0]
    public void lineNumberAreaPaintEvent(QPaintEvent event)
    {
        QPainter painter = new QPainter(lineNumberArea);
        painter.setPen(new QPen(new QColor(org.qtjambi.qt.core.Qt.GlobalColor.black)));
        painter.fillRect(event.rect(), new QBrush(new QColor(org.qtjambi.qt.core.Qt.GlobalColor.lightGray)));

//![extraAreaPaintEvent_0]

//![extraAreaPaintEvent_1]
        QTextBlock block = firstVisibleBlock();
        int blockNumber = block.blockNumber();
        int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
        int bottom = top + (int) blockBoundingRect(block).height();
//![extraAreaPaintEvent_1]

//![extraAreaPaintEvent_2]
        while (block.isValid() && top <= event.rect().bottom()) {
            if (block.isVisible() && bottom >= event.rect().top()) {
                String number = String.valueOf(blockNumber + 1);
                painter.drawText(0, top, lineNumberArea.width(), fontMetrics().height(),
                                Qt.AlignmentFlag.AlignRight.value(), number);
            }

            block = block.next();
            top = bottom;
            bottom = top + (int) blockBoundingRect(block).height();
            ++blockNumber;
        }
    } 
//![extraAreaPaintEvent_2]

//![extraarea]
    private static class LineNumberArea extends QWidget
    {
        public LineNumberArea(CodeEditor editor)
        {
            codeEditor = editor;
            setParent(codeEditor);
        }

        @Override
        public QSize sizeHint()
        {
            return new QSize(codeEditor.lineNumberAreaWidth(), 0);
        }

        @Override
        protected void paintEvent(QPaintEvent event)
        {
            codeEditor.lineNumberAreaPaintEvent(event);
        }

        private CodeEditor codeEditor;
    }
//![extraarea]

    private LineNumberArea lineNumberArea;

    public static void main(String args[])
    {
        QApplication.initialize(args);

        new CodeEditor().show();

        QApplication.execStatic();
        QApplication.shutdown();
    }
}
