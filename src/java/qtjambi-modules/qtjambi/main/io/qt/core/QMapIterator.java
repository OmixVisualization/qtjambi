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
package io.qt.core;

import static io.qt.core.QIterator.__qt_QIterator_decrement;
import static io.qt.core.QIterator.__qt_QIterator_increment;
import static io.qt.core.QIterator.__qt_QIterator_lessThan;
import static io.qt.core.QIterator.__qt_QIterator_operator_equal;
import static io.qt.core.QIterator.__qt_QIterator_value;

import java.util.Iterator;
import java.util.NoSuchElementException;
import java.util.Objects;
import java.util.Optional;

import io.qt.QtUninvokable;
import io.qt.internal.QtJambiIteratorObject;

public final class QMapIterator<K,V> extends io.qt.internal.QtJambiMapIteratorObject<K,V> implements java.lang.Iterable<QPair<K,V>> {

    static {
    	QtJambi_LibraryUtilities.initialize();
    }
    
    private QMapIterator(Object owner) { 
    	super(owner); 
	}
    
    @QtUninvokable
    protected final K key()        {
        return __qt_QMapIterator_key(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private static native <K> K __qt_QMapIterator_key(long __this__nativeId);

    @QtUninvokable
    protected final V val()        {
        return __qt_QIterator_value(QtJambi_LibraryUtilities.internal.nativeId(this));
    }

    @QtUninvokable
    protected final void increment()        {
        __qt_QIterator_increment(QtJambi_LibraryUtilities.internal.nativeId(this));
    }

    @QtUninvokable
    protected final void decrement()        {
        __qt_QIterator_decrement(QtJambi_LibraryUtilities.internal.nativeId(this));
    }

    @QtUninvokable
    private final boolean lessThan(QtJambiIteratorObject<?> other)        {
        if(compareOwners(other))
        	throw new IllegalArgumentException("Incomparable objects.");
        return __qt_QIterator_lessThan(QtJambi_LibraryUtilities.internal.nativeId(this), QtJambi_LibraryUtilities.internal.nativeId(other));
    }

    @QtUninvokable
    private final boolean operator_equal(QMapIterator<K,V> o)        {
        return __qt_QIterator_operator_equal(QtJambi_LibraryUtilities.internal.nativeId(this), QtJambi_LibraryUtilities.internal.nativeId(o));
    }

	@Override
    @QtUninvokable
    public boolean equals(Object other) {
        if (other instanceof QMapIterator) {
        	@SuppressWarnings("unchecked")
        	QMapIterator<K,V> iter = (QMapIterator<K,V>) other;
        	if(compareOwners(iter))
        		return operator_equal(iter);
        }
        return Objects.equals(other, key());
    }

    @QtUninvokable
    private final boolean isValid() {
    	long nativeId = QtJambi_LibraryUtilities.internal.nativeId(this);
    	if(nativeId==0)
    		return false;
    	QtJambiIteratorObject<?> end = end();
    	if(QIterator.canLess(nativeId)) {
	    	try {
	        	return lessThan(end);
			} catch (Exception e) {
			}
    	}
		return !equals(end);
    }
    
    @QtUninvokable
    public final Iterator<QPair<K,V>> iterator(){
    	return toJavaMapIterator();
    }

    @QtUninvokable
	public final Optional<QPair<K,V>> keyValuePair() {
		return !isValid() ? Optional.empty() : Optional.ofNullable(new QPair<>(key(), val()));
	}
    
    @QtUninvokable
    protected final V checkedValue() {
    	if(isValid()) {
    		return val();
    	}else {
    		throw new NoSuchElementException();
    	}
	}
    
    @QtUninvokable
    protected final K checkedKey() {
    	if(isValid()) {
    		return key();
    	}else {
    		throw new NoSuchElementException();
    	}
	}
}
