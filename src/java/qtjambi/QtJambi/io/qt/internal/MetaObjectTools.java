/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
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
package io.qt.internal;

import java.io.UnsupportedEncodingException;
import java.lang.reflect.AccessibleObject;
import java.lang.reflect.AnnotatedType;
import java.lang.reflect.Constructor;
import java.lang.reflect.Field;
import java.lang.reflect.GenericArrayType;
import java.lang.reflect.Member;
import java.lang.reflect.Method;
import java.lang.reflect.Modifier;
import java.lang.reflect.Parameter;
import java.lang.reflect.ParameterizedType;
import java.lang.reflect.Type;
import java.util.AbstractList;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.HashMap;
import java.util.Hashtable;
import java.util.List;
import java.util.Map;
import java.util.logging.Logger;

import io.qt.QBlockedSlotException;
import io.qt.QFlags;
import io.qt.QNoSuchSlotException;
import io.qt.QtByteEnumerator;
import io.qt.QtClassInfo;
import io.qt.QtEnumerator;
import io.qt.QtInvokable;
import io.qt.QtLongEnumerator;
import io.qt.QtPointerType;
import io.qt.QtPropertyConstant;
import io.qt.QtPropertyDesignable;
import io.qt.QtPropertyNotify;
import io.qt.QtPropertyReader;
import io.qt.QtPropertyRequired;
import io.qt.QtPropertyResetter;
import io.qt.QtPropertyScriptable;
import io.qt.QtPropertyStored;
import io.qt.QtPropertyUser;
import io.qt.QtPropertyWriter;
import io.qt.QtReferenceType;
import io.qt.QtShortEnumerator;
import io.qt.QtSignalEmitterInterface;
import io.qt.QtUninvokable;
import io.qt.QtUnlistedEnum;
import io.qt.core.QByteArray;
import io.qt.core.QMetaMethod;
import io.qt.core.QMetaObject;
import io.qt.core.QMetaType;
import io.qt.core.QObject;
import io.qt.core.Qt;
import io.qt.internal.QtJambiSignals.AbstractSignal;
import io.qt.internal.QtJambiSignals.SignalParameterType;


/**
 * Methods to help construct the fake meta object.
 */
public class MetaObjectTools {
    private static class Container {
        private enum AnnotationType {
            Reader,
            Writer,
            Resetter,
            Notify
        }

        private Member member;
        private String name = null;
        private boolean enabled;
        private AnnotationType type;


        private Container(String name, Member member, boolean enabled, AnnotationType type) {
            this.name = name;
            this.member = member;
            this.enabled = enabled;
            this.type = type;
        }

        private Container(QtPropertyReader reader, Method method) {
            this(reader.name(), method, reader.enabled(), AnnotationType.Reader);
        }

        private Container(QtPropertyWriter writer, Method method) {
            this(writer.name(), method, writer.enabled(), AnnotationType.Writer);
        }

        private Container(QtPropertyResetter resetter, Method method) {
            this(resetter.name(), method, resetter.enabled(), AnnotationType.Resetter);
        }
        
        private Container(QtPropertyNotify notify, Field method) {
            this(notify.name(), method, notify.enabled(), AnnotationType.Notify);
        }

        private static String removeAndLowercaseFirst(String name, int count) {
            return Character.toLowerCase(name.charAt(count)) + name.substring(count + 1);
        }

        private String getNameFromMethod(Member method) {
            if (type == AnnotationType.Resetter) {
                return "";
            } else if (type == AnnotationType.Notify) {
                return "";
            } else if (type == AnnotationType.Reader) {
                String name = method.getName();
                int len = name.length();
                if (name.startsWith("get") && len > 3)
                    name = removeAndLowercaseFirst(name, 3);
                else if (isBoolean(((Method)method).getReturnType()) && name.startsWith("is") && len > 2)
                    name = removeAndLowercaseFirst(name, 2);
                else if (isBoolean(((Method)method).getReturnType()) && name.startsWith("has") && len > 3)
                    name = removeAndLowercaseFirst(name, 3);

                return name;
            } else { // starts with "set"
                String name = method.getName();
                if (!name.startsWith("set") || name.length() <= 3) {
                    throw new IllegalArgumentException("The correct pattern for setter accessor names is setXxx where Xxx is the property name with upper case initial.");
                }

                name = removeAndLowercaseFirst(name, 3);
                return name;
            }
        }

        private String name() {
            if (name == null || name.length() == 0)
                name = getNameFromMethod(member);

            return name;
        }

        private boolean enabled() {
            return enabled;
        }

        private static Container readerAnnotation(Method method) {
            QtPropertyReader reader = method.getAnnotation(QtPropertyReader.class);
            return reader == null ? null : new Container(reader, method);
        }

        private static Container writerAnnotation(Method method) {
            QtPropertyWriter writer = method.getAnnotation(QtPropertyWriter.class);
            return writer == null ? null : new Container(writer, method);
        }

        private static Container resetterAnnotation(Method method) {
            QtPropertyResetter resetter = method.getAnnotation(QtPropertyResetter.class);
            return resetter == null ? null : new Container(resetter, method);
        }
        
        private static Container notifyAnnotation(Field field) {
            QtPropertyNotify notify = field.getAnnotation(QtPropertyNotify.class);
            return notify == null ? null : new Container(notify, field);
        }

    }

    private static class MetaData {
        public @NativeAccess int metaData[];
        public @NativeAccess byte stringData[][];

		public @NativeAccess Field signalsArray[];
        public @NativeAccess Method slotsArray[];
        public @NativeAccess Constructor<?> constructorsArray[];

        public @NativeAccess Method propertyReadersArray[];
        public @NativeAccess Method propertyWritersArray[];
        public @NativeAccess Method propertyResettersArray[];
        public @NativeAccess Field propertyNotifiesArray[];
        public @NativeAccess Method propertyDesignableResolverArray[];
        public @NativeAccess Method propertyScriptableResolverArray[];
        public @NativeAccess Method propertyEditableResolverArray[];
        public @NativeAccess Method propertyStoredResolverArray[];
        public @NativeAccess Method propertyUserResolverArray[];
        public @NativeAccess Class<?> extraDataArray[] = {};

        public @NativeAccess String originalSignatures[];
        
        public @NativeAccess boolean hasStaticMembers;
    }

    private static Class<?> qmlListPropertiesClass;
    private static boolean qmlListPropertiesClassResolved;
    
    private static boolean isQQmlListProperty(Class<? extends Object> cls) {
    	if(!qmlListPropertiesClassResolved) {
    		qmlListPropertiesClassResolved = true;
    		Class<?> _qmlListPropertiesClass = null;
        	try {
        		_qmlListPropertiesClass = Class.forName("io.qt.qml.QQmlListProperty");
        	} catch (Exception e) {
        	}
        	qmlListPropertiesClass = _qmlListPropertiesClass;
    	}
		return qmlListPropertiesClass!=null && qmlListPropertiesClass==cls;
	}

    private static Method notBogus(Method method, String propertyName, Class<?> paramType) {
        if (method == null)
            return null;

        Container reader = Container.readerAnnotation(method);
        if (reader != null
            && (!reader.name().equals(propertyName)
                || !reader.enabled()
                || !method.getReturnType().isAssignableFrom(paramType))) {
            return null;
        } else {
            return method;
        }
    }

    private static int queryEnums(Class<?> clazz, Hashtable<String, Class<?>> enums) {
        int enumConstantCount = 0;

        Class<?> declaredClasses[] = clazz.getDeclaredClasses();
        for (Class<?> declaredClass : declaredClasses)
            enumConstantCount += putEnumTypeInHash(declaredClass, enums);

        return enumConstantCount;
    }

    @NativeAccess
    private static Class<?> getEnumForQFlags(Class<?> flagsType) {
        Type t = flagsType.getGenericSuperclass();
        if (t instanceof ParameterizedType) {
            Type typeArguments[] = ((ParameterizedType)t).getActualTypeArguments();
            return ((Class<?>) typeArguments[0]);
        }

        return null;
    }

    private static int putEnumTypeInHash(Class<?> type, Hashtable<String, Class<?>> enums) {
        Class<?> flagsType = QFlags.class.isAssignableFrom(type) ? type : null;
        Class<?> enumType = type.isEnum() ? type : null;
        if (enumType == null && flagsType != null) {
            enumType = getEnumForQFlags(flagsType);
        }

        if (enumType == null)
            return 0;

        // Since Qt supports enums that are not part of the meta object
        // we need to check whether the enum can actually be used in
        // a property.
        Class<?> enclosingClass = enumType.getEnclosingClass();
        if (enclosingClass == null){
        	return -1;
        }
        if (enumType.isAnnotationPresent(QtUnlistedEnum.class)) {
            return -1;
        }

        int enumConstantCount = 0;
        if (!enums.contains(enumType.getName())) {
            enums.put(enumType.getName(), enumType);

            enumConstantCount = enumType.getEnumConstants().length;
        }

        if (flagsType != null && !enums.contains(flagsType.getName()))
            enums.put(flagsType.getName(), flagsType);

        return enumConstantCount;
    }
    
    private static boolean isEnumAllowedForProperty(Class<?> type) {
        Class<?> flagsType = QFlags.class.isAssignableFrom(type) ? type : null;
        Class<?> enumType = type.isEnum() ? type : null;
        if (enumType == null && flagsType != null) {
            enumType = getEnumForQFlags(flagsType);
        }

        if (enumType == null)
            return false;

        // Since Qt supports enums that are not part of the meta object
        // we need to check whether the enum can actually be used in
        // a property.
        Class<?> enclosingClass = enumType.getEnclosingClass();
        if (enclosingClass == null){
        	return false;
        }
        if(enumType.isAnnotationPresent(QtUnlistedEnum.class))
        	return false;
        return true;
    }

    private static Object isDesignable(Method declaredMethod, Class<?> clazz) {
        QtPropertyDesignable designable = declaredMethod.getAnnotation(QtPropertyDesignable.class);

        if (designable != null) {
            String value = designable.value();

            if (value.equals("true")) {
                return Boolean.TRUE;
            } else if (value.equals("false")) {
                return Boolean.FALSE;
            } else try {
                Method m = clazz.getMethod(value);
                if (isBoolean(m.getReturnType()))
                    return m;
                else
                    throw new RuntimeException("Wrong return type of designable method '" + m.getName() + "'");
            } catch (Throwable t) {
                java.util.logging.Logger.getLogger("io.qt.internal").log(java.util.logging.Level.SEVERE, "", t);
            }
        }

        return Boolean.TRUE;
    }
    
