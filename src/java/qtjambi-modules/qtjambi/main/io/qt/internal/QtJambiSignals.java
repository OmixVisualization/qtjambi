/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
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
package io.qt.internal;

import java.io.Serializable;
import java.lang.invoke.MethodHandle;
import java.lang.invoke.MethodHandles;
import java.lang.invoke.SerializedLambda;
import java.lang.ref.Reference;
import java.lang.ref.WeakReference;
import java.lang.reflect.AnnotatedType;
import java.lang.reflect.Field;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.lang.reflect.Modifier;
import java.lang.reflect.Type;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.Collections;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.Objects;
import java.util.function.Function;
import java.util.function.Supplier;

import io.qt.InternalAccess;
import io.qt.NativeAccess;
import io.qt.QMisfittingSignatureException;
import io.qt.QNoSuchSignalException;
import io.qt.QNoSuchSlotException;
import io.qt.QSignalDeclarationException;
import io.qt.QSignalInitializationException;
import io.qt.QSignalInvocationException;
import io.qt.QUninvokableSlotException;
import io.qt.QtObject;
import io.qt.QtObjectInterface;
import io.qt.QtSignalBlockerInterface;
import io.qt.QtSignalEmitterInterface;
import io.qt.QtThreadAffineInterface;
import io.qt.QtUninvokable;
import io.qt.core.QDeclarableSignals;
import io.qt.core.QInstanceMemberSignals;
import io.qt.core.QMetaMethod;
import io.qt.core.QMetaObject;
import io.qt.core.QMetaObject.Connection;
import io.qt.core.QMetaObject.GenericSlot;
import io.qt.core.QMetaObject.Slot0;
import io.qt.core.QMetaObject.Slot1;
import io.qt.core.QMetaObject.Slot2;
import io.qt.core.QMetaObject.Slot3;
import io.qt.core.QMetaObject.Slot4;
import io.qt.core.QMetaObject.Slot5;
import io.qt.core.QMetaObject.Slot6;
import io.qt.core.QMetaObject.Slot7;
import io.qt.core.QMetaObject.Slot8;
import io.qt.core.QMetaObject.Slot9;
import io.qt.core.QMetaProperty;
import io.qt.core.QObject;
import io.qt.core.QPair;
import io.qt.core.QStaticMemberSignals;
import io.qt.core.QThread;
import io.qt.core.Qt;
import io.qt.core.Qt.ConnectionType;

public abstract class QtJambiSignals {
	
	private final static java.util.logging.Logger logger = java.util.logging.Logger.getLogger("io.qt.internal");
	
	private static final Map<Integer, Method> lookupSlots = Collections.synchronizedMap(new HashMap<>());

	private static final byte InCppEmission     = 0x01;
	private static final byte InJavaEmission    = 0x02;
	private static final byte InDisconnect      = 0x04;
	private static final byte IsNativeSignal    = 0x08;
	private static final byte IsStaticSignal    = 0x10;
	private static final byte IsQObjectSignal   = 0x20;
	
	static final class SignalParameterType{
		public SignalParameterType(Class<?> originalType, Class<?> type, Type genericType, AnnotatedType annotatedType, int arrayDimension, boolean isPointer, boolean isReference) {
			super();
			this.originalType = originalType;
			this.genericType = genericType;
			this.type = type;
			this.annotatedType = annotatedType;
			this.arrayDimension = arrayDimension;
			this.isPointer = isPointer;
			this.isReference = isReference;
		}
		
		final Class<?> originalType;
		final Class<?> type;
		final Type genericType;
		final AnnotatedType annotatedType;
		final int arrayDimension;
		final boolean isPointer;
		final boolean isReference;
		
		@Override
		public boolean equals(Object obj) {
			if (this == obj)
				return true;
			if (obj instanceof SignalParameterType) {
				SignalParameterType other = (SignalParameterType) obj;
				if(arrayDimension == other.arrayDimension) {
					return type==other.type && isPointer==other.isPointer && isReference==other.isReference;
				}
				return false;
			}
			return false;
		}
		
		@Override
		public int hashCode() {
			final int prime = 31;
			int result = 1;
			result = prime * result + arrayDimension;
			result = prime * result + ((type == null) ? 0 : type.getName().hashCode());
			result = prime * result + (isPointer ? 0 : 1);
			result = prime * result + (isReference ? 0 : 1);
			return result;
		}
	}

	private interface SlotConnectionFactory<Slot>{
    	public SlotObjectConnection<Slot> create(Object lambdaOwner, Class<?> lambdaOwnerClass, int lambdaHashCode, Slot slotObject, Qt.ConnectionType[] connectionType);
    }
	
	private static abstract class AbstractSignalCore {
		abstract List<SignalParameterType> signalTypes(AbstractSignal signal);
		abstract String name(AbstractSignal signal);
		abstract void emitSignal(AbstractSignal signal, final Object ... args);
		abstract QMetaObject.Connection addConnectionToMethod(AbstractSignal signal, Object receiver, Method slot, MethodHandle slotHandle, List<Supplier<?>> lambdaArgs, boolean unsolvableConnection, boolean allowNativeConnect, Qt.ConnectionType... connectionType);
		abstract QMetaObject.Connection addConnectionToMethod(AbstractSignal signal, Object receiver, QMetaMethod slot, Qt.ConnectionType... connectionType);
		abstract <Slot extends Serializable> QMetaObject.Connection addConnectionToSlotObject(AbstractSignal signal, SlotConnectionFactory<Slot> factory, Object lambdaOwner, Class<?> lambdaOwnerClass, int lambdaHash, Slot slotObject, Qt.ConnectionType[] connectionType);
		abstract boolean removeConnectionToMethod(AbstractSignal signal, Object receiver, Method slot, Object[] lambdaArgs, boolean allowNativeDisconnect);
		abstract boolean removeConnectionToMethod(AbstractSignal signal, Object receiver, QMetaMethod slot, boolean allowNativeDisconnect);
		int methodIndex(AbstractSignal signal) { return -1; }
		abstract boolean removeConnectionToSlotObject(AbstractSignal signal,
				Serializable slotObject, Class<?> _functionalInterfaceClass, Object _lambdaOwner, Class<?> _lambdaOwnerClass,
				int _lambdaHashCode, boolean useLambdaInfo);
		abstract boolean removeConnection(AbstractSignal signal, JavaConnection javaConnection);
		boolean removeConnection(AbstractSignal signal, NativeConnection nativeConnection) {
			if(nativeConnection.receiver()!=null && nativeConnection.method()!=null && !nativeConnection.isSlotObject()) {
				return QObject.disconnect(nativeConnection.sender(), nativeConnection.signal(), nativeConnection.receiver(), nativeConnection.method());
			}else {
				long nativeId = QtJambiInternal.internalAccess.nativeId(nativeConnection);
				if(nativeId!=0)
					return disconnectConnection(nativeId);
				else return false;
			}
		}
	}
	
	private interface AnalyzingSignalCoreInterface{}
	
	private static class AnalyzingSignalCore extends AbstractSignalCore implements AnalyzingSignalCoreInterface{
		final static AnalyzingSignalCore instance = new AnalyzingSignalCore();
		
		@Override
		List<SignalParameterType> signalTypes(AbstractSignal signal) {
			signal.resolveSignal();
			return signal.core.signalTypes(signal);
		}

		@Override
		String name(AbstractSignal signal) {
			signal.resolveSignal();
			return signal.core.name(signal);
		}

		@Override
		Connection addConnectionToMethod(AbstractSignal signal, Object receiver, Method slot, MethodHandle slotHandle, 
				List<Supplier<?>> lambdaArgs, boolean unsolvableConnection, boolean allowNativeConnect, ConnectionType... connectionType) {
			signal.resolveSignal();
			return signal.core.addConnectionToMethod(signal, receiver, slot, slotHandle, lambdaArgs, unsolvableConnection, allowNativeConnect, connectionType);
		}
		
		@Override
		QMetaObject.Connection addConnectionToMethod(AbstractSignal signal, Object receiver, QMetaMethod slot, Qt.ConnectionType... connectionType){
			signal.resolveSignal();
			return signal.core.addConnectionToMethod(signal, receiver, slot, connectionType);			
		}

		@Override
		void emitSignal(AbstractSignal signal, Object... args) {
			signal.resolveSignal();
			signal.core.emitSignal(signal, args);
		}

		@Override
		<Slot extends Serializable> Connection addConnectionToSlotObject(AbstractSignal signal,
				SlotConnectionFactory<Slot> factory, Object lambdaOwner, Class<?> lambdaOwnerClass, int lambdaHash, Slot slotObject, ConnectionType[] connectionType) {
			signal.resolveSignal();
			return signal.core.addConnectionToSlotObject(signal, factory, lambdaOwner, lambdaOwnerClass, lambdaHash, slotObject, connectionType);
		}
		
		int methodIndex(AbstractSignal signal) { 
			signal.resolveSignal();
			return signal.core.methodIndex(signal);
		}
		
		@Override
		boolean removeConnectionToMethod(AbstractSignal signal, Object receiver, Method slot, Object[] lambdaArgs,
				boolean allowNativeDisconnect) {
			signal.resolveSignal();
			return signal.core.removeConnectionToMethod(signal, receiver, slot, lambdaArgs, allowNativeDisconnect);
		}
		
		@Override
		boolean removeConnectionToMethod(AbstractSignal signal, Object receiver, QMetaMethod slot, boolean allowNativeDisconnect) {
			signal.resolveSignal();
			return signal.core.removeConnectionToMethod(signal, receiver, slot, allowNativeDisconnect);
		}

		@Override
		boolean removeConnectionToSlotObject(AbstractSignal signal,
				Serializable slotObject, Class<?> _functionalInterfaceClass, Object _lambdaOwner, Class<?> _lambdaOwnerClass,
				int _lambdaHashCode, boolean useLambdaInfo) {
			signal.resolveSignal();
			return signal.core.removeConnectionToSlotObject(signal, slotObject, _functionalInterfaceClass, _lambdaOwner, _lambdaOwnerClass, _lambdaHashCode, useLambdaInfo);
		}
		
		@Override
		boolean removeConnection(AbstractSignal signal, JavaConnection javaConnection) {
			signal.resolveSignal();
			return signal.core.removeConnection(signal, javaConnection);
		}
		
		@Override
		boolean removeConnection(AbstractSignal signal, NativeConnection nativeConnection) {
			signal.resolveSignal();
			return signal.core.removeConnection(signal, nativeConnection);
		}
	}
	
	private abstract static class AbstractParameterSignalCore extends AbstractSignalCore {
    	private final List<SignalParameterType> signalParameterTypes;
    	
		public AbstractParameterSignalCore(List<SignalParameterType> signalParameterTypes) {
			super();
			this.signalParameterTypes = signalParameterTypes;
		}

		@Override
		final List<SignalParameterType> signalTypes(AbstractSignal signal) {
			return signalParameterTypes;
		}		
	}
	
	private abstract static class AbstractQObjectsSignalCore extends AbstractParameterSignalCore {
		
		private static class ReceiverReference extends WeakReference<Object> implements InternalAccess.Cleanable{
			private InternalAccess.Cleanable cleanable;

			public ReceiverReference(Object referent) {
				super(referent, QtJambiInternal.referenceQueue);
			}

			public void clean() {
				if(cleanable!=null)
					cleanable.clean();
			}
			
			public void setCleanable(InternalAccess.Cleanable cleanable) {
				this.cleanable = cleanable;
			}
		}

		private final List<QPair<AbstractConnection<?>,QMetaObject.Connection>> nativeConnectionHandles = new LinkedList<>();
    	
    	public AbstractQObjectsSignalCore(List<SignalParameterType> signalParameterTypes, int methodIndex) {
			super(signalParameterTypes);
			this.methodIndex = methodIndex;
		}

		final int methodIndex;
		
		@Override
		final String name(AbstractSignal signal) {
			if(signal.containingObject() instanceof QObject && methodIndex>=0) {
				QMetaMethod method = QMetaMethod.fromSignal((QMetaObject.AbstractSignal)signal);
				if(method!=null)
					return method.name().toString();
			}
			return null;
		}
		
		@Override
		final int methodIndex(AbstractSignal signal) { return methodIndex; }

		@Override
		Connection addConnectionToMethod(AbstractSignal signal, Object receiver, QMetaMethod slot,
				ConnectionType... connectionType) {
			QObject senderObject = (QObject)signal.containingObject();
			if(receiver instanceof QObject) {
				int flags = 0;
	            if(connectionType!=null && connectionType.length>0) {
	        		for (Qt.ConnectionType ct : connectionType) {
	        			flags |= ct.value();
					}
	        	}
	            try {
    				return connectNativeToMetaMethod(QtJambiInternal.internalAccess.checkedNativeId(senderObject), methodIndex, QtJambiInternal.internalAccess.checkedNativeId((QObject)receiver), slot.methodIndex(), flags);
    			} catch (QMisfittingSignatureException e) {
    			}
			}else {
				ReceiverReference reference = receiver==null ? null : new ReceiverReference(receiver);
				MetaMethodConnection c = new MetaMethodConnection(reference, slot, connectionType);
				QMetaObject.Connection connection;
				int flags = 0;
	            if(connectionType!=null && connectionType.length>0) {
	        		for (Qt.ConnectionType ct : connectionType) {
	        			flags |= ct.value();
					}
	        	}
    			connection = connectNative(QtJambiInternal.internalAccess.checkedNativeId(senderObject), methodIndex, c, slot.parameterTypes().size(), flags);
				if(connection!=null) {
					QPair<AbstractConnection<?>,QMetaObject.Connection> pair = new QPair<>(c, connection);
					synchronized(nativeConnectionHandles) {
						nativeConnectionHandles.add(pair);
					}
					if(reference!=null) {
						List<QPair<AbstractConnection<?>,QMetaObject.Connection>> nativeConnectionHandles = this.nativeConnectionHandles;
						reference.setCleanable(()->{
							disconnectOne(pair.second);
							synchronized (nativeConnectionHandles) {
								nativeConnectionHandles.remove(pair);
							}
						});
					}
				}
				return connection;
			}
			return null;
		}

		@Override
		Connection addConnectionToMethod(AbstractSignal signal, Object receiver, Method slot, MethodHandle slotHandle, 
				List<Supplier<?>> lambdaArgs, boolean unsolvableConnection, boolean allowNativeConnect, ConnectionType... connectionType) {
			QObject senderObject = (QObject)signal.containingObject();
    		if((lambdaArgs==null || lambdaArgs.isEmpty()) 
    		    && (receiver instanceof QObject || 
					( receiver instanceof QMetaObject.AbstractSignal 
						&& slot.getName().equals("emit") 
						&& ((AbstractSignal)receiver).methodIndex()>=0
						&& ((AbstractSignal)receiver).containingObject() instanceof QObject) )) {
        		QObject receiverObject;
        		int slotMethod = -1;
        		if(receiver instanceof QObject) {
        			receiverObject = (QObject)receiver;
    				slotMethod = QMetaMethod.fromReflectedMethod(slot).methodIndex();
        		}else {
        			AbstractSignal otherSignal = (AbstractSignal)receiver;
        			receiverObject = (QObject)otherSignal.containingObject();
    				slotMethod = otherSignal.methodIndex();
        		}
        		if(slotMethod>=0) {
        			int flags = 0;
    	            if(connectionType!=null && connectionType.length>0) {
    	        		for (Qt.ConnectionType ct : connectionType) {
    	        			flags |= ct.value();
    					}
    	        	}
        			try{
        				return connectNativeToMetaMethod(QtJambiInternal.internalAccess.checkedNativeId(senderObject), methodIndex, QtJambiInternal.internalAccess.checkedNativeId(receiverObject), slotMethod, flags);
        			} catch (QMisfittingSignatureException e) {
        			}
        		}
    		}
    		
			if(slotHandle==null){
				try{
					slotHandle = QtJambiInternal.privateLookup(slot.getDeclaringClass()).unreflect(slot);
					if(slotHandle.isVarargsCollector()) {
						slotHandle = slotHandle.asFixedArity();
					}
				} catch (Throwable e2) {
					logger.log(java.util.logging.Level.WARNING, "Exception caught while analyzing slot "+slot, e2);
					return null;
				}
			}
			ReceiverReference reference = receiver==null ? null : new ReceiverReference(receiver);
			ReflectiveConnection c = new ReflectiveConnection(reference, slot, slotHandle, lambdaArgs, connectionType);
			int flags = 0;
            if(connectionType!=null && connectionType.length>0) {
        		for (Qt.ConnectionType ct : connectionType) {
        			flags |= ct.value();
				}
        	}
            QMetaObject.Connection connection = connectNative(QtJambiInternal.internalAccess.checkedNativeId(senderObject), methodIndex, c, slot.getParameterCount()-(lambdaArgs==null ? 0 : lambdaArgs.size()), flags);
			if(connection!=null) {
				QPair<AbstractConnection<?>,QMetaObject.Connection> pair = new QPair<>(c, connection);
				synchronized(nativeConnectionHandles) {
					nativeConnectionHandles.add(pair);
				}
				if(reference!=null) {
					List<QPair<AbstractConnection<?>,QMetaObject.Connection>> nativeConnectionHandles = this.nativeConnectionHandles;
					reference.setCleanable(()->{
						disconnectOne(pair.second);
						synchronized (nativeConnectionHandles) {
							nativeConnectionHandles.remove(pair);
						}
					});
				}
			}
			return connection;
		}
		
		@Override
		<Slot extends Serializable> Connection addConnectionToSlotObject(AbstractSignal signal,
				SlotConnectionFactory<Slot> factory, Object lambdaOwner, Class<?> lambdaOwnerClass, int lambdaHashCode, Slot slotObject, ConnectionType[] connectionType) {
    		QObject senderObject = (QObject)signal.containingObject();
    		QMetaObject.Connection connection;
			SlotObjectConnection<Slot> c = factory.create(lambdaOwner, lambdaOwnerClass, lambdaHashCode, slotObject, connectionType);
			int flags = 0;
            if(connectionType!=null && connectionType.length>0) {
        		for (Qt.ConnectionType ct : connectionType) {
        			flags |= ct.value();
				}
        	}
			connection = connectNative(QtJambiInternal.internalAccess.checkedNativeId(senderObject), methodIndex, c, -1, flags);
			if(connection!=null) {
				synchronized(nativeConnectionHandles) {
					nativeConnectionHandles.add(new QPair<>(c, connection));
				}
			}
			return connection;
		}
		
		@Override
		boolean removeConnectionToMethod(AbstractSignal signal, Object receiver, QMetaMethod slot, boolean allowNativeDisconnect) {
			QObject senderObject = (QObject)signal.containingObject();
			if(senderObject==null || senderObject.isDisposed())
				return false;
			boolean success = false;
			if((receiver == null || receiver instanceof QObject) && slot == null) {
				success |= disconnectNative(QtJambiInternal.internalAccess.checkedNativeId(senderObject), methodIndex, QtJambiInternal.internalAccess.nativeId((QObject)receiver), -1);
			}
			synchronized(nativeConnectionHandles) {
				List<QPair<AbstractConnection<?>, Connection>> foundPairs = null;
				for(QPair<AbstractConnection<?>, Connection> pair : nativeConnectionHandles) {
					if(pair.first instanceof MetaMethodConnection) {
						MetaMethodConnection rc = (MetaMethodConnection)pair.first;
						if(receiver == null) {
							if(slot == null) {
								if(foundPairs==null) {
									foundPairs = Collections.singletonList(pair);
								}else {
									if(foundPairs.size()==1){
										foundPairs = new ArrayList<>(foundPairs);
									}
									foundPairs.add(pair);
								}
							}else if(rc.slot.equals(slot)){
								if(foundPairs==null) {
									foundPairs = Collections.singletonList(pair);
								}else {
									if(foundPairs.size()==1){
										foundPairs = new ArrayList<>(foundPairs);
									}
									foundPairs.add(pair);
								}
							}
						}else if(rc.resolveReceiver() == receiver){
							if(slot == null) {
								if(foundPairs==null) {
									foundPairs = Collections.singletonList(pair);
								}else {
									if(foundPairs.size()==1){
										foundPairs = new ArrayList<>(foundPairs);
									}
									foundPairs.add(pair);
								}
							}else if(rc.slot.equals(slot)){
								if(foundPairs==null) {
									foundPairs = Collections.singletonList(pair);
								}else {
									if(foundPairs.size()==1){
										foundPairs = new ArrayList<>(foundPairs);
									}
									foundPairs.add(pair);
								}
							}
						}
					}
				}
				if(foundPairs!=null) {
					for (QPair<AbstractConnection<?>, Connection> qPair : foundPairs) {
						if(QObject.disconnect(qPair.second)) {
							success = true;
							synchronized(nativeConnectionHandles) {
								nativeConnectionHandles.remove(qPair);
							}
						}
					}
				}
			}
			return success;
		}
		
