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

import java.nio.ByteBuffer;
import java.nio.CharBuffer;
import java.nio.DoubleBuffer;
import java.nio.FloatBuffer;
import java.nio.IntBuffer;
import java.nio.LongBuffer;
import java.nio.ShortBuffer;
import java.util.Arrays;
import java.util.Collection;
import java.util.Queue;

import io.qt.NativeAccess;
import io.qt.NonNull;
import io.qt.Nullable;
import io.qt.QtPrimitiveType;
import io.qt.QtUninvokable;
import io.qt.StrictNonNull;

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
	public QQueue(@StrictNonNull QMetaType metaType) {
		super(metaType);
	}
	
    /**
     * Creating a container with given element type and size.
     * <p>See <code><a href="https://doc.qt.io/qt/qqueue.html">QQueue::<wbr>QQueue()</a></code></p>
     * @param metaType the type T
     */
	public QQueue(QMetaType.@StrictNonNull Type metaType) {
		this(new QMetaType(metaType));
	}

    /**
     * Creating a container with given content.
     * <p>See <code><a href="https://doc.qt.io/qt/qqueue.html">QQueue::<wbr>QQueue(const QQueue&lt;T> &amp;)</a></code></p>
     * @param other container
     */
	public QQueue(@StrictNonNull Collection<T> other) {
		super(other);
	}

    /**
     * Creating a container with given element type.
     * <p>See <code><a href="https://doc.qt.io/qt/qqueue.html">QQueue::<wbr>QQueue()</a></code></p>
     * @param elementType the type T
     */
	public QQueue(@Nullable Class<T> elementType) {
		super(elementType);
	}
	
	@NativeAccess
    private QQueue(QMetaType elementMetaType, Collection<T> other) {
        super(elementMetaType, other);
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
	public static @NonNull QQueue<Object> createVariantQueue(){
    	return new QQueue<>(new QMetaType(QMetaType.Type.QVariant));
    }
	
    /**
     * Creates and returns a copy of this object.
     * <p>See <code><a href="https://doc.qt.io/qt/qqueue.html">QQueue::<wbr>QQueue(const QQueue&lt;T> &amp;)</a></code></p>
     */
	@Override
	public @NonNull QQueue<T> clone(){
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
    public static <T> @NonNull QQueue<T> of(T element0, T...elements) {
		QMetaType metaType = findElementMetaType(element0, elements);
		@SuppressWarnings("unchecked")
		T[] allElements = (T[])new Object[elements.length+1];
		System.arraycopy(elements, 0, allElements, 1, elements.length);
		allElements[0] = element0;
		return ofTyped(metaType, allElements);
	}
    
    /**
     * Returns a QQueue containing given elements.
     *
     * @param elements
     * @return a {@code QQueue} containing the specified element
     */
    public static @NonNull QQueue<@QtPrimitiveType@NonNull Byte> ofByte(byte @StrictNonNull...elements) {
		return new QQueue<>(QList.ofByte(elements));
	}
    
    /**
     * Returns a QQueue containing given elements.
     *
     * @param elements
     * @return a {@code QQueue} containing the specified element
     */
    public static @NonNull QQueue<@QtPrimitiveType@NonNull Short> ofShort(short @StrictNonNull...elements) {
		return new QQueue<>(QList.ofShort(elements));
	}
    
    /**
     * Returns a QQueue containing given elements.
     *
     * @param elements
     * @return a {@code QQueue} containing the specified element
     */
    public static @NonNull QQueue<@QtPrimitiveType@NonNull Integer> ofInt(int @StrictNonNull...elements) {
		return new QQueue<>(QList.ofInt(elements));
	}
    
    /**
     * Returns a QQueue containing given elements.
     *
     * @param elements
     * @return a {@code QQueue} containing the specified element
     */
    public static @NonNull QQueue<@QtPrimitiveType@NonNull Long> ofLong(long @StrictNonNull...elements) {
		return new QQueue<>(QList.ofLong(elements));
	}
    
    /**
     * Returns a QQueue containing given elements.
     *
     * @param elements
     * @return a {@code QQueue} containing the specified element
     */
    public static @NonNull QQueue<@QtPrimitiveType@NonNull Character> ofChar(char @StrictNonNull...elements) {
		return new QQueue<>(QList.ofChar(elements));
	}
    
    /**
     * Returns a QQueue containing given elements.
     *
     * @param elements
     * @return a {@code QQueue} containing the specified element
     */
    public static @NonNull QQueue<@QtPrimitiveType@NonNull Float> ofFloat(float @StrictNonNull...elements) {
		return new QQueue<>(QList.ofFloat(elements));
	}
    
    /**
     * Returns a QQueue containing given elements.
     *
     * @param elements
     * @return a {@code QQueue} containing the specified element
     */
    public static @NonNull QQueue<@QtPrimitiveType@NonNull Double> ofDouble(double @StrictNonNull...elements) {
		return new QQueue<>(QList.ofDouble(elements));
	}
    
    /**
     * Returns a QQueue containing given elements.
     *
     * @param elements
     * @return a {@code QQueue} containing the specified element
     */
    public static @NonNull QQueue<@QtPrimitiveType@NonNull Boolean> ofBoolean(boolean @StrictNonNull...elements) {
		return new QQueue<>(QList.ofBoolean(elements));
	}
    
    /**
     * Returns a QQueue containing given elements.
     *
     * @param elements
     * @return a {@code QQueue} containing the specified element
     */
    public static @NonNull QQueue<@QtPrimitiveType@NonNull Byte> ofBuffer(@StrictNonNull ByteBuffer elements) {
		return new QQueue<>(QList.ofBuffer(elements));
	}
    
    /**
     * Returns a QQueue containing given elements.
     *
     * @param elements
     * @return a {@code QQueue} containing the specified element
     */
    public static @NonNull QQueue<@QtPrimitiveType@NonNull Short> ofBuffer(@StrictNonNull ShortBuffer elements) {
		return new QQueue<>(QList.ofBuffer(elements));
	}
    
    /**
     * Returns a QQueue containing given elements.
     *
     * @param elements
     * @return a {@code QQueue} containing the specified element
     */
    public static @NonNull QQueue<@QtPrimitiveType@NonNull Integer> ofBuffer(@StrictNonNull IntBuffer elements) {
		return new QQueue<>(QList.ofBuffer(elements));
	}
    
    /**
     * Returns a QQueue containing given elements.
     *
     * @param elements
     * @return a {@code QQueue} containing the specified element
     */
    public static @NonNull QQueue<@QtPrimitiveType@NonNull Long> ofBuffer(@StrictNonNull LongBuffer elements) {
		return new QQueue<>(QList.ofBuffer(elements));
	}
    
    /**
     * Returns a QQueue containing given elements.
     *
     * @param elements
     * @return a {@code QQueue} containing the specified element
     */
    public static @NonNull QQueue<@QtPrimitiveType@NonNull Character> ofBuffer(@StrictNonNull CharBuffer elements) {
		return new QQueue<>(QList.ofBuffer(elements));
	}
    
    /**
     * Returns a QQueue containing given elements.
     *
     * @param elements
     * @return a {@code QQueue} containing the specified element
     */
    public static @NonNull QQueue<@QtPrimitiveType@NonNull Float> ofBuffer(@StrictNonNull FloatBuffer elements) {
		return new QQueue<>(QList.ofBuffer(elements));
	}
    
    /**
     * Returns a QQueue containing given elements.
     *
     * @param elements
     * @return a {@code QQueue} containing the specified element
     */
    public static @NonNull QQueue<@QtPrimitiveType@NonNull Double> ofBuffer(@StrictNonNull DoubleBuffer elements) {
		return new QQueue<>(QList.ofBuffer(elements));
	}
    
    /**
     * Returns a QQueue containing given elements.
     *
     * @param <T> the {@code QQueue}'s element type
     * @param type the {@code QQueue}'s element type
     * @param elements all list elements to be added
     * @return a {@code QQueue} containing the specified element
     *
     */
	@SafeVarargs
    public static <T> @NonNull QQueue<T> ofTyped(@Nullable Class<? super T> type, T @StrictNonNull...elements) {
		QMetaType metaType = findElementMetaType(type, elements);
		return ofTyped(metaType, elements);
	}
    
    /**
     * Returns a QQueue containing given elements.
     *
     * @param <T> the {@code QQueue}'s element type
     * @param metaType the {@code QQueue}'s element type
     * @param elements all list elements to be added
     * @return a {@code QQueue} containing the specified element
     *
     */
	@SafeVarargs
    public static <T> @NonNull QQueue<T> ofTyped(@StrictNonNull QMetaType metaType, T @StrictNonNull...elements) {
		if(metaType.id()==0)
			throw new IllegalArgumentException("QMetaType::UnknownType cannot be type of QQueue.");
		if(metaType.id()==QMetaType.Type.Void.value())
			throw new IllegalArgumentException("void cannot be type of QQueue.");
		return new QQueue<>(metaType, Arrays.asList(elements));
	}
}
