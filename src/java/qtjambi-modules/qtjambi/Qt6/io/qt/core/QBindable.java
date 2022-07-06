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

import io.qt.NativeAccess;
import io.qt.QtPrimitiveType;
import io.qt.QtUninvokable;
import io.qt.QtUtilities;

/**
 * <p>Java wrapper for Qt class <a href="https://doc.qt.io/qt/qbindable.html">QBindable</a></p>
 * Use <code>QBindable</code> to make a <code>{@link QObject.QProperty}</code> field available as bindable <code>{@link QMetaProperty}</code>.
 * <p>Example:</p>
 * <code>
 * public class MyObject extends QObject{<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;private final QProperty&lt;QColor> color = new QProperty&lt;>();<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;public QColor color(){<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;return color.value();<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;}<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;public void setColor(QColor color){<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;color.setValue(color);<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;}<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;public QBindable&lt;QColor> bindableColor(){<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;return new QBindable&lt;>(color);<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;}<br>
 * }
 * </code>
 * <p>To let QtJambi's metaobject system identify a method as bindable getter 
 * the method meets following signature and naming convention:</p>
 * <ol>
 * <li>returns <code>{@link QUntypedBindable}</code> or a subtype of <code>QUntypedBindable</code></li>
 * <li>no parameters</li>
 * <li>method name is "<code>bindableFoo</code>" for an available property "<code>foo</code>"
 * whereas "<code>foo</code>" can be any name</li>
 * </ol>
 * <p>If the bindable getter's name does not meet the upper naming convention
 * use the {@link io.qt.QtPropertyBindable} annotation instead and specify the
 * property's name by the annotation's {@link io.qt.QtPropertyBindable#name()} attribute.</p>
 * <p>Alternatively, it is possible to declare a <code>{@link QObject.QProperty}</code> field <code>public</code> 
 * or by using the <code>{@link io.qt.QtPropertyMember}</code> annotation.
 * In this case, QBindable is supplied automatically:</p>
 * <code>
 * public class MyObject extends QObject{<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;@QtPropertyMember<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;private final QProperty&lt;QColor> color = new QProperty&lt;>();<br>
 * }
 * </code>
 * <p>For primitive-typed implementations of {@link QBindable} see:</p>
 * <ul>
 * <li>{@link QBooleanBindable}</li>
 * <li>{@link QByteBindable}</li>
 * <li>{@link QShortBindable}</li>
 * <li>{@link QIntBindable}</li>
 * <li>{@link QLongBindable}</li>
 * <li>{@link QFloatBindable}</li>
 * <li>{@link QDoubleBindable}</li>
 * <li>{@link QCharBindable}</li>
 * </ul>
 * 
 * @see io.qt.QtPropertyBindable
 * @see io.qt.core.QObject.QProperty
 * @see QMetaProperty#isBindable()
 * @see QMetaProperty#bindable(QObject)
 */
public final class QBindable<T> extends QUntypedBindable {
	
	/**
	 * Creates a new <code>Boolean</code>-typed generic <code>QBindable</code> from primitive-typed property.
	 * @param property
	 * @return <code>QBindable</code>
	 */
	public static QBindable<@QtPrimitiveType Boolean> fromProperty(QBooleanProperty property){
		return new QBindable<>(property, QBooleanBindable.bindableInterface(0));
	}
	
	/**
	 * Creates a new <code>Byte</code>-typed generic <code>QBindable</code> from primitive-typed property.
	 * @param property
	 * @return <code>QBindable</code>
	 */
	public static QBindable<@QtPrimitiveType Byte> fromProperty(QByteProperty property){
		return new QBindable<>(property, QByteBindable.bindableInterface(0));
	}
	
	/**
	 * Creates a new <code>Short</code>-typed generic <code>QBindable</code> from primitive-typed property.
	 * @param property
	 * @return <code>QBindable</code>
	 */
	public static QBindable<@QtPrimitiveType Short> fromProperty(QShortProperty property){
		return new QBindable<>(property, QShortBindable.bindableInterface(0));
	}
	
