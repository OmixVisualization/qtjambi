/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2020 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
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
package io.qt;

import io.qt.internal.QtJambiInternal;
import io.qt.internal.QtJambiObject;

/**
 * The super class of all class types in Qt Jambi. Loading this class or any
 * of its subclasses will imply a dependency on both the Qt Jambi
 * library and the Qt libraries.
 */
public abstract class QtObject extends QtJambiObject{
    
    /**
     * This type is used as solo argument for internal constructors.
     */
    protected static final class QPrivateConstructor { private QPrivateConstructor() {} }

    /**
     * Creates a new QtObject.
     */
	public QtObject() {
		super();
	}

    /**
     * Constructor for internal use only.
     * @param p expected to be <code>null</code>.
     */
	protected QtObject(QPrivateConstructor p) {
		super(p);
	}
	
    /**
     * Explicitly removes the native resources held by the
     * object. Note that though this method does not guarantee that
     * the object will be garbage collected, it is not safe to
     * reference the object after it has been disposed.
     */
    public final void dispose() {
    	QtJambiInternal.disposeObject(this);
    }
    
    /**
     * Is java object connected to native object?
     * @return true if disposed
     */
    @QtUninvokable
    public final boolean isDisposed() {
    	return QtJambiInternal.isObjectDisposed(this);
    }
    
    /**
     * Used to check if this object shares native resources with the other object
     * if the other object is a subtype of QtJambiInterface. Otherwise it will
     * return the result of calling the super class implementation.
     *
     * @param other The object with which to compare
     * @return true if the native resources of the two objects are one and the same.
     */
    @Override
    public boolean equals(Object other) {
    	Boolean result = QtJambiInternal.areObjectsEquals(this, other);
    	return result!=null ? result : super.equals(other);
    }
}
