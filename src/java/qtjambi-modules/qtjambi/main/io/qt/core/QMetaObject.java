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

import static io.qt.core.QMetaMethod.fromReflectedMethod;
import static io.qt.internal.QtJambiInternal.cppNormalizedSignature;
import static io.qt.internal.QtJambiInternal.internalNameOfArgumentType;

import java.io.Serializable;
import java.lang.reflect.InvocationHandler;
import java.lang.reflect.Method;
import java.lang.reflect.Modifier;
import java.lang.reflect.ParameterizedType;
import java.lang.reflect.Proxy;
import java.lang.reflect.Type;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Objects;
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
import io.qt.QtObject;
import io.qt.QtObjectInterface;
import io.qt.QtSignalEmitterInterface;
import io.qt.QtThreadAffineInterface;
import io.qt.QtUninvokable;
import io.qt.QtUtilities;
import io.qt.internal.QtJambiPropertyInfo;
import io.qt.internal.QtJambiInternal;
import io.qt.internal.QtJambiSignals;

/**
 * <p>Java wrapper for Qt class <a href="https://doc.qt.io/qt/qmetaobject.html">QMetaObject</a></p>
 */
public final class QMetaObject {
    static {
    	QtJambi_LibraryUtilities.initialize();
    }
    
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

        @QtUninvokable
        Map<String,QtAbstractEnumerator> enumConstantDirectory() {
            if(enumConstantDirectory==null) {
                enumConstantDirectory = new TreeMap<>();
                for(QtEnumerator entry : entries) {
                    enumConstantDirectory.put(entry.name(), entry);
                }
            }
            return enumConstantDirectory;
        }
        
        @QtUninvokable
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
    
    @QtUninvokable
    synchronized Class<?> enumType(QMetaEnum enumerator){
        if(enumClasses==null)
            enumClasses = new Class[enumeratorCount(metaObjectPointer)];
        if(enumClasses[enumeratorIndex(enumerator)]==null) {
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
                    QMetaEnum enmType = findEnumForFlags(metaObjectPointer, enumeratorIndex(enumerator));
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
            enumClasses[enumeratorIndex(enumerator)] = isFlag ? flagsClass : enumeratorClass;
        }
        return enumClasses[enumeratorIndex(enumerator)];
    }
    
    @QtUninvokable
    private void generateEnumEntries(QMetaEnum enumerator) {
        if(enumEntries==null)
            enumEntries = new HashMap<>();
        ClassLoader cl = type().getClassLoader();
        Class<?>[] ifc = new Class[] {QtEnumerator.class};
        QtEnumerator[] entries = new QtEnumerator[enumerator.keyCount()];
        for (int i = 0; i < entries.length; i++) {
            entries[i] = (QtEnumerator)Proxy.newProxyInstance(cl, ifc, new Enumerator(i, enumerator.key(i), enumerator.value(i)));
        }
        enumEntries.put(enumeratorIndex(enumerator), new EnumEntries(entries));
    }
    
