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
package io.qt.core;

import java.util.Arrays;
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
public final class QSpan<T> extends QConstSpan<T>{
	
	@NativeAccess
    private QSpan(QPrivateConstructor c, QtObject owner) { 
		super(c, owner);
	}
	
	@NativeAccess
	private QSpan(QPrivateConstructor c, QList<T> list){
		super(c, list);
	}
	
	private QSpan(java.nio.Buffer buffer, char type) {
		super(buffer, type);
	}
	
	private QSpan(boolean[] array) {
		super(array);
	}
	
	private QSpan(byte[] array) {
		super(array);
	}

	private QSpan(char[] array) {
		super(array);
	}

	private QSpan(double[] array) {
		super(array);
	}

	private QSpan(float[] array) {
		super(array);
	}

	private QSpan(int[] array) {
		super(array);
	}

	private QSpan(long[] array) {
		super(array);
	}

	private QSpan(short[] array) {
		super(array);
	}

	public QSpan(){
		super();
	}
	
	public QSpan(QSpan<T> other){
		super(other, 0, 0);
	}
	
	private QSpan(QSpan<T> other, int offset, int n){
		super(other, offset, n);
	}
	
	public QSpan(@StrictNonNull QList<T> list){
		super(list);
	}
	
	private QSpan(@StrictNonNull QByteArray byteArray){
		super(byteArray);
	}
	
	private QSpan(@StrictNonNull QString string){
		super(string);
	}
	
	private QSpan(Class<? super T> type, T[] array){
		super(type, array);
	}
	
	private QSpan(QMetaType metaType, T[] array){
		super(metaType, array);
	}
	
	public static @NonNull QSpan<@NonNull@QtPrimitiveType Byte> ofBuffer(java.nio.@StrictNonNull ByteBuffer buffer){
		return new QSpan<>(buffer, 'B');
	}
	
	public static @NonNull QSpan<@NonNull@QtPrimitiveType Short> ofBuffer(java.nio.@StrictNonNull ShortBuffer buffer){
		return new QSpan<>(buffer, 'S');
	}
	
	public static @NonNull QSpan<@NonNull@QtPrimitiveType Integer> ofBuffer(java.nio.@StrictNonNull IntBuffer buffer){
		return new QSpan<>(buffer, 'I');
	}
	
	public static @NonNull QSpan<@NonNull@QtPrimitiveType Long> ofBuffer(java.nio.@StrictNonNull LongBuffer buffer){
		return new QSpan<>(buffer, 'J');
	}
	
	public static @NonNull QSpan<@NonNull@QtPrimitiveType Character> ofBuffer(java.nio.@StrictNonNull CharBuffer buffer){
		return new QSpan<>(buffer, 'C');
	}
	
	public static @NonNull QSpan<@NonNull@QtPrimitiveType Float> ofBuffer(java.nio.@StrictNonNull FloatBuffer buffer){
		return new QSpan<>(buffer, 'F');
	}
	
	public static @NonNull QSpan<@NonNull@QtPrimitiveType Double> ofBuffer(java.nio.@StrictNonNull DoubleBuffer buffer){
		return new QSpan<>(buffer, 'D');
	}
	
	@SuppressWarnings("overrides")
	public static @NonNull QSpan<@NonNull@QtPrimitiveType Byte> ofByte(byte[] array){
		return new QSpan<>(array);
	}
	
	@SuppressWarnings("overrides")
	public static @NonNull QSpan<@NonNull@QtPrimitiveType Short> ofShort(short[] array){
		return new QSpan<>(array);
	}
	
	@SuppressWarnings("overrides")
	public static @NonNull QSpan<@NonNull@QtPrimitiveType Integer> ofInt(int[] array){
		return new QSpan<>(array);
	}
	
	@SuppressWarnings("overrides")
	public static @NonNull QSpan<@NonNull@QtPrimitiveType Long> ofLong(long[] array){
		return new QSpan<>(array);
	}
	
	@SuppressWarnings("overrides")
	public static @NonNull QSpan<@NonNull@QtPrimitiveType Character> ofChar(char[] array){
		return new QSpan<>(array);
	}
	
