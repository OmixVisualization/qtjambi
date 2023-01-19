/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
** 
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
** 
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
** 
** $END_LICENSE$

**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

package io.qt.autotests;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertTrue;

import java.util.Collections;

import org.junit.Assert;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.autotests.generated.General;
import io.qt.core.QBitArray;
import io.qt.core.QByteArray;
import io.qt.core.QByteArrayMatcher;
import io.qt.core.QDate;
import io.qt.core.QDateTime;
import io.qt.core.QDir;
import io.qt.core.QEasingCurve;
import io.qt.core.QElapsedTimer;
import io.qt.core.QFileInfo;
import io.qt.core.QItemSelection;
import io.qt.core.QItemSelectionRange;
import io.qt.core.QLine;
import io.qt.core.QLineF;
import io.qt.core.QLocale;
import io.qt.core.QMargins;
import io.qt.core.QPersistentModelIndex;
import io.qt.core.QPoint;
import io.qt.core.QPointF;
import io.qt.core.QProcessEnvironment;
import io.qt.core.QRect;
import io.qt.core.QRectF;
import io.qt.core.QSize;
import io.qt.core.QSizeF;
import io.qt.core.QStringMatcher;
import io.qt.core.QTextBoundaryFinder;
import io.qt.core.QTime;
import io.qt.core.QUrl;
import io.qt.core.QUuid;
import io.qt.core.QXmlStreamAttribute;
import io.qt.core.QXmlStreamAttributes;
import io.qt.core.QXmlStreamEntityDeclaration;
import io.qt.core.QXmlStreamNamespaceDeclaration;
import io.qt.core.QXmlStreamNotationDeclaration;
import io.qt.core.Qt;
import io.qt.core.Qt.AlignmentFlag;
import io.qt.gui.QAbstractTextDocumentLayout;
import io.qt.gui.QBitmap;
import io.qt.gui.QBrush;
import io.qt.gui.QColor;
import io.qt.gui.QConicalGradient;
import io.qt.gui.QCursor;
import io.qt.gui.QFont;
import io.qt.gui.QFontInfo;
import io.qt.gui.QFontMetrics;
import io.qt.gui.QFontMetricsF;
import io.qt.gui.QIcon;
import io.qt.gui.QImage;
import io.qt.gui.QInputMethodEvent;
import io.qt.gui.QKeySequence;
import io.qt.gui.QLinearGradient;
import io.qt.gui.QMatrix4x4;
import io.qt.gui.QPainter;
import io.qt.gui.QPainterPath;
import io.qt.gui.QPalette;
import io.qt.gui.QPen;
import io.qt.gui.QPicture;
import io.qt.gui.QPixmap;
import io.qt.gui.QPolygon;
import io.qt.gui.QPolygonF;
import io.qt.gui.QQuaternion;
import io.qt.gui.QRadialGradient;
import io.qt.gui.QRegion;
import io.qt.gui.QStandardItem;
import io.qt.gui.QStandardItemModel;
import io.qt.gui.QStaticText;
import io.qt.gui.QTextBlock;
import io.qt.gui.QTextBlockFormat;
import io.qt.gui.QTextCharFormat;
import io.qt.gui.QTextCursor;
import io.qt.gui.QTextDocument;
import io.qt.gui.QTextDocumentFragment;
import io.qt.gui.QTextFormat;
import io.qt.gui.QTextFragment;
import io.qt.gui.QTextFrame;
import io.qt.gui.QTextFrameFormat;
import io.qt.gui.QTextImageFormat;
import io.qt.gui.QTextLayout;
import io.qt.gui.QTextLength;
import io.qt.gui.QTextLine;
import io.qt.gui.QTextListFormat;
import io.qt.gui.QTextOption;
import io.qt.gui.QTextTableCell;
import io.qt.gui.QTextTableCellFormat;
import io.qt.gui.QTextTableFormat;
import io.qt.gui.QTransform;
import io.qt.gui.QVector2D;
import io.qt.gui.QVector3D;
import io.qt.gui.QVector4D;
import io.qt.widgets.QColormap;
import io.qt.widgets.QListWidgetItem;
import io.qt.widgets.QSizePolicy;
import io.qt.widgets.QStyleOption;
import io.qt.widgets.QTableWidgetItem;
import io.qt.widgets.QTableWidgetSelectionRange;
import io.qt.widgets.QTextEdit;
import io.qt.widgets.QTileRules;
import io.qt.widgets.QTreeWidgetItem;

public class TestCloneable extends ApplicationInitializer {
	
	@BeforeClass
    public static void testInitialize() throws Exception {
    	ApplicationInitializer.testInitializeWithWidgets();
    }

    @Test
    public void run_clone_QBitArray() {
        QBitArray org = new QBitArray();
        QBitArray clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
        org.dispose();
        QBitArray clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
        assertEquals(clone, clone2);
    }

