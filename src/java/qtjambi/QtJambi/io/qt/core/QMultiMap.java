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

import static io.qt.core.QMap.*;

import java.util.Collections;
import java.util.List;

import io.qt.QtUninvokable;
import io.qt.internal.*;


public class QMultiMap<K,V> extends io.qt.internal.QtJambiMultiMapObject<K,V>
{
	static {
    	io.qt.core.QtJambi_LibraryInitializer.init();
    }

	private final long beginFunction;
	private final long clearFunction;
	private final long containsFunction;
	private final long countObjectFunction;
	private final long endFunction;
	private final long findFunction;
	private final long firstFunction;
	private final long firstKeyFunction;
	private final long insertFunction;
	private final long keyFunction;
	private final long keysFunction;
	private final long keysForValueFunction;
	private final long lastFunction;
	private final long lastKeyFunction;
	private final long lowerBoundFunction;
	private final long equalFunction;
	private final long removeAllFunction;
	private final long sizeFunction;
	private final long takeFunction;
	private final long uniqueKeysFunction;
	private final long uniteFunction;
	private final long upperBoundFunction;
	private final long valueFunction;
	private final long valuesFunction;
	private final long valuesKeyFunction;
	private final long containsPairFunction;
	private final long countPairFunction;
	private final long findPairFunction;
	private final long removeAllPairFunction;
	private final long replaceOneFunction;
	
    private QMultiMap(Class<K> keyType, Class<V> valueType, long beginFunction, long clearFunction, long containsFunction,
			long countObjectFunction, long endFunction, long findFunction, long firstFunction,
			long firstKeyFunction, long insertFunction, long keyFunction, long keysFunction, long keysForValueFunction,
			long lastFunction, long lastKeyFunction, long lowerBoundFunction, long equalFunction,
			long removeAllFunction, long sizeFunction, long takeFunction, long uniqueKeysFunction, long uniteFunction,
			long upperBoundFunction, long valueFunction, long valuesFunction, long valuesKeyFunction,
			long containsPairFunction, long countPairFunction, long findPairFunction, long removeAllPairFunction,
			long replaceOneFunction) {
		super(null, keyType, valueType);
		this.beginFunction = beginFunction;
		this.clearFunction = clearFunction;
		this.containsFunction = containsFunction;
		this.countObjectFunction = countObjectFunction;
		this.endFunction = endFunction;
		this.findFunction = findFunction;
		this.firstFunction = firstFunction;
		this.firstKeyFunction = firstKeyFunction;
		this.insertFunction = insertFunction;
		this.keyFunction = keyFunction;
		this.keysFunction = keysFunction;
		this.keysForValueFunction = keysForValueFunction;
		this.lastFunction = lastFunction;
		this.lastKeyFunction = lastKeyFunction;
		this.lowerBoundFunction = lowerBoundFunction;
		this.equalFunction = equalFunction;
		this.removeAllFunction = removeAllFunction;
		this.sizeFunction = sizeFunction;
		this.takeFunction = takeFunction;
		this.uniqueKeysFunction = uniqueKeysFunction;
		this.uniteFunction = uniteFunction;
		this.upperBoundFunction = upperBoundFunction;
		this.valueFunction = valueFunction;
		this.valuesFunction = valuesFunction;
		this.valuesKeyFunction = valuesKeyFunction;
		this.containsPairFunction = containsPairFunction;
		this.countPairFunction = countPairFunction;
		this.findPairFunction = findPairFunction;
		this.removeAllPairFunction = removeAllPairFunction;
		this.replaceOneFunction = replaceOneFunction;
	}
	
    private QMultiMap(Class<K> keyType, Class<V> valueType, long beginFunction, long containsFunction,
			long countObjectFunction, long endFunction, long findFunction, long firstFunction,
			long firstKeyFunction, long keyFunction, long keysFunction, long keysForValueFunction,
			long lastFunction, long lastKeyFunction, long lowerBoundFunction, long equalFunction,
			long sizeFunction, long uniqueKeysFunction,  long upperBoundFunction, 
			long valueFunction, long valuesFunction, long valuesKeyFunction,
			long containsPairFunction, long countPairFunction, long findPairFunction) {
    	this(keyType, valueType, beginFunction, 0, containsFunction, countObjectFunction, 
    			endFunction, findFunction, firstFunction, firstKeyFunction, 
    			0, keyFunction, keysFunction, keysForValueFunction, 
    			lastFunction, lastKeyFunction, lowerBoundFunction, 
    			equalFunction, 0, sizeFunction, 0, uniqueKeysFunction, sizeFunction, 
    			upperBoundFunction, valueFunction, valuesFunction, valuesKeyFunction,
    			containsPairFunction, countPairFunction, findPairFunction, 0, 0);
	}
    
