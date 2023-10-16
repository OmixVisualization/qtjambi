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
import io.qt.*;

/**
 * <p>Java wrapper for Qt class <a href="https://doc.qt.io/qt/qstack.html">QStack</a></p>
 */
public class QStack<T> extends QVector<T> implements Deque<T>
{
    public QStack(QMetaType metaType) {
		super(metaType);
	}
    
    public QStack(QMetaType.Type metaType) {
		this(new QMetaType(metaType));
	}

	public QStack(Class<T> elementType) {
		super(elementType);
	}

	public QStack(Collection<T> other) {
		super(other);
	}
	
	public static QStack<Object> createVariantStack(){
    	return new QStack<>(new QMetaType(QMetaType.Type.QVariant.value()));
    }
	
	@Override
	public QStack<T> clone(){
		return new QStack<>(this);
	}

	@NativeAccess
	protected QStack(QPrivateConstructor p){
        super(p);
    }
	
	@Override
    @QtUninvokable
	public T remove() {
		return removeFirst();
	}

	@Override
    @QtUninvokable
	public void addFirst(T e) {
		push(e);
	}

	@Override
    @QtUninvokable
	public void addLast(T e) {
		add(e);
	}

	@Override
    @QtUninvokable
	public boolean offerFirst(T e) {
		push(e);
		return true;
	}

	@Override
    @QtUninvokable
	public boolean offerLast(T e) {
		add(e);
		return true;
	}
	
	@Override
    @QtUninvokable
	public T pollFirst() {
		if(size()==0)
			return null;
		return pop();
	}
	
	@Override
    @QtUninvokable
	public T pollLast() {
		if(size()==0)
			return null;
		T e = removeLast();
		return e;
	}

	@Override
    @QtUninvokable
	public T getFirst() {
		if(size()==0)
			throw new NoSuchElementException();
		return peekFirst();
	}

	@Override
    @QtUninvokable
	public T getLast() {
		if(size()==0)
			throw new NoSuchElementException();
		return peekLast();
	}

	@Override
    @QtUninvokable
	public T poll() {
		return pollFirst();
	}
	
	@Override
    @QtUninvokable
	public T element() {
		return getFirst();
	}
	
	@Override
    @QtUninvokable
	public T peek() {
		return peekFirst();
	}
	
	@Override
    @QtUninvokable
	public boolean offer(T e) {
		return offerLast(e);
	}

	@Override
    @QtUninvokable
	public boolean removeFirstOccurrence(Object o) {
		int idx = indexOf(o);
		return idx>=0 && idx<size() ? remove(idx)!=null : false;
	}

	@Override
    @QtUninvokable
	public boolean removeLastOccurrence(Object o) {
		int idx = lastIndexOf(o);
		return idx>=0 && idx<size() ? remove(idx)!=null : false;
	}

	@Override
    @QtUninvokable
	public java.util.Iterator<T> descendingIterator() {
		return constEnd().toJavaDescendingIterator();
	}

	@Override
    @QtUninvokable
	public T peekFirst() {
		return first();
	}

	@Override
    @QtUninvokable
	public T peekLast() {
		return last();
	}

	@Override
    @QtUninvokable
	public void push(T e) {
		add(e);
	}

	@Override
    @QtUninvokable
	public T pop() {
		return removeLast();
	}
    
    @QtUninvokable
    public final T top() {
        return last();
    }
    
    public final QStack<T> reversed(){
    	QStack<T> result = new QStack<>(elementMetaType());
    	result.reserve(size());
		for(T t : this) {
			result.add(0, t);
		}
    	return result;
    }
    
    /**
     * Returns a QStack containing given elements.
     *
     * @param <E> the {@code QStack}'s element type
     * @param element0 the first element
     * @param elements subsequent elements
     * @return a {@code QStack} containing the specified element
     * @throws NullPointerException if elements are {@code null}
     *
     */
    @SafeVarargs
    public static <T> QStack<T> of(T element0, T...elements) {
		QMetaType metaType = QList.findElementMetaType(element0, elements);
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
