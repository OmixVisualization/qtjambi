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
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.atomic.AtomicInteger;

import org.junit.Assume;
import org.junit.Test;

import io.qt.QtUtilities;
import io.qt.core.QBindable;
import io.qt.core.QBooleanBindable;
import io.qt.core.QBooleanProperty;
import io.qt.core.QBooleanPropertyAlias;
import io.qt.core.QByteBindable;
import io.qt.core.QByteProperty;
import io.qt.core.QBytePropertyAlias;
import io.qt.core.QCharBindable;
import io.qt.core.QCharProperty;
import io.qt.core.QCharPropertyAlias;
import io.qt.core.QDoubleBindable;
import io.qt.core.QDoubleProperty;
import io.qt.core.QDoublePropertyAlias;
import io.qt.core.QFloatBindable;
import io.qt.core.QFloatProperty;
import io.qt.core.QFloatPropertyAlias;
import io.qt.core.QIntBindable;
import io.qt.core.QIntProperty;
import io.qt.core.QIntPropertyAlias;
import io.qt.core.QLibraryInfo;
import io.qt.core.QLongBindable;
import io.qt.core.QLongProperty;
import io.qt.core.QLongPropertyAlias;
import io.qt.core.QMetaType;
import io.qt.core.QProperty;
import io.qt.core.QPropertyAlias;
import io.qt.core.QPropertyChangeHandler;
import io.qt.core.QShortBindable;
import io.qt.core.QShortProperty;
import io.qt.core.QShortPropertyAlias;
import io.qt.core.QThread;
import io.qt.core.QVersionNumber;
import io.qt.widgets.QGraphicsItem;
import io.qt.widgets.QGraphicsPathItem;
import io.qt.widgets.QGraphicsProxyWidget;
import io.qt.widgets.QGraphicsWidget;

public class TestQPropertyQt6 extends ApplicationInitializer {
	
	private boolean canAlias = QLibraryInfo.version().compareTo(new QVersionNumber(7,0,0))<0;
	
    @Test
    public void testQByteProperty() {
    	QByteProperty p1 = new QByteProperty((byte)5);
    	QByteProperty p2 = new QByteProperty((byte)5);
    	QByteProperty boundProperty = new QByteProperty(()->(byte)(p1.value()*p2.value()));
    	AtomicInteger changeCounter = new AtomicInteger();
    	@SuppressWarnings("unused")
		QPropertyChangeHandler handler = boundProperty.onValueChanged(()->changeCounter.incrementAndGet());
    	ApplicationInitializer.runGC();
    	assertEquals(5*5, boundProperty.value());
    	p1.setValue((byte)2);
    	assertEquals(1, changeCounter.intValue());
    	assertEquals(2*5, boundProperty.value());
    	p2.setValue((byte)3);
    	assertEquals(2, changeCounter.intValue());
    	handler.dispose();
    	handler = null;
    	ApplicationInitializer.runGC();
    	assertFalse(boundProperty.binding().isNull());
    	assertEquals(2*3, boundProperty.value());
    	boundProperty.setValue((byte)100);
    	assertEquals(2, changeCounter.intValue());
    	assertTrue(boundProperty.binding().isNull());
    	assertEquals(100, boundProperty.value());
    	p2.setValue((byte)4);
    	assertEquals(100, boundProperty.value());
    	
    	QByteBindable b1 = new QByteBindable(p1);
    	assertEquals(2, b1.value());
    	QByteBindable b2 = new QByteBindable(p2);
    	assertEquals(4, b2.value());
    	QByteBindable bindable = new QByteBindable(boundProperty);
    	assertTrue(bindable.binding().isNull());
    	assertFalse(bindable.makeBinding().isNull());
    	assertEquals(100, bindable.value());
    	bindable.setBinding(b1.makeBinding());
    	assertFalse(bindable.binding().isNull());
    	assertEquals(2, bindable.value());
    	b1.setValue((byte)90);
    	assertEquals(90, bindable.value());
    	bindable.setValue((byte)20);
    	assertTrue(boundProperty.binding().isNull());
    	assertEquals(20, boundProperty.value());
    	b1.setValue((byte)19);
    	assertEquals(20, boundProperty.value());
    	boundProperty.setBinding(()->(byte)(b1.value()+b2.value()));
    	assertEquals(4+19, boundProperty.value());
    	b2.setValue((byte)5);
    	assertEquals(5+19, boundProperty.value());
    	b1.setBinding(()->6);
    	assertEquals(5+6, boundProperty.value());
    	boundProperty.takeBinding();
    	b1.setValue((byte)0);
    	assertEquals(5+6, boundProperty.value());
    	if(canAlias) {
	    	QBytePropertyAlias b2Alias = new QBytePropertyAlias(b2);
	    	assertEquals(5, b2Alias.value());
	    	b2Alias.setValue((byte)8);
	    	assertEquals(8, b2.value());
	    	QByteProperty p3 = new QByteProperty((byte)43);
	    	b2Alias.setBinding(()->(byte)(p3.value()-30));
	    	assertEquals(13, b2Alias.value());
	    	assertEquals(13, b2.value());
	    	assertEquals(13, p2.value());
    	}
    }
    
    @Test
    public void testQIntProperty() {
    	QIntProperty p1 = new QIntProperty(5);
    	QIntProperty p2 = new QIntProperty(5);
    	QIntProperty boundProperty = new QIntProperty(()->p1.value()*p2.value());
    	AtomicInteger changeCounter = new AtomicInteger();
    	@SuppressWarnings("unused")
		QPropertyChangeHandler handler = boundProperty.onValueChanged(()->changeCounter.incrementAndGet());
    	ApplicationInitializer.runGC();
    	assertEquals(5*5, boundProperty.value());
    	p1.setValue(2);
    	assertEquals(1, changeCounter.intValue());
    	assertEquals(2*5, boundProperty.value());
    	p2.setValue(3);
    	assertEquals(2, changeCounter.intValue());
    	handler.dispose();
    	handler = null;
    	ApplicationInitializer.runGC();
    	assertFalse(boundProperty.binding().isNull());
    	assertEquals(2*3, boundProperty.value());
    	boundProperty.setValue(1000);
    	assertEquals(2, changeCounter.intValue());
    	assertTrue(boundProperty.binding().isNull());
    	assertEquals(1000, boundProperty.value());
    	p2.setValue(4);
    	assertEquals(1000, boundProperty.value());
    	
    	QIntBindable b1 = new QIntBindable(p1);
    	assertEquals(2, b1.value());
    	QIntBindable b2 = new QIntBindable(p2);
    	assertEquals(4, b2.value());
    	QIntBindable bindable = new QIntBindable(boundProperty);
    	assertTrue(bindable.binding().isNull());
    	assertFalse(bindable.makeBinding().isNull());
    	assertEquals(1000, bindable.value());
    	bindable.setBinding(b1.makeBinding());
    	assertFalse(bindable.binding().isNull());
    	assertEquals(2, bindable.value());
    	b1.setValue(90);
    	assertEquals(90, bindable.value());
    	bindable.setValue(2000);
    	assertTrue(boundProperty.binding().isNull());
    	assertEquals(2000, boundProperty.value());
    	b1.setValue(190);
    	assertEquals(2000, boundProperty.value());
    	boundProperty.setBinding(()->b1.value()+b2.value());
    	assertEquals(4+190, boundProperty.value());
    	b2.setValue(5);
    	assertEquals(5+190, boundProperty.value());
    	b1.setBinding(()->6);
    	assertEquals(5+6, boundProperty.value());
    	boundProperty.takeBinding();
    	b1.setValue(0);
    	assertEquals(5+6, boundProperty.value());
    	if(canAlias) {
	    	QIntPropertyAlias b2Alias = new QIntPropertyAlias(b2);
	    	assertEquals(5, b2Alias.value());
	    	b2Alias.setValue(8);
	    	assertEquals(8, b2.value());
	    	QIntProperty p3 = new QIntProperty(143);
	    	b2Alias.setBinding(()->p3.value()-30);
	    	assertEquals(113, b2Alias.value());
	    	assertEquals(113, b2.value());
	    	assertEquals(113, p2.value());
    	}
    }
    
