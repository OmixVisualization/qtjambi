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

public class QMap<K,V> extends io.qt.internal.QtJambiMapObject<K,V>
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
	
    private QMap(Class<K> keyType, Class<V> valueType, long beginFunction, long clearFunction, long containsFunction,
			long countObjectFunction, long endFunction, long findFunction, long firstFunction,
			long firstKeyFunction, long insertFunction, long keyFunction, long keysFunction, long keysForValueFunction,
			long lastFunction, long lastKeyFunction, long lowerBoundFunction, long equalFunction,
			long removeAllFunction, long sizeFunction, long takeFunction, long uniqueKeysFunction, long uniteFunction,
			long upperBoundFunction, long valueFunction, long valuesFunction, long valuesKeyFunction) {
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
	}
	
    private QMap(Class<K> keyType, Class<V> valueType, long beginFunction, long containsFunction,
			long countObjectFunction, long endFunction, long findFunction, long firstFunction,
			long firstKeyFunction, long keyFunction, long keysFunction, long keysForValueFunction,
			long lastFunction, long lastKeyFunction, long lowerBoundFunction, long equalFunction,
			long sizeFunction, long uniqueKeysFunction,  long upperBoundFunction, 
			long valueFunction, long valuesFunction, long valuesKeyFunction) {
    	this(keyType, valueType, beginFunction, 0, containsFunction, countObjectFunction, 
    			endFunction, findFunction, firstFunction, firstKeyFunction, 
    			0, keyFunction, keysFunction, keysForValueFunction, 
    			lastFunction, lastKeyFunction, lowerBoundFunction, 
    			equalFunction, 0, sizeFunction, 0, uniqueKeysFunction, sizeFunction, 
    			upperBoundFunction, valueFunction, valuesFunction, valuesKeyFunction);
	}
    
	@QtUninvokable
    protected final io.qt.core.QMapIterator<K,V> begin()    {
        if(beginFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QMap_begin(io.qt.internal.QtJambiInternal.checkedNativeId(this), beginFunction);
    }
    @QtUninvokable
    static native <K,V> io.qt.core.QMapIterator<K,V> __qt_QMap_begin(long __this__nativeId, long beginFunction);

    @QtUninvokable
    public final void clear()    {
        if(clearFunction==0)
        	throw new UnsupportedOperationException();
        __qt_QMap_clear(io.qt.internal.QtJambiInternal.checkedNativeId(this), clearFunction);
    }
    @QtUninvokable
    static native void __qt_QMap_clear(long __this__nativeId, long clearFunction);

    @QtUninvokable
    public final boolean contains(K key)    {
        if(containsFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QMap_contains(io.qt.internal.QtJambiInternal.checkedNativeId(this), key, containsFunction);
    }
    @QtUninvokable
    static native <K> boolean __qt_QMap_contains(long __this__nativeId, K key, long containsFunction);

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
    static native <K> int __qt_QMap_count(long __this__nativeId, K key, long countObjectFunction);

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
    static native <K,V> io.qt.core.QMapIterator<K,V> __qt_QMap_end(long __this__nativeId, long endFunction);

    @QtUninvokable
    public final io.qt.core.QMapIterator<K,V> find(K key)    {
        if(findFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QMap_find(io.qt.internal.QtJambiInternal.checkedNativeId(this), key, findFunction);
    }
    @QtUninvokable
    static native <K,V> io.qt.core.QMapIterator<K,V> __qt_QMap_find(long __this__nativeId, K key, long findFunction);

    @QtUninvokable
    public final V first()    {
        if(firstFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QMap_first(io.qt.internal.QtJambiInternal.checkedNativeId(this), firstFunction);
    }
    @QtUninvokable
    static native <V> V __qt_QMap_first(long __this__nativeId, long firstFunction);

    @QtUninvokable
    public final K firstKey()    {
        if(firstKeyFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QMap_firstKey(io.qt.internal.QtJambiInternal.checkedNativeId(this), firstKeyFunction);
    }
    @QtUninvokable
    static native <K> K __qt_QMap_firstKey(long __this__nativeId, long firstKeyFunction);

    @QtUninvokable
    public final void insert(K key, V value)    {
        if(insertFunction==0)
        	throw new UnsupportedOperationException();
        __qt_QMap_insert(io.qt.internal.QtJambiInternal.checkedNativeId(this), key, value, insertFunction);
    }
    @QtUninvokable
    static native <K,V> void __qt_QMap_insert(long __this__nativeId, K key, V value, long insertFunction);

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
    static native <K,V> K __qt_QMap_key(long __this__nativeId, V value, K defaultKey, long keyFunction);

    @QtUninvokable
    public final java.util.List<K> keys()    {
        if(keysFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QMap_keys(io.qt.internal.QtJambiInternal.checkedNativeId(this), keysFunction);
    }
    @QtUninvokable
    static native <K> java.util.List<K> __qt_QMap_keys(long __this__nativeId, long keysFunction);

    @QtUninvokable
    public final java.util.List<K> keys(V value)    {
        if(keysForValueFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QMap_keysForValue(io.qt.internal.QtJambiInternal.checkedNativeId(this), value, keysForValueFunction);
    }
    @QtUninvokable
    static native <K,V> java.util.List<K> __qt_QMap_keysForValue(long __this__nativeId, V value, long keysForValueFunction);

    @QtUninvokable
    public final V last()    {
        if(lastFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QMap_last(io.qt.internal.QtJambiInternal.checkedNativeId(this), lastFunction);
    }
    @QtUninvokable
    static native <V> V __qt_QMap_last(long __this__nativeId, long lastFunction);

    @QtUninvokable
    public final K lastKey()    {
        if(lastKeyFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QMap_lastKey(io.qt.internal.QtJambiInternal.checkedNativeId(this), lastKeyFunction);
    }
    @QtUninvokable
    static native <K> K __qt_QMap_lastKey(long __this__nativeId, long lastKeyFunction);

    @QtUninvokable
    public final io.qt.core.QMapIterator<K,V> lowerBound(K key)    {
        if(lowerBoundFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QMap_lowerBound(io.qt.internal.QtJambiInternal.checkedNativeId(this), key, lowerBoundFunction);
    }
    @QtUninvokable
    static native <K,V> io.qt.core.QMapIterator<K,V> __qt_QMap_lowerBound(long __this__nativeId, K key, long lowerBoundFunction);

    @QtUninvokable
    private final boolean operator_equal(java.util.Map<K,V> other)    {
        if(equalFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QMap_operator_equal(io.qt.internal.QtJambiInternal.checkedNativeId(this), other, equalFunction);
    }
    @QtUninvokable
    static native <K,V> boolean __qt_QMap_operator_equal(long __this__nativeId, java.util.Map<K,V> other, long equalFunction);

    @QtUninvokable
    public final int removeAll(Object key)    {
        if(removeAllFunction==0)
        	throw new UnsupportedOperationException();
        if(checkKey(key))
        	return __qt_QMap_remove(io.qt.internal.QtJambiInternal.checkedNativeId(this), key, removeAllFunction);
        else return 0;
    }
    @QtUninvokable
    static native <K,V> int __qt_QMap_remove(long __this__nativeId, K key, long removeAllFunction);

    @QtUninvokable
    public final int size()    {
        if(sizeFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QMap_size(io.qt.internal.QtJambiInternal.checkedNativeId(this), sizeFunction);
    }
    @QtUninvokable
    static native int __qt_QMap_size(long __this__nativeId, long sizeFunction);

    @QtUninvokable
    public final V take(K key)    {
        if(takeFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QMap_take(io.qt.internal.QtJambiInternal.checkedNativeId(this), key, takeFunction);
    }
    @QtUninvokable
    static native <K,V> V __qt_QMap_take(long __this__nativeId, K key, long takeFunction);

    @QtUninvokable
    @Deprecated
    public final java.util.List<K> uniqueKeys()    {
        if(uniqueKeysFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QMap_uniqueKeys(io.qt.internal.QtJambiInternal.checkedNativeId(this), uniqueKeysFunction);
    }
    @QtUninvokable
    static native <K> java.util.List<K> __qt_QMap_uniqueKeys(long __this__nativeId, long uniqueKeysFunction);

    @QtUninvokable
    @Deprecated
    public final void unite(java.util.Map<? super K, ? super V> other)    {
        if(uniteFunction==0)
        	throw new UnsupportedOperationException();
        __qt_QMap_unite(io.qt.internal.QtJambiInternal.checkedNativeId(this), other, uniteFunction);
    }
    @QtUninvokable
    static native void __qt_QMap_unite(long __this__nativeId, Object other, long uniteFunction);

    @QtUninvokable
    public final io.qt.core.QMapIterator<K,V> upperBound(K key)    {
        if(upperBoundFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QMap_upperBound(io.qt.internal.QtJambiInternal.checkedNativeId(this), key, upperBoundFunction);
    }
    @QtUninvokable
    static native <K,V> io.qt.core.QMapIterator<K,V> __qt_QMap_upperBound(long __this__nativeId, K key, long upperBoundFunction);


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
    @QtUninvokable
    static native <K,V> V __qt_QMap_value(long __this__nativeId, K key, V defaultValue, long valueFunction);

    @QtUninvokable
    public final java.util.List<V> values()    {
        if(valuesFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QMap_values(io.qt.internal.QtJambiInternal.checkedNativeId(this), valuesFunction);
    }
    @QtUninvokable
    static native <V> java.util.List<V> __qt_QMap_values(long __this__nativeId, long valuesFunction);

    @QtUninvokable
    @Deprecated
    public final java.util.List<V> values(K key)    {
        if(valuesKeyFunction==0)
        	throw new UnsupportedOperationException();
        return __qt_QMap_valuesKey(io.qt.internal.QtJambiInternal.checkedNativeId(this), key, valuesKeyFunction);
    }
    @QtUninvokable
    static native <K,V> java.util.List<V> __qt_QMap_valuesKey(long __this__nativeId, K key, long valuesKeyFunction);

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
