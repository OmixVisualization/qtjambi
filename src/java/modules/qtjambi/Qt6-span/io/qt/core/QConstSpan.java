/****************************************************************************
**
** Copyright (C) 2009-2024 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import java.lang.reflect.Array;
import java.util.Collection;
import java.util.Collections;

import io.qt.NativeAccess;
import io.qt.NonNull;
import io.qt.Nullable;
import io.qt.QtObject;
import io.qt.QtPrimitiveType;
import io.qt.QtUninvokable;
import io.qt.StrictNonNull;

/**
 * <p>Java wrapper for Qt class <code><a href="https://doc.qt.io/qt/qspan.html">QSpan</a></code></p>
 */
public class QConstSpan<T> extends AbstractSpan<T>{

	QConstSpan(){
		super();
	}
	
	QConstSpan(QConstSpan<T> other){
		super(other, 0, 0);
	}
	
	@NativeAccess
    QConstSpan(QPrivateConstructor c, QtObject owner) { 
		super(c, owner);
	}
	
	@NativeAccess
	QConstSpan(QPrivateConstructor c, QList<T> list){
		super(c, list);
	}
	
	QConstSpan(java.nio.Buffer buffer, char type) {
		super(buffer, type);
	}
	
	QConstSpan(boolean[] array) {
		super(array);
	}
	
	QConstSpan(byte[] array) {
		super(array);
	}

	QConstSpan(char[] array) {
		super(array);
	}

	QConstSpan(double[] array) {
		super(array);
	}

	QConstSpan(float[] array) {
		super(array);
	}

	QConstSpan(int[] array) {
		super(array);
	}

	QConstSpan(long[] array) {
		super(array);
	}

	QConstSpan(short[] array) {
		super(array);
	}
	
	QConstSpan(QConstSpan<T> other, int offset, int n){
		super(other, offset, n);
	}
	
	QConstSpan(QList<T> list){
		super(list);
	}
	
	QConstSpan(@StrictNonNull QByteArray byteArray){
		super(byteArray);
	}
	
	QConstSpan(@StrictNonNull QByteArrayView byteArray){
		super(byteArray.data(), byteArray::clone);
	}
	
	QConstSpan(@StrictNonNull QString string){
		super(string);
	}
	
	QConstSpan(Class<? super T> type, T[] array){
		super(type, array);
	}
	
	QConstSpan(QMetaType metaType, T[] array){
		super(metaType, array);
	}
	
	public static @NonNull QConstSpan<@NonNull@QtPrimitiveType Byte> ofBuffer(java.nio.@StrictNonNull ByteBuffer buffer){
		return new QConstSpan<>(buffer, 'B');
	}
	
	public static @NonNull QConstSpan<@NonNull@QtPrimitiveType Short> ofBuffer(java.nio.@StrictNonNull ShortBuffer buffer){
		return new QConstSpan<>(buffer, 'S');
	}
	
	public static @NonNull QConstSpan<@NonNull@QtPrimitiveType Integer> ofBuffer(java.nio.@StrictNonNull IntBuffer buffer){
		return new QConstSpan<>(buffer, 'I');
	}
	
	public static @NonNull QConstSpan<@NonNull@QtPrimitiveType Long> ofBuffer(java.nio.@StrictNonNull LongBuffer buffer){
		return new QConstSpan<>(buffer, 'J');
	}
	
	public static @NonNull QConstSpan<@NonNull@QtPrimitiveType Character> ofBuffer(java.nio.@StrictNonNull CharBuffer buffer){
		return new QConstSpan<>(buffer, 'C');
	}
	
	public static @NonNull QConstSpan<@NonNull@QtPrimitiveType Float> ofBuffer(java.nio.@StrictNonNull FloatBuffer buffer){
		return new QConstSpan<>(buffer, 'F');
	}
	
	public static @NonNull QConstSpan<@NonNull@QtPrimitiveType Double> ofBuffer(java.nio.@StrictNonNull DoubleBuffer buffer){
		return new QConstSpan<>(buffer, 'D');
	}
	
	public static <T> @NonNull QConstSpan<T> ofList(@StrictNonNull Collection<T> list){
		if(list instanceof QList)
			return new QConstSpan<>((QList<T>)list);
		else if(list.isEmpty())
			return new QConstSpan<>();
		else
			return new QConstSpan<>(null, new QList<T>(list));
	}
	
