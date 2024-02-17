/****************************************************************************
**
** Copyright (C) 2009-2024 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.Iterator;
import java.util.List;

import io.qt.QtUninvokable;

/**
 * Abstract superclass of sequential containers in Qt.
 */
abstract class AbstractSequentialContainer<T> extends AbstractContainer<T> implements Collection<T>, Cloneable {
	
	/**
     * {@inheritDoc}
	 */
    AbstractSequentialContainer(QPrivateConstructor p) {
		super(p);
	}
    
    /**
     * <p>Creates and returns a copy of this sequential container.</p>
     */
    @Override
    public abstract AbstractSequentialContainer<T> clone();
    
    /**
     * Returns an array containing all of the elements in this container.
     */
	@Override
    @QtUninvokable
    public final Object[] toArray() {
        Object[] result = new Object[size()];
        int i = 0;
        for (T e : this) {
            result[i++] = e;
        }
        return result;
    }

    /**
     * Returns an array containing all of the elements in this container.
     */
    @SuppressWarnings("unchecked")
    @Override
    @QtUninvokable
    public final <A> A[] toArray(A[] a) {
        if (a.length < size())
            a = (A[])Arrays.copyOf(a, size(), a.getClass());
        int i = 0;
        for (T e : this) {
            a[i++] = (A)e;
        }
        return a;
    }

    /**
     * Returns {@code true} if this container contains all of the elements
     * in the specified collection.
     */
	@Override
    @QtUninvokable
	public final boolean containsAll(Collection<?> c) {
		boolean result = true;
        for (Object object : c) {
            result &= contains(object);
        }
        return result;
	}

	/**
     * Adds all of the elements in the specified collection to this container.
     */
	@Override
    @QtUninvokable
    public boolean addAll(Collection<? extends T> c) {
        boolean result = true;
        for (T o : c) {
            result &= this.add(o);
        }
        return result;
    }

	/**
     * Removes all of this container elements that are also contained in the
     * specified collection.
     */
	@Override
    @QtUninvokable
	public final boolean removeAll(Collection<?> c) {
		boolean result = true;
        for (Object o : c) {
            result &= this.remove(o);
        }
        return result;
	}

	/**
     * Retains only the elements in this container that are contained in the
     * specified collection.
     */
	@Override
    @QtUninvokable
	public boolean retainAll(Collection<?> c) {
		List<T> toBeRemoved = new ArrayList<>();
		for(T e : this) {
			if(!c.contains(e))
				toBeRemoved.add(e);
		}
        if(!toBeRemoved.isEmpty()) {
        	this.removeAll(toBeRemoved);
        }
        return !toBeRemoved.isEmpty();
	}
	
    /**
     * Returns an iterator over elements of type {@code T}.
     * @return an Iterator
     */
	@Override
    @QtUninvokable
	public final Iterator<T> iterator() {
		return constBegin().toJavaIterator();
	}
	
    /**
     * Returns a string representation of this sequential container.
     * @return String
     */
    @QtUninvokable
	public String toString() {
        Iterator<T> it = iterator();
        if (! it.hasNext())
            return "[]";

        StringBuilder sb = new StringBuilder();
        sb.append('[');
        for (;;) {
            T e = it.next();
            sb.append(e == this ? "(this Collection)" : e);
            if (! it.hasNext())
                return sb.append(']').toString();
            sb.append(',').append(' ');
        }
    }
}
