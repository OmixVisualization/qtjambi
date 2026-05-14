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
package io.qt.concurrent;

import java.io.Serializable;

import io.qt.*;
import io.qt.core.*;

/**
 * <p>Used for adjusting task parameters</p>
 * <p>Java wrapper for Qt class <code><a href="https://doc.qt.io/qt/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder&lt;Task,Args&gt;</a></code></p>
 * @since This class was introduced in Qt 6.0.
 */
public abstract class QTaskBuilder<ResultType> extends QtObject implements java.lang.Cloneable{
    static {
        QtJambi_LibraryUtilities.initialize();
    }
    
    @SuppressWarnings("unchecked")
	static <T> Class<T> boxedType(Class<?> elementType){
		if(elementType!=null && elementType.isPrimitive()) {
	    	if(elementType==byte.class) {
	    		elementType = Byte.class;
	    	}else if(elementType==short.class) {
	    		elementType = Short.class;
	    	}else if(elementType==int.class) {
	    		elementType = Integer.class;
	    	}else if(elementType==long.class) {
	    		elementType = Long.class;
	    	}else if(elementType==boolean.class) {
	    		elementType = Boolean.class;
	    	}else if(elementType==char.class) {
	    		elementType = Character.class;
	    	}else if(elementType==float.class) {
	    		elementType = Float.class;
	    	}else if(elementType==double.class) {
	    		elementType = Double.class;
	    	}
		}
		return (Class<T>)elementType;
    }
    
    static <T> void testArgument(Class<T> cls, T value, String arg) {
    	if(cls!=null) {
    		if(!cls.isInstance(value)) {
    			throw new IllegalArgumentException(String.format("Value type mismatch for argument '%1$s'. Expected: %2$s", arg, cls.getTypeName()));
    		}
    	}
    }
    
    /**
     * Constructor for internal use only.
     * @param p expected to be <code>null</code>.
     * @hidden
     */
    @NativeAccess
    QTaskBuilder(QPrivateConstructor p) { super(p); } 

    /**
     * <p>Creates and returns a copy of this object.</p>
     * <p>See <code>QtConcurrent::QTaskBuilder&lt;Task,Args&gt;::<wbr/>QTaskBuilder(QtConcurrent::QTaskBuilder&lt;Task,Args&gt;)</code></p>
     */
    @QtUninvokable
    @Override
    public abstract QTaskBuilder<ResultType> clone();
    
    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qtconcurrent-qtaskbuilder.html#onThreadPool">QtConcurrent::QTaskBuilder&lt;Task,Args&gt;::<wbr/>onThreadPool(QThreadPool&amp;)</a></code></p>
     * <p>This member function returns the object itself.</p>
     * @param newThreadPool
     * @return the object itself
     */
    @QtUninvokable
    public abstract @NonNull QTaskBuilder<ResultType> onThreadPool(@StrictNonNull QThreadPool newThreadPool);
    
    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qtconcurrent-qtaskbuilder.html#spawn">QtConcurrent::QTaskBuilder&lt;Task,Args&gt;::<wbr/>spawn()</a></code></p>
     * @return
     */
    @QtUninvokable
    public @NonNull QFuture<ResultType> spawn(){
    	throw new RuntimeException("Unable to spawn due to missing arguments.");
    }
    
    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qtconcurrent-qtaskbuilder.html#spawn-1">QtConcurrent::QTaskBuilder&lt;Task,Args&gt;::<wbr/>spawn(QtConcurrent::FutureResult)</a></code></p>
     * @param policy
     */
    @QtUninvokable
    public void spawn(io.qt.concurrent.QtConcurrent.@NonNull FutureResult policy){
    	throw new RuntimeException("Unable to spawn due to missing arguments.");
    }
    
    /**
     * <p>See <code><a href="https://doc.qt.io/qt/qtconcurrent-qtaskbuilder.html#withPriority">QtConcurrent::QTaskBuilder&lt;Task,Args&gt;::<wbr/>withPriority(int)</a></code></p>
     * <p>This member function returns the object itself.</p>
     * @param newPriority
     * @return the object itself
     */
    @QtUninvokable
    public abstract @NonNull QTaskBuilder<ResultType> withPriority(int newPriority);
    
    /**
     * <p>Used for adjusting task parameters</p>
     * <p>Java wrapper for Qt class <code><a href="https://doc.qt.io/qt/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder&lt;Task,Args&gt;</a></code></p>
     * @since This class was introduced in Qt 6.0.
     * @param <T> return argument
     * @param <A> first argument
     */
    public static abstract class Params1<T,A> extends QtObject implements java.lang.Cloneable
    {
        static {
            QtJambi_LibraryUtilities.initialize();
        }
        
        Class<A> typeA;
        
        void setTypes(QtFuture.Runnable1<A> fun) {
        	setTypes(QtFuture.Runnable1.class, fun, 0);
        }
        
        void setTypes(QtFuture.Callable1<T,A> fun) {
        	setTypes(QtFuture.Callable1.class, fun, 0);
        }
        
        void setTypes(QtFuture.RunnableWithPromise1<T,A> fun) {
        	setTypes(QtFuture.RunnableWithPromise1.class, fun, 1);
        }
        
