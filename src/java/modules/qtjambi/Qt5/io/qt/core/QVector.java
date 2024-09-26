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

import java.nio.*;
import java.util.*;
import java.util.function.*;
import io.qt.*;

/**
 * <p>Java wrapper for Qt class <a href="https://doc.qt.io/qt/qvector.html">QVector</a></p>
 */
public class QVector<T> extends AbstractList<T> implements Cloneable
{

    static {
        QtJambi_LibraryUtilities.initialize();
    }
    
    /**
     * Constructor for internal use only.
     * @param p expected to be <code>null</code>.
     */
    @NativeAccess
    protected QVector(QPrivateConstructor p) {
        super(p);
    }
    
    public QVector(QMetaType.@StrictNonNull Type metaType) {
		this(new QMetaType(metaType));
	}
    
    public QVector(@Nullable Class<T> elementType) {
		super(null);
		QMetaType metaType = QMetaType.fromType(elementType);
		initialize(elementType, QtJambi_LibraryUtilities.internal.nativeId(metaType), null);
	}
    
	public QVector(@StrictNonNull QMetaType metaType) {
		super(null);
		initialize(metaType.javaType(), QtJambi_LibraryUtilities.internal.nativeId(metaType), null);
	}
    
    public QVector(@StrictNonNull Collection<T> other) {
		super(null);
		QMetaType metaType = QList.findElementMetaType(Objects.requireNonNull(other));
		initialize(metaType.javaType(), QtJambi_LibraryUtilities.internal.nativeId(metaType), other);
    }
    
    QVector(QMetaType metaType, Collection<T> other) {
		super(null);
		initialize(metaType.javaType(), QtJambi_LibraryUtilities.internal.nativeId(metaType), other);
    }
    
    /**
     * Creating a container of type QVariant.
     */
    public static @NonNull QVector<Object> createVariantVector(){
        return new QVector<>(new QMetaType(QMetaType.Type.QVariant));
    }

    @QtUninvokable
    private native void initialize(Class<?> elementType, long elementMetaType, Collection<T> other);
    
    /**
     * Creates and returns a copy of this object.
     */
    @Override
    public @NonNull QVector<T> clone(){
        return new QVector<>(this);
    }
    
    @QtUninvokable
    private native void onDispose(long listAccess);

    @QtUninvokable
    public final void append(java.util.@NonNull Collection<T> t)    {
        appendVector(QtJambi_LibraryUtilities.internal.nativeId(this), t);
    }
    @QtUninvokable
    private native void appendVector(long __this__nativeId, java.util.Collection<T> t);

    @QtUninvokable
    public final void append(T t)    {
        try {
        	append(QtJambi_LibraryUtilities.internal.nativeId(this), t);
        }catch(QNoNativeResourcesException e) {
            throw e;
        }catch(RuntimeException e) {
            throw QSet.handleException(e, elementMetaType(), t);
        }
    }
	
	@QtUninvokable
    private native void append(long __this__nativeId, T t);

    @QtUninvokable
    public final T at(int i)    {
        return at(QtJambi_LibraryUtilities.internal.nativeId(this), i);
    }
    @QtUninvokable
    private native T at(long __this__nativeId, int i);

