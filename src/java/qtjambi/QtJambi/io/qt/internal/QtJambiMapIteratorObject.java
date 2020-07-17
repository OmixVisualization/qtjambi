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

import java.util.function.Consumer;
import java.util.function.Function;
import java.util.function.Supplier;

import io.qt.core.QPair;

public abstract class QtJambiMapIteratorObject<K,V> extends QtJambiIteratorObject<V>{
	
	private final Function<QtJambiMapIteratorObject<K,V>,K> key;

	@SuppressWarnings("unchecked")
	protected <T extends QtJambiIteratorObject<?>,Q,S> QtJambiMapIteratorObject(Consumer<T> decrement, Consumer<T> increment, Function<T,Q> key, Function<T,S> value) {
		super(decrement, increment, value);
		this.key = (Function<QtJambiMapIteratorObject<K,V>,K>)key;
	}

    public java.util.Iterator<QPair<K,V>> toJavaMapIterator(Supplier<QtJambiMapIteratorObject<K,V>> endSupplier){
    	return new java.util.Iterator<QPair<K,V>>() {
            @Override
            public boolean hasNext() {
                return !QtJambiMapIteratorObject.this.equals(endSupplier.get());
            }

            @Override
            public QPair<K,V> next() {
                if(!hasNext())
                    throw new java.util.NoSuchElementException();
                QPair<K,V> e = new QPair<>(_key(), _value());
                _increment();
                return e;
            }
        };
    }
    
    K _key() {
    	return key.apply(QtJambiMapIteratorObject.this);
    }
}
