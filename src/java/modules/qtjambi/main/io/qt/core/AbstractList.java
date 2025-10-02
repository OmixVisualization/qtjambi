/****************************************************************************
**
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import io.qt.QtUninvokable;

abstract class AbstractList<T> extends AbstractSequentialContainer<T> implements java.util.List<T> {
	
    AbstractList(QPrivateConstructor p) {
		super(p);
	}
    
    @QtUninvokable
	protected abstract QSequentialIterator<T> begin();

    @QtUninvokable
    protected abstract QSequentialIterator<T> end();
    
    public abstract void removeAt(int i);
    
    abstract T takeAt(int i);
    
    public abstract void append(java.util.Collection<T> t);
    
    /**
     * Appends all of the elements in the specified collection to the end of
     * this list.
     */
    @Override
    @SuppressWarnings("unchecked")
    @QtUninvokable
    public final boolean addAll(Collection<? extends T> c) {
        append((Collection<T>)c);
        return true;
    }

    /**
     * Inserts all of the elements in the specified collection into this
     * list at the specified position.
     */
    @Override
    @QtUninvokable
    public final boolean addAll(int index, Collection<? extends T> c) {
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
	
	private static class ListIterator<T> implements java.util.ListIterator<T>{
		private final AbstractList<T> list;
		private QSequentialConstIterator<T> current;
    	private QSequentialConstIterator<T> begin;
    	private QSequentialConstIterator<T> end;
    	private int icursor;
    	private boolean hasNext;
    	private boolean hasPrevious;
    	
    	ListIterator(AbstractList<T> list, int index){
    		this.list = list;
    		initialize(index);
    	}
    	
    	private void initialize(int index){
    		begin = list.begin();
    		current = list.begin();
    		end = list.end();
    		icursor = 0;
			for (int i = 0; i < index && end!=null && !current.equals(end); i++) {
				current.increment();
				icursor++;
			}
			hasNext = end!=null && !current.equals(end);
			hasPrevious = begin!=null && !current.equals(begin);
		}
        
        @Override
        public boolean hasNext() {
        	return hasNext;
        }

        @Override
        public T next() {
        	if(!hasNext())
                throw new NoSuchElementException();
        	checkModification();
        	T e = current._value();
        	current.increment();
        	hasNext = end!=null && !current.equals(end);
        	hasPrevious = begin!=null && !current.equals(begin);
        	icursor++;
            return e;
        }
        
        @Override
        public T previous() {
        	if(!hasPrevious())
                throw new NoSuchElementException();
        	checkModification();
        	current.decrement();
        	hasNext = end!=null && !current.equals(end);
        	hasPrevious = begin!=null && !current.equals(begin);
        	T e = current._value();
        	icursor--;
            return e;
        }
        
        @Override
        public void remove() {
        	checkModification();
        	if(icursor==0)
        		throw new IndexOutOfBoundsException(-1);
        	list.remove(icursor-1);
        	initialize(icursor-1);
        }
        
		@Override
        public void set(T e) {
        	checkModification();
        	if(icursor==0)
        		throw new IndexOutOfBoundsException(-1);
        	current.decrement();
        	((QSequentialIterator<T>)current).setValue(e);
        	current.increment();
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
        	return hasPrevious;
        }
        
        @Override
        public void add(T e) {
        	checkModification();
        	list.add(icursor, e);
        	initialize(icursor+1);
        }
        
		void checkModification() {
//        	if(!end.equals(citer.end()))
//    		throw new IllegalMonitorStateException();
		}
	}
	
    /**
     * Returns a list iterator over the elements in this list (in proper
     * sequence), starting at the specified position in the list.
     */
	@Override
    @QtUninvokable
	public final java.util.ListIterator<T> listIterator(int index) {
		return new ListIterator<>(this, index);
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
	public final boolean retainAll(Collection<?> c) {
		return removeIf(c::contains);
	}
}
