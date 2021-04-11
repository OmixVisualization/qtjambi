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

import java.util.Objects;

import io.qt.QNativePointer;
import io.qt.QtUninvokable;
import io.qt.QtUtilities;

/**
 * <p>Java wrapper for Qt class <a href="https://doc.qt.io/qt/qproperty.html">QProperty</a></p>
 * <p>The QProperty class enables automatic property bindings.</p>
 * <p>Example:</p>
 * <code>
 * QIntProperty red = new QIntProperty(255);<br>
 * QIntProperty green = new QIntProperty(0);<br>
 * QIntProperty blue = new QIntProperty(0);<br>
 * QIntProperty alpha = new QIntProperty(255);<br>
 * QProperty&lt;QColor> color = new QProperty&lt;>(QColor.class);<br>
 * color.setBinding(()->new QColor(red.value(), green.value(), blue.value(), alpha.value()));<br>
 * color.value(); &#47;&#47; = red<br>
 * red.setValue(0);<br>
 * green.setValue(255);<br>
 * color.value(); &#47;&#47; = green<br>
 * </code>
 * <p>For primitive-typed implementations see:</p>
 * <ul>
 * <li>{@link QBooleanProperty}</li>
 * <li>{@link QByteProperty}</li>
 * <li>{@link QShortProperty}</li>
 * <li>{@link QIntProperty}</li>
 * <li>{@link QLongProperty}</li>
 * <li>{@link QFloatProperty}</li>
 * <li>{@link QDoubleProperty}</li>
 * <li>{@link QCharProperty}</li>
 * </ul>
 */
public final class QProperty<T> extends QPropertyData<T> {
	
	private static class TypedPropertyBindingData extends QPropertyBindingData{
		TypedPropertyBindingData(QMetaType metaType) {
			super();
			this.metaType = metaType;
		}
		@QtUninvokable
		public boolean setValueBypassingBindings(QProperty<?> property, Object val) {
			return QProperty.setValueBypassingBindings(nativeId(property), nativeId(this.metaType), val);
		}
		final QMetaType metaType;
	}
	
	private static class RCTypedPropertyBindingData extends TypedPropertyBindingData{
		RCTypedPropertyBindingData(QMetaType metaType, Object val) {
			super(metaType);
			__rcValue = val;
		}
		@Override
		public boolean setValueBypassingBindings(QProperty<?> property, Object val) {
			if(super.setValueBypassingBindings(property, val)) {
				__rcValue = val;
				return true;
			}
			else return false;
		}
		@SuppressWarnings("unused")
		private Object __rcValue;
	}
	
	private static QMetaType valueMetaType(Object val) {
		if(val instanceof QList) {
			return QMetaType.fromType(QList.class, ((QList<?>) val).elementMetaType());
		}else if(val instanceof QSet) {
			return QMetaType.fromType(QSet.class, ((QSet<?>) val).elementMetaType());
		}else if(val instanceof QQueue) {
			return QMetaType.fromType(QQueue.class, ((QQueue<?>) val).elementMetaType());
		}else if(val instanceof QStack) {
			return QMetaType.fromType(QStack.class, ((QStack<?>) val).elementMetaType());
		}else if(val instanceof QMap) {
			return QMetaType.fromType(QMap.class, ((QMap<?,?>) val).keyMetaType(), ((QMap<?,?>) val).valueMetaType());
		}else if(val instanceof QHash) {
			return QMetaType.fromType(QHash.class, ((QHash<?,?>) val).keyMetaType(), ((QHash<?,?>) val).valueMetaType());
		}else if(val instanceof QMultiMap) {
			return QMetaType.fromType(QMultiMap.class, ((QMultiMap<?,?>) val).keyMetaType(), ((QMultiMap<?,?>) val).valueMetaType());
		}else if(val instanceof QMultiHash) {
			return QMetaType.fromType(QMultiHash.class, ((QMultiHash<?,?>) val).keyMetaType(), ((QMultiHash<?,?>) val).valueMetaType());
		}else if(val instanceof QPair) {
			return QMetaType.fromType(QPair.class, valueMetaType(((QPair<?,?>) val).first), valueMetaType(((QPair<?,?>) val).second));
		}else if(val instanceof Integer) {
			return new QMetaType(QMetaType.Type.Int);
		}else if(val instanceof Long) {
			return new QMetaType(QMetaType.Type.LongLong);
		}else if(val instanceof Short) {
			return new QMetaType(QMetaType.Type.Short);
		}else if(val instanceof Byte) {
			return new QMetaType(QMetaType.Type.SChar);
		}else if(val instanceof Double) {
			return new QMetaType(QMetaType.Type.Double);
		}else if(val instanceof Float) {
			return new QMetaType(QMetaType.Type.Float);
		}else if(val instanceof Character) {
			return new QMetaType(QMetaType.Type.QChar);
		}else if(val instanceof Boolean) {
			return new QMetaType(QMetaType.Type.Bool);
		}else if(val instanceof QNativePointer) {
			return new QMetaType(QMetaType.Type.VoidStar);
		}else {
			return QMetaType.fromType(val.getClass());
		}
	}

