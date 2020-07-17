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

import static io.qt.core.QMetaMethod.fromReflectedMethod;

import java.io.Serializable;
import java.lang.reflect.InvocationHandler;
import java.lang.reflect.Method;
import java.lang.reflect.Modifier;
import java.lang.reflect.ParameterizedType;
import java.lang.reflect.Proxy;
import java.lang.reflect.Type;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;
import java.util.concurrent.atomic.AtomicReference;
import java.util.function.Supplier;
import java.util.logging.Level;
import java.util.logging.Logger;

import io.qt.QFlags;
import io.qt.QNoDefaultValueException;
import io.qt.QNoSuchMethodException;
import io.qt.QNoSuchSlotException;
import io.qt.QUnsuccessfulInvocationException;
import io.qt.QtAbstractEnumerator;
import io.qt.QtEnumerator;
import io.qt.QtGadget;
import io.qt.QtObjectInterface;
import io.qt.QtSignalEmitterInterface;
import io.qt.QtThreadAffineInterface;
import io.qt.QtUtilities;
import io.qt.internal.QtJambiInternal;
import io.qt.internal.QtJambiSignals;

public final class QMetaObject {
	
	@Override
	public String toString() {
		return metaObjectPointer==0 ? super.toString() : "QMetaObject(" + className() + ")";
	}
	
	private static class EnumEntries{
		EnumEntries(QtEnumerator[] entries) {
			super();
			this.entries = entries;
		}

		final QtEnumerator[] entries;
		private Map<String,QtAbstractEnumerator> enumConstantDirectory;
		private Map<Integer,QtAbstractEnumerator> enumeratorConstantDirectory;

		Map<String,QtAbstractEnumerator> enumConstantDirectory() {
			if(enumConstantDirectory==null) {
				enumConstantDirectory = new TreeMap<>();
				for(QtEnumerator entry : entries) {
					enumConstantDirectory.put(entry.name(), entry);
				}
			}
			return enumConstantDirectory;
		}
		
		Map<Integer,QtAbstractEnumerator> enumeratorConstantDirectory() {
			if(enumConstantDirectory==null) {
				enumeratorConstantDirectory = new TreeMap<>();
				for(QtEnumerator entry : entries) {
					if(!enumeratorConstantDirectory.containsKey(entry.value()))
						enumeratorConstantDirectory.put(entry.value(), entry);
				}
			}
			return enumeratorConstantDirectory;
		}
	}
	
	private static final Class<?> flagsWrapperClass;
	static {
		Class<?> flagsClass = null;
		for(Class<?> cls : QFlags.class.getDeclaredClasses()) {
    		if(QFlags.class.isAssignableFrom(cls) && !Modifier.isAbstract(cls.getModifiers())) {
    			flagsClass = cls;
    		}
    	}
		flagsWrapperClass = flagsClass;
	}

	final long metaObjectPointer;
	private Class<?>[] enumClasses;
	private Map<Integer, EnumEntries> enumEntries;
	
	private QMetaObject(long metaObjectPointer) {
		this.metaObjectPointer = metaObjectPointer;
	}
	
	synchronized Class<?> enumType(QMetaEnum enumerator){
		if(enumClasses==null)
			enumClasses = new Class[enumeratorCount(metaObjectPointer)];
		if(enumClasses[enumerator.enumeratorIndex]==null) {
			boolean isFlag = enumerator.isFlag();
	        String name = enumerator.name();
	        Class<?> foundClass = null;
	        Class<?> enumeratorClass = null;
	        Class<?> flagsClass = null;
	        Class<?> exactDeclaringType = exactType(metaObjectPointer);
	        if(exactDeclaringType!=null) {
		        for(Class<?> cls : exactDeclaringType.getDeclaredClasses()) {
		        	if(QtAbstractEnumerator.class.isAssignableFrom(cls) 
	        				|| QFlags.class.isAssignableFrom(cls)) {
		        		if(cls.getSimpleName().equals(name)) {
		        			foundClass = cls;
		        			break;
		            	}
		        	}
		        }
		        if(foundClass!=null) {
			        if(isFlag) {
			        	if(QtAbstractEnumerator.class.isAssignableFrom(foundClass)) {
			        		enumeratorClass = foundClass;
			        		flagsClass = getFlagFromEnum(exactDeclaringType, foundClass);
			        	}else{
			        		flagsClass = foundClass;
			        		enumeratorClass = getEnumFromFlag(foundClass);
			        	}
			        }else {
			        	if(QtAbstractEnumerator.class.isAssignableFrom(foundClass)) {
			        		flagsClass = getFlagFromEnum(exactDeclaringType, foundClass);
			        		enumeratorClass = foundClass;
			        	}else{
			        		flagsClass = foundClass;		        		
			        		enumeratorClass = getEnumFromFlag(foundClass);
			        	}
			        }
		        }
	        }
	        if(isFlag && flagsClass==null){
	        	if(enumeratorClass==null) {
	        		QMetaEnum enmType = findEnumForFlags(metaObjectPointer, enumerator.enumeratorIndex);
	        		if(enmType!=null) {
	        			enumeratorClass = enmType.type();
	        		}
	        	}
	        	if(enumeratorClass==null) {
	        		enumeratorClass = QtEnumerator.class;
	        	}
    			flagsClass = flagsWrapperClass;
	        }else if(!isFlag && enumeratorClass==null){
	        	enumeratorClass = QtEnumerator.class;
	        	generateEnumEntries(enumerator);
	        }
	        if(isFlag) {
	        	generateEnumEntries(enumerator);
	        }
	        enumClasses[enumerator.enumeratorIndex] = isFlag ? flagsClass : enumeratorClass;
		}
		return enumClasses[enumerator.enumeratorIndex];
	}
	
	private void generateEnumEntries(QMetaEnum enumerator) {
    	if(enumEntries==null)
    		enumEntries = new HashMap<>();
    	ClassLoader cl = type().getClassLoader();
    	Class<?>[] ifc = new Class[] {QtEnumerator.class};
    	QtEnumerator[] entries = new QtEnumerator[enumerator.keyCount()];
    	for (int i = 0; i < entries.length; i++) {
    		entries[i] = (QtEnumerator)Proxy.newProxyInstance(cl, ifc, new Enumerator(i, enumerator.key(i), enumerator.value(i)));
		}
    	enumEntries.put(enumerator.enumeratorIndex, new EnumEntries(entries));
	}
	
	QtAbstractEnumerator[] enumEntries(QMetaEnum enumerator) {
		QtAbstractEnumerator[] entries;
		Class<?> type = enumerator.type();
		if(type.isEnum()) {
			entries = (QtAbstractEnumerator[])type.getEnumConstants();
		}else if(enumEntries!=null){
			EnumEntries ee = enumEntries.get(enumerator.enumeratorIndex);
			if(ee==null) {
				entries = new QtAbstractEnumerator[0];
			}else{
				entries = Arrays.copyOf(ee.entries, ee.entries.length);
			}
		}else {
			entries = new QtAbstractEnumerator[0];
		}
		return entries;
	}
	
	QtAbstractEnumerator enumEntry(QMetaEnum enumerator, int index) {
		Class<?> type = enumerator.type();
		if(type.isEnum()) {
			return (QtAbstractEnumerator)type.getEnumConstants()[index];
		}else if(enumEntries!=null){
			EnumEntries ee = enumEntries.get(enumerator.enumeratorIndex);
			if(ee==null) {
				return null;
			}else{
				return ee.entries[index];
			}
		}else {
			return null;
		}
	}
	
	@SuppressWarnings({ "unchecked", "rawtypes" })
	QtAbstractEnumerator enumEntry(QMetaEnum enumerator, String name) {
		Class<?> type = enumerator.type();
		if(type.isEnum()) {
			return (QtAbstractEnumerator)Enum.valueOf((Class<Enum>)type, name);
		}else if(enumEntries!=null){
			EnumEntries ee = enumEntries.get(enumerator.enumeratorIndex);
			if(ee==null) {
				return null;
			}else{
				return ee.enumConstantDirectory().get(name);
			}
		}else {
			return null;
		}
	}
	
	private static class Enumerator implements InvocationHandler{
		private Enumerator(int ordinal, String name, int value) {
			super();
			this.ordinal = ordinal;
			this.name = name;
			this.value = value;
		}
    	
		private final int ordinal;
    	private final String name;
    	private final int value;
    	
		@Override
		public Object invoke(Object proxy, Method method, Object[] args) {
			switch(method.getName()) {
			case "ordinal": return ordinal;
			case "value": return value;
			case "toString":
			case "name": return name;
			case "getDeclaringClass": return QtEnumerator.class;
			}
			return null;
		}
    }
    
    private static native QMetaEnum findEnumForFlags(long metaObjectPointer, int enumeratorIndex);
    
	private static Class<?> getFlagFromEnum(Class<?> exactDeclaringType, Class<?> type) {
    	for(Class<?> flagsType : exactDeclaringType.getDeclaredClasses()) {
    		if(QFlags.class.isAssignableFrom(flagsType) 
    				&& flagsType.getGenericSuperclass() instanceof ParameterizedType) {
    			ParameterizedType pt = (ParameterizedType)flagsType.getGenericSuperclass();
    			Type[] typeArguments = pt.getActualTypeArguments();
    			if(typeArguments.length==1 && typeArguments[0] instanceof Class) {
    				if(typeArguments[0]==type) {
    					return flagsType;
    				}
    			}
    		}
		}
    	return null;
    }
    
    private static Class<?> getEnumFromFlag(Class<?> type) {
    	if(type.getGenericSuperclass() instanceof ParameterizedType) {
			ParameterizedType pt = (ParameterizedType)type.getGenericSuperclass();
			Type[] typeArguments = pt.getActualTypeArguments();
			if(typeArguments.length==1 && typeArguments[0] instanceof Class) {
				if(typeArguments[0]==type) {
					return (Class<?>)typeArguments[0];
				}
			}
		}
    	return null;
    }
	
	public static interface Connection{
		public boolean isConnected();
		public QtSignalEmitterInterface sender();
		public Object receiver();
	}
	
	public enum Call implements QtEnumerator{
        InvokeMetaMethod(0),
        ReadProperty(1),
        WriteProperty(2),
        ResetProperty(3),
        QueryPropertyDesignable(4),
        QueryPropertyScriptable(5),
        QueryPropertyStored(6),
        QueryPropertyEditable(7),
        QueryPropertyUser(8),
        CreateInstance(9),
        IndexOfMethod(10),
        RegisterPropertyMetaType(11),
        RegisterMethodArgumentMetaType(12);
		
		private Call(int value) { this.value = value; }
		
		private final int value;
		
		public static Call resolve(int value) {
            switch (value) {
            case 0: return InvokeMetaMethod;
            case 1: return ReadProperty;
            case 2: return WriteProperty;
            case 3: return ResetProperty;
            case 4: return QueryPropertyDesignable;
            case 5: return QueryPropertyScriptable;
            case 6: return QueryPropertyStored;
            case 7: return QueryPropertyEditable;
            case 8: return QueryPropertyUser;
            case 9: return CreateInstance;
            case 10: return IndexOfMethod;
            case 11: return RegisterPropertyMetaType;
            case 12: return RegisterMethodArgumentMetaType;
            default: throw new io.qt.QNoSuchEnumValueException(value);
            }
        }

		@Override
		public int value() {
			return value;
		}
    }
	
	public QMetaProperty userProperty() {
		return userProperty(this.metaObjectPointer);
	}
	
	public java.util.List<io.qt.core.QMetaProperty> properties() {
		return Collections.unmodifiableList(properties(this.metaObjectPointer));
	}
	
	public QMetaProperty property(String name) {
		return property(this.metaObjectPointer, name);
	}
	
	public java.util.List<io.qt.core.QMetaEnum> enumerators() {
		return Collections.unmodifiableList(enumerators(this.metaObjectPointer));
	}
	
	public io.qt.core.QMetaEnum enumerator(String name) {
		return enumerator(this.metaObjectPointer, name);
	}
	
    public static void connectSlotsByName(QObject object) {
        io.qt.internal.QtJambiInternal.connectSlotsByName(object);
    }
	
	private native QMetaProperty userProperty(long metaObjectPointer);
	
	private native List<QMetaProperty> properties(long metaObjectPointer);

	private native QMetaProperty property(long metaObjectPointer, String name);
	
	private static native List<QMetaEnum> enumerators(long metaObjectPointer);
	
	private static native int enumeratorCount(long metaObjectPointer);
	
	private static native QMetaEnum enumerator(long metaObjectPointer, String name);
	
	static native QMetaObject.Connection connect(long sender__id, String signal, long receiver__id, String slot, byte connection);
	
	static native QMetaObject.Connection connectMethods(long sender__id, int signalIdx, long signalEnclosingMetaObject, long receiver__id, int slotIdx, long EnclosingMetaObject, byte connection);
	
	static native boolean disconnect(long sender__id, String signal, long receiver__id, String slot);
	
	static native boolean disconnectMethods(long sender__id, int signalIdx, long signalEnclosingMetaObject, long receiver__id, int slotIdx, long EnclosingMetaObject);
	
	private native QMetaMethod methodByIndex(long metaObjectPointer, int index);
	
	public QMetaMethod method(String name, Class<?>... parameterTypes) {
		QMetaMethod method;
		if(parameterTypes.length==0 && name.contains("(")) {
			method = method(metaObjectPointer, io.qt.internal.MetaObjectTools.cppNormalizedSignature(name));
			if(method==null)
				method = method(metaObjectPointer, io.qt.internal.MetaObjectTools.cppNormalizedSignature(name)+"const");
		}else {
			StringBuilder args = new StringBuilder();
			for(int i=0; i<parameterTypes.length; ++i) {
				if(i!=0)
					args.append(',');
				String typeName = io.qt.internal.MetaObjectTools.internalNameOfArgumentType(parameterTypes[i]);
				if(typeName.isEmpty())
					args.append("QVariant");
				else
					args.append(typeName);
			}
			method = method(metaObjectPointer, String.format("%1$s(%2$s)", name, args));
		}
		if(method==null) {
			for(QMetaMethod m : methods(metaObjectPointer)) {
				if(m.name().equals(name) && m.parameterTypes().equals(Arrays.asList(parameterTypes))) {
					method = m;
					break;
				}
			}
		}
		return method;
	}
	
	private native QMetaMethod method(long metaObjectPointer, String normalizedSignature);
	
	public List<QMetaMethod> methods(){
		return Collections.unmodifiableList(methods(metaObjectPointer));
	}
	
	private native List<QMetaMethod> methods(long metaObjectPointer);
	
	public QMetaMethod constructor(Class<?>... parameterTypes) {
		StringBuilder args = new StringBuilder();
		for(int i=0; i<parameterTypes.length; ++i) {
			if(i!=0)
				args.append(',');
			String typeName = io.qt.internal.MetaObjectTools.internalNameOfArgumentType(parameterTypes[i]);
			if(typeName.isEmpty())
				args.append("QVariant");
			else
				args.append(typeName);
		}
		QMetaMethod method = constructor(metaObjectPointer, String.format("%1$s(%2$s)", className(), args));
		if(method==null) {
			for(QMetaMethod m : constructors(metaObjectPointer)) {
				if(m.parameterTypes().equals(Arrays.asList(parameterTypes))) {
					method = m;
					break;
				}
			}
		}
		return method;
	}
	
	private native QMetaMethod constructor(long metaObjectPointer, String normalizedSignature);
	
	public List<QMetaMethod> constructors(){
		return Collections.unmodifiableList(constructors(metaObjectPointer));
	}
	
	private native List<QMetaMethod> constructors(long metaObjectPointer);
	
	public QObject newInstance(Object... args) throws NoSuchMethodException {
		if (!inherits(QObject.staticMetaObject)) {
	        throw new RuntimeException(String.format("Type %1$s does not inherit QObject", className()));
	    }
		List<QMetaMethod> constructors = new ArrayList<>(constructors());
		if(constructors.isEmpty()) {
			throw new UnsupportedOperationException("No constructors available.");
		}
		QMetaMethod constr = null;
		for(QMetaMethod constructor : new ArrayList<>(constructors)) {
			List<Class<?>> parameterTypes = constructor.parameterTypes();
			if(parameterTypes.size()!=args.length) {
				constructors.remove(constructor);
			}else{
				boolean matches = true;
				for(int i=0; i<args.length; ++i) {
					Class<?> parameterType = parameterTypes.get(i);
					if(parameterType.isPrimitive()) {
						if(args[i]==null) {
							matches = false;
							break;
						}else{
							if(parameterType==long.class 
									&& !(args[i] instanceof Long
											|| args[i] instanceof Integer
											|| args[i] instanceof Short
											|| args[i] instanceof Byte)) {
								matches = false;
								break;
							}else if(parameterType==int.class 
									&& !(args[i] instanceof Integer
											|| args[i] instanceof Short
											|| args[i] instanceof Byte)) {
								matches = false;
								break;
							}else if(parameterType==short.class 
									&& !(args[i] instanceof Short
											|| args[i] instanceof Byte)) {
								matches = false;
								break;
							}else if(parameterType==byte.class 
									&& !(args[i] instanceof Byte)) {
								matches = false;
								break;
							}else if(parameterType==double.class 
									&& !(args[i] instanceof Double
											|| args[i] instanceof Float
											|| args[i] instanceof Long
											|| args[i] instanceof Integer
											|| args[i] instanceof Short
											|| args[i] instanceof Byte)) {
								matches = false;
								break;
							}else if(parameterType==float.class 
									&& !(args[i] instanceof Float
											|| args[i] instanceof Integer
											|| args[i] instanceof Short
											|| args[i] instanceof Byte)) {
								matches = false;
								break;
							}else if(parameterType==boolean.class 
									&& !(args[i] instanceof Boolean)) {
								matches = false;
								break;
							}else if(parameterType==char.class 
									&& !(args[i] instanceof Character)) {
								matches = false;
								break;
							}else if(parameterType==void.class) {
								matches = false;
								break;
							}
						}
					}else if(args[i]!=null && !parameterType.isInstance(args[i])) {
						matches = false;
						break;
					}
				}
				if(!matches) {
					constructors.remove(constructor);
				}
			}
		}
		if(!constructors.isEmpty()) {
			constr = constructors.get(0);
		}
		if(constr==null) {
			StringBuilder argsStrg = new StringBuilder();
			for(int i=0; i<args.length; ++i) {
				if(i!=0)
					argsStrg.append(',');
				argsStrg.append(args[i]==null ? Object.class.getName() : args[i].getClass().getName());
			}
			throw new NoSuchMethodException(String.format("Constructor %1$s(%2$s) not available.", this.className(), argsStrg));
		}
		String[] argTypes = new String[args.length];
		List<Class<?>> parameterTypes = constr.parameterTypes();
		for(int i=0; i<args.length; ++i) {
			if(args[i]==null) {
				argTypes[i] = parameterTypes.get(i).getName();
			}else {
				String className = args[i].getClass().getName();
				if(className.contains("$Lambda$")) {
					if(args[i].getClass().getInterfaces().length>1) {
						className = args[i].getClass().getInterfaces()[0].getName();
					}else {
						className = args[i].getClass().getSuperclass().getName();
					}
				}
				argTypes[i] = className;
			}
		}
		return newInstanceImpl(metaObjectPointer, constr.methodIndex(), argTypes, args);
	}
	
	private static native QObject newInstanceImpl(long metaObjectPointer, int methodIndex, String[] argTypes, Object[] args);
	
	public QMetaObject superClass(){
		return superClass(metaObjectPointer);
	}
	
	public String className(){
		return className(metaObjectPointer);
	}
	
	public List<QPair<String,String>> classInfos(){
		return Collections.unmodifiableList(classInfos(metaObjectPointer));
	}
	
	public String classInfo(String name){
		return classInfo(metaObjectPointer, name);
	}
	
	public boolean inherits(QMetaObject metaObject){
		return inherits(metaObjectPointer, metaObject.metaObjectPointer);
	}
	
	public static <T extends QtObjectInterface> T cast(Class<T> targetType, QtObjectInterface object) {
		io.qt.QtUtilities.initializePackage(targetType);
		if(targetType.isInstance(object)) {
			return targetType.cast(object);
		}
		long nativeId = QtJambiInternal.checkedNativeId(object);
		return targetType.cast(cast(nativeId, targetType));
	}
	
	private static native Object cast(long nativeId, Class<?> targetType);
	
	public Class<?> type(){
		return type(metaObjectPointer);
	}
	
	static native QMetaObject forQObject(long metaObjectPointer);
	
	public static QMetaObject forType(Class<?> clazz) {
		String className = clazz.getName();
		int idx = className.lastIndexOf('.');
		if(idx>0)
			QtUtilities.initializePackage(className.substring(0, idx));
		return __qt_forType(clazz);
	}
	
	private static native QMetaObject __qt_forType(Class<?> clazz);
	
	private static native Class<?> type(long metaObjectPointer);
	
	private static native Class<?> exactType(long metaObjectPointer);
	
	private static native QMetaObject superClass(long metaObjectPointer);
	
	private static native boolean inherits(long metaObjectPointer, long other);
	
	private static native String className(long metaObjectPointer);
	
	private static native String classInfo(long metaObjectPointer, String name);
	
	private static native List<QPair<String,String>> classInfos(long metaObjectPointer);
	
	public static boolean checkConnectArgs(String signal, String method) {
		return checkConnectArgsString(io.qt.internal.QtJambiInternal.SignalPrefix+io.qt.internal.MetaObjectTools.cppNormalizedSignature(signal), io.qt.internal.QtJambiInternal.SlotPrefix+io.qt.internal.MetaObjectTools.cppNormalizedSignature(method));
	}
	
	private native static boolean checkConnectArgsString(String signal, String method);
	
	public static boolean checkConnectArgs(QMetaMethod signal, QMetaMethod method){
		return checkConnectArgsMethods(signal.enclosingMetaObject().metaObjectPointer, signal.methodIndex(), method.enclosingMetaObject().metaObjectPointer, method.methodIndex());
	}
	
	private static native boolean checkConnectArgsMethods(long signalMetaObjectPointer, int signalMethodIndex, long methodMetaObjectPointer, int methodMethodIndex);
	
