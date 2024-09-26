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

import java.lang.reflect.*;
import java.nio.*;
import java.util.*;
import java.util.function.*;
import io.qt.*;

/**
 * <p>Java wrapper for Qt class <a href="https://doc.qt.io/qt/qlist.html">QList</a></p>
 */
public class QList<T> extends AbstractList<T> implements java.util.List<T>, Cloneable
{
    static {
        QtJambi_LibraryUtilities.initialize();
    }
    
    /**
     * Constructor for internal use only.
     * @param p expected to be <code>null</code>.
     */
    @NativeAccess
    protected QList(QPrivateConstructor p) {
        super(p);
    }
    
    /**
     * Creating a container with given element type.
     * <p>See <a href="https://doc.qt.io/qt/qlist.html#QList">QList::QList()</a></p>
     * @param metaType the type T
     */
    public QList(QMetaType.@StrictNonNull Type metaType) {
		this(new QMetaType(metaType));
	}
    
    /**
     * Creating a container with given element type.
     * <p>See <a href="https://doc.qt.io/qt/qlist.html#QList">QList::QList()</a></p>
     * @param elementType the type T
     */
    public QList(@Nullable Class<T> elementType) {
        super(null);
        QMetaType metaType = QMetaType.fromType(elementType);
        initialize(elementType, QtJambi_LibraryUtilities.internal.nativeId(metaType), null);
    }
    
    /**
     * Creating a container with given element type.
     * <p>See <a href="https://doc.qt.io/qt/qlist.html#QList">QList::QList()</a></p>
     * @param metaType the type T
     */
    public QList(@StrictNonNull QMetaType metaType) {
        super(null);
        initialize(metaType.javaType(), QtJambi_LibraryUtilities.internal.nativeId(metaType), null);
    }
    
    @NativeAccess
    QList(QMetaType elementMetaType, Collection<T> other) {
		super(null);
		initialize(elementMetaType.javaType(), QtJambi_LibraryUtilities.internal.nativeId(elementMetaType), other);
	}
    
    /**
     * Creating a container with given content.
     * <p>See <a href="https://doc.qt.io/qt/qlist.html#QList">QList::QList(const QList&lt;T> &amp;)</a></p>
     * @param other container
     */
    public QList(@StrictNonNull Collection<T> other) {
        super(null);
        QMetaType metaType = findElementMetaType(Objects.requireNonNull(other));
        initialize(metaType.javaType(), QtJambi_LibraryUtilities.internal.nativeId(metaType), other);
    }
    
    /**
     * Creating a container of type QVariant.
     */
    public static QList<Object> createVariantList(){
        return new QList<>(new QMetaType(QMetaType.Type.QVariant));
    }

    @QtUninvokable
    private native void initialize(Class<?> elementType, long elementMetaType, Collection<T> other);
	
	QList(@StrictNonNull QMetaType metaType, int size) {
        super(null);
		throw new RuntimeException("This constructor is required for compatibility with AbstractSpan.");
    }
    
    /**
     * Creates and returns a copy of this object.
     * <p>See <a href="https://doc.qt.io/qt/qlist.html#QList-8">QList::QList(const QList&lt;T> &amp;)</a></p>
     */
    @Override
    public @NonNull QList<T> clone(){
        return new QList<>(this);
    }
    
