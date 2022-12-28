/****************************************************************************
**
** Copyright (C) 2009-2022 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
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

import static io.qt.core.QHash.keysForValue;
import static io.qt.core.QMap.findMapMetaType;

import java.util.Collections;
import java.util.List;
import java.util.Map;
import java.util.Objects;

import io.qt.NativeAccess;
import io.qt.QNoImplementationException;
import io.qt.QNoNativeResourcesException;
import io.qt.QtUninvokable;

/**
 * <p>Java wrapper for Qt class <a href="https://doc.qt.io/qt/qmultihash.html">QMultiHash</a></p>
 */
public class QMultiHash<K,V> extends io.qt.internal.AbstractMultiHash<K,V> implements Cloneable
{
	static {
    	QtJambi_LibraryUtilities.initialize();
    }

    @NativeAccess
    protected QMultiHash(QPrivateConstructor p) {
		super(p);
	}
	
	public QMultiHash(Class<K> keyType, QMetaType.Type valueMetaType) {
		this(keyType, new QMetaType(valueMetaType));
	}
	
	public QMultiHash(QMetaType.Type keyMetaType, Class<V> valueType) {
		this(new QMetaType(keyMetaType), valueType);
	}
	
	public QMultiHash(QMetaType.Type keyMetaType, QMetaType valueMetaType) {
		this(new QMetaType(keyMetaType), valueMetaType);
	}
	
	public QMultiHash(QMetaType keyMetaType, QMetaType.Type valueMetaType) {
		this(keyMetaType, new QMetaType(valueMetaType));
	}
	
	public QMultiHash(QMetaType.Type keyMetaType, QMetaType.Type valueMetaType) {
		this(new QMetaType(keyMetaType), new QMetaType(valueMetaType));
	}
    
    public QMultiHash(Class<K> keyType, Class<V> valueType) {
		super(null);
		QMetaType keyMetaType = QMetaType.fromType(keyType);
		QMetaType valueMetaType = QMetaType.fromType(valueType);
		if(keyMetaType.id()==0)
			throw new IllegalArgumentException("QMetaType::UnknownType cannot be key type of QMultiHash.");
		if(keyMetaType.id()==QMetaType.Type.Void.value())
			throw new IllegalArgumentException("void cannot be key type of QMultiHash.");
		if(valueMetaType.id()==0)
			throw new IllegalArgumentException("QMetaType::UnknownType cannot be value type of QMultiHash.");
		if(valueMetaType.id()==QMetaType.Type.Void.value())
			throw new IllegalArgumentException("void cannot be value type of QMultiHash.");
		initialize(keyType, QtJambi_LibraryUtilities.internal.nativeId(keyMetaType), valueType, QtJambi_LibraryUtilities.internal.nativeId(valueMetaType), null);
	}
    
    public QMultiHash(Map<K,List<V>> other) {
		super(null);
		QPair<QMetaType, QMetaType> metaTypes = findMapMetaType(Objects.requireNonNull(other));
		if(metaTypes.first.id()==0)
			throw new IllegalArgumentException("QMetaType::UnknownType cannot be key type of QMultiHash.");
		if(metaTypes.first.id()==QMetaType.Type.Void.value())
			throw new IllegalArgumentException("void cannot be key type of QMultiHash.");
		if(metaTypes.second.id()==0)
			throw new IllegalArgumentException("QMetaType::UnknownType cannot be value type of QMultiHash.");
		if(metaTypes.second.id()==QMetaType.Type.Void.value())
			throw new IllegalArgumentException("void cannot be value type of QMultiHash.");
		initialize(metaTypes.first.javaType(), QtJambi_LibraryUtilities.internal.nativeId(metaTypes.first), metaTypes.second.javaType(), QtJambi_LibraryUtilities.internal.nativeId(metaTypes.second), other);
	}
    
