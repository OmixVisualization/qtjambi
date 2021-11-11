/****************************************************************************
**
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

package io.qt.sql;

import java.lang.reflect.Modifier;
import java.util.Objects;
import java.util.function.Supplier;

public class QSqlDriverCreator<T extends QSqlDriver> extends QSqlDriverCreatorBase {
	
	private native long getConstructorHandle(Class<T> type) throws NoSuchMethodException, SecurityException, IllegalAccessException;
	private native T create(Class<T> type, long constructor);
	
	private final Supplier<T> constructorHandle;

	public QSqlDriverCreator(Supplier<T> constructor) {
		constructorHandle = Objects.requireNonNull(constructor);
	}
	
	public QSqlDriverCreator(Class<T> type) {
		if(Modifier.isAbstract(type.getModifiers())) {
        	throw new RuntimeException("Cannot construct QSqlDriverCreator for abstract widget type "+type.getName()+".");
        }
    	long constructor;
        try {
        	constructor = getConstructorHandle(type);
		} catch (NoSuchMethodException | SecurityException e) {
            throw new RuntimeException("Cannot construct QSqlDriverCreator for widget type "+type.getName()+" due to missing constructor "+type.getSimpleName()+"().", e);
		} catch (IllegalAccessException e) {
			throw new RuntimeException("Cannot construct QSqlDriverCreator for widget type "+type.getName()+" due to missing access to constructor "+type.getSimpleName()+"().", e);
		}
		constructorHandle = ()->{
			return create(type, constructor);
		};
	}
	
	@Override
	public T createObject() {
		return constructorHandle.get();
	}

}
