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
 * QDoublePropertyAlias is primitive-typed version of QPropertyAlias&lt;Double&gt;.
 * @see QPropertyAlias
 * @deprecated Class was only meant for internal use, use a <code>QProperty</code> and add a binding to the target
 */
@Deprecated(forRemoval=true, since="Qt 6.6")
@SuppressWarnings("removal")
public final class QDoublePropertyAlias extends QAbstractPropertyAlias {
	
	private static QPropertyAlias<@NonNull@QtPrimitiveType Double> check(QPropertyAlias<@NonNull@QtPrimitiveType Double> other){
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
	public QDoublePropertyAlias(@StrictNonNull QPropertyAlias<@NonNull@QtPrimitiveType Double> other) {
		super(check(other));
	}

	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 */
	public QDoublePropertyAlias(@StrictNonNull QDoubleProperty property) {
		super(property, QDoubleBindable.bindableInterface(0));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 */
	public QDoublePropertyAlias(QObject.@StrictNonNull QDoubleProperty property) {
		super(property, QDoubleBindable.bindableInterface(1));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 */
	public QDoublePropertyAlias(QObject.@StrictNonNull QComputedIntProperty property) {
		super(property, QDoubleBindable.bindableInterface(2));
	}

	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 */
	public QDoublePropertyAlias(@StrictNonNull QProperty<@NonNull@QtPrimitiveType Double> property) {
		super(property, QBindable.bindableInterface(check(property.valueMetaType()).id(), 0));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 */
	public QDoublePropertyAlias(QObject.@StrictNonNull QProperty<@NonNull@QtPrimitiveType Double> property) {
		super(property, QBindable.bindableInterface(check(property.valueMetaType()).id(), 1));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 */
	public QDoublePropertyAlias(QObject.@StrictNonNull QComputedProperty<@NonNull@QtPrimitiveType Double> property) {
		super(property, QBindable.bindableInterface(check(property.valueMetaType()).id(), 2));
	}
	
	/**
	 * Creates a new alias to the provided alias.
	 * @param other
	 */
	public QDoublePropertyAlias(@StrictNonNull QDoublePropertyAlias other) {
		super(other);
	}
	
	/**
	 * Creates a new alias to the provided binding's underlying property.
	 * @param bindable
	 */
	public QDoublePropertyAlias(@StrictNonNull QDoubleBindable bindable) {
		super(bindable.data(), bindable.iface());
	}
	
	/**
	 * Creates a new alias to the provided binding's underlying property.
	 * @param bindable
	 */
	public QDoublePropertyAlias(@StrictNonNull QBindable<@NonNull@QtPrimitiveType Double> bindable) {
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
	public @NonNull QDoublePropertyBinding setBinding(@StrictNonNull QDoublePropertyBinding newBinding) {
		return new QDoubleBindable(aliasedProperty(), iface).setBinding(newBinding);
    }
	
	/**
	 * @see QDoubleProperty#setBinding(QPropertyBinding)
	 */
	@QtUninvokable
	public @NonNull QDoublePropertyBinding setBinding(@StrictNonNull QPropertyBinding<@NonNull@QtPrimitiveType Double> newBinding) {
		return new QDoubleBindable(aliasedProperty(), iface).setBinding(newBinding);
    }
	
	/**
	 * @see QDoubleProperty#setBinding(java.util.function.DoubleSupplier)
	 */
	@QtUninvokable
	public @NonNull QDoublePropertyBinding setBinding(java.util.function.@StrictNonNull DoubleSupplier functor) {
        return setBinding(new QDoublePropertyBinding(functor));
    }
	
	/**
	 * @see QDoubleProperty#binding()
	 */
	@QtUninvokable
	public @NonNull QDoublePropertyBinding binding() {
        return new QDoubleBindable(aliasedProperty(), iface).binding();
    }
	
	/**
	 * @see QDoubleProperty#takeBinding()
	 */
	@QtUninvokable
	public @NonNull QDoublePropertyBinding takeBinding() {
        return new QDoubleBindable(aliasedProperty(), iface).takeBinding();
    }
}