    @Test
    public void run_clone_QBrush() {
        QBrush org = new QBrush();
        QBrush clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
        org.dispose();
        QBrush clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
        assertEquals(clone, clone2);
    }

    @Test
    public void run_clone_QByteArray() {
        QByteArray org = new QByteArray();
        QByteArray clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
        org.dispose();
        QByteArray clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
        assertEquals(clone, clone2);
    }

    @Test
    public void run_clone_QByteArrayMatcher() {
        QByteArrayMatcher org = new QByteArrayMatcher(new QByteArray("1234"));
        QByteArrayMatcher clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
        org.dispose();
        QByteArrayMatcher clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
        assertEquals(clone.pattern(), clone2.pattern());
    }

    @Test
    public void run_clone_QColor() {
        QColor org = new QColor();
        QColor clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
        org.dispose();
        QColor clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
        assertEquals(clone, clone2);
    }

    @Test
    public void run_clone_QColormap() {
        QColormap org = QColormap.instance(0);
        assertNotNull(org);
        QColormap clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
        assertNotNull(clone);
        org.dispose();
        QColormap clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
        assertNotNull(clone2);
        assertEquals(clone.depth(), clone2.depth());
    }

    @Test
    public void run_clone_QCursor() {
        QCursor org = new QCursor();
        QCursor clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
        org.dispose();
        QCursor clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
        assertEquals(clone.hotSpot(), clone2.hotSpot());
    }

    @Test
    public void run_clone_QDateTime() {
        QDateTime org = new QDateTime();
        QDateTime clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
        org.dispose();
        QDateTime clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
        assertEquals(clone, clone2);
    }

    @Test
    public void run_clone_QDir() {
        QDir org = new QDir();
        QDir clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
        org.dispose();
        QDir clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
        assertEquals(clone, clone2);
    }

    @Test
    public void run_clone_QFileInfo() {
        QFileInfo org = new QFileInfo();
        QFileInfo clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
        org.dispose();
        QFileInfo clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
        assertEquals(clone, clone2);
    }

    @Test
    public void run_clone_QFont() {
        QFont org = new QFont();
        QFont clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
        org.dispose();
        QFont clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
        assertEquals(clone, clone2);
    }

    @Test
    public void run_clone_QFontInfo() {
        QFontInfo org = new QFontInfo(new QFont("frodo"));
        QFontInfo clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
        assertNotNull(clone);
        org.dispose();
        QFontInfo clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
        assertNotNull(clone2);
        assertEquals(clone.family(), clone2.family());
    }

    @Test
    public void run_clone_QFontMetrics() {
        QFontMetrics org = new QFontMetrics(new QFont());
        QFontMetrics clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
        assertNotNull(clone);
        org.dispose();
        QFontMetrics clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
        assertNotNull(clone2);
        assertEquals(clone, clone2);
    }

    @Test
    public void run_clone_QFontMetricsF() {
        QFontMetricsF org = new QFontMetricsF(new QFont());
        QFontMetricsF clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
        assertNotNull(clone);
        org.dispose();
        QFontMetricsF clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
        assertNotNull(clone2);
        assertEquals(clone, clone2);
    }

    @Test
    public void run_clone_QIcon() {
        QIcon org = new QIcon("file");
        QIcon clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
        org.dispose();
        QIcon clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
        assertEquals(clone.isNull(), clone2.isNull());
    }

    @Test
    public void run_clone_QImage() {
        QImage org = new QImage();
        QImage clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
        org.dispose();
        QImage clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
        assertEquals(clone, clone2);
    }

    @Test
    public void run_clone_QKeySequence() {
        QKeySequence org = new QKeySequence();
        QKeySequence clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
        org.dispose();
        QKeySequence clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
        assertEquals(clone, clone2);
    }

    @Test
    public void run_clone_QLocale() {
        QLocale org = new QLocale();
        QLocale clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
        org.dispose();
        QLocale clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
        assertEquals(clone, clone2);
    }

    @Test
    public void run_clone_QPainterPath() {
        QPainterPath org = new QPainterPath();
        QPainterPath clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
        org.dispose();
        QPainterPath clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
        assertEquals(clone, clone2);
    }

    @Test
    public void run_clone_QPalette() {
        QPalette org = new QPalette();
        QPalette clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
        org.dispose();
        QPalette clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
        assertEquals(clone, clone2);
    }

    @Test
    public void run_clone_QPen() {
        QPen org = new QPen();
        QPen clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
        org.dispose();
        QPen clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
        assertEquals(clone, clone2);
    }

