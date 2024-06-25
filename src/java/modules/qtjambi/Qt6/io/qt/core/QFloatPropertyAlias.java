/****************************************************************************
**
** Copyright (C) 2009-2024 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
 * QFloatPropertyAlias is primitive-typed version of QPropertyAlias&lt;Float&gt;.
 * @see QPropertyAlias
 * @deprecated Class was only meant for internal use, use a <code>QProperty</code> and add a binding to the target
 */
@Deprecated(forRemoval=true, since="Qt 6.6")
@SuppressWarnings("removal")
public final class QFloatPropertyAlias extends QAbstractPropertyAlias {
	
	private static QPropertyAlias<@NonNull@QtPrimitiveType Float> check(QPropertyAlias<@NonNull@QtPrimitiveType Float> other){
		if(other.iface!=null && !QFloatProperty.checkType(other.iface.metaType())) {
			throw new IllegalArgumentException("Given QPropertyAlias is not of float type.");
		}
		return other;
	}
	
	private static QMetaType check(QMetaType metaType){
		if(!QFloatProperty.checkType(metaType)) {
			throw new IllegalArgumentException("Given QProperty is not of float type.");
		}
		return metaType;
	}
	
	/**
	 * Creates a new alias to the provided alias.
	 * @param other
	 */
	public QFloatPropertyAlias(@StrictNonNull QPropertyAlias<@NonNull@QtPrimitiveType Float> other) {
		super(check(other));
	}

	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 */
	public QFloatPropertyAlias(@StrictNonNull QFloatProperty property) {
		super(property, QFloatBindable.bindableInterface(0));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 */
	public QFloatPropertyAlias(QObject.@StrictNonNull QFloatProperty property) {
		super(property, QFloatBindable.bindableInterface(1));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 */
	public QFloatPropertyAlias(QObject.@StrictNonNull QComputedIntProperty property) {
		super(property, QFloatBindable.bindableInterface(2));
	}

	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 */
	public QFloatPropertyAlias(@StrictNonNull QProperty<@NonNull@QtPrimitiveType Float> property) {
		super(property, QBindable.bindableInterface(check(property.valueMetaType()).id(), 0));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 */
	public QFloatPropertyAlias(QObject.@StrictNonNull QProperty<@NonNull@QtPrimitiveType Float> property) {
		super(property, QBindable.bindableInterface(check(property.valueMetaType()).id(), 1));
	}
	
	/**
	 * Creates a new alias to the provided property.
	 * @param property
	 */
	public QFloatPropertyAlias(QObject.@StrictNonNull QComputedProperty<@NonNull@QtPrimitiveType Float> property) {
		super(property, QBindable.bindableInterface(check(property.valueMetaType()).id(), 2));
	}
	
	/**
	 * Creates a new alias to the provided alias.
	 * @param other
	 */
	public QFloatPropertyAlias(@StrictNonNull QFloatPropertyAlias other) {
		super(other);
	}
	
	/**
	 * Creates a new alias to the provided binding's underlying property.
	 * @param bindable
	 */
	public QFloatPropertyAlias(@StrictNonNull QFloatBindable bindable) {
		super(bindable.data(), bindable.iface());
	}
	
	/**
	 * Creates a new alias to the provided binding's underlying property.
	 * @param bindable
	 */
	public QFloatPropertyAlias(@StrictNonNull QBindable<@NonNull@QtPrimitiveType Float> bindable) {
		super(bindable.data(), bindable.iface());
		check(bindable.valueMetaType());
	}
	
	/**
	 * @see QFloatProperty#value()
	 */
	@QtUninvokable
	public float value() {
        return QFloatBindable.value(QtJambi_LibraryUtilities.internal.nativeId(this.iface), QtJambi_LibraryUtilities.internal.nativeId(this.aliasedProperty()));
    }
	
	/**
	 * @see QFloatProperty#setValue(float)
	 */
	@QtUninvokable
	public void setValue(float newValue) {
		QFloatBindable.setValue(QtJambi_LibraryUtilities.internal.nativeId(this.iface), QtJambi_LibraryUtilities.internal.nativeId(this.aliasedProperty()), newValue);
    }
	
	/**
	 * @see QFloatProperty#setBinding(QFloatPropertyBinding)
	 */
	@QtUninvokable
	public @NonNull QFloatPropertyBinding setBinding(@StrictNonNull QFloatPropertyBinding newBinding) {
		return new QFloatBindable(aliasedProperty(), iface).setBinding(newBinding);
    }
	
	/**
	 * @see QFloatProperty#setBinding(QPropertyBinding)
	 */
	@QtUninvokable
	public @NonNull QFloatPropertyBinding setBinding(@StrictNonNull QPropertyBinding<@NonNull@QtPrimitiveType Float> newBinding) {
		return new QFloatBindable(aliasedProperty(), iface).setBinding(newBinding);
    }
	
	/**
	 * @see QFloatProperty#setBinding(io.qt.QtUtilities.FloatSupplier)
	 */
	@QtUninvokable
	public @NonNull QFloatPropertyBinding setBinding(io.qt.QtUtilities.@StrictNonNull FloatSupplier functor) {
        return setBinding(new QFloatPropertyBinding(functor));
    }
	
	/**
	 * @see QFloatProperty#binding()
	 */
	@QtUninvokable
	public @NonNull QFloatPropertyBinding binding() {
        return new QFloatBindable(aliasedProperty(), iface).binding();
    }
	
	/**
	 * @see QFloatProperty#takeBinding()
	 */
	@QtUninvokable
	public @NonNull QFloatPropertyBinding takeBinding() {
        return new QFloatBindable(aliasedProperty(), iface).takeBinding();
    }
}
