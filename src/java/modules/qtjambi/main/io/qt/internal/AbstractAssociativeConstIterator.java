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

import java.util.ConcurrentModificationException;
import java.util.NoSuchElementException;
import java.util.function.Function;

import io.qt.QtObject;
import io.qt.QtUninvokable;
import io.qt.core.QPair;

public abstract class AbstractAssociativeConstIterator<K,V> extends AbstractSequentialConstIterator<V>{
	
	protected AbstractAssociativeConstIterator(QtObject owner) {
		super(owner);
	}
	
	private final static Function<AbstractAssociativeConstIterator<?,?>, java.util.Iterator<?>> iteratorFactory;
	
	static {
		if(Boolean.getBoolean("io.qt.enable-concurrent-container-modification-check")) {
			@SuppressWarnings("unchecked")
			Function<AbstractAssociativeConstIterator<?,?>, java.util.Iterator<?>> _iteratorFactory = CheckingIncrementalIterator::new;
			iteratorFactory = _iteratorFactory;
		}else {
			@SuppressWarnings("unchecked")
			Function<AbstractAssociativeConstIterator<?,?>, java.util.Iterator<?>> _iteratorFactory = IncrementalIterator::new;
			iteratorFactory = _iteratorFactory;
		}
	}
	
	private static class IncrementalIterator<K,V> implements java.util.Iterator<QPair<K,V>>{
    	final AbstractAssociativeConstIterator<K,V> nativeIterator;
    	final AbstractSequentialConstIterator<V> end;
    	private boolean hasNext;
    	
		IncrementalIterator(AbstractAssociativeConstIterator<K, V> nativeIterator) {
			super();
			this.nativeIterator = nativeIterator;
			end = nativeIterator.end();
			hasNext = end!=null && !nativeIterator.equals(end);
		}

		@Override
		public boolean hasNext() {
			return hasNext;
		}

		@Override
		public QPair<K,V> next() {
			checkNext();
			QPair<K,V> e = new QPair<>(nativeIterator.key(), nativeIterator.val());
			nativeIterator.increment();
            hasNext = end!=null && !nativeIterator.equals(end);
            return e;
		}
    	
		void checkNext() {
            if(!hasNext)
                throw new NoSuchElementException();
		}
    }
	
	private static class CheckingIncrementalIterator<K,V> extends IncrementalIterator<K,V>{
		CheckingIncrementalIterator(AbstractAssociativeConstIterator<K, V> nativeIterator) {
			super(nativeIterator);
		}

		void checkNext() {
    		super.checkNext();
        	if(end!=null && !end.equals(nativeIterator.end()))
        		throw new ConcurrentModificationException();
		}
	}

	@SuppressWarnings("unchecked")
	@QtUninvokable
    protected final java.util.Iterator<QPair<K,V>> toJavaMapIterator(){
		return (java.util.Iterator<QPair<K,V>>)iteratorFactory.apply(this);
    }
	
    @QtUninvokable
    protected abstract K checkedKey();
    
    @QtUninvokable
    protected abstract K key();
}
