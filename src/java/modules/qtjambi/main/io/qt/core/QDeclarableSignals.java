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
package io.qt.core;

import java.util.Arrays;
import java.util.function.Consumer;
import java.util.function.Supplier;

import io.qt.*;
import io.qt.core.QMetaObject.*;

/**
 * QDeclarableSignals is a namespace containing signal types to be used in any other contexts than member signals.
 * Declarable signals are never realized as Qt's meta object signals but have a lightweight Java implementation.
 */
public final class QDeclarableSignals {
	private QDeclarableSignals() {}
	
    /**
     * Use this signal class to declare a local signal.
     */
    public static final class Signal0 extends QMetaObject.AbstractPublicSignal0 {
        
		public Signal0() {
			super(new Class<?>[0]);
		}
    	
    	public Signal0(String signalName) {
			super(signalName, new Class<?>[0]);
		}
    }
    
    /**
     * Use this signal class to declare a local signal.
     *
     * @param <A> The type of the single parameter of the signal.
     */
    public static final class Signal1<A> extends QMetaObject.AbstractPublicSignal1<A> {
        
		public Signal1(@StrictNonNull Class<A> typeA) {
			super(new Class[]{typeA});
		}
    	
    	public Signal1(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA) {
			super(signalName, typeA);
		}
    }
    
    /**
     * Use this signal class to declare a local signal.
     *
     * @param <A> The type of the single parameter of the signal.
     * @param <B> The type of the single parameter of the signal.
     */
    public static final class Signal2<A,B> extends QMetaObject.AbstractPublicSignal2<A,B> {
        
		public Signal2(@StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB) {
			super(typeA, typeB);
		}
    	
    	public Signal2(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB) {
			super(signalName, typeA, typeB);
		}
    }
    
    /**
     * Use this signal class to declare a local signal.
     *
     * @param <A> The type of the single parameter of the signal.
     * @param <B> The type of the single parameter of the signal.
     * @param <C> The type of the single parameter of the signal.
     */
    public static final class Signal3<A,B,C> extends QMetaObject.AbstractPublicSignal3<A,B,C> {
        
		public Signal3(@StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC) {
			super(typeA, typeB, typeC);
		}
    	
    	public Signal3(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC) {
			super(signalName, typeA, typeB, typeC);
		}
    }
    
    /**
     * Use this signal class to declare a local signal.
     *
     * @param <A> The type of the single parameter of the signal.
     * @param <B> The type of the single parameter of the signal.
     * @param <C> The type of the single parameter of the signal.
     * @param <D> The type of the single parameter of the signal.
     */
    public static final class Signal4<A,B,C,D> extends QMetaObject.AbstractPublicSignal4<A,B,C,D> {
        
		public Signal4(@StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD) {
			super(typeA, typeB, typeC, typeD);
		}
    	
    	public Signal4(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD) {
			super(signalName, typeA, typeB, typeC, typeD);
		}
    }
    
    /**
     * Use this signal class to declare a local signal.
     *
     * @param <A> The type of the single parameter of the signal.
     * @param <B> The type of the single parameter of the signal.
     * @param <C> The type of the single parameter of the signal.
     * @param <D> The type of the single parameter of the signal.
     * @param <E> The type of the single parameter of the signal.
     */
    public static final class Signal5<A,B,C,D,E> extends QMetaObject.AbstractPublicSignal5<A,B,C,D,E> {
        
		public Signal5(@StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE) {
			super(typeA, typeB, typeC, typeD, typeE);
		}
    	
    	public Signal5(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE) {
			super(signalName, typeA, typeB, typeC, typeD, typeE);
		}
    }
    
    /**
     * Use this signal class to declare a local signal.
     *
     * @param <A> The type of the single parameter of the signal.
     * @param <B> The type of the single parameter of the signal.
     * @param <C> The type of the single parameter of the signal.
     * @param <D> The type of the single parameter of the signal.
     * @param <E> The type of the single parameter of the signal.
     * @param <F> The type of the single parameter of the signal.
     */
    public static final class Signal6<A,B,C,D,E,F> extends QMetaObject.AbstractPublicSignal6<A,B,C,D,E,F> {
        
		public Signal6(@StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF) {
			super(typeA, typeB, typeC, typeD, typeE, typeF);
		}
    	
    	public Signal6(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF) {
			super(signalName, typeA, typeB, typeC, typeD, typeE, typeF);
		}
    }
    
    /**
     * Use this signal class to declare a local signal.
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
        
		public Signal7(@StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Class<G> typeG) {
			super(typeA, typeB, typeC, typeD, typeE, typeF, typeG);
		}
    	
    	public Signal7(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Class<G> typeG) {
			super(signalName, typeA, typeB, typeC, typeD, typeE, typeF, typeG);
		}
    }
    
    /**
     * Use this signal class to declare a local signal.
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
        
		public Signal8(@StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Class<G> typeG, @StrictNonNull Class<H> typeH) {
			super(typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH);
		}
    	
    	public Signal8(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Class<G> typeG, @StrictNonNull Class<H> typeH) {
			super(signalName, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH);
		}
    }
    
    /**
     * Use this signal class to declare a local signal.
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
        
		public Signal9(@StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Class<G> typeG, @StrictNonNull Class<H> typeH, @StrictNonNull Class<I> typeI) {
			super(typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH, typeI);
		}
    	
    	public Signal9(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Class<G> typeG, @StrictNonNull Class<H> typeH, @StrictNonNull Class<I> typeI) {
			super(signalName, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH, typeI);
		}
    }

    /**
     * Equivalent to {@link Signal1} with default value for the parameter.
     *
     * @param <A> The type of the first parameter of the signal.
     */
    public static final class Signal1Default1<A> extends QMetaObject.AbstractSignal1Default1<A>{
        
