/****************************************************************************
**
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Map;
import java.util.Objects;
import java.util.function.BiPredicate;
import java.util.function.Predicate;

import io.qt.NativeAccess;
import io.qt.QNoImplementationException;
import io.qt.QNoNativeResourcesException;
import io.qt.QtUninvokable;

/**
 * <p>Java wrapper for Qt class <code><a href="https://doc.qt.io/qt/qmultihash.html">QMultiHash</a></code></p>
 */
public class QMultiHash<Key,T> extends AbstractMultiAssociativeContainer<Key,T> implements Map<Key,List<T>>, Cloneable
{
	static {
    	QtJambi_LibraryUtilities.initialize();
    }

    /**
     * Constructor for internal use only.
     * @param p expected to be <code>null</code>.
     * @hidden
     */
    @NativeAccess
    protected QMultiHash(QPrivateConstructor p) {
		super(p);
	}
	
    /**
     * Creating a container with given key and value type.
     * <p>See <code><a href="https://doc.qt.io/qt/qmultihash.html#QMultiHash">QMultiHash::<wbr>QMultiHash()</a></code></p>
     * @param keyType the type K
     * @param valueMetaType the type V
     */
	public QMultiHash(Class<Key> keyType, QMetaType.Type valueMetaType) {
		this(keyType, new QMetaType(valueMetaType));
	}
	
    /**
     * Creating a container with given key and value type.
     * <p>See <code><a href="https://doc.qt.io/qt/qmultihash.html#QMultiHash">QMultiHash::<wbr>QMultiHash()</a></code></p>
     * @param keyMetaType the type K
     * @param valueType the type V
     */
	public QMultiHash(QMetaType.Type keyMetaType, Class<T> valueType) {
		this(new QMetaType(keyMetaType), valueType);
	}
	
    /**
     * Creating a container with given key and value type.
     * <p>See <code><a href="https://doc.qt.io/qt/qmultihash.html#QMultiHash">QMultiHash::<wbr>QMultiHash()</a></code></p>
     * @param keyMetaType the type K
     * @param valueMetaType the type V
     */
	public QMultiHash(QMetaType.Type keyMetaType, QMetaType valueMetaType) {
		this(new QMetaType(keyMetaType), valueMetaType);
	}
	
    /**
     * Creating a container with given key and value type.
     * <p>See <code><a href="https://doc.qt.io/qt/qmultihash.html#QMultiHash">QMultiHash::<wbr>QMultiHash()</a></code></p>
     * @param keyMetaType the type K
     * @param valueMetaType the type V
     */
	public QMultiHash(QMetaType keyMetaType, QMetaType.Type valueMetaType) {
		this(keyMetaType, new QMetaType(valueMetaType));
	}
	
    /**
     * Creating a container with given key and value type.
     * <p>See <code><a href="https://doc.qt.io/qt/qmultihash.html#QMultiHash">QMultiHash::<wbr>QMultiHash()</a></code></p>
     * @param keyMetaType the type K
     * @param valueMetaType the type V
     */
	public QMultiHash(QMetaType.Type keyMetaType, QMetaType.Type valueMetaType) {
		this(new QMetaType(keyMetaType), new QMetaType(valueMetaType));
	}
    
    /**
     * Creating a container with given key and value type.
     * <p>See <code><a href="https://doc.qt.io/qt/qmultihash.html#QMultiHash">QMultiHash::<wbr>QMultiHash()</a></code></p>
     * @param keyType the type K
     * @param valueType the type V
     */
    public QMultiHash(Class<Key> keyType, Class<T> valueType) {
		super(null);
		QMetaType keyMetaType = QMetaType.fromType(keyType);
		QMetaType valueMetaType = QMetaType.fromType(valueType);
		initialize(keyType, QtJambi_LibraryUtilities.internal.nativeId(keyMetaType), valueType, QtJambi_LibraryUtilities.internal.nativeId(valueMetaType), null);
	}
    
    /**
     * Creating a container with given content.
     * <p>See <code><a href="https://doc.qt.io/qt/qmultihash.html#QMultiHash-5">QMultiHash::<wbr>QMultiHash(const QHash&lt;Key, T> &amp;)</a></code></p>
     * @param other map
     */
    public QMultiHash(Map<Key,List<T>> other) {
		super(null);
		QPair<QMetaType, QMetaType> metaTypes = findMapMetaType(Objects.requireNonNull(other));
		initialize(metaTypes.first.javaType(), QtJambi_LibraryUtilities.internal.nativeId(metaTypes.first), metaTypes.second.javaType(), QtJambi_LibraryUtilities.internal.nativeId(metaTypes.second), other);
	}
    
    /**
     * Creating a container with given key and value type.
     * <p>See <code><a href="https://doc.qt.io/qt/qmultihash.html#QMultiHash">QMultiHash::<wbr>QMultiHash()</a></code></p>
     * @param keyType the type K
     * @param valueMetaType the type V
     */
	public QMultiHash(Class<Key> keyType, QMetaType valueMetaType) {
		super(null);
		QMetaType keyMetaType = QMetaType.fromType(keyType);
		initialize(keyMetaType.javaType(), QtJambi_LibraryUtilities.internal.nativeId(keyMetaType), valueMetaType.javaType(), QtJambi_LibraryUtilities.internal.nativeId(valueMetaType), null);
	}
    
