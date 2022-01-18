/****************************************************************************
**
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
package io.qt.core;

import io.qt.NativeAccess;

/**
 * <p>Specifies a function pointer's argument type and value.</p>
 * <p>Qt's object types are represented by pointers, value types by values, e.g.:</p>
 * <p>Java class <code>QWidget</code> leads to <code>QWidget*</code> function return type,
 * Java class <code>QColor</code> leads to <code>QColor</code> function return type (as value).</p>
 * <p>Use {@link #asRef()} to call values and objects by reference type (e.g. <code>QColor&amp;</code>, <code>QObject&amp;</code>).</p>
 * <p>Use {@link #asConstRef()} to call values and objects by const reference (e.g. <code>const QColor&amp;</code>).</p>
 * <p>Use {@link #asPointer()} to call values by pointers (e.g. <code>QColor*</code>).</p>
 * @param <T>
 */
public final class QGenericArgument extends QGenericArgumentType<Object>{
	QGenericArgument(Class<?> classType, QMetaType metaType, int pointerOrReference, Object value) {
		super(classType, metaType, pointerOrReference);
		this.value = value;
	}
	
	@NativeAccess
	private final Object value;
	
	/**
	 * <p>Call value by pointer (e.g. <code>QColor*</code>).</p>
	 * @return new instance
	 */
	public QGenericArgument asPointer(){
		if((metaType.name().endsWith("*") && pointerOrReference>=0) || pointerOrReference==1) {
			return this;
		}else {
			return new QGenericArgument(classType, metaType, 1, value);
		}
	}
	
	/**
	 * <p>Call value or object by reference (e.g. <code>QColor&amp;</code>, <code>QObject&amp;</code>).</p>
	 * @return new instance
	 */
	public QGenericArgument asRef(){
		if(pointerOrReference==-1) {
			return this;
		}else {
			return new QGenericArgument(classType, metaType, -1, value);
		}
	}
	
	/**
	 * <p>Call value or object by const reference (e.g. <code>const QColor&amp;</code>).</p>
	 * @return new instance
	 */
	public QGenericArgument asConstRef(){
		if(pointerOrReference==-2) {
			return this;
		}else {
			return new QGenericArgument(classType, metaType, -2, value);
		}
	}
	
	/**
	 * Call java object as specified type.
	 * @param classType
	 * @param instantiations
	 * @return new instance
	 */
	public <T> QGenericArgument as(Class<? super T> classType, QMetaType... instantiations){
		QMetaType metaType = QMetaType.fromType(classType, instantiations);
		if(metaType==null || !metaType.isValid())
			throw new IllegalArgumentException("Unknown type "+classType.getName());
		return as(metaType);
	}
	
	/**
	 * Call java object as specified type.
	 * @param classType
	 * @param instantiations
	 * @return new instance
	 */
	public QGenericArgument as(QMetaType metaType){
		if(value==null)
			return new QGenericArgument(metaType.javaType(), metaType, 0, metaType.create());
		else
			return new QGenericArgument(value.getClass(), metaType, 0, value);
	}
	
	/**
	 * Call java object as specified type.
	 * @param classType
	 * @param instantiations
	 * @return new instance
	 */
	public QGenericArgument as(QMetaType.Type type){
		QMetaType metaType = new QMetaType(type);
		if(!metaType.isValid())
			throw new IllegalArgumentException("Illegal type: QMetaType::"+type.name());
		return as(metaType);
	}
	
	/**
	 * Call java object.
	 * @param value
	 * @return instance
	 */
	public static QGenericArgument of(Object value){
		return new QGenericArgument(value==null ? null : value.getClass(), value==null || value instanceof Object[] ? null : metaType(value), 0, value);
	}
	
	private static QMetaType metaType(Object obj) {
        if(obj==null)
            return new QMetaType(QMetaType.Type.Nullptr);
        if(obj.getClass()==QList.class) {
            return QMetaType.fromType(QList.class, ((QList<?>)obj).elementMetaType());
        }else if(obj.getClass()==QQueue.class) {
            return QMetaType.fromType(QQueue.class, ((QQueue<?>)obj).elementMetaType());
        }else if(obj.getClass()==QStack.class) {
            return QMetaType.fromType(QStack.class, ((QStack<?>)obj).elementMetaType());
        }else if(obj.getClass()==QSet.class) {
            return QMetaType.fromType(QSet.class, ((QSet<?>)obj).elementMetaType());
        }else if(obj.getClass()==QMultiMap.class) {
            return QMetaType.fromType(QMultiMap.class, ((QMultiMap<?,?>)obj).keyMetaType(), ((QMultiMap<?,?>)obj).valueMetaType());
        }else if(obj.getClass()==QMap.class) {
            return QMetaType.fromType(QMap.class, ((QMap<?,?>)obj).keyMetaType(), ((QMap<?,?>)obj).valueMetaType());
        }else if(obj.getClass()==QMultiHash.class) {
            return QMetaType.fromType(QMultiHash.class, ((QMultiHash<?,?>)obj).keyMetaType(), ((QMultiHash<?,?>)obj).valueMetaType());
        }else if(obj.getClass()==QHash.class) {
            return QMetaType.fromType(QHash.class, ((QHash<?,?>)obj).keyMetaType(), ((QHash<?,?>)obj).valueMetaType());
        }
        return QMetaType.fromType(obj.getClass());
    }
	
	/**
	 * Create generic argument with default value of given type.
	 * @param classType
	 * @param instantiations
	 * @return instance
	 */
	public static QGenericArgument defaultOf(Class<?> classType, QMetaType... instantiations){
		return of(null).as(classType, instantiations);
	}
	
	/**
	 * Create generic argument with default value of given type.
	 * @param metaType
	 * @return instance
	 */
	public static QGenericArgument defaultOf(QMetaType metaType){
		return of(null).as(metaType);
	}
	
	/**
	 * Create generic argument with default value of given type.
	 * @param type
	 * @return instance
	 */
	public static QGenericArgument defaultOf(QMetaType.Type type){
		return of(null).as(type);
	}
	
	/**
	 * Create generic return type from class type.
	 * @param classType
	 * @param instantiations only useful to specify template types like QList&lt;T&gt;.
	 * @return new instance
	 */
	public static <T> QGenericReturnType<T> returning(Class<T> classType, QMetaType... instantiations){
		return QGenericReturnType.<T>of(classType, instantiations);
	}
	
	/**
	 * Create generic return type from meta type.
	 * @param metaType
	 * @return new instance
	 */
	public static <T> QGenericReturnType<T> returning(QMetaType metaType){
		return QGenericReturnType.<T>of(metaType);
	}
	
	/**
	 * Create generic return type from meta type.
	 * @param metaType
	 * @return new instance
	 */
	public static <T> QGenericReturnType<T> returning(QMetaType.Type type){
		return QGenericReturnType.<T>of(type);
	}
}