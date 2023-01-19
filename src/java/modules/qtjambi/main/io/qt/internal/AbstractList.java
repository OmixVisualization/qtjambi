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
import java.util.ListIterator;
import java.util.NoSuchElementException;

import io.qt.QtUninvokable;

public abstract class AbstractList<E> extends AbstractSequentialContainer<E> implements List<E> {
	
    protected AbstractList() {
		super();
	}

    protected AbstractList(QPrivateConstructor p) {
		super(p);
	}
    
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
	public final ListIterator<E> listIterator() {
	    return listIterator(0);
	}
	
	private class QtJambiListIterator implements ListIterator<E>{
		private AbstractSequentialConstIterator<E> current;
    	private AbstractSequentialConstIterator<E> end;
    	private int icursor;
    	
    	QtJambiListIterator(int index){
    		initialize(index);
    	}
    	
    	private void initialize(int index){
    		current = begin();
    		end = end();
    		icursor = 0;
			for (int i = 0; i < index && end!=null && !current.equals(end); i++) {
				current.increment();
				icursor++;
			}
		}
        
        @Override
        public boolean hasNext() {
        	return end!=null && !current.equals(end);
        }

        @Override
        public E next() {
        	if(!hasNext())
                throw new NoSuchElementException();
        	if(!end.equals(end()))
        		throw new IllegalMonitorStateException();
        	E e = current.checkedValue();
        	current.increment();
        	icursor++;
            return e;
        }
        
        @Override
        public E previous() {
        	if(!hasPrevious())
                throw new NoSuchElementException();
        	if(!end.equals(end()))
        		throw new IllegalMonitorStateException();
        	current.decrement();
        	E e = current.checkedValue();
        	icursor--;
            return e;
        }
        
        @Override
        public void remove() {
        	if(!end.equals(end()))
        		throw new IllegalMonitorStateException();
        	if(icursor==0)
        		throw new IndexOutOfBoundsException(-1);
        	AbstractList.this.remove(icursor-1);
        	initialize(icursor-1);
        }
        
        @SuppressWarnings("unchecked")
		@Override
        public void set(E e) {
        	if(!end.equals(end()))
        		throw new IllegalMonitorStateException();
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
        	return !current.equals(begin());
        }
        
        @Override
        public void add(E e) {
        	if(!end.equals(end()))
        		throw new IllegalMonitorStateException();
        	AbstractList.this.add(icursor, e);
        	initialize(icursor+1);
        }
	}
	
	@Override
    @QtUninvokable
	public final ListIterator<E> listIterator(int index) {
		return new QtJambiListIterator(index);
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
