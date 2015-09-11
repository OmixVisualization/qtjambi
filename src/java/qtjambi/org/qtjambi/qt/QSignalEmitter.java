/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
** 
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
** 
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
** 
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
** 
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
** $END_LICENSE$

**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

package org.qtjambi.qt;

import java.lang.reflect.Method;

import org.qtjambi.qt.core.Qt;
import org.qtjambi.qt.QSignalEmitter;
import org.qtjambi.qt.internal.QSignalEmitterInternal;

/**
 * This class implements the functionality to emit signals. All
 * objects in QtJambi can emit signals, so the class is inherited by
 * QtJambiObject.
 */
public class QSignalEmitter extends QSignalEmitterInternal {

	public static interface Slot0{
		public void invoke();
	}
	
	public static interface Slot1<A>{
		public void invoke(A a);
	}
	
	public static interface Slot2<A,B>{
		public void invoke(A a, B b);
	}
	
	public static interface Slot3<A,B,C>{
		public void invoke(A a, B b, C c);
	}
	
	public static interface Slot4<A,B,C,D>{
		public void invoke(A a, B b, C c, D d);
	}
	
	public static interface Slot5<A,B,C,D,E>{
		public void invoke(A a, B b, C c, D d, E e);
	}
	
	public static interface Slot6<A,B,C,D,E,F>{
		public void invoke(A a, B b, C c, D d, E e, F f);
	}
	
	public static interface Slot7<A,B,C,D,E,F,G>{
		public void invoke(A a, B b, C c, D d, E e, F f, G g);
	}
	
	public static interface Slot8<A,B,C,D,E,F,G,H>{
		public void invoke(A a, B b, C c, D d, E e, F f, G g, H h);
	}
	
	public static interface Slot9<A,B,C,D,E,F,G,H,I>{
		public void invoke(A a, B b, C c, D d, E e, F f, G g, H h, I i);
	}
	
	public static interface DefaultValue<V>{
		public V get();
	}

    public abstract class AbstractSignal extends QSignalEmitter.AbstractSignalInternal {
    	
    	private AbstractSignal(){}

	    /**
	     * Connects the signal to a method in an object. Whenever it is emitted, the method will be invoked
	     * on the given object.
	     *
	     * @param receiver  The object that owns the method
	     * @param method    The signature of the method excluding return type and argument names, such as "setText(String)".
	     * @param type      One of the connection types defined in the Qt interface.
	     * @throws QNoSuchSlotException Raised if the method passed in the slot object was not found
	     * @throws java.lang.RuntimeException Raised if the signal object could not be successfully introspected or if the
	     *                                    signatures of the signal and slot are incompatible.
	     */
	    public final void connect(Object receiver, String method,
	                                 Qt.ConnectionType type) {
	        if (receiver == null)
	            throw new NullPointerException("Receiver must be non-null");
	
	        Method slotMethod = org.qtjambi.qt.internal.QtJambiInternal.lookupSlot(receiver, method);
	        if (slotMethod == null)
	            throw new QNoSuchSlotException(receiver, method);
	
	        connectSignalMethod(slotMethod, receiver, type.value());
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
	    public final boolean disconnect(Object receiver, String method) {
	        if (method != null && receiver == null)
	            throw new IllegalArgumentException("Receiver cannot be null if you specify a method");
	
	        Method slotMethod = null;
	        if (method != null) {
	            slotMethod = org.qtjambi.qt.internal.QtJambiInternal.lookupSlot(receiver, method);
	            if (slotMethod == null)
	                throw new QNoSuchSlotException(receiver, method);
	        }
	
	        return removeConnection(receiver, slotMethod);
	    }
	
	    /**
	     * Removes any connection from this signal to the specified receiving object
	     *
	     *  @param receiver The object to which the signal has connections
	     *  @return true if any connection was successfully removed, otherwise false. The method will return false if no
	     *  connection has previously been establish to the receiver.
	     *
	     *  @see #disconnect(Object, String)
	     **/
	    public final boolean disconnect(Object receiver) {
	        return disconnect(receiver, null);
	    }
	
	    /**
	     * Removes all connections from this signal.
	     *
	     * @return  True if the disconnection was successful.
	     * @see #disconnect(Object, String)
	     */
	    public final boolean disconnect() {
	        return disconnect(null, null);
	    }
	
	    /**
	     * Creates an auto-connection from this signal to the specified object and method.
	     *
	     * @param receiver The object that owns the method
	     * @param method The signature of the method excluding return type and argument names, such as "setText(String)".
	     *
	     * @see #connect(Object, String, org.qtjambi.qt.core.Qt.ConnectionType)
	     **/
	    public final void connect(Object receiver, String method) {
	        connect(receiver, method, Qt.ConnectionType.AutoConnection);
	    }
	
	    /**
	     * Creates an auto connection from this signal to another. Whenever this signal is emitted, it will cause the second
	     * signal to be emitted as well.
	     *
	     * @param signalOut The second signal. This will be emitted whenever this signal is emitted.
	     * @throws RuntimeException Raised if either of the signal objects could not be successfully be introspected or if their
	     *                                    signatures are incompatible.
	     */
	    public final void connect(AbstractSignal signalOut) {
	        connect(signalOut, Qt.ConnectionType.AutoConnection);
	    }
	
	    /**
	     * Creates a connection from this signal to another. Whenever this signal is emitted, it will cause the second
	     * signal to be emitted as well.
	     *
	     * @param signalOut The second signal. This will be emitted whenever this signal is emitted.
	     * @param type      One of the connection types defined in the Qt interface.
	     * @throws RuntimeException Raised if either of the signal objects could not be successfully be introspected or if their
	     *                                    signatures are incompatible.
	     */
	    public final void connect(AbstractSignal signalOut, Qt.ConnectionType type) {
	        connectSignalMethod(org.qtjambi.qt.internal.QtJambiInternal.findEmitMethod(signalOut), signalOut,
	                type.value());
	    }
	
	    /**
	     * Disconnects a signal from another signal if the two were previously connected by a call to connect.
	     * A call to this function will assure that the emission of the first signal will not cause the emission of the second.
	     *
	     * @param signalOut The second signal.
	     * @return true if the two signals were successfully disconnected, or false otherwise.
	     */
	    public final boolean disconnect(AbstractSignal signalOut) {
	        return removeConnection(signalOut, org.qtjambi.qt.internal.QtJambiInternal.findEmitMethod(signalOut));
	    }

    }
    
