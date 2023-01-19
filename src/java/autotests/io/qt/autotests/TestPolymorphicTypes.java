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
import static org.junit.Assert.assertTrue;

import org.junit.After;
import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.QtUninvokable;
import io.qt.autotests.generated.CustomEvent;
import io.qt.autotests.generated.CustomStyleOption;
import io.qt.autotests.generated.General;
import io.qt.autotests.generated.PolymorphicType;
import io.qt.core.QEvent;
import io.qt.core.QRect;
import io.qt.core.QRectF;
import io.qt.core.QSize;
import io.qt.core.Qt.Orientations;
import io.qt.gui.QPaintEvent;
import io.qt.gui.QPainter;
import io.qt.widgets.QApplication;
import io.qt.widgets.QFormLayout;
import io.qt.widgets.QGraphicsItem;
import io.qt.widgets.QGraphicsObject;
import io.qt.widgets.QGraphicsProxyWidget;
import io.qt.widgets.QGraphicsRectItem;
import io.qt.widgets.QGraphicsScene;
import io.qt.widgets.QGraphicsTextItem;
import io.qt.widgets.QGraphicsWidget;
import io.qt.widgets.QGridLayout;
import io.qt.widgets.QHBoxLayout;
import io.qt.widgets.QLayout;
import io.qt.widgets.QLayoutItem;
import io.qt.widgets.QProxyStyle;
import io.qt.widgets.QSlider;
import io.qt.widgets.QSpacerItem;
import io.qt.widgets.QStackedLayout;
import io.qt.widgets.QStyleOption;
import io.qt.widgets.QStyleOptionButton;
import io.qt.widgets.QStyleOptionComplex;
import io.qt.widgets.QStyleOptionGraphicsItem;
import io.qt.widgets.QStyleOptionSlider;
import io.qt.widgets.QWidget;
import io.qt.widgets.QWidgetItem;

public class TestPolymorphicTypes extends ApplicationInitializer {
	
	@BeforeClass
    public static void testInitialize() throws Exception {
    	ApplicationInitializer.testInitializeWithWidgets();
    }
	
	public static class TestWidget extends QWidget{
		TestWidget(){
		}

	    private int customEventSomething = 0;
	    private QEvent.Type eventType = null;
	    private Class<?> eventClass = null;
	    private QEvent m_event = null;
	    @Override
	    public boolean event(QEvent arg__1) {
	        m_event = arg__1;
	        eventClass = General.internalAccess.getClass(m_event);
	        eventType = m_event.type();
	        if (m_event instanceof CustomEvent)
	            customEventSomething = ((CustomEvent)m_event).something();
	        else
	            customEventSomething = -1;

	        return super.event(arg__1);
	    }
	}
	
	private TestWidget widget;
	
	@Before
	public void init() {
		widget = new TestWidget();
	}
	
	@After
	public void after() {
		if(widget!=null)
			widget.dispose();
		widget = null;
	}
	
