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

import io.qt.QtPrimitiveType;
import io.qt.QtUtilities.FloatSupplier;
import io.qt.internal.NativeAccess;

/**
 * QFloatPropertyBinding is primitive-typed version of QPropertyBinding&lt;Float&gt;.
 * @see QPropertyBinding
 */
public final class QFloatPropertyBinding extends QUntypedPropertyBinding {
	
	@NativeAccess
	private QFloatPropertyBinding(QPrivateConstructor p) {
		super(p);
	}

	/**
	 * Creates a new <code>QFloatPropertyBinding</code> with the given functor <code>f</code>.
	 * @param t
	 */
	public QFloatPropertyBinding(FloatSupplier f) {
		super(f);
	}
	
	/**
	 * Creates a new <code>QFloatPropertyBinding</code> as copy of the property's available binding.
	 * @param property
	 */
	public QFloatPropertyBinding(QFloatProperty property) {
		super(property.bindingData());
	}
	
	/**
	 * Creates a new <code>QFloatPropertyBinding</code> as copy of the property's available binding.
	 * @param property
	 */
	public QFloatPropertyBinding(QObject.QFloatProperty property) {
		super(property.bindingData());
	}
	
	/**
	 * Creates a new <code>QFloatPropertyBinding</code> as copy of the property's available binding.
	 * @param property
	 */
	public QFloatPropertyBinding(QObject.QComputedFloatProperty property) {
		super(property.bindingData());
	}
	
	/**
	 * Creates a new <code>QFloatPropertyBinding</code> as copy of the property's available binding.
	 * @param property
	 */
	public QFloatPropertyBinding(QProperty<@QtPrimitiveType Float> property) {
		super(check(property.bindingData(), property.valueMetaType()));
	}
	
	/**
	 * Creates a new <code>QFloatPropertyBinding</code> as copy of the property's available binding.
	 * @param property
	 */
	public QFloatPropertyBinding(QObject.QProperty<@QtPrimitiveType Float> property) {
		super(check(property.bindingData(), property.valueMetaType()));
	}
	
	/**
	 * Creates a new <code>QFloatPropertyBinding</code> as copy of the property's available binding.
	 * @param property
	 */
	public QFloatPropertyBinding(QObject.QComputedProperty<@QtPrimitiveType Float> property) {
		super(check(property.bindingData(), property.valueMetaType()));
	}
	
	/**
	 * Creates a new <code>QFloatPropertyBinding</code> as copy of the given binding.
	 * @param other
	 */
	public QFloatPropertyBinding(QFloatPropertyBinding other) {
		super(other, true);
	}
	
	/**
	 * Creates a new <code>QFloatPropertyBinding</code> as copy of the given binding.
	 * @param other
	 */
	public QFloatPropertyBinding(QPropertyBinding<@QtPrimitiveType Float> other) {
		super(check(other, false), true);
	}

	/**
	 * Creates a <code>null</code>-<code>QFloatPropertyBinding</code>.
	 * @param binding
	 * @return binding copy
	 * @see QUntypedPropertyBinding#isNull()
	 */
	public QFloatPropertyBinding() {
		super((QPropertyBindingData)null);
	}
	
	QFloatPropertyBinding(QUntypedPropertyBinding other) {
		super(check(other, true), true);
	}
	
	private static QPropertyBindingData check(QPropertyBindingData data, QMetaType metaType) {
		if(!QFloatProperty.checkType(metaType)) {
			throw new IllegalArgumentException("Given QProperty is not of float type.");
		}
		return data;
	}
	
	private static QUntypedPropertyBinding check(QUntypedPropertyBinding other, boolean noThrow) {
		QMetaType metaType = other==null ? null : other.valueMetaType();
		if(!QFloatProperty.checkType(metaType)) {
			if(metaType.isValid() || !other.isNull())
				if(!noThrow)
					throw new IllegalArgumentException("Given QPropertyBinding is not of float type.");
				else other = new QUntypedPropertyBinding();
		}
		return other;
	}
}
