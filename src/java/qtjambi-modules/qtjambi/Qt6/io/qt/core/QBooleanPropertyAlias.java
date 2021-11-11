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

import io.qt.QtPrimitiveType;
import io.qt.QtUninvokable;

/**
 * <code>QBooleanPropertyAlias</code> is primitive-typed version of <code>QPropertyAlias&lt;Boolean&gt;</code>.
 * @see QPropertyAlias
 */
public final class QBooleanPropertyAlias extends QAbstractPropertyAlias {
	
	private static QPropertyAlias<@QtPrimitiveType Boolean> check(QPropertyAlias<@QtPrimitiveType Boolean> other){
		if(other.iface!=null && !QBooleanProperty.checkType(other.iface.metaType())) {
			throw new IllegalArgumentException("Given QPropertyAlias is not of boolean type.");
		}
		return other;
	}
	
	private static QMetaType check(QMetaType metaType){
		if(!QBooleanProperty.checkType(metaType)) {
			throw new IllegalArgumentException("Given QProperty is not of boolean type.");
		}
		return metaType;
	}
	
	/**
	 * Creates a new alias to the provided alias.
	 * @param other
	 */
	public QBooleanPropertyAlias(QPropertyAlias<@QtPrimitiveType Boolean> other) {
		super(check(other));
	}

	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 */
	public QBooleanPropertyAlias(QBooleanProperty property) {
		super(property, QBooleanBindable.bindableInterface(0));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 */
	public QBooleanPropertyAlias(QObject.QBooleanProperty property) {
		super(property, QBooleanBindable.bindableInterface(1));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 */
	public QBooleanPropertyAlias(QObject.QComputedBooleanProperty property) {
		super(property, QBooleanBindable.bindableInterface(2));
	}

	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 */
	public QBooleanPropertyAlias(QProperty<@QtPrimitiveType Boolean> property) {
		super(property, QBindable.bindableInterface(check(property.valueMetaType()).id(), 0));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 */
	public QBooleanPropertyAlias(QObject.QProperty<@QtPrimitiveType Boolean> property) {
		super(property, QBindable.bindableInterface(check(property.valueMetaType()).id(), 1));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 */
	public QBooleanPropertyAlias(QObject.QComputedProperty<@QtPrimitiveType Boolean> property) {
		super(property, QBindable.bindableInterface(check(property.valueMetaType()).id(), 2));
	}
	
	/**
	 * Creates a new alias to the provided alias.
	 * @param other
	 */
	public QBooleanPropertyAlias(QBooleanPropertyAlias alias) {
		super(alias);
	}
	
	/**
	 * Creates a new alias to the provided binding's underlying property.
	 * @param other
	 */
	public QBooleanPropertyAlias(QBooleanBindable bindable) {
		super(bindable.data(), bindable.iface());
	}
	
	/**
	 * Creates a new alias to the provided binding's underlying property.
	 * @param other
	 */
	public QBooleanPropertyAlias(QBindable<@QtPrimitiveType Boolean> bindable) {
		super(bindable.data(), bindable.iface());
		check(bindable.valueMetaType());
	}
	
	/**
	 * @see QBooleanProperty#value()
	 */
	@QtUninvokable
	public boolean value() {
        return QBooleanBindable.value(QtJambi_LibraryUtilities.internal.nativeId(this.iface), QtJambi_LibraryUtilities.internal.nativeId(this.aliasedProperty()));
    }
	
	/**
	 * @see QBooleanProperty#setValue(boolean)
	 */
	@QtUninvokable
	public void setValue(boolean newValue) {
		QBooleanBindable.setValue(QtJambi_LibraryUtilities.internal.nativeId(this.iface), QtJambi_LibraryUtilities.internal.nativeId(this.aliasedProperty()), newValue);
    }
	
	/**
	 * @see QBooleanProperty#setBinding(QBooleanPropertyBinding)
	 */
	@QtUninvokable
	public QBooleanPropertyBinding setBinding(QBooleanPropertyBinding newBinding) {
		return new QBooleanBindable(aliasedProperty(), iface).setBinding(newBinding);
    }
	
	/**
	 * @see QBooleanProperty#setBinding(QPropertyBinding)
	 */
	@QtUninvokable
	public QBooleanPropertyBinding setBinding(QPropertyBinding<@QtPrimitiveType Boolean> newBinding) {
		return new QBooleanBindable(aliasedProperty(), iface).setBinding(newBinding);
    }
	
	/**
	 * @see QBooleanProperty#setBinding(java.util.function.BooleanSupplier)
	 */
	@QtUninvokable
	public QBooleanPropertyBinding setBinding(java.util.function.BooleanSupplier functor) {
        return setBinding(new QBooleanPropertyBinding(functor));
    }
	
	/**
	 * @see QBooleanProperty#binding()
	 */
	@QtUninvokable
	public QBooleanPropertyBinding binding() {
        return new QBooleanBindable(aliasedProperty(), iface).binding();
    }
	
	/**
	 * @see QBooleanProperty#takeBinding()
	 */
	@QtUninvokable
	public QBooleanPropertyBinding takeBinding() {
        return new QBooleanBindable(aliasedProperty(), iface).takeBinding();
    }
}