	public static <T> @NonNull QConstSpan<T> of(){
		return new QConstSpan<>();
	}
	
	@SafeVarargs
	public static <T> @NonNull QConstSpan<T> of(T element0, T @StrictNonNull...elements){
		return new QConstSpan<>(null, QList.of(element0, elements));
	}
	
	@SafeVarargs
	public static <T> @NonNull QConstSpan<T> ofTyped(@StrictNonNull QMetaType metaType, T @StrictNonNull...elements){
		return new QConstSpan<>(null, QList.ofTyped(metaType, elements));
	}
	
	@SafeVarargs
	public static <T> @NonNull QConstSpan<T> ofTyped(@Nullable Class<? super T> type, T @StrictNonNull...elements){
		return new QConstSpan<>(null, QList.ofTyped(type, elements));
	}
	
	public static @NonNull QConstSpan<@NonNull@QtPrimitiveType Byte> ofByte(byte... array){
		return new QConstSpan<>(array);
	}
	
	public static @NonNull QConstSpan<@NonNull@QtPrimitiveType Short> ofShort(short... array){
		return new QConstSpan<>(array);
	}
	
	public static @NonNull QConstSpan<@NonNull@QtPrimitiveType Integer> ofInt(int... array){
		return new QConstSpan<>(array);
	}
	
	public static @NonNull QConstSpan<@NonNull@QtPrimitiveType Long> ofLong(long... array){
		return new QConstSpan<>(array);
	}
	
	public static @NonNull QConstSpan<@NonNull@QtPrimitiveType Character> ofChar(char... array){
		return new QConstSpan<>(array);
	}
	
	public static @NonNull QConstSpan<@NonNull@QtPrimitiveType Float> ofFloat(float... array){
		return new QConstSpan<>(array);
	}
	
	public static @NonNull QConstSpan<@NonNull@QtPrimitiveType Double> ofDouble(double... array){
		return new QConstSpan<>(array);
	}
	
	public static @NonNull QConstSpan<@NonNull@QtPrimitiveType Boolean> ofBoolean(boolean... array){
		return new QConstSpan<>(array);
	}

	public static <T> @NonNull QConstSpan<T> ofList(@StrictNonNull QList<T> list){
		return new QConstSpan<>(list);
	}
	
	public static @NonNull QConstSpan<@NonNull@QtPrimitiveType Byte> ofByte(@StrictNonNull QByteArray array){
		return new QConstSpan<>(array);
	}
	
	public static @NonNull QConstSpan<@NonNull@QtPrimitiveType Byte> ofByte(@StrictNonNull QByteArrayView array){
		return new QConstSpan<>(array);
	}
	
	public static @NonNull QConstSpan<@NonNull@QtPrimitiveType Character> ofChar(@StrictNonNull QString string){
		return new QConstSpan<>(string);
	}
	
	public static @NonNull QConstSpan<@NonNull@QtPrimitiveType Character> ofChar(@StrictNonNull String string){
		return ofChar(string.toCharArray());
	}
	
	public static @NonNull QConstSpan<@NonNull@QtPrimitiveType Character> ofChar(@StrictNonNull CharSequence string){
		return ofChar(string.toString());
	}

	public static java.nio.@NonNull ByteBuffer asByteBuffer(@StrictNonNull QConstSpan<@NonNull@QtPrimitiveType Byte> span){
		return asBuffer(java.nio.ByteBuffer.class, span);
	}
	
	public static java.nio.@NonNull ShortBuffer asShortBuffer(@StrictNonNull QConstSpan<@NonNull@QtPrimitiveType Short> span){
		return asBuffer(java.nio.ShortBuffer.class, span);
	}
	
	public static java.nio.@NonNull IntBuffer asIntBuffer(@StrictNonNull QConstSpan<@NonNull@QtPrimitiveType Integer> span){
		return asBuffer(java.nio.IntBuffer.class, span);
	}
	
	public static java.nio.@NonNull LongBuffer asLongBuffer(@StrictNonNull QConstSpan<@NonNull@QtPrimitiveType Long> span){
		return asBuffer(java.nio.LongBuffer.class, span);
	}
	
