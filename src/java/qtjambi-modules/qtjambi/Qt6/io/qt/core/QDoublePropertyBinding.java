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

import java.util.function.DoubleSupplier;

import io.qt.NativeAccess;
import io.qt.QtPrimitiveType;

/**
 * QDoublePropertyBinding is primitive-typed version of QPropertyBinding&lt;Double&gt;.
 * @see QPropertyBinding
 */
public final class QDoublePropertyBinding extends QUntypedPropertyBinding {
	
	@NativeAccess
	private QDoublePropertyBinding(QPrivateConstructor p) {
		super(p);
	}

	/**
	 * Creates a new <code>QDoublePropertyBinding</code> with the given functor <code>f</code>.
	 * @param t
	 */
	public QDoublePropertyBinding(DoubleSupplier f) {
		super(f);
	}
	
	/**
	 * Creates a new <code>QDoublePropertyBinding</code> as copy of the property's available binding.
	 * @param property
	 */
	public QDoublePropertyBinding(QDoubleProperty property) {
		super(property.bindingData());
	}
	
	/**
	 * Creates a new <code>QDoublePropertyBinding</code> as copy of the property's available binding.
	 * @param property
	 */
	public QDoublePropertyBinding(QObject.QDoubleProperty property) {
		super(property.bindingData());
	}
	
	/**
	 * Creates a new <code>QDoublePropertyBinding</code> as copy of the property's available binding.
	 * @param property
	 */
	public QDoublePropertyBinding(QObject.QComputedDoubleProperty property) {
		super(property.bindingData());
	}
	
	/**
	 * Creates a new <code>QDoublePropertyBinding</code> as copy of the given binding.
	 * @param other
	 */
	public QDoublePropertyBinding(QDoublePropertyBinding other) {
		super(other);
	}
	
	/**
	 * Creates a new <code>QDoublePropertyBinding</code> as copy of the property's available binding.
	 * @param property
	 */
	public QDoublePropertyBinding(QProperty<@QtPrimitiveType Double> property) {
		super(check(property.bindingData(), property.valueMetaType()));
	}
	
	/**
	 * Creates a new <code>QDoublePropertyBinding</code> as copy of the property's available binding.
	 * @param property
	 */
	public QDoublePropertyBinding(QObject.QProperty<@QtPrimitiveType Double> property) {
		super(check(property.bindingData(), property.valueMetaType()));
	}
	
	/**
	 * Creates a new <code>QDoublePropertyBinding</code> as copy of the property's available binding.
	 * @param property
	 */
	public QDoublePropertyBinding(QObject.QComputedProperty<@QtPrimitiveType Double> property) {
		super(check(property.bindingData(), property.valueMetaType()));
	}
	
	/**
	 * Creates a new <code>QDoublePropertyBinding</code> as copy of the given binding.
	 * @param other
	 */
	public QDoublePropertyBinding(QPropertyBinding<@QtPrimitiveType Double> other) {
		super(check(other, false));
	}

	/**
	 * Creates a <code>null</code>-<code>QDoublePropertyBinding</code>.
	 * @param binding
	 * @return binding copy
	 * @see QUntypedPropertyBinding#isNull()
	 */
	public QDoublePropertyBinding() {
		super((QPropertyBindingData)null);
	}
	
	QDoublePropertyBinding(QUntypedPropertyBinding other) {
		super(check(other, true));
	}
	
	private static QPropertyBindingData check(QPropertyBindingData data, QMetaType metaType) {
		if(!QDoubleProperty.checkType(metaType)) {
			throw new IllegalArgumentException("Given QProperty is not of double type.");
		}
		return data;
	}
	
	private static QUntypedPropertyBinding check(QUntypedPropertyBinding other, boolean noThrow) {
		QMetaType metaType = other==null ? null : other.valueMetaType();
		if(!QDoubleProperty.checkType(metaType)) {
			if(metaType.isValid() || !other.isNull())
				if(!noThrow)
					throw new IllegalArgumentException("Given QPropertyBinding is not of double type.");
				else other = new QUntypedPropertyBinding();
		}
		return other;
	}
}