    @QtUninvokable
    public final void append(java.util.@NonNull Collection<T> t) {
        appendList(QtJambi_LibraryUtilities.internal.nativeId(this), Objects.requireNonNull(t));
    }
    @QtUninvokable
    private native void appendList(long __this__nativeId, java.util.Collection<T> t);

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
    public final void clear()    {
        clear(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private native void clear(long __this__nativeId);

    @QtUninvokable
    public final boolean contains(Object t)    {
        try {
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
        try {
            insert(QtJambi_LibraryUtilities.internal.nativeId(this), i, 1, t);
        }catch(IndexOutOfBoundsException | QNoNativeResourcesException e) {
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
        return lastIndexOf((T)t, (int)-1);
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
    public final int length() {
        return size();
    }

    @QtUninvokable
    public final @NonNull QList<T> mid(int pos) {
        return mid(pos, (int)-1);
    }
    @QtUninvokable
    public final @NonNull QList<T> mid(int pos, int length) {
        return mid(QtJambi_LibraryUtilities.internal.nativeId(this), pos, length);
    }
    @QtUninvokable
    private native QList<T> mid(long __this__nativeId, int pos, int length);

    @QtUninvokable
    public final void move(int from, int to) {
        move(QtJambi_LibraryUtilities.internal.nativeId(this), from, to);
    }
    @QtUninvokable
    private native void move(long __this__nativeId, int from, int to);

    @QtUninvokable
    private final boolean operator_equal(java.util.Collection<T> l) {
        return operator_equal(QtJambi_LibraryUtilities.internal.nativeId(this), l);
    }
    @QtUninvokable
    private native boolean operator_equal(long __this__nativeId, java.util.Collection<T> l);

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
        remove(QtJambi_LibraryUtilities.internal.nativeId(this), i, 1);
    }
	
    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qlist.html#removeFirst">QList::<wbr>removeFirst()</a></code></p>
     */
    @QtUninvokable
    public final T removeFirst() {
		return takeFirst();
    }

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qlist.html#removeFirst">QList::<wbr>removeLast()</a></code></p>
     */
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
     * <p>See <a href="https://doc.qt.io/qt/qlist.html#removeIf" class="member-name-class">QList::<wbr>removeIf(Predicate)</a></p>
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
    private native void remove(long __this__nativeId, int i, int n);

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
     * <p>See <code><a href="https://doc.qt.io/qt/qlist.html#operator-eq-eq" class="member-name-class">QList::<wbr>operator==(QList&lt;T>)const</a></code></p>
     */
    @SuppressWarnings({ "rawtypes", "unchecked" })
    @Override
    @QtUninvokable
    public boolean equals(Object other) {
        if (other instanceof java.util.Collection && checkContainerType(this.elementMetaType(), (java.util.Collection<?>) other)) {
            return operator_equal((java.util.Collection) other);
        }
        return false;
    }

    /**
     * Returns the objects's hash code computed by <code>qHash(QList&lt;T>)</code>.
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
		} catch (QNoImplementationException e) {
			return super.toString();
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
        return indexOf((T)e, (int)0);
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
    
    @QtUninvokable
    public final @NonNull QSet<T> toSet() {
		QSet<T> set = new QSet<>(elementMetaType());
		set.unite(this);
        return set;
    }
    
    /**
     * <p>See <code>operator&lt;&lt;(QDataStream&amp;,<wbr>QList&lt;T>)</code></p>
     */
    @io.qt.QtUninvokable
    public void writeTo(io.qt.core.@StrictNonNull QDataStream stream){
    	java.util.Objects.requireNonNull(stream, "Argument 'stream': null not expected.");
        writeTo(QtJambi_LibraryUtilities.internal.nativeId(this), QtJambi_LibraryUtilities.internal.nativeId(stream));
    }
    
    @io.qt.QtUninvokable
    private native void writeTo(long __this__nativeId, long stream);
    
    /**
     * <p>See <code>operator&gt;&gt;(QDataStream&amp;,<wbr>QList&lt;T>&amp;)</code></p>
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
     * Returns a QList containing given elements.
     *
     * @param <T> the {@code QList}'s element type
     * @param element0 the first element
     * @param elements subsequent elements
     * @return a {@code QList} containing the specified element
     * @throws NullPointerException if elements are {@code null}
     *
     */
    @SuppressWarnings({ "unchecked" })
	@SafeVarargs
    public static <T> @NonNull QList<T> of(T element0, T @StrictNonNull...elements) {
		QMetaType metaType = findElementMetaType(element0, elements);
        if(metaType.id()==QMetaType.Type.QString.value()) {
        	QList<T> result = (QList<T>)(QList<?>)new QStringList();
        	result.append(element0);
        	for (T t : elements) {
        		result.append(t);
        	}
        	return result;
        }else {
    		T[] allElements = (T[])new Object[elements.length+1];
    		System.arraycopy(elements, 0, allElements, 1, elements.length);
    		allElements[0] = element0;
    		return ofTyped(metaType, allElements);
        }
	}
    
    /**
     * Returns a QStringList containing given elements.
     *
     * @param element0 the first element
     * @param elements subsequent elements
     * @return a {@code QStringList} containing the specified element
     */
    @SafeVarargs
    public static @NonNull QStringList of(@NonNull String element0, @NonNull String @StrictNonNull...elements) {
    	String[] allElements = new String[elements.length+1];
		System.arraycopy(elements, 0, allElements, 1, elements.length);
		allElements[0] = (String)element0;
    	return new QStringList(allElements);
    }
    
    /**
     * Returns a QList containing given elements.
     *
     * @param elements
     * @return a {@code QList} containing the specified element
     */
    public static @NonNull QList<@QtPrimitiveType@NonNull Byte> ofByte(byte @StrictNonNull...elements) {
		QList<Byte> result = new QList<>(byte.class);
    	for (byte t : elements) {
			result.append(t);
		}
		return result;
	}
    
    /**
     * Returns a QList containing given elements.
     *
     * @param elements
     * @return a {@code QList} containing the specified element
     */
    public static @NonNull QList<@QtPrimitiveType@NonNull Short> ofShort(short @StrictNonNull...elements) {
		QList<Short> result = new QList<>(short.class);
    	for (short t : elements) {
			result.append(t);
		}
		return result;
	}
    
    /**
     * Returns a QList containing given elements.
     *
     * @param elements
     * @return a {@code QList} containing the specified element
     */
    public static @NonNull QList<@QtPrimitiveType@NonNull Integer> ofInt(int @StrictNonNull...elements) {
		QList<Integer> result = new QList<>(int.class);
    	for (int t : elements) {
			result.append(t);
		}
		return result;
	}
    
    /**
     * Returns a QList containing given elements.
     *
     * @param elements
     * @return a {@code QList} containing the specified element
     */
    public static @NonNull QList<@QtPrimitiveType@NonNull Long> ofLong(long @StrictNonNull...elements) {
		QList<Long> result = new QList<>(long.class);
    	for (long t : elements) {
			result.append(t);
		}
		return result;
	}
    
    /**
     * Returns a QList containing given elements.
     *
     * @param elements
     * @return a {@code QList} containing the specified element
     */
    public static @NonNull QList<@QtPrimitiveType@NonNull Character> ofChar(char @StrictNonNull...elements) {
		QList<Character> result = new QList<>(char.class);
    	for (char t : elements) {
			result.append(t);
		}
		return result;
	}
    
    /**
     * Returns a QList containing given elements.
     *
     * @param elements
     * @return a {@code QList} containing the specified element
     */
    public static @NonNull QList<@QtPrimitiveType@NonNull Float> ofFloat(float @StrictNonNull...elements) {
		QList<Float> result = new QList<>(float.class);
    	for (float t : elements) {
			result.append(t);
		}
		return result;
	}
    
    /**
     * Returns a QList containing given elements.
     *
     * @param elements
     * @return a {@code QList} containing the specified element
     */
    public static @NonNull QList<@QtPrimitiveType@NonNull Double> ofDouble(double @StrictNonNull...elements) {
		QList<Double> result = new QList<>(double.class);
    	for (double t : elements) {
			result.append(t);
		}
		return result;
	}
    
    /**
     * Returns a QList containing given elements.
     *
     * @param elements
     * @return a {@code QList} containing the specified element
     */
    public static @NonNull QList<@QtPrimitiveType@NonNull Boolean> ofBoolean(boolean @StrictNonNull...elements) {
		QList<Boolean> result = new QList<>(boolean.class);
		for (boolean t : elements) {
			result.append(t);
		}
		return result;
	}
    
    /**
     * Returns a QList containing given elements.
     *
     * @param elements
     * @return a {@code QList} containing the specified element
     */
    public static @NonNull QList<@QtPrimitiveType@NonNull Byte> ofBuffer(@StrictNonNull ByteBuffer elements) {
		QList<Byte> result = new QList<>(byte.class);
		while(elements.hasRemaining()) {
			result.append(elements.get());
		}
		return result;
	}
    
    /**
     * Returns a QList containing given elements.
     *
     * @param elements
     * @return a {@code QList} containing the specified element
     */
    public static @NonNull QList<@QtPrimitiveType@NonNull Short> ofBuffer(@StrictNonNull ShortBuffer elements) {
		QList<Short> result = new QList<>(short.class);
		while(elements.hasRemaining()) {
			result.append(elements.get());
		}
		return result;
	}
    
    /**
     * Returns a QList containing given elements.
     *
     * @param elements
     * @return a {@code QList} containing the specified element
     */
    public static @NonNull QList<@QtPrimitiveType@NonNull Integer> ofBuffer(@StrictNonNull IntBuffer elements) {
		QList<Integer> result = new QList<>(int.class);
		while(elements.hasRemaining()) {
			result.append(elements.get());
		}
		return result;
	}
    
    /**
     * Returns a QList containing given elements.
     *
     * @param elements
     * @return a {@code QList} containing the specified element
     */
    public static @NonNull QList<@QtPrimitiveType@NonNull Long> ofBuffer(@StrictNonNull LongBuffer elements) {
		QList<Long> result = new QList<>(long.class);
		while(elements.hasRemaining()) {
			result.append(elements.get());
		}
		return result;
	}
    
    /**
     * Returns a QList containing given elements.
     *
     * @param elements
     * @return a {@code QList} containing the specified element
     */
    public static @NonNull QList<@QtPrimitiveType@NonNull Character> ofBuffer(@StrictNonNull CharBuffer elements) {
		QList<Character> result = new QList<>(char.class);
		while(elements.hasRemaining()) {
			result.append(elements.get());
		}
		return result;
	}
    
    /**
     * Returns a QList containing given elements.
     *
     * @param elements
     * @return a {@code QList} containing the specified element
     */
    public static @NonNull QList<@QtPrimitiveType@NonNull Float> ofBuffer(@StrictNonNull FloatBuffer elements) {
		QList<Float> result = new QList<>(float.class);
		while(elements.hasRemaining()) {
			result.append(elements.get());
		}
		return result;
	}
    
    /**
     * Returns a QList containing given elements.
     *
     * @param elements
     * @return a {@code QList} containing the specified element
     */
    public static @NonNull QList<@QtPrimitiveType@NonNull Double> ofBuffer(@StrictNonNull DoubleBuffer elements) {
		QList<Double> result = new QList<>(double.class);
		while(elements.hasRemaining()) {
			result.append(elements.get());
		}
		return result;
	}
    
    /**
     * Returns a QList containing given elements.
     *
     * @param <T> the {@code QList}'s element type
     * @param type the {@code QList}'s element type
     * @param elements all list elements to be added
     * @return a {@code QList} containing the specified element
     *
     */
	@SafeVarargs
    public static <T> @NonNull QList<T> ofTyped(@StrictNonNull Class<? super T> type, T @StrictNonNull...elements) {
		QMetaType metaType = findElementMetaType(type, elements);
		return ofTyped(metaType, elements);
	}
    
    /**
     * Returns a QList containing given elements.
     *
     * @param <T> the {@code QList}'s element type
     * @param metaType the {@code QList}'s element type
     * @param elements all list elements to be added
     * @return a {@code QList} containing the specified element
     *
     */
    @SuppressWarnings({ "unchecked" })
	@SafeVarargs
    public static <T> @NonNull QList<T> ofTyped(@StrictNonNull QMetaType metaType, T @StrictNonNull...elements) {
		if(metaType.id()==QMetaType.Type.QString.value()) {
			QList<T> result = (QList<T>)(QList<?>)new QStringList();
	    	result.reserve(elements.length);
			for (T t : elements) {
				result.append(t);
			}
			return result;
		}else {
			if(metaType.id()==0)
				throw new IllegalArgumentException("QMetaType::UnknownType cannot be type of QList.");
			if(metaType.id()==QMetaType.Type.Void.value())
				throw new IllegalArgumentException("void cannot be type of QList.");
			return new QList<>(metaType, Arrays.asList(elements));
		}
	}
	
	/**
     * Returns true of both containers share the same data. 
     */
    @QtUninvokable
    public final boolean isSharedWith(@StrictNonNull QList<?> other) {
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
	
	static <B extends java.nio.Buffer> B asBuffer(Class<B> bufferClass, QList<?> list) {
		return null;
	}

    static String pairPrefix() {
    	return "QPair";
    }

    static boolean checkContainerType(QMetaType elementType, java.util.Collection<?> container) {
        if (container instanceof QList && elementType.equals(((QList<?>) container).elementMetaType())) {
            return true;
        } else if (container instanceof QSet && elementType.equals(((QSet<?>) container).elementMetaType())) {
            return true;
        } else if (container instanceof QVector && elementType.equals(((QVector<?>) container).elementMetaType())) {
            return true;
        } else if (container instanceof QLinkedList && elementType.equals(((QLinkedList<?>) container).elementMetaType())) {
            return true;
        } else {
            Class<?> type = elementType.javaType();
            if (type.isPrimitive()) {
                if (type == int.class)
                    type = Integer.class;
                if (type == short.class)
                    type = Short.class;
                if (type == byte.class)
                    type = Byte.class;
                if (type == long.class)
                    type = Long.class;
                if (type == float.class)
                    type = Float.class;
                if (type == double.class)
                    type = Double.class;
                if (type == boolean.class)
                    type = Boolean.class;
                if (type == char.class)
                    type = Character.class;
            }
            for (Object entry : container) {
                if (entry != null && !type.isInstance(entry))
                    return false;
            }
        }
        return true;
    }
    
    @SafeVarargs
    static QMetaType findElementMetaType(Object element0, Object... elements){
        QMetaType result = getMetaType(element0);
        Class<?> type = element0==null ? null : QtJambi_LibraryUtilities.internal.getClass(element0);
        if(type==null && elements!=null) {
        	type = QtJambi_LibraryUtilities.internal.getClass(elements).getComponentType();
        	if(type==Object.class)
        		type = null;
        }
        for(Object e : elements) {
            if(result!=null) {
                QMetaType _result = getMetaType(e);
                if(!result.isValid() || result.id()==QMetaType.Type.Nullptr.value())
                    result = _result;
                if(!_result.equals(result)) {
                    result = null;
                }
            }
            if(e!=null) {
                if(type==null) {
                    type = QtJambi_LibraryUtilities.internal.getClass(e);
                }else {
                    Class<?> type2 = QtJambi_LibraryUtilities.internal.getClass(e);
                    if(type2!=type) {
                        if(type2.isAssignableFrom(type)) {
                            type = type2;
                        }else if(!type.isAssignableFrom(type2)) {
                            do{
                                type = type.getSuperclass();
                            }while(!type.isAssignableFrom(type2));
                        }
                    }
                }
            }
        }
        if(result==null)
            result = QMetaType.fromType(type);
        return result;
    }
    
	static QMetaType findElementMetaType(Class<?> type, Object... elements){
		TypeVariable<?>[] typeParams = type.getTypeParameters();
    	QMetaType result = typeParams==null || typeParams.length==0 ? QMetaType.fromType(type) : new QMetaType();
    	for(Object e : elements) {
    		if(result!=null) {
    			if(e!=null) {
		    		QMetaType _result = getMetaType(e);
		    		if(!result.isValid() || result.id()==QMetaType.Type.Nullptr.value())
		    			result = _result;
		    		if(!_result.equals(result)) {
		    			result = null;
		    		}
	    		}
    		}
    		if(e!=null) {
    			if(type==null) {
    				type = QtJambi_LibraryUtilities.internal.getClass(e);
    			}else {
    				Class<?> type2 = QtJambi_LibraryUtilities.internal.getClass(e);
    				if(type2!=type) {
    					if(type2.isAssignableFrom(type)) {
    						type = type2;
    					}else if(!type.isAssignableFrom(type2)) {
    						do{
    							type = type.getSuperclass();
    						}while(!type.isAssignableFrom(type2));
    					}
    				}
    			}
			}
    	}
    	if(result==null)
    		result = QMetaType.fromType(type);
    	return result;
    }
    
    static QMetaType findElementMetaType(Collection<?> elements){
		Class<?> cls = QtJambi_LibraryUtilities.internal.getClass(elements);
        if(cls==QList.class) {
            return ((QList<?>)elements).elementMetaType();
        }else if(cls==QQueue.class) {
            return ((QQueue<?>)elements).elementMetaType();
        }else if(cls==QStack.class) {
            return ((QStack<?>)elements).elementMetaType();
        }else if(cls==QSet.class) {
            return ((QSet<?>)elements).elementMetaType();
        }else {
            QMetaType result = new QMetaType();
            Class<?> type = null;
            for(Object e : elements) {
                if(result!=null) {
                    QMetaType _result = getMetaType(e);
                    if(!result.isValid() || result.id()==QMetaType.Type.Nullptr.value())
                        result = _result;
                    if(!_result.equals(result)) {
                        result = null;
                    }
                }
                if(e!=null) {
                    if(type==null) {
                        type = QtJambi_LibraryUtilities.internal.getClass(e);
                    }else {
                        Class<?> type2 = QtJambi_LibraryUtilities.internal.getClass(e);
                        if(type2!=type) {
                            if(type2.isAssignableFrom(type)) {
                                type = type2;
                            }else if(!type.isAssignableFrom(type2)) {
                                do{
                                    type = type.getSuperclass();
                                }while(!type.isAssignableFrom(type2));
                            }
                        }
                    }
                }
            }
            if(result==null)
                result = QMetaType.fromType(type);
            return result;
        }
    }
    
    static QMetaType getMetaType(Object obj){
        if(obj==null)
            return new QMetaType(QMetaType.Type.Nullptr);
    	Class<?> objClass = QtJambi_LibraryUtilities.internal.getClass(obj);
        if(objClass==QList.class) {
            return QMetaType.fromType(QList.class, ((QList<?>)obj).elementMetaType());
        }else if(objClass==QQueue.class) {
            return QMetaType.fromType(QQueue.class, ((QQueue<?>)obj).elementMetaType());
        }else if(objClass==QLinkedList.class) {
            return QMetaType.fromType(QLinkedList.class, ((QLinkedList<?>)obj).elementMetaType());
        }else if(objClass==QVector.class) {
            return QMetaType.fromType(QVector.class, ((QVector<?>)obj).elementMetaType());
        }else if(objClass==QStack.class) {
            return QMetaType.fromType(QStack.class, ((QStack<?>)obj).elementMetaType());
        }else if(objClass==QSet.class) {
            return QMetaType.fromType(QSet.class, ((QSet<?>)obj).elementMetaType());
        }else if(objClass==QMultiMap.class) {
            return QMetaType.fromType(QMultiMap.class, ((QMultiMap<?,?>)obj).keyMetaType(), ((QMultiMap<?,?>)obj).valueMetaType());
        }else if(objClass==QMap.class) {
            return QMetaType.fromType(QMap.class, ((QMap<?,?>)obj).keyMetaType(), ((QMap<?,?>)obj).valueMetaType());
        }else if(objClass==QMultiHash.class) {
            return QMetaType.fromType(QMultiHash.class, ((QMultiHash<?,?>)obj).keyMetaType(), ((QMultiHash<?,?>)obj).valueMetaType());
        }else if(objClass==QHash.class) {
            return QMetaType.fromType(QHash.class, ((QHash<?,?>)obj).keyMetaType(), ((QHash<?,?>)obj).valueMetaType());
        }else if(objClass==QPair.class) {
            return QMetaType.fromType(QPair.class, getMetaType(((QPair<?,?>)obj).first), getMetaType(((QPair<?,?>)obj).second));
        }else if(obj instanceof java.util.List) {
            java.util.List<?> list = (java.util.List<?>)obj;
            QMetaType elementMetaType = findElementMetaType(list);
            if(elementMetaType.isValid()) {
                return QMetaType.fromType(QList.class, elementMetaType);
            }
        }else if(obj instanceof java.util.Set) {
            java.util.Set<?> list = (java.util.Set<?>)obj;
            QMetaType elementMetaType = findElementMetaType(list);
            if(elementMetaType.isValid()) {
                return QMetaType.fromType(QSet.class, elementMetaType);
            }
        }else if(obj instanceof java.util.Map) {
            java.util.Map<?,?> map = (java.util.Map<?,?>)obj;
            QPair<QMetaType, QMetaType> metaTypes = QMap.findMapMetaType(map);
            if(metaTypes.first.isValid() && metaTypes.second.isValid()) {
                if(obj instanceof java.util.NavigableMap)
                    return QMetaType.fromType(QMap.class, metaTypes.first, metaTypes.second);
                else
                    return QMetaType.fromType(QHash.class, metaTypes.first, metaTypes.second);
            }
        }
        QMetaType metaType = QMetaType.fromType(objClass);
        return metaType;
    }
	
	/**
     * <p>See <code>QList::<wbr>operator=(QList&lt;T>)</code></p>
     */
    @QtUninvokable
    public final void assign(@StrictNonNull QList<T> other) {
		assign(QtJambi_LibraryUtilities.internal.nativeId(this), other, QtJambi_LibraryUtilities.internal.nativeId(other));
    }
    @QtUninvokable
    private native void assign(long __this__nativeId, Object container, long other);
    
    /**
     * <p>See <code>QList::<wbr>swap(QList&lt;T>&amp;)</code></p>
     */
    @QtUninvokable
    public final void swap(@StrictNonNull QList<T> other) {
    	swap(QtJambi_LibraryUtilities.internal.nativeId(this), other, QtJambi_LibraryUtilities.internal.nativeId(other));
    }
    @QtUninvokable
    private native void swap(long __this__nativeId, Object container, long other);
}
