/****************************************************************************
**
** Copyright (C) 2009-2024 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import java.util.ArrayList;
import java.util.List;
import java.util.Objects;
import java.util.function.Consumer;
import java.util.logging.Level;
import java.util.logging.Logger;

import io.qt.*;
import io.qt.QtObjectInterface;

/**
 * QScope is an object life-time manager to be used in a try-with-resource block.
 * All object created with this scope are cleaned up (disposed) at the end of the scope.
 * <p>Example:</p>
 * <code>
 * try(QScope scope = new QScope()){<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;QDialog dialog = scope.create(QDialog::new);<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;dialog.exec();<br>
 * }<br>
 * &#47;&#47; dialog is disposed
 * </code>
 */
public final class QScope implements AutoCloseable {
	
	private static final Logger logger = Logger.getLogger("io.qt");
	
	static abstract class AbstractEntry<O>{
		AbstractEntry(O data) {
			this.data = data;
		}
		O data;
		abstract void cleanup();
		void dismiss() {
			data = null;
		}
	}
	
	static class RunningEntry extends AbstractEntry<Runnable>{
		RunningEntry(Runnable data) {
			super(data);
		}

		void cleanup(){
			if(data!=null) {
				try {
					data.run();
				} catch (Throwable e) {
					logger.log(Level.WARNING, "scope cleanup", e);
				}
			}
			data = null;
		}
	}
	
	static class DisposingEntry extends AbstractEntry<QtObjectInterface>{
		DisposingEntry(QtObjectInterface data) {
			super(data);
		}

		void cleanup(){
			if(data!=null) {
				try {
					data.dispose();
				} catch (Throwable e) {
					logger.log(Level.WARNING, "scope cleanup", e);
				}
			}
			data = null;
		}
	}
	
	static class DisposingLaterEntry extends AbstractEntry<QObject>{
		DisposingLaterEntry(QObject data) {
			super(data);
		}

		void cleanup(){
			if(data!=null) {
				try {
					data.disposeLater();
				} catch (Throwable e) {
					logger.log(Level.WARNING, "scope cleanup", e);
				}
			}
			data = null;
		}
	}
	
	static class CleanupEntry<O> extends AbstractEntry<O>{
		CleanupEntry(O data, Consumer<O> cleanup) {
			super(data);
			this.cleanup = Objects.requireNonNull(cleanup);
		}
		private Consumer<O> cleanup;
		void cleanup(){
			if(cleanup!=null) {
				try{
					if(data!=null) {
						try {
							cleanup.accept(data);
						} catch (Throwable e) {
							logger.log(Level.WARNING, "scope cleanup", e);
						}
					}
				}finally{
					cleanup = null;
				}
			}
			data = null;
		}
		void dismiss() {
			super.dismiss();
			cleanup = null;
		}
		Consumer<O> getCleanup() {
			return cleanup;
		}
	}

	private static class Data{
		private final List<AbstractEntry<?>> objects = new ArrayList<>();
		private InternalAccess.Cleanable cleanable;
		
	    @QtUninvokable
		void close(){
	    	for(AbstractEntry<?> e : objects) {
	    		e.cleanup();
	    	}
	    	objects.clear();
		}
	    
	    void addEntry(AbstractEntry<?> entry) {
			if(this.cleanable==null)
				throw new IllegalStateException("Scope has already been closed.");
			objects.add(entry);
	    }
	}

	private final Data data = new Data();
	
	/**
	 * Default constructor
	 */
	public QScope() {
		super();
		this.data.cleanable = QtJambi_LibraryUtilities.internal.registerCleaner(this, this.data::close);
	}
	
	/**
	 * Performs a cleanup on all 
	 */
	@Override
    @QtUninvokable
	public void close(){
		if(this.data.cleanable!=null) {
			this.data.cleanable.clean();
			this.data.cleanable = null;
		}
	}
	
