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
import java.util.function.*;

import io.qt.*;

/**
 * <p>Java-iterable wrapper for Qt's constant iterator types:</p>
 * <ul>
 * <li><a href="https://doc.qt.io/qt/qlist-const-iterator.html">QList&lt;T>::const_iterator</a></li>
 * <li><a href="https://doc.qt.io/qt/qset-const-iterator.html">QSet&lt;T>::const_iterator</a></li>
 * <li>and all other sequential constant iterators</li>
 * </ul>
 * @param <T> value type
 * @see QList#constBegin()
 * @see QList#constEnd()
 * @see QSet#constBegin()
 * @see QSet#constEnd()
 * @see #iterator()
 */
public class QSequentialConstPairIterator<Key,T,Container extends QtObjectInterface> extends AbstractIterator<T,Container> implements Iterable<QPair<Key,T>>, Supplier<QPair<Key,T>> {

	static {
    	QtJambi_LibraryUtilities.initialize();
    }
    
	@NativeAccess
	protected QSequentialConstPairIterator(QtConstructInPlace p) { 
    	super(p);
	}
	
	@NativeAccess
	protected QSequentialConstPairIterator(QPrivateConstructor c) { 
    	super(c);
	}
	
	protected QSequentialConstPairIterator(QSequentialConstPairIterator<Key,T,Container> other) { 
    	super(other, false, false);
	}
	
	protected QSequentialConstPairIterator(QAssociativeConstIterator<Key,T,Container> other) { 
    	super(other, false, false);
	}
	
	protected QSequentialConstPairIterator(QSequentialPairIterator<Key,T,Container> other) { 
    	super(other, true, false);
	}
	
    /**
     * Returns a Java iterator between this and the container's end.
     */
    @QtUninvokable
    public final Iterator<QPair<Key,T>> iterator(){
    	if(canCopy())
    		return iterator(clone());
    	else
    		return iterator(this, false);
    }
	
	/**
     * Creates and returns a copy of this object.
     */
    @Override
	public @NonNull QSequentialConstPairIterator<Key,T,Container> clone(){
		return new QSequentialConstPairIterator<>(this);
	}
    
    /**
	 * Returns the value type of the iterator.
	 */
    @QtUninvokable
	public final QMetaType elementMetaType() {
    	long nativeId = QtJambi_LibraryUtilities.internal.nativeId(this);
    	if(AbstractIterator.isAssociative(nativeId)) {
    		return QMetaType.fromType(QPair.class, QAssociativeConstIterator.keyType(nativeId), valueType(nativeId));
    	}else {
    		return valueType(nativeId);
    	}
	}
    
    /**
	 * Returns the key value pair at iterator's position in the container.
	 * @throws NoSuchElementException in case of <code>end</code>.
	 */
    @QtUninvokable
	public final QPair<Key,T> get() {
    	if(!isValid())
    		throw new NoSuchElementException();
    	long nativeId = QtJambi_LibraryUtilities.internal.nativeId(this);
    	if(AbstractIterator.isAssociative(nativeId)) {
    		return new QPair<>(QAssociativeConstIterator.key(nativeId), value(nativeId));
    	}else {
    		return value(nativeId);
    	}
	}
}
