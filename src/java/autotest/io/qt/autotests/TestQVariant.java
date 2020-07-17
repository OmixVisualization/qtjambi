/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2020 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
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
import java.util.TreeMap;
import java.util.TreeSet;
import java.util.function.Consumer;

import org.junit.Test;

import io.qt.QtEnumerator;
import io.qt.QtUninvokable;
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
import io.qt.core.QFileInfo;
import io.qt.core.QJsonArray;
import io.qt.core.QJsonDocument;
import io.qt.core.QJsonObject;
import io.qt.core.QJsonValue;
import io.qt.core.QLine;
import io.qt.core.QLineF;
import io.qt.core.QLocale;
import io.qt.core.QMetaMethod;
import io.qt.core.QMetaMethod.MethodType;
import io.qt.core.QMetaObject;
import io.qt.core.QMetaType;
import io.qt.core.QModelIndex;
import io.qt.core.QObject;
import io.qt.core.QPair;
import io.qt.core.QPersistentModelIndex;
import io.qt.core.QPoint;
import io.qt.core.QPointF;
import io.qt.core.QRect;
import io.qt.core.QRectF;
import io.qt.core.QRegExp;
import io.qt.core.QRegularExpression;
import io.qt.core.QSize;
import io.qt.core.QSizeF;
import io.qt.core.QThread;
import io.qt.core.QTime;
import io.qt.core.QUrl;
import io.qt.core.QUuid;
import io.qt.core.QVariant;
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
import io.qt.widgets.QGraphicsObject;
import io.qt.widgets.QGraphicsPixmapItem;
import io.qt.widgets.QGraphicsWidget;
import io.qt.widgets.QLabel;
import io.qt.widgets.QSizePolicy;
import io.qt.widgets.QSpinBox;
import io.qt.widgets.QStyleOptionGraphicsItem;
import io.qt.widgets.QWidget;

public class TestQVariant extends QApplicationTest {
	
