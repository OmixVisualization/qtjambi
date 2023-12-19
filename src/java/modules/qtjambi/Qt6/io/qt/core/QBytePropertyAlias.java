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
 * QBytePropertyAlias is primitive-typed version of QPropertyAlias&lt;Byte&gt;.
 * @see QPropertyAlias
 */
public final class QBytePropertyAlias extends QAbstractPropertyAlias {
	
	private static QPropertyAlias<@NonNull@QtPrimitiveType Byte> check(QPropertyAlias<@NonNull@QtPrimitiveType Byte> other){
		if(other.iface!=null && !QByteProperty.checkType(other.iface.metaType())) {
			throw new IllegalArgumentException("Given QPropertyAlias is not of byte type.");
		}
		return other;
	}
	
	private static QMetaType check(QMetaType metaType){
		if(!QByteProperty.checkType(metaType)) {
			throw new IllegalArgumentException("Given QProperty is not of byte type.");
		}
		return metaType;
	}
	
	/**
	 * Creates a new alias to the provided alias.
	 * @param other
	 */
	public QBytePropertyAlias(@StrictNonNull QPropertyAlias<@NonNull@QtPrimitiveType Byte> other) {
		super(check(other));
	}

	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 */
	public QBytePropertyAlias(@StrictNonNull QByteProperty property) {
		super(property, QByteBindable.bindableInterface(0));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 */
	public QBytePropertyAlias(QObject.@StrictNonNull QByteProperty property) {
		super(property, QByteBindable.bindableInterface(1));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 */
	public QBytePropertyAlias(QObject.@StrictNonNull QComputedByteProperty property) {
		super(property, QByteBindable.bindableInterface(2));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 */
	public QBytePropertyAlias(@StrictNonNull QProperty<@NonNull@QtPrimitiveType Byte> property) {
		super(property, QBindable.bindableInterface(check(property.valueMetaType()).id(), 0));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 */
	public QBytePropertyAlias(QObject.@StrictNonNull QProperty<@NonNull@QtPrimitiveType Byte> property) {
		super(property, QBindable.bindableInterface(check(property.valueMetaType()).id(), 1));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 */
	public QBytePropertyAlias(QObject.@StrictNonNull QComputedProperty<@NonNull@QtPrimitiveType Byte> property) {
		super(property, QBindable.bindableInterface(check(property.valueMetaType()).id(), 2));
	}
	
	/**
	 * Creates a new alias to the provided alias.
	 * @param other
	 */
	public QBytePropertyAlias(@StrictNonNull QBytePropertyAlias other) {
		super(other);
	}
	
	/**
	 * Creates a new alias to the provided binding's underlying property.
	 * @param bindable
	 */
	public QBytePropertyAlias(@StrictNonNull QByteBindable bindable) {
		super(bindable.data(), bindable.iface());
	}
	
	/**
	 * Creates a new alias to the provided binding's underlying property.
	 * @param bindable
	 */
	public QBytePropertyAlias(@StrictNonNull QBindable<@NonNull@QtPrimitiveType Byte> bindable) {
		super(bindable.data(), bindable.iface());
		check(bindable.valueMetaType());
	}
	
	/**
	 * @see QByteProperty#value()
	 */
	@QtUninvokable
	public byte value() {
        return QByteBindable.value(QtJambi_LibraryUtilities.internal.nativeId(this.iface), QtJambi_LibraryUtilities.internal.nativeId(this.aliasedProperty()));
    }
	
	/**
	 * @see QByteProperty#setValue(byte)
	 */
	@QtUninvokable
	public void setValue(byte newValue) {
		QByteBindable.setValue(QtJambi_LibraryUtilities.internal.nativeId(this.iface), QtJambi_LibraryUtilities.internal.nativeId(this.aliasedProperty()), newValue);
    }
	
	/**
	 * @see QByteProperty#setBinding(QBytePropertyBinding)
	 */
	@QtUninvokable
	public @NonNull QBytePropertyBinding setBinding(@StrictNonNull QBytePropertyBinding newBinding) {
		return new QByteBindable(aliasedProperty(), iface).setBinding(newBinding);
    }
	
	/**
	 * @see QByteProperty#setBinding(QPropertyBinding)
	 */
	@QtUninvokable
	public @NonNull QBytePropertyBinding setBinding(@StrictNonNull QPropertyBinding<@NonNull@QtPrimitiveType Byte> newBinding) {
		return new QByteBindable(aliasedProperty(), iface).setBinding(newBinding);
    }
	
	/**
	 * @see QByteProperty#setBinding(io.qt.QtUtilities.ByteSupplier)
	 */
	@QtUninvokable
	public @NonNull QBytePropertyBinding setBinding(io.qt.QtUtilities.@StrictNonNull ByteSupplier functor) {
        return setBinding(new QBytePropertyBinding(functor));
    }
	
	/**
	 * @see QByteProperty#binding()
	 */
	@QtUninvokable
	public @NonNull QBytePropertyBinding binding() {
        return new QByteBindable(aliasedProperty(), iface).binding();
    }
	
	/**
	 * @see QByteProperty#takeBinding()
	 */
	@QtUninvokable
	public @NonNull QBytePropertyBinding takeBinding() {
        return new QByteBindable(aliasedProperty(), iface).takeBinding();
    }
}
