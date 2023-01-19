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

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.atomic.AtomicReference;

import org.junit.Test;

import io.qt.QNoNativeResourcesException;
import io.qt.QtPrimitiveType;
import io.qt.QtPropertyMember;
import io.qt.autotests.generated.BindableOwner;
import io.qt.core.*;
import io.qt.widgets.QGraphicsItem;
import io.qt.widgets.QGraphicsPathItem;
import io.qt.widgets.QGraphicsProxyWidget;
import io.qt.widgets.QGraphicsWidget;

public class TestQObjectPropertyQt6 extends ApplicationInitializer {
    @Test
    public void testQByteProperty() {
    	class PropertyOwner extends QObject{
    		public final QByteProperty p = new QByteProperty();
    		public final Signal0 pChanged = new Signal0();
    	}
    	PropertyOwner object = new PropertyOwner();
    	AtomicInteger signalCounter = new AtomicInteger();
    	object.pChanged.connect(signalCounter::incrementAndGet);
    	assertEquals(0, signalCounter.get());
    	assertTrue("unable to set property 'p'", object.setProperty("p", (byte)100));
    	assertEquals(1, signalCounter.get());
    	assertEquals(100, object.p.value());
    	object.p.setValue((byte)50);
    	assertEquals(2, signalCounter.get());
    	assertEquals(50, (byte)object.property("p"));
    	QByteBindable pBindable = (QByteBindable)object.metaObject().property("p").bindable(object);
    	QByteProperty freeProperty = new QByteProperty((byte)28);
    	pBindable.setBinding(()->(byte)(freeProperty.value()*2));
    	assertTrue("binding is null", !pBindable.binding().isNull());
    	assertTrue(signalCounter.get()==3 || signalCounter.get()==4);
    	assertEquals(28*2, (byte)object.p.value());
    }
    
    @Test
    public void testQProperty_Byte() {
    	class PropertyOwner extends QObject{
    		public final QProperty<@QtPrimitiveType Byte> p = new QProperty<>();
    		public final Signal0 pChanged = new Signal0();
    	}
    	PropertyOwner object = new PropertyOwner();
    	AtomicInteger signalCounter = new AtomicInteger();
    	object.pChanged.connect(signalCounter::incrementAndGet);
    	assertEquals(0, signalCounter.get());
    	assertTrue("unable to set property 'p'", object.setProperty("p", (byte)100));
    	assertEquals(1, signalCounter.get());
    	assertEquals(100, (byte)object.p.value());
    	object.p.setValue((byte)50);
    	assertEquals(2, signalCounter.get());
    	assertEquals(50, (byte)object.property("p"));
    	assertEquals(QMetaType.Type.SChar.value(), new QBindable<>(object.p).makeBinding().valueMetaType().id());
    	QByteBindable pBindable = (QByteBindable)object.metaObject().property("p").bindable(object);
    	QProperty<Byte> freeProperty = new QProperty<>((byte)28);
    	pBindable.setBinding(()->(byte)(freeProperty.value()*2));
    	assertTrue("binding is null", !pBindable.binding().isNull());
    	assertTrue(signalCounter.get()==3 || signalCounter.get()==4);
    	assertEquals(28*2, (byte)object.p.value());
    }
    
    @Test
    public void testQShortProperty() {
    	class PropertyOwner extends QObject{
    		public final QShortProperty p = new QShortProperty();
    		public final Signal0 pChanged = new Signal0();
    	}
    	PropertyOwner object = new PropertyOwner();
    	AtomicInteger signalCounter = new AtomicInteger();
    	object.pChanged.connect(signalCounter::incrementAndGet);
    	assertEquals(0, signalCounter.get());
    	assertTrue("unable to set property 'p'", object.setProperty("p", (short)100));
    	assertEquals(1, signalCounter.get());
    	assertEquals(100, object.p.value());
    	object.p.setValue((short)50);
    	assertEquals(2, signalCounter.get());
    	assertEquals(50, (short)object.property("p"));
    	QShortBindable pBindable = (QShortBindable)object.metaObject().property("p").bindable(object);
    	QShortProperty freeProperty = new QShortProperty((short)28);
    	pBindable.setBinding(()->(short)(freeProperty.value()*2));
    	assertTrue("binding is null", !pBindable.binding().isNull());
    	assertTrue(signalCounter.get()==3 || signalCounter.get()==4);
    	assertEquals(28*2, (int)object.p.value());
    }
    