	public static java.nio.@NonNull CharBuffer asCharBuffer(@StrictNonNull QConstSpan<@NonNull@QtPrimitiveType Character> span){
		return asBuffer(java.nio.CharBuffer.class, span);
	}
	
	public static java.nio.@NonNull FloatBuffer asFloatBuffer(@StrictNonNull QConstSpan<@NonNull@QtPrimitiveType Float> span){
		return asBuffer(java.nio.FloatBuffer.class, span);
	}
	
	public static java.nio.@NonNull DoubleBuffer asDoubleBuffer(@StrictNonNull QConstSpan<@NonNull@QtPrimitiveType Double> span){
		return asBuffer(java.nio.DoubleBuffer.class, span);
	}
	
	public static byte @NonNull[] asByteArray(@StrictNonNull QConstSpan<@NonNull@QtPrimitiveType Byte> span){
		return asArray(byte[].class, span);
	}
	
	public static short @NonNull[] asShortArray(@StrictNonNull QConstSpan<@NonNull@QtPrimitiveType Short> span){
		return asArray(short[].class, span);
	}
	
	public static int @NonNull[] asIntArray(@StrictNonNull QConstSpan<@NonNull@QtPrimitiveType Integer> span){
		return asArray(int[].class, span);
	}
	
	public static long @NonNull[] asLongArray(@StrictNonNull QConstSpan<@NonNull@QtPrimitiveType Long> span){
		return asArray(long[].class, span);
	}
	
	public static char @NonNull[] asCharArray(@StrictNonNull QConstSpan<@NonNull@QtPrimitiveType Character> span){
		return asArray(char[].class, span);
	}
	
	public static float @NonNull[] asFloatArray(@StrictNonNull QConstSpan<@NonNull@QtPrimitiveType Float> span){
		return asArray(float[].class, span);
	}
	
	public static double @NonNull[] asDoubleArray(@StrictNonNull QConstSpan<@NonNull@QtPrimitiveType Double> span){
		return asArray(double[].class, span);
	}
	
	public static boolean @NonNull[] asBooleanArray(@StrictNonNull QConstSpan<@NonNull@QtPrimitiveType Boolean> span){
		return asArray(boolean[].class, span);
	}
	
	public static <T> T @NonNull[] asObjectArray(@StrictNonNull QConstSpan<@NonNull@QtPrimitiveType T> span){
		return span.toArray();
	}
	
	/**
     * <p>See <code><a href="https://doc.qt.io/qt/qspan.html#first">QSpan::<wbr>first(size_type)const</a></code></p>
     */
	public @NonNull QConstSpan<T> first(int n) {
		return subspan(0, n);
	}
	
	/**
     * <p>See <code><a href="https://doc.qt.io/qt/qspan.html#last">QSpan::<wbr>last(size_type)const</a></code></p>
     */
	public @NonNull QConstSpan<T> last(int n) {
		return subspan(size()-n, n);
	}

	/**
     * <p>See <code><a href="https://doc.qt.io/qt/qspan.html#sliced">QSpan::<wbr>sliced(size_type)const</a></code></p>
     */
	public @NonNull QConstSpan<T> sliced(int offset) {
		return subspan(offset);
	}
	
	/**
     * <p>See <code><a href="https://doc.qt.io/qt/qspan.html#sliced-1">QSpan::<wbr>sliced(size_type,size_type)const</a></code></p>
     */
	public @NonNull QConstSpan<T> sliced(int offset, int n) {
		return subspan(offset, n);
	}
	
	/**
     * <p>See <code><a href="https://doc.qt.io/qt/qspan.html#subspan-2">QSpan::<wbr>subspan(size_type)const</a></code></p>
     */
	public @NonNull QConstSpan<T> subspan(int offset) {
		return subspan(offset, 0);
	}
	
	/**
     * <p>See <code><a href="https://doc.qt.io/qt/qspan.html#subspan-3">QSpan::<wbr>subspan(size_type,size_type)const</a></code></p>
     */
	public @NonNull QConstSpan<T> subspan(int offset, int n) {
    	if(offset<0 || offset>size())
    		throw new IndexOutOfBoundsException(offset);
    	if(offset+n>size())
    		throw new IndexOutOfBoundsException(offset+n);
    	return new QConstSpan<T>(this, offset, n);
	}

