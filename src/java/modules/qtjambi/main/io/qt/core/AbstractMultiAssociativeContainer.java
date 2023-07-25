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

import java.util.AbstractMap.SimpleEntry;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.Collections;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Objects;
import java.util.Set;

import io.qt.NativeAccess;
import io.qt.QtUninvokable;

/**
 * Abstract superclass of multi-associative containers in Qt.
 */
abstract class AbstractMultiAssociativeContainer<Key,T> extends AbstractContainer<T> implements Map<Key,List<T>>, Iterable<QPair<Key,T>>, Cloneable {

	@NativeAccess
	private static boolean putMultiMap(Map<Object, List<Object>> map, Object key, Object value) {
		map.computeIfAbsent(key, k -> new ArrayList<>()).add(value);
		return true;
	}
	
	/**
	 * Returns a {@link List} of the keys contained in this associative container.
	 * @return list of keys
	 */
    @QtUninvokable
	public abstract QList<Key> uniqueKeys();
    
    /**
     * Returns a {@link Set} view of the keys contained in this map.
     */
	@Override
    @QtUninvokable
	public final Set<Key> keySet() {
		return AbstractAssociativeContainer.keySet(uniqueKeys());
	}
	
	/**
	 * Returns all values for this key.
	 */
	public abstract QList<T> values(Key key);
	
    /**
     * Returns a {@link Set} view of the mappings contained in this map.
     */
	@Override
    @QtUninvokable
	public final Set<Entry<Key, List<T>>> entrySet() {
		QList<Key> keys = uniqueKeys();
		return new Set<Entry<Key, List<T>>>(){
			@Override
		    @QtUninvokable
			public int size() {
				return keys.size();
			}

			@Override
		    @QtUninvokable
			public boolean isEmpty() {
				return keys.isEmpty();
			}

			@Override
		    @QtUninvokable
			public boolean contains(Object o) {
				if(o instanceof QPair) {
					@SuppressWarnings("unchecked")
					QPair<Key,T> pair = (QPair<Key,T>)o;
					if(containsKey(pair.first)) {
						return Objects.equals(values(pair.first), pair.second);
					}
				}
				return false;
			}

			@Override
		    @QtUninvokable
			public Iterator<Entry<Key, List<T>>> iterator() {
				if(isEmpty()) {
					return Collections.<Entry<Key, List<T>>>emptyList().iterator();
				}
				return new Iterator<Entry<Key, List<T>>>(){
					private final Iterator<Key> iter = keys.iterator();
					@Override
					public boolean hasNext() {
						return iter.hasNext();
					}

					@Override
					public Entry<Key, List<T>> next() {
						Key key = iter.next();
						return new SimpleEntry<>(key, values(key));
					}
				};
			}

			@Override
		    @QtUninvokable
			public Object[] toArray() {
				Object[] result = new Object[size()];
		        int i = 0;
		        for(Entry<Key, List<T>> e : this) {
		            result[i++] = e;
		        }
		        return result;
			}

		    @SuppressWarnings("unchecked")
			@Override
		    @QtUninvokable
			public <A> A[] toArray(A[] a) {
		        if (a.length < size())
		            a = (A[])Arrays.copyOf(a, size(), a.getClass());
		        int i = 0;
		        for(Entry<Key, List<T>> e : this) {
		            a[i++] = (A)e;
		        }
		        return a;
			}

			@Override
		    @QtUninvokable
			public boolean add(Entry<Key, List<T>> e) {
				throw new UnsupportedOperationException();
			}

			@Override
		    @QtUninvokable
			public boolean remove(Object o) {
				throw new UnsupportedOperationException();
			}

			@Override
		    @QtUninvokable
			public boolean containsAll(Collection<?> c) {
				for(Object o : c) {
					if(!contains(o))
						return false;
				}
				return true;
			}

			@Override
		    @QtUninvokable
			public boolean addAll(Collection<? extends Entry<Key, List<T>>> c) {
				throw new UnsupportedOperationException();
			}

			@Override
		    @QtUninvokable
			public boolean retainAll(Collection<?> c) {
				throw new UnsupportedOperationException();
			}

			@Override
		    @QtUninvokable
			public boolean removeAll(Collection<?> c) {
				throw new UnsupportedOperationException();
			}

			@Override
		    @QtUninvokable
			public void clear() {
				throw new UnsupportedOperationException();
			}
		};
	}

    /**
     * Provides a mutable C++ iterator to the containers begin.
     * @return begin
     */
    @QtUninvokable
	protected abstract QAssociativeIterator<Key,T> begin();

    /**
     * Provides a mutable C++ iterator to the containers end.
     * @return end
     */
    @QtUninvokable
	protected abstract QAssociativeIterator<Key,T> end();

    /**
     * Provides a constant C++ iterator to the containers begin.
     * @return begin
     */
    @QtUninvokable
	protected abstract QAssociativeConstIterator<Key,T> constBegin();

    /**
     * Provides a constant C++ iterator to the containers end.
     * @return end
     */
    @QtUninvokable
	protected abstract QAssociativeConstIterator<Key,T> constEnd();

    /**
     * Copies all of the mappings from the specified map to this map.
     */
	@Override
    @QtUninvokable
	public final void putAll(Map<? extends Key, ? extends List<T>> m) {
		for(Map.Entry<? extends Key, ? extends List<T>> entry : m.entrySet()) {
			put(entry.getKey(), entry.getValue());
		}
	}
	
    /**
     * Returns an iterator over elements of type {@code QPair<K,V>}.
     * @return an Iterator
     */
	@Override
    @QtUninvokable
	public final Iterator<QPair<Key,T>> iterator() {
		return constBegin().iterator();
	}
	
	/**
     * {@inheritDoc}
	 */
    AbstractMultiAssociativeContainer(QPrivateConstructor p) {
		super(p);
	}
    
    /**
     * <p>Creates and returns a copy of this multi-associative container.</p>
     */
    @Override
    public abstract AbstractMultiAssociativeContainer<Key,T> clone();
    
    /**
     * Returns a string representation of this associative container.
     * @return String
     */
    @QtUninvokable
	public String toString() {
        Iterator<QPair<Key,T>> it = iterator();
        if (! it.hasNext())
            return "{}";

        StringBuilder sb = new StringBuilder();
        sb.append('{');
        for (;;) {
        	QPair<Key,T> e = it.next();
            sb.append(e.first == this ? "(this Map)" : e.first);
            sb.append(',').append(' ');
            sb.append(e.second == this ? "(this Map)" : e.second);
            if (! it.hasNext())
                return sb.append('}').toString();
            sb.append(';').append(' ');
        }
    }
}