        void setTypes(QtFuture.RunnableWithVoidPromise1<A> fun) {
        	setTypes(QtFuture.RunnableWithVoidPromise1.class, fun, 1);
        }
        
        <S extends Serializable> void setTypes(Class<S> cls, S fun, int i) {
        	Class<?>[] types = QtJambi_LibraryUtilities.internal.lambdaClassTypes(cls, fun);
            if(types!=null) {
                typeA = boxedType(types[++i]);
            }
        }
        
        void testArguments(A a) {
        	testArgument(typeA, a, "a");
        }
        
        /**
         * <p>See <code><a href="https://doc.qt.io/qt/qtconcurrent-qtaskbuilder.html#onThreadPool">QtConcurrent::QTaskBuilder&lt;Task,Args&gt;::<wbr/>onThreadPool(QThreadPool&amp;)</a></code></p>
         * <p>This member function returns the object itself.</p>
         * @param newThreadPool
         * @return the object itself
         */
        @QtUninvokable
        public abstract @NonNull Params1<T,A> onThreadPool(io.qt.core.@StrictNonNull QThreadPool newThreadPool);
        
        /**
         * <p>See <code><a href="https://doc.qt.io/qt/qtconcurrent-qtaskbuilder.html#withPriority">QtConcurrent::QTaskBuilder&lt;Task,Args&gt;::<wbr/>withPriority(int)</a></code></p>
         * <p>This member function returns the object itself.</p>
         * @param newPriority
         * @return the object itself
         */
        @QtUninvokable
        public abstract @NonNull Params1<T,A> withPriority(int newPriority);
        
        /**
         * <p>See <code><a href="https://doc.qt.io/qt/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder&lt;Task,Args&gt;::<wbr/>withArguments&lt;ExtraArgs...&gt;(ExtraArgs&amp;&amp;)</a></code></p>
         * @param a
         * @return
         */
        @QtUninvokable
        public abstract @NonNull QTaskBuilder<T> withArguments(A a);
        
        /**
         * Constructor for internal use only.
         * @param p expected to be <code>null</code>.
         * @hidden
         */
        @NativeAccess
        Params1(QPrivateConstructor p) { super(p); } 
        
        
        /**
         * <p>Creates and returns a copy of this object.</p>
         */
        @QtUninvokable
        @Override
        public abstract Params1<T,A> clone();
    }
    
    /**
     * <p>Used for adjusting task parameters</p>
     * <p>Java wrapper for Qt class <code><a href="https://doc.qt.io/qt/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder&lt;Task,Args&gt;</a></code></p>
     * @since This class was introduced in Qt 6.0.
     * @param <T> return argument
     * @param <A> first argument
     * @param <B> second argument
     */
    public static abstract class Params2<T,A,B> extends QtObject implements java.lang.Cloneable
    {
        static {
            QtJambi_LibraryUtilities.initialize();
        }
        
        Class<A> typeA;
        Class<B> typeB;
        
        void setTypes(QtFuture.Runnable2<A,B> fun) {
        	setTypes(QtFuture.Runnable2.class, fun, 0);
        }
        
        void setTypes(QtFuture.Callable2<T,A,B> fun) {
        	setTypes(QtFuture.Callable2.class, fun, 0);
        }
        
        void setTypes(QtFuture.RunnableWithPromise2<T,A,B> fun) {
        	setTypes(QtFuture.RunnableWithPromise2.class, fun, 1);
        }
        
        void setTypes(QtFuture.RunnableWithVoidPromise2<A,B> fun) {
        	setTypes(QtFuture.RunnableWithVoidPromise2.class, fun, 1);
        }
        
        <S extends Serializable> void setTypes(Class<S> cls, S fun, int i) {
        	Class<?>[] types = QtJambi_LibraryUtilities.internal.lambdaClassTypes(cls, fun);
            if(types!=null) {
                typeA = boxedType(types[++i]);
                typeB = boxedType(types[++i]);
            }
        }
        
        void testArguments(A a, B b) {
        	testArgument(typeA, a, "a");
        	testArgument(typeB, b, "b");
        }
        
        /**
         * <p>See <code><a href="https://doc.qt.io/qt/qtconcurrent-qtaskbuilder.html#onThreadPool">QtConcurrent::QTaskBuilder&lt;Task,Args&gt;::<wbr/>onThreadPool(QThreadPool&amp;)</a></code></p>
         * <p>This member function returns the object itself.</p>
         * @param newThreadPool
         * @return the object itself
         */
        @QtUninvokable
        public abstract @NonNull Params2<T,A,B> onThreadPool(io.qt.core.@StrictNonNull QThreadPool newThreadPool);
        
        /**
         * <p>See <code><a href="https://doc.qt.io/qt/qtconcurrent-qtaskbuilder.html#withPriority">QtConcurrent::QTaskBuilder&lt;Task,Args&gt;::<wbr/>withPriority(int)</a></code></p>
         * <p>This member function returns the object itself.</p>
         * @param newPriority
         * @return the object itself
         */
        @QtUninvokable
        public abstract @NonNull Params2<T,A,B> withPriority(int newPriority);
        
        /**
         * <p>See <code><a href="https://doc.qt.io/qt/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder&lt;Task,Args&gt;::<wbr/>withArguments&lt;ExtraArgs...&gt;(ExtraArgs&amp;&amp;)</a></code></p>
         * @param a
         * @param b
         * @return
         */
        @QtUninvokable
        public abstract @NonNull QTaskBuilder<T> withArguments(A a, B b);
        
