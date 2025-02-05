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

import java.util.function.BooleanSupplier;

import io.qt.*;

/**
 * QBooleanPropertyBinding is primitive-typed version of QPropertyBinding&lt;Boolean&gt;.
 * @see QPropertyBinding
 */
public final class QBooleanPropertyBinding extends QUntypedPropertyBinding {
	
	@NativeAccess
	private QBooleanPropertyBinding(QPrivateConstructor p) {
		super(p);
	}

	/**
	 * Creates a new <code>QBooleanPropertyBinding</code> with the given functor <code>f</code>.
	 * @param f
	 */
	public QBooleanPropertyBinding(@StrictNonNull BooleanSupplier f) {
		super(f);
	}
	
	/**
	 * Creates a new <code>QBooleanPropertyBinding</code> as copy of the property's available binding.
	 * @param property
	 */
	public QBooleanPropertyBinding(@StrictNonNull QBooleanProperty property) {
		super(property.bindingData());
	}
	
	/**
	 * Creates a new <code>QBooleanPropertyBinding</code> as copy of the property's available binding.
	 * @param property
	 */
	public QBooleanPropertyBinding(QObject.@StrictNonNull QBooleanProperty property) {
		super(property.bindingData());
	}
	
	/**
	 * Creates a new <code>QBooleanPropertyBinding</code> as copy of the property's available binding.
	 * @param property
	 */
	public QBooleanPropertyBinding(QObject.@StrictNonNull QComputedBooleanProperty property) {
		super(property.bindingData());
	}
	
	/**
	 * Creates a new <code>QBooleanPropertyBinding</code> as copy of the property's available binding.
	 * @param property
	 */
	public QBooleanPropertyBinding(@StrictNonNull QProperty<@NonNull@QtPrimitiveType Boolean> property) {
		super(check(property.bindingData(), property.valueMetaType()));
	}
	
	/**
	 * Creates a new <code>QBooleanPropertyBinding</code> as copy of the property's available binding.
	 * @param property
	 */
	public QBooleanPropertyBinding(QObject.@StrictNonNull QProperty<@NonNull@QtPrimitiveType Boolean> property) {
		super(check(property.bindingData(), property.valueMetaType()));
	}
	
	/**
	 * Creates a new <code>QBooleanPropertyBinding</code> as copy of the property's available binding.
	 * @param property
	 */
	public QBooleanPropertyBinding(QObject.@StrictNonNull QComputedProperty<@NonNull@QtPrimitiveType Boolean> property) {
		super(check(property.bindingData(), property.valueMetaType()));
	}
	
	/**
	 * Creates a new <code>QBooleanPropertyBinding</code> as copy of the given binding.
	 * @param other
	 */
	public QBooleanPropertyBinding(@StrictNonNull QBooleanPropertyBinding other) {
		super(other);
	}
	
	/**
	 * Creates a new <code>QBooleanPropertyBinding</code> as copy of the given binding.
	 * @param other
	 */
	public QBooleanPropertyBinding(@StrictNonNull QPropertyBinding<@NonNull@QtPrimitiveType Boolean> other) {
		super(check(other, false));
	}

	/**
	 * Creates a <code>null</code>-<code>QBooleanPropertyBinding</code>.
	 * @see QUntypedPropertyBinding#isNull()
	 */
	public QBooleanPropertyBinding() {
		super((QPropertyBindingData)null);
	}
	
	QBooleanPropertyBinding(QUntypedPropertyBinding other) {
		super(check(other, true));
	}
	
	private static QPropertyBindingData check(QPropertyBindingData data, QMetaType metaType) {
		if(!QBooleanProperty.checkType(metaType)) {
			throw new IllegalArgumentException("Given QProperty is not of boolean type.");
		}
		return data;
	}
	
	private static QUntypedPropertyBinding check(QUntypedPropertyBinding other, boolean noThrow) {
		QMetaType metaType = other==null ? null : other.valueMetaType();
		if(!QBooleanProperty.checkType(metaType)) {
			if(metaType.isValid() || !other.isNull())
				if(!noThrow)
					throw new IllegalArgumentException("Given QPropertyBinding is not of boolean type.");
				else other = new QUntypedPropertyBinding();
		}
		return other;
	}
}