    @Test
    public void run_clone_QPersistentModelIndex() {
        QPersistentModelIndex org = new QPersistentModelIndex();
        QPersistentModelIndex clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
        org.dispose();
        QPersistentModelIndex clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
        assertEquals(clone, clone2);
    }

    @Test
    public void run_clone_QPicture() {
        QPicture org = new QPicture();
        QPicture clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
        org.dispose();
        QPicture clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
        assertEquals(clone.size(), clone2.size());
    }

    @Test
    public void run_clone_QPixmap() {
        QPixmap org = new QPixmap(4, 5);
        QPixmap clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
        org.dispose();
        QPixmap clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
        assertEquals(clone.rect(), clone2.rect());
    }

    @Test
    public void run_clone_QRegion() {
        QRegion org = new QRegion();
        QRegion clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
        org.dispose();
        QRegion clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
        assertEquals(clone, clone2);
    }

    @Test
    public void run_clone_QStringMatcher() {
        QStringMatcher org = new QStringMatcher("pattern");
        QStringMatcher clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
        org.dispose();
        QStringMatcher clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
        assertEquals(clone.pattern(), clone2.pattern());
    }

    @Test
    public void run_clone_QStyleOption() {
        QStyleOption org = new QStyleOption();
        org.setRect(new QRect(5, 5, 1, 1));
        QStyleOption clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
        org.dispose();
        QStyleOption clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
        assertEquals(clone.rect(), clone2.rect());
    }

    @Test
    public void run_clone_QTextBlock() {
        QTextBlock org = new QTextBlock();
        QTextBlock clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
        org.dispose();
        QTextBlock clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
        assertEquals(clone, clone2);
    }

    @Test
    public void run_clone_QTextCursor() {
        QTextCursor org = new QTextCursor();
        QTextCursor clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
        org.dispose();
        QTextCursor clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
        assertEquals(clone, clone2);
    }

    @Test
    public void run_clone_QTextDocumentFragment() {
        QTextDocumentFragment org = new QTextDocumentFragment();
        QTextDocumentFragment clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
        org.dispose();
        QTextDocumentFragment clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
        assertEquals(clone.toPlainText(), clone2.toPlainText());
    }

    @Test
    public void run_clone_QTextFormat() {
        QTextFormat org = new QTextFormat();
        QTextFormat clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
        org.dispose();
        QTextFormat clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
        assertEquals(clone, clone2);
    }

    @Test
    public void run_clone_QTextFragment() {
        QTextFragment org = new QTextFragment();
        QTextFragment clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
        org.dispose();
        QTextFragment clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
        assertEquals(clone, clone2);
    }

    @Test
    public void run_clone_QTextFrame_iterator() {
        QTextFrame.iterator org = new QTextFrame.iterator();
        QTextFrame.iterator clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
        org.dispose();
        QTextFrame.iterator clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
        assertEquals(clone, clone2);
    }

    @Test
    public void run_clone_QTextOption() {
        QTextOption org = new QTextOption();
        org.setAlignment(AlignmentFlag.AlignHCenter);
        QTextOption clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
        org.dispose();
        QTextOption clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
        assertEquals(clone.alignment(), clone2.alignment());
    }

    @Test
    public void run_clone_QTextTableCell() {
        QTextTableCell org = new QTextTableCell();
        QTextTableCell clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
        org.dispose();
        QTextTableCell clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
        assertEquals(clone, clone2);
    }

    @Test
    public void run_clone_QTransform() {
        QTransform org = new QTransform();
        QTransform clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
        org.dispose();
        QTransform clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
        assertEquals(clone, clone2);
    }

    @Test
    public void run_clone_QUrl() {
        QUrl org = new QUrl();
        QUrl clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
        org.dispose();
        QUrl clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
        assertEquals(clone, clone2);
    }

    @Test
    public void run_clone_QXmlStreamAttribute() {
        QXmlStreamAttribute org = new QXmlStreamAttribute();
        QXmlStreamAttribute clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
        org.dispose();
        QXmlStreamAttribute clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
        assertEquals(clone, clone2);
    }

    @Test
    public void run_clone_QXmlStreamEntityDeclaration() {
        QXmlStreamEntityDeclaration org = new QXmlStreamEntityDeclaration();
        QXmlStreamEntityDeclaration clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
        org.dispose();
        QXmlStreamEntityDeclaration clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
        assertEquals(clone, clone2);
    }

	@Test
	public void run_clone_QDate() {
		QDate org = new QDate();
		QDate clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
		org.dispose();
		QDate clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
		assertEquals(clone.day(), clone2.day());
		assertEquals(clone.month(), clone2.month());
		assertEquals(clone.year(), clone2.year());
	}

