/****************************************************************************
**
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

import java.util.List;
import java.util.TreeMap;
import java.util.concurrent.atomic.AtomicInteger;

import org.junit.Assert;
import org.junit.Test;

import io.qt.QtObject;
import io.qt.QtUtilities;
import io.qt.autotests.generated.General;
import io.qt.autotests.generated.Tulip;
import io.qt.autotests.generated.Variants;
import io.qt.core.QCoreApplication;
import io.qt.core.QEasingCurve;
import io.qt.core.QEvent;
import io.qt.core.QHash;
import io.qt.core.QItemSelection;
import io.qt.core.QList;
import io.qt.core.QMap;
import io.qt.core.QMetaType;
import io.qt.core.QMultiHash;
import io.qt.core.QMultiMap;
import io.qt.core.QObject;
import io.qt.core.QPair;
import io.qt.core.QRunnable;
import io.qt.core.QSet;
import io.qt.gui.QStandardItemModel;
import io.qt.gui.QTextCursor;
import io.qt.gui.QTextDocument;
import io.qt.widgets.QGraphicsEllipseItem;
import io.qt.widgets.QGraphicsItem;
import io.qt.widgets.QGraphicsPathItem;

public class TestReferenceCountingContainers extends ApplicationInitializer {
	
    @Test
    public void test_new_QList_QObject() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
	    	QList<QObject> container = new QList<>(QObject.class);
	    	for(int i=0; i<100; ++i) {
	    		QObject object = new QObject();
	    		QtUtilities.getSignalOnDispose(object).connect(counter::incrementAndGet);
	    		container.add(object);
	    		object = null;
	    	}
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(TestReferenceCountingContainers.class) {
	            	Thread.sleep(25);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(0, counter.get());
	        container = null;
	    }
        for (int i = 0; i < 50 && counter.get()<100; i++) {
            ApplicationInitializer.runGC();
            synchronized(TestReferenceCountingContainers.class) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(100, counter.get());
    }
    
    @Test
    public void test_new_QList_QRunnable() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
	    	QList<QRunnable> container = new QList<>(QRunnable.class);
	    	for(int i=0; i<100; ++i) {
	    		QRunnable object = new QRunnable() { @Override public void run() {} };
	    		container.add(object);
	    		QtUtilities.getSignalOnDispose(object).connect(counter::incrementAndGet);
	    		object = null;
	    	}
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(TestReferenceCountingContainers.class) {
	            	Thread.sleep(25);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(0, counter.get());
	        container = null;
    	}
        for (int i = 0; i < 50 && counter.get()<100; i++) {
            ApplicationInitializer.runGC();
            synchronized(TestReferenceCountingContainers.class) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(100, counter.get());
    }
    
    @Test
    public void test_new_QList_QGraphicsItem() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
	    	QList<QGraphicsItem> container = new QList<>(QGraphicsItem.class);
	    	for(int i=0; i<100; ++i) {
	    		QGraphicsItem object = new QGraphicsPathItem();
	    		QtUtilities.getSignalOnDispose(object).connect(counter::incrementAndGet);
	    		container.add(object);
	    		object = null;
	    	}
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(TestReferenceCountingContainers.class) {
	            	Thread.sleep(25);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(0, counter.get());
	        container = null;
    	}
        for (int i = 0; i < 50 && counter.get()<100; i++) {
            ApplicationInitializer.runGC();
            synchronized(TestReferenceCountingContainers.class) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(100, counter.get());
    }
    
    @Test
    public void test_new_QSet_QObject() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
	    	QSet<QObject> container = new QSet<>(QObject.class);
	    	for(int i=0; i<100; ++i) {
	    		QObject object = new QObject();
	    		QtUtilities.getSignalOnDispose(object).connect(counter::incrementAndGet);
	    		container.add(object);
	    		object = null;
	    	}
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(TestReferenceCountingContainers.class) {
	            	Thread.sleep(25);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(0, counter.get());
	        container = null;
	    }
        for (int i = 0; i < 50 && counter.get()<100; i++) {
            ApplicationInitializer.runGC();
            synchronized(TestReferenceCountingContainers.class) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(100, counter.get());
    }
    
    @Test
    public void test_new_QSet_QRunnable() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
	    	QSet<QRunnable> container = new QSet<>(QRunnable.class);
	    	for(int i=0; i<100; ++i) {
	    		QRunnable object = new QRunnable() { @Override public void run() {} };
	    		container.add(object);
	    		QtUtilities.getSignalOnDispose(object).connect(counter::incrementAndGet);
	    		object = null;
	    	}
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(TestReferenceCountingContainers.class) {
	            	Thread.sleep(25);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(0, counter.get());
	        container = null;
    	}
        for (int i = 0; i < 50 && counter.get()<100; i++) {
            ApplicationInitializer.runGC();
            synchronized(TestReferenceCountingContainers.class) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(100, counter.get());
    }
    
    @Test
    public void test_new_QSet_QGraphicsItem() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
	    	QSet<QGraphicsItem> container = new QSet<>(QGraphicsItem.class);
	    	for(int i=0; i<100; ++i) {
	    		QGraphicsItem object = new QGraphicsPathItem();
	    		QtUtilities.getSignalOnDispose(object).connect(counter::incrementAndGet);
	    		container.add(object);
	    		object = null;
	    	}
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(TestReferenceCountingContainers.class) {
	            	Thread.sleep(25);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(0, counter.get());
	        container = null;
    	}
        for (int i = 0; i < 50 && counter.get()<100; i++) {
            ApplicationInitializer.runGC();
            synchronized(TestReferenceCountingContainers.class) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(100, counter.get());
    }
    
    @Test
    public void test_new_QMultiHash_key_QObject() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
	    	QMultiHash<QObject,String> container = new QMultiHash<>(QObject.class, String.class);
	    	for(int i=0; i<100; ++i) {
	    		QObject object = new QObject();
	    		QtUtilities.getSignalOnDispose(object).connect(counter::incrementAndGet);
	    		container.insert(object, "");
	    		object = null;
	    	}
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(TestReferenceCountingContainers.class) {
	            	Thread.sleep(25);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(0, counter.get());
	        container = null;
	    }
        for (int i = 0; i < 50 && counter.get()<100; i++) {
            ApplicationInitializer.runGC();
            synchronized(TestReferenceCountingContainers.class) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(100, counter.get());
    }
    
    @Test
    public void test_new_QMultiHash_key_QRunnable() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
	    	QMultiHash<QRunnable,String> container = new QMultiHash<>(QRunnable.class, String.class);
	    	for(int i=0; i<100; ++i) {
	    		QRunnable object = new QRunnable() { @Override public void run() {} };
	    		container.insert(object, "");
	    		QtUtilities.getSignalOnDispose(object).connect(counter::incrementAndGet);
	    		object = null;
	    	}
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(TestReferenceCountingContainers.class) {
	            	Thread.sleep(25);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(0, counter.get());
	        container = null;
    	}
        for (int i = 0; i < 50 && counter.get()<100; i++) {
            ApplicationInitializer.runGC();
            synchronized(TestReferenceCountingContainers.class) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(100, counter.get());
    }
    
    @Test
    public void test_new_QMultiHash_key_QGraphicsItem() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
	    	QMultiHash<QGraphicsItem,String> container = new QMultiHash<>(QGraphicsItem.class, String.class);
	    	for(int i=0; i<100; ++i) {
	    		QGraphicsItem object = new QGraphicsPathItem();
	    		QtUtilities.getSignalOnDispose(object).connect(counter::incrementAndGet);
	    		container.insert(object, "");
	    		object = null;
	    	}
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(TestReferenceCountingContainers.class) {
	            	Thread.sleep(25);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(0, counter.get());
	        container = null;
    	}
        for (int i = 0; i < 50 && counter.get()<100; i++) {
            ApplicationInitializer.runGC();
            synchronized(TestReferenceCountingContainers.class) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(100, counter.get());
    }
    
    @Test
    public void test_new_QMultiHash_value_QObject() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
	    	QMultiHash<String,QObject> container = new QMultiHash<>(String.class, QObject.class);
	    	for(int i=0; i<100; ++i) {
	    		QObject object = new QObject();
	    		QtUtilities.getSignalOnDispose(object).connect(counter::incrementAndGet);
	    		container.insert(""+i, object);
	    		object = null;
	    	}
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(TestReferenceCountingContainers.class) {
	            	Thread.sleep(25);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(0, counter.get());
	        container = null;
	    }
        for (int i = 0; i < 50 && counter.get()<100; i++) {
            ApplicationInitializer.runGC();
            synchronized(TestReferenceCountingContainers.class) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(100, counter.get());
    }
    
    @Test
    public void test_new_QMultiHash_value_QRunnable() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
	    	QMultiHash<String,QRunnable> container = new QMultiHash<>(String.class, QRunnable.class);
	    	for(int i=0; i<100; ++i) {
	    		QRunnable object = new QRunnable() { @Override public void run() {} };
	    		container.insert(""+i, object);
	    		QtUtilities.getSignalOnDispose(object).connect(counter::incrementAndGet);
	    		object = null;
	    	}
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(TestReferenceCountingContainers.class) {
	            	Thread.sleep(25);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(0, counter.get());
	        container = null;
    	}
        for (int i = 0; i < 50 && counter.get()<100; i++) {
            ApplicationInitializer.runGC();
            synchronized(TestReferenceCountingContainers.class) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(100, counter.get());
    }
    
    @Test
    public void test_new_QMultiHash_value_QGraphicsItem() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
	    	QMultiHash<String,QGraphicsItem> container = new QMultiHash<>(String.class, QGraphicsItem.class);
	    	for(int i=0; i<100; ++i) {
	    		QGraphicsItem object = new QGraphicsPathItem();
	    		QtUtilities.getSignalOnDispose(object).connect(counter::incrementAndGet);
	    		container.insert(""+i, object);
	    		object = null;
	    	}
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(TestReferenceCountingContainers.class) {
	            	Thread.sleep(25);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(0, counter.get());
	        container = null;
    	}
        for (int i = 0; i < 50 && counter.get()<100; i++) {
            ApplicationInitializer.runGC();
            synchronized(TestReferenceCountingContainers.class) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(100, counter.get());
    }
    
    @Test
    public void test_new_QMultiHash_QObject_QObject() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
	    	QMultiHash<QObject,QObject> container = new QMultiHash<>(QObject.class, QObject.class);
	    	for(int i=0; i<100; ++i) {
	    		QObject key = new QObject();
	    		QObject object = new QObject();
	    		container.insert(key, object);
	    		QtUtilities.getSignalOnDispose(key).connect(counter::incrementAndGet);
	    		QtUtilities.getSignalOnDispose(object).connect(counter::incrementAndGet);
	    		key = null;
	    		object = null;
	    	}
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(TestReferenceCountingContainers.class) {
	            	Thread.sleep(25);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(0, counter.get());
	        container = null;
	    }
        for (int i = 0; i < 20 && counter.get()<200; i++) {
            ApplicationInitializer.runGC();
            synchronized(TestReferenceCountingContainers.class) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(200, counter.get());
    }
    
    @Test
    public void test_new_QMultiHash_QRunnable_QRunnable() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
	    	QMultiHash<QRunnable,QRunnable> container = new QMultiHash<>(QRunnable.class, QRunnable.class);
	    	for(int i=0; i<100; ++i) {
	    		QRunnable key = new QRunnable() { @Override public void run() {} };
	    		QRunnable object = new QRunnable() { @Override public void run() {} };
	    		container.insert(key, object);
	    		QtUtilities.getSignalOnDispose(key).connect(counter::incrementAndGet);
	    		QtUtilities.getSignalOnDispose(object).connect(counter::incrementAndGet);
	    		key = null;
	    		object = null;
	    	}
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(TestReferenceCountingContainers.class) {
	            	Thread.sleep(25);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(0, counter.get());
	        container = null;
    	}
        for (int i = 0; i < 20 && counter.get()<200; i++) {
            ApplicationInitializer.runGC();
            synchronized(TestReferenceCountingContainers.class) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(200, counter.get());
    }
    
    @Test
    public void test_new_QMultiHash_QGraphicsItem_QGraphicsItem() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
	    	QMultiHash<QGraphicsItem,QGraphicsItem> container = new QMultiHash<>(QGraphicsItem.class, QGraphicsItem.class);
	    	for(int i=0; i<100; ++i) {
	    		QGraphicsItem key = new QGraphicsEllipseItem();
	    		QGraphicsItem object = new QGraphicsPathItem();
	    		container.insert(key, object);
	    		QtUtilities.getSignalOnDispose(key).connect(counter::incrementAndGet);
	    		QtUtilities.getSignalOnDispose(object).connect(counter::incrementAndGet);
	    		key = null;
	    		object = null;
	    	}
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(TestReferenceCountingContainers.class) {
	            	Thread.sleep(25);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(0, counter.get());
	        container = null;
    	}
        for (int i = 0; i < 20 && counter.get()<200; i++) {
            ApplicationInitializer.runGC();
            synchronized(TestReferenceCountingContainers.class) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(200, counter.get());
    }
    
    @Test
    public void test_new_QMultiMap_key_QObject() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
	    	QMultiMap<QObject,String> container = new QMultiMap<>(QObject.class, String.class);
	    	for(int i=0; i<100; ++i) {
	    		QObject object = new QObject();
	    		QtUtilities.getSignalOnDispose(object).connect(counter::incrementAndGet);
	    		container.insert(object, "");
	    		object = null;
	    	}
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(TestReferenceCountingContainers.class) {
	            	Thread.sleep(25);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(0, counter.get());
	        container = null;
	    }
        for (int i = 0; i < 50 && counter.get()<100; i++) {
            ApplicationInitializer.runGC();
            synchronized(TestReferenceCountingContainers.class) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(100, counter.get());
    }
    
    @Test
    public void test_new_QMultiMap_key_QRunnable() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
	    	QMultiMap<QRunnable,String> container = new QMultiMap<>(QRunnable.class, String.class);
	    	for(int i=0; i<100; ++i) {
	    		QRunnable object = new QRunnable() { @Override public void run() {} };
	    		container.insert(object, "");
	    		QtUtilities.getSignalOnDispose(object).connect(counter::incrementAndGet);
	    		object = null;
	    	}
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(TestReferenceCountingContainers.class) {
	            	Thread.sleep(25);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(0, counter.get());
	        container = null;
    	}
        for (int i = 0; i < 50 && counter.get()<100; i++) {
            ApplicationInitializer.runGC();
            synchronized(TestReferenceCountingContainers.class) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(100, counter.get());
    }
    
    @Test
    public void test_new_QMultiMap_key_QGraphicsItem() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
	    	QMultiMap<QGraphicsItem,String> container = new QMultiMap<>(QGraphicsItem.class, String.class);
	    	for(int i=0; i<100; ++i) {
	    		QGraphicsItem object = new QGraphicsPathItem();
	    		QtUtilities.getSignalOnDispose(object).connect(counter::incrementAndGet);
	    		container.insert(object, "");
	    		object = null;
	    	}
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(TestReferenceCountingContainers.class) {
	            	Thread.sleep(25);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(0, counter.get());
	        container = null;
    	}
        for (int i = 0; i < 50 && counter.get()<100; i++) {
            ApplicationInitializer.runGC();
            synchronized(TestReferenceCountingContainers.class) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(100, counter.get());
    }
    
    @Test
    public void test_new_QMultiMap_value_QObject() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
	    	QMultiMap<String,QObject> container = new QMultiMap<>(String.class, QObject.class);
	    	for(int i=0; i<100; ++i) {
	    		QObject object = new QObject();
	    		QtUtilities.getSignalOnDispose(object).connect(counter::incrementAndGet);
	    		container.insert(""+i, object);
	    		object = null;
	    	}
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(TestReferenceCountingContainers.class) {
	            	Thread.sleep(25);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(0, counter.get());
	        container = null;
	    }
        for (int i = 0; i < 50 && counter.get()<100; i++) {
            ApplicationInitializer.runGC();
            synchronized(TestReferenceCountingContainers.class) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(100, counter.get());
    }
    
    @Test
    public void test_new_QMultiMap_value_QRunnable() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
	    	QMultiMap<String,QRunnable> container = new QMultiMap<>(String.class, QRunnable.class);
	    	for(int i=0; i<100; ++i) {
	    		QRunnable object = new QRunnable() { @Override public void run() {} };
	    		container.insert(""+i, object);
	    		QtUtilities.getSignalOnDispose(object).connect(counter::incrementAndGet);
	    		object = null;
	    	}
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(TestReferenceCountingContainers.class) {
	            	Thread.sleep(25);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(0, counter.get());
	        container = null;
    	}
        for (int i = 0; i < 50 && counter.get()<100; i++) {
            ApplicationInitializer.runGC();
            synchronized(TestReferenceCountingContainers.class) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(100, counter.get());
    }
    
    @Test
    public void test_new_QMultiMap_value_QGraphicsItem() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
	    	QMultiMap<String,QGraphicsItem> container = new QMultiMap<>(String.class, QGraphicsItem.class);
	    	for(int i=0; i<100; ++i) {
	    		QGraphicsItem object = new QGraphicsPathItem();
	    		QtUtilities.getSignalOnDispose(object).connect(counter::incrementAndGet);
	    		container.insert(""+i, object);
	    		object = null;
	    	}
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(TestReferenceCountingContainers.class) {
	            	Thread.sleep(25);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(0, counter.get());
	        container = null;
    	}
        for (int i = 0; i < 50 && counter.get()<100; i++) {
            ApplicationInitializer.runGC();
            synchronized(TestReferenceCountingContainers.class) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(100, counter.get());
    }
    
    @Test
    public void test_new_QMultiMap_QObject_QObject() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
	    	QMultiMap<QObject,QObject> container = new QMultiMap<>(QObject.class, QObject.class);
	    	for(int i=0; i<100; ++i) {
	    		QObject key = new QObject();
	    		QObject object = new QObject();
	    		container.insert(key, object);
	    		QtUtilities.getSignalOnDispose(key).connect(counter::incrementAndGet);
	    		QtUtilities.getSignalOnDispose(object).connect(counter::incrementAndGet);
	    		key = null;
	    		object = null;
	    	}
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(TestReferenceCountingContainers.class) {
	            	Thread.sleep(25);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(0, counter.get());
	        container = null;
	    }
        for (int i = 0; i < 20 && counter.get()<200; i++) {
            ApplicationInitializer.runGC();
            synchronized(TestReferenceCountingContainers.class) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(200, counter.get());
    }
    
    @Test
    public void test_new_QMultiMap_QRunnable_QRunnable() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
	    	QMultiMap<QRunnable,QRunnable> container = new QMultiMap<>(QRunnable.class, QRunnable.class);
	    	for(int i=0; i<100; ++i) {
	    		QRunnable key = new QRunnable() { @Override public void run() {} };
	    		QRunnable object = new QRunnable() { @Override public void run() {} };
	    		container.insert(key, object);
	    		QtUtilities.getSignalOnDispose(key).connect(counter::incrementAndGet);
	    		QtUtilities.getSignalOnDispose(object).connect(counter::incrementAndGet);
	    		key = null;
	    		object = null;
	    	}
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(TestReferenceCountingContainers.class) {
	            	Thread.sleep(25);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(0, counter.get());
	        container = null;
    	}
        for (int i = 0; i < 20 && counter.get()<200; i++) {
            ApplicationInitializer.runGC();
            synchronized(TestReferenceCountingContainers.class) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(200, counter.get());
    }
    
    @Test
    public void test_new_QMultiMap_QGraphicsItem_QGraphicsItem() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
	    	QMultiMap<QGraphicsItem,QGraphicsItem> container = new QMultiMap<>(QGraphicsItem.class, QGraphicsItem.class);
	    	for(int i=0; i<100; ++i) {
	    		QGraphicsItem key = new QGraphicsEllipseItem();
	    		QGraphicsItem object = new QGraphicsPathItem();
	    		container.insert(key, object);
	    		QtUtilities.getSignalOnDispose(key).connect(counter::incrementAndGet);
	    		QtUtilities.getSignalOnDispose(object).connect(counter::incrementAndGet);
	    		key = null;
	    		object = null;
	    	}
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(TestReferenceCountingContainers.class) {
	            	Thread.sleep(25);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(0, counter.get());
	        container = null;
    	}
        for (int i = 0; i < 20 && counter.get()<200; i++) {
            ApplicationInitializer.runGC();
            synchronized(TestReferenceCountingContainers.class) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(200, counter.get());
    }
    
    @Test
    public void test_cpp_QList_QObject() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
	    	QList<QObject> container = Tulip.createListOfObjects();
	    	for(int i=0; i<100; ++i) {
	    		QObject object = new QObject();
	    		QtUtilities.getSignalOnDispose(object).connect(counter::incrementAndGet);
	    		container.add(object);
	    		object = null;
	    	}
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(TestReferenceCountingContainers.class) {
	            	Thread.sleep(25);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(0, counter.get());
	        container = null;
	    }
        for (int i = 0; i < 50 && counter.get()<100; i++) {
            ApplicationInitializer.runGC();
            synchronized(TestReferenceCountingContainers.class) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(100, counter.get());
    }
    
    @Test
    public void test_cpp_QList_QRunnable() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
	    	QList<QRunnable> container = Tulip.createListOfRunnables();
	    	for(int i=0; i<100; ++i) {
	    		QRunnable object = new QRunnable() { @Override public void run() {} };
	    		container.add(object);
	    		QtUtilities.getSignalOnDispose(object).connect(counter::incrementAndGet);
	    		object = null;
	    	}
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(TestReferenceCountingContainers.class) {
	            	Thread.sleep(25);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(0, counter.get());
	        container = null;
    	}
        for (int i = 0; i < 50 && counter.get()<100; i++) {
            ApplicationInitializer.runGC();
            synchronized(TestReferenceCountingContainers.class) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(100, counter.get());
    }
    
    @Test
    public void test_cpp_QList_QEasingCurve_EasingFunction() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	TreeMap<Integer,Double> calls = new TreeMap<>();
    	{
	    	QList<QEasingCurve.EasingFunction> container = Tulip.createListOfEasingFunctions();
	    	Assert.assertTrue(container!=null);
	    	for(int i=0; i<100; ++i) {
	    		int _i = i;
	    		QEasingCurve.EasingFunction object = progress -> {
					calls.put(_i, progress);
					return 0;
	    		};
	    		container.add(object);
	    		General.internalAccess.setJavaOwnership(object);
	    		QtUtilities.getSignalOnDispose(object).connect(counter::incrementAndGet);
	    		object = null;
	    	}
	    	Tulip.testEasingFunctions(container);
	        Assert.assertEquals(100, calls.size());
	        for (int i = 0; i < calls.size(); i++) {
	        	Assert.assertEquals(i+1, calls.get(i).intValue());
			}
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(TestReferenceCountingContainers.class) {
	            	Thread.sleep(25);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(0, counter.get());
	        container = null;
    	}
        for (int i = 0; i < 50 && counter.get()<100; i++) {
            ApplicationInitializer.runGC();
            synchronized(TestReferenceCountingContainers.class) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(100, counter.get());
    }
    
    @Test
    public void test_java_QList_QEasingCurve_EasingFunction() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	TreeMap<Integer,Double> calls = new TreeMap<>();
    	{
	    	QList<QEasingCurve.EasingFunction> container = new QList<>(QEasingCurve.EasingFunction.class);
	    	Assert.assertTrue(container!=null);
	    	for(int i=0; i<100; ++i) {
	    		int _i = i;
	    		QEasingCurve.EasingFunction object = progress -> {
					calls.put(_i, progress);
					return 0;
	    		};
	    		container.add(object);
	    		General.internalAccess.setJavaOwnership(object);
	    		QtUtilities.getSignalOnDispose(object).connect(counter::incrementAndGet);
	    		object = null;
	    	}
	    	Tulip.testEasingFunctions(container);
	        Assert.assertEquals(100, calls.size());
	        for (int i = 0; i < calls.size(); i++) {
	        	Assert.assertEquals(i, calls.get(i).intValue());
			}
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(TestReferenceCountingContainers.class) {
	            	Thread.sleep(25);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(0, counter.get());
	        container = null;
    	}
        for (int i = 0; i < 50 && counter.get()<100; i++) {
            ApplicationInitializer.runGC();
            synchronized(TestReferenceCountingContainers.class) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(100, counter.get());
    }
    
    @Test
    public void test_cpp_QList_Tulip_TestStdFunction() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	TreeMap<Integer,Integer> calls = new TreeMap<>();
    	{
	    	QList<Tulip.TestStdFunction> container = Tulip.createListOfStdFunctions();
	    	Assert.assertTrue(container!=null);
	    	for(int i=0; i<100; ++i) {
	    		int _i = i;
	    		Tulip.TestStdFunction object = (int arg__1, boolean arg__2, double arg__3) -> {
					calls.put(_i, arg__1);
	    		};
	    		container.add(object);
	    		QtUtilities.getSignalOnDispose(object).connect(counter::incrementAndGet);
	    		object = null;
	    	}
	    	Tulip.testStdFunctions(container);
	        Assert.assertEquals(100, calls.size());
	        for (int i = 0; i < calls.size(); i++) {
	        	Assert.assertEquals(i+1, calls.get(i).intValue());
			}
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(TestReferenceCountingContainers.class) {
	            	Thread.sleep(25);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(0, counter.get());
	        container = null;
    	}
        for (int i = 0; i < 50 && counter.get()<100; i++) {
            ApplicationInitializer.runGC();
            synchronized(TestReferenceCountingContainers.class) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(100, counter.get());
    }
    
    @Test
    public void test_java_QList_Tulip_TestStdFunction() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	TreeMap<Integer,Integer> calls = new TreeMap<>();
    	{
	    	QList<Tulip.TestStdFunction> container = new QList<>(Tulip.TestStdFunction.class);
	    	Assert.assertTrue(container!=null);
	    	for(int i=0; i<100; ++i) {
	    		int _i = i;
	    		Tulip.TestStdFunction object = (int arg__1, boolean arg__2, double arg__3) -> {
					calls.put(_i, arg__1);
	    		};
	    		container.add(object);
	    		QtUtilities.getSignalOnDispose(object).connect(counter::incrementAndGet);
	    		object = null;
	    	}
	    	Tulip.testStdFunctions(container);
	        Assert.assertEquals(100, calls.size());
	        for (int i = 0; i < calls.size(); i++) {
	        	Assert.assertEquals(i, calls.get(i).intValue());
			}
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(TestReferenceCountingContainers.class) {
	            	Thread.sleep(25);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(0, counter.get());
	        container = null;
    	}
        for (int i = 0; i < 50 && counter.get()<100; i++) {
            ApplicationInitializer.runGC();
            synchronized(TestReferenceCountingContainers.class) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(100, counter.get());
    }
    
    @Test
    public void test_cpp_QSet_QObject() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
	    	QSet<QObject> container = Tulip.createSetOfObjects();
	    	for(int i=0; i<100; ++i) {
	    		QObject object = new QObject();
	    		QtUtilities.getSignalOnDispose(object).connect(counter::incrementAndGet);
	    		container.add(object);
	    		object = null;
	    	}
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(TestReferenceCountingContainers.class) {
	            	Thread.sleep(25);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(0, counter.get());
	        container = null;
	    }
        for (int i = 0; i < 50 && counter.get()<100; i++) {
            ApplicationInitializer.runGC();
            synchronized(TestReferenceCountingContainers.class) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(100, counter.get());
    }
    
    @Test
    public void test_cpp_QSet_QRunnable() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
	    	QSet<QRunnable> container = Tulip.createSetOfRunnables();
	    	for(int i=0; i<100; ++i) {
	    		QRunnable object = new QRunnable() { @Override public void run() {} };
	    		container.add(object);
	    		QtUtilities.getSignalOnDispose(object).connect(counter::incrementAndGet);
	    		object = null;
	    	}
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(TestReferenceCountingContainers.class) {
	            	Thread.sleep(25);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(0, counter.get());
	        container = null;
    	}
        for (int i = 0; i < 50 && counter.get()<100; i++) {
            ApplicationInitializer.runGC();
            synchronized(TestReferenceCountingContainers.class) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(100, counter.get());
    }
    
    @Test
    public void test_cpp_QHash_value_QObject() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
	    	QHash<String,QObject> container = Tulip.createStringHashOfObjects();
	    	for(int i=0; i<100; ++i) {
	    		QObject object = new QObject();
	    		QtUtilities.getSignalOnDispose(object).connect(counter::incrementAndGet);
	    		container.insert(""+i, object);
	    		object = null;
	    	}
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(TestReferenceCountingContainers.class) {
	            	Thread.sleep(25);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(0, counter.get());
	        container = null;
	    }
        for (int i = 0; i < 50 && counter.get()<100; i++) {
            ApplicationInitializer.runGC();
            synchronized(TestReferenceCountingContainers.class) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(100, counter.get());
    }
    
    @Test
    public void test_cpp_QHash_value_QRunnable() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
	    	QHash<String,QRunnable> container = Tulip.createStringHashOfRunnables();
	    	for(int i=0; i<100; ++i) {
	    		QRunnable object = new QRunnable() { @Override public void run() {} };
	    		container.insert(""+i, object);
	    		QtUtilities.getSignalOnDispose(object).connect(counter::incrementAndGet);
	    		object = null;
	    	}
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(TestReferenceCountingContainers.class) {
	            	Thread.sleep(25);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(0, counter.get());
	        container = null;
    	}
        for (int i = 0; i < 50 && counter.get()<100; i++) {
            ApplicationInitializer.runGC();
            synchronized(TestReferenceCountingContainers.class) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(100, counter.get());
    }
    
    @Test
    public void test_cpp_QMap_value_QObject() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
	    	QMap<String,QObject> container = Tulip.createStringMapOfObjects();
	    	for(int i=0; i<100; ++i) {
	    		QObject object = new QObject();
	    		QtUtilities.getSignalOnDispose(object).connect(counter::incrementAndGet);
	    		container.insert(""+i, object);
	    		object = null;
	    	}
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(TestReferenceCountingContainers.class) {
	            	Thread.sleep(25);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(0, counter.get());
	        container = null;
	    }
        for (int i = 0; i < 50 && counter.get()<100; i++) {
            ApplicationInitializer.runGC();
            synchronized(TestReferenceCountingContainers.class) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(100, counter.get());
    }
    
    @Test
    public void test_cpp_QMap_value_QRunnable() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
	    	QMap<String,QRunnable> container = Tulip.createStringMapOfRunnables();
	    	for(int i=0; i<100; ++i) {
	    		QRunnable object = new QRunnable() { @Override public void run() {} };
	    		container.insert(""+i, object);
	    		QtUtilities.getSignalOnDispose(object).connect(counter::incrementAndGet);
	    		object = null;
	    	}
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(TestReferenceCountingContainers.class) {
	            	Thread.sleep(25);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(0, counter.get());
	        container = null;
    	}
        for (int i = 0; i < 50 && counter.get()<100; i++) {
            ApplicationInitializer.runGC();
            synchronized(TestReferenceCountingContainers.class) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(100, counter.get());
    }
    
    @Test
    public void test_cpp_QMultiHash_value_QObject() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
	    	QMultiHash<String,QObject> container = Tulip.createStringMultiHashOfObjects();
	    	for(int i=0; i<100; ++i) {
	    		QObject object = new QObject();
	    		QtUtilities.getSignalOnDispose(object).connect(counter::incrementAndGet);
	    		container.insert(""+i, object);
	    		object = null;
	    	}
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(TestReferenceCountingContainers.class) {
	            	Thread.sleep(25);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(0, counter.get());
	        container = null;
	    }
        for (int i = 0; i < 50 && counter.get()<100; i++) {
            ApplicationInitializer.runGC();
            synchronized(TestReferenceCountingContainers.class) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(100, counter.get());
    }
    
    @Test
    public void test_cpp_QMultiHash_value_QRunnable() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
	    	QMultiHash<String,QRunnable> container = Tulip.createStringMultiHashOfRunnables();
	    	for(int i=0; i<100; ++i) {
	    		QRunnable object = new QRunnable() { @Override public void run() {} };
	    		container.insert(""+i, object);
	    		QtUtilities.getSignalOnDispose(object).connect(counter::incrementAndGet);
	    		object = null;
	    	}
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(TestReferenceCountingContainers.class) {
	            	Thread.sleep(25);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(0, counter.get());
	        container = null;
    	}
        for (int i = 0; i < 50 && counter.get()<100; i++) {
            ApplicationInitializer.runGC();
            synchronized(TestReferenceCountingContainers.class) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(100, counter.get());
    }
    
    @Test
    public void test_cpp_QMultiMap_value_QObject() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
	    	QMultiMap<String,QObject> container = Tulip.createStringMultiMapOfObjects();
	    	for(int i=0; i<100; ++i) {
	    		QObject object = new QObject();
	    		QtUtilities.getSignalOnDispose(object).connect(counter::incrementAndGet);
	    		container.insert(""+i, object);
	    		object = null;
	    	}
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(TestReferenceCountingContainers.class) {
	            	Thread.sleep(25);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(0, counter.get());
	        container = null;
	    }
        for (int i = 0; i < 50 && counter.get()<100; i++) {
            ApplicationInitializer.runGC();
            synchronized(TestReferenceCountingContainers.class) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(100, counter.get());
    }
    
    @Test
    public void test_cpp_QMultiMap_value_QRunnable() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
	    	QMultiMap<String,QRunnable> container = Tulip.createStringMultiMapOfRunnables();
	    	for(int i=0; i<100; ++i) {
	    		QRunnable object = new QRunnable() { @Override public void run() {} };
	    		container.insert(""+i, object);
	    		QtUtilities.getSignalOnDispose(object).connect(counter::incrementAndGet);
	    		object = null;
	    	}
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(TestReferenceCountingContainers.class) {
	            	Thread.sleep(25);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(0, counter.get());
	        container = null;
    	}
        for (int i = 0; i < 50 && counter.get()<100; i++) {
            ApplicationInitializer.runGC();
            Thread.yield();
            synchronized(TestReferenceCountingContainers.class) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(100, counter.get());
    }
    
    @Test
    public void test_java_QListList_QObject() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
	    	QList<QList<QObject>> container = new QList<>(QMetaType.fromType(QList.class, QMetaType.fromType(QObject.class)));
	    	for(int i=0; i<100; ++i) {
	    		QObject object = new QObject();
	    		QtUtilities.getSignalOnDispose(object).connect(counter::incrementAndGet);
	    		container.add(QList.of(object));
	    		object = null;
	    	}
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(TestReferenceCountingContainers.class) {
	            	Thread.sleep(25);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(0, counter.get());
	        container = null;
	    }
        for (int i = 0; i < 50 && counter.get()<100; i++) {
            ApplicationInitializer.runGC();
            Thread.yield();
            synchronized(TestReferenceCountingContainers.class) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(100, counter.get());
    }
    
    @Test
    public void test_java_QListList_QRunnable() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
    		QList<QList<QRunnable>> container = new QList<>(QMetaType.fromType(QList.class, QMetaType.fromType(QRunnable.class)));
	    	for(int i=0; i<100; ++i) {
	    		QRunnable object = new QRunnable() { @Override public void run() {} };
	    		container.add(QList.of(object));
	    		QtUtilities.getSignalOnDispose(object).connect(counter::incrementAndGet);
	    		object = null;
	    	}
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(TestReferenceCountingContainers.class) {
	            	Thread.sleep(25);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(0, counter.get());
	        container = null;
    	}
        for (int i = 0; i < 50 && counter.get()<100; i++) {
            ApplicationInitializer.runGC();
            Thread.yield();
            synchronized(TestReferenceCountingContainers.class) {
            	Thread.sleep(50);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(100, counter.get());
    }
    
    @Test
    public void test_cpp_QListList_QObject() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
	    	QList<List<QObject>> container = Tulip.createListOfListOfObjects();
	    	for(int i=0; i<100; ++i) {
	    		QObject object = new QObject();
	    		QtUtilities.getSignalOnDispose(object).connect(counter::incrementAndGet);
	    		container.add(QList.of(object));
	    		object = null;
	    	}
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(TestReferenceCountingContainers.class) {
	            	Thread.sleep(25);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(0, counter.get());
	        container = null;
	    }
        for (int i = 0; i < 50 && counter.get()<100; i++) {
            ApplicationInitializer.runGC();
            Thread.yield();
            synchronized(TestReferenceCountingContainers.class) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(100, counter.get());
    }
    
    @Test
    public void test_cpp_QListList_QRunnable() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
	    	QList<List<QRunnable>> container = Tulip.createListOfListOfRunnables();
	    	for(int i=0; i<100; ++i) {
	    		QRunnable object = new QRunnable() { @Override public void run() {} };
	    		container.add(QList.of(object));
	    		QtUtilities.getSignalOnDispose(object).connect(counter::incrementAndGet);
	    		object = null;
	    	}
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            Thread.yield();
	            synchronized(TestReferenceCountingContainers.class) {
	            	Thread.sleep(50);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(0, counter.get());
	        container = null;
    	}
        for (int i = 0; i < 50 && counter.get()<100; i++) {
            ApplicationInitializer.runGC();
            synchronized(TestReferenceCountingContainers.class) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(100, counter.get());
    }
    

    
    @Test
    public void test_java_QListList_QEasingCurve_EasingFunction() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	TreeMap<Integer,Double> calls = new TreeMap<>();
    	{
	    	QList<QList<QEasingCurve.EasingFunction>> container = new QList<>(QMetaType.fromType(QList.class, QMetaType.fromType(QEasingCurve.EasingFunction.class)));
	    	Assert.assertTrue(container!=null);
	    	for(int i=0; i<100; ++i) {
	    		int _i = i;
	    		QEasingCurve.EasingFunction object = progress -> {
					calls.put(_i, progress);
					return 0;
	    		};
	    		container.add(QList.of(object));
	    		General.internalAccess.setJavaOwnership(object);
	    		QtUtilities.getSignalOnDispose(object).connect(counter::incrementAndGet);
	    		object = null;
	    	}
	    	for(QList<QEasingCurve.EasingFunction> list : container) {
		    	Tulip.testEasingFunctions(list);
	    	}
	        Assert.assertEquals(100, calls.size());
	        for (int i = 0; i < calls.size(); i++) {
	        	Assert.assertEquals(0, calls.get(i).intValue());
			}
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            Thread.yield();
	            synchronized(TestReferenceCountingContainers.class) {
	            	Thread.sleep(25);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(0, counter.get());
	        container = null;
    	}
        for (int i = 0; i < 50 && counter.get()<100; i++) {
            ApplicationInitializer.runGC();
            Thread.yield();
            synchronized(TestReferenceCountingContainers.class) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(100, counter.get());
    }
    
    @Test
    public void test_java_QSetList_QEasingCurve_EasingFunction() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	TreeMap<Integer,Double> calls = new TreeMap<>();
    	{
	    	QSet<QList<QEasingCurve.EasingFunction>> container = new QSet<>(QMetaType.fromType(QList.class, QMetaType.fromType(QEasingCurve.EasingFunction.class)));
	    	Assert.assertTrue(container!=null);
	    	for(int i=0; i<100; ++i) {
	    		int _i = i;
	    		QEasingCurve.EasingFunction object = progress -> {
					calls.put(_i, progress);
					return 0;
	    		};
	    		container.add(QList.of(object));
	    		General.internalAccess.setJavaOwnership(object);
	    		QtUtilities.getSignalOnDispose(object).connect(counter::incrementAndGet);
	    		object = null;
	    	}
	    	for(QList<QEasingCurve.EasingFunction> list : container) {
		    	Tulip.testEasingFunctions(list);
	    	}
	        Assert.assertEquals(100, calls.size());
	        for (int i = 0; i < calls.size(); i++) {
	        	Assert.assertEquals(0, calls.get(i).intValue());
			}
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            Thread.yield();
	            synchronized(TestReferenceCountingContainers.class) {
	            	Thread.sleep(25);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(0, counter.get());
	        container = null;
    	}
        for (int i = 0; i < 50 && counter.get()<100; i++) {
            ApplicationInitializer.runGC();
            Thread.yield();
            synchronized(TestReferenceCountingContainers.class) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(100, counter.get());
    }
    
    @Test
    public void test_java_QStringMapList_QEasingCurve_EasingFunction() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	TreeMap<Integer,Double> calls = new TreeMap<>();
    	{
	    	QMap<String,QList<QEasingCurve.EasingFunction>> container = new QMap<>(String.class, QMetaType.fromType(QList.class, QMetaType.fromType(QEasingCurve.EasingFunction.class)));
	    	Assert.assertTrue(container!=null);
	    	for(int i=0; i<100; ++i) {
	    		int _i = i;
	    		QEasingCurve.EasingFunction object = progress -> {
					calls.put(_i, progress);
					return 0;
	    		};
	    		container.put(""+i, QList.of(object));
	    		General.internalAccess.setJavaOwnership(object);
	    		QtUtilities.getSignalOnDispose(object).connect(counter::incrementAndGet);
	    		object = null;
	    	}
	    	for(QList<QEasingCurve.EasingFunction> list : container.values()) {
		    	Tulip.testEasingFunctions(list);
	    	}
	        Assert.assertEquals(100, calls.size());
	        for (int i = 0; i < calls.size(); i++) {
	        	Assert.assertEquals(0, calls.get(i).intValue());
			}
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            Thread.yield();
	            synchronized(TestReferenceCountingContainers.class) {
	            	Thread.sleep(25);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(0, counter.get());
	        container = null;
    	}
        for (int i = 0; i < 50 && counter.get()<100; i++) {
            ApplicationInitializer.runGC();
            Thread.yield();
            synchronized(TestReferenceCountingContainers.class) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(100, counter.get());
    }
    
    @Test
    public void test_java_QStringHashList_QEasingCurve_EasingFunction() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	TreeMap<Integer,Double> calls = new TreeMap<>();
    	{
	    	QHash<String,QList<QEasingCurve.EasingFunction>> container = new QHash<>(String.class, QMetaType.fromType(QList.class, QMetaType.fromType(QEasingCurve.EasingFunction.class)));
	    	Assert.assertTrue(container!=null);
	    	for(int i=0; i<100; ++i) {
	    		int _i = i;
	    		QEasingCurve.EasingFunction object = progress -> {
					calls.put(_i, progress);
					return 0;
	    		};
	    		container.put(""+i, QList.of(object));
	    		General.internalAccess.setJavaOwnership(object);
	    		QtUtilities.getSignalOnDispose(object).connect(counter::incrementAndGet);
	    		object = null;
	    	}
	    	for(QList<QEasingCurve.EasingFunction> list : container.values()) {
    			Tulip.testEasingFunctions(list);
	    	}
	        Assert.assertEquals(100, calls.size());
	        for (int i = 0; i < calls.size(); i++) {
	        	Assert.assertEquals(0, calls.get(i).intValue());
			}
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            Thread.yield();
	            synchronized(TestReferenceCountingContainers.class) {
	            	Thread.sleep(25+i*10);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(0, counter.get());
	        container = null;
    	}
        for (int i = 0; i < 50 && counter.get()<100; i++) {
            ApplicationInitializer.runGC();
            Thread.yield();
            synchronized(TestReferenceCountingContainers.class) {
            	Thread.sleep(25+i*10);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(100, counter.get());
    }
    
    @Test
    public void test_java_QListSet_QEasingCurve_EasingFunction() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	TreeMap<Integer,Double> calls = new TreeMap<>();
    	{
	    	QList<QSet<QEasingCurve.EasingFunction>> container = new QList<>(QMetaType.fromType(QSet.class, QMetaType.fromType(QEasingCurve.EasingFunction.class)));
	    	Assert.assertTrue(container!=null);
	    	for(int i=0; i<100; ++i) {
	    		int _i = i;
	    		QEasingCurve.EasingFunction object = progress -> {
					calls.put(_i, progress);
					return 0;
	    		};
	    		container.add(QSet.of(object));
	    		General.internalAccess.setJavaOwnership(object);
	    		QtUtilities.getSignalOnDispose(object).connect(counter::incrementAndGet);
	    		object = null;
	    	}
	    	for(QSet<QEasingCurve.EasingFunction> list : container) {
		    	Tulip.testEasingFunctions(list);
	    	}
	        Assert.assertEquals(100, calls.size());
	        for (int i = 0; i < calls.size(); i++) {
	        	Assert.assertEquals(0, calls.get(i).intValue());
			}
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            Thread.yield();
	            synchronized(TestReferenceCountingContainers.class) {
	            	Thread.sleep(25+i*10);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(0, counter.get());
	        container = null;
    	}
        for (int i = 0; i < 50 && counter.get()<100; i++) {
            ApplicationInitializer.runGC();
            Thread.yield();
            synchronized(TestReferenceCountingContainers.class) {
            	Thread.sleep(25+i*10);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(100, counter.get());
    }
    
    @Test
    public void test_owner_QItemSelection() throws InterruptedException {
    	QStandardItemModel model = new QStandardItemModel();
    	model.insertColumns(0, 10);
    	model.insertRows(0, 10);
		QItemSelection selection = new QItemSelection();
		selection.select(model.index(2, 2), model.index(4, 4));
		selection.select(model.index(1, 2), model.index(1, 4));
		Assert.assertEquals(2, selection.size());
		Assert.assertEquals(model, General.internalAccess.owner(selection));
		selection = null;
    }
    
    @Test
    public void test_owner_QList_TextCursor() throws InterruptedException {
    	QTextDocument owner = new QTextDocument();
		QList<QTextCursor> list = QList.of(new QTextCursor(owner));
		Assert.assertEquals(1, list.size());
		Assert.assertEquals(owner, General.internalAccess.owner(list));
		list = null;
    }
    
    @Test
    public void test_owner_QListList_TextCursor() throws InterruptedException {
    	QTextDocument owner = new QTextDocument();
		QList<QList<QTextCursor>> list = QList.of(QList.of(new QTextCursor(owner)));
		Assert.assertEquals(1, list.size());
		Assert.assertEquals(owner, General.internalAccess.owner(list));
		list = null;
    }
    
    @Test
    public void test_owner_QListMap_TextCursor() throws InterruptedException {
    	QTextDocument owner = new QTextDocument();
		QList<QMap<QTextCursor,String>> list = QList.of(QMap.of(new QTextCursor(owner), "QTextCursor"));
		Assert.assertEquals(1, list.size());
		Assert.assertEquals(owner, General.internalAccess.owner(list));
		list = null;
    }
    
    @Test
    public void test_owner_QMapSetListList_TextCursor() throws InterruptedException {
    	QTextDocument owner = new QTextDocument();
		QMap<String,QSet<QList<QTextCursor>>> container = QMap.of("A", new QSet<>(QMetaType.fromType(QList.class, QMetaType.fromType(QTextCursor.class))), 
															      "Z", QSet.of(QList.of(new QTextCursor(owner))));
		Assert.assertEquals(2, container.size());
		Assert.assertEquals(owner, General.internalAccess.owner(container));
		container = null;
    }
    
    @Test
    public void test_owner_QListHash_TextCursor() throws InterruptedException {
    	QTextDocument owner = new QTextDocument();
		QList<QHash<QTextCursor,String>> list = QList.of(QHash.of(new QTextCursor(owner), "QTextCursor"));
		Assert.assertEquals(1, list.size());
		Assert.assertEquals(owner, General.internalAccess.owner(list));
		list = null;
    }
    
    @Test
    public void test_owner_QHashSetListList_TextCursor() throws InterruptedException {
    	QTextDocument owner = new QTextDocument();
		QHash<String,QSet<QList<QTextCursor>>> container = QHash.of("A", new QSet<>(QMetaType.fromType(QList.class, QMetaType.fromType(QTextCursor.class))), 
															      "Z", QSet.of(QList.of(new QTextCursor(owner))));
		Assert.assertEquals(2, container.size());
		Assert.assertEquals(owner, General.internalAccess.owner(container));
		container = null;
    }
    
    @Test
    public void test_owner_QListMultiMap_TextCursor() throws InterruptedException {
    	QTextDocument owner = new QTextDocument();
		QList<QMultiMap<QTextCursor,String>> list = QList.of(QMultiMap.of(new QTextCursor(owner), "QTextCursor"));
		Assert.assertEquals(1, list.size());
		Assert.assertEquals(owner, General.internalAccess.owner(list));
		list = null;
    }
    
    @Test
    public void test_owner_QMultiMapSetListList_TextCursor() throws InterruptedException {
    	QTextDocument owner = new QTextDocument();
		QMultiMap<String,QSet<QList<QTextCursor>>> container = QMultiMap.of("A", new QSet<>(QMetaType.fromType(QList.class, QMetaType.fromType(QTextCursor.class))), 
															      "Z", QSet.of(QList.of(new QTextCursor(owner))));
		Assert.assertEquals(2, container.size());
		Assert.assertEquals(owner, General.internalAccess.owner(container));
		container = null;
    }
    
    @Test
    public void test_owner_QListMultiHash_TextCursor() throws InterruptedException {
    	QTextDocument owner = new QTextDocument();
		QList<QMultiHash<QTextCursor,String>> list = QList.of(QMultiHash.of(new QTextCursor(owner), "QTextCursor"));
		Assert.assertEquals(1, list.size());
		Assert.assertEquals(owner, General.internalAccess.owner(list));
		list = null;
    }
    
    @Test
    public void test_owner_QMultiHashSetListList_TextCursor() throws InterruptedException {
    	QTextDocument owner = new QTextDocument();
		QMultiHash<String,QSet<QList<QTextCursor>>> container = QMultiHash.of("A", new QSet<>(QMetaType.fromType(QList.class, QMetaType.fromType(QTextCursor.class))), 
															      "Z", QSet.of(QList.of(new QTextCursor(owner))));
		Assert.assertEquals(2, container.size());
		Assert.assertEquals(owner, General.internalAccess.owner(container));
		container = null;
    }
    
    public static class CustomRunnable extends QtObject implements QRunnable{
    	@Override public void run() {}
    }
    
    @Test
    public void test_java_QListList_CustomPointerType() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
	    	QList<List<CustomRunnable>> container = new QList<>(QMetaType.fromType(QList.class, QMetaType.fromType(CustomRunnable.class)));
	    	for(int i=0; i<100; ++i) {
	    		CustomRunnable object = new CustomRunnable();
	    		container.add(QList.of(object));
	    		QtUtilities.getSignalOnDispose(object).connect(counter::incrementAndGet);
	    		object = null;
	    	}
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            Thread.yield();
	            synchronized(TestReferenceCountingContainers.class) {
	            	Thread.sleep(50);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(0, counter.get());
	        container = null;
    	}
        for (int i = 0; i < 50 && counter.get()<100; i++) {
            ApplicationInitializer.runGC();
            synchronized(TestReferenceCountingContainers.class) {
            	Thread.sleep(25+i*10);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(100, counter.get());
    }
    
    @Test
    public void test_QList_QObject_as_QVariant() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
        	QObject variantHolder = new QObject();
	    	QList<QObject> container = new QList<>(QObject.class);
	    	for(int i=0; i<100; ++i) {
	    		QObject object = new QObject();
	    		QtUtilities.getSignalOnDispose(object).connect(counter::incrementAndGet);
	    		container.add(object);
	    		object = null;
	    	}
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(TestReferenceCountingContainers.class) {
	            	Thread.sleep(25+i*10);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(0, counter.get());
	        variantHolder.setProperty("dynamicVariant", container);
	        Assert.assertTrue(Variants.compare(variantHolder, "dynamicVariant", container));
	        container = null;
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(TestReferenceCountingContainers.class) {
	            	Thread.sleep(25+i*10);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(0, counter.get());
	        variantHolder = null;
	    }
        for (int i = 0; i < 50 && counter.get()<100; i++) {
            ApplicationInitializer.runGC();
            synchronized(TestReferenceCountingContainers.class) {
            	Thread.sleep(25+i*10);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(100, counter.get());
    }
    
    @Test
    public void test_QPair_of_pointers_as_QVariant() throws InterruptedException {
    	AtomicInteger counter = new AtomicInteger();
    	{
        	QObject variantHolder = new QObject();
        	QPair<QObject,QObject> container = new QPair<>(new QObject(),new QObject());
    		QtUtilities.getSignalOnDispose(container.first).connect(counter::incrementAndGet);
    		QtUtilities.getSignalOnDispose(container.second).connect(counter::incrementAndGet);
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(TestReferenceCountingContainers.class) {
	            	Thread.sleep(25+i*10);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(0, counter.get());
	        variantHolder.setProperty("dynamicVariant", container);
	        Assert.assertTrue(Variants.compare(variantHolder, "dynamicVariant", container));
	        container = null;
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(TestReferenceCountingContainers.class) {
	            	Thread.sleep(25+i*10);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(0, counter.get());
	        variantHolder = null;
	    }
        for (int i = 0; i < 50 && counter.get()<2; i++) {
            ApplicationInitializer.runGC();
            synchronized(TestReferenceCountingContainers.class) {
            	Thread.sleep(25+i*10);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(2, counter.get());
    }
    
    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestReferenceCountingContainers.class.getName());
    }
}
