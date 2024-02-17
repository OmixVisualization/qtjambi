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

import io.qt.NativeAccess;
import io.qt.NonNull;
import io.qt.QtPrimitiveType;
import io.qt.StrictNonNull;
import io.qt.QtUtilities.ShortSupplier;

/**
 * QShortPropertyBinding is primitive-typed version of QPropertyBinding&lt;Short&gt;.
 * @see QPropertyBinding
 */
public final class QShortPropertyBinding extends QUntypedPropertyBinding {
	
	@NativeAccess
	private QShortPropertyBinding(QPrivateConstructor p) {
		super(p);
	}

	/**
	 * Creates a new <code>QShortPropertyBinding</code> with the given functor <code>f</code>.
	 * @param f
	 */
	public QShortPropertyBinding(@StrictNonNull ShortSupplier f) {
		super(f);
	}
	
	/**
	 * Creates a new <code>QShortPropertyBinding</code> as copy of the property's available binding.
	 * @param property
	 */
	public QShortPropertyBinding(@StrictNonNull QShortProperty property) {
		super(property.bindingData());
	}
	
	/**
	 * Creates a new <code>QShortPropertyBinding</code> as copy of the property's available binding.
	 * @param property
	 */
	public QShortPropertyBinding(QObject.@StrictNonNull QShortProperty property) {
		super(property.bindingData());
	}
	
	/**
	 * Creates a new <code>QShortPropertyBinding</code> as copy of the property's available binding.
	 * @param property
	 */
	public QShortPropertyBinding(QObject.@StrictNonNull QComputedShortProperty property) {
		super(property.bindingData());
	}
	
	/**
	 * Creates a new <code>QShortPropertyBinding</code> as copy of the given binding.
	 * @param other
	 */
	public QShortPropertyBinding(@StrictNonNull QShortPropertyBinding other) {
		super(other);
	}
	
	/**
	 * Creates a new <code>QShortPropertyBinding</code> as copy of the property's available binding.
	 * @param property
	 */
	public QShortPropertyBinding(@StrictNonNull QProperty<@NonNull@QtPrimitiveType Short> property) {
		super(check(property.bindingData(), property.valueMetaType()));
	}
	
	/**
	 * Creates a new <code>QShortPropertyBinding</code> as copy of the property's available binding.
	 * @param property
	 */
	public QShortPropertyBinding(QObject.@StrictNonNull QProperty<@NonNull@QtPrimitiveType Short> property) {
		super(check(property.bindingData(), property.valueMetaType()));
	}
	
	/**
	 * Creates a new <code>QShortPropertyBinding</code> as copy of the property's available binding.
	 * @param property
	 */
	public QShortPropertyBinding(QObject.@StrictNonNull QComputedProperty<@NonNull@QtPrimitiveType Short> property) {
		super(check(property.bindingData(), property.valueMetaType()));
	}
	
	/**
	 * Creates a new <code>QShortPropertyBinding</code> as copy of the given binding.
	 * @param other
	 */
	public QShortPropertyBinding(@StrictNonNull QPropertyBinding<@NonNull@QtPrimitiveType Short> other) {
		super(check(other, false));
	}

	/**
	 * Creates a <code>null</code>-<code>QShortPropertyBinding</code>.
	 * @see QUntypedPropertyBinding#isNull()
	 */
	public QShortPropertyBinding() {
		super((QPropertyBindingData)null);
	}

	QShortPropertyBinding(QUntypedPropertyBinding other) {
		super(check(other, true));
	}
	
	private static QPropertyBindingData check(QPropertyBindingData data, QMetaType metaType) {
		if(!QShortProperty.checkType(metaType)) {
			throw new IllegalArgumentException("Given QProperty is not of short type.");
		}
		return data;
	}
	
	private static QUntypedPropertyBinding check(QUntypedPropertyBinding other, boolean noThrow) {
		QMetaType metaType = other==null ? null : other.valueMetaType();
		if(!QShortProperty.checkType(metaType)) {
			if(metaType.isValid() || !other.isNull())
				if(!noThrow)
					throw new IllegalArgumentException("Given QPropertyBinding is not of short type.");
				else other = new QUntypedPropertyBinding();
		}
		return other;
	}
}