	@Test
	public void run_clone_QEasingCurve() {
		QEasingCurve org = new QEasingCurve();
		QEasingCurve clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
		org.dispose();
		QEasingCurve clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
		assertEquals(clone, clone2);
	}

	@Test
	public void run_clone_QElapsedTimer() {
		QElapsedTimer org = new QElapsedTimer();
		QElapsedTimer clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
		org.dispose();
		QElapsedTimer clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
		assertEquals(clone, clone2);
	}

	@Test
	public void run_clone_QMargins() {
		QMargins org = new QMargins(1, 2, 3, 4);
		QMargins clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
		org.dispose();
		QMargins clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
		assertEquals(clone.top(), clone2.top());
		assertEquals(clone.bottom(), clone2.bottom());
		assertEquals(clone.left(), clone2.left());
		assertEquals(clone.right(), clone2.right());
	}

	@Test
	public void run_clone_QPoint() {
		QPoint org = new QPoint(1, 2);
		QPoint clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
		org.dispose();
		QPoint clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
		assertEquals(clone.x(), clone2.x());
		assertEquals(clone.y(), clone2.y());
	}

	@Test
	public void run_clone_QPointF() {
		QPointF org = new QPointF(3.3, 4.4);
		QPointF clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
		org.dispose();
		QPointF clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
		assertEquals((Object)clone.x(), (Object)clone2.x());
		assertEquals((Object)clone.y(), (Object)clone2.y());
	}

	@Test
	public void run_clone_QProcessEnvironment() {
		QProcessEnvironment org = new QProcessEnvironment();
		org.insert("VARIABLE", "value");
		QProcessEnvironment clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
		org.dispose();
		QProcessEnvironment clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
		assertEquals(clone.keys(), clone2.keys());
		for(String key : clone.keys()){
			assertEquals(clone.value(key), clone2.value(key));
		}
	}

	@Test
	public void run_clone_QRect() {
		QRect org = new QRect(10, 20, 30, 40);
		QRect clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
		org.dispose();
		QRect clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
		assertEquals(clone.top(), clone2.top());
		assertEquals(clone.bottom(), clone2.bottom());
		assertEquals(clone.left(), clone2.left());
		assertEquals(clone.right(), clone2.right());
	}

	@Test
	public void run_clone_QRectF() {
		QRectF org = new QRectF(10, 20, 30, 40);
		QRectF clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
		org.dispose();
		QRectF clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
		assertEquals((Object)clone.top(), clone2.top());
		assertEquals((Object)clone.bottom(), clone2.bottom());
		assertEquals((Object)clone.left(), clone2.left());
		assertEquals((Object)clone.right(), clone2.right());
	}

	@Test
	public void run_clone_QSize() {
		QSize org = new QSize(20, 60);
		QSize clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
		org.dispose();
		QSize clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
		assertEquals(clone.width(), clone2.width());
		assertEquals(clone.height(), clone2.height());
	}

	@Test
	public void run_clone_QSizeF() {
		QSizeF org = new QSizeF(20, 60);
		QSizeF clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
		org.dispose();
		QSizeF clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
		assertEquals((Object)clone.width(), clone2.width());
		assertEquals((Object)clone.height(), clone2.height());
	}

	@Test
	public void run_clone_QTextBoundaryFinder() {
		QTextBoundaryFinder org = new QTextBoundaryFinder(QTextBoundaryFinder.BoundaryType.Word, "A");
		QTextBoundaryFinder clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
		org.dispose();
		QTextBoundaryFinder clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
		assertEquals(clone.string(), clone2.string());
		assertEquals(clone.type(), clone2.type());
	}

	@Test
	public void run_clone_QTime() {
		QTime org = new QTime();
		QTime clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
		org.dispose();
		QTime clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
		assertEquals(clone.msec(), clone2.msec());
		assertEquals(clone.second(), clone2.second());
		assertEquals(clone.minute(), clone2.minute());
		assertEquals(clone.hour(), clone2.hour());
	}

	@Test
	public void run_clone_QUuid() {
		QUuid org = QUuid.createUuid();
		QUuid clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
		org.dispose();
		QUuid clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
		assertEquals(clone.toString(), clone2.toString());
	}

	public void run_clone_QXmlStreamAttributes() {
		QXmlStreamAttributes org = new QXmlStreamAttributes();
		org.append("x", "x");
		QXmlStreamAttributes clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
		org.dispose();
		QXmlStreamAttributes clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
		assertEquals(clone, clone2);
	}

	@Test
	public void run_clone_QXmlStreamNamespaceDeclaration() {
		QXmlStreamNamespaceDeclaration org = new QXmlStreamNamespaceDeclaration("prefix", "uri.org");
		QXmlStreamNamespaceDeclaration clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
		org.dispose();
		QXmlStreamNamespaceDeclaration clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
		assertEquals(clone.namespaceUri(), clone2.namespaceUri());
		assertEquals(clone.prefix(), clone2.prefix());
	}

