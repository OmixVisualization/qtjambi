/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2015 Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
** $END_LICENSE$

**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

package org.qtjambi.autotests;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import org.junit.AfterClass;
import org.junit.BeforeClass;
import org.junit.Test;

import org.qtjambi.autotests.generated.PolymorphicType;
import org.qtjambi.autotests.generated.CustomStyleOption;
import org.qtjambi.autotests.generated.CustomEvent;
import org.qtjambi.qt.QNoNativeResourcesException;
import org.qtjambi.qt.QtBlockedSlot;
import org.qtjambi.qt.core.QCoreApplication;
import org.qtjambi.qt.core.QEvent;
import org.qtjambi.qt.core.QEventLoop;
import org.qtjambi.qt.core.QRect;
import org.qtjambi.qt.core.QRectF;
import org.qtjambi.qt.core.QSize;
import org.qtjambi.qt.core.Qt.Orientations;
import org.qtjambi.qt.widgets.*;
import org.qtjambi.qt.gui.QPaintEvent;
import org.qtjambi.qt.gui.QPainter;

/*
class CustomStyle extends QWindowsStyle {
    public static QStyleOption m_option = null;

    @Override
    public void drawControl(ControlElement element, QStyleOption opt, QPainter p, QWidget w) {
        m_option = opt;
    }
}*/

public class TestPolymorphicTypes extends QWidget {
    @BeforeClass
    public static void testInitialize() throws Exception {
        Utils.println(2, "TestPolymorphicTypes.testInitialize(): begin");
        QApplicationTest.testInitialize(null);
        Utils.println(2 ,"TestPolymorphicTypes.testInitialize(): done");
    }
    
    /**
     * This method tests, if QtJambi correctly maps types subclassing QObject and 
     * QGraphicsItem (QGraphicsObject) to the correct Java type.   
     * @author Peter Droste
     */
    @Test
	public void testPolymorphicQGraphicsItem() {
		QGraphicsItemInterface item1 = new QGraphicsItem(){
			@Override
			@QtBlockedSlot
			public QRectF boundingRect() {
				return new QRectF(0,0,100,100);
			}

			@Override
			@QtBlockedSlot
			public void paint(QPainter painter, QStyleOptionGraphicsItem option, QWidget widget) {
			}
		};
		QGraphicsItemInterface item2 = new QGraphicsTextItem("text");
		QGraphicsItemInterface item3 = new QGraphicsObject(){
			@Override
			@QtBlockedSlot
			public QRectF boundingRect() {
				return new QRectF(0,0,200,200);
			}

			@Override
			@QtBlockedSlot
			public void paint(QPainter painter, QStyleOptionGraphicsItem option, QWidget widget) {
			}
		};
		QGraphicsItemInterface item4 = new QGraphicsWidget();
		QGraphicsItemInterface item5 = new QGraphicsProxyWidget();
		QGraphicsItemInterface item6 = new QGraphicsRectItem(0,0,300,300);
		QGraphicsItemInterface item7 = new MyGraphicsObject();
		QGraphicsItemInterface item8 = new MyGraphicsItem();
		
		QGraphicsScene scene = new QGraphicsScene();
		scene.addItem(item1);
		scene.addItem(item2);
		scene.addItem(item3);
		scene.addItem(item4);
		scene.addItem(item5);
		scene.addItem(item6);
		scene.addItem(item7);
		scene.addItem(item8);
		// only test if modules are generated:
		try {
			QGraphicsItemInterface item9 = (QGraphicsItemInterface)Class.forName("org.qtjambi.qt.declarative.QDeclarativeItem").newInstance();
			scene.addItem(item9);
		} catch (Throwable e) {}
		try {
			QGraphicsItemInterface item10 = (QGraphicsItemInterface)Class.forName("org.qtjambi.qt.qt3dquick.QDeclarativeItem3D").newInstance();
			scene.addItem(item10);
		} catch (Throwable e) {}
		try {
			QGraphicsItemInterface item11 = (QGraphicsItemInterface)Class.forName("org.qtjambi.qt.svg.QGraphicsSvgItem").newInstance();
			scene.addItem(item11);
		} catch (Throwable e) {}
		for (QGraphicsItemInterface item : scene.items()) {
			if(item.toGraphicsObject()!=null){
				assertEquals(item.toGraphicsObject().getClass().getName()+" == "+item.getClass().getName(), item.toGraphicsObject().getClass(), item.getClass());
			}else{
				assertFalse("class is not ConcreteWrapper "+item.getClass().getName(), item.getClass().getName().equals("org.qtjambi.qt.widgets.QGraphicsItem$ConcreteWrapper"));
			}
		}
    }
	
	public static class MyGraphicsObject extends QGraphicsObject{
		@Override
		@QtBlockedSlot
		public QRectF boundingRect() {
			return new QRectF(0,0,200,200);
		}

		@Override
		@QtBlockedSlot
		public void paint(QPainter painter, QStyleOptionGraphicsItem option, QWidget widget) {
		}
	}
	
