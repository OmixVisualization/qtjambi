/****************************************************************************
**
** Copyright (C) 2009-2022 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
import io.qt.QtUninvokable;
import io.qt.QtUtilities.ShortSupplier;

/**
 * QShortBindable is primitive-typed version of QBindable&lt;Short&gt;.
 * @see QBindable
 */
public final class QShortBindable extends QUntypedBindable {

	private QShortBindable(QPrivateConstructor p) {
		super(p);
	}

	/**
	 * Creates a new <code>short</code>-typed invalid bindable.
	 */
	public QShortBindable() {
		super(null, null);
	}
	
	/**
	 * Creates a copy of the provided bindable. 
	 * If the type of the other bindable is not <code>short</code> it remains invalid.
	 * @param other
	 */
	public QShortBindable(QUntypedBindable other) {
		super(other.data(), other.iface());
		check();
	}
	
	private void check() {
		QBindableInterface iface = iface();
		if(iface!=null) {
			QMetaType metaType = iface.metaType();
			if(metaType==null 
					|| (metaType.id()!=QMetaType.Type.QChar.value()
						&& metaType.id()!=QMetaType.Type.Char16.value()
						&& metaType.id()!=QMetaType.Type.Short.value()
						&& metaType.id()!=QMetaType.Type.UShort.value()
						&& metaType.id()!=QMetaType.Type.QVariant.value())) {
				setIface(null);
				setData(null);
			}
		}
	}

	QShortBindable(QUntypedPropertyData d, QBindableInterface i) {
		super(d, i);
	}
	
	/**
	 * Creates a new bindable from provided property.
	 * @param property
	 */
	public QShortBindable(QShortProperty property) {
		super(property, bindableInterface(0));
	}
	
	/**
	 * Creates a new bindable from provided property.
	 * @param property
	 */
	public QShortBindable(QObject.QShortProperty property) {
		super(property, bindableInterface(1));
	}
	
	/**
	 * Creates a new bindable from provided property.
	 * @param property
	 */
	public QShortBindable(QObject.QComputedShortProperty property) {
		super(property, bindableInterface(2));
	}
	
	/**
	 * Creates a new bindable from provided property.
	 * If the type of the property is not <code>short</code> an {@link IllegalArgumentException} is thrown.
	 * @param property
	 */
	public QShortBindable(QProperty<@QtPrimitiveType Short> property) {
		super(property, QBindable.bindableInterface(property.valueMetaType().id(), 0));
		check();
		if(iface()==null)
			throw new IllegalArgumentException("Given QProperty is not of short type.");
	}
	
	/**
	 * Creates a new bindable from provided property.
	 * If the type of the property is not <code>short</code> an {@link IllegalArgumentException} is thrown.
	 * @param property
	 */
	public QShortBindable(QObject.QProperty<@QtPrimitiveType Short> property) {
		super(property, QBindable.bindableInterface(property.valueMetaType().id(), 1));
		check();
		if(iface()==null)
			throw new IllegalArgumentException("Given QProperty is not of short type.");
	}
	
	/**
	 * Creates a new bindable from provided property.
	 * If the type of the property is not <code>short</code> an {@link IllegalArgumentException} is thrown.
	 * @param property
	 */
	public QShortBindable(QObject.QComputedProperty<@QtPrimitiveType Short> property) {
		super(property, bindableInterface(2));
		check();
		if(iface()==null)
			throw new IllegalArgumentException("Given QProperty is not of short type.");
	}
	
	static native QBindableInterface bindableInterface(int propertyType);

	/**
	 * Creates a binding to the underlying property.
	 */
	@QtUninvokable
	@Override
	public QShortPropertyBinding makeBinding()
    {
		QUntypedPropertyBinding binding = super.makeBinding();
		if(binding instanceof QShortPropertyBinding) {
			return (QShortPropertyBinding)binding;
		}else {
			return new QShortPropertyBinding(binding);
		}
    }
	
    /**
     * Returns the binding expression that is associated with the underlying property. 
     * A default constructed {@link QShortPropertyBinding} will be returned if no such association exists.
     * @return binding
     */
	@QtUninvokable
	@Override
    public QShortPropertyBinding binding()
    {
		QUntypedPropertyBinding binding = super.binding();
		if(binding instanceof QShortPropertyBinding) {
			return (QShortPropertyBinding)binding;
		}else {
			return new QShortPropertyBinding(binding);
		}
    }
    
	@QtUninvokable
	public QShortPropertyBinding setBinding(QShortPropertyBinding binding)
    {
		QUntypedPropertyBinding oldBinding = super.overrideBinding(binding);
		if(oldBinding instanceof QShortPropertyBinding) {
			return (QShortPropertyBinding)oldBinding;
		}else {
			return new QShortPropertyBinding(oldBinding);
		}
    }
	
	@QtUninvokable
	public QShortPropertyBinding setBinding(QPropertyBinding<@QtPrimitiveType Short> binding)
    {
		if(binding!=null && !QShortProperty.checkType(binding.valueMetaType()))
			return new QShortPropertyBinding();
		QUntypedPropertyBinding oldBinding = super.overrideBinding(binding);
		if(oldBinding instanceof QShortPropertyBinding) {
			return (QShortPropertyBinding)oldBinding;
		}else {
			return new QShortPropertyBinding(oldBinding);
		}
    }
	
	@QtUninvokable
	public QShortPropertyBinding setBinding(ShortSupplier functor)
    {
		return setBinding(new QShortPropertyBinding(functor));
    }
	
	/**
	 * <p>Disassociates the binding expression from this property and returns it.</p>
	 * <p>After calling this function, the value of the property will only change if you assign a new value to it, or when a new binding is set.</p>
	 * @return the removed binding
	 */
	@QtUninvokable
    public QShortPropertyBinding takeBinding()
    {
        return setBinding(new QShortPropertyBinding());
    }
    
	/**
	 * Returns the value of the underlying property. This may evaluate a binding expression that is tied to the property, before returning the value.
	 * @return value
	 */
    @io.qt.QtUninvokable
    public final short value() {
        return value(QtJambi_LibraryUtilities.internal.nativeId(this.iface()), QtJambi_LibraryUtilities.internal.nativeId(this.data()));
    }
    
	/**
	 * <p>Assigns newValue to the underlying property and removes the property's associated binding, if present.</p>
	 * @param newValue
	 */
    @io.qt.QtUninvokable
    public final void setValue(short value) {
        setValue(QtJambi_LibraryUtilities.internal.nativeId(this.iface()), QtJambi_LibraryUtilities.internal.nativeId(this.data()), value);
    }
    
    @io.qt.QtUninvokable
    static native short value(long ifaceId, long dataId);
    
    @io.qt.QtUninvokable
    static native void setValue(long ifaceId, long dataId, short value);
}
