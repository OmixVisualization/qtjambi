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

import static org.junit.Assert.*;

import java.io.*;
import java.nio.IntBuffer;
import java.util.*;
import java.util.function.Consumer;

import org.junit.*;

import io.qt.*;
import io.qt.autotests.generated.*;
import io.qt.core.*;
import io.qt.internal.*;
import io.qt.widgets.*;
import io.qt.gui.*;

public class TestIterators extends ApplicationInitializer {
	
	@BeforeClass
    public static void testInitialize() throws Exception {
    	ApplicationInitializer.testInitializeWithWidgets();
    }
	
	@Test
    public void test_QTreeWidgetItemIterator() {
		QTreeWidget widget = new QTreeWidget();
    	try {
	    	widget.addTopLevelItem(new QTreeWidgetItem(Arrays.asList("A")));
	    	widget.addTopLevelItem(new QTreeWidgetItem(Arrays.asList("B")));
	    	widget.addTopLevelItem(new QTreeWidgetItem(Arrays.asList("C")));
	    	widget.addTopLevelItem(new QTreeWidgetItem(Arrays.asList("D")));
	    	widget.addTopLevelItem(new QTreeWidgetItem(Arrays.asList("E")));
	    	List<String> texts = new ArrayList<>();
	    	for(QTreeWidgetItem item : widget) {
	    		Assert.assertTrue(item!=null);
	    		texts.add(item.text(0));
	    	}
	    	assertEquals(Arrays.asList("A", "B", "C", "D", "E"), texts);
    	}finally {
    		widget.dispose();
    	}
    	widget = new QTreeWidget();
    	try {
	    	QTreeWidgetItemIterator iterator = new QTreeWidgetItem(widget).iterator();
	    	assertTrue(General.internalAccess.hasOwnerFunction(iterator));
	    	assertEquals(QApplication.instance(), General.internalAccess.owner(iterator));
    	}finally {
    		widget.dispose();
    	}
    	widget = new QTreeWidget();
    	try {
	    	widget.addTopLevelItem(new QTreeWidgetItem(Arrays.asList("A")));
	    	widget.addTopLevelItem(new QTreeWidgetItem(Arrays.asList("B")));
	    	widget.addTopLevelItem(new QTreeWidgetItem(Arrays.asList("C")));
	    	widget.addTopLevelItem(new QTreeWidgetItem(Arrays.asList("D")));
	    	widget.addTopLevelItem(new QTreeWidgetItem(Arrays.asList("E")));
	    	QTreeWidgetItemIterator iterator = widget.iterator();
	    	widget.dispose();
	    	try {
				iterator.hasNext();
				Assert.assertFalse("QNoNativeResourcesException expected to be thrown.", true);
			} catch (QNoNativeResourcesException e) {
			}
    	}finally {
    		widget.dispose();
    	}
    	try {
    		QTreeWidgetItem item = new QTreeWidgetItem();
    		new QTreeWidgetItemIterator(item);
    		fail("IllegalArgumentException expected to be thrown because QTreeWidgetItem has no widget");
    	}catch(IllegalArgumentException e) {
    	}
    	try {
    		new QTreeWidgetItemIterator((QTreeWidgetItem)null);
    		fail("NullPointerException expected to be thrown because QTreeWidgetItem is null");
    	}catch(NullPointerException e) {
    	}
    	try {
    		new QTreeWidgetItemIterator((QTreeWidget)null);
    		fail("NullPointerException expected to be thrown because QTreeWidget is null");
    	}catch(NullPointerException e) {
    	}
	}
	
    @Test
    public void test_QDirIterator() {
    	String uniqueDirectory = "QtJambi_QDirTest_"+TestUtility.processName();
    	QDir userDir = new QDir(System.getProperty("user.dir", ""));
    	try {
    		if(!userDir.mkdir(uniqueDirectory) && !userDir.exists(uniqueDirectory)) {
    			userDir = QDir.temp();
    	    	Assert.assertTrue(userDir.mkdir(uniqueDirectory));
    		}
	    	Assert.assertTrue(userDir.cd(uniqueDirectory));
	    	Assert.assertTrue(userDir.mkdir("A"));
	    	Assert.assertTrue(userDir.mkdir("B"));
	    	Assert.assertTrue(userDir.mkdir("C"));
	    	Assert.assertTrue(userDir.mkdir("D"));
	    	Assert.assertTrue(userDir.mkdir("E"));
	    	Set<String> subDirs = new HashSet<>();
	    	for(String subdir : new QDirIterator(userDir)) {
    			int idx = subdir.lastIndexOf('/');
    			subDirs.add(subdir.substring(idx+1));
	    	}
	    	Assert.assertEquals(new HashSet<>(Arrays.asList(".", "..", "A", "B", "C", "D", "E")), subDirs);
	    	new QList<>(new QDirIterator(userDir));
    	}finally {
    		userDir.assign(System.getProperty("user.dir", ""));
    		if(userDir.exists(uniqueDirectory)) {
        		userDir.cd(uniqueDirectory);
        		userDir.removeRecursively();
        	}
    		userDir = QDir.temp();
    		if(userDir.exists(uniqueDirectory)) {
        		userDir.cd(uniqueDirectory);
        		userDir.removeRecursively();
        	}
		}
    }
    
    @Test
    public void test_QRegularExpressionMatchIterator() {
    	QRegularExpression expression = new QRegularExpression("^(aa)");
    	List<QRegularExpressionMatch> matches = new ArrayList<>();
    	for(QRegularExpressionMatch match : expression.globalMatch("aa aa aa bb cc aa", 0, QRegularExpression.MatchType.PartialPreferCompleteMatch)) {
    		matches.add(match);
    	}
    	new QList<>(expression.globalMatch("aa aa aa bb cc aa", 0, QRegularExpression.MatchType.PartialPreferCompleteMatch));
    }
    
    @Test
    public void test_QList_template() {
    	QList<String> container = QList.of("A", "B");
    	assertTrue(container.begin() instanceof QList.Iterator);
    	assertTrue(container.constBegin() instanceof QList.ConstIterator);
    	assertTrue(container.reverseBegin() instanceof QList.ReverseIterator);
    	assertTrue(container.constReverseBegin() instanceof QList.ConstReverseIterator);
    	assertEquals(QMetaType.fromType(String.class), container.elementMetaType());
		assertEquals(QMetaType.fromType(String.class), container.begin().elementMetaType());
		assertEquals(QMetaType.fromType(String.class), container.constBegin().elementMetaType());
		assertEquals(QMetaType.fromType(String.class), container.reverseBegin().elementMetaType());
		assertEquals(QMetaType.fromType(String.class), container.constReverseBegin().elementMetaType());
    	assertEquals(container, new QList<>(container.constBegin(), container.constEnd()));
    	assertEquals(container, new QList<>(container.begin(), container.end()));
    	assertEquals(QList.of("B", "A"), new QList<>(container.constReverseBegin(), container.constReverseEnd()));
		QList.Iterator<String> miter1 = container.begin();
		QList.ReverseIterator<String> riter1 = container.reverseBegin();
		QList.ConstReverseIterator<String> criter1 = container.constReverseBegin();
    	QList<String> clone = container.clone();
    	assertTrue(container.isSharedWith(clone));
    	
		QList.ConstIterator<String> iter1 = container.constBegin();
		assertEquals(iter1, miter1);
		assertEquals(miter1, iter1);
		assertEquals(riter1, criter1);
		assertEquals(criter1, riter1);
		iter1.advance();
		QList.ConstIterator<String> iter2 = clone.constBegin();
		iter2.advance();
		assertEquals(iter1, iter2);
		ArrayList<String> test = new ArrayList<>();
		for(QList.ConstIterator<String> iter = container.constBegin(), end = container.constEnd(); !iter.equals(end); iter.advance()) {
			test.add(iter.get());
		}
		assertEquals(test, container);
		assertTrue(miter1.isValid());
		clone.dispose();
		assertTrue(miter1.isValid());
		container.clear();
		assertFalse(miter1.isValid());
		container = QList.of("A", "B");
		miter1 = container.begin();
		assertTrue(miter1.isValid());
		container.dispose();
		assertFalse(miter1.isValid());
		assertTrue(miter1.isDisposed());
		
		container = QList.of("A", "B");
		miter1 = container.begin();
		miter1.set("X");
		iter1 = container.constBegin();
		assertEquals("X", iter1.get());
		riter1 = container.reverseBegin();
		riter1.set("Y");
		assertEquals("Y", riter1.get());
		container.insert(0, "C");
		assertEquals(QList.of("C", "X", "Y"), container);
		assertFalse("mutable iterator not invalid after concurrent mutation", miter1.isValid());
		assertTrue("const iterator invalid after concurrent mutation", iter1.isValid());
		
		container = QList.of("A", "B");
		assertEquals("A", new QList.ConstIterator<>(container.cbegin()).get());
		assertEquals("B", new QList.ConstReverseIterator<>(container.crbegin()).get());
		assertEquals("A", new QList.ConstIterator<>(container.begin()).get());
		assertEquals("B", new QList.ConstReverseIterator<>(container.rbegin()).get());
		QList.Iterator<String> begin = container.begin();
		QList.ReverseIterator<String> rbegin = container.rbegin();
		QList.ConstIterator<String> cbegin = container.cbegin();
		QList.ConstReverseIterator<String> crbegin = container.crbegin();
		assertEquals(begin, cbegin);
		assertEquals(cbegin, begin);
		assertEquals(rbegin, crbegin);
		assertEquals(crbegin, rbegin);
		{
			Consumer<QList.ConstReverseIterator<String>> iteratorTest = keyIterator->{
				var iter = keyIterator.iterator();
				assertTrue(iter.hasNext());
				assertEquals("B", iter.next());
				assertTrue(iter.hasNext());
				assertEquals("A", iter.next());
			};
			iteratorTest.accept(container.crbegin());
			iteratorTest.accept(new QList.ConstReverseIterator<>(container.rbegin()));
		}
		{
			Consumer<QList.ReverseIterator<String>> iteratorTest = keyIterator->{
				var iter = keyIterator.iterator();
				assertTrue(iter.hasNext());
				assertEquals("B", iter.next());
				assertTrue(iter.hasNext());
				assertEquals("A", iter.next());
			};
			iteratorTest.accept(container.rbegin());
		}
		{
			Consumer<QList.ConstReverseIterator<String>> iteratorTest = keyIterator->{
				var iter = keyIterator.descendingIterator();
				assertTrue(iter.hasNext());
				assertEquals("A", iter.next());
				assertTrue(iter.hasNext());
				assertEquals("B", iter.next());
			};
			iteratorTest.accept(container.crend());
			iteratorTest.accept(new QList.ConstReverseIterator<>(container.rend()));
		}
		{
			Consumer<QList.ReverseIterator<String>> iteratorTest = keyIterator->{
				var iter = keyIterator.descendingIterator();
				assertTrue(iter.hasNext());
				assertEquals("A", iter.next());
				assertTrue(iter.hasNext());
				assertEquals("B", iter.next());
			};
			iteratorTest.accept(container.rend());
		}
		{
			Consumer<QList.ConstReverseIterator<String>> iteratorTest = keyIterator->{
				var iter = keyIterator.bidirectionalIterator();
				assertTrue(iter.hasNext());
				assertFalse(iter.hasPrevious());
				assertEquals("B", iter.next());
				assertTrue(iter.hasNext());
				assertEquals("A", iter.next());
				assertFalse(iter.hasNext());
				assertTrue(iter.hasPrevious());
				assertEquals("A", iter.previous());
				try {
					iter.set("X");
					fail("UnsupportedOperationException expected to be thrown");
				} catch (UnsupportedOperationException e) {
				}
			};
			iteratorTest.accept(container.crbegin());
			iteratorTest.accept(new QList.ConstReverseIterator<>(container.rbegin()));
		}
		{
			Consumer<QList.ReverseIterator<String>> iteratorTest = keyIterator->{
				var iter = keyIterator.bidirectionalIterator();
				assertTrue(iter.hasNext());
				assertFalse(iter.hasPrevious());
				assertEquals("B", iter.next());
				assertTrue(iter.hasNext());
				assertEquals("A", iter.next());
				assertFalse(iter.hasNext());
				assertTrue(iter.hasPrevious());
				assertEquals("A", iter.previous());
				iter.set("X");
			};
			iteratorTest.accept(container.rbegin());
		}
		{
			Consumer<QList.ConstIterator<String>> iteratorTest = keyIterator->{
				var iter = keyIterator.bidirectionalIterator();
				assertTrue(iter.hasNext());
				assertFalse(iter.hasPrevious());
				assertEquals("A", iter.next());
				assertTrue(iter.hasNext());
				assertEquals("X", iter.next());
				assertFalse(iter.hasNext());
				assertTrue(iter.hasPrevious());
				assertEquals("X", iter.previous());
				try {
					iter.set("B");
					fail("UnsupportedOperationException expected to be thrown");
				} catch (UnsupportedOperationException e) {
				}
			};
			iteratorTest.accept(container.cbegin());
			iteratorTest.accept(new QList.ConstIterator<>(container.begin()));
		}
		{
			Consumer<QList.Iterator<String>> iteratorTest = keyIterator->{
				var iter = keyIterator.bidirectionalIterator();
				assertTrue(iter.hasNext());
				assertFalse(iter.hasPrevious());
				assertEquals("A", iter.next());
				assertTrue(iter.hasNext());
				assertEquals("X", iter.next());
				assertFalse(iter.hasNext());
				assertTrue(iter.hasPrevious());
				assertEquals("X", iter.previous());
				iter.set("B");
			};
			iteratorTest.accept(container.begin());
		}
    }
    
    @Test
    public void test_QSet_template() {
    	QSet<String> container = QSet.of("A", "B");
    	assertTrue(container.constBegin() instanceof QSet.ConstIterator);
    	assertEquals(QMetaType.fromType(String.class), container.elementMetaType());
		assertEquals(QMetaType.fromType(String.class), container.constBegin().elementMetaType());
    	QSet<String> clone = container.clone();
    	
		QSet.ConstIterator<String> iter1 = container.constBegin();
		iter1.advance();
		QSet.ConstIterator<String> iter2 = clone.constBegin();
		iter2.advance();
		assertEquals(iter1, iter2);
		Set<String> test = new HashSet<>();
		for(QSet.ConstIterator<String> iter = container.constBegin(), end = container.constEnd(); !iter.equals(end); iter.advance()) {
			test.add(iter.get());
		}
		assertEquals(test, container);
		clone.dispose();
		container.clear();
		
		container = QSet.of("A", "B");
		iter1 = container.constBegin();
		container.insert("C");
		assertTrue("const iterator invalid after concurrent mutation", iter1.isValid());
		assertEquals(container.cbegin(), new QSet.ConstIterator<>(container.cbegin()));
    }
    
