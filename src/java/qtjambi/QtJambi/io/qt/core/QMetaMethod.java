/****************************************************************************
**
** Copyright (C) 2009-2020 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import java.io.Serializable;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;
import java.util.Objects;

import io.qt.QUnsuccessfulInvocationException;
import io.qt.QtObjectInterface;
import io.qt.QtUnlistedEnum;
import io.qt.core.QMetaObject.Method0;
import io.qt.core.QMetaObject.Method1;
import io.qt.core.QMetaObject.Method2;
import io.qt.core.QMetaObject.Method3;
import io.qt.core.QMetaObject.Method4;
import io.qt.core.QMetaObject.Method5;
import io.qt.core.QMetaObject.Method6;
import io.qt.core.QMetaObject.Method7;
import io.qt.core.QMetaObject.Method8;
import io.qt.core.QMetaObject.Method9;
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
import io.qt.QtUninvokable;
import io.qt.QtSignalEmitterInterface;
import io.qt.internal.NativeAccess;
import io.qt.internal.QtJambiInternal;

public final class QMetaMethod {
	@QtUnlistedEnum
	public enum Access { Private, Protected, Public };
	@QtUnlistedEnum
	public enum MethodType { Method, Signal, Slot, Constructor };
	
	private final static java.util.logging.Logger logger = java.util.logging.Logger.getLogger("io.qt.internal");
	private final String name;
	private final Class<?> returnType;
	private final int returnMetaType;
	private final List<Class<?>> parameterTypes;
	private final int[] parameterMetaTypes;
	private final String tag;
	private final int methodIndex;
	private final QMetaObject enclosingMetaObject;
	private final Access access;
	private final MethodType methodType;
	@NativeAccess
	private final static Object UNSUPPORTED = new Object();
	
    private QMetaMethod(QMetaObject enclosingMetaObject, String name, String returnType, List<String> parameterTypes, int returnMetaType, int[] parameterMetaTypes, String tag, int methodIndex, int access, int methodType) {
		super();
		this.enclosingMetaObject = enclosingMetaObject;
		this.name = name;
		this.tag = tag;
		this.returnMetaType = returnMetaType;
		this.parameterMetaTypes = parameterMetaTypes;
		this.methodType = MethodType.values()[methodType];
		if(this.methodType==MethodType.Constructor) {
			this.returnType = null;
		}else {
			Class<?> type = Object.class;
			switch(returnType) {
			case "": type = Object.class; break;
			case "int": type = int.class; break;
			case "short": type = short.class; break;
			case "long": type = long.class; break;
			case "byte": type = byte.class; break;
			case "boolean": type = boolean.class; break;
			case "double": type = double.class; break;
			case "char": type = char.class; break;
			case "float": type = float.class; break;
			case "void": type = void.class; break;
			default:
				try {
					type = Class.forName(returnType.replace('/', '.'));
				} catch (Throwable e) {
					logger.log(java.util.logging.Level.SEVERE, e, ()->"Unable to determine java class for return type "+name);
				}
			}
			this.returnType = type;
		}
		this.parameterTypes = new ArrayList<>();
		for (String parameterType : parameterTypes) {
			Class<?> type = Object.class;
			switch(parameterType) {
			case "": type = Object.class; break;
			case "int": type = int.class; break;
			case "short": type = short.class; break;
			case "long": type = long.class; break;
			case "byte": type = byte.class; break;
			case "boolean": type = boolean.class; break;
			case "double": type = double.class; break;
			case "char": type = char.class; break;
			case "float": type = float.class; break;
			default:
				try {
					type = Class.forName(parameterType.replace('/', '.'));
				} catch (Throwable e) {
					logger.log(java.util.logging.Level.SEVERE, e, ()->"Unable to determine java class for argument type "+name);
				}
			}
			this.parameterTypes.add(type);
		}
		this.methodIndex = methodIndex;
		this.access = Access.values()[access];
	}
    
    private QMetaMethod(QMetaObject enclosingMetaObject, String name, Class<?> returnType, List<Class<?>> parameterTypes, int returnMetaType, int[] parameterMetaTypes, String tag, int methodIndex, int access, int methodType) {
		super();
		this.enclosingMetaObject = enclosingMetaObject;
		this.name = name;
		this.tag = tag;
		this.returnMetaType = returnMetaType;
		this.parameterMetaTypes = parameterMetaTypes;
		this.methodType = MethodType.values()[methodType];
		if(this.methodType==MethodType.Constructor) {
			this.returnType = null;
		}else {
			this.returnType = returnType;
		}
		this.parameterTypes = parameterTypes;
		this.methodIndex = methodIndex;
		this.access = Access.values()[access];
	}
    
    @Override
    @QtUninvokable
    public final String toString() {
        return methodSignature();
    }

    @QtUninvokable
	public final String name() {
		return name;
	}

    @QtUninvokable
	public final Class<?> returnType() {
		return returnType;
	}
    
    @QtUninvokable
	public final int returnMetaType() {
		return returnMetaType;
	}
	
    @QtUninvokable
	public final String tag() {
		return tag;
	}

    @QtUninvokable
	public final String methodSignature() {
		StringBuilder args = new StringBuilder();
		for(int i=0; i<parameterTypes.size(); ++i) {
			if(i!=0)
				args.append(',');
			Class<?> parameterType = parameterTypes.get(i);
			if(parameterType.isArray()) {
				int arrayDepth = 1;
				Class<?> componentType = parameterType.getComponentType();
				while(true) {
					if(componentType.isArray()) {
						++arrayDepth;
						componentType = componentType.getComponentType();
					}else {
						args.append(componentType.getName());
						for (int j = 0; j < arrayDepth; j++) {
							args.append("[]");
						}
						break;
					}
				}
			}else {
				args.append(parameterType.getName());
			}
		}
		return String.format("%1$s(%2$s)", name, args);
	}
    
    @QtUninvokable
	public final List<Class<?>> parameterTypes() {
		return Collections.unmodifiableList(parameterTypes);
	}
    
    @QtUninvokable
	public final int[] parameterMetaTypes() {
		return Arrays.copyOf(parameterMetaTypes, parameterMetaTypes.length);
	}
    
    @QtUninvokable
	public final int parameterMetaType(int i) {
		return parameterMetaTypes[i];
	}

    @QtUninvokable
    public final Access access() {
		return access;
	}

    @QtUninvokable
    public final MethodType methodType() {
		return methodType;
	}

    @QtUninvokable
	public final QMetaObject enclosingMetaObject() {
		return enclosingMetaObject;
	}
    
    @QtUninvokable
	public int methodIndex() {
		return methodIndex;
	}
	
    @Override
    @QtUninvokable
	public final int hashCode() {
		final int prime = 31;
		int result = 1;
		result = prime * result + enclosingMetaObject.hashCode();
		result = prime * result + methodIndex;
		return result;
	}

	@Override
    @QtUninvokable
	public final boolean equals(Object obj) {
		if (this == obj)
			return true;
		if (obj == null)
			return false;
		if (getClass() != obj.getClass())
			return false;
		QMetaMethod other = (QMetaMethod) obj;
		if (!enclosingMetaObject.equals(other.enclosingMetaObject))
			return false;
		if (methodIndex != other.methodIndex)
			return false;
		return true;
	}
	
    @QtUninvokable
	public final Object invoke(QObject object, Object... args) throws IllegalArgumentException, QUnsuccessfulInvocationException {
		return invoke(object, Qt.ConnectionType.AutoConnection, args);
	}
	
    @QtUninvokable
	public final Object invoke(QObject object, Qt.ConnectionType connection, Object... args) throws IllegalArgumentException, QUnsuccessfulInvocationException {
		long object_id = io.qt.internal.QtJambiInternal.checkedNativeId(Objects.requireNonNull(object));
		if(parameterTypes.size() != args.length) {
			throw new IllegalArgumentException(String.format("Wrong number of arguments. expected: %2$s, given: %1$s", args.length, parameterTypes.size()));
		}
		if(connection==Qt.ConnectionType.BlockingQueuedConnection && object.thread()==QThread.currentThread()) {
			throw new QUnsuccessfulInvocationException("Blocking-queued invocation on object whose thread is the current thread is not allowed.");
		}
		Class<?>[] argClassTypes = new Class<?>[1+parameterTypes.size()];
		argClassTypes[0] = returnType==null ? void.class : returnType;
		if(argClassTypes[0]!=void.class && argClassTypes[0]!=Void.class) {
			if(connection==Qt.ConnectionType.QueuedConnection) {
				throw new QUnsuccessfulInvocationException("Unable to invoke methods with return values in queued connections.");
			}else if(connection==Qt.ConnectionType.AutoConnection) {
				if(QThread.currentThread() != object.thread()) {
					throw new QUnsuccessfulInvocationException("Unable to invoke methods with return values in queued connections (auto connection with different threads).");
				}
			}
		}
		for(int i=0; i<parameterTypes.size(); ++i) {
			Class<?> argClassType = parameterTypes.get(i);
			argClassTypes[i+1] = argClassType;
			try {
				if(argClassType.isPrimitive()) {
					if(args[i]==null) {
						throw new IllegalArgumentException(String.format("Null not allowed for argument %1$s of type %2$s.", (i+1), argClassType.getName()));
					}else if(argClassType==int.class) {
						args[i] = Integer.class.cast(args[i]);
					}else if(argClassType==short.class) {
						args[i] = Short.class.cast(args[i]);
					}else if(argClassType==byte.class) {
						args[i] = Byte.class.cast(args[i]);
					}else if(argClassType==boolean.class) {
						args[i] = Boolean.class.cast(args[i]);
					}else if(argClassType==char.class) {
						args[i] = Character.class.cast(args[i]);
					}else if(argClassType==long.class) {
						args[i] = Long.class.cast(args[i]);
					}else if(argClassType==float.class) {
						args[i] = Float.class.cast(args[i]);
					}else if(argClassType==double.class) {
						args[i] = Double.class.cast(args[i]);
					}
				}else if(argClassType==QMetaType.GenericFlag.class && args[i]!=null && !(args[i] instanceof QMetaType.GenericFlag)){
				}else if(argClassType==QMetaType.GenericFlags.class && args[i]!=null && !(args[i] instanceof QMetaType.GenericFlags)){
				}else if(argClassType==QMetaType.GenericEnumerator.class && args[i]!=null && !(args[i] instanceof QMetaType.GenericEnumerator)){
				}else if(argClassType==QMetaType.GenericByteEnumerator.class && args[i]!=null && !(args[i] instanceof QMetaType.GenericByteEnumerator)){
				}else if(argClassType==QMetaType.GenericShortEnumerator.class && args[i]!=null && !(args[i] instanceof QMetaType.GenericShortEnumerator)){
				}else if(argClassType==QMetaType.GenericLongEnumerator.class && args[i]!=null && !(args[i] instanceof QMetaType.GenericLongEnumerator)){
				}else if(argClassType==QMetaType.GenericFlag.class && args[i]!=null && !(args[i] instanceof QMetaType.GenericFlag)){
				}else if(argClassType==QMetaType.GenericGadget.class && args[i]!=null && !(args[i] instanceof QMetaType.GenericGadget)){
				}else if(argClassType==QMetaType.GenericValue.class && args[i]!=null && !(args[i] instanceof QMetaType.GenericValue)){
				}else {
					args[i] = argClassType.cast(args[i]);
				}
			} catch (ClassCastException e) {
				throw new IllegalArgumentException(String.format("Wrong argument type for argument %1$s. expected: %2$s, given: %3$s", (i+1), argClassType.getName(), args[i]==null ? "null" : args[i].getClass().getName()), e);
			}
		}
		Object result = _invoke(object_id, enclosingMetaObject.metaObjectPointer, methodIndex, argClassTypes, connection.ordinal(), args);
		if(connection!=Qt.ConnectionType.QueuedConnection) {
			if(result==UNSUPPORTED) {
				throw new QUnsuccessfulInvocationException(this.methodSignature());
			}
			if(returnType!=void.class) {
				if(returnType.isPrimitive()) {
					if(returnType==int.class) {
						return Integer.class.cast(result);
					}else if(returnType==short.class) {
						return Short.class.cast(result);
					}else if(returnType==byte.class) {
						return Byte.class.cast(result);
					}else if(returnType==boolean.class) {
						return Boolean.class.cast(result);
					}else if(returnType==char.class) {
						return Character.class.cast(result);
					}else if(returnType==long.class) {
						return Long.class.cast(result);
					}else if(returnType==float.class) {
						return Float.class.cast(result);
					}else if(returnType==double.class) {
						return Double.class.cast(result);
					}else{
						throw new NullPointerException("Method returns null for "+returnType.getName());
					}
				}else {
					return returnType.cast(result);
				}
			}else {
				return null;
			}
		}else {
			return null;
		}
	}
	
    @QtUninvokable
	private static native Object _invoke(long object__id, long metaObjectPointer, int methodIndex, Class<?>[] argClassTypes, int connection, Object[] args);
	
    @QtUninvokable
	public final Object invokeOnGadget(Object object, Object... args) throws IllegalArgumentException, QUnsuccessfulInvocationException {
		if(parameterTypes.size() != args.length) {
			throw new IllegalArgumentException(String.format("Wrong number of arguments. expected: %2$s, given: %1$s", args.length, parameterTypes.size()));
		}
		long object_id = 0;
		if(object instanceof QtObjectInterface){
			object_id = io.qt.internal.QtJambiInternal.checkedNativeId((QtObjectInterface)object);
		}
		{
			QMetaObject objectMO = QMetaObject.forGadget(object);
			if(objectMO!=enclosingMetaObject && !objectMO.inherits(enclosingMetaObject)) {
				throw new IllegalArgumentException(String.format("Given gadget is not an instance of %1$s", enclosingMetaObject.className()));
			}
		}
		Class<?>[] argClassTypes = new Class<?>[1+parameterTypes.size()];
		argClassTypes[0] = returnType==null ? void.class : returnType;
		for(int i=0; i<parameterTypes.size(); ++i) {
			Class<?> argClassType = parameterTypes.get(i);
			argClassTypes[i+1] = argClassType;
			try {
				if(argClassType.isPrimitive()) {
					if(args[i]==null) {
						throw new IllegalArgumentException(String.format("Null not allowed for argument %1$s of type %2$s.", (i+1), argClassType.getName()));
					}else if(argClassType==int.class) {
						args[i] = Integer.class.cast(args[i]);
					}else if(argClassType==short.class) {
						args[i] = Short.class.cast(args[i]);
					}else if(argClassType==byte.class) {
						args[i] = Byte.class.cast(args[i]);
					}else if(argClassType==boolean.class) {
						args[i] = Boolean.class.cast(args[i]);
					}else if(argClassType==char.class) {
						args[i] = Character.class.cast(args[i]);
					}else if(argClassType==long.class) {
						args[i] = Long.class.cast(args[i]);
					}else if(argClassType==float.class) {
						args[i] = Float.class.cast(args[i]);
					}else if(argClassType==double.class) {
						args[i] = Double.class.cast(args[i]);
					}
				}else if(argClassType==QMetaType.GenericFlag.class && args[i]!=null && !(args[i] instanceof QMetaType.GenericFlag)){
				}else if(argClassType==QMetaType.GenericFlags.class && args[i]!=null && !(args[i] instanceof QMetaType.GenericFlags)){
				}else if(argClassType==QMetaType.GenericEnumerator.class && args[i]!=null && !(args[i] instanceof QMetaType.GenericEnumerator)){
				}else if(argClassType==QMetaType.GenericByteEnumerator.class && args[i]!=null && !(args[i] instanceof QMetaType.GenericByteEnumerator)){
				}else if(argClassType==QMetaType.GenericShortEnumerator.class && args[i]!=null && !(args[i] instanceof QMetaType.GenericShortEnumerator)){
				}else if(argClassType==QMetaType.GenericLongEnumerator.class && args[i]!=null && !(args[i] instanceof QMetaType.GenericLongEnumerator)){
				}else if(argClassType==QMetaType.GenericFlag.class && args[i]!=null && !(args[i] instanceof QMetaType.GenericFlag)){
				}else if(argClassType==QMetaType.GenericGadget.class && args[i]!=null && !(args[i] instanceof QMetaType.GenericGadget)){
				}else if(argClassType==QMetaType.GenericValue.class && args[i]!=null && !(args[i] instanceof QMetaType.GenericValue)){
				}else {
					args[i] = argClassType.cast(args[i]);
				}
			} catch (ClassCastException e) {
				throw new IllegalArgumentException(String.format("Wrong argument type for argument %1$s. expected: %2$s, given: %3$s", (i+1), argClassType.getName(), args[i]==null ? "null" : args[i].getClass().getName()), e);
			}
		}
		Object result = _invokeOnGadget(object_id, object, enclosingMetaObject.metaObjectPointer, methodIndex, argClassTypes, args);
		if(result==UNSUPPORTED) {
			throw new QUnsuccessfulInvocationException(this.methodSignature());
		}
		if(returnType!=void.class) {
			if(returnType.isPrimitive()) {
				if(returnType==int.class) {
					return Integer.class.cast(result);
				}else if(returnType==short.class) {
					return Short.class.cast(result);
				}else if(returnType==byte.class) {
					return Byte.class.cast(result);
				}else if(returnType==boolean.class) {
					return Boolean.class.cast(result);
				}else if(returnType==char.class) {
					return Character.class.cast(result);
				}else if(returnType==long.class) {
					return Long.class.cast(result);
				}else if(returnType==float.class) {
					return Float.class.cast(result);
				}else if(returnType==double.class) {
					return Double.class.cast(result);
				}else{
					throw new NullPointerException("Method returns null for "+returnType.getName());
				}
			}else {
				return returnType.cast(result);
			}
		}else {
			return null;
		}
	}
	
    @QtUninvokable
	private static native Object _invokeOnGadget(long object_id, Object object, long metaObjectPointer, int methodIndex, Class<?>[] argClassTypes, Object[] args);
	
    @QtUninvokable
	public java.lang.reflect.Method toReflectedMethod() {
		if(methodType==MethodType.Method || methodType==MethodType.Slot) {
			try {
				return this.enclosingMetaObject.type().getDeclaredMethod(name, parameterTypes.toArray(new Class[parameterTypes.size()]));
			} catch (NoSuchMethodException | SecurityException e) {
			}
		}else {
			throw new IllegalArgumentException("Method " + this + " is a " + methodType.name().toLowerCase() + ".");
		}
		return null;
	}
	
    @QtUninvokable
	public java.lang.reflect.Constructor<?> toReflectedConstructor() {
		if(methodType==MethodType.Constructor) {
			try {
				return this.enclosingMetaObject.type().getDeclaredConstructor(parameterTypes.toArray(new Class[parameterTypes.size()]));
			} catch (NoSuchMethodException | SecurityException e) {
			}
		}else {
			throw new IllegalArgumentException("Method " + this + " is not a constructor.");
		}
		return null;
	}
	
    @QtUninvokable
	public QMetaObject.AbstractSignal toSignal(QObject sender) {
		if(methodType==MethodType.Signal) {
			return QtJambiInternal.findSignal(sender, name, parameterTypes.toArray(new Class[parameterTypes.size()]));
		}else {
			throw new IllegalArgumentException("Method " + this + " is not a signal.");
		}
	}
    
    private static native QMetaMethod methodByIndex(QMetaObject metaObject, int methodIndex);
    
    @QtUninvokable
    public static QMetaMethod fromSignal(QMetaObject.AbstractSignal signal) {
    	QMetaMethod method = null;
		QtSignalEmitterInterface containingObject = signal.containingObject();
    	if(containingObject instanceof QObject) {
    		if(signal.methodIndex()>=0) {
    			method = methodByIndex(((QObject)containingObject).metaObject(), signal.methodIndex());
    		}else{
	    		List<Class<?>> signalTypeClasses = signal.argumentTypes();
	    		method = ((QObject)containingObject).metaObject().method(signal.name(), signalTypeClasses.toArray(new Class[signalTypeClasses.size()]));
    		}
    	}
    	return method;
    }
	
	public static QMetaMethod fromReflectedConstructor(java.lang.reflect.Constructor<?> constructor) {
		QMetaObject mo = QMetaObject.forType(constructor.getDeclaringClass());
		if(mo!=null) {
			return mo.constructor(constructor.getParameterTypes());
		}
		return null;
	}
	
	public static QMetaMethod fromReflectedMethod(java.lang.reflect.Method method) {
		Class<?> declaringClass = method.getDeclaringClass();
		QMetaMethod qmethod = null;
		QMetaObject mo = QMetaObject.forType(declaringClass);
		if(mo!=null) {
			Object[] ok = {method};
			qmethod = methodFromMethod(mo, mo.metaObjectPointer, ok);
			if(qmethod==null && ok[0]==null) {
				qmethod = mo.method(method.getName(), method.getParameterTypes());
			}
		}
		return qmethod;
	}
	
	private static native QMetaMethod methodFromMethod(QMetaObject mo, long metaObjectPointer, Object[] method);
	
	private static QMetaMethod fromMethodImpl(Serializable method) {
		QtJambiInternal.LambdaInfo info = QtJambiInternal.lamdaInfo(method);
		if(info!=null && info.reflectiveMethod!=null && info.lambdaArgs.isEmpty()) {
			return fromReflectedMethod(info.reflectiveMethod);
		}
		return null;
	}
	
	public static <R> QMetaMethod fromMethod(Method0<R> method) {
		return fromMethodImpl(method);
	}
	
	public static <A,R> QMetaMethod fromMethod(Method1<A,R> method) {
		return fromMethodImpl(method);
	}
	
	public static <A,B,R> QMetaMethod fromMethod(Method2<A,B,R> method) {
		return fromMethodImpl(method);
	}
	
	public static <A,B,C,R> QMetaMethod fromMethod(Method3<A,B,C,R> method) {
		return fromMethodImpl(method);
	}
	
	public static <A,B,C,D,R> QMetaMethod fromMethod(Method4<A,B,C,D,R> method) {
		return fromMethodImpl(method);
	}
	
	public static <A,B,C,D,E,R> QMetaMethod fromMethod(Method5<A,B,C,D,E,R> method) {
		return fromMethodImpl(method);
	}
	
	public static <A,B,C,D,E,F,R> QMetaMethod fromMethod(Method6<A,B,C,D,E,F,R> method) {
		return fromMethodImpl(method);
	}
	
	public static <A,B,C,D,E,F,G,R> QMetaMethod fromMethod(Method7<A,B,C,D,E,F,G,R> method) {
		return fromMethodImpl(method);
	}
	
	public static <A,B,C,D,E,F,G,H,R> QMetaMethod fromMethod(Method8<A,B,C,D,E,F,G,H,R> method) {
		return fromMethodImpl(method);
	}
	
	public static <A,B,C,D,E,F,G,H,I,R> QMetaMethod fromMethod(Method9<A,B,C,D,E,F,G,H,I,R> method) {
		return fromMethodImpl(method);
	}
	
	public static QMetaMethod fromMethod(Slot0 method) {
		return fromMethodImpl(method);
	}
	
	public static <A> QMetaMethod fromMethod(Slot1<A> method) {
		return fromMethodImpl(method);
	}
	
	public static <A,B> QMetaMethod fromMethod(Slot2<A,B> method) {
		return fromMethodImpl(method);
	}
	
	public static <A,B,C> QMetaMethod fromMethod(Slot3<A,B,C> method) {
		return fromMethodImpl(method);
	}
	
	public static <A,B,C,D> QMetaMethod fromMethod(Slot4<A,B,C,D> method) {
		return fromMethodImpl(method);
	}
	
	public static <A,B,C,D,E> QMetaMethod fromMethod(Slot5<A,B,C,D,E> method) {
		return fromMethodImpl(method);
	}
	
	public static <A,B,C,D,E,F> QMetaMethod fromMethod(Slot6<A,B,C,D,E,F> method) {
		return fromMethodImpl(method);
	}
	
	public static <A,B,C,D,E,F,G> QMetaMethod fromMethod(Slot7<A,B,C,D,E,F,G> method) {
		return fromMethodImpl(method);
	}
	
	public static <A,B,C,D,E,F,G,H> QMetaMethod fromMethod(Slot8<A,B,C,D,E,F,G,H> method) {
		return fromMethodImpl(method);
	}
	
	public static <A,B,C,D,E,F,G,H,I> QMetaMethod fromMethod(Slot9<A,B,C,D,E,F,G,H,I> method) {
		return fromMethodImpl(method);
	}
}