		@Override
		boolean removeConnectionToMethod(AbstractSignal signal, Object receiver, Method slot, Object[] lambdaArgs, boolean allowNativeDisconnect) {
			QObject senderObject = (QObject)signal.containingObject();
			if(senderObject==null || senderObject.isDisposed())
				return false;
			boolean success = false;
			if((receiver == null || receiver instanceof QObject) && slot == null) {
				success |= disconnectNative(QtJambiInternal.internalAccess.checkedNativeId(senderObject), methodIndex, QtJambiInternal.internalAccess.nativeId((QObject)receiver), -1);
			}
			synchronized(nativeConnectionHandles) {
				List<QPair<AbstractConnection<?>, Connection>> foundPairs = null;
				for(QPair<AbstractConnection<?>, Connection> pair : nativeConnectionHandles) {
					if(pair.first instanceof AbstractReflectiveConnection) {
						AbstractReflectiveConnection<?> rc = (AbstractReflectiveConnection<?>)pair.first;
						if(receiver == null) {
							if(slot == null) {
								if(foundPairs==null) {
									foundPairs = Collections.singletonList(pair);
								}else {
									if(foundPairs.size()==1){
										foundPairs = new ArrayList<>(foundPairs);
									}
									foundPairs.add(pair);
								}
							}else if(rc.slot.equals(slot)){
								boolean isLambdaArgsEquals = true;
								if(rc.lambdaArgs!=null && lambdaArgs!=null) {
									for (int i = 0; i < lambdaArgs.length; i++) {
										Supplier<?> arg = rc.lambdaArgs.get(i);
										if(!Objects.equals(arg.get(), lambdaArgs[i])) {
											isLambdaArgsEquals = false;
											break;
										}
									}
								}
								if(isLambdaArgsEquals) {
									if(foundPairs==null) {
										foundPairs = Collections.singletonList(pair);
									}else {
										if(foundPairs.size()==1){
											foundPairs = new ArrayList<>(foundPairs);
										}
										foundPairs.add(pair);
									}
								}
							}
						}else if(rc.resolveReceiver() == receiver){
							if(slot == null) {
								if(foundPairs==null) {
									foundPairs = Collections.singletonList(pair);
								}else {
									if(foundPairs.size()==1){
										foundPairs = new ArrayList<>(foundPairs);
									}
									foundPairs.add(pair);
								}
							}else if(rc.slot.equals(slot)){
								boolean isLambdaArgsEquals = true;
								if(rc.lambdaArgs!=null && lambdaArgs!=null) {
									for (int i = 0; i < lambdaArgs.length; i++) {
										Supplier<?> arg = rc.lambdaArgs.get(i);
										if(!Objects.equals(arg.get(), lambdaArgs[i])) {
											isLambdaArgsEquals = false;
											break;
										}
									}
								}
								if(isLambdaArgsEquals) {
									if(foundPairs==null) {
										foundPairs = Collections.singletonList(pair);
									}else {
										if(foundPairs.size()==1){
											foundPairs = new ArrayList<>(foundPairs);
										}
										foundPairs.add(pair);
									}
								}
							}
						}
					}
				}
				if(foundPairs!=null) {
					for (QPair<AbstractConnection<?>, Connection> qPair : foundPairs) {
						if(QObject.disconnect(qPair.second)) {
							success = true;
							synchronized(nativeConnectionHandles) {
								nativeConnectionHandles.remove(qPair);
							}
						}
					}
				}
			}
    		
    		if(receiver instanceof QObject || 
					( receiver instanceof QMetaObject.AbstractSignal 
						&& slot!=null 
						&& slot.getName().equals("emit") 
						&& ((AbstractSignal)receiver).methodIndex()>=0 
						&& ((AbstractSignal)receiver).containingObject() instanceof QObject) ) {
        		QObject receiverObject;
        		QMetaMethod slotMethod = null;
        		if(receiver instanceof QObject) {
        			receiverObject = (QObject)receiver;
        			if(slot!=null) {
        				slotMethod = QMetaMethod.fromReflectedMethod(slot);
        			}
        		}else {
        			AbstractSignal otherSignal = (AbstractSignal)receiver;
        			receiverObject = (QObject)otherSignal.containingObject();
	        		if(otherSignal.methodIndex()>=0) {
	        			slotMethod = QMetaMethod.fromSignal((QMetaObject.AbstractSignal)otherSignal);
	        		}
        		}
				success |= disconnectNative(QtJambiInternal.internalAccess.checkedNativeId(senderObject), methodIndex, QtJambiInternal.internalAccess.checkedNativeId(receiverObject), slotMethod==null ? -1 : slotMethod.methodIndex());
    		}
			return success;
		}
		
		boolean removeConnectionToSlotObject(AbstractSignal signal,
				Serializable slotObject, Class<?> _functionalInterfaceClass, Object _lambdaOwner, Class<?> _lambdaOwnerClass,
				int _lambdaHashCode, boolean useLambdaInfo) {
			boolean success = false;
			QPair<AbstractConnection<?>, Connection> foundPair = null;
			synchronized(nativeConnectionHandles) {
				for(QPair<AbstractConnection<?>, Connection> pair : nativeConnectionHandles) {
					if(pair.first instanceof SlotObjectConnection<?>) {
						SlotObjectConnection<?> sc = (SlotObjectConnection<?>)pair.first;
						if(sc.receiver == slotObject) {
							foundPair = pair;
							break;
						}
					}
				}
			}
			if(foundPair!=null) {
				if(QObject.disconnect(foundPair.second)) {
					synchronized(nativeConnectionHandles) {
						nativeConnectionHandles.remove(foundPair);
					}
				}
			}
			return success;
		}

		@Override
		boolean removeConnection(AbstractSignal signal, JavaConnection javaConnection) {
			return false;
		};
		
		@Override
		boolean removeConnection(AbstractSignal signal, NativeConnection nativeConnection) {
			if(super.removeConnection(signal, nativeConnection)) {
				synchronized(nativeConnectionHandles) {
					QPair<AbstractConnection<?>, Connection> foundPair = null;
					for(QPair<AbstractConnection<?>, Connection> pair : nativeConnectionHandles) {
						if(pair.second.equals(nativeConnection)) {
							foundPair = pair;
							break;
						}
					}
					nativeConnectionHandles.remove(foundPair);
				}
				return true;
			}
			return false;
		}
	}
	
	private static class QObjectsDirectSignalCore extends AbstractQObjectsSignalCore {
		private final EternalConnection directConnection;

		public QObjectsDirectSignalCore(List<SignalParameterType> signalParameterTypes, int methodIndex, EternalConnection directConnection) {
			super(signalParameterTypes, methodIndex);
			this.directConnection = directConnection;
		}

		@Override
		void emitSignal(AbstractSignal signal, Object... args) {
			try{
				directConnection.invoke(args);
            } catch (RuntimeException | Error e) {
            	throw e;
            } catch (Throwable e) {
                throw new QSignalInvocationException(String.format("Exception caught in signal %1$s after invoking slot %2$s: sender=%3$s, receiver=%4$s", 
						signal.fullName(), 
						directConnection.slot,
						signal.containingObject() != null ? signal.containingObject().getClass().getName() : "N/A",
								directConnection.receiver==null ? "(static)" : directConnection.receiver
					),e);
            }
		}
	}
	
	private static class QObjectsPrivateSignalCore extends AbstractQObjectsSignalCore {
		public QObjectsPrivateSignalCore(List<SignalParameterType> signalParameterTypes, int methodIndex) {
			super(signalParameterTypes, methodIndex);
		}

		@Override
		void emitSignal(AbstractSignal signal, Object... args) {
		}
	}
	
	private static class QObjectsSignalCore extends QObjectsPrivateSignalCore {
		public QObjectsSignalCore(List<SignalParameterType> signalParameterTypes, int methodIndex) {
			super(signalParameterTypes, methodIndex);
		}

		@Override
		void emitSignal(AbstractSignal signal, Object... args) {
			QObject senderObject = (QObject)signal.containingObject();
            // When you dispose() a QObject, the first thing that happens is
            // that the native ID is set to 0. Then we proceed to delete the
            // native object. If this has a destructor which emits signals,
            // they will be passed into Java, which will lead to a threadCheck()
            // from signalsBlocked, and you will get an exception. We cannot
            // support listening to signals emitted in the C++ destructor.
            if (!signal.inCppEmission()) {
            	logger.finest(()->String.format("Emit native signal %1$s(%2$s)", signal.fullName(), signal.signalParameters()));
            	if(this.methodIndex>=0) {
        			emitNativeSignal(senderObject, this.methodIndex, args);
            	}else {
            		throw new RuntimeException("QObject signal without method index"); 
            	}
            }
		}
	}
	
	private static class NamelessJavaSignalCore extends AbstractParameterSignalCore implements AnalyzingSignalCoreInterface{

		public NamelessJavaSignalCore(List<SignalParameterType> signalParameterTypes) {
			super(signalParameterTypes);
		}

		@Override
		String name(AbstractSignal signal) {
			return "unknown";
		}
		
		int methodIndex(AbstractSignal signal) { 
			signal.resolveSignal();
			return signal.core.methodIndex(signal);
		}

		@Override
		Connection addConnectionToMethod(AbstractSignal signal, Object receiver, Method slot, MethodHandle slotHandle,
				List<Supplier<?>> lambdaArgs, boolean unsolvableConnection, boolean allowNativeConnect,
				ConnectionType... connectionType) {
			signal.resolveSignal();
			return signal.core.addConnectionToMethod(signal, receiver, slot, slotHandle, lambdaArgs, unsolvableConnection, allowNativeConnect, connectionType);
		}

		@Override
		void emitSignal(AbstractSignal signal, Object... args) {
			signal.resolveSignal();
			signal.core.emitSignal(signal, args);
		}

		@Override
		<Slot extends Serializable> Connection addConnectionToSlotObject(AbstractSignal signal,
				SlotConnectionFactory<Slot> factory, Object lambdaOwner, Class<?> lambdaOwnerClass, int lambdaHash, Slot slotObject, ConnectionType[] connectionType) {
			signal.resolveSignal();
			return signal.core.addConnectionToSlotObject(signal, factory, lambdaOwner, lambdaOwnerClass, lambdaHash, slotObject, connectionType);
		}

		@Override
		boolean removeConnectionToMethod(AbstractSignal signal, Object receiver, Method slot, Object[] lambdaArgs,
				boolean allowNativeDisconnect) {
			signal.resolveSignal();
			return signal.core.removeConnectionToMethod(signal, receiver, slot, lambdaArgs, allowNativeDisconnect);
		}
		
		@Override
		boolean removeConnectionToMethod(AbstractSignal signal, Object receiver, QMetaMethod slot, boolean allowNativeDisconnect) {
			signal.resolveSignal();
			return signal.core.removeConnectionToMethod(signal, receiver, slot, allowNativeDisconnect);
		}
		
		boolean removeConnectionToSlotObject(AbstractSignal signal,
				Serializable slotObject, Class<?> _functionalInterfaceClass, Object _lambdaOwner, Class<?> _lambdaOwnerClass,
				int _lambdaHashCode, boolean useLambdaInfo) {
			signal.resolveSignal();
			return signal.core.removeConnectionToSlotObject(signal, slotObject, _functionalInterfaceClass, _lambdaOwner, _lambdaOwnerClass, _lambdaHashCode, useLambdaInfo);
		}
		
		@Override
		boolean removeConnection(AbstractSignal signal, JavaConnection javaConnection) {
			signal.resolveSignal();
			return signal.core.removeConnection(signal, javaConnection);
		}
		
		@Override
		boolean removeConnection(AbstractSignal signal, NativeConnection nativeConnection) {
			signal.resolveSignal();
			return signal.core.removeConnection(signal, nativeConnection);
		}

		@Override
		Connection addConnectionToMethod(AbstractSignal signal, Object receiver, QMetaMethod slot, ConnectionType... connectionType) {
			signal.resolveSignal();
			return signal.core.addConnectionToMethod(signal, receiver, slot, connectionType);
		}
	}
	
	private abstract static class AbstractJavaSignalCore extends AbstractParameterSignalCore {
		public AbstractJavaSignalCore(List<SignalParameterType> signalParameterTypes) {
			super(signalParameterTypes);
		}

		private List<AbstractConnection<?>> connections;
		
		List<AbstractConnection<?>> connections() {
        	if(this.connections!=null) {
        		return new ArrayList<>(this.connections);
			}else {
				return Collections.emptyList();
			}
		}
		
		void removeConnection_helper(Collection<AbstractConnection<?>> toRemove) {
            if (toRemove != null) {
				if(connections!=null) {
					if(connections.size()==1) {
						for (AbstractConnection<?> c : toRemove){
							if(!c.isEternal() && connections.contains(c)) {
		                		c.setDisconnected();
								connections = null;
								break;
							}
						}
					}else{
		                for (AbstractConnection<?> c : toRemove){
		                	if(!c.isEternal()) {
		                		c.setDisconnected();
		                		connections.remove(c);
		                	}
		                }
					}
				}
            }
        }
		
		void nativeEmit(AbstractSignal signal, QObject sender, Object... args) {
		}
		
		@Override
		final void emitSignal(AbstractSignal signal, Object... args) {
			QtSignalEmitterInterface senderObject = signal.containingObject();
            // When you dispose() a QObject, the first thing that happens is
            // that the native ID is set to 0. Then we proceed to delete the
            // native object. If this has a destructor which emits signals,
            // they will be passed into Java, which will lead to a threadCheck()
            // from signalsBlocked, and you will get an exception. We cannot
            // support listening to signals emitted in the C++ destructor.
			QObject sender = null;
			if(senderObject instanceof QObject)
				sender = (QObject)senderObject;
            if (signal.isQObjectSignal() && !signal.isStaticSignal()) {
                return;
            }
            Object currentThread = null;
            boolean senderWithAffinity = senderObject instanceof QtThreadAffineInterface 
            		&& ((QtThreadAffineInterface) senderObject).thread() == (currentThread = QThread.currentThread());
        	if ( senderWithAffinity && senderObject instanceof QtSignalBlockerInterface && ((QtSignalBlockerInterface)senderObject).signalsBlocked()) {
            	logger.finest(()->String.format("Blocking signal %1$s", signal.fullName()));
                return;
            }

        	nativeEmit(signal, sender, args);

            Collection<AbstractConnection<?>> toRemove = Collections.emptyList();

            for (AbstractConnection<?> c : connections()) {
                // We do a direct connection in three cases:
                // 1. If the connection is explicitly set to be direct
                // 2. If it is automatic and the receiver is not a QObject (no thread() function)
                // 3. If it is automatic, the receiver is a QObject and the sender and receiver
                //    are both in the current thread
                Object receiver;
				Object[] _args = args;
				if(c.argumentCount()<args.length) {
					_args = Arrays.copyOf(_args, c.argumentCount());
				}
                boolean isStatic = false;
                if(c instanceof SlotObjectConnection<?>){
                    Object lambdaOwner = ((SlotObjectConnection<?>) c).lambdaOwner();
                    if(lambdaOwner!=null)
                    	receiver = lambdaOwner;
                    else
                    	receiver = c.resolveReceiver();
                }else if(c instanceof AbstractReflectiveConnection){
                	receiver = c.resolveReceiver();
                	AbstractReflectiveConnection<?> reflectiveConnection = (AbstractReflectiveConnection<?>)c;
					isStatic = Modifier.isStatic(reflectiveConnection.slot.getModifiers());
					if (signal.inCppEmission() && reflectiveConnection.isEternal())
						continue;
				}else {
					continue;
				}
                
                if(
					   (receiver == null && !isStatic)
					|| (receiver instanceof QtObjectInterface && QtJambiInternal.internalAccess.nativeId((QtObjectInterface)receiver) == 0)
				  ){
					if (toRemove.isEmpty()) {
						toRemove = Collections.singletonList(c);
					}else {
						if (toRemove.size()==1) {
							toRemove = new ArrayList<>(toRemove);
						}
						toRemove.add(c);
					}
					continue;
				}
                boolean directCall = false;
                boolean receiverThreadIsCurrent;
                if(receiver instanceof QtThreadAffineInterface && !c.isDirectConnection()) {
                	Object receiverThread = ((QtThreadAffineInterface) receiver).thread();
                	if(receiverThread==null) {
                		directCall = true;
                		receiverThreadIsCurrent = true;
                	}else {
	                	if(currentThread==null) {
	                		currentThread = QThread.currentThread();
	                		receiverThreadIsCurrent = receiverThread==currentThread;
	                	}else {
	                		receiverThreadIsCurrent = receiverThread==currentThread;
	                	}
	                	directCall = receiverThreadIsCurrent && c.isAutoConnection();
                	}
                }else{
                	receiverThreadIsCurrent = true;
                	directCall = true;
                }
                if (directCall) {
                	logger.finest(()->{
						if(c instanceof ReflectiveConnection){
                            ReflectiveConnection methodConnection = (ReflectiveConnection)c;
                            return String.format("Emit signal %1$s: direct invocation of %2$s, receiver=%3$s", 
                            		signal.fullName(), 
                            		methodConnection.slot, 
                            		receiver==null ? "(static)" : receiver.getClass().getName());
						}else {
							return String.format("Emit signal %1$s: direct invocation of slot object.", signal.fullName());
						}
                	});
					try (CurrentSenderSetter setter = new CurrentSenderSetter(senderObject, receiver)){
                        c.invoke(_args);
                    } catch (RuntimeException | Error e) {
                    	throw e;
                    } catch (Throwable e) {
                        if(c instanceof ReflectiveConnection){
                            ReflectiveConnection methodConnection = (ReflectiveConnection)c;
                            throw new QSignalInvocationException(String.format("Exception caught in signal %1$s after invoking slot %2$s: sender=%3$s, receiver=%4$s", 
                            		signal.fullName(), 
									methodConnection.slot,
									senderObject != null ? senderObject.getClass().getName() : "N/A",
											receiver==null ? "(static)" : receiver.getClass().getName()
								),e);
						}else{
							throw new QSignalInvocationException(String.format("Exception caught in signal %1$s: sender=%2$s, receiver=%3$s", 
									signal.fullName(), 
									senderObject != null ? senderObject.getClass().getName() : "N/A",
											receiver==null ? "N/A" : receiver.getClass().getName()
								),e);
						}
                    }
                } else {
                    if (receiver instanceof QtThreadAffineInterface) {
                    	QtThreadAffineInterface eventReceiver = (QtThreadAffineInterface) receiver;
                        logger.finest(()->{
							if(c instanceof ReflectiveConnection){
                                ReflectiveConnection methodConnection = (ReflectiveConnection)c;
                                return String.format("Emit signal %1$s: queueing invocation of %2$s, receiver=%3$s", 
                                		signal.fullName(), 
                                		methodConnection.slot, 
                                		receiver.getClass().getName());
							}else {
								return String.format("Emit signal %1$s: queueing invocation of slot object.", signal.fullName());
							}
                    	});
                        logger.finest(()->"new MetaCall() current thread = "+Thread.currentThread() + "event receiver thread = "+eventReceiver.thread()+" of object "+eventReceiver.getClass().getName());
                        logger.finest(()->c.isBlockingQueuedConnection() ? "invokeAndWait" : "invokeLater");
                        if(c.isBlockingQueuedConnection()) {
                        	if(receiverThreadIsCurrent) {
                        		logger.log(java.util.logging.Level.SEVERE, 
            							()->String.format("Qt: Dead lock detected while activating a BlockingQueuedConnection: signal is %1$s, receiver is %2$s", 
            									signal.fullName(), 
            									receiver.getClass().getName()
            								)
            							);
                        	}
                        	MetaCall metaCall;
                        	if(c instanceof ReflectiveConnection){
                				ReflectiveConnection methodConnection = (ReflectiveConnection)c;
                				metaCall = new MetaCall(eventReceiver, signal, c, senderObject, methodConnection.slot, _args);
                        	}else {
                        		metaCall = new MetaCall(eventReceiver, signal, c, senderObject, null, _args);
                        	}
                        	QMetaObject.invokeMethod(metaCall, Qt.ConnectionType.BlockingQueuedConnection);
                        	metaCall.throwException();
                        }else {
                        	MetaCall metaCall;
                        	if(c instanceof ReflectiveConnection){
                				ReflectiveConnection methodConnection = (ReflectiveConnection)c;
                				metaCall = new ThrowingMetaCall(eventReceiver, signal, c, senderObject, methodConnection.slot, _args);
                        	}else {
                        		metaCall = new ThrowingMetaCall(eventReceiver, signal, c, senderObject, null, _args);
                        	}
                        	QMetaObject.invokeMethod(metaCall, Qt.ConnectionType.QueuedConnection);
                        }
                    }else{
						logger.log(java.util.logging.Level.SEVERE, 
							()->String.format("Cannot emit signal %1$s: queued connection to not thread-affine receiver %2$s.", 
									signal.fullName(), 
									receiver==null ? "N/A" : receiver.getClass().getName()
								)
							);
						continue;
					}
                }
            }

            // Remove the ones marked for removal..
            removeConnection_helper(toRemove);
		}
		