    /**
     * This method tests, if QtJambi correctly maps types subclassing QObject and 
     * QGraphicsItem (QGraphicsObject) to the correct Java type.   
     * @author Peter Droste
     */
    @Test
	public void testPolymorphicQGraphicsItem() {
		QGraphicsItem item1 = new QGraphicsItem.Impl(){
			@Override
			@QtUninvokable
			public QRectF boundingRect() {
				return new QRectF(0,0,100,100);
			}

			@Override
			@QtUninvokable
			public void paint(QPainter painter, QStyleOptionGraphicsItem option, QWidget widget) {
			}
		};
		QGraphicsItem item2 = new QGraphicsTextItem("text");
		QGraphicsItem item3 = new QGraphicsObject(){
			@Override
			@QtUninvokable
			public QRectF boundingRect() {
				return new QRectF(0,0,200,200);
			}

			@Override
			@QtUninvokable
			public void paint(QPainter painter, QStyleOptionGraphicsItem option, QWidget widget) {
			}
		};
		QGraphicsItem item4 = new QGraphicsWidget();
		QGraphicsItem item5 = new QGraphicsProxyWidget();
		QGraphicsItem item6 = new QGraphicsRectItem(0,0,300,300);
		QGraphicsItem item7 = new MyGraphicsObject();
		QGraphicsItem item8 = new MyGraphicsItem();
		QGraphicsItem item9 = new QGraphicsItem(){
			@Override
			@QtUninvokable
			public QRectF boundingRect() {
				return new QRectF(0,0,100,100);
			}

			@Override
			@QtUninvokable
			public void paint(QPainter painter, QStyleOptionGraphicsItem option, QWidget widget) {
			}
		};
		
		QGraphicsScene scene = new QGraphicsScene();
		scene.addItem(item1);
		scene.addItem(item2);
		scene.addItem(item3);
		scene.addItem(item4);
		scene.addItem(item5);
		scene.addItem(item6);
		scene.addItem(item7);
		scene.addItem(item8);
		scene.addItem(item9);
		// only test if modules are generated:
		try {
			QGraphicsItem item11 = (QGraphicsItem)Class.forName("io.qt.svg.QGraphicsSvgItem").getConstructor().newInstance();
			scene.addItem(item11);
		} catch (Throwable e) {}
		for (QGraphicsItem item : scene.items()) {
			if(item.toGraphicsObject()!=null){
				assertEquals(General.internalAccess.getClass(item.toGraphicsObject()).getName()+" == "+General.internalAccess.getClass(item).getName(), General.internalAccess.getClass(item.toGraphicsObject()), General.internalAccess.getClass(item));
			}else{
				assertFalse("class is not ConcreteWrapper "+General.internalAccess.getClass(item).getName(), General.internalAccess.getClass(item).getName().endsWith("$ConcreteWrapper"));
			}
		}
    }
	
	public static class MyGraphicsObject extends QGraphicsObject{
		@Override
		@QtUninvokable
		public QRectF boundingRect() {
			return new QRectF(0,0,200,200);
		}

		@Override
		@QtUninvokable
		public void paint(QPainter painter, QStyleOptionGraphicsItem option, QWidget widget) {
		}
	}
	
	public static class MyGraphicsItem extends QGraphicsItem.Impl{
		@Override
		@QtUninvokable
		public QRectF boundingRect() {
			return new QRectF(0,0,200,200);
		}

		@Override
		@QtUninvokable
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
		QLayoutItem item1 = new QLayoutItem.Impl(){

			@Override
			@QtUninvokable
			public Orientations expandingDirections() {
				return null;
			}

			@Override
			@QtUninvokable
			public QRect geometry() {
				return null;
			}

			@Override
			@QtUninvokable
			public boolean isEmpty() {
				return true;
			}

			@Override
			@QtUninvokable
			public QSize maximumSize() {
				return null;
			}

			@Override
			@QtUninvokable
			public QSize minimumSize() {
				return null;
			}

			@Override
			@QtUninvokable
			public void setGeometry(QRect arg__1) {
			}

			@Override
			@QtUninvokable
			public QSize sizeHint() {
				return null;
			}
		};
		QLayoutItem item2 = new QFormLayout();
		QLayoutItem item3 = new QGridLayout(){};
		QLayoutItem item4 = new QSpacerItem(0,0);
		QWidget w = new QWidget();
		QLayoutItem item5 = new QWidgetItem(w);
		QLayoutItem item6 = new QStackedLayout();
		QLayoutItem item7 = new MyLayout();
		QLayoutItem item8 = new QLayoutItem(){
			@Override
			public Orientations expandingDirections() {
				return null;
			}

			@Override
			public QRect geometry() {
				return null;
			}

			@Override
			public boolean isEmpty() {
				return true;
			}

			@Override
			public QSize maximumSize() {
				return null;
			}

			@Override
			public QSize minimumSize() {
				return null;
			}

			@Override
			public void setGeometry(QRect arg__1) {
			}

			@Override
			public QSize sizeHint() {
				return null;
			}
		};
		