	/**
	 * Removes the given object from this scope.
	 * @param object
	 * @return success
	 */
	@QtUninvokable
	public boolean release(@Nullable Object object) {
		if(this.data.cleanable==null)
			throw new IllegalStateException("Scope has already been closed.");
		for(int i=0; i<this.data.objects.size(); ++i) {
			AbstractEntry<?> entry = this.data.objects.get(i);
			if(entry.data==object) {
				this.data.objects.remove(i);
				entry.dismiss();
				return true;
			}
		}
		return false;
	}
	
	/**
	 * Forces the object's cleanup.
	 * @param object
	 * @return success
	 */
	@QtUninvokable
	public boolean cleanup(@Nullable Object object) {
		if(this.data.cleanable==null)
			throw new IllegalStateException("Scope has already been closed.");
		for(int i=0; i<this.data.objects.size(); ++i) {
			AbstractEntry<?> entry = this.data.objects.get(i);
			if(entry.data==object) {
				this.data.objects.remove(i);
				entry.cleanup();
				return true;
			}
		}
		return false;
	}
	
	/**
	 * Let given action be performed at the end of the scope.
	 * @param action
	 */
	@QtUninvokable
	public @Nullable Runnable capture(@Nullable Runnable action) {
		data.addEntry(new RunningEntry(action));
		return action;
	}
	
	/**
	 * The given object is managed by the scope, i.e. disposed at its closing.
	 * @param object
	 * @return object
	 */
	@QtUninvokable
	public <T extends QtObjectInterface> T capture(T object){
		data.addEntry(new DisposingEntry(object));
		return object;
	}
	
	/**
	 * The given object is managed by the scope, i.e. cleaned up at its closing.
	 * @param cleanup
	 * @param object
	 * @return object
	 */
	@QtUninvokable
	public <T> T capture(@StrictNonNull Consumer<T> cleanup, T object){
		data.addEntry(new CleanupEntry<>(object, cleanup));
		return object;
	}
	
	/**
	 * Factory for argument-less constructors
	 * @param <R> the constructed type
	 */
	@FunctionalInterface
	public static interface Factory<R> {
    	/**
    	 * Creating the object
    	 * @return new object
    	 */
		public R create();
	}
	
	/**
	 * Factory for one argument taking constructors
	 * @param <R> the constructed type
	 */
	@FunctionalInterface
	public static interface Factory1<A, R> {
    	/**
    	 * Creating the object
    	 * @param a
    	 * @return new object
    	 */
		public R create(A a);
	}
	
	/**
	 * Factory for two arguments taking constructors
	 * @param <R> the constructed type
     * @param <A> type of argument 1
     * @param <B> type of argument 2
     */
	@FunctionalInterface
	public static interface Factory2<A, B, R> {
    	/**
    	 * Creating the object
    	 * @param a
    	 * @param b
    	 * @return new object
    	 */
		public R create(A a, B b);
	}
    
	/**
	 * Factory for 3 arguments taking constructors
	 * @param <R> the constructed type
     * @param <A> type of argument 1
     * @param <B> type of argument 2
     * @param <C> type of argument 3
     */
    @FunctionalInterface
	public static interface Factory3<A, B, C, R> {
    	/**
    	 * Creating the object
    	 * @param a
    	 * @param b
    	 * @param c
    	 * @return new object
    	 */
		public R create(A a, B b, C c);
	}
    
	/**
	 * Factory for 4 arguments taking constructors
	 * @param <R> the constructed type
     * @param <A> type of argument 1
     * @param <B> type of argument 2
     * @param <C> type of argument 3
     * @param <D> type of argument 4
     */
    @FunctionalInterface
	public static interface Factory4<A, B, C, D, R>{
    	/**
    	 * Creating the object
    	 * @param a
    	 * @param b
    	 * @param c
    	 * @param d
    	 * @return new object
    	 */
		public R create(A a, B b, C c, D d);
	}
    
