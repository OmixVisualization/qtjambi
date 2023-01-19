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
package io.qt.internal;

import java.lang.invoke.MethodHandle;
import java.lang.reflect.Method;
import java.net.URL;

import io.qt.QtObject;
import io.qt.QtSignalEmitterInterface;
import io.qt.core.QMetaObject;
import io.qt.core.QMetaProperty;
import io.qt.core.QObject;

public abstract class CoreUtility {
	protected CoreUtility() {throw new RuntimeException();}
	protected static abstract class AbstractSignal extends SignalUtility.AbstractSignal {
		protected AbstractSignal(){}
	    
		protected AbstractSignal(Class<?>[] types) {
            super(types);
        }
        
		protected AbstractSignal(Class<?> declaringClass, boolean isStatic) {
            super(declaringClass, isStatic);
        }
        
		protected AbstractSignal(String signalName, Class<?>[] types) {
            super(signalName, types);
        }
    }
    
    protected static abstract class AbstractMultiSignal<Signal extends AbstractSignal> extends SignalUtility.AbstractMultiSignal<Signal> {
    	protected AbstractMultiSignal() {
            super();
        }
    }
    
    protected static void checkConnectionToDisposedSignal(QMetaObject.DisposedSignal signal, Object receiver, boolean slotObject) {
        SignalUtility.checkConnectionToDisposedSignalImpl(signal, receiver, slotObject);
    }
    
    protected static <PI> PI analyzeProperty(QObject object, QtObject property, Class<PI> propertyInfoClass) {
    	return MetaObjectUtility.analyzeProperty(object, property, propertyInfoClass);
    }
    
    protected static void registerPropertyField(QMetaProperty metaProperty, java.lang.reflect.Field field) {
    	MetaObjectUtility.registerPropertyField(metaProperty, field);
    }
    
    protected static void emitNativeSignal(QObject sender, int methodIndex, long metaObjectId, Object args[]) {
    	SignalUtility.emitNativeSignal(sender, methodIndex, metaObjectId, 0, args);
    }
    
    protected static boolean disconnectAll(QtSignalEmitterInterface sender, Object receiver) {
    	return SignalUtility.disconnectAll(sender, receiver);
    }
    
    protected static boolean disconnectOne(QMetaObject.Connection connection) {
    	return SignalUtility.disconnectOne(connection);
    }
    
    protected static Object invokeInterfaceDefaultMethod(Method method, Object object, Object... args) throws Throwable {
		return ReflectionUtility.methodInvocationHandler.invokeInterfaceDefaultMethod(method, object, args);
	}
    
    protected static MethodHandle getMethodHandle(Method method) throws IllegalAccessException {
    	return ReflectionUtility.getMethodHandle(method);
    }
    
    protected static void addClassPath(String path) {
    	ResourceUtility.addSearchPath(path);
    }
    
    protected static void removeClassPath(String path) {
    	ResourceUtility.removeSearchPath(path);
    }
    
    protected static void addClassPath(URL path) {
    	ResourceUtility.addSearchPath(path);
    }
}
