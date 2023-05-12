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
package io.qt.internal;

import java.util.ArrayList;
import java.util.Collection;
import java.util.List;
import java.util.NoSuchElementException;

import io.qt.QtUninvokable;

public abstract class AbstractList<E> extends AbstractSequentialContainer<E> implements List<E> {
	
    protected AbstractList(QPrivateConstructor p) {
		super(p);
	}
    
    @Override
    public abstract AbstractList<E> clone();
    
    @QtUninvokable
	protected abstract AbstractSequentialConstIterator<E> begin();

    @QtUninvokable
    protected abstract AbstractSequentialConstIterator<E> end();

    @QtUninvokable
    protected abstract void removeAtIndex(int cursor);

    @QtUninvokable
    public abstract void append(Collection<E> c);

    @Override
    @SuppressWarnings("unchecked")
    @QtUninvokable
    public final boolean addAll(Collection<? extends E> c) {
        append((Collection<E>)c);
        return true;
    }

    @Override
    @QtUninvokable
    public final boolean addAll(int index, Collection<? extends E> c) {
        for (E o : c) {
            add(index++, o);
        }
        return true;
    }

    @Override
    @QtUninvokable
    public final E remove(int index) {
        E result = get(index);
        this.removeAtIndex(index);
        return result;
    }

	@Override
    @QtUninvokable
	public final java.util.ListIterator<E> listIterator() {
	    return listIterator(0);
	}
	
	private static class ListIterator<E> implements java.util.ListIterator<E>{
		private final AbstractList<E> list;
		private AbstractSequentialConstIterator<E> current;
    	private AbstractSequentialConstIterator<E> begin;
    	private AbstractSequentialConstIterator<E> end;
    	private int icursor;
    	private boolean hasNext;
    	private boolean hasPrevious;
    	
    	ListIterator(AbstractList<E> list, int index){
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
        public E next() {
        	if(!hasNext())
                throw new NoSuchElementException();
        	checkModification();
        	E e = current.val();
        	current.increment();
        	hasNext = end!=null && !current.equals(end);
        	hasPrevious = begin!=null && !current.equals(begin);
        	icursor++;
            return e;
        }
        
        @Override
        public E previous() {
        	if(!hasPrevious())
                throw new NoSuchElementException();
        	checkModification();
        	current.decrement();
        	hasNext = end!=null && !current.equals(end);
        	hasPrevious = begin!=null && !current.equals(begin);
        	E e = current.val();
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
        
        @SuppressWarnings("unchecked")
		@Override
        public void set(E e) {
        	checkModification();
        	if(icursor==0)
        		throw new IndexOutOfBoundsException(-1);
        	current.decrement();
        	((AbstractSequentialIterator<E>)current).setValue(e);
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
        public void add(E e) {
        	checkModification();
        	list.add(icursor, e);
        	initialize(icursor+1);
        }
        
		void checkModification() {
//        	if(!end.equals(citer.end()))
//    		throw new IllegalMonitorStateException();
		}
	}
	
	@Override
    @QtUninvokable
	public final java.util.ListIterator<E> listIterator(int index) {
		return new ListIterator<>(this, index);
	}
	
	@SuppressWarnings("unchecked")
	@Override
    @QtUninvokable
	public final List<E> subList(int fromIndex, int toIndex) {
	    if (fromIndex < 0)
	        throw new IndexOutOfBoundsException("fromIndex = " + fromIndex);
	    if (toIndex > size())
	        throw new IndexOutOfBoundsException("toIndex = " + toIndex);
	    if (fromIndex > toIndex)
	        throw new IllegalArgumentException("fromIndex(" + fromIndex +
	                                       ") > toIndex(" + toIndex + ")");
	    List<E> sublist;
	    try {
	    	sublist = this.getClass().getConstructor().newInstance();
	    }catch(Throwable e) {
	    	sublist = new ArrayList<>();
	    }
	    for (int i = 0; i < toIndex - fromIndex; i++) {
	        sublist.add(get(fromIndex+i));
	    }
	    return sublist;
	}
	
	@Override
    @QtUninvokable
	public final boolean retainAll(Collection<?> c) {
		boolean changed = false;
		for(int i=0; i<size();) {
			if(c.contains(get(i))) {
				++i;
			}else {
				changed = true;
				remove(i);
			}
		}
        return changed;
	}
}
