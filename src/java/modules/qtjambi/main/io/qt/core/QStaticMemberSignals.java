/****************************************************************************
**
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import java.util.function.Supplier;

import io.qt.*;
import io.qt.core.QMetaObject.*;

/**
 * QStaticMemberSignals is a namespace containing signal types to be used as static member signals.
 * Static signals are never realized as Qt's meta object signals but have a lightweight Java implementation.
 */
public final class QStaticMemberSignals {
	private QStaticMemberSignals(){}
    
    /**
     * Use this signal class to define a static signal.
     */
    public static final class Signal0 extends QMetaObject.AbstractPublicSignal0 {
    	@SuppressWarnings("unused")
		private Signal0(Class<?> declaringClass) {
			super(declaringClass);
		}
        
		public Signal0() {
			super(QtJambi_LibraryUtilities.internal.callerClassProvider().get());
		}
    }
    
    /**
     * Use this signal class to define a static signal.
     *
     * @param <A> The type of the single parameter of the signal.
     */
    public static final class Signal1<A> extends QMetaObject.AbstractPublicSignal1<A> {
    	@SuppressWarnings("unused")
		private Signal1(Class<?> declaringClass) {
			super(declaringClass);
		}
        
		public Signal1() {
			super(QtJambi_LibraryUtilities.internal.callerClassProvider().get());
		}
    }
    
    /**
     * Use this signal class to define a static signal.
     *
     * @param <A> The type of the single parameter of the signal.
     * @param <B> The type of the single parameter of the signal.
     */
    public static final class Signal2<A,B> extends QMetaObject.AbstractPublicSignal2<A,B> {
    	@SuppressWarnings("unused")
		private Signal2(Class<?> declaringClass) {
			super(declaringClass);
		}
        
		public Signal2() {
			super(QtJambi_LibraryUtilities.internal.callerClassProvider().get());
		}
    }
    
    /**
     * Use this signal class to define a static signal.
     *
     * @param <A> The type of the single parameter of the signal.
     * @param <B> The type of the single parameter of the signal.
     * @param <C> The type of the single parameter of the signal.
     */
    public static final class Signal3<A,B,C> extends QMetaObject.AbstractPublicSignal3<A,B,C> {
    	@SuppressWarnings("unused")
		private Signal3(Class<?> declaringClass) {
			super(declaringClass);
		}
        
		public Signal3() {
			super(QtJambi_LibraryUtilities.internal.callerClassProvider().get());
		}
    }
    
    /**
     * Use this signal class to define a static signal.
     *
     * @param <A> The type of the single parameter of the signal.
     * @param <B> The type of the single parameter of the signal.
     * @param <C> The type of the single parameter of the signal.
     * @param <D> The type of the single parameter of the signal.
     */
    public static final class Signal4<A,B,C,D> extends QMetaObject.AbstractPublicSignal4<A,B,C,D> {
    	@SuppressWarnings("unused")
		private Signal4(Class<?> declaringClass) {
			super(declaringClass);
		}
        
		public Signal4() {
			super(QtJambi_LibraryUtilities.internal.callerClassProvider().get());
		}
    }
    
    /**
     * Use this signal class to define a static signal.
     *
     * @param <A> The type of the single parameter of the signal.
     * @param <B> The type of the single parameter of the signal.
     * @param <C> The type of the single parameter of the signal.
     * @param <D> The type of the single parameter of the signal.
     * @param <E> The type of the single parameter of the signal.
     */
    public static final class Signal5<A,B,C,D,E> extends QMetaObject.AbstractPublicSignal5<A,B,C,D,E> {
    	@SuppressWarnings("unused")
		private Signal5(Class<?> declaringClass) {
			super(declaringClass);
		}
        
		public Signal5() {
			super(QtJambi_LibraryUtilities.internal.callerClassProvider().get());
		}
    }
    
    /**
     * Use this signal class to define a static signal.
     *
     * @param <A> The type of the single parameter of the signal.
     * @param <B> The type of the single parameter of the signal.
     * @param <C> The type of the single parameter of the signal.
     * @param <D> The type of the single parameter of the signal.
     * @param <E> The type of the single parameter of the signal.
     * @param <F> The type of the single parameter of the signal.
     */
    public static final class Signal6<A,B,C,D,E,F> extends QMetaObject.AbstractPublicSignal6<A,B,C,D,E,F> {
    	@SuppressWarnings("unused")
		private Signal6(Class<?> declaringClass) {
			super(declaringClass);
		}
        
		public Signal6() {
			super(QtJambi_LibraryUtilities.internal.callerClassProvider().get());
		}
    }
    
    /**
     * Use this signal class to define a static signal.
     *
     * @param <A> The type of the single parameter of the signal.
     * @param <B> The type of the single parameter of the signal.
     * @param <C> The type of the single parameter of the signal.
     * @param <D> The type of the single parameter of the signal.
     * @param <E> The type of the single parameter of the signal.
     * @param <F> The type of the single parameter of the signal.
     * @param <G> The type of the single parameter of the signal.
     */
    public static final class Signal7<A,B,C,D,E,F,G> extends QMetaObject.AbstractPublicSignal7<A,B,C,D,E,F,G> {
    	@SuppressWarnings("unused")
		private Signal7(Class<?> declaringClass) {
			super(declaringClass);
		}
        
		public Signal7() {
			super(QtJambi_LibraryUtilities.internal.callerClassProvider().get());
		}
    }
    
    /**
     * Use this signal class to define a static signal.
     *
     * @param <A> The type of the single parameter of the signal.
     * @param <B> The type of the single parameter of the signal.
     * @param <C> The type of the single parameter of the signal.
     * @param <D> The type of the single parameter of the signal.
     * @param <E> The type of the single parameter of the signal.
     * @param <F> The type of the single parameter of the signal.
     * @param <G> The type of the single parameter of the signal.
     * @param <H> The type of the single parameter of the signal.
     */
    public static final class Signal8<A,B,C,D,E,F,G,H> extends QMetaObject.AbstractPublicSignal8<A,B,C,D,E,F,G,H> {
    	@SuppressWarnings("unused")
		private Signal8(Class<?> declaringClass) {
			super(declaringClass);
		}
        
		public Signal8() {
			super(QtJambi_LibraryUtilities.internal.callerClassProvider().get());
		}
    }
    
    /**
     * Use this signal class to define a static signal.
     *
     * @param <A> The type of the single parameter of the signal.
     * @param <B> The type of the single parameter of the signal.
     * @param <C> The type of the single parameter of the signal.
     * @param <D> The type of the single parameter of the signal.
     * @param <E> The type of the single parameter of the signal.
     * @param <F> The type of the single parameter of the signal.
     * @param <G> The type of the single parameter of the signal.
     * @param <H> The type of the single parameter of the signal.
     * @param <I> The type of the single parameter of the signal.
     */
    public static final class Signal9<A,B,C,D,E,F,G,H,I> extends QMetaObject.AbstractPublicSignal9<A,B,C,D,E,F,G,H,I> {
    	@SuppressWarnings("unused")
		private Signal9(Class<?> declaringClass) {
			super(declaringClass);
		}
        
