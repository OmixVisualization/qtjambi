/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
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

import java.util.*;

import org.junit.*;

import io.qt.autotests.generated.*;
import io.qt.core.*;

public class TestContainerReference extends ApplicationInitializer {
	
	@BeforeClass
    public static void testInitialize() throws Exception {
    	ApplicationInitializer.testInitializeWithWidgets();
    }
	
    @Test
    public void run_testNativeQueue() {
    	ContainerReferences containerReferences = new ContainerReferences();
    	QQueue<String> listRef = containerReferences.queueRef();
    	for (String string : listRef) {
			string.length();
		}
    	assertEquals(0, listRef.size());
    	assertEquals(0, containerReferences.constQueue().size());
    	listRef.add("test");
    	assertEquals(1, listRef.size());
    	assertEquals(1, containerReferences.constQueue().size());
    	assertEquals("test", containerReferences.constQueue().peek());
    	listRef.add("test2");
    	assertEquals(2, listRef.size());
    	assertEquals(2, containerReferences.constQueue().size());
    	String concat = "";
    	for (String string : listRef) {
    		concat += string;
		}
    	assertEquals("testtest2", concat);
    	assertTrue(containerReferences.constQueue().contains("test2"));
    	
    	listRef.add("test");
    	assertEquals(2, listRef.count("test"));
    	assertEquals(1, listRef.indexOf("test2"));
    	assertEquals(1, listRef.lastIndexOf("test2"));
    	listRef.prepend("doggy");
    	listRef.prepend("catty");
    	assertEquals(3, containerReferences.constQueue().indexOf("test2"));
    	assertEquals(3, containerReferences.constQueue().lastIndexOf("test2"));
    	assertEquals(Arrays.asList("doggy", "test"), listRef.mid(1, 2));
    	assertEquals(Arrays.asList("doggy", "test"), containerReferences.constQueue().mid(1, 2));
    	listRef.move(0, 1);
    	assertEquals(Arrays.asList("catty", "test"), listRef.mid(1, 2));
    	assertEquals(Arrays.asList("catty", "test"), containerReferences.constQueue().mid(1, 2));
    	listRef.insert(2, "horsy");
    	assertEquals(Arrays.asList("catty", "horsy"), listRef.mid(1, 2));
    	assertEquals(Arrays.asList("catty", "horsy"), containerReferences.constQueue().mid(1, 2));
    	listRef.removeAt(1);
    	assertEquals(Arrays.asList("horsy", "test"), listRef.mid(1, 2));
    	assertEquals(Arrays.asList("horsy", "test"), containerReferences.constQueue().mid(1, 2));
    	listRef.removeOne("horsy");
    	assertEquals(Arrays.asList("test", "test2"), listRef.mid(1, 2));
    	assertEquals(Arrays.asList("test", "test2"), containerReferences.constQueue().mid(1, 2));
    	listRef.removeAll("test");
    	assertEquals(2, listRef.size());
    	assertEquals(2, containerReferences.constQueue().size());
    	listRef.swapItemsAt(0, 1);
    	assertEquals(Arrays.asList("test2", "doggy"), listRef);
    	assertEquals(Arrays.asList("test2", "doggy"), containerReferences.constQueue());
    	assertEquals("test2", listRef.takeAt(0));
    	assertEquals("", listRef.value(20));
    	assertEquals("nothing", listRef.value(20, "nothing"));
    	containerReferences.dispose();
        assertTrue(listRef.isDisposed());
    }

    @Test
    public void run_testNativeQListList() {
    	ContainerReferences containerReferences = new ContainerReferences();
    	QList<List<Float>> listRef = containerReferences.listListRef();
    	assertTrue(listRef!=null);
    	listRef.add(Arrays.asList(3f, 5.5f, 2.8f));
    	assertEquals(1, containerReferences.constListList().size());
    	assertEquals(Arrays.asList(3f, 5.5f, 2.8f), containerReferences.constListList().get(0));
    	containerReferences.dispose();
        assertTrue(listRef.isDisposed());
    }
    
