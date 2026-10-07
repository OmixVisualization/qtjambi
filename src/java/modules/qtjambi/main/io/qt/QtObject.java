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
package io.qt;

/**
 * Super class of all native types provided by QtJambi.
 * The native counterpart is owned by the object if it has been constructed in Java.
 * This counterpart is assumed to be created during Java constructor call
 * and deleted as soon as garbage collection deletes the Java object.
 * 
 * Call {@link #dispose()} to disconnect the Java object from its the native counterpart.
 * If the native component is owned by by Java it is deleted this way.
 * Otherwise, only the link between native and Java object is removed.
 * To check if the Java object is alive use {@link #isDisposed()}.
 */
public abstract class QtObject extends Utility.Object implements QtObjectInterface{
    
    static {
    	QtJambi_LibraryUtilities.initialize();
    }
    
    /**
     * This type is used as solo argument for internal constructors.
     * @hidden
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
     * @hidden
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
	@Override
    public final void dispose() {
    	super.dispose();
    }
    
    /**
     * Is java object connected to native object?
     * @return true if disposed
     */
    @QtUninvokable
	@Override
    public final boolean isDisposed() {
    	return super.isDisposed();
    }
    
    /**
     * Used to check if this object shares the same native counterpart with the other object,
     * i.e. the objects are equals if their native counterparts are identical.
     * Qt value types override this method to make a value comparison.
     * Otherwise it will return the result of calling the super class implementation.
     *
     * @param other other object
     * @return true if the native counterpart of the two objects are one and the same.
     */
    @Override
    public boolean equals(java.lang.Object other) {
    	return super.equals(other);
    }
}