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

import io.qt.QtPrimitiveType;
import io.qt.QtUninvokable;
import io.qt.QtUtilities;

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
 */
public final class QPropertyAlias<T> extends QAbstractPropertyAlias {
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 * @return new alias
	 */
	public static QPropertyAlias<@QtPrimitiveType Boolean> fromProperty(QBooleanProperty property){
		return new QPropertyAlias<>(property, QBooleanBindable.bindableInterface(0));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 * @return new alias
	 */
	public static QPropertyAlias<@QtPrimitiveType Byte> fromProperty(QByteProperty property){
		return new QPropertyAlias<>(property, QByteBindable.bindableInterface(0));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 * @return new alias
	 */
	public static QPropertyAlias<@QtPrimitiveType Short> fromProperty(QShortProperty property){
		return new QPropertyAlias<>(property, QShortBindable.bindableInterface(0));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 * @return new alias
	 */
	public static QPropertyAlias<@QtPrimitiveType Integer> fromProperty(QIntProperty property){
		return new QPropertyAlias<>(property, QIntBindable.bindableInterface(0));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 * @return new alias
	 */
	public static QPropertyAlias<@QtPrimitiveType Long> fromProperty(QLongProperty property){
		return new QPropertyAlias<>(property, QLongBindable.bindableInterface(0));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 * @return new alias
	 */
	public static QPropertyAlias<@QtPrimitiveType Float> fromProperty(QFloatProperty property){
		return new QPropertyAlias<>(property, QFloatBindable.bindableInterface(0));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 * @return new alias
	 */
	public static QPropertyAlias<@QtPrimitiveType Double> fromProperty(QDoubleProperty property){
		return new QPropertyAlias<>(property, QDoubleBindable.bindableInterface(0));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 * @return new alias
	 */
	public static QPropertyAlias<@QtPrimitiveType Character> fromProperty(QCharProperty property){
		return new QPropertyAlias<>(property, QCharBindable.bindableInterface(0));
	}
	
	/**
	 * Creates a new alias to the provided property alias.
	 * @param alias
	 * @return new alias
	 */
	public static QPropertyAlias<@QtPrimitiveType Boolean> fromAlias(QBooleanPropertyAlias alias){
		return new QPropertyAlias<>(alias);
	}
	
	/**
	 * Creates a new alias to the provided property alias.
	 * @param alias
	 * @return new alias
	 */
	public static QPropertyAlias<@QtPrimitiveType Byte> fromAlias(QBytePropertyAlias alias){
		return new QPropertyAlias<>(alias);
	}
	
	/**
	 * Creates a new alias to the provided property alias.
	 * @param alias
	 * @return new alias
	 */
	public static QPropertyAlias<@QtPrimitiveType Short> fromAlias(QShortPropertyAlias alias){
		return new QPropertyAlias<>(alias);
	}
	
	/**
	 * Creates a new alias to the provided property alias.
	 * @param alias
	 * @return new alias
	 */
	public static QPropertyAlias<@QtPrimitiveType Integer> fromAlias(QIntPropertyAlias alias){
		return new QPropertyAlias<>(alias);
	}
	
	/**
	 * Creates a new alias to the provided property alias.
	 * @param alias
	 * @return new alias
	 */
	public static QPropertyAlias<@QtPrimitiveType Long> fromAlias(QLongPropertyAlias alias){
		return new QPropertyAlias<>(alias);
	}
	
	/**
	 * Creates a new alias to the provided property alias.
	 * @param alias
	 * @return new alias
	 */
	public static QPropertyAlias<@QtPrimitiveType Float> fromAlias(QFloatPropertyAlias alias){
		return new QPropertyAlias<>(alias);
	}
	
	/**
	 * Creates a new alias to the provided property alias.
	 * @param alias
	 * @return new alias
	 */
	public static QPropertyAlias<@QtPrimitiveType Double> fromAlias(QDoublePropertyAlias alias){
		return new QPropertyAlias<>(alias);
	}
	
	/**
	 * Creates a new alias to the provided property alias.
	 * @param alias
	 * @return new alias
	 */
	public static QPropertyAlias<@QtPrimitiveType Character> fromAlias(QCharPropertyAlias alias){
		return new QPropertyAlias<>(alias);
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 * @return new alias
	 */
	public static QPropertyAlias<@QtPrimitiveType Boolean> fromProperty(QObject.QBooleanProperty property){
		return new QPropertyAlias<>(property, QBooleanBindable.bindableInterface(1));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 * @return new alias
	 */
	public static QPropertyAlias<@QtPrimitiveType Byte> fromProperty(QObject.QByteProperty property){
		return new QPropertyAlias<>(property, QByteBindable.bindableInterface(1));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 * @return new alias
	 */
	public static QPropertyAlias<@QtPrimitiveType Short> fromProperty(QObject.QShortProperty property){
		return new QPropertyAlias<>(property, QShortBindable.bindableInterface(1));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 * @return new alias
	 */
	public static QPropertyAlias<@QtPrimitiveType Integer> fromProperty(QObject.QIntProperty property){
		return new QPropertyAlias<>(property, QIntBindable.bindableInterface(1));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 * @return new alias
	 */
	public static QPropertyAlias<@QtPrimitiveType Long> fromProperty(QObject.QLongProperty property){
		return new QPropertyAlias<>(property, QLongBindable.bindableInterface(1));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 * @return new alias
	 */
	public static QPropertyAlias<@QtPrimitiveType Float> fromProperty(QObject.QFloatProperty property){
		return new QPropertyAlias<>(property, QFloatBindable.bindableInterface(1));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 * @return new alias
	 */
	public static QPropertyAlias<@QtPrimitiveType Double> fromProperty(QObject.QDoubleProperty property){
		return new QPropertyAlias<>(property, QDoubleBindable.bindableInterface(1));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 * @return new alias
	 */
	public static QPropertyAlias<@QtPrimitiveType Character> fromProperty(QObject.QCharProperty property){
		return new QPropertyAlias<>(property, QCharBindable.bindableInterface(1));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 * @return new alias
	 */
	public static QPropertyAlias<@QtPrimitiveType Boolean> fromProperty(QObject.QComputedBooleanProperty property){
		return new QPropertyAlias<>(property, QBooleanBindable.bindableInterface(2));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 * @return new alias
	 */
	public static QPropertyAlias<@QtPrimitiveType Byte> fromProperty(QObject.QComputedByteProperty property){
		return new QPropertyAlias<>(property, QByteBindable.bindableInterface(2));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 * @return new alias
	 */
	public static QPropertyAlias<@QtPrimitiveType Short> fromProperty(QObject.QComputedShortProperty property){
		return new QPropertyAlias<>(property, QShortBindable.bindableInterface(2));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 * @return new alias
	 */
	public static QPropertyAlias<@QtPrimitiveType Integer> fromProperty(QObject.QComputedIntProperty property){
		return new QPropertyAlias<>(property, QIntBindable.bindableInterface(2));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 * @return new alias
	 */
	public static QPropertyAlias<@QtPrimitiveType Long> fromProperty(QObject.QComputedLongProperty property){
		return new QPropertyAlias<>(property, QLongBindable.bindableInterface(2));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 * @return new alias
	 */
	public static QPropertyAlias<@QtPrimitiveType Float> fromProperty(QObject.QComputedFloatProperty property){
		return new QPropertyAlias<>(property, QFloatBindable.bindableInterface(2));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 * @return new alias
	 */
	public static QPropertyAlias<@QtPrimitiveType Double> fromProperty(QObject.QComputedDoubleProperty property){
		return new QPropertyAlias<>(property, QDoubleBindable.bindableInterface(2));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 * @return new alias
	 */
	public static QPropertyAlias<@QtPrimitiveType Character> fromProperty(QObject.QComputedCharProperty property){
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
	public QPropertyAlias(QProperty<T> property) {
		super(property, QBindable.bindableInterface(property.valueMetaType().id(), 0));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 */
	public QPropertyAlias(QObject.QProperty<T> property) {
		super(property, QBindable.bindableInterface(property.valueMetaType().id(), 1));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 */
	public QPropertyAlias(QObject.QComputedProperty<T> property) {
		super(property, QBindable.bindableInterface(property.valueMetaType().id(), 2));
	}
	
	/**
	 * Creates a new alias to the provided alias.
	 * @param other
	 */
	public QPropertyAlias(QPropertyAlias<T> alias) {
		super(alias);
	}
	
	public QPropertyAlias(QBindable<T> bindable) {
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
	public QPropertyBinding<T> setBinding(QPropertyBinding<T> newBinding) {
		return new QBindable<T>(aliasedProperty(), iface).setBinding(newBinding);
    }
	
	/**
	 * @see QProperty#setBinding(io.qt.QtUtilities.Supplier)
	 */
	@QtUninvokable
	public QPropertyBinding<T> setBinding(QtUtilities.Supplier<T> functor) {
        return setBinding(new QPropertyBinding<>(functor));
    }
	
	/**
	 * @see QProperty#binding()
	 */
	@QtUninvokable
	public QPropertyBinding<T> binding() {
        return new QBindable<T>(aliasedProperty(), iface).binding();
    }
	
	/**
	 * @see QProperty#takeBinding()
	 */
	@QtUninvokable
	public QPropertyBinding<T> takeBinding() {
        return new QBindable<T>(aliasedProperty(), iface).takeBinding();
    }
}