	public QMultiHash(Class<K> keyType, QMetaType valueMetaType) {
		super(null);
		QMetaType keyMetaType = QMetaType.fromType(keyType);
		if(keyMetaType.id()==0)
			throw new IllegalArgumentException("QMetaType::UnknownType cannot be key type of QMultiHash.");
		if(keyMetaType.id()==QMetaType.Type.Void.value())
			throw new IllegalArgumentException("void cannot be key type of QMultiHash.");
		if(valueMetaType.id()==0)
			throw new IllegalArgumentException("QMetaType::UnknownType cannot be value type of QMultiHash.");
		if(valueMetaType.id()==QMetaType.Type.Void.value())
			throw new IllegalArgumentException("void cannot be value type of QMultiHash.");
		initialize(keyMetaType.javaType(), QtJambi_LibraryUtilities.internal.nativeId(keyMetaType), valueMetaType.javaType(), QtJambi_LibraryUtilities.internal.nativeId(valueMetaType), null);
	}
    
	public QMultiHash(QMetaType keyMetaType, Class<V> valueType) {
		super(null);
		QMetaType valueMetaType = QMetaType.fromType(valueType);
		if(keyMetaType.id()==0)
			throw new IllegalArgumentException("QMetaType::UnknownType cannot be key type of QMultiHash.");
		if(keyMetaType.id()==QMetaType.Type.Void.value())
			throw new IllegalArgumentException("void cannot be key type of QMultiHash.");
		if(valueMetaType.id()==0)
			throw new IllegalArgumentException("QMetaType::UnknownType cannot be value type of QMultiHash.");
		if(valueMetaType.id()==QMetaType.Type.Void.value())
			throw new IllegalArgumentException("void cannot be value type of QMultiHash.");
		initialize(keyMetaType.javaType(), QtJambi_LibraryUtilities.internal.nativeId(keyMetaType), valueType, QtJambi_LibraryUtilities.internal.nativeId(valueMetaType), null);
	}
    
	public QMultiHash(QMetaType keyMetaType, QMetaType valueMetaType) {
		super(null);
		if(keyMetaType.id()==0)
			throw new IllegalArgumentException("QMetaType::UnknownType cannot be key type of QMultiHash.");
		if(keyMetaType.id()==QMetaType.Type.Void.value())
			throw new IllegalArgumentException("void cannot be key type of QMultiHash.");
		if(valueMetaType.id()==0)
			throw new IllegalArgumentException("QMetaType::UnknownType cannot be value type of QMultiHash.");
		if(valueMetaType.id()==QMetaType.Type.Void.value())
			throw new IllegalArgumentException("void cannot be value type of QMultiHash.");
		initialize(keyMetaType.javaType(), QtJambi_LibraryUtilities.internal.nativeId(keyMetaType), valueMetaType.javaType(), QtJambi_LibraryUtilities.internal.nativeId(valueMetaType), null);
	}
    
    @QtUninvokable
    private native void initialize(Class<?> keyType, long keyMetaType, Class<?> valueType, long valueMetaType, Map<K,List<V>> other);
    
    @Override
	public QMultiHash<K,V> clone(){
		return new QMultiHash<>(this);
	}
    
    @QtUninvokable
    public final int capacity()    {
        return QHash.capacity(QtJambi_LibraryUtilities.internal.nativeId(this));
    }

    @QtUninvokable
    public final void clear()    {
    	QHash.clear(QtJambi_LibraryUtilities.internal.nativeId(this));
    }

    @QtUninvokable
    public final boolean contains(K key)    {
        try{
        	return QHash.contains(QtJambi_LibraryUtilities.internal.nativeId(this), key);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QSet.handleException(e, keyMetaType(), key);
        }
    }

    @QtUninvokable
    public final int count()    {
        return size();
    }

    @QtUninvokable
    public final int count(K key)    {
        try{
        	return QHash.count(QtJambi_LibraryUtilities.internal.nativeId(this), key);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QSet.handleException(e, keyMetaType(), key);
        }
    }
    
	@QtUninvokable
    protected final io.qt.core.QAssociativeIterator<K,V> begin()    {
        return QHash.begin(QtJambi_LibraryUtilities.internal.nativeId(this));
    }

