/****************************************************************************
**
** Copyright (C) 2009-2024 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import java.nio.*;
import java.util.*;
import io.qt.*;
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
    public QStack(@Nullable Class<T> elementType) {
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
    public final java.util.@NonNull Iterator<T> descendingIterator() {
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
		@SuppressWarnings("unchecked")
		T[] allElements = (T[])new Object[elements.length+1];
		System.arraycopy(elements, 0, allElements, 1, elements.length);
		allElements[0] = element0;
		return ofTyped(metaType, allElements);
	}
    
    /**
     * Returns a QStack containing given elements.
     *
     * @param elements
     * @return a {@code QStack} containing the specified element
     */
    public static @NonNull QStack<@QtPrimitiveType@NonNull Byte> ofByte(byte @StrictNonNull...elements) {
		QStack<Byte> result = new QStack<>(byte.class);
    	result.resize(elements.length);
    	ByteBuffer data = QtJambi_LibraryUtilities.internal.mutableDataB(result);
		data.put(elements, 0, elements.length);
		QtJambi_LibraryUtilities.internal.truncateBuffer(result, data);
		return result;
	}
    
    /**
     * Returns a QStack containing given elements.
     *
     * @param elements
     * @return a {@code QStack} containing the specified element
     */
    public static @NonNull QStack<@QtPrimitiveType@NonNull Short> ofShort(short @StrictNonNull...elements) {
		QStack<Short> result = new QStack<>(short.class);
    	result.resize(elements.length);
    	ShortBuffer data = QtJambi_LibraryUtilities.internal.mutableDataS(result);
		data.put(elements, 0, elements.length);
		QtJambi_LibraryUtilities.internal.truncateBuffer(result, data);
		return result;
	}
    
    /**
     * Returns a QStack containing given elements.
     *
     * @param elements
     * @return a {@code QStack} containing the specified element
     */
    public static @NonNull QStack<@QtPrimitiveType@NonNull Integer> ofInt(int @StrictNonNull...elements) {
		QStack<Integer> result = new QStack<>(int.class);
    	result.resize(elements.length);
    	IntBuffer data = QtJambi_LibraryUtilities.internal.mutableDataI(result);
		data.put(elements, 0, elements.length);
		QtJambi_LibraryUtilities.internal.truncateBuffer(result, data);
		return result;
	}
    
    /**
     * Returns a QStack containing given elements.
     *
     * @param elements
     * @return a {@code QStack} containing the specified element
     */
    public static @NonNull QStack<@QtPrimitiveType@NonNull Long> ofLong(long @StrictNonNull...elements) {
		QStack<Long> result = new QStack<>(long.class);
    	result.resize(elements.length);
    	LongBuffer data = QtJambi_LibraryUtilities.internal.mutableDataJ(result);
		data.put(elements, 0, elements.length);
		QtJambi_LibraryUtilities.internal.truncateBuffer(result, data);
		return result;
	}
    
    /**
     * Returns a QStack containing given elements.
     *
     * @param elements
     * @return a {@code QStack} containing the specified element
     */
    public static @NonNull QStack<@QtPrimitiveType@NonNull Character> ofChar(char @StrictNonNull...elements) {
		QStack<Character> result = new QStack<>(char.class);
    	result.resize(elements.length);
    	CharBuffer data = QtJambi_LibraryUtilities.internal.mutableDataC(result);
		data.put(elements, 0, elements.length);
		QtJambi_LibraryUtilities.internal.truncateBuffer(result, data);
		return result;
	}
    
    /**
     * Returns a QStack containing given elements.
     *
     * @param elements
     * @return a {@code QStack} containing the specified element
     */
    public static @NonNull QStack<@QtPrimitiveType@NonNull Float> ofFloat(float @StrictNonNull...elements) {
		QStack<Float> result = new QStack<>(float.class);
    	result.resize(elements.length);
    	FloatBuffer data = QtJambi_LibraryUtilities.internal.mutableDataF(result);
		data.put(elements, 0, elements.length);
		QtJambi_LibraryUtilities.internal.truncateBuffer(result, data);
		return result;
	}
    
    /**
     * Returns a QStack containing given elements.
     *
     * @param elements
     * @return a {@code QStack} containing the specified element
     */
    public static @NonNull QStack<@QtPrimitiveType@NonNull Double> ofDouble(double @StrictNonNull...elements) {
		QStack<Double> result = new QStack<>(double.class);
    	result.resize(elements.length);
    	DoubleBuffer data = QtJambi_LibraryUtilities.internal.mutableDataD(result);
		data.put(elements, 0, elements.length);
		QtJambi_LibraryUtilities.internal.truncateBuffer(result, data);
		return result;
	}
    
    /**
     * Returns a QStack containing given elements.
     *
     * @param elements
     * @return a {@code QStack} containing the specified element
     */
    public static @NonNull QStack<@QtPrimitiveType@NonNull Boolean> ofBoolean(boolean @StrictNonNull...elements) {
		QStack<Boolean> result = new QStack<>(boolean.class);
    	result.reserve(elements.length);
		for (boolean t : elements) {
			result.append(t);
		}
		return result;
	}
    
    /**
     * Returns a QStack containing given elements.
     *
     * @param elements
     * @return a {@code QStack} containing the specified element
     */
    public static @NonNull QStack<@QtPrimitiveType@NonNull Byte> ofBuffer(@StrictNonNull ByteBuffer elements) {
		QStack<Byte> result = new QStack<>(byte.class);
		result.resize(elements.remaining());
		ByteBuffer data = QtJambi_LibraryUtilities.internal.mutableDataB(result);
		data.put(elements);
		QtJambi_LibraryUtilities.internal.truncateBuffer(result, data);
		return result;
	}
    
    /**
     * Returns a QStack containing given elements.
     *
     * @param elements
     * @return a {@code QStack} containing the specified element
     */
    public static @NonNull QStack<@QtPrimitiveType@NonNull Short> ofBuffer(@StrictNonNull ShortBuffer elements) {
		QStack<Short> result = new QStack<>(short.class);
		result.resize(elements.remaining());
		ShortBuffer data = QtJambi_LibraryUtilities.internal.mutableDataS(result);
		data.put(elements);
		QtJambi_LibraryUtilities.internal.truncateBuffer(result, data);
		return result;
	}
    
    /**
     * Returns a QStack containing given elements.
     *
     * @param elements
     * @return a {@code QStack} containing the specified element
     */
    public static @NonNull QStack<@QtPrimitiveType@NonNull Integer> ofBuffer(@StrictNonNull IntBuffer elements) {
		QStack<Integer> result = new QStack<>(int.class);
		result.resize(elements.remaining());
		IntBuffer data = QtJambi_LibraryUtilities.internal.mutableDataI(result);
		data.put(elements);
		QtJambi_LibraryUtilities.internal.truncateBuffer(result, data);
		return result;
	}
    
    /**
     * Returns a QStack containing given elements.
     *
     * @param elements
     * @return a {@code QStack} containing the specified element
     */
    public static @NonNull QStack<@QtPrimitiveType@NonNull Long> ofBuffer(@StrictNonNull LongBuffer elements) {
		QStack<Long> result = new QStack<>(long.class);
		result.resize(elements.remaining());
		LongBuffer data = QtJambi_LibraryUtilities.internal.mutableDataJ(result);
		data.put(elements);
		QtJambi_LibraryUtilities.internal.truncateBuffer(result, data);
		return result;
	}
    
    /**
     * Returns a QStack containing given elements.
     *
     * @param elements
     * @return a {@code QStack} containing the specified element
     */
    public static @NonNull QStack<@QtPrimitiveType@NonNull Character> ofBuffer(@StrictNonNull CharBuffer elements) {
		QStack<Character> result = new QStack<>(char.class);
		result.resize(elements.remaining());
		CharBuffer data = QtJambi_LibraryUtilities.internal.mutableDataC(result);
		data.put(elements);
		QtJambi_LibraryUtilities.internal.truncateBuffer(result, data);
		return result;
	}
    
    /**
     * Returns a QStack containing given elements.
     *
     * @param elements
     * @return a {@code QStack} containing the specified element
     */
    public static @NonNull QStack<@QtPrimitiveType@NonNull Float> ofBuffer(@StrictNonNull FloatBuffer elements) {
		QStack<Float> result = new QStack<>(float.class);
		result.resize(elements.remaining());
		FloatBuffer data = QtJambi_LibraryUtilities.internal.mutableDataF(result);
		data.put(elements);
		QtJambi_LibraryUtilities.internal.truncateBuffer(result, data);
		return result;
	}
    
    /**
     * Returns a QStack containing given elements.
     *
     * @param elements
     * @return a {@code QStack} containing the specified element
     */
    public static @NonNull QStack<@QtPrimitiveType@NonNull Double> ofBuffer(@StrictNonNull DoubleBuffer elements) {
		QStack<Double> result = new QStack<>(double.class);
		result.resize(elements.remaining());
		DoubleBuffer data = QtJambi_LibraryUtilities.internal.mutableDataD(result);
		data.put(elements);
		QtJambi_LibraryUtilities.internal.truncateBuffer(result, data);
		return result;
	}
    
    /**
     * Returns a QStack containing given elements.
     *
     * @param <T> the {@code QStack}'s element type
     * @param type the {@code QStack}'s element type
     * @param elements all list elements to be added
     * @return a {@code QStack} containing the specified element
     *
     */
	@SafeVarargs
    public static <T> @NonNull QStack<T> ofTyped(@Nullable Class<? super T> type, T @StrictNonNull...elements) {
		QMetaType metaType = QList.findElementMetaType(type, elements);
		return ofTyped(metaType, elements);
	}
    
    /**
     * Returns a QStack containing given elements.
     *
     * @param <T> the {@code QStack}'s element type
     * @param metaType the {@code QStack}'s element type
     * @param elements all list elements to be added
     * @return a {@code QStack} containing the specified element
     *
     */
	@SafeVarargs
    public static <T> @NonNull QStack<T> ofTyped(@StrictNonNull QMetaType metaType, T @StrictNonNull...elements) {
		if(metaType.id()==0)
			throw new IllegalArgumentException("QMetaType::UnknownType cannot be type of QStack.");
		if(metaType.id()==QMetaType.Type.Void.value())
			throw new IllegalArgumentException("void cannot be type of QStack.");
		return new QStack<>(metaType, Arrays.asList(elements));
	}
}
