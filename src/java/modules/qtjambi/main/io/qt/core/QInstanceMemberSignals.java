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

import java.lang.ref.WeakReference;
import java.util.Objects;
import java.util.function.Supplier;

import io.qt.*;
import io.qt.core.QMetaObject.*;

/**
 * Interface giving access to non-QObject lightweight signals.
 * Subtypes also need to implement {@link QtSignalEmitterInterface} to use signals.
 * Non-QObject signals are never realized as Qt's meta object signals but have a lightweight Java implementation.
 */
public interface QInstanceMemberSignals {
    /**
     * Use this signal class to define a non-QObject signal.
     */
    public static final class Signal0 extends AbstractPublicSignal0 {
        
		/**
    	 * Create signal within containing object.
    	 * @param <Q> type of the containing object.
    	 * @param containingObject the containing object
    	 */
    	public <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal0(@StrictNonNull Q containingObject) {
			super();
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
		}
    	
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
    	@Override
		public @NonNull QtSignalEmitterInterface containingObject() {
			return containingObject.get();
		}
    	
    	private final WeakReference<QtSignalEmitterInterface> containingObject;    	
    }
    
    /**
     * Use this signal class to define a non-QObject signal.
     *
     * @param <A> The type of the single parameter of the signal.
     */
    public static final class Signal1<A> extends AbstractPublicSignal1<A> {
        
		/**
    	 * Create signal within containing object.
    	 * @param <Q> type of the containing object.
    	 * @param containingObject the containing object
    	 */
    	public <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal1(@StrictNonNull Q containingObject) {
			super();
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
		}
    	
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
    	@Override
		public @NonNull QtSignalEmitterInterface containingObject() {
			return containingObject.get();
		}
    	
    	private final WeakReference<QtSignalEmitterInterface> containingObject;    	
    }
    
    /**
     * Use this signal class to define a non-QObject signal.
     *
     * @param <A> The type of the single parameter of the signal.
     * @param <B> The type of the single parameter of the signal.
     */
    public static final class Signal2<A,B> extends AbstractPublicSignal2<A,B> {
        
		/**
    	 * Create signal within containing object.
    	 * @param <Q> type of the containing object.
    	 * @param containingObject the containing object
    	 */
    	public <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal2(@StrictNonNull Q containingObject) {
			super();
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
		}
    	
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
    	@Override
		public @NonNull QtSignalEmitterInterface containingObject() {
			return containingObject.get();
		}
    	
    	private final WeakReference<QtSignalEmitterInterface> containingObject;    	
    }
    
    /**
     * Use this signal class to define a non-QObject signal.
     *
     * @param <A> The type of the single parameter of the signal.
     * @param <B> The type of the single parameter of the signal.
     * @param <C> The type of the single parameter of the signal.
     */
    public static final class Signal3<A,B,C> extends AbstractPublicSignal3<A,B,C> {
        
		/**
    	 * Create signal within containing object.
    	 * @param <Q> type of the containing object.
    	 * @param containingObject the containing object
    	 */
    	public <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal3(@StrictNonNull Q containingObject) {
			super();
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
		}
    	
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
    	@Override
		public @NonNull QtSignalEmitterInterface containingObject() {
			return containingObject.get();
		}
    	
    	private final WeakReference<QtSignalEmitterInterface> containingObject;    	
    }
    
    /**
     * Use this signal class to define a non-QObject signal.
     *
     * @param <A> The type of the single parameter of the signal.
     * @param <B> The type of the single parameter of the signal.
     * @param <C> The type of the single parameter of the signal.
     * @param <D> The type of the single parameter of the signal.
     */
    public static final class Signal4<A,B,C,D> extends AbstractPublicSignal4<A,B,C,D> {
        
		/**
    	 * Create signal within containing object.
    	 * @param <Q> type of the containing object.
    	 * @param containingObject the containing object
    	 */
    	public <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal4(@StrictNonNull Q containingObject) {
			super();
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
		}
    	
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
    	@Override
		public @NonNull QtSignalEmitterInterface containingObject() {
			return containingObject.get();
		}
    	
    	private final WeakReference<QtSignalEmitterInterface> containingObject;    	
    }
    
    /**
     * Use this signal class to define a non-QObject signal.
     *
     * @param <A> The type of the single parameter of the signal.
     * @param <B> The type of the single parameter of the signal.
     * @param <C> The type of the single parameter of the signal.
     * @param <D> The type of the single parameter of the signal.
     * @param <E> The type of the single parameter of the signal.
     */
    public static final class Signal5<A,B,C,D,E> extends AbstractPublicSignal5<A,B,C,D,E> {
        
		/**
    	 * Create signal within containing object.
    	 * @param <Q> type of the containing object.
    	 * @param containingObject the containing object
    	 */
    	public <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal5(@StrictNonNull Q containingObject) {
			super();
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
		}
    	
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
    	@Override
		public @NonNull QtSignalEmitterInterface containingObject() {
			return containingObject.get();
		}
    	
    	private final WeakReference<QtSignalEmitterInterface> containingObject;    	
    }
    
    /**
     * Use this signal class to define a non-QObject signal.
     *
     * @param <A> The type of the single parameter of the signal.
     * @param <B> The type of the single parameter of the signal.
     * @param <C> The type of the single parameter of the signal.
     * @param <D> The type of the single parameter of the signal.
     * @param <E> The type of the single parameter of the signal.
     * @param <F> The type of the single parameter of the signal.
     */
    public static final class Signal6<A,B,C,D,E,F> extends AbstractPublicSignal6<A,B,C,D,E,F> {
        
		public <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal6(@StrictNonNull Q containingObject) {
			super();
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
		}
    	
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
    	@Override
		public @NonNull QtSignalEmitterInterface containingObject() {
			return containingObject.get();
		}
    	
    	private final WeakReference<QtSignalEmitterInterface> containingObject;    	
    }
    
    /**
     * Use this signal class to define a non-QObject signal.
     *
     * @param <A> The type of the single parameter of the signal.
     * @param <B> The type of the single parameter of the signal.
     * @param <C> The type of the single parameter of the signal.
     * @param <D> The type of the single parameter of the signal.
     * @param <E> The type of the single parameter of the signal.
     * @param <F> The type of the single parameter of the signal.
     * @param <G> The type of the single parameter of the signal.
     */
    public static final class Signal7<A,B,C,D,E,F,G> extends AbstractPublicSignal7<A,B,C,D,E,F,G> {
        
		/**
    	 * Create signal within containing object.
    	 * @param <Q> type of the containing object.
    	 * @param containingObject the containing object
    	 */
    	public <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal7(@StrictNonNull Q containingObject) {
			super();
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
		}
    	
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
    	@Override
		public @NonNull QtSignalEmitterInterface containingObject() {
			return containingObject.get();
		}
    	
    	private final WeakReference<QtSignalEmitterInterface> containingObject;    	
    }
    
    /**
     * Use this signal class to define a non-QObject signal.
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
    public static final class Signal8<A,B,C,D,E,F,G,H> extends AbstractPublicSignal8<A,B,C,D,E,F,G,H> {
        
		public <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal8(@StrictNonNull Q containingObject) {
			super();
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
		}
    	
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
    	@Override
		public @NonNull QtSignalEmitterInterface containingObject() {
			return containingObject.get();
		}
    	
    	private final WeakReference<QtSignalEmitterInterface> containingObject;    	
    }
    
    /**
     * Use this signal class to define a non-QObject signal.
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
    public static final class Signal9<A,B,C,D,E,F,G,H,I> extends AbstractPublicSignal9<A,B,C,D,E,F,G,H,I> {
        
		/**
    	 * Create signal within containing object.
    	 * @param <Q> type of the containing object.
    	 * @param containingObject the containing object
    	 */
    	public <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal9(@StrictNonNull Q containingObject) {
			super();
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
		}
    	
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
    	@Override
		public @NonNull QtSignalEmitterInterface containingObject() {
			return containingObject.get();
		}
    	
