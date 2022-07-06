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

import java.util.Collection;
import java.util.Map;
import java.util.Objects;

import io.qt.NativeAccess;
import io.qt.QFlags;
import io.qt.QNoImplementationException;
import io.qt.QNoNativeResourcesException;
import io.qt.QtByteEnumerator;
import io.qt.QtEnumerator;
import io.qt.QtLongEnumerator;
import io.qt.QtShortEnumerator;
import io.qt.QtUninvokable;
import io.qt.internal.QtJambiInternal;

/**
 * <p>Java wrapper for Qt class <a href="https://doc.qt.io/qt/qmap.html">QMap</a></p>
 */
public class QMap<K,V> extends io.qt.internal.QtJambiMapObject<K,V> implements Cloneable
{
	static {
    	QtJambi_LibraryUtilities.initialize();
    }

    @NativeAccess
    protected QMap(QPrivateConstructor p) {
		super(p);
	}
	
	public QMap(Class<K> keyType, QMetaType.Type valueMetaType) {
		this(keyType, new QMetaType(valueMetaType));
	}
	
	public QMap(QMetaType.Type keyMetaType, Class<V> valueType) {
		this(new QMetaType(keyMetaType), valueType);
	}
	
	public QMap(QMetaType.Type keyMetaType, QMetaType valueMetaType) {
		this(new QMetaType(keyMetaType), valueMetaType);
	}
	
	public QMap(QMetaType keyMetaType, QMetaType.Type valueMetaType) {
		this(keyMetaType, new QMetaType(valueMetaType));
	}
	
	public QMap(QMetaType.Type keyMetaType, QMetaType.Type valueMetaType) {
		this(new QMetaType(keyMetaType), new QMetaType(valueMetaType));
	}
    
    public QMap(Class<K> keyType, Class<V> valueType) {
		super(null);
		QMetaType keyMetaType = QMetaType.fromType(keyType);
		QMetaType valueMetaType = QMetaType.fromType(valueType);
		if(keyMetaType.id()==0)
			throw new IllegalArgumentException("QMetaType::UnknownType cannot be key type of QMap.");
		if(keyMetaType.id()==QMetaType.Type.Void.value())
			throw new IllegalArgumentException("void cannot be key type of QMap.");
		if(valueMetaType.id()==0)
			throw new IllegalArgumentException("QMetaType::UnknownType cannot be value type of QMap.");
		if(valueMetaType.id()==QMetaType.Type.Void.value())
			throw new IllegalArgumentException("void cannot be value type of QMap.");
		initialize(keyType, QtJambi_LibraryUtilities.internal.nativeId(keyMetaType), valueType, QtJambi_LibraryUtilities.internal.nativeId(valueMetaType), null);
	}
    
    public QMap(Map<K,V> other) {
		super(null);
		QPair<QMetaType, QMetaType> metaTypes = QMap.findMapMetaType(Objects.requireNonNull(other));
		if(metaTypes.first.id()==0)
			throw new IllegalArgumentException("QMetaType::UnknownType cannot be key type of QMap.");
		if(metaTypes.first.id()==QMetaType.Type.Void.value())
			throw new IllegalArgumentException("void cannot be key type of QMap.");
		if(metaTypes.second.id()==0)
			throw new IllegalArgumentException("QMetaType::UnknownType cannot be value type of QMap.");
		if(metaTypes.second.id()==QMetaType.Type.Void.value())
			throw new IllegalArgumentException("void cannot be value type of QMap.");
		initialize(metaTypes.first.javaType(), QtJambi_LibraryUtilities.internal.nativeId(metaTypes.first), metaTypes.second.javaType(), QtJambi_LibraryUtilities.internal.nativeId(metaTypes.second), other);
	}
    
	public QMap(Class<K> keyType, QMetaType valueMetaType) {
		super(null);
		QMetaType keyMetaType = QMetaType.fromType(keyType);
		if(keyMetaType.id()==0)
			throw new IllegalArgumentException("QMetaType::UnknownType cannot be key type of QMap.");
		if(keyMetaType.id()==QMetaType.Type.Void.value())
			throw new IllegalArgumentException("void cannot be key type of QMap.");
		if(valueMetaType.id()==0)
			throw new IllegalArgumentException("QMetaType::UnknownType cannot be value type of QMap.");
		if(valueMetaType.id()==QMetaType.Type.Void.value())
			throw new IllegalArgumentException("void cannot be value type of QMap.");
		initialize(keyType, QtJambi_LibraryUtilities.internal.nativeId(keyMetaType), valueMetaType.javaType(), QtJambi_LibraryUtilities.internal.nativeId(valueMetaType), null);
	}
    
	public QMap(QMetaType keyMetaType, Class<V> valueType) {
		super(null);
		QMetaType valueMetaType = QMetaType.fromType(valueType);
		if(keyMetaType.id()==0)
			throw new IllegalArgumentException("QMetaType::UnknownType cannot be key type of QMap.");
		if(keyMetaType.id()==QMetaType.Type.Void.value())
			throw new IllegalArgumentException("void cannot be key type of QMap.");
		if(valueMetaType.id()==0)
			throw new IllegalArgumentException("QMetaType::UnknownType cannot be value type of QMap.");
		if(valueMetaType.id()==QMetaType.Type.Void.value())
			throw new IllegalArgumentException("void cannot be value type of QMap.");
		initialize(keyMetaType.javaType(), QtJambi_LibraryUtilities.internal.nativeId(keyMetaType), valueType, QtJambi_LibraryUtilities.internal.nativeId(valueMetaType), null);
	}
    