    @Test
    public void testQVariant() {
    	assertEquals(QMetaType.Type.Int.value(), QMetaType.metaTypeId(int.class));
    	assertEquals(QMetaType.Type.LongLong.value(), QMetaType.metaTypeId(long.class));
    	assertEquals(QMetaType.Type.Bool.value(), QMetaType.metaTypeId(boolean.class));
    	assertEquals(QMetaType.Type.Short.value(), QMetaType.metaTypeId(short.class));
    	assertEquals(QMetaType.Type.SChar.value(), QMetaType.metaTypeId(byte.class));
    	assertEquals(QMetaType.Type.Double.value(), QMetaType.metaTypeId(double.class));
    	assertEquals(QMetaType.Type.Float.value(), QMetaType.metaTypeId(float.class));
    	assertEquals(QMetaType.Type.QChar.value(), QMetaType.metaTypeId(char.class));
    	assertEquals(QMetaType.Type.Int.value(), QMetaType.metaTypeId(Integer.class));
    	assertEquals(QMetaType.Type.LongLong.value(), QMetaType.metaTypeId(Long.class));
    	assertEquals(QMetaType.Type.Bool.value(), QMetaType.metaTypeId(Boolean.class));
    	assertEquals(QMetaType.Type.Short.value(), QMetaType.metaTypeId(Short.class));
    	assertEquals(QMetaType.Type.SChar.value(), QMetaType.metaTypeId(Byte.class));
    	assertEquals(QMetaType.Type.Double.value(), QMetaType.metaTypeId(Double.class));
    	assertEquals(QMetaType.Type.Float.value(), QMetaType.metaTypeId(Float.class));
    	assertEquals(QMetaType.Type.QChar.value(), QMetaType.metaTypeId(Character.class));
    	assertEquals(QMetaType.Type.QBitArray.value(), QMetaType.metaTypeId(QBitArray.class));
    	assertEquals(QMetaType.Type.QByteArray.value(), QMetaType.metaTypeId(QByteArray.class));
    	assertEquals(QMetaType.Type.QBrush.value(), QMetaType.metaTypeId(QBrush.class));
    	assertEquals(QMetaType.Type.QBitmap.value(), QMetaType.metaTypeId(QBitmap.class));
    	assertEquals(QMetaType.Type.QCborArray.value(), QMetaType.metaTypeId(QCborArray.class));
    	assertEquals(QMetaType.Type.QCborMap.value(), QMetaType.metaTypeId(QCborMap.class));
//    	assertEquals(QMetaType.Type.QCborValue.value(), QMetaType.metaTypeId(QCborValue.class));
    	assertEquals(QMetaType.Type.QColor.value(), QMetaType.metaTypeId(QColor.class));
    	assertEquals(QMetaType.Type.QCursor.value(), QMetaType.metaTypeId(QCursor.class));
    	assertEquals(QMetaType.Type.QDate.value(), QMetaType.metaTypeId(QDate.class));
    	assertEquals(QMetaType.Type.QDateTime.value(), QMetaType.metaTypeId(QDateTime.class));
    	assertEquals(QMetaType.Type.QEasingCurve.value(), QMetaType.metaTypeId(QEasingCurve.class));
    	assertEquals(QMetaType.Type.QFont.value(), QMetaType.metaTypeId(QFont.class));
    	assertEquals(QMetaType.Type.QIcon.value(), QMetaType.metaTypeId(QIcon.class));
    	assertEquals(QMetaType.Type.QImage.value(), QMetaType.metaTypeId(QImage.class));
    	assertEquals(QMetaType.Type.QJsonArray.value(), QMetaType.metaTypeId(QJsonArray.class));
    	assertEquals(QMetaType.Type.QJsonDocument.value(), QMetaType.metaTypeId(QJsonDocument.class));
    	assertEquals(QMetaType.Type.QJsonObject.value(), QMetaType.metaTypeId(QJsonObject.class));
    	assertEquals(QMetaType.Type.QJsonValue.value(), QMetaType.metaTypeId(QJsonValue.class));
    	assertEquals(QMetaType.Type.QKeySequence.value(), QMetaType.metaTypeId(QKeySequence.class));
    	assertEquals(QMetaType.Type.QLine.value(), QMetaType.metaTypeId(QLine.class));
    	assertEquals(QMetaType.Type.QLineF.value(), QMetaType.metaTypeId(QLineF.class));
    	assertEquals(QMetaType.Type.QLocale.value(), QMetaType.metaTypeId(QLocale.class));
    	assertEquals(QMetaType.Type.QMatrix4x4.value(), QMetaType.metaTypeId(QMatrix4x4.class));
    	assertEquals(QMetaType.Type.QModelIndex.value(), QMetaType.metaTypeId(QModelIndex.class));
    	assertEquals(QMetaType.Type.QPalette.value(), QMetaType.metaTypeId(QPalette.class));
    	assertEquals(QMetaType.Type.QPen.value(), QMetaType.metaTypeId(QPen.class));
    	assertEquals(QMetaType.Type.QPersistentModelIndex.value(), QMetaType.metaTypeId(QPersistentModelIndex.class));
    	assertEquals(QMetaType.Type.QPixmap.value(), QMetaType.metaTypeId(QPixmap.class));
    	assertEquals(QMetaType.Type.QPoint.value(), QMetaType.metaTypeId(QPoint.class));
    	assertEquals(QMetaType.Type.QPointF.value(), QMetaType.metaTypeId(QPointF.class));
    	assertEquals(QMetaType.Type.QPolygon.value(), QMetaType.metaTypeId(QPolygon.class));
    	assertEquals(QMetaType.Type.QPolygonF.value(), QMetaType.metaTypeId(QPolygonF.class));
    	assertEquals(QMetaType.Type.QQuaternion.value(), QMetaType.metaTypeId(QQuaternion.class));
    	assertEquals(QMetaType.Type.QRect.value(), QMetaType.metaTypeId(QRect.class));
    	assertEquals(QMetaType.Type.QRectF.value(), QMetaType.metaTypeId(QRectF.class));
    	assertEquals(QMetaType.Type.QRegExp.value(), QMetaType.metaTypeId(QRegExp.class));
    	assertEquals(QMetaType.Type.QRegion.value(), QMetaType.metaTypeId(QRegion.class));
    	assertEquals(QMetaType.Type.QRegularExpression.value(), QMetaType.metaTypeId(QRegularExpression.class));
    	assertEquals(QMetaType.Type.QQuaternion.value(), QMetaType.metaTypeId(QQuaternion.class));
    	assertEquals(QMetaType.Type.QSize.value(), QMetaType.metaTypeId(QSize.class));
    	assertEquals(QMetaType.Type.QSizeF.value(), QMetaType.metaTypeId(QSizeF.class));
    	assertEquals(QMetaType.Type.QSizePolicy.value(), QMetaType.metaTypeId(QSizePolicy.class));
    	assertEquals(QMetaType.Type.QString.value(), QMetaType.metaTypeId(String.class));
    	assertEquals(QMetaType.Type.QTextFormat.value(), QMetaType.metaTypeId(QTextFormat.class));
    	assertEquals(QMetaType.Type.QTextLength.value(), QMetaType.metaTypeId(QTextLength.class));
    	assertEquals(QMetaType.Type.QTime.value(), QMetaType.metaTypeId(QTime.class));
    	assertEquals(QMetaType.Type.QTransform.value(), QMetaType.metaTypeId(QTransform.class));
    	assertEquals(QMetaType.Type.QUrl.value(), QMetaType.metaTypeId(QUrl.class));
    	assertEquals(QMetaType.Type.QUuid.value(), QMetaType.metaTypeId(QUuid.class));
    	assertEquals(QMetaType.Type.QVector2D.value(), QMetaType.metaTypeId(QVector2D.class));
    	assertEquals(QMetaType.Type.QVector3D.value(), QMetaType.metaTypeId(QVector3D.class));
    	assertEquals(QMetaType.Type.QVector4D.value(), QMetaType.metaTypeId(QVector4D.class));
    	assertEquals(QMetaType.Type.QVariant.value(), QMetaType.metaTypeId(QVariant.class));
    	assertEquals(QMetaType.Type.QObjectStar.value(), QMetaType.metaTypeId(QObject.class));
    	assertEquals(QMetaType.Type.Nullptr.value(), QMetaType.metaTypeId(null));
    	assertEquals("JObjectWrapper", QMetaType.typeName(QMetaType.metaTypeId(Object.class)));
    	assertEquals("JCollectionWrapper", QMetaType.typeName(QMetaType.metaTypeId(List.class)));
    	assertEquals("JMapWrapper", QMetaType.typeName(QMetaType.metaTypeId(Map.class)));
    	assertEquals("JEnumWrapper", QMetaType.typeName(QMetaType.metaTypeId(Enum.class)));
    	assertEquals("QWidget*", QMetaType.typeName(QMetaType.metaTypeId(QWidget.class)));
    	assertEquals(new QSize(5, 5), QVariant.convert(new QSizeF(5, 5), QSize.class));
    	assertEquals(new QLine(6, 5, 8, 1), QVariant.convert(new QLineF(5.8, 5.2, 8.3, 1.2), QLine.class));
    	Tulip tulip = new Tulip();
    	assertEquals(QMetaType.Type.QStringList.value(), QVariant.type(tulip.listRef()));
    	assertEquals(QMetaType.Type.QByteArrayList.value(), QVariant.type(tulip.byteArrayListRef()));
    }
    
    @Test
    public void testQVariant_IntVector() {
    	Object variant = Variants.getIntVector();
    	assertTrue(variant instanceof ArrayList);
    	assertEquals(Arrays.asList(1, 2, 3, 4), variant);
    }
    
    @Test
    public void testQVariant_IntLinkedList() {
    	Object variant = Variants.getIntLinkedList();
    	assertTrue(variant instanceof LinkedList);
    	assertEquals(Arrays.asList(1, 2, 3, 4), variant);
    }
    
    @Test
    public void testQVariant_IntList() {
    	Object variant = Variants.getIntList();
    	assertTrue(variant instanceof ArrayList);
    	assertEquals(Arrays.asList(1, 2, 3, 4), variant);
    }
    
    @Test
    public void testQVariant_IntSet() {
    	Object variant = Variants.getIntSet();
    	assertTrue(variant instanceof HashSet);
    	assertEquals(new TreeSet<>(Arrays.asList(1, 2, 3, 4)), variant);
    }
    
    @Test
    public void testQVariant_IntQueue() {
    	Object variant = Variants.getIntQueue();
    	assertTrue(variant instanceof ArrayDeque);
    	assertEquals(Arrays.asList(1, 2, 3, 4), new ArrayList<>((ArrayDeque<?>)variant));
    }
    
