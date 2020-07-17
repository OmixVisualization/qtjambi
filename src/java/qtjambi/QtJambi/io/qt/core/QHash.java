/****************************************************************************
**
** Copyright (C) 2009-2020 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import io.qt.QtUninvokable;
import io.qt.internal.*;


public class QHash<K,V> extends io.qt.internal.QtJambiHashObject<K,V>
{
	static {
    	io.qt.core.QtJambi_LibraryInitializer.init();
    }

	private final long beginFunction;
    private final long capacityFunction;
	private final long clearFunction;
	private final long containsFunction;
	private final long countObjectFunction;
	private final long endFunction;
	private final long findFunction;
	private final long insertFunction;
	private final long keyFunction;
	private final long keysFunction;
	private final long keysForValueFunction;
	private final long equalFunction;
	private final long removeAllFunction;
	private final long reserveFunction;
	private final long sizeFunction;
	private final long takeFunction;
	private final long uniqueKeysFunction;
	private final long uniteFunction;
	private final long valueFunction;
	private final long valuesFunction;
	private final long valuesKeyFunction;
	
    private QHash(Class<K> keyType, Class<V> valueType, long beginFunction, long capacityFunction, long clearFunction, long containsFunction,
			long countObjectFunction, long endFunction, long findFunction, long insertFunction, long keyFunction, long keysFunction, long keysForValueFunction,
			long equalFunction, long removeAllFunction, long reserveFunction, long sizeFunction, long takeFunction, long uniqueKeysFunction, long uniteFunction,
			long valueFunction, long valuesFunction, long valuesKeyFunction) {
		super(null, keyType, valueType);
		this.beginFunction = beginFunction;
		this.capacityFunction = capacityFunction;
		this.clearFunction = clearFunction;
		this.containsFunction = containsFunction;
		this.countObjectFunction = countObjectFunction;
		this.endFunction = endFunction;
		this.findFunction = findFunction;
		this.insertFunction = insertFunction;
		this.keyFunction = keyFunction;
		this.keysFunction = keysFunction;
		this.keysForValueFunction = keysForValueFunction;
		this.equalFunction = equalFunction;
		this.reserveFunction = reserveFunction;
		this.removeAllFunction = removeAllFunction;
		this.sizeFunction = sizeFunction;
		this.takeFunction = takeFunction;
		this.uniqueKeysFunction = uniqueKeysFunction;
		this.uniteFunction = uniteFunction;
		this.valueFunction = valueFunction;
		this.valuesFunction = valuesFunction;
		this.valuesKeyFunction = valuesKeyFunction;
	}
	
    private QHash(Class<K> keyType, Class<V> valueType, long beginFunction, long capacityFunction, long containsFunction,
			long countObjectFunction, long endFunction, long findFunction, long keyFunction, long keysFunction, long keysForValueFunction,
			long equalFunction, long sizeFunction, long uniqueKeysFunction, 
			long valueFunction, long valuesFunction, long valuesKeyFunction) {
    	this(keyType, valueType, beginFunction, capacityFunction, 0, containsFunction, countObjectFunction, 
    			endFunction, findFunction, 0, keyFunction, keysFunction, keysForValueFunction, 
    			equalFunction, 0, 0, sizeFunction, 0, uniqueKeysFunction, sizeFunction, 
    			valueFunction, valuesFunction, valuesKeyFunction);
	}
    
	@QtUninvokable
    protected final io.qt.core.QMapIterator<K,V> begin()    {
        if(beginFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QHash_begin(io.qt.internal.QtJambiInternal.checkedNativeId(this), beginFunction);
    }
    @QtUninvokable
    static native <K,V> io.qt.core.QMapIterator<K,V> __qt_QHash_begin(long __this__nativeId, long beginFunction);
    
    @QtUninvokable
    public final int capacity()    {
        if(capacityFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QHash_capacity(io.qt.internal.QtJambiInternal.checkedNativeId(this), capacityFunction);
    }
    @QtUninvokable
    static native <T> int __qt_QHash_capacity(long __this__nativeId, long capacityFunction);

    @QtUninvokable
    public final void clear()    {
        if(clearFunction==0)
        	throw new UnsupportedOperationException();
        __qt_QHash_clear(io.qt.internal.QtJambiInternal.checkedNativeId(this), clearFunction);
    }
    @QtUninvokable
    static native void __qt_QHash_clear(long __this__nativeId, long clearFunction);

    @QtUninvokable
    public final boolean contains(K key)    {
        if(containsFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QHash_contains(io.qt.internal.QtJambiInternal.checkedNativeId(this), key, containsFunction);
    }
    @QtUninvokable
    static native <K> boolean __qt_QHash_contains(long __this__nativeId, K key, long containsFunction);

    @QtUninvokable
    public final int count()    {
        return size();
    }

    @QtUninvokable
    public final int count(K key)    {
        if(countObjectFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QHash_count(io.qt.internal.QtJambiInternal.checkedNativeId(this), key, countObjectFunction);
    }
    @QtUninvokable
    static native <K> int __qt_QHash_count(long __this__nativeId, K key, long countObjectFunction);

    @QtUninvokable
    public final boolean empty()    {
        return this.size()==0;
    }

    @QtUninvokable
    protected final io.qt.core.QMapIterator<K,V> end()    {
        if(endFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QHash_end(io.qt.internal.QtJambiInternal.checkedNativeId(this), endFunction);
    }
    @QtUninvokable
    static native <K,V> io.qt.core.QMapIterator<K,V> __qt_QHash_end(long __this__nativeId, long endFunction);

    @QtUninvokable
    public final io.qt.core.QMapIterator<K,V> find(K key)    {
        if(findFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QHash_find(io.qt.internal.QtJambiInternal.checkedNativeId(this), key, findFunction);
    }
    @QtUninvokable
    static native <K,V> io.qt.core.QMapIterator<K,V> __qt_QHash_find(long __this__nativeId, K key, long findFunction);

    @QtUninvokable
    public final void insert(K key, V value)    {
        if(insertFunction==0)
        	throw new UnsupportedOperationException();
        __qt_QHash_insert(io.qt.internal.QtJambiInternal.checkedNativeId(this), key, value, insertFunction);
    }
    @QtUninvokable
    static native <K,V> void __qt_QHash_insert(long __this__nativeId, K key, V value, long insertFunction);

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
        if(keyFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QHash_key(io.qt.internal.QtJambiInternal.checkedNativeId(this), value, defaultKey, keyFunction);
    }
    @QtUninvokable
    static native <K,V> K __qt_QHash_key(long __this__nativeId, V value, K defaultKey, long keyFunction);

    @QtUninvokable
    public final java.util.List<K> keys()    {
        if(keysFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QHash_keys(io.qt.internal.QtJambiInternal.checkedNativeId(this), keysFunction);
    }
    @QtUninvokable
    static native <K> java.util.List<K> __qt_QHash_keys(long __this__nativeId, long keysFunction);

    @QtUninvokable
    public final java.util.List<K> keys(V value)    {
        if(keysForValueFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QHash_keysForValue(io.qt.internal.QtJambiInternal.checkedNativeId(this), value, keysForValueFunction);
    }
    @QtUninvokable
    static native <K,V> java.util.List<K> __qt_QHash_keysForValue(long __this__nativeId, V value, long keysForValueFunction);

    @QtUninvokable
    private final boolean operator_equal(java.util.Map<K,V> other)    {
        if(equalFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QHash_operator_equal(io.qt.internal.QtJambiInternal.checkedNativeId(this), other, equalFunction);
    }
    @QtUninvokable
    static native <K,V> boolean __qt_QHash_operator_equal(long __this__nativeId, java.util.Map<K,V> other, long equalFunction);

    @QtUninvokable
    public final int removeAll(Object key)    {
        if(removeAllFunction==0)
        	throw new UnsupportedOperationException();
        if(checkKey(key))
        	return __qt_QHash_remove(io.qt.internal.QtJambiInternal.checkedNativeId(this), key, removeAllFunction);
        else return 0;
    }
    @QtUninvokable
    static native <K,V> int __qt_QHash_remove(long __this__nativeId, K key, long removeAllFunction);
    
    @QtUninvokable
    public final void reserve(int size)    {
        if(reserveFunction==0)
        	throw new UnsupportedOperationException();
        __qt_QHash_reserve(io.qt.internal.QtJambiInternal.checkedNativeId(this), size, reserveFunction);
    }
    @QtUninvokable
    static native <T> void __qt_QHash_reserve(long __this__nativeId, int size, long reserveFunction);

    @QtUninvokable
    public final int size()    {
        if(sizeFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QHash_size(io.qt.internal.QtJambiInternal.checkedNativeId(this), sizeFunction);
    }
    @QtUninvokable
    static native int __qt_QHash_size(long __this__nativeId, long sizeFunction);

    @QtUninvokable
    public final V take(K key)    {
        if(takeFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QHash_take(io.qt.internal.QtJambiInternal.checkedNativeId(this), key, takeFunction);
    }
    @QtUninvokable
    static native <K,V> V __qt_QHash_take(long __this__nativeId, K key, long takeFunction);

    @QtUninvokable
    @Deprecated
    public final java.util.List<K> uniqueKeys()    {
        if(uniqueKeysFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QHash_uniqueKeys(io.qt.internal.QtJambiInternal.checkedNativeId(this), uniqueKeysFunction);
    }
    @QtUninvokable
    static native <K> java.util.List<K> __qt_QHash_uniqueKeys(long __this__nativeId, long uniqueKeysFunction);

    @QtUninvokable
    @Deprecated
    public final void unite(java.util.Map<? super K,? super V> other)    {
        if(uniteFunction==0)
        	throw new UnsupportedOperationException();
        __qt_QHash_unite(io.qt.internal.QtJambiInternal.checkedNativeId(this), other, uniteFunction);
    }
    @QtUninvokable
    static native void __qt_QHash_unite(long __this__nativeId, Object other, long uniteFunction);

    @QtUninvokable
    public final V value(K key) {
        return value(key, null);
    }
    @QtUninvokable
    public final V value(K key, V defaultValue)    {
        if(valueFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QHash_value(io.qt.internal.QtJambiInternal.checkedNativeId(this), key, defaultValue, valueFunction);
    }
    @QtUninvokable
    static native <K,V> V __qt_QHash_value(long __this__nativeId, K key, V defaultValue, long valueFunction);

    @QtUninvokable
    public final java.util.List<V> values()    {
        if(valuesFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QHash_values(io.qt.internal.QtJambiInternal.checkedNativeId(this), valuesFunction);
    }
    @QtUninvokable
    static native <V> java.util.List<V> __qt_QHash_values(long __this__nativeId, long valuesFunction);

    @QtUninvokable
    @Deprecated
    public final java.util.List<V> values(K key)    {
        if(valuesKeyFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QHash_values(io.qt.internal.QtJambiInternal.checkedNativeId(this), key, valuesKeyFunction);
    }
    @QtUninvokable
    static native <K,V> java.util.List<V> __qt_QHash_values(long __this__nativeId, K key, long valuesKeyFunction);

    @SuppressWarnings("unchecked")
	@Override
    @QtUninvokable
    public boolean equals(Object other) {
        if (other instanceof java.util.Map) {
            return operator_equal((java.util.Map<K,V>) other);
        }
        return false;
    }

    @Override
    @QtUninvokable
    public int hashCode() { 
    	int hashCode = getClass().hashCode();
        for (QPair<K,V> e : this)
            hashCode = 31*hashCode + (e==null ? 0 : e.hashCode());
        return hashCode;
    }
    
    @QtUninvokable
    static native <K> boolean lessThan(K key1, K key2);

    @Override
    @QtUninvokable
    public boolean containsKey(Object key){
        return checkKey(key) ? contains(castKey(key)) : false;
    }

    @Override
    @QtUninvokable
    public V get(Object key){
        return checkKey(key) ? value(castKey(key)) : null;
    }

    @Override
    @QtUninvokable
    public V put(K key, V value){
        V old = value(key);
        insert(key, value);
        return old;
    }

    @Override
    @QtUninvokable
    public V remove(Object key){
        if(checkKey(key))
            return take(castKey(key));
        else return null;
    }

    @Override
    @QtUninvokable
    public boolean containsValue(Object value){
        return checkValue(value) ? keys(castValue(value)).isEmpty() : false;
    }

}