    @Test
    public void test_QMap_template() {
    	QMap<String,Object> container = QMap.of("A", QVariant.fromValue(new QPoint(1,2)),"B", QVariant.fromValue(new QPoint(3,4)));
		assertEquals(QMetaType.fromType(String.class), container.keyMetaType());
		assertEquals(QMetaType.fromType(QVariant.class), container.valueMetaType());
    	assertTrue(container.begin() instanceof QMap.Iterator);
    	assertTrue(container.constBegin() instanceof QMap.ConstIterator);
		assertEquals(QMetaType.fromType(String.class), container.begin().keyMetaType());
		assertEquals(QMetaType.fromType(QVariant.class), container.begin().valueMetaType());
		assertEquals(QMetaType.fromType(String.class), container.constBegin().keyMetaType());
		assertEquals(QMetaType.fromType(QVariant.class), container.constBegin().valueMetaType());
		assertEquals(QMetaType.fromType(String.class), container.keyBegin().elementMetaType());
		assertEquals(QMetaType.fromType(QPair.class, QMetaType.fromType(String.class), QMetaType.fromType(QVariant.class)), container.keyValueBegin().elementMetaType());
		assertEquals(QMetaType.fromType(QPair.class, QMetaType.fromType(String.class), QMetaType.fromType(QVariant.class)), container.constKeyValueBegin().elementMetaType());
    	assertEquals(container, new QMap<>(container.constBegin(), container.constEnd()));
    	assertEquals(container, new QMap<>(container.begin(), container.end()));

		assertTrue(container.constKeyValueBegin().isValid());
		assertTrue(container.keyValueBegin().isValid());
		assertEquals(container.keyValueBegin(), container.constKeyValueBegin());
		
    	QMap.Iterator<String,Object> miter1 = container.begin();
    	QMap<String,Object> clone = container.clone();
    	assertTrue(container.isSharedWith(clone));
    	
		QMap.ConstIterator<String,Object> iter1 = container.constBegin();
		assertEquals(new QPoint(1,2), iter1.value());
		assertEquals(iter1, miter1);
		assertEquals(miter1, iter1);
		iter1.advance();
		QMap.ConstIterator<String,Object> iter2 = clone.constBegin();
		assertEquals(new QPoint(1,2), iter2.value());
		iter2.advance();
		assertEquals(iter1, iter2);
		Map<String,Object> test = new TreeMap<>();
		for(QMap.ConstIterator<String,Object> iter = container.constBegin(), end = container.constEnd(); !iter.equals(end); iter.advance()) {
			test.put(iter.key(), iter.value());
		}
		assertEquals(test, container);
		assertTrue(miter1.isValid());
		clone.dispose();
		assertTrue(miter1.isValid());
		container.clear();
		assertFalse(miter1.isValid());
		assertEquals(new QPoint(3,4), iter1.value());
		assertEquals(new QPoint(3,4), iter2.value());
		assertEquals(iter1, iter2);
		iter1.advance();
		assertFalse(iter1.isValid());
		container = QMap.of("A", QVariant.fromValue(new QPoint(1,2)),"B", QVariant.fromValue(new QPoint(3,4)));
		miter1 = container.begin();
		assertTrue(miter1.isValid());
		container.dispose();
		assertFalse(miter1.isValid());
		assertTrue(miter1.isDisposed());
		
		container = QMap.of("A", QVariant.fromValue(new QPoint(1,2)),"B", QVariant.fromValue(new QPoint(3,4)));
		miter1 = container.begin();
		miter1.setValue(QVariant.fromValue(new QPoint(5,5)));
		iter1 = container.constBegin();
		assertEquals(new QPoint(5,5), iter1.value());
		container.insert("0", new QPoint());
		assertFalse("mutable iterator not invalid after concurrent mutation", miter1.isValid());
		assertTrue("const iterator invalid after concurrent mutation", iter1.isValid());
		
		container = QMap.of("Z", QVariant.fromValue(new QPoint(1,2)), "Y", QVariant.fromValue(new QPoint(1,2)));
		{
			String _A, _B;
			if("Z".equals(container.keyBegin().get())) {
				_A = "Z";
				_B = "Y";
			}else {
				_A = "Y";
				_B = "Z";
			}
			assertEquals(QPair.pair(_A, new QPoint(1,2)), container.constKeyValueBegin().get());
			assertEquals(QList.of(QPair.pair(_A, QVariant.fromValue(new QPoint(1,2))), QPair.pair(_B, QVariant.fromValue(new QPoint(1,2)))), QList.of(container.constKeyValueBegin(), container.constKeyValueEnd()));
			assertEquals(QList.of(_A, _B), new QList<>(container.keyBegin(), container.keyEnd()));
			
			assertEquals(_A, new QMap.ConstIterator<>(container.cbegin()).key());
			assertEquals(new QPair<>(_A, new QPoint(1,2)), new QMap.ConstKeyValueIterator<>(container.constKeyValueBegin()).get());
			assertEquals(_A, new QMap.ConstIterator<>(container.begin()).key());
			assertEquals(new QPair<>(_A, new QPoint(1,2)), new QMap.ConstKeyValueIterator<>(container.keyValueBegin()).get());
			assertEquals(new QPair<>(_A, new QPoint(1,2)), new QMap.ConstKeyValueIterator<>(container.begin()).get());
			assertEquals(new QPair<>(_A, new QPoint(1,2)), new QMap.ConstKeyValueIterator<>(container.constBegin()).get());
			assertEquals(new QPair<>(_A, new QPoint(1,2)), new QMap.KeyValueIterator<>(container.begin()).get());
			assertEquals(_A, new QMap.KeyIterator<>(container.constBegin()).get());
			assertEquals(_A, new QMap.KeyIterator<>(container.begin()).get());
			{
				Consumer<QMap.KeyIterator<String>> iteratorTest = keyIterator->{
					var iter = keyIterator.iterator();
					assertTrue(iter.hasNext());
					assertEquals(_A, iter.next());
					assertTrue(iter.hasNext());
					assertEquals(_B, iter.next());
				};
				iteratorTest.accept(container.keyBegin());
				iteratorTest.accept(new QMap.KeyIterator<>(container.constBegin()));
				iteratorTest.accept(new QMap.KeyIterator<>(container.begin()));
			}
			{
				Consumer<QMap.ConstKeyValueIterator<String,Object>> iteratorTest = keyValueIterator->{
					var iter = keyValueIterator.iterator();
					assertTrue(iter.hasNext());
					assertEquals(new QPair<>(_A, new QPoint(1,2)), iter.next());
					assertTrue(iter.hasNext());
					assertEquals(new QPair<>(_B, new QPoint(1,2)), iter.next());
				};
				iteratorTest.accept(container.constKeyValueBegin());
				iteratorTest.accept(new QMap.ConstKeyValueIterator<>(container.constBegin()));
				iteratorTest.accept(new QMap.ConstKeyValueIterator<>(container.begin()));
			}
			{
				Consumer<QMap.KeyValueIterator<String,Object>> iteratorTest = keyValueIterator->{
					var iter = keyValueIterator.iterator();
					assertTrue(iter.hasNext());
					assertEquals(new QPair<>(_A, new QPoint(1,2)), iter.next());
					assertTrue(iter.hasNext());
					assertEquals(new QPair<>(_B, new QPoint(1,2)), iter.next());
				};
				iteratorTest.accept(container.keyValueBegin());
				iteratorTest.accept(new QMap.KeyValueIterator<>(container.begin()));
			}
	    }
		
		QMap.Iterator<String,Object> begin = container.begin();
		QMap.KeyValueIterator<String,Object> kvbegin = container.keyValueBegin();
		QMap.ConstIterator<String,Object> cbegin = container.cbegin();
		QMap.ConstKeyValueIterator<String,Object> ckvbegin = container.constKeyValueBegin();
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
		container.keyValueBegin().set(QVariant.fromValue(new QPoint(99,99)));
		assertEquals(new QPoint(99,99), container.constBegin().value());
    }
    
    @Test
    public void test_QHash_template() {
    	QHash<String,Object> container = QHash.of("A", QVariant.fromValue(new QPoint(1,2)),"B", QVariant.fromValue(new QPoint(1,2)));
		assertEquals(QMetaType.fromType(String.class), container.keyMetaType());
		assertEquals(QMetaType.fromType(QVariant.class), container.valueMetaType());
    	assertTrue(container.begin() instanceof QHash.Iterator);
    	assertTrue(container.constBegin() instanceof QHash.ConstIterator);
		assertEquals(QMetaType.fromType(String.class), container.begin().keyMetaType());
		assertEquals(QMetaType.fromType(QVariant.class), container.begin().valueMetaType());
		assertEquals(QMetaType.fromType(String.class), container.constBegin().keyMetaType());
		assertEquals(QMetaType.fromType(QVariant.class), container.constBegin().valueMetaType());
		assertEquals(QMetaType.fromType(String.class), container.keyBegin().elementMetaType());
		assertEquals(QMetaType.fromType(QPair.class, QMetaType.fromType(String.class), QMetaType.fromType(QVariant.class)), container.keyValueBegin().elementMetaType());
		assertEquals(QMetaType.fromType(QPair.class, QMetaType.fromType(String.class), QMetaType.fromType(QVariant.class)), container.constKeyValueBegin().elementMetaType());
    	assertEquals(container, new QHash<>(container.constBegin(), container.constEnd()));
    	assertEquals(container, new QHash<>(container.begin(), container.end()));

		assertTrue(container.constKeyValueBegin().isValid());
		assertTrue(container.keyValueBegin().isValid());
		assertEquals(container.keyValueBegin(), container.constKeyValueBegin());
		
    	QHash.Iterator<String,Object> miter1 = container.begin();
    	QHash<String,Object> clone = container.clone();
    	assertTrue(container.isSharedWith(clone));
    	
    	QHash.ConstIterator<String,Object> iter1 = container.constBegin();
		assertEquals(iter1, miter1);
		assertEquals(miter1, iter1);
		iter1.advance();
		QHash.ConstIterator<String,Object> iter2 = clone.constBegin();
		iter2.advance();
		assertEquals(iter1, iter2);
		Map<String,Object> test = new HashMap<>();
		for(QHash.ConstIterator<String,Object> iter = container.constBegin(), end = container.constEnd(); !iter.equals(end); iter.advance()) {
			test.put(iter.key(), iter.value());
		}
		assertEquals(test, container);
		assertTrue(miter1.isValid());
		clone.dispose();
		assertTrue(miter1.isValid());
		container.clear();
		assertFalse(miter1.isValid());
		assertEquals(new QPoint(1,2), iter1.value());
		assertEquals(new QPoint(1,2), iter2.value());
		assertEquals(iter1, iter2);
		iter1.advance();
		assertFalse(iter1.isValid());
		container = QHash.of("A", QVariant.fromValue(new QPoint(1,2)),"B", QVariant.fromValue(new QPoint(1,2)));
		miter1 = container.begin();
		assertTrue(miter1.isValid());
		container.dispose();
		assertFalse(miter1.isValid());
		assertTrue(miter1.isDisposed());
		
		container = QHash.of("-1", QVariant.fromValue(new QPoint(1,2)),"-2", QVariant.fromValue(new QPoint(1,2)));
		miter1 = container.begin();
		miter1.setValue(QVariant.fromValue(new QPoint(5,5)));
		iter1 = container.constBegin();
		assertEquals(new QPoint(5,5), iter1.value());
		for (int i = 0; i < 128; i++) {
			container.insert(""+i, new QPoint());
		}
		assertFalse("mutable iterator not invalid after concurrent mutation", miter1.isValid());
		assertTrue("const iterator invalid after concurrent mutation", iter1.isValid());
		
		container = QHash.of("Z", QVariant.fromValue(new QPoint(1,2)), "Y", QVariant.fromValue(new QPoint(1,2)));
		{
			String _A, _B;
			if("Z".equals(container.keyBegin().get())) {
				_A = "Z";
				_B = "Y";
			}else {
				_A = "Y";
				_B = "Z";
			}
			assertEquals(QPair.pair(_A, new QPoint(1,2)), container.constKeyValueBegin().get());
			assertEquals(QList.of(QPair.pair(_A, QVariant.fromValue(new QPoint(1,2))), QPair.pair(_B, QVariant.fromValue(new QPoint(1,2)))), QList.of(container.constKeyValueBegin(), container.constKeyValueEnd()));
			assertEquals(QList.of(_A, _B), new QList<>(container.keyBegin(), container.keyEnd()));
			
			assertEquals(_A, new QHash.ConstIterator<>(container.cbegin()).key());
			assertEquals(new QPair<>(_A, new QPoint(1,2)), new QHash.ConstKeyValueIterator<>(container.constKeyValueBegin()).get());
			assertEquals(_A, new QHash.ConstIterator<>(container.begin()).key());
			assertEquals(new QPair<>(_A, new QPoint(1,2)), new QHash.ConstKeyValueIterator<>(container.keyValueBegin()).get());
			assertEquals(new QPair<>(_A, new QPoint(1,2)), new QHash.ConstKeyValueIterator<>(container.begin()).get());
			assertEquals(new QPair<>(_A, new QPoint(1,2)), new QHash.ConstKeyValueIterator<>(container.constBegin()).get());
			assertEquals(new QPair<>(_A, new QPoint(1,2)), new QHash.KeyValueIterator<>(container.begin()).get());
			assertEquals(_A, new QHash.KeyIterator<>(container.constBegin()).get());
			assertEquals(_A, new QHash.KeyIterator<>(container.begin()).get());
			{
				Consumer<QHash.KeyIterator<String>> iteratorTest = keyIterator->{
					var iter = keyIterator.iterator();
					assertTrue(iter.hasNext());
					assertEquals(_A, iter.next());
					assertTrue(iter.hasNext());
					assertEquals(_B, iter.next());
				};
				iteratorTest.accept(container.keyBegin());
				iteratorTest.accept(new QHash.KeyIterator<>(container.constBegin()));
				iteratorTest.accept(new QHash.KeyIterator<>(container.begin()));
			}
			{
				Consumer<QHash.ConstKeyValueIterator<String,Object>> iteratorTest = keyValueIterator->{
					var iter = keyValueIterator.iterator();
					assertTrue(iter.hasNext());
					assertEquals(new QPair<>(_A, new QPoint(1,2)), iter.next());
					assertTrue(iter.hasNext());
					assertEquals(new QPair<>(_B, new QPoint(1,2)), iter.next());
				};
				iteratorTest.accept(container.constKeyValueBegin());
				iteratorTest.accept(new QHash.ConstKeyValueIterator<>(container.constBegin()));
				iteratorTest.accept(new QHash.ConstKeyValueIterator<>(container.begin()));
			}
			{
				Consumer<QHash.KeyValueIterator<String,Object>> iteratorTest = keyValueIterator->{
					var iter = keyValueIterator.iterator();
					assertTrue(iter.hasNext());
					assertEquals(new QPair<>(_A, new QPoint(1,2)), iter.next());
					assertTrue(iter.hasNext());
					assertEquals(new QPair<>(_B, new QPoint(1,2)), iter.next());
				};
				iteratorTest.accept(container.keyValueBegin());
				iteratorTest.accept(new QHash.KeyValueIterator<>(container.begin()));
			}
		}
		
		QHash.Iterator<String,Object> begin = container.begin();
		QHash.KeyValueIterator<String,Object> kvbegin = container.keyValueBegin();
		QHash.ConstIterator<String,Object> cbegin = container.cbegin();
		QHash.ConstKeyValueIterator<String,Object> ckvbegin = container.constKeyValueBegin();
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
		
		container.keyValueBegin().set(QVariant.fromValue(new QPoint(99,99)));
		assertEquals(new QPoint(99,99), container.constBegin().value());
    }
    