	public QMap(QMetaType keyMetaType, QMetaType valueMetaType) {
		super(null);
		if(keyMetaType.id()==0)
			throw new IllegalArgumentException("QMetaType::UnknownType cannot be key type of QMap.");
		if(keyMetaType.id()==QMetaType.Type.Void.value())
			throw new IllegalArgumentException("void cannot be key type of QMap.");
		if(valueMetaType.id()==0)
			throw new IllegalArgumentException("QMetaType::UnknownType cannot be value type of QMap.");
		if(valueMetaType.id()==QMetaType.Type.Void.value())
			throw new IllegalArgumentException("void cannot be value type of QMap.");
		initialize(keyMetaType.javaType(), QtJambi_LibraryUtilities.internal.nativeId(keyMetaType), valueMetaType.javaType(), QtJambi_LibraryUtilities.internal.nativeId(valueMetaType), null);
	}
    
    static boolean checkContainerType(QMetaType keyMetaType, QMetaType valueMetaType, java.util.Map<?, ?> container) {
		if (container instanceof QMap 
				&& keyMetaType.equals(((QMap<?, ?>) container).keyMetaType())
				&& valueMetaType.equals(((QMap<?, ?>) container).valueMetaType())) {
			return true;
		} else if (container instanceof QMultiMap 
				&& keyMetaType.equals(((QMultiMap<?, ?>) container).keyMetaType())
				&& valueMetaType.equals(((QMultiMap<?, ?>) container).valueMetaType())) {
			return true;
		} else if (container instanceof QHash
				&& keyMetaType.equals(((QHash<?, ?>) container).keyMetaType())
				&& valueMetaType.equals(((QHash<?, ?>) container).valueMetaType())) {
			return true;
		} else if (container instanceof QMultiHash
				&& keyMetaType.equals(((QMultiHash<?, ?>) container).keyMetaType())
				&& valueMetaType.equals(((QMultiHash<?, ?>) container).valueMetaType())) {
			return true;
		} else {
			Class<?> keyType = keyMetaType.javaType();
			Class<?> valueType = valueMetaType.javaType();
			if (keyType.isPrimitive()) {
				if (keyType == int.class)
					keyType = Integer.class;
				if (keyType == short.class)
					keyType = Short.class;
				if (keyType == byte.class)
					keyType = Byte.class;
				if (keyType == long.class)
					keyType = Long.class;
				if (keyType == float.class)
					keyType = Float.class;
				if (keyType == double.class)
					keyType = Double.class;
				if (keyType == boolean.class)
					keyType = Boolean.class;
				if (keyType == char.class)
					keyType = Character.class;
			}
			if (valueType.isPrimitive()) {
				if (valueType == int.class)
					valueType = Integer.class;
				if (valueType == short.class)
					valueType = Short.class;
				if (valueType == byte.class)
					valueType = Byte.class;
				if (valueType == long.class)
					valueType = Long.class;
				if (valueType == float.class)
					valueType = Float.class;
				if (valueType == double.class)
					valueType = Double.class;
				if (valueType == boolean.class)
					valueType = Boolean.class;
				if (valueType == char.class)
					valueType = Character.class;
			}
			for (Map.Entry<?, ?> entry : container.entrySet()) {
				if (entry.getKey() != null && !keyType.isInstance(entry.getKey()))
					return false;
				if (entry.getValue() != null && !valueType.isInstance(entry.getValue()))
					return false;
			}
		}
		return true;
	}
    
    public static QMap<String,Object> createVariantMap() {
    	return new QMap<>(new QMetaType(QMetaType.Type.QString), new QMetaType(QMetaType.Type.QVariant));
    }
    
    @QtUninvokable
    private native void initialize(Class<?> keyType, long keyMetaType, Class<?> valueType, long valueMetaType, Map<K, V> other);
    
    @Override
	public QMap<K,V> clone(){
		return new QMap<>(this);
	}
    