    private static Object isScriptable(Method declaredMethod, Class<?> clazz) {
        QtPropertyScriptable scriptable = declaredMethod.getAnnotation(QtPropertyScriptable.class);

        if (scriptable != null) {
            String value = scriptable.value();

            if (value.equals("true")) {
                return Boolean.TRUE;
            } else if (value.equals("false")) {
                return Boolean.FALSE;
            } else try {
                Method m = clazz.getMethod(value);
                if (isBoolean(m.getReturnType()))
                    return m;
                else
                    throw new RuntimeException("Wrong return type of scriptable method '" + m.getName() + "'");
            } catch (Throwable t) {
                java.util.logging.Logger.getLogger("io.qt.internal").log(java.util.logging.Level.SEVERE, "", t);
            }
        }
        return Boolean.TRUE;
    }
    
    private static Object isStored(Method declaredMethod, Class<?> clazz) {
    	QtPropertyStored stored = declaredMethod.getAnnotation(QtPropertyStored.class);

        if (stored != null) {
            String value = stored.value();

            if (value.equals("true")) {
                return Boolean.TRUE;
            } else if (value.equals("false")) {
                return Boolean.FALSE;
            } else try {
                Method m = clazz.getMethod(value);
                if (isBoolean(m.getReturnType()))
                    return m;
                else
                    throw new RuntimeException("Wrong return type of scriptable method '" + m.getName() + "'");
            } catch (Throwable t) {
                java.util.logging.Logger.getLogger("io.qt.internal").log(java.util.logging.Level.SEVERE, "", t);
            }
        }
        return Boolean.TRUE;
    }
    
    private static Object isEditable(Method declaredMethod, Class<?> clazz) {
    	/*
    	QtPropertyEditable editable = declaredMethod.getAnnotation(QtPropertyEditable.class);

        if (editable != null) {
            String value = editable.value();

            if (value.equals("true")) {
                return Boolean.TRUE;
            } else if (value.equals("false")) {
                return Boolean.FALSE;
            } else try {
                Method m = clazz.getMethod(value);
                if (isBoolean(m.getReturnType()))
                    return m;
                else
                    throw new RuntimeException("Wrong return type of scriptable method '" + m.getName() + "'");
            } catch (Throwable t) {
                java.util.logging.Logger.getLogger("io.qt.internal").log(java.util.logging.Level.SEVERE, "", t);
            }
        }
		*/
        return Boolean.TRUE;
    }

    private static boolean isValidSetter(Method declaredMethod) {
        return (declaredMethod.getParameterCount() == 1
                && declaredMethod.getReturnType() == Void.TYPE
                && !internalTypeNameOfParameters(declaredMethod).equals(""));
    }

    private static Method getDeclaredMethod(Class<?> clazz, String name, Class<?> args[]) {
        try {
            return clazz.getDeclaredMethod(name, args);
        } catch (NoSuchMethodException e) {
            return null;
        }
    }

    private static String capitalizeFirst(String str) {
        return Character.toUpperCase(str.charAt(0)) + str.substring(1);
    }

    private static boolean isBoolean(Class<?> type) {
        return (type == Boolean.class || type == Boolean.TYPE);
    }

    private static Object isUser(Method declaredMethod, Class<?> clazz) {
    	QtPropertyUser user = declaredMethod.getAnnotation(QtPropertyUser.class);

        if (user != null) {
            String value = user.value();

            if (value.equals("true")) {
                return Boolean.TRUE;
            } else if (value.equals("false")) {
                return Boolean.FALSE;
            } else try {
                Method m = clazz.getMethod(value);
                if (isBoolean(m.getReturnType()))
                    return m;
                else
                    throw new RuntimeException("Wrong return type of scriptable method '" + m.getName() + "'");
            } catch (Throwable t) {
                java.util.logging.Logger.getLogger("io.qt.internal").log(java.util.logging.Level.SEVERE, "", t);
            }
        }

        return Boolean.FALSE;
    }

    private static Boolean isRequired(Method declaredMethod, Class<?> clazz) {
    	QtPropertyRequired required = declaredMethod.getAnnotation(QtPropertyRequired.class);
        if (required != null) {
            return required.value();
        }
        return Boolean.FALSE;
    }

    private static Boolean isFinal(Method declaredMethod) {
    	return Modifier.isFinal(declaredMethod.getModifiers());
    }

    private static Boolean isConstant(Method declaredMethod) {
    	QtPropertyConstant isConstant = declaredMethod.getAnnotation(QtPropertyConstant.class);
        if (isConstant != null) {
            return isConstant.value();
        }
        return Boolean.FALSE;
    }

    private static boolean isValidGetter(Method method) {
        return (method.getParameterCount() == 0
                && method.getReturnType() != Void.TYPE);
    }


    private static String bunchOfClassNamesInARow(Class<?> classes[], Type[] genericTypes, boolean cppName) {
    	List<SignalParameterType> typeList = Collections.emptyList();
    	for (int i = 0; i < classes.length && i < genericTypes.length; i++) {
    		int arrayDimension = 0;
    		Class<?> type = classes[i];
    		Type genericType = genericTypes[i];
    		while (type.isArray()) {
    			arrayDimension++;
    			type = type.getComponentType();
    			if(genericType instanceof GenericArrayType) {
    				genericType = ((GenericArrayType) genericType).getGenericComponentType();
    			}else {
    				genericType = type;
    			}
            }
    		SignalParameterType signalType = new SignalParameterType(classes[i], type, genericType, arrayDimension);
    		if(i==0) {
    			if(classes.length>1) {
    				typeList = new ArrayList<>();
    				typeList.add(signalType);
    			}else {
    				typeList = Collections.singletonList(signalType);
    			}
    		}else {
    			typeList.add(signalType);
    		}
		}
        return bunchOfClassNamesInARow(typeList, cppName);
    }
    
    static String bunchOfClassNamesInARow(QtJambiSignals.SignalParameterType type, boolean cppName) {
    	String className = type.type.getName();
		String arrays = "";
        for (int j=0; j<type.arrayDimension; ++j)
            arrays += "[";
		if(!arrays.isEmpty()){
			if(type.type==int.class){
				className = arrays+"I";
			}else if(type.type==short.class){
				className = arrays+"S";
			}else if(type.type==long.class){
				className = arrays+"J";
			}else if(type.type==byte.class){
				className = arrays+"B";
			}else if(type.type==double.class){
				className = arrays+"D";
			}else if(type.type==float.class){
				className = arrays+"F";
			}else if(type.type==boolean.class){
				className = arrays+"Z";
			}else{
				className = arrays+"L"+className+";";
			}
		}
        if(!type.type.isPrimitive()){
        	registerClass(className, type.originalType);
        }
        if(cppName) {
        	String _className = internalTypeName(className, type.originalType.getClassLoader());
        	if(!className.isEmpty() && _className.isEmpty()) {
        		_className = "JObjectWrapper";
        	}
        	className = _className;
        }
        return className;
    }

    private static native void registerClass(String className, Class<?> originalType);

	static String bunchOfClassNamesInARow(List<QtJambiSignals.SignalParameterType> types, boolean cppName) {
        String classNames = "";

        for (int i=0; i<types.size(); i++) {
			classNames += bunchOfClassNamesInARow(types.get(i), cppName);
            if (i<types.size()-1)
                classNames += ",";
        }

        return classNames;
    }

    private static String methodParameters(Method m) {
        return bunchOfClassNamesInARow(m.getParameterTypes(), m.getGenericParameterTypes(), false);
    }
    
    private static String cppMethodParameters(Method m) {
        return bunchOfClassNamesInARow(m.getParameterTypes(), m.getGenericParameterTypes(), true);
    }
    
    @SuppressWarnings("rawtypes")
	@NativeAccess
    private static Class[] signalTypes(Field field) {
    	QtJambiInternal.ResolvedSignal resolvedSignal = QtJambiInternal.resolveSignal(field, field.getDeclaringClass());
		Class[] result = new Class[resolvedSignal.signalTypes.size()];
		for (int i = 0; i < resolvedSignal.signalTypes.size(); i++) {
    		result[i] = resolvedSignal.signalTypes.get(i).originalType;
		}
		return result;
    }
    
    @NativeAccess
    private static Object[][] methodTypes(AccessibleObject accessible) {
    	Class<?> returnType = null;
		Type genericReturnType = null;
		Class<?>[] parameterTypes = null;
    	Type[] genericParameterTypes = null;
    	AnnotatedType[] annotatedParameterTypes = null;
    	if(accessible instanceof Method) {
    		Method m = (Method)accessible;
    		returnType = m.getReturnType();
    		genericReturnType = m.getGenericReturnType();
    		parameterTypes = m.getParameterTypes();
	    	genericParameterTypes = m.getGenericParameterTypes();
	    	annotatedParameterTypes = m.getAnnotatedParameterTypes();
    	}else if(accessible instanceof Constructor<?>) {
    		Constructor<?> c = (Constructor<?>)accessible;
    		returnType = void.class;
    		genericReturnType = void.class;
    		parameterTypes = c.getParameterTypes();
	    	genericParameterTypes = c.getGenericParameterTypes();
	    	annotatedParameterTypes = c.getAnnotatedParameterTypes();
    	}else if(accessible instanceof Field) {
    		Field f = (Field)accessible;
    		QtJambiInternal.ResolvedSignal resolvedSignal = QtJambiInternal.resolveSignal(f, f.getDeclaringClass());
    		Class<?>[] cresult = new Class[1+resolvedSignal.signalTypes.size()];
    		String[] sresult = new String[1+resolvedSignal.signalTypes.size()];
    		String[] qresult = new String[1+resolvedSignal.signalTypes.size()];
    		sresult[0] = "void";
    		qresult[0] = "void";
    		cresult[0] = void.class;
    		for (int i = 0; i < resolvedSignal.signalTypes.size(); i++) {
	    		sresult[i+1] = bunchOfClassNamesInARow(resolvedSignal.signalTypes.get(i), false);
	    		cresult[i+1] = resolvedSignal.signalTypes.get(i).originalType;
	    		qresult[i+1] = internalTypeNameOfClass(resolvedSignal.signalTypes.get(i).originalType, resolvedSignal.signalTypes.get(i).genericType);
			}
    		return new Object[][]{cresult, sresult, qresult};
    	}
    	if(returnType!=null) {
    		Class<?>[] cresult = new Class[1+parameterTypes.length];
    		String[] sresult = new String[1+parameterTypes.length];
    		String[] qresult = new String[1+parameterTypes.length];
	    	{
	    		Class<?> original = returnType;
	    		Type originalGenericReturnType = genericReturnType;
	    		int arrayDimension = 0;
	    		while (returnType.isArray()) {
	    			arrayDimension++;
	    			returnType = returnType.getComponentType();
	    			if(genericReturnType instanceof GenericArrayType) {
	    				genericReturnType = ((GenericArrayType) genericReturnType).getGenericComponentType();
	    			}else {
	    				genericReturnType = returnType;
	    			}
	            }
	    		SignalParameterType signalType = new SignalParameterType(original, returnType, genericReturnType, arrayDimension);
	    		sresult[0] = bunchOfClassNamesInARow(Collections.singletonList(signalType), false);
	    		cresult[0] = original;
	    		qresult[0] = internalTypeNameOfClass(original, originalGenericReturnType);
	    	}
	    	for (int i = 0; i < genericParameterTypes.length; i++) {
	    		int arrayDimension = 0;
	    		Class<?> type = parameterTypes[i];
	    		Type genericType = genericParameterTypes[i];
	    		AnnotatedType annotatedType = annotatedParameterTypes[i];
	    		while (type.isArray()) {
	    			arrayDimension++;
	    			type = type.getComponentType();
	    			if(genericType instanceof GenericArrayType) {
	    				genericType = ((GenericArrayType) genericType).getGenericComponentType();
	    			}else {
	    				genericType = type;
	    			}
	            }
	    		SignalParameterType signalType = new SignalParameterType(parameterTypes[i], type, genericType, arrayDimension);
	    		sresult[i+1] = bunchOfClassNamesInARow(Collections.singletonList(signalType), false);
	    		cresult[i+1] = parameterTypes[i];
	    		String typeName = internalTypeNameOfClass(parameterTypes[i], genericParameterTypes[i]);
				if(annotatedType.isAnnotationPresent(QtPointerType.class)) {
            		if(!typeName.endsWith("*")) {
            			typeName += "*";
            		}
            	}else if(annotatedType.isAnnotationPresent(QtReferenceType.class)) {
            		if(!typeName.endsWith("&")) {
            			typeName += "&";
            		}
            	}
				qresult[i+1] = typeName;
			}
	    	return new Object[][]{cresult, sresult, qresult};
    	}else {
    		return null;
    	}
    }
    
