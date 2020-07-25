/****************************************************************************
**
** Copyright (C) 2009-2020 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import io.qt.QtUninvokable;


public class QSet<T> extends io.qt.internal.QtJambiSetObject<T>
{

	static {
    	io.qt.core.QtJambi_LibraryInitializer.init();
    }

    private final long beginFunction;
    private final long capacityFunction;
    private final long clearFunction;
    private final long containsFunction;
    private final long endFunction;
    private final long insertFunction;
    private final long intersectFunction;
    private final long intersectsFunction;
    private final long equalFunction;
    private final long removeFunction;
    private final long reserveFunction;
    private final long sizeFunction;
    private final long subtractFunction;
    private final long uniteFunction;
    private final long valuesFunction;

	private QSet(Class<T> elementType, long beginFunction, long capacityFunction, long clearFunction,
			long containsFunction, long endFunction, long insertFunction, long intersectFunction,
			long intersectsFunction, long equalFunction, long removeFunction, long reserveFunction,
			long sizeFunction, long subtractFunction,
			long uniteFunction, long valuesFunction) {
		super(null, elementType);
		this.beginFunction = beginFunction;
		this.capacityFunction = capacityFunction;
		this.clearFunction = clearFunction;
		this.containsFunction = containsFunction;
		this.endFunction = endFunction;
		this.insertFunction = insertFunction;
		this.intersectFunction = intersectFunction;
		this.intersectsFunction = intersectsFunction;
		this.equalFunction = equalFunction;
		this.removeFunction = removeFunction;
		this.reserveFunction = reserveFunction;
		this.sizeFunction = sizeFunction;
		this.subtractFunction = subtractFunction;
		this.uniteFunction = uniteFunction;
		this.valuesFunction = valuesFunction;
	}

	private QSet(Class<T> elementType, long beginFunction, long capacityFunction,
			long containsFunction, long endFunction, long intersectsFunction, 
			long equalFunction, long sizeFunction, long valuesFunction) {
		super(null, elementType);
		this.beginFunction = beginFunction;
		this.capacityFunction = capacityFunction;
		this.clearFunction = 0;
		this.containsFunction = containsFunction;
		this.endFunction = endFunction;
		this.insertFunction = 0;
		this.intersectFunction = 0;
		this.intersectsFunction = intersectsFunction;
		this.equalFunction = equalFunction;
		this.removeFunction = 0;
		this.reserveFunction = 0;
		this.sizeFunction = sizeFunction;
		this.subtractFunction = 0;
		this.uniteFunction = 0;
		this.valuesFunction = valuesFunction;
	}
    
    @QtUninvokable
    protected final QIterator<T> begin()    {
        if(beginFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QSet_begin(io.qt.internal.QtJambiInternal.checkedNativeId(this), beginFunction);
    }
    @QtUninvokable
    static native <T> QIterator<T> __qt_QSet_begin(long __this__nativeId, long beginFunction);

    @QtUninvokable
    public final int capacity()    {
        if(capacityFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QSet_capacity(io.qt.internal.QtJambiInternal.checkedNativeId(this), capacityFunction);
    }
    @QtUninvokable
    static native <T> int __qt_QSet_capacity(long __this__nativeId, long capacityFunction);

    @QtUninvokable
    public final void clear()    {
        if(clearFunction==0)
        	throw new UnsupportedOperationException();
        __qt_QSet_clear(io.qt.internal.QtJambiInternal.checkedNativeId(this), clearFunction);
    }
    @QtUninvokable
    static native <T> void __qt_QSet_clear(long __this__nativeId, long clearFunction);

    @QtUninvokable
    public final boolean contains(Object t)    {
        if(containsFunction==0)
        	throw new UnsupportedOperationException();
        return checkElement(t) && __qt_QSet_contains(io.qt.internal.QtJambiInternal.checkedNativeId(this), castElement(t), containsFunction);
    }
    @QtUninvokable
    static native <T> boolean __qt_QSet_contains(long __this__nativeId, T t, long containsFunction);

    @QtUninvokable
    public final int count()    {
        return size();
    }

    @QtUninvokable
    protected final QIterator<T> end()    {
        if(endFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QSet_end(io.qt.internal.QtJambiInternal.checkedNativeId(this), endFunction);
    }
    @QtUninvokable
    static native <T> QIterator<T> __qt_QSet_end(long __this__nativeId, long endFunction);

    @QtUninvokable
    public final void insert(T t)    {
        if(insertFunction==0)
        	throw new UnsupportedOperationException();
        __qt_QSet_insert(io.qt.internal.QtJambiInternal.checkedNativeId(this), t, insertFunction);
    }
    @QtUninvokable
    static native <T> void __qt_QSet_insert(long __this__nativeId, T t, long appendFunction);

    @QtUninvokable
    public final void intersect(java.util.Collection<T> other)    {
        if(intersectFunction==0)
        	throw new UnsupportedOperationException();
        __qt_QSet_intersect(io.qt.internal.QtJambiInternal.checkedNativeId(this), other, intersectFunction);
    }
    @QtUninvokable
    static native <T> void __qt_QSet_intersect(long __this__nativeId, java.util.Collection<T> other, long intersectFunction);

    @QtUninvokable
    public final boolean intersects(java.util.Collection<T> other)    {
        if(intersectsFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QSet_intersects(io.qt.internal.QtJambiInternal.checkedNativeId(this), other, intersectsFunction);
    }
    @QtUninvokable
    static native <T> boolean __qt_QSet_intersects(long __this__nativeId, java.util.Collection<T> other, long intersectsFunction);

    @QtUninvokable
    public final boolean isEmpty()    {
        return size()==0;
    }

    @QtUninvokable
    private final boolean operator_equal(java.util.Collection<T> l)    {
        if(equalFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QSet_operator_equal(io.qt.internal.QtJambiInternal.checkedNativeId(this), l, equalFunction);
    }
    @QtUninvokable
    static native <T> boolean __qt_QSet_operator_equal(long __this__nativeId, java.util.Collection<T> l, long equalFunction);

    @QtUninvokable
    public final boolean remove(Object t)    {
        if(removeFunction==0)
        	throw new UnsupportedOperationException();
        return checkElement(t) ? __qt_QSet_remove(io.qt.internal.QtJambiInternal.checkedNativeId(this), t, removeFunction) : false;
    }
    @QtUninvokable
    static native <T> boolean __qt_QSet_remove(long __this__nativeId, T t, long removeOneFunction);

    @QtUninvokable
    public final void reserve(int size)    {
        if(reserveFunction==0)
        	throw new UnsupportedOperationException();
        __qt_QSet_reserve(io.qt.internal.QtJambiInternal.checkedNativeId(this), size, reserveFunction);
    }
    @QtUninvokable
    static native <T> void __qt_QSet_reserve(long __this__nativeId, int size, long reserveFunction);

    @QtUninvokable
    public final int size()    {
        if(sizeFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QSet_size(io.qt.internal.QtJambiInternal.checkedNativeId(this), sizeFunction);
    }
    @QtUninvokable
    static native <T> int __qt_QSet_size(long __this__nativeId, long sizeFunction);

    @QtUninvokable
    public final void subtract(java.util.Collection<T> other)    {
        if(subtractFunction==0)
        	throw new UnsupportedOperationException();
        __qt_QSet_subtract(io.qt.internal.QtJambiInternal.checkedNativeId(this), other, subtractFunction);
    }
    @QtUninvokable
    static native <T> void __qt_QSet_subtract(long __this__nativeId, java.util.Collection<T> other, long subtractFunction);

    @QtUninvokable
    public final void unite(java.util.Collection<T> other)    {
        if(uniteFunction==0)
        	throw new UnsupportedOperationException();
        __qt_QSet_unite(io.qt.internal.QtJambiInternal.checkedNativeId(this), other, uniteFunction);
    }
    @QtUninvokable
    static native <T> void __qt_QSet_unite(long __this__nativeId, java.util.Collection<T> other, long uniteFunction);

    @QtUninvokable
    public final java.util.List<T> values()    {
        if(valuesFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QSet_values(io.qt.internal.QtJambiInternal.checkedNativeId(this), valuesFunction);
    }
    @QtUninvokable
    static native <T> java.util.List<T> __qt_QSet_values(long __this__nativeId, long valuesFunction);

    @Override
    @QtUninvokable
    public boolean equals(Object other) {
        if (other instanceof java.util.Collection) {
            @SuppressWarnings("unchecked")
            java.util.Collection<T> __tmp = (java.util.Collection<T>) other;
            return operator_equal(__tmp);
        }
        return false;
    }


    @Override
    @QtUninvokable
    public int hashCode() {
    	int hashCode = getClass().hashCode();
        for (T e : this)
            hashCode = 31*hashCode + (e==null ? 0 : e.hashCode());
        return hashCode;
    }
    
    @Override
    @QtUninvokable
    public boolean add(T e){
        insert(e);
        return true;
    }

}
