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
package io.qt.core;

import java.util.Objects;

/**
 * <p>Specifies a function pointer's return type.</p>
 * <p>Qt's object types are represented by pointers, value types by values, e.g.:</p>
 * <p>Java class <code>QWidget</code> leads to <code>QWidget*</code> function return type,
 * Java class <code>QColor</code> leads to <code>QColor</code> function return type (as value).</p>
 * <p>Use {@link #asRef()} to return values and objects as reference type (e.g. <code>QColor&amp;</code>, <code>QObject&amp;</code>).</p>
 * <p>Use {@link #asConstRef()} to return values and objects as const reference (e.g. <code>const QColor&amp;</code>).</p>
 * <p>Use {@link #asPointer()} to return values as pointers (e.g. <code>QColor*</code>).</p>
 * @param <T>
 */
public final class QGenericReturnType<T> extends QGenericArgumentType<T>{
	QGenericReturnType(Class<?> classType, QMetaType metaType, int pointerOrReference) {
		super(classType, metaType, pointerOrReference);
	}
	
	/**
	 * <p>Return value as pointer (e.g. <code>QColor*</code>).</p>
	 * @return new instance
	 */
	public QGenericReturnType<T> asPointer(){
		if((metaType.name().endsWith("*") && pointerOrReference>=0) || pointerOrReference==1) {
			return this;
		}else {
			return new QGenericReturnType<>(classType, metaType, 1);
		}
	}
	
	/**
	 * <p>Return value or object as reference (e.g. <code>QColor&amp;</code>, <code>QObject&amp;</code>).</p>
	 * @return new instance
	 */
	public QGenericReturnType<T> asRef(){
		if(pointerOrReference==-1) {
			return this;
		}else {
			return new QGenericReturnType<>(classType, metaType, -1);
		}
	}
	
	/**
	 * <p>Return value or object as const reference (e.g. <code>const QColor&amp;</code>).</p>
	 * @return new instance
	 */
	public QGenericReturnType<T> asConstRef(){
		if(pointerOrReference==-2) {
			return this;
		}else {
			return new QGenericReturnType<>(classType, metaType, -2);
		}
	}
	
	/**
	 * Create generic return type from class type.
	 * @param <T>
	 * @param classType
	 * @param instantiations only useful to specify template types like QList&lt;T&gt;.
	 * @return new instance
	 */
	public static <T> QGenericReturnType<T> of(Class<T> classType, QMetaType... instantiations){
		QMetaType metaType = QMetaType.fromType(classType, instantiations);
		if(instantiations!=null && instantiations.length>0 && (metaType==null || !metaType.isValid()))
			throw new IllegalArgumentException("Unknown type "+classType.getName());
		return new QGenericReturnType<>(classType, metaType, 0);
	}
	
	/**
	 * Create generic return type from meta type.
	 * @param <T>
	 * @param metaType
	 * @return new instance
	 */
	public static <T> QGenericReturnType<T> of(QMetaType metaType){
		if(!metaType.isValid())
			throw new IllegalArgumentException("Invalid meta type: "+metaType.name());
		return new QGenericReturnType<T>(null, Objects.requireNonNull(metaType), 0);
	}
	
	/**
	 * Create generic return type from meta type.
	 * @param <T>
	 * @param type
	 * @return new instance
	 */
	public static <T> QGenericReturnType<T> of(QMetaType.Type type){
		QMetaType metaType = new QMetaType(type);
		if(!metaType.isValid())
			throw new IllegalArgumentException("Illegal type: QMetaType::"+type.name());
		return new QGenericReturnType<>(null, metaType, 0);
	}
}