    @QtUninvokable
    protected final io.qt.core.QAssociativeIterator<K,V> end()    {
        return QHash.end(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    
	@QtUninvokable
    protected final io.qt.core.QAssociativeConstIterator<K,V> constBegin()    {
        return QHash.constBegin(QtJambi_LibraryUtilities.internal.nativeId(this));
    }

    @QtUninvokable
    protected final io.qt.core.QAssociativeConstIterator<K,V> constEnd()    {
        return QHash.constEnd(QtJambi_LibraryUtilities.internal.nativeId(this));
    }

    @QtUninvokable
    public final io.qt.core.QAssociativeConstIterator<K,V> find(K key)    {
        return QHash.find(QtJambi_LibraryUtilities.internal.nativeId(this), key);
    }

    @QtUninvokable
    public final void insert(K key, V value)    {
        try{
        	QHash.insert(QtJambi_LibraryUtilities.internal.nativeId(this), key, value);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QMap.handleException(e, keyMetaType(), valueMetaType(), key, value);
        }
    }

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
        	return QHash.key(QtJambi_LibraryUtilities.internal.nativeId(this), value, defaultKey);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QMap.handleException(e, keyMetaType(), valueMetaType(), defaultKey, value);
        }
    }

    @QtUninvokable
    public final java.util.List<K> keys()    {
        return QHash.keys(QtJambi_LibraryUtilities.internal.nativeId(this));
    }

    @QtUninvokable
    public final java.util.List<K> keys(V value)    {
        try{
        	return keysForValue(QtJambi_LibraryUtilities.internal.nativeId(this), value);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QSet.handleException(e, valueMetaType(), value);
        }
    }

    @QtUninvokable
    private final boolean operator_equal(java.util.Map<K,V> other)    {
        return QHash.operator_equal(QtJambi_LibraryUtilities.internal.nativeId(this), other);
    }