    @Test
    public void testQVariant_IntStack() {
    	Object variant = Variants.getIntStack();
    	assertTrue(variant instanceof ArrayDeque);
    	assertEquals(Arrays.asList(1, 2, 3, 4), new ArrayList<>((ArrayDeque<?>)variant));
    }
    
    @Test
    public void testQVariant_StringVector() {
    	Object variant = Variants.getStringVector();
    	assertTrue(variant instanceof ArrayList);
    	assertEquals(Arrays.asList("S1", "S2", "S3", "S4"), variant);
    }
    
    @Test
    public void testQVariant_StringLinkedList() {
    	Object variant = Variants.getStringLinkedList();
    	assertTrue(variant instanceof LinkedList);
    	assertEquals(Arrays.asList("S1", "S2", "S3", "S4"), variant);
    }
    
    @Test
    public void testQVariant_StringList() {
    	Object variant = Variants.getStringList();
    	assertTrue(variant instanceof ArrayList);
    	assertEquals(Arrays.asList("S1", "S2", "S3", "S4"), variant);
    }
    
    @Test
    public void testQVariant_StringSet() {
    	Object variant = Variants.getStringSet();
    	assertTrue(variant instanceof HashSet);
    	assertEquals(new TreeSet<>(Arrays.asList("S1", "S2", "S3", "S4")), variant);
    }
    
    @Test
    public void testQVariant_StringQueue() {
    	Object variant = Variants.getStringQueue();
    	assertTrue(variant instanceof ArrayDeque);
    	assertEquals(Arrays.asList("S1", "S2", "S3", "S4"), new ArrayList<>((ArrayDeque<?>)variant));
    }
    
    @Test
    public void testQVariant_StringStack() {
    	Object variant = Variants.getStringStack();
    	assertTrue(variant instanceof ArrayDeque);
    	assertEquals(Arrays.asList("S1", "S2", "S3", "S4"), new ArrayList<>((ArrayDeque<?>)variant));
    }
    
    @Test
    public void testQVariant_UnknownEnumVector() {
    	Object variant = Variants.getUnknownEnumVector();
    	assertTrue(variant instanceof ArrayList);
    	assertEquals(4, ((Collection<?>)variant).size());
    	int i = 101;
    	for(Object e : (Collection<?>)variant) {
    		assertTrue(e instanceof QtEnumerator);
    		assertEquals(i++, ((QtEnumerator)e).value());
    	}
    }
    
    @Test
    public void testQVariant_UnknownEnumLinkedList() {
    	Object variant = Variants.getUnknownEnumLinkedList();
    	assertTrue(variant instanceof LinkedList);
    	assertEquals(4, ((Collection<?>)variant).size());
    	int i = 101;
    	for(Object e : (Collection<?>)variant) {
    		assertTrue(e instanceof QtEnumerator);
    		assertEquals(i++, ((QtEnumerator)e).value());
    	}
    }
    
    @Test
    public void testQVariant_UnknownEnumList() {
    	Object variant = Variants.getUnknownEnumList();
    	assertTrue(variant instanceof ArrayList);
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
    	assertTrue(variant instanceof HashSet);
    	assertEquals(4, ((Collection<?>)variant).size());
    	HashSet<Integer> values = new HashSet<Integer>();
    	for(Object e : (Collection<?>)variant) {
    		assertTrue(e instanceof QtEnumerator);
    		values.add(((QtEnumerator)e).value());
    	}
    	assertEquals(new TreeSet<>(Arrays.asList(101, 102, 103, 104)), values);
    }
    
    @Test
    public void testQVariant_UnknownEnumQueue() {
    	Object variant = Variants.getUnknownEnumQueue();
    	assertTrue(variant instanceof ArrayDeque);
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
    	assertTrue(variant instanceof ArrayDeque);
    	assertEquals(4, ((Collection<?>)variant).size());
    	int i = 101;
    	for(Object e : (Collection<?>)variant) {
    		assertTrue(e instanceof QtEnumerator);
    		assertEquals(i++, ((QtEnumerator)e).value());
    	}
    }
    
    @Test
    public void testQVariant_DirVector() {
    	Object variant = Variants.getDirVector();
    	assertTrue(variant instanceof ArrayList);
    	assertEquals(Arrays.asList(new QDir("/"), new QDir("/home")), variant);
    }
    
    @Test
    public void testQVariant_DirLinkedList() {
    	Object variant = Variants.getDirLinkedList();
    	assertTrue(variant instanceof LinkedList);
    	assertEquals(Arrays.asList(new QDir("/"), new QDir("/home")), variant);
    }
    
    @Test
    public void testQVariant_DirList() {
    	Object variant = Variants.getDirList();
    	assertTrue(variant instanceof ArrayList);
    	assertEquals(Arrays.asList(new QDir("/"), new QDir("/home")), variant);
    }
    
    @Test
    public void testQVariant_DirSet() {
    	Object variant = Variants.getDirSet();
    	assertNotNull(variant);
    	assertTrue(variant.getClass().getName(), variant instanceof HashSet);
    	assertEquals(new HashSet<>(Arrays.asList(new QDir("/"), new QDir("/home"))), variant);
    }
    
    @Test
    public void testQVariant_DirQueue() {
    	Object variant = Variants.getDirQueue();
    	assertNotNull(variant);
    	assertTrue(variant.getClass().getName(), variant instanceof ArrayDeque);
    	assertEquals(Arrays.asList(new QDir("/"), new QDir("/home")), new ArrayList<>((ArrayDeque<?>)variant));
    }
    
    @Test
    public void testQVariant_DirStack() {
    	Object variant = Variants.getDirStack();
    	assertNotNull(variant);
    	assertTrue(variant.getClass().getName(), variant instanceof ArrayDeque);
    	assertEquals(Arrays.asList(new QDir("/"), new QDir("/home")), new ArrayList<>((ArrayDeque<?>)variant));
    }
    
