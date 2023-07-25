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
package io.qt.core;

import java.lang.reflect.Method;
import java.util.Collections;
import java.util.ConcurrentModificationException;
import java.util.HashMap;
import java.util.Map;
import java.util.NoSuchElementException;
import java.util.Optional;
import java.util.function.Function;

import io.qt.QtObject;
import io.qt.QtUninvokable;
import io.qt.internal.ClassAnalyzerUtility;

abstract class AbstractIterator<T> extends QtObject{
	
	private final static Function<AbstractIterator<?>, java.util.Iterator<?>> iteratorFactory;
	private final static Function<AbstractIterator<?>, java.util.Iterator<?>> diteratorFactory;
	
	static {
		if(Boolean.getBoolean("io.qt.enable-concurrent-container-modification-check")) {
			@SuppressWarnings("unchecked")
			Function<AbstractIterator<?>, java.util.Iterator<?>> _iteratorFactory = CheckingIncrementalIterator::new;
			@SuppressWarnings("unchecked")
			Function<AbstractIterator<?>, java.util.Iterator<?>> _diteratorFactory = CheckingDecrementalIterator::new;
			iteratorFactory = _iteratorFactory;
			diteratorFactory = _diteratorFactory;
		}else {
			@SuppressWarnings("unchecked")
			Function<AbstractIterator<?>, java.util.Iterator<?>> _iteratorFactory = IncrementalIterator::new;
			@SuppressWarnings("unchecked")
			Function<AbstractIterator<?>, java.util.Iterator<?>> _diteratorFactory = DecrementalIterator::new;
			iteratorFactory = _iteratorFactory;
			diteratorFactory = _diteratorFactory;
		}
	}
	
	private final QtObject owner;
	private final Function<QtObject,AbstractIterator<T>> beginSupplier;
	private final Function<QtObject,AbstractIterator<T>> endSupplier;

	/**
     * {@inheritDoc}
	 */
	@SuppressWarnings({ "unchecked", "rawtypes" })
	AbstractIterator(QtObject owner) {
		super((QPrivateConstructor)null);
		this.owner = owner;
		if(owner instanceof AbstractSequentialContainer) {
			if(isConstant()) {
				endSupplier = (Function)(Function<AbstractSequentialContainer,AbstractIterator<?>>)AbstractSequentialContainer::constEnd;
				beginSupplier = (Function)(Function<AbstractSequentialContainer,AbstractIterator<?>>)AbstractSequentialContainer::constBegin;
			}else if(owner instanceof AbstractList) {
				endSupplier = (Function)(Function<AbstractList,AbstractIterator<?>>)AbstractList::end;
				beginSupplier = (Function)(Function<AbstractList,AbstractIterator<?>>)AbstractList::begin;
			}else {// there are no mutable iterators in QSet
				endSupplier = null;
				beginSupplier = null;
			}
		}else if(owner instanceof AbstractAssociativeContainer) {
			if(isConstant()) {
				endSupplier = (Function)(Function<AbstractAssociativeContainer,AbstractIterator<?>>)AbstractAssociativeContainer::constEnd;
				beginSupplier = (Function)(Function<AbstractAssociativeContainer,AbstractIterator<?>>)AbstractAssociativeContainer::constBegin;
			}else {
				endSupplier = (Function)(Function<AbstractAssociativeContainer,AbstractIterator<?>>)AbstractAssociativeContainer::end;
				beginSupplier = (Function)(Function<AbstractAssociativeContainer,AbstractIterator<?>>)AbstractAssociativeContainer::begin;
			}
		}else if(owner instanceof AbstractMultiAssociativeContainer) {
			if(isConstant()) {
				endSupplier = (Function)(Function<AbstractMultiAssociativeContainer,AbstractIterator<?>>)AbstractMultiAssociativeContainer::constEnd;
				beginSupplier = (Function)(Function<AbstractMultiAssociativeContainer,AbstractIterator<?>>)AbstractMultiAssociativeContainer::constBegin;
			}else {
				endSupplier = (Function)(Function<AbstractMultiAssociativeContainer,AbstractIterator<?>>)AbstractMultiAssociativeContainer::end;
				beginSupplier = (Function)(Function<AbstractMultiAssociativeContainer,AbstractIterator<?>>)AbstractMultiAssociativeContainer::begin;
			}
		}else {
			BeginEndFunctions functions = findBeginEndSuppliers(owner);
			beginSupplier = (Function)(isConstant() ? functions.constBegin : functions.begin);
			endSupplier = (Function)(isConstant() ? functions.constEnd : functions.end);
		}
	}
	
