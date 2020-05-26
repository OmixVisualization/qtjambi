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

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.Iterator;
import java.util.List;

import io.qt.QtObject;

public abstract class QtJambiCollectionObject<E> extends QtObject implements Collection<E> {
	
	@NativeAccess
	private final Class<E> elementType;

	protected QtJambiCollectionObject(Class<E> elementType) {
		super();
		this.elementType = elementType;
	}

    protected QtJambiCollectionObject(QPrivateConstructor p, Class<E> elementType) {
		super(p);
		this.elementType = elementType;
	}

	protected abstract QtJambiIteratorObject<E> begin();
	protected abstract QtJambiIteratorObject<E> end();
    
	@Override
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
	public boolean containsAll(Collection<?> c) {
		boolean result = true;
        for (Object object : c) {
            result &= contains(object);
        }
        return result;
	}

	@Override
    public boolean addAll(Collection<? extends E> c) {
        boolean result = true;
        for (E o : c) {
            result &= this.add(o);
        }
        return result;
    }

	@Override
	public boolean removeAll(Collection<?> c) {
		boolean result = true;
        for (Object o : c) {
            result &= this.remove(o);
        }
        return result;
	}

	@Override
	public boolean retainAll(Collection<?> c) {
		List<E> list = new ArrayList<>(this);
        boolean b = list.retainAll(c);
        if(b) {
            this.clear();
            this.addAll(list);
        }
        return b;
	}
	
	@Override
	public Iterator<E> iterator() {
		return begin().toJavaIterator(this::end);
	}
	
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

    final Class<E> elementType() {
		return elementType;
	}
    
    protected final boolean checkElement(Object element) {
    	if(elementType.isPrimitive()) {
    		return QtJambiInternal.getComplexType(elementType).isInstance(element);
    	}
		return elementType.isInstance(element) || element==null;
	}
    
    @SuppressWarnings("unchecked")
    protected final E castElement(Object element) {
    	if(elementType.isPrimitive()) {
    		return (E)QtJambiInternal.getComplexType(elementType).cast(element);
    	}
		return elementType.cast(element);
	}
}
