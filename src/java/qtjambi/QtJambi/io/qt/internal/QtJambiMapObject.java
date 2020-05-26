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
package io.qt.internal;

import java.util.AbstractMap;
import java.util.Comparator;
import java.util.NavigableMap;
import java.util.NavigableSet;
import java.util.Objects;
import java.util.Set;
import java.util.TreeMap;
import java.util.TreeSet;

import io.qt.core.QPair;

public abstract class QtJambiMapObject<K,V> extends QtJambiAbstractMapObject<K,V> implements NavigableMap<K,V> {

	protected QtJambiMapObject(Class<K> keyType, Class<V> valueType) {
		super(keyType, valueType);
	}

    protected QtJambiMapObject(QPrivateConstructor p, Class<K> keyType, Class<V> valueType) {
		super(p, keyType, valueType);
	}
    
    protected abstract QtJambiMapIteratorObject<K,V> find(K key);
    protected abstract QtJambiMapIteratorObject<K,V> lowerBound(K key);
    protected abstract QtJambiMapIteratorObject<K,V> upperBound(K key);

	@Override
	public NavigableMap<K,V> subMap(K fromKey, K toKey) {
		return subMap(fromKey, true, toKey, false);
	}

	@Override
	public NavigableMap<K,V> headMap(K toKey) {
		return headMap(toKey, false);
	}

	@Override
	public NavigableMap<K,V> tailMap(K fromKey) {
		return tailMap(fromKey, true);
	}

    @Override
	public Entry<K, V> lowerEntry(K key) {
    	QtJambiMapIteratorObject<K,V> iterator = lowerBound(key);
    	if(!iterator.equals(end()) && !iterator.equals(begin()) && Objects.equals(iterator._key(), key))
    		iterator._decrement();
    	if(iterator.equals(end()))
    		return null;
    	else
    		return new AbstractMap.SimpleImmutableEntry<>(iterator._key(), iterator._value());
	}

	@Override
	public K lowerKey(K key) {
    	QtJambiMapIteratorObject<K,V> iterator = lowerBound(key);
    	if(!iterator.equals(end()) && !iterator.equals(begin()) && Objects.equals(iterator._key(), key))
    		iterator._decrement();
    	if(iterator.equals(end()))
    		return null;
    	else return iterator._key();
	}

	@Override
	public Entry<K, V> floorEntry(K key) {
    	QtJambiMapIteratorObject<K,V> iterator = lowerBound(key);
    	if(iterator.equals(end()))
    		return null;
    	else
    		return new AbstractMap.SimpleImmutableEntry<>(iterator._key(), iterator._value());
	}

	@Override
	public K floorKey(K key) {
    	QtJambiMapIteratorObject<K,V> iterator = lowerBound(key);
    	if(iterator.equals(end()))
    		return null;
    	else return iterator._key();
	}

	@Override
	public Entry<K, V> ceilingEntry(K key) {
    	QtJambiMapIteratorObject<K,V> iterator = lowerBound(key);
    	if(iterator.equals(end()))
    		return null;
    	else
    		return new AbstractMap.SimpleImmutableEntry<>(iterator._key(), iterator._value());
	}

	@Override
	public K ceilingKey(K key) {
    	QtJambiMapIteratorObject<K,V> iterator = lowerBound(key);
    	if(iterator.equals(end()))
    		return null;
    	else return iterator._key();
	}

	@Override
	public Entry<K, V> higherEntry(K key) {
    	QtJambiMapIteratorObject<K,V> iterator = lowerBound(key);
    	if(!iterator.equals(end()) && Objects.equals(iterator._key(), key))
    		iterator._increment();
    	if(iterator.equals(end()))
    		return null;
    	else
    		return new AbstractMap.SimpleImmutableEntry<>(iterator._key(), iterator._value());
	}

	@Override
	public K higherKey(K key) {
    	QtJambiMapIteratorObject<K,V> iterator = lowerBound(key);
    	if(!iterator.equals(end()) && Objects.equals(iterator._key(), key))
    		iterator._increment();
    	if(iterator.equals(end()))
    		return null;
    	else
    		return iterator._key();
	}

	@Override
	public Entry<K, V> firstEntry() {
		if(isEmpty())
			return null;
		else return new AbstractMap.SimpleImmutableEntry<>(begin()._key(), begin()._value());
	}

