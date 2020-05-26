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

import static io.qt.core.QVector.*;

import io.qt.*;


public class QStack<T> extends io.qt.internal.QtJambiStackObject<T>
{

	static {
    	io.qt.core.QtJambi_LibraryInitializer.init();
    }
	
    private final long appendFunction;
    private final long appendListFunction;
    private final long atFunction;
    private final long beginFunction;
    private final long capacityFunction;
    private final long clearFunction;
    private final long containsFunction;
    private final long countObjectFunction;
    private final long endFunction;
    private final long endsWithFunction;
    private final long fillFunction;
    private final long indexOfFunction;
    private final long insertFunction;
    private final long insertNFunction;
    private final long lastIndexOfFunction;
    private final long midFunction;
    private final long moveFunction;
    private final long equalFunction;
    private final long prependFunction;
    private final long removeNFunction;
    private final long removeAllFunction;
    private final long removeAtFunction;
    private final long removeOneFunction;
    private final long replaceFunction;
    private final long reserveFunction;
    private final long resizeFunction;
    private final long shrinkRoFitFunction;
    private final long sizeFunction;
    private final long startsWithFunction;
    private final long takeAtFunction;
    private final long toListFunction;
    private final long valueFunction;
    private final long valueDefaultFunction;

    private QStack(Class<T> elementType, long appendFunction, long appendListFunction, long atFunction,
			long beginFunction, long capacityFunction, long clearFunction, long containsFunction, long countObjectFunction,
			long endFunction, long endsWithFunction, long fillFunction, long indexOfFunction, long insertFunction, 
			long insertNFunction, long lastIndexOfFunction, long midFunction, long moveFunction, 
			long equalFunction, long prependFunction, long removeNFunction, long removeAllFunction, long removeAtFunction,
			long removeOneFunction, long replaceFunction, long reserveFunction, long resizeFunction,
			long setShrinkRoFitFunction, long sizeFunction, long startsWithFunction, long takeAtFunction, long toListFunction,
			long valueFunction, long valueDefaultFunction){
        super((QPrivateConstructor)null, elementType);
		this.appendFunction = appendFunction;
		this.appendListFunction = appendListFunction;
		this.atFunction = atFunction;
		this.beginFunction = beginFunction;
		this.capacityFunction = capacityFunction;
		this.clearFunction = clearFunction;
		this.containsFunction = containsFunction;
		this.countObjectFunction = countObjectFunction;
		this.endFunction = endFunction;
		this.endsWithFunction = endsWithFunction;
		this.indexOfFunction = indexOfFunction;
		this.fillFunction = fillFunction;
		this.insertFunction = insertFunction;
		this.insertNFunction = insertNFunction;
		this.lastIndexOfFunction = lastIndexOfFunction;
		this.midFunction = midFunction;
		this.moveFunction = moveFunction;
		this.equalFunction = equalFunction;
		this.prependFunction = prependFunction;
		this.removeNFunction = removeNFunction;
		this.removeAllFunction = removeAllFunction;
		this.removeAtFunction = removeAtFunction;
		this.removeOneFunction = removeOneFunction;
		this.replaceFunction = replaceFunction;
		this.reserveFunction = reserveFunction;
		this.resizeFunction = resizeFunction;
		this.shrinkRoFitFunction = setShrinkRoFitFunction;
		this.sizeFunction = sizeFunction;
		this.startsWithFunction = startsWithFunction;
		this.takeAtFunction = takeAtFunction;
		this.toListFunction = toListFunction;
		this.valueFunction = valueFunction;
		this.valueDefaultFunction = valueDefaultFunction;
    }
    
    private QStack(Class<T> elementType, long atFunction,
			long beginFunction, long capacityFunction, long containsFunction, long countObjectFunction,
			long endFunction, long endsWithFunction, long indexOfFunction, 
			long lastIndexOfFunction, long midFunction, long equalFunction, long sizeFunction, 
			long startsWithFunction, long toListFunction, long valueFunction, long valueDefaultFunction){
    	this(elementType, 0, 0, atFunction, beginFunction, capacityFunction, 
    			0, containsFunction, countObjectFunction, endFunction, endsWithFunction, 
    			0, indexOfFunction, 0, 0, lastIndexOfFunction, 
    			midFunction, 0, equalFunction, 0, 0, 0, 0, 0, 0, 0, 0, 0, sizeFunction, 
    			startsWithFunction, 0, toListFunction, valueFunction, valueDefaultFunction);
    }

    @QtUninvokable
    public final void append(java.util.Collection<T> t)    {
        if(appendListFunction==0)
        	throw new UnsupportedOperationException();
        __qt_QVector_appendVector(io.qt.internal.QtJambiInternal.checkedNativeId(this), t, appendListFunction);
    }