    @Test
    public void run_testNativeQMap() {
    	ContainerReferences containerReferences = new ContainerReferences();
    	QMap<String, QPoint> mapRef = containerReferences.mapRef();
    	mapRef.insert("1", new QPoint(0,0));
    	mapRef.insert("2", new QPoint(5,6));
    	mapRef.insert("A", new QPoint(6,7));
    	mapRef.insert("1", new QPoint(1,1));
    	assertEquals(3, containerReferences.constMap().size());
    	assertEquals(new QPoint(1,1), containerReferences.constMap().first());
    	assertEquals(new QPoint(6,7), containerReferences.constMap().last());
    	assertEquals("1", containerReferences.constMap().firstKey());
    	assertEquals("A", containerReferences.constMap().lastKey());
    	assertEquals(new QPoint(6,7), mapRef.take("A"));
    	mapRef.removeAll("1");
    	assertEquals("2", containerReferences.constMap().firstKey());
    	assertEquals("2", containerReferences.constMap().lastKey());
    	assertFalse(containerReferences.constMap().constFind("X").isValid());
    	assertEquals(new QPoint(5,6), containerReferences.constMap().constFind("2").value());
    	assertEquals(Arrays.asList("2"), mapRef.keys());
    	containerReferences.dispose();
        assertTrue(mapRef.isDisposed());
    }
    
    @Test
    public void run_testNativeQMultiMap() {
    	ContainerReferences containerReferences = new ContainerReferences();
    	QMultiMap<String, QPoint> mapRef = containerReferences.multiMapRef();
    	mapRef.insert("1", new QPoint(0,0));
    	mapRef.insert("2", new QPoint(5,6));
    	mapRef.insert("A", new QPoint(6,7));
    	mapRef.insert("1", new QPoint(1,1));
    	assertEquals(4, containerReferences.constMultiMap().size());
    	assertEquals(new QPoint(1,1), containerReferences.constMultiMap().first());
    	assertEquals(new QPoint(6,7), containerReferences.constMultiMap().last());
    	assertEquals("1", containerReferences.constMultiMap().firstKey());
    	assertEquals("A", containerReferences.constMultiMap().lastKey());
    	assertEquals(new QPoint(6,7), mapRef.take("A"));
    	assertEquals(2, mapRef.removeAll("1"));
    	assertEquals("2", containerReferences.constMultiMap().firstKey());
    	assertEquals("2", containerReferences.constMultiMap().lastKey());
    	assertFalse(containerReferences.constMultiMap().constFind("X").isValid());
    	assertEquals(new QPoint(5,6), containerReferences.constMultiMap().constFind("2").value());
    	assertEquals(Arrays.asList("2"), mapRef.keys());
    	containerReferences.dispose();
        assertTrue(mapRef.isDisposed());
    }
    
    @Test
    public void run_testNativeQHash() {
    	ContainerReferences containerReferences = new ContainerReferences();
    	QHash<Integer, QPoint> mapRef = containerReferences.hashRef();
    	mapRef.insert(1, new QPoint(0,0));
    	mapRef.insert(2, new QPoint(5,6));
    	mapRef.insert(3, new QPoint(6,7));
    	mapRef.insert(1, new QPoint(1,1));
    	assertEquals(3, containerReferences.constHash().size());
    	assertEquals(new QPoint(6,7), mapRef.take(3));
    	mapRef.removeAll(1);
    	assertFalse(containerReferences.constHash().constFind(20).isValid());
    	assertEquals(new QPoint(5,6), containerReferences.constHash().constFind(2).value());
    	assertEquals(Arrays.asList(2), mapRef.keys());
        mapRef.dispose();
        assertTrue(mapRef.isDisposed());
        mapRef = containerReferences.hashRef();
    	assertEquals(Arrays.asList(2), mapRef.keys());
    	containerReferences.dispose();
        assertTrue(mapRef.isDisposed());
    	containerReferences.dispose();
        assertTrue(mapRef.isDisposed());
    }
    
    @Test
    public void run_testNativeQMultiHash() {
    	ContainerReferences containerReferences = new ContainerReferences();
    	QMultiHash<Integer, QPoint> mapRef = containerReferences.multiHashRef();
    	mapRef.insert(1, new QPoint(0,0));
    	mapRef.insert(2, new QPoint(5,6));
    	mapRef.insert(3, new QPoint(6,7));
    	mapRef.insert(1, new QPoint(1,1));
    	assertEquals(4, containerReferences.constMultiHash().size());
    	assertEquals(new QPoint(6,7), mapRef.take(3));
    	mapRef.removeAll(1);
    	assertFalse(containerReferences.constMultiHash().constFind(20).isValid());
    	assertEquals(new QPoint(5,6), containerReferences.constMultiHash().constFind(2).value());
    	assertEquals(Arrays.asList(2), mapRef.keys());
        assertTrue(General.internalAccess.isSplitOwnership(mapRef));
        mapRef.dispose();
        assertTrue(mapRef.isDisposed());
        mapRef = containerReferences.multiHashRef();
    	assertEquals(Arrays.asList(2), mapRef.keys());
    	containerReferences.dispose();
        assertTrue(mapRef.isDisposed());
    }
    
