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
 * QCharPropertyAlias is primitive-typed version of QPropertyAlias&lt;Character&gt;.
 * @see QPropertyAlias
 */
public final class QCharPropertyAlias extends QAbstractPropertyAlias {
	
	private static QPropertyAlias<@QtPrimitiveType Character> check(QPropertyAlias<@QtPrimitiveType Character> other){
		if(other.iface!=null && !QCharProperty.checkType(other.iface.metaType())) {
			throw new IllegalArgumentException("Given QPropertyAlias is not of char type.");
		}
		return other;
	}
	
	private static QMetaType check(QMetaType metaType){
		if(!QCharProperty.checkType(metaType)) {
			throw new IllegalArgumentException("Given QProperty is not of char type.");
		}
		return metaType;
	}
	
	/**
	 * Creates a new alias to the provided alias.
	 * @param other
	 */
	public QCharPropertyAlias(QPropertyAlias<@QtPrimitiveType Character> other) {
		super(check(other));
	}

	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 */
	public QCharPropertyAlias(QCharProperty property) {
		super(property, QCharBindable.bindableInterface(0));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 */
	public QCharPropertyAlias(QObject.QCharProperty property) {
		super(property, QCharBindable.bindableInterface(1));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 */
	public QCharPropertyAlias(QObject.QComputedCharProperty property) {
		super(property, QCharBindable.bindableInterface(2));
	}

	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 */
	public QCharPropertyAlias(QProperty<@QtPrimitiveType Character> property) {
		super(property, QBindable.bindableInterface(check(property.valueMetaType()).id(), 0));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 */
	public QCharPropertyAlias(QObject.QProperty<@QtPrimitiveType Character> property) {
		super(property, QBindable.bindableInterface(check(property.valueMetaType()).id(), 1));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 */
	public QCharPropertyAlias(QObject.QComputedProperty<@QtPrimitiveType Character> property) {
		super(property, QBindable.bindableInterface(check(property.valueMetaType()).id(), 2));
	}
	
	/**
	 * Creates a new alias to the provided alias.
	 * @param other
	 */
	public QCharPropertyAlias(QCharPropertyAlias alias) {
		super(alias);
	}
	
	/**
	 * Creates a new alias to the provided binding's underlying property.
	 * @param other
	 */
	public QCharPropertyAlias(QCharBindable bindable) {
		super(bindable.data(), bindable.iface());
	}
	
	/**
	 * Creates a new alias to the provided binding's underlying property.
	 * @param other
	 */
	public QCharPropertyAlias(QBindable<@QtPrimitiveType Character> bindable) {
		super(bindable.data(), bindable.iface());
		check(bindable.valueMetaType());
	}
	
	/**
	 * @see QCharProperty#value()
	 */
	@QtUninvokable
	public char value() {
        return QCharBindable.value(QtJambi_LibraryUtilities.internal.nativeId(this.iface), QtJambi_LibraryUtilities.internal.nativeId(this.aliasedProperty()));
    }
	
	/**
	 * @see QCharProperty#setValue(char)
	 */
	@QtUninvokable
	public void setValue(char newValue) {
		QCharBindable.setValue(QtJambi_LibraryUtilities.internal.nativeId(this.iface), QtJambi_LibraryUtilities.internal.nativeId(this.aliasedProperty()), newValue);
    }
	
	/**
	 * @see QCharProperty#setBinding(QCharPropertyBinding)
	 */
	@QtUninvokable
	public QCharPropertyBinding setBinding(QCharPropertyBinding newBinding) {
		return new QCharBindable(aliasedProperty(), iface).setBinding(newBinding);
    }
	
	/**
	 * @see QCharProperty#setBinding(QPropertyBinding)
	 */
	@QtUninvokable
	public QCharPropertyBinding setBinding(QPropertyBinding<@QtPrimitiveType Character> newBinding) {
		return new QCharBindable(aliasedProperty(), iface).setBinding(newBinding);
    }
	
	/**
	 * @see QCharProperty#setBinding(io.qt.QtUtilities.CharSupplier)
	 */
	@QtUninvokable
	public QCharPropertyBinding setBinding(io.qt.QtUtilities.CharSupplier functor) {
        return setBinding(new QCharPropertyBinding(functor));
    }
	
	/**
	 * @see QCharProperty#binding()
	 */
	@QtUninvokable
	public QCharPropertyBinding binding() {
        return new QCharBindable(aliasedProperty(), iface).binding();
    }
	
	/**
	 * @see QCharProperty#takeBinding()
	 */
	@QtUninvokable
	public QCharPropertyBinding takeBinding() {
        return new QCharBindable(aliasedProperty(), iface).takeBinding();
    }
}