		public Signal9() {
			super(QtJambi_LibraryUtilities.internal.callerClassProvider().get());
		}
    }

    /**
     * Use this signal class to define a static signal.
     */
    public static final class PrivateSignal0 extends QMetaObject.AbstractPrivateSignal0 {
    	
    	@SuppressWarnings("unused")
		private PrivateSignal0(Class<?> declaringClass) {
			super(declaringClass);
		}

		public PrivateSignal0() {
			super(QtJambi_LibraryUtilities.internal.callerClassProvider().get());
		}
    	
    	@QtUninvokable
        private void emit() {
            emitSignal();
        }
    }
    
    /**
     * Use this signal class to define a static signal.
     *
     * @param <A> The type of the single parameter of the signal.
     */
    public static final class PrivateSignal1<A> extends QMetaObject.AbstractPrivateSignal1<A> {
    	@SuppressWarnings("unused")
		private PrivateSignal1(Class<?> declaringClass) {
			super(declaringClass);
		}
    	
    	public PrivateSignal1() {
			super(QtJambi_LibraryUtilities.internal.callerClassProvider().get());
		}
    	
    	@QtUninvokable
        private void emit(A arg1) {
            emitSignal(arg1);
        }
    }
    
    /**
     * Use this signal class to define a static signal.
     *
     * @param <A> The type of the single parameter of the signal.
     * @param <B> The type of the single parameter of the signal.
     */
    public static final class PrivateSignal2<A,B> extends QMetaObject.AbstractPrivateSignal2<A,B> {
    	@SuppressWarnings("unused")
		private PrivateSignal2(Class<?> declaringClass) {
			super(declaringClass);
		}
    	
    	public PrivateSignal2() {
			super(QtJambi_LibraryUtilities.internal.callerClassProvider().get());
		}
    	
    	@QtUninvokable
        private void emit(A arg1, B arg2) {
            emitSignal(arg1, arg2);
        }
    }
    
    /**
     * Use this signal class to define a static signal.
     *
     * @param <A> The type of the single parameter of the signal.
     * @param <B> The type of the single parameter of the signal.
     * @param <C> The type of the single parameter of the signal.
     */
    public static final class PrivateSignal3<A,B,C> extends QMetaObject.AbstractPrivateSignal3<A,B,C> {
    	@SuppressWarnings("unused")
		private PrivateSignal3(Class<?> declaringClass) {
			super(declaringClass);
		}
    	
    	public PrivateSignal3() {
			super(QtJambi_LibraryUtilities.internal.callerClassProvider().get());
		}
    	
    	@QtUninvokable
        private void emit(A arg1, B arg2, C arg3) {
            emitSignal(arg1, arg2, arg3);
        }
    }
    
    /**
     * Use this signal class to define a static signal.
     *
     * @param <A> The type of the single parameter of the signal.
     * @param <B> The type of the single parameter of the signal.
     * @param <C> The type of the single parameter of the signal.
     * @param <D> The type of the single parameter of the signal.
     */
    public static final class PrivateSignal4<A,B,C,D> extends QMetaObject.AbstractPrivateSignal4<A,B,C,D> {
    	@SuppressWarnings("unused")
		private PrivateSignal4(Class<?> declaringClass) {
			super(declaringClass);
		}
    	
    	public PrivateSignal4() {
			super(QtJambi_LibraryUtilities.internal.callerClassProvider().get());
		}
    	
    	@QtUninvokable
        private void emit(A arg1, B arg2, C arg3, D arg4) {
            emitSignal(arg1, arg2, arg3, arg4);
        }
    }
    
    /**
     * Use this signal class to define a static signal.
     *
     * @param <A> The type of the single parameter of the signal.
     * @param <B> The type of the single parameter of the signal.
     * @param <C> The type of the single parameter of the signal.
     * @param <D> The type of the single parameter of the signal.
     * @param <E> The type of the single parameter of the signal.
     */
    public static final class PrivateSignal5<A,B,C,D,E> extends QMetaObject.AbstractPrivateSignal5<A,B,C,D,E> {
    	@SuppressWarnings("unused")
		private PrivateSignal5(Class<?> declaringClass) {
			super(declaringClass);
		}
    	
    	public PrivateSignal5() {
			super(QtJambi_LibraryUtilities.internal.callerClassProvider().get());
		}
    	
    	@QtUninvokable
        private void emit(A arg1, B arg2, C arg3, D arg4, E arg5) {
            emitSignal(arg1, arg2, arg3, arg4, arg5);
        }
    }
    
    /**
     * Use this signal class to define a static signal.
     *
     * @param <A> The type of the single parameter of the signal.
     * @param <B> The type of the single parameter of the signal.
     * @param <C> The type of the single parameter of the signal.
     * @param <D> The type of the single parameter of the signal.
     * @param <E> The type of the single parameter of the signal.
     * @param <F> The type of the single parameter of the signal.
     */
    public static final class PrivateSignal6<A,B,C,D,E,F> extends QMetaObject.AbstractPrivateSignal6<A,B,C,D,E,F> {
    	@SuppressWarnings("unused")
		private PrivateSignal6(Class<?> declaringClass) {
			super(declaringClass);
		}
    	
    	public PrivateSignal6() {
			super(QtJambi_LibraryUtilities.internal.callerClassProvider().get());
		}
    	
    	@QtUninvokable
        private void emit(A arg1, B arg2, C arg3, D arg4, E arg5, F arg6) {
            emitSignal(arg1, arg2, arg3, arg4, arg5, arg6);
        }
    }
    
    /**
     * Use this signal class to define a static signal.
     *
     * @param <A> The type of the single parameter of the signal.
     * @param <B> The type of the single parameter of the signal.
     * @param <C> The type of the single parameter of the signal.
     * @param <D> The type of the single parameter of the signal.
     * @param <E> The type of the single parameter of the signal.
     * @param <F> The type of the single parameter of the signal.
     * @param <G> The type of the single parameter of the signal.
     */
    public static final class PrivateSignal7<A,B,C,D,E,F,G> extends QMetaObject.AbstractPrivateSignal7<A,B,C,D,E,F,G> {
    	@SuppressWarnings("unused")
		private PrivateSignal7(Class<?> declaringClass) {
			super(declaringClass);
		}
    	
    	public PrivateSignal7() {
			super(QtJambi_LibraryUtilities.internal.callerClassProvider().get());
		}
    	
    	@QtUninvokable
        private void emit(A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7) {
            emitSignal(arg1, arg2, arg3, arg4, arg5, arg6, arg7);
        }
    }
    
    /**
     * Use this signal class to define a static signal.
     *
     * @param <A> The type of the single parameter of the signal.
     * @param <B> The type of the single parameter of the signal.
     * @param <C> The type of the single parameter of the signal.
     * @param <D> The type of the single parameter of the signal.
     * @param <E> The type of the single parameter of the signal.
     * @param <F> The type of the single parameter of the signal.
     * @param <G> The type of the single parameter of the signal.
     * @param <H> The type of the single parameter of the signal.
     */
    public static final class PrivateSignal8<A,B,C,D,E,F,G,H> extends QMetaObject.AbstractPrivateSignal8<A,B,C,D,E,F,G,H> {
    	@SuppressWarnings("unused")
		private PrivateSignal8(Class<?> declaringClass) {
			super(declaringClass);
		}
    	
