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

import java.lang.reflect.AnnotatedElement;
import java.lang.reflect.Constructor;
import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.lang.reflect.Parameter;
import java.lang.reflect.Type;
import java.util.Collection;
import java.util.List;
import java.util.Map;
import java.util.function.BiFunction;
import java.util.function.Function;
import java.util.function.Supplier;

import io.qt.InternalAccess;
import io.qt.NativeAccess;
import io.qt.QtByteEnumerator;
import io.qt.QtEnumerator;
import io.qt.QtLongEnumerator;
import io.qt.QtObject;
import io.qt.QtObjectInterface;
import io.qt.QtShortEnumerator;
import io.qt.core.QMetaObject.Method3;
import io.qt.core.QMetaObject.Method4;
import io.qt.core.QMetaObject.Method5;
import io.qt.core.QObject;

/**
 * @hidden
 */
final class AccessUtility implements io.qt.InternalAccess{

	private AccessUtility(){}
	
	@NativeAccess
	private final static AccessUtility instance = new AccessUtility();
	
	@Override
	public int registerMetaType(Class<?> clazz, Type genericType, AnnotatedElement annotatedType, boolean isPointer, boolean isReference) {
		return MetaTypeUtility.registerMetaType(clazz, genericType, annotatedType, isPointer, isReference);
	}
	
	@Override
	public int findMetaType(String name) {
		return MetaTypeUtility.findMetaType(name);
	}


	/**
	 * This is an internal function. Calling it can have unexpected results.
	 *
	 * Disables garbage collection for this object. This should be used when objects
	 * created in java are passed to C++ functions that take ownership of the
	 * objects. Both the Java and C++ part of the object will then be cleaned up by
	 * C++.
	 */
	@Override
	public void setCppOwnership(QtObjectInterface object) {
		NativeUtility.setCppOwnership(object);
	}

	/**
	 * This is an internal function. Calling it can have unexpected results.
	 *
	 * Forces Java ownership of both the Java object and its C++ resources. The C++
	 * resources will be cleaned up when the Java object is finalized.
	 */
	@Override
	public void setJavaOwnership(QtObjectInterface object) {
		NativeUtility.setJavaOwnership(object);
	}

	/**
	 * This is an internal function. Calling it can have unexpected results.
	 *
	 * Reenables garbage collection for this object. Should be used on objects for
	 * which disableGarbageCollection() has previously been called. After calling
	 * this function, the object ownership will be reset to default.
	 */
	@Override
	public void setDefaultOwnership(QtObjectInterface object) {
		NativeUtility.setDefaultOwnership(object);
	}

	/**
	 * This is an internal function. Calling it can have unexpected results.
	 *
	 * Disables garbage collection for this object. This should be used when objects
	 * created in java are passed to C++ functions that take ownership of the
	 * objects. Both the Java and C++ part of the object will then be cleaned up by
	 * C++.
	 */
	@Override
	public void setCppOwnership(QtObject object) {
		NativeUtility.setCppOwnership(object);
	}

	/**
	 * This is an internal function. Calling it can have unexpected results.
	 *
	 * Forces Java ownership of both the Java object and its C++ resources. The C++
	 * resources will be cleaned up when the Java object is finalized.
	 */
	@Override
	public void setJavaOwnership(QtObject object) {
		NativeUtility.setJavaOwnership(object);
	}

	/**
	 * This is an internal function. Calling it can have unexpected results.
	 *
	 * Reenables garbage collection for this object. Should be used on objects for
	 * which disableGarbageCollection() has previously been called. After calling
	 * this function, the object ownership will be reset to default.
	 */
	@Override
	public void setDefaultOwnership(QtObject object) {
		NativeUtility.setDefaultOwnership(object);
	}

	@Override
	public void invalidateObject(QtObjectInterface object) {
		NativeUtility.invalidateObject(object);
	}

	@Override
	public void invalidateObject(QtObject object) {
		NativeUtility.invalidateObject(object);
	}