        /**
         * Constructor for internal use only.
         * @param p expected to be <code>null</code>.
         * @hidden
         */
        @NativeAccess
        Params2(QPrivateConstructor p) { super(p); } 
        
        
        /**
         * <p>Creates and returns a copy of this object.</p>
         */
        @QtUninvokable
        @Override
        public abstract Params2<T,A,B> clone();
    }
    
    /**
     * <p>Used for adjusting task parameters</p>
     * <p>Java wrapper for Qt class <code><a href="https://doc.qt.io/qt/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder&lt;Task,Args&gt;</a></code></p>
     * @since This class was introduced in Qt 6.0.
     * @param <T> return argument
     * @param <A> first argument
     * @param <B> second argument
     * @param <C> third argument
     */
    public static abstract class Params3<T,A,B,C> extends QtObject implements java.lang.Cloneable
    {
        static {
            QtJambi_LibraryUtilities.initialize();
        }
        
        Class<A> typeA;
        Class<B> typeB;
        Class<C> typeC;
        
        void setTypes(QtFuture.Runnable3<A,B,C> fun) {
        	setTypes(QtFuture.Runnable3.class, fun, 0);
        }
        
        void setTypes(QtFuture.Callable3<T,A,B,C> fun) {
        	setTypes(QtFuture.Callable3.class, fun, 0);
        }
        
        void setTypes(QtFuture.RunnableWithPromise3<T,A,B,C> fun) {
        	setTypes(QtFuture.RunnableWithPromise3.class, fun, 1);
        }
        
        void setTypes(QtFuture.RunnableWithVoidPromise3<A,B,C> fun) {
        	setTypes(QtFuture.RunnableWithVoidPromise3.class, fun, 1);
        }
        
        <S extends Serializable> void setTypes(Class<S> cls, S fun, int i) {
        	Class<?>[] types = QtJambi_LibraryUtilities.internal.lambdaClassTypes(cls, fun);
            if(types!=null) {
                typeA = boxedType(types[++i]);
                typeB = boxedType(types[++i]);
                typeC = boxedType(types[++i]);
            }
        }
        
        void testArguments(A a, B b, C c) {
        	testArgument(typeA, a, "a");
        	testArgument(typeB, b, "b");
        	testArgument(typeC, c, "c");
        }
        
        /**
         * <p>See <code><a href="https://doc.qt.io/qt/qtconcurrent-qtaskbuilder.html#onThreadPool">QtConcurrent::QTaskBuilder&lt;Task,Args&gt;::<wbr/>onThreadPool(QThreadPool&amp;)</a></code></p>
         * <p>This member function returns the object itself.</p>
         * @param newThreadPool
         * @return the object itself
         */
        @QtUninvokable
        public abstract @NonNull Params3<T,A,B,C> onThreadPool(io.qt.core.@StrictNonNull QThreadPool newThreadPool);
        
        /**
         * <p>See <code><a href="https://doc.qt.io/qt/qtconcurrent-qtaskbuilder.html#withPriority">QtConcurrent::QTaskBuilder&lt;Task,Args&gt;::<wbr/>withPriority(int)</a></code></p>
         * <p>This member function returns the object itself.</p>
         * @param newPriority
         * @return the object itself
         */
        @QtUninvokable
        public abstract @NonNull Params3<T,A,B,C> withPriority(int newPriority);
        
        /**
         * <p>See <code><a href="https://doc.qt.io/qt/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder&lt;Task,Args&gt;::<wbr/>withArguments&lt;ExtraArgs...&gt;(ExtraArgs&amp;&amp;)</a></code></p>
         * @param a
         * @param b
         * @param c
         * @return
         */
        @QtUninvokable
        public abstract @NonNull QTaskBuilder<T> withArguments(A a, B b, C c);
        
        /**
         * Constructor for internal use only.
         * @param p expected to be <code>null</code>.
         * @hidden
         */
        @NativeAccess
        Params3(QPrivateConstructor p) { super(p); } 
        
        
        /**
         * <p>Creates and returns a copy of this object.</p>
         */
        @QtUninvokable
        @Override
        public abstract Params3<T,A,B,C> clone();
    }
    
    /**
     * <p>Used for adjusting task parameters</p>
     * <p>Java wrapper for Qt class <code><a href="https://doc.qt.io/qt/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder&lt;Task,Args&gt;</a></code></p>
     * @since This class was introduced in Qt 6.0.
     * @param <T> return argument
     * @param <A> first argument
     * @param <B> second argument
     * @param <C> third argument
     * @param <D> forth argument
     */
    public static abstract class Params4<T,A,B,C,D> extends QtObject implements java.lang.Cloneable
    {
        static {
            QtJambi_LibraryUtilities.initialize();
        }
        
        Class<A> typeA;
        Class<B> typeB;
        Class<C> typeC;
        Class<D> typeD;
        
        void setTypes(QtFuture.Runnable4<A,B,C,D> fun) {
        	setTypes(QtFuture.Runnable4.class, fun, 0);
        }
        