		public Signal1Default1(@StrictNonNull Class<A> typeA, @StrictNonNull Supplier<A> arg1Default){
    		super(arg1Default, new Class[]{typeA});
    	}
    	
    	public Signal1Default1(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Supplier<A> arg1Default){
    		super(signalName, arg1Default, typeA);
    	}
    }
    
    /**
     * Equivalent to {@link Signal2} with default value for parameter no. 2.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     */
    public static final class Signal2Default1<A, B> extends QMetaObject.AbstractSignal2Default1<A, B>{
        
		public Signal2Default1(@StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Supplier<B> arg2Default) {
			super(arg2Default, typeA, typeB);
		}
		
		public Signal2Default1(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Supplier<B> arg2Default) {
			super(signalName, arg2Default, typeA, typeB);
		}
	}
    
    /**
     * Equivalent to {@link Signal2} with default values for parameters 1 and 2.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     */
    public static final class Signal2Default2<A, B> extends QMetaObject.AbstractSignal2Default2<A, B>{
        
		public Signal2Default2(@StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Supplier<A> arg1Default, @StrictNonNull Supplier<B> arg2Default){
    		super(arg1Default, arg2Default, typeA, typeB);
    	}
    	
    	public Signal2Default2(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Supplier<A> arg1Default, @StrictNonNull Supplier<B> arg2Default){
    		super(signalName, arg1Default, arg2Default, typeA, typeB);
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
        
		public Signal3Default1(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Supplier<C> arg3Default) {
			super(signalName, arg3Default, typeA, typeB, typeC);
		}
		
		public Signal3Default1(@StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Supplier<C> arg3Default) {
			super(arg3Default, typeA, typeB, typeC);
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
        
		public Signal3Default2(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Supplier<B> arg2Default, @StrictNonNull Supplier<C> arg3Default){
    		super(signalName, arg2Default, arg3Default, typeA, typeB, typeC);
    	}
    	
    	public Signal3Default2(@StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Supplier<B> arg2Default, @StrictNonNull Supplier<C> arg3Default){
    		super(arg2Default, arg3Default, typeA, typeB, typeC);
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
        
		public Signal3Default3(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Supplier<A> arg1Default, @StrictNonNull Supplier<B> arg2Default, @StrictNonNull Supplier<C> arg3Default){
    		super(signalName, arg1Default, arg2Default, arg3Default, typeA, typeB, typeC);
    	}
    	
    	public Signal3Default3(@StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Supplier<A> arg1Default, @StrictNonNull Supplier<B> arg2Default, @StrictNonNull Supplier<C> arg3Default){
    		super(arg1Default, arg2Default, arg3Default, typeA, typeB, typeC);
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
        
		public Signal4Default1(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Supplier<D> arg4Default){
    		super(signalName, arg4Default, typeA, typeB, typeC, typeD);
    	}
    	
    	public Signal4Default1(@StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Supplier<D> arg4Default){
    		super(arg4Default, typeA, typeB, typeC, typeD);
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
        
		public Signal4Default2(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Supplier<C> arg3Default, @StrictNonNull Supplier<D> arg4Default){
    		super(signalName, arg3Default, arg4Default, typeA, typeB, typeC, typeD);
    	}
    	
    	public Signal4Default2(@StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Supplier<C> arg3Default, @StrictNonNull Supplier<D> arg4Default){
    		super(arg3Default, arg4Default, typeA, typeB, typeC, typeD);
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
        
		public Signal4Default3(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Supplier<B> arg2Default, @StrictNonNull Supplier<C> arg3Default, @StrictNonNull Supplier<D> arg4Default){
    		super(signalName, arg2Default, arg3Default, arg4Default, typeA, typeB, typeC, typeD);
    	}
    	
    	public Signal4Default3(@StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Supplier<B> arg2Default, @StrictNonNull Supplier<C> arg3Default, @StrictNonNull Supplier<D> arg4Default){
    		super(arg2Default, arg3Default, arg4Default, typeA, typeB, typeC, typeD);
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
        
		public Signal4Default4(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, 
				@StrictNonNull Supplier<A> arg1Default, @StrictNonNull Supplier<B> arg2Default, 
				@StrictNonNull Supplier<C> arg3Default, @StrictNonNull Supplier<D> arg4Default){
    		super(signalName, arg1Default, arg2Default, arg3Default, arg4Default, typeA, typeB, typeC, typeD);
    	}
    	
    	public Signal4Default4(@StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, 
    			@StrictNonNull Supplier<A> arg1Default, @StrictNonNull Supplier<B> arg2Default, 
    			@StrictNonNull Supplier<C> arg3Default, @StrictNonNull Supplier<D> arg4Default){
    		super(arg1Default, arg2Default, arg3Default, arg4Default, typeA, typeB, typeC, typeD);
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
        		public Signal5Default1(@StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Supplier<E> arg5Default){
    		super(arg5Default, typeA, typeB, typeC, typeD, typeE);
    	}
    	public Signal5Default1(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Supplier<E> arg5Default){
    		super(signalName, arg5Default, typeA, typeB, typeC, typeD, typeE);
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
        		public Signal5Default2(@StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Supplier<D> arg4Default, @StrictNonNull Supplier<E> arg5Default){
    		super(arg4Default, arg5Default, typeA, typeB, typeC, typeD, typeE);
    	}
    	public Signal5Default2(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Supplier<D> arg4Default, @StrictNonNull Supplier<E> arg5Default){
    		super(signalName, arg4Default, arg5Default, typeA, typeB, typeC, typeD, typeE);
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
        		public Signal5Default3(@StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Supplier<C> arg3Default, @StrictNonNull Supplier<D> arg4Default, @StrictNonNull Supplier<E> arg5Default){
    		super(arg3Default, arg4Default, arg5Default, typeA, typeB, typeC, typeD, typeE);
    	}
    	public Signal5Default3(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Supplier<C> arg3Default, @StrictNonNull Supplier<D> arg4Default, @StrictNonNull Supplier<E> arg5Default){
    		super(signalName, arg3Default, arg4Default, arg5Default, typeA, typeB, typeC, typeD, typeE);
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
        		public Signal5Default4(@StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Supplier<B> arg2Default, @StrictNonNull Supplier<C> arg3Default, @StrictNonNull Supplier<D> arg4Default, @StrictNonNull Supplier<E> arg5Default){
    		super(arg2Default, arg3Default, arg4Default, arg5Default, typeA, typeB, typeC, typeD, typeE);
    	}
    	public Signal5Default4(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Supplier<B> arg2Default, @StrictNonNull Supplier<C> arg3Default, @StrictNonNull Supplier<D> arg4Default, @StrictNonNull Supplier<E> arg5Default){
    		super(signalName, arg2Default, arg3Default, arg4Default, arg5Default, typeA, typeB, typeC, typeD, typeE);
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
		public Signal5Default5(@StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Supplier<A> arg1Default, @StrictNonNull Supplier<B> arg2Default, @StrictNonNull Supplier<C> arg3Default, @StrictNonNull Supplier<D> arg4Default, @StrictNonNull Supplier<E> arg5Default){
    		super(arg1Default, arg2Default, arg3Default, arg4Default, arg5Default, typeA, typeB, typeC, typeD, typeE);
    	}
    	public Signal5Default5(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Supplier<A> arg1Default, @StrictNonNull Supplier<B> arg2Default, @StrictNonNull Supplier<C> arg3Default, @StrictNonNull Supplier<D> arg4Default, @StrictNonNull Supplier<E> arg5Default){
    		super(arg1Default, arg2Default, arg3Default, arg4Default, arg5Default, typeA, typeB, typeC, typeD, typeE);
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
        		public Signal6Default1(@StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Supplier<F> arg6Default){
    		super(arg6Default, typeA, typeB, typeC, typeD, typeE, typeF);
    	}
    	public Signal6Default1(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Supplier<F> arg6Default){
    		super(signalName, arg6Default, typeA, typeB, typeC, typeD, typeE, typeF);
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
        		public Signal6Default2(@StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Supplier<E> arg5Default, @StrictNonNull Supplier<F> arg6Default){
    		super(arg5Default, arg6Default, typeA, typeB, typeC, typeD, typeE, typeF);
    	}
    	public Signal6Default2(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Supplier<E> arg5Default, @StrictNonNull Supplier<F> arg6Default){
    		super(signalName, arg5Default, arg6Default, typeA, typeB, typeC, typeD, typeE, typeF);
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
        		public Signal6Default3(@StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Supplier<D> arg4Default, @StrictNonNull Supplier<E> arg5Default, @StrictNonNull Supplier<F> arg6Default){
    		super(arg4Default, arg5Default, arg6Default, typeA, typeB, typeC, typeD, typeE, typeF);
    	}
    	public Signal6Default3(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Supplier<D> arg4Default, @StrictNonNull Supplier<E> arg5Default, @StrictNonNull Supplier<F> arg6Default){
    		super(signalName, arg4Default, arg5Default, arg6Default, typeA, typeB, typeC, typeD, typeE, typeF);
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
        		public Signal6Default4(@StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Supplier<C> arg3Default, @StrictNonNull Supplier<D> arg4Default, @StrictNonNull Supplier<E> arg5Default, @StrictNonNull Supplier<F> arg6Default){
    		super(arg3Default, arg4Default, arg5Default, arg6Default, typeA, typeB, typeC, typeD, typeE, typeF);
    	}
    	public Signal6Default4(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Supplier<C> arg3Default, @StrictNonNull Supplier<D> arg4Default, @StrictNonNull Supplier<E> arg5Default, @StrictNonNull Supplier<F> arg6Default){
    		super(signalName, arg3Default, arg4Default, arg5Default, arg6Default, typeA, typeB, typeC, typeD, typeE, typeF);
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
        		public Signal6Default5(@StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Supplier<B> arg2Default, @StrictNonNull Supplier<C> arg3Default, @StrictNonNull Supplier<D> arg4Default, @StrictNonNull Supplier<E> arg5Default, @StrictNonNull Supplier<F> arg6Default){
    		super(arg2Default, arg3Default, arg4Default, arg5Default, arg6Default, typeA, typeB, typeC, typeD, typeE, typeF);
    	}
    	public Signal6Default5(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Supplier<B> arg2Default, @StrictNonNull Supplier<C> arg3Default, @StrictNonNull Supplier<D> arg4Default, @StrictNonNull Supplier<E> arg5Default, @StrictNonNull Supplier<F> arg6Default){
    		super(signalName, arg2Default, arg3Default, arg4Default, arg5Default, arg6Default, typeA, typeB, typeC, typeD, typeE, typeF);
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
		public Signal6Default6(@StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Supplier<A> arg1Default, @StrictNonNull Supplier<B> arg2Default, @StrictNonNull Supplier<C> arg3Default, @StrictNonNull Supplier<D> arg4Default, @StrictNonNull Supplier<E> arg5Default, @StrictNonNull Supplier<F> arg6Default){
    		super(arg1Default, arg2Default, arg3Default, arg4Default, arg5Default, arg6Default, typeA, typeB, typeC, typeD, typeE, typeF);
    	}
    	public Signal6Default6(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Supplier<A> arg1Default, @StrictNonNull Supplier<B> arg2Default, @StrictNonNull Supplier<C> arg3Default, @StrictNonNull Supplier<D> arg4Default, @StrictNonNull Supplier<E> arg5Default, @StrictNonNull Supplier<F> arg6Default){
    		super(signalName, arg1Default, arg2Default, arg3Default, arg4Default, arg5Default, arg6Default, typeA, typeB, typeC, typeD, typeE, typeF);
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
        		public Signal7Default1(@StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Class<G> typeG, @StrictNonNull Supplier<G> arg7Default){
    		super(arg7Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG);
    	}
    	public Signal7Default1(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Class<G> typeG, @StrictNonNull Supplier<G> arg7Default){
    		super(signalName, arg7Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG);
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
        		public Signal7Default2(@StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Class<G> typeG, @StrictNonNull Supplier<F> arg6Default, @StrictNonNull Supplier<G> arg7Default){
    		super(arg6Default, arg7Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG);
    	}
    	public Signal7Default2(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Class<G> typeG, @StrictNonNull Supplier<F> arg6Default, @StrictNonNull Supplier<G> arg7Default){
    		super(signalName, arg6Default, arg7Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG);
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
        		public Signal7Default3(@StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Class<G> typeG, @StrictNonNull Supplier<E> arg5Default, @StrictNonNull Supplier<F> arg6Default, @StrictNonNull Supplier<G> arg7Default){
    		super(arg5Default, arg6Default, arg7Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG);
    	}
    	public Signal7Default3(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Class<G> typeG, @StrictNonNull Supplier<E> arg5Default, @StrictNonNull Supplier<F> arg6Default, @StrictNonNull Supplier<G> arg7Default){
    		super(signalName, arg5Default, arg6Default, arg7Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG);
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
        		public Signal7Default4(@StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Class<G> typeG, @StrictNonNull Supplier<D> arg4Default, @StrictNonNull Supplier<E> arg5Default, @StrictNonNull Supplier<F> arg6Default, @StrictNonNull Supplier<G> arg7Default){
    		super(arg4Default, arg5Default, arg6Default, arg7Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG);
    	}
    	public Signal7Default4(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Class<G> typeG, @StrictNonNull Supplier<D> arg4Default, @StrictNonNull Supplier<E> arg5Default, @StrictNonNull Supplier<F> arg6Default, @StrictNonNull Supplier<G> arg7Default){
    		super(signalName, arg4Default, arg5Default, arg6Default, arg7Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG);
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
        		public Signal7Default5(@StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Class<G> typeG, @StrictNonNull Supplier<C> arg3Default, @StrictNonNull Supplier<D> arg4Default, @StrictNonNull Supplier<E> arg5Default, @StrictNonNull Supplier<F> arg6Default, @StrictNonNull Supplier<G> arg7Default){
    		super(arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG);
    	}
    	public Signal7Default5(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Class<G> typeG, @StrictNonNull Supplier<C> arg3Default, @StrictNonNull Supplier<D> arg4Default, @StrictNonNull Supplier<E> arg5Default, @StrictNonNull Supplier<F> arg6Default, @StrictNonNull Supplier<G> arg7Default){
    		super(signalName, arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG);
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
        		public Signal7Default6(@StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Class<G> typeG, @StrictNonNull Supplier<B> arg2Default, @StrictNonNull Supplier<C> arg3Default, @StrictNonNull Supplier<D> arg4Default, @StrictNonNull Supplier<E> arg5Default, @StrictNonNull Supplier<F> arg6Default, @StrictNonNull Supplier<G> arg7Default){
    		super(arg2Default, arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG);
    	}
    	public Signal7Default6(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Class<G> typeG, @StrictNonNull Supplier<B> arg2Default, @StrictNonNull Supplier<C> arg3Default, @StrictNonNull Supplier<D> arg4Default, @StrictNonNull Supplier<E> arg5Default, @StrictNonNull Supplier<F> arg6Default, @StrictNonNull Supplier<G> arg7Default){
    		super(signalName, arg2Default, arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG);
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
		public Signal7Default7(@StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Class<G> typeG, @StrictNonNull Supplier<A> arg1Default, @StrictNonNull Supplier<B> arg2Default, @StrictNonNull Supplier<C> arg3Default, @StrictNonNull Supplier<D> arg4Default, @StrictNonNull Supplier<E> arg5Default, @StrictNonNull Supplier<F> arg6Default, @StrictNonNull Supplier<G> arg7Default){
    		super(arg1Default, arg2Default, arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG);
    	}
    	public Signal7Default7(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Class<G> typeG, @StrictNonNull Supplier<A> arg1Default, @StrictNonNull Supplier<B> arg2Default, @StrictNonNull Supplier<C> arg3Default, @StrictNonNull Supplier<D> arg4Default, @StrictNonNull Supplier<E> arg5Default, @StrictNonNull Supplier<F> arg6Default, @StrictNonNull Supplier<G> arg7Default){
    		super(signalName, arg1Default, arg2Default, arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG);
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
        		public Signal8Default1(@StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Class<G> typeG, @StrictNonNull Class<H> typeH, @StrictNonNull Supplier<H> arg8Default){
    		super(arg8Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH);
    	}
    	public Signal8Default1(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Class<G> typeG, @StrictNonNull Class<H> typeH, @StrictNonNull Supplier<H> arg8Default){
    		super(signalName, arg8Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH);
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
        		public Signal8Default2(@StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Class<G> typeG, @StrictNonNull Class<H> typeH, @StrictNonNull Supplier<G> arg7Default, @StrictNonNull Supplier<H> arg8Default){
    		super(arg7Default, arg8Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH);
    	}
    	public Signal8Default2(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Class<G> typeG, @StrictNonNull Class<H> typeH, @StrictNonNull Supplier<G> arg7Default, @StrictNonNull Supplier<H> arg8Default){
    		super(signalName, arg7Default, arg8Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH);
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
        		public Signal8Default3(@StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Class<G> typeG, @StrictNonNull Class<H> typeH, @StrictNonNull Supplier<F> arg6Default, @StrictNonNull Supplier<G> arg7Default, @StrictNonNull Supplier<H> arg8Default){
    		super(arg6Default, arg7Default, arg8Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH);
    	}
    	public Signal8Default3(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Class<G> typeG, @StrictNonNull Class<H> typeH, @StrictNonNull Supplier<F> arg6Default, @StrictNonNull Supplier<G> arg7Default, @StrictNonNull Supplier<H> arg8Default){
    		super(signalName, arg6Default, arg7Default, arg8Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH);
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
        		public Signal8Default4(@StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Class<G> typeG, @StrictNonNull Class<H> typeH, @StrictNonNull Supplier<E> arg5Default, @StrictNonNull Supplier<F> arg6Default, @StrictNonNull Supplier<G> arg7Default, @StrictNonNull Supplier<H> arg8Default){
    		super(arg5Default, arg6Default, arg7Default, arg8Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH);
    	}
    	public Signal8Default4(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Class<G> typeG, @StrictNonNull Class<H> typeH, @StrictNonNull Supplier<E> arg5Default, @StrictNonNull Supplier<F> arg6Default, @StrictNonNull Supplier<G> arg7Default, @StrictNonNull Supplier<H> arg8Default){
    		super(signalName, arg5Default, arg6Default, arg7Default, arg8Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH);
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
        		public Signal8Default5(@StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Class<G> typeG, @StrictNonNull Class<H> typeH, @StrictNonNull Supplier<D> arg4Default, @StrictNonNull Supplier<E> arg5Default, @StrictNonNull Supplier<F> arg6Default, @StrictNonNull Supplier<G> arg7Default, @StrictNonNull Supplier<H> arg8Default){
    		super(arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH);
    	}
    	public Signal8Default5(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Class<G> typeG, @StrictNonNull Class<H> typeH, @StrictNonNull Supplier<D> arg4Default, @StrictNonNull Supplier<E> arg5Default, @StrictNonNull Supplier<F> arg6Default, @StrictNonNull Supplier<G> arg7Default, @StrictNonNull Supplier<H> arg8Default){
    		super(signalName, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH);
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
        		public Signal8Default6(@StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Class<G> typeG, @StrictNonNull Class<H> typeH, @StrictNonNull Supplier<C> arg3Default, @StrictNonNull Supplier<D> arg4Default, @StrictNonNull Supplier<E> arg5Default, @StrictNonNull Supplier<F> arg6Default, @StrictNonNull Supplier<G> arg7Default, @StrictNonNull Supplier<H> arg8Default){
    		super(arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH);
    	}
    	public Signal8Default6(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Class<G> typeG, @StrictNonNull Class<H> typeH, @StrictNonNull Supplier<C> arg3Default, @StrictNonNull Supplier<D> arg4Default, @StrictNonNull Supplier<E> arg5Default, @StrictNonNull Supplier<F> arg6Default, @StrictNonNull Supplier<G> arg7Default, @StrictNonNull Supplier<H> arg8Default){
    		super(signalName, arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH);
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
        		public Signal8Default7(@StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Class<G> typeG, @StrictNonNull Class<H> typeH, @StrictNonNull Supplier<B> arg2Default, @StrictNonNull Supplier<C> arg3Default, @StrictNonNull Supplier<D> arg4Default, @StrictNonNull Supplier<E> arg5Default, @StrictNonNull Supplier<F> arg6Default, @StrictNonNull Supplier<G> arg7Default, @StrictNonNull Supplier<H> arg8Default){
    		super(arg2Default, arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH);
    	}
    	public Signal8Default7(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Class<G> typeG, @StrictNonNull Class<H> typeH, @StrictNonNull Supplier<B> arg2Default, @StrictNonNull Supplier<C> arg3Default, @StrictNonNull Supplier<D> arg4Default, @StrictNonNull Supplier<E> arg5Default, @StrictNonNull Supplier<F> arg6Default, @StrictNonNull Supplier<G> arg7Default, @StrictNonNull Supplier<H> arg8Default){
    		super(signalName, arg2Default, arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH);
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
		public Signal8Default8(@StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Class<G> typeG, @StrictNonNull Class<H> typeH, @StrictNonNull Supplier<A> arg1Default, @StrictNonNull Supplier<B> arg2Default, @StrictNonNull Supplier<C> arg3Default, @StrictNonNull Supplier<D> arg4Default, @StrictNonNull Supplier<E> arg5Default, @StrictNonNull Supplier<F> arg6Default, @StrictNonNull Supplier<G> arg7Default, @StrictNonNull Supplier<H> arg8Default){
    		super(arg1Default, arg2Default, arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH);
    	}
    	public Signal8Default8(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Class<G> typeG, @StrictNonNull Class<H> typeH, @StrictNonNull Supplier<A> arg1Default, @StrictNonNull Supplier<B> arg2Default, @StrictNonNull Supplier<C> arg3Default, @StrictNonNull Supplier<D> arg4Default, @StrictNonNull Supplier<E> arg5Default, @StrictNonNull Supplier<F> arg6Default, @StrictNonNull Supplier<G> arg7Default, @StrictNonNull Supplier<H> arg8Default){
    		super(signalName, arg1Default, arg2Default, arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH);
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
        		public Signal9Default1(@StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Class<G> typeG, @StrictNonNull Class<H> typeH, @StrictNonNull Class<I> typeI, @StrictNonNull Supplier<I> arg9Default){
    		super(arg9Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH, typeI);
    	}
    	public Signal9Default1(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Class<G> typeG, @StrictNonNull Class<H> typeH, @StrictNonNull Class<I> typeI, @StrictNonNull Supplier<I> arg9Default){
    		super(signalName, arg9Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH, typeI);
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
        		public Signal9Default2(@StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Class<G> typeG, @StrictNonNull Class<H> typeH, @StrictNonNull Class<I> typeI, @StrictNonNull Supplier<H> arg8Default, @StrictNonNull Supplier<I> arg9Default){
    		super(arg8Default, arg9Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH, typeI);
    	}
    	public Signal9Default2(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Class<G> typeG, @StrictNonNull Class<H> typeH, @StrictNonNull Class<I> typeI, @StrictNonNull Supplier<H> arg8Default, @StrictNonNull Supplier<I> arg9Default){
    		super(signalName, arg8Default, arg9Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH, typeI);
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
        		public Signal9Default3(@StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Class<G> typeG, @StrictNonNull Class<H> typeH, @StrictNonNull Class<I> typeI, @StrictNonNull Supplier<G> arg7Default, @StrictNonNull Supplier<H> arg8Default, @StrictNonNull Supplier<I> arg9Default){
    		super(arg7Default, arg8Default, arg9Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH, typeI);
    	}
    	public Signal9Default3(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Class<G> typeG, @StrictNonNull Class<H> typeH, @StrictNonNull Class<I> typeI, @StrictNonNull Supplier<G> arg7Default, @StrictNonNull Supplier<H> arg8Default, @StrictNonNull Supplier<I> arg9Default){
    		super(signalName, arg7Default, arg8Default, arg9Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH, typeI);
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
        		public Signal9Default4(@StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Class<G> typeG, @StrictNonNull Class<H> typeH, @StrictNonNull Class<I> typeI, @StrictNonNull Supplier<F> arg6Default, @StrictNonNull Supplier<G> arg7Default, @StrictNonNull Supplier<H> arg8Default, @StrictNonNull Supplier<I> arg9Default){
    		super(arg6Default, arg7Default, arg8Default, arg9Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH, typeI);
    	}
    	public Signal9Default4(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Class<G> typeG, @StrictNonNull Class<H> typeH, @StrictNonNull Class<I> typeI, @StrictNonNull Supplier<F> arg6Default, @StrictNonNull Supplier<G> arg7Default, @StrictNonNull Supplier<H> arg8Default, @StrictNonNull Supplier<I> arg9Default){
    		super(signalName, arg6Default, arg7Default, arg8Default, arg9Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH, typeI);
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
        		public Signal9Default5(@StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Class<G> typeG, @StrictNonNull Class<H> typeH, @StrictNonNull Class<I> typeI, @StrictNonNull Supplier<E> arg5Default, @StrictNonNull Supplier<F> arg6Default, @StrictNonNull Supplier<G> arg7Default, @StrictNonNull Supplier<H> arg8Default, @StrictNonNull Supplier<I> arg9Default){
    		super(arg5Default, arg6Default, arg7Default, arg8Default, arg9Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH, typeI);
    	}
    	public Signal9Default5(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Class<G> typeG, @StrictNonNull Class<H> typeH, @StrictNonNull Class<I> typeI, @StrictNonNull Supplier<E> arg5Default, @StrictNonNull Supplier<F> arg6Default, @StrictNonNull Supplier<G> arg7Default, @StrictNonNull Supplier<H> arg8Default, @StrictNonNull Supplier<I> arg9Default){
    		super(signalName, arg5Default, arg6Default, arg7Default, arg8Default, arg9Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH, typeI);
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
        		public Signal9Default6(@StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Class<G> typeG, @StrictNonNull Class<H> typeH, @StrictNonNull Class<I> typeI, @StrictNonNull Supplier<D> arg4Default, @StrictNonNull Supplier<E> arg5Default, @StrictNonNull Supplier<F> arg6Default, @StrictNonNull Supplier<G> arg7Default, @StrictNonNull Supplier<H> arg8Default, @StrictNonNull Supplier<I> arg9Default){
    		super(arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, arg9Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH, typeI);
		}
    	public Signal9Default6(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Class<G> typeG, @StrictNonNull Class<H> typeH, @StrictNonNull Class<I> typeI, @StrictNonNull Supplier<D> arg4Default, @StrictNonNull Supplier<E> arg5Default, @StrictNonNull Supplier<F> arg6Default, @StrictNonNull Supplier<G> arg7Default, @StrictNonNull Supplier<H> arg8Default, @StrictNonNull Supplier<I> arg9Default){
    		super(signalName, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, arg9Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH, typeI);
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
        		public Signal9Default7(@StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Class<G> typeG, @StrictNonNull Class<H> typeH, @StrictNonNull Class<I> typeI, @StrictNonNull Supplier<C> arg3Default, @StrictNonNull Supplier<D> arg4Default, @StrictNonNull Supplier<E> arg5Default, @StrictNonNull Supplier<F> arg6Default, @StrictNonNull Supplier<G> arg7Default, @StrictNonNull Supplier<H> arg8Default, @StrictNonNull Supplier<I> arg9Default){
    		super(arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, arg9Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH, typeI);
    	}
    	public Signal9Default7(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Class<G> typeG, @StrictNonNull Class<H> typeH, @StrictNonNull Class<I> typeI, @StrictNonNull Supplier<C> arg3Default, @StrictNonNull Supplier<D> arg4Default, @StrictNonNull Supplier<E> arg5Default, @StrictNonNull Supplier<F> arg6Default, @StrictNonNull Supplier<G> arg7Default, @StrictNonNull Supplier<H> arg8Default, @StrictNonNull Supplier<I> arg9Default){
    		super(signalName, arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, arg9Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH, typeI);
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
        		public Signal9Default8(@StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Class<G> typeG, @StrictNonNull Class<H> typeH, @StrictNonNull Class<I> typeI, @StrictNonNull Supplier<B> arg2Default, @StrictNonNull Supplier<C> arg3Default, @StrictNonNull Supplier<D> arg4Default, @StrictNonNull Supplier<E> arg5Default, @StrictNonNull Supplier<F> arg6Default, @StrictNonNull Supplier<G> arg7Default, @StrictNonNull Supplier<H> arg8Default, @StrictNonNull Supplier<I> arg9Default){
    		super(arg2Default, arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, arg9Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH, typeI);
    	}
    	public Signal9Default8(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Class<G> typeG, @StrictNonNull Class<H> typeH, @StrictNonNull Class<I> typeI, @StrictNonNull Supplier<B> arg2Default, @StrictNonNull Supplier<C> arg3Default, @StrictNonNull Supplier<D> arg4Default, @StrictNonNull Supplier<E> arg5Default, @StrictNonNull Supplier<F> arg6Default, @StrictNonNull Supplier<G> arg7Default, @StrictNonNull Supplier<H> arg8Default, @StrictNonNull Supplier<I> arg9Default){
    		super(signalName, arg2Default, arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, arg9Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH, typeI);
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
		public Signal9Default9(@StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Class<G> typeG, @StrictNonNull Class<H> typeH, @StrictNonNull Class<I> typeI, @StrictNonNull Supplier<A> arg1Default, @StrictNonNull Supplier<B> arg2Default, @StrictNonNull Supplier<C> arg3Default, @StrictNonNull Supplier<D> arg4Default, @StrictNonNull Supplier<E> arg5Default, @StrictNonNull Supplier<F> arg6Default, @StrictNonNull Supplier<G> arg7Default, @StrictNonNull Supplier<H> arg8Default, @StrictNonNull Supplier<I> arg9Default){
    		super(arg1Default, arg2Default, arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, arg9Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH, typeI);
    	}
    	public Signal9Default9(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Class<G> typeG, @StrictNonNull Class<H> typeH, @StrictNonNull Class<I> typeI, @StrictNonNull Supplier<A> arg1Default, @StrictNonNull Supplier<B> arg2Default, @StrictNonNull Supplier<C> arg3Default, @StrictNonNull Supplier<D> arg4Default, @StrictNonNull Supplier<E> arg5Default, @StrictNonNull Supplier<F> arg6Default, @StrictNonNull Supplier<G> arg7Default, @StrictNonNull Supplier<H> arg8Default, @StrictNonNull Supplier<I> arg9Default){
    		super(signalName, arg1Default, arg2Default, arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, arg9Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH, typeI);
    	}
    }

    /**
     * Use this signal class to declare a local signal.
     */
    public static final class GenericSignal extends AbstractPrivateGenericSignal implements QMetaObject.GenericConnectable {
        
		public GenericSignal(@StrictNonNull Class<?> @StrictNonNull... types) {
			super(types);
		}
    	
    	public final void emit(Object...args) {
    		super.emit(args);
    	}
    }

    /**
     * Use this signal class to declare a local signal.
     */
    public static final class PrivateGenericSignal extends AbstractPrivateGenericSignal {
            	public PrivateGenericSignal(@StrictNonNull Class<?> @StrictNonNull... types) {
			super(types);
		}
    	
    	public PrivateGenericSignal(@StrictNonNull Consumer<@StrictNonNull GenericSlot> emitConsumer, @StrictNonNull Class<?> @StrictNonNull... types) {
			super(emitConsumer, types);
		}
    	
    	public PrivateGenericSignal(@StrictNonNull String signalName, @StrictNonNull Class<?> @StrictNonNull... types) {
			super(signalName, types);
		}
    	
    	public PrivateGenericSignal(@StrictNonNull Consumer<@StrictNonNull GenericSlot> emitConsumer, String signalName, @StrictNonNull Class<?> @StrictNonNull... types) {
			super(emitConsumer, signalName, types);
		}
    }

    /**
     * Use this signal class to declare a local signal.
     */
    public static abstract class AbstractPrivateGenericSignal extends QMetaObject.AbstractSignal {
            	
    	private final Class<?>[] types;
    	
    	AbstractPrivateGenericSignal(@StrictNonNull Class<?> @StrictNonNull... types) {
    		this("anonymous_signal", types);
    	}
    	
    	AbstractPrivateGenericSignal(@StrictNonNull Consumer<@StrictNonNull GenericSlot> emitConsumer, @StrictNonNull Class<?> @StrictNonNull... types) {
			this(emitConsumer, "anonymous_signal", types);
			emitConsumer.accept(this::emit);
		}
    	
    	AbstractPrivateGenericSignal(@StrictNonNull Consumer<@StrictNonNull GenericSlot> emitConsumer, String signalName, @StrictNonNull Class<?> @StrictNonNull... types) {
			this(signalName, types);
			emitConsumer.accept(this::emit);
		}
    	
    	private AbstractPrivateGenericSignal(@StrictNonNull String signalName, @StrictNonNull Class<?>... types) {
			super(signalName, types);
			for (int i = 0; i < types.length; i++) {
				if(types[i] == void.class) {
					throw new IllegalArgumentException("void is not a valid type for signals");
				}else if(types[i] == null) {
					throw new IllegalArgumentException("null is not a valid type for signals");
				}
			}
			this.types = Arrays.copyOf(types, types.length);
		}
    	
    	@QtUninvokable
    	private void emit(Object...args) {
    		if(args.length!=types.length) {
    			throw new IllegalArgumentException("wrong number of arguments");
    		}
    		for (int i = 0; i < args.length; i++) {
    			if(types[i].isPrimitive()) {
    				if(types[i]==int.class) {
    					args[i] = (int)args[i];
    				}else if(types[i]==byte.class) {
    					args[i] = (byte)args[i];
    				}else if(types[i]==boolean.class) {
    					args[i] = (boolean)args[i];
    				}else if(types[i]==char.class) {
    					args[i] = (char)args[i];
    				}else if(types[i]==short.class) {
    					args[i] = (short)args[i];
    				}else if(types[i]==long.class) {
    					args[i] = (long)args[i];
    				}else if(types[i]==double.class) {
    					args[i] = (double)args[i];
    				}else if(types[i]==float.class) {
    					args[i] = (float)args[i];
    				}else {
    					throw new IllegalArgumentException("illegal signal type: "+types[i]);
    				}
    			}else {
    				args[i] = types[i].cast(args[i]);
    			}
			}
    		super.emitSignal(args);
    	}
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         */
        public final QMetaObject.@NonNull Connection connect(@StrictNonNull GenericSlot slot, Qt.@StrictNonNull ConnectionType @StrictNonNull... type) {
            return addConnectionToSlotObject(slot, type);
        }
    
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(@StrictNonNull GenericSlot slot) {
            return removeConnectionToSlotObject(slot);
        }
    
        /**
         * Initializes a connection to the <i>signal</i>.
         * 
         * @param signal the signal to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         */
        public final QMetaObject.@NonNull Connection connect(@StrictNonNull GenericConnectable signal, Qt.@StrictNonNull ConnectionType @StrictNonNull... type) {
            return addConnectionToSignalObject((AbstractSignal)signal, type);
        }
    
        /**
         * Removes the connection to the given <i>signal</i>.
         * 
         * @param signal the signal to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(@StrictNonNull GenericConnectable signal) {
            return removeConnectionToSignalObject((AbstractSignal)signal);
        }
    }
}
