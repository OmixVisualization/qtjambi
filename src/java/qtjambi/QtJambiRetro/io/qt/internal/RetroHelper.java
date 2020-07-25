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
import java.util.Optional;
import java.util.function.Supplier;
import io.qt.core.QPair;

public class RetroHelper {
	private static final StackWalker stackWalker = StackWalker.getInstance(java.util.Collections.singleton(StackWalker.Option.RETAIN_CLASS_REFERENCE));
	private static final Supplier<Class<?>> callerClassProvider = stackWalker::getCallerClass;
	private static final Supplier<QPair<Class<?>, String>> callerClassProvider2 = ()->{
		Optional<StackWalker.StackFrame> stackFrame = stackWalker.walk(stream->stream.limit(4).skip(3).findFirst());
		if(stackFrame.isPresent()) {
			return new QPair<>(
				stackFrame.get().getDeclaringClass(),
				stackFrame.get().getMethodName());
		}else {
			return null;
		}
	};
	
	static boolean trySetAccessible(Field f) {
		return f.trySetAccessible();
	}
	
	static AnnotatedType getAnnotatedOwnerType(AnnotatedType actualType) {
		return actualType.getAnnotatedOwnerType();
	}
	
	static Supplier<Class<?>> callerClassProvider(){
		return callerClassProvider;
	}
	
	static Supplier<QPair<Class<?>, String>> classAccessChecker(){
		return callerClassProvider2;
	}
}
