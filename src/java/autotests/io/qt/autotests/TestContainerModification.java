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

import java.util.*;

import org.junit.*;

import io.qt.core.*;

public class TestContainerModification extends ApplicationInitializer{
	@BeforeClass
    public static void testInitialize() throws Exception {
    	ApplicationInitializer.testInitializeWithWidgets();
    }
	
	@Test
	public void testConcurrentModificationQList() {
		QList<String> container = new QList<>(String.class);
		container.reserve(4);
		container.add("A");
		container.add("B");
		container.add("C");
		container.add("D");
		ArrayList<String> test = new ArrayList<>();
		for (String string : container) {
			test.add(string);
			if("B".equals(string)) {
				container.add("E");
			}
		}
		Assert.assertEquals(Arrays.asList("A", "B", "C", "D"), test);
		Assert.assertEquals(QList.of("A", "B", "C", "D", "E"), container);
	}
	
	@Test
	public void testConcurrentModificationQSet() {
		QSet<String> container = new QSet<>(String.class);
		container.reserve(4);
		container.add("A");
		container.add("B");
		container.add("C");
		container.add("D");
		Set<String> test = new HashSet<>();
		for (String string : container) {
			test.add(string);
			if("B".equals(string)) {
				container.add("E");
			}
		}
		Assert.assertEquals(new HashSet<>(Arrays.asList("A", "B", "C", "D")), test);
		Assert.assertEquals(QSet.of("A", "B", "C", "D", "E"), container);
	}
	
	@Test
	public void testConcurrentModificationQHash() {
		QHash<String,Object> container = new QHash<>(String.class, Object.class);
		container.reserve(4);
		container.insert("A", QVariant.NULL);
		container.insert("B", QVariant.NULL);
		container.insert("C", QVariant.NULL);
		container.insert("D", QVariant.NULL);
		QHash.ConstIterator<String, Object> iter1 = container.constFind("A");
		QHash.ConstIterator<String, Object> iter1Clone = iter1.clone();
		Assert.assertEquals(iter1, iter1Clone);
		QHash.ConstIterator<String, Object> iter2 = container.constFind("A");
		Assert.assertEquals(iter1, iter2);
		Assert.assertEquals("A", iter1.key());
		Assert.assertEquals("A", iter2.key());
		Map<String,Object> test = new HashMap<>();
		for (QPair<String,Object> entry : container) {
			test.put(entry.first, entry.second);
			if("B".equals(entry.first)) {
				container.insert("E", QVariant.NULL);
			}
		}
		Assert.assertEquals("A", iter1.key());
		Assert.assertEquals("A", iter2.key());
		Assert.assertEquals(iter1, iter2);
		QHash.ConstIterator<String, Object> iter3 = container.constFind("A");
		Assert.assertFalse(iter1.equals(iter3));
		Assert.assertEquals("A", iter3.key());
		Assert.assertEquals(QHash.of("A", QVariant.NULL, "B", QVariant.NULL, "C", QVariant.NULL, "D", QVariant.NULL), test);
		Assert.assertEquals(QHash.of("A", QVariant.NULL, "B", QVariant.NULL, "C", QVariant.NULL, "D", QVariant.NULL, "E", QVariant.NULL), container);
		container.clear();
		Assert.assertEquals("A", iter1.key());
		Assert.assertEquals("A", iter2.key());
		Assert.assertEquals("A", iter3.key());
	}
}