	@Test
	public void run_clone_QXmlStreamNotationDeclaration() {
		QXmlStreamNotationDeclaration org = new QXmlStreamNotationDeclaration();
		QXmlStreamNotationDeclaration clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
		org.dispose();
		QXmlStreamNotationDeclaration clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
		assertEquals(clone, clone2);
	}
	
	@Test
	public void run_clone_QAbstractTextDocumentLayout_PaintContext() {
		QAbstractTextDocumentLayout.PaintContext org = new QAbstractTextDocumentLayout.PaintContext();
		org.setCursorPosition(5);
		QAbstractTextDocumentLayout.PaintContext clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
		org.dispose();
		QAbstractTextDocumentLayout.PaintContext clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
		assertEquals(clone.cursorPosition(), clone2.cursorPosition());
	}

	@Test
	public void run_clone_QAbstractTextDocumentLayout_Selection() {
		QAbstractTextDocumentLayout.Selection org = new QAbstractTextDocumentLayout.Selection();
		org.setFormat(new QTextCharFormat());
		QAbstractTextDocumentLayout.Selection clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
		org.dispose();
		QAbstractTextDocumentLayout.Selection clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
		assertEquals(clone.format(), clone2.format());
	}

	@Test
	public void run_clone_QBitmap() {
		QBitmap org = new QBitmap(100, 400);
		assertEquals(100, org.width());
		assertEquals(400, org.height());
		QBitmap clone = org.clone();
		assertEquals(org.width(), clone.width());
		assertEquals(org.height(), clone.height());
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
		org.dispose();
		QBitmap clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
		assertEquals(clone.width(), clone2.width());
		assertEquals(clone.height(), clone2.height());
	}

	@Test
	public void run_clone_QConicalGradient() {
		QConicalGradient org = new QConicalGradient();
		org.setAngle(50);
		org.setCenter(3.9, 4.1);
		QConicalGradient clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
		assertNotNull(clone);
		org.dispose();
		QConicalGradient clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
		assertNotNull(clone2);
		assertEquals((Object)clone.angle(), clone2.angle());
		assertEquals(clone.center(), clone2.center());
	}

	@Test
	public void run_clone_QInputMethodEvent_Attribute() {
		QInputMethodEvent.Attribute org = new QInputMethodEvent.Attribute(QInputMethodEvent.AttributeType.Language, 0, 1, "1");
		QInputMethodEvent.Attribute clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
		Assert.assertNotNull(clone);
		org.dispose();
		QInputMethodEvent.Attribute clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
		Assert.assertNotNull(clone2);
		assertEquals(clone.type(), clone2.type());
		assertEquals(clone.start(), clone2.start());
		assertEquals(clone.length(), clone2.length());
		assertEquals(clone.value(), clone2.value());
	}

	@Test
	public void run_clone_QItemSelection() {
		QStandardItemModel model = new QStandardItemModel();
		model.insertColumns(0, 5);
		model.insertRows(0, 5);
		QItemSelection org = new QItemSelection(model.index(0, 1), model.index(3, 4));
		QItemSelection clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
		org.dispose();
		QItemSelection clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
		assertEquals(clone.count(), clone2.count());
		for (int i = 0; i < clone.count(); i++) {
			assertEquals(clone.get(i), clone2.get(i));
			assertEquals(clone.get(i).bottom(), clone2.get(i).bottom());
			assertEquals(clone.get(i).top(), clone2.get(i).top());
			assertEquals(clone.get(i).left(), clone2.get(i).left());
			assertEquals(clone.get(i).right(), clone2.get(i).right());
		}
	}

	@Test
	public void run_clone_QItemSelectionRange() {
		QStandardItemModel model = new QStandardItemModel();
		model.insertColumns(0, 5);
		model.insertRows(0, 5);
		QItemSelectionRange org = new QItemSelectionRange(model.index(0, 1), model.index(3, 4));
		QItemSelectionRange clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
		org.dispose();
		QItemSelectionRange clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
		assertEquals(clone.bottom(), clone2.bottom());
		assertEquals(clone.top(), clone2.top());
		assertEquals(clone.left(), clone2.left());
		assertEquals(clone.right(), clone2.right());
	}

	@Test
	public void run_clone_QLine() {
		QLine org = new QLine(1, 5, 6, 9);
		QLine clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
		org.dispose();
		QLine clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
		assertEquals(clone.p1(), clone2.p1());
		assertEquals(clone.p2(), clone2.p2());
	}