    @Test
    public void testQShortProperty() {
    	QShortProperty p1 = new QShortProperty((short)5);
    	QShortProperty p2 = new QShortProperty((short)5);
    	QShortProperty boundProperty = new QShortProperty(()->(short)(p1.value()*p2.value()));
    	AtomicInteger changeCounter = new AtomicInteger();
    	@SuppressWarnings("unused")
		QPropertyChangeHandler handler = boundProperty.onValueChanged(()->changeCounter.incrementAndGet());
    	ApplicationInitializer.runGC();
    	assertEquals(5*5, boundProperty.value());
    	p1.setValue((short)2);
    	assertEquals(1, changeCounter.intValue());
    	assertEquals(2*5, boundProperty.value());
    	p2.setValue((short)3);
    	assertEquals(2, changeCounter.intValue());
    	handler.dispose();
    	handler = null;
    	ApplicationInitializer.runGC();
    	assertFalse(boundProperty.binding().isNull());
    	assertEquals(2*3, boundProperty.value());
    	boundProperty.setValue((short)1000);
    	assertEquals(2, changeCounter.intValue());
    	assertTrue(boundProperty.binding().isNull());
    	assertEquals(1000, boundProperty.value());
    	p2.setValue((short)4);
    	assertEquals(1000, boundProperty.value());
    	
    	QShortBindable b1 = new QShortBindable(p1);
    	assertEquals(2, b1.value());
    	QShortBindable b2 = new QShortBindable(p2);
    	assertEquals(4, b2.value());
    	QShortBindable bindable = new QShortBindable(boundProperty);
    	assertTrue(bindable.binding().isNull());
    	assertFalse(bindable.makeBinding().isNull());
    	assertEquals(1000, bindable.value());
    	bindable.setBinding(b1.makeBinding());
    	assertFalse(bindable.binding().isNull());
    	assertEquals(2, bindable.value());
    	b1.setValue((short)90);
    	assertEquals(90, bindable.value());
    	bindable.setValue((short)2000);
    	assertTrue(boundProperty.binding().isNull());
    	assertEquals(2000, boundProperty.value());
    	b1.setValue((short)190);
    	assertEquals(2000, boundProperty.value());
    	boundProperty.setBinding(()->(short)(b1.value()+b2.value()));
    	assertEquals(4+190, boundProperty.value());
    	b2.setValue((short)5);
    	assertEquals(5+190, boundProperty.value());
    	b1.setBinding(()->6);
    	assertEquals(5+6, boundProperty.value());
    	boundProperty.takeBinding();
    	b1.setValue((short)0);
    	assertEquals(5+6, boundProperty.value());
    	if(canAlias) {
	    	QShortPropertyAlias b2Alias = new QShortPropertyAlias(b2);
	    	assertEquals(5, b2Alias.value());
	    	b2Alias.setValue((short)8);
	    	assertEquals(8, b2.value());
	    	QShortProperty p3 = new QShortProperty((short)143);
	    	b2Alias.setBinding(()->(short)(p3.value()-30));
	    	assertEquals(113, b2Alias.value());
	    	assertEquals(113, b2.value());
	    	assertEquals(113, p2.value());
    	}
    }
    
    @Test
    public void testQLongProperty() {
    	QLongProperty p1 = new QLongProperty(5);
    	QLongProperty p2 = new QLongProperty(5);
    	QLongProperty boundProperty = new QLongProperty(()->p1.value()*p2.value());
    	AtomicInteger changeCounter = new AtomicInteger();
    	@SuppressWarnings("unused")
		QPropertyChangeHandler handler = boundProperty.onValueChanged(()->changeCounter.incrementAndGet());
    	ApplicationInitializer.runGC();
    	assertEquals(5*5, boundProperty.value());
    	p1.setValue(2);
    	assertEquals(1, changeCounter.intValue());
    	assertEquals(2*5, boundProperty.value());
    	p2.setValue(3);
    	assertEquals(2, changeCounter.intValue());
    	handler.dispose();
    	handler = null;
    	ApplicationInitializer.runGC();
    	assertFalse(boundProperty.binding().isNull());
    	assertEquals(2*3, boundProperty.value());
    	boundProperty.setValue(1000);
    	assertEquals(2, changeCounter.intValue());
    	assertTrue(boundProperty.binding().isNull());
    	assertEquals(1000, boundProperty.value());
    	p2.setValue(4);
    	assertEquals(1000, boundProperty.value());
    	
    	QLongBindable b1 = new QLongBindable(p1);
    	assertEquals(2, b1.value());
    	QLongBindable b2 = new QLongBindable(p2);
    	assertEquals(4, b2.value());
    	QLongBindable bindable = new QLongBindable(boundProperty);
    	assertTrue(bindable.binding().isNull());
    	assertFalse(bindable.makeBinding().isNull());
    	assertEquals(1000, bindable.value());
    	bindable.setBinding(b1.makeBinding());
    	assertFalse(bindable.binding().isNull());
    	assertEquals(2, bindable.value());
    	b1.setValue(90);
    	assertEquals(90, bindable.value());
    	bindable.setValue(2000);
    	assertTrue(boundProperty.binding().isNull());
    	assertEquals(2000, boundProperty.value());
    	b1.setValue(190);
    	assertEquals(2000, boundProperty.value());
    	boundProperty.setBinding(()->b1.value()+b2.value());
    	assertEquals(4+190, boundProperty.value());
    	b2.setValue(5);
    	assertEquals(5+190, boundProperty.value());
    	b1.setBinding(()->6);
    	assertEquals(5+6, boundProperty.value());
    	boundProperty.takeBinding();
    	b1.setValue(0);
    	assertEquals(5+6, boundProperty.value());
    	if(canAlias) {
	    	QLongPropertyAlias b2Alias = new QLongPropertyAlias(b2);
	    	assertEquals(5, b2Alias.value());
	    	b2Alias.setValue(8);
	    	assertEquals(8, b2.value());
	    	QLongProperty p3 = new QLongProperty(143);
	    	b2Alias.setBinding(()->p3.value()-30);
	    	assertEquals(113, b2Alias.value());
	    	assertEquals(113, b2.value());
	    	assertEquals(113, p2.value());
    	}
    }
    
