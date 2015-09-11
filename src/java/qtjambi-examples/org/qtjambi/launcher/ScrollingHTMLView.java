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

package org.qtjambi.launcher;

import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.widgets.*;

public class ScrollingHTMLView extends QWidget {

    private int m_y_offset = 0;
    private QTextDocument m_document = new QTextDocument();
    private QPoint m_mouse_pos;

    private QPixmap topFade = new QPixmap(1, 32);
    private QPixmap bottomFade = new QPixmap(1, 32);
    private QPixmap rightFade = new QPixmap(32, 1);

    private QPixmap background;

    private boolean wrap = true;
    private int margine = 200;

    public ScrollingHTMLView() {
        this(null);
    }

    public ScrollingHTMLView(QWidget parent) {
        super(parent);
        setAutoFillBackground(false);

        {
            topFade.fill(new QColor(0, 0, 0, 0));
            QPainter p = new QPainter();
            p.begin(topFade);
            QLinearGradient lg = new QLinearGradient(0, 0, 0, topFade.height());
            lg.setColorAt(0, new QColor(255, 255, 255));
            lg.setColorAt(1, new QColor(0, 0, 0, 0));
            p.fillRect(topFade.rect(), new QBrush(lg));
            p.end();
        }
        {
            bottomFade.fill(new QColor(0, 0, 0, 0));
            QPainter p = new QPainter();
            p.begin(bottomFade);
            QLinearGradient lg = new QLinearGradient(0, bottomFade.height(), 0, 0);
            lg.setColorAt(0, new QColor(255, 255, 255));
            lg.setColorAt(1, new QColor(0, 0, 0, 0));
            p.fillRect(bottomFade.rect(), new QBrush(lg));
            p.end();
        }
        {
            rightFade.fill(new QColor(0, 0, 0, 0));
            QPainter p = new QPainter();
            p.begin(rightFade);
            QLinearGradient lg = new QLinearGradient(rightFade.width(), 0, 0, 0);
            lg.setColorAt(0, new QColor(255, 255, 255));
            lg.setColorAt(1, new QColor(0, 0, 0, 0));
            p.fillRect(rightFade.rect(), new QBrush(lg));
            p.end();
        }
    }


    public void setHtml(String html) {
        if (html == null)
            return;
        m_document.setHtml(html);
        resetYOffset();
        update();
    }

    public void setWordWrap(boolean wrap) {
        this.wrap = wrap;
        m_document.setTextWidth(wrap ? width() - margine : -1);
    }

    @Override
    protected void paintEvent(QPaintEvent e) {
        int w = width(), h = height();
        double margin = 0.1;

        QPainter p = new QPainter();
        p.begin(this);

        if (w > background.width() || h > background.height() || background == null)
            p.fillRect(rect(), new QBrush(new QColor(org.qtjambi.qt.core.Qt.GlobalColor.white)));
        p.drawPixmap(w / 2 - background.width() / 2, h / 2 - background.height() / 2, background);

        QRectF textRect = new QRectF(w * margin, 0, w * (1 - margin), h);
        p.setPen(new QPen(Qt.PenStyle.NoPen));

        int ypos = m_y_offset;

        p.translate(textRect.x(), ypos);

        QAbstractTextDocumentLayout.PaintContext ctx = new QAbstractTextDocumentLayout.PaintContext();
        ctx.setPalette(palette());

        ctx.setClip(new QRectF(0, -ypos, w - 200, h));
        m_document.documentLayout().draw(p, ctx);

        p.resetMatrix();
        p.drawTiledPixmap(0, 0, width(), topFade.height(), topFade);
        p.drawTiledPixmap(0, height() - bottomFade.height(), width(), bottomFade.height(),
                          bottomFade);
        p.drawTiledPixmap(width() - rightFade.width(), 0, rightFade.width(), height(), rightFade);
        p.end();
    }

    @Override
    protected void wheelEvent(QWheelEvent e) {
        setYOffset(m_y_offset + e.delta() / 3);
    }

    @Override
    protected void mousePressEvent(QMouseEvent e) {
        m_mouse_pos = e.pos();
    }

    @Override
    protected void mouseMoveEvent(QMouseEvent e) {
        setYOffset(m_y_offset + e.y() - m_mouse_pos.y());
        m_mouse_pos = e.pos();
    }

    @Override
    public QSize sizeHint() {
        return new QSize(500, 400);
    }

    public QPixmap getBackground() {
        return background;
    }

    public void setBackground(QPixmap background) {
        this.background = background;
    }

    @Override
    protected void resizeEvent(QResizeEvent e) {
        m_document.setTextWidth(wrap ? e.size().width() - margine : -1);
    }

    private static final int OFFSET_BASE = 32;

    private void resetYOffset() {
        setYOffset(OFFSET_BASE);
    }

    private void setYOffset(int pos) {
        int offset_limit = (int)(-m_document.documentLayout().documentSize().height() + height()) -  OFFSET_BASE;
        pos = Math.min(OFFSET_BASE, Math.max(offset_limit, pos));

        m_y_offset = pos;
        update();
    }

    public static void main(String args[]) {
        QApplication.initialize(args);

        ScrollingHTMLView view = new ScrollingHTMLView(null);
        view.show();

        QApplication.execStatic();
        QApplication.shutdown();
    }
}
