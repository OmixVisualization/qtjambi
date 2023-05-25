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

import io.qt.NativeAccess;
import io.qt.QtObject;
import io.qt.QtUninvokable;
import io.qt.internal.AbstractAssociativeIterator;

/**
 * <p>Java-iterable wrapper for Qt's iterator types:
 * <ul>
 * <li><a href="https://doc.qt.io/qt/qmap-iterator.html">QMap&lt;K,V>::iterator</a></li>
 * <li><a href="https://doc.qt.io/qt/qhash-iterator.html">QHash&lt;K,V>::iterator</a></li>
 * <li><a href="https://doc.qt.io/qt/qmultimap-iterator.html">QMultiMap&lt;K,V>::iterator</a></li>
 * <li><a href="https://doc.qt.io/qt/qmultihash-iterator.html">QMultiHash&lt;K,V>::iterator</a></li>
 * </ul>
 * </p>
 * @param <K> key type
 * @param <V> value type
 * @see QMap#begin()
 * @see QMap#end()
 * @see QHash#begin()
 * @see QHash#end()
 * @see QMultiMap#begin()
 * @see QMultiMap#end()
 * @see QMultiHash#begin()
 * @see QMultiHash#end()
 */
public final class QAssociativeIterator<K,V> extends QAssociativeConstIterator<K,V> implements AbstractAssociativeIterator<K,V>{
    
	@NativeAccess
    private QAssociativeIterator(QtObject owner) { 
    	super(owner);
	}
    
	/**
	 * Set the value at iterator's position in the container.
	 * @param newValue the new value
	 */
    @QtUninvokable
	public final boolean setValue(V newValue) {
    	if(!isValid())
    		return false;
    	return true;
    }
    
    @QtUninvokable
    static native <T> void setValue(long __this__nativeId, T newValue);
    
    @Override
	protected boolean isConstant() {
		return false;
	}
}
