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
package io.qt.internal;

import java.util.ListIterator;
import java.util.NoSuchElementException;
import java.util.function.Function;
import java.util.function.Supplier;

import io.qt.QtObject;
import io.qt.QtUninvokable;

public abstract class QtJambiIteratorObject<E> extends QtObject{
	
	private final Object owner;
	private final Function<Object,QtJambiIteratorObject<?>> endSupplier;

	@SuppressWarnings({ "unchecked", "rawtypes" })
	protected QtJambiIteratorObject(Object owner) {
		super((QPrivateConstructor)null);
		this.owner = owner;
		if(owner instanceof QtJambiCollectionObject) {
			endSupplier = (Function)(Function<QtJambiCollectionObject,QtJambiIteratorObject<?>>)QtJambiCollectionObject::end;
		}else if(owner instanceof QtJambiAbstractMapObject) {
			endSupplier = (Function)(Function<QtJambiAbstractMapObject,QtJambiIteratorObject<?>>)QtJambiAbstractMapObject::end;
		}else if(owner instanceof QtJambiAbstractMultiMapObject) {
			endSupplier = (Function)(Function<QtJambiAbstractMultiMapObject,QtJambiIteratorObject<?>>)QtJambiAbstractMultiMapObject::end;
		}else {
			endSupplier = QtJambiInternal.findEndSupplier(owner);
		}
	}
	
    @QtUninvokable
	protected final QtJambiIteratorObject<?> end(){
		return endSupplier==null ? null : endSupplier.apply(owner);
	}
	
    @QtUninvokable
	protected final boolean compareOwners(QtJambiIteratorObject<?> iter) {
		return owner==iter.owner;
	}

    @QtUninvokable
	protected final java.util.Iterator<E> toJavaIterator(){
    	return new java.util.Iterator<E>() {
    		private final QtJambiIteratorObject<?> end = end();
    		
            @Override
            public boolean hasNext() {
                return end!=null && !QtJambiIteratorObject.this.equals(end);
            }

            @Override
            public E next() {
                if(!hasNext())
                    throw new java.util.NoSuchElementException();
            	if(!end.equals(end()))
            		throw new IllegalMonitorStateException();
                E e = checkedValue();
                increment();
                return e;
            }
        };
    }

    @QtUninvokable
    protected final java.util.Iterator<E> toJavaDescendingIterator(Supplier<? extends QtJambiIteratorObject<E>> beginSupplier){
    	return new java.util.Iterator<E>() {
    		private final QtJambiIteratorObject<?> begin = beginSupplier.get();
            @Override
            public boolean hasNext() {
                return !QtJambiIteratorObject.this.equals(begin);
            }

            @Override
            public E next() {
                if(!hasNext())
                    throw new java.util.NoSuchElementException();
            	if(!begin.equals(beginSupplier.get()))
            		throw new IllegalMonitorStateException();
                decrement();
                E e = checkedValue();
                return e;
            }
        };
    }
    
    @QtUninvokable
    final ListIterator<E> toJavaListIterator(Supplier<? extends QtJambiIteratorObject<E>> beginSupplier, int index) {
	    return new ListIterator<E>() {
	    	private final QtJambiIteratorObject<?> end = end();
	    	private int icursor;
			{
				for (int i = 0; i < index && end!=null && !QtJambiIteratorObject.this.equals(end); i++) {
					increment();
					icursor++;
				}
			}
	        
	        @Override
	        public boolean hasNext() {
	        	return end!=null && !QtJambiIteratorObject.this.equals(end);
	        }
	
	        @Override
	        public E next() {
	        	if(!hasNext())
                    throw new NoSuchElementException();
            	if(!end.equals(end()))
            		throw new IllegalMonitorStateException();
            	E e = checkedValue();
            	increment();
            	icursor++;
                return e;
	        }
	        
	        @Override
	        public E previous() {
	        	if(!hasPrevious())
                    throw new NoSuchElementException();
            	if(!end.equals(end()))
            		throw new IllegalMonitorStateException();
            	E e = checkedValue();
            	decrement();
            	icursor--;
                return e;
	        }
	        
	        @Override
	        public void remove() {
	        	throw new UnsupportedOperationException();
	        }
	        
	        @Override
	        public void set(E e) {
	        	throw new UnsupportedOperationException();
	        }
	        
	        @Override
	        public int previousIndex() {
	        	return icursor-1;
	        }
	        
	        @Override
	        public int nextIndex() {
	        	return icursor+1;
	        }
	        
	        @Override
	        public boolean hasPrevious() {
	        	return !QtJambiIteratorObject.this.equals(beginSupplier.get());
	        }
	        
	        @Override
	        public void add(E e) {
	        	throw new UnsupportedOperationException();
	        }
	    };
	}
    
    @QtUninvokable
    protected abstract void decrement();
    
    @QtUninvokable
    protected abstract void increment();
    
    @QtUninvokable
    protected abstract E checkedValue();
}
