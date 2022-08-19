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

import static io.qt.core.QMap.findMapMetaType;

import java.util.Map;
import java.util.Objects;

import io.qt.NativeAccess;
import io.qt.QNoImplementationException;
import io.qt.QNoNativeResourcesException;
import io.qt.QtUninvokable;

/**
 * <p>Java wrapper for Qt class <a href="https://doc.qt.io/qt/qhash.html">QHash</a></p>
 */
public class QHash<K,V> extends io.qt.internal.QtJambiHashObject<K,V> implements Cloneable
{
	static {
    	QtJambi_LibraryUtilities.initialize();
    }

    @NativeAccess
    protected QHash(QPrivateConstructor p) {
		super(p);
	}
    
    public QHash(Class<K> keyType, Class<V> valueType) {
		super(null);
		QMetaType keyMetaType = QMetaType.fromType(keyType);
		QMetaType valueMetaType = QMetaType.fromType(valueType);
		if(keyMetaType.id()==0)
			throw new IllegalArgumentException("QMetaType::UnknownType cannot be key type of QHash.");
		if(keyMetaType.id()==QMetaType.Type.Void.value())
			throw new IllegalArgumentException("void cannot be key type of QHash.");
		if(valueMetaType.id()==0)
			throw new IllegalArgumentException("QMetaType::UnknownType cannot be value type of QHash.");
		if(valueMetaType.id()==QMetaType.Type.Void.value())
			throw new IllegalArgumentException("void cannot be value type of QHash.");
		initialize(keyType, QtJambi_LibraryUtilities.internal.nativeId(keyMetaType), valueType, QtJambi_LibraryUtilities.internal.nativeId(valueMetaType), null);
	}
    
    public QHash(Map<K,V> other) {
		super(null);
		QPair<QMetaType, QMetaType> metaTypes = QMap.findMapMetaType(Objects.requireNonNull(other));
		if(metaTypes.first.id()==0)
			throw new IllegalArgumentException("QMetaType::UnknownType cannot be key type of QHash.");
		if(metaTypes.first.id()==QMetaType.Type.Void.value())
			throw new IllegalArgumentException("void cannot be key type of QHash.");
		if(metaTypes.second.id()==0)
			throw new IllegalArgumentException("QMetaType::UnknownType cannot be value type of QHash.");
		if(metaTypes.second.id()==QMetaType.Type.Void.value())
			throw new IllegalArgumentException("void cannot be value type of QHash.");
		initialize(metaTypes.first.javaType(), QtJambi_LibraryUtilities.internal.nativeId(metaTypes.first), metaTypes.second.javaType(), QtJambi_LibraryUtilities.internal.nativeId(metaTypes.second), other);
	}
    
	public QHash(Class<K> keyType, QMetaType valueMetaType) {
		super(null);
		QMetaType keyMetaType = QMetaType.fromType(keyType);
		if(keyMetaType.id()==0)
			throw new IllegalArgumentException("QMetaType::UnknownType cannot be key type of QHash.");
		if(keyMetaType.id()==QMetaType.Type.Void.value())
			throw new IllegalArgumentException("void cannot be key type of QHash.");
		if(valueMetaType.id()==0)
			throw new IllegalArgumentException("QMetaType::UnknownType cannot be value type of QHash.");
		if(valueMetaType.id()==QMetaType.Type.Void.value())
			throw new IllegalArgumentException("void cannot be value type of QHash.");
		initialize(keyType, QtJambi_LibraryUtilities.internal.nativeId(keyMetaType), valueMetaType.javaType(), QtJambi_LibraryUtilities.internal.nativeId(valueMetaType), null);
	}
    
	public QHash(QMetaType keyMetaType, Class<V> valueType) {
		super(null);
		QMetaType valueMetaType = QMetaType.fromType(valueType);
		if(keyMetaType.id()==0)
			throw new IllegalArgumentException("QMetaType::UnknownType cannot be key type of QHash.");
		if(keyMetaType.id()==QMetaType.Type.Void.value())
			throw new IllegalArgumentException("void cannot be key type of QHash.");
		if(valueMetaType.id()==0)
			throw new IllegalArgumentException("QMetaType::UnknownType cannot be value type of QHash.");
		if(valueMetaType.id()==QMetaType.Type.Void.value())
			throw new IllegalArgumentException("void cannot be value type of QHash.");
		initialize(keyMetaType.javaType(), QtJambi_LibraryUtilities.internal.nativeId(keyMetaType), valueType, QtJambi_LibraryUtilities.internal.nativeId(valueMetaType), null);
	}
	