    protected abstract class AbstractPrivateSignal0 extends AbstractSignal {
        
    	private AbstractPrivateSignal0(){}
    	
    	public final void connect(Slot0 slot) {
        	connect(slot, Qt.ConnectionType.AutoConnection);
        }

        public final void connect(Slot0 slot, Qt.ConnectionType type) {
        	addConnection(slot, type.value());
        }
    }
    
    protected abstract class AbstractPrivateSignal1<A> extends AbstractPrivateSignal0 {
        
    	private AbstractPrivateSignal1(){}
        
        public final void connect(Slot1<A> slot) {
        	connect(slot, Qt.ConnectionType.AutoConnection);
        }

        public final void connect(Slot1<A> slot, Qt.ConnectionType type) {
        	addConnection(slot, type.value());
        }
    }
    
    protected abstract class AbstractPrivateSignal2<A, B> extends AbstractPrivateSignal1<A> {
        private AbstractPrivateSignal2(){}
    	
        public final void connect(Slot2<A,B> slot) {
        	connect(slot, Qt.ConnectionType.AutoConnection);
        }

        public final void connect(Slot2<A,B> slot, Qt.ConnectionType type) {
        	addConnection(slot, type.value());
        }
    }
    
    protected abstract class AbstractPrivateSignal3<A, B, C> extends AbstractPrivateSignal2<A, B> {
        
    	private AbstractPrivateSignal3(){}
        
        public final void connect(Slot3<A,B,C> slot) {
        	connect(slot, Qt.ConnectionType.AutoConnection);
        }

        public final void connect(Slot3<A,B,C> slot, Qt.ConnectionType type) {
        	addConnection(slot, type.value());
        }
    }
    
    protected abstract class AbstractPrivateSignal4<A, B, C, D> extends AbstractPrivateSignal3<A, B, C> {
        
    	private AbstractPrivateSignal4(){}
        
        public final void connect(Slot4<A,B,C,D> slot) {
        	connect(slot, Qt.ConnectionType.AutoConnection);
        }

        public final void connect(Slot4<A,B,C,D> slot, Qt.ConnectionType type) {
        	addConnection(slot, type.value());
        }
    }
    
    protected abstract class AbstractPrivateSignal5<A, B, C, D, E> extends AbstractPrivateSignal4<A, B, C, D> {
        
    	private AbstractPrivateSignal5(){}
        
        public final void connect(Slot5<A,B,C,D,E> slot) {
        	connect(slot, Qt.ConnectionType.AutoConnection);
        }

        public final void connect(Slot5<A,B,C,D,E> slot, Qt.ConnectionType type) {
        	addConnection(slot, type.value());
        }
    }
    
    protected abstract class AbstractPrivateSignal6<A, B, C, D, E, F> extends AbstractPrivateSignal5<A, B, C, D, E> {
        
