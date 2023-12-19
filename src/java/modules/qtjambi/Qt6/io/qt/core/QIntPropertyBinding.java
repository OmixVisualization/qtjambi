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

import java.util.function.IntSupplier;

import io.qt.NativeAccess;
import io.qt.NonNull;
import io.qt.QtPrimitiveType;
import io.qt.StrictNonNull;

/**
 * QIntPropertyBinding is primitive-typed version of QPropertyBinding&lt;Integer&gt;.
 * @see QPropertyBinding
 */
public final class QIntPropertyBinding extends QUntypedPropertyBinding {
	
	@NativeAccess
	private QIntPropertyBinding(QPrivateConstructor p) {
		super(p);
	}

	/**
	 * Creates a new <code>QIntPropertyBinding</code> with the given functor <code>f</code>.
	 * @param f
	 */
	public QIntPropertyBinding(@StrictNonNull IntSupplier f) {
		super(f);
	}
	
	/**
	 * Creates a new <code>QIntPropertyBinding</code> as copy of the property's available binding.
	 * @param property
	 */
	public QIntPropertyBinding(@StrictNonNull QIntProperty property) {
		super(property.bindingData());
	}
	
	/**
	 * Creates a new <code>QIntPropertyBinding</code> as copy of the property's available binding.
	 * @param property
	 */
	public QIntPropertyBinding(QObject.@StrictNonNull QIntProperty property) {
		super(property.bindingData());
	}
	
	/**
	 * Creates a new <code>QIntPropertyBinding</code> as copy of the property's available binding.
	 * @param property
	 */
	public QIntPropertyBinding(QObject.@StrictNonNull QComputedIntProperty property) {
		super(property.bindingData());
	}
	
	/**
	 * Creates a new <code>QIntPropertyBinding</code> as copy of the given binding.
	 * @param other
	 */
	public QIntPropertyBinding(@StrictNonNull QIntPropertyBinding other) {
		super(other);
	}
	
	/**
	 * Creates a new <code>QIntPropertyBinding</code> as copy of the property's available binding.
	 * @param property
	 */
	public QIntPropertyBinding(@StrictNonNull QProperty<@NonNull@QtPrimitiveType Integer> property) {
		super(check(property.bindingData(), property.valueMetaType()));
	}
	
	/**
	 * Creates a new <code>QIntPropertyBinding</code> as copy of the property's available binding.
	 * @param property
	 */
	public QIntPropertyBinding(QObject.@StrictNonNull QProperty<@NonNull@QtPrimitiveType Integer> property) {
		super(check(property.bindingData(), property.valueMetaType()));
	}
	
	/**
	 * Creates a new <code>QIntPropertyBinding</code> as copy of the property's available binding.
	 * @param property
	 */
	public QIntPropertyBinding(QObject.@StrictNonNull QComputedProperty<@NonNull@QtPrimitiveType Integer> property) {
		super(check(property.bindingData(), property.valueMetaType()));
	}
	
	/**
	 * Creates a new <code>QIntPropertyBinding</code> as copy of the given binding.
	 * @param other
	 */
	public QIntPropertyBinding(@StrictNonNull QPropertyBinding<@NonNull@QtPrimitiveType Integer> other) {
		super(check(other, false));
	}

	/**
	 * Creates a <code>null</code>-<code>QIntPropertyBinding</code>.
	 * @see QUntypedPropertyBinding#isNull()
	 */
	public QIntPropertyBinding() {
		super((QPropertyBindingData)null);
	}
	
	QIntPropertyBinding(QUntypedPropertyBinding other) {
		super(check(other, true));
	}
	
	private static QPropertyBindingData check(QPropertyBindingData data, QMetaType metaType) {
		if(!QIntProperty.checkType(metaType)) {
			throw new IllegalArgumentException("Given QProperty is not of int type.");
		}
		return data;
	}
	
	private static QUntypedPropertyBinding check(QUntypedPropertyBinding other, boolean noThrow) {
		QMetaType metaType = other==null ? null : other.valueMetaType();
		if(!QIntProperty.checkType(metaType)) {
			if(metaType.isValid() || !other.isNull())
				if(!noThrow)
					throw new IllegalArgumentException("Given QPropertyBinding is not of int type.");
				else other = new QUntypedPropertyBinding();
		}
		return other;
	}
}