	@Test
	public void run_clone_QLinearGradient() {
		QLinearGradient org = new QLinearGradient();
		org.setStart(3, 13);
		QLinearGradient clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
		assertNotNull(clone);
		org.dispose();
		QLinearGradient clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
		assertNotNull(clone2);
		assertEquals(clone.start(), clone2.start());
	}

	@Test
	public void run_clone_QLineF() {
		QLineF org = new QLineF(1,2,3,4);
		QLineF clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
		org.dispose();
		QLineF clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
		assertEquals(clone.p1(), clone2.p1());
		assertEquals(clone.p2(), clone2.p2());
	}

	@Test
	public void run_clone_QListWidgetItem() {
		QListWidgetItem org = new QListWidgetItem("itemtext");
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(org));
		QListWidgetItem clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
		org.dispose();
		QListWidgetItem clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
		assertEquals(clone.text(), clone2.text());
	}

	@Test
	public void run_clone_QMatrix4x4() {
		QMatrix4x4 org = new QMatrix4x4(1,2,3,4,5,6,7,8,9,10,11,12,3,14,15,16);
		QMatrix4x4 clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
		org.dispose();
		QMatrix4x4 clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
		assertEquals(clone, clone2);
	}

	@Test
	public void run_clone_QPainter_PixmapFragment() {
		QPainter.PixmapFragment org = new QPainter.PixmapFragment();
		org.setHeight(2);
		org.setWidth(4);
		QPainter.PixmapFragment clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
		org.dispose();
		QPainter.PixmapFragment clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
		assertEquals((Object)clone.width(), clone2.width());
		assertEquals((Object)clone.height(), clone2.height());
	}

	@Test
	public void run_clone_QPolygon() {
		QPolygon org = new QPolygon();
		org.append(6, 5);
		QPolygon clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
		org.dispose();
		QPolygon clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
		assertEquals(clone.get(0), clone2.get(0));
	}

	@Test
	public void run_clone_QPolygonF() {
		QPolygonF org = new QPolygonF();
		org.append(6, 5);
		QPolygonF clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
		org.dispose();
		QPolygonF clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
		assertEquals(clone.get(0), clone2.get(0));
	}

	@Test
	public void run_clone_QQuaternion() {
		QQuaternion org = new QQuaternion(0.0f,1.0f,2.0f,3.0f);
		QQuaternion clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
		org.dispose();
		QQuaternion clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
		assertEquals(1f, (Object)clone.x());
		assertEquals(2f, (Object)clone.y());
		assertEquals(3f, (Object)clone.z());
		assertEquals(0f, (Object)clone.scalar());
		assertEquals((Object)clone.x(), clone2.x());
		assertEquals((Object)clone.y(), clone2.y());
		assertEquals((Object)clone.z(), clone2.z());
		assertEquals((Object)clone.scalar(), clone2.scalar());
	}

	@Test
	public void run_clone_QRadialGradient() {
		QRadialGradient org = new QRadialGradient();
		org.setCenter(1, 2);
		QRadialGradient clone = org.clone();
		assertNotNull(clone);
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
		org.dispose();
		QRadialGradient clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
		assertNotNull(clone2);
		assertEquals(clone.center(), clone2.center());
		assertEquals((Object)clone.center().x(), 1.0);
		assertEquals((Object)clone.center().y(), 2.0);
	}

	@Test
	public void run_clone_QSizePolicy() {
		QSizePolicy org = new QSizePolicy(QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Maximum);
		QSizePolicy clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
		org.dispose();
		QSizePolicy clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
		assertEquals(clone.horizontalPolicy(), QSizePolicy.Policy.Expanding);
		assertEquals(clone.verticalPolicy(), QSizePolicy.Policy.Maximum);
		assertEquals(clone.horizontalPolicy(), clone2.horizontalPolicy());
		assertEquals(clone.verticalPolicy(), clone2.verticalPolicy());
	}

	@Test
	public void run_clone_QStandardItem() {
		QStandardItem org = new QStandardItem("text");
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(org));
		QStandardItem clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
		org.dispose();
		QStandardItem clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
		assertEquals(clone.text(), "text");
		assertEquals(clone.text(), clone2.text());
	}

	@Test
	public void run_clone_QStaticText() {
		QStaticText org = new QStaticText("statictext");
		QStaticText clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
		org.dispose();
		QStaticText clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
		assertEquals(clone.text(), "statictext");
		assertEquals(clone, clone2);
	}

	@Test
	public void run_clone_QTableWidgetItem() {
		QTableWidgetItem org = new QTableWidgetItem("text");
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(org));
		QTableWidgetItem clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
		org.dispose();
		QTableWidgetItem clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
		assertEquals(clone.text(), "text");
		assertEquals(clone.text(), clone2.text());
	}

	@Test
	public void run_clone_QTableWidgetSelectionRange() {
		QTableWidgetSelectionRange org = new QTableWidgetSelectionRange(1, 6, 5, 9);
		QTableWidgetSelectionRange clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
		org.dispose();
		QTableWidgetSelectionRange clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
		assertEquals(clone.topRow(), 1);
		assertEquals(clone.leftColumn(), 6);
		assertEquals(clone.bottomRow(), 5);
		assertEquals(clone.rightColumn(), 9);
		assertEquals(clone.topRow(), clone2.topRow());
		assertEquals(clone.leftColumn(), clone2.leftColumn());
		assertEquals(clone.bottomRow(), clone2.bottomRow());
		assertEquals(clone.rightColumn(), clone2.rightColumn());
	}

	@Test
	public void run_clone_QTextBlockFormat() {
		QTextBlockFormat org = new QTextBlockFormat();
		org.setTopMargin(1.45);
		QTextBlockFormat clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
		org.dispose();
		QTextBlockFormat clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
		assertEquals((Object)clone.topMargin(), 1.45);
		assertEquals((Object)clone.topMargin(), clone2.topMargin());
	}

	@Test
	public void run_clone_QTextCharFormat() {
		QTextCharFormat org = new QTextCharFormat();
		org.setFontPointSize(4.5);
		QTextCharFormat clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
		org.dispose();
		QTextCharFormat clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
		assertEquals((Object)clone.fontPointSize(), 4.5);
		assertEquals((Object)clone.fontPointSize(), clone2.fontPointSize());
	}

	@Test
	public void run_clone_QTextDocument() {
		QTextDocument org = new QTextDocument();
		org.setPlainText("plaintext");
		QTextDocument clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
		General.internalAccess.isJavaOwnership(org);
		General.internalAccess.isJavaOwnership(clone);
		org.dispose();
		QTextDocument clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
		General.internalAccess.isJavaOwnership(clone2);
		assertEquals(clone.toPlainText(), "plaintext");
		assertEquals(clone.toPlainText(), clone2.toPlainText());
	}

	@Test
	public void run_clone_QTextEdit_ExtraSelection() {
		QTextCharFormat format = new QTextCharFormat();
		format.setFontPointSize(4.5);
		QTextEdit.ExtraSelection org = new QTextEdit.ExtraSelection();
		org.setFormat(format);
		QTextEdit.ExtraSelection clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
		org.dispose();
		QTextEdit.ExtraSelection clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
		assertEquals((Object)clone.format().fontPointSize(), format.fontPointSize());
		assertEquals((Object)clone.format().fontPointSize(), clone2.format().fontPointSize());
	}

	@Test
	public void run_clone_QTextFrameFormat() {
		QTextFrameFormat org = new QTextFrameFormat();
		org.setBorder(4.5);
		QTextFrameFormat clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
		org.dispose();
		QTextFrameFormat clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
		assertEquals((Object)clone.border(), 4.5);
		assertEquals((Object)clone.border(), clone2.border());
	}

	@Test
	public void run_clone_QTextImageFormat() {
		QTextImageFormat org = new QTextImageFormat();
		org.setHeight(4.5);
		QTextImageFormat clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
		org.dispose();
		QTextImageFormat clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
		assertEquals((Object)clone.height(), 4.5);
		assertEquals((Object)clone.height(), clone2.height());
	}

	/**
	 * this test never could success because void initialized QTextInlineObject is completely instable.
	 * QTextInlineObject's default constructor does not belong to the Qt API and is thus declared private in QtJambi.
	 */
