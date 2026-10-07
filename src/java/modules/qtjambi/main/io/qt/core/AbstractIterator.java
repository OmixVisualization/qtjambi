/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
package io.qt.core;

import java.lang.reflect.*;
import java.util.*;
import java.util.function.*;

import io.qt.*;

abstract class AbstractIterator<T,Container extends QtObjectInterface> extends QtObject implements Cloneable{
    static {
    	QtJambi_LibraryUtilities.initialize();
    }

	private static byte HAS_NEXT = 1;
	private static byte HAS_PREVIOUS = 2;
	private static byte IS_END = 3;
	
	private interface ImplementorInterface<T,Container extends QtObjectInterface>{
		boolean isValid(long nativeId);
		void advance(long nativeId, long n);
		Iterator<T> iterator(QSequentialConstIterator<T,Container> iterator, boolean takeOwnership);
		<Key> Iterator<QPair<Key,T>> iterator(QSequentialConstPairIterator<Key,T,Container> iterator, boolean takeOwnership);
		<Key> Iterator<QPair<Key,T>> iterator(QAssociativeConstIterator<Key,T,Container> iterator, boolean takeOwnership);
		Iterator<T> descendingIterator(QSequentialConstIterator<T,Container> iterator, boolean takeOwnership);
		ListIterator<T> bidirectionalIterator(QSequentialIterator<T,Container> iterator);
		ListIterator<T> bidirectionalIterator(QSequentialConstIterator<T,Container> iterator);
		boolean equals(long nativeId, AbstractIterator<?,?> iterator2);
		int compareTo(long nativeId, AbstractIterator<?,?> iterator2);
		default T value(long nativeId) { return AbstractIterator.value(nativeId); }
		default QMetaType valueType(long nativeId) { return AbstractIterator.valueType(nativeId); }
	}
	
	private static class KeyImplementor<T,Container extends QtObjectInterface> implements ImplementorInterface<T, Container>{
		public KeyImplementor(ImplementorInterface<T, Container> impl) {
			super();
			this.impl = impl;
		}

		private final ImplementorInterface<T,Container> impl;

		public boolean isValid(long nativeId) {
			return impl.isValid(nativeId);
		}

		public void advance(long nativeId, long n) {
			impl.advance(nativeId, n);
		}

		public Iterator<T> iterator(QSequentialConstIterator<T, Container> iterator, boolean takeOwnership) {
			return impl.iterator(iterator, takeOwnership);
		}

		public <Key> Iterator<QPair<Key, T>> iterator(QSequentialConstPairIterator<Key, T, Container> iterator,
				boolean takeOwnership) {
			return impl.iterator(iterator, takeOwnership);
		}

		public <Key> Iterator<QPair<Key, T>> iterator(QAssociativeConstIterator<Key, T, Container> iterator,
				boolean takeOwnership) {
			return impl.iterator(iterator, takeOwnership);
		}

		public Iterator<T> descendingIterator(QSequentialConstIterator<T, Container> iterator, boolean takeOwnership) {
			return impl.descendingIterator(iterator, takeOwnership);
		}

		public ListIterator<T> bidirectionalIterator(QSequentialIterator<T, Container> iterator) {
			return impl.bidirectionalIterator(iterator);
		}

		public ListIterator<T> bidirectionalIterator(QSequentialConstIterator<T, Container> iterator) {
			return impl.bidirectionalIterator(iterator);
		}

		public boolean equals(long nativeId, AbstractIterator<?, ?> iterator2) {
			return impl.equals(nativeId, iterator2);
		}

		public int compareTo(long nativeId, AbstractIterator<?, ?> iterator2) {
			return impl.compareTo(nativeId, iterator2);
		}

		public T value(long nativeId) {
			return QAssociativeConstIterator.key(nativeId);
		}
		
		public QMetaType valueType(long nativeId) { return QAssociativeConstIterator.keyType(nativeId); }
	}
	
	private static class NativeImplementor<T,Container extends QtObjectInterface> implements ImplementorInterface<T,Container>{
		@Override
		public boolean isValid(long nativeId) {
	    	return nativeId!=0 && AbstractIterator.isValid(nativeId);
		}
		
		@Override
		public void advance(long nativeId, long n) {
	    	AbstractIterator.advance(nativeId, n);
		}
	    
	    @QtUninvokable
	    public final <Key> SequentialPairIterator<Key,T,Container> iterator(QSequentialConstPairIterator<Key,T,Container> iterator, boolean takeOwnership){
			return new SequentialPairIterator<>(iterator, takeOwnership);
		}
		
	    @QtUninvokable
	    public final SequentialIterator<T,Container> iterator(QSequentialConstIterator<T,Container> iterator, boolean takeOwnership){
			return new SequentialIterator<>(iterator, takeOwnership);
		}
	    
	    @QtUninvokable
	    public final <Key> AssociativeIterator<Key,T,Container> iterator(QAssociativeConstIterator<Key,T,Container> iterator, boolean takeOwnership){
			return new AssociativeIterator<>(iterator, takeOwnership);
		}
		
	    @QtUninvokable
	    public final DescendingIterator<T,Container> descendingIterator(QSequentialConstIterator<T,Container> iterator, boolean takeOwnership){
			return new DescendingIterator<>(iterator, takeOwnership);
		}

	    @QtUninvokable
	    public final BidirectionalIterator<T,Container> bidirectionalIterator(QSequentialIterator<T,Container> iterator){
	    	return new BidirectionalIterator<>(iterator);
	    }

	    @QtUninvokable
	    public final ConstBidirectionalIterator<T,Container> bidirectionalIterator(QSequentialConstIterator<T,Container> iterator){
	    	return new ConstBidirectionalIterator<>(iterator);
	    }
	    
	    @Override
		public boolean equals(long nativeId, AbstractIterator<?,?> iterator2) {
	    	return AbstractIterator.equals(nativeId, QtJambi_LibraryUtilities.internal.checkedNativeId(iterator2), iterator2);
		}

		@Override
		public int compareTo(long nativeId, AbstractIterator<?,?> iterator2) {
			long nativeId2 = QtJambi_LibraryUtilities.internal.checkedNativeId(iterator2);
			if(AbstractIterator.lessThan(nativeId, nativeId2)) {
				return -1;
			}else if(AbstractIterator.lessThan(nativeId2, nativeId)) {
				return 1;
			}
			return 0;
		}
	    
	    private static class SequentialIterator<E,Container extends QtObjectInterface> implements java.util.Iterator<E>{
	    	final QSequentialConstIterator<E,Container> current;
	    	private byte state;
	    	private final boolean takeOwnership;

	    	SequentialIterator(QSequentialConstIterator<E,Container> current, boolean takeOwnership) {
				super();
				this.current = current;
				this.takeOwnership = takeOwnership;
				state = current.isValid() ? HAS_NEXT : IS_END;
				if(state==IS_END && takeOwnership)
					current.dispose();
			}
	    	
