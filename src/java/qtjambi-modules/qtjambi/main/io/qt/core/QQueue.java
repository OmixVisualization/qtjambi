/****************************************************************************
**
** Copyright (C) 2009-2022 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
import java.util.Queue;

import io.qt.NativeAccess;
import io.qt.QtUninvokable;

/**
 * <p>Java wrapper for Qt class <a href="https://doc.qt.io/qt/qqueue.html">QQueue</a></p>
 */
public class QQueue<T> extends QList<T> implements Queue<T>
{
	public QQueue(QMetaType metaType) {
		super(metaType);
	}
	
	public QQueue(QMetaType.Type metaType) {
		this(new QMetaType(metaType));
	}

	public QQueue(Collection<T> other) {
		super(other);
	}

	public QQueue(Class<T> elementType) {
		super(elementType);
	}

	@NativeAccess
	protected QQueue(QPrivateConstructor p){
        super(p);
    }
	
	public static QQueue<Object> createVariantQueue(){
    	return new QQueue<>(new QMetaType(QMetaType.Type.QVariant));
    }
	
	@Override
	public QQueue<T> clone(){
		return new QQueue<>(this);
	}
    
    @QtUninvokable
    public final void enqueue(T t)    {
    	append(t);
    }
    
    @QtUninvokable
    public final T dequeue()    {
        return takeFirst();
    }

    @QtUninvokable
    public final T head()    {
        return first();
    }

    @Override
    @QtUninvokable
    public final T peek() {
        return head();
    }

    @Override
    @QtUninvokable
    public final T poll() {
        return dequeue();
    }
    
    @Override
	public final boolean offer(T e) {
		enqueue(e);
		return true;
	}

	@Override
	public final T remove() {
		return poll();
	}

	@Override
	public final T element() {
		return peek();
	}
	
	/**
     * Returns a QQueue containing given elements.
     *
     * @param <E> the {@code QQueue}'s element type
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
			throw new IllegalArgumentException("QMetaType::UnknownType cannot be type of QList.");
		if(metaType.id()==QMetaType.Type.Void.value())
			throw new IllegalArgumentException("void cannot be type of QList.");
		QQueue<T> result = new QQueue<>(metaType);
		result.add(element0);
		for (T t : elements) {
			result.add(t);
		}
		return result;
	}
}
