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
package io.qt.internal;

import java.util.AbstractMap;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.List;
import java.util.NavigableMap;
import java.util.NavigableSet;
import java.util.Objects;
import java.util.Set;
import java.util.TreeMap;
import java.util.TreeSet;

import io.qt.QtUninvokable;
import io.qt.core.QPair;

public abstract class AbstractMultiMap<K,V> extends AbstractMultiAssociativeContainer<K,V> implements NavigableMap<K,List<V>> {

	protected AbstractMultiMap() {
		super();
	}

    protected AbstractMultiMap(QPrivateConstructor p) {
		super(p);
	}
    
    @QtUninvokable
    protected abstract AbstractAssociativeConstIterator<K,V> find(K key);
    @QtUninvokable
    protected abstract AbstractAssociativeConstIterator<K,V> lowerBound(K key);
    @QtUninvokable
    protected abstract AbstractAssociativeConstIterator<K,V> upperBound(K key);

	@Override
    @QtUninvokable
	public final NavigableMap<K,List<V>> subMap(K fromKey, K toKey) {
		return subMap(fromKey, true, toKey, false);
	}

	@Override
    @QtUninvokable
	public final NavigableMap<K,List<V>> headMap(K toKey) {
		return headMap(toKey, false);
	}

	@Override
    @QtUninvokable
	public final NavigableMap<K,List<V>> tailMap(K fromKey) {
		return tailMap(fromKey, true);
	}

    @Override
    @QtUninvokable
	public final Entry<K, List<V>> lowerEntry(K key) {
    	AbstractAssociativeConstIterator<K,V> iterator = lowerBound(key);
    	if(!iterator.equals(constEnd()) && !iterator.equals(constBegin()) && Objects.equals(iterator.checkedKey(), key))
    		iterator.decrement();
    	if(iterator.equals(constEnd()))
    		return null;
    	else {
    		List<V> values = new ArrayList<>();
    		K _key = iterator.checkedKey();
    		Comparator<? super K> comparator = comparator();
    		for(; comparator.compare(_key, iterator.checkedKey())==0 && !iterator.equals(constEnd()); iterator.increment()) {
    			values.add(iterator.checkedValue());
    		}
    		return new AbstractMap.SimpleImmutableEntry<>(iterator.checkedKey(), values);
    	}
	}

	@Override
    @QtUninvokable
	public final K lowerKey(K key) {
    	AbstractAssociativeConstIterator<K,V> iterator = lowerBound(key);
    	if(!iterator.equals(constEnd()) && !iterator.equals(constBegin()) && Objects.equals(iterator.checkedKey(), key))
    		iterator.decrement();
    	if(iterator.equals(constEnd()))
    		return null;
    	else return iterator.checkedKey();
	}

	@Override
    @QtUninvokable
	public final Entry<K, List<V>> floorEntry(K key) {
    	AbstractAssociativeConstIterator<K,V> iterator = lowerBound(key);
    	if(iterator.equals(constEnd()))
    		return null;
    	else {
    		List<V> values = new ArrayList<>();
    		K _key = iterator.checkedKey();
    		Comparator<? super K> comparator = comparator();
    		for(; comparator.compare(_key, iterator.checkedKey())==0 && !iterator.equals(constEnd()); iterator.increment()) {
    			values.add(iterator.checkedValue());
    		}
    		return new AbstractMap.SimpleImmutableEntry<>(_key, values);
    	}
	}

	@Override
    @QtUninvokable
	public final K floorKey(K key) {
    	AbstractAssociativeConstIterator<K,V> iterator = lowerBound(key);
    	if(iterator.equals(constEnd()))
    		return null;
    	else return iterator.checkedKey();
	}

	@Override
    @QtUninvokable
	public final Entry<K, List<V>> ceilingEntry(K key) {
    	AbstractAssociativeConstIterator<K,V> iterator = lowerBound(key);
    	if(iterator.equals(constEnd()))
    		return null;
    	else {
    		List<V> values = new ArrayList<>();
    		K _key = iterator.checkedKey();
    		Comparator<? super K> comparator = comparator();
    		for(; comparator.compare(_key, iterator.checkedKey())==0 && !iterator.equals(constEnd()); iterator.increment()) {
    			values.add(iterator.checkedValue());
    		}
    		return new AbstractMap.SimpleImmutableEntry<>(_key, values);
    	}
	}