    	private final WeakReference<QtSignalEmitterInterface> containingObject;    	
    }

    /**
     * Declare and instantiate a field of this class in your
     * <code>QtSignalEmitterInterface</code> subclass to declare a signal that takes nine
     * parameters.
     *
     * Private signals do not have an emit method, as they can only be
     * emitted from inside the declaring class.
     */
    public static final class PrivateSignal0 extends AbstractPrivateSignal0 {
            	/**
    	 * Create signal within containing object.
    	 * @param <Q> type of the containing object.
    	 * @param containingObject the containing object
    	 */
    	public <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> PrivateSignal0(@StrictNonNull Q containingObject) {
			super();
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
		}
    	
    	@QtUninvokable
        private void emit() {
            emitSignal();
        }
    	
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
    	@Override
		public @NonNull QtSignalEmitterInterface containingObject() {
			return containingObject.get();
		}
    	
    	private final WeakReference<QtSignalEmitterInterface> containingObject;    	
    }
    
    /**
     * Declare and instantiate a field of this class in your
     * <code>QtSignalEmitterInterface</code> subclass to declare a signal that takes nine
     * parameters.
     *
     * Private signals do not have an emit method, as they can only be
     * emitted from inside the declaring class.
     *
     * @param <A> The type of the single parameter of the signal.
     */
    public static final class PrivateSignal1<A> extends AbstractPrivateSignal1<A> {
            	/**
    	 * Create signal within containing object.
    	 * @param <Q> type of the containing object.
    	 * @param containingObject the containing object
    	 */
    	public <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> PrivateSignal1(@StrictNonNull Q containingObject) {
			super();
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
		}
    	
    	@QtUninvokable
        private void emit(A arg1) {
            emitSignal(arg1);
        }
    	
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
    	@Override
		public @NonNull QtSignalEmitterInterface containingObject() {
			return containingObject.get();
		}
    	
    	private final WeakReference<QtSignalEmitterInterface> containingObject;    	
    }
    
    /**
     * Declare and instantiate a field of this class in your
     * <code>QtSignalEmitterInterface</code> subclass to declare a signal that takes nine
     * parameters.
     *
     * Private signals do not have an emit method, as they can only be
     * emitted from inside the declaring class.
     *
     * @param <A> The type of the single parameter of the signal.
     * @param <B> The type of the single parameter of the signal.
     */
    public static final class PrivateSignal2<A,B> extends AbstractPrivateSignal2<A,B> {
            	/**
    	 * Create signal within containing object.
    	 * @param <Q> type of the containing object.
    	 * @param containingObject the containing object
    	 */
    	public <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> PrivateSignal2(@StrictNonNull Q containingObject) {
			super();
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
		}
    	
    	@QtUninvokable
        private void emit(A arg1, B arg2) {
            emitSignal(arg1, arg2);
        }
    	
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
    	@Override
		public @NonNull QtSignalEmitterInterface containingObject() {
			return containingObject.get();
		}
    	
    	private final WeakReference<QtSignalEmitterInterface> containingObject;    	
    }
    
    /**
     * Declare and instantiate a field of this class in your
     * <code>QtSignalEmitterInterface</code> subclass to declare a signal that takes nine
     * parameters.
     *
     * Private signals do not have an emit method, as they can only be
     * emitted from inside the declaring class.
     *
     * @param <A> The type of the single parameter of the signal.
     * @param <B> The type of the single parameter of the signal.
     * @param <C> The type of the single parameter of the signal.
     */
    public static final class PrivateSignal3<A,B,C> extends AbstractPrivateSignal3<A,B,C> {
            	/**
    	 * Create signal within containing object.
    	 * @param <Q> type of the containing object.
    	 * @param containingObject the containing object
    	 */
    	public <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> PrivateSignal3(@StrictNonNull Q containingObject) {
			super();
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
		}
    	
    	@QtUninvokable
        private void emit(A arg1, B arg2, C arg3) {
            emitSignal(arg1, arg2, arg3);
        }
    	
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
    	@Override
		public @NonNull QtSignalEmitterInterface containingObject() {
			return containingObject.get();
		}
    	
    	private final WeakReference<QtSignalEmitterInterface> containingObject;    	
    }
    
    /**
     * Declare and instantiate a field of this class in your
     * <code>QtSignalEmitterInterface</code> subclass to declare a signal that takes nine
     * parameters.
     *
     * Private signals do not have an emit method, as they can only be
     * emitted from inside the declaring class.
     *
     * @param <A> The type of the single parameter of the signal.
     * @param <B> The type of the single parameter of the signal.
     * @param <C> The type of the single parameter of the signal.
     * @param <D> The type of the single parameter of the signal.
     */
    public static final class PrivateSignal4<A,B,C,D> extends AbstractPrivateSignal4<A,B,C,D> {
            	/**
    	 * Create signal within containing object.
    	 * @param <Q> type of the containing object.
    	 * @param containingObject the containing object
    	 */
    	public <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> PrivateSignal4(@StrictNonNull Q containingObject) {
			super();
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
		}
    	
    	@QtUninvokable
        private void emit(A arg1, B arg2, C arg3, D arg4) {
            emitSignal(arg1, arg2, arg3, arg4);
        }
    	
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
    	@Override
		public @NonNull QtSignalEmitterInterface containingObject() {
			return containingObject.get();
		}
    	
    	private final WeakReference<QtSignalEmitterInterface> containingObject;    	
    }
    
    /**
     * Declare and instantiate a field of this class in your
     * <code>QtSignalEmitterInterface</code> subclass to declare a signal that takes nine
     * parameters.
     *
     * Private signals do not have an emit method, as they can only be
     * emitted from inside the declaring class.
     *
     * @param <A> The type of the single parameter of the signal.
     * @param <B> The type of the single parameter of the signal.
     * @param <C> The type of the single parameter of the signal.
     * @param <D> The type of the single parameter of the signal.
     * @param <E> The type of the single parameter of the signal.
     */
    public static final class PrivateSignal5<A,B,C,D,E> extends AbstractPrivateSignal5<A,B,C,D,E> {
            	/**
    	 * Create signal within containing object.
    	 * @param <Q> type of the containing object.
    	 * @param containingObject the containing object
    	 */
    	public <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> PrivateSignal5(@StrictNonNull Q containingObject) {
			super();
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
		}
    	
    	@QtUninvokable
        private void emit(A arg1, B arg2, C arg3, D arg4, E arg5) {
            emitSignal(arg1, arg2, arg3, arg4, arg5);
        }
    	
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
    	@Override
		public @NonNull QtSignalEmitterInterface containingObject() {
			return containingObject.get();
		}
    	
    	private final WeakReference<QtSignalEmitterInterface> containingObject;    	
    }
    
    /**
     * Declare and instantiate a field of this class in your
     * <code>QtSignalEmitterInterface</code> subclass to declare a signal that takes nine
     * parameters.
     *
     * Private signals do not have an emit method, as they can only be
     * emitted from inside the declaring class.
     *
     * @param <A> The type of the single parameter of the signal.
     * @param <B> The type of the single parameter of the signal.
     * @param <C> The type of the single parameter of the signal.
     * @param <D> The type of the single parameter of the signal.
     * @param <E> The type of the single parameter of the signal.
     * @param <F> The type of the single parameter of the signal.
     */
    public static final class PrivateSignal6<A,B,C,D,E,F> extends AbstractPrivateSignal6<A,B,C,D,E,F> {
            	/**
    	 * Create signal within containing object.
    	 * @param <Q> type of the containing object.
    	 * @param containingObject the containing object
    	 */
    	public <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> PrivateSignal6(@StrictNonNull Q containingObject) {
			super();
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
		}
    	
    	@QtUninvokable
        private void emit(A arg1, B arg2, C arg3, D arg4, E arg5, F arg6) {
            emitSignal(arg1, arg2, arg3, arg4, arg5, arg6);
        }
    	
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
    	@Override
		public @NonNull QtSignalEmitterInterface containingObject() {
			return containingObject.get();
		}
    	
    	private final WeakReference<QtSignalEmitterInterface> containingObject;    	
    }
    
    /**
     * Declare and instantiate a field of this class in your
     * <code>QtSignalEmitterInterface</code> subclass to declare a signal that takes nine
     * parameters.
     *
     * Private signals do not have an emit method, as they can only be
     * emitted from inside the declaring class.
     *
     * @param <A> The type of the single parameter of the signal.
     * @param <B> The type of the single parameter of the signal.
     * @param <C> The type of the single parameter of the signal.
     * @param <D> The type of the single parameter of the signal.
     * @param <E> The type of the single parameter of the signal.
     * @param <F> The type of the single parameter of the signal.
     * @param <G> The type of the single parameter of the signal.
     */
    public static final class PrivateSignal7<A,B,C,D,E,F,G> extends AbstractPrivateSignal7<A,B,C,D,E,F,G> {
            	/**
    	 * Create signal within containing object.
    	 * @param <Q> type of the containing object.
    	 * @param containingObject the containing object
    	 */
    	public <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> PrivateSignal7(@StrictNonNull Q containingObject) {
			super();
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
		}
    	
    	@QtUninvokable
        private void emit(A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7) {
            emitSignal(arg1, arg2, arg3, arg4, arg5, arg6, arg7);
        }
    	
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
    	@Override
		public @NonNull QtSignalEmitterInterface containingObject() {
			return containingObject.get();
		}
    	
    	private final WeakReference<QtSignalEmitterInterface> containingObject;    	
    }
    
    /**
     * Declare and instantiate a field of this class in your
     * <code>QtSignalEmitterInterface</code> subclass to declare a signal that takes nine
     * parameters.
     *
     * Private signals do not have an emit method, as they can only be
     * emitted from inside the declaring class.
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
    public static final class PrivateSignal8<A,B,C,D,E,F,G,H> extends AbstractPrivateSignal8<A,B,C,D,E,F,G,H> {
            	/**
    	 * Create signal within containing object.
    	 * @param <Q> type of the containing object.
    	 * @param containingObject the containing object
    	 */
    	public <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> PrivateSignal8(@StrictNonNull Q containingObject) {
			super();
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
		}
    	
    	@QtUninvokable
        private void emit(A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7, H arg8) {
            emitSignal(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
        }
    	
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
    	@Override
		public @NonNull QtSignalEmitterInterface containingObject() {
			return containingObject.get();
		}
    	
    	private final WeakReference<QtSignalEmitterInterface> containingObject;    	
    }
    
    /**
     * Declare and instantiate a field of this class in your
     * <code>QtSignalEmitterInterface</code> subclass to declare a signal that takes nine
     * parameters.
     *
     * Private signals do not have an emit method, as they can only be
     * emitted from inside the declaring class.
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
    public static final class PrivateSignal9<A,B,C,D,E,F,G,H,I> extends AbstractPrivateSignal9<A,B,C,D,E,F,G,H,I> {
    	/**
    	 * Create signal within containing object.
    	 * @param <Q> type of the containing object.
    	 * @param containingObject the containing object
    	 */
    	public <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> PrivateSignal9(@StrictNonNull Q containingObject) {
			super();
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
		}
    	
    	@QtUninvokable
        private void emit(A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7, H arg8, I arg9) {
            emitSignal(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
        }
    	
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
    	@Override
		public @NonNull QtSignalEmitterInterface containingObject() {
			return containingObject.get();
		}
    	
    	private final WeakReference<QtSignalEmitterInterface> containingObject;    	
    }
    
    public static final class Signal1Default1<A> extends AbstractSignal1Default1<A>{
        
    	@SuppressWarnings("unused")
		private <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal1Default1(@StrictNonNull Q containingObject) {
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
    	}
    	
		/**
    	 * Create signal within containing object.
    	 * @param <Q> type of the containing object.
    	 * @param containingObject the containing object
    	 */
    	public <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal1Default1(@StrictNonNull Q containingObject, @StrictNonNull Supplier<A> arg1Default){
    		super(arg1Default);
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
    	}
    	
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
    	@Override
		public @NonNull QtSignalEmitterInterface containingObject() {
			return containingObject.get();
		}
    	
    	private final WeakReference<QtSignalEmitterInterface> containingObject;    	
    }
    
    /**
     * Equivalent to {@link Signal2} with default value for parameter no. 2.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     */
    public static final class Signal2Default1<A, B> extends AbstractSignal2Default1<A, B>{
        
    	@SuppressWarnings("unused")
		private <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal2Default1(@StrictNonNull Q containingObject) {
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
    	}
        
		/**
    	 * Create signal within containing object.
    	 * @param <Q> type of the containing object.
    	 * @param containingObject the containing object
    	 */
		public <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal2Default1(@StrictNonNull Q containingObject, @StrictNonNull Supplier<B> arg2Default) {
			super(arg2Default);
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
		}
    	
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
    	@Override
		public @NonNull QtSignalEmitterInterface containingObject() {
			return containingObject.get();
		}
    	
    	private final WeakReference<QtSignalEmitterInterface> containingObject;    	
	}
    
    /**
     * Equivalent to {@link Signal2} with default values for parameters 1 and 2.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     */
    public static final class Signal2Default2<A, B> extends AbstractSignal2Default2<A, B>{
        
    	@SuppressWarnings("unused")
		private <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal2Default2(@StrictNonNull Q containingObject) {
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
    	}
    	
		/**
    	 * Create signal within containing object.
    	 * @param <Q> type of the containing object.
    	 * @param containingObject the containing object
    	 */
    	public <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal2Default2(@StrictNonNull Q containingObject, @StrictNonNull Supplier<A> arg1Default, @StrictNonNull Supplier<B> arg2Default){
    		super(arg1Default, arg2Default);
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
    	}
    	
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
    	@Override
		public @NonNull QtSignalEmitterInterface containingObject() {
			return containingObject.get();
		}
    	
    	private final WeakReference<QtSignalEmitterInterface> containingObject;    	
    }
    
    /**
     * Equivalent to {@link Signal3} with default value for parameter no. 3.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     */
    public static final class Signal3Default1<A, B, C> extends AbstractSignal3Default1<A, B, C> {
        
    	@SuppressWarnings("unused")
		private <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal3Default1(@StrictNonNull Q containingObject) {
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
    	}
        
		/**
    	 * Create signal within containing object.
    	 * @param <Q> type of the containing object.
    	 * @param containingObject the containing object
    	 */
		public <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal3Default1(@StrictNonNull Q containingObject, @StrictNonNull Supplier<C> arg3Default) {
			super(arg3Default);
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
		}
    	
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
    	@Override
		public @NonNull QtSignalEmitterInterface containingObject() {
			return containingObject.get();
		}
    	
    	private final WeakReference<QtSignalEmitterInterface> containingObject;    	
    }
    
    /**
     * Equivalent to {@link Signal3} with default values for parameters 2 and 3.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     */
    public static final class Signal3Default2<A, B, C> extends AbstractSignal3Default2<A, B, C> {
        
    	@SuppressWarnings("unused")
		private <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal3Default2(@StrictNonNull Q containingObject) {
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
    	}
        
		public <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal3Default2(@StrictNonNull Q containingObject, @StrictNonNull Supplier<B> arg2Default, @StrictNonNull Supplier<C> arg3Default){
    		super(arg2Default, arg3Default);
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
    	}
    	
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
    	@Override
		public @NonNull QtSignalEmitterInterface containingObject() {
			return containingObject.get();
		}
    	
    	private final WeakReference<QtSignalEmitterInterface> containingObject;    	
    }
    
    /**
     * Equivalent to {@link Signal3} with default values for parameters 1 to 3.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     */
    public static final class Signal3Default3<A, B, C> extends AbstractSignal3Default3<A, B, C>{
        
    	@SuppressWarnings("unused")
		private <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal3Default3(@StrictNonNull Q containingObject) {
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
    	}
    	
		/**
    	 * Create signal within containing object.
    	 * @param <Q> type of the containing object.
    	 * @param containingObject the containing object
    	 */
    	public <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal3Default3(@StrictNonNull Q containingObject, @StrictNonNull Supplier<A> arg1Default, @StrictNonNull Supplier<B> arg2Default, @StrictNonNull Supplier<C> arg3Default){
    		super(arg1Default, arg2Default, arg3Default);
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
    	}
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
    	@Override
		public @NonNull QtSignalEmitterInterface containingObject() {
			return containingObject.get();
		}
    	
    	private final WeakReference<QtSignalEmitterInterface> containingObject;    	
    }
    
    /**
     * Equivalent to {@link Signal4} with default value for parameter no. 4.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     */
    public static final class Signal4Default1<A, B, C, D> extends AbstractSignal4Default1<A, B, C, D>{
        
    	@SuppressWarnings("unused")
		private <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal4Default1(@StrictNonNull Q containingObject) {
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
    	}
        
		public <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal4Default1(@StrictNonNull Q containingObject, @StrictNonNull Supplier<D> arg4Default){
    		super(arg4Default);
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
    	}
    	
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
    	@Override
		public @NonNull QtSignalEmitterInterface containingObject() {
			return containingObject.get();
		}
    	
    	private final WeakReference<QtSignalEmitterInterface> containingObject;    	
    }
    
    /**
     * Equivalent to {@link Signal4} with default values for parameters 3 and 4.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     */
    public static final class Signal4Default2<A, B, C, D> extends AbstractSignal4Default2<A, B, C, D>{
        
    	@SuppressWarnings("unused")
		private <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal4Default2(@StrictNonNull Q containingObject) {
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
    	}
        
		/**
    	 * Create signal within containing object.
    	 * @param <Q> type of the containing object.
    	 * @param containingObject the containing object
    	 */
    	public <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal4Default2(@StrictNonNull Q containingObject, @StrictNonNull Supplier<C> arg3Default, @StrictNonNull Supplier<D> arg4Default){
    		super(arg3Default, arg4Default);
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
    	}
    	
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
    	@Override
		public @NonNull QtSignalEmitterInterface containingObject() {
			return containingObject.get();
		}
    	
    	private final WeakReference<QtSignalEmitterInterface> containingObject;    	
    }
    
    /**
     * Equivalent to {@link Signal4} with default values for parameters 2 to 4.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     */
    public static final class Signal4Default3<A, B, C, D> extends AbstractSignal4Default3<A, B, C, D>{
        
    	@SuppressWarnings("unused")
		private <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal4Default3(@StrictNonNull Q containingObject) {
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
    	}
        
		/**
    	 * Create signal within containing object.
    	 * @param <Q> type of the containing object.
    	 * @param containingObject the containing object
    	 */
    	public <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal4Default3(@StrictNonNull Q containingObject, @StrictNonNull Supplier<B> arg2Default, @StrictNonNull Supplier<C> arg3Default, @StrictNonNull Supplier<D> arg4Default){
    		super(arg2Default, arg3Default, arg4Default);
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
    	}
    	
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
    	@Override
		public @NonNull QtSignalEmitterInterface containingObject() {
			return containingObject.get();
		}
    	
    	private final WeakReference<QtSignalEmitterInterface> containingObject;    	
    }
    
    /**
     * Equivalent to {@link Signal4} with default values for parameters 1 to 4.
     *
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     */
    public static final class Signal4Default4<A, B, C, D> extends AbstractSignal4Default4<A, B, C, D>{
        
    	@SuppressWarnings("unused")
		private <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal4Default4(@StrictNonNull Q containingObject) {
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
    	}
    	
		/**
    	 * Create signal within containing object.
    	 * @param <Q> type of the containing object.
    	 * @param containingObject the containing object
    	 */
    	public <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal4Default4(
    			@StrictNonNull Q containingObject, 
    			@StrictNonNull Supplier<A> arg1Default, @StrictNonNull Supplier<B> arg2Default, 
    			@StrictNonNull Supplier<C> arg3Default, @StrictNonNull Supplier<D> arg4Default){
    		super(arg1Default, arg2Default, arg3Default, arg4Default);
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
    	}
    	
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
    	@Override
		public @NonNull QtSignalEmitterInterface containingObject() {
			return containingObject.get();
		}
    	
    	private final WeakReference<QtSignalEmitterInterface> containingObject;    	
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
    public static final class Signal5Default1<A, B, C, D, E> extends AbstractSignal5Default1<A, B, C, D, E>{
        
    	@SuppressWarnings("unused")
		private <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal5Default1(@StrictNonNull Q containingObject) {
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
    	}
        
		/**
    	 * Create signal within containing object.
    	 * @param <Q> type of the containing object.
    	 * @param containingObject the containing object
    	 */
    	public <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal5Default1(@StrictNonNull Q containingObject, @StrictNonNull Supplier<E> arg5Default){
    		super(arg5Default);
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
    	}
    	
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
    	@Override
		public @NonNull QtSignalEmitterInterface containingObject() {
			return containingObject.get();
		}
    	
    	private final WeakReference<QtSignalEmitterInterface> containingObject;    	
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
    public static final class Signal5Default2<A, B, C, D, E> extends AbstractSignal5Default2<A, B, C, D, E>{
        
    	@SuppressWarnings("unused")
		private <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal5Default2(@StrictNonNull Q containingObject) {
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
    	}
        
		/**
    	 * Create signal within containing object.
    	 * @param <Q> type of the containing object.
    	 * @param containingObject the containing object
    	 */
    	public <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal5Default2(@StrictNonNull Q containingObject, @StrictNonNull Supplier<D> arg4Default, @StrictNonNull Supplier<E> arg5Default){
    		super(arg4Default, arg5Default);
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
    	}
    	
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
    	@Override
		public @NonNull QtSignalEmitterInterface containingObject() {
			return containingObject.get();
		}
    	
    	private final WeakReference<QtSignalEmitterInterface> containingObject;    	
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
    public static final class Signal5Default3<A, B, C, D, E> extends AbstractSignal5Default3<A, B, C, D, E>{
        
    	@SuppressWarnings("unused")
		private <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal5Default3(@StrictNonNull Q containingObject) {
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
    	}
        
		/**
    	 * Create signal within containing object.
    	 * @param <Q> type of the containing object.
    	 * @param containingObject the containing object
    	 */
    	public <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal5Default3(@StrictNonNull Q containingObject, @StrictNonNull Supplier<C> arg3Default, @StrictNonNull Supplier<D> arg4Default, @StrictNonNull Supplier<E> arg5Default){
    		super(arg3Default, arg4Default, arg5Default);
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
    	}
    	
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
    	@Override
		public @NonNull QtSignalEmitterInterface containingObject() {
			return containingObject.get();
		}
    	
    	private final WeakReference<QtSignalEmitterInterface> containingObject;    	
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
    public static final class Signal5Default4<A, B, C, D, E> extends AbstractSignal5Default4<A, B, C, D, E>{
        
    	@SuppressWarnings("unused")
		private <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal5Default4(@StrictNonNull Q containingObject) {
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
    	}
        
		/**
    	 * Create signal within containing object.
    	 * @param <Q> type of the containing object.
    	 * @param containingObject the containing object
    	 */
    	public <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal5Default4(@StrictNonNull Q containingObject, @StrictNonNull Supplier<B> arg2Default, @StrictNonNull Supplier<C> arg3Default, @StrictNonNull Supplier<D> arg4Default, @StrictNonNull Supplier<E> arg5Default){
    		super(arg2Default, arg3Default, arg4Default, arg5Default);
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
    	}
    	
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
    	@Override
		public @NonNull QtSignalEmitterInterface containingObject() {
			return containingObject.get();
		}
    	
    	private final WeakReference<QtSignalEmitterInterface> containingObject;    	
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
    public static final class Signal5Default5<A, B, C, D, E> extends AbstractSignal5Default5<A, B, C, D, E>{
        
    	@SuppressWarnings("unused")
		private <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal5Default5(@StrictNonNull Q containingObject) {
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
    	}
    	
		/**
    	 * Create signal within containing object.
    	 * @param <Q> type of the containing object.
    	 * @param containingObject the containing object
    	 */
    	public <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal5Default5(@StrictNonNull Q containingObject, @StrictNonNull Supplier<A> arg1Default, @StrictNonNull Supplier<B> arg2Default, @StrictNonNull Supplier<C> arg3Default, @StrictNonNull Supplier<D> arg4Default, @StrictNonNull Supplier<E> arg5Default){
    		super(arg1Default, arg2Default, arg3Default, arg4Default, arg5Default);
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
    	}
    	
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
    	@Override
		public @NonNull QtSignalEmitterInterface containingObject() {
			return containingObject.get();
		}
    	
    	private final WeakReference<QtSignalEmitterInterface> containingObject;    	
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
    public static final class Signal6Default1<A, B, C, D, E, F> extends AbstractSignal6Default1<A, B, C, D, E, F>{
        
    	@SuppressWarnings("unused")
		private <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal6Default1(@StrictNonNull Q containingObject) {
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
    	}
        
		/**
    	 * Create signal within containing object.
    	 * @param <Q> type of the containing object.
    	 * @param containingObject the containing object
    	 */
    	public <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal6Default1(@StrictNonNull Q containingObject, @StrictNonNull Supplier<F> arg6Default){
    		super(arg6Default);
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
    	}
    	
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
    	@Override
		public @NonNull QtSignalEmitterInterface containingObject() {
			return containingObject.get();
		}
    	
    	private final WeakReference<QtSignalEmitterInterface> containingObject;    	
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
    public static final class Signal6Default2<A, B, C, D, E, F> extends AbstractSignal6Default2<A, B, C, D, E, F>{
        
    	@SuppressWarnings("unused")
		private <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal6Default2(@StrictNonNull Q containingObject) {
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
    	}
        
		/**
    	 * Create signal within containing object.
    	 * @param <Q> type of the containing object.
    	 * @param containingObject the containing object
    	 */
    	public <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal6Default2(@StrictNonNull Q containingObject, @StrictNonNull Supplier<E> arg5Default, @StrictNonNull Supplier<F> arg6Default){
    		super(arg5Default, arg6Default);
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
    	}
    	
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
    	@Override
		public @NonNull QtSignalEmitterInterface containingObject() {
			return containingObject.get();
		}
    	
    	private final WeakReference<QtSignalEmitterInterface> containingObject;    	
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
        
    	@SuppressWarnings("unused")
		private <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal6Default3(@StrictNonNull Q containingObject) {
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
    	}
        
		/**
    	 * Create signal within containing object.
    	 * @param <Q> type of the containing object.
    	 * @param containingObject the containing object
    	 */
    	public <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal6Default3(@StrictNonNull Q containingObject, @StrictNonNull Supplier<D> arg4Default, @StrictNonNull Supplier<E> arg5Default, @StrictNonNull Supplier<F> arg6Default){
    		super(arg4Default, arg5Default, arg6Default);
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
    	}
    	
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
    	@Override
		public @NonNull QtSignalEmitterInterface containingObject() {
			return containingObject.get();
		}
    	
    	private final WeakReference<QtSignalEmitterInterface> containingObject;    	
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
        
    	@SuppressWarnings("unused")
		private <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal6Default4(@StrictNonNull Q containingObject) {
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
    	}
        
		/**
    	 * Create signal within containing object.
    	 * @param <Q> type of the containing object.
    	 * @param containingObject the containing object
    	 */
    	public <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal6Default4(@StrictNonNull Q containingObject, @StrictNonNull Supplier<C> arg3Default, @StrictNonNull Supplier<D> arg4Default, @StrictNonNull Supplier<E> arg5Default, @StrictNonNull Supplier<F> arg6Default){
    		super(arg3Default, arg4Default, arg5Default, arg6Default);
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
    	}
    	
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
    	@Override
		public @NonNull QtSignalEmitterInterface containingObject() {
			return containingObject.get();
		}
    	
    	private final WeakReference<QtSignalEmitterInterface> containingObject;    	
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
        
    	@SuppressWarnings("unused")
		private <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal6Default5(@StrictNonNull Q containingObject) {
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
    	}
        
		/**
    	 * Create signal within containing object.
    	 * @param <Q> type of the containing object.
    	 * @param containingObject the containing object
    	 */
    	public <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal6Default5(@StrictNonNull Q containingObject, @StrictNonNull Supplier<B> arg2Default, @StrictNonNull Supplier<C> arg3Default, @StrictNonNull Supplier<D> arg4Default, @StrictNonNull Supplier<E> arg5Default, @StrictNonNull Supplier<F> arg6Default){
    		super(arg2Default, arg3Default, arg4Default, arg5Default, arg6Default);
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
    	}
    	
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
    	@Override
		public @NonNull QtSignalEmitterInterface containingObject() {
			return containingObject.get();
		}
    	
    	private final WeakReference<QtSignalEmitterInterface> containingObject;    	
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
        
    	@SuppressWarnings("unused")
		private <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal6Default6(@StrictNonNull Q containingObject) {
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
    	}
    	
		/**
    	 * Create signal within containing object.
    	 * @param <Q> type of the containing object.
    	 * @param containingObject the containing object
    	 */
    	public <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal6Default6(@StrictNonNull Q containingObject, @StrictNonNull Supplier<A> arg1Default, @StrictNonNull Supplier<B> arg2Default, @StrictNonNull Supplier<C> arg3Default, @StrictNonNull Supplier<D> arg4Default, @StrictNonNull Supplier<E> arg5Default, @StrictNonNull Supplier<F> arg6Default){
    		super(arg1Default, arg2Default, arg3Default, arg4Default, arg5Default, arg6Default);
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
    	}
    	
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
    	@Override
		public @NonNull QtSignalEmitterInterface containingObject() {
			return containingObject.get();
		}
    	
    	private final WeakReference<QtSignalEmitterInterface> containingObject;    	
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
        
    	@SuppressWarnings("unused")
		private <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal7Default1(@StrictNonNull Q containingObject) {
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
    	}
        
		/**
    	 * Create signal within containing object.
    	 * @param <Q> type of the containing object.
    	 * @param containingObject the containing object
    	 */
    	public <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal7Default1(@StrictNonNull Q containingObject, @StrictNonNull Supplier<G> arg7Default){
    		super(arg7Default);
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
    	}
    	
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
    	@Override
		public @NonNull QtSignalEmitterInterface containingObject() {
			return containingObject.get();
		}
    	
    	private final WeakReference<QtSignalEmitterInterface> containingObject;    	
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
        
    	@SuppressWarnings("unused")
		private <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal7Default2(@StrictNonNull Q containingObject) {
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
    	}
        
		/**
    	 * Create signal within containing object.
    	 * @param <Q> type of the containing object.
    	 * @param containingObject the containing object
    	 */
    	public <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal7Default2(@StrictNonNull Q containingObject, @StrictNonNull Supplier<F> arg6Default, @StrictNonNull Supplier<G> arg7Default){
    		super(arg6Default, arg7Default);
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
    	}
    	
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
    	@Override
		public @NonNull QtSignalEmitterInterface containingObject() {
			return containingObject.get();
		}
    	
    	private final WeakReference<QtSignalEmitterInterface> containingObject;    	
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
        
    	@SuppressWarnings("unused")
		private <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal7Default3(@StrictNonNull Q containingObject) {
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
    	}
        
		/**
    	 * Create signal within containing object.
    	 * @param <Q> type of the containing object.
    	 * @param containingObject the containing object
    	 */
    	public <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal7Default3(@StrictNonNull Q containingObject, @StrictNonNull Supplier<E> arg5Default, @StrictNonNull Supplier<F> arg6Default, @StrictNonNull Supplier<G> arg7Default){
    		super(arg5Default, arg6Default, arg7Default);
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
    	}
    	
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
    	@Override
		public @NonNull QtSignalEmitterInterface containingObject() {
			return containingObject.get();
		}
    	
    	private final WeakReference<QtSignalEmitterInterface> containingObject;    	
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
        
    	@SuppressWarnings("unused")
		private <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal7Default4(@StrictNonNull Q containingObject) {
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
    	}
        
		/**
    	 * Create signal within containing object.
    	 * @param <Q> type of the containing object.
    	 * @param containingObject the containing object
    	 */
    	public <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal7Default4(@StrictNonNull Q containingObject, @StrictNonNull Supplier<D> arg4Default, @StrictNonNull Supplier<E> arg5Default, @StrictNonNull Supplier<F> arg6Default, @StrictNonNull Supplier<G> arg7Default){
    		super(arg4Default, arg5Default, arg6Default, arg7Default);
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
    	}
    	
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
    	@Override
		public @NonNull QtSignalEmitterInterface containingObject() {
			return containingObject.get();
		}
    	
    	private final WeakReference<QtSignalEmitterInterface> containingObject;    	
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
        
    	@SuppressWarnings("unused")
		private <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal7Default5(@StrictNonNull Q containingObject) {
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
    	}
        
		/**
    	 * Create signal within containing object.
    	 * @param <Q> type of the containing object.
    	 * @param containingObject the containing object
    	 */
    	public <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal7Default5(@StrictNonNull Q containingObject, @StrictNonNull Supplier<C> arg3Default, @StrictNonNull Supplier<D> arg4Default, @StrictNonNull Supplier<E> arg5Default, @StrictNonNull Supplier<F> arg6Default, @StrictNonNull Supplier<G> arg7Default){
    		super(arg3Default, arg4Default, arg5Default, arg6Default, arg7Default);
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
    	}
    	
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
    	@Override
		public @NonNull QtSignalEmitterInterface containingObject() {
			return containingObject.get();
		}
    	
    	private final WeakReference<QtSignalEmitterInterface> containingObject;    	
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
        
    	@SuppressWarnings("unused")
		private <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal7Default6(@StrictNonNull Q containingObject) {
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
    	}
        
		/**
    	 * Create signal within containing object.
    	 * @param <Q> type of the containing object.
    	 * @param containingObject the containing object
    	 */
    	public <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal7Default6(@StrictNonNull Q containingObject, @StrictNonNull Supplier<B> arg2Default, @StrictNonNull Supplier<C> arg3Default, @StrictNonNull Supplier<D> arg4Default, @StrictNonNull Supplier<E> arg5Default, @StrictNonNull Supplier<F> arg6Default, @StrictNonNull Supplier<G> arg7Default){
    		super(arg2Default, arg3Default, arg4Default, arg5Default, arg6Default, arg7Default);
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
    	}
    	
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
    	@Override
		public @NonNull QtSignalEmitterInterface containingObject() {
			return containingObject.get();
		}
    	
    	private final WeakReference<QtSignalEmitterInterface> containingObject;    	
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
        
    	@SuppressWarnings("unused")
		private <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal7Default7(@StrictNonNull Q containingObject) {
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
    	}
    	
		/**
    	 * Create signal within containing object.
    	 * @param <Q> type of the containing object.
    	 * @param containingObject the containing object
    	 */
    	public <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal7Default7(@StrictNonNull Q containingObject, @StrictNonNull Supplier<A> arg1Default, @StrictNonNull Supplier<B> arg2Default, @StrictNonNull Supplier<C> arg3Default, @StrictNonNull Supplier<D> arg4Default, @StrictNonNull Supplier<E> arg5Default, @StrictNonNull Supplier<F> arg6Default, @StrictNonNull Supplier<G> arg7Default){
    		super(arg1Default, arg2Default, arg3Default, arg4Default, arg5Default, arg6Default, arg7Default);
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
    	}
    	
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
    	@Override
		public @NonNull QtSignalEmitterInterface containingObject() {
			return containingObject.get();
		}
    	
    	private final WeakReference<QtSignalEmitterInterface> containingObject;    	
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
        
    	@SuppressWarnings("unused")
		private <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal8Default1(@StrictNonNull Q containingObject) {
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
    	}
        
		/**
    	 * Create signal within containing object.
    	 * @param <Q> type of the containing object.
    	 * @param containingObject the containing object
    	 */
    	public <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal8Default1(@StrictNonNull Q containingObject, @StrictNonNull Supplier<H> arg8Default){
    		super(arg8Default);
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
    	}
    	
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
    	@Override
		public @NonNull QtSignalEmitterInterface containingObject() {
			return containingObject.get();
		}
    	
    	private final WeakReference<QtSignalEmitterInterface> containingObject;    	
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
        
    	@SuppressWarnings("unused")
		private <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal8Default2(@StrictNonNull Q containingObject) {
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
    	}
        
		/**
    	 * Create signal within containing object.
    	 * @param <Q> type of the containing object.
    	 * @param containingObject the containing object
    	 */
    	public <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal8Default2(@StrictNonNull Q containingObject, @StrictNonNull Supplier<G> arg7Default, @StrictNonNull Supplier<H> arg8Default){
    		super(arg7Default, arg8Default);
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
    	}
    	
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
    	@Override
		public @NonNull QtSignalEmitterInterface containingObject() {
			return containingObject.get();
		}
    	
    	private final WeakReference<QtSignalEmitterInterface> containingObject;    	
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
        
    	@SuppressWarnings("unused")
		private <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal8Default3(@StrictNonNull Q containingObject) {
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
    	}
        
		/**
    	 * Create signal within containing object.
    	 * @param <Q> type of the containing object.
    	 * @param containingObject the containing object
    	 */
    	public <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal8Default3(@StrictNonNull Q containingObject, @StrictNonNull Supplier<F> arg6Default, @StrictNonNull Supplier<G> arg7Default, @StrictNonNull Supplier<H> arg8Default){
    		super(arg6Default, arg7Default, arg8Default);
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
    	}
    	
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
    	@Override
		public @NonNull QtSignalEmitterInterface containingObject() {
			return containingObject.get();
		}
    	
    	private final WeakReference<QtSignalEmitterInterface> containingObject;    	
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
        
    	@SuppressWarnings("unused")
		private <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal8Default4(@StrictNonNull Q containingObject) {
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
    	}
        
		/**
    	 * Create signal within containing object.
    	 * @param <Q> type of the containing object.
    	 * @param containingObject the containing object
    	 */
    	public <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal8Default4(@StrictNonNull Q containingObject, @StrictNonNull Supplier<E> arg5Default, @StrictNonNull Supplier<F> arg6Default, @StrictNonNull Supplier<G> arg7Default, @StrictNonNull Supplier<H> arg8Default){
    		super(arg5Default, arg6Default, arg7Default, arg8Default);
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
    	}
    	
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
    	@Override
		public @NonNull QtSignalEmitterInterface containingObject() {
			return containingObject.get();
		}
    	
    	private final WeakReference<QtSignalEmitterInterface> containingObject;    	
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
        
    	@SuppressWarnings("unused")
		private <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal8Default5(@StrictNonNull Q containingObject) {
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
    	}
        
		/**
    	 * Create signal within containing object.
    	 * @param <Q> type of the containing object.
    	 * @param containingObject the containing object
    	 */
    	public <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal8Default5(@StrictNonNull Q containingObject, @StrictNonNull Supplier<D> arg4Default, @StrictNonNull Supplier<E> arg5Default, @StrictNonNull Supplier<F> arg6Default, @StrictNonNull Supplier<G> arg7Default, @StrictNonNull Supplier<H> arg8Default){
    		super(arg4Default, arg5Default, arg6Default, arg7Default, arg8Default);
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
    	}
    	
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
    	@Override
		public @NonNull QtSignalEmitterInterface containingObject() {
			return containingObject.get();
		}
    	
    	private final WeakReference<QtSignalEmitterInterface> containingObject;    	
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
        
    	@SuppressWarnings("unused")
		private <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal8Default6(@StrictNonNull Q containingObject) {
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
    	}
        
		/**
    	 * Create signal within containing object.
    	 * @param <Q> type of the containing object.
    	 * @param containingObject the containing object
    	 */
    	public <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal8Default6(@StrictNonNull Q containingObject, @StrictNonNull Supplier<C> arg3Default, @StrictNonNull Supplier<D> arg4Default, @StrictNonNull Supplier<E> arg5Default, @StrictNonNull Supplier<F> arg6Default, @StrictNonNull Supplier<G> arg7Default, @StrictNonNull Supplier<H> arg8Default){
    		super(arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default);
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
    	}
    	
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
    	@Override
		public @NonNull QtSignalEmitterInterface containingObject() {
			return containingObject.get();
		}
    	
    	private final WeakReference<QtSignalEmitterInterface> containingObject;    	
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
        
    	@SuppressWarnings("unused")
		private <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal8Default7(@StrictNonNull Q containingObject) {
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
    	}
        
		/**
    	 * Create signal within containing object.
    	 * @param <Q> type of the containing object.
    	 * @param containingObject the containing object
    	 */
    	public <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal8Default7(@StrictNonNull Q containingObject, @StrictNonNull Supplier<B> arg2Default, @StrictNonNull Supplier<C> arg3Default, @StrictNonNull Supplier<D> arg4Default, @StrictNonNull Supplier<E> arg5Default, @StrictNonNull Supplier<F> arg6Default, @StrictNonNull Supplier<G> arg7Default, @StrictNonNull Supplier<H> arg8Default){
    		super(arg2Default, arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default);
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
    	}
    	
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
    	@Override
		public @NonNull QtSignalEmitterInterface containingObject() {
			return containingObject.get();
		}
    	
    	private final WeakReference<QtSignalEmitterInterface> containingObject;    	
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
        
    	@SuppressWarnings("unused")
		private <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal8Default8(@StrictNonNull Q containingObject) {
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
    	}
    	
		/**
    	 * Create signal within containing object.
    	 * @param <Q> type of the containing object.
    	 * @param containingObject the containing object
    	 */
    	public <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal8Default8(@StrictNonNull Q containingObject, @StrictNonNull Supplier<A> arg1Default, @StrictNonNull Supplier<B> arg2Default, @StrictNonNull Supplier<C> arg3Default, @StrictNonNull Supplier<D> arg4Default, @StrictNonNull Supplier<E> arg5Default, @StrictNonNull Supplier<F> arg6Default, @StrictNonNull Supplier<G> arg7Default, @StrictNonNull Supplier<H> arg8Default){
    		super(arg1Default, arg2Default, arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default);
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
    	}
    	
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
    	@Override
		public @NonNull QtSignalEmitterInterface containingObject() {
			return containingObject.get();
		}
    	
    	private final WeakReference<QtSignalEmitterInterface> containingObject;    	
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
        
    	@SuppressWarnings("unused")
		private <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal9Default1(@StrictNonNull Q containingObject) {
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
    	}
        
		/**
    	 * Create signal within containing object.
    	 * @param <Q> type of the containing object.
    	 * @param containingObject the containing object
    	 */
    	public <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal9Default1(@StrictNonNull Q containingObject, @StrictNonNull Supplier<I> arg9Default){
    		super(arg9Default);
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
    	}
    	
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
    	@Override
		public @NonNull QtSignalEmitterInterface containingObject() {
			return containingObject.get();
		}
    	
    	private final WeakReference<QtSignalEmitterInterface> containingObject;    	
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
        
    	@SuppressWarnings("unused")
		private <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal9Default2(@StrictNonNull Q containingObject) {
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
    	}
        
		/**
    	 * Create signal within containing object.
    	 * @param <Q> type of the containing object.
    	 * @param containingObject the containing object
    	 */
    	public <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal9Default2(@StrictNonNull Q containingObject, @StrictNonNull Supplier<H> arg8Default, @StrictNonNull Supplier<I> arg9Default){
    		super(arg8Default, arg9Default);
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
    	}
    	
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
    	@Override
		public @NonNull QtSignalEmitterInterface containingObject() {
			return containingObject.get();
		}
    	
    	private final WeakReference<QtSignalEmitterInterface> containingObject;    	
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
        
    	@SuppressWarnings("unused")
		private <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal9Default3(@StrictNonNull Q containingObject) {
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
    	}
        
		/**
    	 * Create signal within containing object.
    	 * @param <Q> type of the containing object.
    	 * @param containingObject the containing object
    	 */
    	public <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal9Default3(@StrictNonNull Q containingObject, @StrictNonNull Supplier<G> arg7Default, @StrictNonNull Supplier<H> arg8Default, @StrictNonNull Supplier<I> arg9Default){
    		super(arg7Default, arg8Default, arg9Default);
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
    	}
    	
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
    	@Override
		public @NonNull QtSignalEmitterInterface containingObject() {
			return containingObject.get();
		}
    	
    	private final WeakReference<QtSignalEmitterInterface> containingObject;    	
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
        
    	@SuppressWarnings("unused")
		private <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal9Default4(@StrictNonNull Q containingObject) {
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
    	}
        
		/**
    	 * Create signal within containing object.
    	 * @param <Q> type of the containing object.
    	 * @param containingObject the containing object
    	 */
    	public <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal9Default4(@StrictNonNull Q containingObject, @StrictNonNull Supplier<F> arg6Default, @StrictNonNull Supplier<G> arg7Default, @StrictNonNull Supplier<H> arg8Default, @StrictNonNull Supplier<I> arg9Default){
    		super(arg6Default, arg7Default, arg8Default, arg9Default);
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
    	}
    	
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
    	@Override
		public @NonNull QtSignalEmitterInterface containingObject() {
			return containingObject.get();
		}
    	
    	private final WeakReference<QtSignalEmitterInterface> containingObject;    	
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
        
    	@SuppressWarnings("unused")
		private <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal9Default5(@StrictNonNull Q containingObject) {
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
    	}
        
		/**
    	 * Create signal within containing object.
    	 * @param <Q> type of the containing object.
    	 * @param containingObject the containing object
    	 */
    	public <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal9Default5(@StrictNonNull Q containingObject, @StrictNonNull Supplier<E> arg5Default, @StrictNonNull Supplier<F> arg6Default, @StrictNonNull Supplier<G> arg7Default, @StrictNonNull Supplier<H> arg8Default, @StrictNonNull Supplier<I> arg9Default){
    		super(arg5Default, arg6Default, arg7Default, arg8Default, arg9Default);
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
    	}
    	
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
    	@Override
		public @NonNull QtSignalEmitterInterface containingObject() {
			return containingObject.get();
		}
    	
    	private final WeakReference<QtSignalEmitterInterface> containingObject;    	
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
        
    	@SuppressWarnings("unused")
		private <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal9Default6(@StrictNonNull Q containingObject) {
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
    	}
        
		/**
    	 * Create signal within containing object.
    	 * @param <Q> type of the containing object.
    	 * @param containingObject the containing object
    	 */
    	public <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal9Default6(@StrictNonNull Q containingObject, @StrictNonNull Supplier<D> arg4Default, @StrictNonNull Supplier<E> arg5Default, @StrictNonNull Supplier<F> arg6Default, @StrictNonNull Supplier<G> arg7Default, @StrictNonNull Supplier<H> arg8Default, @StrictNonNull Supplier<I> arg9Default){
    		super(arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, arg9Default);
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
		}
    	
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
    	@Override
		public @NonNull QtSignalEmitterInterface containingObject() {
			return containingObject.get();
		}
    	
    	private final WeakReference<QtSignalEmitterInterface> containingObject;    	
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
        
    	@SuppressWarnings("unused")
		private <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal9Default7(@StrictNonNull Q containingObject) {
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
    	}
        
		/**
    	 * Create signal within containing object.
    	 * @param <Q> type of the containing object.
    	 * @param containingObject the containing object
    	 */
    	public <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal9Default7(@StrictNonNull Q containingObject, @StrictNonNull Supplier<C> arg3Default, @StrictNonNull Supplier<D> arg4Default, @StrictNonNull Supplier<E> arg5Default, @StrictNonNull Supplier<F> arg6Default, @StrictNonNull Supplier<G> arg7Default, @StrictNonNull Supplier<H> arg8Default, @StrictNonNull Supplier<I> arg9Default){
    		super(arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, arg9Default);
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
    	}
    	
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
    	@Override
		public @NonNull QtSignalEmitterInterface containingObject() {
			return containingObject.get();
		}
    	
    	private final WeakReference<QtSignalEmitterInterface> containingObject;    	
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
        
    	@SuppressWarnings("unused")
		private <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal9Default8(@StrictNonNull Q containingObject) {
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
    	}
    	
		/**
    	 * Create signal within containing object.
    	 * @param <Q> type of the containing object.
    	 * @param containingObject the containing object
    	 */
    	public <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal9Default8(@StrictNonNull Q containingObject, @StrictNonNull Supplier<B> arg2Default, @StrictNonNull Supplier<C> arg3Default, @StrictNonNull Supplier<D> arg4Default, @StrictNonNull Supplier<E> arg5Default, @StrictNonNull Supplier<F> arg6Default, @StrictNonNull Supplier<G> arg7Default, @StrictNonNull Supplier<H> arg8Default, @StrictNonNull Supplier<I> arg9Default){
    		super(arg2Default, arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, arg9Default);
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
    	}
    	
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
    	@Override
		public @NonNull QtSignalEmitterInterface containingObject() {
			return containingObject.get();
		}
    	
    	private final WeakReference<QtSignalEmitterInterface> containingObject;    	
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
    	
    	@SuppressWarnings("unused")
		private <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal9Default9(@StrictNonNull Q containingObject) {
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
    	}
        
		/**
    	 * Create signal within containing object.
    	 * @param <Q> type of the containing object.
    	 * @param containingObject the containing object
    	 */
    	public <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> Signal9Default9(@StrictNonNull Q containingObject, @StrictNonNull Supplier<A> arg1Default, @StrictNonNull Supplier<B> arg2Default, @StrictNonNull Supplier<C> arg3Default, @StrictNonNull Supplier<D> arg4Default, @StrictNonNull Supplier<E> arg5Default, @StrictNonNull Supplier<F> arg6Default, @StrictNonNull Supplier<G> arg7Default, @StrictNonNull Supplier<H> arg8Default, @StrictNonNull Supplier<I> arg9Default){
    		super(arg1Default, arg2Default, arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, arg9Default);
			this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
    	}
    	
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
    	@Override
		public @NonNull QtSignalEmitterInterface containingObject() {
			return containingObject.get();
		}
    	
    	private final WeakReference<QtSignalEmitterInterface> containingObject;    	
    }
    
    /**
     * Wrapper class supporting the definition of overloaded signal in a class.
     */
    public abstract class MultiSignal extends QMetaObject.AbstractMultiSignal {
    	private final WeakReference<QtSignalEmitterInterface> containingObject;
    	
    	protected <Q extends Object & QtSignalEmitterInterface & QInstanceMemberSignals> MultiSignal(@StrictNonNull Q containingObject){
            super();
            this.containingObject = new WeakReference<>(Objects.requireNonNull(containingObject));
        }
        
        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
    	@Override
		public @NonNull QtSignalEmitterInterface containingObject() {
			return containingObject.get();
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
        public final io.qt.core.QMetaObject.@NonNull Connection connect(io.qt.core.QMetaObject.@StrictNonNull Slot0 slot, io.qt.core.Qt.@StrictNonNull ConnectionType @StrictNonNull... connectionType) throws io.qt.QNoSuchSignalException{
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