	/**
     * Constructs a property with the given type and initialValue.
     * @param initialValue
     * @param type class type
     * @param instantiations optional instantiations for container classes like {@link QList} and {@link QMap}
     */
	public QProperty(T initialValue, Class<T> type, QMetaType... instantiations) {
		super(null);
		QMetaType metaType = QMetaType.fromType(type, instantiations);
		if(metaType.flags().isSet(QMetaType.TypeFlag.IsPointer) || metaType.name().contains("*")) {
			d = new RCTypedPropertyBindingData(metaType, initialValue);
		}else {
			d = new TypedPropertyBindingData(metaType);
		}
		__qt_new(this, metaType, initialValue);
	}

    /**
     * Constructs a property with the given type.
     * @param type class type
     * @param instantiations optional instantiations for container classes like {@link QList} and {@link QMap}
     */
	public QProperty(Class<T> type, QMetaType... instantiations) {
		this(null, type, instantiations);
	}
	
    /**
     * Constructs a property with the provided initialValue. The property type is extracted from <code>initialValue</code>.
     * @param initialValue initial value must not be <code>null</code>
     */
	public QProperty(T initialValue) {
		super(null);
		QMetaType metaType = valueMetaType(Objects.requireNonNull(initialValue));
		if(metaType.flags().isSet(QMetaType.TypeFlag.IsPointer) || metaType.name().contains("*")) {
			d = new RCTypedPropertyBindingData(metaType, initialValue);
		}else {
			d = new TypedPropertyBindingData(metaType);
		}
		__qt_new(this, metaType, initialValue);
	}
	
    /**
     * Constructs a <code>QVariant</code>-typed property.
     */
	public QProperty() {
		super(null);
		QMetaType metaType = new QMetaType(QMetaType.Type.QVariant);
		d = new TypedPropertyBindingData(metaType);
		__qt_new(this, metaType, null);
	}
	
    /**
     * Constructs a property with the provided binding. The property type corresponds to the type of <code>binding</code>.
     * @param binding must not be <code>null</code>
     */
	public QProperty(QPropertyBinding<T> binding) {
		super(null);
		QMetaType metaType = binding.valueMetaType();
		if(metaType.flags().isSet(QMetaType.TypeFlag.IsPointer) || metaType.name().contains("*")) {
			d = new RCTypedPropertyBindingData(metaType, null);
		}else {
			d = new TypedPropertyBindingData(metaType);
		}
		__qt_new(this, metaType, null);
		d.setBinding(binding, this);
	}
	
    /**
     * <p>Constructs a property bound to the provided <code>functor</code>.</p>
     * <p>The property type corresponds to the return type of the <code>functor</code>'s {@link QtUtilities.Supplier#get()}.</p>
     * @param functor
     */
	public QProperty(QtUtilities.Supplier<T> functor) {
		this(new QPropertyBinding<>(functor));
	}
	
	/**
	 * Returns the value of the property. This may evaluate a binding expression that is tied to this property, before returning the value.
	 * @return value
	 */
	@QtUninvokable
	public T value()
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
	public void setValue(T newValue)
    {
		if(newValue!=null && !QVariant.canConvert(newValue, d.metaType))
			throw new ClassCastException("Cannot cast value to "+d.metaType.name());
        d.removeBinding();
        if (d.setValueBypassingBindings(this, newValue))
        	d.notifyObservers(this);
    }
	
