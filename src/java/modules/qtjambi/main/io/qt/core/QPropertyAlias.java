/****************************************************************************
**
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import io.qt.*;

/**
 * <p>Java wrapper for Qt class <a href="https://doc.qt.io/qt/qpropertyalias.html">QPropertyAlias</a></p>
 * <p>Use this class to declare a property as alias for another one.</p>
 * <p>For primitive-typed implementations see:</p>
 * <ul>
 * <li>{@link QBooleanPropertyAlias}</li>
 * <li>{@link QBytePropertyAlias}</li>
 * <li>{@link QShortPropertyAlias}</li>
 * <li>{@link QIntPropertyAlias}</li>
 * <li>{@link QLongPropertyAlias}</li>
 * <li>{@link QFloatPropertyAlias}</li>
 * <li>{@link QDoublePropertyAlias}</li>
 * <li>{@link QCharPropertyAlias}</li>
 * </ul>
 * @deprecated Class was only meant for internal use, use a <code>QProperty</code> and add a binding to the target
 */
@Deprecated(forRemoval=true, since="Qt 6.6")
@SuppressWarnings("removal")
public final class QPropertyAlias<T> extends QAbstractPropertyAlias {
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 * @return new alias
	 */
	public static @NonNull QPropertyAlias<@NonNull@QtPrimitiveType Boolean> fromProperty(@StrictNonNull QBooleanProperty property){
		return new QPropertyAlias<>(property, QBooleanBindable.bindableInterface(0));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 * @return new alias
	 */
	public static @NonNull QPropertyAlias<@NonNull@QtPrimitiveType Byte> fromProperty(@StrictNonNull QByteProperty property){
		return new QPropertyAlias<>(property, QByteBindable.bindableInterface(0));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 * @return new alias
	 */
	public static @NonNull QPropertyAlias<@NonNull@QtPrimitiveType Short> fromProperty(@StrictNonNull QShortProperty property){
		return new QPropertyAlias<>(property, QShortBindable.bindableInterface(0));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 * @return new alias
	 */
	public static @NonNull QPropertyAlias<@NonNull@QtPrimitiveType Integer> fromProperty(@StrictNonNull QIntProperty property){
		return new QPropertyAlias<>(property, QIntBindable.bindableInterface(0));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 * @return new alias
	 */
	public static @NonNull QPropertyAlias<@NonNull@QtPrimitiveType Long> fromProperty(@StrictNonNull QLongProperty property){
		return new QPropertyAlias<>(property, QLongBindable.bindableInterface(0));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 * @return new alias
	 */
	public static @NonNull QPropertyAlias<@NonNull@QtPrimitiveType Float> fromProperty(@StrictNonNull QFloatProperty property){
		return new QPropertyAlias<>(property, QFloatBindable.bindableInterface(0));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 * @return new alias
	 */
	public static @NonNull QPropertyAlias<@NonNull@QtPrimitiveType Double> fromProperty(@StrictNonNull QDoubleProperty property){
		return new QPropertyAlias<>(property, QDoubleBindable.bindableInterface(0));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 * @return new alias
	 */
	public static @NonNull QPropertyAlias<@NonNull@QtPrimitiveType Character> fromProperty(@StrictNonNull QCharProperty property){
		return new QPropertyAlias<>(property, QCharBindable.bindableInterface(0));
	}
	
	/**
	 * Creates a new alias to the provided property alias.
	 * @param alias
	 * @return new alias
	 */
	public static @NonNull QPropertyAlias<@NonNull@QtPrimitiveType Boolean> fromAlias(@StrictNonNull QBooleanPropertyAlias alias){
		return new QPropertyAlias<>(alias);
	}
	
	/**
	 * Creates a new alias to the provided property alias.
	 * @param alias
	 * @return new alias
	 */
	public static @NonNull QPropertyAlias<@NonNull@QtPrimitiveType Byte> fromAlias(@StrictNonNull QBytePropertyAlias alias){
		return new QPropertyAlias<>(alias);
	}
	
	/**
	 * Creates a new alias to the provided property alias.
	 * @param alias
	 * @return new alias
	 */
	public static @NonNull QPropertyAlias<@NonNull@QtPrimitiveType Short> fromAlias(@StrictNonNull QShortPropertyAlias alias){
		return new QPropertyAlias<>(alias);
	}
	
	/**
	 * Creates a new alias to the provided property alias.
	 * @param alias
	 * @return new alias
	 */
	public static @NonNull QPropertyAlias<@NonNull@QtPrimitiveType Integer> fromAlias(@StrictNonNull QIntPropertyAlias alias){
		return new QPropertyAlias<>(alias);
	}
	
	/**
	 * Creates a new alias to the provided property alias.
	 * @param alias
	 * @return new alias
	 */
	public static @NonNull QPropertyAlias<@NonNull@QtPrimitiveType Long> fromAlias(@StrictNonNull QLongPropertyAlias alias){
		return new QPropertyAlias<>(alias);
	}
	
	/**
	 * Creates a new alias to the provided property alias.
	 * @param alias
	 * @return new alias
	 */
	public static @NonNull QPropertyAlias<@NonNull@QtPrimitiveType Float> fromAlias(@StrictNonNull QFloatPropertyAlias alias){
		return new QPropertyAlias<>(alias);
	}
	
	/**
	 * Creates a new alias to the provided property alias.
	 * @param alias
	 * @return new alias
	 */
	public static @NonNull QPropertyAlias<@NonNull@QtPrimitiveType Double> fromAlias(@StrictNonNull QDoublePropertyAlias alias){
		return new QPropertyAlias<>(alias);
	}
	
	/**
	 * Creates a new alias to the provided property alias.
	 * @param alias
	 * @return new alias
	 */
	public static @NonNull QPropertyAlias<@NonNull@QtPrimitiveType Character> fromAlias(@StrictNonNull QCharPropertyAlias alias){
		return new QPropertyAlias<>(alias);
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 * @return new alias
	 */
	public static @NonNull QPropertyAlias<@NonNull@QtPrimitiveType Boolean> fromProperty(QObject.@StrictNonNull QBooleanProperty property){
		return new QPropertyAlias<>(property, QBooleanBindable.bindableInterface(1));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 * @return new alias
	 */
	public static @NonNull QPropertyAlias<@NonNull@QtPrimitiveType Byte> fromProperty(QObject.@StrictNonNull QByteProperty property){
		return new QPropertyAlias<>(property, QByteBindable.bindableInterface(1));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 * @return new alias
	 */
	public static @NonNull QPropertyAlias<@NonNull@QtPrimitiveType Short> fromProperty(QObject.@StrictNonNull QShortProperty property){
		return new QPropertyAlias<>(property, QShortBindable.bindableInterface(1));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 * @return new alias
	 */
	public static @NonNull QPropertyAlias<@NonNull@QtPrimitiveType Integer> fromProperty(QObject.@StrictNonNull QIntProperty property){
		return new QPropertyAlias<>(property, QIntBindable.bindableInterface(1));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 * @return new alias
	 */
	public static @NonNull QPropertyAlias<@NonNull@QtPrimitiveType Long> fromProperty(QObject.@StrictNonNull QLongProperty property){
		return new QPropertyAlias<>(property, QLongBindable.bindableInterface(1));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 * @return new alias
	 */
	public static @NonNull QPropertyAlias<@NonNull@QtPrimitiveType Float> fromProperty(QObject.@StrictNonNull QFloatProperty property){
		return new QPropertyAlias<>(property, QFloatBindable.bindableInterface(1));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 * @return new alias
	 */
	public static @NonNull QPropertyAlias<@NonNull@QtPrimitiveType Double> fromProperty(QObject.@StrictNonNull QDoubleProperty property){
		return new QPropertyAlias<>(property, QDoubleBindable.bindableInterface(1));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 * @return new alias
	 */
	public static @NonNull QPropertyAlias<@NonNull@QtPrimitiveType Character> fromProperty(QObject.@StrictNonNull QCharProperty property){
		return new QPropertyAlias<>(property, QCharBindable.bindableInterface(1));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 * @return new alias
	 */
	public static @NonNull QPropertyAlias<@NonNull@QtPrimitiveType Boolean> fromProperty(QObject.@StrictNonNull QComputedBooleanProperty property){
		return new QPropertyAlias<>(property, QBooleanBindable.bindableInterface(2));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 * @return new alias
	 */
	public static @NonNull QPropertyAlias<@NonNull@QtPrimitiveType Byte> fromProperty(QObject.@StrictNonNull QComputedByteProperty property){
		return new QPropertyAlias<>(property, QByteBindable.bindableInterface(2));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 * @return new alias
	 */
	public static @NonNull QPropertyAlias<@NonNull@QtPrimitiveType Short> fromProperty(QObject.@StrictNonNull QComputedShortProperty property){
		return new QPropertyAlias<>(property, QShortBindable.bindableInterface(2));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 * @return new alias
	 */
	public static @NonNull QPropertyAlias<@NonNull@QtPrimitiveType Integer> fromProperty(QObject.@StrictNonNull QComputedIntProperty property){
		return new QPropertyAlias<>(property, QIntBindable.bindableInterface(2));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 * @return new alias
	 */
	public static @NonNull QPropertyAlias<@NonNull@QtPrimitiveType Long> fromProperty(QObject.@StrictNonNull QComputedLongProperty property){
		return new QPropertyAlias<>(property, QLongBindable.bindableInterface(2));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 * @return new alias
	 */
	public static @NonNull QPropertyAlias<@NonNull@QtPrimitiveType Float> fromProperty(QObject.@StrictNonNull QComputedFloatProperty property){
		return new QPropertyAlias<>(property, QFloatBindable.bindableInterface(2));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 * @return new alias
	 */
	public static @NonNull QPropertyAlias<@NonNull@QtPrimitiveType Double> fromProperty(QObject.@StrictNonNull QComputedDoubleProperty property){
		return new QPropertyAlias<>(property, QDoubleBindable.bindableInterface(2));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 * @return new alias
	 */
	public static @NonNull QPropertyAlias<@NonNull@QtPrimitiveType Character> fromProperty(QObject.@StrictNonNull QComputedCharProperty property){
		return new QPropertyAlias<>(property, QCharBindable.bindableInterface(2));
	}
	
	private QPropertyAlias(QUntypedPropertyData aliasedPropertyPtr, QBindableInterface iface) {
		super(aliasedPropertyPtr, iface);
	}
	
	private QPropertyAlias(QAbstractPropertyAlias other) {
		super(other);
	}

	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 */
	public QPropertyAlias(@StrictNonNull QProperty<T> property) {
		super(property, QBindable.bindableInterface(property.valueMetaType().id(), 0));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 */
	public QPropertyAlias(QObject.@StrictNonNull QProperty<T> property) {
		super(property, QBindable.bindableInterface(property.valueMetaType().id(), 1));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 */
	public QPropertyAlias(QObject.@StrictNonNull QComputedProperty<T> property) {
		super(property, QBindable.bindableInterface(property.valueMetaType().id(), 2));
	}
	
	/**
	 * Creates a new alias to the provided alias.
	 * @param other
	 */
	public QPropertyAlias(@StrictNonNull QPropertyAlias<T> other) {
		super(other);
	}
	
	/**
	 * Creates a new alias to the provided binding's underlying property.
	 * @param bindable
	 */
	public QPropertyAlias(@StrictNonNull QBindable<T> bindable) {
		super(bindable.data(), bindable.iface());
	}
	
	/**
	 * @see QProperty#value()
	 */
	@QtUninvokable
	public T value() {
        return QBindable.value(QtJambi_LibraryUtilities.internal.nativeId(this.iface), QtJambi_LibraryUtilities.internal.nativeId(this.aliasedProperty()), 0);
    }
	
	/**
	 * @see QProperty#setValue(Object)
	 */
	@QtUninvokable
	public void setValue(T newValue) {
		QBindable.setValue(QtJambi_LibraryUtilities.internal.nativeId(this.iface), QtJambi_LibraryUtilities.internal.nativeId(this.aliasedProperty()), 0, newValue);
    }
	
	/**
	 * @see QProperty#setBinding(QPropertyBinding)
	 */
	@QtUninvokable
	public @NonNull QPropertyBinding<T> setBinding(@StrictNonNull QPropertyBinding<T> newBinding) {
		return new QBindable<T>(aliasedProperty(), iface).setBinding(newBinding);
    }
	
	/**
	 * @see QProperty#setBinding(io.qt.QtUtilities.Supplier)
	 */
	@QtUninvokable
	public @NonNull QPropertyBinding<T> setBinding(QtUtilities.@StrictNonNull Supplier<T> functor) {
        return setBinding(new QPropertyBinding<>(functor));
    }
	
	/**
	 * @see QProperty#binding()
	 */
	@QtUninvokable
	public @NonNull QPropertyBinding<T> binding() {
        return new QBindable<T>(aliasedProperty(), iface).binding();
    }
	
	/**
	 * @see QProperty#takeBinding()
	 */
	@QtUninvokable
	public @NonNull QPropertyBinding<T> takeBinding() {
        return new QBindable<T>(aliasedProperty(), iface).takeBinding();
    }
}
