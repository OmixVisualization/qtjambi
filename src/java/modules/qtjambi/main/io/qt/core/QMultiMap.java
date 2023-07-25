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

import java.util.AbstractMap;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;
import java.util.Map;
import java.util.NavigableMap;
import java.util.NavigableSet;
import java.util.Objects;
import java.util.TreeMap;
import java.util.TreeSet;
import java.util.function.BiPredicate;
import java.util.function.Function;
import java.util.function.Predicate;

import io.qt.NativeAccess;
import io.qt.QNoImplementationException;
import io.qt.QNoNativeResourcesException;
import io.qt.QtUninvokable;

/**
 * <p>Java wrapper for Qt class <code><a href="https://doc.qt.io/qt/qmultimap.html">QMultiMap</a></code></p>
 */
public class QMultiMap<Key,T> extends AbstractMultiAssociativeContainer<Key,T> implements NavigableMap<Key,List<T>>, Cloneable
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
    protected QMultiMap(QPrivateConstructor p) {
		super(p);
	}
	
    /**
     * Creating a container with given key and value type.
     * <p>See <code><a href="https://doc.qt.io/qt/qmultimap.html#QMultiMap">QMultiMap::<wbr>QMultiMap()</a></code></p>
     * @param keyType the type K
     * @param valueMetaType the type V
     */
	public QMultiMap(Class<Key> keyType, QMetaType.Type valueMetaType) {
		this(keyType, new QMetaType(valueMetaType));
	}
	
    /**
     * Creating a container with given key and value type.
     * <p>See <code><a href="https://doc.qt.io/qt/qmultimap.html#QMultiMap">QMultiMap::<wbr>QMultiMap()</a></code></p>
     * @param keyMetaType the type K
     * @param valueType the type V
     */
	public QMultiMap(QMetaType.Type keyMetaType, Class<T> valueType) {
		this(new QMetaType(keyMetaType), valueType);
	}
	
    /**
     * Creating a container with given key and value type.
     * <p>See <code><a href="https://doc.qt.io/qt/qmultimap.html#QMultiMap">QMultiMap::<wbr>QMultiMap()</a></code></p>
     * @param keyMetaType the type K
     * @param valueMetaType the type V
     */
	public QMultiMap(QMetaType.Type keyMetaType, QMetaType valueMetaType) {
		this(new QMetaType(keyMetaType), valueMetaType);
	}
	
    /**
     * Creating a container with given key and value type.
     * <p>See <code><a href="https://doc.qt.io/qt/qmultimap.html#QMultiMap">QMultiMap::<wbr>QMultiMap()</a></code></p>
     * @param keyMetaType the type K
     * @param valueMetaType the type V
     */
	public QMultiMap(QMetaType keyMetaType, QMetaType.Type valueMetaType) {
		this(keyMetaType, new QMetaType(valueMetaType));
	}
	
    /**
     * Creating a container with given key and value type.
     * <p>See <code><a href="https://doc.qt.io/qt/qmultimap.html#QMultiMap">QMultiMap::<wbr>QMultiMap()</a></code></p>
     * @param keyMetaType the type K
     * @param valueMetaType the type V
     */
	public QMultiMap(QMetaType.Type keyMetaType, QMetaType.Type valueMetaType) {
		this(new QMetaType(keyMetaType), new QMetaType(valueMetaType));
	}
    
    /**
     * Creating a container with given key and value type.
     * <p>See <code><a href="https://doc.qt.io/qt/qmultimap.html#QMultiMap">QMultiMap::<wbr>QMultiMap()</a></code></p>
     * @param keyType the type K
     * @param valueType the type V
     */
    public QMultiMap(Class<Key> keyType, Class<T> valueType) {
		super(null);
		QMetaType keyMetaType = QMetaType.fromType(keyType);
		QMetaType valueMetaType = QMetaType.fromType(valueType);
		initialize(keyType, QtJambi_LibraryUtilities.internal.nativeId(keyMetaType), valueType, QtJambi_LibraryUtilities.internal.nativeId(valueMetaType), null);
	}
    
    /**
     * Creating a container with given content.
     * <p>See <code><a href="https://doc.qt.io/qt/qmultimap.html#QMultiMap-2">QMultiMap::<wbr>QMultiMap(const QMap&lt;Key, T> &amp;)</a></code></p>
     * @param other map
     */
    public QMultiMap(Map<Key,List<T>> other) {
		super(null);
		QPair<QMetaType, QMetaType> metaTypes = findMapMetaType(Objects.requireNonNull(other));
		initialize(metaTypes.first.javaType(), QtJambi_LibraryUtilities.internal.nativeId(metaTypes.first), metaTypes.second.javaType(), QtJambi_LibraryUtilities.internal.nativeId(metaTypes.second), other);
	}
    
    /**
     * Creating a container with given key and value type.
     * <p>See <code><a href="https://doc.qt.io/qt/qmultimap.html#QMultiMap">QMultiMap::<wbr>QMultiMap()</a></code></p>
     * @param keyType the type K
     * @param valueMetaType the type V
     */
	public QMultiMap(Class<Key> keyType, QMetaType valueMetaType) {
		super(null);
		QMetaType keyMetaType = QMetaType.fromType(keyType);
		initialize(keyType, QtJambi_LibraryUtilities.internal.nativeId(keyMetaType), valueMetaType.javaType(), QtJambi_LibraryUtilities.internal.nativeId(valueMetaType), null);
	}
    
    /**
     * Creating a container with given key and value type.
     * <p>See <code><a href="https://doc.qt.io/qt/qmultimap.html#QMultiMap">QMultiMap::<wbr>QMultiMap()</a></code></p>
     * @param keyMetaType the type K
     * @param valueType the type V
     */
	public QMultiMap(QMetaType keyMetaType, Class<T> valueType) {
		super(null);
		QMetaType valueMetaType = QMetaType.fromType(valueType);
		initialize(keyMetaType.javaType(), QtJambi_LibraryUtilities.internal.nativeId(keyMetaType), valueMetaType.javaType(), QtJambi_LibraryUtilities.internal.nativeId(valueMetaType), null);
	}
    
    /**
     * Creating a container with given key and value type.
     * <p>See <code><a href="https://doc.qt.io/qt/qmultimap.html#QMultiMap">QMultiMap::<wbr>QMultiMap()</a></code></p>
     * @param keyMetaType the type K
     * @param valueMetaType the type V
     */
	public QMultiMap(QMetaType keyMetaType, QMetaType valueMetaType) {
		super(null);
		initialize(keyMetaType.javaType(), QtJambi_LibraryUtilities.internal.nativeId(keyMetaType), valueMetaType.javaType(), QtJambi_LibraryUtilities.internal.nativeId(valueMetaType), null);
	}
    
    @QtUninvokable
    private native void initialize(Class<?> keyType, long keyMetaType, Class<?> valueType, long valueMetaType, Map<Key, List<T>> other);
    
    /**
     * Creates and returns a copy of this object.
     * <p>See <code><a href="https://doc.qt.io/qt/qmultimap.html#QMultiMap-7">QMultiMap::<wbr>QMultiMap(const QMultiMap&lt;Key, T> &amp;)</a></code></p>
     */
    @Override
	public QMultiMap<Key,T> clone(){
		return new QMultiMap<>(this);
	}

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qmultimap.html#clear">QMultiMap::<wbr>clear()</a></code></p>
     */
    @QtUninvokable
    public final void clear()    {
    	QMap.clear(QtJambi_LibraryUtilities.internal.nativeId(this));
    }

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qmultimap.html#contains">QMultiMap::<wbr>contains(Key)const</a></code></p>
     */
    @QtUninvokable
    public final boolean contains(Key key)    {
        try{
        	return QMap.contains(QtJambi_LibraryUtilities.internal.nativeId(this), key);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QSet.handleException(e, keyMetaType(), key);
        }
    }

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qmultimap.html#count-1">QMultiMap::<wbr>count()const</a></code></p>
     */
    @QtUninvokable
    public final int count()    {
        return size();
    }

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qmultimap.html#count">QMultiMap::<wbr>count(Key)const</a></code></p>
     */
    @QtUninvokable
    public final int count(Key key)    {
        try{
        	return QMap.count(QtJambi_LibraryUtilities.internal.nativeId(this), key);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QSet.handleException(e, keyMetaType(), key);
        }
    }
    
    /**
     * Provides a mutable C++ iterator to the containers begin.
     * <p>See <code><a href="https://doc.qt.io/qt/qmultimap.html#begin">QMultiMap::<wbr>begin()</a></code></p>
     * @return begin
     */
	@QtUninvokable
    protected final io.qt.core.QAssociativeIterator<Key,T> begin()    {
        return QMap.begin(QtJambi_LibraryUtilities.internal.nativeId(this));
    }

    /**
     * Provides a mutable C++ iterator to the containers end.
     * <p>See <code><a href="https://doc.qt.io/qt/qmultimap.html#end">QMultiMap::<wbr>end()</a></code></p>
     * @return end
     */
    @QtUninvokable
    protected final io.qt.core.QAssociativeIterator<Key,T> end()    {
        return QMap.end(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    
    /**
     * Provides a constant C++ iterator to the containers begin.
     * <p>See <code><a href="https://doc.qt.io/qt/qmultimap.html#constBegin">QMultiMap::<wbr>constBegin()const</a></code></p>
     * @return begin
     */
	@QtUninvokable
    protected final io.qt.core.QAssociativeConstIterator<Key,T> constBegin()    {
        return QMap.constBegin(QtJambi_LibraryUtilities.internal.nativeId(this));
    }

    /**
     * Provides a constant C++ iterator to the containers end.
     * <p>See <code><a href="https://doc.qt.io/qt/qmultimap.html#constEnd">QMultiMap::<wbr>constEnd()const</a></code></p>
     * @return end
     */
    @QtUninvokable
    protected final io.qt.core.QAssociativeConstIterator<Key,T> constEnd()    {
        return QMap.constEnd(QtJambi_LibraryUtilities.internal.nativeId(this));
    }

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qmultimap.html#find">QMultiMap::<wbr>find(Key)const</a></code></p>
     * @return iterator
     */
    @QtUninvokable
    public final io.qt.core.QAssociativeConstIterator<Key,T> find(Key key)    {
        try{
        	return QMap.find(QtJambi_LibraryUtilities.internal.nativeId(this), key);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QSet.handleException(e, keyMetaType(), key);
        }
    }

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qmultimap.html#first-1">QMultiMap::<wbr>first()const</a></code></p>
     */
    @QtUninvokable
    public final T first()    {
        return QMap.first(QtJambi_LibraryUtilities.internal.nativeId(this));
    }

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qmultimap.html#firstKey">QMultiMap::<wbr>firstKey()const</a></code></p>
     */
    @QtUninvokable
    public final Key firstKey()    {
        return QMap.firstKey(QtJambi_LibraryUtilities.internal.nativeId(this));
    }

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qmultimap.html#insert">QMultiMap::<wbr>insert(Key,T)</a></code></p>
     */
    @QtUninvokable
    public final void insert(Key key, T value)    {
        try{
        	QMap.insert(QtJambi_LibraryUtilities.internal.nativeId(this), key, value);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QMap.handleException(e, keyMetaType(), valueMetaType(), key, value);
        }
    }

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qmultimap.html#isEmpty">QMultiMap::<wbr>isEmpty()const</a></code></p>
     */
    @QtUninvokable
    public final boolean isEmpty()    {
        return size()==0;
    }

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qmultimap.html#key">QMultiMap::<wbr>key(T)const</a></code></p>
     */
    @QtUninvokable
    public final Key key(T value) {
        return key(value, null);
    }
    
    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qmultimap.html#key">QMultiMap::<wbr>key(T,Key)const</a></code></p>
     */
    @QtUninvokable
    public final Key key(T value, Key defaultKey)    {
        try{
        	return QMap.key(QtJambi_LibraryUtilities.internal.nativeId(this), value, defaultKey);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QSet.handleException(e, valueMetaType(), value);
        }
    }

	/**
	 * Returns a {@link List} of the keys contained in this associative container.
     * <p>See <code><a href="https://doc.qt.io/qt/qmultimap.html#keys">QMultiMap::<wbr>keys()const</a></code></p>
	 * @return list of keys
	 */
    @QtUninvokable
    public final QList<Key> keys()    {
        return QMap.keys(QtJambi_LibraryUtilities.internal.nativeId(this));
    }

	/**
     * <p>See <code><a href="https://doc.qt.io/qt/qmultimap.html#keys-1">QMultiMap::<wbr>keys(T)const</a></code></p>
	 */
    @QtUninvokable
    public final QList<Key> keys(T value)    {
        try{
        	return QMap.keysForValue(QtJambi_LibraryUtilities.internal.nativeId(this), value);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QSet.handleException(e, valueMetaType(), value);
        }
    }

	/**
     * <p>See <code><a href="https://doc.qt.io/qt/qmultimap.html#last-1">QMultiMap::<wbr>last()const</a></code></p>
	 */
    @QtUninvokable
    public final T last()    {
        return QMap.last(QtJambi_LibraryUtilities.internal.nativeId(this));
    }

	/**
     * <p>See <code><a href="https://doc.qt.io/qt/qmultimap.html#lastKey">QMultiMap::<wbr>lastKey()const</a></code></p>
	 */
    @QtUninvokable
    public final Key lastKey()    {
        return QMap.lastKey(QtJambi_LibraryUtilities.internal.nativeId(this));
    }

	/**
     * <p>See <code><a href="https://doc.qt.io/qt/qmultimap.html#lowerBound">QMultiMap::<wbr>lowerBound(Key)const</a></code></p>
	 */
    @QtUninvokable
    public final io.qt.core.QAssociativeConstIterator<Key,T> lowerBound(Key key)    {
        try{
        	return QMap.lowerBound(QtJambi_LibraryUtilities.internal.nativeId(this), key);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QSet.handleException(e, keyMetaType(), key);
        }
    }

	/**
     * <p>See <code><a href="https://doc.qt.io/qt/qmultimap.html#removeIf">QMultiMap::<wbr>removeIf(Predicate)</a></code></p>
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
			count += QMap.remove(nativeId, key);
		}
    	return count;
    }

	/**
     * <p>See <code><a href="https://doc.qt.io/qt/qmultimap.html#removeIf">QMultiMap::<wbr>removeIf(Predicate)</a></code></p>
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
     * <p>See <code><a href="https://doc.qt.io/qt/qmultimap.html#remove">QMultiMap::<wbr>remove(Key)</a></code></p>
	 */
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

	/**
     * <p>See <code><a href="https://doc.qt.io/qt/qmultimap.html#size">QMultiMap::size()const</a></code></p>
	 */
    @QtUninvokable
    public final int size()    {
        return QMap.size(QtJambi_LibraryUtilities.internal.nativeId(this));
    }

	/**
     * <p>See <code><a href="https://doc.qt.io/qt/qmultimap.html#take">QMultiMap::<wbr>take(Key)</a></code></p>
	 */
    @QtUninvokable
    public final T take(Key key)    {
    	try {
    		return QMap.take(QtJambi_LibraryUtilities.internal.nativeId(this), key);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QSet.handleException(e, keyMetaType(), key);
    	}
    }

	/**
     * <p>See <code><a href="https://doc.qt.io/qt/qmultimap.html#uniqueKeys">QMultiMap::<wbr>uniqueKeys()const</a></code></p>
	 */
    @QtUninvokable
    public final QList<Key> uniqueKeys()    {
        return uniqueKeys(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private static native <Key> QList<Key> uniqueKeys(long __this__nativeId);

	/**
     * <p>See <code><a href="https://doc.qt.io/qt/qmultimap.html#unite-1">QMultiMap::<wbr>unite(const QMap&lt;Key, T> &amp;)const</a></code></p>
	 */
    @QtUninvokable
    public final void unite(QMap<? super Key,? super T> other)    {
        unite(QtJambi_LibraryUtilities.internal.nativeId(this), other);
    }

	/**
     * <p>See <code><a href="https://doc.qt.io/qt/qmultimap.html#unite">QMultiMap::<wbr>unite(const QMultiMap&lt;Key, T> &amp;)const</a></code></p>
	 */
    @QtUninvokable
    public final void unite(java.util.Map<? super Key,? extends java.util.Collection<? super T>> other)    {
        unite(QtJambi_LibraryUtilities.internal.nativeId(this), other);
    }
    @QtUninvokable
    private static native void unite(long __this__nativeId, Object other);

	/**
     * <p>See <code><a href="https://doc.qt.io/qt/qmultimap.html#upperBound">QMultiMap::<wbr>upperBound(Key)const</a></code></p>
	 */
    @QtUninvokable
    public final io.qt.core.QAssociativeConstIterator<Key,T> upperBound(Key key)    {
        return QMap.upperBound(QtJambi_LibraryUtilities.internal.nativeId(this), key);
    }

	/**
     * <p>See <code><a href="https://doc.qt.io/qt/qmultimap.html#value">QMultiMap::<wbr>value(Key)const</a></code></p>
	 */
    @QtUninvokable
    public final T value(Key key) {
        return value(key, null);
    }
    
	/**
     * <p>See <code><a href="https://doc.qt.io/qt/qmultimap.html#value">QMultiMap::<wbr>value(Key,T)const</a></code></p>
	 */
    @QtUninvokable
    public final T value(Key key, T defaultValue)    {
        try{
        	return QMap.value(QtJambi_LibraryUtilities.internal.nativeId(this), key, defaultValue);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QMap.handleException(e, keyMetaType(), valueMetaType(), key, defaultValue);
        }
    }

    /**
     * Returns a {@link List} of the values contained in this multimap.
     * @see Map#values()
     */
    @Override
    @QtUninvokable
    public final java.util.List<java.util.List<T>> values() {
    	long id = QtJambi_LibraryUtilities.internal.nativeId(this);
        java.util.List<java.util.List<T>> result = new java.util.ArrayList<>();
        for(Object key : uniqueKeys(id)) {
            result.add(valuesKey(id, key));
        }
        return result;
    }

	/**
     * <p>See <code><a href="https://doc.qt.io/qt/qmultimap.html#values">QMultiMap::<wbr>values()const</a></code></p>
	 */
    @QtUninvokable
    public final QList<T> listOfValues()    {
        return QMap.values(QtJambi_LibraryUtilities.internal.nativeId(this));
    }

	/**
     * <p>See <code><a href="https://doc.qt.io/qt/qmultimap.html#values-1">QMultiMap::<wbr>values(Key)const</a></code></p>
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
     * <p>See <code><a href="https://doc.qt.io/qt/qmultimap.html#contains-1">QMultiMap::<wbr>contains(Key,T)const</a></code></p>
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
    @QtUninvokable
    private static native <Key,T> boolean contains(long __this__nativeId, Key key, T value);
    
	/**
     * <p>See <code><a href="https://doc.qt.io/qt/qmultimap.html#count-1">QMultiMap::<wbr>count(Key,T)const</a></code></p>
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
    @QtUninvokable
    private static native <Key,T> int count(long __this__nativeId, Key key, T value);

	/**
     * <p>See <code><a href="https://doc.qt.io/qt/qmultimap.html#find-2">QMultiMap::<wbr>find(Key,T)const</a></code></p>
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
    @QtUninvokable
    private static native <Key,T> io.qt.core.QAssociativeConstIterator<Key,T> find(long __this__nativeId, Key key, T value);
    
	/**
     * <p>See <code><a href="https://doc.qt.io/qt/qmultimap.html#remove-1">QMultiMap::<wbr>remove(Key,T)</a></code></p>
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
    @QtUninvokable
    private static native <Key,T> int remove(long __this__nativeId, Key key, T value);
    
	/**
     * <p>See <code><a href="https://doc.qt.io/qt/qmultimap.html#replace">QMultiMap::<wbr>replace(Key,T)</a></code></p>
	 */
    @QtUninvokable
    public final void replaceOne(Key key, T value)    {
        try{
        	replace(QtJambi_LibraryUtilities.internal.nativeId(this), key, value);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QMap.handleException(e, keyMetaType(), valueMetaType(), key, value);
        }
    }
    @QtUninvokable
    private static native <Key,T> void replace(long __this__nativeId, Key key, T value);

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qmultimap.html#operator-eq-eq">operator==<wbr>(QMultiMap&lt;Key,T>,QMultiMap&lt;Key,T>)</a></code></p>
     */
    @SuppressWarnings("unchecked")
	@Override
    @QtUninvokable
    public boolean equals(Object other) {
        if (other instanceof java.util.Map && QMap.checkContainerType(this.keyMetaType(), this.valueMetaType(), (java.util.Map<?,?>) other)) {
            return QMap.operator_equal(QtJambi_LibraryUtilities.internal.nativeId(this), (java.util.Map<Key,T>) other);
        }
        return false;
    }

    /**
     * Returns the maps comparator.
     */
    @Override
    @QtUninvokable
    public final java.util.Comparator<Key> comparator(){
    	return QMap.createComparator(keyMetaType().javaType(), this);
    }

    /**
     * Returns {@code true} if this map contains a mapping for the specified key.
     * @see Map#containsKey(Object)
     * @see #contains(Object)
     */
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
    	QList<T> old = values(key);
    	for(T value : values)
    		insert(key, value);
        return old;
    }

    /**
     * Removes the mapping for a key from this map if it is present.
     * @see Map#remove(Object)
     * @see #take(Object)
     */
    @SuppressWarnings("unchecked")
	@Override
    @QtUninvokable
    public final java.util.List<T> remove(Object key){
        try{
        	QList<T> result = valuesKey(QtJambi_LibraryUtilities.internal.nativeId(this), (Key)key);
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
    
    /**
     * Returns the objects's hash code computed by <code>qHash(QMultiMap&lt;Key,T>)</code>.
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
     * <p>See <code><a href="https://doc.qt.io/qt/qmultimap.html#operator-lt-lt">operator&lt;&lt;(QDataStream&amp;,<wbr>QMultiMap&lt;Key,T>)</a></code></p>
     */
    @io.qt.QtUninvokable
    public void writeTo(io.qt.core.QDataStream stream){
        writeTo(QtJambi_LibraryUtilities.internal.nativeId(this), QtJambi_LibraryUtilities.internal.nativeId(stream));
    }
    
    @io.qt.QtUninvokable
    private native void writeTo(long __this__nativeId, long stream);
    
    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qmultimap.html#operator-gt-gt">operator&gt;&gt;(QDataStream&amp;,<wbr>QMultiMap&lt;Key,T>&amp;)</a></code></p>
     */
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
     * @param <Key> the {@code QMultiMap}'s key type
     * @param <T> the {@code QMultiMap}'s value type
     * @param k1 the mapping's key
     * @param t1 the mapping's value
     * @return a {@code QMultiMap} containing the specified mapping
     * @throws NullPointerException if the key or the value is {@code null}
     */
    public static <Key,T> QMultiMap<Key,T> of(Key k1, T t1) {
    	QMultiMap<Key,T> result = new QMultiMap<>(
    			QList.findElementMetaType(k1), 
    			QList.findElementMetaType(t1));
    	result.insert(k1, t1);
        return result;
    }

    /**
     * Returns a QMultiMap containing two mappings.
     *
     * @param <Key> the {@code QMultiMap}'s key type
     * @param <T> the {@code QMultiMap}'s value type
     * @param k1 the first mapping's key
     * @param t1 the first mapping's value
     * @param k2 the second mapping's key
     * @param t2 the second mapping's value
     * @return a {@code QMultiMap} containing the specified mappings
     * @throws NullPointerException if any key or value is {@code null}
     */
    public static <Key,T> QMultiMap<Key,T> of(Key k1, T t1, Key k2, T t2) {
    	QMultiMap<Key,T> result = new QMultiMap<>(
    			QList.findElementMetaType(k1, k2), 
    			QList.findElementMetaType(t1, t2));
    	result.insert(k1, t1);
    	result.insert(k2, t2);
        return result;
    }

    /**
     * Returns a QMultiMap containing three mappings.
     *
     * @param <Key> the {@code QMultiMap}'s key type
     * @param <T> the {@code QMultiMap}'s value type
     * @param k1 the first mapping's key
     * @param t1 the first mapping's value
     * @param k2 the second mapping's key
     * @param t2 the second mapping's value
     * @param k3 the third mapping's key
     * @param t3 the third mapping's value
     * @return a {@code QMultiMap} containing the specified mappings
     * @throws NullPointerException if any key or value is {@code null}
     */
    public static <Key,T> QMultiMap<Key,T> of(Key k1, T t1, Key k2, T t2, Key k3, T t3) {
    	QMultiMap<Key,T> result = new QMultiMap<>(
    			QList.findElementMetaType(k1, k2, k3), 
    			QList.findElementMetaType(t1, t2, t3));
    	result.insert(k1, t1);
    	result.insert(k2, t2);
    	result.insert(k3, t3);
        return result;
    }

    /**
     * Returns a QMultiMap containing four mappings.
     *
     * @param <Key> the {@code QMultiMap}'s key type
     * @param <T> the {@code QMultiMap}'s value type
     * @param k1 the first mapping's key
     * @param t1 the first mapping's value
     * @param k2 the second mapping's key
     * @param t2 the second mapping's value
     * @param k3 the third mapping's key
     * @param t3 the third mapping's value
     * @param k4 the fourth mapping's key
     * @param t4 the fourth mapping's value
     * @return a {@code QMultiMap} containing the specified mappings
     * @throws NullPointerException if any key or value is {@code null}
     */
    public static <Key,T> QMultiMap<Key,T> of(Key k1, T t1, Key k2, T t2, Key k3, T t3, Key k4, T t4) {
    	QMultiMap<Key,T> result = new QMultiMap<>(
    			QList.findElementMetaType(k1, k2, k3, k4), 
    			QList.findElementMetaType(t1, t2, t3, t4));
    	result.insert(k1, t1);
    	result.insert(k2, t2);
    	result.insert(k3, t3);
    	result.insert(k4, t4);
        return result;
    }

    /**
     * Returns a QMultiMap containing five mappings.
     *
     * @param <Key> the {@code QMultiMap}'s key type
     * @param <T> the {@code QMultiMap}'s value type
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
     * @return a {@code QMultiMap} containing the specified mappings
     * @throws NullPointerException if any key or value is {@code null}
     */
    public static <Key,T> QMultiMap<Key,T> of(Key k1, T t1, Key k2, T t2, Key k3, T t3, Key k4, T t4, Key k5, T t5) {
    	QMultiMap<Key,T> result = new QMultiMap<>(
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
     * Returns a QMultiMap containing six mappings.
     *
     * @param <Key> the {@code QMultiMap}'s key type
     * @param <T> the {@code QMultiMap}'s value type
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
     * @return a {@code QMultiMap} containing the specified mappings
     * @throws NullPointerException if any key or value is {@code null}
     */
    public static <Key,T> QMultiMap<Key,T> of(Key k1, T t1, Key k2, T t2, Key k3, T t3, Key k4, T t4, Key k5, T t5,
                               Key k6, T t6) {
    	QMultiMap<Key,T> result = new QMultiMap<>(
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
     * Returns a QMultiMap containing seven mappings.
     *
     * @param <Key> the {@code QMultiMap}'s key type
     * @param <T> the {@code QMultiMap}'s value type
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
     * @return a {@code QMultiMap} containing the specified mappings
     * @throws NullPointerException if any key or value is {@code null}
     */
    public static <Key,T> QMultiMap<Key,T> of(Key k1, T t1, Key k2, T t2, Key k3, T t3, Key k4, T t4, Key k5, T t5,
                               Key k6, T t6, Key k7, T t7) {
    	QMultiMap<Key,T> result = new QMultiMap<>(
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
     * Returns a QMultiMap containing eight mappings.
     *
     * @param <Key> the {@code QMultiMap}'s key type
     * @param <T> the {@code QMultiMap}'s value type
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
     * @return a {@code QMultiMap} containing the specified mappings
     * @throws NullPointerException if any key or value is {@code null}
     */
    public static <Key,T> QMultiMap<Key,T> of(Key k1, T t1, Key k2, T t2, Key k3, T t3, Key k4, T t4, Key k5, T t5,
                               Key k6, T t6, Key k7, T t7, Key k8, T t8) {
    	QMultiMap<Key,T> result = new QMultiMap<>(
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
     * Returns a QMultiMap containing nine mappings.
     *
     * @param <Key> the {@code QMultiMap}'s key type
     * @param <T> the {@code QMultiMap}'s value type
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
     * @return a {@code QMultiMap} containing the specified mappings
     * @throws NullPointerException if any key or value is {@code null}
     */
    public static <Key,T> QMultiMap<Key,T> of(Key k1, T t1, Key k2, T t2, Key k3, T t3, Key k4, T t4, Key k5, T t5,
                               Key k6, T t6, Key k7, T t7, Key k8, T t8, Key k9, T t9) {
    	QMultiMap<Key,T> result = new QMultiMap<>(
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
     * Returns a QMultiMap containing ten mappings.
     *
     * @param <Key> the {@code QMultiMap}'s key type
     * @param <T> the {@code QMultiMap}'s value type
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
     * @return a {@code QMultiMap} containing the specified mappings
     * @throws NullPointerException if any key or value is {@code null}
     */
    public static <Key,T> QMultiMap<Key,T> of(Key k1, T t1, Key k2, T t2, Key k3, T t3, Key k4, T t4, Key k5, T t5,
                               Key k6, T t6, Key k7, T t7, Key k8, T t8, Key k9, T t9, Key k10, T t10) {
    	QMultiMap<Key,T> result = new QMultiMap<>(
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
     * @param <Key> the {@code QMultiMap}'s key type
     * @param <T> the {@code QMultiMap}'s value type
     * @param entries {@code java.util.Map.Entry}s containing the keys and values from which the map is populated
     * @return a {@code QMultiMap} containing the specified mappings
     * @throws NullPointerException if any entry, key, or value is {@code null}, or if
     *         the {@code entries} array is {@code null}
     */
    @SafeVarargs
    public static <Key,T> QMultiMap<Key,T> ofEntries(Map.Entry<? extends Key, ? extends T> entry0, Map.Entry<? extends Key, ? extends T>... entries) {
    	QPair<QMetaType, QMetaType> metaTypes = QMap.findMapMetaType(entry0, entries);
    	QMultiMap<Key,T> result = new QMultiMap<>(metaTypes.first, metaTypes.second);
    	result.insert(entry0.getKey(), entry0.getValue());
    	for (Map.Entry<? extends Key, ? extends T> entry : entries) {
        	result.insert(entry.getKey(), entry.getValue());
		}
        return result;
    }
    
    private static <Key,T> QPair<QMetaType,QMetaType> findMapMetaType(Map<Key,List<T>> elements){
        if(elements.getClass()==QMap.class) {
            return new QPair<>(((QMap<?,?>)elements).keyMetaType(), ((QMap<?,?>)elements).valueMetaType());
        }else if(elements.getClass()==QHash.class) {
            return new QPair<>(((QHash<?,?>)elements).keyMetaType(), ((QHash<?,?>)elements).valueMetaType());
        }else if(elements.getClass()==QMultiMap.class) {
            return new QPair<>(((QMultiMap<Key,T>)elements).keyMetaType(), ((QMultiMap<Key,T>)elements).valueMetaType());
        }else if(elements.getClass()==QMultiHash.class) {
            return new QPair<>(((QMultiHash<Key,T>)elements).keyMetaType(), ((QMultiHash<Key,T>)elements).valueMetaType());
        }else {
        	QMetaType keyMetaType = new QMetaType();
        	Class<?> keyType = null;
        	QMetaType valueMetaType = new QMetaType();
        	Class<?> valueType = null;
        	for(Entry<Key,List<T>> entry : elements.entrySet()) {
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
    
    /**
     * <p>Equivalent to {@code subMap(fromKey, true, toKey, false)}.
     * @see NavigableMap#subMap(Object, Object)
     */
	@Override
    @QtUninvokable
	public final NavigableMap<Key,List<T>> subMap(Key fromKey, Key toKey) {
		return subMap(fromKey, true, toKey, false);
	}

    /**
     * <p>Equivalent to {@code headMap(toKey, false)}.
     * @see NavigableMap#headMap(Object)
     */
	@Override
    @QtUninvokable
	public final NavigableMap<Key,List<T>> headMap(Key toKey) {
		return headMap(toKey, false);
	}

    /**
     * <p>Equivalent to {@code tailMap(fromKey, true)}.
     * @see NavigableMap#tailMap(Object)
     */
	@Override
    @QtUninvokable
	public final NavigableMap<Key,List<T>> tailMap(Key fromKey) {
		return tailMap(fromKey, true);
	}

    /**
     * Returns a key-value mapping associated with the greatest key
     * strictly less than the given key, or {@code null} if there is
     * no such key.
     * @see NavigableMap#lowerEntry(Object)
     */
    @Override
    @QtUninvokable
	public final Entry<Key, List<T>> lowerEntry(Key key) {
    	QAssociativeConstIterator<Key,T> iterator = lowerBound(key);
    	if(!iterator.equals(constEnd()) && !iterator.equals(constBegin()) && Objects.equals(iterator.checkedKey(), key))
    		iterator.decrement();
    	if(iterator.equals(constEnd()))
    		return null;
    	else {
    		List<T> values = new ArrayList<>();
    		Key _key = iterator.checkedKey();
    		Comparator<? super Key> comparator = comparator();
    		for(; comparator.compare(_key, iterator.checkedKey())==0 && !iterator.equals(constEnd()); iterator.increment()) {
    			values.add(iterator.checkedValue());
    		}
    		return new AbstractMap.SimpleImmutableEntry<>(iterator.checkedKey(), values);
    	}
	}

    /**
     * Returns the greatest key strictly less than the given key, or
     * {@code null} if there is no such key.
     * @see NavigableMap#lowerKey(Object)
     */
	@Override
    @QtUninvokable
	public final Key lowerKey(Key key) {
    	QAssociativeConstIterator<Key,T> iterator = lowerBound(key);
    	if(!iterator.equals(constEnd()) && !iterator.equals(constBegin()) && Objects.equals(iterator.checkedKey(), key))
    		iterator.decrement();
    	if(iterator.equals(constEnd()))
    		return null;
    	else return iterator.checkedKey();
	}

    /**
     * Returns a key-value mapping associated with the greatest key
     * less than or equal to the given key, or {@code null} if there
     * is no such key.
     * @see NavigableMap#floorEntry(Object)
     */
	@Override
    @QtUninvokable
	public final Entry<Key, List<T>> floorEntry(Key key) {
    	QAssociativeConstIterator<Key,T> iterator = lowerBound(key);
    	if(iterator.equals(constEnd()))
    		return null;
    	else {
    		List<T> values = new ArrayList<>();
    		Key _key = iterator.checkedKey();
    		Comparator<? super Key> comparator = comparator();
    		for(; comparator.compare(_key, iterator.checkedKey())==0 && !iterator.equals(constEnd()); iterator.increment()) {
    			values.add(iterator.checkedValue());
    		}
    		return new AbstractMap.SimpleImmutableEntry<>(_key, values);
    	}
	}

    /**
     * Returns the greatest key less than or equal to the given key,
     * or {@code null} if there is no such key.
     * @see NavigableMap#floorKey(Object)
     */
	@Override
    @QtUninvokable
	public final Key floorKey(Key key) {
    	QAssociativeConstIterator<Key,T> iterator = lowerBound(key);
    	if(iterator.equals(constEnd()))
    		return null;
    	else return iterator.checkedKey();
	}

    /**
     * Returns a key-value mapping associated with the least key
     * greater than or equal to the given key, or {@code null} if
     * there is no such key.
     * @see NavigableMap#ceilingEntry(Object)
     */
	@Override
    @QtUninvokable
	public final Entry<Key, List<T>> ceilingEntry(Key key) {
    	QAssociativeConstIterator<Key,T> iterator = lowerBound(key);
    	if(iterator.equals(constEnd()))
    		return null;
    	else {
    		List<T> values = new ArrayList<>();
    		Key _key = iterator.checkedKey();
    		Comparator<? super Key> comparator = comparator();
    		for(; comparator.compare(_key, iterator.checkedKey())==0 && !iterator.equals(constEnd()); iterator.increment()) {
    			values.add(iterator.checkedValue());
    		}
    		return new AbstractMap.SimpleImmutableEntry<>(_key, values);
    	}
	}

    /**
     * Returns the least key greater than or equal to the given key,
     * or {@code null} if there is no such key.
     * @see NavigableMap#ceilingKey(Object)
     */
	@Override
    @QtUninvokable
	public final Key ceilingKey(Key key) {
    	QAssociativeConstIterator<Key,T> iterator = lowerBound(key);
    	if(iterator.equals(constEnd()))
    		return null;
    	else return iterator.checkedKey();
	}

    /**
     * Returns a key-value mapping associated with the least key
     * strictly greater than the given key, or {@code null} if there
     * is no such key.
     * @see NavigableMap#higherEntry(Object)
     */
	@Override
    @QtUninvokable
	public final Entry<Key, List<T>> higherEntry(Key key) {
    	QAssociativeConstIterator<Key,T> iterator = lowerBound(key);
    	if(!iterator.equals(constEnd()) && Objects.equals(iterator.checkedKey(), key))
    		iterator.increment();
    	if(iterator.equals(constEnd()))
    		return null;
    	else {
    		List<T> values = new ArrayList<>();
    		Key _key = iterator.checkedKey();
    		Comparator<? super Key> comparator = comparator();
    		for(; comparator.compare(_key, iterator.checkedKey())==0 && !iterator.equals(constEnd()); iterator.increment()) {
    			values.add(iterator.checkedValue());
    		}
    		return new AbstractMap.SimpleImmutableEntry<>(_key, values);
    	}
	}

    /**
     * Returns the least key strictly greater than the given key, or
     * {@code null} if there is no such key.
     * @see NavigableMap#higherKey(Object)
     */
	@Override
    @QtUninvokable
	public final Key higherKey(Key key) {
    	QAssociativeConstIterator<Key,T> iterator = lowerBound(key);
    	if(!iterator.equals(constEnd()) && Objects.equals(iterator.checkedKey(), key))
    		iterator.increment();
    	if(iterator.equals(constEnd()))
    		return null;
    	else
    		return iterator.checkedKey();
	}

    /**
     * Returns a key-value mapping associated with the least
     * key in this map, or {@code null} if the map is empty.
     * @see NavigableMap#firstEntry()
     */
	@Override
    @QtUninvokable
	public final Entry<Key, List<T>> firstEntry() {
		if(isEmpty())
			return null;
		else {
			QAssociativeConstIterator<Key,T> iterator = constBegin();
    		List<T> values = new ArrayList<>();
    		Key _key = iterator.checkedKey();
    		Comparator<? super Key> comparator = comparator();
    		for(; comparator.compare(_key, iterator.checkedKey())==0 && !iterator.equals(constEnd()); iterator.increment()) {
    			values.add(iterator.checkedValue());
    		}
    		return new AbstractMap.SimpleImmutableEntry<>(_key, values);
		}
	}

    /**
     * Returns a key-value mapping associated with the greatest
     * key in this map, or {@code null} if the map is empty.
     * @see NavigableMap#lastEntry()
     */
	@Override
    @QtUninvokable
	public final Entry<Key, List<T>> lastEntry() {
		if(isEmpty())
			return null;
		else {
			QAssociativeConstIterator<Key,T> iterator = constEnd();
			iterator.decrement();
    		List<T> values = new ArrayList<>();
    		Key _key = iterator.checkedKey();
    		Comparator<? super Key> comparator = comparator();
    		while(comparator.compare(_key, iterator.checkedKey())==0) {
    			values.add(0, iterator.checkedValue());
    			iterator.decrement();
    			if(iterator.equals(constBegin())) {
    				
    			}
    		}
    		return new AbstractMap.SimpleImmutableEntry<>(_key, values);
		}
	}

    /**
     * Removes and returns a key-value mapping associated with
     * the least key in this map, or {@code null} if the map is empty.
     * @see NavigableMap#pollFirstEntry()
     */
	@Override
    @QtUninvokable
	public final Entry<Key, List<T>> pollFirstEntry() {
		Entry<Key, List<T>> entry = firstEntry();
		if(entry!=null) {
			remove(entry.getKey());
		}
		return entry;
	}

    /**
     * Removes and returns a key-value mapping associated with
     * the greatest key in this map, or {@code null} if the map is empty.
     * @see NavigableMap#pollLastEntry()
     */
	@Override
    @QtUninvokable
	public final Entry<Key, List<T>> pollLastEntry() {
		Entry<Key, List<T>> entry = lastEntry();
		if(entry!=null) {
			remove(entry.getKey());
		}
		return entry;
	}

    /**
     * Returns a reverse order view of the mappings contained in this map.
     * @see NavigableMap#descendingMap()
     */
	@Override
    @QtUninvokable
	public final NavigableMap<Key, List<T>> descendingMap() {
		Comparator<? super Key> comparator = this.comparator();
		TreeMap<Key, List<T>> descendingMap = new TreeMap<>((Key o1, Key o2)-> -1*comparator.compare(o1, o2));
		descendingMap.putAll(this);
		return descendingMap;
	}

    /**
     * Returns a {@link NavigableSet} view of the keys contained in this map.
     * @see NavigableMap#navigableKeySet()
     */
	@Override
    @QtUninvokable
	public final NavigableSet<Key> navigableKeySet() {
		TreeSet<Key> set = new TreeSet<>(this.comparator());
		set.addAll(this.keySet());
		return set;
	}

    /**
     * Returns a reverse order {@link NavigableSet} view of the keys contained in this map.
     * The set's iterator returns the keys in descending order.
     * @see NavigableMap#descendingKeySet()
     */
	@Override
    @QtUninvokable
	public final NavigableSet<Key> descendingKeySet() {
		Comparator<? super Key> comparator = this.comparator();
		TreeSet<Key> set = new TreeSet<>((Key o1, Key o2)-> -1*comparator.compare(o1, o2));
		set.addAll(this.keySet());
		return set;
	}
	
	private static <K,V> Function<? super K, ArrayList<V>> arrayListFactory(){
		return key->new ArrayList<>();
	}

    /**
     * Returns a view of the portion of this map whose keys range from
     * {@code fromKey} to {@code toKey}.  If {@code fromKey} and
     * {@code toKey} are equal, the returned map is empty unless
     * {@code fromInclusive} and {@code toInclusive} are both true.  The
     * returned map is backed by this map, so changes in the returned map are
     * reflected in this map, and vice-versa.  The returned map supports all
     * optional map operations that this map supports.
     * @see NavigableMap#subMap(Object, boolean, Object, boolean)
     */
	@SuppressWarnings("unchecked")
	@Override
    @QtUninvokable
	public final NavigableMap<Key, List<T>> subMap(Key fromKey, boolean fromInclusive, Key toKey, boolean toInclusive) {
		NavigableMap<Key,List<T>> map = this.clone();
		try {
			map.clear();
		} catch (Throwable e) {
		}
		if(!map.isEmpty()) {
			try {
				map = this.getClass().getConstructor().newInstance();
			} catch (Throwable e) {
				map = new TreeMap<>(comparator());
			}
		}
		QAssociativeConstIterator<Key,T> k2 = lowerBound(toKey);
		if(!k2.equals(constEnd())) {
			QAssociativeConstIterator<Key,T> k1 = lowerBound(fromKey);
			if(!fromInclusive) {
				k1.increment();
			}
			for(; !k1.equals(k2); k1.increment()) {
				map.computeIfAbsent(k1.checkedKey(), arrayListFactory()).add(k1.checkedValue());
			}
			if(toInclusive) {
				map.computeIfAbsent(k2.checkedKey(), arrayListFactory()).add(k2.checkedValue());
			}
		}
		return map;
	}

    /**
     * Returns a view of the portion of this map whose keys are less than (or
     * equal to, if {@code inclusive} is true) {@code toKey}.  The returned
     * map is backed by this map, so changes in the returned map are reflected
     * in this map, and vice-versa.  The returned map supports all optional
     * map operations that this map supports.
     * @see NavigableMap#headMap(Object, boolean)
     */
	@SuppressWarnings("unchecked")
	@Override
    @QtUninvokable
    public final NavigableMap<Key, List<T>> headMap(Key toKey, boolean inclusive) {
		QAssociativeConstIterator<Key,T> k = lowerBound(toKey);
		NavigableMap<Key,List<T>> map = this.clone();
		try {
			map.clear();
		} catch (Throwable e) {
		}
		if(!map.isEmpty()) {
			try {
				map = this.getClass().getConstructor().newInstance();
			} catch (Throwable e) {
				map = new TreeMap<>(comparator());
			}
		}
		if(!k.equals(constEnd())) {
			for(QAssociativeConstIterator<Key,T> iterator = constBegin(); !iterator.equals(k); iterator.increment()) {
				map.computeIfAbsent(iterator.checkedKey(), arrayListFactory()).add(iterator.checkedValue());
			}
			if(inclusive) {
				map.computeIfAbsent(k.checkedKey(), arrayListFactory()).add(k.checkedValue());
			}
		}
		return map;
	}

    /**
     * Returns a view of the portion of this map whose keys are greater than (or
     * equal to, if {@code inclusive} is true) {@code fromKey}.  The returned
     * map is backed by this map, so changes in the returned map are reflected
     * in this map, and vice-versa.
     * @see NavigableMap#tailMap(Object, boolean)
     */
	@SuppressWarnings("unchecked")
	@Override
    @QtUninvokable
	public final NavigableMap<Key, List<T>> tailMap(Key fromKey, boolean inclusive) {
		QAssociativeConstIterator<Key,T> k = lowerBound(fromKey);
		QAssociativeConstIterator<Key,T> end = constEnd();
		NavigableMap<Key,List<T>> map = this.clone();
		try {
			map.clear();
		} catch (Throwable e) {
		}
		if(!map.isEmpty()) {
			try {
				map = this.getClass().getConstructor().newInstance();
			} catch (Throwable e) {
				map = new TreeMap<>(comparator());
			}
		}
		if(!k.equals(end)) {
			if(!inclusive) {
				k.increment();
			}
			for(;!k.equals(end); k.increment()) {
				map.computeIfAbsent(k.checkedKey(), arrayListFactory()).add(k.checkedValue());
			}
		}
		return map;
	}
}