    @Test
    public void test_QMultiMap_template() {
    	QMultiMap<String,Object> container = QMultiMap.of("A", QVariant.fromValue(new QPoint(1,2)),"B", QVariant.fromValue(new QPoint(3,4)));
		assertEquals(QMetaType.fromType(String.class), container.keyMetaType());
		assertEquals(QMetaType.fromType(QVariant.class), container.valueMetaType());
    	assertTrue(container.begin() instanceof QMultiMap.Iterator);
    	assertTrue(container.constBegin() instanceof QMultiMap.ConstIterator);
		assertEquals(QMetaType.fromType(String.class), container.begin().keyMetaType());
		assertEquals(QMetaType.fromType(QVariant.class), container.begin().valueMetaType());
		assertEquals(QMetaType.fromType(String.class), container.constBegin().keyMetaType());
		assertEquals(QMetaType.fromType(QVariant.class), container.constBegin().valueMetaType());
		assertEquals(QMetaType.fromType(String.class), container.keyBegin().elementMetaType());
		assertEquals(QMetaType.fromType(QPair.class, QMetaType.fromType(String.class), QMetaType.fromType(QVariant.class)), container.keyValueBegin().elementMetaType());
		assertEquals(QMetaType.fromType(QPair.class, QMetaType.fromType(String.class), QMetaType.fromType(QVariant.class)), container.constKeyValueBegin().elementMetaType());
    	assertEquals(container, new QMultiMap<>(container.constBegin(), container.constEnd()));
    	assertEquals(container, new QMultiMap<>(container.begin(), container.end()));

		assertTrue(container.constKeyValueBegin().isValid());
		assertTrue(container.keyValueBegin().isValid());
		assertEquals(container.keyValueBegin(), container.constKeyValueBegin());
		
    	QMultiMap.Iterator<String,Object> miter1 = container.begin();
    	QMultiMap<String,Object> clone = container.clone();
    	assertTrue(container.isSharedWith(clone));
    	
		QMultiMap.ConstIterator<String,Object> iter1 = container.constBegin();
		assertEquals(iter1, miter1);
		assertEquals(miter1, iter1);
		iter1.advance();
		QMultiMap.ConstIterator<String,Object> iter2 = clone.constBegin();
		iter2.advance();
		assertEquals(iter1, iter2);
		Map<String,List<Object>> test = new TreeMap<>();
		for(QMultiMap.ConstIterator<String,Object> iter = container.constBegin(), end = container.constEnd(); !iter.equals(end); iter.advance()) {
			test.computeIfAbsent(iter.key(), k->new ArrayList<>()).add(iter.value());
		}
		assertEquals(test, container);
		assertTrue(miter1.isValid());
		clone.dispose();
		assertTrue(miter1.isValid());
		container.clear();
		assertFalse(miter1.isValid());
		assertEquals(new QPoint(3,4), iter1.value());
		assertEquals(new QPoint(3,4), iter2.value());
		assertEquals(iter1, iter2);
		iter1.advance();
		assertFalse(iter1.isValid());
		container = QMultiMap.of("A", QVariant.fromValue(new QPoint(1,2)),"B", QVariant.fromValue(new QPoint(3,4)));
		miter1 = container.begin();
		assertTrue(miter1.isValid());
		container.dispose();
		assertFalse(miter1.isValid());
		assertTrue(miter1.isDisposed());
		
		container = QMultiMap.of("A", QVariant.fromValue(new QPoint(1,2)),"B", QVariant.fromValue(new QPoint(3,4)));
		miter1 = container.begin();
		miter1.setValue(QVariant.fromValue(new QPoint(5,5)));
		iter1 = container.constBegin();
		assertEquals(new QPoint(5,5), iter1.value());
		container.insert("0", new QPoint());
		assertFalse("mutable iterator not invalid after concurrent mutation", miter1.isValid());
		assertTrue("const iterator invalid after concurrent mutation", iter1.isValid());
		
		container = QMultiMap.of("Z", QVariant.fromValue(new QPoint(1,2)), "Y", QVariant.fromValue(new QPoint(1,2)));
		{
			String _A, _B;
			if("Z".equals(container.keyBegin().get())) {
				_A = "Z";
				_B = "Y";
			}else {
				_A = "Y";
				_B = "Z";
			}
			assertEquals(QPair.pair(_A, new QPoint(1,2)), container.constKeyValueBegin().get());
			assertEquals(QList.of(QPair.pair(_A, QVariant.fromValue(new QPoint(1,2))), QPair.pair(_B, QVariant.fromValue(new QPoint(1,2)))), QList.of(container.constKeyValueBegin(), container.constKeyValueEnd()));
			assertEquals(QList.of(_A, _B), new QList<>(container.keyBegin(), container.keyEnd()));
			
			assertEquals(_A, new QMultiMap.ConstIterator<>(container.cbegin()).key());
			assertEquals(new QPair<>(_A, new QPoint(1,2)), new QMultiMap.ConstKeyValueIterator<>(container.constKeyValueBegin()).get());
			assertEquals(_A, new QMultiMap.ConstIterator<>(container.begin()).key());
			assertEquals(new QPair<>(_A, new QPoint(1,2)), new QMultiMap.ConstKeyValueIterator<>(container.keyValueBegin()).get());
			assertEquals(new QPair<>(_A, new QPoint(1,2)), new QMultiMap.ConstKeyValueIterator<>(container.begin()).get());
			assertEquals(new QPair<>(_A, new QPoint(1,2)), new QMultiMap.ConstKeyValueIterator<>(container.constBegin()).get());
			assertEquals(new QPair<>(_A, new QPoint(1,2)), new QMultiMap.KeyValueIterator<>(container.begin()).get());
			assertEquals(_A, new QMultiMap.KeyIterator<>(container.constBegin()).get());
			assertEquals(_A, new QMultiMap.KeyIterator<>(container.begin()).get());
			{
				Consumer<QMultiMap.KeyIterator<String>> iteratorTest = keyIterator->{
					var iter = keyIterator.iterator();
					assertTrue(iter.hasNext());
					assertEquals(_A, iter.next());
					assertTrue(iter.hasNext());
					assertEquals(_B, iter.next());
				};
				iteratorTest.accept(container.keyBegin());
				iteratorTest.accept(new QMultiMap.KeyIterator<>(container.constBegin()));
				iteratorTest.accept(new QMultiMap.KeyIterator<>(container.begin()));
			}
			{
				Consumer<QMultiMap.ConstKeyValueIterator<String,Object>> iteratorTest = keyValueIterator->{
					var iter = keyValueIterator.iterator();
					assertTrue(iter.hasNext());
					assertEquals(new QPair<>(_A, new QPoint(1,2)), iter.next());
					assertTrue(iter.hasNext());
					assertEquals(new QPair<>(_B, new QPoint(1,2)), iter.next());
				};
				iteratorTest.accept(container.constKeyValueBegin());
				iteratorTest.accept(new QMultiMap.ConstKeyValueIterator<>(container.constBegin()));
				iteratorTest.accept(new QMultiMap.ConstKeyValueIterator<>(container.begin()));
			}
			{
				Consumer<QMultiMap.KeyValueIterator<String,Object>> iteratorTest = keyValueIterator->{
					var iter = keyValueIterator.iterator();
					assertTrue(iter.hasNext());
					assertEquals(new QPair<>(_A, new QPoint(1,2)), iter.next());
					assertTrue(iter.hasNext());
					assertEquals(new QPair<>(_B, new QPoint(1,2)), iter.next());
				};
				iteratorTest.accept(container.keyValueBegin());
				iteratorTest.accept(new QMultiMap.KeyValueIterator<>(container.begin()));
			}
		}
		
		QMultiMap.Iterator<String,Object> begin = container.begin();
		QMultiMap.KeyValueIterator<String,Object> kvbegin = container.keyValueBegin();
		QMultiMap.ConstIterator<String,Object> cbegin = container.cbegin();
		QMultiMap.ConstKeyValueIterator<String,Object> ckvbegin = container.constKeyValueBegin();
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

		container.keyValueBegin().set(QVariant.fromValue(new QPoint(99,99)));
		assertEquals(new QPoint(99,99), container.constBegin().value());
    }
    
    @Test
    public void test_QMultiHash_template() {
    	QMultiHash<String,Object> container = QMultiHash.of("A", QVariant.fromValue(new QPoint(1,2)),"B", QVariant.fromValue(new QPoint(1,2)));
		assertEquals(QMetaType.fromType(String.class), container.keyMetaType());
		assertEquals(QMetaType.fromType(QVariant.class), container.valueMetaType());
    	assertTrue(container.begin() instanceof QMultiHash.Iterator);
    	assertTrue(container.constBegin() instanceof QMultiHash.ConstIterator);
		assertEquals(QMetaType.fromType(String.class), container.begin().keyMetaType());
		assertEquals(QMetaType.fromType(QVariant.class), container.begin().valueMetaType());
		assertEquals(QMetaType.fromType(String.class), container.constBegin().keyMetaType());
		assertEquals(QMetaType.fromType(QVariant.class), container.constBegin().valueMetaType());
		assertEquals(QMetaType.fromType(String.class), container.keyBegin().elementMetaType());
		assertEquals(QMetaType.fromType(QPair.class, QMetaType.fromType(String.class), QMetaType.fromType(QVariant.class)), container.keyValueBegin().elementMetaType());
		assertEquals(QMetaType.fromType(QPair.class, QMetaType.fromType(String.class), QMetaType.fromType(QVariant.class)), container.constKeyValueBegin().elementMetaType());
    	assertEquals(container, new QMultiHash<>(container.constBegin(), container.constEnd()));
    	assertEquals(container, new QMultiHash<>(container.begin(), container.end()));

		assertTrue(container.constKeyValueBegin().isValid());
		assertTrue(container.keyValueBegin().isValid());
		assertEquals(container.keyValueBegin(), container.constKeyValueBegin());
		
    	QMultiHash.Iterator<String,Object> miter1 = container.begin();
    	QMultiHash<String,Object> clone = container.clone();
    	assertTrue(container.isSharedWith(clone));
    	
    	QMultiHash.ConstIterator<String,Object> iter1 = container.constBegin();
		assertEquals(iter1, miter1);
		assertEquals(miter1, iter1);
		iter1.advance();
		QMultiHash.ConstIterator<String,Object> iter2 = clone.constBegin();
		iter2.advance();
		assertEquals(iter1, iter2);
		Map<String,List<Object>> test = new HashMap<>();
		for(QMultiHash.ConstIterator<String,Object> iter = container.constBegin(), end = container.constEnd(); !iter.equals(end); iter.advance()) {
			test.computeIfAbsent(iter.key(), k->new ArrayList<>()).add(iter.value());
		}
		assertEquals(test, container);
		assertTrue(miter1.isValid());
		clone.dispose();
		assertTrue(miter1.isValid());
		container.clear();
		assertFalse(miter1.isValid());
		assertEquals(new QPoint(1,2), iter1.value());
		assertEquals(new QPoint(1,2), iter2.value());
		assertEquals(iter1, iter2);
		iter1.advance();
		assertFalse(iter1.isValid());
		container = QMultiHash.of("A", QVariant.fromValue(new QPoint(1,2)),"B", QVariant.fromValue(new QPoint(1,2)));
		miter1 = container.begin();
		assertTrue(miter1.isValid());
		container.dispose();
		assertFalse(miter1.isValid());
		assertTrue(miter1.isDisposed());
		
		container = QMultiHash.of("-1", QVariant.fromValue(new QPoint(1,2)),"-2", QVariant.fromValue(new QPoint(1,2)));
		miter1 = container.begin();
		miter1.setValue(QVariant.fromValue(new QPoint(5,5)));
		iter1 = container.constBegin();
		assertEquals(new QPoint(5,5), iter1.value());
		for (int i = 0; i < 128; i++) {
			container.insert(""+i, new QPoint());
		}
		assertFalse("mutable iterator not invalid after concurrent mutation", miter1.isValid());
		assertTrue("const iterator invalid after concurrent mutation", iter1.isValid());
		
		container = QMultiHash.of("Z", QVariant.fromValue(new QPoint(1,2)), "Y", QVariant.fromValue(new QPoint(1,2)));
		{
			String _A, _B;
			if("Z".equals(container.keyBegin().get())) {
				_A = "Z";
				_B = "Y";
			}else {
				_A = "Y";
				_B = "Z";
			}
			assertEquals(QPair.pair(_A, new QPoint(1,2)), container.constKeyValueBegin().get());
			assertEquals(QList.of(QPair.pair(_A, QVariant.fromValue(new QPoint(1,2))), QPair.pair(_B, QVariant.fromValue(new QPoint(1,2)))), QList.of(container.constKeyValueBegin(), container.constKeyValueEnd()));
			assertEquals(QList.of(_A, _B), new QList<>(container.keyBegin(), container.keyEnd()));
			
			assertEquals(_A, new QMultiHash.ConstIterator<>(container.cbegin()).key());
			assertEquals(new QPair<>(_A, new QPoint(1,2)), new QMultiHash.ConstKeyValueIterator<>(container.constKeyValueBegin()).get());
			assertEquals(_A, new QMultiHash.ConstIterator<>(container.begin()).key());
			assertEquals(new QPair<>(_A, new QPoint(1,2)), new QMultiHash.ConstKeyValueIterator<>(container.keyValueBegin()).get());
			assertEquals(new QPair<>(_A, new QPoint(1,2)), new QMultiHash.ConstKeyValueIterator<>(container.begin()).get());
			assertEquals(new QPair<>(_A, new QPoint(1,2)), new QMultiHash.ConstKeyValueIterator<>(container.constBegin()).get());
			assertEquals(new QPair<>(_A, new QPoint(1,2)), new QMultiHash.KeyValueIterator<>(container.begin()).get());
			assertEquals(_A, new QMultiHash.KeyIterator<>(container.constBegin()).get());
			assertEquals(_A, new QMultiHash.KeyIterator<>(container.begin()).get());
			{
				Consumer<QMultiHash.KeyIterator<String>> iteratorTest = keyIterator->{
					var iter = keyIterator.iterator();
					assertTrue(iter.hasNext());
					assertEquals(_A, iter.next());
					assertTrue(iter.hasNext());
					assertEquals(_B, iter.next());
				};
				iteratorTest.accept(container.keyBegin());
				iteratorTest.accept(new QMultiHash.KeyIterator<>(container.constBegin()));
				iteratorTest.accept(new QMultiHash.KeyIterator<>(container.begin()));
			}
			{
				Consumer<QMultiHash.ConstKeyValueIterator<String,Object>> iteratorTest = keyValueIterator->{
					var iter = keyValueIterator.iterator();
					assertTrue(iter.hasNext());
					assertEquals(new QPair<>(_A, new QPoint(1,2)), iter.next());
					assertTrue(iter.hasNext());
					assertEquals(new QPair<>(_B, new QPoint(1,2)), iter.next());
				};
				iteratorTest.accept(container.constKeyValueBegin());
				iteratorTest.accept(new QMultiHash.ConstKeyValueIterator<>(container.constBegin()));
				iteratorTest.accept(new QMultiHash.ConstKeyValueIterator<>(container.begin()));
			}
			{
				Consumer<QMultiHash.KeyValueIterator<String,Object>> iteratorTest = keyValueIterator->{
					var iter = keyValueIterator.iterator();
					assertTrue(iter.hasNext());
					assertEquals(new QPair<>(_A, new QPoint(1,2)), iter.next());
					assertTrue(iter.hasNext());
					assertEquals(new QPair<>(_B, new QPoint(1,2)), iter.next());
				};
				iteratorTest.accept(container.keyValueBegin());
				iteratorTest.accept(new QMultiHash.KeyValueIterator<>(container.begin()));
			}
		}
		
		QMultiHash.Iterator<String,Object> begin = container.begin();
		QMultiHash.KeyValueIterator<String,Object> kvbegin = container.keyValueBegin();
		QMultiHash.ConstIterator<String,Object> cbegin = container.cbegin();
		QMultiHash.ConstKeyValueIterator<String,Object> ckvbegin = container.constKeyValueBegin();
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

		container.keyValueBegin().set(QVariant.fromValue(new QPoint(99,99)));
		assertEquals(new QPoint(99,99), container.constBegin().value());
    }
    
    @Test
    public void test_QSet_auto() {
    	QSet<QPoint> container = QSet.of(new QPoint(1,2),new QPoint(3,4));
    	assertTrue(container.constBegin() instanceof QSet.ConstIterator);
    	assertEquals(QMetaType.fromType(QPoint.class), container.elementMetaType());
		assertEquals(QMetaType.fromType(QPoint.class), container.constBegin().elementMetaType());
    	QSet<QPoint> clone = container.clone();
    	
		QSet.ConstIterator<QPoint> iter1 = container.constBegin();
		iter1.advance();
		QSet.ConstIterator<QPoint> iter2 = clone.constBegin();
		iter2.advance();
		assertEquals(iter1, iter2);
		Set<QPoint> test = new HashSet<>();
		for(QSet.ConstIterator<QPoint> iter = container.constBegin(), end = container.constEnd(); !iter.equals(end); iter.advance()) {
			test.add(iter.get());
		}
		assertEquals(test, container);
		clone.dispose();
		container.clear();
		container = QSet.of(new QPoint(1,2),new QPoint(3,4));
		iter1 = container.constBegin();
		container.insert(new QPoint(8,9));
		assertTrue("const iterator invalid after concurrent mutation", iter1.isValid());

		container = QSet.of(new QPoint(1,2),new QPoint(3,4));
		assertEquals(container.cbegin(), new QSet.ConstIterator<>(container.cbegin()));
    }
    
