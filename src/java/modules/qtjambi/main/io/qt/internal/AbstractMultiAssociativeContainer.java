/****************************************************************************
**
** Copyright (C) 2009-2022 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
import java.util.Iterator;
import java.util.List;
import java.util.Map;

import io.qt.NativeAccess;
import io.qt.QtObject;
import io.qt.QtUninvokable;
import io.qt.core.QPair;

public abstract class AbstractMultiAssociativeContainer<K,V> extends QtObject implements Map<K,List<V>>, Iterable<QPair<K,V>> {

	@NativeAccess
	private static boolean putMultiMap(Map<Object, List<Object>> map, Object key, Object value) {
		map.computeIfAbsent(key, k -> new ArrayList<>()).add(value);
		return true;
	}
	
	@Override
    @QtUninvokable
	public abstract int size();

	@Override
    @QtUninvokable
	public abstract boolean isEmpty();
	
    @QtUninvokable
	public abstract java.util.List<K> keys();

    @QtUninvokable
	protected abstract AbstractAssociativeConstIterator<K,V> begin();

    @QtUninvokable
	protected abstract AbstractAssociativeConstIterator<K,V> end();

    @QtUninvokable
	protected abstract AbstractAssociativeConstIterator<K,V> constBegin();

    @QtUninvokable
	protected abstract AbstractAssociativeConstIterator<K,V> constEnd();

	@Override
    @QtUninvokable
	public final void putAll(Map<? extends K, ? extends List<V>> m) {
		for(Map.Entry<? extends K, ? extends List<V>> entry : m.entrySet()) {
			put(entry.getKey(), entry.getValue());
		}
	}
	
	@Override
    @QtUninvokable
	public final Iterator<QPair<K,V>> iterator() {
		return constBegin().toJavaMapIterator();
	}
	
    protected AbstractMultiAssociativeContainer() {
		super();
	}

    protected AbstractMultiAssociativeContainer(QPrivateConstructor p) {
		super(p);
	}
    
    @QtUninvokable
	public String toString() {
        Iterator<QPair<K,V>> it = iterator();
        if (! it.hasNext())
            return "{}";

        StringBuilder sb = new StringBuilder();
        sb.append('{');
        for (;;) {
        	QPair<K,V> e = it.next();
            sb.append(e.first == this ? "(this Map)" : e.first);
            sb.append(',').append(' ');
            sb.append(e.second == this ? "(this Map)" : e.second);
            if (! it.hasNext())
                return sb.append('}').toString();
            sb.append(';').append(' ');
        }
    }
}
