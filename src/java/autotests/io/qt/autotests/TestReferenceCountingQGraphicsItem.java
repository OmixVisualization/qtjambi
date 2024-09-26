/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2024 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
import static org.junit.Assert.assertTrue;

import java.util.List;
import java.util.concurrent.atomic.AtomicInteger;

import org.junit.Assert;
import org.junit.Assume;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.NonNull;
import io.qt.Nullable;
import io.qt.autotests.generated.General;
import io.qt.core.*;
import io.qt.gui.*;
import io.qt.widgets.*;

public class TestReferenceCountingQGraphicsItem extends ApplicationInitializer{
	
	@BeforeClass
    public static void testInitialize() throws Exception {
		ApplicationInitializer.testInitializeWithWidgets();
		Assume.assumeTrue("A screen is required to create a window.", QGuiApplication.primaryScreen()!=null);
    }
	
	private static final int COUNT = 20;
	
	static void uncalled(){
		QGuiApplication.platformName();
	}
	
	@Test public void testQGraphicsWidgetActionsNull() {
        QGraphicsWidget w = new QGraphicsWidget();
        w.addAction(null);
        w.addActions(null);
        w.removeAction(null);
        assertEquals(0, w.actions().size());
	}
	
	@Test public void testQGraphicsWidgetAddActions() throws InterruptedException {
		AtomicInteger counter = new AtomicInteger();
		{
			QGraphicsWidget w = new QGraphicsWidget();
	        for (int i=0; i<COUNT; ++i) {
                QAction act = new QAction("action" + i, null);
                General.internalAccess.registerCleaner(act, counter::incrementAndGet);
                w.addActions(QList.of(act));
                QApplication.processEvents();
                act = null;
	        }
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(getClass()) {
	            	Thread.sleep(25);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(0, counter.get());
	        List<QAction> actions = w.actions();
	        assertEquals(COUNT, actions.size());
	        for (int i=0; i<COUNT; ++i) {
	            assertTrue(actions.get(i) != null);
	            assertEquals("action" + i, actions.get(i).text());
	        }
            General.internalAccess.registerCleaner(w, counter::incrementAndGet);
	        w = null;
		}
        for (int i = 0; i < 50 && counter.get()<COUNT+1; i++) {
            ApplicationInitializer.runGC();
            synchronized(getClass()) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(COUNT+1, counter.get());
	}
	
	@Test public void testQGraphicsWidgetInsertActions() throws InterruptedException {
		AtomicInteger counter = new AtomicInteger();
		{
			QGraphicsWidget w = new QGraphicsWidget();
	        for (int i=0; i<COUNT; ++i) {
                QAction act = new QAction("action" + i, null);
                General.internalAccess.registerCleaner(act, counter::incrementAndGet);
                w.insertActions(null, QList.of(act));
                QApplication.processEvents();
                act = null;
	        }
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(getClass()) {
	            	Thread.sleep(25);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(0, counter.get());
	        List<QAction> actions = w.actions();
	        assertEquals(COUNT, actions.size());
	        for (int i=0; i<COUNT; ++i) {
	            assertTrue(actions.get(i) != null);
	            assertEquals("action" + i, actions.get(i).text());
	        }
            General.internalAccess.registerCleaner(w, counter::incrementAndGet);
	        w = null;
		}
        for (int i = 0; i < 50 && counter.get()<COUNT+1; i++) {
            ApplicationInitializer.runGC();
            synchronized(getClass()) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(COUNT+1, counter.get());
	}
	
	@Test public void testQGraphicsWidgetInsertAction() throws InterruptedException {
		AtomicInteger counter = new AtomicInteger();
		{
			QGraphicsWidget w = new QGraphicsWidget();
	        for (int i=0; i<COUNT; ++i) {
                QAction act = new QAction("action" + i, null);
                General.internalAccess.registerCleaner(act, counter::incrementAndGet);
                w.insertAction(null, act);
                QApplication.processEvents();
                act = null;
	        }
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(getClass()) {
	            	Thread.sleep(25);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(0, counter.get());
	        List<QAction> actions = w.actions();
	        assertEquals(COUNT, actions.size());
	        for (int i=0; i<COUNT; ++i) {
	            assertTrue(actions.get(i) != null);
	            assertEquals("action" + i, actions.get(i).text());
	        }
            General.internalAccess.registerCleaner(w, counter::incrementAndGet);
	        w = null;
		}
        for (int i = 0; i < 50 && counter.get()<COUNT+1; i++) {
            ApplicationInitializer.runGC();
            synchronized(getClass()) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(COUNT+1, counter.get());
	}
	
	@Test public void testQGraphicsWidgetAddActionTwice() throws InterruptedException {
		AtomicInteger counter = new AtomicInteger();
		QGraphicsWidget w = new QGraphicsWidget();
        for (int i=0; i<COUNT; ++i) {
            QAction act = new QAction("action" + i, null);
            General.internalAccess.registerCleaner(act, counter::incrementAndGet);
            w.addAction(act);
            w.addAction(act);
            QApplication.processEvents();
            act = null;
        }
        for (int i = 0; i < 20 && counter.get()==0; i++) {
            ApplicationInitializer.runGC();
            synchronized(getClass()) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(0, counter.get());
        QList<QAction> actions = w.actions();
        assertEquals(COUNT, actions.size());
        for (int i=0; i<COUNT; ++i) {
        	QAction action = actions.removeFirst();
            assertTrue(action != null);
            assertEquals("action" + i, action.text());
            w.removeAction(action);
            assertTrue("QAction does not have java ownership", General.internalAccess.isJavaOwnership(action));
            action = null;
            QApplication.processEvents();
        }
        actions = null;
        for (int i = 0; i < 120 && counter.get()<COUNT; i++) {
            ApplicationInitializer.runGC();
            synchronized(getClass()) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        assertEquals(0, w.actions().size());
        Assert.assertEquals(COUNT, counter.get());
        w = null;
        ApplicationInitializer.runGC();
	}
	
	@Test public void testQGraphicsWidgetAddAndRemoveAction() throws InterruptedException {
		AtomicInteger counter = new AtomicInteger();
		QGraphicsWidget w = new QGraphicsWidget();
        for (int i=0; i<COUNT; ++i) {
            QAction act = new QAction("action" + i, null);
            General.internalAccess.registerCleaner(act, counter::incrementAndGet);
            w.addAction(act);
            QApplication.processEvents();
            act = null;
        }
        for (int i = 0; i < 20 && counter.get()==0; i++) {
            ApplicationInitializer.runGC();
            synchronized(getClass()) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(0, counter.get());
        QList<QAction> actions = w.actions();
        assertEquals(COUNT, actions.size());
        for (int i=0; i<COUNT; ++i) {
        	QAction action = actions.removeFirst();
            assertTrue(action != null);
            assertEquals("action" + i, action.text());
            w.removeAction(action);
            assertTrue("QAction does not have java ownership", General.internalAccess.isJavaOwnership(action));
            action = null;
            QApplication.processEvents();
        }
        actions = null;
        for (int i = 0; i < 120 && counter.get()<COUNT; i++) {
            ApplicationInitializer.runGC();
            synchronized(getClass()) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        assertEquals(0, w.actions().size());
        Assert.assertEquals(COUNT, counter.get());
        w = null;
        ApplicationInitializer.runGC();
	}
	
	@Test 
	public final void testQGraphicsWidgetSetStyle() throws InterruptedException {
		AtomicInteger counter = new AtomicInteger();
		{
			QGraphicsWidget w = new QGraphicsWidget();
	        {
	        	QStyle style = new QCommonStyle();
	        	General.internalAccess.registerCleaner(style, counter::incrementAndGet);
	        	w.setStyle(style);
	            style = null;
	        }
	        
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(ApplicationInitializer.class) {
	            	Thread.sleep(25+i*10);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(0, counter.get());
	        assertTrue(w.style() != null);
	        w = null;
		}
		for (int i = 0; i < 50 && counter.get()==0; i++) {
            ApplicationInitializer.runGC();
            synchronized(ApplicationInitializer.class) {
            	Thread.sleep(25+i*10);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(1, counter.get());
        ApplicationInitializer.runGC();
	}
	
	@Test 
	public final void testQGraphicsWidgetSetStyleNull() throws InterruptedException {
		AtomicInteger counter = new AtomicInteger();
		{
			QGraphicsWidget w = new QGraphicsWidget();
	        {
	        	QStyle style = new QCommonStyle();
	        	General.internalAccess.registerCleaner(style, counter::incrementAndGet);
	        	w.setStyle(style);
	        	w.setStyle(null);
	            style = null;
	        }
	        
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(ApplicationInitializer.class) {
	            	Thread.sleep(25+i*10);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(1, counter.get());
	        w = null;
		}
        ApplicationInitializer.runGC();
	}
	
	static class GraphicsWidget extends QGraphicsWidget{
		public final void _setGraphicsItem(io.qt.widgets.QGraphicsItem item){
			setGraphicsItem(item);
		}
	}
	
	static class GraphicsLayout extends QGraphicsLayout{
		public final void _setGraphicsItem(io.qt.widgets.QGraphicsItem item){
			setGraphicsItem(item);
		}
		@Override
		public int count() {
			return 0;
		}
		@Override
		public @Nullable QGraphicsLayoutItem itemAt(int i) {
			return null;
		}
		@Override
		public void removeAt(int index) {
		}
		@Override
		protected @NonNull QSizeF sizeHint(Qt.@NonNull SizeHint which, @NonNull QSizeF constraint) {
			return null;
		}
	}
	
	static class GraphicsLayoutItem implements QGraphicsLayoutItem{
		public final void _setGraphicsItem(io.qt.widgets.QGraphicsItem item){
			QGraphicsLayoutItem.MemberAccess.of(this).setGraphicsItem(item);
		}
		
		protected io.qt.core.QSizeF sizeHint(io.qt.core.Qt.SizeHint which, io.qt.core.QSizeF constraint) {
			return null;
		}
	}
	
	@Test 
	public final void testQGraphicsLayoutSetGraphicsItem() throws InterruptedException {
		AtomicInteger counter = new AtomicInteger();
		{
			GraphicsLayout w = new GraphicsLayout();
	        {
	        	QGraphicsItem item = new QGraphicsSimpleTextItem();
	        	General.internalAccess.registerCleaner(item, counter::incrementAndGet);
	        	w._setGraphicsItem(item);
	            item = null;
	        }
	        
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(ApplicationInitializer.class) {
	            	Thread.sleep(25+i*10);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(0, counter.get());
	        assertTrue(w.graphicsItem() != null);
	        w.dispose();
	        w = null;
		}
		for (int i = 0; i < 50 && counter.get()==0; i++) {
            ApplicationInitializer.runGC();
            synchronized(ApplicationInitializer.class) {
            	Thread.sleep(25+i*10);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(1, counter.get());
        ApplicationInitializer.runGC();
	}
	
	@Test 
	public final void testQGraphicsLayoutSetGraphicsItemNull() throws InterruptedException {
		AtomicInteger counter = new AtomicInteger();
		{
			GraphicsLayout w = new GraphicsLayout();
	        {
	        	QGraphicsItem item = new QGraphicsSimpleTextItem();
	        	General.internalAccess.registerCleaner(item, counter::incrementAndGet);
	        	w._setGraphicsItem(item);
	        	w._setGraphicsItem(null);
	            item = null;
	        }
	        
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(ApplicationInitializer.class) {
	            	Thread.sleep(25+i*10);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(1, counter.get());
	        assertTrue(w.graphicsItem() == null);
	        w = null;
		}
        ApplicationInitializer.runGC();
	}
	
	@Test 
	public final void testQGraphicsLayoutItemSetGraphicsItem() throws InterruptedException {
		AtomicInteger counter = new AtomicInteger();
		{
			GraphicsLayoutItem w = new GraphicsLayoutItem();
	        {
	        	QGraphicsItem item = new QGraphicsSimpleTextItem();
	        	General.internalAccess.registerCleaner(item, counter::incrementAndGet);
	        	w._setGraphicsItem(item);
	            item = null;
	        }
	        
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(ApplicationInitializer.class) {
	            	Thread.sleep(25+i*10);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(0, counter.get());
	        assertTrue(w.graphicsItem() != null);
	        w.dispose();
	        w = null;
		}
		for (int i = 0; i < 50 && counter.get()==0; i++) {
            ApplicationInitializer.runGC();
            synchronized(ApplicationInitializer.class) {
            	Thread.sleep(25+i*10);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(1, counter.get());
        ApplicationInitializer.runGC();
	}
	
	@Test 
	public final void testQGraphicsLayoutItemSetGraphicsItemNull() throws InterruptedException {
		AtomicInteger counter = new AtomicInteger();
		{
			GraphicsLayoutItem w = new GraphicsLayoutItem();
	        {
	        	QGraphicsItem item = new QGraphicsSimpleTextItem();
	        	General.internalAccess.registerCleaner(item, counter::incrementAndGet);
	        	w._setGraphicsItem(item);
	        	w._setGraphicsItem(null);
	            item = null;
	        }
	        
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(ApplicationInitializer.class) {
	            	Thread.sleep(25+i*10);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(1, counter.get());
	        assertTrue(w.graphicsItem() == null);
	        w = null;
		}
        ApplicationInitializer.runGC();
	}
	
	@Test 
	public final void testQGraphicsWidgetSetGraphicsItem() throws InterruptedException {
		AtomicInteger counter = new AtomicInteger();
		{
			GraphicsWidget w = new GraphicsWidget();
	        {
	        	QGraphicsItem item = new QGraphicsSimpleTextItem();
	        	General.internalAccess.registerCleaner(item, counter::incrementAndGet);
	        	w._setGraphicsItem(item);
	            item = null;
	        }
	        
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(ApplicationInitializer.class) {
	            	Thread.sleep(25+i*10);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(0, counter.get());
	        assertTrue(w.graphicsItem() != null);
	        w.dispose();
	        w = null;
		}
		for (int i = 0; i < 50 && counter.get()==0; i++) {
            ApplicationInitializer.runGC();
            synchronized(ApplicationInitializer.class) {
            	Thread.sleep(25+i*10);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(1, counter.get());
        ApplicationInitializer.runGC();
	}
	
	@Test 
	public final void testQGraphicsWidgetSetGraphicsItemNull() throws InterruptedException {
		AtomicInteger counter = new AtomicInteger();
		{
			GraphicsWidget w = new GraphicsWidget();
	        {
	        	QGraphicsItem item = new QGraphicsSimpleTextItem();
	        	General.internalAccess.registerCleaner(item, counter::incrementAndGet);
	        	w._setGraphicsItem(item);
	        	w._setGraphicsItem(null);
	            item = null;
	        }
	        
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(ApplicationInitializer.class) {
	            	Thread.sleep(25+i*10);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(1, counter.get());
	        assertTrue(w.graphicsItem() == null);
	        w = null;
		}
        ApplicationInitializer.runGC();
	}
	
	@Test 
	public final void testQGraphicsItemSetEffect() throws InterruptedException {
		AtomicInteger counter = new AtomicInteger();
		{
			QGraphicsItem o = new QGraphicsSimpleTextItem();
	        {
	        	QGraphicsEffect effect = new QGraphicsBlurEffect();
	        	General.internalAccess.registerCleaner(effect, counter::incrementAndGet);
	        	o.setGraphicsEffect(effect);
	            effect = null;
	        }
	        
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(ApplicationInitializer.class) {
	            	Thread.sleep(25+i*10);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(0, counter.get());
	        assertTrue(o.graphicsEffect() != null);
	        o = null;
		}
		for (int i = 0; i < 50 && counter.get()==0; i++) {
            ApplicationInitializer.runGC();
            synchronized(ApplicationInitializer.class) {
            	Thread.sleep(25+i*10);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(1, counter.get());
        ApplicationInitializer.runGC();
	}
	
	@Test 
	public final void testQGraphicsItemSetEffectNull() throws InterruptedException {
		AtomicInteger counter = new AtomicInteger();
		{
			QGraphicsItem o = new QGraphicsSimpleTextItem();
	        {
	        	QGraphicsEffect effect = new QGraphicsBlurEffect();
	        	General.internalAccess.registerCleaner(effect, counter::incrementAndGet);
	        	o.setGraphicsEffect(effect);
	        	o.setGraphicsEffect(null);
	            effect = null;
	        }
	        
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(ApplicationInitializer.class) {
	            	Thread.sleep(25+i*10);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(1, counter.get());
	        o = null;
		}
        ApplicationInitializer.runGC();
	}
	
	@Test 
	public final void testQGraphicsItemInstallSceneEventFilter() throws InterruptedException {
		AtomicInteger counter = new AtomicInteger();
		{
			QGraphicsItem o = new QGraphicsSimpleTextItem();
	        {
	        	QGraphicsItem filter = new QGraphicsSimpleTextItem();
	        	General.internalAccess.registerCleaner(filter, counter::incrementAndGet);
	        	o.installSceneEventFilter(filter);
	            filter = null;
	        }
	        
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(ApplicationInitializer.class) {
	            	Thread.sleep(25+i*10);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(0, counter.get());
	        o = null;
		}
		for (int i = 0; i < 50 && counter.get()==0; i++) {
            ApplicationInitializer.runGC();
            synchronized(ApplicationInitializer.class) {
            	Thread.sleep(25+i*10);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(1, counter.get());
        ApplicationInitializer.runGC();
	}
	
	@Test 
	public final void testQGraphicsItemInstallRemoveSceneEventFilter() throws InterruptedException {
		AtomicInteger counter = new AtomicInteger();
		{
			QGraphicsItem o = new QGraphicsSimpleTextItem();
	        {
	        	QGraphicsItem filter = new QGraphicsSimpleTextItem();
	        	General.internalAccess.registerCleaner(filter, counter::incrementAndGet);
	        	o.installSceneEventFilter(filter);
	        	o.removeSceneEventFilter(filter);
	            filter = null;
	        }
	        
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(ApplicationInitializer.class) {
	            	Thread.sleep(25+i*10);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(1, counter.get());
	        o = null;
		}
        ApplicationInitializer.runGC();
	}
	
	static class GraphicsItem implements QGraphicsItem{

		@Override
		public @NonNull QRectF boundingRect() {
			return null;
		}

		@Override
		public void paint(@Nullable QPainter painter, @Nullable QStyleOptionGraphicsItem option, @Nullable QWidget widget) {
		}
	}
	
	@Test 
	public final void testQGraphicsItemInterfaceSetEffect() throws InterruptedException {
		AtomicInteger counter = new AtomicInteger();
		{
			QGraphicsItem o = new GraphicsItem();
	        {
	        	QGraphicsEffect effect = new QGraphicsBlurEffect();
	        	General.internalAccess.registerCleaner(effect, counter::incrementAndGet);
	        	o.setGraphicsEffect(effect);
	            effect = null;
	        }
	        
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(ApplicationInitializer.class) {
	            	Thread.sleep(25+i*10);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(0, counter.get());
	        assertTrue(o.graphicsEffect() != null);
	        o = null;
		}
		for (int i = 0; i < 50 && counter.get()==0; i++) {
            ApplicationInitializer.runGC();
            synchronized(ApplicationInitializer.class) {
            	Thread.sleep(25+i*10);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(1, counter.get());
        ApplicationInitializer.runGC();
	}
	
	@Test 
	public final void testQGraphicsItemInterfaceSetEffectNull() throws InterruptedException {
		AtomicInteger counter = new AtomicInteger();
		{
			QGraphicsItem o = new GraphicsItem();
	        {
	        	QGraphicsEffect effect = new QGraphicsBlurEffect();
	        	General.internalAccess.registerCleaner(effect, counter::incrementAndGet);
	        	o.setGraphicsEffect(effect);
	        	o.setGraphicsEffect(null);
	            effect = null;
	        }
	        
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(ApplicationInitializer.class) {
	            	Thread.sleep(25+i*10);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(1, counter.get());
	        o = null;
		}
        ApplicationInitializer.runGC();
	}
	
	@Test 
	public final void testQGraphicsItemInterfaceInstallSceneEventFilter() throws InterruptedException {
		AtomicInteger counter = new AtomicInteger();
		{
			QGraphicsItem o = new GraphicsItem();
	        {
	        	QGraphicsItem filter = new QGraphicsSimpleTextItem();
	        	General.internalAccess.registerCleaner(filter, counter::incrementAndGet);
	        	o.installSceneEventFilter(filter);
	            filter = null;
	        }
	        
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(ApplicationInitializer.class) {
	            	Thread.sleep(25+i*10);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(0, counter.get());
	        o = null;
		}
		for (int i = 0; i < 50 && counter.get()==0; i++) {
            ApplicationInitializer.runGC();
            synchronized(ApplicationInitializer.class) {
            	Thread.sleep(25+i*10);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(1, counter.get());
	}
	
	@Test 
	public final void testQGraphicsItemInterfaceInstallRemoveSceneEventFilter() throws InterruptedException {
		AtomicInteger counter = new AtomicInteger();
		{
			QGraphicsItem o = new GraphicsItem();
	        {
	        	QGraphicsItem filter = new QGraphicsSimpleTextItem();
	        	General.internalAccess.registerCleaner(filter, counter::incrementAndGet);
	        	o.installSceneEventFilter(filter);
	        	o.removeSceneEventFilter(filter);
	            filter = null;
	        }
	        
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(ApplicationInitializer.class) {
	            	Thread.sleep(25+i*10);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(1, counter.get());
	        o = null;
		}
        ApplicationInitializer.runGC();
	}
	
	@Test 
	public final void testQGraphicsObjectSetEffect() throws InterruptedException {
		AtomicInteger counter = new AtomicInteger();
		{
			QGraphicsObject o = new QGraphicsWidget();
	        {
	        	QGraphicsEffect effect = new QGraphicsBlurEffect();
	        	General.internalAccess.registerCleaner(effect, counter::incrementAndGet);
	        	o.setGraphicsEffect(effect);
	            effect = null;
	        }
	        
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(ApplicationInitializer.class) {
	            	Thread.sleep(25+i*10);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(0, counter.get());
	        assertTrue(o.graphicsEffect() != null);
	        o = null;
		}
		for (int i = 0; i < 50 && counter.get()==0; i++) {
            ApplicationInitializer.runGC();
            synchronized(ApplicationInitializer.class) {
            	Thread.sleep(25+i*10);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(1, counter.get());
        ApplicationInitializer.runGC();
	}
	
	@Test 
	public final void testQGraphicsObjectSetEffectNull() throws InterruptedException {
		AtomicInteger counter = new AtomicInteger();
		{
			QGraphicsObject o = new QGraphicsWidget();
	        {
	        	QGraphicsEffect effect = new QGraphicsBlurEffect();
	        	General.internalAccess.registerCleaner(effect, counter::incrementAndGet);
	        	o.setGraphicsEffect(effect);
	        	o.setGraphicsEffect(null);
	            effect = null;
	        }
	        
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(ApplicationInitializer.class) {
	            	Thread.sleep(25+i*10);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(1, counter.get());
	        o = null;
		}
        ApplicationInitializer.runGC();
	}
	
	@Test 
	public final void testQGraphicsObjectInstallSceneEventFilter() throws InterruptedException {
		AtomicInteger counter = new AtomicInteger();
		{
			QGraphicsObject o = new QGraphicsWidget();
	        {
	        	QGraphicsItem filter = new QGraphicsSimpleTextItem();
	        	General.internalAccess.registerCleaner(filter, counter::incrementAndGet);
	        	o.installSceneEventFilter(filter);
	            filter = null;
	        }
	        
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(ApplicationInitializer.class) {
	            	Thread.sleep(25+i*10);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(0, counter.get());
	        o = null;
		}
		for (int i = 0; i < 50 && counter.get()==0; i++) {
            ApplicationInitializer.runGC();
            synchronized(ApplicationInitializer.class) {
            	Thread.sleep(25+i*10);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(1, counter.get());
        ApplicationInitializer.runGC();
	}
	
	@Test 
	public final void testQGraphicsObjectInstallRemoveSceneEventFilter() throws InterruptedException {
		AtomicInteger counter = new AtomicInteger();
		{
			QGraphicsObject o = new QGraphicsWidget();
	        {
	        	QGraphicsItem filter = new QGraphicsSimpleTextItem();
	        	General.internalAccess.registerCleaner(filter, counter::incrementAndGet);
	        	o.installSceneEventFilter(filter);
	        	o.removeSceneEventFilter(filter);
	            filter = null;
	        }
	        
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(ApplicationInitializer.class) {
	            	Thread.sleep(25+i*10);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(1, counter.get());
	        o = null;
		}
        ApplicationInitializer.runGC();
	}
	
	@Test 
	public final void testQGraphicsViewSetScene() throws InterruptedException {
		AtomicInteger counter = new AtomicInteger();
		{
			QGraphicsView o = new QGraphicsView();
	        {
	        	QGraphicsScene scene = new QGraphicsScene();
	        	General.internalAccess.registerCleaner(scene, counter::incrementAndGet);
	        	o.setScene(scene);
	            scene = null;
	        }
	        
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(ApplicationInitializer.class) {
	            	Thread.sleep(25+i*10);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(0, counter.get());
	        assertTrue(o.scene() != null);
	        o = null;
		}
		for (int i = 0; i < 50 && counter.get()==0; i++) {
            ApplicationInitializer.runGC();
            synchronized(ApplicationInitializer.class) {
            	Thread.sleep(25+i*10);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(1, counter.get());
        ApplicationInitializer.runGC();
	}
	
	@Test 
	public final void testQGraphicsViewSetSceneNull() throws InterruptedException {
		AtomicInteger counter = new AtomicInteger();
		{
			QGraphicsView o = new QGraphicsView();
	        {
	        	QGraphicsScene scene = new QGraphicsScene();
	        	General.internalAccess.registerCleaner(scene, counter::incrementAndGet);
	        	o.setScene(scene);
	        	o.setScene(null);
	            scene = null;
	        }
	        
	        for (int i = 0; i < 20 && counter.get()==0; i++) {
	            ApplicationInitializer.runGC();
	            synchronized(ApplicationInitializer.class) {
	            	Thread.sleep(25+i*10);
	            }
	            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	            QCoreApplication.processEvents();
			}
	        Assert.assertEquals(1, counter.get());
	        o = null;
		}
        ApplicationInitializer.runGC();
	}
}