    /**
     * Returns the signature of the method m excluding the modifiers and the
     * return type.
     */
    private static String methodSignature(Method m, boolean includeReturnType) {
        return (includeReturnType ? m.getReturnType().getName() + " " : "")
               + m.getName() + "(" + methodParameters(m) + ")";
    }
    
    private static String constructorSignature(Constructor<?> m) {
        return m.getName() + "(" + constructorParameters(m) + ")";
    }
    
    private static String methodSignature(QtJambiInternal.ResolvedSignal r) {
        return r.name + "(" + bunchOfClassNamesInARow(r.signalTypes, false) + ")";
    }

    static String methodSignature(Method m) {
        return methodSignature(m, false);
    }
    
    private static String shortConstructorSignature(Constructor<?> c) {
    	String name = c.getName();
    	int idx = name.lastIndexOf('.');
    	if(idx>0){
    		name = name.substring(idx+1, name.length());
    	}
        return name + "(" + constructorParameters(c) + ")";
    }
    
    private static String constructorParameters(Constructor<?> c) {
        return bunchOfClassNamesInARow(c.getParameterTypes(), c.getGenericParameterTypes(), false);
    }
    
    private static String cppConstructorParameters(Constructor<?> c) {
        return bunchOfClassNamesInARow(c.getParameterTypes(), c.getGenericParameterTypes(), true);
    }


    private static int addString(int metaData[],
                                 Hashtable<String, Integer> strings,
                                 List<String> stringsInOrder,
                                 String string, int offset, int metaDataOffset) {

                if (strings.containsKey(string)) {
                    metaData[metaDataOffset] = strings.get(string);
                    return 0;
                }

                metaData[metaDataOffset] = offset;
                strings.put(string, offset);
                stringsInOrder.add(string);
                return string.length() + 1;
    }

    public static String cppSignalSignature(QtSignalEmitterInterface signalEmitter, String signalName) {
    	QtJambiSignals.AbstractSignal signal = QtJambiInternal.lookupSignal(signalEmitter, signalName, null);
        if (signal != null) {
        	return cppSignalSignature(signal);
        }else {
            return "";
        }
    }
    
