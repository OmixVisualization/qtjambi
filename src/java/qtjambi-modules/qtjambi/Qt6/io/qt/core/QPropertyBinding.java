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

import static io.qt.internal.QtJambiInternal.registerMetaType;

import java.io.Serializable;
import java.lang.reflect.AnnotatedElement;
import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.lang.reflect.ParameterizedType;
import java.lang.reflect.Type;
import java.util.function.Supplier;

import io.qt.NativeAccess;
import io.qt.QtPointerType;
import io.qt.QtPrimitiveType;
import io.qt.QtUtilities;
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
	
	private final static ThreadLocal<Supplier<QMetaType>> pendingMetaType = new ThreadLocal<>();
	
	static void setPendingMetaType(Supplier<QMetaType> metaTypeSupplier) {
		pendingMetaType.set(metaTypeSupplier);
	}
	
	@NativeAccess
	static QMetaType analyzeMetaType(Serializable functor) {
		QtJambiInternal.LambdaInfo lamdaInfo = QtJambiInternal.lambdaInfo(functor);
		if(lamdaInfo==null || lamdaInfo.reflectiveMethod==null) {
			if(functor instanceof QtUtilities.Supplier) {
				Class<?> functorClass = QtJambiInternal.getClass(functor);
				for(Type iface : functorClass.getGenericInterfaces()) {
					if(iface instanceof ParameterizedType) {
						ParameterizedType piface = (ParameterizedType)iface;
						if(piface.getRawType()==QtUtilities.Supplier.class) {
							Type[] args = piface.getActualTypeArguments();
							for (int i = 0; i < args.length; i++) {
								try {
									Class<?> c1 = QtJambiInternal.toClass(args[i]);
									return new QMetaType(registerMetaType(c1, args[i], null, false, false));
								} catch (Exception e) {
								}
							}
						}
					}
				}
				if(functorClass.isSynthetic()) {
					if(functorClass.getName().startsWith(QProperty.class.getName())
						|| functorClass.getName().startsWith(QObject.QProperty.class.getName())) {
						for(Field field : functorClass.getDeclaredFields()) {
							if(field.getType()==QProperty.class) {
								QProperty<?> property = (QProperty<?>)QtJambiInternal.fetchField(functor, field);
								return property.valueMetaType();
							}else if(field.getType()==QObject.QProperty.class) {
								QObject.QProperty<?> property = (QObject.QProperty<?>) QtJambiInternal.fetchField(functor, field);
								return property.valueMetaType();
							}
						}
					}
					Supplier<QMetaType> metaTypeSupplier = pendingMetaType.get();
					if(metaTypeSupplier!=null) {
						QMetaType metaType = metaTypeSupplier.get();
						if(metaType!=null)
							return metaType;
					}
					QtUtilities.Supplier<?> supplier = (QtUtilities.Supplier<?>)functor;
					Object value = supplier.get();
					if(value!=null && !QtJambiInternal.getClass(value).isArray()) {
						int t = QVariant.type(value);
						String typeName;
						if(t!=QMetaType.Type.UnknownType.value()
								&& t!=QMetaType.Type.Void.value()
								&& !(typeName = new QMetaType(t).name()).equals("JObjectWrapper")
								&& !typeName.equals("JEnumWrapper")
								&& !typeName.equals("JMapWrapper")
								&& !typeName.equals("JCollectionWrapper")) {
							return new QMetaType(t);
						}
					}
				}
			}
//			throw new IllegalArgumentException("Only lambda expressions allowed.");
			return new QMetaType(QMetaType.Type.QVariant);
		}else if(lamdaInfo.owner instanceof QProperty) {
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
		AnnotatedElement rt = null;
		if(QtJambiInternal.useAnnotatedType)
			rt = method.getAnnotatedReturnType();
		int t = registerMetaType(
				method.getReturnType(), 
				method.getGenericReturnType(), 
				rt,
		        method.isAnnotationPresent(QtPointerType.class),
		        false);
		return new QMetaType(t);
	}
	
	/**
	 * Creates a new <code>QPropertyBinding</code> with the given functor <code>f</code>.
	 * @param t
	 */
	public QPropertyBinding(QtUtilities.Supplier<? extends T> f) {
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
		super(other);
	}
	
	/**
	 * Creates a new generic <code>QPropertyBinding</code> as copy of the given binding.
	 * @param binding
	 */
	public QPropertyBinding(QPropertyBinding<T> other) {
		super(other);
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