	/**
	 * Factory for 5 arguments taking constructors
	 * @param <R> the constructed type
     * @param <A> type of argument 1
     * @param <B> type of argument 2
     * @param <C> type of argument 3
     * @param <D> type of argument 4
     * @param <E> type of argument 5
     */
    @FunctionalInterface
	public static interface Factory5<A, B, C, D, E, R>{
    	/**
    	 * Creating the object
    	 * @param a
    	 * @param b
    	 * @param c
    	 * @param d
    	 * @param e
    	 * @return new object
    	 */
		public R create(A a, B b, C c, D d, E e);
	}
    
	/**
	 * Factory for 6 arguments taking constructors
	 * @param <R> the constructed type
     * @param <A> type of argument 1
     * @param <B> type of argument 2
     * @param <C> type of argument 3
     * @param <D> type of argument 4
     * @param <E> type of argument 5
     * @param <F> type of argument 6
     */
    @FunctionalInterface
	public static interface Factory6<A, B, C, D, E, F, R>{
    	/**
    	 * Creating the object
    	 * @param a
    	 * @param b
    	 * @param c
    	 * @param d
    	 * @param e
    	 * @param f
    	 * @return new object
    	 */
		public R create(A a, B b, C c, D d, E e, F f);
	}
    
	/**
	 * Factory for 7 arguments taking constructors
	 * @param <R> the constructed type
     * @param <A> type of argument 1
     * @param <B> type of argument 2
     * @param <C> type of argument 3
     * @param <D> type of argument 4
     * @param <E> type of argument 5
     * @param <F> type of argument 6
     * @param <G> type of argument 7
     */
    @FunctionalInterface
	public static interface Factory7<A, B, C, D, E, F, G, R>{
    	/**
    	 * Creating the object
    	 * @param a
    	 * @param b
    	 * @param c
    	 * @param d
    	 * @param e
    	 * @param f
    	 * @param g
    	 * @return new object
    	 */
		public R create(A a, B b, C c, D d, E e, F f, G g);
	}
    
	/**
	 * Factory for 8 arguments taking constructors
	 * @param <R> the constructed type
     * @param <A> type of argument 1
     * @param <B> type of argument 2
     * @param <C> type of argument 3
     * @param <D> type of argument 4
     * @param <E> type of argument 5
     * @param <F> type of argument 6
     * @param <G> type of argument 7
     * @param <H> type of argument 8
     */
    @FunctionalInterface
	public static interface Factory8<A, B, C, D, E, F, G, H, R>{
    	/**
    	 * Creating the object
    	 * @param a
    	 * @param b
    	 * @param c
    	 * @param d
    	 * @param e
    	 * @param f
    	 * @param g
    	 * @param h
    	 * @return new object
    	 */
		public R create(A a, B b, C c, D d, E e, F f, G g, H h);
	}
    
	/**
	 * Factory for 9 arguments taking constructors
	 * @param <R> the constructed type
     * @param <A> type of argument 1
     * @param <B> type of argument 2
     * @param <C> type of argument 3
     * @param <D> type of argument 4
     * @param <E> type of argument 5
     * @param <F> type of argument 6
     * @param <G> type of argument 7
     * @param <H> type of argument 8
     * @param <I> type of argument 9
     */
    @FunctionalInterface
	public static interface Factory9<A, B, C, D, E, F, G, H, I, R>{
    	/**
    	 * Creating the object
    	 * @param a
    	 * @param b
    	 * @param c
    	 * @param d
    	 * @param e
    	 * @param f
    	 * @param g
    	 * @param h
    	 * @param i
    	 * @return new object
    	 */
		public R create(A a, B b, C c, D d, E e, F f, G g, H h, I i);
	}
	
    /**
     * Creates a new object being managed by this scope.
     * @param factory constructor
     * @return new scoped object
     */
    public <O extends QtObjectInterface> O create(@StrictNonNull Factory<O> factory) {
    	return capture(factory.create());
    }
    
    /**
     * Creates a new object being managed by this scope.
     * @param factory constructor
     * @return new scoped object
     */
    public <O extends QtObjectInterface,A> O create(@StrictNonNull Factory1<A,O> factory, A arg0) {
    	return capture(factory.create(arg0));
    }
    
