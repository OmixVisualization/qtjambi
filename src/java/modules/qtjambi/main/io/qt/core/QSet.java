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

import java.nio.*;
import java.util.*;
import java.util.function.*;
import io.qt.*;
import io.qt.core.QtJambi_LibraryUtilities;

/**
 * <p>Java wrapper for Qt class <code><a href="https://doc.qt.io/qt/qset.html">QSet</a></code></p>
 */
public class QSet<T> extends AbstractSequentialContainer<T> implements Set<T>, Cloneable
{

	static {
    	QtJambi_LibraryUtilities.initialize();
    }

    /**
     * Constructor for internal use only.
     * @param p expected to be <code>null</code>.
     * @hidden
     */
    @NativeAccess
    protected QSet(QPrivateConstructor p) {
		super(p);
	}
    
    /**
     * Creating a container with given element type.
     * <p>See <code><a href="https://doc.qt.io/qt/qset.html#QSet">QSet::<wbr>QSet()</a></code></p>
     * @param elementType the type T
     */
	public QSet(@Nullable Class<T> elementType) {
		super(null);
		QMetaType metaType = QMetaType.fromType(elementType);
		initialize(elementType, QtJambi_LibraryUtilities.internal.nativeId(metaType), null);
	}
	
    /**
     * Creating a container with given element type.
     * <p>See <code><a href="https://doc.qt.io/qt/qset.html#QSet">QSet::<wbr>QSet()</a></code></p>
     * @param metaType the type T
     */
	public QSet(QMetaType.@StrictNonNull Type metaType) {
		this(new QMetaType(metaType));
	}
    
    /**
     * Creating a container with given element type.
     * <p>See <code><a href="https://doc.qt.io/qt/qset.html#QSet">QSet::<wbr>QSet()</a></code></p>
     * @param metaType the type T
     */
	public QSet(@StrictNonNull QMetaType metaType) {
		super(null);
		initialize(metaType.javaType(), QtJambi_LibraryUtilities.internal.nativeId(metaType), null);
	}
    
    /**
     * Creating a container with given content.
     * <p>See <code><a href="https://doc.qt.io/qt/qset.html#QSet">QSet::<wbr>QSet(const QSet&lt;T> &amp;)</a></code></p>
     * @param other container
     */
    public QSet(@StrictNonNull Collection<T> other) {
        super(null);
		QMetaType metaType = QList.findElementMetaType(Objects.requireNonNull(other, "Argument 'other': null not expected."));
		initialize(metaType.javaType(), QtJambi_LibraryUtilities.internal.nativeId(metaType), other);
    }
    
    private QSet(QMetaType metaType, Collection<T> other) {
        super(null);
		initialize(metaType.javaType(), QtJambi_LibraryUtilities.internal.nativeId(metaType), other);
    }
    
    @QtUninvokable
    private native void initialize(Class<?> elementType, long elementMetaType, Collection<T> other);
    
    /**
     * Creates and returns a copy of this object.
     */
    @Override
	public @NonNull QSet<T> clone(){
		return new QSet<>(this);
	}
    
