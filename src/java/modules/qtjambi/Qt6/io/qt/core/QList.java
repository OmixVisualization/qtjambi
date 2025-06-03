/****************************************************************************
**
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
package io.qt.core;

import java.lang.reflect.*;
import java.nio.*;
import java.util.*;
import java.util.function.*;
import io.qt.*;

/**
 * <p>Java wrapper for Qt class <code><a href="https://doc.qt.io/qt/qlist.html">QList</a></code></p>
 */
public class QList<T> extends AbstractList<T> implements Cloneable
{

    static {
        QtJambi_LibraryUtilities.initialize();
    }
    
    /**
     * Constructor for internal use only.
     * @param p expected to be <code>null</code>.
     * @hidden
     */
    @NativeAccess
    protected QList(QPrivateConstructor p) {
        super(p);
    }
    
    /**
     * Creating a container with given element type.
     * <p>See <code><a href="https://doc.qt.io/qt/qlist.html#QList">QList::<wbr>QList()</a></code></p>
     * @param metaType the type T
     */
    public QList(QMetaType.@StrictNonNull Type metaType) {
		this(new QMetaType(metaType));
	}
	
	/**
     * Creating a container with given element type and values.
     * <p>See <code><a href="https://doc.qt.io/qt/qlist.html#QList">QList::<wbr>QList()</a></code></p>
     * @param metaType the type T
     */
	@SafeVarargs
	public QList(QMetaType.@StrictNonNull Type metaType, T @StrictNonNull...elements) {
		this(new QMetaType(metaType), elements);
	}
    
    /**
     * Creating a container with given element type.
     * <p>See <code><a href="https://doc.qt.io/qt/qlist.html#QList">QList::<wbr>QList()</a></code></p>
     * @param elementType the type T
     */
    public QList(@Nullable Class<T> elementType) {
		super(null);
		QMetaType metaType = QMetaType.fromType(elementType);
		initialize(elementType, QtJambi_LibraryUtilities.internal.nativeId(metaType), null);
	}
    
    /**
     * Creating a container with given element type and values.
     * <p>See <code><a href="https://doc.qt.io/qt/qlist.html#QList">QList::<wbr>QList()</a></code></p>
     * @param elementType the type T
     */
    @SafeVarargs
	public QList(@Nullable Class<T> elementType, T @StrictNonNull...elements) {
		super(null);
		QMetaType metaType = QMetaType.fromType(elementType);
		initialize(elementType, QtJambi_LibraryUtilities.internal.nativeId(metaType), elements.length==0 ? null : Arrays.asList(elements));
	}
    
    /**
     * Creating a container with given element type.
     * <p>See <code><a href="https://doc.qt.io/qt/qlist.html#QList">QList::<wbr>QList()</a></code></p>
     * @param metaType the type T
     */
	public QList(@StrictNonNull QMetaType metaType) {
		super(null);
		initialize(metaType.javaType(), QtJambi_LibraryUtilities.internal.nativeId(metaType), null);
	}
	
	/**
     * Creating a container with given element type and values.
     * <p>See <code><a href="https://doc.qt.io/qt/qlist.html#QList">QList::<wbr>QList()</a></code></p>
     * @param metaType the type T
     */
	@SafeVarargs
	public QList(@StrictNonNull QMetaType metaType, T @StrictNonNull...elements) {
		super(null);
		initialize(metaType.javaType(), QtJambi_LibraryUtilities.internal.nativeId(metaType), elements.length==0 ? null : Arrays.asList(elements));
	}
    
    /**
     * Creating a container with given element type and size.
     * <p>See <code><a href="https://doc.qt.io/qt/qlist.html#QList-1">QList::<wbr>QList(qsizetype)</a></code></p>
     * @param metaType the type T
     */
    public QList(QMetaType.@StrictNonNull Type metaType, int size) {
		this(new QMetaType(metaType), size);
	}
    
