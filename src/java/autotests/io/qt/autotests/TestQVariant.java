/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
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
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
package io.qt.autotests;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertTrue;

import java.lang.reflect.Method;
import java.util.ArrayDeque;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.Deque;
import java.util.HashMap;
import java.util.HashSet;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.Queue;
import java.util.Set;
import java.util.TreeMap;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.function.Consumer;

import org.junit.Assert;
import org.junit.Assume;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.QtEnumerator;
import io.qt.QtObject;
import io.qt.QtUninvokable;
import io.qt.QtUtilities;
import io.qt.autotests.generated.Tulip;
import io.qt.autotests.generated.Variants;
import io.qt.core.QAbstractEventDispatcher;
import io.qt.core.QBitArray;
import io.qt.core.QByteArray;
import io.qt.core.QCborArray;
import io.qt.core.QCborMap;
import io.qt.core.QCoreApplication;
import io.qt.core.QDate;
import io.qt.core.QDateTime;
import io.qt.core.QDir;
import io.qt.core.QEasingCurve;
import io.qt.core.QEvent;
import io.qt.core.QFileInfo;
import io.qt.core.QHash;
import io.qt.core.QJsonArray;
import io.qt.core.QJsonDocument;
import io.qt.core.QJsonObject;
import io.qt.core.QJsonValue;
import io.qt.core.QLibraryInfo;
import io.qt.core.QLine;
import io.qt.core.QLineF;
import io.qt.core.QList;
import io.qt.core.QLocale;
import io.qt.core.QMap;
import io.qt.core.QMetaMethod;
import io.qt.core.QMetaObject;
import io.qt.core.QMetaType;
import io.qt.core.QModelIndex;
import io.qt.core.QMultiHash;
import io.qt.core.QMultiMap;
import io.qt.core.QObject;
import io.qt.core.QPair;
import io.qt.core.QPersistentModelIndex;
import io.qt.core.QPoint;
import io.qt.core.QPointF;
import io.qt.core.QQueue;
import io.qt.core.QRect;
import io.qt.core.QRectF;
import io.qt.core.QRegularExpression;
import io.qt.core.QRunnable;
import io.qt.core.QSet;
import io.qt.core.QSize;
import io.qt.core.QSizeF;
import io.qt.core.QStack;
import io.qt.core.QStringList;
import io.qt.core.QThread;
import io.qt.core.QTime;
import io.qt.core.QUrl;
import io.qt.core.QUuid;
import io.qt.core.QVariant;
import io.qt.core.QVersionNumber;
import io.qt.core.Qt;
import io.qt.gui.QBitmap;
import io.qt.gui.QBrush;
import io.qt.gui.QColor;
import io.qt.gui.QCursor;
import io.qt.gui.QFont;
import io.qt.gui.QIcon;
import io.qt.gui.QImage;
import io.qt.gui.QKeySequence;
import io.qt.gui.QMatrix4x4;
import io.qt.gui.QPageSize;
import io.qt.gui.QPaintDevice;
import io.qt.gui.QPaintEngine;
import io.qt.gui.QPainter;
import io.qt.gui.QPalette;
import io.qt.gui.QPen;
import io.qt.gui.QPixmap;
import io.qt.gui.QPolygon;
import io.qt.gui.QPolygonF;
import io.qt.gui.QQuaternion;
import io.qt.gui.QRegion;
import io.qt.gui.QTextFormat;
import io.qt.gui.QTextLength;
import io.qt.gui.QTransform;
import io.qt.gui.QVector2D;
import io.qt.gui.QVector3D;
import io.qt.gui.QVector4D;
import io.qt.widgets.QApplication;
import io.qt.widgets.QComboBox;
import io.qt.widgets.QGraphicsItem;
import io.qt.widgets.QGraphicsItemGroup;
import io.qt.widgets.QGraphicsObject;
import io.qt.widgets.QGraphicsPixmapItem;
import io.qt.widgets.QGraphicsSimpleTextItem;
import io.qt.widgets.QGraphicsWidget;
import io.qt.widgets.QLabel;
import io.qt.widgets.QSizePolicy;
import io.qt.widgets.QSpinBox;
import io.qt.widgets.QStyleOptionGraphicsItem;
import io.qt.widgets.QWidget;

public class TestQVariant extends ApplicationInitializer {
	
	@BeforeClass
    public static void testInitialize() throws Exception {
    	ApplicationInitializer.testInitializeWithWidgets();
    }
	
	private static boolean convertToQList = QLibraryInfo.version().compareTo(new QVersionNumber(6, 1, 0)) >= 0;
	
    @Test
    public void testQVariant() {
    	assertEquals(QMetaType.Type.Int.value(), QMetaType.qMetaTypeId(int.class));
    	assertEquals(QMetaType.Type.LongLong.value(), QMetaType.qMetaTypeId(long.class));
    	assertEquals(QMetaType.Type.Bool.value(), QMetaType.qMetaTypeId(boolean.class));
    	assertEquals(QMetaType.Type.Short.value(), QMetaType.qMetaTypeId(short.class));
    	assertEquals(QMetaType.Type.SChar.value(), QMetaType.qMetaTypeId(byte.class));
    	assertEquals(QMetaType.Type.Double.value(), QMetaType.qMetaTypeId(double.class));
    	assertEquals(QMetaType.Type.Float.value(), QMetaType.qMetaTypeId(float.class));
    	assertEquals(QMetaType.Type.QChar.value(), QMetaType.qMetaTypeId(char.class));
    	assertEquals(QMetaType.Type.Int.value(), QMetaType.qMetaTypeId(Integer.class));
    	assertEquals(QMetaType.Type.LongLong.value(), QMetaType.qMetaTypeId(Long.class));
    	assertEquals(QMetaType.Type.Bool.value(), QMetaType.qMetaTypeId(Boolean.class));
    	assertEquals(QMetaType.Type.Short.value(), QMetaType.qMetaTypeId(Short.class));
    	assertEquals(QMetaType.Type.SChar.value(), QMetaType.qMetaTypeId(Byte.class));
    	assertEquals(QMetaType.Type.Double.value(), QMetaType.qMetaTypeId(Double.class));
    	assertEquals(QMetaType.Type.Float.value(), QMetaType.qMetaTypeId(Float.class));
    	assertEquals(QMetaType.Type.QChar.value(), QMetaType.qMetaTypeId(Character.class));
    	assertEquals(QMetaType.Type.QBitArray.value(), QMetaType.qMetaTypeId(QBitArray.class));
    	assertEquals(QMetaType.Type.QByteArray.value(), QMetaType.qMetaTypeId(QByteArray.class));
    	assertEquals(QMetaType.Type.QBrush.value(), QMetaType.qMetaTypeId(QBrush.class));
    	assertEquals(QMetaType.Type.QBitmap.value(), QMetaType.qMetaTypeId(QBitmap.class));
    	assertEquals(QMetaType.Type.QCborArray.value(), QMetaType.qMetaTypeId(QCborArray.class));
    	assertEquals(QMetaType.Type.QCborMap.value(), QMetaType.qMetaTypeId(QCborMap.class));
//    	assertEquals(QMetaType.Type.QCborValue.value(), QMetaType.qMetaTypeId(QCborValue.class));
    	assertEquals(QMetaType.Type.QColor.value(), QMetaType.qMetaTypeId(QColor.class));
    	assertEquals(QMetaType.Type.QCursor.value(), QMetaType.qMetaTypeId(QCursor.class));
    	assertEquals(QMetaType.Type.QDate.value(), QMetaType.qMetaTypeId(QDate.class));
    	assertEquals(QMetaType.Type.QDateTime.value(), QMetaType.qMetaTypeId(QDateTime.class));
    	assertEquals(QMetaType.Type.QEasingCurve.value(), QMetaType.qMetaTypeId(QEasingCurve.class));
    	assertEquals(QMetaType.Type.QFont.value(), QMetaType.qMetaTypeId(QFont.class));
    	assertEquals(QMetaType.Type.QIcon.value(), QMetaType.qMetaTypeId(QIcon.class));
    	assertEquals(QMetaType.Type.QImage.value(), QMetaType.qMetaTypeId(QImage.class));
    	assertEquals(QMetaType.Type.QJsonArray.value(), QMetaType.qMetaTypeId(QJsonArray.class));
    	assertEquals(QMetaType.Type.QJsonDocument.value(), QMetaType.qMetaTypeId(QJsonDocument.class));
    	assertEquals(QMetaType.Type.QJsonObject.value(), QMetaType.qMetaTypeId(QJsonObject.class));
    	assertEquals(QMetaType.Type.QJsonValue.value(), QMetaType.qMetaTypeId(QJsonValue.class));
    	assertEquals(QMetaType.Type.QKeySequence.value(), QMetaType.qMetaTypeId(QKeySequence.class));
    	assertEquals(QMetaType.Type.QLine.value(), QMetaType.qMetaTypeId(QLine.class));
    	assertEquals(QMetaType.Type.QLineF.value(), QMetaType.qMetaTypeId(QLineF.class));
    	assertEquals(QMetaType.Type.QLocale.value(), QMetaType.qMetaTypeId(QLocale.class));
    	assertEquals(QMetaType.Type.QMatrix4x4.value(), QMetaType.qMetaTypeId(QMatrix4x4.class));
    	assertEquals(QMetaType.Type.QModelIndex.value(), QMetaType.qMetaTypeId(QModelIndex.class));
    	assertEquals(QMetaType.Type.QPalette.value(), QMetaType.qMetaTypeId(QPalette.class));
    	assertEquals(QMetaType.Type.QPen.value(), QMetaType.qMetaTypeId(QPen.class));
    	assertEquals(QMetaType.Type.QPersistentModelIndex.value(), QMetaType.qMetaTypeId(QPersistentModelIndex.class));
    	assertEquals(QMetaType.Type.QPixmap.value(), QMetaType.qMetaTypeId(QPixmap.class));
    	assertEquals(QMetaType.Type.QPoint.value(), QMetaType.qMetaTypeId(QPoint.class));
    	assertEquals(QMetaType.Type.QPointF.value(), QMetaType.qMetaTypeId(QPointF.class));
    	assertEquals(QMetaType.Type.QPolygon.value(), QMetaType.qMetaTypeId(QPolygon.class));
    	assertEquals(QMetaType.Type.QPolygonF.value(), QMetaType.qMetaTypeId(QPolygonF.class));
    	assertEquals(QMetaType.Type.QQuaternion.value(), QMetaType.qMetaTypeId(QQuaternion.class));
    	assertEquals(QMetaType.Type.QRect.value(), QMetaType.qMetaTypeId(QRect.class));
    	assertEquals(QMetaType.Type.QRectF.value(), QMetaType.qMetaTypeId(QRectF.class));
    	assertEquals(QMetaType.Type.QRegion.value(), QMetaType.qMetaTypeId(QRegion.class));
    	assertEquals(QMetaType.Type.QRegularExpression.value(), QMetaType.qMetaTypeId(QRegularExpression.class));
    	assertEquals(QMetaType.Type.QQuaternion.value(), QMetaType.qMetaTypeId(QQuaternion.class));
    	assertEquals(QMetaType.Type.QSize.value(), QMetaType.qMetaTypeId(QSize.class));
    	assertEquals(QMetaType.Type.QSizeF.value(), QMetaType.qMetaTypeId(QSizeF.class));
    	assertEquals(QMetaType.Type.QSizePolicy.value(), QMetaType.qMetaTypeId(QSizePolicy.class));
    	assertEquals(QMetaType.Type.QString.value(), QMetaType.qMetaTypeId(String.class));
    	assertEquals(QMetaType.Type.QTextFormat.value(), QMetaType.qMetaTypeId(QTextFormat.class));
    	assertEquals(QMetaType.Type.QTextLength.value(), QMetaType.qMetaTypeId(QTextLength.class));
    	assertEquals(QMetaType.Type.QTime.value(), QMetaType.qMetaTypeId(QTime.class));
    	assertEquals(QMetaType.Type.QTransform.value(), QMetaType.qMetaTypeId(QTransform.class));
    	assertEquals(QMetaType.Type.QUrl.value(), QMetaType.qMetaTypeId(QUrl.class));
    	assertEquals(QMetaType.Type.QUuid.value(), QMetaType.qMetaTypeId(QUuid.class));
    	assertEquals(QMetaType.Type.QVector2D.value(), QMetaType.qMetaTypeId(QVector2D.class));
    	assertEquals(QMetaType.Type.QVector3D.value(), QMetaType.qMetaTypeId(QVector3D.class));
    	assertEquals(QMetaType.Type.QVector4D.value(), QMetaType.qMetaTypeId(QVector4D.class));
    	assertEquals(QMetaType.Type.QVariant.value(), QMetaType.qMetaTypeId(QVariant.class));
    	assertEquals(QMetaType.Type.QObjectStar.value(), QMetaType.qMetaTypeId(QObject.class));
    	assertEquals(QMetaType.Type.Nullptr.value(), QMetaType.qMetaTypeId(null));
    	assertEquals("JObjectWrapper", QMetaType.fromType(Object.class).name().toString());
    	assertEquals("JCollectionWrapper", QMetaType.fromType(List.class).name().toString());
    	assertEquals("JMapWrapper", QMetaType.fromType(Map.class).name().toString());
    	assertEquals("JEnumWrapper", QMetaType.fromType(Enum.class).name().toString());
    	assertEquals("QWidget*", QMetaType.fromType(QWidget.class).name().toString());
    	assertEquals(new QSize(5, 5), QVariant.convert(new QSizeF(5, 5), QSize.class));
    	assertEquals(new QLine(6, 5, 8, 1), QVariant.convert(new QLineF(5.8, 5.2, 8.3, 1.2), QLine.class));
    	Tulip tulip = new Tulip();
    	assertEquals(new QMetaType(QMetaType.Type.QStringList), QMetaType.fromObject(tulip.listRef()));
    	assertEquals(new QMetaType(QMetaType.Type.QByteArrayList), QMetaType.fromObject(tulip.byteArrayListRef()));
    }
    
