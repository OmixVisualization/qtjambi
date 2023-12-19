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
package io.qt.core;

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
    
    public QVector(QMetaType.Type metaType) {
		this(new QMetaType(metaType));
	}
    
    public QVector(Class<T> elementType) {
		super(null);
		QMetaType metaType = QMetaType.fromType(elementType);
		initialize(elementType, QtJambi_LibraryUtilities.internal.nativeId(metaType), null);
	}
    
	public QVector(QMetaType metaType) {
		super(null);
		initialize(metaType.javaType(), QtJambi_LibraryUtilities.internal.nativeId(metaType), null);
	}
    
    public QVector(Collection<T> other) {
		super(null);
		QMetaType metaType = QList.findElementMetaType(Objects.requireNonNull(other));
		initialize(metaType.javaType(), QtJambi_LibraryUtilities.internal.nativeId(metaType), other);
    }
    
    /**
     * Creating a container of type QVariant.
     */
    public static QVector<Object> createVariantVector(){
        return new QVector<>(new QMetaType(QMetaType.Type.QVariant));
    }

    @QtUninvokable
    private native void initialize(Class<?> elementType, long elementMetaType, Collection<T> other);
    
    /**
     * Creates and returns a copy of this object.
     */
    @Override
    public QVector<T> clone(){
        return new QVector<>(this);
    }
    
    @QtUninvokable
    private static native void onDispose(long listAccess);

    @QtUninvokable
    public final void append(java.util.Collection<T> t)    {
        appendVector(QtJambi_LibraryUtilities.internal.nativeId(this), t);
    }
    @QtUninvokable
    private static native <T> void appendVector(long __this__nativeId, java.util.Collection<T> t);

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
    private static native <T> void append(long __this__nativeId, T t);

    @QtUninvokable
    public final T at(int i)    {
        return at(QtJambi_LibraryUtilities.internal.nativeId(this), i);
    }
    @QtUninvokable
    private static native <T> T at(long __this__nativeId, int i);

    @QtUninvokable
    protected final QSequentialIterator<T> begin()    {
        return begin(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private static native <T> QSequentialIterator<T> begin(long __this__nativeId);

    @QtUninvokable
    protected final QSequentialIterator<T> end()    {
        return end(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private static native <T> QSequentialIterator<T> end(long __this__nativeId);

    @QtUninvokable
    protected final QSequentialConstIterator<T> constBegin()    {
        return constBegin(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private static native <T> QSequentialConstIterator<T> constBegin(long __this__nativeId);

    @QtUninvokable
    protected final QSequentialConstIterator<T> constEnd()    {
        return constEnd(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private static native <T> QSequentialConstIterator<T> constEnd(long __this__nativeId);

    @QtUninvokable
    public final int capacity()    {
        return capacity(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private static native <T> int capacity(long __this__nativeId);

    @QtUninvokable
    public final void clear()    {
        clear(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private static native <T> void clear(long __this__nativeId);

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
    private static native <T> boolean contains(long __this__nativeId, T t);

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
    private static native <T> int count(long __this__nativeId, T t);

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
    private static native <T> boolean endsWith(long __this__nativeId, T t);

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
    private static native <T> void fill(long __this__nativeId, T t, int size);

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
    private static native <T> int indexOf(long __this__nativeId, T t, int from);

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
    private static native <T> void insert(long __this__nativeId, int i, int n, T t);

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
    private static native <T> T last(long __this__nativeId);

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
    private static native <T> int lastIndexOf(long __this__nativeId, T t, int from);

    @QtUninvokable
    public final int length()    {
        return size();
    }

    @QtUninvokable
    public final QVector<T> mid(int pos) {
        return mid(pos, (int)-1);
    }
    @QtUninvokable
    public final QVector<T> mid(int pos, int length)    {
        return mid(QtJambi_LibraryUtilities.internal.nativeId(this), pos, length);
    }
    @QtUninvokable
    private static native <T> QVector<T> mid(long __this__nativeId, int pos, int length);

    @QtUninvokable
    public final void move(int from, int to)    {
        move(QtJambi_LibraryUtilities.internal.nativeId(this), from, to);
    }
    @QtUninvokable
    private static native <T> void move(long __this__nativeId, int from, int to);

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
    private static native void remove(long __this__nativeId, int i, int n);

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
    private static native <T> int removeAll(long __this__nativeId, T t);

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
    private static native <T> boolean removeOne(long __this__nativeId, T t);

	/**
     * <p>See <a href="https://doc.qt.io/qt/qvector.html#removeIf" class="member-name-class">QVector::<wbr>removeIf(Predicate)</a></p>
	 */
    @QtUninvokable
    public final boolean removeIf(Predicate<? super T> predicate)    {
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
    private static native <T> void replace(long __this__nativeId, int i, T t);

    @QtUninvokable
    public final void reserve(int size) {
        reserve(QtJambi_LibraryUtilities.internal.nativeId(this), size);
    }
    @QtUninvokable
    private static native <T> void reserve(long __this__nativeId, int size);

    @QtUninvokable
    public final void resize(int size) {
        resize(QtJambi_LibraryUtilities.internal.nativeId(this), size);
    }
    @QtUninvokable
    private static native void resize(long __this__nativeId, int size);

    @QtUninvokable
    public final void squeeze() {
        squeeze(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private static native void squeeze(long __this__nativeId);

    @QtUninvokable
    public final int size() {
        return size(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private static native <T> int size(long __this__nativeId);

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
    private static native <T> boolean startsWith(long __this__nativeId, T t);

    @QtUninvokable
    public final T takeAt(int i) {
        return takeAt(QtJambi_LibraryUtilities.internal.nativeId(this), i);
    }
    @QtUninvokable
    private static native <T> T takeAt(long __this__nativeId, int i);

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
    private static native <T> T takeLast(long __this__nativeId);

    @QtUninvokable
    public final QSet<T> toSet() {
		QSet<T> set = new QSet<>(elementMetaType());
		set.unite(this);
        return set;
    }

    @QtUninvokable
    public final T value(int i) {
        return value(QtJambi_LibraryUtilities.internal.nativeId(this), i);
    }
    @QtUninvokable
    private static native <T> T value(long __this__nativeId, int i);

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
    private static native <T> T valueDefault(long __this__nativeId, int i, T defaultValue);

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
    private static native boolean operator_equal(long __this__nativeId, java.util.Collection<?> l);

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
    private static native int hashCode(long __this__nativeId);
    
    @Override
    @QtUninvokable
    public String toString() {
    	try {
			return toString(QtJambi_LibraryUtilities.internal.nativeId(this));
		} catch (QNoNativeResourcesException e) {
			return "null";
		}
    }
    @QtUninvokable
    private static native String toString(long __this__nativeId);
    
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
    private static native <T> void swapItemsAt(long __this__nativeId, int i, int j);
    
    /**
     * <p>See <code>operator&lt;&lt;(QDataStream&amp;,<wbr>QVector&lt;T>)</code></p>
     */
    @io.qt.QtUninvokable
    public void writeTo(io.qt.core.QDataStream stream){
        writeTo(QtJambi_LibraryUtilities.internal.nativeId(this), QtJambi_LibraryUtilities.internal.nativeId(stream));
    }
    
    @io.qt.QtUninvokable
    private native void writeTo(long __this__nativeId, long stream);
    
    /**
     * <p>See <code>operator&gt;&gt;(QDataStream&amp;,<wbr>QVector&lt;T>&amp;)</code></p>
     */
    @io.qt.QtUninvokable
    public void readFrom(io.qt.core.QDataStream stream){
        readFrom(QtJambi_LibraryUtilities.internal.nativeId(this), QtJambi_LibraryUtilities.internal.nativeId(stream));
    }
    
    @io.qt.QtUninvokable
    private native void readFrom(long __this__nativeId, long stream);
    
    @io.qt.QtUninvokable
    QMetaType elementMetaType() {
    	return elementMetaType(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @io.qt.QtUninvokable
    private static native QMetaType elementMetaType(long containerAccess);
    
    /**
     * Returns a QVector containing given elements.
     *
     * @param <T> the {@code QVector}'s element type
     * @param element0 the first element
     * @param elements subsequent elements
     * @return a {@code QVector} containing the specified element
     * @throws NullPointerException if elements are {@code null}
     */
    @SafeVarargs
    public static <T> QVector<T> of(T element0, T...elements) {
        QMetaType metaType = QList.findElementMetaType(element0, elements);
        QVector<T> result = new QVector<>(metaType);
        result.add(element0);
        for (T t : elements) {
            result.add(t);
        }
        return result;
    }
}