	/**
	 * Creates a new <code>Integer</code>-typed generic <code>QBindable</code> from primitive-typed property.
	 * @param property
	 * @return <code>QBindable</code>
	 */
	public static QBindable<@QtPrimitiveType Integer> fromProperty(QIntProperty property){
		return new QBindable<>(property, QIntBindable.bindableInterface(0));
	}
	
	/**
	 * Creates a new <code>Long</code>-typed generic <code>QBindable</code> from primitive-typed property.
	 * @param property
	 * @return <code>QBindable</code>
	 */
	public static QBindable<@QtPrimitiveType Long> fromProperty(QLongProperty property){
		return new QBindable<>(property, QLongBindable.bindableInterface(0));
	}
	
	/**
	 * Creates a new <code>Float</code>-typed generic <code>QBindable</code> from primitive-typed property.
	 * @param property
	 * @return <code>QBindable</code>
	 */
	public static QBindable<@QtPrimitiveType Float> fromProperty(QFloatProperty property){
		return new QBindable<>(property, QFloatBindable.bindableInterface(0));
	}
	
	/**
	 * Creates a new <code>Double</code>-typed generic <code>QBindable</code> from primitive-typed property.
	 * @param property
	 * @return <code>QBindable</code>
	 */
	public static QBindable<@QtPrimitiveType Double> fromProperty(QDoubleProperty property){
		return new QBindable<>(property, QDoubleBindable.bindableInterface(0));
	}
	
	/**
	 * Creates a new <code>Character</code>-typed generic <code>QBindable</code> from primitive-typed property.
	 * @param property
	 * @return <code>QBindable</code>
	 */
	public static QBindable<@QtPrimitiveType Character> fromProperty(QCharProperty property){
		return new QBindable<>(property, QCharBindable.bindableInterface(0));
	}
	
	/**
	 * Creates a new <code>Boolean</code>-typed generic <code>QBindable</code> from primitive-typed property.
	 * @param property
	 * @return <code>QBindable</code>
	 */
	public static QBindable<@QtPrimitiveType Boolean> fromProperty(QObject.QBooleanProperty property){
		return new QBindable<>(property, QBooleanBindable.bindableInterface(1));
	}
	
	/**
	 * Creates a new <code>Byte</code>-typed generic <code>QBindable</code> from primitive-typed property.
	 * @param property
	 * @return <code>QBindable</code>
	 */
	public static QBindable<@QtPrimitiveType Byte> fromProperty(QObject.QByteProperty property){
		return new QBindable<>(property, QByteBindable.bindableInterface(1));
	}
	
	/**
	 * Creates a new <code>Short</code>-typed generic <code>QBindable</code> from primitive-typed property.
	 * @param property
	 * @return <code>QBindable</code>
	 */
	public static QBindable<@QtPrimitiveType Short> fromProperty(QObject.QShortProperty property){
		return new QBindable<>(property, QShortBindable.bindableInterface(1));
	}
	
	/**
	 * Creates a new <code>Integer</code>-typed generic <code>QBindable</code> from primitive-typed property.
	 * @param property
	 * @return <code>QBindable</code>
	 */
	public static QBindable<@QtPrimitiveType Integer> fromProperty(QObject.QIntProperty property){
		return new QBindable<>(property, QIntBindable.bindableInterface(1));
	}
	
	/**
	 * Creates a new <code>Long</code>-typed generic <code>QBindable</code> from primitive-typed property.
	 * @param property
	 * @return <code>QBindable</code>
	 */
	public static QBindable<@QtPrimitiveType Long> fromProperty(QObject.QLongProperty property){
		return new QBindable<>(property, QLongBindable.bindableInterface(1));
	}
	
	/**
	 * Creates a new <code>Float</code>-typed generic <code>QBindable</code> from primitive-typed property.
	 * @param property
	 * @return <code>QBindable</code>
	 */
	public static QBindable<@QtPrimitiveType Float> fromProperty(QObject.QFloatProperty property){
		return new QBindable<>(property, QFloatBindable.bindableInterface(1));
	}
	
