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

import io.qt.QtPrimitiveType;
import io.qt.QtUninvokable;

/**
 * QByteProperty is primitive-typed version of QProperty&lt;Byte&gt;.
 * @see QProperty
 */
public final class QByteProperty extends QBytePropertyData {
	
	static boolean checkType(QMetaType metaType){
		return metaType!=null 
				&& (metaType.id()==QMetaType.Type.SChar.value()
					|| metaType.id()==QMetaType.Type.Char.value()
					|| metaType.id()==QMetaType.Type.UChar.value()
							|| metaType.id()==QMetaType.Type.QVariant.value());
	}

    /**
     * Constructs a <code>byte</code>-typed property.
     */
	public QByteProperty() {
		super();
	}

    /**
     * Constructs a <code>byte</code>-typed property with the provided initialValue.
     * @param initialValue
     */
	public QByteProperty(byte initialValue) {
		super(initialValue);
	}
	
    /**
     * Constructs a <code>byte</code>-typed property with the provided binding.
     * @param binding
     */
	public QByteProperty(QBytePropertyBinding binding) {
		super();
		d.setBinding(binding, this);
	}
	
    /**
     * Constructs a <code>byte</code>-typed property with the provided binding.
     * @param binding
     */
	public QByteProperty(QPropertyBinding<@QtPrimitiveType Byte> binding) {
		super();
		if(checkType(binding.valueMetaType()))
			d.setBinding(binding, this);
	}
	
    /**
     * <p>Constructs a <code>byte</code>-typed property bound to the provided <code>functor</code>.</p>
     * @param functor
     */
	public QByteProperty(io.qt.QtUtilities.ByteSupplier functor) {
		this(new QBytePropertyBinding(functor));
	}
	
	/**
	 * Returns the value of the property. This may evaluate a binding expression that is tied to this property, before returning the value.
	 * @return value
	 */
	@QtUninvokable
	public byte value()
    {
        if (d.hasBinding())
            d.evaluateIfDirty(this);
        d.registerWithCurrentlyEvaluatingBinding();
        return this.getValueBypassingBindings();
    }
	
	/**
	 * <p>Assigns newValue to this property and removes the property's associated binding, if present.</p>
	 * @param newValue
	 */
	@QtUninvokable
	public void setValue(byte newValue)
    {
        d.removeBinding();
        if (this.setValueBypassingBindings(newValue))
        	d.notifyObservers(this);
    }
	
	/**
	 * <p>Associates the value of this property with the provided <code>newBinding</code> expression and returns the previously associated binding.</p>
	 * <p>The first time the property value is read, the binding is evaluated.
	 * Whenever a dependency of the binding changes, the binding will be re-evaluated the next time the value of this property is read.</p>
	 * @param newBinding
	 * @return oldBinding
	 */
	@QtUninvokable
	public QBytePropertyBinding setBinding(QBytePropertyBinding newBinding)
    {
		QUntypedPropertyBinding oldBinding = d.setBinding(newBinding, this);
        d.notifyObservers(this);
        if(oldBinding instanceof QBytePropertyBinding)
        	return (QBytePropertyBinding)oldBinding;
        else
        	return new QBytePropertyBinding(oldBinding);
    }
	
	/**
	 * <p>Associates the value of this property with the provided <code>newBinding</code> expression and returns the previously associated binding.</p>
	 * <p>The binding's value type ({@link QUntypedPropertyBinding#valueMetaType()}) has to be <code>byte</code>,
	 * otherwise the property remains unchanged.</p>
	 * <p>The first time the property value is read, the binding is evaluated. 
	 * Whenever a dependency of the binding changes, the binding will be re-evaluated the next time the value of this property is read.</p>
	 * @param newBinding
	 * @return oldBinding
	 */
    @QtUninvokable
    public QBytePropertyBinding setBinding(QPropertyBinding<@QtPrimitiveType Byte> newBinding)
    {
        if(!io.qt.core.QByteProperty.checkType(newBinding.valueMetaType()))
            return new QBytePropertyBinding();
        QUntypedPropertyBinding oldBinding = d.setBinding(newBinding, this);
        d.notifyObservers(this);
        if(oldBinding instanceof QBytePropertyBinding)
            return (QBytePropertyBinding)oldBinding;
        else
            return new QBytePropertyBinding(oldBinding);
    }
    
