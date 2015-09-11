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

package org.qtjambi.tests;

import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;

class SmokeEffect
{
    private int m_seeds[];
    private int m_seed_pos;
    private QImage m_image;
    private int m_width;
    private int m_height;
    private int m_data[];

    private final int seed() {
    int s = m_seeds[m_seed_pos];
    m_seed_pos = (m_seed_pos + 1) % m_seeds.length;
    return s;
    }

    private final void seedLastRow() {
    int oset = m_width * m_height;
    for (int x=0; x<m_width; ++x) {
        m_data[oset + x] = seed();
    }
    }

    public SmokeEffect(int width, int height) {
    m_width = width;
    m_height = height;

    m_seeds = new int[16000];
    for (int i=0; i<m_seeds.length; ++i) {
        double d = Math.random();
        m_seeds[i] = (int) (d * 255);
    }
    m_image = new QImage(width, height, QImage.Format.Format_ARGB32_Premultiplied);
    m_image.fill(0);

    m_data = new int[m_width * (m_height + 1)];
    seedLastRow();
    }


    public void next() {
    // blur
    for (int y=1; y<m_height; ++y) {
        int oset = y * m_width;
//      System.out.println("y:::");
        for (int x=1; x<m_width - 1; ++x) {
//      int dl = m_data[oset + m_width + x - 1];
//      int dr = m_data[oset + m_width + x + 1];
//      int d = m_data[oset + m_width + x];
//          m_data[oset + x] = (int) ((dl + (d * 2) + dr) / (4 + 10.0 / m_width));
//      m_data[oset + x] = (int) ((dl + du + dr + dd) / 4);
        m_data[oset + x] = (((m_data[oset+x+m_width]<<2) +
                       (m_data[oset+x]<<1) +
                       (m_data[oset+x+1]) +
                       (m_data[oset+x-1])) - 7) >> 3;
        if (m_data[oset+x] < 0)
            m_data[oset+x] = 0;
//      System.out.print(" " + m_data[oset + x]);
        }
//      System.out.println();
    }

    // seed the last row for the next step...
    seedLastRow();

    QNativePointer np = m_image.bits();
    np.setVerificationEnabled(false);
    int ppl = m_image.bytesPerLine() / 4;
    for (int y=0; y<m_height; ++y) {
        int oset = ppl * y;
        for (int x=0; x<m_width; ++x) {
        int val = m_data[oset + x];
        val = val * val / (255);
        int val_2 = 0;
        np.setIntAt(oset + x, (val << 24) | (val_2 << 16) | (val_2 << 8) | (val_2));
        }
    }
    }

    public QImage image() { return m_image; }
}

public class Image extends QWidget
{
    static final private double GOLDEN_MEAN = 1 / 1.61803399;
    static final private QColor color_green = new QColor(167, 196, 0);

    static private QImage qtLogoImage(int width, int height, boolean render_logo) {
    QBrush tt_green = new QBrush(color_green);
    QBrush tt_black = new QBrush(new QColor(0, 0, 0));

    QImage image = new QImage(width, height, QImage.Format.Format_RGB32);
    QPainter p = new QPainter();
    p.begin(image);
    p.setRenderHint(QPainter.RenderHint.Antialiasing);

    // Fill the background
    p.scale(width, height);
    p.setPen(new QPen(tt_black, 0.05));
    p.setBrush(tt_green);
    p.drawRect(0, 0, 1, 1);


    if (render_logo) {
        // set up painter for the logo drawing..
        p.setPen(Qt.PenStyle.NoPen);
        p.setBrush(new QBrush(QColor.black));
        p.translate(0.5, 0.5);
        p.rotate(-45);

        double thickness = 0.13;
        double inner_radius = 0.24;
        double outer_radius = inner_radius + thickness;

        double ir2 = inner_radius * 2;
        double or2 = outer_radius * 2;
        double t_2 = thickness / 2;

        // draw the black circle
        QPainterPath circle_path = new QPainterPath();
        circle_path.addEllipse(-inner_radius, -inner_radius, ir2, ir2);
        circle_path.addEllipse(-outer_radius, -outer_radius, or2, or2);
        p.drawPath(circle_path);

        QPainterPath t_path = new QPainterPath();
        t_path.addRect(-t_2, 0, thickness, 0.48);
        t_path.addRect(-0.17, -t_2, 0.34, thickness);
        t_path.setFillRule(Qt.FillRule.WindingFill);
        p.drawPath(t_path);
    }
    p.end();

    return image;
    }

