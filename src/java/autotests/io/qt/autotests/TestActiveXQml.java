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

import org.junit.Assert;
import org.junit.Assume;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.activex.QAxWidget;
import io.qt.core.QMetaObject;
import io.qt.qml.util.QmlElement;

public class TestActiveXQml extends ApplicationInitializer {
	
	@BeforeClass
    public static void testInitialize() throws Exception {
    	ApplicationInitializer.testInitializeWithWidgets();
    	try {
    		QAxWidget.staticMetaObject.hashCode();
    	} catch (Error e) {
			if(e.getMessage().startsWith("Cannot mix incompatible Qt library."))
				Assume.assumeNoException(e.getMessage(), e);
			else throw e;
		}
    	io.qt.QtUtilities.initializePackage("io.qt.qml");
    }
	
	@QmlElement
	private static class AxWidgetSubclass extends QAxWidget{
	}
	
	@Test
    public void testSubclassing() {
		try {
			QMetaObject.forType(AxWidgetSubclass.class);
			Assert.fail("UnsupportedOperationException expected to be thrown");
		}catch(UnsupportedOperationException e) {
			Assert.assertEquals("Cannot add @QmlElement to class io.qt.autotests.TestActiveXQml.AxWidgetSubclass because it extends type with dynamic meta object.", e.getMessage());
		}
		try {
			new AxWidgetSubclass();
			Assert.fail("UnsupportedOperationException expected to be thrown");
		}catch(UnsupportedOperationException e) {
			Assert.assertEquals("Cannot add @QmlElement to class io.qt.autotests.TestActiveXQml.AxWidgetSubclass because it extends type with dynamic meta object.", e.getMessage());
		}
	}

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestActiveXQml.class.getName());
    }
}
