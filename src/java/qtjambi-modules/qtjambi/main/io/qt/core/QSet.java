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
public class QSet<T> extends io.qt.internal.QtJambiCollectionObject<T> implements Set<T>, Cloneable
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
    protected final QIterator<T> begin()    {
        return __qt_QSet_begin(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private static native <T> QIterator<T> __qt_QSet_begin(long __this__nativeId);

    @QtUninvokable
    public final int capacity()    {
        return __qt_QSet_capacity(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private static native <T> int __qt_QSet_capacity(long __this__nativeId);

    @QtUninvokable
    public final void clear()    {
        __qt_QSet_clear(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private static native <T> void __qt_QSet_clear(long __this__nativeId);

    @QtUninvokable
    public final boolean contains(Object t)    {
        try {
        	return __qt_QSet_contains(QtJambi_LibraryUtilities.internal.nativeId(this), t);
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
    private static native <T> boolean __qt_QSet_contains(long __this__nativeId, T t);

    @QtUninvokable
    public final int count()    {
        return size();
    }

    @QtUninvokable
    protected final QIterator<T> end()    {
        return __qt_QSet_end(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private static native <T> QIterator<T> __qt_QSet_end(long __this__nativeId);

    @QtUninvokable
    public final void insert(T t)    {
    	try {
    		__qt_QSet_insert(QtJambi_LibraryUtilities.internal.nativeId(this), t);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QSet.handleException(e, elementMetaType(), t);
    	}
    }
    @QtUninvokable
    private static native <T> void __qt_QSet_insert(long __this__nativeId, T t);

    @QtUninvokable
    public final void intersect(java.util.Collection<T> other)    {
        __qt_QSet_intersect(QtJambi_LibraryUtilities.internal.nativeId(this), other);
    }
    @QtUninvokable
    private static native <T> void __qt_QSet_intersect(long __this__nativeId, java.util.Collection<T> other);

    @QtUninvokable
    public final boolean intersects(java.util.Collection<T> other)    {
        return __qt_QSet_intersects(QtJambi_LibraryUtilities.internal.nativeId(this), other);
    }
    @QtUninvokable
    private static native <T> boolean __qt_QSet_intersects(long __this__nativeId, java.util.Collection<T> other);

    @QtUninvokable
    public final boolean isEmpty()    {
        return size()==0;
    }

    @QtUninvokable
    private final boolean operator_equal(java.util.Collection<T> l)    {
        return __qt_QSet_operator_equal(QtJambi_LibraryUtilities.internal.nativeId(this), l);
    }
    @QtUninvokable
    private static native <T> boolean __qt_QSet_operator_equal(long __this__nativeId, java.util.Collection<T> l);

    @QtUninvokable
    public final boolean remove(Object t)    {
    	try {
    		return __qt_QSet_remove(QtJambi_LibraryUtilities.internal.nativeId(this), t);
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
    private static native <T> boolean __qt_QSet_remove(long __this__nativeId, T t);

    @QtUninvokable
    public final void reserve(int size)    {
        __qt_QSet_reserve(QtJambi_LibraryUtilities.internal.nativeId(this), size);
    }
    @QtUninvokable
    private static native <T> void __qt_QSet_reserve(long __this__nativeId, int size);

    @QtUninvokable
    public final int size()    {
        return __qt_QSet_size(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private static native <T> int __qt_QSet_size(long __this__nativeId);

    @QtUninvokable
    public final void subtract(java.util.Collection<T> other)    {
        __qt_QSet_subtract(QtJambi_LibraryUtilities.internal.nativeId(this), other);
    }
    @QtUninvokable
    private static native <T> void __qt_QSet_subtract(long __this__nativeId, java.util.Collection<T> other);

    @QtUninvokable
    public final void unite(java.util.Collection<T> other)    {
        __qt_QSet_unite(QtJambi_LibraryUtilities.internal.nativeId(this), other);
    }
    @QtUninvokable
    private static native <T> void __qt_QSet_unite(long __this__nativeId, java.util.Collection<T> other);

    @QtUninvokable
    public final QList<T> values()    {
        return __qt_QSet_values(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private static native <T> QList<T> __qt_QSet_values(long __this__nativeId);

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
        __qt_QSet_writeTo(QtJambi_LibraryUtilities.internal.nativeId(this), QtJambi_LibraryUtilities.internal.nativeId(stream));
    }
    
    @io.qt.QtUninvokable
    private native void __qt_QSet_writeTo(long __this__nativeId, long stream);
    
    @io.qt.QtUninvokable
    public void readFrom(io.qt.core.QDataStream stream){
        __qt_QSet_readFrom(QtJambi_LibraryUtilities.internal.nativeId(this), QtJambi_LibraryUtilities.internal.nativeId(stream));
    }
    
    @io.qt.QtUninvokable
    private native void __qt_QSet_readFrom(long __this__nativeId, long stream);
    
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