    @Test
    public void testQVariant_IntList() {
    	Object variant = Variants.getIntList();
    	assertNotNull(variant);
    	assertTrue(variant.getClass().getName(), variant instanceof QList);
    	assertEquals(QList.of(1, 2, 3, 4), variant);
    }
    
    @Test
    public void testQVariant_IntSet() {
    	Object variant = Variants.getIntSet();
    	assertNotNull(variant);
    	assertTrue(variant.getClass().getName(), variant instanceof QSet);
    	assertEquals(QSet.of(1, 2, 3, 4), variant);
    }
    
    @Test
    public void testQVariant_IntQueue() {
    	Object variant = Variants.getIntQueue();
    	assertNotNull(variant);
    	assertTrue(variant.getClass().getName(), variant instanceof QQueue);
    	assertEquals(QQueue.of(1, 2, 3, 4), variant);
    }
    
    @Test
    public void testQVariant_IntStack() {
    	Object variant = Variants.getIntStack();
    	assertNotNull(variant);
    	assertTrue(variant.getClass().getName(), variant instanceof QStack);
    	assertEquals(QStack.of(1, 2, 3, 4), variant);
    }
    
    @Test
    public void testQVariant_StringList() {
    	Object variant = Variants.getStringList();
    	assertNotNull(variant);
    	assertTrue(variant.getClass().getName(), variant instanceof QList);
    	assertEquals(new QStringList("S1", "S2", "S3", "S4"), variant);
    }
    
    @Test
    public void testQVariant_StringSet() {
    	Object variant = Variants.getStringSet();
    	assertNotNull(variant);
    	assertTrue(variant.getClass().getName(), variant instanceof QSet);
    	if(QLibraryInfo.version().majorVersion()<6) {
    		assertEquals(new HashSet<>(Arrays.asList("S1", "S2", "S3", "S4")), new HashSet<>((Collection<?>)variant));
    	} else {
    		assertEquals(QSet.of("S1", "S2", "S3", "S4"), variant);	
		}
    }
    
    @Test
    public void testQVariant_StringQueue() {
    	Object variant = Variants.getStringQueue();
    	assertNotNull(variant);
    	assertTrue(variant.getClass().getName(), variant instanceof QQueue);
    	assertEquals(QQueue.of("S1", "S2", "S3", "S4"), variant);
    }
    
    @Test
    public void testQVariant_StringStack() {
    	Object variant = Variants.getStringStack();
    	assertNotNull(variant);
    	assertTrue(variant.getClass().getName(), variant instanceof QStack);
    	assertEquals(QStack.of("S1", "S2", "S3", "S4"), variant);
    }
    
    @Test
    public void testQVariant_UnknownEnumList() {
    	Object variant = Variants.getUnknownEnumList();
    	assertNotNull(variant);
    	assertTrue(variant.getClass().getName(), variant instanceof QList);
    	assertEquals(4, ((Collection<?>)variant).size());
    	int i = 101;
    	for(Object e : (Collection<?>)variant) {
    		assertTrue(e instanceof QtEnumerator);
    		assertEquals(i++, ((QtEnumerator)e).value());
    	}
    }
    
    @Test
    public void testQVariant_UnknownEnumSet() {
    	Object variant = Variants.getUnknownEnumSet();
    	assertNotNull(variant);
    	assertTrue(variant.getClass().getName(), variant instanceof QSet);
    	assertEquals(4, ((Collection<?>)variant).size());
    	QSet<Integer> values = new QSet<>(int.class);
    	for(Object e : (Collection<?>)variant) {
    		assertTrue(e instanceof QtEnumerator);
    		values.add(((QtEnumerator)e).value());
    	}
    	assertEquals(QSet.of(101, 102, 103, 104), values);
    }
    
    @Test
    public void testQVariant_UnknownEnumQueue() {
    	Object variant = Variants.getUnknownEnumQueue();
    	assertNotNull(variant);
    	assertTrue(variant.getClass().getName(), variant instanceof QQueue);
    	assertEquals(4, ((Collection<?>)variant).size());
    	int i = 101;
    	for(Object e : (Collection<?>)variant) {
    		assertTrue(e instanceof QtEnumerator);
    		assertEquals(i++, ((QtEnumerator)e).value());
    	}
    }
    
    @Test
    public void testQVariant_UnknownEnumStack() {
    	Object variant = Variants.getUnknownEnumStack();
    	assertNotNull(variant);
    	assertTrue(variant.getClass().getName(), variant instanceof QStack);
    	assertEquals(4, ((Collection<?>)variant).size());
    	int i = 101;
    	for(Object e : (Collection<?>)variant) {
    		assertTrue(e instanceof QtEnumerator);
    		assertEquals(i++, ((QtEnumerator)e).value());
    	}
    }
    
    @Test
    public void testQVariant_DirList() {
    	Object variant = Variants.getDirList();
    	assertNotNull(variant);
    	assertTrue(variant.getClass().getName(), variant instanceof QList);
    	assertEquals(QList.of(new QDir("/"), new QDir("/home")), variant);
    }
    
    @Test
    public void testQVariant_DirSet() {
    	Object variant = Variants.getDirSet();
    	assertNotNull(variant);
    	assertTrue(variant.getClass().getName(), variant instanceof QSet);
    	assertEquals(new HashSet<>(Arrays.asList(new QDir("/"), new QDir("/home"))), variant);
    }
    
    @Test
    public void testQVariant_DirQueue() {
    	Object variant = Variants.getDirQueue();
    	assertNotNull(variant);
    	assertTrue(variant.getClass().getName(), variant instanceof Queue);
    	assertEquals(Arrays.asList(new QDir("/"), new QDir("/home")), new ArrayList<>((Queue<?>)variant));
    }
    
    @Test
    public void testQVariant_DirStack() {
    	Object variant = Variants.getDirStack();
    	assertNotNull(variant);
    	assertTrue(variant.getClass().getName(), variant instanceof QStack);
    	assertEquals(Arrays.asList(new QDir("/"), new QDir("/home")), new ArrayList<>((QStack<?>)variant));
    }
    
	@Test
    public void testQVariant_ColorHash() {
    	Object variant = Variants.getColorHash();
    	assertNotNull(variant);
    	assertTrue(variant.getClass().getName(), variant instanceof QHash);
    	for(Map.Entry<?,?> entry : ((Map<?,?>)variant).entrySet()) {
        	assertTrue(entry.getKey() instanceof QColor);
        	assertTrue(entry.getValue() instanceof String);
    	}
    	assertEquals(3, ((Map<?,?>)variant).size());
    	assertEquals("red", ((Map<?,?>)variant).get(new QColor(Qt.GlobalColor.red)));
    	assertEquals("green", ((Map<?,?>)variant).get(new QColor(Qt.GlobalColor.green)));
    	assertEquals("blue", ((Map<?,?>)variant).get(new QColor(Qt.GlobalColor.blue)));
    }
    
    @Test
    public void testQVariant_IntMap() {
    	Object variant = Variants.getIntMap();
    	assertNotNull(variant);
    	assertTrue(variant.getClass().getName(), variant instanceof QMap);
    	for(QPair<?,?> entry : (QMap<?,?>)variant) {
        	assertTrue(entry.first instanceof Integer);
        	assertTrue(entry.second instanceof String);
    	}
    	assertEquals(3, ((Map<?,?>)variant).size());
    	assertEquals("Seven", ((Map<?,?>)variant).get(7));
    	assertEquals("Eleven", ((Map<?,?>)variant).get(11));
    	assertEquals("Forty-two", ((Map<?,?>)variant).get(42));
    }
    
    @Test
    public void testQVariant_UnknownEnumMap() {
    	Object variant = Variants.getUnknownEnumMap();
    	assertNotNull(variant);
    	assertTrue(variant.getClass().getName(), variant instanceof QMap);
    	for(Map.Entry<?,?> entry : ((Map<?,?>)variant).entrySet()) {
        	assertTrue(entry.getKey() instanceof QtEnumerator);
        	assertEquals(entry.getValue(), ( (QtEnumerator)entry.getKey() ).value()-100);
    	}
    }
    
    @Test
    public void testQVariant_UnknownEnumHash() {
    	Object variant = Variants.getUnknownEnumHash();
    	assertNotNull(variant);
    	assertTrue(variant.getClass().getName(), variant instanceof QHash);
    	for(Map.Entry<?,?> entry : ((Map<?,?>)variant).entrySet()) {
        	assertTrue(entry.getKey() instanceof QtEnumerator);
        	assertEquals(entry.getValue(), ( (QtEnumerator)entry.getKey() ).value()-100);
    	}
    }
    
    static final Consumer<Collection<?>> variantConsumer = collection -> {
    	assertEquals(4, collection.size());
		int i=1;
		for (Object object : collection) {
    		assertTrue(object instanceof QObject);
			QObject qo = (QObject)object;
    		assertEquals("VariantTestObject", qo.metaObject().className());
    		assertEquals("VariantTestObject"+i, qo.objectName());
    		++i;
		}
	};
    
    @Test
    public void testQVariant_DirHash() {
    	Object variant = Variants.getDirHash();
    	assertNotNull(variant);
    	assertTrue(variant.getClass().getName(), variant instanceof QHash);
    }
    
    @Test
    public void testQVariant_TestObjectList() {
    	Object variant = Variants.getTestObjectList();
    	assertNotNull(variant);
    	assertTrue(variant.getClass().getName(), variant instanceof QList);
    	variantConsumer.accept((List<?>)variant);
    }
    
    @Test
    public void testQVariant_TestObjectSet() {
    	Object variant = Variants.getTestObjectSet();
    	assertNotNull(variant);
    	assertTrue(variant.getClass().getName(), variant instanceof QSet);
    	assertEquals(4, ((Collection<?>)variant).size());
    	for(Object o : (Collection<?>)variant) {
    		assertTrue(o instanceof QObject);
    		QObject qobject = (QObject)o;
    		assertEquals("VariantTestObject", qobject.metaObject().className());
    	}
    }
    
    @Test
    public void testQVariant_QObjectSet() {
    	Object variant = Variants.getQObjectSet();
    	assertNotNull(variant);
    	assertTrue(variant.getClass().getName(), variant instanceof QSet);
    	Set<?> set = (Set<?>)variant;
    	set.size();
    	HashSet<?> eqSet = new HashSet<>(Arrays.asList(QCoreApplication.instance(), QThread.currentThread(), QAbstractEventDispatcher.instance()));
    	set.equals(eqSet);
    	java.util.Iterator<?> iter = set.iterator();
    	if(iter.hasNext()) {
    		iter.next();
    	}
    	assertEquals(eqSet, variant);
    }
    
    @Test
    public void testQVariant_TestObjectLinkedQueue() {
    	Object variant = Variants.getTestObjectQueue();
    	assertNotNull(variant);
    	assertTrue(variant.getClass().getName(), variant instanceof QQueue);
    	variantConsumer.accept((Queue<?>)variant);
    }
    
    @Test
    public void testQVariant_TestObjectStack() {
    	Object variant = Variants.getTestObjectStack();
    	assertNotNull(variant);
    	assertTrue(variant.getClass().getName(), variant instanceof QStack);
    	variantConsumer.accept((QStack<?>)variant);
    }
    