    /**
     * Creating a container with given key and value type.
     * <p>See <code><a href="https://doc.qt.io/qt/qmultihash.html#QMultiHash">QMultiHash::<wbr>QMultiHash()</a></code></p>
     * @param keyMetaType the type K
     * @param valueType the type V
     */
	public QMultiHash(QMetaType keyMetaType, Class<T> valueType) {
		super(null);
		QMetaType valueMetaType = QMetaType.fromType(valueType);
		initialize(keyMetaType.javaType(), QtJambi_LibraryUtilities.internal.nativeId(keyMetaType), valueType, QtJambi_LibraryUtilities.internal.nativeId(valueMetaType), null);
	}
    
    /**
     * Creating a container with given key and value type.
     * <p>See <code><a href="https://doc.qt.io/qt/qmultihash.html#QMultiHash">QMultiHash::<wbr>QMultiHash()</a></code></p>
     * @param keyMetaType the type K
     * @param valueMetaType the type V
     */
	public QMultiHash(QMetaType keyMetaType, QMetaType valueMetaType) {
		super(null);
		initialize(keyMetaType.javaType(), QtJambi_LibraryUtilities.internal.nativeId(keyMetaType), valueMetaType.javaType(), QtJambi_LibraryUtilities.internal.nativeId(valueMetaType), null);
	}
    
    @QtUninvokable
    private native void initialize(Class<?> keyType, long keyMetaType, Class<?> valueType, long valueMetaType, Map<Key,List<T>> other);
    
    /**
     * Creates and returns a copy of this object.
     */
    @Override
	public QMultiHash<Key,T> clone(){
		return new QMultiHash<>(this);
	}
    
    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qmultihash.html#capacity">QMultiHash::<wbr>capacity()const</a></code></p>
     */
    @QtUninvokable
    public final int capacity()    {
        return QHash.capacity(QtJambi_LibraryUtilities.internal.nativeId(this));
    }

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qmultihash.html#clear">QMultiHash::<wbr>clear()</a></code></p>
     */
    @QtUninvokable
    public final void clear()    {
    	QHash.clear(QtJambi_LibraryUtilities.internal.nativeId(this));
    }

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qmultihash.html#contains">QMultiHash::<wbr>contains(Key)const</a></code></p>
     */
    @QtUninvokable
    public final boolean contains(Key key)    {
        try{
        	return QHash.contains(QtJambi_LibraryUtilities.internal.nativeId(this), key);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QSet.handleException(e, keyMetaType(), key);
        }
    }

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qmultihash.html#count-1">QMultiHash::<wbr>count()const</a></code></p>
     */
    @QtUninvokable
    public final int count()    {
        return size();
    }

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qmultihash.html#count">QMultiHash::<wbr>count(Key)const</a></code></p>
     */
    @QtUninvokable
    public final int count(Key key)    {
        try{
        	return QHash.count(QtJambi_LibraryUtilities.internal.nativeId(this), key);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QSet.handleException(e, keyMetaType(), key);
        }
    }
    
    /**
     * Provides a mutable C++ iterator to the containers begin.
     * <p>See <code><a href="https://doc.qt.io/qt/qmultihash.html#begin">QMultiHash::<wbr>begin()</a></code></p>
     * @return begin
     */
	@QtUninvokable
    protected final io.qt.core.QAssociativeIterator<Key,T> begin()    {
        return QHash.begin(QtJambi_LibraryUtilities.internal.nativeId(this));
    }

