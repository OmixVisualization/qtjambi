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

import java.util.Deque;
import java.util.NoSuchElementException;

public abstract class QtJambiLinkedListObject<E> extends QtJambiCollectionObject<E> implements Deque<E> {

	protected QtJambiLinkedListObject(Class<E> elementType) {
		super(elementType);
	}

	protected QtJambiLinkedListObject(QPrivateConstructor p, Class<E> elementType) {
		super(p, elementType);
	}
	
	protected abstract void removeLastImpl();

	@Override
	public E removeFirst() {
		if(size()==0)
			throw new NoSuchElementException();
		return pop();
	}
	
	@Override
	public E remove() {
		return removeFirst();
	}

	@Override
	public E removeLast() {
		if(size()==0)
			throw new NoSuchElementException();
		E e = getLast();
		removeLastImpl();
		return e;
	}
	
	@Override
	public void addFirst(E e) {
		push(castElement(e));
	}

	@Override
	public void addLast(E e) {
		add(castElement(e));
	}

	@Override
	public boolean offerFirst(E e) {
		push(castElement(e));
		return true;
	}

	@Override
	public boolean offerLast(E e) {
		add(castElement(e));
		return true;
	}
	
	@Override
	public E pollFirst() {
		if(size()==0)
			return null;
		return pop();
	}
	
	@Override
	public E pollLast() {
		if(size()==0)
			return null;
		E e = peekLast();
		removeLastImpl();
		return e;
	}

	@Override
	public E getFirst() {
		if(size()==0)
			throw new NoSuchElementException();
		return peekFirst();
	}

	@Override
	public E getLast() {
		if(size()==0)
			throw new NoSuchElementException();
		return peekLast();
	}

	@Override
	public E poll() {
		return pollFirst();
	}
	
	@Override
	public E element() {
		return getFirst();
	}
	
	@Override
	public E peek() {
		return peekFirst();
	}
	
	@Override
	public boolean offer(E e) {
		return offerLast(e);
	}

	@Override
	public boolean removeFirstOccurrence(Object o) {
		return remove(o);
	}

	@Override
	public boolean removeLastOccurrence(Object o) {
		return false;
	}

	@Override
	public java.util.Iterator<E> descendingIterator() {
		return end().toJavaDescendingIterator(this::begin);
	}
}
