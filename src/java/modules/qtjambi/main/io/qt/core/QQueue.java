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

import java.util.Collection;
import java.util.Queue;

import io.qt.NativeAccess;
import io.qt.QtUninvokable;

/**
 * <p>Java wrapper for Qt class <code><a href="https://doc.qt.io/qt/qqueue.html">QQueue</a></code></p>
 */
public class QQueue<T> extends QList<T> implements Queue<T>
{
    /**
     * Creating a container with given element type and size.
     * <p>See <code><a href="https://doc.qt.io/qt/qqueue.html">QQueue::<wbr>QQueue()</a></code></p>
     * @param metaType the type T
     */
	public QQueue(QMetaType metaType) {
		super(metaType);
	}
	
    /**
     * Creating a container with given element type and size.
     * <p>See <code><a href="https://doc.qt.io/qt/qqueue.html">QQueue::<wbr>QQueue()</a></code></p>
     * @param metaType the type T
     */
	public QQueue(QMetaType.Type metaType) {
		this(new QMetaType(metaType));
	}

    /**
     * Creating a container with given content.
     * <p>See <code><a href="https://doc.qt.io/qt/qqueue.html">QQueue::<wbr>QQueue(const QQueue&lt;T> &amp;)</a></code></p>
     * @param other container
     */
	public QQueue(Collection<T> other) {
		super(other);
	}

    /**
     * Creating a container with given element type.
     * <p>See <code><a href="https://doc.qt.io/qt/qqueue.html">QQueue::<wbr>QQueue()</a></code></p>
     * @param elementType the type T
     */
	public QQueue(Class<T> elementType) {
		super(elementType);
	}

    /**
     * Constructor for internal use only.
     * @param p expected to be <code>null</code>.
     * @hidden
     */
	@NativeAccess
	protected QQueue(QPrivateConstructor p){
        super(p);
    }
	
    /**
     * Creating a container of type QVariant.
     */
	public static QQueue<Object> createVariantQueue(){
    	return new QQueue<>(new QMetaType(QMetaType.Type.QVariant));
    }
	
    /**
     * Creates and returns a copy of this object.
     * <p>See <code><a href="https://doc.qt.io/qt/qqueue.html">QQueue::<wbr>QQueue(const QQueue&lt;T> &amp;)</a></code></p>
     */
	@Override
	public QQueue<T> clone(){
		return new QQueue<>(this);
	}
    
    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qqueue.html#enqueue">QQueue::<wbr>enqueue(T)</a></code></p>
     */
    @QtUninvokable
    public final void enqueue(T t)    {
    	append(t);
    }
    
    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qqueue.html#dequeue">QQueue::<wbr>dequeue()</a></code></p>
     */
    @QtUninvokable
    public final T dequeue()    {
        return takeFirst();
    }

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qqueue.html#head">QQueue::<wbr>head()const</a></code></p>
     */
    @QtUninvokable
    public final T head()    {
        return first();
    }

	/**
	 * @see Queue#peek()
	 */
    @Override
    @QtUninvokable
    public final T peek() {
        return head();
    }

	/**
	 * @see Queue#poll()
	 */
    @Override
    @QtUninvokable
    public final T poll() {
        return dequeue();
    }
    
	/**
	 * @see Queue#offer(Object)
	 */
    @Override
	public final boolean offer(T e) {
		enqueue(e);
		return true;
	}

	/**
	 * @see Queue#remove()
	 */
	@Override
	public final T remove() {
		return poll();
	}

	/**
	 * @see Queue#element()
	 */
	@Override
	public final T element() {
		return peek();
	}
	
	/**
     * Returns a QQueue containing given elements.
     *
     * @param <T> the {@code QQueue}'s element type
     * @param element0 the first element
     * @param elements subsequent elements
     * @return a {@code QQueue} containing the specified element
     * @throws NullPointerException if elements are {@code null}
     *
     */
    @SafeVarargs
    public static <T> QQueue<T> of(T element0, T...elements) {
		QMetaType metaType = findElementMetaType(element0, elements);
		if(metaType==null || metaType.id()==0)
			throw new IllegalArgumentException("QMetaType::UnknownType cannot be type of QQueue.");
		if(metaType.id()==QMetaType.Type.Void.value())
			throw new IllegalArgumentException("void cannot be type of QQueue.");
		QQueue<T> result = new QQueue<>(metaType);
		result.add(element0);
		for (T t : elements) {
			result.add(t);
		}
		return result;
	}
}