    @Test
    public void testQCharProperty() {
    	QCharProperty p1 = new QCharProperty((char)5);
    	QCharProperty p2 = new QCharProperty((char)5);
    	QCharProperty boundProperty = new QCharProperty(()->(char)(p1.value()*p2.value()));
    	AtomicInteger changeCounter = new AtomicInteger();
    	@SuppressWarnings("unused")
		QPropertyChangeHandler handler = boundProperty.onValueChanged(()->changeCounter.incrementAndGet());
    	ApplicationInitializer.runGC();
    	assertEquals(5*5, boundProperty.value());
    	p1.setValue((char)2);
    	assertEquals(1, changeCounter.intValue());
    	assertEquals(2*5, boundProperty.value());
    	p2.setValue((char)3);
    	assertEquals(2, changeCounter.intValue());
    	handler.dispose();
    	handler = null;
    	ApplicationInitializer.runGC();
    	assertFalse(boundProperty.binding().isNull());
    	assertEquals(2*3, boundProperty.value());
    	boundProperty.setValue((char)1000);
    	assertEquals(2, changeCounter.intValue());
    	assertTrue(boundProperty.binding().isNull());
    	assertEquals(1000, boundProperty.value());
    	p2.setValue((char)4);
    	assertEquals(1000, boundProperty.value());
    	
    	QCharBindable b1 = new QCharBindable(p1);
    	assertEquals(2, b1.value());
    	QCharBindable b2 = new QCharBindable(p2);
    	assertEquals(4, b2.value());
    	QCharBindable bindable = new QCharBindable(boundProperty);
    	assertTrue(bindable.binding().isNull());
    	assertFalse(bindable.makeBinding().isNull());
    	assertEquals(1000, bindable.value());
    	assertFalse(b1.makeBinding().isNull());
    	bindable.setBinding(b1.makeBinding());
    	assertFalse(bindable.binding().isNull());
    	assertEquals(2, bindable.value());
    	b1.setValue((char)90);
    	assertEquals(90, bindable.value());
    	bindable.setValue((char)2000);
    	assertTrue(boundProperty.binding().isNull());
    	assertEquals(2000, boundProperty.value());
    	b1.setValue((char)190);
    	assertEquals(2000, boundProperty.value());
    	boundProperty.setBinding(()->(char)(b1.value()+b2.value()));
    	assertEquals(4+190, boundProperty.value());
    	b2.setValue((char)5);
    	assertEquals(5+190, boundProperty.value());
    	b1.setBinding(()->6);
    	assertEquals(5+6, boundProperty.value());
    	boundProperty.takeBinding();
    	b1.setValue((char)0);
    	assertEquals(5+6, boundProperty.value());
    	if(canAlias) {
	    	QCharPropertyAlias b2Alias = new QCharPropertyAlias(b2);
	    	assertEquals(5, b2Alias.value());
	    	b2Alias.setValue((char)8);
	    	assertEquals(8, b2.value());
	    	QCharProperty p3 = new QCharProperty((char)143);
	    	b2Alias.setBinding(()->(char)(p3.value()-30));
	    	assertEquals(113, b2Alias.value());
	    	assertEquals(113, b2.value());
	    	assertEquals(113, p2.value());
    	}
    }
    
    @Test
    public void testQFloatProperty() {
    	QFloatProperty p1 = new QFloatProperty(5);
    	QFloatProperty p2 = new QFloatProperty(5);
    	QFloatProperty boundProperty = new QFloatProperty(()->p1.value()*p2.value());
    	AtomicInteger changeCounter = new AtomicInteger();
    	@SuppressWarnings("unused")
		QPropertyChangeHandler handler = boundProperty.onValueChanged(()->changeCounter.incrementAndGet());
    	ApplicationInitializer.runGC();
    	assertEquals(5*5, boundProperty.value(), 0.001);
    	p1.setValue(2);
    	assertEquals(1, changeCounter.intValue());
    	assertEquals(2*5, boundProperty.value(), 0.001);
    	p2.setValue(3);
    	assertEquals(2, changeCounter.intValue());
    	handler.dispose();
    	handler = null;
    	ApplicationInitializer.runGC();
    	assertFalse(boundProperty.binding().isNull());
    	assertEquals(2*3, boundProperty.value(), 0.001);
    	boundProperty.setValue(1000);
    	assertEquals(2, changeCounter.intValue(), 0.001);
    	assertTrue(boundProperty.binding().isNull());
    	assertEquals(1000, boundProperty.value(), 0.001);
    	p2.setValue(4);
    	assertEquals(1000, boundProperty.value(), 0.001);
    	
    	QFloatBindable b1 = new QFloatBindable(p1);
    	assertEquals(2, b1.value(), 0.001);
    	QFloatBindable b2 = new QFloatBindable(p2);
    	assertEquals(4, b2.value(), 0.001);
    	QFloatBindable bindable = new QFloatBindable(boundProperty);
    	assertTrue(bindable.binding().isNull());
    	assertFalse(bindable.makeBinding().isNull());
    	assertEquals(1000, bindable.value(), 0.001);
    	bindable.setBinding(b1.makeBinding());
    	assertFalse(bindable.binding().isNull());
    	assertEquals(2, bindable.value(), 0.001);
    	b1.setValue(90);
    	assertEquals(90, bindable.value(), 0.001);
    	bindable.setValue(2000);
    	assertTrue(boundProperty.binding().isNull());
    	assertEquals(2000, boundProperty.value(), 0.001);
    	b1.setValue(190);
    	assertEquals(2000, boundProperty.value(), 0.001);
    	boundProperty.setBinding(()->b1.value()+b2.value());
    	assertEquals(4+190, boundProperty.value(), 0.001);
    	b2.setValue(5);
    	assertEquals(5+190, boundProperty.value(), 0.001);
    	b1.setBinding(()->6);
    	assertEquals(5+6, boundProperty.value(), 0.001);
    	boundProperty.takeBinding();
    	b1.setValue(0);
    	assertEquals(5+6, boundProperty.value(), 0.001);
    	if(canAlias) {
	    	QFloatPropertyAlias b2Alias = new QFloatPropertyAlias(b2);
	    	assertEquals(5, b2Alias.value(), 0.001);
	    	b2Alias.setValue(8);
	    	assertEquals(8, b2.value(), 0.001);
	    	QFloatProperty p3 = new QFloatProperty(143);
	    	b2Alias.setBinding(()->p3.value()-30);
	    	assertEquals(113, b2Alias.value(), 0.001);
	    	assertEquals(113, b2.value(), 0.001);
	    	assertEquals(113, p2.value(), 0.001);
    	}
    }
    