    @Test
    public void run_testNativeQStringList() {
    	ContainerReferences containerReferences = new ContainerReferences();
    	QList<String> listRef = containerReferences.stringListRef();
    	for (String string : listRef) {
			string.length();
		}
    	assertEquals(0, listRef.size());
    	assertEquals(0, containerReferences.constStringList().size());
    	listRef.add("test");
    	assertEquals(1, listRef.size());
    	assertEquals(1, containerReferences.constStringList().size());
    	assertEquals("test", containerReferences.constStringList().get(0));
    	listRef.add("test2");
    	assertEquals(2, listRef.size());
    	assertEquals(2, containerReferences.constStringList().size());
    	String concat = "";
    	for (String string : listRef) {
    		concat += string;
		}
    	assertEquals("testtest2", concat);
    	assertTrue(containerReferences.constStringList().contains("test2"));
    	
    	assertEquals(1, listRef.indexOf("test2"));
    	assertEquals(1, listRef.lastIndexOf("test2"));
    	listRef.prepend("doggy");
    	listRef.prepend("catty");
    	assertEquals(3, containerReferences.constStringList().indexOf("test2"));
    	assertEquals(3, containerReferences.constStringList().lastIndexOf("test2"));
    	assertEquals(Arrays.asList("doggy", "test"), listRef.mid(1, 2));
    	assertEquals(Arrays.asList("doggy", "test"), containerReferences.constStringList().mid(1, 2));
    	listRef.move(0, 1);
    	assertEquals(Arrays.asList("catty", "test"), listRef.mid(1, 2));
    	assertEquals(Arrays.asList("catty", "test"), containerReferences.constStringList().mid(1, 2));
    	listRef.insert(2, "horsy");
    	assertEquals(Arrays.asList("catty", "horsy"), listRef.mid(1, 2));
    	assertEquals(Arrays.asList("catty", "horsy"), containerReferences.constStringList().mid(1, 2));
    	listRef.removeAt(1);
    	assertEquals(Arrays.asList("horsy", "test"), listRef.mid(1, 2));
    	assertEquals(Arrays.asList("horsy", "test"), containerReferences.constStringList().mid(1, 2));
    	listRef.removeOne("horsy");
    	assertEquals(Arrays.asList("test", "test2"), listRef.mid(1, 2));
    	assertEquals(Arrays.asList("test", "test2"), containerReferences.constStringList().mid(1, 2));
    	listRef.removeAll("test");
    	assertEquals(2, listRef.size());
    	listRef.swapItemsAt(0, 1);
    	assertEquals(Arrays.asList("test2", "doggy"), listRef);
    	assertEquals(Arrays.asList("test2", "doggy"), containerReferences.constStringList());
    	assertEquals("test2", listRef.takeAt(0));
    	assertEquals("", listRef.value(20));
    	assertEquals("nothing", listRef.value(20, "nothing"));
    	containerReferences.dispose();
        assertTrue(listRef.isDisposed());
    }
    
