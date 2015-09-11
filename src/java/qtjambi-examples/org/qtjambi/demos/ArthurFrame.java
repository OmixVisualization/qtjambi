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

import java.util.ArrayList;
import java.util.List;

import org.qtjambi.qt.core.QByteArray;
import org.qtjambi.qt.core.QFile;
import org.qtjambi.qt.core.QObject;
import org.qtjambi.qt.core.QRect;
import org.qtjambi.qt.core.QRectF;
import org.qtjambi.qt.core.QSizeF;
import org.qtjambi.qt.core.QTextCodec;
import org.qtjambi.qt.core.Qt;
import org.qtjambi.qt.gui.QAbstractTextDocumentLayout;
import org.qtjambi.qt.gui.QBrush;
import org.qtjambi.qt.gui.QColor;
import org.qtjambi.qt.gui.QImage;
import org.qtjambi.qt.gui.QLinearGradient;
import org.qtjambi.qt.gui.QPaintEvent;
import org.qtjambi.qt.gui.QPainter;
import org.qtjambi.qt.gui.QPainterPath;
import org.qtjambi.qt.gui.QPalette;
import org.qtjambi.qt.gui.QPen;
import org.qtjambi.qt.gui.QPixmap;
import org.qtjambi.qt.gui.QRegion;
import org.qtjambi.qt.gui.QTextDocument;
import org.qtjambi.qt.widgets.QTextBrowser;
import org.qtjambi.qt.widgets.QWidget;

class ArthurFrame extends QWidget
{
    protected boolean m_prefer_image = false;
    protected QPixmap m_tile;
    protected boolean m_show_doc = false;
    protected QTextDocument m_document = null;
    protected String m_sourceFilename = "";

    private static QImage m_static_image = null;

    public Signal1<Boolean> descriptionEnabledChanged = new Signal1<Boolean>();

    public ArthurFrame(QWidget parent)
    {
        super(parent);

        m_tile = new QPixmap(100, 100);
        m_tile.fill(new QColor(org.qtjambi.qt.core.Qt.GlobalColor.white));

        QPainter pt = new QPainter();
        pt.begin(m_tile);
        QColor color = new QColor(240, 240, 240);
        pt.fillRect(0, 0, 50, 50, new QBrush(color));
        pt.fillRect(50, 50, 50, 50, new QBrush(color));
        pt.end();
    }

    public void paint(QPainter painter)
    {

    }

    public void loadDescription(String filename)
    {
        QFile textFile = new QFile(filename);
        String text;
        if (!textFile.open(new QFile.OpenMode(QFile.OpenModeFlag.ReadOnly))) {
            text = "Unable to load resource file: " + filename;
        } else {
            QByteArray ba = textFile.readAll();
            QTextCodec codec = QTextCodec.codecForHtml(ba);
            text = codec.toUnicode(ba);
        }
        setDescription(text);
    }

    public final void setDescription(String description)
    {
        m_document = new QTextDocument(this);
        m_document.setHtml(description);
    }

    public void paintDescription(QPainter painter)
    {
        if (m_document == null)
            return ;

        int pageWidth = Math.max(width() - 100, 100);
        int pageHeight = Math.max(height() - 100, 100);
        if (pageWidth != m_document.pageSize().width()) {
            m_document.setPageSize(new QSizeF(pageWidth, pageHeight));
        }

        QRect textRect = new QRect(width() / 2 - pageWidth / 2,
                                   height() / 2 - pageHeight / 2,
                                   pageWidth, pageHeight);
        int pad = 10;
        QRect clearRect = textRect.adjusted(-pad, -pad, pad, pad);
        painter.setPen(new QPen(Qt.PenStyle.NoPen));
        painter.setBrush(new QBrush(new QColor(0, 0, 0, 63)));
        int shade = 10;

        painter.drawRect(clearRect.x() + clearRect.width() + 1,
                         clearRect.y() + shade,
                         shade, clearRect.height() + 1);
        painter.drawRect(clearRect.x() + shade, clearRect.y() + clearRect.height() + 1,
                clearRect.width() - shade + 1, shade);
        painter.setRenderHint(QPainter.RenderHint.Antialiasing, false);
        painter.setBrush(new QBrush(new QColor(255, 255, 255, 220)));
        painter.setPen(new QColor(org.qtjambi.qt.core.Qt.GlobalColor.black));
        painter.drawRect(clearRect);

        painter.setClipRegion(new QRegion(textRect), Qt.ClipOperation.IntersectClip);
        painter.translate(textRect.topLeft());

        QAbstractTextDocumentLayout.PaintContext ctx = new QAbstractTextDocumentLayout.PaintContext();
        QLinearGradient g = new QLinearGradient(0, 0, 0, textRect.height());
        g.setColorAt(0, new QColor(org.qtjambi.qt.core.Qt.GlobalColor.black));
        g.setColorAt(0.9, new QColor(org.qtjambi.qt.core.Qt.GlobalColor.black));
        g.setColorAt(1, new QColor(org.qtjambi.qt.core.Qt.GlobalColor.transparent));

        QPalette pal = palette();
        pal.setBrush(QPalette.ColorRole.Text, new QBrush(g));
        ctx.setPalette(pal);
        ctx.setClip(new QRectF(0, 0, textRect.width(), textRect.height()));
        m_document.documentLayout().draw(painter, ctx);
    }

    protected void loadSourceFile(String sourceFile)
    {
        m_sourceFilename = sourceFile;
    }

