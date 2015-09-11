/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2015 Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
** $END_LICENSE$

**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

package org.qtjambi.qt.internal;

import java.lang.ref.WeakReference;
import java.lang.reflect.Field;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.lang.reflect.Modifier;
import java.util.ArrayList;
import java.util.List;

import org.qtjambi.qt.QBlockedSlotException;
import org.qtjambi.qt.QSignalEmitter;
import org.qtjambi.qt.QSignalEmitter.Slot0;
import org.qtjambi.qt.QtBlockedSlot;
import org.qtjambi.qt.QtJambiGeneratedClass;
import org.qtjambi.qt.core.QCoreApplication;
import org.qtjambi.qt.core.QObject;
import org.qtjambi.qt.core.Qt;

public abstract class QSignalEmitterInternal {
    /**
     * QSignalEmitter is a class used internally by Qt Jambi.
     * You should never have to concern yourself with this class.
     * @exclude
     */
    public abstract class AbstractSignalInternal {

        private boolean             inCppEmission       = false;
        private List<Connection>    connections         = null;
        private Class<?>            types[]             = null;
        private int                 arrayDimensions[]   = null;
        private String              name                = "";
        private Class<?>            declaringClass      = null;
        private boolean             connectedToCpp      = false;
        private boolean             inDisconnect        = false;
        @SuppressWarnings("unused")  // used from QtJambi C++ code
        private boolean             inJavaEmission      = false;
        
        protected abstract class Connection{
			public static final int DIRECT_CONNECTION = 0x01;
			public static final int QUEUED_CONNECTION = 0x02;
			public static final int PUBLIC_SLOT = 0x10;
			public static final int HARD_REFERENCE_CONNECTION = 0x40;
			public static final int CONNECTION_TYPE_MASK = 0x87;
			
			private final WeakReference<Object> weakReceiver;
			private final Object receiver;
			public final int flags;
			public 		Object 					args[] = null;
            public  	int                   	convertTypes[]  = null;
			
			public Connection(Object receiver, byte connectionType, boolean isPublic) {
				int flags = 0;
                if((connectionType & HARD_REFERENCE_CONNECTION) != 0) {
                    this.receiver = receiver;
                    this.weakReceiver = null;
                    flags |= HARD_REFERENCE_CONNECTION;
                } else {
                	this.receiver = null;
                    this.weakReceiver = new WeakReference<Object>(receiver);
                }

                if ((connectionType & CONNECTION_TYPE_MASK) == Qt.ConnectionType.QueuedConnection.value())
                    flags |= QUEUED_CONNECTION;
                else if ((connectionType & CONNECTION_TYPE_MASK) == Qt.ConnectionType.DirectConnection.value())
                    flags |= DIRECT_CONNECTION;
                if (isPublic)
                    flags |= PUBLIC_SLOT;
                this.flags = flags;
            }
						
			public final boolean isSlotPublic() {
			    return (flags & PUBLIC_SLOT) != 0;
			}
			public final boolean isQueuedConnection() {
			    return (flags & QUEUED_CONNECTION) != 0;
			}
			public final boolean isDirectConnection() {
			    return (flags & DIRECT_CONNECTION) != 0;
			}
			public final boolean isAutoConnection() {
			    return (flags & (QUEUED_CONNECTION | DIRECT_CONNECTION)) == 0;
			}
			public final boolean isHardReferenceConnection() {
			    return (flags & HARD_REFERENCE_CONNECTION) != 0;
			}
			public final Object resolveReceiver() {
			    if(this.weakReceiver != null)
			        return weakReceiver.get();
			    else
			        return receiver;
			}
        } // protected abstract class Connection

        /**
         * Contains book holding info about a single connection
         * @exclude
         */
        protected class ReflectiveConnection extends Connection{
            public final Method slot;
            public final byte returnType;
            public final long slotId;
            
            public ReflectiveConnection(Object receiver, Method slot, byte returnType, byte connectionType) {
            	super(receiver, connectionType, Modifier.isPublic(slot.getModifiers())
            									&& Modifier.isPublic(receiver.getClass().getModifiers()));
                this.slot = slot;
                this.slotId = org.qtjambi.qt.internal.QtJambiInternal.resolveSlot(slot);
                this.returnType = returnType;

                Class<?> slotParameterTypes[] = slot.getParameterTypes();
                Class<?> signalParameterTypes[] = resolveSignal();
                convertTypes = QtJambiInternal.resolveConversionSchema(signalParameterTypes, slotParameterTypes);
            }
        } // protected class ReflectiveConnection

