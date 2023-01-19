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

import io.qt.QtUninvokable;
import io.qt.internal.AbstractSequentialConstIterator;

@SuppressWarnings("deprecation")
public class QAssociativeConstIterator<K,V> extends io.qt.internal.AbstractAssociativeConstIterator<K,V> implements java.lang.Iterable<QPair<K,V>>, QMapIterator<K,V> {

    static {
    	QtJambi_LibraryUtilities.initialize();
    }
    
    QAssociativeConstIterator(Object owner) { 
    	super(owner); 
	}
    
    @QtUninvokable
    protected final K key()        {
        return key(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private static native <K> K key(long __this__nativeId);

    @QtUninvokable
    protected final V val()        {
        return QSequentialConstIterator.value(QtJambi_LibraryUtilities.internal.nativeId(this));
    }

    @QtUninvokable
    protected final void increment()        {
    	QSequentialConstIterator.increment(QtJambi_LibraryUtilities.internal.nativeId(this));
    }

    @QtUninvokable
    protected final void decrement()        {
    	QSequentialConstIterator.decrement(QtJambi_LibraryUtilities.internal.nativeId(this));
    }

    @QtUninvokable
    private final boolean lessThan(AbstractSequentialConstIterator<?> other)        {
        if(compareOwners(other))
        	throw new IllegalArgumentException("Incomparable objects.");
        return QSequentialConstIterator.lessThan(QtJambi_LibraryUtilities.internal.nativeId(this), QtJambi_LibraryUtilities.internal.nativeId(other));
    }

    @QtUninvokable
    private final boolean operator_equal(QAssociativeConstIterator<K,V> o)        {
        return QSequentialConstIterator.operator_equal(QtJambi_LibraryUtilities.internal.nativeId(this), QtJambi_LibraryUtilities.internal.nativeId(o));
    }

	@Override
    @QtUninvokable
    public boolean equals(Object other) {
        if (other instanceof QAssociativeConstIterator) {
        	@SuppressWarnings("unchecked")
        	QAssociativeConstIterator<K,V> iter = (QAssociativeConstIterator<K,V>) other;
        	if(compareOwners(iter))
        		return operator_equal(iter);
        }
        return Objects.equals(other, key());
    }

    @QtUninvokable
    final boolean isValid() {
    	long nativeId = QtJambi_LibraryUtilities.internal.nativeId(this);
    	if(nativeId==0)
    		return false;
    	AbstractSequentialConstIterator<?> end = end();
    	if(QSequentialConstIterator.canLess(nativeId)) {
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