    	private AbstractPrivateSignal6(){}
        
        public final void connect(Slot6<A,B,C,D,E,F> slot) {
        	connect(slot, Qt.ConnectionType.AutoConnection);
        }

        public final void connect(Slot6<A,B,C,D,E,F> slot, Qt.ConnectionType type) {
        	addConnection(slot, type.value());
        }
    }
    
    protected abstract class AbstractPrivateSignal7<A, B, C, D, E, F, G> extends AbstractPrivateSignal6<A, B, C, D, E, F> {
        
    	private AbstractPrivateSignal7(){}
    	
        public final void connect(Slot7<A,B,C,D,E,F,G> slot) {
        	connect(slot, Qt.ConnectionType.AutoConnection);
        }

        public final void connect(Slot7<A,B,C,D,E,F,G> slot, Qt.ConnectionType type) {
        	addConnection(slot, type.value());
        }
    }
    
    protected abstract class AbstractPrivateSignal8<A, B, C, D, E, F, G, H> extends AbstractPrivateSignal7<A, B, C, D, E, F, G> {
		private AbstractPrivateSignal8(){}
		
        public final void connect(Slot8<A,B,C,D,E,F,G,H> slot) {
        	connect(slot, Qt.ConnectionType.AutoConnection);
        }

        public final void connect(Slot8<A,B,C,D,E,F,G,H> slot, Qt.ConnectionType type) {
        	addConnection(slot, type.value());
        }
	}
    
    protected abstract class AbstractPrivateSignal9<A, B, C, D, E, F, G, H, I> extends AbstractPrivateSignal8<A, B, C, D, E, F, G, H> {
    	private AbstractPrivateSignal9(){}
        
        public final void connect(Slot9<A,B,C,D,E,F,G,H,I> slot) {
        	connect(slot, Qt.ConnectionType.AutoConnection);
        }

        public final void connect(Slot9<A,B,C,D,E,F,G,H,I> slot, Qt.ConnectionType type) {
        	addConnection(slot, type.value());
        }
    }
    
    protected abstract class AbstractPublicSignal0 extends AbstractPrivateSignal0 {
    	private AbstractPublicSignal0(){}
    	
    	/**
         * Emits the signal.
         */
        public final void emit() {
            emit_helper();
        }
    }
    
    protected abstract class AbstractPublicSignal1<A> extends AbstractPrivateSignal1<A> {
    	private AbstractPublicSignal1(){}
    	
    	/**
         * Emits the signal.
         */
        public final void emit(A arg1) {
            emit_helper(arg1);
        }
    }
    
    protected abstract class AbstractPublicSignal2<A,B> extends AbstractPrivateSignal2<A,B> {
    	private AbstractPublicSignal2(){}
    	
    	/**
         * Emits the signal.
         */
        public final void emit(A arg1, B arg2) {
            emit_helper(arg1, arg2);
        }
    }
    
    protected abstract class AbstractPublicSignal3<A,B,C> extends AbstractPrivateSignal3<A,B,C> {
    	private AbstractPublicSignal3(){}
    	
    	/**
         * Emits the signal.
         */
        public final void emit(A arg1, B arg2, C arg3) {
            emit_helper(arg1, arg2, arg3);
        }
    }
    
    protected abstract class AbstractPublicSignal4<A,B,C,D> extends AbstractPrivateSignal4<A,B,C,D> {
    	private AbstractPublicSignal4(){}
    	
    	/**
         * Emits the signal.
         */
        public final void emit(A arg1, B arg2, C arg3, D arg4) {
            emit_helper(arg1, arg2, arg3, arg4);
        }
    }
    
    protected abstract class AbstractPublicSignal5<A,B,C,D,E> extends AbstractPrivateSignal5<A,B,C,D,E> {
    	private AbstractPublicSignal5(){}
    	
    	/**
         * Emits the signal.
         */
        public final void emit(A arg1, B arg2, C arg3, D arg4, E arg5) {
            emit_helper(arg1, arg2, arg3, arg4, arg5);
        }
    }
    
    protected abstract class AbstractPublicSignal6<A,B,C,D,E,F> extends AbstractPrivateSignal6<A,B,C,D,E,F> {
    	private AbstractPublicSignal6(){}
    	
    	/**
         * Emits the signal.
         */
        public final void emit(A arg1, B arg2, C arg3, D arg4, E arg5, F arg6) {
            emit_helper(arg1, arg2, arg3, arg4, arg5, arg6);
        }
    }
    