    @Test
    public void testQProperty_Short() {
    	class PropertyOwner extends QObject{
    		public final QProperty<@QtPrimitiveType Short> p = new QProperty<>();
    		public final Signal0 pChanged = new Signal0();
    	}
    	PropertyOwner object = new PropertyOwner();
    	AtomicInteger signalCounter = new AtomicInteger();
    	object.pChanged.connect(signalCounter::incrementAndGet);
    	assertEquals(0, signalCounter.get());
    	assertTrue("unable to set property 'p'", object.setProperty("p", (short)100));
    	assertEquals(1, signalCounter.get());
    	assertEquals(100, (short)object.p.value());
    	object.p.setValue((short)50);
    	assertEquals(2, signalCounter.get());
    	assertEquals(50, (short)object.property("p"));
    	assertEquals(QMetaType.Type.Short.value(), new QBindable<>(object.p).makeBinding().valueMetaType().id());
    	QShortBindable pBindable = (QShortBindable)object.metaObject().property("p").bindable(object);
    	QProperty<Short> freeProperty = new QProperty<>((short)28);
    	pBindable.setBinding(()->(short)(freeProperty.value()*2));
    	assertTrue("binding is null", !pBindable.binding().isNull());
    	assertTrue(signalCounter.get()==3 || signalCounter.get()==4);
    	assertEquals(28*2, (short)object.p.value());
    }
    
    @Test
    public void testQIntProperty() {
    	class PropertyOwner extends QObject{
    		public final QIntProperty p = new QIntProperty();
    		public final Signal0 pChanged = new Signal0();
    	}
    	PropertyOwner object = new PropertyOwner();
    	AtomicInteger signalCounter = new AtomicInteger();
    	object.pChanged.connect(signalCounter::incrementAndGet);
    	assertEquals(0, signalCounter.get());
    	assertTrue("unable to set property 'p'", object.setProperty("p", 100));
    	assertEquals(1, signalCounter.get());
    	assertEquals(100, object.p.value());
    	object.p.setValue(50);
    	assertEquals(2, signalCounter.get());
    	assertEquals(50, (int)object.property("p"));
    	QIntBindable pBindable = (QIntBindable)object.metaObject().property("p").bindable(object);
    	QIntProperty freeProperty = new QIntProperty(28);
    	pBindable.setBinding(()->freeProperty.value()*2);
    	assertTrue("binding is null", !pBindable.binding().isNull());
    	assertTrue(signalCounter.get()==3 || signalCounter.get()==4);
    	assertEquals(28*2, (int)object.p.value());
    }
    
    @Test
    public void testQProperty_Int() {
    	class PropertyOwner extends QObject{
    		public final QProperty<@QtPrimitiveType Integer> p = new QProperty<>();
    		public final Signal0 pChanged = new Signal0();
    	}
    	PropertyOwner object = new PropertyOwner();
    	AtomicInteger signalCounter = new AtomicInteger();
    	object.pChanged.connect(signalCounter::incrementAndGet);
    	assertEquals(0, signalCounter.get());
    	assertTrue("unable to set property 'p'", object.setProperty("p", 100));
    	assertEquals(1, signalCounter.get());
    	assertEquals(100, (int)object.p.value());
    	object.p.setValue(50);
    	assertEquals(2, signalCounter.get());
    	assertEquals(50, (int)object.property("p"));
    	assertEquals(QMetaType.Type.Int.value(), new QBindable<>(object.p).makeBinding().valueMetaType().id());
    	QIntBindable pBindable = (QIntBindable)object.metaObject().property("p").bindable(object);
    	QProperty<Integer> freeProperty = new QProperty<>(28);
    	pBindable.setBinding(()->freeProperty.value()*2);
    	assertTrue("binding is null", !pBindable.binding().isNull());
    	assertTrue(signalCounter.get()==3 || signalCounter.get()==4);
    	assertEquals(28*2, (int)object.p.value());
    }
    