	@QtUninvokable
    protected final io.qt.core.QMapIterator<K,V> begin()    {
        return __qt_QMap_begin(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    static native <K,V> io.qt.core.QMapIterator<K,V> __qt_QMap_begin(long __this__nativeId);

    @QtUninvokable
    public final void clear()    {
        __qt_QMap_clear(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    static native void __qt_QMap_clear(long __this__nativeId);

    @QtUninvokable
    public final boolean contains(K key)    {
        try{
        	return __qt_QMap_contains(QtJambi_LibraryUtilities.internal.nativeId(this), key);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QSet.handleException(e, keyMetaType(), key);
        }
    }
    @QtUninvokable
    static native <K> boolean __qt_QMap_contains(long __this__nativeId, K key);

    @QtUninvokable
    public final int count()    {
        return size();
    }

    @QtUninvokable
    public final int count(K key)    {
        try{
        	return __qt_QMap_count(QtJambi_LibraryUtilities.internal.nativeId(this), key);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QSet.handleException(e, keyMetaType(), key);
        }
    }
    @QtUninvokable
    static native <K> int __qt_QMap_count(long __this__nativeId, K key);

    @QtUninvokable
    protected final io.qt.core.QMapIterator<K,V> end()    {
        return __qt_QMap_end(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    static native <K,V> io.qt.core.QMapIterator<K,V> __qt_QMap_end(long __this__nativeId);

    @QtUninvokable
    public final io.qt.core.QMapIterator<K,V> find(K key)    {
        try{
        	return __qt_QMap_find(QtJambi_LibraryUtilities.internal.nativeId(this), key);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QSet.handleException(e, keyMetaType(), key);
        }
    }
    @QtUninvokable
    static native <K,V> io.qt.core.QMapIterator<K,V> __qt_QMap_find(long __this__nativeId, K key);

    @QtUninvokable
    public final V first()    {
        return __qt_QMap_first(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    static native <V> V __qt_QMap_first(long __this__nativeId);

    @QtUninvokable
    public final K firstKey()    {
        return __qt_QMap_firstKey(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    static native <K> K __qt_QMap_firstKey(long __this__nativeId);

    @QtUninvokable
    public final void insert(K key, V value)    {
        try{
        	__qt_QMap_insert(QtJambi_LibraryUtilities.internal.nativeId(this), key, value);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QMap.handleException(e, keyMetaType(), valueMetaType(), key, value);
        }
    }
    @QtUninvokable
    static native <K,V> void __qt_QMap_insert(long __this__nativeId, K key, V value);

    @QtUninvokable
    public final boolean isEmpty()    {
        return size()==0;
    }

    @QtUninvokable
    public final K key(V value) {
        return key(value, null);
    }
    @QtUninvokable
    public final K key(V value, K defaultKey)    {
        try{
        	return __qt_QMap_key(QtJambi_LibraryUtilities.internal.nativeId(this), value, defaultKey);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QSet.handleException(e, valueMetaType(), value);
        }
    }
    @QtUninvokable
    static native <K,V> K __qt_QMap_key(long __this__nativeId, V value, K defaultKey);

    @QtUninvokable
    public final java.util.List<K> keys()    {
        return __qt_QMap_keys(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    static native <K> java.util.List<K> __qt_QMap_keys(long __this__nativeId);

    @QtUninvokable
    public final java.util.List<K> keys(V value)    {
        try{
        	return __qt_QMap_keysForValue(QtJambi_LibraryUtilities.internal.nativeId(this), value);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QSet.handleException(e, valueMetaType(), value);
        }
    }
    @QtUninvokable
    static native <K,V> java.util.List<K> __qt_QMap_keysForValue(long __this__nativeId, V value);

    @QtUninvokable
    public final V last()    {
        return __qt_QMap_last(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    static native <V> V __qt_QMap_last(long __this__nativeId);

    @QtUninvokable
    public final K lastKey()    {
        return __qt_QMap_lastKey(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    static native <K> K __qt_QMap_lastKey(long __this__nativeId);

    @QtUninvokable
    public final io.qt.core.QMapIterator<K,V> lowerBound(K key)    {
        try{
        	return __qt_QMap_lowerBound(QtJambi_LibraryUtilities.internal.nativeId(this), key);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QSet.handleException(e, keyMetaType(), key);
        }
    }
    @QtUninvokable
    static native <K,V> io.qt.core.QMapIterator<K,V> __qt_QMap_lowerBound(long __this__nativeId, K key);

    @QtUninvokable
    private final boolean operator_equal(java.util.Map<K,V> other)    {
        return __qt_QMap_operator_equal(QtJambi_LibraryUtilities.internal.nativeId(this), other);
    }
    @QtUninvokable
    static native <K,V> boolean __qt_QMap_operator_equal(long __this__nativeId, java.util.Map<K,V> other);

    @QtUninvokable
    public final int removeAll(Object key)    {
        try{
        	return __qt_QMap_remove(QtJambi_LibraryUtilities.internal.nativeId(this), key);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(IllegalArgumentException e) {
    		return 0;
    	}catch(RuntimeException e) {
    		RuntimeException e1 = QSet.handleException(e, keyMetaType(), key);
    		if(e1==e)
    			throw e;
    		return 0;
    	}
    }
    @QtUninvokable
    static native <K,V> int __qt_QMap_remove(long __this__nativeId, K key);

    @QtUninvokable
    public final int size()    {
        return __qt_QMap_size(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    static native int __qt_QMap_size(long __this__nativeId);

    @QtUninvokable
    public final V take(K key)    {
    	try {
    		return __qt_QMap_take(QtJambi_LibraryUtilities.internal.nativeId(this), key);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QSet.handleException(e, keyMetaType(), key);
    	}
    }
    @QtUninvokable
    static native <K,V> V __qt_QMap_take(long __this__nativeId, K key);

    @QtUninvokable
    public final io.qt.core.QMapIterator<K,V> upperBound(K key)    {
        try{
        	return __qt_QMap_upperBound(QtJambi_LibraryUtilities.internal.nativeId(this), key);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QSet.handleException(e, keyMetaType(), key);
        }
    }
    @QtUninvokable
    static native <K,V> io.qt.core.QMapIterator<K,V> __qt_QMap_upperBound(long __this__nativeId, K key);


    @QtUninvokable
    public final V value(K key) {
        return value(key, null);
    }
    @QtUninvokable
    public final V value(K key, V defaultValue)    {
        try{
        	return __qt_QMap_value(QtJambi_LibraryUtilities.internal.nativeId(this), key, defaultValue);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QMap.handleException(e, keyMetaType(), valueMetaType(), key, defaultValue);
        }
    }
    @QtUninvokable
    static native <K,V> V __qt_QMap_value(long __this__nativeId, K key, V defaultValue);

    @QtUninvokable
    public final java.util.List<V> values()    {
        return __qt_QMap_values(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    static native <V> java.util.List<V> __qt_QMap_values(long __this__nativeId);

    @SuppressWarnings("unchecked")
	@Override
    @QtUninvokable
    public boolean equals(Object other) {
        if (other instanceof java.util.Map && checkContainerType(this.keyMetaType(), this.valueMetaType(), (java.util.Map<?,?>) other)) {
            return operator_equal((java.util.Map<K,V>) other);
        }
        return false;
    }

    @Override
    @QtUninvokable
    public int hashCode() { 
    	try {
    		return hashCode(QtJambi_LibraryUtilities.internal.nativeId(this));
		} catch (QNoNativeResourcesException e) {
			return 0;
		}
    }
    @QtUninvokable
    private static native int hashCode(long __this__nativeId);
    
    @Override
    @QtUninvokable
    public String toString() {
    	try {
			return toString(QtJambi_LibraryUtilities.internal.nativeId(this));
		} catch (QNoImplementationException e) {
			return super.toString();
		} catch (QNoNativeResourcesException e) {
			return "null";
		}
    }
    
    @QtUninvokable
    private static native String toString(long __this__nativeId);
    
	@Override
    @QtUninvokable
    public final java.util.Comparator<K> comparator(){
    	return createComparator(keyMetaType().javaType(), this);
    }
	
    @QtUninvokable
	private static native long clonedAccess(long __this__nativeId);
    @QtUninvokable
	private static native void disposeAccess(long containerAccess);
    @QtUninvokable
    private static native <K> boolean lessThan(K key1, K key2, long containerAccess);
    
    @QtUninvokable
    static <K> java.util.Comparator<K> createComparator(Class<?> keyType, io.qt.QtObject object){
    	if(Comparable.class.isAssignableFrom(keyType)) {
    		return (o1,o2)->{
				@SuppressWarnings("unchecked")
				Comparable<K> c1 = (Comparable<K>)o1;
    			return c1.compareTo(o2);
    		};
    	}else if(QFlags.class.isAssignableFrom(keyType)) {
    		return (o1,o2)->{
    			return ((QFlags<?>)o1).value() < ((QFlags<?>)o2).value() ? -1 : (
    					((QFlags<?>)o2).value() < ((QFlags<?>)o1).value() ? 1 : 0
    					);
    		};
    	}else if(QtEnumerator.class.isAssignableFrom(keyType)) {
    		return (o1,o2)->{
    			return ((QtEnumerator)o1).value() < ((QtEnumerator)o2).value() ? -1 : (
    					((QtEnumerator)o2).value() < ((QtEnumerator)o1).value() ? 1 : 0
    					);
    		};
    	}else if(QtByteEnumerator.class.isAssignableFrom(keyType)) {
    		return (o1,o2)->{
    			return ((QtByteEnumerator)o1).value() < ((QtByteEnumerator)o2).value() ? -1 : (
    					((QtByteEnumerator)o2).value() < ((QtByteEnumerator)o1).value() ? 1 : 0
    					);
    		};
    	}else if(QtLongEnumerator.class.isAssignableFrom(keyType)) {
    		return (o1,o2)->{
    			return ((QtLongEnumerator)o1).value() < ((QtLongEnumerator)o2).value() ? -1 : (
    					((QtLongEnumerator)o2).value() < ((QtLongEnumerator)o1).value() ? 1 : 0
    					);
    		};
    	}else if(QtShortEnumerator.class.isAssignableFrom(keyType)) {
    		return (o1,o2)->{
    			return ((QtShortEnumerator)o1).value() < ((QtShortEnumerator)o2).value() ? -1 : (
    					((QtShortEnumerator)o2).value() < ((QtShortEnumerator)o1).value() ? 1 : 0
    					);
    		};
    	}
    	long _containerAccess = clonedAccess(QtJambi_LibraryUtilities.internal.nativeId(object));
    	java.util.Comparator<K> comparator = (o1,o2)->lessThan(o1, o2, _containerAccess) ? -1 : (lessThan(o2, o1, _containerAccess) ? 1 : 0);
    	QtJambi_LibraryUtilities.internal.registerCleaner(comparator, ()->disposeAccess(_containerAccess));
        return comparator;
    }

    @Override
    @QtUninvokable
    public final boolean containsKey(Object key){
        try{
        	return __qt_QMap_contains(QtJambi_LibraryUtilities.internal.nativeId(this), key);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(IllegalArgumentException e) {
    		return false;
    	}catch(RuntimeException e) {
    		RuntimeException e1 = QSet.handleException(e, keyMetaType(), key);
    		if(e1==e)
    			throw e;
    		return false;
        }
    }

    @Override
    @QtUninvokable
    public final V get(Object key){
		try{
			return __qt_QMap_value(QtJambi_LibraryUtilities.internal.nativeId(this), key, null);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(IllegalArgumentException e) {
    		return null;
    	}catch(RuntimeException e) {
    		RuntimeException e1 = QSet.handleException(e, keyMetaType(), key);
    		if(e1==e)
    			throw e;
    		return null;
		}
    }

    @Override
    @QtUninvokable
    public final V put(K key, V value){
        V old = value(key);
        insert(key, value);
        return old;
    }

    @SuppressWarnings("unchecked")
	@Override
    @QtUninvokable
    public final V remove(Object key){
        try{
        	return __qt_QMap_take(QtJambi_LibraryUtilities.internal.nativeId(this), (K)key);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(IllegalArgumentException e) {
    		return null;
    	}catch(RuntimeException e) {
    		RuntimeException e1 = QSet.handleException(e, keyMetaType(), key);
    		if(e1==e)
    			throw e;
    		return null;
    	}
    }

    @Override
    @QtUninvokable
    public final boolean containsValue(Object value){
    	try {
    		return __qt_QMap_keysForValue(QtJambi_LibraryUtilities.internal.nativeId(this), value).isEmpty();
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(IllegalArgumentException e) {
    		return false;
    	}catch(RuntimeException e) {
    		RuntimeException e1 = QSet.handleException(e, valueMetaType(), value);
    		if(e1==e)
    			throw e;
    		return false;
    	}
    }

    
    @io.qt.QtUninvokable
    public void writeTo(io.qt.core.QDataStream stream){
        __qt_QMap_writeTo(QtJambi_LibraryUtilities.internal.nativeId(this), QtJambi_LibraryUtilities.internal.nativeId(stream));
    }
    
    @io.qt.QtUninvokable
    private native void __qt_QMap_writeTo(long __this__nativeId, long stream);
    
    @io.qt.QtUninvokable
    public void readFrom(io.qt.core.QDataStream stream){
        __qt_QMap_readFrom(QtJambi_LibraryUtilities.internal.nativeId(this), QtJambi_LibraryUtilities.internal.nativeId(stream));
    }
    
    @io.qt.QtUninvokable
    private native void __qt_QMap_readFrom(long __this__nativeId, long stream);
    
    @io.qt.QtUninvokable
    final QMetaType keyMetaType() {
    	return keyMetaType(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @io.qt.QtUninvokable
    static native QMetaType keyMetaType(long __this_nativeId);
    
    @io.qt.QtUninvokable
    final QMetaType valueMetaType() {
    	return valueMetaType(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @io.qt.QtUninvokable
    static native QMetaType valueMetaType(long __this_nativeId);
    
    /**
     * Returns a QMap containing a single mapping.
     *
     * @param <K> the {@code QMap}'s key type
     * @param <V> the {@code QMap}'s value type
     * @param k1 the mapping's key
     * @param v1 the mapping's value
     * @return a {@code QMap} containing the specified mapping
     * @throws NullPointerException if the key or the value is {@code null}
     */
    public static <K, V> QMap<K, V> of(K k1, V v1) {
    	QMap<K, V> result = new QMap<>(
    			QList.findElementMetaType(k1), 
    			QList.findElementMetaType(v1));
    	result.insert(k1, v1);
        return result;
    }

    /**
     * Returns a QMap containing two mappings.
     *
     * @param <K> the {@code QMap}'s key type
     * @param <V> the {@code QMap}'s value type
     * @param k1 the first mapping's key
     * @param v1 the first mapping's value
     * @param k2 the second mapping's key
     * @param v2 the second mapping's value
     * @return a {@code QMap} containing the specified mappings
     * @throws NullPointerException if any key or value is {@code null}
     */
    public static <K, V> QMap<K, V> of(K k1, V v1, K k2, V v2) {
    	QMap<K, V> result = new QMap<>(
    			QList.findElementMetaType(k1, k2), 
    			QList.findElementMetaType(v1, v2));
    	result.insert(k1, v1);
    	result.insert(k2, v2);
        return result;
    }

    /**
     * Returns a QMap containing three mappings.
     *
     * @param <K> the {@code QMap}'s key type
     * @param <V> the {@code QMap}'s value type
     * @param k1 the first mapping's key
     * @param v1 the first mapping's value
     * @param k2 the second mapping's key
     * @param v2 the second mapping's value
     * @param k3 the third mapping's key
     * @param v3 the third mapping's value
     * @return a {@code QMap} containing the specified mappings
     * @throws NullPointerException if any key or value is {@code null}
     */
    public static <K, V> QMap<K, V> of(K k1, V v1, K k2, V v2, K k3, V v3) {
    	QMap<K, V> result = new QMap<>(
    			QList.findElementMetaType(k1, k2, k3), 
    			QList.findElementMetaType(v1, v2, v3));
    	result.insert(k1, v1);
    	result.insert(k2, v2);
    	result.insert(k3, v3);
        return result;
    }

    /**
     * Returns a QMap containing four mappings.
     *
     * @param <K> the {@code QMap}'s key type
     * @param <V> the {@code QMap}'s value type
     * @param k1 the first mapping's key
     * @param v1 the first mapping's value
     * @param k2 the second mapping's key
     * @param v2 the second mapping's value
     * @param k3 the third mapping's key
     * @param v3 the third mapping's value
     * @param k4 the fourth mapping's key
     * @param v4 the fourth mapping's value
     * @return a {@code QMap} containing the specified mappings
     * @throws NullPointerException if any key or value is {@code null}
     */
    public static <K, V> QMap<K, V> of(K k1, V v1, K k2, V v2, K k3, V v3, K k4, V v4) {
    	QMap<K, V> result = new QMap<>(
    			QList.findElementMetaType(k1, k2, k3, k4), 
    			QList.findElementMetaType(v1, v2, v3, v4));
    	result.insert(k1, v1);
    	result.insert(k2, v2);
    	result.insert(k3, v3);
    	result.insert(k4, v4);
        return result;
    }

    /**
     * Returns a QMap containing five mappings.
     *
     * @param <K> the {@code QMap}'s key type
     * @param <V> the {@code QMap}'s value type
     * @param k1 the first mapping's key
     * @param v1 the first mapping's value
     * @param k2 the second mapping's key
     * @param v2 the second mapping's value
     * @param k3 the third mapping's key
     * @param v3 the third mapping's value
     * @param k4 the fourth mapping's key
     * @param v4 the fourth mapping's value
     * @param k5 the fifth mapping's key
     * @param v5 the fifth mapping's value
     * @return a {@code QMap} containing the specified mappings
     * @throws NullPointerException if any key or value is {@code null}
     */
    public static <K, V> QMap<K, V> of(K k1, V v1, K k2, V v2, K k3, V v3, K k4, V v4, K k5, V v5) {
    	QMap<K, V> result = new QMap<>(
    			QList.findElementMetaType(k1, k2, k3, k4, k5), 
    			QList.findElementMetaType(v1, v2, v3, v4, v5));
    	result.insert(k1, v1);
    	result.insert(k2, v2);
    	result.insert(k3, v3);
    	result.insert(k4, v4);
    	result.insert(k5, v5);
        return result;
    }

    /**
     * Returns a QMap containing six mappings.
     *
     * @param <K> the {@code QMap}'s key type
     * @param <V> the {@code QMap}'s value type
     * @param k1 the first mapping's key
     * @param v1 the first mapping's value
     * @param k2 the second mapping's key
     * @param v2 the second mapping's value
     * @param k3 the third mapping's key
     * @param v3 the third mapping's value
     * @param k4 the fourth mapping's key
     * @param v4 the fourth mapping's value
     * @param k5 the fifth mapping's key
     * @param v5 the fifth mapping's value
     * @param k6 the sixth mapping's key
     * @param v6 the sixth mapping's value
     * @return a {@code QMap} containing the specified mappings
     * @throws NullPointerException if any key or value is {@code null}
     */
    public static <K, V> QMap<K, V> of(K k1, V v1, K k2, V v2, K k3, V v3, K k4, V v4, K k5, V v5,
                               K k6, V v6) {
    	QMap<K, V> result = new QMap<>(
    			QList.findElementMetaType(k1, k2, k3, k4, k5, k6), 
    			QList.findElementMetaType(v1, v2, v3, v4, v5, v6));
    	result.insert(k1, v1);
    	result.insert(k2, v2);
    	result.insert(k3, v3);
    	result.insert(k4, v4);
    	result.insert(k5, v5);
    	result.insert(k6, v6);
        return result;
    }

    /**
     * Returns a QMap containing seven mappings.
     *
     * @param <K> the {@code QMap}'s key type
     * @param <V> the {@code QMap}'s value type
     * @param k1 the first mapping's key
     * @param v1 the first mapping's value
     * @param k2 the second mapping's key
     * @param v2 the second mapping's value
     * @param k3 the third mapping's key
     * @param v3 the third mapping's value
     * @param k4 the fourth mapping's key
     * @param v4 the fourth mapping's value
     * @param k5 the fifth mapping's key
     * @param v5 the fifth mapping's value
     * @param k6 the sixth mapping's key
     * @param v6 the sixth mapping's value
     * @param k7 the seventh mapping's key
     * @param v7 the seventh mapping's value
     * @return a {@code QMap} containing the specified mappings
     * @throws NullPointerException if any key or value is {@code null}
     */
    public static <K, V> QMap<K, V> of(K k1, V v1, K k2, V v2, K k3, V v3, K k4, V v4, K k5, V v5,
                               K k6, V v6, K k7, V v7) {
    	QMap<K, V> result = new QMap<>(
    			QList.findElementMetaType(k1, k2, k3, k4, k5, k6, k7), 
    			QList.findElementMetaType(v1, v2, v3, v4, v5, v6, v7));
    	result.insert(k1, v1);
    	result.insert(k2, v2);
    	result.insert(k3, v3);
    	result.insert(k4, v4);
    	result.insert(k5, v5);
    	result.insert(k6, v6);
    	result.insert(k7, v7);
        return result;
    }

    /**
     * Returns a QMap containing eight mappings.
     *
     * @param <K> the {@code QMap}'s key type
     * @param <V> the {@code QMap}'s value type
     * @param k1 the first mapping's key
     * @param v1 the first mapping's value
     * @param k2 the second mapping's key
     * @param v2 the second mapping's value
     * @param k3 the third mapping's key
     * @param v3 the third mapping's value
     * @param k4 the fourth mapping's key
     * @param v4 the fourth mapping's value
     * @param k5 the fifth mapping's key
     * @param v5 the fifth mapping's value
     * @param k6 the sixth mapping's key
     * @param v6 the sixth mapping's value
     * @param k7 the seventh mapping's key
     * @param v7 the seventh mapping's value
     * @param k8 the eighth mapping's key
     * @param v8 the eighth mapping's value
     * @return a {@code QMap} containing the specified mappings
     * @throws NullPointerException if any key or value is {@code null}
     */
    public static <K, V> QMap<K, V> of(K k1, V v1, K k2, V v2, K k3, V v3, K k4, V v4, K k5, V v5,
                               K k6, V v6, K k7, V v7, K k8, V v8) {
    	QMap<K, V> result = new QMap<>(
    			QList.findElementMetaType(k1, k2, k3, k4, k5, k6, k7, k8), 
    			QList.findElementMetaType(v1, v2, v3, v4, v5, v6, v7, v8));
    	result.insert(k1, v1);
    	result.insert(k2, v2);
    	result.insert(k3, v3);
    	result.insert(k4, v4);
    	result.insert(k5, v5);
    	result.insert(k6, v6);
    	result.insert(k7, v7);
    	result.insert(k8, v8);
        return result;
    }

    /**
     * Returns a QMap containing nine mappings.
     *
     * @param <K> the {@code QMap}'s key type
     * @param <V> the {@code QMap}'s value type
     * @param k1 the first mapping's key
     * @param v1 the first mapping's value
     * @param k2 the second mapping's key
     * @param v2 the second mapping's value
     * @param k3 the third mapping's key
     * @param v3 the third mapping's value
     * @param k4 the fourth mapping's key
     * @param v4 the fourth mapping's value
     * @param k5 the fifth mapping's key
     * @param v5 the fifth mapping's value
     * @param k6 the sixth mapping's key
     * @param v6 the sixth mapping's value
     * @param k7 the seventh mapping's key
     * @param v7 the seventh mapping's value
     * @param k8 the eighth mapping's key
     * @param v8 the eighth mapping's value
     * @param k9 the ninth mapping's key
     * @param v9 the ninth mapping's value
     * @return a {@code QMap} containing the specified mappings
     * @throws NullPointerException if any key or value is {@code null}
     */
    public static <K, V> QMap<K, V> of(K k1, V v1, K k2, V v2, K k3, V v3, K k4, V v4, K k5, V v5,
                               K k6, V v6, K k7, V v7, K k8, V v8, K k9, V v9) {
    	QMap<K, V> result = new QMap<>(
    			QList.findElementMetaType(k1, k2, k3, k4, k5, k6, k7, k8, k9), 
    			QList.findElementMetaType(v1, v2, v3, v4, v5, v6, v7, v8, v9));
    	result.insert(k1, v1);
    	result.insert(k2, v2);
    	result.insert(k3, v3);
    	result.insert(k4, v4);
    	result.insert(k5, v5);
    	result.insert(k6, v6);
    	result.insert(k7, v7);
    	result.insert(k8, v8);
    	result.insert(k9, v9);
        return result;
    }

    /**
     * Returns a QMap containing ten mappings.
     *
     * @param <K> the {@code QMap}'s key type
     * @param <V> the {@code QMap}'s value type
     * @param k1 the first mapping's key
     * @param v1 the first mapping's value
     * @param k2 the second mapping's key
     * @param v2 the second mapping's value
     * @param k3 the third mapping's key
     * @param v3 the third mapping's value
     * @param k4 the fourth mapping's key
     * @param v4 the fourth mapping's value
     * @param k5 the fifth mapping's key
     * @param v5 the fifth mapping's value
     * @param k6 the sixth mapping's key
     * @param v6 the sixth mapping's value
     * @param k7 the seventh mapping's key
     * @param v7 the seventh mapping's value
     * @param k8 the eighth mapping's key
     * @param v8 the eighth mapping's value
     * @param k9 the ninth mapping's key
     * @param v9 the ninth mapping's value
     * @param k10 the tenth mapping's key
     * @param v10 the tenth mapping's value
     * @return a {@code QMap} containing the specified mappings
     * @throws NullPointerException if any key or value is {@code null}
     */
    public static <K, V> QMap<K, V> of(K k1, V v1, K k2, V v2, K k3, V v3, K k4, V v4, K k5, V v5,
                               K k6, V v6, K k7, V v7, K k8, V v8, K k9, V v9, K k10, V v10) {
    	QMap<K, V> result = new QMap<>(
    			QList.findElementMetaType(k1, k2, k3, k4, k5, k6, k7, k8, k9, k10), 
    			QList.findElementMetaType(v1, v2, v3, v4, v5, v6, v7, v8, v9, v10));
    	result.insert(k1, v1);
    	result.insert(k2, v2);
    	result.insert(k3, v3);
    	result.insert(k4, v4);
    	result.insert(k5, v5);
    	result.insert(k6, v6);
    	result.insert(k7, v7);
    	result.insert(k8, v8);
    	result.insert(k9, v9);
    	result.insert(k10, v10);
        return result;
    }

    /**
     * Returns a QMap containing keys and values extracted from the given entries.
     *
     * @apiNote
     * It is convenient to create the map entries using the {@link Map#entry Map.entry()} method.
     * For example,
     *
     * <pre>{@code
     *     import static java.util.Map.entry;
     *
     *     QMap<Integer,String> map = QMap.ofEntries(
     *         entry(1, "a"),
     *         entry(2, "b"),
     *         entry(3, "c"),
     *         ...
     *         entry(26, "z"));
     * }</pre>
     *
     * @param <K> the {@code QMap}'s key type
     * @param <V> the {@code QMap}'s value type
     * @param entries {@code java.util.Map.Entry}s containing the keys and values from which the map is populated
     * @return a {@code QMap} containing the specified mappings
     * @throws NullPointerException if any entry, key, or value is {@code null}, or if
     *         the {@code entries} array is {@code null}
     */
    @SafeVarargs
    public static <K, V> QMap<K, V> ofEntries(Map.Entry<? extends K, ? extends V> entry0, Map.Entry<? extends K, ? extends V>... entries) {
    	QPair<QMetaType, QMetaType> metaTypes = findMapMetaType(entry0, entries);
    	QMap<K, V> result = new QMap<>(metaTypes.first, metaTypes.second);
    	result.insert(entry0.getKey(), entry0.getValue());
    	for (Map.Entry<? extends K, ? extends V> entry : entries) {
        	result.insert(entry.getKey(), entry.getValue());
		}
        return result;
    }
    
    static RuntimeException handleException(RuntimeException e, QMetaType keyMetaType, QMetaType valueMetaType, Object key, Object value) {
		if(( (e.getClass()==RuntimeException.class && e.getMessage()!=null && e.getMessage().startsWith("Cannot cast to type")) 
				|| e instanceof ClassCastException ) && value!=null) {
			Class<?> keyType = keyMetaType.javaType();
			if(Collection.class.isAssignableFrom(keyType)) {
				keyType = Collection.class;
			}else if(Map.class.isAssignableFrom(keyType)) {
				keyType = Map.class;
			}
			Class<?> valueType = valueMetaType.javaType();
			if(Collection.class.isAssignableFrom(valueType)) {
				valueType = Collection.class;
			}else if(Map.class.isAssignableFrom(valueType)) {
				valueType = Map.class;
			}
			if(!keyType.isInstance(key)) {
				return new IllegalArgumentException(String.format("Wrong key argument given: %1$s, expected: %2$s", key.getClass().getTypeName(), keyType.getTypeName()), e);
			}else if(!valueType.isInstance(value)) {
				return new IllegalArgumentException(String.format("Wrong value argument given: %1$s, expected: %2$s", value.getClass().getTypeName(), valueType.getTypeName()), e);
			}else {
				return new IllegalArgumentException(e.getMessage(), e);
			}
		}
		return e;
    }
    
    @SafeVarargs
	static QPair<QMetaType,QMetaType> findMapMetaType(Map.Entry<?,?> entry0, Map.Entry<?,?>... entries){
    	QMetaType keyMetaType = QList.getMetaType(entry0.getKey());
    	Class<?> keyType = entry0==null ? null : entry0.getKey().getClass();
    	QMetaType valueMetaType = QList.getMetaType(entry0.getValue());
    	Class<?> valueType = entry0==null ? null : entry0.getValue().getClass();
    	for(Map.Entry<?,?> e : entries) {
    		if(keyMetaType!=null) {
	    		QMetaType _result = QList.getMetaType(e.getKey());
	    		if(!keyMetaType.isValid() || keyMetaType.id()==QMetaType.Type.Nullptr.value())
	    			keyMetaType = _result;
	    		if(!_result.equals(keyMetaType)) {
	    			keyMetaType = null;
	    		}
    		}
    		if(valueMetaType!=null) {
	    		QMetaType _result = QList.getMetaType(e.getValue());
	    		if(!valueMetaType.isValid() || valueMetaType.id()==QMetaType.Type.Nullptr.value())
	    			valueMetaType = _result;
	    		if(!_result.equals(valueMetaType)) {
	    			valueMetaType = null;
	    		}
    		}
    		if(e.getKey()!=null) {
    			if(keyType==null) {
    				keyType = e.getKey().getClass();
    			}else {
    				Class<?> type2 = e.getKey().getClass();
    				if(type2!=keyType) {
    					if(type2.isAssignableFrom(keyType)) {
    						keyType = type2;
    					}else if(!keyType.isAssignableFrom(type2)) {
    						do{
    							keyType = keyType.getSuperclass();
    						}while(!keyType.isAssignableFrom(type2));
    					}
    				}
    			}
			}
    		if(e.getValue()!=null) {
    			if(valueType==null) {
    				valueType = e.getValue().getClass();
    			}else {
    				Class<?> type2 = e.getValue().getClass();
    				if(type2!=valueType) {
    					if(type2.isAssignableFrom(valueType)) {
    						valueType = type2;
    					}else if(!valueType.isAssignableFrom(type2)) {
    						do{
    							valueType = keyType.getSuperclass();
    						}while(!valueType.isAssignableFrom(type2));
    					}
    				}
    			}
			}
    	}
    	if(keyMetaType==null)
    		keyMetaType = QMetaType.fromType(keyType);
    	if(valueMetaType==null)
    		valueMetaType = QMetaType.fromType(valueType);
    	return new QPair<>(keyMetaType, valueMetaType);
    }
    
    static QPair<QMetaType,QMetaType> findMapMetaType(Map<?,?> elements){
    	Class<?> cls = QtJambiInternal.getClass(elements);
        if(cls==QMap.class) {
            return new QPair<>(((QMap<?,?>)elements).keyMetaType(), ((QMap<?,?>)elements).valueMetaType());
        }else if(cls==QHash.class) {
            return new QPair<>(((QHash<?,?>)elements).keyMetaType(), ((QHash<?,?>)elements).valueMetaType());
        }else if(cls==QMultiMap.class) {
            return new QPair<>(((QMultiMap<?,?>)elements).keyMetaType(), ((QMultiMap<?,?>)elements).valueMetaType());
        }else if(cls==QMultiHash.class) {
            return new QPair<>(((QMultiHash<?,?>)elements).keyMetaType(), ((QMultiHash<?,?>)elements).valueMetaType());
        }else {
        	QMetaType keyMetaType = new QMetaType();
        	Class<?> keyType = null;
        	QMetaType valueMetaType = new QMetaType();
        	Class<?> valueType = null;
        	for(Entry<?, ?> entry : elements.entrySet()) {
        		Object key = entry.getKey();
        		Object value = entry.getValue();
        		if(keyMetaType!=null) {
    	    		QMetaType _result = QList.getMetaType(key);
    	    		if(!keyMetaType.isValid() || keyMetaType.id()==QMetaType.Type.Nullptr.value())
    	    			keyMetaType = _result;
    	    		if(!_result.equals(keyMetaType)) {
    	    			keyMetaType = null;
    	    		}
        		}
        		if(key!=null) {
        			if(keyType==null) {
        				keyType = key.getClass();
        			}else {
        				Class<?> type2 = key.getClass();
        				if(type2!=keyType) {
        					if(type2.isAssignableFrom(keyType)) {
        						keyType = type2;
        					}else if(!keyType.isAssignableFrom(type2)) {
        						do{
        							keyType = keyType.getSuperclass();
        						}while(!keyType.isAssignableFrom(type2));
        					}
        				}
        			}
    			}
        		if(valueMetaType!=null) {
    	    		QMetaType _result = QList.getMetaType(value);
    	    		if(!valueMetaType.isValid() || valueMetaType.id()==QMetaType.Type.Nullptr.value())
    	    			valueMetaType = _result;
    	    		if(!_result.equals(valueMetaType)) {
    	    			valueMetaType = null;
    	    		}
        		}
        		if(value!=null) {
        			if(valueType==null) {
        				valueType = value.getClass();
        			}else {
        				Class<?> type2 = value.getClass();
        				if(type2!=valueType) {
        					if(type2.isAssignableFrom(valueType)) {
        						valueType = type2;
        					}else if(!valueType.isAssignableFrom(type2)) {
        						do{
        							valueType = valueType.getSuperclass();
        						}while(!valueType.isAssignableFrom(type2));
        					}
        				}
        			}
    			}
        	}
        	if(keyMetaType==null)
        		keyMetaType = QMetaType.fromType(keyType);
        	if(valueMetaType==null)
        		valueMetaType = QMetaType.fromType(valueType);
        	return new QPair<>(keyMetaType, valueMetaType);
        }
    }
}
