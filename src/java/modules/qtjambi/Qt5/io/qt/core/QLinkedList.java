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
 * <p>Java wrapper for Qt class <a href="https://doc.qt.io/qt/qlinkedlist.html">QLinkedList</a></p>
 */
public class QLinkedList<T> extends AbstractSequentialContainer<T> implements Deque<T>, Cloneable
{

	static {
    	QtJambi_LibraryUtilities.initialize();
    }

    /**
     * Constructor for internal use only.
     * @param p expected to be <code>null</code>.
     */
    @NativeAccess
    protected QLinkedList(QPrivateConstructor p) {
        super(p);
    }
    
    public QLinkedList(QMetaType.Type metaType) {
		this(new QMetaType(metaType));
	}
    
    public QLinkedList(Class<T> elementType) {
		super(null);
		QMetaType metaType = QMetaType.fromType(elementType);
		initialize(elementType, QtJambi_LibraryUtilities.internal.nativeId(metaType), null);
	}
    
	public QLinkedList(QMetaType metaType) {
		super(null);
		initialize(metaType.javaType(), QtJambi_LibraryUtilities.internal.nativeId(metaType), null);
	}
    
    public QLinkedList(Collection<T> other) {
		super(null);
		QMetaType metaType = QList.findElementMetaType(Objects.requireNonNull(other));
		initialize(metaType.javaType(), QtJambi_LibraryUtilities.internal.nativeId(metaType), other);
    }
    
    /**
     * Creating a container of type QVariant.
     */
    public static QLinkedList<Object> createVariantLinkedList(){
    	return new QLinkedList<>(new QMetaType(QMetaType.Type.QVariant));
    }

    @QtUninvokable
    private native void initialize(Class<?> elementType, long elementMetaType, Collection<T> other);
    
    /**
     * Creates and returns a copy of this object.
     */
    @Override
	public QLinkedList<T> clone(){
		return new QLinkedList<>(this);
	}
    
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
    private static native <T> void append(long __this__nativeId, T t);