    	public PrivateSignal8() {
			super(QtJambi_LibraryUtilities.internal.callerClassProvider().get());
		}
    	
    	@QtUninvokable
        private void emit(A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7, H arg8) {
            emitSignal(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
        }
    }
    
    /**
     * Use this signal class to define a static signal.
     *
     * @param <A> The type of the single parameter of the signal.
     * @param <B> The type of the single parameter of the signal.
     * @param <C> The type of the single parameter of the signal.
     * @param <D> The type of the single parameter of the signal.
     * @param <E> The type of the single parameter of the signal.
     * @param <F> The type of the single parameter of the signal.
     * @param <G> The type of the single parameter of the signal.
     * @param <H> The type of the single parameter of the signal.
     * @param <I> The type of the single parameter of the signal.
     */
    public static final class PrivateSignal9<A,B,C,D,E,F,G,H,I> extends QMetaObject.AbstractPrivateSignal9<A,B,C,D,E,F,G,H,I> {
    	@SuppressWarnings("unused")
		private PrivateSignal9(Class<?> declaringClass) {
			super(declaringClass);
		}
    	
    	public PrivateSignal9() {
			super(QtJambi_LibraryUtilities.internal.callerClassProvider().get());
		}
    	
    	@QtUninvokable
        private void emit(A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7, H arg8, I arg9) {
            emitSignal(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
        }
    }
    
    /**
     * Equivalent to {@link Signal1} with default value for the parameter.
     *
     * @param <A> The type of the first parameter of the signal.
     */
    public static final class Signal1Default1<A> extends QMetaObject.AbstractSignal1Default1<A>{
		public Signal1Default1(@StrictNonNull Supplier<? extends A> arg1Default){
    		super(arg1Default, QtJambi_LibraryUtilities.internal.callerClassProvider().get());
    	}
    }
    
    /**
     * Equivalent to {@link Signal2} with default value for parameter no. 2.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     */
    public static final class Signal2Default1<A, B> extends QMetaObject.AbstractSignal2Default1<A, B>{
        
		public Signal2Default1(@StrictNonNull Supplier<? extends B> arg2Default) {
			super(arg2Default, QtJambi_LibraryUtilities.internal.callerClassProvider().get());
		}
	}
    
    /**
     * Equivalent to {@link Signal2} with default values for parameters 1 and 2.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     */
    public static final class Signal2Default2<A, B> extends QMetaObject.AbstractSignal2Default2<A, B>{
        
		public Signal2Default2(@StrictNonNull Supplier<? extends A> arg1Default, @StrictNonNull Supplier<? extends B> arg2Default){
    		super(arg1Default, arg2Default, QtJambi_LibraryUtilities.internal.callerClassProvider().get());
    	}
    }
    
    /**
     * Equivalent to {@link Signal3} with default value for parameter no. 3.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     */
    public static final class Signal3Default1<A, B, C> extends QMetaObject.AbstractSignal3Default1<A, B, C> {
                
		public Signal3Default1(@StrictNonNull Supplier<? extends C> arg3Default) {
			super(arg3Default, QtJambi_LibraryUtilities.internal.callerClassProvider().get());
		}
    }
    
    /**
     * Equivalent to {@link Signal3} with default values for parameters 2 and 3.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     */
    public static final class Signal3Default2<A, B, C> extends QMetaObject.AbstractSignal3Default2<A, B, C> {
        
		public Signal3Default2(@StrictNonNull Supplier<? extends B> arg2Default, @StrictNonNull Supplier<? extends C> arg3Default){
    		super(arg2Default, arg3Default, QtJambi_LibraryUtilities.internal.callerClassProvider().get());
    	}
    }
    
    /**
     * Equivalent to {@link Signal3} with default values for parameters 1 to 3.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     */
    public static final class Signal3Default3<A, B, C> extends QMetaObject.AbstractSignal3Default3<A, B, C>{
		public Signal3Default3(@StrictNonNull Supplier<? extends A> arg1Default, @StrictNonNull Supplier<? extends B> arg2Default, @StrictNonNull Supplier<? extends C> arg3Default){
    		super(arg1Default, arg2Default, arg3Default, QtJambi_LibraryUtilities.internal.callerClassProvider().get());
    	}
    }
    
    /**
     * Equivalent to {@link Signal4} with default value for parameter no. 4.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     */
    public static final class Signal4Default1<A, B, C, D> extends QMetaObject.AbstractSignal4Default1<A, B, C, D>{
        
		public Signal4Default1(@StrictNonNull Supplier<? extends D> arg4Default){
    		super(arg4Default, QtJambi_LibraryUtilities.internal.callerClassProvider().get());
    	}
    }
    
    /**
     * Equivalent to {@link Signal4} with default values for parameters 3 and 4.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     */
    public static final class Signal4Default2<A, B, C, D> extends QMetaObject.AbstractSignal4Default2<A, B, C, D>{
        
		public Signal4Default2(@StrictNonNull Supplier<? extends C> arg3Default, @StrictNonNull Supplier<? extends D> arg4Default){
    		super(arg3Default, arg4Default, QtJambi_LibraryUtilities.internal.callerClassProvider().get());
    	}
    }
    
    /**
     * Equivalent to {@link Signal4} with default values for parameters 2 to 4.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     */
    public static final class Signal4Default3<A, B, C, D> extends QMetaObject.AbstractSignal4Default3<A, B, C, D>{
        
		public Signal4Default3(@StrictNonNull Supplier<? extends B> arg2Default, @StrictNonNull Supplier<? extends C> arg3Default, @StrictNonNull Supplier<? extends D> arg4Default){
    		super(arg2Default, arg3Default, arg4Default, QtJambi_LibraryUtilities.internal.callerClassProvider().get());
    	}
    }
    
    /**
     * Equivalent to {@link Signal4} with default values for parameters 1 to 4.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     */
    public static final class Signal4Default4<A, B, C, D> extends QMetaObject.AbstractSignal4Default4<A, B, C, D>{
        
		public Signal4Default4(
				@StrictNonNull Supplier<? extends A> arg1Default, @StrictNonNull Supplier<? extends B> arg2Default, 
				@StrictNonNull Supplier<? extends C> arg3Default, @StrictNonNull Supplier<? extends D> arg4Default){
    		super(arg1Default, arg2Default, arg3Default, arg4Default, QtJambi_LibraryUtilities.internal.callerClassProvider().get());
    	}
    }
    
    /**
     * Equivalent to {@link Signal5} with default value for parameter no. 5.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     */
    public static final class Signal5Default1<A, B, C, D, E> extends QMetaObject.AbstractSignal5Default1<A, B, C, D, E>{
        
		public Signal5Default1(@StrictNonNull Supplier<? extends E> arg5Default){
    		super(arg5Default, QtJambi_LibraryUtilities.internal.callerClassProvider().get());
    	}
    }
    