    @Test
    public void test_QList_auto() {
    	QList<QVector2D> container = QList.of(new QVector2D(1,2),new QVector2D(3,4));
    	assertTrue(container.begin() instanceof QList.Iterator);
    	assertTrue(container.constBegin() instanceof QList.ConstIterator);
    	assertTrue(container.reverseBegin() instanceof QList.ReverseIterator);
    	assertTrue(container.constReverseBegin() instanceof QList.ConstReverseIterator);
    	assertEquals(QMetaType.fromType(QVector2D.class), container.elementMetaType());
		assertEquals(QMetaType.fromType(QVector2D.class), container.begin().elementMetaType());
		assertEquals(QMetaType.fromType(QVector2D.class), container.constBegin().elementMetaType());
		assertEquals(QMetaType.fromType(QVector2D.class), container.reverseBegin().elementMetaType());
		assertEquals(QMetaType.fromType(QVector2D.class), container.constReverseBegin().elementMetaType());
    	assertEquals(container, new QList<>(container.constBegin(), container.constEnd()));
    	assertEquals(container, new QList<>(container.begin(), container.end()));
    	assertEquals(QList.of(new QVector2D(1,2),new QVector2D(3,4)), new QList<>(container.constBegin(), container.constEnd()));
    	assertEquals(QList.of(new QVector2D(3,4),new QVector2D(1,2)), new QList<>(container.constReverseBegin(), container.constReverseEnd()));

		assertTrue(container.constReverseBegin().isValid());
		assertTrue(container.reverseBegin().isValid());
		assertEquals(container.reverseBegin(), container.constReverseBegin());
		
		QList.Iterator<QVector2D> miter1 = container.begin();
		QList.ReverseIterator<QVector2D> riter1 = container.reverseBegin();
		QList.ConstReverseIterator<QVector2D> criter1 = container.constReverseBegin();
    	QList<QVector2D> clone = container.clone();
    	assertTrue(container.isSharedWith(clone));
    	
		QList.ConstIterator<QVector2D> iter1 = container.constBegin();
		assertEquals(iter1, miter1);
		assertEquals(miter1, iter1);
		assertEquals(riter1, criter1);
		assertEquals(criter1, riter1);
		iter1.advance();
		QList.ConstIterator<QVector2D> iter2 = clone.constBegin();
		iter2.advance();
		assertEquals(iter1, iter2);
		ArrayList<QVector2D> test = new ArrayList<>();
		for(QList.ConstIterator<QVector2D> iter = container.constBegin(), end = container.constEnd(); !iter.equals(end); iter.advance()) {
			test.add(iter.get());
		}
		assertEquals(test, container);
		assertTrue(miter1.isValid());
		clone.dispose();
		assertTrue(miter1.isValid());
		container.clear();
		assertFalse(miter1.isValid());
		container = QList.of(new QVector2D(1,2),new QVector2D(3,4));
		miter1 = container.begin();
		assertTrue(miter1.isValid());
		container.dispose();
		assertFalse(miter1.isValid());
		assertTrue(miter1.isDisposed());
		
		container = QList.of(new QVector2D(1,2),new QVector2D(3,4));
		miter1 = container.begin();
		miter1.set(new QVector2D(5,5));
		iter1 = container.constBegin();
		assertEquals(new QVector2D(5,5), iter1.get());
		assertEquals(new QVector2D(5,5), new QList.ConstIterator<>(miter1).get());
		riter1 = container.reverseBegin();
		riter1.set(new QVector2D(6,6));
		assertEquals(new QVector2D(6,6), riter1.get());
		container.insert(0, new QVector2D());
		assertFalse("mutable iterator not invalid after concurrent mutation", miter1.isValid());
		assertTrue("const iterator invalid after concurrent mutation", iter1.isValid());
		QList<QVector2D> container2 = new QList<>(container.constBegin(), container.constEnd());
		assertEquals(container, container2);
		
		container = QList.of(new QVector2D(1,2),new QVector2D(3,4));
		assertEquals(new QVector2D(1,2), new QList.ConstIterator<>(container.cbegin()).get());
		assertEquals(new QVector2D(3,4), new QList.ConstReverseIterator<>(container.crbegin()).get());
		assertEquals(new QVector2D(1,2), new QList.ConstIterator<>(container.begin()).get());
		assertEquals(new QVector2D(3,4), new QList.ConstReverseIterator<>(container.rbegin()).get());
		QList.Iterator<QVector2D> begin = container.begin();
		QList.ReverseIterator<QVector2D> rbegin = container.rbegin();
		QList.ConstIterator<QVector2D> cbegin = container.cbegin();
		QList.ConstReverseIterator<QVector2D> crbegin = container.crbegin();
		assertEquals(begin, cbegin);
		assertEquals(cbegin, begin);
		assertEquals(rbegin, crbegin);
		assertEquals(crbegin, rbegin);
		{
			Consumer<QList.ConstReverseIterator<QVector2D>> iteratorTest = keyIterator->{
				var iter = keyIterator.iterator();
				assertTrue(iter.hasNext());
				assertEquals(new QVector2D(3,4), iter.next());
				assertTrue(iter.hasNext());
				assertEquals(new QVector2D(1,2), iter.next());
			};
			iteratorTest.accept(container.crbegin());
			iteratorTest.accept(new QList.ConstReverseIterator<>(container.rbegin()));
		}
		{
			Consumer<QList.ReverseIterator<QVector2D>> iteratorTest = keyIterator->{
				var iter = keyIterator.iterator();
				assertTrue(iter.hasNext());
				assertEquals(new QVector2D(3,4), iter.next());
				assertTrue(iter.hasNext());
				assertEquals(new QVector2D(1,2), iter.next());
			};
			iteratorTest.accept(container.rbegin());
		}
		{
			Consumer<QList.ConstReverseIterator<QVector2D>> iteratorTest = keyIterator->{
				var iter = keyIterator.descendingIterator();
				assertTrue(iter.hasNext());
				assertEquals(new QVector2D(1,2), iter.next());
				assertTrue(iter.hasNext());
				assertEquals(new QVector2D(3,4), iter.next());
			};
			iteratorTest.accept(container.crend());
			iteratorTest.accept(new QList.ConstReverseIterator<>(container.rend()));
		}
		{
			Consumer<QList.ReverseIterator<QVector2D>> iteratorTest = keyIterator->{
				var iter = keyIterator.descendingIterator();
				assertTrue(iter.hasNext());
				assertEquals(new QVector2D(1,2), iter.next());
				assertTrue(iter.hasNext());
				assertEquals(new QVector2D(3,4), iter.next());
			};
			iteratorTest.accept(container.rend());
		}
		{
			Consumer<QList.ConstReverseIterator<QVector2D>> iteratorTest = keyIterator->{
				var iter = keyIterator.bidirectionalIterator();
				assertTrue(iter.hasNext());
				assertFalse(iter.hasPrevious());
				assertEquals(new QVector2D(3,4), iter.next());
				assertTrue(iter.hasNext());
				assertEquals(new QVector2D(1,2), iter.next());
				assertFalse(iter.hasNext());
				assertTrue(iter.hasPrevious());
				assertEquals(new QVector2D(1,2), iter.previous());
				try {
					iter.set(new QVector2D(99,99));
					fail("UnsupportedOperationException expected to be thrown");
				} catch (UnsupportedOperationException e) {
				}
			};
			iteratorTest.accept(container.crbegin());
			iteratorTest.accept(new QList.ConstReverseIterator<>(container.rbegin()));
		}
		{
			Consumer<QList.ReverseIterator<QVector2D>> iteratorTest = keyIterator->{
				var iter = keyIterator.bidirectionalIterator();
				assertTrue(iter.hasNext());
				assertFalse(iter.hasPrevious());
				assertEquals(new QVector2D(3,4), iter.next());
				assertTrue(iter.hasNext());
				assertEquals(new QVector2D(1,2), iter.next());
				assertFalse(iter.hasNext());
				assertTrue(iter.hasPrevious());
				assertEquals(new QVector2D(1,2), iter.previous());
				iter.set(new QVector2D(99,99));
			};
			iteratorTest.accept(container.rbegin());
		}
		{
			Consumer<QList.ConstIterator<QVector2D>> iteratorTest = keyIterator->{
				var iter = keyIterator.bidirectionalIterator();
				assertTrue(iter.hasNext());
				assertFalse(iter.hasPrevious());
				assertEquals(new QVector2D(1,2), iter.next());
				assertTrue(iter.hasNext());
				assertEquals(new QVector2D(99,99), iter.next());
				assertFalse(iter.hasNext());
				assertTrue(iter.hasPrevious());
				assertEquals(new QVector2D(99,99), iter.previous());
				try {
					iter.set(new QVector2D(3,4));
					fail("UnsupportedOperationException expected to be thrown");
				} catch (UnsupportedOperationException e) {
				}
			};
			iteratorTest.accept(container.cbegin());
			iteratorTest.accept(new QList.ConstIterator<>(container.begin()));
		}
		{
			Consumer<QList.Iterator<QVector2D>> iteratorTest = keyIterator->{
				var iter = keyIterator.bidirectionalIterator();
				assertTrue(iter.hasNext());
				assertFalse(iter.hasPrevious());
				assertEquals(new QVector2D(1,2), iter.next());
				assertTrue(iter.hasNext());
				assertEquals(new QVector2D(99,99), iter.next());
				assertFalse(iter.hasNext());
				assertTrue(iter.hasPrevious());
				assertEquals(new QVector2D(99,99), iter.previous());
				iter.set(new QVector2D(3,4));
			};
			iteratorTest.accept(container.begin());
		}
    }
    
    @Test
    public void test_QSpan_template() {
    	QList<String> list = QList.of("A", "B");
    	QSpan<String> container = new QSpan<>(list);
    	assertTrue(container.begin() instanceof QSpan.Iterator);
    	assertTrue(container.constBegin() instanceof QSpan.ConstIterator);
    	assertTrue(container.reverseBegin() instanceof QSpan.ReverseIterator);
    	assertTrue(container.constReverseBegin() instanceof QSpan.ConstReverseIterator);
    	assertEquals(QMetaType.fromType(String.class), container.elementMetaType());
		assertEquals(QMetaType.fromType(String.class), container.begin().elementMetaType());
		assertEquals(QMetaType.fromType(String.class), container.constBegin().elementMetaType());
		assertEquals(QMetaType.fromType(String.class), container.reverseBegin().elementMetaType());
		assertEquals(QMetaType.fromType(String.class), container.constReverseBegin().elementMetaType());
    	assertEquals(list, new QList<>(container.constBegin(), container.constEnd()));
    	assertEquals(list, new QList<>(container.begin(), container.end()));
		QSpan.Iterator<String> miter1 = container.begin();
    	QSpan<String> clone = container.clone();
		QSpan.ReverseIterator<String> riter1 = container.reverseBegin();
		QSpan.ConstReverseIterator<String> criter1 = container.constReverseBegin();
//    	assertTrue(container.isSharedWith(clone));

		assertTrue(container.constReverseBegin().isValid());
		assertTrue(container.reverseBegin().isValid());
		assertEquals(container.reverseBegin(), container.constReverseBegin());
		assertEquals(container.constReverseBegin(), container.reverseBegin());
    	
		QSpan.ConstIterator<String> iter1 = container.constBegin();
		assertEquals(iter1, miter1);
		assertEquals(miter1, iter1);
		assertEquals(riter1, criter1);
		assertEquals(criter1, riter1);
		iter1.advance();
		QSpan.ConstIterator<String> iter2 = clone.constBegin();
		iter2.advance();
		assertEquals(iter1, iter2);
		List<String> test = new ArrayList<>();
		for(QSpan.ConstIterator<String> iter = container.constBegin(), end = container.constEnd(); !iter.equals(end); iter.advance()) {
			test.add(iter.get());
		}
		assertEquals(test, container.toList());
		assertTrue(miter1.isValid());
		clone.dispose();
		assertTrue(miter1.isValid());
		container.dispose();
		assertFalse(miter1.isValid());
		container = new QSpan<>(list);
		miter1 = container.begin();
		assertTrue(miter1.isValid());
		container.dispose();
		assertFalse(miter1.isValid());
		assertTrue(miter1.isDisposed());
		
		container = new QSpan<>(list);
		miter1 = container.begin();
		miter1.set("X");
		iter1 = container.constBegin();
		assertEquals("X", iter1.get());
		assertEquals("X", new QSpan.ConstIterator<>(miter1).get());
		riter1 = container.reverseBegin();
		riter1.set("Y");
		assertEquals("Y", riter1.get());
		assertEquals(QList.of("X", "Y"), new QList<>(container));
		container.set(0, "C");
		assertEquals(QList.of("C", "Y"), new QList<>(container));
		assertTrue("mutable iterator invalid after concurrent mutation", miter1.isValid());
		assertTrue("const iterator invalid after concurrent mutation", iter1.isValid());
		
		assertEquals("C", new QSpan.ConstIterator<>(container.cbegin()).get());
		assertEquals("Y", new QSpan.ConstReverseIterator<>(container.crbegin()).get());
		assertEquals("C", new QSpan.ConstIterator<>(container.begin()).get());
		assertEquals("Y", new QSpan.ConstReverseIterator<>(container.rbegin()).get());
		{
			Consumer<QSpan.ConstReverseIterator<String>> iteratorTest = keyIterator->{
				var iter = keyIterator.iterator();
				assertTrue(iter.hasNext());
				assertEquals("Y", iter.next());
				assertTrue(iter.hasNext());
				assertEquals("C", iter.next());
			};
			iteratorTest.accept(container.crbegin());
			iteratorTest.accept(new QSpan.ConstReverseIterator<>(container.rbegin()));
		}
		{
			Consumer<QSpan.ReverseIterator<String>> iteratorTest = keyIterator->{
				var iter = keyIterator.iterator();
				assertTrue(iter.hasNext());
				assertEquals("Y", iter.next());
				assertTrue(iter.hasNext());
				assertEquals("C", iter.next());
			};
			iteratorTest.accept(container.rbegin());
		}
		{
			Consumer<QSpan.ConstReverseIterator<String>> iteratorTest = keyIterator->{
				var iter = keyIterator.descendingIterator();
				assertTrue(iter.hasNext());
				assertEquals("C", iter.next());
				assertTrue(iter.hasNext());
				assertEquals("Y", iter.next());
			};
			iteratorTest.accept(container.crend());
			iteratorTest.accept(new QSpan.ConstReverseIterator<>(container.rend()));
		}
		{
			Consumer<QSpan.ReverseIterator<String>> iteratorTest = keyIterator->{
				var iter = keyIterator.descendingIterator();
				assertTrue(iter.hasNext());
				assertEquals("C", iter.next());
				assertTrue(iter.hasNext());
				assertEquals("Y", iter.next());
			};
			iteratorTest.accept(container.rend());
		}
		{
			Consumer<QSpan.ConstReverseIterator<String>> iteratorTest = keyIterator->{
				var iter = keyIterator.bidirectionalIterator();
				assertTrue(iter.hasNext());
				assertFalse(iter.hasPrevious());
				assertEquals("Y", iter.next());
				assertTrue(iter.hasNext());
				assertEquals("C", iter.next());
				assertFalse(iter.hasNext());
				assertTrue(iter.hasPrevious());
				assertEquals("C", iter.previous());
				try {
					iter.set("X");
					fail("UnsupportedOperationException expected to be thrown");
				} catch (UnsupportedOperationException e) {
				}
			};
			iteratorTest.accept(container.crbegin());
			iteratorTest.accept(new QSpan.ConstReverseIterator<>(container.rbegin()));
		}
		{
			Consumer<QSpan.ReverseIterator<String>> iteratorTest = keyIterator->{
				var iter = keyIterator.bidirectionalIterator();
				assertTrue(iter.hasNext());
				assertFalse(iter.hasPrevious());
				assertEquals("Y", iter.next());
				assertTrue(iter.hasNext());
				assertEquals("C", iter.next());
				assertFalse(iter.hasNext());
				assertTrue(iter.hasPrevious());
				assertEquals("C", iter.previous());
				iter.set("X");
			};
			iteratorTest.accept(container.rbegin());
		}
		{
			Consumer<QSpan.ConstIterator<String>> iteratorTest = keyIterator->{
				var iter = keyIterator.bidirectionalIterator();
				assertTrue(iter.hasNext());
				assertFalse(iter.hasPrevious());
				assertEquals("C", iter.next());
				assertTrue(iter.hasNext());
				assertEquals("X", iter.next());
				assertFalse(iter.hasNext());
				assertTrue(iter.hasPrevious());
				assertEquals("X", iter.previous());
				try {
					iter.set("Y");
					fail("UnsupportedOperationException expected to be thrown");
				} catch (UnsupportedOperationException e) {
				}
			};
			iteratorTest.accept(container.cbegin());
			iteratorTest.accept(new QSpan.ConstIterator<>(container.begin()));
		}
		{
			Consumer<QSpan.Iterator<String>> iteratorTest = keyIterator->{
				var iter = keyIterator.bidirectionalIterator();
				assertTrue(iter.hasNext());
				assertFalse(iter.hasPrevious());
				assertEquals("C", iter.next());
				assertTrue(iter.hasNext());
				assertEquals("X", iter.next());
				assertFalse(iter.hasNext());
				assertTrue(iter.hasPrevious());
				assertEquals("X", iter.previous());
				iter.set("Y");
			};
			iteratorTest.accept(container.begin());
		}
		QSpan.Iterator<String> begin = container.begin();
		QSpan.ReverseIterator<String> rbegin = container.rbegin();
		QSpan.ConstIterator<String> cbegin = container.cbegin();
		QSpan.ConstReverseIterator<String> crbegin = container.crbegin();
		assertEquals(begin, cbegin);
		assertEquals(cbegin, begin);
		assertEquals(rbegin, crbegin);
		assertEquals(crbegin, rbegin);
    }
    
