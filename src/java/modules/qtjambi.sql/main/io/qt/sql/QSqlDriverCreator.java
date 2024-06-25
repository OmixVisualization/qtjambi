/****************************************************************************
**
** Copyright (C) 2009-2024 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

package io.qt.sql;

import java.lang.reflect.Constructor;
import java.lang.reflect.Modifier;
import java.util.Objects;
import java.util.function.Supplier;

import io.qt.NonNull;
import io.qt.StrictNonNull;

/**
 * <p>The QSqlDriverCreator class is a template class that provides a SQL driver factory for a specific driver type.</p>
 * <p>Java wrapper for Qt's class <a href="https://doc.qt.io/qt/qsqldrivercreator.html">QSqlDriverCreator</a></p>
 */
public class QSqlDriverCreator<T extends QSqlDriver> extends QSqlDriverCreatorBase {
	
	private final Supplier<T> constructorHandle;

	/**
	 * Create new QSqlDriverCreator by using <code>T::new</code>.
	 * @param constructor
	 */
	public QSqlDriverCreator(@StrictNonNull Supplier<@NonNull T> constructor) {
		constructorHandle = Objects.requireNonNull(constructor);
	}
	
	/**
	 * Create new QSqlDriverCreator by using <code>T.class</code>.
	 * @param type
	 */
	public QSqlDriverCreator(@StrictNonNull Class<T> type) {
		if(Modifier.isAbstract(type.getModifiers())) {
        	throw new RuntimeException("Cannot construct QSqlDriverCreator for abstract widget type "+type.getName()+".");
        }
    	Constructor<T> constructor;
        try {
        	constructor = type.getDeclaredConstructor();
		} catch (NoSuchMethodException | SecurityException e) {
            throw new RuntimeException("Cannot construct QSqlDriverCreator for widget type "+type.getName()+" due to missing constructor "+type.getSimpleName()+"().", e);
		}
		constructorHandle = QtJambi_LibraryUtilities.internal.getFactory0(constructor);
	}
	
    /**
     * <p>See <a href="https://doc.qt.io/qt/qsqldrivercreator.html#createObject">QSqlDriverCreator::createObject() const</a></p>
     */
	@Override
	public @NonNull T createObject() {
		return constructorHandle.get();
	}

}
