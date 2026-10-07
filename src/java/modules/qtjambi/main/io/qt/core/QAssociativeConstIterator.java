/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import java.util.*;
import io.qt.*;

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
 */
public class QAssociativeConstIterator<Key,T,Container extends QtObjectInterface> extends AbstractIterator<T,Container> {

    static {
    	QtJambi_LibraryUtilities.initialize();
    }
    
    @NativeAccess
    protected QAssociativeConstIterator(QtConstructInPlace p) { 
    	super(p);
	}
    
    @NativeAccess
	protected QAssociativeConstIterator(QPrivateConstructor c) { 
    	super(c);
	}
    
	protected QAssociativeConstIterator(QAssociativeConstIterator<Key,T,Container> other) { 
    	super(other, false, false);
	}
	
	protected QAssociativeConstIterator(QAssociativeIterator<Key,T,Container> other) { 
    	super(other, true, false);
	}
    
    /**
     * Returns the current item's key.
     */
    @QtUninvokable
    final Key _key() {
        return key(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    static native <K> K key(long __this__nativeId);
    
    @QtUninvokable
    static native QMetaType keyType(long __this__nativeId);
    
    /**
	 * Returns the value at iterator's position in the container.
	 * @throws NoSuchElementException in case of <code>end</code>.
	 */
    @QtUninvokable
	public final T value() {
		return validValue();
	}
    
    /**
	 * Returns the key at iterator's position in the container.
	 * @throws NoSuchElementException in case of <code>end</code>.
	 */
    @QtUninvokable
	public final Key key() {
    	if(!isValid())
    		throw new NoSuchElementException();
		return _key();
	}
    
    /**
	 * Returns the value type of the iterator.
	 */
    @QtUninvokable
	public final QMetaType valueMetaType() {
    	return valueType(QtJambi_LibraryUtilities.internal.nativeId(this));
	}
    
    /**
	 * Returns the key type of the iterator.
	 */
    @QtUninvokable
	public final QMetaType keyMetaType() {
    	return keyType(QtJambi_LibraryUtilities.internal.nativeId(this));
	}
	
	/**
     * Creates and returns a copy of this object.
     */
    @Override
	public @NonNull QAssociativeConstIterator<Key,T,Container> clone(){
		return new QAssociativeConstIterator<>(this);
	}
}