    /**
     * Provides a mutable C++ iterator to the containers end.
     * <p>See <code><a href="https://doc.qt.io/qt/qmultihash.html#end">QMultiHash::<wbr>end()</a></code></p>
     * @return end
     */
    @QtUninvokable
    protected final io.qt.core.QAssociativeIterator<Key,T> end()    {
        return QHash.end(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    
    /**
     * Provides a constant C++ iterator to the containers begin.
     * <p>See <code><a href="https://doc.qt.io/qt/qmultihash.html#constBegin">QMultiHash::<wbr>constBegin()const</a></code></p>
     * @return begin
     */
	@QtUninvokable
    protected final io.qt.core.QAssociativeConstIterator<Key,T> constBegin()    {
        return QHash.constBegin(QtJambi_LibraryUtilities.internal.nativeId(this));
    }

    /**
     * Provides a constant C++ iterator to the containers end.
     * <p>See <code><a href="https://doc.qt.io/qt/qmultihash.html#constEnd">QMultiHash::<wbr>constEnd()const</a></code></p>
     * @return end
     */
    @QtUninvokable
    protected final io.qt.core.QAssociativeConstIterator<Key,T> constEnd()    {
        return QHash.constEnd(QtJambi_LibraryUtilities.internal.nativeId(this));
    }

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qmultihash.html#find">QMultiHash::<wbr>find(Key)const</a></code></p>
     * @return iterator
     */
    @QtUninvokable
    public final io.qt.core.QAssociativeConstIterator<Key,T> find(Key key)    {
        return QHash.find(QtJambi_LibraryUtilities.internal.nativeId(this), key);
    }

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qmultihash.html#insert">QMultiHash::<wbr>insert(Key,T)</a></code></p>
     */
    @QtUninvokable
    public final void insert(Key key, T value)    {
        try{
        	QHash.insert(QtJambi_LibraryUtilities.internal.nativeId(this), key, value);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QMap.handleException(e, keyMetaType(), valueMetaType(), key, value);
        }
    }

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qmultihash.html#isEmpty">QMultiHash::<wbr>isEmpty()const</a></code></p>
     */
    @QtUninvokable
    public final boolean isEmpty()    {
        return size()==0;
    }

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qmultihash.html#key">QMultiHash::<wbr>key(T)const</a></code></p>
     */
    @QtUninvokable
    public final Key key(T value) {
        return key(value, null);
    }
    
    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qmultihash.html#key-1">QMultiHash::<wbr>key(T,Key)const</a></code></p>
     */
    @QtUninvokable
    public final Key key(T value, Key defaultKey)    {
        try{
        	return QHash.key(QtJambi_LibraryUtilities.internal.nativeId(this), value, defaultKey);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QMap.handleException(e, keyMetaType(), valueMetaType(), defaultKey, value);
        }
    }

	/**
	 * Returns a {@link List} of the keys contained in this associative container.
     * <p>See <code><a href="https://doc.qt.io/qt/qmultihash.html#keys">QMultiHash::<wbr>keys()const</a></code></p>
	 * @return list of keys
	 */
    @QtUninvokable
    public final QList<Key> keys()    {
        return QHash.keys(QtJambi_LibraryUtilities.internal.nativeId(this));
    }

	/**
     * <p>See <code><a href="https://doc.qt.io/qt/qmultihash.html#keys-1">QMultiHash::<wbr>keys(T)const</a></code></p>
	 */
    @QtUninvokable
    public final QList<Key> keys(T value)    {
        try{
        	return keysForValue(QtJambi_LibraryUtilities.internal.nativeId(this), value);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QSet.handleException(e, valueMetaType(), value);
        }
    }

	/**
     * <p>See <code><a href="https://doc.qt.io/qt/qmultihash.html#removeIf">QMultiHash::<wbr>removeIf(Predicate)</a></code></p>
	 */
    @QtUninvokable
    public final int removeIf(Predicate<Key> predicate)    {
    	List<Key> keys = new ArrayList<>();
    	final long nativeId = QtJambi_LibraryUtilities.internal.nativeId(this);
    	for(Key key : QHash.<Key>keys(nativeId)) {
    		if(predicate.test(key))
    			keys.add(key);
    	}
    	int count = 0;
    	for (Key key : keys) {
			count += QHash.remove(nativeId, key);
		}
    	return count;
    }

	/**
     * <p>See <code><a href="https://doc.qt.io/qt/qmultihash.html#removeIf">QMultiHash::<wbr>removeIf(Predicate)</a></code></p>
	 */
    @QtUninvokable
    public final int removeIf(BiPredicate<Key,T> predicate)    {
    	List<QPair<Key,T>> pairs = new ArrayList<>();
    	final long nativeId = QtJambi_LibraryUtilities.internal.nativeId(this);
    	for(QPair<Key,T> pair : QHash.<Key,T>constBegin(nativeId)) {
    		if(predicate.test(pair.first, pair.second))
    			pairs.add(pair);
    	}
    	int count = 0;
    	for (QPair<Key,T> pair : pairs) {
			count += remove(nativeId, pair.first, pair.second);
		}
    	return count;
    }
    
	/**
     * <p>See <code><a href="https://doc.qt.io/qt/qmultihash.html#remove">QMultiHash::<wbr>remove(Key)</a></code></p>
	 */
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
    
	/**
     * <p>See <code><a href="https://doc.qt.io/qt/qmultihash.html#reserve">QMultiHash::<wbr>reserve(int)</a></code></p>
	 */
    @QtUninvokable
    public final void reserve(int size)    {
    	QHash.reserve(QtJambi_LibraryUtilities.internal.nativeId(this), size);
    }

	/**
     * <p>See <code><a href="https://doc.qt.io/qt/qmultihash.html#size">QMultiHash::<wbr>size()const</a></code></p>
	 */
    @QtUninvokable
    public final int size()    {
        return QHash.size(QtJambi_LibraryUtilities.internal.nativeId(this));
    }

	/**
     * <p>See <code><a href="https://doc.qt.io/qt/qmultihash.html#squeeze">QMultiHash::<wbr>squeeze()</a></code></p>
	 */
    @QtUninvokable
    public final void squeeze() { 
    	reserve(0); 
	}
    
	/**
     * <p>See <code><a href="https://doc.qt.io/qt/qmultihash.html#take">QMultiHash::<wbr>take(Key)</a></code></p>
	 */
    @QtUninvokable
    public final T take(Key key)    {
        try{
        	return QHash.take(QtJambi_LibraryUtilities.internal.nativeId(this), key);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QSet.handleException(e, keyMetaType(), key);
        }
    }

	/**
     * <p>See <code><a href="https://doc.qt.io/qt/qmultihash.html#uniqueKeys">QMultiHash::<wbr>uniqueKeys()const</a></code></p>
	 */
    @QtUninvokable
    public final QList<Key> uniqueKeys()    {
        return uniqueKeys(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private static native <Key> QList<Key> uniqueKeys(long __this__nativeId);

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qmultihash.html#unite-1">QMultiHash::<wbr>unite(const QHash&lt;Key, T> &amp;)</a></code></p>
	 */
    @QtUninvokable
    public final void unite(QHash<? super Key,? super T> other)    {
        unite(QtJambi_LibraryUtilities.internal.nativeId(this), other);
    }
    
    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qmultihash.html#unite">QMultiHash::<wbr>unite(const QMultiHash&lt;Key, T> &amp;)</a></code></p>
	 */
    @QtUninvokable
    public final void unite(java.util.Map<? super Key,? extends java.util.Collection<? super T>> other)    {
        unite(QtJambi_LibraryUtilities.internal.nativeId(this), other);
    }
    @QtUninvokable
    private static native void unite(long __this__nativeId, Object other);

	/**
     * <p>See <code><a href="https://doc.qt.io/qt/qmultihash.html#value">QMultiHash::<wbr>value(Key)</a></code></p>
	 */
    @QtUninvokable
    public final T value(Key key) {
        return value(key, null);
    }
    
	/**
     * <p>See <code><a href="https://doc.qt.io/qt/qmultihash.html#value-1">QMultiHash::<wbr>value(Key,T)</a></code></p>
	 */
    @QtUninvokable
    public final T value(Key key, T defaultValue)    {
        try{
        	return QHash.value(QtJambi_LibraryUtilities.internal.nativeId(this), key, defaultValue);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QMap.handleException(e, keyMetaType(), valueMetaType(), key, defaultValue);
        }
    }

    /**
     * Returns a {@link List} of the values contained in this multihash.
     * @see Map#values()
     */
    @QtUninvokable
    public final java.util.List<java.util.List<T>> values()    {
    	long id = QtJambi_LibraryUtilities.internal.nativeId(this);
    	java.util.List<java.util.List<T>> result = new java.util.ArrayList<>();
        for(Object key : uniqueKeys(id)) {
            result.add(valuesKey(id, key));
        }
        return result;
    }

	/**
     * <p>See <code><a href="https://doc.qt.io/qt/qmultihash.html#values">QMultiHash::<wbr>values()const</a></code></p>
	 */
    @QtUninvokable
    public final QList<T> listOfValues()    {
        return QHash.values(QtJambi_LibraryUtilities.internal.nativeId(this));
    }

	/**
     * <p>See <code><a href="https://doc.qt.io/qt/qmultihash.html#values-1">QMultiHash::<wbr>values(Key)const</a></code></p>
	 */
    @QtUninvokable
    public final QList<T> values(Key key)    {
        try{
        	return valuesKey(QtJambi_LibraryUtilities.internal.nativeId(this), key);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QSet.handleException(e, keyMetaType(), key);
        }
    }
    @QtUninvokable
    private static native <Key,T> QList<T> valuesKey(long __this__nativeId, Key key);
    
	/**
     * <p>See <code><a href="https://doc.qt.io/qt/qmultihash.html#count">QMultiHash::<wbr>count(Key,T)const</a></code></p>
	 */
    @QtUninvokable
    public final int count(Key key, T value)    {
        try{
        	return count(QtJambi_LibraryUtilities.internal.nativeId(this), key, value);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QMap.handleException(e, keyMetaType(), valueMetaType(), key, value);
        }
    }
    private static native <Key,T> int count(long __this__nativeId, Key key, T value);

	/**
     * <p>See <code><a href="https://doc.qt.io/qt/qmultihash.html#find">QMultiHash::<wbr>find(Key,T)const</a></code></p>
	 */
    @QtUninvokable
    public final io.qt.core.QAssociativeConstIterator<Key,T> find(Key key, T value)    {
        try{
        	return find(QtJambi_LibraryUtilities.internal.nativeId(this), key, value);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QMap.handleException(e, keyMetaType(), valueMetaType(), key, value);
        }
    }
    private static native <Key,T> io.qt.core.QAssociativeConstIterator<Key,T> find(long __this__nativeId, Key key, T value);
    
	/**
     * <p>See <code><a href="https://doc.qt.io/qt/qmultihash.html#remove-1">QMultiHash::<wbr>remove(Key,T)</a></code></p>
	 */
    @QtUninvokable
    public final int removeAll(Key key, T value)    {
    	try{
    		return remove(QtJambi_LibraryUtilities.internal.nativeId(this), key, value);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QMap.handleException(e, keyMetaType(), valueMetaType(), key, value);
    	}
    }
    private static native <Key,T> int remove(long __this__nativeId, Key key, T value);
    
	/**
     * <p>See <code><a href="https://doc.qt.io/qt/qmultihash.html#replace">QMultiHash::<wbr>replace(Key,T)</a></code></p>
	 */
    @QtUninvokable
    public final void replaceOne(Key key, T value)    {
        try{
        	replaceOne(QtJambi_LibraryUtilities.internal.nativeId(this), key, value);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QMap.handleException(e, keyMetaType(), valueMetaType(), key, value);
        }
    }
    private static native <Key,T> void replaceOne(long __this__nativeId, Key key, T value);
    
	/**
     * <p>See <code><a href="https://doc.qt.io/qt/qmultihash.html#contains">QMultiHash::<wbr>contains(Key,T)const</a></code></p>
	 */
    @QtUninvokable
    public final boolean contains(Key key, T value)    {
        try{
        	return contains(QtJambi_LibraryUtilities.internal.nativeId(this), key, value);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QMap.handleException(e, keyMetaType(), valueMetaType(), key, value);
        }
    }
    private static native <Key,T> boolean contains(long __this__nativeId, Key key, T value);

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qmultihash.html#operator-eq-eq">operator==<wbr>(QMultiHash&lt;Key,T>,QMultiHash&lt;Key,T>)</a></code></p>
     */
    @SuppressWarnings("unchecked")
	@Override
    @QtUninvokable
    public boolean equals(Object other) {
        if (other instanceof java.util.Map && QMap.checkContainerType(this.keyMetaType(), this.valueMetaType(), (java.util.Map<?,?>) other)) {
            return QHash.operator_equal(QtJambi_LibraryUtilities.internal.nativeId(this), (java.util.Map<Key,T>) other);
        }
        return false;
    }

    /**
     * Returns the objects's hash code computed by <code>qHash(QMultiHash&lt;Key,T>)</code>.
     */
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
    
    /**
     * Returns the string representation of the object given by <code>QVariant(this).toString()</code>.
     */
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
    
    /**
     * Returns {@code true} if this map contains a mapping for the specified key.
     * @see Map#containsKey(Object)
     * @see #contains(Object)
     */
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

    /**
     * Returns the value to which the specified key is mapped,
     * or {@code null} if this map contains no mapping for the key.
     * @see Map#get(Object)
     * @see #value(Object)
     */
    @Override
    @QtUninvokable
    public final List<T> get(Object key){
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

    /**
     * Associates the specified value with the specified key in this map.
     * @see Map#put(Object, Object)
     * @see #insert(Object, Object)
     */
    @Override
    @QtUninvokable
    public final java.util.List<T> put(Key key, java.util.List<T> values){
    	java.util.List<T> old = values(key);
    	for(T value : values)
    		insert(key, value);
        return old;
    }

    /**
     * Removes the mapping for a key from this map if it is present.
     * @see Map#remove(Object)
     * @see #take(Object)
     */
    @Override
    @QtUninvokable
    public final java.util.List<T> remove(Object key){
        try{
        	@SuppressWarnings("unchecked")
			java.util.List<T> result = valuesKey(QtJambi_LibraryUtilities.internal.nativeId(this), (Key)key);
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

    /**
     * Returns {@code true} if this map maps one or more keys to the
     * specified value.
     * @see Map#containsValue(Object)
     * @see #key(Object, Object)
     */
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
    
    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qmultihash.html#operator-lt-lt">operator&lt;&lt;(QDataStream&amp;,<wbr>QMultiHash&lt;Key,T>)</a></code></p>
     */
    @io.qt.QtUninvokable
    public void writeTo(io.qt.core.QDataStream stream){
        writeTo(QtJambi_LibraryUtilities.internal.nativeId(this), QtJambi_LibraryUtilities.internal.nativeId(stream));
    }
    
    @io.qt.QtUninvokable
    private native void writeTo(long __this__nativeId, long stream);
    
    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qmultihash.html#operator-gt-gt">operator&gt;&gt;(QDataStream&amp;,<wbr>QMultiHash&lt;Key,T>&amp;)</a></code></p>
     */
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
     * @param <Key> the {@code QMultiHash}'s key type
     * @param <T> the {@code QMultiHash}'s value type
     * @param k1 the mapping's key
     * @param t1 the mapping's value
     * @return a {@code QMultiHash} containing the specified mapping
     * @throws NullPointerException if the key or the value is {@code null}
     */
    public static <Key,T> QMultiHash<Key,T> of(Key k1, T t1) {
    	QMultiHash<Key,T> result = new QMultiHash<>(
    			QList.findElementMetaType(k1), 
    			QList.findElementMetaType(t1));
    	result.insert(k1, t1);
        return result;
    }

    /**
     * Returns a QMultiHash containing two mappings.
     *
     * @param <Key> the {@code QMultiHash}'s key type
     * @param <T> the {@code QMultiHash}'s value type
     * @param k1 the first mapping's key
     * @param t1 the first mapping's value
     * @param k2 the second mapping's key
     * @param t2 the second mapping's value
     * @return a {@code QMultiHash} containing the specified mappings
     * @throws NullPointerException if any key or value is {@code null}
     */
    public static <Key,T> QMultiHash<Key,T> of(Key k1, T t1, Key k2, T t2) {
    	QMultiHash<Key,T> result = new QMultiHash<>(
    			QList.findElementMetaType(k1, k2), 
    			QList.findElementMetaType(t1, t2));
    	result.insert(k1, t1);
    	result.insert(k2, t2);
        return result;
    }

    /**
     * Returns a QMultiHash containing three mappings.
     *
     * @param <Key> the {@code QMultiHash}'s key type
     * @param <T> the {@code QMultiHash}'s value type
     * @param k1 the first mapping's key
     * @param t1 the first mapping's value
     * @param k2 the second mapping's key
     * @param t2 the second mapping's value
     * @param k3 the third mapping's key
     * @param t3 the third mapping's value
     * @return a {@code QMultiHash} containing the specified mappings
     * @throws NullPointerException if any key or value is {@code null}
     */
    public static <Key,T> QMultiHash<Key,T> of(Key k1, T t1, Key k2, T t2, Key k3, T t3) {
    	QMultiHash<Key,T> result = new QMultiHash<>(
    			QList.findElementMetaType(k1, k2, k3), 
    			QList.findElementMetaType(t1, t2, t3));
    	result.insert(k1, t1);
    	result.insert(k2, t2);
    	result.insert(k3, t3);
        return result;
    }

    /**
     * Returns a QMultiHash containing four mappings.
     *
     * @param <Key> the {@code QMultiHash}'s key type
     * @param <T> the {@code QMultiHash}'s value type
     * @param k1 the first mapping's key
     * @param t1 the first mapping's value
     * @param k2 the second mapping's key
     * @param t2 the second mapping's value
     * @param k3 the third mapping's key
     * @param t3 the third mapping's value
     * @param k4 the fourth mapping's key
     * @param t4 the fourth mapping's value
     * @return a {@code QMultiHash} containing the specified mappings
     * @throws NullPointerException if any key or value is {@code null}
     */
    public static <Key,T> QMultiHash<Key,T> of(Key k1, T t1, Key k2, T t2, Key k3, T t3, Key k4, T t4) {
    	QMultiHash<Key,T> result = new QMultiHash<>(
    			QList.findElementMetaType(k1, k2, k3, k4), 
    			QList.findElementMetaType(t1, t2, t3, t4));
    	result.insert(k1, t1);
    	result.insert(k2, t2);
    	result.insert(k3, t3);
    	result.insert(k4, t4);
        return result;
    }

    /**
     * Returns a QMultiHash containing five mappings.
     *
     * @param <Key> the {@code QMultiHash}'s key type
     * @param <T> the {@code QMultiHash}'s value type
     * @param k1 the first mapping's key
     * @param t1 the first mapping's value
     * @param k2 the second mapping's key
     * @param t2 the second mapping's value
     * @param k3 the third mapping's key
     * @param t3 the third mapping's value
     * @param k4 the fourth mapping's key
     * @param t4 the fourth mapping's value
     * @param k5 the fifth mapping's key
     * @param t5 the fifth mapping's value
     * @return a {@code QMultiHash} containing the specified mappings
     * @throws NullPointerException if any key or value is {@code null}
     */
    public static <Key,T> QMultiHash<Key,T> of(Key k1, T t1, Key k2, T t2, Key k3, T t3, Key k4, T t4, Key k5, T t5) {
    	QMultiHash<Key,T> result = new QMultiHash<>(
    			QList.findElementMetaType(k1, k2, k3, k4, k5), 
    			QList.findElementMetaType(t1, t2, t3, t4, t5));
    	result.insert(k1, t1);
    	result.insert(k2, t2);
    	result.insert(k3, t3);
    	result.insert(k4, t4);
    	result.insert(k5, t5);
        return result;
    }

    /**
     * Returns a QMultiHash containing six mappings.
     *
     * @param <Key> the {@code QMultiHash}'s key type
     * @param <T> the {@code QMultiHash}'s value type
     * @param k1 the first mapping's key
     * @param t1 the first mapping's value
     * @param k2 the second mapping's key
     * @param t2 the second mapping's value
     * @param k3 the third mapping's key
     * @param t3 the third mapping's value
     * @param k4 the fourth mapping's key
     * @param t4 the fourth mapping's value
     * @param k5 the fifth mapping's key
     * @param t5 the fifth mapping's value
     * @param k6 the sixth mapping's key
     * @param t6 the sixth mapping's value
     * @return a {@code QMultiHash} containing the specified mappings
     * @throws NullPointerException if any key or value is {@code null}
     */
    public static <Key,T> QMultiHash<Key,T> of(Key k1, T t1, Key k2, T t2, Key k3, T t3, Key k4, T t4, Key k5, T t5,
                               Key k6, T t6) {
    	QMultiHash<Key,T> result = new QMultiHash<>(
    			QList.findElementMetaType(k1, k2, k3, k4, k5, k6), 
    			QList.findElementMetaType(t1, t2, t3, t4, t5, t6));
    	result.insert(k1, t1);
    	result.insert(k2, t2);
    	result.insert(k3, t3);
    	result.insert(k4, t4);
    	result.insert(k5, t5);
    	result.insert(k6, t6);
        return result;
    }

    /**
     * Returns a QMultiHash containing seven mappings.
     *
     * @param <Key> the {@code QMultiHash}'s key type
     * @param <T> the {@code QMultiHash}'s value type
     * @param k1 the first mapping's key
     * @param t1 the first mapping's value
     * @param k2 the second mapping's key
     * @param t2 the second mapping's value
     * @param k3 the third mapping's key
     * @param t3 the third mapping's value
     * @param k4 the fourth mapping's key
     * @param t4 the fourth mapping's value
     * @param k5 the fifth mapping's key
     * @param t5 the fifth mapping's value
     * @param k6 the sixth mapping's key
     * @param t6 the sixth mapping's value
     * @param k7 the seventh mapping's key
     * @param t7 the seventh mapping's value
     * @return a {@code QMultiHash} containing the specified mappings
     * @throws NullPointerException if any key or value is {@code null}
     */
    public static <Key,T> QMultiHash<Key,T> of(Key k1, T t1, Key k2, T t2, Key k3, T t3, Key k4, T t4, Key k5, T t5,
                               Key k6, T t6, Key k7, T t7) {
    	QMultiHash<Key,T> result = new QMultiHash<>(
    			QList.findElementMetaType(k1, k2, k3, k4, k5, k6, k7), 
    			QList.findElementMetaType(t1, t2, t3, t4, t5, t6, t7));
    	result.insert(k1, t1);
    	result.insert(k2, t2);
    	result.insert(k3, t3);
    	result.insert(k4, t4);
    	result.insert(k5, t5);
    	result.insert(k6, t6);
    	result.insert(k7, t7);
        return result;
    }

    /**
     * Returns a QMultiHash containing eight mappings.
     *
     * @param <Key> the {@code QMultiHash}'s key type
     * @param <T> the {@code QMultiHash}'s value type
     * @param k1 the first mapping's key
     * @param t1 the first mapping's value
     * @param k2 the second mapping's key
     * @param t2 the second mapping's value
     * @param k3 the third mapping's key
     * @param t3 the third mapping's value
     * @param k4 the fourth mapping's key
     * @param t4 the fourth mapping's value
     * @param k5 the fifth mapping's key
     * @param t5 the fifth mapping's value
     * @param k6 the sixth mapping's key
     * @param t6 the sixth mapping's value
     * @param k7 the seventh mapping's key
     * @param t7 the seventh mapping's value
     * @param k8 the eighth mapping's key
     * @param t8 the eighth mapping's value
     * @return a {@code QMultiHash} containing the specified mappings
     * @throws NullPointerException if any key or value is {@code null}
     */
    public static <Key,T> QMultiHash<Key,T> of(Key k1, T t1, Key k2, T t2, Key k3, T t3, Key k4, T t4, Key k5, T t5,
                               Key k6, T t6, Key k7, T t7, Key k8, T t8) {
    	QMultiHash<Key,T> result = new QMultiHash<>(
    			QList.findElementMetaType(k1, k2, k3, k4, k5, k6, k7, k8), 
    			QList.findElementMetaType(t1, t2, t3, t4, t5, t6, t7, t8));
    	result.insert(k1, t1);
    	result.insert(k2, t2);
    	result.insert(k3, t3);
    	result.insert(k4, t4);
    	result.insert(k5, t5);
    	result.insert(k6, t6);
    	result.insert(k7, t7);
    	result.insert(k8, t8);
        return result;
    }

    /**
     * Returns a QMultiHash containing nine mappings.
     *
     * @param <Key> the {@code QMultiHash}'s key type
     * @param <T> the {@code QMultiHash}'s value type
     * @param k1 the first mapping's key
     * @param t1 the first mapping's value
     * @param k2 the second mapping's key
     * @param t2 the second mapping's value
     * @param k3 the third mapping's key
     * @param t3 the third mapping's value
     * @param k4 the fourth mapping's key
     * @param t4 the fourth mapping's value
     * @param k5 the fifth mapping's key
     * @param t5 the fifth mapping's value
     * @param k6 the sixth mapping's key
     * @param t6 the sixth mapping's value
     * @param k7 the seventh mapping's key
     * @param t7 the seventh mapping's value
     * @param k8 the eighth mapping's key
     * @param t8 the eighth mapping's value
     * @param k9 the ninth mapping's key
     * @param t9 the ninth mapping's value
     * @return a {@code QMultiHash} containing the specified mappings
     * @throws NullPointerException if any key or value is {@code null}
     */
    public static <Key,T> QMultiHash<Key,T> of(Key k1, T t1, Key k2, T t2, Key k3, T t3, Key k4, T t4, Key k5, T t5,
                               Key k6, T t6, Key k7, T t7, Key k8, T t8, Key k9, T t9) {
    	QMultiHash<Key,T> result = new QMultiHash<>(
    			QList.findElementMetaType(k1, k2, k3, k4, k5, k6, k7, k8, k9), 
    			QList.findElementMetaType(t1, t2, t3, t4, t5, t6, t7, t8, t9));
    	result.insert(k1, t1);
    	result.insert(k2, t2);
    	result.insert(k3, t3);
    	result.insert(k4, t4);
    	result.insert(k5, t5);
    	result.insert(k6, t6);
    	result.insert(k7, t7);
    	result.insert(k8, t8);
    	result.insert(k9, t9);
        return result;
    }

    /**
     * Returns a QMultiHash containing ten mappings.
     *
     * @param <Key> the {@code QMultiHash}'s key type
     * @param <T> the {@code QMultiHash}'s value type
     * @param k1 the first mapping's key
     * @param t1 the first mapping's value
     * @param k2 the second mapping's key
     * @param t2 the second mapping's value
     * @param k3 the third mapping's key
     * @param t3 the third mapping's value
     * @param k4 the fourth mapping's key
     * @param t4 the fourth mapping's value
     * @param k5 the fifth mapping's key
     * @param t5 the fifth mapping's value
     * @param k6 the sixth mapping's key
     * @param t6 the sixth mapping's value
     * @param k7 the seventh mapping's key
     * @param t7 the seventh mapping's value
     * @param k8 the eighth mapping's key
     * @param t8 the eighth mapping's value
     * @param k9 the ninth mapping's key
     * @param t9 the ninth mapping's value
     * @param k10 the tenth mapping's key
     * @param t10 the tenth mapping's value
     * @return a {@code QMultiHash} containing the specified mappings
     * @throws NullPointerException if any key or value is {@code null}
     */
    public static <Key,T> QMultiHash<Key,T> of(Key k1, T t1, Key k2, T t2, Key k3, T t3, Key k4, T t4, Key k5, T t5,
                               Key k6, T t6, Key k7, T t7, Key k8, T t8, Key k9, T t9, Key k10, T t10) {
    	QMultiHash<Key,T> result = new QMultiHash<>(
    			QList.findElementMetaType(k1, k2, k3, k4, k5, k6, k7, k8, k9, k10), 
    			QList.findElementMetaType(t1, t2, t3, t4, t5, t6, t7, t8, t9, t10));
    	result.insert(k1, t1);
    	result.insert(k2, t2);
    	result.insert(k3, t3);
    	result.insert(k4, t4);
    	result.insert(k5, t5);
    	result.insert(k6, t6);
    	result.insert(k7, t7);
    	result.insert(k8, t8);
    	result.insert(k9, t9);
    	result.insert(k10, t10);
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
     * @param <Key> the {@code QMultiHash}'s key type
     * @param <T> the {@code QMultiHash}'s value type
     * @param entries {@code java.util.Map.Entry}s containing the keys and values from which the map is populated
     * @return a {@code QMultiHash} containing the specified mappings
     * @throws NullPointerException if any entry, key, or value is {@code null}, or if
     *         the {@code entries} array is {@code null}
     */
    @SafeVarargs
    public static <Key,T> QMultiHash<Key,T> ofEntries(Map.Entry<? extends Key, ? extends T> entry0, Map.Entry<? extends Key, ? extends T>... entries) {
    	QPair<QMetaType, QMetaType> metaTypes = findMapMetaType(entry0, entries);
    	QMultiHash<Key,T> result = new QMultiHash<>(metaTypes.first, metaTypes.second);
    	result.insert(entry0.getKey(), entry0.getValue());
    	for (Map.Entry<? extends Key, ? extends T> entry : entries) {
        	result.insert(entry.getKey(), entry.getValue());
		}
        return result;
    }

    /**
     * Returns a QMultiHash containing keys and values extracted from the given pairs.
     *
     * @apiNote
     * It is convenient to create the pair using the {@link QPair#pair(Object, Object)} method.
     * For example,
     *
     * <pre>{@code
     *     import static java.util.QPair.pair;
     *
     *     QMultiHash<Integer,String> map = QMultiHash.ofPairs(
     *         pair(1, "a"),
     *         pair(2, "b"),
     *         pair(3, "c"),
     *         ...
     *         pair(26, "z"));
     * }</pre>
     *
     * @param <Key> the {@code QMultiHash}'s key type
     * @param <T> the {@code QMultiHash}'s value type
     * @param pairs {@code QPair}s containing the keys and values from which the map is populated
     * @return a {@code QMultiHash} containing the specified mappings
     * @throws NullPointerException if any entry, key, or value is {@code null}, or if
     *         the {@code entries} array is {@code null}
     */
    @SafeVarargs
    public static <Key,T> QMultiHash<Key,T> ofPairs(QPair<? extends Key, ? extends T> pair0, QPair<? extends Key, ? extends T>... pairs) {
    	QPair<QMetaType, QMetaType> metaTypes = findMapMetaType(pair0, pairs);
    	QMultiHash<Key,T> result = new QMultiHash<>(metaTypes.first, metaTypes.second);
    	result.insert(pair0.first, pair0.second);
    	for (QPair<? extends Key, ? extends T> entry : pairs) {
        	result.insert(entry.first, entry.second);
		}
        return result;
    }
}