    @Test
    public void testQLongProperty() {
    	class PropertyOwner extends QObject{
    		public final QLongProperty p = new QLongProperty();
    		public final Signal0 pChanged = new Signal0();
    	}
    	PropertyOwner object = new PropertyOwner();
    	AtomicInteger signalCounter = new AtomicInteger();
    	object.pChanged.connect(signalCounter::incrementAndGet);
    	assertEquals(0, signalCounter.get());
    	assertTrue("unable to set property 'p'", object.setProperty("p", 100L));
    	assertEquals(1, signalCounter.get());
    	assertEquals(100, object.p.value());
    	object.p.setValue(50);
    	assertEquals(2, signalCounter.get());
    	assertEquals(50, (long)object.property("p"));
    	QLongBindable pBindable = (QLongBindable)object.metaObject().property("p").bindable(object);
    	QLongProperty freeProperty = new QLongProperty(28);
    	pBindable.setBinding(()->freeProperty.value()*2);
    	assertTrue("binding is null", !pBindable.binding().isNull());
    	assertTrue(signalCounter.get()==3 || signalCounter.get()==4);
    	assertEquals(28*2, (long)object.p.value());
    }
    
    @Test
    public void testQProperty_Long() {
    	class PropertyOwner extends QObject{
    		public final QProperty<@QtPrimitiveType Long> p = new QProperty<>();
    		public final Signal0 pChanged = new Signal0();
    	}
    	PropertyOwner object = new PropertyOwner();
    	AtomicInteger signalCounter = new AtomicInteger();
    	object.pChanged.connect(signalCounter::incrementAndGet);
    	assertEquals(0, signalCounter.get());
    	assertTrue("unable to set property 'p'", object.setProperty("p", 100L));
    	assertEquals(1, signalCounter.get());
    	assertEquals(100, (long)object.p.value());
    	object.p.setValue(50l);
    	assertEquals(2, signalCounter.get());
    	assertEquals(50, (long)object.property("p"));
    	assertEquals(QMetaType.Type.LongLong.value(), new QBindable<>(object.p).makeBinding().valueMetaType().id());
    	QLongBindable pBindable = (QLongBindable)object.metaObject().property("p").bindable(object);
    	QProperty<Long> freeProperty = new QProperty<>(28l);
    	pBindable.setBinding(()->freeProperty.value()*2);
    	assertTrue("binding is null", !pBindable.binding().isNull());
    	assertTrue(signalCounter.get()==3 || signalCounter.get()==4);
    	assertEquals(28*2, (long)object.p.value());
    }
    
    @Test
    public void testQFloatProperty() {
    	class PropertyOwner extends QObject{
    		public final QFloatProperty p = new QFloatProperty();
    		public final Signal0 pChanged = new Signal0();
    	}
    	PropertyOwner object = new PropertyOwner();
    	AtomicInteger signalCounter = new AtomicInteger();
    	object.pChanged.connect(signalCounter::incrementAndGet);
    	assertEquals(0, signalCounter.get());
    	assertTrue("unable to set property 'p'", object.setProperty("p", 100.f));
    	assertEquals(1, signalCounter.get());
    	assertEquals(100, object.p.value(), 0.01);
    	object.p.setValue(50);
    	assertEquals(2, signalCounter.get());
    	assertEquals(50, (float)object.property("p"), 0.01);
    	QFloatBindable pBindable = (QFloatBindable)object.metaObject().property("p").bindable(object);
    	QFloatProperty freeProperty = new QFloatProperty(28);
    	pBindable.setBinding(()->freeProperty.value()*2);
    	assertTrue("binding is null", !pBindable.binding().isNull());
    	assertTrue(signalCounter.get()==3 || signalCounter.get()==4);
    	assertEquals(28*2, (float)object.p.value(), 0.01);
    }
    
    @Test
    public void testQProperty_Float() {
    	class PropertyOwner extends QObject{
    		public final QProperty<@QtPrimitiveType Float> p = new QProperty<>();
    		public final Signal0 pChanged = new Signal0();
    	}
    	PropertyOwner object = new PropertyOwner();
    	AtomicInteger signalCounter = new AtomicInteger();
    	object.pChanged.connect(signalCounter::incrementAndGet);
    	assertEquals(0, signalCounter.get());
    	assertTrue("unable to set property 'p'", object.setProperty("p", 100.f));
    	assertEquals(1, signalCounter.get());
    	assertEquals(100, (float)object.p.value(), 0.01);
    	object.p.setValue(50f);
    	assertEquals(2, signalCounter.get());
    	assertEquals(50, (float)object.property("p"), 0.01);
    	assertEquals(QMetaType.Type.Float.value(), new QBindable<>(object.p).makeBinding().valueMetaType().id());
    	QFloatBindable pBindable = (QFloatBindable)object.metaObject().property("p").bindable(object);
    	QProperty<Float> freeProperty = new QProperty<>(28f);
    	pBindable.setBinding(()->freeProperty.value()*2);
    	assertTrue("binding is null", !pBindable.binding().isNull());
    	assertTrue(signalCounter.get()==3 || signalCounter.get()==4);
    	assertEquals(28*2, (float)object.p.value(), 0.01);
    }
    
