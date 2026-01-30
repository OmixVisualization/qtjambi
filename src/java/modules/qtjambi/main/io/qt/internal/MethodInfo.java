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
package io.qt.internal;

import java.lang.invoke.MethodHandle;
import java.lang.reflect.Constructor;
import java.lang.reflect.Method;

import io.qt.core.QMetaMethod;
import io.qt.core.QMetaObject;

/**
 * @hidden
 */
public final class MethodInfo{
	MethodInfo(Class<?> implClass, 
			boolean hasCapturedArgs, 
			int ownerIndex, 
			int qobjectIndex, 
			String capturingClass, 
			MethodHandle methodHandle, 
			Method reflectiveMethod, 
			boolean isStaticMethod, 
			Constructor<?> reflectiveConstructor, 
			QMetaMethod metaMethod) {
		this.implClass = implClass;
		this.hasCapturedArgs = hasCapturedArgs;
		this.methodHandle = methodHandle;
		this.reflectiveMethod = reflectiveMethod;
		this.isStaticMethod = isStaticMethod;
		this.reflectiveConstructor = reflectiveConstructor;
		this.ownerIndex = ownerIndex;
		this.qobjectIndex = qobjectIndex;
		this.capturingClass = capturingClass;
		if(metaMethod!=null && metaMethod.isValid()) {
			metaObject = metaMethod.enclosingMetaObject();
			methodIndex = metaMethod.methodIndex();
			expectedParameterTypes = metaMethod.parameterCount();
		}else {
			metaObject = null;
			methodIndex = -1;
			expectedParameterTypes = -1;
		}
	}
	public final Class<?> implClass;
	public final boolean hasCapturedArgs;
	public final MethodHandle methodHandle;
	public final Method reflectiveMethod;
	public final boolean isStaticMethod;
	public final Constructor<?> reflectiveConstructor;
	public final QMetaObject metaObject;
	public final int methodIndex;
	public final int expectedParameterTypes;
	final int ownerIndex;
	final int qobjectIndex;
	public final String capturingClass;
	public QMetaMethod metaMethod() {
		return metaObject==null ? new QMetaMethod() : metaObject.method(methodIndex);
	}
}