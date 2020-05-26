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

import java.lang.invoke.MethodHandle;
import java.util.Iterator;
import java.util.Objects;
import java.util.Optional;
import java.util.function.Consumer;
import java.util.function.Function;
import java.util.logging.Level;
import java.util.logging.Logger;

import io.qt.QtUninvokable;

public final class QIterator<T> extends io.qt.internal.QtJambiIteratorObject<T> implements java.lang.Comparable<T>, java.lang.Iterable<T> {

	static {
    	io.qt.core.QtJambi_LibraryInitializer.init();
    }
    
    private static Consumer<QIterator<?>> decrementFct = QIterator::decrement;
    
    private static Consumer<QIterator<?>> incrementFct = QIterator::increment;
    
    private static Function<QIterator<?>,?> valueFct = QIterator::val;
    
    private final Object owner;
    private final MethodHandle endHandle;
    private final long valueFunction;
    private final long incrementFunction;
    private final long decrementFunction;
    private final long lessThanFunction;
    private final long equalsFunction;

    @QtUninvokable
    private T val() {
        if(valueFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QIterator_value(io.qt.internal.QtJambiInternal.checkedNativeId(this), valueFunction);
    }
    static native <T> T __qt_QIterator_value(long __this__nativeId, long valueFunction);

    @QtUninvokable
    private void increment() {
        if(incrementFunction==0)
        	throw new UnsupportedOperationException();
        __qt_QIterator_increment(io.qt.internal.QtJambiInternal.checkedNativeId(this), incrementFunction);
    }
    static native void __qt_QIterator_increment(long __this__nativeId, long incrementFunction);

    @QtUninvokable
    private void decrement() {
        if(decrementFunction==0)
        	throw new UnsupportedOperationException();
        __qt_QIterator_decrement(io.qt.internal.QtJambiInternal.checkedNativeId(this), decrementFunction);
    }
    static native void __qt_QIterator_decrement(long __this__nativeId, long decrementFunction);

    @QtUninvokable
    private boolean lessThan(QIterator<T> other) {
        if(lessThanFunction==0)
        	throw new UnsupportedOperationException();
        if(other.lessThanFunction!=lessThanFunction)
        	throw new IllegalArgumentException("Incomparable objects.");
        return __qt_QIterator_lessThan(io.qt.internal.QtJambiInternal.checkedNativeId(this), io.qt.internal.QtJambiInternal.nativeId(other), lessThanFunction);
    }
    static native boolean __qt_QIterator_lessThan(long __this__nativeId, long other, long lessThanFunction);

    @QtUninvokable
    private boolean operator_equal(QIterator<T> o)        {
        if(equalsFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QIterator_operator_equal(io.qt.internal.QtJambiInternal.checkedNativeId(this), io.qt.internal.QtJambiInternal.nativeId(o), equalsFunction);
    }
    static native boolean __qt_QIterator_operator_equal(long __this__nativeId, long o, long equalsFunction);

    private QIterator(Object owner, long valueFunction, long incrementFunction, long decrementFunction, long lessThanFunction, long equalsFunction) { 
    	super(decrementFct, incrementFct, valueFct);
    	this.owner = owner;
    	this.endHandle = this.findEndFunction(owner);
    	this.valueFunction = valueFunction;
    	this.incrementFunction = incrementFunction;
    	this.decrementFunction = decrementFunction;
    	this.lessThanFunction = lessThanFunction;
    	this.equalsFunction = equalsFunction;
	} 

	@SuppressWarnings("unchecked")
	@Override
    @QtUninvokable
    public boolean equals(Object other) {
        if (other instanceof QIterator) {
            return operator_equal((QIterator<T>) other);
        }
        return Objects.equals(other, value());
    }

    @SuppressWarnings("unchecked")
    @QtUninvokable
	public int compareTo(Object other) {
        if (equals(other)) return 0;
        else if (other instanceof QIterator) {
            if (lessThan((QIterator<T>) other)) return -1;
            else return 1;
        }
        throw new ClassCastException();
    }
    
    @QtUninvokable
    private boolean isValid() {
    	return this.lessThanFunction==0 ? operator_equal(end()) : compareTo(end())<0;
    }
    
    @QtUninvokable
    private QIterator<T> end() {
    	if(endHandle!=null) {
    		try {
				return (QIterator<T>)endHandle.invoke(owner);
			} catch (Throwable e) {
				Logger.getAnonymousLogger().log(Level.SEVERE, "end()", e);
			}
    	}
    	return this;
    }
    
    @QtUninvokable
    public Iterator<T> iterator(){
    	return toJavaIterator(this::end);
    }

    @QtUninvokable
	public Optional<T> value() {
		return isValid() ? Optional.empty() : Optional.ofNullable(val());
	}
}