        void setTypes(QtFuture.Callable4<T,A,B,C,D> fun) {
        	setTypes(QtFuture.Callable4.class, fun, 0);
        }
        
        void setTypes(QtFuture.RunnableWithPromise4<T,A,B,C,D> fun) {
        	setTypes(QtFuture.RunnableWithPromise4.class, fun, 1);
        }
        
        void setTypes(QtFuture.RunnableWithVoidPromise4<A,B,C,D> fun) {
        	setTypes(QtFuture.RunnableWithVoidPromise4.class, fun, 1);
        }
        
        <S extends Serializable> void setTypes(Class<S> cls, S fun, int i) {
        	Class<?>[] types = QtJambi_LibraryUtilities.internal.lambdaClassTypes(cls, fun);
            if(types!=null) {
                typeA = boxedType(types[++i]);
                typeB = boxedType(types[++i]);
                typeC = boxedType(types[++i]);
                typeD = boxedType(types[++i]);
            }
        }
        
        void testArguments(A a, B b, C c, D d) {
        	testArgument(typeA, a, "a");
        	testArgument(typeB, b, "b");
        	testArgument(typeC, c, "c");
        	testArgument(typeD, d, "d");
        }
        
        /**
         * <p>See <code><a href="https://doc.qt.io/qt/qtconcurrent-qtaskbuilder.html#onThreadPool">QtConcurrent::QTaskBuilder&lt;Task,Args&gt;::<wbr/>onThreadPool(QThreadPool&amp;)</a></code></p>
         * <p>This member function returns the object itself.</p>
         * @param newThreadPool
         * @return the object itself
         */
        @QtUninvokable
        public abstract @NonNull Params4<T,A,B,C,D> onThreadPool(io.qt.core.@StrictNonNull QThreadPool newThreadPool);
        
        /**
         * <p>See <code><a href="https://doc.qt.io/qt/qtconcurrent-qtaskbuilder.html#withPriority">QtConcurrent::QTaskBuilder&lt;Task,Args&gt;::<wbr/>withPriority(int)</a></code></p>
         * <p>This member function returns the object itself.</p>
         * @param newPriority
         * @return the object itself
         */
        @QtUninvokable
        public abstract @NonNull Params4<T,A,B,C,D> withPriority(int newPriority);
        
        /**
         * <p>See <code><a href="https://doc.qt.io/qt/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder&lt;Task,Args&gt;::<wbr/>withArguments&lt;ExtraArgs...&gt;(ExtraArgs&amp;&amp;)</a></code></p>
         * @param a
         * @param b
         * @param c
         * @param d
         * @return
         */
        @QtUninvokable
        public abstract @NonNull QTaskBuilder<T> withArguments(A a, B b, C c, D d);
        
        /**
         * Constructor for internal use only.
         * @param p expected to be <code>null</code>.
         * @hidden
         */
        @NativeAccess
        Params4(QPrivateConstructor p) { super(p); } 
        
        
        /**
         * <p>Creates and returns a copy of this object.</p>
         */
        @QtUninvokable
        @Override
        public abstract Params4<T,A,B,C,D> clone();
    }
    
    /**
     * <p>Used for adjusting task parameters</p>
     * <p>Java wrapper for Qt class <code><a href="https://doc.qt.io/qt/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder&lt;Task,Args&gt;</a></code></p>
     * @since This class was introduced in Qt 6.0.
     * @param <T> return argument
     * @param <A> first argument
     * @param <B> second argument
     * @param <C> third argument
     * @param <D> forth argument
     * @param <E> fifth argument
     */
    public static abstract class Params5<T,A,B,C,D,E> extends QtObject implements java.lang.Cloneable
    {
        static {
            QtJambi_LibraryUtilities.initialize();
        }
        
        Class<A> typeA;
        Class<B> typeB;
        Class<C> typeC;
        Class<D> typeD;
        Class<E> typeE;
        
        void setTypes(QtFuture.Runnable5<A,B,C,D,E> fun) {
        	setTypes(QtFuture.Runnable5.class, fun, 0);
        }
        
        void setTypes(QtFuture.Callable5<T,A,B,C,D,E> fun) {
        	setTypes(QtFuture.Callable5.class, fun, 0);
        }
        
        void setTypes(QtFuture.RunnableWithPromise5<T,A,B,C,D,E> fun) {
        	setTypes(QtFuture.RunnableWithPromise5.class, fun, 1);
        }
        
        void setTypes(QtFuture.RunnableWithVoidPromise5<A,B,C,D,E> fun) {
        	setTypes(QtFuture.RunnableWithVoidPromise5.class, fun, 1);
        }
        
        <S extends Serializable> void setTypes(Class<S> cls, S fun, int i) {
        	Class<?>[] types = QtJambi_LibraryUtilities.internal.lambdaClassTypes(cls, fun);
            if(types!=null) {
                typeA = boxedType(types[++i]);
                typeB = boxedType(types[++i]);
                typeC = boxedType(types[++i]);
                typeD = boxedType(types[++i]);
                typeE = boxedType(types[++i]);
            }
        }
        
        void testArguments(A a, B b, C c, D d, E e) {
        	testArgument(typeA, a, "a");
        	testArgument(typeB, b, "b");
        	testArgument(typeC, c, "c");
        	testArgument(typeD, d, "d");
        	testArgument(typeE, e, "e");
        }
        
