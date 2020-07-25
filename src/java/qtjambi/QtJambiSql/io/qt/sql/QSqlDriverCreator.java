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

package io.qt.sql;

import java.lang.invoke.MethodHandle;
import java.lang.reflect.Constructor;
import java.lang.reflect.Modifier;
import java.util.Objects;
import java.util.function.Supplier;

import io.qt.internal.QtJambiInternal;

public class QSqlDriverCreator<T extends QSqlDriver> extends QSqlDriverCreatorBase {
	
	private final Supplier<T> constructorHandle;

	public QSqlDriverCreator(Supplier<T> constructor) {
		constructorHandle = Objects.requireNonNull(constructor);
	}
	
	public QSqlDriverCreator(Class<T> type) {
		MethodHandle handle = findConstructor(type);
		constructorHandle = ()->{
			Object object;
			try {
				object = handle.invoke();
			} catch (RuntimeException | Error e) {
				throw e;
			} catch (Throwable e) {
				throw new RuntimeException(e);
			}
			return type.cast(object);
		};
	}
	
	private static MethodHandle findConstructor(Class<?> type){
    	if(Modifier.isAbstract(type.getModifiers())) {
        	throw new RuntimeException("Cannot construct QSqlDriverCreator for abstract widget type "+type.getName()+".");
        }
    	Constructor<?> constructor;
        try {
        	constructor = type.getDeclaredConstructor();
		} catch (NoSuchMethodException | SecurityException e) {
            throw new RuntimeException("Cannot construct QSqlDriverCreator for widget type "+type.getName()+" due to missing constructor "+type.getSimpleName()+"().", e);
		}
        try {
			return QtJambiInternal.getConstructorHandle(constructor);
		} catch (IllegalAccessException e) {
			throw new RuntimeException("Cannot construct QSqlDriverCreator for widget type "+type.getName()+" due to missing access to constructor "+type.getSimpleName()+"().", e);
		}
    }
	
	@Override
	public T createObject() {
		return constructorHandle.get();
	}

}