    /**
     * Equivalent to {@link Signal5} with default values for parameters 4 and 5.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     */
    public static final class Signal5Default2<A, B, C, D, E> extends QMetaObject.AbstractSignal5Default2<A, B, C, D, E>{
        
		public Signal5Default2(@StrictNonNull Supplier<? extends D> arg4Default, @StrictNonNull Supplier<? extends E> arg5Default){
    		super(arg4Default, arg5Default, QtJambi_LibraryUtilities.internal.callerClassProvider().get());
    	}
    }
    
    /**
     * Equivalent to {@link Signal5} with default values for parameters 3 to 5.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     */
    public static final class Signal5Default3<A, B, C, D, E> extends QMetaObject.AbstractSignal5Default3<A, B, C, D, E>{
        
		public Signal5Default3(@StrictNonNull Supplier<? extends C> arg3Default, @StrictNonNull Supplier<? extends D> arg4Default, @StrictNonNull Supplier<? extends E> arg5Default){
    		super(arg3Default, arg4Default, arg5Default, QtJambi_LibraryUtilities.internal.callerClassProvider().get());
    	}
    }
    
    /**
     * Equivalent to {@link Signal5} with default values for parameters 2 to 5.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     */
    public static final class Signal5Default4<A, B, C, D, E> extends QMetaObject.AbstractSignal5Default4<A, B, C, D, E>{
        
		public Signal5Default4(@StrictNonNull Supplier<? extends B> arg2Default, @StrictNonNull Supplier<? extends C> arg3Default, @StrictNonNull Supplier<? extends D> arg4Default, @StrictNonNull Supplier<? extends E> arg5Default){
    		super(arg2Default, arg3Default, arg4Default, arg5Default, QtJambi_LibraryUtilities.internal.callerClassProvider().get());
    	}
    }
    
    /**
     * Equivalent to {@link Signal5} with default values for parameters 1 to 5.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     */
    public static final class Signal5Default5<A, B, C, D, E> extends QMetaObject.AbstractSignal5Default5<A, B, C, D, E>{
		public Signal5Default5(@StrictNonNull Supplier<? extends A> arg1Default, @StrictNonNull Supplier<? extends B> arg2Default, @StrictNonNull Supplier<? extends C> arg3Default, @StrictNonNull Supplier<? extends D> arg4Default, @StrictNonNull Supplier<? extends E> arg5Default){
    		super(arg1Default, arg2Default, arg3Default, arg4Default, arg5Default, QtJambi_LibraryUtilities.internal.callerClassProvider().get());
    	}
    }
    
    /**
     * Equivalent to {@link Signal6} with default value for parameter no. 6.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     */
    public static final class Signal6Default1<A, B, C, D, E, F> extends QMetaObject.AbstractSignal6Default1<A, B, C, D, E, F>{
        
		public Signal6Default1(@StrictNonNull Supplier<? extends F> arg6Default){
    		super(arg6Default, QtJambi_LibraryUtilities.internal.callerClassProvider().get());
    	}
    }
    
    /**
     * Equivalent to {@link Signal6} with default values for parameters 5 and 6.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     */
    public static final class Signal6Default2<A, B, C, D, E, F> extends QMetaObject.AbstractSignal6Default2<A, B, C, D, E, F>{
        
		public Signal6Default2(@StrictNonNull Supplier<? extends E> arg5Default, @StrictNonNull Supplier<? extends F> arg6Default){
    		super(arg5Default, arg6Default, QtJambi_LibraryUtilities.internal.callerClassProvider().get());
    	}
    }
    
    /**
     * Equivalent to {@link Signal6} with default values for parameters 4 to 6.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     */
    public static final class Signal6Default3<A, B, C, D, E, F> extends AbstractSignal6Default3<A, B, C, D, E, F>{
        
		public Signal6Default3(@StrictNonNull Supplier<? extends D> arg4Default, @StrictNonNull Supplier<? extends E> arg5Default, @StrictNonNull Supplier<? extends F> arg6Default){
    		super(arg4Default, arg5Default, arg6Default, QtJambi_LibraryUtilities.internal.callerClassProvider().get());
    	}
    }
    
    /**
     * Equivalent to {@link Signal6} with default values for parameters 3 to 6.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     */
    public static final class Signal6Default4<A, B, C, D, E, F> extends AbstractSignal6Default4<A, B, C, D, E, F>{
        
		public Signal6Default4(@StrictNonNull Supplier<? extends C> arg3Default, @StrictNonNull Supplier<? extends D> arg4Default, @StrictNonNull Supplier<? extends E> arg5Default, @StrictNonNull Supplier<? extends F> arg6Default){
    		super(arg3Default, arg4Default, arg5Default, arg6Default, QtJambi_LibraryUtilities.internal.callerClassProvider().get());
    	}
    }
    
    /**
     * Equivalent to {@link Signal6} with default values for parameters 2 to 6.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     */
    public static final class Signal6Default5<A, B, C, D, E, F> extends AbstractSignal6Default5<A, B, C, D, E, F>{
        
		public Signal6Default5(@StrictNonNull Supplier<? extends B> arg2Default, @StrictNonNull Supplier<? extends C> arg3Default, @StrictNonNull Supplier<? extends D> arg4Default, @StrictNonNull Supplier<? extends E> arg5Default, @StrictNonNull Supplier<? extends F> arg6Default){
    		super(arg2Default, arg3Default, arg4Default, arg5Default, arg6Default, QtJambi_LibraryUtilities.internal.callerClassProvider().get());
    	}
    }
    
    /**
     * Equivalent to {@link Signal6} with default values for parameters 1 to 6.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     */
    public static final class Signal6Default6<A, B, C, D, E, F> extends AbstractSignal6Default6<A, B, C, D, E, F>{
		public Signal6Default6(@StrictNonNull Supplier<? extends A> arg1Default, @StrictNonNull Supplier<? extends B> arg2Default, @StrictNonNull Supplier<? extends C> arg3Default, @StrictNonNull Supplier<? extends D> arg4Default, @StrictNonNull Supplier<? extends E> arg5Default, @StrictNonNull Supplier<? extends F> arg6Default){
    		super(arg1Default, arg2Default, arg3Default, arg4Default, arg5Default, arg6Default, QtJambi_LibraryUtilities.internal.callerClassProvider().get());
    	}
    }
    
    /**
     * Equivalent to {@link Signal7} with default value for parameter no. 7.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     */
    public static final class Signal7Default1<A, B, C, D, E, F, G> extends AbstractSignal7Default1<A, B, C, D, E, F, G>{
        
		public Signal7Default1(@StrictNonNull Supplier<? extends G> arg7Default){
    		super(arg7Default, QtJambi_LibraryUtilities.internal.callerClassProvider().get());
    	}
    }
    
    /**
     * Equivalent to {@link Signal7} with default values for parameters 6 and 7.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     */
    public static final class Signal7Default2<A, B, C, D, E, F, G> extends AbstractSignal7Default2<A, B, C, D, E, F, G> {
        
