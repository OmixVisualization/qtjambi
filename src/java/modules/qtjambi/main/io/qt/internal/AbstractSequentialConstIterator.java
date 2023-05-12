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

import java.lang.reflect.Method;
import java.util.Collections;
import java.util.ConcurrentModificationException;
import java.util.HashMap;
import java.util.Map;
import java.util.NoSuchElementException;
import java.util.function.Function;

import io.qt.QtObject;
import io.qt.QtUninvokable;

public abstract class AbstractSequentialConstIterator<E> extends QtObject{
	
	private final static Function<AbstractSequentialConstIterator<?>, java.util.Iterator<?>> iteratorFactory;
	private final static Function<AbstractSequentialConstIterator<?>, java.util.Iterator<?>> diteratorFactory;
	
	static {
		if(Boolean.getBoolean("io.qt.enable-concurrent-container-modification-check")) {
			@SuppressWarnings("unchecked")
			Function<AbstractSequentialConstIterator<?>, java.util.Iterator<?>> _iteratorFactory = CheckingIncrementalIterator::new;
			@SuppressWarnings("unchecked")
			Function<AbstractSequentialConstIterator<?>, java.util.Iterator<?>> _diteratorFactory = CheckingDecrementalIterator::new;
			iteratorFactory = _iteratorFactory;
			diteratorFactory = _diteratorFactory;
		}else {
			@SuppressWarnings("unchecked")
			Function<AbstractSequentialConstIterator<?>, java.util.Iterator<?>> _iteratorFactory = IncrementalIterator::new;
			@SuppressWarnings("unchecked")
			Function<AbstractSequentialConstIterator<?>, java.util.Iterator<?>> _diteratorFactory = DecrementalIterator::new;
			iteratorFactory = _iteratorFactory;
			diteratorFactory = _diteratorFactory;
		}
	}
	
	private final QtObject owner;
	private final Function<QtObject,AbstractSequentialConstIterator<E>> beginSupplier;
	private final Function<QtObject,AbstractSequentialConstIterator<E>> endSupplier;

