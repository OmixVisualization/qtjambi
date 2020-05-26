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
import java.util.Collection;
import java.util.List;
import java.util.ListIterator;

public abstract class QtJambiAbstractListObject<E> extends QtJambiCollectionObject<E> implements List<E> {
	
    protected QtJambiAbstractListObject(Class<E> elementType) {
		super(elementType);
	}

    protected QtJambiAbstractListObject(QPrivateConstructor p, Class<E> elementType) {
		super(p, elementType);
	}

    public abstract void removeAt(int cursor);
    public abstract void append(Collection<E> c);

    @Override
    @SuppressWarnings("unchecked")
    public boolean addAll(Collection<? extends E> c) {
        append((Collection<E>)c);
        return true;
    }

    @Override
    public boolean addAll(int index, Collection<? extends E> c) {
        for (E o : c) {
            add(index++, o);
        }
        return true;
    }

    @Override
    public E remove(int index) {
        E result = get(index);
        this.removeAt(index);
        return result;
    }

	@Override
	public ListIterator<E> listIterator() {
	    return listIterator(0);
	}
	
	@Override
	public ListIterator<E> listIterator(int index) {
	    return begin().toJavaListIterator(this::begin, this::end, index);
	}
	
	@SuppressWarnings("unchecked")
	@Override
	public List<E> subList(int fromIndex, int toIndex) {
	    if (fromIndex < 0)
	        throw new IndexOutOfBoundsException("fromIndex = " + fromIndex);
	    if (toIndex > size())
	        throw new IndexOutOfBoundsException("toIndex = " + toIndex);
	    if (fromIndex > toIndex)
	        throw new IllegalArgumentException("fromIndex(" + fromIndex +
	                                       ") > toIndex(" + toIndex + ")");
	    List<E> sublist;
	    try {
	    	sublist = this.getClass().getConstructor().newInstance();
	    }catch(Throwable e) {
	    	sublist = new ArrayList<>();
	    }
	    for (int i = 0; i < toIndex - fromIndex; i++) {
	        sublist.add(get(fromIndex+i));
	    }
	    return sublist;
	}
}