    protected void showSource()
    {
        // Check for existing source
        List<QObject> children = this.children();
        for (QObject o : children) {
            if (o instanceof QTextBrowser)
                return ;
        }

        String contents;
        if (m_sourceFilename.length() == 0) {
            contents = "No source for widget: " + objectName();
        } else {
            QFile f = new QFile(m_sourceFilename);
            if (!f.open(new QFile.OpenMode(QFile.OpenModeFlag.ReadOnly))) {
                contents = "Could not open file: " + m_sourceFilename;
            } else {
                QByteArray ba = f.readAll();
                QTextCodec codec = QTextCodec.codecForLocale();
                contents = codec.toUnicode(ba);
            }
            f.close();
        }

        contents = contents.replace("&", "&amp;");
        contents = contents.replace("<", "&lt;");
        contents = contents.replace(">", "&gt;");

        List <String> keywords = new ArrayList<String>();
        keywords.add("for ");
        keywords.add("if ");
        keywords.add("switch ");
        keywords.add(" int ");
        keywords.add("const");
        keywords.add("void ");
        keywords.add("case ");
        keywords.add("double ");
        keywords.add("static");
        keywords.add("new");
        keywords.add("this");
        keywords.add("package");
        keywords.add("true");
        keywords.add("false");
        keywords.add("public");
        keywords.add("protected");
        keywords.add("private");
        keywords.add("final");
        keywords.add("native");
        keywords.add("import");
        keywords.add("extends");
        keywords.add("implements");
        keywords.add("else");
        keywords.add("class");
        keywords.add("super");

        for (String keyword : keywords)
            contents = contents.replace(keyword, "<font color=olive>" + keyword + "</font>");
        contents = contents.replace("(int ", "(<font color=olive><b>int </b></font>");
        contents = contents.replaceAll("(\\d\\d?)", "<font color=navy>$1</font>");

        String commentRe = "(//.+)\\n";
        // commentRe.setMinimal(true)
        contents = contents.replaceAll(commentRe, "<font color=red>$1</font>\n");

        String stringLiteralRe = "(\".+\")";
        // stringLiteralRe.setMinimal(true)
        contents = contents.replaceAll(stringLiteralRe, "<font color=green>$1</font>");

        String html = contents;
        html = "<html><pre>" + html + "</pre></html>";

        QTextBrowser sourceViewer = new QTextBrowser(null);
        sourceViewer.setWindowTitle("Source: " + m_sourceFilename);
        sourceViewer.setParent(this, new Qt.WindowFlags(Qt.WindowType.Dialog));
        sourceViewer.setAttribute(Qt.WidgetAttribute.WA_DeleteOnClose);
        sourceViewer.setHtml(html);
        sourceViewer.resize(600, 600);
        sourceViewer.show();
    }

    public final boolean preferImage()
    {
        return m_prefer_image;
    }

    public final void setPreferImage(boolean pi) { m_prefer_image = pi; }

    public final void setDescriptionEnabled(boolean enabled)
    {
        if (m_show_doc != enabled) {
            m_show_doc = enabled;
            descriptionEnabledChanged.emit(m_show_doc);
            update();
        }
    }

    @Override
    protected void paintEvent(QPaintEvent e)
    {
        QPainter painter = new QPainter();
        if (preferImage()) {
            if (m_static_image == null) {
                m_static_image = new QImage(size(), QImage.Format.Format_RGB32);
            } else if (m_static_image.size().width() != size().width()
                    || m_static_image.size().height() != size().height()) {
                m_static_image = new QImage(size(), QImage.Format.Format_RGB32);
            }

            painter.begin(m_static_image);
            int o = 10;

            QBrush bg = palette().brush(QPalette.ColorRole.Window);
            painter.fillRect(0, 0, o, o, bg);
            painter.fillRect(width() - o, 0, o, o, bg);
            painter.fillRect(0, height() - o, o, o, bg);
            painter.fillRect(width() - o, height() - o, o, o, bg);
        } else {
            painter.begin(this);
        }

        painter.setClipRect(e.rect());
        painter.setRenderHint(QPainter.RenderHint.Antialiasing);
        QPainterPath clipPath = new QPainterPath();

        QRect r = rect();
        double left = r.x() + 1;
        double top = r.y() + 1;
        double right = r.right();
        double bottom = r.bottom();
        double radius2 = 8 * 2;

        clipPath.moveTo(right - radius2, top);
        clipPath.arcTo(right - radius2, top, radius2, radius2, 90, -90);
        clipPath.arcTo(right - radius2, bottom - radius2, radius2, radius2, 0, -90);
        clipPath.arcTo(left, bottom - radius2, radius2, radius2, 270, -90);
        clipPath.arcTo(left, top, radius2, radius2, 180, -90);
        clipPath.closeSubpath();

        painter.save();
        painter.setClipPath(clipPath, Qt.ClipOperation.IntersectClip);
        painter.drawTiledPixmap(rect(), m_tile);

        paint(painter);

        painter.restore();

        painter.save();
        if (m_show_doc)
            paintDescription(painter);
        painter.restore();

        int level = 180;
        painter.setPen(new QPen(new QBrush(new QColor(level, level, level)), 2));
        painter.setBrush(new QBrush(Qt.BrushStyle.NoBrush));
        painter.drawPath(clipPath);

        if (preferImage()) {
            painter.end();
            painter.begin(this);
            painter.drawImage(e.rect(), m_static_image, e.rect());
        }

        painter.end();
    }




}