        protected class SlotObjectConnection extends Connection {
        	private final WeakReference<Object> lambdaOwnerRef;
			public final Class<?> lambdaOwnerClass;
			
			public SlotObjectConnection(Object lambdaOwner, Class<?> lambdaOwnerClass, Object slotObject, byte connectionType) {
				super(slotObject, connectionType, true);
				this.lambdaOwnerRef = new WeakReference<Object>(lambdaOwner);
				this.lambdaOwnerClass = lambdaOwnerClass;
				
				Class<?> slotParameterTypes[];
                Class<?> signalParameterTypes[] = resolveSignal();
				if(slotObject instanceof QSignalEmitter.Slot0){
					slotParameterTypes = new Class[]{};
				}else if(slotObject instanceof QSignalEmitter.Slot1){
					slotParameterTypes = new Class[]{Object.class};
				}else if(slotObject instanceof QSignalEmitter.Slot2){
					slotParameterTypes = new Class[]{Object.class,Object.class};
				}else if(slotObject instanceof QSignalEmitter.Slot3){
					slotParameterTypes = new Class[]{Object.class,Object.class,Object.class};
				}else if(slotObject instanceof QSignalEmitter.Slot4){
					slotParameterTypes = new Class[]{Object.class,Object.class,Object.class,
													 Object.class};
				}else if(slotObject instanceof QSignalEmitter.Slot5){
					slotParameterTypes = new Class[]{Object.class,Object.class,Object.class,
													 Object.class,Object.class};
				}else if(slotObject instanceof QSignalEmitter.Slot6){
					slotParameterTypes = new Class[]{Object.class,Object.class,Object.class,
													 Object.class,Object.class,Object.class};
				}else if(slotObject instanceof QSignalEmitter.Slot7){
					slotParameterTypes = new Class[]{Object.class,Object.class,Object.class,
							 						 Object.class,Object.class,Object.class,
							 						 Object.class};
				}else if(slotObject instanceof QSignalEmitter.Slot8){
					slotParameterTypes = new Class[]{Object.class,Object.class,Object.class,
							 						 Object.class,Object.class,Object.class,
							 						 Object.class,Object.class};
				}else if(slotObject instanceof QSignalEmitter.Slot9){
					slotParameterTypes = new Class[]{Object.class,Object.class,Object.class,
							 						 Object.class,Object.class,Object.class,
							 						 Object.class,Object.class,Object.class};
				}else{
					throw new RuntimeException("Cannot connect to slot object of type "+slotObject.getClass());
				}
                convertTypes = QtJambiInternal.resolveConversionSchema(signalParameterTypes, slotParameterTypes);
			}
			
			public Object lambdaOwner(){
				return lambdaOwnerRef.get();
			}
        } // protected class SlotObjectConnection

        /**
         * Returns the object containing this signal
         *
         * @exclude
         */
        public final QSignalEmitterInternal containingObject() {
            return QSignalEmitterInternal.this;
        }

        /**
         * Returns the name of the signal
         * @return The signal name
         */
        public final String name() {
            resolveSignal();
            return name;
        }

        /**
         * Returns the name of the class containing the signal
         * @return The fully qualified name of the class declaring the signal
         */
        public final String declaringClassName() {
            resolveSignal();
            return declaringClass == null ? "" : declaringClass.getName();
        }

        /**
         * Returns the full name of the signal, on the form "package.class.signalName"
         *
         *  @return The fully qualified name of the signal
         */
        public final String fullName() {
            return declaringClassName() + "." + name();
        }

        /**
         * @return True if the signal is generated (declared in a generated class)
         */
        private boolean isGenerated() {
            resolveSignal();
            return declaringClass == null ? false : declaringClass.isAnnotationPresent(QtJambiGeneratedClass.class);
        }

        /**
         * Returns true if the connection receiver is the emission of the C++ version of the current
         * signal. This is used to avoid recursion from C++ emissions. Whenever we have a C++ emission
         * we know we will have a function with the same name in the same declaring class as the signal.
         *
         * @param connection The connection to check
         * @return True if true...
         */
        private boolean slotIsCppEmit(ReflectiveConnection connection) {
            return (connection.slot.getName().equals(name())
                    && connection.resolveReceiver() == QSignalEmitterInternal.this
                    && connection.slot.getDeclaringClass().equals(declaringClass));
        }

