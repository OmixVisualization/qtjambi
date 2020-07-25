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

import static io.qt.core.QList.*;

import io.qt.QtUninvokable;


public class QQueue<T> extends io.qt.internal.QtJambiQueueObject<T>
{

    static {
    	io.qt.core.QtJambi_LibraryInitializer.init();
    }
    
    private final long appendFunction;
    private final long appendListFunction;
    private final long atFunction;
    private final long beginFunction;
    private final long clearFunction;
    private final long containsFunction;
    private final long countObjectFunction;
    private final long endFunction;
    private final long endsWithFunction;
    private final long indexOfFunction;
    private final long insertFunction;
    private final long lastIndexOfFunction;
    private final long midFunction;
    private final long moveFunction;
    private final long equalFunction;
    private final long prependFunction;
    private final long removeAllFunction;
    private final long removeAtFunction;
    private final long removeOneFunction;
    private final long replaceFunction;
    private final long reserveFunction;
    private final long sizeFunction;
    private final long startsWithFunction;
    private final long swapFunction;
    private final long takeAtFunction;
    private final long toSetFunction;
    private final long valueFunction;
    private final long valueDefaultFunction;
    
    private QQueue(Class<T> elementType, long appendFunction, long appendListFunction, long atFunction,
			long beginFunction, long clearFunction, long containsFunction, long countObjectFunction,
			long endFunction, long endsWithFunction,
			long indexOfFunction, long insertFunction, long lastIndexOfFunction,
			long midFunction, long moveFunction, long equalFunction, long prependFunction, long removeAllFunction, long removeAtFunction,
			long removeOneFunction, long replaceFunction, long reserveFunction,
			long sizeFunction, long startsWithFunction, long swapFunction, long takeAtFunction, long toSetFunction,
			long valueFunction, long valueDefaultFunction) {
		super((QPrivateConstructor)null, elementType);
		this.appendFunction = appendFunction;
		this.appendListFunction = appendListFunction;
		this.atFunction = atFunction;
		this.beginFunction = beginFunction;
		this.clearFunction = clearFunction;
		this.containsFunction = containsFunction;
		this.countObjectFunction = countObjectFunction;
		this.endFunction = endFunction;
		this.endsWithFunction = endsWithFunction;
		this.indexOfFunction = indexOfFunction;
		this.insertFunction = insertFunction;
		this.lastIndexOfFunction = lastIndexOfFunction;
		this.midFunction = midFunction;
		this.moveFunction = moveFunction;
		this.equalFunction = equalFunction;
		this.prependFunction = prependFunction;
		this.removeAllFunction = removeAllFunction;
		this.removeAtFunction = removeAtFunction;
		this.removeOneFunction = removeOneFunction;
		this.replaceFunction = replaceFunction;
		this.reserveFunction = reserveFunction;
		this.sizeFunction = sizeFunction;
		this.startsWithFunction = startsWithFunction;
		this.swapFunction = swapFunction;
		this.takeAtFunction = takeAtFunction;
		this.toSetFunction = toSetFunction;
		this.valueFunction = valueFunction;
		this.valueDefaultFunction = valueDefaultFunction;
	}
    
    private QQueue(Class<T> elementType, long atFunction,
			long beginFunction, long containsFunction, long countObjectFunction,
			long endFunction, long endsWithFunction,
			long indexOfFunction, long lastIndexOfFunction,
			long midFunction, long equalFunction, long sizeFunction, long startsWithFunction, long toSetFunction,
			long valueFunction, long valueDefaultFunction) {
		this(elementType, 0, 0, atFunction,
				beginFunction, 0, containsFunction, countObjectFunction,
				endFunction, endsWithFunction,
				indexOfFunction, 0, lastIndexOfFunction,
				midFunction, 0, equalFunction, 0, 0, 0, 0, 0, 0,
				sizeFunction, startsWithFunction, 0, 0, toSetFunction,
				valueFunction, valueDefaultFunction);
	}
    
	@QtUninvokable
    public final void append(java.util.Collection<T> t)    {
        if(appendListFunction==0)
        	throw new UnsupportedOperationException();
        __qt_QList_appendList(io.qt.internal.QtJambiInternal.checkedNativeId(this), t, appendListFunction);
    }
    
    @QtUninvokable
    public final void append(T t)    {
        if(appendFunction==0)
        	throw new UnsupportedOperationException();
        __qt_QList_append(io.qt.internal.QtJambiInternal.checkedNativeId(this), t, appendFunction);
    }
    