    protected abstract class AbstractPublicSignal7<A,B,C,D,E,F,G> extends AbstractPrivateSignal7<A,B,C,D,E,F,G> {
    	private AbstractPublicSignal7(){}
    	
    	/**
         * Emits the signal.
         */
        public final void emit(A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7) {
            emit_helper(arg1, arg2, arg3, arg4, arg5, arg6, arg7);
        }
    }
    
    protected abstract class AbstractPublicSignal8<A,B,C,D,E,F,G,H> extends AbstractPrivateSignal8<A,B,C,D,E,F,G,H> {
    	private AbstractPublicSignal8(){}
    	
    	/**
         * Emits the signal.
         */
        public final void emit(A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7, H arg8) {
            emit_helper(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
        }
    }
    
    protected abstract class AbstractPublicSignal9<A,B,C,D,E,F,G,H,I> extends AbstractPrivateSignal9<A,B,C,D,E,F,G,H,I> {
    	private AbstractPublicSignal9(){}
    	
    	/**
         * Emits the signal.
         */
        public final void emit(A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7, H arg8, I arg9) {
            emit_helper(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
        }
    }


    /**
     * Declare and instantiate a field of this class in your
     * QSignalEmitter subclass to declare a signal that takes no
     * parameters.
     */
    public final class Signal0 extends AbstractPublicSignal0 {
    }

    /**
     * Declare and instantiate a field of this class in your
     * QSignalEmitter subclass to declare a signal that takes one
     * parameter.
     *
     * @param <A> The type of the single parameter of the signal.
     */
    public final class Signal1<A> extends AbstractPublicSignal1<A> {
    }
    
    /**
     * Declare and instantiate a field of this class in your
     * QSignalEmitter subclass to declare a signal that takes two
     * parameters.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     */
    public final class Signal2<A, B> extends AbstractPublicSignal2<A, B> {
    }

    /**
     * Declare and instantiate a field of this class in your
     * QSignalEmitter subclass to declare a signal that takes three
     * parameters.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     */
    public final class Signal3<A, B, C> extends AbstractPublicSignal3<A, B, C> {
    }

    /**
     * Declare and instantiate a field of this class in your
     * QSignalEmitter subclass to declare a signal that takes four
     * parameters.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     */

    public final class Signal4<A, B, C, D> extends AbstractPublicSignal4<A, B, C, D> {
    }

    /**
     * Declare and instantiate a field of this class in your
     * QSignalEmitter subclass to declare a signal that takes five
     * parameters.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     */
    public final class Signal5<A, B, C, D, E> extends AbstractPublicSignal5<A, B, C, D, E> {
    }

    /**
     * Declare and instantiate a field of this class in your
     * QSignalEmitter subclass to declare a signal that takes six
     * parameters.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     */
    public final class Signal6<A, B, C, D, E, F> extends AbstractPublicSignal6<A, B, C, D, E, F> {
    }

    /**
     * Declare and instantiate a field of this class in your
     * QSignalEmitter subclass to declare a signal that takes seven
     * parameters.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     */
    public final class Signal7<A, B, C, D, E, F, G> extends AbstractPublicSignal7<A, B, C, D, E, F, G> {
    }

