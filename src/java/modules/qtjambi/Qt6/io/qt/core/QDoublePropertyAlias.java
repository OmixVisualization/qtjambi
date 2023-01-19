/****************************************************************************
**
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

/**
 * QDoublePropertyAlias is primitive-typed version of QPropertyAlias&lt;Double&gt;.
 * @see QPropertyAlias
 */
public final class QDoublePropertyAlias extends QAbstractPropertyAlias {
	
	private static QPropertyAlias<@QtPrimitiveType Double> check(QPropertyAlias<@QtPrimitiveType Double> other){
		if(other.iface!=null && !QDoubleProperty.checkType(other.iface.metaType())) {
			throw new IllegalArgumentException("Given QPropertyAlias is not of double type.");
		}
		return other;
	}
	
	private static QMetaType check(QMetaType metaType){
		if(!QDoubleProperty.checkType(metaType)) {
			throw new IllegalArgumentException("Given QProperty is not of double type.");
		}
		return metaType;
	}
	
	/**
	 * Creates a new alias to the provided alias.
	 * @param other
	 */
	public QDoublePropertyAlias(QPropertyAlias<@QtPrimitiveType Double> other) {
		super(check(other));
	}

	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 */
	public QDoublePropertyAlias(QDoubleProperty property) {
		super(property, QDoubleBindable.bindableInterface(0));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 */
	public QDoublePropertyAlias(QObject.QDoubleProperty property) {
		super(property, QDoubleBindable.bindableInterface(1));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 */
	public QDoublePropertyAlias(QObject.QComputedIntProperty property) {
		super(property, QDoubleBindable.bindableInterface(2));
	}

	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 */
	public QDoublePropertyAlias(QProperty<@QtPrimitiveType Double> property) {
		super(property, QBindable.bindableInterface(check(property.valueMetaType()).id(), 0));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 */
	public QDoublePropertyAlias(QObject.QProperty<@QtPrimitiveType Double> property) {
		super(property, QBindable.bindableInterface(check(property.valueMetaType()).id(), 1));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 */
	public QDoublePropertyAlias(QObject.QComputedProperty<@QtPrimitiveType Double> property) {
		super(property, QBindable.bindableInterface(check(property.valueMetaType()).id(), 2));
	}
	
	/**
	 * Creates a new alias to the provided alias.
	 * @param other
	 */
	public QDoublePropertyAlias(QDoublePropertyAlias alias) {
		super(alias);
	}
	
	/**
	 * Creates a new alias to the provided binding's underlying property.
	 * @param other
	 */
	public QDoublePropertyAlias(QDoubleBindable bindable) {
		super(bindable.data(), bindable.iface());
	}
	
	/**
	 * Creates a new alias to the provided binding's underlying property.
	 * @param other
	 */
	public QDoublePropertyAlias(QBindable<@QtPrimitiveType Double> bindable) {
		super(bindable.data(), bindable.iface());
		check(bindable.valueMetaType());
	}
	
	/**
	 * @see QDoubleProperty#value()
	 */
	@QtUninvokable
	public double value() {
        return QDoubleBindable.value(QtJambi_LibraryUtilities.internal.nativeId(this.iface), QtJambi_LibraryUtilities.internal.nativeId(this.aliasedProperty()));
    }
	
	/**
	 * @see QDoubleProperty#setValue(double)
	 */
	@QtUninvokable
	public void setValue(double newValue) {
		QDoubleBindable.setValue(QtJambi_LibraryUtilities.internal.nativeId(this.iface), QtJambi_LibraryUtilities.internal.nativeId(this.aliasedProperty()), newValue);
    }
	
	/**
	 * @see QDoubleProperty#setBinding(QDoublePropertyBinding)
	 */
	@QtUninvokable
	public QDoublePropertyBinding setBinding(QDoublePropertyBinding newBinding) {
		return new QDoubleBindable(aliasedProperty(), iface).setBinding(newBinding);
    }
	
	/**
	 * @see QDoubleProperty#setBinding(QPropertyBinding)
	 */
	@QtUninvokable
	public QDoublePropertyBinding setBinding(QPropertyBinding<@QtPrimitiveType Double> newBinding) {
		return new QDoubleBindable(aliasedProperty(), iface).setBinding(newBinding);
    }
	
	/**
	 * @see QDoubleProperty#setBinding(java.util.function.DoubleSupplier)
	 */
	@QtUninvokable
	public QDoublePropertyBinding setBinding(java.util.function.DoubleSupplier functor) {
        return setBinding(new QDoublePropertyBinding(functor));
    }
	
	/**
	 * @see QDoubleProperty#binding()
	 */
	@QtUninvokable
	public QDoublePropertyBinding binding() {
        return new QDoubleBindable(aliasedProperty(), iface).binding();
    }
	
	/**
	 * @see QDoubleProperty#takeBinding()
	 */
	@QtUninvokable
	public QDoublePropertyBinding takeBinding() {
        return new QDoubleBindable(aliasedProperty(), iface).takeBinding();
    }
}
