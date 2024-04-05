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
package io.qt.internal;

import java.lang.reflect.Field;
import java.util.function.BiFunction;

import io.qt.QtObject;
import io.qt.core.QMetaProperty;
import io.qt.core.QMetaType;
import io.qt.core.QObject;

/**
 * @hidden
 */
public abstract class PropertyUtility {
	protected PropertyUtility() {throw new RuntimeException();}
    
    protected static <PI> PI analyzeProperty(QObject object, QtObject property, BiFunction<Field, QMetaType, PI> fun1, BiFunction<Field, QMetaProperty, PI> fun2) {
    	return AbstractMetaObjectUtility.analyzeProperty(object, property, fun1, fun2);
    }
    
    protected static void registerPropertyField(QMetaProperty metaProperty, java.lang.reflect.Field field) {
    	AbstractMetaObjectUtility.registerPropertyField(metaProperty, field);
    }
}
