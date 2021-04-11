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

import io.qt.QtUninvokable;
import io.qt.internal.NativeAccess;

/**
 * <p>Java wrapper for Qt class <a href="https://doc.qt.io/qt/qstack.html">QStack</a></p>
 */
public class QStack<T> extends QList<T> implements Deque<T>
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
    
    @NativeAccess
    private QStack(QMetaType elementMetaType, Collection<T> other) {
        super(elementMetaType, other);
    }
    
    public static QStack<Object> createVariantStack(){
        return new QStack<>(new QMetaType(QMetaType.Type.QVariant));
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
    public final T remove() {
        return takeFirst();
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
        T e = takeLast();
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
        int idx = indexOf(o);
        return idx>=0 && idx<size() ? remove(idx)!=null : false;
    }

    @Override
    @QtUninvokable
    public final boolean removeLastOccurrence(Object o) {
        int idx = lastIndexOf(o);
        return idx>=0 && idx<size() ? remove(idx)!=null : false;
    }

    @Override
    @QtUninvokable
    public final java.util.Iterator<T> descendingIterator() {
        return super.descendingIterator();
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
        add(e);
    }

    @Override
    @QtUninvokable
    public final T pop() {
        return takeFirst();
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