    static private QImage makeShade(QImage image, boolean highlight, boolean darker) {
    double shade_alpha = 0.8;
    double shade_size = 0.2;
    double hl_pos = GOLDEN_MEAN;

    QColor shade_color = QColor.fromRgbF(0, 0, 0, shade_alpha);
    QColor transparent = darker ? QColor.fromRgbF(0, 0, 0, 0.25) : QColor.fromRgbF(0, 0, 0, 0);
    QColor hl_color = QColor.fromRgbF(1, 1, 1, 0.7);

    image = image.copy();
    QPainter p = new QPainter();
    p.begin(image);
    p.scale(image.width(), image.height());

    QLinearGradient lg = new QLinearGradient(0, 0, 1, 0);
    lg.setColorAt(0, shade_color);
    lg.setColorAt(shade_size, transparent);
    if (highlight)
        lg.setColorAt(hl_pos, hl_color);
    lg.setColorAt(1 - shade_size, transparent);
    lg.setColorAt(1, shade_color);
    p.fillRect(0, 0, 1, 1, new QBrush(lg));

    if (highlight) {
        QRadialGradient rg = new QRadialGradient(hl_pos, 1 - hl_pos, 0.4, hl_pos, 1 - hl_pos);
        rg.setColorAt(0, hl_color);
        rg.setColorAt(1, transparent);
        p.fillRect(0, 0, 1, 1, new QBrush(rg));
    }

    p.end();

    return image;
    }

    static private QImage warpImage(QImage src, int warp, int sign) {
    int w = src.width();
    int ppl = src.bytesPerLine() / 4;
    int srch = src.height();
    int desth = srch + warp;

    QImage dest = new QImage(w, desth, QImage.Format.Format_ARGB32_Premultiplied);
    dest.fill(0);

    QNativePointer sbits = src.bits();
    QNativePointer dbits = dest.bits();

    sbits.setVerificationEnabled(false);
    dbits.setVerificationEnabled(false);

    double r = w / 2.0;

    int extra_offset = sign >= 0 ? 0 : warp;

    for (int x=0; x<w; ++x) {
        int oset = (int) (((Math.sqrt(Math.abs(r*r - (x-r)*(x-r)))) * sign / r) * warp);
        for (int y=0; y<srch; ++y) {
        int p = sbits.intAt(y * ppl + x);
        dbits.setIntAt(((y + oset + extra_offset) * ppl) + x, p);
        }
    }
    return dest;
    }