    @Test
    public void testQVariant_TestObjectMap() {
    	Object variant = Variants.getTestObjectMap();
    	assertNotNull(variant);
    	assertTrue(variant.getClass().getName(), variant instanceof QMap);
    	assertEquals(4, ((Map<?,?>)variant).size());
    	for(Map.Entry<?, ?> entry : ((Map<?,?>)variant).entrySet()) {
    		assertTrue(entry.getKey() instanceof QObject);
    		assertTrue(entry.getValue() instanceof Integer);
   			assertEquals("VariantTestObject", ((QObject)entry.getKey()).metaObject().className());
    		String objectName = ((QObject)entry.getKey()).objectName();
    		if(((Integer)entry.getValue())==100) {
    			assertTrue(objectName.startsWith("VariantTestObject"));
    		}else {
    			assertEquals(String.format("VariantTestObject%1$s", entry.getValue()), objectName);
    		}
    	}
    }
    
    @Test
    public void testQVariant_TestObjectHash() {
    	Object variant = Variants.getTestObjectHash();
    	assertNotNull(variant);
    	assertTrue(variant.getClass().getName(), variant instanceof QHash);
    	assertEquals(4, ((Map<?,?>)variant).size());
    	for(Map.Entry<?, ?> entry : ((Map<?,?>)variant).entrySet()) {
    		assertTrue(entry.getKey() instanceof QObject);
    		assertTrue(entry.getValue() instanceof Integer);
    		String objectName = ((QObject)entry.getKey()).objectName();
   			assertEquals("VariantTestObject", ((QObject)entry.getKey()).metaObject().className());
   			assertEquals(String.format("VariantTestObject%1$s", entry.getValue()), objectName);
    	}
    }
    
    @Test
    public void testQVariant_UnknownValueList() {
    	Object variant = Variants.getUnknownValueList();
    	assertNotNull(variant);
    	assertTrue(variant.getClass().getName(), variant instanceof QList);
    	assertEquals(1, ((Collection<?>)variant).size());
    	for(Object o : (Collection<?>)variant) {
    		assertTrue(o instanceof QMetaType.GenericValue);
    		assertEquals("UnknownValue", ((QMetaType.GenericTypeInterface)o).metaType().name().toString());
    	}
    }
    
    @Test
    public void testQVariant_UnknownValueHash() {
    	Object variant = Variants.getUnknownValueHash();
    	assertNotNull(variant);
    	assertTrue(variant.getClass().getName(), variant instanceof Map);
    	assertEquals(1, ((Map<?,?>)variant).size());
    	for(Map.Entry<?, ?> entry : ((Map<?,?>)variant).entrySet()) {
    		assertTrue(entry.getKey() instanceof QMetaType.GenericValue);
    		assertEquals("UnknownValue", ((QMetaType.GenericTypeInterface)entry.getKey()).metaType().name().toString());
    		assertEquals(1, entry.getValue());
    	}
    }
    
    @Test
    public void testQVariant_UnknownValueMap() {
    	Object variant = Variants.getUnknownValueMap();
    	assertTrue(variant!=null);
    	assertTrue(variant.getClass().getName(), variant instanceof QMap);
    	assertEquals(1, ((Map<?,?>)variant).size());
    	for(Map.Entry<?, ?> entry : ((Map<?,?>)variant).entrySet()) {
    		assertTrue(entry.getKey() instanceof QMetaType.GenericValue);
    		assertEquals("UnknownValue", ((QMetaType.GenericTypeInterface)entry.getKey()).metaType().name().toString());
    		assertEquals(1, entry.getValue());
    	}
    }
    	
    @Test
    public void testQVariant_StringMultiMap() {
    	Object variant = Variants.getStringMultiMap();
    	assertNotNull(variant);
    	assertTrue(variant.getClass().getName(), variant instanceof QMultiMap);
    	assertEquals(8, ((QMultiMap<?,?>)variant).size());
    	for(Map.Entry<?, ?> entry : ((QMultiMap<?,?>)variant).entrySet()) {
    		assertTrue(""+entry.getKey(), entry.getKey() instanceof String);
    		assertTrue(""+entry.getKey(), entry.getKey().toString().startsWith("TEST"));
    		assertTrue(""+entry.getValue(), entry.getValue() instanceof List);
    		List<?> list = (List<?>)entry.getValue();
    		assertEquals(2, list.size());
    		for (Object object : list) {
    			assertTrue(object instanceof Integer);
			}
    		assertEquals(list.get(0), 200 + (Integer)list.get(1));
    	}
    }
    	
    @Test
    public void testQVariant_TestObjectMultiMap() {
    	Object variant = Variants.getTestObjectMultiMap();
    	assertNotNull(variant);
    	assertTrue(variant.getClass().getName(), variant instanceof QMultiMap);
    	for(Map.Entry<?, ?> entry : ((Map<?,?>)variant).entrySet()) {
    		assertTrue(entry.getKey() instanceof QObject);
    		assertEquals("VariantTestObject", ((QObject)entry.getKey()).metaObject().className());
    		assertTrue(((QObject)entry.getKey()).objectName().startsWith("VariantTestObject"));
    		assertTrue(entry.getValue() instanceof List);
    		List<?> list = (List<?>)entry.getValue();
    		assertEquals(2, list.size());
    		for (Object object : list) {
    			assertTrue(object instanceof Integer);
			}
    		assertEquals(list.get(0), 200 + (Integer)list.get(1));
    	}
    }
    	
    @Test
    public void testQVariant_Int16MultiMap() {
    	Object variant = Variants.getInt16MultiMap();
    	assertNotNull(variant);
    	assertTrue(variant.getClass().getName(), variant instanceof QMultiMap);
    	for(Map.Entry<?, ?> entry : ((Map<?,?>)variant).entrySet()) {
    		assertTrue(entry.getKey() instanceof Short);
    		assertTrue(Short.valueOf((short)3).equals(entry.getKey()) || Short.valueOf((short)7).equals(entry.getKey()));
    		assertTrue(entry.getValue() instanceof List);
    		List<?> list = (List<?>)entry.getValue();
    		assertEquals(2, list.size());
    		for (Object object : list) {
    			assertTrue("unexpeced value: "+object,
    					   Long.valueOf(1).equals(object)
    					|| Long.valueOf(2).equals(object)
    					|| Long.valueOf(501).equals(object)
    					|| Long.valueOf(502).equals(object)
					);
			}
    	}
    }
    	
    @Test
    public void testQVariant_StringMultiHash() {
    	Object variant = Variants.getStringMultiHash();
    	assertNotNull(variant);
    	assertTrue(variant.getClass().getName(), variant instanceof QMultiHash);
    	for(Map.Entry<?, ?> entry : ((Map<?,?>)variant).entrySet()) {
    		assertTrue(entry.getKey() instanceof String);
    		assertTrue(entry.getValue() instanceof List);
    		List<?> list = (List<?>)entry.getValue();
    		assertEquals(2, list.size());
    		for (Object object : list) {
    			assertTrue(object instanceof Integer);
			}
    		assertEquals(list.get(0), 200 + (Integer)list.get(1));
    		assertEquals(entry.getKey(), String.format("TEST%1$s", list.get(1)));
    	}
    }
    	
    @Test
    public void testQVariant_String64MultiHash() {
    	Object variant = Variants.getString64MultiHash();
    	assertNotNull(variant);
    	assertTrue(variant.getClass().getName(), variant instanceof QMultiHash);
    	for(Map.Entry<?, ?> entry : ((Map<?,?>)variant).entrySet()) {
    		assertTrue(entry.getKey() instanceof String);
    		assertTrue(entry.getValue() instanceof List);
    		List<?> list = (List<?>)entry.getValue();
    		assertEquals(2, list.size());
    		for (Object object : list) {
    			assertTrue(object instanceof Long);
			}
    		assertEquals(list.get(0), 200 + (Long)list.get(1));
    		assertEquals(entry.getKey(), String.format("TEST%1$s", list.get(1)));
    	}
    }
    	
    @Test
    public void testQVariant_String16MultiHash() {
    	Object variant = Variants.getString16MultiHash();
    	assertNotNull(variant);
    	assertTrue(variant.getClass().getName(), variant instanceof QMultiHash);
    	for(Map.Entry<?, ?> entry : ((Map<?,?>)variant).entrySet()) {
    		assertTrue(entry.getKey() instanceof String);
    		assertTrue(entry.getValue() instanceof List);
    		List<?> list = (List<?>)entry.getValue();
    		assertEquals(2, list.size());
    		for (Object object : list) {
    			assertTrue(object instanceof Short);
			}
    		assertEquals(list.get(0), (short)(200 + (Short)list.get(1)));
    		assertEquals(entry.getKey(), String.format("TEST%1$s", list.get(1)));
    	}
    }
    	
    @Test
    public void testQVariant_String8MultiHash() {
    	Object variant = Variants.getString8MultiHash();
    	assertNotNull(variant);
    	assertTrue(variant.getClass().getName(), variant instanceof QMultiHash);
    	for(Map.Entry<?, ?> entry : ((Map<?,?>)variant).entrySet()) {
    		assertTrue(entry.getKey() instanceof String);
    		assertTrue(entry.getValue() instanceof List);
    		List<?> list = (List<?>)entry.getValue();
    		assertEquals(2, list.size());
    		for (Object object : list) {
    			assertTrue(object instanceof Byte);
			}
    		assertEquals(list.get(0), (byte)(20 + (Byte)list.get(1)));
    		assertEquals(entry.getKey(), String.format("TEST%1$s", list.get(1)));
    	}
    }
    	
	@Test
    public void testQVariant_TestObjectMultiHash() {
    	Object variant = Variants.getTestObjectMultiHash();
    	assertNotNull(variant);
    	assertTrue(variant.getClass().getName(), variant instanceof QMultiHash);
    	for(Map.Entry<?, ?> entry : ((Map<?,?>)variant).entrySet()) {
    		assertTrue(entry.getKey() instanceof QObject);
    		assertTrue(entry.getValue() instanceof List);
    		List<?> list = (List<?>)entry.getValue();
    		assertEquals(2, list.size());
    		for (Object object : list) {
    			assertTrue(object instanceof Integer);
			}
    		assertEquals(list.get(0), 200 + (Integer)list.get(1));
    		assertEquals(String.format("VariantTestObject%1$s", list.get(1)), ((QObject)entry.getKey()).objectName());
    	}
    }
    	
    @Test
    public void testQVariant_UnknownEnumMultiHash() {
    	Assume.assumeTrue("Is expected and accepted to fail in Qt5.", QLibraryInfo.version().majorVersion()>=6);
    	Object variant = Variants.getUnknownEnumMultiHash();
    	assertNotNull(variant);
    	assertTrue(variant.getClass().getName(), variant instanceof QMultiHash);
    	for(Map.Entry<?, ?> entry : ((Map<?,?>)variant).entrySet()) {
    		assertTrue(entry.getKey() instanceof QtEnumerator);
			assertTrue(Integer.valueOf(101).equals(((QtEnumerator)entry.getKey()).value()) 
					|| Integer.valueOf(102).equals(((QtEnumerator)entry.getKey()).value()));
    		assertTrue(entry.getValue() instanceof List);
    		List<?> list = (List<?>)entry.getValue();
    		assertEquals(2, list.size());
    		for (Object object : list) {
    			assertTrue(object instanceof Integer);
    			assertTrue(Integer.valueOf(1).equals(object) 
    					|| Integer.valueOf(2).equals(object) 
    					|| Integer.valueOf(501).equals(object) 
    					|| Integer.valueOf(502).equals(object));
			}
    	}
    }
    	
    @Test
    public void testQVariant_ColorMultiHash() {
    	Object variant = Variants.getColorMultiHash();
    	assertNotNull(variant);
    	assertTrue(variant.getClass().getName(), variant instanceof QMultiHash);
    	for(Map.Entry<?, ?> entry : ((Map<?,?>)variant).entrySet()) {
    		assertTrue(entry.getKey() instanceof QColor);
			assertTrue(new QColor(Qt.GlobalColor.red).equals(entry.getKey()) 
					|| new QColor(Qt.GlobalColor.green).equals(entry.getKey()));
    		assertTrue(entry.getValue() instanceof List);
    		List<?> list = (List<?>)entry.getValue();
    		assertEquals(2, list.size());
    		for (Object object : list) {
    			assertTrue(object instanceof String);
    			assertTrue("A".equals(object) 
    					|| "B".equals(object) 
    					|| "C".equals(object) 
    					|| "D".equals(object));
			}
    	}
    }
    	
    @Test
    public void testQVariant_ColorMultiMap() {
    	Object variant = Variants.getColorMultiMap();
    	assertNotNull(variant);
    	assertTrue(variant.getClass().getName(), variant instanceof QMultiMap);
    	for(Map.Entry<?, ?> entry : ((Map<?,?>)variant).entrySet()) {
    		assertTrue(entry.getKey() instanceof QColor);
			assertTrue(new QColor(Qt.GlobalColor.red).equals(entry.getKey()) 
					|| new QColor(Qt.GlobalColor.green).equals(entry.getKey()));
    		assertTrue(entry.getValue() instanceof List);
    		List<?> list = (List<?>)entry.getValue();
    		//assertEquals(2, list.size());
    		for (Object object : list) {
    			assertTrue(object instanceof Short);
    			assertTrue(Short.valueOf((short)1).equals(object) 
    					|| Short.valueOf((short)2).equals(object) 
    					|| Short.valueOf((short)501).equals(object) 
    					|| Short.valueOf((short)502).equals(object));
			}
    	}
    }
    	