        protected void connectSignalMethod(Method slotMethod,
                                            Object receiver,
                                            int connectionType) {
            // FIXME: We need to find out if it is save to override methods and call super.method()
            //  when one of the supers might have @QtBlockedSlot set.  First we need to understand the
            //  reason it is blocked, what breaksdown.
            if (slotMethod.getAnnotation(QtBlockedSlot.class) != null)
                throw new QBlockedSlotException(slotMethod.toString());

            if (!matchSlot(slotMethod))
                throw new RuntimeException("Signature of signal '" + fullName() + "' does not match slot '" + slotMethod.toString() + "'");

            addConnection(receiver, slotMethod, connectionType);
        }
        
        protected void addConnection(QSignalEmitter.Slot0 slotObject, int connectionType) {
    		addConnectionToSlotObject(slotObject, connectionType);
        }
        
        protected void addConnection(QSignalEmitter.Slot1<?> slotObject, int connectionType) {
			addConnectionToSlotObject(slotObject, connectionType);
		}
        
        protected void addConnection(QSignalEmitter.Slot2<?,?> slotObject, int connectionType) {
			addConnectionToSlotObject(slotObject, connectionType);
		}
        
        protected void addConnection(QSignalEmitter.Slot3<?,?,?> slotObject, int connectionType) {
			addConnectionToSlotObject(slotObject, connectionType);
		}
        
        protected void addConnection(QSignalEmitter.Slot4<?,?,?,?> slotObject, int connectionType) {
			addConnectionToSlotObject(slotObject, connectionType);
		}
        
        protected void addConnection(QSignalEmitter.Slot5<?,?,?,?,?> slotObject, int connectionType) {
			addConnectionToSlotObject(slotObject, connectionType);
		}
        
        protected void addConnection(QSignalEmitter.Slot6<?,?,?,?,?,?> slotObject, int connectionType) {
			addConnectionToSlotObject(slotObject, connectionType);
		}
        
        protected void addConnection(QSignalEmitter.Slot7<?,?,?,?,?,?,?> slotObject, int connectionType) {
			addConnectionToSlotObject(slotObject, connectionType);
		}
        
        protected void addConnection(QSignalEmitter.Slot8<?,?,?,?,?,?,?,?> slotObject, int connectionType) {
			addConnectionToSlotObject(slotObject, connectionType);
		}
        
        protected void addConnection(QSignalEmitter.Slot9<?,?,?,?,?,?,?,?,?> slotObject, int connectionType) {
			addConnectionToSlotObject(slotObject, connectionType);
		}

        /**
         * Array dimensions for each of the signal arguments in order of declaration. Used in combination with
         * resolveSignal(). An array dimension of 0 means the argument is not of an array type.
         *
         * @return An array of integers indicating the number of dimensions of each of the signal arguments
         */
        int[] arrayDimensions() {
            resolveSignal();
            return arrayDimensions;
        }

        /**
         * Base types of all signal arguments in order of declaration. If the argument is of an array type,
         * then the base type of the array is returned by resolveSignal, and the actual number of dimensions
         * of the array can be retrieved using arrayDimensions(). If the argument is not of an array type,
         * the argument's type is returned.
         *
         * @return An array of Class objects specifying the base type of each of the signal arguments.
         */
        Class<?>[] resolveSignal() {
            if (types == null) {
                boolean found = false;
                types = new Class[0]; // For signals with no parameters
                arrayDimensions = new int[0];

                Class<?> cls = QSignalEmitterInternal.this.getClass();
                while (cls != null) {
                    Field fields[] = cls.getDeclaredFields();
                    for (Field field : fields) {
                        if (AbstractSignalInternal.class.isAssignableFrom(field.getType())) {
                            AbstractSignalInternal sig = QtJambiInternal.fetchSignal(QSignalEmitterInternal.this, field);
                            if (sig == this) {
                                found = true;
                                declaringClass = field.getDeclaringClass();

                                QtJambiInternal.ResolvedSignal resolvedSignal = QtJambiInternal.resolveSignal(field, declaringClass);

                                name = resolvedSignal.name;
                                types = resolvedSignal.types;
                                arrayDimensions = resolvedSignal.arrayDimensions;

                                break;
                            }
                        }
                    }

                    cls = cls.getSuperclass();
                }

                if (!found) {
                    throw new RuntimeException("Signals must be declared as members of QSignalEmitter subclasses");
                }
            }

            if (types.length == 0
                && !(this instanceof QSignalEmitter.Signal0)
                && !(this instanceof QSignalEmitter.PrivateSignal0))
                throw new RuntimeException("Signal initialization failed");

            return types;
        }

