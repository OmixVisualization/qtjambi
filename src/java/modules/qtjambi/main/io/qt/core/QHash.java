/****************************************************************************
**
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import static io.qt.core.QMap.findMapMetaType;

import java.util.*;
import java.util.function.*;

import io.qt.*;

/**
 * <p>Java wrapper for Qt class <a href="https://doc.qt.io/qt/qhash.html"><code>QHash</code></a></p>
 */
public class QHash<Key,T> extends AbstractAssociativeContainer<Key,T> implements Map<Key,T>, Cloneable
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
    protected QHash(QPrivateConstructor p) {
		super(p);
	}
    
    /**
     * Creating a container with given key and value type.
     * <p>See <code><a href="https://doc.qt.io/qt/qhash.html#QHash">QHash::<wbr>QHash()</a></code></p>
     * @param keyType the type K
     * @param valueType the type V
     */
    public QHash(@Nullable Class<Key> keyType, @Nullable Class<T> valueType) {
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
    
    /**
     * Creating a container with given content.
     * <p>See <code><a href="https://doc.qt.io/qt/qhash.html#QHash-2">QHash::<wbr>QHash(const QHash&lt;Key, T> &amp;)</a></code></p>
     * @param other map
     */
    public QHash(@StrictNonNull Map<Key,T> other) {
		super(null);
		QPair<QMetaType, QMetaType> metaTypes = QMap.findMapMetaType(Objects.requireNonNull(other, "Argument 'other': null not expected."));
		if(metaTypes.first==null || metaTypes.first.id()==0)
			throw new IllegalArgumentException("QMetaType::UnknownType cannot be key type of QMap.");
		if(metaTypes.first.id()==QMetaType.Type.Void.value())
			throw new IllegalArgumentException("void cannot be key type of QMap.");
		if(metaTypes.second==null || metaTypes.second.id()==0)
			throw new IllegalArgumentException("QMetaType::UnknownType cannot be value type of QMap.");
		if(metaTypes.second.id()==QMetaType.Type.Void.value())
			throw new IllegalArgumentException("void cannot be value type of QMap.");
		initialize(metaTypes.first.javaType(), QtJambi_LibraryUtilities.internal.nativeId(metaTypes.first), metaTypes.second.javaType(), QtJambi_LibraryUtilities.internal.nativeId(metaTypes.second), other);
	}
    
    /**
     * Creating a container with given key and value type.
     * <p>See <code><a href="https://doc.qt.io/qt/qhash.html#QHash">QHash::<wbr>QHash()</a></code></p>
     * @param keyType the type K
     * @param valueMetaType the type V
     */
	public QHash(@Nullable Class<Key> keyType, @StrictNonNull QMetaType valueMetaType) {
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
    
    /**
     * Creating a container with given key and value type.
     * <p>See <code><a href="https://doc.qt.io/qt/qhash.html#QHash">QHash::<wbr>QHash()</a></code></p>
     * @param keyMetaType the type K
     * @param valueType the type V
     */
	public QHash(@StrictNonNull QMetaType keyMetaType, @Nullable Class<T> valueType) {
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
	
    /**
     * Creating a container with given key and value type.
     * <p>See <code><a href="https://doc.qt.io/qt/qhash.html#QHash">QHash::<wbr>QHash()</a></code></p>
     * @param keyType the type K
     * @param valueMetaType the type V
     */
	public QHash(@Nullable Class<Key> keyType, QMetaType.@StrictNonNull Type valueMetaType) {
		this(keyType, new QMetaType(valueMetaType));
	}
	
    /**
     * Creating a container with given key and value type.
     * <p>See <code><a href="https://doc.qt.io/qt/qhash.html#QHash">QHash::<wbr>QHash()</a></code></p>
     * @param keyMetaType the type K
     * @param valueType the type V
     */
	public QHash(QMetaType.@StrictNonNull Type keyMetaType, @Nullable Class<T> valueType) {
		this(new QMetaType(keyMetaType), valueType);
	}
	
    /**
     * Creating a container with given key and value type.
     * <p>See <code><a href="https://doc.qt.io/qt/qhash.html#QHash">QHash::<wbr>QHash()</a></code></p>
     * @param keyMetaType the type K
     * @param valueMetaType the type V
     */
	public QHash(QMetaType.@StrictNonNull Type keyMetaType, @StrictNonNull QMetaType valueMetaType) {
		this(new QMetaType(keyMetaType), valueMetaType);
	}
	
    /**
     * Creating a container with given key and value type.
     * <p>See <code><a href="https://doc.qt.io/qt/qhash.html#QHash">QHash::<wbr>QHash()</a></code></p>
     * @param keyMetaType the type K
     * @param valueMetaType the type V
     */
	public QHash(@StrictNonNull QMetaType keyMetaType, QMetaType.@StrictNonNull Type valueMetaType) {
		this(keyMetaType, new QMetaType(valueMetaType));
	}
	
    /**
     * Creating a container with given key and value type.
     * <p>See <code><a href="https://doc.qt.io/qt/qhash.html#QHash">QHash::<wbr>QHash()</a></code></p>
     * @param keyMetaType the type K
     * @param valueMetaType the type V
     */
	public QHash(QMetaType.@StrictNonNull Type keyMetaType, QMetaType.@StrictNonNull Type valueMetaType) {
		this(new QMetaType(keyMetaType), new QMetaType(valueMetaType));
	}
    
    /**
     * Creating a container with given key and value type.
     * <p>See <code><a href="https://doc.qt.io/qt/qhash.html#QHash">QHash::<wbr>QHash()</a></code></p>
     * @param keyMetaType the type K
     * @param valueMetaType the type V
     */
	public QHash(@StrictNonNull QMetaType keyMetaType, @StrictNonNull QMetaType valueMetaType) {
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
	
	private QHash(QMetaType keyMetaType, QMetaType valueMetaType, Map<Key,T> other) {
		super(null);
		initialize(keyMetaType.javaType(), QtJambi_LibraryUtilities.internal.nativeId(keyMetaType), valueMetaType.javaType(), QtJambi_LibraryUtilities.internal.nativeId(valueMetaType), other);
	}
    
    @QtUninvokable
    private native void initialize(Class<?> keyType, long keyMetaType, Class<?> valueType, long valueMetaType, Map<Key, T> other);
    
    /**
     * Creating a QHash with QVariant type ({@code QHash<QString,QVariant>}).
     * <p>See <code><a href="https://doc.qt.io/qt/qhash.html#QHash">QHash::<wbr>QHash()</a></code></p>
     */
    public static @NonNull QHash<@NonNull String,Object> createVariantHash() {
    	return new QHash<>(new QMetaType(QMetaType.Type.QString), new QMetaType(QMetaType.Type.QVariant));
    }
    
    /**
     * Creates and returns a copy of this object.
     * <p>See <code><a href="https://doc.qt.io/qt/qhash.html#QHash-2">QHash::<wbr>QHash(const QHash&lt;Key, T> &amp;)</a></code></p>
     */
    @Override
	public @NonNull QHash<Key,T> clone(){
		return new QHash<>(this);
	}
    
    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qhash.html#capacity">QHash::<wbr>capacity()const</a></code></p>
     */
    @QtUninvokable
    public final int capacity()    {
        return capacity(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private native int capacity(long __this__nativeId);

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qhash.html#clear">QHash::<wbr>clear()</a></code></p>
     */
    @QtUninvokable
    public final void clear()    {
        clear(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private native void clear(long __this__nativeId);

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qhash.html#contains">QHash::<wbr>contains(Key)const</a></code></p>
     */
    @QtUninvokable
    public final boolean contains(Key key)    {
        try{
        	return contains(QtJambi_LibraryUtilities.internal.nativeId(this), key);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QSet.handleException(e, keyMetaType(), key);
        }
    }
    @QtUninvokable
    private native boolean contains(long __this__nativeId, Object key);

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qhash.html#count-1">QHash::<wbr>count()const</a></code></p>
     */
    @QtUninvokable
    public final int count()    {
        return size();
    }

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qhash.html#count">QHash::<wbr>count(Key)const</a></code></p>
     */
    @QtUninvokable
    public final int count(Key key)    {
        try{
        	return count(QtJambi_LibraryUtilities.internal.nativeId(this), key);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QSet.handleException(e, keyMetaType(), key);
        }
    }
    @QtUninvokable
    private native int count(long __this__nativeId, Key key);

    /**
     * Provides a constant C++ iterator to the containers end.
     * <p>See <code><a href="https://doc.qt.io/qt/qhash.html#constEnd">QHash::<wbr>constEnd()const</a></code></p>
     * @return end
     */
    @QtUninvokable
    protected final io.qt.core.@NonNull QAssociativeConstIterator<Key,T> constEnd()    {
        return constEnd(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private native io.qt.core.QAssociativeConstIterator<Key,T> constEnd(long __this__nativeId);
    
    /**
     * Provides a constant C++ iterator to the containers begin.
     * <p>See <code><a href="https://doc.qt.io/qt/qhash.html#constBegin">QHash::<wbr>constBegin()const</a></code></p>
     * @return begin
     */
	@QtUninvokable
    protected final io.qt.core.@NonNull QAssociativeConstIterator<Key,T> constBegin()    {
        return constBegin(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private native io.qt.core.QAssociativeConstIterator<Key,T> constBegin(long __this__nativeId);

    /**
     * Provides a mutable C++ iterator to the containers end.
     * <p>See <code><a href="https://doc.qt.io/qt/qhash.html#end">QHash::<wbr>end()</a></code></p>
     * @return end
     */
    @QtUninvokable
    protected final io.qt.core.@NonNull QAssociativeIterator<Key,T> end()    {
        return end(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private native io.qt.core.QAssociativeIterator<Key,T> end(long __this__nativeId);
    
    /**
     * Provides a mutable C++ iterator to the containers begin.
     * <p>See <code><a href="https://doc.qt.io/qt/qhash.html#begin">QHash::<wbr>begin()</a></code></p>
     * @return begin
     */
	@QtUninvokable
    protected final io.qt.core.@NonNull QAssociativeIterator<Key,T> begin()    {
        return begin(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private native io.qt.core.QAssociativeIterator<Key,T> begin(long __this__nativeId);

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qhash.html#find">QHash::<wbr>find(Key)const</a></code></p>
     * @return iterator
     */
    @QtUninvokable
    public final io.qt.core.@NonNull QAssociativeConstIterator<Key,T> find(Key key)    {
        try{
        	return find(QtJambi_LibraryUtilities.internal.nativeId(this), key);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QSet.handleException(e, keyMetaType(), key);
        }
    }
    @QtUninvokable
    private native io.qt.core.QAssociativeConstIterator<Key,T> find(long __this__nativeId, Key key);

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qhash.html#insert">QHash::<wbr>insert(Key,T)</a></code></p>
     */
    @QtUninvokable
    public final void insert(Key key, T value)    {
    	try {
    		insert(QtJambi_LibraryUtilities.internal.nativeId(this), key, value);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QMap.handleException(e, keyMetaType(), valueMetaType(), key, value);
    	}
    }
    @QtUninvokable
    private native void insert(long __this__nativeId, Key key, T value);

    /**
     * <p>See <a href="https://doc.qt.io/qt/qhash.html#isEmpty">QHash::<wbr>isEmpty()const</a></p>
     */
    @QtUninvokable
    public final boolean isEmpty()    {
        return size()==0;
    }

    /**
     * <p>See <a href="https://doc.qt.io/qt/qhash.html#key">QHash::<wbr>key(T)const</a></p>
     */
    @QtUninvokable
    public final Key key(T value) {
        return key(value, null);
    }

    /**
     * <p>See <a href="https://doc.qt.io/qt/qhash.html#key-1">QHash::<wbr>key(T,Key)const</a></p>
     */
    @QtUninvokable
    public final Key key(T value, Key defaultKey) {
        try{
        	return key(QtJambi_LibraryUtilities.internal.nativeId(this), value, defaultKey);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QMap.handleException(e, keyMetaType(), valueMetaType(), defaultKey, value);
        }
    }
    @QtUninvokable
    private native Key key(long __this__nativeId, T value, Key defaultKey);

	/**
	 * Returns a {@link List} of the keys contained in this associative container.
     * <p>See <a href="https://doc.qt.io/qt/qhash.html#keys">QHash::<wbr>keys()const</a></p>
	 * @return list of keys
	 */
    @QtUninvokable
    public final @NonNull QList<Key> keys()    {
        return keys(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private native QList<Key> keys(long __this__nativeId);

	/**
     * <p>See <a href="https://doc.qt.io/qt/qhash.html#keys-1">QHash::<wbr>keys(T)const</a></p>
	 */
    @QtUninvokable
    public final @NonNull QList<Key> keys(T value)    {
    	try {
    		return keysForValue(QtJambi_LibraryUtilities.internal.nativeId(this), value);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QSet.handleException(e, valueMetaType(), value);
    	}
    }
    @QtUninvokable
    private native QList<Key> keysForValue(long __this__nativeId, Object value);

	/**
     * <p>See <a href="https://doc.qt.io/qt/qhash.html#removeIf">QHash::<wbr>removeIf(Predicate)</a></p>
	 */
    @QtUninvokable
    public final int removeIf(@StrictNonNull Predicate<Key> predicate)    {
    	List<Key> keys = new ArrayList<>();
    	final long nativeId = QtJambi_LibraryUtilities.internal.nativeId(this);
    	for(Key key : keys(nativeId)) {
    		if(predicate.test(key))
    			keys.add(key);
    	}
    	int count = 0;
    	for (Key key : keys) {
			count += remove(nativeId, key);
		}
    	return count;
    }

	/**
     * <p>See <a href="https://doc.qt.io/qt/qhash.html#removeIf">QHash::<wbr>removeIf(Predicate)</a></p>
	 */
    @QtUninvokable
    public final int removeIf(@StrictNonNull BiPredicate<Key,T> predicate)    {
    	List<QPair<Key,T>> pairs = new ArrayList<>();
    	final long nativeId = QtJambi_LibraryUtilities.internal.nativeId(this);
    	for(QPair<Key,T> pair : constBegin(nativeId)) {
    		if(predicate.test(pair.first, pair.second))
    			pairs.add(pair);
    	}
    	int count = 0;
    	for (QPair<Key,T> pair : pairs) {
			count += remove(nativeId, pair.first);
		}
    	return count;
    }
    
	/**
     * <p>See <a href="https://doc.qt.io/qt/qhash.html#remove">QHash::<wbr>remove(Key)</a></p>
	 */
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
    private native int remove(long __this__nativeId, Object key);
    
	/**
     * <p>See <a href="https://doc.qt.io/qt/qhash.html#reserve">QHash::<wbr>reserve(int)</a></p>
	 */
    @QtUninvokable
    public final void reserve(int size)    {
        reserve(QtJambi_LibraryUtilities.internal.nativeId(this), size);
    }
    @QtUninvokable
    private native void reserve(long __this__nativeId, int size);

	/**
     * <p>See <a href="https://doc.qt.io/qt/qhash.html#size">QHash::<wbr>size()const</a></p>
	 */
    @QtUninvokable
    public final int size()    {
        return size(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private native int size(long __this__nativeId);
    
	/**
     * <p>See <a href="https://doc.qt.io/qt/qhash.html#squeeze">QHash::<wbr>squeeze()</a></p>
	 */
    @QtUninvokable
    public final void squeeze() { 
    	if(capacity()>0)
    		reserve(0); 
	}

	/**
     * <p>See <a href="https://doc.qt.io/qt/qhash.html#take">QHash::<wbr>take(Key)</a></p>
	 */
    @QtUninvokable
    public final T take(Key key)    {
    	try {
    		return take(QtJambi_LibraryUtilities.internal.nativeId(this), key);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QSet.handleException(e, keyMetaType(), key);
    	}
    }
    @QtUninvokable
    private native T take(long __this__nativeId, Key key);

	/**
     * <p>See <a href="https://doc.qt.io/qt/qhash.html#value">QHash::<wbr>value(Key)</a></p>
	 */
    @QtUninvokable
    public final T value(Key key) {
        return value(key, null);
    }

	/**
     * <p>See <a href="https://doc.qt.io/qt/qhash.html#value-1">QHash::<wbr>value(Key,T)</a></p>
	 */
    @QtUninvokable
    public final T value(Key key, T defaultValue)    {
    	try {
    		return value(QtJambi_LibraryUtilities.internal.nativeId(this), key, defaultValue);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QMap.handleException(e, keyMetaType(), valueMetaType(), key, defaultValue);
    	}
    }
    @QtUninvokable
    private native T value(long __this__nativeId, Object key, Object defaultValue);

	/**
     * <p>See <a href="https://doc.qt.io/qt/qhash.html#values">QHash::<wbr>values()const</a></p>
	 */
    @QtUninvokable
    public final @NonNull QList<T> values()    {
        return values(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private native QList<T> values(long __this__nativeId);

    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qhash.html#operator-eq-eq">operator==<wbr>(QHash&lt;Key,T>,QHash&lt;Key,T>)</a></code></p>
     */
	@Override
    @QtUninvokable
    public boolean equals(Object other) {
        if (other instanceof java.util.Map && QMap.checkContainerType(this.keyMetaType(), this.valueMetaType(), (java.util.Map<?,?>) other)) {
            return operator_equal(QtJambi_LibraryUtilities.internal.nativeId(this), (java.util.Map<?,?>) other);
        }
        return false;
    }
    @QtUninvokable
    private native boolean operator_equal(long __this__nativeId, java.util.Map<?,?> other);

    /**
     * Returns the objects's hash code computed by <code>qHash(QHash&lt;Key,T>)</code>.
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
    private native int hashCode(long __this__nativeId);
    
    /**
     * Returns the string representation of the object given by <code>QVariant(this).toString()</code>.
     */
    @Override
    @QtUninvokable
    public @NonNull String toString() {
    	try {
			return toString(QtJambi_LibraryUtilities.internal.nativeId(this));
		} catch (QNoImplementationException e) {
			return super.toString();
		} catch (QNoNativeResourcesException e) {
			return "null";
		}
    }
    @QtUninvokable
    private native String toString(long __this__nativeId);
    
    @QtUninvokable
    private native boolean lessThan(Key key1, Key key2);

    /**
     * Returns {@code true} if this map contains a mapping for the specified key.
     * @see Map#containsKey(Object)
     * @see #contains(Object)
     */
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

    /**
     * Returns the value to which the specified key is mapped,
     * or {@code null} if this map contains no mapping for the key.
     * @see Map#get(Object)
     * @see #value(Object)
     */
    @Override
    @QtUninvokable
    public final T get(Object key){
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

    /**
     * Associates the specified value with the specified key in this map.
     * @see Map#put(Object, Object)
     * @see #insert(Object, Object)
     */
    @Override
    @QtUninvokable
    public final T put(Key key, T value){
        T old = value(key);
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
    public final T remove(Object key){
        try{
        	return take(QtJambi_LibraryUtilities.internal.nativeId(this), (Key)key);
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
     * <p>See <code><a href="https://doc.qt.io/qt/qhash.html#operator-lt-lt">operator&lt;&lt;(QDataStream&amp;,<wbr>QHash&lt;Key,T>)</a></code></p>
     */
    @io.qt.QtUninvokable
    public void writeTo(io.qt.core.@StrictNonNull QDataStream stream){
    	java.util.Objects.requireNonNull(stream, "Argument 'stream': null not expected.");
        writeTo(QtJambi_LibraryUtilities.internal.nativeId(this), QtJambi_LibraryUtilities.internal.nativeId(stream));
    }
    
    @io.qt.QtUninvokable
    private native void writeTo(long __this__nativeId, long stream);
    
    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qhash.html#operator-gt-gt">operator&gt;&gt;(QDataStream&amp;,<wbr>QHash&lt;Key,T>&amp;)</a></code></p>
     */
    @io.qt.QtUninvokable
    public void readFrom(io.qt.core.@StrictNonNull QDataStream stream){
    	java.util.Objects.requireNonNull(stream, "Argument 'stream': null not expected.");
        readFrom(QtJambi_LibraryUtilities.internal.nativeId(this), QtJambi_LibraryUtilities.internal.nativeId(stream));
    }
    
    @io.qt.QtUninvokable
    private native void readFrom(long __this__nativeId, long stream);
    
    @io.qt.QtUninvokable
    final QMetaType keyMetaType() {
    	return keyMetaType(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @io.qt.QtUninvokable
    private native QMetaType keyMetaType(long __this_nativeId);
    
    @io.qt.QtUninvokable
    final QMetaType valueMetaType() {
    	return valueMetaType(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @io.qt.QtUninvokable
    private native QMetaType valueMetaType(long __this_nativeId);
    
    /**
     * Returns a QHash containing a single mapping.
     *
     * @param <Key> the {@code QHash}'s key type
     * @param <T> the {@code QHash}'s value type
     * @param k1 the mapping's key
     * @param t1 the mapping's value
     * @return a {@code QHash} containing the specified mapping
     * @throws NullPointerException if the key or the value is {@code null}
     */
    public static <Key,T> @NonNull QHash<Key,T> of(Key k1, T t1) {
    	return ofTyped(
    			QList.findElementMetaType(k1), 
    			QList.findElementMetaType(t1), k1, t1);
    }

    /**
     * Returns a QHash containing two mappings.
     *
     * @param <Key> the {@code QHash}'s key type
     * @param <T> the {@code QHash}'s value type
     * @param k1 the first mapping's key
     * @param t1 the first mapping's value
     * @param k2 the second mapping's key
     * @param t2 the second mapping's value
     * @return a {@code QHash} containing the specified mappings
     * @throws NullPointerException if any key or value is {@code null}
     */
    public static <Key,T> @NonNull QHash<Key,T> of(Key k1, T t1, Key k2, T t2) {
    	return ofTyped(
    			QList.findElementMetaType(k1, k2), 
    			QList.findElementMetaType(t1, t2),
    			k1, t1,
    			k2, t2);
    }

    /**
     * Returns a QHash containing three mappings.
     *
     * @param <Key> the {@code QHash}'s key type
     * @param <T> the {@code QHash}'s value type
     * @param k1 the first mapping's key
     * @param t1 the first mapping's value
     * @param k2 the second mapping's key
     * @param t2 the second mapping's value
     * @param k3 the third mapping's key
     * @param t3 the third mapping's value
     * @return a {@code QHash} containing the specified mappings
     * @throws NullPointerException if any key or value is {@code null}
     */
    public static <Key,T> @NonNull QHash<Key,T> of(Key k1, T t1, Key k2, T t2, Key k3, T t3) {
    	return ofTyped(
    			QList.findElementMetaType(k1, k2, k3), 
    			QList.findElementMetaType(t1, t2, t3),
    			k1, t1,
    			k2, t2,
    			k3, t3);
    }

    /**
     * Returns a QHash containing four mappings.
     *
     * @param <Key> the {@code QHash}'s key type
     * @param <T> the {@code QHash}'s value type
     * @param k1 the first mapping's key
     * @param t1 the first mapping's value
     * @param k2 the second mapping's key
     * @param t2 the second mapping's value
     * @param k3 the third mapping's key
     * @param t3 the third mapping's value
     * @param k4 the fourth mapping's key
     * @param t4 the fourth mapping's value
     * @return a {@code QHash} containing the specified mappings
     * @throws NullPointerException if any key or value is {@code null}
     */
    public static <Key,T> @NonNull QHash<Key,T> of(Key k1, T t1, Key k2, T t2, Key k3, T t3, Key k4, T t4) {
    	return ofTyped(
    			QList.findElementMetaType(k1, k2, k3, k4), 
    			QList.findElementMetaType(t1, t2, t3, t4),
    			k1, t1,
    			k2, t2,
    			k3, t3,
    			k4, t4);
    }

    /**
     * Returns a QHash containing five mappings.
     *
     * @param <Key> the {@code QHash}'s key type
     * @param <T> the {@code QHash}'s value type
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
     * @return a {@code QHash} containing the specified mappings
     * @throws NullPointerException if any key or value is {@code null}
     */
    public static <Key,T> @NonNull QHash<Key,T> of(Key k1, T t1, Key k2, T t2, Key k3, T t3, Key k4, T t4, Key k5, T t5) {
    	return ofTyped(
    			QList.findElementMetaType(k1, k2, k3, k4, k5), 
    			QList.findElementMetaType(t1, t2, t3, t4, t5),
    			k1, t1,
    			k2, t2,
    			k3, t3,
    			k4, t4,
    			k5, t5);
    }

    /**
     * Returns a QHash containing six mappings.
     *
     * @param <Key> the {@code QHash}'s key type
     * @param <T> the {@code QHash}'s value type
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
     * @return a {@code QHash} containing the specified mappings
     * @throws NullPointerException if any key or value is {@code null}
     */
    public static <Key,T> @NonNull QHash<Key,T> of(Key k1, T t1, Key k2, T t2, Key k3, T t3, Key k4, T t4, Key k5, T t5, Key k6, T t6) {
    	return ofTyped(
    			QList.findElementMetaType(k1, k2, k3, k4, k5, k6), 
    			QList.findElementMetaType(t1, t2, t3, t4, t5, t6),
    			k1, t1,
    			k2, t2,
    			k3, t3,
    			k4, t4,
    			k5, t5,
    			k6, t6);
    }

    /**
     * Returns a QHash containing seven mappings.
     *
     * @param <Key> the {@code QHash}'s key type
     * @param <T> the {@code QHash}'s value type
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
     * @return a {@code QHash} containing the specified mappings
     * @throws NullPointerException if any key or value is {@code null}
     */
    public static <Key,T> @NonNull QHash<Key,T> of(Key k1, T t1, Key k2, T t2, Key k3, T t3, Key k4, T t4, Key k5, T t5,
                               Key k6, T t6, Key k7, T t7) {
    	return ofTyped(
    			QList.findElementMetaType(k1, k2, k3, k4, k5, k6, k7), 
    			QList.findElementMetaType(t1, t2, t3, t4, t5, t6, t7),
    			k1, t1,
    			k2, t2,
    			k3, t3,
    			k4, t4,
    			k5, t5,
    			k6, t6,
    			k7, t7);
    }

    /**
     * Returns a QHash containing eight mappings.
     *
     * @param <Key> the {@code QHash}'s key type
     * @param <T> the {@code QHash}'s value type
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
     * @return a {@code QHash} containing the specified mappings
     * @throws NullPointerException if any key or value is {@code null}
     */
    public static <Key,T> @NonNull QHash<Key,T> of(Key k1, T t1, Key k2, T t2, Key k3, T t3, Key k4, T t4, Key k5, T t5,
                               Key k6, T t6, Key k7, T t7, Key k8, T t8) {
    	return ofTyped(
    			QList.findElementMetaType(k1, k2, k3, k4, k5, k6, k7, k8), 
    			QList.findElementMetaType(t1, t2, t3, t4, t5, t6, t7, t8),
    			k1, t1,
    			k2, t2,
    			k3, t3,
    			k4, t4,
    			k5, t5,
    			k6, t6,
    			k7, t7,
    			k8, t8);
    }

    /**
     * Returns a QHash containing nine mappings.
     *
     * @param <Key> the {@code QHash}'s key type
     * @param <T> the {@code QHash}'s value type
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
     * @return a {@code QHash} containing the specified mappings
     * @throws NullPointerException if any key or value is {@code null}
     */
    public static <Key,T> @NonNull QHash<Key,T> of(Key k1, T t1, Key k2, T t2, Key k3, T t3, Key k4, T t4, Key k5, T t5,
                               Key k6, T t6, Key k7, T t7, Key k8, T t8, Key k9, T t9) {
    	return ofTyped(
    			QList.findElementMetaType(k1, k2, k3, k4, k5, k6, k7, k8, k9), 
    			QList.findElementMetaType(t1, t2, t3, t4, t5, t6, t7, t8, t9),
    			k1, t1,
    			k2, t2,
    			k3, t3,
    			k4, t4,
    			k5, t5,
    			k6, t6,
    			k7, t7,
    			k8, t8,
    			k9, t9);
    }

    /**
     * Returns a QHash containing ten mappings.
     *
     * @param <Key> the {@code QHash}'s key type
     * @param <T> the {@code QHash}'s value type
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
     * @return a {@code QHash} containing the specified mappings
     * @throws NullPointerException if any key or value is {@code null}
     */
    public static <Key,T> @NonNull QHash<Key,T> of(Key k1, T t1, Key k2, T t2, Key k3, T t3, Key k4, T t4, Key k5, T t5,
                               Key k6, T t6, Key k7, T t7, Key k8, T t8, Key k9, T t9, Key k10, T t10) {
    	return ofTyped(
    			QList.findElementMetaType(k1, k2, k3, k4, k5, k6, k7, k8, k9, k10), 
    			QList.findElementMetaType(t1, t2, t3, t4, t5, t6, t7, t8, t9, t10),
    			k1, t1,
    			k2, t2,
    			k3, t3,
    			k4, t4,
    			k5, t5,
    			k6, t6,
    			k7, t7,
    			k8, t8,
    			k9, t9,
    			k10, t10);
    }
    
    /**
     * Returns a QHash containing a single mapping.
     *
     * @param <Key> the {@code QHash}'s key type
     * @param <T> the {@code QHash}'s value type
     * @param keyType key type
     * @param valueType value type
     * @param k1 the mapping's key
     * @param t1 the mapping's value
     * @return a {@code QHash} containing the specified mapping
     */
    public static <Key,T> @NonNull QHash<Key,T> ofTyped(@Nullable Class<Key> keyType, @Nullable Class<T> valueType, Key k1, T t1) {
    	return ofTyped(
    			QList.findElementMetaType(keyType, k1), 
    			QList.findElementMetaType(valueType, t1),
    			k1, t1);
    }

    /**
     * Returns a QHash containing two mappings.
     *
     * @param <Key> the {@code QHash}'s key type
     * @param <T> the {@code QHash}'s value type
     * @param keyType key type
     * @param valueType value type
     * @param k1 the first mapping's key
     * @param t1 the first mapping's value
     * @param k2 the second mapping's key
     * @param t2 the second mapping's value
     * @return a {@code QHash} containing the specified mappings
     */
    public static <Key,T> @NonNull QHash<Key,T> ofTyped(@Nullable Class<Key> keyType, @Nullable Class<T> valueType, Key k1, T t1, Key k2, T t2) {
    	return ofTyped(
    			QList.findElementMetaType(keyType, k1, k2), 
    			QList.findElementMetaType(valueType, t1, t2), k1, t1, k2, t2);
    }

    /**
     * Returns a QHash containing three mappings.
     *
     * @param <Key> the {@code QHash}'s key type
     * @param <T> the {@code QHash}'s value type
     * @param keyType key type
     * @param valueType value type
     * @param k1 the first mapping's key
     * @param t1 the first mapping's value
     * @param k2 the second mapping's key
     * @param t2 the second mapping's value
     * @param k3 the third mapping's key
     * @param t3 the third mapping's value
     * @return a {@code QHash} containing the specified mappings
     */
    public static <Key,T> @NonNull QHash<Key,T> ofTyped(@Nullable Class<Key> keyType, @Nullable Class<T> valueType, Key k1, T t1, Key k2, T t2, Key k3, T t3) {
    	return ofTyped(
    			QList.findElementMetaType(keyType, k1, k2, k3), 
    			QList.findElementMetaType(valueType, t1, t2, t3), 
    			k1, t1,
    			k2, t2,
    			k3, t3);
    }

    /**
     * Returns a QHash containing four mappings.
     *
     * @param <Key> the {@code QHash}'s key type
     * @param <T> the {@code QHash}'s value type
     * @param keyType key type
     * @param valueType value type
     * @param k1 the first mapping's key
     * @param t1 the first mapping's value
     * @param k2 the second mapping's key
     * @param t2 the second mapping's value
     * @param k3 the third mapping's key
     * @param t3 the third mapping's value
     * @param k4 the fourth mapping's key
     * @param t4 the fourth mapping's value
     * @return a {@code QHash} containing the specified mappings
     */
    public static <Key,T> @NonNull QHash<Key,T> ofTyped(@Nullable Class<Key> keyType, @Nullable Class<T> valueType, Key k1, T t1, Key k2, T t2, Key k3, T t3, Key k4, T t4) {
    	return ofTyped(
    			QList.findElementMetaType(keyType, k1, k2, k3, k4), 
    			QList.findElementMetaType(valueType, t1, t2, t3, t4),
    			k1, t1,
    			k2, t2,
    			k3, t3,
    			k4, t4);
    }

    /**
     * Returns a QHash containing five mappings.
     *
     * @param <Key> the {@code QHash}'s key type
     * @param <T> the {@code QHash}'s value type
     * @param keyType key type
     * @param valueType value type
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
     * @return a {@code QHash} containing the specified mappings
     */
    public static <Key,T> @NonNull QHash<Key,T> ofTyped(@Nullable Class<Key> keyType, @Nullable Class<T> valueType, Key k1, T t1, Key k2, T t2, Key k3, T t3, Key k4, T t4, Key k5, T t5) {
    	return ofTyped(
    			QList.findElementMetaType(keyType, k1, k2, k3, k4, k5), 
    			QList.findElementMetaType(valueType, t1, t2, t3, t4, t5),
    			k1, t1,
    			k2, t2,
    			k3, t3,
    			k4, t4,
    			k5, t5);
    }

    /**
     * Returns a QHash containing six mappings.
     *
     * @param <Key> the {@code QHash}'s key type
     * @param <T> the {@code QHash}'s value type
     * @param keyType key type
     * @param valueType value type
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
     * @return a {@code QHash} containing the specified mappings
     */
    public static <Key,T> @NonNull QHash<Key,T> ofTyped(@Nullable Class<Key> keyType, @Nullable Class<T> valueType, Key k1, T t1, Key k2, T t2, Key k3, T t3, Key k4, T t4, Key k5, T t5, Key k6, T t6) {
    	return ofTyped(
    			QList.findElementMetaType(keyType, k1, k2, k3, k4, k5, k6), 
    			QList.findElementMetaType(valueType, t1, t2, t3, t4, t5, t6),
    			k1, t1,
    			k2, t2,
    			k3, t3,
    			k4, t4,
    			k5, t5,
    			k6, t6);
    }

    /**
     * Returns a QHash containing seven mappings.
     *
     * @param <Key> the {@code QHash}'s key type
     * @param <T> the {@code QHash}'s value type
     * @param keyType key type
     * @param valueType value type
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
     * @return a {@code QHash} containing the specified mappings
     */
    public static <Key,T> @NonNull QHash<Key,T> ofTyped(@Nullable Class<Key> keyType, @Nullable Class<T> valueType, Key k1, T t1, Key k2, T t2, Key k3, T t3, Key k4, T t4, Key k5, T t5,
                               Key k6, T t6, Key k7, T t7) {
    	return ofTyped(
    			QList.findElementMetaType(keyType, k1, k2, k3, k4, k5, k6, k7), 
    			QList.findElementMetaType(valueType, t1, t2, t3, t4, t5, t6, t7),
    			k1, t1,
    			k2, t2,
    			k3, t3,
    			k4, t4,
    			k5, t5,
    			k6, t6,
    			k7, t7);
    }

    /**
     * Returns a QHash containing eight mappings.
     *
     * @param <Key> the {@code QHash}'s key type
     * @param <T> the {@code QHash}'s value type
     * @param keyType key type
     * @param valueType value type
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
     * @return a {@code QHash} containing the specified mappings
     */
    public static <Key,T> @NonNull QHash<Key,T> ofTyped(@Nullable Class<Key> keyType, @Nullable Class<T> valueType, Key k1, T t1, Key k2, T t2, Key k3, T t3, Key k4, T t4, Key k5, T t5,
                               Key k6, T t6, Key k7, T t7, Key k8, T t8) {
    	return ofTyped(
    			QList.findElementMetaType(keyType, k1, k2, k3, k4, k5, k6, k7, k8), 
    			QList.findElementMetaType(valueType, t1, t2, t3, t4, t5, t6, t7, t8),
    			k1, t1,
    			k2, t2,
    			k3, t3,
    			k4, t4,
    			k5, t5,
    			k6, t6,
    			k7, t7,
    			k8, t8);
    }

    /**
     * Returns a QHash containing nine mappings.
     *
     * @param <Key> the {@code QHash}'s key type
     * @param <T> the {@code QHash}'s value type
     * @param keyType key type
     * @param valueType value type
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
     * @return a {@code QHash} containing the specified mappings
     */
    public static <Key,T> @NonNull QHash<Key,T> ofTyped(@Nullable Class<Key> keyType, @Nullable Class<T> valueType, Key k1, T t1, Key k2, T t2, Key k3, T t3, Key k4, T t4, Key k5, T t5,
                               Key k6, T t6, Key k7, T t7, Key k8, T t8, Key k9, T t9) {
    	return ofTyped(
    			QList.findElementMetaType(keyType, k1, k2, k3, k4, k5, k6, k7, k8, k9), 
    			QList.findElementMetaType(valueType, t1, t2, t3, t4, t5, t6, t7, t8, t9),
    			k1, t1, k2, t2,
    			k3, t3,
    			k4, t4,
    			k5, t5,
    			k6, t6,
    			k7, t7,
    			k8, t8,
    			k9, t9);
    }

    /**
     * Returns a QHash containing ten mappings.
     *
     * @param <Key> the {@code QHash}'s key type
     * @param <T> the {@code QHash}'s value type
     * @param keyType key type
     * @param valueType value type
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
     * @return a {@code QHash} containing the specified mappings
     */
    public static <Key,T> @NonNull QHash<Key,T> ofTyped(@Nullable Class<Key> keyType, @Nullable Class<T> valueType, Key k1, T t1, Key k2, T t2, Key k3, T t3, Key k4, T t4, Key k5, T t5,
                               Key k6, T t6, Key k7, T t7, Key k8, T t8, Key k9, T t9, Key k10, T t10) {
    	return ofTyped(
    			QList.findElementMetaType(keyType, k1, k2, k3, k4, k5, k6, k7, k8, k9, k10), 
    			QList.findElementMetaType(valueType, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10),
    			k1, t1,
    			k2, t2,
    			k3, t3,
    			k4, t4,
    			k5, t5,
    			k6, t6,
    			k7, t7,
    			k8, t8,
    			k9, t9,
    			k10, t10);
    }
    
    /**
     * Returns a QHash containing a single mapping.
     *
     * @param <Key> the {@code QHash}'s key type
     * @param <T> the {@code QHash}'s value type
     * @param keyType key type
     * @param valueType value type
     * @param k1 the mapping's key
     * @param t1 the mapping's value
     * @return a {@code QHash} containing the specified mapping
     */
    public static <Key,T> @NonNull QHash<Key,T> ofTyped(@StrictNonNull QMetaType keyType, @StrictNonNull QMetaType valueType, Key k1, T t1) {
		if(keyType.id()==0)
			throw new IllegalArgumentException("QMetaType::UnknownType cannot be key type of QHash.");
		if(keyType.id()==QMetaType.Type.Void.value())
			throw new IllegalArgumentException("void cannot be key type of QHash.");
		if(valueType.id()==0)
			throw new IllegalArgumentException("QMetaType::UnknownType cannot be value type of QHash.");
		if(valueType.id()==QMetaType.Type.Void.value())
			throw new IllegalArgumentException("void cannot be value type of QHash.");
    	return new QHash<>(keyType, valueType, Collections.singletonMap(k1, t1));
    }

    /**
     * Returns a QHash containing two mappings.
     *
     * @param <Key> the {@code QHash}'s key type
     * @param <T> the {@code QHash}'s value type
     * @param keyType key type
     * @param valueType value type
     * @param k1 the first mapping's key
     * @param t1 the first mapping's value
     * @param k2 the second mapping's key
     * @param t2 the second mapping's value
     * @return a {@code QHash} containing the specified mappings
     */
    public static <Key,T> @NonNull QHash<Key,T> ofTyped(@StrictNonNull QMetaType keyType, @StrictNonNull QMetaType valueType, Key k1, T t1, Key k2, T t2) {
    	QHash<Key,T> result = new QHash<>(keyType, valueType);
    	result.insert(k1, t1);
    	result.insert(k2, t2);
        return result;
    }

    /**
     * Returns a QHash containing three mappings.
     *
     * @param <Key> the {@code QHash}'s key type
     * @param <T> the {@code QHash}'s value type
     * @param keyType key type
     * @param valueType value type
     * @param k1 the first mapping's key
     * @param t1 the first mapping's value
     * @param k2 the second mapping's key
     * @param t2 the second mapping's value
     * @param k3 the third mapping's key
     * @param t3 the third mapping's value
     * @return a {@code QHash} containing the specified mappings
     */
    public static <Key,T> @NonNull QHash<Key,T> ofTyped(@StrictNonNull QMetaType keyType, @StrictNonNull QMetaType valueType, Key k1, T t1, Key k2, T t2, Key k3, T t3) {
    	QHash<Key,T> result = new QHash<>(keyType, valueType);
    	result.insert(k1, t1);
    	result.insert(k2, t2);
    	result.insert(k3, t3);
        return result;
    }

    /**
     * Returns a QHash containing four mappings.
     *
     * @param <Key> the {@code QHash}'s key type
     * @param <T> the {@code QHash}'s value type
     * @param keyType key type
     * @param valueType value type
     * @param k1 the first mapping's key
     * @param t1 the first mapping's value
     * @param k2 the second mapping's key
     * @param t2 the second mapping's value
     * @param k3 the third mapping's key
     * @param t3 the third mapping's value
     * @param k4 the fourth mapping's key
     * @param t4 the fourth mapping's value
     * @return a {@code QHash} containing the specified mappings
     */
    public static <Key,T> @NonNull QHash<Key,T> ofTyped(@StrictNonNull QMetaType keyType, @StrictNonNull QMetaType valueType, Key k1, T t1, Key k2, T t2, Key k3, T t3, Key k4, T t4) {
    	QHash<Key,T> result = new QHash<>(keyType, valueType);
    	result.insert(k1, t1);
    	result.insert(k2, t2);
    	result.insert(k3, t3);
    	result.insert(k4, t4);
        return result;
    }

    /**
     * Returns a QHash containing five mappings.
     *
     * @param <Key> the {@code QHash}'s key type
     * @param <T> the {@code QHash}'s value type
     * @param keyType key type
     * @param valueType value type
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
     * @return a {@code QHash} containing the specified mappings
     */
    public static <Key,T> @NonNull QHash<Key,T> ofTyped(@StrictNonNull QMetaType keyType, @StrictNonNull QMetaType valueType, Key k1, T t1, Key k2, T t2, Key k3, T t3, Key k4, T t4, Key k5, T t5) {
    	QHash<Key,T> result = new QHash<>(keyType, valueType);
    	result.insert(k1, t1);
    	result.insert(k2, t2);
    	result.insert(k3, t3);
    	result.insert(k4, t4);
    	result.insert(k5, t5);
        return result;
    }

    /**
     * Returns a QHash containing six mappings.
     *
     * @param <Key> the {@code QHash}'s key type
     * @param <T> the {@code QHash}'s value type
     * @param keyType key type
     * @param valueType value type
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
     * @return a {@code QHash} containing the specified mappings
     */
    public static <Key,T> @NonNull QHash<Key,T> ofTyped(@StrictNonNull QMetaType keyType, @StrictNonNull QMetaType valueType, Key k1, T t1, Key k2, T t2, Key k3, T t3, Key k4, T t4, Key k5, T t5, Key k6, T t6) {
    	QHash<Key,T> result = new QHash<>(keyType, valueType);
    	result.insert(k1, t1);
    	result.insert(k2, t2);
    	result.insert(k3, t3);
    	result.insert(k4, t4);
    	result.insert(k5, t5);
    	result.insert(k6, t6);
        return result;
    }

    /**
     * Returns a QHash containing seven mappings.
     *
     * @param <Key> the {@code QHash}'s key type
     * @param <T> the {@code QHash}'s value type
     * @param keyType key type
     * @param valueType value type
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
     * @return a {@code QHash} containing the specified mappings
     */
    public static <Key,T> @NonNull QHash<Key,T> ofTyped(@StrictNonNull QMetaType keyType, @StrictNonNull QMetaType valueType, Key k1, T t1, Key k2, T t2, Key k3, T t3, Key k4, T t4, Key k5, T t5,
                               Key k6, T t6, Key k7, T t7) {
    	QHash<Key,T> result = new QHash<>(keyType, valueType);
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
     * Returns a QHash containing eight mappings.
     *
     * @param <Key> the {@code QHash}'s key type
     * @param <T> the {@code QHash}'s value type
     * @param keyType key type
     * @param valueType value type
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
     * @return a {@code QHash} containing the specified mappings
     */
    public static <Key,T> @NonNull QHash<Key,T> ofTyped(@StrictNonNull QMetaType keyType, @StrictNonNull QMetaType valueType, Key k1, T t1, Key k2, T t2, Key k3, T t3, Key k4, T t4, Key k5, T t5,
                               Key k6, T t6, Key k7, T t7, Key k8, T t8) {
    	QHash<Key,T> result = new QHash<>(keyType, valueType);
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
     * Returns a QHash containing nine mappings.
     *
     * @param <Key> the {@code QHash}'s key type
     * @param <T> the {@code QHash}'s value type
     * @param keyType key type
     * @param valueType value type
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
     * @return a {@code QHash} containing the specified mappings
     */
    public static <Key,T> @NonNull QHash<Key,T> ofTyped(@StrictNonNull QMetaType keyType, @StrictNonNull QMetaType valueType, Key k1, T t1, Key k2, T t2, Key k3, T t3, Key k4, T t4, Key k5, T t5,
                               Key k6, T t6, Key k7, T t7, Key k8, T t8, Key k9, T t9) {
    	QHash<Key,T> result = new QHash<>(keyType, valueType);
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
     * Returns a QHash containing ten mappings.
     *
     * @param <Key> the {@code QHash}'s key type
     * @param <T> the {@code QHash}'s value type
     * @param keyType key type
     * @param valueType value type
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
     * @return a {@code QHash} containing the specified mappings
     */
    public static <Key,T> @NonNull QHash<Key,T> ofTyped(@StrictNonNull QMetaType keyType, @StrictNonNull QMetaType valueType, Key k1, T t1, Key k2, T t2, Key k3, T t3, Key k4, T t4, Key k5, T t5,
                               Key k6, T t6, Key k7, T t7, Key k8, T t8, Key k9, T t9, Key k10, T t10) {
    	QHash<Key,T> result = new QHash<>(keyType, valueType);
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
     * Returns a QHash containing keys and values extracted from the given entries.
     *
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
     * @param <Key> the {@code QHash}'s key type
     * @param <T> the {@code QHash}'s value type
     * @param entries {@code java.util.Map.Entry}s containing the keys and values from which the map is populated
     * @return a {@code QHash} containing the specified mappings
     * @throws NullPointerException if any entry, key, or value is {@code null}, or if
     *         the {@code entries} array is {@code null}
     */
    @SafeVarargs
    public static <Key,T> @NonNull QHash<Key,T> ofEntries(Map.@StrictNonNull Entry<? extends Key, ? extends T> entry0, Map.@StrictNonNull Entry<? extends Key, ? extends T> @StrictNonNull... entries) {
    	QPair<QMetaType, QMetaType> metaTypes = findMapMetaType(entry0, entries);
    	QHash<Key,T> result = new QHash<>(metaTypes.first, metaTypes.second);
    	result.insert(entry0.getKey(), entry0.getValue());
    	for (Map.Entry<? extends Key, ? extends T> entry : entries) {
        	result.insert(entry.getKey(), entry.getValue());
		}
        return result;
    }

    /**
     * Returns a QHash containing keys and values extracted from the given pairs.
     *
     * It is convenient to create the pair using the {@link QPair#pair(Object, Object)} method.
     * For example,
     *
     * <pre>{@code
     *     import static java.util.QPair.pair;
     *
     *     QHash<Integer,String> map = QHash.ofPairs(
     *         pair(1, "a"),
     *         pair(2, "b"),
     *         pair(3, "c"),
     *         ...
     *         pair(26, "z"));
     * }</pre>
     *
     * @param <Key> the {@code QHash}'s key type
     * @param <T> the {@code QHash}'s value type
     * @param pairs {@code QPair}s containing the keys and values from which the map is populated
     * @return a {@code QHash} containing the specified mappings
     * @throws NullPointerException if any entry, key, or value is {@code null}, or if
     *         the {@code entries} array is {@code null}
     */
    @SafeVarargs
    public static <Key,T> @NonNull QHash<Key,T> ofPairs(@StrictNonNull QPair<? extends Key, ? extends T> pair0, @StrictNonNull QPair<? extends Key, ? extends T> @StrictNonNull... pairs) {
    	QPair<QMetaType, QMetaType> metaTypes = findMapMetaType(pair0, pairs);
    	QHash<Key,T> result = new QHash<>(metaTypes.first, metaTypes.second);
    	result.insert(pair0.first, pair0.second);
    	for (QPair<? extends Key, ? extends T> entry : pairs) {
        	result.insert(entry.first, entry.second);
		}
        return result;
    }
    
    /**
     * Returns a QHash containing keys and values extracted from the given pairs.
     *
     * It is convenient to create the pair using the {@link QPair#pair(Object, Object)} method.
     * For example,
     *
     * <pre>{@code
     *     import static java.util.QPair.pair;
     *
     *     QHash<Integer,String> map = QHash.ofTypedPairs(
     *         int.class,
     *         String.class,
     *         pair(1, "a"),
     *         pair(2, "b"),
     *         pair(3, "c"),
     *         ...
     *         pair(26, "z"));
     * }</pre>
     *
     * @param <Key> the {@code QHash}'s key type
     * @param <T> the {@code QHash}'s value type
     * @param pairs {@code QPair}s containing the keys and values from which the map is populated
     * @return a {@code QHash} containing the specified mappings
     * @throws NullPointerException if any pair, key, or value is {@code null}, or if
     *         the {@code pairs} array is {@code null}
     */
    @SafeVarargs
    public static <Key,T> @NonNull QHash<Key,T> ofTypedPairs(@Nullable Class<Key> keyType, @Nullable Class<T> valueType, QPair<? extends Key, ? extends T>... pairs) {
    	QPair<QMetaType, QMetaType> metaTypes = findMapMetaType(keyType, valueType, pairs);
        return ofTypedPairs(metaTypes.first, metaTypes.second, pairs);
    }
    


    /**
     * Returns a QHash containing keys and values extracted from the given pairs.
     *
     * It is convenient to create the pair using the {@link QPair#pair(Object, Object)} method.
     * For example,
     *
     * <pre>{@code
     *     import static java.util.QPair.pair;
     *
     *     QHash<Integer,String> map = QHash.ofTypedPairs(
     *         QMetaType.fromType(int.class),
     *         QMetaType.fromType(String.class),
     *         pair(1, "a"),
     *         pair(2, "b"),
     *         pair(3, "c"),
     *         ...
     *         pair(26, "z"));
     * }</pre>
     *
     * @param <Key> the {@code QHash}'s key type
     * @param <T> the {@code QHash}'s value type
     * @param pairs {@code QPair}s containing the keys and values from which the map is populated
     * @return a {@code QHash} containing the specified mappings
     * @throws NullPointerException if any pair, key, or value is {@code null}, or if
     *         the {@code pairs} array is {@code null}
     */
    @SafeVarargs
    public static <Key,T> @NonNull QHash<Key,T> ofTypedPairs(@StrictNonNull QMetaType keyType, @StrictNonNull QMetaType valueType, QPair<? extends Key, ? extends T>... pairs) {
    	QHash<Key,T> result = new QHash<>(keyType, valueType);
    	for (QPair<? extends Key, ? extends T> entry : pairs) {
        	result.insert(entry.first, entry.second);
		}
        return result;
    }

    /**
     * Returns a QHash containing keys and values extracted from the given entries.
     *
     * It is convenient to create the map entries using the {@link Map#entry Map.entry()} method.
     * For example,
     *
     * <pre>{@code
     *     import static java.util.Map.entry;
     *
     *     QHash<Integer,String> map = QHash.ofTypedEntries(
     *         int.class,
     *         String.class,
     *         entry(1, "a"),
     *         entry(2, "b"),
     *         entry(3, "c"),
     *         ...
     *         entry(26, "z"));
     * }</pre>
     *
     * @param <Key> the {@code QHash}'s key type
     * @param <T> the {@code QHash}'s value type
     * @param keyType key type
     * @param valueType value type
     * @param entries {@code java.util.Map.Entry}s containing the keys and values from which the map is populated
     * @return a {@code QHash} containing the specified mappings
     * @throws NullPointerException if any entry, key, or value is {@code null}, or if
     *         the {@code entries} array is {@code null}
     */
    @SafeVarargs
    public static <Key,T> @NonNull QHash<Key,T> ofTypedEntries(@Nullable Class<Key> keyType, @Nullable Class<T> valueType, Map.Entry<? extends Key, ? extends T>... entries) {
    	QPair<QMetaType, QMetaType> metaTypes = findMapMetaType(keyType, valueType, entries);
        return ofTypedEntries(metaTypes.first, metaTypes.second, entries);
    }

    /**
     * Returns a QHash containing keys and values extracted from the given entries.
     *
     * It is convenient to create the map entries using the {@link Map#entry Map.entry()} method.
     * For example,
     *
     * <pre>{@code
     *     import static java.util.Map.entry;
     *
     *     QHash<Integer,String> map = QHash.ofTypedEntries(
     *         QMetaType.fromType(int.class),
     *         QMetaType.fromType(String.class),
     *         entry(1, "a"),
     *         entry(2, "b"),
     *         entry(3, "c"),
     *         ...
     *         entry(26, "z"));
     * }</pre>
     *
     * @param <Key> the {@code QHash}'s key type
     * @param <T> the {@code QHash}'s value type
     * @param keyType key type
     * @param valueType value type
     * @param entries {@code java.util.Map.Entry}s containing the keys and values from which the map is populated
     * @return a {@code QHash} containing the specified mappings
     * @throws NullPointerException if any entry, key, or value is {@code null}, or if
     *         the {@code entries} array is {@code null}
     */
    @SafeVarargs
    public static <Key,T> @NonNull QHash<Key,T> ofTypedEntries(@StrictNonNull QMetaType keyType, @StrictNonNull QMetaType valueType, Map.Entry<? extends Key, ? extends T>... entries) {
    	QHash<Key,T> result = new QHash<>(keyType, valueType);
    	for (Map.Entry<? extends Key, ? extends T> entry : entries) {
        	result.insert(entry.getKey(), entry.getValue());
		}
        return result;
    }
    
    /**
     * <p>See <code>QHash::<wbr>operator=(QHash&lt;Key,T>)</code></p>
     */
    @QtUninvokable
    public final void assign(@StrictNonNull QHash<Key,T> other) {
		assign(QtJambi_LibraryUtilities.internal.nativeId(this), other, QtJambi_LibraryUtilities.internal.nativeId(other));
    }
    
    @QtUninvokable
    private native void assign(long __this__nativeId, Object otherObj, long other);
	
	/**
     * <p>See <code>QHash::<wbr>swap(QHash&lt;Key,T>&amp;)</code></p>
     */
    @QtUninvokable
    public final void swap(@StrictNonNull QHash<Key,T> other) {
    	swap(QtJambi_LibraryUtilities.internal.nativeId(this), other, QtJambi_LibraryUtilities.internal.nativeId(other));
    }
    
    @QtUninvokable
    private native void swap(long __this__nativeId, Object otherObj, long other);
    
    /**
     * Returns true of both containers share the same data. 
     */
    @QtUninvokable
    public final boolean isSharedWith(@StrictNonNull QHash<?,?> other) {
		return isSharedWith(QtJambi_LibraryUtilities.internal.nativeId(this), QtJambi_LibraryUtilities.internal.nativeId(other));
	}
	@QtUninvokable
    private native boolean isSharedWith(long __this__nativeId, long other);

	/**
     * Returns true if container is not shared. 
     */
    @QtUninvokable
    public final boolean isDetached() {
		return isDetached(QtJambi_LibraryUtilities.internal.nativeId(this));
	}
	@QtUninvokable
    private native boolean isDetached(long __this__nativeId);
	
	/**
     * Detached the container if it is shared.
     */
    @QtUninvokable
    public final void detach() {
    	detach(QtJambi_LibraryUtilities.internal.nativeId(this));
	}
	@QtUninvokable
    private native boolean detach(long __this__nativeId);
}