	@SuppressWarnings("overrides")
	public static @NonNull QSpan<@NonNull@QtPrimitiveType Float> ofFloat(float[] array){
		return new QSpan<>(array);
	}
	
	@SuppressWarnings("overrides")
	public static @NonNull QSpan<@NonNull@QtPrimitiveType Double> ofDouble(double[] array){
		return new QSpan<>(array);
	}
	
	@SuppressWarnings("overrides")
	public static @NonNull QSpan<@NonNull@QtPrimitiveType Boolean> ofBoolean(boolean[] array){
		return new QSpan<>(array);
	}

	public static <T> @NonNull QSpan<T> ofList(@StrictNonNull QList<T> list){
		return new QSpan<>(list);
	}
	
	public static <T> @NonNull QSpan<T> of(){
		return new QSpan<>();
	}
	
	public static <T> @NonNull QSpan<T> of(T @StrictNonNull[] elements){
		return new QSpan<>(QList.findElementMetaType(Arrays.asList(elements)), elements);
	}
	
	@SafeVarargs
	public static <T> @NonNull QSpan<T> ofTyped(@StrictNonNull QMetaType metaType, T @StrictNonNull...elements){
		return new QSpan<>(metaType, elements);
	}
	
	@SafeVarargs
	public static <T> @NonNull QSpan<T> ofTyped(@Nullable Class<? super T> type, T @StrictNonNull...elements){
		return new QSpan<>(type, elements);
	}
	
	public static @NonNull QSpan<@NonNull@QtPrimitiveType Byte> ofByte(@StrictNonNull QByteArray array){
		return new QSpan<>(array);
	}
	
	public static @NonNull QSpan<@NonNull@QtPrimitiveType Character> ofChar(@StrictNonNull QString string){
		return new QSpan<>(string);
	}

	public static java.nio.@NonNull ByteBuffer asByteBuffer(@StrictNonNull QSpan<@NonNull@QtPrimitiveType Byte> span){
		return asBuffer(java.nio.ByteBuffer.class, span);
	}
	
	public static java.nio.@NonNull ShortBuffer asShortBuffer(@StrictNonNull QSpan<@NonNull@QtPrimitiveType Short> span){
		return asBuffer(java.nio.ShortBuffer.class, span);
	}
	
	public static java.nio.@NonNull IntBuffer asIntBuffer(@StrictNonNull QSpan<@NonNull@QtPrimitiveType Integer> span){
		return asBuffer(java.nio.IntBuffer.class, span);
	}
	
	public static java.nio.@NonNull LongBuffer asLongBuffer(@StrictNonNull QSpan<@NonNull@QtPrimitiveType Long> span){
		return asBuffer(java.nio.LongBuffer.class, span);
	}
	
	public static java.nio.@NonNull CharBuffer asCharBuffer(@StrictNonNull QSpan<@NonNull@QtPrimitiveType Character> span){
		return asBuffer(java.nio.CharBuffer.class, span);
	}
	
	public static java.nio.@NonNull FloatBuffer asFloatBuffer(@StrictNonNull QSpan<@NonNull@QtPrimitiveType Float> span){
		return asBuffer(java.nio.FloatBuffer.class, span);
	}
	
	public static java.nio.@NonNull DoubleBuffer asDoubleBuffer(@StrictNonNull QSpan<@NonNull@QtPrimitiveType Double> span){
		return asBuffer(java.nio.DoubleBuffer.class, span);
	}
	
	public static byte @NonNull[] asByteArray(@StrictNonNull QSpan<@NonNull@QtPrimitiveType Byte> span){
		return asArray(byte[].class, span);
	}
	
	public static short @NonNull[] asShortArray(@StrictNonNull QSpan<@NonNull@QtPrimitiveType Short> span){
		return asArray(short[].class, span);
	}
	
	public static int @NonNull[] asIntArray(@StrictNonNull QSpan<@NonNull@QtPrimitiveType Integer> span){
		return asArray(int[].class, span);
	}
	
	public static long @NonNull[] asLongArray(@StrictNonNull QSpan<@NonNull@QtPrimitiveType Long> span){
		return asArray(long[].class, span);
	}
	
	public static char @NonNull[] asCharArray(@StrictNonNull QSpan<@NonNull@QtPrimitiveType Character> span){
		return asArray(char[].class, span);
	}
	