	/**
	 * Specifies if this type is constant iterator.
	 */
	boolean isConstant() {
		return true;
	}
	
	/**
	 * Returns the container's end iterator.
	 */
    @QtUninvokable
	final AbstractIterator<T> end(){
		return endSupplier==null ? null : endSupplier.apply(owner);
	}
    
	/**
	 * Returns the container's begin iterator.
	 */
    @QtUninvokable
	final AbstractIterator<T> begin(){
		return beginSupplier==null ? null : beginSupplier.apply(owner);
	}
	
    @QtUninvokable
    private static native boolean operator_equal(long __this__nativeId, long o);
    
    /**
     * Compares this iterator with other object.
     */
    @Override
    @QtUninvokable
    public boolean equals(Object other) {
        if (other instanceof AbstractIterator) {
        	AbstractIterator<?> iter = (AbstractIterator<?>) other;
        	if(owner==iter.owner)
        		return operator_equal(QtJambi_LibraryUtilities.internal.nativeId(this), QtJambi_LibraryUtilities.internal.nativeId(iter));
        }
    	return false;
    }
    
    private static class IncrementalIterator<E> implements java.util.Iterator<E>{
    	final AbstractIterator<E> nativeIterator;
		final AbstractIterator<E> end;
    	private boolean hasNext;

    	IncrementalIterator(AbstractIterator<E> nativeIterator) {
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
            E e = nativeIterator._value();
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
    	CheckingIncrementalIterator(AbstractIterator<E> nativeIterator) {
			super(nativeIterator);
		}

		void checkNext() {
    		super.checkNext();
        	if(end!=null && !end.equals(nativeIterator.end()))
        		throw new ConcurrentModificationException();
		}
    }
    
    private static class DecrementalIterator<E> implements java.util.Iterator<E>{
    	final AbstractIterator<E> nativeIterator;
    	final AbstractIterator<E> begin;
    	private boolean hasNext;

		public DecrementalIterator(AbstractIterator<E> nativeIterator) {
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
			E e = nativeIterator._value();
            return e;
		}
    	
		void checkNext() {
            if(!hasNext)
                throw new NoSuchElementException();
		}
    }
    
    private static class CheckingDecrementalIterator<E> extends DecrementalIterator<E>{
    	public CheckingDecrementalIterator(AbstractIterator<E> nativeIterator) {
			super(nativeIterator);
		}

		void checkNext() {
    		super.checkNext();
        	if(begin!=null && !begin.equals(nativeIterator.begin()))
        		throw new ConcurrentModificationException();
		}
    }
    
    /**
     * Returns a java iterator over the containers entries between this and end.
     */
    @SuppressWarnings("unchecked")
	@QtUninvokable
	final java.util.Iterator<T> toJavaIterator(){
    	return (java.util.Iterator<T>)iteratorFactory.apply(this);
    }
    
    /**
     * Returns a descending java iterator over the containers entries between this and end.
     */
	@SuppressWarnings("unchecked")
    @QtUninvokable
    final java.util.Iterator<T> toJavaDescendingIterator(){
    	return (java.util.Iterator<T>)diteratorFactory.apply(this);
    }
    