    @Test
    public void test_QSpan_auto() {
    	QList<QVector2D> list = QList.of(new QVector2D(1,2),new QVector2D(3,4));
    	QSpan<QVector2D> container = new QSpan<>(list);
    	assertTrue(container.begin() instanceof QSpan.Iterator);
    	assertTrue(container.constBegin() instanceof QSpan.ConstIterator);
    	assertTrue(container.reverseBegin() instanceof QSpan.ReverseIterator);
    	assertTrue(container.constReverseBegin() instanceof QSpan.ConstReverseIterator);
    	assertEquals(QMetaType.fromType(QVector2D.class), container.elementMetaType());
		assertEquals(QMetaType.fromType(QVector2D.class), container.begin().elementMetaType());
		assertEquals(QMetaType.fromType(QVector2D.class), container.constBegin().elementMetaType());
		assertEquals(QMetaType.fromType(QVector2D.class), container.reverseBegin().elementMetaType());
		assertEquals(QMetaType.fromType(QVector2D.class), container.constReverseBegin().elementMetaType());
    	assertEquals(list, new QList<>(container.constBegin(), container.constEnd()));
    	assertEquals(list, new QList<>(container.begin(), container.end()));
    	assertEquals(QList.of(new QVector2D(3,4),new QVector2D(1,2)), new QList<>(container.constReverseBegin(), container.constReverseEnd()));
		QSpan.Iterator<QVector2D> miter1 = container.begin();
		QSpan.ReverseIterator<QVector2D> riter1 = container.reverseBegin();
		QSpan.ConstReverseIterator<QVector2D> criter1 = container.constReverseBegin();
    	QSpan<QVector2D> clone = container.clone();
//    	assertTrue(container.isSharedWith(clone));
    	
		QSpan.ConstIterator<QVector2D> iter1 = container.constBegin();
		assertEquals(iter1, miter1);
		assertEquals(miter1, iter1);
		assertEquals(riter1, criter1);
		assertEquals(criter1, riter1);
		iter1.advance();
		QSpan.ConstIterator<QVector2D> iter2 = clone.constBegin();
		iter2.advance();
		assertEquals(iter1, iter2);
		List<QVector2D> test = new ArrayList<>();
		for(QSpan.ConstIterator<QVector2D> iter = container.constBegin(), end = container.constEnd(); !iter.equals(end); iter.advance()) {
			test.add(iter.get());
		}
		assertEquals(test, container.toList());
		assertTrue(miter1.isValid());
		clone.dispose();
		assertTrue(miter1.isValid());
		container.dispose();
		assertFalse(miter1.isValid());
		container = new QSpan<>(list);
		miter1 = container.begin();
		assertTrue(miter1.isValid());
		container.dispose();
		assertFalse(miter1.isValid());
		assertTrue(miter1.isDisposed());
		
		container = new QSpan<>(list);
		miter1 = container.begin();
		miter1.set(new QVector2D(5,5));
		iter1 = container.constBegin();
		assertEquals(new QVector2D(5,5), iter1.get());
		riter1 = container.reverseBegin();
		riter1.set(new QVector2D(6,6));
		assertEquals(new QVector2D(6,6), riter1.get());
		container.set(0, new QVector2D());
		assertTrue("mutable iterator invalid after concurrent mutation", miter1.isValid());
		assertTrue("const iterator invalid after concurrent mutation", iter1.isValid());
		
		container.set(0, new QVector2D(1,1));
		assertEquals(new QVector2D(1,1), new QSpan.ConstIterator<>(container.cbegin()).get());
		assertEquals(new QVector2D(6,6), new QSpan.ConstReverseIterator<>(container.crbegin()).get());
		assertEquals(new QVector2D(1,1), new QSpan.ConstIterator<>(container.begin()).get());
		assertEquals(new QVector2D(6,6), new QSpan.ConstReverseIterator<>(container.rbegin()).get());
		QSpan.Iterator<QVector2D> begin = container.begin();
		QSpan.ReverseIterator<QVector2D> rbegin = container.rbegin();
		QSpan.ConstIterator<QVector2D> cbegin = container.cbegin();
		QSpan.ConstReverseIterator<QVector2D> crbegin = container.crbegin();
		assertEquals(begin, cbegin);
		assertEquals(cbegin, begin);
		assertEquals(rbegin, crbegin);
		assertEquals(crbegin, rbegin);
		{
			Consumer<QSpan.ConstReverseIterator<QVector2D>> iteratorTest = keyIterator->{
				var iter = keyIterator.iterator();
				assertTrue(iter.hasNext());
				assertEquals(new QVector2D(6,6), iter.next());
				assertTrue(iter.hasNext());
				assertEquals(new QVector2D(1,1), iter.next());
			};
			iteratorTest.accept(container.crbegin());
			iteratorTest.accept(new QSpan.ConstReverseIterator<>(container.rbegin()));
		}
		{
			Consumer<QSpan.ReverseIterator<QVector2D>> iteratorTest = keyIterator->{
				var iter = keyIterator.iterator();
				assertTrue(iter.hasNext());
				assertEquals(new QVector2D(6,6), iter.next());
				assertTrue(iter.hasNext());
				assertEquals(new QVector2D(1,1), iter.next());
			};
			iteratorTest.accept(container.rbegin());
		}
		{
			Consumer<QSpan.ConstReverseIterator<QVector2D>> iteratorTest = keyIterator->{
				var iter = keyIterator.descendingIterator();
				assertTrue(iter.hasNext());
				assertEquals(new QVector2D(1,1), iter.next());
				assertTrue(iter.hasNext());
				assertEquals(new QVector2D(6,6), iter.next());
			};
			iteratorTest.accept(container.crend());
			iteratorTest.accept(new QSpan.ConstReverseIterator<>(container.rend()));
		}
		{
			Consumer<QSpan.ReverseIterator<QVector2D>> iteratorTest = keyIterator->{
				var iter = keyIterator.descendingIterator();
				assertTrue(iter.hasNext());
				assertEquals(new QVector2D(1,1), iter.next());
				assertTrue(iter.hasNext());
				assertEquals(new QVector2D(6,6), iter.next());
			};
			iteratorTest.accept(container.rend());
		}
		{
			Consumer<QSpan.ConstReverseIterator<QVector2D>> iteratorTest = keyIterator->{
				var iter = keyIterator.bidirectionalIterator();
				assertTrue(iter.hasNext());
				assertFalse(iter.hasPrevious());
				assertEquals(new QVector2D(6,6), iter.next());
				assertTrue(iter.hasNext());
				assertEquals(new QVector2D(1,1), iter.next());
				assertFalse(iter.hasNext());
				assertTrue(iter.hasPrevious());
				assertEquals(new QVector2D(1,1), iter.previous());
				try {
					iter.set(new QVector2D(99,99));
					fail("UnsupportedOperationException expected to be thrown");
				} catch (UnsupportedOperationException e) {
				}
			};
			iteratorTest.accept(container.crbegin());
			iteratorTest.accept(new QSpan.ConstReverseIterator<>(container.rbegin()));
		}
		{
			Consumer<QSpan.ReverseIterator<QVector2D>> iteratorTest = keyIterator->{
				var iter = keyIterator.bidirectionalIterator();
				assertTrue(iter.hasNext());
				assertFalse(iter.hasPrevious());
				assertEquals(new QVector2D(6,6), iter.next());
				assertTrue(iter.hasNext());
				assertEquals(new QVector2D(1,1), iter.next());
				assertFalse(iter.hasNext());
				assertTrue(iter.hasPrevious());
				assertEquals(new QVector2D(1,1), iter.previous());
				iter.set(new QVector2D(99,99));
			};
			iteratorTest.accept(container.rbegin());
		}
		{
			Consumer<QSpan.ConstIterator<QVector2D>> iteratorTest = keyIterator->{
				var iter = keyIterator.bidirectionalIterator();
				assertTrue(iter.hasNext());
				assertFalse(iter.hasPrevious());
				assertEquals(new QVector2D(1,1), iter.next());
				assertTrue(iter.hasNext());
				assertEquals(new QVector2D(99,99), iter.next());
				assertFalse(iter.hasNext());
				assertTrue(iter.hasPrevious());
				assertEquals(new QVector2D(99,99), iter.previous());
				try {
					iter.set(new QVector2D(6,6));
					fail("UnsupportedOperationException expected to be thrown");
				} catch (UnsupportedOperationException e) {
				}
			};
			iteratorTest.accept(container.cbegin());
			iteratorTest.accept(new QSpan.ConstIterator<>(container.begin()));
		}
		{
			Consumer<QSpan.Iterator<QVector2D>> iteratorTest = keyIterator->{
				var iter = keyIterator.bidirectionalIterator();
				assertTrue(iter.hasNext());
				assertFalse(iter.hasPrevious());
				assertEquals(new QVector2D(1,1), iter.next());
				assertTrue(iter.hasNext());
				assertEquals(new QVector2D(99,99), iter.next());
				assertFalse(iter.hasNext());
				assertTrue(iter.hasPrevious());
				assertEquals(new QVector2D(99,99), iter.previous());
				iter.set(new QVector2D(6,6));
			};
			iteratorTest.accept(container.begin());
		}
    }
    
    @Test
    public void test_QConstSpan_template() {
    	QList<String> list = QList.of("A", "B");
    	QConstSpan<String> container = QConstSpan.ofList(list);
    	assertTrue(container.constBegin() instanceof QConstSpan.ConstIterator);
    	assertTrue(container.constReverseBegin() instanceof QConstSpan.ConstReverseIterator);
    	assertEquals(QMetaType.fromType(String.class), container.elementMetaType());
		assertEquals(QMetaType.fromType(String.class), container.constBegin().elementMetaType());
    	assertEquals(list, new QList<>(container.constBegin(), container.constEnd()));
		QConstSpan<String> clone = container.clone();
//    	assertTrue(container.isSharedWith(clone));
    	
		QConstSpan.ConstIterator<String> iter1 = container.constBegin();
		iter1.advance();
		QConstSpan.ConstIterator<String> iter2 = clone.constBegin();
		iter2.advance();
		assertEquals(iter1, iter2);
		List<String> test = new ArrayList<>();
		for(QConstSpan.ConstIterator<String> iter = container.constBegin(), end = container.constEnd(); !iter.equals(end); iter.advance()) {
			test.add(iter.get());
		}
		assertEquals(test, container.toList());
		
		{
			Consumer<QConstSpan.ConstReverseIterator<String>> iteratorTest = keyIterator->{
				var iter = keyIterator.iterator();
				assertTrue(iter.hasNext());
				assertEquals("B", iter.next());
				assertTrue(iter.hasNext());
				assertEquals("A", iter.next());
			};
			iteratorTest.accept(container.crbegin());
		}
		{
			Consumer<QConstSpan.ConstReverseIterator<String>> iteratorTest = keyIterator->{
				var iter = keyIterator.descendingIterator();
				assertTrue(iter.hasNext());
				assertEquals("A", iter.next());
				assertTrue(iter.hasNext());
				assertEquals("B", iter.next());
			};
			iteratorTest.accept(container.crend());
		}
		{
			Consumer<QConstSpan.ConstReverseIterator<String>> iteratorTest = keyIterator->{
				var iter = keyIterator.bidirectionalIterator();
				assertTrue(iter.hasNext());
				assertFalse(iter.hasPrevious());
				assertEquals("B", iter.next());
				assertTrue(iter.hasNext());
				assertEquals("A", iter.next());
				assertFalse(iter.hasNext());
				assertTrue(iter.hasPrevious());
				assertEquals("A", iter.previous());
				try {
					iter.set("X");
					fail("UnsupportedOperationException expected to be thrown");
				} catch (UnsupportedOperationException e) {
				}
			};
			iteratorTest.accept(container.crbegin());
		}
		{
			Consumer<QConstSpan.ConstIterator<String>> iteratorTest = keyIterator->{
				var iter = keyIterator.bidirectionalIterator();
				assertTrue(iter.hasNext());
				assertFalse(iter.hasPrevious());
				assertEquals("A", iter.next());
				assertTrue(iter.hasNext());
				assertEquals("B", iter.next());
				assertFalse(iter.hasNext());
				assertTrue(iter.hasPrevious());
				assertEquals("B", iter.previous());
				try {
					iter.set("X");
					fail("UnsupportedOperationException expected to be thrown");
				} catch (UnsupportedOperationException e) {
				}
			};
			iteratorTest.accept(container.cbegin());
		}
    }
    
    @Test
    public void test_QConstSpan_auto() {
    	QList<QVector2D> list = QList.of(new QVector2D(1,2),new QVector2D(3,4));
    	QConstSpan<QVector2D> container = QConstSpan.ofList(list);
    	assertEquals(QMetaType.fromType(QVector2D.class), container.elementMetaType());
		assertEquals(QMetaType.fromType(QVector2D.class), container.constBegin().elementMetaType());
    	assertTrue(container.constBegin() instanceof QConstSpan.ConstIterator);
    	assertTrue(container.constReverseBegin() instanceof QConstSpan.ConstReverseIterator);
    	assertEquals(list, new QList<>(container.constBegin(), container.constEnd()));
		QConstSpan<QVector2D> clone = container.clone();
//    	assertTrue(container.isSharedWith(clone));
    	
		QConstSpan.ConstIterator<QVector2D> iter1 = container.constBegin();
		iter1.advance();
		QConstSpan.ConstIterator<QVector2D> iter2 = clone.constBegin();
		iter2.advance();
		assertEquals(iter1, iter2);
		List<QVector2D> test = new ArrayList<>();
		for(QConstSpan.ConstIterator<QVector2D> iter = container.constBegin(), end = container.constEnd(); !iter.equals(end); iter.advance()) {
			test.add(iter.get());
		}
		assertEquals(test, container.toList());
		
		{
			Consumer<QConstSpan.ConstReverseIterator<QVector2D>> iteratorTest = keyIterator->{
				var iter = keyIterator.iterator();
				assertTrue(iter.hasNext());
				assertEquals(new QVector2D(3,4), iter.next());
				assertTrue(iter.hasNext());
				assertEquals(new QVector2D(1,2), iter.next());
			};
			iteratorTest.accept(container.crbegin());
		}
		{
			Consumer<QConstSpan.ConstReverseIterator<QVector2D>> iteratorTest = keyIterator->{
				var iter = keyIterator.descendingIterator();
				assertTrue(iter.hasNext());
				assertEquals(new QVector2D(1,2), iter.next());
				assertTrue(iter.hasNext());
				assertEquals(new QVector2D(3,4), iter.next());
			};
			iteratorTest.accept(container.crend());
		}
		{
			Consumer<QConstSpan.ConstReverseIterator<QVector2D>> iteratorTest = keyIterator->{
				var iter = keyIterator.bidirectionalIterator();
				assertTrue(iter.hasNext());
				assertFalse(iter.hasPrevious());
				assertEquals(new QVector2D(3,4), iter.next());
				assertTrue(iter.hasNext());
				assertEquals(new QVector2D(1,2), iter.next());
				assertFalse(iter.hasNext());
				assertTrue(iter.hasPrevious());
				assertEquals(new QVector2D(1,2), iter.previous());
				try {
					iter.set(new QVector2D(99,99));
					fail("UnsupportedOperationException expected to be thrown");
				} catch (UnsupportedOperationException e) {
				}
			};
			iteratorTest.accept(container.crbegin());
		}
		{
			Consumer<QConstSpan.ConstIterator<QVector2D>> iteratorTest = keyIterator->{
				var iter = keyIterator.bidirectionalIterator();
				assertTrue(iter.hasNext());
				assertFalse(iter.hasPrevious());
				assertEquals(new QVector2D(1,2), iter.next());
				assertTrue(iter.hasNext());
				assertEquals(new QVector2D(3,4), iter.next());
				assertFalse(iter.hasNext());
				assertTrue(iter.hasPrevious());
				assertEquals(new QVector2D(3,4), iter.previous());
				try {
					iter.set(new QVector2D(99,99));
					fail("UnsupportedOperationException expected to be thrown");
				} catch (UnsupportedOperationException e) {
				}
			};
			iteratorTest.accept(container.cbegin());
		}
    }
    