	public static class MyGraphicsItem extends QGraphicsItem{
		@Override
		@QtBlockedSlot
		public QRectF boundingRect() {
			return new QRectF(0,0,200,200);
		}

		@Override
		@QtBlockedSlot
		public void paint(QPainter painter, QStyleOptionGraphicsItem option, QWidget widget) {
		}
	}
	
	/**
     * This method tests, if QtJambi correctly maps types subclassing QObject and 
     * QLayoutItem (QSpacerItem, QWidgetItem, QLayout) to the correct Java type.   
     * @author Peter Droste
     */
	@Test
	public void testPolymorphicQLayoutItem() {
		QLayoutItemInterface item1 = new QLayoutItem(){

			@Override
			@QtBlockedSlot
			public Orientations expandingDirections() {
				return null;
			}

			@Override
			@QtBlockedSlot
			public QRect geometry() {
				return null;
			}

			@Override
			@QtBlockedSlot
			public boolean isEmpty() {
				return false;
			}

			@Override
			@QtBlockedSlot
			public QSize maximumSize() {
				return null;
			}

			@Override
			@QtBlockedSlot
			public QSize minimumSize() {
				return null;
			}

			@Override
			@QtBlockedSlot
			public void setGeometry(QRect arg__1) {
			}

			@Override
			@QtBlockedSlot
			public QSize sizeHint() {
				return null;
			}
		};
		QLayoutItemInterface item2 = new QFormLayout();
		QLayoutItemInterface item3 = new QGridLayout(){};
		QLayoutItemInterface item4 = new QSpacerItem(0,0);
		QLayoutItemInterface item5 = new QWidgetItem(new QWidget());
		QLayoutItemInterface item6 = new QStackedLayout();
		QLayoutItemInterface item7 = new MyLayout();
		
		QHBoxLayout layout = new QHBoxLayout();
		layout.addItem(item1);
		layout.addItem(item2);
		layout.addItem(item3);
		layout.addItem(item4);
		layout.addItem(item5);
		layout.addItem(item6);
		layout.addItem(item7);
		for (int j = 0; j < layout.count(); j++) {
			QLayoutItemInterface item = layout.itemAt(j);
			assertFalse("class is not ConcreteWrapper "+item.getClass().getName(), item.getClass().getName().equals("org.qtjambi.qt.widgets.QLayoutItem$ConcreteWrapper"));
		}
    }
	
	public static class MyLayout extends QLayout{

		@Override
		@QtBlockedSlot
		public void addItem(QLayoutItemInterface arg__1) {
			// TODO Auto-generated method stub
			
		}

		@Override
		@QtBlockedSlot
		public int count() {
			// TODO Auto-generated method stub
			return 0;
		}

		@Override
		@QtBlockedSlot
		public QLayoutItemInterface itemAt(int index) {
			// TODO Auto-generated method stub
			return null;
		}

		@Override
		@QtBlockedSlot
		public QSize sizeHint() {
			// TODO Auto-generated method stub
			return null;
		}

		@Override
		@QtBlockedSlot
		public QLayoutItemInterface takeAt(int index) {
			// TODO Auto-generated method stub
			return null;
		}
	}

    // TODO explain why we don't just use QApplicaitonTest subclass or #testDispose() method ?
    @AfterClass
    public static void testDispose() throws Exception {
        Utils.println(2, "TestPolymorphicTypes.testDispose(): begin");
        if(Utils.releaseNativeResources() > 0) {
            System.gc();
            System.runFinalization();
        }
        QApplication.processEvents();
        QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDelete.value());
        QApplication.quit();
        System.err.flush();
        System.out.flush();
        QApplication app = QApplication.instance();
        if(app != null)
            app.dispose();
        try {
            Utils.println(3, "TestPolymorphicTypes.testDispose(): done  app="+app);
        } catch(QNoNativeResourcesException e) {
            Utils.println(3, "TestPolymorphicTypes.testDispose(): done  org.qtjambi.qt.QNoNativeResourcesException: app="+e.getMessage());
        }
        app = null;		// kill hard-reference
        Utils.println(3, "TestPolymorphicTypes.testDispose(): shutdown PRE");
        QApplication.shutdown();
        Utils.println(3, "TestPolymorphicTypes.testDispose(): shutdown POST");

        QtJambiUnittestTools.getObjectCount(1, 0);  // fflush(stdout)
        QtJambiUnittestTools.getObjectCount(2, 0);  // fflush(stderr)
        int objectCount = QtJambiUnittestTools.getObjectCount(3, 0);  // QtJambiLink::QtJambiLink_dump()
        QtJambiUnittestTools.getObjectCount(2, 0);  // fflush(stderr)
        Utils.println(3, "TestPolymorphicTypes.testDispose(): end objectCount="+objectCount);

        if(objectCount == 0)
            return;  // optimization due to class loading causing some references to be set

        QtJambiUnittestTools.clearStaticReferences();
        System.gc();
        System.runFinalization();
        System.gc();
        System.runFinalization();