	@SuppressWarnings({ "unchecked", "rawtypes" })
	protected AbstractSequentialConstIterator(QtObject owner) {
		super((QPrivateConstructor)null);
		this.owner = owner;
		if(owner instanceof AbstractSequentialContainer) {
			if(isConstant()) {
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
			if(isConstant()) {
				endSupplier = (Function)(Function<AbstractAssociativeContainer,AbstractSequentialConstIterator<?>>)AbstractAssociativeContainer::constEnd;
				beginSupplier = (Function)(Function<AbstractAssociativeContainer,AbstractSequentialConstIterator<?>>)AbstractAssociativeContainer::constBegin;
			}else {
				endSupplier = (Function)(Function<AbstractAssociativeContainer,AbstractSequentialConstIterator<?>>)AbstractAssociativeContainer::end;
				beginSupplier = (Function)(Function<AbstractAssociativeContainer,AbstractSequentialConstIterator<?>>)AbstractAssociativeContainer::begin;
			}
		}else if(owner instanceof AbstractMultiAssociativeContainer) {
			if(isConstant()) {
				endSupplier = (Function)(Function<AbstractMultiAssociativeContainer,AbstractSequentialConstIterator<?>>)AbstractMultiAssociativeContainer::constEnd;
				beginSupplier = (Function)(Function<AbstractMultiAssociativeContainer,AbstractSequentialConstIterator<?>>)AbstractMultiAssociativeContainer::constBegin;
			}else {
				endSupplier = (Function)(Function<AbstractMultiAssociativeContainer,AbstractSequentialConstIterator<?>>)AbstractMultiAssociativeContainer::end;
				beginSupplier = (Function)(Function<AbstractMultiAssociativeContainer,AbstractSequentialConstIterator<?>>)AbstractMultiAssociativeContainer::begin;
			}
		}else {
			BeginEndFunctions functions = findBeginEndSuppliers(owner);
			beginSupplier = (Function)(isConstant() ? functions.constBegin : functions.begin);
			endSupplier = (Function)(isConstant() ? functions.constEnd : functions.end);
		}
	}
	
	protected boolean isConstant() {
		return true;
	}
	
    @QtUninvokable
	protected final AbstractSequentialConstIterator<E> end(){
		return endSupplier==null ? null : endSupplier.apply(owner);
	}
    
    @QtUninvokable
	protected final AbstractSequentialConstIterator<E> begin(){
		return beginSupplier==null ? null : beginSupplier.apply(owner);
	}
	
    @QtUninvokable
	protected final boolean compareOwners(AbstractSequentialConstIterator<?> iter) {
		return owner==iter.owner;
	}
    
    @QtUninvokable
    protected abstract boolean equals(AbstractSequentialConstIterator<?> o);
    
    @Override
    @QtUninvokable
    public boolean equals(Object other) {
        if (other instanceof AbstractSequentialConstIterator) {
        	AbstractSequentialConstIterator<?> iter = (AbstractSequentialConstIterator<?>) other;
        	if(compareOwners(iter))
        		return equals(iter);
        }
    	return false;
    }
    
    private static class IncrementalIterator<E> implements java.util.Iterator<E>{
    	final AbstractSequentialConstIterator<E> nativeIterator;
		final AbstractSequentialConstIterator<E> end;
    	private boolean hasNext;

    	IncrementalIterator(AbstractSequentialConstIterator<E> nativeIterator) {
			super();
			this.nativeIterator = nativeIterator;
			end = nativeIterator.end();
			hasNext = end!=null && !nativeIterator.equals(end);
		}

		@Override
		public boolean hasNext() {
			return hasNext;
		}

		@Override
		public E next() {
			checkNext();
            E e = nativeIterator.val();
            nativeIterator.increment();
            hasNext = end!=null && !nativeIterator.equals(end);
            return e;
		}
    	
		void checkNext() {
            if(!hasNext)
                throw new NoSuchElementException();
		}
    }
    
    private static class CheckingIncrementalIterator<E> extends IncrementalIterator<E>{
    	CheckingIncrementalIterator(AbstractSequentialConstIterator<E> nativeIterator) {
			super(nativeIterator);
		}

		void checkNext() {
    		super.checkNext();
        	if(end!=null && !end.equals(nativeIterator.end()))
        		throw new ConcurrentModificationException();
		}
    }
    
    private static class DecrementalIterator<E> implements java.util.Iterator<E>{
    	final AbstractSequentialConstIterator<E> nativeIterator;
    	final AbstractSequentialConstIterator<E> begin;
    	private boolean hasNext;

		public DecrementalIterator(AbstractSequentialConstIterator<E> nativeIterator) {
			super();
			this.nativeIterator = nativeIterator;
			begin = nativeIterator.begin();
			hasNext = begin!=null && !nativeIterator.equals(begin);
		}

		@Override
		public boolean hasNext() {
			return hasNext;
		}

		@Override
		public E next() {
			checkNext();
			nativeIterator.decrement();
            hasNext = begin!=null && !nativeIterator.equals(begin);
			E e = nativeIterator.val();
            return e;
		}
    	
		void checkNext() {
            if(!hasNext)
                throw new NoSuchElementException();
		}
    }
    
    private static class CheckingDecrementalIterator<E> extends DecrementalIterator<E>{
    	public CheckingDecrementalIterator(AbstractSequentialConstIterator<E> nativeIterator) {
			super(nativeIterator);
		}

		void checkNext() {
    		super.checkNext();
        	if(begin!=null && !begin.equals(nativeIterator.begin()))
        		throw new ConcurrentModificationException();
		}
    }
    
    @SuppressWarnings("unchecked")
	@QtUninvokable
	protected final java.util.Iterator<E> toJavaIterator(){
    	return (java.util.Iterator<E>)iteratorFactory.apply(this);
    }
    
	@SuppressWarnings("unchecked")
    @QtUninvokable
    protected final java.util.Iterator<E> toJavaDescendingIterator(){
    	return (java.util.Iterator<E>)diteratorFactory.apply(this);
    }
    
    @QtUninvokable
    protected abstract void decrement();
    
    @QtUninvokable
    protected abstract void increment();
    
    @QtUninvokable
    protected abstract E checkedValue();
    
    @QtUninvokable
    protected abstract E val();
	
	private static class BeginEndFunctions{
		BeginEndFunctions(
				Function<QtObject, AbstractSequentialConstIterator<?>> constBegin,
				Function<QtObject, AbstractSequentialConstIterator<?>> constEnd,
				Function<QtObject, AbstractSequentialConstIterator<?>> begin,
				Function<QtObject, AbstractSequentialConstIterator<?>> end) {
			super();
			this.begin = begin;
			this.end = end;
			this.constBegin = constBegin;
			this.constEnd = constEnd;
		}
		
		final Function<QtObject,AbstractSequentialConstIterator<?>> begin;
		final Function<QtObject,AbstractSequentialConstIterator<?>> end;
		final Function<QtObject,AbstractSequentialConstIterator<?>> constBegin;
		final Function<QtObject,AbstractSequentialConstIterator<?>> constEnd;
	}
	
	private static final Map<Class<?>, BeginEndFunctions> endMethodHandles = Collections.synchronizedMap(new HashMap<>());
	
	private static BeginEndFunctions findBeginEndSuppliers(QtObject beginOwner) {
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
}