	@QtUninvokable
	static boolean checkType(QMetaType myMetaType, QMetaType metaType) {
    	if(myMetaType.sizeOf()==1							// byte
				&& (myMetaType.id()==QMetaType.Type.Char.value()
					|| myMetaType.id()==QMetaType.Type.SChar.value()
					|| myMetaType.id()==QMetaType.Type.UChar.value())) {
			if(metaType.sizeOf()!=1
					|| (metaType.id()!=QMetaType.Type.SChar.value()
						&& metaType.id()!=QMetaType.Type.Char.value()
						&& metaType.id()!=QMetaType.Type.UChar.value())) {
				return false;
			}
		} else if(myMetaType.sizeOf()==2					// char
				&& (myMetaType.id()==QMetaType.Type.QChar.value()
					|| myMetaType.id()==QMetaType.Type.Char16.value()
					|| myMetaType.id()==QMetaType.Type.Short.value()
					|| myMetaType.id()==QMetaType.Type.UShort.value())) {
			if(metaType.sizeOf()!=2
					|| (metaType.id()!=QMetaType.Type.QChar.value()
						&& metaType.id()!=QMetaType.Type.Char16.value()
						&& metaType.id()!=QMetaType.Type.Short.value()
						&& metaType.id()!=QMetaType.Type.UShort.value())) {
				return false;
			}
		} else if(myMetaType.sizeOf()==4					// int
				&& (myMetaType.id()==QMetaType.Type.Int.value()
					|| myMetaType.id()==QMetaType.Type.Long.value()
					|| myMetaType.id()==QMetaType.Type.UInt.value()
					|| myMetaType.id()==QMetaType.Type.ULong.value()
					|| myMetaType.id()==QMetaType.Type.Char32.value())) {
			if(metaType.sizeOf()!=4
					|| (metaType.id()!=QMetaType.Type.Int.value()
						&& metaType.id()!=QMetaType.Type.Long.value()
						&& metaType.id()!=QMetaType.Type.ULong.value()
						&& metaType.id()!=QMetaType.Type.UInt.value()
						&& metaType.id()!=QMetaType.Type.Char32.value())) {
				return false;
			}
		} else if(myMetaType.sizeOf()==8					// long
				&& (myMetaType.id()==QMetaType.Type.LongLong.value()
					|| myMetaType.id()==QMetaType.Type.Long.value()
					|| myMetaType.id()==QMetaType.Type.ULongLong.value()
					|| myMetaType.id()==QMetaType.Type.ULong.value())) {
			if(metaType.sizeOf()!=8
					|| (metaType.id()!=QMetaType.Type.LongLong.value()
						&& metaType.id()!=QMetaType.Type.Long.value()
						&& metaType.id()!=QMetaType.Type.ULong.value()
						&& metaType.id()!=QMetaType.Type.ULongLong.value())) {
				return false;
			}
		} else if(myMetaType.sizeOf()==4					// float
				&& (myMetaType.id()==QMetaType.Type.Double.value()
					|| myMetaType.id()==QMetaType.Type.Float.value())) {
			if(metaType.sizeOf()!=4
					|| (metaType.id()!=QMetaType.Type.Double.value()
						&& metaType.id()!=QMetaType.Type.Float.value())) {
				return false;
			}
		} else if(myMetaType.sizeOf()==8					// double
				&& (myMetaType.id()==QMetaType.Type.Double.value()
					|| myMetaType.id()==QMetaType.Type.Float.value())) {
			if(metaType.sizeOf()!=8
					|| (metaType.id()!=QMetaType.Type.Double.value()
						&& metaType.id()!=QMetaType.Type.Float.value())) {
				return false;
			}
		} else if(!myMetaType.equals(metaType)) {
			Class<?> myType = myMetaType.javaType();
			if(myType==null || metaType.javaType()==null || !myType.isAssignableFrom(metaType.javaType())) {
				return false;
			}
		}
    	return true;
	}
	