    /**
     * Creates a new object being managed by this scope.
     * @param factory constructor
     * @return new scoped object
     */
    public <O extends QtObjectInterface,A,B> O create(@StrictNonNull Factory2<A,B,O> factory, A arg0, B arg1) {
    	return capture(factory.create(arg0, arg1));
    }
    
    /**
     * Creates a new object being managed by this scope.
     * @param factory constructor
     * @return new scoped object
     */
    public <O extends QtObjectInterface,A,B,C> O create(@StrictNonNull Factory3<A,B,C,O> factory, A arg0, B arg1, C arg2) {
    	return capture(factory.create(arg0, arg1, arg2));
    }
    
    /**
     * Creates a new object being managed by this scope.
     * @param factory constructor
     * @return new scoped object
     */
    public <O extends QtObjectInterface,A,B,C,D> O create(@StrictNonNull Factory4<A,B,C,D,O> factory, A arg0, B arg1, C arg2, D arg3) {
    	return capture(factory.create(arg0, arg1, arg2, arg3));
    }
    
    /**
     * Creates a new object being managed by this scope.
     * @param factory constructor
     * @return new scoped object
     */
    public <O extends QtObjectInterface,A,B,C,D,E> O create(@StrictNonNull Factory5<A,B,C,D,E,O> factory, A arg0, B arg1, C arg2, D arg3, E arg4) {
    	return capture(factory.create(arg0, arg1, arg2, arg3, arg4));
    }
    
    /**
     * Creates a new object being managed by this scope.
     * @param factory constructor
     * @return new scoped object
     */
    public <O extends QtObjectInterface,A,B,C,D,E,F> O create(@StrictNonNull Factory6<A,B,C,D,E,F,O> factory, A arg0, B arg1, C arg2, D arg3, E arg4, F arg5) {
    	return capture(factory.create(arg0, arg1, arg2, arg3, arg4, arg5));
    }
    
    /**
     * Creates a new object being managed by this scope.
     * @param factory constructor
     * @return new scoped object
     */
    public <O extends QtObjectInterface,A,B,C,D,E,F,G> O create(@StrictNonNull Factory7<A,B,C,D,E,F,G,O> factory, A arg0, B arg1, C arg2, D arg3, E arg4, F arg5, G arg6) {
    	return capture(factory.create(arg0, arg1, arg2, arg3, arg4, arg5, arg6));
    }
    
    /**
     * Creates a new object being managed by this scope.
     * @param factory constructor
     * @return new scoped object
     */
    public <O extends QtObjectInterface,A,B,C,D,E,F,G,H> O create(@StrictNonNull Factory8<A,B,C,D,E,F,G,H,O> factory, A arg0, B arg1, C arg2, D arg3, E arg4, F arg5, G arg6, H arg7) {
    	return capture(factory.create(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7));
    }
    
    /**
     * Creates a new object being managed by this scope.
     * @param factory constructor
     * @return new scoped object
     */
    public <O extends QtObjectInterface,A,B,C,D,E,F,G,H,I> O create(@StrictNonNull Factory9<A,B,C,D,E,F,G,H,I,O> factory, A arg0, B arg1, C arg2, D arg3, E arg4, F arg5, G arg6, H arg7, I arg8) {
    	return capture(factory.create(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8));
    }
	
    /**
     * Creates a new object being managed by this scope with cleanup function.
     * @param cleanup the destroyer function
     * @param factory constructor
     * @return new scoped object
     */
    public <O> O create(@StrictNonNull Consumer<O> cleanup, @StrictNonNull Factory<O> factory) {
    	return capture(cleanup, factory.create());
    }
    
    /**
     * Creates a new object being managed by this scope with cleanup function.
     * @param cleanup the destroyer function
     * @param factory constructor
     * @return new scoped object
     */
    public <O,A> O create(@StrictNonNull Consumer<O> cleanup, @StrictNonNull Factory1<A,O> factory, A arg0) {
    	return capture(cleanup, factory.create(arg0));
    }
    