	@Override
    @QtUninvokable
	public final K ceilingKey(K key) {
    	AbstractAssociativeConstIterator<K,V> iterator = lowerBound(key);
    	if(iterator.equals(constEnd()))
    		return null;
    	else return iterator.checkedKey();
	}

	@Override
    @QtUninvokable
	public final Entry<K, List<V>> higherEntry(K key) {
    	AbstractAssociativeConstIterator<K,V> iterator = lowerBound(key);
    	if(!iterator.equals(constEnd()) && Objects.equals(iterator.checkedKey(), key))
    		iterator.increment();
    	if(iterator.equals(constEnd()))
    		return null;
    	else {
    		List<V> values = new ArrayList<>();
    		K _key = iterator.checkedKey();
    		Comparator<? super K> comparator = comparator();
    		for(; comparator.compare(_key, iterator.checkedKey())==0 && !iterator.equals(constEnd()); iterator.increment()) {
    			values.add(iterator.checkedValue());
    		}
    		return new AbstractMap.SimpleImmutableEntry<>(_key, values);
    	}
	}

	@Override
    @QtUninvokable
	public final K higherKey(K key) {
    	AbstractAssociativeConstIterator<K,V> iterator = lowerBound(key);
    	if(!iterator.equals(constEnd()) && Objects.equals(iterator.checkedKey(), key))
    		iterator.increment();
    	if(iterator.equals(constEnd()))
    		return null;
    	else
    		return iterator.checkedKey();
	}

	@Override
    @QtUninvokable
	public final Entry<K, List<V>> firstEntry() {
		if(isEmpty())
			return null;
		else {
			AbstractAssociativeConstIterator<K,V> iterator = constBegin();
    		List<V> values = new ArrayList<>();
    		K _key = iterator.checkedKey();
    		Comparator<? super K> comparator = comparator();
    		for(; comparator.compare(_key, iterator.checkedKey())==0 && !iterator.equals(constEnd()); iterator.increment()) {
    			values.add(iterator.checkedValue());
    		}
    		return new AbstractMap.SimpleImmutableEntry<>(_key, values);
		}
	}

	@Override
    @QtUninvokable
	public final Entry<K, List<V>> lastEntry() {
		if(isEmpty())
			return null;
		else {
			AbstractAssociativeConstIterator<K,V> iterator = constEnd();
			iterator.decrement();
    		List<V> values = new ArrayList<>();
    		K _key = iterator.checkedKey();
    		Comparator<? super K> comparator = comparator();
    		while(comparator.compare(_key, iterator.checkedKey())==0) {
    			values.add(0, iterator.checkedValue());
    			iterator.decrement();
    			if(iterator.equals(constBegin())) {
    				
    			}
    		}
    		return new AbstractMap.SimpleImmutableEntry<>(_key, values);
		}
	}

	@Override
    @QtUninvokable
	public final Entry<K, List<V>> pollFirstEntry() {
		Entry<K, List<V>> entry = firstEntry();
		if(entry!=null) {
			remove(entry.getKey());
		}
		return entry;
	}

	@Override
    @QtUninvokable
	public final Entry<K, List<V>> pollLastEntry() {
		Entry<K, List<V>> entry = lastEntry();
		if(entry!=null) {
			remove(entry.getKey());
		}
		return entry;
	}

	@Override
    @QtUninvokable
	public final NavigableMap<K, List<V>> descendingMap() {
		Comparator<? super K> comparator = this.comparator();
		TreeMap<K, List<V>> descendingMap = new TreeMap<>((K o1, K o2)-> -1*comparator.compare(o1, o2));
		descendingMap.putAll(this);
		return descendingMap;
	}

	@Override
    @QtUninvokable
	public final NavigableSet<K> navigableKeySet() {
		TreeSet<K> set = new TreeSet<>(this.comparator());
		set.addAll(this.keySet());
		return set;
	}

	@Override
    @QtUninvokable
	public final NavigableSet<K> descendingKeySet() {
		Comparator<? super K> comparator = this.comparator();
		TreeSet<K> set = new TreeSet<>((K o1, K o2)-> -1*comparator.compare(o1, o2));
		set.addAll(this.keySet());
		return set;
	}

