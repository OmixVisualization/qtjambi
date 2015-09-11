/****************************************************************************
**
** Copyright (C) 2008-2009 Nokia. All rights reserved.
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

import org.qtjambi.qt.core.QRegExp;
import org.qtjambi.qt.core.Qt;
import org.qtjambi.qt.gui.QBrush;
import org.qtjambi.qt.gui.QColor;
import org.qtjambi.qt.gui.QFont;
import org.qtjambi.qt.gui.QSyntaxHighlighter;
import org.qtjambi.qt.gui.QTextCharFormat;
import org.qtjambi.qt.gui.QTextDocument;

public class XMLHighlighter extends QSyntaxHighlighter {

    public class HighlightingRule {
        public QRegExp pattern;
        public QTextCharFormat format;

        public HighlightingRule(QRegExp pattern, QTextCharFormat format) {
            this.pattern = pattern;
            this.format = format;
        }
    }

    Vector<HighlightingRule> highlightingRules = new Vector<HighlightingRule>();

    QRegExp commentStartExpression;
    QRegExp commentEndExpression;

    QTextCharFormat tagNameFormat = new QTextCharFormat();
    QTextCharFormat commentFormat = new QTextCharFormat();
    QTextCharFormat quotationFormat = new QTextCharFormat();
    QTextCharFormat attributeFormat = new QTextCharFormat();

    public XMLHighlighter(QTextDocument parent) {

        super(parent);

        HighlightingRule rule;
        QBrush brush;
        QRegExp pattern;

        // Tagname
        brush = new QBrush(new QColor(org.qtjambi.qt.core.Qt.GlobalColor.darkMagenta));
        tagNameFormat.setForeground(brush);
        tagNameFormat.setFontWeight(QFont.Weight.Bold.value());
        pattern = new QRegExp("<[A-Za-z//-]+");
        rule = new HighlightingRule(pattern, tagNameFormat);
        highlightingRules.add(rule);
        pattern = new QRegExp(">");
        rule = new HighlightingRule(pattern, tagNameFormat);
        highlightingRules.add(rule);

        // Attribute
        brush = new QBrush(new QColor(org.qtjambi.qt.core.Qt.GlobalColor.darkGreen));
        attributeFormat.setForeground(brush);
        attributeFormat.setFontWeight(QFont.Weight.Bold.value());
        pattern = new QRegExp("[A-Za-z//-]+=");
        rule = new HighlightingRule(pattern, attributeFormat);
        highlightingRules.add(rule);


        // String
        brush = new QBrush(new QColor(org.qtjambi.qt.core.Qt.GlobalColor.blue), Qt.BrushStyle.SolidPattern);
        pattern = new QRegExp("\".*\"");
        pattern.setMinimal(true);
        quotationFormat.setForeground(brush);
        rule = new HighlightingRule(pattern, quotationFormat);
        highlightingRules.add(rule);

        // Block comment
        brush = new QBrush(new QColor(org.qtjambi.qt.core.Qt.GlobalColor.gray), Qt.BrushStyle.SolidPattern);
        commentFormat.setForeground(brush);

        commentStartExpression = new QRegExp("<!--");
        commentEndExpression = new QRegExp("-->");
    }

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
        setCurrentBlockState(0);

        int startIndex = 0;
        if (previousBlockState() != 1)
            startIndex = commentStartExpression.indexIn(text);

        while (startIndex >= 0) {
            int endIndex = commentEndExpression.indexIn(text, startIndex);
            int commentLength;
            if (endIndex == -1) {
                setCurrentBlockState(1);
                commentLength = text.length() - startIndex;
            } else {
                commentLength = endIndex - startIndex + commentEndExpression.matchedLength();
            }
            setFormat(startIndex, commentLength, commentFormat);
            startIndex = commentStartExpression.indexIn(text, startIndex + commentLength);
        }
    }
}
