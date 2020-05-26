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

import static io.qt.core.QIterator.__qt_QIterator_decrement;
import static io.qt.core.QIterator.__qt_QIterator_increment;
import static io.qt.core.QIterator.__qt_QIterator_lessThan;
import static io.qt.core.QIterator.__qt_QIterator_operator_equal;
import static io.qt.core.QIterator.__qt_QIterator_value;

import java.lang.invoke.MethodHandle;
import java.util.Iterator;
import java.util.Objects;
import java.util.Optional;
import java.util.function.Consumer;
import java.util.function.Function;
import java.util.logging.Level;
import java.util.logging.Logger;

import io.qt.QtUninvokable;

public class QMapIterator<K,V> extends io.qt.internal.QtJambiMapIteratorObject<K,V> implements java.lang.Comparable<K>, java.lang.Iterable<QPair<K,V>> {

    static {
    	io.qt.core.QtJambi_LibraryInitializer.init();
    }
    
    private static Consumer<QMapIterator<?,?>> decrementFct = QMapIterator::decrement;
    
    private static Consumer<QMapIterator<?,?>> incrementFct = QMapIterator::increment;
    
    private static Function<QMapIterator<?,?>,?> keyFct = QMapIterator::key;
    
    private static Function<QMapIterator<?,?>,?> valueFct = QMapIterator::val;

    private final Object owner;
    private final MethodHandle endHandle;
    private final long keyFunction;
    private final long valueFunction;
    private final long incrementFunction;
    private final long decrementFunction;
    private final long lessThanFunction;
    private final long equalsFunction;

    @QtUninvokable
    protected final K key()        {
        if(keyFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QMapIterator_key(io.qt.internal.QtJambiInternal.checkedNativeId(this), keyFunction);
    }
    private static native <K> K __qt_QMapIterator_key(long __this__nativeId, long keyFunction);

    @QtUninvokable
    protected final V val()        {
        if(valueFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QIterator_value(io.qt.internal.QtJambiInternal.checkedNativeId(this), valueFunction);
    }

    @QtUninvokable
    protected final void increment()        {
        if(incrementFunction==0)
        	throw new UnsupportedOperationException();
        __qt_QIterator_increment(io.qt.internal.QtJambiInternal.checkedNativeId(this), incrementFunction);
    }

    @QtUninvokable
    protected final void decrement()        {
        if(decrementFunction==0)
        	throw new UnsupportedOperationException();
        __qt_QIterator_decrement(io.qt.internal.QtJambiInternal.checkedNativeId(this), decrementFunction);
    }

    @QtUninvokable
    private final boolean lessThan(QMapIterator<K,V> other)        {
        if(lessThanFunction==0)
        	throw new UnsupportedOperationException();
        if(other.lessThanFunction!=lessThanFunction)
        	throw new IllegalArgumentException("Incomparable objects.");
        return __qt_QIterator_lessThan(io.qt.internal.QtJambiInternal.checkedNativeId(this), io.qt.internal.QtJambiInternal.nativeId(other), lessThanFunction);
    }

    @QtUninvokable
    private final boolean operator_equal(QMapIterator<K,V> o)        {
        if(equalsFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QIterator_operator_equal(io.qt.internal.QtJambiInternal.checkedNativeId(this), io.qt.internal.QtJambiInternal.nativeId(o), equalsFunction);
    }
    @QtUninvokable
    private native boolean __qt_QMapIterator_operator_equal(long __this__nativeId, long o, long equalsFunction);

    private QMapIterator(Object owner, long keyFunction, long valueFunction, long incrementFunction, long decrementFunction, long lessThanFunction, long equalsFunction) { 
    	super(decrementFct, incrementFct, keyFct, valueFct); 
    	this.owner = owner;
    	this.endHandle = this.findEndFunction(owner);
    	this.keyFunction = keyFunction;
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
        if (other instanceof QMapIterator) {
            return operator_equal((QMapIterator<K,V>) other);
        }
        return Objects.equals(other, key());
    }

    @SuppressWarnings("unchecked")
    @QtUninvokable
	public int compareTo(Object other) {
        if (equals(other)) return 0;
        else if (other instanceof QMapIterator) {
            if (lessThan((QMapIterator<K,V>) other)) return -1;
            else return 1;
        }
        throw new ClassCastException();
    }
    
    @QtUninvokable
    private boolean isValid() {
    	return this.lessThanFunction==0 ? operator_equal(end()) : compareTo(end())<0;
    }
    
    @QtUninvokable
    private QMapIterator<K,V> end() {
    	if(endHandle!=null) {
    		try {
				return (QMapIterator<K,V>)endHandle.invoke(owner);
			} catch (Throwable e) {
				Logger.getAnonymousLogger().log(Level.SEVERE, "end()", e);
			}
    	}
    	return this;
    }
    
    @QtUninvokable
    public Iterator<QPair<K,V>> iterator(){
    	return toJavaMapIterator(this::end);
    }

    @QtUninvokable
	public Optional<QPair<K,V>> keyValuePair() {
		return isValid() ? Optional.empty() : Optional.ofNullable(new QPair<>(key(), val()));
	}
}
