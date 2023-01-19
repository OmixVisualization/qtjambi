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

import java.util.function.LongSupplier;

import io.qt.NativeAccess;
import io.qt.QtPrimitiveType;

/**
 * QLongPropertyBinding is primitive-typed version of QPropertyBinding&lt;Long&gt;.
 * @see QPropertyBinding
 */
public final class QLongPropertyBinding extends QUntypedPropertyBinding {
	
	@NativeAccess
	private QLongPropertyBinding(QPrivateConstructor p) {
		super(p);
	}

	/**
	 * Creates a new <code>QLongPropertyBinding</code> with the given functor <code>f</code>.
	 * @param t
	 */
	public QLongPropertyBinding(LongSupplier f) {
		super(f);
	}
	
	/**
	 * Creates a new <code>QLongPropertyBinding</code> as copy of the property's available binding.
	 * @param property
	 */
	public QLongPropertyBinding(QLongProperty property) {
		super(property.bindingData());
	}
	
	/**
	 * Creates a new <code>QLongPropertyBinding</code> as copy of the property's available binding.
	 * @param property
	 */
	public QLongPropertyBinding(QObject.QLongProperty property) {
		super(property.bindingData());
	}
	
	/**
	 * Creates a new <code>QLongPropertyBinding</code> as copy of the property's available binding.
	 * @param property
	 */
	public QLongPropertyBinding(QObject.QComputedLongProperty property) {
		super(property.bindingData());
	}
	
	/**
	 * Creates a new <code>QLongPropertyBinding</code> as copy of the given binding.
	 * @param other
	 */
	public QLongPropertyBinding(QLongPropertyBinding other) {
		super(other);
	}
	
	/**
	 * Creates a new <code>QLongPropertyBinding</code> as copy of the property's available binding.
	 * @param property
	 */
	public QLongPropertyBinding(QProperty<@QtPrimitiveType Long> property) {
		super(check(property.bindingData(), property.valueMetaType()));
	}
	
	/**
	 * Creates a new <code>QLongPropertyBinding</code> as copy of the property's available binding.
	 * @param property
	 */
	public QLongPropertyBinding(QObject.QProperty<@QtPrimitiveType Long> property) {
		super(check(property.bindingData(), property.valueMetaType()));
	}
	
	/**
	 * Creates a new <code>QLongPropertyBinding</code> as copy of the property's available binding.
	 * @param property
	 */
	public QLongPropertyBinding(QObject.QComputedProperty<@QtPrimitiveType Long> property) {
		super(check(property.bindingData(), property.valueMetaType()));
	}
	
	/**
	 * Creates a new <code>QLongPropertyBinding</code> as copy of the given binding.
	 * @param other
	 */
	public QLongPropertyBinding(QPropertyBinding<@QtPrimitiveType Long> other) {
		super(check(other, false));
	}

	/**
	 * Creates a <code>null</code>-<code>QLongPropertyBinding</code>.
	 * @param binding
	 * @return binding copy
	 * @see QUntypedPropertyBinding#isNull()
	 */
	public QLongPropertyBinding() {
		super((QPropertyBindingData)null);
	}

	QLongPropertyBinding(QUntypedPropertyBinding other) {
		super(check(other, true));
	}
	
	private static QPropertyBindingData check(QPropertyBindingData data, QMetaType metaType) {
		if(!QLongProperty.checkType(metaType)) {
			throw new IllegalArgumentException("Given QProperty is not of long type.");
		}
		return data;
	}
	
	private static QUntypedPropertyBinding check(QUntypedPropertyBinding other, boolean noThrow) {
		QMetaType metaType = other==null ? null : other.valueMetaType();
		if(!QLongProperty.checkType(metaType)) {
			if(metaType.isValid() || !other.isNull())
				if(!noThrow)
					throw new IllegalArgumentException("Given QPropertyBinding is not of long type.");
				else other = new QUntypedPropertyBinding();
		}
		return other;
	}
}
