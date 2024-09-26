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

import static io.qt.core.QMetaMethod.*;
import static io.qt.internal.MetaTypeUtility.*;

import java.io.Serializable;
import java.lang.reflect.*;
import java.util.*;
import java.util.function.*;
import java.util.logging.*;

import io.qt.*;
import io.qt.internal.*;

/**
 * <p>Java wrapper for Qt class <code><a href="https://doc.qt.io/qt/qmetaobject.html">QMetaObject</a></code></p>
 */
public final class QMetaObject {
    static {
    	QtJambi_LibraryUtilities.initialize();
    }

	private static final char SlotPrefix = '1';
	private static final char SignalPrefix = '2';
    
    @Override
    public @NonNull String toString() {
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
    public static interface Connection extends Cloneable{
        /**
         * Returns true if the connection is valid.
         */
        public boolean isConnected();
        
        /**
         * Provides the sender of the connected signal.
         * @return sender
         */
        public @Nullable QtSignalEmitterInterface sender();
        
        /**
         * Provides the receiver of the signal-slot connection.
         * @return receiver
         */
        public @Nullable Object receiver();
        
        /**
         * Create a copy of the connection.
         * @return clone
         */
		public @NonNull Connection clone();
    }
    
    /**
     * Enum representing meta calls.
     */
    @SuppressWarnings("unused")
    public enum Call implements QtEnumerator{
        InvokeMetaMethod(0),
        ReadProperty(1),
        WriteProperty(2),
        ResetProperty(3),
        CreateInstance(QtJambi_LibraryUtilities.qtMajorVersion>5 ? 9 : 4),
        IndexOfMethod(QtJambi_LibraryUtilities.qtMajorVersion>5 ? 10 : 5),
        RegisterPropertyMetaType(QtJambi_LibraryUtilities.qtMajorVersion>5 ? 11 : 6),
        RegisterMethodArgumentMetaType(QtJambi_LibraryUtilities.qtMajorVersion>5 ? 12 : 7);
        
        private Call(int value) { this.value = value; }
        
        private final int value;
        
		public static Call resolve(int value) {
            switch (value) {
            case 0: return InvokeMetaMethod;
            case 1: return ReadProperty;
            case 2: return WriteProperty;
            case 3: return ResetProperty;
            case 4: 
                if(QtJambi_LibraryUtilities.qtMajorVersion>5)
                    return CreateInstance;
                else return QtJambi_LibraryUtilities.internal.resolveEnum(Call.class, value, "QueryPropertyDesignable");
            case 5: 
                if(QtJambi_LibraryUtilities.qtMajorVersion>5)
                    return IndexOfMethod;
                else return QtJambi_LibraryUtilities.internal.resolveEnum(Call.class, value, "QueryPropertyScriptable");
            case 6: 
                if(QtJambi_LibraryUtilities.qtMajorVersion>5)
                    return RegisterPropertyMetaType;
                else return QtJambi_LibraryUtilities.internal.resolveEnum(Call.class, value, "QueryPropertyStored");
            case 7: 
                if(QtJambi_LibraryUtilities.qtMajorVersion>5)
                    return RegisterMethodArgumentMetaType;
                else return QtJambi_LibraryUtilities.internal.resolveEnum(Call.class, value, "QueryPropertyEditable");
            case 8:
                if(QtJambi_LibraryUtilities.qtMajorVersion>5)
                    return QtJambi_LibraryUtilities.internal.resolveEnum(Call.class, value, "RegisterQPropertyObserver");
                else return QtJambi_LibraryUtilities.internal.resolveEnum(Call.class, value, "QueryPropertyUser");
            case 9: 
                if(QtJambi_LibraryUtilities.qtMajorVersion>5)
                    return QtJambi_LibraryUtilities.internal.resolveEnum(Call.class, value, "SetQPropertyBinding");
                else return CreateInstance;
            case 10: 
                if(QtJambi_LibraryUtilities.qtMajorVersion>5)
                    throw new io.qt.QNoSuchEnumValueException(value);
                else return IndexOfMethod;
            case 11: 
                if(QtJambi_LibraryUtilities.qtMajorVersion>5)
                    throw new io.qt.QNoSuchEnumValueException(value);
                else return RegisterPropertyMetaType;
            case 12: 
                if(QtJambi_LibraryUtilities.qtMajorVersion>5)
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
    public @NonNull QMetaProperty userProperty() {
        return userProperty(this.metaObjectPointer);
    }
    @QtUninvokable
    private native QMetaProperty userProperty(long metaObjectPointer);
    
    @QtUninvokable
    public @NonNull QList<@NonNull QMetaProperty> properties() {
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
    public final int propertyOffset(){
        return propertyOffset(metaObjectPointer);
    }
    @QtUninvokable
    private native int propertyOffset(long metaObjectPointer);
    
    @QtUninvokable
    public @NonNull QMetaProperty property(String name) {
        return property(this.metaObjectPointer, name);
    }
    @QtUninvokable
    private native QMetaProperty property(long metaObjectPointer, String name);
    
    @QtUninvokable
    public @NonNull QMetaProperty property(int index) {
        return propertyByIndex(this.metaObjectPointer, index);
    }
    @QtUninvokable
    private native QMetaProperty propertyByIndex(long metaObjectPointer, int index);
    
    @QtUninvokable
    public @NonNull QList<io.qt.core.@NonNull QMetaEnum> enumerators() {
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
    public final int enumeratorOffset(){
        return enumeratorOffset(metaObjectPointer);
    }
    @QtUninvokable
    private native int enumeratorOffset(long metaObjectPointer);
    
    @QtUninvokable
    public @NonNull QMetaEnum enumerator(String name) {
        return enumerator(this.metaObjectPointer, name);
    }
    @QtUninvokable
    private static native QMetaEnum enumerator(long metaObjectPointer, String name);
    
    @QtUninvokable
    public @NonNull QMetaEnum enumerator(int index) {
        return enumeratorByIndex(this.metaObjectPointer, index);
    }
    @QtUninvokable
    private static native QMetaEnum enumeratorByIndex(long metaObjectPointer, int index);
    
    @QtUninvokable
    public static void connectSlotsByName(@Nullable QObject object) {
        connectSlotsByName(QtJambi_LibraryUtilities.internal.checkedNativeId(object));
    }
    
    @QtUninvokable
    private static native void connectSlotsByName(long object);
            
    @QtUninvokable
    static native QMetaObject.@NonNull Connection connect(QObject sender, String signal, QObject receiver, String slot, byte connection);
    
    @QtUninvokable
    static native QMetaObject.Connection connectMethods(QObject sender, int signalIdx, long signalEnclosingMetaObject, QObject receiver, int slotIdx, long EnclosingMetaObject, byte connection);
    
    @QtUninvokable
    static native boolean disconnect(QObject sender, String signal, QObject receiver, String slot);
    
    @QtUninvokable
    static native boolean disconnectMethods(QObject sender, int signalIdx, long signalEnclosingMetaObject, QObject receiver, int slotIdx, long EnclosingMetaObject);
    
    static String internalNameOfType(Class<? extends Object> cls) {
    	return internalNameOfArgumentType(cls);
    }
    
    @QtUninvokable
    public final @NonNull QMetaMethod method(int methodIndex) {
        return methodByIndex(metaObjectPointer, methodIndex);
    }
    @QtUninvokable
    private native QMetaMethod methodByIndex(long metaObjectPointer, int index);
    
    @QtUninvokable
    public final @NonNull QMetaMethod method(@Nullable String name, @NonNull Class<?> @NonNull... parameterTypes) {
    	if(name==null)
    		return new QMetaMethod();
        QMetaMethod method;
        int idx = name.indexOf('(');
        if(parameterTypes.length==0 && idx>0) {
        	int spacePos = name.substring(0, idx).trim().lastIndexOf(' ');
			if (idx > spacePos && spacePos > 0)
				throw new RuntimeException(String.format("Do not specify return type in slot signature: '%1$s'", name));
        	String cppNormalizedSignature = cppNormalizedSignature(name, this.type());
            method = method(metaObjectPointer, cppNormalizedSignature);
            if(!method.isValid())
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
        if(!method.isValid()) {
            for(QMetaMethod m : methods(metaObjectPointer)) {
                if(m.name().toString().equals(name)) {
                	if(m.parameterClassTypes().equals(Arrays.asList(parameterTypes))) {
	                    method = m;
	                    break;
                	}else if(idx<0 && m.parameterClassTypes().size()==parameterTypes.length) {
                		QMetaMethod _method = null;
                		for(int i=0; i<parameterTypes.length; ++i) {
                			if(m.parameterClassTypes().get(i).isAssignableFrom(parameterTypes[i])) {
        	                    _method = m;
        	                    break;                				
                			}
                		}
                		if(_method!=null) {
                			method = _method;
                			break;
                		}
                	}
                }
            }
        }
        return method;
    }
    
    @QtUninvokable
    private native QMetaMethod method(long metaObjectPointer, String normalizedSignature);
    
    @QtUninvokable
    public @NonNull QList<@NonNull QMetaMethod> methods(){
        return methods(metaObjectPointer);
    }    
    @QtUninvokable
    private native QList<QMetaMethod> methods(long metaObjectPointer);
    
    @QtUninvokable
    public final QMetaType metaType(){
        return metaType(metaObjectPointer);
    }
    @QtUninvokable
    private native QMetaType metaType(long metaObjectPointer);
    
    @QtUninvokable
    public final int methodCount(){
        return methodCount(metaObjectPointer);
    }
    @QtUninvokable
    private native int methodCount(long metaObjectPointer);
    
    @QtUninvokable
    public final int methodOffset(){
        return methodOffset(metaObjectPointer);
    }
    @QtUninvokable
    private native int methodOffset(long metaObjectPointer);
    
    @QtUninvokable
    public @NonNull QMetaMethod constructor(Class<?>... parameterTypes) {
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
        if(!method.isValid()) {
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
    public @NonNull QMetaMethod constructor(int index) {
        return constructorByIndex(metaObjectPointer, index);
    }
    @QtUninvokable
    private native QMetaMethod constructorByIndex(long metaObjectPointer, int index);
    
    @QtUninvokable
    public final @NonNull QList<@NonNull QMetaMethod> constructors(){
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
    public @Nullable QObject newInstance(Object... args) throws NoSuchMethodException {
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
        if(constr==null || !constr.isValid()) {
            StringBuilder argsStrg = new StringBuilder();
            for(int i=0; i<args.length; ++i) {
                if(i!=0)
                    argsStrg.append(',');
                argsStrg.append(args[i]==null ? Object.class.getName() : args[i].getClass().getName());
            }
            throw new NoSuchMethodException(String.format("Constructor %1$s(%2$s) not available.", this.className(), argsStrg));
        }
        return newInstanceImpl(QtJambi_LibraryUtilities.internal.nativeId(constr), args);
    }
    
    @QtUninvokable
    private static native QObject newInstanceImpl(long metaMethodId, Object[] args);
    
    @QtUninvokable
    public @Nullable QMetaObject superClass(){
        return superClass(metaObjectPointer);
    }
    @QtUninvokable
    private static native QMetaObject superClass(long metaObjectPointer);
    
    @QtUninvokable
    public @NonNull String className(){
        return className(metaObjectPointer);
    }
    @QtUninvokable
    private static native String className(long metaObjectPointer);
    
    @QtUninvokable
    public @NonNull QList<@NonNull QPair<@NonNull String,@NonNull String>> classInfos(){
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
    public @Nullable String classInfo(@Nullable String name){
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
    public boolean inherits(@Nullable QMetaObject metaObject){
        return metaObject!=null && inherits(metaObjectPointer, metaObject==null ? 0 : metaObject.metaObjectPointer);
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
    public static <T extends QtObjectInterface> T cast(@StrictNonNull Class<T> targetType, QtObjectInterface object) {
        io.qt.QtUtilities.initializePackage(targetType);
        if(object==null || targetType.isInstance(object)) {
            return targetType.cast(object);
        }
        return targetType.cast(cast(object, targetType));
    }
    @QtUninvokable
    private static native Object cast(QtObjectInterface object, Class<?> targetType);
    
    @QtUninvokable
    public @Nullable Class<?> type(){
        return type(metaObjectPointer);
    }
    @QtUninvokable
    private static native Class<?> type(long metaObjectPointer);

    @QtUninvokable
    public static native @Nullable QMetaObject forType(@Nullable Class<?> clazz);
        
    @QtUninvokable
    private static native Class<?> exactType(long metaObjectPointer);
                    
    @QtUninvokable
    public static boolean checkConnectArgs(@NonNull String signal, @NonNull String method) {
        return checkConnectArgsString(SignalPrefix+cppNormalizedSignature(signal, null), SlotPrefix+cppNormalizedSignature(method, null));
    }
    @QtUninvokable
    private native static boolean checkConnectArgsString(String signal, String method);
    
    @QtUninvokable
    public static boolean checkConnectArgs(@NonNull QMetaMethod signal, @NonNull QMetaMethod method){
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
    public static Object invokeMethod(@StrictNonNull QObject obj, @StrictNonNull String member, Object... args) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
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
    public static Object invokeMethod(@StrictNonNull QObject obj, @StrictNonNull String member, Qt.@NonNull ConnectionType type, Object... args) throws QUnsuccessfulInvocationException, QNoSuchMethodException {
        Class<?>[] parameterTypes;
        if(member.contains("(")) {
            parameterTypes = new Class<?>[0];
        }else {
            parameterTypes = new Class<?>[args.length];
            for (int i = 0; i < parameterTypes.length; i++) {
                if(args[i]==null) {
                    parameterTypes[i] = Object.class;
                }else {
                    if(args[i].getClass().isSynthetic()) {
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
    public static <R> R invokeMethod(@StrictNonNull Method0<R> method) throws QUnsuccessfulInvocationException {
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
     * @param type synchronous or asynchronous invocation
     * @return method result value - if the invocation is asynchronous, the return value cannot be evaluated.
     * @throws QUnsuccessfulInvocationException if not able to invoke slot
     */
    @SuppressWarnings("unchecked")
    @QtUninvokable
    public static <R> R invokeMethod(@StrictNonNull Method0<R> method, Qt.@NonNull ConnectionType type) throws QUnsuccessfulInvocationException {
        ClassAnalyzerUtility.LambdaInfo info = ClassAnalyzerUtility.lambdaInfo(method);
        QObject context;
        QThread thread = null;
        if(info!=null && info.qobject!=null) {
        	context = info.qobject;
        	if(info.reflectiveMethod!=null) {
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
        	thread = context.thread();
        }else {
            if(method instanceof QtObjectInterface) {
                if( ((QtObjectInterface)method).isDisposed() )
                    return null;
            }
        	if(method instanceof QtThreadAffineInterface) {
        		thread = ((QtThreadAffineInterface) method).thread();
                if(thread!=null) {
                	if(thread.thread()==thread) {
                		context = thread;
                	}else if(QCoreApplication.instance()!=null && thread==QCoreApplication.instance().thread()){
                		context = QCoreApplication.instance();
                	}else {
                		context = null;
                	}
                }else {
                	context = null;
                }
        	}else {
        		context = QObject.getQPropertyOwner(info);
        		if(context!=null)
        			thread = context.thread();
        	}
        }
        return invokeMethod(thread, context, method, type);
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
    public static <A,R> R invokeMethod(@StrictNonNull Method1<A,R> method, A arg1) throws QUnsuccessfulInvocationException {
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
     * @param type synchronous or asynchronous invocation
     * @param arg1 Argument for the first parameter.
     * @return method result value - if the invocation is asynchronous, the return value cannot be evaluated.
     * @throws QUnsuccessfulInvocationException if not able to invoke slot
     */
    @SuppressWarnings("unchecked")
    @QtUninvokable
    public static <A,R> R invokeMethod(@StrictNonNull Method1<A,R> method, Qt.@NonNull ConnectionType type, A arg1) throws QUnsuccessfulInvocationException {
        ClassAnalyzerUtility.LambdaInfo info = ClassAnalyzerUtility.lambdaInfo(method);
        QObject context;
        QThread thread = null;
        if(info!=null && info.qobject!=null) {
        	context = info.qobject;
        	if(info.reflectiveMethod!=null) {
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
        	thread = context.thread();
        }else {
            if(method instanceof QtObjectInterface) {
                if( ((QtObjectInterface)method).isDisposed() )
                    return null;
            }
        	if(method instanceof QtThreadAffineInterface) {
        		thread = ((QtThreadAffineInterface) method).thread();
                if(thread!=null) {
                	if(thread.thread()==thread) {
                		context = thread;
                	}else if(QCoreApplication.instance()!=null && thread==QCoreApplication.instance().thread()){
                		context = QCoreApplication.instance();
                	}else {
                		context = null;
                	}
                }else {
                	context = null;
                }
        	}else {
        		context = QObject.getQPropertyOwner(info);
        		if(context!=null)
        			thread = context.thread();
        	}
        }
        return invokeMethod(thread, context, method, type, arg1);
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
    public static <A,B,R> R invokeMethod(@StrictNonNull Method2<A,B,R> method, A arg1, B arg2) throws QUnsuccessfulInvocationException {
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
     * @param type synchronous or asynchronous invocation
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @return method result value - if the invocation is asynchronous, the return value cannot be evaluated.
     * @throws QUnsuccessfulInvocationException if not able to invoke slot
     */
    @SuppressWarnings("unchecked")
    @QtUninvokable
    public static <A,B,R> R invokeMethod(@StrictNonNull Method2<A,B,R> method, Qt.@NonNull ConnectionType type, A arg1, B arg2) throws QUnsuccessfulInvocationException {
        ClassAnalyzerUtility.LambdaInfo info = ClassAnalyzerUtility.lambdaInfo(method);
        QObject context;
        QThread thread = null;
        if(info!=null && info.qobject!=null) {
        	context = info.qobject;
        	if(info.reflectiveMethod!=null) {
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
        	thread = context.thread();
        }else {
        	if(method instanceof QtObjectInterface) {
                if( ((QtObjectInterface)method).isDisposed() )
                    return null;
            }
        	if(method instanceof QtThreadAffineInterface)
        		thread = ((QtThreadAffineInterface) method).thread();
            if(thread!=null) {
            	if(thread.thread()==thread) {
            		context = thread;
            	}else if(QCoreApplication.instance()!=null && thread==QCoreApplication.instance().thread()){
            		context = QCoreApplication.instance();
            	}else {
            		context = null;
            	}
            }else {
            	context = null;
            }
        }
        return invokeMethod(thread, context, method, type, arg1, arg2);
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
    public static <A,B,C,R> R invokeMethod(@StrictNonNull Method3<A,B,C,R> method, A arg1, B arg2, C arg3) throws QUnsuccessfulInvocationException {
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
     * @param type synchronous or asynchronous invocation
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @return method result value - if the invocation is asynchronous, the return value cannot be evaluated.
     * @throws QUnsuccessfulInvocationException if not able to invoke slot
     */
    @SuppressWarnings("unchecked")
    @QtUninvokable
    public static <A,B,C,R> R invokeMethod(@StrictNonNull Method3<A,B,C,R> method, Qt.@NonNull ConnectionType type, A arg1, B arg2, C arg3) throws QUnsuccessfulInvocationException {
        ClassAnalyzerUtility.LambdaInfo info = ClassAnalyzerUtility.lambdaInfo(method);
        QObject context;
        QThread thread = null;
        if(info!=null && info.qobject!=null) {
        	context = info.qobject;
        	if(info.reflectiveMethod!=null) {
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
        	thread = context.thread();
        }else {
            if(method instanceof QtObjectInterface) {
                if( ((QtObjectInterface)method).isDisposed() )
                    return null;
            }
        	if(method instanceof QtThreadAffineInterface)
        		thread = ((QtThreadAffineInterface) method).thread();
            if(thread!=null) {
            	if(thread.thread()==thread) {
            		context = thread;
            	}else if(QCoreApplication.instance()!=null && thread==QCoreApplication.instance().thread()){
            		context = QCoreApplication.instance();
            	}else {
            		context = null;
            	}
            }else {
            	context = null;
            }
        }
        return invokeMethod(thread, context, method, type, arg1, arg2, arg3);
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
    public static <A,B,C,D,R> R invokeMethod(@StrictNonNull Method4<A,B,C,D,R> method, A arg1, B arg2, C arg3, D arg4) throws QUnsuccessfulInvocationException {
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
     * @param type synchronous or asynchronous invocation
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @param arg4 Argument for the fourth parameter.
     * @return method result value - if the invocation is asynchronous, the return value cannot be evaluated.
     * @throws QUnsuccessfulInvocationException if not able to invoke slot
     */
    @SuppressWarnings("unchecked")
    @QtUninvokable
    public static <A,B,C,D,R> R invokeMethod(@StrictNonNull Method4<A,B,C,D,R> method, Qt.@NonNull ConnectionType type, A arg1, B arg2, C arg3, D arg4) throws QUnsuccessfulInvocationException {
        ClassAnalyzerUtility.LambdaInfo info = ClassAnalyzerUtility.lambdaInfo(method);
        QObject context;
        QThread thread = null;
        if(info!=null && info.qobject!=null) {
        	context = info.qobject;
        	if(info.reflectiveMethod!=null) {
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
        	thread = context.thread();
        }else {
            if(method instanceof QtObjectInterface) {
                if( ((QtObjectInterface)method).isDisposed() )
                    return null;
            }
        	if(method instanceof QtThreadAffineInterface)
        		thread = ((QtThreadAffineInterface) method).thread();
            if(thread!=null) {
            	if(thread.thread()==thread) {
            		context = thread;
            	}else if(QCoreApplication.instance()!=null && thread==QCoreApplication.instance().thread()){
            		context = QCoreApplication.instance();
            	}else {
            		context = null;
            	}
            }else {
            	context = null;
            }
        }
        return invokeMethod(thread, context, method, type, arg1, arg2, arg3, arg4);
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
    public static <A,B,C,D,E,R> R invokeMethod(@StrictNonNull Method5<A,B,C,D,E,R> method, A arg1, B arg2, C arg3, D arg4, E arg5) throws QUnsuccessfulInvocationException {
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
     * @param type synchronous or asynchronous invocation
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
    public static <A,B,C,D,E,R> R invokeMethod(@StrictNonNull Method5<A,B,C,D,E,R> method, Qt.@NonNull ConnectionType type, A arg1, B arg2, C arg3, D arg4, E arg5) throws QUnsuccessfulInvocationException {
        ClassAnalyzerUtility.LambdaInfo info = ClassAnalyzerUtility.lambdaInfo(method);
        QObject context;
        QThread thread = null;
        if(info!=null && info.qobject!=null) {
        	context = info.qobject;
        	if(info.reflectiveMethod!=null) {
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
	            if(qmethod!=null && qmethod.isValid()) {
	                return (R)qmethod.invoke(info.qobject, type, arg1, arg2, arg3, arg4, arg5);
	            }
        	}
        	thread = context.thread();
        }else {
            if(method instanceof QtObjectInterface) {
                if( ((QtObjectInterface)method).isDisposed() )
                    return null;
            }
        	if(method instanceof QtThreadAffineInterface)
        		thread = ((QtThreadAffineInterface) method).thread();
            if(thread!=null) {
            	if(thread.thread()==thread) {
            		context = thread;
            	}else if(QCoreApplication.instance()!=null && thread==QCoreApplication.instance().thread()){
            		context = QCoreApplication.instance();
            	}else {
            		context = null;
            	}
            }else {
            	context = null;
            }
        }
        return invokeMethod(thread, context, method, type, arg1, arg2, arg3, arg4, arg5);
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
    public static <A,B,C,D,E,F,R> R invokeMethod(@StrictNonNull Method6<A,B,C,D,E,F,R> method, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6) throws QUnsuccessfulInvocationException {
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
     * @param type synchronous or asynchronous invocation
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
    public static <A,B,C,D,E,F,R> R invokeMethod(@StrictNonNull Method6<A,B,C,D,E,F,R> method, Qt.@NonNull ConnectionType type, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6) throws QUnsuccessfulInvocationException {
        ClassAnalyzerUtility.LambdaInfo info = ClassAnalyzerUtility.lambdaInfo(method);
        QObject context;
        QThread thread = null;
        if(info!=null && info.qobject!=null) {
        	context = info.qobject;
        	if(info.reflectiveMethod!=null) {
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
        	thread = context.thread();
        }else {
            if(method instanceof QtObjectInterface) {
                if( ((QtObjectInterface)method).isDisposed() )
                    return null;
            }
        	if(method instanceof QtThreadAffineInterface)
        		thread = ((QtThreadAffineInterface) method).thread();
            if(thread!=null) {
            	if(thread.thread()==thread) {
            		context = thread;
            	}else if(QCoreApplication.instance()!=null && thread==QCoreApplication.instance().thread()){
            		context = QCoreApplication.instance();
            	}else {
            		context = null;
            	}
            }else {
            	context = null;
            }
        }
        return invokeMethod(thread, context, method, type, arg1, arg2, arg3, arg4, arg5, arg6);
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
    public static <A,B,C,D,E,F,G,R> R invokeMethod(@StrictNonNull Method7<A,B,C,D,E,F,G,R> method, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7) throws QUnsuccessfulInvocationException {
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
     * @param type synchronous or asynchronous invocation
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
    public static <A,B,C,D,E,F,G,R> R invokeMethod(@StrictNonNull Method7<A,B,C,D,E,F,G,R> method, Qt.@NonNull ConnectionType type, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7) throws QUnsuccessfulInvocationException {
        ClassAnalyzerUtility.LambdaInfo info = ClassAnalyzerUtility.lambdaInfo(method);
        QObject context;
        QThread thread = null;
        if(info!=null && info.qobject!=null) {
        	context = info.qobject;
        	if(info.reflectiveMethod!=null) {
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
        	thread = context.thread();
        }else {
            if(method instanceof QtObjectInterface) {
                if( ((QtObjectInterface)method).isDisposed() )
                    return null;
            }
        	if(method instanceof QtThreadAffineInterface)
        		thread = ((QtThreadAffineInterface) method).thread();
            if(thread!=null) {
            	if(thread.thread()==thread) {
            		context = thread;
            	}else if(QCoreApplication.instance()!=null && thread==QCoreApplication.instance().thread()){
            		context = QCoreApplication.instance();
            	}else {
            		context = null;
            	}
            }else {
            	context = null;
            }
        }
        return invokeMethod(thread, context, method, type, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
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
    public static <A,B,C,D,E,F,G,H,R> R invokeMethod(@StrictNonNull Method8<A,B,C,D,E,F,G,H,R> method, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7, H arg8) throws QUnsuccessfulInvocationException {
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
     * @param type synchronous or asynchronous invocation
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
    public static <A,B,C,D,E,F,G,H,R> R invokeMethod(@StrictNonNull Method8<A,B,C,D,E,F,G,H,R> method, Qt.@NonNull ConnectionType type, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7, H arg8) throws QUnsuccessfulInvocationException {
        ClassAnalyzerUtility.LambdaInfo info = ClassAnalyzerUtility.lambdaInfo(method);
        QObject context;
        QThread thread = null;
        if(info!=null && info.qobject!=null) {
        	context = info.qobject;
        	if(info.reflectiveMethod!=null) {
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
        	thread = context.thread();
        }else {
            if(method instanceof QtObjectInterface) {
                if( ((QtObjectInterface)method).isDisposed() )
                    return null;
            }
        	if(method instanceof QtThreadAffineInterface)
        		thread = ((QtThreadAffineInterface) method).thread();
            if(thread!=null) {
            	if(thread.thread()==thread) {
            		context = thread;
            	}else if(QCoreApplication.instance()!=null && thread==QCoreApplication.instance().thread()){
            		context = QCoreApplication.instance();
            	}else {
            		context = null;
            	}
            }else {
            	context = null;
            }
        }
        return invokeMethod(thread, context, method, type, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
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
    public static <A,B,C,D,E,F,G,H,I,R> R invokeMethod(@StrictNonNull Method9<A,B,C,D,E,F,G,H,I,R> method, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7, H arg8, I arg9) throws QUnsuccessfulInvocationException {
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
     * @param type synchronous or asynchronous invocation
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
    public static <A,B,C,D,E,F,G,H,I,R> R invokeMethod(@StrictNonNull Method9<A,B,C,D,E,F,G,H,I,R> method, Qt.@NonNull ConnectionType type, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7, H arg8, I arg9) throws QUnsuccessfulInvocationException {
        ClassAnalyzerUtility.LambdaInfo info = ClassAnalyzerUtility.lambdaInfo(method);
        QObject context;
        QThread thread = null;
        if(info!=null && info.qobject!=null) {
        	context = info.qobject;
        	if(info.reflectiveMethod!=null) {
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
        	thread = context.thread();
        }else {
            if(method instanceof QtObjectInterface) {
                if( ((QtObjectInterface)method).isDisposed() )
                    return null;
            }
        	if(method instanceof QtThreadAffineInterface)
        		thread = ((QtThreadAffineInterface) method).thread();
            if(thread!=null) {
            	if(thread.thread()==thread) {
            		context = thread;
            	}else if(QCoreApplication.instance()!=null && thread==QCoreApplication.instance().thread()){
            		context = QCoreApplication.instance();
            	}else {
            		context = null;
            	}
            }else {
            	context = null;
            }
        }
        return invokeMethod(thread, context, method, type, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
    }
    
    /**
     * Calling <code>invokeMethod(method, AutoConnection)</code>.
     * @param method invoked method
     * @throws QUnsuccessfulInvocationException if not able to invoke slot
     */
    @QtUninvokable
    public static void invokeMethod(@StrictNonNull Slot0 method) throws QUnsuccessfulInvocationException {
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
     * @param type synchronous or asynchronous invocation
     * @throws QUnsuccessfulInvocationException if not able to invoke slot
     */
    @QtUninvokable
    public static void invokeMethod(@StrictNonNull Slot0 method, Qt.@NonNull ConnectionType type) throws QUnsuccessfulInvocationException {
        ClassAnalyzerUtility.LambdaInfo info = ClassAnalyzerUtility.lambdaInfo(method);
        QThread thread = null;
        QObject context;
        if(info!=null && info.qobject!=null) {
        	context = info.qobject;
            if(info.reflectiveMethod!=null) {
                QMetaMethod qmethod = fromReflectedMethod(info.reflectiveMethod);
                if(qmethod!=null && qmethod.isValid() && qmethod.parameterTypes().size()==info.lambdaArgs.size()) {
                    qmethod.invoke(info.qobject, type, info.lambdaArgs.toArray());
                    return;
                }
            }
            thread = context.thread();
        }else {
            if(method instanceof QtObjectInterface) {
                if( ((QtObjectInterface)method).isDisposed() )
                    return;
            }
        	if(method instanceof QtThreadAffineInterface) {
        		thread = ((QtThreadAffineInterface) method).thread();
                if(thread!=null) {
                	if(thread.thread()==thread) {
                		context = thread;
                	}else if(QCoreApplication.instance()!=null && thread==QCoreApplication.instance().thread()){
                		context = QCoreApplication.instance();
                	}else {
                		context = null;
                	}
                }else {
                	context = null;
                }
        	}else {
        		context = QObject.getQPropertyOwner(info);
        		if(context!=null)
        			thread = context.thread();
        	}
        }
        invokeMethod(thread, context, method, type);
    }
    
    /**
     * Calling <code>invokeMethod(method, AutoConnection, ...)</code>.
     * @param <A> The type of the first parameter of the slot.
     * @param method invoked method
     * @param arg1 Argument for the first parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke slot
     */
    @QtUninvokable
    public static <A> void invokeMethod(@StrictNonNull Slot1<A> method, A arg1) throws QUnsuccessfulInvocationException {
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
     * @param type synchronous or asynchronous invocation
     * @param arg1 Argument for the first parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke slot
     */
    @QtUninvokable
    public static <A> void invokeMethod(@StrictNonNull Slot1<A> method, Qt.@NonNull ConnectionType type, A arg1) throws QUnsuccessfulInvocationException {
        ClassAnalyzerUtility.LambdaInfo info = ClassAnalyzerUtility.lambdaInfo(method);
        QThread thread = null;
        QObject context;
        if(info!=null && info.qobject!=null) {
        	context = info.qobject;
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
            thread = context.thread();
        }else {
            if(method instanceof QtObjectInterface) {
                if( ((QtObjectInterface)method).isDisposed() )
                    return;
            }
        	if(method instanceof QtThreadAffineInterface) {
        		thread = ((QtThreadAffineInterface) method).thread();
                if(thread!=null) {
                	if(thread.thread()==thread) {
                		context = thread;
                	}else if(QCoreApplication.instance()!=null && thread==QCoreApplication.instance().thread()){
                		context = QCoreApplication.instance();
                	}else {
                		context = null;
                	}
                }else {
                	context = null;
                }
        	}else {
        		context = QObject.getQPropertyOwner(info);
        		if(context!=null)
        			thread = context.thread();
        	}
        }
        invokeMethod(thread, context, method, type, arg1);
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
    public static <A,B> void invokeMethod(@StrictNonNull Slot2<A,B> method, A arg1, B arg2) throws QUnsuccessfulInvocationException {
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
     * @param type synchronous or asynchronous invocation
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke slot
     */
    @QtUninvokable
    public static <A,B> void invokeMethod(@StrictNonNull Slot2<A,B> method, Qt.@NonNull ConnectionType type, A arg1, B arg2) throws QUnsuccessfulInvocationException {
        ClassAnalyzerUtility.LambdaInfo info = ClassAnalyzerUtility.lambdaInfo(method);
        QThread thread = null;
        QObject context;
        if(info!=null && info.qobject!=null) {
        	context = info.qobject;
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
                if(qmethod!=null && qmethod.isValid()) {
                    qmethod.invoke(info.qobject, type, arg1, arg2);
                    return;
                }
            }
            thread = context.thread();
        }else {
            if(method instanceof QtObjectInterface) {
                if( ((QtObjectInterface)method).isDisposed() )
                    return;
            }
        	if(method instanceof QtThreadAffineInterface) {
        		thread = ((QtThreadAffineInterface) method).thread();
                if(thread!=null) {
                	if(thread.thread()==thread) {
                		context = thread;
                	}else if(QCoreApplication.instance()!=null && thread==QCoreApplication.instance().thread()){
                		context = QCoreApplication.instance();
                	}else {
                		context = null;
                	}
                }else {
                	context = null;
                }
        	}else {
        		context = QObject.getQPropertyOwner(info);
        		if(context!=null)
        			thread = context.thread();
        	}
        }
        invokeMethod(thread, context, method, type, arg1, arg2);
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
    public static <A,B,C> void invokeMethod(@StrictNonNull Slot3<A,B,C> method, A arg1, B arg2, C arg3) throws QUnsuccessfulInvocationException {
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
     * @param type synchronous or asynchronous invocation
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke slot
     */
    @QtUninvokable
    public static <A,B,C> void invokeMethod(@StrictNonNull Slot3<A,B,C> method, Qt.@NonNull ConnectionType type, A arg1, B arg2, C arg3) throws QUnsuccessfulInvocationException {
        ClassAnalyzerUtility.LambdaInfo info = ClassAnalyzerUtility.lambdaInfo(method);
        QThread thread = null;
        QObject context;
        if(info!=null && info.qobject!=null) {
        	context = info.qobject;
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
            thread = context.thread();
        }else {
            if(method instanceof QtObjectInterface) {
                if( ((QtObjectInterface)method).isDisposed() )
                    return;
            }
            if(method instanceof QtThreadAffineInterface)
                thread = ((QtThreadAffineInterface) method).thread();
            if(thread!=null) {
            	if(thread.thread()==thread) {
            		context = thread;
            	}else if(QCoreApplication.instance()!=null && thread==QCoreApplication.instance().thread()){
            		context = QCoreApplication.instance();
            	}else {
            		context = null;
            	}
            }else {
            	context = null;
            }
        }
        invokeMethod(thread, context, method, type, arg1, arg2, arg3);
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
    public static <A,B,C,D> void invokeMethod(@StrictNonNull Slot4<A,B,C,D> method, A arg1, B arg2, C arg3, D arg4) throws QUnsuccessfulInvocationException {
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
     * @param type synchronous or asynchronous invocation
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @param arg4 Argument for the fourth parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke slot
     */
    @QtUninvokable
    public static <A,B,C,D> void invokeMethod(@StrictNonNull Slot4<A,B,C,D> method, Qt.@NonNull ConnectionType type, A arg1, B arg2, C arg3, D arg4) throws QUnsuccessfulInvocationException {
        ClassAnalyzerUtility.LambdaInfo info = ClassAnalyzerUtility.lambdaInfo(method);
        QThread thread = null;
        QObject context;
        if(info!=null && info.qobject!=null) {
        	context = info.qobject;
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
            thread = context.thread();
        }else {
            if(method instanceof QtObjectInterface) {
                if( ((QtObjectInterface)method).isDisposed() )
                    return;
            }
            if(method instanceof QtThreadAffineInterface)
                thread = ((QtThreadAffineInterface) method).thread();
            if(thread!=null) {
            	if(thread.thread()==thread) {
            		context = thread;
            	}else if(QCoreApplication.instance()!=null && thread==QCoreApplication.instance().thread()){
            		context = QCoreApplication.instance();
            	}else {
            		context = null;
            	}
            }else {
            	context = null;
            }
        }
        invokeMethod(thread, context, method, type, arg1, arg2, arg3, arg4);
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
    public static <A,B,C,D,E> void invokeMethod(@StrictNonNull Slot5<A,B,C,D,E> method, A arg1, B arg2, C arg3, D arg4, E arg5) throws QUnsuccessfulInvocationException {
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
     * @param type synchronous or asynchronous invocation
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @param arg4 Argument for the fourth parameter.
     * @param arg5 Argument for the fifth parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke slot
     */
    @QtUninvokable
    public static <A,B,C,D,E> void invokeMethod(@StrictNonNull Slot5<A,B,C,D,E> method, Qt.@NonNull ConnectionType type, A arg1, B arg2, C arg3, D arg4, E arg5) throws QUnsuccessfulInvocationException {
        ClassAnalyzerUtility.LambdaInfo info = ClassAnalyzerUtility.lambdaInfo(method);
        QThread thread = null;
        QObject context;
        if(info!=null && info.qobject!=null) {
        	context = info.qobject;
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
            thread = context.thread();
        }else {
            if(method instanceof QtObjectInterface) {
                if( ((QtObjectInterface)method).isDisposed() )
                    return;
            }
            if(method instanceof QtThreadAffineInterface)
                thread = ((QtThreadAffineInterface) method).thread();
            if(thread!=null) {
            	if(thread.thread()==thread) {
            		context = thread;
            	}else if(QCoreApplication.instance()!=null && thread==QCoreApplication.instance().thread()){
            		context = QCoreApplication.instance();
            	}else {
            		context = null;
            	}
            }else {
            	context = null;
            }
        }
        invokeMethod(thread, context, method, type, arg1, arg2, arg3, arg4, arg5);
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
    public static <A,B,C,D,E,F> void invokeMethod(@StrictNonNull Slot6<A,B,C,D,E,F> method, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6) throws QUnsuccessfulInvocationException {
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
     * @param type synchronous or asynchronous invocation
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @param arg4 Argument for the fourth parameter.
     * @param arg5 Argument for the fifth parameter.
     * @param arg6 Argument for the sixth parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke slot
     */
    @QtUninvokable
    public static <A,B,C,D,E,F> void invokeMethod(@StrictNonNull Slot6<A,B,C,D,E,F> method, Qt.@NonNull ConnectionType type, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6) throws QUnsuccessfulInvocationException {
        ClassAnalyzerUtility.LambdaInfo info = ClassAnalyzerUtility.lambdaInfo(method);
        QThread thread = null;
        QObject context;
        if(info!=null && info.qobject!=null) {
        	context = info.qobject;
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
            thread = context.thread();
        }else {
            if(method instanceof QtObjectInterface) {
                if( ((QtObjectInterface)method).isDisposed() )
                    return;
            }
            if(method instanceof QtThreadAffineInterface)
                thread = ((QtThreadAffineInterface) method).thread();
            if(thread!=null) {
            	if(thread.thread()==thread) {
            		context = thread;
            	}else if(QCoreApplication.instance()!=null && thread==QCoreApplication.instance().thread()){
            		context = QCoreApplication.instance();
            	}else {
            		context = null;
            	}
            }else {
            	context = null;
            }
        }
        invokeMethod(thread, context, method, type, arg1, arg2, arg3, arg4, arg5, arg6);
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
    public static <A,B,C,D,E,F,G> void invokeMethod(@StrictNonNull Slot7<A,B,C,D,E,F,G> method, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7) throws QUnsuccessfulInvocationException {
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
     * @param type synchronous or asynchronous invocation
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
    public static <A,B,C,D,E,F,G> void invokeMethod(@StrictNonNull Slot7<A,B,C,D,E,F,G> method, Qt.@NonNull ConnectionType type, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7) throws QUnsuccessfulInvocationException {
        ClassAnalyzerUtility.LambdaInfo info = ClassAnalyzerUtility.lambdaInfo(method);
        QThread thread = null;
        QObject context;
        if(info!=null && info.qobject!=null) {
        	context = info.qobject;
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
            thread = context.thread();
        }else {
            if(method instanceof QtObjectInterface) {
                if( ((QtObjectInterface)method).isDisposed() )
                    return;
            }
            if(method instanceof QtThreadAffineInterface)
                thread = ((QtThreadAffineInterface) method).thread();
            if(thread!=null) {
            	if(thread.thread()==thread) {
            		context = thread;
            	}else if(QCoreApplication.instance()!=null && thread==QCoreApplication.instance().thread()){
            		context = QCoreApplication.instance();
            	}else {
            		context = null;
            	}
            }else {
            	context = null;
            }
        }
        invokeMethod(thread, context, method, type, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
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
    public static <A,B,C,D,E,F,G,H> void invokeMethod(@StrictNonNull Slot8<A,B,C,D,E,F,G,H> method, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7, H arg8) throws QUnsuccessfulInvocationException {
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
     * @param type synchronous or asynchronous invocation
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
    public static <A,B,C,D,E,F,G,H> void invokeMethod(@StrictNonNull Slot8<A,B,C,D,E,F,G,H> method, Qt.@NonNull ConnectionType type, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7, H arg8) throws QUnsuccessfulInvocationException {
        ClassAnalyzerUtility.LambdaInfo info = ClassAnalyzerUtility.lambdaInfo(method);
        QThread thread = null;
        QObject context;
        if(info!=null && info.qobject!=null) {
        	context = info.qobject;
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
            thread = context.thread();
        }else {
            if(method instanceof QtObjectInterface) {
                if( ((QtObjectInterface)method).isDisposed() )
                    return;
            }
            if(method instanceof QtThreadAffineInterface)
                thread = ((QtThreadAffineInterface) method).thread();
            if(thread!=null) {
            	if(thread.thread()==thread) {
            		context = thread;
            	}else if(QCoreApplication.instance()!=null && thread==QCoreApplication.instance().thread()){
            		context = QCoreApplication.instance();
            	}else {
            		context = null;
            	}
            }else {
            	context = null;
            }
        }
        invokeMethod(thread, context, method, type, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
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
    public static <A,B,C,D,E,F,G,H,I> void invokeMethod(@StrictNonNull Slot9<A,B,C,D,E,F,G,H,I> method, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7, H arg8, I arg9) throws QUnsuccessfulInvocationException {
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
     * @param type synchronous or asynchronous invocation
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
    public static <A,B,C,D,E,F,G,H,I> void invokeMethod(@StrictNonNull Slot9<A,B,C,D,E,F,G,H,I> method, Qt.@NonNull ConnectionType type, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7, H arg8, I arg9) throws QUnsuccessfulInvocationException {
        ClassAnalyzerUtility.LambdaInfo info = ClassAnalyzerUtility.lambdaInfo(method);
        QThread thread = null;
        QObject context;
        if(info!=null && info.qobject!=null) {
        	context = info.qobject;
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
            thread = context.thread();
        }else {
            if(method instanceof QtObjectInterface) {
                if( ((QtObjectInterface)method).isDisposed() )
                    return;
            }
            if(method instanceof QtThreadAffineInterface)
                thread = ((QtThreadAffineInterface) method).thread();
            if(thread!=null) {
            	if(thread.thread()==thread) {
            		context = thread;
            	}else if(QCoreApplication.instance()!=null && thread==QCoreApplication.instance().thread()){
            		context = QCoreApplication.instance();
            	}else {
            		context = null;
            	}
            }else {
            	context = null;
            }
        }
        invokeMethod(thread, context, method, type, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
    }
    
    /**
     * Calling <code>invokeMethod(method, AutoConnection)</code>.
     * 
     * @param <R> The return type of the method.
     * @param context the context for invocation
     * @param method invoked method
     * @return method result value - if the invocation is asynchronous, the return value cannot be evaluated.
     * @throws QUnsuccessfulInvocationException if not able to invoke slot
     */
    @QtUninvokable
    public static <R> R invokeMethod(@Nullable QObject context, @StrictNonNull Method0<R> method) throws QUnsuccessfulInvocationException {
        return invokeMethod(context!=null ? context.thread() : null, context, method, Qt.ConnectionType.AutoConnection);
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
     * @param context the context for invocation
     * @param method invoked method
     * @param type synchronous or asynchronous invocation
     * @return method result value - if the invocation is asynchronous, the return value cannot be evaluated.
     * @throws QUnsuccessfulInvocationException if not able to invoke slot
     */
    @QtUninvokable
    public static <R> R invokeMethod(@Nullable QObject context, @StrictNonNull Method0<R> method, Qt.@NonNull ConnectionType type) throws QUnsuccessfulInvocationException {
    	return invokeMethod(context!=null ? context.thread() : null, context, method, type);
    }
    
    /**
     * Calling <code>invokeMethod(method, AutoConnection, ...)</code>.
     * 
     * @param <A> The type of the first parameter of the method.
     * @param <R> The return type of the method.
     * @param context the context for invocation
     * @param method invoked method
     * @param arg1 Argument for the first parameter.
     * @return method result value - if the invocation is asynchronous, the return value cannot be evaluated.
     * @throws QUnsuccessfulInvocationException if not able to invoke slot
     */
    @QtUninvokable
    public static <A,R> R invokeMethod(@Nullable QObject context, @StrictNonNull Method1<A,R> method, A arg1) throws QUnsuccessfulInvocationException {
        return invokeMethod(context!=null ? context.thread() : null, context, method, Qt.ConnectionType.AutoConnection, arg1);
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
     * @param type synchronous or asynchronous invocation
     * @param arg1 Argument for the first parameter.
     * @return method result value - if the invocation is asynchronous, the return value cannot be evaluated.
     * @throws QUnsuccessfulInvocationException if not able to invoke slot
     */
    @QtUninvokable
    public static <A,R> R invokeMethod(@Nullable QObject context, @StrictNonNull Method1<A,R> method, Qt.@NonNull ConnectionType type, A arg1) throws QUnsuccessfulInvocationException {
    	return invokeMethod(context!=null ? context.thread() : null, context, method, type, arg1);
    }
    
    /**
     * Calling <code>invokeMethod(method, AutoConnection, ...)</code>.
     * 
     * @param <A> The type of the first parameter of the method.
     * @param <B> The type of the second parameter of the method.
     * @param <R> The return type of the method.
     * @param context the context for invocation
     * @param method invoked method
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @return method result value - if the invocation is asynchronous, the return value cannot be evaluated.
     * @throws QUnsuccessfulInvocationException if not able to invoke slot
     */
    @QtUninvokable
    public static <A,B,R> R invokeMethod(@Nullable QObject context, @StrictNonNull Method2<A,B,R> method, A arg1, B arg2) throws QUnsuccessfulInvocationException {
        return invokeMethod(context!=null ? context.thread() : null, context, method, Qt.ConnectionType.AutoConnection, arg1, arg2);
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
     * @param context the context for invocation
     * @param method invoked method
     * @param type synchronous or asynchronous invocation
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @return method result value - if the invocation is asynchronous, the return value cannot be evaluated.
     * @throws QUnsuccessfulInvocationException if not able to invoke slot
     */
    @QtUninvokable
    public static <A,B,R> R invokeMethod(@Nullable QObject context, @StrictNonNull Method2<A,B,R> method, Qt.@NonNull ConnectionType type, A arg1, B arg2) throws QUnsuccessfulInvocationException {
    	return invokeMethod(context!=null ? context.thread() : null, context, method, type, arg1, arg2);
    }
    
    /**
     * Calling <code>invokeMethod(method, AutoConnection, ...)</code>.
     * 
     * @param <A> The type of the first parameter of the method.
     * @param <B> The type of the second parameter of the method.
     * @param <C> The type of the third parameter of the method.
     * @param <R> The return type of the method.
     * @param context the context for invocation
     * @param method invoked method
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @return method result value - if the invocation is asynchronous, the return value cannot be evaluated.
     * @throws QUnsuccessfulInvocationException if not able to invoke slot
     */
    @QtUninvokable
    public static <A,B,C,R> R invokeMethod(@Nullable QObject context, @StrictNonNull Method3<A,B,C,R> method, A arg1, B arg2, C arg3) throws QUnsuccessfulInvocationException {
        return invokeMethod(context!=null ? context.thread() : null, context, method, Qt.ConnectionType.AutoConnection, arg1, arg2, arg3);
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
     * @param context the context for invocation
     * @param method invoked method
     * @param type synchronous or asynchronous invocation
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @return method result value - if the invocation is asynchronous, the return value cannot be evaluated.
     * @throws QUnsuccessfulInvocationException if not able to invoke slot
     */
    @QtUninvokable
    public static <A,B,C,R> R invokeMethod(@Nullable QObject context, @StrictNonNull Method3<A,B,C,R> method, Qt.@NonNull ConnectionType type, A arg1, B arg2, C arg3) throws QUnsuccessfulInvocationException {
    	return invokeMethod(context!=null ? context.thread() : null, context, method, type, arg1, arg2, arg3);
    }
    
    /**
     * Calling <code>invokeMethod(method, AutoConnection, ...)</code>.
     * 
     * @param <A> The type of the first parameter of the method.
     * @param <B> The type of the second parameter of the method.
     * @param <C> The type of the third parameter of the method.
     * @param <D> The type of the fourth parameter of the method.
     * @param <R> The return type of the method.
     * @param context the context for invocation
     * @param method invoked method
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @param arg4 Argument for the fourth parameter.
     * @return method result value - if the invocation is asynchronous, the return value cannot be evaluated.
     * @throws QUnsuccessfulInvocationException if not able to invoke slot
     */
    @QtUninvokable
    public static <A,B,C,D,R> R invokeMethod(@Nullable QObject context, @StrictNonNull Method4<A,B,C,D,R> method, A arg1, B arg2, C arg3, D arg4) throws QUnsuccessfulInvocationException {
        return invokeMethod(context!=null ? context.thread() : null, context, method, Qt.ConnectionType.AutoConnection, arg1, arg2, arg3, arg4);
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
     * @param context the context for invocation
     * @param method invoked method
     * @param type synchronous or asynchronous invocation
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @param arg4 Argument for the fourth parameter.
     * @return method result value - if the invocation is asynchronous, the return value cannot be evaluated.
     * @throws QUnsuccessfulInvocationException if not able to invoke slot
     */
    @QtUninvokable
    public static <A,B,C,D,R> R invokeMethod(@Nullable QObject context, @StrictNonNull Method4<A,B,C,D,R> method, Qt.@NonNull ConnectionType type, A arg1, B arg2, C arg3, D arg4) throws QUnsuccessfulInvocationException {
    	return invokeMethod(context!=null ? context.thread() : null, context, method, type, arg1, arg2, arg3, arg4);
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
     * @param context the context for invocation
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
    public static <A,B,C,D,E,R> R invokeMethod(@Nullable QObject context, @StrictNonNull Method5<A,B,C,D,E,R> method, A arg1, B arg2, C arg3, D arg4, E arg5) throws QUnsuccessfulInvocationException {
        return invokeMethod(context!=null ? context.thread() : null, context, method, Qt.ConnectionType.AutoConnection, arg1, arg2, arg3, arg4, arg5);
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
     * @param context the context for invocation
     * @param method invoked method
     * @param type synchronous or asynchronous invocation
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @param arg4 Argument for the fourth parameter.
     * @param arg5 Argument for the fifth parameter.
     * @return method result value - if the invocation is asynchronous, the return value cannot be evaluated.
     * @throws QUnsuccessfulInvocationException if not able to invoke slot
     */
    @QtUninvokable
    public static <A,B,C,D,E,R> R invokeMethod(@Nullable QObject context, @StrictNonNull Method5<A,B,C,D,E,R> method, Qt.@NonNull ConnectionType type, A arg1, B arg2, C arg3, D arg4, E arg5) throws QUnsuccessfulInvocationException {
    	return invokeMethod(context!=null ? context.thread() : null, context, method, type, arg1, arg2, arg3, arg4, arg5);
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
     * @param context the context for invocation
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
    public static <A,B,C,D,E,F,R> R invokeMethod(@Nullable QObject context, @StrictNonNull Method6<A,B,C,D,E,F,R> method, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6) throws QUnsuccessfulInvocationException {
        return invokeMethod(context!=null ? context.thread() : null, context, method, Qt.ConnectionType.AutoConnection, arg1, arg2, arg3, arg4, arg5, arg6);
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
     * @param context the context for invocation
     * @param method invoked method
     * @param type synchronous or asynchronous invocation
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
    public static <A,B,C,D,E,F,R> R invokeMethod(@Nullable QObject context, @StrictNonNull Method6<A,B,C,D,E,F,R> method, Qt.@NonNull ConnectionType type, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6) throws QUnsuccessfulInvocationException {
    	return invokeMethod(context!=null ? context.thread() : null, context, method, type, arg1, arg2, arg3, arg4, arg5, arg6);
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
     * @param context the context for invocation
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
    public static <A,B,C,D,E,F,G,R> R invokeMethod(@Nullable QObject context, @StrictNonNull Method7<A,B,C,D,E,F,G,R> method, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7) throws QUnsuccessfulInvocationException {
        return invokeMethod(context!=null ? context.thread() : null, context, method, Qt.ConnectionType.AutoConnection, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
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
     * @param context the context for invocation
     * @param method invoked method
     * @param type synchronous or asynchronous invocation
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
    public static <A,B,C,D,E,F,G,R> R invokeMethod(@Nullable QObject context, @StrictNonNull Method7<A,B,C,D,E,F,G,R> method, Qt.@NonNull ConnectionType type, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7) throws QUnsuccessfulInvocationException {
    	return invokeMethod(context!=null ? context.thread() : null, context, method, type, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
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
     * @param context the context for invocation
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
    public static <A,B,C,D,E,F,G,H,R> R invokeMethod(@Nullable QObject context, @StrictNonNull Method8<A,B,C,D,E,F,G,H,R> method, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7, H arg8) throws QUnsuccessfulInvocationException {
        return invokeMethod(context!=null ? context.thread() : null, context, method, Qt.ConnectionType.AutoConnection, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
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
     * @param context the context for invocation
     * @param method invoked method
     * @param type synchronous or asynchronous invocation
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
    public static <A,B,C,D,E,F,G,H,R> R invokeMethod(@Nullable QObject context, @StrictNonNull Method8<A,B,C,D,E,F,G,H,R> method, Qt.@NonNull ConnectionType type, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7, H arg8) throws QUnsuccessfulInvocationException {
    	return invokeMethod(context!=null ? context.thread() : null, context, method, type, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
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
     * @param context the context for invocation
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
    public static <A,B,C,D,E,F,G,H,I,R> R invokeMethod(@Nullable QObject context, @StrictNonNull Method9<A,B,C,D,E,F,G,H,I,R> method, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7, H arg8, I arg9) throws QUnsuccessfulInvocationException {
        return invokeMethod(context!=null ? context.thread() : null, context, method, Qt.ConnectionType.AutoConnection, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
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
     * @param context the context for invocation
     * @param method invoked method
     * @param type synchronous or asynchronous invocation
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
    public static <A,B,C,D,E,F,G,H,I,R> R invokeMethod(@Nullable QObject context, @StrictNonNull Method9<A,B,C,D,E,F,G,H,I,R> method, Qt.@NonNull ConnectionType type, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7, H arg8, I arg9) throws QUnsuccessfulInvocationException {
    	return invokeMethod(context!=null ? context.thread() : null, context, method, type, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
    }
    
    /**
     * Calling <code>invokeMethod(method, AutoConnection)</code>.
     * @param context the context for invocation
     * @param method invoked method
     * @throws QUnsuccessfulInvocationException if not able to invoke slot
     */
    @QtUninvokable
    public static void invokeMethod(@Nullable QObject context, @StrictNonNull Slot0 method) throws QUnsuccessfulInvocationException {
        invokeMethod(context!=null ? context.thread() : null, context, method, Qt.ConnectionType.AutoConnection);
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
     * @param context the context for invocation
     * @param method invoked method
     * @param type synchronous or asynchronous invocation
     * @throws QUnsuccessfulInvocationException if not able to invoke slot
     */
    @QtUninvokable
    public static void invokeMethod(@Nullable QObject context, @StrictNonNull Slot0 method, Qt.@NonNull ConnectionType type) throws QUnsuccessfulInvocationException {
    	invokeMethod(context!=null ? context.thread() : null, context, method, type);
    }
    
    /**
     * Calling <code>invokeMethod(method, AutoConnection, ...)</code>.
     * @param <A> The type of the first parameter of the slot.
     * @param context the context for invocation
     * @param method invoked method
     * @param arg1 Argument for the first parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke slot
     */
    @QtUninvokable
    public static <A> void invokeMethod(@Nullable QObject context, @StrictNonNull Slot1<A> method, A arg1) throws QUnsuccessfulInvocationException {
        invokeMethod(context!=null ? context.thread() : null, context, method, Qt.ConnectionType.AutoConnection, arg1);
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
     * @param context the context for invocation
     * @param method invoked method
     * @param type synchronous or asynchronous invocation
     * @param arg1 Argument for the first parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke slot
     */
    @QtUninvokable
    public static <A> void invokeMethod(@Nullable QObject context, @StrictNonNull Slot1<A> method, Qt.@NonNull ConnectionType type, A arg1) throws QUnsuccessfulInvocationException {
    	invokeMethod(context!=null ? context.thread() : null, context, method, type, arg1);
    }
    
    /**
     * Calling <code>invokeMethod(method, AutoConnection, ...)</code>.
     * @param <A> The type of the first parameter of the slot.
     * @param <B> The type of the second parameter of the slot.
     * @param context the context for invocation
     * @param method invoked method
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke slot
     */
    @QtUninvokable
    public static <A,B> void invokeMethod(@Nullable QObject context, @StrictNonNull Slot2<A,B> method, A arg1, B arg2) throws QUnsuccessfulInvocationException {
        invokeMethod(context!=null ? context.thread() : null, context, method, Qt.ConnectionType.AutoConnection, arg1, arg2);
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
     * @param context the context for invocation
     * @param method invoked method
     * @param type synchronous or asynchronous invocation
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke slot
     */
    @QtUninvokable
    public static <A,B> void invokeMethod(@Nullable QObject context, @StrictNonNull Slot2<A,B> method, Qt.@NonNull ConnectionType type, A arg1, B arg2) throws QUnsuccessfulInvocationException {
    	invokeMethod(context!=null ? context.thread() : null, context, method, type, arg1, arg2);
    }
    
    /**
     * Calling <code>invokeMethod(method, AutoConnection, ...)</code>.
     * @param <A> The type of the first parameter of the slot.
     * @param <B> The type of the second parameter of the slot.
     * @param <C> The type of the third parameter of the slot.
     * @param context the context for invocation
     * @param method invoked method
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke slot
     */
    @QtUninvokable
    public static <A,B,C> void invokeMethod(@Nullable QObject context, @StrictNonNull Slot3<A,B,C> method, A arg1, B arg2, C arg3) throws QUnsuccessfulInvocationException {
        invokeMethod(context!=null ? context.thread() : null, context, method, Qt.ConnectionType.AutoConnection, arg1, arg2, arg3);
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
     * @param context the context for invocation
     * @param method invoked method
     * @param type synchronous or asynchronous invocation
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke slot
     */
    @QtUninvokable
    public static <A,B,C> void invokeMethod(@Nullable QObject context, @StrictNonNull Slot3<A,B,C> method, Qt.@NonNull ConnectionType type, A arg1, B arg2, C arg3) throws QUnsuccessfulInvocationException {
    	invokeMethod(context!=null ? context.thread() : null, context, method, type, arg1, arg2, arg3);
    }
    
    /**
     * Calling <code>invokeMethod(method, AutoConnection, ...)</code>.
     * @param <A> The type of the first parameter of the slot.
     * @param <B> The type of the second parameter of the slot.
     * @param <C> The type of the third parameter of the slot.
     * @param <D> The type of the fourth parameter of the slot.
     * @param context the context for invocation
     * @param method invoked method
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @param arg4 Argument for the fourth parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke slot
     */
    @QtUninvokable
    public static <A,B,C,D> void invokeMethod(@Nullable QObject context, @StrictNonNull Slot4<A,B,C,D> method, A arg1, B arg2, C arg3, D arg4) throws QUnsuccessfulInvocationException {
        invokeMethod(context!=null ? context.thread() : null, context, method, Qt.ConnectionType.AutoConnection, arg1, arg2, arg3, arg4);
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
     * @param context the context for invocation
     * @param method invoked method
     * @param type synchronous or asynchronous invocation
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @param arg4 Argument for the fourth parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke slot
     */
    @QtUninvokable
    public static <A,B,C,D> void invokeMethod(@Nullable QObject context, @StrictNonNull Slot4<A,B,C,D> method, Qt.@NonNull ConnectionType type, A arg1, B arg2, C arg3, D arg4) throws QUnsuccessfulInvocationException {
    	invokeMethod(context!=null ? context.thread() : null, context, method, type, arg1, arg2, arg3, arg4);
    }
    
    /**
     * Calling <code>invokeMethod(method, AutoConnection, ...)</code>.
     * @param <A> The type of the first parameter of the slot.
     * @param <B> The type of the second parameter of the slot.
     * @param <C> The type of the third parameter of the slot.
     * @param <D> The type of the fourth parameter of the slot.
     * @param <E> The type of the fifth parameter of the slot.
     * @param context the context for invocation
     * @param method invoked method
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @param arg4 Argument for the fourth parameter.
     * @param arg5 Argument for the fifth parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke slot
     */
    @QtUninvokable
    public static <A,B,C,D,E> void invokeMethod(@Nullable QObject context, @StrictNonNull Slot5<A,B,C,D,E> method, A arg1, B arg2, C arg3, D arg4, E arg5) throws QUnsuccessfulInvocationException {
        invokeMethod(context!=null ? context.thread() : null, context, method, Qt.ConnectionType.AutoConnection, arg1, arg2, arg3, arg4, arg5);
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
     * @param context the context for invocation
     * @param method invoked method
     * @param type synchronous or asynchronous invocation
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @param arg4 Argument for the fourth parameter.
     * @param arg5 Argument for the fifth parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke slot
     */
    @QtUninvokable
    public static <A,B,C,D,E> void invokeMethod(@Nullable QObject context, @StrictNonNull Slot5<A,B,C,D,E> method, Qt.@NonNull ConnectionType type, A arg1, B arg2, C arg3, D arg4, E arg5) throws QUnsuccessfulInvocationException {
    	invokeMethod(context!=null ? context.thread() : null, context, method, type, arg1, arg2, arg3, arg4, arg5);
    }
    
    /**
     * Calling <code>invokeMethod(method, AutoConnection, ...)</code>.
     * @param <A> The type of the first parameter of the slot.
     * @param <B> The type of the second parameter of the slot.
     * @param <C> The type of the third parameter of the slot.
     * @param <D> The type of the fourth parameter of the slot.
     * @param <E> The type of the fifth parameter of the slot.
     * @param <F> The type of the sixth parameter of the slot.
     * @param context the context for invocation
     * @param method invoked method
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @param arg4 Argument for the fourth parameter.
     * @param arg5 Argument for the fifth parameter.
     * @param arg6 Argument for the sixth parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke slot
     */
    public static <A,B,C,D,E,F> void invokeMethod(@Nullable QObject context, @StrictNonNull Slot6<A,B,C,D,E,F> method, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6) throws QUnsuccessfulInvocationException {
        invokeMethod(context!=null ? context.thread() : null, context, method, Qt.ConnectionType.AutoConnection, arg1, arg2, arg3, arg4, arg5, arg6);
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
     * @param context the context for invocation
     * @param method invoked method
     * @param type synchronous or asynchronous invocation
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @param arg4 Argument for the fourth parameter.
     * @param arg5 Argument for the fifth parameter.
     * @param arg6 Argument for the sixth parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke slot
     */
    @QtUninvokable
    public static <A,B,C,D,E,F> void invokeMethod(@Nullable QObject context, @StrictNonNull Slot6<A,B,C,D,E,F> method, Qt.@NonNull ConnectionType type, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6) throws QUnsuccessfulInvocationException {
    	invokeMethod(context!=null ? context.thread() : null, context, method, type, arg1, arg2, arg3, arg4, arg5, arg6);
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
     * @param context the context for invocation
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
    public static <A,B,C,D,E,F,G> void invokeMethod(@Nullable QObject context, @StrictNonNull Slot7<A,B,C,D,E,F,G> method, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7) throws QUnsuccessfulInvocationException {
        invokeMethod(context!=null ? context.thread() : null, context, method, Qt.ConnectionType.AutoConnection, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
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
     * @param context the context for invocation
     * @param method invoked method
     * @param type synchronous or asynchronous invocation
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
    public static <A,B,C,D,E,F,G> void invokeMethod(@Nullable QObject context, @StrictNonNull Slot7<A,B,C,D,E,F,G> method, Qt.@NonNull ConnectionType type, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7) throws QUnsuccessfulInvocationException {
    	invokeMethod(context!=null ? context.thread() : null, context, method, type, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
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
     * @param context the context for invocation
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
    public static <A,B,C,D,E,F,G,H> void invokeMethod(@Nullable QObject context, @StrictNonNull Slot8<A,B,C,D,E,F,G,H> method, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7, H arg8) throws QUnsuccessfulInvocationException {
        invokeMethod(context!=null ? context.thread() : null, context, method, Qt.ConnectionType.AutoConnection, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
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
     * @param context the context for invocation
     * @param method invoked method
     * @param type synchronous or asynchronous invocation
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
    public static <A,B,C,D,E,F,G,H> void invokeMethod(@Nullable QObject context, @StrictNonNull Slot8<A,B,C,D,E,F,G,H> method, Qt.@NonNull ConnectionType type, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7, H arg8) throws QUnsuccessfulInvocationException {
    	invokeMethod(context!=null ? context.thread() : null, context, method, type, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
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
     * @param context the context for invocation
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
    public static <A,B,C,D,E,F,G,H,I> void invokeMethod(@Nullable QObject context, @StrictNonNull Slot9<A,B,C,D,E,F,G,H,I> method, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7, H arg8, I arg9) throws QUnsuccessfulInvocationException {
        invokeMethod(context!=null ? context.thread() : null, context, method, Qt.ConnectionType.AutoConnection, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
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
     * @param context the context for invocation
     * @param method invoked method
     * @param type synchronous or asynchronous invocation
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
    public static <A,B,C,D,E,F,G,H,I> void invokeMethod(@Nullable QObject context, @StrictNonNull Slot9<A,B,C,D,E,F,G,H,I> method, Qt.@NonNull ConnectionType type, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7, H arg8, I arg9) throws QUnsuccessfulInvocationException {
        invokeMethod(context!=null ? context.thread() : null, context, method, type, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
    }
    
    @QtUninvokable
    private static <R> R invokeMethod(QThread thread, QObject context, Method0<R> method, Qt.ConnectionType type) throws QUnsuccessfulInvocationException {
        switch(type) {
        case AutoConnection:
            if(thread!=null && thread!=QThread.currentThread()) {
                throw new QUnsuccessfulInvocationException("Unable to invoke methods with return values in queued connections (auto connection with different threads).");
            }
            // fall through
        case DirectConnection:
            try {
                return method.invoke();
            } catch (Throwable e) {
                throw new QUnsuccessfulInvocationException(e);
            }
        case BlockingQueuedConnection:
            if(thread!=null && thread!=QThread.currentThread()) {
                List<R> result = Arrays.asList((R)null);
                if(context!=null) {
                	invokeMethod(QtJambi_LibraryUtilities.internal.nativeId(context), ()->{
                		if(!(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
                			try {
                                result.set(0, method.invoke());
                            } catch (Throwable e) {
                                Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invocation.", e);
                            }
                		}
                	}, true);
                }else {
	                QObject invoker = new QObject() {
	                    @Override
	                    public boolean event(QEvent event) {
	                        if(event.type()==QEvent.Type.DeferredDispose
	                                && !(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
	                            try {
	                                result.set(0, method.invoke());
	                            } catch (Throwable e) {
	                                Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invocation.", e);
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
                }
                return result.get(0);
            }
            throw new QUnsuccessfulInvocationException("Blocking-queued invocation of method on not allowed without thread affinity.");
        case QueuedConnection:
            throw new QUnsuccessfulInvocationException("Unable to invoke methods with return values in queued connections.");
        default:
            break;
        }
        throw new QUnsuccessfulInvocationException("Unable to invoke method.");
    }
    
    @QtUninvokable
    private static <A,R> R invokeMethod(QThread thread, QObject context, Method1<A,R> method, Qt.ConnectionType type, A arg1) throws QUnsuccessfulInvocationException {
        switch(type) {
        case AutoConnection:
            if(thread!=null && thread!=QThread.currentThread()) {
                throw new QUnsuccessfulInvocationException("Unable to invoke methods with return values in queued connections (auto connection with different threads).");
            }
            // fall through
        case DirectConnection:
            try {
                return method.invoke(arg1);
            } catch (Throwable e) {
                throw new QUnsuccessfulInvocationException(e);
            }
        case BlockingQueuedConnection:
            if(thread!=null && thread!=QThread.currentThread()) {
                List<R> result = Arrays.asList((R)null);
                if(context!=null) {
                	invokeMethod(QtJambi_LibraryUtilities.internal.nativeId(context), ()->{
                		if(!(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
                			try {
                                result.set(0, method.invoke(arg1));
                            } catch (Throwable e) {
                                Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invocation.", e);
                            }
                		}
                	}, true);
                }else {
	                QObject invoker = new QObject() {
	                    @Override
	                    public boolean event(QEvent event) {
	                        if(event.type()==QEvent.Type.DeferredDispose
	                                && !(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
	                            try {
	                                result.set(0, method.invoke(arg1));
	                            } catch (Throwable e) {
	                                Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invocation.", e);
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
                }
                return result.get(0);
            }
            throw new QUnsuccessfulInvocationException("Blocking-queued invocation of method on not allowed without thread affinity.");
        case QueuedConnection:
            throw new QUnsuccessfulInvocationException("Unable to invoke methods with return values in queued connections.");
        default:
            break;
        }
        throw new QUnsuccessfulInvocationException("Unable to invoke method.");
    }
    
    @QtUninvokable
    private static <A,B,R> R invokeMethod(QThread thread, QObject context, Method2<A,B,R> method, Qt.ConnectionType type, A arg1, B arg2) throws QUnsuccessfulInvocationException {
        switch(type) {
        case AutoConnection:
            if(thread!=null && thread!=QThread.currentThread()) {
                throw new QUnsuccessfulInvocationException("Unable to invoke methods with return values in queued connections (auto connection with different threads).");
            }
        case DirectConnection:
            try {
                return method.invoke(arg1, arg2);
            } catch (Throwable e) {
                throw new QUnsuccessfulInvocationException(e);
            }
        case BlockingQueuedConnection:
            if(thread!=null && thread!=QThread.currentThread()) {
                List<R> result = Arrays.asList((R)null);
                if(context!=null) {
                	invokeMethod(QtJambi_LibraryUtilities.internal.nativeId(context), ()->{
                		if(!(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
                			try {
                                result.set(0, method.invoke(arg1, arg2));
                            } catch (Throwable e) {
                                Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invocation.", e);
                            }
                		}
                	}, true);
                }else {
	                QObject invoker = new QObject() {
	                    @Override
	                    public boolean event(QEvent event) {
	                        if(event.type()==QEvent.Type.DeferredDispose
	                                && !(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
	                            try {
	                                result.set(0, method.invoke(arg1, arg2));
	                            } catch (Throwable e) {
	                                Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invocation.", e);
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
                }
                return result.get(0);
            }
            throw new QUnsuccessfulInvocationException("Blocking-queued invocation of method on not allowed without thread affinity.");
        case QueuedConnection:
            throw new QUnsuccessfulInvocationException("Unable to invoke methods with return values in queued connections.");
        default:
            break;
        }
        throw new QUnsuccessfulInvocationException("Unable to invoke method.");
    }
    
    @QtUninvokable
    private static <A,B,C,R> R invokeMethod(QThread thread, QObject context, Method3<A,B,C,R> method, Qt.ConnectionType type, A arg1, B arg2, C arg3) throws QUnsuccessfulInvocationException {
        switch(type) {
        case AutoConnection:
            if(thread!=null && thread!=QThread.currentThread()) {
                throw new QUnsuccessfulInvocationException("Unable to invoke methods with return values in queued connections (auto connection with different threads).");
            }
        case DirectConnection:
            try {
                return method.invoke(arg1, arg2, arg3);
            } catch (Throwable e) {
                throw new QUnsuccessfulInvocationException(e);
            }
        case BlockingQueuedConnection:
            if(thread!=null && thread!=QThread.currentThread()) {
                List<R> result = Arrays.asList((R)null);
                if(context!=null) {
                	invokeMethod(QtJambi_LibraryUtilities.internal.nativeId(context), ()->{
                		if(!(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
                			try {
                                result.set(0, method.invoke(arg1, arg2, arg3));
                            } catch (Throwable e) {
                                Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invocation.", e);
                            }
                		}
                	}, true);
                }else {
	                QObject invoker = new QObject() {
	                    @Override
	                    public boolean event(QEvent event) {
	                        if(event.type()==QEvent.Type.DeferredDispose
	                                && !(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
	                            try {
	                                result.set(0, method.invoke(arg1, arg2, arg3));
	                            } catch (Throwable e) {
	                                Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invocation.", e);
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
                }
                return result.get(0);
            }
            throw new QUnsuccessfulInvocationException("Blocking-queued invocation of method on not allowed without thread affinity.");
        case QueuedConnection:
            throw new QUnsuccessfulInvocationException("Unable to invoke methods with return values in queued connections.");
        default:
            break;
        }
        throw new QUnsuccessfulInvocationException("Unable to invoke method.");
    }
    
    @QtUninvokable
    private static <A,B,C,D,R> R invokeMethod(QThread thread, QObject context, Method4<A,B,C,D,R> method, Qt.ConnectionType type, A arg1, B arg2, C arg3, D arg4) throws QUnsuccessfulInvocationException {
        switch(type) {
        case AutoConnection:
            if(thread!=null && thread!=QThread.currentThread()) {
                throw new QUnsuccessfulInvocationException("Unable to invoke methods with return values in queued connections (auto connection with different threads).");
            }
        case DirectConnection:
            try {
                return method.invoke(arg1, arg2, arg3, arg4);
            } catch (Throwable e) {
                throw new QUnsuccessfulInvocationException(e);
            }
        case BlockingQueuedConnection:
            if(thread!=null && thread!=QThread.currentThread()) {
                List<R> result = Arrays.asList((R)null);
                if(context!=null) {
                	invokeMethod(QtJambi_LibraryUtilities.internal.nativeId(context), ()->{
                		if(!(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
                			try {
                                result.set(0, method.invoke(arg1, arg2, arg3, arg4));
                            } catch (Throwable e) {
                                Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invocation.", e);
                            }
                		}
                	}, true);
                }else {
	                QObject invoker = new QObject() {
	                    @Override
	                    public boolean event(QEvent event) {
	                        if(event.type()==QEvent.Type.DeferredDispose
	                                && !(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
	                            try {
	                                result.set(0, method.invoke(arg1, arg2, arg3, arg4));
	                            } catch (Throwable e) {
	                                Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invocation.", e);
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
                }
                return result.get(0);
            }
            throw new QUnsuccessfulInvocationException("Blocking-queued invocation of method on not allowed without thread affinity.");
        case QueuedConnection:
            throw new QUnsuccessfulInvocationException("Unable to invoke methods with return values in queued connections.");
        default:
            break;
        }
        throw new QUnsuccessfulInvocationException("Unable to invoke method.");
    }
    
    @QtUninvokable
    private static <A,B,C,D,E,R> R invokeMethod(QThread thread, QObject context, Method5<A,B,C,D,E,R> method, Qt.ConnectionType type, A arg1, B arg2, C arg3, D arg4, E arg5) throws QUnsuccessfulInvocationException {
        switch(type) {
        case AutoConnection:
            if(thread!=null && thread!=QThread.currentThread()) {
                throw new QUnsuccessfulInvocationException("Unable to invoke methods with return values in queued connections (auto connection with different threads).");
            }
        case DirectConnection:
            try {
                return method.invoke(arg1, arg2, arg3, arg4, arg5);
            } catch (Throwable e) {
                throw new QUnsuccessfulInvocationException(e);
            }
        case BlockingQueuedConnection:
            if(thread!=null && thread!=QThread.currentThread()) {
                List<R> result = Arrays.asList((R)null);
                if(context!=null) {
                	invokeMethod(QtJambi_LibraryUtilities.internal.nativeId(context), ()->{
                		if(!(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
                			try {
                                result.set(0, method.invoke(arg1, arg2, arg3, arg4, arg5));
                            } catch (Throwable e) {
                                Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invocation.", e);
                            }
                		}
                	}, true);
                }else {
	                QObject invoker = new QObject() {
	                    @Override
	                    public boolean event(QEvent event) {
	                        if(event.type()==QEvent.Type.DeferredDispose
	                                && !(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
	                            try {
	                                result.set(0, method.invoke(arg1, arg2, arg3, arg4, arg5));
	                            } catch (Throwable e) {
	                                Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invocation.", e);
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
                }
                return result.get(0);
            }
            throw new QUnsuccessfulInvocationException("Blocking-queued invocation of method on not allowed without thread affinity.");
        case QueuedConnection:
            throw new QUnsuccessfulInvocationException("Unable to invoke methods with return values in queued connections.");
        default:
            break;
        }
        throw new QUnsuccessfulInvocationException("Unable to invoke method.");
    }
    
    @QtUninvokable
    private static <A,B,C,D,E,F,R> R invokeMethod(QThread thread, QObject context, Method6<A,B,C,D,E,F,R> method, Qt.ConnectionType type, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6) throws QUnsuccessfulInvocationException {
        switch(type) {
        case AutoConnection:
            if(thread!=null && thread!=QThread.currentThread()) {
                throw new QUnsuccessfulInvocationException("Unable to invoke methods with return values in queued connections (auto connection with different threads).");
            }
        case DirectConnection:
            try {
                return method.invoke(arg1, arg2, arg3, arg4, arg5, arg6);
            } catch (Throwable e) {
                throw new QUnsuccessfulInvocationException(e);
            }
        case BlockingQueuedConnection:
            if(thread!=null && thread!=QThread.currentThread()) {
                List<R> result = Arrays.asList((R)null);
                if(context!=null) {
                	invokeMethod(QtJambi_LibraryUtilities.internal.nativeId(context), ()->{
                		if(!(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
                			try {
                                result.set(0, method.invoke(arg1, arg2, arg3, arg4, arg5, arg6));
                            } catch (Throwable e) {
                                Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invocation.", e);
                            }
                		}
                	}, true);
                }else {
	                QObject invoker = new QObject() {
	                    @Override
	                    public boolean event(QEvent event) {
	                        if(event.type()==QEvent.Type.DeferredDispose
	                                && !(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
	                            try {
	                                result.set(0, method.invoke(arg1, arg2, arg3, arg4, arg5, arg6));
	                            } catch (Throwable e) {
	                                Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invocation.", e);
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
                }
                return result.get(0);
            }
            throw new QUnsuccessfulInvocationException("Blocking-queued invocation of method on not allowed without thread affinity.");
        case QueuedConnection:
            throw new QUnsuccessfulInvocationException("Unable to invoke methods with return values in queued connections.");
        default:
            break;
        }
        throw new QUnsuccessfulInvocationException("Unable to invoke method.");
    }
    
    @QtUninvokable
    private static <A,B,C,D,E,F,G,R> R invokeMethod(QThread thread, QObject context, Method7<A,B,C,D,E,F,G,R> method, Qt.ConnectionType type, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7) throws QUnsuccessfulInvocationException {
        switch(type) {
        case AutoConnection:
            if(thread!=null && thread!=QThread.currentThread()) {
                throw new QUnsuccessfulInvocationException("Unable to invoke methods with return values in queued connections (auto connection with different threads).");
            }
        case DirectConnection:
            try {
                return method.invoke(arg1, arg2, arg3, arg4, arg5, arg6, arg7);
            } catch (Throwable e) {
                throw new QUnsuccessfulInvocationException(e);
            }
        case BlockingQueuedConnection:
            if(thread!=null && thread!=QThread.currentThread()) {
                List<R> result = Arrays.asList((R)null);
                if(context!=null) {
                	invokeMethod(QtJambi_LibraryUtilities.internal.nativeId(context), ()->{
                		if(!(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
                			try {
                                result.set(0, method.invoke(arg1, arg2, arg3, arg4, arg5, arg6, arg7));
                            } catch (Throwable e) {
                                Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invocation.", e);
                            }
                		}
                	}, true);
                }else {
	                QObject invoker = new QObject() {
	                    @Override
	                    public boolean event(QEvent event) {
	                        if(event.type()==QEvent.Type.DeferredDispose
	                                && !(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
	                            try {
	                                result.set(0, method.invoke(arg1, arg2, arg3, arg4, arg5, arg6, arg7));
	                            } catch (Throwable e) {
	                                Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invocation.", e);
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
                }
                return result.get(0);
            }
            throw new QUnsuccessfulInvocationException("Blocking-queued invocation of method on not allowed without thread affinity.");
        case QueuedConnection:
            throw new QUnsuccessfulInvocationException("Unable to invoke methods with return values in queued connections.");
        default:
            break;
        }
        throw new QUnsuccessfulInvocationException("Unable to invoke method.");
    }
    
    @QtUninvokable
    private static <A,B,C,D,E,F,G,H,R> R invokeMethod(QThread thread, QObject context, Method8<A,B,C,D,E,F,G,H,R> method, Qt.ConnectionType type, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7, H arg8) throws QUnsuccessfulInvocationException {
        switch(type) {
        case AutoConnection:
            if(thread!=null && thread!=QThread.currentThread()) {
                throw new QUnsuccessfulInvocationException("Unable to invoke methods with return values in queued connections (auto connection with different threads).");
            }
        case DirectConnection:
            try {
                return method.invoke(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
            } catch (Throwable e) {
                throw new QUnsuccessfulInvocationException(e);
            }
        case BlockingQueuedConnection:
            if(thread!=null && thread!=QThread.currentThread()) {
                List<R> result = Arrays.asList((R)null);
                if(context!=null) {
                	invokeMethod(QtJambi_LibraryUtilities.internal.nativeId(context), ()->{
                		if(!(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
                			try {
                                result.set(0, method.invoke(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8));
                            } catch (Throwable e) {
                                Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invocation.", e);
                            }
                		}
                	}, true);
                }else {
	                QObject invoker = new QObject() {
	                    @Override
	                    public boolean event(QEvent event) {
	                        if(event.type()==QEvent.Type.DeferredDispose
	                                && !(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
	                            try {
	                                result.set(0, method.invoke(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8));
	                            } catch (Throwable e) {
	                                Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invocation.", e);
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
                }
                return result.get(0);
            }
            throw new QUnsuccessfulInvocationException("Blocking-queued invocation of method on not allowed without thread affinity.");
        case QueuedConnection:
            throw new QUnsuccessfulInvocationException("Unable to invoke methods with return values in queued connections.");
        default:
            break;
        }
        throw new QUnsuccessfulInvocationException("Unable to invoke method.");
    }
    
    @QtUninvokable
    private static <A,B,C,D,E,F,G,H,I,R> R invokeMethod(QThread thread, QObject context, Method9<A,B,C,D,E,F,G,H,I,R> method, Qt.ConnectionType type, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7, H arg8, I arg9) throws QUnsuccessfulInvocationException {
        switch(type) {
        case AutoConnection:
            if(thread!=null && thread!=QThread.currentThread()) {
                throw new QUnsuccessfulInvocationException("Unable to invoke methods with return values in queued connections (auto connection with different threads).");
            }
        case DirectConnection:
            try {
                return method.invoke(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
            } catch (Throwable e) {
                throw new QUnsuccessfulInvocationException(e);
            }
        case BlockingQueuedConnection:
            if(thread!=null && thread!=QThread.currentThread()) {
                List<R> result = Arrays.asList((R)null);
                if(context!=null) {
                	invokeMethod(QtJambi_LibraryUtilities.internal.nativeId(context), ()->{
                		if(!(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
                			try {
                                result.set(0, method.invoke(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9));
                            } catch (Throwable e) {
                                Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invocation.", e);
                            }
                		}
                	}, true);
                }else {
	                QObject invoker = new QObject() {
	                    @Override
	                    public boolean event(QEvent event) {
	                        if(event.type()==QEvent.Type.DeferredDispose
	                                && !(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
	                            try {
	                                result.set(0, method.invoke(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9));
	                            } catch (Throwable e) {
	                                Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invocation.", e);
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
                }
                return result.get(0);
            }
            throw new QUnsuccessfulInvocationException("Blocking-queued invocation of method on not allowed without thread affinity.");
        case QueuedConnection:
            throw new QUnsuccessfulInvocationException("Unable to invoke methods with return values in queued connections.");
        default:
            break;
        }
        throw new QUnsuccessfulInvocationException("Unable to invoke method.");
    }
    
    @QtUninvokable
    private static void invokeMethod(QThread thread, QObject context, Slot0 method, Qt.ConnectionType type) throws QUnsuccessfulInvocationException {
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
                    if(context!=null) {
                    	invokeMethod(QtJambi_LibraryUtilities.internal.nativeId(context), ()->{
                    		if(!(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
                    			try {
                                    method.invoke();
                                } catch (Throwable e) {
                                    Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invocation.", e);
                                }
                    		}
                    	}, true);
                    }else {
	                    QObject invoker = new QObject() {
	                        @Override
	                        public boolean event(QEvent event) {
	                            if(event.type()==QEvent.Type.DeferredDispose
	                                    && !(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
	                                try {
	                                    method.invoke();
	                                } catch (Throwable e) {
	                                    Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invocation.", e);
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
                    }
                    return;
                }else {
                    throw new QUnsuccessfulInvocationException("Blocking-queued invocation on object whose thread is the current thread is not allowed.");
                }
            }
            throw new QUnsuccessfulInvocationException("Blocking-queued invocation of method on not allowed without thread affinity.");
        case QueuedConnection:
            if(context!=null) {
            	invokeMethod(QtJambi_LibraryUtilities.internal.nativeId(context), ()->{
            		if(!(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
            			try {
                            method.invoke();
                        } catch (Throwable e) {
                            Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invocation.", e);
                        }
            		}
            	}, false);
            }else {
	            QObject invoker = new QObject() {
	                @Override
	                public boolean event(QEvent event) {
	                    if(event.type()==QEvent.Type.DeferredDispose
	                            && !(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
	                        try {
	                            method.invoke();
	                        } catch (Throwable e) {
	                            Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invocation.", e);
	                        }
	                    }
	                    return super.event(event);
	                }
	            };
	            QtJambi_LibraryUtilities.internal.setCppOwnership(invoker);
	            if(thread!=null)
	                invoker.moveToThread(thread);
	            invoker.disposeLater();
            }
            return;
        default:
            break;
        }
        throw new QUnsuccessfulInvocationException("Unable to invoke method.");
    }
    
    @QtUninvokable
    private static <A> void invokeMethod(QThread thread, QObject context, Slot1<A> method, Qt.ConnectionType type, A arg1) throws QUnsuccessfulInvocationException {
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
                    if(context!=null) {
                    	invokeMethod(QtJambi_LibraryUtilities.internal.nativeId(context), ()->{
                    		if(!(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
                    			try {
                                    method.invoke(arg1);
                                } catch (Throwable e) {
                                    Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invocation.", e);
                                }
                    		}
                    	}, true);
                    }else {
	                    QObject invoker = new QObject() {
	                        @Override
	                        public boolean event(QEvent event) {
	                            if(event.type()==QEvent.Type.DeferredDispose
	                                    && !(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
	                                try {
	                                    method.invoke(arg1);
	                                } catch (Throwable e) {
	                                    Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invocation.", e);
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
                    }
                    return;
                }else {
                    throw new QUnsuccessfulInvocationException("Blocking-queued invocation on object whose thread is the current thread is not allowed.");
                }
            }
            throw new QUnsuccessfulInvocationException("Blocking-queued invocation of method on not allowed without thread affinity.");
        case QueuedConnection:
            if(context!=null) {
            	invokeMethod(QtJambi_LibraryUtilities.internal.nativeId(context), ()->{
            		if(!(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
            			try {
                            method.invoke(arg1);
                        } catch (Throwable e) {
                            Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invocation.", e);
                        }
            		}
            	}, false);
            }else {
	            QObject invoker = new QObject() {
	                @Override
	                public boolean event(QEvent event) {
	                    if(event.type()==QEvent.Type.DeferredDispose
	                            && !(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
	                        try {
	                            method.invoke(arg1);
	                        } catch (Throwable e) {
	                            Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invocation.", e);
	                        }
	                    }
	                    return super.event(event);
	                }
	            };
	            QtJambi_LibraryUtilities.internal.setCppOwnership(invoker);
	            if(thread!=null)
	                invoker.moveToThread(thread);
	            invoker.disposeLater();
            }
            return;
        default:
            break;
        }
        throw new QUnsuccessfulInvocationException("Unable to invoke method.");
    }
    
    @QtUninvokable
    private static <A,B> void invokeMethod(QThread thread, QObject context, Slot2<A,B> method, Qt.ConnectionType type, A arg1, B arg2) throws QUnsuccessfulInvocationException {
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
                    if(context!=null) {
                    	invokeMethod(QtJambi_LibraryUtilities.internal.nativeId(context), ()->{
                    		if(!(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
                    			try {
                                    method.invoke(arg1, arg2);
                                } catch (Throwable e) {
                                    Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invocation.", e);
                                }
                    		}
                    	}, true);
                    }else {
	                    QObject invoker = new QObject() {
	                        @Override
	                        public boolean event(QEvent event) {
	                            if(event.type()==QEvent.Type.DeferredDispose
	                                    && !(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
	                                try {
	                                    method.invoke(arg1, arg2);
	                                } catch (Throwable e) {
	                                    Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invocation.", e);
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
                    }
                    return;
                }else {
                    throw new QUnsuccessfulInvocationException("Blocking-queued invocation on object whose thread is the current thread is not allowed.");
                }
            }
            throw new QUnsuccessfulInvocationException("Blocking-queued invocation of method on not allowed without thread affinity.");
        case QueuedConnection:
            if(context!=null) {
            	invokeMethod(QtJambi_LibraryUtilities.internal.nativeId(context), ()->{
            		if(!(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
            			try {
                            method.invoke(arg1, arg2);
                        } catch (Throwable e) {
                            Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invocation.", e);
                        }
            		}
            	}, false);
            }else {
	            QObject invoker = new QObject() {
	                @Override
	                public boolean event(QEvent event) {
	                    if(event.type()==QEvent.Type.DeferredDispose
	                            && !(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
	                        try {
	                            method.invoke(arg1, arg2);
	                        } catch (Throwable e) {
	                            Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invocation.", e);
	                        }
	                    }
	                    return super.event(event);
	                }
	            };
	            QtJambi_LibraryUtilities.internal.setCppOwnership(invoker);
	            if(thread!=null)
	                invoker.moveToThread(thread);
	            invoker.disposeLater();
            }
            return;
        default:
            break;
        }
        throw new QUnsuccessfulInvocationException("Unable to invoke method.");
    }
    
    @QtUninvokable
    private static <A,B,C> void invokeMethod(QThread thread, QObject context, Slot3<A,B,C> method, Qt.ConnectionType type, A arg1, B arg2, C arg3) throws QUnsuccessfulInvocationException {
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
                    if(context!=null) {
                    	invokeMethod(QtJambi_LibraryUtilities.internal.nativeId(context), ()->{
                    		if(!(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
                    			try {
                                    method.invoke(arg1, arg2, arg3);
                                } catch (Throwable e) {
                                    Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invocation.", e);
                                }
                    		}
                    	}, true);
                    }else {
	                    QObject invoker = new QObject() {
	                        @Override
	                        public boolean event(QEvent event) {
	                            if(event.type()==QEvent.Type.DeferredDispose
	                                    && !(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
	                                try {
	                                    method.invoke(arg1, arg2, arg3);
	                                } catch (Throwable e) {
	                                    Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invocation.", e);
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
                    }
                    return;
                }else {
                    throw new QUnsuccessfulInvocationException("Blocking-queued invocation on object whose thread is the current thread is not allowed.");
                }
            }
            throw new QUnsuccessfulInvocationException("Blocking-queued invocation of method on not allowed without thread affinity.");
        case QueuedConnection:
            if(context!=null) {
            	invokeMethod(QtJambi_LibraryUtilities.internal.nativeId(context), ()->{
            		if(!(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
            			try {
                            method.invoke(arg1, arg2, arg3);
                        } catch (Throwable e) {
                            Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invocation.", e);
                        }
            		}
            	}, false);
            }else {
	            QObject invoker = new QObject() {
	                @Override
	                public boolean event(QEvent event) {
	                    if(event.type()==QEvent.Type.DeferredDispose
	                            && !(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
	                        try {
	                            method.invoke(arg1, arg2, arg3);
	                        } catch (Throwable e) {
	                            Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invocation.", e);
	                        }
	                    }
	                    return super.event(event);
	                }
	            };
	            QtJambi_LibraryUtilities.internal.setCppOwnership(invoker);
	            if(thread!=null)
	                invoker.moveToThread(thread);
	            invoker.disposeLater();
            }
            return;
        default:
            break;
        }
        throw new QUnsuccessfulInvocationException("Unable to invoke method.");
    }
    
    @QtUninvokable
    private static <A,B,C,D> void invokeMethod(QThread thread, QObject context, Slot4<A,B,C,D> method, Qt.ConnectionType type, A arg1, B arg2, C arg3, D arg4) throws QUnsuccessfulInvocationException {
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
                    if(context!=null) {
                    	invokeMethod(QtJambi_LibraryUtilities.internal.nativeId(context), ()->{
                    		if(!(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
                    			try {
                                    method.invoke(arg1, arg2, arg3, arg4);
                                } catch (Throwable e) {
                                    Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invocation.", e);
                                }
                    		}
                    	}, true);
                    }else {
	                    QObject invoker = new QObject() {
	                        @Override
	                        public boolean event(QEvent event) {
	                            if(event.type()==QEvent.Type.DeferredDispose
	                                    && !(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
	                                try {
	                                    method.invoke(arg1, arg2, arg3, arg4);
	                                } catch (Throwable e) {
	                                    Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invocation.", e);
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
                    }
                    return;
                }else {
                    throw new QUnsuccessfulInvocationException("Blocking-queued invocation on object whose thread is the current thread is not allowed.");
                }
            }
            throw new QUnsuccessfulInvocationException("Blocking-queued invocation of method on not allowed without thread affinity.");
        case QueuedConnection:
        	if(context!=null) {
            	invokeMethod(QtJambi_LibraryUtilities.internal.nativeId(context), ()->{
            		if(!(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
            			try {
                            method.invoke(arg1, arg2, arg3, arg4);
                        } catch (Throwable e) {
                            Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invocation.", e);
                        }
            		}
            	}, false);
            }else {
	            QObject invoker = new QObject() {
	                @Override
	                public boolean event(QEvent event) {
	                    if(event.type()==QEvent.Type.DeferredDispose
	                            && !(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
	                        try {
	                            method.invoke(arg1, arg2, arg3, arg4);
	                        } catch (Throwable e) {
	                            Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invocation.", e);
	                        }
	                    }
	                    return super.event(event);
	                }
	            };
	            QtJambi_LibraryUtilities.internal.setCppOwnership(invoker);
	            if(thread!=null)
	                invoker.moveToThread(thread);
	            invoker.disposeLater();
            }
            return;
        default:
            break;
        }
        throw new QUnsuccessfulInvocationException("Unable to invoke method.");
    }
    
    @QtUninvokable
    private static <A,B,C,D,E> void invokeMethod(QThread thread, QObject context, Slot5<A,B,C,D,E> method, Qt.@NonNull ConnectionType type, A arg1, B arg2, C arg3, D arg4, E arg5) throws QUnsuccessfulInvocationException {
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
                    if(context!=null) {
                    	invokeMethod(QtJambi_LibraryUtilities.internal.nativeId(context), ()->{
                    		if(!(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
                    			try {
                                    method.invoke(arg1, arg2, arg3, arg4, arg5);
                                } catch (Throwable e) {
                                    Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invocation.", e);
                                }
                    		}
                    	}, true);
                    }else {
	                    QObject invoker = new QObject() {
	                        @Override
	                        public boolean event(QEvent event) {
	                            if(event.type()==QEvent.Type.DeferredDispose
	                                    && !(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
	                                try {
	                                    method.invoke(arg1, arg2, arg3, arg4, arg5);
	                                } catch (Throwable e) {
	                                    Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invocation.", e);
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
                    }
                    return;
                }else {
                    throw new QUnsuccessfulInvocationException("Blocking-queued invocation on object whose thread is the current thread is not allowed.");
                }
            }
            throw new QUnsuccessfulInvocationException("Blocking-queued invocation of method on not allowed without thread affinity.");
        case QueuedConnection:
            if(context!=null) {
            	invokeMethod(QtJambi_LibraryUtilities.internal.nativeId(context), ()->{
            		if(!(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
            			try {
                            method.invoke(arg1, arg2, arg3, arg4, arg5);
                        } catch (Throwable e) {
                            Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invocation.", e);
                        }
            		}
            	}, false);
            }else {
	            QObject invoker = new QObject() {
	                @Override
	                public boolean event(QEvent event) {
	                    if(event.type()==QEvent.Type.DeferredDispose
	                            && !(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
	                        try {
	                            method.invoke(arg1, arg2, arg3, arg4, arg5);
	                        } catch (Throwable e) {
	                            Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invocation.", e);
	                        }
	                    }
	                    return super.event(event);
	                }
	            };
	            QtJambi_LibraryUtilities.internal.setCppOwnership(invoker);
	            if(thread!=null)
	                invoker.moveToThread(thread);
	            invoker.disposeLater();
            }
            return;
        default:
            break;
        }
        throw new QUnsuccessfulInvocationException("Unable to invoke method.");
    }
    
    @QtUninvokable
    private static <A,B,C,D,E,F> void invokeMethod(QThread thread, QObject context, Slot6<A,B,C,D,E,F> method, Qt.ConnectionType type, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6) throws QUnsuccessfulInvocationException {
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
                    if(context!=null) {
                    	invokeMethod(QtJambi_LibraryUtilities.internal.nativeId(context), ()->{
                    		if(!(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
                    			try {
                                    method.invoke(arg1, arg2, arg3, arg4, arg5, arg6);
                                } catch (Throwable e) {
                                    Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invocation.", e);
                                }
                    		}
                    	}, true);
                    }else {
	                    QObject invoker = new QObject() {
	                        @Override
	                        public boolean event(QEvent event) {
	                            if(event.type()==QEvent.Type.DeferredDispose
	                                    && !(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
	                                try {
	                                    method.invoke(arg1, arg2, arg3, arg4, arg5, arg6);
	                                } catch (Throwable e) {
	                                    Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invocation.", e);
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
                    }
                    return;
                }else {
                    throw new QUnsuccessfulInvocationException("Blocking-queued invocation on object whose thread is the current thread is not allowed.");
                }
            }
            throw new QUnsuccessfulInvocationException("Blocking-queued invocation of method on not allowed without thread affinity.");
        case QueuedConnection:
            if(context!=null) {
            	invokeMethod(QtJambi_LibraryUtilities.internal.nativeId(context), ()->{
            		if(!(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
            			try {
                            method.invoke(arg1, arg2, arg3, arg4, arg5, arg6);
                        } catch (Throwable e) {
                            Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invocation.", e);
                        }
            		}
            	}, true);
            }else {
	            QObject invoker = new QObject() {
	                @Override
	                public boolean event(QEvent event) {
	                    if(event.type()==QEvent.Type.DeferredDispose
	                            && !(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
	                        try {
	                            method.invoke(arg1, arg2, arg3, arg4, arg5, arg6);
	                        } catch (Throwable e) {
	                            Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invocation.", e);
	                        }
	                    }
	                    return super.event(event);
	                }
	            };
	            QtJambi_LibraryUtilities.internal.setCppOwnership(invoker);
	            if(thread!=null)
	                invoker.moveToThread(thread);
	            invoker.disposeLater();
            }
            return;
        default:
            break;
        }
        throw new QUnsuccessfulInvocationException("Unable to invoke method.");
    }
    
    @QtUninvokable
    private static <A,B,C,D,E,F,G> void invokeMethod(QThread thread, QObject context, Slot7<A,B,C,D,E,F,G> method, Qt.ConnectionType type, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7) throws QUnsuccessfulInvocationException {
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
                    if(context!=null) {
                    	invokeMethod(QtJambi_LibraryUtilities.internal.nativeId(context), ()->{
                    		if(!(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
                    			try {
                                    method.invoke(arg1, arg2, arg3, arg4, arg5, arg6, arg7);
                                } catch (Throwable e) {
                                    Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invocation.", e);
                                }
                    		}
                    	}, true);
                    }else {
	                    QObject invoker = new QObject() {
	                        @Override
	                        public boolean event(QEvent event) {
	                            if(event.type()==QEvent.Type.DeferredDispose
	                                    && !(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
	                                try {
	                                    method.invoke(arg1, arg2, arg3, arg4, arg5, arg6, arg7);
	                                } catch (Throwable e) {
	                                    Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invocation.", e);
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
                    }
                    return;
                }else {
                    throw new QUnsuccessfulInvocationException("Blocking-queued invocation on object whose thread is the current thread is not allowed.");
                }
            }
            throw new QUnsuccessfulInvocationException("Blocking-queued invocation of method on not allowed without thread affinity.");
        case QueuedConnection:
            if(context!=null) {
            	invokeMethod(QtJambi_LibraryUtilities.internal.nativeId(context), ()->{
            		if(!(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
            			try {
                            method.invoke(arg1, arg2, arg3, arg4, arg5, arg6, arg7);
                        } catch (Throwable e) {
                            Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invocation.", e);
                        }
            		}
            	}, false);
            }else {
	            QObject invoker = new QObject() {
	                @Override
	                public boolean event(QEvent event) {
	                    if(event.type()==QEvent.Type.DeferredDispose
	                            && !(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
	                        try {
	                            method.invoke(arg1, arg2, arg3, arg4, arg5, arg6, arg7);
	                        } catch (Throwable e) {
	                            Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invocation.", e);
	                        }
	                    }
	                    return super.event(event);
	                }
	            };
	            QtJambi_LibraryUtilities.internal.setCppOwnership(invoker);
	            if(thread!=null)
	                invoker.moveToThread(thread);
	            invoker.disposeLater();
            }
            return;
        default:
            break;
        }
        throw new QUnsuccessfulInvocationException("Unable to invoke method.");
    }
    
    @QtUninvokable
    private static <A,B,C,D,E,F,G,H> void invokeMethod(QThread thread, QObject context, Slot8<A,B,C,D,E,F,G,H> method, Qt.ConnectionType type, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7, H arg8) throws QUnsuccessfulInvocationException {
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
                    if(context!=null) {
                    	invokeMethod(QtJambi_LibraryUtilities.internal.nativeId(context), ()->{
                    		if(!(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
                    			try {
                                    method.invoke(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
                                } catch (Throwable e) {
                                    Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invocation.", e);
                                }
                    		}
                    	}, true);
                    }else {
	                    QObject invoker = new QObject() {
	                        @Override
	                        public boolean event(QEvent event) {
	                            if(event.type()==QEvent.Type.DeferredDispose
	                                    && !(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
	                                try {
	                                    method.invoke(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
	                                } catch (Throwable e) {
	                                    Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invocation.", e);
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
                    }
                    return;
                }else {
                    throw new QUnsuccessfulInvocationException("Blocking-queued invocation on object whose thread is the current thread is not allowed.");
                }
            }
            throw new QUnsuccessfulInvocationException("Blocking-queued invocation of method on not allowed without thread affinity.");
        case QueuedConnection:
            if(context!=null) {
            	invokeMethod(QtJambi_LibraryUtilities.internal.nativeId(context), ()->{
            		if(!(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
            			try {
                            method.invoke(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
                        } catch (Throwable e) {
                            Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invocation.", e);
                        }
            		}
            	}, false);
            }else {
	            QObject invoker = new QObject() {
	                @Override
	                public boolean event(QEvent event) {
	                    if(event.type()==QEvent.Type.DeferredDispose
	                            && !(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
	                        try {
	                            method.invoke(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
	                        } catch (Throwable e) {
	                            Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invocation.", e);
	                        }
	                    }
	                    return super.event(event);
	                }
	            };
	            QtJambi_LibraryUtilities.internal.setCppOwnership(invoker);
	            if(thread!=null)
	                invoker.moveToThread(thread);
	            invoker.disposeLater();
            }
            return;
        default:
            break;
        }
        throw new QUnsuccessfulInvocationException("Unable to invoke method.");
    }
    
    @QtUninvokable
    private static <A,B,C,D,E,F,G,H,I> void invokeMethod(QThread thread, QObject context, Slot9<A,B,C,D,E,F,G,H,I> method, Qt.ConnectionType type, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7, H arg8, I arg9) throws QUnsuccessfulInvocationException {
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
                    if(context!=null) {
                    	invokeMethod(QtJambi_LibraryUtilities.internal.nativeId(context), ()->{
                    		if(!(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
                    			try {
                                    method.invoke(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
                                } catch (Throwable e) {
                                    Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invocation.", e);
                                }
                    		}
                    	}, true);
                    }else {
	                    QObject invoker = new QObject() {
	                        @Override
	                        public boolean event(QEvent event) {
	                            if(event.type()==QEvent.Type.DeferredDispose
	                                    && !(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
	                                try {
	                                    method.invoke(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
	                                } catch (Throwable e) {
	                                    Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invocation.", e);
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
                    }
                    return;
                }else {
                    throw new QUnsuccessfulInvocationException("Blocking-queued invocation on object whose thread is the current thread is not allowed.");
                }
            }
            throw new QUnsuccessfulInvocationException("Blocking-queued invocation of method on not allowed without thread affinity.");
        case QueuedConnection:
            if(context!=null) {
            	invokeMethod(QtJambi_LibraryUtilities.internal.nativeId(context), ()->{
            		if(!(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
            			try {
                            method.invoke(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
                        } catch (Throwable e) {
                            Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invocation.", e);
                        }
            		}
            	}, false);
            }else {
	            QObject invoker = new QObject() {
	                @Override
	                public boolean event(QEvent event) {
	                    if(event.type()==QEvent.Type.DeferredDispose
	                            && !(method instanceof QtObjectInterface && ((QtObjectInterface)method).isDisposed() )) {
	                        try {
	                            method.invoke(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
	                        } catch (Throwable e) {
	                            Logger.getLogger("io.qt.core").log(Level.SEVERE, "Exception thrown during method invocation.", e);
	                        }
	                    }
	                    return super.event(event);
	                }
	            };
	            QtJambi_LibraryUtilities.internal.setCppOwnership(invoker);
	            if(thread!=null)
	                invoker.moveToThread(thread);
	            invoker.disposeLater();
            }
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
    public static void invokeMethod(@StrictNonNull AbstractPublicSignal0 signal) throws QUnsuccessfulInvocationException {
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
     * @param type synchronous or asynchronous invocation
     * @throws QUnsuccessfulInvocationException if not able to invoke signal
     */
    @QtUninvokable
    public static void invokeMethod(@StrictNonNull AbstractPublicSignal0 signal, Qt.@NonNull ConnectionType type) throws QUnsuccessfulInvocationException {
    	if(signal.containingObject() instanceof QObject) {
        	QObject qobject = (QObject)signal.containingObject();
        	QThread thread = qobject.thread();
        	switch(type){
			case AutoConnection:
				if(thread!=null && thread!=QThread.currentThread())
					break;
			case DirectConnection:
				signal.emit();
				return;
			default:
				break;
        	}
        	QMetaMethod qmethod = qobject.metaObject().methodByIndex(qobject.metaObject().metaObjectPointer, signal.methodIndex());
            if(qmethod!=null && qmethod.isValid()) {
                qmethod.invoke(qobject, type);
            }else {
            	invokeMethod(thread, qobject, signal::emit, type);
            }
        }else if(signal.containingObject() instanceof QtThreadAffineInterface) {
        	invokeMethod(((QtThreadAffineInterface)signal.containingObject()).thread(), (QObject)null, signal::emit, type);
        }else {
        	invokeMethod((QThread)null, (QObject)null, signal::emit, type);
        }
    }
    
    /**
     * Calling <code>invokeMethod(signal, AutoConnection)</code>.
     * @param signal invoked signal
     */
    @QtUninvokable
    public static void invokeMethod(@StrictNonNull AbstractPrivateSignal0 signal) throws QUnsuccessfulInvocationException {
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
     * @param type synchronous or asynchronous invocation
     * @throws QUnsuccessfulInvocationException if not able to invoke signal
     */
    @QtUninvokable
    public static void invokeMethod(@StrictNonNull AbstractPrivateSignal0 signal, Qt.@NonNull ConnectionType type) throws QUnsuccessfulInvocationException {
        if(signal.containingObject() instanceof QObject && !((QObject)signal.containingObject()).isDisposed()) {
            QObject qobject = (QObject)signal.containingObject();
            QMetaMethod qmethod = qobject.metaObject().methodByIndex(qobject.metaObject().metaObjectPointer, signal.methodIndex());
            if(qmethod!=null && qmethod.isValid()) {
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
    public static <A> void invokeMethod(@StrictNonNull AbstractPublicSignal1<A> signal, A arg1) throws QUnsuccessfulInvocationException {
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
     * @param type synchronous or asynchronous invocation
     * @param arg1 Argument for the first parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke signal
     */
    @QtUninvokable
    public static <A> void invokeMethod(@StrictNonNull AbstractPublicSignal1<A> signal, Qt.@NonNull ConnectionType type, A arg1) throws QUnsuccessfulInvocationException {
    	if(signal.containingObject() instanceof QObject) {
        	QObject qobject = (QObject)signal.containingObject();
        	QThread thread = qobject.thread();
        	switch(type){
			case AutoConnection:
				if(thread!=null && thread!=QThread.currentThread())
					break;
			case DirectConnection:
				signal.emit(arg1);
				return;
			default:
				break;
        	}
        	QMetaMethod qmethod = qobject.metaObject().methodByIndex(qobject.metaObject().metaObjectPointer, signal.methodIndex());
            if(qmethod!=null && qmethod.isValid()) {
                qmethod.invoke(qobject, type, arg1);
            }else {
            	invokeMethod(thread, qobject, signal::emit, type, arg1);
            }
        }else if(signal.containingObject() instanceof QtThreadAffineInterface) {
        	invokeMethod(((QtThreadAffineInterface)signal.containingObject()).thread(), (QObject)null, signal::emit, type, arg1);
        }else {
        	invokeMethod((QThread)null, (QObject)null, signal::emit, type, arg1);
        }
    }
    
    /**
     * Calling <code>invokeMethod(signal, AutoConnection, ...)</code>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param signal invoked signal
     * @param arg1 Argument for the first parameter.
     */
    @QtUninvokable
    public static <A> void invokeMethod(@StrictNonNull AbstractPrivateSignal1<A> signal, A arg1) throws QUnsuccessfulInvocationException {
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
     * @param type synchronous or asynchronous invocation
     * @param arg1 Argument for the first parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke signal
     */
    @QtUninvokable
    public static <A> void invokeMethod(@StrictNonNull AbstractPrivateSignal1<A> signal, Qt.@NonNull ConnectionType type, A arg1) throws QUnsuccessfulInvocationException {
        if(signal.containingObject() instanceof QObject && !((QObject)signal.containingObject()).isDisposed()) {
            QObject qobject = (QObject)signal.containingObject();
            QMetaMethod qmethod = qobject.metaObject().methodByIndex(qobject.metaObject().metaObjectPointer, signal.methodIndex());
            if(qmethod!=null && qmethod.isValid()) {
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
     * @param signal invoked signal
     */
    @QtUninvokable
    public static <A> void invokeMethod(@StrictNonNull AbstractSignal1Default1<A> signal) throws QUnsuccessfulInvocationException {
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
     * @param <A> The type of the first parameter of the signal.
     * @param signal invoked signal
     * @param type synchronous or asynchronous invocation
     * @throws QUnsuccessfulInvocationException if not able to invoke signal
     */
    @QtUninvokable
    public static <A> void invokeMethod(@StrictNonNull AbstractSignal1Default1<A> signal, Qt.@NonNull ConnectionType type) throws QUnsuccessfulInvocationException {
    	if(signal.containingObject() instanceof QObject) {
        	QObject qobject = (QObject)signal.containingObject();
        	QThread thread = qobject.thread();
        	switch(type){
			case AutoConnection:
				if(thread!=null && thread!=QThread.currentThread())
					break;
			case DirectConnection:
				signal.emit();
				return;
			default:
				break;
        	}
        	QMetaMethod qmethod = qobject.metaObject().methodByIndex(qobject.metaObject().metaObjectPointer, signal.methodIndex());
            if(qmethod!=null && qmethod.isValid()) {
                qmethod.invoke(qobject, type, 
                		signal.arg1Default.get());
            }else {
            	invokeMethod(thread, qobject, 
            			(Slot0)signal::emit, 
            			type);
            }
        }else if(signal.containingObject() instanceof QtThreadAffineInterface) {
        	invokeMethod(((QtThreadAffineInterface)signal.containingObject()).thread(), 
        			(QObject)null, 
        			(Slot0)signal::emit, 
        			type);
        }else {
        	invokeMethod((QThread)null, 
        			(QObject)null, 
        			(Slot0)signal::emit, 
        			type);
        }
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
    public static <A,B> void invokeMethod(@StrictNonNull AbstractPublicSignal2<A,B> signal, A arg1, B arg2) throws QUnsuccessfulInvocationException {
        invokeMethod(signal, Qt.ConnectionType.AutoConnection, arg1, arg2);
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
    public static <A,B> void invokeMethod(@StrictNonNull AbstractPrivateSignal2<A,B> signal, A arg1, B arg2) throws QUnsuccessfulInvocationException {
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
     * @param type synchronous or asynchronous invocation
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke signal
     */
    @QtUninvokable
    public static <A,B> void invokeMethod(@StrictNonNull AbstractPublicSignal2<A,B> signal, Qt.@NonNull ConnectionType type, A arg1, B arg2) throws QUnsuccessfulInvocationException {
    	if(signal.containingObject() instanceof QObject) {
        	QObject qobject = (QObject)signal.containingObject();
        	QThread thread = qobject.thread();
        	switch(type){
			case AutoConnection:
				if(thread!=null && thread!=QThread.currentThread())
					break;
			case DirectConnection:
				signal.emit(arg1, arg2);
				return;
			default:
				break;
        	}
        	QMetaMethod qmethod = qobject.metaObject().methodByIndex(qobject.metaObject().metaObjectPointer, signal.methodIndex());
            if(qmethod!=null && qmethod.isValid()) {
                qmethod.invoke(qobject, type, arg1, arg2);
            }else {
            	invokeMethod(thread, qobject, signal::emit, type, arg1, arg2);
            }
        }else if(signal.containingObject() instanceof QtThreadAffineInterface) {
        	invokeMethod(((QtThreadAffineInterface)signal.containingObject()).thread(), (QObject)null, signal::emit, type, arg1, arg2);
        }else {
        	invokeMethod((QThread)null, (QObject)null, signal::emit, type, arg1, arg2);
        }
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
     * @param type synchronous or asynchronous invocation
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke signal
     */
    @QtUninvokable
    public static <A,B> void invokeMethod(@StrictNonNull AbstractPrivateSignal2<A,B> signal, Qt.@NonNull ConnectionType type, A arg1, B arg2) throws QUnsuccessfulInvocationException {
        if(signal.containingObject() instanceof QObject && !((QObject)signal.containingObject()).isDisposed()) {
            QObject qobject = (QObject)signal.containingObject();
            QMetaMethod qmethod = qobject.metaObject().methodByIndex(qobject.metaObject().metaObjectPointer, signal.methodIndex());
            if(qmethod!=null && qmethod.isValid()) {
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
     * @param signal invoked signal
     * @param arg1 Argument for the first parameter.
     */
    @QtUninvokable
    public static <A,B> void invokeMethod(@StrictNonNull AbstractSignal2Default1<A,B> signal, A arg1) throws QUnsuccessfulInvocationException {
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
     * @param <B> The type of the second parameter of the signal.
     * @param signal invoked signal
     * @param type synchronous or asynchronous invocation
     * @param arg1 Argument for the first parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke signal
     */
    @QtUninvokable
    public static <A,B> void invokeMethod(@StrictNonNull AbstractSignal2Default1<A,B> signal, Qt.@NonNull ConnectionType type, A arg1) throws QUnsuccessfulInvocationException {
    	if(signal.containingObject() instanceof QObject) {
        	QObject qobject = (QObject)signal.containingObject();
        	QThread thread = qobject.thread();
        	switch(type){
			case AutoConnection:
				if(thread!=null && thread!=QThread.currentThread())
					break;
			case DirectConnection:
				signal.emit(arg1);
				return;
			default:
				break;
        	}
        	QMetaMethod qmethod = qobject.metaObject().methodByIndex(qobject.metaObject().metaObjectPointer, signal.methodIndex());
            if(qmethod!=null && qmethod.isValid()) {
                qmethod.invoke(qobject, type, 
                		arg1, 
                		signal.arg2Default.get());
            }else {
            	invokeMethod(thread, qobject, 
            			(Slot1<A>)signal::emit, 
            			type, arg1);
            }
        }else if(signal.containingObject() instanceof QtThreadAffineInterface) {
        	invokeMethod(((QtThreadAffineInterface)signal.containingObject()).thread(), 
        			(QObject)null, 
        			(Slot1<A>)signal::emit, 
        			type, arg1);
        }else {
        	invokeMethod((QThread)null, 
        			(QObject)null, 
        			(Slot1<A>)signal::emit, 
        			type, arg1);
        }
    }
    
    /**
     * Calling <code>invokeMethod(signal, AutoConnection, ...)</code>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param signal invoked signal
     */
    @QtUninvokable
    public static <A,B> void invokeMethod(@StrictNonNull AbstractSignal2Default2<A,B> signal) throws QUnsuccessfulInvocationException {
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
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param signal invoked signal
     * @param type synchronous or asynchronous invocation
     * @throws QUnsuccessfulInvocationException if not able to invoke signal
     */
    @QtUninvokable
    public static <A,B> void invokeMethod(@StrictNonNull AbstractSignal2Default2<A,B> signal, Qt.@NonNull ConnectionType type) throws QUnsuccessfulInvocationException {
    	if(signal.containingObject() instanceof QObject) {
        	QObject qobject = (QObject)signal.containingObject();
        	QThread thread = qobject.thread();
        	switch(type){
			case AutoConnection:
				if(thread!=null && thread!=QThread.currentThread())
					break;
			case DirectConnection:
				signal.emit();
				return;
			default:
				break;
        	}
        	QMetaMethod qmethod = qobject.metaObject().methodByIndex(qobject.metaObject().metaObjectPointer, signal.methodIndex());
            if(qmethod!=null && qmethod.isValid()) {
                qmethod.invoke(qobject, type, 
                		signal.arg1Default.get(), 
                		((AbstractSignal2Default1<A,B>)signal).arg2Default.get());
            }else {
            	invokeMethod(thread, qobject, 
            			(Slot0)signal::emit, 
            			type);
            }
        }else if(signal.containingObject() instanceof QtThreadAffineInterface) {
        	invokeMethod(((QtThreadAffineInterface)signal.containingObject()).thread(), 
        			(QObject)null, 
        			(Slot0)signal::emit, 
        			type);
        }else {
        	invokeMethod((QThread)null, 
        			(QObject)null, 
        			(Slot0)signal::emit, 
        			type);
        }
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
    public static <A,B,C> void invokeMethod(@StrictNonNull AbstractPublicSignal3<A,B,C> signal, A arg1, B arg2, C arg3) throws QUnsuccessfulInvocationException {
        invokeMethod(signal, Qt.ConnectionType.AutoConnection, arg1, arg2, arg3);
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
    public static <A,B,C> void invokeMethod(@StrictNonNull AbstractPrivateSignal3<A,B,C> signal, A arg1, B arg2, C arg3) throws QUnsuccessfulInvocationException {
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
     * @param type synchronous or asynchronous invocation
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke signal
     */
    @QtUninvokable
    public static <A,B,C> void invokeMethod(@StrictNonNull AbstractPublicSignal3<A,B,C> signal, Qt.@NonNull ConnectionType type, A arg1, B arg2, C arg3) throws QUnsuccessfulInvocationException {
    	if(signal.containingObject() instanceof QObject) {
        	QObject qobject = (QObject)signal.containingObject();
        	QThread thread = qobject.thread();
        	switch(type){
			case AutoConnection:
				if(thread!=null && thread!=QThread.currentThread())
					break;
			case DirectConnection:
				signal.emit(arg1, arg2, arg3);
				return;
			default:
				break;
        	}
        	QMetaMethod qmethod = qobject.metaObject().methodByIndex(qobject.metaObject().metaObjectPointer, signal.methodIndex());
            if(qmethod!=null && qmethod.isValid()) {
                qmethod.invoke(qobject, type, arg1, arg2, arg3);
            }else {
            	invokeMethod(thread, qobject, signal::emit, type, arg1, arg2, arg3);
            }
        }else if(signal.containingObject() instanceof QtThreadAffineInterface) {
        	invokeMethod(((QtThreadAffineInterface)signal.containingObject()).thread(), (QObject)null, signal::emit, type, arg1, arg2, arg3);
        }else {
        	invokeMethod((QThread)null, (QObject)null, signal::emit, type, arg1, arg2, arg3);
        }
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
     * @param type synchronous or asynchronous invocation
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke signal
     */
    @QtUninvokable
    public static <A,B,C> void invokeMethod(@StrictNonNull AbstractPrivateSignal3<A,B,C> signal, Qt.@NonNull ConnectionType type, A arg1, B arg2, C arg3) throws QUnsuccessfulInvocationException {
        if(signal.containingObject() instanceof QObject && !((QObject)signal.containingObject()).isDisposed()) {
            QObject qobject = (QObject)signal.containingObject();
            QMetaMethod qmethod = qobject.metaObject().methodByIndex(qobject.metaObject().metaObjectPointer, signal.methodIndex());
            if(qmethod!=null && qmethod.isValid()) {
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
     * @param signal invoked signal
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     */
    @QtUninvokable
    public static <A,B,C> void invokeMethod(@StrictNonNull AbstractSignal3Default1<A,B,C> signal, A arg1, B arg2) throws QUnsuccessfulInvocationException {
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
     * @param <C> The type of the third parameter of the signal.
     * @param signal invoked signal
     * @param type synchronous or asynchronous invocation
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke signal
     */
    @QtUninvokable
    public static <A,B,C> void invokeMethod(@StrictNonNull AbstractSignal3Default1<A,B,C> signal, Qt.@NonNull ConnectionType type, A arg1, B arg2) throws QUnsuccessfulInvocationException {
    	if(signal.containingObject() instanceof QObject) {
        	QObject qobject = (QObject)signal.containingObject();
        	QThread thread = qobject.thread();
        	switch(type){
			case AutoConnection:
				if(thread!=null && thread!=QThread.currentThread())
					break;
			case DirectConnection:
				signal.emit(arg1, arg2);
				return;
			default:
				break;
        	}
        	QMetaMethod qmethod = qobject.metaObject().methodByIndex(qobject.metaObject().metaObjectPointer, signal.methodIndex());
            if(qmethod!=null && qmethod.isValid()) {
                qmethod.invoke(qobject, type, 
                		arg1, arg2, 
                		signal.arg3Default.get());
            }else {
            	invokeMethod(thread, qobject, 
            			(Slot2<A,B>)signal::emit, 
            			type, arg1, arg2);
            }
        }else if(signal.containingObject() instanceof QtThreadAffineInterface) {
        	invokeMethod(((QtThreadAffineInterface)signal.containingObject()).thread(), 
        			(QObject)null, 
        			(Slot2<A,B>)signal::emit, 
        			type, arg1, arg2);
        }else {
        	invokeMethod((QThread)null, 
        			(QObject)null, 
        			(Slot2<A,B>)signal::emit, 
        			type, arg1, arg2);
        }
    }
    
    /**
     * Calling <code>invokeMethod(signal, AutoConnection, ...)</code>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param signal invoked signal
     * @param arg1 Argument for the first parameter.
     */
    @QtUninvokable
    public static <A,B,C> void invokeMethod(@StrictNonNull AbstractSignal3Default2<A,B,C> signal, A arg1) throws QUnsuccessfulInvocationException {
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
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param signal invoked signal
     * @param type synchronous or asynchronous invocation
     * @param arg1 Argument for the first parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke signal
     */
    @QtUninvokable
    public static <A,B,C> void invokeMethod(@StrictNonNull AbstractSignal3Default2<A,B,C> signal, Qt.@NonNull ConnectionType type, A arg1) throws QUnsuccessfulInvocationException {
    	if(signal.containingObject() instanceof QObject) {
        	QObject qobject = (QObject)signal.containingObject();
        	QThread thread = qobject.thread();
        	switch(type){
			case AutoConnection:
				if(thread!=null && thread!=QThread.currentThread())
					break;
			case DirectConnection:
				signal.emit(arg1);
				return;
			default:
				break;
        	}
        	QMetaMethod qmethod = qobject.metaObject().methodByIndex(qobject.metaObject().metaObjectPointer, signal.methodIndex());
            if(qmethod!=null && qmethod.isValid()) {
                qmethod.invoke(qobject, type, 
                		arg1, 
                		signal.arg2Default.get(), 
                		((AbstractSignal3Default1<A,B,C>)signal).arg3Default.get());
            }else {
            	invokeMethod(thread, qobject, 
            			(Slot1<A>)signal::emit, 
            			type, arg1);
            }
        }else if(signal.containingObject() instanceof QtThreadAffineInterface) {
        	invokeMethod(((QtThreadAffineInterface)signal.containingObject()).thread(), 
        			(QObject)null, 
        			(Slot1<A>)signal::emit, 
        			type, arg1);
        }else {
        	invokeMethod((QThread)null, 
        			(QObject)null, 
        			(Slot1<A>)signal::emit, 
        			type, arg1);
        }
    }
    
    /**
     * Calling <code>invokeMethod(signal, AutoConnection, ...)</code>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param signal invoked signal
     */
    @QtUninvokable
    public static <A,B,C> void invokeMethod(@StrictNonNull AbstractSignal3Default3<A,B,C> signal) throws QUnsuccessfulInvocationException {
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
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param signal invoked signal
     * @param type synchronous or asynchronous invocation
     * @throws QUnsuccessfulInvocationException if not able to invoke signal
     */
    @QtUninvokable
    public static <A,B,C> void invokeMethod(@StrictNonNull AbstractSignal3Default3<A,B,C> signal, Qt.@NonNull ConnectionType type) throws QUnsuccessfulInvocationException {
    	if(signal.containingObject() instanceof QObject) {
        	QObject qobject = (QObject)signal.containingObject();
        	QThread thread = qobject.thread();
        	switch(type){
			case AutoConnection:
				if(thread!=null && thread!=QThread.currentThread())
					break;
			case DirectConnection:
				signal.emit();
				return;
			default:
				break;
        	}
        	QMetaMethod qmethod = qobject.metaObject().methodByIndex(qobject.metaObject().metaObjectPointer, signal.methodIndex());
            if(qmethod!=null && qmethod.isValid()) {
                qmethod.invoke(qobject, type, 
                		signal.arg1Default.get(), 
                		((AbstractSignal3Default2<A,B,C>)signal).arg2Default.get(), 
                		((AbstractSignal3Default1<A,B,C>)signal).arg3Default.get());
            }else {
            	invokeMethod(thread, qobject, 
            			(Slot0)signal::emit, 
            			type);
            }
        }else if(signal.containingObject() instanceof QtThreadAffineInterface) {
        	invokeMethod(((QtThreadAffineInterface)signal.containingObject()).thread(), 
        			(QObject)null, 
        			(Slot0)signal::emit, 
        			type);
        }else {
        	invokeMethod((QThread)null, 
        			(QObject)null, 
        			(Slot0)signal::emit, 
        			type);
        }
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
    public static <A,B,C,D> void invokeMethod(@StrictNonNull AbstractPublicSignal4<A,B,C,D> signal, A arg1, B arg2, C arg3, D arg4) throws QUnsuccessfulInvocationException {
        invokeMethod(signal, Qt.ConnectionType.AutoConnection, arg1, arg2, arg3, arg4);
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
    public static <A,B,C,D> void invokeMethod(@StrictNonNull AbstractPrivateSignal4<A,B,C,D> signal, A arg1, B arg2, C arg3, D arg4) throws QUnsuccessfulInvocationException {
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
     * @param type synchronous or asynchronous invocation
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @param arg4 Argument for the fourth parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke signal
     */
    @QtUninvokable
    public static <A,B,C,D> void invokeMethod(@StrictNonNull AbstractPublicSignal4<A,B,C,D> signal, Qt.@NonNull ConnectionType type, A arg1, B arg2, C arg3, D arg4) throws QUnsuccessfulInvocationException {
    	if(signal.containingObject() instanceof QObject) {
        	QObject qobject = (QObject)signal.containingObject();
        	QThread thread = qobject.thread();
        	switch(type){
			case AutoConnection:
				if(thread!=null && thread!=QThread.currentThread())
					break;
			case DirectConnection:
				signal.emit(arg1, arg2, arg3, arg4);
				return;
			default:
				break;
        	}
        	QMetaMethod qmethod = qobject.metaObject().methodByIndex(qobject.metaObject().metaObjectPointer, signal.methodIndex());
            if(qmethod!=null && qmethod.isValid()) {
                qmethod.invoke(qobject, type, arg1, arg2, arg3, arg4);
            }else {
            	invokeMethod(thread, qobject, signal::emit, type, arg1, arg2, arg3, arg4);
            }
        }else if(signal.containingObject() instanceof QtThreadAffineInterface) {
        	invokeMethod(((QtThreadAffineInterface)signal.containingObject()).thread(), (QObject)null, signal::emit, type, arg1, arg2, arg3, arg4);
        }else {
        	invokeMethod((QThread)null, (QObject)null, signal::emit, type, arg1, arg2, arg3, arg4);
        }
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
     * @param type synchronous or asynchronous invocation
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @param arg4 Argument for the fourth parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke signal
     */
    @QtUninvokable
    public static <A,B,C,D> void invokeMethod(@StrictNonNull AbstractPrivateSignal4<A,B,C,D> signal, Qt.@NonNull ConnectionType type, A arg1, B arg2, C arg3, D arg4) throws QUnsuccessfulInvocationException {
        if(signal.containingObject() instanceof QObject && !((QObject)signal.containingObject()).isDisposed()) {
            QObject qobject = (QObject)signal.containingObject();
            QMetaMethod qmethod = qobject.metaObject().methodByIndex(qobject.metaObject().metaObjectPointer, signal.methodIndex());
            if(qmethod!=null && qmethod.isValid()) {
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
     * @param signal invoked signal
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     */
    @QtUninvokable
    public static <A,B,C,D> void invokeMethod(@StrictNonNull AbstractSignal4Default1<A,B,C,D> signal, A arg1, B arg2, C arg3) throws QUnsuccessfulInvocationException {
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
     * @param <D> The type of the fourth parameter of the signal.
     * @param signal invoked signal
     * @param type synchronous or asynchronous invocation
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke signal
     */
    @QtUninvokable
    public static <A,B,C,D> void invokeMethod(@StrictNonNull AbstractSignal4Default1<A,B,C,D> signal, Qt.@NonNull ConnectionType type, A arg1, B arg2, C arg3) throws QUnsuccessfulInvocationException {
    	if(signal.containingObject() instanceof QObject) {
        	QObject qobject = (QObject)signal.containingObject();
        	QThread thread = qobject.thread();
        	switch(type){
			case AutoConnection:
				if(thread!=null && thread!=QThread.currentThread())
					break;
			case DirectConnection:
				signal.emit(arg1, arg2, arg3);
				return;
			default:
				break;
        	}
        	QMetaMethod qmethod = qobject.metaObject().methodByIndex(qobject.metaObject().metaObjectPointer, signal.methodIndex());
            if(qmethod!=null && qmethod.isValid()) {
                qmethod.invoke(qobject, type, 
                		arg1, arg2, arg3, 
                		signal.arg4Default.get());
            }else {
            	invokeMethod(thread, qobject, 
            			(Slot3<A,B,C>)signal::emit, 
            			type, arg1, arg2, arg3);
            }
        }else if(signal.containingObject() instanceof QtThreadAffineInterface) {
        	invokeMethod(((QtThreadAffineInterface)signal.containingObject()).thread(), 
        			(QObject)null, 
        			(Slot3<A,B,C>)signal::emit, 
        			type, arg1, arg2, arg3);
        }else {
        	invokeMethod((QThread)null, 
        			(QObject)null, 
        			(Slot3<A,B,C>)signal::emit, 
        			type, arg1, arg2, arg3);
        }
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
     */
    @QtUninvokable
    public static <A,B,C,D> void invokeMethod(@StrictNonNull AbstractSignal4Default2<A,B,C,D> signal, A arg1, B arg2) throws QUnsuccessfulInvocationException {
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
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param signal invoked signal
     * @param type synchronous or asynchronous invocation
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke signal
     */
    @QtUninvokable
    public static <A,B,C,D> void invokeMethod(@StrictNonNull AbstractSignal4Default2<A,B,C,D> signal, Qt.@NonNull ConnectionType type, A arg1, B arg2) throws QUnsuccessfulInvocationException {
    	if(signal.containingObject() instanceof QObject) {
        	QObject qobject = (QObject)signal.containingObject();
        	QThread thread = qobject.thread();
        	switch(type){
			case AutoConnection:
				if(thread!=null && thread!=QThread.currentThread())
					break;
			case DirectConnection:
				signal.emit(arg1, arg2);
				return;
			default:
				break;
        	}
        	QMetaMethod qmethod = qobject.metaObject().methodByIndex(qobject.metaObject().metaObjectPointer, signal.methodIndex());
            if(qmethod!=null && qmethod.isValid()) {
                qmethod.invoke(qobject, type, 
                		arg1, arg2, 
                		signal.arg3Default.get(), 
                		((AbstractSignal4Default1<A,B,C,D>)signal).arg4Default.get());
            }else {
            	invokeMethod(thread, qobject, 
            			(Slot2<A,B>)signal::emit, 
            			type, arg1, arg2);
            }
        }else if(signal.containingObject() instanceof QtThreadAffineInterface) {
        	invokeMethod(((QtThreadAffineInterface)signal.containingObject()).thread(), 
        			(QObject)null, 
        			(Slot2<A,B>)signal::emit, 
        			type, arg1, arg2);
        }else {
        	invokeMethod((QThread)null, 
        			(QObject)null, 
        			(Slot2<A,B>)signal::emit, 
        			type, arg1, arg2);
        }
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
     */
    @QtUninvokable
    public static <A,B,C,D> void invokeMethod(@StrictNonNull AbstractSignal4Default3<A,B,C,D> signal, A arg1) throws QUnsuccessfulInvocationException {
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
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param signal invoked signal
     * @param type synchronous or asynchronous invocation
     * @param arg1 Argument for the first parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke signal
     */
    @QtUninvokable
    public static <A,B,C,D> void invokeMethod(@StrictNonNull AbstractSignal4Default3<A,B,C,D> signal, Qt.@NonNull ConnectionType type, A arg1) throws QUnsuccessfulInvocationException {
    	if(signal.containingObject() instanceof QObject) {
        	QObject qobject = (QObject)signal.containingObject();
        	QThread thread = qobject.thread();
        	switch(type){
			case AutoConnection:
				if(thread!=null && thread!=QThread.currentThread())
					break;
			case DirectConnection:
				signal.emit(arg1);
				return;
			default:
				break;
        	}
        	QMetaMethod qmethod = qobject.metaObject().methodByIndex(qobject.metaObject().metaObjectPointer, signal.methodIndex());
            if(qmethod!=null && qmethod.isValid()) {
                qmethod.invoke(qobject, type, 
                		arg1, 
                		signal.arg2Default.get(),
                		((AbstractSignal4Default2<A,B,C,D>)signal).arg3Default.get(),
                		((AbstractSignal4Default1<A,B,C,D>)signal).arg4Default.get());
            }else {
            	invokeMethod(thread, qobject, 
            			(Slot1<A>)signal::emit, 
            			type, arg1);
            }
        }else if(signal.containingObject() instanceof QtThreadAffineInterface) {
        	invokeMethod(((QtThreadAffineInterface)signal.containingObject()).thread(), 
        			(QObject)null, 
        			(Slot1<A>)signal::emit, 
        			type, arg1);
        }else {
        	invokeMethod((QThread)null, 
        			(QObject)null, 
        			(Slot1<A>)signal::emit, 
        			type, arg1);
        }
    }
    
    /**
     * Calling <code>invokeMethod(signal, AutoConnection, ...)</code>.
     * 
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param signal invoked signal
     */
    @QtUninvokable
    public static <A,B,C,D> void invokeMethod(@StrictNonNull AbstractSignal4Default4<A,B,C,D> signal) throws QUnsuccessfulInvocationException {
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
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param signal invoked signal
     * @param type synchronous or asynchronous invocation
     * @throws QUnsuccessfulInvocationException if not able to invoke signal
     */
    @QtUninvokable
    public static <A,B,C,D> void invokeMethod(@StrictNonNull AbstractSignal4Default4<A,B,C,D> signal, Qt.@NonNull ConnectionType type) throws QUnsuccessfulInvocationException {
    	if(signal.containingObject() instanceof QObject) {
        	QObject qobject = (QObject)signal.containingObject();
        	QThread thread = qobject.thread();
        	switch(type){
			case AutoConnection:
				if(thread!=null && thread!=QThread.currentThread())
					break;
			case DirectConnection:
				signal.emit();
				return;
			default:
				break;
        	}
        	QMetaMethod qmethod = qobject.metaObject().methodByIndex(qobject.metaObject().metaObjectPointer, signal.methodIndex());
            if(qmethod!=null && qmethod.isValid()) {
                qmethod.invoke(qobject, type, 
                		signal.arg1Default.get(), 
                		((AbstractSignal4Default3<A,B,C,D>)signal).arg2Default.get(), 
                		((AbstractSignal4Default2<A,B,C,D>)signal).arg3Default.get(), 
                		((AbstractSignal4Default1<A,B,C,D>)signal).arg4Default.get());
            }else {
            	invokeMethod(thread, qobject, 
            			(Slot0)signal::emit, 
            			type);
            }
        }else if(signal.containingObject() instanceof QtThreadAffineInterface) {
        	invokeMethod(((QtThreadAffineInterface)signal.containingObject()).thread(), 
        			(QObject)null, 
        			(Slot0)signal::emit, 
        			type);
        }else {
        	invokeMethod((QThread)null, 
        			(QObject)null, 
        			(Slot0)signal::emit, 
        			type);
        }
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
    public static <A,B,C,D,E> void invokeMethod(@StrictNonNull AbstractPublicSignal5<A,B,C,D,E> signal, A arg1, B arg2, C arg3, D arg4, E arg5) throws QUnsuccessfulInvocationException {
        invokeMethod(signal, Qt.ConnectionType.AutoConnection, arg1, arg2, arg3, arg4, arg5);
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
    public static <A,B,C,D,E> void invokeMethod(@StrictNonNull AbstractPrivateSignal5<A,B,C,D,E> signal, A arg1, B arg2, C arg3, D arg4, E arg5) throws QUnsuccessfulInvocationException {
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
     * @param type synchronous or asynchronous invocation
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @param arg4 Argument for the fourth parameter.
     * @param arg5 Argument for the fifth parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke signal
     */
    @QtUninvokable
    public static <A,B,C,D,E> void invokeMethod(@StrictNonNull AbstractPublicSignal5<A,B,C,D,E> signal, Qt.@NonNull ConnectionType type, A arg1, B arg2, C arg3, D arg4, E arg5) throws QUnsuccessfulInvocationException {
    	if(signal.containingObject() instanceof QObject) {
        	QObject qobject = (QObject)signal.containingObject();
        	QThread thread = qobject.thread();
        	switch(type){
			case AutoConnection:
				if(thread!=null && thread!=QThread.currentThread())
					break;
			case DirectConnection:
				signal.emit(arg1, arg2, arg3, arg4, arg5);
				return;
			default:
				break;
        	}
        	QMetaMethod qmethod = qobject.metaObject().methodByIndex(qobject.metaObject().metaObjectPointer, signal.methodIndex());
            if(qmethod!=null && qmethod.isValid()) {
                qmethod.invoke(qobject, type, arg1, arg2, arg3, arg4, arg5);
            }else {
            	invokeMethod(thread, qobject, signal::emit, type, arg1, arg2, arg3, arg4, arg5);
            }
        }else if(signal.containingObject() instanceof QtThreadAffineInterface) {
        	invokeMethod(((QtThreadAffineInterface)signal.containingObject()).thread(), (QObject)null, signal::emit, type, arg1, arg2, arg3, arg4, arg5);
        }else {
        	invokeMethod((QThread)null, (QObject)null, signal::emit, type, arg1, arg2, arg3, arg4, arg5);
        }
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
     * @param type synchronous or asynchronous invocation
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @param arg4 Argument for the fourth parameter.
     * @param arg5 Argument for the fifth parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke signal
     */
    @QtUninvokable
    public static <A,B,C,D,E> void invokeMethod(@StrictNonNull AbstractPrivateSignal5<A,B,C,D,E> signal, Qt.@NonNull ConnectionType type, A arg1, B arg2, C arg3, D arg4, E arg5) throws QUnsuccessfulInvocationException {
        if(signal.containingObject() instanceof QObject && !((QObject)signal.containingObject()).isDisposed()) {
            QObject qobject = (QObject)signal.containingObject();
            QMetaMethod qmethod = qobject.metaObject().methodByIndex(qobject.metaObject().metaObjectPointer, signal.methodIndex());
            if(qmethod!=null && qmethod.isValid()) {
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
     * @param signal invoked signal
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @param arg4 Argument for the fourth parameter.
     */
    @QtUninvokable
    public static <A,B,C,D,E> void invokeMethod(@StrictNonNull AbstractSignal5Default1<A,B,C,D,E> signal, A arg1, B arg2, C arg3, D arg4) throws QUnsuccessfulInvocationException {
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
     * @param <E> The type of the fifth parameter of the signal.
     * @param signal invoked signal
     * @param type synchronous or asynchronous invocation
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @param arg4 Argument for the fourth parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke signal
     */
    @QtUninvokable
    public static <A,B,C,D,E> void invokeMethod(@StrictNonNull AbstractSignal5Default1<A,B,C,D,E> signal, Qt.@NonNull ConnectionType type, A arg1, B arg2, C arg3, D arg4) throws QUnsuccessfulInvocationException {
    	if(signal.containingObject() instanceof QObject) {
        	QObject qobject = (QObject)signal.containingObject();
        	QThread thread = qobject.thread();
        	switch(type){
			case AutoConnection:
				if(thread!=null && thread!=QThread.currentThread())
					break;
			case DirectConnection:
				signal.emit(arg1, arg2, arg3, arg4);
				return;
			default:
				break;
        	}
        	QMetaMethod qmethod = qobject.metaObject().methodByIndex(qobject.metaObject().metaObjectPointer, signal.methodIndex());
            if(qmethod!=null && qmethod.isValid()) {
                qmethod.invoke(qobject, type, 
                		arg1, arg2, arg3, arg4, 
                		signal.arg5Default.get());
            }else {
            	invokeMethod(thread, qobject, 
            			(Slot4<A,B,C,D>)signal::emit, 
            			type, arg1, arg2, arg3, arg4);
            }
        }else if(signal.containingObject() instanceof QtThreadAffineInterface) {
        	invokeMethod(((QtThreadAffineInterface)signal.containingObject()).thread(), 
        			(QObject)null, 
        			(Slot4<A,B,C,D>)signal::emit, 
        			type, arg1, arg2, arg3, arg4);
        }else {
        	invokeMethod((QThread)null, 
        			(QObject)null, 
        			(Slot4<A,B,C,D>)signal::emit, 
        			type, arg1, arg2, arg3, arg4);
        }
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
     */
    @QtUninvokable
    public static <A,B,C,D,E> void invokeMethod(@StrictNonNull AbstractSignal5Default2<A,B,C,D,E> signal, A arg1, B arg2, C arg3) throws QUnsuccessfulInvocationException {
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
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param signal invoked signal
     * @param type synchronous or asynchronous invocation
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke signal
     */
    @QtUninvokable
    public static <A,B,C,D,E> void invokeMethod(@StrictNonNull AbstractSignal5Default2<A,B,C,D,E> signal, Qt.@NonNull ConnectionType type, A arg1, B arg2, C arg3) throws QUnsuccessfulInvocationException {
    	if(signal.containingObject() instanceof QObject) {
        	QObject qobject = (QObject)signal.containingObject();
        	QThread thread = qobject.thread();
        	switch(type){
			case AutoConnection:
				if(thread!=null && thread!=QThread.currentThread())
					break;
			case DirectConnection:
				signal.emit(arg1, arg2, arg3);
				return;
			default:
				break;
        	}
        	QMetaMethod qmethod = qobject.metaObject().methodByIndex(qobject.metaObject().metaObjectPointer, signal.methodIndex());
            if(qmethod!=null && qmethod.isValid()) {
                qmethod.invoke(qobject, type, 
                		arg1, arg2, arg3, 
                		signal.arg4Default.get(), 
                		((AbstractSignal5Default1<A,B,C,D,E>)signal).arg5Default.get());
            }else {
            	invokeMethod(thread, qobject, 
            			(Slot3<A,B,C>)signal::emit, 
            			type, arg1, arg2, arg3);
            }
        }else if(signal.containingObject() instanceof QtThreadAffineInterface) {
        	invokeMethod(((QtThreadAffineInterface)signal.containingObject()).thread(), 
        			(QObject)null, 
        			(Slot3<A,B,C>)signal::emit, 
        			type, arg1, arg2, arg3);
        }else {
        	invokeMethod((QThread)null, 
        			(QObject)null, 
        			(Slot3<A,B,C>)signal::emit, 
        			type, arg1, arg2, arg3);
        }
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
     */
    @QtUninvokable
    public static <A,B,C,D,E> void invokeMethod(@StrictNonNull AbstractSignal5Default3<A,B,C,D,E> signal, A arg1, B arg2) throws QUnsuccessfulInvocationException {
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
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param signal invoked signal
     * @param type synchronous or asynchronous invocation
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke signal
     */
    @QtUninvokable
    public static <A,B,C,D,E> void invokeMethod(@StrictNonNull AbstractSignal5Default3<A,B,C,D,E> signal, Qt.@NonNull ConnectionType type, A arg1, B arg2) throws QUnsuccessfulInvocationException {
    	if(signal.containingObject() instanceof QObject) {
        	QObject qobject = (QObject)signal.containingObject();
        	QThread thread = qobject.thread();
        	switch(type){
			case AutoConnection:
				if(thread!=null && thread!=QThread.currentThread())
					break;
			case DirectConnection:
				signal.emit(arg1, arg2);
				return;
			default:
				break;
        	}
        	QMetaMethod qmethod = qobject.metaObject().methodByIndex(qobject.metaObject().metaObjectPointer, signal.methodIndex());
            if(qmethod!=null && qmethod.isValid()) {
                qmethod.invoke(qobject, type, 
                		arg1, arg2, 
                		signal.arg3Default.get(),
                		((AbstractSignal5Default2<A,B,C,D,E>)signal).arg4Default.get(),
                		((AbstractSignal5Default1<A,B,C,D,E>)signal).arg5Default.get());
            }else {
            	invokeMethod(thread, qobject, 
            			(Slot2<A,B>)signal::emit, 
            			type, arg1, arg2);
            }
        }else if(signal.containingObject() instanceof QtThreadAffineInterface) {
        	invokeMethod(((QtThreadAffineInterface)signal.containingObject()).thread(), 
        			(QObject)null, 
        			(Slot2<A,B>)signal::emit, 
        			type, arg1, arg2);
        }else {
        	invokeMethod((QThread)null, 
        			(QObject)null, 
        			(Slot2<A,B>)signal::emit, 
        			type, arg1, arg2);
        }
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
     */
    @QtUninvokable
    public static <A,B,C,D,E> void invokeMethod(@StrictNonNull AbstractSignal5Default4<A,B,C,D,E> signal, A arg1) throws QUnsuccessfulInvocationException {
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
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param signal invoked signal
     * @param type synchronous or asynchronous invocation
     * @param arg1 Argument for the first parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke signal
     */
    @QtUninvokable
    public static <A,B,C,D,E> void invokeMethod(@StrictNonNull AbstractSignal5Default4<A,B,C,D,E> signal, Qt.@NonNull ConnectionType type, A arg1) throws QUnsuccessfulInvocationException {
    	if(signal.containingObject() instanceof QObject) {
        	QObject qobject = (QObject)signal.containingObject();
        	QThread thread = qobject.thread();
        	switch(type){
			case AutoConnection:
				if(thread!=null && thread!=QThread.currentThread())
					break;
			case DirectConnection:
				signal.emit(arg1);
				return;
			default:
				break;
        	}
        	QMetaMethod qmethod = qobject.metaObject().methodByIndex(qobject.metaObject().metaObjectPointer, signal.methodIndex());
            if(qmethod!=null && qmethod.isValid()) {
                qmethod.invoke(qobject, type, 
                		arg1, 
                		signal.arg2Default.get(),
                		((AbstractSignal5Default3<A,B,C,D,E>)signal).arg3Default.get(),
                		((AbstractSignal5Default2<A,B,C,D,E>)signal).arg4Default.get(),
                		((AbstractSignal5Default1<A,B,C,D,E>)signal).arg5Default.get());
            }else {
            	invokeMethod(thread, qobject, 
            			(Slot1<A>)signal::emit, 
            			type, arg1);
            }
        }else if(signal.containingObject() instanceof QtThreadAffineInterface) {
        	invokeMethod(((QtThreadAffineInterface)signal.containingObject()).thread(), 
        			(QObject)null, 
        			(Slot1<A>)signal::emit, 
        			type, arg1);
        }else {
        	invokeMethod((QThread)null, 
        			(QObject)null, 
        			(Slot1<A>)signal::emit, 
        			type, arg1);
        }
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
     */
    @QtUninvokable
    public static <A,B,C,D,E> void invokeMethod(@StrictNonNull AbstractSignal5Default5<A,B,C,D,E> signal) throws QUnsuccessfulInvocationException {
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
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param signal invoked signal
     * @param type synchronous or asynchronous invocation
     * @throws QUnsuccessfulInvocationException if not able to invoke signal
     */
    @QtUninvokable
    public static <A,B,C,D,E> void invokeMethod(@StrictNonNull AbstractSignal5Default5<A,B,C,D,E> signal, Qt.@NonNull ConnectionType type) throws QUnsuccessfulInvocationException {
    	if(signal.containingObject() instanceof QObject) {
        	QObject qobject = (QObject)signal.containingObject();
        	QThread thread = qobject.thread();
        	switch(type){
			case AutoConnection:
				if(thread!=null && thread!=QThread.currentThread())
					break;
			case DirectConnection:
				signal.emit();
				return;
			default:
				break;
        	}
        	QMetaMethod qmethod = qobject.metaObject().methodByIndex(qobject.metaObject().metaObjectPointer, signal.methodIndex());
            if(qmethod!=null && qmethod.isValid()) {
                qmethod.invoke(qobject, type, 
                		signal.arg1Default.get(), 
                		((AbstractSignal5Default4<A,B,C,D,E>)signal).arg2Default.get(), 
                		((AbstractSignal5Default3<A,B,C,D,E>)signal).arg3Default.get(), 
                		((AbstractSignal5Default2<A,B,C,D,E>)signal).arg4Default.get(), 
                		((AbstractSignal5Default1<A,B,C,D,E>)signal).arg5Default.get());
            }else {
            	invokeMethod(thread, qobject, 
            			(Slot0)signal::emit, 
            			type);
            }
        }else if(signal.containingObject() instanceof QtThreadAffineInterface) {
        	invokeMethod(((QtThreadAffineInterface)signal.containingObject()).thread(), 
        			(QObject)null, 
        			(Slot0)signal::emit, 
        			type);
        }else {
        	invokeMethod((QThread)null, 
        			(QObject)null, 
        			(Slot0)signal::emit, 
        			type);
        }
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
     * @param arg6 Argument for the sixth parameter.
     */
    @QtUninvokable
    public static <A,B,C,D,E,F> void invokeMethod(@StrictNonNull AbstractPublicSignal6<A,B,C,D,E,F> signal, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6) throws QUnsuccessfulInvocationException {
        invokeMethod(signal, Qt.ConnectionType.AutoConnection, arg1, arg2, arg3, arg4, arg5, arg6);
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
     * @param arg6 Argument for the sixth parameter.
     */
    @QtUninvokable
    public static <A,B,C,D,E,F> void invokeMethod(@StrictNonNull AbstractPrivateSignal6<A,B,C,D,E,F> signal, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6) throws QUnsuccessfulInvocationException {
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
     * @param type synchronous or asynchronous invocation
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @param arg4 Argument for the fourth parameter.
     * @param arg5 Argument for the fifth parameter.
     * @param arg6 Argument for the sixth parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke signal
     */
    public static <A,B,C,D,E,F> void invokeMethod(@StrictNonNull AbstractPublicSignal6<A,B,C,D,E,F> signal, Qt.@NonNull ConnectionType type, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6) throws QUnsuccessfulInvocationException {
    	if(signal.containingObject() instanceof QObject) {
        	QObject qobject = (QObject)signal.containingObject();
        	QThread thread = qobject.thread();
        	switch(type){
			case AutoConnection:
				if(thread!=null && thread!=QThread.currentThread())
					break;
			case DirectConnection:
				signal.emit(arg1, arg2, arg3, arg4, arg5, arg6);
				return;
			default:
				break;
        	}
        	QMetaMethod qmethod = qobject.metaObject().methodByIndex(qobject.metaObject().metaObjectPointer, signal.methodIndex());
            if(qmethod!=null && qmethod.isValid()) {
                qmethod.invoke(qobject, type, arg1, arg2, arg3, arg4, arg5, arg6);
            }else {
            	invokeMethod(thread, qobject, signal::emit, type, arg1, arg2, arg3, arg4, arg5, arg6);
            }
        }else if(signal.containingObject() instanceof QtThreadAffineInterface) {
        	invokeMethod(((QtThreadAffineInterface)signal.containingObject()).thread(), (QObject)null, signal::emit, type, arg1, arg2, arg3, arg4, arg5, arg6);
        }else {
        	invokeMethod((QThread)null, (QObject)null, signal::emit, type, arg1, arg2, arg3, arg4, arg5, arg6);
        }
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
     * @param type synchronous or asynchronous invocation
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @param arg4 Argument for the fourth parameter.
     * @param arg5 Argument for the fifth parameter.
     * @param arg6 Argument for the sixth parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke signal
     */
    public static <A,B,C,D,E,F> void invokeMethod(@StrictNonNull AbstractPrivateSignal6<A,B,C,D,E,F> signal, Qt.@NonNull ConnectionType type, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6) throws QUnsuccessfulInvocationException {
        if(signal.containingObject() instanceof QObject && !((QObject)signal.containingObject()).isDisposed()) {
            QObject qobject = (QObject)signal.containingObject();
            QMetaMethod qmethod = qobject.metaObject().methodByIndex(qobject.metaObject().metaObjectPointer, signal.methodIndex());
            if(qmethod!=null && qmethod.isValid()) {
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
     * @param signal invoked signal
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @param arg4 Argument for the fourth parameter.
     * @param arg5 Argument for the fifth parameter.
     */
    @QtUninvokable
    public static <A,B,C,D,E,F> void invokeMethod(@StrictNonNull AbstractSignal6Default1<A,B,C,D,E,F> signal, A arg1, B arg2, C arg3, D arg4, E arg5) throws QUnsuccessfulInvocationException {
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
     * @param <F> The type of the sixth parameter of the signal.
     * @param signal invoked signal
     * @param type synchronous or asynchronous invocation
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @param arg4 Argument for the fourth parameter.
     * @param arg5 Argument for the fifth parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke signal
     */
    public static <A,B,C,D,E,F> void invokeMethod(@StrictNonNull AbstractSignal6Default1<A,B,C,D,E,F> signal, Qt.@NonNull ConnectionType type, A arg1, B arg2, C arg3, D arg4, E arg5) throws QUnsuccessfulInvocationException {
    	if(signal.containingObject() instanceof QObject) {
        	QObject qobject = (QObject)signal.containingObject();
        	QThread thread = qobject.thread();
        	switch(type){
			case AutoConnection:
				if(thread!=null && thread!=QThread.currentThread())
					break;
			case DirectConnection:
				signal.emit(arg1, arg2, arg3, arg4, arg5);
				return;
			default:
				break;
        	}
        	QMetaMethod qmethod = qobject.metaObject().methodByIndex(qobject.metaObject().metaObjectPointer, signal.methodIndex());
            if(qmethod!=null && qmethod.isValid()) {
                qmethod.invoke(qobject, type, 
                		arg1, arg2, arg3, arg4, arg5, 
                		signal.arg6Default.get());
            }else {
            	invokeMethod(thread, qobject, 
            			(Slot5<A,B,C,D,E>)signal::emit, 
            			type, arg1, arg2, arg3, arg4, arg5);
            }
        }else if(signal.containingObject() instanceof QtThreadAffineInterface) {
        	invokeMethod(((QtThreadAffineInterface)signal.containingObject()).thread(), 
        			(QObject)null, 
        			(Slot5<A,B,C,D,E>)signal::emit, 
        			type, arg1, arg2, arg3, arg4, arg5);
        }else {
        	invokeMethod((QThread)null, 
        			(QObject)null, 
        			(Slot5<A,B,C,D,E>)signal::emit, 
        			type, arg1, arg2, arg3, arg4, arg5);
        }
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
     */
    @QtUninvokable
    public static <A,B,C,D,E,F> void invokeMethod(@StrictNonNull AbstractSignal6Default2<A,B,C,D,E,F> signal, A arg1, B arg2, C arg3, D arg4) throws QUnsuccessfulInvocationException {
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
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param signal invoked signal
     * @param type synchronous or asynchronous invocation
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @param arg4 Argument for the fourth parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke signal
     */
    public static <A,B,C,D,E,F> void invokeMethod(@StrictNonNull AbstractSignal6Default2<A,B,C,D,E,F> signal, Qt.@NonNull ConnectionType type, A arg1, B arg2, C arg3, D arg4) throws QUnsuccessfulInvocationException {
    	if(signal.containingObject() instanceof QObject) {
        	QObject qobject = (QObject)signal.containingObject();
        	QThread thread = qobject.thread();
        	switch(type){
			case AutoConnection:
				if(thread!=null && thread!=QThread.currentThread())
					break;
			case DirectConnection:
				signal.emit(arg1, arg2, arg3, arg4);
				return;
			default:
				break;
        	}
        	QMetaMethod qmethod = qobject.metaObject().methodByIndex(qobject.metaObject().metaObjectPointer, signal.methodIndex());
            if(qmethod!=null && qmethod.isValid()) {
                qmethod.invoke(qobject, type, 
                		arg1, arg2, arg3, arg4, 
                		signal.arg5Default.get(), 
                		((AbstractSignal6Default1<A,B,C,D,E,F>)signal).arg6Default.get());
            }else {
            	invokeMethod(thread, qobject, 
            			(Slot4<A,B,C,D>)signal::emit, 
            			type, arg1, arg2, arg3, arg4);
            }
        }else if(signal.containingObject() instanceof QtThreadAffineInterface) {
        	invokeMethod(((QtThreadAffineInterface)signal.containingObject()).thread(), 
        			(QObject)null, 
        			(Slot4<A,B,C,D>)signal::emit, 
        			type, arg1, arg2, arg3, arg4);
        }else {
        	invokeMethod((QThread)null, 
        			(QObject)null, 
        			(Slot4<A,B,C,D>)signal::emit, 
        			type, arg1, arg2, arg3, arg4);
        }
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
     */
    @QtUninvokable
    public static <A,B,C,D,E,F> void invokeMethod(@StrictNonNull AbstractSignal6Default3<A,B,C,D,E,F> signal, A arg1, B arg2, C arg3) throws QUnsuccessfulInvocationException {
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
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param signal invoked signal
     * @param type synchronous or asynchronous invocation
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke signal
     */
    public static <A,B,C,D,E,F> void invokeMethod(@StrictNonNull AbstractSignal6Default3<A,B,C,D,E,F> signal, Qt.@NonNull ConnectionType type, A arg1, B arg2, C arg3) throws QUnsuccessfulInvocationException {
    	if(signal.containingObject() instanceof QObject) {
        	QObject qobject = (QObject)signal.containingObject();
        	QThread thread = qobject.thread();
        	switch(type){
			case AutoConnection:
				if(thread!=null && thread!=QThread.currentThread())
					break;
			case DirectConnection:
				signal.emit(arg1, arg2, arg3);
				return;
			default:
				break;
        	}
        	QMetaMethod qmethod = qobject.metaObject().methodByIndex(qobject.metaObject().metaObjectPointer, signal.methodIndex());
            if(qmethod!=null && qmethod.isValid()) {
                qmethod.invoke(qobject, type, 
                		arg1, arg2, arg3, 
                		signal.arg4Default.get(),
                		((AbstractSignal6Default2<A,B,C,D,E,F>)signal).arg5Default.get(),
                		((AbstractSignal6Default1<A,B,C,D,E,F>)signal).arg6Default.get());
            }else {
            	invokeMethod(thread, qobject, 
            			(Slot3<A,B,C>)signal::emit, 
            			type, arg1, arg2, arg3);
            }
        }else if(signal.containingObject() instanceof QtThreadAffineInterface) {
        	invokeMethod(((QtThreadAffineInterface)signal.containingObject()).thread(), 
        			(QObject)null, 
        			(Slot3<A,B,C>)signal::emit, 
        			type, arg1, arg2, arg3);
        }else {
        	invokeMethod((QThread)null, 
        			(QObject)null, 
        			(Slot3<A,B,C>)signal::emit, 
        			type, arg1, arg2, arg3);
        }
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
     */
    @QtUninvokable
    public static <A,B,C,D,E,F> void invokeMethod(@StrictNonNull AbstractSignal6Default4<A,B,C,D,E,F> signal, A arg1, B arg2) throws QUnsuccessfulInvocationException {
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
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param signal invoked signal
     * @param type synchronous or asynchronous invocation
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke signal
     */
    public static <A,B,C,D,E,F> void invokeMethod(@StrictNonNull AbstractSignal6Default4<A,B,C,D,E,F> signal, Qt.@NonNull ConnectionType type, A arg1, B arg2) throws QUnsuccessfulInvocationException {
    	if(signal.containingObject() instanceof QObject) {
        	QObject qobject = (QObject)signal.containingObject();
        	QThread thread = qobject.thread();
        	switch(type){
			case AutoConnection:
				if(thread!=null && thread!=QThread.currentThread())
					break;
			case DirectConnection:
				signal.emit(arg1, arg2);
				return;
			default:
				break;
        	}
        	QMetaMethod qmethod = qobject.metaObject().methodByIndex(qobject.metaObject().metaObjectPointer, signal.methodIndex());
            if(qmethod!=null && qmethod.isValid()) {
                qmethod.invoke(qobject, type, 
                		arg1, arg2, 
                		signal.arg3Default.get(),
                		((AbstractSignal6Default3<A,B,C,D,E,F>)signal).arg4Default.get(),
                		((AbstractSignal6Default2<A,B,C,D,E,F>)signal).arg5Default.get(),
                		((AbstractSignal6Default1<A,B,C,D,E,F>)signal).arg6Default.get());
            }else {
            	invokeMethod(thread, qobject, 
            			(Slot2<A,B>)signal::emit, 
            			type, arg1, arg2);
            }
        }else if(signal.containingObject() instanceof QtThreadAffineInterface) {
        	invokeMethod(((QtThreadAffineInterface)signal.containingObject()).thread(), 
        			(QObject)null, 
        			(Slot2<A,B>)signal::emit, 
        			type, arg1, arg2);
        }else {
        	invokeMethod((QThread)null, 
        			(QObject)null, 
        			(Slot2<A,B>)signal::emit, 
        			type, arg1, arg2);
        }
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
     */
    @QtUninvokable
    public static <A,B,C,D,E,F> void invokeMethod(@StrictNonNull AbstractSignal6Default5<A,B,C,D,E,F> signal, A arg1) throws QUnsuccessfulInvocationException {
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
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param signal invoked signal
     * @param type synchronous or asynchronous invocation
     * @param arg1 Argument for the first parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke signal
     */
    public static <A,B,C,D,E,F> void invokeMethod(@StrictNonNull AbstractSignal6Default5<A,B,C,D,E,F> signal, Qt.@NonNull ConnectionType type, A arg1) throws QUnsuccessfulInvocationException {
    	if(signal.containingObject() instanceof QObject) {
        	QObject qobject = (QObject)signal.containingObject();
        	QThread thread = qobject.thread();
        	switch(type){
			case AutoConnection:
				if(thread!=null && thread!=QThread.currentThread())
					break;
			case DirectConnection:
				signal.emit(arg1);
				return;
			default:
				break;
        	}
        	QMetaMethod qmethod = qobject.metaObject().methodByIndex(qobject.metaObject().metaObjectPointer, signal.methodIndex());
            if(qmethod!=null && qmethod.isValid()) {
                qmethod.invoke(qobject, type, 
                		arg1, 
                		signal.arg2Default.get(),
                		((AbstractSignal6Default4<A,B,C,D,E,F>)signal).arg3Default.get(),
                		((AbstractSignal6Default3<A,B,C,D,E,F>)signal).arg4Default.get(),
                		((AbstractSignal6Default2<A,B,C,D,E,F>)signal).arg5Default.get(),
                		((AbstractSignal6Default1<A,B,C,D,E,F>)signal).arg6Default.get());
            }else {
            	invokeMethod(thread, qobject, 
            			(Slot1<A>)signal::emit, 
            			type, arg1);
            }
        }else if(signal.containingObject() instanceof QtThreadAffineInterface) {
        	invokeMethod(((QtThreadAffineInterface)signal.containingObject()).thread(), 
        			(QObject)null, 
        			(Slot1<A>)signal::emit, 
        			type, arg1);
        }else {
        	invokeMethod((QThread)null, 
        			(QObject)null, 
        			(Slot1<A>)signal::emit, 
        			type, arg1);
        }
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
     */
    @QtUninvokable
    public static <A,B,C,D,E,F> void invokeMethod(@StrictNonNull AbstractSignal6Default6<A,B,C,D,E,F> signal) throws QUnsuccessfulInvocationException {
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
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param signal invoked signal
     * @param type synchronous or asynchronous invocation
     * @throws QUnsuccessfulInvocationException if not able to invoke signal
     */
    public static <A,B,C,D,E,F> void invokeMethod(@StrictNonNull AbstractSignal6Default6<A,B,C,D,E,F> signal, Qt.@NonNull ConnectionType type) throws QUnsuccessfulInvocationException {
    	if(signal.containingObject() instanceof QObject) {
        	QObject qobject = (QObject)signal.containingObject();
        	QThread thread = qobject.thread();
        	switch(type){
			case AutoConnection:
				if(thread!=null && thread!=QThread.currentThread())
					break;
			case DirectConnection:
				signal.emit();
				return;
			default:
				break;
        	}
        	QMetaMethod qmethod = qobject.metaObject().methodByIndex(qobject.metaObject().metaObjectPointer, signal.methodIndex());
            if(qmethod!=null && qmethod.isValid()) {
                qmethod.invoke(qobject, type, 
                		signal.arg1Default.get(), 
                		((AbstractSignal6Default5<A,B,C,D,E,F>)signal).arg2Default.get(), 
                		((AbstractSignal6Default4<A,B,C,D,E,F>)signal).arg3Default.get(), 
                		((AbstractSignal6Default3<A,B,C,D,E,F>)signal).arg4Default.get(), 
                		((AbstractSignal6Default2<A,B,C,D,E,F>)signal).arg5Default.get(), 
                		((AbstractSignal6Default1<A,B,C,D,E,F>)signal).arg6Default.get());
            }else {
            	invokeMethod(thread, qobject, 
            			(Slot0)signal::emit, 
            			type);
            }
        }else if(signal.containingObject() instanceof QtThreadAffineInterface) {
        	invokeMethod(((QtThreadAffineInterface)signal.containingObject()).thread(), 
        			(QObject)null, 
        			(Slot0)signal::emit, 
        			type);
        }else {
        	invokeMethod((QThread)null, 
        			(QObject)null, 
        			(Slot0)signal::emit, 
        			type);
        }
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
     * @param arg7 Argument for the seventh parameter.
     */
    @QtUninvokable
    public static <A,B,C,D,E,F,G> void invokeMethod(@StrictNonNull AbstractPublicSignal7<A,B,C,D,E,F,G> signal, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7) throws QUnsuccessfulInvocationException {
        invokeMethod(signal, Qt.ConnectionType.AutoConnection, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
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
     * @param arg7 Argument for the seventh parameter.
     */
    @QtUninvokable
    public static <A,B,C,D,E,F,G> void invokeMethod(@StrictNonNull AbstractPrivateSignal7<A,B,C,D,E,F,G> signal, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7) throws QUnsuccessfulInvocationException {
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
     * @param type synchronous or asynchronous invocation
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
    public static <A,B,C,D,E,F,G> void invokeMethod(@StrictNonNull AbstractPublicSignal7<A,B,C,D,E,F,G> signal, Qt.@NonNull ConnectionType type, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7) throws QUnsuccessfulInvocationException {
    	if(signal.containingObject() instanceof QObject) {
        	QObject qobject = (QObject)signal.containingObject();
        	QThread thread = qobject.thread();
        	switch(type){
			case AutoConnection:
				if(thread!=null && thread!=QThread.currentThread())
					break;
			case DirectConnection:
				signal.emit(arg1, arg2, arg3, arg4, arg5, arg6, arg7);
				return;
			default:
				break;
        	}
        	QMetaMethod qmethod = qobject.metaObject().methodByIndex(qobject.metaObject().metaObjectPointer, signal.methodIndex());
            if(qmethod!=null && qmethod.isValid()) {
                qmethod.invoke(qobject, type, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
            }else {
            	invokeMethod(thread, qobject, signal::emit, type, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
            }
        }else if(signal.containingObject() instanceof QtThreadAffineInterface) {
        	invokeMethod(((QtThreadAffineInterface)signal.containingObject()).thread(), (QObject)null, signal::emit, type, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
        }else {
        	invokeMethod((QThread)null, (QObject)null, signal::emit, type, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
        }
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
     * @param type synchronous or asynchronous invocation
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
    public static <A,B,C,D,E,F,G> void invokeMethod(@StrictNonNull AbstractPrivateSignal7<A,B,C,D,E,F,G> signal, Qt.@NonNull ConnectionType type, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7) throws QUnsuccessfulInvocationException {
        if(signal.containingObject() instanceof QObject && !((QObject)signal.containingObject()).isDisposed()) {
            QObject qobject = (QObject)signal.containingObject();
            QMetaMethod qmethod = qobject.metaObject().methodByIndex(qobject.metaObject().metaObjectPointer, signal.methodIndex());
            if(qmethod!=null && qmethod.isValid()) {
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
     * @param signal invoked signal
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @param arg4 Argument for the fourth parameter.
     * @param arg5 Argument for the fifth parameter.
     * @param arg6 Argument for the sixth parameter.
     */
    @QtUninvokable
    public static <A,B,C,D,E,F,G> void invokeMethod(@StrictNonNull AbstractSignal7Default1<A,B,C,D,E,F,G> signal, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6) throws QUnsuccessfulInvocationException {
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
     * @param <G> The type of the seventh parameter of the signal.
     * @param signal invoked signal
     * @param type synchronous or asynchronous invocation
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @param arg4 Argument for the fourth parameter.
     * @param arg5 Argument for the fifth parameter.
     * @param arg6 Argument for the sixth parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke signal
     */
    @QtUninvokable
    public static <A,B,C,D,E,F,G> void invokeMethod(@StrictNonNull AbstractSignal7Default1<A,B,C,D,E,F,G> signal, Qt.@NonNull ConnectionType type, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6) throws QUnsuccessfulInvocationException {
    	if(signal.containingObject() instanceof QObject) {
        	QObject qobject = (QObject)signal.containingObject();
        	QThread thread = qobject.thread();
        	switch(type){
			case AutoConnection:
				if(thread!=null && thread!=QThread.currentThread())
					break;
			case DirectConnection:
				signal.emit(arg1, arg2, arg3, arg4, arg5, arg6);
				return;
			default:
				break;
        	}
        	QMetaMethod qmethod = qobject.metaObject().methodByIndex(qobject.metaObject().metaObjectPointer, signal.methodIndex());
            if(qmethod!=null && qmethod.isValid()) {
                qmethod.invoke(qobject, type, 
                		arg1, arg2, arg3, arg4, arg5, arg6, 
                		signal.arg7Default.get());
            }else {
            	invokeMethod(thread, qobject, 
            			(Slot6<A,B,C,D,E,F>)signal::emit, 
            			type, arg1, arg2, arg3, arg4, arg5, arg6);
            }
        }else if(signal.containingObject() instanceof QtThreadAffineInterface) {
        	invokeMethod(((QtThreadAffineInterface)signal.containingObject()).thread(), 
        			(QObject)null, 
        			(Slot6<A,B,C,D,E,F>)signal::emit, 
        			type, arg1, arg2, arg3, arg4, arg5, arg6);
        }else {
        	invokeMethod((QThread)null, 
        			(QObject)null, 
        			(Slot6<A,B,C,D,E,F>)signal::emit, 
        			type, arg1, arg2, arg3, arg4, arg5, arg6);
        }
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
     */
    @QtUninvokable
    public static <A,B,C,D,E,F,G> void invokeMethod(@StrictNonNull AbstractSignal7Default2<A,B,C,D,E,F,G> signal, A arg1, B arg2, C arg3, D arg4, E arg5) throws QUnsuccessfulInvocationException {
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
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     * @param signal invoked signal
     * @param type synchronous or asynchronous invocation
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @param arg4 Argument for the fourth parameter.
     * @param arg5 Argument for the fifth parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke signal
     */
    @QtUninvokable
    public static <A,B,C,D,E,F,G> void invokeMethod(@StrictNonNull AbstractSignal7Default2<A,B,C,D,E,F,G> signal, Qt.@NonNull ConnectionType type, A arg1, B arg2, C arg3, D arg4, E arg5) throws QUnsuccessfulInvocationException {
    	if(signal.containingObject() instanceof QObject) {
        	QObject qobject = (QObject)signal.containingObject();
        	QThread thread = qobject.thread();
        	switch(type){
			case AutoConnection:
				if(thread!=null && thread!=QThread.currentThread())
					break;
			case DirectConnection:
				signal.emit(arg1, arg2, arg3, arg4, arg5);
				return;
			default:
				break;
        	}
        	QMetaMethod qmethod = qobject.metaObject().methodByIndex(qobject.metaObject().metaObjectPointer, signal.methodIndex());
            if(qmethod!=null && qmethod.isValid()) {
                qmethod.invoke(qobject, type, 
                		arg1, arg2, arg3, arg4, arg5, 
                		signal.arg6Default.get(), 
                		((AbstractSignal7Default1<A,B,C,D,E,F,G>)signal).arg7Default.get());
            }else {
            	invokeMethod(thread, qobject, 
            			(Slot5<A,B,C,D,E>)signal::emit, 
            			type, arg1, arg2, arg3, arg4, arg5);
            }
        }else if(signal.containingObject() instanceof QtThreadAffineInterface) {
        	invokeMethod(((QtThreadAffineInterface)signal.containingObject()).thread(), 
        			(QObject)null, 
        			(Slot5<A,B,C,D,E>)signal::emit, 
        			type, arg1, arg2, arg3, arg4, arg5);
        }else {
        	invokeMethod((QThread)null, 
        			(QObject)null, 
        			(Slot5<A,B,C,D,E>)signal::emit, 
        			type, arg1, arg2, arg3, arg4, arg5);
        }
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
     */
    @QtUninvokable
    public static <A,B,C,D,E,F,G> void invokeMethod(@StrictNonNull AbstractSignal7Default3<A,B,C,D,E,F,G> signal, A arg1, B arg2, C arg3, D arg4) throws QUnsuccessfulInvocationException {
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
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     * @param signal invoked signal
     * @param type synchronous or asynchronous invocation
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @param arg4 Argument for the fourth parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke signal
     */
    @QtUninvokable
    public static <A,B,C,D,E,F,G> void invokeMethod(@StrictNonNull AbstractSignal7Default3<A,B,C,D,E,F,G> signal, Qt.@NonNull ConnectionType type, A arg1, B arg2, C arg3, D arg4) throws QUnsuccessfulInvocationException {
    	if(signal.containingObject() instanceof QObject) {
        	QObject qobject = (QObject)signal.containingObject();
        	QThread thread = qobject.thread();
        	switch(type){
			case AutoConnection:
				if(thread!=null && thread!=QThread.currentThread())
					break;
			case DirectConnection:
				signal.emit(arg1, arg2, arg3, arg4);
				return;
			default:
				break;
        	}
        	QMetaMethod qmethod = qobject.metaObject().methodByIndex(qobject.metaObject().metaObjectPointer, signal.methodIndex());
            if(qmethod!=null && qmethod.isValid()) {
                qmethod.invoke(qobject, type, 
                		arg1, arg2, arg3, arg4, 
                		signal.arg5Default.get(),
                		((AbstractSignal7Default2<A,B,C,D,E,F,G>)signal).arg6Default.get(),
                		((AbstractSignal7Default1<A,B,C,D,E,F,G>)signal).arg7Default.get());
            }else {
            	invokeMethod(thread, qobject, 
            			(Slot4<A,B,C,D>)signal::emit, 
            			type, arg1, arg2, arg3, arg4);
            }
        }else if(signal.containingObject() instanceof QtThreadAffineInterface) {
        	invokeMethod(((QtThreadAffineInterface)signal.containingObject()).thread(), 
        			(QObject)null, 
        			(Slot4<A,B,C,D>)signal::emit, 
        			type, arg1, arg2, arg3, arg4);
        }else {
        	invokeMethod((QThread)null, 
        			(QObject)null, 
        			(Slot4<A,B,C,D>)signal::emit, 
        			type, arg1, arg2, arg3, arg4);
        }
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
     */
    @QtUninvokable
    public static <A,B,C,D,E,F,G> void invokeMethod(@StrictNonNull AbstractSignal7Default4<A,B,C,D,E,F,G> signal, A arg1, B arg2, C arg3) throws QUnsuccessfulInvocationException {
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
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     * @param signal invoked signal
     * @param type synchronous or asynchronous invocation
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke signal
     */
    @QtUninvokable
    public static <A,B,C,D,E,F,G> void invokeMethod(@StrictNonNull AbstractSignal7Default4<A,B,C,D,E,F,G> signal, Qt.@NonNull ConnectionType type, A arg1, B arg2, C arg3) throws QUnsuccessfulInvocationException {
    	if(signal.containingObject() instanceof QObject) {
        	QObject qobject = (QObject)signal.containingObject();
        	QThread thread = qobject.thread();
        	switch(type){
			case AutoConnection:
				if(thread!=null && thread!=QThread.currentThread())
					break;
			case DirectConnection:
				signal.emit(arg1, arg2, arg3);
				return;
			default:
				break;
        	}
        	QMetaMethod qmethod = qobject.metaObject().methodByIndex(qobject.metaObject().metaObjectPointer, signal.methodIndex());
            if(qmethod!=null && qmethod.isValid()) {
                qmethod.invoke(qobject, type, 
                		arg1, arg2, arg3, 
                		signal.arg4Default.get(),
                		((AbstractSignal7Default3<A,B,C,D,E,F,G>)signal).arg5Default.get(),
                		((AbstractSignal7Default2<A,B,C,D,E,F,G>)signal).arg6Default.get(),
                		((AbstractSignal7Default1<A,B,C,D,E,F,G>)signal).arg7Default.get());
            }else {
            	invokeMethod(thread, qobject, 
            			(Slot3<A,B,C>)signal::emit, 
            			type, arg1, arg2, arg3);
            }
        }else if(signal.containingObject() instanceof QtThreadAffineInterface) {
        	invokeMethod(((QtThreadAffineInterface)signal.containingObject()).thread(), 
        			(QObject)null, 
        			(Slot3<A,B,C>)signal::emit, 
        			type, arg1, arg2, arg3);
        }else {
        	invokeMethod((QThread)null, 
        			(QObject)null, 
        			(Slot3<A,B,C>)signal::emit, 
        			type, arg1, arg2, arg3);
        }
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
     */
    @QtUninvokable
    public static <A,B,C,D,E,F,G> void invokeMethod(@StrictNonNull AbstractSignal7Default5<A,B,C,D,E,F,G> signal, A arg1, B arg2) throws QUnsuccessfulInvocationException {
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
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     * @param signal invoked signal
     * @param type synchronous or asynchronous invocation
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke signal
     */
    @QtUninvokable
    public static <A,B,C,D,E,F,G> void invokeMethod(@StrictNonNull AbstractSignal7Default5<A,B,C,D,E,F,G> signal, Qt.@NonNull ConnectionType type, A arg1, B arg2) throws QUnsuccessfulInvocationException {
    	if(signal.containingObject() instanceof QObject) {
        	QObject qobject = (QObject)signal.containingObject();
        	QThread thread = qobject.thread();
        	switch(type){
			case AutoConnection:
				if(thread!=null && thread!=QThread.currentThread())
					break;
			case DirectConnection:
				signal.emit(arg1, arg2);
				return;
			default:
				break;
        	}
        	QMetaMethod qmethod = qobject.metaObject().methodByIndex(qobject.metaObject().metaObjectPointer, signal.methodIndex());
            if(qmethod!=null && qmethod.isValid()) {
                qmethod.invoke(qobject, type, 
                		arg1, arg2, 
                		signal.arg3Default.get(),
                		((AbstractSignal7Default4<A,B,C,D,E,F,G>)signal).arg4Default.get(),
                		((AbstractSignal7Default3<A,B,C,D,E,F,G>)signal).arg5Default.get(),
                		((AbstractSignal7Default2<A,B,C,D,E,F,G>)signal).arg6Default.get(),
                		((AbstractSignal7Default1<A,B,C,D,E,F,G>)signal).arg7Default.get());
            }else {
            	invokeMethod(thread, qobject, 
            			(Slot2<A,B>)signal::emit, 
            			type, arg1, arg2);
            }
        }else if(signal.containingObject() instanceof QtThreadAffineInterface) {
        	invokeMethod(((QtThreadAffineInterface)signal.containingObject()).thread(), 
        			(QObject)null, 
        			(Slot2<A,B>)signal::emit, 
        			type, arg1, arg2);
        }else {
        	invokeMethod((QThread)null, 
        			(QObject)null, 
        			(Slot2<A,B>)signal::emit, 
        			type, arg1, arg2);
        }
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
     */
    @QtUninvokable
    public static <A,B,C,D,E,F,G> void invokeMethod(@StrictNonNull AbstractSignal7Default6<A,B,C,D,E,F,G> signal, A arg1) throws QUnsuccessfulInvocationException {
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
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     * @param signal invoked signal
     * @param type synchronous or asynchronous invocation
     * @param arg1 Argument for the first parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke signal
     */
    @QtUninvokable
    public static <A,B,C,D,E,F,G> void invokeMethod(@StrictNonNull AbstractSignal7Default6<A,B,C,D,E,F,G> signal, Qt.@NonNull ConnectionType type, A arg1) throws QUnsuccessfulInvocationException {
    	if(signal.containingObject() instanceof QObject) {
        	QObject qobject = (QObject)signal.containingObject();
        	QThread thread = qobject.thread();
        	switch(type){
			case AutoConnection:
				if(thread!=null && thread!=QThread.currentThread())
					break;
			case DirectConnection:
				signal.emit(arg1);
				return;
			default:
				break;
        	}
        	QMetaMethod qmethod = qobject.metaObject().methodByIndex(qobject.metaObject().metaObjectPointer, signal.methodIndex());
            if(qmethod!=null && qmethod.isValid()) {
                qmethod.invoke(qobject, type, 
                		arg1, 
                		signal.arg2Default.get(),
                		((AbstractSignal7Default5<A,B,C,D,E,F,G>)signal).arg3Default.get(),
                		((AbstractSignal7Default4<A,B,C,D,E,F,G>)signal).arg4Default.get(),
                		((AbstractSignal7Default3<A,B,C,D,E,F,G>)signal).arg5Default.get(),
                		((AbstractSignal7Default2<A,B,C,D,E,F,G>)signal).arg6Default.get(),
                		((AbstractSignal7Default1<A,B,C,D,E,F,G>)signal).arg7Default.get());
            }else {
            	invokeMethod(thread, qobject, 
            			(Slot1<A>)signal::emit, 
            			type, arg1);
            }
        }else if(signal.containingObject() instanceof QtThreadAffineInterface) {
        	invokeMethod(((QtThreadAffineInterface)signal.containingObject()).thread(), 
        			(QObject)null, 
        			(Slot1<A>)signal::emit, 
        			type, arg1);
        }else {
        	invokeMethod((QThread)null, 
        			(QObject)null, 
        			(Slot1<A>)signal::emit, 
        			type, arg1);
        }
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
     */
    @QtUninvokable
    public static <A,B,C,D,E,F,G> void invokeMethod(@StrictNonNull AbstractSignal7Default7<A,B,C,D,E,F,G> signal) throws QUnsuccessfulInvocationException {
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
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     * @param signal invoked signal
     * @param type synchronous or asynchronous invocation
     * @throws QUnsuccessfulInvocationException if not able to invoke signal
     */
    @QtUninvokable
    public static <A,B,C,D,E,F,G> void invokeMethod(@StrictNonNull AbstractSignal7Default7<A,B,C,D,E,F,G> signal, Qt.@NonNull ConnectionType type) throws QUnsuccessfulInvocationException {
    	if(signal.containingObject() instanceof QObject) {
        	QObject qobject = (QObject)signal.containingObject();
        	QThread thread = qobject.thread();
        	switch(type){
			case AutoConnection:
				if(thread!=null && thread!=QThread.currentThread())
					break;
			case DirectConnection:
				signal.emit();
				return;
			default:
				break;
        	}
        	QMetaMethod qmethod = qobject.metaObject().methodByIndex(qobject.metaObject().metaObjectPointer, signal.methodIndex());
            if(qmethod!=null && qmethod.isValid()) {
                qmethod.invoke(qobject, type, 
                		signal.arg1Default.get(), 
                		((AbstractSignal7Default6<A,B,C,D,E,F,G>)signal).arg2Default.get(), 
                		((AbstractSignal7Default5<A,B,C,D,E,F,G>)signal).arg3Default.get(), 
                		((AbstractSignal7Default4<A,B,C,D,E,F,G>)signal).arg4Default.get(), 
                		((AbstractSignal7Default3<A,B,C,D,E,F,G>)signal).arg5Default.get(), 
                		((AbstractSignal7Default2<A,B,C,D,E,F,G>)signal).arg6Default.get(), 
                		((AbstractSignal7Default1<A,B,C,D,E,F,G>)signal).arg7Default.get());
            }else {
            	invokeMethod(thread, qobject, 
            			(Slot0)signal::emit, 
            			type);
            }
        }else if(signal.containingObject() instanceof QtThreadAffineInterface) {
        	invokeMethod(((QtThreadAffineInterface)signal.containingObject()).thread(), 
        			(QObject)null, 
        			(Slot0)signal::emit, 
        			type);
        }else {
        	invokeMethod((QThread)null, 
        			(QObject)null, 
        			(Slot0)signal::emit, 
        			type);
        }
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
     * @param arg8 Argument for the eighth parameter.
     */
    @QtUninvokable
    public static <A,B,C,D,E,F,G,H> void invokeMethod(@StrictNonNull AbstractPrivateSignal8<A,B,C,D,E,F,G,H> signal, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7, H arg8) throws QUnsuccessfulInvocationException {
        invokeMethod(signal, Qt.ConnectionType.AutoConnection, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
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
     * @param arg8 Argument for the eighth parameter.
     */
    @QtUninvokable
    public static <A,B,C,D,E,F,G,H> void invokeMethod(@StrictNonNull AbstractPublicSignal8<A,B,C,D,E,F,G,H> signal, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7, H arg8) throws QUnsuccessfulInvocationException {
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
     * @param type synchronous or asynchronous invocation
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
    public static <A,B,C,D,E,F,G,H> void invokeMethod(@StrictNonNull AbstractPrivateSignal8<A,B,C,D,E,F,G,H> signal, Qt.@NonNull ConnectionType type, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7, H arg8) throws QUnsuccessfulInvocationException {
        if(signal.containingObject() instanceof QObject && !((QObject)signal.containingObject()).isDisposed()) {
            QObject qobject = (QObject)signal.containingObject();
            QMetaMethod qmethod = qobject.metaObject().methodByIndex(qobject.metaObject().metaObjectPointer, signal.methodIndex());
            if(qmethod!=null && qmethod.isValid()) {
                qmethod.invoke(qobject, type, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
                return;
            }
        }
        throw new QUnsuccessfulInvocationException("Unable to invoke method.");
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
     * @param type synchronous or asynchronous invocation
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
    public static <A,B,C,D,E,F,G,H> void invokeMethod(@StrictNonNull AbstractPublicSignal8<A,B,C,D,E,F,G,H> signal, Qt.@NonNull ConnectionType type, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7, H arg8) throws QUnsuccessfulInvocationException {
    	if(signal.containingObject() instanceof QObject) {
        	QObject qobject = (QObject)signal.containingObject();
        	QThread thread = qobject.thread();
        	switch(type){
			case AutoConnection:
				if(thread!=null && thread!=QThread.currentThread())
					break;
			case DirectConnection:
				signal.emit(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
				return;
			default:
				break;
        	}
        	QMetaMethod qmethod = qobject.metaObject().methodByIndex(qobject.metaObject().metaObjectPointer, signal.methodIndex());
            if(qmethod!=null && qmethod.isValid()) {
                qmethod.invoke(qobject, type, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
            }else {
            	invokeMethod(thread, qobject, signal::emit, type, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
            }
        }else if(signal.containingObject() instanceof QtThreadAffineInterface) {
        	invokeMethod(((QtThreadAffineInterface)signal.containingObject()).thread(), (QObject)null, signal::emit, type, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
        }else {
        	invokeMethod((QThread)null, (QObject)null, signal::emit, type, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
        }
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
    public static <A,B,C,D,E,F,G,H> void invokeMethod(@StrictNonNull AbstractSignal8Default1<A,B,C,D,E,F,G,H> signal, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7) throws QUnsuccessfulInvocationException {
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
     * @param <H> The type of the eighth parameter of the signal.
     * @param signal invoked signal
     * @param type synchronous or asynchronous invocation
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
    public static <A,B,C,D,E,F,G,H> void invokeMethod(@StrictNonNull AbstractSignal8Default1<A,B,C,D,E,F,G,H> signal, Qt.@NonNull ConnectionType type, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7) throws QUnsuccessfulInvocationException {
    	if(signal.containingObject() instanceof QObject) {
        	QObject qobject = (QObject)signal.containingObject();
        	QThread thread = qobject.thread();
        	switch(type){
			case AutoConnection:
				if(thread!=null && thread!=QThread.currentThread())
					break;
			case DirectConnection:
				signal.emit(arg1, arg2, arg3, arg4, arg5, arg6, arg7);
				return;
			default:
				break;
        	}
        	QMetaMethod qmethod = qobject.metaObject().methodByIndex(qobject.metaObject().metaObjectPointer, signal.methodIndex());
            if(qmethod!=null && qmethod.isValid()) {
                qmethod.invoke(qobject, type, 
                		arg1, arg2, arg3, arg4, arg5, arg6, arg7, 
                		signal.arg8Default.get());
            }else {
            	invokeMethod(thread, qobject, 
            			(Slot7<A,B,C,D,E,F,G>)signal::emit, 
            			type, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
            }
        }else if(signal.containingObject() instanceof QtThreadAffineInterface) {
        	invokeMethod(((QtThreadAffineInterface)signal.containingObject()).thread(), 
        			(QObject)null, 
        			(Slot7<A,B,C,D,E,F,G>)signal::emit, 
        			type, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
        }else {
        	invokeMethod((QThread)null, 
        			(QObject)null, 
        			(Slot7<A,B,C,D,E,F,G>)signal::emit, 
        			type, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
        }
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
     */
    @QtUninvokable
    public static <A,B,C,D,E,F,G,H> void invokeMethod(@StrictNonNull AbstractSignal8Default2<A,B,C,D,E,F,G,H> signal, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6) throws QUnsuccessfulInvocationException {
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
     * @param <G> The type of the seventh parameter of the signal.
     * @param <H> The type of the eighth parameter of the signal.
     * @param signal invoked signal
     * @param type synchronous or asynchronous invocation
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @param arg4 Argument for the fourth parameter.
     * @param arg5 Argument for the fifth parameter.
     * @param arg6 Argument for the sixth parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke signal
     */
    @QtUninvokable
    public static <A,B,C,D,E,F,G,H> void invokeMethod(@StrictNonNull AbstractSignal8Default2<A,B,C,D,E,F,G,H> signal, Qt.@NonNull ConnectionType type, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6) throws QUnsuccessfulInvocationException {
    	if(signal.containingObject() instanceof QObject) {
        	QObject qobject = (QObject)signal.containingObject();
        	QThread thread = qobject.thread();
        	switch(type){
			case AutoConnection:
				if(thread!=null && thread!=QThread.currentThread())
					break;
			case DirectConnection:
				signal.emit(arg1, arg2, arg3, arg4, arg5, arg6);
				return;
			default:
				break;
        	}
        	QMetaMethod qmethod = qobject.metaObject().methodByIndex(qobject.metaObject().metaObjectPointer, signal.methodIndex());
            if(qmethod!=null && qmethod.isValid()) {
                qmethod.invoke(qobject, type, 
                		arg1, arg2, arg3, arg4, arg5, arg6, 
                		signal.arg7Default.get(), 
                		((AbstractSignal8Default1<A,B,C,D,E,F,G,H>)signal).arg8Default.get());
            }else {
            	invokeMethod(thread, qobject, 
            			(Slot6<A,B,C,D,E,F>)signal::emit, 
            			type, arg1, arg2, arg3, arg4, arg5, arg6);
            }
        }else if(signal.containingObject() instanceof QtThreadAffineInterface) {
        	invokeMethod(((QtThreadAffineInterface)signal.containingObject()).thread(), 
        			(QObject)null, 
        			(Slot6<A,B,C,D,E,F>)signal::emit, 
        			type, arg1, arg2, arg3, arg4, arg5, arg6);
        }else {
        	invokeMethod((QThread)null, 
        			(QObject)null, 
        			(Slot6<A,B,C,D,E,F>)signal::emit, 
        			type, arg1, arg2, arg3, arg4, arg5, arg6);
        }
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
     */
    @QtUninvokable
    public static <A,B,C,D,E,F,G,H> void invokeMethod(@StrictNonNull AbstractSignal8Default3<A,B,C,D,E,F,G,H> signal, A arg1, B arg2, C arg3, D arg4, E arg5) throws QUnsuccessfulInvocationException {
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
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     * @param <H> The type of the eighth parameter of the signal.
     * @param signal invoked signal
     * @param type synchronous or asynchronous invocation
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @param arg4 Argument for the fourth parameter.
     * @param arg5 Argument for the fifth parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke signal
     */
    @QtUninvokable
    public static <A,B,C,D,E,F,G,H> void invokeMethod(@StrictNonNull AbstractSignal8Default3<A,B,C,D,E,F,G,H> signal, Qt.@NonNull ConnectionType type, A arg1, B arg2, C arg3, D arg4, E arg5) throws QUnsuccessfulInvocationException {
    	if(signal.containingObject() instanceof QObject) {
        	QObject qobject = (QObject)signal.containingObject();
        	QThread thread = qobject.thread();
        	switch(type){
			case AutoConnection:
				if(thread!=null && thread!=QThread.currentThread())
					break;
			case DirectConnection:
				signal.emit(arg1, arg2, arg3, arg4, arg5);
				return;
			default:
				break;
        	}
        	QMetaMethod qmethod = qobject.metaObject().methodByIndex(qobject.metaObject().metaObjectPointer, signal.methodIndex());
            if(qmethod!=null && qmethod.isValid()) {
                qmethod.invoke(qobject, type, 
                		arg1, arg2, arg3, arg4, arg5, 
                		signal.arg6Default.get(),
                		((AbstractSignal8Default2<A,B,C,D,E,F,G,H>)signal).arg7Default.get(),
                		((AbstractSignal8Default1<A,B,C,D,E,F,G,H>)signal).arg8Default.get());
            }else {
            	invokeMethod(thread, qobject, 
            			(Slot5<A,B,C,D,E>)signal::emit, 
            			type, arg1, arg2, arg3, arg4, arg5);
            }
        }else if(signal.containingObject() instanceof QtThreadAffineInterface) {
        	invokeMethod(((QtThreadAffineInterface)signal.containingObject()).thread(), 
        			(QObject)null, 
        			(Slot5<A,B,C,D,E>)signal::emit, 
        			type, arg1, arg2, arg3, arg4, arg5);
        }else {
        	invokeMethod((QThread)null, 
        			(QObject)null, 
        			(Slot5<A,B,C,D,E>)signal::emit, 
        			type, arg1, arg2, arg3, arg4, arg5);
        }
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
     */
    @QtUninvokable
    public static <A,B,C,D,E,F,G,H> void invokeMethod(@StrictNonNull AbstractSignal8Default4<A,B,C,D,E,F,G,H> signal, A arg1, B arg2, C arg3, D arg4) throws QUnsuccessfulInvocationException {
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
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     * @param <H> The type of the eighth parameter of the signal.
     * @param signal invoked signal
     * @param type synchronous or asynchronous invocation
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @param arg4 Argument for the fourth parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke signal
     */
    @QtUninvokable
    public static <A,B,C,D,E,F,G,H> void invokeMethod(@StrictNonNull AbstractSignal8Default4<A,B,C,D,E,F,G,H> signal, Qt.@NonNull ConnectionType type, A arg1, B arg2, C arg3, D arg4) throws QUnsuccessfulInvocationException {
    	if(signal.containingObject() instanceof QObject) {
        	QObject qobject = (QObject)signal.containingObject();
        	QThread thread = qobject.thread();
        	switch(type){
			case AutoConnection:
				if(thread!=null && thread!=QThread.currentThread())
					break;
			case DirectConnection:
				signal.emit(arg1, arg2, arg3, arg4);
				return;
			default:
				break;
        	}
        	QMetaMethod qmethod = qobject.metaObject().methodByIndex(qobject.metaObject().metaObjectPointer, signal.methodIndex());
            if(qmethod!=null && qmethod.isValid()) {
                qmethod.invoke(qobject, type, 
                		arg1, arg2, arg3, arg4, 
                		signal.arg5Default.get(),
                		((AbstractSignal8Default3<A,B,C,D,E,F,G,H>)signal).arg6Default.get(),
                		((AbstractSignal8Default2<A,B,C,D,E,F,G,H>)signal).arg7Default.get(),
                		((AbstractSignal8Default1<A,B,C,D,E,F,G,H>)signal).arg8Default.get());
            }else {
            	invokeMethod(thread, qobject, 
            			(Slot4<A,B,C,D>)signal::emit, 
            			type, arg1, arg2, arg3, arg4);
            }
        }else if(signal.containingObject() instanceof QtThreadAffineInterface) {
        	invokeMethod(((QtThreadAffineInterface)signal.containingObject()).thread(), 
        			(QObject)null, 
        			(Slot4<A,B,C,D>)signal::emit, 
        			type, arg1, arg2, arg3, arg4);
        }else {
        	invokeMethod((QThread)null, 
        			(QObject)null, 
        			(Slot4<A,B,C,D>)signal::emit, 
        			type, arg1, arg2, arg3, arg4);
        }
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
     */
    @QtUninvokable
    public static <A,B,C,D,E,F,G,H> void invokeMethod(@StrictNonNull AbstractSignal8Default5<A,B,C,D,E,F,G,H> signal, A arg1, B arg2, C arg3) throws QUnsuccessfulInvocationException {
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
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     * @param <H> The type of the eighth parameter of the signal.
     * @param signal invoked signal
     * @param type synchronous or asynchronous invocation
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke signal
     */
    @QtUninvokable
    public static <A,B,C,D,E,F,G,H> void invokeMethod(@StrictNonNull AbstractSignal8Default5<A,B,C,D,E,F,G,H> signal, Qt.@NonNull ConnectionType type, A arg1, B arg2, C arg3) throws QUnsuccessfulInvocationException {
    	if(signal.containingObject() instanceof QObject) {
        	QObject qobject = (QObject)signal.containingObject();
        	QThread thread = qobject.thread();
        	switch(type){
			case AutoConnection:
				if(thread!=null && thread!=QThread.currentThread())
					break;
			case DirectConnection:
				signal.emit(arg1, arg2, arg3);
				return;
			default:
				break;
        	}
        	QMetaMethod qmethod = qobject.metaObject().methodByIndex(qobject.metaObject().metaObjectPointer, signal.methodIndex());
            if(qmethod!=null && qmethod.isValid()) {
                qmethod.invoke(qobject, type, 
                		arg1, arg2, arg3, 
                		signal.arg4Default.get(),
                		((AbstractSignal8Default4<A,B,C,D,E,F,G,H>)signal).arg5Default.get(),
                		((AbstractSignal8Default3<A,B,C,D,E,F,G,H>)signal).arg6Default.get(),
                		((AbstractSignal8Default2<A,B,C,D,E,F,G,H>)signal).arg7Default.get(),
                		((AbstractSignal8Default1<A,B,C,D,E,F,G,H>)signal).arg8Default.get());
            }else {
            	invokeMethod(thread, qobject, 
            			(Slot3<A,B,C>)signal::emit, 
            			type, arg1, arg2, arg3);
            }
        }else if(signal.containingObject() instanceof QtThreadAffineInterface) {
        	invokeMethod(((QtThreadAffineInterface)signal.containingObject()).thread(), 
        			(QObject)null, 
        			(Slot3<A,B,C>)signal::emit, 
        			type, arg1, arg2, arg3);
        }else {
        	invokeMethod((QThread)null, 
        			(QObject)null, 
        			(Slot3<A,B,C>)signal::emit, 
        			type, arg1, arg2, arg3);
        }
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
     */
    @QtUninvokable
    public static <A,B,C,D,E,F,G,H> void invokeMethod(@StrictNonNull AbstractSignal8Default6<A,B,C,D,E,F,G,H> signal, A arg1, B arg2) throws QUnsuccessfulInvocationException {
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
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     * @param <H> The type of the eighth parameter of the signal.
     * @param signal invoked signal
     * @param type synchronous or asynchronous invocation
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke signal
     */
    @QtUninvokable
    public static <A,B,C,D,E,F,G,H> void invokeMethod(@StrictNonNull AbstractSignal8Default6<A,B,C,D,E,F,G,H> signal, Qt.@NonNull ConnectionType type, A arg1, B arg2) throws QUnsuccessfulInvocationException {
    	if(signal.containingObject() instanceof QObject) {
        	QObject qobject = (QObject)signal.containingObject();
        	QThread thread = qobject.thread();
        	switch(type){
			case AutoConnection:
				if(thread!=null && thread!=QThread.currentThread())
					break;
			case DirectConnection:
				signal.emit(arg1, arg2);
				return;
			default:
				break;
        	}
        	QMetaMethod qmethod = qobject.metaObject().methodByIndex(qobject.metaObject().metaObjectPointer, signal.methodIndex());
            if(qmethod!=null && qmethod.isValid()) {
                qmethod.invoke(qobject, type, 
                		arg1, arg2, 
                		signal.arg3Default.get(),
                		((AbstractSignal8Default5<A,B,C,D,E,F,G,H>)signal).arg4Default.get(),
                		((AbstractSignal8Default4<A,B,C,D,E,F,G,H>)signal).arg5Default.get(),
                		((AbstractSignal8Default3<A,B,C,D,E,F,G,H>)signal).arg6Default.get(),
                		((AbstractSignal8Default2<A,B,C,D,E,F,G,H>)signal).arg7Default.get(),
                		((AbstractSignal8Default1<A,B,C,D,E,F,G,H>)signal).arg8Default.get());
            }else {
            	invokeMethod(thread, qobject, 
            			(Slot2<A,B>)signal::emit, 
            			type, arg1, arg2);
            }
        }else if(signal.containingObject() instanceof QtThreadAffineInterface) {
        	invokeMethod(((QtThreadAffineInterface)signal.containingObject()).thread(), 
        			(QObject)null, 
        			(Slot2<A,B>)signal::emit, 
        			type, arg1, arg2);
        }else {
        	invokeMethod((QThread)null, 
        			(QObject)null, 
        			(Slot2<A,B>)signal::emit, 
        			type, arg1, arg2);
        }
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
     */
    @QtUninvokable
    public static <A,B,C,D,E,F,G,H> void invokeMethod(@StrictNonNull AbstractSignal8Default7<A,B,C,D,E,F,G,H> signal, A arg1) throws QUnsuccessfulInvocationException {
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
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     * @param <H> The type of the eighth parameter of the signal.
     * @param signal invoked signal
     * @param type synchronous or asynchronous invocation
     * @param arg1 Argument for the first parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke signal
     */
    @QtUninvokable
    public static <A,B,C,D,E,F,G,H> void invokeMethod(@StrictNonNull AbstractSignal8Default7<A,B,C,D,E,F,G,H> signal, Qt.@NonNull ConnectionType type, A arg1) throws QUnsuccessfulInvocationException {
    	if(signal.containingObject() instanceof QObject) {
        	QObject qobject = (QObject)signal.containingObject();
        	QThread thread = qobject.thread();
        	switch(type){
			case AutoConnection:
				if(thread!=null && thread!=QThread.currentThread())
					break;
			case DirectConnection:
				signal.emit(arg1);
				return;
			default:
				break;
        	}
        	QMetaMethod qmethod = qobject.metaObject().methodByIndex(qobject.metaObject().metaObjectPointer, signal.methodIndex());
            if(qmethod!=null && qmethod.isValid()) {
                qmethod.invoke(qobject, type, 
                		arg1, 
                		signal.arg2Default.get(),
                		((AbstractSignal8Default6<A,B,C,D,E,F,G,H>)signal).arg3Default.get(),
                		((AbstractSignal8Default5<A,B,C,D,E,F,G,H>)signal).arg4Default.get(),
                		((AbstractSignal8Default4<A,B,C,D,E,F,G,H>)signal).arg5Default.get(),
                		((AbstractSignal8Default3<A,B,C,D,E,F,G,H>)signal).arg6Default.get(),
                		((AbstractSignal8Default2<A,B,C,D,E,F,G,H>)signal).arg7Default.get(),
                		((AbstractSignal8Default1<A,B,C,D,E,F,G,H>)signal).arg8Default.get());
            }else {
            	invokeMethod(thread, qobject, 
            			(Slot1<A>)signal::emit, 
            			type, arg1);
            }
        }else if(signal.containingObject() instanceof QtThreadAffineInterface) {
        	invokeMethod(((QtThreadAffineInterface)signal.containingObject()).thread(), 
        			(QObject)null, 
        			(Slot1<A>)signal::emit, 
        			type, arg1);
        }else {
        	invokeMethod((QThread)null, 
        			(QObject)null, 
        			(Slot1<A>)signal::emit, 
        			type, arg1);
        }
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
     */
    @QtUninvokable
    public static <A,B,C,D,E,F,G,H> void invokeMethod(@StrictNonNull AbstractSignal8Default8<A,B,C,D,E,F,G,H> signal) throws QUnsuccessfulInvocationException {
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
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     * @param <H> The type of the eighth parameter of the signal.
     * @param signal invoked signal
     * @param type synchronous or asynchronous invocation
     * @throws QUnsuccessfulInvocationException if not able to invoke signal
     */
    @QtUninvokable
    public static <A,B,C,D,E,F,G,H> void invokeMethod(@StrictNonNull AbstractSignal8Default8<A,B,C,D,E,F,G,H> signal, Qt.@NonNull ConnectionType type) throws QUnsuccessfulInvocationException {
    	if(signal.containingObject() instanceof QObject) {
        	QObject qobject = (QObject)signal.containingObject();
        	QThread thread = qobject.thread();
        	switch(type){
			case AutoConnection:
				if(thread!=null && thread!=QThread.currentThread())
					break;
			case DirectConnection:
				signal.emit();
				return;
			default:
				break;
        	}
        	QMetaMethod qmethod = qobject.metaObject().methodByIndex(qobject.metaObject().metaObjectPointer, signal.methodIndex());
            if(qmethod!=null && qmethod.isValid()) {
                qmethod.invoke(qobject, type, 
                		signal.arg1Default.get(), 
                		((AbstractSignal8Default7<A,B,C,D,E,F,G,H>)signal).arg2Default.get(), 
                		((AbstractSignal8Default6<A,B,C,D,E,F,G,H>)signal).arg3Default.get(), 
                		((AbstractSignal8Default5<A,B,C,D,E,F,G,H>)signal).arg4Default.get(), 
                		((AbstractSignal8Default4<A,B,C,D,E,F,G,H>)signal).arg5Default.get(), 
                		((AbstractSignal8Default3<A,B,C,D,E,F,G,H>)signal).arg6Default.get(), 
                		((AbstractSignal8Default2<A,B,C,D,E,F,G,H>)signal).arg7Default.get(), 
                		((AbstractSignal8Default1<A,B,C,D,E,F,G,H>)signal).arg8Default.get());
            }else {
            	invokeMethod(thread, qobject, 
            			(Slot0)signal::emit, 
            			type);
            }
        }else if(signal.containingObject() instanceof QtThreadAffineInterface) {
        	invokeMethod(((QtThreadAffineInterface)signal.containingObject()).thread(), 
        			(QObject)null, 
        			(Slot0)signal::emit, 
        			type);
        }else {
        	invokeMethod((QThread)null, 
        			(QObject)null, 
        			(Slot0)signal::emit, 
        			type);
        }
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
    public static <A,B,C,D,E,F,G,H,I> void invokeMethod(@StrictNonNull AbstractPrivateSignal9<A,B,C,D,E,F,G,H,I> signal, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7, H arg8, I arg9) throws QUnsuccessfulInvocationException {
        invokeMethod(signal, Qt.ConnectionType.AutoConnection, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
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
    public static <A,B,C,D,E,F,G,H,I> void invokeMethod(@StrictNonNull AbstractPublicSignal9<A,B,C,D,E,F,G,H,I> signal, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7, H arg8, I arg9) throws QUnsuccessfulInvocationException {
        invokeMethod(signal, Qt.ConnectionType.AutoConnection, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
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
     */
    @QtUninvokable
    public static <A,B,C,D,E,F,G,H,I> void invokeMethod(@StrictNonNull AbstractSignal9Default1<A,B,C,D,E,F,G,H,I> signal, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7, H arg8) throws QUnsuccessfulInvocationException {
        invokeMethod(signal, Qt.ConnectionType.AutoConnection, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
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
     */
    @QtUninvokable
    public static <A,B,C,D,E,F,G,H,I> void invokeMethod(@StrictNonNull AbstractSignal9Default2<A,B,C,D,E,F,G,H,I> signal, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7) throws QUnsuccessfulInvocationException {
        invokeMethod(signal, Qt.ConnectionType.AutoConnection, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
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
     */
    @QtUninvokable
    public static <A,B,C,D,E,F,G,H,I> void invokeMethod(@StrictNonNull AbstractSignal9Default3<A,B,C,D,E,F,G,H,I> signal, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6) throws QUnsuccessfulInvocationException {
        invokeMethod(signal, Qt.ConnectionType.AutoConnection, arg1, arg2, arg3, arg4, arg5, arg6);
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
     */
    @QtUninvokable
    public static <A,B,C,D,E,F,G,H,I> void invokeMethod(@StrictNonNull AbstractSignal9Default4<A,B,C,D,E,F,G,H,I> signal, A arg1, B arg2, C arg3, D arg4, E arg5) throws QUnsuccessfulInvocationException {
        invokeMethod(signal, Qt.ConnectionType.AutoConnection, arg1, arg2, arg3, arg4, arg5);
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
     */
    @QtUninvokable
    public static <A,B,C,D,E,F,G,H,I> void invokeMethod(@StrictNonNull AbstractSignal9Default5<A,B,C,D,E,F,G,H,I> signal, A arg1, B arg2, C arg3, D arg4) throws QUnsuccessfulInvocationException {
        invokeMethod(signal, Qt.ConnectionType.AutoConnection, arg1, arg2, arg3, arg4);
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
     */
    @QtUninvokable
    public static <A,B,C,D,E,F,G,H,I> void invokeMethod(@StrictNonNull AbstractSignal9Default6<A,B,C,D,E,F,G,H,I> signal, A arg1, B arg2, C arg3) throws QUnsuccessfulInvocationException {
        invokeMethod(signal, Qt.ConnectionType.AutoConnection, arg1, arg2, arg3);
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
     */
    @QtUninvokable
    public static <A,B,C,D,E,F,G,H,I> void invokeMethod(@StrictNonNull AbstractSignal9Default7<A,B,C,D,E,F,G,H,I> signal, A arg1, B arg2) throws QUnsuccessfulInvocationException {
        invokeMethod(signal, Qt.ConnectionType.AutoConnection, arg1, arg2);
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
     */
    @QtUninvokable
    public static <A,B,C,D,E,F,G,H,I> void invokeMethod(@StrictNonNull AbstractSignal9Default8<A,B,C,D,E,F,G,H,I> signal, A arg1) throws QUnsuccessfulInvocationException {
        invokeMethod(signal, Qt.ConnectionType.AutoConnection, arg1);
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
     */
    @QtUninvokable
    public static <A,B,C,D,E,F,G,H,I> void invokeMethod(@StrictNonNull AbstractSignal9Default9<A,B,C,D,E,F,G,H,I> signal) throws QUnsuccessfulInvocationException {
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
     * @param type synchronous or asynchronous invocation
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
    public static <A,B,C,D,E,F,G,H,I> void invokeMethod(@StrictNonNull AbstractPublicSignal9<A,B,C,D,E,F,G,H,I> signal, Qt.@NonNull ConnectionType type, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7, H arg8, I arg9) throws QUnsuccessfulInvocationException {
    	if(signal.containingObject() instanceof QObject) {
        	QObject qobject = (QObject)signal.containingObject();
        	QThread thread = qobject.thread();
        	switch(type){
			case AutoConnection:
				if(thread!=null && thread!=QThread.currentThread())
					break;
			case DirectConnection:
				signal.emit(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
				return;
			default:
				break;
        	}
        	QMetaMethod qmethod = qobject.metaObject().methodByIndex(qobject.metaObject().metaObjectPointer, signal.methodIndex());
            if(qmethod!=null && qmethod.isValid()) {
                qmethod.invoke(qobject, type, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
            }else {
            	invokeMethod(thread, qobject, signal::emit, type, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
            }
        }else if(signal.containingObject() instanceof QtThreadAffineInterface) {
        	invokeMethod(((QtThreadAffineInterface)signal.containingObject()).thread(), (QObject)null, signal::emit, type, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
        }else {
        	invokeMethod((QThread)null, (QObject)null, signal::emit, type, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
        }
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
     * @param type synchronous or asynchronous invocation
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
    public static <A,B,C,D,E,F,G,H,I> void invokeMethod(@StrictNonNull AbstractPrivateSignal9<A,B,C,D,E,F,G,H,I> signal, Qt.@NonNull ConnectionType type, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7, H arg8, I arg9) throws QUnsuccessfulInvocationException {
        if(signal.containingObject() instanceof QObject && !((QObject)signal.containingObject()).isDisposed()) {
            QObject qobject = (QObject)signal.containingObject();
            QMetaMethod qmethod = qobject.metaObject().methodByIndex(qobject.metaObject().metaObjectPointer, signal.methodIndex());
            if(qmethod!=null && qmethod.isValid()) {
                qmethod.invoke(qobject, type, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
                return;
            }
        }
        throw new QUnsuccessfulInvocationException("Unable to invoke method.");
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
     * @param type synchronous or asynchronous invocation
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
    public static <A,B,C,D,E,F,G,H,I> void invokeMethod(@StrictNonNull AbstractSignal9Default1<A,B,C,D,E,F,G,H,I> signal, Qt.@NonNull ConnectionType type, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7, H arg8) throws QUnsuccessfulInvocationException {
    	if(signal.containingObject() instanceof QObject) {
        	QObject qobject = (QObject)signal.containingObject();
        	QThread thread = qobject.thread();
        	switch(type){
			case AutoConnection:
				if(thread!=null && thread!=QThread.currentThread())
					break;
			case DirectConnection:
				signal.emit(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
				return;
			default:
				break;
        	}
        	QMetaMethod qmethod = qobject.metaObject().methodByIndex(qobject.metaObject().metaObjectPointer, signal.methodIndex());
            if(qmethod!=null && qmethod.isValid()) {
                qmethod.invoke(qobject, type, 
                		arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, 
                		signal.arg9Default.get());
            }else {
            	invokeMethod(thread, qobject, 
            			(Slot8<A,B,C,D,E,F,G,H>)signal::emit, 
            			type, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
            }
        }else if(signal.containingObject() instanceof QtThreadAffineInterface) {
        	invokeMethod(((QtThreadAffineInterface)signal.containingObject()).thread(), 
        			(QObject)null, 
        			(Slot8<A,B,C,D,E,F,G,H>)signal::emit, 
        			type, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
        }else {
        	invokeMethod((QThread)null, 
        			(QObject)null, 
        			(Slot8<A,B,C,D,E,F,G,H>)signal::emit, 
        			type, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
        }
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
     * @param type synchronous or asynchronous invocation
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
    public static <A,B,C,D,E,F,G,H,I> void invokeMethod(@StrictNonNull AbstractSignal9Default2<A,B,C,D,E,F,G,H,I> signal, Qt.@NonNull ConnectionType type, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6, G arg7) throws QUnsuccessfulInvocationException {
    	if(signal.containingObject() instanceof QObject) {
        	QObject qobject = (QObject)signal.containingObject();
        	QThread thread = qobject.thread();
        	switch(type){
			case AutoConnection:
				if(thread!=null && thread!=QThread.currentThread())
					break;
			case DirectConnection:
				signal.emit(arg1, arg2, arg3, arg4, arg5, arg6, arg7);
				return;
			default:
				break;
        	}
        	QMetaMethod qmethod = qobject.metaObject().methodByIndex(qobject.metaObject().metaObjectPointer, signal.methodIndex());
            if(qmethod!=null && qmethod.isValid()) {
                qmethod.invoke(qobject, type, 
                		arg1, arg2, arg3, arg4, arg5, arg6, arg7, 
                		signal.arg8Default.get(), 
                		((AbstractSignal9Default1<A,B,C,D,E,F,G,H,I>)signal).arg9Default.get());
            }else {
            	invokeMethod(thread, qobject, 
            			(Slot7<A,B,C,D,E,F,G>)signal::emit, 
            			type, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
            }
        }else if(signal.containingObject() instanceof QtThreadAffineInterface) {
        	invokeMethod(((QtThreadAffineInterface)signal.containingObject()).thread(), 
        			(QObject)null, 
        			(Slot7<A,B,C,D,E,F,G>)signal::emit, 
        			type, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
        }else {
        	invokeMethod((QThread)null, 
        			(QObject)null, 
        			(Slot7<A,B,C,D,E,F,G>)signal::emit, 
        			type, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
        }
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
     * @param type synchronous or asynchronous invocation
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @param arg4 Argument for the fourth parameter.
     * @param arg5 Argument for the fifth parameter.
     * @param arg6 Argument for the sixth parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke signal
     */
    @QtUninvokable
    public static <A,B,C,D,E,F,G,H,I> void invokeMethod(@StrictNonNull AbstractSignal9Default3<A,B,C,D,E,F,G,H,I> signal, Qt.@NonNull ConnectionType type, A arg1, B arg2, C arg3, D arg4, E arg5, F arg6) throws QUnsuccessfulInvocationException {
    	if(signal.containingObject() instanceof QObject) {
        	QObject qobject = (QObject)signal.containingObject();
        	QThread thread = qobject.thread();
        	switch(type){
			case AutoConnection:
				if(thread!=null && thread!=QThread.currentThread())
					break;
			case DirectConnection:
				signal.emit(arg1, arg2, arg3, arg4, arg5, arg6);
				return;
			default:
				break;
        	}
        	QMetaMethod qmethod = qobject.metaObject().methodByIndex(qobject.metaObject().metaObjectPointer, signal.methodIndex());
            if(qmethod!=null && qmethod.isValid()) {
                qmethod.invoke(qobject, type, 
                		arg1, arg2, arg3, arg4, arg5, arg6, 
                		signal.arg7Default.get(), 
                		((AbstractSignal9Default2<A,B,C,D,E,F,G,H,I>)signal).arg8Default.get(), 
                		((AbstractSignal9Default1<A,B,C,D,E,F,G,H,I>)signal).arg9Default.get());
            }else {
            	invokeMethod(thread, qobject, 
            			(Slot6<A,B,C,D,E,F>)signal::emit, 
            			type, arg1, arg2, arg3, arg4, arg5, arg6);
            }
        }else if(signal.containingObject() instanceof QtThreadAffineInterface) {
        	invokeMethod(((QtThreadAffineInterface)signal.containingObject()).thread(), 
        			(QObject)null, 
        			(Slot6<A,B,C,D,E,F>)signal::emit, 
        			type, arg1, arg2, arg3, arg4, arg5, arg6);
        }else {
        	invokeMethod((QThread)null, 
        			(QObject)null, 
        			(Slot6<A,B,C,D,E,F>)signal::emit, 
        			type, arg1, arg2, arg3, arg4, arg5, arg6);
        }
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
     * @param type synchronous or asynchronous invocation
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @param arg4 Argument for the fourth parameter.
     * @param arg5 Argument for the fifth parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke signal
     */
    @QtUninvokable
    public static <A,B,C,D,E,F,G,H,I> void invokeMethod(@StrictNonNull AbstractSignal9Default4<A,B,C,D,E,F,G,H,I> signal, Qt.@NonNull ConnectionType type, A arg1, B arg2, C arg3, D arg4, E arg5) throws QUnsuccessfulInvocationException {
    	if(signal.containingObject() instanceof QObject) {
        	QObject qobject = (QObject)signal.containingObject();
        	QThread thread = qobject.thread();
        	switch(type){
			case AutoConnection:
				if(thread!=null && thread!=QThread.currentThread())
					break;
			case DirectConnection:
				signal.emit(arg1, arg2, arg3, arg4, arg5);
				return;
			default:
				break;
        	}
        	QMetaMethod qmethod = qobject.metaObject().methodByIndex(qobject.metaObject().metaObjectPointer, signal.methodIndex());
            if(qmethod!=null && qmethod.isValid()) {
                qmethod.invoke(qobject, type, 
                		arg1, arg2, arg3, arg4, arg5, 
                		signal.arg6Default.get(),
                		((AbstractSignal9Default3<A,B,C,D,E,F,G,H,I>)signal).arg7Default.get(),
                		((AbstractSignal9Default2<A,B,C,D,E,F,G,H,I>)signal).arg8Default.get(),
                		((AbstractSignal9Default1<A,B,C,D,E,F,G,H,I>)signal).arg9Default.get());
            }else {
            	invokeMethod(thread, qobject, 
            			(Slot5<A,B,C,D,E>)signal::emit, 
            			type, arg1, arg2, arg3, arg4, arg5);
            }
        }else if(signal.containingObject() instanceof QtThreadAffineInterface) {
        	invokeMethod(((QtThreadAffineInterface)signal.containingObject()).thread(), 
        			(QObject)null, 
        			(Slot5<A,B,C,D,E>)signal::emit, 
        			type, arg1, arg2, arg3, arg4, arg5);
        }else {
        	invokeMethod((QThread)null, 
        			(QObject)null, 
        			(Slot5<A,B,C,D,E>)signal::emit, 
        			type, arg1, arg2, arg3, arg4, arg5);
        }
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
     * @param type synchronous or asynchronous invocation
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @param arg4 Argument for the fourth parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke signal
     */
    @QtUninvokable
    public static <A,B,C,D,E,F,G,H,I> void invokeMethod(@StrictNonNull AbstractSignal9Default5<A,B,C,D,E,F,G,H,I> signal, Qt.@NonNull ConnectionType type, A arg1, B arg2, C arg3, D arg4) throws QUnsuccessfulInvocationException {
    	if(signal.containingObject() instanceof QObject) {
        	QObject qobject = (QObject)signal.containingObject();
        	QThread thread = qobject.thread();
        	switch(type){
			case AutoConnection:
				if(thread!=null && thread!=QThread.currentThread())
					break;
			case DirectConnection:
				signal.emit(arg1, arg2, arg3, arg4);
				return;
			default:
				break;
        	}
        	QMetaMethod qmethod = qobject.metaObject().methodByIndex(qobject.metaObject().metaObjectPointer, signal.methodIndex());
            if(qmethod!=null && qmethod.isValid()) {
                qmethod.invoke(qobject, type, 
                		arg1, arg2, arg3, arg4, 
                		signal.arg5Default.get(),
                		((AbstractSignal9Default4<A,B,C,D,E,F,G,H,I>)signal).arg6Default.get(),
                		((AbstractSignal9Default3<A,B,C,D,E,F,G,H,I>)signal).arg7Default.get(),
                		((AbstractSignal9Default2<A,B,C,D,E,F,G,H,I>)signal).arg8Default.get(),
                		((AbstractSignal9Default1<A,B,C,D,E,F,G,H,I>)signal).arg9Default.get());
            }else {
            	invokeMethod(thread, qobject, 
            			(Slot4<A,B,C,D>)signal::emit, 
            			type, arg1, arg2, arg3, arg4);
            }
        }else if(signal.containingObject() instanceof QtThreadAffineInterface) {
        	invokeMethod(((QtThreadAffineInterface)signal.containingObject()).thread(), 
        			(QObject)null, 
        			(Slot4<A,B,C,D>)signal::emit, 
        			type, arg1, arg2, arg3, arg4);
        }else {
        	invokeMethod((QThread)null, 
        			(QObject)null, 
        			(Slot4<A,B,C,D>)signal::emit, 
        			type, arg1, arg2, arg3, arg4);
        }
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
     * @param type synchronous or asynchronous invocation
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @param arg3 Argument for the third parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke signal
     */
    @QtUninvokable
    public static <A,B,C,D,E,F,G,H,I> void invokeMethod(@StrictNonNull AbstractSignal9Default6<A,B,C,D,E,F,G,H,I> signal, Qt.@NonNull ConnectionType type, A arg1, B arg2, C arg3) throws QUnsuccessfulInvocationException {
    	if(signal.containingObject() instanceof QObject) {
        	QObject qobject = (QObject)signal.containingObject();
        	QThread thread = qobject.thread();
        	switch(type){
			case AutoConnection:
				if(thread!=null && thread!=QThread.currentThread())
					break;
			case DirectConnection:
				signal.emit(arg1, arg2, arg3);
				return;
			default:
				break;
        	}
        	QMetaMethod qmethod = qobject.metaObject().methodByIndex(qobject.metaObject().metaObjectPointer, signal.methodIndex());
            if(qmethod!=null && qmethod.isValid()) {
                qmethod.invoke(qobject, type, 
                		arg1, arg2, arg3, 
                		signal.arg4Default.get(),
                		((AbstractSignal9Default5<A,B,C,D,E,F,G,H,I>)signal).arg5Default.get(),
                		((AbstractSignal9Default4<A,B,C,D,E,F,G,H,I>)signal).arg6Default.get(),
                		((AbstractSignal9Default3<A,B,C,D,E,F,G,H,I>)signal).arg7Default.get(),
                		((AbstractSignal9Default2<A,B,C,D,E,F,G,H,I>)signal).arg8Default.get(),
                		((AbstractSignal9Default1<A,B,C,D,E,F,G,H,I>)signal).arg9Default.get());
            }else {
            	invokeMethod(thread, qobject, 
            			(Slot3<A,B,C>)signal::emit, 
            			type, arg1, arg2, arg3);
            }
        }else if(signal.containingObject() instanceof QtThreadAffineInterface) {
        	invokeMethod(((QtThreadAffineInterface)signal.containingObject()).thread(), 
        			(QObject)null, 
        			(Slot3<A,B,C>)signal::emit, 
        			type, arg1, arg2, arg3);
        }else {
        	invokeMethod((QThread)null, 
        			(QObject)null, 
        			(Slot3<A,B,C>)signal::emit, 
        			type, arg1, arg2, arg3);
        }
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
     * @param type synchronous or asynchronous invocation
     * @param arg1 Argument for the first parameter.
     * @param arg2 Argument for the second parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke signal
     */
    @QtUninvokable
    public static <A,B,C,D,E,F,G,H,I> void invokeMethod(@StrictNonNull AbstractSignal9Default7<A,B,C,D,E,F,G,H,I> signal, Qt.@NonNull ConnectionType type, A arg1, B arg2) throws QUnsuccessfulInvocationException {
    	if(signal.containingObject() instanceof QObject) {
        	QObject qobject = (QObject)signal.containingObject();
        	QThread thread = qobject.thread();
        	switch(type){
			case AutoConnection:
				if(thread!=null && thread!=QThread.currentThread())
					break;
			case DirectConnection:
				signal.emit(arg1, arg2);
				return;
			default:
				break;
        	}
        	QMetaMethod qmethod = qobject.metaObject().methodByIndex(qobject.metaObject().metaObjectPointer, signal.methodIndex());
            if(qmethod!=null && qmethod.isValid()) {
                qmethod.invoke(qobject, type, 
                		arg1, arg2, 
                		signal.arg3Default.get(),
                		((AbstractSignal9Default6<A,B,C,D,E,F,G,H,I>)signal).arg4Default.get(),
                		((AbstractSignal9Default5<A,B,C,D,E,F,G,H,I>)signal).arg5Default.get(),
                		((AbstractSignal9Default4<A,B,C,D,E,F,G,H,I>)signal).arg6Default.get(),
                		((AbstractSignal9Default3<A,B,C,D,E,F,G,H,I>)signal).arg7Default.get(),
                		((AbstractSignal9Default2<A,B,C,D,E,F,G,H,I>)signal).arg8Default.get(),
                		((AbstractSignal9Default1<A,B,C,D,E,F,G,H,I>)signal).arg9Default.get());
            }else {
            	invokeMethod(thread, qobject, 
            			(Slot2<A,B>)signal::emit, 
            			type, arg1, arg2);
            }
        }else if(signal.containingObject() instanceof QtThreadAffineInterface) {
        	invokeMethod(((QtThreadAffineInterface)signal.containingObject()).thread(), 
        			(QObject)null, 
        			(Slot2<A,B>)signal::emit, 
        			type, arg1, arg2);
        }else {
        	invokeMethod((QThread)null, 
        			(QObject)null, 
        			(Slot2<A,B>)signal::emit, 
        			type, arg1, arg2);
        }
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
     * @param type synchronous or asynchronous invocation
     * @param arg1 Argument for the first parameter.
     * @throws QUnsuccessfulInvocationException if not able to invoke signal
     */
    @QtUninvokable
    public static <A,B,C,D,E,F,G,H,I> void invokeMethod(@StrictNonNull AbstractSignal9Default8<A,B,C,D,E,F,G,H,I> signal, Qt.@NonNull ConnectionType type, A arg1) throws QUnsuccessfulInvocationException {
    	if(signal.containingObject() instanceof QObject) {
        	QObject qobject = (QObject)signal.containingObject();
        	QThread thread = qobject.thread();
        	switch(type){
			case AutoConnection:
				if(thread!=null && thread!=QThread.currentThread())
					break;
			case DirectConnection:
				signal.emit(arg1);
				return;
			default:
				break;
        	}
        	QMetaMethod qmethod = qobject.metaObject().methodByIndex(qobject.metaObject().metaObjectPointer, signal.methodIndex());
            if(qmethod!=null && qmethod.isValid()) {
                qmethod.invoke(qobject, type, 
                		arg1, 
                		signal.arg2Default.get(),
                		((AbstractSignal9Default7<A,B,C,D,E,F,G,H,I>)signal).arg3Default.get(),
                		((AbstractSignal9Default6<A,B,C,D,E,F,G,H,I>)signal).arg4Default.get(),
                		((AbstractSignal9Default5<A,B,C,D,E,F,G,H,I>)signal).arg5Default.get(),
                		((AbstractSignal9Default4<A,B,C,D,E,F,G,H,I>)signal).arg6Default.get(),
                		((AbstractSignal9Default3<A,B,C,D,E,F,G,H,I>)signal).arg7Default.get(),
                		((AbstractSignal9Default2<A,B,C,D,E,F,G,H,I>)signal).arg8Default.get(),
                		((AbstractSignal9Default1<A,B,C,D,E,F,G,H,I>)signal).arg9Default.get());
            }else {
            	invokeMethod(thread, qobject, 
            			(Slot1<A>)signal::emit, 
            			type, arg1);
            }
        }else if(signal.containingObject() instanceof QtThreadAffineInterface) {
        	invokeMethod(((QtThreadAffineInterface)signal.containingObject()).thread(), 
        			(QObject)null, 
        			(Slot1<A>)signal::emit, 
        			type, arg1);
        }else {
        	invokeMethod((QThread)null, 
        			(QObject)null, 
        			(Slot1<A>)signal::emit, 
        			type, arg1);
        }
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
     * @param type synchronous or asynchronous invocation
     * @throws QUnsuccessfulInvocationException if not able to invoke signal
     */
    @QtUninvokable
    public static <A,B,C,D,E,F,G,H,I> void invokeMethod(@StrictNonNull AbstractSignal9Default9<A,B,C,D,E,F,G,H,I> signal, Qt.@NonNull ConnectionType type) throws QUnsuccessfulInvocationException {
    	if(signal.containingObject() instanceof QObject) {
        	QObject qobject = (QObject)signal.containingObject();
        	QThread thread = qobject.thread();
        	switch(type){
			case AutoConnection:
				if(thread!=null && thread!=QThread.currentThread())
					break;
			case DirectConnection:
				signal.emit();
				return;
			default:
				break;
        	}
        	QMetaMethod qmethod = qobject.metaObject().methodByIndex(qobject.metaObject().metaObjectPointer, signal.methodIndex());
            if(qmethod!=null && qmethod.isValid()) {
                qmethod.invoke(qobject, type, 
                		signal.arg1Default.get(),
                		((AbstractSignal9Default8<A,B,C,D,E,F,G,H,I>)signal).arg2Default.get(),
                		((AbstractSignal9Default7<A,B,C,D,E,F,G,H,I>)signal).arg3Default.get(),
                		((AbstractSignal9Default6<A,B,C,D,E,F,G,H,I>)signal).arg4Default.get(),
                		((AbstractSignal9Default5<A,B,C,D,E,F,G,H,I>)signal).arg5Default.get(),
                		((AbstractSignal9Default4<A,B,C,D,E,F,G,H,I>)signal).arg6Default.get(),
                		((AbstractSignal9Default3<A,B,C,D,E,F,G,H,I>)signal).arg7Default.get(),
                		((AbstractSignal9Default2<A,B,C,D,E,F,G,H,I>)signal).arg8Default.get(),
                		((AbstractSignal9Default1<A,B,C,D,E,F,G,H,I>)signal).arg9Default.get());
            }else {
            	invokeMethod(thread, qobject, 
            			(Slot0)signal::emit, 
            			type);
            }
        }else if(signal.containingObject() instanceof QtThreadAffineInterface) {
        	invokeMethod(((QtThreadAffineInterface)signal.containingObject()).thread(), 
        			(QObject)null, 
        			(Slot0)signal::emit, 
        			type);
        }else {
        	invokeMethod((QThread)null, 
        			(QObject)null, 
        			(Slot0)signal::emit, 
        			type);
        }
    }
    
    private static native void invokeMethod(long context, Runnable runnable, boolean blocking);
    
    /**
     * Calling <code>writeProperty(property, AutoConnection, value)</code>.
     * @param <A> The type of the property.
     * @param property property to be set
     * @param value new value
     */
    @QtUninvokable
    public static <A> void writeProperty(@StrictNonNull QObject.QProperty<A> property, A value) {
    	writeProperty(property, Qt.ConnectionType.AutoConnection, value);
    }
    
    /**
     * <p>Sets the value of the property.</p>
     * 
     * <p>The execution can be either synchronous or asynchronous, depending on type:</p>
     * <ul>
     * <li>If type is {@link Qt.ConnectionType#DirectConnection}, the property will be changed immediately.</li>
     * <li>If type is {@link Qt.ConnectionType#QueuedConnection}, a QEvent will be sent and the property is changed as soon as the application enters the main event loop.</li>
     * <li>If type is {@link Qt.ConnectionType#BlockingQueuedConnection}, the property will be changed in the same way as for {@link Qt.ConnectionType#QueuedConnection}, except that the current thread will block until the event is delivered. Using this connection type to communicate between objects in the same thread will lead to deadlocks.</li>
     * <li>If type is {@link Qt.ConnectionType#AutoConnection}, the property is changed synchronously if the object lives in the same thread as the caller; otherwise it will invoke the member asynchronously.</li>
     * </ul>
     * 
     * @param <A> The type of the property.
     * @param property property to be set
     * @param type synchronous or asynchronous action
     * @param value new value
     * @throws QUnsuccessfulInvocationException if calling with BlockingQueuedConnection in current thread
     */
    @QtUninvokable
    public static <A> void writeProperty(@StrictNonNull QObject.QProperty<A> property, Qt.@NonNull ConnectionType type, A value) throws QUnsuccessfulInvocationException {
        QThread thread = property.owner().thread();
        if(type==Qt.ConnectionType.AutoConnection && thread!=null && thread!=QThread.currentThread()) {
            type = Qt.ConnectionType.QueuedConnection;
        }
        boolean blocking = false;
        switch(type) {
        case AutoConnection:
        case DirectConnection:
        	property.setValue(value);
        	break;
        case BlockingQueuedConnection:
        	blocking = true;
        	if(thread==QThread.currentThread()) {
                throw new QUnsuccessfulInvocationException("Blocking-queued invocation on object whose thread is the current thread is not allowed.");
            }
        case QueuedConnection:
        	invokeMethod(QtJambi_LibraryUtilities.internal.nativeId(property.owner()), ()->{
        		if(!property.isDisposed() && !property.owner().isDisposed()) {
        			property.setValue(value);
        		}
        	}, blocking);
            break;
        default:
        	throw new QUnsuccessfulInvocationException("Unable to set value of property.");
        }
    }
    
    /**
     * Calling <code>writeProperty(property, AutoConnection, value)</code>.
     * @param property property to be set
     * @param value new value
     */
    @QtUninvokable
    public static void writeProperty(@StrictNonNull QObject.QByteProperty property, byte value) {
    	writeProperty(property, Qt.ConnectionType.AutoConnection, value);
    }
    
    /**
     * <p>Sets the value of the property.</p>
     * 
     * <p>The execution can be either synchronous or asynchronous, depending on type:</p>
     * <ul>
     * <li>If type is {@link Qt.ConnectionType#DirectConnection}, the property will be changed immediately.</li>
     * <li>If type is {@link Qt.ConnectionType#QueuedConnection}, a QEvent will be sent and the property is changed as soon as the application enters the main event loop.</li>
     * <li>If type is {@link Qt.ConnectionType#BlockingQueuedConnection}, the property will be changed in the same way as for {@link Qt.ConnectionType#QueuedConnection}, except that the current thread will block until the event is delivered. Using this connection type to communicate between objects in the same thread will lead to deadlocks.</li>
     * <li>If type is {@link Qt.ConnectionType#AutoConnection}, the property is changed synchronously if the object lives in the same thread as the caller; otherwise it will invoke the member asynchronously.</li>
     * </ul>
     * 
     * @param property property to be set
     * @param type synchronous or asynchronous action
     * @param value new value
     * @throws QUnsuccessfulInvocationException if calling with BlockingQueuedConnection in current thread
     */
    @QtUninvokable
    public static void writeProperty(@StrictNonNull QObject.QByteProperty property, Qt.@NonNull ConnectionType type, byte value) throws QUnsuccessfulInvocationException {
        QThread thread = property.owner().thread();
        if(type==Qt.ConnectionType.AutoConnection && thread!=null && thread!=QThread.currentThread()) {
            type = Qt.ConnectionType.QueuedConnection;
        }
        boolean blocking = false;
        switch(type) {
        case AutoConnection:
        case DirectConnection:
        	property.setValue(value);
        	break;
        case BlockingQueuedConnection:
        	blocking = true;
        	if(thread==QThread.currentThread()) {
                throw new QUnsuccessfulInvocationException("Blocking-queued invocation on object whose thread is the current thread is not allowed.");
            }
        case QueuedConnection:
        	invokeMethod(QtJambi_LibraryUtilities.internal.nativeId(property.owner()), ()->{
        		if(!property.isDisposed() && !property.owner().isDisposed()) {
        			property.setValue(value);
        		}
        	}, blocking);
            break;
        default:
        	throw new QUnsuccessfulInvocationException("Unable to set value of property.");
        }
    }
    
    /**
     * Calling <code>writeProperty(property, AutoConnection, value)</code>.
     * @param property property to be set
     * @param value new value
     */
    @QtUninvokable
    public static void writeProperty(@StrictNonNull QObject.QIntProperty property, int value) {
    	writeProperty(property, Qt.ConnectionType.AutoConnection, value);
    }
    
    /**
     * <p>Sets the value of the property.</p>
     * 
     * <p>The execution can be either synchronous or asynchronous, depending on type:</p>
     * <ul>
     * <li>If type is {@link Qt.ConnectionType#DirectConnection}, the property will be changed immediately.</li>
     * <li>If type is {@link Qt.ConnectionType#QueuedConnection}, a QEvent will be sent and the property is changed as soon as the application enters the main event loop.</li>
     * <li>If type is {@link Qt.ConnectionType#BlockingQueuedConnection}, the property will be changed in the same way as for {@link Qt.ConnectionType#QueuedConnection}, except that the current thread will block until the event is delivered. Using this connection type to communicate between objects in the same thread will lead to deadlocks.</li>
     * <li>If type is {@link Qt.ConnectionType#AutoConnection}, the property is changed synchronously if the object lives in the same thread as the caller; otherwise it will invoke the member asynchronously.</li>
     * </ul>
     * 
     * @param property property to be set
     * @param type synchronous or asynchronous action
     * @param value new value
     * @throws QUnsuccessfulInvocationException if calling with BlockingQueuedConnection in current thread
     */
    @QtUninvokable
    public static void writeProperty(@StrictNonNull QObject.QIntProperty property, Qt.@NonNull ConnectionType type, int value) throws QUnsuccessfulInvocationException {
        QThread thread = property.owner().thread();
        if(type==Qt.ConnectionType.AutoConnection && thread!=null && thread!=QThread.currentThread()) {
            type = Qt.ConnectionType.QueuedConnection;
        }
        boolean blocking = false;
        switch(type) {
        case AutoConnection:
        case DirectConnection:
        	property.setValue(value);
        	break;
        case BlockingQueuedConnection:
        	blocking = true;
        	if(thread==QThread.currentThread()) {
                throw new QUnsuccessfulInvocationException("Blocking-queued invocation on object whose thread is the current thread is not allowed.");
            }
        case QueuedConnection:
        	invokeMethod(QtJambi_LibraryUtilities.internal.nativeId(property.owner()), ()->{
        		if(!property.isDisposed() && !property.owner().isDisposed()) {
        			property.setValue(value);
        		}
        	}, blocking);
            break;
        default:
        	throw new QUnsuccessfulInvocationException("Unable to set value of property.");
        }
    }
    
    /**
     * Calling <code>writeProperty(property, AutoConnection, value)</code>.
     * @param property property to be set
     * @param value new value
     */
    @QtUninvokable
    public static void writeProperty(@StrictNonNull QObject.QShortProperty property, short value) {
    	writeProperty(property, Qt.ConnectionType.AutoConnection, value);
    }
    
    /**
     * <p>Sets the value of the property.</p>
     * 
     * <p>The execution can be either synchronous or asynchronous, depending on type:</p>
     * <ul>
     * <li>If type is {@link Qt.ConnectionType#DirectConnection}, the property will be changed immediately.</li>
     * <li>If type is {@link Qt.ConnectionType#QueuedConnection}, a QEvent will be sent and the property is changed as soon as the application enters the main event loop.</li>
     * <li>If type is {@link Qt.ConnectionType#BlockingQueuedConnection}, the property will be changed in the same way as for {@link Qt.ConnectionType#QueuedConnection}, except that the current thread will block until the event is delivered. Using this connection type to communicate between objects in the same thread will lead to deadlocks.</li>
     * <li>If type is {@link Qt.ConnectionType#AutoConnection}, the property is changed synchronously if the object lives in the same thread as the caller; otherwise it will invoke the member asynchronously.</li>
     * </ul>
     * 
     * @param property property to be set
     * @param type synchronous or asynchronous action
     * @param value new value
     * @throws QUnsuccessfulInvocationException if calling with BlockingQueuedConnection in current thread
     */
    @QtUninvokable
    public static void writeProperty(@StrictNonNull QObject.QShortProperty property, Qt.@NonNull ConnectionType type, short value) throws QUnsuccessfulInvocationException {
        QThread thread = property.owner().thread();
        if(type==Qt.ConnectionType.AutoConnection && thread!=null && thread!=QThread.currentThread()) {
            type = Qt.ConnectionType.QueuedConnection;
        }
        boolean blocking = false;
        switch(type) {
        case AutoConnection:
        case DirectConnection:
        	property.setValue(value);
        	break;
        case BlockingQueuedConnection:
        	blocking = true;
        	if(thread==QThread.currentThread()) {
                throw new QUnsuccessfulInvocationException("Blocking-queued invocation on object whose thread is the current thread is not allowed.");
            }
        case QueuedConnection:
        	invokeMethod(QtJambi_LibraryUtilities.internal.nativeId(property.owner()), ()->{
        		if(!property.isDisposed() && !property.owner().isDisposed()) {
        			property.setValue(value);
        		}
        	}, blocking);
            break;
        default:
        	throw new QUnsuccessfulInvocationException("Unable to set value of property.");
        }
    }
    
    /**
     * Calling <code>writeProperty(property, AutoConnection, value)</code>.
     * @param property property to be set
     * @param value new value
     */
    @QtUninvokable
    public static void writeProperty(@StrictNonNull QObject.QLongProperty property, long value) {
    	writeProperty(property, Qt.ConnectionType.AutoConnection, value);
    }
    
    /**
     * <p>Sets the value of the property.</p>
     * 
     * <p>The execution can be either synchronous or asynchronous, depending on type:</p>
     * <ul>
     * <li>If type is {@link Qt.ConnectionType#DirectConnection}, the property will be changed immediately.</li>
     * <li>If type is {@link Qt.ConnectionType#QueuedConnection}, a QEvent will be sent and the property is changed as soon as the application enters the main event loop.</li>
     * <li>If type is {@link Qt.ConnectionType#BlockingQueuedConnection}, the property will be changed in the same way as for {@link Qt.ConnectionType#QueuedConnection}, except that the current thread will block until the event is delivered. Using this connection type to communicate between objects in the same thread will lead to deadlocks.</li>
     * <li>If type is {@link Qt.ConnectionType#AutoConnection}, the property is changed synchronously if the object lives in the same thread as the caller; otherwise it will invoke the member asynchronously.</li>
     * </ul>
     * 
     * @param property property to be set
     * @param type synchronous or asynchronous action
     * @param value new value
     * @throws QUnsuccessfulInvocationException if calling with BlockingQueuedConnection in current thread
     */
    @QtUninvokable
    public static void writeProperty(@StrictNonNull QObject.QLongProperty property, Qt.@NonNull ConnectionType type, long value) throws QUnsuccessfulInvocationException {
        QThread thread = property.owner().thread();
        if(type==Qt.ConnectionType.AutoConnection && thread!=null && thread!=QThread.currentThread()) {
            type = Qt.ConnectionType.QueuedConnection;
        }
        boolean blocking = false;
        switch(type) {
        case AutoConnection:
        case DirectConnection:
        	property.setValue(value);
        	break;
        case BlockingQueuedConnection:
        	blocking = true;
        	if(thread==QThread.currentThread()) {
                throw new QUnsuccessfulInvocationException("Blocking-queued invocation on object whose thread is the current thread is not allowed.");
            }
        case QueuedConnection:
        	invokeMethod(QtJambi_LibraryUtilities.internal.nativeId(property.owner()), ()->{
        		if(!property.isDisposed() && !property.owner().isDisposed()) {
        			property.setValue(value);
        		}
        	}, blocking);
            break;
        default:
        	throw new QUnsuccessfulInvocationException("Unable to set value of property.");
        }
    }
    
    /**
     * Calling <code>writeProperty(property, AutoConnection, value)</code>.
     * @param property property to be set
     * @param value new value
     */
    @QtUninvokable
    public static void writeProperty(@StrictNonNull QObject.QFloatProperty property, float value) {
    	writeProperty(property, Qt.ConnectionType.AutoConnection, value);
    }
    
    /**
     * <p>Sets the value of the property.</p>
     * 
     * <p>The execution can be either synchronous or asynchronous, depending on type:</p>
     * <ul>
     * <li>If type is {@link Qt.ConnectionType#DirectConnection}, the property will be changed immediately.</li>
     * <li>If type is {@link Qt.ConnectionType#QueuedConnection}, a QEvent will be sent and the property is changed as soon as the application enters the main event loop.</li>
     * <li>If type is {@link Qt.ConnectionType#BlockingQueuedConnection}, the property will be changed in the same way as for {@link Qt.ConnectionType#QueuedConnection}, except that the current thread will block until the event is delivered. Using this connection type to communicate between objects in the same thread will lead to deadlocks.</li>
     * <li>If type is {@link Qt.ConnectionType#AutoConnection}, the property is changed synchronously if the object lives in the same thread as the caller; otherwise it will invoke the member asynchronously.</li>
     * </ul>
     * 
     * @param property property to be set
     * @param type synchronous or asynchronous action
     * @param value new value
     * @throws QUnsuccessfulInvocationException if calling with BlockingQueuedConnection in current thread
     */
    @QtUninvokable
    public static void writeProperty(@StrictNonNull QObject.QFloatProperty property, Qt.@NonNull ConnectionType type, float value) throws QUnsuccessfulInvocationException {
        QThread thread = property.owner().thread();
        if(type==Qt.ConnectionType.AutoConnection && thread!=null && thread!=QThread.currentThread()) {
            type = Qt.ConnectionType.QueuedConnection;
        }
        boolean blocking = false;
        switch(type) {
        case AutoConnection:
        case DirectConnection:
        	property.setValue(value);
        	break;
        case BlockingQueuedConnection:
        	blocking = true;
        	if(thread==QThread.currentThread()) {
                throw new QUnsuccessfulInvocationException("Blocking-queued invocation on object whose thread is the current thread is not allowed.");
            }
        case QueuedConnection:
        	invokeMethod(QtJambi_LibraryUtilities.internal.nativeId(property.owner()), ()->{
        		if(!property.isDisposed() && !property.owner().isDisposed()) {
        			property.setValue(value);
        		}
        	}, blocking);
            break;
        default:
        	throw new QUnsuccessfulInvocationException("Unable to set value of property.");
        }
    }
    
    /**
     * Calling <code>writeProperty(property, AutoConnection, value)</code>.
     * @param property property to be set
     * @param value new value
     */
    @QtUninvokable
    public static void writeProperty(@StrictNonNull QObject.QDoubleProperty property, double value) {
    	writeProperty(property, Qt.ConnectionType.AutoConnection, value);
    }
    
    /**
     * <p>Sets the value of the property.</p>
     * 
     * <p>The execution can be either synchronous or asynchronous, depending on type:</p>
     * <ul>
     * <li>If type is {@link Qt.ConnectionType#DirectConnection}, the property will be changed immediately.</li>
     * <li>If type is {@link Qt.ConnectionType#QueuedConnection}, a QEvent will be sent and the property is changed as soon as the application enters the main event loop.</li>
     * <li>If type is {@link Qt.ConnectionType#BlockingQueuedConnection}, the property will be changed in the same way as for {@link Qt.ConnectionType#QueuedConnection}, except that the current thread will block until the event is delivered. Using this connection type to communicate between objects in the same thread will lead to deadlocks.</li>
     * <li>If type is {@link Qt.ConnectionType#AutoConnection}, the property is changed synchronously if the object lives in the same thread as the caller; otherwise it will invoke the member asynchronously.</li>
     * </ul>
     * 
     * @param property property to be set
     * @param type synchronous or asynchronous action
     * @param value new value
     * @throws QUnsuccessfulInvocationException if calling with BlockingQueuedConnection in current thread
     */
    @QtUninvokable
    public static void writeProperty(@StrictNonNull QObject.QDoubleProperty property, Qt.@NonNull ConnectionType type, double value) throws QUnsuccessfulInvocationException {
        QThread thread = property.owner().thread();
        if(type==Qt.ConnectionType.AutoConnection && thread!=null && thread!=QThread.currentThread()) {
            type = Qt.ConnectionType.QueuedConnection;
        }
        boolean blocking = false;
        switch(type) {
        case AutoConnection:
        case DirectConnection:
        	property.setValue(value);
        	break;
        case BlockingQueuedConnection:
        	blocking = true;
        	if(thread==QThread.currentThread()) {
                throw new QUnsuccessfulInvocationException("Blocking-queued invocation on object whose thread is the current thread is not allowed.");
            }
        case QueuedConnection:
        	invokeMethod(QtJambi_LibraryUtilities.internal.nativeId(property.owner()), ()->{
        		if(!property.isDisposed() && !property.owner().isDisposed()) {
        			property.setValue(value);
        		}
        	}, blocking);
            break;
        default:
        	throw new QUnsuccessfulInvocationException("Unable to set value of property.");
        }
    }
    
    /**
     * Calling <code>writeProperty(property, AutoConnection, value)</code>.
     * @param property property to be set
     * @param value new value
     */
    @QtUninvokable
    public static void writeProperty(@StrictNonNull QObject.QBooleanProperty property, boolean value) {
    	writeProperty(property, Qt.ConnectionType.AutoConnection, value);
    }
    
    /**
     * <p>Sets the value of the property.</p>
     * 
     * <p>The execution can be either synchronous or asynchronous, depending on type:</p>
     * <ul>
     * <li>If type is {@link Qt.ConnectionType#DirectConnection}, the property will be changed immediately.</li>
     * <li>If type is {@link Qt.ConnectionType#QueuedConnection}, a QEvent will be sent and the property is changed as soon as the application enters the main event loop.</li>
     * <li>If type is {@link Qt.ConnectionType#BlockingQueuedConnection}, the property will be changed in the same way as for {@link Qt.ConnectionType#QueuedConnection}, except that the current thread will block until the event is delivered. Using this connection type to communicate between objects in the same thread will lead to deadlocks.</li>
     * <li>If type is {@link Qt.ConnectionType#AutoConnection}, the property is changed synchronously if the object lives in the same thread as the caller; otherwise it will invoke the member asynchronously.</li>
     * </ul>
     * 
     * @param property property to be set
     * @param type synchronous or asynchronous action
     * @param value new value
     * @throws QUnsuccessfulInvocationException if calling with BlockingQueuedConnection in current thread
     */
    @QtUninvokable
    public static void writeProperty(@StrictNonNull QObject.QBooleanProperty property, Qt.@NonNull ConnectionType type, boolean value) throws QUnsuccessfulInvocationException {
        QThread thread = property.owner().thread();
        if(type==Qt.ConnectionType.AutoConnection && thread!=null && thread!=QThread.currentThread()) {
            type = Qt.ConnectionType.QueuedConnection;
        }
        boolean blocking = false;
        switch(type) {
        case AutoConnection:
        case DirectConnection:
        	property.setValue(value);
        	break;
        case BlockingQueuedConnection:
        	blocking = true;
        	if(thread==QThread.currentThread()) {
                throw new QUnsuccessfulInvocationException("Blocking-queued invocation on object whose thread is the current thread is not allowed.");
            }
        case QueuedConnection:
        	invokeMethod(QtJambi_LibraryUtilities.internal.nativeId(property.owner()), ()->{
        		if(!property.isDisposed() && !property.owner().isDisposed()) {
        			property.setValue(value);
        		}
        	}, blocking);
            break;
        default:
        	throw new QUnsuccessfulInvocationException("Unable to set value of property.");
        }
    }
    
    /**
     * Calling <code>writeProperty(property, AutoConnection, value)</code>.
     * @param property property to be set
     * @param value new value
     */
    @QtUninvokable
    public static void writeProperty(@StrictNonNull QObject.QCharProperty property, char value) {
    	writeProperty(property, Qt.ConnectionType.AutoConnection, value);
    }
    
    /**
     * <p>Sets the value of the property.</p>
     * 
     * <p>The execution can be either synchronous or asynchronous, depending on type:</p>
     * <ul>
     * <li>If type is {@link Qt.ConnectionType#DirectConnection}, the property will be changed immediately.</li>
     * <li>If type is {@link Qt.ConnectionType#QueuedConnection}, a QEvent will be sent and the property is changed as soon as the application enters the main event loop.</li>
     * <li>If type is {@link Qt.ConnectionType#BlockingQueuedConnection}, the property will be changed in the same way as for {@link Qt.ConnectionType#QueuedConnection}, except that the current thread will block until the event is delivered. Using this connection type to communicate between objects in the same thread will lead to deadlocks.</li>
     * <li>If type is {@link Qt.ConnectionType#AutoConnection}, the property is changed synchronously if the object lives in the same thread as the caller; otherwise it will invoke the member asynchronously.</li>
     * </ul>
     * 
     * @param property property to be set
     * @param type synchronous or asynchronous action
     * @param value new value
     * @throws QUnsuccessfulInvocationException if calling with BlockingQueuedConnection in current thread
     */
    @QtUninvokable
    public static void writeProperty(@StrictNonNull QObject.QCharProperty property, Qt.@NonNull ConnectionType type, char value) throws QUnsuccessfulInvocationException {
        QThread thread = property.owner().thread();
        if(type==Qt.ConnectionType.AutoConnection && thread!=null && thread!=QThread.currentThread()) {
            type = Qt.ConnectionType.QueuedConnection;
        }
        boolean blocking = false;
        switch(type) {
        case AutoConnection:
        case DirectConnection:
        	property.setValue(value);
        	break;
        case BlockingQueuedConnection:
        	blocking = true;
        	if(thread==QThread.currentThread()) {
                throw new QUnsuccessfulInvocationException("Blocking-queued invocation on object whose thread is the current thread is not allowed.");
            }
        case QueuedConnection:
        	invokeMethod(QtJambi_LibraryUtilities.internal.nativeId(property.owner()), ()->{
        		if(!property.isDisposed() && !property.owner().isDisposed()) {
        			property.setValue(value);
        		}
        	}, blocking);
            break;
        default:
        	throw new QUnsuccessfulInvocationException("Unable to set value of property.");
        }
    }
    
    /**
     * <p>Reads the value of the property in the object's thread.</p>
     * 
     * @param <A> The type of the property.
     * @param property property to be read
     * @return value
     */
    @QtUninvokable
    public static <A> A readProperty(@StrictNonNull QObject.QProperty<A> property) {
        QThread thread = property.owner().thread();
        if(thread!=QThread.currentThread()) {
        	List<A> result = Arrays.asList((A)null);
        	invokeMethod(QtJambi_LibraryUtilities.internal.nativeId(property.owner()), ()->{
        		if(!property.isDisposed() && !property.owner().isDisposed()) {
        			result.set(0, property.value());
        		}
        	}, true);
            return result.get(0);
        }else {
            return property.value();
        }
    }
    
    /**
     * <p>Reads the value of the property in the object's thread.</p>
     * 
     * @param property property to be read
     * @return value
     */
    @QtUninvokable
    public static byte readProperty(@StrictNonNull QObject.QByteProperty property) {
        QThread thread = property.owner().thread();
        if(thread!=QThread.currentThread()) {
        	byte[] result = new byte[0];
        	invokeMethod(QtJambi_LibraryUtilities.internal.nativeId(property.owner()), ()->{
        		if(!property.isDisposed() && !property.owner().isDisposed()) {
        			result[0] = property.value();
        		}
        	}, true);
            return result[0];
        }else {
            return property.value();
        }
    }
    
    /**
     * <p>Reads the value of the property in the object's thread.</p>
     * 
     * @param property property to be read
     * @return value
     */
    @QtUninvokable
    public static int readProperty(@StrictNonNull QObject.QIntProperty property) {
        QThread thread = property.owner().thread();
        if(thread!=QThread.currentThread()) {
        	int[] result = new int[0];
        	invokeMethod(QtJambi_LibraryUtilities.internal.nativeId(property.owner()), ()->{
        		if(!property.isDisposed() && !property.owner().isDisposed()) {
        			result[0] = property.value();
        		}
        	}, true);
            return result[0];
        }else {
            return property.value();
        }
    }
    
    /**
     * <p>Reads the value of the property in the object's thread.</p>
     * 
     * @param property property to be read
     * @return value
     */
    @QtUninvokable
    public static long readProperty(@StrictNonNull QObject.QLongProperty property) {
        QThread thread = property.owner().thread();
        if(thread!=QThread.currentThread()) {
        	long[] result = new long[0];
        	invokeMethod(QtJambi_LibraryUtilities.internal.nativeId(property.owner()), ()->{
        		if(!property.isDisposed() && !property.owner().isDisposed()) {
        			result[0] = property.value();
        		}
        	}, true);
            return result[0];
        }else {
            return property.value();
        }
    }
    
    /**
     * <p>Reads the value of the property in the object's thread.</p>
     * 
     * @param property property to be read
     * @return value
     */
    @QtUninvokable
    public static short readProperty(@StrictNonNull QObject.QShortProperty property) {
        QThread thread = property.owner().thread();
        if(thread!=QThread.currentThread()) {
        	short[] result = new short[0];
        	invokeMethod(QtJambi_LibraryUtilities.internal.nativeId(property.owner()), ()->{
        		if(!property.isDisposed() && !property.owner().isDisposed()) {
        			result[0] = property.value();
        		}
        	}, true);
            return result[0];
        }else {
            return property.value();
        }
    }
    
    /**
     * <p>Reads the value of the property in the object's thread.</p>
     * 
     * @param property property to be read
     * @return value
     */
    @QtUninvokable
    public static float readProperty(@StrictNonNull QObject.QFloatProperty property) {
        QThread thread = property.owner().thread();
        if(thread!=QThread.currentThread()) {
        	float[] result = new float[0];
        	invokeMethod(QtJambi_LibraryUtilities.internal.nativeId(property.owner()), ()->{
        		if(!property.isDisposed() && !property.owner().isDisposed()) {
        			result[0] = property.value();
        		}
        	}, true);
            return result[0];
        }else {
            return property.value();
        }
    }
    
    /**
     * <p>Reads the value of the property in the object's thread.</p>
     * 
     * @param property property to be read
     * @return value
     */
    @QtUninvokable
    public static double readProperty(@StrictNonNull QObject.QDoubleProperty property) {
        QThread thread = property.owner().thread();
        if(thread!=QThread.currentThread()) {
        	double[] result = new double[0];
        	invokeMethod(QtJambi_LibraryUtilities.internal.nativeId(property.owner()), ()->{
        		if(!property.isDisposed() && !property.owner().isDisposed()) {
        			result[0] = property.value();
        		}
        	}, true);
            return result[0];
        }else {
            return property.value();
        }
    }
    
    /**
     * <p>Reads the value of the property in the object's thread.</p>
     * 
     * @param property property to be read
     * @return value
     */
    @QtUninvokable
    public static boolean readProperty(@StrictNonNull QObject.QBooleanProperty property) {
        QThread thread = property.owner().thread();
        if(thread!=QThread.currentThread()) {
        	boolean[] result = new boolean[0];
        	invokeMethod(QtJambi_LibraryUtilities.internal.nativeId(property.owner()), ()->{
        		if(!property.isDisposed() && !property.owner().isDisposed()) {
        			result[0] = property.value();
        		}
        	}, true);
            return result[0];
        }else {
            return property.value();
        }
    }
    
    /**
     * <p>Reads the value of the property in the object's thread.</p>
     * 
     * @param property property to be read
     * @return value
     */
    @QtUninvokable
    public static char readProperty(@StrictNonNull QObject.QCharProperty property) {
        QThread thread = property.owner().thread();
        if(thread!=QThread.currentThread()) {
        	char[] result = new char[0];
        	invokeMethod(QtJambi_LibraryUtilities.internal.nativeId(property.owner()), ()->{
        		if(!property.isDisposed() && !property.owner().isDisposed()) {
        			result[0] = property.value();
        		}
        	}, true);
            return result[0];
        }else {
            return property.value();
        }
    }
    
    /**
     * <p>Reads the value of the property in the object's thread.</p>
     * 
     * @param <A> The type of the property.
     * @param property property to be read
     * @return value
     */
    @QtUninvokable
    public static <A> A readProperty(@StrictNonNull QObject.QComputedProperty<A> property) {
        QThread thread = property.owner().thread();
        if(thread!=QThread.currentThread()) {
        	List<A> result = Arrays.asList((A)null);
        	invokeMethod(QtJambi_LibraryUtilities.internal.nativeId(property.owner()), ()->{
        		if(!property.isDisposed() && !property.owner().isDisposed()) {
        			result.set(0, property.value());
        		}
        	}, true);
            return result.get(0);
        }else {
            return property.value();
        }
    }
    
    /**
     * <p>Reads the value of the property in the object's thread.</p>
     * 
     * @param property property to be read
     * @return value
     */
    @QtUninvokable
    public static byte readProperty(@StrictNonNull QObject.QComputedByteProperty property) {
        QThread thread = property.owner().thread();
        if(thread!=QThread.currentThread()) {
        	byte[] result = new byte[0];
        	invokeMethod(QtJambi_LibraryUtilities.internal.nativeId(property.owner()), ()->{
        		if(!property.isDisposed() && !property.owner().isDisposed()) {
        			result[0] = property.value();
        		}
        	}, true);
            return result[0];
        }else {
            return property.value();
        }
    }
    
    /**
     * <p>Reads the value of the property in the object's thread.</p>
     * 
     * @param property property to be read
     * @return value
     */
    @QtUninvokable
    public static int readProperty(@StrictNonNull QObject.QComputedIntProperty property) {
        QThread thread = property.owner().thread();
        if(thread!=QThread.currentThread()) {
        	int[] result = new int[0];
        	invokeMethod(QtJambi_LibraryUtilities.internal.nativeId(property.owner()), ()->{
        		if(!property.isDisposed() && !property.owner().isDisposed()) {
        			result[0] = property.value();
        		}
        	}, true);
            return result[0];
        }else {
            return property.value();
        }
    }
    
    /**
     * <p>Reads the value of the property in the object's thread.</p>
     * 
     * @param property property to be read
     * @return value
     */
    @QtUninvokable
    public static long readProperty(@StrictNonNull QObject.QComputedLongProperty property) {
        QThread thread = property.owner().thread();
        if(thread!=QThread.currentThread()) {
        	long[] result = new long[0];
        	invokeMethod(QtJambi_LibraryUtilities.internal.nativeId(property.owner()), ()->{
        		if(!property.isDisposed() && !property.owner().isDisposed()) {
        			result[0] = property.value();
        		}
        	}, true);
            return result[0];
        }else {
            return property.value();
        }
    }
    
    /**
     * <p>Reads the value of the property in the object's thread.</p>
     * 
     * @param property property to be read
     * @return value
     */
    @QtUninvokable
    public static short readProperty(@StrictNonNull QObject.QComputedShortProperty property) {
        QThread thread = property.owner().thread();
        if(thread!=QThread.currentThread()) {
        	short[] result = new short[0];
        	invokeMethod(QtJambi_LibraryUtilities.internal.nativeId(property.owner()), ()->{
        		if(!property.isDisposed() && !property.owner().isDisposed()) {
        			result[0] = property.value();
        		}
        	}, true);
            return result[0];
        }else {
            return property.value();
        }
    }
    
    /**
     * <p>Reads the value of the property in the object's thread.</p>
     * 
     * @param <A> The type of the property.
     * @param property property to be read
     * @return value
     */
    @QtUninvokable
    public static float readProperty(@StrictNonNull QObject.QComputedFloatProperty property) {
        QThread thread = property.owner().thread();
        if(thread!=QThread.currentThread()) {
        	float[] result = new float[0];
        	invokeMethod(QtJambi_LibraryUtilities.internal.nativeId(property.owner()), ()->{
        		if(!property.isDisposed() && !property.owner().isDisposed()) {
        			result[0] = property.value();
        		}
        	}, true);
            return result[0];
        }else {
            return property.value();
        }
    }
    
    /**
     * <p>Reads the value of the property in the object's thread.</p>
     * 
     * @param property property to be read
     * @return value
     */
    @QtUninvokable
    public static double readProperty(@StrictNonNull QObject.QComputedDoubleProperty property) {
        QThread thread = property.owner().thread();
        if(thread!=QThread.currentThread()) {
        	double[] result = new double[0];
        	invokeMethod(QtJambi_LibraryUtilities.internal.nativeId(property.owner()), ()->{
        		if(!property.isDisposed() && !property.owner().isDisposed()) {
        			result[0] = property.value();
        		}
        	}, true);
            return result[0];
        }else {
            return property.value();
        }
    }
    
    /**
     * <p>Reads the value of the property in the object's thread.</p>
     * 
     * @param property property to be read
     * @return value
     */
    @QtUninvokable
    public static boolean readProperty(@StrictNonNull QObject.QComputedBooleanProperty property) {
        QThread thread = property.owner().thread();
        if(thread!=QThread.currentThread()) {
        	boolean[] result = new boolean[0];
        	invokeMethod(QtJambi_LibraryUtilities.internal.nativeId(property.owner()), ()->{
        		if(!property.isDisposed() && !property.owner().isDisposed()) {
        			result[0] = property.value();
        		}
        	}, true);
            return result[0];
        }else {
            return property.value();
        }
    }
    
    /**
     * <p>Reads the value of the property in the object's thread.</p>
     * 
     * @param property property to be read
     * @return value
     */
    @QtUninvokable
    public static char readProperty(@StrictNonNull QObject.QComputedCharProperty property) {
        QThread thread = property.owner().thread();
        if(thread!=QThread.currentThread()) {
        	char[] result = new char[0];
        	invokeMethod(QtJambi_LibraryUtilities.internal.nativeId(property.owner()), ()->{
        		if(!property.isDisposed() && !property.owner().isDisposed()) {
        			result[0] = property.value();
        		}
        	}, true);
            return result[0];
        }else {
            return property.value();
        }
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
    
    static abstract class AbstractMultiSignal extends CoreUtility.AbstractMultiSignal<AbstractSignal> {
        AbstractMultiSignal(){
        	super();
    	}
        
        @io.qt.QtUninvokable
    	final List<AbstractSignal> allSignals(){
        	return signals();
        }
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
         * @hidden
         */
        @Override
        protected final void checkConnection(Object receiver, boolean slotObject) {
            CoreUtility.checkConnectionToDisposedSignal(this, receiver, slotObject);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         */
        public final QMetaObject.@NonNull Connection connect(@StrictNonNull Slot0 slot, Qt. @NonNull ConnectionType @NonNull... connectionType) {
            return addConnectionToSlotObject(slot, connectionType);
        }
    
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(@StrictNonNull Slot0 slot) {
            return removeConnectionToSlotObject(slot);
        }
    }
    
    /**
     * Supertype of all signals in QtJambi.
     */
    public static abstract class AbstractSignal extends CoreUtility.AbstractSignal implements Signal {
        
    	AbstractSignal(){
        	super();
        }
        
        AbstractSignal(Consumer<Object[]> argumentTest){
			super(argumentTest);
		}
    
        AbstractSignal(Class<?> declaringClass) {
            super(declaringClass);
        }
        
        AbstractSignal(Class<?> declaringClass, boolean isDisposedSignal) {
            super(declaringClass, isDisposedSignal);
        }
        
        AbstractSignal(@StrictNonNull String signalName, Class<?>... types) {
            super(signalName, types);
        }
        
        final Class<?> signalDeclaringClass(){
			return getDeclaringClass();
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
        public final QMetaObject.@NonNull Connection connect(@StrictNonNull Object receiver, @StrictNonNull String method, Qt.@NonNull ConnectionType @NonNull... connectionType) {
            if (receiver == null)
                throw new NullPointerException("Receiver must be non-null");
            if (method == null)
                throw new NullPointerException("Slot must be non-null");
            return addConnection(receiver, method, connectionType);
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
        public final boolean disconnect(@Nullable Object receiver) {
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
        public final boolean disconnect(QMetaObject.@NonNull Connection connection) {
            return removeConnection(connection);
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
    
    /**
     * Interface abstraction of signals.
     */
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
     * An interface for anything connectable to a signal without parameters.
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
     * An interface for parameterless emitable signal.
     */
    public static interface Emitable0 extends Signal{
        public void emit();
    }

    /**
     * An interface for emitable signal with one parameter.
     * @param <A> The type of the first parameter.
     */
    public static interface Emitable1<A> extends Signal{
        public void emit(A a);
    }

    /**
     * An interface for emitable signal with two parameters.
     * @param <A> The type of the first parameter.
     * @param <B> The type of the second parameter.
     */
    public static interface Emitable2<A,B> extends Signal{
        public void emit(A a, B b);
    }
    
    /**
     * An interface for emitable signal with three parameters.
     * @param <A> The type of the first parameter.
     * @param <B> The type of the second parameter.
     * @param <C> The type of the third parameter.
     */
    public static interface Emitable3<A,B,C> extends Signal{
        public void emit(A a, B b, C c);
    }
    
    /**
     * An interface for emitable signal with four parameters.
     * @param <A> The type of the first parameter.
     * @param <B> The type of the second parameter.
     * @param <C> The type of the third parameter.
     * @param <D> The type of the fourth parameter.
     */
    public static interface Emitable4<A,B,C,D> extends Signal{
        public void emit(A a, B b, C c, D d);
    }
    
    /**
     * An interface for emitable signal with five parameters.
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
     * An interface for emitable signal with six parameters.
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
     * An interface for emitable signal with seven parameters.
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
     * An interface for emitable signal with eight parameters.
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
     * An interface for emitable signal with nine parameters.
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
     * A generic handle to a method of variadic arguments with return value.
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
     * An interface for testing signal emits with one argument.
     * @param <A> The type of the first argument.
     */
    public static interface ArgChecker1<A>{
        public void check(A a);
    }

    /**
     * An interface for testing signal emits with two arguments.
     * @param <A> The type of the first argument.
     * @param <B> The type of the second argument.
     */
    public static interface ArgChecker2<A,B>{
        public void check(A a, B b);
    }
    
    /**
     * An interface for testing signal emits with three arguments.
     * @param <A> The type of the first argument.
     * @param <B> The type of the second argument.
     * @param <C> The type of the third argument.
     */
    public static interface ArgChecker3<A,B,C>{
        public void check(A a, B b, C c);
    }
    
    /**
     * An interface for testing signal emits with four arguments.
     * @param <A> The type of the first argument.
     * @param <B> The type of the second argument.
     * @param <C> The type of the third argument.
     * @param <D> The type of the fourth argument.
     */
    public static interface ArgChecker4<A,B,C,D>{
        public void check(A a, B b, C c, D d);
    }
    
    /**
     * An interface for testing signal emits with five arguments.
     * @param <A> The type of the first argument.
     * @param <B> The type of the second argument.
     * @param <C> The type of the third argument.
     * @param <D> The type of the fourth argument.
     * @param <E> The type of the fifth argument.
     */
    public static interface ArgChecker5<A,B,C,D,E>{
        public void check(A a, B b, C c, D d, E e);
    }
    
    /**
     * An interface for testing signal emits with six arguments.
     * @param <A> The type of the first argument.
     * @param <B> The type of the second argument.
     * @param <C> The type of the third argument.
     * @param <D> The type of the fourth argument.
     * @param <E> The type of the fifth argument.
     * @param <F> The type of the sixth argument.
     */
    public static interface ArgChecker6<A,B,C,D,E,F>{
        public void check(A a, B b, C c, D d, E e, F f);
    }
    
    /**
     * An interface for testing signal emits with seven arguments.
     * @param <A> The type of the first argument.
     * @param <B> The type of the second argument.
     * @param <C> The type of the third argument.
     * @param <D> The type of the fourth argument.
     * @param <E> The type of the fifth argument.
     * @param <F> The type of the sixth argument.
     * @param <G> The type of the seventh argument.
     */
    public static interface ArgChecker7<A,B,C,D,E,F,G>{
        public void check(A a, B b, C c, D d, E e, F f, G g);
    }
    
    /**
     * An interface for testing signal emits with eight arguments.
     * @param <A> The type of the first argument.
     * @param <B> The type of the second argument.
     * @param <C> The type of the third argument.
     * @param <D> The type of the fourth argument.
     * @param <E> The type of the fifth argument.
     * @param <F> The type of the sixth argument.
     * @param <G> The type of the seventh argument.
     * @param <H> The type of the eighth argument.
     */
    public static interface ArgChecker8<A,B,C,D,E,F,G,H>{
        public void check(A a, B b, C c, D d, E e, F f, G g, H h);
    }
    
    /**
     * An interface for testing signal emits with nine arguments.
     * @param <A> The type of the first argument.
     * @param <B> The type of the second argument.
     * @param <C> The type of the third argument.
     * @param <D> The type of the fourth argument.
     * @param <E> The type of the fifth argument.
     * @param <F> The type of the sixth argument.
     * @param <G> The type of the seventh argument.
     * @param <H> The type of the eighth argument.
     * @param <I> The type of the ninth argument.
     */
    public static interface ArgChecker9<A,B,C,D,E,F,G,H,I>{
        public void check(A a, B b, C c, D d, E e, F f, G g, H h, I i);
    }

    /**
     * Supertype for parameterless signals.
     */
    public static abstract class AbstractPrivateSignal0 extends AbstractSignal {
        AbstractPrivateSignal0(){super();}
        
        AbstractPrivateSignal0(Class<?> declaringClass) {
            super(declaringClass);
        }
        
        AbstractPrivateSignal0(@StrictNonNull String signalName) {
            super(signalName, new Class<?>[0]);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         */
        public final QMetaObject.@NonNull Connection connect(@StrictNonNull Slot0 slot, Qt.@NonNull ConnectionType @NonNull... connectionType) {
            return addConnectionToSlotObject(slot, connectionType);
        }
    
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(@StrictNonNull Slot0 slot) {
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
        public final QMetaObject.@NonNull Connection connect(@StrictNonNull Connectable0 signal, Qt.@NonNull ConnectionType @NonNull... connectionType) {
            return addConnectionToSignalObject((AbstractSignal)signal, connectionType);
        }
    
        /**
         * Removes the connection to the given <i>signal</i>.
         * 
         * @param signal the signal to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(@StrictNonNull Connectable0 signal) {
            return removeConnectionToSignalObject((AbstractSignal)signal);
        }
    }

    /**
     * Supertype for all signals with one parameter.
     * @param <A> The type of the first parameter of the signal.
     */
    public static abstract class AbstractPrivateSignal1<A> extends AbstractSignal {
        
        AbstractPrivateSignal1(){
        	super();
    	}
        
        @SuppressWarnings("unchecked")
		AbstractPrivateSignal1(@StrictNonNull ArgChecker1<A> argumentTest){
        	super((array)->argumentTest.check((A)array[0]));
    	}
        
        AbstractPrivateSignal1(Class<?> declaringClass) {
            super(declaringClass);
        }
        
        AbstractPrivateSignal1(@StrictNonNull String signalName, Class<A> typeA) {
            super(signalName, typeA);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         */
        public final QMetaObject.@NonNull Connection connect(@StrictNonNull Slot0 slot, Qt.@NonNull ConnectionType @NonNull... connectionType) {
            return addConnectionToSlotObject(slot, connectionType);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         */
        public final QMetaObject.@NonNull Connection connect(@StrictNonNull Slot1<? super A> slot, Qt.@NonNull ConnectionType @NonNull... connectionType) {
            return addConnectionToSlotObject(slot, connectionType);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(@StrictNonNull Slot0 slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(@StrictNonNull Slot1<? super A> slot) {
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
        public final QMetaObject.@NonNull Connection connect(@StrictNonNull Connectable1<? super A> signal, Qt.@NonNull ConnectionType @NonNull... connectionType) {
            return addConnectionToSignalObject((AbstractSignal)signal, connectionType);
        }
        
        /**
         * Removes the connection to the given <i>signal</i>.
         * 
         * @param signal the signal to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(@StrictNonNull Connectable1<? super A> signal) {
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
        
        @SuppressWarnings("unchecked")
		AbstractPrivateSignal2(@StrictNonNull ArgChecker2<A,B> argumentTest){
        	super((array)->argumentTest.check((A)array[0], (B)array[1]));
    	}
    
        AbstractPrivateSignal2(Class<?> declaringClass) {
            super(declaringClass);
        }
        
        AbstractPrivateSignal2(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB) {
            super(signalName, typeA, typeB);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         */
        public final QMetaObject.@NonNull Connection connect(@StrictNonNull Slot0 slot, Qt.@NonNull ConnectionType @NonNull... connectionType) {
            return addConnectionToSlotObject(slot, connectionType);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         */
        public final QMetaObject.@NonNull Connection connect(@StrictNonNull Slot1<? super A> slot, Qt.@NonNull ConnectionType @NonNull... connectionType) {
            return addConnectionToSlotObject(slot, connectionType);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         */
        public final QMetaObject.@NonNull Connection connect(@StrictNonNull Slot2<? super A,? super B> slot, Qt.@NonNull ConnectionType @NonNull... connectionType) {
            return addConnectionToSlotObject(slot, connectionType);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(@StrictNonNull Slot0 slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(@StrictNonNull Slot1<? super A> slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(@StrictNonNull Slot2<? super A,? super B> slot) {
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
        public final QMetaObject.@NonNull Connection connect(@StrictNonNull Connectable2<? super A,? super B> signal, Qt.@NonNull ConnectionType @NonNull... connectionType) {
            return addConnectionToSignalObject((AbstractSignal)signal, connectionType);
        }
        
        /**
         * Removes the connection to the given <i>signal</i>.
         * 
         * @param signal the signal to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(@StrictNonNull Connectable2<? super A,? super B> signal) {
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
        
        @SuppressWarnings("unchecked")
		AbstractPrivateSignal3(@StrictNonNull ArgChecker3<A,B,C> argumentTest){
        	super((array)->argumentTest.check((A)array[0], (B)array[1], (C)array[2]));
    	}
        
        AbstractPrivateSignal3(Class<?> declaringClass) {
            super(declaringClass);
        }
        
        AbstractPrivateSignal3(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC) {
            super(signalName, typeA, typeB, typeC);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         */
        public final QMetaObject.@NonNull Connection connect(@StrictNonNull Slot0 slot, Qt.@NonNull ConnectionType @NonNull... connectionType) {
            return addConnectionToSlotObject(slot, connectionType);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         */
        public final QMetaObject.@NonNull Connection connect(@StrictNonNull Slot1<? super A> slot, Qt.@NonNull ConnectionType @NonNull... connectionType) {
            return addConnectionToSlotObject(slot, connectionType);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         */
        public final QMetaObject.@NonNull Connection connect(@StrictNonNull Slot2<? super A,? super B> slot, Qt.@NonNull ConnectionType @NonNull... connectionType) {
            return addConnectionToSlotObject(slot, connectionType);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         */
        public final QMetaObject.@NonNull Connection connect(@StrictNonNull Slot3<? super A,? super B,? super C> slot, Qt.@NonNull ConnectionType @NonNull... connectionType) {
            return addConnectionToSlotObject(slot, connectionType);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(@StrictNonNull Slot0 slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(@StrictNonNull Slot1<? super A> slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(@StrictNonNull Slot2<? super A,? super B> slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(@StrictNonNull Slot3<? super A,? super B,? super C> slot) {
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
        public final QMetaObject.@NonNull Connection connect(@StrictNonNull Connectable3<? super A,? super B,? super C> signal, Qt.@NonNull ConnectionType @NonNull... connectionType) {
            return addConnectionToSignalObject((AbstractSignal)signal, connectionType);
        }
        
        /**
         * Removes the connection to the given <i>signal</i>.
         * 
         * @param signal the signal to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(@StrictNonNull Connectable3<? super A,? super B,? super C> signal) {
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
        
        @SuppressWarnings("unchecked")
		AbstractPrivateSignal4(@StrictNonNull ArgChecker4<A,B,C,D> argumentTest){
        	super((array)->argumentTest.check((A)array[0], (B)array[1], (C)array[2], (D)array[3]));
    	}
        
        AbstractPrivateSignal4(Class<?> declaringClass) {
            super(declaringClass);
        }
        
        AbstractPrivateSignal4(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD) {
            super(signalName, typeA, typeB, typeC, typeD);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         */
        public final QMetaObject.@NonNull Connection connect(@StrictNonNull Slot0 slot, Qt.@NonNull ConnectionType @NonNull... connectionType) {
            return addConnectionToSlotObject(slot, connectionType);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         */
        public final QMetaObject.@NonNull Connection connect(@StrictNonNull Slot1<? super A> slot, Qt.@NonNull ConnectionType @NonNull... connectionType) {
            return addConnectionToSlotObject(slot, connectionType);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         */
        public final QMetaObject.@NonNull Connection connect(@StrictNonNull Slot2<? super A,? super B> slot, Qt.@NonNull ConnectionType @NonNull... connectionType) {
            return addConnectionToSlotObject(slot, connectionType);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         */
        public final QMetaObject.@NonNull Connection connect(@StrictNonNull Slot3<? super A,? super B,? super C> slot, Qt.@NonNull ConnectionType @NonNull... connectionType) {
            return addConnectionToSlotObject(slot, connectionType);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         */
        public final QMetaObject.@NonNull Connection connect(@StrictNonNull Slot4<? super A,? super B,? super C,? super D> slot, Qt.@NonNull ConnectionType @NonNull... connectionType) {
            return addConnectionToSlotObject(slot, connectionType);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(@StrictNonNull Slot0 slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(@StrictNonNull Slot1<? super A> slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(@StrictNonNull Slot2<? super A,? super B> slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(@StrictNonNull Slot3<? super A,? super B,? super C> slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(@StrictNonNull Slot4<? super A,? super B,? super C,? super D> slot) {
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
        public final QMetaObject.@NonNull Connection connect(@StrictNonNull Connectable4<? super A,? super B,? super C,? super D> signal, Qt.@NonNull ConnectionType @NonNull... connectionType) {
            return addConnectionToSignalObject((AbstractSignal)signal, connectionType);
        }
        
        /**
         * Removes the connection to the given <i>signal</i>.
         * 
         * @param signal the signal to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(@StrictNonNull Connectable4<? super A,? super B,? super C,? super D> signal) {
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
        
        @SuppressWarnings("unchecked")
		AbstractPrivateSignal5(@StrictNonNull ArgChecker5<A,B,C,D,E> argumentTest){
        	super((array)->argumentTest.check((A)array[0], (B)array[1], (C)array[2], (D)array[3], (E)array[4]));
    	}
        
        AbstractPrivateSignal5(Class<?> declaringClass) {
            super(declaringClass);
        }
        
        AbstractPrivateSignal5(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE) {
            super(signalName, typeA, typeB, typeC, typeD, typeE);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         */
        public final QMetaObject.@NonNull Connection connect(@StrictNonNull Slot0 slot, Qt.@NonNull ConnectionType @NonNull... connectionType) {
            return addConnectionToSlotObject(slot, connectionType);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         */
        public final QMetaObject.@NonNull Connection connect(@StrictNonNull Slot1<? super A> slot, Qt.@NonNull ConnectionType @NonNull... connectionType) {
            return addConnectionToSlotObject(slot, connectionType);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         */
        public final QMetaObject.@NonNull Connection connect(@StrictNonNull Slot2<? super A,? super B> slot, Qt.@NonNull ConnectionType @NonNull... connectionType) {
            return addConnectionToSlotObject(slot, connectionType);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         */
        public final QMetaObject.@NonNull Connection connect(@StrictNonNull Slot3<? super A,? super B,? super C> slot, Qt.@NonNull ConnectionType @NonNull... connectionType) {
            return addConnectionToSlotObject(slot, connectionType);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         */
        public final QMetaObject.@NonNull Connection connect(@StrictNonNull Slot4<? super A,? super B,? super C,? super D> slot, Qt.@NonNull ConnectionType @NonNull... connectionType) {
            return addConnectionToSlotObject(slot, connectionType);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         */
        public final QMetaObject.@NonNull Connection connect(@StrictNonNull Slot5<? super A,? super B,? super C,? super D,? super E> slot, Qt.@NonNull ConnectionType @NonNull... connectionType) {
            return addConnectionToSlotObject(slot, connectionType);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(@StrictNonNull Slot0 slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(@StrictNonNull Slot1<? super A> slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(@StrictNonNull Slot2<? super A,? super B> slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(@StrictNonNull Slot3<? super A,? super B,? super C> slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(@StrictNonNull Slot4<? super A,? super B,? super C,? super D> slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(@StrictNonNull Slot5<? super A,? super B,? super C,? super D,? super E> slot) {
            return removeConnectionToSlotObject(slot);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param signal the signal to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         */
        public final QMetaObject.@NonNull Connection connect(@StrictNonNull Connectable5<? super A,? super B,? super C,? super D,? super E> signal, Qt.@NonNull ConnectionType @NonNull... connectionType) {
            return addConnectionToSignalObject((AbstractSignal)signal, connectionType);
        }
        
        /**
         * Removes the connection to the given <i>signal</i>.
         * 
         * @param signal the signal to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(@StrictNonNull Connectable5<? super A,? super B,? super C,? super D,? super E> signal) {
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
        
        @SuppressWarnings("unchecked")
		AbstractPrivateSignal6(@StrictNonNull ArgChecker6<A,B,C,D,E,F> argumentTest){
        	super((array)->argumentTest.check((A)array[0], (B)array[1], (C)array[2], (D)array[3], (E)array[4], (F)array[5]));
    	}
        
        AbstractPrivateSignal6(Class<?> declaringClass) {
            super(declaringClass);
        }
        
        AbstractPrivateSignal6(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF) {
            super(signalName, typeA, typeB, typeC, typeD, typeE, typeF);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         */
        public final QMetaObject.@NonNull Connection connect(@StrictNonNull Slot0 slot, Qt.@NonNull ConnectionType @NonNull... connectionType) {
            return addConnectionToSlotObject(slot, connectionType);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         */
        public final QMetaObject.@NonNull Connection connect(@StrictNonNull Slot1<? super A> slot, Qt.@NonNull ConnectionType @NonNull... connectionType) {
            return addConnectionToSlotObject(slot, connectionType);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         */
        public final QMetaObject.@NonNull Connection connect(@StrictNonNull Slot2<? super A,? super B> slot, Qt.@NonNull ConnectionType @NonNull... connectionType) {
            return addConnectionToSlotObject(slot, connectionType);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         */
        public final QMetaObject.@NonNull Connection connect(@StrictNonNull Slot3<? super A,? super B,? super C> slot, Qt.@NonNull ConnectionType @NonNull... connectionType) {
            return addConnectionToSlotObject(slot, connectionType);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         */
        public final QMetaObject.@NonNull Connection connect(@StrictNonNull Slot4<? super A,? super B,? super C,? super D> slot, Qt.@NonNull ConnectionType @NonNull... connectionType) {
            return addConnectionToSlotObject(slot, connectionType);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         */
        public final QMetaObject.@NonNull Connection connect(@StrictNonNull Slot5<? super A,? super B,? super C,? super D,? super E> slot, Qt.@NonNull ConnectionType @NonNull... connectionType) {
            return addConnectionToSlotObject(slot, connectionType);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         */
        public final QMetaObject.@NonNull Connection connect(@StrictNonNull Slot6<? super A,? super B,? super C,? super D,? super E,? super F> slot, Qt.@NonNull ConnectionType @NonNull... connectionType) {
            return addConnectionToSlotObject(slot, connectionType);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(@StrictNonNull Slot0 slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(@StrictNonNull Slot1<? super A> slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(@StrictNonNull Slot2<? super A,? super B> slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(@StrictNonNull Slot3<? super A,? super B,? super C> slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(@StrictNonNull Slot4<? super A,? super B,? super C,? super D> slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(@StrictNonNull Slot5<? super A,? super B,? super C,? super D,? super E> slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(@StrictNonNull Slot6<? super A,? super B,? super C,? super D,? super E,? super F> slot) {
            return removeConnectionToSlotObject(slot);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param signal the signal to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         */
        public final QMetaObject.@NonNull Connection connect(@StrictNonNull Connectable6<? super A,? super B,? super C,? super D,? super E,? super F> signal, Qt.@NonNull ConnectionType @NonNull... connectionType) {
            return addConnectionToSignalObject((AbstractSignal)signal, connectionType);
        }
        
        /**
         * Removes the connection to the given <i>signal</i>.
         * 
         * @param signal the signal to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(@StrictNonNull Connectable6<? super A,? super B,? super C,? super D,? super E,? super F> signal) {
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
        
        @SuppressWarnings("unchecked")
		AbstractPrivateSignal7(@StrictNonNull ArgChecker7<A,B,C,D,E,F,G> argumentTest){
        	super((array)->argumentTest.check((A)array[0], (B)array[1], (C)array[2], (D)array[3], (E)array[4],
        			(F)array[5], (G)array[6]));
    	}
        
        AbstractPrivateSignal7(Class<?> declaringClass) {
            super(declaringClass);
        }
        
        AbstractPrivateSignal7(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Class<G> typeG) {
            super(signalName, typeA, typeB, typeC, typeD, typeE, typeF, typeG);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         */
        public final QMetaObject.@NonNull Connection connect(@StrictNonNull Slot0 slot, Qt.@NonNull ConnectionType @NonNull... connectionType) {
            return addConnectionToSlotObject(slot, connectionType);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         */
        public final QMetaObject.@NonNull Connection connect(@StrictNonNull Slot1<? super A> slot, Qt.@NonNull ConnectionType @NonNull... connectionType) {
            return addConnectionToSlotObject(slot, connectionType);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         */
        public final QMetaObject.@NonNull Connection connect(@StrictNonNull Slot2<? super A,? super B> slot, Qt.@NonNull ConnectionType @NonNull... connectionType) {
            return addConnectionToSlotObject(slot, connectionType);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         */
        public final QMetaObject.@NonNull Connection connect(@StrictNonNull Slot3<? super A,? super B,? super C> slot, Qt.@NonNull ConnectionType @NonNull... connectionType) {
            return addConnectionToSlotObject(slot, connectionType);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         */
        public final QMetaObject.@NonNull Connection connect(@StrictNonNull Slot4<? super A,? super B,? super C,? super D> slot, Qt.@NonNull ConnectionType @NonNull... connectionType) {
            return addConnectionToSlotObject(slot, connectionType);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         */
        public final QMetaObject.@NonNull Connection connect(@StrictNonNull Slot5<? super A,? super B,? super C,? super D,? super E> slot, Qt.@NonNull ConnectionType @NonNull... connectionType) {
            return addConnectionToSlotObject(slot, connectionType);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         */
        public final QMetaObject.@NonNull Connection connect(@StrictNonNull Slot6<? super A,? super B,? super C,? super D,? super E,? super F> slot, Qt.@NonNull ConnectionType @NonNull... connectionType) {
            return addConnectionToSlotObject(slot, connectionType);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         */
        public final QMetaObject.@NonNull Connection connect(@StrictNonNull Slot7<? super A,? super B,? super C,? super D,? super E,? super F,? super G> slot, Qt.@NonNull ConnectionType @NonNull... connectionType) {
            return addConnectionToSlotObject(slot, connectionType);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(@StrictNonNull Slot0 slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(@StrictNonNull Slot1<? super A> slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(@StrictNonNull Slot2<? super A,? super B> slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(@StrictNonNull Slot3<? super A,? super B,? super C> slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(@StrictNonNull Slot4<? super A,? super B,? super C,? super D> slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(@StrictNonNull Slot5<? super A,? super B,? super C,? super D,? super E> slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(@StrictNonNull Slot6<? super A,? super B,? super C,? super D,? super E,? super F> slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(@StrictNonNull Slot7<? super A,? super B,? super C,? super D,? super E,? super F,? super G> slot) {
            return removeConnectionToSlotObject(slot);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param signal the signal to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         */
        public final QMetaObject.@NonNull Connection connect(@StrictNonNull Connectable7<? super A,? super B,? super C,? super D,? super E,? super F,? super G> signal, Qt.@NonNull ConnectionType @NonNull... connectionType) {
            return addConnectionToSignalObject((AbstractSignal)signal, connectionType);
        }
        
        /**
         * Removes the connection to the given <i>signal</i>.
         * 
         * @param signal the signal to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(@StrictNonNull Connectable7<? super A,? super B,? super C,? super D,? super E,? super F,? super G> signal) {
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
        
        @SuppressWarnings("unchecked")
		AbstractPrivateSignal8(@StrictNonNull ArgChecker8<A,B,C,D,E,F,G,H> argumentTest){
        	super((array)->argumentTest.check((A)array[0], (B)array[1], (C)array[2], (D)array[3], (E)array[4],
        			(F)array[5], (G)array[6], (H)array[7]));
    	}
        
        AbstractPrivateSignal8(Class<?> declaringClass) {
            super(declaringClass);
        }
        
        AbstractPrivateSignal8(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Class<G> typeG, @StrictNonNull Class<H> typeH) {
            super(signalName, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         */
        public final QMetaObject.@NonNull Connection connect(@StrictNonNull Slot0 slot, Qt.@NonNull ConnectionType @NonNull... connectionType) {
            return addConnectionToSlotObject(slot, connectionType);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         */
        public final QMetaObject.@NonNull Connection connect(@StrictNonNull Slot1<? super A> slot, Qt.@NonNull ConnectionType @NonNull... connectionType) {
            return addConnectionToSlotObject(slot, connectionType);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         */
        public final QMetaObject.@NonNull Connection connect(@StrictNonNull Slot2<? super A,? super B> slot, Qt.@NonNull ConnectionType @NonNull... connectionType) {
            return addConnectionToSlotObject(slot, connectionType);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         */
        public final QMetaObject.@NonNull Connection connect(@StrictNonNull Slot3<? super A,? super B,? super C> slot, Qt.@NonNull ConnectionType @NonNull... connectionType) {
            return addConnectionToSlotObject(slot, connectionType);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         */
        public final QMetaObject.@NonNull Connection connect(@StrictNonNull Slot4<? super A,? super B,? super C,? super D> slot, Qt.@NonNull ConnectionType @NonNull... connectionType) {
            return addConnectionToSlotObject(slot, connectionType);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         */
        public final QMetaObject.@NonNull Connection connect(@StrictNonNull Slot5<? super A,? super B,? super C,? super D,? super E> slot, Qt.@NonNull ConnectionType @NonNull... connectionType) {
            return addConnectionToSlotObject(slot, connectionType);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         */
        public final QMetaObject.@NonNull Connection connect(@StrictNonNull Slot6<? super A,? super B,? super C,? super D,? super E,? super F> slot, Qt.@NonNull ConnectionType @NonNull... connectionType) {
            return addConnectionToSlotObject(slot, connectionType);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         */
        public final QMetaObject.@NonNull Connection connect(@StrictNonNull Slot7<? super A,? super B,? super C,? super D,? super E,? super F,? super G> slot, Qt.@NonNull ConnectionType @NonNull... connectionType) {
            return addConnectionToSlotObject(slot, connectionType);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         */
        public final QMetaObject.@NonNull Connection connect(@StrictNonNull Slot8<? super A,? super B,? super C,? super D,? super E,? super F,? super G,? super H> slot, Qt.@NonNull ConnectionType @NonNull... connectionType) {
            return addConnectionToSlotObject(slot, connectionType);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(@StrictNonNull Slot0 slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(@StrictNonNull Slot1<? super A> slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(@StrictNonNull Slot2<? super A,? super B> slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(@StrictNonNull Slot3<? super A,? super B,? super C> slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(@StrictNonNull Slot4<? super A,? super B,? super C,? super D> slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(@StrictNonNull Slot5<? super A,? super B,? super C,? super D,? super E> slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(@StrictNonNull Slot6<? super A,? super B,? super C,? super D,? super E,? super F> slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(@StrictNonNull Slot7<? super A,? super B,? super C,? super D,? super E,? super F,? super G> slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(@StrictNonNull Slot8<? super A,? super B,? super C,? super D,? super E,? super F,? super G,? super H> slot) {
            return removeConnectionToSlotObject(slot);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param signal the signal to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         */
        public final QMetaObject.@NonNull Connection connect(@StrictNonNull Connectable8<? super A,? super B,? super C,? super D,? super E,? super F,? super G,? super H> signal, Qt.@NonNull ConnectionType @NonNull... connectionType) {
            return addConnectionToSignalObject((AbstractSignal)signal, connectionType);
        }
        
        /**
         * Removes the connection to the given <i>signal</i>.
         * 
         * @param signal the signal to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(@StrictNonNull Connectable8<? super A,? super B,? super C,? super D,? super E,? super F,? super G,? super H> signal) {
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
        
        @SuppressWarnings("unchecked")
		AbstractPrivateSignal9(@StrictNonNull ArgChecker9<A,B,C,D,E,F,G,H,I> argumentTest){
        	super((array)->argumentTest.check((A)array[0], (B)array[1], (C)array[2], (D)array[3], (E)array[4],
        			(F)array[5], (G)array[6], (H)array[7], (I)array[8]));
    	}
        
        AbstractPrivateSignal9(Class<?> declaringClass) {
            super(declaringClass);
        }
        
        AbstractPrivateSignal9(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Class<G> typeG, @StrictNonNull Class<H> typeH, @StrictNonNull Class<I> typeI) {
            super(signalName, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH, typeI);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         */
        public final QMetaObject.@NonNull Connection connect(@StrictNonNull Slot0 slot, Qt.@NonNull ConnectionType @NonNull... connectionType) {
            return addConnectionToSlotObject(slot, connectionType);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         */
        public final QMetaObject.@NonNull Connection connect(@StrictNonNull Slot1<? super A> slot, Qt.@NonNull ConnectionType @NonNull... connectionType) {
            return addConnectionToSlotObject(slot, connectionType);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         */
        public final QMetaObject.@NonNull Connection connect(@StrictNonNull Slot2<? super A,? super B> slot, Qt.@NonNull ConnectionType @NonNull... connectionType) {
            return addConnectionToSlotObject(slot, connectionType);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         */
        public final QMetaObject.@NonNull Connection connect(@StrictNonNull Slot3<? super A,? super B,? super C> slot, Qt.@NonNull ConnectionType @NonNull... connectionType) {
            return addConnectionToSlotObject(slot, connectionType);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         */
        public final QMetaObject.@NonNull Connection connect(@StrictNonNull Slot4<? super A,? super B,? super C,? super D> slot, Qt.@NonNull ConnectionType @NonNull... connectionType) {
            return addConnectionToSlotObject(slot, connectionType);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         */
        public final QMetaObject.@NonNull Connection connect(@StrictNonNull Slot5<? super A,? super B,? super C,? super D,? super E> slot, Qt.@NonNull ConnectionType @NonNull... connectionType) {
            return addConnectionToSlotObject(slot, connectionType);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         */
        public final QMetaObject.@NonNull Connection connect(@StrictNonNull Slot6<? super A,? super B,? super C,? super D,? super E,? super F> slot, Qt.@NonNull ConnectionType @NonNull... connectionType) {
            return addConnectionToSlotObject(slot, connectionType);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         */
        public final QMetaObject.@NonNull Connection connect(@StrictNonNull Slot7<? super A,? super B,? super C,? super D,? super E,? super F,? super G> slot, Qt.@NonNull ConnectionType @NonNull... connectionType) {
            return addConnectionToSlotObject(slot, connectionType);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         */
        public final QMetaObject.@NonNull Connection connect(@StrictNonNull Slot8<? super A,? super B,? super C,? super D,? super E,? super F,? super G,? super H> slot, Qt.@NonNull ConnectionType @NonNull... connectionType) {
            return addConnectionToSlotObject(slot, connectionType);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param slot the slot to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         */
        public final QMetaObject.@NonNull Connection connect(@StrictNonNull Slot9<? super A,? super B,? super C,? super D,? super E,? super F,? super G,? super H,? super I> slot, Qt.@NonNull ConnectionType @NonNull... connectionType) {
            return addConnectionToSlotObject(slot, connectionType);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(@StrictNonNull Slot0 slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(@StrictNonNull Slot1<? super A> slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(@StrictNonNull Slot2<? super A,? super B> slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(@StrictNonNull Slot3<? super A,? super B,? super C> slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(@StrictNonNull Slot4<? super A,? super B,? super C,? super D> slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(@StrictNonNull Slot5<? super A,? super B,? super C,? super D,? super E> slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(@StrictNonNull Slot6<? super A,? super B,? super C,? super D,? super E,? super F> slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(@StrictNonNull Slot7<? super A,? super B,? super C,? super D,? super E,? super F,? super G> slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(@StrictNonNull Slot8<? super A,? super B,? super C,? super D,? super E,? super F,? super G,? super H> slot) {
            return removeConnectionToSlotObject(slot);
        }
        
        /**
         * Removes the connection to the given <i>slot</i>.
         * 
         * @param slot the slot to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(@StrictNonNull Slot9<? super A,? super B,? super C,? super D,? super E,? super F,? super G,? super H,? super I> slot) {
            return removeConnectionToSlotObject(slot);
        }
    
        /**
         * Initializes a connection to the <i>slot</i>.
         * 
         * @param signal the signal to be connected
         * @param connectionType type of connection
         * @return connection if successful or <code>null</code> otherwise
         * @throws io.qt.QMisfittingSignatureException Raised if their signatures are incompatible.
         */
        public final QMetaObject.@NonNull Connection connect(@StrictNonNull Connectable9<? super A,? super B,? super C,? super D,? super E,? super F,? super G,? super H,? super I> signal, Qt.@NonNull ConnectionType @NonNull... connectionType) {
            return addConnectionToSignalObject((AbstractSignal)signal, connectionType);
        }
        
        /**
         * Removes the connection to the given <i>signal</i>.
         * 
         * @param signal the signal to be disconnected
         * @return <code>true</code> if successfully disconnected, or <code>false</code> otherwise.
         */
        public final boolean disconnect(@StrictNonNull Connectable9<? super A,? super B,? super C,? super D,? super E,? super F,? super G,? super H,? super I> signal) {
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
        
        AbstractPublicSignal0(Class<?> declaringClass) {
            super(declaringClass);
        }
        
        AbstractPublicSignal0(@StrictNonNull String signalName) {
            super(signalName);
        }
        
        private final static Object[] zeroarray = {};
        
        /**
         * Emits the signal.
         */
        @Override
        public final void emit() {
            emitSignal(zeroarray);
        }
        
        /**
         * @hidden
         */
        @Override
        protected final boolean isPrivateSignal() {
        	return false;
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
		AbstractPublicSignal1(){
			super();
		}
        
        AbstractPublicSignal1(@StrictNonNull ArgChecker1<A> argumentTest) {
			super(argumentTest);
		}

		AbstractPublicSignal1(Class<?> declaringClass) {
            super(declaringClass);
        }
        
        AbstractPublicSignal1(@StrictNonNull String signalName, Class<A> typeA) {
            super(signalName, typeA);
        }
        
        /**
         * Emits the signal.
         * @param arg1
         */
        @Override
        public final void emit(A arg1) {
            emitSignal(arg1);
        }

        /**
         * @hidden
         */
        @Override
        protected final boolean isPrivateSignal() {
        	return false;
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
        
        AbstractPublicSignal2(@StrictNonNull ArgChecker2<A, B> argumentTest) {
			super(argumentTest);
		}

		AbstractPublicSignal2(Class<?> declaringClass) {
            super(declaringClass);
        }
        
        AbstractPublicSignal2(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB) {
            super(signalName, typeA, typeB);
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

        /**
         * @hidden
         */
        @Override
        protected final boolean isPrivateSignal() {
        	return false;
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
        
        AbstractPublicSignal3(@StrictNonNull ArgChecker3<A, B, C> argumentTest) {
			super(argumentTest);
		}

		AbstractPublicSignal3(Class<?> declaringClass) {
            super(declaringClass);
        }
        
        AbstractPublicSignal3(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC) {
            super(signalName, typeA, typeB, typeC);
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

        /**
         * @hidden
         */
        @Override
        protected final boolean isPrivateSignal() {
        	return false;
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
        
        AbstractPublicSignal4(@StrictNonNull ArgChecker4<A, B, C, D> argumentTest) {
			super(argumentTest);
		}

		AbstractPublicSignal4(Class<?> declaringClass) {
            super(declaringClass);
        }
        
        AbstractPublicSignal4(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD) {
            super(signalName, typeA, typeB, typeC, typeD);
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

        /**
         * @hidden
         */
        @Override
        protected final boolean isPrivateSignal() {
        	return false;
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
        
        AbstractPublicSignal5(@StrictNonNull ArgChecker5<A, B, C, D, E> argumentTest) {
			super(argumentTest);
		}

		AbstractPublicSignal5(Class<?> declaringClass) {
            super(declaringClass);
        }
        
        AbstractPublicSignal5(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE) {
            super(signalName, typeA, typeB, typeC, typeD, typeE);
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

        /**
         * @hidden
         */
        @Override
        protected final boolean isPrivateSignal() {
        	return false;
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
        
        AbstractPublicSignal6(@StrictNonNull ArgChecker6<A, B, C, D, E, F> argumentTest) {
			super(argumentTest);
		}

		AbstractPublicSignal6(Class<?> declaringClass) {
            super(declaringClass);
        }
        
        AbstractPublicSignal6(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF) {
            super(signalName, typeA, typeB, typeC, typeD, typeE, typeF);
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

        /**
         * @hidden
         */
        @Override
        protected final boolean isPrivateSignal() {
        	return false;
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
        
        AbstractPublicSignal7(@StrictNonNull ArgChecker7<A, B, C, D, E, F, G> argumentTest) {
			super(argumentTest);
		}

		AbstractPublicSignal7(Class<?> declaringClass) {
            super(declaringClass);
        }
        
        AbstractPublicSignal7(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Class<G> typeG) {
            super(signalName, typeA, typeB, typeC, typeD, typeE, typeF, typeG);
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

        /**
         * @hidden
         */
        @Override
        protected final boolean isPrivateSignal() {
        	return false;
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
        
        AbstractPublicSignal8(@StrictNonNull ArgChecker8<A, B, C, D, E, F, G, H> argumentTest) {
			super(argumentTest);
		}

		AbstractPublicSignal8(Class<?> declaringClass) {
            super(declaringClass);
        }
        
        AbstractPublicSignal8(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Class<G> typeG, @StrictNonNull Class<H> typeH) {
            super(signalName, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH);
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

        /**
         * @hidden
         */
        @Override
        protected final boolean isPrivateSignal() {
        	return false;
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
        
        AbstractPublicSignal9(@StrictNonNull ArgChecker9<A, B, C, D, E, F, G, H, I> argumentTest) {
			super(argumentTest);
		}

		AbstractPublicSignal9(Class<?> declaringClass) {
            super(declaringClass);
        }
        
        AbstractPublicSignal9(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Class<G> typeG, @StrictNonNull Class<H> typeH, @StrictNonNull Class<I> typeI) {
            super(signalName, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH, typeI);
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

        /**
         * @hidden
         */
        @Override
        protected final boolean isPrivateSignal() {
        	return false;
        }
    }
    
    /**
     * Supertype for all public default signals with one parameters.
     * @param <A> The type of the first parameter of the signal.
     */
    public static abstract class AbstractSignal1Default1<A> extends AbstractPublicSignal1<A> implements  Emitable0, Connectable0 {
    	AbstractSignal1Default1() {
    		this.arg1Default = null;
    	}
    	
		AbstractSignal1Default1(@StrictNonNull Supplier<? extends A> arg1Default){
            super();
            this.arg1Default = arg1Default;
            if(this.arg1Default==null){
                throw new QNoDefaultValueException(1);
            }
        }
		
		AbstractSignal1Default1(@StrictNonNull Supplier<? extends A> arg1Default, @StrictNonNull ArgChecker1<A> argumentTest){
            super(argumentTest);
            this.arg1Default = arg1Default;
            if(this.arg1Default==null){
                throw new QNoDefaultValueException(1);
            }
        }
        
        AbstractSignal1Default1(@StrictNonNull Supplier<? extends A> arg1Default, Class<?> declaringClass){
            super(declaringClass);
            this.arg1Default = arg1Default;
            if(this.arg1Default==null){
                throw new QNoDefaultValueException(1);
            }
        }
        
        AbstractSignal1Default1(@StrictNonNull String signalName, Class<A> typeA, @StrictNonNull Supplier<? extends A> arg1Default) {
            super(signalName, typeA);
            this.arg1Default = arg1Default;
            if(this.arg1Default==null){
                throw new QNoDefaultValueException(1);
            }
        }
        
        private final Supplier<? extends A> arg1Default;
        
        /**
         * Emits the signal with default value for arg1.
         */
        @Override
        public final void emit() {
            super.emitDefaultSignal(arg1Default);
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
    	
        AbstractSignal2Default1(Supplier<? extends B> arg2Default){
            super();
            if(arg2Default!=null){
                this.arg2Default = arg2Default;
            }else{
                throw new QNoDefaultValueException(2);
            }
        }
        
        AbstractSignal2Default1(Supplier<? extends B> arg2Default, @StrictNonNull ArgChecker2<A, B> argumentTest){
            super(argumentTest);
            if(arg2Default!=null){
                this.arg2Default = arg2Default;
            }else{
                throw new QNoDefaultValueException(2);
            }
        }

		AbstractSignal2Default1(Supplier<? extends B> arg2Default, Class<?> declaringClass){
            super(declaringClass);
            if(arg2Default!=null){
                this.arg2Default = arg2Default;
            }else{
                throw new QNoDefaultValueException(2);
            }
        }
        
        AbstractSignal2Default1(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, Supplier<? extends B> arg2Default) {
            super(signalName, typeA, typeB);
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
     * Supertype for all public default signals with two parameters.
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     */
    public static abstract class AbstractSignal2Default2<A, B> extends AbstractSignal2Default1<A, B> implements  Emitable0, Connectable0 {
    	AbstractSignal2Default2() {
    		this.arg1Default = null;
    	}
    	
		AbstractSignal2Default2(@StrictNonNull Supplier<? extends A> arg1Default, Supplier<? extends B> arg2Default){
            super(arg2Default);
            this.arg1Default = arg1Default;
            if(this.arg1Default==null){
                throw new QNoDefaultValueException(1);
            }
        }
    	
		AbstractSignal2Default2(@StrictNonNull Supplier<? extends A> arg1Default, Supplier<? extends B> arg2Default, @StrictNonNull ArgChecker2<A, B> argumentTest){
            super(arg2Default, argumentTest);
            this.arg1Default = arg1Default;
            if(this.arg1Default==null){
                throw new QNoDefaultValueException(1);
            }
        }
        
        AbstractSignal2Default2(@StrictNonNull Supplier<? extends A> arg1Default, Supplier<? extends B> arg2Default, Class<?> declaringClass){
            super(arg2Default, declaringClass);
            this.arg1Default = arg1Default;
            if(this.arg1Default==null){
                throw new QNoDefaultValueException(1);
            }
        }
        
        AbstractSignal2Default2(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Supplier<? extends A> arg1Default, Supplier<? extends B> arg2Default) {
            super(signalName, typeA, typeB, arg2Default);
            this.arg1Default = arg1Default;
            if(this.arg1Default==null){
                throw new QNoDefaultValueException(1);
            }
        }
        
        private final Supplier<? extends A> arg1Default;
        
        /**
         * Emits the signal with default value for arg1.
         */
        @Override
        public final void emit() {
            super.emitDefaultSignal(arg1Default);
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
    	
        AbstractSignal3Default1(Supplier<? extends C> arg3Default){
            super();
            if(arg3Default!=null){
                this.arg3Default = arg3Default;
            }else{
                throw new QNoDefaultValueException(3);
            }
        }
        
        AbstractSignal3Default1(Supplier<? extends C> arg3Default, @StrictNonNull ArgChecker3<A, B, C> argumentTest){
            super(argumentTest);
            if(arg3Default!=null){
                this.arg3Default = arg3Default;
            }else{
                throw new QNoDefaultValueException(3);
            }
        }

		AbstractSignal3Default1(Supplier<? extends C> arg3Default, Class<?> declaringClass){
            super(declaringClass);
            if(arg3Default!=null){
                this.arg3Default = arg3Default;
            }else{
                throw new QNoDefaultValueException(3);
            }
        }
        
        AbstractSignal3Default1(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, Supplier<? extends C> arg3Default) {
            super(signalName, typeA, typeB, typeC);
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
    	
        AbstractSignal3Default2(Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default){
            super(arg3Default);
            if(arg2Default!=null){
                this.arg2Default = arg2Default;
            }else{
                throw new QNoDefaultValueException(2);
            }
        }
        
        AbstractSignal3Default2(Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, @StrictNonNull ArgChecker3<A, B, C> argumentTest){
            super(arg3Default, argumentTest);
            if(arg2Default!=null){
                this.arg2Default = arg2Default;
            }else{
                throw new QNoDefaultValueException(2);
            }
        }
    	
    	AbstractSignal3Default2(Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Class<?> declaringClass){
            super(arg3Default, declaringClass);
            if(arg2Default!=null){
                this.arg2Default = arg2Default;
            }else{
                throw new QNoDefaultValueException(2);
            }
        }
        
        AbstractSignal3Default2(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default) {
            super(signalName, typeA, typeB, typeC, arg3Default);
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
    public static abstract class AbstractSignal3Default3<A, B, C> extends AbstractSignal3Default2<A, B, C> implements  Emitable0, Connectable0 {
    	AbstractSignal3Default3() {
    		this.arg1Default = null;
    	}
    	
		AbstractSignal3Default3(@StrictNonNull Supplier<? extends A> arg1Default, Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default){
            super(arg2Default, arg3Default);
            this.arg1Default = arg1Default;
            if(this.arg1Default==null){
                throw new QNoDefaultValueException(1);
            }
        }
		
		AbstractSignal3Default3(@StrictNonNull Supplier<? extends A> arg1Default, Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, @StrictNonNull ArgChecker3<A, B, C> argumentTest){
            super(arg2Default, arg3Default, argumentTest);
            this.arg1Default = arg1Default;
            if(this.arg1Default==null){
                throw new QNoDefaultValueException(1);
            }
        }
		
		AbstractSignal3Default3(@StrictNonNull Supplier<? extends A> arg1Default, Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Class<?> declaringClass){
            super(arg2Default, arg3Default, declaringClass);
            this.arg1Default = arg1Default;
            if(this.arg1Default==null){
                throw new QNoDefaultValueException(1);
            }
        }
        
        AbstractSignal3Default3(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Supplier<? extends A> arg1Default, Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default) {
            super(signalName, typeA, typeB, typeC, arg2Default, arg3Default);
            this.arg1Default = arg1Default;
            if(this.arg1Default==null){
                throw new QNoDefaultValueException(1);
            }
        }
        
        private final Supplier<? extends A> arg1Default;
        
        /**
         * Emits the signal with default value for arg1.
         */
        @Override
        public final void emit() {
            super.emitDefaultSignal(arg1Default);
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
        
        AbstractSignal4Default1(Supplier<? extends D> arg4Default){
            super();
            if(arg4Default!=null){
                this.arg4Default = arg4Default;
            }else{
                throw new QNoDefaultValueException(4);
            }
        }
        
        AbstractSignal4Default1(Supplier<? extends D> arg4Default, @StrictNonNull ArgChecker4<A, B, C, D> argumentTest){
            super(argumentTest);
            if(arg4Default!=null){
                this.arg4Default = arg4Default;
            }else{
                throw new QNoDefaultValueException(4);
            }
        }

		AbstractSignal4Default1(Supplier<? extends D> arg4Default, Class<?> declaringClass){
            super(declaringClass);
            if(arg4Default!=null){
                this.arg4Default = arg4Default;
            }else{
                throw new QNoDefaultValueException(4);
            }
        }
        
        AbstractSignal4Default1(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, Supplier<? extends D> arg4Default) {
            super(signalName, typeA, typeB, typeC, typeD);
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
    	
        AbstractSignal4Default2(Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default){
            super(arg4Default);
            if(arg3Default!=null){
                this.arg3Default = arg3Default;
            }else{
                throw new QNoDefaultValueException(3);
            }
        }
        
        AbstractSignal4Default2(Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, @StrictNonNull ArgChecker4<A, B, C, D> argumentTest){
            super(arg4Default, argumentTest);
            if(arg3Default!=null){
                this.arg3Default = arg3Default;
            }else{
                throw new QNoDefaultValueException(3);
            }
        }
    	
    	AbstractSignal4Default2(Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Class<?> declaringClass){
            super(arg4Default, declaringClass);
            if(arg3Default!=null){
                this.arg3Default = arg3Default;
            }else{
                throw new QNoDefaultValueException(3);
            }
        }
        
        AbstractSignal4Default2(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default) {
            super(signalName, typeA, typeB, typeC, typeD, arg4Default);
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
    	
        AbstractSignal4Default3(Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default){
            super(arg3Default, arg4Default);
            if(arg2Default!=null){
                this.arg2Default = arg2Default;
            }else{
                throw new QNoDefaultValueException(2);
            }
        }
        
        AbstractSignal4Default3(Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, @StrictNonNull ArgChecker4<A, B, C, D> argumentTest){
            super(arg3Default, arg4Default, argumentTest);
            if(arg2Default!=null){
                this.arg2Default = arg2Default;
            }else{
                throw new QNoDefaultValueException(2);
            }
        }
    	
		AbstractSignal4Default3(Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Class<?> declaringClass){
            super(arg3Default, arg4Default, declaringClass);
            if(arg2Default!=null){
                this.arg2Default = arg2Default;
            }else{
                throw new QNoDefaultValueException(2);
            }
        }
        
        AbstractSignal4Default3(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default) {
            super(signalName, typeA, typeB, typeC, typeD, arg3Default, arg4Default);
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
     * Supertype for all public default signals with four parameters.
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     */
    public static abstract class AbstractSignal4Default4<A, B, C, D> extends AbstractSignal4Default3<A, B, C, D> implements  Emitable0, Connectable0 {
    	AbstractSignal4Default4() {
    		this.arg1Default = null;
    	}
    	
		AbstractSignal4Default4(@StrictNonNull Supplier<? extends A> arg1Default, Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, @StrictNonNull ArgChecker4<A, B, C, D> argumentTest){
            super(arg2Default, arg3Default, arg4Default, argumentTest);
            this.arg1Default = arg1Default;
            if(this.arg1Default==null){
                throw new QNoDefaultValueException(1);
            }
        }
    	
		AbstractSignal4Default4(@StrictNonNull Supplier<? extends A> arg1Default, Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default){
            super(arg2Default, arg3Default, arg4Default);
            this.arg1Default = arg1Default;
            if(this.arg1Default==null){
                throw new QNoDefaultValueException(1);
            }
        }
        
        AbstractSignal4Default4(@StrictNonNull Supplier<? extends A> arg1Default, Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Class<?> declaringClass){
            super(arg2Default, arg3Default, arg4Default, declaringClass);
            this.arg1Default = arg1Default;
            if(this.arg1Default==null){
                throw new QNoDefaultValueException(1);
            }
        }
        
        AbstractSignal4Default4(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Supplier<? extends A> arg1Default, Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default) {
            super(signalName, typeA, typeB, typeC, typeD, arg2Default, arg3Default, arg4Default);
            this.arg1Default = arg1Default;
            if(this.arg1Default==null){
                throw new QNoDefaultValueException(1);
            }
        }
        
        private final Supplier<? extends A> arg1Default;
        
        /**
         * Emits the signal with default value for arg1.
         */
        @Override
        public final void emit() {
            super.emitDefaultSignal(arg1Default);
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
    	
        AbstractSignal5Default1(Supplier<? extends E> arg5Default){
            super();
            if(arg5Default!=null){
                this.arg5Default = arg5Default;
            }else{
                throw new QNoDefaultValueException(5);
            }
        }
        
        AbstractSignal5Default1(Supplier<? extends E> arg5Default, @StrictNonNull ArgChecker5<A, B, C, D, E> argumentTest){
            super(argumentTest);
            if(arg5Default!=null){
                this.arg5Default = arg5Default;
            }else{
                throw new QNoDefaultValueException(5);
            }
        }

		AbstractSignal5Default1(Supplier<? extends E> arg5Default, Class<?> declaringClass){
            super(declaringClass);
            if(arg5Default!=null){
                this.arg5Default = arg5Default;
            }else{
                throw new QNoDefaultValueException(5);
            }
        }
        
        AbstractSignal5Default1(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, Supplier<? extends E> arg5Default) {
            super(signalName, typeA, typeB, typeC, typeD, typeE);
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
    	
        AbstractSignal5Default2(Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default){
            super(arg5Default);
            if(arg4Default!=null){
                this.arg4Default = arg4Default;
            }else{
                throw new QNoDefaultValueException(4);
            }
        }
        
        AbstractSignal5Default2(Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, @StrictNonNull ArgChecker5<A, B, C, D, E> argumentTest){
            super(arg5Default, argumentTest);
            if(arg4Default!=null){
                this.arg4Default = arg4Default;
            }else{
                throw new QNoDefaultValueException(4);
            }
        }
    	
		AbstractSignal5Default2(Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Class<?> declaringClass){
            super(arg5Default, declaringClass);
            if(arg4Default!=null){
                this.arg4Default = arg4Default;
            }else{
                throw new QNoDefaultValueException(4);
            }
        }
        
        AbstractSignal5Default2(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default) {
            super(signalName, typeA, typeB, typeC, typeD, typeE, arg5Default);
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
    	
        AbstractSignal5Default3(Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default){
            super(arg4Default, arg5Default);
            if(arg3Default!=null){
                this.arg3Default = arg3Default;
            }else{
                throw new QNoDefaultValueException(3);
            }
        }
        
        AbstractSignal5Default3(Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, @StrictNonNull ArgChecker5<A, B, C, D, E> argumentTest){
            super(arg4Default, arg5Default, argumentTest);
            if(arg3Default!=null){
                this.arg3Default = arg3Default;
            }else{
                throw new QNoDefaultValueException(3);
            }
        }
    	
		AbstractSignal5Default3(Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Class<?> declaringClass){
            super(arg4Default, arg5Default, declaringClass);
            if(arg3Default!=null){
                this.arg3Default = arg3Default;
            }else{
                throw new QNoDefaultValueException(3);
            }
        }
        
        AbstractSignal5Default3(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default) {
            super(signalName, typeA, typeB, typeC, typeD, typeE, arg4Default, arg5Default);
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
    	
        AbstractSignal5Default4(Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default){
            super(arg3Default, arg4Default, arg5Default);
            if(arg2Default!=null){
                this.arg2Default = arg2Default;
            }else{
                throw new QNoDefaultValueException(2);
            }
        }
        
        AbstractSignal5Default4(Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, @StrictNonNull ArgChecker5<A, B, C, D, E> argumentTest){
            super(arg3Default, arg4Default, arg5Default, argumentTest);
            if(arg2Default!=null){
                this.arg2Default = arg2Default;
            }else{
                throw new QNoDefaultValueException(2);
            }
        }
    	
		AbstractSignal5Default4(Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Class<?> declaringClass){
            super(arg3Default, arg4Default, arg5Default, declaringClass);
            if(arg2Default!=null){
                this.arg2Default = arg2Default;
            }else{
                throw new QNoDefaultValueException(2);
            }
        }
        
        AbstractSignal5Default4(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default) {
            super(signalName, typeA, typeB, typeC, typeD, typeE, arg3Default, arg4Default, arg5Default);
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
     * Supertype for all public default signals with five parameters.
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     */
    public static abstract class AbstractSignal5Default5<A, B, C, D, E> extends AbstractSignal5Default4<A, B, C, D, E> implements  Emitable0, Connectable0 {
    	AbstractSignal5Default5() {
    		this.arg1Default = null;
    	}
    	
		AbstractSignal5Default5(@StrictNonNull Supplier<? extends A> arg1Default, Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default){
            super(arg2Default, arg3Default, arg4Default, arg5Default);
            this.arg1Default = arg1Default;
            if(this.arg1Default==null){
                throw new QNoDefaultValueException(1);
            }
        }
		
		AbstractSignal5Default5(@StrictNonNull Supplier<? extends A> arg1Default, Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, @StrictNonNull ArgChecker5<A, B, C, D, E> argumentTest){
            super(arg2Default, arg3Default, arg4Default, arg5Default, argumentTest);
            this.arg1Default = arg1Default;
            if(this.arg1Default==null){
                throw new QNoDefaultValueException(1);
            }
        }
        
        AbstractSignal5Default5(@StrictNonNull Supplier<? extends A> arg1Default, Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Class<?> declaringClass){
            super(arg2Default, arg3Default, arg4Default, arg5Default, declaringClass);
            this.arg1Default = arg1Default;
            if(this.arg1Default==null){
                throw new QNoDefaultValueException(1);
            }
        }
        
        AbstractSignal5Default5(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Supplier<? extends A> arg1Default, Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default) {
            super(signalName, typeA, typeB, typeC, typeD, typeE, arg2Default, arg3Default, arg4Default, arg5Default);
            this.arg1Default = arg1Default;
            if(this.arg1Default==null){
                throw new QNoDefaultValueException(1);
            }
        }
        
        private final Supplier<? extends A> arg1Default;
        
        /**
         * Emits the signal with default value for arg1.
         */
        @Override
        public final void emit() {
            super.emitDefaultSignal(arg1Default);
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

        AbstractSignal6Default1(Supplier<? extends F> arg6Default){
            super();
            if(arg6Default!=null){
                this.arg6Default = arg6Default;
            }else{
                throw new QNoDefaultValueException(6);
            }
        }
        
        AbstractSignal6Default1(Supplier<? extends F> arg6Default, @StrictNonNull ArgChecker6<A, B, C, D, E, F> argumentTest){
            super(argumentTest);
            if(arg6Default!=null){
                this.arg6Default = arg6Default;
            }else{
                throw new QNoDefaultValueException(6);
            }
        }

		AbstractSignal6Default1(Supplier<? extends F> arg6Default, Class<?> declaringClass){
            super(declaringClass);
            if(arg6Default!=null){
                this.arg6Default = arg6Default;
            }else{
                throw new QNoDefaultValueException(6);
            }
        }
        
        AbstractSignal6Default1(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, Supplier<? extends F> arg6Default) {
            super(signalName, typeA, typeB, typeC, typeD, typeE, typeF);
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
    	
        AbstractSignal6Default2(Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default){
            super(arg6Default);
            if(arg5Default!=null){
                this.arg5Default = arg5Default;
            }else{
                throw new QNoDefaultValueException(5);
            }
        }
        
        AbstractSignal6Default2(Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, @StrictNonNull ArgChecker6<A, B, C, D, E, F> argumentTest){
            super(arg6Default, argumentTest);
            if(arg5Default!=null){
                this.arg5Default = arg5Default;
            }else{
                throw new QNoDefaultValueException(5);
            }
        }
    	
		AbstractSignal6Default2(Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Class<?> declaringClass){
            super(arg6Default, declaringClass);
            if(arg5Default!=null){
                this.arg5Default = arg5Default;
            }else{
                throw new QNoDefaultValueException(5);
            }
        }
        
        AbstractSignal6Default2(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default) {
            super(signalName, typeA, typeB, typeC, typeD, typeE, typeF, arg6Default);
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
    	
        AbstractSignal6Default3(Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default){
            super(arg5Default, arg6Default);
            if(arg4Default!=null){
                this.arg4Default = arg4Default;
            }else{
                throw new QNoDefaultValueException(4);
            }
        }
        
        AbstractSignal6Default3(Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, @StrictNonNull ArgChecker6<A, B, C, D, E, F> argumentTest){
            super(arg5Default, arg6Default, argumentTest);
            if(arg4Default!=null){
                this.arg4Default = arg4Default;
            }else{
                throw new QNoDefaultValueException(4);
            }
        }
    	
		AbstractSignal6Default3(Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Class<?> declaringClass){
            super(arg5Default, arg6Default, declaringClass);
            if(arg4Default!=null){
                this.arg4Default = arg4Default;
            }else{
                throw new QNoDefaultValueException(4);
            }
        }
        
        AbstractSignal6Default3(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default) {
            super(signalName, typeA, typeB, typeC, typeD, typeE, typeF, arg5Default, arg6Default);
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
    	
        AbstractSignal6Default4(Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default){
            super(arg4Default, arg5Default, arg6Default);
            if(arg3Default!=null){
                this.arg3Default = arg3Default;
            }else{
                throw new QNoDefaultValueException(3);
            }
        }
        
        AbstractSignal6Default4(Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, @StrictNonNull ArgChecker6<A, B, C, D, E, F> argumentTest){
            super(arg4Default, arg5Default, arg6Default, argumentTest);
            if(arg3Default!=null){
                this.arg3Default = arg3Default;
            }else{
                throw new QNoDefaultValueException(3);
            }
        }
    	
		AbstractSignal6Default4(Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Class<?> declaringClass){
            super(arg4Default, arg5Default, arg6Default, declaringClass);
            if(arg3Default!=null){
                this.arg3Default = arg3Default;
            }else{
                throw new QNoDefaultValueException(3);
            }
        }
        
        AbstractSignal6Default4(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default) {
            super(signalName, typeA, typeB, typeC, typeD, typeE, typeF, arg4Default, arg5Default, arg6Default);
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
    	
        AbstractSignal6Default5(Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default){
            super(arg3Default, arg4Default, arg5Default, arg6Default);
            if(arg2Default!=null){
                this.arg2Default = arg2Default;
            }else{
                throw new QNoDefaultValueException(2);
            }
        }
        
        AbstractSignal6Default5(Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, @StrictNonNull ArgChecker6<A, B, C, D, E, F> argumentTest){
            super(arg3Default, arg4Default, arg5Default, arg6Default, argumentTest);
            if(arg2Default!=null){
                this.arg2Default = arg2Default;
            }else{
                throw new QNoDefaultValueException(2);
            }
        }
    	
		AbstractSignal6Default5(Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Class<?> declaringClass){
            super(arg3Default, arg4Default, arg5Default, arg6Default, declaringClass);
            if(arg2Default!=null){
                this.arg2Default = arg2Default;
            }else{
                throw new QNoDefaultValueException(2);
            }
        }
        
        AbstractSignal6Default5(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default) {
            super(signalName, typeA, typeB, typeC, typeD, typeE, typeF, arg3Default, arg4Default, arg5Default, arg6Default);
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
    public static abstract class AbstractSignal6Default6<A, B, C, D, E, F> extends AbstractSignal6Default5<A, B, C, D, E, F> implements  Emitable0, Connectable0 {
    	AbstractSignal6Default6() {
    		this.arg1Default = null;
    	}
    	
		AbstractSignal6Default6(@StrictNonNull Supplier<? extends A> arg1Default, Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default){
            super(arg2Default, arg3Default, arg4Default, arg5Default, arg6Default);
            this.arg1Default = arg1Default;
            if(this.arg1Default==null){
                throw new QNoDefaultValueException(1);
            }
        }
    	
		AbstractSignal6Default6(@StrictNonNull Supplier<? extends A> arg1Default, Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, @StrictNonNull ArgChecker6<A, B, C, D, E, F> argumentTest){
            super(arg2Default, arg3Default, arg4Default, arg5Default, arg6Default, argumentTest);
            this.arg1Default = arg1Default;
            if(this.arg1Default==null){
                throw new QNoDefaultValueException(1);
            }
        }
        
        AbstractSignal6Default6(@StrictNonNull Supplier<? extends A> arg1Default, Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Class<?> declaringClass){
            super(arg2Default, arg3Default, arg4Default, arg5Default, arg6Default, declaringClass);
            this.arg1Default = arg1Default;
            if(this.arg1Default==null){
                throw new QNoDefaultValueException(1);
            }
        }
        
        AbstractSignal6Default6(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Supplier<? extends A> arg1Default, Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default) {
            super(signalName, typeA, typeB, typeC, typeD, typeE, typeF, arg2Default, arg3Default, arg4Default, arg5Default, arg6Default);
            this.arg1Default = arg1Default;
            if(this.arg1Default==null){
                throw new QNoDefaultValueException(1);
            }
        }
        
        private final Supplier<? extends A> arg1Default;
        
        /**
         * Emits the signal with default value for arg1.
         */
        @Override
        public final void emit() {
            super.emitDefaultSignal(arg1Default);
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

        AbstractSignal7Default1(Supplier<? extends G> arg7Default){
            super();
            if(arg7Default!=null){
                this.arg7Default = arg7Default;
            }else{
                throw new QNoDefaultValueException(7);
            }
        }
        
        AbstractSignal7Default1(Supplier<? extends G> arg7Default, @StrictNonNull ArgChecker7<A, B, C, D, E, F, G> argumentTest){
            super(argumentTest);
            if(arg7Default!=null){
                this.arg7Default = arg7Default;
            }else{
                throw new QNoDefaultValueException(7);
            }
        }

		AbstractSignal7Default1(Supplier<? extends G> arg7Default, Class<?> declaringClass){
            super(declaringClass);
            if(arg7Default!=null){
                this.arg7Default = arg7Default;
            }else{
                throw new QNoDefaultValueException(7);
            }
        }
        
        AbstractSignal7Default1(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Class<G> typeG, Supplier<? extends G> arg7Default) {
            super(signalName, typeA, typeB, typeC, typeD, typeE, typeF, typeG);
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

        AbstractSignal7Default2(Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default){
            super(arg7Default);
            this.arg6Default = arg6Default;
            if(this.arg6Default==null){
                throw new QNoDefaultValueException(6);
            }
        }
        
        AbstractSignal7Default2(Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, @StrictNonNull ArgChecker7<A, B, C, D, E, F, G> argumentTest){
            super(arg7Default, argumentTest);
            this.arg6Default = arg6Default;
            if(this.arg6Default==null){
                throw new QNoDefaultValueException(6);
            }
        }

		AbstractSignal7Default2(Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Class<?> declaringClass){
            super(arg7Default, declaringClass);
            this.arg6Default = arg6Default;
            if(this.arg6Default==null){
                throw new QNoDefaultValueException(6);
            }
        }
        
        AbstractSignal7Default2(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Class<G> typeG, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default) {
            super(signalName, typeA, typeB, typeC, typeD, typeE, typeF, typeG, arg7Default);
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

        AbstractSignal7Default3(Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default){
            super(arg6Default, arg7Default);
            this.arg5Default = arg5Default;
            if(this.arg5Default==null){
                throw new QNoDefaultValueException(5);
            }
        }
        
        AbstractSignal7Default3(Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, @StrictNonNull ArgChecker7<A, B, C, D, E, F, G> argumentTest){
            super(arg6Default, arg7Default, argumentTest);
            this.arg5Default = arg5Default;
            if(this.arg5Default==null){
                throw new QNoDefaultValueException(5);
            }
        }
    	
		AbstractSignal7Default3(Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Class<?> declaringClass){
            super(arg6Default, arg7Default, declaringClass);
            this.arg5Default = arg5Default;
            if(this.arg5Default==null){
                throw new QNoDefaultValueException(5);
            }
        }
        
        AbstractSignal7Default3(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Class<G> typeG, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default) {
            super(signalName, typeA, typeB, typeC, typeD, typeE, typeF, typeG, arg6Default, arg7Default);
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

        AbstractSignal7Default4(Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default){
            super(arg5Default, arg6Default, arg7Default);
            this.arg4Default = arg4Default;
            if(this.arg4Default==null){
                throw new QNoDefaultValueException(4);
            }
        }
        
        AbstractSignal7Default4(Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, @StrictNonNull ArgChecker7<A, B, C, D, E, F, G> argumentTest){
            super(arg5Default, arg6Default, arg7Default, argumentTest);
            this.arg4Default = arg4Default;
            if(this.arg4Default==null){
                throw new QNoDefaultValueException(4);
            }
        }
    	
		AbstractSignal7Default4(Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Class<?> declaringClass){
            super(arg5Default, arg6Default, arg7Default, declaringClass);
            this.arg4Default = arg4Default;
            if(this.arg4Default==null){
                throw new QNoDefaultValueException(4);
            }
        }
        
        AbstractSignal7Default4(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Class<G> typeG, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default) {
            super(signalName, typeA, typeB, typeC, typeD, typeE, typeF, typeG, arg5Default, arg6Default, arg7Default);
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
    	
        AbstractSignal7Default5(Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default){
            super(arg4Default, arg5Default, arg6Default, arg7Default);
            this.arg3Default = arg3Default;
            if(this.arg3Default==null){
                throw new QNoDefaultValueException(3);
            }
        }
        
        AbstractSignal7Default5(Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, @StrictNonNull ArgChecker7<A, B, C, D, E, F, G> argumentTest){
            super(arg4Default, arg5Default, arg6Default, arg7Default, argumentTest);
            this.arg3Default = arg3Default;
            if(this.arg3Default==null){
                throw new QNoDefaultValueException(3);
            }
        }
    	
		AbstractSignal7Default5(Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Class<?> declaringClass){
            super(arg4Default, arg5Default, arg6Default, arg7Default, declaringClass);
            this.arg3Default = arg3Default;
            if(this.arg3Default==null){
                throw new QNoDefaultValueException(3);
            }
        }
        
        AbstractSignal7Default5(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Class<G> typeG, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default) {
            super(signalName, typeA, typeB, typeC, typeD, typeE, typeF, typeG, arg4Default, arg5Default, arg6Default, arg7Default);
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
    	
        AbstractSignal7Default6(Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default){
            super(arg3Default, arg4Default, arg5Default, arg6Default, arg7Default);
            this.arg2Default = arg2Default;
            if(this.arg2Default==null){
                throw new QNoDefaultValueException(2);
            }
        }
        
        AbstractSignal7Default6(Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, @StrictNonNull ArgChecker7<A, B, C, D, E, F, G> argumentTest){
            super(arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, argumentTest);
            this.arg2Default = arg2Default;
            if(this.arg2Default==null){
                throw new QNoDefaultValueException(2);
            }
        }
    	
		AbstractSignal7Default6(Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Class<?> declaringClass){
            super(arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, declaringClass);
            this.arg2Default = arg2Default;
            if(this.arg2Default==null){
                throw new QNoDefaultValueException(2);
            }
        }
        
        AbstractSignal7Default6(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Class<G> typeG, Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default) {
            super(signalName, typeA, typeB, typeC, typeD, typeE, typeF, typeG, arg3Default, arg4Default, arg5Default, arg6Default, arg7Default);
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
     * Supertype for all public default signals with seven parameters.
     * @param <A> The type of the first parameter of the signal.
     * @param <B> The type of the second parameter of the signal.
     * @param <C> The type of the third parameter of the signal.
     * @param <D> The type of the fourth parameter of the signal.
     * @param <E> The type of the fifth parameter of the signal.
     * @param <F> The type of the sixth parameter of the signal.
     * @param <G> The type of the seventh parameter of the signal.
     */
    public static abstract class AbstractSignal7Default7<A, B, C, D, E, F, G> extends AbstractSignal7Default6<A, B, C, D, E, F, G> implements  Emitable0, Connectable0 {
    	AbstractSignal7Default7() {
    		this.arg1Default = null;
    	}
    	
		AbstractSignal7Default7(@StrictNonNull Supplier<? extends A> arg1Default, Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default){
            super(arg2Default, arg3Default, arg4Default, arg5Default, arg6Default, arg7Default);
            this.arg1Default = arg1Default;
            if(this.arg1Default==null){
                throw new QNoDefaultValueException(1);
            }
        }
    	
		AbstractSignal7Default7(@StrictNonNull Supplier<? extends A> arg1Default, Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, @StrictNonNull ArgChecker7<A, B, C, D, E, F, G> argumentTest){
            super(arg2Default, arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, argumentTest);
            this.arg1Default = arg1Default;
            if(this.arg1Default==null){
                throw new QNoDefaultValueException(1);
            }
        }
        
        AbstractSignal7Default7(@StrictNonNull Supplier<? extends A> arg1Default, Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Class<?> declaringClass){
            super(arg2Default, arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, declaringClass);
            this.arg1Default = arg1Default;
            if(this.arg1Default==null){
                throw new QNoDefaultValueException(1);
            }
        }
        
        AbstractSignal7Default7(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Class<G> typeG, @StrictNonNull Supplier<? extends A> arg1Default, Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default) {
            super(signalName, typeA, typeB, typeC, typeD, typeE, typeF, typeG, arg2Default, arg3Default, arg4Default, arg5Default, arg6Default, arg7Default);
            this.arg1Default = arg1Default;
            if(this.arg1Default==null){
                throw new QNoDefaultValueException(1);
            }
        }
        
        private final Supplier<? extends A> arg1Default;
        
        /**
         * Emits the signal with default value for arg1.
         */
        @Override
        public final void emit() {
            super.emitDefaultSignal(arg1Default);
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
        
        AbstractSignal8Default1(Supplier<? extends H> arg8Default){
            super();
            this.arg8Default = arg8Default;
            if(this.arg8Default==null){
                throw new QNoDefaultValueException(8);
            }
        }
        
        AbstractSignal8Default1(Supplier<? extends H> arg8Default, @StrictNonNull ArgChecker8<A, B, C, D, E, F, G, H> argumentTest){
            super(argumentTest);
            this.arg8Default = arg8Default;
            if(this.arg8Default==null){
                throw new QNoDefaultValueException(8);
            }
        }

		AbstractSignal8Default1(Supplier<? extends H> arg8Default, Class<?> declaringClass){
            super(declaringClass);
            this.arg8Default = arg8Default;
            if(this.arg8Default==null){
                throw new QNoDefaultValueException(8);
            }
        }
        
        AbstractSignal8Default1(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Class<G> typeG, @StrictNonNull Class<H> typeH, Supplier<? extends H> arg8Default) {
            super(signalName, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH);
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
        
        AbstractSignal8Default2(Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default){
            super(arg8Default);
            this.arg7Default = arg7Default;
            if(this.arg7Default==null){
                throw new QNoDefaultValueException(7);
            }
        }
        
        AbstractSignal8Default2(Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default, @StrictNonNull ArgChecker8<A, B, C, D, E, F, G, H> argumentTest){
            super(arg8Default, argumentTest);
            this.arg7Default = arg7Default;
            if(this.arg7Default==null){
                throw new QNoDefaultValueException(7);
            }
        }

		AbstractSignal8Default2(Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default, Class<?> declaringClass){
            super(arg8Default, declaringClass);
            this.arg7Default = arg7Default;
            if(this.arg7Default==null){
                throw new QNoDefaultValueException(7);
            }
        }
        
        AbstractSignal8Default2(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Class<G> typeG, @StrictNonNull Class<H> typeH, Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default) {
            super(signalName, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH, arg8Default);
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
    	
        AbstractSignal8Default3(Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default){
            super(arg7Default, arg8Default);
            this.arg6Default = arg6Default;
            if(this.arg6Default==null){
                throw new QNoDefaultValueException(6);
            }
        }
        
        AbstractSignal8Default3(Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default, @StrictNonNull ArgChecker8<A, B, C, D, E, F, G, H> argumentTest){
            super(arg7Default, arg8Default, argumentTest);
            this.arg6Default = arg6Default;
            if(this.arg6Default==null){
                throw new QNoDefaultValueException(6);
            }
        }
    	
		AbstractSignal8Default3(Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default, Class<?> declaringClass){
            super(arg7Default, arg8Default, declaringClass);
            this.arg6Default = arg6Default;
            if(this.arg6Default==null){
                throw new QNoDefaultValueException(6);
            }
        }
        
        AbstractSignal8Default3(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Class<G> typeG, @StrictNonNull Class<H> typeH, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default) {
            super(signalName, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH, arg7Default, arg8Default);
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
		
		AbstractSignal8Default4(Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default, @StrictNonNull ArgChecker8<A, B, C, D, E, F, G, H> argumentTest){
            super(arg6Default, arg7Default, arg8Default, argumentTest);
            this.arg5Default = arg5Default;
            if(this.arg5Default==null){
                throw new QNoDefaultValueException(5);
            }
        }
        
        AbstractSignal8Default4(Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default, Class<?> declaringClass){
            super(arg6Default, arg7Default, arg8Default, declaringClass);
            this.arg5Default = arg5Default;
            if(this.arg5Default==null){
                throw new QNoDefaultValueException(5);
            }
        }
        
        AbstractSignal8Default4(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Class<G> typeG, @StrictNonNull Class<H> typeH, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default) {
            super(signalName, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH, arg6Default, arg7Default, arg8Default);
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
    	
        AbstractSignal8Default5(Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default){
            super(arg5Default, arg6Default, arg7Default, arg8Default);
            this.arg4Default = arg4Default;
            if(this.arg4Default==null){
                throw new QNoDefaultValueException(4);
            }
        }
        
        AbstractSignal8Default5(Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default, @StrictNonNull ArgChecker8<A, B, C, D, E, F, G, H> argumentTest){
            super(arg5Default, arg6Default, arg7Default, arg8Default, argumentTest);
            this.arg4Default = arg4Default;
            if(this.arg4Default==null){
                throw new QNoDefaultValueException(4);
            }
        }
    	
		AbstractSignal8Default5(Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default, Class<?> declaringClass){
            super(arg5Default, arg6Default, arg7Default, arg8Default, declaringClass);
            this.arg4Default = arg4Default;
            if(this.arg4Default==null){
                throw new QNoDefaultValueException(4);
            }
        }
        
        AbstractSignal8Default5(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Class<G> typeG, @StrictNonNull Class<H> typeH, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default) {
            super(signalName, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH, arg5Default, arg6Default, arg7Default, arg8Default);
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
    	
        AbstractSignal8Default6(Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default){
            super(arg4Default, arg5Default, arg6Default, arg7Default, arg8Default);
            this.arg3Default = arg3Default;
            if(this.arg3Default==null){
                throw new QNoDefaultValueException(3);
            }
        }
        
        AbstractSignal8Default6(Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default, @StrictNonNull ArgChecker8<A, B, C, D, E, F, G, H> argumentTest){
            super(arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, argumentTest);
            this.arg3Default = arg3Default;
            if(this.arg3Default==null){
                throw new QNoDefaultValueException(3);
            }
        }
    	
		AbstractSignal8Default6(Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default, Class<?> declaringClass){
            super(arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, declaringClass);
            this.arg3Default = arg3Default;
            if(this.arg3Default==null){
                throw new QNoDefaultValueException(3);
            }
        }
        
        AbstractSignal8Default6(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Class<G> typeG, @StrictNonNull Class<H> typeH, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default) {
            super(signalName, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default);
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
    	
		AbstractSignal8Default7(Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default, @StrictNonNull ArgChecker8<A, B, C, D, E, F, G, H> argumentTest){
            super(arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, argumentTest);
            this.arg2Default = arg2Default;
            if(this.arg2Default==null){
                throw new QNoDefaultValueException(2);
            }
        }
        
        AbstractSignal8Default7(Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default, Class<?> declaringClass){
            super(arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, declaringClass);
            this.arg2Default = arg2Default;
            if(this.arg2Default==null){
                throw new QNoDefaultValueException(2);
            }
        }
        
        AbstractSignal8Default7(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Class<G> typeG, @StrictNonNull Class<H> typeH, Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default) {
            super(signalName, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH, arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default);
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
    public static abstract class AbstractSignal8Default8<A, B, C, D, E, F, G, H> extends AbstractSignal8Default7<A, B, C, D, E, F, G, H> implements  Emitable0, Connectable0 {
    	AbstractSignal8Default8() {
    		this.arg1Default = null;
    	}
    	
		AbstractSignal8Default8(@StrictNonNull Supplier<? extends A> arg1Default, Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default){
            super(arg2Default, arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default);
            this.arg1Default = arg1Default;
            if(this.arg1Default==null){
                throw new QNoDefaultValueException(1);
            }
        }
    	
		AbstractSignal8Default8(@StrictNonNull Supplier<? extends A> arg1Default, Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default, @StrictNonNull ArgChecker8<A, B, C, D, E, F, G, H> argumentTest){
            super(arg2Default, arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, argumentTest);
            this.arg1Default = arg1Default;
            if(this.arg1Default==null){
                throw new QNoDefaultValueException(1);
            }
        }
        
        AbstractSignal8Default8(@StrictNonNull Supplier<? extends A> arg1Default, Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default, Class<?> declaringClass){
            super(arg2Default, arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, declaringClass);
            this.arg1Default = arg1Default;
            if(this.arg1Default==null){
                throw new QNoDefaultValueException(1);
            }
        }
        
        AbstractSignal8Default8(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Class<G> typeG, @StrictNonNull Class<H> typeH, @StrictNonNull Supplier<? extends A> arg1Default, Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default) {
            super(signalName, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH, arg2Default, arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default);
            this.arg1Default = arg1Default;
            if(this.arg1Default==null){
                throw new QNoDefaultValueException(1);
            }
        }
        
        private final Supplier<? extends A> arg1Default;
        
        /**
         * Emits the signal with default value for arg1.
         */
        @Override
        public final void emit() {
            super.emitDefaultSignal(arg1Default);
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
        
		AbstractSignal9Default1(Supplier<? extends I> arg9Default, @StrictNonNull ArgChecker9<A, B, C, D, E, F, G, H, I> argumentTest){
            super(argumentTest);
            this.arg9Default = arg9Default;
            if(this.arg9Default==null){
                throw new QNoDefaultValueException(9);
            }
        }
        
        AbstractSignal9Default1(Supplier<? extends I> arg9Default, Class<?> declaringClass){
            super(declaringClass);
            this.arg9Default = arg9Default;
            if(this.arg9Default==null){
                throw new QNoDefaultValueException(9);
            }
        }
        
        AbstractSignal9Default1(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Class<G> typeG, @StrictNonNull Class<H> typeH, @StrictNonNull Class<I> typeI, Supplier<? extends I> arg9Default) {
            super(signalName, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH, typeI);
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

		AbstractSignal9Default2(Supplier<? extends H> arg8Default, Supplier<? extends I> arg9Default, @StrictNonNull ArgChecker9<A, B, C, D, E, F, G, H, I> argumentTest){
            super(arg9Default, argumentTest);
            this.arg8Default = arg8Default;
            if(this.arg8Default==null){
                throw new QNoDefaultValueException(8);
            }
        }
        
        AbstractSignal9Default2(Supplier<? extends H> arg8Default, Supplier<? extends I> arg9Default, Class<?> declaringClass){
            super(arg9Default, declaringClass);
            this.arg8Default = arg8Default;
            if(this.arg8Default==null){
                throw new QNoDefaultValueException(8);
            }
        }
        
        AbstractSignal9Default2(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Class<G> typeG, @StrictNonNull Class<H> typeH, @StrictNonNull Class<I> typeI, Supplier<? extends H> arg8Default, Supplier<? extends I> arg9Default) {
            super(signalName, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH, typeI, arg9Default);
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
		
		AbstractSignal9Default3(Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default, Supplier<? extends I> arg9Default, @StrictNonNull ArgChecker9<A, B, C, D, E, F, G, H, I> argumentTest){
            super(arg8Default, arg9Default, argumentTest);
            this.arg7Default = arg7Default;
            if(this.arg7Default==null){
                throw new QNoDefaultValueException(7);
            }
        }
        
        AbstractSignal9Default3(Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default, Supplier<? extends I> arg9Default, Class<?> declaringClass){
            super(arg8Default, arg9Default, declaringClass);
            this.arg7Default = arg7Default;
            if(this.arg7Default==null){
                throw new QNoDefaultValueException(7);
            }
        }
        
        AbstractSignal9Default3(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Class<G> typeG, @StrictNonNull Class<H> typeH, @StrictNonNull Class<I> typeI, Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default, Supplier<? extends I> arg9Default) {
            super(signalName, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH, typeI, arg8Default, arg9Default);
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
    	
		AbstractSignal9Default4(Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default, Supplier<? extends I> arg9Default, @StrictNonNull ArgChecker9<A, B, C, D, E, F, G, H, I> argumentTest){
            super(arg7Default, arg8Default, arg9Default, argumentTest);
            this.arg6Default = arg6Default;
            if(this.arg6Default==null){
                throw new QNoDefaultValueException(6);
            }
        }
        
        AbstractSignal9Default4(Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default, Supplier<? extends I> arg9Default, Class<?> declaringClass){
            super(arg7Default, arg8Default, arg9Default, declaringClass);
            this.arg6Default = arg6Default;
            if(this.arg6Default==null){
                throw new QNoDefaultValueException(6);
            }
        }
        
        AbstractSignal9Default4(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Class<G> typeG, @StrictNonNull Class<H> typeH, @StrictNonNull Class<I> typeI, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default, Supplier<? extends I> arg9Default) {
            super(signalName, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH, typeI, arg7Default, arg8Default, arg9Default);
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
		
		AbstractSignal9Default5(Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default, Supplier<? extends I> arg9Default, @StrictNonNull ArgChecker9<A, B, C, D, E, F, G, H, I> argumentTest){
            super(arg6Default, arg7Default, arg8Default, arg9Default, argumentTest);
            this.arg5Default = arg5Default;
            if(this.arg5Default==null){
                throw new QNoDefaultValueException(5);
            }
        }
        
        AbstractSignal9Default5(Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default, Supplier<? extends I> arg9Default, Class<?> declaringClass){
            super(arg6Default, arg7Default, arg8Default, arg9Default, declaringClass);
            this.arg5Default = arg5Default;
            if(this.arg5Default==null){
                throw new QNoDefaultValueException(5);
            }
        }
        
        AbstractSignal9Default5(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Class<G> typeG, @StrictNonNull Class<H> typeH, @StrictNonNull Class<I> typeI, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default, Supplier<? extends I> arg9Default) {
            super(signalName, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH, typeI, arg6Default, arg7Default, arg8Default, arg9Default);
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
    	
    	AbstractSignal9Default6(Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default, Supplier<? extends I> arg9Default, @StrictNonNull ArgChecker9<A, B, C, D, E, F, G, H, I> argumentTest){
            super(arg5Default, arg6Default, arg7Default, arg8Default, arg9Default, argumentTest);
            this.arg4Default = arg4Default;
            if(this.arg4Default==null){
                throw new QNoDefaultValueException(4);
            }
        }
        
		AbstractSignal9Default6(Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default, Supplier<? extends I> arg9Default){
            super(arg5Default, arg6Default, arg7Default, arg8Default, arg9Default);
            this.arg4Default = arg4Default;
            if(this.arg4Default==null){
                throw new QNoDefaultValueException(4);
            }
        }
        
        AbstractSignal9Default6(Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default, Supplier<? extends I> arg9Default, Class<?> declaringClass){
            super(arg5Default, arg6Default, arg7Default, arg8Default, arg9Default, declaringClass);
            this.arg4Default = arg4Default;
            if(this.arg4Default==null){
                throw new QNoDefaultValueException(4);
            }
        }
        
        AbstractSignal9Default6(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Class<G> typeG, @StrictNonNull Class<H> typeH, @StrictNonNull Class<I> typeI, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default, Supplier<? extends I> arg9Default) {
            super(signalName, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH, typeI, arg5Default, arg6Default, arg7Default, arg8Default, arg9Default);
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
    	
    	AbstractSignal9Default7(Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default, Supplier<? extends I> arg9Default, @StrictNonNull ArgChecker9<A, B, C, D, E, F, G, H, I> argumentTest){
            super(arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, arg9Default, argumentTest);
            this.arg3Default = arg3Default;
            if(this.arg3Default==null){
                throw new QNoDefaultValueException(3);
            }
        }
        
        AbstractSignal9Default7(Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default, Supplier<? extends I> arg9Default, Class<?> declaringClass){
            super(arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, arg9Default, declaringClass);
            this.arg3Default = arg3Default;
            if(this.arg3Default==null){
                throw new QNoDefaultValueException(3);
            }
        }
        
        AbstractSignal9Default7(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Class<G> typeG, @StrictNonNull Class<H> typeH, @StrictNonNull Class<I> typeI, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default, Supplier<? extends I> arg9Default) {
            super(signalName, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH, typeI, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, arg9Default);
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
		
		AbstractSignal9Default8(Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default, Supplier<? extends I> arg9Default, @StrictNonNull ArgChecker9<A, B, C, D, E, F, G, H, I> argumentTest){
            super(arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, arg9Default, argumentTest);
            this.arg2Default = arg2Default;
            if(this.arg2Default==null){
                throw new QNoDefaultValueException(2);
            }
        }
        
        AbstractSignal9Default8(Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default, Supplier<? extends I> arg9Default, Class<?> declaringClass){
            super(arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, arg9Default, declaringClass);
            this.arg2Default = arg2Default;
            if(this.arg2Default==null){
                throw new QNoDefaultValueException(2);
            }
        }
        
        AbstractSignal9Default8(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Class<G> typeG, @StrictNonNull Class<H> typeH, @StrictNonNull Class<I> typeI, Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default, Supplier<? extends I> arg9Default) {
            super(signalName, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH, typeI, arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, arg9Default);
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
    public static abstract class AbstractSignal9Default9<A, B, C, D, E, F, G, H, I> extends AbstractSignal9Default8<A, B, C, D, E, F, G, H, I> implements  Emitable0, Connectable0 {
    	AbstractSignal9Default9() {
    		this.arg1Default = null;
    	}
    	
		AbstractSignal9Default9(@StrictNonNull Supplier<? extends A> arg1Default, Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default, Supplier<? extends I> arg9Default){
            super(arg2Default, arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, arg9Default);
            this.arg1Default = arg1Default;
            if(this.arg1Default==null){
                throw new QNoDefaultValueException(1);
            }
        }
    	
		AbstractSignal9Default9(@StrictNonNull Supplier<? extends A> arg1Default, Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default, Supplier<? extends I> arg9Default, @StrictNonNull ArgChecker9<A, B, C, D, E, F, G, H, I> argumentTest){
            super(arg2Default, arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, arg9Default, argumentTest);
            this.arg1Default = arg1Default;
            if(this.arg1Default==null){
                throw new QNoDefaultValueException(1);
            }
        }
        
        AbstractSignal9Default9(@StrictNonNull Supplier<? extends A> arg1Default, Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default, Supplier<? extends I> arg9Default, Class<?> declaringClass){
            super(arg2Default, arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, arg9Default, declaringClass);
            this.arg1Default = arg1Default;
            if(this.arg1Default==null){
                throw new QNoDefaultValueException(1);
            }
        }
        
        AbstractSignal9Default9(@StrictNonNull String signalName, @StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, @StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, @StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, @StrictNonNull Class<G> typeG, @StrictNonNull Class<H> typeH, @StrictNonNull Class<I> typeI, @StrictNonNull Supplier<? extends A> arg1Default, Supplier<? extends B> arg2Default, Supplier<? extends C> arg3Default, Supplier<? extends D> arg4Default, Supplier<? extends E> arg5Default, Supplier<? extends F> arg6Default, Supplier<? extends G> arg7Default, Supplier<? extends H> arg8Default, Supplier<? extends I> arg9Default) {
            super(signalName, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH, typeI, arg2Default, arg3Default, arg4Default, arg5Default, arg6Default, arg7Default, arg8Default, arg9Default);
            this.arg1Default = arg1Default;
            if(this.arg1Default==null){
                throw new QNoDefaultValueException(1);
            }
        }
        
        private final Supplier<? extends A> arg1Default;
        
        /**
         * Emits the signal with default value for arg1.
         */
        @Override
        public final void emit() {
            super.emitDefaultSignal(arg1Default);
        }
    }
    
    @QtUninvokable
    public static @Nullable AbstractPrivateSignal0 findSignal(@StrictNonNull QObject sender, @StrictNonNull String name) {
        return (AbstractPrivateSignal0)findSignalImpl(sender, name);
    }
    
    @SuppressWarnings("unchecked")
    @QtUninvokable
    public static <A> @Nullable AbstractPrivateSignal1<A> findSignal(@StrictNonNull QObject sender, @StrictNonNull String name, @StrictNonNull Class<A> typeA) {
        return (AbstractPrivateSignal1<A>)findSignalImpl(sender, name, typeA);
    }
    
    @SuppressWarnings("unchecked")
    @QtUninvokable
    public static <A,B> @Nullable AbstractPrivateSignal2<A,B> findSignal(
    		@StrictNonNull QObject sender, @StrictNonNull String name, 
    		@StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB) {
        return (AbstractPrivateSignal2<A,B>)findSignalImpl(sender, 
                name, typeA, typeB);
    }
    
    @SuppressWarnings("unchecked")
    @QtUninvokable
    public static <A,B,C> @Nullable AbstractPrivateSignal3<A,B,C> findSignal(
    		@StrictNonNull QObject sender, @StrictNonNull String name, 
    		@StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, 
    		@StrictNonNull Class<C> typeC) {
        return (AbstractPrivateSignal3<A,B,C>)findSignalImpl(sender, 
                name, typeA, typeB, typeC);
    }
    
    @SuppressWarnings("unchecked")
    @QtUninvokable
    public static <A,B,C,D> @Nullable AbstractPrivateSignal4<A,B,C,D> findSignal(
    		@StrictNonNull QObject sender, @StrictNonNull String name, 
    		@StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, 
    		@StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD) {
        return (AbstractPrivateSignal4<A,B,C,D>)findSignalImpl(sender, 
                name, typeA, typeB, typeC, typeD);
    }
    
    @SuppressWarnings("unchecked")
    @QtUninvokable
    public static <A,B,C,D,E> @Nullable AbstractPrivateSignal5<A,B,C,D,E> findSignal(
    		@StrictNonNull QObject sender, @StrictNonNull String name, 
    		@StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, 
    		@StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, 
    		@StrictNonNull Class<E> typeE) {
        return (AbstractPrivateSignal5<A,B,C,D,E>)findSignalImpl(sender, 
                name, typeA, typeB, typeC, typeD, typeE);
    }
    
    @SuppressWarnings("unchecked")
    @QtUninvokable
    public static <A,B,C,D,E,F> @Nullable AbstractPrivateSignal6<A,B,C,D,E,F> findSignal(
    		@StrictNonNull QObject sender, @StrictNonNull String name, 
    		@StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, 
    		@StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, 
    		@StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF) {
        return (AbstractPrivateSignal6<A,B,C,D,E,F>)findSignalImpl(sender, 
                name, typeA, typeB, typeC, typeD, typeE, typeF);
    }
    
    @SuppressWarnings("unchecked")
    @QtUninvokable
    public static <A,B,C,D,E,F,G> @Nullable AbstractPrivateSignal7<A,B,C,D,E,F,G> findSignal(
    		@StrictNonNull QObject sender, @StrictNonNull String name, 
    		@StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, 
    		@StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, 
    		@StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, 
    		@StrictNonNull Class<G> typeG) {
        return (AbstractPrivateSignal7<A,B,C,D,E,F,G>)findSignalImpl(sender, 
                name, typeA, typeB, typeC, typeD, typeE, typeF, typeG);
    }
    
    @SuppressWarnings("unchecked")
    @QtUninvokable
    public static <A,B,C,D,E,F,G,H> @Nullable AbstractPrivateSignal8<A,B,C,D,E,F,G,H> findSignal(
    		@StrictNonNull QObject sender, @StrictNonNull String name, 
    		@StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, 
    		@StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, 
    		@StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, 
    		@StrictNonNull Class<G> typeG, @StrictNonNull Class<H> typeH) {
        return (AbstractPrivateSignal8<A,B,C,D,E,F,G,H>)findSignalImpl(sender, 
                name, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH);
    }
    
    @SuppressWarnings("unchecked")
    @QtUninvokable
    public static <A,B,C,D,E,F,G,H,I> @Nullable AbstractPrivateSignal9<A,B,C,D,E,F,G,H,I> findSignal(
    		@StrictNonNull QObject sender, @StrictNonNull String name, 
    		@StrictNonNull Class<A> typeA, @StrictNonNull Class<B> typeB, 
    		@StrictNonNull Class<C> typeC, @StrictNonNull Class<D> typeD, 
    		@StrictNonNull Class<E> typeE, @StrictNonNull Class<F> typeF, 
    		@StrictNonNull Class<G> typeG, @StrictNonNull Class<H> typeH, 
    		@StrictNonNull Class<I> typeI) {
        return (AbstractPrivateSignal9<A,B,C,D,E,F,G,H,I>)findSignalImpl(sender, 
                name, typeA, typeB, typeC, typeD, typeE, typeF, typeG, typeH, typeI);
    }
    
    @QtUninvokable
    public static @Nullable AbstractSignal findSignal(@StrictNonNull QObject sender, @StrictNonNull String name, @NonNull Class<?> @NonNull... types){
        return findSignalImpl(sender, name, types);		
    }
    
    @QtUninvokable
    private static AbstractSignal findSignalImpl(QObject sender, String name, Class<?>... types){
    	QMetaMethod signal = sender.metaObject().method(name, types);
        return signal.toSignal(sender);		
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
    
	private static String cppNormalizedSignature(@StrictNonNull String signalName, Class<?> classType) {
		int idx = signalName.indexOf("(");
        if(idx>=0) {
            String parameters = signalName.substring(idx).trim();
            String name = signalName.substring(0, idx);
            if(parameters.startsWith("(") && parameters.endsWith(")")) {
                parameters = parameters.substring(1, parameters.length()-1).trim();
                if(parameters.isEmpty()) {
                    return name+"()";
                }else {
                    String[] argumentTypes = parameters.split("\\,");
                    List<Parameter[]> possibleMethods = new ArrayList<>();
                    while(classType!=null) {
                    	for(Method method : classType.getDeclaredMethods()) {
                    		if(method.getParameterCount()==argumentTypes.length) {
                    			try {
                    				possibleMethods.add(method.getParameters());
                    			}catch(java.lang.reflect.MalformedParametersException e) {}
                    		}
                    	}
                    	classType = classType.getSuperclass();
                    }
                    name += "(";
                    for (int i = 0; i < argumentTypes.length; ++i) {
                        if(i>0) {
                            name += ",";
                        }
                        Class<?> paramType = null;
                        Type genericParamType = null;
                        AnnotatedElement annotatedParamType = null;
                        int arrayDimension = 0;
                        argumentTypes[i] = argumentTypes[i].replace(" ", "");
                        while(argumentTypes[i].endsWith("[]")) {
                        	++arrayDimension;
                        	argumentTypes[i] = argumentTypes[i].substring(0, argumentTypes[i].length()-2);
                        }
                        try {
                        	if(arrayDimension==0)
                        		paramType = Class.forName(argumentTypes[i].replace(" ", ""));
                        	else {
                        		String predix = "";
                        		for (int j = 0; j < arrayDimension; j++) {
                        			predix += '[';
								}
                        		switch(argumentTypes[i]) {
                        		case "int":
                            		paramType = Class.forName(predix + "I");
                        			break;
                        		case "long":
                            		paramType = Class.forName(predix + "J");
                        			break;
                        		case "short":
                            		paramType = Class.forName(predix + "S");
                        			break;
                        		case "byte":
                            		paramType = Class.forName(predix + "B");
                        			break;
                        		case "char":
                            		paramType = Class.forName(predix + "C");
                        			break;
                        		case "float":
                            		paramType = Class.forName(predix + "F");
                        			break;
                        		case "double":
                            		paramType = Class.forName(predix + "D");
                        			break;
                        		case "boolean":
                            		paramType = Class.forName(predix + "Z");
                        			break;
                    			default:
                            		paramType = Class.forName(predix + "L" + argumentTypes[i] + ";");
                    				break;
                        		}
                        	}
						} catch (Throwable e) {
						}
                        if(paramType==null && classType!=null) {
                        	try {
                        		if(arrayDimension==0)
                        			paramType = classType.getClassLoader().loadClass(argumentTypes[i].replace(" ", ""));
                        		else {
                        			String predix = "";
                            		for (int j = 0; j < arrayDimension; j++) {
                            			predix += '[';
    								}
                            		paramType = Class.forName(predix + "L" + argumentTypes[i] + ";");
                        		}
    						} catch (Throwable e) {
    						}
                        }
                        if(paramType==null) {
                        	if(arrayDimension==0) {
	                        	for(Parameter[] method : possibleMethods) {
	                        		if(method[i].getType().getName().equals(argumentTypes[i])
	                        				|| method[i].getType().getSimpleName().equals(argumentTypes[i])
	                        				|| method[i].getParameterizedType().getTypeName().equals(argumentTypes[i])) {
	                        			paramType = method[i].getType();
	                        			genericParamType = method[i].getParameterizedType();
	                        			if(ClassAnalyzerUtility.useAnnotatedType) {
	                        				annotatedParamType = method[i].getAnnotatedType();
	                        			}
	                        			break;
	                        		}
	                        	}
                        	}else {
                        		for(Parameter[] method : possibleMethods) {
                        			Class<?> type = method[i].getType();
                        			int _arrayDimension = 0;
                        			while(type.isArray()) {
                        				type = type.getComponentType();
                        				++_arrayDimension;
                        			}
                        			
                        			if(arrayDimension==_arrayDimension 
                        					&& (type.getName().equals(argumentTypes[i])
	                        				 || type.getSimpleName().equals(argumentTypes[i]))) {
	                        			paramType = method[i].getType();
	                        			genericParamType = method[i].getParameterizedType();
	                        			if(ClassAnalyzerUtility.useAnnotatedType) {
	                        				annotatedParamType = method[i].getAnnotatedType();
	                        			}
	                        			break;
	                        		}
                        		}
                        	}
                        }
                        if(arrayDimension>0) {
                        	String predix = "";
                    		for (int j = 0; j < arrayDimension; j++) {
                    			predix += '[';
							}
                    		argumentTypes[i] = predix + "L" + argumentTypes[i] + ";";
                        }
                        String cpptype = 
                        		paramType==null
                        		? internalTypeName(argumentTypes[i], QMetaObject.class.getClassLoader())
                        				: internalTypeNameOfClass(paramType, genericParamType==null ? paramType : genericParamType, annotatedParamType);
                        if(cpptype.isEmpty())
                        	cpptype = argumentTypes[i];
                        name += cpptype;
                    }
                    name += ")";
                }
                return name;
            }
            return signalName;
        }else {
            return signalName+"()";
        }
	}
	
	static QMetaMethod signalMethod(Signal signal) {
        QMetaMethod method = null;
		if(signal instanceof AbstractSignal) {
			return CoreUtility.signalMethod((AbstractSignal)signal);
        }else {
	        QtSignalEmitterInterface containingObject = signal.containingObject();
	        if(containingObject instanceof QObject) {
	            if(signal.methodIndex()>=0) {
	                method = ((QObject)containingObject).metaObject().method(signal.methodIndex());
	            }else{
	                java.util.List<Class<?>> signalTypeClasses = signal.argumentTypes();
	                method = ((QObject)containingObject).metaObject().method(signal.name(), signalTypeClasses.toArray(new Class[signalTypeClasses.size()]));
	            }
	        }
		}
        if(method==null)
            method = new QMetaMethod();
        return method;
	}
	
	private static class CoreUtility extends io.qt.internal.CoreUtility{
		static {
			QtJambi_LibraryUtilities.initialize();
		}
		protected static abstract class AbstractSignal extends io.qt.internal.CoreUtility.AbstractSignal {
	        AbstractSignal(){
	        	super();
	        }
	        
	        AbstractSignal(Consumer<Object[]> argumentTest){
				super(argumentTest);
			}
	    
	        AbstractSignal(Class<?> declaringClass) {
	            super(declaringClass);
	        }
	        
	        AbstractSignal(Class<?> declaringClass, boolean isDisposed) {
	            super(declaringClass, isDisposed);
	        }
	        
	        AbstractSignal(@StrictNonNull String signalName, Class<?>[] types) {
	            super(signalName, types);
	        }
	    }
		
		static QMetaMethod signalMethod(AbstractSignal signal) {
			return io.qt.internal.CoreUtility.signalMethod(signal);
		}
	    
	    protected static abstract class AbstractMultiSignal<S extends AbstractSignal> extends io.qt.internal.CoreUtility.AbstractMultiSignal<S> {
	        AbstractMultiSignal() {
	            super();
	        }
	    }
	    
	    protected static void checkConnectionToDisposedSignal(QMetaObject.DisposedSignal signal, Object receiver, boolean slotObject) {
	    	io.qt.internal.CoreUtility.checkConnectionToDisposedSignal(signal, receiver, slotObject);
	    }
	}
}
