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
package io.qt.internal;

import java.io.IOException;
import java.io.NotSerializableException;
import java.io.Serializable;
import java.lang.invoke.MethodHandle;
import java.lang.invoke.MethodHandles;
import java.lang.invoke.SerializedLambda;
import java.lang.ref.Reference;
import java.lang.ref.WeakReference;
import java.lang.reflect.AnnotatedElement;
import java.lang.reflect.Constructor;
import java.lang.reflect.Field;
import java.lang.reflect.GenericArrayType;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.lang.reflect.Modifier;
import java.lang.reflect.ParameterizedType;
import java.lang.reflect.Type;
import java.lang.reflect.TypeVariable;
import java.lang.reflect.WildcardType;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.NavigableMap;
import java.util.Objects;
import java.util.Set;
import java.util.concurrent.locks.ReentrantReadWriteLock;
import java.util.function.Consumer;
import java.util.function.Function;
import java.util.function.Supplier;

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
import io.qt.QtPointerType;
import io.qt.QtPrimitiveType;
import io.qt.QtReferenceType;
import io.qt.QtSignalBlockerInterface;
import io.qt.QtSignalEmitterInterface;
import io.qt.QtThreadAffineInterface;
import io.qt.QtUninvokable;
import io.qt.QtUtilities;
import io.qt.core.QByteArray;
import io.qt.core.QDeclarableSignals;
import io.qt.core.QHash;
import io.qt.core.QInstanceMemberSignals;
import io.qt.core.QList;
import io.qt.core.QMap;
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
import io.qt.core.QObject;
import io.qt.core.QPair;
import io.qt.core.QSet;
import io.qt.core.QStaticMemberSignals;
import io.qt.core.QStringList;
import io.qt.core.QThread;
import io.qt.core.Qt;
import io.qt.core.Qt.ConnectionType;

/**
 * @hidden
 */
abstract class SignalUtility {
	static {
		QtJambi_LibraryUtilities.initialize();
	}
	
	private final static String wrapper = "Wrapper";
	private final static String qvariant = "QVariant";
	
	protected SignalUtility() {throw new RuntimeException();}

	private final static java.util.logging.Logger logger = java.util.logging.Logger.getLogger("io.qt.internal");
	
	private static final Map<Integer, Method> lookupSlots = Collections.synchronizedMap(new HashMap<>());

	@NativeAccess
	static final class SignalParameterType{
		public SignalParameterType(Class<?> type, Type genericType, AnnotatedElement annotatedType, boolean isPointer, boolean isReference) {
			super();
			this.genericType = genericType;
			this.annotatedType = annotatedType;
			this.type = type;
			if(annotatedType!=null) {
	        	if(!isReference) {
	        		QtReferenceType referenceType = annotatedType.getAnnotation(QtReferenceType.class);
	        		isReference = referenceType!=null && !referenceType.isConst();
	        	}
	        	if(!isPointer) {
	        		QtPointerType pointerType = annotatedType.getAnnotation(QtPointerType.class);
	        		isPointer = pointerType!=null;
	        	}
        	}
			this.isPointer = isPointer;
			this.isReference = isReference;
		}
		
		@NativeAccess
		final Class<?> type;
		final Type genericType;
		final AnnotatedElement annotatedType;
		final boolean isPointer;
		final boolean isReference;
		
		@Override
		public boolean equals(Object obj) {
			if (this == obj)
				return true;
			if (obj instanceof SignalParameterType) {
				SignalParameterType other = (SignalParameterType) obj;
				if(isPointer==other.isPointer 
						&& isReference==other.isReference) {
					if(type==other.type)
						return true;
				}
				return false;
			}
			return false;
		}
		
		@Override
		public int hashCode() {
			final int prime = 31;
			int result = 1;
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
		abstract void emitSignal(AbstractSignal signal, final Object[] args, Supplier<?>... suppliers);
		abstract QMetaObject.Connection addConnectionToMethod(AbstractSignal signal, Object receiver, Method slot, MethodHandle slotHandle, List<Supplier<?>> lambdaArgs, Qt.ConnectionType... connectionType);
		abstract QMetaObject.Connection addConnectionToMethod(AbstractSignal signal, Object receiver, QMetaMethod slot, Qt.ConnectionType... connectionType);
		abstract <Slot extends Serializable> QMetaObject.Connection addConnectionToSlotObject(AbstractSignal signal, SlotConnectionFactory<Slot> factory, Object lambdaOwner, Class<?> lambdaOwnerClass, int lambdaHash, Slot slotObject, Qt.ConnectionType[] connectionType);
		abstract boolean removeConnectionToMethod(AbstractSignal signal, Object receiver, Method slot, Object[] lambdaArgs, boolean allowNativeDisconnect);
		abstract boolean removeConnectionToMethod(AbstractSignal signal, Object receiver, QMetaMethod slot, boolean allowNativeDisconnect);
		int methodIndex(AbstractSignal signal) { return -1; }
		Class<?> getDeclaringClass(AbstractSignal signal){ return null; }
		abstract boolean removeConnectionToSlotObject(AbstractSignal signal,
				Serializable slotObject, Class<?> _functionalInterfaceClass, Object _lambdaOwner, Class<?> _lambdaOwnerClass,
				int _lambdaHashCode, boolean useLambdaInfo);
		abstract boolean removeConnection(AbstractSignal signal, JavaConnection javaConnection);
		boolean removeConnection(AbstractSignal signal, NativeConnection nativeConnection) {
			if(nativeConnection.receiver()!=null && nativeConnection.method()!=null && !nativeConnection.isSlotObject()) {
				return QObject.disconnect(nativeConnection.sender(), nativeConnection.signal(), nativeConnection.receiver(), nativeConnection.method());
			}else {
				long nativeId = NativeUtility.nativeId(nativeConnection);
				if(nativeId!=0)
					return disconnectConnection(nativeId);
				else return false;
			}
		}
		String toString(AbstractSignal signal) {
			return String.format("%1$s(%2$s)", signal.fullName(), signal.signalParameters());
		}
	}
	
	private interface AnalyzingSignalCoreInterface{
	}
	
	private static class AnalyzingSignalCore extends AbstractSignalCore implements AnalyzingSignalCoreInterface{
		final static AnalyzingSignalCore instance = new AnalyzingSignalCore();
		
		@Override
		final List<SignalParameterType> signalTypes(AbstractSignal signal) {
			signal.resolveSignal();
			return signal.core.signalTypes(signal);
		}

		@Override
		final String name(AbstractSignal signal) {
			signal.resolveSignal();
			return signal.core.name(signal);
		}

		@Override
		final Connection addConnectionToMethod(AbstractSignal signal, Object receiver, Method slot, MethodHandle slotHandle, 
				List<Supplier<?>> lambdaArgs, ConnectionType... connectionType) {
			signal.resolveSignal();
			return signal.core.addConnectionToMethod(signal, receiver, slot, slotHandle, lambdaArgs, connectionType);
		}
		
		@Override
		final QMetaObject.Connection addConnectionToMethod(AbstractSignal signal, Object receiver, QMetaMethod slot, Qt.ConnectionType... connectionType){
			signal.resolveSignal();
			return signal.core.addConnectionToMethod(signal, receiver, slot, connectionType);
		}

		@Override
		final void emitSignal(AbstractSignal signal, Object[] args, Supplier<?>... suppliers) {
			signal.resolveSignal();
			signal.core.emitSignal(signal, args, suppliers);
		}

		@Override
		final <Slot extends Serializable> Connection addConnectionToSlotObject(AbstractSignal signal,
				SlotConnectionFactory<Slot> factory, Object lambdaOwner, Class<?> lambdaOwnerClass, int lambdaHash, Slot slotObject, ConnectionType[] connectionType) {
			signal.resolveSignal();
			return signal.core.addConnectionToSlotObject(signal, factory, lambdaOwner, lambdaOwnerClass, lambdaHash, slotObject, connectionType);
		}
		
		final int methodIndex(AbstractSignal signal) {
			signal.resolveSignal();
			return signal.core.methodIndex(signal);
		}
		
		@Override
		final boolean removeConnectionToMethod(AbstractSignal signal, Object receiver, Method slot, Object[] lambdaArgs,
				boolean allowNativeDisconnect) {
			signal.resolveSignal();
			return signal.core.removeConnectionToMethod(signal, receiver, slot, lambdaArgs, allowNativeDisconnect);
		}
		
		@Override
		final boolean removeConnectionToMethod(AbstractSignal signal, Object receiver, QMetaMethod slot, boolean allowNativeDisconnect) {
			signal.resolveSignal();
			return signal.core.removeConnectionToMethod(signal, receiver, slot, allowNativeDisconnect);
		}

		@Override
		final boolean removeConnectionToSlotObject(AbstractSignal signal,
				Serializable slotObject, Class<?> _functionalInterfaceClass, Object _lambdaOwner, Class<?> _lambdaOwnerClass,
				int _lambdaHashCode, boolean useLambdaInfo) {
			signal.resolveSignal();
			return signal.core.removeConnectionToSlotObject(signal, slotObject, _functionalInterfaceClass, _lambdaOwner, _lambdaOwnerClass, _lambdaHashCode, useLambdaInfo);
		}
		
		@Override
		final boolean removeConnection(AbstractSignal signal, JavaConnection javaConnection) {
			signal.resolveSignal();
			return signal.core.removeConnection(signal, javaConnection);
		}
		
		@Override
		final boolean removeConnection(AbstractSignal signal, NativeConnection nativeConnection) {
			signal.resolveSignal();
			return signal.core.removeConnection(signal, nativeConnection);
		}
		
		final Class<?> getDeclaringClass(AbstractSignal signal){ 
			signal.resolveSignal();
			return signal.core.getDeclaringClass(signal); 
		}
		
		final String toString(AbstractSignal signal) {
			signal.resolveSignal();
			return signal.core.toString(signal); 
		}
	}
	
	private static class AnalyzingCheckingSignalCore extends AnalyzingSignalCore{
		private final Consumer<Object[]> argumentTest;
		AnalyzingCheckingSignalCore(Consumer<Object[]> argumentTest) {
			this.argumentTest = argumentTest;
		}
	}
	
	private static class AnalyzingStaticSignalCore extends AnalyzingSignalCore{
		private final Class<?> declaringClass;

		AnalyzingStaticSignalCore(Class<?> declaringClass) {
			this.declaringClass = declaringClass;
		}

		Class<?> declaringClass() {
			return declaringClass;
		}
	}
	
	private abstract static class AbstractParameterSignalCore extends AbstractSignalCore {
    	private final List<SignalParameterType> signalParameterTypes;
    	
		public AbstractParameterSignalCore(List<SignalParameterType> signalParameterTypes) {
			super();
			this.signalParameterTypes = signalParameterTypes;
		}

		@Override
		List<SignalParameterType> signalTypes(AbstractSignal signal) {
			return signalParameterTypes;
		}		
	}
	
	private abstract static class AbstractQObjectsSignalCore extends AbstractParameterSignalCore {
		private final List<QPair<AbstractConnection<?>,QMetaObject.Connection>> nativeConnectionHandles = new LinkedList<>();
    	
    	public AbstractQObjectsSignalCore(List<SignalParameterType> signalParameterTypes, final int methodIndex, final long metaObjectId) {
			super(signalParameterTypes);
			this.methodIndex = methodIndex;
			this.metaObjectId = metaObjectId;
		}

		final int methodIndex;
		final long metaObjectId;
		
		String toString(AbstractSignal signal) {
			QMetaMethod method = QMetaMethod.fromSignal(signal);
			if(method!=null && method.isValid())
				return method.cppMethodSignature().toString();
			return super.toString(signal);
		}
		
		@Override
		final String name(AbstractSignal signal) {
			if(methodIndex>=0) {
				QMetaMethod method = QMetaMethod.fromSignal(signal);
				if(method!=null && method.isValid())
					return method.name().toString();
			}
			return null;
		}
		
		@Override
		final int methodIndex(AbstractSignal signal) { return methodIndex; }

		@Override
		final Connection addConnectionToMethod(AbstractSignal signal, Object receiver, QMetaMethod slot,
				ConnectionType... connectionType) {
			QObject senderObject = (QObject)signal.containingObject();
			if(receiver instanceof QObject) {
				int flags = 0;
	            if(connectionType!=null && connectionType.length>0) {
	        		for (Qt.ConnectionType ct : connectionType) {
	        			flags |= ct.value();
					}
	        	}
				return connectNativeToMetaMethod(NativeUtility.checkedNativeId(senderObject), methodIndex, metaObjectId, NativeUtility.checkedNativeId((QObject)receiver), NativeUtility.checkedNativeId(slot), flags);
			}else {
				NativeUtility.CleanableReference reference = receiver==null ? null : new NativeUtility.CleanableReference(receiver);
				MetaMethodConnection c = new MetaMethodConnection(reference, slot, connectionType);
				int flags = 0;
	            if(connectionType!=null && connectionType.length>0) {
	        		for (Qt.ConnectionType ct : connectionType) {
	        			flags |= ct.value();
					}
	        	}
	            QMetaObject.Connection connection = connectNative(NativeUtility.checkedNativeId(senderObject), methodIndex, metaObjectId, 0, c, slot.parameterTypes().size(), flags);
				if(connection.isConnected()) {
					QPair<AbstractConnection<?>,QMetaObject.Connection> pair = new QPair<>(c, connection);
					synchronized(nativeConnectionHandles) {
						nativeConnectionHandles.add(pair);
					}
					if(reference!=null) {
						List<QPair<AbstractConnection<?>,QMetaObject.Connection>> nativeConnectionHandles = this.nativeConnectionHandles;
						reference.setCleanable(()->{
							disconnectOne(pair.second);
							synchronized(nativeConnectionHandles) {
								nativeConnectionHandles.remove(pair);
							}
						});
					}
				}
				return connection;
			}
		}

		@Override
		final Connection addConnectionToMethod(AbstractSignal signal, Object receiver, Method slot, MethodHandle slotHandle, 
				List<Supplier<?>> lambdaArgs, ConnectionType... connectionType) {
			QObject senderObject = (QObject)signal.containingObject();
    		if((lambdaArgs==null || lambdaArgs.isEmpty()) 
    		    && (receiver instanceof QObject || 
					( receiver instanceof QMetaObject.AbstractSignal 
						&& slot.getName().equals("emit") 
						&& ((AbstractSignal)receiver).methodIndex()>=0
						&& ((AbstractSignal)receiver).containingObject() instanceof QObject) )) {
        		QObject receiverObject;
        		QMetaMethod slotMethod = null;
        		if(receiver instanceof QObject) {
        			receiverObject = (QObject)receiver;
    				slotMethod = QMetaMethod.fromReflectedMethod(slot);
        		}else {
        			AbstractSignal otherSignal = (AbstractSignal)receiver;
        			receiverObject = (QObject)otherSignal.containingObject();
    				slotMethod = QMetaMethod.fromSignal(otherSignal);
        		}
        		if(slotMethod!=null && slotMethod.isValid()) {
        			int flags = 0;
    	            if(connectionType!=null && connectionType.length>0) {
    	        		for (Qt.ConnectionType ct : connectionType) {
    	        			flags |= ct.value();
    					}
    	        	}
        			try{
        				return connectNativeToMetaMethod(NativeUtility.checkedNativeId(senderObject), methodIndex, metaObjectId, NativeUtility.checkedNativeId(receiverObject), NativeUtility.checkedNativeId(slotMethod), flags);
        			} catch (QMisfittingSignatureException e) {
        			}
        		}
    		}
    		
			if(slotHandle==null){
				try{
					slotHandle = ReflectionUtility.privateLookup(slot.getDeclaringClass()).unreflect(slot);
					if(slotHandle.isVarargsCollector()) {
						slotHandle = slotHandle.asFixedArity();
					}
				} catch (Throwable e2) {
					logger.log(java.util.logging.Level.WARNING, "Exception caught while analyzing slot "+slot, e2);
					return new NonConnection();
				}
			}
			NativeUtility.CleanableReference reference = receiver==null ? null : new NativeUtility.CleanableReference(receiver);
			ReflectiveConnection c = new ReflectiveConnection(reference, slot, slotHandle, lambdaArgs, connectionType);
			int flags = 0;
            if(connectionType!=null && connectionType.length>0) {
        		for (Qt.ConnectionType ct : connectionType) {
        			flags |= ct.value();
				}
        	}
            QMetaObject.Connection connection = connectNative(NativeUtility.checkedNativeId(senderObject), methodIndex, metaObjectId, 
            		receiver instanceof QObject ? NativeUtility.checkedNativeId((QObject)receiver) : 0, 
    				c, slot.getParameterCount()-(lambdaArgs==null ? 0 : lambdaArgs.size()), flags);
			if(connection.isConnected()) {
				QPair<AbstractConnection<?>,QMetaObject.Connection> pair = new QPair<>(c, connection);
				synchronized(nativeConnectionHandles) {
					nativeConnectionHandles.add(pair);
				}
				if(reference!=null) {
					List<QPair<AbstractConnection<?>,QMetaObject.Connection>> nativeConnectionHandles = this.nativeConnectionHandles;
					reference.setCleanable(()->{
						disconnectOne(pair.second);
						synchronized(nativeConnectionHandles) {
							nativeConnectionHandles.remove(pair);
						}
					});
				}
			}
			return connection;
		}
		
		@Override
		final <Slot extends Serializable> Connection addConnectionToSlotObject(AbstractSignal signal,
				SlotConnectionFactory<Slot> factory, Object lambdaOwner, Class<?> lambdaOwnerClass, int lambdaHashCode, Slot slotObject, ConnectionType[] connectionType) {
    		QObject senderObject = (QObject)signal.containingObject();
			SlotObjectConnection<Slot> c = factory.create(lambdaOwner, lambdaOwnerClass, lambdaHashCode, slotObject, connectionType);
			int flags = 0;
            if(connectionType!=null && connectionType.length>0) {
        		for (Qt.ConnectionType ct : connectionType) {
        			flags |= ct.value();
				}
        	}
            QMetaObject.Connection connection = connectNative(NativeUtility.checkedNativeId(senderObject), 
            		methodIndex, metaObjectId, 
            		lambdaOwner instanceof QObject ? NativeUtility.checkedNativeId((QObject)lambdaOwner) : 0,
    				c, -1, flags);
			if(connection.isConnected()) {
				synchronized(nativeConnectionHandles) {
					nativeConnectionHandles.add(new QPair<>(c, connection));
				}
			}
			return connection;
		}
		
