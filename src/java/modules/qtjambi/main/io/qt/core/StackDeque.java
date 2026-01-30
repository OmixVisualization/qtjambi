/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
package io.qt.core;

import java.util.Collection;
import java.util.Deque;
import java.util.Iterator;
import java.util.Spliterator;
import java.util.function.Consumer;
import java.util.function.Predicate;
import java.util.stream.Stream;

final class StackDeque<T> implements Deque<T>, Cloneable{
    	
    	final QStack<T> stack;

		StackDeque(QStack<T> stack) {
			this.stack = stack;
		}

		@Override
		public boolean isEmpty() {
			return stack.isEmpty();
		}

		@Override
		public Object[] toArray() {
			return stack.toArray();
		}

		@Override
		public <A> A[] toArray(A[] a) {
			return stack.toArray(a);
		}

		@Override
		public boolean containsAll(Collection<?> c) {
			return stack.containsAll(c);
		}

		@Override
		public boolean removeAll(Collection<?> c) {
			return stack.removeAll(c);
		}

		@Override
		public boolean retainAll(Collection<?> c) {
			return stack.retainAll(c);
		}

		@Override
		public void clear() {
			stack.clear();
		}

		@Override
		public void addFirst(T e) {
			stack.addFirst(e);
		}

		@Override
		public void addLast(T e) {
			stack.addLast(e);
		}

		@Override
		public boolean offerFirst(T e) {
			return stack.offerFirst(e);
		}

		@Override
		public boolean offerLast(T e) {
			return stack.offerLast(e);
		}

		@Override
		public T removeFirst() {
			return stack.removeFirst();
		}

		@Override
		public T removeLast() {
			return stack.removeLast();
		}

		@Override
		public T pollFirst() {
			return stack.pollFirst();
		}

		@Override
		public T pollLast() {
			return stack.pollLast();
		}

		@Override
		public T getFirst() {
			return stack.getFirst();
		}

		@Override
		public T getLast() {
			return stack.getLast();
		}

		@Override
		public T peekFirst() {
			return stack.peekFirst();
		}

		@Override
		public T peekLast() {
			return stack.peekLast();
		}

		@Override
		public boolean removeFirstOccurrence(Object o) {
			return stack.removeFirstOccurrence(o);
		}

		@Override
		public boolean removeLastOccurrence(Object o) {
			return stack.removeFirstOccurrence(o);
		}

		@Override
		public boolean add(T e) {
			return stack.add(e);
		}

		@Override
		public boolean offer(T e) {
			return stack.offer(e);
		}

		@Override
		public T remove() {
			return stack.remove();
		}

		@Override
		public T poll() {
			return stack.poll();
		}

		@Override
		public T element() {
			return stack.element();
		}

		@Override
		public T peek() {
			return stack.peek();
		}

		@Override
		public boolean addAll(Collection<? extends T> c) {
			return stack.addAll(c);
		}

		@Override
		public void push(T e) {
			stack.push(e);
		}

		@Override
		public T pop() {
			return stack.pop();
		}

		@Override
		public boolean remove(Object o) {
			return stack.remove(o);
		}

		@Override
		public boolean contains(Object o) {
			return stack.contains(o);
		}

		@Override
		public int size() {
			return stack.size();
		}

		@Override
		public Iterator<T> iterator() {
			return stack.iterator();
		}

		@Override
		public Iterator<T> descendingIterator() {
			return stack.descendingIterator();
		}

		@Override
		public boolean removeIf(Predicate<? super T> filter) {
			return stack.removeIf(filter);
		}

		@Override
		public Spliterator<T> spliterator() {
			return stack.spliterator();
		}

		@Override
		public Stream<T> stream() {
			return stack.stream();
		}

		@Override
		public Stream<T> parallelStream() {
			return stack.parallelStream();
		}

		@Override
		public void forEach(Consumer<? super T> action) {
			stack.forEach(action);
		}

		@Override
		public int hashCode() {
			return stack.hashCode();
		}

		@Override
		public boolean equals(Object obj) {
			return stack.equals(obj);
		}

		@Override
		protected Object clone() throws CloneNotSupportedException {
			return stack.clone();
		}

		@Override
		public String toString() {
			return stack.toString();
		}
    }