		public Signal7Default2(@StrictNonNull Supplier<? extends F> arg6Default, @StrictNonNull Supplier<? extends G> arg7Default){
    		super(arg6Default, arg7Default, QtJambi_LibraryUtilities.internal.callerClassProvider().get());
    	}
    }
    
    /**
     * Equivalent to {@link Signal7} with default values for parameters 5 to 7.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     */
    public static final class Signal7Default3<A, B, C, D, E, F, G> extends AbstractSignal7Default3<A, B, C, D, E, F, G> {
        
		public Signal7Default3(@StrictNonNull Supplier<? extends E> arg5Default, @StrictNonNull Supplier<? extends F> arg6Default, @StrictNonNull Supplier<? extends G> arg7Default){
    		super(arg5Default, arg6Default, arg7Default, QtJambi_LibraryUtilities.internal.callerClassProvider().get());
    	}
    }
    
    /**
     * Equivalent to {@link Signal7} with default values for parameters 4 to 7.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     */
    public static final class Signal7Default4<A, B, C, D, E, F, G> extends AbstractSignal7Default4<A, B, C, D, E, F, G> {
        
		public Signal7Default4(@StrictNonNull Supplier<? extends D> arg4Default, @StrictNonNull Supplier<? extends E> arg5Default, @StrictNonNull Supplier<? extends F> arg6Default, @StrictNonNull Supplier<? extends G> arg7Default){
    		super(arg4Default, arg5Default, arg6Default, arg7Default, QtJambi_LibraryUtilities.internal.callerClassProvider().get());
    	}
    }
    
    /**
     * Equivalent to {@link Signal7} with default values for parameters 3 to 7.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     */
    public static final class Signal7Default5<A, B, C, D, E, F, G> extends AbstractSignal7Default5<A, B, C, D, E, F, G> {
        
		public Signal7Default5(@StrictNonNull Supplier<? extends C> arg3Default, @StrictNonNull Supplier<? extends D> arg4Default, @StrictNonNull Supplier<? extends E> arg5Default, @StrictNonNull Supplier<? extends F> arg6Default, @StrictNonNull Supplier<? extends G> arg7Default){
    		super(arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, QtJambi_LibraryUtilities.internal.callerClassProvider().get());
    	}
    }
    
    /**
     * Equivalent to {@link Signal7} with default values for parameters 2 to 7.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     */
    public static final class Signal7Default6<A, B, C, D, E, F, G> extends AbstractSignal7Default6<A, B, C, D, E, F, G> {
        
		public Signal7Default6(@StrictNonNull Supplier<? extends B> arg2Default, @StrictNonNull Supplier<? extends C> arg3Default, @StrictNonNull Supplier<? extends D> arg4Default, @StrictNonNull Supplier<? extends E> arg5Default, @StrictNonNull Supplier<? extends F> arg6Default, @StrictNonNull Supplier<? extends G> arg7Default){
    		super(arg2Default, arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, QtJambi_LibraryUtilities.internal.callerClassProvider().get());
    	}
    }
    
    /**
     * Equivalent to {@link Signal7} with default values for parameters 1 to 7.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     */
    public static final class Signal7Default7<A, B, C, D, E, F, G> extends AbstractSignal7Default7<A, B, C, D, E, F, G>{
		public Signal7Default7(@StrictNonNull Supplier<? extends A> arg1Default, @StrictNonNull Supplier<? extends B> arg2Default, @StrictNonNull Supplier<? extends C> arg3Default, @StrictNonNull Supplier<? extends D> arg4Default, @StrictNonNull Supplier<? extends E> arg5Default, @StrictNonNull Supplier<? extends F> arg6Default, @StrictNonNull Supplier<? extends G> arg7Default){
    		super(arg1Default, arg2Default, arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, QtJambi_LibraryUtilities.internal.callerClassProvider().get());
    	}
    }
    
    /**
     * Equivalent to {@link Signal8} with default value for parameter no. 8.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     * @param <H> The type of the eighth parameter of the signal.
     */
    public static final class Signal8Default1<A, B, C, D, E, F, G, H> extends AbstractSignal8Default1<A, B, C, D, E, F, G, H>{
        
		public Signal8Default1(@StrictNonNull Supplier<? extends H> arg8Default){
    		super(arg8Default, QtJambi_LibraryUtilities.internal.callerClassProvider().get());
    	}
    }
    
    /**
     * Equivalent to {@link Signal8} with default values for parameters 7 and 8.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     * @param <H> The type of the eighth parameter of the signal.
     */
    public static final class Signal8Default2<A, B, C, D, E, F, G, H> extends AbstractSignal8Default2<A, B, C, D, E, F, G, H> {
        
		public Signal8Default2(@StrictNonNull Supplier<? extends G> arg7Default, @StrictNonNull Supplier<? extends H> arg8Default){
    		super(arg7Default, arg8Default, QtJambi_LibraryUtilities.internal.callerClassProvider().get());
    	}
    }
    
    /**
     * Equivalent to {@link Signal8} with default values for parameters 6 to 8.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     * @param <H> The type of the eighth parameter of the signal.
     */
    public static final class Signal8Default3<A, B, C, D, E, F, G, H> extends AbstractSignal8Default3<A, B, C, D, E, F, G, H> {
        
		public Signal8Default3(@StrictNonNull Supplier<? extends F> arg6Default, @StrictNonNull Supplier<? extends G> arg7Default, @StrictNonNull Supplier<? extends H> arg8Default){
    		super(arg6Default, arg7Default, arg8Default, QtJambi_LibraryUtilities.internal.callerClassProvider().get());
    	}
    }
    
    /**
     * Equivalent to {@link Signal8} with default values for parameters 5 to 8.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     * @param <H> The type of the eighth parameter of the signal.
     */
    public static final class Signal8Default4<A, B, C, D, E, F, G, H> extends AbstractSignal8Default4<A, B, C, D, E, F, G, H> {
        
		public Signal8Default4(@StrictNonNull Supplier<? extends E> arg5Default, @StrictNonNull Supplier<? extends F> arg6Default, @StrictNonNull Supplier<? extends G> arg7Default, @StrictNonNull Supplier<? extends H> arg8Default){
    		super(arg5Default, arg6Default, arg7Default, arg8Default, QtJambi_LibraryUtilities.internal.callerClassProvider().get());
    	}
    }
    
    /**
     * Equivalent to {@link Signal8} with default values for parameters 4 to 8.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     * @param <H> The type of the eighth parameter of the signal.
     */
    public static final class Signal8Default5<A, B, C, D, E, F, G, H> extends AbstractSignal8Default5<A, B, C, D, E, F, G, H> {
        
		public Signal8Default5(@StrictNonNull Supplier<? extends D> arg4Default, @StrictNonNull Supplier<? extends E> arg5Default, @StrictNonNull Supplier<? extends F> arg6Default, @StrictNonNull Supplier<? extends G> arg7Default, @StrictNonNull Supplier<? extends H> arg8Default){
    		super(arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, QtJambi_LibraryUtilities.internal.callerClassProvider().get());
    	}
    }
    