			@Override
			public boolean hasNext() {
				if(state==0)
					advance(QtJambi_LibraryUtilities.internal.nativeId(current));
				return state==HAS_NEXT;
			}

			@Override
			public E next() {
				final long nativeId = QtJambi_LibraryUtilities.internal.nativeId(current);
				if(state==0)
					advance(nativeId);
				if(state==IS_END)
	                throw new NoSuchElementException();
	            state = 0;
	            return ((AbstractIterator<E,Container>)current).impl.value(nativeId);
			}
	    	
			private void advance(final long nativeId) {
				AbstractIterator.increment(nativeId);
				state = AbstractIterator.isValid(nativeId) ? HAS_NEXT : IS_END;
				if(state==IS_END && takeOwnership)
					current.dispose();
			}
	    }
	    
	    private static class DescendingIterator<E,Container extends QtObjectInterface> implements java.util.Iterator<E>{
	    	final QSequentialConstIterator<E,Container> current;
	    	private byte state;
	    	private final boolean takeOwnership;

			public DescendingIterator(QSequentialConstIterator<E,Container> current, boolean takeOwnership) {
				super();
		    	if(!current.isBidirectionalIterator())
		    		throw new UnsupportedOperationException("descendingIterator()");
				this.current = current;
				this.takeOwnership = takeOwnership;
			}
			
			@Override
			public boolean hasNext() {
				if(state==0)
					advance(QtJambi_LibraryUtilities.internal.nativeId(current));
				return state==HAS_NEXT;
			}

			@Override
			public E next() {
				final long nativeId = QtJambi_LibraryUtilities.internal.nativeId(current);
				if(state==0)
					advance(nativeId);
				if(state==IS_END)
	                throw new NoSuchElementException();
	            state = 0;
	            return ((AbstractIterator<E,Container>)current).impl.value(nativeId);
			}
	    	
			private void advance(final long nativeId) {
				state = AbstractIterator.isBegin(nativeId) ? IS_END : HAS_NEXT;
				if(state==HAS_NEXT)
					AbstractIterator.decrement(nativeId);
				else if(state==IS_END && takeOwnership)
					current.dispose();
			}
	    }

	    private static class AssociativeIterator<Key,T,Container extends QtObjectInterface> implements java.util.Iterator<QPair<Key,T>>{
	    	final QAssociativeConstIterator<Key,T,Container> current;
	    	private byte state;
	    	private final boolean takeOwnership;

			AssociativeIterator(QAssociativeConstIterator<Key,T,Container> current, boolean takeOwnership) {
				super();
				this.current = current;
				this.takeOwnership = takeOwnership;
				state = current.isValid() ? HAS_NEXT : IS_END;
				if(state==IS_END && takeOwnership)
					current.dispose();
			}

			@Override
			public boolean hasNext() {
				if(state==0)
					advance(QtJambi_LibraryUtilities.internal.nativeId(current));
				return state==HAS_NEXT;
			}

			@Override
			public QPair<Key,T> next() {
				final long nativeId = QtJambi_LibraryUtilities.internal.nativeId(current);
				if(state==0)
					advance(nativeId);
				if(state==IS_END)
	                throw new NoSuchElementException();
	            state = 0;
	            return new QPair<>(QAssociativeConstIterator.key(nativeId), AbstractIterator.value(nativeId));
			}
	    	
			private void advance(final long nativeId) {
				AbstractIterator.advance(nativeId, 1);
				state = ((AbstractIterator<?,?>)current).impl.isValid(nativeId) ? HAS_NEXT : IS_END;
				if(state==IS_END && takeOwnership)
					current.dispose();
			}
	    }
	    
	    private static class SequentialPairIterator<Key,T,Container extends QtObjectInterface> implements java.util.Iterator<QPair<Key,T>>{
	    	final QSequentialConstPairIterator<Key,T,Container> current;
	    	private byte state;
	    	private final boolean isAssociative;
	    	private final boolean takeOwnership;

	    	SequentialPairIterator(QSequentialConstPairIterator<Key,T,Container> current, boolean takeOwnership) {
				super();
				this.current = current;
				this.takeOwnership = takeOwnership;
				long nativeId = QtJambi_LibraryUtilities.internal.nativeId(current);
				state = AbstractIterator.isValid(nativeId) ? HAS_NEXT : IS_END;
				if(state==IS_END && takeOwnership) {
					current.dispose();
					isAssociative = false;
	    		}else
					isAssociative = AbstractIterator.isAssociative(nativeId);
			}

			@Override
			public boolean hasNext() {
				if(state==0)
					advance(QtJambi_LibraryUtilities.internal.nativeId(current));
				return state==HAS_NEXT;
			}

			@Override
			public QPair<Key,T> next() {
				final long nativeId = QtJambi_LibraryUtilities.internal.nativeId(current);
				if(state==0)
					advance(nativeId);
				if(state==IS_END)
	                throw new NoSuchElementException();
	            state = 0;
	            if(isAssociative) {
	            	return new QPair<>(QAssociativeConstIterator.key(nativeId), AbstractIterator.value(nativeId));
	            }else {
	            	return AbstractIterator.value(nativeId);
	            }
			}
	    	
			private void advance(final long nativeId) {
				AbstractIterator.advance(nativeId, 1);
				state = ((AbstractIterator<?,?>)current).impl.isValid(nativeId) ? HAS_NEXT : IS_END;
				if(state==IS_END && takeOwnership)
					current.dispose();
			}
	    }
		
		private static class BidirectionalIterator<T,Container extends QtObjectInterface> implements java.util.ListIterator<T>{
			final QSequentialIterator<T,Container> current;
	    	private int icursor;
	    	private boolean hasNext;
	    	private boolean hasPrevious;
	    	
	    	BidirectionalIterator(QSequentialIterator<T,Container> current){
	    		if(current.isConstant())
	    			throw new RuntimeException("Cannot produce mutable iterator from read-only iterator.");
	    		this.current = current;
	    		long nativeId = QtJambi_LibraryUtilities.internal.nativeId(current);
		    	if(!isBidirectionalIterator(nativeId))
		    		throw new UnsupportedOperationException("bidirectionalIterator()");
	    		hasNext = !isEnd(nativeId);
	        	hasPrevious = !isBegin(nativeId);
	    	}
	    	
	        @Override
	        public boolean hasNext() {
	        	return hasNext;
	        }

	        @Override
	        public T next() {
	        	if(!hasNext())
	                throw new NoSuchElementException();
	        	long nativeId = QtJambi_LibraryUtilities.internal.nativeId(current);
	        	T e = ((AbstractIterator<T,Container>)current).impl.value(nativeId);
	        	increment(nativeId);
	        	hasNext = !isEnd(nativeId);
	        	hasPrevious = !isBegin(nativeId);
	        	icursor++;
	            return e;
	        }
	        