		@Override
		Connection addConnectionToMethod(AbstractSignal signal, Object receiver, Method slot, MethodHandle slotHandle, 
				List<Supplier<?>> lambdaArgs, boolean unsolvableConnection, boolean allowNativeConnect, ConnectionType... connectionType) {
			if(isUnique(connectionType)){
				for (AbstractConnection<?> c : connections()) {
					Object resolvedReceiver = c.resolveReceiver();
					if(c instanceof ReflectiveConnection){
						if (resolvedReceiver == receiver
							&& slot.equals(((ReflectiveConnection)c).slot)) {
							return null;
						}
					}
				}
			}

			if(slotHandle==null){
				try{
					slotHandle = QtJambiInternal.privateLookup(slot.getDeclaringClass()).unreflect(slot);
					if(slotHandle.isVarargsCollector()) {
						slotHandle = slotHandle.asFixedArity();
					}
				} catch (Throwable e2) {
					logger.log(java.util.logging.Level.WARNING, "Exception caught while analyzing slot "+slot, e2);
					return null;
				}
			}
			
            AbstractReflectiveConnection<?> conn;
            if(unsolvableConnection) {
            	conn = new EternalConnection(new WeakReference<Object>(receiver), slot, slotHandle, lambdaArgs, connectionType);
            }else {
            	conn = new ReflectiveConnection(new WeakReference<Object>(receiver), slot, slotHandle, lambdaArgs, connectionType);
            }
            if(!(receiver instanceof QtThreadAffineInterface) && (conn.isQueuedConnection() || conn.isBlockingQueuedConnection())) {
            	throw new IllegalArgumentException(String.format("Cannot establish %1$s connection to slot of not thread-affine receiver", 
            			conn.isBlockingQueuedConnection() ? "blocking-queued" : "queued"));
            }
            if(connections==null) {
            	connections = Collections.singletonList(conn);
            }else {
            	if(connections.size()==1) {
            		connections = new ArrayList<>(connections);
            	}
            	connections.add(conn);
            }
            return new JavaConnection(signal, conn);
		}
		
		@Override
		<Slot extends Serializable> Connection addConnectionToSlotObject(AbstractSignal signal,
				SlotConnectionFactory<Slot> factory, Object lambdaOwner, Class<?> lambdaOwnerClass, int lambdaHashCode, Slot slotObject, ConnectionType[] connectionType) {
			if(isUnique(connectionType)){
				for (AbstractConnection<?> c : connections()) {
					Object resolvedReceiver = c.resolveReceiver();
					if(c instanceof SlotObjectConnection){
						if (resolvedReceiver == slotObject) {
							return null;
						}
					}
				}
			}
			
            SlotObjectConnection<?> conn = factory.create(new WeakReference<Object>(lambdaOwner), lambdaOwnerClass, lambdaHashCode, slotObject, connectionType);
            if(!(lambdaOwner instanceof QtThreadAffineInterface) && (conn.isQueuedConnection() || conn.isBlockingQueuedConnection())) {
            	throw new IllegalArgumentException(String.format("Cannot establish %1$s connection to slot of not thread-affine receiver", 
            			conn.isBlockingQueuedConnection() ? "blocking-queued" : "queued"));
            }
            if(connections==null) {
            	connections = Collections.singletonList(conn);
            }else {
            	if(connections.size()==1) {
            		connections = new ArrayList<>(connections);
            	}
            	connections.add(conn);
            }
            return new JavaConnection(signal, conn);
		}

		@Override
		boolean removeConnectionToMethod(AbstractSignal signal, Object receiver, QMetaMethod slot, boolean allowNativeDisconnect) {
			boolean returned = false;
			signal.setInDisconnect(true);
			try{
				Collection<AbstractConnection<?>> toRemove = Collections.emptyList();
				for (AbstractConnection<?> c : connections()) {
					Object resolvedReceiver = c.resolveReceiver();
					if(c instanceof MetaMethodConnection){
						if (resolvedReceiver == null) {  // GCed receiver
							if (toRemove.isEmpty()) {
								toRemove = Collections.singletonList(c);
							}else {
								if (toRemove.size()==1) {
									toRemove = new ArrayList<>(toRemove);
								}
								toRemove.add(c);
							}
						} else if ((receiver == null || resolvedReceiver == receiver)
							&& (slot == null || slot.equals(((MetaMethodConnection)c).slot))) {
							if (toRemove.isEmpty()) {
								toRemove = Collections.singletonList(c);
							}else {
								if (toRemove.size()==1) {
									toRemove = new ArrayList<>(toRemove);
								}
								toRemove.add(c);
							}
							returned = true;
						}
					}
				}
				removeConnection_helper(toRemove);
			}finally{
				signal.setInDisconnect(false);
			}
			return returned;
		}
		
		@Override
		boolean removeConnectionToMethod(AbstractSignal signal, Object receiver, Method slot, Object[] lambdaArgs,
				boolean allowNativeDisconnect) {
			boolean returned = false;
			signal.setInDisconnect(true);
			try{
				QtSignalEmitterInterface containingObject = signal.containingObject();
				Collection<AbstractConnection<?>> toRemove = Collections.emptyList();
				for (AbstractConnection<?> c : connections()) {
					Object resolvedReceiver = c.resolveReceiver();
					if(c instanceof ReflectiveConnection){
						if (resolvedReceiver == null) {  // GCed receiver
							if (toRemove.isEmpty()) {
								toRemove = Collections.singletonList(c);
							}else {
								if (toRemove.size()==1) {
									toRemove = new ArrayList<>(toRemove);
								}
								toRemove.add(c);
							}
						} else if ((receiver == null || resolvedReceiver == receiver)
							&& (slot == null || slot.equals(((ReflectiveConnection)c).slot))) {
							if (toRemove.isEmpty()) {
								toRemove = Collections.singletonList(c);
							}else {
								if (toRemove.size()==1) {
									toRemove = new ArrayList<>(toRemove);
								}
								toRemove.add(c);
							}
							returned = true;
						}
					}else if(c instanceof SlotObjectConnection){
						SlotObjectConnection<?> slotObjectConnection = (SlotObjectConnection<?>)c;
						Object lambdaOwner = slotObjectConnection.lambdaOwner();
						Class<?> lambdaOwnerClass = slotObjectConnection.lambdaOwnerClass;
						if(lambdaOwner!=null || lambdaOwnerClass!=null){
							if(receiver != null && slot == null){
								if(lambdaOwner == receiver || 
										lambdaOwnerClass == receiver){
									if (toRemove.isEmpty()) {
										toRemove = Collections.singletonList(c);
									}else {
										if (toRemove.size()==1) {
											toRemove = new ArrayList<>(toRemove);
										}
										toRemove.add(c);
									}
									returned = true;
								}
							}else if(receiver == null && slot == null){
								if (toRemove.isEmpty()) {
									toRemove = Collections.singletonList(c);
								}else {
									if (toRemove.size()==1) {
										toRemove = new ArrayList<>(toRemove);
									}
									toRemove.add(c);
								}
								returned = true;
							}else{
								// remove if lambda owner has been disposed
								if(lambdaOwner instanceof QtJambiObject && QtJambiInternal.internalAccess.nativeId((QtJambiObject)lambdaOwner)==0){
									if (toRemove.isEmpty()) {
										toRemove = Collections.singletonList(c);
									}else {
										if (toRemove.size()==1) {
											toRemove = new ArrayList<>(toRemove);
										}
										toRemove.add(c);
									}
								}
							}
						}else if(resolvedReceiver == receiver){
							if (toRemove.isEmpty()) {
								toRemove = Collections.singletonList(c);
							}else {
								if (toRemove.size()==1) {
									toRemove = new ArrayList<>(toRemove);
								}
								toRemove.add(c);
							}
							returned = true;
						}
					}
				}

				removeConnection_helper(toRemove);

				if (signal.isNativeSignal() 
						&& containingObject instanceof QObject 
						&& (receiver instanceof QObject 
								|| receiver == null
								|| ( receiver instanceof QMetaObject.AbstractSignal 
										&& slot.getName().equals("emit") 
										&& ((AbstractSignal)receiver).methodIndex()>=0 
										&& ((AbstractSignal)receiver).containingObject() instanceof QObject)
					)) {
					
					QObject senderObject = (QObject)signal.containingObject();
	        		QMetaMethod signalMethod = null;
	        		if(signal.methodIndex()>=0) {
	        			signalMethod = QMetaMethod.fromSignal((QMetaObject.AbstractSignal)signal);
	        		}
	        		if(signalMethod!=null) {
		        		QObject receiverObject = null;
		        		QMetaMethod slotMethod = null;
		        		if(receiver instanceof QObject) {
		        			receiverObject = (QObject)receiver;
		        			if(slot!=null) {
		        				slotMethod = QMetaMethod.fromReflectedMethod(slot);
		        			}
		        		}else if(receiver instanceof AbstractSignal){
		        			AbstractSignal otherSignal = (AbstractSignal)receiver;
		        			receiverObject = (QObject)otherSignal.containingObject();
			        		if(otherSignal.methodIndex()>=0) {
			        			slotMethod = QMetaMethod.fromSignal((QMetaObject.AbstractSignal)otherSignal);
			        		}
		        		}
        				returned |= QObject.disconnect(senderObject, signalMethod, receiverObject, slotMethod);
	        		}
				}

			}finally{
				signal.setInDisconnect(false);
			}
			return returned;
		}
		
		@Override
		boolean removeConnectionToSlotObject(AbstractSignal signal,
				Serializable slotObject, Class<?> _functionalInterfaceClass, Object _lambdaOwner, Class<?> _lambdaOwnerClass,
				int _lambdaHashCode, boolean useLambdaInfo) {
			boolean returned = true;
			Collection<AbstractConnection<?>> toRemove = Collections.emptyList();
			if(useLambdaInfo) {
				for (AbstractConnection<?> c : connections()) {
					Object resolvedReceiver = c.resolveReceiver();
					if(c instanceof SlotObjectConnection){
						SlotObjectConnection<?> slotObjectConnection = (SlotObjectConnection<?>)c;
						Object lambdaOwner = slotObjectConnection.lambdaOwner();
						Class<?> lambdaOwnerClass = slotObjectConnection.lambdaOwnerClass;
						if(resolvedReceiver==slotObject || (
								_functionalInterfaceClass!=null 
									&& _functionalInterfaceClass.isInstance(resolvedReceiver) 
									&& _lambdaOwner==lambdaOwner 
									&& _lambdaOwnerClass==lambdaOwnerClass 
									&& _lambdaHashCode==slotObjectConnection.lambdaHashCode
								)){
							if (toRemove.isEmpty()) {
								toRemove = Collections.singletonList(c);
							}else {
								if (toRemove.size()==1) {
									toRemove = new ArrayList<>(toRemove);
								}
								toRemove.add(c);
							}
							returned = true;
						}
					}
				}
			}else {
				for (AbstractConnection<?> c : connections()) {
					Object resolvedReceiver = c.resolveReceiver();
					if(resolvedReceiver==slotObject){
						if (toRemove.isEmpty()) {
							toRemove = Collections.singletonList(c);
						}else {
							if (toRemove.size()==1) {
								toRemove = new ArrayList<>(toRemove);
							}
							toRemove.add(c);
						}
						returned = true;
					}
				}				
			}
			removeConnection_helper(toRemove);
			return returned;
		}
		
		@Override
		boolean removeConnection(AbstractSignal signal, JavaConnection javaConnection) {
			boolean returned = false;
			Collection<AbstractConnection<?>> toRemove = Collections.emptyList();
			for (AbstractConnection<?> c : connections()) {
				Object resolvedReceiver = c.resolveReceiver();
				if(c instanceof ReflectiveConnection){
					if (resolvedReceiver == null   // GCed receiver
						|| c.equals(javaConnection)) {
						if (toRemove.isEmpty()) {
							toRemove = Collections.singletonList(c);
						}else {
							if (toRemove.size()==1) {
								toRemove = new ArrayList<>(toRemove);
							}
							toRemove.add(c);
						}
					}
				}else if(c instanceof SlotObjectConnection){
					SlotObjectConnection<?> slotObjectConnection = (SlotObjectConnection<?>)c;
					Object lambdaOwner = slotObjectConnection.lambdaOwner();
					Class<?> lambdaOwnerClass = slotObjectConnection.lambdaOwnerClass;
					if(lambdaOwner!=null || lambdaOwnerClass!=null){
						if(c.equals(javaConnection)){
							if (toRemove.isEmpty()) {
								toRemove = Collections.singletonList(c);
							}else {
								if (toRemove.size()==1) {
									toRemove = new ArrayList<>(toRemove);
								}
								toRemove.add(c);
							}
							returned = true;
						}else{
							// remove if lambda owner has been disposed
							if(lambdaOwner instanceof QtJambiObject && QtJambiInternal.internalAccess.nativeId((QtJambiObject)lambdaOwner)==0){
								if (toRemove.isEmpty()) {
									toRemove = Collections.singletonList(c);
								}else {
									if (toRemove.size()==1) {
										toRemove = new ArrayList<>(toRemove);
									}
									toRemove.add(c);
								}
							}
						}
					}else if(c.equals(javaConnection)){
						if (toRemove.isEmpty()) {
							toRemove = Collections.singletonList(c);
						}else {
							if (toRemove.size()==1) {
								toRemove = new ArrayList<>(toRemove);
							}
							toRemove.add(c);
						}
						returned = true;
					}
				}
			}

			removeConnection_helper(toRemove);
			return returned;
		}

		@Override
		Connection addConnectionToMethod(AbstractSignal signal, Object receiver, QMetaMethod slot,
				ConnectionType... connectionType) {
			MetaMethodConnection conn = new MetaMethodConnection(new WeakReference<Object>(receiver), slot, connectionType);
			if(connections==null) {
            	connections = Collections.singletonList(conn);
            }else {
            	if(connections.size()==1) {
            		connections = new ArrayList<>(connections);
            	}
            	connections.add(conn);
            }
            return new JavaConnection(signal, conn);
		}
	}
	
	private static class JavaSignalCore extends AbstractJavaSignalCore {
		public JavaSignalCore(String name, List<SignalParameterType> signalParameterTypes) {
			super(signalParameterTypes);
			this.name = name;
		}

		private final String name;
		
		@Override
		String name(AbstractSignal signal) {
			return name;
		}
	}
	
	/**
     * QSignalEmitter is a class used internally by Qt Jambi.
     * You should never have to concern yourself with this class.
     */
    protected static abstract class AbstractSignal {
    	private byte flags = 0;
    	private AbstractSignalCore core;
		private Class<?> declaringClass;

		protected AbstractSignal() {
			this.core = AnalyzingSignalCore.instance;
        }
		
		/**
		 * Static member signal constructor
		 */
		protected AbstractSignal(Class<?> declaringClass, boolean isDisposed) {
			this.declaringClass = declaringClass;
			flags |= IsStaticSignal;
			if(isDisposed) {
				this.core = new JavaSignalCore("disposed", Collections.emptyList());
			}else {
				this.core = AnalyzingSignalCore.instance;
			}
        }
		
		protected AbstractSignal(Class<?>[] types) {
			this("anonymous_signal", types);
		}

        @io.qt.QtUninvokable
		public synchronized int methodIndex() {
			return core.methodIndex(this);
		}

        @io.qt.QtUninvokable
		Class<?> getDeclaringClass() {
        	name();// initializes the core
			return declaringClass;
		}
        
        protected AbstractSignal(String name, Class<?>[] types) {
        	name = Objects.requireNonNull(name);
        	List<SignalParameterType> typeList = Collections.emptyList();
        	for (int i = 0; i < types.length; i++) {
        		int arrayDimension = 0;
        		Class<?> type = types[i];
        		while (type.isArray()) {
        			arrayDimension++;
        			type = type.getComponentType();
                }
        		SignalParameterType signalType = new SignalParameterType(types[i], type, type, null, arrayDimension, false, false);
        		if(i==0) {
        			if(types.length>1) {
        				typeList = new ArrayList<>();
        				typeList.add(signalType);
        			}else {
        				typeList = Collections.singletonList(signalType);
        			}
        		}else {
        			typeList.add(signalType);
        		}
			}
        	this.core = new JavaSignalCore(name, types.length>1 ? Collections.unmodifiableList(typeList) : typeList);
        }

        @io.qt.QtUninvokable
        synchronized List<SignalParameterType> signalTypes() {
			return core.signalTypes(this);
		}

        @io.qt.QtUninvokable
        public List<Class<?>> argumentTypes() {
        	List<SignalParameterType> signalTypes = signalTypes();
        	switch(signalTypes.size()){
        	case 0:
        		return Collections.emptyList();
        	case 1:
        		return Collections.singletonList(signalTypes.get(0).originalType);
    		default:
    			List<Class<?>> result = new ArrayList<>();
    			for(SignalParameterType signalType : signalTypes) {
    				result.add(signalType.originalType);
    			}
    			return Collections.unmodifiableList(result);
        	}
		}