        // Cache string containing list of Java argument types for signal
        private String signalParameters = null;
        private String signalParameters() {
            if (signalParameters == null)
                signalParameters = QtJambiInternal.signalParameters(this);

            return signalParameters;
        }


        // Cache string containing cpp signature for signal
        private String cppSignalSignature = null;
        private String cppSignalSignature() {
            if (cppSignalSignature == null)
                cppSignalSignature = MetaObjectTools.cppSignalSignature(this);

            return cppSignalSignature;
        }

        /**
         * @exclude
         */
        @SuppressWarnings({ "rawtypes", "unchecked" })
		protected synchronized final void emit_helper(Object ... args) {
        	
            // #### THIS NO LONGER WORKS IN Qt5 ####
        	// try to connect signal to native cpp signal if not yet happened.
        	if (!connectedToCpp) {
                connectedToCpp = true;
                __qt_signalInitialization(name());
            }

            // When you dispose() a QObject, the first thing that happens is
            // that the native ID is set to 0. Then we proceed to delete the
            // native object. If this has a destructor which emits signals,
            // they will be passed into Java, which will lead to a threadCheck()
            // from signalsBlocked, and you will get an exception. We cannot
            // support listening to signals emitted in the C++ destructor.
            if (QSignalEmitterInternal.this instanceof QObject
                && (((QObject) QSignalEmitterInternal.this).nativeId()) == 0) {
                return ;
            }

            // #### THIS NO LONGER WORKS IN Qt5 ####
            // If the signal is generated, it will automatically be connected
            // to the original C++ function for the signal, so the native
            // signal will be emitted by this mechanism. In other cases, we
            // need to make magic and dynamically fake a signal emission
            // in c++ for the signal.
			// #### THIS NO LONGER WORKS IN Qt5 ####
            if (!isGenerated() && QSignalEmitterInternal.this instanceof QObject) {
                MetaObjectTools.emitNativeSignal((QObject) QSignalEmitterInternal.this, name() + "(" + signalParameters() + ")", cppSignalSignature(), args);
            }

            List<Connection> cons = connections;
            if (cons == null)
                return;

            inJavaEmission = true;
            try {
                List<Connection> toRemove = null;

                for (Connection c : cons) {

                    Object receiver = c.resolveReceiver();
                    Object lambdaOwner = null;
                    if(c instanceof SlotObjectConnection){
                    	lambdaOwner = ((SlotObjectConnection) c).lambdaOwner();
                    }
                    // If the receiver has been deleted we take the connection out of the list
                    if (receiver == null ||
                        (receiver instanceof QtJambiObject && ((QtJambiObject)receiver).nativeId() == 0) ||
                        (lambdaOwner instanceof QtJambiObject && ((QtJambiObject)lambdaOwner).nativeId() == 0)) {
                        if (toRemove == null)
                            toRemove = new ArrayList<Connection>();
                        toRemove.add(c);
                        continue;
                    }

                    if (inCppEmission && c instanceof ReflectiveConnection && slotIsCppEmit((ReflectiveConnection)c))
                        continue;

                    if (args.length == c.convertTypes.length) {
                        c.args = args;
                    } else {
                        if (c.args == null)
                            c.args = new Object[c.convertTypes.length];
                        System.arraycopy(args, 0, c.args, 0, c.args.length);
                    }

                    // We do a direct connection in three cases:
                    // 1. If the connection is explicitly set to be direct
                    // 2. If it is automatic and the receiver is not a QObject (no thread() function)
                    // 3. If it is automatic, the receiver is a QObject and the sender and receiver
                    //    are both in the current thread
                    boolean notEmitterFlag;
                    boolean receiverWithAffinity;
                    if(lambdaOwner!=null){
                    	notEmitterFlag = !(lambdaOwner instanceof QSignalEmitter);
                    	receiverWithAffinity = (lambdaOwner instanceof QSignalEmitter
                                && ((QSignalEmitter) lambdaOwner).thread() == Thread.currentThread()
                                && ((QSignalEmitter) lambdaOwner).thread() == thread());
                    }else{
                    	notEmitterFlag = !(receiver instanceof QSignalEmitter);
                    	receiverWithAffinity = (receiver instanceof QSignalEmitter
                                && ((QSignalEmitter) receiver).thread() == Thread.currentThread()
                                && ((QSignalEmitter) receiver).thread() == thread());
                    }
                    boolean senderWithAffinity = (QSignalEmitterInternal.this.thread() == Thread.currentThread()
                        && QSignalEmitterInternal.this.thread() == thread());
                    if (senderWithAffinity &&
                            ((c.isDirectConnection() && receiverWithAffinity)
                            || (c.isAutoConnection() && notEmitterFlag)
                            || (c.isAutoConnection() && receiverWithAffinity))) {
                        // signalsBlocked() is not thread-safe but we checked above that the current
                        //  thread has affinity with sender.
                        if (QSignalEmitterInternal.this.signalsBlocked())
                            continue;

                        QSignalEmitterInternal oldEmitter = currentSender.get();
                        currentSender.set(QSignalEmitterInternal.this);
                        try {
                            boolean updateSender;
                            long oldSender = 0;
                            if(lambdaOwner!=null){
                            	updateSender = lambdaOwner instanceof QObject && QSignalEmitterInternal.this instanceof QObject;
                                if (updateSender) {
                                    oldSender = QtJambiInternal.setQObjectSender(((QObject) lambdaOwner).nativeId(),
                                                                                 ((QObject) QSignalEmitterInternal.this).nativeId());
                                }
                            }else{
                            	updateSender = receiver instanceof QObject && QSignalEmitterInternal.this instanceof QObject;
                                if (updateSender) {
                                    oldSender = QtJambiInternal.setQObjectSender(((QObject) receiver).nativeId(),
                                                                                 ((QObject) QSignalEmitterInternal.this).nativeId());
                                }
                            }

                            if(c instanceof ReflectiveConnection){
                            	ReflectiveConnection methodConnection = (ReflectiveConnection)c;
	                            try {
	                            	methodConnection.slot.invoke(receiver, c.args);
	                            } catch (IllegalAccessException e) {
	                                QtJambiInternal.invokeSlot(receiver, methodConnection.slotId, 
	                                		methodConnection.returnType, methodConnection.args, methodConnection.convertTypes);
	                            }
                            }else if(c instanceof SlotObjectConnection){
                            	switch(c.args.length){
                            	case 0:
                           			((QSignalEmitter.Slot0) receiver).invoke();
                            		break;
                            	case 1:
                           			((QSignalEmitter.Slot1) receiver).invoke(
                           					c.args[0]
                       					);
                            		break;
                            	case 2:
                           			((QSignalEmitter.Slot2) receiver).invoke(
                           					c.args[0], c.args[1]
                       					);
                            		break;
                            	case 3:
                           			((QSignalEmitter.Slot3) receiver).invoke(
                           					c.args[0], c.args[1], c.args[2]
                       					);
                            		break;
                            	case 4:
                           			((QSignalEmitter.Slot4) receiver).invoke(
                           					c.args[0], c.args[1], c.args[2], 
                           					c.args[3]
                       					);
                            		break;
                            	case 5:
                           			((QSignalEmitter.Slot5) receiver).invoke(
                           					c.args[0], c.args[1], c.args[2], 
                           					c.args[3], c.args[4]
                       					);
                            		break;
                            	case 6:
                           			((QSignalEmitter.Slot6) receiver).invoke(
                           					c.args[0], c.args[1], c.args[2], 
                           					c.args[3], c.args[4], c.args[5]
                       					);
                            		break;
                            	case 7:
                           			((QSignalEmitter.Slot7) receiver).invoke(
                           					c.args[0], c.args[1], c.args[2],
                           					c.args[3], c.args[4], c.args[5],
                           					c.args[6]
                       					);
                            		break;
                            	case 8:
                           			((QSignalEmitter.Slot8) receiver).invoke(
                           					c.args[0], c.args[1], c.args[2],
                           					c.args[3], c.args[4], c.args[5], 
                           					c.args[6], c.args[7]
                       					);
                            		break;
                            	case 9:
                           			((QSignalEmitter.Slot9) receiver).invoke(
                           					c.args[0], c.args[1], c.args[2],
                           					c.args[3], c.args[4], c.args[5], 
                           					c.args[6], c.args[7], c.args[8]
                       					);
                            		break;
                            	}
                            }

                            if (updateSender) {
                            	if(lambdaOwner!=null){
                            		QtJambiInternal.resetQObjectSender(((QObject) lambdaOwner).nativeId(),
                                            oldSender);
                            	}else{
                            		QtJambiInternal.resetQObjectSender(((QObject) receiver).nativeId(),
                                            oldSender);
                            	}
                            }

                        } catch (InvocationTargetException e) {
                            System.err.println("Exception caught after invoking slot");
                            e.getCause().printStackTrace();

                        } catch (Exception e) {
                            System.err.println("Exception caught after invoking slot:");
                            e.printStackTrace();
                        } finally {
                            currentSender.set(oldEmitter);
                        }
                    } else if(c.isDirectConnection()) {
                        // You are asking for a DirectConnection but the sender and receiver do not both
                        //  have the same affinity of the current thread.
                        //  log this state, discarded emit due to being unable to deliver it.
                        System.err.println("emit() discarded due to thread-affinity mismatch sender=" + QSignalEmitterInternal.this +
                            "; receiver=" + receiver + "; type=DirectConnection; senderWithAffinity=" + senderWithAffinity +
                            "; receiverWithAffinity=" + receiverWithAffinity);
                    } else {
                        // Otherwise we marshal the delivery via the event system that allows it to cross-thread.
                        QObject sender = null;
                        if((receiver instanceof QObject || lambdaOwner instanceof QObject) && QSignalEmitterInternal.this instanceof QObject) {
                            sender = (QObject) QSignalEmitterInternal.this;
                        }

                        QtJambiInternal.QMetaCallEvent event = new QtJambiInternal.QMetaCallEvent(c, sender, c.args);
                        QObject eventReceiver = null;
                        if (lambdaOwner instanceof QObject)
                            eventReceiver = (QObject) lambdaOwner;
                        else if (receiver instanceof QObject)
                            eventReceiver = (QObject) receiver;
                        else
                            eventReceiver = QCoreApplication.instance();

                        QCoreApplication.postEvent(eventReceiver, event);
                    }
                }

                // Remove the ones marked for removal..
                removeConnection_helper(toRemove);

            } finally {
                inJavaEmission = false;
            }
        }