    @Test
    public void testQDoubleProperty() {
    	class PropertyOwner extends QObject{
    		public final QDoubleProperty p = new QDoubleProperty();
    		public final Signal0 pChanged = new Signal0();
    	}
    	PropertyOwner object = new PropertyOwner();
    	AtomicInteger signalCounter = new AtomicInteger();
    	object.pChanged.connect(signalCounter::incrementAndGet);
    	assertEquals(0, signalCounter.get());
    	assertTrue("unable to set property 'p'", object.setProperty("p", 100.));
    	assertEquals(1, signalCounter.get());
    	assertEquals(100, object.p.value(), 0.01);
    	object.p.setValue(50);
    	assertEquals(2, signalCounter.get());
    	assertEquals(50, (double)object.property("p"), 0.01);
    	QDoubleBindable pBindable = (QDoubleBindable)object.metaObject().property("p").bindable(object);
    	QDoubleProperty freeProperty = new QDoubleProperty(28);
    	pBindable.setBinding(()->freeProperty.value()*2);
    	assertTrue("binding is null", !pBindable.binding().isNull());
    	assertTrue(signalCounter.get()==3 || signalCounter.get()==4);
    	assertEquals(28*2, (double)object.p.value(), 0.01);
    }
    
    @Test
    public void testQProperty_Double() {
    	class PropertyOwner extends QObject{
    		public final QProperty<@QtPrimitiveType Double> p = new QProperty<>();
    		public final Signal0 pChanged = new Signal0();
    	}
    	PropertyOwner object = new PropertyOwner();
    	AtomicInteger signalCounter = new AtomicInteger();
    	object.pChanged.connect(signalCounter::incrementAndGet);
    	assertEquals(0, signalCounter.get());
    	assertTrue("unable to set property 'p'", object.setProperty("p", 100.));
    	assertEquals(1, signalCounter.get());
    	assertEquals(100, (double)object.p.value(), 0.01);
    	object.p.setValue(50.);
    	assertEquals(2, signalCounter.get());
    	assertEquals(50, (double)object.property("p"), 0.01);
    	assertEquals(QMetaType.Type.Double.value(), new QBindable<>(object.p).makeBinding().valueMetaType().id());
    	QDoubleBindable pBindable = (QDoubleBindable)object.metaObject().property("p").bindable(object);
    	QProperty<Double> freeProperty = new QProperty<>(28.);
    	pBindable.setBinding(()->freeProperty.value()*2);
    	assertTrue("binding is null", !pBindable.binding().isNull());
    	assertTrue(signalCounter.get()==3 || signalCounter.get()==4);
    	assertEquals(28*2, (double)object.p.value(), 0.01);
    }
    
    @Test
    public void testQCharProperty() {
    	class PropertyOwner extends QObject{
    		public final QCharProperty p = new QCharProperty();
    		public final Signal0 pChanged = new Signal0();
    	}
    	PropertyOwner object = new PropertyOwner();
    	AtomicInteger signalCounter = new AtomicInteger();
    	object.pChanged.connect(signalCounter::incrementAndGet);
    	assertEquals(0, signalCounter.get());
    	assertTrue("unable to set property 'p'", object.setProperty("p", (char)100));
    	assertEquals(1, signalCounter.get());
    	assertEquals(100, object.p.value());
    	object.p.setValue((char)50);
    	assertEquals(2, signalCounter.get());
    	assertEquals(50, (char)object.property("p"));
    	QCharBindable pBindable = (QCharBindable)object.metaObject().property("p").bindable(object);
    	QCharProperty freeProperty = new QCharProperty((char)28);
    	pBindable.setBinding(()->(char)(freeProperty.value()*2));
    	assertTrue("binding is null", !pBindable.binding().isNull());
    	assertTrue(signalCounter.get()==3 || signalCounter.get()==4);
    	assertEquals(28*2, (int)object.p.value());
    }
    