	        @Override
	        public T previous() {
	        	if(!hasPrevious())
	                throw new NoSuchElementException();
	        	long nativeId = QtJambi_LibraryUtilities.internal.nativeId(current);
	        	decrement(nativeId);
	        	hasNext = !isEnd(nativeId);
	        	hasPrevious = !isBegin(nativeId);
	        	T e = ((AbstractIterator<T,Container>)current).impl.value(nativeId);
	        	icursor--;
	            return e;
	        }
	        
			@Override
	        public void set(T e) {
	        	if(icursor==0)
	        		throw new IndexOutOfBoundsException(-1);
	        	current.advance(-1);
	        	current.set(e);
	        	current.advance();
	        }
	        
	        @Override
	        public int previousIndex() {
	        	return icursor-1;
	        }
	        
	        @Override
	        public int nextIndex() {
	        	return icursor;
	        }
	        
	        @Override
	        public boolean hasPrevious() {
	        	return hasPrevious;
	        }
	        
			@Override
			public void remove() {
				throw new UnsupportedOperationException("remove");
			}

			@Override
			public void add(T e) {
				throw new UnsupportedOperationException("add");
			}
		}
		
		private static class ConstBidirectionalIterator<T,Container extends QtObjectInterface> implements java.util.ListIterator<T>{
			final QSequentialConstIterator<T,Container> current;
	    	private int icursor;
	    	private boolean hasNext;
	    	private boolean hasPrevious;
	    	
	    	ConstBidirectionalIterator(QSequentialConstIterator<T,Container> current){
	    		this.current = current;
	    		long nativeId = QtJambi_LibraryUtilities.internal.nativeId(current);
		    	if(!isBidirectionalIterator(nativeId))
		    		throw new UnsupportedOperationException("bidirectionalIterator()");
	    		hasNext = !isEnd(nativeId);
	        	hasPrevious = !isBegin(nativeId);
	    	}
	    	
	        @Override
	        public boolean hasNext() {
	        	return hasNext;
	        }

	        @Override
	        public T next() {
	        	if(!hasNext())
	                throw new NoSuchElementException();
	        	long nativeId = QtJambi_LibraryUtilities.internal.nativeId(current);
	        	T e = ((AbstractIterator<T,Container>)current).impl.value(nativeId);
	        	increment(nativeId);
	        	hasNext = !isEnd(nativeId);
	        	hasPrevious = !isBegin(nativeId);
	        	icursor++;
	            return e;
	        }
	        
	        @Override
	        public T previous() {
	        	if(!hasPrevious())
	                throw new NoSuchElementException();
	        	long nativeId = QtJambi_LibraryUtilities.internal.nativeId(current);
	        	decrement(nativeId);
	        	hasNext = !isEnd(nativeId);
	        	hasPrevious = !isBegin(nativeId);
	        	T e = ((AbstractIterator<T,Container>)current).impl.value(nativeId);
	        	icursor--;
	            return e;
	        }
	        
			@Override
	        public void set(T e) {
				throw new UnsupportedOperationException("set");
	        }
	        
	        @Override
	        public int previousIndex() {
	        	return icursor-1;
	        }
	        
	        @Override
	        public int nextIndex() {
	        	return icursor;
	        }
	        
	        @Override
	        public boolean hasPrevious() {
	        	return hasPrevious;
	        }
	        
			@Override
			public void remove() {
				throw new UnsupportedOperationException("remove");
			}

			@Override
			public void add(T e) {
				throw new UnsupportedOperationException("add");
			}
		}
	}
	
	private static class JavaImplementor<T, Container extends QtObjectInterface> implements ImplementorInterface<T,Container> {
		
		private static class Factories<Key,T, Container extends QtObjectInterface>{
			private final BiFunction<QSequentialConstIterator<T,Container>, JavaImplementor<T, Container>, java.util.Iterator<T>> sequentialIteratorFactory;
			private final BiFunction<QAssociativeConstIterator<Key,T,Container>, JavaImplementor<T, Container>, java.util.Iterator<QPair<Key,T>>> associativeIteratorFactory;
			private final BiFunction<QSequentialConstPairIterator<Key,T,Container>, JavaImplementor<T, Container>, java.util.Iterator<QPair<Key,T>>> sequentialPairIteratorFactory;
			private final BiFunction<QSequentialConstIterator<T,Container>, JavaImplementor<T, Container>, java.util.Iterator<T>> descendingLteratorFactory;
			private final BiFunction<AbstractIterator<T,Container>, JavaImplementor<T, Container>, java.util.ListIterator<T>> bidirectionalIteratorFactory;
			
			Factories(){
				if(Boolean.getBoolean("io.qt.enable-concurrent-container-modification-check")) {
					associativeIteratorFactory = CheckingAssociativeIterator::new;
					sequentialPairIteratorFactory = CheckingSequentialPairIterator::new;
					sequentialIteratorFactory = CheckingSequentialIterator::new;
					descendingLteratorFactory = CheckingDescendingIterator::new;
					bidirectionalIteratorFactory = CheckingBidirectionalIterator::new;
				}else {
					associativeIteratorFactory = AssociativeIterator::new;
					sequentialPairIteratorFactory = SequentialPairIterator::new;
					sequentialIteratorFactory = SequentialIterator::new;
					descendingLteratorFactory = DescendingIterator::new;
					bidirectionalIteratorFactory = BidirectionalIterator::new;
				}
			}
		    
		    private static class SequentialIterator<E, Container extends QtObjectInterface> implements java.util.Iterator<E>{
		    	final AbstractIterator<E,Container> current;
		    	final JavaImplementor<E, Container> impl;
				final AbstractIterator<E,? extends Container> end;
		    	private byte state;

		    	SequentialIterator(AbstractIterator<E,Container> current, JavaImplementor<E, Container> impl) {
					super();
					this.current = current;
					this.impl = impl;
					end = impl.end();
					state = (end==null || current.equals(end)) ? IS_END : HAS_NEXT;
				}

				@Override
				public boolean hasNext() {
					advance();
					return state==HAS_NEXT;
				}

				@Override
				public E next() {
					advance();
					if(state==IS_END)
		                throw new NoSuchElementException();
		            state = 0;
		            return current.impl.value(QtJambi_LibraryUtilities.internal.nativeId(current));
				}
		    	
				void advance() {
					if(state==0) {
						current.advance();
						state = current.equals(end) ? IS_END : HAS_NEXT;
					}
				}
		    }
		    
		    private static class DescendingIterator<E, Container extends QtObjectInterface> implements java.util.Iterator<E>{
		    	final AbstractIterator<E,Container> current;
		    	final JavaImplementor<E, Container> impl;
		    	final AbstractIterator<E,? extends Container> begin;
		    	private byte state;