    @Test
    public void testQVariant_Int64MultiHash() {
    	Object variant = Variants.getInt64MultiHash();
    	assertNotNull(variant);
    	assertTrue(variant.getClass().getName(), variant instanceof QMultiHash);
    	for(Map.Entry<?, ?> entry : ((Map<?,?>)variant).entrySet()) {
    		assertTrue(entry.getKey() instanceof Long);
			assertTrue(Long.valueOf(3).equals(entry.getKey()) 
					|| Long.valueOf(7).equals(entry.getKey()));
    		assertTrue(entry.getValue() instanceof List);
    		List<?> list = (List<?>)entry.getValue();
    		assertEquals(2, list.size());
    		for (Object object : list) {
    			assertTrue(object instanceof Long);
    			assertTrue(Long.valueOf(1).equals(object) 
    					|| Long.valueOf(2).equals(object) 
    					|| Long.valueOf(501).equals(object) 
    					|| Long.valueOf(502).equals(object));
			}
    	}
    }
    	
    @Test
    public void testQVariant_Int16MultiHash() {
    	Object variant = Variants.getInt16MultiHash();
    	assertNotNull(variant);
    	assertTrue(variant.getClass().getName(), variant instanceof QMultiHash);
    	for(Map.Entry<?, ?> entry : ((Map<?,?>)variant).entrySet()) {
    		assertTrue(entry.getKey() instanceof Short);
			assertTrue(Short.valueOf((short)3).equals(entry.getKey()) 
					|| Short.valueOf((short)7).equals(entry.getKey()));
    		assertTrue(entry.getValue() instanceof List);
    		List<?> list = (List<?>)entry.getValue();
    		assertEquals(2, list.size());
    		for (Object object : list) {
    			assertTrue(object instanceof Long);
    			assertTrue(Long.valueOf(1).equals(object) 
    					|| Long.valueOf(2).equals(object) 
    					|| Long.valueOf(501).equals(object) 
    					|| Long.valueOf(502).equals(object));
			}
    	}
    }
    	
    @Test
    public void testQVariant_CharMultiHash() {
    	Object variant = Variants.getCharMultiHash();
    	assertNotNull(variant);
    	assertTrue(variant.getClass().getName(), variant instanceof QMultiHash);
    	for(Map.Entry<?, ?> entry : ((Map<?,?>)variant).entrySet()) {
    		assertTrue(entry.getKey() instanceof Byte);
			assertTrue(Byte.valueOf((byte)'a').equals(entry.getKey()) 
					|| Byte.valueOf((byte)'b').equals(entry.getKey()));
    		assertTrue(entry.getValue() instanceof List);
    		List<?> list = (List<?>)entry.getValue();
    		assertEquals(2, list.size());
    		for (Object object : list) {
    			assertTrue(object instanceof Short);
    			assertTrue(Short.valueOf((short)1).equals(object) 
    					|| Short.valueOf((short)2).equals(object) 
    					|| Short.valueOf((short)501).equals(object) 
    					|| Short.valueOf((short)502).equals(object));
			}
    	}
    }
    	
    @Test
    public void testQVariant_UnknownEnumMultiMap() {
    	Object variant = Variants.getUnknownEnumMultiMap();
    	assertNotNull(variant);
    	assertTrue(variant.getClass().getName(), variant instanceof QMultiMap);
    	for(Map.Entry<?, ?> entry : ((Map<?,?>)variant).entrySet()) {
    		assertTrue(entry.getKey() instanceof QtEnumerator);
			assertTrue(Integer.valueOf(101).equals(((QtEnumerator)entry.getKey()).value()) 
					|| Integer.valueOf(102).equals(((QtEnumerator)entry.getKey()).value()));
    		assertTrue(entry.getValue() instanceof List);
    		List<?> list = (List<?>)entry.getValue();
    		assertEquals(2, list.size());
    		for (Object object : list) {
    			assertTrue(object instanceof Integer);
    			assertTrue(Integer.valueOf(1).equals(object) 
    					|| Integer.valueOf(2).equals(object) 
    					|| Integer.valueOf(501).equals(object) 
    					|| Integer.valueOf(502).equals(object));
			}
    	}
    }
    	
    @Test
    public void testQVariant_RectFMultiHash() {
    	Object variant = Variants.getRectFMultiHash();
    	assertNotNull(variant);
    	assertTrue(variant.getClass().getName(), variant instanceof QMultiHash);
    }
    	
    @Test
    public void testQVariant_CharMultiMap() {
    	Object variant = Variants.getCharMultiMap();
    	assertNotNull(variant);
    	assertTrue(variant.getClass().getName(), variant instanceof QMultiMap);
    	for(Map.Entry<?, ?> entry : ((Map<?,?>)variant).entrySet()) {
    		assertTrue(entry.getKey() instanceof Byte);
			assertTrue(Byte.valueOf((byte)'a').equals(entry.getKey()) 
					|| Byte.valueOf((byte)'b').equals(entry.getKey()));
    		assertTrue(entry.getValue() instanceof List);
    		List<?> list = (List<?>)entry.getValue();
    		assertEquals(2, list.size());
    		for (Object object : list) {
    			assertTrue(object instanceof Short);
    			assertTrue(Short.valueOf((short)1).equals(object) 
    					|| Short.valueOf((short)2).equals(object) 
    					|| Short.valueOf((short)501).equals(object) 
    					|| Short.valueOf((short)502).equals(object));
			}
    	}
	}
    
    @Test
    public void run_QVariantString() {
        Object variant = "this is my string";
        String expectedString = "this is my string";
        double expectedDouble = 0.0;
        QByteArray expectedByteArray = new QByteArray("this is my string");
        int expectedInt = 0;
        boolean expectedBool = true;
        QBitArray expectedBitArray = new QBitArray();
        char expectedChar = 0;
        QDate expectedDate = new QDate();
        QTime expectedTime = new QTime();
        QDateTime expectedDateTime = new QDateTime(expectedDate, expectedTime);
        QPoint expectedPoint = new QPoint();
        QPointF expectedPointF = new QPointF();
        QRect expectedRect = new QRect();
        QRectF expectedRectF = new QRectF();
        QSize expectedSize = new QSize();
        QSizeF expectedSizeF = new QSizeF();
        List<Object> expectedList = QList.createVariantList();
        Map<String, Object> expectedMap = QMap.createVariantMap();
        if(convertToQList)
        	expectedList = QList.of('t', 'h', 'i', 's', ' ', 'i', 's', ' ', 'm', 'y', ' ', 's', 't', 'r', 'i', 'n', 'g');

        testQVariant("Normal string", variant, expectedDouble, true, expectedString, true, expectedByteArray, true, expectedInt, true, expectedBool, true, expectedBitArray, false, expectedChar,
                false, expectedDate, true, expectedTime, true, expectedDateTime, true, expectedPoint, false, expectedPointF, false, expectedRect, false, expectedRectF, false,
                expectedSize, false, expectedSizeF, false, expectedList, convertToQList, expectedMap, false);
    }

    @Test
    public void run_QVariantByteArray() {
        Object variant = new QByteArray("this is my string");
        String expectedString = "this is my string";
        double expectedDouble = 0.0;
        QByteArray expectedByteArray = new QByteArray("this is my string");
        int expectedInt = 0;
        boolean expectedBool = true;
        QBitArray expectedBitArray = new QBitArray();
        char expectedChar = 0;
        QDate expectedDate = new QDate();
        QTime expectedTime = new QTime();
        QDateTime expectedDateTime = new QDateTime();
        QPoint expectedPoint = new QPoint();
        QPointF expectedPointF = new QPointF();
        QRect expectedRect = new QRect();
        QRectF expectedRectF = new QRectF();
        QSize expectedSize = new QSize();
        QSizeF expectedSizeF = new QSizeF();
        List<Object> expectedList = QList.createVariantList();
        Map<String, Object> expectedMap = QMap.createVariantMap();
        if(convertToQList)
        	expectedList = QList.of((byte)'t', (byte)'h', (byte)'i', (byte)'s', (byte)' ', (byte)'i', (byte)'s', (byte)' ', (byte)'m', (byte)'y', (byte)' ', (byte)'s', (byte)'t', (byte)'r', (byte)'i', (byte)'n', (byte)'g');

        testQVariant("QByteArray", variant, expectedDouble, true, expectedString, true, expectedByteArray, true, expectedInt, true, expectedBool, true, expectedBitArray,
                false, expectedChar, false, expectedDate, false, expectedTime, false, expectedDateTime, false, expectedPoint, false, expectedPointF, false, expectedRect, false, expectedRectF, false,
                expectedSize, false, expectedSizeF, false, expectedList, convertToQList, expectedMap, false);
    }

    @Test
    public void run_QVariantObject() {
        Object variant = new QObject();
        String expectedString = variant.toString();
        double expectedDouble = 0.0;
        QByteArray expectedByteArray = new QByteArray("");
        int expectedInt = 0;
        boolean expectedBool = false;
        QBitArray expectedBitArray = new QBitArray();
        char expectedChar = 0;
        QDate expectedDate = new QDate();
        QTime expectedTime = new QTime();
        QDateTime expectedDateTime = new QDateTime(expectedDate, expectedTime);
        QPoint expectedPoint = new QPoint();
        QPointF expectedPointF = new QPointF();
        QRect expectedRect = new QRect();
        QRectF expectedRectF = new QRectF();
        QSize expectedSize = new QSize();
        QSizeF expectedSizeF = new QSizeF();
        List<Object> expectedList = QList.createVariantList();
        Map<String, Object> expectedMap = QMap.createVariantMap();

        testQVariant("java.lang.Object", variant, expectedDouble, false, expectedString, true, expectedByteArray, false, expectedInt, false, expectedBool, false,
                expectedBitArray, false, expectedChar, false, expectedDate, false, expectedTime, false, expectedDateTime, false, expectedPoint, false, expectedPointF, false, expectedRect, false,
                expectedRectF, false, expectedSize, false, expectedSizeF, false, expectedList, false, expectedMap, false);
    }

    @Test
    public void run_QVariantStringDouble() {
        Object variant = "123.456";
        String expectedString = "123.456";
        double expectedDouble = 123.456;
        QByteArray expectedByteArray = new QByteArray("123.456");
        int expectedInt = 0;
        boolean expectedBool = true;
        QBitArray expectedBitArray = new QBitArray();
        char expectedChar = 0;
        QDate expectedDate = new QDate();
        QTime expectedTime = new QTime();
        QDateTime expectedDateTime = new QDateTime();
        QPoint expectedPoint = new QPoint();
        QPointF expectedPointF = new QPointF();
        QRect expectedRect = new QRect();
        QRectF expectedRectF = new QRectF();
        QSize expectedSize = new QSize();
        QSizeF expectedSizeF = new QSizeF();
        List<Object> expectedList = QList.createVariantList();
        Map<String, Object> expectedMap = QMap.createVariantMap();
        if(convertToQList)
        	expectedList = QList.of('1', '2', '3', '.', '4', '5', '6');

        testQVariant("String double", variant, expectedDouble, true, expectedString, true, expectedByteArray, true, expectedInt, true, expectedBool, true, expectedBitArray,
                false, expectedChar, false, expectedDate, true, expectedTime, true, expectedDateTime, true, expectedPoint, false, expectedPointF, false, expectedRect, false, expectedRectF, false,
                expectedSize, false, expectedSizeF, false, expectedList, convertToQList, expectedMap, false);
    }

    @Test
    public void run_QVariantByteArrayDouble() {
        Object variant = new QByteArray("456.789");;
        String expectedString = "456.789";
        double expectedDouble = 456.789;
        QByteArray expectedByteArray = new QByteArray("456.789");
        int expectedInt = 0;
        boolean expectedBool = true;
        QBitArray expectedBitArray = new QBitArray();
        char expectedChar = 0;
        QDate expectedDate = new QDate();
        QTime expectedTime = new QTime();
        QDateTime expectedDateTime = new QDateTime(expectedDate, expectedTime);
        QPoint expectedPoint = new QPoint();
        QPointF expectedPointF = new QPointF();
        QRect expectedRect = new QRect();
        QRectF expectedRectF = new QRectF();
        QSize expectedSize = new QSize();
        QSizeF expectedSizeF = new QSizeF();
        List<Object> expectedList = QList.createVariantList();
        Map<String, Object> expectedMap = QMap.createVariantMap();
        if(convertToQList)
        	expectedList = QList.of((byte)'4', (byte)'5', (byte)'6', (byte)'.', (byte)'7', (byte)'8', (byte)'9');

        testQVariant("Byte array double", variant, expectedDouble, true, expectedString, true, expectedByteArray, true, expectedInt, true, expectedBool, true,
                expectedBitArray, false, expectedChar, false, expectedDate, false, expectedTime, false, expectedDateTime, false, expectedPoint, false, expectedPointF, false, expectedRect, false,
                expectedRectF, false, expectedSize, false, expectedSizeF, false, expectedList, convertToQList, expectedMap, false);
    }