	/**
	 * Creates a new <code>Double</code>-typed generic <code>QBindable</code> from primitive-typed property.
	 * @param property
	 * @return <code>QBindable</code>
	 */
	public static QBindable<@QtPrimitiveType Double> fromProperty(QObject.QDoubleProperty property){
		return new QBindable<>(property, QDoubleBindable.bindableInterface(1));
	}
	
	/**
	 * Creates a new <code>Character</code>-typed generic <code>QBindable</code> from primitive-typed property.
	 * @param property
	 * @return <code>QBindable</code>
	 */
	public static QBindable<@QtPrimitiveType Character> fromProperty(QObject.QCharProperty property){
		return new QBindable<>(property, QCharBindable.bindableInterface(1));
	}
	
	/**
	 * Creates a new <code>Boolean</code>-typed generic <code>QBindable</code> from primitive-typed property.
	 * @param property
	 * @return <code>QBindable</code>
	 */
	public static QBindable<@QtPrimitiveType Boolean> fromProperty(QObject.QComputedBooleanProperty property){
		return new QBindable<>(property, QBooleanBindable.bindableInterface(2));
	}
	
	/**
	 * Creates a new <code>Byte</code>-typed generic <code>QBindable</code> from primitive-typed property.
	 * @param property
	 * @return <code>QBindable</code>
	 */
	public static QBindable<@QtPrimitiveType Byte> fromProperty(QObject.QComputedByteProperty property){
		return new QBindable<>(property, QByteBindable.bindableInterface(2));
	}
	
	/**
	 * Creates a new <code>Short</code>-typed generic <code>QBindable</code> from primitive-typed property.
	 * @param property
	 * @return <code>QBindable</code>
	 */
	public static QBindable<@QtPrimitiveType Short> fromProperty(QObject.QComputedShortProperty property){
		return new QBindable<>(property, QShortBindable.bindableInterface(2));
	}
	
	/**
	 * Creates a new <code>Integer</code>-typed generic <code>QBindable</code> from primitive-typed property.
	 * @param property
	 * @return <code>QBindable</code>
	 */
	public static QBindable<@QtPrimitiveType Integer> fromProperty(QObject.QComputedIntProperty property){
		return new QBindable<>(property, QIntBindable.bindableInterface(2));
	}
	
	/**
	 * Creates a new <code>Long</code>-typed generic <code>QBindable</code> from primitive-typed property.
	 * @param property
	 * @return <code>QBindable</code>
	 */
	public static QBindable<@QtPrimitiveType Long> fromProperty(QObject.QComputedLongProperty property){
		return new QBindable<>(property, QLongBindable.bindableInterface(2));
	}
	
	/**
	 * Creates a new <code>Float</code>-typed generic <code>QBindable</code> from primitive-typed property.
	 * @param property
	 * @return <code>QBindable</code>
	 */
	public static QBindable<@QtPrimitiveType Float> fromProperty(QObject.QComputedFloatProperty property){
		return new QBindable<>(property, QFloatBindable.bindableInterface(2));
	}
	
	/**
	 * Creates a new <code>Double</code>-typed generic <code>QBindable</code> from primitive-typed property.
	 * @param property
	 * @return <code>QBindable</code>
	 */
	public static QBindable<@QtPrimitiveType Double> fromProperty(QObject.QComputedDoubleProperty property){
		return new QBindable<>(property, QDoubleBindable.bindableInterface(2));
	}
	
	/**
	 * Creates a new <code>Character</code>-typed generic <code>QBindable</code> from primitive-typed property.
	 * @param property
	 * @return <code>QBindable</code>
	 */
	public static QBindable<@QtPrimitiveType Character> fromProperty(QObject.QComputedCharProperty property){
		return new QBindable<>(property, QCharBindable.bindableInterface(2));
	}
	
