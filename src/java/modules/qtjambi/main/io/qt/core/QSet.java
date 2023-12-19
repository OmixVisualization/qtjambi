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
import java.util.Iterator;
import java.util.Map;
import java.util.Objects;
import java.util.Set;
import java.util.function.Predicate;

import io.qt.NativeAccess;
import io.qt.QNoImplementationException;
import io.qt.QNoNativeResourcesException;
import io.qt.QtUninvokable;

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
	public QSet(Class<T> elementType) {
		super(null);
		QMetaType metaType = QMetaType.fromType(elementType);
		initialize(elementType, QtJambi_LibraryUtilities.internal.nativeId(metaType), null);
	}
	
    /**
     * Creating a container with given element type.
     * <p>See <code><a href="https://doc.qt.io/qt/qset.html#QSet">QSet::<wbr>QSet()</a></code></p>
     * @param metaType the type T
     */
	public QSet(QMetaType.Type metaType) {
		this(new QMetaType(metaType));
	}
    
    /**
     * Creating a container with given element type.
     * <p>See <code><a href="https://doc.qt.io/qt/qset.html#QSet">QSet::<wbr>QSet()</a></code></p>
     * @param metaType the type T
     */
	public QSet(QMetaType metaType) {
		super(null);
		initialize(metaType.javaType(), QtJambi_LibraryUtilities.internal.nativeId(metaType), null);
	}
    
    /**
     * Creating a container with given content.
     * <p>See <code><a href="https://doc.qt.io/qt/qset.html#QSet">QSet::<wbr>QSet(const QSet&lt;T> &amp;)</a></code></p>
     * @param other container
     */
    public QSet(Collection<T> other) {
        super(null);
		QMetaType metaType = QList.findElementMetaType(Objects.requireNonNull(other));
		initialize(metaType.javaType(), QtJambi_LibraryUtilities.internal.nativeId(metaType), other);
    }
    
    @QtUninvokable
    private native void initialize(Class<?> elementType, long elementMetaType, Collection<T> other);
    
    /**
     * Creates and returns a copy of this object.
     */
    @Override
	public QSet<T> clone(){
		return new QSet<>(this);
	}
    
    /**
     * Provides a constant C++ iterator to the containers begin.
     * <p>See <code><a href="https://doc.qt.io/qt/qset.html#constBegin">QSet::constBegin()const</a></code></p>
     * @return begin
     */
    @QtUninvokable
    protected final QSequentialConstIterator<T> constBegin()    {
        return constBegin(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private static native <T> QSequentialConstIterator<T> constBegin(long __this__nativeId);

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qset.html#capacity">QSet::<wbr>capacity()const</a></code></p>
     */
    @QtUninvokable
    public final int capacity()    {
        return capacity(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private static native <T> int capacity(long __this__nativeId);

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qset.html#clear">QSet::<wbr>clear()</a></code></p>
     */
    @QtUninvokable
    public final void clear()    {
        clear(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private static native <T> void clear(long __this__nativeId);

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
    private static native <T> boolean contains(long __this__nativeId, T t);

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
    protected final QSequentialConstIterator<T> constEnd()    {
        return constEnd(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private static native <T> QSequentialConstIterator<T> constEnd(long __this__nativeId);

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
    private static native <T> void insert(long __this__nativeId, T t);

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qset.html#intersect">QSet::<wbr>intersect(QSet&lt;T>)</a></code></p>
     */
    @QtUninvokable
    public final void intersect(java.util.Collection<T> other)    {
        intersect(QtJambi_LibraryUtilities.internal.nativeId(this), other);
    }
    @QtUninvokable
    private static native <T> void intersect(long __this__nativeId, java.util.Collection<T> other);

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qset.html#intersects">QSet::<wbr>intersects(QSet&lt;T>)const</a></code></p>
     */
    @QtUninvokable
    public final boolean intersects(java.util.Collection<T> other)    {
        return intersects(QtJambi_LibraryUtilities.internal.nativeId(this), other);
    }
    @QtUninvokable
    private static native <T> boolean intersects(long __this__nativeId, java.util.Collection<T> other);

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
    private static native <T> boolean remove(long __this__nativeId, T t);
    
    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qset.html#removeIf">QSet::<wbr>removeIf(Predicate)</a></code></p>
	 */
    @QtUninvokable
    public final boolean removeIf(Predicate<? super T> predicate)    {
        Objects.requireNonNull(predicate);
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
    private static native <T> void reserve(long __this__nativeId, int size);

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qset.html#size">QSet::<wbr>size()const</a></code></p>
     */
    @QtUninvokable
    public final int size()    {
        return size(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private static native <T> int size(long __this__nativeId);

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qset.html#subtract">QSet::<wbr>subtract(QSet&lt;T>)</a></code></p>
     */
    @QtUninvokable
    public final void subtract(java.util.Collection<T> other)    {
        subtract(QtJambi_LibraryUtilities.internal.nativeId(this), other);
    }
    @QtUninvokable
    private static native <T> void subtract(long __this__nativeId, java.util.Collection<T> other);

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qset.html#unite">QSet::<wbr>unite(QSet&lt;T>)</a></code></p>
     */
    @QtUninvokable
    public final void unite(java.util.Collection<T> other)    {
        unite(QtJambi_LibraryUtilities.internal.nativeId(this), other);
    }
    @QtUninvokable
    private static native <T> void unite(long __this__nativeId, java.util.Collection<T> other);

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qset.html#values">QSet::<wbr>values()const</a></code></p>
     */
    @QtUninvokable
    public final QList<T> values()    {
        return values(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private static native <T> QList<T> values(long __this__nativeId);

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
    private static native boolean operator_equal(long __this__nativeId, java.util.Collection<?> l);

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
    private static native int hashCode(long __this__nativeId);
    
    /**
     * Returns the string representation of the object given by <code>QVariant(this).toString()</code>.
     */
    @Override
    @QtUninvokable
    public String toString() {
    	try {
			return toString(QtJambi_LibraryUtilities.internal.nativeId(this));
		} catch (QNoImplementationException e) {
			return super.toString();
		} catch (QNoNativeResourcesException e) {
			return "null";
		}
    }
    @QtUninvokable
    private static native String toString(long __this__nativeId);
    
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
    public void writeTo(io.qt.core.QDataStream stream){
        writeTo(QtJambi_LibraryUtilities.internal.nativeId(this), QtJambi_LibraryUtilities.internal.nativeId(stream));
    }
    
    @io.qt.QtUninvokable
    private native void writeTo(long __this__nativeId, long stream);
    
    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qset.html#operator-gt-gt">operator&gt;&gt;(QDataStream&amp;,<wbr>QSet&lt;T>&amp;)</a></code></p>
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
    private static native QMetaType elementMetaType(long __this_nativeId);
    
    /**
     * Returns a QSet containing given elements.
     *
     * @param <T> the {@code QSet}'s element type
     * @param element0 the first element
     * @param elements subsequent elements
     * @return a {@code QSet} containing the specified element
     * @throws NullPointerException if elements are {@code null}
     */
    @SafeVarargs
    public static <T> QSet<T> of(T element0, T...elements) {
		QMetaType metaType = QList.findElementMetaType(element0, elements);
		QSet<T> result = new QSet<>(metaType);
		result.add(element0);
		for (T t : elements) {
			result.add(t);
		}
		return result;
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