	@Override
	public Entry<K, V> lastEntry() {
		if(isEmpty())
			return null;
		else return new AbstractMap.SimpleImmutableEntry<>(end()._key(), end()._value());
	}

	@Override
	public Entry<K, V> pollFirstEntry() {
		Entry<K, V> entry = firstEntry();
		if(entry!=null) {
			remove(entry.getKey());
		}
		return entry;
	}

	@Override
	public Entry<K, V> pollLastEntry() {
		Entry<K, V> entry = lastEntry();
		if(entry!=null) {
			remove(entry.getKey());
		}
		return entry;
	}

	@Override
	public NavigableMap<K, V> descendingMap() {
		Comparator<? super K> comparator = this.comparator();
		TreeMap<K, V> descendingMap = new TreeMap<>((K o1, K o2)-> -1*comparator.compare(o1, o2));
		descendingMap.putAll(this);
		return descendingMap;
	}

	@Override
	public NavigableSet<K> navigableKeySet() {
		TreeSet<K> set = new TreeSet<>(this.comparator());
		set.addAll(this.keySet());
		return set;
	}

	@Override
	public NavigableSet<K> descendingKeySet() {
		Comparator<? super K> comparator = this.comparator();
		TreeSet<K> set = new TreeSet<>((K o1, K o2)-> -1*comparator.compare(o1, o2));
		set.addAll(this.keySet());
		return set;
	}

	@SuppressWarnings("unchecked")
	@Override
	public NavigableMap<K, V> subMap(K fromKey, boolean fromInclusive, K toKey, boolean toInclusive) {
		NavigableMap<K,V> map;
		try {
			map = this.getClass().getConstructor().newInstance();
		} catch (Throwable e) {
			map = new TreeMap<>(comparator());
		}
		QtJambiMapIteratorObject<K,V> k2 = lowerBound(toKey);
		if(!k2.equals(end())) {
			QtJambiMapIteratorObject<K,V> k1 = lowerBound(fromKey);
			if(!fromInclusive) {
				k1._increment();
			}
			for(; !k1.equals(k2); k1._increment()) {
				map.put(k1._key(), k1._value());
			}
			if(toInclusive) {
				map.put(k2._key(), k2._value());
			}
		}
		return map;
	}

	@SuppressWarnings("unchecked")
	@Override
	public NavigableMap<K, V> headMap(K toKey, boolean inclusive) {
		QtJambiMapIteratorObject<K,V> k = lowerBound(toKey);
		NavigableMap<K,V> map;
		try {
			map = this.getClass().getConstructor().newInstance();
		} catch (Throwable e) {
			map = new TreeMap<>(comparator());
		}
		if(!k.equals(end())) {
			for(QtJambiMapIteratorObject<K,V> iterator = begin(); !iterator.equals(k); iterator._increment()) {
				map.put(iterator._key(), iterator._value());
			}
			if(inclusive) {
				map.put(k._key(), k._value());
			}
		}
		return map;
	}

	@SuppressWarnings("unchecked")
	@Override
	public NavigableMap<K, V> tailMap(K fromKey, boolean inclusive) {
		QtJambiMapIteratorObject<K,V> k = lowerBound(fromKey);
		QtJambiMapIteratorObject<K,V> end = end();
		NavigableMap<K,V> map;
		try {
			map = this.getClass().getConstructor().newInstance();
		} catch (Throwable e) {
			map = new TreeMap<>(comparator());
		}
		if(!k.equals(end)) {
			if(!inclusive) {
				k._increment();
			}
			for(;!k.equals(end); k._increment()) {
				map.put(k._key(), k._value());
			}
		}
		return map;
	}
	
	@Override
	public Set<Entry<K, V>> entrySet() {
		Comparator<? super K> comparator = comparator();
		Set<Entry<K, V>> entrySet = new TreeSet<>((e1, e2)->comparator.compare(e1.getKey(), e2.getKey()));
		for(QPair<K,V> pair : this) {
			entrySet.add(new AbstractMap.SimpleImmutableEntry<>(pair.first, pair.second));
		}
		return entrySet;
	}
	
	@Override
	public Set<K> keySet() {
		TreeSet<K> set = new TreeSet<>(comparator());
		set.addAll(keys());
		return set;
	}
}