    /**
     * Creating a container with given element type and size.
     * <p>See <code><a href="https://doc.qt.io/qt/qlist.html#QList-1">QList::<wbr>QList(qsizetype)</a></code></p>
     * @param elementType the type T
     */
    public QList(@Nullable Class<T> elementType, int size) {
		this(QMetaType.fromType(elementType), size);
	}
    
    /**
     * Creating a container with given element type and size.
     * <p>See <code><a href="https://doc.qt.io/qt/qlist.html#QList-1">QList::<wbr>QList(qsizetype)</a></code></p>
     * @param metaType the type T
     */
	public QList(@StrictNonNull QMetaType metaType, int size) {
		this(metaType);
		resize(size);
	}
    
    /**
     * Creating a container filled with given value.
     * <p>See <code><a href="https://doc.qt.io/qt/qlist.html#QList-2">QList::<wbr>QList(qsizetype,T)</a></code></p>
     */
	public QList(int size, @StrictNonNull T value) {
		this(findElementMetaType(value));
		fill(value, size);
	}
    
    /**
     * Creating a container with given content.
     * <p>See <code><a href="https://doc.qt.io/qt/qlist.html#QList">QList::<wbr>QList(const QList&lt;T> &amp;)</a></code></p>
     * @param other container
     */
    public QList(@StrictNonNull Collection<T> other) {
		super(null);
		QMetaType metaType = findElementMetaType(Objects.requireNonNull(other, "Argument 'other': null not expected."));
		initialize(metaType.javaType(), QtJambi_LibraryUtilities.internal.nativeId(metaType), other);
    }
    
    @NativeAccess
    QList(QMetaType elementMetaType, Collection<T> other) {
		super(null);
		initialize(elementMetaType.javaType(), QtJambi_LibraryUtilities.internal.nativeId(elementMetaType), other);
	}

    /**
     * Creating a container with given content.
     * <p>See <code><a href="https://doc.qt.io/qt/qlist.html#QList">QList::<wbr>QList(InputIterator,InputIterator)</a></code></p>
     * @param iterable
     */
    public QList(@StrictNonNull Iterable<T> iterable) {
		super(null);
		QMetaType metaType;
		if(iterable instanceof AbstractSpan) {
			AbstractSpan<T> span = (AbstractSpan<T>)iterable;
			metaType = span._elementType();
		}else if(iterable instanceof Collection) {
			metaType = findElementMetaType((Collection<?>)iterable);
		}else {
			throw new IllegalArgumentException("Cannot create QList from unknown iterable");
		}
		initialize(metaType.javaType(), QtJambi_LibraryUtilities.internal.nativeId(metaType), iterable);
    }
    
    /**
     * Creating a container of type QVariant.
     */
    public static @NonNull QList<Object> createVariantList(){
    	return new QList<>(new QMetaType(QMetaType.Type.QVariant));
    }
    
    /**
     * Creating a container of type QVariant and given size.
     */
    public static @NonNull QList<Object> createVariantList(int size){
    	return new QList<>(new QMetaType(QMetaType.Type.QVariant), size);
    }

    @QtUninvokable
    private native void initialize(Class<?> elementType, long elementMetaType, Iterable<T> other);
    
