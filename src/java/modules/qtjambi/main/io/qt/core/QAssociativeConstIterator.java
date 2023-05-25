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
import java.util.Optional;

import io.qt.NativeAccess;
import io.qt.QtObject;
import io.qt.QtUninvokable;
import io.qt.internal.AbstractSequentialConstIterator;

/**
 * <p>Java-iterable wrapper for Qt's constant iterator types:
 * <ul>
 * <li><a href="https://doc.qt.io/qt/qmap-const-iterator.html">QMap&lt;K,V>::const_iterator</a></li>
 * <li><a href="https://doc.qt.io/qt/qhash-const-iterator.html">QHash&lt;K,V>::const_iterator</a></li>
 * <li><a href="https://doc.qt.io/qt/qmultimap-const-iterator.html">QMultiMap&lt;K,V>::const_iterator</a></li>
 * <li><a href="https://doc.qt.io/qt/qmultihash-const-iterator.html">QMultiHash&lt;K,V>::const_iterator</a></li>
 * </ul>
 * </p>
 * @param <K> key type
 * @param <V> value type
 * @see QMap#constBegin()
 * @see QMap#constEnd()
 * @see QMap#find(Object)
 * @see QMap#lowerBound(Object)
 * @see QMap#upperBound(Object)
 * @see QHash#constBegin()
 * @see QHash#constEnd()
 * @see QHash#find(Object)
 * @see QMultiMap#constBegin()
 * @see QMultiMap#constEnd()
 * @see QMultiMap#find(Object)
 * @see QMultiMap#find(Object, Object)
 * @see QMultiMap#lowerBound(Object)
 * @see QMultiMap#upperBound(Object)
 * @see QMultiHash#constBegin()
 * @see QMultiHash#constEnd()
 * @see QMultiHash#find(Object)
 * @see #iterator()
 */
@SuppressWarnings("deprecation")
public class QAssociativeConstIterator<K,V> extends io.qt.internal.AbstractAssociativeConstIterator<K,V> implements java.lang.Iterable<QPair<K,V>>, QMapIterator<K,V> {

    static {
    	QtJambi_LibraryUtilities.initialize();
    }
    
    @NativeAccess
    QAssociativeConstIterator(QtObject owner) { 
    	super(owner); 
	}
    
    /**
     * Returns the current item's key.
     */
    @QtUninvokable
    protected final K key() {
        return key(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private static native <K> K key(long __this__nativeId);

    /**
     * Returns the current item's value.
     */
    @QtUninvokable
    protected final V val() {
        return QSequentialConstIterator.value(QtJambi_LibraryUtilities.internal.nativeId(this));
    }

    /**
     * Advances the iterator to the next item in the container.
     */
    @QtUninvokable
    protected final void increment() {
    	QSequentialConstIterator.increment(QtJambi_LibraryUtilities.internal.nativeId(this));
    }

    /**
     * Advances the iterator to the previous item in the container.
     */
    @QtUninvokable
    protected final void decrement() {
    	QSequentialConstIterator.decrement(QtJambi_LibraryUtilities.internal.nativeId(this));
    }

    @QtUninvokable
    private final boolean lessThan(AbstractSequentialConstIterator<?> other) {
        if(compareOwners(other))
        	throw new IllegalArgumentException("Incomparable objects.");
        return QSequentialConstIterator.lessThan(QtJambi_LibraryUtilities.internal.nativeId(this), QtJambi_LibraryUtilities.internal.nativeId(other));
    }

    /**
     * Returns true if other points to the same item as this iterator; otherwise returns false.
     */
    @QtUninvokable
    protected final boolean equals(AbstractSequentialConstIterator<?> o) {
        return QSequentialConstIterator.operator_equal(QtJambi_LibraryUtilities.internal.nativeId(this), QtJambi_LibraryUtilities.internal.nativeId(o));
    }

    /**
     * {@inheritDoc}
     */
	@Override
    @QtUninvokable
    public boolean equals(Object other) {
        if (other instanceof QAssociativeConstIterator) {
        	return super.equals(other);
        }
        return false;
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
    
    /**
     * {@inheritDoc}
     */
    @QtUninvokable
    public final Iterator<QPair<K,V>> iterator(){
    	return toJavaMapIterator();
    }

    /**
	 * Returns the key value pair at iterator's position in the container or emptiness in case of <code>end</code>.
	 */
    @QtUninvokable
	public final Optional<QPair<K,V>> keyValuePair() {
		return !isValid() ? Optional.empty() : Optional.ofNullable(new QPair<>(key(), val()));
	}
    
    /**
     * Returns the current item's value if item is valid
     * or throws NoSuchElementException otherwise.
     */
    @QtUninvokable
    protected final V checkedValue() throws NoSuchElementException {
    	if(isValid()) {
    		return val();
    	}else {
    		throw new NoSuchElementException();
    	}
	}
    
    /**
     * Returns the current item's key if item is valid
     * or throws NoSuchElementException otherwise.
     */
    @QtUninvokable
    protected final K checkedKey() throws NoSuchElementException {
    	if(isValid()) {
    		return key();
    	}else {
    		throw new NoSuchElementException();
    	}
	}
}