    @Test
    public void testQDoubleProperty() {
    	QDoubleProperty p1 = new QDoubleProperty(5);
    	QDoubleProperty p2 = new QDoubleProperty(5);
    	QDoubleProperty boundProperty = new QDoubleProperty(()->p1.value()*p2.value());
    	AtomicInteger changeCounter = new AtomicInteger();
    	@SuppressWarnings("unused")
		QPropertyChangeHandler handler = boundProperty.onValueChanged(()->changeCounter.incrementAndGet());
    	ApplicationInitializer.runGC();
    	assertEquals(5*5, boundProperty.value(), 0.001);
    	p1.setValue(2);
    	assertEquals(1, changeCounter.intValue());
    	assertEquals(2*5, boundProperty.value(), 0.001);
    	p2.setValue(3);
    	assertEquals(2, changeCounter.intValue());
    	handler.dispose();
    	handler = null;
    	ApplicationInitializer.runGC();
    	assertFalse(boundProperty.binding().isNull());
    	assertEquals(2*3, boundProperty.value(), 0.001);
    	boundProperty.setValue(1000);
    	assertEquals(2, changeCounter.intValue(), 0.001);
    	assertTrue(boundProperty.binding().isNull());
    	assertEquals(1000, boundProperty.value(), 0.001);
    	p2.setValue(4);
    	assertEquals(1000, boundProperty.value(), 0.001);
    	
    	QDoubleBindable b1 = new QDoubleBindable(p1);
    	assertEquals(2, b1.value(), 0.001);
    	QDoubleBindable b2 = new QDoubleBindable(p2);
    	assertEquals(4, b2.value(), 0.001);
    	QDoubleBindable bindable = new QDoubleBindable(boundProperty);
    	assertTrue(bindable.binding().isNull());
    	assertFalse(bindable.makeBinding().isNull());
    	assertEquals(1000, bindable.value(), 0.001);
    	bindable.setBinding(b1.makeBinding());
    	assertFalse(bindable.binding().isNull());
    	assertEquals(2, bindable.value(), 0.001);
    	b1.setValue(90);
    	assertEquals(90, bindable.value(), 0.001);
    	bindable.setValue(2000);
    	assertTrue(boundProperty.binding().isNull());
    	assertEquals(2000, boundProperty.value(), 0.001);
    	b1.setValue(190);
    	assertEquals(2000, boundProperty.value(), 0.001);
    	boundProperty.setBinding(()->b1.value()+b2.value());
    	assertEquals(4+190, boundProperty.value(), 0.001);
    	b2.setValue(5);
    	assertEquals(5+190, boundProperty.value(), 0.001);
    	b1.setBinding(()->6);
    	assertEquals(5+6, boundProperty.value(), 0.001);
    	boundProperty.takeBinding();
    	b1.setValue(0);
    	assertEquals(5+6, boundProperty.value(), 0.001);
    	if(canAlias) {
	    	QDoublePropertyAlias b2Alias = new QDoublePropertyAlias(b2);
	    	assertEquals(5, b2Alias.value(), 0.001);
	    	b2Alias.setValue(8);
	    	assertEquals(8, b2.value(), 0.001);
	    	QDoubleProperty p3 = new QDoubleProperty(143);
	    	b2Alias.setBinding(()->p3.value()-30);
	    	assertEquals(113, b2Alias.value(), 0.001);
	    	assertEquals(113, b2.value(), 0.001);
	    	assertEquals(113, p2.value(), 0.001);
    	}
    }
    
    @Test
    public void testQBooleanProperty() {
    	QBooleanProperty p1 = new QBooleanProperty(false);
    	QBooleanProperty p2 = new QBooleanProperty(true);
    	QBooleanProperty boundProperty = new QBooleanProperty(()->p1.value() || p2.value());
    	AtomicInteger changeCounter = new AtomicInteger();
    	@SuppressWarnings("unused")
		QPropertyChangeHandler handler = boundProperty.onValueChanged(()->changeCounter.incrementAndGet());
    	ApplicationInitializer.runGC();
    	assertTrue(boundProperty.value());
    	p1.setValue(true);
    	assertEquals(0, changeCounter.intValue());
    	assertTrue(boundProperty.value());
    	p1.setValue(false);
    	p2.setValue(false);
    	assertEquals(1, changeCounter.intValue());
    	assertFalse(boundProperty.value());
    	handler.dispose();
    	handler = null;
    	ApplicationInitializer.runGC();
    	assertFalse(boundProperty.binding().isNull());
    	
    	QBooleanBindable b1 = new QBooleanBindable(p1);
    	assertFalse(b1.value());
    	QBooleanBindable b2 = new QBooleanBindable(p2);
    	assertFalse(b2.value());
    	QBooleanBindable bindable = new QBooleanBindable(boundProperty);
    	assertFalse(bindable.binding().isNull());
    	assertFalse(bindable.makeBinding().isNull());
    	assertFalse(bindable.value());
    	bindable.setBinding(b1.makeBinding());
    	assertFalse(bindable.binding().isNull());
    	assertFalse(bindable.value());
    	b1.setValue(false);
    	assertFalse(bindable.value());
    	bindable.setValue(true);
    	assertTrue(boundProperty.binding().isNull());
    	assertTrue(boundProperty.value());
    	boundProperty.setBinding(()->!b1.value() && b2.value());
    	assertFalse(boundProperty.value());
    	b2.setValue(true);
    	assertTrue(boundProperty.value());
    	b1.setBinding(()->true);
    	assertFalse(boundProperty.value());
    	boundProperty.takeBinding();
    	b1.setValue(false);
    	assertFalse(boundProperty.value());
    	if(canAlias) {
	    	QBooleanPropertyAlias b2Alias = new QBooleanPropertyAlias(b2);
	    	assertTrue(b2Alias.value());
	    	b2Alias.setValue(true);
	    	assertTrue(b2.value());
	    	QBooleanProperty p3 = new QBooleanProperty(true);
	    	b2Alias.setBinding(()->p3.value());
	    	assertTrue(b2Alias.value());
	    	assertTrue(b2.value());
	    	assertTrue(p2.value());
    	}
    }
    
    @Test
    public void testQProperty_Byte() {
    	QProperty<Byte> p1 = new QProperty<Byte>((byte)5);
    	QProperty<Byte> p2 = new QProperty<Byte>((byte)5);
    	QProperty<Byte> boundProperty = new QProperty<Byte>(()->(byte)(p1.value()*p2.value()));
    	AtomicInteger changeCounter = new AtomicInteger();
    	@SuppressWarnings("unused")
		QPropertyChangeHandler handler = boundProperty.onValueChanged(()->changeCounter.incrementAndGet());
    	ApplicationInitializer.runGC();
    	assertEquals(5*5, (byte)boundProperty.value());
    	p1.setValue((byte)2);
    	assertEquals(1, changeCounter.intValue());
    	assertEquals(2*5, (byte)boundProperty.value());
    	p2.setValue((byte)3);
    	assertEquals(2, changeCounter.intValue());
    	handler.dispose();
    	handler = null;
    	ApplicationInitializer.runGC();
    	assertFalse(boundProperty.binding().isNull());
    	assertEquals(2*3, (byte)boundProperty.value());
    	boundProperty.setValue((byte)100);
    	assertEquals(2, changeCounter.intValue());
    	assertTrue(boundProperty.binding().isNull());
    	assertEquals(100, (byte)boundProperty.value());
    	p2.setValue((byte)4);
    	assertEquals(100, (byte)boundProperty.value());
    	
    	QBindable<Byte> b1 = new QBindable<>(p1);
    	assertEquals(2, (byte)b1.value());
    	QBindable<Byte> b2 = new QBindable<>(p2);
    	assertEquals(4, (byte)b2.value());
    	QBindable<Byte> bindable = new QBindable<>(boundProperty);
    	assertTrue(bindable.binding().isNull());
    	assertFalse(bindable.makeBinding().isNull());
    	assertEquals(100, (byte)bindable.value());
    	bindable.setBinding(b1.makeBinding());
    	assertFalse(bindable.binding().isNull());
    	assertEquals(2, (byte)bindable.value());
    	b1.setValue((byte)90);
    	assertEquals(90, (byte)bindable.value());
    	bindable.setValue((byte)20);
    	assertTrue(boundProperty.binding().isNull());
    	assertEquals(20, (byte)boundProperty.value());
    	b1.setValue((byte)19);
    	assertEquals(20, (byte)boundProperty.value());
    	boundProperty.setBinding(()->(byte)(b1.value()+b2.value()));
    	assertEquals(4+19, (byte)boundProperty.value());
    	b2.setValue((byte)5);
    	assertEquals(5+19, (byte)boundProperty.value());
    	b1.setBinding(()->(byte)6);
    	assertEquals(5+6, (byte)boundProperty.value());
    	boundProperty.takeBinding();
    	b1.setValue((byte)0);
    	assertEquals(5+6, (byte)boundProperty.value());
    	if(canAlias) {
	    	QPropertyAlias<Byte> b2Alias = new QPropertyAlias<>(b2);
	    	assertEquals(5, (byte)b2Alias.value());
	    	b2Alias.setValue((byte)8);
	    	assertEquals(8, (byte)b2.value());
	    	QProperty<Byte> p3 = new QProperty<Byte>((byte)43);
	    	b2Alias.setBinding(()->(byte)(p3.value()-30));
	    	assertEquals(13, (byte)b2Alias.value());
	    	assertEquals(13, (byte)b2.value());
	    	assertEquals(13, (byte)p2.value());
    	}
    }
    
