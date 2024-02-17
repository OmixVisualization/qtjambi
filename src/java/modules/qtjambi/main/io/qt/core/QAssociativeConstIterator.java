/****************************************************************************
**
** Copyright (C) 2009-2024 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import java.util.ConcurrentModificationException;
import java.util.Iterator;
import java.util.NoSuchElementException;
import java.util.Optional;
import java.util.function.Function;

import io.qt.NativeAccess;
import io.qt.QtObject;
import io.qt.QtUninvokable;

/**
 * <p>Java-iterable wrapper for Qt's constant iterator types:</p>
 * <ul>
 * <li><a href="https://doc.qt.io/qt/qmap-const-iterator.html">QMap&lt;K,V>::const_iterator</a></li>
 * <li><a href="https://doc.qt.io/qt/qhash-const-iterator.html">QHash&lt;K,V>::const_iterator</a></li>
 * <li><a href="https://doc.qt.io/qt/qmultimap-const-iterator.html">QMultiMap&lt;K,V>::const_iterator</a></li>
 * <li><a href="https://doc.qt.io/qt/qmultihash-const-iterator.html">QMultiHash&lt;K,V>::const_iterator</a></li>
 * </ul>
 * @param <Key> key type
 * @param <T> value type
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
public class QAssociativeConstIterator<Key,T> extends AbstractIterator<T> implements java.lang.Iterable<QPair<Key,T>> {

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
    final Key _key() {
        return key(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private static native <K> K key(long __this__nativeId);

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
	

	
	private final static Function<QAssociativeConstIterator<?,?>, java.util.Iterator<?>> iteratorFactory;
	
	static {
		if(Boolean.getBoolean("io.qt.enable-concurrent-container-modification-check")) {
			@SuppressWarnings("unchecked")
			Function<QAssociativeConstIterator<?,?>, java.util.Iterator<?>> _iteratorFactory = CheckingIncrementalIterator::new;
			iteratorFactory = _iteratorFactory;
		}else {
			@SuppressWarnings("unchecked")
			Function<QAssociativeConstIterator<?,?>, java.util.Iterator<?>> _iteratorFactory = IncrementalIterator::new;
			iteratorFactory = _iteratorFactory;
		}
	}
	
	private static class IncrementalIterator<Key,T> implements java.util.Iterator<QPair<Key,T>>{
    	final QAssociativeConstIterator<Key,T> nativeIterator;
    	final AbstractIterator<T> end;
    	private boolean hasNext;
    	
		IncrementalIterator(QAssociativeConstIterator<Key, T> nativeIterator) {
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
		public QPair<Key,T> next() {
			checkNext();
			QPair<Key,T> e = new QPair<>(nativeIterator._key(), nativeIterator._value());
			nativeIterator.increment();
            hasNext = end!=null && !nativeIterator.equals(end);
            return e;
		}
    	
		void checkNext() {
            if(!hasNext)
                throw new NoSuchElementException();
		}
    }
	
	private static class CheckingIncrementalIterator<Key,T> extends IncrementalIterator<Key,T>{
		CheckingIncrementalIterator(QAssociativeConstIterator<Key, T> nativeIterator) {
			super(nativeIterator);
		}

		void checkNext() {
    		super.checkNext();
        	if(end!=null && !end.equals(nativeIterator.end()))
        		throw new ConcurrentModificationException();
		}
	}

    /**
     * Returns a Java iterator between this and the container's end.
     */
	@SuppressWarnings("unchecked")
    @QtUninvokable
    public final Iterator<QPair<Key,T>> iterator(){
    	return (java.util.Iterator<QPair<Key,T>>)iteratorFactory.apply(this);
    }

    /**
	 * Returns the key value pair at iterator's position in the container or emptiness in case of <code>end</code>.
	 */
    @QtUninvokable
	public final Optional<QPair<Key,T>> keyValuePair() {
		return !isValid() ? Optional.empty() : Optional.ofNullable(new QPair<>(_key(), _value()));
	}
    
    /**
	 * Returns the key at iterator's position in the container or emptiness in case of <code>end</code>.
	 */
    @QtUninvokable
	public final Optional<Key> key() {
		return !isValid() ? Optional.empty() : Optional.ofNullable(_key());
	}
    
    /**
     * Returns the current item's key if item is valid
     * or throws NoSuchElementException otherwise.
     */
    @QtUninvokable
    final Key checkedKey() throws NoSuchElementException {
    	if(isValid()) {
    		return _key();
    	}else {
    		throw new NoSuchElementException();
    	}
	}
}
