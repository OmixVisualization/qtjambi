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

import java.util.Iterator;
import java.util.NoSuchElementException;
import java.util.Objects;
import java.util.Optional;
import java.util.function.Supplier;

import io.qt.QtUninvokable;
import io.qt.internal.AbstractSequentialConstIterator;

@SuppressWarnings("deprecation")
public class QSequentialConstIterator<T> extends io.qt.internal.AbstractSequentialConstIterator<T> implements java.lang.Iterable<T>, QIterator<T> {

	static {
    	QtJambi_LibraryUtilities.initialize();
    }
    
    QSequentialConstIterator(Object owner) { 
    	super(owner);
	}
    
    @QtUninvokable
    static native boolean canLess(long __this__nativeId);
    
    @QtUninvokable
    private T val() {
        return value(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    static native <T> T value(long __this__nativeId);

    @QtUninvokable
    protected final void increment() {
        increment(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    static native void increment(long __this__nativeId);

    @QtUninvokable
    protected final void decrement() {
        decrement(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    static native void decrement(long __this__nativeId);

    @QtUninvokable
    private boolean lessThan(AbstractSequentialConstIterator<?> other) {
        return lessThan(QtJambi_LibraryUtilities.internal.nativeId(this), QtJambi_LibraryUtilities.internal.nativeId(other));
    }
    @QtUninvokable
    static native boolean lessThan(long __this__nativeId, long other);

    @QtUninvokable
    private boolean operator_equal(QSequentialConstIterator<T> o)        {
        return operator_equal(QtJambi_LibraryUtilities.internal.nativeId(this), QtJambi_LibraryUtilities.internal.nativeId(o));
    }
    @QtUninvokable
    static native boolean operator_equal(long __this__nativeId, long o);

	@Override
    @QtUninvokable
    public boolean equals(Object other) {
        if (other instanceof QSequentialConstIterator) {
        	@SuppressWarnings("unchecked")
        	QSequentialConstIterator<T> iter = (QSequentialConstIterator<T>) other;
        	if(compareOwners(iter))
        		return operator_equal(iter);
        }
        return Objects.equals(other, value());
    }

    @QtUninvokable
    boolean isValid() {
    	long nativeId = QtJambi_LibraryUtilities.internal.nativeId(this);
    	if(nativeId==0)
    		return false;
    	AbstractSequentialConstIterator<?> end = end();
    	if(canLess(nativeId)) {
	    	try {
	        	return lessThan(end);
			} catch (Exception e) {
			}
    	}
		return !equals(end);
    }
    
    @QtUninvokable
    public final Iterator<T> iterator(){
    	return toJavaIterator();
    }
    
    @QtUninvokable
	protected final java.util.Iterator<T> descendingIterator(Supplier<? extends AbstractSequentialConstIterator<T>> beginSupplier) {
    	return toJavaDescendingIterator(beginSupplier);
    }

    @QtUninvokable
	public final Optional<T> value() {
		return !isValid() ? Optional.empty() : Optional.ofNullable(val());
	}
    
    @QtUninvokable
    protected final T checkedValue() {
    	if(isValid()) {
    		return val();
    	}else {
    		throw new NoSuchElementException();
    	}
	}
}