	@QtUninvokable
    protected final io.qt.core.QMapIterator<K,V> begin()    {
        if(beginFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QMap_begin(io.qt.internal.QtJambiInternal.checkedNativeId(this), beginFunction);
    }

    @QtUninvokable
    public final void clear()    {
        if(clearFunction==0)
        	throw new UnsupportedOperationException();
        __qt_QMap_clear(io.qt.internal.QtJambiInternal.checkedNativeId(this), clearFunction);
    }

    @QtUninvokable
    public final boolean contains(K key)    {
        if(containsFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QMap_contains(io.qt.internal.QtJambiInternal.checkedNativeId(this), key, containsFunction);
    }

    @QtUninvokable
    public final int count()    {
        return size();
    }

    @QtUninvokable
    public final int count(K key)    {
        if(countObjectFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QMap_count(io.qt.internal.QtJambiInternal.checkedNativeId(this), key, countObjectFunction);
    }

    @QtUninvokable
    public final boolean empty()    {
        return this.size()==0;
    }

    @QtUninvokable
    protected final io.qt.core.QMapIterator<K,V> end()    {
        if(endFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QMap_end(io.qt.internal.QtJambiInternal.checkedNativeId(this), endFunction);
    }

    @QtUninvokable
    public final io.qt.core.QMapIterator<K,V> find(K key)    {
        if(findFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QMap_find(io.qt.internal.QtJambiInternal.checkedNativeId(this), key, findFunction);
    }

    @QtUninvokable
    public final V first()    {
        if(firstFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QMap_first(io.qt.internal.QtJambiInternal.checkedNativeId(this), firstFunction);
    }

    @QtUninvokable
    public final K firstKey()    {
        if(firstKeyFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QMap_firstKey(io.qt.internal.QtJambiInternal.checkedNativeId(this), firstKeyFunction);
    }

    @QtUninvokable
    public final void insert(K key, V value)    {
        if(insertFunction==0)
        	throw new UnsupportedOperationException();
        __qt_QMap_insert(io.qt.internal.QtJambiInternal.checkedNativeId(this), key, value, insertFunction);
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
        if(keyFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QMap_key(io.qt.internal.QtJambiInternal.checkedNativeId(this), value, defaultKey, keyFunction);
    }

    @QtUninvokable
    public final java.util.List<K> keys()    {
        if(keysFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QMap_keys(io.qt.internal.QtJambiInternal.checkedNativeId(this), keysFunction);
    }

    @QtUninvokable
    public final java.util.List<K> keys(V value)    {
        if(keysForValueFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QMap_keysForValue(io.qt.internal.QtJambiInternal.checkedNativeId(this), value, keysForValueFunction);
    }

    @QtUninvokable
    public final V last()    {
        if(lastFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QMap_last(io.qt.internal.QtJambiInternal.checkedNativeId(this), lastFunction);
    }

    @QtUninvokable
    public final K lastKey()    {
        if(lastKeyFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QMap_lastKey(io.qt.internal.QtJambiInternal.checkedNativeId(this), lastKeyFunction);
    }

    @QtUninvokable
    public final io.qt.core.QMapIterator<K,V> lowerBound(K key)    {
        if(lowerBoundFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QMap_lowerBound(io.qt.internal.QtJambiInternal.checkedNativeId(this), key, lowerBoundFunction);
    }

    @QtUninvokable
    private final boolean operator_equal(java.util.Map<K,V> other)    {
        if(equalFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QMap_operator_equal(io.qt.internal.QtJambiInternal.checkedNativeId(this), other, equalFunction);
    }

    @QtUninvokable
    public final int removeAll(Object key)    {
        if(removeAllFunction==0)
        	throw new UnsupportedOperationException();
        if(checkKey(key))
        	return __qt_QMap_remove(io.qt.internal.QtJambiInternal.checkedNativeId(this), key, removeAllFunction);
        else return 0;
    }

    @QtUninvokable
    public final int size()    {
        if(sizeFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QMap_size(io.qt.internal.QtJambiInternal.checkedNativeId(this), sizeFunction);
    }

    @QtUninvokable
    public final V take(K key)    {
        if(takeFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QMap_take(io.qt.internal.QtJambiInternal.checkedNativeId(this), key, takeFunction);
    }

    @QtUninvokable
    public final java.util.List<K> uniqueKeys()    {
        if(uniqueKeysFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QMap_uniqueKeys(io.qt.internal.QtJambiInternal.checkedNativeId(this), uniqueKeysFunction);
    }

    @QtUninvokable
    public final void unite(java.util.Map<? super K,? extends java.util.Collection<? super V>> other)    {
        if(uniteFunction==0)
        	throw new UnsupportedOperationException();
        __qt_QMap_unite(io.qt.internal.QtJambiInternal.checkedNativeId(this), other, uniteFunction);
    }

    @QtUninvokable
    public final io.qt.core.QMapIterator<K,V> upperBound(K key)    {
        if(upperBoundFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QMap_upperBound(io.qt.internal.QtJambiInternal.checkedNativeId(this), key, upperBoundFunction);
    }


    @QtUninvokable
    public final V value(K key) {
        return value(key, null);
    }
    
    @QtUninvokable
    public final V value(K key, V defaultValue)    {
        if(valueFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QMap_value(io.qt.internal.QtJambiInternal.checkedNativeId(this), key, defaultValue, valueFunction);
    }

    @Override
    @QtUninvokable
    public java.util.Collection<java.util.List<V>> values() {
        java.util.List<java.util.List<V>> result = new java.util.ArrayList<>();
        for(K key : keys()) {
            result.add(values(key));
        }
        return result;
    }

    @QtUninvokable
    public final java.util.List<V> values(K key)    {
        if(valuesKeyFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QMap_valuesKey(io.qt.internal.QtJambiInternal.checkedNativeId(this), key, valuesKeyFunction);
    }
    
    @QtUninvokable
    public final boolean contains(K key, V value)    {
        if(containsPairFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QMultiMap_contains(io.qt.internal.QtJambiInternal.checkedNativeId(this), key, value, containsPairFunction);
    }
    @QtUninvokable
    private static native <K,V> boolean __qt_QMultiMap_contains(long __this__nativeId, K key, V value, long containsPairFunction);
    
    @QtUninvokable
    public final int count(K key, V value)    {
        if(countPairFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QMultiMap_count(io.qt.internal.QtJambiInternal.checkedNativeId(this), key, value, countPairFunction);
    }
    @QtUninvokable
    private static native <K,V> int __qt_QMultiMap_count(long __this__nativeId, K key, V value, long countPairFunction);

    @QtUninvokable
    public final io.qt.core.QMapIterator<K,V> find(K key, V value)    {
        if(findPairFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QMultiMap_find(io.qt.internal.QtJambiInternal.checkedNativeId(this), key, findPairFunction);
    }
    @QtUninvokable
    static native <K,V> io.qt.core.QMapIterator<K,V> __qt_QMultiMap_find(long __this__nativeId, K key, long findPairFunction);
    
    @QtUninvokable
    public final int removeAll(K key, V value)    {
        if(removeAllPairFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QMultiMap_remove(io.qt.internal.QtJambiInternal.checkedNativeId(this), key, value, removeAllPairFunction);
    }
    @QtUninvokable
    static native <K,V> int __qt_QMultiMap_remove(long __this__nativeId, K key, V value, long removeAllPairFunction);
    
    @QtUninvokable
    public final void replaceOne(K key, V value)    {
        if(replaceOneFunction==0)
        	throw new UnsupportedOperationException();
        __qt_QMultiMap_replace(io.qt.internal.QtJambiInternal.checkedNativeId(this), key, value, replaceOneFunction);
    }
    @QtUninvokable
    static native <K,V> void __qt_QMultiMap_replace(long __this__nativeId, K key, V value, long replaceOneFunction);

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
    public java.util.Comparator<K> comparator(){
        return (o1,o2)->lessThan(o1, o2) ? -1 : (lessThan(o2, o1) ? 1 : 0);
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
    public List<V> get(Object key){
        return checkKey(key) ? values(castKey(key)) : Collections.emptyList();
    }

    @Override
    @QtUninvokable
    public java.util.List<V> put(K key, java.util.List<V> values){
    	java.util.List<V> old = values(key);
    	for(V value : values)
    		insert(key, value);
        return old;
    }

    @Override
    @QtUninvokable
    public java.util.List<V> remove(Object key){
		if(checkKey(key)) {
            return Collections.singletonList(take(castKey(key)));
        }else return Collections.emptyList();
    }

    @Override
    @QtUninvokable
    public boolean containsValue(Object value){
        return checkValue(value) ? keys(castValue(value)).isEmpty() : false;
    }
    
    @Override
    @QtUninvokable
    public int hashCode() { 
    	int hashCode = getClass().hashCode();
        for (QPair<K,V> e : this)
            hashCode = 31*hashCode + (e==null ? 0 : e.hashCode());
        return hashCode;
    }
}