    @QtUninvokable
    public final T at(int i)    {
        if(atFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QList_at(io.qt.internal.QtJambiInternal.checkedNativeId(this), i, atFunction);
    }
    
    @QtUninvokable
    protected final QIterator<T> begin()    {
        if(beginFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QList_begin(io.qt.internal.QtJambiInternal.checkedNativeId(this), beginFunction);
    }
    
    @QtUninvokable
    public final void clear()    {
        if(clearFunction==0)
        	throw new UnsupportedOperationException();
        __qt_QList_clear(io.qt.internal.QtJambiInternal.checkedNativeId(this), clearFunction);
    }
    
    @QtUninvokable
    public final boolean contains(Object t)    {
        if(containsFunction==0)
        	throw new UnsupportedOperationException();
        return checkElement(t) && __qt_QList_contains(io.qt.internal.QtJambiInternal.checkedNativeId(this), castElement(t), containsFunction);
    }
    
    @QtUninvokable
    public final int count()    {
        return size();
    }

    @QtUninvokable
    public final int count(T t)    {
        if(countObjectFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QList_count(io.qt.internal.QtJambiInternal.checkedNativeId(this), t, countObjectFunction);
    }
    
    @QtUninvokable
    protected final QIterator<T> end()    {
        if(endFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QList_end(io.qt.internal.QtJambiInternal.checkedNativeId(this), endFunction);
    }
    
    @QtUninvokable
    public final boolean endsWith(T t)    {
        if(endsWithFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QList_endsWith(io.qt.internal.QtJambiInternal.checkedNativeId(this), t, endsWithFunction);
    }
    
    @QtUninvokable
    public final T first()    {
    	if(size()==0)
    		throw new ArrayIndexOutOfBoundsException(0);
    	return at(0);
    }

    @QtUninvokable
    @Override
    public final int indexOf(Object t) {
        return checkElement(t) ? indexOf(castElement(t), (int)0) : -1;
    }
    @QtUninvokable
    public final int indexOf(T t, int from)    {
        if(indexOfFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QList_indexOf(io.qt.internal.QtJambiInternal.checkedNativeId(this), t, from, indexOfFunction);
    }
    
    @QtUninvokable
    public final void insert(int i, T t)    {
        if(insertFunction==0)
        	throw new UnsupportedOperationException();
        __qt_QList_insert(io.qt.internal.QtJambiInternal.checkedNativeId(this), i, t, insertFunction);
    }
    
    @QtUninvokable
    public final boolean isEmpty()    {
        return size()==0;
    }

    @QtUninvokable
    public final T last()    {
    	int size = size();
    	if(size==0)
    		throw new ArrayIndexOutOfBoundsException(0);
    	return at(size-1);
    }

    @QtUninvokable
    @Override
    public final int lastIndexOf(Object t) {
        return checkElement(t) ? lastIndexOf(castElement(t), (int)-1) : -1;
    }
    @QtUninvokable
    public final int lastIndexOf(T t, int from)    {
        if(lastIndexOfFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QList_lastIndexOf(io.qt.internal.QtJambiInternal.checkedNativeId(this), t, from, lastIndexOfFunction);
    }
    
    @QtUninvokable
    public final int length()    {
        return size();
    }

    @QtUninvokable
    public final java.util.List<T> mid(int pos) {
        return mid(pos, (int)-1);
    }
    @QtUninvokable
    public final java.util.List<T> mid(int pos, int length)    {
        if(midFunction==0)
        	throw new UnsupportedOperationException();
        if (pos >= size() || pos < 0) {
        	throw new IndexOutOfBoundsException(String.format("Accessing container of size %1$s at %2$s", size(), pos));
        }
        if (length>=0 && pos+length >= size()) {
        	throw new IndexOutOfBoundsException(String.format("Accessing container of size %1$s from %2$s to %3$s", size(), pos, pos+length));
        }
        return __qt_QList_mid(io.qt.internal.QtJambiInternal.checkedNativeId(this), pos, length, midFunction);
    }

    @QtUninvokable
    public final void move(int from, int to)    {
        if(moveFunction==0)
        	throw new UnsupportedOperationException();
        if (from >= size() || from < 0) {
        	throw new IndexOutOfBoundsException(String.format("Accessing container of size %1$s at %2$s", size(), from));
        }
        if (to >= size() || to < 0) {
        	throw new IndexOutOfBoundsException(String.format("Accessing container of size %1$s at %2$s", size(), to));
        }
        __qt_QList_move(io.qt.internal.QtJambiInternal.checkedNativeId(this), from, to, moveFunction);
    }
    
    @QtUninvokable
    private final boolean operator_equal(java.util.Collection<T> l)    {
        if(equalFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QList_operator_equal(io.qt.internal.QtJambiInternal.checkedNativeId(this), l, equalFunction);
    }
    
    @QtUninvokable
    public final void prepend(T t)    {
        if(prependFunction==0)
        	throw new UnsupportedOperationException();
        __qt_QList_prepend(io.qt.internal.QtJambiInternal.checkedNativeId(this), t, prependFunction);
    }
    
    @QtUninvokable
    public final int removeAll(T t)    {
        if(removeAllFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QList_removeAll(io.qt.internal.QtJambiInternal.checkedNativeId(this), t, removeAllFunction);
    }
    
    @QtUninvokable
    public final void removeAt(int i)    {
        if(removeAtFunction==0)
        	throw new UnsupportedOperationException();
        if (i >= size() || i < 0) {
        	throw new IndexOutOfBoundsException(String.format("Accessing container of size %1$s at %2$s", size(), i));
        }
        __qt_QList_removeAt(io.qt.internal.QtJambiInternal.checkedNativeId(this), i, removeAtFunction);
    }
    
    @QtUninvokable
    public final void removeFirst()    {
    	removeAt(0);
    }

    @QtUninvokable
    public final void removeLast()    {
    	removeAt(size()-1);
    }

    @QtUninvokable
    public final boolean removeOne(T t)    {
        if(removeOneFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QList_removeOne(io.qt.internal.QtJambiInternal.checkedNativeId(this), t, removeOneFunction);
    }
    
    @QtUninvokable
    public final void replace(int i, T t)    {
        if (i >= size() || i < 0) {
        	throw new IndexOutOfBoundsException(String.format("Accessing container of size %1$s at %2$s", size(), i));
        }
        if(replaceFunction==0)
        	throw new UnsupportedOperationException();
        __qt_QList_replace(io.qt.internal.QtJambiInternal.checkedNativeId(this), i, t, replaceFunction);
    }
    
    @QtUninvokable
    public final void reserve(int size)    {
        if(reserveFunction==0)
        	throw new UnsupportedOperationException();
        __qt_QList_reserve(io.qt.internal.QtJambiInternal.checkedNativeId(this), size, reserveFunction);
    }
    
    @QtUninvokable
    public final int size()    {
        if(sizeFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QList_size(io.qt.internal.QtJambiInternal.checkedNativeId(this), sizeFunction);
    }
    
    @QtUninvokable
    public final boolean startsWith(T t)    {
        if(startsWithFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QList_startsWith(io.qt.internal.QtJambiInternal.checkedNativeId(this), t, startsWithFunction);
    }
    
    @QtUninvokable
    public final void swap(int i, int j)    {
        if(swapFunction==0)
        	throw new UnsupportedOperationException();
        __qt_QList_swap(io.qt.internal.QtJambiInternal.checkedNativeId(this), i, j, swapFunction);
    }
    
    @QtUninvokable
    public final T takeAt(int i)    {
        if(takeAtFunction==0)
        	throw new UnsupportedOperationException();
        if (i >= size() || i < 0) {
        	throw new IndexOutOfBoundsException(String.format("Accessing container of size %1$s at %2$s", size(), i));
        }
        return __qt_QList_takeAt(io.qt.internal.QtJambiInternal.checkedNativeId(this), i, takeAtFunction);
    }
    
    @QtUninvokable
    public final T takeFirst()    {
    	return takeAt(0);
    }
    
    @QtUninvokable
    public final T takeLast()    {
    	return takeAt(size()-1);
    }

    @QtUninvokable
    public final java.util.Set<T> toSet()    {
        if(toSetFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QList_toSet(io.qt.internal.QtJambiInternal.checkedNativeId(this), toSetFunction);
    }
    
    @QtUninvokable
    public final T value(int i)    {
        if(valueFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QList_value(io.qt.internal.QtJambiInternal.checkedNativeId(this), i, valueFunction);
    }
    
    @QtUninvokable
    public final T value(int i, T defaultValue)    {
        if(valueDefaultFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QList_valueDefault(io.qt.internal.QtJambiInternal.checkedNativeId(this), i, defaultValue, valueDefaultFunction);
    }
    
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
    public T peek() {
        return head();
    }

    @Override
    @QtUninvokable
    public T poll() {
        return dequeue();
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
        append(e);
        return true;
    }

    @Override
    @QtUninvokable
    public void add(int index, T e){
            insert(index, e);
    }

    @Override
    @QtUninvokable
    public T set(int index, T e){
        if(e!=null){
            T el = get(index);
            replace(index, e);
            return el;
        } else return null;
    }
    
    @Override
    @QtUninvokable
    public boolean remove(Object e){
        return checkElement(e) ? removeOne(castElement(e)) : false;
    }


    @Override
    @QtUninvokable
    public T get(int index) {
        return at(index);
    }

}
