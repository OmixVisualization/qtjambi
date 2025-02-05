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
package io.qt;

import java.io.Serializable;
import java.lang.invoke.MethodType;
import java.lang.invoke.SerializedLambda;
import java.lang.reflect.AnnotatedElement;
import java.lang.reflect.Constructor;
import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.lang.reflect.Parameter;
import java.lang.reflect.Type;
import java.util.Collection;
import java.util.Map;
import java.util.function.BiFunction;
import java.util.function.Function;
import java.util.function.Supplier;

import io.qt.core.QMetaObject;
import io.qt.core.QObject;

/**
 * @hidden
 */
public interface InternalAccess {
	
	<E extends Enum<E> & io.qt.QtEnumerator> E resolveEnum(Class<E> cl, int value, String name);
	
	<E extends Enum<E> & io.qt.QtByteEnumerator> E resolveEnum(Class<E> cl, byte value, String name);
	
	<E extends Enum<E> & io.qt.QtShortEnumerator> E resolveEnum(Class<E> cl, short value, String name);
	
	<E extends Enum<E> & io.qt.QtLongEnumerator> E resolveEnum(Class<E> cl, long value, String name);
	
	<T extends QtAbstractFlagEnumerator> T[] flagConstants(QFlags<T> flags);
	
	QFlags<?> asFlags(QtAbstractFlagEnumerator flag);
	
	boolean isSmallEnum(QtAbstractFlagEnumerator enm);

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
    
    Object findAssociation(Object object);
    
    interface NativeIdInfo{
    	long nativeId();
    	boolean needsReferenceCounting();
    }
    
    NativeIdInfo checkedNativeIdInfo(io.qt.QtObject object);
    
    NativeIdInfo checkedNativeIdInfo(io.qt.QtObjectInterface object);
    
    boolean needsReferenceCounting(io.qt.QtObject object);
    
    boolean needsReferenceCounting(io.qt.QtObjectInterface object);
    
    java.nio.ByteBuffer mutableData(io.qt.core.QByteArray byteArray);
    
    java.nio.CharBuffer mutableData(io.qt.core.QString string);
    
    <C extends QtObject & Iterable<Character>> java.nio.CharBuffer mutableDataC(C list);
    
    <C extends QtObject & Iterable<Byte>> java.nio.ByteBuffer mutableDataB(C list);
    
    <C extends QtObject & Iterable<Short>> java.nio.ShortBuffer mutableDataS(C list);
    
    <C extends QtObject & Iterable<Integer>> java.nio.IntBuffer mutableDataI(C list);
    
    <C extends QtObject & Iterable<Long>> java.nio.LongBuffer mutableDataJ(C list);
    
    <C extends QtObject & Iterable<Float>> java.nio.FloatBuffer mutableDataF(C list);
    
    <C extends QtObject & Iterable<Double>> java.nio.DoubleBuffer mutableDataD(C list);
    
    void truncateBuffer(io.qt.QtObject owner, java.nio.Buffer buffer);
    
    void truncateBuffer(io.qt.QtObjectInterface owner, java.nio.Buffer buffer);

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
    
    /**
     * @hidden
     */
    public interface Cleanable {
        void clean();
        void dismiss();
    }
    
    Cleanable registerCleaner(Object object, Runnable action);
    
    <S extends java.io.Serializable> io.qt.core.QObject lambdaContext(S lambdaExpression);
    
    <S extends java.io.Serializable> Class<?> lambdaReturnType(Class<S> type, S lambdaExpression);
    
    <S extends java.io.Serializable> int[] lambdaMetaTypes(Class<S> type, S lambdaExpression);
    
    <S extends java.io.Serializable> Class<?>[] lambdaClassTypes(Class<S> type, S lambdaExpression);
    
    <S extends java.io.Serializable> java.lang.reflect.Executable lambdaExecutable(S lambdaExpression);
    
    <T> Class<T> getClass(T object);
    
    Object readField(Object owner, Field f);
    
    <V> V readField(Object owner, Class<?> declaringClass, String fieldName, Class<V> fieldType);
    
    void writeField(Object owner, Field f, Object newValue);
    
    void writeField(Object owner, Class<?> declaringClass, String fieldName, Object newValue);
    
    int registerMetaType(Parameter parameter);
    
    int findMetaType(String name);
    
    Class<?> toClass(Type type);
    
    Class<?> findGeneratedSuperclass(Class<?> clazz);
    
    boolean isGeneratedClass(Class<?> clazz);
    
    int registerMetaType(Class<?> clazz, Type genericType, AnnotatedElement annotatedType, boolean isPointer, boolean isReference);
    
    /**
     * @hidden
     */
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
    
    <Q extends QtObjectInterface,M extends io.qt.MemberAccess<Q>> M findMemberAccess(Q ifc, Class<Q> interfaceClass, Class<M> accessClass);
    
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
    
    SerializedLambda serializeLambdaExpression(Serializable slotObject);
    
    boolean useAnnotatedType();
}
