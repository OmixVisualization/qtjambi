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
package io.qt.core;

import java.io.*;
import java.lang.invoke.*;
import java.lang.reflect.*;
import java.util.*;
import java.util.function.*;

import io.qt.*;

/**
 * Abstract superclass of containers in Qt.
 */
abstract class AbstractContainer<T> extends QtObject implements Cloneable{
    static {
    	QtJambi_LibraryUtilities.initialize();
    }

	@NativeAccess
	Object __rcContainer;
	
	/**
     * Returns the number of elements in this container. If this container
     * contains more than {@code Integer.MAX_VALUE} elements, returns
     * {@code Integer.MAX_VALUE}.
     *
     * @return the number of elements in this container
     */
    @QtUninvokable
	public abstract int size();

    /**
     * Returns {@code true} if this container contains no elements.
     *
     * @return {@code true} if this container contains no elements
     */
    @QtUninvokable
	public boolean isEmpty() {
    	return size()>0;
    }
	
    /**
     * Provides a constant C++ iterator to the container's begin.
     * @return begin
     */
    @QtUninvokable
    protected abstract AbstractIterator<T,? extends AbstractContainer<T>> constBegin();

    /**
     * Provides a constant C++ iterator to the container's end.
     * @return end
     */
    @QtUninvokable
	protected abstract AbstractIterator<T,? extends AbstractContainer<T>> constEnd();

	/**
     * {@inheritDoc}
	 */
    AbstractContainer(QPrivateConstructor p) {
		super(p);
	}
    
    /**
     * <p>Creates and returns a copy of this container.</p>
     */
    @Override
    public abstract AbstractContainer<T> clone();
}

/**
 * @hidden
 */
class CoreUtility extends io.qt.internal.CoreUtility {
    static {
        QtJambi_LibraryUtilities.initialize();
    }

    protected static abstract class AbstractSignal extends io.qt.internal.CoreUtility.AbstractSignal {
        AbstractSignal() {
            super();
        }

        AbstractSignal(Consumer<Object[]> argumentTest) {
            super(argumentTest);
        }

        AbstractSignal(Class<?> declaringClass) {
            super(declaringClass);
        }

        AbstractSignal(Class<?> declaringClass, boolean isDisposed) {
            super(declaringClass, isDisposed);
        }

        AbstractSignal(@StrictNonNull String signalName, Class<?>[] types) {
            super(signalName, types);
        }
    }

    protected static abstract class AbstractMultiSignal<S extends AbstractSignal>
            extends io.qt.internal.CoreUtility.AbstractMultiSignal<S> {
        AbstractMultiSignal() {
            super();
        }
    }

    protected static void checkConnectionToDisposedSignal(QMetaObject.DisposedSignal signal, Object receiver,
            boolean slotObject) {
        io.qt.internal.CoreUtility.checkConnectionToDisposedSignal(signal, receiver, slotObject);
    }
    
    protected static QMetaMethod signalMethod(io.qt.internal.CoreUtility.AbstractSignal signal) {
        return io.qt.internal.CoreUtility.signalMethod(signal);
    }
    
    protected static String internalNameOfArgumentType(Class<? extends Object> cls) {
    	return io.qt.internal.CoreUtility.internalNameOfArgumentType(cls);
    }

    protected static String internalTypeNameOfClass(Class<? extends Object> cls, Type genericType, AnnotatedElement annotatedType) {
        return io.qt.internal.CoreUtility.internalTypeNameOfClass(cls, genericType, annotatedType);
    }

    protected static String internalTypeName(String s, ClassLoader classLoader) {
        return io.qt.internal.CoreUtility.internalTypeName(s, classLoader);
    }
    
    protected static void addClassPath(String path) {
        io.qt.internal.CoreUtility.addClassPath(path);
    }

    protected static void removeClassPath(String path) {
        io.qt.internal.CoreUtility.removeClassPath(path);
    }

    protected static void addClassPath(java.net.URL path) {
        io.qt.internal.CoreUtility.addClassPath(path);
    }
    
    protected static QMetaType[] findSuperInstantiations(Class<?> clazz){
    	return io.qt.internal.CoreUtility.findSuperInstantiations(clazz);
    }
    
