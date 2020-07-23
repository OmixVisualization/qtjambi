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

public class QLinkedList<T> extends io.qt.internal.QtJambiLinkedListObject<T>
{

	static {
    	io.qt.core.QtJambi_LibraryInitializer.init();
    }

    private final long appendFunction;
    private final long beginFunction;
    private final long clearFunction;
    private final long containsFunction;
    private final long countObjectFunction;
    private final long endFunction;
    private final long endsWithFunction;
    private final long firstFunction;
    private final long lastFunction;
    private final long equalFunction;
    private final long prependFunction;
    private final long removeAllFunction;
    private final long removeFirstFunction;
    private final long removeLastFunction;
    private final long removeOneFunction;
    private final long sizeFunction;
    private final long startsWithFunction;
    private final long takeFirstFunction;
    private final long takeLastFunction;
    
    private QLinkedList(Class<T> elementType, long appendFunction,
			long beginFunction, long clearFunction, long containsFunction, long countObjectFunction,
			long endFunction, long endsWithFunction, long firstFunction,
			long lastFunction, long equalFunction, long prependFunction,
			long removeAllFunction, long removeFirstFunction, long removeLastFunction, long removeOneFunction,
			long sizeFunction, long startsWithFunction, long takeFirstFunction,
			long takeLastFunction) {
		super((QPrivateConstructor)null, elementType);
		this.appendFunction = appendFunction;
		this.beginFunction = beginFunction;
		this.clearFunction = clearFunction;
		this.containsFunction = containsFunction;
		this.countObjectFunction = countObjectFunction;
		this.endFunction = endFunction;
		this.endsWithFunction = endsWithFunction;
		this.firstFunction = firstFunction;
		this.lastFunction = lastFunction;
		this.equalFunction = equalFunction;
		this.prependFunction = prependFunction;
		this.removeAllFunction = removeAllFunction;
		this.removeFirstFunction = removeFirstFunction;
		this.removeLastFunction = removeLastFunction;
		this.removeOneFunction = removeOneFunction;
		this.sizeFunction = sizeFunction;
		this.startsWithFunction = startsWithFunction;
		this.takeFirstFunction = takeFirstFunction;
		this.takeLastFunction = takeLastFunction;
	}
    
    private QLinkedList(Class<T> elementType,
			long beginFunction, long containsFunction, long countObjectFunction,
			long endFunction, long endsWithFunction, long firstFunction,
			long lastFunction, long equalFunction, long sizeFunction, long startsWithFunction) {
    	this(elementType, 0, beginFunction, 0, containsFunction, countObjectFunction,
    			endFunction, endsWithFunction, firstFunction,
    			lastFunction, equalFunction, 0, 0, 0, 0, 0,
    			sizeFunction, startsWithFunction, 0, 0);
    }
    
	@QtUninvokable
    public final void append(T t)    {
        if(appendFunction==0)
        	throw new UnsupportedOperationException();
        __qt_QLinkedList_append(io.qt.internal.QtJambiInternal.checkedNativeId(this), t, appendFunction);
    }
    @QtUninvokable
    static native <T> void __qt_QLinkedList_append(long __this__nativeId, T t, long appendFunction);