    @Test
	@SuppressWarnings({ "rawtypes", "serial", "unchecked" })
    public void testQProperty_QVariant() {
    	QProperty<Integer> p1 = new QProperty<Integer>(5);
    	QProperty<Integer> p2 = new QProperty<Integer>(5);    	
		QProperty boundProperty = new QProperty(
    			new QtUtilities.Supplier() {
					@Override
					public Object get() {
						return p1.value()*p2.value();
					}
    			}
			);
    	assertFalse(boundProperty.binding().isNull());
    	QBindable bindable = new QBindable(p1);
    	assertEquals(QMetaType.fromType(int.class), bindable.metaType());
    	assertTrue(bindable.binding().isNull());
    	assertFalse(bindable.makeBinding().isNull());
    	bindable = new QBindable(p2);
    	assertEquals(QMetaType.fromType(int.class), bindable.metaType());
    	assertTrue(bindable.binding().isNull());
    	assertFalse(bindable.makeBinding().isNull());
    	bindable = new QBindable(boundProperty);
    	assertEquals(new QMetaType(QMetaType.Type.QVariant), bindable.metaType());
    	assertFalse(bindable.binding().isNull());
    	assertFalse(bindable.makeBinding().isNull());
    	QIntBindable ibindable = new QIntBindable(boundProperty);
    	assertEquals(new QMetaType(QMetaType.Type.QVariant), ibindable.metaType());
    	assertFalse(ibindable.binding().isNull());
    	assertFalse(ibindable.makeBinding().isNull());
    	assertEquals(25, ibindable.value());
    }
    
    @Test
    public void testQProperty_Int() {
    	QProperty<Integer> p1 = new QProperty<Integer>(5);
    	QProperty<Integer> p2 = new QProperty<Integer>(5);
    	QProperty<Integer> boundProperty = new QProperty<Integer>(()->p1.value()*p2.value());
    	AtomicInteger changeCounter = new AtomicInteger();
    	@SuppressWarnings("unused")
		QPropertyChangeHandler handler = boundProperty.onValueChanged(()->changeCounter.incrementAndGet());
    	ApplicationInitializer.runGC();
    	assertEquals(5*5, (int)boundProperty.value());
    	p1.setValue(2);
    	assertEquals(1, changeCounter.intValue());
    	assertEquals(2*5, (int)boundProperty.value());
    	p2.setValue(3);
    	assertEquals(2, changeCounter.intValue());
    	assertFalse(boundProperty.binding().isNull());
    	assertEquals(2*3, (int)boundProperty.value());
    	handler.dispose();
    	handler = null;
    	ApplicationInitializer.runGC();
    	boundProperty.setValue(1000);
    	assertEquals(2, changeCounter.intValue());
    	assertTrue(boundProperty.binding().isNull());
    	assertEquals(1000, (int)boundProperty.value());
    	p2.setValue(4);
    	assertEquals(1000, (int)boundProperty.value());
    	
    	QBindable<Integer> b1 = new QBindable<>(p1);
    	assertEquals(2, (int)b1.value());
    	QBindable<Integer> b2 = new QBindable<>(p2);
    	assertEquals(4, (int)b2.value());
    	QIntBindable bindable = new QIntBindable(boundProperty);
    	assertTrue(bindable.binding().isNull());
    	assertFalse(bindable.makeBinding().isNull());
    	assertEquals(1000, bindable.value());
    	bindable.setBinding(b1.makeBinding());
    	assertFalse(bindable.binding().isNull());
    	assertEquals(2, bindable.value());
    	b1.setValue(90);
    	assertEquals(90, bindable.value());
    	bindable.setValue(2000);
    	assertTrue(boundProperty.binding().isNull());
    	assertEquals(2000, (int)boundProperty.value());
    	b1.setValue(190);
    	assertEquals(2000, (int)boundProperty.value());
    	boundProperty.setBinding(()->b1.value()+b2.value());
    	assertEquals(4+190, (int)boundProperty.value());
    	b2.setValue(5);
    	assertEquals(5+190, (int)boundProperty.value());
    	b1.setBinding(()->6);
    	assertEquals(5+6, (int)boundProperty.value());
    	boundProperty.takeBinding();
    	b1.setValue(0);
    	assertEquals(5+6, (int)boundProperty.value());
    	if(canAlias) {
	    	QPropertyAlias<Integer> b2Alias = new QPropertyAlias<>(b2);
	    	assertEquals(5, (int)b2Alias.value());
	    	b2Alias.setValue(8);
	    	assertEquals(8, (int)b2.value());
	    	QIntProperty p3 = new QIntProperty(143);
	    	b2Alias.setBinding(()->p3.value()-30);
	    	assertEquals(113, (int)b2Alias.value());
	    	assertEquals(113, (int)b2.value());
	    	assertEquals(113, (int)p2.value());
    	}
    }
    
