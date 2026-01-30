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

import io.qt.NativeAccess;
import io.qt.QtObject;
import io.qt.QtUninvokable;

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
public final class QSequentialIterator<T> extends QSequentialConstIterator<T>{
	@NativeAccess
    private QSequentialIterator(QPrivateConstructor c, QtObject owner) { 
    	super(c, owner);
	}
    
	/**
	 * Set the value at iterator's position in the container.
	 * @param newValue the new value
	 */
    @QtUninvokable
	public final boolean setValue(T newValue) {
    	return checkedSetValue(newValue);
    }
    
    /**
	 * Specifies if this type is constant iterator.
	 */
	@Override
	boolean isConstant() {
		return false;
	}
	
	/**
     * Returns a Java mutable iterator between this and the container's end.
     */
    @QtUninvokable
    public final java.util.ListIterator<T> mutableIterator(){
    	return toMutableJavaIterator();
    }
}