	/**
	 * Creates a new <code>Boolean</code>-typed generic <code>QBindable</code> from primitive-typed property.
	 * @param property
	 * @return <code>QBindable</code>
	 */
	public static QBindable<@QtPrimitiveType Boolean> fromProperty(QBooleanPropertyAlias property){
		return new QBindable<>(property.aliasedProperty(), property.iface);
	}
	
	/**
	 * Creates a new <code>Byte</code>-typed generic <code>QBindable</code> from primitive-typed property.
	 * @param property
	 * @return <code>QBindable</code>
	 */
	public static QBindable<@QtPrimitiveType Byte> fromProperty(QBytePropertyAlias property){
		return new QBindable<>(property.aliasedProperty(), property.iface);
	}
	
	/**
	 * Creates a new <code>Short</code>-typed generic <code>QBindable</code> from primitive-typed property.
	 * @param property
	 * @return <code>QBindable</code>
	 */
	public static QBindable<@QtPrimitiveType Short> fromProperty(QShortPropertyAlias property){
		return new QBindable<>(property.aliasedProperty(), property.iface);
	}
	
	/**
	 * Creates a new <code>Integer</code>-typed generic <code>QBindable</code> from primitive-typed property.
	 * @param property
	 * @return <code>QBindable</code>
	 */
	public static QBindable<@QtPrimitiveType Integer> fromProperty(QIntPropertyAlias property){
		return new QBindable<>(property.aliasedProperty(), property.iface);
	}
	
	/**
	 * Creates a new <code>Long</code>-typed generic <code>QBindable</code> from primitive-typed property.
	 * @param property
	 * @return <code>QBindable</code>
	 */
	public static QBindable<@QtPrimitiveType Long> fromProperty(QLongPropertyAlias property){
		return new QBindable<>(property.aliasedProperty(), property.iface);
	}
	
	/**
	 * Creates a new <code>Float</code>-typed generic <code>QBindable</code> from primitive-typed property.
	 * @param property
	 * @return <code>QBindable</code>
	 */
	public static QBindable<@QtPrimitiveType Float> fromProperty(QFloatPropertyAlias property){
		return new QBindable<>(property.aliasedProperty(), property.iface);
	}
	
	/**
	 * Creates a new <code>Double</code>-typed generic <code>QBindable</code> from primitive-typed property.
	 * @param property
	 * @return <code>QBindable</code>
	 */
	public static QBindable<@QtPrimitiveType Double> fromProperty(QDoublePropertyAlias property){
		return new QBindable<>(property.aliasedProperty(), property.iface);
	}
	
	/**
	 * Creates a new <code>Character</code>-typed generic <code>QBindable</code> from primitive-typed property.
	 * @param property
	 * @return <code>QBindable</code>
	 */
	public static QBindable<@QtPrimitiveType Character> fromProperty(QCharPropertyAlias property){
		return new QBindable<>(property.aliasedProperty(), property.iface);
	}

	private QBindable(QPrivateConstructor p) {
		super(p);
	}

	/**
	 * Creates a new invalid <code>QBindable</code>.
	 */
	public QBindable() {
		super(null, null);
	}
	
	/**
	 * Creates a new <code>QBindable</code> from provided property.
	 * @param property
	 */
	public QBindable(QProperty<T> property) {
		super(property.valueMetaType().isValid() ? property : null, property.valueMetaType().isValid() ? bindableInterface(property.valueMetaType().id(), 0) : null);
	}
	
	/**
	 * Creates a new <code>QBindable</code> from provided property.
	 * @param property
	 */
	public QBindable(QObject.QProperty<T> property) {
		super(property.valueMetaType().isValid() ? property : null, property.valueMetaType().isValid() ? bindableInterface(property.valueMetaType().id(), 1) : null);
	}
	
	/**
	 * Creates a new <code>QBindable</code> from provided property.
	 * @param property
	 */
	public QBindable(QObject.QComputedProperty<T> property) {
		super(property.valueMetaType().isValid() ? property : null, property.valueMetaType().isValid() ? bindableInterface(property.valueMetaType().id(), 2) : null);
	}
	
