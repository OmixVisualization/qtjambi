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

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.Iterator;
import java.util.List;

import io.qt.QtUninvokable;

public abstract class AbstractSequentialContainer<E> extends AbstractContainer<E> implements Collection<E>, Cloneable {
	
    protected AbstractSequentialContainer(QPrivateConstructor p) {
		super(p);
	}
    
    @Override
    public abstract AbstractSequentialContainer<E> clone();
    
	@Override
    @QtUninvokable
    public Object[] toArray() {
        Object[] result = new Object[size()];
        int i = 0;
        for (E e : this) {
            result[i++] = e;
        }
        return result;
    }

    @SuppressWarnings("unchecked")
    @Override
    @QtUninvokable
    public <T> T[] toArray(T[] a) {
        if (a.length < size())
            a = (T[])Arrays.copyOf(a, size(), a.getClass());
        int i = 0;
        for (E e : this) {
            a[i++] = (T)e;
        }
        return a;
    }

	@Override
    @QtUninvokable
	public boolean containsAll(Collection<?> c) {
		boolean result = true;
        for (Object object : c) {
            result &= contains(object);
        }
        return result;
	}

	@Override
    @QtUninvokable
    public boolean addAll(Collection<? extends E> c) {
        boolean result = true;
        for (E o : c) {
            result &= this.add(o);
        }
        return result;
    }

	@Override
    @QtUninvokable
	public boolean removeAll(Collection<?> c) {
		boolean result = true;
        for (Object o : c) {
            result &= this.remove(o);
        }
        return result;
	}

	@Override
    @QtUninvokable
	public boolean retainAll(Collection<?> c) {
		List<E> toBeRemoved = new ArrayList<>();
		for(E e : this) {
			if(!c.contains(e))
				toBeRemoved.add(e);
		}
        if(!toBeRemoved.isEmpty()) {
        	this.removeAll(toBeRemoved);
        }
        return !toBeRemoved.isEmpty();
	}
	
	@Override
    @QtUninvokable
	public final Iterator<E> iterator() {
		return constBegin().toJavaIterator();
	}
	
    @QtUninvokable
	public String toString() {
        Iterator<E> it = iterator();
        if (! it.hasNext())
            return "[]";

        StringBuilder sb = new StringBuilder();
        sb.append('[');
        for (;;) {
            E e = it.next();
            sb.append(e == this ? "(this Collection)" : e);
            if (! it.hasNext())
                return sb.append(']').toString();
            sb.append(',').append(' ');
        }
    }

    @QtUninvokable
    protected java.util.Iterator<E> descendingIterator() {
        return constEnd().toJavaDescendingIterator();
    }
}
