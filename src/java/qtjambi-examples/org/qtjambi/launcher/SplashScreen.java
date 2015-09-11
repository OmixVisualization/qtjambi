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

class SplashScreen extends QSplashScreen {

    private static final QSize SIZE = new QSize(600, 300);
    private QPixmap cleanSplash;
    private QPixmap logo;
    private QRect splashScreenRect;

    public SplashScreen() {
        QRect r = QApplication.desktop().screenGeometry();
        splashScreenRect = new QRect(r.width() / 2 - SIZE.width() / 2,
                                     r.height() / 2 - SIZE.height() / 2,
                                     SIZE.width(), SIZE.height());

        QPixmap desktopBg = QApplication.desktop().grab(new QRect(splashScreenRect.x(),
                                               splashScreenRect.y(),
                                               splashScreenRect.width(),
                                               splashScreenRect.height()));

        QImage target = new QImage(SIZE, QImage.Format.Format_RGB32);
        logo = new QPixmap("classpath:org/qtjambi/images/logo.png");

        final int margin = 20;
        final int shadow = 35;
        QRectF tr = new QRectF(0, 0, SIZE.width() - margin - shadow -1 , SIZE.height() - margin - shadow -1);

        final int round = 10;
        final int round2 = round * 2;

        QPainterPath path = new QPainterPath();
        path.moveTo(tr.left() + round, tr.top());
        path.lineTo(tr.right() - round, tr.top());
        path.arcTo(tr.right() - round2, tr.top(), round2, round2, 90, -90);
        path.lineTo(tr.right(), tr.bottom() - round);
        path.arcTo(tr.right() - round2, tr.bottom() - round2, round2, round2, 0, -90);
        path.lineTo(tr.left() - round, tr.bottom());
        path.arcTo(tr.left(), tr.bottom() - round2, round2, round2, 270, -90);
        path.lineTo(tr.left(), tr.top() - round);
        path.arcTo(tr.left(), tr.top(), round2, round2, 180, -90);

        QPainter p = new QPainter(target);
        p.setRenderHint(QPainter.RenderHint.Antialiasing);

        p.drawPixmap(0, 0, desktopBg);

        p.setClipPath(path);

        // The background blurring...
        p.save();
        p.setOpacity(0.3);
        p.drawPixmap(-1, -1, desktopBg);
        p.drawPixmap(1, 1, desktopBg);
        p.setOpacity(0.2);
        p.drawPixmap(1, -1, desktopBg);
        p.drawPixmap(-1, 1, desktopBg);
        p.setOpacity(1);

        QLinearGradient gradient = new QLinearGradient(0, 0, 0, tr.height());

        double alpha = 0.3;
        QColor whiteAlpha = QColor.fromRgbF(1, 1, 1, alpha);
        QColor whiteHighlight = QColor.fromRgbF(1, 1, 1);
        QColor whiteBorder = QColor.fromRgbF(0.5, 0.5, 0.5, 0.8);

        double highlight = 0.79;
        double size = 0.08;

        gradient.setColorAt(highlight - size, whiteAlpha);
        gradient.setColorAt(highlight - size*0.9, whiteBorder);
        gradient.setColorAt(highlight, whiteHighlight);
        gradient.setColorAt(highlight + size*0.9, whiteBorder);
        gradient.setColorAt(highlight + size, whiteAlpha);

        p.fillRect(target.rect(), new QBrush(gradient));
        p.restore();

        final QRectF rectRight = new QRectF(tr.left() - margin + tr.width() + 1,
                   tr.top() + shadow,
                   shadow, tr.height() - shadow + 1 - margin);
        final QRectF rectBottom = new QRectF(tr.left() + shadow,
                tr.top() - margin + tr.height() + 1, tr.width() - shadow + 1 - margin, shadow);
        final QRectF rectBottomRight = new QRectF(tr.left() - margin + tr.width() + 1,
                tr.top() - margin + tr.height() + 1, shadow, shadow);
        final QRectF rectTopRight = new QRectF(tr.left() - margin + tr.width() + 1,
                tr.top(), shadow, shadow);
        final QRectF rectBottomLeft = new QRectF(tr.left(),
                tr.top() - margin + tr.height() + 1, shadow, shadow);

        QPainterPath clipPath = new QPainterPath();
        clipPath.addRect(new QRectF(0, 0, SIZE.width(), SIZE.height()));
        clipPath.addPath(path);

        p.setClipPath(clipPath);

        final QColor dark = new QColor(org.qtjambi.qt.core.Qt.GlobalColor.black);
        final QColor light = new QColor(org.qtjambi.qt.core.Qt.GlobalColor.transparent);

        // Drop shadow: right shadow
        {
            QLinearGradient lg = new QLinearGradient(rectRight.topLeft(),
                                                     rectRight.topRight());
            lg.setColorAt(0, dark);
            lg.setColorAt(1, light);
            p.fillRect(rectRight, new QBrush(lg));
        }

        // bottom shadow
        {
            QLinearGradient lg = new QLinearGradient(rectBottom.topLeft(), rectBottom.bottomLeft());
            lg.setColorAt(0, dark);
            lg.setColorAt(1, light);
            p.fillRect(rectBottom, new QBrush(lg));
        }

        // bottom/right corner shadow
        {
            QRadialGradient g = new QRadialGradient(rectBottomRight.topLeft(), shadow);
            g.setColorAt(0, dark);
            g.setColorAt(1, light);
            p.fillRect(rectBottomRight, new QBrush(g));
        }

        // top/right corner
        {
            QRadialGradient g = new QRadialGradient(rectTopRight.bottomLeft(), shadow);
            g.setColorAt(0, dark);
            g.setColorAt(1, light);
            p.fillRect(rectTopRight, new QBrush(g));
        }

        // bottom/left corner
        {
            QRadialGradient g = new QRadialGradient(rectBottomLeft.topRight(), shadow);
            g.setColorAt(0, dark);
            g.setColorAt(1, light);
            p.fillRect(rectBottomLeft, new QBrush(g));
        }

        p.setClipPath(path);

        // The border
        for (int i=20; i>=0; i-=3) {
            int intensity = i * 255 / 20;
            QColor c = new QColor(intensity, intensity, intensity, (255 - intensity) / 3);
            p.setPen(new QPen(c, i));
            p.drawPath(path);
        }

        p.drawPixmap(0, 0, logo);

        p.end();

        cleanSplash = QPixmap.fromImage(target);
        setPixmap(cleanSplash);

        if (org.qtjambi.qt.QSysInfo.macVersion() > 0) {
            QBitmap bm = new QBitmap(tr.size().toSize());
            bm.fill(new QColor(org.qtjambi.qt.core.Qt.GlobalColor.color0));
            QPainter bmp = new QPainter(bm);
            bmp.translate(-tr.x(), -tr.y());
            bmp.fillPath(path, new QBrush(new QColor(org.qtjambi.qt.core.Qt.GlobalColor.color1)));
            bmp.end();
            setMask(bm);
        }
    }

    public QRect splashScreenRect() {
        return splashScreenRect;
    }

    private void updateProgress(String progressMessage) {
        QPixmap pm = cleanSplash.copy();
        QPainter p = new QPainter(pm);

        QFont font = p.font();
        font.setPixelSize(11);
        p.setFont(font);

        QFontMetrics metrics = new QFontMetrics(font);
        double textWidth = metrics.width(progressMessage);
        double textHeight = metrics.ascent();

        QRectF rect = new QRectF(logo.rect().adjusted(10, 0, 0, 0));
        p.drawText(rect, progressMessage, new QTextOption(new Qt.Alignment(Qt.AlignmentFlag.AlignLeft, Qt.AlignmentFlag.AlignBottom)));
        p.end();

        setPixmap(pm);
        QCoreApplication.processEvents();
    }


}
