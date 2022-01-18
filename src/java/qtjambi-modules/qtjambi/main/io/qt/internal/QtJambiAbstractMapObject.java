/****************************************************************************
**
** Copyright (C) 2009-2022 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import java.util.Iterator;
import java.util.Map;

import io.qt.QtObject;
import io.qt.QtUninvokable;
import io.qt.core.QPair;

public abstract class QtJambiAbstractMapObject<K,V> extends QtObject implements Map<K,V>, Iterable<QPair<K,V>> {
	
	@Override
    @QtUninvokable
	public abstract int size();

	@Override
    @QtUninvokable
	public abstract boolean isEmpty();
	
    @QtUninvokable
	public abstract java.util.List<K> keys();

    @QtUninvokable
    protected abstract QtJambiMapIteratorObject<K,V> begin();

    @QtUninvokable
	protected abstract QtJambiMapIteratorObject<K,V> end();

	@Override
    @QtUninvokable
	public final void putAll(Map<? extends K, ? extends V> m) {
		for(Map.Entry<? extends K, ? extends V> entry : m.entrySet()) {
			put(entry.getKey(), entry.getValue());
		}
	}

	@Override
    @QtUninvokable
	public final Iterator<QPair<K,V>> iterator() {
		return begin().toJavaMapIterator();
	}
	
    protected QtJambiAbstractMapObject() {
		super();
	}

    protected QtJambiAbstractMapObject(QPrivateConstructor p) {
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
            sb.append('=');
            sb.append(e.second == this ? "(this Map)" : e.second);
            if (! it.hasNext())
                return sb.append('}').toString();
            sb.append(';').append(' ');
        }
    }
}