        /**
         * <p>See <code><a href="https://doc.qt.io/qt/qtconcurrent-qtaskbuilder.html#onThreadPool">QtConcurrent::QTaskBuilder&lt;Task,Args&gt;::<wbr/>onThreadPool(QThreadPool&amp;)</a></code></p>
         * <p>This member function returns the object itself.</p>
         * @param newThreadPool
         * @return the object itself
         */
        @QtUninvokable
        public abstract @NonNull Params5<T,A,B,C,D,E> onThreadPool(io.qt.core.@StrictNonNull QThreadPool newThreadPool);
        
        /**
         * <p>See <code><a href="https://doc.qt.io/qt/qtconcurrent-qtaskbuilder.html#withPriority">QtConcurrent::QTaskBuilder&lt;Task,Args&gt;::<wbr/>withPriority(int)</a></code></p>
         * <p>This member function returns the object itself.</p>
         * @param newPriority
         * @return the object itself
         */
        @QtUninvokable
        public abstract @NonNull Params5<T,A,B,C,D,E> withPriority(int newPriority);
        
        /**
         * <p>See <code><a href="https://doc.qt.io/qt/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder&lt;Task,Args&gt;::<wbr/>withArguments&lt;ExtraArgs...&gt;(ExtraArgs&amp;&amp;)</a></code></p>
         * @param a
         * @param b
         * @param c
         * @param d
         * @param e
         * @return
         */
        @QtUninvokable
        public abstract @NonNull QTaskBuilder<T> withArguments(A a, B b, C c, D d, E e);
        
        /**
         * Constructor for internal use only.
         * @param p expected to be <code>null</code>.
         * @hidden
         */
        @NativeAccess
        Params5(QPrivateConstructor p) { super(p); } 
        
        
        /**
         * <p>Creates and returns a copy of this object.</p>
         */
        @QtUninvokable
        @Override
        public abstract Params5<T,A,B,C,D,E> clone();
    }
    
    /**
     * <p>Used for adjusting task parameters</p>
     * <p>Java wrapper for Qt class <code><a href="https://doc.qt.io/qt/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder&lt;Task,Args&gt;</a></code></p>
     * @since This class was introduced in Qt 6.0.
     * @param <T> return argument
     * @param <A> first argument
     * @param <B> second argument
     * @param <C> third argument
     * @param <D> forth argument
     * @param <E> fifth argument
     * @param <F> sixth argument
     */
    public static abstract class Params6<T,A,B,C,D,E,F> extends QtObject implements java.lang.Cloneable
    {
        static {
            QtJambi_LibraryUtilities.initialize();
        }
        
        Class<A> typeA;
        Class<B> typeB;
        Class<C> typeC;
        Class<D> typeD;
        Class<E> typeE;
        Class<F> typeF;
        
        void setTypes(QtFuture.Runnable6<A,B,C,D,E,F> fun) {
        	setTypes(QtFuture.Runnable6.class, fun, 0);
        }
        
        void setTypes(QtFuture.Callable6<T,A,B,C,D,E,F> fun) {
        	setTypes(QtFuture.Callable6.class, fun, 0);
        }
        
        void setTypes(QtFuture.RunnableWithPromise6<T,A,B,C,D,E,F> fun) {
        	setTypes(QtFuture.RunnableWithPromise6.class, fun, 1);
        }
        
        void setTypes(QtFuture.RunnableWithVoidPromise6<A,B,C,D,E,F> fun) {
        	setTypes(QtFuture.RunnableWithVoidPromise6.class, fun, 1);
        }
        
        <S extends Serializable> void setTypes(Class<S> cls, S fun, int i) {
        	Class<?>[] types = QtJambi_LibraryUtilities.internal.lambdaClassTypes(cls, fun);
            if(types!=null) {
                typeA = boxedType(types[++i]);
                typeB = boxedType(types[++i]);
                typeC = boxedType(types[++i]);
                typeD = boxedType(types[++i]);
                typeE = boxedType(types[++i]);
                typeF = boxedType(types[++i]);
            }
        }
        
        void testArguments(A a, B b, C c, D d, E e, F f) {
        	testArgument(typeA, a, "a");
        	testArgument(typeB, b, "b");
        	testArgument(typeC, c, "c");
        	testArgument(typeD, d, "d");
        	testArgument(typeE, e, "e");
        	testArgument(typeF, f, "f");
        }
        
        /**
         * <p>See <code><a href="https://doc.qt.io/qt/qtconcurrent-qtaskbuilder.html#onThreadPool">QtConcurrent::QTaskBuilder&lt;Task,Args&gt;::<wbr/>onThreadPool(QThreadPool&amp;)</a></code></p>
         * <p>This member function returns the object itself.</p>
         * @param newThreadPool
         * @return the object itself
         */
        @QtUninvokable
        public abstract @NonNull Params6<T,A,B,C,D,E,F> onThreadPool(io.qt.core.@StrictNonNull QThreadPool newThreadPool);
        
        /**
         * <p>See <code><a href="https://doc.qt.io/qt/qtconcurrent-qtaskbuilder.html#withPriority">QtConcurrent::QTaskBuilder&lt;Task,Args&gt;::<wbr/>withPriority(int)</a></code></p>
         * <p>This member function returns the object itself.</p>
         * @param newPriority
         * @return the object itself
         */
        @QtUninvokable
        public abstract @NonNull Params6<T,A,B,C,D,E,F> withPriority(int newPriority);
        