        @io.qt.QtUninvokable
        private List<Class<?>> signalTypeClasses() {
        	List<SignalParameterType> signalTypes = signalTypes();
        	switch(signalTypes.size()){
        	case 0:
        		return Collections.emptyList();
        	case 1:
        		return Collections.singletonList(signalTypes.get(0).type);
    		default:
    			List<Class<?>> result = new ArrayList<>();
    			for(SignalParameterType signalType : signalTypes) {
    				result.add(signalType.type);
    			}
    			return Collections.unmodifiableList(result);
        	}
		}

        @io.qt.QtUninvokable
        private boolean inCppEmission() {
			return (flags & InCppEmission)==InCppEmission;
		}

        @NativeAccess
        @io.qt.QtUninvokable
        private void setInCppEmission(boolean inCppEmission) {
        	if(inCppEmission)
        		flags |= InCppEmission;
        	else
        		flags &= ~InCppEmission;
		}

        @NativeAccess
        @io.qt.QtUninvokable
        private boolean inJavaEmission() {
			return (flags & InJavaEmission)==InJavaEmission;
		}

        @io.qt.QtUninvokable
        private void setInJavaEmission(boolean inJavaEmission) {
        	if(inJavaEmission)
        		flags |= InJavaEmission;
        	else
        		flags &= ~InJavaEmission;
		}

        @io.qt.QtUninvokable
        private boolean inDisconnect() {
			return (flags & InDisconnect)==InDisconnect;
		}

        @io.qt.QtUninvokable
        private void setInDisconnect(boolean inDisconnect) {
        	if(inDisconnect)
        		flags |= InDisconnect;
        	else
        		flags &= ~InDisconnect;
		}

        @io.qt.QtUninvokable
        private boolean isNativeSignal() {
			return (flags & IsNativeSignal)==IsNativeSignal;
		}
        
        @io.qt.QtUninvokable
        private boolean isStaticSignal() {
			return (flags & IsStaticSignal)==IsStaticSignal;
		}

        @io.qt.QtUninvokable
        private void setNativeSignal(boolean isNativeSignal) {
        	if(isNativeSignal)
        		flags |= IsNativeSignal;
        	else
        		flags &= ~IsNativeSignal;
		}

        @io.qt.QtUninvokable
        private boolean isQObjectSignal() {
			return (flags & IsQObjectSignal)==IsQObjectSignal;
		}

        @io.qt.QtUninvokable
        private void setQObjectSignal(boolean isQObjectSignal) {
        	if(isQObjectSignal)
        		flags |= IsQObjectSignal;
        	else
        		flags &= ~IsQObjectSignal;
		}

        /**
         * Returns the object containing this signal.
         * @return the signal containing object
         */
        @io.qt.QtUninvokable
        public QtSignalEmitterInterface containingObject() {
        	return null;
        }

        /**
         * Returns the name of the signal
         * @return The signal name
         */
        @io.qt.QtUninvokable
        public synchronized final String name() {
            return core.name(this);
        }
        
        @NativeAccess
        @io.qt.QtUninvokable
		private synchronized void initializeExtraSignal(Class<?> declaringClass, List<Class<?>> signalParameterTypes, int methodIndex) {
        	if(methodIndex>=0 && core instanceof AnalyzingSignalCoreInterface) {
	        	this.declaringClass = declaringClass;
	        	setNativeSignal(true);
	        	setQObjectSignal(true);
				List<SignalParameterType> typeList = Collections.emptyList();
	        	for (int i = 0; i < signalParameterTypes.size(); i++) {
	        		int arrayDimension = 0;
	        		Class<?> type = signalParameterTypes.get(i);
	        		if(type!=null) {
		        		while (type.isArray()) {
		        			arrayDimension++;
		        			type = type.getComponentType();
		                }
	        		}
					SignalParameterType signalType = new SignalParameterType(type, type, type, null, arrayDimension, false, false);
	        		if(i==0) {
	        			if(signalParameterTypes.size()>1) {
	        				typeList = new ArrayList<>();
	        				typeList.add(signalType);
	        			}else {
	        				typeList = Collections.singletonList(signalType);
	        			}
	        		}else {
	        			typeList.add(signalType);
	        		}
				}
	        	this.core = new QObjectsSignalCore(typeList, methodIndex);
        	}
		}
		
        /**
         * Returns the full name of the signal, on the form "package.class.signalName"
         *
         *  @return The fully qualified name of the signal
         */
        @io.qt.QtUninvokable
        public final String fullName() {
        	String name = name();
            return declaringClass == null ? name : declaringClass.getName() + "." + name;
        }

        @io.qt.QtUninvokable
        protected QMetaObject.Connection addConnection(Object receiver, String method, Qt.ConnectionType... connectionType) {
	        Method slotMethod = lookupSlot(receiver, method, null);
	        if (slotMethod == null) {
	        	QMetaObject metaObject = receiver instanceof QObject ? ((QObject)receiver).metaObject() : QMetaObject.forType(receiver.getClass());
	        	if(metaObject!=null) {
	        		QMetaMethod metaMethod = metaObject.method(method);
	        		if(metaMethod!=null && metaMethod.isValid()) {
	        			if(!matchTypes(metaMethod.parameterClassTypes().toArray(new Class[metaMethod.parameterTypes().size()]))) {
	        				throw new QMisfittingSignatureException("Incompatible sender/receiver arguments " + fullName() + " --> " + metaMethod.methodSignature());
	        			}
	        			return addConnectionToMethod(receiver, metaMethod, connectionType);
	        		}
	        	}
	            throw new QNoSuchSlotException(receiver, method);
	        }
	        return addConnection(receiver, slotMethod, connectionType);
        }

        @NativeAccess
        @io.qt.QtUninvokable
        private QMetaObject.Connection addConnectionFromCpp(Object receiver, Method slotMethod, int conType) {
            if (slotMethod.isAnnotationPresent(QtUninvokable.class)) {
            	QMetaMethod method = QMetaMethod.fromReflectedMethod(slotMethod);
            	if(method.isValid()) {
            		slotMethod = method.toReflectedMethod();
            	}else {
            		throw new QUninvokableSlotException(slotMethod);
            	}
            }

            if (!matchSlot(slotMethod))
            	throw new QMisfittingSignatureException("Incompatible sender/receiver arguments " + fullName() + " --> " + slotMethod.toString());

            List<Qt.ConnectionType> connectionType = new ArrayList<>();
            if((conType & Qt.ConnectionType.DirectConnection.value()) == Qt.ConnectionType.DirectConnection.value()) {
            	connectionType.add(Qt.ConnectionType.DirectConnection);
            }
            if((conType & Qt.ConnectionType.UniqueConnection.value()) == Qt.ConnectionType.UniqueConnection.value()) {
            	connectionType.add(Qt.ConnectionType.UniqueConnection);
            }
            if((conType & Qt.ConnectionType.QueuedConnection.value()) == Qt.ConnectionType.QueuedConnection.value()) {
            	connectionType.add(Qt.ConnectionType.QueuedConnection);
            }
            return addConnectionToMethod(receiver, slotMethod, null, null, false, false, connectionType.toArray(new Qt.ConnectionType[connectionType.size()]));
        }

        @io.qt.QtUninvokable
        protected QMetaObject.Connection addConnection(Object receiver, Method slotMethod, Qt.ConnectionType... connectionType) {
            if (slotMethod.isAnnotationPresent(QtUninvokable.class)) {
            	QMetaMethod method = QMetaMethod.fromReflectedMethod(slotMethod);
            	if(method.isValid()) {
            		slotMethod = method.toReflectedMethod();
            	}else {
            		throw new QUninvokableSlotException(slotMethod);
            	}
            }

            if (!matchSlot(slotMethod))
                throw new QMisfittingSignatureException("Incompatible sender/receiver arguments " + fullName() + " --> " + slotMethod.toString());

            return addConnectionToMethod(receiver, slotMethod, null, null, false, true, connectionType);
        }
        
        /**
         * Base types of all signal arguments in order of declaration. If the argument is of an array type,
         * then the base type of the array is returned by resolveSignal, and the actual number of dimensions
         * of the array can be retrieved using arrayDimensions(). If the argument is not of an array type,
         * the argument's type is returned.
         *
         * @return An array of Class objects specifying the base type of each of the signal arguments.
         */
        @io.qt.QtUninvokable
        private synchronized void resolveSignal() {
            if (core instanceof AnalyzingSignalCoreInterface) {
                boolean found = false;
                if( (flags & IsStaticSignal) == IsStaticSignal ) {
                	Field fields[] = declaringClass.getDeclaredFields();
                	java.lang.invoke.MethodHandles.Lookup lookup = QtJambiInternal.privateLookup(declaringClass);
                    for (Field field : fields) {
                        if (AbstractSignal.class.isAssignableFrom(field.getType())) {
                            AbstractSignal sig = (AbstractSignal)QtJambiInternal.fetchField(lookup, null, field);
                            if (sig == this) {
                            	if(!java.lang.reflect.Modifier.isStatic(field.getModifiers()))
                            		throw new QSignalDeclarationException(String.format("Missing modifier 'static' at signal %1$s.%2$s.", declaringClass.getSimpleName(), field.getName()));
                            	if (!Modifier.isFinal(field.getModifiers())) {
		                    		if(!Boolean.getBoolean("qtjambi.allow-nonfinal-signals") && !Boolean.getBoolean("io.qt.allow-nonfinal-signals")) {
		                    			java.util.logging.Logger.getLogger("io.qt.internal").severe(String.format("Missing modifier 'final' at signal %1$s.%2$s. Specify JVM argument -Dqtjambi.allow-nonfinal-signals=true to disable this error.", declaringClass.getSimpleName(), field.getName()));
		                    			throw new QSignalDeclarationException(String.format("Missing modifier 'final' at signal %1$s.%2$s.", declaringClass.getSimpleName(), field.getName()));
		                    		}
		                    	}
                                found = true;
                                analyzeSignalField(field);
                            }
                        }
                    }
                    if (!found)
                    	throw new QSignalInitializationException("Static signals must be declared as static members of a class.");
                }else {
                	QtSignalEmitterInterface containingObject = containingObject();
                	Class<?> cls = containingObject==null ? null : containingObject.getClass();
                    loop: while (cls != null) {
                        Field fields[] = cls.getDeclaredFields();
                        java.lang.invoke.MethodHandles.Lookup lookup = QtJambiInternal.privateLookup(cls);
                        for (Field field : fields) {
                            if (AbstractSignal.class.isAssignableFrom(field.getType())) {
                            	AbstractSignal sig = (AbstractSignal)QtJambiInternal.fetchField(lookup, containingObject, field);
                                if (sig == this) {
                                	if(java.lang.reflect.Modifier.isStatic(field.getModifiers()))
                                		throw new QSignalDeclarationException(String.format("Modifier 'static' not allowed for signal %1$s.%2$s. Use QStaticMemberSignals instead to declare a static signal.", declaringClass.getSimpleName(), field.getName()));
                                	if (!Modifier.isFinal(field.getModifiers())) {
    		                    		if(!Boolean.getBoolean("qtjambi.allow-nonfinal-signals") && !Boolean.getBoolean("io.qt.allow-nonfinal-signals")) {
    		                    			java.util.logging.Logger.getLogger("io.qt.internal").severe(String.format("Missing modifier 'final' at signal %1$s.%2$s. Specify JVM argument -Dqtjambi.allow-nonfinal-signals=true to disable this error.", cls.getSimpleName(), field.getName()));
    		                    			throw new QSignalDeclarationException(String.format("Missing modifier 'final' at signal %1$s.%2$s.", cls.getSimpleName(), field.getName()));
    		                    		}
    		                    	}
                                    found = true;
                                    analyzeSignalField(field);
                                    break loop;
                                }
                            }
                        }

                        cls = cls.getSuperclass();
                    }
                    if (!found) {
                    	if(this.getClass().getDeclaringClass()==QObject.class)
                    		throw new QSignalDeclarationException("Signals must be declared as members of QObject subclasses.");
                    	else if(this.getClass().getDeclaringClass()==QInstanceMemberSignals.class)
                    		throw new QSignalDeclarationException("Signals must be declared as members of QInstanceMemberSignals subclasses.");
                    	else
                    		throw new QSignalInitializationException("Unable to resolve signal.");
                    }
                }

                if(core instanceof AnalyzingSignalCoreInterface)
                	throw new QSignalInitializationException("Unable to detect signal name and types");

                if (core.signalTypes(this).size() == 0
                    && !(this instanceof QObject.Signal0)
                    && !(this instanceof QObject.PrivateSignal0)
                    && !(this instanceof QStaticMemberSignals.Signal0)
                    && !(this instanceof QStaticMemberSignals.PrivateSignal0)
                    && !(this instanceof QInstanceMemberSignals.Signal0)
                    && !(this instanceof QInstanceMemberSignals.PrivateSignal0)
                    && !(this instanceof QDeclarableSignals.AbstractPrivateGenericSignal))
                	throw new QSignalInitializationException("Signal initialization failed");
            }
        }

        @NativeAccess
        @io.qt.QtUninvokable
        private synchronized void initializeSignal(Field field, Method method, int methodIndex) {
        	if(methodIndex>=0 && core instanceof AnalyzingSignalCoreInterface) {
	        	declaringClass = field.getDeclaringClass();
	        	setNativeSignal(true);
	        	setQObjectSignal(true);
	
	            QtJambiInternal.ResolvedSignal resolvedSignal = QtJambiInternal.resolveSignal(field, declaringClass);
	            if(method!=null) {
	            	MethodHandle slotHandle = null;
	            	try{
    					slotHandle = QtJambiInternal.privateLookup(method.getDeclaringClass()).unreflect(method);
    					if(slotHandle.isVarargsCollector()) {
    						slotHandle = slotHandle.asFixedArity();
    					}
    				} catch (Throwable e2) {
    					logger.log(java.util.logging.Level.WARNING, "Exception caught while analyzing slot "+method, e2);
    				}
	            	EternalConnection con = new EternalConnection(new WeakReference<Object>(this.containingObject()), method, slotHandle, null);
	            	this.core = new QObjectsDirectSignalCore(resolvedSignal.signalTypes, methodIndex, con);
	            }else {
	            	this.core = new QObjectsSignalCore(resolvedSignal.signalTypes, methodIndex);
	            }
        	}
		}

        @io.qt.QtUninvokable
        private synchronized void analyzeSignalField(Field field) {
        	if(core instanceof AnalyzingSignalCoreInterface) {
	        	declaringClass = field.getDeclaringClass();
	        	setNativeSignal(QtJambiInternal.isGeneratedClass(declaringClass));
	        	setQObjectSignal(QObject.class.isAssignableFrom(declaringClass));
	
	            QtJambiInternal.ResolvedSignal resolvedSignal = QtJambiInternal.resolveSignal(field, declaringClass);
	
	            String name = resolvedSignal.field.getName();
	            List<SignalParameterType> signalParameterTypes = resolvedSignal.signalTypes;
	        	if(this instanceof io.qt.core.QDeclarableSignals.AbstractPrivateGenericSignal) {
					Class<?>[] types = (Class<?>[])QtJambiInternal.fetchField(this, io.qt.core.QDeclarableSignals.AbstractPrivateGenericSignal.class, "types", Class[].class);
					List<SignalParameterType> typeList = Collections.emptyList();
		        	for (int i = 0; i < types.length; i++) {
		        		int arrayDimension = 0;
		        		Class<?> type = types[i];
		        		while (type.isArray()) {
		        			arrayDimension++;
		        			type = type.getComponentType();
		                }
		        		SignalParameterType signalType = new SignalParameterType(types[i], type, type, null, arrayDimension, false, false);
		        		if(i==0) {
		        			if(types.length>1) {
		        				typeList = new ArrayList<>();
		        				typeList.add(signalType);
		        			}else {
		        				typeList = Collections.singletonList(signalType);
		        			}
		        		}else {
		        			typeList.add(signalType);
		        		}
					}
		        	signalParameterTypes = types.length>1 ? Collections.unmodifiableList(typeList) : typeList;
	            }
	        	if(isQObjectSignal()) {
	        		Class<?>[] argumentTypes = new Class[resolvedSignal.signalTypes.size()];
					int i=0;
	    			for(SignalParameterType signalType : resolvedSignal.signalTypes) {
	    				argumentTypes[i++] = signalType.originalType;
	    			}
					QMetaMethod signalMethod = QMetaObject.forType(declaringClass).method(name, argumentTypes);
					int methodIndex = -1;
					if(signalMethod!=null) {
						methodIndex = signalMethod.methodIndex();
					}
					if(methodIndex>=0) {
						this.core = new QObjectsSignalCore(resolvedSignal.signalTypes, methodIndex);
					}else {
						core = new JavaSignalCore(name, signalParameterTypes);
					}
	        	}else {
	        		core = new JavaSignalCore(name, signalParameterTypes);
	        	}
        	}
		}

        @io.qt.QtUninvokable
        private String signalParameters() {
        	return MetaObjectTools.bunchOfClassNamesInARow(signalTypes(), false);
        }

        @io.qt.QtUninvokable
        protected synchronized final void emitSignal(final Object ... args) {
        	setInJavaEmission(true);
            try {
            	core.emitSignal(this, args);
            } finally {
            	setInJavaEmission(false);
            }
        }

        @io.qt.QtUninvokable
        private boolean matchSlot(Method slot) {
        	Class<?>[] slotArguments = slot.getParameterTypes();
            List<SignalParameterType> signalTypes = signalTypes();
            if (slotArguments.length > signalTypes.size()) {
                return false;
            }

            for (int i = 0; i < slotArguments.length; ++i) {
                if (!matchTwoTypes(slotArguments[i],
                		signalTypes.get(i).type,
                		signalTypes.get(i).arrayDimension)) {
                    return false;
                }
            }

            return true;
        }