	/**
	 * <p>Associates the value of this property with the provided <code>newBinding</code> expression.</p>
	 * <p>The binding's value type ({@link QUntypedPropertyBinding#valueMetaType()}) has to be <code>byte</code>, 
	 * otherwise the property remains unchanged and the method returns <code>false</code>.</p>
	 * <p>The first time the property value is read, the binding is evaluated. 
	 * Whenever a dependency of the binding changes, the binding will be re-evaluated the next time the value of this property is read.</p>
	 * <p>Returns true if the type of this property is the same as the type the binding function returns; false otherwise.</p>
	 * @param newBinding
	 * @return true if types match, false otherwise.
	 */
    @QtUninvokable
    public boolean setBinding(QUntypedPropertyBinding newBinding)
    {
        if(io.qt.core.QByteProperty.checkType(newBinding.valueMetaType())) {
            d.setBinding(newBinding, this);
            d.notifyObservers(this);
            return true;
        }else {
            return false;
        }
    }
	
	/**
	 * <p>Associates the value of this property with the provided <code>functor</code> and returns the previously associated binding.</p>
	 * <p>The first time the property value is read, the binding is evaluated by invoking {@link io.qt.QtUtilities.ByteSupplier#getAsByte()} of <code>functor</code>.
	 * Whenever a dependency of the binding changes, the binding will be re-evaluated the next time the value of this property is read.</p>
	 * @param functor
	 * @return oldBinding
	 */
	@QtUninvokable
	public QBytePropertyBinding setBinding(io.qt.QtUtilities.ByteSupplier functor)
    {
        return setBinding(new QBytePropertyBinding(functor));
    }
	
	@QtUninvokable
	private QBytePropertyBinding makeBinding()
    {
        return new QBytePropertyBinding(this::value);
    }
	
	/**
	 * Checks if the property has a binding.
	 * @return true if the property has a binding, false otherwise.
	 */
	@QtUninvokable
	public boolean hasBinding() { return d.hasBinding(); }
	
    /**
     * Returns the binding expression that is associated with this property. 
     * A default constructed {@link QBytePropertyBinding} will be returned if no such association exists.
     * @return binding
     */
	@QtUninvokable
	public QBytePropertyBinding binding()
    {
        return new QBytePropertyBinding(this);
    }
	
	/**
	 * <p>Disassociates the binding expression from this property and returns it.</p>
	 * <p>After calling this function, the value of the property will only change if you assign a new value to it, or when a new binding is set.</p>
	 * @return the removed binding
	 */
	@QtUninvokable
	public QBytePropertyBinding takeBinding()
    {
        return new QBytePropertyBinding(d.setBinding(new QBytePropertyBinding(), this));
    }
	
    /**
     * <p>Registers the given functor f as a callback that shall be called whenever the value of the property changes.</p>
     * <p>The returned property change handler object keeps track of the registration. 
     * As long as the change handler is alive i.e. as long as a reference to the {@link QPropertyChangeHandler} instance exists, 
     * the callback remains installed. When the garbage collection deletes the instance, the callback is de-registered.</p>
     * @param f
     * @return property change handler
     * @see QPropertyChangeHandler
     */
	@QtUninvokable
	public QPropertyChangeHandler onValueChanged(Runnable f)
    {
        return new QPropertyChangeHandler(d, f);
    }

	/**
	 * Subscribes the given functor f as a callback that is called immediately and whenever the value of the property changes in the future.
	 * @param f
	 * @return property change handler
     * @see QPropertyChangeHandler
     * @see #onValueChanged(Runnable)
	 */
	@QtUninvokable
	public QPropertyChangeHandler subscribe(Runnable f)
    {
        f.run();
        return onValueChanged(f);
    }
    
    /**
     * <p>Registers the given functor f as a callback that shall be called whenever the value of the bindable changes.</p>
     * <p>The returned property notifier object keeps track of the registration. 
     * As long as the notifier is alive i.e. as long as a reference to the {@link QPropertyNotifier} instance exists, 
     * the callback remains installed. When the garbage collection deletes the instance, the callback is de-registered.</p>
     * @param f
     * @return property notifier
     * @see QPropertyNotifier
     */
    @io.qt.QtUninvokable
    public QPropertyNotifier addNotifier(Runnable f)
    {
        return new QPropertyNotifier(d, f);
    }
	
	@QtUninvokable
	QPropertyBindingData bindingData() { return d; }
	
	private final QPropertyBindingData d = new QPropertyBindingData();
}