				public DescendingIterator(AbstractIterator<E,Container> current, JavaImplementor<E, Container> impl) {
					super();
			    	if(!current.isBidirectionalIterator())
			    		throw new UnsupportedOperationException("descendingIterator()");
					this.current = current;
					this.impl = impl;
					begin = impl.begin();
				}

				@Override
				public boolean hasNext() {
					advance();
					return state==HAS_NEXT;
				}

				@Override
				public E next() {
					advance();
					if(state==IS_END)
		                throw new NoSuchElementException();
		            state = 0;
		            return current.impl.value(QtJambi_LibraryUtilities.internal.nativeId(current));
				}
		    	
				void advance() {
					if(state==0) {
						state = current.equals(begin) ? IS_END : HAS_NEXT;
						if(state==HAS_NEXT)
							current.advance(-1);
					}
				}
		    }
			
			private static class BidirectionalIterator<T, Container extends QtObjectInterface> implements java.util.ListIterator<T>{
				final AbstractIterator<T,Container> current;
		    	final JavaImplementor<T, Container> impl;
				final AbstractIterator<T,? extends Container> begin;
				final AbstractIterator<T,? extends Container> end;
		    	private int icursor;
		    	private boolean hasNext;
		    	private boolean hasPrevious;
		    	
		    	BidirectionalIterator(AbstractIterator<T,Container> current, JavaImplementor<T, Container> impl){
		    		this.current = current;
		    		this.impl = impl;
			    	if(!current.isBidirectionalIterator())
			    		throw new UnsupportedOperationException("bidirectionalIterator()");
		    		begin = impl.begin();
		    		end = impl.end();
		        	hasNext = end!=null && !current.equals(end);
		        	hasPrevious = begin!=null && !current.equals(begin);
		    	}

		        @Override
		        public T next() {
		        	if(!hasNext())
		                throw new NoSuchElementException();
		        	T e = current.impl.value(QtJambi_LibraryUtilities.internal.nativeId(current));
		        	current.advance();
		        	hasNext = end!=null && !current.equals(end);
		        	hasPrevious = begin!=null && !current.equals(begin);
		        	icursor++;
		            return e;
		        }
		        
		        @Override
		        public T previous() {
		        	if(!hasPrevious())
		                throw new NoSuchElementException();
		        	current.advance(-1);
		        	hasNext = end!=null && !current.equals(end);
		        	hasPrevious = begin!=null && !current.equals(begin);
		        	T e = current.impl.value(QtJambi_LibraryUtilities.internal.nativeId(current));
		        	icursor--;
		            return e;
		        }
		        
				@Override
		        public void set(T e) {
					if(current instanceof QSequentialIterator) {
			        	checkIndex();
			        	current.advance(-1);
			        	((QSequentialIterator<T,Container>)current).set(e);
			        	current.advance();
					}else {
						throw new UnsupportedOperationException("set");
					}
		        }
		        
		        @Override
		        public int previousIndex() {
		        	return icursor-1;
		        }
		        
		        @Override
		        public int nextIndex() {
		        	return icursor;
		        }
		    	
		        @Override
		        public boolean hasNext() {
		        	return hasNext;
		        }
		        
		        @Override
		        public boolean hasPrevious() {
		        	return hasPrevious;
		        }
		        
				void checkIndex() {
					if(icursor==0)
		        		throw new IndexOutOfBoundsException(-1);
				}

				@Override
				public void remove() {
					throw new UnsupportedOperationException("remove");
				}

				@Override
				public void add(T e) {
					throw new UnsupportedOperationException("add");
				}
			}
			
			private static class SequentialPairIterator<Key, T, Container extends QtObjectInterface> implements java.util.Iterator<QPair<Key,T>>{
				final QSequentialConstPairIterator<Key,T,Container> current;
		    	final JavaImplementor<T, Container> impl;
		    	final AbstractIterator<T,? extends Container> end;
		    	private byte state;
		    	
		    	SequentialPairIterator(QSequentialConstPairIterator<Key,T,Container> current, JavaImplementor<T, Container> impl) {
					super();
					this.current = current;
					this.impl= impl;
					end = impl.end();
					state = (end==null || current.equals(end)) ? IS_END : HAS_NEXT;
				}

				@Override
				public boolean hasNext() {
					advance();
					return state==HAS_NEXT;
				}

				@Override
				public QPair<Key,T> next() {
					advance();
					if(state==IS_END)
		                throw new NoSuchElementException();
		            state = 0;
		            return current.get();
				}
		    	
				void advance() {
					if(state==0) {
						current.advance();
						state = current.equals(end) ? IS_END : HAS_NEXT;
					}
				}
		    }
			
			private static class AssociativeIterator<Key, T, Container extends QtObjectInterface> implements java.util.Iterator<QPair<Key,T>>{
				final QAssociativeConstIterator<Key,T,Container> current;
		    	final JavaImplementor<T, Container> impl;
		    	final AbstractIterator<T,? extends Container> end;
		    	private byte state;
		    	
				AssociativeIterator(QAssociativeConstIterator<Key,T,Container> current, JavaImplementor<T, Container> impl) {
					super();
					this.current = current;
					this.impl= impl;
					end = impl.end();
					state = (end==null || current.equals(end)) ? IS_END : HAS_NEXT;
				}

				@Override
				public boolean hasNext() {
					advance();
					return state==HAS_NEXT;
				}

				@Override
				public QPair<Key,T> next() {
					advance();
					if(state==IS_END)
		                throw new NoSuchElementException();
		            state = 0;
		            return new QPair<>(current._key(), current._value());
				}
		    	
				void advance() {
					if(state==0) {
						current.advance();
						state = current.equals(end) ? IS_END : HAS_NEXT;
					}
				}
		    }
		    
		    private static class CheckingSequentialIterator<E, Container extends QtObjectInterface> extends SequentialIterator<E, Container>{
		    	CheckingSequentialIterator(AbstractIterator<E,Container> iterator, JavaImplementor<E, Container> nativeIterator) {
					super(iterator, nativeIterator);
				}

		    	@Override
				void advance() {
		        	if(end!=null && !end.equals(impl.end()))
		        		throw new ConcurrentModificationException();
		    		super.advance();
				}
		    }
		    
		    private static class CheckingDescendingIterator<E, Container extends QtObjectInterface> extends DescendingIterator<E, Container>{
		    	public CheckingDescendingIterator(AbstractIterator<E,Container> iterator, JavaImplementor<E, Container> nativeIterator) {
					super(iterator, nativeIterator);
				}

		    	@Override
				void advance() {
		        	if(begin!=null && !begin.equals(impl.begin()))
		        		throw new ConcurrentModificationException();
		    		super.advance();
				}
		    }
			
			private static class CheckingBidirectionalIterator<T, Container extends QtObjectInterface> extends BidirectionalIterator<T, Container>{