    @Test
    public void test_QMap_auto() {
    	QMap<String,QPoint> container = QMap.of("A", new QPoint(1,2),"B", new QPoint(3,4));
		assertEquals(QMetaType.fromType(String.class), container.keyMetaType());
		assertEquals(QMetaType.fromType(QPoint.class), container.valueMetaType());
    	assertTrue(container.begin() instanceof QMap.Iterator);
    	assertTrue(container.constBegin() instanceof QMap.ConstIterator);
		assertEquals(QMetaType.fromType(String.class), container.begin().keyMetaType());
		assertEquals(QMetaType.fromType(QPoint.class), container.begin().valueMetaType());
		assertEquals(QMetaType.fromType(String.class), container.constBegin().keyMetaType());
		assertEquals(QMetaType.fromType(QPoint.class), container.constBegin().valueMetaType());
		assertEquals(QMetaType.fromType(String.class), container.keyBegin().elementMetaType());
		assertEquals(QMetaType.fromType(QPair.class, QMetaType.fromType(String.class), QMetaType.fromType(QPoint.class)), container.keyValueBegin().elementMetaType());
		assertEquals(QMetaType.fromType(QPair.class, QMetaType.fromType(String.class), QMetaType.fromType(QPoint.class)), container.constKeyValueBegin().elementMetaType());
    	assertEquals(container, new QMap<>(container.constBegin(), container.constEnd()));
    	assertEquals(container, new QMap<>(container.begin(), container.end()));

		assertTrue(container.constKeyValueBegin().isValid());
		assertTrue(container.keyValueBegin().isValid());
		assertEquals(container.keyValueBegin(), container.constKeyValueBegin());
		
    	QMap.Iterator<String,QPoint> miter1 = container.begin();
    	QMap<String,QPoint> clone = container.clone();
    	assertTrue(container.isSharedWith(clone));
    	
		QMap.ConstIterator<String,QPoint> iter1 = container.constBegin();
		assertEquals(iter1, miter1);
		assertEquals(miter1, iter1);
		iter1.advance();
		QMap.ConstIterator<String,QPoint> iter2 = clone.constBegin();
		iter2.advance();
		assertEquals(iter1, iter2);
		Map<String,QPoint> test = new TreeMap<>();
		for(QMap.ConstIterator<String,QPoint> iter = container.constBegin(), end = container.constEnd(); !iter.equals(end); iter.advance()) {
			test.put(iter.key(), iter.value());
		}
		assertEquals(test, container);
		assertTrue(miter1.isValid());
		clone.dispose();
		assertTrue(miter1.isValid());
		container.clear();
		assertFalse(miter1.isValid());
		assertEquals(new QPoint(3,4), iter1.value());
		assertEquals(new QPoint(3,4), iter2.value());
		assertEquals(iter1, iter2);
		iter1.advance();
		assertFalse(iter1.isValid());
		container = QMap.of("A", new QPoint(1,2),"B", new QPoint(3,4));
		miter1 = container.begin();
		assertTrue(miter1.isValid());
		container.dispose();
		assertFalse(miter1.isValid());
		assertTrue(miter1.isDisposed());
		
		container = QMap.of("A", new QPoint(1,2),"B", new QPoint(3,4));
		miter1 = container.begin();
		miter1.setValue(new QPoint(5,5));
		iter1 = container.constBegin();
		assertEquals(new QPoint(5,5), iter1.value());
		assertEquals(new QPoint(5,5), new QMap.ConstIterator<>(iter1).value());
		container.insert("0", new QPoint());
		assertFalse("mutable iterator not invalid after concurrent mutation", miter1.isValid());
		assertTrue("const iterator invalid after concurrent mutation", iter1.isValid());
		QList<QPoint> lv = QList.ofValues(container.constBegin(), container.constEnd());
		QList<String> lk = QList.ofKeys(container.constBegin(), container.constEnd());
		QList<QPair<String, QPoint>> lkv = QList.ofKeyValuePairs(container.constBegin(), container.constEnd());
		assertEquals(QMetaType.fromType(QPoint.class), lv.elementMetaType());
		assertEquals(QMetaType.fromType(String.class), lk.elementMetaType());
		assertEquals(QMetaType.fromType(QPair.class, QMetaType.fromType(String.class), QMetaType.fromType(QPoint.class)), lkv.elementMetaType());
		assertEquals(QList.of(new QPoint(0,0), new QPoint(5,5), new QPoint(3,4)), lv);
		assertEquals(QList.of("0", "A", "B"), lk);
		assertEquals(QList.of(new QPair<>("0", new QPoint(0,0)), new QPair<>("A", new QPoint(5,5)), new QPair<>("B", new QPoint(3,4))), lkv);
		
		container = QMap.of("Z", new QPoint(1,2), "Y", new QPoint(1,2));
		{
			String _A, _B;
			if("Z".equals(container.keyBegin().get())) {
				_A = "Z";
				_B = "Y";
			}else {
				_A = "Y";
				_B = "Z";
			}
			assertEquals(QPair.pair(_A, new QPoint(1,2)), container.constKeyValueBegin().get());
			assertEquals(QList.of(QPair.pair(_A, new QPoint(1,2)), QPair.pair(_B, new QPoint(1,2))), QList.of(container.constKeyValueBegin(), container.constKeyValueEnd()));
			assertEquals(QList.of(_A, _B), new QList<>(container.keyBegin(), container.keyEnd()));
			
			assertEquals(_A, new QMap.ConstIterator<>(container.cbegin()).key());
			assertEquals(new QPair<>(_A, new QPoint(1,2)), new QMap.ConstKeyValueIterator<>(container.constKeyValueBegin()).get());
			assertEquals(_A, new QMap.ConstIterator<>(container.begin()).key());
			assertEquals(new QPair<>(_A, new QPoint(1,2)), new QMap.ConstKeyValueIterator<>(container.keyValueBegin()).get());
			assertEquals(new QPair<>(_A, new QPoint(1,2)), new QMap.ConstKeyValueIterator<>(container.constBegin()).get());
			assertEquals(new QPair<>(_A, new QPoint(1,2)), new QMap.ConstKeyValueIterator<>(container.begin()).get());
			assertEquals(new QPair<>(_A, new QPoint(1,2)), new QMap.KeyValueIterator<>(container.begin()).get());
			assertEquals(_A, new QMap.KeyIterator<>(container.constBegin()).get());
			assertEquals(_A, new QMap.KeyIterator<>(container.begin()).get());
			{
				Consumer<QMap.KeyIterator<String>> iteratorTest = keyIterator->{
					var iter = keyIterator.iterator();
					assertTrue(iter.hasNext());
					assertEquals(_A, iter.next());
					assertTrue(iter.hasNext());
					assertEquals(_B, iter.next());
				};
				iteratorTest.accept(container.keyBegin());
				iteratorTest.accept(new QMap.KeyIterator<>(container.constBegin()));
				iteratorTest.accept(new QMap.KeyIterator<>(container.begin()));
			}
			{
				Consumer<QMap.ConstKeyValueIterator<String,QPoint>> iteratorTest = keyValueIterator->{
					var iter = keyValueIterator.iterator();
					assertTrue(iter.hasNext());
					assertEquals(new QPair<>(_A, new QPoint(1,2)), iter.next());
					assertTrue(iter.hasNext());
					assertEquals(new QPair<>(_B, new QPoint(1,2)), iter.next());
				};
				iteratorTest.accept(container.constKeyValueBegin());
				iteratorTest.accept(new QMap.ConstKeyValueIterator<>(container.constBegin()));
				iteratorTest.accept(new QMap.ConstKeyValueIterator<>(container.begin()));
			}
			{
				Consumer<QMap.KeyValueIterator<String,QPoint>> iteratorTest = keyValueIterator->{
					var iter = keyValueIterator.iterator();
					assertTrue(iter.hasNext());
					assertEquals(new QPair<>(_A, new QPoint(1,2)), iter.next());
					assertTrue(iter.hasNext());
					assertEquals(new QPair<>(_B, new QPoint(1,2)), iter.next());
				};
				iteratorTest.accept(container.keyValueBegin());
				iteratorTest.accept(new QMap.KeyValueIterator<>(container.begin()));
			}
		}
		
		QMap.Iterator<String,QPoint> begin = container.begin();
		QMap.KeyValueIterator<String,QPoint> kvbegin = container.keyValueBegin();
		QMap.ConstIterator<String,QPoint> cbegin = container.cbegin();
		QMap.ConstKeyValueIterator<String,QPoint> ckvbegin = container.constKeyValueBegin();
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

		container.keyValueBegin().set(new QPoint(99,99));
		assertEquals(new QPoint(99,99), container.constBegin().value());
    }
    
    @Test
    public void test_QHash_auto() {
    	QHash<String,QPoint> container = QHash.of("A", new QPoint(1,2),"B", new QPoint(1,2));
		assertEquals(QMetaType.fromType(String.class), container.keyMetaType());
		assertEquals(QMetaType.fromType(QPoint.class), container.valueMetaType());
    	assertTrue(container.begin() instanceof QHash.Iterator);
    	assertTrue(container.constBegin() instanceof QHash.ConstIterator);
		assertEquals(QMetaType.fromType(String.class), container.begin().keyMetaType());
		assertEquals(QMetaType.fromType(QPoint.class), container.begin().valueMetaType());
		assertEquals(QMetaType.fromType(String.class), container.constBegin().keyMetaType());
		assertEquals(QMetaType.fromType(QPoint.class), container.constBegin().valueMetaType());
		assertEquals(QMetaType.fromType(String.class), container.keyBegin().elementMetaType());
		assertEquals(QMetaType.fromType(QPair.class, QMetaType.fromType(String.class), QMetaType.fromType(QPoint.class)), container.keyValueBegin().elementMetaType());
		assertEquals(QMetaType.fromType(QPair.class, QMetaType.fromType(String.class), QMetaType.fromType(QPoint.class)), container.constKeyValueBegin().elementMetaType());

		assertTrue(container.constKeyValueBegin().isValid());
		assertTrue(container.keyValueBegin().isValid());
		assertEquals(container.keyValueBegin(), container.constKeyValueBegin());
		
    	assertEquals(container, new QHash<>(container.constBegin(), container.constEnd()));
    	assertEquals(container, new QHash<>(container.begin(), container.end()));
    	QHash.Iterator<String,QPoint> miter1 = container.begin();
    	QHash<String,QPoint> clone = container.clone();
    	assertTrue(container.isSharedWith(clone));
    	
    	QHash.ConstIterator<String,QPoint> iter1 = container.constBegin();
		assertEquals(iter1, miter1);
		assertEquals(miter1, iter1);
		iter1.advance();
		QHash.ConstIterator<String,QPoint> iter2 = clone.constBegin();
		iter2.advance();
		assertEquals(iter1, iter2);
		Map<String,QPoint> test = new HashMap<>();
		for(QHash.ConstIterator<String,QPoint> iter = container.constBegin(), end = container.constEnd(); !iter.equals(end); iter.advance()) {
			test.put(iter.key(), iter.value());
		}
		assertEquals(test, container);
		assertTrue(miter1.isValid());
		clone.dispose();
		assertTrue(miter1.isValid());
		container.clear();
		assertFalse(miter1.isValid());
		assertEquals(new QPoint(1,2), iter1.value());
		assertEquals(new QPoint(1,2), iter2.value());
		assertEquals(iter1, iter2);
		iter1.advance();
		assertFalse(iter1.isValid());
		container = QHash.of("A", new QPoint(1,2),"B", new QPoint(1,2));
		miter1 = container.begin();
		assertTrue(miter1.isValid());
		container.dispose();
		assertFalse(miter1.isValid());
		assertTrue(miter1.isDisposed());
		
		container = QHash.of("Z", new QPoint(1,2), "Y", new QPoint(1,2));
		{
			String _A, _B;
			if("Z".equals(container.keyBegin().get())) {
				_A = "Z";
				_B = "Y";
			}else {
				_A = "Y";
				_B = "Z";
			}
			assertEquals(QPair.pair(_A, new QPoint(1,2)), container.constKeyValueBegin().get());
			assertEquals(QList.of(QPair.pair(_A, new QPoint(1,2)), QPair.pair(_B, new QPoint(1,2))), QList.of(container.constKeyValueBegin(), container.constKeyValueEnd()));
			assertEquals(QList.of(_A, _B), new QList<>(container.keyBegin(), container.keyEnd()));
			
			assertEquals(_A, new QHash.ConstIterator<>(container.cbegin()).key());
			assertEquals(new QPair<>(_A, new QPoint(1,2)), new QHash.ConstKeyValueIterator<>(container.constKeyValueBegin()).get());
			assertEquals(_A, new QHash.ConstIterator<>(container.begin()).key());
			assertEquals(new QPair<>(_A, new QPoint(1,2)), new QHash.ConstKeyValueIterator<>(container.keyValueBegin()).get());
			assertEquals(new QPair<>(_A, new QPoint(1,2)), new QHash.ConstKeyValueIterator<>(container.begin()).get());
			assertEquals(new QPair<>(_A, new QPoint(1,2)), new QHash.ConstKeyValueIterator<>(container.constBegin()).get());
			assertEquals(new QPair<>(_A, new QPoint(1,2)), new QHash.KeyValueIterator<>(container.begin()).get());
			assertEquals(_A, new QHash.KeyIterator<>(container.constBegin()).get());
			assertEquals(_A, new QHash.KeyIterator<>(container.begin()).get());
			assertEquals(QMetaType.fromType(String.class), new QHash.KeyIterator<>(container.constBegin()).elementMetaType());
			assertEquals(QMetaType.fromType(String.class), new QHash.KeyIterator<>(container.begin()).elementMetaType());
			{
				Consumer<QHash.KeyIterator<String>> iteratorTest = keyIterator->{
					var iter = keyIterator.iterator();
					assertTrue(iter.hasNext());
					assertEquals(_A, iter.next());
					assertTrue(iter.hasNext());
					assertEquals(_B, iter.next());
				};
				iteratorTest.accept(container.keyBegin());
				iteratorTest.accept(new QHash.KeyIterator<>(container.constBegin()));
				iteratorTest.accept(new QHash.KeyIterator<>(container.begin()));
			}
			{
				Consumer<QHash.ConstKeyValueIterator<String,QPoint>> iteratorTest = keyValueIterator->{
					var iter = keyValueIterator.iterator();
					assertTrue(iter.hasNext());
					assertEquals(new QPair<>(_A, new QPoint(1,2)), iter.next());
					assertTrue(iter.hasNext());
					assertEquals(new QPair<>(_B, new QPoint(1,2)), iter.next());
				};
				iteratorTest.accept(container.constKeyValueBegin());
				iteratorTest.accept(new QHash.ConstKeyValueIterator<>(container.constBegin()));
				iteratorTest.accept(new QHash.ConstKeyValueIterator<>(container.begin()));
			}
			{
				Consumer<QHash.KeyValueIterator<String,QPoint>> iteratorTest = keyValueIterator->{
					var iter = keyValueIterator.iterator();
					assertTrue(iter.hasNext());
					assertEquals(new QPair<>(_A, new QPoint(1,2)), iter.next());
					assertTrue(iter.hasNext());
					assertEquals(new QPair<>(_B, new QPoint(1,2)), iter.next());
				};
				iteratorTest.accept(container.keyValueBegin());
				iteratorTest.accept(new QHash.KeyValueIterator<>(container.begin()));
			}
		}
		miter1 = container.begin();
		miter1.setValue(new QPoint(5,5));
		iter1 = container.constBegin();
		assertEquals(new QPoint(5,5), iter1.value());
		for (int i = 0; i < 128; i++) {
			container.insert(""+i, new QPoint());
		}
		assertFalse("mutable iterator not invalid after concurrent mutation", miter1.isValid());
		assertTrue("const iterator invalid after concurrent mutation", iter1.isValid());
		
		container = QHash.of("Z", new QPoint(1,2), "Y", new QPoint(1,2));
		QHash.Iterator<String,QPoint> begin = container.begin();
		QHash.KeyValueIterator<String,QPoint> kvbegin = container.keyValueBegin();
		QHash.ConstIterator<String,QPoint> cbegin = container.cbegin();
		QHash.ConstKeyValueIterator<String,QPoint> ckvbegin = container.constKeyValueBegin();
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

		container.keyValueBegin().set(new QPoint(99,99));
		assertEquals(new QPoint(99,99), container.constBegin().value());
    }
    
    @Test
    public void test_QMultiMap_auto() {
    	QMultiMap<String,QPoint> container = QMultiMap.of("A", new QPoint(1,2), "B", new QPoint(3,4));
		assertEquals(QMetaType.fromType(String.class), container.keyMetaType());
		assertEquals(QMetaType.fromType(QPoint.class), container.valueMetaType());
    	assertTrue(container.begin() instanceof QMultiMap.Iterator);
    	assertTrue(container.constBegin() instanceof QMultiMap.ConstIterator);
		assertEquals(QMetaType.fromType(String.class), container.begin().keyMetaType());
		assertEquals(QMetaType.fromType(QPoint.class), container.begin().valueMetaType());
		assertEquals(QMetaType.fromType(String.class), container.constBegin().keyMetaType());
		assertEquals(QMetaType.fromType(QPoint.class), container.constBegin().valueMetaType());
		assertEquals(QMetaType.fromType(String.class), container.keyBegin().elementMetaType());
		assertEquals(QMetaType.fromType(QPair.class, QMetaType.fromType(String.class), QMetaType.fromType(QPoint.class)), container.keyValueBegin().elementMetaType());
		assertEquals(QMetaType.fromType(QPair.class, QMetaType.fromType(String.class), QMetaType.fromType(QPoint.class)), container.constKeyValueBegin().elementMetaType());
    	assertEquals(container, new QMultiMap<>(container.constBegin(), container.constEnd()));
    	assertEquals(container, new QMultiMap<>(container.begin(), container.end()));

		assertTrue(container.constKeyValueBegin().isValid());
		assertTrue(container.keyValueBegin().isValid());
		assertEquals(container.keyValueBegin(), container.constKeyValueBegin());
		
    	QMultiMap.Iterator<String,QPoint> miter1 = container.begin();
    	QMultiMap<String,QPoint> clone = container.clone();
    	assertTrue(container.isSharedWith(clone));
    	
		QMultiMap.ConstIterator<String,QPoint> iter1 = container.constBegin();
		assertEquals(iter1, miter1);
		assertEquals(miter1, iter1);
		iter1.advance();
		QMultiMap.ConstIterator<String,QPoint> iter2 = clone.constBegin();
		iter2.advance();
		assertEquals(iter1, iter2);
		Map<String,List<QPoint>> test = new TreeMap<>();
		for(QMultiMap.ConstIterator<String,QPoint> iter = container.constBegin(), end = container.constEnd(); !iter.equals(end); iter.advance()) {
			test.computeIfAbsent(iter.key(), k->new ArrayList<>()).add(iter.value());
		}
		assertEquals(test, container);
		assertTrue(miter1.isValid());
		clone.dispose();
		assertTrue(miter1.isValid());
		container.clear();
		assertFalse(miter1.isValid());
		assertEquals(new QPoint(3,4), iter1.value());
		assertEquals(new QPoint(3,4), iter2.value());
		assertEquals(iter1, iter2);
		iter1.advance();
		assertFalse(iter1.isValid());
		container = QMultiMap.of("A", new QPoint(1,2),"B", new QPoint(3,4));
		miter1 = container.begin();
		assertTrue(miter1.isValid());
		container.dispose();
		assertFalse(miter1.isValid());
		assertTrue(miter1.isDisposed());
		
		container = QMultiMap.of("A", new QPoint(1,2),"B", new QPoint(3,4));
		miter1 = container.begin();
		miter1.setValue(new QPoint(5,5));
		iter1 = container.constBegin();
		assertEquals(new QPoint(5,5), iter1.value());
		container.insert("0", new QPoint());
		assertFalse("mutable iterator not invalid after concurrent mutation", miter1.isValid());
		assertTrue("const iterator invalid after concurrent mutation", iter1.isValid());
		
		container = QMultiMap.of("Z", new QPoint(1,2), "Y", new QPoint(1,2));
		{
			String _A, _B;
			if("Z".equals(container.keyBegin().get())) {
				_A = "Z";
				_B = "Y";
			}else {
				_A = "Y";
				_B = "Z";
			}
			assertEquals(QPair.pair(_A, new QPoint(1,2)), container.constKeyValueBegin().get());
			assertEquals(QList.of(QPair.pair(_A, new QPoint(1,2)), QPair.pair(_B, new QPoint(1,2))), QList.of(container.constKeyValueBegin(), container.constKeyValueEnd()));
			assertEquals(QList.of(_A, _B), new QList<>(container.keyBegin(), container.keyEnd()));
			
			assertEquals(_A, new QMultiMap.ConstIterator<>(container.cbegin()).key());
			assertEquals(new QPair<>(_A, new QPoint(1,2)), new QMultiMap.ConstKeyValueIterator<>(container.constKeyValueBegin()).get());
			assertEquals(_A, new QMultiMap.ConstIterator<>(container.begin()).key());
			assertEquals(new QPair<>(_A, new QPoint(1,2)), new QMultiMap.ConstKeyValueIterator<>(container.keyValueBegin()).get());
			assertEquals(new QPair<>(_A, new QPoint(1,2)), new QMultiMap.ConstKeyValueIterator<>(container.constBegin()).get());
			assertEquals(new QPair<>(_A, new QPoint(1,2)), new QMultiMap.ConstKeyValueIterator<>(container.begin()).get());
			assertEquals(new QPair<>(_A, new QPoint(1,2)), new QMultiMap.KeyValueIterator<>(container.begin()).get());
			assertEquals(_A, new QMultiMap.KeyIterator<>(container.constBegin()).get());
			assertEquals(_A, new QMultiMap.KeyIterator<>(container.begin()).get());
			{
				Consumer<QMultiMap.KeyIterator<String>> iteratorTest = keyIterator->{
					var iter = keyIterator.iterator();
					assertTrue(iter.hasNext());
					assertEquals(_A, iter.next());
					assertTrue(iter.hasNext());
					assertEquals(_B, iter.next());
				};
				iteratorTest.accept(container.keyBegin());
				iteratorTest.accept(new QMultiMap.KeyIterator<>(container.constBegin()));
				iteratorTest.accept(new QMultiMap.KeyIterator<>(container.begin()));
			}
			{
				Consumer<QMultiMap.ConstKeyValueIterator<String,QPoint>> iteratorTest = keyValueIterator->{
					var iter = keyValueIterator.iterator();
					assertTrue(iter.hasNext());
					assertEquals(new QPair<>(_A, new QPoint(1,2)), iter.next());
					assertTrue(iter.hasNext());
					assertEquals(new QPair<>(_B, new QPoint(1,2)), iter.next());
				};
				iteratorTest.accept(container.constKeyValueBegin());
				iteratorTest.accept(new QMultiMap.ConstKeyValueIterator<>(container.constBegin()));
				iteratorTest.accept(new QMultiMap.ConstKeyValueIterator<>(container.begin()));
			}
			{
				Consumer<QMultiMap.KeyValueIterator<String,QPoint>> iteratorTest = keyValueIterator->{
					var iter = keyValueIterator.iterator();
					assertTrue(iter.hasNext());
					assertEquals(new QPair<>(_A, new QPoint(1,2)), iter.next());
					assertTrue(iter.hasNext());
					assertEquals(new QPair<>(_B, new QPoint(1,2)), iter.next());
				};
				iteratorTest.accept(container.keyValueBegin());
				iteratorTest.accept(new QMultiMap.KeyValueIterator<>(container.begin()));
			}
		}
		
		QMultiMap.Iterator<String,QPoint> begin = container.begin();
		QMultiMap.KeyValueIterator<String,QPoint> kvbegin = container.keyValueBegin();
		QMultiMap.ConstIterator<String,QPoint> cbegin = container.cbegin();
		QMultiMap.ConstKeyValueIterator<String,QPoint> ckvbegin = container.constKeyValueBegin();
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

		container.keyValueBegin().set(new QPoint(99,99));
		assertEquals(new QPoint(99,99), container.constBegin().value());
    }
    