    /**
     * Declare and instantiate a field of this class in your
     * QSignalEmitter subclass to declare a signal that takes eight
     * parameters.
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
    public final class Signal8<A, B, C, D, E, F, G, H> extends AbstractPublicSignal8<A, B, C, D, E, F, G, H> {
    }

    /**
     * Declare and instantiate a field of this class in your
     * QSignalEmitter subclass to declare a signal that takes nine
     * parameters.
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
    public final class Signal9<A, B, C, D, E, F, G, H, I> extends AbstractPublicSignal9<A, B, C, D, E, F, G, H, I>{
    }
    
    public final class Signal1Default1<A> extends AbstractPublicSignal1<A> {
    	
    	public Signal1Default1(DefaultValue<A> arg1Default){
    		super();
    		if(arg1Default!=null){
    			this.arg1Default = arg1Default;
    		}else{
        		throw new QNoDefaultValueException(this.fullName(), 1);
        	}
    	}
    	
    	private final DefaultValue<A> arg1Default;
        
        public void emit() {
       		emit(arg1Default.get());
        }
    }
    
    protected abstract class AbstractSignal2Default1<A, B> extends AbstractPublicSignal2<A, B> {
    	
    	private AbstractSignal2Default1(DefaultValue<B> arg2Default){
    		super();
    		if(arg2Default!=null){
    			this.arg2Default = arg2Default;
    		}else{
        		throw new QNoDefaultValueException(this.fullName(), 2);
        	}
    	}
    	
    	private final DefaultValue<B> arg2Default;
        
        public void emit(A arg1) {
       		emit(arg1, arg2Default.get());
        }
    }
    
    public final class Signal2Default1<A, B> extends AbstractSignal2Default1<A, B>{
		public Signal2Default1(DefaultValue<B> arg2Default) {
			super(arg2Default);
		}}
    
    public final class Signal2Default2<A, B> extends AbstractSignal2Default1<A, B> {
    	
    	public Signal2Default2(DefaultValue<A> arg1Default, DefaultValue<B> arg2Default){
    		super(arg2Default);
    		if(arg1Default!=null){
    			this.arg1Default = arg1Default;
    		}else{
        		throw new QNoDefaultValueException(this.fullName(), 1);
        	}
    	}
    	
    	private final DefaultValue<A> arg1Default;
        
        public void emit() {
       		emit(arg1Default.get());
        }
    }
    
    protected abstract class AbstractSignal3Default1<A, B, C> extends AbstractPublicSignal3<A, B, C> {
    	
    	private AbstractSignal3Default1(DefaultValue<C> arg3Default){
    		super();
    		if(arg3Default!=null){
    			this.arg3Default = arg3Default;
    		}else{
        		throw new QNoDefaultValueException(this.fullName(), 3);
        	}
    	}
    	
    	private final DefaultValue<C> arg3Default;
        
        public void emit(A arg1, B arg2) {
       		emit(arg1, arg2, arg3Default.get());
        }
    }
    
    protected abstract class AbstractSignal3Default2<A, B, C> extends AbstractSignal3Default1<A, B, C> {
    	
    	private AbstractSignal3Default2(DefaultValue<B> arg2Default, DefaultValue<C> arg3Default){
    		super(arg3Default);
    		if(arg2Default!=null){
    			this.arg2Default = arg2Default;
    		}else{
        		throw new QNoDefaultValueException(this.fullName(), 2);
        	}
    	}
    	
    	private final DefaultValue<B> arg2Default;
        
        public void emit(A arg1) {
       		emit(arg1, arg2Default.get());
        }
    }
    
    public final class Signal3Default1<A, B, C> extends AbstractSignal3Default1<A, B, C> {
		public Signal3Default1(DefaultValue<C> arg3Default) {
			super(arg3Default);
		}
    }
    
    public final class Signal3Default2<A, B, C> extends AbstractSignal3Default2<A, B, C> {
    	public Signal3Default2(DefaultValue<B> arg2Default, DefaultValue<C> arg3Default){
    		super(arg2Default, arg3Default);
    	}
    }
    
    public final class Signal3Default3<A, B, C> extends AbstractSignal3Default2<A, B, C> {
    	
    	public Signal3Default3(DefaultValue<A> arg1Default, DefaultValue<B> arg2Default, DefaultValue<C> arg3Default){
    		super(arg2Default, arg3Default);
    		if(arg2Default!=null){
    			this.arg1Default = arg1Default;
    		}else{
        		throw new QNoDefaultValueException(this.fullName(), 1);
        	}
    	}
    	
    	private final DefaultValue<A> arg1Default;
        
        public void emit() {
       		emit(arg1Default.get());
        }
    }
    
    protected abstract class AbstractSignal4Default1<A, B, C, D> extends AbstractPublicSignal4<A, B, C, D> {
    	
    	private AbstractSignal4Default1(DefaultValue<D> arg4Default){
    		super();
    		if(arg4Default!=null){
    			this.arg4Default = arg4Default;
    		}else{
        		throw new QNoDefaultValueException(this.fullName(), 4);
        	}
    	}
    	
    	private final DefaultValue<D> arg4Default;
        
        public void emit(A arg1, B arg2, C arg3) {
       		emit(arg1, arg2, arg3, arg4Default.get());
        }
    }
    
    protected abstract class Signal4Default1<A, B, C, D> extends AbstractSignal4Default1<A, B, C, D>{
    	public Signal4Default1(DefaultValue<D> arg4Default){
    		super(arg4Default);
    	}
    }
    
    protected abstract class AbstractSignal4Default2<A, B, C, D> extends AbstractSignal4Default1<A, B, C, D> {
    	
    	private AbstractSignal4Default2(DefaultValue<C> arg3Default, DefaultValue<D> arg4Default){
    		super(arg4Default);
    		if(arg3Default!=null){
    			this.arg3Default = arg3Default;
    		}else{
        		throw new QNoDefaultValueException(this.fullName(), 3);
        	}
    	}
    	
    	private final DefaultValue<C> arg3Default;
        
        public void emit(A arg1, B arg2) {
       		emit(arg1, arg2, arg3Default.get());
        }
    }
    
    protected abstract class Signal4Default2<A, B, C, D> extends AbstractSignal4Default2<A, B, C, D>{
    	public Signal4Default2(DefaultValue<C> arg3Default, DefaultValue<D> arg4Default){
    		super(arg3Default, arg4Default);
    	}
    }
    
    protected abstract class AbstractSignal4Default3<A, B, C, D> extends AbstractSignal4Default2<A, B, C, D> {
    	
    	private AbstractSignal4Default3(DefaultValue<B> arg2Default, DefaultValue<C> arg3Default, DefaultValue<D> arg4Default){
    		super(arg3Default, arg4Default);
    		if(arg2Default!=null){
    			this.arg2Default = arg2Default;
    		}else{
        		throw new QNoDefaultValueException(this.fullName(), 2);
        	}
    	}
    	
    	private final DefaultValue<B> arg2Default;
        
        public void emit(A arg1) {
       		emit(arg1, arg2Default.get());
        }
    }
    
    protected abstract class Signal4Default3<A, B, C, D> extends AbstractSignal4Default3<A, B, C, D>{
    	public Signal4Default3(DefaultValue<B> arg2Default, DefaultValue<C> arg3Default, DefaultValue<D> arg4Default){
    		super(arg2Default, arg3Default, arg4Default);
    	}
    }
    
    public final class Signal4Default4<A, B, C, D> extends AbstractSignal4Default3<A, B, C, D> {
    	
    	public Signal4Default4(DefaultValue<A> arg1Default, DefaultValue<B> arg2Default, 
    			DefaultValue<C> arg3Default, DefaultValue<D> arg4Default){
    		super(arg2Default, arg3Default, arg4Default);
    		if(arg1Default!=null){
    			this.arg1Default = arg1Default;
    		}else{
        		throw new QNoDefaultValueException(this.fullName(), 1);
        	}
    	}
    	
    	private final DefaultValue<A> arg1Default;
        
        public void emit() {
       		emit(arg1Default.get());
        }
    }
    
    protected abstract class AbstractSignal5Default1<A, B, C, D, E> extends AbstractPublicSignal5<A, B, C, D, E> {
    	
    	private AbstractSignal5Default1(DefaultValue<E> arg5Default){
    		super();
    		if(arg5Default!=null){
    			this.arg5Default = arg5Default;
    		}else{
        		throw new QNoDefaultValueException(this.fullName(), 5);
        	}
    	}
    	
    	private final DefaultValue<E> arg5Default;
        
        public void emit(A arg1, B arg2, C arg3, D arg4) {
       		emit(arg1, arg2, arg3, arg4, arg5Default.get());
        }
    }
    
    protected abstract class Signal5Default1<A, B, C, D, E> extends AbstractSignal5Default1<A, B, C, D, E>{
    	public Signal5Default1(DefaultValue<E> arg5Default){
    		super(arg5Default);
    	}
    }
    
    protected abstract class AbstractSignal6Default1<A, B, C, D, E, F> extends AbstractPublicSignal6<A, B, C, D, E, F> {
    	
    	private AbstractSignal6Default1(DefaultValue<F> arg6Default){
    		super();
    		if(arg6Default!=null){
    			this.arg6Default = arg6Default;
    		}else{
        		throw new QNoDefaultValueException(this.fullName(), 6);
        	}
    	}
    	
    	private final DefaultValue<F> arg6Default;
        
        public void emit(A arg1, B arg2, C arg3, D arg4, E arg5) {
       		emit(arg1, arg2, arg3, arg4, arg5, arg6Default.get());
        }
    }
    
    protected abstract class Signal6Default1<A, B, C, D, E, F> extends AbstractSignal6Default1<A, B, C, D, E, F>{
    	public Signal6Default1(DefaultValue<F> arg6Default){
    		super(arg6Default);
    	}
    }
    
    protected abstract class AbstractSignal7Default1<A, B, C, D, E, F, G> extends AbstractPublicSignal7<A, B, C, D, E, F, G> {
    	
    	private AbstractSignal7Default1(DefaultValue<G> arg7Default){
    		super();
    		if(arg7Default!=null){
    			this.arg7Default = arg7Default;
    		}else{
        		throw new QNoDefaultValueException(this.fullName(), 7);
        	}
    	}
    	
    	private final DefaultValue<G> arg7Default;
        
        public void emit(A arg1, B arg2, C arg3, D arg4, E arg5, F arg6) {
       		emit(arg1, arg2, arg3, arg4, arg5, arg6, arg7Default.get());
        }
    }
    
    protected abstract class Signal7Default1<A, B, C, D, E, F, G> extends AbstractSignal7Default1<A, B, C, D, E, F, G>{
    	public Signal7Default1(DefaultValue<G> arg7Default){
    		super(arg7Default);
    	}
    }
    
    protected abstract class AbstractSignal8Default1<A, B, C, D, E, F, G, H> extends AbstractPublicSignal8<A, B, C, D, E, F, G, H> {
    	
    	private AbstractSignal8Default1(DefaultValue<H> arg8Default){
    		super();
    		if(arg8Default!=null){
    			this.arg8Default = arg8Default;
    		}else{
        		throw new QNoDefaultValueException(this.fullName(), 8);
        	}
    	}
    	
    	private final DefaultValue<H> arg8Default;
        
        public void emit(A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7) {
       		emit(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8Default.get());
        }
    }
    
    protected abstract class Signal8Default1<A, B, C, D, E, F, G, H> extends AbstractSignal8Default1<A, B, C, D, E, F, G, H>{
    	public Signal8Default1(DefaultValue<H> arg8Default){
    		super(arg8Default);
    	}
    }
    
    protected abstract class AbstractSignal9Default1<A, B, C, D, E, F, G, H, I> extends AbstractPublicSignal9<A, B, C, D, E, F, G, H, I> {
    	
    	private AbstractSignal9Default1(DefaultValue<I> arg9Default){
    		super();
    		if(arg9Default!=null){
    			this.arg9Default = arg9Default;
    		}else{
        		throw new QNoDefaultValueException(this.fullName(), 9);
        	}
    	}
    	
    	private final DefaultValue<I> arg9Default;
        
        public void emit(A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7, H arg8) {
       		emit(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9Default.get());
        }
    }
    
    protected abstract class Signal9Default1<A, B, C, D, E, F, G, H, I> extends AbstractSignal9Default1<A, B, C, D, E, F, G, H, I>{
    	public Signal9Default1(DefaultValue<I> arg9Default){
    		super(arg9Default);
    	}
    }

    /**
     * Declare and instantiate a field of this class in your
     * QSignalEmitter subclass to declare a signal that takes no
     * parameters.
     *
     * Private signals do not have an emit method, as they can only be
     * emitted from inside Qt.
     */
    public final class PrivateSignal0 extends AbstractPrivateSignal0 {
    	/**
         * Emits the signal.
         */
    	@SuppressWarnings("unused")
        private void emit() {
            emit_helper();
        }
    }

