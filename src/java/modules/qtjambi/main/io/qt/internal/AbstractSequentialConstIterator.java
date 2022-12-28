/****************************************************************************
**
** Copyright (C) 2009-2022 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import java.lang.reflect.Method;
import java.util.Collections;
import java.util.HashMap;
import java.util.Map;
import java.util.NoSuchElementException;
import java.util.function.Function;
import java.util.function.Supplier;

import io.qt.QtObject;
import io.qt.QtUninvokable;

public abstract class AbstractSequentialConstIterator<E> extends QtObject{
	
	private final Object owner;
	private final Function<Object,AbstractSequentialConstIterator<?>> beginSupplier;
	private final Function<Object,AbstractSequentialConstIterator<?>> endSupplier;

	@SuppressWarnings({ "unchecked", "rawtypes" })
	protected AbstractSequentialConstIterator(Object owner) {
		super((QPrivateConstructor)null);
		boolean isConst = !(this instanceof AbstractSequentialIterator
							|| this instanceof AbstractAssociativeIterator);
		this.owner = owner;
		if(owner instanceof AbstractSequentialContainer) {
			if(isConst) {
				endSupplier = (Function)(Function<AbstractSequentialContainer,AbstractSequentialConstIterator<?>>)AbstractSequentialContainer::constEnd;
				beginSupplier = (Function)(Function<AbstractSequentialContainer,AbstractSequentialConstIterator<?>>)AbstractSequentialContainer::constBegin;
			}else if(owner instanceof AbstractList) {
				endSupplier = (Function)(Function<AbstractList,AbstractSequentialConstIterator<?>>)AbstractList::end;
				beginSupplier = (Function)(Function<AbstractList,AbstractSequentialConstIterator<?>>)AbstractList::begin;
			}else {
				endSupplier = null;
				beginSupplier = null;
			}
		}else if(owner instanceof AbstractAssociativeContainer) {
			if(isConst) {
				endSupplier = (Function)(Function<AbstractAssociativeContainer,AbstractSequentialConstIterator<?>>)AbstractAssociativeContainer::constEnd;
				beginSupplier = (Function)(Function<AbstractAssociativeContainer,AbstractSequentialConstIterator<?>>)AbstractAssociativeContainer::constBegin;
			}else {
				endSupplier = (Function)(Function<AbstractAssociativeContainer,AbstractSequentialConstIterator<?>>)AbstractAssociativeContainer::end;
				beginSupplier = (Function)(Function<AbstractAssociativeContainer,AbstractSequentialConstIterator<?>>)AbstractAssociativeContainer::begin;
			}
		}else if(owner instanceof AbstractMultiAssociativeContainer) {
			if(isConst) {
				endSupplier = (Function)(Function<AbstractMultiAssociativeContainer,AbstractSequentialConstIterator<?>>)AbstractMultiAssociativeContainer::constEnd;
				beginSupplier = (Function)(Function<AbstractMultiAssociativeContainer,AbstractSequentialConstIterator<?>>)AbstractMultiAssociativeContainer::constBegin;
			}else {
				endSupplier = (Function)(Function<AbstractMultiAssociativeContainer,AbstractSequentialConstIterator<?>>)AbstractMultiAssociativeContainer::end;
				beginSupplier = (Function)(Function<AbstractMultiAssociativeContainer,AbstractSequentialConstIterator<?>>)AbstractMultiAssociativeContainer::begin;
			}
		}else {
			endSupplier = findEndSupplier(owner, isConst);
			beginSupplier = findBeginSupplier(owner, isConst);
		}
	}
	
    @QtUninvokable
	protected final AbstractSequentialConstIterator<?> end(){
		return endSupplier==null ? null : endSupplier.apply(owner);
	}
    
    @QtUninvokable
	protected final AbstractSequentialConstIterator<?> begin(){
		return beginSupplier==null ? null : beginSupplier.apply(owner);
	}
	
    @QtUninvokable
	protected final boolean compareOwners(AbstractSequentialConstIterator<?> iter) {
		return owner==iter.owner;
	}

    @QtUninvokable
	protected final java.util.Iterator<E> toJavaIterator(){
    	return new java.util.Iterator<E>() {
    		private final AbstractSequentialConstIterator<?> end = end();
    		
            @Override
            public boolean hasNext() {
                return end!=null && !AbstractSequentialConstIterator.this.equals(end);
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
    protected final java.util.Iterator<E> toJavaDescendingIterator(Supplier<? extends AbstractSequentialConstIterator<E>> beginSupplier){
    	return new java.util.Iterator<E>() {
    		private final AbstractSequentialConstIterator<?> begin = beginSupplier.get();
            @Override
            public boolean hasNext() {
                return !AbstractSequentialConstIterator.this.equals(begin);
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
    static <T> java.util.ListIterator<T> listIterator(AbstractSequentialIterator<T> iter, int index){
		@SuppressWarnings("unchecked")
    	AbstractSequentialConstIterator<T> citer = (AbstractSequentialConstIterator<T>)iter;
    	return new java.util.ListIterator<T>() {
			private AbstractSequentialConstIterator<T> current = citer;
        	private AbstractSequentialConstIterator<?> end = citer.end();
        	private int icursor;
        	
        	{
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
            public T next() {
            	if(!hasNext())
                    throw new NoSuchElementException();
            	if(!end.equals(citer.end()))
            		throw new IllegalMonitorStateException();
            	T e = current.checkedValue();
            	current.increment();
            	icursor++;
                return e;
            }
            
            @Override
            public T previous() {
            	if(!hasPrevious())
                    throw new NoSuchElementException();
            	if(!end.equals(citer.end()))
            		throw new IllegalMonitorStateException();
            	current.decrement();
            	T e = current.checkedValue();
            	icursor--;
                return e;
            }
            
            @Override
            public void remove() {
            	throw new UnsupportedOperationException();
            }
            
    		@SuppressWarnings("unchecked")
			@Override
            public void set(T e) {
            	if(!end.equals(citer.end()))
            		throw new IllegalMonitorStateException();
            	if(icursor==0)
            		throw new IndexOutOfBoundsException(-1);
            	current.decrement();
            	((AbstractSequentialIterator<T>)current).setValue(e);
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
            	return !current.equals(citer.begin());
            }
            
            @Override
            public void add(T e) {
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
	
	private static class BeginEndFunctions{
		BeginEndFunctions(
				Function<Object, AbstractSequentialConstIterator<?>> constBegin,
				Function<Object, AbstractSequentialConstIterator<?>> constEnd,
				Function<Object, AbstractSequentialConstIterator<?>> begin,
				Function<Object, AbstractSequentialConstIterator<?>> end) {
			super();
			this.begin = begin;
			this.end = end;
			this.constBegin = constBegin;
			this.constEnd = constEnd;
		}
		
		final Function<Object,AbstractSequentialConstIterator<?>> begin;
		final Function<Object,AbstractSequentialConstIterator<?>> end;
		final Function<Object,AbstractSequentialConstIterator<?>> constBegin;
		final Function<Object,AbstractSequentialConstIterator<?>> constEnd;
	}
	private static final Map<Class<?>, BeginEndFunctions> endMethodHandles = Collections.synchronizedMap(new HashMap<>());
	
	private static BeginEndFunctions findBeginEndSuppliers(Object beginOwner) {
		return endMethodHandles.computeIfAbsent(ClassAnalyzerUtility.getClass(beginOwner), cls -> {
			Method beginMethod = null;
			Method constBeginMethod = null;
			Method endMethod = null;
			Method constEndMethod = null;
			while ((endMethod == null && constEndMethod == null
				 && beginMethod == null && constBeginMethod == null) 
					&& cls!=null && cls != NativeUtility.Object.class) {
				Method methods[] = cls.getDeclaredMethods();
				for (Method method : methods) {
					if (method.getParameterCount() == 0
							&& AbstractSequentialConstIterator.class.isAssignableFrom(method.getReturnType())) {
						if(method.getName().equals("begin")) {
							beginMethod = method;
						}else if(method.getName().equals("end")) {
							endMethod = method;
						}else if(method.getName().equals("constEnd")) {
							constEndMethod = method;
						}else if(method.getName().equals("constBegin")) {
							constBeginMethod = method;
						}
					}
					if(endMethod != null && constEndMethod != null
							 && beginMethod != null && constBeginMethod != null) {
						break;
					}
				}
				cls = cls.getSuperclass();
			}
			if(constBeginMethod==null && beginMethod!=null) {
				if(!beginMethod.getReturnType().isAssignableFrom(AbstractSequentialIterator.class)
						&& !beginMethod.getReturnType().isAssignableFrom(AbstractAssociativeIterator.class)) {
					constBeginMethod = beginMethod;
					beginMethod = null;
				}
			}
			if(constEndMethod==null && endMethod!=null) {
				if(!endMethod.getReturnType().isAssignableFrom(AbstractSequentialIterator.class)
						&& !endMethod.getReturnType().isAssignableFrom(AbstractAssociativeIterator.class)) {
					constEndMethod = endMethod;
					endMethod = null;
				}
			}
			if (endMethod != null || constEndMethod != null)
				return new BeginEndFunctions(ReflectionUtility.functionFromMethod(constBeginMethod), 
											 ReflectionUtility.functionFromMethod(constEndMethod), 
											 ReflectionUtility.functionFromMethod(beginMethod), 
											 ReflectionUtility.functionFromMethod(endMethod));
			else return new BeginEndFunctions(null,null,null,null);
		});
	}
	
	private static Function<Object,AbstractSequentialConstIterator<?>> findBeginSupplier(Object beginOwner, boolean isConst){
		BeginEndFunctions functions = findBeginEndSuppliers(beginOwner);
		return isConst ? functions.constBegin : functions.begin;
	}
	
	private static Function<Object,AbstractSequentialConstIterator<?>> findEndSupplier(Object beginOwner, boolean isConst){
		BeginEndFunctions functions = findBeginEndSuppliers(beginOwner);
		return isConst ? functions.constEnd : functions.end;
	}
}