//	@Test
//	public void run_clone_QTextInlineObject() {
//		QTextInlineObject org = new QTextInlineObject();
//		org.setDescent(6.987);
//		QTextInlineObject clone = org.clone();
//		org.dispose();
//		QTextInlineObject clone2 = clone.clone();
//		assertEquals((Object)clone.descent(), 6.987);
//		assertEquals((Object)clone.descent(), clone2.descent());
//	}

	@Test
	public void run_clone_QTextLayout_FormatRange() {
		QTextLayout.FormatRange org = new QTextLayout.FormatRange();
		org.setLength(6);
		org.setStart(3);
		QTextLayout.FormatRange clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
		org.dispose();
		QTextLayout.FormatRange clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
		assertEquals(clone.start(), 3);
		assertEquals(clone.length(), 6);
		assertEquals(clone.start(), clone2.start());
		assertEquals(clone.length(), clone2.length());
	}

	@Test
	public void run_clone_QTextLength() {
		QTextLength org = new QTextLength();
		QTextLength clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
		org.dispose();
		QTextLength clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
		assertEquals(clone, clone2);
	}

//	@Test
	public void run_clone_QTextLine() {
		QTextLayout layout = new QTextLayout("\n\n");
		layout.beginLayout();
		QTextLine org = layout.createLine();
		org.setPosition(new QPointF(2,3));
		QTextLine clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
		org.dispose();
		QTextLine clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
		assertEquals((Object)clone.position().x(), 2.);
		assertEquals((Object)clone.position().y(), 3.);
		assertEquals((Object)clone.position(), clone2.position());
		layout.endLayout();
	}

	@Test
	public void run_clone_QTextListFormat() {
		QTextListFormat org = new QTextListFormat();
		org.setIndent(5);
		QTextListFormat clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
		org.dispose();
		QTextListFormat clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
		assertEquals(clone.indent(), 5);
		assertEquals(clone.indent(), clone2.indent());
	}

	@Test
	public void run_clone_QTextOption_Tab() {
		QTextOption.Tab org = new QTextOption.Tab(1.0, QTextOption.TabType.LeftTab ,'0');
		QTextOption.Tab clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
		org.dispose();
		QTextOption.Tab clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
		assertEquals(clone.delimiter(), '0');
		assertEquals(clone.type(), QTextOption.TabType.LeftTab);
		assertEquals((Object)clone.position(), 1.0);
		assertEquals(clone.delimiter(), clone2.delimiter());
		assertEquals(clone.type(), clone2.type());
		assertEquals((Object)clone.position(), clone2.position());
	}

	@Test
	public void run_clone_QTextTableCellFormat() {
		QTextTableCellFormat org = new QTextTableCellFormat();
		org.setLeftPadding(8.9);
		QTextTableCellFormat clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
		org.dispose();
		QTextTableCellFormat clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
		assertEquals((Object)clone.leftPadding(), 8.9);
		assertEquals((Object)clone.leftPadding(), clone2.leftPadding());
	}

	@Test
	public void run_clone_QTextTableFormat() {
		QTextTableFormat org = new QTextTableFormat();
		org.setBorder(6.6);
		QTextTableFormat clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
		org.dispose();
		QTextTableFormat clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
		assertEquals((Object)clone.border(), 6.6);
		assertEquals((Object)clone.border(), clone2.border());
	}

	@Test
	public void run_clone_QTileRules() {
		QTileRules org = new QTileRules(Qt.TileRule.StretchTile, Qt.TileRule.RoundTile);
		QTileRules clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
		org.dispose();
		QTileRules clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
		assertEquals(clone.horizontal(), Qt.TileRule.StretchTile);
		assertEquals(clone.vertical(), Qt.TileRule.RoundTile);
		assertEquals(clone.horizontal(), clone2.horizontal());
		assertEquals(clone.vertical(), clone2.vertical());
	}

	@Test
	public void run_clone_QTreeWidgetItem() {
		QTreeWidgetItem org = new QTreeWidgetItem(Collections.singletonList("text"));
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(org));
		QTreeWidgetItem clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
		org.dispose();
		QTreeWidgetItem clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
		assertEquals(clone.text(0), "text");
		assertEquals(clone.text(0), clone2.text(0));
	}

	@Test
	public void run_clone_QVector2D() {
		QVector2D org = new QVector2D(1,2);
		QVector2D clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
		org.dispose();
		QVector2D clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
		assertEquals(1f, (Object)clone.x());
		assertEquals(2f, (Object)clone.y());
		assertEquals((Object)clone.x(), clone2.x());
		assertEquals((Object)clone.y(), clone2.y());
	}

	@Test
	public void run_clone_QVector3D() {
		QVector3D org = new QVector3D(1,2,3);
		QVector3D clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
		org.dispose();
		QVector3D clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
		assertEquals(1f, (Object)clone.x());
		assertEquals(2f, (Object)clone.y());
		assertEquals(3f, (Object)clone.z());
		assertEquals((Object)clone.x(), clone2.x());
		assertEquals((Object)clone.y(), clone2.y());
		assertEquals((Object)clone.z(), clone2.z());
	}

	@Test
	public void run_clone_QVector4D() {
		QVector4D org = new QVector4D(1,2,3,0);
		QVector4D clone = org.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone));
		org.dispose();
		QVector4D clone2 = clone.clone();
		assertTrue("not java ownership", General.internalAccess.isJavaOwnership(clone2));
		assertEquals(1f, (Object)clone.x());
		assertEquals(2f, (Object)clone.y());
		assertEquals(3f, (Object)clone.z());
		assertEquals(0f, (Object)clone.w());
		assertEquals((Object)clone.x(), clone2.x());
		assertEquals((Object)clone.y(), clone2.y());
		assertEquals((Object)clone.z(), clone2.z());
		assertEquals((Object)clone.w(), clone2.w());
	}
	
    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestCloneable.class.getName());
    }
}
