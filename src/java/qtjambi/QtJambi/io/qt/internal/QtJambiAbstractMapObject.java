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

import java.util.Iterator;
import java.util.Map;

import io.qt.QtObject;
import io.qt.core.QPair;

public abstract class QtJambiAbstractMapObject<K,V> extends QtObject implements Map<K,V>, Iterable<QPair<K,V>> {
	
	@Override
	public abstract int size();

	@Override
	public abstract boolean isEmpty();
	
	public abstract java.util.List<K> keys();
	protected abstract QtJambiMapIteratorObject<K,V> begin();
	protected abstract QtJambiMapIteratorObject<K,V> end();

	@Override
	public void putAll(Map<? extends K, ? extends V> m) {
		for(Map.Entry<? extends K, ? extends V> entry : m.entrySet()) {
			put(entry.getKey(), entry.getValue());
		}
	}

	@Override
	public Iterator<QPair<K,V>> iterator() {
		return begin().toJavaMapIterator(this::end);
	}
	
	@NativeAccess
	private final Class<K> keyType;
	
	@NativeAccess
	private final Class<V> valueType;

    protected QtJambiAbstractMapObject(Class<K> keyType, Class<V> valueType) {
		super();
		this.keyType = keyType;
		this.valueType = valueType;
	}

    protected QtJambiAbstractMapObject(QPrivateConstructor p, Class<K> keyType, Class<V> valueType) {
		super(p);
		this.keyType = keyType;
		this.valueType = valueType;
	}
    
    final Class<K> keyType() {
		return keyType;
	}

    final Class<V> valueType() {
		return valueType;
	}
    
    protected final boolean checkKey(Object key) {
    	if(keyType.isPrimitive()) {
    		return QtJambiInternal.getComplexType(keyType).isInstance(key);
    	}
		return keyType.isInstance(key) || key==null;
	}

    protected final boolean checkValue(Object value) {
    	if(valueType.isPrimitive()) {
    		return QtJambiInternal.getComplexType(valueType).isInstance(value);
    	}
		return valueType.isInstance(value) || value==null;
	}
    
    @SuppressWarnings("unchecked")
	protected final K castKey(Object key) {
    	if(keyType.isPrimitive()) {
    		return (K)QtJambiInternal.getComplexType(keyType).cast(key);
    	}
		return keyType.cast(key);
	}

    @SuppressWarnings("unchecked")
    protected final V castValue(Object value) {
    	if(valueType.isPrimitive()) {
    		return (V)QtJambiInternal.getComplexType(valueType).cast(value);
    	}
		return valueType.cast(value);
	}
	
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