	/**
	 * Creates a new <code>QBindable</code> from provided property.
	 * @param property
	 */
	public QBindable(QPropertyAlias<T> property) {
		super(property.aliasedProperty(), property.iface);
	}
	
	static native QBindableInterface bindableInterface(int metaType, int propertyType);
	
	QBindable(QUntypedPropertyData d, QBindableInterface i) {
		super(d, i);
	}

	/**
	 * Creates a binding to the underlying property.
	 */
	@QtUninvokable
	@Override
	public QPropertyBinding<T> makeBinding()
    {
        return new QPropertyBinding<>(super.makeBinding());
    }
	
    /**
     * Returns the binding expression that is associated with the underlying property. 
     * A default constructed {@link QPropertyBinding} will be returned if no such association exists.
     * @return binding
     */
	@QtUninvokable
	@Override
    public QPropertyBinding<T> binding()
    {
        return new QPropertyBinding<>(super.binding());
    }
	
	/**
	 * <p>Disassociates the binding expression from the underlying property and returns it.</p>
	 * <p>After calling this function, the value of the property will only change if you assign a new value to it, or when a new binding is set.</p>
	 * @return the removed binding
	 */
	@QtUninvokable
    public QPropertyBinding<T> takeBinding()
    {
        return setBinding(new QPropertyBinding<>());
    }
    
	/**
	 * <p>Associates the value of the underlying property with the provided <code>newBinding</code> expression and returns the previously associated binding.</p>
	 * <p>The binding's value type ({@link QUntypedPropertyBinding#valueMetaType()}) has to be equals to the property's value type <code>T</code>, 
	 * otherwise the property remains unchanged.</p>
	 * <p>The first time the property value is read, the binding is evaluated. 
	 * Whenever a dependency of the binding changes, the binding will be re-evaluated the next time the value of the underlying property is read.</p>
	 * @param newBinding
	 * @return oldBinding
	 */
	@SuppressWarnings("unchecked")
	@QtUninvokable
	public QPropertyBinding<T> setBinding(QPropertyBinding<T> binding)
    {
		if(iface()!=null && binding!=null && !io.qt.core.QProperty.checkType(iface().metaType(), binding.valueMetaType()))
			return new QPropertyBinding<>();
		QUntypedPropertyBinding oldBinding = super.overrideBinding(binding);
		if(oldBinding instanceof QPropertyBinding) {
			return (QPropertyBinding<T>)oldBinding;
		}else {
			return new QPropertyBinding<>(oldBinding);
		}
    }
	
	/**
	 * <p>Associates the value of the underlying property with the provided <code>functor</code> and returns the previously associated binding.</p>
	 * <p>The first time the property value is read, the binding is evaluated by invoking {@link java.util.function.Supplier#get()} of <code>functor</code>.
	 * Whenever a dependency of the binding changes, the binding will be re-evaluated the next time the value of the underlying property is read.</p>
	 * @param functor
	 * @return oldBinding
	 */
	@QtUninvokable
	public QPropertyBinding<T> setBinding(QtUtilities.Supplier<T> functor)
    {
		try {
			QPropertyBinding.setPendingMetaType(this::metaType);
			return setBinding(new QPropertyBinding<>(functor));
		}finally {
			QPropertyBinding.setPendingMetaType(null);
		}
    }
	
	/**
	 * Returns the value of the underlying property. This may evaluate a binding expression that is tied to the property, before returning the value.
	 * @return value
	 */
	@io.qt.QtUninvokable
    public final T value() {
		return value(QtJambi_LibraryUtilities.internal.nativeId(this.iface()), QtJambi_LibraryUtilities.internal.nativeId(this.data()), 0);
	}
	