    @Test
    public void run_QVariantStringInteger() {
        Object variant = "321";
        String expectedString = "321";
        double expectedDouble = 321.0;
        QByteArray expectedByteArray = new QByteArray("321");
        int expectedInt = 321;
        boolean expectedBool = true;
        QBitArray expectedBitArray = new QBitArray();
        char expectedChar = 0;
        QDate expectedDate = new QDate();
        QTime expectedTime = new QTime();
        QDateTime expectedDateTime = new QDateTime(expectedDate, expectedTime);
        QPoint expectedPoint = new QPoint();
        QPointF expectedPointF = new QPointF();
        QRect expectedRect = new QRect();
        QRectF expectedRectF = new QRectF();
        QSize expectedSize = new QSize();
        QSizeF expectedSizeF = new QSizeF();
        List<Object> expectedList = QList.createVariantList();
        Map<String, Object> expectedMap = QMap.createVariantMap();
        if(convertToQList)
        	expectedList = QList.of('3', '2', '1');

        testQVariant("String integer", variant, expectedDouble, true, expectedString, true, expectedByteArray, true, expectedInt, true, expectedBool, true, expectedBitArray,
                false, expectedChar, false, expectedDate, true, expectedTime, true, expectedDateTime, true, expectedPoint, false, expectedPointF, false, expectedRect, false, expectedRectF, false,
                expectedSize, false, expectedSizeF, false, expectedList, convertToQList, expectedMap, false);
    }

    @Test
    public void run_QVariantStringBooleanFalse() {
        Object variant = "FaLsE";
        String expectedString = "FaLsE";
        double expectedDouble = 0.0;
        QByteArray expectedByteArray = new QByteArray("FaLsE");
        int expectedInt = 0;
        boolean expectedBool = false;
        QBitArray expectedBitArray = new QBitArray();
        char expectedChar = 0;
        QDate expectedDate = new QDate();
        QTime expectedTime = new QTime();
        QDateTime expectedDateTime = new QDateTime();
        QPoint expectedPoint = new QPoint();
        QPointF expectedPointF = new QPointF();
        QRect expectedRect = new QRect();
        QRectF expectedRectF = new QRectF();
        QSize expectedSize = new QSize();
        QSizeF expectedSizeF = new QSizeF();
        List<Object> expectedList = QList.createVariantList();
        Map<String, Object> expectedMap = QMap.createVariantMap();
        if(convertToQList)
        	expectedList = QList.of('F', 'a', 'L', 's', 'E');

        testQVariant("String boolean", variant, expectedDouble, true, expectedString, true, expectedByteArray, true, expectedInt, true, expectedBool, true, expectedBitArray,
                false, expectedChar, false, expectedDate, true, expectedTime, true, expectedDateTime, true, expectedPoint, false, expectedPointF, false, expectedRect, false, expectedRectF, false,
                expectedSize, false, expectedSizeF, false, expectedList, convertToQList, expectedMap, false);
    }

    @Test
    public void run_QVariantBooleanTrue() {
        Object variant = "tRUe";;
        String expectedString = "tRUe";;
        double expectedDouble = 0.0;
        QByteArray expectedByteArray = new QByteArray("tRUe");
        int expectedInt = 0;
        boolean expectedBool = true;
        QBitArray expectedBitArray = new QBitArray();
        char expectedChar = 0;
        QDate expectedDate = new QDate();
        QTime expectedTime = new QTime();
        QDateTime expectedDateTime = new QDateTime();
        QPoint expectedPoint = new QPoint();
        QPointF expectedPointF = new QPointF();
        QRect expectedRect = new QRect();
        QRectF expectedRectF = new QRectF();
        QSize expectedSize = new QSize();
        QSizeF expectedSizeF = new QSizeF();
        List<Object> expectedList = QList.createVariantList();
        Map<String, Object> expectedMap = QMap.createVariantMap();
        if(convertToQList)
        	expectedList = QList.of('t', 'R', 'U', 'e');

        testQVariant("String boolean (true)", variant, expectedDouble, true, expectedString, true, expectedByteArray, true, expectedInt, true, expectedBool, true,
                expectedBitArray, false, expectedChar, false, expectedDate, true, expectedTime, true, expectedDateTime, true, expectedPoint, false, expectedPointF, false, expectedRect, false,
                expectedRectF, false, expectedSize, false, expectedSizeF, false, expectedList, convertToQList, expectedMap, false);
    }

    @Test
    public void run_QVariantDouble() {
        Object variant = 123.567;
        String expectedString = "123.567";
        double expectedDouble = 123.567;
        QByteArray expectedByteArray = new QByteArray("123.567");
        int expectedInt = 123;
        boolean expectedBool = true;
        QBitArray expectedBitArray = new QBitArray();
        char expectedChar = 0;
        QDate expectedDate = new QDate();
        QTime expectedTime = new QTime();
        QDateTime expectedDateTime = new QDateTime(expectedDate, expectedTime);
        QPoint expectedPoint = new QPoint();
        QPointF expectedPointF = new QPointF();
        QRect expectedRect = new QRect();
        QRectF expectedRectF = new QRectF();
        QSize expectedSize = new QSize();
        QSizeF expectedSizeF = new QSizeF();
        List<Object> expectedList = QList.createVariantList();
        Map<String, Object> expectedMap = QMap.createVariantMap();

        testQVariant("Double", variant, expectedDouble, true, expectedString, true, expectedByteArray, true, expectedInt, true, expectedBool, true, expectedBitArray, false,
                expectedChar, false, expectedDate, false, expectedTime, false, expectedDateTime, false, expectedPoint, false, expectedPointF, false, expectedRect, false, expectedRectF, false,
                expectedSize, false, expectedSizeF, false, expectedList, false, expectedMap, false);
    }

    @Test
    public void run_QVariantNull() {
        Object variant = null;
        String expectedString = "";
        double expectedDouble = 0.0;
        QByteArray expectedByteArray = new QByteArray("");
        int expectedInt = 0;
        boolean expectedBool = false;
        QBitArray expectedBitArray = new QBitArray();
        char expectedChar = 0;
        QDate expectedDate = new QDate();
        QTime expectedTime = new QTime();
        QDateTime expectedDateTime = new QDateTime();
        QPoint expectedPoint = new QPoint();
        QPointF expectedPointF = new QPointF();
        QRect expectedRect = new QRect();
        QRectF expectedRectF = new QRectF();
        QSize expectedSize = new QSize();
        QSizeF expectedSizeF = new QSizeF();
        List<Object> expectedList = QList.createVariantList();
        Map<String, Object> expectedMap = QMap.createVariantMap();

        testQVariant("Null", variant, expectedDouble, false, expectedString, true, expectedByteArray, true, expectedInt, false, expectedBool, false, expectedBitArray, false,
                expectedChar, false, expectedDate, false, expectedTime, false, expectedDateTime, false, expectedPoint, false, expectedPointF, false, expectedRect, false, expectedRectF, false,
                expectedSize, false, expectedSizeF, false, expectedList, false, expectedMap, false);

    }

    public void testQVariant(String name, Object object, double expectedDouble, boolean canConvertDouble, String expectedString, boolean canConvertString, QByteArray expectedByteArray,
            boolean canConvertByteArray, int expectedInt, boolean canConvertInt, boolean expectedBool, boolean canConvertBool, QBitArray expectedBitArray, boolean canConvertBitArray,
            char expectedChar, boolean canConvertChar, QDate expectedDate, boolean canConvertDate, QTime expectedTime, boolean canConvertTime, QDateTime expectedDateTime,
            boolean canConvertDateTime, QPoint expectedPoint, boolean canConvertPoint, QPointF expectedPointF, boolean canConvertPointF, QRect expectedRect, boolean canConvertRect,
            QRectF expectedRectF, boolean canConvertRectF, QSize expectedSize, boolean canConvertSize, QSizeF expectedSizeF, boolean canConvertSizeF,
            List<?> expectedList, boolean canConvertList, Map<?, ?> expectedMap, boolean canConvertMap) {

        assertEquals("Can convert String", canConvertString, QVariant.canConvertToString(object));
        assertEquals("Can convert ByteArray", canConvertByteArray, QVariant.canConvertToByteArray(object));
        assertEquals("Can convert BitArray", canConvertBitArray, QVariant.canConvertToBitArray(object));
        assertEquals("Can convert Char", canConvertChar, QVariant.canConvertToChar(object));
        assertEquals("Can convert Date", canConvertDate, QVariant.canConvertToDate(object));
        assertEquals("Can convert Time", canConvertTime, QVariant.canConvertToTime(object));
        assertEquals("Can convert DateTime", canConvertDateTime, QVariant.canConvertToDateTime(object));
        assertEquals("Can convert Point", canConvertPoint, QVariant.canConvertToPoint(object));
        assertEquals("Can convert PointF", canConvertPointF, QVariant.canConvertToPointF(object));
        assertEquals("Can convert Rect", canConvertRect, QVariant.canConvertToRect(object));
        assertEquals("Can convert RectF", canConvertRectF, QVariant.canConvertToRectF(object));
        assertEquals("Can convert Size", canConvertSize, QVariant.canConvertToSize(object));
        assertEquals("Can convert SizeF", canConvertSizeF, QVariant.canConvertToSizeF(object));
        assertEquals("Can convert List", canConvertList, QVariant.canConvertToList(object));
        assertEquals("Can convert Map", canConvertMap, QVariant.canConvertToMap(object));
        assertEquals("Can convert Double", canConvertDouble, QVariant.canConvertToDouble(object));
        assertEquals("Can convert Int", canConvertInt, QVariant.canConvertToInt(object));
        assertEquals("Can convert Bool", canConvertBool, QVariant.canConvertToBoolean(object));

        assertEquals(expectedString, QVariant.toString(object));
        assertEquals(expectedDouble, QVariant.toDouble(object), 0.0);
        assertEquals(expectedInt, QVariant.toInt(object));
        assertEquals(expectedBool, QVariant.toBoolean(object));
        assertEquals(expectedBitArray, QVariant.toBitArray(object));
        assertEquals(expectedChar, QVariant.toChar(object));
        assertEquals(expectedDate, QVariant.toDate(object));
        assertEquals(expectedTime, QVariant.toTime(object));
        assertEquals(expectedDateTime, QVariant.toDateTime(object));
        assertEquals(expectedPoint.x(), QVariant.toPoint(object).x());
        assertEquals(expectedPoint.y(), QVariant.toPoint(object).y());
        assertEquals(expectedPointF.x(), QVariant.toPointF(object).x(), 0.0);
        assertEquals(expectedPointF.y(), QVariant.toPointF(object).y(), 0.0);
        assertEquals(expectedRect.left(), QVariant.toRect(object).left());
        assertEquals(expectedRect.top(), QVariant.toRect(object).top());
        assertEquals(expectedRect.right(), QVariant.toRect(object).right());
        assertEquals(expectedRect.bottom(), QVariant.toRect(object).bottom());
        assertEquals(expectedRectF.left(), QVariant.toRectF(object).left(), 0.0);
        assertEquals(expectedRectF.top(), QVariant.toRectF(object).top(), 0.0);
        assertEquals(expectedRectF.bottom(), QVariant.toRectF(object).bottom(), 0.0);
        assertEquals(expectedRectF.right(), QVariant.toRectF(object).right(), 0.0);
        assertEquals(expectedSize.width(), QVariant.toSize(object).width());
        assertEquals(expectedSize.height(), QVariant.toSize(object).height());
        assertEquals(expectedSizeF.width(), QVariant.toSizeF(object).width(), 0.0);
        assertEquals(expectedSizeF.height(), QVariant.toSizeF(object).height(), 0.0);
        assertEquals(expectedList, QVariant.toList(object));
        assertEquals(expectedMap, QVariant.toMap(object));
        {
            QByteArray ba = QVariant.toByteArray(object);
            assertEquals(expectedByteArray.size(), ba.size());
            for (int i = 0; i < ba.size(); ++i) {
                assertEquals(expectedByteArray.at(i), ba.at(i));
            }
        }
    }