				CheckingBidirectionalIterator(AbstractIterator<T,Container> iterator, JavaImplementor<T, Container> current) {
					super(iterator, current);
				}
				
				@Override
				void checkIndex() {
					super.checkIndex();
					if(end!=null && !end.equals(impl.end()))
		        		throw new ConcurrentModificationException();
				}

		    	@Override
				public boolean hasNext() {
		        	if(end!=null && !end.equals(impl.end()))
		        		throw new ConcurrentModificationException();
		    		return super.hasNext();
				}

		    	@Override
				public boolean hasPrevious() {
		        	if(end!=null && !end.equals(impl.end()))
		        		throw new ConcurrentModificationException();
		    		return super.hasPrevious();
				}
			}
			
			private static class CheckingAssociativeIterator<Key, T, Container extends QtObjectInterface> extends AssociativeIterator<Key, T, Container>{
				CheckingAssociativeIterator(QAssociativeConstIterator<Key,T,Container> iterator, JavaImplementor<T, Container> nativeIterator) {
					super(iterator, nativeIterator);
				}

				@Override
				void advance() {
		        	if(end!=null && !end.equals(impl.end()))
		        		throw new ConcurrentModificationException();
		    		super.advance();
				}
			}
			
			private static class CheckingSequentialPairIterator<Key, T, Container extends QtObjectInterface> extends SequentialPairIterator<Key, T, Container>{
				CheckingSequentialPairIterator(QSequentialConstPairIterator<Key,T,Container> iterator, JavaImplementor<T, Container> nativeIterator) {
					super(iterator, nativeIterator);
				}

				@Override
				void advance() {
		        	if(end!=null && !end.equals(impl.end()))
		        		throw new ConcurrentModificationException();
		    		super.advance();
				}
			}
		}
		
		private static final Factories<?,?,?> factories = new Factories<>();

		@SuppressWarnings("unchecked")
		private static <Key, T, Container extends QtObjectInterface> Factories<Key,T, Container> factories(){
			return (Factories<Key, T, Container>)factories;
		}
		
		final Container owner;
		
		public JavaImplementor(Container owner) {
			super();
			this.owner = owner;
		}
		
		@Override
		public final boolean equals(long nativeId, AbstractIterator<?,?> iterator2) {
			if(iterator2.impl instanceof JavaImplementor && owner==((JavaImplementor<?,?>)iterator2.impl).owner) {
				return AbstractIterator.equals(nativeId, QtJambi_LibraryUtilities.internal.checkedNativeId(iterator2), iterator2);
			}
			return false;
		}

		@Override
		public int compareTo(long nativeId, AbstractIterator<?,?> iterator2) {
			if(iterator2.impl instanceof JavaImplementor && owner==((JavaImplementor<?,?>)iterator2.impl).owner) {
				long nativeId2 = QtJambi_LibraryUtilities.internal.checkedNativeId(iterator2);
				if(AbstractIterator.lessThan(nativeId, nativeId2)) {
					return -1;
				}else if(AbstractIterator.lessThan(nativeId2, nativeId)) {
					return 1;
				}
				return 0;
			}
			return -2;
		}

		AbstractIterator<T,? extends Container> end() {
			return null;
		}
		AbstractIterator<T,? extends Container> begin() {
			return null;
		}
	    
	    public final boolean isValid(long nativeId) {
	    	if(nativeId==0)
	    		return false;
	    	AbstractIterator<?,?> end = end();
	    	if(end!=null) {
		    	if(AbstractIterator.canLess(nativeId)) {
			    	try {
			        	return AbstractIterator.lessThan(nativeId, QtJambi_LibraryUtilities.internal.checkedNativeId(end));
					} catch (Exception e) {
					}
		    	}
				return !AbstractIterator.equals(nativeId, QtJambi_LibraryUtilities.internal.checkedNativeId(end), end);
	    	}
	    	else return true;
	    }
	    
	    @Override
		public void advance(long nativeId, long n) {
	    	if(n>0) {
				for(;n>0;--n) {
					if(isValid(nativeId))
						increment(nativeId);
					else throw new NoSuchElementException();
				}
			}else if(n<0) {
				if(!isBidirectionalIterator(nativeId))
					throw new UnsupportedOperationException("retreat");
				for(;n<0;++n) {
					if(isValid(nativeId))
						decrement(nativeId);
					else throw new NoSuchElementException();
				}
			}
		}

		@Override
		public Iterator<T> iterator(QSequentialConstIterator<T,Container> iterator, boolean takeOwnership) {
			Factories<?, T, Container> factories = factories();
			return factories.sequentialIteratorFactory.apply(iterator, this);
		}
		
		@Override
		public <Key> Iterator<QPair<Key,T>> iterator(QSequentialConstPairIterator<Key,T,Container> iterator, boolean takeOwnership) {
			Factories<Key, T, Container> factories = factories();
			return factories.sequentialPairIteratorFactory.apply(iterator, this);
		}
		
		@Override
		public <Key> Iterator<QPair<Key,T>> iterator(QAssociativeConstIterator<Key,T,Container> iterator, boolean takeOwnership) {
			Factories<Key, T, Container> factories = factories();
			return factories.associativeIteratorFactory.apply(iterator, this);
		}

		@Override
		public Iterator<T> descendingIterator(QSequentialConstIterator<T,Container> iterator, boolean takeOwnership) {
			Factories<?, T, Container> factories = factories();
			return factories.descendingLteratorFactory.apply(iterator, this);
		}
		
		@QtUninvokable
	    public final ListIterator<T> bidirectionalIterator(QSequentialIterator<T,Container> iterator){
			Factories<?, T, Container> factories = factories();
    		return factories.bidirectionalIteratorFactory.apply(iterator, this);
	    }
		
		@QtUninvokable
	    public final ListIterator<T> bidirectionalIterator(QSequentialConstIterator<T,Container> iterator){
			Factories<?, T, Container> factories = factories();
    		return factories.bidirectionalIteratorFactory.apply(iterator, this);
	    }
	}
	
	private static class GenericImplementor<T,Container extends QtObjectInterface> extends JavaImplementor<T,Container> {
		private final Function<Container,AbstractIterator<T,Container>> beginSupplier;
		private final Function<Container,AbstractIterator<T,Container>> endSupplier;
		
		private static class BeginEndFunctions<T,Container extends QtObjectInterface>{
			BeginEndFunctions(
					Function<Container, AbstractIterator<T,Container>> constBegin,
					Function<Container, AbstractIterator<T,Container>> constEnd,
					Function<Container, AbstractIterator<T,Container>> begin,
					Function<Container, AbstractIterator<T,Container>> end) {
				super();
				this.begin = begin;
				this.end = end;
				this.constBegin = constBegin;
				this.constEnd = constEnd;
			}
			
			final Function<Container,AbstractIterator<T,Container>> begin;
			final Function<Container,AbstractIterator<T,Container>> end;
			final Function<Container,AbstractIterator<T,Container>> constBegin;
			final Function<Container,AbstractIterator<T,Container>> constEnd;
		}
		