    @Test
    public void testQProperty_Char() {
    	class PropertyOwner extends QObject{
    		public final QProperty<@QtPrimitiveType Character> p = new QProperty<>();
    		public final Signal0 pChanged = new Signal0();
    	}
    	PropertyOwner object = new PropertyOwner();
    	AtomicInteger signalCounter = new AtomicInteger();
    	object.pChanged.connect(signalCounter::incrementAndGet);
    	assertEquals(0, signalCounter.get());
    	assertTrue("unable to set property 'p'", object.setProperty("p", (char)100));
    	assertEquals(1, signalCounter.get());
    	assertEquals(100, (char)object.p.value());
    	object.p.setValue((char)50);
    	assertEquals(2, signalCounter.get());
    	assertEquals(50, (char)object.property("p"));
    	assertEquals(QMetaType.Type.QChar.value(), new QBindable<>(object.p).makeBinding().valueMetaType().id());
    	QCharBindable pBindable = (QCharBindable)object.metaObject().property("p").bindable(object);
    	QProperty<Character> freeProperty = new QProperty<>((char)28);
    	pBindable.setBinding(()->(char)(freeProperty.value()*2));
    	assertTrue("binding is null", !pBindable.binding().isNull());
    	assertTrue(signalCounter.get()==3 || signalCounter.get()==4);
    	assertEquals(28*2, (char)object.p.value());
    }
    
    @Test
    public void testQBooleanProperty() {
    	class PropertyOwner extends QObject{
    		public final QBooleanProperty p = new QBooleanProperty();
    		public final Signal0 pChanged = new Signal0();
    	}
    	PropertyOwner object = new PropertyOwner();
    	AtomicInteger signalCounter = new AtomicInteger();
    	object.pChanged.connect(signalCounter::incrementAndGet);
    	assertEquals(0, signalCounter.get());
    	assertTrue("unable to set property 'p'", object.setProperty("p", true));
    	assertEquals(1, signalCounter.get());
    	assertEquals(true, object.p.value());
    	object.p.setValue(false);
    	assertEquals(2, signalCounter.get());
    	assertEquals(false, (boolean)object.property("p"));
    	QBooleanBindable pBindable = (QBooleanBindable)object.metaObject().property("p").bindable(object);
    	QBooleanProperty freeProperty = new QBooleanProperty(true);
    	pBindable.setBinding(()->freeProperty.value());
    	assertTrue("binding is null", !pBindable.binding().isNull());
    	assertTrue(signalCounter.get()==3 || signalCounter.get()==4);
    	assertEquals(true, (boolean)object.p.value());
    }
    
    @Test
    public void testQProperty_Boolean() {
    	class PropertyOwner extends QObject{
    		public final QProperty<@QtPrimitiveType Boolean> p = new QProperty<>();
    		public final Signal0 pChanged = new Signal0();
    	}
    	PropertyOwner object = new PropertyOwner();
    	AtomicInteger signalCounter = new AtomicInteger();
    	object.pChanged.connect(signalCounter::incrementAndGet);
    	assertEquals(0, signalCounter.get());
    	assertTrue("unable to set property 'p'", object.setProperty("p", true));
    	assertEquals(1, signalCounter.get());
    	assertEquals(true, object.p.value());
    	object.p.setValue(false);
    	assertEquals(2, signalCounter.get());
    	assertEquals(false, (boolean)object.property("p"));
    	QBooleanBindable pBindable = (QBooleanBindable)object.metaObject().property("p").bindable(object);
    	QProperty<Boolean> freeProperty = new QProperty<>(true);
    	pBindable.setBinding(()->freeProperty.value());
    	assertTrue("binding is null", !pBindable.binding().isNull());
    	assertTrue(signalCounter.get()==3 || signalCounter.get()==4);
    	assertEquals(true, (boolean)object.p.value());
    }
    
