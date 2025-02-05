/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.QtPropertyReader;
import io.qt.core.QMetaObject;
import io.qt.core.QMetaType;
import io.qt.core.QObject;
import org.junit.Assert;

public class TestMetaObjectMetaTypeQt65 extends ApplicationInitializer {
	
	@BeforeClass
    public static void testInitialize() throws Exception {
    	ApplicationInitializer.testInitializeWithWidgets();
    }
	
	static class Object1 extends QObject{
		public final Signal0 enabledChanged = new Signal0();
		public final Signal0 visibleChanged = new Signal0();
    	private boolean enabled;
    	private boolean visible;

    	@QtPropertyReader(name="isEnabled")
		public boolean isEnabled() {
			return enabled;
		}

		public void setEnabled(boolean enabled) {
			this.enabled = enabled;
			enabledChanged.emit();
		}

		public boolean visible() {
			return visible;
		}

		public void setVisible(boolean visible) {
			this.visible = visible;
			enabledChanged.emit();
		}
    }
	
	static class Object2 extends QObject{
		public final Signal0 enabledChanged = new Signal0();
		public final Signal0 visibleChanged = new Signal0();
    	private boolean enabled;
    	private boolean visible;

    	@QtPropertyReader(name="isEnabled")
		public boolean isEnabled() {
			return enabled;
		}

		public void setEnabled(boolean enabled) {
			this.enabled = enabled;
			enabledChanged.emit();
		}

		public boolean visible() {
			return visible;
		}

		public void setVisible(boolean visible) {
			this.visible = visible;
			enabledChanged.emit();
		}
    }
	
	@Test 
	public void test1() {
		QMetaType metaType = QMetaType.fromType(Object1.class);
		QMetaObject metaObject = metaType.metaObject();
		Assert.assertTrue(metaObject!=null);
		Assert.assertEquals(2, metaObject.propertyCount()-QObject.staticMetaObject.propertyCount());
		Assert.assertEquals(6, metaObject.methodCount()-QObject.staticMetaObject.methodCount());
	}
	@Test 
	public void test2() {
		QMetaObject metaObject = QMetaObject.forType(Object1.class);
		QMetaType metaType = metaObject.metaType();
		Assert.assertTrue(metaType.isValid());
		Assert.assertEquals(2, metaObject.propertyCount()-QObject.staticMetaObject.propertyCount());
		Assert.assertEquals(6, metaObject.methodCount()-QObject.staticMetaObject.methodCount());
	}

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestMetaObjectMetaTypeQt65.class.getName());
    }
}