        /**
         * <p>See <code><a href="https://doc.qt.io/qt/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder&lt;Task,Args&gt;::<wbr/>withArguments&lt;ExtraArgs...&gt;(ExtraArgs&amp;&amp;)</a></code></p>
         * @param a
         * @param b
         * @param c
         * @param d
         * @param e
         * @param f
         * @return
         */
        @QtUninvokable
        public abstract @NonNull QTaskBuilder<T> withArguments(A a, B b, C c, D d, E e, F f);
        
        /**
         * Constructor for internal use only.
         * @param p expected to be <code>null</code>.
         * @hidden
         */
        @NativeAccess
        Params6(QPrivateConstructor p) { super(p); } 
        
        
        /**
         * <p>Creates and returns a copy of this object.</p>
         */
        @QtUninvokable
        @Override
        public abstract Params6<T,A,B,C,D,E,F> clone();
    }
    
    /**
     * <p>Used for adjusting task parameters</p>
     * <p>Java wrapper for Qt class <code><a href="https://doc.qt.io/qt/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder&lt;Task,Args&gt;</a></code></p>
     * @since This class was introduced in Qt 6.0.
     * @param <T> return argument
     * @param <A> first argument
     * @param <B> second argument
     * @param <C> third argument
     * @param <D> forth argument
     * @param <E> fifth argument
     * @param <F> sixth argument
     * @param <G> seventh argument
     */
    public static abstract class Params7<T,A,B,C,D,E,F,G> extends QtObject implements java.lang.Cloneable
    {
        static {
            QtJambi_LibraryUtilities.initialize();
        }
        
        Class<A> typeA;
        Class<B> typeB;
        Class<C> typeC;
        Class<D> typeD;
        Class<E> typeE;
        Class<F> typeF;
        Class<G> typeG;
        
        void setTypes(QtFuture.Runnable7<A,B,C,D,E,F,G> fun) {
        	setTypes(QtFuture.Runnable7.class, fun, 0);
        }
        
        void setTypes(QtFuture.Callable7<T,A,B,C,D,E,F,G> fun) {
        	setTypes(QtFuture.Callable7.class, fun, 0);
        }
        
        void setTypes(QtFuture.RunnableWithPromise7<T,A,B,C,D,E,F,G> fun) {
        	setTypes(QtFuture.RunnableWithPromise7.class, fun, 1);
        }
        
        void setTypes(QtFuture.RunnableWithVoidPromise7<A,B,C,D,E,F,G> fun) {
        	setTypes(QtFuture.RunnableWithVoidPromise7.class, fun, 1);
        }
        
        <S extends Serializable> void setTypes(Class<S> cls, S fun, int i) {
        	Class<?>[] types = QtJambi_LibraryUtilities.internal.lambdaClassTypes(cls, fun);
            if(types!=null) {
                typeA = boxedType(types[++i]);
                typeB = boxedType(types[++i]);
                typeC = boxedType(types[++i]);
                typeD = boxedType(types[++i]);
                typeE = boxedType(types[++i]);
                typeF = boxedType(types[++i]);
                typeG = boxedType(types[++i]);
            }
        }
        
        void testArguments(A a, B b, C c, D d, E e, F f, G g) {
        	testArgument(typeA, a, "a");
        	testArgument(typeB, b, "b");
        	testArgument(typeC, c, "c");
        	testArgument(typeD, d, "d");
        	testArgument(typeE, e, "e");
        	testArgument(typeF, f, "f");
        	testArgument(typeG, g, "g");
        }
        
        /**
         * <p>See <code><a href="https://doc.qt.io/qt/qtconcurrent-qtaskbuilder.html#onThreadPool">QtConcurrent::QTaskBuilder&lt;Task,Args&gt;::<wbr/>onThreadPool(QThreadPool&amp;)</a></code></p>
         * <p>This member function returns the object itself.</p>
         * @param newThreadPool
         * @return the object itself
         */
        @QtUninvokable
        public abstract @NonNull Params7<T,A,B,C,D,E,F,G> onThreadPool(io.qt.core.@StrictNonNull QThreadPool newThreadPool);
        
        /**
         * <p>See <code><a href="https://doc.qt.io/qt/qtconcurrent-qtaskbuilder.html#withPriority">QtConcurrent::QTaskBuilder&lt;Task,Args&gt;::<wbr/>withPriority(int)</a></code></p>
         * <p>This member function returns the object itself.</p>
         * @param newPriority
         * @return the object itself
         */
        @QtUninvokable
        public abstract @NonNull Params7<T,A,B,C,D,E,F,G> withPriority(int newPriority);
        
        /**
         * <p>See <code><a href="https://doc.qt.io/qt/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder&lt;Task,Args&gt;::<wbr/>withArguments&lt;ExtraArgs...&gt;(ExtraArgs&amp;&amp;)</a></code></p>
         * @param a
         * @param b
         * @param c
         * @param d
         * @param e
         * @param f
         * @param g
         * @return
         */
        @QtUninvokable
        public abstract @NonNull QTaskBuilder<T> withArguments(A a, B b, C c, D d, E e, F f, G g);
        