    @Test
    public void testQVariant_ColorHash() {
    	Object variant = Variants.getColorHash();
    	assertNotNull(variant);
    	assertTrue(variant.getClass().getName(), variant instanceof HashMap);
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
    	assertTrue(variant.getClass().getName(), variant instanceof TreeMap);
    	for(Map.Entry<?,?> entry : ((Map<?,?>)variant).entrySet()) {
        	assertTrue(entry.getKey() instanceof Integer);
        	assertTrue(entry.getValue() instanceof String);
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
    	assertTrue(variant.getClass().getName(), variant instanceof TreeMap);
    	for(Map.Entry<?,?> entry : ((Map<?,?>)variant).entrySet()) {
        	assertTrue(entry.getKey() instanceof QtEnumerator);
        	assertEquals(entry.getValue(), ( (QtEnumerator)entry.getKey() ).value()-100);
    	}
    }
    
    @Test
    public void testQVariant_UnknownEnumHash() {
    	Object variant = Variants.getUnknownEnumHash();
    	assertNotNull(variant);
    	assertTrue(variant.getClass().getName(), variant instanceof HashMap);
    	for(Map.Entry<?,?> entry : ((Map<?,?>)variant).entrySet()) {
        	assertTrue(entry.getKey() instanceof QtEnumerator);
        	assertEquals(entry.getValue(), ( (QtEnumerator)entry.getKey() ).value()-100);
    	}
    }
    
    private final Consumer<Collection<?>> variantConsumer = collection -> {
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
    	assertTrue(variant.getClass().getName(), variant instanceof HashMap);
    }
    
    @Test
    public void testQVariant_TestObjectVector() {
    	Object variant = Variants.getTestObjectVector();
    	assertTrue(variant instanceof ArrayList);
    	variantConsumer.accept((ArrayList<?>)variant);
    }
    
    @Test
    public void testQVariant_TestObjectLinkedList() {
    	Object variant = Variants.getTestObjectLinkedList();
    	assertTrue(variant instanceof LinkedList);
    	variantConsumer.accept((LinkedList<?>)variant);
    }
    
    @Test
    public void testQVariant_TestObjectList() {
    	Object variant = Variants.getTestObjectList();
    	assertTrue(variant instanceof ArrayList);
    	variantConsumer.accept((ArrayList<?>)variant);
    }
    
    @Test
    public void testQVariant_TestObjectSet() {
    	Object variant = Variants.getTestObjectSet();
    	assertTrue(variant instanceof HashSet);
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
    	assertTrue(variant instanceof HashSet);
    	assertEquals(new HashSet<>(Arrays.asList(QCoreApplication.instance(), QThread.currentThread(), QAbstractEventDispatcher.instance())), variant);
    }
    
    @Test
    public void testQVariant_TestObjectLinkedQueue() {
    	Object variant = Variants.getTestObjectQueue();
    	assertTrue(variant instanceof ArrayDeque);
    	variantConsumer.accept((ArrayDeque<?>)variant);
    }
    
    @Test
    public void testQVariant_TestObjectStack() {
    	Object variant = Variants.getTestObjectStack();
    	assertTrue(variant instanceof ArrayDeque);
    	variantConsumer.accept((ArrayDeque<?>)variant);
    }
    
    @Test
    public void testQVariant_TestObjectMap() {
    	Object variant = Variants.getTestObjectMap();
    	assertTrue(variant instanceof TreeMap);
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
    	assertTrue(variant.getClass().getName(), variant instanceof HashMap);
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
    public void testQVariant_UnknownValueVector() {
    	Object variant = Variants.getUnknownValueVector();
    	assertTrue(variant instanceof ArrayList);
    	assertEquals(1, ((Collection<?>)variant).size());
    	for(Object o : (Collection<?>)variant) {
    		assertTrue(o instanceof QMetaType.GenericValue);
    		assertEquals("UnknownValue", QMetaType.typeName(((QMetaType.GenericTypeInterface)o).metaType()));
    	}
    }
    
    @Test
    public void testQVariant_UnknownValueList() {
    	Object variant = Variants.getUnknownValueList();
    	assertTrue(variant instanceof ArrayList);
    	assertEquals(1, ((Collection<?>)variant).size());
    	for(Object o : (Collection<?>)variant) {
    		assertTrue(o instanceof QMetaType.GenericValue);
    		assertEquals("UnknownValue", QMetaType.typeName(((QMetaType.GenericTypeInterface)o).metaType()));
    	}
    }
    
    @Test
    public void testQVariant_UnknownValueLinkedList() {
    	Object variant = Variants.getUnknownValueLinkedList();
    	assertTrue(variant instanceof LinkedList);
    	assertEquals(1, ((Collection<?>)variant).size());
    	for(Object o : (Collection<?>)variant) {
    		assertTrue(o instanceof QMetaType.GenericValue);
    		assertEquals("UnknownValue", QMetaType.typeName(((QMetaType.GenericTypeInterface)o).metaType()));
    	}
    }
    
    @Test
    public void testQVariant_UnknownValueHash() {
    	Object variant = Variants.getUnknownValueHash();
    	assertNotNull(variant);
    	assertTrue(variant.getClass().getName(), variant instanceof HashMap);
    	assertEquals(1, ((Map<?,?>)variant).size());
    	for(Map.Entry<?, ?> entry : ((Map<?,?>)variant).entrySet()) {
    		assertTrue(entry.getKey() instanceof QMetaType.GenericValue);
    		assertEquals("UnknownValue", QMetaType.typeName(((QMetaType.GenericTypeInterface)entry.getKey()).metaType()));
    		assertEquals(1, entry.getValue());
    	}
    }
    
    @Test
    public void testQVariant_UnknownValueMap() {
    	Object variant = Variants.getUnknownValueMap();
    	assertTrue(variant instanceof TreeMap);
    	assertEquals(1, ((Map<?,?>)variant).size());
    	for(Map.Entry<?, ?> entry : ((Map<?,?>)variant).entrySet()) {
    		assertTrue(entry.getKey() instanceof QMetaType.GenericValue);
    		assertEquals("UnknownValue", QMetaType.typeName(((QMetaType.GenericTypeInterface)entry.getKey()).metaType()));
    		assertEquals(1, entry.getValue());
    	}
    }
    	
    @Test
    public void testQVariant_StringMultiMap() {
    	Object variant = Variants.getStringMultiMap();
    	assertTrue(variant instanceof TreeMap);
    	assertEquals(4, ((Map<?,?>)variant).size());
    	for(Map.Entry<?, ?> entry : ((Map<?,?>)variant).entrySet()) {
    		assertTrue(entry.getKey() instanceof String);
    		assertTrue(entry.getKey().toString().startsWith("TEST"));
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
    public void testQVariant_TestObjectMultiMap() {
    	Object variant = Variants.getTestObjectMultiMap();
    	assertTrue("variant is null", variant!=null);
    	assertTrue(variant.getClass().getName(), variant instanceof TreeMap);
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
    	assertTrue("variant is null", variant!=null);
    	assertTrue(variant.getClass().getName(), variant instanceof TreeMap);
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
    	assertTrue(variant instanceof HashMap);
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
    	assertTrue(variant instanceof HashMap);
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
    	assertTrue(variant instanceof HashMap);
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
    	assertTrue(variant.getClass().getName(), variant instanceof HashMap);
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
    	assertTrue(variant.getClass().getName(), variant instanceof HashMap);
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
    	Object variant = Variants.getUnknownEnumMultiHash();
    	assertNotNull(variant);
    	assertTrue(variant.getClass().getName(), variant instanceof HashMap);
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
    	assertTrue(variant.getClass().getName(), variant instanceof HashMap);
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
    	assertTrue(variant.getClass().getName(), variant instanceof TreeMap);
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
    	assertTrue(variant.getClass().getName(), variant instanceof HashMap);
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
    	assertTrue(variant.getClass().getName(), variant instanceof HashMap);
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
    	assertTrue(variant.getClass().getName(), variant instanceof HashMap);
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
    	assertTrue(variant.getClass().getName(), variant instanceof TreeMap);
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
    	assertTrue(variant.getClass().getName(), variant instanceof HashMap);
    }
    	
    @Test
    public void testQVariant_CharMultiMap() {
    	Object variant = Variants.getCharMultiMap();
    	assertNotNull(variant);
    	assertTrue(variant.getClass().getName(), variant instanceof TreeMap);
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
        QRegExp expectedRegExp = new QRegExp();
        QSize expectedSize = new QSize();
        QSizeF expectedSizeF = new QSizeF();
        List<Object> expectedList = new ArrayList<Object>();
        Map<String, Object> expectedMap = new HashMap<String, Object>();

        testQVariant("Normal string", variant, expectedDouble, true, expectedString, true, expectedByteArray, true, expectedInt, true, expectedBool, true, expectedBitArray, false, expectedChar,
                false, expectedDate, true, expectedTime, true, expectedDateTime, true, expectedPoint, false, expectedPointF, false, expectedRect, false, expectedRectF, false, expectedRegExp, false,
                expectedSize, false, expectedSizeF, false, expectedList, false, expectedMap, false);
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
        QRegExp expectedRegExp = new QRegExp();
        QSize expectedSize = new QSize();
        QSizeF expectedSizeF = new QSizeF();
        List<Object> expectedList = new ArrayList<Object>();
        Map<String, Object> expectedMap = new HashMap<String, Object>();

        testQVariant("QByteArray", variant, expectedDouble, true, expectedString, true, expectedByteArray, true, expectedInt, true, expectedBool, true, expectedBitArray,
                false, expectedChar, false, expectedDate, false, expectedTime, false, expectedDateTime, false, expectedPoint, false, expectedPointF, false, expectedRect, false, expectedRectF, false,
                expectedRegExp, false, expectedSize, false, expectedSizeF, false, expectedList, false, expectedMap, false);
    }

    @Test
    public void run_QVariantObject() {
        Object variant = new QObject();
        String expectedString = "";
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
        QRegExp expectedRegExp = new QRegExp();
        QSize expectedSize = new QSize();
        QSizeF expectedSizeF = new QSizeF();
        List<Object> expectedList = new ArrayList<Object>();
        Map<String, Object> expectedMap = new HashMap<String, Object>();

        testQVariant("java.lang.Object", variant, expectedDouble, false, expectedString, true, expectedByteArray, false, expectedInt, false, expectedBool, false,
                expectedBitArray, false, expectedChar, false, expectedDate, false, expectedTime, false, expectedDateTime, false, expectedPoint, false, expectedPointF, false, expectedRect, false,
                expectedRectF, false, expectedRegExp, false, expectedSize, false, expectedSizeF, false, expectedList, false, expectedMap, false);
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
        QRegExp expectedRegExp = new QRegExp();
        QSize expectedSize = new QSize();
        QSizeF expectedSizeF = new QSizeF();
        List<Object> expectedList = new ArrayList<Object>();
        Map<String, Object> expectedMap = new HashMap<String, Object>();

        testQVariant("String double", variant, expectedDouble, true, expectedString, true, expectedByteArray, true, expectedInt, true, expectedBool, true, expectedBitArray,
                false, expectedChar, false, expectedDate, true, expectedTime, true, expectedDateTime, true, expectedPoint, false, expectedPointF, false, expectedRect, false, expectedRectF, false,
                expectedRegExp, false, expectedSize, false, expectedSizeF, false, expectedList, false, expectedMap, false);
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
        QRegExp expectedRegExp = new QRegExp();
        QSize expectedSize = new QSize();
        QSizeF expectedSizeF = new QSizeF();
        List<Object> expectedList = new ArrayList<Object>();
        Map<String, Object> expectedMap = new HashMap<String, Object>();

        testQVariant("Byte array double", variant, expectedDouble, true, expectedString, true, expectedByteArray, true, expectedInt, true, expectedBool, true,
                expectedBitArray, false, expectedChar, false, expectedDate, false, expectedTime, false, expectedDateTime, false, expectedPoint, false, expectedPointF, false, expectedRect, false,
                expectedRectF, false, expectedRegExp, false, expectedSize, false, expectedSizeF, false, expectedList, false, expectedMap, false);
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
        QRegExp expectedRegExp = new QRegExp();
        QSize expectedSize = new QSize();
        QSizeF expectedSizeF = new QSizeF();
        List<Object> expectedList = new ArrayList<Object>();
        Map<String, Object> expectedMap = new HashMap<String, Object>();

        testQVariant("String integer", variant, expectedDouble, true, expectedString, true, expectedByteArray, true, expectedInt, true, expectedBool, true, expectedBitArray,
                false, expectedChar, false, expectedDate, true, expectedTime, true, expectedDateTime, true, expectedPoint, false, expectedPointF, false, expectedRect, false, expectedRectF, false,
                expectedRegExp, false, expectedSize, false, expectedSizeF, false, expectedList, false, expectedMap, false);
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
        QRegExp expectedRegExp = new QRegExp();
        QSize expectedSize = new QSize();
        QSizeF expectedSizeF = new QSizeF();
        List<Object> expectedList = new ArrayList<Object>();
        Map<String, Object> expectedMap = new HashMap<String, Object>();

        testQVariant("String boolean", variant, expectedDouble, true, expectedString, true, expectedByteArray, true, expectedInt, true, expectedBool, true, expectedBitArray,
                false, expectedChar, false, expectedDate, true, expectedTime, true, expectedDateTime, true, expectedPoint, false, expectedPointF, false, expectedRect, false, expectedRectF, false,
                expectedRegExp, false, expectedSize, false, expectedSizeF, false, expectedList, false, expectedMap, false);
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
        QRegExp expectedRegExp = new QRegExp();
        QSize expectedSize = new QSize();
        QSizeF expectedSizeF = new QSizeF();
        List<Object> expectedList = new ArrayList<Object>();
        Map<String, Object> expectedMap = new HashMap<String, Object>();

        testQVariant("String boolean (true)", variant, expectedDouble, true, expectedString, true, expectedByteArray, true, expectedInt, true, expectedBool, true,
                expectedBitArray, false, expectedChar, false, expectedDate, true, expectedTime, true, expectedDateTime, true, expectedPoint, false, expectedPointF, false, expectedRect, false,
                expectedRectF, false, expectedRegExp, false, expectedSize, false, expectedSizeF, false, expectedList, false, expectedMap, false);
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
        QRegExp expectedRegExp = new QRegExp();
        QSize expectedSize = new QSize();
        QSizeF expectedSizeF = new QSizeF();
        List<Object> expectedList = new ArrayList<Object>();
        Map<String, Object> expectedMap = new HashMap<String, Object>();

        testQVariant("Double", variant, expectedDouble, true, expectedString, true, expectedByteArray, true, expectedInt, true, expectedBool, true, expectedBitArray, false,
                expectedChar, false, expectedDate, false, expectedTime, false, expectedDateTime, false, expectedPoint, false, expectedPointF, false, expectedRect, false, expectedRectF, false,
                expectedRegExp, false, expectedSize, false, expectedSizeF, false, expectedList, false, expectedMap, false);
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
        QRegExp expectedRegExp = new QRegExp();
        QSize expectedSize = new QSize();
        QSizeF expectedSizeF = new QSizeF();
        List<Object> expectedList = new ArrayList<Object>();
        Map<String, Object> expectedMap = new HashMap<String, Object>();

        testQVariant("Null", variant, expectedDouble, false, expectedString, true, expectedByteArray, true, expectedInt, false, expectedBool, false, expectedBitArray, false,
                expectedChar, false, expectedDate, false, expectedTime, false, expectedDateTime, false, expectedPoint, false, expectedPointF, false, expectedRect, false, expectedRectF, false,
                expectedRegExp, false, expectedSize, false, expectedSizeF, false, expectedList, false, expectedMap, false);

    }

    public void testQVariant(String name, Object object, Double expectedDouble, Boolean canConvertDouble, String expectedString, Boolean canConvertString, QByteArray expectedByteArray,
            Boolean canConvertByteArray, Integer expectedInt, Boolean canConvertInt, Boolean expectedBool, Boolean canConvertBool, QBitArray expectedBitArray, Boolean canConvertBitArray,
            Character expectedChar, Boolean canConvertChar, QDate expectedDate, Boolean canConvertDate, QTime expectedTime, Boolean canConvertTime, QDateTime expectedDateTime,
            Boolean canConvertDateTime, QPoint expectedPoint, Boolean canConvertPoint, QPointF expectedPointF, Boolean canConvertPointF, QRect expectedRect, Boolean canConvertRect,
            QRectF expectedRectF, Boolean canConvertRectF, QRegExp expectedRegExp, Boolean canConvertRegExp, QSize expectedSize, Boolean canConvertSize, QSizeF expectedSizeF, Boolean canConvertSizeF,
            List<?> expectedList, Boolean canConvertList, Map<?, ?> expectedMap, Boolean canConvertMap) {

        assertEquals(expectedString, QVariant.toString(object));
        assertEquals((double)expectedDouble, (double)QVariant.toDouble(object), 0.0);
        assertEquals((double)expectedInt, (int)QVariant.toInt(object), 0.0);
        assertEquals(expectedBool, QVariant.toBoolean(object));
        assertTrue(QVariant.toBitArray(object).equals(expectedBitArray));
        assertEquals((char)QVariant.toChar(object), (char)expectedChar);
        assertTrue(QVariant.toDate(object).equals(expectedDate));
        assertTrue(QVariant.toTime(object).equals(expectedTime));
        assertTrue(QVariant.toDateTime(object).equals(expectedDateTime));
        QVariant.toDateTime(object);
        assertEquals(QVariant.toPoint(object).x(), expectedPoint.x());
        assertEquals(QVariant.toPoint(object).y(), expectedPoint.y());
        assertEquals(QVariant.toPointF(object).x(), expectedPointF.x(), 0.0);
        assertEquals(QVariant.toPointF(object).y(), expectedPointF.y(), 0.0);
        assertEquals(QVariant.toRect(object).left(), expectedRect.left());
        assertEquals(QVariant.toRect(object).top(), expectedRect.top());
        assertEquals(QVariant.toRect(object).right(), expectedRect.right());
        assertEquals(QVariant.toRect(object).bottom(), expectedRect.bottom());
        assertEquals(QVariant.toRectF(object).left(), expectedRectF.left(), 0.0);
        assertEquals(QVariant.toRectF(object).top(), expectedRectF.top(), 0.0);
        assertEquals(QVariant.toRectF(object).bottom(), expectedRectF.bottom(), 0.0);
        assertEquals(QVariant.toRectF(object).right(), expectedRectF.right(), 0.0);
        assertTrue(QVariant.toRegExp(object).equals(expectedRegExp));
        assertEquals(QVariant.toSize(object).width(), expectedSize.width());
        assertEquals(QVariant.toSize(object).height(), expectedSize.height());
        assertEquals(QVariant.toSizeF(object).width(), expectedSizeF.width(), 0.0);
        assertEquals(QVariant.toSizeF(object).height(), expectedSizeF.height(), 0.0);
        assertEquals(QVariant.toList(object), expectedList);
        assertEquals(QVariant.toMap(object), expectedMap);
        {
            QByteArray ba = QVariant.toByteArray(object);
            assertEquals(expectedByteArray.size(), ba.size());
            for (int i = 0; i < ba.size(); ++i) {
                assertEquals(expectedByteArray.at(i), ba.at(i));
            }
        }

        assertEquals(QVariant.canConvertToString(object), canConvertString);
        assertEquals(QVariant.canConvertToDouble(object), canConvertDouble);
        assertEquals(QVariant.canConvertToByteArray(object), canConvertByteArray);
        assertEquals(QVariant.canConvertToInt(object), canConvertInt);
        assertEquals(QVariant.canConvertToBoolean(object), canConvertBool);
        assertEquals(QVariant.canConvertToBitArray(object), canConvertBitArray);
        assertEquals(QVariant.canConvertToChar(object), canConvertChar);
        assertEquals(QVariant.canConvertToDate(object), canConvertDate);
        assertEquals(QVariant.canConvertToTime(object), canConvertTime);
        assertEquals(QVariant.canConvertToDateTime(object), canConvertDateTime);
        assertEquals(QVariant.canConvertToPoint(object), canConvertPoint);
        assertEquals(QVariant.canConvertToPointF(object), canConvertPointF);
        assertEquals(QVariant.canConvertToRect(object), canConvertRect);
        assertEquals(QVariant.canConvertToRectF(object), canConvertRectF);
        assertEquals(QVariant.canConvertToRegExp(object), canConvertRegExp);
        assertEquals(QVariant.canConvertToSize(object), canConvertSize);
        assertEquals(QVariant.canConvertToSizeF(object), canConvertSizeF);
        assertEquals(QVariant.canConvertToList(object), canConvertList);
        assertEquals(QVariant.canConvertToMap(object), canConvertMap);
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
        assertEquals(flags.toString(), v.currentToString());
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
	    	Map<String,Object> sentArguments = new TreeMap<>();
	    	sentArguments.put("test1", Arrays.asList(new QPair<>("file1", new QFileInfo(QDir.currentPath())), new QPair<>("file2", new QFileInfo(QDir.rootPath()))));
	    	sentArguments.put("test2", new QPair<>("file", Arrays.asList(new QFileInfo(QDir.rootPath()))));
	    	sentArguments.put("test3", Arrays.asList("STRG", 9, 4.5));
	    	sentArguments.put("test4", new LinkedList<>(Arrays.asList(new QColor(Qt.GlobalColor.red), new QColor(Qt.GlobalColor.green), new QColor(Qt.GlobalColor.blue))));
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
	
	        Map<String,Object> receivedArguments = new TreeMap<>();
	    	for(QMetaMethod m : object.metaObject().methods()) {
	    		System.out.println(m);
	    		if(m.methodType()==MethodType.Signal && m.parameterTypes().size()==1) {
	    			QMetaObject.AbstractPrivateSignal1<?> signal = (QMetaObject.AbstractPrivateSignal1<?>)m.toSignal(object);
	    			String name = m.name();
	    			signal.connect(p->receivedArguments.put(name, p));    			
	    		}
	    		if(m.name().startsWith("_") && sentArguments.containsKey(m.name().substring(1))) {
	    			m.invoke(object, sentArguments.get(m.name().substring(1)));
	    		}
	    	}
	    	if(sentArguments.containsKey("test1")) {
	    		assertEquals(sentArguments.get("test1"), receivedArguments.get("test1"));
	    	}
	    	
	    	if(sentArguments.containsKey("test2")) {
		    	assertTrue(receivedArguments.get("test2")!=null);
		    	assertEquals(QPair.class, receivedArguments.get("test2").getClass());
		    	assertEquals(((QPair<?,?>)sentArguments.get("test2")).first, ((QPair<?,?>)receivedArguments.get("test2")).first);
		    	assertEquals(ArrayDeque.class, ((QPair<?,?>)receivedArguments.get("test2")).second.getClass());
		    	assertTrue(Arrays.equals(((Collection<?>)((QPair<?,?>)sentArguments.get("test2")).second).toArray(), ((Collection<?>)((QPair<?,?>)receivedArguments.get("test2")).second).toArray()));
	    	}
	    	
	    	if(sentArguments.containsKey("test3")) {
		    	assertTrue(receivedArguments.get("test3")!=null);
		    	assertEquals(ArrayDeque.class, receivedArguments.get("test3").getClass());
		    	assertTrue(Arrays.equals(((Collection<?>)sentArguments.get("test3")).toArray(), ((Collection<?>)receivedArguments.get("test3")).toArray()));
	    	}
	    	if(sentArguments.containsKey("test4")) {
		    	assertEquals(sentArguments.get("test4"), receivedArguments.get("test4"));
		    	assertEquals(sentArguments.get("test5"), receivedArguments.get("test5"));
		    	assertEquals(sentArguments.get("test6"), receivedArguments.get("test6"));
		    	assertEquals(sentArguments.get("test7"), receivedArguments.get("test7"));
	    	}
	    	if(sentArguments.containsKey("test8")) {
		    	assertTrue(receivedArguments.get("test8")!=null);
		    	assertEquals(ArrayDeque.class, receivedArguments.get("test8").getClass());
		    	assertTrue(Arrays.equals(((Collection<?>)sentArguments.get("test8")).toArray(), ((Collection<?>)receivedArguments.get("test8")).toArray()));
	    	}
	    	if(sentArguments.containsKey("test9")) {
		    	assertTrue(receivedArguments.get("test9")!=null);
		    	assertEquals(ArrayList.class, receivedArguments.get("test9").getClass());
		    	assertEquals(((List<?>)sentArguments.get("test9")).size(), ((List<?>)receivedArguments.get("test9")).size());
		    	assertEquals(ArrayDeque.class, ((List<?>)receivedArguments.get("test9")).get(0).getClass());
		    	assertTrue(Arrays.equals(((Collection<?>)((List<?>)sentArguments.get("test9")).get(0)).toArray(), ((Collection<?>)((List<?>)receivedArguments.get("test9")).get(0)).toArray()));
		    	assertEquals(ArrayDeque.class, ((List<?>)receivedArguments.get("test9")).get(1).getClass());
		    	assertTrue(Arrays.equals(((Collection<?>)((List<?>)sentArguments.get("test9")).get(1)).toArray(), ((Collection<?>)((List<?>)receivedArguments.get("test9")).get(1)).toArray()));
	    	}
	    	if(sentArguments.containsKey("test10")) {
	    		assertEquals(sentArguments.get("test10"), receivedArguments.get("test10"));
	    	}
	    	if(sentArguments.containsKey("test11")) {
		    	assertTrue(receivedArguments.get("test11")!=null);
		    	assertEquals(ArrayList.class, receivedArguments.get("test11").getClass());
		    	assertEquals(((List<?>)sentArguments.get("test11")).size(), ((List<?>)receivedArguments.get("test11")).size());
		    	assertEquals(ArrayDeque.class, ((List<?>)receivedArguments.get("test11")).get(0).getClass());
		    	assertTrue(Arrays.equals(((Collection<?>)((List<?>)sentArguments.get("test11")).get(0)).toArray(), ((Collection<?>)((List<?>)receivedArguments.get("test11")).get(0)).toArray()));
		    	assertEquals(ArrayDeque.class, ((List<?>)receivedArguments.get("test11")).get(1).getClass());
		    	assertTrue(Arrays.equals(((Collection<?>)((List<?>)sentArguments.get("test11")).get(1)).toArray(), ((Collection<?>)((List<?>)receivedArguments.get("test11")).get(1)).toArray()));
	    	}
	    	if(sentArguments.containsKey("test12")) {
		    	assertTrue(receivedArguments.get("test12")!=null);
		    	assertEquals(TreeMap.class, receivedArguments.get("test12").getClass());
		    	assertEquals(((Map<?,?>)sentArguments.get("test12")).size(), ((Map<?,?>)receivedArguments.get("test12")).size());
		    	for(Object key : ((Map<?,?>)sentArguments.get("test12")).keySet()) {
			    	assertTrue(((Map<?,?>)receivedArguments.get("test12")).containsKey(key));
			    	assertEquals(ArrayDeque.class, ((Map<?,?>)receivedArguments.get("test12")).get(key).getClass());
			    	assertTrue(Arrays.equals(((Collection<?>)((Map<?,?>)sentArguments.get("test12")).get(key)).toArray(), ((Collection<?>)((Map<?,?>)receivedArguments.get("test12")).get(key)).toArray()));
		    	}
	    	}
	    	if(sentArguments.containsKey("test13")) {
		    	assertTrue(receivedArguments.get("test13")!=null);
		    	assertEquals(HashMap.class, receivedArguments.get("test13").getClass());
		    	assertEquals(((Map<?,?>)sentArguments.get("test13")).size(), ((Map<?,?>)receivedArguments.get("test13")).size());
		    	for(Object key : ((Map<?,?>)sentArguments.get("test13")).keySet()) {
			    	assertTrue(((Map<?,?>)receivedArguments.get("test13")).containsKey(key));
			    	assertEquals(ArrayDeque.class, ((Map<?,?>)receivedArguments.get("test13")).get(key).getClass());
			    	assertTrue(Arrays.equals(((Collection<?>)((Map<?,?>)sentArguments.get("test13")).get(key)).toArray(), ((Collection<?>)((Map<?,?>)receivedArguments.get("test13")).get(key)).toArray()));
		    	}
	    	}
	    	if(sentArguments.containsKey("test14")) {
		    	assertTrue(receivedArguments.get("test14")!=null);
		    	assertEquals(HashMap.class, receivedArguments.get("test14").getClass());
		    	assertEquals(((Map<?,?>)sentArguments.get("test14")).size(), ((Map<?,?>)receivedArguments.get("test14")).size());
		    	for(Object key : ((Map<?,?>)sentArguments.get("test14")).keySet()) {
		    		assertTrue(key instanceof QUrl.FormattingOptions);
			    	assertTrue(((Map<?,?>)receivedArguments.get("test14")).containsKey(key));
			    	Object sentValue = ((Map<?,?>)sentArguments.get("test14")).get(key);
			    	Object receivedValue = ((Map<?,?>)receivedArguments.get("test14")).get(key);
			    	assertTrue(receivedValue instanceof Collection<?>);
			    	assertEquals(ArrayDeque.class, ((List<?>)receivedValue).get(0).getClass());
			    	assertTrue(Arrays.equals(((Collection<?>)((List<?>)sentValue).get(0)).toArray(), ((Collection<?>)((List<?>)receivedValue).get(0)).toArray()));
			    	assertEquals(ArrayDeque.class, ((List<?>)receivedValue).get(1).getClass());
			    	assertTrue(Arrays.equals(((Collection<?>)((List<?>)sentValue).get(1)).toArray(), ((Collection<?>)((List<?>)receivedValue).get(1)).toArray()));
		    	}
	    	}
	    	if(sentArguments.containsKey("test15")) {
		    	assertTrue(receivedArguments.get("test15")!=null);
		    	assertEquals(ArrayDeque.class, receivedArguments.get("test15").getClass());
		    	assertEquals(((Deque<?>)sentArguments.get("test15")).size(), ((Deque<?>)receivedArguments.get("test15")).size());
		    	assertEquals(ArrayDeque.class, ((Deque<?>)receivedArguments.get("test15")).getFirst().getClass());
		    	assertTrue(Arrays.equals(((Collection<?>)((Deque<?>)sentArguments.get("test15")).getFirst()).toArray(), ((Collection<?>)((Deque<?>)receivedArguments.get("test15")).getFirst()).toArray()));
		    	((Deque<?>)receivedArguments.get("test15")).pop();
		    	((Deque<?>)sentArguments.get("test15")).pop();
		    	assertEquals(ArrayDeque.class, ((Deque<?>)receivedArguments.get("test15")).getFirst().getClass());
		    	assertTrue(Arrays.equals(((Collection<?>)((Deque<?>)sentArguments.get("test15")).getFirst()).toArray(), ((Collection<?>)((Deque<?>)receivedArguments.get("test15")).getFirst()).toArray()));
	    	}
    	}finally {
    		object.dispose();
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

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestQVariant.class.getName());
    }
}