	public QHash(Class<K> keyType, QMetaType.Type valueMetaType) {
		this(keyType, new QMetaType(valueMetaType));
	}
	
	public QHash(QMetaType.Type keyMetaType, Class<V> valueType) {
		this(new QMetaType(keyMetaType), valueType);
	}
	
	public QHash(QMetaType.Type keyMetaType, QMetaType valueMetaType) {
		this(new QMetaType(keyMetaType), valueMetaType);
	}
	
	public QHash(QMetaType keyMetaType, QMetaType.Type valueMetaType) {
		this(keyMetaType, new QMetaType(valueMetaType));
	}
	
	public QHash(QMetaType.Type keyMetaType, QMetaType.Type valueMetaType) {
		this(new QMetaType(keyMetaType), new QMetaType(valueMetaType));
	}
    
	public QHash(QMetaType keyMetaType, QMetaType valueMetaType) {
		super(null);
		if(keyMetaType.id()==0)
			throw new IllegalArgumentException("QMetaType::UnknownType cannot be key type of QHash.");
		if(keyMetaType.id()==QMetaType.Type.Void.value())
			throw new IllegalArgumentException("void cannot be key type of QHash.");
		if(valueMetaType.id()==0)
			throw new IllegalArgumentException("QMetaType::UnknownType cannot be value type of QHash.");
		if(valueMetaType.id()==QMetaType.Type.Void.value())
			throw new IllegalArgumentException("void cannot be value type of QHash.");
		initialize(keyMetaType.javaType(), QtJambi_LibraryUtilities.internal.nativeId(keyMetaType), valueMetaType.javaType(), QtJambi_LibraryUtilities.internal.nativeId(valueMetaType), null);
	}
    
    @QtUninvokable
    private native void initialize(Class<?> keyType, long keyMetaType, Class<?> valueType, long valueMetaType, Map<K, V> other);
    
    public static QHash<String,Object> createVariantHash() {
    	return new QHash<>(new QMetaType(QMetaType.Type.QString), new QMetaType(QMetaType.Type.QVariant));
    }
    
    @Override
	public QHash<K,V> clone(){
		return new QHash<>(this);
	}
    