        /**
         * Constructor for internal use only.
         * @param p expected to be <code>null</code>.
         * @hidden
         */
        @NativeAccess
        Params7(QPrivateConstructor p) { super(p); } 
        
        
        /**
         * <p>Creates and returns a copy of this object.</p>
         */
        @QtUninvokable
        @Override
        public abstract Params7<T,A,B,C,D,E,F,G> clone();
    }
    
    /**
     * <p>Used for adjusting task parameters</p>
     * <p>Java wrapper for Qt class <code><a href="https://doc.qt.io/qt/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder&lt;Task,Args&gt;</a></code></p>
     * @since This class was introduced in Qt 6.0.
     * @param <T> return argument
     * @param <A> first argument
     * @param <B> second argument
     * @param <C> third argument
     * @param <D> forth argument
     * @param <E> fifth argument
     * @param <F> sixth argument
     * @param <G> seventh argument
     * @param <H> eighth argument
     */
    public static abstract class Params8<T,A,B,C,D,E,F,G,H> extends QtObject implements java.lang.Cloneable
    {
        static {
            QtJambi_LibraryUtilities.initialize();
        }
        
        Class<A> typeA;
        Class<B> typeB;
        Class<C> typeC;
        Class<D> typeD;
        Class<E> typeE;
        Class<F> typeF;
        Class<G> typeG;
        Class<H> typeH;
        
        void setTypes(QtFuture.Runnable8<A,B,C,D,E,F,G,H> fun) {
        	setTypes(QtFuture.Runnable8.class, fun, 0);
        }
        
        void setTypes(QtFuture.Callable8<T,A,B,C,D,E,F,G,H> fun) {
        	setTypes(QtFuture.Callable8.class, fun, 0);
        }
        
        void setTypes(QtFuture.RunnableWithPromise8<T,A,B,C,D,E,F,G,H> fun) {
        	setTypes(QtFuture.RunnableWithPromise8.class, fun, 1);
        }
        
        void setTypes(QtFuture.RunnableWithVoidPromise8<A,B,C,D,E,F,G,H> fun) {
        	setTypes(QtFuture.RunnableWithVoidPromise8.class, fun, 1);
        }
        
        <S extends Serializable> void setTypes(Class<S> cls, S fun, int i) {
        	Class<?>[] types = QtJambi_LibraryUtilities.internal.lambdaClassTypes(cls, fun);
            if(types!=null) {
                typeA = boxedType(types[++i]);
                typeB = boxedType(types[++i]);
                typeC = boxedType(types[++i]);
                typeD = boxedType(types[++i]);
                typeE = boxedType(types[++i]);
                typeF = boxedType(types[++i]);
                typeG = boxedType(types[++i]);
                typeH = boxedType(types[++i]);
            }
        }
        
        void testArguments(A a, B b, C c, D d, E e, F f, G g, H h) {
        	testArgument(typeA, a, "a");
        	testArgument(typeB, b, "b");
        	testArgument(typeC, c, "c");
        	testArgument(typeD, d, "d");
        	testArgument(typeE, e, "e");
        	testArgument(typeF, f, "f");
        	testArgument(typeG, g, "g");
        	testArgument(typeH, h, "h");
        }
        
        /**
         * <p>See <code><a href="https://doc.qt.io/qt/qtconcurrent-qtaskbuilder.html#onThreadPool">QtConcurrent::QTaskBuilder&lt;Task,Args&gt;::<wbr/>onThreadPool(QThreadPool&amp;)</a></code></p>
         * <p>This member function returns the object itself.</p>
         * @param newThreadPool
         * @return the object itself
         */
        @QtUninvokable
        public abstract @NonNull Params8<T,A,B,C,D,E,F,G,H> onThreadPool(io.qt.core.@StrictNonNull QThreadPool newThreadPool);
        
        /**
         * <p>See <code><a href="https://doc.qt.io/qt/qtconcurrent-qtaskbuilder.html#withPriority">QtConcurrent::QTaskBuilder&lt;Task,Args&gt;::<wbr/>withPriority(int)</a></code></p>
         * <p>This member function returns the object itself.</p>
         * @param newPriority
         * @return the object itself
         */
        @QtUninvokable
        public abstract @NonNull Params8<T,A,B,C,D,E,F,G,H> withPriority(int newPriority);
        
        /**
         * <p>See <code><a href="https://doc.qt.io/qt/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder&lt;Task,Args&gt;::<wbr/>withArguments&lt;ExtraArgs...&gt;(ExtraArgs&amp;&amp;)</a></code></p>
         * @param a
         * @param b
         * @param c
         * @param d
         * @param e
         * @param f
         * @param g
         * @param h
         * @return
         */
        @QtUninvokable
        public abstract @NonNull QTaskBuilder<T> withArguments(A a, B b, C c, D d, E e, F f, G g, H h);
        
        /**
         * Constructor for internal use only.
         * @param p expected to be <code>null</code>.
         * @hidden
         */
        @NativeAccess
        Params8(QPrivateConstructor p) { super(p); } 
        
        
        /**
         * <p>Creates and returns a copy of this object.</p>
         */
        @QtUninvokable
        @Override
        public abstract Params8<T,A,B,C,D,E,F,G,H> clone();
    }
    