    /**
     * {@inheritDoc}
     */
	@Override
    @QtUninvokable
    public final boolean equals(Object other) {
        if (other instanceof AbstractSpan) {
        	AbstractSpan<?> _other = (AbstractSpan<?>)other;
        	return hashCode()==_other.hashCode();
        }
        return false;
    }
    
	@QtUninvokable
	@Override
    public final java.util.Iterator<T> iterator(){
		QSequentialConstIterator<T> begin = constBegin();
    	return begin==null ? Collections.<T>emptyList().listIterator() : begin.iterator();
    }
    
	@QtUninvokable
	public final java.util.Iterator<T> descendingIterator() {
		QSequentialConstIterator<T> end = constEnd();
		return end==null ? Collections.<T>emptyList().listIterator() : end.toJavaDescendingIterator();
    }
	
	/**
	 * Returns the value type of the QSpan.
	 */
    @QtUninvokable
	public final QMetaType elementType() {
    	return _elementType();
	}
    
    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qspan.html#isEmpty">QSpan::<wbr>isEmpty()const</a></code></p>
     */
    @QtUninvokable
	public final boolean isEmpty() {
    	return size()==0;
    }

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qspan.html#size">QSpan::<wbr>size()const</a></code></p>
     */
    @QtUninvokable
	public final int size() {
		return AbstractSpan.size(QtJambi_LibraryUtilities.internal.nativeId(this));
	}
    
    @QtUninvokable
	public final boolean isBuffered() {
		return d.isBuffered();
	}
    
    @QtUninvokable
	public final boolean isConst() {
		return AbstractSpan.isConst(QtJambi_LibraryUtilities.internal.nativeId(this));
	}
    
    @QtUninvokable
	public final void flush() {
    	d.commit(this);
	}
    
    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qspan.html#size_bytes">QSpan::<wbr>size_bytes()const</a></code></p>
     */
    @QtUninvokable
	public final int size_bytes() {
		return size() * (int)_elementType().sizeOf();
	}
    
    /**
     * Returns all elements as array
     * @return array
     */
    @SuppressWarnings("unchecked")
	@QtUninvokable
	public final T[] toArray() {
    	Class<?> elementType = _elementType().javaType();
    	if(elementType==null || elementType.isPrimitive()) {
	    	if(elementType==null || elementType==void.class) {
	    		return (T[])new Void[0];
	    	}else if(elementType==byte.class) {
	    		elementType = Byte.class;
	    	}else if(elementType==short.class) {
	    		elementType = Short.class;
	    	}else if(elementType==int.class) {
	    		elementType = Integer.class;
	    	}else if(elementType==long.class) {
	    		elementType = Long.class;
	    	}else if(elementType==boolean.class) {
	    		elementType = Boolean.class;
	    	}else if(elementType==char.class) {
	    		elementType = Character.class;
	    	}else if(elementType==float.class) {
	    		elementType = Float.class;
	    	}else if(elementType==double.class) {
	    		elementType = Double.class;
	    	}
    	}
    	Object result = Array.newInstance(elementType, (int)size());
    	int i=0;
    	for(T value : this) {
    		Array.set(result, i++, value);
    	}
    	return (T[])result;
    }
    
    /**
     * Returns all elements as array
     * @return array
     */
	@QtUninvokable
	public final QList<T> toList() {
    	return d.toList(this);
    }
    
    /**
     * Returns the value at given position
     * @param index
     * @return value
     */
    @QtUninvokable
	public final T at(int index) {
    	return get(index);
    }
    
    /**
     * Returns the value at given position
     * @param index
     * @return value
     */
    @QtUninvokable
	public final T get(int index) {
    	if(index<0 || index>=size())
    		throw new IndexOutOfBoundsException(index);
    	return get(QtJambi_LibraryUtilities.internal.nativeId(this), index);
    }
    
    /**
     * {@inheritDoc}
     */
	@Override
	public QConstSpan<T> clone(){
		return new QConstSpan<T>(this);
	}

    /**
     * {@inheritDoc}
     */
	@Override
	public final String toString() {
		return toString(QtJambi_LibraryUtilities.internal.nativeId(this));
	}
	
    /**
     * {@inheritDoc}
     */
	@Override
	public int hashCode() {
		return hashCode(QtJambi_LibraryUtilities.internal.nativeId(this));
	}
	
	boolean isConstSpan() {
		return true;
	}
}