    @Test
    public void testQProperty_Short() {
    	QProperty<Short> p1 = new QProperty<Short>((short)5);
    	QProperty<Short> p2 = new QProperty<Short>((short)5);
    	QProperty<Short> boundProperty = new QProperty<Short>(()->(short)(p1.value()*p2.value()));
    	AtomicInteger changeCounter = new AtomicInteger();
    	@SuppressWarnings("unused")
		QPropertyChangeHandler handler = boundProperty.onValueChanged(()->changeCounter.incrementAndGet());
    	ApplicationInitializer.runGC();
    	assertEquals(5*5, (short)boundProperty.value());
    	p1.setValue((short)2);
    	assertEquals(1, changeCounter.intValue());
    	assertEquals(2*5, (short)boundProperty.value());
    	p2.setValue((short)3);
    	assertEquals(2, changeCounter.intValue());
    	handler.dispose();
    	handler = null;
    	ApplicationInitializer.runGC();
    	assertFalse(boundProperty.binding().isNull());
    	assertEquals(2*3, (short)boundProperty.value());
    	boundProperty.setValue((short)1000);
    	assertEquals(2, changeCounter.intValue());
    	assertTrue(boundProperty.binding().isNull());
    	assertEquals(1000, (short)boundProperty.value());
    	p2.setValue((short)4);
    	assertEquals(1000, (short)boundProperty.value());
    	
    	QBindable<Short> b1 = new QBindable<>(p1);
    	assertEquals(2, (short)b1.value());
    	QBindable<Short> b2 = new QBindable<>(p2);
    	assertEquals(4, (short)b2.value());
    	QBindable<Short> bindable = new QBindable<>(boundProperty);
    	assertTrue(bindable.binding().isNull());
    	assertFalse(bindable.makeBinding().isNull());
    	assertEquals(1000, (short)bindable.value());
    	bindable.setBinding(b1.makeBinding());
    	assertFalse(bindable.binding().isNull());
    	assertEquals(2, (short)bindable.value());
    	b1.setValue((short)90);
    	assertEquals(90, (short)bindable.value());
    	bindable.setValue((short)2000);
    	assertTrue(boundProperty.binding().isNull());
    	assertEquals(2000, (short)boundProperty.value());
    	b1.setValue((short)190);
    	assertEquals(2000, (short)boundProperty.value());
    	boundProperty.setBinding(()->(short)(b1.value()+b2.value()));
    	assertEquals(4+190, (short)boundProperty.value());
    	b2.setValue((short)5);
    	assertEquals(5+190, (short)boundProperty.value());
    	b1.setBinding(()->(short)6);
    	assertEquals(5+6, (short)boundProperty.value());
    	boundProperty.takeBinding();
    	b1.setValue((short)0);
    	assertEquals(5+6, (short)boundProperty.value());
    	if(canAlias) {
	    	QPropertyAlias<Short> b2Alias = new QPropertyAlias<>(b2);
	    	assertEquals(5, (short)b2Alias.value());
	    	b2Alias.setValue((short)8);
	    	assertEquals(8, (short)b2.value());
	    	QProperty<Short> p3 = new QProperty<>((short)143);
	    	b2Alias.setBinding(()->(short)(p3.value()-30));
	    	assertEquals(113, (short)b2Alias.value());
	    	assertEquals(113, (short)b2.value());
	    	assertEquals(113, (short)p2.value());
    	}
    }
    
    @Test
    public void testQProperty_Long() {
    	QProperty<Long> p1 = new QProperty<>(5l);
    	QProperty<Long> p2 = new QProperty<>(5l);
    	QProperty<Long> boundProperty = new QProperty<>(()->p1.value()*p2.value());
    	AtomicInteger changeCounter = new AtomicInteger();
    	@SuppressWarnings("unused")
		QPropertyChangeHandler handler = boundProperty.onValueChanged(()->changeCounter.incrementAndGet());
    	ApplicationInitializer.runGC();
    	assertEquals(5*5, (long)boundProperty.value());
    	p1.setValue(2l);
    	assertEquals(1, changeCounter.intValue());
    	assertEquals(2*5, (long)boundProperty.value());
    	p2.setValue(3l);
    	assertEquals(2, changeCounter.intValue());
    	handler.dispose();
    	handler = null;
    	ApplicationInitializer.runGC();
    	assertFalse(boundProperty.binding().isNull());
    	assertEquals(2*3, (long)boundProperty.value());
    	boundProperty.setValue(1000l);
    	assertEquals(2, changeCounter.intValue());
    	assertTrue(boundProperty.binding().isNull());
    	assertEquals(1000, (long)boundProperty.value());
    	p2.setValue(4l);
    	assertEquals(1000, (long)boundProperty.value());
    	
    	QBindable<Long> b1 = new QBindable<>(p1);
    	assertEquals(2, (long)b1.value());
    	QBindable<Long> b2 = new QBindable<>(p2);
    	assertEquals(4, (long)b2.value());
    	QBindable<Long> bindable = new QBindable<>(boundProperty);
    	assertTrue(bindable.binding().isNull());
    	assertFalse(bindable.makeBinding().isNull());
    	assertEquals(1000, (long)bindable.value());
    	bindable.setBinding(b1.makeBinding());
    	assertFalse(bindable.binding().isNull());
    	assertEquals(2, (long)bindable.value());
    	b1.setValue(90l);
    	assertEquals(90, (long)bindable.value());
    	bindable.setValue(2000l);
    	assertTrue(boundProperty.binding().isNull());
    	assertEquals(2000, (long)boundProperty.value());
    	b1.setValue(190l);
    	assertEquals(2000, (long)boundProperty.value());
    	boundProperty.setBinding(()->b1.value()+b2.value());
    	assertEquals(4+190, (long)boundProperty.value());
    	b2.setValue(5l);
    	assertEquals(5+190, (long)boundProperty.value());
    	b1.setBinding(()->6l);
    	assertEquals(5+6, (long)boundProperty.value());
    	boundProperty.takeBinding();
    	b1.setValue(0l);
    	assertEquals(5+6, (long)boundProperty.value());
    	if(canAlias) {
	    	QPropertyAlias<Long> b2Alias = new QPropertyAlias<>(b2);
	    	assertEquals(5, (long)b2Alias.value());
	    	b2Alias.setValue(8l);
	    	assertEquals(8, (long)b2.value());
	    	QProperty<Long> p3 = new QProperty<>(143l);
	    	b2Alias.setBinding(()->p3.value()-30);
	    	assertEquals(113, (long)b2Alias.value());
	    	assertEquals(113, (long)b2.value());
	    	assertEquals(113, (long)p2.value());
    	}
    }
    