    /**
     * <p>Used for adjusting task parameters</p>
     * <p>Java wrapper for Qt class <code><a href="https://doc.qt.io/qt/qtconcurrent-qtaskbuilder.html">QtConcurrent::QTaskBuilder&lt;Task,Args&gt;</a></code></p>
     * @since This class was introduced in Qt 6.0.
     * @param <T> return argument
     * @param <A> first argument
     * @param <B> second argument
     * @param <C> third argument
     * @param <D> forth argument
     * @param <E> fifth argument
     * @param <F> sixth argument
     * @param <G> seventh argument
     * @param <H> eighth argument
     * @param <I> ninth argument
     */
    public static abstract class Params9<T,A,B,C,D,E,F,G,H,I> extends QtObject implements java.lang.Cloneable
    {
        static {
            QtJambi_LibraryUtilities.initialize();
        }
        
        Class<A> typeA;
        Class<B> typeB;
        Class<C> typeC;
        Class<D> typeD;
        Class<E> typeE;
        Class<F> typeF;
        Class<G> typeG;
        Class<H> typeH;
        Class<I> typeI;
        
        void setTypes(QtFuture.Runnable9<A,B,C,D,E,F,G,H,I> fun) {
        	setTypes(QtFuture.Runnable9.class, fun, 0);
        }
        
        void setTypes(QtFuture.Callable9<T, A,B,C,D,E,F,G,H,I> fun) {
        	setTypes(QtFuture.Callable9.class, fun, 0);
        }
        
        void setTypes(QtFuture.RunnableWithPromise9<T,A,B,C,D,E,F,G,H,I> fun) {
        	setTypes(QtFuture.RunnableWithPromise9.class, fun, 1);
        }
        
        void setTypes(QtFuture.RunnableWithVoidPromise9<A,B,C,D,E,F,G,H,I> fun) {
        	setTypes(QtFuture.RunnableWithVoidPromise9.class, fun, 1);
        }
        
        <S extends Serializable> void setTypes(Class<S> cls, S fun, int i) {
        	Class<?>[] types = QtJambi_LibraryUtilities.internal.lambdaClassTypes(cls, fun);
            if(types!=null) {
                typeA = boxedType(types[++i]);
                typeB = boxedType(types[++i]);
                typeC = boxedType(types[++i]);
                typeD = boxedType(types[++i]);
                typeE = boxedType(types[++i]);
                typeF = boxedType(types[++i]);
                typeG = boxedType(types[++i]);
                typeH = boxedType(types[++i]);
                typeI = boxedType(types[++i]);
            }
        }
        
        void testArguments(A a, B b, C c, D d, E e, F f, G g, H h, I i) {
        	testArgument(typeA, a, "a");
        	testArgument(typeB, b, "b");
        	testArgument(typeC, c, "c");
        	testArgument(typeD, d, "d");
        	testArgument(typeE, e, "e");
        	testArgument(typeF, f, "f");
        	testArgument(typeG, g, "g");
        	testArgument(typeH, h, "h");
        	testArgument(typeI, i, "i");
        }
        
        /**
         * <p>See <code><a href="https://doc.qt.io/qt/qtconcurrent-qtaskbuilder.html#onThreadPool">QtConcurrent::QTaskBuilder&lt;Task,Args&gt;::<wbr/>onThreadPool(QThreadPool&amp;)</a></code></p>
         * <p>This member function returns the object itself.</p>
         * @param newThreadPool
         * @return the object itself
         */
        @QtUninvokable
        public abstract @NonNull Params9<T,A,B,C,D,E,F,G,H,I> onThreadPool(io.qt.core.@StrictNonNull QThreadPool newThreadPool);
        
        /**
         * <p>See <code><a href="https://doc.qt.io/qt/qtconcurrent-qtaskbuilder.html#withPriority">QtConcurrent::QTaskBuilder&lt;Task,Args&gt;::<wbr/>withPriority(int)</a></code></p>
         * <p>This member function returns the object itself.</p>
         * @param newPriority
         * @return the object itself
         */
        @QtUninvokable
        public abstract @NonNull Params9<T,A,B,C,D,E,F,G,H,I> withPriority(int newPriority);
        
        /**
         * <p>See <code><a href="https://doc.qt.io/qt/qtconcurrent-qtaskbuilder.html#withArguments">QtConcurrent::QTaskBuilder&lt;Task,Args&gt;::<wbr/>withArguments&lt;ExtraArgs...&gt;(ExtraArgs&amp;&amp;)</a></code></p>
         * @param a
         * @param b
         * @param c
         * @param d
         * @param e
         * @param f
         * @param g
         * @param h
         * @param i
         * @return
         */
        @QtUninvokable
        public abstract @NonNull QTaskBuilder<T> withArguments(A a, B b, C c, D d, E e, F f, G g, H h, I i);
        
        /**
         * Constructor for internal use only.
         * @param p expected to be <code>null</code>.
         * @hidden
         */
        @NativeAccess
        Params9(QPrivateConstructor p) { super(p); } 
        
        
        /**
         * <p>Creates and returns a copy of this object.</p>
         */
        @QtUninvokable
        @Override
        public abstract Params9<T, A,B,C,D,E,F,G,H,I> clone();
    }
}