        @io.qt.QtUninvokable
        boolean matchTypes(Class<?>... types) {
        	List<SignalParameterType> typeList = Collections.emptyList();
        	for (int i = 0; i < types.length; i++) {
        		int arrayDimension = 0;
        		Class<?> type = types[i];
        		if(type!=null) {
	        		while (type.isArray()) {
	        			arrayDimension++;
	        			type = type.getComponentType();
	                }
        		}
        		SignalParameterType signalType = new SignalParameterType(types[i], type, type, null, arrayDimension, false, false);
        		if(i==0) {
        			if(types.length>1) {
        				typeList = new ArrayList<>();
        				typeList.add(signalType);
        			}else {
        				typeList = Collections.singletonList(signalType);
        			}
        		}else {
        			typeList.add(signalType);
        		}
			}
        	List<SignalParameterType> signalTypes = signalTypes();
        	if(signalTypes.equals(typeList)) {
        		return true;
        	}else {
        		String signalClassName = this.getClass().getName();
        		if(signalClassName.endsWith("Default1")
        				|| signalClassName.endsWith("Default2")
        				|| signalClassName.endsWith("Default3")
        				|| signalClassName.endsWith("Default4")
        				|| signalClassName.endsWith("Default5")
        				|| signalClassName.endsWith("Default6")
        				|| signalClassName.endsWith("Default7")
        				|| signalClassName.endsWith("Default8")
        				|| signalClassName.endsWith("Default9")) {
        			signalTypes = new ArrayList<>(signalTypes);
    				if(signalClassName.endsWith("Default1")) {
    					if(signalTypes.size()>=1) {
    						signalTypes.remove(signalTypes.size()-1);
    						if(signalTypes.equals(typeList))
    			        		return true;
    					}
            		}else if(signalClassName.endsWith("Default2")) {
    					if(signalTypes.size()>=2) {
    						signalTypes.remove(signalTypes.size()-1);
    						if(signalTypes.equals(typeList))
    			        		return true;
    						signalTypes.remove(signalTypes.size()-1);
    						if(signalTypes.equals(typeList))
    			        		return true;
    					}            			
            		}else if(signalClassName.endsWith("Default3")) {
    					if(signalTypes.size()>=3) {
    						signalTypes.remove(signalTypes.size()-1);
    						if(signalTypes.equals(typeList))
    			        		return true;
    						signalTypes.remove(signalTypes.size()-1);
    						if(signalTypes.equals(typeList))
    			        		return true;
    						signalTypes.remove(signalTypes.size()-1);
    						if(signalTypes.equals(typeList))
    			        		return true;
    					}            			
            		}else if(signalClassName.endsWith("Default4")) {
    					if(signalTypes.size()>=4) {
    						signalTypes.remove(signalTypes.size()-1);
    						if(signalTypes.equals(typeList))
    			        		return true;
    						signalTypes.remove(signalTypes.size()-1);
    						if(signalTypes.equals(typeList))
    			        		return true;
    						signalTypes.remove(signalTypes.size()-1);
    						if(signalTypes.equals(typeList))
    			        		return true;
    						signalTypes.remove(signalTypes.size()-1);
    						if(signalTypes.equals(typeList))
    			        		return true;
    					}            			
            		}else if(signalClassName.endsWith("Default5")) {
    					if(signalTypes.size()>=5) {
    						signalTypes.remove(signalTypes.size()-1);
    						if(signalTypes.equals(typeList))
    			        		return true;
    						signalTypes.remove(signalTypes.size()-1);
    						if(signalTypes.equals(typeList))
    			        		return true;
    						signalTypes.remove(signalTypes.size()-1);
    						if(signalTypes.equals(typeList))
    			        		return true;
    						signalTypes.remove(signalTypes.size()-1);
    						if(signalTypes.equals(typeList))
    			        		return true;
    						signalTypes.remove(signalTypes.size()-1);
    						if(signalTypes.equals(typeList))
    			        		return true;
    					}            			
            		}else if(signalClassName.endsWith("Default6")) {
    					if(signalTypes.size()>=6) {
    						signalTypes.remove(signalTypes.size()-1);
    						if(signalTypes.equals(typeList))
    			        		return true;
    						signalTypes.remove(signalTypes.size()-1);
    						if(signalTypes.equals(typeList))
    			        		return true;
    						signalTypes.remove(signalTypes.size()-1);
    						if(signalTypes.equals(typeList))
    			        		return true;
    						signalTypes.remove(signalTypes.size()-1);
    						if(signalTypes.equals(typeList))
    			        		return true;
    						signalTypes.remove(signalTypes.size()-1);
    						if(signalTypes.equals(typeList))
    			        		return true;
    						signalTypes.remove(signalTypes.size()-1);
    						if(signalTypes.equals(typeList))
    			        		return true;
    					}            			
            		}else if(signalClassName.endsWith("Default7")) {
    					if(signalTypes.size()>=7) {
    						signalTypes.remove(signalTypes.size()-1);
    						if(signalTypes.equals(typeList))
    			        		return true;
    						signalTypes.remove(signalTypes.size()-1);
    						if(signalTypes.equals(typeList))
    			        		return true;
    						signalTypes.remove(signalTypes.size()-1);
    						if(signalTypes.equals(typeList))
    			        		return true;
    						signalTypes.remove(signalTypes.size()-1);
    						if(signalTypes.equals(typeList))
    			        		return true;
    						signalTypes.remove(signalTypes.size()-1);
    						if(signalTypes.equals(typeList))
    			        		return true;
    						signalTypes.remove(signalTypes.size()-1);
    						if(signalTypes.equals(typeList))
    			        		return true;
    						signalTypes.remove(signalTypes.size()-1);
    						if(signalTypes.equals(typeList))
    			        		return true;
    					}            			
            		}else if(signalClassName.endsWith("Default8")) {
    					if(signalTypes.size()>=8) {
    						signalTypes.remove(signalTypes.size()-1);
    						if(signalTypes.equals(typeList))
    			        		return true;
    						signalTypes.remove(signalTypes.size()-1);
    						if(signalTypes.equals(typeList))
    			        		return true;
    						signalTypes.remove(signalTypes.size()-1);
    						if(signalTypes.equals(typeList))
    			        		return true;
    						signalTypes.remove(signalTypes.size()-1);
    						if(signalTypes.equals(typeList))
    			        		return true;
    						signalTypes.remove(signalTypes.size()-1);
    						if(signalTypes.equals(typeList))
    			        		return true;
    						signalTypes.remove(signalTypes.size()-1);
    						if(signalTypes.equals(typeList))
    			        		return true;
    						signalTypes.remove(signalTypes.size()-1);
    						if(signalTypes.equals(typeList))
    			        		return true;
    						signalTypes.remove(signalTypes.size()-1);
    						if(signalTypes.equals(typeList))
    			        		return true;
    					}            			
            		}else if(signalClassName.endsWith("Default9")) {
    					if(signalTypes.size()>=9) {
    						signalTypes.remove(signalTypes.size()-1);
    						if(signalTypes.equals(typeList))
    			        		return true;
    						signalTypes.remove(signalTypes.size()-1);
    						if(signalTypes.equals(typeList))
    			        		return true;
    						signalTypes.remove(signalTypes.size()-1);
    						if(signalTypes.equals(typeList))
    			        		return true;
    						signalTypes.remove(signalTypes.size()-1);
    						if(signalTypes.equals(typeList))
    			        		return true;
    						signalTypes.remove(signalTypes.size()-1);
    						if(signalTypes.equals(typeList))
    			        		return true;
    						signalTypes.remove(signalTypes.size()-1);
    						if(signalTypes.equals(typeList))
    			        		return true;
    						signalTypes.remove(signalTypes.size()-1);
    						if(signalTypes.equals(typeList))
    			        		return true;
    						signalTypes.remove(signalTypes.size()-1);
    						if(signalTypes.equals(typeList))
    			        		return true;
    						signalTypes.remove(signalTypes.size()-1);
    						if(signalTypes.equals(typeList))
    			        		return true;
    					}            			
            		}
        		}
        	}
            return false;
        }

        @io.qt.QtUninvokable
        private boolean matchTwoTypes(Class<?> slotArgument, Class<?> signalArgument, int signalArrayDims) {
            return matchTwoTypes(slotArgument, signalArgument, signalArrayDims, false);
        }

        @io.qt.QtUninvokable
        private boolean matchTwoTypes(Class<?> slotArgument,
                                      Class<?> signalArgument,
                                      int signalArrayDims,
                                      boolean wasArray) {
        	if(slotArgument.isPrimitive() && !wasArray)
        		slotArgument = QtJambiInternal.getComplexType(slotArgument);
        	if(signalArgument.isPrimitive() && signalArrayDims==0)
        		signalArgument = QtJambiInternal.getComplexType(signalArgument);

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
            } else if (!slotArgument.isAssignableFrom(signalArgument)) {
                return false;
            }

            return true;
        }
        
        protected void checkConnection(Object receiver, boolean slotObject) {
        	if(receiver==this) {
    			throw new IllegalArgumentException("Cannot connect a signal to itself.");
    		}
        }

        @io.qt.QtUninvokable
        private synchronized QMetaObject.Connection addConnectionToMethod(Object receiver, Method slot, MethodHandle slotHandle, Object[] lambdaArgs, boolean unsolvableConnection, boolean allowNativeConnect, Qt.ConnectionType... connectionType) {
    		checkConnection(receiver, false);
        	List<Supplier<?>> _lambdaArgs = null;
			if(lambdaArgs!=null && lambdaArgs.length>0) {
				if(lambdaArgs.length==1) {
					if(lambdaArgs[0]!=null && lambdaArgs[0] == this.containingObject()) {
						_lambdaArgs = Collections.singletonList(new WeakReference<>(lambdaArgs[0])::get);
					}else {
						Object arg = lambdaArgs[0];
						_lambdaArgs = Collections.singletonList(()->arg);
					}
				}else {
					_lambdaArgs = new ArrayList<>();
					for (int i = 0; i < lambdaArgs.length; i++) {
						Object arg = lambdaArgs[i];
						if(arg!=null && arg == this.containingObject()) {
							_lambdaArgs.add(new WeakReference<>(arg)::get);
						}else {
							_lambdaArgs.add(()->arg);
						}
					}
				}
			}
        	return core.addConnectionToMethod(this, receiver, slot, slotHandle, _lambdaArgs, unsolvableConnection, allowNativeConnect, connectionType);
        }
        
        @io.qt.QtUninvokable
        private synchronized QMetaObject.Connection addConnectionToMethod(Object receiver, QMetaMethod slot, Qt.ConnectionType... connectionType) {
    		checkConnection(receiver, false);
        	return core.addConnectionToMethod(this, receiver, slot, connectionType);
        }

        @io.qt.QtUninvokable
        protected QMetaObject.Connection addConnectionToSlotObject(GenericSlot slotObject, Qt.ConnectionType[] connectionType) {
        	return addConnectionToSlotObject(GenericSlotObjectConnection::new, slotObject, connectionType);
        }

        @io.qt.QtUninvokable
        protected QMetaObject.Connection addConnectionToSlotObject(Slot0 slotObject, Qt.ConnectionType[] connectionType) {
        	return addConnectionToSlotObject(SlotObjectConnection0::new, slotObject, connectionType);
        }

        @io.qt.QtUninvokable
        protected QMetaObject.Connection addConnectionToSlotObject(Slot1<?> slotObject, Qt.ConnectionType[] connectionType) {
        	return addConnectionToSlotObject(SlotObjectConnection1::new, slotObject, connectionType);
        }

        @io.qt.QtUninvokable
        protected QMetaObject.Connection addConnectionToSlotObject(Slot2<?,?> slotObject, Qt.ConnectionType[] connectionType) {
        	return addConnectionToSlotObject(SlotObjectConnection2::new, slotObject, connectionType);
        }

        @io.qt.QtUninvokable
        protected QMetaObject.Connection addConnectionToSlotObject(Slot3<?,?,?> slotObject, Qt.ConnectionType[] connectionType) {
        	return addConnectionToSlotObject(SlotObjectConnection3::new, slotObject, connectionType);
        }

        @io.qt.QtUninvokable
        protected QMetaObject.Connection addConnectionToSlotObject(Slot4<?,?,?,?> slotObject, Qt.ConnectionType[] connectionType) {
        	return addConnectionToSlotObject(SlotObjectConnection4::new, slotObject, connectionType);
        }

        @io.qt.QtUninvokable
        protected QMetaObject.Connection addConnectionToSlotObject(Slot5<?,?,?,?,?> slotObject, Qt.ConnectionType[] connectionType) {
        	return addConnectionToSlotObject(SlotObjectConnection5::new, slotObject, connectionType);
        }

        @io.qt.QtUninvokable
        protected QMetaObject.Connection addConnectionToSlotObject(Slot6<?,?,?,?,?,?> slotObject, Qt.ConnectionType[] connectionType) {
        	return addConnectionToSlotObject(SlotObjectConnection6::new, slotObject, connectionType);
        }

        @io.qt.QtUninvokable
        protected QMetaObject.Connection addConnectionToSlotObject(Slot7<?,?,?,?,?,?,?> slotObject, Qt.ConnectionType[] connectionType) {
        	return addConnectionToSlotObject(SlotObjectConnection7::new, slotObject, connectionType);
        }

        @io.qt.QtUninvokable
        protected QMetaObject.Connection addConnectionToSlotObject(Slot8<?,?,?,?,?,?,?,?> slotObject, Qt.ConnectionType[] connectionType) {
        	return addConnectionToSlotObject(SlotObjectConnection8::new, slotObject, connectionType);
        }

        @io.qt.QtUninvokable
        protected QMetaObject.Connection addConnectionToSlotObject(Slot9<?,?,?,?,?,?,?,?,?> slotObject, Qt.ConnectionType[] connectionType) {
        	return addConnectionToSlotObject(SlotObjectConnection9::new, slotObject, connectionType);
        }

        @io.qt.QtUninvokable
        private synchronized <Slot extends Serializable> QMetaObject.Connection addConnectionToSlotObject(SlotConnectionFactory<Slot> factory, Slot slotObject, Qt.ConnectionType[] connectionType) {
            Object lambdaOwner = null;
            Class<?> lambdaOwnerClass = null;
			int lambdaHashCode = 0;
			SerializedLambda serializedLambda = QtJambiInternal.serializeLambdaExpression(slotObject);
            if(serializedLambda!=null){
				MethodHandle methodHandle = QtJambiInternal.lambdaSlotHandles(slotObject.getClass(), serializedLambda);
				Method reflectiveMethod = null;
				if(methodHandle!=null){
					try{
						reflectiveMethod = MethodHandles.reflectAs(Method.class, methodHandle);
						if(methodHandle.isVarargsCollector()) {
							methodHandle = methodHandle.asFixedArity();
						}
					} catch (Throwable e1) {
						logger.log(java.util.logging.Level.WARNING, "Exception caught while analyzing slot", e1);
					}
				}
				
				if(reflectiveMethod!=null && serializedLambda!=null && methodHandle!=null){
					Object arg1;
					if(Modifier.isStatic(reflectiveMethod.getModifiers())){
						Object[] lambdaArgs = null;
						Object owner = null;
						if(serializedLambda.getCapturedArgCount()>0){
							lambdaArgs = new Object[serializedLambda.getCapturedArgCount()];
							for(int i=0; i<serializedLambda.getCapturedArgCount(); i++) {
								lambdaArgs[i] = serializedLambda.getCapturedArg(i);
								if(owner==null && serializedLambda.getCapturedArg(i) instanceof QObject){
									owner = serializedLambda.getCapturedArg(i);
								}
					        	checkConnection(lambdaArgs[i], true);
							}
						}
						return addConnectionToMethod(owner, reflectiveMethod, methodHandle, lambdaArgs, false, true, connectionType);
					}else if(serializedLambda.getCapturedArgCount()>0
							&& (arg1 = serializedLambda.getCapturedArg(0))!=null
							&& reflectiveMethod.getDeclaringClass().isInstance(arg1)){
						String className = arg1.getClass().getName();
						if(!(arg1.getClass().isSynthetic() && className.contains("Lambda$") && className.contains("/"))){
							if (reflectiveMethod.isAnnotationPresent(QtUninvokable.class)) {
								QMetaMethod method = QMetaMethod.fromReflectedMethod(reflectiveMethod);
				            	if(method.isValid()) {
				            		reflectiveMethod = method.toReflectedMethod();
				            	}else {
				            		throw new QUninvokableSlotException(reflectiveMethod);
				            	}
							}
							Object[] lambdaArgs = null;
							if(serializedLambda.getCapturedArgCount()>1){
								lambdaArgs = new Object[serializedLambda.getCapturedArgCount()-1];
								for(int i=1; i<serializedLambda.getCapturedArgCount(); i++) {
									lambdaArgs[i-1] = serializedLambda.getCapturedArg(i);
								}
							}
							return addConnectionToMethod(arg1, reflectiveMethod, methodHandle, lambdaArgs, false, true, connectionType);
						}else{
							try {
								lambdaOwnerClass = Class.forName(serializedLambda.getCapturingClass().replace('/', '.'));
							} catch (Throwable e) {
							}
						}
					}else{
						try {
							lambdaOwnerClass = Class.forName(serializedLambda.getCapturingClass().replace('/', '.'));
						} catch (Throwable e) {
						}
					}
				}else{
					String[] split = slotObject.getClass().getName().split("Lambda\\$");
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
						lambdaOwner = QtJambiInternal.fetchField(slotObject, slotObject.getClass(), "arg$1", null);
					} catch (Throwable e) {
					}
	        		checkConnection(lambdaOwner, true);
				}
				
				lambdaHashCode = hashcode(serializedLambda);
				if(serializedLambda.getCapturedArgCount()>0){
					lambdaOwner = serializedLambda.getCapturedArg(0);
					if(!(lambdaOwner instanceof QtThreadAffineInterface) && !(lambdaOwner instanceof QtJambiSignals.AbstractSignal)){
						for(int i=1; i<serializedLambda.getCapturedArgCount(); i++) {
							Object arg = serializedLambda.getCapturedArg(i);
							if(arg instanceof QtThreadAffineInterface){
								lambdaOwner = arg;
								break;
							}
						}
					}
				}
				if(lambdaOwner==null){
					lambdaOwner = lambdaOwnerClass;
				}
			}
            
