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
package io.qt.internal;

import java.lang.invoke.MethodHandle;
import java.util.ListIterator;
import java.util.NoSuchElementException;
import java.util.function.Consumer;
import java.util.function.Function;
import java.util.function.Supplier;

import io.qt.QtObject;

public abstract class QtJambiIteratorObject<E> extends QtObject{

    private final Consumer<QtJambiIteratorObject<E>> decrement;
    
    private final Consumer<QtJambiIteratorObject<E>> increment;
    
    private final Function<QtJambiIteratorObject<E>,E> value;
    
	@SuppressWarnings("unchecked")
	protected <T extends QtJambiIteratorObject<?>,S> QtJambiIteratorObject(Consumer<T> decrement, Consumer<T> increment, Function<T,S> value) {
		super((QPrivateConstructor)null);
		this.increment = (Consumer<QtJambiIteratorObject<E>>)increment;
		this.decrement = (Consumer<QtJambiIteratorObject<E>>)decrement;
		this.value = (Function<QtJambiIteratorObject<E>,E>)value;
	}

    public java.util.Iterator<E> toJavaIterator(Supplier<? extends QtJambiIteratorObject<E>> endSupplier){
    	return new java.util.Iterator<>() {
            @Override
            public boolean hasNext() {
                return !QtJambiIteratorObject.this.equals(endSupplier.get());
            }

            @Override
            public E next() {
                if(!hasNext())
                    throw new java.util.NoSuchElementException();
                E e = _value();
                _increment();
                return e;
            }
        };
    }

    public java.util.Iterator<E> toJavaDescendingIterator(Supplier<? extends QtJambiIteratorObject<E>> beginSupplier){
    	return new java.util.Iterator<>() {
            @Override
            public boolean hasNext() {
                return !QtJambiIteratorObject.this.equals(beginSupplier.get());
            }

            @Override
            public E next() {
                if(!hasNext())
                    throw new java.util.NoSuchElementException();
                E e = _value();
                _decrement();
                return e;
            }
        };
    }
    
    public ListIterator<E> toJavaListIterator(Supplier<? extends QtJambiIteratorObject<E>> beginSupplier, Supplier<? extends QtJambiIteratorObject<E>> endSupplier, int index) {
	    return new ListIterator<>() {
	    	private int icursor;
			{
				for (int i = 0; i < index && !QtJambiIteratorObject.this.equals(endSupplier.get()); i++) {
					_increment();
					icursor++;
				}
			}
	        
	        @Override
	        public boolean hasNext() {
	        	return !QtJambiIteratorObject.this.equals(endSupplier.get());
	        }
	
	        @Override
	        public E next() {
	        	if(!hasNext())
                    throw new NoSuchElementException();
            	E e = _value();
            	_increment();
            	icursor++;
                return e;
	        }
	        
	        @Override
	        public E previous() {
	        	if(!hasNext())
                    throw new NoSuchElementException();
            	E e = _value();
            	_decrement();
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
    
    void _decrement() {
    	decrement.accept(QtJambiIteratorObject.this);
    }
    
    void _increment() {
    	increment.accept(QtJambiIteratorObject.this);
    }
    
    E _value() {
    	return value.apply(QtJambiIteratorObject.this);
    }
    
    protected MethodHandle findEndFunction(Object owner) {
		return QtJambiInternal.findEndMethod(owner);
	}
}
