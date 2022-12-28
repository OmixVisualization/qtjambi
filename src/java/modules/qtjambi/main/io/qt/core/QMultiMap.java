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

import java.util.Collections;
import java.util.List;
import java.util.Map;
import java.util.Objects;

import io.qt.NativeAccess;
import io.qt.QNoImplementationException;
import io.qt.QNoNativeResourcesException;
import io.qt.QtUninvokable;

/**
 * <p>Java wrapper for Qt class <a href="https://doc.qt.io/qt/qmultimap.html">QMultiMap</a></p>
 */
public class QMultiMap<K,V> extends io.qt.internal.AbstractMultiMap<K,V> implements Cloneable
{
	static {
    	QtJambi_LibraryUtilities.initialize();
    }

    @NativeAccess
    protected QMultiMap(QPrivateConstructor p) {
		super(p);
	}
	
	public QMultiMap(Class<K> keyType, QMetaType.Type valueMetaType) {
		this(keyType, new QMetaType(valueMetaType));
	}
	
	public QMultiMap(QMetaType.Type keyMetaType, Class<V> valueType) {
		this(new QMetaType(keyMetaType), valueType);
	}
	
	public QMultiMap(QMetaType.Type keyMetaType, QMetaType valueMetaType) {
		this(new QMetaType(keyMetaType), valueMetaType);
	}
	
	public QMultiMap(QMetaType keyMetaType, QMetaType.Type valueMetaType) {
		this(keyMetaType, new QMetaType(valueMetaType));
	}
	
	public QMultiMap(QMetaType.Type keyMetaType, QMetaType.Type valueMetaType) {
		this(new QMetaType(keyMetaType), new QMetaType(valueMetaType));
	}
    
    public QMultiMap(Class<K> keyType, Class<V> valueType) {
		super(null);
		QMetaType keyMetaType = QMetaType.fromType(keyType);
		QMetaType valueMetaType = QMetaType.fromType(valueType);
		if(keyMetaType.id()==0)
			throw new IllegalArgumentException("QMetaType::UnknownType cannot be key type of QMultiMap.");
		if(keyMetaType.id()==QMetaType.Type.Void.value())
			throw new IllegalArgumentException("void cannot be key type of QMultiMap.");
		if(valueMetaType.id()==0)
			throw new IllegalArgumentException("QMetaType::UnknownType cannot be value type of QMultiMap.");
		if(valueMetaType.id()==QMetaType.Type.Void.value())
			throw new IllegalArgumentException("void cannot be value type of QMultiMap.");
		initialize(keyType, QtJambi_LibraryUtilities.internal.nativeId(keyMetaType), valueType, QtJambi_LibraryUtilities.internal.nativeId(valueMetaType), null);
	}
    
    public QMultiMap(Map<K,List<V>> other) {
		super(null);
		QPair<QMetaType, QMetaType> metaTypes = findMapMetaType(Objects.requireNonNull(other));
		if(metaTypes.first.id()==0)
			throw new IllegalArgumentException("QMetaType::UnknownType cannot be key type of QMultiMap.");
		if(metaTypes.first.id()==QMetaType.Type.Void.value())
			throw new IllegalArgumentException("void cannot be key type of QMultiMap.");
		if(metaTypes.second.id()==0)
			throw new IllegalArgumentException("QMetaType::UnknownType cannot be value type of QMultiMap.");
		if(metaTypes.second.id()==QMetaType.Type.Void.value())
			throw new IllegalArgumentException("void cannot be value type of QMultiMap.");
		initialize(metaTypes.first.javaType(), QtJambi_LibraryUtilities.internal.nativeId(metaTypes.first), metaTypes.second.javaType(), QtJambi_LibraryUtilities.internal.nativeId(metaTypes.second), other);
	}
    