    @Test
    public void run_QByteArray_toString() {
        byte data[] = {'a', 'e', 'r', 'y', 'n'};

        // Normal construction
        assertEquals(new QByteArray(data).toString(), "aeryn");

        // String construction
        assertEquals(new QByteArray("aeryn").toString(), "aeryn");

        // Using int, char constructor
        assertEquals(new QByteArray(5, (byte) 'a').toString(), "aaaaa");
    }

    private class DataPrimitive {
        public Object value;
        public String methodName;

        public DataPrimitive(Object value, String methodName) {
            this.value = value;
            this.methodName = methodName;
        }
    }

    @Test
    public void run_primitives() {
        DataPrimitive[] data = { new DataPrimitive(Long.valueOf(3), "currentQInt64"),
                new DataPrimitive(Long.valueOf(4), "currentQUInt64"), 
                new DataPrimitive(Integer.valueOf(5), "currentQInt32"),
                new DataPrimitive(Integer.valueOf(6), "currentQUInt32"),
                new DataPrimitive(Short.valueOf((short) 7), "currentQInt16"),
                new DataPrimitive(Short.valueOf((short) 8), "currentQUInt16"),
                new DataPrimitive(Byte.valueOf((byte) 9), "currentQInt8"),
                new DataPrimitive(Byte.valueOf((byte) 10), "currentQUInt8"),
                new DataPrimitive(Float.valueOf(11), "currentFloat"),
                new DataPrimitive(Double.valueOf(12), "currentDouble"),
                new DataPrimitive(Character.valueOf('\u2211'), "currentQChar") };

        for (int i = 0; i < data.length; i++) {
            Object value = data[i].value;
            String methodName = data[i].methodName;
            Variants v = new Variants();
            Object returned = v.pushThrough(value);
            assertTrue(v.isValid());
            Object nativlySet = null;

            try {
                Method method = Variants.class.getMethod(methodName);
                nativlySet = method.invoke(v);
            } catch (Exception e) {
                System.err.println("Failed to call: " + methodName);
                e.printStackTrace();
            }
            assertEquals(methodName, value, nativlySet);
            assertEquals(methodName, value, returned);
        }
    }

    @Test
    public void run_primitives2() {
        Variants v = new Variants();

        int value = 1;

        long the_long = ++value;
        v.pushThrough(the_long);
        assertEquals(v.currentQInt64(), the_long);

        long the_ulong = ++value;
        v.pushThrough(the_ulong);
        assertEquals(v.currentQUInt64(), the_ulong);

        int the_int = ++value;
        v.pushThrough(the_int);
        assertEquals(v.currentQInt32(), the_int);

        int the_uint = ++value;
        v.pushThrough(the_uint);
        assertEquals(v.currentQUInt32(), the_uint);

        char the_short = (char) ++value;
        v.pushThrough(the_short);

        assertEquals(v.currentQInt16(), the_short);

        char the_ushort = (char) ++value;
        v.pushThrough(the_ushort);
        assertEquals(v.currentQUInt16(), the_ushort);

        byte the_byte = (byte) ++value;
        v.pushThrough(the_byte);
        assertEquals(v.currentQInt8(), the_byte);

        byte the_ubyte = (byte) ++value;
        v.pushThrough(the_ubyte);
        assertEquals(v.currentQUInt8(), the_ubyte);
    }

    @Test
    public void run_JObjectWrapper_test() {
        Variants v = new Variants();

        v.pushThrough(new Object() {
            @Override
            public String toString() {
                return "abcd";
            }
        });
        assertEquals("abcd", v.currentToString());
    }
	
	@Test
    public void run_boolean_test() {
        Variants v = new Variants();
		v.pushThrough(true);
        assertTrue(v.currentToBoolean());
		v.pushThrough(false);
        assertFalse(v.currentToBoolean());
		v.pushThrough(1);
        assertTrue(v.currentToBoolean());
		v.pushThrough(0);
        assertFalse(v.currentToBoolean());
		v.pushThrough("true");
        assertTrue(v.currentToBoolean());
		v.pushThrough("false");
        assertFalse(v.currentToBoolean());
		v.pushThrough(new Object());
        assertFalse(v.currentToBoolean());
    }
	
	@Test
    public void run_double_test() {
        Variants v = new Variants();
		v.pushThrough(1.1);
        assertEquals(1.1, v.currentToDouble(), 0.000001);
		v.pushThrough(1);
        assertEquals(1.0, v.currentToDouble(), 0.000001);
		v.pushThrough(4l);
        assertEquals(4.0, v.currentToDouble(), 0.000001);
		v.pushThrough("9");
        assertEquals(9, v.currentToDouble(), 0.000001);
		v.pushThrough("54.7");
        assertEquals(54.7, v.currentToDouble(), 0.000001);
		v.pushThrough(new Object());
		assertEquals(0.0, v.currentToDouble(), 0.000001);
    }
	
	@Test
    public void run_int_test() {
        Variants v = new Variants();
		v.pushThrough(1);
        assertEquals(1, v.currentToInt());
		v.pushThrough(4l);
        assertEquals(4, v.currentToInt());
		v.pushThrough("9");
        assertEquals(9, v.currentToInt());
		v.pushThrough(54.3);
        assertEquals(54, v.currentToInt());
		v.pushThrough(54.7);
        assertEquals(55, v.currentToInt());
		v.pushThrough(new Object());
		assertEquals(0, v.currentToInt());
    }

    @Test
    public void run_QEnumerator_test() {
        Variants v = new Variants();
        Object pushedObject = v.pushThrough(Qt.ArrowType.UpArrow);
        assertEquals(Qt.ArrowType.UpArrow, pushedObject);
        assertEquals(Qt.ArrowType.UpArrow.value(), v.currentToInt());
        assertEquals(Qt.ArrowType.UpArrow.toString(), v.currentToString());
    }

    @Test
    public void run_QFlags_test() {
		Variants v = new Variants();
		Qt.GestureFlags flags = new Qt.GestureFlags(Qt.GestureFlag.IgnoredGesturesPropagateToParent, Qt.GestureFlag.ReceivePartialGestures);
        Object pushedObject = v.pushThrough(flags);
        assertEquals(flags, pushedObject);
        assertEquals(flags.value(), v.currentToInt());
//        assertEquals(flags.toString(), v.currentToString());
    }
	
	@Test
    public void test_QVariant_DockWidgetArea(){
		Object value = Variants.getDockWidgetArea();
		assertNotNull(value);
		assertEquals(Qt.DockWidgetArea.class, value.getClass());
		assertEquals(Qt.DockWidgetArea.BottomDockWidgetArea, value);
	}
	
	@Test
    public void test_QVariant_DockWidgetAreas(){
		Object value = Variants.getDockWidgetAreas();
		assertNotNull(value);
		assertEquals(Qt.DockWidgetAreas.class, value.getClass());
		assertEquals(new Qt.DockWidgetAreas(Qt.DockWidgetArea.LeftDockWidgetArea, Qt.DockWidgetArea.BottomDockWidgetArea), value);
	}
	
	@Test
    public void test_QVariant_PageSize(){
		Object value = Variants.getPageSize();
		assertNotNull(value);
		assertEquals(QPageSize.class, value.getClass());
		assertEquals(new QSize(20,20), ((QPageSize)value).sizePoints());
	}
	
	@Test
    public void test_QVariant_SharedPointerOfWidget(){
		Object value = Variants.getSharedPointerOfWidget();
		assertNotNull(value);
		assertEquals(QLabel.class, value.getClass());
		assertEquals("TEST_TEXT", ((QLabel)value).text());
	}
	
	@Test
    public void test_QVariant_SpinBox(){
		Object value = Variants.getSpinBox();
		assertNotNull(value);
		assertEquals(QSpinBox.class, value.getClass());
		((QSpinBox)value).dispose();
	}
	
	@Test
    public void test_QVariant_DirPair(){
		Object value = Variants.getDirPair();
		assertNotNull(value);
		assertEquals(QPair.class, value.getClass());
		QPair<?,?> pair = (QPair<?,?>)value;
		assertNotNull(pair.first);
		assertNotNull(pair.second);
		assertEquals(QDir.class, pair.first.getClass());
		assertEquals(String.class, pair.second.getClass());
		assertEquals(new QDir("/"), pair.first);
		assertEquals("ROOT", pair.second);
	}


    private boolean itemChangeCalled = false;
    @Test
    public void run_interfaces_passed_as_QVariant() {
    	Object[] result = {null};
        QGraphicsPixmapItem it = new QGraphicsPixmapItem() {

            @QtUninvokable
            public Object itemChange(QGraphicsItem.GraphicsItemChange change, Object value) {
            	if(change == QGraphicsItem.GraphicsItemChange.ItemChildAddedChange) {
            		result[0] = value;
            	}
                itemChangeCalled = change == QGraphicsItem.GraphicsItemChange.ItemChildRemovedChange
                                          && value instanceof QGraphicsPixmapItem;
                return super.itemChange(change, value);
            }
        };

        itemChangeCalled = false;
        // Note that ChildAddedChange will send an incomplete Object to itemChange. This is documented
        // to happen in C++ as well, since the change event is posted from the child's constructor.
        QGraphicsPixmapItem it2 = new QGraphicsPixmapItem(it);
        it2.setParentItem(null);
        assertTrue(itemChangeCalled);
        assertEquals("not able to find correct java object during native constructor call", it2, result[0]);
    }
    
    @Test
    public void testGraphicsItemChange() {
    	QGraphicsItem parent = new QGraphicsWidget();
    	QGraphicsObject child = new QGraphicsObject() {
			@Override
			public void paint(QPainter painter, QStyleOptionGraphicsItem option, QWidget widget) {
			}
			
			@Override
			public QRectF boundingRect() {
				return new QRectF();
			}

			@Override
			protected Object itemChange(GraphicsItemChange change, Object value) {
				return super.itemChange(change, value);
			}
		};
		child.setParentItem(parent);
		assertEquals(parent, child.parentItem());
		parent = new QGraphicsItem() {
			@Override
			public void paint(QPainter painter, QStyleOptionGraphicsItem option, QWidget widget) {
			}
			
			@Override
			public QRectF boundingRect() {
				return new QRectF();
			}
		};
		child.setParentItem(parent);
		assertEquals(parent, child.parentItem());
    }
    