	@SuppressWarnings("unchecked")
	@Override
    @QtUninvokable
	public final NavigableMap<K, List<V>> subMap(K fromKey, boolean fromInclusive, K toKey, boolean toInclusive) {
		NavigableMap<K,List<V>> map;
		try {
			map = this.getClass().getConstructor().newInstance();
		} catch (Throwable e) {
			map = new TreeMap<>(comparator());
		}
		AbstractAssociativeConstIterator<K,V> k2 = lowerBound(toKey);
		if(!k2.equals(constEnd())) {
			AbstractAssociativeConstIterator<K,V> k1 = lowerBound(fromKey);
			if(!fromInclusive) {
				k1.increment();
			}
			for(; !k1.equals(k2); k1.increment()) {
				map.computeIfAbsent(k1.checkedKey(), MetaObjectUtility.arrayListFactory()).add(k1.checkedValue());
			}
			if(toInclusive) {
				map.computeIfAbsent(k2.checkedKey(), MetaObjectUtility.arrayListFactory()).add(k2.checkedValue());
			}
		}
		return map;
	}

	@SuppressWarnings("unchecked")
	@Override
    @QtUninvokable
    public final NavigableMap<K, List<V>> headMap(K toKey, boolean inclusive) {
		AbstractAssociativeConstIterator<K,V> k = lowerBound(toKey);
		NavigableMap<K,List<V>> map;
		try {
			map = this.getClass().getConstructor().newInstance();
		} catch (Throwable e) {
			map = new TreeMap<>(comparator());
		}
		if(!k.equals(constEnd())) {
			for(AbstractAssociativeConstIterator<K,V> iterator = constBegin(); !iterator.equals(k); iterator.increment()) {
				map.computeIfAbsent(iterator.checkedKey(), MetaObjectUtility.arrayListFactory()).add(iterator.checkedValue());
			}
			if(inclusive) {
				map.computeIfAbsent(k.checkedKey(), MetaObjectUtility.arrayListFactory()).add(k.checkedValue());
			}
		}
		return map;
	}

	@SuppressWarnings("unchecked")
	@Override
    @QtUninvokable
	public final NavigableMap<K, List<V>> tailMap(K fromKey, boolean inclusive) {
		AbstractAssociativeConstIterator<K,V> k = lowerBound(fromKey);
		AbstractAssociativeConstIterator<K,V> end = constEnd();
		NavigableMap<K,List<V>> map;
		try {
			map = this.getClass().getConstructor().newInstance();
		} catch (Throwable e) {
			map = new TreeMap<>(comparator());
		}
		if(!k.equals(end)) {
			if(!inclusive) {
				k.increment();
			}
			for(;!k.equals(end); k.increment()) {
				map.computeIfAbsent(k.checkedKey(), MetaObjectUtility.arrayListFactory()).add(k.checkedValue());
			}
		}
		return map;
	}
	
	@Override
    @QtUninvokable
	public final Set<Entry<K, List<V>>> entrySet() {
		Comparator<? super K> comparator = comparator();
		Set<Entry<K, List<V>>> entrySet = new TreeSet<>((e1, e2)->comparator.compare(e1.getKey(), e2.getKey()));
		if(!this.isEmpty()) {
			K currentKey = null;
			List<V> currentValues = null;
			for(QPair<K,V> pair : this) {
				if(currentValues==null) {
					currentValues = new ArrayList<>();
					currentValues.add(pair.second);
					currentKey = pair.first;
				}else{
					if(comparator.compare(currentKey, pair.first)==0) {
						currentValues.add(pair.second);
					}else {
						entrySet.add(new AbstractMap.SimpleImmutableEntry<>(currentKey, currentValues));
						currentValues = new ArrayList<>();
						currentValues.add(pair.second);
						currentKey = pair.first;
					}
				}
			}
			entrySet.add(new AbstractMap.SimpleImmutableEntry<>(currentKey, currentValues));
		}
		return entrySet;
	}
	
	@Override
    @QtUninvokable
	public final Set<K> keySet() {
		TreeSet<K> set = new TreeSet<>(comparator());
		set.addAll(keys());
		return set;
	}
}