    @QtUninvokable
    public final int removeAll(Object key)    {
        try{
        	return QHash.remove(QtJambi_LibraryUtilities.internal.nativeId(this), key);
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
    public final void reserve(int size)    {
    	QHash.reserve(QtJambi_LibraryUtilities.internal.nativeId(this), size);
    }

    @QtUninvokable
    public final int size()    {
        return QHash.size(QtJambi_LibraryUtilities.internal.nativeId(this));
    }

    @QtUninvokable
    public final V take(K key)    {
        try{
        	return QHash.take(QtJambi_LibraryUtilities.internal.nativeId(this), key);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QSet.handleException(e, keyMetaType(), key);
        }
    }

    @QtUninvokable
    public final java.util.List<K> uniqueKeys()    {
        return uniqueKeys(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private static native <K> java.util.List<K> uniqueKeys(long __this__nativeId);

    @QtUninvokable
    public final void unite(java.util.Map<? super K,? extends java.util.Collection<? super V>> other)    {
        unite(QtJambi_LibraryUtilities.internal.nativeId(this), other);
    }
    @QtUninvokable
    private static native void unite(long __this__nativeId, Object other);

    @QtUninvokable
    public final V value(K key) {
        return value(key, null);
    }
    
    @QtUninvokable
    public final V value(K key, V defaultValue)    {
        try{
        	return QHash.value(QtJambi_LibraryUtilities.internal.nativeId(this), key, defaultValue);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QMap.handleException(e, keyMetaType(), valueMetaType(), key, defaultValue);
        }
    }

    @QtUninvokable
    public final java.util.Collection<java.util.List<V>> values()    {
    	long id = QtJambi_LibraryUtilities.internal.nativeId(this);
    	java.util.List<java.util.List<V>> result = new java.util.ArrayList<>();
        for(Object key : uniqueKeys(id)) {
            result.add(valuesKey(id, key));
        }
        return result;
    }

    @QtUninvokable
    public final java.util.List<V> values(K key)    {
        try{
        	return valuesKey(QtJambi_LibraryUtilities.internal.nativeId(this), key);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QSet.handleException(e, keyMetaType(), key);
        }
    }
    @QtUninvokable
    private static native <K,V> java.util.List<V> valuesKey(long __this__nativeId, K key);
    
    @QtUninvokable
    public final int count(K key, V value)    {
        try{
        	return count(QtJambi_LibraryUtilities.internal.nativeId(this), key, value);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QMap.handleException(e, keyMetaType(), valueMetaType(), key, value);
        }
    }
    private static native <K,V> int count(long __this__nativeId, K key, V value);

    @QtUninvokable
    public final io.qt.core.QAssociativeConstIterator<K,V> find(K key, V value)    {
        try{
        	return find(QtJambi_LibraryUtilities.internal.nativeId(this), key, value);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QMap.handleException(e, keyMetaType(), valueMetaType(), key, value);
        }
    }
    private static native <K,V> io.qt.core.QAssociativeConstIterator<K,V> find(long __this__nativeId, K key, V value);
    
    @QtUninvokable
    public final int removeAll(K key, V value)    {
    	try{
    		return remove(QtJambi_LibraryUtilities.internal.nativeId(this), key, value);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QMap.handleException(e, keyMetaType(), valueMetaType(), key, value);
    	}
    }
    private static native <K,V> int remove(long __this__nativeId, K key, V value);
    
    @QtUninvokable
    public final void replaceOne(K key, V value)    {
        try{
        	replaceOne(QtJambi_LibraryUtilities.internal.nativeId(this), key, value);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QMap.handleException(e, keyMetaType(), valueMetaType(), key, value);
        }
    }
    private static native <K,V> void replaceOne(long __this__nativeId, K key, V value);
    
    @QtUninvokable
    public final boolean contains(K key, V value)    {
        try{
        	return contains(QtJambi_LibraryUtilities.internal.nativeId(this), key, value);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QMap.handleException(e, keyMetaType(), valueMetaType(), key, value);
        }
    }
    private static native <K,V> boolean contains(long __this__nativeId, K key, V value);

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
    
    @Override
    @QtUninvokable
    public final boolean containsKey(Object key){
        try{
        	return QHash.contains(QtJambi_LibraryUtilities.internal.nativeId(this), key);
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
    public final List<V> get(Object key){
		try{
			return valuesKey(QtJambi_LibraryUtilities.internal.nativeId(this), key);
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
    public final java.util.List<V> put(K key, java.util.List<V> values){
    	java.util.List<V> old = values(key);
    	for(V value : values)
    		insert(key, value);
        return old;
    }

    @Override
    @QtUninvokable
    public final java.util.List<V> remove(Object key){
        try{
        	@SuppressWarnings("unchecked")
			java.util.List<V> result = valuesKey(QtJambi_LibraryUtilities.internal.nativeId(this), (K)key);
        	removeAll(key);
        	return result;
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(IllegalArgumentException e) {
    		return Collections.emptyList();
    	}catch(RuntimeException e) {
    		RuntimeException e1 = QSet.handleException(e, keyMetaType(), key);
    		if(e1==e)
    			throw e;
    		return Collections.emptyList();
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
    	return QHash.keyMetaType(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    
    @io.qt.QtUninvokable
    final QMetaType valueMetaType() {
    	return QHash.valueMetaType(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    
    /**
     * Returns a QMultiHash containing a single mapping.
     *
     * @param <K> the {@code QMultiHash}'s key type
     * @param <V> the {@code QMultiHash}'s value type
     * @param k1 the mapping's key
     * @param v1 the mapping's value
     * @return a {@code QMultiHash} containing the specified mapping
     * @throws NullPointerException if the key or the value is {@code null}
     */
    public static <K, V> QMultiHash<K, V> of(K k1, V v1) {
    	QMultiHash<K, V> result = new QMultiHash<>(
    			QList.findElementMetaType(k1), 
    			QList.findElementMetaType(v1));
    	result.insert(k1, v1);
        return result;
    }

    /**
     * Returns a QMultiHash containing two mappings.
     *
     * @param <K> the {@code QMultiHash}'s key type
     * @param <V> the {@code QMultiHash}'s value type
     * @param k1 the first mapping's key
     * @param v1 the first mapping's value
     * @param k2 the second mapping's key
     * @param v2 the second mapping's value
     * @return a {@code QMultiHash} containing the specified mappings
     * @throws NullPointerException if any key or value is {@code null}
     */
    public static <K, V> QMultiHash<K, V> of(K k1, V v1, K k2, V v2) {
    	QMultiHash<K, V> result = new QMultiHash<>(
    			QList.findElementMetaType(k1, k2), 
    			QList.findElementMetaType(v1, v2));
    	result.insert(k1, v1);
    	result.insert(k2, v2);
        return result;
    }

    /**
     * Returns a QMultiHash containing three mappings.
     *
     * @param <K> the {@code QMultiHash}'s key type
     * @param <V> the {@code QMultiHash}'s value type
     * @param k1 the first mapping's key
     * @param v1 the first mapping's value
     * @param k2 the second mapping's key
     * @param v2 the second mapping's value
     * @param k3 the third mapping's key
     * @param v3 the third mapping's value
     * @return a {@code QMultiHash} containing the specified mappings
     * @throws NullPointerException if any key or value is {@code null}
     */
    public static <K, V> QMultiHash<K, V> of(K k1, V v1, K k2, V v2, K k3, V v3) {
    	QMultiHash<K, V> result = new QMultiHash<>(
    			QList.findElementMetaType(k1, k2, k3), 
    			QList.findElementMetaType(v1, v2, v3));
    	result.insert(k1, v1);
    	result.insert(k2, v2);
    	result.insert(k3, v3);
        return result;
    }

    /**
     * Returns a QMultiHash containing four mappings.
     *
     * @param <K> the {@code QMultiHash}'s key type
     * @param <V> the {@code QMultiHash}'s value type
     * @param k1 the first mapping's key
     * @param v1 the first mapping's value
     * @param k2 the second mapping's key
     * @param v2 the second mapping's value
     * @param k3 the third mapping's key
     * @param v3 the third mapping's value
     * @param k4 the fourth mapping's key
     * @param v4 the fourth mapping's value
     * @return a {@code QMultiHash} containing the specified mappings
     * @throws NullPointerException if any key or value is {@code null}
     */
    public static <K, V> QMultiHash<K, V> of(K k1, V v1, K k2, V v2, K k3, V v3, K k4, V v4) {
    	QMultiHash<K, V> result = new QMultiHash<>(
    			QList.findElementMetaType(k1, k2, k3, k4), 
    			QList.findElementMetaType(v1, v2, v3, v4));
    	result.insert(k1, v1);
    	result.insert(k2, v2);
    	result.insert(k3, v3);
    	result.insert(k4, v4);
        return result;
    }

    /**
     * Returns a QMultiHash containing five mappings.
     *
     * @param <K> the {@code QMultiHash}'s key type
     * @param <V> the {@code QMultiHash}'s value type
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
     * @return a {@code QMultiHash} containing the specified mappings
     * @throws NullPointerException if any key or value is {@code null}
     */
    public static <K, V> QMultiHash<K, V> of(K k1, V v1, K k2, V v2, K k3, V v3, K k4, V v4, K k5, V v5) {
    	QMultiHash<K, V> result = new QMultiHash<>(
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
     * Returns a QMultiHash containing six mappings.
     *
     * @param <K> the {@code QMultiHash}'s key type
     * @param <V> the {@code QMultiHash}'s value type
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
     * @return a {@code QMultiHash} containing the specified mappings
     * @throws NullPointerException if any key or value is {@code null}
     */
    public static <K, V> QMultiHash<K, V> of(K k1, V v1, K k2, V v2, K k3, V v3, K k4, V v4, K k5, V v5,
                               K k6, V v6) {
    	QMultiHash<K, V> result = new QMultiHash<>(
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
     * Returns a QMultiHash containing seven mappings.
     *
     * @param <K> the {@code QMultiHash}'s key type
     * @param <V> the {@code QMultiHash}'s value type
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
     * @return a {@code QMultiHash} containing the specified mappings
     * @throws NullPointerException if any key or value is {@code null}
     */
    public static <K, V> QMultiHash<K, V> of(K k1, V v1, K k2, V v2, K k3, V v3, K k4, V v4, K k5, V v5,
                               K k6, V v6, K k7, V v7) {
    	QMultiHash<K, V> result = new QMultiHash<>(
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
     * Returns a QMultiHash containing eight mappings.
     *
     * @param <K> the {@code QMultiHash}'s key type
     * @param <V> the {@code QMultiHash}'s value type
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
     * @return a {@code QMultiHash} containing the specified mappings
     * @throws NullPointerException if any key or value is {@code null}
     */
    public static <K, V> QMultiHash<K, V> of(K k1, V v1, K k2, V v2, K k3, V v3, K k4, V v4, K k5, V v5,
                               K k6, V v6, K k7, V v7, K k8, V v8) {
    	QMultiHash<K, V> result = new QMultiHash<>(
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
     * Returns a QMultiHash containing nine mappings.
     *
     * @param <K> the {@code QMultiHash}'s key type
     * @param <V> the {@code QMultiHash}'s value type
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
     * @return a {@code QMultiHash} containing the specified mappings
     * @throws NullPointerException if any key or value is {@code null}
     */
    public static <K, V> QMultiHash<K, V> of(K k1, V v1, K k2, V v2, K k3, V v3, K k4, V v4, K k5, V v5,
                               K k6, V v6, K k7, V v7, K k8, V v8, K k9, V v9) {
    	QMultiHash<K, V> result = new QMultiHash<>(
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
     * Returns a QMultiHash containing ten mappings.
     *
     * @param <K> the {@code QMultiHash}'s key type
     * @param <V> the {@code QMultiHash}'s value type
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
     * @return a {@code QMultiHash} containing the specified mappings
     * @throws NullPointerException if any key or value is {@code null}
     */
    public static <K, V> QMultiHash<K, V> of(K k1, V v1, K k2, V v2, K k3, V v3, K k4, V v4, K k5, V v5,
                               K k6, V v6, K k7, V v7, K k8, V v8, K k9, V v9, K k10, V v10) {
    	QMultiHash<K, V> result = new QMultiHash<>(
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
     * Returns a QMultiHash containing keys and values extracted from the given entries.
     *
     * @apiNote
     * It is convenient to create the map entries using the {@link Map#entry Map.entry()} method.
     * For example,
     *
     * <pre>{@code
     *     import static java.util.Map.entry;
     *
     *     QMultiHash<Integer,String> map = QMultiHash.ofEntries(
     *         entry(1, "a"),
     *         entry(2, "b"),
     *         entry(3, "c"),
     *         ...
     *         entry(26, "z"));
     * }</pre>
     *
     * @param <K> the {@code QMultiHash}'s key type
     * @param <V> the {@code QMultiHash}'s value type
     * @param entries {@code java.util.Map.Entry}s containing the keys and values from which the map is populated
     * @return a {@code QMultiHash} containing the specified mappings
     * @throws NullPointerException if any entry, key, or value is {@code null}, or if
     *         the {@code entries} array is {@code null}
     */
    @SafeVarargs
    public static <K, V> QMultiHash<K, V> ofEntries(Map.Entry<? extends K, ? extends V> entry0, Map.Entry<? extends K, ? extends V>... entries) {
    	QPair<QMetaType, QMetaType> metaTypes = findMapMetaType(entry0, entries);
    	QMultiHash<K, V> result = new QMultiHash<>(metaTypes.first, metaTypes.second);
    	result.insert(entry0.getKey(), entry0.getValue());
    	for (Map.Entry<? extends K, ? extends V> entry : entries) {
        	result.insert(entry.getKey(), entry.getValue());
		}
        return result;
    }
}