        private boolean matchSlot(Method slot) {
            Class<?> slotArguments[] = slot.getParameterTypes();
            Class<?> signalArguments[] = resolveSignal();
            int signalArrayDims[] = arrayDimensions();

            if (slotArguments.length > signalArguments.length) {
                return false;
            }

            for (int i = 0; i < slotArguments.length; ++i) {
                if (!matchTwoTypes(slotArguments[i],
                                   signalArguments[i],
                                   signalArrayDims[i])) {
                    return false;
                }
            }

            return true;
        }

        private boolean matchTwoTypes(Class<?> slotArgument, Class<?> signalArgument, int signalArrayDims) {
            return matchTwoTypes(slotArgument, signalArgument, signalArrayDims, false);
        }

        private boolean matchTwoTypes(Class<?> slotArgument,
                                      Class<?> signalArgument,
                                      int signalArrayDims,
                                      boolean wasArray) {

            if (slotArgument.isArray() || signalArrayDims < 0) {
                int slotArrayDims = 0;
                while (slotArgument.isArray()) {
                    slotArgument = slotArgument.getComponentType();
                    ++slotArrayDims;
                }
                // JRE7 returns Class as the top level item, but isArray() is set (for each dimension, hence we loop)
                // JRE5/JRE6 returns GenericArrayTypeImpl for each dimension and the final unwrapping returns a Class with isArray() not set
                // So this while loop below is needed since JRE7, the purpose is not really to count but to unwrap.
                // This section of code is commented out because it is not needed because we unwrap in
                //  QtJambiInternal#resolveSignal() and use the keep the result.
                //if (signalArgument.isArray()) {
                //    int newSignalArrayDims = 0;
                //    while (signalArgument.isArray()) {
                //        newSignalArrayDims++;
                //        signalArgument = signalArgument.getComponentType();
                //    }
                //    if(signalArrayDims >= 0 && newSignalArrayDims != signalArrayDims)
                //        return false;
                //}
                return slotArrayDims == signalArrayDims && matchTwoTypes(slotArgument, signalArgument, 0, true);
            } else if (slotArgument.isPrimitive() && !wasArray) {
                return matchTwoTypes(QtJambiInternal.getComplexType(slotArgument),
                        signalArgument, signalArrayDims);
            } else if (!slotArgument.isAssignableFrom(signalArgument)) {
                return false;
            }

            return true;
        }

