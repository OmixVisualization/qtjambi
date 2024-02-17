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

import java.util.Iterator;
import java.util.Objects;

import io.qt.NativeAccess;
import io.qt.QtObject;
import io.qt.QtUninvokable;

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
public class QSequentialConstIterator<T> extends AbstractIterator<T> implements java.lang.Iterable<T> {

	static {
    	QtJambi_LibraryUtilities.initialize();
    }
    
	@NativeAccess
    QSequentialConstIterator(QtObject owner) { 
    	super(owner);
	}

    /**
     * {@inheritDoc}
     */
	@Override
    @QtUninvokable
    public boolean equals(Object other) {
        if (other instanceof QSequentialConstIterator) {
        	return super.equals(other);
        }
        return isValid() && Objects.equals(other, _value());
    }

    /**
     * Returns a Java iterator between this and the container's end.
     */
    @QtUninvokable
    public final Iterator<T> iterator(){
    	return toJavaIterator();
    }
    
    /**
     * Returns a descending Java iterator between this and the container's end.
     */
    @QtUninvokable
	public final java.util.Iterator<T> descendingIterator() {
    	return toJavaDescendingIterator();
    }
}