    /**
     * Provides a constant C++ iterator to the containers begin.
     * <p>See <code><a href="https://doc.qt.io/qt/qset.html#constBegin">QSet::constBegin()const</a></code></p>
     * @return begin
     */
    @QtUninvokable
    protected final @NonNull QSequentialConstIterator<T> constBegin()    {
        return constBegin(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private native QSequentialConstIterator<T> constBegin(long __this__nativeId);

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qset.html#capacity">QSet::<wbr>capacity()const</a></code></p>
     */
    @QtUninvokable
    public final int capacity()    {
        return capacity(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private native int capacity(long __this__nativeId);

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qset.html#clear">QSet::<wbr>clear()</a></code></p>
     */
    @QtUninvokable
    public final void clear()    {
        clear(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private native void clear(long __this__nativeId);

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qset.html#contains">QSet::<wbr>contains(T)const</a></code></p>
     */
    @QtUninvokable
    public final boolean contains(Object t)    {
        try {
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
    private native boolean contains(long __this__nativeId, Object t);

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qset.html#count">QSet::<wbr>count()const</a></code></p>
     */
    @QtUninvokable
    public final int count()    {
        return size();
    }

    /**
     * Provides a constant C++ iterator to the containers end.
     * <p>See <code><a href="https://doc.qt.io/qt/qset.html#constEnd">QSet::constEnd()const</a></code></p>
     * @return end
     */
    @QtUninvokable
    protected final @NonNull QSequentialConstIterator<T> constEnd()    {
        return constEnd(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private native QSequentialConstIterator<T> constEnd(long __this__nativeId);

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qset.html#insert">QSet::<wbr>insert(T)</a></code></p>
     */
    @QtUninvokable
    public final void insert(T t)    {
    	try {
    		insert(QtJambi_LibraryUtilities.internal.nativeId(this), t);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QSet.handleException(e, elementMetaType(), t);
    	}
    }
    @QtUninvokable
    private native void insert(long __this__nativeId, T t);

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qset.html#intersect">QSet::<wbr>intersect(QSet&lt;T>)</a></code></p>
     */
    @QtUninvokable
    public final void intersect(java.util.@NonNull Collection<T> other)    {
        intersect(QtJambi_LibraryUtilities.internal.nativeId(this), other);
    }
    @QtUninvokable
    private native void intersect(long __this__nativeId, java.util.Collection<T> other);

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qset.html#intersects">QSet::<wbr>intersects(QSet&lt;T>)const</a></code></p>
     */
    @QtUninvokable
    public final boolean intersects(java.util.@NonNull Collection<T> other)    {
        return intersects(QtJambi_LibraryUtilities.internal.nativeId(this), other);
    }
    @QtUninvokable
    private native boolean intersects(long __this__nativeId, java.util.Collection<T> other);

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qset.html#isEmpty">QSet::<wbr>isEmpty()const</a></code></p>
     */
    @QtUninvokable
    public final boolean isEmpty()    {
        return size()==0;
    }

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qset.html#remove">QSet::<wbr>remove(T)</a></code></p>
     */
    @QtUninvokable
    public final boolean remove(Object t)    {
    	try {
    		return remove(QtJambi_LibraryUtilities.internal.nativeId(this), t);
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
    private native boolean remove(long __this__nativeId, Object t);
    
    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qset.html#removeIf">QSet::<wbr>removeIf(Predicate)</a></code></p>
	 */
    @QtUninvokable
    public final boolean removeIf(@StrictNonNull Predicate<? super T> predicate)    {
        Objects.requireNonNull(predicate, "Argument 'predicate': null not expected.");
        boolean removed = false;
        QSet<T> clone = clone();
        final Iterator<T> each = clone.iterator();
        while (each.hasNext()) {
        	T value = each.next();
            if (predicate.test(value)) {
                remove(value);
                removed = true;
            }
        }
        return removed;
    }

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qset.html#reserve">QSet::<wbr>reserve(int)</a></code></p>
     */
    @QtUninvokable
    public final void reserve(int size)    {
        reserve(QtJambi_LibraryUtilities.internal.nativeId(this), size);
    }
    @QtUninvokable
    private native void reserve(long __this__nativeId, int size);

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qset.html#size">QSet::<wbr>size()const</a></code></p>
     */
    @QtUninvokable
    public final int size()    {
        return size(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private native int size(long __this__nativeId);

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qset.html#subtract">QSet::<wbr>subtract(QSet&lt;T>)</a></code></p>
     */
    @QtUninvokable
    public final void subtract(java.util.@NonNull Collection<T> other)    {
        subtract(QtJambi_LibraryUtilities.internal.nativeId(this), other);
    }
    @QtUninvokable
    private native void subtract(long __this__nativeId, java.util.Collection<T> other);

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qset.html#unite">QSet::<wbr>unite(QSet&lt;T>)</a></code></p>
     */
    @QtUninvokable
    public final void unite(java.util.@NonNull Collection<T> other)    {
        unite(QtJambi_LibraryUtilities.internal.nativeId(this), other);
    }
    @QtUninvokable
    private native void unite(long __this__nativeId, java.util.Collection<T> other);

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qset.html#values">QSet::<wbr>values()const</a></code></p>
     */
    @QtUninvokable
    public final @NonNull QList<T> values()    {
        return values(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private native QList<T> values(long __this__nativeId);
    
    /**
     * <p>See <code>QSet::<wbr>operator=(QSet&lt;T>)</code></p>
     */
    @QtUninvokable
    public final void assign(@StrictNonNull QSet<T> other) {
		assign(QtJambi_LibraryUtilities.internal.nativeId(this), other, QtJambi_LibraryUtilities.internal.nativeId(other));
    }
    
    @QtUninvokable
    private native void assign(long __this__nativeId, Object container, long other);
    
    /**
     * <p>See <code>QSet::<wbr>swap(QSet&lt;T>)</code></p>
     */
    @QtUninvokable
    public final void swap(@StrictNonNull QSet<T> other) {
    	swap(QtJambi_LibraryUtilities.internal.nativeId(this), other, QtJambi_LibraryUtilities.internal.nativeId(other));
    }
    
    @QtUninvokable
    private native void swap(long __this__nativeId, Object otherObj, long other);

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qset.html#operator-eq-eq">QSet::<wbr>operator==(QSet&lt;T>)const</a></code></p>
     */
	@Override
    @QtUninvokable
    public boolean equals(Object other) {
        if (other instanceof java.util.Collection && QList.checkContainerType(this.elementMetaType(), (java.util.Collection<?>) other)) {
            return operator_equal(QtJambi_LibraryUtilities.internal.nativeId(this), (java.util.Collection<?>) other);
        }
        return false;
    }

    @QtUninvokable
    private native boolean operator_equal(long __this__nativeId, java.util.Collection<?> l);

    /**
     * Returns the objects's hash code computed by <code>qHash(QSet&lt;T>)</code>.
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
    private native int hashCode(long __this__nativeId);
    
    /**
     * Returns the string representation of the object given by <code>QVariant(this).toString()</code>.
     */
    @Override
    @QtUninvokable
    public @NonNull String toString() {
    	try {
			return toString(QtJambi_LibraryUtilities.internal.nativeId(this));
		} catch (QNoImplementationException e) {
			return super.toString();
		} catch (QNoNativeResourcesException e) {
			return "null";
		}
    }
    @QtUninvokable
    private native String toString(long __this__nativeId);
    
    /**
     * Adds the specified element to this set if it is not already present
     * (optional operation).  More formally, adds the specified element
     * {@code e} to this set if the set contains no element {@code e2}
     * such that
     * {@code Objects.equals(e, e2)}.
     * If this set already contains the element, the call leaves the set
     * unchanged and returns {@code false}.  In combination with the
     * restriction on constructors, this ensures that sets never contain
     * duplicate elements.
     */
    @Override
    @QtUninvokable
    public final boolean add(T e){
        insert(e);
        return true;
    }
    
    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qset.html#operator-lt-lt">operator&lt;&lt;(QDataStream&amp;,<wbr>QSet&lt;T>)</a></code></p>
     */
    @io.qt.QtUninvokable
    public void writeTo(io.qt.core.@StrictNonNull QDataStream stream){
    	java.util.Objects.requireNonNull(stream, "Argument 'stream': null not expected.");
        writeTo(QtJambi_LibraryUtilities.internal.nativeId(this), QtJambi_LibraryUtilities.internal.nativeId(stream));
    }
    
    @io.qt.QtUninvokable
    private native void writeTo(long __this__nativeId, long stream);
    
    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qset.html#operator-gt-gt">operator&gt;&gt;(QDataStream&amp;,<wbr>QSet&lt;T>&amp;)</a></code></p>
     */
    @io.qt.QtUninvokable
    public void readFrom(io.qt.core.@StrictNonNull QDataStream stream){
    	java.util.Objects.requireNonNull(stream, "Argument 'stream': null not expected.");
        readFrom(QtJambi_LibraryUtilities.internal.nativeId(this), QtJambi_LibraryUtilities.internal.nativeId(stream));
    }
    
    @io.qt.QtUninvokable
    private native void readFrom(long __this__nativeId, long stream);
    
    @io.qt.QtUninvokable
    final QMetaType elementMetaType() {
    	return elementMetaType(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @io.qt.QtUninvokable
    private native QMetaType elementMetaType(long __this_nativeId);
    
    /**
     * Returns a QSet containing given elements.
     *
     * @param <T> the {@code QSet}'s element type
     * @param element0 the first element
     * @param elements subsequent elements
     * @return a {@code QSet} containing the specified element
     * @throws NullPointerException if elements are {@code null}
     */
    @SuppressWarnings({ "unchecked" })
    @SafeVarargs
    public static <T> @NonNull QSet<T> of(T element0, T @StrictNonNull...elements) {
        QMetaType metaType = QList.findElementMetaType(element0, elements);
		T[] allElements = (T[])new Object[elements.length+1];
		System.arraycopy(elements, 0, allElements, 1, elements.length);
		allElements[0] = element0;
		return ofTyped(metaType, allElements);
    }
    
    /**
     * Returns a QSet containing given elements.
     *
     * @param elements
     * @return a {@code QSet} containing the specified element
     */
    public static @NonNull QSet<@QtPrimitiveType@NonNull Byte> ofByte(byte @StrictNonNull...elements) {
		QSet<Byte> result = new QSet<>(byte.class);
		for (byte t : elements) {
			result.insert(t);
		}
		return result;
	}
    
    /**
     * Returns a QSet containing given elements.
     *
     * @param elements
     * @return a {@code QSet} containing the specified element
     */
    public static @NonNull QSet<@QtPrimitiveType@NonNull Short> ofShort(short @StrictNonNull...elements) {
		QSet<Short> result = new QSet<>(short.class);
		for (short t : elements) {
			result.insert(t);
		}
		return result;
	}
    
    /**
     * Returns a QSet containing given elements.
     *
     * @param elements
     * @return a {@code QSet} containing the specified element
     */
    public static @NonNull QSet<@QtPrimitiveType@NonNull Integer> ofInt(int @StrictNonNull...elements) {
		QSet<Integer> result = new QSet<>(int.class);
		for (int t : elements) {
			result.insert(t);
		}
		return result;
	}
    
    /**
     * Returns a QSet containing given elements.
     *
     * @param elements
     * @return a {@code QSet} containing the specified element
     */
    public static @NonNull QSet<@QtPrimitiveType@NonNull Long> ofLong(long @StrictNonNull...elements) {
		QSet<Long> result = new QSet<>(long.class);
		for (long t : elements) {
			result.insert(t);
		}
		return result;
	}
    
    /**
     * Returns a QSet containing given elements.
     *
     * @param elements
     * @return a {@code QSet} containing the specified element
     */
    public static @NonNull QSet<@QtPrimitiveType@NonNull Character> ofChar(char @StrictNonNull...elements) {
		QSet<Character> result = new QSet<>(char.class);
		for (char t : elements) {
			result.insert(t);
		}
		return result;
	}
    
    /**
     * Returns a QSet containing given elements.
     *
     * @param elements
     * @return a {@code QSet} containing the specified element
     */
    public static @NonNull QSet<@QtPrimitiveType@NonNull Float> ofFloat(float @StrictNonNull...elements) {
		QSet<Float> result = new QSet<>(float.class);
		for (float t : elements) {
			result.insert(t);
		}
		return result;
	}
    
    /**
     * Returns a QSet containing given elements.
     *
     * @param elements
     * @return a {@code QSet} containing the specified element
     */
    public static @NonNull QSet<@QtPrimitiveType@NonNull Double> ofDouble(double @StrictNonNull...elements) {
		QSet<Double> result = new QSet<>(double.class);
		for (double t : elements) {
			result.insert(t);
		}
		return result;
	}
    
    /**
     * Returns a QSet containing given elements.
     *
     * @param elements
     * @return a {@code QSet} containing the specified element
     */
    public static @NonNull QSet<@QtPrimitiveType@NonNull Boolean> ofBoolean(boolean @StrictNonNull...elements) {
		QSet<Boolean> result = new QSet<>(boolean.class);
		for (boolean t : elements) {
			result.insert(t);
		}
		return result;
	}
    
    /**
     * Returns a QSet containing given elements.
     *
     * @param elements
     * @return a {@code QSet} containing the specified element
     */
    public static @NonNull QSet<@QtPrimitiveType@NonNull Byte> ofBuffer(@StrictNonNull ByteBuffer elements) {
		QSet<Byte> result = new QSet<>(byte.class);
		while(elements.hasRemaining()) {
			result.insert(elements.get());
		}
		return result;
	}
    
    /**
     * Returns a QSet containing given elements.
     *
     * @param elements
     * @return a {@code QSet} containing the specified element
     */
    public static @NonNull QSet<@QtPrimitiveType@NonNull Short> ofBuffer(@StrictNonNull ShortBuffer elements) {
		QSet<Short> result = new QSet<>(short.class);
		while(elements.hasRemaining()) {
			result.insert(elements.get());
		}
		return result;
	}
    
    /**
     * Returns a QSet containing given elements.
     *
     * @param elements
     * @return a {@code QSet} containing the specified element
     */
    public static @NonNull QSet<@QtPrimitiveType@NonNull Integer> ofBuffer(@StrictNonNull IntBuffer elements) {
		QSet<Integer> result = new QSet<>(int.class);
		while(elements.hasRemaining()) {
			result.insert(elements.get());
		}
		return result;
	}
    
    /**
     * Returns a QSet containing given elements.
     *
     * @param elements
     * @return a {@code QSet} containing the specified element
     */
    public static @NonNull QSet<@QtPrimitiveType@NonNull Long> ofBuffer(@StrictNonNull LongBuffer elements) {
		QSet<Long> result = new QSet<>(long.class);
		while(elements.hasRemaining()) {
			result.insert(elements.get());
		}
		return result;
	}
    
    /**
     * Returns a QSet containing given elements.
     *
     * @param elements
     * @return a {@code QSet} containing the specified element
     */
    public static @NonNull QSet<@QtPrimitiveType@NonNull Character> ofBuffer(@StrictNonNull CharBuffer elements) {
		QSet<Character> result = new QSet<>(char.class);
		while(elements.hasRemaining()) {
			result.insert(elements.get());
		}
		return result;
	}
    
    /**
     * Returns a QSet containing given elements.
     *
     * @param elements
     * @return a {@code QSet} containing the specified element
     */
    public static @NonNull QSet<@QtPrimitiveType@NonNull Float> ofBuffer(@StrictNonNull FloatBuffer elements) {
		QSet<Float> result = new QSet<>(float.class);
		while(elements.hasRemaining()) {
			result.insert(elements.get());
		}
		return result;
	}
    
    /**
     * Returns a QSet containing given elements.
     *
     * @param elements
     * @return a {@code QSet} containing the specified element
     */
    public static @NonNull QSet<@QtPrimitiveType@NonNull Double> ofBuffer(@StrictNonNull DoubleBuffer elements) {
		QSet<Double> result = new QSet<>(double.class);
		while(elements.hasRemaining()) {
			result.insert(elements.get());
		}
		return result;
	}
    
    /**
     * Returns a QSet containing given elements.
     *
     * @param <T> the {@code QSet}'s element type
     * @param type the {@code QSet}'s element type
     * @param elements all list elements to be added
     * @return a {@code QSet} containing the specified element
     *
     */
	@SafeVarargs
    public static <T> @NonNull QSet<T> ofTyped(@Nullable Class<? super T> type, T @StrictNonNull...elements) {
		QMetaType metaType = QList.findElementMetaType(type, elements);
		return ofTyped(metaType, elements);
	}
    
    /**
     * Returns a QSet containing given elements.
     *
     * @param <T> the {@code QSet}'s element type
     * @param metaType the {@code QSet}'s element type
     * @param elements all list elements to be added
     * @return a {@code QSet} containing the specified element
     *
     */
	@SafeVarargs
    public static <T> @NonNull QSet<T> ofTyped(@StrictNonNull QMetaType metaType, T @StrictNonNull...elements) {
		if(metaType.id()==0)
			throw new IllegalArgumentException("QMetaType::UnknownType cannot be type of QSet.");
		if(metaType.id()==QMetaType.Type.Void.value())
			throw new IllegalArgumentException("void cannot be type of QSet.");
		return new QSet<>(metaType, Arrays.asList(elements));
	}
    
    static RuntimeException handleException(RuntimeException e, QMetaType elementMetaType, Object value) {
		if((   (e.getClass()==RuntimeException.class && e.getMessage()!=null && e.getMessage().startsWith("Cannot cast to type")) 
			|| (e.getClass()==IllegalArgumentException.class && e.getMessage()!=null && e.getMessage().startsWith("Cannot cast object of type "))
			|| e instanceof ClassCastException ) && value!=null) {
			Class<?> elementType = elementMetaType.javaType();
			if(Collection.class.isAssignableFrom(elementType)) {
				elementType = Collection.class;
			}else if(Map.class.isAssignableFrom(elementType)) {
				elementType = Map.class;
			}
			if(!elementType.isInstance(value)) {
				return new IllegalArgumentException(String.format("Wrong argument given: %1$s, expected: %2$s", value.getClass().getTypeName(), elementType.getTypeName()), e);
			}else {
				return new IllegalArgumentException(e.getMessage(), e);
			}
		}
		return e;
    }
}
