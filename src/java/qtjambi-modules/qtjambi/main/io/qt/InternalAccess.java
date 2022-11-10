/****************************************************************************
**
** Copyright (C) 2009-2022 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
package io.qt;

import java.lang.invoke.MethodType;
import java.lang.reflect.Constructor;
import java.lang.reflect.Method;
import java.lang.reflect.Parameter;
import java.util.Collection;
import java.util.Map;
import java.util.function.BiFunction;
import java.util.function.Function;
import java.util.function.Supplier;

import io.qt.core.QMetaObject;
import io.qt.core.QObject;

public interface InternalAccess {
	
	<E extends Enum<E> & io.qt.QtEnumerator> E resolveEnum(Class<E> cl, int value, String name);
	
	<E extends Enum<E> & io.qt.QtByteEnumerator> E resolveEnum(Class<E> cl, byte value, String name);
	
	<E extends Enum<E> & io.qt.QtShortEnumerator> E resolveEnum(Class<E> cl, short value, String name);
	
	<E extends Enum<E> & io.qt.QtLongEnumerator> E resolveEnum(Class<E> cl, long value, String name);

    boolean isJavaOwnership(io.qt.QtObject object);

    boolean isJavaOwnership(io.qt.QtObjectInterface object);
    
    boolean isSplitOwnership(io.qt.QtObject object);
    
    boolean isSplitOwnership(io.qt.QtObjectInterface object);
    
    boolean isCppOwnership(io.qt.QtObject object);
    
    boolean isCppOwnership(io.qt.QtObjectInterface object);

    void setCppOwnership(io.qt.QtObject object);

    void setCppOwnership(io.qt.QtObjectInterface object);

    void setJavaOwnership(io.qt.QtObject object);

    void setJavaOwnership(io.qt.QtObjectInterface object);
    
    void setDefaultOwnership(io.qt.QtObject object);
    
    void setDefaultOwnership(io.qt.QtObjectInterface object);

    long nativeId(io.qt.QtObject object);

    long nativeId(io.qt.QtObjectInterface object);
    
    long checkedNativeId(io.qt.QtObject object);
    
    long checkedNativeId(io.qt.QtObjectInterface object);

    void removeFromMapReferenceCount(io.qt.QtObjectInterface owner, Class<? extends io.qt.QtObjectInterface> declaringClass, String fieldName, boolean isStatic, Object value);

    void removeFromCollectionReferenceCount(io.qt.QtObjectInterface owner, Class<? extends io.qt.QtObjectInterface> declaringClass, String fieldName, boolean isStatic, Object value);
    
    void addAllReferenceCount(io.qt.QtObjectInterface owner, Class<? extends io.qt.QtObjectInterface> declaringClass, String fieldName, boolean isThreadSafe, boolean isStatic, Collection<?> values);
    
    void putReferenceCount(io.qt.QtObjectInterface owner, Class<? extends io.qt.QtObjectInterface> declaringClass, String fieldName, boolean isThreadSafe, boolean isStatic, Object key, Object value);
    
    void clearReferenceCount(io.qt.QtObjectInterface owner, Class<? extends io.qt.QtObjectInterface> declaringClass, String fieldName, boolean isStatic);
    
    java.util.function.Supplier<Class<?>> callerClassProvider();

    java.util.Map<Object,Object> newRCMap();

    java.util.List<Object> newRCList();

    void addReferenceCount(io.qt.QtObjectInterface owner, Class<? extends io.qt.QtObjectInterface> declaringClass, String fieldName, boolean isThreadSafe, boolean isStatic, Object value);

    void registerDependentObject(io.qt.QtObject dependentObject, io.qt.QtObjectInterface owner);

    void registerDependentObject(io.qt.QtObjectInterface dependentObject, io.qt.QtObjectInterface owner);

	void registerDependentObject(QtObjectInterface dependentObject, QtObject owner);

	void registerDependentObject(QtObject dependentObject, QtObject owner);

	void unregisterDependentObject(QtObjectInterface dependentObject, QtObjectInterface owner);

	void unregisterDependentObject(QtObject dependentObject, QtObjectInterface owner);

	void unregisterDependentObject(QtObjectInterface dependentObject, QtObject owner);

	void unregisterDependentObject(QtObject dependentObject, QtObject owner);

    void setReferenceCount(io.qt.QtObjectInterface owner, Class<? extends io.qt.QtObjectInterface> declaringClass, String fieldName, boolean threadSafe, boolean isStatic, Object newValue);
    
    void invalidateObject(io.qt.QtObject object);
    
    void invalidateObject(io.qt.QtObjectInterface object);
    
    public interface Cleanable {
        void clean();
    }
    
    Cleanable registerCleaner(Object object, Runnable action);
    
    <S extends java.io.Serializable> io.qt.core.QObject lambdaContext(S lambdaExpression);
    
    <S extends java.io.Serializable> Class<?> lambdaReturnType(Class<S> type, S lambdaExpression);
    
    <S extends java.io.Serializable> int[] lambdaMetaTypes(Class<S> type, S lambdaExpression);
    
    <S extends java.io.Serializable> Class<?>[] lambdaClassTypes(Class<S> type, S lambdaExpression);
    
    <S extends java.io.Serializable> java.lang.reflect.Executable lambdaExecutable(S lambdaExpression);
    
    <T> Class<T> getClass(T object);
    
    int registerMetaType(Parameter parameter);
    
    Class<?> findGeneratedSuperclass(Class<?> clazz);
    
    public final class CallerContext{
        public CallerContext(Class<?> declaringClass, String methodName, String methodDescriptor, MethodType methodType, int lineNumber) {
            super();
            this.declaringClass = declaringClass;
            this.methodName = methodName;
            this.methodDescriptor = methodDescriptor;
            this.methodType = methodType;
            this.lineNumber = lineNumber;
        }

        public final Class<?> declaringClass;
        public final String methodName;
        public final String methodDescriptor;
        public final MethodType methodType;
        public final int lineNumber;
    }

    Supplier<CallerContext> callerContextProvider();
    
    boolean hasOwnerFunction(QtObject object);
    boolean hasOwnerFunction(QtObjectInterface object);
    
    public QObject owner(QtObject object);
    public QObject owner(QtObjectInterface object);
    
    <Q extends QtObjectInterface,M> M findMemberAccess(Q ifc, Class<Q> interfaceClass, Class<M> accessClass);
    
    <T> Supplier<T> getFactory0(Constructor<T> constructor);
    
    <A,T> Function<A,T> getFactory1(Constructor<T> constructor);
    
    <A,B,T> BiFunction<A,B,T> getFactory2(Constructor<T> constructor);
    
    <A,B,C,T> QMetaObject.Method3<A,B,C,T> getFactory3(Constructor<T> constructor);
    
    <A,B,C,D,T> QMetaObject.Method4<A,B,C,D,T> getFactory4(Constructor<T> constructor);
    
    <A,B,C,D,E,T> QMetaObject.Method5<A,B,C,D,E,T> getFactory5(Constructor<T> constructor);
    
    <T> T invokeContructor(Constructor<T> constructor, Object... args) throws Throwable;
    
    Object invokeMethod(Method method, Object object, Object... args) throws Throwable;
    
    Package getDefinedPackage(ClassLoader cl, String pkg);
    
    void setQmlClassInfoGeneratorFunction(Function<Class<?>, Map<String, String>> qmlClassInfogeneratorFunction);
}
