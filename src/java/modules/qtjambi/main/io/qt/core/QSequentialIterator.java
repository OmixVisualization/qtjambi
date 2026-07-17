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

import java.util.function.*;
import java.util.*;

import io.qt.*;

/**
 * <p>Java-iterable wrapper for Qt's iterator types:</p>
 * <ul>
 * <li><a href="https://doc.qt.io/qt/qlist-iterator.html">QList&lt;T>::iterator</a></li>
 * <li>and all other sequential iterators</li>
 * </ul>
 * @param <T> value type
 * @see QList#begin()
 * @see QList#end()
 */
public class QSequentialIterator<T,Container extends QtObjectInterface> extends QSequentialConstIterator<T,Container> implements Consumer<T>{
	@NativeAccess
	protected QSequentialIterator(QtConstructInPlace p) { 
    	super(p);
	}
	
	@NativeAccess
	protected QSequentialIterator(QPrivateConstructor c) { 
    	super(c);
	}
	
	protected QSequentialIterator(QSequentialIterator<T,Container> other) { 
    	super(other);
	}
    
	/**
	 * Set the value at iterator's position in the container.
	 * @param newValue the new value
	 */
	@Deprecated(since = "QtJambi 6.11.2", forRemoval = true)
	@QtUninvokable
	private final boolean setValue(T newValue) {
    	return checkedSetValue(newValue);
    }
	
	/**
	 * Set the value at iterator's position in the container.
	 * @param newValue the new value
	 */
	@QtUninvokable
	public final void set(T newValue) {
		if(isValid()) {
			setValue(QtJambi_LibraryUtilities.internal.nativeId(this), newValue);
		}else {
			throw new NoSuchElementException();
		}
	}
    
    /**
	 * Specifies if this type is constant iterator.
	 */
	@Override
	final boolean isConstant() {
		return false;
	}
	
    /**
     * Returns a Java mutable iterator between this and the container's end.
     * @deprecated Use {@link #bidirectionalIterator()} instead.
     */
	@Deprecated(since = "QtJambi 6.11.2", forRemoval = true)
    @QtUninvokable
    public final ListIterator<T> mutableIterator(){
    	return bidirectionalIterator();
    }
	
	/**
     * Returns a Java bidirectional iterator between this and the container's end.
     */
    @QtUninvokable
    protected ListIterator<T> bidirectionalIterator(){
		return bidirectionalIterator(this);
    }
	
	/**
     * Creates and returns a copy of this object.
     */
    @Override
	public @NonNull QSequentialIterator<T,Container> clone(){
		return new QSequentialIterator<>(this);
	}

	@Override
	public final void accept(T newValue) {
		set(newValue);
	}
}
