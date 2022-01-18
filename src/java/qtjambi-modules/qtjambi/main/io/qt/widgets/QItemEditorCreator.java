/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2022 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

package io.qt.widgets;

import java.lang.reflect.Constructor;
import java.lang.reflect.Modifier;
import java.util.Objects;

import io.qt.core.QByteArray;

/**
 * <p>Java wrapper for Qt class <a href="https://doc.qt.io/qt/qitemeditorcreator.html">QItemEditorCreator</a></p>
 */
public class QItemEditorCreator<T extends QWidget> implements QItemEditorCreatorBase {
	
	public interface ConstructorHandle<T>{
		T create(QWidget parent);
	}

	private final ConstructorHandle<T> constructorHandle;
	private final QByteArray valuePropertyName;

	public QItemEditorCreator(String valuePropertyName, ConstructorHandle<T> constructorHandle) {
        this(new QByteArray(Objects.requireNonNull(valuePropertyName)), constructorHandle);
    }
	
    public QItemEditorCreator(QByteArray valuePropertyName, ConstructorHandle<T> constructor) {
        super();
        this.constructorHandle = Objects.requireNonNull(constructor);
        this.valuePropertyName = valuePropertyName.clone();
    }
    
    public QItemEditorCreator(String valuePropertyName, Class<T> widgetType) {
        this(new QByteArray(Objects.requireNonNull(valuePropertyName)), widgetType);
    }
	
    public QItemEditorCreator(QByteArray valuePropertyName, Class<T> widgetType) {
        this(valuePropertyName, widgetType, findConstructor("QItemEditorCreator", widgetType));
    }
    
    static long findConstructor(String requestingClass, Class<?> widgetType){
    	if(Modifier.isAbstract(widgetType.getModifiers())) {
        	throw new RuntimeException("Cannot construct "+requestingClass+" for abstract widget type "+widgetType.getName()+".");
        }
    	Constructor<?> constructor;
        try {
        	constructor = widgetType.getDeclaredConstructor(QWidget.class);
		} catch (NoSuchMethodException | SecurityException e) {
            throw new RuntimeException("Cannot construct "+requestingClass+" for widget type "+widgetType.getName()+" due to missing constructor T(QWidget).", e);
		}
        return constructorHandle(constructor);
    }
    
    private QItemEditorCreator(QByteArray valuePropertyName, Class<T> widgetType, long constructorHandle) {
    	super();
        this.constructorHandle = parent -> construct(widgetType, constructorHandle, parent);
        this.valuePropertyName = valuePropertyName.clone();
    }
    
    static native long constructorHandle(Constructor<?> constructor);
    static native <T> T construct(Class<T> cls, long constructorHandle, QWidget parent);

    @Override
    public T createWidget(QWidget parent) {
    	T widget = this.constructorHandle.create(parent);
    	if(widget!=null && widget.parent()==null) {
    		QtJambi_LibraryUtilities.internal.setJavaOwnership(widget);
    	}
    	return widget;
    }

    @Override
    public QByteArray valuePropertyName() {
        return valuePropertyName.clone();
    }
}
