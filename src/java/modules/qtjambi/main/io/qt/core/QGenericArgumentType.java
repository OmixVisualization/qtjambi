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

abstract class QGenericArgumentType<T>{
	QGenericArgumentType(Class<?> classType, QMetaType metaType, int pointerOrReference) {
		super();
		if(classType==null && metaType!=null && metaType.id()!=QMetaType.Type.Nullptr.value()) {
			classType = metaType.javaType();
		}
		this.classType = classType;
		this.metaType = metaType;
		this.pointerOrReference = pointerOrReference;
	}
	@NativeAccess
	final Class<?> classType;
	@NativeAccess
	final QMetaType metaType;
	@NativeAccess
	final int pointerOrReference;
	
	/**
	 * <p>Call value by pointer (e.g. <code>QColor*</code>).</p>
	 * @return new instance
	 */
	public abstract QGenericArgumentType<T> asPointer();
	
	/**
	 * <p>Call value or object by reference (e.g. <code>QColor&amp;</code>, <code>QObject&amp;</code>).</p>
	 * @return new instance
	 */
	public abstract QGenericArgumentType<T> asRef();
	
	/**
	 * <p>Call value or object by const reference (e.g. <code>const QColor&amp;</code>).</p>
	 * @return new instance
	 */
	public abstract QGenericArgumentType<T> asConstRef();
}