	public static Object invokeMethod(QObject obj, String member, Object... args) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
		return invokeMethod(obj, member, Qt.ConnectionType.AutoConnection, args);
	}
	
	public static Object invokeMethod(QObject obj, String member, Qt.ConnectionType type, Object... args) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
		Class<?>[] parameterTypes;
		if(member.contains("(")) {
			parameterTypes = new Class<?>[0];
		}else {
			parameterTypes = new Class<?>[args.length];
			for (int i = 0; i < parameterTypes.length; i++) {
				if(args[i]==null) {
					parameterTypes[i] = Object.class;
				}else {
					String className = args[i].getClass().getName();
					if(className.contains("$Lambda$")) {
						if(args[i].getClass().getInterfaces().length>1) {
							parameterTypes[i] = args[i].getClass().getInterfaces()[0];
						}else {
							parameterTypes[i] = args[i].getClass().getSuperclass();
						}
					}else {
						parameterTypes[i] = args[i].getClass();
					}
				}
			}
		}
		QMetaMethod method = obj.metaObject().method(member, parameterTypes);
		if(method==null) {
			throw new QNoSuchMethodException(member);
		}
		return method.invoke(obj, type, args);
	}
	
	public static <R> R invokeMethod(Method0<R> method) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
		return invokeMethod(method, Qt.ConnectionType.AutoConnection);
	}
	
	@SuppressWarnings("unchecked")
	public static <R> R invokeMethod(Method0<R> method, Qt.ConnectionType type) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
		QtJambiInternal.LambdaInfo info = QtJambiInternal.lamdaInfo(method);
		if(info!=null && info.qobject!=null && !info.qobject.isDisposed() && info.reflectiveMethod!=null && (info.reflectiveMethod!=null || info.signal!=null)) {
			QMetaMethod qmethod = info.reflectiveMethod!=null ? fromReflectedMethod(info.reflectiveMethod) : info.qobject.metaObject().methodByIndex(info.qobject.metaObject().metaObjectPointer, info.signal.methodIndex());
			if(qmethod!=null && qmethod.parameterTypes().size()==info.lambdaArgs.size()) {
				if(info.lambdaArgs.isEmpty()) {
					return (R)qmethod.invoke(info.qobject, type);
				}else {
					Object[] args = info.lambdaArgs.toArray();
					return (R)qmethod.invoke(info.qobject, type, args);
				}
			}
		}
		if(method instanceof QtObjectInterface) {
			if( ((QtObjectInterface)method).isDisposed() )
				return null;
		}
		switch(type) {
		case AutoConnection:
			if(method instanceof QtThreadAffineInterface) {
				QThread thread = ((QtThreadAffineInterface) method).thread();
				if(thread!=null && thread!=QThread.currentThread()) {
					throw new QUnsuccessfulInvocationException("Unable to invoke methods with return values in queued connections (auto connection with different threads).");
				}
			}
		case DirectConnection:
			try {
				return method.invoke();
			} catch (Throwable e) {
				throw new QUnsuccessfulInvocationException(e);
			}
		case BlockingQueuedConnection:
			if(method instanceof QtThreadAffineInterface) {
				QThread thread = ((QtThreadAffineInterface) method).thread();
				if(thread!=null && thread!=QThread.currentThread()) {
					AtomicReference<R> result = new AtomicReference<>();
					QObject invoker = new QObject() {
						@Override
						public boolean event(QEvent event) {
							if(event.type()==QEvent.Type.DeferredDispose
									&& (info==null || info.qobject==null || !info.qobject.isDisposed()) 
									&& !(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
								try {
									result.set(method.invoke());
								} catch (Throwable e) {
									Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invokation.", e);
								}
								synchronized (this) {
									notifyAll();
								}
							}
							return super.event(event);
						}
					};
					invoker.moveToThread(thread);
					invoker.disposeLater();
					synchronized (invoker) {
						try {
							invoker.wait();
						} catch (InterruptedException e) {
							throw new QUnsuccessfulInvocationException(e);
						}
					}
					return result.get();
				}
			}
			throw new QUnsuccessfulInvocationException("Blocking-queued invocation of method on not allowed without thread affinity.");
		case QueuedConnection:
			throw new QUnsuccessfulInvocationException("Unable to invoke methods with return values in queued connections.");
		default:
			break;
		}
		throw new QUnsuccessfulInvocationException("Unable to invoke method.");
	}
	
	public static <A,R> R invokeMethod(Method1<A,R> method, A arg1) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
		return invokeMethod(method, Qt.ConnectionType.AutoConnection, arg1);
	}
	
	@SuppressWarnings("unchecked")
	public static <A,R> R invokeMethod(Method1<A,R> method, Qt.ConnectionType type, A arg1) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
		QtJambiInternal.LambdaInfo info = QtJambiInternal.lamdaInfo(method);
		if(info!=null && info.qobject!=null && !info.qobject.isDisposed() && info.reflectiveMethod!=null && (info.reflectiveMethod!=null || info.signal!=null)) {
			QMetaMethod qmethod = info.reflectiveMethod!=null ? fromReflectedMethod(info.reflectiveMethod) : info.qobject.metaObject().methodByIndex(info.qobject.metaObject().metaObjectPointer, info.signal.methodIndex());
			if(qmethod!=null && qmethod.parameterTypes().size()==info.lambdaArgs.size()+1) {
				if(info.lambdaArgs.isEmpty()) {
					return (R)qmethod.invoke(info.qobject, type, arg1);
				}else {
					Object[] args = new Object[qmethod.parameterTypes().size()];
					args[0] = arg1;
					System.arraycopy(info.lambdaArgs.toArray(), 0, args, 1, info.lambdaArgs.size());
					return (R)qmethod.invoke(info.qobject, type, args);
				}
			}
		}
		if(method instanceof QtObjectInterface) {
			if( ((QtObjectInterface)method).isDisposed() )
				return null;
		}
		switch(type) {
		case AutoConnection:
			if(method instanceof QtThreadAffineInterface) {
				QThread thread = ((QtThreadAffineInterface) method).thread();
				if(thread!=null && thread!=QThread.currentThread()) {
					throw new QUnsuccessfulInvocationException("Unable to invoke methods with return values in queued connections (auto connection with different threads).");
				}
			}
		case DirectConnection:
			try {
				return method.invoke(arg1);
			} catch (Throwable e) {
				throw new QUnsuccessfulInvocationException(e);
			}
		case BlockingQueuedConnection:
			if(method instanceof QtThreadAffineInterface) {
				QThread thread = ((QtThreadAffineInterface) method).thread();
				if(thread!=null && thread!=QThread.currentThread()) {
					AtomicReference<R> result = new AtomicReference<>();
					QObject invoker = new QObject() {
						@Override
						public boolean event(QEvent event) {
							if(event.type()==QEvent.Type.DeferredDispose
									&& (info==null || info.qobject==null || !info.qobject.isDisposed()) 
									&& !(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
								try {
									result.set(method.invoke(arg1));
								} catch (Throwable e) {
									Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invokation.", e);
								}
								synchronized (this) {
									notifyAll();
								}
							}
							return super.event(event);
						}
					};
					invoker.moveToThread(thread);
					invoker.disposeLater();
					synchronized (invoker) {
						try {
							invoker.wait();
						} catch (InterruptedException e) {
							throw new QUnsuccessfulInvocationException(e);
						}
					}
					return result.get();
				}
			}
			throw new QUnsuccessfulInvocationException("Blocking-queued invocation of method on not allowed without thread affinity.");
		case QueuedConnection:
			throw new QUnsuccessfulInvocationException("Unable to invoke methods with return values in queued connections.");
		default:
			break;
		}
		throw new QUnsuccessfulInvocationException("Unable to invoke method.");
	}
	
	public static <A,B,R> R invokeMethod(Method2<A,B,R> method, A arg1, B arg2) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
		return invokeMethod(method, Qt.ConnectionType.AutoConnection, arg1, arg2);
	}
	
	@SuppressWarnings("unchecked")
	public static <A,B,R> R invokeMethod(Method2<A,B,R> method, Qt.ConnectionType type, A arg1, B arg2) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
		QtJambiInternal.LambdaInfo info = QtJambiInternal.lamdaInfo(method);
		if(info!=null && info.qobject!=null && !info.qobject.isDisposed() && info.reflectiveMethod!=null && (info.reflectiveMethod!=null || info.signal!=null)) {
			QMetaMethod qmethod = info.reflectiveMethod!=null ? fromReflectedMethod(info.reflectiveMethod) : info.qobject.metaObject().methodByIndex(info.qobject.metaObject().metaObjectPointer, info.signal.methodIndex());
			if(qmethod!=null && qmethod.parameterTypes().size()==info.lambdaArgs.size()+2) {
				if(info.lambdaArgs.isEmpty()) {
					return (R)qmethod.invoke(info.qobject, type, arg1, arg2);
				}else {
					Object[] args = new Object[qmethod.parameterTypes().size()];
					args[0] = arg1;
					args[1] = arg2;
					System.arraycopy(info.lambdaArgs.toArray(), 0, args, 2, info.lambdaArgs.size());
					return (R)qmethod.invoke(info.qobject, type, args);
				}
			}
		}
		if(method instanceof QtObjectInterface) {
			if( ((QtObjectInterface)method).isDisposed() )
				return null;
		}
		switch(type) {
		case AutoConnection:
			if(method instanceof QtThreadAffineInterface) {
				QThread thread = ((QtThreadAffineInterface) method).thread();
				if(thread!=null && thread!=QThread.currentThread()) {
					throw new QUnsuccessfulInvocationException("Unable to invoke methods with return values in queued connections (auto connection with different threads).");
				}
			}
		case DirectConnection:
			try {
				return method.invoke(arg1, arg2);
			} catch (Throwable e) {
				throw new QUnsuccessfulInvocationException(e);
			}
		case BlockingQueuedConnection:
			if(method instanceof QtThreadAffineInterface) {
				QThread thread = ((QtThreadAffineInterface) method).thread();
				if(thread!=null && thread!=QThread.currentThread()) {
					AtomicReference<R> result = new AtomicReference<>();
					QObject invoker = new QObject() {
						@Override
						public boolean event(QEvent event) {
							if(event.type()==QEvent.Type.DeferredDispose
									&& (info==null || info.qobject==null || !info.qobject.isDisposed()) 
									&& !(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
								try {
									result.set(method.invoke(arg1, arg2));
								} catch (Throwable e) {
									Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invokation.", e);
								}
								synchronized (this) {
									notifyAll();
								}
							}
							return super.event(event);
						}
					};
					invoker.moveToThread(thread);
					invoker.disposeLater();
					synchronized (invoker) {
						try {
							invoker.wait();
						} catch (InterruptedException e) {
							throw new QUnsuccessfulInvocationException(e);
						}
					}
					return result.get();
				}
			}
			throw new QUnsuccessfulInvocationException("Blocking-queued invocation of method on not allowed without thread affinity.");
		case QueuedConnection:
			throw new QUnsuccessfulInvocationException("Unable to invoke methods with return values in queued connections.");
		default:
			break;
		}
		throw new QUnsuccessfulInvocationException("Unable to invoke method.");
	}
	
	public static <A,B,C,R> R invokeMethod(Method3<A,B,C,R> method, A arg1, B arg2, C arg3) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
		return invokeMethod(method, Qt.ConnectionType.AutoConnection, arg1, arg2, arg3);
	}
	
	@SuppressWarnings("unchecked")
	public static <A,B,C,R> R invokeMethod(Method3<A,B,C,R> method, Qt.ConnectionType type, A arg1, B arg2, C arg3) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
		QtJambiInternal.LambdaInfo info = QtJambiInternal.lamdaInfo(method);
		if(info!=null && info.qobject!=null && !info.qobject.isDisposed() && info.reflectiveMethod!=null && (info.reflectiveMethod!=null || info.signal!=null)) {
			QMetaMethod qmethod = info.reflectiveMethod!=null ? fromReflectedMethod(info.reflectiveMethod) : info.qobject.metaObject().methodByIndex(info.qobject.metaObject().metaObjectPointer, info.signal.methodIndex());
			if(qmethod!=null && qmethod.parameterTypes().size()==info.lambdaArgs.size()+3) {
				if(info.lambdaArgs.isEmpty()) {
					return (R)qmethod.invoke(info.qobject, type, arg1, arg2, arg3);
				}else {
					Object[] args = new Object[qmethod.parameterTypes().size()];
					args[0] = arg1;
					args[1] = arg2;
					args[2] = arg3;
					System.arraycopy(info.lambdaArgs.toArray(), 0, args, 3, info.lambdaArgs.size());
					return (R)qmethod.invoke(info.qobject, type, args);
				}
			}
		}
		if(method instanceof QtObjectInterface) {
			if( ((QtObjectInterface)method).isDisposed() )
				return null;
		}
		switch(type) {
		case AutoConnection:
			if(method instanceof QtThreadAffineInterface) {
				QThread thread = ((QtThreadAffineInterface) method).thread();
				if(thread!=null && thread!=QThread.currentThread()) {
					throw new QUnsuccessfulInvocationException("Unable to invoke methods with return values in queued connections (auto connection with different threads).");
				}
			}
		case DirectConnection:
			try {
				return method.invoke(arg1, arg2, arg3);
			} catch (Throwable e) {
				throw new QUnsuccessfulInvocationException(e);
			}
		case BlockingQueuedConnection:
			if(method instanceof QtThreadAffineInterface) {
				QThread thread = ((QtThreadAffineInterface) method).thread();
				if(thread!=null && thread!=QThread.currentThread()) {
					AtomicReference<R> result = new AtomicReference<>();
					QObject invoker = new QObject() {
						@Override
						public boolean event(QEvent event) {
							if(event.type()==QEvent.Type.DeferredDispose
									&& (info==null || info.qobject==null || !info.qobject.isDisposed()) 
									&& !(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
								try {
									result.set(method.invoke(arg1, arg2, arg3));
								} catch (Throwable e) {
									Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invokation.", e);
								}
								synchronized (this) {
									notifyAll();
								}
							}
							return super.event(event);
						}
					};
					invoker.moveToThread(thread);
					invoker.disposeLater();
					synchronized (invoker) {
						try {
							invoker.wait();
						} catch (InterruptedException e) {
							throw new QUnsuccessfulInvocationException(e);
						}
					}
					return result.get();
				}
			}
			throw new QUnsuccessfulInvocationException("Blocking-queued invocation of method on not allowed without thread affinity.");
		case QueuedConnection:
			throw new QUnsuccessfulInvocationException("Unable to invoke methods with return values in queued connections.");
		default:
			break;
		}
		throw new QUnsuccessfulInvocationException("Unable to invoke method.");
	}
	
	public static <A,B,C,D,R> R invokeMethod(Method4<A,B,C,D,R> method, A arg1, B arg2, C arg3, D arg4) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
		return invokeMethod(method, Qt.ConnectionType.AutoConnection, arg1, arg2, arg3, arg4);
	}
	
	@SuppressWarnings("unchecked")
	public static <A,B,C,D,R> R invokeMethod(Method4<A,B,C,D,R> method, Qt.ConnectionType type, A arg1, B arg2, C arg3, D arg4) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
		QtJambiInternal.LambdaInfo info = QtJambiInternal.lamdaInfo(method);
		if(info!=null && info.qobject!=null && !info.qobject.isDisposed() && info.reflectiveMethod!=null && (info.reflectiveMethod!=null || info.signal!=null)) {
			QMetaMethod qmethod = info.reflectiveMethod!=null ? fromReflectedMethod(info.reflectiveMethod) : info.qobject.metaObject().methodByIndex(info.qobject.metaObject().metaObjectPointer, info.signal.methodIndex());
			if(qmethod!=null && qmethod.parameterTypes().size()==info.lambdaArgs.size()+4) {
				if(info.lambdaArgs.isEmpty()) {
					return (R)qmethod.invoke(info.qobject, type, arg1, arg2, arg3, arg4);
				}else {
					Object[] args = new Object[qmethod.parameterTypes().size()];
					args[0] = arg1;
					args[1] = arg2;
					args[2] = arg3;
					args[3] = arg4;
					System.arraycopy(info.lambdaArgs.toArray(), 0, args, 4, info.lambdaArgs.size());
					return (R)qmethod.invoke(info.qobject, type, args);
				}
			}
		}
		if(method instanceof QtObjectInterface) {
			if( ((QtObjectInterface)method).isDisposed() )
				return null;
		}
		switch(type) {
		case AutoConnection:
			if(method instanceof QtThreadAffineInterface) {
				QThread thread = ((QtThreadAffineInterface) method).thread();
				if(thread!=null && thread!=QThread.currentThread()) {
					throw new QUnsuccessfulInvocationException("Unable to invoke methods with return values in queued connections (auto connection with different threads).");
				}
			}
		case DirectConnection:
			try {
				return method.invoke(arg1, arg2, arg3, arg4);
			} catch (Throwable e) {
				throw new QUnsuccessfulInvocationException(e);
			}
		case BlockingQueuedConnection:
			if(method instanceof QtThreadAffineInterface) {
				QThread thread = ((QtThreadAffineInterface) method).thread();
				if(thread!=null && thread!=QThread.currentThread()) {
					AtomicReference<R> result = new AtomicReference<>();
					QObject invoker = new QObject() {
						@Override
						public boolean event(QEvent event) {
							if(event.type()==QEvent.Type.DeferredDispose
									&& (info==null || info.qobject==null || !info.qobject.isDisposed()) 
									&& !(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
								try {
									result.set(method.invoke(arg1, arg2, arg3, arg4));
								} catch (Throwable e) {
									Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invokation.", e);
								}
								synchronized (this) {
									notifyAll();
								}
							}
							return super.event(event);
						}
					};
					invoker.moveToThread(thread);
					invoker.disposeLater();
					synchronized (invoker) {
						try {
							invoker.wait();
						} catch (InterruptedException e) {
							throw new QUnsuccessfulInvocationException(e);
						}
					}
					return result.get();
				}
			}
			throw new QUnsuccessfulInvocationException("Blocking-queued invocation of method on not allowed without thread affinity.");
		case QueuedConnection:
			throw new QUnsuccessfulInvocationException("Unable to invoke methods with return values in queued connections.");
		default:
			break;
		}
		throw new QUnsuccessfulInvocationException("Unable to invoke method.");
	}
	
	public static <A,B,C,D,E,R> R invokeMethod(Method5<A,B,C,D,E,R> method, A arg1, B arg2, C arg3, D arg4, E arg5) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
		return invokeMethod(method, Qt.ConnectionType.AutoConnection, arg1, arg2, arg3, arg4, arg5);
	}
	
	@SuppressWarnings("unchecked")
	public static <A,B,C,D,E,R> R invokeMethod(Method5<A,B,C,D,E,R> method, Qt.ConnectionType type, A arg1, B arg2, C arg3, D arg4, E arg5) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
		QtJambiInternal.LambdaInfo info = QtJambiInternal.lamdaInfo(method);
		if(info!=null && info.qobject!=null && !info.qobject.isDisposed() && info.reflectiveMethod!=null && (info.reflectiveMethod!=null || info.signal!=null)) {
			QMetaMethod qmethod = info.reflectiveMethod!=null ? fromReflectedMethod(info.reflectiveMethod) : info.qobject.metaObject().methodByIndex(info.qobject.metaObject().metaObjectPointer, info.signal.methodIndex());
			if(qmethod!=null && qmethod.parameterTypes().size()==info.lambdaArgs.size()+5) {
				if(info.lambdaArgs.isEmpty()) {
					return (R)qmethod.invoke(info.qobject, type, arg1, arg2, arg3, arg4, arg5);
				}else {
					Object[] args = new Object[qmethod.parameterTypes().size()];
					args[0] = arg1;
					args[1] = arg2;
					args[2] = arg3;
					args[3] = arg4;
					args[4] = arg5;
					System.arraycopy(info.lambdaArgs.toArray(), 0, args, 5, info.lambdaArgs.size());
					return (R)qmethod.invoke(info.qobject, type, args);
				}
			}
			if(qmethod!=null) {
				return (R)qmethod.invoke(info.qobject, type, arg1, arg2, arg3, arg4, arg5);
			}
		}
		if(method instanceof QtObjectInterface) {
			if( ((QtObjectInterface)method).isDisposed() )
				return null;
		}
		switch(type) {
		case AutoConnection:
			if(method instanceof QtThreadAffineInterface) {
				QThread thread = ((QtThreadAffineInterface) method).thread();
				if(thread!=null && thread!=QThread.currentThread()) {
					throw new QUnsuccessfulInvocationException("Unable to invoke methods with return values in queued connections (auto connection with different threads).");
				}
			}
		case DirectConnection:
			try {
				return method.invoke(arg1, arg2, arg3, arg4, arg5);
			} catch (Throwable e) {
				throw new QUnsuccessfulInvocationException(e);
			}
		case BlockingQueuedConnection:
			if(method instanceof QtThreadAffineInterface) {
				QThread thread = ((QtThreadAffineInterface) method).thread();
				if(thread!=null && thread!=QThread.currentThread()) {
					AtomicReference<R> result = new AtomicReference<>();
					QObject invoker = new QObject() {
						@Override
						public boolean event(QEvent event) {
							if(event.type()==QEvent.Type.DeferredDispose
									&& (info==null || info.qobject==null || !info.qobject.isDisposed()) 
									&& !(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
								try {
									result.set(method.invoke(arg1, arg2, arg3, arg4, arg5));
								} catch (Throwable e) {
									Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invokation.", e);
								}
								synchronized (this) {
									notifyAll();
								}
							}
							return super.event(event);
						}
					};
					invoker.moveToThread(thread);
					invoker.disposeLater();
					synchronized (invoker) {
						try {
							invoker.wait();
						} catch (InterruptedException e) {
							throw new QUnsuccessfulInvocationException(e);
						}
					}
					return result.get();
				}
			}
			throw new QUnsuccessfulInvocationException("Blocking-queued invocation of method on not allowed without thread affinity.");
		case QueuedConnection:
			throw new QUnsuccessfulInvocationException("Unable to invoke methods with return values in queued connections.");
		default:
			break;
		}
		throw new QUnsuccessfulInvocationException("Unable to invoke method.");
	}
	
	public static <A,B,C,D,E,F,R> R invokeMethod(Method6<A,B,C,D,E,F,R> method, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
		return invokeMethod(method, Qt.ConnectionType.AutoConnection, arg1, arg2, arg3, arg4, arg5, arg6);
	}
	
	@SuppressWarnings("unchecked")
	public static <A,B,C,D,E,F,R> R invokeMethod(Method6<A,B,C,D,E,F,R> method, Qt.ConnectionType type, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
		QtJambiInternal.LambdaInfo info = QtJambiInternal.lamdaInfo(method);
		if(info!=null && info.qobject!=null && !info.qobject.isDisposed() && info.reflectiveMethod!=null && (info.reflectiveMethod!=null || info.signal!=null)) {
			QMetaMethod qmethod = info.reflectiveMethod!=null ? fromReflectedMethod(info.reflectiveMethod) : info.qobject.metaObject().methodByIndex(info.qobject.metaObject().metaObjectPointer, info.signal.methodIndex());
			if(qmethod!=null && qmethod.parameterTypes().size()==info.lambdaArgs.size()+6) {
				if(info.lambdaArgs.isEmpty()) {
					return (R)qmethod.invoke(info.qobject, type, arg1, arg2, arg3, arg4, arg5, arg6);
				}else {
					Object[] args = new Object[qmethod.parameterTypes().size()];
					args[0] = arg1;
					args[1] = arg2;
					args[2] = arg3;
					args[3] = arg4;
					args[4] = arg5;
					args[5] = arg6;
					System.arraycopy(info.lambdaArgs.toArray(), 0, args, 6, info.lambdaArgs.size());
					return (R)qmethod.invoke(info.qobject, type, args);
				}
			}
		}
		if(method instanceof QtObjectInterface) {
			if( ((QtObjectInterface)method).isDisposed() )
				return null;
		}
		switch(type) {
		case AutoConnection:
			if(method instanceof QtThreadAffineInterface) {
				QThread thread = ((QtThreadAffineInterface) method).thread();
				if(thread!=null && thread!=QThread.currentThread()) {
					throw new QUnsuccessfulInvocationException("Unable to invoke methods with return values in queued connections (auto connection with different threads).");
				}
			}
		case DirectConnection:
			try {
				return method.invoke(arg1, arg2, arg3, arg4, arg5, arg6);
			} catch (Throwable e) {
				throw new QUnsuccessfulInvocationException(e);
			}
		case BlockingQueuedConnection:
			if(method instanceof QtThreadAffineInterface) {
				QThread thread = ((QtThreadAffineInterface) method).thread();
				if(thread!=null && thread!=QThread.currentThread()) {
					AtomicReference<R> result = new AtomicReference<>();
					QObject invoker = new QObject() {
						@Override
						public boolean event(QEvent event) {
							if(event.type()==QEvent.Type.DeferredDispose
									&& (info==null || info.qobject==null || !info.qobject.isDisposed()) 
									&& !(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
								try {
									result.set(method.invoke(arg1, arg2, arg3, arg4, arg5, arg6));
								} catch (Throwable e) {
									Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invokation.", e);
								}
								synchronized (this) {
									notifyAll();
								}
							}
							return super.event(event);
						}
					};
					invoker.moveToThread(thread);
					invoker.disposeLater();
					synchronized (invoker) {
						try {
							invoker.wait();
						} catch (InterruptedException e) {
							throw new QUnsuccessfulInvocationException(e);
						}
					}
					return result.get();
				}
			}
			throw new QUnsuccessfulInvocationException("Blocking-queued invocation of method on not allowed without thread affinity.");
		case QueuedConnection:
			throw new QUnsuccessfulInvocationException("Unable to invoke methods with return values in queued connections.");
		default:
			break;
		}
		throw new QUnsuccessfulInvocationException("Unable to invoke method.");
	}
	
	public static <A,B,C,D,E,F,G,R> R invokeMethod(Method7<A,B,C,D,E,F,G,R> method, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
		return invokeMethod(method, Qt.ConnectionType.AutoConnection, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
	}
	
	@SuppressWarnings("unchecked")
	public static <A,B,C,D,E,F,G,R> R invokeMethod(Method7<A,B,C,D,E,F,G,R> method, Qt.ConnectionType type, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
		QtJambiInternal.LambdaInfo info = QtJambiInternal.lamdaInfo(method);
		if(info!=null && info.qobject!=null && !info.qobject.isDisposed() && info.reflectiveMethod!=null && (info.reflectiveMethod!=null || info.signal!=null)) {
			QMetaMethod qmethod = info.reflectiveMethod!=null ? fromReflectedMethod(info.reflectiveMethod) : info.qobject.metaObject().methodByIndex(info.qobject.metaObject().metaObjectPointer, info.signal.methodIndex());
			if(qmethod!=null && qmethod.parameterTypes().size()==info.lambdaArgs.size()+7) {
				if(info.lambdaArgs.isEmpty()) {
					return (R)qmethod.invoke(info.qobject, type, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
				}else {
					Object[] args = new Object[qmethod.parameterTypes().size()];
					args[0] = arg1;
					args[1] = arg2;
					args[2] = arg3;
					args[3] = arg4;
					args[4] = arg5;
					args[5] = arg6;
					args[6] = arg7;
					System.arraycopy(info.lambdaArgs.toArray(), 0, args, 7, info.lambdaArgs.size());
					return (R)qmethod.invoke(info.qobject, type, args);
				}
			}
		}
		if(method instanceof QtObjectInterface) {
			if( ((QtObjectInterface)method).isDisposed() )
				return null;
		}
		switch(type) {
		case AutoConnection:
			if(method instanceof QtThreadAffineInterface) {
				QThread thread = ((QtThreadAffineInterface) method).thread();
				if(thread!=null && thread!=QThread.currentThread()) {
					throw new QUnsuccessfulInvocationException("Unable to invoke methods with return values in queued connections (auto connection with different threads).");
				}
			}
		case DirectConnection:
			try {
				return method.invoke(arg1, arg2, arg3, arg4, arg5, arg6, arg7);
			} catch (Throwable e) {
				throw new QUnsuccessfulInvocationException(e);
			}
		case BlockingQueuedConnection:
			if(method instanceof QtThreadAffineInterface) {
				QThread thread = ((QtThreadAffineInterface) method).thread();
				if(thread!=null && thread!=QThread.currentThread()) {
					AtomicReference<R> result = new AtomicReference<>();
					QObject invoker = new QObject() {
						@Override
						public boolean event(QEvent event) {
							if(event.type()==QEvent.Type.DeferredDispose
									&& (info==null || info.qobject==null || !info.qobject.isDisposed()) 
									&& !(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
								try {
									result.set(method.invoke(arg1, arg2, arg3, arg4, arg5, arg6, arg7));
								} catch (Throwable e) {
									Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invokation.", e);
								}
								synchronized (this) {
									notifyAll();
								}
							}
							return super.event(event);
						}
					};
					invoker.moveToThread(thread);
					invoker.disposeLater();
					synchronized (invoker) {
						try {
							invoker.wait();
						} catch (InterruptedException e) {
							throw new QUnsuccessfulInvocationException(e);
						}
					}
					return result.get();
				}
			}
			throw new QUnsuccessfulInvocationException("Blocking-queued invocation of method on not allowed without thread affinity.");
		case QueuedConnection:
			throw new QUnsuccessfulInvocationException("Unable to invoke methods with return values in queued connections.");
		default:
			break;
		}
		throw new QUnsuccessfulInvocationException("Unable to invoke method.");
	}
	
	public static <A,B,C,D,E,F,G,H,R> R invokeMethod(Method8<A,B,C,D,E,F,G,H,R> method, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7, H arg8) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
		return invokeMethod(method, Qt.ConnectionType.AutoConnection, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
	}
	
	@SuppressWarnings("unchecked")
	public static <A,B,C,D,E,F,G,H,R> R invokeMethod(Method8<A,B,C,D,E,F,G,H,R> method, Qt.ConnectionType type, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7, H arg8) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
		QtJambiInternal.LambdaInfo info = QtJambiInternal.lamdaInfo(method);
		if(info!=null && info.qobject!=null && !info.qobject.isDisposed() && info.reflectiveMethod!=null && (info.reflectiveMethod!=null || info.signal!=null)) {
			QMetaMethod qmethod = info.reflectiveMethod!=null ? fromReflectedMethod(info.reflectiveMethod) : info.qobject.metaObject().methodByIndex(info.qobject.metaObject().metaObjectPointer, info.signal.methodIndex());
			if(qmethod!=null && qmethod.parameterTypes().size()==info.lambdaArgs.size()+8) {
				if(info.lambdaArgs.isEmpty()) {
					return (R)qmethod.invoke(info.qobject, type, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
				}else {
					Object[] args = new Object[qmethod.parameterTypes().size()];
					args[0] = arg1;
					args[1] = arg2;
					args[2] = arg3;
					args[3] = arg4;
					args[4] = arg5;
					args[5] = arg6;
					args[6] = arg7;
					args[7] = arg8;
					System.arraycopy(info.lambdaArgs.toArray(), 0, args, 8, info.lambdaArgs.size());
					return (R)qmethod.invoke(info.qobject, type, args);
				}
			}
		}
		if(method instanceof QtObjectInterface) {
			if( ((QtObjectInterface)method).isDisposed() )
				return null;
		}
		switch(type) {
		case AutoConnection:
			if(method instanceof QtThreadAffineInterface) {
				QThread thread = ((QtThreadAffineInterface) method).thread();
				if(thread!=null && thread!=QThread.currentThread()) {
					throw new QUnsuccessfulInvocationException("Unable to invoke methods with return values in queued connections (auto connection with different threads).");
				}
			}
		case DirectConnection:
			try {
				return method.invoke(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
			} catch (Throwable e) {
				throw new QUnsuccessfulInvocationException(e);
			}
		case BlockingQueuedConnection:
			if(method instanceof QtThreadAffineInterface) {
				QThread thread = ((QtThreadAffineInterface) method).thread();
				if(thread!=null && thread!=QThread.currentThread()) {
					AtomicReference<R> result = new AtomicReference<>();
					QObject invoker = new QObject() {
						@Override
						public boolean event(QEvent event) {
							if(event.type()==QEvent.Type.DeferredDispose
									&& (info==null || info.qobject==null || !info.qobject.isDisposed()) 
									&& !(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
								try {
									result.set(method.invoke(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8));
								} catch (Throwable e) {
									Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invokation.", e);
								}
								synchronized (this) {
									notifyAll();
								}
							}
							return super.event(event);
						}
					};
					invoker.moveToThread(thread);
					invoker.disposeLater();
					synchronized (invoker) {
						try {
							invoker.wait();
						} catch (InterruptedException e) {
							throw new QUnsuccessfulInvocationException(e);
						}
					}
					return result.get();
				}
			}
			throw new QUnsuccessfulInvocationException("Blocking-queued invocation of method on not allowed without thread affinity.");
		case QueuedConnection:
			throw new QUnsuccessfulInvocationException("Unable to invoke methods with return values in queued connections.");
		default:
			break;
		}
		throw new QUnsuccessfulInvocationException("Unable to invoke method.");
	}
	
	public static <A,B,C,D,E,F,G,H,I,R> R invokeMethod(Method9<A,B,C,D,E,F,G,H,I,R> method, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7, H arg8, I arg9) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
		return invokeMethod(method, Qt.ConnectionType.AutoConnection, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
	}
	
	@SuppressWarnings("unchecked")
	public static <A,B,C,D,E,F,G,H,I,R> R invokeMethod(Method9<A,B,C,D,E,F,G,H,I,R> method, Qt.ConnectionType type, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7, H arg8, I arg9) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
		QtJambiInternal.LambdaInfo info = QtJambiInternal.lamdaInfo(method);
		if(info!=null && info.qobject!=null && !info.qobject.isDisposed() && info.reflectiveMethod!=null && (info.reflectiveMethod!=null || info.signal!=null)) {
			QMetaMethod qmethod = info.reflectiveMethod!=null ? fromReflectedMethod(info.reflectiveMethod) : info.qobject.metaObject().methodByIndex(info.qobject.metaObject().metaObjectPointer, info.signal.methodIndex());
			if(qmethod!=null && qmethod.parameterTypes().size()==info.lambdaArgs.size()+9) {
				if(info.lambdaArgs.isEmpty()) {
					return (R)qmethod.invoke(info.qobject, type, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
				}else {
					Object[] args = new Object[qmethod.parameterTypes().size()];
					args[0] = arg1;
					args[1] = arg2;
					args[2] = arg3;
					args[3] = arg4;
					args[4] = arg5;
					args[5] = arg6;
					args[6] = arg7;
					args[7] = arg8;
					args[8] = arg9;
					System.arraycopy(info.lambdaArgs.toArray(), 0, args, 9, info.lambdaArgs.size());
					return (R)qmethod.invoke(info.qobject, type, args);
				}
			}
		}
		if(method instanceof QtObjectInterface) {
			if( ((QtObjectInterface)method).isDisposed() )
				return null;
		}
		switch(type) {
		case AutoConnection:
			if(method instanceof QtThreadAffineInterface) {
				QThread thread = ((QtThreadAffineInterface) method).thread();
				if(thread!=null && thread!=QThread.currentThread()) {
					throw new QUnsuccessfulInvocationException("Unable to invoke methods with return values in queued connections (auto connection with different threads).");
				}
			}
		case DirectConnection:
			try {
				return method.invoke(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
			} catch (Throwable e) {
				throw new QUnsuccessfulInvocationException(e);
			}
		case BlockingQueuedConnection:
			if(method instanceof QtThreadAffineInterface) {
				QThread thread = ((QtThreadAffineInterface) method).thread();
				if(thread!=null && thread!=QThread.currentThread()) {
					AtomicReference<R> result = new AtomicReference<>();
					QObject invoker = new QObject() {
						@Override
						public boolean event(QEvent event) {
							if(event.type()==QEvent.Type.DeferredDispose
									&& (info==null || info.qobject==null || !info.qobject.isDisposed()) 
									&& !(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
								try {
									result.set(method.invoke(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9));
								} catch (Throwable e) {
									Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invokation.", e);
								}
								synchronized (this) {
									notifyAll();
								}
							}
							return super.event(event);
						}
					};
					invoker.moveToThread(thread);
					invoker.disposeLater();
					synchronized (invoker) {
						try {
							invoker.wait();
						} catch (InterruptedException e) {
							throw new QUnsuccessfulInvocationException(e);
						}
					}
					return result.get();
				}
			}
			throw new QUnsuccessfulInvocationException("Blocking-queued invocation of method on not allowed without thread affinity.");
		case QueuedConnection:
			throw new QUnsuccessfulInvocationException("Unable to invoke methods with return values in queued connections.");
		default:
			break;
		}
		throw new QUnsuccessfulInvocationException("Unable to invoke method.");
	}
	
	public static void invokeMethod(Slot0 method) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
		invokeMethod(method, Qt.ConnectionType.AutoConnection);
	}
	
	public static void invokeMethod(Slot0 method, Qt.ConnectionType type) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
		QtJambiInternal.LambdaInfo info = QtJambiInternal.lamdaInfo(method);
		QThread thread = null;
		if(info!=null && info.qobject!=null && !info.qobject.isDisposed()) {
			if(info.reflectiveMethod!=null || info.signal!=null) {
				QMetaMethod qmethod = info.reflectiveMethod!=null ? fromReflectedMethod(info.reflectiveMethod) : info.qobject.metaObject().methodByIndex(info.qobject.metaObject().metaObjectPointer, info.signal.methodIndex());
				if(qmethod!=null && qmethod.parameterTypes().size()==info.lambdaArgs.size()) {
					qmethod.invoke(info.qobject, type, info.lambdaArgs.toArray());
					return;
				}
			}
			thread = info.qobject.thread();
		}else {
			if(method instanceof QtObjectInterface) {
				if( ((QtObjectInterface)method).isDisposed() )
					return;
			}
			if(method instanceof QtThreadAffineInterface)
				thread = ((QtThreadAffineInterface) method).thread();
		}
		if(type==Qt.ConnectionType.AutoConnection && thread!=null && thread!=QThread.currentThread()) {
			type = Qt.ConnectionType.QueuedConnection;
		}
		switch(type) {
		case AutoConnection:
		case DirectConnection:
			try {
				method.invoke();
				return;
			} catch (Throwable e) {
				throw new QUnsuccessfulInvocationException(e);
			}
		case BlockingQueuedConnection:
			if(thread!=null) {
				if(thread!=QThread.currentThread()) {
					QObject invoker = new QObject() {
						@Override
						public boolean event(QEvent event) {
							if(event.type()==QEvent.Type.DeferredDispose
									&& (info==null || info.qobject==null || !info.qobject.isDisposed()) 
									&& !(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
								try {
									method.invoke();
								} catch (Throwable e) {
									Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invokation.", e);
								}
								synchronized (this) {
									notifyAll();
								}
							}
							return super.event(event);
						}
					};
					invoker.moveToThread(thread);
					invoker.disposeLater();
					synchronized (invoker) {
						try {
							invoker.wait();
						} catch (InterruptedException e) {
							throw new QUnsuccessfulInvocationException(e);
						}
					}
					return;
				}else {
					throw new QUnsuccessfulInvocationException("Blocking-queued invocation on object whose thread is the current thread is not allowed.");
				}
			}
			throw new QUnsuccessfulInvocationException("Blocking-queued invocation of method on not allowed without thread affinity.");
		case QueuedConnection:
			QObject invoker = new QObject() {
				@Override
				public boolean event(QEvent event) {
					if(event.type()==QEvent.Type.DeferredDispose
							&& (info==null || info.qobject==null || !info.qobject.isDisposed()) 
							&& !(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
						try {
							method.invoke();
						} catch (Throwable e) {
							Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invokation.", e);
						}
					}
					return super.event(event);
				}
			};
			QtJambiInternal.setCppOwnership(invoker);
			if(thread!=null)
				invoker.moveToThread(thread);
			invoker.disposeLater();
			return;
		default:
			break;
		}
		throw new QUnsuccessfulInvocationException("Unable to invoke method.");
	}
	
	public static <A> void invokeMethod(Slot1<A> method, A arg1) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
		invokeMethod(method, Qt.ConnectionType.AutoConnection, arg1);
	}
	
	public static <A> void invokeMethod(Slot1<A> method, Qt.ConnectionType type, A arg1) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
		QtJambiInternal.LambdaInfo info = QtJambiInternal.lamdaInfo(method);
		QThread thread = null;
		if(info!=null && info.qobject!=null && !info.qobject.isDisposed()) {
			if(info.reflectiveMethod!=null || info.signal!=null) {
				QMetaMethod qmethod = info.reflectiveMethod!=null ? fromReflectedMethod(info.reflectiveMethod) : info.qobject.metaObject().methodByIndex(info.qobject.metaObject().metaObjectPointer, info.signal.methodIndex());
				if(qmethod!=null && qmethod.parameterTypes().size()==info.lambdaArgs.size()+1) {
					if(info.lambdaArgs.isEmpty()) {
						qmethod.invoke(info.qobject, type, arg1);
					}else {
						Object[] args = new Object[qmethod.parameterTypes().size()];
						args[0] = arg1;
						System.arraycopy(info.lambdaArgs.toArray(), 0, args, 1, info.lambdaArgs.size());
						qmethod.invoke(info.qobject, type, args);
					}
					return;
				}
			}
			thread = info.qobject.thread();
		}else {
			if(method instanceof QtObjectInterface) {
				if( ((QtObjectInterface)method).isDisposed() )
					return;
			}
			if(method instanceof QtThreadAffineInterface)
				thread = ((QtThreadAffineInterface) method).thread();
		}
		if(type==Qt.ConnectionType.AutoConnection && thread!=null && thread!=QThread.currentThread()) {
			type = Qt.ConnectionType.QueuedConnection;
		}
		switch(type) {
		case AutoConnection:
		case DirectConnection:
			try {
				method.invoke(arg1);
				return;
			} catch (Throwable e) {
				throw new QUnsuccessfulInvocationException(e);
			}
		case BlockingQueuedConnection:
			if(thread!=null) {
				if(thread!=QThread.currentThread()) {
					QObject invoker = new QObject() {
						@Override
						public boolean event(QEvent event) {
							if(event.type()==QEvent.Type.DeferredDispose
									&& (info==null || info.qobject==null || !info.qobject.isDisposed()) 
									&& !(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
								try {
									method.invoke(arg1);
								} catch (Throwable e) {
									Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invokation.", e);
								}
								synchronized (this) {
									notifyAll();
								}
							}
							return super.event(event);
						}
					};
					invoker.moveToThread(thread);
					invoker.disposeLater();
					synchronized (invoker) {
						try {
							invoker.wait();
						} catch (InterruptedException e) {
							throw new QUnsuccessfulInvocationException(e);
						}
					}
					return;
				}else {
					throw new QUnsuccessfulInvocationException("Blocking-queued invocation on object whose thread is the current thread is not allowed.");
				}
			}
			throw new QUnsuccessfulInvocationException("Blocking-queued invocation of method on not allowed without thread affinity.");
		case QueuedConnection:
			QObject invoker = new QObject() {
				@Override
				public boolean event(QEvent event) {
					if(event.type()==QEvent.Type.DeferredDispose
							&& (info==null || info.qobject==null || !info.qobject.isDisposed()) 
							&& !(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
						try {
							method.invoke(arg1);
						} catch (Throwable e) {
							Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invokation.", e);
						}
					}
					return super.event(event);
				}
			};
			QtJambiInternal.setCppOwnership(invoker);
			if(thread!=null)
				invoker.moveToThread(thread);
			invoker.disposeLater();
			return;
		default:
			break;
		}
		throw new QUnsuccessfulInvocationException("Unable to invoke method.");
	}
	
	public static <A,B> void invokeMethod(Slot2<A,B> method, A arg1, B arg2) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
		invokeMethod(method, Qt.ConnectionType.AutoConnection, arg1, arg2);
	}
	
	public static <A,B> void invokeMethod(Slot2<A,B> method, Qt.ConnectionType type, A arg1, B arg2) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
		QtJambiInternal.LambdaInfo info = QtJambiInternal.lamdaInfo(method);
		QThread thread = null;
		if(info!=null && info.qobject!=null && !info.qobject.isDisposed()) {
			if(info.reflectiveMethod!=null || info.signal!=null) {
				QMetaMethod qmethod = info.reflectiveMethod!=null ? fromReflectedMethod(info.reflectiveMethod) : info.qobject.metaObject().methodByIndex(info.qobject.metaObject().metaObjectPointer, info.signal.methodIndex());
				if(qmethod!=null && qmethod.parameterTypes().size()==info.lambdaArgs.size()+2) {
					if(info.lambdaArgs.isEmpty()) {
						qmethod.invoke(info.qobject, type, arg1, arg2);
					}else {
						Object[] args = new Object[qmethod.parameterTypes().size()];
						args[0] = arg1;
						args[1] = arg2;
						System.arraycopy(info.lambdaArgs.toArray(), 0, args, 2, info.lambdaArgs.size());
						qmethod.invoke(info.qobject, type, args);
					}
					return;
				}
				if(qmethod!=null) {
					qmethod.invoke(info.qobject, type, arg1, arg2);
					return;
				}
			}
			thread = info.qobject.thread();
		}else {
			if(method instanceof QtObjectInterface) {
				if( ((QtObjectInterface)method).isDisposed() )
					return;
			}
			if(method instanceof QtThreadAffineInterface)
				thread = ((QtThreadAffineInterface) method).thread();
		}
		if(type==Qt.ConnectionType.AutoConnection && thread!=null && thread!=QThread.currentThread()) {
			type = Qt.ConnectionType.QueuedConnection;
		}
		switch(type) {
		case AutoConnection:
		case DirectConnection:
			try {
				method.invoke(arg1, arg2);
				return;
			} catch (Throwable e) {
				throw new QUnsuccessfulInvocationException(e);
			}
		case BlockingQueuedConnection:
			if(thread!=null) {
				if(thread!=QThread.currentThread()) {
					QObject invoker = new QObject() {
						@Override
						public boolean event(QEvent event) {
							if(event.type()==QEvent.Type.DeferredDispose
									&& (info==null || info.qobject==null || !info.qobject.isDisposed()) 
									&& !(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
								try {
									method.invoke(arg1, arg2);
								} catch (Throwable e) {
									Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invokation.", e);
								}
								synchronized (this) {
									notifyAll();
								}
							}
							return super.event(event);
						}
					};
					invoker.moveToThread(thread);
					invoker.disposeLater();
					synchronized (invoker) {
						try {
							invoker.wait();
						} catch (InterruptedException e) {
							throw new QUnsuccessfulInvocationException(e);
						}
					}
					return;
				}else {
					throw new QUnsuccessfulInvocationException("Blocking-queued invocation on object whose thread is the current thread is not allowed.");
				}
			}
			throw new QUnsuccessfulInvocationException("Blocking-queued invocation of method on not allowed without thread affinity.");
		case QueuedConnection:
			QObject invoker = new QObject() {
				@Override
				public boolean event(QEvent event) {
					if(event.type()==QEvent.Type.DeferredDispose
							&& (info==null || info.qobject==null || !info.qobject.isDisposed()) 
							&& !(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
						try {
							method.invoke(arg1, arg2);
						} catch (Throwable e) {
							Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invokation.", e);
						}
					}
					return super.event(event);
				}
			};
			QtJambiInternal.setCppOwnership(invoker);
			if(thread!=null)
				invoker.moveToThread(thread);
			invoker.disposeLater();
			return;
		default:
			break;
		}
		throw new QUnsuccessfulInvocationException("Unable to invoke method.");
	}
	
	public static <A,B,C> void invokeMethod(Slot3<A,B,C> method, A arg1, B arg2, C arg3) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
		invokeMethod(method, Qt.ConnectionType.AutoConnection, arg1, arg2, arg3);
	}
	
	public static <A,B,C> void invokeMethod(Slot3<A,B,C> method, Qt.ConnectionType type, A arg1, B arg2, C arg3) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
		QtJambiInternal.LambdaInfo info = QtJambiInternal.lamdaInfo(method);
		QThread thread = null;
		if(info!=null && info.qobject!=null) {
			if(info.reflectiveMethod!=null || info.signal!=null) {
				QMetaMethod qmethod = info.reflectiveMethod!=null ? fromReflectedMethod(info.reflectiveMethod) : info.qobject.metaObject().methodByIndex(info.qobject.metaObject().metaObjectPointer, info.signal.methodIndex());
				if(qmethod!=null && qmethod.parameterTypes().size()==info.lambdaArgs.size()+3) {
					if(info.lambdaArgs.isEmpty()) {
						qmethod.invoke(info.qobject, type, arg1, arg2, arg3);
					}else {
						Object[] args = new Object[qmethod.parameterTypes().size()];
						args[0] = arg1;
						args[1] = arg2;
						args[2] = arg3;
						System.arraycopy(info.lambdaArgs.toArray(), 0, args, 3, info.lambdaArgs.size());
						qmethod.invoke(info.qobject, type, args);
					}
					return;
				}
			}
			thread = info.qobject.thread();
		}else {
			if(method instanceof QtObjectInterface) {
				if( ((QtObjectInterface)method).isDisposed() )
					return;
			}
			if(method instanceof QtThreadAffineInterface)
				thread = ((QtThreadAffineInterface) method).thread();
		}
		if(type==Qt.ConnectionType.AutoConnection && thread!=null && thread!=QThread.currentThread()) {
			type = Qt.ConnectionType.QueuedConnection;
		}
		switch(type) {
		case AutoConnection:
		case DirectConnection:
			try {
				method.invoke(arg1, arg2, arg3);
				return;
			} catch (Throwable e) {
				throw new QUnsuccessfulInvocationException(e);
			}
		case BlockingQueuedConnection:
			if(thread!=null) {
				if(thread!=QThread.currentThread()) {
					QObject invoker = new QObject() {
						@Override
						public boolean event(QEvent event) {
							if(event.type()==QEvent.Type.DeferredDispose
									&& (info==null || info.qobject==null || !info.qobject.isDisposed()) 
									&& !(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
								try {
									method.invoke(arg1, arg2, arg3);
								} catch (Throwable e) {
									Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invokation.", e);
								}
								synchronized (this) {
									notifyAll();
								}
							}
							return super.event(event);
						}
					};
					invoker.moveToThread(thread);
					invoker.disposeLater();
					synchronized (invoker) {
						try {
							invoker.wait();
						} catch (InterruptedException e) {
							throw new QUnsuccessfulInvocationException(e);
						}
					}
					return;
				}else {
					throw new QUnsuccessfulInvocationException("Blocking-queued invocation on object whose thread is the current thread is not allowed.");
				}
			}
			throw new QUnsuccessfulInvocationException("Blocking-queued invocation of method on not allowed without thread affinity.");
		case QueuedConnection:
			QObject invoker = new QObject() {
				@Override
				public boolean event(QEvent event) {
					if(event.type()==QEvent.Type.DeferredDispose
							&& (info==null || info.qobject==null || !info.qobject.isDisposed()) 
							&& !(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
						try {
							method.invoke(arg1, arg2, arg3);
						} catch (Throwable e) {
							Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invokation.", e);
						}
					}
					return super.event(event);
				}
			};
			QtJambiInternal.setCppOwnership(invoker);
			if(thread!=null)
				invoker.moveToThread(thread);
			invoker.disposeLater();
			return;
		default:
			break;
		}
		throw new QUnsuccessfulInvocationException("Unable to invoke method.");
	}
	
	public static <A,B,C,D> void invokeMethod(Slot4<A,B,C,D> method, A arg1, B arg2, C arg3, D arg4) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
		invokeMethod(method, Qt.ConnectionType.AutoConnection, arg1, arg2, arg3, arg4);
	}
	
	public static <A,B,C,D> void invokeMethod(Slot4<A,B,C,D> method, Qt.ConnectionType type, A arg1, B arg2, C arg3, D arg4) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
		QtJambiInternal.LambdaInfo info = QtJambiInternal.lamdaInfo(method);
		QThread thread = null;
		if(info!=null && info.qobject!=null) {
			if(info.reflectiveMethod!=null || info.signal!=null) {
				QMetaMethod qmethod = info.reflectiveMethod!=null ? fromReflectedMethod(info.reflectiveMethod) : info.qobject.metaObject().methodByIndex(info.qobject.metaObject().metaObjectPointer, info.signal.methodIndex());
				if(qmethod!=null && qmethod.parameterTypes().size()==info.lambdaArgs.size()+4) {
					if(info.lambdaArgs.isEmpty()) {
						qmethod.invoke(info.qobject, type, arg1, arg2, arg3, arg4);
					}else {
						Object[] args = new Object[qmethod.parameterTypes().size()];
						args[0] = arg1;
						args[1] = arg2;
						args[2] = arg3;
						args[3] = arg4;
						System.arraycopy(info.lambdaArgs.toArray(), 0, args, 4, info.lambdaArgs.size());
						qmethod.invoke(info.qobject, type, args);
					}
					return;
				}
			}
			thread = info.qobject.thread();
		}else {
			if(method instanceof QtObjectInterface) {
				if( ((QtObjectInterface)method).isDisposed() )
					return;
			}
			if(method instanceof QtThreadAffineInterface)
				thread = ((QtThreadAffineInterface) method).thread();
		}
		if(type==Qt.ConnectionType.AutoConnection && thread!=null && thread!=QThread.currentThread()) {
			type = Qt.ConnectionType.QueuedConnection;
		}
		switch(type) {
		case AutoConnection:
		case DirectConnection:
			try {
				method.invoke(arg1, arg2, arg3, arg4);
				return;
			} catch (Throwable e) {
				throw new QUnsuccessfulInvocationException(e);
			}
		case BlockingQueuedConnection:
			if(thread!=null) {
				if(thread!=QThread.currentThread()) {
					QObject invoker = new QObject() {
						@Override
						public boolean event(QEvent event) {
							if(event.type()==QEvent.Type.DeferredDispose
									&& (info==null || info.qobject==null || !info.qobject.isDisposed()) 
									&& !(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
								try {
									method.invoke(arg1, arg2, arg3, arg4);
								} catch (Throwable e) {
									Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invokation.", e);
								}
								synchronized (this) {
									notifyAll();
								}
							}
							return super.event(event);
						}
					};
					invoker.moveToThread(thread);
					invoker.disposeLater();
					synchronized (invoker) {
						try {
							invoker.wait();
						} catch (InterruptedException e) {
							throw new QUnsuccessfulInvocationException(e);
						}
					}
					return;
				}else {
					throw new QUnsuccessfulInvocationException("Blocking-queued invocation on object whose thread is the current thread is not allowed.");
				}
			}
			throw new QUnsuccessfulInvocationException("Blocking-queued invocation of method on not allowed without thread affinity.");
		case QueuedConnection:
			QObject invoker = new QObject() {
				@Override
				public boolean event(QEvent event) {
					if(event.type()==QEvent.Type.DeferredDispose
							&& (info==null || info.qobject==null || !info.qobject.isDisposed()) 
							&& !(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
						try {
							method.invoke(arg1, arg2, arg3, arg4);
						} catch (Throwable e) {
							Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invokation.", e);
						}
					}
					return super.event(event);
				}
			};
			QtJambiInternal.setCppOwnership(invoker);
			if(thread!=null)
				invoker.moveToThread(thread);
			invoker.disposeLater();
			return;
		default:
			break;
		}
		throw new QUnsuccessfulInvocationException("Unable to invoke method.");
	}
	
	public static <A,B,C,D,E> void invokeMethod(Slot5<A,B,C,D,E> method, A arg1, B arg2, C arg3, D arg4, E arg5) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
		invokeMethod(method, Qt.ConnectionType.AutoConnection, arg1, arg2, arg3, arg4, arg5);
	}
	
	public static <A,B,C,D,E> void invokeMethod(Slot5<A,B,C,D,E> method, Qt.ConnectionType type, A arg1, B arg2, C arg3, D arg4, E arg5) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
		QtJambiInternal.LambdaInfo info = QtJambiInternal.lamdaInfo(method);
		QThread thread = null;
		if(info!=null && info.qobject!=null) {
			if(info.reflectiveMethod!=null || info.signal!=null) {
				QMetaMethod qmethod = info.reflectiveMethod!=null ? fromReflectedMethod(info.reflectiveMethod) : info.qobject.metaObject().methodByIndex(info.qobject.metaObject().metaObjectPointer, info.signal.methodIndex());
				if(qmethod!=null && qmethod.parameterTypes().size()==info.lambdaArgs.size()+5) {
					if(info.lambdaArgs.isEmpty()) {
						qmethod.invoke(info.qobject, type, arg1, arg2, arg3, arg4, arg5);
					}else {
						Object[] args = new Object[qmethod.parameterTypes().size()];
						args[0] = arg1;
						args[1] = arg2;
						args[2] = arg3;
						args[3] = arg4;
						args[4] = arg5;
						System.arraycopy(info.lambdaArgs.toArray(), 0, args, 5, info.lambdaArgs.size());
						qmethod.invoke(info.qobject, type, args);
					}
					return;
				}
			}
			thread = info.qobject.thread();
		}else {
			if(method instanceof QtObjectInterface) {
				if( ((QtObjectInterface)method).isDisposed() )
					return;
			}
			if(method instanceof QtThreadAffineInterface)
				thread = ((QtThreadAffineInterface) method).thread();
		}
		if(type==Qt.ConnectionType.AutoConnection && thread!=null && thread!=QThread.currentThread()) {
			type = Qt.ConnectionType.QueuedConnection;
		}
		switch(type) {
		case AutoConnection:
		case DirectConnection:
			try {
				method.invoke(arg1, arg2, arg3, arg4, arg5);
				return;
			} catch (Throwable e) {
				throw new QUnsuccessfulInvocationException(e);
			}
		case BlockingQueuedConnection:
			if(thread!=null) {
				if(thread!=QThread.currentThread()) {
					QObject invoker = new QObject() {
						@Override
						public boolean event(QEvent event) {
							if(event.type()==QEvent.Type.DeferredDispose
									&& (info==null || info.qobject==null || !info.qobject.isDisposed()) 
									&& !(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
								try {
									method.invoke(arg1, arg2, arg3, arg4, arg5);
								} catch (Throwable e) {
									Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invokation.", e);
								}
								synchronized (this) {
									notifyAll();
								}
							}
							return super.event(event);
						}
					};
					invoker.moveToThread(thread);
					invoker.disposeLater();
					synchronized (invoker) {
						try {
							invoker.wait();
						} catch (InterruptedException e) {
							throw new QUnsuccessfulInvocationException(e);
						}
					}
					return;
				}else {
					throw new QUnsuccessfulInvocationException("Blocking-queued invocation on object whose thread is the current thread is not allowed.");
				}
			}
			throw new QUnsuccessfulInvocationException("Blocking-queued invocation of method on not allowed without thread affinity.");
		case QueuedConnection:
			QObject invoker = new QObject() {
				@Override
				public boolean event(QEvent event) {
					if(event.type()==QEvent.Type.DeferredDispose
							&& (info==null || info.qobject==null || !info.qobject.isDisposed()) 
							&& !(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
						try {
							method.invoke(arg1, arg2, arg3, arg4, arg5);
						} catch (Throwable e) {
							Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invokation.", e);
						}
					}
					return super.event(event);
				}
			};
			QtJambiInternal.setCppOwnership(invoker);
			if(thread!=null)
				invoker.moveToThread(thread);
			invoker.disposeLater();
			return;
		default:
			break;
		}
		throw new QUnsuccessfulInvocationException("Unable to invoke method.");
	}
	
	public static <A,B,C,D,E,F> void invokeMethod(Slot6<A,B,C,D,E,F> method, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
		invokeMethod(method, Qt.ConnectionType.AutoConnection, arg1, arg2, arg3, arg4, arg5, arg6);
	}
	
	public static <A,B,C,D,E,F> void invokeMethod(Slot6<A,B,C,D,E,F> method, Qt.ConnectionType type, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
		QtJambiInternal.LambdaInfo info = QtJambiInternal.lamdaInfo(method);
		QThread thread = null;
		if(info!=null && info.qobject!=null) {
			if(info.reflectiveMethod!=null || info.signal!=null) {
				QMetaMethod qmethod = info.reflectiveMethod!=null ? fromReflectedMethod(info.reflectiveMethod) : info.qobject.metaObject().methodByIndex(info.qobject.metaObject().metaObjectPointer, info.signal.methodIndex());
				if(qmethod!=null && qmethod.parameterTypes().size()==info.lambdaArgs.size()+6) {
					if(info.lambdaArgs.isEmpty()) {
						qmethod.invoke(info.qobject, type, arg1, arg2, arg3, arg4, arg5, arg6);
					}else {
						Object[] args = new Object[qmethod.parameterTypes().size()];
						args[0] = arg1;
						args[1] = arg2;
						args[2] = arg3;
						args[3] = arg4;
						args[4] = arg5;
						args[5] = arg6;
						System.arraycopy(info.lambdaArgs.toArray(), 0, args, 6, info.lambdaArgs.size());
						qmethod.invoke(info.qobject, type, args);
					}
					return;
				}
			}
			thread = info.qobject.thread();
		}else {
			if(method instanceof QtObjectInterface) {
				if( ((QtObjectInterface)method).isDisposed() )
					return;
			}
			if(method instanceof QtThreadAffineInterface)
				thread = ((QtThreadAffineInterface) method).thread();
		}
		if(type==Qt.ConnectionType.AutoConnection && thread!=null && thread!=QThread.currentThread()) {
			type = Qt.ConnectionType.QueuedConnection;
		}
		switch(type) {
		case AutoConnection:
		case DirectConnection:
			try {
				method.invoke(arg1, arg2, arg3, arg4, arg5, arg6);
				return;
			} catch (Throwable e) {
				throw new QUnsuccessfulInvocationException(e);
			}
		case BlockingQueuedConnection:
			if(thread!=null) {
				if(thread!=QThread.currentThread()) {
					QObject invoker = new QObject() {
						@Override
						public boolean event(QEvent event) {
							if(event.type()==QEvent.Type.DeferredDispose
									&& (info==null || info.qobject==null || !info.qobject.isDisposed()) 
									&& !(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
								try {
									method.invoke(arg1, arg2, arg3, arg4, arg5, arg6);
								} catch (Throwable e) {
									Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invokation.", e);
								}
								synchronized (this) {
									notifyAll();
								}
							}
							return super.event(event);
						}
					};
					invoker.moveToThread(thread);
					invoker.disposeLater();
					synchronized (invoker) {
						try {
							invoker.wait();
						} catch (InterruptedException e) {
							throw new QUnsuccessfulInvocationException(e);
						}
					}
					return;
				}else {
					throw new QUnsuccessfulInvocationException("Blocking-queued invocation on object whose thread is the current thread is not allowed.");
				}
			}
			throw new QUnsuccessfulInvocationException("Blocking-queued invocation of method on not allowed without thread affinity.");
		case QueuedConnection:
			QObject invoker = new QObject() {
				@Override
				public boolean event(QEvent event) {
					if(event.type()==QEvent.Type.DeferredDispose
							&& (info==null || info.qobject==null || !info.qobject.isDisposed()) 
							&& !(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
						try {
							method.invoke(arg1, arg2, arg3, arg4, arg5, arg6);
						} catch (Throwable e) {
							Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invokation.", e);
						}
					}
					return super.event(event);
				}
			};
			QtJambiInternal.setCppOwnership(invoker);
			if(thread!=null)
				invoker.moveToThread(thread);
			invoker.disposeLater();
			return;
		default:
			break;
		}
		throw new QUnsuccessfulInvocationException("Unable to invoke method.");
	}
	
	public static <A,B,C,D,E,F,G> void invokeMethod(Slot7<A,B,C,D,E,F,G> method, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
		invokeMethod(method, Qt.ConnectionType.AutoConnection, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
	}
	
	public static <A,B,C,D,E,F,G> void invokeMethod(Slot7<A,B,C,D,E,F,G> method, Qt.ConnectionType type, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
		QtJambiInternal.LambdaInfo info = QtJambiInternal.lamdaInfo(method);
		QThread thread = null;
		if(info!=null && info.qobject!=null) {
			if(info.reflectiveMethod!=null || info.signal!=null) {
				QMetaMethod qmethod = info.reflectiveMethod!=null ? fromReflectedMethod(info.reflectiveMethod) : info.qobject.metaObject().methodByIndex(info.qobject.metaObject().metaObjectPointer, info.signal.methodIndex());
				if(qmethod!=null && qmethod.parameterTypes().size()==info.lambdaArgs.size()+7) {
					if(info.lambdaArgs.isEmpty()) {
						qmethod.invoke(info.qobject, type, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
					}else {
						Object[] args = new Object[qmethod.parameterTypes().size()];
						args[0] = arg1;
						args[1] = arg2;
						args[2] = arg3;
						args[3] = arg4;
						args[4] = arg5;
						args[5] = arg6;
						args[6] = arg7;
						System.arraycopy(info.lambdaArgs.toArray(), 0, args, 7, info.lambdaArgs.size());
						qmethod.invoke(info.qobject, type, args);
					}
					return;
				}
			}
			thread = info.qobject.thread();
		}else {
			if(method instanceof QtObjectInterface) {
				if( ((QtObjectInterface)method).isDisposed() )
					return;
			}
			if(method instanceof QtThreadAffineInterface)
				thread = ((QtThreadAffineInterface) method).thread();
		}
		if(type==Qt.ConnectionType.AutoConnection && thread!=null && thread!=QThread.currentThread()) {
			type = Qt.ConnectionType.QueuedConnection;
		}
		switch(type) {
		case AutoConnection:
		case DirectConnection:
			try {
				method.invoke(arg1, arg2, arg3, arg4, arg5, arg6, arg7);
				return;
			} catch (Throwable e) {
				throw new QUnsuccessfulInvocationException(e);
			}
		case BlockingQueuedConnection:
			if(thread!=null) {
				if(thread!=QThread.currentThread()) {
					QObject invoker = new QObject() {
						@Override
						public boolean event(QEvent event) {
							if(event.type()==QEvent.Type.DeferredDispose
									&& (info==null || info.qobject==null || !info.qobject.isDisposed()) 
									&& !(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
								try {
									method.invoke(arg1, arg2, arg3, arg4, arg5, arg6, arg7);
								} catch (Throwable e) {
									Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invokation.", e);
								}
								synchronized (this) {
									notifyAll();
								}
							}
							return super.event(event);
						}
					};
					invoker.moveToThread(thread);
					invoker.disposeLater();
					synchronized (invoker) {
						try {
							invoker.wait();
						} catch (InterruptedException e) {
							throw new QUnsuccessfulInvocationException(e);
						}
					}
					return;
				}else {
					throw new QUnsuccessfulInvocationException("Blocking-queued invocation on object whose thread is the current thread is not allowed.");
				}
			}
			throw new QUnsuccessfulInvocationException("Blocking-queued invocation of method on not allowed without thread affinity.");
		case QueuedConnection:
			QObject invoker = new QObject() {
				@Override
				public boolean event(QEvent event) {
					if(event.type()==QEvent.Type.DeferredDispose
							&& (info==null || info.qobject==null || !info.qobject.isDisposed()) 
							&& !(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
						try {
							method.invoke(arg1, arg2, arg3, arg4, arg5, arg6, arg7);
						} catch (Throwable e) {
							Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invokation.", e);
						}
					}
					return super.event(event);
				}
			};
			QtJambiInternal.setCppOwnership(invoker);
			if(thread!=null)
				invoker.moveToThread(thread);
			invoker.disposeLater();
			return;
		default:
			break;
		}
		throw new QUnsuccessfulInvocationException("Unable to invoke method.");
	}
	
	public static <A,B,C,D,E,F,G,H> void invokeMethod(Slot8<A,B,C,D,E,F,G,H> method, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7, H arg8) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
		invokeMethod(method, Qt.ConnectionType.AutoConnection, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
	}
	
	public static <A,B,C,D,E,F,G,H> void invokeMethod(Slot8<A,B,C,D,E,F,G,H> method, Qt.ConnectionType type, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7, H arg8) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
		QtJambiInternal.LambdaInfo info = QtJambiInternal.lamdaInfo(method);
		QThread thread = null;
		if(info!=null && info.qobject!=null && !info.qobject.isDisposed()) {
			if(info.reflectiveMethod!=null || info.signal!=null) {
				QMetaMethod qmethod = info.reflectiveMethod!=null ? fromReflectedMethod(info.reflectiveMethod) : info.qobject.metaObject().methodByIndex(info.qobject.metaObject().metaObjectPointer, info.signal.methodIndex());
				if(qmethod!=null && qmethod.parameterTypes().size()==info.lambdaArgs.size()+8) {
					if(info.lambdaArgs.isEmpty()) {
						qmethod.invoke(info.qobject, type, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
					}else {
						Object[] args = new Object[qmethod.parameterTypes().size()];
						args[0] = arg1;
						args[1] = arg2;
						args[2] = arg3;
						args[3] = arg4;
						args[4] = arg5;
						args[5] = arg6;
						args[6] = arg7;
						args[7] = arg8;
						System.arraycopy(info.lambdaArgs.toArray(), 0, args, 8, info.lambdaArgs.size());
						qmethod.invoke(info.qobject, type, args);
					}
					return;
				}
			}
			thread = info.qobject.thread();
		}else {
			if(method instanceof QtObjectInterface) {
				if( ((QtObjectInterface)method).isDisposed() )
					return;
			}
			if(method instanceof QtThreadAffineInterface)
				thread = ((QtThreadAffineInterface) method).thread();
		}
		if(type==Qt.ConnectionType.AutoConnection && thread!=null && thread!=QThread.currentThread()) {
			type = Qt.ConnectionType.QueuedConnection;
		}
		switch(type) {
		case AutoConnection:
		case DirectConnection:
			try {
				method.invoke(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
				return;
			} catch (Throwable e) {
				throw new QUnsuccessfulInvocationException(e);
			}
		case BlockingQueuedConnection:
			if(thread!=null) {
				if(thread!=QThread.currentThread()) {
					QObject invoker = new QObject() {
						@Override
						public boolean event(QEvent event) {
							if(event.type()==QEvent.Type.DeferredDispose
									&& (info==null || info.qobject==null || !info.qobject.isDisposed()) 
									&& !(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
								try {
									method.invoke(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
								} catch (Throwable e) {
									Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invokation.", e);
								}
								synchronized (this) {
									notifyAll();
								}
							}
							return super.event(event);
						}
					};
					invoker.moveToThread(thread);
					invoker.disposeLater();
					synchronized (invoker) {
						try {
							invoker.wait();
						} catch (InterruptedException e) {
							throw new QUnsuccessfulInvocationException(e);
						}
					}
					return;
				}else {
					throw new QUnsuccessfulInvocationException("Blocking-queued invocation on object whose thread is the current thread is not allowed.");
				}
			}
			throw new QUnsuccessfulInvocationException("Blocking-queued invocation of method on not allowed without thread affinity.");
		case QueuedConnection:
			QObject invoker = new QObject() {
				@Override
				public boolean event(QEvent event) {
					if(event.type()==QEvent.Type.DeferredDispose
							&& (info==null || info.qobject==null || !info.qobject.isDisposed()) 
							&& !(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
						try {
							method.invoke(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
						} catch (Throwable e) {
							Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invokation.", e);
						}
					}
					return super.event(event);
				}
			};
			QtJambiInternal.setCppOwnership(invoker);
			if(thread!=null)
				invoker.moveToThread(thread);
			invoker.disposeLater();
			return;
		default:
			break;
		}
		throw new QUnsuccessfulInvocationException("Unable to invoke method.");
	}
	
	public static <A,B,C,D,E,F,G,H,I> void invokeMethod(Slot9<A,B,C,D,E,F,G,H,I> method, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7, H arg8, I arg9) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
		invokeMethod(method, Qt.ConnectionType.AutoConnection, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
	}
	
	public static <A,B,C,D,E,F,G,H,I> void invokeMethod(Slot9<A,B,C,D,E,F,G,H,I> method, Qt.ConnectionType type, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7, H arg8, I arg9) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
		QtJambiInternal.LambdaInfo info = QtJambiInternal.lamdaInfo(method);
		QThread thread = null;
		if(info!=null && info.qobject!=null && !info.qobject.isDisposed()) {
			if(info.reflectiveMethod!=null || info.signal!=null) {
				QMetaMethod qmethod = info.reflectiveMethod!=null ? fromReflectedMethod(info.reflectiveMethod) : info.qobject.metaObject().methodByIndex(info.qobject.metaObject().metaObjectPointer, info.signal.methodIndex());
				if(qmethod!=null && qmethod.parameterTypes().size()==info.lambdaArgs.size()+9) {
					if(info.lambdaArgs.isEmpty()) {
						qmethod.invoke(info.qobject, type, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
					}else {
						Object[] args = new Object[qmethod.parameterTypes().size()];
						args[0] = arg1;
						args[1] = arg2;
						args[2] = arg3;
						args[3] = arg4;
						args[4] = arg5;
						args[5] = arg6;
						args[6] = arg7;
						args[7] = arg8;
						args[8] = arg9;
						System.arraycopy(info.lambdaArgs.toArray(), 0, args, 9, info.lambdaArgs.size());
						qmethod.invoke(info.qobject, type, args);
					}
					return;
				}
			}
			thread = info.qobject.thread();
		}else {
			if(method instanceof QtObjectInterface) {
				if( ((QtObjectInterface)method).isDisposed() )
					return;
			}
			if(method instanceof QtThreadAffineInterface)
				thread = ((QtThreadAffineInterface) method).thread();
		}
		if(type==Qt.ConnectionType.AutoConnection && thread!=null && thread!=QThread.currentThread()) {
			type = Qt.ConnectionType.QueuedConnection;
		}
		switch(type) {
		case AutoConnection:
		case DirectConnection:
			try {
				method.invoke(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
				return;
			} catch (Throwable e) {
				throw new QUnsuccessfulInvocationException(e);
			}
		case BlockingQueuedConnection:
			if(thread!=null) {
				if(thread!=QThread.currentThread()) {
					QObject invoker = new QObject() {
						@Override
						public boolean event(QEvent event) {
							if(event.type()==QEvent.Type.DeferredDispose
									&& (info==null || info.qobject==null || !info.qobject.isDisposed()) 
									&& !(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
								try {
									method.invoke(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
								} catch (Throwable e) {
									Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invokation.", e);
								}
								synchronized (this) {
									notifyAll();
								}
							}
							return super.event(event);
						}
					};
					invoker.moveToThread(thread);
					invoker.disposeLater();
					synchronized (invoker) {
						try {
							invoker.wait();
						} catch (InterruptedException e) {
							throw new QUnsuccessfulInvocationException(e);
						}
					}
					return;
				}else {
					throw new QUnsuccessfulInvocationException("Blocking-queued invocation on object whose thread is the current thread is not allowed.");
				}
			}
			throw new QUnsuccessfulInvocationException("Blocking-queued invocation of method on not allowed without thread affinity.");
		case QueuedConnection:
			QObject invoker = new QObject() {
				@Override
				public boolean event(QEvent event) {
					if(event.type()==QEvent.Type.DeferredDispose
							&& (info==null || info.qobject==null || !info.qobject.isDisposed()) 
							&& !(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
						try {
							method.invoke(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
						} catch (Throwable e) {
							Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invokation.", e);
						}
					}
					return super.event(event);
				}
			};
			QtJambiInternal.setCppOwnership(invoker);
			if(thread!=null)
				invoker.moveToThread(thread);
			invoker.disposeLater();
			return;
		default:
			break;
		}
		throw new QUnsuccessfulInvocationException("Unable to invoke method.");
	}
	
	public static void invokeMethod(AbstractPrivateSignal0 signal) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
		invokeMethod(signal, Qt.ConnectionType.AutoConnection);
	}
	
	public static void invokeMethod(AbstractPrivateSignal0 signal, Qt.ConnectionType type) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
		if(signal.containingObject() instanceof QObject && !((QObject)signal.containingObject()).isDisposed()) {
			QObject qobject = (QObject)signal.containingObject();
			QMetaMethod qmethod = qobject.metaObject().methodByIndex(qobject.metaObject().metaObjectPointer, signal.methodIndex());
			if(qmethod!=null) {
				qmethod.invoke(qobject, type);
				return;
			}
		}
		throw new QUnsuccessfulInvocationException("Unable to invoke method.");
	}
	
	public static <A> void invokeMethod(AbstractPrivateSignal1<A> signal, A arg1) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
		invokeMethod(signal, Qt.ConnectionType.AutoConnection, arg1);
	}
	
	public static <A> void invokeMethod(AbstractPrivateSignal1<A> signal, Qt.ConnectionType type, A arg1) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
		if(signal.containingObject() instanceof QObject && !((QObject)signal.containingObject()).isDisposed()) {
			QObject qobject = (QObject)signal.containingObject();
			QMetaMethod qmethod = qobject.metaObject().methodByIndex(qobject.metaObject().metaObjectPointer, signal.methodIndex());
			if(qmethod!=null) {
				qmethod.invoke(qobject, type, arg1);
				return;
			}
		}
		throw new QUnsuccessfulInvocationException("Unable to invoke method.");
	}
	
	public static <A,B> void invokeMethod(AbstractPrivateSignal2<A,B> signal, A arg1, B arg2) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
		invokeMethod(signal, Qt.ConnectionType.AutoConnection, arg1, arg2);
	}
	
	public static <A,B> void invokeMethod(AbstractPrivateSignal2<A,B> signal, Qt.ConnectionType type, A arg1, B arg2) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
		if(signal.containingObject() instanceof QObject && !((QObject)signal.containingObject()).isDisposed()) {
			QObject qobject = (QObject)signal.containingObject();
			QMetaMethod qmethod = qobject.metaObject().methodByIndex(qobject.metaObject().metaObjectPointer, signal.methodIndex());
			if(qmethod!=null) {
				qmethod.invoke(qobject, type, arg1, arg2);
				return;
			}
		}
		throw new QUnsuccessfulInvocationException("Unable to invoke method.");
	}
	
	public static <A,B,C> void invokeMethod(AbstractPrivateSignal3<A,B,C> signal, A arg1, B arg2, C arg3) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
		invokeMethod(signal, Qt.ConnectionType.AutoConnection, arg1, arg2, arg3);
	}
	
	public static <A,B,C> void invokeMethod(AbstractPrivateSignal3<A,B,C> signal, Qt.ConnectionType type, A arg1, B arg2, C arg3) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
		if(signal.containingObject() instanceof QObject && !((QObject)signal.containingObject()).isDisposed()) {
			QObject qobject = (QObject)signal.containingObject();
			QMetaMethod qmethod = qobject.metaObject().methodByIndex(qobject.metaObject().metaObjectPointer, signal.methodIndex());
			if(qmethod!=null) {
				qmethod.invoke(qobject, type, arg1, arg2, arg3);
				return;
			}
		}
		throw new QUnsuccessfulInvocationException("Unable to invoke method.");
	}
	
	public static <A,B,C,D> void invokeMethod(AbstractPrivateSignal4<A,B,C,D> signal, A arg1, B arg2, C arg3, D arg4) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
		invokeMethod(signal, Qt.ConnectionType.AutoConnection, arg1, arg2, arg3, arg4);
	}
	
	public static <A,B,C,D> void invokeMethod(AbstractPrivateSignal4<A,B,C,D> signal, Qt.ConnectionType type, A arg1, B arg2, C arg3, D arg4) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
		if(signal.containingObject() instanceof QObject && !((QObject)signal.containingObject()).isDisposed()) {
			QObject qobject = (QObject)signal.containingObject();
			QMetaMethod qmethod = qobject.metaObject().methodByIndex(qobject.metaObject().metaObjectPointer, signal.methodIndex());
			if(qmethod!=null) {
				qmethod.invoke(qobject, type, arg1, arg2, arg3, arg4);
				return;
			}
		}
		throw new QUnsuccessfulInvocationException("Unable to invoke method.");
	}
	
	public static <A,B,C,D,E> void invokeMethod(AbstractPrivateSignal5<A,B,C,D,E> signal, A arg1, B arg2, C arg3, D arg4, E arg5) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
		invokeMethod(signal, Qt.ConnectionType.AutoConnection, arg1, arg2, arg3, arg4, arg5);
	}
	
	public static <A,B,C,D,E> void invokeMethod(AbstractPrivateSignal5<A,B,C,D,E> signal, Qt.ConnectionType type, A arg1, B arg2, C arg3, D arg4, E arg5) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
		if(signal.containingObject() instanceof QObject && !((QObject)signal.containingObject()).isDisposed()) {
			QObject qobject = (QObject)signal.containingObject();
			QMetaMethod qmethod = qobject.metaObject().methodByIndex(qobject.metaObject().metaObjectPointer, signal.methodIndex());
			if(qmethod!=null) {
				qmethod.invoke(qobject, type, arg1, arg2, arg3, arg4, arg5);
				return;
			}
		}
		throw new QUnsuccessfulInvocationException("Unable to invoke method.");
	}
	
	public static <A,B,C,D,E,F> void invokeMethod(AbstractPrivateSignal6<A,B,C,D,E,F> signal, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
		invokeMethod(signal, Qt.ConnectionType.AutoConnection, arg1, arg2, arg3, arg4, arg5, arg6);
	}
	
	public static <A,B,C,D,E,F> void invokeMethod(AbstractPrivateSignal6<A,B,C,D,E,F> signal, Qt.ConnectionType type, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
		if(signal.containingObject() instanceof QObject && !((QObject)signal.containingObject()).isDisposed()) {
			QObject qobject = (QObject)signal.containingObject();
			QMetaMethod qmethod = qobject.metaObject().methodByIndex(qobject.metaObject().metaObjectPointer, signal.methodIndex());
			if(qmethod!=null) {
				qmethod.invoke(qobject, type, arg1, arg2, arg3, arg4, arg5, arg6);
				return;
			}
		}
		throw new QUnsuccessfulInvocationException("Unable to invoke method.");
	}
	
	public static <A,B,C,D,E,F,G> void invokeMethod(AbstractPrivateSignal7<A,B,C,D,E,F,G> signal, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
		invokeMethod(signal, Qt.ConnectionType.AutoConnection, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
	}
	
	public static <A,B,C,D,E,F,G> void invokeMethod(AbstractPrivateSignal7<A,B,C,D,E,F,G> signal, Qt.ConnectionType type, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
		if(signal.containingObject() instanceof QObject && !((QObject)signal.containingObject()).isDisposed()) {
			QObject qobject = (QObject)signal.containingObject();
			QMetaMethod qmethod = qobject.metaObject().methodByIndex(qobject.metaObject().metaObjectPointer, signal.methodIndex());
			if(qmethod!=null) {
				qmethod.invoke(qobject, type, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
				return;
			}
		}
		throw new QUnsuccessfulInvocationException("Unable to invoke method.");
	}
	
	public static <A,B,C,D,E,F,G,H> void invokeMethod(AbstractPrivateSignal8<A,B,C,D,E,F,G,H> signal, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7, H arg8) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
		invokeMethod(signal, Qt.ConnectionType.AutoConnection, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
	}
	
	public static <A,B,C,D,E,F,G,H> void invokeMethod(AbstractPrivateSignal8<A,B,C,D,E,F,G,H> signal, Qt.ConnectionType type, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7, H arg8) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
		if(signal.containingObject() instanceof QObject && !((QObject)signal.containingObject()).isDisposed()) {
			QObject qobject = (QObject)signal.containingObject();
			QMetaMethod qmethod = qobject.metaObject().methodByIndex(qobject.metaObject().metaObjectPointer, signal.methodIndex());
			if(qmethod!=null) {
				qmethod.invoke(qobject, type, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
				return;
			}
		}
		throw new QUnsuccessfulInvocationException("Unable to invoke method.");
	}
	
	public static <A,B,C,D,E,F,G,H,I> void invokeMethod(AbstractPrivateSignal9<A,B,C,D,E,F,G,H,I> signal, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7, H arg8, I arg9) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
		invokeMethod(signal, Qt.ConnectionType.AutoConnection, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
	}
	
	public static <A,B,C,D,E,F,G,H,I> void invokeMethod(AbstractPrivateSignal9<A,B,C,D,E,F,G,H,I> signal, Qt.ConnectionType type, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7, H arg8, I arg9) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
		if(signal.containingObject() instanceof QObject && !((QObject)signal.containingObject()).isDisposed()) {
			QObject qobject = (QObject)signal.containingObject();
			QMetaMethod qmethod = qobject.metaObject().methodByIndex(qobject.metaObject().metaObjectPointer, signal.methodIndex());
			if(qmethod!=null) {
				qmethod.invoke(qobject, type, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
				return;
			}
		}
		throw new QUnsuccessfulInvocationException("Unable to invoke method.");
	}

	@Override
	public int hashCode() {
		final int prime = 31;
		int result = 1;
		result = prime * result + (int) (metaObjectPointer ^ (metaObjectPointer >>> 32));
		return result;
	}

	@Override
	public boolean equals(Object obj) {
		if (this == obj)
			return true;
		if (obj == null)
			return false;
		if (getClass() != obj.getClass())
			return false;
		QMetaObject other = (QMetaObject) obj;
		if (metaObjectPointer != other.metaObjectPointer)
			return false;
		return true;
	}
	
	private final static class SignalAccess extends QtJambiSignals {
		public static abstract class AbstractSignal extends QtJambiSignals.AbstractSignal {
			
			AbstractSignal(){}
		
			AbstractSignal(Class<?>... types) {
				super(types);
			}
			
			AbstractSignal(Class<?> declaringClass, boolean isStatic) {
				super(declaringClass, isStatic);
			}
			
			AbstractSignal(String signalName, Class<?>... types) {
				super(signalName, types);
			}
		}
		
		static abstract class AbstractMultiSignal extends QtJambiSignals.MultiSignal {
			AbstractMultiSignal(SignalConfiguration signal1, SignalConfiguration signal2,
					SignalConfiguration[] furtherSignals) {
				super(signal1, signal2, furtherSignals);
			}
		}
		
		static abstract class SignalConfiguration extends QtJambiSignals.SignalConfiguration {
			SignalConfiguration(io.qt.core.QMetaObject.AbstractSignal signal, Class<?>[] types) {
				super(signal, types);
			}
		}
		
		static void checkConnectionToDisposedSignal(QMetaObject.DisposedSignal signal, Object receiver, boolean slotObject) {
			QtJambiSignals.checkConnectionToDisposedSignalImpl(signal, receiver, slotObject);
		}
		
		private static void initialize() {}
		static {
			QtJambiSignals.registerDisposedSignalFactory(DisposedSignal::new);
		}
	}
	
	static abstract class SignalConfiguration extends SignalAccess.SignalConfiguration {
		SignalConfiguration(io.qt.core.QMetaObject.AbstractSignal signal, Class<?>... types) {
			super(signal, types);
		}
	}
	
	static abstract class AbstractMultiSignal extends SignalAccess.AbstractMultiSignal {
		AbstractMultiSignal(SignalConfiguration signal1, SignalConfiguration signal2,
				SignalConfiguration[] furtherSignals) {
			super(signal1, signal2, furtherSignals);
		}
	}
	
	static {
		SignalAccess.initialize();
	}
	
	public static final class DisposedSignal extends AbstractSignal {
		
		private DisposedSignal(Class<?> declaringClass) {
			super(declaringClass, true);
		}
		
		@Override
		protected final void checkConnection(Object receiver, boolean slotObject) {
			SignalAccess.checkConnectionToDisposedSignal(this, receiver, slotObject);
        }
	
		public final QMetaObject.Connection connect(Slot0 slot, Qt.ConnectionType... type) {
	    	return addConnectionToSlotObject(slot, type);
	    }
	
		public final boolean disconnect(Slot0 slot) {
	    	return removeConnectionToSlotObject(slot);
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
	    public final QMetaObject.Connection connect(AbstractPublicSignal0 signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(QObject.Signal1Default1<?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(QObject.Signal2Default2<?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(QObject.Signal3Default3<?,?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(QObject.Signal4Default4<?,?,?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(QObject.Signal5Default5<?,?,?,?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(QObject.Signal6Default6<?,?,?,?,?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(QObject.Signal7Default7<?,?,?,?,?,?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(QObject.Signal8Default8<?,?,?,?,?,?,?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(QObject.Signal9Default9<?,?,?,?,?,?,?,?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(QStaticMemberSignals.Signal1Default1<?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(QStaticMemberSignals.Signal2Default2<?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(QStaticMemberSignals.Signal3Default3<?,?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(QStaticMemberSignals.Signal4Default4<?,?,?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(QStaticMemberSignals.Signal5Default5<?,?,?,?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(QStaticMemberSignals.Signal6Default6<?,?,?,?,?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(QStaticMemberSignals.Signal7Default7<?,?,?,?,?,?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(QStaticMemberSignals.Signal8Default8<?,?,?,?,?,?,?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(QStaticMemberSignals.Signal9Default9<?,?,?,?,?,?,?,?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(QInstanceMemberSignals.Signal1Default1<?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(QInstanceMemberSignals.Signal2Default2<?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(QInstanceMemberSignals.Signal3Default3<?,?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(QInstanceMemberSignals.Signal4Default4<?,?,?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(QInstanceMemberSignals.Signal5Default5<?,?,?,?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(QInstanceMemberSignals.Signal6Default6<?,?,?,?,?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(QInstanceMemberSignals.Signal7Default7<?,?,?,?,?,?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(QInstanceMemberSignals.Signal8Default8<?,?,?,?,?,?,?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(QInstanceMemberSignals.Signal9Default9<?,?,?,?,?,?,?,?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(QDeclarableSignals.Signal1Default1<?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(QDeclarableSignals.Signal2Default2<?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(QDeclarableSignals.Signal3Default3<?,?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(QDeclarableSignals.Signal4Default4<?,?,?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(QDeclarableSignals.Signal5Default5<?,?,?,?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(QDeclarableSignals.Signal6Default6<?,?,?,?,?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(QDeclarableSignals.Signal7Default7<?,?,?,?,?,?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(QDeclarableSignals.Signal8Default8<?,?,?,?,?,?,?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(QDeclarableSignals.Signal9Default9<?,?,?,?,?,?,?,?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	
	    /**
	     * Disconnects a signal from another signal if the two were previously connected by a call to connect.
	     * A call to this function will assure that the emission of the first signal will not cause the emission of the second.
	     *
	     * @param signalOut The second signal.
	     * @return true if the two signals were successfully disconnected, or false otherwise.
	     */
	    public final boolean disconnect(AbstractPublicSignal0 signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(QObject.Signal1Default1<?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(QObject.Signal2Default2<?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(QObject.Signal3Default3<?,?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(QObject.Signal4Default4<?,?,?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(QObject.Signal5Default5<?,?,?,?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(QObject.Signal6Default6<?,?,?,?,?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(QObject.Signal7Default7<?,?,?,?,?,?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(QObject.Signal8Default8<?,?,?,?,?,?,?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(QObject.Signal9Default9<?,?,?,?,?,?,?,?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(QStaticMemberSignals.Signal1Default1<?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(QStaticMemberSignals.Signal2Default2<?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(QStaticMemberSignals.Signal3Default3<?,?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(QStaticMemberSignals.Signal4Default4<?,?,?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(QStaticMemberSignals.Signal5Default5<?,?,?,?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(QStaticMemberSignals.Signal6Default6<?,?,?,?,?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(QStaticMemberSignals.Signal7Default7<?,?,?,?,?,?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(QStaticMemberSignals.Signal8Default8<?,?,?,?,?,?,?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(QStaticMemberSignals.Signal9Default9<?,?,?,?,?,?,?,?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(QInstanceMemberSignals.Signal1Default1<?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(QInstanceMemberSignals.Signal2Default2<?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(QInstanceMemberSignals.Signal3Default3<?,?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(QInstanceMemberSignals.Signal4Default4<?,?,?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(QInstanceMemberSignals.Signal5Default5<?,?,?,?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(QInstanceMemberSignals.Signal6Default6<?,?,?,?,?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(QInstanceMemberSignals.Signal7Default7<?,?,?,?,?,?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(QInstanceMemberSignals.Signal8Default8<?,?,?,?,?,?,?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(QInstanceMemberSignals.Signal9Default9<?,?,?,?,?,?,?,?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(QDeclarableSignals.Signal1Default1<?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(QDeclarableSignals.Signal2Default2<?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(QDeclarableSignals.Signal3Default3<?,?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(QDeclarableSignals.Signal4Default4<?,?,?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(QDeclarableSignals.Signal5Default5<?,?,?,?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(QDeclarableSignals.Signal6Default6<?,?,?,?,?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(QDeclarableSignals.Signal7Default7<?,?,?,?,?,?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(QDeclarableSignals.Signal8Default8<?,?,?,?,?,?,?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(QDeclarableSignals.Signal9Default9<?,?,?,?,?,?,?,?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	}
	
	public static abstract class AbstractSignal extends SignalAccess.AbstractSignal {
		
		AbstractSignal(){}
	
		AbstractSignal(Class<?>... types) {
			super(types);
		}
		
		AbstractSignal(Class<?> declaringClass, boolean isDisposedSignal) {
			super(declaringClass, isDisposedSignal);
		}
		
		AbstractSignal(String signalName, Class<?>... types) {
			super(signalName, types);
		}
	
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
	    public final QMetaObject.Connection connect(Object receiver, String method, Qt.ConnectionType... type) {
	        if (receiver == null)
	            throw new NullPointerException("Receiver must be non-null");
	        if (method == null)
	            throw new NullPointerException("Slot must be non-null");
	    	return addConnection(receiver, method, type);
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
	        return removeConnection(receiver, method);
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
			if(receiver instanceof QMetaObject.Connection){
				return disconnect((QMetaObject.Connection)receiver);
			}else{
				return disconnect(receiver, null);
			}
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
	     * Removes the given connection from this signal.
	     *
		 * @param connection the connection to be removed
	     * @return  True if the disconnection was successful.
	     */
	    public final boolean disconnect(QMetaObject.Connection connection) {
	        return removeConnection(connection);
	    }
	}
	
	public static interface AbstractSlot extends Serializable{
	}

	@FunctionalInterface
	public static interface GenericSlot extends AbstractSlot{
		public void invoke(Object... args) throws Throwable;
	}

	@FunctionalInterface
	public static interface Slot0 extends AbstractSlot{
		public void invoke() throws Throwable;
	}

	@FunctionalInterface
	public static interface Slot1<A> extends AbstractSlot{
		public void invoke(A a) throws Throwable;
	}

	@FunctionalInterface
	public static interface Slot2<A,B> extends AbstractSlot{
		public void invoke(A a, B b) throws Throwable;
	}
	
	@FunctionalInterface
	public static interface Slot3<A,B,C> extends AbstractSlot{
		public void invoke(A a, B b, C c) throws Throwable;
	}
	
	@FunctionalInterface
	public static interface Slot4<A,B,C,D> extends AbstractSlot{
		public void invoke(A a, B b, C c, D d) throws Throwable;
	}
	
	@FunctionalInterface
	public static interface Slot5<A,B,C,D,E> extends AbstractSlot{
		public void invoke(A a, B b, C c, D d, E e) throws Throwable;
	}
	
	@FunctionalInterface
	public static interface Slot6<A,B,C,D,E,F> extends AbstractSlot{
		public void invoke(A a, B b, C c, D d, E e, F f) throws Throwable;
	}
	
	@FunctionalInterface
	public static interface Slot7<A,B,C,D,E,F,G> extends AbstractSlot{
		public void invoke(A a, B b, C c, D d, E e, F f, G g) throws Throwable;
	}
	
	@FunctionalInterface
	public static interface Slot8<A,B,C,D,E,F,G,H> extends AbstractSlot{
		public void invoke(A a, B b, C c, D d, E e, F f, G g, H h) throws Throwable;
	}
	
	@FunctionalInterface
	public static interface Slot9<A,B,C,D,E,F,G,H,I> extends AbstractSlot{
		public void invoke(A a, B b, C c, D d, E e, F f, G g, H h, I i) throws Throwable;
	}
	
	@FunctionalInterface
	public static interface GenericMethod<R> extends AbstractSlot{
		public R invoke(Object... args) throws Throwable;
	}

	@FunctionalInterface
	public static interface Method0<R> extends AbstractSlot{
		public R invoke() throws Throwable;
	}

	@FunctionalInterface
	public static interface Method1<A,R> extends AbstractSlot{
		public R invoke(A a) throws Throwable;
	}

	@FunctionalInterface
	public static interface Method2<A,B,R> extends AbstractSlot{
		public R invoke(A a, B b) throws Throwable;
	}
	
	@FunctionalInterface
	public static interface Method3<A,B,C,R> extends AbstractSlot{
		public R invoke(A a, B b, C c) throws Throwable;
	}
	
	@FunctionalInterface
	public static interface Method4<A,B,C,D,R> extends AbstractSlot{
		public R invoke(A a, B b, C c, D d) throws Throwable;
	}
	
	@FunctionalInterface
	public static interface Method5<A,B,C,D,E,R> extends AbstractSlot{
		public R invoke(A a, B b, C c, D d, E e) throws Throwable;
	}
	
	@FunctionalInterface
	public static interface Method6<A,B,C,D,E,F,R> extends AbstractSlot{
		public R invoke(A a, B b, C c, D d, E e, F f) throws Throwable;
	}
	
	@FunctionalInterface
	public static interface Method7<A,B,C,D,E,F,G,R> extends AbstractSlot{
		public R invoke(A a, B b, C c, D d, E e, F f, G g) throws Throwable;
	}
	
	@FunctionalInterface
	public static interface Method8<A,B,C,D,E,F,G,H,R> extends AbstractSlot{
		public R invoke(A a, B b, C c, D d, E e, F f, G g, H h) throws Throwable;
	}
	
	@FunctionalInterface
	public static interface Method9<A,B,C,D,E,F,G,H,I,R> extends AbstractSlot{
		public R invoke(A a, B b, C c, D d, E e, F f, G g, H h, I i) throws Throwable;
	}

	public static abstract class AbstractPrivateSignal0 extends AbstractSignal {
	    
		AbstractPrivateSignal0(){super();}
		
		AbstractPrivateSignal0(Class<?> declaringClass, boolean isDisposedSignal) {
			super(declaringClass, isDisposedSignal);
		}
		
		AbstractPrivateSignal0(Class<?>... types) {
			super(types);
		}
		
		AbstractPrivateSignal0(String signalName, Class<?>... types) {
			super(signalName, types);
		}
	
		public final QMetaObject.Connection connect(Slot0 slot, Qt.ConnectionType... type) {
	    	return addConnectionToSlotObject(slot, type);
	    }
	
		public final boolean disconnect(Slot0 slot) {
	    	return removeConnectionToSlotObject(slot);
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
	    public final QMetaObject.Connection connect(AbstractPublicSignal0 signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(QObject.Signal1Default1<?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(QObject.Signal2Default2<?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(QObject.Signal3Default3<?,?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(QObject.Signal4Default4<?,?,?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(QObject.Signal5Default5<?,?,?,?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(QObject.Signal6Default6<?,?,?,?,?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(QObject.Signal7Default7<?,?,?,?,?,?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(QObject.Signal8Default8<?,?,?,?,?,?,?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(QObject.Signal9Default9<?,?,?,?,?,?,?,?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(QStaticMemberSignals.Signal1Default1<?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(QStaticMemberSignals.Signal2Default2<?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(QStaticMemberSignals.Signal3Default3<?,?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(QStaticMemberSignals.Signal4Default4<?,?,?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(QStaticMemberSignals.Signal5Default5<?,?,?,?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(QStaticMemberSignals.Signal6Default6<?,?,?,?,?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(QStaticMemberSignals.Signal7Default7<?,?,?,?,?,?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(QStaticMemberSignals.Signal8Default8<?,?,?,?,?,?,?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(QStaticMemberSignals.Signal9Default9<?,?,?,?,?,?,?,?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(QInstanceMemberSignals.Signal1Default1<?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(QInstanceMemberSignals.Signal2Default2<?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(QInstanceMemberSignals.Signal3Default3<?,?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(QInstanceMemberSignals.Signal4Default4<?,?,?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(QInstanceMemberSignals.Signal5Default5<?,?,?,?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(QInstanceMemberSignals.Signal6Default6<?,?,?,?,?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(QInstanceMemberSignals.Signal7Default7<?,?,?,?,?,?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(QInstanceMemberSignals.Signal8Default8<?,?,?,?,?,?,?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(QInstanceMemberSignals.Signal9Default9<?,?,?,?,?,?,?,?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(QDeclarableSignals.Signal1Default1<?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(QDeclarableSignals.Signal2Default2<?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(QDeclarableSignals.Signal3Default3<?,?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(QDeclarableSignals.Signal4Default4<?,?,?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(QDeclarableSignals.Signal5Default5<?,?,?,?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(QDeclarableSignals.Signal6Default6<?,?,?,?,?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(QDeclarableSignals.Signal7Default7<?,?,?,?,?,?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(QDeclarableSignals.Signal8Default8<?,?,?,?,?,?,?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(QDeclarableSignals.Signal9Default9<?,?,?,?,?,?,?,?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	
	    /**
	     * Disconnects a signal from another signal if the two were previously connected by a call to connect.
	     * A call to this function will assure that the emission of the first signal will not cause the emission of the second.
	     *
	     * @param signalOut The second signal.
	     * @return true if the two signals were successfully disconnected, or false otherwise.
	     */
	    public final boolean disconnect(AbstractPublicSignal0 signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(QObject.Signal1Default1<?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(QObject.Signal2Default2<?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(QObject.Signal3Default3<?,?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(QObject.Signal4Default4<?,?,?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(QObject.Signal5Default5<?,?,?,?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(QObject.Signal6Default6<?,?,?,?,?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(QObject.Signal7Default7<?,?,?,?,?,?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(QObject.Signal8Default8<?,?,?,?,?,?,?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(QObject.Signal9Default9<?,?,?,?,?,?,?,?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(QStaticMemberSignals.Signal1Default1<?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(QStaticMemberSignals.Signal2Default2<?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(QStaticMemberSignals.Signal3Default3<?,?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(QStaticMemberSignals.Signal4Default4<?,?,?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(QStaticMemberSignals.Signal5Default5<?,?,?,?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(QStaticMemberSignals.Signal6Default6<?,?,?,?,?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(QStaticMemberSignals.Signal7Default7<?,?,?,?,?,?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(QStaticMemberSignals.Signal8Default8<?,?,?,?,?,?,?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(QStaticMemberSignals.Signal9Default9<?,?,?,?,?,?,?,?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(QInstanceMemberSignals.Signal1Default1<?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(QInstanceMemberSignals.Signal2Default2<?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(QInstanceMemberSignals.Signal3Default3<?,?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(QInstanceMemberSignals.Signal4Default4<?,?,?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(QInstanceMemberSignals.Signal5Default5<?,?,?,?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(QInstanceMemberSignals.Signal6Default6<?,?,?,?,?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(QInstanceMemberSignals.Signal7Default7<?,?,?,?,?,?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(QInstanceMemberSignals.Signal8Default8<?,?,?,?,?,?,?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(QInstanceMemberSignals.Signal9Default9<?,?,?,?,?,?,?,?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(QDeclarableSignals.Signal1Default1<?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(QDeclarableSignals.Signal2Default2<?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(QDeclarableSignals.Signal3Default3<?,?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(QDeclarableSignals.Signal4Default4<?,?,?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(QDeclarableSignals.Signal5Default5<?,?,?,?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(QDeclarableSignals.Signal6Default6<?,?,?,?,?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(QDeclarableSignals.Signal7Default7<?,?,?,?,?,?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(QDeclarableSignals.Signal8Default8<?,?,?,?,?,?,?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(QDeclarableSignals.Signal9Default9<?,?,?,?,?,?,?,?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	}

	public static abstract class AbstractPrivateSignal1<A> extends AbstractSignal {
	    
		AbstractPrivateSignal1(){super();}
		
		AbstractPrivateSignal1(Class<?> declaringClass, boolean isDisposedSignal) {
			super(declaringClass, isDisposedSignal);
		}
		
		AbstractPrivateSignal1(Class<?>... types) {
			super(types);
		}
		
		AbstractPrivateSignal1(String signalName, Class<?>... types) {
			super(signalName, types);
		}
	
	    public final QMetaObject.Connection connect(Slot0 slot, Qt.ConnectionType... type) {
	    	return addConnectionToSlotObject(slot, type);
	    }
	
	    public final QMetaObject.Connection connect(Slot1<? super A> slot, Qt.ConnectionType... type) {
	    	return addConnectionToSlotObject(slot, type);
	    }
		
		public final boolean disconnect(Slot0 slot) {
	    	return removeConnectionToSlotObject(slot);
	    }
	    
	    public final boolean disconnect(Slot1<? super A> slot) {
	    	return removeConnectionToSlotObject(slot);
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
	    public final QMetaObject.Connection connect(AbstractPublicSignal0 signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(AbstractPublicSignal1<A> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(AbstractSignal2Default1<A,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(AbstractSignal3Default2<A,?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(AbstractSignal4Default3<A,?,?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(AbstractSignal5Default4<A,?,?,?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(AbstractSignal6Default5<A,?,?,?,?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(AbstractSignal7Default6<A,?,?,?,?,?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(AbstractSignal8Default7<A,?,?,?,?,?,?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(AbstractSignal9Default8<A,?,?,?,?,?,?,?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	
	    /**
	     * Disconnects a signal from another signal if the two were previously connected by a call to connect.
	     * A call to this function will assure that the emission of the first signal will not cause the emission of the second.
	     *
	     * @param signalOut The second signal.
	     * @return true if the two signals were successfully disconnected, or false otherwise.
	     */
	    public final boolean disconnect(AbstractPublicSignal0 signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(AbstractPublicSignal1<A> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(AbstractSignal2Default1<A,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(AbstractSignal3Default2<A,?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(AbstractSignal4Default3<A,?,?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(AbstractSignal5Default4<A,?,?,?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(AbstractSignal6Default5<A,?,?,?,?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(AbstractSignal7Default6<A,?,?,?,?,?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(AbstractSignal8Default7<A,?,?,?,?,?,?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(AbstractSignal9Default8<A,?,?,?,?,?,?,?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	}

	public static abstract class AbstractPrivateSignal2<A, B> extends AbstractSignal {
		AbstractPrivateSignal2(){super();}
	
		AbstractPrivateSignal2(Class<?> declaringClass, boolean isDisposedSignal) {
			super(declaringClass, isDisposedSignal);
		}
		
		AbstractPrivateSignal2(Class<?>... types) {
			super(types);
		}
		
		AbstractPrivateSignal2(String signalName, Class<?>... types) {
			super(signalName, types);
		}
	
	    public final QMetaObject.Connection connect(Slot0 slot, Qt.ConnectionType... type) {
	    	return addConnectionToSlotObject(slot, type);
	    }
	
	    public final QMetaObject.Connection connect(Slot1<? super A> slot, Qt.ConnectionType... type) {
	    	return addConnectionToSlotObject(slot, type);
	    }
	
	    public final QMetaObject.Connection connect(Slot2<? super A,? super B> slot, Qt.ConnectionType... type) {
	    	return addConnectionToSlotObject(slot, type);
	    }
		
		public final boolean disconnect(Slot0 slot) {
	    	return removeConnectionToSlotObject(slot);
	    }
	    
	    public final boolean disconnect(Slot1<? super A> slot) {
	    	return removeConnectionToSlotObject(slot);
	    }
		
	    public final boolean disconnect(Slot2<? super A,? super B> slot) {
	    	return removeConnectionToSlotObject(slot);
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
	    public final QMetaObject.Connection connect(AbstractPublicSignal0 signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(AbstractPublicSignal1<A> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(AbstractPublicSignal2<A,B> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(AbstractSignal3Default1<A,B,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(AbstractSignal4Default2<A,B,?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(AbstractSignal5Default3<A,B,?,?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(AbstractSignal6Default4<A,B,?,?,?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(AbstractSignal7Default5<A,B,?,?,?,?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(AbstractSignal8Default6<A,B,?,?,?,?,?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(AbstractSignal9Default7<A,B,?,?,?,?,?,?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	
	    /**
	     * Disconnects a signal from another signal if the two were previously connected by a call to connect.
	     * A call to this function will assure that the emission of the first signal will not cause the emission of the second.
	     *
	     * @param signalOut The second signal.
	     * @return true if the two signals were successfully disconnected, or false otherwise.
	     */
	    public final boolean disconnect(AbstractPublicSignal0 signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(AbstractPublicSignal1<A> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(AbstractPublicSignal2<A,B> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(AbstractSignal3Default1<A,B,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(AbstractSignal4Default2<A,B,?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(AbstractSignal5Default3<A,B,?,?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(AbstractSignal6Default4<A,B,?,?,?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(AbstractSignal7Default5<A,B,?,?,?,?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(AbstractSignal8Default6<A,B,?,?,?,?,?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(AbstractSignal9Default7<A,B,?,?,?,?,?,?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	}

	public static abstract class AbstractPrivateSignal3<A, B, C> extends AbstractSignal {
	    
		AbstractPrivateSignal3(){super();}
		
		AbstractPrivateSignal3(Class<?> declaringClass, boolean isDisposedSignal) {
			super(declaringClass, isDisposedSignal);
		}
		
		AbstractPrivateSignal3(Class<?>... types) {
			super(types);
		}
		
		AbstractPrivateSignal3(String signalName, Class<?>... types) {
			super(signalName, types);
		}
	
	    public final QMetaObject.Connection connect(Slot0 slot, Qt.ConnectionType... type) {
	    	return addConnectionToSlotObject(slot, type);
	    }
	
	    public final QMetaObject.Connection connect(Slot1<? super A> slot, Qt.ConnectionType... type) {
	    	return addConnectionToSlotObject(slot, type);
	    }
	
	    public final QMetaObject.Connection connect(Slot2<? super A,? super B> slot, Qt.ConnectionType... type) {
	    	return addConnectionToSlotObject(slot, type);
	    }
	
	    public final QMetaObject.Connection connect(Slot3<? super A,? super B,? super C> slot, Qt.ConnectionType... type) {
	    	return addConnectionToSlotObject(slot, type);
	    }
		
		public final boolean disconnect(Slot0 slot) {
	    	return removeConnectionToSlotObject(slot);
	    }
	    
	    public final boolean disconnect(Slot1<? super A> slot) {
	    	return removeConnectionToSlotObject(slot);
	    }
		
	    public final boolean disconnect(Slot2<? super A,? super B> slot) {
	    	return removeConnectionToSlotObject(slot);
	    }
	    
	    public final boolean disconnect(Slot3<? super A,? super B,? super C> slot) {
	    	return removeConnectionToSlotObject(slot);
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
	    public final QMetaObject.Connection connect(AbstractPublicSignal0 signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(AbstractPublicSignal1<A> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(AbstractPublicSignal2<A,B> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(AbstractPublicSignal3<A,B,C> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(AbstractSignal4Default1<A,B,C,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(AbstractSignal5Default2<A,B,C,?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(AbstractSignal6Default3<A,B,C,?,?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(AbstractSignal7Default4<A,B,C,?,?,?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(AbstractSignal8Default5<A,B,C,?,?,?,?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(AbstractSignal9Default6<A,B,C,?,?,?,?,?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	
	    /**
	     * Disconnects a signal from another signal if the two were previously connected by a call to connect.
	     * A call to this function will assure that the emission of the first signal will not cause the emission of the second.
	     *
	     * @param signalOut The second signal.
	     * @return true if the two signals were successfully disconnected, or false otherwise.
	     */
	    public final boolean disconnect(AbstractPublicSignal0 signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(AbstractPublicSignal1<A> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(AbstractPublicSignal2<A,B> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(AbstractPublicSignal3<A,B,C> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(AbstractSignal4Default1<A,B,C,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(AbstractSignal5Default2<A,B,C,?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(AbstractSignal6Default3<A,B,C,?,?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(AbstractSignal7Default4<A,B,C,?,?,?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(AbstractSignal8Default5<A,B,C,?,?,?,?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(AbstractSignal9Default6<A,B,C,?,?,?,?,?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	}

	public static abstract class AbstractPrivateSignal4<A, B, C, D> extends AbstractSignal {
	    
		AbstractPrivateSignal4(){super();}
		
		AbstractPrivateSignal4(Class<?> declaringClass, boolean isDisposedSignal) {
			super(declaringClass, isDisposedSignal);
		}
		
		AbstractPrivateSignal4(Class<?>... types) {
			super(types);
		}
		
		AbstractPrivateSignal4(String signalName, Class<?>... types) {
			super(signalName, types);
		}
	
	    public final QMetaObject.Connection connect(Slot0 slot, Qt.ConnectionType... type) {
	    	return addConnectionToSlotObject(slot, type);
	    }
	
	    public final QMetaObject.Connection connect(Slot1<? super A> slot, Qt.ConnectionType... type) {
	    	return addConnectionToSlotObject(slot, type);
	    }
	
	    public final QMetaObject.Connection connect(Slot2<? super A,? super B> slot, Qt.ConnectionType... type) {
	    	return addConnectionToSlotObject(slot, type);
	    }
	
	    public final QMetaObject.Connection connect(Slot3<? super A,? super B,? super C> slot, Qt.ConnectionType... type) {
	    	return addConnectionToSlotObject(slot, type);
	    }
	
	    public final QMetaObject.Connection connect(Slot4<? super A,? super B,? super C,? super D> slot, Qt.ConnectionType... type) {
	    	return addConnectionToSlotObject(slot, type);
	    }
		
		public final boolean disconnect(Slot0 slot) {
	    	return removeConnectionToSlotObject(slot);
	    }
	    
	    public final boolean disconnect(Slot1<? super A> slot) {
	    	return removeConnectionToSlotObject(slot);
	    }
		
	    public final boolean disconnect(Slot2<? super A,? super B> slot) {
	    	return removeConnectionToSlotObject(slot);
	    }
	    
	    public final boolean disconnect(Slot3<? super A,? super B,? super C> slot) {
	    	return removeConnectionToSlotObject(slot);
	    }
	    
	    public final boolean disconnect(Slot4<? super A,? super B,? super C,? super D> slot) {
	    	return removeConnectionToSlotObject(slot);
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
	    public final QMetaObject.Connection connect(AbstractPublicSignal0 signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(AbstractPublicSignal1<A> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(AbstractPublicSignal2<A,B> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(AbstractPublicSignal3<A,B,C> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(AbstractPublicSignal4<A,B,C,D> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(AbstractSignal5Default1<A,B,C,D,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(AbstractSignal6Default2<A,B,C,D,?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(AbstractSignal7Default3<A,B,C,D,?,?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(AbstractSignal8Default4<A,B,C,D,?,?,?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(AbstractSignal9Default5<A,B,C,D,?,?,?,?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	
	    /**
	     * Disconnects a signal from another signal if the two were previously connected by a call to connect.
	     * A call to this function will assure that the emission of the first signal will not cause the emission of the second.
	     *
	     * @param signalOut The second signal.
	     * @return true if the two signals were successfully disconnected, or false otherwise.
	     */
	    public final boolean disconnect(AbstractPublicSignal0 signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(AbstractPublicSignal1<A> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(AbstractPublicSignal2<A,B> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(AbstractPublicSignal3<A,B,C> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(AbstractPublicSignal4<A,B,C,D> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(AbstractSignal5Default1<A,B,C,D,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(AbstractSignal6Default2<A,B,C,D,?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(AbstractSignal7Default3<A,B,C,D,?,?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(AbstractSignal8Default4<A,B,C,D,?,?,?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(AbstractSignal9Default5<A,B,C,D,?,?,?,?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	}

	public static abstract class AbstractPrivateSignal5<A, B, C, D, E> extends AbstractSignal {
	    
		AbstractPrivateSignal5(){super();}
		
		AbstractPrivateSignal5(Class<?> declaringClass, boolean isDisposedSignal) {
			super(declaringClass, isDisposedSignal);
		}
		
		AbstractPrivateSignal5(Class<?>... types) {
			super(types);
		}
		
		AbstractPrivateSignal5(String signalName, Class<?>... types) {
			super(signalName, types);
		}
	
	    public final QMetaObject.Connection connect(Slot0 slot, Qt.ConnectionType... type) {
	    	return addConnectionToSlotObject(slot, type);
	    }
	
	    public final QMetaObject.Connection connect(Slot1<? super A> slot, Qt.ConnectionType... type) {
	    	return addConnectionToSlotObject(slot, type);
	    }
	
	    public final QMetaObject.Connection connect(Slot2<? super A,? super B> slot, Qt.ConnectionType... type) {
	    	return addConnectionToSlotObject(slot, type);
	    }
	
	    public final QMetaObject.Connection connect(Slot3<? super A,? super B,? super C> slot, Qt.ConnectionType... type) {
	    	return addConnectionToSlotObject(slot, type);
	    }
	
	    public final QMetaObject.Connection connect(Slot4<? super A,? super B,? super C,? super D> slot, Qt.ConnectionType... type) {
	    	return addConnectionToSlotObject(slot, type);
	    }
	
	    public final QMetaObject.Connection connect(Slot5<? super A,? super B,? super C,? super D,? super E> slot, Qt.ConnectionType... type) {
	    	return addConnectionToSlotObject(slot, type);
	    }
		
		public final boolean disconnect(Slot0 slot) {
	    	return removeConnectionToSlotObject(slot);
	    }
	    
	    public final boolean disconnect(Slot1<? super A> slot) {
	    	return removeConnectionToSlotObject(slot);
	    }
		
	    public final boolean disconnect(Slot2<? super A,? super B> slot) {
	    	return removeConnectionToSlotObject(slot);
	    }
	    
	    public final boolean disconnect(Slot3<? super A,? super B,? super C> slot) {
	    	return removeConnectionToSlotObject(slot);
	    }
	    
	    public final boolean disconnect(Slot4<? super A,? super B,? super C,? super D> slot) {
	    	return removeConnectionToSlotObject(slot);
	    }
	    
	    public final boolean disconnect(Slot5<? super A,? super B,? super C,? super D,? super E> slot) {
	    	return removeConnectionToSlotObject(slot);
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
	    public final QMetaObject.Connection connect(AbstractPublicSignal0 signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(AbstractPublicSignal1<A> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(AbstractPublicSignal2<A,B> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(AbstractPublicSignal3<A,B,C> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(AbstractPublicSignal4<A,B,C,D> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(AbstractPublicSignal5<A,B,C,D,E> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(AbstractSignal6Default1<A,B,C,D,E,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(AbstractSignal7Default2<A,B,C,D,E,?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(AbstractSignal8Default3<A,B,C,D,E,?,?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(AbstractSignal9Default4<A,B,C,D,E,?,?,?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	
	    /**
	     * Disconnects a signal from another signal if the two were previously connected by a call to connect.
	     * A call to this function will assure that the emission of the first signal will not cause the emission of the second.
	     *
	     * @param signalOut The second signal.
	     * @return true if the two signals were successfully disconnected, or false otherwise.
	     */
	    public final boolean disconnect(AbstractPublicSignal0 signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(AbstractPublicSignal1<A> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(AbstractPublicSignal2<A,B> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(AbstractPublicSignal3<A,B,C> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(AbstractPublicSignal4<A,B,C,D> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(AbstractPublicSignal5<A,B,C,D,E> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(AbstractSignal6Default1<A,B,C,D,E,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(AbstractSignal7Default2<A,B,C,D,E,?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(AbstractSignal8Default3<A,B,C,D,E,?,?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(AbstractSignal9Default4<A,B,C,D,E,?,?,?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	}

	public static abstract class AbstractPrivateSignal6<A, B, C, D, E, F> extends AbstractSignal {
	    
		AbstractPrivateSignal6(){super();}
		
		AbstractPrivateSignal6(Class<?> declaringClass, boolean isDisposedSignal) {
			super(declaringClass, isDisposedSignal);
		}
		
		AbstractPrivateSignal6(Class<?>... types) {
			super(types);
		}
		
		AbstractPrivateSignal6(String signalName, Class<?>... types) {
			super(signalName, types);
		}
	
	    public final QMetaObject.Connection connect(Slot0 slot, Qt.ConnectionType... type) {
	    	return addConnectionToSlotObject(slot, type);
	    }
	
	    public final QMetaObject.Connection connect(Slot1<? super A> slot, Qt.ConnectionType... type) {
	    	return addConnectionToSlotObject(slot, type);
	    }
	
	    public final QMetaObject.Connection connect(Slot2<? super A,? super B> slot, Qt.ConnectionType... type) {
	    	return addConnectionToSlotObject(slot, type);
	    }
	
	    public final QMetaObject.Connection connect(Slot3<? super A,? super B,? super C> slot, Qt.ConnectionType... type) {
	    	return addConnectionToSlotObject(slot, type);
	    }
	
	    public final QMetaObject.Connection connect(Slot4<? super A,? super B,? super C,? super D> slot, Qt.ConnectionType... type) {
	    	return addConnectionToSlotObject(slot, type);
	    }
	
	    public final QMetaObject.Connection connect(Slot5<? super A,? super B,? super C,? super D,? super E> slot, Qt.ConnectionType... type) {
	    	return addConnectionToSlotObject(slot, type);
	    }
	
	    public final QMetaObject.Connection connect(Slot6<? super A,? super B,? super C,? super D,? super E,? super F> slot, Qt.ConnectionType... type) {
	    	return addConnectionToSlotObject(slot, type);
	    }
		
		public final boolean disconnect(Slot0 slot) {
	    	return removeConnectionToSlotObject(slot);
	    }
	    
	    public final boolean disconnect(Slot1<? super A> slot) {
	    	return removeConnectionToSlotObject(slot);
	    }
		
	    public final boolean disconnect(Slot2<? super A,? super B> slot) {
	    	return removeConnectionToSlotObject(slot);
	    }
	    
	    public final boolean disconnect(Slot3<? super A,? super B,? super C> slot) {
	    	return removeConnectionToSlotObject(slot);
	    }
	    
	    public final boolean disconnect(Slot4<? super A,? super B,? super C,? super D> slot) {
	    	return removeConnectionToSlotObject(slot);
	    }
	    
	    public final boolean disconnect(Slot5<? super A,? super B,? super C,? super D,? super E> slot) {
	    	return removeConnectionToSlotObject(slot);
	    }
	    
	    public final boolean disconnect(Slot6<? super A,? super B,? super C,? super D,? super E,? super F> slot) {
	    	return removeConnectionToSlotObject(slot);
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
	    public final QMetaObject.Connection connect(AbstractPublicSignal0 signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(AbstractPublicSignal1<A> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(AbstractPublicSignal2<A,B> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(AbstractPublicSignal3<A,B,C> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(AbstractPublicSignal4<A,B,C,D> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(AbstractPublicSignal5<A,B,C,D,E> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(AbstractPublicSignal6<A,B,C,D,E,F> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(AbstractSignal7Default1<A,B,C,D,E,F,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(AbstractSignal8Default2<A,B,C,D,E,F,?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(AbstractSignal9Default3<A,B,C,D,E,F,?,?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	
	    /**
	     * Disconnects a signal from another signal if the two were previously connected by a call to connect.
	     * A call to this function will assure that the emission of the first signal will not cause the emission of the second.
	     *
	     * @param signalOut The second signal.
	     * @return true if the two signals were successfully disconnected, or false otherwise.
	     */
	    public final boolean disconnect(AbstractPublicSignal0 signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(AbstractPublicSignal1<A> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(AbstractPublicSignal2<A,B> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(AbstractPublicSignal3<A,B,C> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(AbstractPublicSignal4<A,B,C,D> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(AbstractPublicSignal5<A,B,C,D,E> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(AbstractPublicSignal6<A,B,C,D,E,F> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	
	    public final boolean disconnect(AbstractSignal7Default1<A,B,C,D,E,F,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	
	    public final boolean disconnect(AbstractSignal8Default2<A,B,C,D,E,F,?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	
	    public final boolean disconnect(AbstractSignal9Default3<A,B,C,D,E,F,?,?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	}

	public static abstract class AbstractPrivateSignal7<A, B, C, D, E, F, G> extends AbstractSignal {
	    
		AbstractPrivateSignal7(){super();}
		
		AbstractPrivateSignal7(Class<?> declaringClass, boolean isDisposedSignal) {
			super(declaringClass, isDisposedSignal);
		}
		
		AbstractPrivateSignal7(Class<?>... types) {
			super(types);
		}
		
		AbstractPrivateSignal7(String signalName, Class<?>... types) {
			super(signalName, types);
		}
	
	    public final QMetaObject.Connection connect(Slot0 slot, Qt.ConnectionType... type) {
	    	return addConnectionToSlotObject(slot, type);
	    }
	
	    public final QMetaObject.Connection connect(Slot1<? super A> slot, Qt.ConnectionType... type) {
	    	return addConnectionToSlotObject(slot, type);
	    }
	
	    public final QMetaObject.Connection connect(Slot2<? super A,? super B> slot, Qt.ConnectionType... type) {
	    	return addConnectionToSlotObject(slot, type);
	    }
	
	    public final QMetaObject.Connection connect(Slot3<? super A,? super B,? super C> slot, Qt.ConnectionType... type) {
	    	return addConnectionToSlotObject(slot, type);
	    }
	
	    public final QMetaObject.Connection connect(Slot4<? super A,? super B,? super C,? super D> slot, Qt.ConnectionType... type) {
	    	return addConnectionToSlotObject(slot, type);
	    }
	
	    public final QMetaObject.Connection connect(Slot5<? super A,? super B,? super C,? super D,? super E> slot, Qt.ConnectionType... type) {
	    	return addConnectionToSlotObject(slot, type);
	    }
	
	    public final QMetaObject.Connection connect(Slot6<? super A,? super B,? super C,? super D,? super E,? super F> slot, Qt.ConnectionType... type) {
	    	return addConnectionToSlotObject(slot, type);
	    }
	
	    public final QMetaObject.Connection connect(Slot7<? super A,? super B,? super C,? super D,? super E,? super F,? super G> slot, Qt.ConnectionType... type) {
	    	return addConnectionToSlotObject(slot, type);
	    }
		
		public final boolean disconnect(Slot0 slot) {
	    	return removeConnectionToSlotObject(slot);
	    }
	    
	    public final boolean disconnect(Slot1<? super A> slot) {
	    	return removeConnectionToSlotObject(slot);
	    }
		
	    public final boolean disconnect(Slot2<? super A,? super B> slot) {
	    	return removeConnectionToSlotObject(slot);
	    }
	    
	    public final boolean disconnect(Slot3<? super A,? super B,? super C> slot) {
	    	return removeConnectionToSlotObject(slot);
	    }
	    
	    public final boolean disconnect(Slot4<? super A,? super B,? super C,? super D> slot) {
	    	return removeConnectionToSlotObject(slot);
	    }
	    
	    public final boolean disconnect(Slot5<? super A,? super B,? super C,? super D,? super E> slot) {
	    	return removeConnectionToSlotObject(slot);
	    }
	    
	    public final boolean disconnect(Slot6<? super A,? super B,? super C,? super D,? super E,? super F> slot) {
	    	return removeConnectionToSlotObject(slot);
	    }
		
	    public final boolean disconnect(Slot7<? super A,? super B,? super C,? super D,? super E,? super F,? super G> slot) {
	    	return removeConnectionToSlotObject(slot);
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
	    public final QMetaObject.Connection connect(AbstractPublicSignal0 signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(AbstractPublicSignal1<A> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(AbstractPublicSignal2<A,B> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(AbstractPublicSignal3<A,B,C> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(AbstractPublicSignal4<A,B,C,D> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(AbstractPublicSignal5<A,B,C,D,E> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(AbstractPublicSignal6<A,B,C,D,E,F> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(AbstractPublicSignal7<A,B,C,D,E,F,G> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(AbstractSignal8Default1<A,B,C,D,E,F,G,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(AbstractSignal9Default2<A,B,C,D,E,F,G,?,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	
	    /**
	     * Disconnects a signal from another signal if the two were previously connected by a call to connect.
	     * A call to this function will assure that the emission of the first signal will not cause the emission of the second.
	     *
	     * @param signalOut The second signal.
	     * @return true if the two signals were successfully disconnected, or false otherwise.
	     */
	    public final boolean disconnect(AbstractPublicSignal0 signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(AbstractPublicSignal1<A> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(AbstractPublicSignal2<A,B> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(AbstractPublicSignal3<A,B,C> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(AbstractPublicSignal4<A,B,C,D> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(AbstractPublicSignal5<A,B,C,D,E> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(AbstractPublicSignal6<A,B,C,D,E,F> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(AbstractPublicSignal7<A,B,C,D,E,F,G> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(AbstractSignal8Default1<A,B,C,D,E,F,G,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(AbstractSignal9Default2<A,B,C,D,E,F,G,?,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	}

	public static abstract class AbstractPrivateSignal8<A, B, C, D, E, F, G, H> extends AbstractSignal {
		AbstractPrivateSignal8(){super();}
		
		AbstractPrivateSignal8(Class<?> declaringClass, boolean isDisposedSignal) {
			super(declaringClass, isDisposedSignal);
		}
		
		AbstractPrivateSignal8(Class<?>... types) {
			super(types);
		}
		
		AbstractPrivateSignal8(String signalName, Class<?>... types) {
			super(signalName, types);
		}
	
	    public final QMetaObject.Connection connect(Slot0 slot, Qt.ConnectionType... type) {
	    	return addConnectionToSlotObject(slot, type);
	    }
	
	    public final QMetaObject.Connection connect(Slot1<? super A> slot, Qt.ConnectionType... type) {
	    	return addConnectionToSlotObject(slot, type);
	    }
	
	    public final QMetaObject.Connection connect(Slot2<? super A,? super B> slot, Qt.ConnectionType... type) {
	    	return addConnectionToSlotObject(slot, type);
	    }
	
	    public final QMetaObject.Connection connect(Slot3<? super A,? super B,? super C> slot, Qt.ConnectionType... type) {
	    	return addConnectionToSlotObject(slot, type);
	    }
	
	    public final QMetaObject.Connection connect(Slot4<? super A,? super B,? super C,? super D> slot, Qt.ConnectionType... type) {
	    	return addConnectionToSlotObject(slot, type);
	    }
	
	    public final QMetaObject.Connection connect(Slot5<? super A,? super B,? super C,? super D,? super E> slot, Qt.ConnectionType... type) {
	    	return addConnectionToSlotObject(slot, type);
	    }
	
	    public final QMetaObject.Connection connect(Slot6<? super A,? super B,? super C,? super D,? super E,? super F> slot, Qt.ConnectionType... type) {
	    	return addConnectionToSlotObject(slot, type);
	    }
	
	    public final QMetaObject.Connection connect(Slot7<? super A,? super B,? super C,? super D,? super E,? super F,? super G> slot, Qt.ConnectionType... type) {
	    	return addConnectionToSlotObject(slot, type);
	    }
	
	    public final QMetaObject.Connection connect(Slot8<? super A,? super B,? super C,? super D,? super E,? super F,? super G,? super H> slot, Qt.ConnectionType... type) {
	    	return addConnectionToSlotObject(slot, type);
	    }
		
		public final boolean disconnect(Slot0 slot) {
	    	return removeConnectionToSlotObject(slot);
	    }
	    
	    public final boolean disconnect(Slot1<? super A> slot) {
	    	return removeConnectionToSlotObject(slot);
	    }
		
	    public final boolean disconnect(Slot2<? super A,? super B> slot) {
	    	return removeConnectionToSlotObject(slot);
	    }
	    
	    public final boolean disconnect(Slot3<? super A,? super B,? super C> slot) {
	    	return removeConnectionToSlotObject(slot);
	    }
	    
	    public final boolean disconnect(Slot4<? super A,? super B,? super C,? super D> slot) {
	    	return removeConnectionToSlotObject(slot);
	    }
	    
	    public final boolean disconnect(Slot5<? super A,? super B,? super C,? super D,? super E> slot) {
	    	return removeConnectionToSlotObject(slot);
	    }
	    
	    public final boolean disconnect(Slot6<? super A,? super B,? super C,? super D,? super E,? super F> slot) {
	    	return removeConnectionToSlotObject(slot);
	    }
		
	    public final boolean disconnect(Slot7<? super A,? super B,? super C,? super D,? super E,? super F,? super G> slot) {
	    	return removeConnectionToSlotObject(slot);
	    }
		
	    public final boolean disconnect(Slot8<? super A,? super B,? super C,? super D,? super E,? super F,? super G,? super H> slot) {
	    	return removeConnectionToSlotObject(slot);
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
	    public final QMetaObject.Connection connect(AbstractPublicSignal0 signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(AbstractPublicSignal1<A> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(AbstractPublicSignal2<A,B> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(AbstractPublicSignal3<A,B,C> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(AbstractPublicSignal4<A,B,C,D> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(AbstractPublicSignal5<A,B,C,D,E> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(AbstractPublicSignal6<A,B,C,D,E,F> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(AbstractPublicSignal7<A,B,C,D,E,F,G> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(AbstractPublicSignal8<A,B,C,D,E,F,G,H> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(AbstractSignal9Default1<A,B,C,D,E,F,G,H,?> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	
	    /**
	     * Disconnects a signal from another signal if the two were previously connected by a call to connect.
	     * A call to this function will assure that the emission of the first signal will not cause the emission of the second.
	     *
	     * @param signalOut The second signal.
	     * @return true if the two signals were successfully disconnected, or false otherwise.
	     */
	    public final boolean disconnect(AbstractPublicSignal0 signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(AbstractPublicSignal1<A> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(AbstractPublicSignal2<A,B> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(AbstractPublicSignal3<A,B,C> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(AbstractPublicSignal4<A,B,C,D> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(AbstractPublicSignal5<A,B,C,D,E> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(AbstractPublicSignal6<A,B,C,D,E,F> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(AbstractPublicSignal7<A,B,C,D,E,F,G> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(AbstractPublicSignal8<A,B,C,D,E,F,G,H> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(AbstractSignal9Default1<A,B,C,D,E,F,G,H,?> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	}

	public static abstract class AbstractPrivateSignal9<A, B, C, D, E, F, G, H, I> extends AbstractSignal {
		AbstractPrivateSignal9(){super();}
		
		AbstractPrivateSignal9(Class<?> declaringClass, boolean isDisposedSignal) {
			super(declaringClass, isDisposedSignal);
		}
		
		AbstractPrivateSignal9(Class<?>... types) {
			super(types);
		}
		
		AbstractPrivateSignal9(String signalName, Class<?>... types) {
			super(signalName, types);
		}
	
	    public final QMetaObject.Connection connect(Slot0 slot, Qt.ConnectionType... type) {
	    	return addConnectionToSlotObject(slot, type);
	    }
	
	    public final QMetaObject.Connection connect(Slot1<? super A> slot, Qt.ConnectionType... type) {
	    	return addConnectionToSlotObject(slot, type);
	    }
	
	    public final QMetaObject.Connection connect(Slot2<? super A,? super B> slot, Qt.ConnectionType... type) {
	    	return addConnectionToSlotObject(slot, type);
	    }
	
	    public final QMetaObject.Connection connect(Slot3<? super A,? super B,? super C> slot, Qt.ConnectionType... type) {
	    	return addConnectionToSlotObject(slot, type);
	    }
	
	    public final QMetaObject.Connection connect(Slot4<? super A,? super B,? super C,? super D> slot, Qt.ConnectionType... type) {
	    	return addConnectionToSlotObject(slot, type);
	    }
	
	    public final QMetaObject.Connection connect(Slot5<? super A,? super B,? super C,? super D,? super E> slot, Qt.ConnectionType... type) {
	    	return addConnectionToSlotObject(slot, type);
	    }
	
	    public final QMetaObject.Connection connect(Slot6<? super A,? super B,? super C,? super D,? super E,? super F> slot, Qt.ConnectionType... type) {
	    	return addConnectionToSlotObject(slot, type);
	    }
	
	    public final QMetaObject.Connection connect(Slot7<? super A,? super B,? super C,? super D,? super E,? super F,? super G> slot, Qt.ConnectionType... type) {
	    	return addConnectionToSlotObject(slot, type);
	    }
	
	    public final QMetaObject.Connection connect(Slot8<? super A,? super B,? super C,? super D,? super E,? super F,? super G,? super H> slot, Qt.ConnectionType... type) {
	    	return addConnectionToSlotObject(slot, type);
	    }
	
	    public final QMetaObject.Connection connect(Slot9<? super A,? super B,? super C,? super D,? super E,? super F,? super G,? super H,? super I> slot, Qt.ConnectionType... type) {
	    	return addConnectionToSlotObject(slot, type);
	    }
		
		public final boolean disconnect(Slot0 slot) {
	    	return removeConnectionToSlotObject(slot);
	    }
	    
	    public final boolean disconnect(Slot1<? super A> slot) {
	    	return removeConnectionToSlotObject(slot);
	    }
		
	    public final boolean disconnect(Slot2<? super A,? super B> slot) {
	    	return removeConnectionToSlotObject(slot);
	    }
	    
	    public final boolean disconnect(Slot3<? super A,? super B,? super C> slot) {
	    	return removeConnectionToSlotObject(slot);
	    }
	    
	    public final boolean disconnect(Slot4<? super A,? super B,? super C,? super D> slot) {
	    	return removeConnectionToSlotObject(slot);
	    }
	    
	    public final boolean disconnect(Slot5<? super A,? super B,? super C,? super D,? super E> slot) {
	    	return removeConnectionToSlotObject(slot);
	    }
	    
	    public final boolean disconnect(Slot6<? super A,? super B,? super C,? super D,? super E,? super F> slot) {
	    	return removeConnectionToSlotObject(slot);
	    }
		
	    public final boolean disconnect(Slot7<? super A,? super B,? super C,? super D,? super E,? super F,? super G> slot) {
	    	return removeConnectionToSlotObject(slot);
	    }
		
	    public final boolean disconnect(Slot8<? super A,? super B,? super C,? super D,? super E,? super F,? super G,? super H> slot) {
	    	return removeConnectionToSlotObject(slot);
	    }
	    
	    public final boolean disconnect(Slot9<? super A,? super B,? super C,? super D,? super E,? super F,? super G,? super H,? super I> slot) {
	    	return removeConnectionToSlotObject(slot);
	    }
	
	    /**
	     * Creates a connection from this signal to another. Whenever this signal is emitted, it will cause the second
	     * signal to be emitted as well.
	     *
	     * @param signalOut The second signal. This will be emitted whenever this signal is emitted.
	     * @param type      One of the connection types defined in the Qt interface.
		 * @param unique    Unique connection.
	     * @throws RuntimeException Raised if either of the signal objects could not be successfully be introspected or if their
	     *                                    signatures are incompatible.
	     */
	    public final QMetaObject.Connection connect(AbstractPublicSignal0 signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(AbstractPublicSignal1<A> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(AbstractPublicSignal2<A,B> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(AbstractPublicSignal3<A,B,C> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(AbstractPublicSignal4<A,B,C,D> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(AbstractPublicSignal5<A,B,C,D,E> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(AbstractPublicSignal6<A,B,C,D,E,F> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(AbstractPublicSignal7<A,B,C,D,E,F,G> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(AbstractPublicSignal8<A,B,C,D,E,F,G,H> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	    
	    public final QMetaObject.Connection connect(AbstractPublicSignal9<A,B,C,D,E,F,G,H,I> signalOut, Qt.ConnectionType... type) {
	        return connect(signalOut::emit, type);
	    }
	
	    /**
	     * Disconnects a signal from another signal if the two were previously connected by a call to connect.
	     * A call to this function will assure that the emission of the first signal will not cause the emission of the second.
	     *
	     * @param signalOut The second signal.
	     * @return true if the two signals were successfully disconnected, or false otherwise.
	     */
	    public final boolean disconnect(AbstractPublicSignal0 signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(AbstractPublicSignal1<A> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(AbstractPublicSignal2<A,B> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(AbstractPublicSignal3<A,B,C> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(AbstractPublicSignal4<A,B,C,D> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(AbstractPublicSignal5<A,B,C,D,E> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(AbstractPublicSignal6<A,B,C,D,E,F> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(AbstractPublicSignal7<A,B,C,D,E,F,G> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(AbstractPublicSignal8<A,B,C,D,E,F,G,H> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	    
	    public final boolean disconnect(AbstractPublicSignal9<A,B,C,D,E,F,G,H,I> signalOut) {
	        return disconnect(signalOut::emit);
	    }
	}

	public static abstract class AbstractPublicSignal0 extends AbstractPrivateSignal0 {
		AbstractPublicSignal0(){super();}
		
		AbstractPublicSignal0(Class<?> declaringClass, boolean isDisposedSignal) {
			super(declaringClass, isDisposedSignal);
		}
		
		AbstractPublicSignal0(Class<?>... types) {
			super(types);
		}
		
		AbstractPublicSignal0(String signalName, Class<?>... types) {
			super(signalName, types);
		}
		
		/**
	     * Emits the signal.
	     */
	    public final void emit() {
	        emitSignal();
	    }
	}

	public static abstract class AbstractPublicSignal1<A> extends AbstractPrivateSignal1<A> {
		AbstractPublicSignal1(){}
		
		AbstractPublicSignal1(Class<?> declaringClass, boolean isDisposedSignal) {
			super(declaringClass, isDisposedSignal);
		}
		
		AbstractPublicSignal1(Class<?>... types) {
			super(types);
		}
		
		AbstractPublicSignal1(String signalName, Class<?>... types) {
			super(signalName, types);
		}
		
		/**
	     * Emits the signal.
	     */
	    public final void emit(A arg1) {
	        emitSignal(arg1);
	    }
	}

	public static abstract class AbstractPublicSignal2<A,B> extends AbstractPrivateSignal2<A,B> {
		AbstractPublicSignal2(){super();}
		
		AbstractPublicSignal2(Class<?> declaringClass, boolean isDisposedSignal) {
			super(declaringClass, isDisposedSignal);
		}
		
		AbstractPublicSignal2(Class<?>... types) {
			super(types);
		}
		
		AbstractPublicSignal2(String signalName, Class<?>... types) {
			super(signalName, types);
		}
		
		/**
	     * Emits the signal.
	     */
	    public final void emit(A arg1, B arg2) {
	        emitSignal(arg1, arg2);
	    }
	}

	public static abstract class AbstractPublicSignal3<A,B,C> extends AbstractPrivateSignal3<A,B,C> {
		AbstractPublicSignal3(){super();}
		
		AbstractPublicSignal3(Class<?> declaringClass, boolean isDisposedSignal) {
			super(declaringClass, isDisposedSignal);
		}
		
		AbstractPublicSignal3(Class<?>... types) {
			super(types);
		}
		
		AbstractPublicSignal3(String signalName, Class<?>... types) {
			super(signalName, types);
		}
		
		/**
	     * Emits the signal.
	     */
	    public final void emit(A arg1, B arg2, C arg3) {
	        emitSignal(arg1, arg2, arg3);
	    }
	}

	public static abstract class AbstractPublicSignal4<A,B,C,D> extends AbstractPrivateSignal4<A,B,C,D> {
		AbstractPublicSignal4(){super();}
		
		AbstractPublicSignal4(Class<?> declaringClass, boolean isDisposedSignal) {
			super(declaringClass, isDisposedSignal);
		}
		
		AbstractPublicSignal4(Class<?>... types) {
			super(types);
		}
		
		AbstractPublicSignal4(String signalName, Class<?>... types) {
			super(signalName, types);
		}
		
		/**
	     * Emits the signal.
	     */
	    public final void emit(A arg1, B arg2, C arg3, D arg4) {
	        emitSignal(arg1, arg2, arg3, arg4);
	    }
	}

	public static abstract class AbstractPublicSignal5<A,B,C,D,E> extends AbstractPrivateSignal5<A,B,C,D,E> {
		AbstractPublicSignal5(){super();}
		
		AbstractPublicSignal5(Class<?> declaringClass, boolean isDisposedSignal) {
			super(declaringClass, isDisposedSignal);
		}
		
		AbstractPublicSignal5(Class<?>... types) {
			super(types);
		}
		
		AbstractPublicSignal5(String signalName, Class<?>... types) {
			super(signalName, types);
		}
		
		/**
	     * Emits the signal.
	     */
	    public final void emit(A arg1, B arg2, C arg3, D arg4, E arg5) {
	        emitSignal(arg1, arg2, arg3, arg4, arg5);
	    }
	}

	public static abstract class AbstractPublicSignal6<A,B,C,D,E,F> extends AbstractPrivateSignal6<A,B,C,D,E,F> {
		AbstractPublicSignal6(){super();}
		
		AbstractPublicSignal6(Class<?> declaringClass, boolean isDisposedSignal) {
			super(declaringClass, isDisposedSignal);
		}
		
		AbstractPublicSignal6(Class<?>... types) {
			super(types);
		}
		
		AbstractPublicSignal6(String signalName, Class<?>... types) {
			super(signalName, types);
		}
		
		/**
	     * Emits the signal.
	     */
	    public final void emit(A arg1, B arg2, C arg3, D arg4, E arg5, F arg6) {
	        emitSignal(arg1, arg2, arg3, arg4, arg5, arg6);
	    }
	}

	public static abstract class AbstractPublicSignal7<A,B,C,D,E,F,G> extends AbstractPrivateSignal7<A,B,C,D,E,F,G> {
		AbstractPublicSignal7(){super();}
		
		AbstractPublicSignal7(Class<?> declaringClass, boolean isDisposedSignal) {
			super(declaringClass, isDisposedSignal);
		}
		
		AbstractPublicSignal7(Class<?>... types) {
			super(types);
		}
		
		AbstractPublicSignal7(String signalName, Class<?>... types) {
			super(signalName, types);
		}
		
		/**
	     * Emits the signal.
	     */
	    public final void emit(A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7) {
	        emitSignal(arg1, arg2, arg3, arg4, arg5, arg6, arg7);
	    }
	}

	public static abstract class AbstractPublicSignal8<A,B,C,D,E,F,G,H> extends AbstractPrivateSignal8<A,B,C,D,E,F,G,H> {
		AbstractPublicSignal8(){super();}
		
		AbstractPublicSignal8(Class<?> declaringClass, boolean isDisposedSignal) {
			super(declaringClass, isDisposedSignal);
		}
		
		AbstractPublicSignal8(Class<?>... types) {
			super(types);
		}
		
		AbstractPublicSignal8(String signalName, Class<?>... types) {
			super(signalName, types);
		}
		
		/**
	     * Emits the signal.
	     */
	    public final void emit(A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7, H arg8) {
	        emitSignal(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
	    }
	}

	public static abstract class AbstractPublicSignal9<A,B,C,D,E,F,G,H,I> extends AbstractPrivateSignal9<A,B,C,D,E,F,G,H,I> {
		AbstractPublicSignal9(){super();}
		
		AbstractPublicSignal9(Class<?> declaringClass, boolean isDisposedSignal) {
			super(declaringClass, isDisposedSignal);
		}
		
		AbstractPublicSignal9(Class<?>... types) {
			super(types);
		}
		
		AbstractPublicSignal9(String signalName, Class<?>... types) {
			super(signalName, types);
		}
		
		/**
	     * Emits the signal.
	     */
	    public final void emit(A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7, H arg8, I arg9) {
	        emitSignal(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
	    }
	}

	public static abstract class AbstractSignal2Default1<A, B> extends AbstractPublicSignal2<A, B> {
		
		AbstractSignal2Default1(Supplier<B> arg2Default, Class<?> declaringClass, boolean isDisposedSignal){
			super(declaringClass, isDisposedSignal);
			if(arg2Default!=null){
				this.arg2Default = arg2Default;
			}else{
	    		throw new QNoDefaultValueException(2);
	    	}
		}
		
		AbstractSignal2Default1(Supplier<B> arg2Default){
			super();
			if(arg2Default!=null){
				this.arg2Default = arg2Default;
			}else{
	    		throw new QNoDefaultValueException(2);
	    	}
		}
		
		AbstractSignal2Default1(Supplier<B> arg2Default, Class<?>... types) {
			super(types);
			if(arg2Default!=null){
				this.arg2Default = arg2Default;
			}else{
	    		throw new QNoDefaultValueException(2);
	    	}
		}
		
		AbstractSignal2Default1(String signalName, Supplier<B> arg2Default){
			super(signalName);
			if(arg2Default!=null){
				this.arg2Default = arg2Default;
			}else{
	    		throw new QNoDefaultValueException(2);
	    	}
		}
		
		AbstractSignal2Default1(String signalName, Supplier<B> arg2Default, Class<?>... types) {
			super(signalName, types);
			if(arg2Default!=null){
				this.arg2Default = arg2Default;
			}else{
	    		throw new QNoDefaultValueException(2);
	    	}
		}
		
		private final Supplier<B> arg2Default;
	    
	    public void emit(A arg1) {
	   		emit(arg1, arg2Default.get());
	    }
	}

	public static abstract class AbstractSignal3Default1<A, B, C> extends AbstractPublicSignal3<A, B, C> {
		
		AbstractSignal3Default1(Supplier<C> arg3Default, Class<?> declaringClass, boolean isDisposedSignal){
			super(declaringClass, isDisposedSignal);
			if(arg3Default!=null){
				this.arg3Default = arg3Default;
			}else{
	    		throw new QNoDefaultValueException(3);
	    	}
		}
		
		AbstractSignal3Default1(Supplier<C> arg3Default){
			super();
			if(arg3Default!=null){
				this.arg3Default = arg3Default;
			}else{
	    		throw new QNoDefaultValueException(3);
	    	}
		}
		
		AbstractSignal3Default1(Supplier<C> arg3Default, Class<?>... types) {
			super(types);
			if(arg3Default!=null){
				this.arg3Default = arg3Default;
			}else{
	    		throw new QNoDefaultValueException(3);
	    	}
		}
		
		AbstractSignal3Default1(String signalName, Supplier<C> arg3Default, Class<?>... types) {
			super(signalName, types);
			if(arg3Default!=null){
				this.arg3Default = arg3Default;
			}else{
	    		throw new QNoDefaultValueException(3);
	    	}
		}
		
		private final Supplier<C> arg3Default;
	    
	    public void emit(A arg1, B arg2) {
	   		emit(arg1, arg2, arg3Default.get());
	    }
	}

	public static abstract class AbstractSignal3Default2<A, B, C> extends AbstractSignal3Default1<A, B, C> {
		
		AbstractSignal3Default2(Supplier<B> arg2Default, Supplier<C> arg3Default, Class<?> declaringClass, boolean isDisposedSignal){
			super(arg3Default, declaringClass, isDisposedSignal);
			if(arg2Default!=null){
				this.arg2Default = arg2Default;
			}else{
	    		throw new QNoDefaultValueException(2);
	    	}
		}
		
		AbstractSignal3Default2(Supplier<B> arg2Default, Supplier<C> arg3Default){
			super(arg3Default);
			if(arg2Default!=null){
				this.arg2Default = arg2Default;
			}else{
	    		throw new QNoDefaultValueException(2);
	    	}
		}
		
		AbstractSignal3Default2(Supplier<B> arg2Default, Supplier<C> arg3Default, Class<?>... types) {
			super(arg3Default, types);
			if(arg2Default!=null){
				this.arg2Default = arg2Default;
			}else{
	    		throw new QNoDefaultValueException(2);
	    	}
		}
		
		AbstractSignal3Default2(String signalName, Supplier<B> arg2Default, Supplier<C> arg3Default, Class<?>... types) {
			super(signalName, arg3Default, types);
			if(arg2Default!=null){
				this.arg2Default = arg2Default;
			}else{
	    		throw new QNoDefaultValueException(2);
	    	}
		}
		
		private final Supplier<B> arg2Default;
	    
	    public void emit(A arg1) {
	   		emit(arg1, arg2Default.get());
	    }
	}

	public static abstract class AbstractSignal4Default1<A, B, C, D> extends AbstractPublicSignal4<A, B, C, D> {
		
		AbstractSignal4Default1(Supplier<D> arg4Default, Class<?> declaringClass, boolean isDisposedSignal){
			super(declaringClass, isDisposedSignal);
			if(arg4Default!=null){
				this.arg4Default = arg4Default;
			}else{
	    		throw new QNoDefaultValueException(4);
	    	}
		}
		
		AbstractSignal4Default1(Supplier<D> arg4Default){
			super();
			if(arg4Default!=null){
				this.arg4Default = arg4Default;
			}else{
	    		throw new QNoDefaultValueException(4);
	    	}
		}
		
		AbstractSignal4Default1(Supplier<D> arg4Default, Class<?>... types) {
			super(types);
			if(arg4Default!=null){
				this.arg4Default = arg4Default;
			}else{
	    		throw new QNoDefaultValueException(4);
	    	}
		}
		
		AbstractSignal4Default1(String signalName, Supplier<D> arg4Default, Class<?>... types) {
			super(signalName, types);
			if(arg4Default!=null){
				this.arg4Default = arg4Default;
			}else{
	    		throw new QNoDefaultValueException(4);
	    	}
		}
		
		private final Supplier<D> arg4Default;
	    
	    public void emit(A arg1, B arg2, C arg3) {
	   		emit(arg1, arg2, arg3, arg4Default.get());
	    }
	}

	public static abstract class AbstractSignal4Default2<A, B, C, D> extends AbstractSignal4Default1<A, B, C, D> {
		
		AbstractSignal4Default2(Supplier<C> arg3Default, Supplier<D> arg4Default, Class<?> declaringClass, boolean isDisposedSignal){
			super(arg4Default, declaringClass, isDisposedSignal);
			if(arg3Default!=null){
				this.arg3Default = arg3Default;
			}else{
	    		throw new QNoDefaultValueException(3);
	    	}
		}
		
		AbstractSignal4Default2(Supplier<C> arg3Default, Supplier<D> arg4Default){
			super(arg4Default);
			if(arg3Default!=null){
				this.arg3Default = arg3Default;
			}else{
	    		throw new QNoDefaultValueException(3);
	    	}
		}
		
		AbstractSignal4Default2(Supplier<C> arg3Default, Supplier<D> arg4Default, Class<?>... types) {
			super(arg4Default, types);
			if(arg3Default!=null){
				this.arg3Default = arg3Default;
			}else{
	    		throw new QNoDefaultValueException(3);
	    	}
		}
		
		AbstractSignal4Default2(String signalName, Supplier<C> arg3Default, Supplier<D> arg4Default, Class<?>... types) {
			super(signalName, arg4Default, types);
			if(arg3Default!=null){
				this.arg3Default = arg3Default;
			}else{
	    		throw new QNoDefaultValueException(3);
	    	}
		}
		
		private final Supplier<C> arg3Default;
	    
	    public void emit(A arg1, B arg2) {
	   		emit(arg1, arg2, arg3Default.get());
	    }
	}

	public static abstract class AbstractSignal4Default3<A, B, C, D> extends AbstractSignal4Default2<A, B, C, D> {
		
		AbstractSignal4Default3(Supplier<B> arg2Default, Supplier<C> arg3Default, Supplier<D> arg4Default, Class<?> declaringClass, boolean isDisposedSignal){
			super(arg3Default, arg4Default, declaringClass, isDisposedSignal);
			if(arg2Default!=null){
				this.arg2Default = arg2Default;
			}else{
	    		throw new QNoDefaultValueException(2);
	    	}
		}
		
		AbstractSignal4Default3(Supplier<B> arg2Default, Supplier<C> arg3Default, Supplier<D> arg4Default){
			super(arg3Default, arg4Default);
			if(arg2Default!=null){
				this.arg2Default = arg2Default;
			}else{
	    		throw new QNoDefaultValueException(2);
	    	}
		}
		
		AbstractSignal4Default3(Supplier<B> arg2Default, Supplier<C> arg3Default, Supplier<D> arg4Default, Class<?>... types) {
			super(arg3Default, arg4Default, types);
			if(arg3Default!=null){
				this.arg2Default = arg2Default;
			}else{
	    		throw new QNoDefaultValueException(2);
	    	}
		}
		
		AbstractSignal4Default3(String signalName, Supplier<B> arg2Default, Supplier<C> arg3Default, Supplier<D> arg4Default, Class<?>... types) {
			super(signalName, arg3Default, arg4Default, types);
			if(arg3Default!=null){
				this.arg2Default = arg2Default;
			}else{
	    		throw new QNoDefaultValueException(2);
	    	}
		}
		
		private final Supplier<B> arg2Default;
	    
	    public void emit(A arg1) {
	   		emit(arg1, arg2Default.get());
	    }
	}

	public static abstract class AbstractSignal5Default1<A, B, C, D, E> extends AbstractPublicSignal5<A, B, C, D, E> {
		
		AbstractSignal5Default1(Supplier<E> arg5Default, Class<?> declaringClass, boolean isDisposedSignal){
			super(declaringClass, isDisposedSignal);
			if(arg5Default!=null){
				this.arg5Default = arg5Default;
			}else{
	    		throw new QNoDefaultValueException(5);
	    	}
		}
		
		AbstractSignal5Default1(Supplier<E> arg5Default){
			super();
			if(arg5Default!=null){
				this.arg5Default = arg5Default;
			}else{
	    		throw new QNoDefaultValueException(5);
	    	}
		}
		
		AbstractSignal5Default1(Supplier<E> arg5Default, Class<?>... types) {
			super(types);
			if(arg5Default!=null){
				this.arg5Default = arg5Default;
			}else{
	    		throw new QNoDefaultValueException(5);
	    	}
		}
		
		AbstractSignal5Default1(String signalName, Supplier<E> arg5Default, Class<?>... types) {
			super(signalName, types);
			if(arg5Default!=null){
				this.arg5Default = arg5Default;
			}else{
	    		throw new QNoDefaultValueException(5);
	    	}
		}
		
		private final Supplier<E> arg5Default;
	    
	    public void emit(A arg1, B arg2, C arg3, D arg4) {
	   		emit(arg1, arg2, arg3, arg4, arg5Default.get());
	    }
	}

	public static abstract class AbstractSignal5Default2<A, B, C, D, E> extends AbstractSignal5Default1<A, B, C, D, E> {
		
		AbstractSignal5Default2(Supplier<D> arg4Default, Supplier<E> arg5Default, Class<?> declaringClass, boolean isDisposedSignal){
			super(arg5Default, declaringClass, isDisposedSignal);
			if(arg4Default!=null){
				this.arg4Default = arg4Default;
			}else{
	    		throw new QNoDefaultValueException(4);
	    	}
		}
		
		AbstractSignal5Default2(Supplier<D> arg4Default, Supplier<E> arg5Default){
			super(arg5Default);
			if(arg4Default!=null){
				this.arg4Default = arg4Default;
			}else{
	    		throw new QNoDefaultValueException(4);
	    	}
		}
		
		AbstractSignal5Default2(Supplier<D> arg4Default, Supplier<E> arg5Default, Class<?>... types) {
			super(arg5Default, types);
			if(arg4Default!=null){
				this.arg4Default = arg4Default;
			}else{
	    		throw new QNoDefaultValueException(4);
	    	}
		}
		
		AbstractSignal5Default2(String signalName, Supplier<D> arg4Default, Supplier<E> arg5Default, Class<?>... types) {
			super(signalName, arg5Default, types);
			if(arg4Default!=null){
				this.arg4Default = arg4Default;
			}else{
	    		throw new QNoDefaultValueException(4);
	    	}
		}
		
		private final Supplier<D> arg4Default;
	    
	    public void emit(A arg1, B arg2, C arg3) {
	   		emit(arg1, arg2, arg3, arg4Default.get());
	    }
	}

	public static abstract class AbstractSignal5Default3<A, B, C, D, E> extends AbstractSignal5Default2<A, B, C, D, E> {
		
		AbstractSignal5Default3(Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default, Class<?> declaringClass, boolean isDisposedSignal){
			super(arg4Default, arg5Default, declaringClass, isDisposedSignal);
			if(arg3Default!=null){
				this.arg3Default = arg3Default;
			}else{
	    		throw new QNoDefaultValueException(3);
	    	}
		}
		
		AbstractSignal5Default3(Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default){
			super(arg4Default, arg5Default);
			if(arg3Default!=null){
				this.arg3Default = arg3Default;
			}else{
	    		throw new QNoDefaultValueException(3);
	    	}
		}
		
		AbstractSignal5Default3(Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default, Class<?>... types) {
			super(arg4Default, arg5Default, types);
			if(arg3Default!=null){
				this.arg3Default = arg3Default;
			}else{
	    		throw new QNoDefaultValueException(3);
	    	}
		}
		
		AbstractSignal5Default3(String signalName, Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default, Class<?>... types) {
			super(signalName, arg4Default, arg5Default, types);
			if(arg3Default!=null){
				this.arg3Default = arg3Default;
			}else{
	    		throw new QNoDefaultValueException(3);
	    	}
		}
		
		private final Supplier<C> arg3Default;
	    
	    public void emit(A arg1, B arg2) {
	   		emit(arg1, arg2, arg3Default.get());
	    }
	}

	public static abstract class AbstractSignal5Default4<A, B, C, D, E> extends AbstractSignal5Default3<A, B, C, D, E> {
		
		AbstractSignal5Default4(Supplier<B> arg2Default, Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default, Class<?> declaringClass, boolean isDisposedSignal){
			super(arg3Default, arg4Default, arg5Default, declaringClass, isDisposedSignal);
			if(arg2Default!=null){
				this.arg2Default = arg2Default;
			}else{
	    		throw new QNoDefaultValueException(2);
	    	}
		}
		
		AbstractSignal5Default4(Supplier<B> arg2Default, Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default){
			super(arg3Default, arg4Default, arg5Default);
			if(arg2Default!=null){
				this.arg2Default = arg2Default;
			}else{
	    		throw new QNoDefaultValueException(2);
	    	}
		}
		
		AbstractSignal5Default4(Supplier<B> arg2Default, Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default, Class<?>... types) {
			super(arg3Default, arg4Default, arg5Default, types);
			if(arg2Default!=null){
				this.arg2Default = arg2Default;
			}else{
	    		throw new QNoDefaultValueException(2);
	    	}
		}
		
		AbstractSignal5Default4(String signalName, Supplier<B> arg2Default, Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default, Class<?>... types) {
			super(signalName, arg3Default, arg4Default, arg5Default, types);
			if(arg2Default!=null){
				this.arg2Default = arg2Default;
			}else{
	    		throw new QNoDefaultValueException(2);
	    	}
		}
		
		private final Supplier<B> arg2Default;
	    
	    public void emit(A arg1) {
	   		emit(arg1, arg2Default.get());
	    }
	}

	public static abstract class AbstractSignal6Default1<A, B, C, D, E, F> extends AbstractPublicSignal6<A, B, C, D, E, F> {
		
		AbstractSignal6Default1(Supplier<F> arg6Default, Class<?> declaringClass, boolean isDisposedSignal){
			super(declaringClass, isDisposedSignal);
			if(arg6Default!=null){
				this.arg6Default = arg6Default;
			}else{
	    		throw new QNoDefaultValueException(6);
	    	}
		}
		
		AbstractSignal6Default1(Supplier<F> arg6Default){
			super();
			if(arg6Default!=null){
				this.arg6Default = arg6Default;
			}else{
	    		throw new QNoDefaultValueException(6);
	    	}
		}
		
		AbstractSignal6Default1(Supplier<F> arg6Default, Class<?>... types) {
			super(types);
			if(arg6Default!=null){
				this.arg6Default = arg6Default;
			}else{
	    		throw new QNoDefaultValueException(6);
	    	}
		}
		
		AbstractSignal6Default1(String signalName, Supplier<F> arg6Default, Class<?>... types) {
			super(signalName, types);
			if(arg6Default!=null){
				this.arg6Default = arg6Default;
			}else{
	    		throw new QNoDefaultValueException(6);
	    	}
		}
		
		private final Supplier<F> arg6Default;
	    
	    public void emit(A arg1, B arg2, C arg3, D arg4, E arg5) {
	   		emit(arg1, arg2, arg3, arg4, arg5, arg6Default.get());
	    }
	}

	public static abstract class AbstractSignal6Default2<A, B, C, D, E, F> extends AbstractSignal6Default1<A, B, C, D, E, F> {
		
		AbstractSignal6Default2(Supplier<E> arg5Default, Supplier<F> arg6Default, Class<?> declaringClass, boolean isDisposedSignal){
			super(arg6Default, declaringClass, isDisposedSignal);
			if(arg5Default!=null){
				this.arg5Default = arg5Default;
			}else{
	    		throw new QNoDefaultValueException(5);
	    	}
		}
		
		AbstractSignal6Default2(Supplier<E> arg5Default, Supplier<F> arg6Default){
			super(arg6Default);
			if(arg5Default!=null){
				this.arg5Default = arg5Default;
			}else{
	    		throw new QNoDefaultValueException(5);
	    	}
		}
		
		AbstractSignal6Default2(Supplier<E> arg5Default, Supplier<F> arg6Default, Class<?>... types) {
			super(arg6Default, types);
			if(arg5Default!=null){
				this.arg5Default = arg5Default;
			}else{
	    		throw new QNoDefaultValueException(5);
	    	}
		}
		
		AbstractSignal6Default2(String signalName, Supplier<E> arg5Default, Supplier<F> arg6Default, Class<?>... types) {
			super(signalName, arg6Default, types);
			if(arg5Default!=null){
				this.arg5Default = arg5Default;
			}else{
	    		throw new QNoDefaultValueException(5);
	    	}
		}
		
		private final Supplier<E> arg5Default;
	    
	    public void emit(A arg1, B arg2, C arg3, D arg4) {
	   		emit(arg1, arg2, arg3, arg4, arg5Default.get());
	    }
	}
	
	public static abstract class AbstractSignal6Default3<A, B, C, D, E, F> extends AbstractSignal6Default2<A, B, C, D, E, F> {
    	
    	AbstractSignal6Default3(Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Class<?> declaringClass, boolean isDisposedSignal){
    		super(arg5Default, arg6Default, declaringClass, isDisposedSignal);
    		if(arg4Default!=null){
    			this.arg4Default = arg4Default;
    		}else{
        		throw new QNoDefaultValueException(4);
        	}
    	}
    	
    	AbstractSignal6Default3(Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default){
    		super(arg5Default, arg6Default);
    		if(arg4Default!=null){
    			this.arg4Default = arg4Default;
    		}else{
        		throw new QNoDefaultValueException(4);
        	}
    	}
    	
    	AbstractSignal6Default3(Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Class<?>... types) {
			super(arg5Default, arg6Default, types);
			if(arg4Default!=null){
    			this.arg4Default = arg4Default;
    		}else{
        		throw new QNoDefaultValueException(4);
        	}
		}
    	
    	AbstractSignal6Default3(String signalName, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Class<?>... types) {
			super(signalName, arg5Default, arg6Default, types);
			if(arg4Default!=null){
    			this.arg4Default = arg4Default;
    		}else{
        		throw new QNoDefaultValueException(4);
        	}
		}
    	
    	private final Supplier<D> arg4Default;
        
        public void emit(A arg1, B arg2, C arg3) {
       		emit(arg1, arg2, arg3, arg4Default.get());
        }
    }
    
    public static abstract class AbstractSignal6Default4<A, B, C, D, E, F> extends AbstractSignal6Default3<A, B, C, D, E, F> {
    	
    	AbstractSignal6Default4(Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Class<?> declaringClass, boolean isDisposedSignal){
    		super(arg4Default, arg5Default, arg6Default, declaringClass, isDisposedSignal);
    		if(arg3Default!=null){
    			this.arg3Default = arg3Default;
    		}else{
        		throw new QNoDefaultValueException(3);
        	}
    	}
    	
    	AbstractSignal6Default4(Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default){
    		super(arg4Default, arg5Default, arg6Default);
    		if(arg3Default!=null){
    			this.arg3Default = arg3Default;
    		}else{
        		throw new QNoDefaultValueException(3);
        	}
    	}
    	
    	AbstractSignal6Default4(Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Class<?>... types) {
			super(arg4Default, arg5Default, arg6Default, types);
			if(arg3Default!=null){
    			this.arg3Default = arg3Default;
    		}else{
        		throw new QNoDefaultValueException(3);
        	}
		}
    	
    	AbstractSignal6Default4(String signalName, Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Class<?>... types) {
			super(signalName, arg4Default, arg5Default, arg6Default, types);
			if(arg3Default!=null){
    			this.arg3Default = arg3Default;
    		}else{
        		throw new QNoDefaultValueException(3);
        	}
		}
    	
    	private final Supplier<C> arg3Default;
        
        public void emit(A arg1, B arg2) {
       		emit(arg1, arg2, arg3Default.get());
        }
    }
    
    public static abstract class AbstractSignal6Default5<A, B, C, D, E, F> extends AbstractSignal6Default4<A, B, C, D, E, F> {
    	
    	AbstractSignal6Default5(Supplier<B> arg2Default, Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Class<?> declaringClass, boolean isDisposedSignal){
    		super(arg3Default, arg4Default, arg5Default, arg6Default, declaringClass, isDisposedSignal);
    		if(arg2Default!=null){
    			this.arg2Default = arg2Default;
    		}else{
        		throw new QNoDefaultValueException(2);
        	}
    	}
    	
    	AbstractSignal6Default5(Supplier<B> arg2Default, Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default){
    		super(arg3Default, arg4Default, arg5Default, arg6Default);
    		if(arg2Default!=null){
    			this.arg2Default = arg2Default;
    		}else{
        		throw new QNoDefaultValueException(2);
        	}
    	}
    	
    	AbstractSignal6Default5(Supplier<B> arg2Default, Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Class<?>... types) {
			super(arg3Default, arg4Default, arg5Default, arg6Default, types);
			if(arg2Default!=null){
    			this.arg2Default = arg2Default;
    		}else{
        		throw new QNoDefaultValueException(2);
        	}
		}
    	
    	AbstractSignal6Default5(String signalName, Supplier<B> arg2Default, Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Class<?>... types) {
			super(signalName, arg3Default, arg4Default, arg5Default, arg6Default, types);
			if(arg2Default!=null){
    			this.arg2Default = arg2Default;
    		}else{
        		throw new QNoDefaultValueException(2);
        	}
		}
    	
    	private final Supplier<B> arg2Default;
        
        public void emit(A arg1) {
       		emit(arg1, arg2Default.get());
        }
    }

    public static abstract class AbstractSignal7Default1<A, B, C, D, E, F, G> extends AbstractPublicSignal7<A, B, C, D, E, F, G> {
    	
    	AbstractSignal7Default1(Supplier<G> arg7Default, Class<?> declaringClass, boolean isDisposedSignal){
    		super(declaringClass, isDisposedSignal);
    		if(arg7Default!=null){
    			this.arg7Default = arg7Default;
    		}else{
        		throw new QNoDefaultValueException(7);
        	}
    	}
    	
    	AbstractSignal7Default1(Supplier<G> arg7Default){
    		super();
    		if(arg7Default!=null){
    			this.arg7Default = arg7Default;
    		}else{
        		throw new QNoDefaultValueException(7);
        	}
    	}
    	
    	AbstractSignal7Default1(Supplier<G> arg7Default, Class<?>... types) {
			super(types);
			if(arg7Default!=null){
    			this.arg7Default = arg7Default;
    		}else{
        		throw new QNoDefaultValueException(7);
        	}
		}
    	
    	AbstractSignal7Default1(String signalName, Supplier<G> arg7Default, Class<?>... types) {
			super(signalName, types);
			if(arg7Default!=null){
    			this.arg7Default = arg7Default;
    		}else{
        		throw new QNoDefaultValueException(7);
        	}
		}
    	
    	private final Supplier<G> arg7Default;
        
        public void emit(A arg1, B arg2, C arg3, D arg4, E arg5, F arg6) {
       		emit(arg1, arg2, arg3, arg4, arg5, arg6, arg7Default.get());
        }
    }
    
    public static abstract class AbstractSignal7Default2<A, B, C, D, E, F, G> extends AbstractSignal7Default1<A, B, C, D, E, F, G> {
    	
    	AbstractSignal7Default2(Supplier<F> arg6Default, Supplier<G> arg7Default, Class<?> declaringClass, boolean isDisposedSignal){
    		super(arg7Default, declaringClass, isDisposedSignal);
			this.arg6Default = arg6Default;
    		if(this.arg6Default==null){
        		throw new QNoDefaultValueException(6);
        	}
    	}
    	
    	AbstractSignal7Default2(Supplier<F> arg6Default, Supplier<G> arg7Default){
    		super(arg7Default);
			this.arg6Default = arg6Default;
    		if(this.arg6Default==null){
        		throw new QNoDefaultValueException(6);
        	}
    	}
    	
    	AbstractSignal7Default2(Supplier<F> arg6Default, Supplier<G> arg7Default, Class<?>... types) {
			super(arg7Default, types);
			this.arg6Default = arg6Default;
    		if(this.arg6Default==null){
        		throw new QNoDefaultValueException(6);
        	}
		}
    	
    	AbstractSignal7Default2(String signalName, Supplier<F> arg6Default, Supplier<G> arg7Default, Class<?>... types) {
			super(signalName, arg7Default, types);
			this.arg6Default = arg6Default;
    		if(this.arg6Default==null){
        		throw new QNoDefaultValueException(6);
        	}
		}
    	
    	private final Supplier<F> arg6Default;
        
        public void emit(A arg1, B arg2, C arg3, D arg4, E arg5) {
       		emit(arg1, arg2, arg3, arg4, arg5, arg6Default.get());
        }
    }
    
    public static abstract class AbstractSignal7Default3<A, B, C, D, E, F, G> extends AbstractSignal7Default2<A, B, C, D, E, F, G> {
    	
    	AbstractSignal7Default3(Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default, Class<?> declaringClass, boolean isDisposedSignal){
    		super(arg6Default, arg7Default, declaringClass, isDisposedSignal);
			this.arg5Default = arg5Default;
    		if(this.arg5Default==null){
        		throw new QNoDefaultValueException(5);
        	}
    	}
    	
    	AbstractSignal7Default3(Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default){
    		super(arg6Default, arg7Default);
			this.arg5Default = arg5Default;
    		if(this.arg5Default==null){
        		throw new QNoDefaultValueException(5);
        	}
    	}
    	
    	AbstractSignal7Default3(Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default, Class<?>... types) {
			super(arg6Default, arg7Default, types);
			this.arg5Default = arg5Default;
    		if(this.arg5Default==null){
        		throw new QNoDefaultValueException(5);
        	}
		}
    	
    	AbstractSignal7Default3(String signalName, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default, Class<?>... types) {
			super(signalName, arg6Default, arg7Default, types);
			this.arg5Default = arg5Default;
    		if(this.arg5Default==null){
        		throw new QNoDefaultValueException(5);
        	}
		}
    	
    	private final Supplier<E> arg5Default;
        
        public void emit(A arg1, B arg2, C arg3, D arg4) {
       		emit(arg1, arg2, arg3, arg4, arg5Default.get());
        }
    }
    
    public static abstract class AbstractSignal7Default4<A, B, C, D, E, F, G> extends AbstractSignal7Default3<A, B, C, D, E, F, G> {
    	
    	AbstractSignal7Default4(Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default, Class<?> declaringClass, boolean isDisposedSignal){
    		super(arg5Default, arg6Default, arg7Default, declaringClass, isDisposedSignal);
			this.arg4Default = arg4Default;
    		if(this.arg4Default==null){
        		throw new QNoDefaultValueException(4);
        	}
    	}
    	
    	AbstractSignal7Default4(Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default){
    		super(arg5Default, arg6Default, arg7Default);
			this.arg4Default = arg4Default;
    		if(this.arg4Default==null){
        		throw new QNoDefaultValueException(4);
        	}
    	}
    	
    	AbstractSignal7Default4(Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default, Class<?>... types) {
			super(arg5Default, arg6Default, arg7Default, types);
			this.arg4Default = arg4Default;
    		if(this.arg4Default==null){
        		throw new QNoDefaultValueException(4);
        	}
		}
    	
    	AbstractSignal7Default4(String signalName, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default, Class<?>... types) {
			super(signalName, arg5Default, arg6Default, arg7Default, types);
			this.arg4Default = arg4Default;
    		if(this.arg4Default==null){
        		throw new QNoDefaultValueException(4);
        	}
		}
    	
    	private final Supplier<D> arg4Default;
        
        public void emit(A arg1, B arg2, C arg3) {
       		emit(arg1, arg2, arg3, arg4Default.get());
        }
    }
    
    public static abstract class AbstractSignal7Default5<A, B, C, D, E, F, G> extends AbstractSignal7Default4<A, B, C, D, E, F, G> {
    	
    	AbstractSignal7Default5(Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default, Class<?> declaringClass, boolean isDisposedSignal){
    		super(arg4Default, arg5Default, arg6Default, arg7Default, declaringClass, isDisposedSignal);
			this.arg3Default = arg3Default;
    		if(this.arg3Default==null){
        		throw new QNoDefaultValueException(3);
        	}
    	}
    	
    	AbstractSignal7Default5(Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default){
    		super(arg4Default, arg5Default, arg6Default, arg7Default);
			this.arg3Default = arg3Default;
    		if(this.arg3Default==null){
        		throw new QNoDefaultValueException(3);
        	}
    	}
    	
    	AbstractSignal7Default5(Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default, Class<?>... types) {
			super(arg4Default, arg5Default, arg6Default, arg7Default, types);
			this.arg3Default = arg3Default;
    		if(this.arg3Default==null){
        		throw new QNoDefaultValueException(3);
        	}
		}
    	
    	AbstractSignal7Default5(String signalName, Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default, Class<?>... types) {
			super(signalName, arg4Default, arg5Default, arg6Default, arg7Default, types);
			this.arg3Default = arg3Default;
    		if(this.arg3Default==null){
        		throw new QNoDefaultValueException(3);
        	}
		}
    	
    	private final Supplier<C> arg3Default;
        
        public void emit(A arg1, B arg2) {
       		emit(arg1, arg2, arg3Default.get());
        }
    }
    
    public static abstract class AbstractSignal7Default6<A, B, C, D, E, F, G> extends AbstractSignal7Default5<A, B, C, D, E, F, G> {
    	
    	AbstractSignal7Default6(Supplier<B> arg2Default, Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default, Class<?> declaringClass, boolean isDisposedSignal){
    		super(arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, declaringClass, isDisposedSignal);
			this.arg2Default = arg2Default;
    		if(this.arg2Default==null){
        		throw new QNoDefaultValueException(2);
        	}
    	}
    	
    	AbstractSignal7Default6(Supplier<B> arg2Default, Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default){
    		super(arg3Default, arg4Default, arg5Default, arg6Default, arg7Default);
			this.arg2Default = arg2Default;
    		if(this.arg2Default==null){
        		throw new QNoDefaultValueException(2);
        	}
    	}
    	
    	AbstractSignal7Default6(Supplier<B> arg2Default, Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default, Class<?>... types) {
			super(arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, types);
			this.arg2Default = arg2Default;
    		if(this.arg2Default==null){
        		throw new QNoDefaultValueException(2);
        	}
		}
    	
    	AbstractSignal7Default6(String signalName, Supplier<B> arg2Default, Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default, Class<?>... types) {
			super(signalName, arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, types);
			this.arg2Default = arg2Default;
    		if(this.arg2Default==null){
        		throw new QNoDefaultValueException(2);
        	}
		}
    	
    	private final Supplier<B> arg2Default;
        
        public void emit(A arg1) {
       		emit(arg1, arg2Default.get());
        }
    }
    
    public static abstract class AbstractSignal8Default1<A, B, C, D, E, F, G, H> extends AbstractPublicSignal8<A, B, C, D, E, F, G, H> {
    	
    	AbstractSignal8Default1(Supplier<H> arg8Default, Class<?> declaringClass, boolean isDisposedSignal){
    		super(declaringClass, isDisposedSignal);
			this.arg8Default = arg8Default;
			if(this.arg8Default==null){
        		throw new QNoDefaultValueException(8);
        	}
    	}
    	
    	AbstractSignal8Default1(Supplier<H> arg8Default){
    		super();
			this.arg8Default = arg8Default;
			if(this.arg8Default==null){
        		throw new QNoDefaultValueException(8);
        	}
    	}
    	
    	AbstractSignal8Default1(Supplier<H> arg8Default, Class<?>... types) {
			super(types);
			this.arg8Default = arg8Default;
			if(this.arg8Default==null){
        		throw new QNoDefaultValueException(8);
        	}
		}
    	
    	AbstractSignal8Default1(String signalName, Supplier<H> arg8Default, Class<?>... types) {
			super(signalName, types);
			this.arg8Default = arg8Default;
			if(this.arg8Default==null){
        		throw new QNoDefaultValueException(8);
        	}
		}
    	
    	private final Supplier<H> arg8Default;
        
        public void emit(A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7) {
       		emit(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8Default.get());
        }
    }
    
    public static abstract class AbstractSignal8Default2<A, B, C, D, E, F, G, H> extends AbstractSignal8Default1<A, B, C, D, E, F, G, H> {
    	
    	AbstractSignal8Default2(Supplier<G> arg7Default, Supplier<H> arg8Default, Class<?> declaringClass, boolean isDisposedSignal){
    		super(arg8Default, declaringClass, isDisposedSignal);
			this.arg7Default = arg7Default;
			if(this.arg7Default==null){
        		throw new QNoDefaultValueException(7);
        	}
    	}
    	
    	AbstractSignal8Default2(Supplier<G> arg7Default, Supplier<H> arg8Default){
    		super(arg8Default);
			this.arg7Default = arg7Default;
			if(this.arg7Default==null){
        		throw new QNoDefaultValueException(7);
        	}
    	}
    	
    	AbstractSignal8Default2(Supplier<G> arg7Default, Supplier<H> arg8Default, Class<?>... types) {
			super(arg8Default, types);
			this.arg7Default = arg7Default;
			if(this.arg7Default==null){
        		throw new QNoDefaultValueException(7);
        	}
		}
    	
    	AbstractSignal8Default2(String signalName, Supplier<G> arg7Default, Supplier<H> arg8Default, Class<?>... types) {
			super(signalName, arg8Default, types);
			this.arg7Default = arg7Default;
			if(this.arg7Default==null){
        		throw new QNoDefaultValueException(7);
        	}
		}
    	
    	private final Supplier<G> arg7Default;
        
        public void emit(A arg1, B arg2, C arg3, D arg4, E arg5, F arg6) {
       		emit(arg1, arg2, arg3, arg4, arg5, arg6, arg7Default.get());
        }
    }
    
    public static abstract class AbstractSignal8Default3<A, B, C, D, E, F, G, H> extends AbstractSignal8Default2<A, B, C, D, E, F, G, H> {
    	
    	AbstractSignal8Default3(Supplier<F> arg6Default, Supplier<G> arg7Default, Supplier<H> arg8Default, Class<?> declaringClass, boolean isDisposedSignal){
    		super(arg7Default, arg8Default, declaringClass, isDisposedSignal);
			this.arg6Default = arg6Default;
			if(this.arg6Default==null){
        		throw new QNoDefaultValueException(6);
        	}
    	}
    	
    	AbstractSignal8Default3(Supplier<F> arg6Default, Supplier<G> arg7Default, Supplier<H> arg8Default){
    		super(arg7Default, arg8Default);
			this.arg6Default = arg6Default;
			if(this.arg6Default==null){
        		throw new QNoDefaultValueException(6);
        	}
    	}
    	
    	AbstractSignal8Default3(Supplier<F> arg6Default, Supplier<G> arg7Default, Supplier<H> arg8Default, Class<?>... types) {
			super(arg7Default, arg8Default, types);
			this.arg6Default = arg6Default;
			if(this.arg6Default==null){
        		throw new QNoDefaultValueException(6);
        	}
		}
    	
    	AbstractSignal8Default3(String signalName, Supplier<F> arg6Default, Supplier<G> arg7Default, Supplier<H> arg8Default, Class<?>... types) {
			super(signalName, arg7Default, arg8Default, types);
			this.arg6Default = arg6Default;
			if(this.arg6Default==null){
        		throw new QNoDefaultValueException(6);
        	}
		}
    	
    	private final Supplier<F> arg6Default;
        
        public void emit(A arg1, B arg2, C arg3, D arg4, E arg5) {
       		emit(arg1, arg2, arg3, arg4, arg5, arg6Default.get());
        }
    }
    
    public static abstract class AbstractSignal8Default4<A, B, C, D, E, F, G, H> extends AbstractSignal8Default3<A, B, C, D, E, F, G, H> {
    	
    	AbstractSignal8Default4(Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default, Supplier<H> arg8Default){
    		super(arg6Default, arg7Default, arg8Default);
			this.arg5Default = arg5Default;
			if(this.arg5Default==null){
        		throw new QNoDefaultValueException(5);
        	}
    	}
    	
    	AbstractSignal8Default4(Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default, Supplier<H> arg8Default, Class<?> declaringClass, boolean isDisposedSignal){
    		super(arg6Default, arg7Default, arg8Default, declaringClass, isDisposedSignal);
			this.arg5Default = arg5Default;
			if(this.arg5Default==null){
        		throw new QNoDefaultValueException(5);
        	}
    	}
    	
    	AbstractSignal8Default4(Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default, Supplier<H> arg8Default, Class<?>... types) {
			super(arg6Default, arg7Default, arg8Default, types);
			this.arg5Default = arg5Default;
			if(this.arg5Default==null){
        		throw new QNoDefaultValueException(5);
        	}
		}
    	
    	AbstractSignal8Default4(String signalName, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default, Supplier<H> arg8Default, Class<?>... types) {
			super(signalName, arg6Default, arg7Default, arg8Default, types);
			this.arg5Default = arg5Default;
			if(this.arg5Default==null){
        		throw new QNoDefaultValueException(5);
        	}
		}
    	
    	private final Supplier<E> arg5Default;
        
        public void emit(A arg1, B arg2, C arg3, D arg4) {
       		emit(arg1, arg2, arg3, arg4, arg5Default.get());
        }
    }
    
    public static abstract class AbstractSignal8Default5<A, B, C, D, E, F, G, H> extends AbstractSignal8Default4<A, B, C, D, E, F, G, H> {
    	
    	AbstractSignal8Default5(Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default, Supplier<H> arg8Default, Class<?> declaringClass, boolean isDisposedSignal){
    		super(arg5Default, arg6Default, arg7Default, arg8Default, declaringClass, isDisposedSignal);
			this.arg4Default = arg4Default;
			if(this.arg4Default==null){
        		throw new QNoDefaultValueException(4);
        	}
    	}
    	
    	AbstractSignal8Default5(Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default, Supplier<H> arg8Default){
    		super(arg5Default, arg6Default, arg7Default, arg8Default);
			this.arg4Default = arg4Default;
			if(this.arg4Default==null){
        		throw new QNoDefaultValueException(4);
        	}
    	}
    	
    	AbstractSignal8Default5(Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default, Supplier<H> arg8Default, Class<?>... types) {
			super(arg5Default, arg6Default, arg7Default, arg8Default, types);
			this.arg4Default = arg4Default;
			if(this.arg4Default==null){
        		throw new QNoDefaultValueException(4);
        	}
		}
    	
    	AbstractSignal8Default5(String signalName, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default, Supplier<H> arg8Default, Class<?>... types) {
			super(signalName, arg5Default, arg6Default, arg7Default, arg8Default, types);
			this.arg4Default = arg4Default;
			if(this.arg4Default==null){
        		throw new QNoDefaultValueException(4);
        	}
		}
    	
    	private final Supplier<D> arg4Default;
        
        public void emit(A arg1, B arg2, C arg3) {
       		emit(arg1, arg2, arg3, arg4Default.get());
        }
    }
    
    public static abstract class AbstractSignal8Default6<A, B, C, D, E, F, G, H> extends AbstractSignal8Default5<A, B, C, D, E, F, G, H> {
    	
    	AbstractSignal8Default6(Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default, Supplier<H> arg8Default, Class<?> declaringClass, boolean isDisposedSignal){
    		super(arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, declaringClass, isDisposedSignal);
			this.arg3Default = arg3Default;
			if(this.arg3Default==null){
        		throw new QNoDefaultValueException(3);
        	}
    	}
    	
    	AbstractSignal8Default6(Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default, Supplier<H> arg8Default){
    		super(arg4Default, arg5Default, arg6Default, arg7Default, arg8Default);
			this.arg3Default = arg3Default;
			if(this.arg3Default==null){
        		throw new QNoDefaultValueException(3);
        	}
    	}
    	
    	AbstractSignal8Default6(Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default, Supplier<H> arg8Default, Class<?>... types) {
			super(arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, types);
			this.arg3Default = arg3Default;
			if(this.arg3Default==null){
        		throw new QNoDefaultValueException(3);
        	}
		}
    	
    	AbstractSignal8Default6(String signalName, Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default, Supplier<H> arg8Default, Class<?>... types) {
			super(signalName, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, types);
			this.arg3Default = arg3Default;
			if(this.arg3Default==null){
        		throw new QNoDefaultValueException(3);
        	}
		}
    	
    	private final Supplier<C> arg3Default;
        
        public void emit(A arg1, B arg2) {
       		emit(arg1, arg2, arg3Default.get());
        }
    }
    
    public static abstract class AbstractSignal8Default7<A, B, C, D, E, F, G, H> extends AbstractSignal8Default6<A, B, C, D, E, F, G, H> {
    	
    	AbstractSignal8Default7(Supplier<B> arg2Default, Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default, Supplier<H> arg8Default){
    		super(arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default);
			this.arg2Default = arg2Default;
			if(this.arg2Default==null){
        		throw new QNoDefaultValueException(2);
        	}
    	}
    	
    	AbstractSignal8Default7(Supplier<B> arg2Default, Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default, Supplier<H> arg8Default, Class<?> declaringClass, boolean isDisposedSignal){
    		super(arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, declaringClass, isDisposedSignal);
			this.arg2Default = arg2Default;
			if(this.arg2Default==null){
        		throw new QNoDefaultValueException(2);
        	}
    	}
    	
    	AbstractSignal8Default7(Supplier<B> arg2Default, Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default, Supplier<H> arg8Default, Class<?>... types) {
			super(arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, types);
			this.arg2Default = arg2Default;
			if(this.arg2Default==null){
        		throw new QNoDefaultValueException(2);
        	}
		}
    	
    	AbstractSignal8Default7(String signalName, Supplier<B> arg2Default, Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default, Supplier<H> arg8Default, Class<?>... types) {
			super(signalName, arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, types);
			this.arg2Default = arg2Default;
			if(this.arg2Default==null){
        		throw new QNoDefaultValueException(2);
        	}
		}
    	
    	private final Supplier<B> arg2Default;
        
        public void emit(A arg1) {
       		emit(arg1, arg2Default.get());
        }
    }
    
    public static abstract class AbstractSignal9Default1<A, B, C, D, E, F, G, H, I> extends AbstractPublicSignal9<A, B, C, D, E, F, G, H, I> {
    	
    	AbstractSignal9Default1(Supplier<I> arg9Default){
    		super();
			this.arg9Default = arg9Default;
    		if(this.arg9Default==null){
        		throw new QNoDefaultValueException(9);
        	}
    	}
    	
    	AbstractSignal9Default1(Supplier<I> arg9Default, Class<?> declaringClass, boolean isDisposedSignal){
    		super(declaringClass, isDisposedSignal);
			this.arg9Default = arg9Default;
    		if(this.arg9Default==null){
        		throw new QNoDefaultValueException(9);
        	}
    	}
    	
    	AbstractSignal9Default1(Supplier<I> arg9Default, Class<?>... types) {
			super(types);
			this.arg9Default = arg9Default;
    		if(this.arg9Default==null){
        		throw new QNoDefaultValueException(9);
        	}
		}
    	
    	AbstractSignal9Default1(String signalName, Supplier<I> arg9Default, Class<?>... types) {
			super(signalName, types);
			this.arg9Default = arg9Default;
    		if(this.arg9Default==null){
        		throw new QNoDefaultValueException(9);
        	}
		}
    	
    	private final Supplier<I> arg9Default;
        
        public void emit(A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7, H arg8) {
       		emit(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9Default.get());
        }
    }
    
    public static abstract class AbstractSignal9Default2<A, B, C, D, E, F, G, H, I> extends AbstractSignal9Default1<A, B, C, D, E, F, G, H, I> {
    	
    	AbstractSignal9Default2(Supplier<H> arg8Default, Supplier<I> arg9Default){
    		super(arg9Default);
			this.arg8Default = arg8Default;
    		if(this.arg8Default==null){
        		throw new QNoDefaultValueException(8);
        	}
    	}
    	
    	AbstractSignal9Default2(Supplier<H> arg8Default, Supplier<I> arg9Default, Class<?> declaringClass, boolean isDisposedSignal){
    		super(arg9Default, declaringClass, isDisposedSignal);
			this.arg8Default = arg8Default;
    		if(this.arg8Default==null){
        		throw new QNoDefaultValueException(8);
        	}
    	}
    	
    	AbstractSignal9Default2(Supplier<H> arg8Default, Supplier<I> arg9Default, Class<?>... types) {
			super(arg9Default, types);
			this.arg8Default = arg8Default;
    		if(this.arg8Default==null){
        		throw new QNoDefaultValueException(8);
        	}
		}
    	
    	AbstractSignal9Default2(String signalName, Supplier<H> arg8Default, Supplier<I> arg9Default, Class<?>... types) {
			super(signalName, arg9Default, types);
			this.arg8Default = arg8Default;
    		if(this.arg8Default==null){
        		throw new QNoDefaultValueException(8);
        	}
		}
    	
    	private final Supplier<H> arg8Default;
        
        public void emit(A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7) {
       		emit(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8Default.get());
        }
    }
    
    public static abstract class AbstractSignal9Default3<A, B, C, D, E, F, G, H, I> extends AbstractSignal9Default2<A, B, C, D, E, F, G, H, I> {
    	
    	AbstractSignal9Default3(Supplier<G> arg7Default, Supplier<H> arg8Default, Supplier<I> arg9Default){
    		super(arg8Default, arg9Default);
			this.arg7Default = arg7Default;
    		if(this.arg7Default==null){
        		throw new QNoDefaultValueException(7);
        	}
    	}
    	
    	AbstractSignal9Default3(Supplier<G> arg7Default, Supplier<H> arg8Default, Supplier<I> arg9Default, Class<?> declaringClass, boolean isDisposedSignal){
    		super(arg8Default, arg9Default, declaringClass, isDisposedSignal);
			this.arg7Default = arg7Default;
    		if(this.arg7Default==null){
        		throw new QNoDefaultValueException(7);
        	}
    	}
    	
    	AbstractSignal9Default3(Supplier<G> arg7Default, Supplier<H> arg8Default, Supplier<I> arg9Default, Class<?>... types) {
			super(arg8Default, arg9Default, types);
			this.arg7Default = arg7Default;
    		if(this.arg7Default==null){
        		throw new QNoDefaultValueException(7);
        	}
		}
    	
    	AbstractSignal9Default3(String signalName, Supplier<G> arg7Default, Supplier<H> arg8Default, Supplier<I> arg9Default, Class<?>... types) {
			super(signalName, arg8Default, arg9Default, types);
			this.arg7Default = arg7Default;
    		if(this.arg7Default==null){
        		throw new QNoDefaultValueException(7);
        	}
		}
    	
    	private final Supplier<G> arg7Default;
        
        public void emit(A arg1, B arg2, C arg3, D arg4, E arg5, F arg6) {
       		emit(arg1, arg2, arg3, arg4, arg5, arg6, arg7Default.get());
        }
    }
    
    public static abstract class AbstractSignal9Default4<A, B, C, D, E, F, G, H, I> extends AbstractSignal9Default3<A, B, C, D, E, F, G, H, I> {
    	
    	AbstractSignal9Default4(Supplier<F> arg6Default, Supplier<G> arg7Default, Supplier<H> arg8Default, Supplier<I> arg9Default){
    		super(arg7Default, arg8Default, arg9Default);
			this.arg6Default = arg6Default;
    		if(this.arg6Default==null){
        		throw new QNoDefaultValueException(6);
        	}
    	}
    	
    	AbstractSignal9Default4(Supplier<F> arg6Default, Supplier<G> arg7Default, Supplier<H> arg8Default, Supplier<I> arg9Default, Class<?> declaringClass, boolean isDisposedSignal){
    		super(arg7Default, arg8Default, arg9Default, declaringClass, isDisposedSignal);
			this.arg6Default = arg6Default;
    		if(this.arg6Default==null){
        		throw new QNoDefaultValueException(6);
        	}
    	}
    	
    	AbstractSignal9Default4(Supplier<F> arg6Default, Supplier<G> arg7Default, Supplier<H> arg8Default, Supplier<I> arg9Default, Class<?>... types) {
			super(arg7Default, arg8Default, arg9Default, types);
			this.arg6Default = arg6Default;
    		if(this.arg6Default==null){
        		throw new QNoDefaultValueException(6);
        	}
		}
    	
    	AbstractSignal9Default4(String signalName, Supplier<F> arg6Default, Supplier<G> arg7Default, Supplier<H> arg8Default, Supplier<I> arg9Default, Class<?>... types) {
			super(signalName, arg7Default, arg8Default, arg9Default, types);
			this.arg6Default = arg6Default;
    		if(this.arg6Default==null){
        		throw new QNoDefaultValueException(6);
        	}
		}
    	
    	private final Supplier<F> arg6Default;
        
        public void emit(A arg1, B arg2, C arg3, D arg4, E arg5) {
       		emit(arg1, arg2, arg3, arg4, arg5, arg6Default.get());
        }
    }
    
    public static abstract class AbstractSignal9Default5<A, B, C, D, E, F, G, H, I> extends AbstractSignal9Default4<A, B, C, D, E, F, G, H, I> {
    	
    	AbstractSignal9Default5(Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default, Supplier<H> arg8Default, Supplier<I> arg9Default){
    		super(arg6Default, arg7Default, arg8Default, arg9Default);
			this.arg5Default = arg5Default;
    		if(this.arg5Default==null){
        		throw new QNoDefaultValueException(5);
        	}
    	}
    	
    	AbstractSignal9Default5(Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default, Supplier<H> arg8Default, Supplier<I> arg9Default, Class<?> declaringClass, boolean isDisposedSignal){
    		super(arg6Default, arg7Default, arg8Default, arg9Default, declaringClass, isDisposedSignal);
			this.arg5Default = arg5Default;
    		if(this.arg5Default==null){
        		throw new QNoDefaultValueException(5);
        	}
    	}
    	
    	AbstractSignal9Default5(Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default, Supplier<H> arg8Default, Supplier<I> arg9Default, Class<?>... types) {
			super(arg6Default, arg7Default, arg8Default, arg9Default, types);
			this.arg5Default = arg5Default;
    		if(this.arg5Default==null){
        		throw new QNoDefaultValueException(5);
        	}
		}
    	
    	AbstractSignal9Default5(String signalName, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default, Supplier<H> arg8Default, Supplier<I> arg9Default, Class<?>... types) {
			super(signalName, arg6Default, arg7Default, arg8Default, arg9Default, types);
			this.arg5Default = arg5Default;
    		if(this.arg5Default==null){
        		throw new QNoDefaultValueException(5);
        	}
		}
    	
    	private final Supplier<E> arg5Default;
        
        public void emit(A arg1, B arg2, C arg3, D arg4) {
       		emit(arg1, arg2, arg3, arg4, arg5Default.get());
        }
    }
    
    public static abstract class AbstractSignal9Default6<A, B, C, D, E, F, G, H, I> extends AbstractSignal9Default5<A, B, C, D, E, F, G, H, I> {
    	
    	AbstractSignal9Default6(Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default, Supplier<H> arg8Default, Supplier<I> arg9Default){
    		super(arg5Default, arg6Default, arg7Default, arg8Default, arg9Default);
			this.arg4Default = arg4Default;
    		if(this.arg4Default==null){
        		throw new QNoDefaultValueException(4);
        	}
    	}
    	
    	AbstractSignal9Default6(Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default, Supplier<H> arg8Default, Supplier<I> arg9Default, Class<?> declaringClass, boolean isDisposedSignal){
    		super(arg5Default, arg6Default, arg7Default, arg8Default, arg9Default, declaringClass, isDisposedSignal);
			this.arg4Default = arg4Default;
    		if(this.arg4Default==null){
        		throw new QNoDefaultValueException(4);
        	}
    	}
    	
    	AbstractSignal9Default6(Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default, Supplier<H> arg8Default, Supplier<I> arg9Default, Class<?>... types) {
			super(arg5Default, arg6Default, arg7Default, arg8Default, arg9Default, types);
			this.arg4Default = arg4Default;
    		if(this.arg4Default==null){
        		throw new QNoDefaultValueException(4);
        	}
		}
    	
    	AbstractSignal9Default6(String signalName, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default, Supplier<H> arg8Default, Supplier<I> arg9Default, Class<?>... types) {
			super(signalName, arg5Default, arg6Default, arg7Default, arg8Default, arg9Default, types);
			this.arg4Default = arg4Default;
    		if(this.arg4Default==null){
        		throw new QNoDefaultValueException(4);
        	}
		}
    	
    	private final Supplier<D> arg4Default;
        
        public void emit(A arg1, B arg2, C arg3) {
       		emit(arg1, arg2, arg3, arg4Default.get());
        }
    }
    
    public static abstract class AbstractSignal9Default7<A, B, C, D, E, F, G, H, I> extends AbstractSignal9Default6<A, B, C, D, E, F, G, H, I> {
    	
    	AbstractSignal9Default7(Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default, Supplier<H> arg8Default, Supplier<I> arg9Default){
    		super(arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, arg9Default);
			this.arg3Default = arg3Default;
    		if(this.arg3Default==null){
        		throw new QNoDefaultValueException(3);
        	}
    	}
    	
    	AbstractSignal9Default7(Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default, Supplier<H> arg8Default, Supplier<I> arg9Default, Class<?> declaringClass, boolean isDisposedSignal){
    		super(arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, arg9Default, declaringClass, isDisposedSignal);
			this.arg3Default = arg3Default;
    		if(this.arg3Default==null){
        		throw new QNoDefaultValueException(3);
        	}
    	}
    	
    	AbstractSignal9Default7(Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default, Supplier<H> arg8Default, Supplier<I> arg9Default, Class<?>... types) {
			super(arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, arg9Default, types);
			this.arg3Default = arg3Default;
    		if(this.arg3Default==null){
        		throw new QNoDefaultValueException(3);
        	}
		}
    	
    	AbstractSignal9Default7(String signalName, Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default, Supplier<H> arg8Default, Supplier<I> arg9Default, Class<?>... types) {
			super(signalName, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, arg9Default, types);
			this.arg3Default = arg3Default;
    		if(this.arg3Default==null){
        		throw new QNoDefaultValueException(3);
        	}
		}
    	
    	private final Supplier<C> arg3Default;
        
        public void emit(A arg1, B arg2) {
       		emit(arg1, arg2, arg3Default.get());
        }
    }
    
    public static abstract class AbstractSignal9Default8<A, B, C, D, E, F, G, H, I> extends AbstractSignal9Default7<A, B, C, D, E, F, G, H, I> {
    	
    	AbstractSignal9Default8(Supplier<B> arg2Default, Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default, Supplier<H> arg8Default, Supplier<I> arg9Default){
    		super(arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, arg9Default);
			this.arg2Default = arg2Default;
    		if(this.arg2Default==null){
        		throw new QNoDefaultValueException(2);
        	}
    	}
    	
    	AbstractSignal9Default8(Supplier<B> arg2Default, Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default, Supplier<H> arg8Default, Supplier<I> arg9Default, Class<?> declaringClass, boolean isDisposedSignal){
    		super(arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, arg9Default, declaringClass, isDisposedSignal);
			this.arg2Default = arg2Default;
    		if(this.arg2Default==null){
        		throw new QNoDefaultValueException(2);
        	}
    	}
    	
    	AbstractSignal9Default8(Supplier<B> arg2Default, Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default, Supplier<H> arg8Default, Supplier<I> arg9Default, Class<?>... types) {
			super(arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, arg9Default, types);
			this.arg2Default = arg2Default;
    		if(this.arg2Default==null){
        		throw new QNoDefaultValueException(2);
        	}
		}
    	
    	AbstractSignal9Default8(String signalName, Supplier<B> arg2Default, Supplier<C> arg3Default, Supplier<D> arg4Default, Supplier<E> arg5Default, Supplier<F> arg6Default, Supplier<G> arg7Default, Supplier<H> arg8Default, Supplier<I> arg9Default, Class<?>... types) {
			super(signalName, arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, arg9Default, types);
			this.arg2Default = arg2Default;
    		if(this.arg2Default==null){
        		throw new QNoDefaultValueException(2);
        	}
		}
    	
    	private final Supplier<B> arg2Default;
        
        public void emit(A arg1) {
       		emit(arg1, arg2Default.get());
        }
    }
	
	public static AbstractPrivateSignal0 findSignal(QObject sender, String name) {
		return (AbstractPrivateSignal0)findSignalImpl(sender, name);
	}
	
	@SuppressWarnings("unchecked")
	public static <A> AbstractPrivateSignal1<A> findSignal(QObject sender, String name, Class<A> typeA) {
		return (AbstractPrivateSignal1<A>)findSignalImpl(sender, name, typeA);
	}
	
	@SuppressWarnings("unchecked")
	public static <A,B> AbstractPrivateSignal2<A,B> findSignal(
			QObject sender, String name, 
			Class<A> typeA, Class<B> typeB) {
		return (AbstractPrivateSignal2<A,B>)findSignalImpl(sender, 
				name, typeA, typeB);
	}
	
	@SuppressWarnings("unchecked")
	public static <A,B,C> AbstractPrivateSignal3<A,B,C> findSignal(
			QObject sender, String name, 
			Class<A> typeA, Class<B> typeB, Class<C> typeC) {
		return (AbstractPrivateSignal3<A,B,C>)findSignalImpl(sender, 
				name, typeA, typeB, typeC);
	}
	
	@SuppressWarnings("unchecked")
	public static <A,B,C,D> AbstractPrivateSignal4<A,B,C,D> findSignal(
			QObject sender, String name, 
			Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD) {
		return (AbstractPrivateSignal4<A,B,C,D>)findSignalImpl(sender, 
				name, typeA, typeB, typeC, typeD);
	}
	
	@SuppressWarnings("unchecked")
	public static <A,B,C,D,E> AbstractPrivateSignal5<A,B,C,D,E> findSignal(
			QObject sender, String name, 
			Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE) {
		return (AbstractPrivateSignal5<A,B,C,D,E>)findSignalImpl(sender, 
				name, typeA, typeB, typeC, typeD, typeE);
	}
	
	@SuppressWarnings("unchecked")
	public static <A,B,C,D,E,F> AbstractPrivateSignal6<A,B,C,D,E,F> findSignal(
			QObject sender, String name, 
			Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Class<F> typeF) {
		return (AbstractPrivateSignal6<A,B,C,D,E,F>)findSignalImpl(sender, 
				name, typeA, typeB, typeC, typeD, typeE, typeF);
	}
	
	@SuppressWarnings("unchecked")
	public static <A,B,C,D,E,F,G> AbstractPrivateSignal7<A,B,C,D,E,F,G> findSignal(
			QObject sender, String name, 
			Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Class<F> typeF, Class<G> typeG) {
		return (AbstractPrivateSignal7<A,B,C,D,E,F,G>)findSignalImpl(sender, 
				name, typeA, typeB, typeC, typeD, typeE, typeF, typeG);
	}
	
	@SuppressWarnings("unchecked")
	public static <A,B,C,D,E,F,G,H> AbstractPrivateSignal8<A,B,C,D,E,F,G,H> findSignal(
			QObject sender, String name, 
			Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Class<F> typeF, Class<G> typeG, Class<H> typeH) {
		return (AbstractPrivateSignal8<A,B,C,D,E,F,G,H>)findSignalImpl(sender, 
				name, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH);
	}
	
	@SuppressWarnings("unchecked")
	public static <A,B,C,D,E,F,G,H,I> AbstractPrivateSignal9<A,B,C,D,E,F,G,H,I> findSignal(
			QObject sender, String name, 
			Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Class<F> typeF, Class<G> typeG, Class<H> typeH, Class<I> typeI) {
		return (AbstractPrivateSignal9<A,B,C,D,E,F,G,H,I>)findSignalImpl(sender, 
				name, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH, typeI);
	}
	
	public static AbstractSignal findSignal(QObject sender, String name, Class<?>... types){
		return findSignalImpl(sender, name, types);		
	}
	
	private static AbstractSignal findSignalImpl(QObject sender, String name, Class<?>... types){
		QtJambiInternal.disposedCheck(sender);
		return QtJambiInternal.findSignal(sender, name, types);
	}
	
	static QMetaObject forGadget(Object gadget) {
    	if(gadget instanceof QtGadget) {
    		return ((QtGadget) gadget).staticMetaObject();
    	}else {
    		return QMetaObject.forType(gadget.getClass());
    	}
    }

	QtAbstractEnumerator getEnumEntry(int index, int value) {
		if(enumEntries!=null) {
			EnumEntries ee = enumEntries.get(index);
			if(ee==null) {
				return null;
			}else{
				return ee.enumeratorConstantDirectory().get(value);
			}
		}
		return null;
	}
}