		@Override
		final boolean removeConnectionToMethod(AbstractSignal signal, Object receiver, QMetaMethod slot, boolean allowNativeDisconnect) {
			QObject senderObject = (QObject)signal.containingObject();
			if(senderObject==null || senderObject.isDisposed())
				return false;
			boolean success = false;
			if(((receiver == null || receiver instanceof QObject) && slot == null)
					|| (receiver instanceof QObject && slot != null)) {
				success |= disconnectNative(NativeUtility.checkedNativeId(senderObject), methodIndex, metaObjectId, NativeUtility.nativeId((QObject)receiver), NativeUtility.nativeId(slot));
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
		final boolean removeConnectionToMethod(AbstractSignal signal, Object receiver, Method slot, Object[] lambdaArgs, boolean allowNativeDisconnect) {
			QObject senderObject = (QObject)signal.containingObject();
			if(senderObject==null || senderObject.isDisposed())
				return false;
			boolean success = false;
			if((receiver == null || receiver instanceof QObject) && slot == null) {
				success |= disconnectNative(NativeUtility.checkedNativeId(senderObject), methodIndex, metaObjectId, NativeUtility.nativeId((QObject)receiver), 0);
			}
			synchronized(nativeConnectionHandles) {
				List<QPair<AbstractConnection<?>, Connection>> foundPairs = null;
				for(QPair<AbstractConnection<?>, Connection> pair : nativeConnectionHandles) {
					if(pair.first instanceof AbstractReflectiveConnection) {
						AbstractReflectiveConnection rc = (AbstractReflectiveConnection)pair.first;
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
						&& ((AbstractSignal)receiver).isQObjectSignal()) ) {
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
	        			slotMethod = QMetaMethod.fromSignal(otherSignal);
	        		}
        		}
				success |= disconnectNative(NativeUtility.checkedNativeId(senderObject), methodIndex, metaObjectId, NativeUtility.checkedNativeId(receiverObject), NativeUtility.checkedNativeId(slotMethod));
    		}
			return success;
		}
		
		final boolean removeConnectionToSlotObject(AbstractSignal signal,
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
		final boolean removeConnection(AbstractSignal signal, JavaConnection javaConnection) {
			return false;
		};
		
		@Override
		final boolean removeConnection(AbstractSignal signal, NativeConnection nativeConnection) {
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
	
	private static class QObjectsPrivateSignalCore extends AbstractQObjectsSignalCore {
		public QObjectsPrivateSignalCore(List<SignalParameterType> signalParameterTypes, int methodIndex, long metaObjectId) {
			super(signalParameterTypes, methodIndex, metaObjectId);
		}

		@Override
		void emitSignal(AbstractSignal signal, Object[] args, Supplier<?>... suppliers) {
		}
	}
	
	private static class QObjectsSignalCore extends QObjectsPrivateSignalCore {
		public QObjectsSignalCore(Class<?> declaringClass, boolean isNativeSignal, List<SignalParameterType> signalParameterTypes, int methodIndex, long metaObjectId) {
			super(signalParameterTypes, methodIndex, metaObjectId);
			this.isNativeSignal = isNativeSignal;
			this.declaringClass = declaringClass;
		}

		private final boolean isNativeSignal;
		private final Class<?> declaringClass;
		
		Class<?> getDeclaringClass(AbstractSignal signal){ 
			return declaringClass; 
		}

		@Override
		void emitSignal(AbstractSignal signal, Object[] args, Supplier<?>... suppliers) {
			QObject senderObject = (QObject)signal.containingObject();
			logger.finest(()->String.format("Emit native signal %1$s(%2$s)", signal.fullName(), signal.signalParameters()));
        	if(this.methodIndex>=0) {
        		if(suppliers.length>0) {
        			if(isNativeSignal) {
        				emitNativeSignal(senderObject, this.methodIndex, this.metaObjectId, suppliers.length, args);
        			}else {
        				int offset = args.length;
        				args = Arrays.copyOf(args, args.length+suppliers.length);
                		for (int i = 0; i < suppliers.length; i++) {
                			Supplier<?> supplier = suppliers[i];
                			args[offset+i] = supplier==null ? null : supplier.get();
        				}
        				emitNativeSignal(senderObject, this.methodIndex, this.metaObjectId, 0, args);
        			}
        		}else {
        			emitNativeSignal(senderObject, this.methodIndex, this.metaObjectId, 0, args);
        		}
        	}else {
        		throw new RuntimeException("QObject signal without method index"); 
        	}
		}
	}
	
	private static class QObjectsCheckingSignalCore extends QObjectsSignalCore {
		private final Consumer<Object[]> argumentTest;
		
		public QObjectsCheckingSignalCore(Class<?> declaringClass, boolean isNativeSignal,
				List<SignalParameterType> signalParameterTypes, int methodIndex, long metaObjectId, Consumer<Object[]> argumentTest) {
			super(declaringClass, isNativeSignal, signalParameterTypes, methodIndex, metaObjectId);
			this.argumentTest = argumentTest;
		}
		
		@Override
		final void emitSignal(AbstractSignal signal, final Object[] args, Supplier<?>... suppliers) {
			if(suppliers.length>0) {
				int offset = args.length;
				Object[] args2 = Arrays.copyOf(args, args.length+suppliers.length);
        		for (int i = 0; i < suppliers.length; i++) {
        			Supplier<?> supplier = suppliers[i];
        			args2[offset+i] = supplier==null ? null : supplier.get();
				}
        		argumentTest.accept(args2);
			}else {
				argumentTest.accept(args);
			}
			super.emitSignal(signal, args, suppliers);
		}
	}
	
	private static final class MultiSignalAnalyingSignalCore extends AbstractSignalCore implements AnalyzingSignalCoreInterface{
		
		final AbstractMultiSignal<?> multiSignal;
		final Class<?>[] types;

		public MultiSignalAnalyingSignalCore(AbstractMultiSignal<?> multiSignal, Class<?>[] types) {
			super();
			this.multiSignal = multiSignal;
			this.types = types;
		}
		
		@Override
		List<SignalParameterType> signalTypes(AbstractSignal signal) {
			multiSignal.resolveMultiSignal();
			return signal.core.signalTypes(signal);
		}

		@Override
		String name(AbstractSignal signal) {
			return multiSignal.name();
		}
		
		int methodIndex(AbstractSignal signal) { 
			multiSignal.resolveMultiSignal();
			return signal.core.methodIndex(signal);
		}

		@Override
		Connection addConnectionToMethod(AbstractSignal signal, Object receiver, Method slot, MethodHandle slotHandle,
				List<Supplier<?>> lambdaArgs, ConnectionType... connectionType) {
			multiSignal.resolveMultiSignal();
			return signal.core.addConnectionToMethod(signal, receiver, slot, slotHandle, lambdaArgs, connectionType);
		}

		@Override
		void emitSignal(AbstractSignal signal, Object[] args, Supplier<?>... suppliers) {
			multiSignal.resolveMultiSignal();
			signal.core.emitSignal(signal, args, suppliers);
		}

		@Override
		<Slot extends Serializable> Connection addConnectionToSlotObject(AbstractSignal signal,
				SlotConnectionFactory<Slot> factory, Object lambdaOwner, Class<?> lambdaOwnerClass, int lambdaHash, Slot slotObject, ConnectionType[] connectionType) {
			multiSignal.resolveMultiSignal();
			return signal.core.addConnectionToSlotObject(signal, factory, lambdaOwner, lambdaOwnerClass, lambdaHash, slotObject, connectionType);
		}

		@Override
		boolean removeConnectionToMethod(AbstractSignal signal, Object receiver, Method slot, Object[] lambdaArgs,
				boolean allowNativeDisconnect) {
			multiSignal.resolveMultiSignal();
			return signal.core.removeConnectionToMethod(signal, receiver, slot, lambdaArgs, allowNativeDisconnect);
		}
		
		@Override
		boolean removeConnectionToMethod(AbstractSignal signal, Object receiver, QMetaMethod slot, boolean allowNativeDisconnect) {
			multiSignal.resolveMultiSignal();
			return signal.core.removeConnectionToMethod(signal, receiver, slot, allowNativeDisconnect);
		}
		
		boolean removeConnectionToSlotObject(AbstractSignal signal,
				Serializable slotObject, Class<?> _functionalInterfaceClass, Object _lambdaOwner, Class<?> _lambdaOwnerClass,
				int _lambdaHashCode, boolean useLambdaInfo) {
			multiSignal.resolveMultiSignal();
			return signal.core.removeConnectionToSlotObject(signal, slotObject, _functionalInterfaceClass, _lambdaOwner, _lambdaOwnerClass, _lambdaHashCode, useLambdaInfo);
		}
		
		@Override
		boolean removeConnection(AbstractSignal signal, JavaConnection javaConnection) {
			multiSignal.resolveMultiSignal();
			return signal.core.removeConnection(signal, javaConnection);
		}
		
		@Override
		boolean removeConnection(AbstractSignal signal, NativeConnection nativeConnection) {
			multiSignal.resolveMultiSignal();
			return signal.core.removeConnection(signal, nativeConnection);
		}

		@Override
		Connection addConnectionToMethod(AbstractSignal signal, Object receiver, QMetaMethod slot, ConnectionType... connectionType) {
			multiSignal.resolveMultiSignal();
			return signal.core.addConnectionToMethod(signal, receiver, slot, connectionType);
		}
		
		final Class<?> getDeclaringClass(AbstractSignal signal){ 
			multiSignal.resolveMultiSignal();
			return signal.core.getDeclaringClass(signal); 
		}
		
		final String toString(AbstractSignal signal) {
			multiSignal.resolveMultiSignal();
			return signal.core.toString(signal); 
		}
	}
	
	private abstract static class AbstractJavaSignalCore extends AbstractParameterSignalCore {
		public AbstractJavaSignalCore(List<SignalParameterType> signalParameterTypes) {
			super(signalParameterTypes);
		}

		private int currentConnectionId = 0;
		private AbstractConnection<?> firstConnection;
		private AbstractConnection<?> lastConnection;
		private ReentrantReadWriteLock connectionsLock = new ReentrantReadWriteLock();
		
		AbstractConnection<?> next(AbstractConnection<?> current){
			connectionsLock.readLock().lock();
			AbstractConnection<?> next = current.next;
			connectionsLock.readLock().unlock();
			return next;
		}
		
		AbstractConnection<?> begin(){
			connectionsLock.readLock().lock();
			AbstractConnection<?> next = firstConnection;
			connectionsLock.readLock().unlock();
			return next;
		}
		
		AbstractConnection<?> eraseAndNext(AbstractConnection<?> current){
			connectionsLock.writeLock().lock();
			current.setDisconnected();
			if(current==firstConnection) {
				if(current.next==null) {
					firstConnection = null;
					lastConnection = null;
					current = null;
				}else {
					firstConnection = current.next;
					current = current.next;
				}
			}else {
				current.previous.next = current.next;
				if(current.next==null) {
					lastConnection = current.previous;
				}else {
					current.next.previous = current.previous;
				}
				current= current.next;
			}
			connectionsLock.writeLock().unlock();
			return current;
		}
		
		private void addConnection(AbstractConnection<?> connection) {
            connectionsLock.writeLock().lock();
			try{
				connection.id = ++currentConnectionId;
				if(firstConnection==null) {
					firstConnection = connection;
					lastConnection = connection;
				}else {
					connection.previous = lastConnection;
					lastConnection.next = connection;
					lastConnection = connection;
				}
			}finally {
				connectionsLock.writeLock().unlock();
			}
		}
		
		@Override
		void emitSignal(AbstractSignal signal, Object[] args, Supplier<?>... suppliers) {
			QtSignalEmitterInterface senderObject = signal.containingObject();
            Object currentThread = null;
            boolean senderWithAffinity = senderObject instanceof QtThreadAffineInterface 
            		&& ((QtThreadAffineInterface) senderObject).thread() == (currentThread = QThread.currentThread());
        	if ( senderWithAffinity && senderObject instanceof QtSignalBlockerInterface && ((QtSignalBlockerInterface)senderObject).signalsBlocked()) {
            	logger.finest(()->String.format("Blocking signal %1$s", signal.fullName()));
                return;
            }

        	if(suppliers.length>0) {
        		int offset = args.length;
				args = Arrays.copyOf(args, args.length+suppliers.length);
        		for (int i = 0; i < suppliers.length; i++) {
        			Supplier<?> supplier = suppliers[i];
        			args[offset+i] = supplier==null ? null : supplier.get();
				}
        	}

			connectionsLock.readLock().lock();
			AbstractConnection<?> currentConnection = firstConnection;
			int highestConnectionId = lastConnection==null ? 0 : lastConnection.id;
			connectionsLock.readLock().unlock();
			
			while(currentConnection!=null && currentConnection.id<=highestConnectionId){
                // We do a direct connection in three cases:
                // 1. If the connection is explicitly set to be direct
                // 2. If it is automatic and the receiver is not a QObject (no thread() function)
                // 3. If it is automatic, the receiver is a QObject and the sender and receiver
                //    are both in the current thread
                Object receiver;
				Object[] _args = args;
				if(currentConnection.argumentCount()<args.length) {
					_args = Arrays.copyOf(_args, currentConnection.argumentCount());
				}
                boolean isStatic = false;
                if(currentConnection instanceof SlotObjectConnection<?>){
                    Object lambdaOwner = ((SlotObjectConnection<?>) currentConnection).lambdaOwner();
                    if(lambdaOwner!=null)
                    	receiver = lambdaOwner;
                    else
                    	receiver = currentConnection.resolveReceiver();
                }else if(currentConnection instanceof AbstractReflectiveConnection){
                	receiver = currentConnection.resolveReceiver();
                	AbstractReflectiveConnection reflectiveConnection = (AbstractReflectiveConnection)currentConnection;
					isStatic = Modifier.isStatic(reflectiveConnection.slot.getModifiers());
                }else if(currentConnection instanceof MetaMethodConnection){
                	receiver = currentConnection.resolveReceiver();
				}else {
					continue;
				}
                
                if(
					   (receiver == null && !isStatic)
					|| (receiver instanceof QtObjectInterface && NativeUtility.nativeId((QtObjectInterface)receiver) == 0)
				  ){
					currentConnection = eraseAndNext(currentConnection);
					continue;
				}
                boolean directCall = false;
                boolean receiverThreadIsCurrent;
                if(receiver instanceof QtThreadAffineInterface && !currentConnection.isDirectConnection()) {
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
	                	directCall = receiverThreadIsCurrent && currentConnection.isAutoConnection();
                	}
                }else{
                	receiverThreadIsCurrent = true;
                	directCall = true;
                }
                if (directCall) {
            		AbstractConnection<?> c = currentConnection;
                	logger.finest(()->{
						if(c instanceof ReflectiveConnection){
                            ReflectiveConnection methodConnection = (ReflectiveConnection)c;
                            return String.format("Emit signal %1$s: direct invocation of %2$s, receiver=%3$s", 
                            		signal.fullName(), 
                            		methodConnection.slot, 
                            		receiver==null ? "(static)" : ClassAnalyzerUtility.getClass(receiver).getName());
						}else {
							return String.format("Emit signal %1$s: direct invocation of slot object.", signal.fullName());
						}
                	});
					try (CurrentSenderSetter setter = new CurrentSenderSetter(senderObject, receiver)){
                        currentConnection.invoke(_args);
                    } catch (RuntimeException | Error e) {
                    	throw e;
                    } catch (Throwable e) {
                        if(currentConnection instanceof ReflectiveConnection){
                            ReflectiveConnection methodConnection = (ReflectiveConnection)currentConnection;
                            throw new QSignalInvocationException(String.format("Exception caught in signal %1$s after invoking slot %2$s: sender=%3$s, receiver=%4$s", 
                            		signal.fullName(), 
									methodConnection.slot,
									senderObject != null ? ClassAnalyzerUtility.getClass(senderObject).getName() : "N/A",
											receiver==null ? "(static)" : ClassAnalyzerUtility.getClass(receiver).getName()
								),e);
						}else{
							throw new QSignalInvocationException(String.format("Exception caught in signal %1$s: sender=%2$s, receiver=%3$s", 
									signal.fullName(), 
									senderObject != null ? ClassAnalyzerUtility.getClass(senderObject).getName() : "N/A",
											receiver==null ? "N/A" : ClassAnalyzerUtility.getClass(receiver).getName()
								),e);
						}
                    }
                } else {
                    if (receiver instanceof QtThreadAffineInterface) {
                    	QtThreadAffineInterface eventReceiver = (QtThreadAffineInterface) receiver;
                		AbstractConnection<?> c = currentConnection;
                        logger.finest(()->{
							if(c instanceof ReflectiveConnection){
                                ReflectiveConnection methodConnection = (ReflectiveConnection)c;
                                return String.format("Emit signal %1$s: queueing invocation of %2$s, receiver=%3$s", 
                                		signal.fullName(), 
                                		methodConnection.slot, 
                                		ClassAnalyzerUtility.getClass(receiver).getName());
							}else {
								return String.format("Emit signal %1$s: queueing invocation of slot object.", signal.fullName());
							}
                    	});
                        logger.finest(()->"new MetaCall() current thread = "+Thread.currentThread() + "event receiver thread = "+eventReceiver.thread()+" of object "+ClassAnalyzerUtility.getClass(eventReceiver).getName());
                        logger.finest(()->c.isBlockingQueuedConnection() ? "invokeAndWait" : "invokeLater");
                        if(currentConnection.isBlockingQueuedConnection()) {
                        	if(receiverThreadIsCurrent) {
                        		logger.log(java.util.logging.Level.SEVERE, 
            							()->String.format("Qt: Dead lock detected while activating a BlockingQueuedConnection: signal is %1$s, receiver is %2$s", 
            									signal.fullName(), 
            									ClassAnalyzerUtility.getClass(receiver).getName()
            								)
            							);
                        	}
                        	MetaCall metaCall;
                        	if(currentConnection instanceof ReflectiveConnection){
                				ReflectiveConnection methodConnection = (ReflectiveConnection)currentConnection;
                				metaCall = new MetaCall(eventReceiver, signal, currentConnection, senderObject, methodConnection.slot, _args);
                        	}else {
                        		metaCall = new MetaCall(eventReceiver, signal, currentConnection, senderObject, null, _args);
                        	}
                        	QMetaObject.invokeMethod(metaCall, Qt.ConnectionType.BlockingQueuedConnection);
                        	metaCall.throwException();
                        }else {
                        	MetaCall metaCall;
                        	if(currentConnection instanceof ReflectiveConnection){
                				ReflectiveConnection methodConnection = (ReflectiveConnection)currentConnection;
                				metaCall = new ThrowingMetaCall(eventReceiver, signal, currentConnection, senderObject, methodConnection.slot, _args);
                        	}else {
                        		metaCall = new ThrowingMetaCall(eventReceiver, signal, currentConnection, senderObject, null, _args);
                        	}
                        	QMetaObject.invokeMethod(metaCall, Qt.ConnectionType.QueuedConnection);
                        }
                    }else{
						logger.log(java.util.logging.Level.SEVERE, 
							()->String.format("Cannot emit signal %1$s: queued connection to not thread-affine receiver %2$s.", 
									signal.fullName(), 
									receiver==null ? "N/A" : ClassAnalyzerUtility.getClass(receiver).getName()
								)
							);
						currentConnection = next(currentConnection);
						continue;
					}
                }
				currentConnection = next(currentConnection);
            }
		}
		
		@Override
		final Connection addConnectionToMethod(AbstractSignal signal, Object receiver, Method slot, MethodHandle slotHandle, 
				List<Supplier<?>> lambdaArgs, ConnectionType... connectionType) {
			if(isUnique(connectionType)){
				AbstractConnection<?> currentConnection = begin();
				while(currentConnection!=null){
					Object resolvedReceiver = currentConnection.resolveReceiver();
					if(currentConnection instanceof ReflectiveConnection){
						if (resolvedReceiver == receiver
							&& slot.equals(((ReflectiveConnection)currentConnection).slot)) {
							return new NonConnection();
						}
					}
					currentConnection = next(currentConnection);
				}
			}

			if(slotHandle==null){
				try{
					slotHandle = ReflectionUtility.privateLookup(slot.getDeclaringClass()).unreflect(slot);
					if(slotHandle.isVarargsCollector()) {
						slotHandle = slotHandle.asFixedArity();
					}
				} catch (Throwable e2) {
					logger.log(java.util.logging.Level.WARNING, "Exception caught while analyzing slot "+slot, e2);
					return new NonConnection();
				}
			}
			
            ReflectiveConnection conn = new ReflectiveConnection(new WeakReference<Object>(receiver), slot, slotHandle, lambdaArgs, connectionType);
            if(!(receiver instanceof QtThreadAffineInterface) && (conn.isQueuedConnection() || conn.isBlockingQueuedConnection())) {
            	throw new IllegalArgumentException(String.format("Cannot establish %1$s connection to slot of not thread-affine receiver", 
            			conn.isBlockingQueuedConnection() ? "blocking-queued" : "queued"));
            }
            addConnection(conn);
            return new JavaConnection(signal, conn);
		}
		
		@Override
		final <Slot extends Serializable> Connection addConnectionToSlotObject(AbstractSignal signal,
				SlotConnectionFactory<Slot> factory, Object lambdaOwner, Class<?> lambdaOwnerClass, int lambdaHashCode, Slot slotObject, ConnectionType[] connectionType) {
			if(isUnique(connectionType)){
				AbstractConnection<?> currentConnection = begin();
				while(currentConnection!=null){
					Object resolvedReceiver = currentConnection.resolveReceiver();
					if(currentConnection instanceof SlotObjectConnection){
						if (resolvedReceiver == slotObject) {
							return new NonConnection();
						}
					}
					currentConnection = next(currentConnection);
				}
			}
			
            SlotObjectConnection<?> conn = factory.create(new WeakReference<Object>(lambdaOwner), lambdaOwnerClass, lambdaHashCode, slotObject, connectionType);
            if(!(lambdaOwner instanceof QtThreadAffineInterface) && (conn.isQueuedConnection() || conn.isBlockingQueuedConnection())) {
            	throw new IllegalArgumentException(String.format("Cannot establish %1$s connection to slot of not thread-affine receiver", 
            			conn.isBlockingQueuedConnection() ? "blocking-queued" : "queued"));
            }
            addConnection(conn);
            return new JavaConnection(signal, conn);
		}

		@Override
		final boolean removeConnectionToMethod(AbstractSignal signal, Object receiver, QMetaMethod slot, boolean allowNativeDisconnect) {
			boolean returned = false;
			Method slotMethod = null;
			if(slot!=null) {
				switch(slot.methodType()) {
				case Constructor:
					break;
				case Signal:
					break;
				case Method:
				case Slot:
					slotMethod = slot.toReflectedMethod();
					break;
				default:
					break;
				}
			}
			AbstractConnection<?> currentConnection = begin();
			while(currentConnection!=null){
				Object resolvedReceiver = currentConnection.resolveReceiver();
				if(currentConnection instanceof MetaMethodConnection){
					if (resolvedReceiver == null) {  // GCed receiver
						currentConnection = eraseAndNext(currentConnection);
						continue;
					} else if ((receiver == null || resolvedReceiver == receiver)
						&& (slot == null || slot.equals(((MetaMethodConnection)currentConnection).slot))) {
						currentConnection = eraseAndNext(currentConnection);
						returned = true;
						continue;
					}
				}else if(currentConnection instanceof ReflectiveConnection){
					if (resolvedReceiver == null) {  // GCed receiver
						currentConnection = eraseAndNext(currentConnection);
						continue;
					} else if ((receiver == null || resolvedReceiver == receiver)
						&& (slot==null || (slotMethod != null && slotMethod.equals(((ReflectiveConnection)currentConnection).slot)))) {
						currentConnection = eraseAndNext(currentConnection);
						returned = true;
						continue;
					}
				}else if(currentConnection instanceof SlotObjectConnection){
					SlotObjectConnection<?> slotObjectConnection = (SlotObjectConnection<?>)currentConnection;
					Object lambdaOwner = slotObjectConnection.lambdaOwner();
					Class<?> lambdaOwnerClass = slotObjectConnection.lambdaOwnerClass;
					if(lambdaOwner!=null || lambdaOwnerClass!=null){
						if(receiver != null && slot == null){
							if(lambdaOwner == receiver || 
									lambdaOwnerClass == receiver){
								currentConnection = eraseAndNext(currentConnection);
								returned = true;
								continue;
							}
						}else if(receiver == null && slot == null){
							currentConnection = eraseAndNext(currentConnection);
							returned = true;
							continue;
						}else{
							// remove if lambda owner has been disposed
							if(lambdaOwner instanceof NativeUtility.Object && NativeUtility.nativeId((NativeUtility.Object)lambdaOwner)==0){
								currentConnection = eraseAndNext(currentConnection);
								continue;
							}
						}
					}else if(resolvedReceiver == receiver){
						currentConnection = eraseAndNext(currentConnection);
						returned = true;
						continue;
					}
				}
				currentConnection = next(currentConnection);
			}
			return returned;
		}
		
		@Override
		final boolean removeConnectionToMethod(AbstractSignal signal, Object receiver, Method slot, Object[] lambdaArgs, boolean allowNativeDisconnect) {
			boolean returned = false;
			AbstractConnection<?> currentConnection = begin();
			while(currentConnection!=null) {
				Object resolvedReceiver = currentConnection.resolveReceiver();
				if(currentConnection instanceof MetaMethodConnection){
					if (resolvedReceiver == null) {  // GCed receiver
						currentConnection = eraseAndNext(currentConnection);
						continue;
					} else if ((receiver == null || resolvedReceiver == receiver) && slot == null) {
						currentConnection = eraseAndNext(currentConnection);
						returned = true;
						continue;
					}
				}else if(currentConnection instanceof ReflectiveConnection){
					if (resolvedReceiver == null) {  // GCed receiver
						currentConnection = eraseAndNext(currentConnection);
						continue;
					} else if ((receiver == null || resolvedReceiver == receiver)
						&& (slot == null || slot.equals(((ReflectiveConnection)currentConnection).slot))) {
						currentConnection = eraseAndNext(currentConnection);
						returned = true;
						continue;
					}
				}else if(currentConnection instanceof SlotObjectConnection){
					SlotObjectConnection<?> slotObjectConnection = (SlotObjectConnection<?>)currentConnection;
					Object lambdaOwner = slotObjectConnection.lambdaOwner();
					Class<?> lambdaOwnerClass = slotObjectConnection.lambdaOwnerClass;
					if(lambdaOwner!=null || lambdaOwnerClass!=null){
						if(receiver != null && slot == null){
							if(lambdaOwner == receiver || 
									lambdaOwnerClass == receiver){
								currentConnection = eraseAndNext(currentConnection);
								returned = true;
								continue;
							}
						}else if(receiver == null && slot == null){
							currentConnection = eraseAndNext(currentConnection);
							returned = true;
							continue;
						}else{
							// remove if lambda owner has been disposed
							if(lambdaOwner instanceof NativeUtility.Object && NativeUtility.nativeId((NativeUtility.Object)lambdaOwner)==0){
								currentConnection = eraseAndNext(currentConnection);
								continue;
							}
						}
					}else if(resolvedReceiver == receiver){
						currentConnection = eraseAndNext(currentConnection);
						returned = true;
						continue;
					}
				}
				currentConnection = next(currentConnection);
			}
			return returned;
		}
		
		@Override
		final boolean removeConnectionToSlotObject(AbstractSignal signal,
				Serializable slotObject, Class<?> _functionalInterfaceClass, Object _lambdaOwner, Class<?> _lambdaOwnerClass,
				int _lambdaHashCode, boolean useLambdaInfo) {
			boolean returned = true;
			if(useLambdaInfo) {
				AbstractConnection<?> currentConnection = begin();
				while(currentConnection!=null) {
					Object resolvedReceiver = currentConnection.resolveReceiver();
					if(currentConnection instanceof SlotObjectConnection){
						SlotObjectConnection<?> slotObjectConnection = (SlotObjectConnection<?>)currentConnection;
						Object lambdaOwner = slotObjectConnection.lambdaOwner();
						Class<?> lambdaOwnerClass = slotObjectConnection.lambdaOwnerClass;
						if(resolvedReceiver==slotObject || (
								_functionalInterfaceClass!=null 
									&& _functionalInterfaceClass.isInstance(resolvedReceiver) 
									&& _lambdaOwner==lambdaOwner 
									&& _lambdaOwnerClass==lambdaOwnerClass 
									&& _lambdaHashCode==slotObjectConnection.lambdaHashCode
								)){
							currentConnection = eraseAndNext(currentConnection);
							returned = true;
							continue;
						}
					}
					currentConnection = next(currentConnection);
				}
			}else {
				AbstractConnection<?> currentConnection = begin();
				while(currentConnection!=null) {
					Object resolvedReceiver = currentConnection.resolveReceiver();
					if(resolvedReceiver==slotObject){
						currentConnection = eraseAndNext(currentConnection);
						returned = true;
						continue;
					}
					currentConnection = next(currentConnection);
				}
			}
			return returned;
		}
		
		@Override
		final boolean removeConnection(AbstractSignal signal, JavaConnection javaConnection) {
			boolean returned = false;
			AbstractConnection<?> currentConnection = begin();
			while(currentConnection!=null){
				Object resolvedReceiver = currentConnection.resolveReceiver();
				if(currentConnection instanceof ReflectiveConnection){
					if (resolvedReceiver == null   // GCed receiver
						|| currentConnection.equals(javaConnection)) {
						currentConnection = eraseAndNext(currentConnection);
						continue;
					}
				}else if(currentConnection instanceof SlotObjectConnection){
					SlotObjectConnection<?> slotObjectConnection = (SlotObjectConnection<?>)currentConnection;
					Object lambdaOwner = slotObjectConnection.lambdaOwner();
					Class<?> lambdaOwnerClass = slotObjectConnection.lambdaOwnerClass;
					if(lambdaOwner!=null || lambdaOwnerClass!=null){
						if(currentConnection.equals(javaConnection)){
							currentConnection = eraseAndNext(currentConnection);
							returned = true;
							continue;
						}else{
							// remove if lambda owner has been disposed
							if(lambdaOwner instanceof NativeUtility.Object && NativeUtility.nativeId((NativeUtility.Object)lambdaOwner)==0){
								currentConnection = eraseAndNext(currentConnection);
								continue;
							}
						}
					}else if(currentConnection.equals(javaConnection)){
						currentConnection = eraseAndNext(currentConnection);
						returned = true;
						continue;
					}
				}
				currentConnection = next(currentConnection);
			}
			return returned;
		}

		@Override
		final Connection addConnectionToMethod(AbstractSignal signal, Object receiver, QMetaMethod slot,
				ConnectionType... connectionType) {
			Method slotMethod = null;
			switch(slot.methodType()) {
			case Slot:
			case Method:
				try {
					slotMethod = slot.methodType()==QMetaMethod.MethodType.Signal ? null : slot.toReflectedMethod();
				} catch (Throwable e) {}
				break;
			default:
				break;
			}
			AbstractConnection<?> conn;
			if(slotMethod!=null) {
				conn = new ReflectiveConnection(new WeakReference<Object>(receiver), slotMethod, null, null, connectionType);
			}else {
				conn = new MetaMethodConnection(new WeakReference<Object>(receiver), slot, connectionType);
			}
			addConnection(conn);
            return new JavaConnection(signal, conn);
		}
	}
	
	private static class JavaSignalCore extends AbstractJavaSignalCore {
		public JavaSignalCore(Class<?> declaringClass, String name, List<SignalParameterType> signalParameterTypes) {
			super(signalParameterTypes);
			this.name = name;
			this.declaringClass = declaringClass;
		}

		private final String name;
		
		@Override
		final String name(AbstractSignal signal) {
			return name;
		}

		private final Class<?> declaringClass;
		
		Class<?> getDeclaringClass(AbstractSignal signal){ 
			return declaringClass; 
		}
	}
	
	private static class JavaCheckingSignalCore extends JavaSignalCore {
		private final Consumer<Object[]> argumentTest;
		
		public JavaCheckingSignalCore(Class<?> declaringClass, String name, List<SignalParameterType> signalParameterTypes, Consumer<Object[]> argumentTest) {
			super(declaringClass, name, signalParameterTypes);
			this.argumentTest = argumentTest;
		}
		
		@Override
		final void emitSignal(AbstractSignal signal, Object[] args, Supplier<?>... suppliers) {
			if(suppliers.length>0) {
				int offset = args.length;
				args = Arrays.copyOf(args, args.length+suppliers.length);
        		for (int i = 0; i < suppliers.length; i++) {
        			Supplier<?> supplier = suppliers[i];
        			args[offset+i] = supplier==null ? null : supplier.get();
				}
        		suppliers = new Supplier[0];
			}
			argumentTest.accept(args);
			super.emitSignal(signal, args, suppliers);
		}
	}
	
	private static class JavaStaticSignalCore extends JavaSignalCore {
		private JavaStaticSignalCore(String name, List<SignalParameterType> signalParameterTypes, Class<?> declaringClass) {
			super(declaringClass, name, signalParameterTypes);
		}
	}
	
    protected static abstract class AbstractSignal implements QMetaObject.Signal{
    	private AbstractSignalCore core;

    	/**
    	 * Instance member signal constructor
    	 * @see QObject
    	 * @see QInstanceMemberSignals
    	 */
		protected AbstractSignal() {
			this.core = AnalyzingSignalCore.instance;
        }
		
		/**
    	 * Instance member signal constructor
    	 * @see QObject
    	 * @see QInstanceMemberSignals
    	 */
		protected AbstractSignal(Consumer<Object[]> argumentTest) {
			Objects.requireNonNull(argumentTest);
			this.core = new AnalyzingCheckingSignalCore(argumentTest);
        }
		
		/**
		 * Static member signal constructor
		 * @see QStaticMemberSignals
		 */
		protected AbstractSignal(Class<?> declaringClass) {
			this.core = new AnalyzingStaticSignalCore(declaringClass);
        }
		
		/**
		 * Disposed signal constructor
		 * @see QtUtilities#getSignalOnDispose(QtObjectInterface)
		 */
		protected AbstractSignal(Class<?> declaringClass, boolean isDisposed) {
			this.core = new JavaStaticSignalCore("disposed", Collections.emptyList(), declaringClass);
        }
        
		/**
		 * Declarable signal constructor
		 * @see QDeclarableSignals
		 */
        protected AbstractSignal(String name, Class<?>[] types) {
        	name = Objects.requireNonNull(name);
        	List<SignalParameterType> typeList = Collections.emptyList();
        	for (int i = 0; i < types.length; i++) {
        		SignalParameterType signalType = new SignalParameterType(types[i], types[i], null, false, false);
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
        	this.core = new JavaSignalCore(null, name, types.length>1 ? Collections.unmodifiableList(typeList) : typeList);
        }
		
        @io.qt.QtUninvokable
		public final int methodIndex() {
			return core.methodIndex(this);
		}

        @io.qt.QtUninvokable
        protected final Class<?> getDeclaringClass() {
			return core.getDeclaringClass(this);
		}

        @io.qt.QtUninvokable
        final List<SignalParameterType> signalTypes() {
    		return core.signalTypes(this);
		}

        @io.qt.QtUninvokable
        public final List<Class<?>> argumentTypes() {
        	List<SignalParameterType> types = core.signalTypes(this);
        	switch(types.size()){
        	case 0:
        		return Collections.emptyList();
        	case 1:
        		return Collections.singletonList(types.get(0).type);
    		default:
    			List<Class<?>> result = new ArrayList<>();
    			for(SignalParameterType signalType : types) {
    				result.add(signalType.type);
    			}
    			return Collections.unmodifiableList(result);
        	}
		}

        @io.qt.QtUninvokable
        private boolean isStaticSignal() {
			return core instanceof JavaStaticSignalCore;
		}

        @io.qt.QtUninvokable
        private boolean isQObjectSignal() {
			return core instanceof AbstractQObjectsSignalCore;
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
        public final String name() {
            return core.name(this);
        }
        
        /**
         * Returns the full name of the signal, on the form "package.class.signalName"
         *  @return The fully qualified name of the signal
         */
        @io.qt.QtUninvokable
        public final String fullName() {
        	String name = name();
        	Class<?> declaringClass = getDeclaringClass();
            return declaringClass == null ? name : declaringClass.getName() + "." + name;
        }

		@io.qt.QtUninvokable
        protected final QMetaObject.Connection addConnection(Object receiver, String method, Qt.ConnectionType... connectionType) {
        	QMetaObject metaObject = receiver instanceof QObject ? ((QObject)receiver).metaObject() : QMetaObject.forType(ClassAnalyzerUtility.getClass(receiver));
        	Method slotMethod = null;
        	if(metaObject!=null) {
        		QMetaMethod metaMethod = metaObject.method(method);
        		if(metaMethod==null || !metaMethod.isValid()) {
        			slotMethod = lookupSlot(receiver, method, null);
        			if (slotMethod == null) {
        	            throw new QNoSuchSlotException(receiver, method);
        	        }
        			metaMethod = QMetaMethod.fromReflectedMethod(slotMethod);
        		}
        		if(metaMethod!=null && metaMethod.isValid()) {
        			return addConnectionToMethod(receiver, metaMethod, connectionType);
        		}
        	}
        	if (slotMethod == null)
        		slotMethod = lookupSlot(receiver, method, null);
	        if (slotMethod == null) {
	            throw new QNoSuchSlotException(receiver, method);
	        }else {
	        	if (slotMethod.isAnnotationPresent(QtUninvokable.class)) {
            		throw new QUninvokableSlotException(slotMethod);
	            }

	            AnnotatedElement[] annotatedParameterTypes = null;
	            if(ClassAnalyzerUtility.useAnnotatedType)
	            	annotatedParameterTypes = slotMethod.getAnnotatedParameterTypes();
	        	Match match = matchMethodTypes(slotMethod.getParameterTypes(), slotMethod.getGenericParameterTypes(), annotatedParameterTypes);
				if(match==Match.NoMatch) {
					QMetaMethod signal = QMetaMethod.fromSignal(this);
					if(signal!=null && signal.isValid()) {
						throw new QMisfittingSignatureException(String.format("Incompatible sender/receiver arguments %1$s -> %2$s", signal.cppMethodSignature(), slotMethod.toGenericString()));
					}
					throw new QMisfittingSignatureException(String.format("Incompatible sender/receiver arguments %1$s(%2$s) -> %3$s", fullName(), signalParameters(), slotMethod.toGenericString()));
				}
	            return addConnectionToMethod(receiver, slotMethod, null, null, connectionType);
	        }
        }

        @io.qt.QtUninvokable
        protected final QMetaObject.Connection addConnection(Object receiver, Method slotMethod, Qt.ConnectionType... connectionType) {
            if (slotMethod.isAnnotationPresent(QtUninvokable.class)) {
            	QMetaMethod method = QMetaMethod.fromReflectedMethod(slotMethod);
            	if(method!=null && method.isValid() && method.methodType()!=QMetaMethod.MethodType.Constructor) {
            		return addConnectionToMethod(receiver, method, connectionType);
            	}else {
            		throw new QUninvokableSlotException(slotMethod);
            	}
            }

            AnnotatedElement[] annotatedParameterTypes = null;
            if(ClassAnalyzerUtility.useAnnotatedType)
            	annotatedParameterTypes = slotMethod.getAnnotatedParameterTypes();
			if(matchMethodTypes(slotMethod.getParameterTypes(), slotMethod.getGenericParameterTypes(), annotatedParameterTypes)==Match.NoMatch) {
				QMetaMethod signal = QMetaMethod.fromSignal(this);
				if(signal!=null && signal.isValid()) {
					QMetaMethod method = QMetaMethod.fromReflectedMethod(slotMethod);
					if(method!=null && method.isValid()) {
						throw new QMisfittingSignatureException(String.format("Incompatible sender/receiver arguments %1$s -> %2$s", signal.cppMethodSignature(), method.cppMethodSignature()));
					}
					throw new QMisfittingSignatureException(String.format("Incompatible sender/receiver arguments %1$s -> %2$s", signal.cppMethodSignature(), slotMethod.toGenericString()));
				}
				throw new QMisfittingSignatureException(String.format("Incompatible sender/receiver arguments %1$s(%2$s) -> %3$s", fullName(), signalParameters(), slotMethod.toGenericString()));
			}
            return addConnectionToMethod(receiver, slotMethod, null, null, connectionType);
        }
        
        @io.qt.QtUninvokable
        private synchronized void resolveSignal() {
            if (core instanceof AnalyzingSignalCoreInterface) {
                boolean found = false;
                if(core instanceof AnalyzingStaticSignalCore) {
                	AnalyzingStaticSignalCore staticCore = (AnalyzingStaticSignalCore)core;
                	Field fields[] = staticCore.declaringClass().getDeclaredFields();
                    for (Field field : fields) {
                        if (AbstractSignal.class.isAssignableFrom(field.getType())) {
                            AbstractSignal sig;
							try {
								sig = (AbstractSignal)ReflectionUtility.readField(null, field);
							} catch (Exception e) {
								sig = null;
							}
                            if (sig == this) {
                            	if(!java.lang.reflect.Modifier.isStatic(field.getModifiers()))
                            		throw new QSignalDeclarationException(String.format("Missing modifier 'static' at signal %1$s.%2$s.", field.getDeclaringClass().getSimpleName(), field.getName()));
                            	if (!Modifier.isFinal(field.getModifiers())) {
		                    		if(!Boolean.getBoolean("qtjambi.allow-nonfinal-signals") && !Boolean.getBoolean("io.qt.allow-nonfinal-signals")) {
		                    			java.util.logging.Logger.getLogger("io.qt.internal").severe(String.format("Missing modifier 'final' at signal %1$s.%2$s. Specify JVM argument -Dqtjambi.allow-nonfinal-signals=true to disable this error.", field.getDeclaringClass().getSimpleName(), field.getName()));
		                    			throw new QSignalDeclarationException(String.format("Missing modifier 'final' at signal %1$s.%2$s.", field.getDeclaringClass().getSimpleName(), field.getName()));
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
                	Class<?> cls = containingObject==null ? null : ClassAnalyzerUtility.getClass(containingObject);
                    loop: while (cls != null) {
                        Field fields[] = cls.getDeclaredFields();
                        for (Field field : fields) {
                            if (AbstractSignal.class.isAssignableFrom(field.getType())) {
                            	AbstractSignal sig = (AbstractSignal)ReflectionUtility.readField(containingObject, field);
                                if (sig == this) {
                                	if(java.lang.reflect.Modifier.isStatic(field.getModifiers()))
                                		throw new QSignalDeclarationException(String.format("Modifier 'static' not allowed for signal %1$s.%2$s. Use QStaticMemberSignals instead to declare a static signal.", cls.getSimpleName(), field.getName()));
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
                    	if(QObject.class.isAssignableFrom(ClassAnalyzerUtility.getClass(this).getDeclaringClass()))
                    		throw new QSignalDeclarationException("Signals must be declared as members of QObject subclasses.");
                    	else if(QInstanceMemberSignals.class.isAssignableFrom(ClassAnalyzerUtility.getClass(this).getDeclaringClass()))
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
        private synchronized void initializeSignal(Class<?> declaringClass, List<SignalParameterType> signalTypes, final int methodIndex, final long metaObjectId) {
        	if(methodIndex>=0 && core instanceof AnalyzingSignalCoreInterface) {
        		if(core instanceof AnalyzingCheckingSignalCore) {
    				Consumer<Object[]> argumentTest = ((AnalyzingCheckingSignalCore)core).argumentTest;
    				this.core = new QObjectsCheckingSignalCore(declaringClass, true, signalTypes, methodIndex, metaObjectId, argumentTest);
        		}else {
        			this.core = new QObjectsSignalCore(declaringClass, true, signalTypes, methodIndex, metaObjectId);
        		}
        	}
		}
        
        private static final Map<Field,List<SignalParameterType>> signalTypesByField = Collections.synchronizedMap(new HashMap<>());
        
        @io.qt.QtUninvokable
        private synchronized void analyzeSignalField(Field field) {
        	if(core instanceof AnalyzingSignalCoreInterface) {
        		Class<?> declaringClass = field.getDeclaringClass();
	            String name = field.getName();
	            List<SignalParameterType> signalTypes;
	            if(QObject.class.isAssignableFrom(declaringClass)) {
            		QMetaObject metaObject = QMetaObject.forType(declaringClass);
            		SignalInfo signalInfo = SignalUtility.signalInfo(containingObject(), metaObject, field, null);
            		if(signalInfo.methodIndex>=0 && signalInfo.enclosingMetaObject!=0) {
            			boolean isGenerated = ClassAnalyzerUtility.isGeneratedClass(declaringClass);
            			if(core instanceof AnalyzingCheckingSignalCore) {
            				Consumer<Object[]> argumentTest = ((AnalyzingCheckingSignalCore)core).argumentTest;
            				if(signalInfo.signalTypes!=null) {
                				this.core = new QObjectsCheckingSignalCore(declaringClass, isGenerated, signalInfo.signalTypes, signalInfo.methodIndex, signalInfo.enclosingMetaObject, argumentTest);
                			}else {
                				signalTypes = signalTypesByField.computeIfAbsent(field, SignalUtility::resolveSignal);
                				this.core = new QObjectsCheckingSignalCore(declaringClass, isGenerated, signalTypes, signalInfo.methodIndex, signalInfo.enclosingMetaObject, argumentTest);
                			}
            			}else {
            				if(signalInfo.signalTypes!=null) {
                				this.core = new QObjectsSignalCore(declaringClass, isGenerated, signalInfo.signalTypes, signalInfo.methodIndex, signalInfo.enclosingMetaObject);
                			}else {
                				signalTypes = signalTypesByField.computeIfAbsent(field, SignalUtility::resolveSignal);
                				this.core = new QObjectsSignalCore(declaringClass, isGenerated, signalTypes, signalInfo.methodIndex, signalInfo.enclosingMetaObject);
                			}
            			}
            			return;
            		}
            	}
            	signalTypes = signalTypesByField.computeIfAbsent(field, SignalUtility::resolveSignal);
            	if(core instanceof AnalyzingCheckingSignalCore) {
    				Consumer<Object[]> argumentTest = ((AnalyzingCheckingSignalCore)core).argumentTest;
    				core = new JavaCheckingSignalCore(declaringClass, name, signalTypes, argumentTest);
            	}else {
            		core = new JavaSignalCore(declaringClass, name, signalTypes);
            	}
        	}
		}
        
        @io.qt.QtUninvokable
        synchronized void setCore(Class<?> declaringClass, String name, List<SignalParameterType> signalTypes, int methodIndex, long metaObjectId) {
        	if(core instanceof AnalyzingSignalCoreInterface) {
                final boolean isQObjectSignal = QObject.class.isAssignableFrom(declaringClass);
                final boolean isNativeSignal = ClassAnalyzerUtility.isGeneratedClass(declaringClass);
                if(core instanceof AnalyzingCheckingSignalCore) {
    				Consumer<Object[]> argumentTest = ((AnalyzingCheckingSignalCore)core).argumentTest;
    				if(methodIndex>=0 && signalTypes!=null && isQObjectSignal) {
    	        		core = new QObjectsCheckingSignalCore(declaringClass, isNativeSignal, signalTypes, methodIndex, metaObjectId, argumentTest);
    	        	}else {
    	        		core = new JavaCheckingSignalCore(declaringClass, name, signalTypes, argumentTest);
    	        	}
                }else {
                	if(methodIndex>=0 && signalTypes!=null && isQObjectSignal) {
    	        		core = new QObjectsSignalCore(declaringClass, isNativeSignal, signalTypes, methodIndex, metaObjectId);
    	        	}else {
    	        		core = new JavaSignalCore(declaringClass, name, signalTypes);
    	        	}
                }
        	}
        }

        @io.qt.QtUninvokable
        private String signalParameters() {
        	List<String> args = new ArrayList<>();
        	for (SignalUtility.SignalParameterType type : signalTypes()) {
        		if(type.genericType!=null)
        			args.add(type.genericType.getTypeName());
        		else
        			args.add(type.type.getName());
        	}
        	return String.join(", ", args);
        }
        
        @io.qt.QtUninvokable
        protected final void emitDefaultSignal(Supplier<?> arg1Default, final Object ... args) {
        	core.emitSignal(this, args, arg1Default);
        }
        
        @io.qt.QtUninvokable
        protected final void emitDefaultSignal(Supplier<?> arg1Default, Supplier<?> arg2Default, final Object ... args) {
        	core.emitSignal(this, args, arg1Default, arg2Default);
        }
        
        @io.qt.QtUninvokable
        protected final void emitDefaultSignal(Supplier<?> arg1Default, Supplier<?> arg2Default, Supplier<?> arg3Default, 
        		final Object ... args) {
        	core.emitSignal(this, args, arg1Default, arg2Default, arg3Default);
        }
        
        @io.qt.QtUninvokable
        protected final void emitDefaultSignal(Supplier<?> arg1Default, Supplier<?> arg2Default, Supplier<?> arg3Default, 
        		Supplier<?> arg4Default, final Object ... args) {
        	core.emitSignal(this, args, arg1Default, arg2Default, arg3Default, 
												arg4Default);
        }
        
        @io.qt.QtUninvokable
        protected final void emitDefaultSignal(Supplier<?> arg1Default, Supplier<?> arg2Default, Supplier<?> arg3Default, 
        		Supplier<?> arg4Default, Supplier<?> arg5Default, final Object ... args) {
        	core.emitSignal(this, args, arg1Default, arg2Default, arg3Default, 
        										arg4Default, arg5Default);
        }
        
        @io.qt.QtUninvokable
        protected final void emitDefaultSignal(Supplier<?> arg1Default, Supplier<?> arg2Default, Supplier<?> arg3Default, 
        		Supplier<?> arg4Default, Supplier<?> arg5Default, Supplier<?> arg6Default, 
        		final Object ... args) {
        	core.emitSignal(this, args, arg1Default, arg2Default, arg3Default, 
	        									arg4Default, arg5Default, arg6Default);
        }
        
        @io.qt.QtUninvokable
        protected final void emitDefaultSignal(Supplier<?> arg1Default, Supplier<?> arg2Default, Supplier<?> arg3Default, 
        		Supplier<?> arg4Default, Supplier<?> arg5Default, Supplier<?> arg6Default, 
        		Supplier<?> arg7Default, final Object ... args) {
        	core.emitSignal(this, args, arg1Default, arg2Default, arg3Default, 
								        		arg4Default, arg5Default, arg6Default, 
								        		arg7Default);
        }
        
        @io.qt.QtUninvokable
        protected final void emitDefaultSignal(Supplier<?> arg1Default, Supplier<?> arg2Default, Supplier<?> arg3Default, 
        		Supplier<?> arg4Default, Supplier<?> arg5Default, Supplier<?> arg6Default, 
        		Supplier<?> arg7Default, Supplier<?> arg8Default, final Object ... args) {
        	core.emitSignal(this, args, arg1Default, arg2Default, arg3Default, 
								        		arg4Default, arg5Default, arg6Default, 
								        		arg7Default, arg8Default);        	
        }
        
        @io.qt.QtUninvokable
        protected final void emitDefaultSignal(Supplier<?> arg1Default, Supplier<?> arg2Default, Supplier<?> arg3Default, 
        		Supplier<?> arg4Default, Supplier<?> arg5Default, Supplier<?> arg6Default, 
        		Supplier<?> arg7Default, Supplier<?> arg8Default, Supplier<?> arg9Default, final Object ... args) {
        	core.emitSignal(this, args, arg1Default, arg2Default, arg3Default, 
								        		arg4Default, arg5Default, arg6Default, 
								        		arg7Default, arg8Default, arg9Default);
        }

        @io.qt.QtUninvokable
        protected final void emitSignal(final Object ... args) {
        	core.emitSignal(this, args);
        }
        
        private enum Match{
        	NoMatch,
        	JavaMatch,
        	NativeMatch
        }

        @io.qt.QtUninvokable
        final boolean matchTypes(Class<?>... types) {
        	List<SignalParameterType> receivedTypes = Collections.emptyList();
        	for (int i = 0; i < types.length; i++) {
        		SignalParameterType signalType = new SignalParameterType(types[i], types[i], null, false, false);
        		if(i==0) {
        			if(types.length>1) {
        				receivedTypes = new ArrayList<>();
        				receivedTypes.add(signalType);
        			}else {
        				receivedTypes = Collections.singletonList(signalType);
        			}
        		}else {
        			receivedTypes.add(signalType);
        		}
			}
        	List<SignalParameterType> sentTypes = signalTypes();
        	if(sentTypes.equals(receivedTypes)) {
        		return true;
        	}else {
        		String signalClassName = ClassAnalyzerUtility.getClass(this).getName();
        		if(signalClassName.endsWith("Default1")
        				|| signalClassName.endsWith("Default2")
        				|| signalClassName.endsWith("Default3")
        				|| signalClassName.endsWith("Default4")
        				|| signalClassName.endsWith("Default5")
        				|| signalClassName.endsWith("Default6")
        				|| signalClassName.endsWith("Default7")
        				|| signalClassName.endsWith("Default8")
        				|| signalClassName.endsWith("Default9")) {
        			sentTypes = new ArrayList<>(sentTypes);
    				if(signalClassName.endsWith("Default1")) {
    					if(sentTypes.size()>=1) {
    						sentTypes.remove(sentTypes.size()-1);
    						if(sentTypes.equals(receivedTypes))
    			        		return true;
    					}
            		}else if(signalClassName.endsWith("Default2")) {
    					if(sentTypes.size()>=2) {
    						sentTypes.remove(sentTypes.size()-1);
    						if(sentTypes.equals(receivedTypes))
    			        		return true;
    						sentTypes.remove(sentTypes.size()-1);
    						if(sentTypes.equals(receivedTypes))
    			        		return true;
    					}            			
            		}else if(signalClassName.endsWith("Default3")) {
    					if(sentTypes.size()>=3) {
    						sentTypes.remove(sentTypes.size()-1);
    						if(sentTypes.equals(receivedTypes))
    			        		return true;
    						sentTypes.remove(sentTypes.size()-1);
    						if(sentTypes.equals(receivedTypes))
    			        		return true;
    						sentTypes.remove(sentTypes.size()-1);
    						if(sentTypes.equals(receivedTypes))
    			        		return true;
    					}            			
            		}else if(signalClassName.endsWith("Default4")) {
    					if(sentTypes.size()>=4) {
    						sentTypes.remove(sentTypes.size()-1);
    						if(sentTypes.equals(receivedTypes))
    			        		return true;
    						sentTypes.remove(sentTypes.size()-1);
    						if(sentTypes.equals(receivedTypes))
    			        		return true;
    						sentTypes.remove(sentTypes.size()-1);
    						if(sentTypes.equals(receivedTypes))
    			        		return true;
    						sentTypes.remove(sentTypes.size()-1);
    						if(sentTypes.equals(receivedTypes))
    			        		return true;
    					}            			
            		}else if(signalClassName.endsWith("Default5")) {
    					if(sentTypes.size()>=5) {
    						sentTypes.remove(sentTypes.size()-1);
    						if(sentTypes.equals(receivedTypes))
    			        		return true;
    						sentTypes.remove(sentTypes.size()-1);
    						if(sentTypes.equals(receivedTypes))
    			        		return true;
    						sentTypes.remove(sentTypes.size()-1);
    						if(sentTypes.equals(receivedTypes))
    			        		return true;
    						sentTypes.remove(sentTypes.size()-1);
    						if(sentTypes.equals(receivedTypes))
    			        		return true;
    						sentTypes.remove(sentTypes.size()-1);
    						if(sentTypes.equals(receivedTypes))
    			        		return true;
    					}            			
            		}else if(signalClassName.endsWith("Default6")) {
    					if(sentTypes.size()>=6) {
    						sentTypes.remove(sentTypes.size()-1);
    						if(sentTypes.equals(receivedTypes))
    			        		return true;
    						sentTypes.remove(sentTypes.size()-1);
    						if(sentTypes.equals(receivedTypes))
    			        		return true;
    						sentTypes.remove(sentTypes.size()-1);
    						if(sentTypes.equals(receivedTypes))
    			        		return true;
    						sentTypes.remove(sentTypes.size()-1);
    						if(sentTypes.equals(receivedTypes))
    			        		return true;
    						sentTypes.remove(sentTypes.size()-1);
    						if(sentTypes.equals(receivedTypes))
    			        		return true;
    						sentTypes.remove(sentTypes.size()-1);
    						if(sentTypes.equals(receivedTypes))
    			        		return true;
    					}            			
            		}else if(signalClassName.endsWith("Default7")) {
    					if(sentTypes.size()>=7) {
    						sentTypes.remove(sentTypes.size()-1);
    						if(sentTypes.equals(receivedTypes))
    			        		return true;
    						sentTypes.remove(sentTypes.size()-1);
    						if(sentTypes.equals(receivedTypes))
    			        		return true;
    						sentTypes.remove(sentTypes.size()-1);
    						if(sentTypes.equals(receivedTypes))
    			        		return true;
    						sentTypes.remove(sentTypes.size()-1);
    						if(sentTypes.equals(receivedTypes))
    			        		return true;
    						sentTypes.remove(sentTypes.size()-1);
    						if(sentTypes.equals(receivedTypes))
    			        		return true;
    						sentTypes.remove(sentTypes.size()-1);
    						if(sentTypes.equals(receivedTypes))
    			        		return true;
    						sentTypes.remove(sentTypes.size()-1);
    						if(sentTypes.equals(receivedTypes))
    			        		return true;
    					}            			
            		}else if(signalClassName.endsWith("Default8")) {
    					if(sentTypes.size()>=8) {
    						sentTypes.remove(sentTypes.size()-1);
    						if(sentTypes.equals(receivedTypes))
    			        		return true;
    						sentTypes.remove(sentTypes.size()-1);
    						if(sentTypes.equals(receivedTypes))
    			        		return true;
    						sentTypes.remove(sentTypes.size()-1);
    						if(sentTypes.equals(receivedTypes))
    			        		return true;
    						sentTypes.remove(sentTypes.size()-1);
    						if(sentTypes.equals(receivedTypes))
    			        		return true;
    						sentTypes.remove(sentTypes.size()-1);
    						if(sentTypes.equals(receivedTypes))
    			        		return true;
    						sentTypes.remove(sentTypes.size()-1);
    						if(sentTypes.equals(receivedTypes))
    			        		return true;
    						sentTypes.remove(sentTypes.size()-1);
    						if(sentTypes.equals(receivedTypes))
    			        		return true;
    						sentTypes.remove(sentTypes.size()-1);
    						if(sentTypes.equals(receivedTypes))
    			        		return true;
    					}            			
            		}else if(signalClassName.endsWith("Default9")) {
    					if(sentTypes.size()>=9) {
    						sentTypes.remove(sentTypes.size()-1);
    						if(sentTypes.equals(receivedTypes))
    			        		return true;
    						sentTypes.remove(sentTypes.size()-1);
    						if(sentTypes.equals(receivedTypes))
    			        		return true;
    						sentTypes.remove(sentTypes.size()-1);
    						if(sentTypes.equals(receivedTypes))
    			        		return true;
    						sentTypes.remove(sentTypes.size()-1);
    						if(sentTypes.equals(receivedTypes))
    			        		return true;
    						sentTypes.remove(sentTypes.size()-1);
    						if(sentTypes.equals(receivedTypes))
    			        		return true;
    						sentTypes.remove(sentTypes.size()-1);
    						if(sentTypes.equals(receivedTypes))
    			        		return true;
    						sentTypes.remove(sentTypes.size()-1);
    						if(sentTypes.equals(receivedTypes))
    			        		return true;
    						sentTypes.remove(sentTypes.size()-1);
    						if(sentTypes.equals(receivedTypes))
    			        		return true;
    						sentTypes.remove(sentTypes.size()-1);
    						if(sentTypes.equals(receivedTypes))
    			        		return true;
    					}            			
            		}
        		}
        	}
            return false;
        }
        
        @io.qt.QtUninvokable
        private static boolean matches(boolean isNativeConnect, Type sentType, Type receivedType) {
        	if(sentType instanceof Class) {
            	Class<?> sentClass = (Class<?>)sentType;
        		if(receivedType instanceof Class) {
        			Class<?> receivedClass = (Class<?>)receivedType;
                	if(sentClass==receivedClass || receivedClass.isAssignableFrom(sentClass)) {
                		return true;
                	}
        		}else if(receivedType instanceof GenericArrayType) {
        			GenericArrayType genericArrayType = (GenericArrayType)receivedType;
        			if(sentClass.isArray() && matches(isNativeConnect, sentClass.getComponentType(), genericArrayType.getGenericComponentType())) {
    					return true;
        			}
        		}else if(receivedType instanceof WildcardType) {
        			WildcardType receivedWildcardType = (WildcardType)receivedType;
        			for(Type bound : receivedWildcardType.getUpperBounds()) {
        				if(!matches(isNativeConnect, sentType, bound))
        					return false;
        			}
        			for(Type bound : receivedWildcardType.getLowerBounds()) {
        				if(!matches(isNativeConnect, bound, sentType))
        					return false;
        			}
        			return true;
        		}else if(receivedType instanceof TypeVariable) {
        			TypeVariable<?> receivedVarType = (TypeVariable<?>)receivedType;
        			for(Type bound : receivedVarType.getBounds()) {
        				if(!matches(isNativeConnect, sentType, bound))
        					return false;
        			}
        			return true;
        		}else if(receivedType instanceof ParameterizedType) {
        			ParameterizedType receivedParamType = (ParameterizedType)receivedType;
        			return matches(isNativeConnect, sentType, receivedParamType.getRawType());
        		}
        	}else if(sentType instanceof WildcardType) {
    			WildcardType sentWildcardType = (WildcardType)sentType;
    			for(Type bound : sentWildcardType.getUpperBounds()) {
    				if(!matches(isNativeConnect, bound, receivedType))
    					return false;
    			}
    			for(Type bound : sentWildcardType.getLowerBounds()) {
    				if(!matches(isNativeConnect, receivedType, bound))
    					return false;
    			}
    			return true;
    		}else if(sentType instanceof TypeVariable) {
    			TypeVariable<?> sentVarType = (TypeVariable<?>)receivedType;
    			for(Type bound : sentVarType.getBounds()) {
    				if(!matches(isNativeConnect, bound, receivedType))
    					return false;
    			}
    			return true;
    		}else if(sentType instanceof ParameterizedType) {
        		ParameterizedType sentParamType = (ParameterizedType)sentType;
        		if(receivedType instanceof WildcardType) {
        			WildcardType receivedWildcardType = (WildcardType)receivedType;
        			for(Type bound : receivedWildcardType.getUpperBounds()) {
        				if(!matches(isNativeConnect, sentType, bound))
        					return false;
        			}
        			for(Type bound : receivedWildcardType.getLowerBounds()) {
        				if(!matches(isNativeConnect, bound, sentType))
        					return false;
        			}
        			return true;
        		}else if(receivedType instanceof TypeVariable) {
        			TypeVariable<?> receivedVarType = (TypeVariable<?>)receivedType;
        			for(Type bound : receivedVarType.getBounds()) {
        				if(!matches(isNativeConnect, sentType, bound))
        					return false;
        			}
        			return true;
        		}else if(receivedType instanceof ParameterizedType) {
        			ParameterizedType receivedParamType = (ParameterizedType)receivedType;
    				if(!matches(isNativeConnect, sentParamType.getRawType(), receivedParamType.getRawType()))
    					return false;
    				Type[] sentArgs = sentParamType.getActualTypeArguments();
	    			Type[] receivedArgs = receivedParamType.getActualTypeArguments();
	    			if(sentArgs.length!=receivedArgs.length)
	    				return false;
	    			for (int i = 0; i < receivedArgs.length; i++) {
						if(!matches(isNativeConnect, sentArgs[i], receivedArgs[i]))
							return false;
					}
	    			return true;
        		}else if(receivedType instanceof Class) {
        			return matches(isNativeConnect, sentParamType.getRawType(), receivedType);
        		}
    		}else if(sentType instanceof GenericArrayType) {
    			GenericArrayType sentArrayType = (GenericArrayType)sentType;
        		if(receivedType instanceof GenericArrayType) {
        			GenericArrayType receivedArrayType = (GenericArrayType)receivedType;
    				if(matches(isNativeConnect, sentArrayType.getGenericComponentType(), receivedArrayType.getGenericComponentType()))
    					return true;
        		}else if(receivedType instanceof Class && ((Class<?>) receivedType).isArray()) {
        			return matches(isNativeConnect, sentArrayType.getGenericComponentType(), ((Class<?>) receivedType).getComponentType());
        		}
        	}
        	return false;
        }
        
        @io.qt.QtUninvokable
        private static Match matches(boolean isNativeConnect, SignalParameterType sentType, SignalParameterType receivedType) {
        	Class<?> sentClass = sentType.type;
        	Class<?> receivedClass = receivedType.type;
        	if(sentClass==null)
        		sentClass = Object.class;
        	if(sentClass==int.class)
        		sentClass = Integer.class;
        	if(sentClass==byte.class)
        		sentClass = byte.class;
        	if(sentClass==short.class)
        		sentClass = Short.class;
        	if(sentClass==long.class)
        		sentClass = Long.class;
        	if(sentClass==boolean.class)
        		sentClass = Boolean.class;
        	if(sentClass==char.class)
        		sentClass = Character.class;
        	if(sentClass==double.class)
        		sentClass = Double.class;
        	if(sentClass==float.class)
        		sentClass = Float.class;
        	if(receivedClass==null)
        		receivedClass = Object.class;
        	if(receivedClass==int.class)
        		receivedClass = Integer.class;
        	if(receivedClass==byte.class)
        		receivedClass = Byte.class;
        	if(receivedClass==short.class)
        		receivedClass = Short.class;
        	if(receivedClass==long.class)
        		receivedClass = Long.class;
        	if(receivedClass==boolean.class)
        		receivedClass = Boolean.class;
        	if(receivedClass==char.class)
        		receivedClass = Character.class;
        	if(receivedClass==double.class)
        		receivedClass = Double.class;
        	if(receivedClass==float.class)
        		receivedClass = Float.class;
        	if(isNativeConnect) {
            	if(Set.class==receivedClass)
            		receivedClass = QSet.class;        		
            	if(List.class==receivedClass)
            		receivedClass = QList.class;
            	if(NavigableMap.class==receivedClass)
            		receivedClass = QMap.class;        		
            	if(Map.class==receivedClass)
            		receivedClass = QHash.class;        		
            	if(Set.class==sentClass)
            		sentClass = QSet.class;        		
            	if(List.class==sentClass)
            		sentClass = QList.class;        		
            	if(NavigableMap.class==sentClass)
            		sentClass = QMap.class;        		
            	if(Map.class==sentClass)
            		sentClass = QHash.class;        		
        	}
        	if(sentClass==receivedClass || receivedClass.isAssignableFrom(sentClass)) {
        		if(sentType.genericType==null
        				|| sentType.genericType instanceof Class 
        				|| sentType.genericType==receivedType.genericType) {
        			return (!isNativeConnect || sentClass==receivedClass) && (sentType.isReference==receivedType.isReference) && (sentType.isPointer==receivedType.isPointer) ? Match.NativeMatch : Match.JavaMatch;
        		}else if(sentType.genericType instanceof GenericArrayType || receivedType.genericType instanceof GenericArrayType) {
        			if(matches(isNativeConnect, sentType.genericType, receivedType.genericType)) {
    					return (!isNativeConnect || sentClass==receivedClass) && (sentType.isReference==receivedType.isReference) && (sentType.isPointer==receivedType.isPointer) ? Match.NativeMatch : Match.JavaMatch;
        			}
        		}else if(sentType.genericType instanceof ParameterizedType) {
        			ParameterizedType sentParamType = (ParameterizedType)sentType.genericType;
        			if(receivedType.genericType instanceof ParameterizedType) {
	        			ParameterizedType receivedParamType = (ParameterizedType)receivedType.genericType;
	        			Type[] sentArgs = sentParamType.getActualTypeArguments();
	        			Type[] receivedArgs = receivedParamType.getActualTypeArguments();
	        			if(sentArgs.length!=receivedArgs.length)
	        				return Match.NoMatch;
	        			for (int i = 0; i < receivedArgs.length; i++) {
							if(!matches(isNativeConnect, sentArgs[i], receivedArgs[i]))
								return Match.NoMatch;
						}
	        			return (sentType.isReference==receivedType.isReference) && (sentType.isPointer==receivedType.isPointer) ? Match.NativeMatch : Match.JavaMatch;
        			}else if(receivedType.genericType instanceof Class){
        				if(sentParamType.getRawType()==receivedType.genericType || ((Class<?>)receivedType.genericType).isAssignableFrom((Class<?>)sentParamType.getRawType())) {
        					return Match.JavaMatch;
        				}
            		}else if(receivedType.genericType instanceof TypeVariable<?>) {
            			return Match.JavaMatch;
        			}
        		}else if(sentType.genericType instanceof TypeVariable<?>) {
        			return Match.JavaMatch;
        		}
        	}else if(sentType.genericType instanceof TypeVariable<?>) {
        		return Match.JavaMatch;
        	}
        	if(isNativeConnect) {
        		if(receivedClass==QStringList.class
        				&& sentClass==QList.class
        				&& sentType.genericType instanceof ParameterizedType) {
        			Type[] sentArgs = ((ParameterizedType)sentType.genericType).getActualTypeArguments();
        			if(sentArgs.length==1 && sentArgs[0]==String.class) {
        				return (sentType.isReference==receivedType.isReference) && (sentType.isPointer==receivedType.isPointer) ? Match.NativeMatch : Match.JavaMatch;
        			}
        		}
        	}
        	return Match.NoMatch;
        }
        
        @io.qt.QtUninvokable
        private static Match matches(boolean isNativeConnect, List<SignalParameterType> sentTypes, List<SignalParameterType> receivedTypes) {
        	Match result = Match.NoMatch;
        	if(sentTypes.size()==receivedTypes.size()) {
        		result = Match.NativeMatch;
        		for (int i = 0; i < sentTypes.size(); i++) {
        			switch(matches(isNativeConnect, sentTypes.get(i), receivedTypes.get(i))) {
					case JavaMatch:
						result = Match.JavaMatch;
						break;
					case NativeMatch:
						break;
					case NoMatch:
						return Match.NoMatch;
					default:
						break;
        			}
				}
        		return result;
        	}
        	return result;
        }
        
        @io.qt.QtUninvokable
        private Match matchMethodTypes(List<SignalParameterType> receivedTypes, String signalClassName) {
        	List<SignalParameterType> sentTypes = signalTypes();
        	boolean isNativeConnect = isQObjectSignal() && methodIndex()>=0;
        	Match result = matches(isNativeConnect, sentTypes, receivedTypes);
        	if(result!=Match.NoMatch) {
        		return result;
        	}else if(sentTypes.size()>receivedTypes.size()){
        		sentTypes = new ArrayList<>(sentTypes);
        		while(sentTypes.size()>receivedTypes.size()) {
        			sentTypes.remove(sentTypes.size()-1);
        			result = matches(isNativeConnect, sentTypes, receivedTypes);
        			if(result!=Match.NoMatch)
                		return result;
        		}
        	}else if(sentTypes.size()<receivedTypes.size() 
        			&& signalClassName!=null
        			&& (signalClassName.endsWith("Default1")
            				|| signalClassName.endsWith("Default2")
            				|| signalClassName.endsWith("Default3")
            				|| signalClassName.endsWith("Default4")
            				|| signalClassName.endsWith("Default5")
            				|| signalClassName.endsWith("Default6")
            				|| signalClassName.endsWith("Default7")
            				|| signalClassName.endsWith("Default8")
            				|| signalClassName.endsWith("Default9"))){
    			receivedTypes = new ArrayList<>(receivedTypes);
				if(signalClassName.endsWith("Default1")) {
					if(receivedTypes.size()>=1) {
						receivedTypes.remove(receivedTypes.size()-1);
	        			result = matches(isNativeConnect, sentTypes, receivedTypes);
	        			if(result!=Match.NoMatch)
	                		return result;
					}
        		}else if(signalClassName.endsWith("Default2")) {
					if(receivedTypes.size()>=2) {
						receivedTypes.remove(receivedTypes.size()-1);
	        			result = matches(isNativeConnect, sentTypes, receivedTypes);
	        			if(result!=Match.NoMatch)
	                		return result;
						receivedTypes.remove(receivedTypes.size()-1);
	        			result = matches(isNativeConnect, sentTypes, receivedTypes);
	        			if(result!=Match.NoMatch)
	                		return result;
					}            			
        		}else if(signalClassName.endsWith("Default3")) {
					if(receivedTypes.size()>=3) {
						receivedTypes.remove(receivedTypes.size()-1);
	        			result = matches(isNativeConnect, sentTypes, receivedTypes);
	        			if(result!=Match.NoMatch)
	                		return result;
						receivedTypes.remove(receivedTypes.size()-1);
	        			result = matches(isNativeConnect, sentTypes, receivedTypes);
	        			if(result!=Match.NoMatch)
	                		return result;
						receivedTypes.remove(receivedTypes.size()-1);
	        			result = matches(isNativeConnect, sentTypes, receivedTypes);
	        			if(result!=Match.NoMatch)
	                		return result;
					}            			
        		}else if(signalClassName.endsWith("Default4")) {
					if(receivedTypes.size()>=4) {
						receivedTypes.remove(receivedTypes.size()-1);
	        			result = matches(isNativeConnect, sentTypes, receivedTypes);
	        			if(result!=Match.NoMatch)
	                		return result;
						receivedTypes.remove(receivedTypes.size()-1);
	        			result = matches(isNativeConnect, sentTypes, receivedTypes);
	        			if(result!=Match.NoMatch)
	                		return result;
						receivedTypes.remove(receivedTypes.size()-1);
	        			result = matches(isNativeConnect, sentTypes, receivedTypes);
	        			if(result!=Match.NoMatch)
	                		return result;
						receivedTypes.remove(receivedTypes.size()-1);
	        			result = matches(isNativeConnect, sentTypes, receivedTypes);
	        			if(result!=Match.NoMatch)
	                		return result;
					}            			
        		}else if(signalClassName.endsWith("Default5")) {
					if(receivedTypes.size()>=5) {
						receivedTypes.remove(receivedTypes.size()-1);
	        			result = matches(isNativeConnect, sentTypes, receivedTypes);
	        			if(result!=Match.NoMatch)
	                		return result;
						receivedTypes.remove(receivedTypes.size()-1);
	        			result = matches(isNativeConnect, sentTypes, receivedTypes);
	        			if(result!=Match.NoMatch)
	                		return result;
						receivedTypes.remove(receivedTypes.size()-1);
	        			result = matches(isNativeConnect, sentTypes, receivedTypes);
	        			if(result!=Match.NoMatch)
	                		return result;
						receivedTypes.remove(receivedTypes.size()-1);
	        			result = matches(isNativeConnect, sentTypes, receivedTypes);
	        			if(result!=Match.NoMatch)
	                		return result;
						receivedTypes.remove(receivedTypes.size()-1);
	        			result = matches(isNativeConnect, sentTypes, receivedTypes);
	        			if(result!=Match.NoMatch)
	                		return result;
					}            			
        		}else if(signalClassName.endsWith("Default6")) {
					if(receivedTypes.size()>=6) {
						receivedTypes.remove(receivedTypes.size()-1);
	        			result = matches(isNativeConnect, sentTypes, receivedTypes);
	        			if(result!=Match.NoMatch)
	                		return result;
						receivedTypes.remove(receivedTypes.size()-1);
	        			result = matches(isNativeConnect, sentTypes, receivedTypes);
	        			if(result!=Match.NoMatch)
	                		return result;
						receivedTypes.remove(receivedTypes.size()-1);
	        			result = matches(isNativeConnect, sentTypes, receivedTypes);
	        			if(result!=Match.NoMatch)
	                		return result;
						receivedTypes.remove(receivedTypes.size()-1);
	        			result = matches(isNativeConnect, sentTypes, receivedTypes);
	        			if(result!=Match.NoMatch)
	                		return result;
						receivedTypes.remove(receivedTypes.size()-1);
	        			result = matches(isNativeConnect, sentTypes, receivedTypes);
	        			if(result!=Match.NoMatch)
	                		return result;
						receivedTypes.remove(receivedTypes.size()-1);
	        			result = matches(isNativeConnect, sentTypes, receivedTypes);
	        			if(result!=Match.NoMatch)
	                		return result;
					}            			
        		}else if(signalClassName.endsWith("Default7")) {
					if(receivedTypes.size()>=7) {
						receivedTypes.remove(receivedTypes.size()-1);
	        			result = matches(isNativeConnect, sentTypes, receivedTypes);
	        			if(result!=Match.NoMatch)
	                		return result;
						receivedTypes.remove(receivedTypes.size()-1);
	        			result = matches(isNativeConnect, sentTypes, receivedTypes);
	        			if(result!=Match.NoMatch)
	                		return result;
						receivedTypes.remove(receivedTypes.size()-1);
	        			result = matches(isNativeConnect, sentTypes, receivedTypes);
	        			if(result!=Match.NoMatch)
	                		return result;
						receivedTypes.remove(receivedTypes.size()-1);
	        			result = matches(isNativeConnect, sentTypes, receivedTypes);
	        			if(result!=Match.NoMatch)
	                		return result;
						receivedTypes.remove(receivedTypes.size()-1);
	        			result = matches(isNativeConnect, sentTypes, receivedTypes);
	        			if(result!=Match.NoMatch)
	                		return result;
						receivedTypes.remove(receivedTypes.size()-1);
	        			result = matches(isNativeConnect, sentTypes, receivedTypes);
	        			if(result!=Match.NoMatch)
	                		return result;
						receivedTypes.remove(receivedTypes.size()-1);
	        			result = matches(isNativeConnect, sentTypes, receivedTypes);
	        			if(result!=Match.NoMatch)
	                		return result;
					}            			
        		}else if(signalClassName.endsWith("Default8")) {
					if(receivedTypes.size()>=8) {
						receivedTypes.remove(receivedTypes.size()-1);
	        			result = matches(isNativeConnect, sentTypes, receivedTypes);
	        			if(result!=Match.NoMatch)
	                		return result;
						receivedTypes.remove(receivedTypes.size()-1);
	        			result = matches(isNativeConnect, sentTypes, receivedTypes);
	        			if(result!=Match.NoMatch)
	                		return result;
						receivedTypes.remove(receivedTypes.size()-1);
	        			result = matches(isNativeConnect, sentTypes, receivedTypes);
	        			if(result!=Match.NoMatch)
	                		return result;
						receivedTypes.remove(receivedTypes.size()-1);
	        			result = matches(isNativeConnect, sentTypes, receivedTypes);
	        			if(result!=Match.NoMatch)
	                		return result;
						receivedTypes.remove(receivedTypes.size()-1);
	        			result = matches(isNativeConnect, sentTypes, receivedTypes);
	        			if(result!=Match.NoMatch)
	                		return result;
						receivedTypes.remove(receivedTypes.size()-1);
	        			result = matches(isNativeConnect, sentTypes, receivedTypes);
	        			if(result!=Match.NoMatch)
	                		return result;
						receivedTypes.remove(receivedTypes.size()-1);
	        			result = matches(isNativeConnect, sentTypes, receivedTypes);
	        			if(result!=Match.NoMatch)
	                		return result;
						receivedTypes.remove(receivedTypes.size()-1);
	        			result = matches(isNativeConnect, sentTypes, receivedTypes);
	        			if(result!=Match.NoMatch)
	                		return result;
					}            			
        		}else if(signalClassName.endsWith("Default9")) {
					if(receivedTypes.size()>=9) {
						receivedTypes.remove(receivedTypes.size()-1);
	        			result = matches(isNativeConnect, sentTypes, receivedTypes);
	        			if(result!=Match.NoMatch)
	                		return result;
						receivedTypes.remove(receivedTypes.size()-1);
	        			result = matches(isNativeConnect, sentTypes, receivedTypes);
	        			if(result!=Match.NoMatch)
	                		return result;
						receivedTypes.remove(receivedTypes.size()-1);
	        			result = matches(isNativeConnect, sentTypes, receivedTypes);
	        			if(result!=Match.NoMatch)
	                		return result;
						receivedTypes.remove(receivedTypes.size()-1);
	        			result = matches(isNativeConnect, sentTypes, receivedTypes);
	        			if(result!=Match.NoMatch)
	                		return result;
						receivedTypes.remove(receivedTypes.size()-1);
	        			result = matches(isNativeConnect, sentTypes, receivedTypes);
	        			if(result!=Match.NoMatch)
	                		return result;
						receivedTypes.remove(receivedTypes.size()-1);
	        			result = matches(isNativeConnect, sentTypes, receivedTypes);
	        			if(result!=Match.NoMatch)
	                		return result;
						receivedTypes.remove(receivedTypes.size()-1);
	        			result = matches(isNativeConnect, sentTypes, receivedTypes);
	        			if(result!=Match.NoMatch)
	                		return result;
						receivedTypes.remove(receivedTypes.size()-1);
	        			result = matches(isNativeConnect, sentTypes, receivedTypes);
	        			if(result!=Match.NoMatch)
	                		return result;
						receivedTypes.remove(receivedTypes.size()-1);
	        			result = matches(isNativeConnect, sentTypes, receivedTypes);
	        			if(result!=Match.NoMatch)
	                		return result;
					}            			
        		}
    		}
            return result;
        }
        
        @io.qt.QtUninvokable
        private Match matchMethodTypes(Class<?>[] types, Type[] genericTypes, AnnotatedElement[] annotatedTypes) {
        	List<SignalParameterType> typeList = Collections.emptyList();
        	for (int i = 0; i < types.length; i++) {
        		Class<?> type = types[i];
        		Type genericType = genericTypes==null ? type : genericTypes[i];
        		AnnotatedElement annotatedType = annotatedTypes==null ? null : annotatedTypes[i];
        		SignalParameterType signalType = new SignalParameterType(type, genericType, annotatedType, false, false);
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
        	return matchMethodTypes(typeList, null);
        }

        protected void checkConnection(Object receiver, boolean slotObject) {
        	if(receiver==this) {
    			throw new IllegalArgumentException("Cannot connect a signal to itself.");
    		}
        }

        @io.qt.QtUninvokable
        private QMetaObject.Connection addConnectionToMethod(Object receiver, Method slot, MethodHandle slotHandle, Object[] lambdaArgs, Qt.ConnectionType... connectionType) {
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
        	return core.addConnectionToMethod(this, receiver, slot, slotHandle, _lambdaArgs, connectionType);
        }
        
        @io.qt.QtUninvokable
        private QMetaObject.Connection addConnectionToMethod(Object receiver, QMetaMethod slot, Qt.ConnectionType... connectionType) {
    		checkConnection(receiver, false);
    		QMetaMethod signalMethod = QMetaMethod.fromSignal(this);
    		boolean checkConnectArgs = false;
			if(signalMethod!=null 
					&& signalMethod.isValid() 
					&& (checkConnectArgs = QMetaObject.checkConnectArgs(signalMethod, slot))) {
				boolean hasWrappers = false;
				if(isDynamic(signalMethod.enclosingMetaObject())) {
					for(QByteArray ba : signalMethod.parameterTypes()) {
						if(ba.startsWith((byte)'J') && ba.endsWith(wrapper)) {
							hasWrappers = true;
							break;
						}
					}
					if(!hasWrappers) {
						for(QByteArray ba : slot.parameterTypes()) {
	    					if(ba.startsWith((byte)'J') && ba.endsWith(wrapper)) {
	    						hasWrappers = true;
	    						break;
	    					}
	    				}
					}
				}
				if(!hasWrappers) {
					return core.addConnectionToMethod(this, receiver, slot, connectionType);
				}
			}
			if(receiver instanceof QObject && slot.methodType()==QMetaMethod.MethodType.Signal) {
				AbstractSignal signal = slot.toSignal((QObject)receiver);
				if(signal!=null) {
					Match match = matchMethodTypes(signal.signalTypes(), ClassAnalyzerUtility.getClass(signal).getName());
					switch(match) {
					case NoMatch:
    					if(signalMethod!=null && signalMethod.isValid()) {
							throw new QMisfittingSignatureException(String.format("Incompatible sender/receiver arguments %1$s -> %2$s", signalMethod.cppMethodSignature(), slot.cppMethodSignature()));
    					}
    					throw new QMisfittingSignatureException(String.format("Incompatible sender/receiver arguments %1$s(%2$s) -> %3$s", fullName(), signalParameters(), slot.cppMethodSignature()));
					case NativeMatch:
						if(!checkConnectArgs)
	    					throw new QMisfittingSignatureException(String.format("Incompatible sender/receiver arguments %1$s(%2$s) -> %3$s", fullName(), signalParameters(), slot.cppMethodSignature()));
					case JavaMatch:
    					throw new QMisfittingSignatureException(String.format("Incompatible sender/receiver arguments %1$s(%2$s) -> %3$s", fullName(), signalParameters(), slot.cppMethodSignature()));
					default:
						break;
					}
				}
			}else if(slot.methodType()==QMetaMethod.MethodType.Method
					|| slot.methodType()==QMetaMethod.MethodType.Slot){
				Method slotMethod = slot.toReflectedMethod();
				if(slotMethod!=null) {
		            AnnotatedElement[] annotatedParameterTypes = null;
		            if(ClassAnalyzerUtility.useAnnotatedType)
		            	annotatedParameterTypes = slotMethod.getAnnotatedParameterTypes();
					Match match = matchMethodTypes(slotMethod.getParameterTypes(), slotMethod.getGenericParameterTypes(), annotatedParameterTypes);
					switch(match) {
					case NoMatch:
    					if(signalMethod!=null && signalMethod.isValid()) {
							throw new QMisfittingSignatureException(String.format("Incompatible sender/receiver arguments %1$s -> %2$s", signalMethod.cppMethodSignature(), slot.cppMethodSignature()));
    					}
						throw new QMisfittingSignatureException(String.format("Incompatible sender/receiver arguments %1$s(%2$s) -> %3$s", fullName(), signalParameters(), slot.cppMethodSignature()));
					case NativeMatch:
						if(checkConnectArgs)
							break;
					case JavaMatch:
						return addConnectionToMethod(receiver, slotMethod, null, null, connectionType);
					default:
						break;
					}
				}
			}
        	return core.addConnectionToMethod(this, receiver, slot, connectionType);
        }

        @io.qt.QtUninvokable
        protected final QMetaObject.Connection addConnectionToSlotObject(GenericSlot slotObject, Qt.ConnectionType[] connectionType) {
        	return addConnectionToSlotObject(GenericSlotObjectConnection::new, slotObject, connectionType);
        }

        @io.qt.QtUninvokable
        protected final QMetaObject.Connection addConnectionToSlotObject(Slot0 slotObject, Qt.ConnectionType[] connectionType) {
        	return addConnectionToSlotObject(SlotObjectConnection0::new, slotObject, connectionType);
        }

        @io.qt.QtUninvokable
        protected final QMetaObject.Connection addConnectionToSlotObject(Slot1<?> slotObject, Qt.ConnectionType[] connectionType) {
        	return addConnectionToSlotObject(SlotObjectConnection1::new, slotObject, connectionType);
        }

        @io.qt.QtUninvokable
        protected final QMetaObject.Connection addConnectionToSlotObject(Slot2<?,?> slotObject, Qt.ConnectionType[] connectionType) {
        	return addConnectionToSlotObject(SlotObjectConnection2::new, slotObject, connectionType);
        }

        @io.qt.QtUninvokable
        protected final QMetaObject.Connection addConnectionToSlotObject(Slot3<?,?,?> slotObject, Qt.ConnectionType[] connectionType) {
        	return addConnectionToSlotObject(SlotObjectConnection3::new, slotObject, connectionType);
        }

        @io.qt.QtUninvokable
        protected final QMetaObject.Connection addConnectionToSlotObject(Slot4<?,?,?,?> slotObject, Qt.ConnectionType[] connectionType) {
        	return addConnectionToSlotObject(SlotObjectConnection4::new, slotObject, connectionType);
        }

        @io.qt.QtUninvokable
        protected final QMetaObject.Connection addConnectionToSlotObject(Slot5<?,?,?,?,?> slotObject, Qt.ConnectionType[] connectionType) {
        	return addConnectionToSlotObject(SlotObjectConnection5::new, slotObject, connectionType);
        }

        @io.qt.QtUninvokable
        protected final QMetaObject.Connection addConnectionToSlotObject(Slot6<?,?,?,?,?,?> slotObject, Qt.ConnectionType[] connectionType) {
        	return addConnectionToSlotObject(SlotObjectConnection6::new, slotObject, connectionType);
        }

        @io.qt.QtUninvokable
        protected final QMetaObject.Connection addConnectionToSlotObject(Slot7<?,?,?,?,?,?,?> slotObject, Qt.ConnectionType[] connectionType) {
        	return addConnectionToSlotObject(SlotObjectConnection7::new, slotObject, connectionType);
        }

        @io.qt.QtUninvokable
        protected final QMetaObject.Connection addConnectionToSlotObject(Slot8<?,?,?,?,?,?,?,?> slotObject, Qt.ConnectionType[] connectionType) {
        	return addConnectionToSlotObject(SlotObjectConnection8::new, slotObject, connectionType);
        }

        @io.qt.QtUninvokable
        protected final QMetaObject.Connection addConnectionToSlotObject(Slot9<?,?,?,?,?,?,?,?,?> slotObject, Qt.ConnectionType[] connectionType) {
        	return addConnectionToSlotObject(SlotObjectConnection9::new, slotObject, connectionType);
        }

        @io.qt.QtUninvokable
        private <Slot extends Serializable> QMetaObject.Connection addConnectionToSlotObject(SlotConnectionFactory<Slot> factory, Slot slotObject, Qt.ConnectionType[] connectionType) {
            Object lambdaOwner = null;
            Class<?> lambdaOwnerClass = null;
			int lambdaHashCode = 0;
			Class<?> lambdaClass = ClassAnalyzerUtility.getClass(slotObject);
			SerializedLambda serializedLambda = ClassAnalyzerUtility.serializeLambdaExpression(slotObject);
            if(serializedLambda!=null){
				MethodHandle methodHandle = ReflectionUtility.lambdaSlotHandles(lambdaClass, serializedLambda);
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
				
				if(reflectiveMethod!=null && methodHandle!=null){
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
						Class<?>[] paraTypes = reflectiveMethod.getParameterTypes();
						Type[] genTypes = reflectiveMethod.getGenericParameterTypes();
						AnnotatedElement[] annTypes = null;
			            if(ClassAnalyzerUtility.useAnnotatedType)
			            	annTypes = reflectiveMethod.getAnnotatedParameterTypes();
			            if(lambdaArgs!=null && lambdaArgs.length>0) {
			            	Class<?>[] _paraTypes = new Class[paraTypes.length-lambdaArgs.length];
							Type[] _genTypes = new Type[genTypes.length-lambdaArgs.length];
							System.arraycopy(paraTypes, lambdaArgs.length, _paraTypes, 0, _paraTypes.length);
							System.arraycopy(genTypes, lambdaArgs.length, _genTypes, 0, _genTypes.length);
							paraTypes = _paraTypes;
							genTypes = _genTypes;
							if(annTypes!=null) {
								AnnotatedElement[] _annTypes = new AnnotatedElement[annTypes.length-lambdaArgs.length];
								System.arraycopy(annTypes, lambdaArgs.length, _annTypes, 0, _annTypes.length);
								annTypes = _annTypes;
							}
			            }
						Match match = matchMethodTypes(paraTypes, genTypes, annTypes);
						if(match==Match.NoMatch) {
							QMetaMethod signal = QMetaMethod.fromSignal(this);
							if(signal!=null && signal.isValid()) {
								throw new QMisfittingSignatureException(String.format("Incompatible sender/receiver arguments %1$s -> %2$s", signal.cppMethodSignature(), reflectiveMethod.toGenericString()));
							}
							throw new QMisfittingSignatureException(String.format("Incompatible sender/receiver arguments %1$s(%2$s) -> %3$s", fullName(), signalParameters(), reflectiveMethod.toGenericString()));
						}
						return addConnectionToMethod(owner, reflectiveMethod, methodHandle, lambdaArgs, connectionType);
					}else if(serializedLambda.getCapturedArgCount()>0
							&& (arg1 = serializedLambda.getCapturedArg(0))!=null
							&& reflectiveMethod.getDeclaringClass().isInstance(arg1)){
						if (reflectiveMethod.isAnnotationPresent(QtUninvokable.class)) {
							QMetaMethod method = QMetaMethod.fromReflectedMethod(reflectiveMethod);
			            	if(method.isValid()) {
			            		reflectiveMethod = method.toReflectedMethod();
			            	}
						}
						Object[] lambdaArgs = null;
						if(serializedLambda.getCapturedArgCount()>1){
							lambdaArgs = new Object[serializedLambda.getCapturedArgCount()-1];
							for(int i=1; i<serializedLambda.getCapturedArgCount(); i++) {
								lambdaArgs[i-1] = serializedLambda.getCapturedArg(i);
							}
						}else if(arg1 instanceof QMetaObject.AbstractSignal && reflectiveMethod.getName().equals("emit")) {
							QMetaObject.AbstractSignal signal = (QMetaObject.AbstractSignal)arg1;
							QMetaMethod method = QMetaMethod.fromSignal(signal);
				    		if(method!=null && method.isValid()) {
				    			return addConnectionToMethod(signal.containingObject(), method, connectionType);
				    		}
			        	}
						Class<?>[] paraTypes = reflectiveMethod.getParameterTypes();
						Type[] genTypes = reflectiveMethod.getGenericParameterTypes();
						AnnotatedElement[] annTypes = null;
			            if(ClassAnalyzerUtility.useAnnotatedType)
			            	annTypes = reflectiveMethod.getAnnotatedParameterTypes();
						if(lambdaArgs==null || lambdaArgs.length==0) {
							QMetaMethod metaMethod = QMetaMethod.fromReflectedMethod(reflectiveMethod);
							if(metaMethod!=null && metaMethod.isValid()) {
								if(metaMethod.methodType()==QMetaMethod.MethodType.Signal) {
									Method _reflectiveMethod = reflectiveMethod;
									java.util.logging.Logger.getLogger("io.qt.internal").warning(()->String.format("Java method '%1$s' points to signal '%2$s'. Use signal connection instead: connect(receiver.%3$s)", _reflectiveMethod.toGenericString(), metaMethod.methodSignature(), metaMethod.name()));
								}
								return addConnectionToMethod(arg1, metaMethod, connectionType);
							}
						}else {
							Class<?>[] _paraTypes = new Class[paraTypes.length-lambdaArgs.length];
							Type[] _genTypes = new Type[genTypes.length-lambdaArgs.length];
							System.arraycopy(paraTypes, lambdaArgs.length, _paraTypes, 0, _paraTypes.length);
							System.arraycopy(genTypes, lambdaArgs.length, _genTypes, 0, _genTypes.length);
							paraTypes = _paraTypes;
							genTypes = _genTypes;
							if(annTypes!=null) {
								AnnotatedElement[] _annTypes = new AnnotatedElement[annTypes.length-lambdaArgs.length];
								System.arraycopy(annTypes, lambdaArgs.length, _annTypes, 0, _annTypes.length);
								annTypes = _annTypes;
							}
						}
						Match match = matchMethodTypes(paraTypes, genTypes, annTypes);
						if(match==Match.NoMatch) {
							QMetaMethod signal = QMetaMethod.fromSignal(this);
							if(signal!=null && signal.isValid()) {
								throw new QMisfittingSignatureException(String.format("Incompatible sender/receiver arguments %1$s -> %2$s", signal.cppMethodSignature(), reflectiveMethod.toGenericString()));
							}
							throw new QMisfittingSignatureException(String.format("Incompatible sender/receiver arguments %1$s(%2$s) -> %3$s", fullName(), signalParameters(), reflectiveMethod.toGenericString()));
						}
						return addConnectionToMethod(arg1, reflectiveMethod, methodHandle, lambdaArgs, connectionType);
					}else{
						try {
							lambdaOwnerClass = Class.forName(serializedLambda.getCapturingClass().replace('/', '.'));
						} catch (Throwable e) {
						}
					}
				}else{
					String[] split = lambdaClass.getName().split("Lambda\\$");
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
						lambdaOwner = ReflectionUtility.readField(slotObject, lambdaClass, "arg$1", null);
					} catch (Throwable e) {
					}
	        		checkConnection(lambdaOwner, true);
				}
				
				lambdaHashCode = hashcode(serializedLambda);
				if(serializedLambda.getCapturedArgCount()>0){
					lambdaOwner = serializedLambda.getCapturedArg(0);
					if(!(lambdaOwner instanceof QtThreadAffineInterface) && !(lambdaOwner instanceof SignalUtility.AbstractSignal)){
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
			}else if(lambdaClass.isSynthetic() && lambdaClass.getSuperclass()==Object.class){
				for(Field field : lambdaClass.getDeclaredFields()) {
					if(!Modifier.isStatic(field.getModifiers())) {
						//if(QObject.class.isAssignableFrom(field.getType())) 
						{
							try {
								lambdaOwner = ReflectionUtility.readField(slotObject, field);
								if(lambdaOwner!=null)
									break;
							} catch (Throwable e) {
							}
						}
					}
				}
        		checkConnection(lambdaOwner, true);
			}
            
            return this.core.addConnectionToSlotObject(this, factory, lambdaOwner, lambdaOwnerClass, lambdaHashCode, slotObject, connectionType);
        }
        
        @io.qt.QtUninvokable
        protected final QMetaObject.Connection addConnectionToSignalObject(AbstractSignal signalObject, Qt.ConnectionType[] connectionType) {
        	this.name();
    		QMetaMethod method = QMetaMethod.fromSignal((QMetaObject.AbstractSignal)signalObject);
    		QMisfittingSignatureException mfe = null;
    		if(method!=null && method.isValid()) {
    			try {
					return addConnectionToMethod(signalObject.containingObject(), method, connectionType);
				} catch (QMisfittingSignatureException e) {
					mfe = e;
				}
    		}
    		for(int signalTypeCount = signalTypes().size(); signalTypeCount>=0; --signalTypeCount) {
	    		for(Method mtd : ClassAnalyzerUtility.getClass(signalObject).getMethods()) {
	    			if(mtd.getName().equals("emit") 
	    					&& mtd.getReturnType()==void.class
	    					&& mtd.getParameterCount()==signalTypeCount) {
	    				return addConnectionToMethod(signalObject, mtd, null, null, connectionType);
	    			}
	    		}
    		}
    		if(mfe!=null)
    			throw mfe;
        	return new NonConnection();//this.core.addConnectionToSignal(this, signalObject, connectionType);
        }
        
        @io.qt.QtUninvokable
		protected final boolean removeConnectionToSignalObject(AbstractSignal signalObject) {
        	this.name();
    		QMetaMethod method = QMetaMethod.fromSignal((QMetaObject.AbstractSignal)signalObject);
    		if(method!=null && method.isValid()) {
    			boolean result = removeConnectionToMethod(signalObject.containingObject(), method, true);
    			if(result)
    				return result;
    		}
    		for(int signalTypeCount = signalTypes().size(); signalTypeCount>=0; --signalTypeCount) {
	    		for(Method mtd : ClassAnalyzerUtility.getClass(signalObject).getMethods()) {
	    			if(mtd.getName().equals("emit") 
	    					&& mtd.getReturnType()==void.class
	    					&& mtd.getParameterCount()==signalTypeCount) {
	    				return removeConnectionToMethod(signalObject, mtd, null, true);
	    			}
	    		}
    		}
        	return false;
        }

        @io.qt.QtUninvokable
		protected final boolean removeConnectionToSlotObject(Serializable slotObject) {
        	if(slotObject instanceof AbstractSignal) {
        		return removeConnectionToSignalObject((AbstractSignal)slotObject);
        	}
			SerializedLambda _serializedLambda = ClassAnalyzerUtility.serializeLambdaExpression(slotObject);
			Class<?> _lambdaOwnerClass = null;
			Class<?> _functionalInterfaceClass = null;
            if(_serializedLambda!=null){
        		try {
					_lambdaOwnerClass = ClassAnalyzerUtility.getClass(slotObject).getClassLoader().loadClass(_serializedLambda.getCapturingClass().replace('/', '.'));
					_functionalInterfaceClass = ClassAnalyzerUtility.getClass(slotObject).getClassLoader().loadClass(_serializedLambda.getFunctionalInterfaceClass().replace('/', '.'));
				} catch (Throwable e) {
				}
            	MethodHandle methodHandle = ReflectionUtility.lambdaSlotHandles(ClassAnalyzerUtility.getClass(slotObject), _serializedLambda);
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
						Object[] lambdaArgs = null;
						if(_serializedLambda.getCapturedArgCount()>0){
							lambdaArgs = new Object[_serializedLambda.getCapturedArgCount()];
							for(int i=0; i<_serializedLambda.getCapturedArgCount(); i++) {
								lambdaArgs[i] = _serializedLambda.getCapturedArg(i);
							}
						}
						if(removeConnectionToMethod(null, reflectiveMethod, lambdaArgs, true))
							return true;
					}else if(_serializedLambda.getCapturedArgCount()>0 
							&& reflectiveMethod.getDeclaringClass().isInstance(_serializedLambda.getCapturedArg(0))){
						Object[] lambdaArgs = null;
						if(_serializedLambda.getCapturedArgCount()>1){
							if(_serializedLambda.getCapturedArg(0) instanceof QMetaObject.AbstractSignal && reflectiveMethod.getName().equals("emit")) {
								QMetaObject.AbstractSignal signalObject = (QMetaObject.AbstractSignal)_serializedLambda.getCapturedArg(0);
								QMetaMethod method = QMetaMethod.fromSignal(signalObject);
					    		if(method!=null && method.isValid()) {
					    			boolean result = removeConnectionToMethod(signalObject.containingObject(), method, true);
					    			if(result)
					    				return result;
					    		}
				        	}
							lambdaArgs = new Object[_serializedLambda.getCapturedArgCount()-1];
							for(int i=1; i<_serializedLambda.getCapturedArgCount(); i++) {
								lambdaArgs[i-1] = _serializedLambda.getCapturedArg(i);
							}
						}
						if(removeConnectionToMethod(_serializedLambda.getCapturedArg(0), reflectiveMethod, lambdaArgs, true))
							return true;
					}
				}

				int _lambdaHashCode = 0;
				if(_serializedLambda!=null){
					_lambdaHashCode = hashcode(_serializedLambda);
					if(_serializedLambda.getCapturedArgCount()>0){
						_lambdaOwner = _serializedLambda.getCapturedArg(0);
						if(!(_lambdaOwner instanceof QtThreadAffineInterface) || !(_lambdaOwner instanceof SignalUtility.AbstractSignal)){
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
        	if(receiver!=null && method!=null) {
	        	QMetaObject metaObject = receiver instanceof QObject ? ((QObject)receiver).metaObject() : QMetaObject.forType(ClassAnalyzerUtility.getClass(receiver));
	        	Method slotMethod = null;
	        	if(metaObject!=null) {
	        		QMetaMethod metaMethod = metaObject.method(method);
	        		if(metaMethod==null || !metaMethod.isValid()) {
	        			slotMethod = lookupSlot(receiver, method, null);
	        			if (slotMethod == null) {
	        	            throw new QNoSuchSlotException(receiver, method);
	        	        }
	        			metaMethod = QMetaMethod.fromReflectedMethod(slotMethod);
	        		}
	        		if(metaMethod!=null && metaMethod.isValid()) {
	        			return removeConnectionToMethod(receiver, metaMethod, true);
	        		}
	        	}
	        	if (slotMethod == null)
	        		slotMethod = lookupSlot(receiver, method, null);
	        	if (slotMethod == null)
	        		throw new QNoSuchSlotException(receiver, method);
		        return removeConnectionToMethod(receiver, slotMethod, null, true);
        	}else {
        		return removeConnectionToMethod(receiver, null, null, true);
        	}
        }

        @io.qt.QtUninvokable
        final boolean removeConnectionToMethod(Object receiver, Method slot, Object[] lambdaArgs, boolean allowNativeDisconnect) {
			return core.removeConnectionToMethod(this, receiver, slot, lambdaArgs, allowNativeDisconnect);
		}
        
        @io.qt.QtUninvokable
        final boolean removeConnectionToMethod(Object receiver, QMetaMethod slot, boolean allowNativeDisconnect) {
			return core.removeConnectionToMethod(this, receiver, slot, allowNativeDisconnect);
		}

        @io.qt.QtUninvokable
		protected final boolean removeConnection(QMetaObject.Connection conn) {
            boolean returned = false;
        	if(conn instanceof JavaConnection) {
				JavaConnection javaConnection = (JavaConnection)conn;
				if(javaConnection.signal.get()==this) {
					returned = core.removeConnection(this, javaConnection);
				}
        	}else if(conn instanceof NativeConnection) {
        		NativeConnection nativeConnection = (NativeConnection)conn;
        		if(!nativeConnection.isDisposed() && nativeConnection.isConnected()) {
        			if(nativeConnection.sender()==containingObject() && this.methodIndex()==nativeConnection.signal().methodIndex()) {
        				returned = core.removeConnection(this, nativeConnection);
        			}
        		}
        	}
            return returned;
        }

		@Override
		public final String toString() {
			return core.toString(this);
		}
    }
    
    protected abstract static class SignalConfiguration<Signal extends AbstractSignal>{
    	protected SignalConfiguration(Signal signal, Class<?>[] types) {
			super();
			this.signal = signal;
			this.types = types;
		}
    	final Signal signal;
    	final Class<?>[] types;
    }
    
    private static abstract class AbstractMultiSignalCore<Signal extends AbstractSignal>{
		public AbstractMultiSignalCore() {
			super();
		}
    	abstract List<Signal> signals(AbstractMultiSignal<Signal> multiSignal);
    	abstract String name(AbstractMultiSignal<Signal> multiSignal);
    	abstract Class<?> declaringClass(AbstractMultiSignal<Signal> multiSignal);
    }
    
    private static class AnalyzingMultiSignalCore<Signal extends AbstractSignal> extends AbstractMultiSignalCore<Signal>{
    	
    	@SuppressWarnings("rawtypes")
		public final static AnalyzingMultiSignalCore INSTANCE = new AnalyzingMultiSignalCore<>();
    	
		public AnalyzingMultiSignalCore() {
			super();
		}
		
		List<Signal> preparedSignals(){ return null; }
		
		final List<Signal> signals(AbstractMultiSignal<Signal> multiSignal) {
    		multiSignal.resolveMultiSignal();
    		return multiSignal.core.signals(multiSignal);
		}
		final String name(AbstractMultiSignal<Signal> multiSignal) {
    		multiSignal.resolveMultiSignal();
    		return multiSignal.core.name(multiSignal);
    	}
		final Class<?> declaringClass(AbstractMultiSignal<Signal> multiSignal){
    		multiSignal.resolveMultiSignal();
    		return multiSignal.core.declaringClass(multiSignal);
		}
    }
    
    private final static class PreparedAnalyzingMultiSignalCore<Signal extends AbstractSignal> extends AnalyzingMultiSignalCore<Signal>{
    	final List<Signal> signals;
		public PreparedAnalyzingMultiSignalCore(List<Signal> signals) {
			super();
			this.signals = signals;
		}
		
		List<Signal> preparedSignals(){ return signals; }
    }
    
    private static final class MultiSignalCore<Signal extends AbstractSignal> extends AbstractMultiSignalCore<Signal>{
		public MultiSignalCore(List<Signal> signals, String name, Class<?> declaringClass) {
			super();
			this.signals = signals;
			this.name = name;
			this.declaringClass = declaringClass;
		}
    	private final List<Signal> signals;
		private String name;
        private Class<?> declaringClass;
        
        @Override
        final List<Signal> signals(AbstractMultiSignal<Signal> multiSignal) {return signals;}
    	
    	@Override
    	final String name(AbstractMultiSignal<Signal> multiSignal) {return name;}
    	
    	@Override
    	final Class<?> declaringClass(AbstractMultiSignal<Signal> multiSignal){return declaringClass;}
    }
	
    protected static abstract class AbstractMultiSignal<Signal extends AbstractSignal> {
    	private AbstractMultiSignalCore<Signal> core;
    	
    	@SuppressWarnings("unchecked")
		protected AbstractMultiSignal(){
			super();
    		this.core = AnalyzingMultiSignalCore.INSTANCE;
    	}
    	
        @SafeVarargs
		protected AbstractMultiSignal(SignalConfiguration<Signal> signal1, SignalConfiguration<Signal> signal2, SignalConfiguration<Signal>... furtherSignals){
			super();
			List<Signal> signals = new ArrayList<>(furtherSignals.length+2);
			for (int i = 0; i < furtherSignals.length+2; i++) {
				SignalConfiguration<Signal> signalConfiguration;
				if(i==0) {
					signalConfiguration = signal1;
				}else if(i==1) {
					signalConfiguration = signal2;
				}else {
					signalConfiguration = furtherSignals[i-2];
				}
				AbstractSignal signal = signalConfiguration.signal;
				if(signal.core instanceof AnalyzingSignalCore && signalConfiguration.types!=null) {
		        	signal.core = new MultiSignalAnalyingSignalCore(this, signalConfiguration.types);
				}
				signals.add(signalConfiguration.signal);
			}
    		this.core = new PreparedAnalyzingMultiSignalCore<>(signals);
    	}
        
        @io.qt.QtUninvokable
        @SuppressWarnings("unchecked")
        private synchronized final void resolveMultiSignal() {
        	if(core instanceof AnalyzingMultiSignalCore) {
        		AnalyzingMultiSignalCore<Signal> core = (AnalyzingMultiSignalCore<Signal>)this.core;
        		boolean found = false;
        		QtSignalEmitterInterface containingObject = containingObject();
            	Class<?> cls = containingObject==null ? ClassAnalyzerUtility.getClass(this).getDeclaringClass() : ClassAnalyzerUtility.getClass(containingObject);
                loop: while (cls != null) {
                    Field fields[] = cls.getDeclaredFields();
                    for (Field field : fields) {
                        if (AbstractMultiSignal.class.isAssignableFrom(field.getType())) {
							AbstractMultiSignal<Signal> sig;
							try {
								sig = (AbstractMultiSignal<Signal>)ReflectionUtility.readField(containingObject, field);
							} catch (Exception e1) {
								sig = null;
							}
                            if (sig == this) {
                            	found = true;
                            	if(QObject.MultiSignal.class.isAssignableFrom(field.getType()))
                            		initializeMultiSignal(this, field.getDeclaringClass(), field);
                            	if(this.core instanceof AnalyzingMultiSignalCore) {
                            		if(QStaticMemberSignals.MultiSignal.class.isAssignableFrom(field.getType())) {
		                            	if(!java.lang.reflect.Modifier.isStatic(field.getModifiers()))
		                            		throw new QSignalDeclarationException(String.format("Missing modifier 'static' at signal %1$s.%2$s.", cls.getSimpleName(), field.getName()));
                            		}else {
		                            	if(java.lang.reflect.Modifier.isStatic(field.getModifiers()))
		                            		throw new QSignalDeclarationException(String.format("Modifier 'static' not allowed for signal %1$s.%2$s. Use QStaticMemberSignals instead to declare a static signal.", cls.getSimpleName(), field.getName()));
                            		}
	                            	if (!Modifier.isFinal(field.getModifiers()))
		                    			throw new QSignalDeclarationException(String.format("Missing modifier 'final' at signal %1$s.%2$s.", cls.getSimpleName(), field.getName()));
	                    			if(!Modifier.isFinal(field.getType().getModifiers()))
	                    				throw new QSignalDeclarationException(String.format("Missing modifier 'final' at signal class %1$s.", field.getType().getTypeName()));
	                    			if(field.getDeclaringClass()!=field.getType().getEnclosingClass())
	                    				throw new QSignalDeclarationException(String.format("Declaration error at signal %1$s.%2$s: Multi signal class has to be declared in the class using it.", cls.getSimpleName(), field.getName()));
	                                String name = field.getName();
	                                final Class<?> declaringClass = field.getDeclaringClass();
	                    			QMetaObject metaObject = QMetaObject.forType(declaringClass);
	                    			List<Signal> signals = core.preparedSignals();
	                    			if(signals==null) {
	                    				Class<?> multiSignalSuperType = field.getType().getSuperclass();
	                    				while(multiSignalSuperType!=null 
	                    						&& multiSignalSuperType.getEnclosingClass()!=QObject.class
                								&& multiSignalSuperType.getEnclosingClass()!=QStaticMemberSignals.class
                								&& multiSignalSuperType.getEnclosingClass()!=QInstanceMemberSignals.class) {
	                    					multiSignalSuperType = multiSignalSuperType.getSuperclass();
	                    				}
	                    				SignalClasses signalClasses = signalClasses(multiSignalSuperType.getEnclosingClass());
	                            		if(signalClasses==null)
	                            			throw new QSignalInitializationException("Unable to resolve multi signal "+name+".");
	                            		Map<List<Class<?>>,EmitMethodInfo> emitMethods = SignalUtility.findEmitMethods(ClassAnalyzerUtility.getClass(this));
	                    				if(emitMethods==null)
	                            			throw new QSignalInitializationException("Unable to resolve multi signal "+name+".");
	                    				signals = new ArrayList<>();
	                    				for(Map.Entry<List<Class<?>>,EmitMethodInfo> entry : emitMethods.entrySet()) {
	                    					Class<?> signalClass;
	                    					if(Modifier.isPublic(entry.getValue().method.getModifiers())) {
	                    						signalClass = signalClasses.getPublicSignal(entry.getKey().size());
	                    					}else {
	                    						signalClass = signalClasses.getPrivateSignal(entry.getKey().size());
	                    					}
	                        				if(signalClass==null)
	                                			throw new QSignalInitializationException("Unable to resolve multi signal "+name+".");
	                        				Function<Object,Signal> factory = getSignalFactory(signalClass);
	                	    				if(factory==null)
	                	    					throw new QSignalInitializationException("Unable to resolve multi signal "+name+".");
	                	    				Signal signal;
	                	    				try {
	                	    					signal = factory.apply(containingObject==null ? declaringClass : containingObject);
	                						} catch (Throwable e) {
	                							throw new QSignalInitializationException("Unable to resolve multi signal "+name+".");
	                						}
	                	    				signals.add(signal);
	                	    				SignalInfo signalInfo = SignalUtility.signalInfo(null, metaObject, field, entry.getValue().method);
	                	            		if(signalInfo.signalTypes==null) {
	                	            			signal.setCore(declaringClass, name, resolveSignal(entry.getValue().method), -1, 0);
	                	            		}else {
	                	            			signal.setCore(declaringClass, name, signalInfo.signalTypes, signalInfo.methodIndex, signalInfo.enclosingMetaObject);
	                	            		}
	                    				}
	                    				signals = Collections.unmodifiableList(signals);
	                    			}else {
		                                for (AbstractSignal signal : signals) {
		                                	MultiSignalAnalyingSignalCore signalCore = (MultiSignalAnalyingSignalCore)signal.core;
		                                	Class<?> _cls = ClassAnalyzerUtility.getClass(this);
		                                	while (_cls != SignalUtility.AbstractMultiSignal.class) {
		                                		try {
		                                			Method emitMethod = _cls.getDeclaredMethod("emit", signalCore.types);
		                    	            		SignalInfo signalInfo = SignalUtility.signalInfo(null, metaObject, field, emitMethod);
		                    	            		if(signalInfo.signalTypes==null) {
		                    	            			signal.setCore(declaringClass, name, resolveSignal(emitMethod), -1, 0);
		                    	            		}else {
		                    	            			signal.setCore(declaringClass, name, signalInfo.signalTypes, signalInfo.methodIndex, signalInfo.enclosingMetaObject);
		                    	            		}
		                                        	break;
		                                		}catch(Throwable t) {}
		                    					_cls = _cls.getSuperclass();
		                    				}
										}
	                    			}
	                                this.core = new MultiSignalCore<>(signals, name, declaringClass);
                            	}
                                break loop;
                            }
                        }
                    }

                    cls = cls.getSuperclass();
                }
                if (!found) {
            		throw new QSignalDeclarationException("Multi signals must be declared as members of QObject subclasses.");
                }
        	}
        	if(core instanceof AnalyzingMultiSignalCore) {
        		throw new QSignalInitializationException("Unable to resolve multi signal.");
        	}
        }

        @io.qt.QtUninvokable
    	protected final List<Signal> signals() {
			return core.signals(this);
		}
        
    	@NativeAccess
        @io.qt.QtUninvokable
    	private final Signal signal(int i) {
    		List<Signal> signals = signals();
			return i>=0 && i<signals.size() ? signals.get(i) : null;
		}
    	
    	@SuppressWarnings("unchecked")
		@NativeAccess
        @io.qt.QtUninvokable
    	private synchronized void initializeSignals(Field field, int[] methodIndexes, long[] metaObjectIds, List<SignalParameterType>[] signalParameterTypes, Class<?>[] signalObjectTypes) {
    		if(core instanceof AnalyzingMultiSignalCore) {
    			List<Signal> signals = ((AnalyzingMultiSignalCore<Signal>)core).preparedSignals();
				String name = field.getName();
				Class<?> declaringClass = field.getDeclaringClass();
				List<Signal> _signals = signals;
				if(_signals==null)
					_signals = new ArrayList<>();
				for (int i = 0; i < signalParameterTypes.length; i++) {
	    			AbstractSignal signal;
	    			if(signals==null) {
						Function<Object,Signal> factory = getSignalFactory(signalObjectTypes[i]);
	    				if(factory==null)
	    					throw new QSignalInitializationException("Unable to initialize multi signal.");
	    				try {
							signal = factory.apply(containingObject());
						} catch (Throwable e) {
							throw new QSignalInitializationException("Unable to initialize multi signal.");
						}
	    				_signals.add((Signal)signal);
	    			}else {
	    				signal = signals.get(i);
	    			}
					List<SignalParameterType> signalTypes = signalParameterTypes[i];
    				signal.setCore(declaringClass, name, signalTypes, methodIndexes[i], metaObjectIds[i]);
	    		}
	    		core = new MultiSignalCore<>(_signals, name, declaringClass);
    		}
    	}

    	@SuppressWarnings("unchecked")
        @io.qt.QtUninvokable
		protected <A,B,C,D,E,F,G,H,I> QMetaObject.AbstractPrivateSignal9<A,B,C,D,E,F,G,H,I> overload(Class<A> type1, Class<B> type2, Class<C> type3, Class<D> type4, Class<E> type5, 
    			Class<F> type6, Class<G> type7, Class<H> type8, Class<I> type9) throws QNoSuchSignalException{
    		for(Signal signal : signals()) {
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
    		for(Signal signal : signals()) {
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
    		for(Signal signal : signals()) {
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
    		for(Signal signal : signals()) {
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
    		for(Signal signal : signals()) {
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
    		for(Signal signal : signals()) {
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
    		for(Signal signal : signals()) {
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
    		for(Signal signal : signals()) {
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
    		for(Signal signal : signals()) {
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
    		for(Signal signal : signals()) {
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
	    	for(AbstractSignal signal : signals()) {
	    		result |= signal.removeConnection(connection);
	    	}
	        return result;
	    }

        @io.qt.QtUninvokable
	    protected boolean disconnectAll() {
	    	boolean result = false;
	    	for(Signal signal : signals()) {
	    		result |= signal.removeConnection(null, null);
	    	}
	        return result;
	    }
        
        @io.qt.QtUninvokable
        protected final boolean disconnect(QMetaObject.AbstractSlot slotObject) {
        	boolean result = false;
        	for(AbstractSignal signal : signals()) {
        		result |= signal.removeConnectionToSlotObject(slotObject);
        	}
        	return result;
        }
        
        @io.qt.QtUninvokable
		protected final boolean disconnect(AbstractSignal signalObject) {
        	boolean result = false;
        	for(AbstractSignal signal : signals()) {
        		result |= signal.removeConnectionToSignalObject(signalObject);
        	}
        	return result;
        }
        
        @io.qt.QtUninvokable
        protected final QMetaObject.Connection connect(AbstractSignal signalObject, Qt.ConnectionType... connectionType) {
        	List<AbstractSignal> matchingSignals = new ArrayList<>();
        	for(AbstractSignal _signal : signals()) {
				if(_signal.matchMethodTypes(signalObject.signalTypes(), ClassAnalyzerUtility.getClass(signalObject).getName())!=AbstractSignal.Match.NoMatch) {
					matchingSignals.add(_signal);
				}
			}
        	if(matchingSignals.size()>1) {
        		List<AbstractSignal> _matchingSignals = new ArrayList<>();
        		for (AbstractSignal signal : matchingSignals) {
					if(signal.signalTypes().size()==signalObject.signalTypes().size()) {
						_matchingSignals.add(signal);
					}
				}
        		if(!_matchingSignals.isEmpty())
        			matchingSignals = _matchingSignals;
        	}
        	if(matchingSignals.isEmpty()) {
    			throwMismatchException(signalObject.argumentTypes().toArray(new Class[signalObject.signalTypes().size()]));
        	}
        	return matchingSignals.get(0).addConnectionToSignalObject(signalObject, connectionType);
        }
        
        @io.qt.QtUninvokable
        protected final QMetaObject.Connection connect(QMetaObject.AbstractSlot slotObject, Qt.ConnectionType... connectionType) {
        	List<AbstractSignal> matchingSignals = new ArrayList<>();
        	ClassAnalyzerUtility.LambdaInfo info = ClassAnalyzerUtility.lambdaInfo(slotObject);
        	if(info!=null && info.reflectiveMethod!=null) {
        		if(info.owner instanceof AbstractSignal && info.reflectiveMethod.getName().equals("emit")) {
            		return connect((AbstractSignal)info.owner, connectionType);
            	}
            	Class<?>[] parameterClassTypes = info.reflectiveMethod.getParameterTypes();
            	Type[] genericParameterTypes = info.reflectiveMethod.getGenericParameterTypes();
            	AnnotatedElement[] annotatedParameterTypes = null;
            	if(ClassAnalyzerUtility.useAnnotatedType)
            		annotatedParameterTypes = info.reflectiveMethod.getAnnotatedParameterTypes();
            	if((info.lambdaArgs!=null && !info.lambdaArgs.isEmpty())
            			|| ((info.owner!=null || info.qobject!=null) && info.reflectiveMethod!=null && Modifier.isStatic(info.reflectiveMethod.getModifiers()))) {
            		int sub = info.lambdaArgs!=null ? info.lambdaArgs.size() : 0;
            		if((info.owner!=null || info.qobject!=null) && info.reflectiveMethod!=null && Modifier.isStatic(info.reflectiveMethod.getModifiers())) {
            			++sub;
            		}
            		Class<?>[] _parameterClassTypes = new Class[parameterClassTypes.length - sub];
            		Type[] _genericParameterTypes = new Type[genericParameterTypes.length - sub];
            		System.arraycopy(parameterClassTypes, sub, _parameterClassTypes, 0, _parameterClassTypes.length);
            		System.arraycopy(genericParameterTypes, sub, _genericParameterTypes, 0, _genericParameterTypes.length);
            		parameterClassTypes = _parameterClassTypes;
            		genericParameterTypes = _genericParameterTypes;
            		if(annotatedParameterTypes!=null) {
    	        		AnnotatedElement[] _annotatedParameterTypes = new AnnotatedElement[annotatedParameterTypes.length - sub];
    	        		System.arraycopy(annotatedParameterTypes, sub, _annotatedParameterTypes, 0, _annotatedParameterTypes.length);
    	        		annotatedParameterTypes = _annotatedParameterTypes;
            		}
            	}
            	for(AbstractSignal signal : signals()) {
    				if(signal.matchMethodTypes(parameterClassTypes, genericParameterTypes, annotatedParameterTypes)!=AbstractSignal.Match.NoMatch) {
    					matchingSignals.add(signal);
    				}
    			}
            	if(matchingSignals.size()>1) {
            		List<AbstractSignal> _matchingSignals = new ArrayList<>();
            		for (AbstractSignal signal : matchingSignals) {
    					if(signal.signalTypes().size()==parameterClassTypes.length) {
    						_matchingSignals.add(signal);
    					}
    				}
            		if(!_matchingSignals.isEmpty())
            			matchingSignals = _matchingSignals;
            	}
            	if(matchingSignals.isEmpty()) {
            		throwMismatchException(parameterClassTypes);
            	}
        	}else {
        		Class<?>[] parameterTypes;
        		if(slotObject instanceof QMetaObject.Slot0) {
        			parameterTypes = ClassAnalyzerUtility.lambdaClassTypes(QMetaObject.Slot0.class, (QMetaObject.Slot0)slotObject);
            	}else if(slotObject instanceof QMetaObject.Slot1) {
            		parameterTypes = ClassAnalyzerUtility.lambdaClassTypes(QMetaObject.Slot1.class, (QMetaObject.Slot1<?>)slotObject);
            	}else if(slotObject instanceof QMetaObject.Slot2) {
            		parameterTypes = ClassAnalyzerUtility.lambdaClassTypes(QMetaObject.Slot2.class, (QMetaObject.Slot2<?,?>)slotObject);
            	}else if(slotObject instanceof QMetaObject.Slot3) {
            		parameterTypes = ClassAnalyzerUtility.lambdaClassTypes(QMetaObject.Slot3.class, (QMetaObject.Slot3<?,?,?>)slotObject);
            	}else if(slotObject instanceof QMetaObject.Slot4) {
            		parameterTypes = ClassAnalyzerUtility.lambdaClassTypes(QMetaObject.Slot4.class, (QMetaObject.Slot4<?,?,?,?>)slotObject);
            	}else if(slotObject instanceof QMetaObject.Slot5) {
            		parameterTypes = ClassAnalyzerUtility.lambdaClassTypes(QMetaObject.Slot5.class, (QMetaObject.Slot5<?,?,?,?,?>)slotObject);
            	}else if(slotObject instanceof QMetaObject.Slot6) {
            		parameterTypes = ClassAnalyzerUtility.lambdaClassTypes(QMetaObject.Slot6.class, (QMetaObject.Slot6<?,?,?,?,?,?>)slotObject);
            	}else if(slotObject instanceof QMetaObject.Slot7) {
            		parameterTypes = ClassAnalyzerUtility.lambdaClassTypes(QMetaObject.Slot7.class, (QMetaObject.Slot7<?,?,?,?,?,?,?>)slotObject);
            	}else if(slotObject instanceof QMetaObject.Slot8) {
            		parameterTypes = ClassAnalyzerUtility.lambdaClassTypes(QMetaObject.Slot8.class, (QMetaObject.Slot8<?,?,?,?,?,?,?,?>)slotObject);
            	}else if(slotObject instanceof QMetaObject.Slot9) {
            		parameterTypes = ClassAnalyzerUtility.lambdaClassTypes(QMetaObject.Slot9.class, (QMetaObject.Slot9<?,?,?,?,?,?,?,?,?>)slotObject);
            	}else{
            		throw new QMisfittingSignatureException("Invalid slot type "+ClassAnalyzerUtility.getClass(slotObject));
            	}
        		if(parameterTypes!=null) {
	        		Class<?>[] parameterClassTypes = new Class[parameterTypes.length-1];
	        		System.arraycopy(parameterTypes, 1, parameterClassTypes, 0, parameterClassTypes.length);
	            	Type[] genericParameterTypes = parameterClassTypes;
	            	AnnotatedElement[] annotatedParameterTypes = null;
	            	for(AbstractSignal signal : signals()) {
	    				if(signal.matchMethodTypes(parameterClassTypes, genericParameterTypes, annotatedParameterTypes)!=AbstractSignal.Match.NoMatch) {
	    					matchingSignals.add(signal);
	    				}
	    			}
	            	if(matchingSignals.size()>1) {
	            		List<AbstractSignal> _matchingSignals = new ArrayList<>();
	            		for (AbstractSignal signal : matchingSignals) {
	    					if(signal.signalTypes().size()==parameterClassTypes.length) {
	    						_matchingSignals.add(signal);
	    					}
	    				}
	            		if(!_matchingSignals.isEmpty())
	            			matchingSignals = _matchingSignals;
	            	}
	            	if(matchingSignals.isEmpty()) {
	            		throwMismatchException(parameterClassTypes);
	            	}
        		}
        	}
        	if(matchingSignals.isEmpty()) {
        		throwMismatchException();
        		return null;
        	}
        	if(slotObject instanceof QMetaObject.Slot0) {
        		return matchingSignals.get(0).addConnectionToSlotObject((QMetaObject.Slot0)slotObject, connectionType);
        	}else if(slotObject instanceof QMetaObject.Slot1) {
        		return matchingSignals.get(0).addConnectionToSlotObject((QMetaObject.Slot1<?>)slotObject, connectionType);
        	}else if(slotObject instanceof QMetaObject.Slot2) {
        		return matchingSignals.get(0).addConnectionToSlotObject((QMetaObject.Slot2<?,?>)slotObject, connectionType);
        	}else if(slotObject instanceof QMetaObject.Slot3) {
        		return matchingSignals.get(0).addConnectionToSlotObject((QMetaObject.Slot3<?,?,?>)slotObject, connectionType);
        	}else if(slotObject instanceof QMetaObject.Slot4) {
        		return matchingSignals.get(0).addConnectionToSlotObject((QMetaObject.Slot4<?,?,?,?>)slotObject, connectionType);
        	}else if(slotObject instanceof QMetaObject.Slot5) {
        		return matchingSignals.get(0).addConnectionToSlotObject((QMetaObject.Slot5<?,?,?,?,?>)slotObject, connectionType);
        	}else if(slotObject instanceof QMetaObject.Slot6) {
        		return matchingSignals.get(0).addConnectionToSlotObject((QMetaObject.Slot6<?,?,?,?,?,?>)slotObject, connectionType);
        	}else if(slotObject instanceof QMetaObject.Slot7) {
        		return matchingSignals.get(0).addConnectionToSlotObject((QMetaObject.Slot7<?,?,?,?,?,?,?>)slotObject, connectionType);
        	}else if(slotObject instanceof QMetaObject.Slot8) {
        		return matchingSignals.get(0).addConnectionToSlotObject((QMetaObject.Slot8<?,?,?,?,?,?,?,?>)slotObject, connectionType);
        	}else if(slotObject instanceof QMetaObject.Slot9) {
        		return matchingSignals.get(0).addConnectionToSlotObject((QMetaObject.Slot9<?,?,?,?,?,?,?,?,?>)slotObject, connectionType);
        	}else if(slotObject!=null){
        		throw new QMisfittingSignatureException("Invalid slot type "+ClassAnalyzerUtility.getClass(slotObject));
        	}else {
        		return null;
        	}
        }
        
		@io.qt.QtUninvokable
        protected QMetaObject.Connection connect(Object receiver, String method, Qt.ConnectionType... connectionType) {
        	QMetaObject metaObject = receiver instanceof QObject ? ((QObject)receiver).metaObject() : QMetaObject.forType(ClassAnalyzerUtility.getClass(receiver));
	        Class<?>[] parameterClassTypes = null;
        	List<AbstractSignal> matchingSignals = new ArrayList<>();
        	Map<AbstractSignal,AbstractSignal.Match> matches = new HashMap<>();
        	QMetaMethod metaMethod = null;
        	Method slotMethod = null;
        	if(metaObject!=null) {
        		metaMethod = metaObject.method(method);
        		if(metaMethod==null || !metaMethod.isValid()) {
        			slotMethod = lookupSlot(receiver, method, null);
        			if (slotMethod == null) {
        	            throw new QNoSuchSlotException(receiver, method);
        	        }
        			metaMethod = QMetaMethod.fromReflectedMethod(slotMethod);
        		}
        		if(metaMethod!=null && metaMethod.isValid()) {
        			parameterClassTypes = metaMethod.parameterClassTypes().toArray(new Class[metaMethod.parameterTypes().size()]);
        			boolean done = false;
        			for(AbstractSignal signal : signals()) {
        	        	signal.name();
        				QMetaMethod signalMethod = QMetaMethod.fromSignal(signal);
        				if(signalMethod!=null 
            					&& signalMethod.isValid() 
            					&& QMetaObject.checkConnectArgs(signalMethod, metaMethod)) {
            				boolean hasWrappers = false;
            				for(QByteArray ba : signalMethod.parameterTypes()) {
            					if((ba.startsWith((byte)'J') && ba.endsWith(wrapper)) || ba.equals(qvariant)) {
            						hasWrappers = true;
            						break;
            					}
            				}
            				if(!hasWrappers) {
            					for(QByteArray ba : metaMethod.parameterTypes()) {
                					if((ba.startsWith((byte)'J') && ba.endsWith(wrapper)) || ba.equals(qvariant)) {
                						hasWrappers = true;
                						break;
                					}
                				}
            				}
            				if(!hasWrappers) {
            					matchingSignals.add(signal);
            					done = true;
            				}
            			}
        			}
        			if(!done) {
	        			if(receiver instanceof QObject && metaMethod.methodType()==QMetaMethod.MethodType.Signal) {
	        				AbstractSignal signal = metaMethod.toSignal((QObject)receiver);
	        				if(signal!=null) {
	        					for(AbstractSignal _signal : signals()) {
	        						AbstractSignal.Match match = _signal.matchMethodTypes(signal.signalTypes(), ClassAnalyzerUtility.getClass(signal).getName());
	    	        				if(match!=AbstractSignal.Match.NoMatch) {
	    	        					matches.put(_signal, match);
	    	    						matchingSignals.add(_signal);
	    	        				}
	    	        			}
	        					done = true;
	        				}
	        			}else if(metaMethod.methodType()==QMetaMethod.MethodType.Method
	        					|| metaMethod.methodType()==QMetaMethod.MethodType.Slot){
	        				slotMethod = metaMethod.toReflectedMethod();
	        				done = slotMethod!=null;
	        			}
	        			if(!done) {
		        			for(AbstractSignal signal : signals()) {
		        				AbstractSignal.Match match = signal.matchMethodTypes(parameterClassTypes, null, null);
		        				if(match!=AbstractSignal.Match.NoMatch) {
		        					matches.put(signal, match);
		    						matchingSignals.add(signal);
		        				}
		        			}
	        			}
        			}
        		}else {
        			metaMethod = null;
        		}
        	}
        	if (metaMethod == null) {
        		if (slotMethod == null)
        			slotMethod = lookupSlot(receiver, method, null);
        		if (slotMethod != null) {
	        		if (slotMethod.isAnnotationPresent(QtUninvokable.class)) {
	            		throw new QUninvokableSlotException(slotMethod);
	                }
    	        	parameterClassTypes = slotMethod.getParameterTypes();
    	        	Type[] genericParameterTypes = slotMethod.getGenericParameterTypes();
    	        	AnnotatedElement[] annotatedParameterTypes = null;
    	        	if(ClassAnalyzerUtility.useAnnotatedType) 
    	        		annotatedParameterTypes = slotMethod.getAnnotatedParameterTypes();
    	        	for(AbstractSignal signal : signals()) {
    	        		AbstractSignal.Match match = signal.matchMethodTypes(parameterClassTypes, genericParameterTypes, annotatedParameterTypes);
    					if(match!=AbstractSignal.Match.NoMatch) {
    						matches.put(signal, match);
    						matchingSignals.add(signal);
    					}
    				}
    	        }else {
    	        	throw new QNoSuchSlotException(receiver, method);
    	        }
	        }
        	if(matchingSignals.size()>1 && parameterClassTypes!=null) {
        		List<AbstractSignal> _matchingSignals = new ArrayList<>();
        		for (AbstractSignal signal : matchingSignals) {
					if(signal.signalTypes().size()==parameterClassTypes.length) {
						_matchingSignals.add(signal);
					}
				}
        		if(!_matchingSignals.isEmpty())
        			matchingSignals = _matchingSignals;
        	}
        	if(matchingSignals.isEmpty()) {
        		if(parameterClassTypes!=null)
        			throwMismatchException(parameterClassTypes);
        		else
        			throw new QNoSuchSlotException(receiver, method);
        	/*
        	}else if(matchingSignals.size()>1) {
        		String ambinuousSignals = "";
        		for(AbstractSignal signal : matchingSignals) {
        			if(!ambinuousSignals.isEmpty())
        				ambinuousSignals += ", ";
					ambinuousSignals += signal.name()+"("+signal.signalParameters()+")";
        		}
        		throw new QMisfittingSignatureException("Signal is ambiguous. Use overload() to select one of: "+ambinuousSignals); 
    		*/
        	}
        	if(metaMethod==null || !metaMethod.isValid() || matches.get(matchingSignals.get(0))==AbstractSignal.Match.JavaMatch) {
        		return matchingSignals.get(0).addConnectionToMethod(receiver, slotMethod, null, null, connectionType);
        	}
        	else
        		return matchingSignals.get(0).addConnectionToMethod(receiver, metaMethod, connectionType);
        }
        
        @io.qt.QtUninvokable
        protected boolean disconnect(Object receiver, String method) {
	    	boolean result = false;
	    	for(AbstractSignal signal : signals()) {
	    		result |= signal.removeConnection(receiver, method);
	    	}
	        return result;
        }

        @io.qt.QtUninvokable
	    private void throwMismatchException(Class<?>... type) throws QNoSuchSignalException{
	    	String name = this.fullName() + "(";
	    	for (int i = 0; i < type.length; i++) {
				if(i!=0)
					name += ",";
				name += type[i].getTypeName().replace("$", ".");
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
        	String name = core.name(this);
            return name==null ? "unnamed" : name;
        }

        /**
         * Returns the full name of the signal, on the form "package.class.signalName"
         *  @return The fully qualified name of the signal
         */
        @io.qt.QtUninvokable
        public final String fullName() {
        	Class<?> declaringClass = core.declaringClass(this);
        	String name = name();
            return declaringClass == null ? name : declaringClass.getName() + "." + name;
        }
    }
	
	private static class MetaCall implements QMetaObject.Slot0, QtThreadAffineInterface, QtObjectInterface {
        private static final long serialVersionUID = 0L;
        private void writeObject(java.io.ObjectOutputStream s) throws IOException{
        	throw new NotSerializableException(ClassAnalyzerUtility.getClass(this).getName());
    	}
		
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
							this.signalEmitter != null ? ClassAnalyzerUtility.getClass(this.signalEmitter).getName() : "N/A",
									eventReceiver==null ? "(static)" : eventReceiver
						),throwable);
				}else{
					throw new QSignalInvocationException(String.format("Exception caught in signal %1$s: sender=%2$s, receiver=%3$s", 
							signal.fullName(), 
							this.signalEmitter != null ? ClassAnalyzerUtility.getClass(this.signalEmitter).getName() : "N/A",
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
					|| (receiver instanceof NativeUtility.Object && ((NativeUtility.Object)receiver).isDisposed()) )
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
        private static final long serialVersionUID = 0L;

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
                ptr = setQObjectSender(NativeUtility.nativeId((QObject) receiver),
                		NativeUtility.nativeId((QObject)signalEmitter));
        	}else {
        		ptr = 0;
            }
		}
		@Override
		public void close() throws Exception {
            if(signalEmitter instanceof QObject && receiver instanceof QObject){
                resetQObjectSender(NativeUtility.nativeId((QObject) receiver), ptr);
            }
		}
		private final QtSignalEmitterInterface signalEmitter;
		private final Object receiver;
		private final long ptr;
		private native long setQObjectSender(long receiver, long sender);
		private native void resetQObjectSender(long receiver, long data);
	}
	
	@NativeAccess
	static class NativeConnection extends QtObject implements QMetaObject.Connection{
		private NativeConnection(QPrivateConstructor p) {
			super(p);
		}

		@Override
		public boolean isConnected() {
			return isConnected(NativeUtility.checkedNativeId(this));
		}

		@Override
		public QObject sender() {
			return sender(NativeUtility.checkedNativeId(this));
		}

		@Override
		public QObject receiver() {
			return receiver(NativeUtility.checkedNativeId(this));
		}
		
		public QMetaMethod signal() {
			return signal(NativeUtility.checkedNativeId(this));
		}
		
		public QMetaMethod method() {
			return method(NativeUtility.checkedNativeId(this));
		}
		
		public boolean isSlotObject() {
			return isSlotObject(NativeUtility.checkedNativeId(this));
		}
		
		@Override
		public boolean equals(Object other) {
			if(other instanceof NativeConnection) {
				return equals(NativeUtility.checkedNativeId(this), NativeUtility.nativeId((NativeConnection)other));
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
	
	public static QMetaMethod method(QMetaObject.Connection connection) {
		if(connection instanceof NativeConnection)
			return ((NativeConnection)connection).method();
		return null;
	}
	
	public static boolean isSlotObject(QMetaObject.Connection connection) {
		if(connection instanceof NativeConnection)
			return ((NativeConnection)connection).isSlotObject();
		return false;
	}
	
	static class NonConnection implements QMetaObject.Connection{
		@Override
		public boolean isConnected() {
			return false;
		}
		@Override
		public QtSignalEmitterInterface sender() {
			return null;
		}
		@Override
		public Object receiver() {
			return null;
		}
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
        
        AbstractConnection<?> previous;
        AbstractConnection<?> next;
        int id = 0;
        
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
        	return (flags & Disconnected) != Disconnected;
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
			if ((Object)ClassAnalyzerUtility.getClass(this) != ClassAnalyzerUtility.getClass(obj) && !(obj instanceof JavaConnection))
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
    } // private class AbstractConnection
    
    static interface SlotInvoker{
    	void invoke(Object[] _arguments) throws Throwable;
    }

    /**
     * Contains book holding info about a single connection
     */
    private static abstract class AbstractReflectiveConnection extends AbstractConnection<Reference<Object>>{
        public final Method slot;
        public final SlotInvoker slotHandle;
        private List<Supplier<?>> lambdaArgs = null;
        
        private AbstractReflectiveConnection(Reference<Object> receiver, Method slot, MethodHandle slotHandle, List<Supplier<?>> lambdaArgs, Qt.ConnectionType... connectionType) {
            super(receiver, connectionType);
            this.slot = slot;
            if(slotHandle==null) {
            	try {
					slotHandle = ReflectionUtility.getMethodHandle(slot);
				} catch (Exception e) {
				}
            }
			this.slotHandle = ReflectionUtility.getSlotInvoker(slot, slotHandle);
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
				slotHandle.invoke(_arguments);
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
						invokeSignal(slot.getDeclaringClass(), slot, true, returnType, _arguments, _convertTypes);
					}else {
						invokeSignal(receiver, slot, false, returnType, _arguments, _convertTypes);
					}
				} catch (InvocationTargetException e) {
					throw e.getTargetException();
				}
			}
		}
		
		private static native Object invokeSignal(Object receiver, Method method, boolean isStatic, byte returnType, Object args[], byte slotTypes[]);
		
		@Override
		public String toString() {
			return "connection to "+this.slot+" ["+resolveReceiver()+"]";
		}
    	
    	public Object resolveReceiver() {
        	return receiver!=null ? receiver.get() : null;
        }
    } // private class AbstractReflectiveConnection
    
    private static class ReflectiveConnection extends AbstractReflectiveConnection{
    	
    	private ReflectiveConnection(Reference<Object> receiver, Method slot, MethodHandle slotHandle, List<Supplier<?>> lambdaArgs, Qt.ConnectionType... connectionType) {
    		super(receiver, slot, slotHandle, lambdaArgs, connectionType);
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

	private static native final Connection connectNative(long senderObjectId, int signal, long senderMetaObjectId, long contextNativeId, AbstractConnection<?> connection, int argumentCount, int connectionType);
	
	private static native final Connection connectNativeToMetaMethod(long senderObjectId, int signal, long senderMetaObjectId, long receiverObjectId, long slotId, int connectionType);
	
	private static native final boolean disconnectNative(long senderObjectId, int signal, long senderMetaObjectId, long receiverObjectId, long slotId);

	protected native static void emitNativeSignal(QObject senderNative, int methodIndex, long senderMetaObjectId, int defaults, Object args[]);
	
    protected static void checkConnectionToDisposedSignalImpl(QMetaObject.DisposedSignal signal, Object receiver, boolean slotObject) {
    	if(receiver==signal) {
			throw new IllegalArgumentException("Cannot connect a signal to itself.");
		}else if(receiver instanceof QtObjectInterface) {
			QMetaObject.DisposedSignal disposed = NativeUtility.getSignalOnDispose((QtObjectInterface)receiver, false);
			if(signal==disposed) {
				if(slotObject) {
					throw new IllegalArgumentException("Cannot connect a lambda expression capturing a certain object to this object's \"disposed\" signal.");
				}else {
					throw new IllegalArgumentException("Cannot connect an object to its own \"disposed\" signal.");
				}
			}
		}
    }
    
	/**
	 * Disconnect all connections made from the given object to a specific object.
	 */
	protected static boolean disconnectAll(QtSignalEmitterInterface sender, Object receiver) {
		boolean result = false;
		if (sender != null) {
			Class<?> cls = ClassAnalyzerUtility.getClass(sender);
			while (cls != null && cls != Object.class) {
				Field fields[] = cls.getDeclaredFields();

				for (Field f : fields) {
					if (!java.lang.reflect.Modifier.isStatic(f.getModifiers())
							&& AbstractSignal.class.isAssignableFrom(f.getType())) {
						AbstractSignal signal = (AbstractSignal) ReflectionUtility.readField(sender, f);
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
				QMetaObject.DisposedSignal disposed = NativeUtility.getSignalOnDispose((QtObjectInterface) sender, false);
				if (disposed != null) {
					result |= disposed.disconnect(receiver);
				}
			}
		}
		return result;
	}

	protected static boolean disconnectOne(QMetaObject.Connection connection) {
		if (connection instanceof SignalUtility.JavaConnection) {
			SignalUtility.JavaConnection javaConnection = (SignalUtility.JavaConnection) connection;
			SignalUtility.AbstractSignal signal = javaConnection.signal();
			if (signal != null) {
				return signal.removeConnection(connection);
			}
		} else if (connection instanceof SignalUtility.NativeConnection) {
			SignalUtility.NativeConnection nativeConnection = (SignalUtility.NativeConnection) connection;
			long nativeId = NativeUtility.nativeId(nativeConnection);
			if (nativeId != 0)
				return disconnectConnection(nativeId);
		}
		return false;
	}

	static native boolean disconnectConnection(long connection__id);
    
	@NativeAccess
	private static Method lookupSlot(Object object, String signature, Class<?>[] requiredParameters) {
		Class<?> cls = (object instanceof Class) ? (Class<?>) object : ClassAnalyzerUtility.getClass(object);
		int h = 1;
		h = 31 * h + cls.hashCode();
		h = 31 * h + signature.hashCode();
		if(requiredParameters!=null) {
			h = 31 * h + requiredParameters.length;
	        for (Class<?> element : requiredParameters) {
	            h = 31 * h + (element == null ? 0 : element.hashCode());
	        }
		}
		return lookupSlots.computeIfAbsent(h,
				argumentsHashCode -> {
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

//			            if (strTypes.length() == 0)
//			                argumentTypes = new String[0];
//			            else
//			                argumentTypes = RetroTranslatorHelper.split(strTypes, ",");

						for (int i = 0; i < argumentTypes.length; ++i)
							argumentTypes[i] = argumentTypes[i].replace(" ", "");

						return findFunctionRecursive(cls, name, argumentTypes);
					} else {
						return findFunctionRecursive(cls, signature, requiredParameters);
					}
				}
			);
	}

	private static Method findFunctionRecursive(Class<?> cls, String functionName, String argumentTypes[]) {
		Method result = null;
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
				result = m;
				break;
			}
		}
		if (result == null) {
			if (cls.getSuperclass() == null)
				return null;
			else
				return findFunctionRecursive(cls.getSuperclass(), functionName, argumentTypes);
		}
		return result;
	}

	private static Method findFunctionRecursive(Class<?> cls, String functionName, Class<?> argumentTypes[]) {
		Method result = null;
		for (Method m : cls.getDeclaredMethods()) {
			if (m.getName().equals(functionName) && Arrays.deepEquals(argumentTypes, m.getParameterTypes())) {
				result = m;
				break;
			}
		}
		if (result == null) {
			if (cls.getSuperclass() == null)
				return null;
			else
				return findFunctionRecursive(cls.getSuperclass(), functionName, argumentTypes);
		}
		return result;
	}
	
	static List<SignalParameterType> resolveSignal(Field field) {
		return resolveSignal(field, null, null);
	}
	
	static List<SignalParameterType> resolveSignal(Method emitMethod) {
		return resolveSignal(emitMethod, null, null);
	}
	
	@NativeAccess
	static List<SignalParameterType> resolveSignal(Object data, boolean[] argumentIsPointerType, boolean[] argumentIsReferenceType) {
		List<SignalParameterType> typeList = Collections.emptyList();
		Class<?>[] classTypes = null;
		Type[] types = null;
		AnnotatedElement[] actualTypes = null;
		if(data instanceof QPair) {
			QPair<?,?> pair = (QPair<?,?>)data;
			if(pair.first instanceof AbstractMultiSignal<?>) {
				AbstractMultiSignal<?> multisignal = (AbstractMultiSignal<?>)pair.first;
				Map<List<Class<?>>,EmitMethodInfo> emitMethodInfos = findEmitMethods(ClassAnalyzerUtility.getClass(multisignal));
				if(pair.second instanceof Integer) {
					AbstractSignal signal = multisignal.signal((int)pair.second);
					if(signal.core instanceof MultiSignalAnalyingSignalCore) {
						MultiSignalAnalyingSignalCore core = (MultiSignalAnalyingSignalCore)signal.core;
						classTypes = core.types;
						EmitMethodInfo emitMethodInfo = emitMethodInfos.get(Arrays.asList(classTypes));
						if(emitMethodInfo!=null) {
							types = emitMethodInfo.method.getGenericParameterTypes();
							if(ClassAnalyzerUtility.useAnnotatedType)
								actualTypes = emitMethodInfo.method.getAnnotatedParameterTypes();
						}
					}
				}else if(pair.second instanceof Class[]) {
					EmitMethodInfo emitMethodInfo = emitMethodInfos.get(Arrays.asList((Class[])pair.second));
					if(emitMethodInfo!=null) {
						types = emitMethodInfo.method.getGenericParameterTypes();
						if(ClassAnalyzerUtility.useAnnotatedType)
							actualTypes = emitMethodInfo.method.getAnnotatedParameterTypes();
					}
				}
			}
		}else if(data instanceof Field) {
			Type genericType = ((Field)data).getGenericType();
			if (genericType instanceof java.lang.reflect.ParameterizedType) {
				java.lang.reflect.ParameterizedType p = (java.lang.reflect.ParameterizedType) genericType;
				types = p.getActualTypeArguments();
			}
			if(ClassAnalyzerUtility.useAnnotatedType) {
				AnnotatedElement annotatedType = ((Field)data).getAnnotatedType();
				if (annotatedType instanceof java.lang.reflect.AnnotatedParameterizedType) {
					java.lang.reflect.AnnotatedParameterizedType p = (java.lang.reflect.AnnotatedParameterizedType) annotatedType;
					actualTypes = p.getAnnotatedActualTypeArguments();
				}
			}
		}else if(data instanceof Method) {
			classTypes = ((Method)data).getParameterTypes();
			types = ((Method)data).getGenericParameterTypes();
			if(ClassAnalyzerUtility.useAnnotatedType)
				actualTypes = ((Method)data).getAnnotatedParameterTypes();
		}else if(data instanceof Class[]) {
			classTypes = (Class<?>[])data;
		}
		
		if(actualTypes!=null) {
			for (int j = 0; j < actualTypes.length; ++j) {
				java.lang.reflect.AnnotatedType actualType = (java.lang.reflect.AnnotatedType)actualTypes[j];
				boolean isPrimitive = actualType.isAnnotationPresent(QtPrimitiveType.class);
				boolean isPointer = actualType.isAnnotationPresent(QtPointerType.class);
				if(!isPointer && argumentIsPointerType!=null && j<argumentIsPointerType.length)
					isPointer = argumentIsPointerType[j];
				QtReferenceType referenceType = actualType.getAnnotation(QtReferenceType.class);
				boolean isReference = !isPointer && referenceType!=null && !referenceType.isConst();
				if(!isReference && argumentIsReferenceType!=null && j<argumentIsReferenceType.length)
					isReference = argumentIsReferenceType[j];
				if (!isPrimitive) {
					AnnotatedElement annotatedOwnerType = RetroHelper.getAnnotatedOwnerType(actualType);
					if (annotatedOwnerType != null) {
						isPrimitive = annotatedOwnerType.isAnnotationPresent(QtPrimitiveType.class);
					}
				}

				Type type = actualType.getType();
				Class<?> classType = ClassAnalyzerUtility.toClass(type);
				if (isPrimitive) {
					if (classType == Integer.class) {
						classType = int.class;
					} else if (classType == Short.class) {
						classType = short.class;
					} else if (classType == Byte.class) {
						classType = byte.class;
					} else if (classType == Long.class) {
						classType = long.class;
					} else if (classType == Double.class) {
						classType = double.class;
					} else if (classType == Float.class) {
						classType = float.class;
					} else if (classType == Boolean.class) {
						classType = boolean.class;
					} else if (classType == Character.class) {
						classType = char.class;
					}
				}
				
				SignalParameterType signalType = new SignalParameterType(classType, type, actualType, isPointer, isReference);
				if (j == 0) {
					if (actualTypes.length > 1) {
						typeList = new ArrayList<>();
						typeList.add(signalType);
					} else {
						typeList = Collections.singletonList(signalType);
					}
				} else {
					typeList.add(signalType);
				}
			}
			typeList = actualTypes.length > 1 ? Collections.unmodifiableList(typeList) : typeList;
		}else if(types!=null) {
			for (int j = 0; j < types.length; ++j) {
				Class<?> classType = ClassAnalyzerUtility.toClass(types[j]);
				
				SignalParameterType signalType = new SignalParameterType(classType, types[j], null, false, false);
				if (j == 0) {
					if (types.length > 1) {
						typeList = new ArrayList<>();
						typeList.add(signalType);
					} else {
						typeList = Collections.singletonList(signalType);
					}
				} else {
					typeList.add(signalType);
				}
			}
			typeList = types.length > 1 ? Collections.unmodifiableList(typeList) : typeList;
		}else if(classTypes!=null) {
			for (int j = 0; j < classTypes.length; ++j) {
				Class<?> classType = classTypes[j];
				boolean isPointer = false;
				if(argumentIsPointerType!=null && j<argumentIsPointerType.length)
					isPointer = argumentIsPointerType[j];
				boolean isReference = false;
				if(argumentIsReferenceType!=null && j<argumentIsReferenceType.length)
					isReference = argumentIsReferenceType[j];
				SignalParameterType signalType = new SignalParameterType(classType, classType, null, isPointer, isReference);
				if (j == 0) {
					if (classTypes.length > 1) {
						typeList = new ArrayList<>();
						typeList.add(signalType);
					} else {
						typeList = Collections.singletonList(signalType);
					}
				} else {
					typeList.add(signalType);
				}
			}
			typeList = classTypes.length > 1 ? Collections.unmodifiableList(typeList) : typeList;
		}
		
		return typeList;
	}

	@NativeAccess
    private static class SignalInfo{
		@NativeAccess
    	private SignalInfo(long enclosingMetaObject, int methodIndex, List<SignalParameterType> signalTypes) {
			super();
			this.enclosingMetaObject = enclosingMetaObject;
			this.methodIndex = methodIndex;
			this.signalTypes = signalTypes;
		}
		final int methodIndex;
		final long enclosingMetaObject;
    	final List<SignalParameterType> signalTypes;
    }
    
    private static native SignalInfo signalInfo(Object containingObject, QMetaObject metaObject, Field field, Method emitMethod);
    
    static final class EmitMethodInfo{
    	public EmitMethodInfo(Method method, long methodId, List<SignalParameterType> parameterTypes) {
			super();
			this.method = method;
			this.methodId = methodId;
			this.parameterTypes = parameterTypes;
		}
		public final Method method;
		public final long methodId;
		public final List<SignalParameterType> parameterTypes;
    }

	private final static Map<Class<?>,Map<List<Class<?>>,EmitMethodInfo>> allEmitMethods = Collections.synchronizedMap(new HashMap<>());
	
	static Map<List<Class<?>>,EmitMethodInfo> findEmitMethods(Class<?> signalClass) {
		return allEmitMethods.computeIfAbsent(signalClass, cls->{
			Map<List<Class<?>>,EmitMethodInfo> methodMap = Collections.emptyMap();
			if (SignalUtility.AbstractSignal.class.isAssignableFrom(signalClass)
					|| SignalUtility.AbstractMultiSignal.class.isAssignableFrom(signalClass)) {
				while (cls != SignalUtility.AbstractSignal.class
						&& cls != SignalUtility.AbstractMultiSignal.class) {
					Method methods[] = cls.getDeclaredMethods();
					for (Method method : methods) {
						if (!method.isSynthetic() 
                                && !method.isBridge() 
                                && !Modifier.isStatic(method.getModifiers())
                                && method.getName().equals("emit") && method.getParameterCount()<10) {
							List<SignalParameterType> params = SignalUtility.resolveSignal(method);
							if(methodMap.isEmpty()) {
								methodMap = Collections.singletonMap(Collections.unmodifiableList(Arrays.asList(method.getParameterTypes())), 
																	 new EmitMethodInfo(method, fromReflectedMethod(method), params));
							}else {
								if(methodMap.size()==1) {
									methodMap = new HashMap<>(methodMap);
								}
								if(!methodMap.containsKey(params))
									methodMap.put(Collections.unmodifiableList(Arrays.asList(method.getParameterTypes())), 
												  new EmitMethodInfo(method, fromReflectedMethod(method), params));
							}
						}
					}
					cls = cls.getSuperclass();
				}
			}
			if(methodMap.size()>1)
				methodMap = Collections.unmodifiableMap(methodMap);
			return methodMap;
		});
	}
	
	private static native long fromReflectedMethod(Method method);
    
    private static native long metaObjectId(QMetaObject metaObject);
    
    private static native boolean isDynamic(QMetaObject metaObject);
	
	private static native void initializeMultiSignal(AbstractMultiSignal<?> signal, Class<?> declaringClass, Field field);
	
	static class SignalClasses{
		private SignalClasses(Class<?>[] publicSignals, Class<?>[] privateSignals) {
			super();
			this.publicSignals = publicSignals;
			this.privateSignals = privateSignals;
		}
		private final Class<?>[] publicSignals;
		private final Class<?>[] privateSignals;
		
		public Class<?> getPublicSignal(int i) {
			return publicSignals[i];
		}
		public Class<?> getPrivateSignal(int i) {
			return privateSignals[i];
		}
	}
	
	private final static Map<Class<?>,SignalClasses> allSignalTypes = Collections.synchronizedMap(new HashMap<>());
	
	static SignalClasses signalClasses(Class<?> enclosingClass){
		return allSignalTypes.computeIfAbsent(enclosingClass, enclosingType->{
					Class<?>[] publicSignals = new Class[10];
					Class<?>[] privateSignals = new Class[10];
					for(Class<?> type : enclosingType.getClasses()) {
						if(!Modifier.isAbstract(type.getModifiers()) && AbstractSignal.class.isAssignableFrom(type)) {
							String sname = type.getSimpleName();
							Class<?>[] _signals;
							if(sname.startsWith("PrivateSignal")) {
								sname = sname.substring(13);
								_signals = privateSignals;
							}else if(sname.startsWith("Signal")) {
								sname = sname.substring(6);
								_signals = publicSignals;
							}else continue;
							try {
								_signals[Integer.parseInt(sname)] = type;
							} catch (Exception e) {
							}
						}
					}
					return new SignalClasses(publicSignals, privateSignals);
				});
	}
	
	private static final Map<Class<?>, Function<?,?>> signalFactories = Collections.synchronizedMap(new HashMap<>());
	
	@SuppressWarnings("unchecked")
	static <Signal> Function<Object,Signal> getSignalFactory(Class<?> signalObjectType) {
		return (Function<Object,Signal>)signalFactories.computeIfAbsent(signalObjectType, cls->{
			try {
				Constructor<?> constructor;
				if(cls.getEnclosingClass()==QObject.class) {
					constructor = cls.getDeclaredConstructor(QObject.class);
				}else if(cls.getEnclosingClass()==QInstanceMemberSignals.class) {
					constructor = cls.getDeclaredConstructor(Object.class);
				}else if(cls.getEnclosingClass()==QStaticMemberSignals.class || cls==QMetaObject.DisposedSignal.class) {
					constructor = cls.getDeclaredConstructor(Class.class);
				}else {
					constructor = null;
				}
				return ReflectionUtility.getFactory1(constructor);
			} catch (Throwable e) {
				e.printStackTrace();
			}
			return null;
		});
	}
}
