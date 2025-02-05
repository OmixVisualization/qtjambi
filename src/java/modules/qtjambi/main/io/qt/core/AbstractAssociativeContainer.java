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

import java.util.Arrays;
import java.util.Collection;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Objects;
import java.util.Set;

import io.qt.QtUninvokable;

/**
 * Abstract superclass of associative containers in Qt.
 */
abstract class AbstractAssociativeContainer<Key,T> extends AbstractContainer<T> implements Map<Key,T>, Iterable<QPair<Key,T>>, Cloneable {
	
    /**
     * <p>Creates and returns a copy of this associative container.</p>
     */
	@Override
    public abstract AbstractAssociativeContainer<Key,T> clone();
	
	/**
	 * Returns a {@link List} of the keys contained in this associative container.
	 * @return list of keys
	 */
    @QtUninvokable
	public abstract QList<Key> keys();

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
	public final void putAll(Map<? extends Key, ? extends T> m) {
		for(Map.Entry<? extends Key, ? extends T> entry : m.entrySet()) {
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
    AbstractAssociativeContainer(QPrivateConstructor p) {
		super(p);
	}
	
    /**
     * Returns a {@link Set} view of the mappings contained in this map.
     */
	@Override
    @QtUninvokable
	public final Set<Entry<Key, T>> entrySet() {
		return new Set<Entry<Key, T>>(){
			@Override
		    @QtUninvokable
			public int size() {
				return AbstractAssociativeContainer.this.size();
			}

			@Override
		    @QtUninvokable
			public boolean isEmpty() {
				return AbstractAssociativeContainer.this.isEmpty();
			}

			@Override
		    @QtUninvokable
			public boolean contains(Object o) {
				if(o instanceof QPair) {
					@SuppressWarnings("unchecked")
					QPair<Key,T> pair = (QPair<Key,T>)o;
					if(AbstractAssociativeContainer.this.containsKey(pair.first)) {
						return Objects.equals(AbstractAssociativeContainer.this.get(pair.first), pair.second);
					}
				}
				return false;
			}

			@Override
		    @QtUninvokable
			public Iterator<Entry<Key, T>> iterator() {
				return new Iterator<Entry<Key, T>>(){
					private final Iterator<QPair<Key, T>> iter = AbstractAssociativeContainer.this.iterator();
					@Override
					public boolean hasNext() {
						return iter.hasNext();
					}

					@Override
					public Entry<Key, T> next() {
						QPair<Key, T> pair = iter.next();
						return QPair.toEntry(pair);
					}
				};
			}

			@Override
		    @QtUninvokable
			public Object[] toArray() {
				Object[] result = new Object[size()];
		        int i = 0;
		        for(Entry<Key, T> e : this) {
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
		        for(Entry<Key, T> e : this) {
		            a[i++] = (A)e;
		        }
		        return a;
			}

			@Override
		    @QtUninvokable
			public boolean add(Entry<Key, T> e) {
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
			public boolean addAll(Collection<? extends Entry<Key, T>> c) {
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
	
	static <K> Set<K> keySet(QList<K> keys) {
		return new Set<K>(){
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
				return keys.contains(o);
			}

			@Override
		    @QtUninvokable
			public Iterator<K> iterator() {
				return keys.iterator();
			}

			@Override
		    @QtUninvokable
			public Object[] toArray() {
		        return keys.toArray();
			}

			@Override
		    @QtUninvokable
			public <T> T[] toArray(T[] a) {
		        return keys.toArray(a);
			}

			@Override
		    @QtUninvokable
			public boolean add(K e) {
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
			public boolean addAll(Collection<? extends K> c) {
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
     * Returns a {@link Set} view of the keys contained in this map.
     */
	@Override
    @QtUninvokable
	public final Set<Key> keySet() {
		return keySet(keys());
	}
    
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
            sb.append('=');
            sb.append(e.second == this ? "(this Map)" : e.second);
            if (! it.hasNext())
                return sb.append('}').toString();
            sb.append(';').append(' ');
        }
    }
}