    @QtUninvokable
    protected final @NonNull QSequentialIterator<T> begin()    {
        return begin(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private native QSequentialIterator<T> begin(long __this__nativeId);

    @QtUninvokable
    protected final @NonNull QSequentialIterator<T> end()    {
        return end(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private native QSequentialIterator<T> end(long __this__nativeId);

    @QtUninvokable
    protected final @NonNull QSequentialConstIterator<T> constBegin()    {
        return constBegin(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private native QSequentialConstIterator<T> constBegin(long __this__nativeId);

    @QtUninvokable
    protected final @NonNull QSequentialConstIterator<T> constEnd()    {
        return constEnd(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private native QSequentialConstIterator<T> constEnd(long __this__nativeId);

    @QtUninvokable
    public final int capacity()    {
        return capacity(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private native int capacity(long __this__nativeId);

    @QtUninvokable
    public final void clear()    {
        clear(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private native void clear(long __this__nativeId);

    @QtUninvokable
    public final boolean contains(Object t)    {
        try{
        	return contains(QtJambi_LibraryUtilities.internal.nativeId(this), t);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(IllegalArgumentException e) {
    		return false;
    	}catch(RuntimeException e) {
    		RuntimeException e1 = QSet.handleException(e, elementMetaType(), t);
    		if(e1==e)
    			throw e;
    		return false;
    	}
    }
    @QtUninvokable
    private native boolean contains(long __this__nativeId, Object t);

    @QtUninvokable
    public final int count()    {
        return size();
    }

    @QtUninvokable
    public final int count(T t)    {
        try{
        	return count(QtJambi_LibraryUtilities.internal.nativeId(this), t);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QSet.handleException(e, elementMetaType(), t);
    	}
    }
    @QtUninvokable
    private native int count(long __this__nativeId, T t);

    @QtUninvokable
    public final boolean endsWith(T t)    {
        try{
        	return endsWith(QtJambi_LibraryUtilities.internal.nativeId(this), t);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QSet.handleException(e, elementMetaType(), t);
        }
    }
    @QtUninvokable
    private native boolean endsWith(long __this__nativeId, T t);

    @QtUninvokable
    public final void fill(T t) {
        fill(t, (int)-1);
    }
    @QtUninvokable
    public final void fill(T t, int size)    {
    	try {
    		fill(QtJambi_LibraryUtilities.internal.nativeId(this), t, size);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QSet.handleException(e, elementMetaType(), t);
    	}
    }
    @QtUninvokable
    private native void fill(long __this__nativeId, T t, int size);

    @QtUninvokable
    public final T first()    {
		try{
			return at(0);
		} catch (IndexOutOfBoundsException e) {
			throw new NoSuchElementException();
		}
    }

    @QtUninvokable
    public final int indexOf(T t, int from)    {
    	try {
    		return indexOf(QtJambi_LibraryUtilities.internal.nativeId(this), t, from);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(IllegalArgumentException e) {
    		return -1;
    	}catch(RuntimeException e) {
    		RuntimeException e1 = QSet.handleException(e, elementMetaType(), t);
    		if(e1==e)
    			throw e;
    		return -1;
    	}
    }
    @QtUninvokable
    private native int indexOf(long __this__nativeId, T t, int from);

    @QtUninvokable
    public final void insert(int i, T t)    {
        insert(i, 1, t);
    }

    @QtUninvokable
    public final void insert(int i, int n, T t)    {
        try {
        	insert(QtJambi_LibraryUtilities.internal.nativeId(this), i, n, t);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QSet.handleException(e, elementMetaType(), t);
    	}
    }
    @QtUninvokable
    private native void insert(long __this__nativeId, int i, int n, T t);

    @QtUninvokable
    public final boolean isEmpty()    {
        return size()==0;
    }

    @QtUninvokable
    public final T last() {
        try{
			return last(QtJambi_LibraryUtilities.internal.nativeId(this));
		} catch (IndexOutOfBoundsException e) {
			throw new NoSuchElementException();
		}
    }
    @QtUninvokable
    private native T last(long __this__nativeId);

    @SuppressWarnings("unchecked")
	@QtUninvokable
    @Override
    public final int lastIndexOf(Object t) {
    	try {
    		return lastIndexOf((T)t, (int)-1);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(IllegalArgumentException e) {
    		return -1;
    	}catch(RuntimeException e) {
    		RuntimeException e1 = QSet.handleException(e, elementMetaType(), t);
    		if(e1==e)
    			throw e;
    		return -1;
    	}
    }
    @QtUninvokable
    public final int lastIndexOf(T t, int from)    {
    	try {
    		return lastIndexOf(QtJambi_LibraryUtilities.internal.nativeId(this), t, from);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(IllegalArgumentException e) {
    		return -1;
    	}catch(RuntimeException e) {
    		RuntimeException e1 = QSet.handleException(e, elementMetaType(), t);
    		if(e1==e)
    			throw e;
    		return -1;
    	}
    }
    @QtUninvokable
    private native int lastIndexOf(long __this__nativeId, T t, int from);

    @QtUninvokable
    public final int length()    {
        return size();
    }

    @QtUninvokable
    public final @NonNull QVector<T> mid(int pos) {
        return mid(pos, (int)-1);
    }
    @QtUninvokable
    public final @NonNull QVector<T> mid(int pos, int length)    {
        return mid(QtJambi_LibraryUtilities.internal.nativeId(this), pos, length);
    }
    @QtUninvokable
    private native QVector<T> mid(long __this__nativeId, int pos, int length);

    @QtUninvokable
    public final void move(int from, int to)    {
        move(QtJambi_LibraryUtilities.internal.nativeId(this), from, to);
    }
    @QtUninvokable
    private native void move(long __this__nativeId, int from, int to);

    @QtUninvokable
    public final void prepend(T t) {
    	try {
        	insert(QtJambi_LibraryUtilities.internal.nativeId(this), 0, 1, t);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QSet.handleException(e, elementMetaType(), t);
    	}
    }

    @QtUninvokable
    public final void remove(int i, int n) {
        remove(QtJambi_LibraryUtilities.internal.nativeId(this), i, n);
    }
    @QtUninvokable
    private native void remove(long __this__nativeId, int i, int n);

    @QtUninvokable
    public final int removeAll(T t) {
        try {
        	return removeAll(QtJambi_LibraryUtilities.internal.nativeId(this), t);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(IllegalArgumentException e) {
    		return 0;
    	}catch(RuntimeException e) {
    		RuntimeException e1 = QSet.handleException(e, elementMetaType(), t);
    		if(e1==e)
    			throw e;
    		return 0;
    	}
    }
    @QtUninvokable
    private native int removeAll(long __this__nativeId, T t);

    @QtUninvokable
    public final void removeAt(int i) {
        remove(i, 1);
    }

    @QtUninvokable
    public final T removeFirst() {
        return takeFirst();
    }

    @QtUninvokable
    public final T removeLast() {
        return takeLast();
    }

    @QtUninvokable
    public final boolean removeOne(T t) {
        try {
    		return removeOne(QtJambi_LibraryUtilities.internal.nativeId(this), t);
        }catch(QNoNativeResourcesException e) {
            throw e;
        }catch(IllegalArgumentException e) {
        }catch(RuntimeException e) {
            RuntimeException e1 = QSet.handleException(e, elementMetaType(), t);
            if(e1==e)
                throw e;
        }
        return false;
    }
    @QtUninvokable
    private native boolean removeOne(long __this__nativeId, T t);

	/**
     * <p>See <a href="https://doc.qt.io/qt/qvector.html#removeIf" class="member-name-class">QVector::<wbr>removeIf(Predicate)</a></p>
	 */
    @QtUninvokable
    public final boolean removeIf(@StrictNonNull Predicate<? super T> predicate)    {
        Objects.requireNonNull(predicate);
        boolean removed = false;
        for(T value : clone()) {
            if (predicate.test(value)) {
                removeAll(value);
                removed = true;
            }
        }
        return removed;
    }
	
    @QtUninvokable
    public final void replace(int i, T t) {
        try {
        	replace(QtJambi_LibraryUtilities.internal.nativeId(this), i, t);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QSet.handleException(e, elementMetaType(), t);
    	}
    }
    @QtUninvokable
    private native void replace(long __this__nativeId, int i, T t);

    @QtUninvokable
    public final void reserve(int size) {
        reserve(QtJambi_LibraryUtilities.internal.nativeId(this), size);
    }
    @QtUninvokable
    private native void reserve(long __this__nativeId, int size);

    @QtUninvokable
    public final void resize(int size) {
        resize(QtJambi_LibraryUtilities.internal.nativeId(this), size);
    }
    @QtUninvokable
    private native void resize(long __this__nativeId, int size);

    @QtUninvokable
    public final void squeeze() {
        squeeze(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private native void squeeze(long __this__nativeId);

    @QtUninvokable
    public final int size() {
        return size(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private native int size(long __this__nativeId);

    @QtUninvokable
    public final boolean startsWith(T t) {
        try{
        	return startsWith(QtJambi_LibraryUtilities.internal.nativeId(this), t);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QSet.handleException(e, elementMetaType(), t);
        }
    }
    @QtUninvokable
    private native boolean startsWith(long __this__nativeId, T t);

    @QtUninvokable
    public final T takeAt(int i) {
        return takeAt(QtJambi_LibraryUtilities.internal.nativeId(this), i);
    }
    @QtUninvokable
    private native T takeAt(long __this__nativeId, int i);

    @QtUninvokable
    public final T takeFirst() {
		try{
			return takeAt(0);
		} catch (IndexOutOfBoundsException e) {
			throw new NoSuchElementException();
		}
    }
    
    @QtUninvokable
    public final T takeLast() {
        try{
        	return takeLast(QtJambi_LibraryUtilities.internal.nativeId(this));
		} catch (IndexOutOfBoundsException e) {
			throw new NoSuchElementException();
		}
    }
    @QtUninvokable
    private native T takeLast(long __this__nativeId);

    @QtUninvokable
    public final @NonNull QSet<T> toSet() {
		QSet<T> set = new QSet<>(elementMetaType());
		set.unite(this);
        return set;
    }

    @QtUninvokable
    public final T value(int i) {
        return value(QtJambi_LibraryUtilities.internal.nativeId(this), i);
    }
    @QtUninvokable
    private native T value(long __this__nativeId, int i);

    @QtUninvokable
    public final T value(int i, T defaultValue) {
    	try {
    		return valueDefault(QtJambi_LibraryUtilities.internal.nativeId(this), i, defaultValue);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QSet.handleException(e, elementMetaType(), defaultValue);
    	}
    }
    @QtUninvokable
    private native T valueDefault(long __this__nativeId, int i, T defaultValue);

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qvector.html#operator-eq-eq" class="member-name-class">QVector::<wbr>operator==(QVector&lt;T>)const</a></code></p>
     */
    @Override
    @QtUninvokable
    public boolean equals(Object other) {
    	if(other instanceof StackDeque<?>) {
			other = ((StackDeque<?>)other).stack;
		}
        if (other instanceof java.util.Collection && QList.checkContainerType(this.elementMetaType(), (java.util.Collection<?>) other)) {
            return operator_equal(QtJambi_LibraryUtilities.internal.nativeId(this), (java.util.Collection<?>) other);
        }
        return false;
    }
	
    @QtUninvokable
    private native boolean operator_equal(long __this__nativeId, java.util.Collection<?> l);

    /**
     * Returns the objects's hash code computed by <code>qHash(QVector&lt;T>)</code>.
     */
    @Override
    @QtUninvokable
    public int hashCode() { 
    	try {
    		return hashCode(QtJambi_LibraryUtilities.internal.nativeId(this));
		} catch (QNoNativeResourcesException e) {
			return 0;
		}
    }
    @QtUninvokable
    private native int hashCode(long __this__nativeId);
    
    @Override
    @QtUninvokable
    public @NonNull String toString() {
    	try {
			return toString(QtJambi_LibraryUtilities.internal.nativeId(this));
		} catch (QNoNativeResourcesException e) {
			return "null";
		}
    }
    @QtUninvokable
    private native String toString(long __this__nativeId);
    
    @Override
    @QtUninvokable
    public boolean add(T e){
        append(e);
        return true;
    }

    @Override
    @QtUninvokable
    public void add(int index, T e){
            insert(index, e);
    }

    @Override
    @QtUninvokable
    public T set(int index, T e){
        if(e!=null){
            T el = at(index);
            replace(index, e);
            return el;
        } else return null;
    }
    
    @SuppressWarnings("unchecked")
	@Override
    @QtUninvokable
    public boolean remove(Object e){
        return removeOne((T)e);
    }

    @SuppressWarnings("unchecked")
	@Override
    @QtUninvokable
    public int indexOf(Object e){
        return indexOf((T)e, 0);
    }

    @Override
    @QtUninvokable
    public T get(int index) {
        return at(index);
    }
    
    @QtUninvokable
    public final void swapItemsAt(int i, int j) {
        swapItemsAt(QtJambi_LibraryUtilities.internal.nativeId(this), i, j);
    }
    
    @QtUninvokable
    private native void swapItemsAt(long __this__nativeId, int i, int j);
    
    /**
     * <p>See <code>operator&lt;&lt;(QDataStream&amp;,<wbr>QVector&lt;T>)</code></p>
     */
    @io.qt.QtUninvokable
    public void writeTo(io.qt.core.@StrictNonNull QDataStream stream){
    	java.util.Objects.requireNonNull(stream, "Argument 'stream': null not expected.");
        writeTo(QtJambi_LibraryUtilities.internal.nativeId(this), QtJambi_LibraryUtilities.internal.nativeId(stream));
    }
    
    @io.qt.QtUninvokable
    private native void writeTo(long __this__nativeId, long stream);
    
    /**
     * <p>See <code>operator&gt;&gt;(QDataStream&amp;,<wbr>QVector&lt;T>&amp;)</code></p>
     */
    @io.qt.QtUninvokable
    public void readFrom(io.qt.core.@StrictNonNull QDataStream stream){
    	java.util.Objects.requireNonNull(stream, "Argument 'stream': null not expected.");
        readFrom(QtJambi_LibraryUtilities.internal.nativeId(this), QtJambi_LibraryUtilities.internal.nativeId(stream));
    }
    
    @io.qt.QtUninvokable
    private native void readFrom(long __this__nativeId, long stream);
    
    @io.qt.QtUninvokable
    QMetaType elementMetaType() {
    	return elementMetaType(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @io.qt.QtUninvokable
    private native QMetaType elementMetaType(long containerAccess);
    
    /**
     * Returns a QVector containing given elements.
     *
     * @param <T> the {@code QVector}'s element type
     * @param element0 the first element
     * @param elements subsequent elements
     * @return a {@code QVector} containing the specified element
     * @throws NullPointerException if elements are {@code null}
     */
    @SuppressWarnings({ "unchecked" })
    @SafeVarargs
    public static <T> @NonNull QVector<T> of(T element0, T @StrictNonNull...elements) {
        QMetaType metaType = QList.findElementMetaType(element0, elements);
		T[] allElements = (T[])new Object[elements.length+1];
		System.arraycopy(elements, 0, allElements, 1, elements.length);
		allElements[0] = element0;
		return ofTyped(metaType, allElements);
    }
    
    /**
     * Returns a QVector containing given elements.
     *
     * @param elements
     * @return a {@code QVector} containing the specified element
     */
    public static @NonNull QVector<@QtPrimitiveType@NonNull Byte> ofByte(byte @StrictNonNull...elements) {
		QVector<Byte> result = new QVector<>(byte.class);
    	result.resize(elements.length);
    	ByteBuffer data = QtJambi_LibraryUtilities.internal.mutableDataB(result);
		data.put(elements, 0, elements.length);
		QtJambi_LibraryUtilities.internal.truncateBuffer(result, data);
		return result;
	}
    
    /**
     * Returns a QVector containing given elements.
     *
     * @param elements
     * @return a {@code QVector} containing the specified element
     */
    public static @NonNull QVector<@QtPrimitiveType@NonNull Short> ofShort(short @StrictNonNull...elements) {
		QVector<Short> result = new QVector<>(short.class);
    	result.resize(elements.length);
    	ShortBuffer data = QtJambi_LibraryUtilities.internal.mutableDataS(result);
		data.put(elements, 0, elements.length);
		QtJambi_LibraryUtilities.internal.truncateBuffer(result, data);
		return result;
	}
    
    /**
     * Returns a QVector containing given elements.
     *
     * @param elements
     * @return a {@code QVector} containing the specified element
     */
    public static @NonNull QVector<@QtPrimitiveType@NonNull Integer> ofInt(int @StrictNonNull...elements) {
		QVector<Integer> result = new QVector<>(int.class);
    	result.resize(elements.length);
    	IntBuffer data = QtJambi_LibraryUtilities.internal.mutableDataI(result);
		data.put(elements, 0, elements.length);
		QtJambi_LibraryUtilities.internal.truncateBuffer(result, data);
		return result;
	}
    
    /**
     * Returns a QVector containing given elements.
     *
     * @param elements
     * @return a {@code QVector} containing the specified element
     */
    public static @NonNull QVector<@QtPrimitiveType@NonNull Long> ofLong(long @StrictNonNull...elements) {
		QVector<Long> result = new QVector<>(long.class);
    	result.resize(elements.length);
    	LongBuffer data = QtJambi_LibraryUtilities.internal.mutableDataJ(result);
		data.put(elements, 0, elements.length);
		QtJambi_LibraryUtilities.internal.truncateBuffer(result, data);
		return result;
	}
    
    /**
     * Returns a QVector containing given elements.
     *
     * @param elements
     * @return a {@code QVector} containing the specified element
     */
    public static @NonNull QVector<@QtPrimitiveType@NonNull Character> ofChar(char @StrictNonNull...elements) {
		QVector<Character> result = new QVector<>(char.class);
    	result.resize(elements.length);
    	CharBuffer data = QtJambi_LibraryUtilities.internal.mutableDataC(result);
		data.put(elements, 0, elements.length);
		QtJambi_LibraryUtilities.internal.truncateBuffer(result, data);
		return result;
	}
    
    /**
     * Returns a QVector containing given elements.
     *
     * @param elements
     * @return a {@code QVector} containing the specified element
     */
    public static @NonNull QVector<@QtPrimitiveType@NonNull Float> ofFloat(float @StrictNonNull...elements) {
		QVector<Float> result = new QVector<>(float.class);
    	result.resize(elements.length);
    	FloatBuffer data = QtJambi_LibraryUtilities.internal.mutableDataF(result);
		data.put(elements, 0, elements.length);
		QtJambi_LibraryUtilities.internal.truncateBuffer(result, data);
		return result;
	}
    
    /**
     * Returns a QVector containing given elements.
     *
     * @param elements
     * @return a {@code QVector} containing the specified element
     */
    public static @NonNull QVector<@QtPrimitiveType@NonNull Double> ofDouble(double @StrictNonNull...elements) {
		QVector<Double> result = new QVector<>(double.class);
    	result.resize(elements.length);
    	DoubleBuffer data = QtJambi_LibraryUtilities.internal.mutableDataD(result);
		data.put(elements, 0, elements.length);
		QtJambi_LibraryUtilities.internal.truncateBuffer(result, data);
		return result;
	}
    
    /**
     * Returns a QVector containing given elements.
     *
     * @param elements
     * @return a {@code QVector} containing the specified element
     */
    public static @NonNull QVector<@QtPrimitiveType@NonNull Boolean> ofBoolean(boolean @StrictNonNull...elements) {
		QVector<Boolean> result = new QVector<>(boolean.class);
    	result.reserve(elements.length);
		for (boolean t : elements) {
			result.append(t);
		}
		return result;
	}
    
    /**
     * Returns a QVector containing given elements.
     *
     * @param elements
     * @return a {@code QVector} containing the specified element
     */
    public static @NonNull QVector<@QtPrimitiveType@NonNull Byte> ofBuffer(@StrictNonNull ByteBuffer elements) {
		QVector<Byte> result = new QVector<>(byte.class);
		result.resize(elements.remaining());
		ByteBuffer data = QtJambi_LibraryUtilities.internal.mutableDataB(result);
		data.put(elements);
		QtJambi_LibraryUtilities.internal.truncateBuffer(result, data);
		return result;
	}
    
    /**
     * Returns a QVector containing given elements.
     *
     * @param elements
     * @return a {@code QVector} containing the specified element
     */
    public static @NonNull QVector<@QtPrimitiveType@NonNull Short> ofBuffer(@StrictNonNull ShortBuffer elements) {
		QVector<Short> result = new QVector<>(short.class);
		result.resize(elements.remaining());
		ShortBuffer data = QtJambi_LibraryUtilities.internal.mutableDataS(result);
		data.put(elements);
		QtJambi_LibraryUtilities.internal.truncateBuffer(result, data);
		return result;
	}
    
    /**
     * Returns a QVector containing given elements.
     *
     * @param elements
     * @return a {@code QVector} containing the specified element
     */
    public static @NonNull QVector<@QtPrimitiveType@NonNull Integer> ofBuffer(@StrictNonNull IntBuffer elements) {
		QVector<Integer> result = new QVector<>(int.class);
		result.resize(elements.remaining());
		IntBuffer data = QtJambi_LibraryUtilities.internal.mutableDataI(result);
		data.put(elements);
		QtJambi_LibraryUtilities.internal.truncateBuffer(result, data);
		return result;
	}
    
    /**
     * Returns a QVector containing given elements.
     *
     * @param elements
     * @return a {@code QVector} containing the specified element
     */
    public static @NonNull QVector<@QtPrimitiveType@NonNull Long> ofBuffer(@StrictNonNull LongBuffer elements) {
		QVector<Long> result = new QVector<>(long.class);
		result.resize(elements.remaining());
		LongBuffer data = QtJambi_LibraryUtilities.internal.mutableDataJ(result);
		data.put(elements);
		QtJambi_LibraryUtilities.internal.truncateBuffer(result, data);
		return result;
	}
    
    /**
     * Returns a QVector containing given elements.
     *
     * @param elements
     * @return a {@code QVector} containing the specified element
     */
    public static @NonNull QVector<@QtPrimitiveType@NonNull Character> ofBuffer(@StrictNonNull CharBuffer elements) {
		QVector<Character> result = new QVector<>(char.class);
		result.resize(elements.remaining());
		CharBuffer data = QtJambi_LibraryUtilities.internal.mutableDataC(result);
		data.put(elements);
		QtJambi_LibraryUtilities.internal.truncateBuffer(result, data);
		return result;
	}
    
    /**
     * Returns a QVector containing given elements.
     *
     * @param elements
     * @return a {@code QVector} containing the specified element
     */
    public static @NonNull QVector<@QtPrimitiveType@NonNull Float> ofBuffer(@StrictNonNull FloatBuffer elements) {
		QVector<Float> result = new QVector<>(float.class);
		result.resize(elements.remaining());
		FloatBuffer data = QtJambi_LibraryUtilities.internal.mutableDataF(result);
		data.put(elements);
		QtJambi_LibraryUtilities.internal.truncateBuffer(result, data);
		return result;
	}
    
    /**
     * Returns a QVector containing given elements.
     *
     * @param elements
     * @return a {@code QVector} containing the specified element
     */
    public static @NonNull QVector<@QtPrimitiveType@NonNull Double> ofBuffer(@StrictNonNull DoubleBuffer elements) {
		QVector<Double> result = new QVector<>(double.class);
		result.resize(elements.remaining());
		DoubleBuffer data = QtJambi_LibraryUtilities.internal.mutableDataD(result);
		data.put(elements);
		QtJambi_LibraryUtilities.internal.truncateBuffer(result, data);
		return result;
	}
    
    /**
     * Returns a QVector containing given elements.
     *
     * @param <T> the {@code QVector}'s element type
     * @param type the {@code QVector}'s element type
     * @param elements all list elements to be added
     * @return a {@code QVector} containing the specified element
     *
     */
	@SafeVarargs
    public static <T> @NonNull QVector<T> ofTyped(@Nullable Class<? super T> type, T @StrictNonNull...elements) {
		QMetaType metaType = QList.findElementMetaType(type, elements);
		return ofTyped(metaType, elements);
	}
    
    /**
     * Returns a QVector containing given elements.
     *
     * @param <T> the {@code QVector}'s element type
     * @param metaType the {@code QVector}'s element type
     * @param elements all list elements to be added
     * @return a {@code QVector} containing the specified element
     *
     */
	@SafeVarargs
    public static <T> @NonNull QVector<T> ofTyped(@StrictNonNull QMetaType metaType, T @StrictNonNull...elements) {
		if(metaType.id()==0)
			throw new IllegalArgumentException("QMetaType::UnknownType cannot be type of QVector.");
		if(metaType.id()==QMetaType.Type.Void.value())
			throw new IllegalArgumentException("void cannot be type of QVector.");
		return new QVector<>(metaType, Arrays.asList(elements));
	}
	
	/**
     * <p>See <code>QVector::<wbr>operator=(QVector&lt;T>)</code></p>
     */
    @QtUninvokable
    public final void assign(@StrictNonNull QVector<T> other) {
		assign(QtJambi_LibraryUtilities.internal.nativeId(this), other, QtJambi_LibraryUtilities.internal.nativeId(other));
    }
    
    @QtUninvokable
    private native void assign(long __this__nativeId, Object container, long other);
    
    /**
     * <p>See <code>QVector::<wbr>swap(QVector&lt;T>&amp;)</code></p>
     */
    @QtUninvokable
    public final void swap(@StrictNonNull QVector<T> other) {
    	swap(QtJambi_LibraryUtilities.internal.nativeId(this), other, QtJambi_LibraryUtilities.internal.nativeId(other));
    }
    
    @QtUninvokable
    private native void swap(long __this__nativeId, Object container, long other);
	
	/**
     * Returns true of both containers share the same data. 
     */
    @QtUninvokable
    public final boolean isSharedWith(@StrictNonNull QVector<?> other) {
		return isSharedWith(QtJambi_LibraryUtilities.internal.nativeId(this), QtJambi_LibraryUtilities.internal.nativeId(other));
	}
	@QtUninvokable
    private native boolean isSharedWith(long __this__nativeId, long other);

	/**
     * Returns true if container is not shared. 
     */
    @QtUninvokable
    public final boolean isDetached() {
		return isDetached(QtJambi_LibraryUtilities.internal.nativeId(this));
	}
	@QtUninvokable
    private native boolean isDetached(long __this__nativeId);
	
	/**
     * Detached the container if it is shared.
     */
    @QtUninvokable
    public final void detach() {
    	detach(QtJambi_LibraryUtilities.internal.nativeId(this));
	}
	@QtUninvokable
    private native boolean detach(long __this__nativeId);
}
