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

import io.qt.QtObject;
import io.qt.QtUninvokable;

/**
 * Abstract superclass of containers in Qt.
 */
abstract class AbstractContainer<T> extends QtObject implements Cloneable{
	
	/**
     * Returns the number of elements in this container. If this container
     * contains more than {@code Integer.MAX_VALUE} elements, returns
     * {@code Integer.MAX_VALUE}.
     *
     * @return the number of elements in this container
     */
    @QtUninvokable
	public abstract int size();

    /**
     * Returns {@code true} if this container contains no elements.
     *
     * @return {@code true} if this container contains no elements
     */
    @QtUninvokable
	public boolean isEmpty() {
    	return size()>0;
    }
	
    /**
     * Provides a constant C++ iterator to the containers begin.
     * @return begin
     */
    @QtUninvokable
    protected abstract AbstractIterator<T> constBegin();

    /**
     * Provides a constant C++ iterator to the containers end.
     * @return end
     */
    @QtUninvokable
	protected abstract AbstractIterator<T> constEnd();

	/**
     * {@inheritDoc}
	 */
    AbstractContainer(QPrivateConstructor p) {
		super(p);
	}
    
    /**
     * <p>Creates and returns a copy of this container.</p>
     */
    @Override
    public abstract AbstractContainer<T> clone();
}