    /**
     * Advances the iterator to the next item in the container.
     */
    @QtUninvokable
    final void increment() {
        increment(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    static native void increment(long __this__nativeId);

    /**
     * Advances the iterator to the previous item in the container.
     */
    @QtUninvokable
    final void decrement() {
        decrement(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    static native void decrement(long __this__nativeId);
    
    /**
     * Returns the current item's value.
     */
    @QtUninvokable
    final T _value() {
        return value(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    static native <T> T value(long __this__nativeId);

    /**
	 * Returns the value at iterator's position in the container or emptiness in case of <code>end</code>.
	 */
    @QtUninvokable
	public final Optional<T> value() {
		return !isValid() ? Optional.empty() : Optional.ofNullable(_value());
	}
    
    /**
     * Returns the current item's value if item is valid
     * or throws NoSuchElementException otherwise.
     */
    @QtUninvokable
    final T checkedValue() throws NoSuchElementException {
    	if(isValid()) {
    		return _value();
    	}else {
    		throw new NoSuchElementException();
    	}
	}
    
    @QtUninvokable
    private static native boolean canLess(long __this__nativeId);
    @QtUninvokable
    private static native boolean lessThan(long __this__nativeId, long other);
    
    /**
     * Returns {@code true} if iterator is valid, i.e. it is less end.
     */
    @QtUninvokable
    final boolean isValid() {
    	long nativeId = QtJambi_LibraryUtilities.internal.nativeId(this);
    	if(nativeId==0)
    		return false;
    	AbstractIterator<?> end = end();
    	if(canLess(nativeId)) {
	    	try {
	        	return lessThan(nativeId, QtJambi_LibraryUtilities.internal.nativeId(end));
			} catch (Exception e) {
			}
    	}
		return owner!=end.owner || !operator_equal(nativeId, QtJambi_LibraryUtilities.internal.nativeId(end));
    }
	
	private static class BeginEndFunctions{
		BeginEndFunctions(
				Function<QtObject, AbstractIterator<?>> constBegin,
				Function<QtObject, AbstractIterator<?>> constEnd,
				Function<QtObject, AbstractIterator<?>> begin,
				Function<QtObject, AbstractIterator<?>> end) {
			super();
			this.begin = begin;
			this.end = end;
			this.constBegin = constBegin;
			this.constEnd = constEnd;
		}
		
		final Function<QtObject,AbstractIterator<?>> begin;
		final Function<QtObject,AbstractIterator<?>> end;
		final Function<QtObject,AbstractIterator<?>> constBegin;
		final Function<QtObject,AbstractIterator<?>> constEnd;
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
					&& cls!=null && cls != QtObject.class) {
				Method methods[] = cls.getDeclaredMethods();
				for (Method method : methods) {
					if (method.getParameterCount() == 0
							&& AbstractIterator.class.isAssignableFrom(method.getReturnType())) {
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
				if(beginMethod.getReturnType()!=QSequentialIterator.class
						&& beginMethod.getReturnType()!=QAssociativeIterator.class) {
					constBeginMethod = beginMethod;
					beginMethod = null;
				}
			}
			if(constEndMethod==null && endMethod!=null) {
				if(endMethod.getReturnType()!=QSequentialIterator.class
						&& endMethod.getReturnType()!=QAssociativeIterator.class) {
					constEndMethod = endMethod;
					endMethod = null;
				}
			}
			if (endMethod != null || constEndMethod != null)
				return new BeginEndFunctions(CoreUtility.functionFromMethod(constBeginMethod), 
											CoreUtility.functionFromMethod(constEndMethod), 
											CoreUtility.functionFromMethod(beginMethod), 
											CoreUtility.functionFromMethod(endMethod));
			else return new BeginEndFunctions(null,null,null,null);
		});
	}
	
	private static class CoreUtility extends io.qt.internal.CoreUtility{
		protected static <A,B> Function<A,B> functionFromMethod(Method method){
            return io.qt.internal.CoreUtility.functionFromMethod(method);
        }
    }
}
