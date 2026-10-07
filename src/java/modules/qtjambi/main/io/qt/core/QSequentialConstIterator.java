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
public class QSequentialConstIterator<T,Container extends QtObjectInterface> extends AbstractIterator<T,Container> implements Iterable<T>, Supplier<T> {
    
	@NativeAccess
	protected QSequentialConstIterator(QtConstructInPlace p) { 
    	super(p);
	}
	
	@NativeAccess
	protected QSequentialConstIterator(QPrivateConstructor c) { 
    	super(c);
	}
	
	protected QSequentialConstIterator(QSequentialConstIterator<T,Container> other) { 
    	super(other, false, false);
	}
	
	protected QSequentialConstIterator(QSequentialIterator<T,Container> other) { 
    	super(other, true, false);
	}
	
	/**
	 * KeyIterator constructor
	 */
	@SuppressWarnings({ "rawtypes", "unchecked" })
	QSequentialConstIterator(QAssociativeConstIterator other, boolean targetConst) { 
    	super(other, targetConst, true);
	}
	
	/**
	 * KeyIterator constructor
	 */
	@SuppressWarnings({ "rawtypes", "unchecked" })
	QSequentialConstIterator(QAssociativeConstIterator other) { 
    	super(other, true, true);
	}
	
    /**
     * Returns a Java iterator between this and the container's end.
     */
    @QtUninvokable
    public final Iterator<T> iterator(){
    	if(canCopy())
    		return iterator(clone());
    	else
    		return iterator(this, false);
    }
    
    /**
     * Returns a descending Java iterator between this and the container's end.
     */
    @QtUninvokable
	protected Iterator<T> descendingIterator() {
    	if(canCopy())
    		return descendingIterator(clone());
    	else
    		return descendingIterator(this, false);
    }
	
	/**
     * Returns a Java bidirectional iterator between this and the container's end.
     */
    @QtUninvokable
    protected ListIterator<T> bidirectionalIterator(){
    	if(canCopy()) {
			return bidirectionalIterator(clone());
    	}else{
    		return bidirectionalIterator(this);
		}
    }
    
	/**
     * Creates and returns a copy of this object.
     */
    @Override
	public @NonNull QSequentialConstIterator<T,Container> clone(){
		return new QSequentialConstIterator<>(this);
	}
    
    /**
	 * Returns the value at iterator's position in the container.
	 * @throws NoSuchElementException in case of <code>end</code>.
	 */
    @QtUninvokable
	public final T get() {
		return validValue();
	}
    
    /**
	 * Returns the value type of the iterator.
	 */
    @QtUninvokable
	public final QMetaType elementMetaType() {
    	return super.valueType();
	}
}