	@QtUninvokable
    protected final io.qt.core.QMapIterator<K,V> begin()    {
        return begin(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    static native <K,V> io.qt.core.QMapIterator<K,V> begin(long __this__nativeId);
    
    @QtUninvokable
    public final int capacity()    {
        return capacity(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    static native <T> int capacity(long __this__nativeId);

    @QtUninvokable
    public final void clear()    {
        clear(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    static native void clear(long __this__nativeId);

    @QtUninvokable
    public final boolean contains(K key)    {
        try{
        	return contains(QtJambi_LibraryUtilities.internal.nativeId(this), key);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QSet.handleException(e, keyMetaType(), key);
        }
    }
    @QtUninvokable
    static native <K> boolean contains(long __this__nativeId, K key);

    @QtUninvokable
    public final int count()    {
        return size();
    }

    @QtUninvokable
    public final int count(K key)    {
        try{
        	return count(QtJambi_LibraryUtilities.internal.nativeId(this), key);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QSet.handleException(e, keyMetaType(), key);
        }
    }
    @QtUninvokable
    static native <K> int count(long __this__nativeId, K key);

    @QtUninvokable
    protected final io.qt.core.QMapIterator<K,V> end()    {
        return end(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    static native <K,V> io.qt.core.QMapIterator<K,V> end(long __this__nativeId);

    @QtUninvokable
    public final io.qt.core.QMapIterator<K,V> find(K key)    {
        try{
        	return find(QtJambi_LibraryUtilities.internal.nativeId(this), key);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QSet.handleException(e, keyMetaType(), key);
        }
    }
    @QtUninvokable
    static native <K,V> io.qt.core.QMapIterator<K,V> find(long __this__nativeId, K key);

    @QtUninvokable
    public final void insert(K key, V value)    {
    	try {
    		insert(QtJambi_LibraryUtilities.internal.nativeId(this), key, value);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QMap.handleException(e, keyMetaType(), valueMetaType(), key, value);
    	}
    }
    @QtUninvokable
    static native <K,V> void insert(long __this__nativeId, K key, V value);

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
        	return key(QtJambi_LibraryUtilities.internal.nativeId(this), value, defaultKey);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QMap.handleException(e, keyMetaType(), valueMetaType(), defaultKey, value);
        }
    }
    @QtUninvokable
    static native <K,V> K key(long __this__nativeId, V value, K defaultKey);

    @QtUninvokable
    public final java.util.List<K> keys()    {
        return keys(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    static native <K> java.util.List<K> keys(long __this__nativeId);

    @QtUninvokable
    public final java.util.List<K> keys(V value)    {
    	try {
    		return keysForValue(QtJambi_LibraryUtilities.internal.nativeId(this), value);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QSet.handleException(e, valueMetaType(), value);
    	}
    }
    @QtUninvokable
    static native <K,V> java.util.List<K> keysForValue(long __this__nativeId, V value);

    @QtUninvokable
    private final boolean operator_equal(java.util.Map<K,V> other)    {
        return operator_equal(QtJambi_LibraryUtilities.internal.nativeId(this), other);
    }
    @QtUninvokable
    static native <K,V> boolean operator_equal(long __this__nativeId, java.util.Map<K,V> other);

    @QtUninvokable
    public final int removeAll(Object key)    {
        try{
        	return remove(QtJambi_LibraryUtilities.internal.nativeId(this), key);
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
    static native <K,V> int remove(long __this__nativeId, K key);
    
    @QtUninvokable
    public final void reserve(int size)    {
        reserve(QtJambi_LibraryUtilities.internal.nativeId(this), size);
    }
    @QtUninvokable
    static native <T> void reserve(long __this__nativeId, int size);

    @QtUninvokable
    public final int size()    {
        return size(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    static native int size(long __this__nativeId);
    
    @QtUninvokable
    public final void squeeze() { reserve(1); }

    @QtUninvokable
    public final V take(K key)    {
    	try {
    		return take(QtJambi_LibraryUtilities.internal.nativeId(this), key);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QSet.handleException(e, keyMetaType(), key);
    	}
    }
    @QtUninvokable
    static native <K,V> V take(long __this__nativeId, K key);

    @QtUninvokable
    public final V value(K key) {
        return value(key, null);
    }
    @QtUninvokable
    public final V value(K key, V defaultValue)    {
    	try {
    		return value(QtJambi_LibraryUtilities.internal.nativeId(this), key, defaultValue);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QMap.handleException(e, keyMetaType(), valueMetaType(), key, defaultValue);
    	}
    }
    @QtUninvokable
    static native <K,V> V value(long __this__nativeId, K key, V defaultValue);

    @QtUninvokable
    public final java.util.List<V> values()    {
        return values(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    static native <V> java.util.List<V> values(long __this__nativeId);

    @SuppressWarnings("unchecked")
	@Override
    @QtUninvokable
    public boolean equals(Object other) {
        if (other instanceof java.util.Map && QMap.checkContainerType(this.keyMetaType(), this.valueMetaType(), (java.util.Map<?,?>) other)) {
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
    
    @QtUninvokable
    static native <K> boolean lessThan(K key1, K key2);

    @Override
    @QtUninvokable
    public final boolean containsKey(Object key){
        try{
        	return contains(QtJambi_LibraryUtilities.internal.nativeId(this), key);
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
			return value(QtJambi_LibraryUtilities.internal.nativeId(this), key, null);
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
        	return take(QtJambi_LibraryUtilities.internal.nativeId(this), (K)key);
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
    		return keysForValue(QtJambi_LibraryUtilities.internal.nativeId(this), value).isEmpty();
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
        writeTo(QtJambi_LibraryUtilities.internal.nativeId(this), QtJambi_LibraryUtilities.internal.nativeId(stream));
    }
    
    @io.qt.QtUninvokable
    private native void writeTo(long __this__nativeId, long stream);
    
    @io.qt.QtUninvokable
    public void readFrom(io.qt.core.QDataStream stream){
        readFrom(QtJambi_LibraryUtilities.internal.nativeId(this), QtJambi_LibraryUtilities.internal.nativeId(stream));
    }
    
    @io.qt.QtUninvokable
    private native void readFrom(long __this__nativeId, long stream);
    
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
     * Returns a QHash containing a single mapping.
     *
     * @param <K> the {@code QHash}'s key type
     * @param <V> the {@code QHash}'s value type
     * @param k1 the mapping's key
     * @param v1 the mapping's value
     * @return a {@code QHash} containing the specified mapping
     * @throws NullPointerException if the key or the value is {@code null}
     */
    public static <K, V> QHash<K, V> of(K k1, V v1) {
    	QHash<K, V> result = new QHash<>(
    			QList.findElementMetaType(k1), 
    			QList.findElementMetaType(v1));
    	result.insert(k1, v1);
        return result;
    }

    /**
     * Returns a QHash containing two mappings.
     *
     * @param <K> the {@code QHash}'s key type
     * @param <V> the {@code QHash}'s value type
     * @param k1 the first mapping's key
     * @param v1 the first mapping's value
     * @param k2 the second mapping's key
     * @param v2 the second mapping's value
     * @return a {@code QHash} containing the specified mappings
     * @throws NullPointerException if any key or value is {@code null}
     */
    public static <K, V> QHash<K, V> of(K k1, V v1, K k2, V v2) {
    	QHash<K, V> result = new QHash<>(
    			QList.findElementMetaType(k1, k2), 
    			QList.findElementMetaType(v1, v2));
    	result.insert(k1, v1);
    	result.insert(k2, v2);
        return result;
    }

    /**
     * Returns a QHash containing three mappings.
     *
     * @param <K> the {@code QHash}'s key type
     * @param <V> the {@code QHash}'s value type
     * @param k1 the first mapping's key
     * @param v1 the first mapping's value
     * @param k2 the second mapping's key
     * @param v2 the second mapping's value
     * @param k3 the third mapping's key
     * @param v3 the third mapping's value
     * @return a {@code QHash} containing the specified mappings
     * @throws NullPointerException if any key or value is {@code null}
     */
    public static <K, V> QHash<K, V> of(K k1, V v1, K k2, V v2, K k3, V v3) {
    	QHash<K, V> result = new QHash<>(
    			QList.findElementMetaType(k1, k2, k3), 
    			QList.findElementMetaType(v1, v2, v3));
    	result.insert(k1, v1);
    	result.insert(k2, v2);
    	result.insert(k3, v3);
        return result;
    }

    /**
     * Returns a QHash containing four mappings.
     *
     * @param <K> the {@code QHash}'s key type
     * @param <V> the {@code QHash}'s value type
     * @param k1 the first mapping's key
     * @param v1 the first mapping's value
     * @param k2 the second mapping's key
     * @param v2 the second mapping's value
     * @param k3 the third mapping's key
     * @param v3 the third mapping's value
     * @param k4 the fourth mapping's key
     * @param v4 the fourth mapping's value
     * @return a {@code QHash} containing the specified mappings
     * @throws NullPointerException if any key or value is {@code null}
     */
    public static <K, V> QHash<K, V> of(K k1, V v1, K k2, V v2, K k3, V v3, K k4, V v4) {
    	QHash<K, V> result = new QHash<>(
    			QList.findElementMetaType(k1, k2, k3, k4), 
    			QList.findElementMetaType(v1, v2, v3, v4));
    	result.insert(k1, v1);
    	result.insert(k2, v2);
    	result.insert(k3, v3);
    	result.insert(k4, v4);
        return result;
    }

    /**
     * Returns a QHash containing five mappings.
     *
     * @param <K> the {@code QHash}'s key type
     * @param <V> the {@code QHash}'s value type
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
     * @return a {@code QHash} containing the specified mappings
     * @throws NullPointerException if any key or value is {@code null}
     */
    public static <K, V> QHash<K, V> of(K k1, V v1, K k2, V v2, K k3, V v3, K k4, V v4, K k5, V v5) {
    	QHash<K, V> result = new QHash<>(
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
     * Returns a QHash containing six mappings.
     *
     * @param <K> the {@code QHash}'s key type
     * @param <V> the {@code QHash}'s value type
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
     * @return a {@code QHash} containing the specified mappings
     * @throws NullPointerException if any key or value is {@code null}
     */
    public static <K, V> QHash<K, V> of(K k1, V v1, K k2, V v2, K k3, V v3, K k4, V v4, K k5, V v5,
                               K k6, V v6) {
    	QHash<K, V> result = new QHash<>(
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
     * Returns a QHash containing seven mappings.
     *
     * @param <K> the {@code QHash}'s key type
     * @param <V> the {@code QHash}'s value type
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
     * @return a {@code QHash} containing the specified mappings
     * @throws NullPointerException if any key or value is {@code null}
     */
    public static <K, V> QHash<K, V> of(K k1, V v1, K k2, V v2, K k3, V v3, K k4, V v4, K k5, V v5,
                               K k6, V v6, K k7, V v7) {
    	QHash<K, V> result = new QHash<>(
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
     * Returns a QHash containing eight mappings.
     *
     * @param <K> the {@code QHash}'s key type
     * @param <V> the {@code QHash}'s value type
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
     * @return a {@code QHash} containing the specified mappings
     * @throws NullPointerException if any key or value is {@code null}
     */
    public static <K, V> QHash<K, V> of(K k1, V v1, K k2, V v2, K k3, V v3, K k4, V v4, K k5, V v5,
                               K k6, V v6, K k7, V v7, K k8, V v8) {
    	QHash<K, V> result = new QHash<>(
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
     * Returns a QHash containing nine mappings.
     *
     * @param <K> the {@code QHash}'s key type
     * @param <V> the {@code QHash}'s value type
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
     * @return a {@code QHash} containing the specified mappings
     * @throws NullPointerException if any key or value is {@code null}
     */
    public static <K, V> QHash<K, V> of(K k1, V v1, K k2, V v2, K k3, V v3, K k4, V v4, K k5, V v5,
                               K k6, V v6, K k7, V v7, K k8, V v8, K k9, V v9) {
    	QHash<K, V> result = new QHash<>(
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
     * Returns a QHash containing ten mappings.
     *
     * @param <K> the {@code QHash}'s key type
     * @param <V> the {@code QHash}'s value type
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
     * @return a {@code QHash} containing the specified mappings
     * @throws NullPointerException if any key or value is {@code null}
     */
    public static <K, V> QHash<K, V> of(K k1, V v1, K k2, V v2, K k3, V v3, K k4, V v4, K k5, V v5,
                               K k6, V v6, K k7, V v7, K k8, V v8, K k9, V v9, K k10, V v10) {
    	QHash<K, V> result = new QHash<>(
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
     * Returns a QHash containing keys and values extracted from the given entries.
     *
     * @apiNote
     * It is convenient to create the map entries using the {@link Map#entry Map.entry()} method.
     * For example,
     *
     * <pre>{@code
     *     import static java.util.Map.entry;
     *
     *     QHash<Integer,String> map = QHash.ofEntries(
     *         entry(1, "a"),
     *         entry(2, "b"),
     *         entry(3, "c"),
     *         ...
     *         entry(26, "z"));
     * }</pre>
     *
     * @param <K> the {@code QHash}'s key type
     * @param <V> the {@code QHash}'s value type
     * @param entries {@code java.util.Map.Entry}s containing the keys and values from which the map is populated
     * @return a {@code QHash} containing the specified mappings
     * @throws NullPointerException if any entry, key, or value is {@code null}, or if
     *         the {@code entries} array is {@code null}
     */
    @SafeVarargs
    public static <K, V> QHash<K, V> ofEntries(Map.Entry<? extends K, ? extends V> entry0, Map.Entry<? extends K, ? extends V>... entries) {
    	QPair<QMetaType, QMetaType> metaTypes = findMapMetaType(entry0, entries);
    	QHash<K, V> result = new QHash<>(metaTypes.first, metaTypes.second);
    	result.insert(entry0.getKey(), entry0.getValue());
    	for (Map.Entry<? extends K, ? extends V> entry : entries) {
        	result.insert(entry.getKey(), entry.getValue());
		}
        return result;
    }
}