	/**
	 * <p>Associates the value of this property with the provided <code>newBinding</code> expression and returns the previously associated binding.</p>
	 * <p>The binding's value type ({@link QUntypedPropertyBinding#valueMetaType()}) has to be equals to the property's value type <code>T</code>, 
	 * otherwise the property remains unchanged.</p>
	 * <p>The first time the property value is read, the binding is evaluated. 
	 * Whenever a dependency of the binding changes, the binding will be re-evaluated the next time the value of this property is read.</p>
	 * @param newBinding
	 * @return oldBinding
	 */
	@SuppressWarnings("unchecked")
	@QtUninvokable
    public QPropertyBinding<T> setBinding(QPropertyBinding<T> newBinding)
    {
		if(newBinding!=null && !checkType(valueMetaType(), newBinding.valueMetaType()))
			return new QPropertyBinding<>();
        QPropertyBindingData bd = bindingData();
        QUntypedPropertyBinding oldBinding = bd.setBinding(newBinding, this);
        d.notifyObservers(this);
        if(oldBinding instanceof QPropertyBinding)
        	return (QPropertyBinding<T>)oldBinding;
        else
        	return new QPropertyBinding<>(oldBinding);
    }
	
	/**
	 * <p>Associates the value of this property with the provided <code>newBinding</code> expression.</p>
	 * <p>The binding's value type ({@link QUntypedPropertyBinding#valueMetaType()}) has to be equals to the property's value type <code>T</code>, 
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
		if(newBinding!=null && !checkType(valueMetaType(), newBinding.valueMetaType()))
			return false;
        d.setBinding(newBinding, this);
        d.notifyObservers(this);
        return true;
    }
	
	/**
	 * <p>Associates the value of this property with the provided <code>functor</code> and returns the previously associated binding.</p>
	 * <p>The first time the property value is read, the binding is evaluated by invoking {@link QtUtilities.Supplier#get()} of <code>functor</code>.
	 * Whenever a dependency of the binding changes, the binding will be re-evaluated the next time the value of this property is read.</p>
	 * @param functor
	 * @return oldBinding
	 */
	@QtUninvokable
	public QPropertyBinding<T> setBinding(QtUtilities.Supplier<T> functor)
    {
        return setBinding(new QPropertyBinding<>(functor));
    }
	
	@QtUninvokable
	private QPropertyBinding<T> makeBinding()
    {
        return new QPropertyBinding<>(this::value);
    }
	
	/**
	 * Checks if the property has a binding.
	 * @return true if the property has a binding, false otherwise.
	 */
	@QtUninvokable
	public boolean hasBinding() { return d.hasBinding(); }
	
    /**
     * Returns the binding expression that is associated with this property. 
     * A default constructed {@link QPropertyBinding&lt;T>} will be returned if no such association exists.
     * @return binding
     */
	@QtUninvokable
	public QPropertyBinding<T> binding()
    {
        return new QPropertyBinding<>(this);
    }
	
	/**
	 * <p>Disassociates the binding expression from this property and returns it.</p>
	 * <p>After calling this function, the value of the property will only change if you assign a new value to it, or when a new binding is set.</p>
	 * @return the removed binding
	 */
	@QtUninvokable
	public QPropertyBinding<T> takeBinding()
    {
        return new QPropertyBinding<>(d.setBinding(new QPropertyBinding<>(), this));
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
	
	@QtUninvokable
	QPropertyBindingData bindingData() { return d; }
	
	QMetaType valueMetaType() {
		return d.metaType;
	}
	
	private final TypedPropertyBindingData d;
	
	native static void __qt_new(Object instance, QMetaType metaType, Object val);

	/**
	 * {@inheritDoc}
	 */
	@QtUninvokable
    @Override
	public final T getValueBypassingBindings() {
		return getValueBypassingBindings(nativeId(this), nativeId(d.metaType));
	}
	
	@QtUninvokable
	static native <T> T getValueBypassingBindings(long this_id, long metaTypeId);
	
	/**
	 * {@inheritDoc}
	 */
	@QtUninvokable
    @Override
	public final boolean setValueBypassingBindings(T val) {
		return d.setValueBypassingBindings(this, val);
	}
	
	@QtUninvokable
	static native boolean setValueBypassingBindings(long this_id, long metaTypeId, Object val);
}
