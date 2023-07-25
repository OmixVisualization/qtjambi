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

package io.qt.widgets;

import java.lang.reflect.Constructor;
import java.lang.reflect.Modifier;
import java.util.Objects;
import java.util.function.Function;

import io.qt.core.QByteArray;

/**
 * <p>Java wrapper for Qt class <a href="https://doc.qt.io/qt/qitemeditorcreator.html">QItemEditorCreator</a></p>
 */
public class QItemEditorCreator<T extends QWidget> implements QItemEditorCreatorBase {
	
	private final Function<QWidget, T> constructorHandle;
	private final QByteArray valuePropertyName;
	
	public QItemEditorCreator(String valuePropertyName, Function<QWidget, T> constructorHandle) {
        this(new QByteArray(Objects.requireNonNull(valuePropertyName)), constructorHandle);
    }
	
    public QItemEditorCreator(QByteArray valuePropertyName, Function<QWidget, T> constructorHandle) {
        super();
        this.constructorHandle = Objects.requireNonNull(constructorHandle);
        this.valuePropertyName = valuePropertyName.clone();
    }
    
    public QItemEditorCreator(String valuePropertyName, Class<T> widgetType) {
        this(new QByteArray(Objects.requireNonNull(valuePropertyName)), widgetType);
    }
	
    public QItemEditorCreator(QByteArray valuePropertyName, Class<T> widgetType) {
        this(valuePropertyName, findConstructor("QItemEditorCreator", widgetType));
    }
    
    static <T> Function<QWidget, T> findConstructor(String requestingClass, Class<T> widgetType){
    	if(Modifier.isAbstract(widgetType.getModifiers())) {
        	throw new RuntimeException("Cannot construct "+requestingClass+" for abstract widget type "+widgetType.getName()+".");
        }
    	Constructor<T> constructor;
        try {
        	constructor = widgetType.getDeclaredConstructor(QWidget.class);
		} catch (NoSuchMethodException | SecurityException e) {
            throw new RuntimeException("Cannot construct "+requestingClass+" for widget type "+widgetType.getName()+" due to missing constructor T(QWidget).", e);
		}
        return QtJambi_LibraryUtilities.internal.getFactory1(constructor);
    }

    @Override
    public final T createWidget(QWidget parent) {
    	return this.constructorHandle.apply(parent);
    }

    @Override
    public final QByteArray valuePropertyName() {
        return valuePropertyName.clone();
    }
}