            return this.core.addConnectionToSlotObject(this, factory, lambdaOwner, lambdaOwnerClass, lambdaHashCode, slotObject, connectionType);
        }

        @io.qt.QtUninvokable
		protected synchronized boolean removeConnectionToSlotObject(Serializable slotObject) {
            if (inDisconnect())
                return false;  // Why might we re-enter ?  Why are we not reentrant ?
			setInDisconnect(true);
			try{
				SerializedLambda _serializedLambda = QtJambiInternal.serializeLambdaExpression(slotObject);
				Class<?> _lambdaOwnerClass = null;
				Class<?> _functionalInterfaceClass = null;
	            if(_serializedLambda!=null){
            		try {
						_lambdaOwnerClass = slotObject.getClass().getClassLoader().loadClass(_serializedLambda.getCapturingClass().replace('/', '.'));
						_functionalInterfaceClass = slotObject.getClass().getClassLoader().loadClass(_serializedLambda.getFunctionalInterfaceClass().replace('/', '.'));
					} catch (Throwable e) {
					}
	            	MethodHandle methodHandle = QtJambiInternal.lambdaSlotHandles(slotObject.getClass(), _serializedLambda);
					Method reflectiveMethod = null;
					if(methodHandle!=null){
						reflectiveMethod = MethodHandles.reflectAs(Method.class, methodHandle);
						if(methodHandle.isVarargsCollector()) {
							methodHandle = methodHandle.asFixedArity();
						}
					}
					Object _lambdaOwner = null;
					if(reflectiveMethod!=null && _serializedLambda!=null && methodHandle!=null){
						if(Modifier.isStatic(reflectiveMethod.getModifiers())){
							setInDisconnect(false);
							Object[] lambdaArgs = null;
							if(_serializedLambda.getCapturedArgCount()>0){
								lambdaArgs = new Object[_serializedLambda.getCapturedArgCount()];
								for(int i=0; i<_serializedLambda.getCapturedArgCount(); i++) {
									lambdaArgs[i] = _serializedLambda.getCapturedArg(i);
								}
							}
							try{
								if(removeConnectionToMethod(null, reflectiveMethod, lambdaArgs, true))
									return true;
							}finally {
								setInDisconnect(true);
							}
						}else if(_serializedLambda.getCapturedArgCount()>0 
								&& reflectiveMethod.getDeclaringClass().isInstance(_serializedLambda.getCapturedArg(0))){
							Object[] lambdaArgs = null;
							if(_serializedLambda.getCapturedArgCount()>1){
								lambdaArgs = new Object[_serializedLambda.getCapturedArgCount()-1];
								for(int i=1; i<_serializedLambda.getCapturedArgCount(); i++) {
									lambdaArgs[i-1] = _serializedLambda.getCapturedArg(i);
								}
							}
							setInDisconnect(false);
							try{
								if(removeConnectionToMethod(_serializedLambda.getCapturedArg(0), reflectiveMethod, lambdaArgs, true))
									return true;
							}finally {
								setInDisconnect(true);
							}
						}
					}

					int _lambdaHashCode = 0;
					if(_serializedLambda!=null){
						_lambdaHashCode = hashcode(_serializedLambda);
						if(_serializedLambda.getCapturedArgCount()>0){
							_lambdaOwner = _serializedLambda.getCapturedArg(0);
							if(!(_lambdaOwner instanceof QtThreadAffineInterface) || !(_lambdaOwner instanceof QtJambiSignals.AbstractSignal)){
								for(int i=1; i<_serializedLambda.getCapturedArgCount(); i++) {
									Object arg = _serializedLambda.getCapturedArg(i);
									if(arg instanceof QtThreadAffineInterface){
										_lambdaOwner = arg;
										break;
									}
								}
							}
						}
					}
					
					if(_lambdaOwner==null){
						_lambdaOwner = _lambdaOwnerClass;
					}
					return core.removeConnectionToSlotObject(this, slotObject, _functionalInterfaceClass, _lambdaOwner, _lambdaOwnerClass, _lambdaHashCode, true);
				}else{
					return core.removeConnectionToSlotObject(this, slotObject, _functionalInterfaceClass, null, _lambdaOwnerClass, 0, false);
				}
			}finally{
				setInDisconnect(false);
			}
		}

		private static int hashcode(SerializedLambda serializedLambda) {
			if(serializedLambda==null)
				return 0;
			final int prime = 31;
			int result = 1;
			result = prime * result + ((serializedLambda.getCapturingClass() == null) ? 0 : serializedLambda.getCapturingClass().hashCode());
			result = prime * result + ((serializedLambda.getFunctionalInterfaceClass() == null) ? 0 : serializedLambda.getFunctionalInterfaceClass().hashCode());
			result = prime * result + ((serializedLambda.getFunctionalInterfaceMethodName() == null) ? 0 : serializedLambda.getFunctionalInterfaceMethodName().hashCode());
			result = prime * result + ((serializedLambda.getFunctionalInterfaceMethodSignature() == null) ? 0 : serializedLambda.getFunctionalInterfaceMethodSignature().hashCode());
			result = prime * result + ((serializedLambda.getImplClass() == null) ? 0 : serializedLambda.getImplClass().hashCode());
			result = prime * result + serializedLambda.getImplMethodKind();
			result = prime * result + ((serializedLambda.getImplMethodName() == null) ? 0 : serializedLambda.getImplMethodName().hashCode());
			result = prime * result + ((serializedLambda.getImplMethodSignature() == null) ? 0 : serializedLambda.getImplMethodSignature().hashCode());
			result = prime * result + ((serializedLambda.getInstantiatedMethodType() == null) ? 0 : serializedLambda.getInstantiatedMethodType().hashCode());
			result = prime * result + serializedLambda.getCapturedArgCount();
			for(int i=0; i<serializedLambda.getCapturedArgCount(); i++) {
				result = prime * result + ((serializedLambda.getCapturedArg(i) == null) ? 0 : serializedLambda.getCapturedArg(i).hashCode());
			}
			return result;
		}

        @io.qt.QtUninvokable
        protected final boolean removeConnection(Object receiver, String method) {
        	if(receiver instanceof QMetaObject.Connection && method==null){
				return removeConnection((QMetaObject.Connection)receiver);
			}else {
	        	Method slotMethod = null;
		        if (method != null) {
		            slotMethod = lookupSlot(receiver, method, null);
		            if (slotMethod == null) {
		            	QMetaObject metaObject = receiver instanceof QObject ? ((QObject)receiver).metaObject() : QMetaObject.forType(receiver.getClass());
			        	if(metaObject!=null) {
			        		QMetaMethod metaMethod = metaObject.method(method);
			        		if(metaMethod!=null && metaMethod.isValid()) {
			        			if(!matchTypes(metaMethod.parameterClassTypes().toArray(new Class[metaMethod.parameterTypes().size()]))) {
			        				throw new QMisfittingSignatureException("Incompatible sender/receiver arguments " + fullName() + " --> " + metaMethod.methodSignature());
			        			}
			        			return removeConnectionToMethod(receiver, metaMethod, true);
			        		}
			        	}
			        	throw new QNoSuchSlotException(receiver, method);
		            }
		        }
		        return removeConnectionToMethod(receiver, slotMethod, null, true);
			}
        }

        @NativeAccess
        @io.qt.QtUninvokable
        private boolean removeConnectionFromCpp(Object receiver, Method slot) {
        	return removeConnectionToMethod(receiver, slot, null, false);
        }

        @io.qt.QtUninvokable
        synchronized boolean removeConnectionToMethod(Object receiver, Method slot, Object[] lambdaArgs, boolean allowNativeDisconnect) {
			if (inDisconnect())
				return false;  // Why might we re-enter ?  Why are we not reentrant ?
			return core.removeConnectionToMethod(this, receiver, slot, lambdaArgs, allowNativeDisconnect);
		}
        
        @io.qt.QtUninvokable
        synchronized boolean removeConnectionToMethod(Object receiver, QMetaMethod slot, boolean allowNativeDisconnect) {
			if (inDisconnect())
				return false;  // Why might we re-enter ?  Why are we not reentrant ?
			return core.removeConnectionToMethod(this, receiver, slot, allowNativeDisconnect);
		}

        @io.qt.QtUninvokable
		public synchronized boolean removeConnection(QMetaObject.Connection conn) {
            boolean returned = false;
        	if(conn instanceof JavaConnection) {
				JavaConnection javaConnection = (JavaConnection)conn;
				if(javaConnection.signal.get()==this && inDisconnect()) {
					setInDisconnect(true);
					try{
						returned = core.removeConnection(this, javaConnection);
					}finally{
						setInDisconnect(false);
					}
				}
        	}else if(conn instanceof NativeConnection) {
        		NativeConnection nativeConnection = (NativeConnection)conn;
        		if(!nativeConnection.isDisposed() && nativeConnection.isConnected()) {
        			if(nativeConnection.sender()==containingObject() && this.methodIndex()==nativeConnection.signal().methodIndex()) {
						setInDisconnect(true);
	        			try{
	        				returned = core.removeConnection(this, nativeConnection);
	        			}finally {
							setInDisconnect(false);
	        			}
        			}
        		}
        	}
            return returned;
        }

		@Override
		public synchronized String toString() {
			if(core instanceof AnalyzingSignalCoreInterface)
				return super.toString();
			return this.fullName() + "(" + signalParameters() + ")";
		}
    }
    
    protected abstract static class SignalConfiguration{
    	protected SignalConfiguration(AbstractSignal signal, Class<?>... types) {
			super();
			this.signal = signal;
			this.types = types;
		}
    	AbstractSignal signal;
    	Class<?>[] types;
    }
	
    protected static abstract class MultiSignal {
    	private final List<QMetaObject.AbstractSignal> signals;
		private String name;
        private Class<?> declaringClass;

		protected MultiSignal(SignalConfiguration signal1, SignalConfiguration signal2, SignalConfiguration... furtherSignals){
			super();
			List<QMetaObject.AbstractSignal> signals = new ArrayList<>(furtherSignals.length+2);
			for (int i = 0; i < furtherSignals.length+2; i++) {
				SignalConfiguration signalConfiguration;
				if(i==0) {
					signalConfiguration = signal1;
				}else if(i==1) {
					signalConfiguration = signal2;
				}else {
					signalConfiguration = furtherSignals[i-2];
				}
				AbstractSignal signal = signalConfiguration.signal;
				if(signal.core instanceof AnalyzingSignalCore && signalConfiguration.types!=null) {
					List<SignalParameterType> typeList = Collections.emptyList();
		        	for (int j = 0; j < signalConfiguration.types.length; j++) {
		        		int arrayDimension = 0;
		        		Class<?> type = signalConfiguration.types[j];
		        		while (type.isArray()) {
		        			arrayDimension++;
		        			type = type.getComponentType();
		                }
		        		SignalParameterType signalType = new SignalParameterType(signalConfiguration.types[j], type, type, null, arrayDimension, false, false);
		        		if(j==0) {
		        			if(signalConfiguration.types.length>1) {
		        				typeList = new ArrayList<>();
		        				typeList.add(signalType);
		        			}else {
		        				typeList = Collections.singletonList(signalType);
		        			}
		        		}else {
		        			typeList.add(signalType);
		        		}
					}
		        	signal.core = new NamelessJavaSignalCore(signalConfiguration.types.length>1 ? Collections.unmodifiableList(typeList) : typeList);
				}
				signals.add((QMetaObject.AbstractSignal)signalConfiguration.signal);
			}
    		this.signals = Collections.unmodifiableList(signals);
    	}

        @io.qt.QtUninvokable
    	protected final List<QMetaObject.AbstractSignal> signals() {
			return signals;
		}
    	
    	@NativeAccess
        @io.qt.QtUninvokable
    	private final QMetaObject.AbstractSignal signal(int i) {
			return i>=0 && i<signals.size() ? signals.get(i) : null;
		}
    	
    	@NativeAccess
        @io.qt.QtUninvokable
    	private void initializeSignals(Field field, List<Method> methods, int[] methodIndexes) {
			this.name = field.getName();
			this.declaringClass = field.getDeclaringClass();
			boolean isNativeSignal = QtJambiInternal.isGeneratedClass(declaringClass);
			boolean isQObjectSignal = QObject.class.isAssignableFrom(declaringClass);
    		for (int i = 0; i < signals.size(); i++) {
    			Method method = i<methods.size() ? methods.get(i) : null;
    			AbstractSignal signal = signals.get(i);
    			synchronized(signal) {
	    			if(signal.core instanceof NamelessJavaSignalCore) {
		    			List<SignalParameterType> signalTypes = signal.signalTypes();
		    			signal.declaringClass = this.declaringClass;
		    			signal.setNativeSignal(isNativeSignal);
		    			signal.setQObjectSignal(isQObjectSignal);
		    			
		    			if(methodIndexes[i]>=0) {
							if(method!=null) {
				            	MethodHandle slotHandle = null;
				            	try{
			    					slotHandle = QtJambiInternal.privateLookup(method.getDeclaringClass()).unreflect(method);
			    					if(slotHandle.isVarargsCollector()) {
			    						slotHandle = slotHandle.asFixedArity();
			    					}
			    				} catch (Throwable e2) {
			    					logger.log(java.util.logging.Level.WARNING, "Exception caught while analyzing slot "+method, e2);
			    				}
				            	EternalConnection con = new EternalConnection(new WeakReference<Object>(this.containingObject()), method, slotHandle, null);
				            	signal.core = new QObjectsDirectSignalCore(signalTypes, methodIndexes[i], con);
				            }else {
				            	signal.core = new QObjectsSignalCore(signalTypes, methodIndexes[i]);
				            }
						}else {
							signal.core = new JavaSignalCore(this.name, signalTypes);
							if(containingObject()!=null && method!=null) {
								signal.addConnectionToMethod(containingObject(), method, null, null, true, false, Qt.ConnectionType.UniqueConnection);
			                }
						}
	    			}
	    		}
    		}
    	}

    	@SuppressWarnings("unchecked")
        @io.qt.QtUninvokable
		protected <A,B,C,D,E,F,G,H,I> QMetaObject.AbstractPrivateSignal9<A,B,C,D,E,F,G,H,I> overload(Class<A> type1, Class<B> type2, Class<C> type3, Class<D> type4, Class<E> type5, 
    			Class<F> type6, Class<G> type7, Class<H> type8, Class<I> type9) throws QNoSuchSignalException{
    		for(QMetaObject.AbstractSignal signal : signals) {
    			AbstractSignal _signal = signal;
            	if(_signal instanceof QMetaObject.AbstractPrivateSignal9 && _signal.matchTypes(type1, type2, type3, type4, type5, type6, type7, type8, type9)) {
            		return (QMetaObject.AbstractPrivateSignal9<A,B,C,D,E,F,G,H,I>)signal;
            	}
    		}
    		throwMismatchException(type1, type2, type3, type4, type5, type6, type7, type8, type9);
    		return null;
    	}
    	
    	@SuppressWarnings("unchecked")
        @io.qt.QtUninvokable
    	protected <A,B,C,D,E,F,G,H> QMetaObject.AbstractPrivateSignal8<A,B,C,D,E,F,G,H> overload(Class<A> type1, Class<B> type2, Class<C> type3, Class<D> type4, Class<E> type5, 
    			Class<F> type6, Class<G> type7, Class<H> type8) throws QNoSuchSignalException{
    		for(QMetaObject.AbstractSignal signal : signals) {
    			AbstractSignal _signal = signal;
            	if(_signal instanceof QMetaObject.AbstractPrivateSignal8 && _signal.matchTypes(type1, type2, type3, type4, type5, type6, type7, type8)) {
            		return (QMetaObject.AbstractPrivateSignal8<A,B,C,D,E,F,G,H>)signal;
            	}
    		}
    		throwMismatchException(type1, type2, type3, type4, type5, type6, type7, type8);
    		return null;
    	}
    	
    	@SuppressWarnings("unchecked")
        @io.qt.QtUninvokable
		protected <A,B,C,D,E,F,G> QMetaObject.AbstractPrivateSignal7<A,B,C,D,E,F,G> overload(Class<A> type1, Class<B> type2, Class<C> type3, Class<D> type4, Class<E> type5, 
    			Class<F> type6, Class<G> type7) throws QNoSuchSignalException{
    		for(QMetaObject.AbstractSignal signal : signals) {
    			AbstractSignal _signal = signal;
            	if(_signal instanceof QMetaObject.AbstractPrivateSignal7 && _signal.matchTypes(type1, type2, type3, type4, type5, type6, type7)) {
            		return (QMetaObject.AbstractPrivateSignal7<A,B,C,D,E,F,G>)signal;
            	}
    		}
    		throwMismatchException(type1, type2, type3, type4, type5, type6, type7);
    		return null;
    	}
    	
    	@SuppressWarnings("unchecked")
        @io.qt.QtUninvokable
		protected <A,B,C,D,E,F> QMetaObject.AbstractPrivateSignal6<A,B,C,D,E,F> overload(Class<A> type1, Class<B> type2, Class<C> type3, Class<D> type4, Class<E> type5, 
    			Class<F> type6) throws QNoSuchSignalException{
    		for(QMetaObject.AbstractSignal signal : signals) {
    			AbstractSignal _signal = signal;
            	if(_signal instanceof QMetaObject.AbstractPrivateSignal6 && _signal.matchTypes(type1, type2, type3, type4, type5, type6)) {
            		return (QMetaObject.AbstractPrivateSignal6<A,B,C,D,E,F>)signal;
            	}
    		}
    		throwMismatchException(type1, type2, type3, type4, type5, type6);
    		return null;
    	}
    	
    	@SuppressWarnings("unchecked")
        @io.qt.QtUninvokable
		protected <A,B,C,D,E> QMetaObject.AbstractPrivateSignal5<A,B,C,D,E> overload(Class<A> type1, Class<B> type2, Class<C> type3, Class<D> type4, Class<E> type5) throws QNoSuchSignalException{
    		for(QMetaObject.AbstractSignal signal : signals) {
    			AbstractSignal _signal = signal;
            	if(_signal instanceof QMetaObject.AbstractPrivateSignal5 && _signal.matchTypes(type1, type2, type3, type4, type5)) {
            		return (QMetaObject.AbstractPrivateSignal5<A,B,C,D,E>)signal;
            	}
    		}
    		throwMismatchException(type1, type2, type3, type4, type5);
    		return null;
    	}
    	
    	@SuppressWarnings("unchecked")
        @io.qt.QtUninvokable
		protected <A,B,C,D> QMetaObject.AbstractPrivateSignal4<A,B,C,D> overload(Class<A> type1, Class<B> type2, Class<C> type3, Class<D> type4) throws QNoSuchSignalException{
    		for(QMetaObject.AbstractSignal signal : signals) {
    			AbstractSignal _signal = signal;
            	if(_signal instanceof QMetaObject.AbstractPrivateSignal4 && _signal.matchTypes(type1, type2, type3, type4)) {
            		return (QMetaObject.AbstractPrivateSignal4<A,B,C,D>)signal;
            	}
    		}
    		throwMismatchException(type1, type2, type3, type4);
    		return null;
    	}
    	
    	@SuppressWarnings("unchecked")
        @io.qt.QtUninvokable
		protected <A,B,C> QMetaObject.AbstractPrivateSignal3<A,B,C> overload(Class<A> type1, Class<B> type2, Class<C> type3) throws QNoSuchSignalException{
    		for(QMetaObject.AbstractSignal signal : signals) {
    			AbstractSignal _signal = signal;
            	if(_signal instanceof QMetaObject.AbstractPrivateSignal3 && _signal.matchTypes(type1, type2, type3)) {
            		return (QMetaObject.AbstractPrivateSignal3<A,B,C>)signal;
            	}
    		}
    		throwMismatchException(type1, type2, type3);
    		return null;
    	}
    	
    	@SuppressWarnings("unchecked")
        @io.qt.QtUninvokable
		protected <A,B> QMetaObject.AbstractPrivateSignal2<A,B> overload(Class<A> type1, Class<B> type2) throws QNoSuchSignalException{
    		for(QMetaObject.AbstractSignal signal : signals) {
    			AbstractSignal _signal = signal;
            	if(_signal instanceof QMetaObject.AbstractPrivateSignal2 && _signal.matchTypes(type1, type2)) {
            		return (QMetaObject.AbstractPrivateSignal2<A,B>)signal;
            	}
    		}
    		throwMismatchException(type1, type2);
    		return null;
    	}
    	
    	@SuppressWarnings("unchecked")
        @io.qt.QtUninvokable
		protected <A> QMetaObject.AbstractPrivateSignal1<A> overload(Class<A> type1) throws QNoSuchSignalException{
    		for(QMetaObject.AbstractSignal signal : signals) {
    			AbstractSignal _signal = signal;
            	if(_signal instanceof QMetaObject.AbstractPrivateSignal1 && _signal.matchTypes(type1)) {
            		return (QMetaObject.AbstractPrivateSignal1<A>)signal;
            	}
    		}
    		throwMismatchException(type1);
    		return null;
    	}

        @io.qt.QtUninvokable
    	protected QMetaObject.AbstractPrivateSignal0 overload() throws QNoSuchSignalException{
    		for(QMetaObject.AbstractSignal signal : signals) {
    			AbstractSignal _signal = signal;
            	if(_signal instanceof QMetaObject.AbstractPrivateSignal0 && _signal.matchTypes()) {
            		return (QMetaObject.AbstractPrivateSignal0)signal;
            	}
    		}
    		throwMismatchException();
    		return null;
    	}

        @io.qt.QtUninvokable
	    protected boolean disconnect(QMetaObject.Connection connection) {
	    	boolean result = false;
	    	for(AbstractSignal signal : signals) {
	    		result |= signal.removeConnection(connection);
	    	}
	        return result;
	    }

        @io.qt.QtUninvokable
	    protected boolean disconnectAll() {
	    	boolean result = false;
	    	for(QMetaObject.AbstractSignal signal : signals) {
	    		result |= signal.disconnect();
	    	}
	        return result;
	    }

        @io.qt.QtUninvokable
	    private void throwMismatchException(Class<?>... type) throws QNoSuchSignalException{
	    	String name = this.fullName() + "(";
	    	for (int i = 0; i < type.length; i++) {
				if(i!=0)
					name += ",";
				name += type[i].getName().replace("$", ".");
			}
	    	name += ")";
	    	throw new QNoSuchSignalException(name);
	    }

        /**
         * Returns the object containing this signal
         */
        @io.qt.QtUninvokable
        public QtSignalEmitterInterface containingObject() {
        	return null;
        }

        /**
         * Returns the name of the signal
         * @return The signal name
         */
        @io.qt.QtUninvokable
        public final String name() {
            return name==null ? "unnamed" : name;
        }

        /**
         * Returns the full name of the signal, on the form "package.class.signalName"
         *
         *  @return The fully qualified name of the signal
         */
        @io.qt.QtUninvokable
        public final String fullName() {
        	String name = name();
            return declaringClass == null ? name : declaringClass.getName() + "." + name;
        }
    }
	
	private static class MetaCall implements QMetaObject.Slot0, QtThreadAffineInterface, QtObjectInterface {

		private static final long serialVersionUID = 2770243659499018807L;
		
		private final QtThreadAffineInterface eventReceiver;
		private final AbstractSignal signal;
        private final Object arguments[];
        private final AbstractConnection<?> connection;
        private final QtSignalEmitterInterface signalEmitter;
        private Throwable throwable;
		private final Method slot;

        public MetaCall(QtThreadAffineInterface eventReceiver, AbstractSignal signal, AbstractConnection<?> connection, QtSignalEmitterInterface senderObject, Method slot, Object[] arguments) {
            super();
            this.eventReceiver = eventReceiver;
            this.signal = signal;
            this.arguments = arguments;
            this.connection = connection;
            this.signalEmitter = senderObject;
            this.slot = slot;
        }

        public void throwException() {
        	if(throwable instanceof RuntimeException)
    			throw (RuntimeException)throwable;
    		else if(throwable instanceof Error)
    			throw (Error)throwable;
    		else if(throwable!=null) {
    			if(slot!=null){
                    throw new QSignalInvocationException(String.format("Exception caught in signal %1$s after invoking slot %2$s: sender=%3$s, receiver=%4$s", 
                    		signal.fullName(), 
							slot,
							this.signalEmitter != null ? this.signalEmitter.getClass().getName() : "N/A",
									eventReceiver==null ? "(static)" : eventReceiver
						),throwable);
				}else{
					throw new QSignalInvocationException(String.format("Exception caught in signal %1$s: sender=%2$s, receiver=%3$s", 
							signal.fullName(), 
							this.signalEmitter != null ? this.signalEmitter.getClass().getName() : "N/A",
									eventReceiver==null ? "N/A" : eventReceiver
						),throwable);
				}
    		}
		}

		public void invoke() {
            Object receiver;
            boolean isStatic = false;
            if(connection instanceof SlotObjectConnection){
            	Object lambdaOwner = ((SlotObjectConnection<?>) connection).lambdaOwner();
            	if(lambdaOwner!=null)
            		receiver = lambdaOwner;
            	else
            		receiver = connection.resolveReceiver();
            }else if(connection instanceof ReflectiveConnection){
                ReflectiveConnection methodConnection = (ReflectiveConnection)connection;
                isStatic = Modifier.isStatic(methodConnection.slot.getModifiers());
                receiver = connection.resolveReceiver();
            }else {
            	return;
            }
			if ((receiver == null && isStatic)
					|| (receiver instanceof QtJambiObject && ((QtJambiObject)receiver).isDisposed()) )
				return;
            try (CurrentSenderSetter setter = new CurrentSenderSetter(signalEmitter, receiver)){
            	logger.finest(()->{
            		if(connection instanceof ReflectiveConnection){
                        ReflectiveConnection methodConnection = (ReflectiveConnection)connection;
                        return String.format("Emit signal %1$s: queued invoke %2$s, receiver=%3$s", 
                        		signal.fullName(), 
                        		methodConnection.slot, 
                        		receiver==null ? "(static)" : receiver);
					}else {
						return String.format("Emit signal %1$s: queued invoke slot object.", signal.fullName());
					}
            	});
                connection.invoke(arguments);
			} catch (Throwable e) {
				/*
				logger.log(java.util.logging.Level.SEVERE, e, 
						()->{
							if(connection instanceof ReflectiveConnection){
			                    ReflectiveConnection methodConnection = (ReflectiveConnection)connection;
								return String.format("Exception caught in signal %1$s after invoking slot %2$s in a queued connection: sender=%3$s, receiver=%4$s", 
										signal.fullName(), 
										methodConnection.slot,
										signalEmitter != null ? signalEmitter.getClass().getName() : "N/A",
										receiver!=null ? receiver : "N/A"
									);
							}else{
								return String.format("Exception caught in signal %1$s while executing queued connection: sender=%2$s, receiver=%3$s",
										signal.fullName(), 
										signalEmitter != null ? signalEmitter.getClass().getName() : "N/A",
										receiver!=null ? receiver : "N/A"
									);
							}
						}
					);*/
				throwable = e;
            }
        }

		@Override
		public boolean isDisposed() {
			return eventReceiver instanceof QtObjectInterface && ((QtObjectInterface)eventReceiver).isDisposed();
		}

		@Override
		public QThread thread() {
			return eventReceiver==null ? QThread.currentThread() : eventReceiver.thread();
		}

		@Override
		public void dispose() {
		}
    }
	
	private static class ThrowingMetaCall extends MetaCall{
		
		private static final long serialVersionUID = 4051575621054775749L;

		public ThrowingMetaCall(QtThreadAffineInterface eventReceiver, AbstractSignal signal, AbstractConnection<?> connection,
				QtSignalEmitterInterface senderObject, Method slot, Object[] arguments) {
			super(eventReceiver, signal, connection, senderObject, slot, arguments);
		}

		public void invoke() {
			super.invoke();
			super.throwException();
		}
	}
	
	private static class CurrentSenderSetter implements AutoCloseable{
		public CurrentSenderSetter(QtSignalEmitterInterface signalEmitter, Object resolvedReceiver) {
			super();
			this.receiver = resolvedReceiver;
			this.signalEmitter = signalEmitter;
            if(signalEmitter instanceof QObject && receiver instanceof QObject){
                ptr = setQObjectSender(QtJambiInternal.internalAccess.nativeId((QObject) receiver),
                		QtJambiInternal.internalAccess.nativeId((QObject)signalEmitter));
        	}else {
        		ptr = 0;
            }
		}
		@Override
		public void close() throws Exception {
            if(signalEmitter instanceof QObject && receiver instanceof QObject){
                resetQObjectSender(QtJambiInternal.internalAccess.nativeId((QObject) receiver), ptr);
            }
		}
		private final QtSignalEmitterInterface signalEmitter;
		private final Object receiver;
		private final long ptr;

		/**
		 * Sets the current QObject sender for receiver to sender. A function call to
		 * setQObjectSender must always be followed by a call to resetQObjectSender
		 * where the data returned from set is passed to reset. Package visibility due
		 * to handling of raw memory pointers in long type.
		 * 
		 * @param receiver The receiver QObject
		 * @param sender   The sender QObject
		 * @return A magic data to be used in the following reset call.
		 */
		private native long setQObjectSender(long receiver, long sender);

		/**
		 * Resets the current sender for the object in receiver. Package visibility due
		 * to handling of raw memory pointers in long type.
		 * 
		 * @param receiver The receiver QObject.
		 * @param data     A magic value which must be the return value from the recent
		 *                 setQObjectSender call.
		 */
		private native void resetQObjectSender(long receiver, long data);
	}
	
	@NativeAccess
	static class NativeConnection extends QtObject implements QMetaObject.Connection{
		private NativeConnection(QPrivateConstructor p) {
			super(p);
		}

		@Override
		public boolean isConnected() {
			return isConnected(io.qt.internal.QtJambiInternal.internalAccess.checkedNativeId(this));
		}

		@Override
		public QObject sender() {
			return sender(io.qt.internal.QtJambiInternal.internalAccess.checkedNativeId(this));
		}

		@Override
		public QObject receiver() {
			return receiver(io.qt.internal.QtJambiInternal.internalAccess.checkedNativeId(this));
		}
		
		public QMetaMethod signal() {
			return signal(io.qt.internal.QtJambiInternal.internalAccess.checkedNativeId(this));
		}
		
		public QMetaMethod method() {
			return method(io.qt.internal.QtJambiInternal.internalAccess.checkedNativeId(this));
		}
		
		public boolean isSlotObject() {
			return isSlotObject(io.qt.internal.QtJambiInternal.internalAccess.checkedNativeId(this));
		}
		
		@Override
		public boolean equals(Object other) {
			if(other instanceof NativeConnection) {
				return equals(io.qt.internal.QtJambiInternal.internalAccess.checkedNativeId(this), io.qt.internal.QtJambiInternal.internalAccess.nativeId((NativeConnection)other));
			}
			return super.equals(other);
		}
		
		private static native boolean isConnected(long nativeId);

		private static native QObject sender(long nativeId);

		private static native QObject receiver(long nativeId);

		private static native QMetaMethod signal(long nativeId);

		private static native QMetaMethod method(long nativeId);
		
		private static native boolean isSlotObject(long nativeId);
		
		private static native boolean equals(long nativeId, long otherNativeId);
	}
    
    static class JavaConnection implements QMetaObject.Connection{
		private final WeakReference<AbstractConnection<?>> connection;
		private final WeakReference<AbstractSignal> signal;
		
		JavaConnection(AbstractSignal signal, AbstractConnection<?> connection){
			this.connection = new WeakReference<>(connection);
			this.signal = new WeakReference<>(signal);
		}

		@Override
		public int hashCode() {
			AbstractConnection<?> connection = this.connection.get();
			AbstractSignal signal = this.signal.get();
			final int prime = 31;
			int result = 1;
			result = prime * result + ((connection == null) ? 0 : connection.hashCode());
			result = prime * result + ((signal == null) ? 0 : signal.hashCode());
			return result;
		}

		@Override
		public boolean equals(Object obj) {
			AbstractConnection<?> connection = this.connection.get();
			AbstractSignal signal = this.signal.get();
			if(!(obj instanceof JavaConnection)) {
				return false;
			}
			AbstractConnection<?> connection2 = ((JavaConnection)obj).connection.get();
			AbstractSignal signal2 = ((JavaConnection)obj).signal.get();
			if (connection == null) {
				if (connection2 != null)
					return false;
			} else if (connection!=connection2)
				return false;
			if (signal == null) {
				if (signal2 != null)
					return false;
			} else if (signal!=signal2)
				return false;
			return true;
		}

		@Override
		public boolean isConnected() {
			AbstractConnection<?> connection = this.connection.get();
			return connection!=null && connection.isConnected();
		}

		@Override
		public QtSignalEmitterInterface sender() {
			AbstractSignal signal = this.signal.get();
			return signal==null ? null : signal.containingObject();
		}

		@Override
		public Object receiver() {
			AbstractConnection<?> connection = this.connection.get();
			if(connection instanceof SlotObjectConnection) {
				Object lambdaOwner = ((SlotObjectConnection<?>) connection).lambdaOwner();
				if(lambdaOwner!=null) {
					return lambdaOwner;
				}
			}
			return connection==null ? null : connection.resolveReceiver();
		}

		public AbstractSignal signal() {
			return signal.get();
		}
	}
	
    private static abstract class AbstractConnection<T>{
    	private final static int Disconnected = 0x08;
        public final T receiver;
        private byte flags;
        
        AbstractConnection(T receiver, Qt.ConnectionType... connectionType) {
            this.receiver = receiver;

            byte flags = 0;
            if(connectionType!=null && connectionType.length>0) {
        		for (Qt.ConnectionType c : connectionType) {
        			flags |= c.value();
				}
        	}
            this.flags = flags;
        }
        
        public final boolean isConnected() {
        	return (flags & Disconnected) == Disconnected;
        }
        public final void setDisconnected() {
        	flags |= Disconnected;
        }
        public final boolean isDirectConnection() {
            return ((flags & Qt.ConnectionType.QueuedConnection.value()) == 0) && ((flags & Qt.ConnectionType.DirectConnection.value()) == Qt.ConnectionType.DirectConnection.value());
        }
		public final boolean isQueuedConnection() {
            return ((flags & Qt.ConnectionType.DirectConnection.value()) == 0) && ((flags & Qt.ConnectionType.QueuedConnection.value()) == Qt.ConnectionType.QueuedConnection.value());
        }
        public final boolean isBlockingQueuedConnection() {
            return (flags & (Qt.ConnectionType.QueuedConnection.value() | Qt.ConnectionType.DirectConnection.value())) == (Qt.ConnectionType.QueuedConnection.value() | Qt.ConnectionType.DirectConnection.value());
        }
        public final boolean isAutoConnection() {
            return (flags & (Qt.ConnectionType.QueuedConnection.value() | Qt.ConnectionType.DirectConnection.value())) == 0;
        }
        public abstract Object resolveReceiver();
		
		abstract void invoke(Object[] args) throws Throwable;
		
		@Override
		public boolean equals(Object obj) {
			if (this == obj)
				return true;
			if (obj == null)
				return false;
			if (getClass() != obj.getClass() && !(obj instanceof JavaConnection))
				return false;
			if (obj.hashCode() != hashCode())
				return false;
			return true;
		}
		
		public boolean equals(QMetaObject.Connection connection) {
			if(connection instanceof JavaConnection) {
				JavaConnection javaConnection = (JavaConnection)connection;
				return javaConnection.connection.get()==this;
			}
			return false;
		}
		
		boolean isEternal() {
			return false;
		}
		
		abstract int argumentCount();
    } // protected abstract class QConnection

    /**
     * Contains book holding info about a single connection
     */
    private static class MetaMethodConnection extends AbstractConnection<WeakReference<Object>>{
        public final QMetaMethod slot;
        
        private MetaMethodConnection(WeakReference<Object> receiver, QMetaMethod slot, Qt.ConnectionType... connectionType) {
            super(receiver, connectionType);
            this.slot = slot;
        }
        
		void invoke(Object[] args) throws Throwable{
			Object receiver = resolveReceiver();
			if(receiver instanceof QObject){
				slot.invoke((QObject)receiver, args, Qt.ConnectionType.DirectConnection);
			}else if(receiver!=null){
				slot.invokeOnGadget(receiver, args);
			}
		}
		
		@Override
		public String toString() {
			return "connection to "+this.slot+" ["+resolveReceiver()+"]";
		}

		@Override
		public Object resolveReceiver() {
        	return receiver!=null ? receiver.get() : null;
		}
		
		int argumentCount() {
			return slot.parameterCount();
		}
    } // private class AbstractReflectiveConnection

    /**
     * Contains book holding info about a single connection
     */
    private static abstract class AbstractReflectiveConnection<O> extends AbstractConnection<O>{
        public final Method slot;
        public final MethodHandle slotHandle;
        private List<Supplier<?>> lambdaArgs = null;
        
        private AbstractReflectiveConnection(O receiver, Method slot, MethodHandle slotHandle, List<Supplier<?>> lambdaArgs, Qt.ConnectionType... connectionType) {
            super(receiver, connectionType);
            this.slot = slot;
			this.slotHandle = slotHandle;
			if(lambdaArgs==null)
				lambdaArgs = Collections.emptyList();
			this.lambdaArgs = lambdaArgs;
        }
        
        int argumentCount() {
    		return slot.getParameterCount() - lambdaArgs.size();
		}

    	private byte typeConversionCode(Class<?> cls) {
    		if (cls.isPrimitive()) {
    			if (cls == int.class) {
    				return 'I';
    			} else if (cls == long.class) {
    				return 'J';
    			} else if (cls == short.class) {
    				return 'S';
    			} else if (cls == boolean.class) {
    				return 'Z';
    			} else if (cls == byte.class) {
    				return 'B';
    			} else if (cls == float.class) {
    				return 'F';
    			} else if (cls == double.class) {
    				return 'D';
    			} else if (cls == char.class) {
    				return 'C';
    			} else if (cls == void.class) {
    				return 'V';
    			} else {
    				throw new RuntimeException("Error in conversion to primitive for complex type " + cls.getName());
    			}
    		}else return 'L';
    	}
        
		void invoke(Object[] args) throws Throwable{
			Object receiver = resolveReceiver();
			if(slotHandle!=null){
				Object[] _arguments;
				if(Modifier.isStatic(slot.getModifiers())){
					if(lambdaArgs.isEmpty()){
						_arguments = args;
					}else{
						_arguments = new Object[args.length + lambdaArgs.size()];
						for (int i = 0; i < lambdaArgs.size(); i++) {
							_arguments[i] = lambdaArgs.get(i).get();
						}
						System.arraycopy(args, 0, _arguments, lambdaArgs.size(), args.length);
					}
				}else{
					if(receiver==null)
						return;
					if(lambdaArgs.isEmpty()){
						_arguments = new Object[args.length + 1];
						_arguments[0] = receiver;
						System.arraycopy(args, 0, _arguments, 1, args.length);
					}else{
						_arguments = new Object[args.length + lambdaArgs.size() + 1];
						_arguments[0] = receiver;
						for (int i = 0; i < lambdaArgs.size(); i++) {
							_arguments[i+1] = lambdaArgs.get(i).get();
						}
						System.arraycopy(args, 0, _arguments, lambdaArgs.size() + 1, args.length);
					}
				}
				switch(_arguments.length){
					case 0:
						slotHandle.invoke();
						break;
					case 1:
						slotHandle.invoke(_arguments[0]);
						break;
					case 2:
						slotHandle.invoke(_arguments[0], _arguments[1]);
						break;
					case 3:
						slotHandle.invoke(
								_arguments[0], _arguments[1], _arguments[2]
							);
						break;
					case 4:
						slotHandle.invoke(
								_arguments[0], _arguments[1], _arguments[2],
								_arguments[3]
							);
						break;
					case 5:
						slotHandle.invoke(
								_arguments[0], _arguments[1], _arguments[2],
								_arguments[3], _arguments[4]
							);
						break;
					case 6:
						slotHandle.invoke(
								_arguments[0], _arguments[1], _arguments[2],
								_arguments[3], _arguments[4], _arguments[5]
							);
						break;
					case 7:
						slotHandle.invoke(
								_arguments[0], _arguments[1], _arguments[2],
								_arguments[3], _arguments[4], _arguments[5],
								_arguments[6]
							);
						break;
					case 8:
						slotHandle.invoke(
								_arguments[0], _arguments[1], _arguments[2],
								_arguments[3], _arguments[4], _arguments[5],
								_arguments[6], _arguments[7]
							);
						break;
					case 9:
						slotHandle.invoke(
								_arguments[0], _arguments[1], _arguments[2],
								_arguments[3], _arguments[4], _arguments[5],
								_arguments[6], _arguments[7], _arguments[8]
							);
						break;
					case 10:
						slotHandle.invoke(
								_arguments[0], _arguments[1], _arguments[2],
								_arguments[3], _arguments[4], _arguments[5],
								_arguments[6], _arguments[7], _arguments[8],
								_arguments[9]
							);
						break;
					default:
						slotHandle.asSpreader(Object[].class, _arguments.length).invoke(_arguments);
						break;
				}
			}else{
				if(receiver==null && !Modifier.isStatic(slot.getModifiers()))
					return;
				Object[] _arguments;
				if(lambdaArgs.isEmpty()){
					_arguments = args;
				}else{
					_arguments = new Object[args.length + lambdaArgs.size()];
					for (int i = 0; i < lambdaArgs.size(); i++) {
						_arguments[i] = lambdaArgs.get(i).get();
					}
					System.arraycopy(args, 0, _arguments, lambdaArgs.size(), args.length);
				}
				try{
					slot.invoke(Modifier.isStatic(slot.getModifiers()) ? null : receiver, _arguments);
				} catch (IllegalAccessException e) {
					Class<?>[] slotParameterTypes = slot.getParameterTypes();
					if(lambdaArgs!=null && lambdaArgs.size()>0){
						Class<?>[] _slotParameterTypes = (Class<?>[])new Class[slotParameterTypes.length - lambdaArgs.size()];
						System.arraycopy(slotParameterTypes, lambdaArgs.size(), _slotParameterTypes, 0, slotParameterTypes.length - lambdaArgs.size());
						slotParameterTypes = _slotParameterTypes;
					}
					byte returnType = typeConversionCode(slot.getReturnType());
		            byte[] _convertTypes = new byte[slotParameterTypes.length];
		    		for (int i = 0; i < _convertTypes.length; ++i) {
		    			_convertTypes[i] = typeConversionCode(slotParameterTypes[i]);
		    		}
					if(Modifier.isStatic(slot.getModifiers())) {
						QtJambiInternal.invokeMethod(slot.getDeclaringClass(), slot, true, 
							returnType, _arguments, _convertTypes);
					}else {
						QtJambiInternal.invokeMethod(receiver, slot, false, 
								returnType, _arguments, _convertTypes);
					}
				} catch (InvocationTargetException e) {
					throw e.getTargetException();
				}
			}
		}
		
		@Override
		public String toString() {
			return "connection to "+this.slot+" ["+resolveReceiver()+"]";
		}
    } // private class AbstractReflectiveConnection
    
    private static class ReflectiveConnection extends AbstractReflectiveConnection<Reference<Object>>{
    	
    	private ReflectiveConnection(Reference<Object> receiver, Method slot, MethodHandle slotHandle, List<Supplier<?>> lambdaArgs, Qt.ConnectionType... connectionType) {
    		super(receiver, slot, slotHandle, lambdaArgs, connectionType);
    	}
    	
    	public Object resolveReceiver() {
        	return receiver!=null ? receiver.get() : null;
        }
    }
    
    private static class EternalConnection extends AbstractReflectiveConnection<WeakReference<Object>>{
    	private EternalConnection(WeakReference<Object> receiver, Method slot, MethodHandle slotHandle, List<Supplier<?>> lambdaArgs, Qt.ConnectionType... connectionType) {
    		super(receiver, slot, slotHandle, lambdaArgs, connectionType);
    	}
		
		boolean isEternal() {
			return true;
		}
		
		public Object resolveReceiver() {
			return receiver!=null ? receiver.get() : null;
        }
    }

    private abstract static class SlotObjectConnection<S> extends AbstractConnection<S> {
		private final int lambdaHashCode;
        private final Object lambdaOwner;
        public final Class<?> lambdaOwnerClass;
        
        private SlotObjectConnection(Object lambdaOwner, Class<?> lambdaOwnerClass, int lambdaHashCode, S slotObject, Qt.ConnectionType... connectionType) {
            super(slotObject, connectionType);
			this.lambdaHashCode = lambdaHashCode;
            this.lambdaOwner = lambdaOwner;
            this.lambdaOwnerClass = lambdaOwnerClass;
        }
        
        public S resolveReceiver() {
        	return receiver;
        }
        
        public Object lambdaOwner(){
            return lambdaOwner;
        }
		
		abstract void invoke(Object[] args) throws Throwable;
		
		@Override
		public String toString() {
			return "slot connection to "+this.resolveReceiver();
		}
    } // private class SlotObjectConnection
    
    private static final class GenericSlotObjectConnection extends SlotObjectConnection<GenericSlot>{
    	private GenericSlotObjectConnection(Object lambdaOwner, Class<?> lambdaOwnerClass, int lambdaHashCode, GenericSlot slotObject, Qt.ConnectionType[] connectionType) {
			super(lambdaOwner, lambdaOwnerClass, lambdaHashCode, slotObject, connectionType);
		}

		void invoke(Object[] args) throws Throwable{
    		resolveReceiver().invoke(args[0]);
    	}
		
		int argumentCount() {
    		return 1;
		}
    }
    
    private static final class SlotObjectConnection0 extends SlotObjectConnection<Slot0>{
    	private SlotObjectConnection0(Object lambdaOwner, Class<?> lambdaOwnerClass, int lambdaHashCode, Slot0 slotObject, Qt.ConnectionType[] connectionType) {
			super(lambdaOwner, lambdaOwnerClass, lambdaHashCode, slotObject, connectionType);
		}

		void invoke(Object[] args) throws Throwable{
    		resolveReceiver().invoke();
    	}
		
		int argumentCount() {
    		return 0;        		
		}
    }
    
    @SuppressWarnings("rawtypes")
	private static final class SlotObjectConnection1 extends SlotObjectConnection<Slot1>{
    	private SlotObjectConnection1(Object lambdaOwner, Class<?> lambdaOwnerClass, int lambdaHashCode, Slot1 slotObject, Qt.ConnectionType[] connectionType) {
			super(lambdaOwner, lambdaOwnerClass, lambdaHashCode, slotObject, connectionType);
		}

    	@SuppressWarnings("unchecked")
		void invoke(Object[] args) throws Throwable{
    		resolveReceiver().invoke(args[0]);
    	}
    	
    	int argumentCount() {
    		return 1;
		}
    }
    
    @SuppressWarnings("rawtypes")
    private static final class SlotObjectConnection2 extends SlotObjectConnection<Slot2>{
    	private SlotObjectConnection2(Object lambdaOwner, Class<?> lambdaOwnerClass, int lambdaHashCode, Slot2 slotObject, Qt.ConnectionType[] connectionType) {
			super(lambdaOwner, lambdaOwnerClass, lambdaHashCode, slotObject, connectionType);
		}

		@SuppressWarnings("unchecked")
		void invoke(Object[] args) throws Throwable{
    		resolveReceiver().invoke(
						args[0], args[1]
    				);
    	}
		
		int argumentCount() {
    		return 2;
		}
    }
    
    @SuppressWarnings("rawtypes")
    private static final class SlotObjectConnection3 extends SlotObjectConnection<Slot3>{
    	private SlotObjectConnection3(Object lambdaOwner, Class<?> lambdaOwnerClass, int lambdaHashCode, Slot3 slotObject, Qt.ConnectionType[] connectionType) {
			super(lambdaOwner, lambdaOwnerClass, lambdaHashCode, slotObject, connectionType);
		}

		@SuppressWarnings("unchecked")
		void invoke(Object[] args) throws Throwable{
    		resolveReceiver().invoke(
						args[0], args[1], args[2]
    				);
    	}
		
		int argumentCount() {
    		return 3;
		}
    }
    
    @SuppressWarnings("rawtypes")
    private static final class SlotObjectConnection4 extends SlotObjectConnection<Slot4>{
    	private SlotObjectConnection4(Object lambdaOwner, Class<?> lambdaOwnerClass, int lambdaHashCode, Slot4 slotObject, Qt.ConnectionType[] connectionType) {
			super(lambdaOwner, lambdaOwnerClass, lambdaHashCode, slotObject, connectionType);
		}

		@SuppressWarnings("unchecked")
		void invoke(Object[] args) throws Throwable{
    		resolveReceiver().invoke(
						args[0], args[1], args[2],
						args[3]
    				);
    	}
		
		int argumentCount() {
    		return 4;
		}
    }
    
    @SuppressWarnings("rawtypes")
    private static final class SlotObjectConnection5 extends SlotObjectConnection<Slot5>{
    	private SlotObjectConnection5(Object lambdaOwner, Class<?> lambdaOwnerClass, int lambdaHashCode, Slot5 slotObject, Qt.ConnectionType[] connectionType) {
			super(lambdaOwner, lambdaOwnerClass, lambdaHashCode, slotObject, connectionType);
		}

		@SuppressWarnings("unchecked")
		void invoke(Object[] args) throws Throwable{
    		resolveReceiver().invoke(
						args[0], args[1], args[2],
						args[3], args[4]
    				);
    	}
		
		int argumentCount() {
    		return 5;
		}
    }
    
    @SuppressWarnings("rawtypes")
    private static final class SlotObjectConnection6 extends SlotObjectConnection<Slot6>{
    	private SlotObjectConnection6(Object lambdaOwner, Class<?> lambdaOwnerClass, int lambdaHashCode, Slot6 slotObject, Qt.ConnectionType[] connectionType) {
			super(lambdaOwner, lambdaOwnerClass, lambdaHashCode, slotObject, connectionType);
		}

		@SuppressWarnings("unchecked")
		void invoke(Object[] args) throws Throwable{
    		resolveReceiver().invoke(
						args[0], args[1], args[2],
						args[3], args[4], args[5]
    				);
    	}
		
		int argumentCount() {
    		return 6;
		}
    }
    
    @SuppressWarnings("rawtypes")
    private static final class SlotObjectConnection7 extends SlotObjectConnection<Slot7>{
    	private SlotObjectConnection7(Object lambdaOwner, Class<?> lambdaOwnerClass, int lambdaHashCode, Slot7 slotObject, Qt.ConnectionType[] connectionType) {
			super(lambdaOwner, lambdaOwnerClass, lambdaHashCode, slotObject, connectionType);
		}

		@SuppressWarnings("unchecked")
		void invoke(Object[] args) throws Throwable{
    		resolveReceiver().invoke(
						args[0], args[1], args[2],
						args[3], args[4], args[5], 
						args[6]
    				);
    	}
		
		int argumentCount() {
    		return 7;
		}
    }
    
    @SuppressWarnings("rawtypes")
    private static final class SlotObjectConnection8 extends SlotObjectConnection<Slot8>{
    	private SlotObjectConnection8(Object lambdaOwner, Class<?> lambdaOwnerClass, int lambdaHashCode, Slot8 slotObject, Qt.ConnectionType[] connectionType) {
			super(lambdaOwner, lambdaOwnerClass, lambdaHashCode, slotObject, connectionType);
		}

		@SuppressWarnings("unchecked")
		void invoke(Object[] args) throws Throwable{
    		resolveReceiver().invoke(
						args[0], args[1], args[2],
						args[3], args[4], args[5], 
						args[6], args[7]
    				);
    	}
		
		int argumentCount() {
    		return 8;
		}
    }
    
    @SuppressWarnings("rawtypes")
    private static final class SlotObjectConnection9 extends SlotObjectConnection<Slot9>{
    	private SlotObjectConnection9(Object lambdaOwner, Class<?> lambdaOwnerClass, int lambdaHashCode, Slot9 slotObject, Qt.ConnectionType[] connectionType) {
			super(lambdaOwner, lambdaOwnerClass, lambdaHashCode, slotObject, connectionType);
		}

		@SuppressWarnings("unchecked")
		void invoke(Object[] args) throws Throwable{
    		resolveReceiver().invoke(
						args[0], args[1], args[2],
						args[3], args[4], args[5], 
						args[6], args[7], args[8]
    				);
    	}
		
		int argumentCount() {
    		return 9;
		}
    }
    
    private static boolean isUnique(Qt.ConnectionType... connectionType) {
    	for (Qt.ConnectionType c : connectionType) {
			if(c==Qt.ConnectionType.UniqueConnection)
				return true;
		}
    	return false;
    }

	private static native final Connection connectNative(long senderObjectId, int signal, AbstractConnection<?> connection, int argumentCount, int connectionType);
	
	private static native final Connection connectNativeToMetaMethod(long senderObjectId, int signal, long receiverObjectId, int slot, int connectionType);
	
	private static native final boolean disconnectNative(long senderObjectId, int signal, long receiverObjectId, int slot);

	protected native static void emitNativeSignal(QObject senderNative, int methodIndex, Object args[]);
	
	protected static void registerDisposedSignalFactory(Function<Class<?>,QMetaObject.DisposedSignal> disposedSignalFactory) {
		QtJambiInternal.registerDisposedSignalFactory(disposedSignalFactory);
	}
	
    protected static void checkConnectionToDisposedSignalImpl(QMetaObject.DisposedSignal signal, Object receiver, boolean slotObject) {
    	if(receiver==signal) {
			throw new IllegalArgumentException("Cannot connect a signal to itself.");
		}else if(receiver instanceof QtObjectInterface) {
			QMetaObject.DisposedSignal disposed = QtJambiInternal.getSignalOnDispose((QtObjectInterface)receiver, false);
			if(signal==disposed) {
				if(slotObject) {
					throw new IllegalArgumentException("Cannot connect a lambda expression capturing a certain object to this object's \"disposed\" signal.");
				}else {
					throw new IllegalArgumentException("Cannot connect an object to its own \"disposed\" signal.");
				}
			}
		}
    }
    
    protected static QtJambiPropertyInfo analyzeProperty(QObject object, QtObject property) {
    	return MetaObjectTools.analyzeProperty(object, property);
    }
    
    protected static void registerPropertyField(QMetaProperty metaProperty, java.lang.reflect.Field field) {
    	MetaObjectTools.registerPropertyField(metaProperty, field);
    }
    
    protected static Class<?> signalDeclaringClass(AbstractSignal signal) {
		return signal.getDeclaringClass();
	}
    
	/**
	 * Disconnect all connections made from the given object to a specific object.
	 *
	 * @param sender   The sender to disconnect from receiver
	 * @param receiver The receiver to disconnect, or null to disconnect all
	 *                 receivers
	 */
	protected static boolean disconnectAll(QtSignalEmitterInterface sender, Object receiver) {
		boolean result = false;
		if (sender != null) {
			Class<?> cls = sender.getClass();
			while (cls != null && cls != Object.class) {
				Field fields[] = cls.getDeclaredFields();
				MethodHandles.Lookup lookup = QtJambiInternal.privateLookup(cls);

				for (Field f : fields) {
					if (!java.lang.reflect.Modifier.isStatic(f.getModifiers())
							&& AbstractSignal.class.isAssignableFrom(f.getType())) {
						AbstractSignal signal = (AbstractSignal) QtJambiInternal.fetchField(lookup, sender, f);
						if (signal != null) {
							if (receiver == null) {
								try {
									result |= signal.removeConnectionToMethod(null, null, null, true);
								} catch (io.qt.QNoNativeResourcesException e) {
								}
							} else if (receiver instanceof QMetaObject.Connection) {
								result |= signal.removeConnection((QMetaObject.Connection) receiver);
							} else if (receiver instanceof QMetaObject.AbstractSlot) {
								try {
									result |= signal.removeConnectionToSlotObject((QMetaObject.AbstractSlot) receiver);
								} catch (io.qt.QNoNativeResourcesException e) {
								}
							} else {
								try {
									result |= signal.removeConnectionToMethod(receiver, null, null, true);
								} catch (io.qt.QNoNativeResourcesException e) {
								}
							}
						}
					}
				}

				cls = cls.getSuperclass();
			}
			if (sender instanceof QtObjectInterface) {
				QMetaObject.DisposedSignal disposed = QtJambiInternal.getSignalOnDispose((QtObjectInterface) sender, false);
				if (disposed != null) {
					result |= disposed.disconnect(receiver);
				}
			}
		}
		return result;
	}

	protected static boolean disconnectOne(QMetaObject.Connection connection) {
		if (connection instanceof QtJambiSignals.JavaConnection) {
			QtJambiSignals.JavaConnection javaConnection = (QtJambiSignals.JavaConnection) connection;
			QtJambiSignals.AbstractSignal signal = javaConnection.signal();
			if (signal != null) {
				return signal.removeConnection(connection);
			}
		} else if (connection instanceof QtJambiSignals.NativeConnection) {
			QtJambiSignals.NativeConnection nativeConnection = (QtJambiSignals.NativeConnection) connection;
			long nativeId = QtJambiInternal.internalAccess.nativeId(nativeConnection);
			if (nativeId != 0)
				return disconnectConnection(nativeId);
		}
		return false;
	}

	static native boolean disconnectConnection(long connection__id);
    
	@NativeAccess
	private static Method lookupSlot(Object object, String signature, Class<?>[] requiredParameters) {
		Class<?> cls = object.getClass();

		if (requiredParameters == null) {
			int pos = signature.indexOf('(');
			if (pos < 0) {
				throw new RuntimeException("Wrong syntax in slot signature: '" + signature + "'");
			}
			int spacePos = signature.trim().lastIndexOf(' ', pos);
			if (pos > spacePos && spacePos > 0) {
				throw new RuntimeException("Do not specify return type in slot signature: '" + signature + "'");
			}

			String name = signature.substring(0, pos).trim();

			int pos2 = signature.indexOf(')', pos);
			if (pos2 < 0) {
				throw new RuntimeException("Wrong syntax in slot signature: '" + signature + "'");
			}
			String strTypes = signature.substring(pos + 1, pos2).trim();

			String argumentTypes[] = {};
			if (!strTypes.isEmpty())
				argumentTypes = strTypes.split("\\,");

//            if (strTypes.length() == 0)
//                argumentTypes = new String[0];
//            else
//                argumentTypes = RetroTranslatorHelper.split(strTypes, ",");

			for (int i = 0; i < argumentTypes.length; ++i)
				argumentTypes[i] = argumentTypes[i].replace(" ", "");

			Method result = findFunctionRecursive(cls, name, argumentTypes);
			if (result == null && object instanceof Class) {
				result = findFunctionRecursive((Class<?>) object, name, argumentTypes);
			}
			return result;
		} else {
			Method result = findFunctionRecursive(cls, signature, requiredParameters);
			if (result == null && object instanceof Class) {
				result = findFunctionRecursive((Class<?>) object, signature, requiredParameters);
			}
			return result;
		}
	}

	/**
	 * Generates a hash from arguments
	 */
	private static int argumentsHashCode(Class<?> cls, String functionName, Object argumentTypes[]) {
		final int prime = 31;
		int result = 1;
		result = prime * result + System.identityHashCode(cls);
		result = prime * result + ((functionName == null) ? 0 : functionName.hashCode());
		result = prime * result + argumentTypes.getClass().hashCode();
		result = prime * result + Arrays.deepHashCode(argumentTypes);
		return result;
	}
	
	private static Method findFunctionRecursive(Class<?> cls, String functionName, String argumentTypes[]) {
		Method result = lookupSlots.computeIfAbsent(argumentsHashCode(cls, functionName, argumentTypes),
				argumentsHashCode -> {
					for (Method m : cls.getDeclaredMethods()) {
						boolean found;
						if (!m.getName().equals(functionName))
							continue;

						Class<?> a[] = m.getParameterTypes();
						if (a.length != argumentTypes.length)
							continue;

						found = true;
						for (int i = 0; i < a.length; ++i) {
							String arg = a[i].getName();

							Class<?> t = a[i];

							if (t.isArray()) {
								String brackets = "";

								do {
									t = t.getComponentType();
									brackets += "[]";
								} while (t.isArray());

								arg = t.getName() + brackets;
							}

							if (argumentTypes[i].indexOf('.') < 0) {
								arg = arg.substring(arg.lastIndexOf('.') + 1);
							}

							String argLenient = t.isMemberClass() ? arg.replace('$', '.') : null;
							if (!arg.equals(argumentTypes[i])
									&& (argLenient == null || !argLenient.equals(argumentTypes[i]))) {
								if (m.isVarArgs() && argumentTypes[i].endsWith("...")) {
									if (arg.endsWith("[]")) {
										arg = arg.substring(0, arg.length() - 2) + "...";
										if (argLenient != null && argLenient.endsWith("[]")) {
											argLenient = argLenient.substring(0, argLenient.length() - 2) + "...";
										}
									}
									if (!arg.equals(argumentTypes[i])
											&& (argLenient == null || !argLenient.equals(argumentTypes[i]))) {
										found = false;
										break;
									}
								} else {
									found = false;
									break;
								}
							}
						}

						if (found) {
							return m;
						}
					}
					return null;
				});
		if (result == null) {
			if (cls.getSuperclass() == null)
				return null;
			else
				return findFunctionRecursive(cls.getSuperclass(), functionName, argumentTypes);
		}
		return result;
	}

	private static Method findFunctionRecursive(Class<?> cls, String functionName, Class<?> argumentTypes[]) {
		Method result = lookupSlots.computeIfAbsent(argumentsHashCode(cls, functionName, argumentTypes),
				argumentsHashCode -> {
					for (Method m : cls.getDeclaredMethods()) {
						if (m.getName().equals(functionName) && Arrays.deepEquals(argumentTypes, m.getParameterTypes()))
							return m;
					}
					return null;
				});
		if (result == null) {
			if (cls.getSuperclass() == null)
				return null;
			else
				return findFunctionRecursive(cls.getSuperclass(), functionName, argumentTypes);
		}
		return result;
	}
}