        QtJambiUnittestTools.getObjectCount(1, 0);  // fflush(stdout)
        QtJambiUnittestTools.getObjectCount(2, 0);  // fflush(stderr)
        objectCount = QtJambiUnittestTools.getObjectCount(3, 0);  // QtJambiLink::QtJambiLink_dump()
        QtJambiUnittestTools.getObjectCount(2, 0);  // fflush(stderr)
        Utils.println(3, "TestPolymorphicTypes.testDispose(): end objectCount="+objectCount);
    }

    @Test
    public void testGetPaintEvent()
    {
        QEvent event = PolymorphicType.getPaintEvent();
        assertEquals(event.type(), QEvent.Type.Paint);
        assertTrue(event instanceof QPaintEvent);
    }

    @Test
    public void testGetCustomEvent()
    {
        QEvent event = PolymorphicType.getCustomEvent(10);
        assertEquals(event.type(), QEvent.Type.resolve(QEvent.Type.User.value() + 1));
        assertTrue(event instanceof CustomEvent);

        CustomEvent customEvent = (CustomEvent) event;
        assertEquals(customEvent.m_something(), 10);
    }

    @Test
    public void testSendPaintEvent()
    {
        PolymorphicType.sendPaintEvent(this);
        assertEquals(QEvent.Type.Paint, eventType);
        assertEquals(QPaintEvent.class, eventClass);
        assertEquals(0, m_event.nativeId());
    }

    @Test
    public void testSendCustomEvent()
    {
        PolymorphicType.sendCustomEvent(this, 20);
        assertEquals(QEvent.Type.resolve(QEvent.Type.User.value() + 1), eventType);
        assertEquals(CustomEvent.class, eventClass);
        assertEquals(0, m_event.nativeId());
        assertEquals(20, customEventSomething);
    }

    @Test
    public void testGetButtonStyleOption()
    {
        QStyleOption opt = PolymorphicType.getButtonStyleOption();
        assertEquals(QStyleOption.OptionType.SO_Button.value(), opt.type());
        assertTrue(opt==null ? "" : opt.getClass().getName(), opt instanceof QStyleOptionButton);
    }

    @Test
    public void testGetCustomStyleOption()
    {
        QStyleOption opt = PolymorphicType.getCustomStyleOption(30);
        assertTrue(opt instanceof CustomStyleOption);
        assertEquals(QStyleOption.OptionType.SO_CustomBase.value() + 1, opt.type());

        CustomStyleOption customOpt = (CustomStyleOption) opt;
        assertEquals(customOpt.m_something(), 30);
    }

    @Test
    public void testGetUnmappedCustomStyleOption()
    {
        QStyleOption opt = PolymorphicType.getUnmappedCustomStyleOption();
        assertEquals(opt.type(), QStyleOption.OptionType.SO_Default.value());
        assertTrue(opt instanceof QStyleOption);
        assertEquals(opt.getClass().getName(), "org.qtjambi.qt.widgets.QStyleOption");
    }

    /*
    @Test
    public void testSendButtonStyleOption()
    {
        this.setStyle(new CustomStyle());
        PolymorphicType.sendButtonStyleOption(this);
        assertTrue(CustomStyle.m_option != null);
        assertEquals(CustomStyle.m_option.type(), QStyleOption.OptionType.SO_Button.value());
        assertTrue(CustomStyle.m_option instanceof QStyleOptionButton);
    }

    @Test
    public void testSendCustomStyleOption()
    {
        this.setStyle(new CustomStyle());
        PolymorphicType.sendCustomStyleOption(this, 40);
        assertTrue(CustomStyle.m_option != null);
        assertEquals(CustomStyle.m_option.type(), QStyleOption.OptionType.SO_CustomBase.value() + 1);
        assertTrue(CustomStyle.m_option instanceof CustomStyleOption);

        CustomStyleOption customOpt = (CustomStyleOption) CustomStyle.m_option;
        assertEquals(customOpt.m_something(), 40);
    }

    @Test
    public void testSendUnmappedCustomStyleOption()
    {
        this.setStyle(new CustomStyle());
        PolymorphicType.sendUnmappedCustomStyleOption(this);
        assertTrue(CustomStyle.m_option != null);
        assertEquals(CustomStyle.m_option.type(), QStyleOption.OptionType.SO_Default.value());
        assertTrue(CustomStyle.m_option instanceof QStyleOption);
        assertEquals(CustomStyle.m_option.getClass().getName(), "org.qtjambi.qt.gui.QStyleOption");
    }
    */

    private int customEventSomething = 0;
    private QEvent.Type eventType = null;
    private Class<?> eventClass = null;
    private QEvent m_event = null;
    @Override
    public boolean event(QEvent arg__1) {
        m_event = arg__1;
        eventClass = m_event.getClass();
        eventType = m_event.type();
        if (m_event instanceof CustomEvent)
            customEventSomething = ((CustomEvent)m_event).m_something();
        else
            customEventSomething = -1;

        return super.event(arg__1);
    }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestPolymorphicTypes.class.getName());
    }
}