		QHBoxLayout layout = new QHBoxLayout();
		layout.addItem(item1);
		layout.addItem(item2);
		layout.addItem(item3);
		layout.addItem(item4);
		layout.addItem(item5);
		layout.addItem(item6);
		layout.addItem(item7);
		layout.addItem(item8);
		for (int j = 0; j < layout.count(); j++) {
			QLayoutItem item = layout.itemAt(j);
			assertFalse(item==null);
			assertFalse("class is not ConcreteWrapper "+General.internalAccess.getClass(item).getName(), General.internalAccess.getClass(item).getName().endsWith("$ConcreteWrapper"));
		}
    }
	
	public static class MyLayout extends QLayout{

		@Override
		@QtUninvokable
		public void addItem(QLayoutItem arg__1) {
			// TODO Auto-generated method stub
			
		}

		@Override
		@QtUninvokable
		public int count() {
			// TODO Auto-generated method stub
			return 0;
		}

		@Override
		@QtUninvokable
		public QLayoutItem itemAt(int index) {
			// TODO Auto-generated method stub
			return null;
		}

		@Override
		@QtUninvokable
		public QSize sizeHint() {
			// TODO Auto-generated method stub
			return null;
		}

		@Override
		@QtUninvokable
		public QLayoutItem takeAt(int index) {
			// TODO Auto-generated method stub
			return null;
		}
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
        assertEquals(event.type(), CustomEvent.customType());
        assertTrue(event instanceof CustomEvent);

        CustomEvent customEvent = (CustomEvent) event;
        assertEquals(customEvent.something(), 10);
    }

    @Test
    public void testSendPaintEvent()
    {
        PolymorphicType.sendPaintEvent(widget);
        assertEquals(QEvent.Type.Paint, widget.eventType);
        assertEquals(QPaintEvent.class, widget.eventClass);
        assertEquals(0, General.internalAccess.nativeId(widget.m_event));
    }

    @Test
    public void testSendCustomEvent()
    {
        PolymorphicType.sendCustomEvent(widget, 20);
        assertEquals(CustomEvent.customType(), widget.eventType);
        assertEquals(CustomEvent.class, widget.eventClass);
        assertEquals(0, General.internalAccess.nativeId(widget.m_event));
        assertEquals(20, widget.customEventSomething);
    }

    @Test
    public void testGetButtonStyleOption()
    {
        QStyleOption opt = PolymorphicType.getButtonStyleOption();
        assertTrue(opt==null ? "" : General.internalAccess.getClass(opt).getName(), opt instanceof QStyleOptionButton);
    }

    @Test
    public void testGetCustomStyleOption()
    {
        QStyleOption opt = PolymorphicType.getCustomStyleOption(30);
        assertTrue(opt instanceof CustomStyleOption);

        CustomStyleOption customOpt = (CustomStyleOption) opt;
        assertEquals(customOpt.something(), 30);
    }

    @Test
    public void testGetUnmappedCustomStyleOption()
    {
        QStyleOption opt = PolymorphicType.getUnmappedCustomStyleOption();
        assertTrue(opt instanceof QStyleOption);
        assertEquals(General.internalAccess.getClass(opt).getName(), "io.qt.widgets.QStyleOption");
    }
    
    @Test
    public void testSliderStyleOption() {
    	QStyleOptionComplex[] option = {null};
    	QProxyStyle style = new QProxyStyle(){
			@Override
			public void drawComplexControl(ComplexControl control, QStyleOptionComplex _option, QPainter painter,
					QWidget widget) {
				option[0] = _option;
				super.drawComplexControl(control, _option, painter, widget);
				QApplication.quit();
			}
    	};
    	QSlider slider = new QSlider();
    	slider.setStyle(style);
    	slider.show();
    	QApplication.exec();
    	slider.hide();
    	assertTrue("Type unexpected: " + (option[0]==null ? "null" : General.internalAccess.getClass(option[0]).getName()), option[0] instanceof QStyleOptionSlider);
    }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestPolymorphicTypes.class.getName());
    }
}
