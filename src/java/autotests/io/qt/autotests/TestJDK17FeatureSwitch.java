/****************************************************************************
**
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

import static org.junit.Assert.assertEquals;

import java.lang.reflect.Field;
import java.lang.reflect.Modifier;
import java.util.Arrays;

import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.core.QEvent;
import io.qt.core.QMetaObject;

public class TestJDK17FeatureSwitch extends ApplicationInitializer {
	
	@BeforeClass
	public static void testInitialize() throws Exception {
		ApplicationInitializer.testInitializeWithWidgets();
		QMetaObject.forType(TestJDK17FeatureSwitch.class);
    }
    
	@Test
    public void test() {
    	assertEquals("MouseButtonPress", testEvent(QEvent.Type.MouseButtonPress));
    	assertEquals("CursorChange", testEvent2(QEvent.Type.CursorChange));
    	checkSwitches();
    	QEvent.Type type = QEvent.Type.resolve(1001);
    	assertEquals("Default", testEvent(type));
    	assertEquals("Default", testEvent2(type));
    	checkSwitches();
    }
	
	private void checkSwitches() {
		try {
			for(Field field : TestJDK17FeatureSwitch.class.getDeclaredFields()) {
				if(field.isSynthetic()) {
					System.out.println(field + " = " + Arrays.toString((int[])field.get(null)));
				}
			}
		} catch (Throwable e1) {
		}
		for(int i=1;;++i) {
    		try {
				Class<?> syntheticClass = Class.forName(TestJDK17FeatureSwitch.class.getName()+"$"+i, false, TestJDK17FeatureSwitch.class.getClassLoader());
				if(syntheticClass.isSynthetic()) {
					for(Field declaredField : syntheticClass.getDeclaredFields()) {
						if(Modifier.isStatic(declaredField.getModifiers()) 
								&& declaredField.getType()==int[].class
								&& declaredField.getName().startsWith("$SwitchMap$")) {
							System.out.println(declaredField + " = " + Arrays.toString((int[])declaredField.get(null)));
						}
					}
				}
			} catch (Throwable e1) {
				break;
			}
    	}
	}
    
    private String testEvent(QEvent.Type type) {
    	return switch (type) {
		case MouseButtonPress -> "MouseButtonPress";
		case MouseButtonRelease -> "MouseButtonRelease";
		default -> "Default";
		};
    }
    
    private String testEvent2(QEvent.Type type) {
    	switch (type) {
		case KeyRelease:
			return "KeyRelease";
		case KeyPress:
			return "KeyPress";
		case CursorChange:
			return "CursorChange";
		default:
			return "Default";
		}
    }
    
    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestJDK17FeatureSwitch.class.getName());
    }
}