    @Test
    public void test_Native2Java_Bindable_in_Java() {
    	class PropertyOwner extends QObject{
    		public final QIntProperty x = new QIntProperty();
    		public final QProperty<String> text = new QProperty<>();
    	}
    	PropertyOwner source = new PropertyOwner();
    	source.x.setValue(5);
    	assertEquals(5, source.x.value());
    	source.text.setValue("TEST");
    	assertEquals("TEST", source.text.value());
    	BindableOwner target = new BindableOwner();
    	assertTrue(target.metaObject().property("x").bindable(target).setBinding(source.metaObject().property("x").bindable(source).makeBinding()));
    	assertEquals(5, target.x());
    	assertEquals("TEST", source.property("text"));
    	assertTrue(target.metaObject().property("text").bindable(target).setBinding(source.metaObject().property("text").bindable(source).makeBinding()));
    	assertEquals("TEST", target.text());
    }
    
    @Test
    public void test_Java2Native_Bindable_in_CPP() {
    	class PropertyOwner extends QObject{
    		public final QIntProperty x = new QIntProperty();
    		public final QProperty<String> text = new QProperty<>();
    	}
    	BindableOwner source = new BindableOwner();
    	source.setX(5);
    	source.setText("TEST");
    	PropertyOwner target = new PropertyOwner();
    	BindableOwner.bind(target, "x", source, "x");
    	assertEquals(5, target.x.value());
    	assertEquals("TEST", source.property("text"));
    	BindableOwner.bind(target, "text", source, "text");
    	assertEquals("TEST", target.text.value());
    }
    
    @Test
    public void test_Native2Java_Bindable_in_CPP() {
    	class PropertyOwner extends QObject{
    		public final QIntProperty x = new QIntProperty();
    		public final QProperty<String> text = new QProperty<>();
    	}
    	PropertyOwner source = new PropertyOwner();
    	source.x.setValue(5);
    	assertEquals(5, source.x.value());
    	source.text.setValue("TEST");
    	assertEquals("TEST", source.text.value());
    	BindableOwner target = new BindableOwner();
    	BindableOwner.bind(target, "x", source, "x");
    	assertEquals(5, target.x());
    	assertEquals("TEST", source.property("text"));
    	BindableOwner.bind(target, "text", source, "text");
    	assertEquals("TEST", target.text());
    }
    
    @Test
    public void test_Java2Native_Bindable_in_Java() {
    	class PropertyOwner extends QObject{
    		public final QIntProperty x = new QIntProperty();
    		public final QProperty<String> text = new QProperty<>();
    	}
    	BindableOwner source = new BindableOwner();
    	source.setX(5);
    	source.setText("TEST");
    	PropertyOwner target = new PropertyOwner();
    	assertTrue(target.metaObject().property("x").bindable(target).setBinding(source.metaObject().property("x").bindable(source).makeBinding()));
    	assertEquals(5, target.x.value());
    	assertEquals("TEST", source.property("text"));
    	assertTrue(target.metaObject().property("text").bindable(target).setBinding(source.metaObject().property("text").bindable(source).makeBinding()));
    	assertEquals("TEST", target.text.value());
    }
    
    @Test
    public void testQProperty_Pointer() {
    	class PropertyOwner extends QObject{
    		@QtPropertyMember
    		private final QProperty<QGraphicsItem> p = new QProperty<>();
    		public final Signal0 pChanged = new Signal0();
    		public final QProperty<QGraphicsItem> freeProperty = new QProperty<>(new QGraphicsProxyWidget());
    		public final QBooleanProperty decider = new QBooleanProperty(false);
    	}
    	PropertyOwner object = new PropertyOwner();
    	AtomicInteger signalCounter = new AtomicInteger();
    	object.pChanged.connect(signalCounter::incrementAndGet);
    	assertEquals(0, signalCounter.get());
    	QGraphicsWidget w1 = new QGraphicsWidget();
    	assertTrue("unable to set property 'p'", object.setProperty("p", w1));
    	assertEquals(1, signalCounter.get());
    	assertEquals(w1, object.p.value());
    	object.p.setValue(new QGraphicsPathItem());
    	assertEquals(2, signalCounter.get());
    	ApplicationInitializer.runGC();
    	assertTrue(object.property("p") instanceof QGraphicsPathItem);
    	ApplicationInitializer.runGC();
    	assertTrue(object.property("p") instanceof QGraphicsPathItem);
    	ApplicationInitializer.runGC();
    	assertTrue(object.property("p") instanceof QGraphicsPathItem);
    	ApplicationInitializer.runGC();
    	assertTrue(object.p.value() instanceof QGraphicsPathItem);
    	ApplicationInitializer.runGC();
    	assertTrue(object.p.value() instanceof QGraphicsPathItem);
    	ApplicationInitializer.runGC();
    	assertTrue(object.p.value() instanceof QGraphicsPathItem);
    	@SuppressWarnings("unchecked")
		QBindable<QGraphicsItem> pBindable = (QBindable<QGraphicsItem>)object.metaObject().property("p").bindable(object);
    	assertTrue(object.freeProperty.value() instanceof QGraphicsProxyWidget);
    	pBindable.setBinding(()->{
    		return object.decider.value() ? object.freeProperty.value() : null;
    	});
    	assertTrue("binding is null", !pBindable.binding().isNull());
    	assertTrue(signalCounter.get()==3 || signalCounter.get()==4);
    	assertEquals(null, object.p.value());
    	object.setProperty("decider", true);
    	assertTrue(object.p.value() instanceof QGraphicsProxyWidget);
    }
    