		private static final Map<Class<?>, BeginEndFunctions<?,?>> endMethodHandles = Collections.synchronizedMap(new HashMap<>());
		
		@SuppressWarnings("unchecked")
		private BeginEndFunctions<T,Container> findBeginEndSuppliers(Container beginOwner) {
			return (BeginEndFunctions<T,Container>)endMethodHandles.computeIfAbsent(QtJambi_LibraryUtilities.internal.getClass(beginOwner), cls -> {
				Method beginMethod = null;
				Method constBeginMethod = null;
				Method endMethod = null;
				Method constEndMethod = null;
				while ((endMethod == null && constEndMethod == null
						&& beginMethod == null && constBeginMethod == null) 
						&& cls!=null && cls != QtObject.class) {
					Method methods[] = cls.getDeclaredMethods();
					for (Method method : methods) {
						if (method.getParameterCount() == 0) {
							if(AbstractIterator.class.isAssignableFrom(method.getReturnType())) {
								if(method.getName().equals("begin")) {
									beginMethod = method;
								}else if(method.getName().equals("end")) {
									endMethod = method;
								}else if(method.getName().equals("constEnd")) {
									constEndMethod = method;
								}else if(method.getName().equals("constBegin")) {
									constBeginMethod = method;
								}
							}
						}
						if(endMethod != null && constEndMethod != null
								 && beginMethod != null && constBeginMethod != null) {
							break;
						}
					}
					cls = cls.getSuperclass();
				}
				if(constBeginMethod==null && beginMethod!=null) {
					if(beginMethod.getReturnType()!=QSequentialIterator.class
							&& beginMethod.getReturnType()!=QAssociativeIterator.class) {
						constBeginMethod = beginMethod;
						beginMethod = null;
					}
				}
				if(constEndMethod==null && endMethod!=null) {
					if(endMethod.getReturnType()!=QSequentialIterator.class
							&& endMethod.getReturnType()!=QAssociativeIterator.class) {
						constEndMethod = endMethod;
						endMethod = null;
					}
				}
				if (endMethod != null || constEndMethod != null)
					return new BeginEndFunctions<>(CoreUtility.functionFromMethod(constBeginMethod), 
												CoreUtility.functionFromMethod(constEndMethod), 
												CoreUtility.functionFromMethod(beginMethod), 
												CoreUtility.functionFromMethod(endMethod));
				else return new BeginEndFunctions<>(null,null,null,null);
			});
		}
		
		public GenericImplementor(Container owner, boolean isConstant) {
			super(owner);
			BeginEndFunctions<T,Container> functions = findBeginEndSuppliers(owner);
			beginSupplier = (isConstant ? functions.constBegin : functions.begin);
			endSupplier = (isConstant ? functions.constEnd : functions.end);
		}
		@Override
		final AbstractIterator<T,Container> end(){
			return endSupplier==null ? null : endSupplier.apply(owner);
		}
		@Override
		final AbstractIterator<T,Container> begin(){
			return beginSupplier==null ? null : beginSupplier.apply(owner);
		}
	}
	
	private static class ConstSequentialContainerImplementor<T> extends JavaImplementor<T,AbstractSequentialContainer<T>> {
		public ConstSequentialContainerImplementor(AbstractSequentialContainer<T> container) {
			super(container);
		}
		@Override
		AbstractIterator<T,? extends AbstractSequentialContainer<T>> end() {
			return owner.constEnd();
		}
		@Override
		AbstractIterator<T,? extends AbstractSequentialContainer<T>> begin() {
			return owner.constBegin();
		}
	}
	
	private static class MutableListImplementor<T> extends JavaImplementor<T,AbstractList<T>> {
		public MutableListImplementor(AbstractList<T> container) {
			super(container);
		}
		@Override
		AbstractIterator<T,? extends AbstractList<T>> end() {
			return owner.end();
		}
		@Override
		AbstractIterator<T,? extends AbstractList<T>> begin() {
			return owner.begin();
		}
	}
	
	private static class QConstSpanImplementor<T> extends JavaImplementor<T,AbstractSpan<T>> {
		public QConstSpanImplementor(AbstractSpan<T> container) {
			super(container);
		}
		@Override
		AbstractIterator<T,? extends AbstractSpan<T>> end() {
			return owner.constEnd();
		}
		@Override
		AbstractIterator<T,? extends AbstractSpan<T>> begin() {
			return owner.constBegin();
		}
	}
	
	private static class QSpanImplementor<T> extends JavaImplementor<T,AbstractSpan<T>> {
		public QSpanImplementor(AbstractSpan<T> container) {
			super(container);
		}
		@Override
		AbstractIterator<T,? extends AbstractSpan<T>> end() {
			return owner.end();
		}
		@Override
		AbstractIterator<T,? extends AbstractSpan<T>> begin() {
			return owner.begin();
		}
	}
	
	private static class QByteArrayViewImplementor extends JavaImplementor<Byte,QByteArrayView> {
		public QByteArrayViewImplementor(QByteArrayView container) {
			super(container);
		}
		@Override
		QByteArrayView.ConstIterator end() {
			return owner.end();
		}
		@Override
		QByteArrayView.ConstIterator begin() {
			return owner.begin();
		}
	}
	
	private static class QByteArrayImplementor extends JavaImplementor<Byte,QByteArray> {
		public QByteArrayImplementor(QByteArray container) {
			super(container);
		}
		@Override
		QByteArray.ConstIterator end() {
			return owner.constEnd();
		}
		@Override
		QByteArray.ConstIterator begin() {
			return owner.constBegin();
		}
	}
	
	private static class QStringImplementor extends JavaImplementor<Character,QString> {
		public QStringImplementor(QString container) {
			super(container);
		}
		@Override
		QString.ConstIterator end() {
			return owner.constEnd();
		}
		@Override
		QString.ConstIterator begin() {
			return owner.constBegin();
		}
	}
	
	private static class QFutureImplementor<T> extends JavaImplementor<T,QFuture<T>> {
		public QFutureImplementor(QFuture<T> container) {
			super(container);
		}
		@Override
		QFuture.ConstIterator<T> end() {
			return owner.constEnd();
		}
		@Override
		QFuture.ConstIterator<T> begin() {
			return owner.constBegin();
		}
	}
	
	private static class ConstAssociativeContainerImplementor<Key,T> extends JavaImplementor<T,AbstractAssociativeContainer<Key,T>> {
		public ConstAssociativeContainerImplementor(AbstractAssociativeContainer<Key,T> container) {
			super(container);
		}
		@Override
		AbstractIterator<T,? extends AbstractAssociativeContainer<Key,T>> end() {
			return owner.constEnd();
		}
		@Override
		AbstractIterator<T,? extends AbstractAssociativeContainer<Key,T>> begin() {
			return owner.constBegin();
		}
	}
	
