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
import io.qt.QtUtilities.ShortSupplier;

/**
 * QShortPropertyAlias is primitive-typed version of QPropertyAlias&lt;Short&gt;.
 * @see QPropertyAlias
 */
public final class QShortPropertyAlias extends QAbstractPropertyAlias {
	
	private static QPropertyAlias<@QtPrimitiveType Short> check(QPropertyAlias<@QtPrimitiveType Short> other){
		if(other.iface!=null && !QShortProperty.checkType(other.iface.metaType())) {
			throw new IllegalArgumentException("Given QPropertyAlias is not of short type.");
		}
		return other;
	}
	
	private static QMetaType check(QMetaType metaType){
		if(!QShortProperty.checkType(metaType)) {
			throw new IllegalArgumentException("Given QProperty is not of short type.");
		}
		return metaType;
	}
	
	/**
	 * Creates a new alias to the provided alias.
	 * @param other
	 */
	public QShortPropertyAlias(QPropertyAlias<@QtPrimitiveType Short> other) {
		super(check(other));
	}

	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 */
	public QShortPropertyAlias(QShortProperty property) {
		super(property, QShortBindable.bindableInterface(0));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 */
	public QShortPropertyAlias(QObject.QShortProperty property) {
		super(property, QShortBindable.bindableInterface(1));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 */
	public QShortPropertyAlias(QObject.QComputedShortProperty property) {
		super(property, QShortBindable.bindableInterface(2));
	}

	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 */
	public QShortPropertyAlias(QProperty<@QtPrimitiveType Short> property) {
		super(property, QBindable.bindableInterface(check(property.valueMetaType()).id(), 0));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 */
	public QShortPropertyAlias(QObject.QProperty<@QtPrimitiveType Short> property) {
		super(property, QBindable.bindableInterface(check(property.valueMetaType()).id(), 1));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 */
	public QShortPropertyAlias(QObject.QComputedProperty<@QtPrimitiveType Short> property) {
		super(property, QBindable.bindableInterface(check(property.valueMetaType()).id(), 2));
	}
	
	/**
	 * Creates a new alias to the provided alias.
	 * @param other
	 */
	public QShortPropertyAlias(QShortPropertyAlias alias) {
		super(alias);
	}
	
	/**
	 * Creates a new alias to the provided binding's underlying property.
	 * @param other
	 */
	public QShortPropertyAlias(QShortBindable bindable) {
		super(bindable.data(), bindable.iface());
	}
	
	/**
	 * Creates a new alias to the provided binding's underlying property.
	 * @param other
	 */
	public QShortPropertyAlias(QBindable<@QtPrimitiveType Short> bindable) {
		super(bindable.data(), bindable.iface());
		check(bindable.valueMetaType());
	}
	
	/**
	 * @see QShortProperty#value()
	 */
	@QtUninvokable
	public short value() {
        return QShortBindable.value(QtJambi_LibraryUtilities.internal.nativeId(this.iface), QtJambi_LibraryUtilities.internal.nativeId(this.aliasedProperty()));
    }
	
	/**
	 * @see QShortProperty#setValue(short)
	 */
	@QtUninvokable
	public void setValue(short newValue) {
		QShortBindable.setValue(QtJambi_LibraryUtilities.internal.nativeId(this.iface), QtJambi_LibraryUtilities.internal.nativeId(this.aliasedProperty()), newValue);
    }
	
	/**
	 * @see QShortProperty#setBinding(QShortPropertyBinding)
	 */
	@QtUninvokable
	public QShortPropertyBinding setBinding(QShortPropertyBinding newBinding) {
		return new QShortBindable(aliasedProperty(), iface).setBinding(newBinding);
    }
	
	/**
	 * @see QShortProperty#setBinding(QPropertyBinding)
	 */
	@QtUninvokable
	public QShortPropertyBinding setBinding(QPropertyBinding<@QtPrimitiveType Short> newBinding) {
		return new QShortBindable(aliasedProperty(), iface).setBinding(newBinding);
    }
	
	/**
	 * @see QShortProperty#setBinding(ShortSupplier)
	 */
	@QtUninvokable
	public QShortPropertyBinding setBinding(ShortSupplier functor) {
        return setBinding(new QShortPropertyBinding(functor));
    }
	
	/**
	 * @see QShortProperty#binding()
	 */
	@QtUninvokable
	public QShortPropertyBinding binding() {
        return new QShortBindable(aliasedProperty(), iface).binding();
    }
	
	/**
	 * @see QShortProperty#takeBinding()
	 */
	@QtUninvokable
	public QShortPropertyBinding takeBinding() {
        return new QShortBindable(aliasedProperty(), iface).takeBinding();
    }
}