    protected static <T> void registerDataStreamOperators(int metaType, Class<?> classType, java.util.function.BiConsumer<QDataStream, T> datastreamInFn, java.util.function.Function<QDataStream, T> datastreamOutFn){
    	io.qt.internal.CoreUtility.registerDataStreamOperators(metaType, classType, datastreamInFn, datastreamOutFn);
    }
    
    protected static <T> void registerDebugStreamOperator(int metaType, Class<?> classType, java.util.function.BiConsumer<QDebug, T> debugstreamFn) {
    	io.qt.internal.CoreUtility.registerDebugStreamOperator(metaType, classType, debugstreamFn);
	}
    
    protected static boolean registerConverter(int metaType1, Class<?> classType1, int metaType2, Class<?> classType2, java.util.function.Function<?,?> converterFn) {
    	return io.qt.internal.CoreUtility.registerConverter(metaType1, classType1, metaType2, classType2, converterFn);
    }
	protected static Object invokeInterfaceDefaultMethod(Method method, Object object, Object... args) throws Throwable {
		return io.qt.internal.CoreUtility.invokeInterfaceDefaultMethod(method, object, args);
	}
	protected static MethodHandle getMethodHandle(Method method) throws IllegalAccessException {
		return io.qt.internal.CoreUtility.getMethodHandle(method);
	}
	
	protected static void emitNativeSignal(QObject sender, int methodIndex, long metaObjectId, Object... args) {
        io.qt.internal.CoreUtility.emitNativeSignal(sender, methodIndex, metaObjectId, args);
    }
 
    protected static boolean disconnectAll(QtSignalEmitterInterface sender, Object receiver) {
        return io.qt.internal.CoreUtility.disconnectAll(sender, receiver);
    }
 
    protected static boolean disconnectOne(QMetaObject.Connection connection) {
        return io.qt.internal.CoreUtility.disconnectOne(connection);
    }
 
    protected static void registerPropertyField(QMetaProperty metaProperty, java.lang.reflect.Field field) {
        io.qt.internal.CoreUtility.registerPropertyField(metaProperty, field);
    }
    
    protected static <PI> PI analyzeProperty(QObject containingObject, QtObject property, BiFunction<Field, QMetaType, PI> fun1, BiFunction<Field, QMetaProperty, PI> fun2) {
		return io.qt.internal.CoreUtility.analyzeProperty(containingObject, property, fun1, fun2);
	}
    
    protected static <A,B> Function<A,B> functionFromMethod(Method method){
        return io.qt.internal.CoreUtility.functionFromMethod(method);
    }
    
    protected static QMetaMethod fromMethod(java.io.Serializable method) {
    	return io.qt.internal.CoreUtility.fromMethod(method);
    }
    
    protected static <S extends Serializable, Bindable> @NonNull Bindable fromProperty(Class<S> type, S propertyGetter, BiFunction<QObject, QMetaProperty, Bindable> constr){
    	return io.qt.internal.CoreUtility.fromProperty(type, propertyGetter, constr);
    }
    
    protected static <L> L lambdaInfo(Serializable slotObject, QMetaObject.Method8<Object,QObject,QMetaObject,Integer,Integer,Boolean,List<Object>,java.lang.reflect.Method,L> constructor) {
    	return io.qt.internal.CoreUtility.lambdaInfo(slotObject, constructor);
    }
    
    protected static <L> L lambdaInfo(Serializable slotObject, Object object, QMetaObject.Method8<Object,QObject,QMetaObject,Integer,Integer,Boolean,List<Object>,java.lang.reflect.Method,L> constructor) {
    	return io.qt.internal.CoreUtility.lambdaInfo(slotObject, object, constructor);
    }
    
    protected static <L> L lambdaInfo(Serializable slotObject, QObject object, QMetaObject.Method8<Object,QObject,QMetaObject,Integer,Integer,Boolean,List<Object>,java.lang.reflect.Method,L> constructor) {
    	return io.qt.internal.CoreUtility.lambdaInfo(slotObject, object, constructor);
    }
}
