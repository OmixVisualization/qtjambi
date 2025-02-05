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
 * QLongPropertyAlias is primitive-typed version of QPropertyAlias&lt;Long&gt;.
 * @see QPropertyAlias
 */
@Deprecated(forRemoval=true, since="Qt 6.6")
@SuppressWarnings("removal")
public final class QLongPropertyAlias extends QAbstractPropertyAlias {
	
	private static QPropertyAlias<@NonNull@QtPrimitiveType Long> check(QPropertyAlias<@NonNull@QtPrimitiveType Long> other){
		if(other.iface!=null && !QLongProperty.checkType(other.iface.metaType())) {
			throw new IllegalArgumentException("Given QPropertyAlias is not of long type.");
		}
		return other;
	}
	
	private static QMetaType check(QMetaType metaType){
		if(!QLongProperty.checkType(metaType)) {
			throw new IllegalArgumentException("Given QProperty is not of long type.");
		}
		return metaType;
	}
	
	/**
	 * Creates a new alias to the provided alias.
	 * @param other
	 */
	public QLongPropertyAlias(@StrictNonNull QPropertyAlias<@NonNull@QtPrimitiveType Long> other) {
		super(check(other));
	}

	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 */
	public QLongPropertyAlias(@StrictNonNull QLongProperty property) {
		super(property, QLongBindable.bindableInterface(0));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 */
	public QLongPropertyAlias(QObject.@StrictNonNull QLongProperty property) {
		super(property, QLongBindable.bindableInterface(1));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 */
	public QLongPropertyAlias(QObject.@StrictNonNull QComputedIntProperty property) {
		super(property, QLongBindable.bindableInterface(2));
	}

	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 */
	public QLongPropertyAlias(@StrictNonNull QProperty<@NonNull@QtPrimitiveType Long> property) {
		super(property, QBindable.bindableInterface(check(property.valueMetaType()).id(), 0));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 */
	public QLongPropertyAlias(QObject.@StrictNonNull QProperty<@NonNull@QtPrimitiveType Long> property) {
		super(property, QBindable.bindableInterface(check(property.valueMetaType()).id(), 1));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 */
	public QLongPropertyAlias(QObject.@StrictNonNull QComputedProperty<@NonNull@QtPrimitiveType Long> property) {
		super(property, QBindable.bindableInterface(check(property.valueMetaType()).id(), 2));
	}
	
	/**
	 * Creates a new alias to the provided alias.
	 * @param other
	 */
	public QLongPropertyAlias(@StrictNonNull QLongPropertyAlias other) {
		super(other);
	}
	
	/**
	 * Creates a new alias to the provided binding's underlying property.
	 * @param bindable
	 */
	public QLongPropertyAlias(@StrictNonNull QLongBindable bindable) {
		super(bindable.data(), bindable.iface());
	}
	
	/**
	 * Creates a new alias to the provided binding's underlying property.
	 * @param bindable
	 */
	public QLongPropertyAlias(@StrictNonNull QBindable<@NonNull@QtPrimitiveType Long> bindable) {
		super(bindable.data(), bindable.iface());
		check(bindable.valueMetaType());
	}
	
	/**
	 * @see QLongProperty#value()
	 */
	@QtUninvokable
	public long value() {
        return QLongBindable.value(QtJambi_LibraryUtilities.internal.nativeId(this.iface), QtJambi_LibraryUtilities.internal.nativeId(this.aliasedProperty()));
    }
	
	/**
	 * @see QLongProperty#setValue(long)
	 */
	@QtUninvokable
	public void setValue(long newValue) {
		QLongBindable.setValue(QtJambi_LibraryUtilities.internal.nativeId(this.iface), QtJambi_LibraryUtilities.internal.nativeId(this.aliasedProperty()), newValue);
    }
	
	/**
	 * @see QLongProperty#setBinding(QLongPropertyBinding)
	 */
	@QtUninvokable
	public @NonNull QLongPropertyBinding setBinding(@StrictNonNull QLongPropertyBinding newBinding) {
		return new QLongBindable(aliasedProperty(), iface).setBinding(newBinding);
    }
	
	/**
	 * @see QLongProperty#setBinding(QPropertyBinding)
	 */
	@QtUninvokable
	public @NonNull QLongPropertyBinding setBinding(@StrictNonNull QPropertyBinding<@NonNull@QtPrimitiveType Long> newBinding) {
		return new QLongBindable(aliasedProperty(), iface).setBinding(newBinding);
    }
	
	/**
	 * @see QLongProperty#setBinding(java.util.function.LongSupplier)
	 */
	@QtUninvokable
	public @NonNull QLongPropertyBinding setBinding(java.util.function.@StrictNonNull LongSupplier functor) {
        return setBinding(new QLongPropertyBinding(functor));
    }
	
	/**
	 * @see QLongProperty#binding()
	 */
	@QtUninvokable
	public @NonNull QLongPropertyBinding binding() {
        return new QLongBindable(aliasedProperty(), iface).binding();
    }
	
	/**
	 * @see QLongProperty#takeBinding()
	 */
	@QtUninvokable
	public @NonNull QLongPropertyBinding takeBinding() {
        return new QLongBindable(aliasedProperty(), iface).takeBinding();
    }
}