    /**
     * Equivalent to {@link Signal8} with default values for parameters 3 to 8.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     * @param <H> The type of the eighth parameter of the signal.
     */
    public static final class Signal8Default6<A, B, C, D, E, F, G, H> extends AbstractSignal8Default6<A, B, C, D, E, F, G, H> {
        
		public Signal8Default6(@StrictNonNull Supplier<? extends C> arg3Default, @StrictNonNull Supplier<? extends D> arg4Default, @StrictNonNull Supplier<? extends E> arg5Default, @StrictNonNull Supplier<? extends F> arg6Default, @StrictNonNull Supplier<? extends G> arg7Default, @StrictNonNull Supplier<? extends H> arg8Default){
    		super(arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, QtJambi_LibraryUtilities.internal.callerClassProvider().get());
    	}
    }
    
    /**
     * Equivalent to {@link Signal8} with default values for parameters 2 to 8.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     * @param <H> The type of the eighth parameter of the signal.
     */
    public static final class Signal8Default7<A, B, C, D, E, F, G, H> extends AbstractSignal8Default7<A, B, C, D, E, F, G, H> {
        
		public Signal8Default7(@StrictNonNull Supplier<? extends B> arg2Default, @StrictNonNull Supplier<? extends C> arg3Default, @StrictNonNull Supplier<? extends D> arg4Default, @StrictNonNull Supplier<? extends E> arg5Default, @StrictNonNull Supplier<? extends F> arg6Default, @StrictNonNull Supplier<? extends G> arg7Default, @StrictNonNull Supplier<? extends H> arg8Default){
    		super(arg2Default, arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, QtJambi_LibraryUtilities.internal.callerClassProvider().get());
    	}
    }
    
    /**
     * Equivalent to {@link Signal8} with default values for parameters 1 to 8.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     * @param <H> The type of the eighth parameter of the signal.
     */
    public static final class Signal8Default8<A, B, C, D, E, F, G, H> extends AbstractSignal8Default8<A, B, C, D, E, F, G, H>{
		public Signal8Default8(@StrictNonNull Supplier<? extends A> arg1Default, @StrictNonNull Supplier<? extends B> arg2Default, @StrictNonNull Supplier<? extends C> arg3Default, @StrictNonNull Supplier<? extends D> arg4Default, @StrictNonNull Supplier<? extends E> arg5Default, @StrictNonNull Supplier<? extends F> arg6Default, @StrictNonNull Supplier<? extends G> arg7Default, @StrictNonNull Supplier<? extends H> arg8Default){
    		super(arg1Default, arg2Default, arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, QtJambi_LibraryUtilities.internal.callerClassProvider().get());
    	}
    }
    
    /**
     * Equivalent to {@link Signal9} with default value for parameter no. 9.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     * @param <H> The type of the eighth parameter of the signal.
     * @param <I> The type of the ninth parameter of the signal.
     */
    public static final class Signal9Default1<A, B, C, D, E, F, G, H, I> extends AbstractSignal9Default1<A, B, C, D, E, F, G, H, I>{
		public Signal9Default1(@StrictNonNull Supplier<? extends I> arg9Default){
    		super(arg9Default, QtJambi_LibraryUtilities.internal.callerClassProvider().get());
    	}
    }
    
    /**
     * Equivalent to {@link Signal9} with default values for parameters 8 and 9.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     * @param <H> The type of the eighth parameter of the signal.
     * @param <I> The type of the ninth parameter of the signal.
     */
    public static final class Signal9Default2<A, B, C, D, E, F, G, H, I> extends AbstractSignal9Default2<A, B, C, D, E, F, G, H, I> {
		public Signal9Default2(@StrictNonNull Supplier<? extends H> arg8Default, @StrictNonNull Supplier<? extends I> arg9Default){
    		super(arg8Default, arg9Default, QtJambi_LibraryUtilities.internal.callerClassProvider().get());
    	}
    }
    
    /**
     * Equivalent to {@link Signal9} with default values for parameters 7 to 9.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     * @param <H> The type of the eighth parameter of the signal.
     * @param <I> The type of the ninth parameter of the signal.
     */
    public static final class Signal9Default3<A, B, C, D, E, F, G, H, I> extends AbstractSignal9Default3<A, B, C, D, E, F, G, H, I> {
		public Signal9Default3(@StrictNonNull Supplier<? extends G> arg7Default, @StrictNonNull Supplier<? extends H> arg8Default, @StrictNonNull Supplier<? extends I> arg9Default){
    		super(arg7Default, arg8Default, arg9Default, QtJambi_LibraryUtilities.internal.callerClassProvider().get());
    	}
    }
    
    /**
     * Equivalent to {@link Signal9} with default values for parameters 6 to 9.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     * @param <H> The type of the eighth parameter of the signal.
     * @param <I> The type of the ninth parameter of the signal.
     */
    public static final class Signal9Default4<A, B, C, D, E, F, G, H, I> extends AbstractSignal9Default4<A, B, C, D, E, F, G, H, I> {
		public Signal9Default4(@StrictNonNull Supplier<? extends F> arg6Default, @StrictNonNull Supplier<? extends G> arg7Default, @StrictNonNull Supplier<? extends H> arg8Default, @StrictNonNull Supplier<? extends I> arg9Default){
    		super(arg6Default, arg7Default, arg8Default, arg9Default, QtJambi_LibraryUtilities.internal.callerClassProvider().get());
    	}
    }
    
    /**
     * Equivalent to {@link Signal9} with default values for parameters 5 to 9.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     * @param <H> The type of the eighth parameter of the signal.
     * @param <I> The type of the ninth parameter of the signal.
     */
    public static final class Signal9Default5<A, B, C, D, E, F, G, H, I> extends AbstractSignal9Default5<A, B, C, D, E, F, G, H, I> {
		public Signal9Default5(@StrictNonNull Supplier<? extends E> arg5Default, @StrictNonNull Supplier<? extends F> arg6Default, @StrictNonNull Supplier<? extends G> arg7Default, @StrictNonNull Supplier<? extends H> arg8Default, @StrictNonNull Supplier<? extends I> arg9Default){
    		super(arg5Default, arg6Default, arg7Default, arg8Default, arg9Default, QtJambi_LibraryUtilities.internal.callerClassProvider().get());
    	}
    }
    
    /**
     * Equivalent to {@link Signal9} with default values for parameters 4 to 9.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     * @param <H> The type of the eighth parameter of the signal.
     * @param <I> The type of the ninth parameter of the signal.
     */
    public static final class Signal9Default6<A, B, C, D, E, F, G, H, I> extends AbstractSignal9Default6<A, B, C, D, E, F, G, H, I> {
		public Signal9Default6(@StrictNonNull Supplier<? extends D> arg4Default, @StrictNonNull Supplier<? extends E> arg5Default, @StrictNonNull Supplier<? extends F> arg6Default, @StrictNonNull Supplier<? extends G> arg7Default, @StrictNonNull Supplier<? extends H> arg8Default, @StrictNonNull Supplier<? extends I> arg9Default){
    		super(arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, arg9Default, QtJambi_LibraryUtilities.internal.callerClassProvider().get());
		}
    }
    