    @QtUninvokable
    QtAbstractEnumerator[] enumEntries(QMetaEnum enumerator) {
        QtAbstractEnumerator[] entries;
        Class<?> type = enumerator.type();
        if(type.isEnum()) {
            entries = (QtAbstractEnumerator[])type.getEnumConstants();
        }else if(enumEntries!=null){
            EnumEntries ee = enumEntries.get(enumeratorIndex(enumerator));
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
    
    @QtUninvokable
    QtAbstractEnumerator enumEntry(QMetaEnum enumerator, int index) {
        Class<?> type = enumerator.type();
        if(type.isEnum()) {
            return (QtAbstractEnumerator)type.getEnumConstants()[index];
        }else if(enumEntries!=null){
            EnumEntries ee = enumEntries.get(enumeratorIndex(enumerator));
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
    @QtUninvokable
    QtAbstractEnumerator enumEntry(QMetaEnum enumerator, String name) {
        Class<?> type = enumerator.type();
        if(type.isEnum()) {
            return (QtAbstractEnumerator)Enum.valueOf((Class<Enum>)type, name);
        }else if(enumEntries!=null){
            EnumEntries ee = enumEntries.get(enumeratorIndex(enumerator));
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
        @QtUninvokable
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
    
    @QtUninvokable
    private static native QMetaEnum findEnumForFlags(long metaObjectPointer, int enumeratorIndex);
    
    @QtUninvokable
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
    
    @QtUninvokable
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
    
    /**
     * Represents a handle to a signal-slot (or signal-functor) connection.
     * 
     * It can be used to check if the connection is valid and to disconnect it using {@link QObject#disconnect(Connection)}. 
     * For a signal-functor connection without a context object, it is the only way to selectively disconnect that connection.
     * 
     * As Connection is just a handle, the underlying signal-slot connection is unaffected when Connection is destroyed or reassigned.
     */
    public static interface Connection{
        /**
         * Returns true if the connection is valid.
         */
        public boolean isConnected();
        
        /**
         * Provides the sender of the connected signal.
         * @return sender
         */
        public QtSignalEmitterInterface sender();
        
        /**
         * Provides the receiver of the signal-slot connection.
         * @return receiver
         */
        public Object receiver();
    }
    
    /**
     * Enum representing meta calls.
     */
    public enum Call implements QtEnumerator{
        InvokeMetaMethod(0),
        ReadProperty(1),
        WriteProperty(2),
        ResetProperty(3),
        CreateInstance(QtJambiInternal.majorVersion()>5 ? 9 : 4),
        IndexOfMethod(QtJambiInternal.majorVersion()>5 ? 10 : 5),
        RegisterPropertyMetaType(QtJambiInternal.majorVersion()>5 ? 11 : 6),
        RegisterMethodArgumentMetaType(QtJambiInternal.majorVersion()>5 ? 12 : 7);
        
        private Call(int value) { this.value = value; }
        
        private final int value;
        
        public static Call resolve(int value) {
            switch (value) {
            case 0: return InvokeMetaMethod;
            case 1: return ReadProperty;
            case 2: return WriteProperty;
            case 3: return ResetProperty;
            case 4: 
                if(QtJambiInternal.majorVersion()>5)
                    return CreateInstance;
                else return QtJambi_LibraryUtilities.internal.resolveEnum(Call.class, value, "QueryPropertyDesignable");
            case 5: 
                if(QtJambiInternal.majorVersion()>5)
                    return IndexOfMethod;
                else return QtJambi_LibraryUtilities.internal.resolveEnum(Call.class, value, "QueryPropertyScriptable");
            case 6: 
                if(QtJambiInternal.majorVersion()>5)
                    return RegisterPropertyMetaType;
                else return QtJambi_LibraryUtilities.internal.resolveEnum(Call.class, value, "QueryPropertyStored");
            case 7: 
                if(QtJambiInternal.majorVersion()>5)
                    return RegisterMethodArgumentMetaType;
                else return QtJambi_LibraryUtilities.internal.resolveEnum(Call.class, value, "QueryPropertyEditable");
            case 8:
                if(QtJambiInternal.majorVersion()>5)
                    return QtJambi_LibraryUtilities.internal.resolveEnum(Call.class, value, "RegisterQPropertyObserver");
                else return QtJambi_LibraryUtilities.internal.resolveEnum(Call.class, value, "QueryPropertyUser");
            case 9: 
                if(QtJambiInternal.majorVersion()>5)
                    return QtJambi_LibraryUtilities.internal.resolveEnum(Call.class, value, "SetQPropertyBinding");
                else return CreateInstance;
            case 10: 
                if(QtJambiInternal.majorVersion()>5)
                    throw new io.qt.QNoSuchEnumValueException(value);
                else return IndexOfMethod;
            case 11: 
                if(QtJambiInternal.majorVersion()>5)
                    throw new io.qt.QNoSuchEnumValueException(value);
                else return RegisterPropertyMetaType;
            case 12: 
                if(QtJambiInternal.majorVersion()>5)
                    throw new io.qt.QNoSuchEnumValueException(value);
                else return RegisterMethodArgumentMetaType;
            default: throw new io.qt.QNoSuchEnumValueException(value);
            }
        }

        @Override
        public int value() {
            return value;
        }
    }
    
    @QtUninvokable
    public QMetaProperty userProperty() {
        return userProperty(this.metaObjectPointer);
    }
    @QtUninvokable
    private native QMetaProperty userProperty(long metaObjectPointer);
    
    @QtUninvokable
    public QList<io.qt.core.QMetaProperty> properties() {
        return properties(this.metaObjectPointer);
    }
    @QtUninvokable
    private native QList<QMetaProperty> properties(long metaObjectPointer);
    
    @QtUninvokable
    public int propertyCount() {
    	return propertyCount(metaObjectPointer);
    }
    @QtUninvokable
    private static native int propertyCount(long metaObjectPointer);
    
    @QtUninvokable
    public QMetaProperty property(String name) {
        return property(this.metaObjectPointer, name);
    }
    @QtUninvokable
    private native QMetaProperty property(long metaObjectPointer, String name);
    
    @QtUninvokable
    public QList<io.qt.core.QMetaEnum> enumerators() {
        return enumerators(this.metaObjectPointer);
    }
    @QtUninvokable
    private static native QList<QMetaEnum> enumerators(long metaObjectPointer);
    
    @QtUninvokable
    public int enumeratorCount() {
    	return enumeratorCount(metaObjectPointer);
    }
    @QtUninvokable
    private static native int enumeratorCount(long metaObjectPointer);
    
    @QtUninvokable
    public io.qt.core.QMetaEnum enumerator(String name) {
        return enumerator(this.metaObjectPointer, name);
    }
    @QtUninvokable
    private static native QMetaEnum enumerator(long metaObjectPointer, String name);
    
    @QtUninvokable
    public static void connectSlotsByName(QObject object) {
        connectSlotsByName(QtJambi_LibraryUtilities.internal.checkedNativeId(object));
    }
    
    @QtUninvokable
    private static native void connectSlotsByName(long object);
            
    @QtUninvokable
    static native QMetaObject.Connection connect(QObject sender, String signal, QObject receiver, String slot, byte connection);
    
    @QtUninvokable
    static native QMetaObject.Connection connectMethods(QObject sender, int signalIdx, long signalEnclosingMetaObject, QObject receiver, int slotIdx, long EnclosingMetaObject, byte connection);
    
    @QtUninvokable
    static native boolean disconnect(QObject sender, String signal, QObject receiver, String slot);
    
    @QtUninvokable
    static native boolean disconnectMethods(QObject sender, int signalIdx, long signalEnclosingMetaObject, QObject receiver, int slotIdx, long EnclosingMetaObject);
    
    static String internalNameOfType(Class<? extends Object> cls) {
    	return QtJambiInternal.internalNameOfArgumentType(cls);
    }
    
    @QtUninvokable
    QMetaMethod methodByIndex(int methodIndex) {
        return methodByIndex(metaObjectPointer, methodIndex);
    }
    @QtUninvokable
    private native QMetaMethod methodByIndex(long metaObjectPointer, int index);
    
    @QtUninvokable
    public QMetaMethod method(String name, Class<?>... parameterTypes) {
    	if(name==null)
    		return null;
        QMetaMethod method;
        int idx = name.indexOf('(');
        if(parameterTypes.length==0 && idx>0) {
        	int spacePos = name.substring(0, idx).trim().lastIndexOf(' ');
			if (idx > spacePos && spacePos > 0)
				throw new RuntimeException(String.format("Do not specify return type in slot signature: '%1$s'", name));
        	String cppNormalizedSignature = cppNormalizedSignature(name, this.type());
            method = method(metaObjectPointer, cppNormalizedSignature);
            if(method==null)
                method = method(metaObjectPointer, cppNormalizedSignature+"const");
        }else {
            StringBuilder args = new StringBuilder();
            for(int i=0; i<parameterTypes.length; ++i) {
                if(i!=0)
                    args.append(',');
                String typeName = internalNameOfArgumentType(parameterTypes[i]);
                if(typeName.isEmpty())
                    args.append("QVariant");
                else
                    args.append(typeName);
            }
            method = method(metaObjectPointer, String.format("%1$s(%2$s)", name, args));
        }
        if(method==null || !method.isValid()) {
            for(QMetaMethod m : methods(metaObjectPointer)) {
                if(m.name().toString().equals(name)) {
                	if(m.parameterClassTypes().equals(Arrays.asList(parameterTypes))) {
	                    method = m;
	                    break;
                	}else if(idx<0 && m.parameterClassTypes().size()==parameterTypes.length) {
                		method = null;
                		for(int i=0; i<parameterTypes.length; ++i) {
                			if(m.parameterClassTypes().get(i).isAssignableFrom(parameterTypes[i])) {
        	                    method = m;
        	                    break;                				
                			}
                		}
                		if(method!=null)
                			break;
                	}
                }
            }
        }
        return method;
    }
    
    @QtUninvokable
    private native QMetaMethod method(long metaObjectPointer, String normalizedSignature);
    
    @QtUninvokable
    public QList<QMetaMethod> methods(){
        return methods(metaObjectPointer);
    }    
    @QtUninvokable
    private native QList<QMetaMethod> methods(long metaObjectPointer);
    
    @QtUninvokable
    public final int methodCount(){
        return methodCount(metaObjectPointer);
    }
    @QtUninvokable
    private native int methodCount(long metaObjectPointer);
    
    @QtUninvokable
    public QMetaMethod constructor(Class<?>... parameterTypes) {
        StringBuilder args = new StringBuilder();
        for(int i=0; i<parameterTypes.length; ++i) {
            if(i!=0)
                args.append(',');
            String typeName = internalNameOfArgumentType(parameterTypes[i]);
            if(typeName.isEmpty())
                args.append("QVariant");
            else
                args.append(typeName);
        }
        QMetaMethod method = constructor(metaObjectPointer, String.format("%1$s(%2$s)", className(), args));
        if(method==null || !method.isValid()) {
            for(QMetaMethod m : constructors(metaObjectPointer)) {
                if(m.parameterClassTypes().equals(Arrays.asList(parameterTypes))) {
                    method = m;
                    break;
                }
            }
        }
        return method;
    }
    @QtUninvokable
    private native QMetaMethod constructor(long metaObjectPointer, String normalizedSignature);
    
    @QtUninvokable
    public final QList<QMetaMethod> constructors(){
        return constructors(metaObjectPointer);
    }
    @QtUninvokable
    private native QList<QMetaMethod> constructors(long metaObjectPointer);
    
    @QtUninvokable
    public final int constructorCount(){
        return constructorCount(metaObjectPointer);
    }
    @QtUninvokable
    private native int constructorCount(long metaObjectPointer);
    
    @QtUninvokable
    public QObject newInstance(Object... args) throws NoSuchMethodException {
        if (!inherits(QObject.staticMetaObject)) {
            throw new RuntimeException(String.format("Type %1$s does not inherit QObject", className()));
        }
        if(constructorCount()==0) {
            throw new UnsupportedOperationException("No constructors available.");
        }
        QMetaMethod constr = null;
        QList<QMetaMethod> constructors = constructors();
        for(QMetaMethod constructor : constructors.clone()) {
            List<Class<?>> parameterTypes = constructor.parameterClassTypes();
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
        List<Class<?>> parameterTypes = constr.parameterClassTypes();
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
    
    @QtUninvokable
    private static native QObject newInstanceImpl(long metaObjectPointer, int methodIndex, String[] argTypes, Object[] args);
    
    @QtUninvokable
    public QMetaObject superClass(){
        return superClass(metaObjectPointer);
    }
    @QtUninvokable
    private static native QMetaObject superClass(long metaObjectPointer);
    
    @QtUninvokable
    public String className(){
        return className(metaObjectPointer);
    }
    @QtUninvokable
    private static native String className(long metaObjectPointer);
    
    @QtUninvokable
    public QList<QPair<String,String>> classInfos(){
        return classInfos(metaObjectPointer);
    }
    @QtUninvokable
    private static native QList<QPair<String,String>> classInfos(long metaObjectPointer);
    
    @QtUninvokable
    public int classInfoCount() {
    	return classInfoCount(metaObjectPointer);
    }
    @QtUninvokable
    private static native int classInfoCount(long metaObjectPointer);
    
    @QtUninvokable
    public String classInfo(String name){
        return classInfo(metaObjectPointer, name);
    }
    @QtUninvokable
    private static native String classInfo(long metaObjectPointer, String name);

    /**
     * Returns true if the class described by this QMetaObject inherits the type described by metaObject; otherwise returns false.
     * A type is considered to inherit itself.
     * @param metaObject
     * @return inherits
     */
    @QtUninvokable
    public boolean inherits(QMetaObject metaObject){
        return metaObject!=null && inherits(metaObjectPointer, metaObject.metaObjectPointer);
    }
    @QtUninvokable
    private static native boolean inherits(long metaObjectPointer, long other);

    /**
     * Casts an object to the given <i>targetType</i>. Returns null if object is not instance of <i>targetType</i>.
     * @param <T> type
     * @param targetType
     * @param object
     * @return the object as targetType or null
     */
    @QtUninvokable
    public static <T extends QtObjectInterface> T cast(Class<T> targetType, QtObjectInterface object) {
        io.qt.QtUtilities.initializePackage(targetType);
        if(object==null || targetType.isInstance(object)) {
            return targetType.cast(object);
        }
        return targetType.cast(cast(object, targetType));
    }
    @QtUninvokable
    private static native Object cast(QtObjectInterface object, Class<?> targetType);
    
    @QtUninvokable
    public Class<?> type(){
        return type(metaObjectPointer);
    }
    @QtUninvokable
    private static native Class<?> type(long metaObjectPointer);

    @QtUninvokable
    public static QMetaObject forType(Class<?> clazz) {
        QtUtilities.initializePackage(clazz);
        return __qt_forType(clazz);
    }
    @QtUninvokable
    private static native QMetaObject __qt_forType(Class<?> clazz);
        
    @QtUninvokable
    private static native Class<?> exactType(long metaObjectPointer);
                    
    @QtUninvokable
    public static boolean checkConnectArgs(String signal, String method) {
        return checkConnectArgsString(io.qt.internal.QtJambiInternal.SignalPrefix+cppNormalizedSignature(signal, null), io.qt.internal.QtJambiInternal.SlotPrefix+cppNormalizedSignature(method, null));
    }
    @QtUninvokable
    private native static boolean checkConnectArgsString(String signal, String method);
    
    @QtUninvokable
    public static boolean checkConnectArgs(QMetaMethod signal, QMetaMethod method){
        return checkConnectArgsMethods(signal.enclosingMetaObject().metaObjectPointer, signal.methodIndex(), method.enclosingMetaObject().metaObjectPointer, method.methodIndex());
    }
    @QtUninvokable
    private static native boolean checkConnectArgsMethods(long signalMetaObjectPointer, int signalMethodIndex, long methodMetaObjectPointer, int methodMethodIndex);
    
    /**
     * Calling <code>invokeMethod(obj, AutoConnection, args)</code>.
     * @param obj object
     * @param member method name
     * @param args arguments
     * @return method result value if any - if the invocation is asynchronous, the return value cannot be evaluated.
     * @throws QUnsuccessfulInvocationException if not able to invoke slot
     * @throws QNoSuchMethodException if method not available
     */
    @QtUninvokable
    public static Object invokeMethod(QObject obj, String member, Object... args) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
        return invokeMethod(obj, member, Qt.ConnectionType.AutoConnection, args);
    }
    
    /**
     * <p>Invokes the given method on the given object and returns it's result value.</p>
     * 
     * <p>The invocation can be either synchronous or asynchronous, depending on type:</p>
     * <ul>
     * <li>If type is {@link Qt.ConnectionType#DirectConnection}, the member will be invoked immediately.</li>
     * <li>If type is {@link Qt.ConnectionType#QueuedConnection}, a QEvent will be sent and the member is invoked as soon as the application enters the main event loop.</li>
     * <li>If type is {@link Qt.ConnectionType#BlockingQueuedConnection}, the method will be invoked in the same way as for {@link Qt.ConnectionType#QueuedConnection}, except that the current thread will block until the event is delivered. Using this connection type to communicate between objects in the same thread will lead to deadlocks.</li>
     * <li>If type is {@link Qt.ConnectionType#AutoConnection}, the member is invoked synchronously if obj lives in the same thread as the caller; otherwise it will invoke the member asynchronously.</li>
     * </ul>
     * <p>If the invocation is asynchronous, the return value cannot be evaluated.</p>
     * 
     * @param obj object
     * @param member method name
     * @param args arguments
     * @return method result value if any - if the invocation is asynchronous, the return value cannot be evaluated.
     * @throws QUnsuccessfulInvocationException if not able to invoke slot
     * @throws QNoSuchMethodException if method not available
     */
    @QtUninvokable
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
        if(method==null || !method.isValid()) {
            throw new QNoSuchMethodException(member);
        }
        return method.invoke(obj, type, args);
    }
    
    /**
     * Calling <code>invokeMethod(method, AutoConnection)</code>.
     * 
     * @param <R> The return type of the method.
     * @param method invoked method
     * @return method result value - if the invocation is asynchronous, the return value cannot be evaluated.
     * @throws QUnsuccessfulInvocationException if not able to invoke slot
     */
    @QtUninvokable
    public static <R> R invokeMethod(Method0<R> method) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
        return invokeMethod(method, Qt.ConnectionType.AutoConnection);
    }
    
    /**
     * <p>Invokes the method and returns it's result value.</p>
     * 
     * <p>The invocation can be either synchronous or asynchronous, depending on type:</p>
     * <ul>
     * <li>If type is {@link Qt.ConnectionType#DirectConnection}, the member will be invoked immediately.</li>
     * <li>If type is {@link Qt.ConnectionType#QueuedConnection}, a QEvent will be sent and the member is invoked as soon as the application enters the main event loop.</li>
     * <li>If type is {@link Qt.ConnectionType#BlockingQueuedConnection}, the method will be invoked in the same way as for {@link Qt.ConnectionType#QueuedConnection}, except that the current thread will block until the event is delivered. Using this connection type to communicate between objects in the same thread will lead to deadlocks.</li>
     * <li>If type is {@link Qt.ConnectionType#AutoConnection}, the member is invoked synchronously if obj lives in the same thread as the caller; otherwise it will invoke the member asynchronously.</li>
     * </ul>
     * <p>If the invocation is asynchronous, the return value cannot be evaluated.</p>
     * 
     * @param <R> The return type of the method.
     * @param method invoked method
     * @param type synchronous or asynchronous invokation
     * @return method result value - if the invocation is asynchronous, the return value cannot be evaluated.
     * @throws QUnsuccessfulInvocationException if not able to invoke slot
     */
    @SuppressWarnings("unchecked")
    @QtUninvokable
    public static <R> R invokeMethod(Method0<R> method, Qt.ConnectionType type) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
        QtJambiInternal.LambdaInfo info = QtJambiInternal.lamdaInfo(method);
        if(info!=null && info.qobject!=null && !info.qobject.isDisposed() && info.reflectiveMethod!=null) {
            QMetaMethod qmethod = fromReflectedMethod(info.reflectiveMethod);
            if(qmethod!=null && qmethod.isValid() && qmethod.parameterTypes().size()==info.lambdaArgs.size()) {
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
    
    /**
     * Calling <code>invokeMethod(method, AutoConnection, ...)</code>.
     * 
     * @param <A> The type of the first parameter of the method.
     * @param <R> The return type of the method.
     * @param method invoked method
     * @param arg1 Argument for the first parameter.
     * @return method result value - if the invocation is asynchronous, the return value cannot be evaluated.
     * @throws QUnsuccessfulInvocationException if not able to invoke slot
     */
    @QtUninvokable
    public static <A,R> R invokeMethod(Method1<A,R> method, A arg1) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
        return invokeMethod(method, Qt.ConnectionType.AutoConnection, arg1);
    }
    
    /**
     * <p>Invokes the method and returns it's result value.</p>
     * 
     * <p>The invocation can be either synchronous or asynchronous, depending on type:</p>
     * <ul>
     * <li>If type is {@link Qt.ConnectionType#DirectConnection}, the member will be invoked immediately.</li>
     * <li>If type is {@link Qt.ConnectionType#QueuedConnection}, a QEvent will be sent and the member is invoked as soon as the application enters the main event loop.</li>
     * <li>If type is {@link Qt.ConnectionType#BlockingQueuedConnection}, the method will be invoked in the same way as for {@link Qt.ConnectionType#QueuedConnection}, except that the current thread will block until the event is delivered. Using this connection type to communicate between objects in the same thread will lead to deadlocks.</li>
     * <li>If type is {@link Qt.ConnectionType#AutoConnection}, the member is invoked synchronously if obj lives in the same thread as the caller; otherwise it will invoke the member asynchronously.</li>
     * </ul>
     * <p>If the invocation is asynchronous, the return value cannot be evaluated.</p>
     * 
     * @param <A> The type of the first parameter of the method.
     * @param <R> The return type of the method.
     * @param method invoked method
     * @param type synchronous or asynchronous invokation
     * @param arg1 Argument for the first parameter.
     * @return method result value - if the invocation is asynchronous, the return value cannot be evaluated.
     * @throws QUnsuccessfulInvocationException if not able to invoke slot
     */
    @SuppressWarnings("unchecked")
    @QtUninvokable
    public static <A,R> R invokeMethod(Method1<A,R> method, Qt.ConnectionType type, A arg1) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
        QtJambiInternal.LambdaInfo info = QtJambiInternal.lamdaInfo(method);
        if(info!=null && info.qobject!=null && !info.qobject.isDisposed() && info.reflectiveMethod!=null) {
            QMetaMethod qmethod = fromReflectedMethod(info.reflectiveMethod);
            if(qmethod!=null && qmethod.isValid() && qmethod.parameterTypes().size()==info.lambdaArgs.size()+1) {
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
    
    /**
     * Calling <code>invokeMethod(method, AutoConnection, ...)</code>.
     * 
     * @param <A> The type of the first parameter of the method.
     * @param <B> The type of the second parameter of the method.
     * @param <R> The return type of the method.
     * @param method invoked method
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @return method result value - if the invocation is asynchronous, the return value cannot be evaluated.
     * @throws QUnsuccessfulInvocationException if not able to invoke slot
     */
    @QtUninvokable
    public static <A,B,R> R invokeMethod(Method2<A,B,R> method, A arg1, B arg2) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
        return invokeMethod(method, Qt.ConnectionType.AutoConnection, arg1, arg2);
    }
    
    /**
     * <p>Invokes the method and returns it's result value.</p>
     * 
     * <p>The invocation can be either synchronous or asynchronous, depending on type:</p>
     * <ul>
     * <li>If type is {@link Qt.ConnectionType#DirectConnection}, the member will be invoked immediately.</li>
     * <li>If type is {@link Qt.ConnectionType#QueuedConnection}, a QEvent will be sent and the member is invoked as soon as the application enters the main event loop.</li>
     * <li>If type is {@link Qt.ConnectionType#BlockingQueuedConnection}, the method will be invoked in the same way as for {@link Qt.ConnectionType#QueuedConnection}, except that the current thread will block until the event is delivered. Using this connection type to communicate between objects in the same thread will lead to deadlocks.</li>
     * <li>If type is {@link Qt.ConnectionType#AutoConnection}, the member is invoked synchronously if obj lives in the same thread as the caller; otherwise it will invoke the member asynchronously.</li>
     * </ul>
     * <p>If the invocation is asynchronous, the return value cannot be evaluated.</p>
     * 
     * @param <A> The type of the first parameter of the method.
     * @param <B> The type of the second parameter of the method.
     * @param <R> The return type of the method.
     * @param method invoked method
     * @param type synchronous or asynchronous invokation
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @return method result value - if the invocation is asynchronous, the return value cannot be evaluated.
     * @throws QUnsuccessfulInvocationException if not able to invoke slot
     */
    @SuppressWarnings("unchecked")
    @QtUninvokable
    public static <A,B,R> R invokeMethod(Method2<A,B,R> method, Qt.ConnectionType type, A arg1, B arg2) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
        QtJambiInternal.LambdaInfo info = QtJambiInternal.lamdaInfo(method);
        if(info!=null && info.qobject!=null && !info.qobject.isDisposed() && info.reflectiveMethod!=null) {
            QMetaMethod qmethod = fromReflectedMethod(info.reflectiveMethod);
            if(qmethod!=null && qmethod.isValid() && qmethod.parameterTypes().size()==info.lambdaArgs.size()+2) {
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
    
    /**
     * Calling <code>invokeMethod(method, AutoConnection, ...)</code>.
     * 
     * @param <A> The type of the first parameter of the method.
     * @param <B> The type of the second parameter of the method.
     * @param <C> The type of the third parameter of the method.
     * @param <R> The return type of the method.
     * @param method invoked method
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @return method result value - if the invocation is asynchronous, the return value cannot be evaluated.
     * @throws QUnsuccessfulInvocationException if not able to invoke slot
     */
    @QtUninvokable
    public static <A,B,C,R> R invokeMethod(Method3<A,B,C,R> method, A arg1, B arg2, C arg3) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
        return invokeMethod(method, Qt.ConnectionType.AutoConnection, arg1, arg2, arg3);
    }
    
    /**
     * <p>Invokes the method and returns it's result value.</p>
     * 
     * <p>The invocation can be either synchronous or asynchronous, depending on type:</p>
     * <ul>
     * <li>If type is {@link Qt.ConnectionType#DirectConnection}, the member will be invoked immediately.</li>
     * <li>If type is {@link Qt.ConnectionType#QueuedConnection}, a QEvent will be sent and the member is invoked as soon as the application enters the main event loop.</li>
     * <li>If type is {@link Qt.ConnectionType#BlockingQueuedConnection}, the method will be invoked in the same way as for {@link Qt.ConnectionType#QueuedConnection}, except that the current thread will block until the event is delivered. Using this connection type to communicate between objects in the same thread will lead to deadlocks.</li>
     * <li>If type is {@link Qt.ConnectionType#AutoConnection}, the member is invoked synchronously if obj lives in the same thread as the caller; otherwise it will invoke the member asynchronously.</li>
     * </ul>
     * <p>If the invocation is asynchronous, the return value cannot be evaluated.</p>
     * 
     * @param <A> The type of the first parameter of the method.
     * @param <B> The type of the second parameter of the method.
     * @param <C> The type of the third parameter of the method.
     * @param <R> The return type of the method.
     * @param method invoked method
     * @param type synchronous or asynchronous invokation
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @return method result value - if the invocation is asynchronous, the return value cannot be evaluated.
     * @throws QUnsuccessfulInvocationException if not able to invoke slot
     */
    @SuppressWarnings("unchecked")
    @QtUninvokable
    public static <A,B,C,R> R invokeMethod(Method3<A,B,C,R> method, Qt.ConnectionType type, A arg1, B arg2, C arg3) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
        QtJambiInternal.LambdaInfo info = QtJambiInternal.lamdaInfo(method);
        if(info!=null && info.qobject!=null && !info.qobject.isDisposed() && info.reflectiveMethod!=null) {
            QMetaMethod qmethod = fromReflectedMethod(info.reflectiveMethod);
            if(qmethod!=null && qmethod.isValid() && qmethod.parameterTypes().size()==info.lambdaArgs.size()+3) {
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
    
    /**
     * Calling <code>invokeMethod(method, AutoConnection, ...)</code>.
     * 
     * @param <A> The type of the first parameter of the method.
     * @param <B> The type of the second parameter of the method.
     * @param <C> The type of the third parameter of the method.
     * @param <D> The type of the fourth parameter of the method.
     * @param <R> The return type of the method.
     * @param method invoked method
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @param arg4 Argument for the fourth parameter.
     * @return method result value - if the invocation is asynchronous, the return value cannot be evaluated.
     * @throws QUnsuccessfulInvocationException if not able to invoke slot
     */
    @QtUninvokable
    public static <A,B,C,D,R> R invokeMethod(Method4<A,B,C,D,R> method, A arg1, B arg2, C arg3, D arg4) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
        return invokeMethod(method, Qt.ConnectionType.AutoConnection, arg1, arg2, arg3, arg4);
    }
    
    /**
     * <p>Invokes the method and returns it's result value.</p>
     * 
     * <p>The invocation can be either synchronous or asynchronous, depending on type:</p>
     * <ul>
     * <li>If type is {@link Qt.ConnectionType#DirectConnection}, the member will be invoked immediately.</li>
     * <li>If type is {@link Qt.ConnectionType#QueuedConnection}, a QEvent will be sent and the member is invoked as soon as the application enters the main event loop.</li>
     * <li>If type is {@link Qt.ConnectionType#BlockingQueuedConnection}, the method will be invoked in the same way as for {@link Qt.ConnectionType#QueuedConnection}, except that the current thread will block until the event is delivered. Using this connection type to communicate between objects in the same thread will lead to deadlocks.</li>
     * <li>If type is {@link Qt.ConnectionType#AutoConnection}, the member is invoked synchronously if obj lives in the same thread as the caller; otherwise it will invoke the member asynchronously.</li>
     * </ul>
     * <p>If the invocation is asynchronous, the return value cannot be evaluated.</p>
     * 
     * @param <A> The type of the first parameter of the method.
     * @param <B> The type of the second parameter of the method.
     * @param <C> The type of the third parameter of the method.
     * @param <D> The type of the fourth parameter of the method.
     * @param <R> The return type of the method.
     * @param method invoked method
     * @param type synchronous or asynchronous invokation
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @param arg4 Argument for the fourth parameter.
     * @return method result value - if the invocation is asynchronous, the return value cannot be evaluated.
     * @throws QUnsuccessfulInvocationException if not able to invoke slot
     */
    @SuppressWarnings("unchecked")
    @QtUninvokable
    public static <A,B,C,D,R> R invokeMethod(Method4<A,B,C,D,R> method, Qt.ConnectionType type, A arg1, B arg2, C arg3, D arg4) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
        QtJambiInternal.LambdaInfo info = QtJambiInternal.lamdaInfo(method);
        if(info!=null && info.qobject!=null && !info.qobject.isDisposed() && info.reflectiveMethod!=null) {
            QMetaMethod qmethod = fromReflectedMethod(info.reflectiveMethod);
            if(qmethod!=null && qmethod.isValid() && qmethod.parameterTypes().size()==info.lambdaArgs.size()+4) {
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
    
    /**
     * Calling <code>invokeMethod(method, AutoConnection, ...)</code>.
     * 
     * @param <A> The type of the first parameter of the method.
     * @param <B> The type of the second parameter of the method.
     * @param <C> The type of the third parameter of the method.
     * @param <D> The type of the fourth parameter of the method.
     * @param <E> The type of the fifth parameter of the method.
     * @param <R> The return type of the method.
     * @param method invoked method
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @param arg4 Argument for the fourth parameter.
     * @param arg5 Argument for the fifth parameter.
     * @return method result value - if the invocation is asynchronous, the return value cannot be evaluated.
     * @throws QUnsuccessfulInvocationException if not able to invoke slot
     */
    @QtUninvokable
    public static <A,B,C,D,E,R> R invokeMethod(Method5<A,B,C,D,E,R> method, A arg1, B arg2, C arg3, D arg4, E arg5) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
        return invokeMethod(method, Qt.ConnectionType.AutoConnection, arg1, arg2, arg3, arg4, arg5);
    }
    
    /**
     * <p>Invokes the method and returns it's result value.</p>
     * 
     * <p>The invocation can be either synchronous or asynchronous, depending on type:</p>
     * <ul>
     * <li>If type is {@link Qt.ConnectionType#DirectConnection}, the member will be invoked immediately.</li>
     * <li>If type is {@link Qt.ConnectionType#QueuedConnection}, a QEvent will be sent and the member is invoked as soon as the application enters the main event loop.</li>
     * <li>If type is {@link Qt.ConnectionType#BlockingQueuedConnection}, the method will be invoked in the same way as for {@link Qt.ConnectionType#QueuedConnection}, except that the current thread will block until the event is delivered. Using this connection type to communicate between objects in the same thread will lead to deadlocks.</li>
     * <li>If type is {@link Qt.ConnectionType#AutoConnection}, the member is invoked synchronously if obj lives in the same thread as the caller; otherwise it will invoke the member asynchronously.</li>
     * </ul>
     * <p>If the invocation is asynchronous, the return value cannot be evaluated.</p>
     * 
     * @param <A> The type of the first parameter of the method.
     * @param <B> The type of the second parameter of the method.
     * @param <C> The type of the third parameter of the method.
     * @param <D> The type of the fourth parameter of the method.
     * @param <E> The type of the fifth parameter of the method.
     * @param <R> The return type of the method.
     * @param method invoked method
     * @param type synchronous or asynchronous invokation
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @param arg4 Argument for the fourth parameter.
     * @param arg5 Argument for the fifth parameter.
     * @return method result value - if the invocation is asynchronous, the return value cannot be evaluated.
     * @throws QUnsuccessfulInvocationException if not able to invoke slot
     */
    @SuppressWarnings("unchecked")
    @QtUninvokable
    public static <A,B,C,D,E,R> R invokeMethod(Method5<A,B,C,D,E,R> method, Qt.ConnectionType type, A arg1, B arg2, C arg3, D arg4, E arg5) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
        QtJambiInternal.LambdaInfo info = QtJambiInternal.lamdaInfo(method);
        if(info!=null && info.qobject!=null && !info.qobject.isDisposed() && info.reflectiveMethod!=null) {
            QMetaMethod qmethod = fromReflectedMethod(info.reflectiveMethod);
            if(qmethod!=null && qmethod.isValid() && qmethod.parameterTypes().size()==info.lambdaArgs.size()+5) {
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
    
    /**
     * Calling <code>invokeMethod(method, AutoConnection, ...)</code>.
     * 
     * @param <A> The type of the first parameter of the method.
     * @param <B> The type of the second parameter of the method.
     * @param <C> The type of the third parameter of the method.
     * @param <D> The type of the fourth parameter of the method.
     * @param <E> The type of the fifth parameter of the method.
     * @param <F> The type of the sixth parameter of the method.
     * @param <R> The return type of the method.
     * @param method invoked method
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @param arg4 Argument for the fourth parameter.
     * @param arg5 Argument for the fifth parameter.
     * @param arg6 Argument for the sixth parameter.
     * @return method result value - if the invocation is asynchronous, the return value cannot be evaluated.
     * @throws QUnsuccessfulInvocationException if not able to invoke slot
     */
    @QtUninvokable
    public static <A,B,C,D,E,F,R> R invokeMethod(Method6<A,B,C,D,E,F,R> method, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
        return invokeMethod(method, Qt.ConnectionType.AutoConnection, arg1, arg2, arg3, arg4, arg5, arg6);
    }
    
    /**
     * <p>Invokes the method and returns it's result value.</p>
     * 
     * <p>The invocation can be either synchronous or asynchronous, depending on type:</p>
     * <ul>
     * <li>If type is {@link Qt.ConnectionType#DirectConnection}, the member will be invoked immediately.</li>
     * <li>If type is {@link Qt.ConnectionType#QueuedConnection}, a QEvent will be sent and the member is invoked as soon as the application enters the main event loop.</li>
     * <li>If type is {@link Qt.ConnectionType#BlockingQueuedConnection}, the method will be invoked in the same way as for {@link Qt.ConnectionType#QueuedConnection}, except that the current thread will block until the event is delivered. Using this connection type to communicate between objects in the same thread will lead to deadlocks.</li>
     * <li>If type is {@link Qt.ConnectionType#AutoConnection}, the member is invoked synchronously if obj lives in the same thread as the caller; otherwise it will invoke the member asynchronously.</li>
     * </ul>
     * <p>If the invocation is asynchronous, the return value cannot be evaluated.</p>
     * 
     * @param <A> The type of the first parameter of the method.
     * @param <B> The type of the second parameter of the method.
     * @param <C> The type of the third parameter of the method.
     * @param <D> The type of the fourth parameter of the method.
     * @param <E> The type of the fifth parameter of the method.
     * @param <F> The type of the sixth parameter of the method.
     * @param <R> The return type of the method.
     * @param method invoked method
     * @param type synchronous or asynchronous invokation
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @param arg4 Argument for the fourth parameter.
     * @param arg5 Argument for the fifth parameter.
     * @param arg6 Argument for the sixth parameter.
     * @return method result value - if the invocation is asynchronous, the return value cannot be evaluated.
     * @throws QUnsuccessfulInvocationException if not able to invoke slot
     */
    @SuppressWarnings("unchecked")
    @QtUninvokable
    public static <A,B,C,D,E,F,R> R invokeMethod(Method6<A,B,C,D,E,F,R> method, Qt.ConnectionType type, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
        QtJambiInternal.LambdaInfo info = QtJambiInternal.lamdaInfo(method);
        if(info!=null && info.qobject!=null && !info.qobject.isDisposed() && info.reflectiveMethod!=null) {
            QMetaMethod qmethod = fromReflectedMethod(info.reflectiveMethod);
            if(qmethod!=null && qmethod.isValid() && qmethod.parameterTypes().size()==info.lambdaArgs.size()+6) {
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
    
    /**
     * Calling <code>invokeMethod(method, AutoConnection, ...)</code>.
     * 
     * @param <A> The type of the first parameter of the method.
     * @param <B> The type of the second parameter of the method.
     * @param <C> The type of the third parameter of the method.
     * @param <D> The type of the fourth parameter of the method.
     * @param <E> The type of the fifth parameter of the method.
     * @param <F> The type of the sixth parameter of the method.
     * @param <G> The type of the seventh parameter of the method.
     * @param <R> The return type of the method.
     * @param method invoked method
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @param arg4 Argument for the fourth parameter.
     * @param arg5 Argument for the fifth parameter.
     * @param arg6 Argument for the sixth parameter.
     * @param arg7 Argument for the seventh parameter.
     * @return method result value - if the invocation is asynchronous, the return value cannot be evaluated.
     * @throws QUnsuccessfulInvocationException if not able to invoke slot
     */
    @QtUninvokable
    public static <A,B,C,D,E,F,G,R> R invokeMethod(Method7<A,B,C,D,E,F,G,R> method, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
        return invokeMethod(method, Qt.ConnectionType.AutoConnection, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
    }
    
    /**
     * <p>Invokes the method and returns it's result value.</p>
     * 
     * <p>The invocation can be either synchronous or asynchronous, depending on type:</p>
     * <ul>
     * <li>If type is {@link Qt.ConnectionType#DirectConnection}, the member will be invoked immediately.</li>
     * <li>If type is {@link Qt.ConnectionType#QueuedConnection}, a QEvent will be sent and the member is invoked as soon as the application enters the main event loop.</li>
     * <li>If type is {@link Qt.ConnectionType#BlockingQueuedConnection}, the method will be invoked in the same way as for {@link Qt.ConnectionType#QueuedConnection}, except that the current thread will block until the event is delivered. Using this connection type to communicate between objects in the same thread will lead to deadlocks.</li>
     * <li>If type is {@link Qt.ConnectionType#AutoConnection}, the member is invoked synchronously if obj lives in the same thread as the caller; otherwise it will invoke the member asynchronously.</li>
     * </ul>
     * <p>If the invocation is asynchronous, the return value cannot be evaluated.</p>
     * 
     * @param <A> The type of the first parameter of the method.
     * @param <B> The type of the second parameter of the method.
     * @param <C> The type of the third parameter of the method.
     * @param <D> The type of the fourth parameter of the method.
     * @param <E> The type of the fifth parameter of the method.
     * @param <F> The type of the sixth parameter of the method.
     * @param <G> The type of the seventh parameter of the method.
     * @param <R> The return type of the method.
     * @param method invoked method
     * @param type synchronous or asynchronous invokation
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @param arg4 Argument for the fourth parameter.
     * @param arg5 Argument for the fifth parameter.
     * @param arg6 Argument for the sixth parameter.
     * @param arg7 Argument for the seventh parameter.
     * @return method result value - if the invocation is asynchronous, the return value cannot be evaluated.
     * @throws QUnsuccessfulInvocationException if not able to invoke slot
     */
    @SuppressWarnings("unchecked")
    @QtUninvokable
    public static <A,B,C,D,E,F,G,R> R invokeMethod(Method7<A,B,C,D,E,F,G,R> method, Qt.ConnectionType type, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
        QtJambiInternal.LambdaInfo info = QtJambiInternal.lamdaInfo(method);
        if(info!=null && info.qobject!=null && !info.qobject.isDisposed() && info.reflectiveMethod!=null) {
            QMetaMethod qmethod = fromReflectedMethod(info.reflectiveMethod);
            if(qmethod!=null && qmethod.isValid() && qmethod.parameterTypes().size()==info.lambdaArgs.size()+7) {
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
    
    /**
     * Calling <code>invokeMethod(method, AutoConnection, ...)</code>.
     * 
     * @param <A> The type of the first parameter of the method.
     * @param <B> The type of the second parameter of the method.
     * @param <C> The type of the third parameter of the method.
     * @param <D> The type of the fourth parameter of the method.
     * @param <E> The type of the fifth parameter of the method.
     * @param <F> The type of the sixth parameter of the method.
     * @param <G> The type of the seventh parameter of the method.
     * @param <H> The type of the eighth parameter of the method.
     * @param <R> The return type of the method.
     * @param method invoked method
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @param arg4 Argument for the fourth parameter.
     * @param arg5 Argument for the fifth parameter.
     * @param arg6 Argument for the sixth parameter.
     * @param arg7 Argument for the seventh parameter.
     * @param arg8 Argument for the eighth parameter.
     * @return method result value - if the invocation is asynchronous, the return value cannot be evaluated.
     * @throws QUnsuccessfulInvocationException if not able to invoke slot
     */
    @QtUninvokable
    public static <A,B,C,D,E,F,G,H,R> R invokeMethod(Method8<A,B,C,D,E,F,G,H,R> method, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7, H arg8) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
        return invokeMethod(method, Qt.ConnectionType.AutoConnection, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
    }
    
    /**
     * <p>Invokes the method and returns it's result value.</p>
     * 
     * <p>The invocation can be either synchronous or asynchronous, depending on type:</p>
     * <ul>
     * <li>If type is {@link Qt.ConnectionType#DirectConnection}, the member will be invoked immediately.</li>
     * <li>If type is {@link Qt.ConnectionType#QueuedConnection}, a QEvent will be sent and the member is invoked as soon as the application enters the main event loop.</li>
     * <li>If type is {@link Qt.ConnectionType#BlockingQueuedConnection}, the method will be invoked in the same way as for {@link Qt.ConnectionType#QueuedConnection}, except that the current thread will block until the event is delivered. Using this connection type to communicate between objects in the same thread will lead to deadlocks.</li>
     * <li>If type is {@link Qt.ConnectionType#AutoConnection}, the member is invoked synchronously if obj lives in the same thread as the caller; otherwise it will invoke the member asynchronously.</li>
     * </ul>
     * <p>If the invocation is asynchronous, the return value cannot be evaluated.</p>
     * 
     * @param <A> The type of the first parameter of the method.
     * @param <B> The type of the second parameter of the method.
     * @param <C> The type of the third parameter of the method.
     * @param <D> The type of the fourth parameter of the method.
     * @param <E> The type of the fifth parameter of the method.
     * @param <F> The type of the sixth parameter of the method.
     * @param <G> The type of the seventh parameter of the method.
     * @param <H> The type of the eighth parameter of the method.
     * @param <R> The return type of the method.
     * @param method invoked method
     * @param type synchronous or asynchronous invokation
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @param arg4 Argument for the fourth parameter.
     * @param arg5 Argument for the fifth parameter.
     * @param arg6 Argument for the sixth parameter.
     * @param arg7 Argument for the seventh parameter.
     * @param arg8 Argument for the eighth parameter.
     * @return method result value - if the invocation is asynchronous, the return value cannot be evaluated.
     * @throws QUnsuccessfulInvocationException if not able to invoke slot
     */
    @SuppressWarnings("unchecked")
    @QtUninvokable
    public static <A,B,C,D,E,F,G,H,R> R invokeMethod(Method8<A,B,C,D,E,F,G,H,R> method, Qt.ConnectionType type, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7, H arg8) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
        QtJambiInternal.LambdaInfo info = QtJambiInternal.lamdaInfo(method);
        if(info!=null && info.qobject!=null && !info.qobject.isDisposed() && info.reflectiveMethod!=null) {
            QMetaMethod qmethod = fromReflectedMethod(info.reflectiveMethod);
            if(qmethod!=null && qmethod.isValid() && qmethod.parameterTypes().size()==info.lambdaArgs.size()+8) {
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
    
    /**
     * Calling <code>invokeMethod(method, AutoConnection, ...)</code>.
     * 
     * @param <A> The type of the first parameter of the method.
     * @param <B> The type of the second parameter of the method.
     * @param <C> The type of the third parameter of the method.
     * @param <D> The type of the fourth parameter of the method.
     * @param <E> The type of the fifth parameter of the method.
     * @param <F> The type of the sixth parameter of the method.
     * @param <G> The type of the seventh parameter of the method.
     * @param <H> The type of the eighth parameter of the method.
     * @param <I> The type of the ninth parameter of the method.
     * @param <R> The return type of the method.
     * @param method invoked method
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @param arg4 Argument for the fourth parameter.
     * @param arg5 Argument for the fifth parameter.
     * @param arg6 Argument for the sixth parameter.
     * @param arg7 Argument for the seventh parameter.
     * @param arg8 Argument for the eighth parameter.
     * @param arg9 Argument for the ninth parameter.
     * @return method result value - if the invocation is asynchronous, the return value cannot be evaluated.
     * @throws QUnsuccessfulInvocationException if not able to invoke slot
     */
    @QtUninvokable
    public static <A,B,C,D,E,F,G,H,I,R> R invokeMethod(Method9<A,B,C,D,E,F,G,H,I,R> method, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7, H arg8, I arg9) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
        return invokeMethod(method, Qt.ConnectionType.AutoConnection, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
    }
    
    /**
     * <p>Invokes the method and returns it's result value.</p>
     * 
     * <p>The invocation can be either synchronous or asynchronous, depending on type:</p>
     * <ul>
     * <li>If type is {@link Qt.ConnectionType#DirectConnection}, the member will be invoked immediately.</li>
     * <li>If type is {@link Qt.ConnectionType#QueuedConnection}, a QEvent will be sent and the member is invoked as soon as the application enters the main event loop.</li>
     * <li>If type is {@link Qt.ConnectionType#BlockingQueuedConnection}, the method will be invoked in the same way as for {@link Qt.ConnectionType#QueuedConnection}, except that the current thread will block until the event is delivered. Using this connection type to communicate between objects in the same thread will lead to deadlocks.</li>
     * <li>If type is {@link Qt.ConnectionType#AutoConnection}, the member is invoked synchronously if obj lives in the same thread as the caller; otherwise it will invoke the member asynchronously.</li>
     * </ul>
     * <p>If the invocation is asynchronous, the return value cannot be evaluated.</p>
     * 
     * @param <A> The type of the first parameter of the method.
     * @param <B> The type of the second parameter of the method.
     * @param <C> The type of the third parameter of the method.
     * @param <D> The type of the fourth parameter of the method.
     * @param <E> The type of the fifth parameter of the method.
     * @param <F> The type of the sixth parameter of the method.
     * @param <G> The type of the seventh parameter of the method.
     * @param <H> The type of the eighth parameter of the method.
     * @param <I> The type of the ninth parameter of the method.
     * @param <R> The return type of the method.
     * @param method invoked method
     * @param type synchronous or asynchronous invokation
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @param arg4 Argument for the fourth parameter.
     * @param arg5 Argument for the fifth parameter.
     * @param arg6 Argument for the sixth parameter.
     * @param arg7 Argument for the seventh parameter.
     * @param arg8 Argument for the eighth parameter.
     * @param arg9 Argument for the ninth parameter.
     * @return method result value - if the invocation is asynchronous, the return value cannot be evaluated.
     * @throws QUnsuccessfulInvocationException if not able to invoke slot
     */
    @SuppressWarnings("unchecked")
    @QtUninvokable
    public static <A,B,C,D,E,F,G,H,I,R> R invokeMethod(Method9<A,B,C,D,E,F,G,H,I,R> method, Qt.ConnectionType type, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7, H arg8, I arg9) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
        QtJambiInternal.LambdaInfo info = QtJambiInternal.lamdaInfo(method);
        if(info!=null && info.qobject!=null && !info.qobject.isDisposed() && info.reflectiveMethod!=null) {
            QMetaMethod qmethod = fromReflectedMethod(info.reflectiveMethod);
            if(qmethod!=null && qmethod.isValid() && qmethod.parameterTypes().size()==info.lambdaArgs.size()+9) {
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
    
    /**
     * Calling <code>invokeMethod(method, AutoConnection)</code>.
     * @param method invoked method
     * @throws QUnsuccessfulInvocationException if not able to invoke slot
     */
    @QtUninvokable
    public static void invokeMethod(Slot0 method) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
        invokeMethod(method, Qt.ConnectionType.AutoConnection);
    }
    
    /**
     * <p>Invokes the slot.</p>
     * 
     * <p>The invocation can be either synchronous or asynchronous, depending on type:</p>
     * <ul>
     * <li>If type is {@link Qt.ConnectionType#DirectConnection}, the member will be invoked immediately.</li>
     * <li>If type is {@link Qt.ConnectionType#QueuedConnection}, a QEvent will be sent and the member is invoked as soon as the application enters the main event loop.</li>
     * <li>If type is {@link Qt.ConnectionType#BlockingQueuedConnection}, the method will be invoked in the same way as for {@link Qt.ConnectionType#QueuedConnection}, except that the current thread will block until the event is delivered. Using this connection type to communicate between objects in the same thread will lead to deadlocks.</li>
     * <li>If type is {@link Qt.ConnectionType#AutoConnection}, the member is invoked synchronously if obj lives in the same thread as the caller; otherwise it will invoke the member asynchronously.</li>
     * </ul>
     * 
     * @param method invoked method
     * @param type synchronous or asynchronous invokation
     * @throws QUnsuccessfulInvocationException if not able to invoke slot
     */
    @QtUninvokable
    public static void invokeMethod(Slot0 method, Qt.ConnectionType type) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
        QtJambiInternal.LambdaInfo info = QtJambiInternal.lamdaInfo(method);
        QThread thread = null;
        if(info!=null && info.qobject!=null && !info.qobject.isDisposed()) {
            if(info.reflectiveMethod!=null) {
                QMetaMethod qmethod = fromReflectedMethod(info.reflectiveMethod);
                if(qmethod!=null && qmethod.isValid() && qmethod.parameterTypes().size()==info.lambdaArgs.size()) {
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
                    synchronized (invoker) {
                        invoker.disposeLater();
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
            QtJambi_LibraryUtilities.internal.setCppOwnership(invoker);
            if(thread!=null)
                invoker.moveToThread(thread);
            invoker.disposeLater();
            return;
        default:
            break;
        }
        throw new QUnsuccessfulInvocationException("Unable to invoke method.");
    }
    
    /**
     * Calling <code>invokeMethod(method, AutoConnection, ...)</code>.
     * @param <A> The type of the first parameter of the slot.
     * @param method invoked method
     * @param arg1 Argument for the first parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke slot
     */
    @QtUninvokable
    public static <A> void invokeMethod(Slot1<A> method, A arg1) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
        invokeMethod(method, Qt.ConnectionType.AutoConnection, arg1);
    }
    
    /**
     * <p>Invokes the slot.</p>
     * 
     * <p>The invocation can be either synchronous or asynchronous, depending on type:</p>
     * <ul>
     * <li>If type is {@link Qt.ConnectionType#DirectConnection}, the member will be invoked immediately.</li>
     * <li>If type is {@link Qt.ConnectionType#QueuedConnection}, a QEvent will be sent and the member is invoked as soon as the application enters the main event loop.</li>
     * <li>If type is {@link Qt.ConnectionType#BlockingQueuedConnection}, the method will be invoked in the same way as for {@link Qt.ConnectionType#QueuedConnection}, except that the current thread will block until the event is delivered. Using this connection type to communicate between objects in the same thread will lead to deadlocks.</li>
     * <li>If type is {@link Qt.ConnectionType#AutoConnection}, the member is invoked synchronously if obj lives in the same thread as the caller; otherwise it will invoke the member asynchronously.</li>
     * </ul>
     * 
     * @param <A> The type of the first parameter of the slot.
     * @param method invoked method
     * @param type synchronous or asynchronous invokation
     * @param arg1 Argument for the first parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke slot
     */
    @QtUninvokable
    public static <A> void invokeMethod(Slot1<A> method, Qt.ConnectionType type, A arg1) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
        QtJambiInternal.LambdaInfo info = QtJambiInternal.lamdaInfo(method);
        QThread thread = null;
        if(info!=null && info.qobject!=null && !info.qobject.isDisposed()) {
            if(info.reflectiveMethod!=null) {
                QMetaMethod qmethod = fromReflectedMethod(info.reflectiveMethod);
                if(qmethod!=null && qmethod.isValid() && qmethod.parameterTypes().size()==info.lambdaArgs.size()+1) {
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
                    synchronized (invoker) {
                        invoker.disposeLater();
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
            QtJambi_LibraryUtilities.internal.setCppOwnership(invoker);
            if(thread!=null)
                invoker.moveToThread(thread);
            invoker.disposeLater();
            return;
        default:
            break;
        }
        throw new QUnsuccessfulInvocationException("Unable to invoke method.");
    }
    
    /**
     * Calling <code>invokeMethod(method, AutoConnection, ...)</code>.
     * @param <A> The type of the first parameter of the slot.
     * @param <B> The type of the second parameter of the slot.
     * @param method invoked method
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke slot
     */
    @QtUninvokable
    public static <A,B> void invokeMethod(Slot2<A,B> method, A arg1, B arg2) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
        invokeMethod(method, Qt.ConnectionType.AutoConnection, arg1, arg2);
    }
    
    /**
     * <p>Invokes the slot.</p>
     * 
     * <p>The invocation can be either synchronous or asynchronous, depending on type:</p>
     * <ul>
     * <li>If type is {@link Qt.ConnectionType#DirectConnection}, the member will be invoked immediately.</li>
     * <li>If type is {@link Qt.ConnectionType#QueuedConnection}, a QEvent will be sent and the member is invoked as soon as the application enters the main event loop.</li>
     * <li>If type is {@link Qt.ConnectionType#BlockingQueuedConnection}, the method will be invoked in the same way as for {@link Qt.ConnectionType#QueuedConnection}, except that the current thread will block until the event is delivered. Using this connection type to communicate between objects in the same thread will lead to deadlocks.</li>
     * <li>If type is {@link Qt.ConnectionType#AutoConnection}, the member is invoked synchronously if obj lives in the same thread as the caller; otherwise it will invoke the member asynchronously.</li>
     * </ul>
     * 
     * @param <A> The type of the first parameter of the slot.
     * @param <B> The type of the second parameter of the slot.
     * @param method invoked method
     * @param type synchronous or asynchronous invokation
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke slot
     */
    @QtUninvokable
    public static <A,B> void invokeMethod(Slot2<A,B> method, Qt.ConnectionType type, A arg1, B arg2) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
        QtJambiInternal.LambdaInfo info = QtJambiInternal.lamdaInfo(method);
        QThread thread = null;
        if(info!=null && info.qobject!=null && !info.qobject.isDisposed()) {
            if(info.reflectiveMethod!=null) {
                QMetaMethod qmethod = fromReflectedMethod(info.reflectiveMethod);
                if(qmethod!=null && qmethod.isValid() && qmethod.parameterTypes().size()==info.lambdaArgs.size()+2) {
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
                    synchronized (invoker) {
                        invoker.disposeLater();
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
            QtJambi_LibraryUtilities.internal.setCppOwnership(invoker);
            if(thread!=null)
                invoker.moveToThread(thread);
            invoker.disposeLater();
            return;
        default:
            break;
        }
        throw new QUnsuccessfulInvocationException("Unable to invoke method.");
    }
    
    /**
     * Calling <code>invokeMethod(method, AutoConnection, ...)</code>.
     * @param <A> The type of the first parameter of the slot.
     * @param <B> The type of the second parameter of the slot.
     * @param <C> The type of the third parameter of the slot.
     * @param method invoked method
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke slot
     */
    @QtUninvokable
    public static <A,B,C> void invokeMethod(Slot3<A,B,C> method, A arg1, B arg2, C arg3) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
        invokeMethod(method, Qt.ConnectionType.AutoConnection, arg1, arg2, arg3);
    }
    
    /**
     * <p>Invokes the slot.</p>
     * 
     * <p>The invocation can be either synchronous or asynchronous, depending on type:</p>
     * <ul>
     * <li>If type is {@link Qt.ConnectionType#DirectConnection}, the member will be invoked immediately.</li>
     * <li>If type is {@link Qt.ConnectionType#QueuedConnection}, a QEvent will be sent and the member is invoked as soon as the application enters the main event loop.</li>
     * <li>If type is {@link Qt.ConnectionType#BlockingQueuedConnection}, the method will be invoked in the same way as for {@link Qt.ConnectionType#QueuedConnection}, except that the current thread will block until the event is delivered. Using this connection type to communicate between objects in the same thread will lead to deadlocks.</li>
     * <li>If type is {@link Qt.ConnectionType#AutoConnection}, the member is invoked synchronously if obj lives in the same thread as the caller; otherwise it will invoke the member asynchronously.</li>
     * </ul>
     * 
     * @param <A> The type of the first parameter of the slot.
     * @param <B> The type of the second parameter of the slot.
     * @param <C> The type of the third parameter of the slot.
     * @param method invoked method
     * @param type synchronous or asynchronous invokation
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke slot
     */
    @QtUninvokable
    public static <A,B,C> void invokeMethod(Slot3<A,B,C> method, Qt.ConnectionType type, A arg1, B arg2, C arg3) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
        QtJambiInternal.LambdaInfo info = QtJambiInternal.lamdaInfo(method);
        QThread thread = null;
        if(info!=null && info.qobject!=null) {
            if(info.reflectiveMethod!=null) {
                QMetaMethod qmethod = fromReflectedMethod(info.reflectiveMethod);
                if(qmethod!=null && qmethod.isValid() && qmethod.parameterTypes().size()==info.lambdaArgs.size()+3) {
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
                    synchronized (invoker) {
                        invoker.disposeLater();
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
            QtJambi_LibraryUtilities.internal.setCppOwnership(invoker);
            if(thread!=null)
                invoker.moveToThread(thread);
            invoker.disposeLater();
            return;
        default:
            break;
        }
        throw new QUnsuccessfulInvocationException("Unable to invoke method.");
    }
    
    /**
     * Calling <code>invokeMethod(method, AutoConnection, ...)</code>.
     * @param <A> The type of the first parameter of the slot.
     * @param <B> The type of the second parameter of the slot.
     * @param <C> The type of the third parameter of the slot.
     * @param <D> The type of the fourth parameter of the slot.
     * @param method invoked method
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @param arg4 Argument for the fourth parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke slot
     */
    @QtUninvokable
    public static <A,B,C,D> void invokeMethod(Slot4<A,B,C,D> method, A arg1, B arg2, C arg3, D arg4) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
        invokeMethod(method, Qt.ConnectionType.AutoConnection, arg1, arg2, arg3, arg4);
    }
    
    /**
     * <p>Invokes the slot.</p>
     * 
     * <p>The invocation can be either synchronous or asynchronous, depending on type:</p>
     * <ul>
     * <li>If type is {@link Qt.ConnectionType#DirectConnection}, the member will be invoked immediately.</li>
     * <li>If type is {@link Qt.ConnectionType#QueuedConnection}, a QEvent will be sent and the member is invoked as soon as the application enters the main event loop.</li>
     * <li>If type is {@link Qt.ConnectionType#BlockingQueuedConnection}, the method will be invoked in the same way as for {@link Qt.ConnectionType#QueuedConnection}, except that the current thread will block until the event is delivered. Using this connection type to communicate between objects in the same thread will lead to deadlocks.</li>
     * <li>If type is {@link Qt.ConnectionType#AutoConnection}, the member is invoked synchronously if obj lives in the same thread as the caller; otherwise it will invoke the member asynchronously.</li>
     * </ul>
     * 
     * @param <A> The type of the first parameter of the slot.
     * @param <B> The type of the second parameter of the slot.
     * @param <C> The type of the third parameter of the slot.
     * @param <D> The type of the fourth parameter of the slot.
     * @param method invoked method
     * @param type synchronous or asynchronous invokation
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @param arg4 Argument for the fourth parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke slot
     */
    @QtUninvokable
    public static <A,B,C,D> void invokeMethod(Slot4<A,B,C,D> method, Qt.ConnectionType type, A arg1, B arg2, C arg3, D arg4) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
        QtJambiInternal.LambdaInfo info = QtJambiInternal.lamdaInfo(method);
        QThread thread = null;
        if(info!=null && info.qobject!=null) {
            if(info.reflectiveMethod!=null) {
                QMetaMethod qmethod = fromReflectedMethod(info.reflectiveMethod);
                if(qmethod!=null && qmethod.isValid() && qmethod.parameterTypes().size()==info.lambdaArgs.size()+4) {
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
                    synchronized (invoker) {
                        invoker.disposeLater();
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
            QtJambi_LibraryUtilities.internal.setCppOwnership(invoker);
            if(thread!=null)
                invoker.moveToThread(thread);
            invoker.disposeLater();
            return;
        default:
            break;
        }
        throw new QUnsuccessfulInvocationException("Unable to invoke method.");
    }
    
    /**
     * Calling <code>invokeMethod(method, AutoConnection, ...)</code>.
     * @param <A> The type of the first parameter of the slot.
     * @param <B> The type of the second parameter of the slot.
     * @param <C> The type of the third parameter of the slot.
     * @param <D> The type of the fourth parameter of the slot.
     * @param <E> The type of the fifth parameter of the slot.
     * @param method invoked method
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @param arg4 Argument for the fourth parameter.
     * @param arg5 Argument for the fifth parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke slot
     */
    @QtUninvokable
    public static <A,B,C,D,E> void invokeMethod(Slot5<A,B,C,D,E> method, A arg1, B arg2, C arg3, D arg4, E arg5) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
        invokeMethod(method, Qt.ConnectionType.AutoConnection, arg1, arg2, arg3, arg4, arg5);
    }
    
    /**
     * <p>Invokes the slot.</p>
     * 
     * <p>The invocation can be either synchronous or asynchronous, depending on type:</p>
     * <ul>
     * <li>If type is {@link Qt.ConnectionType#DirectConnection}, the member will be invoked immediately.</li>
     * <li>If type is {@link Qt.ConnectionType#QueuedConnection}, a QEvent will be sent and the member is invoked as soon as the application enters the main event loop.</li>
     * <li>If type is {@link Qt.ConnectionType#BlockingQueuedConnection}, the method will be invoked in the same way as for {@link Qt.ConnectionType#QueuedConnection}, except that the current thread will block until the event is delivered. Using this connection type to communicate between objects in the same thread will lead to deadlocks.</li>
     * <li>If type is {@link Qt.ConnectionType#AutoConnection}, the member is invoked synchronously if obj lives in the same thread as the caller; otherwise it will invoke the member asynchronously.</li>
     * </ul>
     * 
     * @param <A> The type of the first parameter of the slot.
     * @param <B> The type of the second parameter of the slot.
     * @param <C> The type of the third parameter of the slot.
     * @param <D> The type of the fourth parameter of the slot.
     * @param <E> The type of the fifth parameter of the slot.
     * @param method invoked method
     * @param type synchronous or asynchronous invokation
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @param arg4 Argument for the fourth parameter.
     * @param arg5 Argument for the fifth parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke slot
     */
    @QtUninvokable
    public static <A,B,C,D,E> void invokeMethod(Slot5<A,B,C,D,E> method, Qt.ConnectionType type, A arg1, B arg2, C arg3, D arg4, E arg5) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
        QtJambiInternal.LambdaInfo info = QtJambiInternal.lamdaInfo(method);
        QThread thread = null;
        if(info!=null && info.qobject!=null) {
            if(info.reflectiveMethod!=null) {
                QMetaMethod qmethod = fromReflectedMethod(info.reflectiveMethod);
                if(qmethod!=null && qmethod.isValid() && qmethod.parameterTypes().size()==info.lambdaArgs.size()+5) {
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
                    synchronized (invoker) {
                        invoker.disposeLater();
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
            QtJambi_LibraryUtilities.internal.setCppOwnership(invoker);
            if(thread!=null)
                invoker.moveToThread(thread);
            invoker.disposeLater();
            return;
        default:
            break;
        }
        throw new QUnsuccessfulInvocationException("Unable to invoke method.");
    }
    
    /**
     * Calling <code>invokeMethod(method, AutoConnection, ...)</code>.
     * @param <A> The type of the first parameter of the slot.
     * @param <B> The type of the second parameter of the slot.
     * @param <C> The type of the third parameter of the slot.
     * @param <D> The type of the fourth parameter of the slot.
     * @param <E> The type of the fifth parameter of the slot.
     * @param <F> The type of the sixth parameter of the slot.
     * @param method invoked method
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @param arg4 Argument for the fourth parameter.
     * @param arg5 Argument for the fifth parameter.
     * @param arg6 Argument for the sixth parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke slot
     */
    public static <A,B,C,D,E,F> void invokeMethod(Slot6<A,B,C,D,E,F> method, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
        invokeMethod(method, Qt.ConnectionType.AutoConnection, arg1, arg2, arg3, arg4, arg5, arg6);
    }
    
    /**
     * <p>Invokes the slot.</p>
     * 
     * <p>The invocation can be either synchronous or asynchronous, depending on type:</p>
     * <ul>
     * <li>If type is {@link Qt.ConnectionType#DirectConnection}, the member will be invoked immediately.</li>
     * <li>If type is {@link Qt.ConnectionType#QueuedConnection}, a QEvent will be sent and the member is invoked as soon as the application enters the main event loop.</li>
     * <li>If type is {@link Qt.ConnectionType#BlockingQueuedConnection}, the method will be invoked in the same way as for {@link Qt.ConnectionType#QueuedConnection}, except that the current thread will block until the event is delivered. Using this connection type to communicate between objects in the same thread will lead to deadlocks.</li>
     * <li>If type is {@link Qt.ConnectionType#AutoConnection}, the member is invoked synchronously if obj lives in the same thread as the caller; otherwise it will invoke the member asynchronously.</li>
     * </ul>
     * 
     * @param <A> The type of the first parameter of the slot.
     * @param <B> The type of the second parameter of the slot.
     * @param <C> The type of the third parameter of the slot.
     * @param <D> The type of the fourth parameter of the slot.
     * @param <E> The type of the fifth parameter of the slot.
     * @param <F> The type of the sixth parameter of the slot.
     * @param method invoked method
     * @param type synchronous or asynchronous invokation
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @param arg4 Argument for the fourth parameter.
     * @param arg5 Argument for the fifth parameter.
     * @param arg6 Argument for the sixth parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke slot
     */
    @QtUninvokable
    public static <A,B,C,D,E,F> void invokeMethod(Slot6<A,B,C,D,E,F> method, Qt.ConnectionType type, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
        QtJambiInternal.LambdaInfo info = QtJambiInternal.lamdaInfo(method);
        QThread thread = null;
        if(info!=null && info.qobject!=null) {
            if(info.reflectiveMethod!=null) {
                QMetaMethod qmethod = fromReflectedMethod(info.reflectiveMethod);
                if(qmethod!=null && qmethod.isValid() && qmethod.parameterTypes().size()==info.lambdaArgs.size()+6) {
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
                    synchronized (invoker) {
                        invoker.disposeLater();
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
            QtJambi_LibraryUtilities.internal.setCppOwnership(invoker);
            if(thread!=null)
                invoker.moveToThread(thread);
            invoker.disposeLater();
            return;
        default:
            break;
        }
        throw new QUnsuccessfulInvocationException("Unable to invoke method.");
    }
    
    /**
     * Calling <code>invokeMethod(method, AutoConnection, ...)</code>.
     * @param <A> The type of the first parameter of the slot.
     * @param <B> The type of the second parameter of the slot.
     * @param <C> The type of the third parameter of the slot.
     * @param <D> The type of the fourth parameter of the slot.
     * @param <E> The type of the fifth parameter of the slot.
     * @param <F> The type of the sixth parameter of the slot.
     * @param <G> The type of the seventh parameter of the slot.
     * @param method invoked method
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @param arg4 Argument for the fourth parameter.
     * @param arg5 Argument for the fifth parameter.
     * @param arg6 Argument for the sixth parameter.
     * @param arg7 Argument for the seventh parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke slot
     */
    @QtUninvokable
    public static <A,B,C,D,E,F,G> void invokeMethod(Slot7<A,B,C,D,E,F,G> method, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
        invokeMethod(method, Qt.ConnectionType.AutoConnection, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
    }
    
    /**
     * <p>Invokes the slot.</p>
     * 
     * <p>The invocation can be either synchronous or asynchronous, depending on type:</p>
     * <ul>
     * <li>If type is {@link Qt.ConnectionType#DirectConnection}, the member will be invoked immediately.</li>
     * <li>If type is {@link Qt.ConnectionType#QueuedConnection}, a QEvent will be sent and the member is invoked as soon as the application enters the main event loop.</li>
     * <li>If type is {@link Qt.ConnectionType#BlockingQueuedConnection}, the method will be invoked in the same way as for {@link Qt.ConnectionType#QueuedConnection}, except that the current thread will block until the event is delivered. Using this connection type to communicate between objects in the same thread will lead to deadlocks.</li>
     * <li>If type is {@link Qt.ConnectionType#AutoConnection}, the member is invoked synchronously if obj lives in the same thread as the caller; otherwise it will invoke the member asynchronously.</li>
     * </ul>
     * 
     * @param <A> The type of the first parameter of the slot.
     * @param <B> The type of the second parameter of the slot.
     * @param <C> The type of the third parameter of the slot.
     * @param <D> The type of the fourth parameter of the slot.
     * @param <E> The type of the fifth parameter of the slot.
     * @param <F> The type of the sixth parameter of the slot.
     * @param <G> The type of the seventh parameter of the slot.
     * @param method invoked method
     * @param type synchronous or asynchronous invokation
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @param arg4 Argument for the fourth parameter.
     * @param arg5 Argument for the fifth parameter.
     * @param arg6 Argument for the sixth parameter.
     * @param arg7 Argument for the seventh parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke slot
     */
    @QtUninvokable
    public static <A,B,C,D,E,F,G> void invokeMethod(Slot7<A,B,C,D,E,F,G> method, Qt.ConnectionType type, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
        QtJambiInternal.LambdaInfo info = QtJambiInternal.lamdaInfo(method);
        QThread thread = null;
        if(info!=null && info.qobject!=null) {
            if(info.reflectiveMethod!=null) {
                QMetaMethod qmethod = fromReflectedMethod(info.reflectiveMethod);
                if(qmethod!=null && qmethod.isValid() && qmethod.parameterTypes().size()==info.lambdaArgs.size()+7) {
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
                    synchronized (invoker) {
                        invoker.disposeLater();
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
            QtJambi_LibraryUtilities.internal.setCppOwnership(invoker);
            if(thread!=null)
                invoker.moveToThread(thread);
            invoker.disposeLater();
            return;
        default:
            break;
        }
        throw new QUnsuccessfulInvocationException("Unable to invoke method.");
    }
    
    /**
     * Calling <code>invokeMethod(method, AutoConnection, ...)</code>.
     * @param <A> The type of the first parameter of the slot.
     * @param <B> The type of the second parameter of the slot.
     * @param <C> The type of the third parameter of the slot.
     * @param <D> The type of the fourth parameter of the slot.
     * @param <E> The type of the fifth parameter of the slot.
     * @param <F> The type of the sixth parameter of the slot.
     * @param <G> The type of the seventh parameter of the slot.
     * @param <H> The type of the eighth parameter of the slot.
     * @param method invoked method
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @param arg4 Argument for the fourth parameter.
     * @param arg5 Argument for the fifth parameter.
     * @param arg6 Argument for the sixth parameter.
     * @param arg7 Argument for the seventh parameter.
     * @param arg8 Argument for the eighth parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke slot
     */
    @QtUninvokable
    public static <A,B,C,D,E,F,G,H> void invokeMethod(Slot8<A,B,C,D,E,F,G,H> method, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7, H arg8) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
        invokeMethod(method, Qt.ConnectionType.AutoConnection, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
    }
    
    /**
     * <p>Invokes the slot.</p>
     * 
     * <p>The invocation can be either synchronous or asynchronous, depending on type:</p>
     * <ul>
     * <li>If type is {@link Qt.ConnectionType#DirectConnection}, the member will be invoked immediately.</li>
     * <li>If type is {@link Qt.ConnectionType#QueuedConnection}, a QEvent will be sent and the member is invoked as soon as the application enters the main event loop.</li>
     * <li>If type is {@link Qt.ConnectionType#BlockingQueuedConnection}, the method will be invoked in the same way as for {@link Qt.ConnectionType#QueuedConnection}, except that the current thread will block until the event is delivered. Using this connection type to communicate between objects in the same thread will lead to deadlocks.</li>
     * <li>If type is {@link Qt.ConnectionType#AutoConnection}, the member is invoked synchronously if obj lives in the same thread as the caller; otherwise it will invoke the member asynchronously.</li>
     * </ul>
     * 
     * @param <A> The type of the first parameter of the slot.
     * @param <B> The type of the second parameter of the slot.
     * @param <C> The type of the third parameter of the slot.
     * @param <D> The type of the fourth parameter of the slot.
     * @param <E> The type of the fifth parameter of the slot.
     * @param <F> The type of the sixth parameter of the slot.
     * @param <G> The type of the seventh parameter of the slot.
     * @param <H> The type of the eighth parameter of the slot.
     * @param method invoked method
     * @param type synchronous or asynchronous invokation
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @param arg4 Argument for the fourth parameter.
     * @param arg5 Argument for the fifth parameter.
     * @param arg6 Argument for the sixth parameter.
     * @param arg7 Argument for the seventh parameter.
     * @param arg8 Argument for the eighth parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke slot
     */
    @QtUninvokable
    public static <A,B,C,D,E,F,G,H> void invokeMethod(Slot8<A,B,C,D,E,F,G,H> method, Qt.ConnectionType type, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7, H arg8) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
        QtJambiInternal.LambdaInfo info = QtJambiInternal.lamdaInfo(method);
        QThread thread = null;
        if(info!=null && info.qobject!=null && !info.qobject.isDisposed()) {
            if(info.reflectiveMethod!=null) {
                QMetaMethod qmethod = fromReflectedMethod(info.reflectiveMethod);
                if(qmethod!=null && qmethod.isValid() && qmethod.parameterTypes().size()==info.lambdaArgs.size()+8) {
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
                    synchronized (invoker) {
                        invoker.disposeLater();
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
            QtJambi_LibraryUtilities.internal.setCppOwnership(invoker);
            if(thread!=null)
                invoker.moveToThread(thread);
            invoker.disposeLater();
            return;
        default:
            break;
        }
        throw new QUnsuccessfulInvocationException("Unable to invoke method.");
    }
    
    /**
     * Calling <code>invokeMethod(method, AutoConnection, ...)</code>.
     * @param <A> The type of the first parameter of the slot.
     * @param <B> The type of the second parameter of the slot.
     * @param <C> The type of the third parameter of the slot.
     * @param <D> The type of the fourth parameter of the slot.
     * @param <E> The type of the fifth parameter of the slot.
     * @param <F> The type of the sixth parameter of the slot.
     * @param <G> The type of the seventh parameter of the slot.
     * @param <H> The type of the eighth parameter of the slot.
     * @param <I> The type of the ninth parameter of the slot.
     * @param method invoked method
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @param arg4 Argument for the fourth parameter.
     * @param arg5 Argument for the fifth parameter.
     * @param arg6 Argument for the sixth parameter.
     * @param arg7 Argument for the seventh parameter.
     * @param arg8 Argument for the eighth parameter.
     * @param arg9 Argument for the ninth parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke slot
     */
    @QtUninvokable
    public static <A,B,C,D,E,F,G,H,I> void invokeMethod(Slot9<A,B,C,D,E,F,G,H,I> method, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7, H arg8, I arg9) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
        invokeMethod(method, Qt.ConnectionType.AutoConnection, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
    }
    
    /**
     * <p>Invokes the slot.</p>
     * 
     * <p>The invocation can be either synchronous or asynchronous, depending on type:</p>
     * <ul>
     * <li>If type is {@link Qt.ConnectionType#DirectConnection}, the member will be invoked immediately.</li>
     * <li>If type is {@link Qt.ConnectionType#QueuedConnection}, a QEvent will be sent and the member is invoked as soon as the application enters the main event loop.</li>
     * <li>If type is {@link Qt.ConnectionType#BlockingQueuedConnection}, the method will be invoked in the same way as for {@link Qt.ConnectionType#QueuedConnection}, except that the current thread will block until the event is delivered. Using this connection type to communicate between objects in the same thread will lead to deadlocks.</li>
     * <li>If type is {@link Qt.ConnectionType#AutoConnection}, the member is invoked synchronously if obj lives in the same thread as the caller; otherwise it will invoke the member asynchronously.</li>
     * </ul>
     * 
     * @param <A> The type of the first parameter of the slot.
     * @param <B> The type of the second parameter of the slot.
     * @param <C> The type of the third parameter of the slot.
     * @param <D> The type of the fourth parameter of the slot.
     * @param <E> The type of the fifth parameter of the slot.
     * @param <F> The type of the sixth parameter of the slot.
     * @param <G> The type of the seventh parameter of the slot.
     * @param <H> The type of the eighth parameter of the slot.
     * @param <I> The type of the ninth parameter of the slot.
     * @param method invoked method
     * @param type synchronous or asynchronous invokation
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @param arg4 Argument for the fourth parameter.
     * @param arg5 Argument for the fifth parameter.
     * @param arg6 Argument for the sixth parameter.
     * @param arg7 Argument for the seventh parameter.
     * @param arg8 Argument for the eighth parameter.
     * @param arg9 Argument for the ninth parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke slot
     */
    @QtUninvokable
    public static <A,B,C,D,E,F,G,H,I> void invokeMethod(Slot9<A,B,C,D,E,F,G,H,I> method, Qt.ConnectionType type, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7, H arg8, I arg9) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
        QtJambiInternal.LambdaInfo info = QtJambiInternal.lamdaInfo(method);
        QThread thread = null;
        if(info!=null && info.qobject!=null && !info.qobject.isDisposed()) {
            if(info.reflectiveMethod!=null) {
                QMetaMethod qmethod = fromReflectedMethod(info.reflectiveMethod);
                if(qmethod!=null && qmethod.isValid() && qmethod.parameterTypes().size()==info.lambdaArgs.size()+9) {
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
                    synchronized (invoker) {
                        invoker.disposeLater();
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
            QtJambi_LibraryUtilities.internal.setCppOwnership(invoker);
            if(thread!=null)
                invoker.moveToThread(thread);
            invoker.disposeLater();
            return;
        default:
            break;
        }
        throw new QUnsuccessfulInvocationException("Unable to invoke method.");
    }
    
    /**
     * Calling <code>invokeMethod(signal, AutoConnection)</code>.
     * @param signal invoked signal
     */
    @QtUninvokable
    public static void invokeMethod(AbstractPrivateSignal0 signal) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
        invokeMethod(signal, Qt.ConnectionType.AutoConnection);
    }
    
    /**
     * <p>Invokes the signal.</p>
     * 
     * <p>The invocation can be either synchronous or asynchronous, depending on type:</p>
     * <ul>
     * <li>If type is {@link Qt.ConnectionType#DirectConnection}, the member will be invoked immediately.</li>
     * <li>If type is {@link Qt.ConnectionType#QueuedConnection}, a QEvent will be sent and the member is invoked as soon as the application enters the main event loop.</li>
     * <li>If type is {@link Qt.ConnectionType#BlockingQueuedConnection}, the method will be invoked in the same way as for {@link Qt.ConnectionType#QueuedConnection}, except that the current thread will block until the event is delivered. Using this connection type to communicate between objects in the same thread will lead to deadlocks.</li>
     * <li>If type is {@link Qt.ConnectionType#AutoConnection}, the member is invoked synchronously if obj lives in the same thread as the caller; otherwise it will invoke the member asynchronously.</li>
     * </ul>
     * 
     * @param signal invoked signal
     * @param type synchronous or asynchronous invokation
     * @throws QUnsuccessfulInvocationException if not able to invoke signal
     */
    @QtUninvokable
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
    
    /**
     * Calling <code>invokeMethod(signal, AutoConnection, ...)</code>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param signal invoked signal
     * @param arg1 Argument for the first parameter.
     */
    @QtUninvokable
    public static <A> void invokeMethod(AbstractPrivateSignal1<A> signal, A arg1) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
        invokeMethod(signal, Qt.ConnectionType.AutoConnection, arg1);
    }
    
    /**
     * <p>Invokes the signal.</p>
     * 
     * <p>The invocation can be either synchronous or asynchronous, depending on type:</p>
     * <ul>
     * <li>If type is {@link Qt.ConnectionType#DirectConnection}, the member will be invoked immediately.</li>
     * <li>If type is {@link Qt.ConnectionType#QueuedConnection}, a QEvent will be sent and the member is invoked as soon as the application enters the main event loop.</li>
     * <li>If type is {@link Qt.ConnectionType#BlockingQueuedConnection}, the method will be invoked in the same way as for {@link Qt.ConnectionType#QueuedConnection}, except that the current thread will block until the event is delivered. Using this connection type to communicate between objects in the same thread will lead to deadlocks.</li>
     * <li>If type is {@link Qt.ConnectionType#AutoConnection}, the member is invoked synchronously if obj lives in the same thread as the caller; otherwise it will invoke the member asynchronously.</li>
     * </ul>
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param signal invoked signal
     * @param type synchronous or asynchronous invokation
     * @param arg1 Argument for the first parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke signal
     */
    @QtUninvokable
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
    
    /**
     * Calling <code>invokeMethod(signal, AutoConnection, ...)</code>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param signal invoked signal
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     */
    @QtUninvokable
    public static <A,B> void invokeMethod(AbstractPrivateSignal2<A,B> signal, A arg1, B arg2) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
        invokeMethod(signal, Qt.ConnectionType.AutoConnection, arg1, arg2);
    }
    
    /**
     * <p>Invokes the signal.</p>
     * 
     * <p>The invocation can be either synchronous or asynchronous, depending on type:</p>
     * <ul>
     * <li>If type is {@link Qt.ConnectionType#DirectConnection}, the member will be invoked immediately.</li>
     * <li>If type is {@link Qt.ConnectionType#QueuedConnection}, a QEvent will be sent and the member is invoked as soon as the application enters the main event loop.</li>
     * <li>If type is {@link Qt.ConnectionType#BlockingQueuedConnection}, the method will be invoked in the same way as for {@link Qt.ConnectionType#QueuedConnection}, except that the current thread will block until the event is delivered. Using this connection type to communicate between objects in the same thread will lead to deadlocks.</li>
     * <li>If type is {@link Qt.ConnectionType#AutoConnection}, the member is invoked synchronously if obj lives in the same thread as the caller; otherwise it will invoke the member asynchronously.</li>
     * </ul>
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param signal invoked signal
     * @param type synchronous or asynchronous invokation
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke signal
     */
    @QtUninvokable
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
    
    /**
     * Calling <code>invokeMethod(signal, AutoConnection, ...)</code>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param signal invoked signal
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     */
    @QtUninvokable
    public static <A,B,C> void invokeMethod(AbstractPrivateSignal3<A,B,C> signal, A arg1, B arg2, C arg3) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
        invokeMethod(signal, Qt.ConnectionType.AutoConnection, arg1, arg2, arg3);
    }
    
    /**
     * <p>Invokes the signal.</p>
     * 
     * <p>The invocation can be either synchronous or asynchronous, depending on type:</p>
     * <ul>
     * <li>If type is {@link Qt.ConnectionType#DirectConnection}, the member will be invoked immediately.</li>
     * <li>If type is {@link Qt.ConnectionType#QueuedConnection}, a QEvent will be sent and the member is invoked as soon as the application enters the main event loop.</li>
     * <li>If type is {@link Qt.ConnectionType#BlockingQueuedConnection}, the method will be invoked in the same way as for {@link Qt.ConnectionType#QueuedConnection}, except that the current thread will block until the event is delivered. Using this connection type to communicate between objects in the same thread will lead to deadlocks.</li>
     * <li>If type is {@link Qt.ConnectionType#AutoConnection}, the member is invoked synchronously if obj lives in the same thread as the caller; otherwise it will invoke the member asynchronously.</li>
     * </ul>
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param signal invoked signal
     * @param type synchronous or asynchronous invokation
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke signal
     */
    @QtUninvokable
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
    
    /**
     * Calling <code>invokeMethod(signal, AutoConnection, ...)</code>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param signal invoked signal
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @param arg4 Argument for the fourth parameter.
     */
    @QtUninvokable
    public static <A,B,C,D> void invokeMethod(AbstractPrivateSignal4<A,B,C,D> signal, A arg1, B arg2, C arg3, D arg4) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
        invokeMethod(signal, Qt.ConnectionType.AutoConnection, arg1, arg2, arg3, arg4);
    }
    
    /**
     * <p>Invokes the signal.</p>
     * 
     * <p>The invocation can be either synchronous or asynchronous, depending on type:</p>
     * <ul>
     * <li>If type is {@link Qt.ConnectionType#DirectConnection}, the member will be invoked immediately.</li>
     * <li>If type is {@link Qt.ConnectionType#QueuedConnection}, a QEvent will be sent and the member is invoked as soon as the application enters the main event loop.</li>
     * <li>If type is {@link Qt.ConnectionType#BlockingQueuedConnection}, the method will be invoked in the same way as for {@link Qt.ConnectionType#QueuedConnection}, except that the current thread will block until the event is delivered. Using this connection type to communicate between objects in the same thread will lead to deadlocks.</li>
     * <li>If type is {@link Qt.ConnectionType#AutoConnection}, the member is invoked synchronously if obj lives in the same thread as the caller; otherwise it will invoke the member asynchronously.</li>
     * </ul>
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param signal invoked signal
     * @param type synchronous or asynchronous invokation
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @param arg4 Argument for the fourth parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke signal
     */
    @QtUninvokable
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
    
    /**
     * Calling <code>invokeMethod(signal, AutoConnection, ...)</code>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param signal invoked signal
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @param arg4 Argument for the fourth parameter.
     * @param arg5 Argument for the fifth parameter.
     */
    @QtUninvokable
    public static <A,B,C,D,E> void invokeMethod(AbstractPrivateSignal5<A,B,C,D,E> signal, A arg1, B arg2, C arg3, D arg4, E arg5) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
        invokeMethod(signal, Qt.ConnectionType.AutoConnection, arg1, arg2, arg3, arg4, arg5);
    }
    
    /**
     * <p>Invokes the signal.</p>
     * 
     * <p>The invocation can be either synchronous or asynchronous, depending on type:</p>
     * <ul>
     * <li>If type is {@link Qt.ConnectionType#DirectConnection}, the member will be invoked immediately.</li>
     * <li>If type is {@link Qt.ConnectionType#QueuedConnection}, a QEvent will be sent and the member is invoked as soon as the application enters the main event loop.</li>
     * <li>If type is {@link Qt.ConnectionType#BlockingQueuedConnection}, the method will be invoked in the same way as for {@link Qt.ConnectionType#QueuedConnection}, except that the current thread will block until the event is delivered. Using this connection type to communicate between objects in the same thread will lead to deadlocks.</li>
     * <li>If type is {@link Qt.ConnectionType#AutoConnection}, the member is invoked synchronously if obj lives in the same thread as the caller; otherwise it will invoke the member asynchronously.</li>
     * </ul>
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param signal invoked signal
     * @param type synchronous or asynchronous invokation
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @param arg4 Argument for the fourth parameter.
     * @param arg5 Argument for the fifth parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke signal
     */
    @QtUninvokable
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
    
    /**
     * Calling <code>invokeMethod(signal, AutoConnection, ...)</code>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param signal invoked signal
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @param arg4 Argument for the fourth parameter.
     * @param arg5 Argument for the fifth parameter.
     */
    @QtUninvokable
    public static <A,B,C,D,E,F> void invokeMethod(AbstractPrivateSignal6<A,B,C,D,E,F> signal, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
        invokeMethod(signal, Qt.ConnectionType.AutoConnection, arg1, arg2, arg3, arg4, arg5, arg6);
    }
    
    /**
     * <p>Invokes the signal.</p>
     * 
     * <p>The invocation can be either synchronous or asynchronous, depending on type:</p>
     * <ul>
     * <li>If type is {@link Qt.ConnectionType#DirectConnection}, the member will be invoked immediately.</li>
     * <li>If type is {@link Qt.ConnectionType#QueuedConnection}, a QEvent will be sent and the member is invoked as soon as the application enters the main event loop.</li>
     * <li>If type is {@link Qt.ConnectionType#BlockingQueuedConnection}, the method will be invoked in the same way as for {@link Qt.ConnectionType#QueuedConnection}, except that the current thread will block until the event is delivered. Using this connection type to communicate between objects in the same thread will lead to deadlocks.</li>
     * <li>If type is {@link Qt.ConnectionType#AutoConnection}, the member is invoked synchronously if obj lives in the same thread as the caller; otherwise it will invoke the member asynchronously.</li>
     * </ul>
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param signal invoked signal
     * @param type synchronous or asynchronous invokation
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @param arg4 Argument for the fourth parameter.
     * @param arg5 Argument for the fifth parameter.
     * @param arg6 Argument for the sixth parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke signal
     */
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
    
    /**
     * Calling <code>invokeMethod(signal, AutoConnection, ...)</code>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     * @param signal invoked signal
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @param arg4 Argument for the fourth parameter.
     * @param arg5 Argument for the fifth parameter.
     * @param arg6 Argument for the sixth parameter.
     */
    @QtUninvokable
    public static <A,B,C,D,E,F,G> void invokeMethod(AbstractPrivateSignal7<A,B,C,D,E,F,G> signal, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
        invokeMethod(signal, Qt.ConnectionType.AutoConnection, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
    }
    
    /**
     * <p>Invokes the signal.</p>
     * 
     * <p>The invocation can be either synchronous or asynchronous, depending on type:</p>
     * <ul>
     * <li>If type is {@link Qt.ConnectionType#DirectConnection}, the member will be invoked immediately.</li>
     * <li>If type is {@link Qt.ConnectionType#QueuedConnection}, a QEvent will be sent and the member is invoked as soon as the application enters the main event loop.</li>
     * <li>If type is {@link Qt.ConnectionType#BlockingQueuedConnection}, the method will be invoked in the same way as for {@link Qt.ConnectionType#QueuedConnection}, except that the current thread will block until the event is delivered. Using this connection type to communicate between objects in the same thread will lead to deadlocks.</li>
     * <li>If type is {@link Qt.ConnectionType#AutoConnection}, the member is invoked synchronously if obj lives in the same thread as the caller; otherwise it will invoke the member asynchronously.</li>
     * </ul>
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     * @param signal invoked signal
     * @param type synchronous or asynchronous invokation
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @param arg4 Argument for the fourth parameter.
     * @param arg5 Argument for the fifth parameter.
     * @param arg6 Argument for the sixth parameter.
     * @param arg7 Argument for the seventh parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke signal
     */
    @QtUninvokable
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
    
    /**
     * Calling <code>invokeMethod(signal, AutoConnection, ...)</code>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     * @param <H> The type of the eighth parameter of the signal.
     * @param signal invoked signal
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @param arg4 Argument for the fourth parameter.
     * @param arg5 Argument for the fifth parameter.
     * @param arg6 Argument for the sixth parameter.
     * @param arg7 Argument for the seventh parameter.
     */
    @QtUninvokable
    public static <A,B,C,D,E,F,G,H> void invokeMethod(AbstractPrivateSignal8<A,B,C,D,E,F,G,H> signal, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7, H arg8) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
        invokeMethod(signal, Qt.ConnectionType.AutoConnection, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
    }
    
    /**
     * <p>Invokes the signal.</p>
     * 
     * <p>The invocation can be either synchronous or asynchronous, depending on type:</p>
     * <ul>
     * <li>If type is {@link Qt.ConnectionType#DirectConnection}, the member will be invoked immediately.</li>
     * <li>If type is {@link Qt.ConnectionType#QueuedConnection}, a QEvent will be sent and the member is invoked as soon as the application enters the main event loop.</li>
     * <li>If type is {@link Qt.ConnectionType#BlockingQueuedConnection}, the method will be invoked in the same way as for {@link Qt.ConnectionType#QueuedConnection}, except that the current thread will block until the event is delivered. Using this connection type to communicate between objects in the same thread will lead to deadlocks.</li>
     * <li>If type is {@link Qt.ConnectionType#AutoConnection}, the member is invoked synchronously if obj lives in the same thread as the caller; otherwise it will invoke the member asynchronously.</li>
     * </ul>
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     * @param <H> The type of the eighth parameter of the signal.
     * @param signal invoked signal
     * @param type synchronous or asynchronous invokation
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @param arg4 Argument for the fourth parameter.
     * @param arg5 Argument for the fifth parameter.
     * @param arg6 Argument for the sixth parameter.
     * @param arg7 Argument for the seventh parameter.
     * @param arg8 Argument for the eighth parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke signal
     */
    @QtUninvokable
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
    
    /**
     * Calling <code>invokeMethod(signal, AutoConnection, ...)</code>.
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
     * @param signal invoked signal
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @param arg4 Argument for the fourth parameter.
     * @param arg5 Argument for the fifth parameter.
     * @param arg6 Argument for the sixth parameter.
     * @param arg7 Argument for the seventh parameter.
     * @param arg8 Argument for the eighth parameter.
     * @param arg9 Argument for the ninth parameter.
     */
    @QtUninvokable
    public static <A,B,C,D,E,F,G,H,I> void invokeMethod(AbstractPrivateSignal9<A,B,C,D,E,F,G,H,I> signal, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7, H arg8, I arg9) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
        invokeMethod(signal, Qt.ConnectionType.AutoConnection, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
    }
    
    /**
     * <p>Invokes the signal.</p>
     * 
     * <p>The invocation can be either synchronous or asynchronous, depending on type:</p>
     * <ul>
     * <li>If type is {@link Qt.ConnectionType#DirectConnection}, the member will be invoked immediately.</li>
     * <li>If type is {@link Qt.ConnectionType#QueuedConnection}, a QEvent will be sent and the member is invoked as soon as the application enters the main event loop.</li>
     * <li>If type is {@link Qt.ConnectionType#BlockingQueuedConnection}, the method will be invoked in the same way as for {@link Qt.ConnectionType#QueuedConnection}, except that the current thread will block until the event is delivered. Using this connection type to communicate between objects in the same thread will lead to deadlocks.</li>
     * <li>If type is {@link Qt.ConnectionType#AutoConnection}, the member is invoked synchronously if obj lives in the same thread as the caller; otherwise it will invoke the member asynchronously.</li>
     * </ul>
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
     * @param signal invoked signal
     * @param type synchronous or asynchronous invokation
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @param arg4 Argument for the fourth parameter.
     * @param arg5 Argument for the fifth parameter.
     * @param arg6 Argument for the sixth parameter.
     * @param arg7 Argument for the seventh parameter.
     * @param arg8 Argument for the eighth parameter.
     * @param arg9 Argument for the ninth parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke signal
     */
    @QtUninvokable
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
        static abstract class AbstractSignal extends QtJambiSignals.AbstractSignal {
            AbstractSignal(){}
        
            AbstractSignal(boolean isGeneric, Class<?>... types) {
                super(isGeneric, types);
            }
            
            AbstractSignal(Class<?> declaringClass, boolean isStatic) {
                super(declaringClass, isStatic);
            }
            
            AbstractSignal(String signalName, Class<?>... types) {
                super(signalName, types);
            }
        }
        
        static abstract class AbstractMultiSignal extends QtJambiSignals.AbstractMultiSignal<AbstractSignal> {
            AbstractMultiSignal() {
                super();
            }
        }
        
        static void checkConnectionToDisposedSignal(QMetaObject.DisposedSignal signal, Object receiver, boolean slotObject) {
            QtJambiSignals.checkConnectionToDisposedSignalImpl(signal, receiver, slotObject);
        }
        
        static QtJambiPropertyInfo _analyzeProperty(QObject object, QtObject property) {
        	return QtJambiSignals.analyzeProperty(object, property);
        }
        
        static void _registerPropertyField(QMetaProperty metaProperty, java.lang.reflect.Field field) {
        	QtJambiSignals.registerPropertyField(metaProperty, field);
        }
        
        static void _emitNativeSignal(QObject sender, int methodIndex, Object args[]) {
        	QtJambiSignals.emitNativeSignal(sender, methodIndex, 0, args);
        }
        
        static Class<?> _signalDeclaringClass(AbstractSignal signal){
        	return QtJambiSignals.signalDeclaringClass(signal);
        }
        
        static boolean _disconnectAll(QtSignalEmitterInterface sender, Object receiver) {
        	return QtJambiSignals.disconnectAll(sender, receiver);
        }
        
        static boolean _disconnectOne(QMetaObject.Connection connection) {
        	return QtJambiSignals.disconnectOne(connection);
        }
        
        private static void initialize() {}
    }
    
    static abstract class AbstractMultiSignal extends SignalAccess.AbstractMultiSignal {
        AbstractMultiSignal(){
        	super();
    	}
    }
    
    static {
        SignalAccess.initialize();
    }
    
    /**
     * Signal emitted when a {@link io.qt.QtObjectInterface} is being disposed.
     * @see QtObjectInterface#isDisposed()
     * @see QtObjectInterface#dispose()
     */
    public static final class DisposedSignal extends AbstractSignal {
        private DisposedSignal(Class<?> declaringClass) {
            super(declaringClass, true);
        }
        
        /**
         * Internal
         */
        @Override
        protected final void checkConnection(Object receiver, boolean slotObject) {
            SignalAccess.checkConnectionToDisposedSignal(this, receiver, slotObject);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
         */
        public final QMetaObject.Connection connect(Slot0 slot, Qt.ConnectionType... connectionType) {
            return addConnectionToSlotObject(slot, connectionType);
        }
    
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(Slot0 slot) {
            return removeConnectionToSlotObject(slot);
        }
    }
    
    /**
     * Supertype of all signals in QtJambi.
     */
    public static abstract class AbstractSignal extends SignalAccess.AbstractSignal implements Signal {
        
        AbstractSignal(){}
    
        AbstractSignal(boolean isGeneric, Class<?>... types) {
            super(isGeneric, types);
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
         * @param connectionType One of the connection types defined in the Qt interface.
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
            if (receiver == null && this instanceof DisposedSignal)
            	return false;
            if(receiver instanceof QMetaObject.Connection && method==null){
                return removeConnection((QMetaObject.Connection)receiver);
            }else{
            	return removeConnection(receiver, method);
            }
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
         * Removes the given connection from this signal.
         *
         * @param connection the connection to be removed
         * @return true if the disconnection was successful.
         */
        public final boolean disconnect(QMetaObject.Connection connection) {
            return removeConnection(connection);
        }

		static QtJambiPropertyInfo analyzeProperty(QObject object, QtObject property) {
			return SignalAccess._analyzeProperty(object, property);
		}
		
		static void registerPropertyField(QMetaProperty metaProperty, java.lang.reflect.Field field) {
			SignalAccess._registerPropertyField(metaProperty, field);
		}
		
		static void emitNativeSignal(QObject sender, int methodIndex, Object... args) {
			SignalAccess._emitNativeSignal(sender, methodIndex, args);
		}
		
		Class<?> signalDeclaringClass(){
			return SignalAccess._signalDeclaringClass(this);
		}
		
		static Class<?> signalDeclaringClass(AbstractSignal signal){
        	return SignalAccess._signalDeclaringClass(signal);
        }
		
		static boolean disconnectAll(QtSignalEmitterInterface sender, Object receiver) {
			return SignalAccess._disconnectAll(sender, receiver);
		}
		
		static boolean disconnectOne(QMetaObject.Connection connection) {
			return SignalAccess._disconnectOne(connection);
		}
    }
    
    /**
     * Supertype of all slot handles.
     */
    public static interface AbstractSlot extends Serializable{
    }

    /**
     * A generic slot handle to a method of variadic arguments.
     */
    @FunctionalInterface
    public static interface GenericSlot extends AbstractSlot{
        public void invoke(Object... args) throws Throwable;
    }

    /**
     * A handle to parameterless slot.
     */
    @FunctionalInterface
    public static interface Slot0 extends AbstractSlot{
        public void invoke() throws Throwable;
    }

    /**
     * A handle to slot with one parameter.
     * @param <A> The type of the first parameter of the slot.
     */
    @FunctionalInterface
    public static interface Slot1<A> extends AbstractSlot{
        public void invoke(A a) throws Throwable;
    }

    /**
     * A handle to slot with two parameters.
     * @param <A> The type of the first parameter of the slot.
     * @param <B> The type of the second parameter of the slot.
     */
    @FunctionalInterface
    public static interface Slot2<A,B> extends AbstractSlot{
        public void invoke(A a, B b) throws Throwable;
    }
    
    /**
     * A handle to slot with three parameters.
     * @param <A> The type of the first parameter of the slot.
     * @param <B> The type of the second parameter of the slot.
     * @param <C> The type of the third parameter of the slot.
     */
    @FunctionalInterface
    public static interface Slot3<A,B,C> extends AbstractSlot{
        public void invoke(A a, B b, C c) throws Throwable;
    }
    
    /**
     * A handle to slot with four parameters.
     * @param <A> The type of the first parameter of the slot.
     * @param <B> The type of the second parameter of the slot.
     * @param <C> The type of the third parameter of the slot.
     * @param <D> The type of the fourth parameter of the slot.
     */
    @FunctionalInterface
    public static interface Slot4<A,B,C,D> extends AbstractSlot{
        public void invoke(A a, B b, C c, D d) throws Throwable;
    }
    
    /**
     * A handle to slot with five parameters.
     * @param <A> The type of the first parameter of the slot.
     * @param <B> The type of the second parameter of the slot.
     * @param <C> The type of the third parameter of the slot.
     * @param <D> The type of the fourth parameter of the slot.
     * @param <E> The type of the fifth parameter of the slot.
     */
    @FunctionalInterface
    public static interface Slot5<A,B,C,D,E> extends AbstractSlot{
        public void invoke(A a, B b, C c, D d, E e) throws Throwable;
    }
    
    /**
     * A handle to slot with six parameters.
     * @param <A> The type of the first parameter of the slot.
     * @param <B> The type of the second parameter of the slot.
     * @param <C> The type of the third parameter of the slot.
     * @param <D> The type of the fourth parameter of the slot.
     * @param <E> The type of the fifth parameter of the slot.
     * @param <F> The type of the sixth parameter of the slot.
     */
    @FunctionalInterface
    public static interface Slot6<A,B,C,D,E,F> extends AbstractSlot{
        public void invoke(A a, B b, C c, D d, E e, F f) throws Throwable;
    }
    
    /**
     * A handle to slot with seven parameters.
     * @param <A> The type of the first parameter of the slot.
     * @param <B> The type of the second parameter of the slot.
     * @param <C> The type of the third parameter of the slot.
     * @param <D> The type of the fourth parameter of the slot.
     * @param <E> The type of the fifth parameter of the slot.
     * @param <F> The type of the sixth parameter of the slot.
     * @param <G> The type of the seventh parameter of the slot.
     */
    @FunctionalInterface
    public static interface Slot7<A,B,C,D,E,F,G> extends AbstractSlot{
        public void invoke(A a, B b, C c, D d, E e, F f, G g) throws Throwable;
    }
    
    /**
     * A handle to slot with eight parameters.
     * @param <A> The type of the first parameter of the slot.
     * @param <B> The type of the second parameter of the slot.
     * @param <C> The type of the third parameter of the slot.
     * @param <D> The type of the fourth parameter of the slot.
     * @param <E> The type of the fifth parameter of the slot.
     * @param <F> The type of the sixth parameter of the slot.
     * @param <G> The type of the seventh parameter of the slot.
     * @param <H> The type of the eighth parameter of the slot.
     */
    @FunctionalInterface
    public static interface Slot8<A,B,C,D,E,F,G,H> extends AbstractSlot{
        public void invoke(A a, B b, C c, D d, E e, F f, G g, H h) throws Throwable;
    }
    
    /**
     * A handle to slot with nine parameters.
     * @param <A> The type of the first parameter of the slot.
     * @param <B> The type of the second parameter of the slot.
     * @param <C> The type of the third parameter of the slot.
     * @param <D> The type of the fourth parameter of the slot.
     * @param <E> The type of the fifth parameter of the slot.
     * @param <F> The type of the sixth parameter of the slot.
     * @param <G> The type of the seventh parameter of the slot.
     * @param <H> The type of the eighth parameter of the slot.
     * @param <I> The type of the ninth parameter of the slot.
     */
    @FunctionalInterface
    public static interface Slot9<A,B,C,D,E,F,G,H,I> extends AbstractSlot{
        public void invoke(A a, B b, C c, D d, E e, F f, G g, H h, I i) throws Throwable;
    }
    
    public static interface Signal{
    	String name();
    	String fullName();
    	QtSignalEmitterInterface containingObject();
    	int methodIndex();
    	List<Class<?>> argumentTypes();
    }
    
    /**
     * An interface to a generic signal.
     */
    public static interface GenericConnectable extends Signal{
    }

    /**
     * An interface to signal callable without parameters.
     */
    public static interface Connectable0 extends Signal{
    }

    /**
     * An interface for anything connectable to a signal with one parameter.
     * @param <A> The type of the first parameter.
     */
    public static interface Connectable1<A> extends Signal{
    }

    /**
     * An interface for anything connectable to a signal with two parameters.
     * @param <A> The type of the first parameter.
     * @param <B> The type of the second parameter.
     */
    public static interface Connectable2<A,B> extends Signal{
    }
    
    /**
     * An interface for anything connectable to a signal with three parameters.
     * @param <A> The type of the first parameter.
     * @param <B> The type of the second parameter.
     * @param <C> The type of the third parameter.
     */
    public static interface Connectable3<A,B,C> extends Signal{
    }
    
    /**
     * An interface for anything connectable to a signal with four parameters.
     * @param <A> The type of the first parameter.
     * @param <B> The type of the second parameter.
     * @param <C> The type of the third parameter.
     * @param <D> The type of the fourth parameter.
     */
    public static interface Connectable4<A,B,C,D> extends Signal{
    }
    
    /**
     * An interface for anything connectable to a signal with five parameters.
     * @param <A> The type of the first parameter.
     * @param <B> The type of the second parameter.
     * @param <C> The type of the third parameter.
     * @param <D> The type of the fourth parameter.
     * @param <E> The type of the fifth parameter.
     */
    public static interface Connectable5<A,B,C,D,E> extends Signal{
    }
    
    /**
     * An interface for anything connectable to a signal with six parameters.
     * @param <A> The type of the first parameter.
     * @param <B> The type of the second parameter.
     * @param <C> The type of the third parameter.
     * @param <D> The type of the fourth parameter.
     * @param <E> The type of the fifth parameter.
     * @param <F> The type of the sixth parameter.
     */
    public static interface Connectable6<A,B,C,D,E,F> extends Signal{
    }
    
    /**
     * An interface for anything connectable to a signal with seven parameters.
     * @param <A> The type of the first parameter.
     * @param <B> The type of the second parameter.
     * @param <C> The type of the third parameter.
     * @param <D> The type of the fourth parameter.
     * @param <E> The type of the fifth parameter.
     * @param <F> The type of the sixth parameter.
     * @param <G> The type of the seventh parameter.
     */
    public static interface Connectable7<A,B,C,D,E,F,G> extends Signal{
    }
    
    /**
     * An interface for anything connectable to a signal with eight parameters.
     * @param <A> The type of the first parameter.
     * @param <B> The type of the second parameter.
     * @param <C> The type of the third parameter.
     * @param <D> The type of the fourth parameter.
     * @param <E> The type of the fifth parameter.
     * @param <F> The type of the sixth parameter.
     * @param <G> The type of the seventh parameter.
     * @param <H> The type of the eighth parameter.
     */
    public static interface Connectable8<A,B,C,D,E,F,G,H> extends Signal{
    }
    
    /**
     * An interface for anything connectable to a signal with nine parameters.
     * @param <A> The type of the first parameter.
     * @param <B> The type of the second parameter.
     * @param <C> The type of the third parameter.
     * @param <D> The type of the fourth parameter.
     * @param <E> The type of the fifth parameter.
     * @param <F> The type of the sixth parameter.
     * @param <G> The type of the seventh parameter.
     * @param <H> The type of the eighth parameter.
     * @param <I> The type of the ninth parameter.
     */
    public static interface Connectable9<A,B,C,D,E,F,G,H,I> extends Signal{
    }
    
    /**
     * An interface to parameterless signal.
     */
    public static interface Emitable0 extends Signal{
        public void emit();
    }

    /**
     * An interface for anything connectable to a signal with one parameter.
     * @param <A> The type of the first parameter.
     */
    public static interface Emitable1<A> extends Signal{
        public void emit(A a);
    }

    /**
     * An interface for anything connectable to a signal with two parameters.
     * @param <A> The type of the first parameter.
     * @param <B> The type of the second parameter.
     */
    public static interface Emitable2<A,B> extends Signal{
        public void emit(A a, B b);
    }
    
    /**
     * An interface for anything connectable to a signal with three parameters.
     * @param <A> The type of the first parameter.
     * @param <B> The type of the second parameter.
     * @param <C> The type of the third parameter.
     */
    public static interface Emitable3<A,B,C> extends Signal{
        public void emit(A a, B b, C c);
    }
    
    /**
     * An interface for anything connectable to a signal with four parameters.
     * @param <A> The type of the first parameter.
     * @param <B> The type of the second parameter.
     * @param <C> The type of the third parameter.
     * @param <D> The type of the fourth parameter.
     */
    public static interface Emitable4<A,B,C,D> extends Signal{
        public void emit(A a, B b, C c, D d);
    }
    
    /**
     * An interface for anything connectable to a signal with five parameters.
     * @param <A> The type of the first parameter.
     * @param <B> The type of the second parameter.
     * @param <C> The type of the third parameter.
     * @param <D> The type of the fourth parameter.
     * @param <E> The type of the fifth parameter.
     */
    public static interface Emitable5<A,B,C,D,E> extends Signal{
        public void emit(A a, B b, C c, D d, E e);
    }
    
    /**
     * An interface for anything connectable to a signal with six parameters.
     * @param <A> The type of the first parameter.
     * @param <B> The type of the second parameter.
     * @param <C> The type of the third parameter.
     * @param <D> The type of the fourth parameter.
     * @param <E> The type of the fifth parameter.
     * @param <F> The type of the sixth parameter.
     */
    public static interface Emitable6<A,B,C,D,E,F> extends Signal{
        public void emit(A a, B b, C c, D d, E e, F f);
    }
    
    /**
     * An interface for anything connectable to a signal with seven parameters.
     * @param <A> The type of the first parameter.
     * @param <B> The type of the second parameter.
     * @param <C> The type of the third parameter.
     * @param <D> The type of the fourth parameter.
     * @param <E> The type of the fifth parameter.
     * @param <F> The type of the sixth parameter.
     * @param <G> The type of the seventh parameter.
     */
    public static interface Emitable7<A,B,C,D,E,F,G> extends Signal{
        public void emit(A a, B b, C c, D d, E e, F f, G g);
    }
    
    /**
     * An interface for anything connectable to a signal with eight parameters.
     * @param <A> The type of the first parameter.
     * @param <B> The type of the second parameter.
     * @param <C> The type of the third parameter.
     * @param <D> The type of the fourth parameter.
     * @param <E> The type of the fifth parameter.
     * @param <F> The type of the sixth parameter.
     * @param <G> The type of the seventh parameter.
     * @param <H> The type of the eighth parameter.
     */
    public static interface Emitable8<A,B,C,D,E,F,G,H> extends Signal{
        public void emit(A a, B b, C c, D d, E e, F f, G g, H h);
    }
    
    /**
     * An interface for anything connectable to a signal with nine parameters.
     * @param <A> The type of the first parameter.
     * @param <B> The type of the second parameter.
     * @param <C> The type of the third parameter.
     * @param <D> The type of the fourth parameter.
     * @param <E> The type of the fifth parameter.
     * @param <F> The type of the sixth parameter.
     * @param <G> The type of the seventh parameter.
     * @param <H> The type of the eighth parameter.
     * @param <I> The type of the ninth parameter.
     */
    public static interface Emitable9<A,B,C,D,E,F,G,H,I> extends Signal{
        public void emit(A a, B b, C c, D d, E e, F f, G g, H h, I i);
    }
    
    /**
     * A generic slot handle to a method of variadic arguments with return value.
     * @param <R> The type of the return value of the method.
     */
    @FunctionalInterface
    public static interface GenericMethod<R> extends AbstractSlot{
        public R invoke(Object... args) throws Throwable;
    }

    /**
     * A handle to parameterless method with return value.
     * @param <R> The type of the return value of the method.
     */
    @FunctionalInterface
    public static interface Method0<R> extends AbstractSlot{
        public R invoke() throws Throwable;
    }

    /**
     * A handle to method with onw parameter and return value.
     * @param <A> The type of the first parameter of the method.
     * @param <R> The type of the return value of the method.
     */
    @FunctionalInterface
    public static interface Method1<A,R> extends AbstractSlot{
        public R invoke(A a) throws Throwable;
    }

    /**
     * A handle to method with two parameters and return value.
     * @param <A> The type of the first parameter of the method.
     * @param <B> The type of the second parameter of the method.
     * @param <R> The type of the return value of the method.
     */
    @FunctionalInterface
    public static interface Method2<A,B,R> extends AbstractSlot{
        public R invoke(A a, B b) throws Throwable;
    }
    
    /**
     * A handle to method with three parameters and return value.
     * @param <A> The type of the first parameter of the method.
     * @param <B> The type of the second parameter of the method.
     * @param <C> The type of the third parameter of the method.
     * @param <R> The type of the return value of the method.
     */
    @FunctionalInterface
    public static interface Method3<A,B,C,R> extends AbstractSlot{
        public R invoke(A a, B b, C c) throws Throwable;
    }
    
    /**
     * A handle to method with four parameters and return value.
     * @param <A> The type of the first parameter of the method.
     * @param <B> The type of the second parameter of the method.
     * @param <C> The type of the third parameter of the method.
     * @param <D> The type of the fourth parameter of the method.
     * @param <R> The type of the return value of the method.
     */
    @FunctionalInterface
    public static interface Method4<A,B,C,D,R> extends AbstractSlot{
        public R invoke(A a, B b, C c, D d) throws Throwable;
    }
    
    /**
     * A handle to method with five parameters and return value.
     * @param <A> The type of the first parameter of the method.
     * @param <B> The type of the second parameter of the method.
     * @param <C> The type of the third parameter of the method.
     * @param <D> The type of the fourth parameter of the method.
     * @param <E> The type of the fifth parameter of the method.
     * @param <R> The type of the return value of the method.
     */
    @FunctionalInterface
    public static interface Method5<A,B,C,D,E,R> extends AbstractSlot{
        public R invoke(A a, B b, C c, D d, E e) throws Throwable;
    }
    
    /**
     * A handle to method with six parameters and return value.
     * @param <A> The type of the first parameter of the method.
     * @param <B> The type of the second parameter of the method.
     * @param <C> The type of the third parameter of the method.
     * @param <D> The type of the fourth parameter of the method.
     * @param <E> The type of the fifth parameter of the method.
     * @param <F> The type of the sixth parameter of the method.
     * @param <R> The type of the return value of the method.
     */
    @FunctionalInterface
    public static interface Method6<A,B,C,D,E,F,R> extends AbstractSlot{
        public R invoke(A a, B b, C c, D d, E e, F f) throws Throwable;
    }
    
    /**
     * A handle to method with seven parameters and return value.
     * @param <A> The type of the first parameter of the method.
     * @param <B> The type of the second parameter of the method.
     * @param <C> The type of the third parameter of the method.
     * @param <D> The type of the fourth parameter of the method.
     * @param <E> The type of the fifth parameter of the method.
     * @param <F> The type of the sixth parameter of the method.
     * @param <G> The type of the seventh parameter of the method.
     * @param <R> The type of the return value of the method.
     */
    @FunctionalInterface
    public static interface Method7<A,B,C,D,E,F,G,R> extends AbstractSlot{
        public R invoke(A a, B b, C c, D d, E e, F f, G g) throws Throwable;
    }
    
    /**
     * A handle to method with eight parameters and return value.
     * @param <A> The type of the first parameter of the method.
     * @param <B> The type of the second parameter of the method.
     * @param <C> The type of the third parameter of the method.
     * @param <D> The type of the fourth parameter of the method.
     * @param <E> The type of the fifth parameter of the method.
     * @param <F> The type of the sixth parameter of the method.
     * @param <G> The type of the seventh parameter of the method.
     * @param <H> The type of the eighth parameter of the method.
     * @param <R> The type of the return value of the method.
     */
    @FunctionalInterface
    public static interface Method8<A,B,C,D,E,F,G,H,R> extends AbstractSlot{
        public R invoke(A a, B b, C c, D d, E e, F f, G g, H h) throws Throwable;
    }
    
    /**
     * A handle to method with nine parameters and return value.
     * @param <A> The type of the first parameter of the method.
     * @param <B> The type of the second parameter of the method.
     * @param <C> The type of the third parameter of the method.
     * @param <D> The type of the fourth parameter of the method.
     * @param <E> The type of the fifth parameter of the method.
     * @param <F> The type of the sixth parameter of the method.
     * @param <G> The type of the seventh parameter of the method.
     * @param <H> The type of the eighth parameter of the method.
     * @param <I> The type of the ninth parameter of the method.
     * @param <R> The type of the return value of the method.
     */
    @FunctionalInterface
    public static interface Method9<A,B,C,D,E,F,G,H,I,R> extends AbstractSlot{
        public R invoke(A a, B b, C c, D d, E e, F f, G g, H h, I i) throws Throwable;
    }

    /**
     * Supertype for parameterless signals.
     */
    public static abstract class AbstractPrivateSignal0 extends AbstractSignal {
        AbstractPrivateSignal0(){super();}
        
        AbstractPrivateSignal0(Class<?> declaringClass, boolean isDisposedSignal) {
            super(declaringClass, isDisposedSignal);
        }
        
        AbstractPrivateSignal0(Class<?>... types) {
            super(false, types);
        }
        
        AbstractPrivateSignal0(String signalName, Class<?>... types) {
            super(signalName, types);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
         */
        public final QMetaObject.Connection connect(Slot0 slot, Qt.ConnectionType... type) {
            return addConnectionToSlotObject(slot, type);
        }
    
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(Slot0 slot) {
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
        public final QMetaObject.Connection connect(Connectable0 signal, Qt.ConnectionType... type) {
            return addConnectionToSignalObject((AbstractSignal)signal, type);
        }
    
        /**
         * Removes the connection to the given <i>signal</i>.
         * 
         * @param signal the signal to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(Connectable0 signal) {
            return removeConnectionToSignalObject((AbstractSignal)signal);
        }
    }

    /**
     * Supertype for all signals with one parameter.
     * @param <A> The type of the first parameter of the signal.
     */
    public static abstract class AbstractPrivateSignal1<A> extends AbstractSignal {
        
        AbstractPrivateSignal1(){super();}
        
        AbstractPrivateSignal1(Class<?> declaringClass, boolean isDisposedSignal) {
            super(declaringClass, isDisposedSignal);
        }
        
        AbstractPrivateSignal1(Class<?>... types) {
            super(false, types);
        }
        
        AbstractPrivateSignal1(String signalName, Class<?>... types) {
            super(signalName, types);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
         */
        public final QMetaObject.Connection connect(Slot0 slot, Qt.ConnectionType... type) {
            return addConnectionToSlotObject(slot, type);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
         */
        public final QMetaObject.Connection connect(Slot1<? super A> slot, Qt.ConnectionType... type) {
            return addConnectionToSlotObject(slot, type);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(Slot0 slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(Slot1<? super A> slot) {
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
        public final QMetaObject.Connection connect(Connectable1<? super A> signal, Qt.ConnectionType... type) {
            return addConnectionToSignalObject((AbstractSignal)signal, type);
        }
        
        /**
         * Removes the connection to the given <i>signal</i>.
         * 
         * @param signal the signal to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(Connectable1<? super A> signal) {
            return removeConnectionToSignalObject((AbstractSignal)signal);
        }
    }

    /**
     * Supertype for all signals with two parameters.
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     */
    public static abstract class AbstractPrivateSignal2<A, B> extends AbstractSignal {
        
        AbstractPrivateSignal2(){super();}
    
        AbstractPrivateSignal2(Class<?> declaringClass, boolean isDisposedSignal) {
            super(declaringClass, isDisposedSignal);
        }
        
        AbstractPrivateSignal2(Class<?>... types) {
            super(false, types);
        }
        
        AbstractPrivateSignal2(String signalName, Class<?>... types) {
            super(signalName, types);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
         */
        public final QMetaObject.Connection connect(Slot0 slot, Qt.ConnectionType... type) {
            return addConnectionToSlotObject(slot, type);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
         */
        public final QMetaObject.Connection connect(Slot1<? super A> slot, Qt.ConnectionType... type) {
            return addConnectionToSlotObject(slot, type);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
         */
        public final QMetaObject.Connection connect(Slot2<? super A,? super B> slot, Qt.ConnectionType... type) {
            return addConnectionToSlotObject(slot, type);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(Slot0 slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(Slot1<? super A> slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(Slot2<? super A,? super B> slot) {
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
        public final QMetaObject.Connection connect(Connectable2<? super A,? super B> signal, Qt.ConnectionType... type) {
            return addConnectionToSignalObject((AbstractSignal)signal, type);
        }
        
        /**
         * Removes the connection to the given <i>signal</i>.
         * 
         * @param signal the signal to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(Connectable2<? super A,? super B> signal) {
            return removeConnectionToSignalObject((AbstractSignal)signal);
        }
    }

    /**
     * Supertype for all signals with three parameters.
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     */
    public static abstract class AbstractPrivateSignal3<A, B, C> extends AbstractSignal {
        

        AbstractPrivateSignal3(){super();}
        
        AbstractPrivateSignal3(Class<?> declaringClass, boolean isDisposedSignal) {
            super(declaringClass, isDisposedSignal);
        }
        
        AbstractPrivateSignal3(Class<?>... types) {
            super(false, types);
        }
        
        AbstractPrivateSignal3(String signalName, Class<?>... types) {
            super(signalName, types);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
         */
        public final QMetaObject.Connection connect(Slot0 slot, Qt.ConnectionType... type) {
            return addConnectionToSlotObject(slot, type);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
         */
        public final QMetaObject.Connection connect(Slot1<? super A> slot, Qt.ConnectionType... type) {
            return addConnectionToSlotObject(slot, type);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
         */
        public final QMetaObject.Connection connect(Slot2<? super A,? super B> slot, Qt.ConnectionType... type) {
            return addConnectionToSlotObject(slot, type);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
         */
        public final QMetaObject.Connection connect(Slot3<? super A,? super B,? super C> slot, Qt.ConnectionType... type) {
            return addConnectionToSlotObject(slot, type);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(Slot0 slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(Slot1<? super A> slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(Slot2<? super A,? super B> slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(Slot3<? super A,? super B,? super C> slot) {
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
        public final QMetaObject.Connection connect(Connectable3<? super A,? super B,? super C> signal, Qt.ConnectionType... type) {
            return addConnectionToSignalObject((AbstractSignal)signal, type);
        }
        
        /**
         * Removes the connection to the given <i>signal</i>.
         * 
         * @param signal the signal to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(Connectable3<? super A,? super B,? super C> signal) {
            return removeConnectionToSignalObject((AbstractSignal)signal);
        }
    }

    /**
     * Supertype for all signals with four parameters.
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     */
    public static abstract class AbstractPrivateSignal4<A, B, C, D> extends AbstractSignal {
        
        
        AbstractPrivateSignal4(){super();}
        
        AbstractPrivateSignal4(Class<?> declaringClass, boolean isDisposedSignal) {
            super(declaringClass, isDisposedSignal);
        }
        
        AbstractPrivateSignal4(Class<?>... types) {
            super(false, types);
        }
        
        AbstractPrivateSignal4(String signalName, Class<?>... types) {
            super(signalName, types);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
         */
        public final QMetaObject.Connection connect(Slot0 slot, Qt.ConnectionType... type) {
            return addConnectionToSlotObject(slot, type);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
         */
        public final QMetaObject.Connection connect(Slot1<? super A> slot, Qt.ConnectionType... type) {
            return addConnectionToSlotObject(slot, type);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
         */
        public final QMetaObject.Connection connect(Slot2<? super A,? super B> slot, Qt.ConnectionType... type) {
            return addConnectionToSlotObject(slot, type);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
         */
        public final QMetaObject.Connection connect(Slot3<? super A,? super B,? super C> slot, Qt.ConnectionType... type) {
            return addConnectionToSlotObject(slot, type);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
         */
        public final QMetaObject.Connection connect(Slot4<? super A,? super B,? super C,? super D> slot, Qt.ConnectionType... type) {
            return addConnectionToSlotObject(slot, type);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(Slot0 slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(Slot1<? super A> slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(Slot2<? super A,? super B> slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(Slot3<? super A,? super B,? super C> slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(Slot4<? super A,? super B,? super C,? super D> slot) {
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
        public final QMetaObject.Connection connect(Connectable4<? super A,? super B,? super C,? super D> signal, Qt.ConnectionType... type) {
            return addConnectionToSignalObject((AbstractSignal)signal, type);
        }
        
        /**
         * Removes the connection to the given <i>signal</i>.
         * 
         * @param signal the signal to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(Connectable4<? super A,? super B,? super C,? super D> signal) {
            return removeConnectionToSignalObject((AbstractSignal)signal);
        }
    }

    /**
     * Supertype for all signals with five parameters.
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     */
    public static abstract class AbstractPrivateSignal5<A, B, C, D, E> extends AbstractSignal {
        
        
        AbstractPrivateSignal5(){super();}
        
        AbstractPrivateSignal5(Class<?> declaringClass, boolean isDisposedSignal) {
            super(declaringClass, isDisposedSignal);
        }
        
        AbstractPrivateSignal5(Class<?>... types) {
            super(false, types);
        }
        
        AbstractPrivateSignal5(String signalName, Class<?>... types) {
            super(signalName, types);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
         */
        public final QMetaObject.Connection connect(Slot0 slot, Qt.ConnectionType... type) {
            return addConnectionToSlotObject(slot, type);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
         */
        public final QMetaObject.Connection connect(Slot1<? super A> slot, Qt.ConnectionType... type) {
            return addConnectionToSlotObject(slot, type);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
         */
        public final QMetaObject.Connection connect(Slot2<? super A,? super B> slot, Qt.ConnectionType... type) {
            return addConnectionToSlotObject(slot, type);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
         */
        public final QMetaObject.Connection connect(Slot3<? super A,? super B,? super C> slot, Qt.ConnectionType... type) {
            return addConnectionToSlotObject(slot, type);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
         */
        public final QMetaObject.Connection connect(Slot4<? super A,? super B,? super C,? super D> slot, Qt.ConnectionType... type) {
            return addConnectionToSlotObject(slot, type);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
         */
        public final QMetaObject.Connection connect(Slot5<? super A,? super B,? super C,? super D,? super E> slot, Qt.ConnectionType... type) {
            return addConnectionToSlotObject(slot, type);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(Slot0 slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(Slot1<? super A> slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(Slot2<? super A,? super B> slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(Slot3<? super A,? super B,? super C> slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(Slot4<? super A,? super B,? super C,? super D> slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(Slot5<? super A,? super B,? super C,? super D,? super E> slot) {
            return removeConnectionToSlotObject(slot);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         */
        public final QMetaObject.Connection connect(Connectable5<? super A,? super B,? super C,? super D,? super E> slot, Qt.ConnectionType... type) {
            return addConnectionToSignalObject((AbstractSignal)slot, type);
        }
        
        /**
         * Removes the connection to the given <i>signal</i>.
         * 
         * @param signal the signal to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(Connectable5<? super A,? super B,? super C,? super D,? super E> signal) {
            return removeConnectionToSignalObject((AbstractSignal)signal);
        }
    }

    /**
     * Supertype for all signals with six parameters.
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     */
    public static abstract class AbstractPrivateSignal6<A, B, C, D, E, F> extends AbstractSignal {
        
        
        AbstractPrivateSignal6(){super();}
        
        AbstractPrivateSignal6(Class<?> declaringClass, boolean isDisposedSignal) {
            super(declaringClass, isDisposedSignal);
        }
        
        AbstractPrivateSignal6(Class<?>... types) {
            super(false, types);
        }
        
        AbstractPrivateSignal6(String signalName, Class<?>... types) {
            super(signalName, types);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
         */
        public final QMetaObject.Connection connect(Slot0 slot, Qt.ConnectionType... type) {
            return addConnectionToSlotObject(slot, type);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
         */
        public final QMetaObject.Connection connect(Slot1<? super A> slot, Qt.ConnectionType... type) {
            return addConnectionToSlotObject(slot, type);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
         */
        public final QMetaObject.Connection connect(Slot2<? super A,? super B> slot, Qt.ConnectionType... type) {
            return addConnectionToSlotObject(slot, type);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
         */
        public final QMetaObject.Connection connect(Slot3<? super A,? super B,? super C> slot, Qt.ConnectionType... type) {
            return addConnectionToSlotObject(slot, type);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
         */
        public final QMetaObject.Connection connect(Slot4<? super A,? super B,? super C,? super D> slot, Qt.ConnectionType... type) {
            return addConnectionToSlotObject(slot, type);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
         */
        public final QMetaObject.Connection connect(Slot5<? super A,? super B,? super C,? super D,? super E> slot, Qt.ConnectionType... type) {
            return addConnectionToSlotObject(slot, type);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
         */
        public final QMetaObject.Connection connect(Slot6<? super A,? super B,? super C,? super D,? super E,? super F> slot, Qt.ConnectionType... type) {
            return addConnectionToSlotObject(slot, type);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(Slot0 slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(Slot1<? super A> slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(Slot2<? super A,? super B> slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(Slot3<? super A,? super B,? super C> slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(Slot4<? super A,? super B,? super C,? super D> slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(Slot5<? super A,? super B,? super C,? super D,? super E> slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(Slot6<? super A,? super B,? super C,? super D,? super E,? super F> slot) {
            return removeConnectionToSlotObject(slot);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         */
        public final QMetaObject.Connection connect(Connectable6<? super A,? super B,? super C,? super D,? super E,? super F> slot, Qt.ConnectionType... type) {
            return addConnectionToSignalObject((AbstractSignal)slot, type);
        }
        
        /**
         * Removes the connection to the given <i>signal</i>.
         * 
         * @param signal the signal to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(Connectable6<? super A,? super B,? super C,? super D,? super E,? super F> signal) {
            return removeConnectionToSignalObject((AbstractSignal)signal);
        }
    }

    /**
     * Supertype for all signals with seven parameters.
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     */
    public static abstract class AbstractPrivateSignal7<A, B, C, D, E, F, G> extends AbstractSignal {
        
        
        AbstractPrivateSignal7(){super();}
        
        AbstractPrivateSignal7(Class<?> declaringClass, boolean isDisposedSignal) {
            super(declaringClass, isDisposedSignal);
        }
        
        AbstractPrivateSignal7(Class<?>... types) {
            super(false, types);
        }
        
        AbstractPrivateSignal7(String signalName, Class<?>... types) {
            super(signalName, types);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
         */
        public final QMetaObject.Connection connect(Slot0 slot, Qt.ConnectionType... type) {
            return addConnectionToSlotObject(slot, type);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
         */
        public final QMetaObject.Connection connect(Slot1<? super A> slot, Qt.ConnectionType... type) {
            return addConnectionToSlotObject(slot, type);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
         */
        public final QMetaObject.Connection connect(Slot2<? super A,? super B> slot, Qt.ConnectionType... type) {
            return addConnectionToSlotObject(slot, type);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
         */
        public final QMetaObject.Connection connect(Slot3<? super A,? super B,? super C> slot, Qt.ConnectionType... type) {
            return addConnectionToSlotObject(slot, type);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
         */
        public final QMetaObject.Connection connect(Slot4<? super A,? super B,? super C,? super D> slot, Qt.ConnectionType... type) {
            return addConnectionToSlotObject(slot, type);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
         */
        public final QMetaObject.Connection connect(Slot5<? super A,? super B,? super C,? super D,? super E> slot, Qt.ConnectionType... type) {
            return addConnectionToSlotObject(slot, type);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
         */
        public final QMetaObject.Connection connect(Slot6<? super A,? super B,? super C,? super D,? super E,? super F> slot, Qt.ConnectionType... type) {
            return addConnectionToSlotObject(slot, type);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
         */
        public final QMetaObject.Connection connect(Slot7<? super A,? super B,? super C,? super D,? super E,? super F,? super G> slot, Qt.ConnectionType... type) {
            return addConnectionToSlotObject(slot, type);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(Slot0 slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(Slot1<? super A> slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(Slot2<? super A,? super B> slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(Slot3<? super A,? super B,? super C> slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(Slot4<? super A,? super B,? super C,? super D> slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(Slot5<? super A,? super B,? super C,? super D,? super E> slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(Slot6<? super A,? super B,? super C,? super D,? super E,? super F> slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(Slot7<? super A,? super B,? super C,? super D,? super E,? super F,? super G> slot) {
            return removeConnectionToSlotObject(slot);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         */
        public final QMetaObject.Connection connect(Connectable7<? super A,? super B,? super C,? super D,? super E,? super F,? super G> slot, Qt.ConnectionType... type) {
            return addConnectionToSignalObject((AbstractSignal)slot, type);
        }
        
        /**
         * Removes the connection to the given <i>signal</i>.
         * 
         * @param signal the signal to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(Connectable7<? super A,? super B,? super C,? super D,? super E,? super F,? super G> signal) {
            return removeConnectionToSignalObject((AbstractSignal)signal);
        }
    }

    /**
     * Supertype for all signals with eight parameters.
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     * @param <H> The type of the eighth parameter of the signal.
     */
    public static abstract class AbstractPrivateSignal8<A, B, C, D, E, F, G, H> extends AbstractSignal {
        
        AbstractPrivateSignal8(){super();}
        
        AbstractPrivateSignal8(Class<?> declaringClass, boolean isDisposedSignal) {
            super(declaringClass, isDisposedSignal);
        }
        
        AbstractPrivateSignal8(Class<?>... types) {
            super(false, types);
        }
        
        AbstractPrivateSignal8(String signalName, Class<?>... types) {
            super(signalName, types);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
         */
        public final QMetaObject.Connection connect(Slot0 slot, Qt.ConnectionType... type) {
            return addConnectionToSlotObject(slot, type);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
         */
        public final QMetaObject.Connection connect(Slot1<? super A> slot, Qt.ConnectionType... type) {
            return addConnectionToSlotObject(slot, type);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
         */
        public final QMetaObject.Connection connect(Slot2<? super A,? super B> slot, Qt.ConnectionType... type) {
            return addConnectionToSlotObject(slot, type);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
         */
        public final QMetaObject.Connection connect(Slot3<? super A,? super B,? super C> slot, Qt.ConnectionType... type) {
            return addConnectionToSlotObject(slot, type);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
         */
        public final QMetaObject.Connection connect(Slot4<? super A,? super B,? super C,? super D> slot, Qt.ConnectionType... type) {
            return addConnectionToSlotObject(slot, type);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
         */
        public final QMetaObject.Connection connect(Slot5<? super A,? super B,? super C,? super D,? super E> slot, Qt.ConnectionType... type) {
            return addConnectionToSlotObject(slot, type);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
         */
        public final QMetaObject.Connection connect(Slot6<? super A,? super B,? super C,? super D,? super E,? super F> slot, Qt.ConnectionType... type) {
            return addConnectionToSlotObject(slot, type);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
         */
        public final QMetaObject.Connection connect(Slot7<? super A,? super B,? super C,? super D,? super E,? super F,? super G> slot, Qt.ConnectionType... type) {
            return addConnectionToSlotObject(slot, type);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
         */
        public final QMetaObject.Connection connect(Slot8<? super A,? super B,? super C,? super D,? super E,? super F,? super G,? super H> slot, Qt.ConnectionType... type) {
            return addConnectionToSlotObject(slot, type);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(Slot0 slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(Slot1<? super A> slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(Slot2<? super A,? super B> slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(Slot3<? super A,? super B,? super C> slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(Slot4<? super A,? super B,? super C,? super D> slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(Slot5<? super A,? super B,? super C,? super D,? super E> slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(Slot6<? super A,? super B,? super C,? super D,? super E,? super F> slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(Slot7<? super A,? super B,? super C,? super D,? super E,? super F,? super G> slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(Slot8<? super A,? super B,? super C,? super D,? super E,? super F,? super G,? super H> slot) {
            return removeConnectionToSlotObject(slot);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         */
        public final QMetaObject.Connection connect(Connectable8<? super A,? super B,? super C,? super D,? super E,? super F,? super G,? super H> slot, Qt.ConnectionType... type) {
            return addConnectionToSignalObject((AbstractSignal)slot, type);
        }
        
        /**
         * Removes the connection to the given <i>signal</i>.
         * 
         * @param signal the signal to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(Connectable8<? super A,? super B,? super C,? super D,? super E,? super F,? super G,? super H> signal) {
            return removeConnectionToSignalObject((AbstractSignal)signal);
        }
    }

    /**
     * Supertype for all signals with nine parameters.
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
    public static abstract class AbstractPrivateSignal9<A, B, C, D, E, F, G, H, I> extends AbstractSignal {
        
        AbstractPrivateSignal9(){super();}
        
        AbstractPrivateSignal9(Class<?> declaringClass, boolean isDisposedSignal) {
            super(declaringClass, isDisposedSignal);
        }
        
        AbstractPrivateSignal9(Class<?>... types) {
            super(false, types);
        }
        
        AbstractPrivateSignal9(String signalName, Class<?>... types) {
            super(signalName, types);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
         */
        public final QMetaObject.Connection connect(Slot0 slot, Qt.ConnectionType... type) {
            return addConnectionToSlotObject(slot, type);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
         */
        public final QMetaObject.Connection connect(Slot1<? super A> slot, Qt.ConnectionType... type) {
            return addConnectionToSlotObject(slot, type);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
         */
        public final QMetaObject.Connection connect(Slot2<? super A,? super B> slot, Qt.ConnectionType... type) {
            return addConnectionToSlotObject(slot, type);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
         */
        public final QMetaObject.Connection connect(Slot3<? super A,? super B,? super C> slot, Qt.ConnectionType... type) {
            return addConnectionToSlotObject(slot, type);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
         */
        public final QMetaObject.Connection connect(Slot4<? super A,? super B,? super C,? super D> slot, Qt.ConnectionType... type) {
            return addConnectionToSlotObject(slot, type);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
         */
        public final QMetaObject.Connection connect(Slot5<? super A,? super B,? super C,? super D,? super E> slot, Qt.ConnectionType... type) {
            return addConnectionToSlotObject(slot, type);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
         */
        public final QMetaObject.Connection connect(Slot6<? super A,? super B,? super C,? super D,? super E,? super F> slot, Qt.ConnectionType... type) {
            return addConnectionToSlotObject(slot, type);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
         */
        public final QMetaObject.Connection connect(Slot7<? super A,? super B,? super C,? super D,? super E,? super F,? super G> slot, Qt.ConnectionType... type) {
            return addConnectionToSlotObject(slot, type);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
         */
        public final QMetaObject.Connection connect(Slot8<? super A,? super B,? super C,? super D,? super E,? super F,? super G,? super H> slot, Qt.ConnectionType... type) {
            return addConnectionToSlotObject(slot, type);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         * @throws io.qt.QUninvokableSlotException Raised if slot is annotated <code>&commat;QtUninvokable</code>.
         */
        public final QMetaObject.Connection connect(Slot9<? super A,? super B,? super C,? super D,? super E,? super F,? super G,? super H,? super I> slot, Qt.ConnectionType... type) {
            return addConnectionToSlotObject(slot, type);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(Slot0 slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(Slot1<? super A> slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(Slot2<? super A,? super B> slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(Slot3<? super A,? super B,? super C> slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(Slot4<? super A,? super B,? super C,? super D> slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(Slot5<? super A,? super B,? super C,? super D,? super E> slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(Slot6<? super A,? super B,? super C,? super D,? super E,? super F> slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(Slot7<? super A,? super B,? super C,? super D,? super E,? super F,? super G> slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(Slot8<? super A,? super B,? super C,? super D,? super E,? super F,? super G,? super H> slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(Slot9<? super A,? super B,? super C,? super D,? super E,? super F,? super G,? super H,? super I> slot) {
            return removeConnectionToSlotObject(slot);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         */
        public final QMetaObject.Connection connect(Connectable9<? super A,? super B,? super C,? super D,? super E,? super F,? super G,? super H,? super I> slot, Qt.ConnectionType... type) {
            return addConnectionToSignalObject((AbstractSignal)slot, type);
        }
        
        /**
         * Removes the connection to the given <i>signal</i>.
         * 
         * @param signal the signal to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(Connectable9<? super A,? super B,? super C,? super D,? super E,? super F,? super G,? super H,? super I> signal) {
            return removeConnectionToSignalObject((AbstractSignal)signal);
        }
    }

    /**
     * Supertype for all public parameterless signals.
     */
    public static abstract class AbstractPublicSignal0 extends AbstractPrivateSignal0 implements    Emitable0,
																								    Connectable0,
																								    Connectable1<Object>,
																								    Connectable2<Object,Object>,
																								    Connectable3<Object,Object,Object>,
																								    Connectable4<Object,Object,Object,Object>,
																								    Connectable5<Object,Object,Object,Object,Object>,
																									Connectable6<Object,Object,Object,Object,Object,Object>,
																									Connectable7<Object,Object,Object,Object,Object,Object,Object>,
																									Connectable8<Object,Object,Object,Object,Object,Object,Object,Object>,
																									Connectable9<Object,Object,Object,Object,Object,Object,Object,Object,Object>,
																									GenericConnectable
    {
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
        @Override
        public final void emit() {
            emitSignal();
        }
    }

    /**
     * Supertype for all public signals with one parameter.
     * @param <A> The type of the first parameter of the signal.
     */
    public static abstract class AbstractPublicSignal1<A> extends AbstractPrivateSignal1<A> implements  Emitable1<A>,
																									    Connectable1<A>,
																									    Connectable2<A,Object>,
																									    Connectable3<A,Object,Object>,
																									    Connectable4<A,Object,Object,Object>,
																									    Connectable5<A,Object,Object,Object,Object>,
																										Connectable6<A,Object,Object,Object,Object,Object>,
																										Connectable7<A,Object,Object,Object,Object,Object,Object>,
																										Connectable8<A,Object,Object,Object,Object,Object,Object,Object>,
																										Connectable9<A,Object,Object,Object,Object,Object,Object,Object,Object>,
																										GenericConnectable
    {
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
         * @param arg1
         */
        @Override
        public final void emit(A arg1) {
            emitSignal(arg1);
        }
    }

    /**
     * Supertype for all public signals with two parameters.
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     */
    public static abstract class AbstractPublicSignal2<A,B> extends AbstractPrivateSignal2<A,B> implements  Emitable2<A,B>,
																										    Connectable2<A,B>,
																										    Connectable3<A,B,Object>,
																										    Connectable4<A,B,Object,Object>,
																										    Connectable5<A,B,Object,Object,Object>,
																											Connectable6<A,B,Object,Object,Object,Object>,
																											Connectable7<A,B,Object,Object,Object,Object,Object>,
																											Connectable8<A,B,Object,Object,Object,Object,Object,Object>,
																											Connectable9<A,B,Object,Object,Object,Object,Object,Object,Object>,
																											GenericConnectable
    {
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
         * @param arg1
         * @param arg2
         */
        @Override
        public final void emit(A arg1, B arg2) {
            emitSignal(arg1, arg2);
        }
    }

    /**
     * Supertype for all public signals with three parameters.
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     */
    public static abstract class AbstractPublicSignal3<A,B,C> extends AbstractPrivateSignal3<A,B,C> implements  Emitable3<A,B,C>,
																											    Connectable3<A,B,C>,
																											    Connectable4<A,B,C,Object>,
																											    Connectable5<A,B,C,Object,Object>,
																												Connectable6<A,B,C,Object,Object,Object>,
																												Connectable7<A,B,C,Object,Object,Object,Object>,
																												Connectable8<A,B,C,Object,Object,Object,Object,Object>,
																												Connectable9<A,B,C,Object,Object,Object,Object,Object,Object>,
																												GenericConnectable
    {
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
         * @param arg1
         * @param arg2
         * @param arg3
         */
        @Override
        public final void emit(A arg1, B arg2, C arg3) {
            emitSignal(arg1, arg2, arg3);
        }
    }

    /**
     * Supertype for all public signals with four parameters.
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     */
    public static abstract class AbstractPublicSignal4<A,B,C,D> extends AbstractPrivateSignal4<A,B,C,D> implements  Emitable4<A,B,C,D>,
																												    Connectable4<A,B,C,D>,
																												    Connectable5<A,B,C,D,Object>,
																													Connectable6<A,B,C,D,Object,Object>,
																													Connectable7<A,B,C,D,Object,Object,Object>,
																													Connectable8<A,B,C,D,Object,Object,Object,Object>,
																													Connectable9<A,B,C,D,Object,Object,Object,Object,Object>,
																													GenericConnectable
    {
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
         * @param arg1
         * @param arg2
         * @param arg3
         * @param arg4
         */
        @Override
        public final void emit(A arg1, B arg2, C arg3, D arg4) {
            emitSignal(arg1, arg2, arg3, arg4);
        }
    }

    /**
     * Supertype for all public signals with five parameters.
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     */
    public static abstract class AbstractPublicSignal5<A,B,C,D,E> extends AbstractPrivateSignal5<A,B,C,D,E> implements  Emitable5<A,B,C,D,E>,
																													    Connectable5<A,B,C,D,E>,
																														Connectable6<A,B,C,D,E,Object>,
																														Connectable7<A,B,C,D,E,Object,Object>,
																														Connectable8<A,B,C,D,E,Object,Object,Object>,
																														Connectable9<A,B,C,D,E,Object,Object,Object,Object>,
																														GenericConnectable
    {
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
         * @param arg1
         * @param arg2
         * @param arg3
         * @param arg4
         * @param arg5
         */
        @Override
        public final void emit(A arg1, B arg2, C arg3, D arg4, E arg5) {
            emitSignal(arg1, arg2, arg3, arg4, arg5);
        }
    }

    /**
     * Supertype for all public signals with six parameters.
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     */
    public static abstract class AbstractPublicSignal6<A,B,C,D,E,F> extends AbstractPrivateSignal6<A,B,C,D,E,F> implements  Emitable6<A,B,C,D,E,F>,
    																														Connectable6<A,B,C,D,E,F>,
																															Connectable7<A,B,C,D,E,F,Object>,
																															Connectable8<A,B,C,D,E,F,Object,Object>,
																															Connectable9<A,B,C,D,E,F,Object,Object,Object>,
																															GenericConnectable
	{
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
         * @param arg1
         * @param arg2
         * @param arg3
         * @param arg4
         * @param arg5
         * @param arg6
         */
        @Override
        public final void emit(A arg1, B arg2, C arg3, D arg4, E arg5, F arg6) {
            emitSignal(arg1, arg2, arg3, arg4, arg5, arg6);
        }
    }

    /**
     * Supertype for all public signals with seven parameters.
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     */
    public static abstract class AbstractPublicSignal7<A,B,C,D,E,F,G> extends AbstractPrivateSignal7<A,B,C,D,E,F,G> implements  Emitable7<A,B,C,D,E,F,G>,
																																Connectable7<A,B,C,D,E,F,G>,
																																Connectable8<A,B,C,D,E,F,G,Object>,
																																Connectable9<A,B,C,D,E,F,G,Object,Object>,
																																GenericConnectable
    {
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
         * @param arg1
         * @param arg2
         * @param arg3
         * @param arg4
         * @param arg5
         * @param arg6
         * @param arg7
         */
        @Override
        public final void emit(A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7) {
            emitSignal(arg1, arg2, arg3, arg4, arg5, arg6, arg7);
        }
    }

    /**
     * Supertype for all public signals with eight parameters.
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     * @param <H> The type of the eighth parameter of the signal.
     */
    public static abstract class AbstractPublicSignal8<A,B,C,D,E,F,G,H> extends AbstractPrivateSignal8<A,B,C,D,E,F,G,H> implements  Emitable8<A,B,C,D,E,F,G,H>,
    																																Connectable8<A,B,C,D,E,F,G,H>,
    																																Connectable9<A,B,C,D,E,F,G,H,Object>,
    																																GenericConnectable
	{
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
         * @param arg1
         * @param arg2
         * @param arg3
         * @param arg4
         * @param arg5
         * @param arg6
         * @param arg7
         * @param arg8
         */
        @Override
        public final void emit(A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7, H arg8) {
            emitSignal(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
        }
    }

    /**
     * Supertype for all public signals with nine parameters.
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
    public static abstract class AbstractPublicSignal9<A,B,C,D,E,F,G,H,I> extends AbstractPrivateSignal9<A,B,C,D,E,F,G,H,I> implements  Emitable9<A,B,C,D,E,F,G,H,I>, 
    																																   Connectable9<A,B,C,D,E,F,G,H,I>,
    																																   GenericConnectable 
    {
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
         * @param arg1
         * @param arg2
         * @param arg3
         * @param arg4
         * @param arg5
         * @param arg6
         * @param arg7
         * @param arg8
         * @param arg9
         */
        @Override
        public final void emit(A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7, H arg8, I arg9) {
            emitSignal(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
        }
    }

    /**
     * Supertype for all public default signals with two parameters.
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     */
    public static abstract class AbstractSignal2Default1<A, B> extends AbstractPublicSignal2<A, B> implements  Emitable1<A>, Connectable1<A> {
    	AbstractSignal2Default1() {
    		this.arg2Default = null;
    	}
    	
    	AbstractSignal2Default1(Supplier<? extends B> arg2Default, Class<?> declaringClass, boolean isDisposedSignal){
            super(declaringClass, isDisposedSignal);
            if(arg2Default!=null){
                this.arg2Default = arg2Default;
            }else{
                throw new QNoDefaultValueException(2);
            }
        }
        
        AbstractSignal2Default1(Supplier<? extends B> arg2Default){
            super();
            if(arg2Default!=null){
                this.arg2Default = arg2Default;
            }else{
                throw new QNoDefaultValueException(2);
            }
        }
        
        AbstractSignal2Default1(Supplier<? extends B> arg2Default, Class<?>... types) {
            super(types);
            if(arg2Default!=null){
                this.arg2Default = arg2Default;
            }else{
                throw new QNoDefaultValueException(2);
            }
        }
        
        AbstractSignal2Default1(String signalName, Supplier<? extends B> arg2Default){
            super(signalName);
            if(arg2Default!=null){
                this.arg2Default = arg2Default;
            }else{
                throw new QNoDefaultValueException(2);
            }
        }
        
        AbstractSignal2Default1(String signalName, Supplier<? extends B> arg2Default, Class<?>... types) {
            super(signalName, types);
            if(arg2Default!=null){
                this.arg2Default = arg2Default;
            }else{
                throw new QNoDefaultValueException(2);
            }
        }
        
        private final Supplier<? extends B> arg2Default;
        
        /**
         * Emits the signal with default value for arg2.
         * @param arg1
         */
        @Override
        public final void emit(A arg1) {
        	super.emitDefaultSignal(arg2Default, arg1);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends A> arg1Default) {
        	super.emitDefaultSignal(arg1Default, arg2Default);
        }
    }

    /**
     * Supertype for all public default signals with three parameters.
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     */
    public static abstract class AbstractSignal3Default1<A, B, C> extends AbstractPublicSignal3<A, B, C> implements  Emitable2<A,B>, Connectable2<A,B> {
    	AbstractSignal3Default1() {
    		this.arg3Default = null;
    	}
    	
    	AbstractSignal3Default1(Supplier<? extends C> arg3Default, Class<?> declaringClass, boolean isDisposedSignal){
            super(declaringClass, isDisposedSignal);
            if(arg3Default!=null){
                this.arg3Default = arg3Default;
            }else{
                throw new QNoDefaultValueException(3);
            }
        }
        
        AbstractSignal3Default1(Supplier<? extends C> arg3Default){
            super();
            if(arg3Default!=null){
                this.arg3Default = arg3Default;
            }else{
                throw new QNoDefaultValueException(3);
            }
        }
        
        AbstractSignal3Default1(Supplier<? extends C> arg3Default, Class<?>... types) {
            super(types);
            if(arg3Default!=null){
                this.arg3Default = arg3Default;
            }else{
                throw new QNoDefaultValueException(3);
            }
        }
        
        AbstractSignal3Default1(String signalName, Supplier<? extends C> arg3Default, Class<?>... types) {
            super(signalName, types);
            if(arg3Default!=null){
                this.arg3Default = arg3Default;
            }else{
                throw new QNoDefaultValueException(3);
            }
        }
        
        private final Supplier<? extends C> arg3Default;
        
        /**
         * Emits the signal with default value for arg3.
         * @param arg1
         * @param arg2
         */
        @Override
        public final void emit(A arg1, B arg2) {
        	emitDefaultSignal(arg3Default, arg1, arg2);
        }

        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends B> arg2Default, A arg1) {
        	super.emitDefaultSignal(arg2Default, arg3Default, arg1);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends A> arg1Default,
        		Supplier<? extends B> arg2Default) {
        	super.emitDefaultSignal(arg1Default, arg2Default, arg3Default);
        }
    }

    /**
     * Supertype for all public default signals with three parameters.
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     */
    public static abstract class AbstractSignal3Default2<A, B, C> extends AbstractSignal3Default1<A, B, C> implements  Emitable1<A>, Connectable1<A> {
    	AbstractSignal3Default2() {
    		this.arg2Default = null;
    	}
    	
    	AbstractSignal3Default2(Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Class<?> declaringClass, boolean isDisposedSignal){
            super(arg3Default, declaringClass, isDisposedSignal);
            if(arg2Default!=null){
                this.arg2Default = arg2Default;
            }else{
                throw new QNoDefaultValueException(2);
            }
        }
        
        AbstractSignal3Default2(Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default){
            super(arg3Default);
            if(arg2Default!=null){
                this.arg2Default = arg2Default;
            }else{
                throw new QNoDefaultValueException(2);
            }
        }
        
        AbstractSignal3Default2(Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Class<?>... types) {
            super(arg3Default, types);
            if(arg2Default!=null){
                this.arg2Default = arg2Default;
            }else{
                throw new QNoDefaultValueException(2);
            }
        }
        
        AbstractSignal3Default2(String signalName, Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Class<?>... types) {
            super(signalName, arg3Default, types);
            if(arg2Default!=null){
                this.arg2Default = arg2Default;
            }else{
                throw new QNoDefaultValueException(2);
            }
        }
        
        private final Supplier<? extends B> arg2Default;
        
        /**
         * Emits the signal with default value for arg2.
         * @param arg1
         */
        @Override
        public final void emit(A arg1) {
            super.emitDefaultSignal(arg2Default, arg1);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends A> arg1Default) {
        	super.emitDefaultSignal(arg1Default, arg2Default);
        }
    }

    /**
     * Supertype for all public default signals with four parameters.
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     */
    public static abstract class AbstractSignal4Default1<A, B, C, D> extends AbstractPublicSignal4<A, B, C, D> implements  Emitable3<A,B,C>, Connectable3<A,B,C> {
    	AbstractSignal4Default1() {
    		this.arg4Default = null;
    	}
    	
    	AbstractSignal4Default1(Supplier<? extends D> arg4Default, Class<?> declaringClass, boolean isDisposedSignal){
            super(declaringClass, isDisposedSignal);
            if(arg4Default!=null){
                this.arg4Default = arg4Default;
            }else{
                throw new QNoDefaultValueException(4);
            }
        }
        
        AbstractSignal4Default1(Supplier<? extends D> arg4Default){
            super();
            if(arg4Default!=null){
                this.arg4Default = arg4Default;
            }else{
                throw new QNoDefaultValueException(4);
            }
        }
        
        AbstractSignal4Default1(Supplier<? extends D> arg4Default, Class<?>... types) {
            super(types);
            if(arg4Default!=null){
                this.arg4Default = arg4Default;
            }else{
                throw new QNoDefaultValueException(4);
            }
        }
        
        AbstractSignal4Default1(String signalName, Supplier<? extends D> arg4Default, Class<?>... types) {
            super(signalName, types);
            if(arg4Default!=null){
                this.arg4Default = arg4Default;
            }else{
                throw new QNoDefaultValueException(4);
            }
        }
        
        private final Supplier<? extends D> arg4Default;
        
        /**
         * Emits the signal with default value for arg4.
         * @param arg1
         * @param arg2
         * @param arg3
         */
        @Override
        public void emit(A arg1, B arg2, C arg3) {
        	super.emitDefaultSignal(arg4Default, arg1, arg2, arg3);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends C> arg3Default, A arg1, B arg2) {
        	super.emitDefaultSignal(arg3Default, arg4Default, arg1, arg2);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, A arg1) {
        	super.emitDefaultSignal(arg2Default, arg3Default, arg4Default, arg1);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends A> arg1Default,
        		Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default) {
        	super.emitDefaultSignal(arg1Default, arg2Default, arg3Default, arg4Default);
        }
    }

    /**
     * Supertype for all public default signals with four parameters.
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     */
    public static abstract class AbstractSignal4Default2<A, B, C, D> extends AbstractSignal4Default1<A, B, C, D> implements  Emitable2<A,B>, Connectable2<A,B> {
    	AbstractSignal4Default2() {
    		this.arg3Default = null;
    	}
    	
    	AbstractSignal4Default2(Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Class<?> declaringClass, boolean isDisposedSignal){
            super(arg4Default, declaringClass, isDisposedSignal);
            if(arg3Default!=null){
                this.arg3Default = arg3Default;
            }else{
                throw new QNoDefaultValueException(3);
            }
        }
        
        AbstractSignal4Default2(Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default){
            super(arg4Default);
            if(arg3Default!=null){
                this.arg3Default = arg3Default;
            }else{
                throw new QNoDefaultValueException(3);
            }
        }
        
        AbstractSignal4Default2(Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Class<?>... types) {
            super(arg4Default, types);
            if(arg3Default!=null){
                this.arg3Default = arg3Default;
            }else{
                throw new QNoDefaultValueException(3);
            }
        }
        
        AbstractSignal4Default2(String signalName, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Class<?>... types) {
            super(signalName, arg4Default, types);
            if(arg3Default!=null){
                this.arg3Default = arg3Default;
            }else{
                throw new QNoDefaultValueException(3);
            }
        }
        
        private final Supplier<? extends C> arg3Default;
        
        /**
         * Emits the signal with default value for arg3.
         * @param arg1
         * @param arg2
         */
        @Override
        public final void emit(A arg1, B arg2) {
        	super.emitDefaultSignal(arg3Default, arg1, arg2);
        }

        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends B> arg2Default, A arg1) {
        	super.emitDefaultSignal(arg2Default, arg3Default, arg1);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends A> arg1Default,
        		Supplier<? extends B> arg2Default) {
        	super.emitDefaultSignal(arg1Default, arg2Default, arg3Default);
        }
    }

    /**
     * Supertype for all public default signals with four parameters.
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     */
    public static abstract class AbstractSignal4Default3<A, B, C, D> extends AbstractSignal4Default2<A, B, C, D> implements  Emitable1<A>, Connectable1<A> {
    	AbstractSignal4Default3() {
    		this.arg2Default = null;
    	}
    	
		AbstractSignal4Default3(Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Class<?> declaringClass, boolean isDisposedSignal){
            super(arg3Default, arg4Default, declaringClass, isDisposedSignal);
            if(arg2Default!=null){
                this.arg2Default = arg2Default;
            }else{
                throw new QNoDefaultValueException(2);
            }
        }
        
        AbstractSignal4Default3(Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default){
            super(arg3Default, arg4Default);
            if(arg2Default!=null){
                this.arg2Default = arg2Default;
            }else{
                throw new QNoDefaultValueException(2);
            }
        }
        
        AbstractSignal4Default3(Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Class<?>... types) {
            super(arg3Default, arg4Default, types);
            if(arg3Default!=null){
                this.arg2Default = arg2Default;
            }else{
                throw new QNoDefaultValueException(2);
            }
        }
        
        AbstractSignal4Default3(String signalName, Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Class<?>... types) {
            super(signalName, arg3Default, arg4Default, types);
            if(arg3Default!=null){
                this.arg2Default = arg2Default;
            }else{
                throw new QNoDefaultValueException(2);
            }
        }
        
        private final Supplier<? extends B> arg2Default;
        
        /**
         * Emits the signal with default value for arg2.
         * @param arg1
         */
        @Override
        public final void emit(A arg1) {
        	super.emitDefaultSignal(arg2Default, arg1);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends A> arg1Default) {
        	super.emitDefaultSignal(arg1Default, arg2Default);
        }
    }

    /**
     * Supertype for all public default signals with five parameters.
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     */
    public static abstract class AbstractSignal5Default1<A, B, C, D, E> extends AbstractPublicSignal5<A, B, C, D, E> implements  Emitable4<A,B,C,D>, Connectable4<A,B,C,D> {
    	AbstractSignal5Default1() {
    		this.arg5Default = null;
    	}
    	
		AbstractSignal5Default1(Supplier<? extends E> arg5Default, Class<?> declaringClass, boolean isDisposedSignal){
            super(declaringClass, isDisposedSignal);
            if(arg5Default!=null){
                this.arg5Default = arg5Default;
            }else{
                throw new QNoDefaultValueException(5);
            }
        }
        
        AbstractSignal5Default1(Supplier<? extends E> arg5Default){
            super();
            if(arg5Default!=null){
                this.arg5Default = arg5Default;
            }else{
                throw new QNoDefaultValueException(5);
            }
        }
        
        AbstractSignal5Default1(Supplier<? extends E> arg5Default, Class<?>... types) {
            super(types);
            if(arg5Default!=null){
                this.arg5Default = arg5Default;
            }else{
                throw new QNoDefaultValueException(5);
            }
        }
        
        AbstractSignal5Default1(String signalName, Supplier<? extends E> arg5Default, Class<?>... types) {
            super(signalName, types);
            if(arg5Default!=null){
                this.arg5Default = arg5Default;
            }else{
                throw new QNoDefaultValueException(5);
            }
        }
        
        private final Supplier<? extends E> arg5Default;
        
        /**
         * Emits the signal with default value for arg5.
         * @param arg1
         * @param arg2
         * @param arg3
         * @param arg4
         */
        @Override
        public final void emit(A arg1, B arg2, C arg3, D arg4) {
        	super.emitDefaultSignal(arg5Default, arg1, arg2, arg3, arg4);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends D> arg4Default, A arg1, B arg2, C arg3) {
        	super.emitDefaultSignal(arg4Default, arg5Default, arg1, arg2, arg3);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, A arg1, B arg2) {
        	super.emitDefaultSignal(arg3Default, arg4Default, arg5Default, arg1, arg2);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, A arg1) {
        	super.emitDefaultSignal(arg2Default, arg3Default, arg4Default, arg5Default, arg1);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends A> arg1Default,
        		Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default) {
        	super.emitDefaultSignal(arg1Default, arg2Default, arg3Default, arg4Default, arg5Default);
        }
    }

    /**
     * Supertype for all public default signals with five parameters.
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     */
    public static abstract class AbstractSignal5Default2<A, B, C, D, E> extends AbstractSignal5Default1<A, B, C, D, E> implements  Emitable3<A,B,C>, Connectable3<A,B,C> {
    	AbstractSignal5Default2() {
    		this.arg4Default = null;
    	}
    	
		AbstractSignal5Default2(Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Class<?> declaringClass, boolean isDisposedSignal){
            super(arg5Default, declaringClass, isDisposedSignal);
            if(arg4Default!=null){
                this.arg4Default = arg4Default;
            }else{
                throw new QNoDefaultValueException(4);
            }
        }
        
        AbstractSignal5Default2(Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default){
            super(arg5Default);
            if(arg4Default!=null){
                this.arg4Default = arg4Default;
            }else{
                throw new QNoDefaultValueException(4);
            }
        }
        
        AbstractSignal5Default2(Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Class<?>... types) {
            super(arg5Default, types);
            if(arg4Default!=null){
                this.arg4Default = arg4Default;
            }else{
                throw new QNoDefaultValueException(4);
            }
        }
        
        AbstractSignal5Default2(String signalName, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Class<?>... types) {
            super(signalName, arg5Default, types);
            if(arg4Default!=null){
                this.arg4Default = arg4Default;
            }else{
                throw new QNoDefaultValueException(4);
            }
        }
        
        private final Supplier<? extends D> arg4Default;
        
        /**
         * Emits the signal with default value for arg4.
         * @param arg1
         * @param arg2
         * @param arg3
         */
        @Override
        public final void emit(A arg1, B arg2, C arg3) {
        	super.emitDefaultSignal(arg4Default, arg1, arg2, arg3);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends C> arg3Default, A arg1, B arg2) {
        	super.emitDefaultSignal(arg3Default, arg4Default, arg1, arg2);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, A arg1) {
        	super.emitDefaultSignal(arg2Default, arg3Default, arg4Default, arg1);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends A> arg1Default,
        		Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default) {
        	super.emitDefaultSignal(arg1Default, arg2Default, arg3Default, arg4Default);
        }
    }

    /**
     * Supertype for all public default signals with five parameters.
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     */
    public static abstract class AbstractSignal5Default3<A, B, C, D, E> extends AbstractSignal5Default2<A, B, C, D, E> implements  Emitable2<A,B>, Connectable2<A,B> {
    	AbstractSignal5Default3() {
    		this.arg3Default = null;
    	}
    	
		AbstractSignal5Default3(Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Class<?> declaringClass, boolean isDisposedSignal){
            super(arg4Default, arg5Default, declaringClass, isDisposedSignal);
            if(arg3Default!=null){
                this.arg3Default = arg3Default;
            }else{
                throw new QNoDefaultValueException(3);
            }
        }
        
        AbstractSignal5Default3(Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default){
            super(arg4Default, arg5Default);
            if(arg3Default!=null){
                this.arg3Default = arg3Default;
            }else{
                throw new QNoDefaultValueException(3);
            }
        }
        
        AbstractSignal5Default3(Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Class<?>... types) {
            super(arg4Default, arg5Default, types);
            if(arg3Default!=null){
                this.arg3Default = arg3Default;
            }else{
                throw new QNoDefaultValueException(3);
            }
        }
        
        AbstractSignal5Default3(String signalName, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Class<?>... types) {
            super(signalName, arg4Default, arg5Default, types);
            if(arg3Default!=null){
                this.arg3Default = arg3Default;
            }else{
                throw new QNoDefaultValueException(3);
            }
        }
        
        private final Supplier<? extends C> arg3Default;
        
        /**
         * Emits the signal with default value for arg3.
         * @param arg1
         * @param arg2
         */
        @Override
        public final void emit(A arg1, B arg2) {
        	super.emitDefaultSignal(arg3Default, arg1, arg2);
        }

        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends B> arg2Default, A arg1) {
        	super.emitDefaultSignal(arg2Default, arg3Default, arg1);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends A> arg1Default,
        		Supplier<? extends B> arg2Default) {
        	super.emitDefaultSignal(arg1Default, arg2Default, arg3Default);
        }
    }

    /**
     * Supertype for all public default signals with five parameters.
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     */
    public static abstract class AbstractSignal5Default4<A, B, C, D, E> extends AbstractSignal5Default3<A, B, C, D, E> implements  Emitable1<A>, Connectable1<A> {
    	AbstractSignal5Default4() {
    		this.arg2Default = null;
    	}
    	
		AbstractSignal5Default4(Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Class<?> declaringClass, boolean isDisposedSignal){
            super(arg3Default, arg4Default, arg5Default, declaringClass, isDisposedSignal);
            if(arg2Default!=null){
                this.arg2Default = arg2Default;
            }else{
                throw new QNoDefaultValueException(2);
            }
        }
        
        AbstractSignal5Default4(Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default){
            super(arg3Default, arg4Default, arg5Default);
            if(arg2Default!=null){
                this.arg2Default = arg2Default;
            }else{
                throw new QNoDefaultValueException(2);
            }
        }
        
        AbstractSignal5Default4(Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Class<?>... types) {
            super(arg3Default, arg4Default, arg5Default, types);
            if(arg2Default!=null){
                this.arg2Default = arg2Default;
            }else{
                throw new QNoDefaultValueException(2);
            }
        }
        
        AbstractSignal5Default4(String signalName, Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Class<?>... types) {
            super(signalName, arg3Default, arg4Default, arg5Default, types);
            if(arg2Default!=null){
                this.arg2Default = arg2Default;
            }else{
                throw new QNoDefaultValueException(2);
            }
        }
        
        private final Supplier<? extends B> arg2Default;
        
        /**
         * Emits the signal with default value for arg2.
         * @param arg1
         */
        @Override
        public final void emit(A arg1) {
        	super.emitDefaultSignal(arg2Default, arg1);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends A> arg1Default) {
        	super.emitDefaultSignal(arg1Default, arg2Default);
        }
    }

    /**
     * Supertype for all public default signals with six parameters.
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     */
    public static abstract class AbstractSignal6Default1<A, B, C, D, E, F> extends AbstractPublicSignal6<A, B, C, D, E, F> implements  Emitable5<A,B,C,D,E>, Connectable5<A,B,C,D,E> {
    	AbstractSignal6Default1() {
    		this.arg6Default = null;
    	}

		AbstractSignal6Default1(Supplier<? extends F> arg6Default, Class<?> declaringClass, boolean isDisposedSignal){
            super(declaringClass, isDisposedSignal);
            if(arg6Default!=null){
                this.arg6Default = arg6Default;
            }else{
                throw new QNoDefaultValueException(6);
            }
        }
        
        AbstractSignal6Default1(Supplier<? extends F> arg6Default){
            super();
            if(arg6Default!=null){
                this.arg6Default = arg6Default;
            }else{
                throw new QNoDefaultValueException(6);
            }
        }
        
        AbstractSignal6Default1(Supplier<? extends F> arg6Default, Class<?>... types) {
            super(types);
            if(arg6Default!=null){
                this.arg6Default = arg6Default;
            }else{
                throw new QNoDefaultValueException(6);
            }
        }
        
        AbstractSignal6Default1(String signalName, Supplier<? extends F> arg6Default, Class<?>... types) {
            super(signalName, types);
            if(arg6Default!=null){
                this.arg6Default = arg6Default;
            }else{
                throw new QNoDefaultValueException(6);
            }
        }
        
        private final Supplier<? extends F> arg6Default;
        
        /**
         * Emits the signal with default value for arg6.
         * @param arg1
         * @param arg2
         * @param arg3
         * @param arg4
         * @param arg5
         */
        @Override
        public final void emit(A arg1, B arg2, C arg3, D arg4, E arg5) {
        	super.emitDefaultSignal(arg6Default, arg1, arg2, arg3, arg4, arg5);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends E> arg5Default, A arg1, B arg2, C arg3, D arg4) {
        	super.emitDefaultSignal(arg5Default, arg6Default, arg1, arg2, arg3, arg4);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, A arg1, B arg2, C arg3) {
        	super.emitDefaultSignal(arg4Default, arg5Default, arg6Default, arg1, arg2, arg3);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, A arg1, B arg2) {
        	super.emitDefaultSignal(arg3Default, arg4Default, arg5Default, arg6Default, arg1, arg2);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, A arg1) {
        	super.emitDefaultSignal(arg2Default, arg3Default, arg4Default, arg5Default, arg6Default, arg1);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends A> arg1Default,
        		Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default) {
        	super.emitDefaultSignal(arg1Default, arg2Default, arg3Default, arg4Default, arg5Default, arg6Default);
        }
    }

    /**
     * Supertype for all public default signals with six parameters.
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     */
    public static abstract class AbstractSignal6Default2<A, B, C, D, E, F> extends AbstractSignal6Default1<A, B, C, D, E, F> implements  Emitable4<A,B,C,D>, Connectable4<A,B,C,D> {
    	AbstractSignal6Default2() {
    		this.arg5Default = null;
    	}
    	
		AbstractSignal6Default2(Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Class<?> declaringClass, boolean isDisposedSignal){
            super(arg6Default, declaringClass, isDisposedSignal);
            if(arg5Default!=null){
                this.arg5Default = arg5Default;
            }else{
                throw new QNoDefaultValueException(5);
            }
        }
        
        AbstractSignal6Default2(Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default){
            super(arg6Default);
            if(arg5Default!=null){
                this.arg5Default = arg5Default;
            }else{
                throw new QNoDefaultValueException(5);
            }
        }
        
        AbstractSignal6Default2(Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Class<?>... types) {
            super(arg6Default, types);
            if(arg5Default!=null){
                this.arg5Default = arg5Default;
            }else{
                throw new QNoDefaultValueException(5);
            }
        }
        
        AbstractSignal6Default2(String signalName, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Class<?>... types) {
            super(signalName, arg6Default, types);
            if(arg5Default!=null){
                this.arg5Default = arg5Default;
            }else{
                throw new QNoDefaultValueException(5);
            }
        }
        
        private final Supplier<? extends E> arg5Default;
        
        /**
         * Emits the signal with default value for arg5.
         * @param arg1
         * @param arg2
         * @param arg3
         * @param arg4
         */
        @Override
        public final void emit(A arg1, B arg2, C arg3, D arg4) {
        	super.emitDefaultSignal(arg5Default, arg1, arg2, arg3, arg4);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends D> arg4Default, A arg1, B arg2, C arg3) {
        	super.emitDefaultSignal(arg4Default, arg5Default, arg1, arg2, arg3);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, A arg1, B arg2) {
        	super.emitDefaultSignal(arg3Default, arg4Default, arg5Default, arg1, arg2);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, A arg1) {
        	super.emitDefaultSignal(arg2Default, arg3Default, arg4Default, arg5Default, arg1);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends A> arg1Default,
        		Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default) {
        	super.emitDefaultSignal(arg1Default, arg2Default, arg3Default, arg4Default, arg5Default);
        }
    }
    
    /**
     * Supertype for all public default signals with six parameters.
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     */
    public static abstract class AbstractSignal6Default3<A, B, C, D, E, F> extends AbstractSignal6Default2<A, B, C, D, E, F> implements  Emitable3<A,B,C>, Connectable3<A,B,C> {
    	AbstractSignal6Default3() {
    		this.arg4Default = null;
    	}
    	
		AbstractSignal6Default3(Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Class<?> declaringClass, boolean isDisposedSignal){
            super(arg5Default, arg6Default, declaringClass, isDisposedSignal);
            if(arg4Default!=null){
                this.arg4Default = arg4Default;
            }else{
                throw new QNoDefaultValueException(4);
            }
        }
        
        AbstractSignal6Default3(Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default){
            super(arg5Default, arg6Default);
            if(arg4Default!=null){
                this.arg4Default = arg4Default;
            }else{
                throw new QNoDefaultValueException(4);
            }
        }
        
        AbstractSignal6Default3(Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Class<?>... types) {
            super(arg5Default, arg6Default, types);
            if(arg4Default!=null){
                this.arg4Default = arg4Default;
            }else{
                throw new QNoDefaultValueException(4);
            }
        }
        
        AbstractSignal6Default3(String signalName, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Class<?>... types) {
            super(signalName, arg5Default, arg6Default, types);
            if(arg4Default!=null){
                this.arg4Default = arg4Default;
            }else{
                throw new QNoDefaultValueException(4);
            }
        }
        
        private final Supplier<? extends D> arg4Default;
        
        /**
         * Emits the signal with default value for arg4.
         * @param arg1
         * @param arg2
         * @param arg3
         */
        @Override
        public final void emit(A arg1, B arg2, C arg3) {
        	super.emitDefaultSignal(arg4Default, arg1, arg2, arg3);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends C> arg3Default, A arg1, B arg2) {
        	super.emitDefaultSignal(arg3Default, arg4Default, arg1, arg2);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, A arg1) {
        	super.emitDefaultSignal(arg2Default, arg3Default, arg4Default, arg1);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends A> arg1Default,
        		Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default) {
        	super.emitDefaultSignal(arg1Default, arg2Default, arg3Default, arg4Default);
        }
    }
    
    /**
     * Supertype for all public default signals with six parameters.
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     */
    public static abstract class AbstractSignal6Default4<A, B, C, D, E, F> extends AbstractSignal6Default3<A, B, C, D, E, F> implements  Emitable2<A,B>, Connectable2<A,B> {
    	AbstractSignal6Default4() {
    		this.arg3Default = null;
    	}
    	
		AbstractSignal6Default4(Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Class<?> declaringClass, boolean isDisposedSignal){
            super(arg4Default, arg5Default, arg6Default, declaringClass, isDisposedSignal);
            if(arg3Default!=null){
                this.arg3Default = arg3Default;
            }else{
                throw new QNoDefaultValueException(3);
            }
        }
        
        AbstractSignal6Default4(Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default){
            super(arg4Default, arg5Default, arg6Default);
            if(arg3Default!=null){
                this.arg3Default = arg3Default;
            }else{
                throw new QNoDefaultValueException(3);
            }
        }
        
        AbstractSignal6Default4(Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Class<?>... types) {
            super(arg4Default, arg5Default, arg6Default, types);
            if(arg3Default!=null){
                this.arg3Default = arg3Default;
            }else{
                throw new QNoDefaultValueException(3);
            }
        }
        
        AbstractSignal6Default4(String signalName, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Class<?>... types) {
            super(signalName, arg4Default, arg5Default, arg6Default, types);
            if(arg3Default!=null){
                this.arg3Default = arg3Default;
            }else{
                throw new QNoDefaultValueException(3);
            }
        }
        
        private final Supplier<? extends C> arg3Default;
        
        /**
         * Emits the signal with default value for arg3.
         * @param arg1
         * @param arg2
         */
        @Override
        public final void emit(A arg1, B arg2) {
            super.emitDefaultSignal(arg3Default, arg1, arg2);
        }

        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends B> arg2Default, A arg1) {
        	super.emitDefaultSignal(arg2Default, arg3Default, arg1);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends A> arg1Default,
        		Supplier<? extends B> arg2Default) {
        	super.emitDefaultSignal(arg1Default, arg2Default, arg3Default);
        }
    }
    
    /**
     * Supertype for all public default signals with six parameters.
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     */
    public static abstract class AbstractSignal6Default5<A, B, C, D, E, F> extends AbstractSignal6Default4<A, B, C, D, E, F> implements  Emitable1<A>, Connectable1<A> {
    	AbstractSignal6Default5() {
    		this.arg2Default = null;
    	}
    	
		AbstractSignal6Default5(Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Class<?> declaringClass, boolean isDisposedSignal){
            super(arg3Default, arg4Default, arg5Default, arg6Default, declaringClass, isDisposedSignal);
            if(arg2Default!=null){
                this.arg2Default = arg2Default;
            }else{
                throw new QNoDefaultValueException(2);
            }
        }
        
        AbstractSignal6Default5(Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default){
            super(arg3Default, arg4Default, arg5Default, arg6Default);
            if(arg2Default!=null){
                this.arg2Default = arg2Default;
            }else{
                throw new QNoDefaultValueException(2);
            }
        }
        
        AbstractSignal6Default5(Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Class<?>... types) {
            super(arg3Default, arg4Default, arg5Default, arg6Default, types);
            if(arg2Default!=null){
                this.arg2Default = arg2Default;
            }else{
                throw new QNoDefaultValueException(2);
            }
        }
        
        AbstractSignal6Default5(String signalName, Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Class<?>... types) {
            super(signalName, arg3Default, arg4Default, arg5Default, arg6Default, types);
            if(arg2Default!=null){
                this.arg2Default = arg2Default;
            }else{
                throw new QNoDefaultValueException(2);
            }
        }
        
        private final Supplier<? extends B> arg2Default;
        
        /**
         * Emits the signal with default value for arg2.
         * @param arg1
         */
        @Override
        public final void emit(A arg1) {
            super.emitDefaultSignal(arg2Default, arg1);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends A> arg1Default) {
        	super.emitDefaultSignal(arg1Default, arg2Default);
        }
    }

    /**
     * Supertype for all public default signals with seven parameters.
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     */
    public static abstract class AbstractSignal7Default1<A, B, C, D, E, F, G> extends AbstractPublicSignal7<A, B, C, D, E, F, G> implements  Emitable6<A,B,C,D,E,F>, Connectable6<A,B,C,D,E,F> {
    	AbstractSignal7Default1() {
    		this.arg7Default = null;
    	}

		AbstractSignal7Default1(Supplier<? extends G> arg7Default, Class<?> declaringClass, boolean isDisposedSignal){
            super(declaringClass, isDisposedSignal);
            if(arg7Default!=null){
                this.arg7Default = arg7Default;
            }else{
                throw new QNoDefaultValueException(7);
            }
        }
        
        AbstractSignal7Default1(Supplier<? extends G> arg7Default){
            super();
            if(arg7Default!=null){
                this.arg7Default = arg7Default;
            }else{
                throw new QNoDefaultValueException(7);
            }
        }
        
        AbstractSignal7Default1(Supplier<? extends G> arg7Default, Class<?>... types) {
            super(types);
            if(arg7Default!=null){
                this.arg7Default = arg7Default;
            }else{
                throw new QNoDefaultValueException(7);
            }
        }
        
        AbstractSignal7Default1(String signalName, Supplier<? extends G> arg7Default, Class<?>... types) {
            super(signalName, types);
            if(arg7Default!=null){
                this.arg7Default = arg7Default;
            }else{
                throw new QNoDefaultValueException(7);
            }
        }
        
        private final Supplier<? extends G> arg7Default;
        
        /**
         * Emits the signal with default value for arg7.
         * @param arg1
         * @param arg2
         * @param arg3
         * @param arg4
         * @param arg5
         * @param arg6
         */
        @Override
        public final void emit(A arg1, B arg2, C arg3, D arg4, E arg5, F arg6) {
            super.emitDefaultSignal(arg7Default, arg1, arg2, arg3, arg4, arg5, arg6);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends F> arg6Default, 
        		A arg1, B arg2, C arg3, D arg4, E arg5) {
        	super.emitDefaultSignal(arg6Default, arg7Default, arg1, arg2, arg3, arg4, arg5);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, 
        		A arg1, B arg2, C arg3, D arg4) {
        	super.emitDefaultSignal(arg5Default, arg6Default, arg7Default, arg1, arg2, arg3, arg4);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, 
        		Supplier<? extends F> arg6Default, A arg1, B arg2, C arg3) {
        	super.emitDefaultSignal(arg4Default, arg5Default, arg6Default, arg7Default, arg1, arg2, arg3);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, 
        		Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, A arg1, B arg2) {
        	super.emitDefaultSignal(arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg1, arg2);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, 
        		Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, A arg1) {
        	super.emitDefaultSignal(arg2Default, arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg1);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends A> arg1Default,
        		Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, 
        		Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default) {
        	super.emitDefaultSignal(arg1Default, arg2Default, arg3Default, arg4Default, arg5Default, arg6Default, arg7Default);
        }
    }
    
    /**
     * Supertype for all public default signals with seven parameters.
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     */
    public static abstract class AbstractSignal7Default2<A, B, C, D, E, F, G> extends AbstractSignal7Default1<A, B, C, D, E, F, G> implements  Emitable5<A,B,C,D,E>, Connectable5<A,B,C,D,E> {
    	AbstractSignal7Default2() {
    		this.arg6Default = null;
    	}

		AbstractSignal7Default2(Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Class<?> declaringClass, boolean isDisposedSignal){
            super(arg7Default, declaringClass, isDisposedSignal);
            this.arg6Default = arg6Default;
            if(this.arg6Default==null){
                throw new QNoDefaultValueException(6);
            }
        }
        
        AbstractSignal7Default2(Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default){
            super(arg7Default);
            this.arg6Default = arg6Default;
            if(this.arg6Default==null){
                throw new QNoDefaultValueException(6);
            }
        }
        
        AbstractSignal7Default2(Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Class<?>... types) {
            super(arg7Default, types);
            this.arg6Default = arg6Default;
            if(this.arg6Default==null){
                throw new QNoDefaultValueException(6);
            }
        }
        
        AbstractSignal7Default2(String signalName, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Class<?>... types) {
            super(signalName, arg7Default, types);
            this.arg6Default = arg6Default;
            if(this.arg6Default==null){
                throw new QNoDefaultValueException(6);
            }
        }
        
        private final Supplier<? extends F> arg6Default;
        
        /**
         * Emits the signal with default value for arg6.
         * @param arg1
         * @param arg2
         * @param arg3
         * @param arg4
         * @param arg5
         */
        @Override
        public final void emit(A arg1, B arg2, C arg3, D arg4, E arg5) {
            super.emitDefaultSignal(arg6Default, arg1, arg2, arg3, arg4, arg5);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends E> arg5Default, A arg1, B arg2, C arg3, D arg4) {
        	super.emitDefaultSignal(arg5Default, arg6Default, arg1, arg2, arg3, arg4);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, A arg1, B arg2, C arg3) {
        	super.emitDefaultSignal(arg4Default, arg5Default, arg6Default, arg1, arg2, arg3);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, A arg1, B arg2) {
        	super.emitDefaultSignal(arg3Default, arg4Default, arg5Default, arg6Default, arg1, arg2);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, A arg1) {
        	super.emitDefaultSignal(arg2Default, arg3Default, arg4Default, arg5Default, arg6Default, arg1);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends A> arg1Default,
        		Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default) {
        	super.emitDefaultSignal(arg1Default, arg2Default, arg3Default, arg4Default, arg5Default, arg6Default);
        }
    }
    
    /**
     * Supertype for all public default signals with seven parameters.
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     */
    public static abstract class AbstractSignal7Default3<A, B, C, D, E, F, G> extends AbstractSignal7Default2<A, B, C, D, E, F, G> implements  Emitable4<A,B,C,D>, Connectable4<A,B,C,D> {
    	AbstractSignal7Default3() {
    		this.arg5Default = null;
    	}
    	
		AbstractSignal7Default3(Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Class<?> declaringClass, boolean isDisposedSignal){
            super(arg6Default, arg7Default, declaringClass, isDisposedSignal);
            this.arg5Default = arg5Default;
            if(this.arg5Default==null){
                throw new QNoDefaultValueException(5);
            }
        }
        
        AbstractSignal7Default3(Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default){
            super(arg6Default, arg7Default);
            this.arg5Default = arg5Default;
            if(this.arg5Default==null){
                throw new QNoDefaultValueException(5);
            }
        }
        
        AbstractSignal7Default3(Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Class<?>... types) {
            super(arg6Default, arg7Default, types);
            this.arg5Default = arg5Default;
            if(this.arg5Default==null){
                throw new QNoDefaultValueException(5);
            }
        }
        
        AbstractSignal7Default3(String signalName, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Class<?>... types) {
            super(signalName, arg6Default, arg7Default, types);
            this.arg5Default = arg5Default;
            if(this.arg5Default==null){
                throw new QNoDefaultValueException(5);
            }
        }
        
        private final Supplier<? extends E> arg5Default;
        
        /**
         * Emits the signal with default value for arg5.
         * @param arg1
         * @param arg2
         * @param arg3
         * @param arg4
         */
        @Override
        public final void emit(A arg1, B arg2, C arg3, D arg4) {
            super.emitDefaultSignal(arg5Default, arg1, arg2, arg3, arg4);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends D> arg4Default, A arg1, B arg2, C arg3) {
        	super.emitDefaultSignal(arg4Default, arg5Default, arg1, arg2, arg3);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, A arg1, B arg2) {
        	super.emitDefaultSignal(arg3Default, arg4Default, arg5Default, arg1, arg2);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, A arg1) {
        	super.emitDefaultSignal(arg2Default, arg3Default, arg4Default, arg5Default, arg1);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends A> arg1Default,
        		Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default) {
        	super.emitDefaultSignal(arg1Default, arg2Default, arg3Default, arg4Default, arg5Default);
        }
    }
    
    /**
     * Supertype for all public default signals with seven parameters.
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     */
    public static abstract class AbstractSignal7Default4<A, B, C, D, E, F, G> extends AbstractSignal7Default3<A, B, C, D, E, F, G> implements  Emitable3<A,B,C>, Connectable3<A,B,C> {
    	AbstractSignal7Default4() {
    		this.arg4Default = null;
    	}
    	
		AbstractSignal7Default4(Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Class<?> declaringClass, boolean isDisposedSignal){
            super(arg5Default, arg6Default, arg7Default, declaringClass, isDisposedSignal);
            this.arg4Default = arg4Default;
            if(this.arg4Default==null){
                throw new QNoDefaultValueException(4);
            }
        }
        
        AbstractSignal7Default4(Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default){
            super(arg5Default, arg6Default, arg7Default);
            this.arg4Default = arg4Default;
            if(this.arg4Default==null){
                throw new QNoDefaultValueException(4);
            }
        }
        
        AbstractSignal7Default4(Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Class<?>... types) {
            super(arg5Default, arg6Default, arg7Default, types);
            this.arg4Default = arg4Default;
            if(this.arg4Default==null){
                throw new QNoDefaultValueException(4);
            }
        }
        
        AbstractSignal7Default4(String signalName, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Class<?>... types) {
            super(signalName, arg5Default, arg6Default, arg7Default, types);
            this.arg4Default = arg4Default;
            if(this.arg4Default==null){
                throw new QNoDefaultValueException(4);
            }
        }
        
        private final Supplier<? extends D> arg4Default;
        
        /**
         * Emits the signal with default value for arg4.
         * @param arg1
         * @param arg2
         * @param arg3
         */
        @Override
        public final void emit(A arg1, B arg2, C arg3) {
            super.emitDefaultSignal(arg4Default, arg1, arg2, arg3);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends C> arg3Default, A arg1, B arg2) {
        	super.emitDefaultSignal(arg3Default, arg4Default, arg1, arg2);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, A arg1) {
        	super.emitDefaultSignal(arg2Default, arg3Default, arg4Default, arg1);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends A> arg1Default,
        		Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default) {
        	super.emitDefaultSignal(arg1Default, arg2Default, arg3Default, arg4Default);
        }
    }
    
    /**
     * Supertype for all public default signals with seven parameters.
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     */
    public static abstract class AbstractSignal7Default5<A, B, C, D, E, F, G> extends AbstractSignal7Default4<A, B, C, D, E, F, G> implements  Emitable2<A,B>, Connectable2<A,B> {
    	AbstractSignal7Default5() {
    		this.arg3Default = null;
    	}
    	
		AbstractSignal7Default5(Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Class<?> declaringClass, boolean isDisposedSignal){
            super(arg4Default, arg5Default, arg6Default, arg7Default, declaringClass, isDisposedSignal);
            this.arg3Default = arg3Default;
            if(this.arg3Default==null){
                throw new QNoDefaultValueException(3);
            }
        }
        
        AbstractSignal7Default5(Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default){
            super(arg4Default, arg5Default, arg6Default, arg7Default);
            this.arg3Default = arg3Default;
            if(this.arg3Default==null){
                throw new QNoDefaultValueException(3);
            }
        }
        
        AbstractSignal7Default5(Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Class<?>... types) {
            super(arg4Default, arg5Default, arg6Default, arg7Default, types);
            this.arg3Default = arg3Default;
            if(this.arg3Default==null){
                throw new QNoDefaultValueException(3);
            }
        }
        
        AbstractSignal7Default5(String signalName, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Class<?>... types) {
            super(signalName, arg4Default, arg5Default, arg6Default, arg7Default, types);
            this.arg3Default = arg3Default;
            if(this.arg3Default==null){
                throw new QNoDefaultValueException(3);
            }
        }
        
        private final Supplier<? extends C> arg3Default;
        
        /**
         * Emits the signal with default value for arg3.
         * @param arg1
         * @param arg2
         */
        @Override
        public final void emit(A arg1, B arg2) {
            super.emitDefaultSignal(arg3Default, arg1, arg2);
        }

        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends B> arg2Default, A arg1) {
        	super.emitDefaultSignal(arg2Default, arg3Default, arg1);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends A> arg1Default,
        		Supplier<? extends B> arg2Default) {
        	super.emitDefaultSignal(arg1Default, arg2Default, arg3Default);
        }
    }
    
    /**
     * Supertype for all public default signals with seven parameters.
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     */
    public static abstract class AbstractSignal7Default6<A, B, C, D, E, F, G> extends AbstractSignal7Default5<A, B, C, D, E, F, G> implements  Emitable1<A>, Connectable1<A> {
    	AbstractSignal7Default6() {
    		this.arg2Default = null;
    	}
    	
		AbstractSignal7Default6(Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Class<?> declaringClass, boolean isDisposedSignal){
            super(arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, declaringClass, isDisposedSignal);
            this.arg2Default = arg2Default;
            if(this.arg2Default==null){
                throw new QNoDefaultValueException(2);
            }
        }
        
        AbstractSignal7Default6(Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default){
            super(arg3Default, arg4Default, arg5Default, arg6Default, arg7Default);
            this.arg2Default = arg2Default;
            if(this.arg2Default==null){
                throw new QNoDefaultValueException(2);
            }
        }
        
        AbstractSignal7Default6(Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Class<?>... types) {
            super(arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, types);
            this.arg2Default = arg2Default;
            if(this.arg2Default==null){
                throw new QNoDefaultValueException(2);
            }
        }
        
        AbstractSignal7Default6(String signalName, Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Class<?>... types) {
            super(signalName, arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, types);
            this.arg2Default = arg2Default;
            if(this.arg2Default==null){
                throw new QNoDefaultValueException(2);
            }
        }
        
        private final Supplier<? extends B> arg2Default;
        
        /**
         * Emits the signal with default value for arg2.
         * @param arg1
         */
        @Override
        public final void emit(A arg1) {
            super.emitDefaultSignal(arg2Default, arg1);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends A> arg1Default) {
        	super.emitDefaultSignal(arg1Default, arg2Default);
        }
    }
    
    /**
     * Supertype for all public default signals with eight parameters.
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     * @param <H> The type of the eighth parameter of the signal.
     */
    public static abstract class AbstractSignal8Default1<A, B, C, D, E, F, G, H> extends AbstractPublicSignal8<A, B, C, D, E, F, G, H> implements  Emitable7<A,B,C,D,E,F,G>, Connectable7<A,B,C,D,E,F,G> {
    	AbstractSignal8Default1() {
    		this.arg8Default = null;
    	}
        
		AbstractSignal8Default1(Supplier<? extends H> arg8Default, Class<?> declaringClass, boolean isDisposedSignal){
            super(declaringClass, isDisposedSignal);
            this.arg8Default = arg8Default;
            if(this.arg8Default==null){
                throw new QNoDefaultValueException(8);
            }
        }
        
        AbstractSignal8Default1(Supplier<? extends H> arg8Default){
            super();
            this.arg8Default = arg8Default;
            if(this.arg8Default==null){
                throw new QNoDefaultValueException(8);
            }
        }
        
        AbstractSignal8Default1(Supplier<? extends H> arg8Default, Class<?>... types) {
            super(types);
            this.arg8Default = arg8Default;
            if(this.arg8Default==null){
                throw new QNoDefaultValueException(8);
            }
        }
        
        AbstractSignal8Default1(String signalName, Supplier<? extends H> arg8Default, Class<?>... types) {
            super(signalName, types);
            this.arg8Default = arg8Default;
            if(this.arg8Default==null){
                throw new QNoDefaultValueException(8);
            }
        }
        
        private final Supplier<? extends H> arg8Default;
        
        /**
         * Emits the signal with default value for arg8.
         * @param arg1
         * @param arg2
         * @param arg3
         * @param arg4
         * @param arg5
         * @param arg6
         * @param arg7
         */
        @Override
        public final void emit(A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7) {
            super.emitDefaultSignal(arg8Default, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends G> arg7Default, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6) {
        	super.emitDefaultSignal(arg7Default, arg8Default, arg1, arg2, arg3, arg4, arg5, arg6);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends F> arg6Default, 
        		Supplier<? extends G> arg7Default, A arg1, B arg2, C arg3, D arg4, E arg5) {
        	super.emitDefaultSignal(arg6Default, arg7Default, arg8Default, arg1, arg2, arg3, arg4, arg5);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, 
        		Supplier<? extends G> arg7Default, A arg1, B arg2, C arg3, D arg4) {
        	super.emitDefaultSignal(arg5Default, arg6Default, arg7Default, arg8Default, arg1, arg2, arg3, arg4);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, 
        		Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, A arg1, B arg2, C arg3) {
        	super.emitDefaultSignal(arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, arg1, arg2, arg3);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, 
        		Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, A arg1, B arg2) {
        	super.emitDefaultSignal(arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, arg1, arg2);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, 
        		Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, A arg1) {
        	super.emitDefaultSignal(arg2Default, arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, arg1);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends A> arg1Default,
        		Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, 
        		Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default) {
        	super.emitDefaultSignal(arg1Default, arg2Default, arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default);
        }
    }
    
    /**
     * Supertype for all public default signals with eight parameters.
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     * @param <H> The type of the eighth parameter of the signal.
     */
    public static abstract class AbstractSignal8Default2<A, B, C, D, E, F, G, H> extends AbstractSignal8Default1<A, B, C, D, E, F, G, H> implements  Emitable6<A,B,C,D,E,F>, Connectable6<A,B,C,D,E,F> {
    	AbstractSignal8Default2() {
    		this.arg7Default = null;
    	}

		AbstractSignal8Default2(Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default, Class<?> declaringClass, boolean isDisposedSignal){
            super(arg8Default, declaringClass, isDisposedSignal);
            this.arg7Default = arg7Default;
            if(this.arg7Default==null){
                throw new QNoDefaultValueException(7);
            }
        }
        
        AbstractSignal8Default2(Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default){
            super(arg8Default);
            this.arg7Default = arg7Default;
            if(this.arg7Default==null){
                throw new QNoDefaultValueException(7);
            }
        }
        
        AbstractSignal8Default2(Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default, Class<?>... types) {
            super(arg8Default, types);
            this.arg7Default = arg7Default;
            if(this.arg7Default==null){
                throw new QNoDefaultValueException(7);
            }
        }
        
        AbstractSignal8Default2(String signalName, Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default, Class<?>... types) {
            super(signalName, arg8Default, types);
            this.arg7Default = arg7Default;
            if(this.arg7Default==null){
                throw new QNoDefaultValueException(7);
            }
        }
        
        private final Supplier<? extends G> arg7Default;
        
        /**
         * Emits the signal with default value for arg7.
         * @param arg1
         * @param arg2
         * @param arg3
         * @param arg4
         * @param arg5
         * @param arg6
         */
        @Override
        public final void emit(A arg1, B arg2, C arg3, D arg4, E arg5, F arg6) {
            super.emitDefaultSignal(arg7Default, arg1, arg2, arg3, arg4, arg5, arg6);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends F> arg6Default, 
        		A arg1, B arg2, C arg3, D arg4, E arg5) {
        	super.emitDefaultSignal(arg6Default, arg7Default, arg1, arg2, arg3, arg4, arg5);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, 
        		A arg1, B arg2, C arg3, D arg4) {
        	super.emitDefaultSignal(arg5Default, arg6Default, arg7Default, arg1, arg2, arg3, arg4);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, 
        		Supplier<? extends F> arg6Default, A arg1, B arg2, C arg3) {
        	super.emitDefaultSignal(arg4Default, arg5Default, arg6Default, arg7Default, arg1, arg2, arg3);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, 
        		Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, A arg1, B arg2) {
        	super.emitDefaultSignal(arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg1, arg2);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, 
        		Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, A arg1) {
        	super.emitDefaultSignal(arg2Default, arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg1);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends A> arg1Default,
        		Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, 
        		Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default) {
        	super.emitDefaultSignal(arg1Default, arg2Default, arg3Default, arg4Default, arg5Default, arg6Default, arg7Default);
        }
    }
    
    /**
     * Supertype for all public default signals with eight parameters.
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     * @param <H> The type of the eighth parameter of the signal.
     */
    public static abstract class AbstractSignal8Default3<A, B, C, D, E, F, G, H> extends AbstractSignal8Default2<A, B, C, D, E, F, G, H> implements  Emitable5<A,B,C,D,E>, Connectable5<A,B,C,D,E> {
    	AbstractSignal8Default3() {
    		this.arg6Default = null;
    	}
    	
		AbstractSignal8Default3(Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default, Class<?> declaringClass, boolean isDisposedSignal){
            super(arg7Default, arg8Default, declaringClass, isDisposedSignal);
            this.arg6Default = arg6Default;
            if(this.arg6Default==null){
                throw new QNoDefaultValueException(6);
            }
        }
        
        AbstractSignal8Default3(Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default){
            super(arg7Default, arg8Default);
            this.arg6Default = arg6Default;
            if(this.arg6Default==null){
                throw new QNoDefaultValueException(6);
            }
        }
        
        AbstractSignal8Default3(Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default, Class<?>... types) {
            super(arg7Default, arg8Default, types);
            this.arg6Default = arg6Default;
            if(this.arg6Default==null){
                throw new QNoDefaultValueException(6);
            }
        }
        
        AbstractSignal8Default3(String signalName, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default, Class<?>... types) {
            super(signalName, arg7Default, arg8Default, types);
            this.arg6Default = arg6Default;
            if(this.arg6Default==null){
                throw new QNoDefaultValueException(6);
            }
        }
        
        private final Supplier<? extends F> arg6Default;
        
        /**
         * Emits the signal with default value for arg6.
         * @param arg1
         * @param arg2
         * @param arg3
         * @param arg4
         * @param arg5
         */
        @Override
        public final void emit(A arg1, B arg2, C arg3, D arg4, E arg5) {
            super.emitDefaultSignal(arg6Default, arg1, arg2, arg3, arg4, arg5);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends E> arg5Default, A arg1, B arg2, C arg3, D arg4) {
        	super.emitDefaultSignal(arg5Default, arg6Default, arg1, arg2, arg3, arg4);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, A arg1, B arg2, C arg3) {
        	super.emitDefaultSignal(arg4Default, arg5Default, arg6Default, arg1, arg2, arg3);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, A arg1, B arg2) {
        	super.emitDefaultSignal(arg3Default, arg4Default, arg5Default, arg6Default, arg1, arg2);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, A arg1) {
        	super.emitDefaultSignal(arg2Default, arg3Default, arg4Default, arg5Default, arg6Default, arg1);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends A> arg1Default,
        		Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default) {
        	super.emitDefaultSignal(arg1Default, arg2Default, arg3Default, arg4Default, arg5Default, arg6Default);
        }
    }
    
    /**
     * Supertype for all public default signals with eight parameters.
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     * @param <H> The type of the eighth parameter of the signal.
     */
    public static abstract class AbstractSignal8Default4<A, B, C, D, E, F, G, H> extends AbstractSignal8Default3<A, B, C, D, E, F, G, H> implements  Emitable4<A,B,C,D>, Connectable4<A,B,C,D> {
    	AbstractSignal8Default4() {
    		this.arg5Default = null;
    	}
    	
		AbstractSignal8Default4(Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default){
            super(arg6Default, arg7Default, arg8Default);
            this.arg5Default = arg5Default;
            if(this.arg5Default==null){
                throw new QNoDefaultValueException(5);
            }
        }
        
        AbstractSignal8Default4(Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default, Class<?> declaringClass, boolean isDisposedSignal){
            super(arg6Default, arg7Default, arg8Default, declaringClass, isDisposedSignal);
            this.arg5Default = arg5Default;
            if(this.arg5Default==null){
                throw new QNoDefaultValueException(5);
            }
        }
        
        AbstractSignal8Default4(Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default, Class<?>... types) {
            super(arg6Default, arg7Default, arg8Default, types);
            this.arg5Default = arg5Default;
            if(this.arg5Default==null){
                throw new QNoDefaultValueException(5);
            }
        }
        
        AbstractSignal8Default4(String signalName, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default, Class<?>... types) {
            super(signalName, arg6Default, arg7Default, arg8Default, types);
            this.arg5Default = arg5Default;
            if(this.arg5Default==null){
                throw new QNoDefaultValueException(5);
            }
        }
        
        private final Supplier<? extends E> arg5Default;
        
        /**
         * Emits the signal with default value for arg5.
         * @param arg1
         * @param arg2
         * @param arg3
         * @param arg4
         */
        @Override
        public final void emit(A arg1, B arg2, C arg3, D arg4) {
            super.emitDefaultSignal(arg5Default, arg1, arg2, arg3, arg4);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends D> arg4Default, A arg1, B arg2, C arg3) {
        	super.emitDefaultSignal(arg4Default, arg5Default, arg1, arg2, arg3);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, A arg1, B arg2) {
        	super.emitDefaultSignal(arg3Default, arg4Default, arg5Default, arg1, arg2);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, A arg1) {
        	super.emitDefaultSignal(arg2Default, arg3Default, arg4Default, arg5Default, arg1);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends A> arg1Default,
        		Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default) {
        	super.emitDefaultSignal(arg1Default, arg2Default, arg3Default, arg4Default, arg5Default);
        }
    }
    
    /**
     * Supertype for all public default signals with eight parameters.
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     * @param <H> The type of the eighth parameter of the signal.
     */
    public static abstract class AbstractSignal8Default5<A, B, C, D, E, F, G, H> extends AbstractSignal8Default4<A, B, C, D, E, F, G, H> implements  Emitable3<A,B,C>, Connectable3<A,B,C> {
    	AbstractSignal8Default5() {
    		this.arg4Default = null;
    	}
    	
		AbstractSignal8Default5(Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default, Class<?> declaringClass, boolean isDisposedSignal){
            super(arg5Default, arg6Default, arg7Default, arg8Default, declaringClass, isDisposedSignal);
            this.arg4Default = arg4Default;
            if(this.arg4Default==null){
                throw new QNoDefaultValueException(4);
            }
        }
        
        AbstractSignal8Default5(Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default){
            super(arg5Default, arg6Default, arg7Default, arg8Default);
            this.arg4Default = arg4Default;
            if(this.arg4Default==null){
                throw new QNoDefaultValueException(4);
            }
        }
        
        AbstractSignal8Default5(Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default, Class<?>... types) {
            super(arg5Default, arg6Default, arg7Default, arg8Default, types);
            this.arg4Default = arg4Default;
            if(this.arg4Default==null){
                throw new QNoDefaultValueException(4);
            }
        }
        
        AbstractSignal8Default5(String signalName, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default, Class<?>... types) {
            super(signalName, arg5Default, arg6Default, arg7Default, arg8Default, types);
            this.arg4Default = arg4Default;
            if(this.arg4Default==null){
                throw new QNoDefaultValueException(4);
            }
        }
        
        private final Supplier<? extends D> arg4Default;
        
        /**
         * Emits the signal with default value for arg4.
         * @param arg1
         * @param arg2
         * @param arg3
         */
        @Override
        public final void emit(A arg1, B arg2, C arg3) {
            super.emitDefaultSignal(arg4Default, arg1, arg2, arg3);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends C> arg3Default, A arg1, B arg2) {
        	super.emitDefaultSignal(arg3Default, arg4Default, arg1, arg2);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, A arg1) {
        	super.emitDefaultSignal(arg2Default, arg3Default, arg4Default, arg1);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends A> arg1Default,
        		Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default) {
        	super.emitDefaultSignal(arg1Default, arg2Default, arg3Default, arg4Default);
        }
    }
    
    /**
     * Supertype for all public default signals with eight parameters.
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     * @param <H> The type of the eighth parameter of the signal.
     */
    public static abstract class AbstractSignal8Default6<A, B, C, D, E, F, G, H> extends AbstractSignal8Default5<A, B, C, D, E, F, G, H> implements  Emitable2<A,B>, Connectable2<A,B> {
    	AbstractSignal8Default6() {
    		this.arg3Default = null;
    	}
    	
		AbstractSignal8Default6(Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default, Class<?> declaringClass, boolean isDisposedSignal){
            super(arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, declaringClass, isDisposedSignal);
            this.arg3Default = arg3Default;
            if(this.arg3Default==null){
                throw new QNoDefaultValueException(3);
            }
        }
        
        AbstractSignal8Default6(Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default){
            super(arg4Default, arg5Default, arg6Default, arg7Default, arg8Default);
            this.arg3Default = arg3Default;
            if(this.arg3Default==null){
                throw new QNoDefaultValueException(3);
            }
        }
        
        AbstractSignal8Default6(Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default, Class<?>... types) {
            super(arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, types);
            this.arg3Default = arg3Default;
            if(this.arg3Default==null){
                throw new QNoDefaultValueException(3);
            }
        }
        
        AbstractSignal8Default6(String signalName, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default, Class<?>... types) {
            super(signalName, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, types);
            this.arg3Default = arg3Default;
            if(this.arg3Default==null){
                throw new QNoDefaultValueException(3);
            }
        }
        
        private final Supplier<? extends C> arg3Default;
        
        /**
         * Emits the signal with default value for arg3.
         * @param arg1
         * @param arg2
         */
        @Override
        public final void emit(A arg1, B arg2) {
            super.emitDefaultSignal(arg3Default, arg1, arg2);
        }

        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends B> arg2Default, A arg1) {
        	super.emitDefaultSignal(arg2Default, arg3Default, arg1);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends A> arg1Default,
        		Supplier<? extends B> arg2Default) {
        	super.emitDefaultSignal(arg1Default, arg2Default, arg3Default);
        }
    }
    
    /**
     * Supertype for all public default signals with eight parameters.
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     * @param <H> The type of the eighth parameter of the signal.
     */
    public static abstract class AbstractSignal8Default7<A, B, C, D, E, F, G, H> extends AbstractSignal8Default6<A, B, C, D, E, F, G, H> implements  Emitable1<A>, Connectable1<A> {
    	AbstractSignal8Default7() {
    		this.arg2Default = null;
    	}
    	
		AbstractSignal8Default7(Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default){
            super(arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default);
            this.arg2Default = arg2Default;
            if(this.arg2Default==null){
                throw new QNoDefaultValueException(2);
            }
        }
        
        AbstractSignal8Default7(Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default, Class<?> declaringClass, boolean isDisposedSignal){
            super(arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, declaringClass, isDisposedSignal);
            this.arg2Default = arg2Default;
            if(this.arg2Default==null){
                throw new QNoDefaultValueException(2);
            }
        }
        
        AbstractSignal8Default7(Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default, Class<?>... types) {
            super(arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, types);
            this.arg2Default = arg2Default;
            if(this.arg2Default==null){
                throw new QNoDefaultValueException(2);
            }
        }
        
        AbstractSignal8Default7(String signalName, Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default, Class<?>... types) {
            super(signalName, arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, types);
            this.arg2Default = arg2Default;
            if(this.arg2Default==null){
                throw new QNoDefaultValueException(2);
            }
        }
        
        private final Supplier<? extends B> arg2Default;
        
        /**
         * Emits the signal with default value for arg2.
         * @param arg1
         */
        @Override
        public final void emit(A arg1) {
            super.emitDefaultSignal(arg2Default, arg1);
        }
    }
    
    /**
     * Supertype for all public default signals with nine parameters.
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
    public static abstract class AbstractSignal9Default1<A, B, C, D, E, F, G, H, I> extends AbstractPublicSignal9<A, B, C, D, E, F, G, H, I> implements  Emitable8<A,B,C,D,E,F,G,H>, Connectable8<A,B,C,D,E,F,G,H> {
    	AbstractSignal9Default1() {
    		this.arg9Default = null;
    	}
        
		AbstractSignal9Default1(Supplier<? extends I> arg9Default){
            super();
            this.arg9Default = arg9Default;
            if(this.arg9Default==null){
                throw new QNoDefaultValueException(9);
            }
        }
        
        AbstractSignal9Default1(Supplier<? extends I> arg9Default, Class<?> declaringClass, boolean isDisposedSignal){
            super(declaringClass, isDisposedSignal);
            this.arg9Default = arg9Default;
            if(this.arg9Default==null){
                throw new QNoDefaultValueException(9);
            }
        }
        
        AbstractSignal9Default1(Supplier<? extends I> arg9Default, Class<?>... types) {
            super(types);
            this.arg9Default = arg9Default;
            if(this.arg9Default==null){
                throw new QNoDefaultValueException(9);
            }
        }
        
        AbstractSignal9Default1(String signalName, Supplier<? extends I> arg9Default, Class<?>... types) {
            super(signalName, types);
            this.arg9Default = arg9Default;
            if(this.arg9Default==null){
                throw new QNoDefaultValueException(9);
            }
        }
        
        private final Supplier<? extends I> arg9Default;
        
        /**
         * Emits the signal with default value for arg9.
         * @param arg1
         * @param arg2
         * @param arg3
         * @param arg4
         * @param arg5
         * @param arg6
         * @param arg7
         * @param arg8
         */
        @Override
        public final void emit(A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7, H arg8) {
            super.emitDefaultSignal(arg9Default, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends H> arg8Default, 
        		A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7) {
        	super.emitDefaultSignal(arg8Default, arg9Default, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default, 
        		A arg1, B arg2, C arg3, D arg4, E arg5, F arg6) {
        	super.emitDefaultSignal(arg7Default, arg8Default, arg9Default, arg1, arg2, arg3, arg4, arg5, arg6);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends F> arg6Default, 
        		Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default, A arg1, B arg2, C arg3, D arg4, E arg5) {
        	super.emitDefaultSignal(arg6Default, arg7Default, arg8Default, arg9Default, arg1, arg2, arg3, arg4, arg5);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, 
        		Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default, A arg1, B arg2, C arg3, D arg4) {
        	super.emitDefaultSignal(arg5Default, arg6Default, arg7Default, arg8Default, arg9Default, arg1, arg2, arg3, arg4);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, 
        		Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default, 
        		A arg1, B arg2, C arg3) {
        	super.emitDefaultSignal(arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, arg9Default, arg1, arg2, arg3);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, 
        		Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, 
        		Supplier<? extends H> arg8Default, A arg1, B arg2) {
        	super.emitDefaultSignal(arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, arg9Default, arg1, arg2);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, 
        		Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default,
        		Supplier<? extends H> arg8Default, A arg1) {
        	super.emitDefaultSignal(arg2Default, arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, arg9Default, arg1);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends A> arg1Default,
        		Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, 
        		Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default,
        		Supplier<? extends H> arg8Default) {
        	super.emitDefaultSignal(arg1Default, arg2Default, arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, arg9Default);
        }
    }
    
    /**
     * Supertype for all public default signals with nine parameters.
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
    public static abstract class AbstractSignal9Default2<A, B, C, D, E, F, G, H, I> extends AbstractSignal9Default1<A, B, C, D, E, F, G, H, I> implements  Emitable7<A,B,C,D,E,F,G>, Connectable7<A,B,C,D,E,F,G> {
    	AbstractSignal9Default2() {
    		this.arg8Default = null;
    	}

		AbstractSignal9Default2(Supplier<? extends H> arg8Default, Supplier<? extends I> arg9Default){
            super(arg9Default);
            this.arg8Default = arg8Default;
            if(this.arg8Default==null){
                throw new QNoDefaultValueException(8);
            }
        }
        
        AbstractSignal9Default2(Supplier<? extends H> arg8Default, Supplier<? extends I> arg9Default, Class<?> declaringClass, boolean isDisposedSignal){
            super(arg9Default, declaringClass, isDisposedSignal);
            this.arg8Default = arg8Default;
            if(this.arg8Default==null){
                throw new QNoDefaultValueException(8);
            }
        }
        
        AbstractSignal9Default2(Supplier<? extends H> arg8Default, Supplier<? extends I> arg9Default, Class<?>... types) {
            super(arg9Default, types);
            this.arg8Default = arg8Default;
            if(this.arg8Default==null){
                throw new QNoDefaultValueException(8);
            }
        }
        
        AbstractSignal9Default2(String signalName, Supplier<? extends H> arg8Default, Supplier<? extends I> arg9Default, Class<?>... types) {
            super(signalName, arg9Default, types);
            this.arg8Default = arg8Default;
            if(this.arg8Default==null){
                throw new QNoDefaultValueException(8);
            }
        }
        
        private final Supplier<? extends H> arg8Default;
        
        /**
         * Emits the signal with default value for arg8.
         * @param arg1
         * @param arg2
         * @param arg3
         * @param arg4
         * @param arg5
         * @param arg6
         * @param arg7
         */
        @Override
        public final void emit(A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7) {
            super.emitDefaultSignal(arg8Default, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends G> arg7Default, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6) {
        	super.emitDefaultSignal(arg7Default, arg8Default, arg1, arg2, arg3, arg4, arg5, arg6);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends F> arg6Default, 
        		Supplier<? extends G> arg7Default, A arg1, B arg2, C arg3, D arg4, E arg5) {
        	super.emitDefaultSignal(arg6Default, arg7Default, arg8Default, arg1, arg2, arg3, arg4, arg5);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, 
        		Supplier<? extends G> arg7Default, A arg1, B arg2, C arg3, D arg4) {
        	super.emitDefaultSignal(arg5Default, arg6Default, arg7Default, arg8Default, arg1, arg2, arg3, arg4);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, 
        		Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, A arg1, B arg2, C arg3) {
        	super.emitDefaultSignal(arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, arg1, arg2, arg3);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, 
        		Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, A arg1, B arg2) {
        	super.emitDefaultSignal(arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, arg1, arg2);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, 
        		Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, A arg1) {
        	super.emitDefaultSignal(arg2Default, arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, arg1);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends A> arg1Default,
        		Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, 
        		Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default) {
        	super.emitDefaultSignal(arg1Default, arg2Default, arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default);
        }
    }
    
    /**
     * Supertype for all public default signals with nine parameters.
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
    public static abstract class AbstractSignal9Default3<A, B, C, D, E, F, G, H, I> extends AbstractSignal9Default2<A, B, C, D, E, F, G, H, I> implements  Emitable6<A,B,C,D,E,F>, Connectable6<A,B,C,D,E,F> {
    	AbstractSignal9Default3() {
    		this.arg7Default = null;
    	}

		AbstractSignal9Default3(Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default, Supplier<? extends I> arg9Default){
            super(arg8Default, arg9Default);
            this.arg7Default = arg7Default;
            if(this.arg7Default==null){
                throw new QNoDefaultValueException(7);
            }
        }
        
        AbstractSignal9Default3(Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default, Supplier<? extends I> arg9Default, Class<?> declaringClass, boolean isDisposedSignal){
            super(arg8Default, arg9Default, declaringClass, isDisposedSignal);
            this.arg7Default = arg7Default;
            if(this.arg7Default==null){
                throw new QNoDefaultValueException(7);
            }
        }
        
        AbstractSignal9Default3(Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default, Supplier<? extends I> arg9Default, Class<?>... types) {
            super(arg8Default, arg9Default, types);
            this.arg7Default = arg7Default;
            if(this.arg7Default==null){
                throw new QNoDefaultValueException(7);
            }
        }
        
        AbstractSignal9Default3(String signalName, Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default, Supplier<? extends I> arg9Default, Class<?>... types) {
            super(signalName, arg8Default, arg9Default, types);
            this.arg7Default = arg7Default;
            if(this.arg7Default==null){
                throw new QNoDefaultValueException(7);
            }
        }
        
        private final Supplier<? extends G> arg7Default;
        
        /**
         * Emits the signal with default value for arg7.
         * @param arg1
         * @param arg2
         * @param arg3
         * @param arg4
         * @param arg5
         * @param arg6
         */
        @Override
        public final void emit(A arg1, B arg2, C arg3, D arg4, E arg5, F arg6) {
            super.emitDefaultSignal(arg7Default, arg1, arg2, arg3, arg4, arg5, arg6);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends F> arg6Default, 
        		A arg1, B arg2, C arg3, D arg4, E arg5) {
        	super.emitDefaultSignal(arg6Default, arg7Default, arg1, arg2, arg3, arg4, arg5);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, 
        		A arg1, B arg2, C arg3, D arg4) {
        	super.emitDefaultSignal(arg5Default, arg6Default, arg7Default, arg1, arg2, arg3, arg4);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, 
        		Supplier<? extends F> arg6Default, A arg1, B arg2, C arg3) {
        	super.emitDefaultSignal(arg4Default, arg5Default, arg6Default, arg7Default, arg1, arg2, arg3);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, 
        		Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, A arg1, B arg2) {
        	super.emitDefaultSignal(arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg1, arg2);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, 
        		Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, A arg1) {
        	super.emitDefaultSignal(arg2Default, arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg1);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends A> arg1Default,
        		Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, 
        		Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default) {
        	super.emitDefaultSignal(arg1Default, arg2Default, arg3Default, arg4Default, arg5Default, arg6Default, arg7Default);
        }
    }
    
    /**
     * Supertype for all public default signals with nine parameters.
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
    public static abstract class AbstractSignal9Default4<A, B, C, D, E, F, G, H, I> extends AbstractSignal9Default3<A, B, C, D, E, F, G, H, I> implements  Emitable5<A,B,C,D,E>, Connectable5<A,B,C,D,E> {
    	AbstractSignal9Default4() {
    		this.arg6Default = null;
    	}
    	
		AbstractSignal9Default4(Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default, Supplier<? extends I> arg9Default){
            super(arg7Default, arg8Default, arg9Default);
            this.arg6Default = arg6Default;
            if(this.arg6Default==null){
                throw new QNoDefaultValueException(6);
            }
        }
        
        AbstractSignal9Default4(Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default, Supplier<? extends I> arg9Default, Class<?> declaringClass, boolean isDisposedSignal){
            super(arg7Default, arg8Default, arg9Default, declaringClass, isDisposedSignal);
            this.arg6Default = arg6Default;
            if(this.arg6Default==null){
                throw new QNoDefaultValueException(6);
            }
        }
        
        AbstractSignal9Default4(Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default, Supplier<? extends I> arg9Default, Class<?>... types) {
            super(arg7Default, arg8Default, arg9Default, types);
            this.arg6Default = arg6Default;
            if(this.arg6Default==null){
                throw new QNoDefaultValueException(6);
            }
        }
        
        AbstractSignal9Default4(String signalName, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default, Supplier<? extends I> arg9Default, Class<?>... types) {
            super(signalName, arg7Default, arg8Default, arg9Default, types);
            this.arg6Default = arg6Default;
            if(this.arg6Default==null){
                throw new QNoDefaultValueException(6);
            }
        }
        
        private final Supplier<? extends F> arg6Default;
        
        /**
         * Emits the signal with default value for arg6.
         * @param arg1
         * @param arg2
         * @param arg3
         * @param arg4
         * @param arg5
         */
        @Override
        public final void emit(A arg1, B arg2, C arg3, D arg4, E arg5) {
            super.emitDefaultSignal(arg6Default, arg1, arg2, arg3, arg4, arg5);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends E> arg5Default, A arg1, B arg2, C arg3, D arg4) {
        	super.emitDefaultSignal(arg5Default, arg6Default, arg1, arg2, arg3, arg4);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, A arg1, B arg2, C arg3) {
        	super.emitDefaultSignal(arg4Default, arg5Default, arg6Default, arg1, arg2, arg3);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, A arg1, B arg2) {
        	super.emitDefaultSignal(arg3Default, arg4Default, arg5Default, arg6Default, arg1, arg2);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, A arg1) {
        	super.emitDefaultSignal(arg2Default, arg3Default, arg4Default, arg5Default, arg6Default, arg1);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends A> arg1Default,
        		Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default) {
        	super.emitDefaultSignal(arg1Default, arg2Default, arg3Default, arg4Default, arg5Default, arg6Default);
        }
    }
    
    /**
     * Supertype for all public default signals with nine parameters.
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
    public static abstract class AbstractSignal9Default5<A, B, C, D, E, F, G, H, I> extends AbstractSignal9Default4<A, B, C, D, E, F, G, H, I> implements  Emitable4<A,B,C,D>, Connectable4<A,B,C,D> {        
    	AbstractSignal9Default5() {
    		this.arg5Default = null;
    	}
    	
		AbstractSignal9Default5(Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default, Supplier<? extends I> arg9Default){
            super(arg6Default, arg7Default, arg8Default, arg9Default);
            this.arg5Default = arg5Default;
            if(this.arg5Default==null){
                throw new QNoDefaultValueException(5);
            }
        }
        
        AbstractSignal9Default5(Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default, Supplier<? extends I> arg9Default, Class<?> declaringClass, boolean isDisposedSignal){
            super(arg6Default, arg7Default, arg8Default, arg9Default, declaringClass, isDisposedSignal);
            this.arg5Default = arg5Default;
            if(this.arg5Default==null){
                throw new QNoDefaultValueException(5);
            }
        }
        
        AbstractSignal9Default5(Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default, Supplier<? extends I> arg9Default, Class<?>... types) {
            super(arg6Default, arg7Default, arg8Default, arg9Default, types);
            this.arg5Default = arg5Default;
            if(this.arg5Default==null){
                throw new QNoDefaultValueException(5);
            }
        }
        
        AbstractSignal9Default5(String signalName, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default, Supplier<? extends I> arg9Default, Class<?>... types) {
            super(signalName, arg6Default, arg7Default, arg8Default, arg9Default, types);
            this.arg5Default = arg5Default;
            if(this.arg5Default==null){
                throw new QNoDefaultValueException(5);
            }
        }
        
        private final Supplier<? extends E> arg5Default;
        
        /**
         * Emits the signal with default value for arg5.
         * @param arg1
         * @param arg2
         * @param arg3
         * @param arg4
         */
        @Override
        public final void emit(A arg1, B arg2, C arg3, D arg4) {
            super.emitDefaultSignal(arg5Default, arg1, arg2, arg3, arg4);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends D> arg4Default, A arg1, B arg2, C arg3) {
        	super.emitDefaultSignal(arg4Default, arg5Default, arg1, arg2, arg3);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, A arg1, B arg2) {
        	super.emitDefaultSignal(arg3Default, arg4Default, arg5Default, arg1, arg2);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, A arg1) {
        	super.emitDefaultSignal(arg2Default, arg3Default, arg4Default, arg5Default, arg1);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends A> arg1Default,
        		Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default) {
        	super.emitDefaultSignal(arg1Default, arg2Default, arg3Default, arg4Default, arg5Default);
        }
    }
    
    /**
     * Supertype for all public default signals with nine parameters.
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
    public static abstract class AbstractSignal9Default6<A, B, C, D, E, F, G, H, I> extends AbstractSignal9Default5<A, B, C, D, E, F, G, H, I> implements  Emitable3<A,B,C>, Connectable3<A,B,C> {
    	AbstractSignal9Default6() {
    		this.arg4Default = null;
    	}
        
		AbstractSignal9Default6(Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default, Supplier<? extends I> arg9Default){
            super(arg5Default, arg6Default, arg7Default, arg8Default, arg9Default);
            this.arg4Default = arg4Default;
            if(this.arg4Default==null){
                throw new QNoDefaultValueException(4);
            }
        }
        
        AbstractSignal9Default6(Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default, Supplier<? extends I> arg9Default, Class<?> declaringClass, boolean isDisposedSignal){
            super(arg5Default, arg6Default, arg7Default, arg8Default, arg9Default, declaringClass, isDisposedSignal);
            this.arg4Default = arg4Default;
            if(this.arg4Default==null){
                throw new QNoDefaultValueException(4);
            }
        }
        
        AbstractSignal9Default6(Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default, Supplier<? extends I> arg9Default, Class<?>... types) {
            super(arg5Default, arg6Default, arg7Default, arg8Default, arg9Default, types);
            this.arg4Default = arg4Default;
            if(this.arg4Default==null){
                throw new QNoDefaultValueException(4);
            }
        }
        
        AbstractSignal9Default6(String signalName, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default, Supplier<? extends I> arg9Default, Class<?>... types) {
            super(signalName, arg5Default, arg6Default, arg7Default, arg8Default, arg9Default, types);
            this.arg4Default = arg4Default;
            if(this.arg4Default==null){
                throw new QNoDefaultValueException(4);
            }
        }
        
        private final Supplier<? extends D> arg4Default;
        
        /**
         * Emits the signal with default value for arg4.
         * @param arg1
         * @param arg2
         * @param arg3
         */
        @Override
        public final void emit(A arg1, B arg2, C arg3) {
            super.emitDefaultSignal(arg4Default, arg1, arg2, arg3);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends C> arg3Default, A arg1, B arg2) {
        	super.emitDefaultSignal(arg3Default, arg4Default, arg1, arg2);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, A arg1) {
        	super.emitDefaultSignal(arg2Default, arg3Default, arg4Default, arg1);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends A> arg1Default,
        		Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default) {
        	super.emitDefaultSignal(arg1Default, arg2Default, arg3Default, arg4Default);
        }
    }
    
    /**
     * Supertype for all public default signals with nine parameters.
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
    public static abstract class AbstractSignal9Default7<A, B, C, D, E, F, G, H, I> extends AbstractSignal9Default6<A, B, C, D, E, F, G, H, I> implements  Emitable2<A,B>, Connectable2<A,B> {
    	AbstractSignal9Default7() {
    		this.arg3Default = null;
    	}
    	
		AbstractSignal9Default7(Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default, Supplier<? extends I> arg9Default){
            super(arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, arg9Default);
            this.arg3Default = arg3Default;
            if(this.arg3Default==null){
                throw new QNoDefaultValueException(3);
            }
        }
        
        AbstractSignal9Default7(Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default, Supplier<? extends I> arg9Default, Class<?> declaringClass, boolean isDisposedSignal){
            super(arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, arg9Default, declaringClass, isDisposedSignal);
            this.arg3Default = arg3Default;
            if(this.arg3Default==null){
                throw new QNoDefaultValueException(3);
            }
        }
        
        AbstractSignal9Default7(Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default, Supplier<? extends I> arg9Default, Class<?>... types) {
            super(arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, arg9Default, types);
            this.arg3Default = arg3Default;
            if(this.arg3Default==null){
                throw new QNoDefaultValueException(3);
            }
        }
        
        AbstractSignal9Default7(String signalName, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default, Supplier<? extends I> arg9Default, Class<?>... types) {
            super(signalName, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, arg9Default, types);
            this.arg3Default = arg3Default;
            if(this.arg3Default==null){
                throw new QNoDefaultValueException(3);
            }
        }
        
        private final Supplier<? extends C> arg3Default;
        
        /**
         * Emits the signal with default value for arg3.
         * @param arg1
         * @param arg2
         */
        @Override
        public final void emit(A arg1, B arg2) {
            super.emitDefaultSignal(arg3Default, arg1, arg2);
        }

        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends B> arg2Default, A arg1) {
        	super.emitDefaultSignal(arg2Default, arg3Default, arg1);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends A> arg1Default,
        		Supplier<? extends B> arg2Default) {
        	super.emitDefaultSignal(arg1Default, arg2Default, arg3Default);
        }
    }
    
    /**
     * Supertype for all public default signals with nine parameters.
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
    public static abstract class AbstractSignal9Default8<A, B, C, D, E, F, G, H, I> extends AbstractSignal9Default7<A, B, C, D, E, F, G, H, I> implements  Emitable1<A>, Connectable1<A> {
    	AbstractSignal9Default8() {
    		this.arg2Default = null;
    	}
    	
		AbstractSignal9Default8(Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default, Supplier<? extends I> arg9Default){
            super(arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, arg9Default);
            this.arg2Default = arg2Default;
            if(this.arg2Default==null){
                throw new QNoDefaultValueException(2);
            }
        }
        
        AbstractSignal9Default8(Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default, Supplier<? extends I> arg9Default, Class<?> declaringClass, boolean isDisposedSignal){
            super(arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, arg9Default, declaringClass, isDisposedSignal);
            this.arg2Default = arg2Default;
            if(this.arg2Default==null){
                throw new QNoDefaultValueException(2);
            }
        }
        
        AbstractSignal9Default8(Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default, Supplier<? extends I> arg9Default, Class<?>... types) {
            super(arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, arg9Default, types);
            this.arg2Default = arg2Default;
            if(this.arg2Default==null){
                throw new QNoDefaultValueException(2);
            }
        }
        
        AbstractSignal9Default8(String signalName, Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default, Supplier<? extends I> arg9Default, Class<?>... types) {
            super(signalName, arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, arg9Default, types);
            this.arg2Default = arg2Default;
            if(this.arg2Default==null){
                throw new QNoDefaultValueException(2);
            }
        }
        
        private final Supplier<? extends B> arg2Default;
        
        /**
         * Emits the signal with default value for arg2.
         * @param arg1
         */
        @Override
        public final void emit(A arg1) {
            super.emitDefaultSignal(arg2Default, arg1);
        }
        
        @io.qt.QtUninvokable
        final void emitDefaultSignal(Supplier<? extends A> arg1Default) {
        	super.emitDefaultSignal(arg1Default, arg2Default);
        }
    }
    
    @QtUninvokable
    public static AbstractPrivateSignal0 findSignal(QObject sender, String name) {
        return (AbstractPrivateSignal0)findSignalImpl(sender, name);
    }
    
    @SuppressWarnings("unchecked")
    @QtUninvokable
    public static <A> AbstractPrivateSignal1<A> findSignal(QObject sender, String name, Class<A> typeA) {
        return (AbstractPrivateSignal1<A>)findSignalImpl(sender, name, typeA);
    }
    
    @SuppressWarnings("unchecked")
    @QtUninvokable
    public static <A,B> AbstractPrivateSignal2<A,B> findSignal(
            QObject sender, String name, 
            Class<A> typeA, Class<B> typeB) {
        return (AbstractPrivateSignal2<A,B>)findSignalImpl(sender, 
                name, typeA, typeB);
    }
    
    @SuppressWarnings("unchecked")
    @QtUninvokable
    public static <A,B,C> AbstractPrivateSignal3<A,B,C> findSignal(
            QObject sender, String name, 
            Class<A> typeA, Class<B> typeB, Class<C> typeC) {
        return (AbstractPrivateSignal3<A,B,C>)findSignalImpl(sender, 
                name, typeA, typeB, typeC);
    }
    
    @SuppressWarnings("unchecked")
    @QtUninvokable
    public static <A,B,C,D> AbstractPrivateSignal4<A,B,C,D> findSignal(
            QObject sender, String name, 
            Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD) {
        return (AbstractPrivateSignal4<A,B,C,D>)findSignalImpl(sender, 
                name, typeA, typeB, typeC, typeD);
    }
    
    @SuppressWarnings("unchecked")
    @QtUninvokable
    public static <A,B,C,D,E> AbstractPrivateSignal5<A,B,C,D,E> findSignal(
            QObject sender, String name, 
            Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE) {
        return (AbstractPrivateSignal5<A,B,C,D,E>)findSignalImpl(sender, 
                name, typeA, typeB, typeC, typeD, typeE);
    }
    
    @SuppressWarnings("unchecked")
    @QtUninvokable
    public static <A,B,C,D,E,F> AbstractPrivateSignal6<A,B,C,D,E,F> findSignal(
            QObject sender, String name, 
            Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Class<F> typeF) {
        return (AbstractPrivateSignal6<A,B,C,D,E,F>)findSignalImpl(sender, 
                name, typeA, typeB, typeC, typeD, typeE, typeF);
    }
    
    @SuppressWarnings("unchecked")
    @QtUninvokable
    public static <A,B,C,D,E,F,G> AbstractPrivateSignal7<A,B,C,D,E,F,G> findSignal(
            QObject sender, String name, 
            Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Class<F> typeF, Class<G> typeG) {
        return (AbstractPrivateSignal7<A,B,C,D,E,F,G>)findSignalImpl(sender, 
                name, typeA, typeB, typeC, typeD, typeE, typeF, typeG);
    }
    
    @SuppressWarnings("unchecked")
    @QtUninvokable
    public static <A,B,C,D,E,F,G,H> AbstractPrivateSignal8<A,B,C,D,E,F,G,H> findSignal(
            QObject sender, String name, 
            Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Class<F> typeF, Class<G> typeG, Class<H> typeH) {
        return (AbstractPrivateSignal8<A,B,C,D,E,F,G,H>)findSignalImpl(sender, 
                name, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH);
    }
    
    @SuppressWarnings("unchecked")
    @QtUninvokable
    public static <A,B,C,D,E,F,G,H,I> AbstractPrivateSignal9<A,B,C,D,E,F,G,H,I> findSignal(
            QObject sender, String name, 
            Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Class<F> typeF, Class<G> typeG, Class<H> typeH, Class<I> typeI) {
        return (AbstractPrivateSignal9<A,B,C,D,E,F,G,H,I>)findSignalImpl(sender, 
                name, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH, typeI);
    }
    
    @QtUninvokable
    public static AbstractSignal findSignal(QObject sender, String name, Class<?>... types){
        return findSignalImpl(sender, name, types);		
    }
    
    @QtUninvokable
    private static AbstractSignal findSignalImpl(QObject sender, String name, Class<?>... types){
    	QtJambi_LibraryUtilities.internal.checkedNativeId(Objects.requireNonNull(sender));
        return QtJambiInternal.findSignal(sender, name, types);
    }
    
    @QtUninvokable
    static QMetaObject forGadget(Object gadget) {
        if(gadget instanceof QtGadget) {
            return ((QtGadget) gadget).staticMetaObject();
        }else {
            return QMetaObject.forType(gadget.getClass());
        }
    }

    @QtUninvokable
    QtAbstractEnumerator getEnumEntry(QMetaEnum metaEnum, int value) {
        if(enumEntries!=null) {
            EnumEntries ee = enumEntries.get(enumeratorIndex(metaEnum));
            if(ee==null) {
                return null;
            }else{
                return ee.enumeratorConstantDirectory().get(value);
            }
        }
        return null;
    }
    
    @QtUninvokable
    private int enumeratorIndex(QMetaEnum metaEnum) {
        return enumeratorIndex(metaObjectPointer, QtJambi_LibraryUtilities.internal.nativeId(metaEnum));
    }
    
    @QtUninvokable
    private static native int enumeratorIndex(long metaObjectPointer, long metaEnumId);
    
    static boolean disconnectAll(QtSignalEmitterInterface sender, Object receiver) {
    	return AbstractSignal.disconnectAll(sender, receiver);
    }
}