	public QMultiMap(Class<K> keyType, QMetaType valueMetaType) {
		super(null);
		QMetaType keyMetaType = QMetaType.fromType(keyType);
		if(keyMetaType.id()==0)
			throw new IllegalArgumentException("QMetaType::UnknownType cannot be key type of QMultiMap.");
		if(keyMetaType.id()==QMetaType.Type.Void.value())
			throw new IllegalArgumentException("void cannot be key type of QMultiMap.");
		if(valueMetaType.id()==0)
			throw new IllegalArgumentException("QMetaType::UnknownType cannot be value type of QMultiMap.");
		if(valueMetaType.id()==QMetaType.Type.Void.value())
			throw new IllegalArgumentException("void cannot be value type of QMultiMap.");
		initialize(keyType, QtJambi_LibraryUtilities.internal.nativeId(keyMetaType), valueMetaType.javaType(), QtJambi_LibraryUtilities.internal.nativeId(valueMetaType), null);
	}
    
	public QMultiMap(QMetaType keyMetaType, Class<V> valueType) {
		super(null);
		QMetaType valueMetaType = QMetaType.fromType(valueType);
		if(keyMetaType.id()==0)
			throw new IllegalArgumentException("QMetaType::UnknownType cannot be key type of QMultiMap.");
		if(keyMetaType.id()==QMetaType.Type.Void.value())
			throw new IllegalArgumentException("void cannot be key type of QMultiMap.");
		if(valueMetaType.id()==0)
			throw new IllegalArgumentException("QMetaType::UnknownType cannot be value type of QMultiMap.");
		if(valueMetaType.id()==QMetaType.Type.Void.value())
			throw new IllegalArgumentException("void cannot be value type of QMultiMap.");
		initialize(keyMetaType.javaType(), QtJambi_LibraryUtilities.internal.nativeId(keyMetaType), valueMetaType.javaType(), QtJambi_LibraryUtilities.internal.nativeId(valueMetaType), null);
	}
    
	public QMultiMap(QMetaType keyMetaType, QMetaType valueMetaType) {
		super(null);
		if(keyMetaType.id()==0)
			throw new IllegalArgumentException("QMetaType::UnknownType cannot be key type of QMultiMap.");
		if(keyMetaType.id()==QMetaType.Type.Void.value())
			throw new IllegalArgumentException("void cannot be key type of QMultiMap.");
		if(valueMetaType.id()==0)
			throw new IllegalArgumentException("QMetaType::UnknownType cannot be value type of QMultiMap.");
		if(valueMetaType.id()==QMetaType.Type.Void.value())
			throw new IllegalArgumentException("void cannot be value type of QMultiMap.");
		initialize(keyMetaType.javaType(), QtJambi_LibraryUtilities.internal.nativeId(keyMetaType), valueMetaType.javaType(), QtJambi_LibraryUtilities.internal.nativeId(valueMetaType), null);
	}
    
    @QtUninvokable
    private native void initialize(Class<?> keyType, long keyMetaType, Class<?> valueType, long valueMetaType, Map<K, List<V>> other);
    
    @Override
	public QMultiMap<K,V> clone(){
		return new QMultiMap<>(this);
	}

    @QtUninvokable
    public final void clear()    {
    	QMap.clear(QtJambi_LibraryUtilities.internal.nativeId(this));
    }

    @QtUninvokable
    public final boolean contains(K key)    {
        try{
        	return QMap.contains(QtJambi_LibraryUtilities.internal.nativeId(this), key);
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
        	return QMap.count(QtJambi_LibraryUtilities.internal.nativeId(this), key);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QSet.handleException(e, keyMetaType(), key);
        }
    }
    
	@QtUninvokable
    protected final io.qt.core.QAssociativeIterator<K,V> begin()    {
        return QMap.begin(QtJambi_LibraryUtilities.internal.nativeId(this));
    }