	private static class AssociativeContainerImplementor<Key,T> extends JavaImplementor<T,AbstractAssociativeContainer<Key,T>> {
		public AssociativeContainerImplementor(AbstractAssociativeContainer<Key,T> container) {
			super(container);
		}
		@Override
		AbstractIterator<T,? extends AbstractAssociativeContainer<Key,T>> end() {
			return owner.end();
		}
		@Override
		AbstractIterator<T,? extends AbstractAssociativeContainer<Key,T>> begin() {
			return owner.begin();
		}
	}
	
	private static class ConstMultiAssociativeContainerImplementor<Key,T> extends JavaImplementor<T,AbstractMultiAssociativeContainer<Key,T>> {
		public ConstMultiAssociativeContainerImplementor(AbstractMultiAssociativeContainer<Key,T> container) {
			super(container);
		}
		@Override
		AbstractIterator<T,? extends AbstractMultiAssociativeContainer<Key,T>> end() {
			return owner.constEnd();
		}
		@Override
		AbstractIterator<T,? extends AbstractMultiAssociativeContainer<Key,T>> begin() {
			return owner.constBegin();
		}
	}
	
	private static class MultiAssociativeContainerImplementor<Key,T> extends JavaImplementor<T,AbstractMultiAssociativeContainer<Key,T>> {
		public MultiAssociativeContainerImplementor(AbstractMultiAssociativeContainer<Key,T> container) {
			super(container);
		}
		@Override
		AbstractIterator<T,? extends AbstractMultiAssociativeContainer<Key,T>> end() {
			return owner.end();
		}
		@Override
		AbstractIterator<T,? extends AbstractMultiAssociativeContainer<Key,T>> begin() {
			return owner.begin();
		}
	}
	
	private final ImplementorInterface<T,Container> impl;
	
	AbstractIterator(QPrivateConstructor p){
		super(p);
		impl = new NativeImplementor<>();
	}
	
	@SuppressWarnings("unchecked")
	AbstractIterator(QtConstructInPlace p) {
		super((QPrivateConstructor)null);
		QtObject owner = p.argumentAt(0, QtObject.class);
		if(owner instanceof QByteArray) {
			this.impl = (ImplementorInterface<T,Container>)(ImplementorInterface<?,?>)new QByteArrayImplementor((QByteArray)owner);
		}else if(owner instanceof QByteArrayView) {
			this.impl = (ImplementorInterface<T,Container>)(ImplementorInterface<?,?>)new QByteArrayViewImplementor((QByteArrayView)owner);
		}else if(owner instanceof QString) {
			this.impl = (ImplementorInterface<T,Container>)(ImplementorInterface<?,?>)new QStringImplementor((QString)owner);
		}else if(owner instanceof QFuture) {
			QFuture<T> future = (QFuture<T>)owner;
			impl = (ImplementorInterface<T,Container>)(ImplementorInterface<?,?>)new QFutureImplementor<>(future);
		}else if(owner instanceof AbstractSpan) {
			AbstractSpan<T> span = (AbstractSpan<T>)owner;
			if(span.isConstSpan() || isConstant()) {
				impl = (ImplementorInterface<T,Container>)(ImplementorInterface<?,?>)new QConstSpanImplementor<>(span);
			}else {
				impl = (ImplementorInterface<T,Container>)(ImplementorInterface<?,?>)new QSpanImplementor<>(span);
			}
		}else if(owner instanceof AbstractSequentialContainer) {
			if(isConstant()) {
				AbstractSequentialContainer<T> container = (AbstractSequentialContainer<T>)owner;
				impl = (ImplementorInterface<T,Container>)(ImplementorInterface<?,?>)new ConstSequentialContainerImplementor<>(container);
			}else if(owner instanceof AbstractList) {
				AbstractList<T> container = (AbstractList<T>)owner;
				impl = (ImplementorInterface<T,Container>)(ImplementorInterface<?,?>)new MutableListImplementor<>(container);
			}else {// there are no mutable iterators in QSet
				impl = (ImplementorInterface<T,Container>)(ImplementorInterface<?,?>)new JavaImplementor<>(owner);
			}
		}else if(owner instanceof AbstractAssociativeContainer) {
			AbstractAssociativeContainer<Object,T> container = (AbstractAssociativeContainer<Object,T>)owner;
			if(isConstant()) {
				impl = (ImplementorInterface<T,Container>)(ImplementorInterface<?,?>)new ConstAssociativeContainerImplementor<>(container);
			}else {
				impl = (ImplementorInterface<T,Container>)(ImplementorInterface<?,?>)new AssociativeContainerImplementor<>(container);
			}
		}else if(owner instanceof AbstractMultiAssociativeContainer) {
			AbstractMultiAssociativeContainer<Object,T> container = (AbstractMultiAssociativeContainer<Object,T>)owner;
			if(isConstant()) {
				impl = (ImplementorInterface<T,Container>)(ImplementorInterface<?,?>)new ConstMultiAssociativeContainerImplementor<>(container);
			}else {
				impl = (ImplementorInterface<T,Container>)(ImplementorInterface<?,?>)new MultiAssociativeContainerImplementor<>(container);
			}
		}else {
			impl = (ImplementorInterface<T,Container>)(ImplementorInterface<?,?>)new GenericImplementor<>(owner, isConstant());
		}
	}
	
	AbstractIterator(AbstractIterator<T,Container> other, boolean targetConst, boolean isKeyIterator){
		super((QPrivateConstructor)null);
		if(!canCopy(QtJambi_LibraryUtilities.internal.nativeId(other))) {
			if(other.impl instanceof JavaImplementor) {
				Container owner = ((JavaImplementor<T,Container>)other.impl).owner;
				if(owner!=null)
					throw new RuntimeException(String.format("Unable to clone iterator of %1$s", QtJambi_LibraryUtilities.internal.getClass(owner).getName().replace('$', '.')));
			}
			throw new UnsupportedOperationException("Unable to clone iterator");
		}
		if(isKeyIterator) {
			impl = new KeyImplementor<>(other.impl);
		}else {
			impl = other.impl;
		}
		initialize(this, other, targetConst);
	}
	
	@QtUninvokable
    private static native <T,Container extends QtObjectInterface> void initialize(AbstractIterator<T,Container> iter, AbstractIterator<T,Container> other, boolean targetConst);
	
	/**
	 * Specifies if this type is constant iterator.
	 */
	boolean isConstant() {
		return true;
	}
	
    /**
     * Compares this iterator with other object.
     */
    @Override
    @QtUninvokable
    public boolean equals(Object other) {
        if (other instanceof AbstractIterator) {
        	return equals((AbstractIterator<?,?>) other);
        }
    	return false;
    }
    