    public static String cppNormalizedSignature(String signalName) {
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
					name += "(";
					for (int i = 0; i < argumentTypes.length; ++i) {
						if(i>0) {
							name += ",";
						}
						name += internalTypeName(argumentTypes[i].replace(" ", ""), MetaObjectTools.class.getClassLoader());
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
    
    public static String cppSlotSignature(Object receiver, String slot) {
    	if(receiver instanceof QtSignalEmitterInterface) {
    		AbstractSignal signal = io.qt.internal.QtJambiInternal.lookupSignal((QtSignalEmitterInterface)receiver, slot, null);
    		if(signal!=null) {
    			return cppSignalSignature(signal);
    		}
    	}
    	Method slotMethod = io.qt.internal.QtJambiInternal.lookupSlot(receiver, slot);
        if (slotMethod != null) {
        	if(slotMethod.isAnnotationPresent(QtUninvokable.class)) {
        		throw new QBlockedSlotException(slot);
        	}
        	QMetaMethod method = QMetaMethod.fromReflectedMethod(slotMethod);
        	char prefix = io.qt.internal.QtJambiInternal.SlotPrefix;
        	if(method!=null) {
        		if(method.methodType()==QMetaMethod.MethodType.Signal) {
        			prefix = io.qt.internal.QtJambiInternal.SignalPrefix;
        		}
        		return prefix + cppMethodSignature(method);
        	}
            return prefix + slotMethod.getName() + "(" + cppMethodParameters(slotMethod) + ")";
        }
   		throw new QNoSuchSlotException(receiver, slot);
    }
    
	public static String cppSignalSignature(String signal) {
		if(signal.length()>0) {
			if(signal.charAt(0)!=io.qt.internal.QtJambiInternal.SignalPrefix
					&& signal.charAt(0)!=io.qt.internal.QtJambiInternal.SlotPrefix) {
				signal = io.qt.internal.QtJambiInternal.SignalPrefix + signal;
			}
		}
    	return signal;
    }
	
	public static String cppSlotSignature(String signal) {
		if(signal.length()>0) {
			if(signal.charAt(0)!=io.qt.internal.QtJambiInternal.SignalPrefix
					&& signal.charAt(0)!=io.qt.internal.QtJambiInternal.SlotPrefix) {
				signal = io.qt.internal.QtJambiInternal.SlotPrefix + signal;
			}
		}
    	return signal;
    }
    
	public static QByteArray cppSignalSignature(QByteArray signal) {
		if(signal.length()>0) {
			if(!signal.startsWith((byte)io.qt.internal.QtJambiInternal.SignalPrefix)
					&& !signal.startsWith((byte)io.qt.internal.QtJambiInternal.SlotPrefix)) {
				signal.clone().prepend(""+io.qt.internal.QtJambiInternal.SignalPrefix);
			}
		}
    	return signal;
    }
	
	public static QByteArray cppSlotSignature(QByteArray signal) {
		if(signal.length()>0) {
			if(!signal.startsWith((byte)io.qt.internal.QtJambiInternal.SignalPrefix)
					&& !signal.startsWith((byte)io.qt.internal.QtJambiInternal.SlotPrefix)) {
				signal.clone().prepend(""+io.qt.internal.QtJambiInternal.SlotPrefix);
			}
		}
    	return signal;
    }
	
	public static String cppSignalSignature(QMetaObject.AbstractSignal signal) {
    	return cppSignalSignature((QtJambiSignals.AbstractSignal)signal);
    }

    static String cppSignalSignature(QtJambiSignals.AbstractSignal signal) {
        QMetaMethod method = QMetaMethod.fromSignal((QMetaObject.AbstractSignal)signal);
        if(method!=null && method.methodType()==QMetaMethod.MethodType.Signal) {
        	return io.qt.internal.QtJambiInternal.SignalPrefix + cppMethodSignature(method);
        }
        String signalParameters = bunchOfClassNamesInARow(signal.signalTypes(), true);
        return io.qt.internal.QtJambiInternal.SignalPrefix + signal.name() + "(" + signalParameters + ")";
    }

    private static String signalParameters(QtJambiInternal.ResolvedSignal resolvedSignal) {
        return bunchOfClassNamesInARow(resolvedSignal.signalTypes, false);
    }

    private static String signalParameters(Field field, Class<?> declaringClass) {
        QtJambiInternal.ResolvedSignal resolvedSignal = QtJambiInternal.resolveSignal(field, declaringClass);
        return signalParameters(resolvedSignal);
    }


    private native static String internalTypeName(String s, ClassLoader classLoader);
    
    private native static String internalMethodName(QMetaMethod method);
    
    private native static String cppMethodSignature(QMetaMethod method);
    
    /**
     * This method is used from inside the native qtjambi method
     * <tt>QtJambiTypeManager::resolveQFlags(const QString &amp;, const QString &amp;) const</tt>
     * @param cls a QFlags class
     * @return the type argument
     */
    private static Class<?> resolveQFlagsActualTypeArgument(Class<? extends QFlags<?>> cls){
		Type type = cls;
		while(true){
    		if(type  instanceof ParameterizedType){
    			ParameterizedType ptype = (ParameterizedType)type;
    			if(ptype.getRawType()==QFlags.class){
    				Type[] actualTypeArguments = ptype.getActualTypeArguments();
    				if(actualTypeArguments.length==1 && actualTypeArguments[0] instanceof Class<?>){
    					return (Class<?>)actualTypeArguments[0];
    				}else{
    					return QtEnumerator.class;
    				}
    			}else{
    				if(ptype.getRawType() instanceof Class<?>){
    					type = ((Class<?>)ptype.getRawType()).getGenericSuperclass();
    				}else{
    					return QtEnumerator.class; 
    				}
    			}
    		}else if(type instanceof Class){
    			if(type==QFlags.class){
    	    		return QtEnumerator.class;
    			}else{
    				type = ((Class<?>)type).getGenericSuperclass();
    			}
    		}
		}
//    	return null;
    }

    private static String internalTypeNameOfSignal(String s, ClassLoader classCoader) {
        try {
            return internalTypeName(s, classCoader);
        } catch (Throwable t) {
            return "";
        }
    }
    
    private static String internalTypeNameOfParameters(Method declaredMethod) {
        try {
        	String s = methodParameters(declaredMethod);
            return internalTypeName(s, declaredMethod.getDeclaringClass().getClassLoader());
        } catch (Throwable t) {
            return "";
        }
    }
    
    public static String internalTypeNameOfParameters(Constructor<?> declaredConstructor) {
        try {
        	String s = constructorParameters(declaredConstructor);
            return internalTypeName(s, declaredConstructor.getDeclaringClass().getClassLoader());
        } catch (Throwable t) {
            return "";
        }
    }
    
    public static String internalTypeNameOfMethodSignature(Constructor<?> constructor) {
        try {
        	String javaMethodSignature = shortConstructorSignature(constructor);
            return internalTypeName(javaMethodSignature, constructor.getDeclaringClass().getClassLoader());
        } catch (Throwable t) {
            return "";
        }
    }
    
    public static String internalNameOfArgumentType(Class<? extends Object> cls) {
    	return internalTypeNameOfClass(cls, cls);
    }
    
    private static String internalTypeNameOfClass(Class<? extends Object> cls, Type genericType) {
        try {
        	if(isQQmlListProperty(cls) && genericType instanceof ParameterizedType) {
            	ParameterizedType ptype = (ParameterizedType)genericType;
            	Type actualTypes[] = ptype.getActualTypeArguments();
            	if(actualTypes.length==1 && actualTypes[0] instanceof Class<?>) {
            		String argumentName = internalTypeNameOfClass((Class<?>)actualTypes[0], actualTypes[0]);
        			if(argumentName.endsWith("*")) {
        				argumentName = argumentName.substring(0, argumentName.length()-1);
        			}
            		return "QQmlListProperty<"+argumentName+">";
            	}
            }
            String returnType = cls.getName();
            if(!cls.isPrimitive()) {
                if(cls.isArray()){
//                    returnType = Object.class.getName();
                }else{
                    try {
                        MetaObjectTools.class.getClassLoader().loadClass(returnType);
                    } catch(Exception e) {
                        if(QObject.class.isAssignableFrom(cls)) {
                            returnType = QObject.class.getName();
                        } else {
                            returnType = Object.class.getName();
                        }
                    }
                }
            }
            String result = internalTypeName(returnType, cls.getClassLoader());
            boolean isEnumOrFlags = Enum.class.isAssignableFrom(cls) || QFlags.class.isAssignableFrom(cls);
            if (isEnumOrFlags && "JObjectWrapper".equals(result) && cls.getDeclaringClass() != null && (QObject.class.isAssignableFrom(cls.getDeclaringClass()) || Qt.class.isAssignableFrom(cls.getDeclaringClass()))) {
            	if(QtJambiInternal.isGeneratedClass(cls.getDeclaringClass())){
//            	if(QFlags.class.isAssignableFrom(cls)){
//            		return "QFlags<"+internalTypeNameOfClass(getEnumForQFlags(cls)) +">";
//            	}else{
            		result = internalTypeNameOfClass(cls.getDeclaringClass(), null).replace("*", "") + "::" + cls.getSimpleName();
//	            	}else{
		                // To avoid using JObjectWrapper for enums and flags (which is required in certain cases.)
//		                return cls.getDeclaringClass().getName().replace(".", "::") + "::" + cls.getSimpleName();
	            	}
//            	}
            }
            return result;
        } catch (Throwable t) {
        	java.util.logging.Logger.getLogger("io.qt.internal").log(java.util.logging.Level.SEVERE, "", t);
            return "";
        }
    }

	private static class StringList extends ArrayList<String>{
		private static final long serialVersionUID = -7793211808465428478L;

		@Override
		public boolean add(String e) {
			if (!contains(e))
				return super.add(e);
			return true;
		}
    }

    /**
     * this method creates Qt5 meta object data from the submitted class.
     * It is based upon code of the moc tool.
     */
    @NativeAccess
    private static MetaData buildMetaData(Class<?> clazz) {
    	try {
			if(clazz.isPrimitive() || clazz.isArray() || clazz.isSynthetic() || clazz.getName().contains("$Lamda$")) {
				return null;
			}
			List<String> strings = new StringList();
			strings.add("Reserving the first string for QDynamicMetaObject identification.");
			List<String> intdataComments = /*new ArrayList<>();*/new AbstractList<String>() {
				@Override
				public boolean add(String e) { return false; }
				@Override
				public String get(int index) {return null;}
				@Override
				public int size() {return 0; }
			};
			List<Integer> intdata = new ArrayList<Integer>();
			final String classname = clazz.getName().replace(".", "::");
			strings.add(classname);
			MetaData metaData = new MetaData();
			
			Hashtable<String,String> classInfos = new Hashtable<String, String>();
			
			for(QtClassInfo info : clazz.getAnnotationsByType(QtClassInfo.class)) {
				classInfos.put(info.key(), info.value());
			}
//        classInfos.put("__qt__binding_shell_language", "Qt Jambi");
			
			for(Map.Entry<String,String> entry : classInfos.entrySet()){
				strings.add(entry.getKey());
				strings.add(entry.getValue());
			}
			
			List<Constructor<?>> constructors = new ArrayList<Constructor<?>>();
			List<Method> slots = new ArrayList<Method>();
			List<Method> methods = new ArrayList<Method>();
			
			List<String> originalSignatures = new ArrayList<String>();

			Hashtable<String, Method> propertyReaders = new Hashtable<String, Method>();
			Hashtable<String, Method> propertyWriters = new Hashtable<String, Method>();
			Hashtable<String, Object> propertyDesignableResolvers = new Hashtable<String, Object>();
			Hashtable<String, Object> propertyScriptableResolvers = new Hashtable<String, Object>();
			Hashtable<String, Object> propertyEditableResolvers = new Hashtable<String, Object>();
			Hashtable<String, Object> propertyStoredResolvers = new Hashtable<String, Object>();
			Hashtable<String, Object> propertyUserResolvers = new Hashtable<String, Object>();
			Hashtable<String, Boolean> propertyRequiredResolvers = new Hashtable<String, Boolean>();
			Hashtable<String, Boolean> propertyConstantResolvers = new Hashtable<String, Boolean>();
			Hashtable<String, Boolean> propertyFinalResolvers = new Hashtable<String, Boolean>();
			Hashtable<String, Method> propertyResetters = new Hashtable<String, Method>();
			Hashtable<String, Field> propertyNotifies = new Hashtable<String, Field>();

			// First we get all enums actually declared in the class
			Hashtable<String, Class<?>> enums = new Hashtable<String, Class<?>>();
			queryEnums(clazz, enums);

			Field declaredFields[] = clazz.getDeclaredFields();
			List<Field> signalFields = new ArrayList<Field>();
			List<QtJambiInternal.ResolvedSignal> resolvedSignals = new ArrayList<QtJambiInternal.ResolvedSignal>();
			for (Field declaredField : declaredFields) {
			    if (!Modifier.isStatic(declaredField.getModifiers()) && QtJambiInternal.isSignalType(declaredField.getType())) {
			        // If we can't convert all the types we don't list the signal
			        QtJambiInternal.ResolvedSignal resolvedSignal = QtJambiInternal.resolveSignal(declaredField, declaredField.getDeclaringClass());
			        String signalParameters = signalParameters(resolvedSignal);
			        if (signalParameters.length() == 0 || internalTypeNameOfSignal(signalParameters, clazz.getClassLoader()).length() != 0) {
			            signalFields.add(declaredField);
			            resolvedSignals.add(resolvedSignal);
			            originalSignatures.add(methodSignature(resolvedSignal));
			            
			            strings.add(resolvedSignal.name);
			            for (int i = 1; i <= resolvedSignal.signalTypes.size(); i++) {
			            	QtJambiSignals.SignalParameterType signalType = resolvedSignal.signalTypes.get(i-1);
			            	String typeName;
			            	if(signalType.arrayDimension==0){
			            		typeName = internalTypeNameOfClass(signalType.type, signalType.genericType);
			            	}else{
			            		typeName = internalTypeNameOfClass(Object.class, Object.class);
			            	}
			            	QMetaType.Type type = metaType(typeName);
			            	if(type==QMetaType.Type.UnknownType || type==QMetaType.Type.User){
			            		strings.add(typeName);
			            	}
			            	strings.add("arg__"+i);
						}
			            strings.add(""); // for the method tag
			        }

			        // Rules for resetters:
			        // 1. Zero or one argument
			        {
			            Container notify = Container.notifyAnnotation(declaredField);

			            if (notify != null
			                && resolvedSignal.signalTypes.size() <= 1) {
			                propertyNotifies.put(notify.name(), declaredField);
			            }
			        }
			    }
			}
			
//			if(QObject.class.isAssignableFrom(clazz)) 
			{
			    cloop: for(Constructor<?> constructor : clazz.getDeclaredConstructors()){
			    	if(!constructor.isSynthetic() && constructor.isAnnotationPresent(QtInvokable.class)) {
			        	Class<?>[] parameterTypes = constructor.getParameterTypes();
			        	for (Class<?> parameterType : parameterTypes) {
							if(parameterType==QtJambiInternal.privateConstructorClass()) {
								continue cloop;
							}
						}
			        	constructors.add(constructor);
			        	metaData.hasStaticMembers = true;
			        	originalSignatures.add(constructorSignature(constructor));
			        	Parameter[] parameters = constructor.getParameters();
			        	Type[] genericParameterTypes = constructor.getGenericParameterTypes();
			        	for (int i = 0; i < parameterTypes.length; i++) {
			            	String typeName = internalTypeNameOfClass(parameterTypes[i], genericParameterTypes[i]);
			            	QMetaType.Type type = metaType(typeName);
			            	if(type==QMetaType.Type.UnknownType || type==QMetaType.Type.User){
			            		strings.add(typeName);
			            	}
			            	if(parameters[i].isNamePresent()) {
			            		strings.add(parameters[i].getName());
			            	}else {
			            		strings.add("arg__"+(i+1));
			            	}
						}
			        	strings.add("");
			    	}
			    }
			}
			
			for (Method declaredMethod : clazz.getDeclaredMethods()) {
				if(declaredMethod.isSynthetic() 
						|| declaredMethod.isBridge()) {
					continue;
				}
			    if (
			    		(
			    				(
			    						QObject.class.isAssignableFrom(clazz)
			    						&& !declaredMethod.isAnnotationPresent(QtUninvokable.class) 
			    						&& !Modifier.isStatic(declaredMethod.getModifiers())
			    				) || (
			            				declaredMethod.isAnnotationPresent(QtInvokable.class) 
			            				&& declaredMethod.getAnnotation(QtInvokable.class).value()
			    				)
						)
			    		&& !overridesGeneratedSlot(declaredMethod, clazz)
					) {

			        // If we can't convert the type, we don't list it
			        String methodParameters = methodParameters(declaredMethod);
			        String returnType = declaredMethod.getReturnType().getName();
			        String returnTypeName = internalTypeNameOfClass(declaredMethod.getReturnType(), declaredMethod.getGenericReturnType());
			        if(returnTypeName.startsWith("QQmlListProperty<") && QMetaType.type(returnTypeName)==QMetaType.Type.UnknownType.value()) {
			        	registerQmlListProperty(returnTypeName);
			        }
			        if(declaredMethod.getAnnotatedReturnType().isAnnotationPresent(QtPointerType.class)) {
			    		if(!returnTypeName.endsWith("*")) {
			    			returnTypeName += "*";
			    		}
			    	}else if(declaredMethod.getAnnotatedReturnType().isAnnotationPresent(QtReferenceType.class)) {
			    		if(!returnTypeName.endsWith("&")) {
			    			returnTypeName += "&";
			    		}
			    	}
			        if ((methodParameters.equals("") || !internalTypeNameOfParameters(declaredMethod).equals(""))
			            &&(returnType.equals("") || returnType.equals("void") || !returnTypeName.equals(""))) {
			        	String methodSignature = methodSignature(declaredMethod);
			        	if(!originalSignatures.contains(methodSignature)) {
			                if(Modifier.isStatic(declaredMethod.getModifiers())){
			                	methods.add(declaredMethod);
	                    		metaData.hasStaticMembers = true;
			                }else{
			                	if(!QObject.class.isAssignableFrom(clazz)) {
			                		// we need to make sure that static_metacall is set
			                		metaData.hasStaticMembers = true;
			                	}
			                	slots.add(declaredMethod);
			                }
			                originalSignatures.add(methodSignature);
			                strings.add(declaredMethod.getName());
			                strings.add(returnTypeName);
			                Parameter[] parameters = declaredMethod.getParameters();
				        	Class<?>[] parameterTypes = declaredMethod.getParameterTypes();
				        	Type[] genericParameterTypes = declaredMethod.getGenericParameterTypes();
				        	AnnotatedType[] annotatedParameterTypes = declaredMethod.getAnnotatedParameterTypes();
			                for (int i = 1; i <= parameterTypes.length; i++) {
			                	String typeName = internalTypeNameOfClass(parameterTypes[i-1], genericParameterTypes[i-1]);
			                	if(annotatedParameterTypes[i-1].isAnnotationPresent(QtPointerType.class)) {
			                		if(!typeName.endsWith("*")) {
			                			typeName += "*";
			                		}
			                	}else if(annotatedParameterTypes[i-1].isAnnotationPresent(QtReferenceType.class)) {
			                		if(!typeName.endsWith("&")) {
			                			typeName += "&";
			                		}
			                	}
			                	QMetaType.Type type = metaType(typeName);
			                	if(type==QMetaType.Type.UnknownType || type==QMetaType.Type.User){
			                		strings.add(typeName);
			                	}
			                	if(parameters[i-1].isNamePresent()) {
			                		strings.add(parameters[i-1].getName());
			                	}else {
			                		strings.add("arg__"+i);
			                	}
							}
			                strings.add(""); // for the method tag
			        	}
			        }
			    }
			    
			    final String declaredMethodName = declaredMethod.getName();

			    // Rules for readers:
			    // 1. Zero arguments
			    // 2. Return something other than void
			    // 3. We can convert the type
			    Container reader = Container.readerAnnotation(declaredMethod);
			    {

			        if ( 
			        		(reader != null && reader.enabled())
			                && isValidGetter(declaredMethod)
			                && !internalTypeNameOfClass(declaredMethod.getReturnType(), declaredMethod.getGenericReturnType()).equals("")) {

			        	String name = reader.name();
			            // If the return type of the property reader is not registered, then
			            // we need to register the owner class in the meta object (in which case
			            // it has to be a QObject)
			            Class<?> returnType = declaredMethod.getReturnType();

			            if ( (QFlags.class.isAssignableFrom(returnType) || Enum.class.isAssignableFrom(returnType))
			            		&& !isEnumAllowedForProperty(returnType) ) {
			            	int type = QMetaType.registerMetaType(returnType);
			            	if(type==QMetaType.Type.UnknownType.value()) {
			                    System.err.println("Problem in property '" + name + "' in '" + clazz.getName()
			                                       + "' with return type '"+returnType.getName()
			                                       +"': Unable to register meta type.");
			                    continue;
			            	}
			            }

			            propertyReaders.put(name, declaredMethod);
			            propertyDesignableResolvers.put(name, isDesignable(declaredMethod, clazz));
			            propertyScriptableResolvers.put(name, isScriptable(declaredMethod, clazz));
			            propertyEditableResolvers.put(name, isEditable(declaredMethod, clazz));
			            propertyStoredResolvers.put(name, isStored(declaredMethod, clazz));
			            propertyUserResolvers.put(name, isUser(declaredMethod, clazz));
			            propertyRequiredResolvers.put(name, isRequired(declaredMethod, clazz));
			            propertyConstantResolvers.put(name, isConstant(declaredMethod));
			            propertyFinalResolvers.put(name, isFinal(declaredMethod));
			            strings.add(name);
			        }
			    }

			    // Rules for writers:
			    // 1. Takes exactly one argument
			    // 2. Return void
			    // 3. We can convert the type
			    Container writer = Container.writerAnnotation(declaredMethod);
			    {

			        if ( writer != null 
			        		&& writer.enabled()
			        		&& isValidSetter(declaredMethod)) {
			            propertyWriters.put(writer.name(), declaredMethod);
			        }
			    }
			    
			    // Rules for notifys:
			    // 1. No arguments
			    // 2. Return void
			    Container resetter = Container.resetterAnnotation(declaredMethod);
			    {

			        if (resetter != null
			            && declaredMethod.getParameterCount() == 0
			            && declaredMethod.getReturnType() == Void.TYPE) {
			            propertyResetters.put(resetter.name(), declaredMethod);
			        }
			    }

			    // Check naming convention by looking for setXxx patterns, but only if it hasn't already been
			    // annotated as a writer
			    if (QObject.class.isAssignableFrom(clazz)
					&& writer == null
			        && reader == null // reader can't be a writer, cause the signature doesn't match, just an optimization
			        && declaredMethodName.startsWith("set")
			        && declaredMethodName.length() > 3
			        && Character.isUpperCase(declaredMethodName.charAt(3))
			        && isValidSetter(declaredMethod)) {

			        Class<?> paramType = declaredMethod.getParameterTypes()[0];
			        String propertyName = Character.toLowerCase(declaredMethodName.charAt(3))
			                            + declaredMethodName.substring(4);

			        if (!propertyReaders.containsKey(propertyName)) {
			            // We need a reader as well, and the reader must not be annotated as disabled
			            // The reader can be called 'xxx', 'getXxx', 'isXxx' or 'hasXxx'
			            // (just booleans for the last two)
			            Method readerMethod = notBogus(getDeclaredMethod(clazz, propertyName, null), propertyName, paramType);
			            if (readerMethod == null)
			                readerMethod = notBogus(getDeclaredMethod(clazz, "get" + capitalizeFirst(propertyName), null), propertyName, paramType);
			            if (readerMethod == null && isBoolean(paramType))
			                readerMethod = notBogus(getDeclaredMethod(clazz, "is" + capitalizeFirst(propertyName), null), propertyName, paramType);
			            if (readerMethod == null && isBoolean(paramType))
			                readerMethod = notBogus(getDeclaredMethod(clazz, "has" + capitalizeFirst(propertyName), null), propertyName, paramType);

			            if (readerMethod != null) { // yay
			                reader = Container.readerAnnotation(readerMethod);
			                if (reader == null) {
			                    propertyReaders.put(propertyName, readerMethod);
			                    propertyWriters.put(propertyName, declaredMethod);

			                    propertyDesignableResolvers.put(propertyName, isDesignable(readerMethod, clazz));
			                    propertyScriptableResolvers.put(propertyName, isScriptable(readerMethod, clazz));
			                    propertyUserResolvers.put(propertyName, isUser(readerMethod, clazz));
			                    propertyRequiredResolvers.put(propertyName, isRequired(readerMethod, clazz));
			                }
			            }
			        }
			    }
			}
			ArrayList<Method> slotsMethods = new ArrayList<Method>(slots);
			slotsMethods.addAll(methods);
			
			metaData.signalsArray = signalFields.toArray(new Field[signalFields.size()]);
			metaData.slotsArray = slotsMethods.toArray(new Method[slotsMethods.size()]);
			metaData.constructorsArray = constructors.toArray(new Constructor[constructors.size()]);
			int flagsIndex = 0;
			{
			    // Until 4.7.x QtJambi used revision=1 however due to a change in the way
			    //  4.7.x works some features of QtJambi stopped working.
			    // revision 1         = MO_HEADER_LEN=10
			    // revision 2 (4.5.x) = MO_HEADER_LEN=12 (added: constructorCount, constructorData)
			    // revision 3         = MO_HEADER_LEN=13 (added: flags)
			    // revision 4 (4.6.x) = MO_HEADER_LEN=14 (added: signalCount)
			    // revision 5 (4.7.x) = MO_HEADER_LEN=14 (normalization)
			    // revision 6 (4.8.x) = MO_HEADER_LEN=14 (added support for qt_static_metacall)
			    // revision 7 (5.0.x) = MO_HEADER_LEN=14 (Qt5 to break backwards compatibility)
			    // The format is compatible to share the same encoding code
			    //  then we can change the revision to suit the Qt
			    /// implementation we are working with.

				final int MO_HEADER_LEN = 14;  // header size        	
				// revision
				intdata.add(resolveMetaDataRevision());		intdataComments.add("revision");
				// classname
				intdata.add(strings.indexOf(classname));		intdataComments.add("className");
				// classinfo
				intdata.add(classInfos.size());		intdataComments.add("classInfoCount");
				intdata.add(classInfos.isEmpty() ? 0 : MO_HEADER_LEN);		intdataComments.add("classInfoData");
				
				// methods
				int methodCount = signalFields.size() + slots.size() + methods.size();
				intdata.add(methodCount);		intdataComments.add("methodCount");
				final int METHOD_METADATA_INDEX = intdata.size();
				intdata.add(0);		intdataComments.add("methodData");
				
			    // properties
			    intdata.add(propertyReaders.size());		intdataComments.add("propertyCount");
			    final int PROPERTY_METADATA_INDEX = intdata.size();
				intdata.add(0);		intdataComments.add("propertyData");
				
				// enums/sets
				intdata.add(enums.size());		intdataComments.add("enumeratorCount");
				final int ENUM_METADATA_INDEX = intdata.size();
				intdata.add(0);		intdataComments.add("enumeratorData");
				
				// constructors
				intdata.add(!constructors.isEmpty() ? constructors.size() : 0);		intdataComments.add("constructorCount");
				final int CONSTRUCTOR_METADATA_INDEX = intdata.size();
				intdata.add(0);		intdataComments.add("constructorData");
				
				// flags
				flagsIndex = intdata.size();
				intdata.add(0);		intdataComments.add("flags");
				
				// signalCount
				intdata.add(signalFields.size());		intdataComments.add("signalCount");
				
				//
				// Build classinfo array
				//
				for(Map.Entry<String,String> entry : classInfos.entrySet()){
					// classinfo: key, value
					intdata.add(strings.indexOf(entry.getKey()));		intdataComments.add("classinfo: key");
					intdata.add(strings.indexOf(entry.getValue()));		intdataComments.add("classinfo: value");
				}
				
				HashMap<Object,Integer> paramIndexOfMethods = new HashMap<Object,Integer>();
				
			    //
				// Build signals array first, otherwise the signal indices would be wrong
				//
				if(signalFields.size() + slotsMethods.size() > 0){

					intdata.set(METHOD_METADATA_INDEX, intdata.size());
					
			    	for (int i = 0; i < signalFields.size(); ++i) {
			    		Field signalField = signalFields.get(i);
			    		QtJambiInternal.ResolvedSignal resolvedSignal = resolvedSignals.get(i);
			    		MethodAttributes flags = MethodFlags.MethodSignal.asFlags();
			    		if (Modifier.isPrivate(signalField.getModifiers()))
			    			flags.set(MethodFlags.AccessPrivate);
			            else if (Modifier.isPublic(signalField.getModifiers()))
			                flags.set(MethodFlags.AccessPublic);
			            else
			                flags.set(MethodFlags.AccessProtected);
			            
			    		if (signalField.isAnnotationPresent(QtInvokable.class))
			                flags.set(MethodFlags.MethodScriptable);
			    		int argc = resolvedSignal.signalTypes.size();
			    		
			    		// signals: name, argc, parameters, tag, flags
			    		intdata.add(strings.indexOf(signalField.getName()));		intdataComments.add("signal["+i+"]: name");
			    		intdata.add(argc);		intdataComments.add("signal["+i+"]: argc");
			    		paramIndexOfMethods.put(resolvedSignal, intdata.size());
			    		intdata.add(0);		intdataComments.add("signal["+i+"]: parameters");
			    		intdata.add(strings.indexOf(""));		intdataComments.add("signal["+i+"]: tag");
			    		intdata.add(flags.value());		intdataComments.add("signal["+i+"]: flags");
			    	}
			    	
			    	//
			    	// Build slots array
			    	// Build method array
			    	//
			    	for (int i = 0; i < slotsMethods.size(); i++) {
						Method method = slotsMethods.get(i);
						MethodAttributes flags = slots.contains(method) ? MethodFlags.MethodSlot.asFlags() : MethodFlags.MethodMethod.asFlags();
			    		if (Modifier.isPrivate(method.getModifiers()))
			                flags.set(MethodFlags.AccessPrivate);
			            else if (Modifier.isPublic(method.getModifiers()))
			            	flags.set(MethodFlags.AccessPublic);
			            else
			            	flags.set(MethodFlags.AccessProtected);
			            
			    		if (!method.isAnnotationPresent(QtInvokable.class) || method.getAnnotation(QtInvokable.class).value())
			    			flags.set(MethodFlags.MethodScriptable);
			    		int argc = method.getParameterTypes().length;
			    		
			    		// slots: name, argc, parameters, tag, flags
			    		intdata.add(strings.indexOf(method.getName()));
			    		intdataComments.add("slot["+i+"]: name");
			    		intdata.add(argc);
			    		intdataComments.add("slot["+i+"]: argc");
			    		paramIndexOfMethods.put(method, intdata.size());
			    		intdata.add(0);
			    		intdataComments.add("slot["+i+"]: parameters");
			    		intdata.add(strings.indexOf(""));
			    		intdataComments.add("slot["+i+"]: tag");
			    		intdata.add(flags.value());
			    		intdataComments.add("slot["+i+"]: flags");
					}
				}
				
				//
				// Build method parameters array
				//
				
				for (int i = 0; i < signalFields.size(); ++i) {
					QtJambiInternal.ResolvedSignal resolvedSignal = resolvedSignals.get(i);
					// signals: parameters
					int METHOD_PARAMETER_INDEX = paramIndexOfMethods.get(resolvedSignal);
					intdata.set(METHOD_PARAMETER_INDEX, intdata.size());
					intdata.add(QMetaType.Type.Void.value());		intdataComments.add("signal["+i+"].returnType");
					for (int j = 0; j < resolvedSignal.signalTypes.size(); j++) {
						QtJambiSignals.SignalParameterType signalType = resolvedSignal.signalTypes.get(j);
						String typeName;
			        	if(signalType.arrayDimension==0){
			        		typeName = internalTypeNameOfClass(signalType.type, signalType.genericType);
			        	}else{
			        		typeName = internalTypeNameOfClass(Object.class, Object.class);
			        	}
			        	QMetaType.Type type = metaType(typeName);
			        	if(type==QMetaType.Type.UnknownType || type==QMetaType.Type.User){
			        		if(!strings.contains(typeName)){
			        			strings.add(typeName);
			        		}
			        		intdata.add(0x80000000 | strings.indexOf(typeName));
			        	}else{
			        		intdata.add(type.value());
			        	}
			    		intdataComments.add("signal["+i+"]: parameter["+j+"].arg");
					}
					for (int j = 1; j <= resolvedSignal.signalTypes.size(); j++) {
						intdata.add(strings.indexOf("arg__"+j));		intdataComments.add("signal["+i+"]: parameter["+j+"].argName");
					}
				}
			    
			    //
			    // Build constructors array
			    //
			    
			    if(!constructors.isEmpty()){
			    	intdata.set(CONSTRUCTOR_METADATA_INDEX, intdata.size());
			        for (int i = 0; i < constructors.size(); i++) {
						Constructor<?> constructor = constructors.get(i);
						MethodAttributes flags = MethodFlags.MethodConstructor.asFlags();
			    		if (Modifier.isPrivate(constructor.getModifiers()))
			                flags.set(MethodFlags.AccessPrivate);
			            else if (Modifier.isPublic(constructor.getModifiers()))
			            	flags.set(MethodFlags.AccessPublic);
			            else
			            	flags.set(MethodFlags.AccessProtected);
			            
			    		if (constructor.isAnnotationPresent(QtInvokable.class) && constructor.getAnnotation(QtInvokable.class).value())
			    			flags.set(MethodFlags.MethodScriptable);
			    		int argc = constructor.getParameterTypes().length;
			    		
			    		// constructors: name, argc, parameters, tag, flags
			    		String className = constructor.getDeclaringClass().getName();
			    		if(className.contains(".")){
			    			className = className.substring(className.lastIndexOf('.')+1);
			    		}
			    		strings.add(className);
			    		intdata.add(strings.indexOf(className));		intdataComments.add("constructor["+i+"]: name");
			    		intdata.add(argc);		intdataComments.add("constructor["+i+"]: argc");
			    		paramIndexOfMethods.put(constructor, intdata.size());
			    		intdata.add(0);		intdataComments.add("constructor["+i+"]: parameters");
			    		intdata.add(strings.indexOf(""));		intdataComments.add("constructor["+i+"]: tag");
			    		intdata.add(flags.value());		intdataComments.add("constructor["+i+"]: flags");
					}
			    }
				
				for (int i = 0; i < slotsMethods.size(); i++) {
					Method method = slotsMethods.get(i);
					// slot/method: parameters
					int METHOD_PARAMETER_INDEX = paramIndexOfMethods.get(method);
					intdata.set(METHOD_PARAMETER_INDEX, intdata.size());
					String typeName = internalTypeNameOfClass(method.getReturnType(), method.getGenericReturnType());
					if(method.getAnnotatedReturnType().isAnnotationPresent(QtPointerType.class)) {
			    		if(!typeName.endsWith("*")) {
			    			typeName += "*";
			    		}
			    	}else if(method.getAnnotatedReturnType().isAnnotationPresent(QtReferenceType.class)) {
			    		if(!typeName.endsWith("&")) {
			    			typeName += "&";
			    		}
			    	}
					QMetaType.Type type = metaType(typeName);
			    	if(type==QMetaType.Type.UnknownType || type==QMetaType.Type.User){
			    		if(!strings.contains(typeName)){
			    			strings.add(typeName);
			    		}
			    		intdata.add(0x80000000 | strings.indexOf(typeName));
			    	}else{
			    		intdata.add(type.value());
			    	}
					intdataComments.add("slot["+i+"].returnType");
					Parameter[] parameters = method.getParameters();
					Class<?>[] parameterTypes = method.getParameterTypes();
					Type[] genericParameterTypes = method.getGenericParameterTypes();
					AnnotatedType[] annotatedParameterTypes = method.getAnnotatedParameterTypes();
					for (int j = 0; j < parameterTypes.length; j++) {
			    		typeName = internalTypeNameOfClass(parameterTypes[j], genericParameterTypes[j]);
			    		if(annotatedParameterTypes[j].isAnnotationPresent(QtPointerType.class)) {
			        		if(!typeName.endsWith("*")) {
			        			typeName += "*";
			        		}
			        	}else if(annotatedParameterTypes[j].isAnnotationPresent(QtReferenceType.class)) {
			        		if(!typeName.endsWith("&")) {
			        			typeName += "&";
			        		}
			        	}
			        	type = metaType(typeName);
			        	if(type==QMetaType.Type.UnknownType || type==QMetaType.Type.User){
			        		if(!strings.contains(typeName)){
			        			strings.add(typeName);
			        		}
			        		intdata.add(0x80000000 | strings.indexOf(typeName));
			        	}else{
			        		intdata.add(type.value());
			        	}
			    		intdataComments.add("slot["+i+"]: parameter["+j+"].arg");
					}
					for (int j = 0; j < parameterTypes.length; j++) {
			    		if(parameters[j].isNamePresent()) {
			    			intdata.add(strings.indexOf(parameters[j].getName()));
			        	}else {
			        		intdata.add(strings.indexOf("arg__"+(j+1)));
			        	}
						intdataComments.add("slot["+i+"]: parameter["+j+"].argName");
					}
				}
				
				for (int i = 0; i < constructors.size(); i++) {
					Constructor<?> constructor = constructors.get(i);
					// constructors: parameters
					int METHOD_PARAMETER_INDEX = paramIndexOfMethods.get(constructor);
					intdata.set(METHOD_PARAMETER_INDEX, intdata.size());
//				paramIndexOfMethods.put(constructor, intdata.size());
					intdata.add(0x80000000 | strings.indexOf(""));
					intdataComments.add("constructor["+i+"].returnType");
					Parameter[] parameters = constructor.getParameters();
					Class<?>[] parameterTypes = constructor.getParameterTypes();
					Type[] genericParameterTypes = constructor.getGenericParameterTypes();
					AnnotatedType[] annotatedParameterTypes = constructor.getAnnotatedParameterTypes();
					for (int j = 0; j < parameterTypes.length; j++) {
						String typeName = internalTypeNameOfClass(parameterTypes[j], genericParameterTypes[j]);
						if(annotatedParameterTypes[j].isAnnotationPresent(QtPointerType.class)) {
			        		if(!typeName.endsWith("*")) {
			        			typeName += "*";
			        		}
			        	}else if(annotatedParameterTypes[j].isAnnotationPresent(QtReferenceType.class)) {
			        		if(!typeName.endsWith("&")) {
			        			typeName += "&";
			        		}
			        	}
						QMetaType.Type type = metaType(typeName);
			        	if(type==QMetaType.Type.UnknownType || type==QMetaType.Type.User){
			        		if(!strings.contains(typeName)){
			        			strings.add(typeName);
			        		}
			        		intdata.add(0x80000000 | strings.indexOf(typeName));
			        	}else{
			        		intdata.add(type.value());
			        	}
			        	intdataComments.add("constructor["+i+"]: parameter["+(j)+"].arg");
					}
					for (int j = 0; j < parameterTypes.length; j++) {
			        	if(parameters[j].isNamePresent()) {
			    			intdata.add(strings.indexOf(parameters[j].getName()));
			        	}else {
			        		intdata.add(strings.indexOf("arg__"+(j+1)));
			        	}
						intdataComments.add("constructor["+i+"]: parameter["+(j)+"].argName");
					}
				}
				
				//
				// Build property array
				//
				MetaObjectAttributes metaObjectFlags = new MetaObjectAttributes(0);
				
				String propertyNames[] = propertyReaders.keySet().toArray(new String[propertyReaders.keySet().size()]);
			    metaData.propertyReadersArray = new Method[propertyNames.length];
			    metaData.propertyResettersArray = new Method[propertyNames.length];
			    metaData.propertyNotifiesArray = new Field[propertyNames.length];
			    metaData.propertyWritersArray = new Method[propertyNames.length];
			    metaData.propertyDesignableResolverArray = new Method[propertyNames.length];
			    metaData.propertyScriptableResolverArray = new Method[propertyNames.length];
			    metaData.propertyEditableResolverArray = new Method[propertyNames.length];
			    metaData.propertyStoredResolverArray = new Method[propertyNames.length];
			    metaData.propertyUserResolverArray = new Method[propertyNames.length];
			    if(!propertyReaders.isEmpty()){
			    	if(!QObject.class.isAssignableFrom(clazz)) {
			    		metaObjectFlags.set(MetaObjectFlags.PropertyAccessInStaticMetaCall);
			    	}
			    	intdata.set(PROPERTY_METADATA_INDEX, intdata.size());
			        for (int i=0; i<propertyNames.length; ++i) {
			            Method reader = propertyReaders.get(propertyNames[i]);
			            Method writer = propertyWriters.get(propertyNames[i]);
			            Method resetter = propertyResetters.get(propertyNames[i]);
			            Field notify = propertyNotifies.get(propertyNames[i]);
			            Object designableVariant = propertyDesignableResolvers.get(propertyNames[i]);
			            Object scriptableVariant = propertyScriptableResolvers.get(propertyNames[i]);
			            Object editableVariant = propertyEditableResolvers.get(propertyNames[i]);
			            Object storedVariant = propertyStoredResolvers.get(propertyNames[i]);
			            Object userVariant = propertyUserResolvers.get(propertyNames[i]);
			            Boolean requiredVariant = propertyRequiredResolvers.get(propertyNames[i]);
			            Boolean constantVariant = propertyConstantResolvers.get(propertyNames[i]);
			            Boolean finalVariant = propertyFinalResolvers.get(propertyNames[i]);

			            if (writer != null && !reader.getReturnType().isAssignableFrom(writer.getParameterTypes()[0])) {
			            	Logger.getLogger("io.qt.internal").warning("Writer for property '"
			            			+ clazz.getName() + "::" + propertyNames[i] + "' takes a type (" + writer.getParameterTypes()[0].getName() + ") which is incompatible with reader's return type (" + reader.getReturnType().getName() + ").");
			                writer = null;
			            }

			            PropertyAttributes flags = PropertyFlags.Invalid.asFlags();
			            // Type (need to special case flags and enums)
			            String typeName = internalTypeNameOfClass(reader.getReturnType(),reader.getGenericReturnType());
			            if(reader.getAnnotatedReturnType().isAnnotationPresent(QtPointerType.class)) {
			        		if(!typeName.endsWith("*")) {
			        			typeName += "*";
			        		}
			        	}else if(reader.getAnnotatedReturnType().isAnnotationPresent(QtReferenceType.class)) {
			        		if(!typeName.endsWith("&")) {
			        			typeName += "&";
			        		}
			        	}
			            
			            if (!isBuiltinType(typeName))
			                flags.set(PropertyFlags.EnumOrFlag);
			            if (writer!=null){
			                flags.set(PropertyFlags.Writable);
			                String s = "set";
			                s += propertyNames[i].toUpperCase().charAt(0);
			                s += propertyNames[i].substring(1);
			                if (s.equals(writer.getName()))
			                    flags.set(PropertyFlags.StdCppSet);

			            }
			            if (reader!=null)
			                flags.set(PropertyFlags.Readable);
			            if (resetter!=null)
			                flags.set(PropertyFlags.Resettable);
			            
			            if (designableVariant instanceof Boolean) {
			                if ((Boolean) designableVariant)
			                	flags.set(PropertyFlags.Designable);
			            } else if (designableVariant instanceof Method) {
			                metaData.propertyDesignableResolverArray[i] = (Method) designableVariant;
			            	flags.set(PropertyFlags.ResolveDesignable);
			            }
			            
			            if (scriptableVariant instanceof Boolean) {
			                if ((Boolean) scriptableVariant)
			                	flags.set(PropertyFlags.Scriptable);
			            } else if (scriptableVariant instanceof Method) {
			            	flags.set(PropertyFlags.ResolveScriptable);
			                metaData.propertyScriptableResolverArray[i] = (Method) scriptableVariant;
			            }
			            
			            if (editableVariant instanceof Boolean) {
			                if ((Boolean) editableVariant)
			                	flags.set(PropertyFlags.Editable);
			            } else if (editableVariant instanceof Method) {
			            	flags.set(PropertyFlags.ResolveEditable);
			                metaData.propertyEditableResolverArray[i] = (Method) editableVariant;
			            }
			            
			            if (storedVariant instanceof Boolean) {
			                if ((Boolean) storedVariant)
			                	flags.set(PropertyFlags.Stored);
			            } else if (storedVariant instanceof Method) {
			            	flags.set(PropertyFlags.ResolveStored);
			                metaData.propertyStoredResolverArray[i] = (Method) storedVariant;
			            }
			            
			            if (userVariant instanceof Boolean) {
			                if ((Boolean) userVariant)
			                	flags.set(PropertyFlags.User);
			            } else if (userVariant instanceof Method) {
			            	flags.set(PropertyFlags.ResolveUser);
			                metaData.propertyUserResolverArray[i] = (Method) userVariant;
			            }
			            
			            if (constantVariant!=null && constantVariant && writer!=null && notify!=null) {
		                	flags.set(PropertyFlags.Constant);
			            }
			            
			            if (requiredVariant!=null && requiredVariant) {
		                	try {
								flags.set(PropertyFlags.valueOf("Required"));
							} catch (Throwable e1) {
							}
			            }

			            if (finalVariant!=null && finalVariant) {
		                	flags.set(PropertyFlags.Final);
			            }
			            
			            if (notify!=null)
			                flags.set(PropertyFlags.Notify);
			            
			         // properties: name, type, flags
			            int idx = strings.indexOf(propertyNames[i]);
			            if(idx==-1){
			            	strings.add(propertyNames[i]);
			            	idx = strings.indexOf(propertyNames[i]);
			            }
			            intdata.add(idx);
			            intdataComments.add("property["+i+"].name");
			            QMetaType.Type type = metaType(typeName);
			        	if(type==QMetaType.Type.UnknownType || type==QMetaType.Type.User){
			        		idx = strings.indexOf(typeName);
			        		if(idx==-1){
			                	strings.add(typeName);
			                	idx = strings.indexOf(typeName);
			                }
			        		intdata.add(0x80000000 | idx);
			        	}else{
			        		intdata.add(type.value());
			        	}
			        	intdataComments.add("property["+i+"].type");
			            intdata.add(flags.value());
			            intdataComments.add("property["+i+"].flags");
			            
			            metaData.propertyReadersArray[i] = reader;
			            metaData.propertyWritersArray[i] = writer;
			            metaData.propertyResettersArray[i] = resetter;
			            metaData.propertyNotifiesArray[i] = notify;
			        }
			        
			        for (int i=0; i<propertyNames.length; ++i) {
			        	Field notify = propertyNotifies.get(propertyNames[i]);
			        	if(notify!=null) {
			        		if(signalFields.contains(notify)) {
			        			intdata.add(signalFields.indexOf(notify));
			        		}else {
			        			intdata.add(0x70000000);	            			
			        		}
			                intdataComments.add("property["+i+"].notify_signal_id");
			        	}else {
			        		intdata.add(0);
			        		intdataComments.add("property["+i+"].notify_signal_id");
			        	}
			        }
			    }
			    
			    if(metaObjectFlags.value()!=0) {
			    	intdata.set(flagsIndex, metaObjectFlags.value());
			    }
			    //
			    // Build enums array
			    //
			    
			    if(!enums.isEmpty()){
			    	intdata.set(ENUM_METADATA_INDEX, intdata.size());
			        List<Class<?>> enumList = new ArrayList<Class<?>>(enums.values());
			        HashMap<Object,Integer> dataIndexOfEnums = new HashMap<Object,Integer>();
			        
			        for (int i = 0; i < enumList.size(); i++) {
			        	Class<?> enumClass = enumList.get(i);
			            // enums: name, flags, count, data
			        	strings.add(enumClass.getSimpleName());
			        	intdata.add(strings.indexOf(enumClass.getSimpleName()));	intdataComments.add("enum["+i+"].name");
			        	intdata.add(QFlags.class.isAssignableFrom(enumClass) ? 0x1 : 0x0);	intdataComments.add("enum["+i+"].flags");
			        	
			            // Get the enum class
			            Class<?> contentEnumClass = Enum.class.isAssignableFrom(enumClass) ? enumClass : getEnumForQFlags(enumClass);
			        	
			        	intdata.add(contentEnumClass.getEnumConstants().length);	intdataComments.add("enum["+i+"].count");
			        	dataIndexOfEnums.put(enumClass, intdata.size());
			        	intdata.add(0);	intdataComments.add("enum["+i+"].data");
			        }
			        
			        for (int i = 0; i < enumList.size(); i++) {
			        	Class<?> enumClass = enumList.get(i);
			        	@SuppressWarnings("unchecked")
						Class<Enum<?>> contentEnumClass = (Class<Enum<?>>)(Enum.class.isAssignableFrom(enumClass) ? enumClass : getEnumForQFlags(enumClass));
			        	// enum data: key, value
			    		int ENUM_DATA_INDEX = dataIndexOfEnums.get(enumClass);
			    		intdata.set(ENUM_DATA_INDEX, intdata.size());
			        	for(Enum<?> enumConstant : contentEnumClass.getEnumConstants()){
			        		strings.add(enumConstant.name());
			        		intdata.add(strings.indexOf(enumConstant.name()));	intdataComments.add("enum["+i+"].data: key");
			            	if(enumConstant instanceof QtEnumerator){
			            		QtEnumerator enumerator = (QtEnumerator)enumConstant;
			            		intdata.add(enumerator.value());
			            	}else if(enumConstant instanceof QtShortEnumerator){
			            		QtShortEnumerator enumerator = (QtShortEnumerator)enumConstant;
			            		intdata.add((int)enumerator.value());
			            	}else if(enumConstant instanceof QtByteEnumerator){
			            		QtByteEnumerator enumerator = (QtByteEnumerator)enumConstant;
			            		intdata.add((int)enumerator.value());
			            	}else if(enumConstant instanceof QtLongEnumerator){
			            		QtLongEnumerator enumerator = (QtLongEnumerator)enumConstant;
			            		intdata.add((int)enumerator.value());
			            	}else{
			            		intdata.add(enumConstant.ordinal());
			            	}
			            	intdataComments.add("enum["+i+"].data: value");
			        	}
			        }
			    }
			    
			    //
			    // Terminate data array
			    //
			    intdata.add(0); // eod
				intdataComments.add("end of data");
			}

			metaData.metaData = new int[intdata.size()];
			if(intdataComments instanceof ArrayList) {
			    List<String> nms = Arrays.asList(
			    		"revision",
			    	    "className",
			    	    "classInfoCount",
			    	    "classInfoData",
			    	    "methodCount",
			    	    "methodData",
			    	    "propertyCount",
			    	    "propertyData",
			    	    "enumeratorCount",
			    	    "enumeratorData",
			    	    "constructorCount",
			    	    "constructorData",
			    	    "flags",
			    	    "signalCount"
					);
			    System.out.println(classname+": metaData.metaData{");
			    for (int i = 0; i < metaData.metaData.length; i++) {
			    	try {
			    		String strg = null;
			    		try {
							if(intdataComments.get(i).endsWith("]: name")) {
								strg = strings.get(intdata.get(i));
							}else if(intdataComments.get(i).endsWith("].argName")) {
								strg = strings.get(intdata.get(i));
							}else if(intdataComments.get(i).endsWith("].arg")) {
								int idx = intdata.get(i);
								if((idx & 0x80000000) == 0x80000000) {
									idx = idx & ~0x80000000;
									if(idx>=0 && idx<strings.size())
										strg = strings.get(idx);
								}else if(idx>=0 && idx<QMetaType.Type.values().length){
									for(QMetaType.Type t : QMetaType.Type.values()) {
										if(t.value()==idx) {
											strg = ""+t;
											break;
										}
									}
								}
							}else if(intdataComments.get(i).endsWith("].returnType")) {
								int idx = intdata.get(i);
								if((idx & 0x80000000) == 0x80000000) {
									idx = idx & ~0x80000000;
									if(idx>=0 && idx<strings.size())
										strg = strings.get(idx);
								}else if(idx>=0 && idx<QMetaType.Type.values().length){
									for(QMetaType.Type t : QMetaType.Type.values()) {
										if(t.value()==idx) {
											strg = ""+t;
											break;
										}
									}
								}
							}
						} catch (Exception e) {
							strg = "???";
						}
			    		if(strg!=null) {
			    			if(i<nms.size()) {
								System.out.printf("\t%1$s: %3$s=%2$s (%4$s) --> %5$s\n", i, intdata.get(i), intdataComments.get(i), nms.get(i), strg);
							}else {
								System.out.printf("\t%1$s: %3$s=%2$s --> %4$s\n", i, intdata.get(i), intdataComments.get(i), strg);
							}
			    		}else {
			    			if(i<nms.size()) {
								System.out.printf("\t%1$s: %3$s=%2$s (%4$s)\n", i, intdata.get(i), intdataComments.get(i), nms.get(i));
							}else {
								System.out.printf("\t%1$s: %3$s=%2$s\n", i, intdata.get(i), intdataComments.get(i));
							}
			    		}
					} catch (IndexOutOfBoundsException e) {
						System.out.printf("\t%1$s: %2$s\n", i, intdata.get(i));
					}
			    	metaData.metaData[i] = intdata.get(i);
				}
			    System.out.println("}");
			}else {
				for (int i = 0; i < metaData.metaData.length; i++) {
					metaData.metaData[i] = intdata.get(i);
				}
			}
			
			metaData.stringData = new byte[strings.size()][];
			for (int i = 0; i < metaData.stringData.length; i++) {
//        	System.out.printf("string[%1$s]= %2$s\n", i, strings.get(i));
				try {
					metaData.stringData[i] = strings.get(i).getBytes("UTF-8");
				} catch (UnsupportedEncodingException e) {
					metaData.stringData[i] = strings.get(i).getBytes();
				}
			}
			metaData.originalSignatures = originalSignatures.toArray(new String[originalSignatures.size()]);
			return metaData;
		} catch (RuntimeException | Error e) {
			e.printStackTrace();
			throw e;
		} catch (Throwable e) {
			e.printStackTrace();
			throw new RuntimeException(e);
		}
    }

    private static boolean overridesGeneratedSlot(Method declaredMethod, Class<?> clazz) {
    	if(!Modifier.isPrivate(declaredMethod.getModifiers()) && !Modifier.isStatic(declaredMethod.getModifiers()) && clazz.getSuperclass()!=null) {
    		try {
				Method declaredSuperMethod = clazz.getSuperclass().getDeclaredMethod(declaredMethod.getName(), declaredMethod.getParameterTypes());
				if(declaredSuperMethod!=null) {
					Class<?> declaringClass = declaredSuperMethod.getDeclaringClass();
					if(QtJambiInternal.isGeneratedClass(declaringClass)) {
						return true;
					}else {
						return overridesGeneratedSlot(declaredSuperMethod, declaringClass);
					}
				}
			} catch (Throwable e) {
			}
    	}
		return false;
	}

	private static native int registerQmlListProperty(String type);

	// Using a variable to ensure this is changed in all the right places in the
    //  future when higher values are supported.
    public static final int METAOBJECT_REVISION_HIGHEST_SUPPORTED = 7;
    // This property allows you to override the QMetaObject revision number for
    //  QtJambi to use.
    public static final String K_io_qt_qtjambi_metadata_revision = "io.qt.qtjambi.metadata.revision";
    private static int revision;
    // This should be updated as the code-base supports the correct data layout
    //  for each new revision.  We don't necessarily have to support the features
    //  that new revision brings as well.
    private static int resolveMetaDataRevision() {
        int r = revision;
        if(r != 0)
            return r;

        int[] versionA = NativeLibraryManager.getVersion();
        int major = -1;
        if(versionA.length > 0)
            major = versionA[0];
        int minor = -1;
        if(versionA.length > 1)
            minor = versionA[1];
        @SuppressWarnings("unused")
		int plevel = -1;
        if(versionA.length > 2)
            plevel = versionA[2];
        // It became a requirement in 4.7.x to move away from revision 1
        //  in order to restore broken functionality due to improvements
        //  in Qt.  Before this time QtJambi always used to report
        //  revision=1.
        // The following is the default version for that version of Qt
        //  this is compatible with what QtJambi provides and needs to
        //  be updated with any future revision.
        if(major <= 3)
            r = 1;  // Good luck with getting QtJambi working!
        else if(major == 4 && minor <= 5)
            r = 1;  // historically this version was used
        else if(major == 4 && minor == 6)
            r = 4;  // 4.6.x (historically revision 1 was used)
        else if(major == 4 && minor == 7)
            r = 5;  // 4.7.x (known issues with 1 through 3, use revision 4 minimum, 5 is best)
        else if(major == 4)
            r = 6;  // 4.8.x
        else if(major == 5)
            r = 7;  // 5.0.x (Qt5 requires a minimum of this revision)
        else  // All future versions
            r = METAOBJECT_REVISION_HIGHEST_SUPPORTED;

        // The above computes the automatic default so we can report it below
        int revisionOverride = resolveMetaDataRevisionFromSystemProperty(r);
        if(revisionOverride > 0)
            r = revisionOverride;

        revision = r;
        return r;
    }

    /**
     * A facility to override the default metadata revision with a system property.
     * More useful for testing and fault diagnosis than any practical runtime purpose.
     * @param defaultRevision Value only used in error messages.
     * @return -1 on parse error, 0 when no configured, >0 is a configured value.
     */
    private static int resolveMetaDataRevisionFromSystemProperty(int defaultRevision) {
        int r = 0;
        String s = null;
        try {
            s = System.getProperty(K_io_qt_qtjambi_metadata_revision);
            if(s != null) {
                r = Integer.parseInt(s);
                if(r <= 0 || r > METAOBJECT_REVISION_HIGHEST_SUPPORTED)
                    r = -1;  // invalidate causing the value to be ignored
            }
        } catch(NumberFormatException e) {
            r = -1;
        } catch(SecurityException e) {
            java.util.logging.Logger.getLogger("io.qt.internal").log(java.util.logging.Level.SEVERE, "", e);
        }
        if(r < 0)
            System.err.println("System Property " + K_io_qt_qtjambi_metadata_revision + " invalid value: " + s + " using default: " + defaultRevision);
        return r;
    }
    
    private static int generateTypeInfo(List<String> strings, String typeName){
        if (isBuiltinType(typeName)) {
        	QMetaType.Type type = nameToBuiltinType(typeName);
        	return type.value();
        } else {
        	return 0x80000000 | strings.indexOf(typeName);
        }
    }
    
    private static boolean isBuiltinType(String type)
    {
    	QMetaType.Type id = metaType(type);
       if (id == QMetaType.Type.UnknownType)
           return false;
       return (id.value() < QMetaType.Type.User.value());
    }
    
    private static QMetaType.Type nameToBuiltinType(String name)
    {
        if (name.isEmpty())
            return QMetaType.Type.UnknownType;

        QMetaType.Type tp = metaType(name);
        return tp.value() < QMetaType.Type.User.value() ? tp : QMetaType.Type.UnknownType;
    }
    
    private static int aggregateParameterCount(List<?> list)
    {
        int sum = 0;
        for (int i = 0; i < list.size(); ++i){
        	Object method = list.get(i);
        	if(method instanceof QtJambiInternal.ResolvedSignal){
        		sum += ((QtJambiInternal.ResolvedSignal)method).signalTypes.size() + 1; // +1 for return type
        	}else if(method instanceof Method){
        		sum += ((Method)method).getGenericParameterTypes().length + 1; // +1 for return type
        	}else if(method instanceof Constructor){
        		sum += ((Constructor<?>)method).getGenericParameterTypes().length + 1; // +1 for return type
        	}
        }
        return sum;
    }
    

    
    static QMetaType.Type metaType(String typeName){
        if("double".equals(typeName)){
            return QMetaType.Type.Double;
        }
        if("int".equals(typeName)){
            return QMetaType.Type.Int;
        }
        if("float".equals(typeName)){
            return QMetaType.Type.Float;
        }
        if("boolean".equals(typeName) || "bool".equals(typeName)){
            return QMetaType.Type.Bool;
        }
        if("short".equals(typeName)){
            return QMetaType.Type.Short;
        }
        if("long".equals(typeName)){
            return QMetaType.Type.Long;
        }
        if("byte".equals(typeName) || "char".equals(typeName)){
            return QMetaType.Type.Char;
        }
        if("void".equals(typeName)){
            return QMetaType.Type.Void;
        }
        try {
            return QMetaType.Type.valueOf(typeName);
        } catch (Exception e) {
            for(QMetaType.Type t : QMetaType.Type.values()){
                if(typeName.endsWith(t.name())){
                    return t;
                }
            }
            return QMetaType.Type.User;
        }
    }
}