        private synchronized void addConnection(Object receiver, Method slot,
                int connectionType) {

            if (!connectedToCpp) {
                connectedToCpp = true;
                __qt_signalInitialization(name());
            }

            Class<?> returnType = slot.getReturnType();
            byte returnSig;
            if (!returnType.isPrimitive())
                returnSig = 'L';
            else
                returnSig = org.qtjambi.qt.internal.QtJambiInternal.primitiveToByte(returnType);

            try {
                slot.setAccessible(true);
            } catch (SecurityException e) {
                // We don't care about the exception, as we'll use a fall back
                // if the slot turns out to be inaccessible
            }

            List<Connection> newList = cloneConnectionsForceInstance();
           	newList.add(new ReflectiveConnection(receiver, slot, returnSig,(byte) connectionType));
            connections = newList;
        }
        
        private synchronized void addConnectionToSlotObject(Object slotObject, int connectionType) {

            if (!connectedToCpp) {
                connectedToCpp = true;
                __qt_signalInitialization(name());
            }

            Object lambdaOwner = null;
            Class<?> lambdaOwnerClass = null;
            if(slotObject.getClass().isSynthetic()){
            	String className = slotObject.getClass().getName();
                if(className.contains("Lambda$") && className.contains("/")){
                	String[] split = className.split("Lambda\\$");
                	if(split.length>0){
                		while(split[0].endsWith("$") || split[0].endsWith(".")){
                			split[0] = split[0].substring(0, split[0].length()-1);
                		}
                		try {
                			lambdaOwnerClass = Class.forName(split[0]);
            			} catch (Throwable e) {
            			}
                	}
	                try {
						Field arg$1Field = slotObject.getClass().getDeclaredField("arg$1");
						arg$1Field.setAccessible(true);
						lambdaOwner = arg$1Field.get(slotObject);
					} catch (Throwable e) {
					}
	                if(lambdaOwner==null){
	                	lambdaOwner = lambdaOwnerClass;
	                }
	                if(lambdaOwner instanceof QSignalEmitterInternal){
	                	QSignalEmitterInternal sLambdaOwner = (QSignalEmitterInternal)lambdaOwner;
	                	if(sLambdaOwner.__qt_lambdaExpressions==null){
	                		sLambdaOwner.__qt_lambdaExpressions = new ArrayList<Object>();
	                	}
	                	sLambdaOwner.__qt_lambdaExpressions.add(slotObject);
	                }else{
	                	connectionType |= ReflectiveConnection.HARD_REFERENCE_CONNECTION;
	                }
            	}
            }

            List<Connection> newList = cloneConnectionsForceInstance();
           	newList.add(new SlotObjectConnection(lambdaOwner, lambdaOwnerClass, slotObject,(byte) connectionType));
            connections = newList;
        }