    @Test 
    public void testRuntimeMetaTypes() {
    	QObject object = Variants.createInternalObject(null);
    	try {
    		Class<?> qvecClass = QList.class;
    		if(QLibraryInfo.version().majorVersion()<6) {
    			try {
					qvecClass = Class.forName(qvecClass.getName().replace("QList", "QVector"));
				} catch (ClassNotFoundException e) {
				}
    		}
	    	Map<String,Object> sentArguments = new TreeMap<>();
	    	sentArguments.put("test1", Arrays.asList(new QPair<>("file1", new QFileInfo(QDir.currentPath())), new QPair<>("file2", new QFileInfo(QDir.rootPath()))));
	    	sentArguments.put("test2", new QPair<>("file", Arrays.asList(new QFileInfo(QDir.rootPath()))));
	    	sentArguments.put("test3", Arrays.asList("STRG", 9, 4.5, new Object(), new QObject()));
	    	if(QLibraryInfo.version().majorVersion()<6) {
	    		sentArguments.put("test4", new LinkedList<>(Arrays.asList(new QColor(Qt.GlobalColor.red), new QColor(Qt.GlobalColor.green), new QColor(Qt.GlobalColor.blue))));
	    	}
	    	sentArguments.put("test5", new HashSet<>(Arrays.asList(new QPoint(1,2), new QPoint(3,4))));
	    	sentArguments.put("test6", new HashSet<>(Arrays.asList(object, QApplication.instance())));
	    	sentArguments.put("test7", new HashSet<>(Arrays.asList(new QComboBox(), new QComboBox(), new QComboBox(), new QComboBox())));
	    	sentArguments.put("test8", Arrays.asList(2,4,6,8));
	    	sentArguments.put("test9", Arrays.asList(Arrays.asList(2,4,6,8), Arrays.asList(3,5,7,9)));
	    	sentArguments.put("test10", mapOf("file1", new QFileInfo(QDir.rootPath())));
	    	sentArguments.put("test11", Arrays.asList(Arrays.asList(2,4,6,8), Arrays.asList(3,5,7,9)));
	    	sentArguments.put("test12", mapOf((byte)5, Arrays.asList(2,4,6,8), (byte)9, Arrays.asList(3,5,7,9)));
	    	sentArguments.put("test13", mapOf((byte)5, Arrays.asList(2,4,6,8), (byte)9, Arrays.asList(3,5,7,9)));
	    	sentArguments.put("test14", mapOf(new QUrl.FormattingOptions(QUrl.FormattingOption.DecodeReserved), Arrays.asList(Arrays.asList(2,4,6,8), Arrays.asList(3,5,7,9))));
	    	sentArguments.put("test15", new ArrayDeque<>(Arrays.asList(Arrays.asList(2,4,6,8), Arrays.asList(3,5,7,9))));
	    	sentArguments.put("test16", Arrays.asList(new QPair<>("object1", QApplication.instance()), new QPair<>("object2", object)));
	    	sentArguments.put("test17", Arrays.asList(new QGraphicsItemGroup(), new QGraphicsSimpleTextItem()));
	    	sentArguments.put("test18", mapOf(new QUrl.FormattingOptions(QUrl.FormattingOption.DecodeReserved), Arrays.asList(Arrays.asList(2,4,6,8), Arrays.asList(3,5,7,9))));
	
	        Map<String,Object> receivedArguments = new TreeMap<>();
	    	for(QMetaMethod m : object.metaObject().methods()) {
//	    		System.out.println(m.cppMethodSignature() + " = " + m.methodSignature());
	    		if(m.methodType()==QMetaMethod.MethodType.Signal && m.parameterTypes().size()==1) {
	    			QMetaObject.AbstractPrivateSignal1<?> signal = (QMetaObject.AbstractPrivateSignal1<?>)m.toSignal(object);
	    			String name = m.name().toString();
	    			signal.connect(p->receivedArguments.put(name, p));
	    		}
	    		if(m.name().startsWith("_") && sentArguments.containsKey(m.name().toString().substring(1))) {
	    			m.invoke(object, sentArguments.get(m.name().toString().substring(1)));
	    		}
	    	}
	    	if(sentArguments.containsKey("test1")) {
	    		assertEquals(sentArguments.get("test1"), receivedArguments.get("test1"));
	    	}
	    	
	    	if(sentArguments.containsKey("test2")) {
		    	assertTrue(receivedArguments.get("test2")!=null);
		    	assertEquals(QPair.class, receivedArguments.get("test2").getClass());
		    	assertEquals(((QPair<?,?>)sentArguments.get("test2")).first, ((QPair<?,?>)receivedArguments.get("test2")).first);
		    	assertEquals(QQueue.class, ((QPair<?,?>)receivedArguments.get("test2")).second.getClass());
		    	assertTrue(Arrays.equals(((Collection<?>)((QPair<?,?>)sentArguments.get("test2")).second).toArray(), ((Collection<?>)((QPair<?,?>)receivedArguments.get("test2")).second).toArray()));
	    	}
	    	
	    	if(sentArguments.containsKey("test3")) {
		    	assertTrue(receivedArguments.get("test3")!=null);
		    	assertEquals(QQueue.class, receivedArguments.get("test3").getClass());
		    	assertTrue(Arrays.equals(((Collection<?>)sentArguments.get("test3")).toArray(), ((Collection<?>)receivedArguments.get("test3")).toArray()));
	    	}
	    	if(sentArguments.containsKey("test4")) {
		    	assertTrue(Arrays.equals(((Collection<?>)sentArguments.get("test4")).toArray(), ((Collection<?>)receivedArguments.get("test4")).toArray()));
	    	}
	    	assertEquals(sentArguments.get("test5"), receivedArguments.get("test5"));
	    	assertEquals(sentArguments.get("test6"), receivedArguments.get("test6"));
	    	assertEquals(sentArguments.get("test7"), receivedArguments.get("test7"));
	    	if(sentArguments.containsKey("test8")) {
		    	assertTrue(receivedArguments.get("test8")!=null);
		    	assertEquals(QStack.class, receivedArguments.get("test8").getClass());
		    	assertTrue(Arrays.equals(((Collection<?>)sentArguments.get("test8")).toArray(), ((Collection<?>)receivedArguments.get("test8")).toArray()));
	    	}
	    	if(sentArguments.containsKey("test9")) {
		    	assertTrue(receivedArguments.get("test9")!=null);
		    	assertEquals(QList.class, receivedArguments.get("test9").getClass());
		    	assertEquals(((List<?>)sentArguments.get("test9")).size(), ((List<?>)receivedArguments.get("test9")).size());
		    	assertEquals(QStack.class, ((List<?>)receivedArguments.get("test9")).get(0).getClass());
		    	assertTrue(Arrays.equals(((Collection<?>)((List<?>)sentArguments.get("test9")).get(0)).toArray(), ((Collection<?>)((List<?>)receivedArguments.get("test9")).get(0)).toArray()));
		    	assertEquals(QStack.class, ((List<?>)receivedArguments.get("test9")).get(1).getClass());
		    	assertTrue(Arrays.equals(((Collection<?>)((List<?>)sentArguments.get("test9")).get(1)).toArray(), ((Collection<?>)((List<?>)receivedArguments.get("test9")).get(1)).toArray()));
	    	}
	    	if(sentArguments.containsKey("test10")) {
	    		assertEquals(sentArguments.get("test10"), receivedArguments.get("test10"));
	    	}
	    	if(sentArguments.containsKey("test11")) {
		    	assertTrue(receivedArguments.get("test11")!=null);
		    	assertEquals(qvecClass, receivedArguments.get("test11").getClass());
		    	assertEquals(((List<?>)sentArguments.get("test11")).size(), ((List<?>)receivedArguments.get("test11")).size());
		    	assertEquals(QStack.class, ((List<?>)receivedArguments.get("test11")).get(0).getClass());
		    	assertTrue(Arrays.equals(((Collection<?>)((List<?>)sentArguments.get("test11")).get(0)).toArray(), ((Collection<?>)((List<?>)receivedArguments.get("test11")).get(0)).toArray()));
		    	assertEquals(QStack.class, ((List<?>)receivedArguments.get("test11")).get(1).getClass());
		    	assertTrue(Arrays.equals(((Collection<?>)((List<?>)sentArguments.get("test11")).get(1)).toArray(), ((Collection<?>)((List<?>)receivedArguments.get("test11")).get(1)).toArray()));
	    	}
	    	if(sentArguments.containsKey("test12")) {
	    		Object received = receivedArguments.get("test12");
		    	assertTrue(received!=null);
		    	assertEquals(QMap.class, received.getClass());
		    	assertEquals(((Map<?,?>)sentArguments.get("test12")).size(), ((Map<?,?>)received).size());
		    	for(Object key : ((Map<?,?>)sentArguments.get("test12")).keySet()) {
			    	assertTrue(((Map<?,?>)received).containsKey(key));
			    	Object value = ((Map<?,?>)received).get(key);
			    	assertTrue(value!=null);
			    	assertEquals(QStack.class, value.getClass());
			    	assertTrue(Arrays.equals(((Collection<?>)((Map<?,?>)sentArguments.get("test12")).get(key)).toArray(), ((Collection<?>)((Map<?,?>)received).get(key)).toArray()));
		    	}
	    	}
	    	if(sentArguments.containsKey("test13")) {
	    		Object received = receivedArguments.get("test13");
		    	assertTrue(received!=null);
		    	assertEquals(QHash.class, received.getClass());
		    	assertEquals(((Map<?,?>)sentArguments.get("test13")).size(), ((Map<?,?>)received).size());
		    	for(Object key : ((Map<?,?>)sentArguments.get("test13")).keySet()) {
			    	assertTrue(((Map<?,?>)received).containsKey(key));
			    	Object value = ((Map<?,?>)received).get(key);
			    	assertTrue(value!=null);
			    	assertEquals(QStack.class, value.getClass());
			    	assertTrue(Arrays.equals(((Collection<?>)((Map<?,?>)sentArguments.get("test13")).get(key)).toArray(), ((Collection<?>)((Map<?,?>)received).get(key)).toArray()));
		    	}
	    	}
	    	if(sentArguments.containsKey("test14")) {
	    		Object received = receivedArguments.get("test14");
	    		assertTrue(received!=null);
		    	assertEquals(QMultiHash.class, received.getClass());
		    	assertEquals(((Map<?,?>)sentArguments.get("test14")).size(), ((QMultiHash<?,?>)received).entrySet().size());
		    	for(Object key : ((Map<?,?>)sentArguments.get("test14")).keySet()) {
		    		assertTrue(key instanceof QUrl.FormattingOptions);
			    	assertTrue(((Map<?,?>)received).containsKey(key));
			    	Object sentValue = ((Map<?,?>)sentArguments.get("test14")).get(key);
			    	Object receivedValue = ((Map<?,?>)received).get(key);
			    	assertTrue(receivedValue instanceof Collection<?>);
			    	assertTrue(((List<?>)receivedValue).get(0)!=null);
			    	assertTrue(((List<?>)receivedValue).get(1)!=null);
			    	assertEquals(QStack.class, ((List<?>)receivedValue).get(0).getClass());
			    	assertTrue(Arrays.equals(((Collection<?>)((List<?>)sentValue).get(0)).toArray(), ((Collection<?>)((List<?>)receivedValue).get(0)).toArray()));
			    	assertEquals(QStack.class, ((List<?>)receivedValue).get(1).getClass());
			    	assertTrue(Arrays.equals(((Collection<?>)((List<?>)sentValue).get(1)).toArray(), ((Collection<?>)((List<?>)receivedValue).get(1)).toArray()));
		    	}
	    	}
	    	if(sentArguments.containsKey("test15")) {
	    		Object received = receivedArguments.get("test15");
		    	assertTrue(received!=null);
		    	assertEquals(QStack.class, received.getClass());
		    	Deque<?> sent = (Deque<?>)sentArguments.get("test15");
		    	QStack<?> rstack = (QStack<?>)received;
		    	assertEquals(sent.size(), rstack.size());
		    	assertEquals(QQueue.class, rstack.getFirst().getClass());
		    	assertTrue(Arrays.equals(((Collection<?>)sent.getFirst()).toArray(), ((Collection<?>)rstack.getFirst()).toArray()));
		    	rstack.removeLast();
		    	sent.removeLast();
		    	assertEquals(QQueue.class, rstack.getFirst().getClass());
		    	assertTrue(Arrays.equals(((Collection<?>)sent.getFirst()).toArray(), ((Collection<?>)rstack.getFirst()).toArray()));
	    	}
	    	
	    	if(sentArguments.containsKey("test16")) {
	    		assertEquals(sentArguments.get("test16"), receivedArguments.get("test16"));
	    	}
	    	if(sentArguments.containsKey("test17")) {
	    		Object received = receivedArguments.get("test17");
	    		assertTrue(received!=null);
		    	assertEquals(QStack.class, received.getClass());
		    	QStack<?> stack = (QStack<?>)received;
	    		assertEquals(sentArguments.get("test17"), stack);
	    	}
	    	if(sentArguments.containsKey("test18")) {
	    		Object received = receivedArguments.get("test18");
	    		assertTrue(received!=null);
		    	assertEquals(QMultiMap.class, received.getClass());
		    	assertEquals(((Map<?,?>)sentArguments.get("test18")).size(), ((QMultiMap<?,?>)received).entrySet().size());
		    	for(Object key : ((Map<?,?>)sentArguments.get("test18")).keySet()) {
		    		assertTrue(key instanceof QUrl.FormattingOptions);
			    	assertTrue(((Map<?,?>)received).containsKey(key));
			    	Object sentValue = ((Map<?,?>)sentArguments.get("test18")).get(key);
			    	Object receivedValue = ((Map<?,?>)received).get(key);
			    	assertTrue(receivedValue instanceof Collection<?>);
			    	assertTrue(((List<?>)receivedValue).get(0)!=null);
			    	assertTrue(((List<?>)receivedValue).get(1)!=null);
			    	assertEquals(QQueue.class, ((List<?>)receivedValue).get(0).getClass());
			    	assertTrue(Arrays.equals(((Collection<?>)((List<?>)sentValue).get(0)).toArray(), ((Collection<?>)((List<?>)receivedValue).get(0)).toArray()));
			    	assertEquals(QQueue.class, ((List<?>)receivedValue).get(1).getClass());
			    	assertTrue(Arrays.equals(((Collection<?>)((List<?>)sentValue).get(1)).toArray(), ((Collection<?>)((List<?>)receivedValue).get(1)).toArray()));
		    	}
	    	}
    	}finally {
    		object.dispose();
    	}
    }
    
