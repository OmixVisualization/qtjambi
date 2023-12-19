/****************************************************************************
**
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import java.util.*;
import io.qt.*;

/**
 * <p>Java wrapper for Qt class <code><a href="https://doc.qt.io/qt/qstack.html">QStack</a></code></p>
 * <p>QStack will no longer implement Deque interface if compiled with Java 21.
 * Instead call {@link asDeque()} to get a Deque wrapper for the stack.</p>
 */
public class QStack<T> extends QList<T> implements Queue<T>, Deque<T>
{
    /**
     * Creating a container with given element type and size.
     * <p>See <code><a href="https://doc.qt.io/qt/qstack.html">QStack::<wbr>QStack()</a></code></p>
     * @param metaType the type T
     */
    public QStack(@StrictNonNull QMetaType metaType) {
        super(metaType);
    }
    
    /**
     * Creating a container with given element type and size.
     * <p>See <code><a href="https://doc.qt.io/qt/qstack.html">QStack::<wbr>QStack()</a></code></p>
     * @param metaType the type T
     */
    public QStack(QMetaType.@StrictNonNull Type metaType) {
		this(new QMetaType(metaType));
	}

    /**
     * Creating a container with given element type.
     * <p>See <code><a href="https://doc.qt.io/qt/qstack.html">QStack::<wbr>QStack()</a></code></p>
     * @param elementType the type T
     */
    public QStack(@StrictNonNull Class<T> elementType) {
        super(elementType);
    }

    /**
     * Creating a container with given content.
     * <p>See <code><a href="https://doc.qt.io/qt/qstack.html">QStack::<wbr>QStack(const QStack&lt;T> &amp;)</a></code></p>
     * @param other container
     */
    public QStack(@StrictNonNull Collection<T> other) {
        super(other);
    }
    
    @NativeAccess
    private QStack(QMetaType elementMetaType, Collection<T> other) {
        super(elementMetaType, other);
    }
    
    /**
     * Creating a container of type QVariant.
     */
    public static @NonNull QStack<Object> createVariantStack(){
        return new QStack<>(new QMetaType(QMetaType.Type.QVariant));
    }
    
    /**
     * Creates and returns a copy of this object.
     * <p>See <code><a href="https://doc.qt.io/qt/qstack.html">QStack::<wbr>QStack(const QStack&lt;T> &amp;)</a></code></p>
     */
    @Override
    public @NonNull QStack<T> clone(){
        return new QStack<>(this);
    }

    /**
     * Constructor for internal use only.
     * @param p expected to be <code>null</code>.
     * @hidden
     */
    @NativeAccess
    protected QStack(QPrivateConstructor p){
        super(p);
    }
    
    /**
     * @see Deque#remove()
     */
	@Override
    @QtUninvokable
    public final T remove() {
        return takeFirst();
    }

    /**
     * @see Deque#addFirst(Object)
     */
	@Override
    @QtUninvokable
    public final void addFirst(T e) {
        push(e);
    }

    /**
     * @see Deque#addLast(Object)
     */
	@Override
    @QtUninvokable
    public final void addLast(T e) {
        add(e);
    }

    /**
     * @see Deque#offerFirst(Object)
     */
	@Override
    @QtUninvokable
    public final boolean offerFirst(T e) {
        push(e);
        return true;
    }

    /**
     * @see Deque#offerLast(Object)
     */
	@Override
    @QtUninvokable
    public final boolean offerLast(T e) {
        add(e);
        return true;
    }
    
    /**
     * @see Deque#pollFirst()
     */
	@Override
    @QtUninvokable
    public final T pollFirst() {
        if(size()==0)
            return null;
        return pop();
    }
    
    /**
     * @see Deque#pollLast()
     */
	@Override
    @QtUninvokable
    public final T pollLast() {
        if(size()==0)
            return null;
        T e = takeLast();
        return e;
    }

