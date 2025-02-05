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
import io.qt.QtUtilities.ByteSupplier;

/**
 * QBytePropertyBinding is primitive-typed version of QPropertyBinding&lt;Byte&gt;.
 * @see QPropertyBinding
 */
public final class QBytePropertyBinding extends QUntypedPropertyBinding {
	
	@NativeAccess
	private QBytePropertyBinding(QPrivateConstructor p) {
		super(p);
	}

	/**
	 * Creates a new <code>QBytePropertyBinding</code> with the given functor <code>f</code>.
	 * @param f
	 */
	public QBytePropertyBinding(@StrictNonNull ByteSupplier f) {
		super(f);
	}
	
	/**
	 * Creates a new <code>QBytePropertyBinding</code> as copy of the property's available binding.
	 * @param property
	 */
	public QBytePropertyBinding(@StrictNonNull QByteProperty property) {
		super(property.bindingData());
	}
	
	/**
	 * Creates a new <code>QBytePropertyBinding</code> as copy of the property's available binding.
	 * @param property
	 */
	public QBytePropertyBinding(QObject.@StrictNonNull QByteProperty property) {
		super(property.bindingData());
	}
	
	/**
	 * Creates a new <code>QBytePropertyBinding</code> as copy of the property's available binding.
	 * @param property
	 */
	public QBytePropertyBinding(QObject.@StrictNonNull QComputedByteProperty property) {
		super(property.bindingData());
	}
	
	/**
	 * Creates a new <code>QBytePropertyBinding</code> as copy of the property's available binding.
	 * @param property
	 */
	public QBytePropertyBinding(@StrictNonNull QProperty<@NonNull@QtPrimitiveType Byte> property) {
		super(check(property.bindingData(), property.valueMetaType()));
	}
	
	/**
	 * Creates a new <code>QBytePropertyBinding</code> as copy of the property's available binding.
	 * @param property
	 */
	public QBytePropertyBinding(QObject.@StrictNonNull QProperty<@NonNull@QtPrimitiveType Byte> property) {
		super(check(property.bindingData(), property.valueMetaType()));
	}
	
	/**
	 * Creates a new <code>QBytePropertyBinding</code> as copy of the property's available binding.
	 * @param property
	 */
	public QBytePropertyBinding(QObject.@StrictNonNull QComputedProperty<@NonNull@QtPrimitiveType Byte> property) {
		super(check(property.bindingData(), property.valueMetaType()));
	}
	
	/**
	 * Creates a new <code>QBytePropertyBinding</code> as copy of the given binding.
	 * @param other
	 */
	public QBytePropertyBinding(@StrictNonNull QBytePropertyBinding other) {
		super(other);
	}
	
	/**
	 * Creates a new <code>QBytePropertyBinding</code> as copy of the given binding.
	 * @param other
	 */
	public QBytePropertyBinding(@StrictNonNull QPropertyBinding<@NonNull@QtPrimitiveType Byte> other) {
		super(check(other, false));
	}

	/**
	 * Creates a <code>null</code>-<code>QBytePropertyBinding</code>.
	 * @see QUntypedPropertyBinding#isNull()
	 */
	public QBytePropertyBinding() {
		super((QPropertyBindingData)null);
	}
	
	QBytePropertyBinding(QUntypedPropertyBinding other) {
		super(check(other, true));
	}
	
	private static QPropertyBindingData check(QPropertyBindingData data, QMetaType metaType) {
		if(!QByteProperty.checkType(metaType)) {
			throw new IllegalArgumentException("Given QProperty is not of byte type.");
		}
		return data;
	}
	
	private static QUntypedPropertyBinding check(QUntypedPropertyBinding other, boolean noThrow) {
		QMetaType metaType = other==null ? null : other.valueMetaType();
		if(!QByteProperty.checkType(metaType)) {
			if(metaType.isValid() || !other.isNull())
				if(!noThrow)
					throw new IllegalArgumentException("Given QPropertyBinding is not of byte type.");
				else other = new QUntypedPropertyBinding();
		}
		return other;
	}
}