    @Test 
    public void testVariantGC() throws Exception {
    	QObject carrier = new QObject();
    	{
    		AtomicBoolean destroyed = new AtomicBoolean(false);
	    	{
	    		QObject prop = new QObject();
	    		prop.destroyed.connect(()->destroyed.set(true));
	    		carrier.setProperty("customProperty", prop);
	    		assertEquals("JObjectWrapper", Variants.propertyType(carrier, "customProperty").name().toString());
	    		assertEquals(new QColor(), Variants.fetchColorProperty(carrier, "customProperty"));
	    		assertEquals(prop, Variants.fetchObjectProperty(carrier, "customProperty"));
	    		assertEquals(null, Variants.fetchEventProperty(carrier, "customProperty"));
	    		assertEquals(null, Variants.fetchPaintDeviceProperty(carrier, "customProperty"));
	    		assertEquals(null, Variants.fetchRunnableProperty(carrier, "customProperty"));
	    	}
	    	for (int i = 0; i < 5; i++) {
	        	ApplicationInitializer.runGC();
	        	synchronized (carrier) {
					Thread.sleep(100);
				}
				QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose);			
			}
	    	assertFalse(destroyed.get());
	    	carrier.setProperty("customProperty", null);
    	}
    	{
        	AtomicBoolean destroyed = new AtomicBoolean(false);
	    	{
	    		QColor prop = new QColor(0xff23ab){};
	    		QtUtilities.getSignalOnDispose(prop).connect(()->destroyed.set(true));
	    		carrier.setProperty("customProperty", prop);
	    		assertEquals("JObjectWrapper", Variants.propertyType(carrier, "customProperty").name().toString());
	    		assertEquals(prop, Variants.fetchColorProperty(carrier, "customProperty"));
	    		assertEquals(null, Variants.fetchObjectProperty(carrier, "customProperty"));
	    		assertEquals(null, Variants.fetchEventProperty(carrier, "customProperty"));
	    		assertEquals(null, Variants.fetchPaintDeviceProperty(carrier, "customProperty"));
	    		assertEquals(null, Variants.fetchRunnableProperty(carrier, "customProperty"));
	    	}
	    	for (int i = 0; i < 5; i++) {
	        	ApplicationInitializer.runGC();
	        	synchronized (carrier) {
					Thread.sleep(100);
				}
				QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose);			
			}
	    	assertFalse(destroyed.get());
			carrier.setProperty("customProperty", null);
    	}
    	{
        	AtomicBoolean destroyed = new AtomicBoolean(false);
	    	{
	    		QEvent prop = new QEvent(QEvent.Type.ActionAdded);
	    		QtUtilities.getSignalOnDispose(prop).connect(()->destroyed.set(true));
	    		carrier.setProperty("customProperty", prop);
	    		assertEquals("JObjectWrapper", Variants.propertyType(carrier, "customProperty").name().toString());
	    		assertEquals(new QColor(), Variants.fetchColorProperty(carrier, "customProperty"));
	    		assertEquals(null, Variants.fetchObjectProperty(carrier, "customProperty"));
	    		assertEquals(prop, Variants.fetchEventProperty(carrier, "customProperty"));
	    		assertEquals(null, Variants.fetchPaintDeviceProperty(carrier, "customProperty"));
	    		assertEquals(null, Variants.fetchRunnableProperty(carrier, "customProperty"));
	    	}
	    	for (int i = 0; i < 5; i++) {
	        	ApplicationInitializer.runGC();
	        	synchronized (carrier) {
					Thread.sleep(100);
				}
				QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose);			
			}
	    	assertFalse(destroyed.get());
			carrier.setProperty("customProperty", null);
    	}
    	{
        	AtomicBoolean destroyed = new AtomicBoolean(false);
	    	{
	    		QEvent prop = new QEvent(QEvent.Type.ActionAdded){};
	    		QtUtilities.getSignalOnDispose(prop).connect(()->destroyed.set(true));
	    		carrier.setProperty("customProperty", prop);
	    		assertEquals("JObjectWrapper", Variants.propertyType(carrier, "customProperty").name().toString());
	    		assertEquals(new QColor(), Variants.fetchColorProperty(carrier, "customProperty"));
	    		assertEquals(null, Variants.fetchObjectProperty(carrier, "customProperty"));
	    		assertEquals(prop, Variants.fetchEventProperty(carrier, "customProperty"));
	    		assertEquals(null, Variants.fetchPaintDeviceProperty(carrier, "customProperty"));
	    		assertEquals(null, Variants.fetchRunnableProperty(carrier, "customProperty"));
	    	}
	    	for (int i = 0; i < 5; i++) {
	        	ApplicationInitializer.runGC();
	        	synchronized (carrier) {
					Thread.sleep(100);
				}
				QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose);			
			}
	    	assertFalse(destroyed.get());
			carrier.setProperty("customProperty", null);
    	}
    	{
        	AtomicBoolean destroyed = new AtomicBoolean(false);
	    	{
	    		class Prop extends QObject implements QRunnable, QPaintDevice{
					@Override
					public QPaintEngine paintEngine() {
						return null;
					}

					@Override
					public void run() {
					}
	    		}
	    		Prop prop = new Prop();
	    		prop.destroyed.connect(()->destroyed.set(true));
	    		carrier.setProperty("customProperty", prop);
	    		assertEquals("JObjectWrapper", Variants.propertyType(carrier, "customProperty").name().toString());
	    		assertEquals(new QColor(), Variants.fetchColorProperty(carrier, "customProperty"));
	    		assertEquals(prop, Variants.fetchObjectProperty(carrier, "customProperty"));
	    		assertEquals(null, Variants.fetchEventProperty(carrier, "customProperty"));
	    		assertEquals(prop, Variants.fetchPaintDeviceProperty(carrier, "customProperty"));
	    		assertEquals(prop, Variants.fetchRunnableProperty(carrier, "customProperty"));
	    	}
	    	for (int i = 0; i < 5; i++) {
	        	ApplicationInitializer.runGC();
	        	synchronized (carrier) {
					Thread.sleep(100);
				}
				QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose);			
			}
	    	assertFalse(destroyed.get());
			carrier.setProperty("customProperty", null);
    	}
    	{
        	AtomicBoolean destroyed = new AtomicBoolean(false);
	    	{
	    		class Prop extends QEvent implements QRunnable, QPaintDevice{
					public Prop() {
						super(QEvent.Type.User);
					}

					@Override
					public QPaintEngine paintEngine() {
						return null;
					}

					@Override
					public void run() {
					}
	    		}
	    		Prop prop = new Prop();
	    		QtUtilities.getSignalOnDispose(prop).connect(()->destroyed.set(true));
	    		carrier.setProperty("customProperty", prop);
	    		assertEquals("JObjectWrapper", Variants.propertyType(carrier, "customProperty").name().toString());
	    		assertEquals(new QColor(), Variants.fetchColorProperty(carrier, "customProperty"));
	    		assertEquals(null, Variants.fetchObjectProperty(carrier, "customProperty"));
	    		assertEquals(prop, Variants.fetchEventProperty(carrier, "customProperty"));
	    		assertEquals(prop, Variants.fetchPaintDeviceProperty(carrier, "customProperty"));
	    		assertEquals(prop, Variants.fetchRunnableProperty(carrier, "customProperty"));
	    	}
	    	for (int i = 0; i < 5; i++) {
	        	ApplicationInitializer.runGC();
	        	synchronized (carrier) {
					Thread.sleep(100);
				}
				QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose);			
			}
	    	assertFalse(destroyed.get());
			carrier.setProperty("customProperty", null);
    	}
    	{
        	AtomicBoolean destroyed = new AtomicBoolean(false);
	    	{
	    		QRunnable prop = ()->{};
	    		prop.isDisposed();
	    		QtUtilities.getSignalOnDispose(prop).connect(()->destroyed.set(true));
	    		carrier.setProperty("customProperty", prop);
	    		assertEquals("JObjectWrapper", Variants.propertyType(carrier, "customProperty").name().toString());
	    		assertEquals(new QColor(), Variants.fetchColorProperty(carrier, "customProperty"));
	    		assertEquals(null, Variants.fetchObjectProperty(carrier, "customProperty"));
	    		assertEquals(null, Variants.fetchEventProperty(carrier, "customProperty"));
	    		assertEquals(null, Variants.fetchPaintDeviceProperty(carrier, "customProperty"));
	    		assertEquals(prop, Variants.fetchRunnableProperty(carrier, "customProperty"));
	    	}
	    	for (int i = 0; i < 5; i++) {
	        	ApplicationInitializer.runGC();
	        	synchronized (carrier) {
					Thread.sleep(100);
				}
				QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose);			
			}
	    	assertFalse(destroyed.get());
			carrier.setProperty("customProperty", null);
    	}
    	{
        	AtomicBoolean destroyed = new AtomicBoolean(false);
	    	{
	    		QPaintDevice prop = ()->null;
	    		prop.isDisposed();
	    		QtUtilities.getSignalOnDispose(prop).connect(()->destroyed.set(true));
	    		carrier.setProperty("customProperty", prop);
	    		assertEquals("JObjectWrapper", Variants.propertyType(carrier, "customProperty").name().toString());
	    		assertEquals(new QColor(), Variants.fetchColorProperty(carrier, "customProperty"));
	    		assertEquals(null, Variants.fetchObjectProperty(carrier, "customProperty"));
	    		assertEquals(null, Variants.fetchEventProperty(carrier, "customProperty"));
	    		assertEquals(prop, Variants.fetchPaintDeviceProperty(carrier, "customProperty"));
	    		assertEquals(null, Variants.fetchRunnableProperty(carrier, "customProperty"));
	    	}
	    	for (int i = 0; i < 5; i++) {
	        	ApplicationInitializer.runGC();
	        	synchronized (carrier) {
					Thread.sleep(100);
				}
				QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose);			
			}
	    	assertFalse(destroyed.get());
			carrier.setProperty("customProperty", null);
    	}
    	{
        	AtomicBoolean destroyed = new AtomicBoolean(false);
	    	{
	    		class Prop extends QtObject implements QRunnable, QPaintDevice{
					public Prop() {
						super();
					}

					@Override
					public QPaintEngine paintEngine() {
						return null;
					}

					@Override
					public void run() {
					}
	    		}
	    		Prop prop = new Prop();
	    		QtUtilities.getSignalOnDispose(prop).connect(()->destroyed.set(true));
	    		carrier.setProperty("customProperty", prop);
	    		assertEquals("JObjectWrapper", Variants.propertyType(carrier, "customProperty").name().toString());
	    		assertEquals(new QColor(), Variants.fetchColorProperty(carrier, "customProperty"));
	    		assertEquals(null, Variants.fetchObjectProperty(carrier, "customProperty"));
	    		assertEquals(null, Variants.fetchEventProperty(carrier, "customProperty"));
	    		assertEquals(prop, Variants.fetchPaintDeviceProperty(carrier, "customProperty"));
	    		assertEquals(prop, Variants.fetchRunnableProperty(carrier, "customProperty"));
	    	}
	    	for (int i = 0; i < 5; i++) {
	        	ApplicationInitializer.runGC();
	        	synchronized (carrier) {
					Thread.sleep(100);
				}
				QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose);			
			}
	    	assertFalse(destroyed.get());
			carrier.setProperty("customProperty", null);
    	}
    	{
	    	AtomicBoolean destroyed = new AtomicBoolean(false);
	    	{
	    		QColor prop = new QColor(0xff23ad);
	    		QMetaObject.Connection connection = QtUtilities.getSignalOnDispose(prop).connect(()->destroyed.set(true));
	    		assertTrue("not connected", connection.isConnected());
	    		carrier.setProperty("customProperty", prop);
	    		assertEquals(QMetaType.fromType(QColor.class), Variants.propertyType(carrier, "customProperty"));
	    		assertEquals(prop, Variants.fetchColorProperty(carrier, "customProperty"));
	    		assertEquals(null, Variants.fetchObjectProperty(carrier, "customProperty"));
	    		assertEquals(null, Variants.fetchEventProperty(carrier, "customProperty"));
	    		assertEquals(null, Variants.fetchPaintDeviceProperty(carrier, "customProperty"));
	    		assertEquals(null, Variants.fetchRunnableProperty(carrier, "customProperty"));
	    		prop = null;
	    	}
	    	for (int i = 0; i < 25; i++) {
	    		if(destroyed.get())
	    			break;
	        	ApplicationInitializer.runGC();
	        	synchronized (carrier) {
	        		Thread.yield();
					Thread.sleep(100);
				}
				QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose);			
			}
	    	assertTrue(destroyed.get());
			carrier.setProperty("customProperty", null);
    	}
    }
    
    static <K, V> Map<K, V> mapOf(K k1, V v1) {
    	Map<K,V> map = new HashMap<>();
    	map.put(k1, v1);
    	return map;
    }
    
    static <K, V> Map<K, V> mapOf(K k1, V v1, K k2, V v2) {
    	Map<K,V> map = new HashMap<>();
    	map.put(k2, v2);
    	return map;
    }
    
    @Test
    public void testConvertInterface() {
    	QGraphicsObject object = new QGraphicsWidget();
    	QMetaType.fromType(QGraphicsObject.class);
    	QGraphicsItem item = io.qt.autotests.generated.Variants.convertInterface(object);
    	Assert.assertTrue(item==object);
    }
    
    @Test
    public void testConvertJavaObject() {
    	QObject qobject = new QWidget();
    	QObject result = Variants.convertJavaObject(qobject);
    	Assert.assertTrue(result==qobject);
    }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestQVariant.class.getName());
    }
}