    /**
     * @see Deque#getFirst()
     */
	@Override
    @QtUninvokable
    public final T getFirst() {
        if(size()==0)
            throw new NoSuchElementException();
        return peekFirst();
    }

    /**
     * @see Deque#getLast()
     */
	@Override
    @QtUninvokable
    public final T getLast() {
        if(size()==0)
            throw new NoSuchElementException();
        return peekLast();
    }

    /**
     * @see Deque#poll()
     */
	@Override
    @QtUninvokable
    public final T poll() {
        return pollFirst();
    }
    
    /**
     * @see Deque#element()
     */
	@Override
    @QtUninvokable
    public final T element() {
        return getFirst();
    }
    
    /**
     * @see Deque#peek()
     */
	@Override
    @QtUninvokable
    public final T peek() {
        return peekFirst();
    }
    
    /**
     * @see Deque#offer(Object)
     */
	@Override
    @QtUninvokable
    public final boolean offer(T e) {
        return offerLast(e);
    }

    /**
     * @see Deque#removeFirstOccurrence(Object)
     */
	@Override
    @QtUninvokable
    public final boolean removeFirstOccurrence(Object o) {
        int idx = indexOf(o);
        return idx>=0 && idx<size() ? remove(idx)!=null : false;
    }

    /**
     * @see Deque#removeLastOccurrence(Object)
     */
	@Override
    @QtUninvokable
    public final boolean removeLastOccurrence(Object o) {
        int idx = lastIndexOf(o);
        return idx>=0 && idx<size() ? remove(idx)!=null : false;
    }

    /**
     * @see Deque#descendingIterator()
     */
	@Override
    @QtUninvokable
    public final java.util.Iterator<T> descendingIterator() {
    	return constEnd().toJavaDescendingIterator();
    }

    /**
     * @see Deque#peekFirst()
     */
	@Override
    @QtUninvokable
    public final T peekFirst() {
        return first();
    }
    
    /**
     * Creates and returns a copy of this object.
     * <p>See <code><a href="https://doc.qt.io/qt/qstack.html#top">QStack::<wbr>top()const</a></code></p>
     */
    @QtUninvokable
    public final T top() {
        return last();
    }

    /**
     * @see Deque#peekLast()
     */
	@Override
    @QtUninvokable
    public final T peekLast() {
        return last();
    }

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qstack.html#push">QStack::<wbr>push(T)</a></code></p>
     * @see Deque#push(Object)
     */
	@Override
    @QtUninvokable
    public final void push(T e) {
        add(e);
    }

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qstack.html#pop">QStack::<wbr>pop()</a></code></p>
     * @see Deque#pop()
     */
	@Override
    @QtUninvokable
    public final T pop() {
        return takeLast();
    }
	
	/**
     * Due to changes in Java 21 QStack does no longer implement Deque interface.
     * Instead call {@link asDeque()} to get a Deque wrapper for the stack.
     * @return Deque wrapper
     * @since 6.6.1
     */
    @QtUninvokable
    public final @NonNull Deque<T> asDeque(){
    	return new StackDeque<>(this);
    }

    /**
     * Returns a QStack containing given elements.
     *
     * @param <T> the {@code QStack}'s element type
     * @param element0 the first element
     * @param elements subsequent elements
     * @return a {@code QStack} containing the specified element
     * @throws NullPointerException if elements are {@code null}
     *
     */
    @SafeVarargs
    public static <T> @NonNull QStack<T> of(T element0, T @StrictNonNull...elements) {
		QMetaType metaType = findElementMetaType(element0, elements);
		if(metaType==null || metaType.id()==0)
			throw new IllegalArgumentException("QMetaType::UnknownType cannot be type of QStack.");
		if(metaType.id()==QMetaType.Type.Void.value())
			throw new IllegalArgumentException("void cannot be type of QStack.");
		QStack<T> result = new QStack<>(metaType);
		result.add(element0);
		for (T t : elements) {
			result.add(t);
		}
		return result;
	}
}