    /**
     * Equivalent to {@link Signal9} with default values for parameters 3 to 9.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     * @param <H> The type of the eighth parameter of the signal.
     * @param <I> The type of the ninth parameter of the signal.
     */
    public static final class Signal9Default7<A, B, C, D, E, F, G, H, I> extends AbstractSignal9Default7<A, B, C, D, E, F, G, H, I> {
		public Signal9Default7(@StrictNonNull Supplier<? extends C> arg3Default, @StrictNonNull Supplier<? extends D> arg4Default, @StrictNonNull Supplier<? extends E> arg5Default, @StrictNonNull Supplier<? extends F> arg6Default, @StrictNonNull Supplier<? extends G> arg7Default, @StrictNonNull Supplier<? extends H> arg8Default, @StrictNonNull Supplier<? extends I> arg9Default){
    		super(arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, arg9Default, QtJambi_LibraryUtilities.internal.callerClassProvider().get());
    	}
    }
    
    /**
     * Equivalent to {@link Signal9} with default values for parameters 2 to 9.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     * @param <H> The type of the eighth parameter of the signal.
     * @param <I> The type of the ninth parameter of the signal.
     */
    public static final class Signal9Default8<A, B, C, D, E, F, G, H, I> extends AbstractSignal9Default8<A, B, C, D, E, F, G, H, I> {
		public Signal9Default8(@StrictNonNull Supplier<? extends B> arg2Default, @StrictNonNull Supplier<? extends C> arg3Default, @StrictNonNull Supplier<? extends D> arg4Default, @StrictNonNull Supplier<? extends E> arg5Default, @StrictNonNull Supplier<? extends F> arg6Default, @StrictNonNull Supplier<? extends G> arg7Default, @StrictNonNull Supplier<? extends H> arg8Default, @StrictNonNull Supplier<? extends I> arg9Default){
    		super(arg2Default, arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, arg9Default, QtJambi_LibraryUtilities.internal.callerClassProvider().get());
    	}
    }
    
    /**
     * Equivalent to {@link Signal9} with default values for parameters 1 to 9.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     * @param <H> The type of the eighth parameter of the signal.
     * @param <I> The type of the ninth parameter of the signal.
     */
    public static final class Signal9Default9<A, B, C, D, E, F, G, H, I> extends AbstractSignal9Default9<A, B, C, D, E, F, G, H, I>{
		public Signal9Default9(@StrictNonNull Supplier<? extends A> arg1Default, @StrictNonNull Supplier<? extends B> arg2Default, @StrictNonNull Supplier<? extends C> arg3Default, @StrictNonNull Supplier<? extends D> arg4Default, @StrictNonNull Supplier<? extends E> arg5Default, @StrictNonNull Supplier<? extends F> arg6Default, @StrictNonNull Supplier<? extends G> arg7Default, @StrictNonNull Supplier<? extends H> arg8Default, @StrictNonNull Supplier<? extends I> arg9Default){
    		super(arg1Default, arg2Default, arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, arg9Default, QtJambi_LibraryUtilities.internal.callerClassProvider().get());
    	}
    }
    
    /**
     * Wrapper class supporting the definition of overloaded signal in a class.
     */
    public static abstract class MultiSignal extends QMetaObject.AbstractMultiSignal {
    	protected MultiSignal(){
            super();
        }
        
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
    	@Override
		public @Nullable QtSignalEmitterInterface containingObject() {
			return null;
		}
    
        /**
         * Removes the given connection from this signal.
         *
         * @param connection the connection to be removed
         * @return <code>true</code> if the disconnection was successful.
         */
        public final boolean disconnect(QMetaObject.@NonNull Connection connection) {
            return super.disconnect(connection);
        }
        
        /**
         * Disconnects all overloaded signals.
         * @return <code>true</code> if any disconnection was successful.
         */
        @Override
        public final boolean disconnectAll() {
            return super.disconnectAll();
        }
        
        /**
         * Connects the signal to a method in an object. Whenever it is emitted, the method will be invoked
         * on the given object.
         *
         * @param receiver  The object that owns the method
         * @param method    The signature of the method excluding return type and argument names, such as "setText(String)".
         * @param connectionType One of the connection types defined in the Qt interface.
         * @throws QNoSuchSlotException Raised if the method passed in the slot object was not found
         * @throws java.lang.RuntimeException Raised if the signal object could not be successfully introspected or if the
         *                                    signatures of the signal and slot are incompatible.
         */
        public final QMetaObject.@NonNull Connection connect(Object receiver, String method, Qt.ConnectionType... connectionType) {
            if (receiver == null)
                throw new NullPointerException("Receiver must be non-null");
            if (method == null)
                throw new NullPointerException("Slot must be non-null");
            return super.connect(receiver, method, connectionType);
        }
        
        /**
         * Disconnects the signal from a method in an object if the two were previously connected by a call to connect.
         *
         * @param receiver The object to which the signal is connected
         * @param method The method in the receiver object to which the signal is connected
         * @return true if the connection was successfully removed, otherwise false. The method will return false if the
         * connection has not been previously established by a call to connect.
         * @throws QNoSuchSlotException Raised if the method passed in the slot object was not found
         */
        public final boolean disconnect(@Nullable Object receiver, @Nullable String method) {
            if (method != null && receiver == null)
                throw new IllegalArgumentException("Receiver cannot be null if you specify a method");
            return super.disconnect(receiver, method);
        }
        
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         */
        public final io.qt.core.QMetaObject.@NonNull Connection connect(io.qt.core.QMetaObject.@StrictNonNull Slot0 slot, io.qt.core.Qt.ConnectionType... connectionType) throws io.qt.QNoSuchSignalException{
            return super.connect(slot, connectionType);
        }
    
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(io.qt.core.QMetaObject.@StrictNonNull Slot0 slot) {
            return super.disconnect(slot);
        }
        
        @io.qt.QtUninvokable
        private QMetaObject.@NonNull Connection connectSlot(QMetaObject.@StrictNonNull AbstractSlot slotObject, Qt.ConnectionType[] connectionType) {
            return super.connect(slotObject, connectionType);
        }
        
        @io.qt.QtUninvokable
        private boolean disconnectSlot(QMetaObject.@StrictNonNull AbstractSlot slotObject) {
            return super.disconnect(slotObject);
        }
        
        @io.qt.QtUninvokable
        private QMetaObject.@NonNull Connection connectSignal(QMetaObject.@StrictNonNull AbstractSignal slotObject, Qt.ConnectionType[] connectionType) {
            return super.connect(slotObject, connectionType);
        }
        
        @io.qt.QtUninvokable
        private boolean disconnectSignal(QMetaObject.@StrictNonNull AbstractSignal slotObject) {
            return super.disconnect(slotObject);
        }
    }
    