    @Test
    public void run_testNativeQByteArrayList() {
    	ContainerReferences containerReferences = new ContainerReferences();
    	QList<QByteArray> listRef = containerReferences.byteArrayListRef();
    	for (QByteArray string : listRef) {
			string.length();
		}
    	assertEquals(0, listRef.size());
    	assertEquals(0, containerReferences.constByteArrayList().size());
    	listRef.add(new QByteArray("test"));
    	assertEquals(1, listRef.size());
    	assertEquals(1, containerReferences.constByteArrayList().size());
    	assertEquals(new QByteArray("test"), containerReferences.constByteArrayList().get(0));
    	listRef.add(new QByteArray("test2"));
    	assertEquals(2, listRef.size());
    	assertEquals(2, containerReferences.constByteArrayList().size());
    	String concat = "";
    	for (QByteArray string : listRef) {
    		concat += string;
		}
    	assertEquals("testtest2", concat);
    	assertTrue(containerReferences.constByteArrayList().contains(new QByteArray("test2")));

    	listRef.add(new QByteArray("test"));
    	assertEquals(2, listRef.count(new QByteArray("test")));
    	assertEquals(2, containerReferences.constByteArrayList().count(new QByteArray("test")));
    	assertEquals(1, listRef.indexOf(new QByteArray("test2")));
    	assertEquals(1, listRef.lastIndexOf(new QByteArray("test2")));
    	listRef.prepend(new QByteArray("doggy"));
    	listRef.prepend(new QByteArray("catty"));
    	assertEquals(3, containerReferences.constByteArrayList().indexOf(new QByteArray("test2")));
    	assertEquals(3, containerReferences.constByteArrayList().lastIndexOf(new QByteArray("test2")));
    	assertEquals(Arrays.asList(new QByteArray("doggy"), new QByteArray("test")), listRef.mid(1, 2));
    	assertEquals(Arrays.asList(new QByteArray("doggy"), new QByteArray("test")), containerReferences.constByteArrayList().mid(1, 2));
    	listRef.move(0, 1);
    	assertEquals(Arrays.asList(new QByteArray("catty"), new QByteArray("test")), listRef.mid(1, 2));
    	assertEquals(Arrays.asList(new QByteArray("catty"), new QByteArray("test")), containerReferences.constByteArrayList().mid(1, 2));
    	listRef.insert(2, new QByteArray("horsy"));
    	assertEquals(Arrays.asList(new QByteArray("catty"), new QByteArray("horsy")), listRef.mid(1, 2));
    	assertEquals(Arrays.asList(new QByteArray("catty"), new QByteArray("horsy")), containerReferences.constByteArrayList().mid(1, 2));
    	listRef.removeAt(1);
    	assertEquals(Arrays.asList(new QByteArray("horsy"), new QByteArray("test")), listRef.mid(1, 2));
    	assertEquals(Arrays.asList(new QByteArray("horsy"), new QByteArray("test")), containerReferences.constByteArrayList().mid(1, 2));
    	listRef.removeOne(new QByteArray("horsy"));
    	assertEquals(Arrays.asList(new QByteArray("test"), new QByteArray("test2")), listRef.mid(1, 2));
    	assertEquals(Arrays.asList(new QByteArray("test"), new QByteArray("test2")), containerReferences.constByteArrayList().mid(1, 2));
    	listRef.removeAll(new QByteArray("test"));
    	assertEquals(2, listRef.size());
    	assertEquals(2, containerReferences.constByteArrayList().size());
    	listRef.swapItemsAt(0, 1);
    	assertEquals(Arrays.asList(new QByteArray("test2"), new QByteArray("doggy")), listRef);
    	assertEquals(Arrays.asList(new QByteArray("test2"), new QByteArray("doggy")), containerReferences.constByteArrayList());
    	assertEquals(new QByteArray("test2"), listRef.takeAt(0));
    	assertEquals(new QByteArray(), listRef.value(20));
    	assertEquals(new QByteArray("nothing"), listRef.value(20, new QByteArray("nothing")));
    	containerReferences.dispose();
        assertTrue(listRef.isDisposed());
    }
    
    @Test
    public void run_testNativeQListMemberField() {
    	ContainerReferences containerReferences = new ContainerReferences();
    	QList<String> listRef = containerReferences.listRef();
    	for (String string : listRef) {
			string.length();
		}
    	assertEquals(0, listRef.size());
    	assertEquals(0, containerReferences.list().size());
    	listRef.add("test");
    	assertEquals(1, listRef.size());
    	assertEquals(1, containerReferences.list().size());
    	assertEquals("test", containerReferences.list().get(0));
    	listRef.add("test2");
    	assertEquals(2, listRef.size());
    	assertEquals(2, containerReferences.list().size());
    	String concat = "";
    	for (String string : listRef) {
    		concat += string;
		}
    	assertEquals("testtest2", concat);
    	assertTrue(containerReferences.list().contains("test2"));
    	
    	listRef.add("test");
    	assertEquals(2, listRef.count("test"));
    	assertEquals(2, containerReferences.list().count("test"));
    	assertEquals(1, listRef.indexOf("test2"));
    	assertEquals(1, listRef.lastIndexOf("test2"));
    	listRef.prepend("doggy");
    	listRef.prepend("catty");
    	assertEquals(3, containerReferences.list().indexOf("test2"));
    	assertEquals(3, containerReferences.list().lastIndexOf("test2"));
    	assertEquals(Arrays.asList("doggy", "test"), listRef.mid(1, 2));
    	assertEquals(Arrays.asList("doggy", "test"), containerReferences.list().mid(1, 2));
    	listRef.move(0, 1);
    	assertEquals(Arrays.asList("catty", "test"), listRef.mid(1, 2));
    	assertEquals(Arrays.asList("catty", "test"), containerReferences.list().mid(1, 2));
    	listRef.insert(2, "horsy");
    	assertEquals(Arrays.asList("catty", "horsy"), listRef.mid(1, 2));
    	assertEquals(Arrays.asList("catty", "horsy"), containerReferences.list().mid(1, 2));
    	listRef.removeAt(1);
    	assertEquals(Arrays.asList("horsy", "test"), listRef.mid(1, 2));
    	assertEquals(Arrays.asList("horsy", "test"), containerReferences.list().mid(1, 2));
    	listRef.removeOne("horsy");
    	assertEquals(Arrays.asList("test", "test2"), listRef.mid(1, 2));
    	assertEquals(Arrays.asList("test", "test2"), containerReferences.list().mid(1, 2));
    	listRef.removeAll("test");
    	assertEquals(2, listRef.size());
    	assertEquals(2, containerReferences.list().size());
    	listRef.swapItemsAt(0, 1);
    	assertEquals(Arrays.asList("test2", "doggy"), listRef);
    	assertEquals(Arrays.asList("test2", "doggy"), containerReferences.list());
    	assertEquals("test2", listRef.takeAt(0));
    	assertEquals("", listRef.value(20));
    	assertEquals("nothing", listRef.value(20, "nothing"));
    	containerReferences.dispose();
        assertTrue(listRef.isDisposed());
    }
    