    @QtUninvokable
    protected final io.qt.core.QAssociativeIterator<K,V> end()    {
        return QMap.end(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    
	@QtUninvokable
    protected final io.qt.core.QAssociativeConstIterator<K,V> constBegin()    {
        return QMap.constBegin(QtJambi_LibraryUtilities.internal.nativeId(this));
    }

    @QtUninvokable
    protected final io.qt.core.QAssociativeConstIterator<K,V> constEnd()    {
        return QMap.constEnd(QtJambi_LibraryUtilities.internal.nativeId(this));
    }

    @QtUninvokable
    public final io.qt.core.QAssociativeConstIterator<K,V> find(K key)    {
        try{
        	return QMap.find(QtJambi_LibraryUtilities.internal.nativeId(this), key);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QSet.handleException(e, keyMetaType(), key);
        }
    }

    @QtUninvokable
    public final V first()    {
        return QMap.first(QtJambi_LibraryUtilities.internal.nativeId(this));
    }

    @QtUninvokable
    public final K firstKey()    {
        return QMap.firstKey(QtJambi_LibraryUtilities.internal.nativeId(this));
    }

    @QtUninvokable
    public final void insert(K key, V value)    {
        try{
        	QMap.insert(QtJambi_LibraryUtilities.internal.nativeId(this), key, value);
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
        	return QMap.key(QtJambi_LibraryUtilities.internal.nativeId(this), value, defaultKey);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QSet.handleException(e, valueMetaType(), value);
        }
    }

    @QtUninvokable
    public final java.util.List<K> keys()    {
        return QMap.keys(QtJambi_LibraryUtilities.internal.nativeId(this));
    }

    @QtUninvokable
    public final java.util.List<K> keys(V value)    {
        try{
        	return QMap.keysForValue(QtJambi_LibraryUtilities.internal.nativeId(this), value);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QSet.handleException(e, valueMetaType(), value);
        }
    }

    @QtUninvokable
    public final V last()    {
        return QMap.last(QtJambi_LibraryUtilities.internal.nativeId(this));
    }

    @QtUninvokable
    public final K lastKey()    {
        return QMap.lastKey(QtJambi_LibraryUtilities.internal.nativeId(this));
    }

    @QtUninvokable
    public final io.qt.core.QAssociativeConstIterator<K,V> lowerBound(K key)    {
        try{
        	return QMap.lowerBound(QtJambi_LibraryUtilities.internal.nativeId(this), key);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QSet.handleException(e, keyMetaType(), key);
        }
    }

    @QtUninvokable
    private final boolean operator_equal(java.util.Map<K,V> other)    {
        return QMap.operator_equal(QtJambi_LibraryUtilities.internal.nativeId(this), other);
    }

    @QtUninvokable
    public final int removeAll(Object key)    {
        try{
        	return QMap.remove(QtJambi_LibraryUtilities.internal.nativeId(this), key);
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
    public final int size()    {
        return QMap.size(QtJambi_LibraryUtilities.internal.nativeId(this));
    }

    @QtUninvokable
    public final V take(K key)    {
    	try {
    		return QMap.take(QtJambi_LibraryUtilities.internal.nativeId(this), key);
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
    public final io.qt.core.QAssociativeConstIterator<K,V> upperBound(K key)    {
        return QMap.upperBound(QtJambi_LibraryUtilities.internal.nativeId(this), key);
    }


    @QtUninvokable
    public final V value(K key) {
        return value(key, null);
    }
    
    @QtUninvokable
    public final V value(K key, V defaultValue)    {
        try{
        	return QMap.value(QtJambi_LibraryUtilities.internal.nativeId(this), key, defaultValue);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QMap.handleException(e, keyMetaType(), valueMetaType(), key, defaultValue);
        }
    }

    @Override
    @QtUninvokable
    public final java.util.Collection<java.util.List<V>> values() {
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
    public final boolean contains(K key, V value)    {
        try{
        	return contains(QtJambi_LibraryUtilities.internal.nativeId(this), key, value);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QMap.handleException(e, keyMetaType(), valueMetaType(), key, value);
        }
    }
    @QtUninvokable
    private static native <K,V> boolean contains(long __this__nativeId, K key, V value);
    
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
    @QtUninvokable
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
    @QtUninvokable
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
    @QtUninvokable
    private static native <K,V> int remove(long __this__nativeId, K key, V value);
    
    @QtUninvokable
    public final void replaceOne(K key, V value)    {
        try{
        	replace(QtJambi_LibraryUtilities.internal.nativeId(this), key, value);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QMap.handleException(e, keyMetaType(), valueMetaType(), key, value);
        }
    }
    @QtUninvokable
    private static native <K,V> void replace(long __this__nativeId, K key, V value);

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
    public final java.util.Comparator<K> comparator(){
    	return QMap.createComparator(keyMetaType().javaType(), this);
    }

    @Override
    @QtUninvokable
    public final boolean containsKey(Object key){
        try{
        	return QMap.contains(QtJambi_LibraryUtilities.internal.nativeId(this), key);
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

    @SuppressWarnings("unchecked")
	@Override
    @QtUninvokable
    public final java.util.List<V> remove(Object key){
        try{
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
    		return QMap.keysForValue(QtJambi_LibraryUtilities.internal.nativeId(this), value).isEmpty();
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
    	return QMap.keyMetaType(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    
    @io.qt.QtUninvokable
    final QMetaType valueMetaType() {
    	return QMap.valueMetaType(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    
    /**
     * Returns a QMultiMap containing a single mapping.
     *
     * @param <K> the {@code QMultiMap}'s key type
     * @param <V> the {@code QMultiMap}'s value type
     * @param k1 the mapping's key
     * @param v1 the mapping's value
     * @return a {@code QMultiMap} containing the specified mapping
     * @throws NullPointerException if the key or the value is {@code null}
     */
    public static <K, V> QMultiMap<K, V> of(K k1, V v1) {
    	QMultiMap<K, V> result = new QMultiMap<>(
    			QList.findElementMetaType(k1), 
    			QList.findElementMetaType(v1));
    	result.insert(k1, v1);
        return result;
    }

    /**
     * Returns a QMultiMap containing two mappings.
     *
     * @param <K> the {@code QMultiMap}'s key type
     * @param <V> the {@code QMultiMap}'s value type
     * @param k1 the first mapping's key
     * @param v1 the first mapping's value
     * @param k2 the second mapping's key
     * @param v2 the second mapping's value
     * @return a {@code QMultiMap} containing the specified mappings
     * @throws NullPointerException if any key or value is {@code null}
     */
    public static <K, V> QMultiMap<K, V> of(K k1, V v1, K k2, V v2) {
    	QMultiMap<K, V> result = new QMultiMap<>(
    			QList.findElementMetaType(k1, k2), 
    			QList.findElementMetaType(v1, v2));
    	result.insert(k1, v1);
    	result.insert(k2, v2);
        return result;
    }

    /**
     * Returns a QMultiMap containing three mappings.
     *
     * @param <K> the {@code QMultiMap}'s key type
     * @param <V> the {@code QMultiMap}'s value type
     * @param k1 the first mapping's key
     * @param v1 the first mapping's value
     * @param k2 the second mapping's key
     * @param v2 the second mapping's value
     * @param k3 the third mapping's key
     * @param v3 the third mapping's value
     * @return a {@code QMultiMap} containing the specified mappings
     * @throws NullPointerException if any key or value is {@code null}
     */
    public static <K, V> QMultiMap<K, V> of(K k1, V v1, K k2, V v2, K k3, V v3) {
    	QMultiMap<K, V> result = new QMultiMap<>(
    			QList.findElementMetaType(k1, k2, k3), 
    			QList.findElementMetaType(v1, v2, v3));
    	result.insert(k1, v1);
    	result.insert(k2, v2);
    	result.insert(k3, v3);
        return result;
    }

    /**
     * Returns a QMultiMap containing four mappings.
     *
     * @param <K> the {@code QMultiMap}'s key type
     * @param <V> the {@code QMultiMap}'s value type
     * @param k1 the first mapping's key
     * @param v1 the first mapping's value
     * @param k2 the second mapping's key
     * @param v2 the second mapping's value
     * @param k3 the third mapping's key
     * @param v3 the third mapping's value
     * @param k4 the fourth mapping's key
     * @param v4 the fourth mapping's value
     * @return a {@code QMultiMap} containing the specified mappings
     * @throws NullPointerException if any key or value is {@code null}
     */
    public static <K, V> QMultiMap<K, V> of(K k1, V v1, K k2, V v2, K k3, V v3, K k4, V v4) {
    	QMultiMap<K, V> result = new QMultiMap<>(
    			QList.findElementMetaType(k1, k2, k3, k4), 
    			QList.findElementMetaType(v1, v2, v3, v4));
    	result.insert(k1, v1);
    	result.insert(k2, v2);
    	result.insert(k3, v3);
    	result.insert(k4, v4);
        return result;
    }

    /**
     * Returns a QMultiMap containing five mappings.
     *
     * @param <K> the {@code QMultiMap}'s key type
     * @param <V> the {@code QMultiMap}'s value type
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
     * @return a {@code QMultiMap} containing the specified mappings
     * @throws NullPointerException if any key or value is {@code null}
     */
    public static <K, V> QMultiMap<K, V> of(K k1, V v1, K k2, V v2, K k3, V v3, K k4, V v4, K k5, V v5) {
    	QMultiMap<K, V> result = new QMultiMap<>(
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
     * Returns a QMultiMap containing six mappings.
     *
     * @param <K> the {@code QMultiMap}'s key type
     * @param <V> the {@code QMultiMap}'s value type
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
     * @return a {@code QMultiMap} containing the specified mappings
     * @throws NullPointerException if any key or value is {@code null}
     */
    public static <K, V> QMultiMap<K, V> of(K k1, V v1, K k2, V v2, K k3, V v3, K k4, V v4, K k5, V v5,
                               K k6, V v6) {
    	QMultiMap<K, V> result = new QMultiMap<>(
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
     * Returns a QMultiMap containing seven mappings.
     *
     * @param <K> the {@code QMultiMap}'s key type
     * @param <V> the {@code QMultiMap}'s value type
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
     * @return a {@code QMultiMap} containing the specified mappings
     * @throws NullPointerException if any key or value is {@code null}
     */
    public static <K, V> QMultiMap<K, V> of(K k1, V v1, K k2, V v2, K k3, V v3, K k4, V v4, K k5, V v5,
                               K k6, V v6, K k7, V v7) {
    	QMultiMap<K, V> result = new QMultiMap<>(
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
     * Returns a QMultiMap containing eight mappings.
     *
     * @param <K> the {@code QMultiMap}'s key type
     * @param <V> the {@code QMultiMap}'s value type
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
     * @return a {@code QMultiMap} containing the specified mappings
     * @throws NullPointerException if any key or value is {@code null}
     */
    public static <K, V> QMultiMap<K, V> of(K k1, V v1, K k2, V v2, K k3, V v3, K k4, V v4, K k5, V v5,
                               K k6, V v6, K k7, V v7, K k8, V v8) {
    	QMultiMap<K, V> result = new QMultiMap<>(
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
     * Returns a QMultiMap containing nine mappings.
     *
     * @param <K> the {@code QMultiMap}'s key type
     * @param <V> the {@code QMultiMap}'s value type
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
     * @return a {@code QMultiMap} containing the specified mappings
     * @throws NullPointerException if any key or value is {@code null}
     */
    public static <K, V> QMultiMap<K, V> of(K k1, V v1, K k2, V v2, K k3, V v3, K k4, V v4, K k5, V v5,
                               K k6, V v6, K k7, V v7, K k8, V v8, K k9, V v9) {
    	QMultiMap<K, V> result = new QMultiMap<>(
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
     * Returns a QMultiMap containing ten mappings.
     *
     * @param <K> the {@code QMultiMap}'s key type
     * @param <V> the {@code QMultiMap}'s value type
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
     * @return a {@code QMultiMap} containing the specified mappings
     * @throws NullPointerException if any key or value is {@code null}
     */
    public static <K, V> QMultiMap<K, V> of(K k1, V v1, K k2, V v2, K k3, V v3, K k4, V v4, K k5, V v5,
                               K k6, V v6, K k7, V v7, K k8, V v8, K k9, V v9, K k10, V v10) {
    	QMultiMap<K, V> result = new QMultiMap<>(
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
     * Returns a QMultiMap containing keys and values extracted from the given entries.
     *
     * @apiNote
     * It is convenient to create the map entries using the {@link Map#entry Map.entry()} method.
     * For example,
     *
     * <pre>{@code
     *     import static java.util.Map.entry;
     *
     *     QMultiMap<Integer,String> map = QMultiMap.ofEntries(
     *         entry(1, "a"),
     *         entry(2, "b"),
     *         entry(3, "c"),
     *         ...
     *         entry(26, "z"));
     * }</pre>
     *
     * @param <K> the {@code QMultiMap}'s key type
     * @param <V> the {@code QMultiMap}'s value type
     * @param entries {@code java.util.Map.Entry}s containing the keys and values from which the map is populated
     * @return a {@code QMultiMap} containing the specified mappings
     * @throws NullPointerException if any entry, key, or value is {@code null}, or if
     *         the {@code entries} array is {@code null}
     */
    @SafeVarargs
    public static <K, V> QMultiMap<K, V> ofEntries(Map.Entry<? extends K, ? extends V> entry0, Map.Entry<? extends K, ? extends V>... entries) {
    	QPair<QMetaType, QMetaType> metaTypes = QMap.findMapMetaType(entry0, entries);
    	QMultiMap<K, V> result = new QMultiMap<>(metaTypes.first, metaTypes.second);
    	result.insert(entry0.getKey(), entry0.getValue());
    	for (Map.Entry<? extends K, ? extends V> entry : entries) {
        	result.insert(entry.getKey(), entry.getValue());
		}
        return result;
    }
    
    private static <K,V> QPair<QMetaType,QMetaType> findMapMetaType(Map<K,List<V>> elements){
        if(elements.getClass()==QMap.class) {
            return new QPair<>(((QMap<?,?>)elements).keyMetaType(), ((QMap<?,?>)elements).valueMetaType());
        }else if(elements.getClass()==QHash.class) {
            return new QPair<>(((QHash<?,?>)elements).keyMetaType(), ((QHash<?,?>)elements).valueMetaType());
        }else if(elements.getClass()==QMultiMap.class) {
            return new QPair<>(((QMultiMap<K,V>)elements).keyMetaType(), ((QMultiMap<K,V>)elements).valueMetaType());
        }else if(elements.getClass()==QMultiHash.class) {
            return new QPair<>(((QMultiHash<K,V>)elements).keyMetaType(), ((QMultiHash<K,V>)elements).valueMetaType());
        }else {
        	QMetaType keyMetaType = new QMetaType();
        	Class<?> keyType = null;
        	QMetaType valueMetaType = new QMetaType();
        	Class<?> valueType = null;
        	for(Entry<K,List<V>> entry : elements.entrySet()) {
        		Object key = entry.getKey();
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
        		for(Object value : entry.getValue()) {
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
        	}
        	if(keyMetaType==null)
        		keyMetaType = QMetaType.fromType(keyType);
        	if(valueMetaType==null)
        		valueMetaType = QMetaType.fromType(valueType);
        	return new QPair<>(keyMetaType, valueMetaType);
        }
    }
}