    public static void emit(@StrictNonNull PrivateSignal0 signal) throws io.qt.QSignalAccessException {
    	Class<?> callerClass = QtJambi_LibraryUtilities.internal.callerClassProvider().get();
    	Class<?> signalDeclaringClass = signal.signalDeclaringClass();
    	if(callerClass==signalDeclaringClass || (MultiSignal.class.isAssignableFrom(callerClass) && callerClass.getEnclosingClass()==signalDeclaringClass)) {
    		signal.emit();
    	}else {
    		throw new io.qt.QSignalAccessException(signalDeclaringClass, signal.name(), callerClass);
    	}
    }
    
    public static <A> void emit(@StrictNonNull PrivateSignal1<A> signal, A arg1) throws io.qt.QSignalAccessException {
    	Class<?> callerClass = QtJambi_LibraryUtilities.internal.callerClassProvider().get();
    	Class<?> signalDeclaringClass = signal.signalDeclaringClass();
    	if(callerClass==signalDeclaringClass || (MultiSignal.class.isAssignableFrom(callerClass) && callerClass.getEnclosingClass()==signalDeclaringClass)) {
    		signal.emit(arg1);
    	}else {
    		throw new io.qt.QSignalAccessException(signalDeclaringClass, signal.name(), callerClass);
    	}
    }
    
    public static <A,B> void emit(@StrictNonNull PrivateSignal2<A,B> signal, A arg1, B arg2) throws io.qt.QSignalAccessException {
    	Class<?> callerClass = QtJambi_LibraryUtilities.internal.callerClassProvider().get();
    	Class<?> signalDeclaringClass = signal.signalDeclaringClass();
    	if(callerClass==signalDeclaringClass || (MultiSignal.class.isAssignableFrom(callerClass) && callerClass.getEnclosingClass()==signalDeclaringClass)) {
    		signal.emit(arg1, arg2);
    	}else {
    		throw new io.qt.QSignalAccessException(signalDeclaringClass, signal.name(), callerClass);
    	}
    }
    
    public static <A,B,C> void emit(@StrictNonNull PrivateSignal3<A,B,C> signal, A arg1, B arg2, C arg3) throws io.qt.QSignalAccessException {
    	Class<?> callerClass = QtJambi_LibraryUtilities.internal.callerClassProvider().get();
    	Class<?> signalDeclaringClass = signal.signalDeclaringClass();
    	if(callerClass==signalDeclaringClass || (MultiSignal.class.isAssignableFrom(callerClass) && callerClass.getEnclosingClass()==signalDeclaringClass)) {
    		signal.emit(arg1, arg2, arg3);
    	}else {
    		throw new io.qt.QSignalAccessException(signalDeclaringClass, signal.name(), callerClass);
    	}
    }
    
    public static <A,B,C,D> void emit(@StrictNonNull PrivateSignal4<A,B,C,D> signal, A arg1, B arg2, C arg3, D arg4) throws io.qt.QSignalAccessException {
    	Class<?> callerClass = QtJambi_LibraryUtilities.internal.callerClassProvider().get();
    	Class<?> signalDeclaringClass = signal.signalDeclaringClass();
    	if(callerClass==signalDeclaringClass || (MultiSignal.class.isAssignableFrom(callerClass) && callerClass.getEnclosingClass()==signalDeclaringClass)) {
    		signal.emit(arg1, arg2, arg3, arg4);
    	}else {
    		throw new io.qt.QSignalAccessException(signalDeclaringClass, signal.name(), callerClass);
    	}
    }
    
    public static <A,B,C,D,E> void emit(@StrictNonNull PrivateSignal5<A,B,C,D,E> signal, A arg1, B arg2, C arg3, D arg4, E arg5) throws io.qt.QSignalAccessException {
    	Class<?> callerClass = QtJambi_LibraryUtilities.internal.callerClassProvider().get();
    	Class<?> signalDeclaringClass = signal.signalDeclaringClass();
    	if(callerClass==signalDeclaringClass || (MultiSignal.class.isAssignableFrom(callerClass) && callerClass.getEnclosingClass()==signalDeclaringClass)) {
    		signal.emit(arg1, arg2, arg3, arg4, arg5);
    	}else {
    		throw new io.qt.QSignalAccessException(signalDeclaringClass, signal.name(), callerClass);
    	}
    }
    
    public static <A,B,C,D,E,F> void emit(@StrictNonNull PrivateSignal6<A,B,C,D,E,F> signal, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6) throws io.qt.QSignalAccessException {
    	Class<?> callerClass = QtJambi_LibraryUtilities.internal.callerClassProvider().get();
    	Class<?> signalDeclaringClass = signal.signalDeclaringClass();
    	if(callerClass==signalDeclaringClass || (MultiSignal.class.isAssignableFrom(callerClass) && callerClass.getEnclosingClass()==signalDeclaringClass)) {
    		signal.emit(arg1, arg2, arg3, arg4, arg5, arg6);
    	}else {
    		throw new io.qt.QSignalAccessException(signalDeclaringClass, signal.name(), callerClass);
    	}
    }
    
    public static <A,B,C,D,E,F,G> void emit(@StrictNonNull PrivateSignal7<A,B,C,D,E,F,G> signal, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7) throws io.qt.QSignalAccessException {
    	Class<?> callerClass = QtJambi_LibraryUtilities.internal.callerClassProvider().get();
    	Class<?> signalDeclaringClass = signal.signalDeclaringClass();
    	if(callerClass==signalDeclaringClass || (MultiSignal.class.isAssignableFrom(callerClass) && callerClass.getEnclosingClass()==signalDeclaringClass)) {
    		signal.emit(arg1, arg2, arg3, arg4, arg5, arg6, arg7);
    	}else {
    		throw new io.qt.QSignalAccessException(signalDeclaringClass, signal.name(), callerClass);
    	}
    }
    
    public static <A,B,C,D,E,F,G,H> void emit(@StrictNonNull PrivateSignal8<A,B,C,D,E,F,G,H> signal, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7, H arg8) throws io.qt.QSignalAccessException {
    	Class<?> callerClass = QtJambi_LibraryUtilities.internal.callerClassProvider().get();
    	Class<?> signalDeclaringClass = signal.signalDeclaringClass();
    	if(callerClass==signalDeclaringClass || (MultiSignal.class.isAssignableFrom(callerClass) && callerClass.getEnclosingClass()==signalDeclaringClass)) {
    		signal.emit(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
    	}else {
    		throw new io.qt.QSignalAccessException(signalDeclaringClass, signal.name(), callerClass);
    	}
    }
    
    public static <A,B,C,D,E,F,G,H,I> void emit(@StrictNonNull PrivateSignal9<A,B,C,D,E,F,G,H,I> signal, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7, H arg8, I arg9) throws io.qt.QSignalAccessException {
    	Class<?> callerClass = QtJambi_LibraryUtilities.internal.callerClassProvider().get();
    	Class<?> signalDeclaringClass = signal.signalDeclaringClass();
    	if(callerClass==signalDeclaringClass || (MultiSignal.class.isAssignableFrom(callerClass) && callerClass.getEnclosingClass()==signalDeclaringClass)) {
    		signal.emit(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
    	}else {
    		throw new io.qt.QSignalAccessException(signalDeclaringClass, signal.name(), callerClass);
    	}
    }
}
