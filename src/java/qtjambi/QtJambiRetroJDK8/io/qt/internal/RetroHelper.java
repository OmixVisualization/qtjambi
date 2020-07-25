/****************************************************************************
**
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

package io.qt.internal;

import java.lang.reflect.AnnotatedType;
import java.lang.reflect.Field;
import java.util.function.Supplier;

import io.qt.core.QPair;

public class RetroHelper {
	
	static boolean trySetAccessible(Field f) {
		try {
			f.setAccessible(true);
		} catch (Throwable e) {
		}
		return f.isAccessible();
	}
	
	static AnnotatedType getAnnotatedOwnerType(AnnotatedType actualType) {
		return null;
	}
	
	@SuppressWarnings("deprecation")
	private static Supplier<Class<?>> callerClassProvider = ()->sun.reflect.Reflection.getCallerClass(4);
	
	private static Supplier<QPair<Class<?>, String>> classAccessChecker = ()->{
		StackTraceElement[] stackTrace = Thread.currentThread().getStackTrace();
		if(stackTrace.length>4) {
			try {
				return new QPair<Class<?>, String>(Class.forName(stackTrace[4].getClassName()), stackTrace[4].getMethodName());
			} catch (ClassNotFoundException e) {
				e.printStackTrace();
			}
		}
		return null;
	};
	
	static Supplier<Class<?>> callerClassProvider(){
		return callerClassProvider;
	}
	
	static Supplier<QPair<Class<?>, String>> classAccessChecker(){
		return classAccessChecker;
	}
}
