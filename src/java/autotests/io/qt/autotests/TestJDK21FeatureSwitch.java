/****************************************************************************
**
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

import static org.junit.Assert.assertEquals;

import java.lang.reflect.Field;
import java.lang.reflect.Modifier;
import java.util.Arrays;

import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.core.QEvent;
import io.qt.core.QMetaObject;

public class TestJDK21FeatureSwitch extends ApplicationInitializer {
	
	@BeforeClass
	public static void testInitialize() throws Exception {
		ApplicationInitializer.testInitializeWithWidgets();
		QMetaObject.forType(TestJDK21FeatureSwitch.class);
    }
    
	@Test
    public void test() {
    	assertEquals("MouseButtonPress", testNullableEvent(QEvent.Type.MouseButtonPress));
    	assertEquals("MouseButtonPress", testObject(QEvent.Type.MouseButtonPress));
    	checkSwitches();
    	QEvent.Type type = QEvent.Type.resolve(1001);
    	try {
			assertEquals("Default", testNullableEvent(type));
		} catch (ArrayIndexOutOfBoundsException e) {}
    	assertEquals("Default", testObject(type));
    	assertEquals("null", testNullableEvent(null));
    	assertEquals("null", testObject(null));
    	checkSwitches();
    }
	
	private void checkSwitches() {
		try {
//			for(var cls : TestJDK19FeatureSwitch.class.getDeclaredClasses()) {
//				System.out.println(cls);
//			}
//			for(var cls : TestJDK19FeatureSwitch.class.getNestMembers()) {
//				System.out.println(cls);
//			}
//			for(var mtd : TestJDK19FeatureSwitch.class.getDeclaredMethods()) {
//				if(mtd.isSynthetic()) {
//					System.out.println(mtd);
//				}
//			}
			for(Field field : TestJDK21FeatureSwitch.class.getDeclaredFields()) {
				if(field.isSynthetic()) {
					System.out.println(field + " = " + Arrays.toString((int[])field.get(null)));
				}
			}
		} catch (Throwable e1) {
		}
		for(int i=1;;++i) {
    		try {
				Class<?> syntheticClass = Class.forName(TestJDK21FeatureSwitch.class.getName()+"$"+i, false, TestJDK21FeatureSwitch.class.getClassLoader());
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
    
    private String testNullableEvent(QEvent.Type type) {
    	return switch (type) {
    	case null -> "null";
		case MouseButtonPress -> """
				MouseButtonPress""";
		case MouseButtonRelease -> "MouseButtonRelease";
		default -> {yield "Default";}
		};
    }
    
    private String testObject(Object type) {
    	if(type instanceof QEvent.Type t) {
    		System.out.println(t);
    	}
    	return switch (type) {
    	case null -> "null";
		case QEvent.Type t when t==QEvent.Type.MouseButtonPress -> "MouseButtonPress";
		case QEvent.Type t -> 
							switch(t) { case MouseButtonRelease -> "MouseButtonRelease";
								default -> "Default";
							};
		default -> "Unknown";
		};
    }
    
    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestJDK21FeatureSwitch.class.getName());
    }
}