	/**
	 * <p>Assigns newValue to the underlying property and removes the property's associated binding, if present.</p>
	 * @param newValue
	 */
	@io.qt.QtUninvokable
    public final void setValue(T value) {
		setValue(QtJambi_LibraryUtilities.internal.nativeId(this.iface()), QtJambi_LibraryUtilities.internal.nativeId(this.data()), 0, value);
	}
    
    @io.qt.QtUninvokable
    static native <T> T value(long ifaceId, long dataId, long metaTypeId);
    
    @io.qt.QtUninvokable
    static native <T> boolean setValue(long ifaceId, long dataId, long metaTypeId, T value);

	QMetaType valueMetaType() {
		if(iface()!=null)
			return iface().metaType();
		else
			return new QMetaType();
	}
	
	@NativeAccess
	private static QUntypedBindable createBinding(QUntypedPropertyData property) {
		if(property instanceof QProperty) {
			return new QBindable<>((QProperty<?>)property);
		}else if(property instanceof QObject.QProperty) {
			return new QBindable<>((QObject.QProperty<?>)property);
		}else if(property instanceof QObject.QComputedProperty) {
			return new QBindable<>((QObject.QComputedProperty<?>)property);
		}else if(property instanceof QBooleanProperty) {
			return new QBooleanBindable((QBooleanProperty)property);
		}else if(property instanceof QObject.QBooleanProperty) {
			return new QBooleanBindable((QObject.QBooleanProperty)property);
		}else if(property instanceof QObject.QComputedBooleanProperty) {
			return new QBooleanBindable((QObject.QComputedBooleanProperty)property);
		}else if(property instanceof QByteProperty) {
			return new QByteBindable((QByteProperty)property);
		}else if(property instanceof QObject.QByteProperty) {
			return new QByteBindable((QObject.QByteProperty)property);
		}else if(property instanceof QObject.QComputedByteProperty) {
			return new QByteBindable((QObject.QComputedByteProperty)property);
		}else if(property instanceof QShortProperty) {
			return new QShortBindable((QShortProperty)property);
		}else if(property instanceof QObject.QShortProperty) {
			return new QShortBindable((QObject.QShortProperty)property);
		}else if(property instanceof QObject.QComputedShortProperty) {
			return new QShortBindable((QObject.QComputedShortProperty)property);
		}else if(property instanceof QIntProperty) {
			return new QIntBindable((QIntProperty)property);
		}else if(property instanceof QObject.QIntProperty) {
			return new QIntBindable((QObject.QIntProperty)property);
		}else if(property instanceof QObject.QComputedIntProperty) {
			return new QIntBindable((QObject.QComputedIntProperty)property);
		}else if(property instanceof QLongProperty) {
			return new QLongBindable((QLongProperty)property);
		}else if(property instanceof QObject.QLongProperty) {
			return new QLongBindable((QObject.QLongProperty)property);
		}else if(property instanceof QObject.QComputedLongProperty) {
			return new QLongBindable((QObject.QComputedLongProperty)property);
		}else if(property instanceof QFloatProperty) {
			return new QFloatBindable((QFloatProperty)property);
		}else if(property instanceof QObject.QFloatProperty) {
			return new QFloatBindable((QObject.QFloatProperty)property);
		}else if(property instanceof QObject.QComputedFloatProperty) {
			return new QFloatBindable((QObject.QComputedFloatProperty)property);
		}else if(property instanceof QDoubleProperty) {
			return new QDoubleBindable((QDoubleProperty)property);
		}else if(property instanceof QObject.QDoubleProperty) {
			return new QDoubleBindable((QObject.QDoubleProperty)property);
		}else if(property instanceof QObject.QComputedDoubleProperty) {
			return new QDoubleBindable((QObject.QComputedDoubleProperty)property);
		}else if(property instanceof QCharProperty) {
			return new QCharBindable((QCharProperty)property);
		}else if(property instanceof QObject.QCharProperty) {
			return new QCharBindable((QObject.QCharProperty)property);
		}else if(property instanceof QObject.QComputedCharProperty) {
			return new QCharBindable((QObject.QComputedCharProperty)property);
		}
		return null;
	}
}