    @QtUninvokable
    public final void append(T t)    {
        if(appendFunction==0)
        	throw new UnsupportedOperationException();
        __qt_QVector_append(io.qt.internal.QtJambiInternal.checkedNativeId(this), t, appendFunction);
    }

    @QtUninvokable
    public final T at(int i)    {
        if(atFunction==0)
        	throw new UnsupportedOperationException();
        if (i >= size() || i < 0) {
        	throw new IndexOutOfBoundsException(String.format("Accessing container of size %1$s at %2$s", size(), i));
        }
        return __qt_QVector_at(io.qt.internal.QtJambiInternal.checkedNativeId(this), i, atFunction);
    }

    @QtUninvokable
    protected final QIterator<T> begin()    {
        if(beginFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QVector_begin(io.qt.internal.QtJambiInternal.checkedNativeId(this), beginFunction);
    }

    @QtUninvokable
    public final int capacity()    {
        if(capacityFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QVector_capacity(io.qt.internal.QtJambiInternal.checkedNativeId(this), capacityFunction);
    }

    @QtUninvokable
    public final void clear()    {
        if(clearFunction==0)
        	throw new UnsupportedOperationException();
        __qt_QVector_clear(io.qt.internal.QtJambiInternal.checkedNativeId(this), clearFunction);
    }

    @QtUninvokable
    public final boolean contains(Object t)    {
        if(containsFunction==0)
        	throw new UnsupportedOperationException();
        return checkElement(t) && __qt_QVector_contains(io.qt.internal.QtJambiInternal.checkedNativeId(this), castElement(t), containsFunction);
    }

    @QtUninvokable
    public final int count()    {
        return size();
    }

    @QtUninvokable
    public final int count(T t)    {
        if(countObjectFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QVector_count(io.qt.internal.QtJambiInternal.checkedNativeId(this), t, countObjectFunction);
    }

    @QtUninvokable
    public final boolean empty()    {
        return size()==0;
    }

    @QtUninvokable
    protected final QIterator<T> end()    {
        if(endFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QVector_end(io.qt.internal.QtJambiInternal.checkedNativeId(this), endFunction);
    }

    @QtUninvokable
    public final boolean endsWith(T t)    {
        if(endsWithFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QVector_endsWith(io.qt.internal.QtJambiInternal.checkedNativeId(this), t, endsWithFunction);
    }

    @QtUninvokable
    public final void fill(T t) {
        fill(t, (int)-1);
    }
    @QtUninvokable
    public final void fill(T t, int size)    {
        if(fillFunction==0)
        	throw new UnsupportedOperationException();
        __qt_QVector_fill(io.qt.internal.QtJambiInternal.checkedNativeId(this), t, size, fillFunction);
    }

    @QtUninvokable
    public final T first()    {
    	return at(0);
    }

    @QtUninvokable
    public final T front()    {
        return first();
    }

    @QtUninvokable
    public final int indexOf(T t, int from)    {
        if(indexOfFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QVector_indexOf(io.qt.internal.QtJambiInternal.checkedNativeId(this), t, from, indexOfFunction);
    }

    @QtUninvokable
    public final void insert(int i, T t)    {
        if(insertFunction==0)
        	throw new UnsupportedOperationException();
        __qt_QVector_insert(io.qt.internal.QtJambiInternal.checkedNativeId(this), i, t, insertFunction);
    }

    @QtUninvokable
    public final void insert(int i, int n, T t)    {
        if(insertNFunction==0)
        	throw new UnsupportedOperationException();
        __qt_QVector_insertN(io.qt.internal.QtJambiInternal.checkedNativeId(this), i, n, t, insertNFunction);
    }

    @QtUninvokable
    public final boolean isEmpty()    {
        return size()==0;
    }

    @QtUninvokable
    public final T last()    {
    	return at(size()-1);
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
        return __qt_QVector_lastIndexOf(io.qt.internal.QtJambiInternal.checkedNativeId(this), t, from, lastIndexOfFunction);
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
        return __qt_QVector_mid(io.qt.internal.QtJambiInternal.checkedNativeId(this), pos, length, midFunction);
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
        __qt_QVector_move(io.qt.internal.QtJambiInternal.checkedNativeId(this), from, to, moveFunction);
    }

    @QtUninvokable
    private final boolean operator_equal(java.util.Collection<T> l)    {
        if(equalFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QVector_operator_equal(io.qt.internal.QtJambiInternal.checkedNativeId(this), l, equalFunction);
    }

    @QtUninvokable
    public final void pop_back()    {
    	removeLast();
    }

    @QtUninvokable
    public final void pop_front()    {
    	removeFirst();
    }

    @QtUninvokable
    public final void prepend(T t)    {
        if(prependFunction==0)
        	throw new UnsupportedOperationException();
        __qt_QVector_prepend(io.qt.internal.QtJambiInternal.checkedNativeId(this), t, prependFunction);
    }

    @QtUninvokable
    public final void push_back(T t)    {
    	append(t);
    }

    @QtUninvokable
    public final void push_front(T t)    {
    	prepend(t);
    }

    @QtUninvokable
    public final void remove(int i, int n)    {
        if(removeNFunction==0)
        	throw new UnsupportedOperationException();
        __qt_QVector_removeN(io.qt.internal.QtJambiInternal.checkedNativeId(this), i, n, removeNFunction);
    }

    @QtUninvokable
    public final int removeAll(T t)    {
        if(removeAllFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QVector_removeAll(io.qt.internal.QtJambiInternal.checkedNativeId(this), t, removeAllFunction);
    }

    @QtUninvokable
    public final void removeAt(int i)    {
        if(removeAtFunction==0)
        	throw new UnsupportedOperationException();
        if (i >= size() || i < 0) {
        	throw new IndexOutOfBoundsException(String.format("Accessing container of size %1$s at %2$s", size(), i));
        }
        __qt_QVector_removeAt(io.qt.internal.QtJambiInternal.checkedNativeId(this), i, removeAtFunction);
    }

    @QtUninvokable
    public final T removeFirst()    {
    	T result = first();
    	removeAt(0);
    	return result;
    }

    @QtUninvokable
    public final T removeLast()    {
    	T result = last();
    	removeAt(size()-1);
    	return result;
    }

    @QtUninvokable
    public final boolean removeOne(T t)    {
        if(removeOneFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QVector_removeOne(io.qt.internal.QtJambiInternal.checkedNativeId(this), t, removeOneFunction);
    }

    @QtUninvokable
    public final void replace(int i, T t)    {
        if (i >= size() || i < 0) {
        	throw new IndexOutOfBoundsException(String.format("Accessing container of size %1$s at %2$s", size(), i));
        }
        if(replaceFunction==0)
        	throw new UnsupportedOperationException();
        __qt_QVector_replace(io.qt.internal.QtJambiInternal.checkedNativeId(this), i, t, replaceFunction);
    }

    @QtUninvokable
    public final void reserve(int size)    {
        if(reserveFunction==0)
        	throw new UnsupportedOperationException();
        __qt_QVector_reserve(io.qt.internal.QtJambiInternal.checkedNativeId(this), size, reserveFunction);
    }

    @QtUninvokable
    public final void resize(int size)    {
        if(resizeFunction==0)
        	throw new UnsupportedOperationException();
        __qt_QVector_resize(io.qt.internal.QtJambiInternal.checkedNativeId(this), size, resizeFunction);
    }

    @QtUninvokable
    public final void shrink_to_fit()    {
        if(shrinkRoFitFunction==0)
        	throw new UnsupportedOperationException();
        __qt_QVector_shrink_to_fit(io.qt.internal.QtJambiInternal.checkedNativeId(this), shrinkRoFitFunction);
    }

    @QtUninvokable
    public final int size()    {
        if(sizeFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QVector_size(io.qt.internal.QtJambiInternal.checkedNativeId(this), sizeFunction);
    }

    @QtUninvokable
    public final boolean startsWith(T t)    {
        if(startsWithFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QVector_startsWith(io.qt.internal.QtJambiInternal.checkedNativeId(this), t, startsWithFunction);
    }

    @QtUninvokable
    public final T takeAt(int i)    {
        if(takeAtFunction==0)
        	throw new UnsupportedOperationException();
        if (i >= size() || i < 0) {
        	throw new IndexOutOfBoundsException(String.format("Accessing container of size %1$s at %2$s", size(), i));
        }
        return __qt_QVector_takeAt(io.qt.internal.QtJambiInternal.checkedNativeId(this), i, takeAtFunction);
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
    public final java.util.List<T> toList()    {
        if(toListFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QVector_toList(io.qt.internal.QtJambiInternal.checkedNativeId(this), toListFunction);
    }

    @QtUninvokable
    public final T value(int i)    {
        if(valueFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QVector_value(io.qt.internal.QtJambiInternal.checkedNativeId(this), i, valueFunction);
    }

    @QtUninvokable
    public final T value(int i, T defaultValue)    {
        if(valueDefaultFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QVector_valueDefault(io.qt.internal.QtJambiInternal.checkedNativeId(this), i, defaultValue, valueDefaultFunction);
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
    public int indexOf(Object e){
        return checkElement(e) ? indexOf(castElement(e), 0) : -1;
    }

    @Override
    @QtUninvokable
    public T get(int index) {
        return at(index);
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
		add(e);
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