        private List<Connection> cloneConnectionsForceInstance() {
            List<Connection> newList = new ArrayList<Connection>();
            if (connections != null)
                newList.addAll(connections);
            return newList;
        }

        private List<Connection> cloneConnections() {
            if (connections == null)
                return null;
            List<Connection> newList = new ArrayList<Connection>();
            newList.addAll(connections);
            return newList;
        }

        protected synchronized boolean removeConnection(Object receiver, Method slot) {
            if (inDisconnect)
                return false;  // Why might we re-enter ?  Why are we not reentrant ?
            inDisconnect = true;

            if (!connectedToCpp) {
                // We seem to initialize only to possibly perform cppDisconnect later in the method
                connectedToCpp = true;
                __qt_signalInitialization(name());
            }

            boolean returned = false;
            if (connections != null) {
                List<Connection> toRemove = null;
                for (Connection c : connections) {
                    Object resolvedReceiver = c.resolveReceiver();
                    if(c instanceof ReflectiveConnection){
	                    if (resolvedReceiver == null) {  // GCed receiver
	                        if (toRemove == null)
	                            toRemove = new ArrayList<Connection>();
	                        toRemove.add(c);
	                    } else if ((receiver == null || resolvedReceiver == receiver)
	                        && (slot == null || slot.equals(((ReflectiveConnection)c).slot))) {
	                        if (toRemove == null)
	                            toRemove = new ArrayList<Connection>();
	                        toRemove.add(c);
	                        returned = true;
	                    }
                    }else if(c instanceof SlotObjectConnection){
                    	SlotObjectConnection slotObjectConnection = (SlotObjectConnection)c;
                    	Object lambdaOwner = slotObjectConnection.lambdaOwner();
                        Class<?> lambdaOwnerClass = slotObjectConnection.lambdaOwnerClass;
                        if(lambdaOwner!=null || lambdaOwnerClass!=null){
                        	if(receiver != null && slot == null){
                        		if(lambdaOwner == receiver || 
                        				lambdaOwnerClass == receiver){
                        			if (toRemove == null)
        	                            toRemove = new ArrayList<Connection>();
        	                        toRemove.add(c);
        	                        returned = true;
                        		}
                        	}else if(receiver == null && slot == null){
                    			if (toRemove == null)
    	                            toRemove = new ArrayList<Connection>();
    	                        toRemove.add(c);
    	                        returned = true;
                    		}else{
                        		// remove if lambda owner has been disposed
                        		if(lambdaOwner instanceof QtJambiObject && ((QtJambiObject)lambdaOwner).nativeId()==0){
                        			if (toRemove == null)
        	                            toRemove = new ArrayList<Connection>();
        	                        toRemove.add(c);
                        		}
                        	}
                        }else if(resolvedReceiver == receiver){
                        	if (toRemove == null)
	                            toRemove = new ArrayList<Connection>();
	                        toRemove.add(c);
	                        returned = true;
                        }
                    }
                }

                removeConnection_helper(toRemove);
            }

            if (QSignalEmitterInternal.this instanceof QObject && (receiver instanceof QObject || receiver == null)) {
                String methodSignature = null;
                if (slot != null) {
                    methodSignature = slot.toString();
                    int paren_pos = methodSignature.indexOf('(');
                    methodSignature = methodSignature.substring(methodSignature.lastIndexOf(' ', paren_pos) + 1);
                }
                returned |= org.qtjambi.qt.internal.QtJambiInternal.cppDisconnect((QObject) QSignalEmitterInternal.this,
                        this.fullName(), (QObject) receiver, methodSignature);
                if (receiver == null && slot == null)
                    connectedToCpp = false;
            }


            inDisconnect = false;
            return returned;
        }