    /**
     * Declare and instantiate a field of this class in your
     * QSignalEmitter subclass to declare a signal that takes one
     * parameter.
     *
     * Private signals do not have an emit method, as they can only be
     * emitted from inside Qt.
     *
     * @param <A> The type of the single parameter of the signal.
     */
    public final class PrivateSignal1<A> extends AbstractPrivateSignal1<A> {
    	/**
         * Emits the signal.
         */
    	@SuppressWarnings("unused")
        private void emit(A arg1) {
            emit_helper(arg1);
        }
    }

    /**
     * Declare and instantiate a field of this class in your
     * QSignalEmitter subclass to declare a signal that takes two
     * parameters.
     *
     * Private signals do not have an emit method, as they can only be
     * emitted from inside Qt.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     */
    public final class PrivateSignal2<A, B> extends AbstractPrivateSignal2<A, B> {
    	/**
         * Emits the signal.
         */
    	@SuppressWarnings("unused")
        private void emit(A arg1, B arg2) {
            emit_helper(arg1, arg2);
        }
    }

    /**
     * Declare and instantiate a field of this class in your
     * QSignalEmitter subclass to declare a signal that takes three
     * parameters.
     *
     * Private signals do not have an emit method, as they can only be
     * emitted from inside Qt.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     */
    public final class PrivateSignal3<A, B, C> extends AbstractPrivateSignal3<A, B, C> {
    	/**
         * Emits the signal.
         */
    	@SuppressWarnings("unused")
        private void emit(A arg1, B arg2, C arg3) {
            emit_helper(arg1, arg2, arg3);
        }
    }