    @Test
    public void run_testNativeQList() {
    	ContainerReferences containerReferences = new ContainerReferences();
    	QList<String> listRef = containerReferences.listRef();
    	for (String string : listRef) {
			string.length();
		}
    	assertEquals(0, listRef.size());
    	assertEquals(0, containerReferences.constList().size());
    	listRef.add("test");
    	assertEquals(1, listRef.size());
    	assertEquals(1, containerReferences.constList().size());
    	assertEquals("test", containerReferences.constList().get(0));
    	listRef.add("test2");
    	assertEquals(2, listRef.size());
    	assertEquals(2, containerReferences.constList().size());
    	String concat = "";
    	for (String string : listRef) {
    		concat += string;
		}
    	assertEquals("testtest2", concat);
    	assertTrue(containerReferences.constList().contains("test2"));
    	
    	listRef.add("test");
    	assertEquals(2, listRef.count("test"));
    	assertEquals(2, containerReferences.constList().count("test"));
    	assertEquals(1, listRef.indexOf("test2"));
    	assertEquals(1, listRef.lastIndexOf("test2"));
    	listRef.prepend("doggy");
    	listRef.prepend("catty");
    	assertEquals(3, containerReferences.constList().indexOf("test2"));
    	assertEquals(3, containerReferences.constList().lastIndexOf("test2"));
    	assertEquals(Arrays.asList("doggy", "test"), listRef.mid(1, 2));
    	assertEquals(Arrays.asList("doggy", "test"), containerReferences.constList().mid(1, 2));
    	listRef.move(0, 1);
    	assertEquals(Arrays.asList("catty", "test"), listRef.mid(1, 2));
    	assertEquals(Arrays.asList("catty", "test"), containerReferences.constList().mid(1, 2));
    	listRef.insert(2, "horsy");
    	assertEquals(Arrays.asList("catty", "horsy"), listRef.mid(1, 2));
    	assertEquals(Arrays.asList("catty", "horsy"), containerReferences.constList().mid(1, 2));
    	listRef.removeAt(1);
    	assertEquals(Arrays.asList("horsy", "test"), listRef.mid(1, 2));
    	assertEquals(Arrays.asList("horsy", "test"), containerReferences.constList().mid(1, 2));
    	listRef.removeOne("horsy");
    	assertEquals(Arrays.asList("test", "test2"), listRef.mid(1, 2));
    	assertEquals(Arrays.asList("test", "test2"), containerReferences.constList().mid(1, 2));
    	listRef.removeAll("test");
    	assertEquals(2, listRef.size());
    	assertEquals(2, containerReferences.constList().size());
    	listRef.swapItemsAt(0, 1);
    	assertEquals(Arrays.asList("test2", "doggy"), listRef);
    	assertEquals(Arrays.asList("test2", "doggy"), containerReferences.constList());
    	assertEquals("test2", listRef.takeAt(0));
    	assertEquals("", listRef.value(20));
    	assertEquals("nothing", listRef.value(20, "nothing"));
    	containerReferences.dispose();
        assertTrue(listRef.isDisposed());
    }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestContainerReference.class.getName());
    }
}