	@Override
	public boolean isJavaOwnership(QtObject object) {
		return NativeUtility.isJavaOwnership(object);
	}

	@Override
	public boolean isJavaOwnership(QtObjectInterface object) {
		return NativeUtility.isJavaOwnership(object);
	}

	@Override
	public boolean isSplitOwnership(QtObject object) {
		return NativeUtility.isSplitOwnership(object);
	}

	@Override
	public boolean isSplitOwnership(QtObjectInterface object) {
		return NativeUtility.isSplitOwnership(object);
	}

	@Override
	public boolean isCppOwnership(QtObject object) {
		return NativeUtility.isCppOwnership(object);
	}

	@Override
	public boolean isCppOwnership(QtObjectInterface object) {
		return NativeUtility.isCppOwnership(object);
	}

	@Override
	public long nativeId(QtObject object) {
		return NativeUtility.nativeId(object);
	}

	@Override
	public long nativeId(QtObjectInterface object) {
		return NativeUtility.nativeId(object);
	}

	@Override
	public long checkedNativeId(QtObject object) {
		return NativeUtility.checkedNativeId(object);
	}

	@Override
	public long checkedNativeId(QtObjectInterface object) {
		return NativeUtility.checkedNativeId(object);
	}
	
	@Override
	public java.nio.ByteBuffer mutableData(io.qt.core.QByteArray byteArray){
		return NativeUtility.mutableData(byteArray);
	}

	@Override
	public Supplier<Class<?>> callerClassProvider() {
		return RetroHelper.callerClassProvider();
	}

	@Override
	public void registerDependentObject(QtObjectInterface dependentObject, QtObjectInterface owner) {
		NativeUtility.registerDependentObject(dependentObject, owner);
	}

	public void registerDependentObject(QtObject dependentObject, QtObjectInterface owner) {
		NativeUtility.registerDependentObject(dependentObject, owner);
	}

	@Override
	public void registerDependentObject(QtObjectInterface dependentObject, QtObject owner) {
		NativeUtility.registerDependentObject(dependentObject, owner);
	}

	@Override
	public void registerDependentObject(QtObject dependentObject, QtObject owner) {
		NativeUtility.registerDependentObject(dependentObject, owner);
	}

	@Override
	public void unregisterDependentObject(QtObjectInterface dependentObject, QtObjectInterface owner) {
		NativeUtility.unregisterDependentObject(dependentObject, owner);
	}

	@Override
	public void unregisterDependentObject(QtObject dependentObject, QtObjectInterface owner) {
		NativeUtility.unregisterDependentObject(dependentObject, owner);
	}

	@Override
	public void unregisterDependentObject(QtObjectInterface dependentObject, QtObject owner) {
		NativeUtility.unregisterDependentObject(dependentObject, owner);
	}

	@Override
	public void unregisterDependentObject(QtObject dependentObject, QtObject owner) {
		NativeUtility.unregisterDependentObject(dependentObject, owner);
	}

	@Override
	public InternalAccess.Cleanable registerCleaner(Object object, Runnable action) {
		return NativeUtility.registerCleaner(object, action);
	}

	@Override
	public <S extends java.io.Serializable> io.qt.core.QObject lambdaContext(S lambdaExpression) {
		return ClassAnalyzerUtility.lambdaContext(lambdaExpression);
	}

	@Override
	public <S extends java.io.Serializable> Class<?> lambdaReturnType(Class<S> type, S lambdaExpression) {
		return ClassAnalyzerUtility.lambdaReturnType(type, lambdaExpression);
	}
	
	@Override
	public <S extends java.io.Serializable> int[] lambdaMetaTypes(Class<S> type, S lambdaExpression) {
		return ClassAnalyzerUtility.lambdaMetaTypes(type, lambdaExpression);
	}
	
