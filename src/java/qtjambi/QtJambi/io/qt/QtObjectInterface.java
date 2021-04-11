/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2021 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

/**
 * The super interface of all interface types in Qt Jambi.
 */
public interface QtObjectInterface{
    /**
     * Explicitly removes the native resources held by the
     * object. Note that though this method does not guarantee that
     * the object will be garbage collected, it is not safe to
     * reference the object after it has been disposed.
     */
	@io.qt.QtDeclaredFinal
	public default void dispose() {
		QtJambiInternal.disposeObject(this);
	}
	
    /**
     * Is java object connected to native object?
     * @return true if disposed
     */
	@io.qt.QtDeclaredFinal
	public default boolean isDisposed() {
		return QtJambiInternal.isObjectDisposed(this);
	}
}