    @Test
    public void testQProperty_Char() {
    	QProperty<Character> p1 = new QProperty<>((char)5);
    	QProperty<Character> p2 = new QProperty<>((char)5);
    	QProperty<Character> boundProperty = new QProperty<>(()->(char)(p1.value()*p2.value()));
    	AtomicInteger changeCounter = new AtomicInteger();
    	@SuppressWarnings("unused")
		QPropertyChangeHandler handler = boundProperty.onValueChanged(()->changeCounter.incrementAndGet());
    	ApplicationInitializer.runGC();
    	assertEquals(5*5, (char)boundProperty.value());
    	p1.setValue((char)2);
    	assertEquals(1, changeCounter.intValue());
    	assertEquals(2*5, (char)boundProperty.value());
    	p2.setValue((char)3);
    	assertEquals(2, changeCounter.intValue());
    	handler.dispose();
    	handler = null;
    	ApplicationInitializer.runGC();
    	assertFalse(boundProperty.binding().isNull());
    	assertEquals(2*3, (char)boundProperty.value());
    	boundProperty.setValue((char)1000);
    	assertEquals(2, changeCounter.intValue());
    	assertTrue(boundProperty.binding().isNull());
    	assertEquals(1000, (char)boundProperty.value());
    	p2.setValue((char)4);
    	assertEquals(1000, (char)boundProperty.value());
    	
    	QBindable<Character> b1 = new QBindable<>(p1);
    	assertEquals(2, (char)b1.value());
    	QBindable<Character> b2 = new QBindable<>(p2);
    	assertEquals(4, (char)b2.value());
    	QBindable<Character> bindable = new QBindable<>(boundProperty);
    	assertTrue(bindable.binding().isNull());
    	assertFalse(bindable.makeBinding().isNull());
    	assertEquals(1000, (char)bindable.value());
    	assertFalse(b1.makeBinding().isNull());
    	bindable.setBinding(b1.makeBinding());
    	assertFalse(bindable.binding().isNull());
    	assertEquals(2, (char)bindable.value());
    	b1.setValue((char)90);
    	assertEquals(90, (char)bindable.value());
    	bindable.setValue((char)2000);
    	assertTrue(boundProperty.binding().isNull());
    	assertEquals(2000, (char)boundProperty.value());
    	b1.setValue((char)190);
    	assertEquals(2000, (char)boundProperty.value());
    	boundProperty.setBinding(()->(char)(b1.value()+b2.value()));
    	assertEquals(4+190, (char)boundProperty.value());
    	b2.setValue((char)5);
    	assertEquals(5+190, (char)boundProperty.value());
    	b1.setBinding(()->(char)6);
    	assertEquals(5+6, (char)boundProperty.value());
    	boundProperty.takeBinding();
    	b1.setValue((char)0);
    	assertEquals(5+6, (char)boundProperty.value());
    	if(canAlias) {
	    	QPropertyAlias<Character> b2Alias = new QPropertyAlias<>(b2);
	    	assertEquals(5, (char)b2Alias.value());
	    	b2Alias.setValue((char)8);
	    	assertEquals(8, (char)b2.value());
	    	QProperty<Character> p3 = new QProperty<>((char)143);
	    	b2Alias.setBinding(()->(char)(p3.value()-30));
	    	assertEquals(113, (char)b2Alias.value());
	    	assertEquals(113, (char)b2.value());
	    	assertEquals(113, (char)p2.value());
    	}
    }
    
    @Test
    public void testQProperty_Float() {
    	QProperty<Float> p1 = new QProperty<>(5f);
    	QProperty<Float> p2 = new QProperty<>(5f);
    	QProperty<Float> boundProperty = new QProperty<>(()->p1.value()*p2.value());
    	AtomicInteger changeCounter = new AtomicInteger();
    	@SuppressWarnings("unused")
		QPropertyChangeHandler handler = boundProperty.onValueChanged(()->changeCounter.incrementAndGet());
    	ApplicationInitializer.runGC();
    	assertEquals(5*5, boundProperty.value(), 0.001);
    	p1.setValue(2f);
    	assertEquals(1, changeCounter.intValue());
    	assertEquals(2*5, boundProperty.value(), 0.001);
    	p2.setValue(3f);
    	assertEquals(2, changeCounter.intValue());
    	handler.dispose();
    	handler = null;
    	ApplicationInitializer.runGC();
    	assertFalse(boundProperty.binding().isNull());
    	assertEquals(2*3, boundProperty.value(), 0.001);
    	boundProperty.setValue(1000f);
    	assertEquals(2, changeCounter.intValue(), 0.001);
    	assertTrue(boundProperty.binding().isNull());
    	assertEquals(1000, boundProperty.value(), 0.001);
    	p2.setValue(4f);
    	assertEquals(1000, boundProperty.value(), 0.001);
    	
    	QBindable<Float> b1 = new QBindable<>(p1);
    	assertEquals(2, b1.value(), 0.001);
    	QBindable<Float> b2 = new QBindable<>(p2);
    	assertEquals(4, b2.value(), 0.001);
    	QBindable<Float> bindable = new QBindable<>(boundProperty);
    	assertTrue(bindable.binding().isNull());
    	assertFalse(bindable.makeBinding().isNull());
    	assertEquals(1000, bindable.value(), 0.001);
    	bindable.setBinding(b1.makeBinding());
    	assertFalse(bindable.binding().isNull());
    	assertEquals(2, bindable.value(), 0.001);
    	b1.setValue(90f);
    	assertEquals(90, bindable.value(), 0.001);
    	bindable.setValue(2000f);
    	assertTrue(boundProperty.binding().isNull());
    	assertEquals(2000, boundProperty.value(), 0.001);
    	b1.setValue(190f);
    	assertEquals(2000, boundProperty.value(), 0.001);
    	boundProperty.setBinding(()->b1.value()+b2.value());
    	assertEquals(4+190, boundProperty.value(), 0.001);
    	b2.setValue(5f);
    	assertEquals(5+190, boundProperty.value(), 0.001);
    	b1.setBinding(()->6f);
    	assertEquals(5+6, boundProperty.value(), 0.001);
    	boundProperty.takeBinding();
    	b1.setValue(0f);
    	assertEquals(5+6, boundProperty.value(), 0.001);
    	if(canAlias) {
	    	QPropertyAlias<Float> b2Alias = new QPropertyAlias<>(b2);
	    	assertEquals(5, b2Alias.value(), 0.001);
	    	b2Alias.setValue(8f);
	    	assertEquals(8, b2.value(), 0.001);
	    	QFloatProperty p3 = new QFloatProperty(143);
	    	b2Alias.setBinding(()->p3.value()-30);
	    	assertEquals(113, b2Alias.value(), 0.001);
	    	assertEquals(113, b2.value(), 0.001);
	    	assertEquals(113, p2.value(), 0.001);
    	}
    }
    
    @Test
    public void testQProperty_Double() {
    	QProperty<Double> p1 = new QProperty<>(5.);
    	QProperty<Double> p2 = new QProperty<>(5.);
    	QProperty<Double> boundProperty = new QProperty<>(()->p1.value()*p2.value());
    	AtomicInteger changeCounter = new AtomicInteger();
    	@SuppressWarnings("unused")
		QPropertyChangeHandler handler = boundProperty.onValueChanged(()->changeCounter.incrementAndGet());
    	ApplicationInitializer.runGC();
    	assertEquals(5*5, boundProperty.value(), 0.001);
    	p1.setValue(2.);
    	assertEquals(1, changeCounter.intValue());
    	assertEquals(2*5, boundProperty.value(), 0.001);
    	p2.setValue(3.);
    	assertEquals(2, changeCounter.intValue());
    	handler.dispose();
    	handler = null;
    	ApplicationInitializer.runGC();
    	assertFalse(boundProperty.binding().isNull());
    	assertEquals(2*3, boundProperty.value(), 0.001);
    	boundProperty.setValue(1000.);
    	assertEquals(2, changeCounter.intValue(), 0.001);
    	assertTrue(boundProperty.binding().isNull());
    	assertEquals(1000, boundProperty.value(), 0.001);
    	p2.setValue(4.);
    	assertEquals(1000, boundProperty.value(), 0.001);
    	
    	QBindable<Double> b1 = new QBindable<>(p1);
    	assertEquals(2, b1.value(), 0.001);
    	QBindable<Double> b2 = new QBindable<>(p2);
    	assertEquals(4, b2.value(), 0.001);
    	QBindable<Double> bindable = new QBindable<>(boundProperty);
    	assertTrue(bindable.binding().isNull());
    	assertFalse(bindable.makeBinding().isNull());
    	assertEquals(1000, bindable.value(), 0.001);
    	bindable.setBinding(b1.makeBinding());
    	assertFalse(bindable.binding().isNull());
    	assertEquals(2, bindable.value(), 0.001);
    	b1.setValue(90.);
    	assertEquals(90, bindable.value(), 0.001);
    	bindable.setValue(2000.);
    	assertTrue(boundProperty.binding().isNull());
    	assertEquals(2000, boundProperty.value(), 0.001);
    	b1.setValue(190.);
    	assertEquals(2000, boundProperty.value(), 0.001);
    	boundProperty.setBinding(()->b1.value()+b2.value());
    	assertEquals(4+190, boundProperty.value(), 0.001);
    	b2.setValue(5.);
    	assertEquals(5+190, boundProperty.value(), 0.001);
    	b1.setBinding(()->6.);
    	assertEquals(5+6, boundProperty.value(), 0.001);
    	boundProperty.takeBinding();
    	b1.setValue(0.);
    	assertEquals(5+6, boundProperty.value(), 0.001);
    	if(canAlias) {
	    	QPropertyAlias<Double> b2Alias = new QPropertyAlias<>(b2);
	    	assertEquals(5, b2Alias.value(), 0.001);
	    	b2Alias.setValue(8.);
	    	assertEquals(8, b2.value(), 0.001);
	    	QDoubleProperty p3 = new QDoubleProperty(143);
	    	b2Alias.setBinding(()->p3.value()-30);
	    	assertEquals(113, b2Alias.value(), 0.001);
	    	assertEquals(113, b2.value(), 0.001);
	    	assertEquals(113, p2.value(), 0.001);
    	}
    }
    