	public static float @NonNull[] asFloatArray(@StrictNonNull QSpan<@NonNull@QtPrimitiveType Float> span){
		return asArray(float[].class, span);
	}
	
	public static double @NonNull[] asDoubleArray(@StrictNonNull QSpan<@NonNull@QtPrimitiveType Double> span){
		return asArray(double[].class, span);
	}
	
	public static boolean @NonNull[] asBooleanArray(@StrictNonNull QSpan<@NonNull@QtPrimitiveType Boolean> span){
		return asArray(boolean[].class, span);
	}
	
	public static <T> T @NonNull[] asObjectArray(@StrictNonNull QSpan<@NonNull@QtPrimitiveType T> span){
		return span.toArray();
	}
	
	/**
     * <p>See <code><a href="https://doc.qt.io/qt/qspan.html#first">QSpan::<wbr>first(size_type)const</a></code></p>
     */
	public final @NonNull QSpan<T> first(int n) {
		return subspan(0, n);
	}
	
	/**
     * <p>See <code><a href="https://doc.qt.io/qt/qspan.html#last">QSpan::<wbr>last(size_type)const</a></code></p>
     */
	public final @NonNull QSpan<T> last(int n) {
		return subspan(size()-n, n);
	}
	
	/**
     * <p>See <code><a href="https://doc.qt.io/qt/qspan.html#chopped">QSpan::<wbr>chopped(size_type)const</a></code></p>
     */
	public @NonNull QSpan<T> chopped(int n) {
		return first(size()-n);
	}

	/**
     * <p>See <code><a href="https://doc.qt.io/qt/qspan.html#sliced">QSpan::<wbr>sliced(size_type)const</a></code></p>
     */
	public final @NonNull QSpan<T> sliced(int offset) {
		return subspan(offset);
	}
	
	/**
     * <p>See <code><a href="https://doc.qt.io/qt/qspan.html#sliced-1">QSpan::<wbr>sliced(size_type,size_type)const</a></code></p>
     */
	public final @NonNull QSpan<T> sliced(int offset, int n) {
		return subspan(offset, n);
	}
	
	/**
     * <p>See <code><a href="https://doc.qt.io/qt/qspan.html#subspan-2">QSpan::<wbr>subspan(size_type)const</a></code></p>
     */
	public final @NonNull QSpan<T> subspan(int offset) {
		return subspan(offset, 0);
	}
	
	/**
     * <p>See <code><a href="https://doc.qt.io/qt/qspan.html#subspan-3">QSpan::<wbr>subspan(size_type,size_type)const</a></code></p>
     */
	public final @NonNull QSpan<T> subspan(int offset, int n) {
    	if(offset<0 || offset>size())
    		throw new IndexOutOfBoundsException(offset);
    	if(offset+n>size())
    		throw new IndexOutOfBoundsException(offset+n);
    	return new QSpan<T>(this, offset, n);
	}

    /**
     * @throws IllegalAccessException if QSpan is read-only
     */
	@QtUninvokable
    public final java.util.ListIterator<T> mutableIterator() throws IllegalAccessException{
		if(isConst()){
			throw new IllegalAccessException("QSpan is read-only");
		}else {
			QSequentialIterator<T> begin = begin();
			if(begin!=null) {
				return begin.mutableIterator();
			}else {
				return Collections.<T>emptyList().listIterator();
			}
		}
    }
    
    /**
     * Changes the value at given position
     * @param index
     * @param value
     */
    @QtUninvokable
	public final void replace(int index, T value) {
    	set(index, value);
    }
    
    /**
     * Changes the value at given position
     * @param index
     * @param value
     */
    @QtUninvokable
	public final void set(int index, T value) {
    	if(index<0 || index>=size())
    		throw new IndexOutOfBoundsException(index);
    	if(!set(QtJambi_LibraryUtilities.internal.nativeId(this), index, value) && isConst())
    		throw new RuntimeException("Unable to change a constant QSpan");
    }

    /**
     * {@inheritDoc}
     */
	@Override
	public final QSpan<T> clone(){
		return new QSpan<T>(this);
	}
	
	boolean isConstSpan() {
		return false;
	}
}