    /**
     * Creates a new object being managed by this scope with cleanup function.
     * @param cleanup the destroyer function
     * @param factory constructor
     * @return new scoped object
     */
    public <O,A,B> O create(@StrictNonNull Consumer<O> cleanup, @StrictNonNull Factory2<A,B,O> factory, A arg0, B arg1) {
    	return capture(cleanup, factory.create(arg0, arg1));
    }
    
    /**
     * Creates a new object being managed by this scope with cleanup function.
     * @param cleanup the destroyer function
     * @param factory constructor
     * @return new scoped object
     */
    public <O,A,B,C> O create(@StrictNonNull Consumer<O> cleanup, @StrictNonNull Factory3<A,B,C,O> factory, A arg0, B arg1, C arg2) {
    	return capture(cleanup, factory.create(arg0, arg1, arg2));
    }
    
    /**
     * Creates a new object being managed by this scope with cleanup function.
     * @param cleanup the destroyer function
     * @param factory constructor
     * @return new scoped object
     */
    public <O,A,B,C,D> O create(@StrictNonNull Consumer<O> cleanup, @StrictNonNull Factory4<A,B,C,D,O> factory, A arg0, B arg1, C arg2, D arg3) {
    	return capture(cleanup, factory.create(arg0, arg1, arg2, arg3));
    }
    
    /**
     * Creates a new object being managed by this scope with cleanup function.
     * @param cleanup the destroyer function
     * @param factory constructor
     * @return new scoped object
     */
    public <O,A,B,C,D,E> O create(@StrictNonNull Consumer<O> cleanup, @StrictNonNull Factory5<A,B,C,D,E,O> factory, A arg0, B arg1, C arg2, D arg3, E arg4) {
    	return capture(cleanup, factory.create(arg0, arg1, arg2, arg3, arg4));
    }
    
    /**
     * Creates a new object being managed by this scope with cleanup function.
     * @param cleanup the destroyer function
     * @param factory constructor
     * @return new scoped object
     */
    public <O,A,B,C,D,E,F> O create(@StrictNonNull Consumer<O> cleanup, @StrictNonNull Factory6<A,B,C,D,E,F,O> factory, A arg0, B arg1, C arg2, D arg3, E arg4, F arg5) {
    	return capture(cleanup, factory.create(arg0, arg1, arg2, arg3, arg4, arg5));
    }
    
    /**
     * Creates a new object being managed by this scope with cleanup function.
     * @param cleanup the destroyer function
     * @param factory constructor
     * @return new scoped object
     */
    public <O,A,B,C,D,E,F,G> O create(@StrictNonNull Consumer<O> cleanup, @StrictNonNull Factory7<A,B,C,D,E,F,G,O> factory, A arg0, B arg1, C arg2, D arg3, E arg4, F arg5, G arg6) {
    	return capture(cleanup, factory.create(arg0, arg1, arg2, arg3, arg4, arg5, arg6));
    }
    
    /**
     * Creates a new object being managed by this scope with cleanup function.
     * @param cleanup the destroyer function
     * @param factory constructor
     * @return new scoped object
     */
    public <O,A,B,C,D,E,F,G,H> O create(@StrictNonNull Consumer<O> cleanup, @StrictNonNull Factory8<A,B,C,D,E,F,G,H,O> factory, A arg0, B arg1, C arg2, D arg3, E arg4, F arg5, G arg6, H arg7) {
    	return capture(cleanup, factory.create(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7));
    }
    
    /**
     * Creates a new object being managed by this scope with cleanup function.
     * @param cleanup the destroyer function
     * @param factory constructor
     * @return new scoped object
     */
    public <O,A,B,C,D,E,F,G,H,I> O create(@StrictNonNull Consumer<O> cleanup, @StrictNonNull Factory9<A,B,C,D,E,F,G,H,I,O> factory, A arg0, B arg1, C arg2, D arg3, E arg4, F arg5, G arg6, H arg7, I arg8) {
    	return capture(cleanup, factory.create(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8));
    }
}
