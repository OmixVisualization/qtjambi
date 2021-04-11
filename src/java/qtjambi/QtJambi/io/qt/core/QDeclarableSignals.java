/****************************************************************************
**
** Copyright (C) 2009-2021 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import java.util.Arrays;
import java.util.function.Consumer;
import java.util.function.Supplier;

import io.qt.QNoDefaultValueException;
import io.qt.QtUninvokable;
import io.qt.core.QMetaObject.AbstractSignal6Default3;
import io.qt.core.QMetaObject.AbstractSignal6Default4;
import io.qt.core.QMetaObject.AbstractSignal6Default5;
import io.qt.core.QMetaObject.AbstractSignal7Default1;
import io.qt.core.QMetaObject.AbstractSignal7Default2;
import io.qt.core.QMetaObject.AbstractSignal7Default3;
import io.qt.core.QMetaObject.AbstractSignal7Default4;
import io.qt.core.QMetaObject.AbstractSignal7Default5;
import io.qt.core.QMetaObject.AbstractSignal7Default6;
import io.qt.core.QMetaObject.AbstractSignal8Default1;
import io.qt.core.QMetaObject.AbstractSignal8Default2;
import io.qt.core.QMetaObject.AbstractSignal8Default3;
import io.qt.core.QMetaObject.AbstractSignal8Default4;
import io.qt.core.QMetaObject.AbstractSignal8Default5;
import io.qt.core.QMetaObject.AbstractSignal8Default6;
import io.qt.core.QMetaObject.AbstractSignal8Default7;
import io.qt.core.QMetaObject.AbstractSignal9Default1;
import io.qt.core.QMetaObject.AbstractSignal9Default2;
import io.qt.core.QMetaObject.AbstractSignal9Default3;
import io.qt.core.QMetaObject.AbstractSignal9Default4;
import io.qt.core.QMetaObject.AbstractSignal9Default5;
import io.qt.core.QMetaObject.AbstractSignal9Default6;
import io.qt.core.QMetaObject.AbstractSignal9Default7;
import io.qt.core.QMetaObject.AbstractSignal9Default8;
import io.qt.core.QMetaObject.GenericSlot;

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
    	public Signal1(Class<A> typeA) {
			super(typeA);
		}
    	
    	public Signal1(String signalName, Class<A> typeA) {
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
    	public Signal2(Class<A> typeA, Class<B> typeB) {
			super(typeA, typeB);
		}
    	
    	public Signal2(String signalName, Class<A> typeA, Class<B> typeB) {
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
    	public Signal3(Class<A> typeA, Class<B> typeB, Class<C> typeC) {
			super(typeA, typeB, typeC);
		}
    	
    	public Signal3(String signalName, Class<A> typeA, Class<B> typeB, Class<C> typeC) {
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
    	public Signal4(Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD) {
			super(typeA, typeB, typeC, typeD);
		}
    	
    	public Signal4(String signalName, Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD) {
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
    	public Signal5(Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE) {
			super(typeA, typeB, typeC, typeD, typeE);
		}
    	
    	public Signal5(String signalName, Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE) {
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
    	public Signal6(Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Class<F> typeF) {
			super(typeA, typeB, typeC, typeD, typeE, typeF);
		}
    	
    	public Signal6(String signalName, Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Class<F> typeF) {
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
    	public Signal7(Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Class<F> typeF, Class<G> typeG) {
			super(typeA, typeB, typeC, typeD, typeE, typeF, typeG);
		}
    	
    	public Signal7(String signalName, Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Class<F> typeF, Class<G> typeG) {
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
    	public Signal8(Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Class<F> typeF, Class<G> typeG, Class<H> typeH) {
			super(typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH);
		}
    	
    	public Signal8(String signalName, Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Class<F> typeF, Class<G> typeG, Class<H> typeH) {
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
    	public Signal9(Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Class<F> typeF, Class<G> typeG, Class<H> typeH, Class<I> typeI) {
			super(typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH, typeI);
		}
    	
    	public Signal9(String signalName, Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Class<F> typeF, Class<G> typeG, Class<H> typeH, Class<I> typeI) {
			super(signalName, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH, typeI);
		}
    }

    public static final class Signal1Default1<A> extends QMetaObject.AbstractPublicSignal1<A> {
    	public Signal1Default1(Class<A> typeA, Supplier<A> arg1Default){
    		super(typeA);
    		if(arg1Default!=null){
    			this.arg1Default = arg1Default;
    		}else{
        		throw new QNoDefaultValueException(1);
        	}
    	}
    	
    	public Signal1Default1(String signalName, Class<A> typeA, Supplier<A> arg1Default){
    		super(signalName, typeA);
    		if(arg1Default!=null){
    			this.arg1Default = arg1Default;
    		}else{
        		throw new QNoDefaultValueException(1);
        	}
    	}
    	
    	private final Supplier<A> arg1Default;
        
        public void emit() {
       		emit(arg1Default.get());
        }
    }
    
    public static final class Signal2Default1<A, B> extends QMetaObject.AbstractSignal2Default1<A, B>{
		public Signal2Default1(Class<A> typeA, Class<B> typeB, Supplier<B> arg2Default) {
			super(arg2Default, typeA, typeB);
		}
		
		public Signal2Default1(String signalName, Class<A> typeA, Class<B> typeB, Supplier<B> arg2Default) {
			super(signalName, arg2Default, typeA, typeB);
		}
	}
    
    public static final class Signal2Default2<A, B> extends QMetaObject.AbstractSignal2Default1<A, B> {
    	
    	public Signal2Default2(Class<A> typeA, Class<B> typeB, Supplier<A> arg1Default, Supplier<B> arg2Default){
    		super(arg2Default, typeA, typeB);
    		if(arg1Default!=null){
    			this.arg1Default = arg1Default;
    		}else{
        		throw new QNoDefaultValueException(1);
        	}
    	}
    	
    	public Signal2Default2(String signalName, Class<A> typeA, Class<B> typeB, Supplier<A> arg1Default, Supplier<B> arg2Default){
    		super(signalName, arg2Default, typeA, typeB);
    		if(arg1Default!=null){
    			this.arg1Default = arg1Default;
    		}else{
        		throw new QNoDefaultValueException(1);
        	}
    	}
    	
    	private final Supplier<A> arg1Default;
        
        public void emit() {
       		emit(arg1Default.get());
        }
    }
    
    public static final class Signal3Default1<A, B, C> extends QMetaObject.AbstractSignal3Default1<A, B, C> {
		public Signal3Default1(String signalName, Class<A> typeA, Class<B> typeB, Class<C> typeC, Supplier<C> arg3Default) {
			super(signalName, arg3Default, typeA, typeB, typeC);
		}
		
		public Signal3Default1(Class<A> typeA, Class<B> typeB, Class<C> typeC, Supplier<C> arg3Default) {
			super(arg3Default, typeA, typeB, typeC);
		}
    }
    
    public static final class Signal3Default2<A, B, C> extends QMetaObject.AbstractSignal3Default2<A, B, C> {
    	public Signal3Default2(String signalName, Class<A> typeA, Class<B> typeB, Class<C> typeC, Supplier<B> arg2Default, Supplier<C> arg3Default){
    		super(signalName, arg2Default, arg3Default, typeA, typeB, typeC);
    	}
    	
    	public Signal3Default2(Class<A> typeA, Class<B> typeB, Class<C> typeC, Supplier<B> arg2Default, Supplier<C> arg3Default){
    		super(arg2Default, arg3Default, typeA, typeB, typeC);
    	}
    }
    
    public static final class Signal3Default3<A, B, C> extends QMetaObject.AbstractSignal3Default2<A, B, C> {
    	
    	public Signal3Default3(String signalName, Class<A> typeA, Class<B> typeB, Class<C> typeC, Supplier<A> arg1Default, Supplier<B> arg2Default, Supplier<C> arg3Default){
    		super(signalName, arg2Default, arg3Default, typeA, typeB, typeC);
    		if(arg2Default!=null){
    			this.arg1Default = arg1Default;
    		}else{
        		throw new QNoDefaultValueException(1);
        	}
    	}
    	
    	public Signal3Default3(Class<A> typeA, Class<B> typeB, Class<C> typeC, Supplier<A> arg1Default, Supplier<B> arg2Default, Supplier<C> arg3Default){
    		super(arg2Default, arg3Default, typeA, typeB, typeC);
    		if(arg2Default!=null){
    			this.arg1Default = arg1Default;
    		}else{
        		throw new QNoDefaultValueException(1);
        	}
    	}
    	
    	private final Supplier<A> arg1Default;
        
        public void emit() {
       		emit(arg1Default.get());
        }
    }
    
    public static final class Signal4Default1<A, B, C, D> extends QMetaObject.AbstractSignal4Default1<A, B, C, D>{
    	public Signal4Default1(String signalName, Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Supplier<D> arg4Default){
    		super(signalName, arg4Default, typeA, typeB, typeC, typeD);
    	}
    	
    	public Signal4Default1(Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Supplier<D> arg4Default){
    		super(arg4Default, typeA, typeB, typeC, typeD);
    	}
    }
    
    public static final class Signal4Default2<A, B, C, D> extends QMetaObject.AbstractSignal4Default2<A, B, C, D>{
    	public Signal4Default2(String signalName, Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Supplier<C> arg3Default, Supplier<D> arg4Default){
    		super(signalName, arg3Default, arg4Default, typeA, typeB, typeC, typeD);
    	}
    	
    	public Signal4Default2(Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Supplier<C> arg3Default, Supplier<D> arg4Default){
    		super(arg3Default, arg4Default, typeA, typeB, typeC, typeD);
    	}
    }
    
    public static final class Signal4Default3<A, B, C, D> extends QMetaObject.AbstractSignal4Default3<A, B, C, D>{
    	public Signal4Default3(String signalName, Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Supplier<B> arg2Default, Supplier<C> arg3Default, Supplier<D> arg4Default){
    		super(signalName, arg2Default, arg3Default, arg4Default, typeA, typeB, typeC, typeD);
    	}
    	
    	public Signal4Default3(Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Supplier<B> arg2Default, Supplier<C> arg3Default, Supplier<D> arg4Default){
    		super(arg2Default, arg3Default, arg4Default, typeA, typeB, typeC, typeD);
    	}
    }
    
    public static final class Signal4Default4<A, B, C, D> extends QMetaObject.AbstractSignal4Default3<A, B, C, D> {
    	
    	public Signal4Default4(String signalName, Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, 
    			Supplier<A> arg1Default, Supplier<B> arg2Default, 
    			Supplier<C> arg3Default, Supplier<D> arg4Default){
    		super(signalName, arg2Default, arg3Default, arg4Default, typeA, typeB, typeC, typeD);
    		if(arg1Default!=null){
    			this.arg1Default = arg1Default;
    		}else{
        		throw new QNoDefaultValueException(1);
        	}
    	}
    	
    	public Signal4Default4(Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, 
    			Supplier<A> arg1Default, Supplier<B> arg2Default, 
    			Supplier<C> arg3Default, Supplier<D> arg4Default){
    		super(arg2Default, arg3Default, arg4Default, typeA, typeB, typeC, typeD);
    		if(arg1Default!=null){
    			this.arg1Default = arg1Default;
    		}else{
        		throw new QNoDefaultValueException(1);
        	}
    	}
    	
    	private final Supplier<A> arg1Default;
        
        public void emit() {
       		emit(arg1Default.get());
        }
    }
    
    public static final class Signal5Default1<A, B, C, D, E> extends QMetaObject.AbstractSignal5Default1<A, B, C, D, E>{
    	public Signal5Default1(Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Supplier<E> arg5Default){
    		super(arg5Default, typeA, typeB, typeC, typeD, typeE);
    	}
    	public Signal5Default1(String signalName, Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Supplier<E> arg5Default){
    		super(signalName, arg5Default, typeA, typeB, typeC, typeD, typeE);
    	}
    }
    
    public static final class Signal5Default2<A, B, C, D, E> extends QMetaObject.AbstractSignal5Default2<A, B, C, D, E>{
    	public Signal5Default2(Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Supplier<D> arg4Default, Supplier<E> arg5Default){
    		super(arg4Default, arg5Default, typeA, typeB, typeC, typeD, typeE);
    	}
    	public Signal5Default2(String signalName, Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Supplier<D> arg4Default, Supplier<E> arg5Default){
    		super(signalName, arg4Default, arg5Default, typeA, typeB, typeC, typeD, typeE);
    	}
    }
    
    public static final class Signal5Default3<A, B, C, D, E> extends QMetaObject.AbstractSignal5Default3<A, B, C, D, E>{
    	public Signal5Default3(Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default){
    		super(arg3Default, arg4Default, arg5Default, typeA, typeB, typeC, typeD, typeE);
    	}
    	public Signal5Default3(String signalName, Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default){
    		super(signalName, arg3Default, arg4Default, arg5Default, typeA, typeB, typeC, typeD, typeE);
    	}
    }
    
    public static final class Signal5Default4<A, B, C, D, E> extends QMetaObject.AbstractSignal5Default4<A, B, C, D, E>{
    	public Signal5Default4(Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Supplier<B> arg2Default, Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default){
    		super(arg2Default, arg3Default, arg4Default, arg5Default, typeA, typeB, typeC, typeD, typeE);
    	}
    	public Signal5Default4(String signalName, Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Supplier<B> arg2Default, Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default){
    		super(signalName, arg2Default, arg3Default, arg4Default, arg5Default, typeA, typeB, typeC, typeD, typeE);
    	}
    }
    
    public static final class Signal5Default5<A, B, C, D, E> extends QMetaObject.AbstractSignal5Default4<A, B, C, D, E>{
    	
    	public Signal5Default5(Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Supplier<A> arg1Default, Supplier<B> arg2Default, Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default){
    		super(arg2Default, arg3Default, arg4Default, arg5Default, typeA, typeB, typeC, typeD, typeE);
    		if(arg1Default!=null){
    			this.arg1Default = arg1Default;
    		}else{
        		throw new QNoDefaultValueException(1);
        	}
    	}
    	public Signal5Default5(String signalName, Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Supplier<A> arg1Default, Supplier<B> arg2Default, Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default){
    		super(arg2Default, arg3Default, arg4Default, arg5Default, typeA, typeB, typeC, typeD, typeE);
    		if(arg1Default!=null){
    			this.arg1Default = arg1Default;
    		}else{
        		throw new QNoDefaultValueException(1);
        	}
    	}
    	
    	private final Supplier<A> arg1Default;
        
        public void emit() {
       		emit(arg1Default.get());
        }
    }
    
    public static final class Signal6Default1<A, B, C, D, E, F> extends QMetaObject.AbstractSignal6Default1<A, B, C, D, E, F>{
    	public Signal6Default1(Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Class<F> typeF, Supplier<F> arg6Default){
    		super(arg6Default, typeA, typeB, typeC, typeD, typeE, typeF);
    	}
    	public Signal6Default1(String signalName, Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Class<F> typeF, Supplier<F> arg6Default){
    		super(signalName, arg6Default, typeA, typeB, typeC, typeD, typeE, typeF);
    	}
    }
    
    public static final class Signal6Default2<A, B, C, D, E, F> extends QMetaObject.AbstractSignal6Default2<A, B, C, D, E, F>{
    	public Signal6Default2(Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Class<F> typeF, Supplier<E> arg5Default, Supplier<F> arg6Default){
    		super(arg5Default, arg6Default, typeA, typeB, typeC, typeD, typeE, typeF);
    	}
    	public Signal6Default2(String signalName, Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Class<F> typeF, Supplier<E> arg5Default, Supplier<F> arg6Default){
    		super(signalName, arg5Default, arg6Default, typeA, typeB, typeC, typeD, typeE, typeF);
    	}
    }
    
    public static final class Signal6Default3<A, B, C, D, E, F> extends AbstractSignal6Default3<A, B, C, D, E, F>{
    	public Signal6Default3(Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Class<F> typeF, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default){
    		super(arg4Default, arg5Default, arg6Default, typeA, typeB, typeC, typeD, typeE, typeF);
    	}
    	public Signal6Default3(String signalName, Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Class<F> typeF, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default){
    		super(signalName, arg4Default, arg5Default, arg6Default, typeA, typeB, typeC, typeD, typeE, typeF);
    	}
    }
    
    public static final class Signal6Default4<A, B, C, D, E, F> extends AbstractSignal6Default4<A, B, C, D, E, F>{
    	public Signal6Default4(Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Class<F> typeF, Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default){
    		super(arg3Default, arg4Default, arg5Default, arg6Default, typeA, typeB, typeC, typeD, typeE, typeF);
    	}
    	public Signal6Default4(String signalName, Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Class<F> typeF, Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default){
    		super(signalName, arg3Default, arg4Default, arg5Default, arg6Default, typeA, typeB, typeC, typeD, typeE, typeF);
    	}
    }
    
    public static final class Signal6Default5<A, B, C, D, E, F> extends AbstractSignal6Default5<A, B, C, D, E, F>{
    	public Signal6Default5(Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Class<F> typeF, Supplier<B> arg2Default, Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default){
    		super(arg2Default, arg3Default, arg4Default, arg5Default, arg6Default, typeA, typeB, typeC, typeD, typeE, typeF);
    	}
    	public Signal6Default5(String signalName, Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Class<F> typeF, Supplier<B> arg2Default, Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default){
    		super(signalName, arg2Default, arg3Default, arg4Default, arg5Default, arg6Default, typeA, typeB, typeC, typeD, typeE, typeF);
    	}
    }
    
    public static final class Signal6Default6<A, B, C, D, E, F> extends AbstractSignal6Default5<A, B, C, D, E, F>{
    	public Signal6Default6(Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Class<F> typeF, Supplier<A> arg1Default, Supplier<B> arg2Default, Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default){
    		super(arg2Default, arg3Default, arg4Default, arg5Default, arg6Default, typeA, typeB, typeC, typeD, typeE, typeF);
			this.arg1Default = arg1Default;
    		if(this.arg1Default==null){
        		throw new QNoDefaultValueException(1);
        	}
    	}
    	public Signal6Default6(String signalName, Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Class<F> typeF, Supplier<A> arg1Default, Supplier<B> arg2Default, Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default){
    		super(signalName, arg2Default, arg3Default, arg4Default, arg5Default, arg6Default, typeA, typeB, typeC, typeD, typeE, typeF);
			this.arg1Default = arg1Default;
    		if(this.arg1Default==null){
        		throw new QNoDefaultValueException(1);
        	}
    	}
    	
    	private final Supplier<A> arg1Default;
        
    	public void emit() {
       		emit(arg1Default.get());
        }
    }
    
    public static final class Signal7Default1<A, B, C, D, E, F, G> extends AbstractSignal7Default1<A, B, C, D, E, F, G>{
    	public Signal7Default1(Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Class<F> typeF, Class<G> typeG, Supplier<G> arg7Default){
    		super(arg7Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG);
    	}
    	public Signal7Default1(String signalName, Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Class<F> typeF, Class<G> typeG, Supplier<G> arg7Default){
    		super(signalName, arg7Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG);
    	}
    }
    
    public static final class Signal7Default2<A, B, C, D, E, F, G> extends AbstractSignal7Default2<A, B, C, D, E, F, G> {
    	public Signal7Default2(Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Class<F> typeF, Class<G> typeG, Supplier<F> arg6Default, Supplier<G> arg7Default){
    		super(arg6Default, arg7Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG);
    	}
    	public Signal7Default2(String signalName, Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Class<F> typeF, Class<G> typeG, Supplier<F> arg6Default, Supplier<G> arg7Default){
    		super(signalName, arg6Default, arg7Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG);
    	}
    }
    
    public static final class Signal7Default3<A, B, C, D, E, F, G> extends AbstractSignal7Default3<A, B, C, D, E, F, G> {
    	public Signal7Default3(Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Class<F> typeF, Class<G> typeG, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default){
    		super(arg5Default, arg6Default, arg7Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG);
    	}
    	public Signal7Default3(String signalName, Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Class<F> typeF, Class<G> typeG, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default){
    		super(signalName, arg5Default, arg6Default, arg7Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG);
    	}
    }
    
    public static final class Signal7Default4<A, B, C, D, E, F, G> extends AbstractSignal7Default4<A, B, C, D, E, F, G> {
    	public Signal7Default4(Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Class<F> typeF, Class<G> typeG, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default){
    		super(arg4Default, arg5Default, arg6Default, arg7Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG);
    	}
    	public Signal7Default4(String signalName, Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Class<F> typeF, Class<G> typeG, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default){
    		super(signalName, arg4Default, arg5Default, arg6Default, arg7Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG);
    	}
    }
    
    public static final class Signal7Default5<A, B, C, D, E, F, G> extends AbstractSignal7Default5<A, B, C, D, E, F, G> {
    	public Signal7Default5(Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Class<F> typeF, Class<G> typeG, Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default){
    		super(arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG);
    	}
    	public Signal7Default5(String signalName, Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Class<F> typeF, Class<G> typeG, Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default){
    		super(signalName, arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG);
    	}
    }
    
    public static final class Signal7Default6<A, B, C, D, E, F, G> extends AbstractSignal7Default6<A, B, C, D, E, F, G> {
    	public Signal7Default6(Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Class<F> typeF, Class<G> typeG, Supplier<B> arg2Default, Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default){
    		super(arg2Default, arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG);
    	}
    	public Signal7Default6(String signalName, Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Class<F> typeF, Class<G> typeG, Supplier<B> arg2Default, Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default){
    		super(signalName, arg2Default, arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG);
    	}
    }
    
    public static final class Signal7Default7<A, B, C, D, E, F, G> extends AbstractSignal7Default6<A, B, C, D, E, F, G> {
    	public Signal7Default7(Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Class<F> typeF, Class<G> typeG, Supplier<A> arg1Default, Supplier<B> arg2Default, Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default){
    		super(arg2Default, arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG);
			this.arg1Default = arg1Default;
    		if(this.arg1Default==null){
        		throw new QNoDefaultValueException(2);
        	}
    	}
    	public Signal7Default7(String signalName, Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Class<F> typeF, Class<G> typeG, Supplier<A> arg1Default, Supplier<B> arg2Default, Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default){
    		super(signalName, arg2Default, arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG);
			this.arg1Default = arg1Default;
    		if(this.arg1Default==null){
        		throw new QNoDefaultValueException(2);
        	}
    	}
    	
    	private final Supplier<A> arg1Default;
        
        public void emit() {
       		emit(arg1Default.get());
        }
    }
    
    public static final class Signal8Default1<A, B, C, D, E, F, G, H> extends AbstractSignal8Default1<A, B, C, D, E, F, G, H>{
    	public Signal8Default1(Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Class<F> typeF, Class<G> typeG, Class<H> typeH, Supplier<H> arg8Default){
    		super(arg8Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH);
    	}
    	public Signal8Default1(String signalName, Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Class<F> typeF, Class<G> typeG, Class<H> typeH, Supplier<H> arg8Default){
    		super(signalName, arg8Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH);
    	}
    }
    
    public static final class Signal8Default2<A, B, C, D, E, F, G, H> extends AbstractSignal8Default2<A, B, C, D, E, F, G, H> {
    	public Signal8Default2(Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Class<F> typeF, Class<G> typeG, Class<H> typeH, Supplier<G> arg7Default, Supplier<H> arg8Default){
    		super(arg7Default, arg8Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH);
    	}
    	public Signal8Default2(String signalName, Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Class<F> typeF, Class<G> typeG, Class<H> typeH, Supplier<G> arg7Default, Supplier<H> arg8Default){
    		super(signalName, arg7Default, arg8Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH);
    	}
    }
    
    public static final class Signal8Default3<A, B, C, D, E, F, G, H> extends AbstractSignal8Default3<A, B, C, D, E, F, G, H> {
    	public Signal8Default3(Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Class<F> typeF, Class<G> typeG, Class<H> typeH, Supplier<F> arg6Default, Supplier<G> arg7Default, Supplier<H> arg8Default){
    		super(arg6Default, arg7Default, arg8Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH);
    	}
    	public Signal8Default3(String signalName, Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Class<F> typeF, Class<G> typeG, Class<H> typeH, Supplier<F> arg6Default, Supplier<G> arg7Default, Supplier<H> arg8Default){
    		super(signalName, arg6Default, arg7Default, arg8Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH);
    	}
    }
    
    public static final class Signal8Default4<A, B, C, D, E, F, G, H> extends AbstractSignal8Default4<A, B, C, D, E, F, G, H> {
    	public Signal8Default4(Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Class<F> typeF, Class<G> typeG, Class<H> typeH, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default, Supplier<H> arg8Default){
    		super(arg5Default, arg6Default, arg7Default, arg8Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH);
    	}
    	public Signal8Default4(String signalName, Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Class<F> typeF, Class<G> typeG, Class<H> typeH, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default, Supplier<H> arg8Default){
    		super(signalName, arg5Default, arg6Default, arg7Default, arg8Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH);
    	}
    }
    
    public static final class Signal8Default5<A, B, C, D, E, F, G, H> extends AbstractSignal8Default5<A, B, C, D, E, F, G, H> {
    	public Signal8Default5(Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Class<F> typeF, Class<G> typeG, Class<H> typeH, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default, Supplier<H> arg8Default){
    		super(arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH);
    	}
    	public Signal8Default5(String signalName, Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Class<F> typeF, Class<G> typeG, Class<H> typeH, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default, Supplier<H> arg8Default){
    		super(signalName, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH);
    	}
    }
    
    public static final class Signal8Default6<A, B, C, D, E, F, G, H> extends AbstractSignal8Default6<A, B, C, D, E, F, G, H> {
    	public Signal8Default6(Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Class<F> typeF, Class<G> typeG, Class<H> typeH, Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default, Supplier<H> arg8Default){
    		super(arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH);
    	}
    	public Signal8Default6(String signalName, Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Class<F> typeF, Class<G> typeG, Class<H> typeH, Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default, Supplier<H> arg8Default){
    		super(signalName, arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH);
    	}
    }
    
    public static final class Signal8Default7<A, B, C, D, E, F, G, H> extends AbstractSignal8Default7<A, B, C, D, E, F, G, H> {
    	public Signal8Default7(Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Class<F> typeF, Class<G> typeG, Class<H> typeH, Supplier<B> arg2Default, Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default, Supplier<H> arg8Default){
    		super(arg2Default, arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH);
    	}
    	public Signal8Default7(String signalName, Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Class<F> typeF, Class<G> typeG, Class<H> typeH, Supplier<B> arg2Default, Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default, Supplier<H> arg8Default){
    		super(signalName, arg2Default, arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH);
    	}
    }
    
    public static final class Signal8Default8<A, B, C, D, E, F, G, H> extends AbstractSignal8Default7<A, B, C, D, E, F, G, H> {
    	public Signal8Default8(Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Class<F> typeF, Class<G> typeG, Class<H> typeH, Supplier<A> arg1Default, Supplier<B> arg2Default, Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default, Supplier<H> arg8Default){
    		super(arg2Default, arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH);
			this.arg1Default = arg1Default;
			if(this.arg1Default==null){
        		throw new QNoDefaultValueException(1);
        	}
    	}
    	public Signal8Default8(String signalName, Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Class<F> typeF, Class<G> typeG, Class<H> typeH, Supplier<A> arg1Default, Supplier<B> arg2Default, Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default, Supplier<H> arg8Default){
    		super(signalName, arg2Default, arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH);
			this.arg1Default = arg1Default;
			if(this.arg1Default==null){
        		throw new QNoDefaultValueException(1);
        	}
    	}
    	
    	private final Supplier<A> arg1Default;
        
        public void emit() {
       		emit(arg1Default.get());
        }
    }
    
    public static final class Signal9Default1<A, B, C, D, E, F, G, H, I> extends AbstractSignal9Default1<A, B, C, D, E, F, G, H, I>{
    	public Signal9Default1(Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Class<F> typeF, Class<G> typeG, Class<H> typeH, Class<I> typeI, Supplier<I> arg9Default){
    		super(arg9Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH, typeI);
    	}
    	public Signal9Default1(String signalName, Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Class<F> typeF, Class<G> typeG, Class<H> typeH, Class<I> typeI, Supplier<I> arg9Default){
    		super(signalName, arg9Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH, typeI);
    	}
    }
    
    public static final class Signal9Default2<A, B, C, D, E, F, G, H, I> extends AbstractSignal9Default2<A, B, C, D, E, F, G, H, I> {
    	public Signal9Default2(Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Class<F> typeF, Class<G> typeG, Class<H> typeH, Class<I> typeI, Supplier<H> arg8Default, Supplier<I> arg9Default){
    		super(arg8Default, arg9Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH, typeI);
    	}
    	public Signal9Default2(String signalName, Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Class<F> typeF, Class<G> typeG, Class<H> typeH, Class<I> typeI, Supplier<H> arg8Default, Supplier<I> arg9Default){
    		super(signalName, arg8Default, arg9Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH, typeI);
    	}
    }
    
    public static final class Signal9Default3<A, B, C, D, E, F, G, H, I> extends AbstractSignal9Default3<A, B, C, D, E, F, G, H, I> {
    	public Signal9Default3(Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Class<F> typeF, Class<G> typeG, Class<H> typeH, Class<I> typeI, Supplier<G> arg7Default, Supplier<H> arg8Default, Supplier<I> arg9Default){
    		super(arg7Default, arg8Default, arg9Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH, typeI);
    	}
    	public Signal9Default3(String signalName, Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Class<F> typeF, Class<G> typeG, Class<H> typeH, Class<I> typeI, Supplier<G> arg7Default, Supplier<H> arg8Default, Supplier<I> arg9Default){
    		super(signalName, arg7Default, arg8Default, arg9Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH, typeI);
    	}
    }
    
    public static final class Signal9Default4<A, B, C, D, E, F, G, H, I> extends AbstractSignal9Default4<A, B, C, D, E, F, G, H, I> {
    	public Signal9Default4(Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Class<F> typeF, Class<G> typeG, Class<H> typeH, Class<I> typeI, Supplier<F> arg6Default, Supplier<G> arg7Default, Supplier<H> arg8Default, Supplier<I> arg9Default){
    		super(arg6Default, arg7Default, arg8Default, arg9Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH, typeI);
    	}
    	public Signal9Default4(String signalName, Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Class<F> typeF, Class<G> typeG, Class<H> typeH, Class<I> typeI, Supplier<F> arg6Default, Supplier<G> arg7Default, Supplier<H> arg8Default, Supplier<I> arg9Default){
    		super(signalName, arg6Default, arg7Default, arg8Default, arg9Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH, typeI);
    	}
    }
    
    public static final class Signal9Default5<A, B, C, D, E, F, G, H, I> extends AbstractSignal9Default5<A, B, C, D, E, F, G, H, I> {
    	public Signal9Default5(Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Class<F> typeF, Class<G> typeG, Class<H> typeH, Class<I> typeI, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default, Supplier<H> arg8Default, Supplier<I> arg9Default){
    		super(arg5Default, arg6Default, arg7Default, arg8Default, arg9Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH, typeI);
    	}
    	public Signal9Default5(String signalName, Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Class<F> typeF, Class<G> typeG, Class<H> typeH, Class<I> typeI, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default, Supplier<H> arg8Default, Supplier<I> arg9Default){
    		super(signalName, arg5Default, arg6Default, arg7Default, arg8Default, arg9Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH, typeI);
    	}
    }
    
    public static final class Signal9Default6<A, B, C, D, E, F, G, H, I> extends AbstractSignal9Default6<A, B, C, D, E, F, G, H, I> {
    	public Signal9Default6(Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Class<F> typeF, Class<G> typeG, Class<H> typeH, Class<I> typeI, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default, Supplier<H> arg8Default, Supplier<I> arg9Default){
    		super(arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, arg9Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH, typeI);
		}
    	public Signal9Default6(String signalName, Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Class<F> typeF, Class<G> typeG, Class<H> typeH, Class<I> typeI, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default, Supplier<H> arg8Default, Supplier<I> arg9Default){
    		super(signalName, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, arg9Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH, typeI);
		}
    }
    
    public static final class Signal9Default7<A, B, C, D, E, F, G, H, I> extends AbstractSignal9Default7<A, B, C, D, E, F, G, H, I> {
    	public Signal9Default7(Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Class<F> typeF, Class<G> typeG, Class<H> typeH, Class<I> typeI, Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default, Supplier<H> arg8Default, Supplier<I> arg9Default){
    		super(arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, arg9Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH, typeI);
    	}
    	public Signal9Default7(String signalName, Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Class<F> typeF, Class<G> typeG, Class<H> typeH, Class<I> typeI, Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default, Supplier<H> arg8Default, Supplier<I> arg9Default){
    		super(signalName, arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, arg9Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH, typeI);
    	}
    }
    
    public static final class Signal9Default8<A, B, C, D, E, F, G, H, I> extends AbstractSignal9Default8<A, B, C, D, E, F, G, H, I> {
    	public Signal9Default8(Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Class<F> typeF, Class<G> typeG, Class<H> typeH, Class<I> typeI, Supplier<B> arg2Default, Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default, Supplier<H> arg8Default, Supplier<I> arg9Default){
    		super(arg2Default, arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, arg9Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH, typeI);
    	}
    	public Signal9Default8(String signalName, Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Class<F> typeF, Class<G> typeG, Class<H> typeH, Class<I> typeI, Supplier<B> arg2Default, Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default, Supplier<H> arg8Default, Supplier<I> arg9Default){
    		super(signalName, arg2Default, arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, arg9Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH, typeI);
    	}
    }
    
    public static final class Signal9Default9<A, B, C, D, E, F, G, H, I> extends AbstractSignal9Default8<A, B, C, D, E, F, G, H, I> {
    	
    	public Signal9Default9(Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Class<F> typeF, Class<G> typeG, Class<H> typeH, Class<I> typeI, Supplier<A> arg1Default, Supplier<B> arg2Default, Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default, Supplier<H> arg8Default, Supplier<I> arg9Default){
    		super(arg2Default, arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, arg9Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH, typeI);
			this.arg1Default = arg1Default;
    		if(this.arg1Default==null){
        		throw new QNoDefaultValueException(1);
        	}
    	}
    	public Signal9Default9(String signalName, Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Class<F> typeF, Class<G> typeG, Class<H> typeH, Class<I> typeI, Supplier<A> arg1Default, Supplier<B> arg2Default, Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default, Supplier<H> arg8Default, Supplier<I> arg9Default){
    		super(signalName, arg2Default, arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, arg9Default, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH, typeI);
			this.arg1Default = arg1Default;
    		if(this.arg1Default==null){
        		throw new QNoDefaultValueException(1);
        	}
    	}
    	
    	private final Supplier<A> arg1Default;
        
        public void emit() {
       		emit(arg1Default.get());
        }
    }

    /**
     * Use this signal class to declare a local signal.
     */
    public static final class GenericSignal extends AbstractPrivateGenericSignal {
    	public GenericSignal(Class<?>...types) {
			super(types);
		}
    	
    	public void emit(Object...args) {
    		super.emit(args);
    	}
    }

    /**
     * Use this signal class to declare a local signal.
     */
    public static final class PrivateGenericSignal extends AbstractPrivateGenericSignal {
    	public PrivateGenericSignal(Class<?>...types) {
			super(types);
		}
    	
    	public PrivateGenericSignal(Consumer<GenericSlot> emitConsumer, Class<?>...types) {
			super(emitConsumer, types);
		}
    	
    	public PrivateGenericSignal(String signalName, Class<?>...types) {
			super(signalName, types);
		}
    	
    	public PrivateGenericSignal(Consumer<GenericSlot> emitConsumer, String signalName, Class<?>...types) {
			super(emitConsumer, signalName, types);
		}
    }

    /**
     * Use this signal class to declare a local signal.
     */
    public static abstract class AbstractPrivateGenericSignal extends QMetaObject.AbstractSignal {
    	
    	private final Class<?>[] types;
    	
    	AbstractPrivateGenericSignal(Class<?>...types) {
    		this((Consumer<GenericSlot>)null, types);
    	}
    	
    	AbstractPrivateGenericSignal(String signalName, Class<?>...types) {
    		this(null, signalName, types);
    	}
    	
    	AbstractPrivateGenericSignal(Consumer<GenericSlot> emitConsumer, Class<?>...types) {
			super(types);
			for (int i = 0; i < types.length; i++) {
				if(types[i] == void.class) {
					throw new IllegalArgumentException("void is not a valid type for signals");
				}else if(types[i] == null) {
					throw new IllegalArgumentException("null is not a valid type for signals");
				}
			}
			this.types = Arrays.copyOf(types, types.length);
			if(emitConsumer!=null) {
				emitConsumer.accept(this::emit);
			}
		}
    	
    	AbstractPrivateGenericSignal(Consumer<GenericSlot> emitConsumer, String signalName, Class<?>... types) {
			super(signalName, types);
			for (int i = 0; i < types.length; i++) {
				if(types[i] == void.class) {
					throw new IllegalArgumentException("void is not a valid type for signals");
				}else if(types[i] == null) {
					throw new IllegalArgumentException("null is not a valid type for signals");
				}
			}
			this.types = Arrays.copyOf(types, types.length);
			if(emitConsumer!=null) {
				emitConsumer.accept(this::emit);
			}
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
    }
}