	@Override
	public <S extends java.io.Serializable> Class<?>[] lambdaClassTypes(Class<S> type, S lambdaExpression) {
		return ClassAnalyzerUtility.lambdaClassTypes(type, lambdaExpression);
	}
	
	public <S extends java.io.Serializable> java.lang.reflect.Executable lambdaExecutable(S lambdaExpression){
		return ClassAnalyzerUtility.lambdaExecutable(lambdaExpression);
	}
	
	@Override
	public Class<?> findGeneratedSuperclass(Class<?> clazz){
		return ClassAnalyzerUtility.findGeneratedSuperclass(clazz);
	}
	
	@Override
	public boolean isGeneratedClass(Class<?> clazz) {
		return ClassAnalyzerUtility.isGeneratedClass(clazz);
	}

	@Override
	public Supplier<CallerContext> callerContextProvider() {
		return RetroHelper.callerContextProvider();
	}
	
	@Override
	public QObject owner(QtObjectInterface object) {
		return NativeUtility.owner(object);
	}

	@Override
	public boolean hasOwnerFunction(QtObjectInterface object) {
		return NativeUtility.hasOwnerFunction(object);
	}
	
	@Override
	public QObject owner(QtObject object) {
		return NativeUtility.owner(object);
	}

	@Override
	public boolean hasOwnerFunction(QtObject object) {
		return NativeUtility.hasOwnerFunction(object);
	}
	
	@Override
	public <Q extends QtObjectInterface,M> M findMemberAccess(Q ifc, Class<Q> interfaceClass, Class<M> accessClass) {
		NativeUtility.NativeLink link = NativeUtility.findInterfaceLink(ifc, true);
		return accessClass.cast(link.getMemberAccess(interfaceClass));
	}

	@Override
	public <T> Class<T> getClass(T object) {
		return ClassAnalyzerUtility.getClass(object);
	}

	@Override
	public int registerMetaType(Parameter parameter) {
		return MetaTypeUtility.registerMetaType(parameter);
	}

	@Override
	public <T> Supplier<T> getFactory0(Constructor<T> constructor) {
		return ReflectionUtility.getFactory0(constructor);
	}

	@Override
	public <A, T> Function<A, T> getFactory1(Constructor<T> constructor) {
		return ReflectionUtility.getFactory1(constructor);
	}

	@Override
	public <A, B, T> BiFunction<A, B, T> getFactory2(Constructor<T> constructor) {
		return ReflectionUtility.getFactory2(constructor);
	}

	@Override
	public <A, B, C, T> Method3<A, B, C, T> getFactory3(Constructor<T> constructor) {
		return ReflectionUtility.getFactory3(constructor);
	}

	@Override
	public <A, B, C, D, T> Method4<A, B, C, D, T> getFactory4(Constructor<T> constructor) {
		return ReflectionUtility.getFactory4(constructor);
	}

	@Override
	public <A, B, C, D, E, T> Method5<A, B, C, D, E, T> getFactory5(Constructor<T> constructor) {
		return ReflectionUtility.getFactory5(constructor);
	}

	@Override
	public <T> T invokeContructor(Constructor<T> constructor, Object... args) throws Throwable {
		return ReflectionUtility.invokeContructor(constructor, args);
	}

	@Override
	public Object invokeMethod(Method method, Object object, Object... args) throws Throwable {
		return ReflectionUtility.invokeMethod(method, object, args);
	}

	@Override
	public Package getDefinedPackage(ClassLoader cl, String pkg) {
		return RetroHelper.getDefinedPackage(cl, pkg);
	}
	
	@Override
	public void setQmlClassInfoGeneratorFunction(Function<Class<?>, Map<String, String>> qmlClassInfogeneratorFunction) {
		MetaObjectUtility.setQmlClassInfoGeneratorFunction(qmlClassInfogeneratorFunction);
	}

	@Override
	public Class<?> toClass(Type type) {
		return ClassAnalyzerUtility.toClass(type);
	}

