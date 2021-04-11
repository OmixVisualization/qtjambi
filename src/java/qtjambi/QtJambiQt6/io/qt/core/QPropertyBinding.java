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
package io.qt.core;

import static io.qt.internal.QtJambiInternal.registerMetaType;

import java.lang.reflect.Method;

import io.qt.QtPointerType;
import io.qt.QtPrimitiveType;
import io.qt.QtReferenceType;
import io.qt.QtUtilities;
import io.qt.internal.NativeAccess;
import io.qt.internal.QtJambiInternal;

/**
 * <p>Java wrapper for Qt class <a href="https://doc.qt.io/qt/qpropertybinding.html">QPropertyBinding</a></p>
 * <p>For primitive-typed implementations see:</p>
 * <ul>
 * <li>{@link QBooleanPropertyBinding}</li>
 * <li>{@link QBytePropertyBinding}</li>
 * <li>{@link QShortPropertyBinding}</li>
 * <li>{@link QIntPropertyBinding}</li>
 * <li>{@link QLongPropertyBinding}</li>
 * <li>{@link QFloatPropertyBinding}</li>
 * <li>{@link QDoublePropertyBinding}</li>
 * <li>{@link QCharPropertyBinding}</li>
 * </ul>
 */
public final class QPropertyBinding<T> extends QUntypedPropertyBinding {
	
	/**
	 * Creates a new generic <code>QPropertyBinding</code> as copy of the given primitive-typed binding.
	 * @param binding
	 * @return binding copy
	 */
	public static QPropertyBinding<@QtPrimitiveType Boolean> fromBinding(QBooleanPropertyBinding binding){
		return new QPropertyBinding<>(binding);
	}
	
	/**
	 * Creates a new generic <code>QPropertyBinding</code> as copy of the given primitive-typed binding.
	 * @param binding
	 * @return binding copy
	 */
	public static QPropertyBinding<@QtPrimitiveType Byte> fromBinding(QBytePropertyBinding binding){
		return new QPropertyBinding<>(binding);
	}
	
	/**
	 * Creates a new generic <code>QPropertyBinding</code> as copy of the given primitive-typed binding.
	 * @param binding
	 * @return binding copy
	 */
	public static QPropertyBinding<@QtPrimitiveType Short> fromBinding(QShortPropertyBinding binding){
		return new QPropertyBinding<>(binding);
	}
	
	/**
	 * Creates a new generic <code>QPropertyBinding</code> as copy of the given primitive-typed binding.
	 * @param binding
	 * @return binding copy
	 */
	public static QPropertyBinding<@QtPrimitiveType Integer> fromBinding(QIntPropertyBinding binding){
		return new QPropertyBinding<>(binding);
	}
	
	/**
	 * Creates a new generic <code>QPropertyBinding</code> as copy of the given primitive-typed binding.
	 * @param binding
	 * @return binding copy
	 */
	public static QPropertyBinding<@QtPrimitiveType Long> fromBinding(QLongPropertyBinding binding){
		return new QPropertyBinding<>(binding);
	}
	
	/**
	 * Creates a new generic <code>QPropertyBinding</code> as copy of the given primitive-typed binding.
	 * @param binding
	 * @return binding copy
	 */
	public static QPropertyBinding<@QtPrimitiveType Float> fromBinding(QFloatPropertyBinding binding){
		return new QPropertyBinding<>(binding);
	}
	
	/**
	 * Creates a new generic <code>QPropertyBinding</code> as copy of the given primitive-typed binding.
	 * @param binding
	 * @return binding copy
	 */
	public static QPropertyBinding<@QtPrimitiveType Double> fromBinding(QDoublePropertyBinding binding){
		return new QPropertyBinding<>(binding);
	}
	
	/**
	 * Creates a new generic <code>QPropertyBinding</code> as copy of the given primitive-typed binding.
	 * @param binding
	 * @return binding copy
	 */
	public static QPropertyBinding<@QtPrimitiveType Character> fromBinding(QCharPropertyBinding binding){
		return new QPropertyBinding<>(binding);
	}
	
	@NativeAccess
	static QMetaType analyzeMetaType(QtUtilities.Supplier<?> functor) {
		QtJambiInternal.LambdaInfo lamdaInfo = QtJambiInternal.lamdaInfo(functor);
		if(lamdaInfo==null || lamdaInfo.reflectiveMethod==null)
			throw new IllegalArgumentException("Only lambda expressions allowed.");
		if(lamdaInfo.owner instanceof QProperty) {
			QProperty<?> property = (QProperty<?>)lamdaInfo.owner;
			return property.valueMetaType();
		}else if(lamdaInfo.owner instanceof QObject.QProperty) {
			QObject.QProperty<?> property = (QObject.QProperty<?>)lamdaInfo.owner;
			return property.valueMetaType();
		}else if(lamdaInfo.owner instanceof QObject.QComputedProperty) {
			QObject.QComputedProperty<?> property = (QObject.QComputedProperty<?>)lamdaInfo.owner;
			return property.valueMetaType();
		}else if(lamdaInfo.owner instanceof QPropertyAlias) {
			QPropertyAlias<?> property = (QPropertyAlias<?>)lamdaInfo.owner;
			if(property.iface!=null)
				return property.iface.metaType();
		}else if(lamdaInfo.owner instanceof QBindable) {
			QBindable<?> bindable = (QBindable<?>)lamdaInfo.owner;
			return bindable.valueMetaType();
		}
		Method method = lamdaInfo.reflectiveMethod;
		int t = registerMetaType(
				method.getReturnType(), 
				method.getGenericReturnType(), 
		        method.isAnnotationPresent(QtPointerType.class) || method.getAnnotatedReturnType().isAnnotationPresent(QtPointerType.class),
		        method.isAnnotationPresent(QtReferenceType.class) || method.getAnnotatedReturnType().isAnnotationPresent(QtReferenceType.class));
		return new QMetaType(t);
	}
	
	/**
	 * Creates a new <code>QPropertyBinding</code> with the given functor <code>f</code>.
	 * @param t
	 */
	public QPropertyBinding(QtUtilities.Supplier<T> f) {
		super(f);
	}
	
	/**
	 * Creates a new <code>QPropertyBinding</code> as copy of the property's available binding.
	 * @param property
	 */
	public QPropertyBinding(QProperty<T> property) {
		super(property.bindingData());
	}
	
	/**
	 * Creates a new <code>QPropertyBinding</code> as copy of the property's available binding.
	 * @param property
	 */
	public QPropertyBinding(QObject.QProperty<T> property) {
		super(property.bindingData());
	}
	
	/**
	 * Creates a new <code>QPropertyBinding</code> as copy of the property's available binding.
	 * @param property
	 */
	public QPropertyBinding(QObject.QComputedProperty<T> property) {
		super(property.bindingData());
	}
	
	@NativeAccess
	private QPropertyBinding(QPrivateConstructor p) {
		super(p);
	}

	QPropertyBinding(QUntypedPropertyBinding other) {
		super(other, true);
	}
	
	/**
	 * Creates a new generic <code>QPropertyBinding</code> as copy of the given binding.
	 * @param binding
	 */
	public QPropertyBinding(QPropertyBinding<T> other) {
		super(other, true);
	}

	/**
	 * Creates a <code>null</code>-<code>QPropertyBinding</code>.
	 * @param binding
	 * @return binding copy
	 * @see QUntypedPropertyBinding#isNull()
	 */
	public QPropertyBinding() {
		super((QPropertyBindingData)null);
	}
}
