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

import static io.qt.internal.QtJambiInternal.nativeId;

import io.qt.QtPrimitiveType;
import io.qt.QtUninvokable;

/**
 * QIntPropertyAlias is primitive-typed version of QPropertyAlias&lt;Integer&gt;.
 * @see QPropertyAlias
 */
public final class QIntPropertyAlias extends QAbstractPropertyAlias {
	
	private static QPropertyAlias<@QtPrimitiveType Integer> check(QPropertyAlias<@QtPrimitiveType Integer> other){
		if(other.iface!=null && !QIntProperty.checkType(other.iface.metaType())) {
			throw new IllegalArgumentException("Given QPropertyAlias is not of int type.");
		}
		return other;
	}
	
	private static QMetaType check(QMetaType metaType){
		if(!QIntProperty.checkType(metaType)) {
			throw new IllegalArgumentException("Given QProperty is not of int type.");
		}
		return metaType;
	}
	
	/**
	 * Creates a new alias to the provided alias.
	 * @param other
	 */
	public QIntPropertyAlias(QPropertyAlias<@QtPrimitiveType Integer> other) {
		super(check(other));
	}

	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 */
	public QIntPropertyAlias(QIntProperty property) {
		super(property, QIntBindable.bindableInterface(0));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 */
	public QIntPropertyAlias(QObject.QIntProperty property) {
		super(property, QIntBindable.bindableInterface(1));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 */
	public QIntPropertyAlias(QObject.QComputedIntProperty property) {
		super(property, QIntBindable.bindableInterface(2));
	}

	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 */
	public QIntPropertyAlias(QProperty<@QtPrimitiveType Integer> property) {
		super(property, QBindable.bindableInterface(check(property.valueMetaType()).id(), 0));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 */
	public QIntPropertyAlias(QObject.QProperty<@QtPrimitiveType Integer> property) {
		super(property, QBindable.bindableInterface(check(property.valueMetaType()).id(), 1));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 */
	public QIntPropertyAlias(QObject.QComputedProperty<@QtPrimitiveType Integer> property) {
		super(property, QBindable.bindableInterface(check(property.valueMetaType()).id(), 2));
	}
	
	/**
	 * Creates a new alias to the provided alias.
	 * @param other
	 */
	public QIntPropertyAlias(QIntPropertyAlias alias) {
		super(alias);
	}
	
	/**
	 * Creates a new alias to the provided binding's underlying property.
	 * @param other
	 */
	public QIntPropertyAlias(QIntBindable bindable) {
		super(bindable.data(), bindable.iface());
	}
	
	/**
	 * Creates a new alias to the provided binding's underlying property.
	 * @param other
	 */
	public QIntPropertyAlias(QBindable<@QtPrimitiveType Integer> bindable) {
		super(bindable.data(), bindable.iface());
		check(bindable.valueMetaType());
	}
	
	/**
	 * @see QIntProperty#value()
	 */
	@QtUninvokable
	public int value() {
        return QIntBindable.value(nativeId(this.iface), nativeId(this.aliasedProperty()));
    }
	
	/**
	 * @see QIntProperty#setValue(int)
	 */
	@QtUninvokable
	public void setValue(int newValue) {
		QIntBindable.setValue(nativeId(this.iface), nativeId(this.aliasedProperty()), newValue);
    }
	
	/**
	 * @see QIntProperty#setBinding(QIntPropertyBinding)
	 */
	@QtUninvokable
	public QIntPropertyBinding setBinding(QIntPropertyBinding newBinding) {
		return new QIntBindable(aliasedProperty(), iface).setBinding(newBinding);
    }
	
	/**
	 * @see QIntProperty#setBinding(QPropertyBinding)
	 */
	@QtUninvokable
	public QIntPropertyBinding setBinding(QPropertyBinding<@QtPrimitiveType Integer> newBinding) {
		return new QIntBindable(aliasedProperty(), iface).setBinding(newBinding);
    }
	
	/**
	 * @see QIntProperty#setBinding(java.util.function.IntSupplier)
	 */
	@QtUninvokable
	public QIntPropertyBinding setBinding(java.util.function.IntSupplier functor) {
        return setBinding(new QIntPropertyBinding(functor));
    }
	
	/**
	 * @see QIntProperty#binding()
	 */
	@QtUninvokable
	public QIntPropertyBinding binding() {
        return new QIntBindable(aliasedProperty(), iface).binding();
    }
	
	/**
	 * @see QIntProperty#takeBinding()
	 */
	@QtUninvokable
	public QIntPropertyBinding takeBinding() {
        return new QIntBindable(aliasedProperty(), iface).takeBinding();
    }
}