	@Override
	public <E extends Enum<E> & QtEnumerator> E resolveEnum(Class<E> cl, int value, String name) {
		return EnumUtility.resolveEnum(cl, value, name);
	}

	@Override
	public <E extends Enum<E> & QtByteEnumerator> E resolveEnum(Class<E> cl, byte value, String name) {
		return EnumUtility.resolveEnum(cl, value, name);
	}

	@Override
	public <E extends Enum<E> & QtShortEnumerator> E resolveEnum(Class<E> cl, short value, String name) {
		return EnumUtility.resolveEnum(cl, value, name);
	}

	@Override
	public <E extends Enum<E> & QtLongEnumerator> E resolveEnum(Class<E> cl, long value, String name) {
		return EnumUtility.resolveEnum(cl, value, name);
	}

	@Override
	public void removeFromMapReferenceCount(QtObjectInterface owner,
			Class<? extends QtObjectInterface> declaringClass, String fieldName, boolean isStatic, Object value) {
		ReferenceUtility.removeFromMapReferenceCount(owner, declaringClass, fieldName, isStatic, value);
	}

	@Override
	public void removeFromCollectionReferenceCount(QtObjectInterface owner,
			Class<? extends QtObjectInterface> declaringClass, String fieldName, boolean isStatic, Object value) {
		ReferenceUtility.removeFromCollectionReferenceCount(owner, declaringClass, fieldName, isStatic, value);
	}

	@Override
	public void addAllReferenceCount(QtObjectInterface owner, Class<? extends QtObjectInterface> declaringClass,
			String fieldName, boolean isThreadSafe, boolean isStatic, Collection<?> values) {
		ReferenceUtility.addAllReferenceCount(owner, declaringClass, fieldName, isThreadSafe, isStatic, values);
	}

	@Override
	public void putReferenceCount(QtObjectInterface owner, Class<? extends QtObjectInterface> declaringClass,
			String fieldName, boolean isThreadSafe, boolean isStatic, Object key, Object value) {
		ReferenceUtility.putReferenceCount(owner, declaringClass, fieldName, isThreadSafe, isStatic, key, value);
	}

	@Override
	public void clearReferenceCount(QtObjectInterface owner, Class<? extends QtObjectInterface> declaringClass,
			String fieldName, boolean isStatic) {
		ReferenceUtility.clearReferenceCount(owner, declaringClass, fieldName, isStatic);
		
	}

	@Override
	public Map<Object, Object> newRCMap() {
		return ReferenceUtility.newRCMap();
	}

	@Override
	public List<Object> newRCList() {
		return ReferenceUtility.newRCList();
	}

	@Override
	public void addReferenceCount(QtObjectInterface owner, Class<? extends QtObjectInterface> declaringClass,
			String fieldName, boolean isThreadSafe, boolean isStatic, Object value) {
		ReferenceUtility.addReferenceCount(owner, declaringClass, fieldName, isThreadSafe, isStatic, value);
	}

	@Override
	public void setReferenceCount(QtObjectInterface owner, Class<? extends QtObjectInterface> declaringClass,
			String fieldName, boolean threadSafe, boolean isStatic, Object newValue) {
		ReferenceUtility.setReferenceCount(owner, declaringClass, fieldName, threadSafe, isStatic, newValue);
	}

	@Override
	public Object readField(Object owner, Field f) {
		return ReflectionUtility.readField(owner, f);
	}

	@Override
	public <V> V readField(Object owner, Class<?> declaringClass, String fieldName, Class<V> fieldType) {
		return ReflectionUtility.readField(owner, declaringClass, fieldName, fieldType);
	}

	@Override
	public void writeField(Object owner, Field f, Object newValue) {
		ReflectionUtility.writeField(owner, f, newValue);
	}

	@Override
	public void writeField(Object owner, Class<?> declaringClass, String fieldName, Object newValue) {
		ReflectionUtility.writeField(owner, declaringClass, fieldName, newValue);
	}
}