    @Test
    public void testDeletion() {
    	class PropertyOwner extends QObject{
    		public final QIntProperty x = new QIntProperty(5);
    	}
    	PropertyOwner p1 = new PropertyOwner();
    	PropertyOwner p2 = new PropertyOwner();
    	PropertyOwner p3 = new PropertyOwner();
    	assertEquals(5, p1.x.value());
    	assertEquals(5, p2.x.value());
    	assertEquals(5, p3.x.value());
    	p1.x.setBinding(()->{
    		return p2.x.value()*2;
    	});
    	assertEquals(10, p1.x.value());
    	p2.x.setBinding(()->{
    		return p3.x.value()*2;
    	});
    	assertEquals(20, p1.x.value());
    	assertEquals(10, p2.x.value());
    	p3.x.setValue(1);
    	assertEquals(4, p1.x.value());
    	assertEquals(2, p2.x.value());
    	p2.dispose();
    	p3.x.setValue(2);
    	try {
    		p2.x.value();
    		assertTrue("QNoNativeResourcesException expected", false);
    	}catch(QNoNativeResourcesException e) {}
    	assertEquals(4, p1.x.value());
    }
    
    @Test
    public void testFindNotifySignal() {
    	class PropertyOwner extends QObject{
    		@QtPropertyMember
    		private final QIntProperty x = new QIntProperty();
    		public final Signal1<Integer> xChanged = new Signal1<>();
    	}
    	PropertyOwner owner = new PropertyOwner();
    	int[] received = {0};
    	owner.xChanged.connect(i->received[0] = i);
    	owner.x.setValue(5);
    	assertEquals(5, received[0]);
    	owner.setProperty("x", 8);
    	assertEquals(8, received[0]);
    }
    
    @Test
    public void testComputedProperty() {
    	AtomicInteger iValue = new AtomicInteger(3);
    	AtomicReference<String> sValue = new AtomicReference<>("");
    	class PropertyOwner extends QObject{
    		@QtPropertyMember
    		private final QComputedIntProperty iComputed = new QComputedIntProperty(this::computeInt);
    		@QtPropertyMember
    		private final QIntProperty iProp = new QIntProperty(()->iComputed.value());
    		public final Signal1<Integer> iChanged = new Signal1<>();
    		private int computeInt() {
    			return iValue.get();
    		}
    		
    		@QtPropertyMember
    		private final QComputedProperty<String> sComputed = new QComputedProperty<>(this::computeString);
    		@QtPropertyMember
    		private final QProperty<String> sProp = new QProperty<String>(()->sComputed.value());
    		public final Signal1<String> sChanged = new Signal1<>();
    		private String computeString() {
    			return sValue.get();
    		}
    		public void notifyIComputed(Qt.ConnectionType type) {
    			QComputedIntProperty iComputed = this.iComputed;
    			QMetaObject.invokeMethod(iComputed::notifyProperty, type);
    		}
    	}
    	PropertyOwner owner = new PropertyOwner();
    	int[] iReceived = {owner.iProp.value()};
    	owner.iChanged.connect(i->iReceived[0] = i);
    	iValue.set(5);
    	owner.notifyIComputed(Qt.ConnectionType.AutoConnection);
    	assertEquals(iValue.get(), iReceived[0]);
    	
    	String[] sReceived = {owner.sProp.value()};
    	owner.sChanged.connect(s->sReceived[0] = s);
    	sValue.set("TEST");
    	owner.sComputed.notifyProperty();
    	assertEquals(sValue.get(), sReceived[0]);
    }
}