    /**
     * Compares this iterator with other object.
     */
    @QtUninvokable
    protected boolean equals(AbstractIterator<?,?> other) {
    	return impl.equals(QtJambi_LibraryUtilities.internal.checkedNativeId(this), other);
    }
    
    /**
     * Compares this iterator with other object.
     */
    @QtUninvokable
    protected int compareTo(AbstractIterator<?,?> other) {
    	return impl.compareTo(QtJambi_LibraryUtilities.internal.checkedNativeId(this), other);
    }
    
    static <Key,T,Container extends QtObjectInterface> java.util.Iterator<QPair<Key,T>> iterator(QSequentialConstPairIterator<Key,T,Container> _this){
    	return iterator(_this, true);
    }
	
	static <Key,T,Container extends QtObjectInterface> java.util.Iterator<QPair<Key,T>> iterator(QSequentialConstPairIterator<Key,T,Container> _this, boolean takeOwnership){
		return ((AbstractIterator<T,Container>)_this).impl.iterator(_this, takeOwnership);
    }
    
    static <T,Container extends QtObjectInterface> java.util.Iterator<T> iterator(QSequentialConstIterator<T,Container> _this){
    	return iterator(_this, true);
    }
	
	static <T,Container extends QtObjectInterface> java.util.Iterator<T> iterator(QSequentialConstIterator<T,Container> _this, boolean takeOwnership){
		return ((AbstractIterator<T,Container>)_this).impl.iterator(_this, takeOwnership);
    }
	
	static <T,Key,Container extends QtObjectInterface> java.util.Iterator<QPair<Key,T>> iterator(QAssociativeConstIterator<Key,T,Container> _this){
		return iterator(_this, true);
	}
	
	static <T,Key,Container extends QtObjectInterface> java.util.Iterator<QPair<Key,T>> iterator(QAssociativeConstIterator<Key,T,Container> _this, boolean takeOwnership){
		return ((AbstractIterator<T,Container>)_this).impl.iterator(_this, takeOwnership);
    }
	
	static <T,Container extends QtObjectInterface> java.util.Iterator<T> descendingIterator(QSequentialConstIterator<T,Container> _this){
		return descendingIterator(_this, true);
	}
    
	static <T,Container extends QtObjectInterface> java.util.Iterator<T> descendingIterator(QSequentialConstIterator<T,Container> _this, boolean takeOwnership){
		return ((AbstractIterator<T,Container>)_this).impl.descendingIterator(_this, takeOwnership);
	}
	
	static <T,Container extends QtObjectInterface> java.util.ListIterator<T> bidirectionalIterator(QSequentialIterator<T,Container> _this){
		return ((AbstractIterator<T,Container>)_this).impl.bidirectionalIterator(_this);
    }
	
	static <T,Container extends QtObjectInterface> java.util.ListIterator<T> bidirectionalIterator(QSequentialConstIterator<T,Container> _this){
		return ((AbstractIterator<T,Container>)_this).impl.bidirectionalIterator(_this);
    }
	
    @QtUninvokable
    private static native void increment(long __this__nativeId);
    @QtUninvokable
    private static native void advance(long __this__nativeId, long n);
    @QtUninvokable
    private static native void decrement(long __this__nativeId);
    @QtUninvokable
    private static native boolean isBidirectionalIterator(long __this__nativeId);
    @QtUninvokable
    static native boolean isAssociative(long __this__nativeId);
    
    boolean isBidirectionalIterator() {
    	return isBidirectionalIterator(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    
    /**
     * Returns the current item's value.
     */
    @QtUninvokable
    final T validValue() {
    	long nativeId = QtJambi_LibraryUtilities.internal.nativeId(this);
    	if(!impl.isValid(nativeId))
    		throw new NoSuchElementException();
		return impl.value(nativeId);
    }
    /**
     * Returns the current item's value.
     */
    @QtUninvokable
    final T _value() {
        return value(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    static native <T> T value(long __this__nativeId);
    
    @QtUninvokable
    QMetaType valueType(){
    	long nativeId = QtJambi_LibraryUtilities.internal.nativeId(this);
		return impl.valueType(nativeId);
    }
    
    @QtUninvokable
    static native QMetaType valueType(long __this__nativeId);

    @Deprecated
    private Optional<T> value() {
    	long nativeId = QtJambi_LibraryUtilities.internal.nativeId(this);
		return nativeId==0 || !impl.isValid(nativeId) ? Optional.empty() : Optional.ofNullable(value(nativeId));
	}
    
    /**
	 * Set the value at iterator's position in the container.
	 * @param newValue the new value
	 */
    @QtUninvokable
	final boolean checkedSetValue(T newValue) {
    	long nativeId = QtJambi_LibraryUtilities.internal.nativeId(this);
    	if(isConstant() || nativeId==0 || !impl.isValid(nativeId))
    		return false;
    	return setValue(nativeId, newValue);
    }
    
    @QtUninvokable
    static native <T> boolean setValue(long __this__nativeId, T newValue);
    
    @QtUninvokable
    private static native boolean isValid(long __this__nativeId);
    @QtUninvokable
    private static native boolean isBegin(long __this__nativeId);
    @QtUninvokable
    private static native boolean isEnd(long __this__nativeId);
    @QtUninvokable
    private static native boolean canLess(long __this__nativeId);
    @QtUninvokable
    private static native boolean canCopy(long __this__nativeId);
    @QtUninvokable
    private static native boolean lessThan(long __this__nativeId, long other);
    
    @QtUninvokable
    final boolean canCopy() {
    	return canCopy(QtJambi_LibraryUtilities.internal.checkedNativeId(this));
    }

    /**
     * Returns {@code true} if iterator is valid, i.e. it is less end.
     */
    @QtUninvokable
    public final boolean isValid() {
    	long nativeId = QtJambi_LibraryUtilities.internal.nativeId(this);
    	return nativeId!=0 && impl.isValid(nativeId);
    }
	
    @QtUninvokable
	private static native int hashCode(long id);
	
    @QtUninvokable
	private static native boolean equals(long id, long other, AbstractIterator<?,?> otherObj);
    
    @QtUninvokable
	private static native String toString(long id);

	@Override
    @QtUninvokable
	public final int hashCode() {
		return hashCode(QtJambi_LibraryUtilities.internal.checkedNativeId(this));
	}

	@Override
    @QtUninvokable
	public final String toString() {
		return toString(QtJambi_LibraryUtilities.internal.checkedNativeId(this));
	}
    
    /**
     * <p>Creates and returns a copy of this object.</p>
     */
    @Override
    public abstract AbstractIterator<T,Container> clone();
	
	@QtUninvokable
    public final void advance() {
		impl.advance(QtJambi_LibraryUtilities.internal.checkedNativeId(this), 1);
	}
	
	@QtUninvokable
    public final void advance(long n) {
		impl.advance(QtJambi_LibraryUtilities.internal.checkedNativeId(this), n);
	}
}
