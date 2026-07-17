/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
import java.util.List;
import java.util.NoSuchElementException;

import io.qt.NonNull;
import io.qt.QtUninvokable;

abstract class AbstractList<T> extends AbstractSequentialContainer<T> implements java.util.List<T> {
	
    AbstractList(QPrivateConstructor p) {
		super(p);
	}
    
    @Override
    public abstract AbstractList<T> clone();
    
    @QtUninvokable
	protected abstract QSequentialIterator<T,? extends AbstractList<T>> begin();

    @QtUninvokable
    protected abstract QSequentialIterator<T,? extends AbstractList<T>> end();
    
    public abstract void removeAt(int i);
    
    abstract T takeAt(int i);
    
    public abstract void append(java.util.@NonNull Collection<? extends T> t);
    
    /**
     * Appends all of the elements in the specified collection to the end of
     * this list.
     */
    @Override
    @QtUninvokable
    public final boolean addAll(@NonNull Collection<? extends T> c) {
        append(c);
        return true;
    }

    /**
     * Inserts all of the elements in the specified collection into this
     * list at the specified position.
     */
    @Override
    @QtUninvokable
    public final boolean addAll(int index, @NonNull Collection<? extends T> c) {
        for (T o : c) {
            add(index++, o);
        }
        return true;
    }

    /**
     * Removes the element at the specified position in this list.
     */
    @Override
    @QtUninvokable
    public final T remove(int index) {
        return takeAt(index);
    }

    /**
     * Returns a list iterator over the elements in this list (in proper
     * sequence).
     */
	@Override
    @QtUninvokable
	public final java.util.ListIterator<T> listIterator() {
	    return listIterator(0);
	}
	
	private class Iterator implements java.util.Iterator<T>{
    	int icursor;
    	
    	Iterator(int index){
    		icursor = index;
    	}
    	
        @Override
        public final boolean hasNext() {
    		int size = size();
        	return icursor >= 0 && icursor<size;
        }

        @Override
        public final T next() {
        	try {
	        	T e = get(icursor);
	        	++icursor;
	            return e;
        	} catch (IndexOutOfBoundsException e) {
                throw new NoSuchElementException();
            }
        }
        
        @Override
        public final void remove() {
        	if(icursor==0)
        		throw new IndexOutOfBoundsException(-1);
        	AbstractList.this.remove(--icursor);
        }
	}
	
	private final class ListIterator extends Iterator implements java.util.ListIterator<T>{
    	ListIterator(int index){
    		super(index);
    	}
    	
        @Override
        public T previous() {
        	try {
	        	--icursor;
	        	T e = get(icursor);
	            return e;
	    	} catch (IndexOutOfBoundsException e) {
	            throw new NoSuchElementException();
	        }
        }
        
		@Override
        public void set(T e) {
        	if(icursor==0)
        		throw new IndexOutOfBoundsException(-1);
        	AbstractList.this.set(icursor-1, e);
        }
        
        @Override
        public int previousIndex() {
        	return icursor-1;
        }
        
        @Override
        public int nextIndex() {
        	return icursor;
        }
        
        @Override
        public boolean hasPrevious() {
    		int size = size();
        	return icursor>0 && size>0;
        }
        
        @Override
        public void add(T e) {
        	AbstractList.this.add(icursor, e);
        	++icursor;
        }
	}
	
    /**
     * Returns a list iterator over the elements in this list (in proper
     * sequence), starting at the specified position in the list.
     */
	@Override
    @QtUninvokable
	public final java.util.ListIterator<T> listIterator(int index) {
		return new ListIterator(index);
	}
	
    /**
     * Returns a view of the portion of this list between the specified
     * {@code fromIndex}, inclusive, and {@code toIndex}, exclusive.  (If
     * {@code fromIndex} and {@code toIndex} are equal, the returned list is
     * empty.)  The returned list is backed by this list, so non-structural
     * changes in the returned list are reflected in this list, and vice-versa.
     * The returned list supports all of the optional list operations supported
     * by this list.
     */
	@Override
    @QtUninvokable
	public final List<T> subList(int fromIndex, int toIndex) {
	    return mid(fromIndex, toIndex-fromIndex);
	}
	
	abstract AbstractList<T> mid(int pos, int length);
	
	/**
     * Retains only the elements in this list that are contained in the
     * specified collection (optional operation).  In other words, removes
     * from this list all of its elements that are not contained in the
     * specified collection.
     */
	@Override
    @QtUninvokable
	public final boolean retainAll(@NonNull Collection<?> c) {
		return removeIf(c::contains);
	}
}