    @Test
    public void test_QMultiHash_auto() {
    	QMultiHash<String,QPoint> container = QMultiHash.of("A", new QPoint(1,2), "B", new QPoint(1,2));
		assertEquals(QMetaType.fromType(String.class), container.keyMetaType());
		assertEquals(QMetaType.fromType(QPoint.class), container.valueMetaType());
    	assertTrue(container.begin() instanceof QMultiHash.Iterator);
    	assertTrue(container.constBegin() instanceof QMultiHash.ConstIterator);
		assertEquals(QMetaType.fromType(String.class), container.begin().keyMetaType());
		assertEquals(QMetaType.fromType(QPoint.class), container.begin().valueMetaType());
		assertEquals(QMetaType.fromType(String.class), container.constBegin().keyMetaType());
		assertEquals(QMetaType.fromType(QPoint.class), container.constBegin().valueMetaType());
		assertEquals(QMetaType.fromType(String.class), container.keyBegin().elementMetaType());
		assertEquals(QMetaType.fromType(QPair.class, QMetaType.fromType(String.class), QMetaType.fromType(QPoint.class)), container.keyValueBegin().elementMetaType());
		assertEquals(QMetaType.fromType(QPair.class, QMetaType.fromType(String.class), QMetaType.fromType(QPoint.class)), container.constKeyValueBegin().elementMetaType());
    	assertEquals(container, new QMultiHash<>(container.constBegin(), container.constEnd()));
    	assertEquals(container, new QMultiHash<>(container.begin(), container.end()));

		assertTrue(container.constKeyValueBegin().isValid());
		assertTrue(container.keyValueBegin().isValid());
		assertEquals(container.keyValueBegin(), container.constKeyValueBegin());
		
    	QMultiHash.Iterator<String,QPoint> miter1 = container.begin();
    	QMultiHash<String,QPoint> clone = container.clone();
    	assertTrue(container.isSharedWith(clone));
    	
    	QMultiHash.ConstIterator<String,QPoint> iter1 = container.constBegin();
		assertEquals(iter1, miter1);
		assertEquals(miter1, iter1);
		iter1.advance();
		QMultiHash.ConstIterator<String,QPoint> iter2 = clone.constBegin();
		iter2.advance();
		assertEquals(iter1, iter2);
		Map<String,List<QPoint>> test = new HashMap<>();
		for(QMultiHash.ConstIterator<String,QPoint> iter = container.constBegin(), end = container.constEnd(); !iter.equals(end); iter.advance()) {
			test.computeIfAbsent(iter.key(), k->new ArrayList<>()).add(iter.value());
		}
		assertEquals(test, container);
		assertTrue(miter1.isValid());
		clone.dispose();
		assertTrue(miter1.isValid());
		container.clear();
		assertFalse(miter1.isValid());
		assertEquals(new QPoint(1,2), iter1.value());
		assertEquals(new QPoint(1,2), iter2.value());
		assertEquals(iter1, iter2);
		iter1.advance();
		assertFalse(iter1.isValid());
		container = QMultiHash.of("A", new QPoint(1,2),"B", new QPoint(1,2));
		miter1 = container.begin();
		assertTrue(miter1.isValid());
		container.dispose();
		assertFalse(miter1.isValid());
		assertTrue(miter1.isDisposed());
		
		container = QMultiHash.of("-1", new QPoint(1,2),"-2", new QPoint(1,2));
		miter1 = container.begin();
		miter1.setValue(new QPoint(5,5));
		iter1 = container.constBegin();
		assertEquals(new QPoint(5,5), iter1.value());
		for (int i = 0; i < 128; i++) {
			container.insert(""+i, new QPoint());
		}
		assertFalse("mutable iterator not invalid after concurrent mutation", miter1.isValid());
		assertTrue("const iterator invalid after concurrent mutation", iter1.isValid());
		
		container = QMultiHash.of("Z", new QPoint(1,2), "Y", new QPoint(1,2));
		{
			String _A, _B;
			if("Z".equals(container.keyBegin().get())) {
				_A = "Z";
				_B = "Y";
			}else {
				_A = "Y";
				_B = "Z";
			}
			assertEquals(QPair.pair(_A, new QPoint(1,2)), container.constKeyValueBegin().get());
			assertEquals(QList.of(QPair.pair(_A, new QPoint(1,2)), QPair.pair(_B, new QPoint(1,2))), QList.of(container.constKeyValueBegin(), container.constKeyValueEnd()));
			assertEquals(QList.of(_A, _B), new QList<>(container.keyBegin(), container.keyEnd()));
			
			assertEquals(_A, new QMultiHash.ConstIterator<>(container.cbegin()).key());
			assertEquals(new QPair<>(_A, new QPoint(1,2)), new QMultiHash.ConstKeyValueIterator<>(container.constKeyValueBegin()).get());
			assertEquals(_A, new QMultiHash.ConstIterator<>(container.begin()).key());
			assertEquals(new QPair<>(_A, new QPoint(1,2)), new QMultiHash.ConstKeyValueIterator<>(container.keyValueBegin()).get());
			assertEquals(new QPair<>(_A, new QPoint(1,2)), new QMultiHash.ConstKeyValueIterator<>(container.constBegin()).get());
			assertEquals(new QPair<>(_A, new QPoint(1,2)), new QMultiHash.ConstKeyValueIterator<>(container.begin()).get());
			assertEquals(new QPair<>(_A, new QPoint(1,2)), new QMultiHash.KeyValueIterator<>(container.begin()).get());
			assertEquals(_A, new QMultiHash.KeyIterator<>(container.constBegin()).get());
			assertEquals(_A, new QMultiHash.KeyIterator<>(container.begin()).get());
			{
				Consumer<QMultiHash.KeyIterator<String>> iteratorTest = keyIterator->{
					var iter = keyIterator.iterator();
					assertTrue(iter.hasNext());
					assertEquals(_A, iter.next());
					assertTrue(iter.hasNext());
					assertEquals(_B, iter.next());
				};
				iteratorTest.accept(container.keyBegin());
				iteratorTest.accept(new QMultiHash.KeyIterator<>(container.constBegin()));
				iteratorTest.accept(new QMultiHash.KeyIterator<>(container.begin()));
			}
			{
				Consumer<QMultiHash.ConstKeyValueIterator<String,QPoint>> iteratorTest = keyValueIterator->{
					var iter = keyValueIterator.iterator();
					assertTrue(iter.hasNext());
					assertEquals(new QPair<>(_A, new QPoint(1,2)), iter.next());
					assertTrue(iter.hasNext());
					assertEquals(new QPair<>(_B, new QPoint(1,2)), iter.next());
				};
				iteratorTest.accept(container.constKeyValueBegin());
				iteratorTest.accept(new QMultiHash.ConstKeyValueIterator<>(container.constBegin()));
				iteratorTest.accept(new QMultiHash.ConstKeyValueIterator<>(container.begin()));
			}
			{
				Consumer<QMultiHash.KeyValueIterator<String,QPoint>> iteratorTest = keyValueIterator->{
					var iter = keyValueIterator.iterator();
					assertTrue(iter.hasNext());
					assertEquals(new QPair<>(_A, new QPoint(1,2)), iter.next());
					assertTrue(iter.hasNext());
					assertEquals(new QPair<>(_B, new QPoint(1,2)), iter.next());
				};
				iteratorTest.accept(container.keyValueBegin());
				iteratorTest.accept(new QMultiHash.KeyValueIterator<>(container.begin()));
			}
		}
		
		QMultiHash.Iterator<String,QPoint> begin = container.begin();
		QMultiHash.KeyValueIterator<String,QPoint> kvbegin = container.keyValueBegin();
		QMultiHash.ConstIterator<String,QPoint> cbegin = container.cbegin();
		QMultiHash.ConstKeyValueIterator<String,QPoint> ckvbegin = container.constKeyValueBegin();
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

		container.keyValueBegin().set(new QPoint(99,99));
		assertEquals(new QPoint(99,99), container.constBegin().value());
    }
    
    @Test
    public void test_QString() {
    	QString container = new QString("AB");
    	assertTrue(container.begin() instanceof QString.Iterator);
    	assertTrue(container.constBegin() instanceof QString.ConstIterator);
    	assertTrue(container.reverseBegin() instanceof QString.ReverseIterator);
    	assertTrue(container.constReverseBegin() instanceof QString.ConstReverseIterator);
		assertEquals(new QMetaType(QMetaType.Type.QChar), container.begin().elementMetaType());
		assertEquals(new QMetaType(QMetaType.Type.QChar), container.cbegin().elementMetaType());
    	assertEquals(QList.of('A', 'B'), new QList<>(container.cbegin(), container.cend()));
    	assertEquals(QList.of('A', 'B'), new QList<>(container.begin(), container.end()));
    	assertEquals(QList.of('B', 'A'), new QList<>(container.rbegin(), container.rend()));
    	assertEquals(QList.of('B', 'A'), new QList<>(container.crbegin(), container.crend()));

		assertTrue(container.constReverseBegin().isValid());
		assertTrue(container.reverseBegin().isValid());
		assertEquals(container.reverseBegin(), container.constReverseBegin());
		
    	QString.Iterator miter1 = container.begin();
    	QString clone = container.clone();
    	assertTrue(container.isSharedWith(clone));
    	
    	QString.ConstIterator iter1 = container.cbegin();
		assertEquals(iter1, miter1);
		assertEquals(miter1, iter1);
		iter1.advance();
		QString.ConstIterator iter2 = clone.cbegin();
		iter2.advance();
		assertEquals(iter1, iter2);
		StringBuilder test = new StringBuilder();
		for(QString.ConstIterator iter = container.cbegin(), end = container.cend(); !iter.equals(end); iter.advance()) {
			test.append(iter.get());
		}
		assertEquals(test.toString(), container.toString());
		test = new StringBuilder();
		for(QString.ConstReverseIterator iter = container.crbegin(), end = container.crend(); !iter.equals(end); iter.advance()) {
			test.append(iter.get());
		}
		assertEquals(test.toString(), "BA");
		clone.dispose();
		assertTrue(miter1.isValid());
		container.clear();
		assertFalse(miter1.isValid());
		container = new QString("AB");
		miter1 = container.begin();
		assertTrue(miter1.isValid());
		container.dispose();
		assertFalse(miter1.isValid());
		assertTrue(miter1.isDisposed());
		
		container = new QString("AB");
		miter1 = container.begin();
		miter1.set('X');
		iter1 = container.cbegin();
		assertEquals('X', iter1.getAsChar());
		assertEquals('X', container.begin().getAsChar());
		assertEquals('B', container.constReverseBegin().getAsChar());
		assertEquals('B', container.reverseBegin().getAsChar());
		container.insert(0, "C");
		assertFalse("mutable iterator not invalid after concurrent mutation", miter1.isValid());
		assertTrue("const iterator invalid after concurrent mutation", iter1.isValid());
		
		container = new QString("ABCDE");
		QString.Iterator erasedIter = container.erase(container.cbegin());
		assertTrue(erasedIter.isValid());
		assertEquals('B', erasedIter.getAsChar());
		
		container = new QString("ABCDE");
		QString.ConstIterator it1 = container.cbegin();
		it1.advance();
		QString.ConstIterator it2 = it1.clone();
		it2.advance();
		erasedIter = container.erase(it1, it2);
		assertTrue(erasedIter.isValid());
		assertEquals('C', erasedIter.getAsChar());
		assertEquals("ACDE", container.toString());
		
		assertEquals('A', new QString.ConstIterator(container.cbegin()).getAsChar());
		assertEquals('E', new QString.ConstReverseIterator(container.crbegin()).getAsChar());
		assertEquals('A', new QString.ConstIterator(container.begin()).getAsChar());
		assertEquals('E', new QString.ConstReverseIterator(container.rbegin()).getAsChar());
		QString.Iterator begin = container.begin();
		QString.ReverseIterator rbegin = container.rbegin();
		QString.ConstIterator cbegin = container.cbegin();
		QString.ConstReverseIterator crbegin = container.crbegin();
		assertEquals(begin, cbegin);
		assertEquals(cbegin, begin);
		assertEquals(rbegin, crbegin);
		assertEquals(crbegin, rbegin);
    }
    
