/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
import static org.junit.Assert.assertNotEquals;
import static org.junit.Assert.assertTrue;

import java.util.Map;
import java.util.TreeMap;

import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.core.QCborMap;
import io.qt.core.QCborValue;
import io.qt.core.QHash;
import io.qt.core.QJsonObject;
import io.qt.core.QJsonValue;
import io.qt.core.QList;
import io.qt.core.QMap;
import io.qt.core.QMetaType;
import io.qt.core.QPair;

public class TestIteratorsQt610 extends ApplicationInitializer {
	@BeforeClass
    public static void testInitialize() throws Exception {
    	ApplicationInitializer.testInitializeWithWidgets();
    }
    
    @Test
    public void test_QJsonObject() {
    	QJsonObject container = new QJsonObject();
    	assertTrue(container.begin() instanceof QJsonObject.Iterator);
    	assertTrue(container.constBegin() instanceof QJsonObject.ConstIterator);
    	assertTrue(container.keyValueBegin() instanceof QJsonObject.KeyValueIterator);
    	assertTrue(container.constKeyValueBegin() instanceof QJsonObject.ConstKeyValueIterator);
		assertEquals(QMetaType.fromType(String.class), container.begin().keyMetaType());
		assertEquals(QMetaType.fromType(QJsonValue.class), container.begin().valueMetaType());
		assertEquals(QMetaType.fromType(String.class), container.constBegin().keyMetaType());
		assertEquals(QMetaType.fromType(QJsonValue.class), container.constBegin().valueMetaType());
		assertEquals(QMetaType.fromType(QPair.class, QMetaType.fromType(String.class), QMetaType.fromType(QJsonValue.class)), container.keyValueBegin().elementMetaType());
		assertEquals(QMetaType.fromType(QPair.class, QMetaType.fromType(String.class), QMetaType.fromType(QJsonValue.class)), container.constKeyValueBegin().elementMetaType());
    	container.insert("A", 1);
    	container.insert("B", 2);
    	container.insert("C", 3);
		assertEquals(QHash.of("A", new QJsonValue(1), "B", new QJsonValue(2), "C", new QJsonValue(3)), new QHash<>(container.constBegin(), container.constEnd()));
		assertEquals(QHash.of("A", new QJsonValue(1), "B", new QJsonValue(2), "C", new QJsonValue(3)), new QHash<>(container.begin(), container.end()));
		
		assertTrue(container.constKeyValueBegin().isValid());
		assertTrue(container.keyValueBegin().isValid());
		assertEquals(container.keyValueBegin(), container.constKeyValueBegin());
		assertEquals(container.constKeyValueBegin(), container.keyValueBegin());

    	QJsonObject.Iterator miter1 = container.begin();
    	QJsonObject clone = container.clone();
    	
		QJsonObject.ConstIterator iter1 = container.constBegin();
		assertEquals(iter1, miter1);
		assertEquals(miter1, iter1);
		assertTrue(iter1.isValid());
		iter1.advance();
		assertTrue(iter1.isValid());
		QJsonObject.ConstIterator iter2 = clone.constBegin();
		iter2.advance();
		assertNotEquals(iter1, iter2);
		assertEquals(iter1.key(), iter2.key());
		assertEquals(iter1.value().toVariant(), iter2.value().toVariant());
		QJsonObject test = new QJsonObject();
		for(QJsonObject.ConstIterator iter = container.constBegin(), end = container.constEnd(); !iter.equals(end); iter.advance()) {
			test.insert(iter.key(), iter.value().toInt());
		}
		assertEquals(test, container);
		assertTrue(miter1.isValid());
		clone.dispose();
		assertTrue(miter1.isValid());
		container = new QJsonObject();
    	container.insert("A", 1);
    	container.insert("B", 2);
    	container.insert("C", 3);
		miter1 = container.begin();
		assertTrue(miter1.isValid());
		container.dispose();
		assertFalse(miter1.isValid());
		assertTrue(miter1.isDisposed());
		
		container = new QJsonObject();
    	container.insert("A", 1);
    	container.insert("B", 2);
    	container.insert("C", 3);
		miter1 = container.begin();
		miter1.setValue(new QJsonValue(5));
		iter1 = container.constBegin();
		assertEquals(5, iter1.value().toInt());
		
		QList<QJsonValue> lv = QList.ofValues(container.constBegin(), container.constEnd());
		QList<String> lk = QList.ofKeys(container.constBegin(), container.constEnd());
		QList<QPair<String, QJsonValue>> lkv = QList.ofKeyValuePairs(container.constBegin(), container.constEnd());
		assertEquals(QMetaType.fromType(QJsonValue.class), lv.elementMetaType());
		assertEquals(QMetaType.fromType(String.class), lk.elementMetaType());
		assertEquals(QMetaType.fromType(QPair.class, QMetaType.fromType(String.class), QMetaType.fromType(QJsonValue.class)), lkv.elementMetaType());
		assertEquals(QList.of(new QJsonValue(5), new QJsonValue(2), new QJsonValue(3)), lv);
		assertEquals(QList.of("A", "B", "C"), lk);
		assertEquals(QList.of(new QPair<>("A", new QJsonValue(5)), new QPair<>("B", new QJsonValue(2)), new QPair<>("C", new QJsonValue(3))), lkv);

		container.swap(new QJsonObject());
		assertFalse("mutable iterator not invalid after concurrent mutation", miter1.isValid());
		assertFalse("const iterator not invalid after concurrent mutation", iter1.isValid());
		
		container = new QJsonObject();
    	container.insert("A", 1);
    	container.insert("B", 2);
    	container.insert("C", 3);
		assertEquals("A", container.begin().key());
		assertEquals("A", container.constBegin().key());
		assertEquals(new QJsonValue(1), container.begin().get());
		assertEquals(new QJsonValue(1), container.constBegin().get());
		assertEquals(new QJsonValue(1), new QJsonObject.ConstIterator(container.begin()).get());
    	QJsonObject.Iterator erasedIter = container.erase(container.begin());
		assertTrue(erasedIter.isValid());
		assertEquals("B", erasedIter.key());
		assertEquals(new QJsonValue(2), erasedIter.value());
		
		assertEquals(new QJsonValue(2), new QJsonObject.ConstIterator(container.cbegin()).get());
		assertEquals(new QPair<>("B", new QJsonValue(2)), new QJsonObject.ConstKeyValueIterator(container.constKeyValueBegin()).get());
		assertEquals(new QJsonValue(2), new QJsonObject.ConstIterator(container.begin()).get());
		assertEquals(new QPair<>("B", new QJsonValue(2)), new QJsonObject.ConstKeyValueIterator(container.keyValueBegin()).get());
		assertEquals(new QPair<>("B", new QJsonValue(2)), new QJsonObject.ConstKeyValueIterator(container.constBegin()).get());
		assertEquals(new QPair<>("B", new QJsonValue(2)), new QJsonObject.ConstKeyValueIterator(container.begin()).get());
		assertEquals(new QPair<>("B", new QJsonValue(2)), new QJsonObject.KeyValueIterator(container.begin()).get());
		QJsonObject.Iterator begin = container.begin();
		QJsonObject.KeyValueIterator kvbegin = container.keyValueBegin();
		QJsonObject.ConstIterator cbegin = container.cbegin();
		QJsonObject.ConstKeyValueIterator ckvbegin = container.constKeyValueBegin();
		assertEquals(begin, cbegin);
		assertEquals(begin, kvbegin);
		assertEquals(begin, ckvbegin);
		assertEquals(cbegin, begin);
		assertEquals(cbegin, kvbegin);
		assertEquals(cbegin, ckvbegin);
		assertEquals(kvbegin, ckvbegin);
		assertEquals(kvbegin, cbegin);
		assertEquals(kvbegin, begin);
		assertEquals(ckvbegin, kvbegin);
		assertEquals(ckvbegin, cbegin);
		assertEquals(ckvbegin, begin);
		
		container.keyValueBegin().set(new QJsonValue(99));
		assertEquals(new QJsonValue(99), container.constBegin().value());
    }
	