    /**
     * Declare and instantiate a field of this class in your
     * QSignalEmitter subclass to declare a signal that takes four
     * parameters.
     *
     * Private signals do not have an emit method, as they can only be
     * emitted from inside Qt.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     */

    public final class PrivateSignal4<A, B, C, D> extends AbstractPrivateSignal4<A, B, C, D> {
        
    	/**
         * Emits the signal.
         */
    	@SuppressWarnings("unused")
        private void emit(A arg1, B arg2, C arg3, D arg4) {
            emit_helper(arg1, arg2, arg3, arg4);
        }
    }

    /**
     * Declare and instantiate a field of this class in your
     * QSignalEmitter subclass to declare a signal that takes five
     * parameters.
     *
     * Private signals do not have an emit method, as they can only be
     * emitted from inside Qt.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     */
    public final class PrivateSignal5<A, B, C, D, E> extends AbstractPrivateSignal5<A,B,C,D,E> {
    	/**
         * Emits the signal.
         */
    	@SuppressWarnings("unused")
        private void emit(A arg1, B arg2, C arg3, D arg4, E arg5) {
            emit_helper(arg1, arg2, arg3, arg4, arg5);
        }
    }

    /**
     * Declare and instantiate a field of this class in your
     * QSignalEmitter subclass to declare a signal that takes six
     * parameters.
     *
     * Private signals do not have an emit method, as they can only be
     * emitted from inside Qt.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     */
    public final class PrivateSignal6<A, B, C, D, E, F> extends AbstractPrivateSignal6<A, B, C, D, E, F> {
    	/**
         * Emits the signal.
         */
    	@SuppressWarnings("unused")
        private void emit(A arg1, B arg2, C arg3, D arg4, E arg5, F arg6) {
            emit_helper(arg1, arg2, arg3, arg4, arg5, arg6);
        }
    }

