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
import java.util.Map;
import java.util.Objects;
import java.util.Set;

import io.qt.NativeAccess;
import io.qt.QNoImplementationException;
import io.qt.QNoNativeResourcesException;
import io.qt.QtUninvokable;

/**
 * <p>Java wrapper for Qt class <a href="https://doc.qt.io/qt/qset.html">QSet</a></p>
 */
public class QSet<T> extends io.qt.internal.AbstractSequentialContainer<T> implements Set<T>, Cloneable
{

	static {
    	QtJambi_LibraryUtilities.initialize();
    }

    @NativeAccess
    protected QSet(QPrivateConstructor p) {
		super(p);
	}
    
	public QSet(Class<T> elementType) {
		super(null);
		QMetaType metaType = QMetaType.fromType(elementType);
		if(metaType.id()==0)
			throw new IllegalArgumentException("QMetaType::UnknownType cannot be type of QSet.");
		if(metaType.id()==QMetaType.Type.QVariant.value())
			throw new IllegalArgumentException("QVariant cannot be type of QSet.");
		if(metaType.id()==QMetaType.Type.Void.value())
			throw new IllegalArgumentException("void cannot be type of QSet.");
		initialize(elementType, QtJambi_LibraryUtilities.internal.nativeId(metaType), null);
	}
	
	public QSet(QMetaType.Type metaType) {
		this(new QMetaType(metaType));
	}
    
	public QSet(QMetaType metaType) {
		super(null);
		if(metaType.id()==0)
			throw new IllegalArgumentException("QMetaType::UnknownType cannot be type of QSet.");
		if(metaType.id()==QMetaType.Type.QVariant.value())
			throw new IllegalArgumentException("QVariant cannot be type of QSet.");
		if(metaType.id()==QMetaType.Type.Void.value())
			throw new IllegalArgumentException("void cannot be type of QSet.");
		initialize(metaType.javaType(), QtJambi_LibraryUtilities.internal.nativeId(metaType), null);
	}
    
    public QSet(Collection<T> other) {
        super(null);
		QMetaType metaType = QList.findElementMetaType(Objects.requireNonNull(other));
		if(metaType==null || metaType.id()==0)
			throw new IllegalArgumentException("QMetaType::UnknownType cannot be type of QSet.");
		if(metaType.id()==QMetaType.Type.QVariant.value())
			throw new IllegalArgumentException("QVariant cannot be type of QSet.");
		if(metaType.id()==QMetaType.Type.Void.value())
			throw new IllegalArgumentException("void cannot be type of QSet.");
		initialize(metaType.javaType(), QtJambi_LibraryUtilities.internal.nativeId(metaType), other);
    }
    
    @QtUninvokable
    private native void initialize(Class<?> elementType, long elementMetaType, Collection<T> other);
    
    @Override
	public QSet<T> clone(){
		return new QSet<>(this);
	}
    
    @QtUninvokable
    protected final QSequentialConstIterator<T> constBegin()    {
        return constBegin(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private static native <T> QSequentialConstIterator<T> constBegin(long __this__nativeId);

    @QtUninvokable
    public final int capacity()    {
        return capacity(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private static native <T> int capacity(long __this__nativeId);

    @QtUninvokable
    public final void clear()    {
        clear(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private static native <T> void clear(long __this__nativeId);

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

    @QtUninvokable
    public final int count()    {
        return size();
    }

    @QtUninvokable
    protected final QSequentialConstIterator<T> constEnd()    {
        return constEnd(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private static native <T> QSequentialConstIterator<T> constEnd(long __this__nativeId);

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

    @QtUninvokable
    public final void intersect(java.util.Collection<T> other)    {
        intersect(QtJambi_LibraryUtilities.internal.nativeId(this), other);
    }
    @QtUninvokable
    private static native <T> void intersect(long __this__nativeId, java.util.Collection<T> other);

    @QtUninvokable
    public final boolean intersects(java.util.Collection<T> other)    {
        return intersects(QtJambi_LibraryUtilities.internal.nativeId(this), other);
    }
    @QtUninvokable
    private static native <T> boolean intersects(long __this__nativeId, java.util.Collection<T> other);

    @QtUninvokable
    public final boolean isEmpty()    {
        return size()==0;
    }

    @QtUninvokable
    private final boolean operator_equal(java.util.Collection<T> l)    {
        return operator_equal(QtJambi_LibraryUtilities.internal.nativeId(this), l);
    }
    @QtUninvokable
    private static native <T> boolean operator_equal(long __this__nativeId, java.util.Collection<T> l);

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

    @QtUninvokable
    public final void reserve(int size)    {
        reserve(QtJambi_LibraryUtilities.internal.nativeId(this), size);
    }
    @QtUninvokable
    private static native <T> void reserve(long __this__nativeId, int size);

    @QtUninvokable
    public final int size()    {
        return size(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private static native <T> int size(long __this__nativeId);

    @QtUninvokable
    public final void subtract(java.util.Collection<T> other)    {
        subtract(QtJambi_LibraryUtilities.internal.nativeId(this), other);
    }
    @QtUninvokable
    private static native <T> void subtract(long __this__nativeId, java.util.Collection<T> other);

    @QtUninvokable
    public final void unite(java.util.Collection<T> other)    {
        unite(QtJambi_LibraryUtilities.internal.nativeId(this), other);
    }
    @QtUninvokable
    private static native <T> void unite(long __this__nativeId, java.util.Collection<T> other);

    @QtUninvokable
    public final QList<T> values()    {
        return values(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private static native <T> QList<T> values(long __this__nativeId);

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
    	try {
    		return hashCode(QtJambi_LibraryUtilities.internal.nativeId(this));
		} catch (QNoNativeResourcesException e) {
			return 0;
		}
    }
    @QtUninvokable
    private static native int hashCode(long __this__nativeId);
    
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
    
    @Override
    @QtUninvokable
    public final boolean add(T e){
        insert(e);
        return true;
    }
    
    @io.qt.QtUninvokable
    public void writeTo(io.qt.core.QDataStream stream){
        writeTo(QtJambi_LibraryUtilities.internal.nativeId(this), QtJambi_LibraryUtilities.internal.nativeId(stream));
    }
    
    @io.qt.QtUninvokable
    private native void writeTo(long __this__nativeId, long stream);
    
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
     * @param <E> the {@code QSet}'s element type
     * @param element0 the first element
     * @param elements subsequent elements
     * @return a {@code QSet} containing the specified element
     * @throws NullPointerException if elements are {@code null}
     */
    @SafeVarargs
    public static <T> QSet<T> of(T element0, T...elements) {
		QMetaType metaType = QList.findElementMetaType(element0, elements);
		if(metaType==null || metaType.id()==0)
			throw new IllegalArgumentException("QMetaType::UnknownType cannot be type of QList.");
		if(metaType.id()==QMetaType.Type.Void.value())
			throw new IllegalArgumentException("void cannot be type of QList.");
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