    @Test
    public void test_QByteArray() {
    	QByteArray container = new QByteArray("AB");
    	assertTrue(container.begin() instanceof QByteArray.Iterator);
    	assertTrue(container.constBegin() instanceof QByteArray.ConstIterator);
    	assertTrue(container.reverseBegin() instanceof QByteArray.ReverseIterator);
    	assertTrue(container.constReverseBegin() instanceof QByteArray.ConstReverseIterator);
		assertEquals(new QMetaType(QMetaType.Type.Char), container.begin().elementMetaType());
		assertEquals(new QMetaType(QMetaType.Type.Char), container.constBegin().elementMetaType());
    	assertEquals(QList.of((byte)'A', (byte)'B'), new QList<>(container.constBegin(), container.constEnd()));
    	assertEquals(QList.of((byte)'A', (byte)'B'), new QList<>(container.begin(), container.end()));
    	assertEquals(QList.of((byte)'B', (byte)'A'), new QList<>(container.rbegin(), container.rend()));
    	assertEquals(QList.of((byte)'B', (byte)'A'), new QList<>(container.crbegin(), container.crend()));

		assertTrue(container.constReverseBegin().isValid());
		assertTrue(container.reverseBegin().isValid());
		assertEquals(container.reverseBegin(), container.constReverseBegin());
		
    	QByteArray.Iterator miter1 = container.begin();
    	QByteArray clone = container.clone();
    	assertTrue(container.isSharedWith(clone));
    	
    	QByteArray.ConstIterator iter1 = container.constBegin();
		assertEquals(iter1, miter1);
		assertEquals(miter1, iter1);
		iter1.advance();
		QByteArray.ConstIterator iter2 = clone.constBegin();
		iter2.advance();
		assertEquals(iter1, iter2);
		ByteArrayOutputStream test = new ByteArrayOutputStream();
		for(QByteArray.ConstIterator iter = container.constBegin(), end = container.constEnd(); !iter.equals(end); iter.advance()) {
			test.write(iter.get());
		}
		assertArrayEquals(test.toByteArray(), container.toArray());
		test = new ByteArrayOutputStream();
		for(QByteArray.ConstReverseIterator iter = container.crbegin(), end = container.crend(); !iter.equals(end); iter.advance()) {
			test.write(iter.get());
		}
		assertArrayEquals(test.toByteArray(), new byte[]{(byte)'B', (byte)'A'});
		assertTrue(miter1.isValid());
		clone.dispose();
		assertTrue(miter1.isValid());
		container.clear();
		assertFalse(miter1.isValid());
		container = new QByteArray("AB");
		miter1 = container.begin();
		assertTrue(miter1.isValid());
		container.dispose();
		assertFalse(miter1.isValid());
		assertTrue(miter1.isDisposed());
		
		container = new QByteArray("AB");
		miter1 = container.begin();
		miter1.set((byte)5);
		iter1 = container.constBegin();
		assertEquals(5, iter1.getAsByte());
		assertEquals(5, container.begin().getAsByte());
		assertEquals((byte)'B', container.constReverseBegin().getAsByte());
		assertEquals((byte)'B', container.reverseBegin().getAsByte());
		container.insert(0, "C");
		assertFalse("mutable iterator not invalid after concurrent mutation", miter1.isValid());
		assertTrue("const iterator invalid after concurrent mutation", iter1.isValid());
		
		container = new QByteArray("ABCDE");
		QByteArray.Iterator erasedIter = container.erase(container.constBegin());
		assertTrue(erasedIter.isValid());
		assertEquals((byte)'B', erasedIter.getAsByte());
		
		container = new QByteArray("ABCDE");
		QByteArray.ConstIterator it1 = container.constBegin();
		it1.advance();
		QByteArray.ConstIterator it2 = it1.clone();
		it2.advance();
		erasedIter = container.erase(it1, it2);
		assertTrue(erasedIter.isValid());
		assertEquals((byte)'C', erasedIter.getAsByte());
		assertEquals("ACDE", container.toString());
		
		assertEquals((byte)'A', new QByteArray.ConstIterator(container.cbegin()).getAsByte());
		assertEquals((byte)'E', new QByteArray.ConstReverseIterator(container.crbegin()).getAsByte());
		assertEquals((byte)'A', new QByteArray.ConstIterator(container.begin()).getAsByte());
		assertEquals((byte)'E', new QByteArray.ConstReverseIterator(container.rbegin()).getAsByte());
		QByteArray.Iterator begin = container.begin();
		QByteArray.ReverseIterator rbegin = container.rbegin();
		QByteArray.ConstIterator cbegin = container.cbegin();
		QByteArray.ConstReverseIterator crbegin = container.crbegin();
		assertEquals(begin, cbegin);
		assertEquals(cbegin, begin);
		assertEquals(rbegin, crbegin);
		assertEquals(crbegin, rbegin);
    }
    
    @Test
    public void test_QByteArrayView() {
    	QByteArray byteArray = new QByteArray("AB");
    	QByteArrayView container = new QByteArrayView(byteArray);
    	assertTrue(container.cbegin() instanceof QByteArrayView.ConstIterator);
    	assertTrue(container.crbegin() instanceof QByteArrayView.ConstReverseIterator);
		assertEquals(new QMetaType(QMetaType.Type.Char), container.begin().elementMetaType());
    	assertEquals(QList.of((byte)'A', (byte)'B'), new QList<>(container.cbegin(), container.cend()));
    	assertEquals(QList.of((byte)'B', (byte)'A'), new QList<>(container.crbegin(), container.crend()));
    	QByteArrayView clone = container.clone();
    	
    	QByteArrayView.ConstIterator iter1 = container.cbegin();
		iter1.advance();
		QByteArrayView.ConstIterator iter2 = clone.cbegin();
		iter2.advance();
		assertEquals(iter1, iter2);
		ByteArrayOutputStream test = new ByteArrayOutputStream();
		for(QByteArrayView.ConstIterator iter = container.cbegin(), end = container.cend(); !iter.equals(end); iter.advance()) {
			test.write(iter.get());
		}
		assertArrayEquals(test.toByteArray(), container.toArray());
		test = new ByteArrayOutputStream();
		for(QByteArrayView.ConstReverseIterator iter = container.crbegin(), end = container.crend(); !iter.equals(end); iter.advance()) {
			test.write(iter.get());
		}
		assertArrayEquals(test.toByteArray(), new byte[]{(byte)'B', (byte)'A'});
		clone.dispose();
		container = new QByteArrayView(byteArray);
		iter1 = container.cbegin();
		assertEquals((byte)'A', iter1.getAsByte());
		assertEquals((byte)'B', container.crbegin().getAsByte());

		assertEquals((byte)'A', new QByteArrayView.ConstIterator(container.cbegin()).getAsByte());
		assertEquals((byte)'B', new QByteArrayView.ConstReverseIterator(container.crbegin()).getAsByte());
    }
    
    @Test
    public void test_QVersionNumber() {
    	QVersionNumber container = new QVersionNumber(1,2,3,4);
    	assertTrue(container.cbegin() instanceof QVersionNumber.ConstIterator);
    	assertTrue(container.crbegin() instanceof QVersionNumber.ConstReverseIterator);
		assertEquals(new QMetaType(QMetaType.Type.Int), container.begin().elementMetaType());
    	assertEquals(QList.of(1,2,3,4), new QList<>(container.cbegin(), container.cend()));
    	assertEquals(QList.of(4,3,2,1), new QList<>(container.crbegin(), container.crend()));
    	
    	QVersionNumber.ConstIterator iter1 = container.cbegin();
    	QVersionNumber.ConstIterator iter2 = container.cbegin();
    	assertEquals(iter1, iter2);
		iter1.advance();
		iter2.advance();
		assertEquals(iter1, iter2);
		int[] array = new int[4];
		IntBuffer test = IntBuffer.wrap(array);
		test.mark();
		for(QVersionNumber.ConstIterator iter = container.cbegin(), end = container.cend(); !iter.equals(end); iter.advance()) {
			test.put(iter.get());
		}
		assertArrayEquals(new int[] {1,2,3,4}, array);
		test.reset();
		for(QVersionNumber.ConstReverseIterator iter = container.crbegin(), end = container.crend(); !iter.equals(end); iter.advance()) {
			test.put(iter.get());
		}
		assertArrayEquals(new int[] {4,3,2,1}, array);
		container = new QVersionNumber(1,2,3,4);
		iter1 = container.cbegin();
		assertEquals(1, iter1.getAsInt());
		assertEquals(4, container.crbegin().getAsInt());
    }
    
    @Test
    public void test_QRegion() {
    	QRegion container = new QRegion();
    	assertTrue(container.begin() instanceof QRegion.ConstIterator);
    	assertTrue(container.constReverseBegin() instanceof QRegion.ConstReverseIterator);
		assertEquals(QMetaType.fromType(QRect.class), container.begin().elementMetaType());
		assertEquals(QMetaType.fromType(QRect.class), container.constReverseBegin().elementMetaType());
    	container.setRects(new QRect(1,2,3,4), new QRect(5,6,7,8));
    	assertEquals(QList.of(new QRect(1,2,3,4), new QRect(5,6,7,8)), new QList<>(container.begin(), container.end()));
    	QRegion.ConstIterator iter1 = container.begin();
    	QRegion clone = container.clone();
		iter1.advance();
		QRegion.ConstIterator iter2 = clone.begin();
		iter2.advance();
		assertEquals(iter1, iter2);
		ArrayList<QRect> test = new ArrayList<>();
		for(QRegion.ConstIterator iter = container.begin(), end = container.end(); !iter.equals(end); iter.advance()) {
			test.add(iter.get());
		}
		assertEquals(test, container.rects().toList());
		clone.dispose();
		container.setRects();
		assertTrue("const iterator invalid after concurrent mutation", iter1.isValid());
    }
    
    @Test
    public void test_QCborMap() {
    	QCborMap container = QCborMap.fromVariantMap(QMap.of("A", 1, "B", 2));
    	assertTrue(container.begin() instanceof QCborMap.Iterator);
    	assertTrue(container.constBegin() instanceof QCborMap.ConstIterator);
		assertEquals(QMetaType.fromType(QCborValue.class), container.begin().keyMetaType());
		assertEquals(QMetaType.fromType(QCborValue.class), container.begin().valueMetaType());
		assertEquals(QMetaType.fromType(QCborValue.class), container.constBegin().keyMetaType());
		assertEquals(QMetaType.fromType(QCborValue.class), container.constBegin().valueMetaType());
		assertEquals(QHash.of(new QCborValue("A"), new QCborValue(1), new QCborValue("B"), new QCborValue(2)), new QHash<>(container.constBegin(), container.constEnd()));
		assertEquals(QHash.of(new QCborValue("A"), new QCborValue(1), new QCborValue("B"), new QCborValue(2)), new QHash<>(container.begin(), container.end()));
		
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
		assertEquals(new QPair<>(new QCborValue("B"), new QCborValue(2)), new QCborMap.ConstIterator(container.begin()).get());
		QCborMap.Iterator begin = container.begin();
		QCborMap.ConstIterator cbegin = container.cbegin();
		assertEquals(begin, cbegin);
		assertEquals(cbegin, begin);
    }
    
    @Test
    public void test_QCborArray() {
    	QCborArray container = new QCborArray("X", "Y", "Z");
    	assertTrue(container.begin() instanceof QCborArray.Iterator);
    	assertTrue(container.constBegin() instanceof QCborArray.ConstIterator);
		assertEquals(QMetaType.fromType(QCborValue.class), container.begin().elementMetaType());
		assertEquals(QMetaType.fromType(QCborValue.class), container.constBegin().elementMetaType());
    	assertEquals(QList.of(new QCborValue("X"), new QCborValue("Y"), new QCborValue("Z")), new QList<>(container.constBegin(), container.constEnd()));
    	assertEquals(QList.of(new QCborValue("X"), new QCborValue("Y"), new QCborValue("Z")), new QList<>(container.begin(), container.end()));
    	QCborArray.Iterator miter1 = container.begin();
    	QCborArray clone = container.clone();
    	
		QCborArray.ConstIterator iter1 = container.constBegin();
		assertEquals(iter1, miter1);
		assertEquals(QMetaType.fromType(QCborValue.class), miter1.elementMetaType());
		assertEquals(QMetaType.fromType(QCborValue.class), iter1.elementMetaType());
		assertEquals(miter1, iter1);
		iter1.advance();
		QCborArray.ConstIterator iter2 = clone.constBegin();
		iter2.advance();
		assertEquals(iter1, iter2);
		QList<Object> test = QList.createVariantList();
		for(QCborArray.ConstIterator iter = container.constBegin(), end = container.constEnd(); !iter.equals(end); iter.advance()) {
			test.add(iter.get().toVariant());
		}
		assertEquals(test, container.toVariantList());
		assertTrue(miter1.isValid());
		clone.dispose();
		assertTrue(miter1.isValid());
		container.clear();
		assertFalse(miter1.isValid());
		container = new QCborArray("X", "Y", "Z");
		miter1 = container.begin();
		assertTrue(miter1.isValid());
		container.dispose();
		assertFalse(miter1.isValid());
		assertTrue(miter1.isDisposed());
		
		container = new QCborArray("X", "Y", "Z");
		miter1 = container.begin();
		miter1.set(new QCborValue("X"));
		iter1 = container.constBegin();
		assertEquals(new QCborValue("X"), iter1.get());
		assertEquals(new QCborValue("X"), container.extract(container.begin()));
		assertEquals(new QCborArray("Y", "Z"), container);
		container.assign(new QCborArray());
		assertFalse("mutable iterator not invalid after concurrent mutation", miter1.isValid());
		assertTrue("const iterator invalid after concurrent mutation", iter1.isValid());
		
		container = new QCborArray("X", "Y", "Z");
		QCborArray.Iterator erasedIter = container.erase(container.begin());
		assertTrue(erasedIter.isValid());
		assertEquals(new QCborValue("Y"), erasedIter.get());
		
		container = new QCborArray("X", "Y", "Z");
		erasedIter = container.erase(container.constBegin());
		assertTrue(erasedIter.isValid());
		assertEquals(new QCborValue("Y"), erasedIter.get());
		
		assertEquals(new QCborValue("Y"), new QCborArray.ConstIterator(container.cbegin()).get());
		assertEquals(new QCborValue("Y"), new QCborArray.ConstIterator(container.begin()).get());
		QCborArray.Iterator begin = container.begin();
		QCborArray.ConstIterator cbegin = container.cbegin();
		assertEquals(begin, cbegin);
		assertEquals(cbegin, begin);
    }
    
    @Test
    public void test_QJsonArray() {
    	QJsonArray container = new QJsonArray("X", "Y", "Z");
    	assertTrue(container.begin() instanceof QJsonArray.Iterator);
    	assertTrue(container.constBegin() instanceof QJsonArray.ConstIterator);
		assertEquals(QMetaType.fromType(QJsonValue.class), container.begin().elementMetaType());
		assertEquals(QMetaType.fromType(QJsonValue.class), container.constBegin().elementMetaType());
    	assertEquals(QList.of(new QJsonValue("X"), new QJsonValue("Y"), new QJsonValue("Z")), new QList<>(container.constBegin(), container.constEnd()));
    	assertEquals(QList.of(new QJsonValue("X"), new QJsonValue("Y"), new QJsonValue("Z")), new QList<>(container.begin(), container.end()));
    	QJsonArray.Iterator miter1 = container.begin();
    	QJsonArray clone = container.clone();
    	
		QJsonArray.ConstIterator iter1 = container.constBegin();
		assertTrue(iter1.isValid());
		assertEquals(iter1, miter1);
		assertEquals(miter1, iter1);
		iter1.advance();
		assertTrue(iter1.isValid());
		QJsonArray.ConstIterator iter2 = clone.constBegin();
		iter2.advance();
		assertNotEquals(iter1, iter2);
		assertNotEquals(iter1, container.constEnd());
		assertNotEquals(iter2, clone.constEnd());
		assertTrue(iter1.isValid());
		assertTrue(iter2.isValid());
		assertEquals(iter1.get(), iter2.get());
		QList<Object> test = QList.createVariantList();
		for(QJsonArray.ConstIterator iter = container.constBegin(), end = container.constEnd(); !iter.equals(end); iter.advance()) {
			test.add(iter.get().toString());
		}
		assertEquals(test, container.toList());
		assertTrue(miter1.isValid());
		clone.dispose();
		assertTrue(miter1.isValid());
		container = new QJsonArray("X", "Y", "Z");
		miter1 = container.begin();
		assertTrue(miter1.isValid());
		container.dispose();
		assertFalse(miter1.isValid());
		assertTrue(miter1.isDisposed());
		
		container = new QJsonArray("X", "Y", "Z");
		miter1 = container.begin();
		miter1.set(new QJsonValue("D"));
		iter1 = container.constBegin();
		assertEquals("D", iter1.get().toVariant());
		container.insert(0, "C");
		container.assign(new QJsonArray());
		assertFalse("mutable iterator not invalid after concurrent mutation", miter1.isValid());
		assertFalse("const iterator not invalid after concurrent mutation", iter1.isValid());
		
		container = new QJsonArray("X", "Y", "Z");
		QJsonArray.Iterator erasedIter = container.erase(container.begin());
		assertTrue(erasedIter.isValid());
		assertEquals(new QJsonValue("Y"), erasedIter.get());
		
		assertEquals(new QJsonValue("Y"), new QJsonArray.ConstIterator(container.cbegin()).get());
		assertEquals(new QJsonValue("Y"), new QJsonArray.ConstIterator(container.begin()).get());
		QJsonArray.Iterator begin = container.begin();
		QJsonArray.ConstIterator cbegin = container.cbegin();
		assertEquals(begin, cbegin);
		assertEquals(cbegin, begin);
    }
    
    @Test
    public void test_QJsonObject() {
    	QJsonObject container = new QJsonObject();
    	assertTrue(container.begin() instanceof QJsonObject.Iterator);
    	assertTrue(container.constBegin() instanceof QJsonObject.ConstIterator);
		assertEquals(QMetaType.fromType(String.class), container.begin().keyMetaType());
		assertEquals(QMetaType.fromType(QJsonValue.class), container.begin().valueMetaType());
		assertEquals(QMetaType.fromType(String.class), container.constBegin().keyMetaType());
		assertEquals(QMetaType.fromType(QJsonValue.class), container.constBegin().valueMetaType());
    	container.insert("A", 1);
    	container.insert("B", 2);
    	container.insert("C", 3);
		assertEquals(QHash.of("A", new QJsonValue(1), "B", new QJsonValue(2), "C", new QJsonValue(3)), new QHash<>(container.constBegin(), container.constEnd()));
		assertEquals(QHash.of("A", new QJsonValue(1), "B", new QJsonValue(2), "C", new QJsonValue(3)), new QHash<>(container.begin(), container.end()));
		
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
		assertEquals(new QJsonValue(2), new QJsonObject.ConstIterator(container.begin()).get());
		QJsonObject.Iterator begin = container.begin();
		QJsonObject.ConstIterator cbegin = container.cbegin();
		assertEquals(begin, cbegin);
		assertEquals(cbegin, begin);
    }
    
    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestIterators.class.getName());
    }
}