    /**
     * Declare and instantiate a field of this class in your
     * QSignalEmitter subclass to declare a signal that takes seven
     * parameters.
     *
     * Private signals do not have an emit method, as they can only be
     * emitted from inside Qt.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     */
    public final class PrivateSignal7<A, B, C, D, E, F, G> extends AbstractPrivateSignal7<A, B, C, D, E, F, G> {
    	/**
         * Emits the signal.
         */
    	@SuppressWarnings("unused")
        private void emit(A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7) {
            emit_helper(arg1, arg2, arg3, arg4, arg5, arg6, arg7);
        }
    }

    /**
     * Declare and instantiate a field of this class in your
     * QSignalEmitter subclass to declare a signal that takes eight
     * parameters.
     *
     * Private signals do not have an emit method, as they can only be
     * emitted from inside Qt.
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
    public final class PrivateSignal8<A, B, C, D, E, F, G, H> extends AbstractPrivateSignal8<A, B, C, D, E, F, G, H> {
    	/**
		 * Emits the signal.
		 */
		@SuppressWarnings("unused")
		private void emit(A arg1, B arg2, C arg3, D arg4, E arg5, F arg6,
		        G arg7, H arg8) {
		    emit_helper(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
		}
    }

    /**
     * Declare and instantiate a field of this class in your
     * QSignalEmitter subclass to declare a signal that takes nine
     * parameters.
     *
     * Private signals do not have an emit method, as they can only be
     * emitted from inside Qt.
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
    public final class PrivateSignal9<A, B, C, D, E, F, G, H, I> extends AbstractPrivateSignal9<A, B, C, D, E, F, G, H, I> {
    	/**
         * Emits the signal.
         */
    	@SuppressWarnings("unused")
        private void emit(A arg1, B arg2, C arg3, D arg4, E arg5, F arg6,
                G arg7, H arg8, I arg9) {
            emit_helper(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
        }
    }


    /**
     * Returns the thread affinity of the object. If this is an instance of
     * QObject the thread that owns the object is returned. For non-QObjects
     * the current thread is returned.
     */
    @Override
    public Thread thread() { return Thread.currentThread(); }

    /**
     * Returns true if this QSignalEmitter is blocked. If it is
     * blocked, no signals will be emitted.
     */
    @Override
    public boolean signalsBlocked() {
        return signalsBlocked;
    }

    /**
     * Blocks this QSignalEmitter from emiting its signals.
     */
    public boolean blockSignals(boolean b) {
        boolean returned = signalsBlocked;
        signalsBlocked = b;
        return returned;
    }

    /**
     * If a signal is currently being emitted (e.g. if this method is called from within a slot that has been invoked by a signal),
     * then this function will return the object containing the signal that was emitted.
     * @return Current sender, or null if a signal is not currently being emitted.
     */
    public static QSignalEmitter signalSender() {
        return (QSignalEmitter) currentSender.get();
    }

    /**
     * Disconnect all connections originating in this signal emitter.
     */
    public final void disconnect() {
        disconnect(null);
    }

    /**
     * Disconnect all connections made from this signal emitter to a specific object.
     *
     * @param other The receiver to disconnect, or null to disconnect all receivers
     */
    public final void disconnect(Object other) {
        org.qtjambi.qt.internal.QtJambiInternal.disconnect(this, other);
    }

    private boolean signalsBlocked = false;
}
