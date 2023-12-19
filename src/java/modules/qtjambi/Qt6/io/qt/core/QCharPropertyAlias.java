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

import io.qt.*;

/**
 * QCharPropertyAlias is primitive-typed version of QPropertyAlias&lt;Character&gt;.
 * @see QPropertyAlias
 */
public final class QCharPropertyAlias extends QAbstractPropertyAlias {
	
	private static QPropertyAlias<@NonNull@QtPrimitiveType Character> check(QPropertyAlias<@NonNull@QtPrimitiveType Character> other){
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
	public QCharPropertyAlias(@StrictNonNull QPropertyAlias<@NonNull@QtPrimitiveType Character> other) {
		super(check(other));
	}

	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 */
	public QCharPropertyAlias(@StrictNonNull QCharProperty property) {
		super(property, QCharBindable.bindableInterface(0));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 */
	public QCharPropertyAlias(QObject.@StrictNonNull QCharProperty property) {
		super(property, QCharBindable.bindableInterface(1));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 */
	public QCharPropertyAlias(QObject.@StrictNonNull QComputedCharProperty property) {
		super(property, QCharBindable.bindableInterface(2));
	}

	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 */
	public QCharPropertyAlias(@StrictNonNull QProperty<@NonNull@QtPrimitiveType Character> property) {
		super(property, QBindable.bindableInterface(check(property.valueMetaType()).id(), 0));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 */
	public QCharPropertyAlias(QObject.@StrictNonNull QProperty<@NonNull@QtPrimitiveType Character> property) {
		super(property, QBindable.bindableInterface(check(property.valueMetaType()).id(), 1));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 */
	public QCharPropertyAlias(QObject.@StrictNonNull QComputedProperty<@NonNull@QtPrimitiveType Character> property) {
		super(property, QBindable.bindableInterface(check(property.valueMetaType()).id(), 2));
	}
	
	/**
	 * Creates a new alias to the provided alias.
	 * @param other
	 */
	public QCharPropertyAlias(@StrictNonNull QCharPropertyAlias other) {
		super(other);
	}
	
	/**
	 * Creates a new alias to the provided binding's underlying property.
	 * @param bindable
	 */
	public QCharPropertyAlias(@StrictNonNull QCharBindable bindable) {
		super(bindable.data(), bindable.iface());
	}
	
	/**
	 * Creates a new alias to the provided binding's underlying property.
	 * @param bindable
	 */
	public QCharPropertyAlias(@StrictNonNull QBindable<@NonNull@QtPrimitiveType Character> bindable) {
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
	public @NonNull QCharPropertyBinding setBinding(@StrictNonNull QCharPropertyBinding newBinding) {
		return new QCharBindable(aliasedProperty(), iface).setBinding(newBinding);
    }
	
	/**
	 * @see QCharProperty#setBinding(QPropertyBinding)
	 */
	@QtUninvokable
	public @NonNull QCharPropertyBinding setBinding(@StrictNonNull QPropertyBinding<@NonNull@QtPrimitiveType Character> newBinding) {
		return new QCharBindable(aliasedProperty(), iface).setBinding(newBinding);
    }
	
	/**
	 * @see QCharProperty#setBinding(io.qt.QtUtilities.CharSupplier)
	 */
	@QtUninvokable
	public @NonNull QCharPropertyBinding setBinding(io.qt.QtUtilities.@StrictNonNull CharSupplier functor) {
        return setBinding(new QCharPropertyBinding(functor));
    }
	
	/**
	 * @see QCharProperty#binding()
	 */
	@QtUninvokable
	public @NonNull QCharPropertyBinding binding() {
        return new QCharBindable(aliasedProperty(), iface).binding();
    }
	
	/**
	 * @see QCharProperty#takeBinding()
	 */
	@QtUninvokable
	public @NonNull QCharPropertyBinding takeBinding() {
        return new QCharBindable(aliasedProperty(), iface).takeBinding();
    }
}