        private void removeConnection_helper(List<Connection> toRemove) {
            if (toRemove != null) {
                List<Connection> newList = cloneConnections();
                for (Connection c : toRemove){
                	newList.remove(c);
                    if(c instanceof SlotObjectConnection){
                    	Object lambdaOwner = ((SlotObjectConnection) c).lambdaOwner();
                    	if(lambdaOwner instanceof QSignalEmitterInternal){
    	                	QSignalEmitterInternal sLambdaOwner = (QSignalEmitterInternal)lambdaOwner;
    	                	if(sLambdaOwner.__qt_lambdaExpressions!=null){
    	                		sLambdaOwner.__qt_lambdaExpressions.remove(c.resolveReceiver());
    	                		if(sLambdaOwner.__qt_lambdaExpressions.isEmpty()){
    	                			sLambdaOwner.__qt_lambdaExpressions = null;
    	                		}
    	                	}
    	                }
                    }
                }
                connections = newList;
            }
        }
    }

    /**
     * @exclude
     */
    protected boolean __qt_signalInitialization(String name) {
        return false;
    }

    public abstract boolean signalsBlocked();

    public abstract Thread thread();

    protected static final ThreadLocal<QSignalEmitterInternal> currentSender = new ThreadLocal<QSignalEmitterInternal>();

    /**
     * Paranoid cleanup method for the current Thread's ThreadLocal data.
     */
    static void currentSenderRemove() {
        currentSender.remove();
    }
    
    private ArrayList<Object> __qt_lambdaExpressions;
}