    @QtUninvokable
    public final void clear() {
        clear(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private static native <T> void clear(long __this__nativeId);

    @QtUninvokable
    public final boolean contains(Object t) {
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
    public final int count() {
        return size();
    }

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
    private static native <T> int count(long __this__nativeId, T t);

    @QtUninvokable
    protected final QSequentialIterator<T> begin() {
        return begin(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private static native <T> QSequentialIterator<T> begin(long __this__nativeId);

    @QtUninvokable
    protected final QSequentialIterator<T> end() {
        return end(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private static native <T> QSequentialIterator<T> end(long __this__nativeId);

    @QtUninvokable
    protected final QSequentialConstIterator<T> constBegin() {
        return constBegin(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private static native <T> QSequentialConstIterator<T> constBegin(long __this__nativeId);

    @QtUninvokable
    protected final QSequentialConstIterator<T> constEnd() {
        return constEnd(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private static native <T> QSequentialConstIterator<T> constEnd(long __this__nativeId);

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
    private static native <T> boolean endsWith(long __this__nativeId, T t);

    @QtUninvokable
    public final T first() {
        try{
			return first(QtJambi_LibraryUtilities.internal.nativeId(this));
		} catch (IndexOutOfBoundsException e) {
			throw new NoSuchElementException();
		}
    }
    @QtUninvokable
    private static native <T> T first(long __this__nativeId);

    @QtUninvokable
    public final boolean isEmpty() {
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

    @QtUninvokable
    private final boolean operator_equal(java.util.Collection<T> l) {
        return operator_equal(QtJambi_LibraryUtilities.internal.nativeId(this), l);
    }
    @QtUninvokable
    private static native <T> boolean operator_equal(long __this__nativeId, java.util.Collection<T> l);

    @QtUninvokable
    public final void prepend(T t) {
        try{
        	prepend(QtJambi_LibraryUtilities.internal.nativeId(this), t);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QSet.handleException(e, elementMetaType(), t);
    	}
    }
    @QtUninvokable
    private static native <T> void prepend(long __this__nativeId, T t);

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
    public final T removeFirst() {
    	T value = first();
        removeFirst(QtJambi_LibraryUtilities.internal.nativeId(this));
        return value;
    }
    @QtUninvokable
    private static native void removeFirst(long __this__nativeId);

    @QtUninvokable
    public final T removeLast() {
    	T value = last();
        removeLast(QtJambi_LibraryUtilities.internal.nativeId(this));
        return value;
    }
    @QtUninvokable
    private static native void removeLast(long __this__nativeId);
	
	/**
     * <p>See <a href="https://doc.qt.io/qt/qlinkedlist.html#removeIf" class="member-name-class">QLinkedList::<wbr>removeIf(Predicate)</a></p>
	 */
    @QtUninvokable
    public final boolean removeIf(Predicate<? super T> predicate)    {
        Objects.requireNonNull(predicate);
        boolean removed = false;
        for(T value : clone()) {
            if (predicate.test(value)) {
                removeOne(value);
                removed = true;
            }
        }
        return removed;
    }

    @QtUninvokable
    public final boolean removeOne(T t) {
        try{
        	return removeOne(QtJambi_LibraryUtilities.internal.nativeId(this), t);
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
    private static native <T> boolean removeOne(long __this__nativeId, T t);

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
    public final T takeFirst() {
        return takeFirst(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private static native <T> T takeFirst(long __this__nativeId);

    @QtUninvokable
    public final T takeLast() {
        return takeLast(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private static native <T> T takeLast(long __this__nativeId);

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qlinkedlist.html#operator-eq-eq" class="member-name-class">QLinkedList::<wbr>operator==(QLinkedList&lt;T>)const</a></code></p>
     */
    @SuppressWarnings({ "rawtypes", "unchecked" })
    @Override
    @QtUninvokable
    public boolean equals(Object other) {
        if (other instanceof java.util.Collection && QList.checkContainerType(this.elementMetaType(), (java.util.Collection<?>) other)) {
            return operator_equal((java.util.Collection) other);
        }
        return false;
    }


    /**
     * Returns the objects's hash code computed by <code>qHash(QLinkedList&lt;T>)</code>.
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
    public final boolean add(T e){
        if(e!=null){
            append(e);
            return true;
        }else
            return false;
    }

    @SuppressWarnings("unchecked")
	@Override
    @QtUninvokable
    public final boolean remove(Object e){
        return removeOne((T)e);
    }

	@Override
    @QtUninvokable
	public final T peekFirst() {
		return first();
	}

	@Override
    @QtUninvokable
	public final T peekLast() {
		return last();
	}

	@Override
    @QtUninvokable
	public final void push(T e) {
		this.append(e);
	}

	@Override
    @QtUninvokable
	public final T pop() {
		return removeFirst();
	}

    /**
     * <p>See <code>operator&lt;&lt;(QDataStream&amp;,<wbr>QLinkedList&lt;T>)</code></p>
     */
    @io.qt.QtUninvokable
    public void writeTo(io.qt.core.QDataStream stream){
        writeTo(QtJambi_LibraryUtilities.internal.nativeId(this), QtJambi_LibraryUtilities.internal.nativeId(stream));
    }
    
    @io.qt.QtUninvokable
    private native void writeTo(long __this__nativeId, long stream);
    
    /**
     * <p>See <code>operator&gt;&gt;(QDataStream&amp;,<wbr>QLinkedList&lt;T>&amp;)</code></p>
     */
    @io.qt.QtUninvokable
    public void readFrom(io.qt.core.QDataStream stream){
        readFrom(QtJambi_LibraryUtilities.internal.nativeId(this), QtJambi_LibraryUtilities.internal.nativeId(stream));
    }
    
    @io.qt.QtUninvokable
    private native void readFrom(long __this__nativeId, long stream);
    
    @io.qt.QtUninvokable
    final QMetaType elementMetaType() {
    	return elementMetaType(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @io.qt.QtUninvokable
    private static native QMetaType elementMetaType(long containerAccess);
	
	@Override
    @QtUninvokable
	public final T remove() {
		return removeFirst();
	}

	@Override
    @QtUninvokable
	public final void addFirst(T e) {
		push(e);
	}

	@Override
    @QtUninvokable
	public final void addLast(T e) {
		add(e);
	}

	@Override
    @QtUninvokable
	public final boolean offerFirst(T e) {
		push(e);
		return true;
	}

	@Override
    @QtUninvokable
	public final boolean offerLast(T e) {
		add(e);
		return true;
	}
	
	@Override
    @QtUninvokable
	public final T pollFirst() {
		if(size()==0)
			return null;
		return pop();
	}
	
	@Override
    @QtUninvokable
	public final T pollLast() {
		if(size()==0)
			return null;
		T e = peekLast();
		removeLast();
		return e;
	}

	@Override
    @QtUninvokable
	public final T getFirst() {
		if(size()==0)
			throw new NoSuchElementException();
		return peekFirst();
	}

	@Override
    @QtUninvokable
	public final T getLast() {
		if(size()==0)
			throw new NoSuchElementException();
		return peekLast();
	}

	@Override
    @QtUninvokable
	public final T poll() {
		return pollFirst();
	}
	
	@Override
    @QtUninvokable
	public final T element() {
		return getFirst();
	}
	
	@Override
    @QtUninvokable
	public final T peek() {
		return peekFirst();
	}
	
	@Override
    @QtUninvokable
	public final boolean offer(T e) {
		return offerLast(e);
	}

	@Override
    @QtUninvokable
	public final boolean removeFirstOccurrence(Object o) {
		return remove(o);
	}

	@Override
    @QtUninvokable
	public final boolean removeLastOccurrence(Object o) {
		return false;
	}

	@Override
    @QtUninvokable
	public final java.util.Iterator<T> descendingIterator() {
		return constEnd().toJavaDescendingIterator();
	}
    
    /**
     * Returns a QLinkedList containing given elements.
     *
     * @param <T> the {@code QLinkedList}'s element type
     * @param element0 the first element
     * @param elements subsequent elements
     * @return a {@code QLinkedList} containing the specified element
     * @throws NullPointerException if elements are {@code null}
     */
    @SafeVarargs
    public static <T> QLinkedList<T> of(T element0, T...elements) {
        QMetaType metaType = QList.findElementMetaType(element0, elements);
        QLinkedList<T> result = new QLinkedList<>(metaType);
        result.add(element0);
        for (T t : elements) {
            result.add(t);
        }
        return result;
    }
}
