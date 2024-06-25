/****************************************************************************
**
** Copyright (C) 2009-2024 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import java.io.Serializable;
import java.lang.reflect.AnnotatedElement;
import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.lang.reflect.ParameterizedType;
import java.lang.reflect.Type;
import java.util.function.Supplier;

import io.qt.*;
import io.qt.internal.ClassAnalyzerUtility;

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
	public static @NonNull QPropertyBinding<@QtPrimitiveType Boolean> fromBinding(@StrictNonNull QBooleanPropertyBinding binding){
		return new QPropertyBinding<>(binding);
	}
	
	/**
	 * Creates a new generic <code>QPropertyBinding</code> as copy of the given primitive-typed binding.
	 * @param binding
	 * @return binding copy
	 */
	public static @NonNull QPropertyBinding<@QtPrimitiveType Byte> fromBinding(@StrictNonNull QBytePropertyBinding binding){
		return new QPropertyBinding<>(binding);
	}
	
	/**
	 * Creates a new generic <code>QPropertyBinding</code> as copy of the given primitive-typed binding.
	 * @param binding
	 * @return binding copy
	 */
	public static @NonNull QPropertyBinding<@QtPrimitiveType Short> fromBinding(@StrictNonNull QShortPropertyBinding binding){
		return new QPropertyBinding<>(binding);
	}
	
	/**
	 * Creates a new generic <code>QPropertyBinding</code> as copy of the given primitive-typed binding.
	 * @param binding
	 * @return binding copy
	 */
	public static @NonNull QPropertyBinding<@QtPrimitiveType Integer> fromBinding(@StrictNonNull QIntPropertyBinding binding){
		return new QPropertyBinding<>(binding);
	}
	
	/**
	 * Creates a new generic <code>QPropertyBinding</code> as copy of the given primitive-typed binding.
	 * @param binding
	 * @return binding copy
	 */
	public static @NonNull QPropertyBinding<@QtPrimitiveType Long> fromBinding(@StrictNonNull QLongPropertyBinding binding){
		return new QPropertyBinding<>(binding);
	}
	
	/**
	 * Creates a new generic <code>QPropertyBinding</code> as copy of the given primitive-typed binding.
	 * @param binding
	 * @return binding copy
	 */
	public static @NonNull QPropertyBinding<@QtPrimitiveType Float> fromBinding(@StrictNonNull QFloatPropertyBinding binding){
		return new QPropertyBinding<>(binding);
	}
	
	/**
	 * Creates a new generic <code>QPropertyBinding</code> as copy of the given primitive-typed binding.
	 * @param binding
	 * @return binding copy
	 */
	public static @NonNull QPropertyBinding<@QtPrimitiveType Double> fromBinding(@StrictNonNull QDoublePropertyBinding binding){
		return new QPropertyBinding<>(binding);
	}
	
	/**
	 * Creates a new generic <code>QPropertyBinding</code> as copy of the given primitive-typed binding.
	 * @param binding
	 * @return binding copy
	 */
	public static @NonNull QPropertyBinding<@QtPrimitiveType Character> fromBinding(@StrictNonNull QCharPropertyBinding binding){
		return new QPropertyBinding<>(binding);
	}
	
	private final static ThreadLocal<Supplier<QMetaType>> pendingMetaType = new ThreadLocal<>();
	
	static void setPendingMetaType(Supplier<QMetaType> metaTypeSupplier) {
		pendingMetaType.set(metaTypeSupplier);
	}
	
	@SuppressWarnings({ "removal", "deprecation" })
	@NativeAccess
	static QMetaType analyzeMetaType(Serializable functor) {
		ClassAnalyzerUtility.LambdaInfo lamdaInfo = ClassAnalyzerUtility.lambdaInfo(functor);
		if(lamdaInfo==null || lamdaInfo.reflectiveMethod==null) {
			if(functor instanceof QtUtilities.Supplier) {
				Class<?> functorClass = QtJambi_LibraryUtilities.internal.getClass(functor);
				for(Type iface : functorClass.getGenericInterfaces()) {
					if(iface instanceof ParameterizedType) {
						ParameterizedType piface = (ParameterizedType)iface;
						if(piface.getRawType()==QtUtilities.Supplier.class) {
							Type[] args = piface.getActualTypeArguments();
							for (int i = 0; i < args.length; i++) {
								try {
									Class<?> c1 = QtJambi_LibraryUtilities.internal.toClass(args[i]);
									return new QMetaType(QtJambi_LibraryUtilities.internal.registerMetaType(c1, args[i], null, false, false));
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
								QProperty<?> property = (QProperty<?>)QtJambi_LibraryUtilities.internal.readField(functor, field);
								return property.valueMetaType();
							}else if(field.getType()==QObject.QProperty.class) {
								QObject.QProperty<?> property = (QObject.QProperty<?>) QtJambi_LibraryUtilities.internal.readField(functor, field);
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
					if(value!=null && !QtJambi_LibraryUtilities.internal.getClass(value).isArray()) {
						QMetaType t = QMetaType.fromObject(value);
						String typeName;
						if(t.id()!=QMetaType.Type.UnknownType.value()
								&& t.id()!=QMetaType.Type.Void.value()
								&& !(typeName = t.name()).equals("JObjectWrapper")
								&& !typeName.equals("JEnumWrapper")
								&& !typeName.equals("JMapWrapper")
								&& !typeName.equals("JCollectionWrapper")) {
							return t;
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
		if(ClassAnalyzerUtility.useAnnotatedType)
			rt = method.getAnnotatedReturnType();
		int t = QtJambi_LibraryUtilities.internal.registerMetaType(
				method.getReturnType(), 
				method.getGenericReturnType(), 
				rt,
		        method.isAnnotationPresent(QtPointerType.class),
		        false);
		return new QMetaType(t);
	}
	
	/**
	 * Creates a new <code>QPropertyBinding</code> with the given functor <code>f</code>.
	 * @param f
	 */
	public QPropertyBinding(QtUtilities.@StrictNonNull Supplier<? extends T> f) {
		super(f);
	}
	
	/**
	 * Creates a new <code>QPropertyBinding</code> as copy of the property's available binding.
	 * @param property
	 */
	public QPropertyBinding(@StrictNonNull QProperty<T> property) {
		super(property.bindingData());
	}
	
	/**
	 * Creates a new <code>QPropertyBinding</code> as copy of the property's available binding.
	 * @param property
	 */
	public QPropertyBinding(QObject.@StrictNonNull QProperty<T> property) {
		super(property.bindingData());
	}
	
	/**
	 * Creates a new <code>QPropertyBinding</code> as copy of the property's available binding.
	 * @param property
	 */
	public QPropertyBinding(QObject.@StrictNonNull QComputedProperty<T> property) {
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
	 * @param other
	 */
	public QPropertyBinding(@StrictNonNull QPropertyBinding<T> other) {
		super(other);
	}

	/**
	 * Creates a <code>null</code>-<code>QPropertyBinding</code>.
	 * @see QUntypedPropertyBinding#isNull()
	 */
	public QPropertyBinding() {
		super((QPropertyBindingData)null);
	}
}