    static private QImage composeCup(QImage bg_image, QImage fg_image, int size, int warp) {
    double coffey_alpha = 1;
    QColor dark_brown = QColor.fromRgbF(0.3, 0.15, 0, coffey_alpha);
    QColor light_brown = QColor.fromRgbF(0.66, 0.33, 0, coffey_alpha);
    QColor highlight = QColor.fromRgbF(1, 1, 0.8, coffey_alpha);
    QColor transparent = QColor.transparent;
    double highlight_pos = 0.4;
    double highlight_size = 0.05;


    QImage combined = new QImage(size + size / 2,
                     size + warp * 2 + size / 8,
                     QImage.Format.Format_ARGB32_Premultiplied);
    combined.fill(0);

    QPainter p = new QPainter();
    p.begin(combined);
    p.setRenderHint(QPainter.RenderHint.Antialiasing);
    p.setPen(Qt.PenStyle.NoPen);

    // draw the background
    p.drawImage(0, 0, bg_image);

    // Draw the coffey
    QRectF coffey_bounds = new QRectF(0, warp, size, 2 * warp);
    QConicalGradient cg = new QConicalGradient(coffey_bounds.width()*GOLDEN_MEAN + warp*0.1,
                           warp*1.2 + coffey_bounds.height() / 2.0,
                           -30);
    cg.setColorAt(0, dark_brown);
    cg.setColorAt(highlight_pos - highlight_size, light_brown);
    cg.setColorAt(0.4, highlight);
    cg.setColorAt(highlight_pos + highlight_size, light_brown);
    cg.setColorAt(1, dark_brown);
    p.setPen(new QPen(new QBrush(QColor.black), size * 0.01));
    p.setBrush(new QBrush(cg));
    p.drawEllipse(coffey_bounds);

    // draw the foreground
    p.drawImage(0, warp, fg_image);

    // Draw the handle
    double handle_dim = size * 0.8;
    QRectF handle_bounds = new QRectF(size - handle_dim / 2, size / 3, handle_dim, handle_dim);
    double hcx = handle_bounds.width() / 2;
    double hcy = handle_bounds.height() / 2;
    QImage handle_im = new QImage(handle_bounds.size().toSize(),
                      QImage.Format.Format_ARGB32_Premultiplied);
    {
        handle_im.fill(0);
        QPainter ph = new QPainter();
        ph.begin(handle_im);
        QRadialGradient rg = new QRadialGradient(hcx, hcy, hcx, hcx, hcy + size / 30);
        double ir = 0.5;
        double or = 0.9;
        double aa = 0.02;
        double shade = 0.08;
        rg.setColorAt(ir - aa, transparent);
        rg.setColorAt(ir, QColor.fromRgbF(0, 0, 0, 1));
        rg.setColorAt(ir + shade, color_green);
        rg.setColorAt((ir + or) / 2, QColor.fromRgbF(0.8, 0.85, 0.6));
        rg.setColorAt(or - shade, color_green);
        rg.setColorAt(or, QColor.fromRgbF(0, 0, 0, 1));
        rg.setColorAt(or + aa, transparent);
        ph.setBrush(new QBrush(rg));
        ph.setPen(Qt.PenStyle.NoPen);
        ph.drawEllipse(0, 0, (int) handle_bounds.width(), (int) handle_bounds.height());

        QLinearGradient lg = new QLinearGradient(0, 0, handle_bounds.width(), 0);
        lg.setColorAt(0.42, transparent);
        lg.setColorAt(0.47, QColor.fromRgbF(0, 0, 0));
        ph.setBrush(new QBrush(lg));
        ph.setCompositionMode(QPainter.CompositionMode.CompositionMode_DestinationIn);
        ph.drawRect(0, 0, (int) handle_bounds.width(), (int) handle_bounds.height());
        ph.end();
    }
    p.drawImage(handle_bounds.topLeft(), handle_im);

    // The drop shadow...
    QRadialGradient dsg = new QRadialGradient(0, 0, 0.5, 0, 0);
    dsg.setColorAt(0, QColor.fromRgbF(0, 0, 0, .75));
    dsg.setColorAt(1, transparent);
    p.translate(size * 3 / 4, size + warp * 1.4);
    p.scale(size * 1.5, size / 2);
    p.setBrush(new QBrush(dsg));
    p.setPen(Qt.PenStyle.NoPen);
    p.setCompositionMode(QPainter.CompositionMode.CompositionMode_DestinationOver);
    p.drawEllipse(new QRectF(-0.5, -0.5, 1, 1));
//      p.drawEllipse(new QRectF(size - size / 2, size + warp - size / 2, size, size));



    p.end();

    return combined;
    }

    public static QImage qoffeyLogo() { return qoffeyLogo(128); }
    public static QImage qoffeyLogo(int size) {
    int warp = size / 5;
    QImage bg_image = null, fg_image = null;
    {
        QImage image = qtLogoImage(size, size, false);
        QImage shade = makeShade(image, false, true);
        bg_image = warpImage(shade, warp, -1);
    }
    {
        QImage image = qtLogoImage(size, size, true);
        QImage shade = makeShade(image, true, false);
        fg_image = warpImage(shade, warp, 1);
    }

    QImage cup = composeCup(bg_image, fg_image, size, warp);
    return cup.scaledToWidth(size, Qt.TransformationMode.SmoothTransformation);
    }

    private SmokeEffect m_smoke_effect;
    private QImage m_logo;

    public Image()
    {
    m_smoke_effect = new SmokeEffect(132, 128);
    m_logo = qoffeyLogo(200);

    QTimer t = new QTimer(this);
    t.timeout.connect(this, "update()");
    t.start(10);
    }

    @Override
    protected void paintEvent(QPaintEvent e) {
    m_smoke_effect.next();

    QPainter p = new QPainter();
    p.begin(this);
    p.fillRect(rect(), new QBrush(QColor.white));
//  p.fillRect(rect(), new QBrush(Qt.CrossPattern));

    p.drawImage(50, 80, m_logo);

    p.setRenderHint(QPainter.RenderHint.SmoothPixmapTransform);
    QImage im = m_smoke_effect.image();
    p.drawImage(new QRect(50, -20, im.width(), im.height()),
            im,
            new QRect(0, (int)(im.height()*0.60), im.width(), (int)(im.height()*0.39)));


    p.end();
    }

    @Override
    public QSize sizeHint() {
    return new QSize(270, 320);
    }


    public static void main(String args[])
    {
    QApplication.initialize(args);

    QApplication.setWindowIcon(new QIcon(QPixmap.fromImage(qoffeyLogo(64))));


    //QPixmap pm = QPixmap.fromImage(qoffeyLogo(400));

    Image im = new Image();
    im.show();

    QApplication.execStatic();
    QApplication.shutdown();
    }
}
