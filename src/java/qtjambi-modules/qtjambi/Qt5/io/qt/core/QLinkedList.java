/****************************************************************************
**
** Copyright (C) 2009-2021 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import java.util.Collection;
import java.util.Deque;
import java.util.NoSuchElementException;

import io.qt.QNoNativeResourcesException;
import io.qt.QtUninvokable;
import io.qt.NativeAccess;

/**
 * <p>Java wrapper for Qt class <a href="https://doc.qt.io/qt/qlinkedlist.html">QLinkedList</a></p>
 */
public class QLinkedList<T> extends io.qt.internal.QtJambiCollectionObject<T> implements Deque<T>, Cloneable
{

	static {
    	QtJambi_LibraryUtilities.initialize();
    }

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
		if(metaType.id()==0)
			throw new IllegalArgumentException("QMetaType::UnknownType cannot be type of QLinkedList.");
		if(metaType.id()==QMetaType.Type.Void.value())
			throw new IllegalArgumentException("void cannot be type of QLinkedList.");
		initialize(elementType, QtJambi_LibraryUtilities.internal.nativeId(metaType), null);
	}
    
	public QLinkedList(QMetaType metaType) {
		super(null);
		if(metaType.id()==0)
			throw new IllegalArgumentException("QMetaType::UnknownType cannot be type of QLinkedList.");
		if(metaType.id()==QMetaType.Type.Void.value())
			throw new IllegalArgumentException("void cannot be type of QLinkedList.");
		initialize(metaType.javaType(), QtJambi_LibraryUtilities.internal.nativeId(metaType), null);
	}
    
    public QLinkedList(Collection<T> other) {
		super(null);
		QMetaType metaType = QList.findElementMetaType(other);
		if(metaType==null || metaType.id()==0)
			throw new IllegalArgumentException("QMetaType::UnknownType cannot be type of QLinkedList.");
		if(metaType.id()==QMetaType.Type.Void.value())
			throw new IllegalArgumentException("void cannot be type of QLinkedList.");
		initialize(metaType.javaType(), QtJambi_LibraryUtilities.internal.nativeId(metaType), other);
    }
    
    public static QLinkedList<Object> createVariantLinkedList(){
    	return new QLinkedList<>(new QMetaType(QMetaType.Type.QVariant));
    }

    @QtUninvokable
    private native void initialize(Class<?> elementType, long elementMetaType, Collection<T> other);
    
    @Override
	public QLinkedList<T> clone(){
		return new QLinkedList<>(this);
	}
    
	@QtUninvokable
    public final void append(T t)    {
		try {
			__qt_QLinkedList_append(QtJambi_LibraryUtilities.internal.nativeId(this), t);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QSet.handleException(e, elementMetaType(), t);
    	}
    }
    @QtUninvokable
    private static native <T> void __qt_QLinkedList_append(long __this__nativeId, T t);

    @QtUninvokable
    protected final QIterator<T> begin()    {
        return __qt_QLinkedList_begin(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private static native <T> QIterator<T> __qt_QLinkedList_begin(long __this__nativeId);

    @QtUninvokable
    public final void clear()    {
        __qt_QLinkedList_clear(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private static native <T> void __qt_QLinkedList_clear(long __this__nativeId);

    @QtUninvokable
    public final boolean contains(Object t)    {
        try{
        	return __qt_QLinkedList_contains(QtJambi_LibraryUtilities.internal.nativeId(this), t);
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
    private static native <T> boolean __qt_QLinkedList_contains(long __this__nativeId, T t);

    @QtUninvokable
    public final int count()    {
        return size();
    }

    @QtUninvokable
    public final int count(T t)    {
        try{
        	return __qt_QLinkedList_count(QtJambi_LibraryUtilities.internal.nativeId(this), t);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QSet.handleException(e, elementMetaType(), t);
    	}
    }
    @QtUninvokable
    private static native <T> int __qt_QLinkedList_count(long __this__nativeId, T t);

    @QtUninvokable
    protected final QIterator<T> end()    {
        return __qt_QLinkedList_end(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private static native <T> QIterator<T> __qt_QLinkedList_end(long __this__nativeId);

    @QtUninvokable
    public final boolean endsWith(T t)    {
        try{
        	return __qt_QLinkedList_endsWith(QtJambi_LibraryUtilities.internal.nativeId(this), t);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QSet.handleException(e, elementMetaType(), t);
        }
    }
    @QtUninvokable
    private static native <T> boolean __qt_QLinkedList_endsWith(long __this__nativeId, T t);

    @QtUninvokable
    public final T first()    {
		if(size()==0)
			throw new NoSuchElementException();
        return __qt_QLinkedList_first(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private static native <T> T __qt_QLinkedList_first(long __this__nativeId);

    @QtUninvokable
    public final boolean isEmpty()    {
        return size()==0;
    }

    @QtUninvokable
    public final T last()    {
		if(size()==0)
			throw new NoSuchElementException();
        return __qt_QLinkedList_last(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private static native <T> T __qt_QLinkedList_last(long __this__nativeId);

    @QtUninvokable
    private final boolean operator_equal(java.util.Collection<T> l)    {
        return __qt_QLinkedList_operator_equal(QtJambi_LibraryUtilities.internal.nativeId(this), l);
    }
    @QtUninvokable
    private static native <T> boolean __qt_QLinkedList_operator_equal(long __this__nativeId, java.util.Collection<T> l);

    @QtUninvokable
    public final void prepend(T t)    {
        try{
        	__qt_QLinkedList_prepend(QtJambi_LibraryUtilities.internal.nativeId(this), t);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QSet.handleException(e, elementMetaType(), t);
    	}
    }
    @QtUninvokable
    private static native <T> void __qt_QLinkedList_prepend(long __this__nativeId, T t);

    @QtUninvokable
    public final int removeAll(T t)    {
    	try {
    		return __qt_QLinkedList_removeAll(QtJambi_LibraryUtilities.internal.nativeId(this), t);
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
    private static native <T> int __qt_QLinkedList_removeAll(long __this__nativeId, T t);

    @QtUninvokable
    public final T removeFirst()    {
    	T value = first();
        __qt_QLinkedList_removeFirst(QtJambi_LibraryUtilities.internal.nativeId(this));
        return value;
    }
    @QtUninvokable
    private static native void __qt_QLinkedList_removeFirst(long __this__nativeId);

    @QtUninvokable
    public final T removeLast()    {
    	T value = last();
        __qt_QLinkedList_removeLast(QtJambi_LibraryUtilities.internal.nativeId(this));
        return value;
    }
    @QtUninvokable
    private static native void __qt_QLinkedList_removeLast(long __this__nativeId);

    @QtUninvokable
    public final boolean removeOne(T t)    {
        try{
        	return __qt_QLinkedList_removeOne(QtJambi_LibraryUtilities.internal.nativeId(this), t);
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
    private static native <T> boolean __qt_QLinkedList_removeOne(long __this__nativeId, T t);

    @QtUninvokable
    public final int size()    {
        return __qt_QLinkedList_size(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private static native <T> int __qt_QLinkedList_size(long __this__nativeId);

    @QtUninvokable
    public final boolean startsWith(T t)    {
        try{
        	return __qt_QLinkedList_startsWith(QtJambi_LibraryUtilities.internal.nativeId(this), t);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QSet.handleException(e, elementMetaType(), t);
        }
    }
    @QtUninvokable
    private static native <T> boolean __qt_QLinkedList_startsWith(long __this__nativeId, T t);

    @QtUninvokable
    public final T takeFirst()    {
        return __qt_QLinkedList_takeFirst(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private static native <T> T __qt_QLinkedList_takeFirst(long __this__nativeId);

    @QtUninvokable
    public final T takeLast()    {
        return __qt_QLinkedList_takeLast(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private static native <T> T __qt_QLinkedList_takeLast(long __this__nativeId);

    @SuppressWarnings({ "rawtypes", "unchecked" })
    @Override
    @QtUninvokable
    public boolean equals(Object other) {
        if (other instanceof java.util.Collection && QList.checkContainerType(this.elementMetaType(), (java.util.Collection<?>) other)) {
            return operator_equal((java.util.Collection) other);
        }
        return false;
    }


    @Override
    @QtUninvokable
    public int hashCode() { 
    	int hashCode = getClass().hashCode();
        for (T e : this)
            hashCode = 31*hashCode + (e==null ? 0 : e.hashCode());
        return hashCode;
    }
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

    @io.qt.QtUninvokable
    public void writeTo(io.qt.core.QDataStream stream){
        __qt_QLinkedList_writeTo(QtJambi_LibraryUtilities.internal.nativeId(this), QtJambi_LibraryUtilities.internal.nativeId(stream));
    }
    
    @io.qt.QtUninvokable
    private native void __qt_QLinkedList_writeTo(long __this__nativeId, long stream);
    
    @io.qt.QtUninvokable
    public void readFrom(io.qt.core.QDataStream stream){
        __qt_QLinkedList_readFrom(QtJambi_LibraryUtilities.internal.nativeId(this), QtJambi_LibraryUtilities.internal.nativeId(stream));
    }
    
    @io.qt.QtUninvokable
    private native void __qt_QLinkedList_readFrom(long __this__nativeId, long stream);
    
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
		return super.descendingIterator();
	}
    
    /**
     * Returns a QLinkedList containing given elements.
     *
     * @param <E> the {@code QLinkedList}'s element type
     * @param element0 the first element
     * @param elements subsequent elements
     * @return a {@code QLinkedList} containing the specified element
     * @throws NullPointerException if elements are {@code null}
     */
    @SafeVarargs
    public static <T> QLinkedList<T> of(T element0, T...elements) {
        QMetaType metaType = QList.findElementMetaType(element0, elements);
        if(metaType==null || metaType.id()==0)
            throw new IllegalArgumentException("QMetaType::UnknownType cannot be type of QLinkedList.");
        if(metaType.id()==QMetaType.Type.Void.value())
            throw new IllegalArgumentException("void cannot be type of QLinkedList.");
        QLinkedList<T> result = new QLinkedList<>(metaType);
        result.add(element0);
        for (T t : elements) {
            result.add(t);
        }
        return result;
    }
}