	@Test
    public void test_QCborMap() {
    	QCborMap container = QCborMap.fromVariantMap(QMap.of("A", 1, "B", 2));
    	assertTrue(container.begin() instanceof QCborMap.Iterator);
    	assertTrue(container.constBegin() instanceof QCborMap.ConstIterator);
    	assertTrue(container.keyValueBegin() instanceof QCborMap.KeyValueIterator);
    	assertTrue(container.constKeyValueBegin() instanceof QCborMap.ConstKeyValueIterator);
		assertEquals(QMetaType.fromType(QCborValue.class), container.begin().keyMetaType());
		assertEquals(QMetaType.fromType(QCborValue.class), container.begin().valueMetaType());
		assertEquals(QMetaType.fromType(QCborValue.class), container.constBegin().keyMetaType());
		assertEquals(QMetaType.fromType(QCborValue.class), container.constBegin().valueMetaType());
		assertEquals(QMetaType.fromType(QPair.class, QMetaType.fromType(QCborValue.class), QMetaType.fromType(QCborValue.class)), container.keyValueBegin().elementMetaType());
		assertEquals(QMetaType.fromType(QPair.class, QMetaType.fromType(QCborValue.class), QMetaType.fromType(QCborValue.class)), container.constKeyValueBegin().elementMetaType());
		assertEquals(QHash.of(new QCborValue("A"), new QCborValue(1), new QCborValue("B"), new QCborValue(2)), new QHash<>(container.constBegin(), container.constEnd()));
		assertEquals(QHash.of(new QCborValue("A"), new QCborValue(1), new QCborValue("B"), new QCborValue(2)), new QHash<>(container.begin(), container.end()));
		
		assertTrue(container.constKeyValueBegin().isValid());
		assertTrue(container.keyValueBegin().isValid());
		assertEquals(container.keyValueBegin(), container.constKeyValueBegin());

    	QCborMap.Iterator miter1 = container.begin();
    	QCborMap clone = container.clone();
    	
		QCborMap.ConstIterator iter1 = container.constBegin();
		assertEquals(iter1, miter1);
		assertEquals(miter1, iter1);
		iter1.advance();
		QCborMap.ConstIterator iter2 = clone.constBegin();
		iter2.advance();
		assertEquals(iter1, iter2);
		Map<String,Object> test = new TreeMap<>();
		for(QCborMap.ConstIterator iter = container.constBegin(), end = container.constEnd(); !iter.equals(end); iter.advance()) {
			test.put(iter.key().toString(), iter.value().toVariant());
		}
		assertEquals(QCborMap.fromVariantMap(test), container);
		assertTrue(miter1.isValid());
		clone.dispose();
		assertTrue(miter1.isValid());
		container.clear();
		assertFalse(miter1.isValid());
		container = QCborMap.fromVariantMap(QMap.of("A", 1, "B", 2));
		miter1 = container.begin();
		assertTrue(miter1.isValid());
		container.dispose();
		assertFalse(miter1.isValid());
		assertTrue(miter1.isDisposed());
		
		container = QCborMap.fromVariantMap(QMap.of("A", 1, "B", 2));
		miter1 = container.begin();
		miter1.setValue(new QCborValue(5));
		iter1 = container.constBegin();
		assertEquals(5l, iter1.value().toInteger());
		miter1.setValue(new QCborValue(8));
		assertEquals(8l, container.extract(iter1).toInteger());
		assertEquals(2l, container.extract(container.find("B")).toInteger());
		
		container = QCborMap.fromVariantMap(QMap.of("A", 1, "B", 2));
		QList<QCborValue> lv = QList.ofValues(container.constBegin(), container.constEnd());
		QList<QCborValue> lk = QList.ofKeys(container.constBegin(), container.constEnd());
		QList<QPair<QCborValue, QCborValue>> lkv = QList.ofKeyValuePairs(container.constBegin(), container.constEnd());
		assertEquals(QMetaType.fromType(QCborValue.class), lv.elementMetaType());
		assertEquals(QMetaType.fromType(QCborValue.class), lk.elementMetaType());
		assertEquals(QMetaType.fromType(QPair.class, QMetaType.fromType(QCborValue.class), QMetaType.fromType(QCborValue.class)), lkv.elementMetaType());
		assertEquals(QList.of(new QCborValue(1), new QCborValue(2)), lv);
		assertEquals(QList.of(new QCborValue("A"), new QCborValue("B")), lk);
		assertEquals(QList.of(new QPair<>(new QCborValue("A"), new QCborValue(1)), new QPair<>(new QCborValue("B"), new QCborValue(2))), lkv);
		
		container.swap(new QCborMap());
		assertFalse("mutable iterator not invalid after concurrent mutation", miter1.isValid());
		assertTrue("const iterator invalid after concurrent mutation", iter1.isValid());
		
		container = QCborMap.fromVariantMap(QMap.of("A", 1, "B", 2));
		QCborMap.Iterator erasedIter = container.erase(container.begin());
		assertTrue(erasedIter.isValid());
		assertEquals(new QCborValue("B"), erasedIter.key());
		assertEquals(new QCborValue(2), erasedIter.value());
		
		container = QCborMap.fromVariantMap(QMap.of("A", 1, "B", 2));
		assertEquals(new QPair<>(new QCborValue("A"), new QCborValue(1)), new QCborMap.ConstIterator(container.begin()).get());
		assertEquals(new QPair<>(new QCborValue("A"), new QCborValue(1)), container.constBegin().get());
		assertEquals(new QPair<>(new QCborValue("A"), new QCborValue(1)), container.begin().get());
		erasedIter = container.erase(container.constBegin());
		assertTrue(erasedIter.isValid());
		assertEquals(new QCborValue("B"), erasedIter.key());
		assertEquals(new QCborValue(2), erasedIter.value());
		
		assertEquals(new QPair<>(new QCborValue("B"), new QCborValue(2)), new QCborMap.ConstIterator(container.cbegin()).get());
		assertEquals(new QPair<>(new QCborValue("B"), new QCborValue(2)), new QCborMap.ConstKeyValueIterator(container.constKeyValueBegin()).get());
		assertEquals(new QPair<>(new QCborValue("B"), new QCborValue(2)), new QCborMap.ConstIterator(container.begin()).get());
		assertEquals(new QPair<>(new QCborValue("B"), new QCborValue(2)), new QCborMap.ConstKeyValueIterator(container.keyValueBegin()).get());
		assertEquals(new QPair<>(new QCborValue("B"), new QCborValue(2)), new QCborMap.ConstKeyValueIterator(container.begin()).get());
		assertEquals(new QPair<>(new QCborValue("B"), new QCborValue(2)), new QCborMap.ConstKeyValueIterator(container.constBegin()).get());
		assertEquals(new QPair<>(new QCborValue("B"), new QCborValue(2)), new QCborMap.KeyValueIterator(container.begin()).get());
		QCborMap.Iterator begin = container.begin();
		QCborMap.KeyValueIterator kvbegin = container.keyValueBegin();
		QCborMap.ConstIterator cbegin = container.cbegin();
		QCborMap.ConstKeyValueIterator ckvbegin = container.constKeyValueBegin();
		assertEquals(begin, cbegin);
		assertEquals(begin, kvbegin);
		assertEquals(begin, ckvbegin);
		assertEquals(cbegin, begin);
		assertEquals(cbegin, kvbegin);
		assertEquals(cbegin, ckvbegin);
		assertEquals(kvbegin, ckvbegin);
		assertEquals(kvbegin, cbegin);
		assertEquals(kvbegin, begin);
		assertEquals(ckvbegin, kvbegin);
		assertEquals(ckvbegin, cbegin);
		assertEquals(ckvbegin, begin);

		container.keyValueBegin().set(new QCborValue(99));
		assertEquals(new QCborValue(99), container.constBegin().value());
    }
}