    @QtUninvokable
    protected final QIterator<T> begin()    {
        if(beginFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QLinkedList_begin(io.qt.internal.QtJambiInternal.checkedNativeId(this), beginFunction);
    }
    @QtUninvokable
    static native <T> QIterator<T> __qt_QLinkedList_begin(long __this__nativeId, long beginFunction);

    @QtUninvokable
    public final void clear()    {
        if(clearFunction==0)
        	throw new UnsupportedOperationException();
        __qt_QLinkedList_clear(io.qt.internal.QtJambiInternal.checkedNativeId(this), clearFunction);
    }
    @QtUninvokable
    static native <T> void __qt_QLinkedList_clear(long __this__nativeId, long clearFunction);

    @QtUninvokable
    public final boolean contains(Object t)    {
        if(containsFunction==0)
        	throw new UnsupportedOperationException();
        return t==null || checkElement(t) && __qt_QLinkedList_contains(io.qt.internal.QtJambiInternal.checkedNativeId(this), castElement(t), containsFunction);
    }
    @QtUninvokable
    static native <T> boolean __qt_QLinkedList_contains(long __this__nativeId, T t, long containsFunction);

    @QtUninvokable
    public final int count()    {
        return size();
    }

    @QtUninvokable
    public final int count(T t)    {
        if(countObjectFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QLinkedList_count(io.qt.internal.QtJambiInternal.checkedNativeId(this), t, countObjectFunction);
    }
    @QtUninvokable
    static native <T> int __qt_QLinkedList_count(long __this__nativeId, T t, long countObjectFunction);

    @QtUninvokable
    protected final QIterator<T> end()    {
        if(endFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QLinkedList_end(io.qt.internal.QtJambiInternal.checkedNativeId(this), endFunction);
    }
    @QtUninvokable
    static native <T> QIterator<T> __qt_QLinkedList_end(long __this__nativeId, long endFunction);

    @QtUninvokable
    public final boolean endsWith(T t)    {
        if(endsWithFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QLinkedList_endsWith(io.qt.internal.QtJambiInternal.checkedNativeId(this), t, endsWithFunction);
    }
    @QtUninvokable
    static native <T> boolean __qt_QLinkedList_endsWith(long __this__nativeId, T t, long endsWithFunction);

    @QtUninvokable
    public final T first()    {
        if(firstFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QLinkedList_first(io.qt.internal.QtJambiInternal.checkedNativeId(this), firstFunction);
    }
    @QtUninvokable
    private native T __qt_QLinkedList_first(long __this__nativeId, long firstFunction);

    @QtUninvokable
    public final boolean isEmpty()    {
        return size()==0;
    }

    @QtUninvokable
    public final T last()    {
        if(lastFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QLinkedList_last(io.qt.internal.QtJambiInternal.checkedNativeId(this), lastFunction);
    }
    @QtUninvokable
    private native T __qt_QLinkedList_last(long __this__nativeId, long lastFunction);

    @QtUninvokable
    private final boolean operator_equal(java.util.Collection<T> l)    {
        if(equalFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QLinkedList_operator_equal(io.qt.internal.QtJambiInternal.checkedNativeId(this), l, equalFunction);
    }
    @QtUninvokable
    static native <T> boolean __qt_QLinkedList_operator_equal(long __this__nativeId, java.util.Collection<T> l, long equalFunction);

    @QtUninvokable
    public final void prepend(T t)    {
        if(prependFunction==0)
        	throw new UnsupportedOperationException();
        __qt_QLinkedList_prepend(io.qt.internal.QtJambiInternal.checkedNativeId(this), t, prependFunction);
    }
    @QtUninvokable
    static native <T> void __qt_QLinkedList_prepend(long __this__nativeId, T t, long prependFunction);

    @QtUninvokable
    public final int removeAll(T t)    {
        if(removeAllFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QLinkedList_removeAll(io.qt.internal.QtJambiInternal.checkedNativeId(this), t, removeAllFunction);
    }
    @QtUninvokable
    static native <T> int __qt_QLinkedList_removeAll(long __this__nativeId, T t, long removeAllFunction);

    @QtUninvokable
    public final T removeFirst()    {
    	T value = first();
        if(removeFirstFunction==0)
        	throw new UnsupportedOperationException();
        __qt_QLinkedList_removeFirst(io.qt.internal.QtJambiInternal.checkedNativeId(this), removeFirstFunction);
        return value;
    }
    @QtUninvokable
    private native void __qt_QLinkedList_removeFirst(long __this__nativeId, long removeFirstFunction);

    @QtUninvokable
    public final T removeLast()    {
    	T value = last();
        if(removeLastFunction==0)
        	throw new UnsupportedOperationException();
        __qt_QLinkedList_removeLast(io.qt.internal.QtJambiInternal.checkedNativeId(this), removeLastFunction);
        return value;
    }
    @QtUninvokable
    private native void __qt_QLinkedList_removeLast(long __this__nativeId, long removeLastFunction);

    @QtUninvokable
    public final boolean removeOne(T t)    {
        if(removeOneFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QLinkedList_removeOne(io.qt.internal.QtJambiInternal.checkedNativeId(this), t, removeOneFunction);
    }
    @QtUninvokable
    static native <T> boolean __qt_QLinkedList_removeOne(long __this__nativeId, T t, long removeOneFunction);

    @QtUninvokable
    public final int size()    {
        if(sizeFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QLinkedList_size(io.qt.internal.QtJambiInternal.checkedNativeId(this), sizeFunction);
    }
    @QtUninvokable
    static native <T> int __qt_QLinkedList_size(long __this__nativeId, long sizeFunction);

    @QtUninvokable
    public final boolean startsWith(T t)    {
        if(startsWithFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QLinkedList_startsWith(io.qt.internal.QtJambiInternal.checkedNativeId(this), t, startsWithFunction);
    }
    @QtUninvokable
    static native <T> boolean __qt_QLinkedList_startsWith(long __this__nativeId, T t, long startsWithFunction);

    @QtUninvokable
    public final int takeFirst()    {
        if(takeFirstFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QLinkedList_takeFirst(io.qt.internal.QtJambiInternal.checkedNativeId(this), takeFirstFunction);
    }
    @QtUninvokable
    private native int __qt_QLinkedList_takeFirst(long __this__nativeId, long takeFirstFunction);

    @QtUninvokable
    public final int takeLast()    {
        if(takeLastFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QLinkedList_takeLast(io.qt.internal.QtJambiInternal.checkedNativeId(this), takeLastFunction);
    }
    @QtUninvokable
    private native int __qt_QLinkedList_takeLast(long __this__nativeId, long takeLastFunction);

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
        if(e!=null){
            append(e);
            return true;
        }else
            return false;
    }

    @Override
    @QtUninvokable
    public boolean remove(Object e){
        return checkElement(e) ? removeOne(castElement(e)) : false;
    }

	@Override
    @QtUninvokable
	public T peekFirst() {
		return first();
	}

	@Override
    @QtUninvokable
	public T peekLast() {
		return last();
	}

	@Override
    @QtUninvokable
	public void push(T e) {
		this.append(e);
	}

	@Override
    @QtUninvokable
	public T pop() {
		return removeFirst();
	}

	@Override
    @QtUninvokable
	protected void removeLastImpl() {
		removeLast();
	}
}