    /**
     * Creates and returns a copy of this object.
     * <p>See <code><a href="https://doc.qt.io/qt/qlist.html#QList-8">QList::<wbr>QList(const QList&lt;T> &amp;)</a></code></p>
     */
    @Override
	public @NonNull QList<T> clone(){
		return new QList<>(this);
	}
    
    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qlist.html#append-2">QList::<wbr>append(const QList&lt;T> &amp;)</a></code></p>
     */
    @QtUninvokable
    public final void append(java.util.@NonNull Collection<T> t) {
        appendList(QtJambi_LibraryUtilities.internal.nativeId(this), t);
    }
    @QtUninvokable
    private native void appendList(long __this__nativeId, java.util.Collection<T> t);

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qlist.html#append">QList::<wbr>append(T)</a></code></p>
     */
    @QtUninvokable
    public final void append(T t) {
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
    
    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qlist.html#at">QList::<wbr>at(qsizetype)const</a></code></p>
     */
    @QtUninvokable
    public final T at(int i) {
        return at(QtJambi_LibraryUtilities.internal.nativeId(this), i);
    }
    @QtUninvokable
    private native T at(long __this__nativeId, int i);

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qlist.html#clear">QList::<wbr>clear()</a></code></p>
     */
    @QtUninvokable
    public final void clear() {
        clear(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private native void clear(long __this__nativeId);

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qlist.html#contains">QList::<wbr>contains(T)const</a></code></p>
     */
    @QtUninvokable
    public final boolean contains(Object t) {
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

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qlist.html#count-1">QList::<wbr>count()const</a></code></p>
     */
    @QtUninvokable
    public final int count() {
        return size();
    }

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qlist.html#count">QList::<wbr>count(T)const</a></code></p>
     */
    @QtUninvokable
    public final int count(T t) {
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

    /**
     * Provides a mutable C++ iterator to the containers begin.
     * <p>See <code><a href="https://doc.qt.io/qt/qlist.html#begin">QList::<wbr>begin()</a></code></p>
     * @return begin
     */
    @QtUninvokable
    protected final @NonNull QSequentialIterator<T> begin() {
        return begin(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private native QSequentialIterator<T> begin(long __this__nativeId);

    /**
     * Provides a mutable C++ iterator to the containers end.
     * <p>See <code><a href="https://doc.qt.io/qt/qlist.html#end">QList::<wbr>end()</a></code></p>
     * @return end
     */
    @QtUninvokable
    protected final @NonNull QSequentialIterator<T> end() {
        return end(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private native QSequentialIterator<T> end(long __this__nativeId);

    /**
     * Provides a constant C++ iterator to the containers begin.
     * <p>See <code><a href="https://doc.qt.io/qt/qlist.html#constBegin">QList::<wbr>constBegin()const</a></code></p>
     * @return begin
     */
    @QtUninvokable
    protected final @NonNull QSequentialConstIterator<T> constBegin() {
        return constBegin(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private native QSequentialConstIterator<T> constBegin(long __this__nativeId);

    /**
     * Provides a constant C++ iterator to the containers end.
     * <p>See <code><a href="https://doc.qt.io/qt/qlist.html#constEnd">QList::<wbr>constEnd()const</a></code></p>
     * @return end
     */
    @QtUninvokable
    protected final @NonNull QSequentialConstIterator<T> constEnd() {
        return constEnd(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private native QSequentialConstIterator<T> constEnd(long __this__nativeId);

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qlist.html#endsWith">QList::<wbr>endsWith(T)const</a></code></p>
     */
    @QtUninvokable
    public final boolean endsWith(T t) {
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

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qlist.html#first">QList::<wbr>first()const</a></code></p>
     */
    @QtUninvokable
    public final T first() {
    	try {
    		return at(0);
		} catch (IndexOutOfBoundsException e) {
			throw new NoSuchElementException();
		}
    }
    
    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qlist.html#indexOf">QList::<wbr>indexOf(T,qsizetype)const</a></code></p>
     */
    @QtUninvokable
    public final int indexOf(T t, int from) {
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

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qlist.html#insert">QList::<wbr>insert(qsizetype,T)</a></code></p>
     */
    @QtUninvokable
    public final void insert(int i, T t) {
    	insert(i, 1, t);
    }

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qlist.html#isEmpty">QList::<wbr>isEmpty()const</a></code></p>
     */
    @QtUninvokable
    public final boolean isEmpty() {
        return size()==0;
    }

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qlist.html#last">QList::<wbr>last()const</a></code></p>
     */
    @QtUninvokable
    public final T last() {
        try {
			return last(QtJambi_LibraryUtilities.internal.nativeId(this));
		} catch (IndexOutOfBoundsException e) {
			throw new NoSuchElementException();
		}
    }
    @QtUninvokable
    private native T last(long __this__nativeId);

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qlist.html#lastIndexOf">QList::<wbr>lastIndexOf(T,qsizetype)const</a></code></p>
     */
    @SuppressWarnings("unchecked")
	@QtUninvokable
    @Override
    public final int lastIndexOf(Object t) {
		return lastIndexOf((T)t, (int)-1);
    }
    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qlist.html#lastIndexOf">QList::<wbr>lastIndexOf(T,qsizetype)const</a></code></p>
     */
    @QtUninvokable
    public final int lastIndexOf(T t, int from) {
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

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qlist.html#length">QList::<wbr>length()const</a></code></p>
     */
    @QtUninvokable
    public final int length() {
        return size();
    }

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qlist.html#mid">QList::<wbr>mid(qsizetype,qsizetype)const</a></code></p>
     */
    @QtUninvokable
    public final @NonNull QList<T> mid(int pos) {
        return mid(pos, (int)-1);
    }

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qlist.html#mid">QList::<wbr>mid(qsizetype,qsizetype)const</a></code></p>
     */
    @QtUninvokable
    public final @NonNull QList<T> mid(int pos, int length) {
        return mid(QtJambi_LibraryUtilities.internal.nativeId(this), pos, length);
    }
    @QtUninvokable
    private native QList<T> mid(long __this__nativeId, int pos, int length);

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qlist.html#move">QList::<wbr>move(qsizetype,qsizetype)</a></code></p>
     */
    @QtUninvokable
    public final void move(int from, int to) {
        move(QtJambi_LibraryUtilities.internal.nativeId(this), from, to);
    }
    @QtUninvokable
    private native void move(long __this__nativeId, int from, int to);

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qlist.html#prepend">QList::<wbr>prepend(T)</a></code></p>
     */
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

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qlist.html#removeAll">QList::<wbr>removeAll(T)</a></code></p>
     */
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
    
    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qlist.html#removeIf">QList::<wbr>removeIf(Predicate)</a></code></p>
	 */
    @QtUninvokable
    public final boolean removeIf(@StrictNonNull Predicate<? super T> predicate)    {
        Objects.requireNonNull(predicate, "Argument 'predicate': null not expected.");
        boolean removed = false;
        for(T value : clone()) {
            if (predicate.test(value)) {
                removeAll(value);
                removed = true;
            }
        }
        return removed;
    }

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qlist.html#removeAt">QList::<wbr>removeAt(qsizetype)</a></code></p>
     */
    @QtUninvokable
    public final void removeAt(int i) {
    	remove(i, 1);
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

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qlist.html#removeOne">QList::<wbr>removeOne(T)</a></code></p>
     */
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
     * <p>See <code><a href="https://doc.qt.io/qt/qlist.html#replace">QList::<wbr>replace(qsizetype,T)</a></code></p>
     */
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

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qlist.html#reserve">QList::<wbr>reserve(qsizetype)</a></code></p>
     */
    @QtUninvokable
    public final void reserve(int size) {
        reserve(QtJambi_LibraryUtilities.internal.nativeId(this), size);
    }
    @QtUninvokable
    private native void reserve(long __this__nativeId, int size);

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qlist.html#size">QList::<wbr>size()const</a></code></p>
     */
    @QtUninvokable
    public final int size() {
        return size(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private native int size(long __this__nativeId);

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qlist.html#startsWith">QList::<wbr>startsWith(T)const</a></code></p>
     */
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

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qlist.html#takeAt">QList::<wbr>takeAt(qsizetype)</a></code></p>
     */
    @QtUninvokable
    public final T takeAt(int i) {
        return takeAt(QtJambi_LibraryUtilities.internal.nativeId(this), i);
    }
    @QtUninvokable
    private native T takeAt(long __this__nativeId, int i);

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qlist.html#takeFirst">QList::<wbr>takeFirst()</a></code></p>
     */
    @QtUninvokable
    public final T takeFirst() {
    	try {
    		return takeAt(0);
		} catch (IndexOutOfBoundsException e) {
			throw new NoSuchElementException();
		}
    }
    
    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qlist.html#takeLast">QList::<wbr>takeLast()</a></code></p>
     */
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

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qlist.html#value">QList::<wbr>value(qsizetype)</a></code></p>
     */
    @QtUninvokable
    public final T value(int i) {
        return value(QtJambi_LibraryUtilities.internal.nativeId(this), i);
    }
    @QtUninvokable
    private native T value(long __this__nativeId, int i);

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qlist.html#value-1">QList::<wbr>value(qsizetype,T)</a></code></p>
     */
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
     * <p>See <code><a href="https://doc.qt.io/qt/qlist.html#operator-eq-eq">QList::<wbr>operator==(QList&lt;T>)const</a></code></p>
     */
    @Override
    @QtUninvokable
    public boolean equals(Object other) {
    	if(other instanceof StackDeque<?>) {
			other = ((StackDeque<?>)other).stack;
		}
    	if (other instanceof java.util.Collection && checkContainerType(this.elementMetaType(), (java.util.Collection<?>) other)) {
            return operator_equal(QtJambi_LibraryUtilities.internal.nativeId(this), (java.util.Collection<?>) other);
        }
        return false;
    }
    @QtUninvokable
    private native boolean operator_equal(long __this__nativeId, java.util.Collection<?> l);

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
    
    /**
     * Returns the string representation of the object given by <code>QVariant(this).toString()</code>.
     */
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
    
    public static java.nio.@NonNull ByteBuffer asByteBuffer(@StrictNonNull QList<@NonNull@QtPrimitiveType Byte> list){
		return asBuffer(java.nio.ByteBuffer.class, list);
	}
	
	public static java.nio.@NonNull ShortBuffer asShortBuffer(@StrictNonNull QList<@NonNull@QtPrimitiveType Short> list){
		return asBuffer(java.nio.ShortBuffer.class, list);
	}
	
	public static java.nio.@NonNull IntBuffer asIntBuffer(@StrictNonNull QList<@NonNull@QtPrimitiveType Integer> list){
		return asBuffer(java.nio.IntBuffer.class, list);
	}
	
	public static java.nio.@NonNull LongBuffer asLongBuffer(@StrictNonNull QList<@NonNull@QtPrimitiveType Long> list){
		return asBuffer(java.nio.LongBuffer.class, list);
	}
	
	public static java.nio.@NonNull CharBuffer asCharBuffer(@StrictNonNull QList<@NonNull@QtPrimitiveType Character> list){
		return asBuffer(java.nio.CharBuffer.class, list);
	}
	
	public static java.nio.@NonNull FloatBuffer asFloatBuffer(@StrictNonNull QList<@NonNull@QtPrimitiveType Float> list){
		return asBuffer(java.nio.FloatBuffer.class, list);
	}
	
	public static java.nio.@NonNull DoubleBuffer asDoubleBuffer(@StrictNonNull QList<@NonNull@QtPrimitiveType Double> list){
		return asBuffer(java.nio.DoubleBuffer.class, list);
	}
    
	private native static <B extends java.nio.Buffer> B asBuffer(Class<B> bufferClass, long nativeId, QList<?> list);
	
	static <B extends java.nio.Buffer> B asBuffer(Class<B> bufferClass, QList<?> list) {
		return asBuffer(bufferClass, QtJambi_LibraryUtilities.internal.nativeId(list), list);
	}
	
	public static byte @NonNull[] asByteArray(@StrictNonNull QList<@NonNull@QtPrimitiveType Byte> list){
		return asArray(byte[].class, list);
	}
	
	public static short @NonNull[] asShortArray(@StrictNonNull QList<@NonNull@QtPrimitiveType Short> list){
		return asArray(short[].class, list);
	}
	
	public static int @NonNull[] asIntArray(@StrictNonNull QList<@NonNull@QtPrimitiveType Integer> list){
		return asArray(int[].class, list);
	}
	
	public static long @NonNull[] asLongArray(@StrictNonNull QList<@NonNull@QtPrimitiveType Long> list){
		return asArray(long[].class, list);
	}
	
	public static char @NonNull[] asCharArray(@StrictNonNull QList<@NonNull@QtPrimitiveType Character> list){
		return asArray(char[].class, list);
	}
	
	public static float @NonNull[] asFloatArray(@StrictNonNull QList<@NonNull@QtPrimitiveType Float> list){
		return asArray(float[].class, list);
	}
	
	public static double @NonNull[] asDoubleArray(@StrictNonNull QList<@NonNull@QtPrimitiveType Double> list){
		return asArray(double[].class, list);
	}
	
	public static boolean @NonNull[] asBooleanArray(@StrictNonNull QList<@NonNull@QtPrimitiveType Boolean> list){
		return asArray(boolean[].class, list);
	}
	
	private static <A> A asArray(Class<A> arrayClass, QList<?> list) {
    	Class<?> expectedClass = arrayClass.getComponentType();
    	char c;
    	if(expectedClass==long.class)
    		c = 'J';
    	else if(expectedClass==boolean.class)
    		c = 'Z';
    	else
    		c = Character.toUpperCase(expectedClass.getSimpleName().charAt(0));
    	QMetaType elementMetaType = list.elementMetaType();
    	if(elementMetaType.javaType()!=expectedClass || !expectedClass.isPrimitive())
    		throw new IllegalArgumentException(String.format("Cannot convert QSpan<%1$s> to %2$s[]", elementMetaType.name(), expectedClass.getTypeName()));
    	return arrayClass.cast(asArray(QtJambi_LibraryUtilities.internal.nativeId(list), c));
    }
	
	private native static <A> A asArray(long nativeId, char c);
	
	public static <T> T @NonNull[] asObjectArray(@StrictNonNull QList<@NonNull@QtPrimitiveType T> list){
		return list.toArray();
	}
    
    /**
     * Appends the specified element to the end of this list.
     * @see List#add(Object)
     */
    @Override
    @QtUninvokable
    public boolean add(T e){
        append(e);
        return true;
    }

    /**
     * Inserts the specified element at the specified position in this list.
     * @see List#add(int, Object)
     */
    @Override
    @QtUninvokable
    public final void add(int index, T e){
            insert(index, e);
    }

    /**
     * Replaces the element at the specified position in this list with the
     * specified element.
     * @see List#set(int, Object)
     */
    @Override
    @QtUninvokable
    public final T set(int index, T e){
        if(e!=null){
            T el = at(index);
            replace(index, e);
            return el;
        } else return null;
    }
    
    /**
     * Removes the first occurrence of the specified element from this list,
     * if it is present.
     * @see List#remove(Object)
     */
    @SuppressWarnings("unchecked")
	@Override
    @QtUninvokable
    public final boolean remove(Object e){
        return removeOne((T)e);
    }

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qlist.html#indexOf">QList::<wbr>indexOf(T,qsizetype)const</a></code></p>
     */
    @SuppressWarnings("unchecked")
	@Override
    @QtUninvokable
    public final int indexOf(Object e){
        return indexOf((T)e, (int)0);
    }

    /**
     * Returns the element at the specified position in this list.
     * @see List#get(int)
     */
    @Override
    @QtUninvokable
    public final T get(int index) {
        return at(index);
    }
    
    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qlist.html#swapItemsAt">QList::<wbr>swapItemsAt(qsizetype,qsizetype)</a></code></p>
     */
    @QtUninvokable
    public final void swapItemsAt(int i, int j) {
        swapItemsAt(QtJambi_LibraryUtilities.internal.nativeId(this), i, j);
    }
    
    @QtUninvokable
    private native void swapItemsAt(long __this__nativeId, int i, int j);
    
    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qlist.html#fill">QList::<wbr>fill(T,qsizetype)</a></code></p>
     */
    @QtUninvokable
    public final void fill(T t) {
        fill(t, (int)-1);
    }
    
    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qlist.html#fill">QList::<wbr>fill(T,qsizetype)</a></code></p>
     */
    @QtUninvokable
    public final void fill(T t, int size) {
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

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qlist.html#insert-1">QList::<wbr>insert(qsizetype,qsizetype,T)</a></code></p>
     */
    @QtUninvokable
    public final void insert(int i, int n, T t) {
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

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qlist.html#remove">QList::<wbr>remove(qsizetype,qsizetype)</a></code></p>
     */
    @QtUninvokable
    public final void remove(int i, int n) {
        remove(QtJambi_LibraryUtilities.internal.nativeId(this), i, n);
    }
    @QtUninvokable
    private native void remove(long __this__nativeId, int i, int n);

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qlist.html#capacity">QList::<wbr>capacity()const</a></code></p>
     */
    @QtUninvokable
    public final int capacity() {
        return capacity(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private native int capacity(long __this__nativeId);

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qlist.html#resize">QList::<wbr>resize(qsizetype)</a></code></p>
     */
    @QtUninvokable
    public final void resize(int size) {
        resize(QtJambi_LibraryUtilities.internal.nativeId(this), size);
    }
    @QtUninvokable
    private native void resize(long __this__nativeId, int size);

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qlist.html#squeeze">QList::<wbr>squeeze()</a></code></p>
     */
    @QtUninvokable
    public final void squeeze() {
        squeeze(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private native void squeeze(long __this__nativeId);
    
    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qlist.html#operator-lt-lt">operator&lt;&lt;(QDataStream&amp;,<wbr>QList&lt;T>)</a></code></p>
     */
    @io.qt.QtUninvokable
    public void writeTo(io.qt.core.@StrictNonNull QDataStream stream){
    	java.util.Objects.requireNonNull(stream, "Argument 'stream': null not expected.");
        writeTo(QtJambi_LibraryUtilities.internal.nativeId(this), QtJambi_LibraryUtilities.internal.nativeId(stream));
    }
    
    @io.qt.QtUninvokable
    private native void writeTo(long __this__nativeId, long stream);
    
    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qlist.html#operator-gt-gt">operator&gt;&gt;(QDataStream&amp;,<wbr>QList&lt;T>&amp;)</a></code></p>
     */
    @io.qt.QtUninvokable
    public void readFrom(io.qt.core.@StrictNonNull QDataStream stream){
    	java.util.Objects.requireNonNull(stream, "Argument 'stream': null not expected.");
        readFrom(QtJambi_LibraryUtilities.internal.nativeId(this), QtJambi_LibraryUtilities.internal.nativeId(stream));
    }
    
    @io.qt.QtUninvokable
    private native void readFrom(long __this__nativeId, long stream);
    
    @io.qt.QtUninvokable
    final QMetaType elementMetaType() {
        return elementMetaType(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @io.qt.QtUninvokable
    private native QMetaType elementMetaType(long __this_nativeId);
    
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
	@SafeVarargs
    public static <T> @NonNull QList<T> of(T element0, T @StrictNonNull...elements) {
		QMetaType metaType = findElementMetaType(element0, elements);
        if(metaType.id()==QMetaType.Type.QString.value()) {
        	@SuppressWarnings("unchecked")
			QList<T> result = (QList<T>)(QList<?>)new QStringList();
        	result.reserve(elements.length+1);
        	result.append(element0);
        	for (T t : elements) {
        		result.append(t);
        	}
        	return result;
        }else {
    		@SuppressWarnings("unchecked")
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
    	result.resize(elements.length);
    	ByteBuffer data = QtJambi_LibraryUtilities.internal.mutableDataB(result);
		data.put(elements, 0, elements.length);
		QtJambi_LibraryUtilities.internal.truncateBuffer(result, data);
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
    	result.resize(elements.length);
    	ShortBuffer data = QtJambi_LibraryUtilities.internal.mutableDataS(result);
		data.put(elements, 0, elements.length);
		QtJambi_LibraryUtilities.internal.truncateBuffer(result, data);
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
    	result.resize(elements.length);
    	IntBuffer data = QtJambi_LibraryUtilities.internal.mutableDataI(result);
		data.put(elements, 0, elements.length);
		QtJambi_LibraryUtilities.internal.truncateBuffer(result, data);
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
    	result.resize(elements.length);
    	LongBuffer data = QtJambi_LibraryUtilities.internal.mutableDataJ(result);
		data.put(elements, 0, elements.length);
		QtJambi_LibraryUtilities.internal.truncateBuffer(result, data);
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
    	result.resize(elements.length);
    	CharBuffer data = QtJambi_LibraryUtilities.internal.mutableDataC(result);
		data.put(elements, 0, elements.length);
		QtJambi_LibraryUtilities.internal.truncateBuffer(result, data);
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
    	result.resize(elements.length);
    	FloatBuffer data = QtJambi_LibraryUtilities.internal.mutableDataF(result);
		data.put(elements, 0, elements.length);
		QtJambi_LibraryUtilities.internal.truncateBuffer(result, data);
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
    	result.resize(elements.length);
    	DoubleBuffer data = QtJambi_LibraryUtilities.internal.mutableDataD(result);
		data.put(elements, 0, elements.length);
		QtJambi_LibraryUtilities.internal.truncateBuffer(result, data);
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
    	result.reserve(elements.length);
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
		result.resize(elements.remaining());
		ByteBuffer data = QtJambi_LibraryUtilities.internal.mutableDataB(result);
		data.put(elements);
		QtJambi_LibraryUtilities.internal.truncateBuffer(result, data);
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
		result.resize(elements.remaining());
		ShortBuffer data = QtJambi_LibraryUtilities.internal.mutableDataS(result);
		data.put(elements);
		QtJambi_LibraryUtilities.internal.truncateBuffer(result, data);
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
		result.resize(elements.remaining());
		IntBuffer data = QtJambi_LibraryUtilities.internal.mutableDataI(result);
		data.put(elements);
		QtJambi_LibraryUtilities.internal.truncateBuffer(result, data);
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
		result.resize(elements.remaining());
		LongBuffer data = QtJambi_LibraryUtilities.internal.mutableDataJ(result);
		data.put(elements);
		QtJambi_LibraryUtilities.internal.truncateBuffer(result, data);
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
		result.resize(elements.remaining());
		CharBuffer data = QtJambi_LibraryUtilities.internal.mutableDataC(result);
		data.put(elements);
		QtJambi_LibraryUtilities.internal.truncateBuffer(result, data);
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
		result.resize(elements.remaining());
		FloatBuffer data = QtJambi_LibraryUtilities.internal.mutableDataF(result);
		data.put(elements);
		QtJambi_LibraryUtilities.internal.truncateBuffer(result, data);
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
		result.resize(elements.remaining());
		DoubleBuffer data = QtJambi_LibraryUtilities.internal.mutableDataD(result);
		data.put(elements);
		QtJambi_LibraryUtilities.internal.truncateBuffer(result, data);
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
    public static <T> @NonNull QList<T> ofTyped(@Nullable Class<? super T> type, T @StrictNonNull...elements) {
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
	
	static String pairPrefix() {
    	return "std::pair";
    }

    static boolean checkContainerType(QMetaType elementType, java.util.Collection<?> container) {
    	if (container instanceof QList && elementType.equals(((QList<?>) container).elementMetaType())) {
			return true;
		} else if (container instanceof QSet && elementType.equals(((QSet<?>) container).elementMetaType())) {
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
		if(type==null)
			return new QMetaType(QMetaType.Type.Nullptr);
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
}
