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

import static io.qt.internal.QtJambiInternal.nativeId;

import io.qt.QtPrimitiveType;
import io.qt.QtUninvokable;

/**
 * QCharBindable is primitive-typed version of QBindable&lt;Character&gt;.
 * @see QBindable
 */
public final class QCharBindable extends QUntypedBindable {

	private QCharBindable(QPrivateConstructor p) {
		super(p);
	}

	/**
	 * Creates a new <code>char</code>-typed invalid bindable.
	 */
	public QCharBindable() {
		super(null, null, true);
	}
	
	/**
	 * Creates a copy of the provided bindable. 
	 * If the type of the other bindable is not <code>char</code> it remains invalid.
	 * @param other
	 */
	public QCharBindable(QUntypedBindable other) {
		super(other.data(), other.iface(), true);
		check();
	}
	
	private void check() {
		QBindableInterface iface = iface();
		if(iface!=null) {
			QMetaType metaType = iface.metaType();
			if(metaType==null 
					|| metaType.sizeOf()!=2
					|| (metaType.id()!=QMetaType.Type.QChar.value()
						&& metaType.id()!=QMetaType.Type.Char16.value()
						&& metaType.id()!=QMetaType.Type.Short.value()
						&& metaType.id()!=QMetaType.Type.UShort.value())) {
				setIface(null);
				setData(null);
			}
		}
	}
	
	/**
	 * Creates a new bindable from provided property.
	 * @param property
	 */
	public QCharBindable(QCharProperty property) {
		super(property, bindableInterface(0), true);
	}
	
	/**
	 * Creates a new bindable from provided property.
	 * @param property
	 */
	public QCharBindable(QObject.QCharProperty property) {
		super(property, bindableInterface(1), true);
	}
	
	/**
	 * Creates a new bindable from provided property.
	 * @param property
	 */
	public QCharBindable(QObject.QComputedCharProperty property) {
		super(property, bindableInterface(2), true);
	}
	
	/**
	 * Creates a new bindable from provided property.
	 * If the type of the property is not <code>char</code> an {@link IllegalArgumentException} is thrown.
	 * @param property
	 */
	public QCharBindable(QProperty<@QtPrimitiveType Character> property) {
		super(property, QBindable.bindableInterface(property.valueMetaType().id(), 0), true);
		check();
		if(iface()==null)
			throw new IllegalArgumentException("Given QProperty is not of char type.");
	}
	
	/**
	 * Creates a new bindable from provided property.
	 * If the type of the property is not <code>float</code> an {@link IllegalArgumentException} is thrown.
	 * @param property
	 */
	public QCharBindable(QObject.QProperty<@QtPrimitiveType Character> property) {
		super(property, QBindable.bindableInterface(property.valueMetaType().id(), 1), true);
		check();
		if(iface()==null)
			throw new IllegalArgumentException("Given QProperty is not of char type.");
	}
	
	/**
	 * Creates a new bindable from provided property.
	 * If the type of the property is not <code>float</code> an {@link IllegalArgumentException} is thrown.
	 * @param property
	 */
	public QCharBindable(QObject.QComputedProperty<@QtPrimitiveType Character> property) {
		super(property, bindableInterface(2), true);
		check();
		if(iface()==null)
			throw new IllegalArgumentException("Given QProperty is not of char type.");
	}
	
	static native QBindableInterface bindableInterface(int propertyType);

	QCharBindable(QUntypedPropertyData d, QBindableInterface i) {
		super(d, i, true);
	}

	/**
	 * Creates a binding to the underlying property.
	 */
	@QtUninvokable
	@Override
	public QCharPropertyBinding makeBinding()
    {
		QUntypedPropertyBinding binding = super.makeBinding();
		if(binding instanceof QCharPropertyBinding) {
			return (QCharPropertyBinding)binding;
		}else {
			return new QCharPropertyBinding(binding);
		}
    }
	
    /**
     * Returns the binding expression that is associated with the underlying property. 
     * A default constructed {@link QCharPropertyBinding} will be returned if no such association exists.
     * @return binding
     */
	@QtUninvokable
	@Override
    public QCharPropertyBinding binding()
    {
		QUntypedPropertyBinding binding = super.binding();
		if(binding instanceof QCharPropertyBinding) {
			return (QCharPropertyBinding)binding;
		}else {
			return new QCharPropertyBinding(binding);
		}
    }
    
	@QtUninvokable
	public QCharPropertyBinding setBinding(QCharPropertyBinding binding)
    {
		QUntypedPropertyBinding oldBinding = super.overrideBinding(binding);
		if(oldBinding instanceof QCharPropertyBinding) {
			return (QCharPropertyBinding)oldBinding;
		}else {
			return new QCharPropertyBinding(oldBinding);
		}
    }
    
	@QtUninvokable
	public QCharPropertyBinding setBinding(QPropertyBinding<@QtPrimitiveType Character> binding)
    {
		if(binding!=null && !QCharProperty.checkType(binding.valueMetaType()))
			return new QCharPropertyBinding();
		QUntypedPropertyBinding oldBinding = super.overrideBinding(binding);
		if(oldBinding instanceof QCharPropertyBinding) {
			return (QCharPropertyBinding)oldBinding;
		}else {
			return new QCharPropertyBinding(oldBinding);
		}
    }
	
	@QtUninvokable
	public QCharPropertyBinding setBinding(io.qt.QtUtilities.CharSupplier functor)
    {
		return setBinding(new QCharPropertyBinding(functor));
    }
	
	/**
	 * <p>Disassociates the binding expression from this property and returns it.</p>
	 * <p>After calling this function, the value of the property will only change if you assign a new value to it, or when a new binding is set.</p>
	 * @return the removed binding
	 */
	@QtUninvokable
    public QCharPropertyBinding takeBinding()
    {
        return setBinding(new QCharPropertyBinding());
    }
    
	/**
	 * Returns the value of the underlying property. This may evaluate a binding expression that is tied to the property, before returning the value.
	 * @return value
	 */
    @io.qt.QtUninvokable
    public final char value() {
        return value(nativeId(this.iface()), nativeId(this.data()));
    }
    
	/**
	 * <p>Assigns newValue to the underlying property and removes the property's associated binding, if present.</p>
	 * @param newValue
	 */
    @io.qt.QtUninvokable
    public final void setValue(char value) {
        setValue(nativeId(this.iface()), nativeId(this.data()), value);
    }
    
    @io.qt.QtUninvokable
    static native char value(long ifaceId, long dataId);
    
    @io.qt.QtUninvokable
    static native void setValue(long ifaceId, long dataId, char value);
}