    @Test
    public void testQProperty_Boolean() {
    	QProperty<Boolean> p1 = new QProperty<>(false);
    	QProperty<Boolean> p2 = new QProperty<>(true);
    	QProperty<Boolean> boundProperty = new QProperty<>(()->p1.value() || p2.value());
    	AtomicInteger changeCounter = new AtomicInteger();
    	@SuppressWarnings("unused")
		QPropertyChangeHandler handler = boundProperty.onValueChanged(()->changeCounter.incrementAndGet());
    	ApplicationInitializer.runGC();
    	assertTrue(boundProperty.value());
    	p1.setValue(true);
    	assertEquals(0, changeCounter.intValue());
    	assertTrue(boundProperty.value());
    	p1.setValue(false);
    	p2.setValue(false);
    	assertEquals(1, changeCounter.intValue());
    	assertFalse(boundProperty.value());
    	handler.dispose();
    	handler = null;
    	ApplicationInitializer.runGC();
    	assertFalse(boundProperty.binding().isNull());
    	
    	QBindable<Boolean> b1 = new QBindable<>(p1);
    	assertFalse(b1.value());
    	QBindable<Boolean> b2 = new QBindable<>(p2);
    	assertFalse(b2.value());
    	QBindable<Boolean> bindable = new QBindable<>(boundProperty);
    	assertFalse(bindable.binding().isNull());
    	assertFalse(bindable.makeBinding().isNull());
    	assertFalse(bindable.value());
    	bindable.setBinding(b1.makeBinding());
    	assertFalse(bindable.binding().isNull());
    	assertFalse(bindable.value());
    	b1.setValue(false);
    	assertFalse(bindable.value());
    	bindable.setValue(true);
    	assertTrue(boundProperty.binding().isNull());
    	assertTrue(boundProperty.value());
    	boundProperty.setBinding(()->!b1.value() && b2.value());
    	assertFalse(boundProperty.value());
    	b2.setValue(true);
    	assertTrue(boundProperty.value());
    	b1.setBinding(()->true);
    	assertFalse(boundProperty.value());
    	boundProperty.takeBinding();
    	b1.setValue(false);
    	assertFalse(boundProperty.value());
    	if(canAlias) {
	    	QPropertyAlias<Boolean> b2Alias = new QPropertyAlias<>(b2);
	    	assertTrue(b2Alias.value());
	    	b2Alias.setValue(true);
	    	assertTrue(b2.value());
	    	QBooleanProperty p3 = new QBooleanProperty(true);
	    	b2Alias.setBinding(()->p3.value());
	    	assertTrue(b2Alias.value());
	    	assertTrue(b2.value());
	    	assertTrue(p2.value());
    	}
    }
    
    @Test
    public void testQProperty_Pointer() {
    	QProperty<QGraphicsItem> p = new QProperty<>();
    	AtomicInteger signalCounter = new AtomicInteger();
    	@SuppressWarnings("unused")
		QPropertyChangeHandler listener = p.onValueChanged(signalCounter::incrementAndGet);
    	assertEquals(0, signalCounter.get());
    	QGraphicsWidget w1 = new QGraphicsWidget();
    	p.setValue(w1);
    	assertEquals(1, signalCounter.get());
    	assertEquals(w1, p.value());
    	p.setValue(new QGraphicsPathItem());
    	assertEquals(2, signalCounter.get());
    	ApplicationInitializer.runGC();
    	assertTrue(p.value() instanceof QGraphicsPathItem);
    	ApplicationInitializer.runGC();
    	assertTrue(p.value() instanceof QGraphicsPathItem);
    	ApplicationInitializer.runGC();
    	assertTrue(p.value() instanceof QGraphicsPathItem);
		QBindable<QGraphicsItem> pBindable = new QBindable<>(p);
    	QProperty<QGraphicsItem> freeProperty = new QProperty<>(new QGraphicsProxyWidget(), QGraphicsItem.class);
    	assertEquals("QGraphicsItem*", new QBindable<>(freeProperty).metaType().name());
    	assertTrue("freeProperty value is " + (freeProperty.value()==null ? "null" : freeProperty.value().getClass().getTypeName()), freeProperty.value() instanceof QGraphicsProxyWidget);
    	QBooleanProperty decider = new QBooleanProperty(false);
    	pBindable.setBinding(()->{
    		return decider.value() ? freeProperty.value() : null;
    	});
    	assertTrue(!pBindable.binding().isNull());
    	assertTrue(signalCounter.get()==3 || signalCounter.get()==4);
    	assertEquals(null, p.value());
    	decider.setValue(true);
    	assertTrue(p.value() instanceof QGraphicsProxyWidget);
    }
    
    //this is expected to crash from time to time
    @Test
    public void testQPropertyChangeHandlerDelete() {
    	Assume.assumeTrue("Disabled unless you specify test.enable.criticals=true", Boolean.getBoolean("test.enable.criticals"));
    	QProperty<String> prop = new QProperty<>("");
    	List<QPropertyChangeHandler> handlers = new ArrayList<>();
		for(int i=0; i<10000; ++i) {
			handlers.add(prop.onValueChanged(()->{}));
		}
    	QThread thread = QThread.create(()->{
    		while(!QThread.interrupted()){
    			prop.setValue(""+System.nanoTime());
    		}
    	});
    	thread.start();
    	QThread.yieldCurrentThread();
		QThread.msleep(1);
    	for (QPropertyChangeHandler handler : handlers) {
			QThread.msleep(1);
			handler.dispose();			
		}
    	thread.interrupt();
    }
}
