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

import static io.qt.internal.QtJambiInternal.internalTypeNameOfClass;
import static io.qt.internal.QtJambiInternal.registerMetaType;
import static io.qt.internal.QtJambiInternal.registerQmlListProperty;

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
import java.util.HashSet;
import java.util.Hashtable;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.TreeMap;
import java.util.logging.Logger;

import io.qt.NativeAccess;
import io.qt.QFlags;
import io.qt.QSignalDeclarationException;
import io.qt.QtByteEnumerator;
import io.qt.QtClassInfo;
import io.qt.QtEnumerator;
import io.qt.QtInvokable;
import io.qt.QtLongEnumerator;
import io.qt.QtPointerType;
import io.qt.QtPropertyConstant;
import io.qt.QtPropertyDesignable;
import io.qt.QtPropertyMember;
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
import io.qt.core.QMetaMethod;
import io.qt.core.QMetaObject;
import io.qt.core.QMetaType;
import io.qt.core.QObject;
import io.qt.core.QPair;
import io.qt.internal.QtJambiSignals.AbstractSignal;
import io.qt.internal.QtJambiSignals.SignalParameterType;


/**
 * Methods to help construct the fake meta object.
 */
final class MetaObjectTools extends AbstractMetaObjectTools{
	
	private static class PrivateConstructorAccess extends io.qt.QtObject{
    	static Class<?> type(){
    		return QPrivateConstructor.class;
    	}
    }
    
    private MetaObjectTools() { throw new RuntimeException();}
    
    static class AnnotationInfo{
    	AnnotationInfo(String name, boolean enabled) {
			super();
			this.name = name;
			this.enabled = enabled;
		}
		final String name; 
		final boolean enabled;
    }
    
    static class QPropertyTypeInfo{
		QPropertyTypeInfo(Class<?> propertyType, Type genericPropertyType, AnnotatedType annotatedPropertyType, boolean isPointer,
				boolean isReference, boolean isWritable) {
			super();
			this.propertyType = propertyType;
			this.genericPropertyType = genericPropertyType;
			this.annotatedPropertyType = annotatedPropertyType;
			this.isPointer = isPointer;
			this.isReference = isReference;
			this.isWritable = isWritable;
		}
		final Class<?> propertyType;
    	final Type genericPropertyType;
    	final AnnotatedType annotatedPropertyType;
    	final boolean isPointer;
    	final boolean isReference;
    	final boolean isWritable;
    }
    
    static class PropertyAnnotation {
        enum AnnotationType {
            Reader,
            Writer,
            Resetter,
            Notify,
            Bindable,
            Member
        }

        private Member member;
        private String name = null;
        private boolean enabled;
        private AnnotationType type;


        private PropertyAnnotation(String name, Member member, boolean enabled, AnnotationType type) {
            this.name = name;
            this.member = member;
            this.enabled = enabled;
            this.type = type;
        }

        private static String removeAndLowercaseFirst(String name, int count) {
            return Character.toLowerCase(name.charAt(count)) + name.substring(count + 1);
        }

        private String getNameFromMethod(Member member) {
        	String name = member.getName();
            switch(type) {
            case Resetter:
            	if(name.startsWith("reset") && name.length() > 5) {
            		return removeAndLowercaseFirst(name, 5);
            	}
                return "";
            case Bindable:
            	if(name.startsWith("bindable") && name.length() > 8) {
            		return removeAndLowercaseFirst(name, 8);
            	}
                return "";
            case Member:
            	if(name.endsWith("Prop") && name.length() > 4) {
            		return name.substring(0, name.length()-4);
            	}
            	if(name.endsWith("Property") && name.length() > 8) {
            		return name.substring(0, name.length()-8);
            	}
                return name;
            case Notify:
            	if(name.endsWith("Changed") && name.length() > 7) {
            		return name.substring(0, name.length()-7);
            	}
                return "";
            case Reader:
                int len = name.length();
                if (name.startsWith("get") && len > 3)
                    name = removeAndLowercaseFirst(name, 3);
                else if (isBoolean(((Method)member).getReturnType()) && name.startsWith("is") && len > 2)
                    name = removeAndLowercaseFirst(name, 2);
                else if (isBoolean(((Method)member).getReturnType()) && name.startsWith("has") && len > 3)
                    name = removeAndLowercaseFirst(name, 3);
                return name;
            case Writer: // starts with "set"
        	default:
                if (!name.startsWith("set") || name.length() <= 3) {
                    throw new IllegalArgumentException("The correct pattern for setter accessor names is setXxx where Xxx is the property name with upper case initial.");
                }
                name = removeAndLowercaseFirst(name, 3);
                return name;
            }
        }

        String name() {
            if (name == null || name.length() == 0)
                name = getNameFromMethod(member);

            return name;
        }

        boolean enabled() {
            return enabled;
        }

        static PropertyAnnotation readerAnnotation(Method method) {
            QtPropertyReader reader = method.getAnnotation(QtPropertyReader.class);
            return reader == null ? null : new PropertyAnnotation(reader.name(), method, reader.enabled(), AnnotationType.Reader);
        }
        
        static PropertyAnnotation memberAnnotation(Field field) {
            QtPropertyMember member = field.getAnnotation(QtPropertyMember.class);
            return member == null ? null : new PropertyAnnotation(member.name(), field, member.enabled(), AnnotationType.Member);
        }

        static PropertyAnnotation writerAnnotation(Method method) {
            QtPropertyWriter writer = method.getAnnotation(QtPropertyWriter.class);
            return writer == null ? null : new PropertyAnnotation(writer.name(), method, writer.enabled(), AnnotationType.Writer);
        }

        static PropertyAnnotation resetterAnnotation(Method method) {
            QtPropertyResetter resetter = method.getAnnotation(QtPropertyResetter.class);
            return resetter == null ? null : new PropertyAnnotation(resetter.name(), method, resetter.enabled(), AnnotationType.Resetter);
        }
        
        static PropertyAnnotation notifyAnnotation(Field field) {
            QtPropertyNotify notify = field.getAnnotation(QtPropertyNotify.class);
            return notify == null ? null : new PropertyAnnotation(notify.name(), field, notify.enabled(), AnnotationType.Notify);
        }
        
        static PropertyAnnotation bindableAnnotation(Method method) {
        	AnnotationInfo bindable = analyzeBindableAnnotation(method);
            return bindable == null ? null : new PropertyAnnotation(bindable.name, method, bindable.enabled, AnnotationType.Bindable);
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
    
    private static class MetaData {
        final @NativeAccess List<Integer> metaData = new ArrayList<>();
        final @NativeAccess List<String> stringData = new StringList();

        final @NativeAccess List<Field> signalFields = new ArrayList<>();
        final @NativeAccess List<Method> methods = new ArrayList<>();
        final @NativeAccess List<Constructor<?>> constructors = new ArrayList<>();

        final @NativeAccess List<Method> propertyReaders = new ArrayList<>();
        final @NativeAccess List<Method> propertyWriters = new ArrayList<>();
        final @NativeAccess List<Method> propertyResetters = new ArrayList<>();
        final @NativeAccess List<Field>  propertyNotifies = new ArrayList<>();
        final @NativeAccess List<Method> propertyBindables = new ArrayList<>();
        final @NativeAccess List<Field>  propertyQPropertyFields = new ArrayList<>();
        final @NativeAccess List<Field>  propertyMemberFields = new ArrayList<>();
        final @NativeAccess List<Method> propertyDesignableResolvers = new ArrayList<>();
        final @NativeAccess List<Method> propertyScriptableResolvers = new ArrayList<>();
        final @NativeAccess List<Method> propertyEditableResolvers = new ArrayList<>();
        final @NativeAccess List<Method> propertyStoredResolvers = new ArrayList<>();
        final @NativeAccess List<Method> propertyUserResolvers = new ArrayList<>();
        final @NativeAccess List<Class<?>> relatedMetaObjects = new ArrayList<>();

        @NativeAccess boolean hasStaticMembers;
        final @NativeAccess List<Integer> metaTypes = new ArrayList<>();
    }

    private static Method notBogus(Method method, String propertyName, Class<?> paramType) {
        if (method == null)
            return null;

        PropertyAnnotation reader = PropertyAnnotation.readerAnnotation(method);
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
            } else if(NativeLibraryManager.VERSION_MAJOR<6) { 
            	try {
	                Method m = clazz.getMethod(value);
	                if (isBoolean(m.getReturnType()))
	                    return m;
	                else
	                    throw new RuntimeException("Wrong return type of designable method '" + m.getName() + "'");
	            } catch (Throwable t) {
	                java.util.logging.Logger.getLogger("io.qt.internal").log(java.util.logging.Level.SEVERE, "", t);
	            }
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
            } else if(NativeLibraryManager.VERSION_MAJOR<6) {
            	try {
	                Method m = clazz.getMethod(value);
	                if (isBoolean(m.getReturnType()))
	                    return m;
	                else
	                    throw new RuntimeException("Wrong return type of scriptable method '" + m.getName() + "'");
	            } catch (Throwable t) {
	                java.util.logging.Logger.getLogger("io.qt.internal").log(java.util.logging.Level.SEVERE, "", t);
	            }
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
            } else if(NativeLibraryManager.VERSION_MAJOR<6) {
            	try {
	                Method m = clazz.getMethod(value);
	                if (isBoolean(m.getReturnType()))
	                    return m;
	                else
	                    throw new RuntimeException("Wrong return type of scriptable method '" + m.getName() + "'");
	            } catch (Throwable t) {
	                java.util.logging.Logger.getLogger("io.qt.internal").log(java.util.logging.Level.SEVERE, "", t);
	            }
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


    private static String bunchOfClassNamesInARow(Class<?> classes[], Type[] genericTypes, AnnotatedType[] annotatedTypes, boolean cppName) {
        List<SignalParameterType> typeList = Collections.emptyList();
        for (int i = 0; i < classes.length && i < genericTypes.length && i < annotatedTypes.length; i++) {
            int arrayDimension = 0;
            Class<?> type = classes[i];
            Type genericType = genericTypes[i];
            AnnotatedType annotatedType = annotatedTypes[i];
            while (type.isArray()) {
                arrayDimension++;
                type = type.getComponentType();
                if(genericType instanceof GenericArrayType) {
                    genericType = ((GenericArrayType) genericType).getGenericComponentType();
                }else {
                    genericType = type;
                }
            }
            boolean isPointer = annotatedType.isAnnotationPresent(QtPointerType.class);
            QtReferenceType referenceType = annotatedType!=null ? annotatedType.getAnnotation(QtReferenceType.class) : null;
            boolean isReference = !isPointer && referenceType!=null && !referenceType.isConst();
            SignalParameterType signalType = new SignalParameterType(classes[i], type, genericType, annotatedType, arrayDimension, isPointer, isReference);
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
    	if(type.type==null)
    		return "<unknown type>";
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
        if(cppName) {
            String _className = internalTypeName(className, type.originalType.getClassLoader());
            if(!className.isEmpty() && _className.isEmpty()) {
                _className = "JObjectWrapper";
            }
            className = _className;
        }
        return className;
    }

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
        return bunchOfClassNamesInARow(m.getParameterTypes(), m.getGenericParameterTypes(), m.getAnnotatedParameterTypes(), false);
    }

	private native static String internalTypeName(String s, ClassLoader classLoader);
    
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
    private static QPair<Object[],String[]> methodTypes(AccessibleObject accessible) {
        Class<?> returnType = null;
        Type genericReturnType = null;
        AnnotatedType annotatedReturnType = null;
        Class<?>[] parameterTypes = null;
        Type[] genericParameterTypes = null;
        AnnotatedType[] annotatedParameterTypes = null;
        if(accessible instanceof Method) {
            Method m = (Method)accessible;
            returnType = m.getReturnType();
            genericReturnType = m.getGenericReturnType();
            parameterTypes = m.getParameterTypes();
            annotatedReturnType = m.getAnnotatedReturnType();
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
            if(isValidQProperty(f)) {
            	MetaObjectTools.QPropertyTypeInfo propertyTypeInfo = getQPropertyTypeInfo(f);
            	if(propertyTypeInfo!=null) {
    	            Class<?>[] cresult = new Class[1];
    	            String[] qresult = new String[1];
	                cresult[0] = propertyTypeInfo.propertyType;
	                qresult[0] = internalTypeNameOfClass(propertyTypeInfo.propertyType, propertyTypeInfo.genericPropertyType);
	                if(propertyTypeInfo.isPointer) {
	                    if(!qresult[0].endsWith("*")) {
	                        qresult[0] += "*";
	                    }
	                }
	                if(propertyTypeInfo.isReference) {
	                    if(qresult[0].endsWith("*")) {
	                        qresult[0] = qresult[0].substring(0, qresult[0].length()-2);
	                    }
	                    if(!qresult[0].endsWith("&")) {
	                        qresult[0] += "&";
	                    }
	                }
		            return new QPair<>(cresult, qresult);
            	}
            }else if(QtJambiInternal.isSignalType(f.getType())) {
	            QtJambiInternal.ResolvedSignal resolvedSignal = QtJambiInternal.resolveSignal(f, f.getDeclaringClass());
	            Class<?>[] cresult = new Class[1+resolvedSignal.signalTypes.size()];
	            String[] qresult = new String[1+resolvedSignal.signalTypes.size()];
	            qresult[0] = "void";
	            cresult[0] = void.class;
	            for (int i = 0; i < resolvedSignal.signalTypes.size(); i++) {
	                cresult[i+1] = resolvedSignal.signalTypes.get(i).originalType;
	                qresult[i+1] = internalTypeNameOfClass(resolvedSignal.signalTypes.get(i).originalType, resolvedSignal.signalTypes.get(i).genericType);
	                if(resolvedSignal.signalTypes.get(i).isPointer) {
	                    if(!qresult[i+1].endsWith("*")) {
	                        qresult[i+1] += "*";
	                    }
	                }
	                if(resolvedSignal.signalTypes.get(i).isReference) {
	                    if(qresult[i+1].endsWith("*")) {
	                        qresult[i+1] = qresult[i+1].substring(0, qresult[i+1].length()-2);
	                    }
	                    if(!qresult[i+1].endsWith("&")) {
	                        qresult[i+1] += "&";
	                    }
	                }
	            }
	            return new QPair<>(cresult, qresult);
            }else {
	            Class<?>[] cresult = new Class[1];
	            String[] qresult = new String[1];
	            AnnotatedType annotatedType = f.getAnnotatedType();
                boolean isPointer = annotatedType!=null && annotatedType.isAnnotationPresent(QtPointerType.class);
                QtReferenceType referenceType = annotatedType!=null ? annotatedType.getAnnotation(QtReferenceType.class) : null;
                boolean isReference = !isPointer && referenceType!=null && !referenceType.isConst();
                cresult[0] = f.getType();
                qresult[0] = internalTypeNameOfClass(f.getType(), f.getGenericType());
                if(isPointer) {
                    if(!qresult[0].endsWith("*")) {
                        qresult[0] += "*";
                    }
                }
                if(isReference) {
                    if(qresult[0].endsWith("*")) {
                        qresult[0] = qresult[0].substring(0, qresult[0].length()-2);
                    }
                    if(!qresult[0].endsWith("&")) {
                        qresult[0] += "&";
                    }
                }
                return new QPair<>(cresult, qresult);            	
            }
        }
        if(returnType!=null) {
            Class<?>[] cresult = new Class[1+parameterTypes.length];
            String[] qresult = new String[1+parameterTypes.length];
            {
                boolean isPointer = annotatedReturnType!=null && annotatedReturnType.isAnnotationPresent(QtPointerType.class);
                QtReferenceType referenceType = annotatedReturnType!=null ? annotatedReturnType.getAnnotation(QtReferenceType.class) : null;
                boolean isReference = !isPointer && referenceType!=null && !referenceType.isConst();

                cresult[0] = returnType;
                qresult[0] = internalTypeNameOfClass(returnType, genericReturnType);
                if(isPointer) {
                    if(!qresult[0].endsWith("*")) {
                        qresult[0] += "*";
                    }
                }
                if(isReference) {
                    if(qresult[0].endsWith("*")) {
                        qresult[0] = qresult[0].substring(0, qresult[0].length()-2);
                    }
                    if(!qresult[0].endsWith("&")) {
                        qresult[0] += "&";
                    }
                }
            }
            for (int i = 0; i < genericParameterTypes.length; i++) {
                AnnotatedType annotatedType = annotatedParameterTypes[i];
                boolean isPointer = annotatedType!=null && annotatedType.isAnnotationPresent(QtPointerType.class);
                QtReferenceType referenceType = annotatedType!=null ? annotatedType.getAnnotation(QtReferenceType.class) : null;
                boolean isReference = !isPointer && referenceType!=null && !referenceType.isConst();
                cresult[i+1] = parameterTypes[i];
                String typeName = internalTypeNameOfClass(parameterTypes[i], genericParameterTypes[i]);
                if(isPointer) {
                    if(!typeName.endsWith("*")) {
                        typeName += "*";
                    }
                }
                if(isReference) {
                    if(typeName.endsWith("*")) {
                        typeName = typeName.substring(0, typeName.length()-2);
                    }
                    if(!typeName.endsWith("&")) {
                        typeName += "&";
                    }
                }
                qresult[i+1] = typeName;
            }
            return new QPair<>(cresult, qresult);
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
        return r.field.getName() + "(" + bunchOfClassNamesInARow(r.signalTypes, false) + ")";
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
        return bunchOfClassNamesInARow(c.getParameterTypes(), c.getGenericParameterTypes(), c.getAnnotatedParameterTypes(), false);
    }
    
    private static QtJambiSignals.AbstractSignal lookupSignal(QtSignalEmitterInterface signalEmitter, String name, Class<?>[] requiredParameters) {
		try {
			if (name == null || signalEmitter == null) {
				java.util.logging.Logger.getLogger("io.qt.internal").log(java.util.logging.Level.WARNING,
						"lookupSignal: Name or object is null");
				return null;
			}

			String parameters;
			if (requiredParameters != null) {
				return QtJambiInternal.findSignal((QtSignalEmitterInterface) signalEmitter, name, requiredParameters);
			} else {
				parameters = "";
				int idx = name.indexOf("(");
				if (idx >= 0) {
					parameters = name.substring(idx).trim();
					name = name.substring(0, idx);
				}
			}

			String[] argumentTypes = {};
			if (parameters != null && !parameters.isEmpty()) {
				if (parameters.startsWith("(") && parameters.endsWith(")")) {
					parameters = parameters.substring(1, parameters.length() - 1).trim();
					if (!parameters.isEmpty()) {
						argumentTypes = parameters.split("\\,");
						for (int i = 0; i < argumentTypes.length; ++i)
							argumentTypes[i] = argumentTypes[i].replace(" ", "");
					}
				}
			}

			Set<AbstractSignal> visitedSignals = new HashSet<>();

			for (Class<?> cls = signalEmitter.getClass(); QtSignalEmitterInterface.class
					.isAssignableFrom(cls); cls = cls.getSuperclass()) {

				try {
					Field f = cls.getDeclaredField(name);
					if (!java.lang.reflect.Modifier.isStatic(f.getModifiers())) {
						List<? extends QtJambiSignals.AbstractSignal> signals = Collections.emptyList();
						if (QtJambiSignals.AbstractSignal.class.isAssignableFrom(f.getType())) {
							signals = Collections.singletonList((QtJambiSignals.AbstractSignal) QtJambiInternal.fetchField(signalEmitter, f));
						} else if (QtJambiSignals.MultiSignal.class.isAssignableFrom(f.getType())) {
							QtJambiSignals.MultiSignal multiSignal = (QtJambiSignals.MultiSignal) QtJambiInternal.fetchField(signalEmitter, f);
							signals = multiSignal.signals();
						}
						visitedSignals.addAll(signals);
						for (QtJambiSignals.AbstractSignal signal : signals) {
							if (argumentTypes.length == 0 && signal.signalTypes().isEmpty()) {
								return signal;
							} else if (argumentTypes.length == signal.signalTypes().size()) {
								List<SignalParameterType> signalTypes = signal.signalTypes();

								boolean matches = true;
								for (int i = 0; i < signalTypes.size(); ++i) {
									Class<?> t = signalTypes.get(i).type;
									String arg = t.getName();

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
										matches = false;
										break;
									}
								}
								if (matches) {
									return signal;
								}
							}
						}
					}
				} catch (NoSuchFieldException | SecurityException t) {
				}
			}

			if (signalEmitter instanceof QObject) {
				List<AbstractSignal> listOfExtraSignal = QtJambiInternal.getListOfExtraSignal(QtJambiInternal.internalAccess.nativeId((QObject) signalEmitter));
				visitedSignals.addAll(listOfExtraSignal);
				for (AbstractSignal signal : listOfExtraSignal) {
					if (signal.name().equals(name)) {
						if (argumentTypes.length == 0 && signal.signalTypes().isEmpty()) {
							return signal;
						} else if (argumentTypes.length == signal.signalTypes().size()) {
							List<SignalParameterType> signalTypes = signal.signalTypes();

							boolean matches = true;
							for (int i = 0; i < signalTypes.size(); ++i) {
								Class<?> t = signalTypes.get(i).type;
								String arg = t.getName();

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
									matches = false;
									break;
								}
							}
							if (matches) {
								return signal;
							}
						}
					}
				}
				for (QMetaMethod method : QMetaObject.forType(signalEmitter.getClass()).methods()) {
					if (method.methodType() == QMetaMethod.MethodType.Signal) {
						AbstractSignal signal = method.toSignal((QObject) signalEmitter);
						if (signal != null && signal.name().equals(name) && !visitedSignals.contains(signal)) {
							if (argumentTypes.length == 0 && signal.signalTypes().isEmpty()) {
								return signal;
							} else if (argumentTypes.length == signal.signalTypes().size()) {
								List<SignalParameterType> signalTypes = signal.signalTypes();

								boolean matches = true;
								for (int i = 0; i < signalTypes.size(); ++i) {
									Class<?> t = signalTypes.get(i).type;
									String arg = t.getName();

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
										matches = false;
										break;
									}
								}
								if (matches) {
									return signal;
								}
							}
						}
					}
				}
			}

			return null;
		} catch (RuntimeException | Error e) {
			e.printStackTrace();
			throw e;
		} catch (Throwable e) {
			e.printStackTrace();
			throw new RuntimeException(e);
		}
	}
    
    public static String cppSignalSignature(QtSignalEmitterInterface signalEmitter, String signalName) {
        QtJambiSignals.AbstractSignal signal = lookupSignal(signalEmitter, signalName, null);
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
    
    public static String cppSignalSignature(String signal) {
        if(signal.length()>0) {
            if(signal.charAt(0)!=io.qt.internal.QtJambiInternal.SignalPrefix
                    && signal.charAt(0)!=io.qt.internal.QtJambiInternal.SlotPrefix) {
                signal = io.qt.internal.QtJambiInternal.SignalPrefix + signal;
            }
        }
        return signal;
    }
    
    public static String cppSlotSignature(String slot) {
        if(slot.length()>0) {
            if(slot.charAt(0)!=io.qt.internal.QtJambiInternal.SignalPrefix
                    && slot.charAt(0)!=io.qt.internal.QtJambiInternal.SlotPrefix) {
                slot = io.qt.internal.QtJambiInternal.SlotPrefix + slot;
            }
        }
        return slot;
    }
    
    public static String cppSignalSignature(QMetaObject.AbstractSignal signal) {
        return cppSignalSignature((QtJambiSignals.AbstractSignal)signal);
    }

    static String cppSignalSignature(QtJambiSignals.AbstractSignal signal) {
        QMetaMethod method = QMetaMethod.fromSignal((QMetaObject.AbstractSignal)signal);
        if(method!=null && method.isValid() && method.methodType()==QMetaMethod.MethodType.Signal) {
            return io.qt.internal.QtJambiInternal.SignalPrefix + method.cppMethodSignature().toString();
        }
        String signalParameters = bunchOfClassNamesInARow(signal.signalTypes(), true);
        return io.qt.internal.QtJambiInternal.SignalPrefix + signal.name() + "(" + signalParameters + ")";
    }

    private static String signalParameters(QtJambiInternal.ResolvedSignal resolvedSignal) {
        return bunchOfClassNamesInARow(resolvedSignal.signalTypes, false);
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
    
    private static Class<?> getBoxedType(Class<?> type){
    	if(type.isPrimitive()) {
    		if(int.class==type) {
    			type = Integer.class;
    		}else if(byte.class==type) {
    			type = Byte.class;
    		}else if(short.class==type) {
    			type = Short.class;
    		}else if(long.class==type) {
    			type = Long.class;
    		}else if(double.class==type) {
    			type = Double.class;
    		}else if(float.class==type) {
    			type = Float.class;
    		}else if(boolean.class==type) {
    			type = Boolean.class;
    		}else if(char.class==type) {
    			type = Character.class;
    		}
    	}
    	return type;
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
            MetaData metaData = new MetaData();
            metaData.stringData.add("Reserving the first string for QDynamicMetaObject identification.");
            List<String> intdataComments = /*new ArrayList<>();*/new AbstractList<String>() {
                @Override
                public boolean add(String e) { return false; }
                @Override
                public String get(int index) {return null;}
                @Override
                public int size() {return 0; }
            };
            final String classname = clazz.getName().replace(".", "::");
            metaData.stringData.add(classname);
            
            Hashtable<String,String> classInfos = new Hashtable<String, String>();
            
            for(QtClassInfo info : clazz.getAnnotationsByType(QtClassInfo.class)) {
                classInfos.put(info.key(), info.value());
            }
            
            for(Map.Entry<String,String> entry : classInfos.entrySet()){
                metaData.stringData.add(entry.getKey());
                metaData.stringData.add(entry.getValue());
            }
            
            List<Method> slots = new ArrayList<Method>();
            List<Method> methods = new ArrayList<Method>();
            
            List<String> originalSignatures = new ArrayList<String>();

            TreeMap<String, Method> propertyReaders = new TreeMap<>();
            TreeMap<String, List<Method>> propertyWriters = new TreeMap<>();
            TreeMap<String, Object> propertyDesignableResolvers = new TreeMap<>();
            TreeMap<String, Object> propertyScriptableResolvers = new TreeMap<>();
            TreeMap<String, Object> propertyEditableResolvers = new TreeMap<>();
            TreeMap<String, Object> propertyStoredResolvers = new TreeMap<>();
            TreeMap<String, Object> propertyUserResolvers = new TreeMap<>();
            TreeMap<String, Boolean> propertyRequiredResolvers = new TreeMap<>();
            TreeMap<String, Boolean> propertyConstantResolvers = new TreeMap<>();
            TreeMap<String, Boolean> propertyFinalResolvers = new TreeMap<>();
                                                                                         
            TreeMap<String, Method> propertyResetters = new TreeMap<>();
            TreeMap<String, Field> propertyNotifies = new TreeMap<>();
            TreeMap<String, Method> propertyBindables = new TreeMap<>();
            TreeMap<String, Field> propertyMembers = new TreeMap<>();
            TreeMap<String, Field> propertyQPropertyFields = new TreeMap<>();

            // First we get all enums actually declared in the class
            Hashtable<String, Class<?>> enums = new Hashtable<String, Class<?>>();
            queryEnums(clazz, enums);

            List<QtJambiInternal.ResolvedSignal> resolvedSignals = new ArrayList<>();
//            cl.getEnclosingClass() != QInstanceMemberSignals.class
            boolean isQObject = QObject.class.isAssignableFrom(clazz);
//            if(QObject.class.isAssignableFrom(clazz)) 
            {
	            Field declaredFields[] = clazz.getDeclaredFields();
	            for (Field declaredField : declaredFields) {
	            	if(QtJambiInternal.isSignalType(declaredField.getType())) {
	            		if (!Modifier.isStatic(declaredField.getModifiers())) {
	            			if(!isQObject && declaredField.getType().getEnclosingClass() == QObject.class) {
	            				throw new QSignalDeclarationException(String.format("Declaration error at signal %1$s.%2$s: do not use QObject signals within non-QObjects.", clazz.getSimpleName(), declaredField.getName()));
	            			}
		                    // If we can't convert all the types we don't list the signal
		                    QtJambiInternal.ResolvedSignal resolvedSignal = QtJambiInternal.resolveSignal(declaredField, declaredField.getDeclaringClass());
		                    String signalParameters = signalParameters(resolvedSignal);
		                    if (signalParameters.length() == 0 || internalTypeNameOfSignal(signalParameters, clazz.getClassLoader()).length() != 0) {
		                    	if (!Modifier.isFinal(declaredField.getModifiers())) {
		                    		if(!Boolean.getBoolean("qtjambi.allow-nonfinal-signals") && !Boolean.getBoolean("io.qt.allow-nonfinal-signals")) {
		                    			java.util.logging.Logger.getLogger("io.qt.internal").severe(String.format("Missing modifier 'final' at signal %1$s.%2$s. Specify JVM argument -Dqtjambi.allow-nonfinal-signals=true to disable this error.", clazz.getSimpleName(), declaredField.getName()));
		                    			throw new QSignalDeclarationException(String.format("Missing modifier 'final' at signal %1$s.%2$s.", clazz.getSimpleName(), declaredField.getName()));
		                    		}
		                    	}
		                        metaData.signalFields.add(declaredField);
		                        resolvedSignals.add(resolvedSignal);
		                        originalSignatures.add(methodSignature(resolvedSignal));
		                        
		                        metaData.stringData.add(resolvedSignal.field.getName());
		                        for (int i = 1; i <= resolvedSignal.signalTypes.size(); i++) {
		                            QtJambiSignals.SignalParameterType signalType = resolvedSignal.signalTypes.get(i-1);
		                            String typeName;
		                            if(signalType.arrayDimension==0){
		                                typeName = internalTypeNameOfClass(signalType.type, signalType.genericType);
		                            }else{
		                                typeName = internalTypeNameOfClass(signalType.originalType, signalType.originalType);
		                            }
		                            if(signalType.isPointer) {
		                                if(!typeName.endsWith("*")) {
		                                    typeName += "*";
		                                }
		                            }
		                            if(signalType.isReference) {
		                                if(typeName.endsWith("*")) {
		                                    typeName = typeName.substring(0, typeName.length()-2);
		                                }
		                                if(!typeName.endsWith("&")) {
		                                    typeName += "&";
		                                }
		                            }
		                            QMetaType.Type type = metaType(typeName);
		                            if(type==QMetaType.Type.UnknownType || type==QMetaType.Type.User){
		                                metaData.stringData.add(typeName);
		                            }
		                            metaData.stringData.add("arg__"+i);
		                        }
		                        metaData.stringData.add(""); // for the method tag
		                    	
			                    // Rules for resetters:
			                    // 1. Zero or one argument
			                    if(resolvedSignal.signalTypes.size() <= 1){
			                        PropertyAnnotation notify = PropertyAnnotation.notifyAnnotation(declaredField);
			
			                        if (notify != null) {
			                            propertyNotifies.put(notify.name(), declaredField);
			                        }
			                    }
		                    }
	                	}else {
	                		throw new QSignalDeclarationException(String.format("Modifier 'static' not allowed for signal %1$s.%2$s. Use QStaticMemberSignals instead to declare a static signal.", clazz.getSimpleName(), declaredField.getName()));
	                	}
	            	}else{
	            		PropertyAnnotation member = PropertyAnnotation.memberAnnotation(declaredField);
	            		if(member!=null) {
	            			if(member.enabled()) {
	        					propertyMembers.put(member.name(), declaredField);
	            			}
	            		}else if(isQObject && isValidQProperty(declaredField)) {
	                		propertyQPropertyFields.put(declaredField.getName(), declaredField);
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
                            if(parameterType==PrivateConstructorAccess.type()) {
                                continue cloop;
                            }
                        }
                        metaData.constructors.add(constructor);
                        metaData.hasStaticMembers = true;
                        originalSignatures.add(constructorSignature(constructor));
                        Parameter[] parameters = constructor.getParameters();
                        Type[] genericParameterTypes = constructor.getGenericParameterTypes();
                        for (int i = 0; i < parameterTypes.length; i++) {
                            String typeName = internalTypeNameOfClass(parameterTypes[i], genericParameterTypes[i]);
                            QMetaType.Type type = metaType(typeName);
                            if(type==QMetaType.Type.UnknownType || type==QMetaType.Type.User){
                                metaData.stringData.add(typeName);
                            }
                            if(parameters[i].isNamePresent()) {
                                metaData.stringData.add(parameters[i].getName());
                            }else {
                                metaData.stringData.add("arg__"+(i+1));
                            }
                        }
                        metaData.stringData.add("");
                    }
                }
            }
            
            List<Method> possibleBindables = Collections.emptyList();
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
                    if(returnTypeName.startsWith("QQmlListProperty<") && QtJambiInternal.findMetaType(returnTypeName)==QMetaType.Type.UnknownType.value()) {
                        registerQmlListProperty(returnTypeName);
                    }
                    QtReferenceType referenceType = declaredMethod.getAnnotatedReturnType().getAnnotation(QtReferenceType.class);
                    if(referenceType==null)
                    	declaredMethod.getAnnotation(QtReferenceType.class);
                    if(declaredMethod.getAnnotatedReturnType().isAnnotationPresent(QtPointerType.class)) {
                        if(!returnTypeName.endsWith("*")) {
                            returnTypeName += "*";
                        }
                    }else if(referenceType!=null && !referenceType.isConst()) {
                        if(returnTypeName.endsWith("*")) {
                            returnTypeName = returnTypeName.substring(0, returnTypeName.length()-2);
                        }
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
                            metaData.stringData.add(declaredMethod.getName());
                            metaData.stringData.add(returnTypeName);
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
                                }
                                QtReferenceType _referenceType = annotatedParameterTypes[i-1].getAnnotation(QtReferenceType.class);
                                if(_referenceType!=null && !_referenceType.isConst()) {
                                    if(typeName.endsWith("*")) {
                                        typeName = typeName.substring(0, typeName.length()-2);
                                    }
                                    if(!typeName.endsWith("&")) {
                                        typeName += "&";
                                    }
                                }
                                QMetaType.Type type = metaType(typeName);
                                if(type==QMetaType.Type.UnknownType || type==QMetaType.Type.User){
                                    metaData.stringData.add(typeName);
                                }
                                if(parameters[i-1].isNamePresent()) {
                                    metaData.stringData.add(parameters[i-1].getName());
                                }else {
                                    metaData.stringData.add("arg__"+i);
                                }
                            }
                            metaData.stringData.add(""); // for the method tag
                        }
                    }
                }
                
                final String declaredMethodName = declaredMethod.getName();

                // Rules for readers:
                // 1. Zero arguments
                // 2. Return something other than void
                // 3. We can convert the type
                PropertyAnnotation reader = PropertyAnnotation.readerAnnotation(declaredMethod);
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
                            int type = registerMetaType(returnType);
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
                        metaData.stringData.add(name);
                    }
                }

                // Rules for writers:
                // 1. Takes exactly one argument
                // 2. Return void
                // 3. We can convert the type
                PropertyAnnotation writer = PropertyAnnotation.writerAnnotation(declaredMethod);
                {
                    if ( writer != null 
                            && writer.enabled()
                            && isValidSetter(declaredMethod)) {
                        propertyWriters.computeIfAbsent(writer.name(), name->new ArrayList<>()).add(declaredMethod);
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
                            reader = PropertyAnnotation.readerAnnotation(readerMethod);
                            if (reader == null) {
                                propertyReaders.put(propertyName, readerMethod);
                                propertyWriters.computeIfAbsent(propertyName, name->new ArrayList<>()).add(declaredMethod);

                                propertyDesignableResolvers.put(propertyName, isDesignable(readerMethod, clazz));
                                propertyScriptableResolvers.put(propertyName, isScriptable(readerMethod, clazz));
                                propertyUserResolvers.put(propertyName, isUser(readerMethod, clazz));
                                propertyRequiredResolvers.put(propertyName, isRequired(readerMethod, clazz));
                            }
                        }
                    }
                }
                
                // Rules for notifys:
                // 1. No arguments
                // 2. Return void
                {
                    PropertyAnnotation resetter = PropertyAnnotation.resetterAnnotation(declaredMethod);
                    if (resetter != null
                        && declaredMethod.getParameterCount() == 0
                        && declaredMethod.getReturnType() == void.class) {
                        propertyResetters.put(resetter.name(), declaredMethod);
                    }
                }
                
                if(isValidBindable(declaredMethod)) {
	                PropertyAnnotation bindables = PropertyAnnotation.bindableAnnotation(declaredMethod);
                    if (bindables != null) {
                        propertyBindables.put(bindables.name(), declaredMethod);
                    }else{
                    	if(possibleBindables.isEmpty())
                    		possibleBindables = new ArrayList<>();
                    	possibleBindables.add(declaredMethod);
                    }
                }
            }
            
            for(Method possibleBindable : possibleBindables) {
            	String name = possibleBindable.getName();
            	if(name.startsWith("bindable") && name.length() > 8) {
            		name = PropertyAnnotation.removeAndLowercaseFirst(name, 8);
            		if (propertyReaders.containsKey(name)) {
            			propertyBindables.put(name, possibleBindable);
            		}
            	}
            }
            
            {
	            TreeMap<String, Field> _propertyQPropertyFields = new TreeMap<>(propertyQPropertyFields);
	            propertyQPropertyFields.clear();
	            for(Map.Entry<String, Field> entry : _propertyQPropertyFields.entrySet()) {
	            	String name = entry.getKey();
	        		if(name.endsWith("Prop")) {
	        			name = name.substring(0, name.length()-4);
	        		}else if(name.endsWith("Property")) {
	        			name = name.substring(0, name.length()-8);
	        		}
	            	if(!propertyReaders.containsKey(entry.getKey()) 
	            			&& !propertyReaders.containsKey(name)
	            			&& !Modifier.isStatic(entry.getValue().getModifiers())
	            			&& Modifier.isFinal(entry.getValue().getModifiers())
	            			&& Modifier.isPublic(entry.getValue().getModifiers())) {
	            		propertyReaders.put(name, null);
	        			propertyQPropertyFields.put(name, entry.getValue());
	            	}
	            }
            }
            for(String prop : propertyMembers.keySet()) {
            	if(!propertyReaders.containsKey(prop)) {
            		propertyReaders.put(prop, null);
            	}
            }
            
            for(String property : propertyReaders.keySet()) {
            	if(propertyNotifies.get(property)==null) {
            		for(QtJambiInternal.ResolvedSignal resolvedSignal : resolvedSignals) {
            			if(resolvedSignal.field.getName().equals(property+"Changed")) {
                			if(resolvedSignal.signalTypes.isEmpty()) {
                				propertyNotifies.put(property, resolvedSignal.field);
                			}else {
                				if(propertyReaders.get(property)!=null) {
                					if(resolvedSignal.signalTypes.get(0).originalType.isAssignableFrom(getBoxedType(propertyReaders.get(property).getReturnType()))) {
                        				propertyNotifies.put(property, resolvedSignal.field);
                        			}
                    			}else if(propertyMembers.get(property)!=null) {
                    				Field field = propertyMembers.get(property);
                    				if(isValidQProperty(field)) {
                    					MetaObjectTools.QPropertyTypeInfo pinfo = getQPropertyTypeInfo(field);
                    					if(resolvedSignal.signalTypes.get(0).originalType.isAssignableFrom(getBoxedType(pinfo.propertyType))) {
                            				propertyNotifies.put(property, resolvedSignal.field);
                            			}
                    				}else if(resolvedSignal.signalTypes.get(0).originalType.isAssignableFrom(getBoxedType(field.getType()))) {
                        				propertyNotifies.put(property, resolvedSignal.field);
                        			}
                    			}else if(propertyQPropertyFields.get(property)!=null) {
                    				MetaObjectTools.QPropertyTypeInfo pinfo = getQPropertyTypeInfo(propertyQPropertyFields.get(property));
                					if(resolvedSignal.signalTypes.get(0).originalType.isAssignableFrom(getBoxedType(pinfo.propertyType))) {
                        				propertyNotifies.put(property, resolvedSignal.field);
                        			}
                    			}
                			}
                			break;
                		}
            		}
            	}
            }
            
            metaData.methods.addAll(slots);
            metaData.methods.addAll(methods);
            
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
                metaData.metaData.add(resolveMetaDataRevision());		intdataComments.add("revision");
                // classname
                metaData.metaData.add(metaData.stringData.indexOf(classname));		intdataComments.add("className");
                // classinfo
                metaData.metaData.add(classInfos.size());		intdataComments.add("classInfoCount");
                metaData.metaData.add(classInfos.isEmpty() ? 0 : MO_HEADER_LEN);		intdataComments.add("classInfoData");
                
                // methods
                int methodCount = metaData.signalFields.size() + slots.size() + methods.size();
                metaData.metaData.add(methodCount);		intdataComments.add("methodCount");
                final int METHOD_METADATA_INDEX = metaData.metaData.size();
                metaData.metaData.add(0);		intdataComments.add("methodData");
                
                // properties
                metaData.metaData.add(propertyReaders.size());		intdataComments.add("propertyCount");
                final int PROPERTY_METADATA_INDEX = metaData.metaData.size();
                metaData.metaData.add(0);		intdataComments.add("propertyData");
                
                // enums/sets
                metaData.metaData.add(enums.size());		intdataComments.add("enumeratorCount");
                final int ENUM_METADATA_INDEX = metaData.metaData.size();
                metaData.metaData.add(0);		intdataComments.add("enumeratorData");
                
                // constructors
                metaData.metaData.add(!metaData.constructors.isEmpty() ? metaData.constructors.size() : 0);		intdataComments.add("constructorCount");
                final int CONSTRUCTOR_METADATA_INDEX = metaData.metaData.size();
                metaData.metaData.add(0);		intdataComments.add("constructorData");
                
                // flags
                flagsIndex = metaData.metaData.size();
                metaData.metaData.add(0);		intdataComments.add("flags");
                
                // signalCount
                metaData.metaData.add(metaData.signalFields.size());		intdataComments.add("signalCount");
                
                //
                // Build classinfo array
                //
                for(Map.Entry<String,String> entry : classInfos.entrySet()){
                    // classinfo: key, value
                    metaData.metaData.add(metaData.stringData.indexOf(entry.getKey()));		intdataComments.add("classinfo: key");
                    metaData.metaData.add(metaData.stringData.indexOf(entry.getValue()));		intdataComments.add("classinfo: value");
                }
                
                HashMap<Object,Integer> paramIndexOfMethods = new HashMap<Object,Integer>();
                HashMap<Field,Integer> signalIndexes = new HashMap<>();
                
                //
                // Build signals array first, otherwise the signal indices would be wrong
                //
                if(metaData.signalFields.size() + metaData.methods.size() > 0){

                    metaData.metaData.set(METHOD_METADATA_INDEX, metaData.metaData.size());
                    
                    for (int i = 0; i < metaData.signalFields.size(); ++i) {
                        Field signalField = metaData.signalFields.get(i);
                        signalIndexes.put(signalField, i);
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
                        
                        // signals: name, argc, parameters, tag, flags, initial metatype offsets
                        metaData.metaData.add(metaData.stringData.indexOf(signalField.getName()));		intdataComments.add("signal["+i+"]: name");
                        metaData.metaData.add(argc);		intdataComments.add("signal["+i+"]: argc");
                        paramIndexOfMethods.put(resolvedSignal, metaData.metaData.size());
                        metaData.metaData.add(0);		intdataComments.add("signal["+i+"]: parameters");
                        metaData.metaData.add(metaData.stringData.indexOf(""));		intdataComments.add("signal["+i+"]: tag");
                        metaData.metaData.add(flags.value());		intdataComments.add("signal["+i+"]: flags");
                        if(NativeLibraryManager.VERSION_MAJOR>5) {
                            metaData.metaData.add(0);		intdataComments.add("signal["+i+"]: initial metatype offsets");
                        }
                    }
                    
                    //
                    // Build slots array
                    // Build method array
                    //
                    for (int i = 0; i < metaData.methods.size(); i++) {
                        Method method = metaData.methods.get(i);
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
                        
                        // slots: name, argc, parameters, tag, flags, initial metatype offsets
                        metaData.metaData.add(metaData.stringData.indexOf(method.getName()));
                        intdataComments.add("slot["+i+"]: name");
                        metaData.metaData.add(argc);
                        intdataComments.add("slot["+i+"]: argc");
                        paramIndexOfMethods.put(method, metaData.metaData.size());
                        metaData.metaData.add(0);
                        intdataComments.add("slot["+i+"]: parameters");
                        metaData.metaData.add(metaData.stringData.indexOf(""));
                        intdataComments.add("slot["+i+"]: tag");
                        metaData.metaData.add(flags.value());
                        intdataComments.add("slot["+i+"]: flags");
                        if(NativeLibraryManager.VERSION_MAJOR>5) {
                            metaData.metaData.add(0);		intdataComments.add("slot["+i+"]: initial metatype offsets");
                        }
                    }
                }
                
                //
                // Build method parameters array
                //
                for(int i=0; i<propertyReaders.size(); ++i) {
                    metaData.metaTypes.add(0);
                }
                
                for (int i = 0; i < metaData.signalFields.size(); ++i) {
                    QtJambiInternal.ResolvedSignal resolvedSignal = resolvedSignals.get(i);
                    // signals: parameters
                    int METHOD_PARAMETER_INDEX = paramIndexOfMethods.get(resolvedSignal);
                    metaData.metaData.set(METHOD_PARAMETER_INDEX, metaData.metaData.size());
                    if(NativeLibraryManager.VERSION_MAJOR>5) {
                        metaData.metaData.set(METHOD_PARAMETER_INDEX+3, metaData.metaTypes.size());
                    }
                    metaData.metaData.add(QMetaType.Type.Void.value());		intdataComments.add("signal["+i+"].returnType");
                    metaData.metaTypes.add(QMetaType.Type.Void.value());
                    for (int j = 0; j < resolvedSignal.signalTypes.size(); j++) {
                        QtJambiSignals.SignalParameterType signalType = resolvedSignal.signalTypes.get(j);
                        String typeName;
                        if(signalType.arrayDimension==0){
                            typeName = internalTypeNameOfClass(signalType.type, signalType.genericType);
                        }else{
                            typeName = internalTypeNameOfClass(signalType.originalType, signalType.originalType);
                        }
                        if(signalType.isPointer) {
                            if(!typeName.endsWith("*")) {
                                typeName += "*";
                            }
                        }
                        if(signalType.isReference) {
                            if(typeName.endsWith("*")) {
                                typeName = typeName.substring(0, typeName.length()-2);
                            }
                            if(!typeName.endsWith("&")) {
                                typeName += "&";
                            }
                        }
                        QMetaType.Type type = metaType(typeName);
                        if(type==QMetaType.Type.UnknownType || type==QMetaType.Type.User){
                            int t = QtJambiInternal.findMetaType(typeName);
                            if(t==QMetaType.Type.UnknownType.value() || !(signalType.genericType instanceof Class || new QMetaType(t).name().toString().equals(typeName))) {
                                t = registerMetaType(signalType.type, signalType.genericType, signalType.annotatedType, signalType.isPointer, signalType.isReference);
                            }
                            if(t!=QMetaType.Type.UnknownType.value())
                                typeName = new QMetaType(t).name().toString();
                            if(!metaData.stringData.contains(typeName)){
                                metaData.stringData.add(typeName);
                            }
                            metaData.metaData.add(0x80000000 | metaData.stringData.indexOf(typeName));
                            metaData.metaTypes.add(t);
                        }else{
                            metaData.metaData.add(type.value());
                            metaData.metaTypes.add(type.value());
                        }
                        intdataComments.add("signal["+i+"]: parameter["+j+"].arg");
                    }
                    for (int j = 0; j < resolvedSignal.signalTypes.size(); j++) {
                        metaData.metaData.add(metaData.stringData.indexOf("arg__"+(j+1)));		intdataComments.add("signal["+i+"]: parameter["+j+"].argName");
                    }
                }
                
                //
                // Build constructors array
                //
                
                if(!metaData.constructors.isEmpty()){
                    metaData.metaData.set(CONSTRUCTOR_METADATA_INDEX, metaData.metaData.size());
                    for (int i = 0; i < metaData.constructors.size(); i++) {
                        Constructor<?> constructor = metaData.constructors.get(i);
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
                        metaData.stringData.add(className);
                        metaData.metaData.add(metaData.stringData.indexOf(className));		intdataComments.add("constructor["+i+"]: name");
                        metaData.metaData.add(argc);		intdataComments.add("constructor["+i+"]: argc");
                        paramIndexOfMethods.put(constructor, metaData.metaData.size());
                        metaData.metaData.add(0);		intdataComments.add("constructor["+i+"]: parameters");
                        metaData.metaData.add(metaData.stringData.indexOf(""));		intdataComments.add("constructor["+i+"]: tag");
                        metaData.metaData.add(flags.value());		intdataComments.add("constructor["+i+"]: flags");
                        if(NativeLibraryManager.VERSION_MAJOR>5) {
                            metaData.metaData.add(0);		intdataComments.add("slot["+i+"]: initial metatype offsets");
                        }
                    }
                }
                
                for (int i = 0; i < metaData.methods.size(); i++) {
                    Method method = metaData.methods.get(i);
                    // slot/method: parameters
                    int METHOD_PARAMETER_INDEX = paramIndexOfMethods.get(method);
                    metaData.metaData.set(METHOD_PARAMETER_INDEX, metaData.metaData.size());
                    if(NativeLibraryManager.VERSION_MAJOR>5) {
                        metaData.metaData.set(METHOD_PARAMETER_INDEX+3, metaData.metaTypes.size());
                    }
                    String typeName = internalTypeNameOfClass(method.getReturnType(), method.getGenericReturnType());
                    if(method.getAnnotatedReturnType().isAnnotationPresent(QtPointerType.class)) {
                        if(!typeName.endsWith("*")) {
                            typeName += "*";
                        }
                    }
                    QtReferenceType referenceType = method.getAnnotatedReturnType().getAnnotation(QtReferenceType.class);
                    if(referenceType==null)
                    	referenceType = method.getAnnotation(QtReferenceType.class);
                    if(referenceType!=null && !referenceType.isConst()) {
                        if(typeName.endsWith("*")) {
                            typeName = typeName.substring(0, typeName.length()-2);
                        }
                        if(!typeName.endsWith("&")) {
                            typeName += "&";
                        }
                    }
                    QMetaType.Type type = metaType(typeName);
                    if(type==QMetaType.Type.UnknownType || type==QMetaType.Type.User){
                        int t = QtJambiInternal.findMetaType(typeName);
                        if(t==QMetaType.Type.UnknownType.value() || !(method.getGenericReturnType() instanceof Class || new QMetaType(t).name().toString().equals(typeName))) {
                        	referenceType = method.getAnnotation(QtReferenceType.class);
                            t = registerMetaType(
                                    method.getReturnType(), 
                                    method.getGenericReturnType(), 
                                    method.getAnnotatedReturnType(),
                                    method.isAnnotationPresent(QtPointerType.class),
                                    referenceType!=null && !referenceType.isConst());
                        }
                        if(t!=QMetaType.Type.UnknownType.value())
                            typeName = new QMetaType(t).name().toString();
                        if(!metaData.stringData.contains(typeName)){
                            metaData.stringData.add(typeName);
                        }
                        metaData.metaData.add(0x80000000 | metaData.stringData.indexOf(typeName));
                        metaData.metaTypes.add(t);
                    }else{
                        metaData.metaData.add(type.value());
                        metaData.metaTypes.add(type.value());
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
                        }
                        referenceType = annotatedParameterTypes[j].getAnnotation(QtReferenceType.class);
                        if(referenceType!=null && !referenceType.isConst()) {
                            if(typeName.endsWith("*")) {
                                typeName = typeName.substring(0, typeName.length()-2);
                            }
                            if(!typeName.endsWith("&")) {
                                typeName += "&";
                            }
                        }
                        type = metaType(typeName);
                        if(type==QMetaType.Type.UnknownType || type==QMetaType.Type.User){
                            int t = QtJambiInternal.findMetaType(typeName);
                            if(t==QMetaType.Type.UnknownType.value() || !(genericParameterTypes[j] instanceof Class || new QMetaType(t).name().toString().equals(typeName))) {
                                t = registerMetaType(parameterTypes[j], 
                                        genericParameterTypes[j],
                                        annotatedParameterTypes[j],
                                        false,
                                        false);
                            }
                            if(t!=QMetaType.Type.UnknownType.value())
                                typeName = new QMetaType(t).name().toString();
                            if(!metaData.stringData.contains(typeName)){
                                metaData.stringData.add(typeName);
                            }
                            metaData.metaData.add(0x80000000 | metaData.stringData.indexOf(typeName));
                            metaData.metaTypes.add(t);
                        }else{
                            metaData.metaData.add(type.value());
                            metaData.metaTypes.add(type.value());
                        }
                        intdataComments.add("slot["+i+"]: parameter["+j+"].arg");
                    }
                    for (int j = 0; j < parameterTypes.length; j++) {
                        if(parameters[j].isNamePresent()) {
                            metaData.metaData.add(metaData.stringData.indexOf(parameters[j].getName()));
                        }else {
                            metaData.metaData.add(metaData.stringData.indexOf("arg__"+(j+1)));
                        }
                        intdataComments.add("slot["+i+"]: parameter["+j+"].argName");
                    }
                }
                
                for (int i = 0; i < metaData.constructors.size(); i++) {
                    Constructor<?> constructor = metaData.constructors.get(i);
                    // constructors: parameters
                    int METHOD_PARAMETER_INDEX = paramIndexOfMethods.get(constructor);
                    metaData.metaData.set(METHOD_PARAMETER_INDEX, metaData.metaData.size());
                    if(NativeLibraryManager.VERSION_MAJOR>5) {
                        metaData.metaData.set(METHOD_PARAMETER_INDEX+3, metaData.metaTypes.size());
                    }
//				paramIndexOfMethods.put(constructor, metaData.metaData.size());
                    metaData.metaData.add(0x80000000 | metaData.stringData.indexOf(""));
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
                        }
                        QtReferenceType referenceType = annotatedParameterTypes[j].getAnnotation(QtReferenceType.class);
                        if(referenceType!=null && !referenceType.isConst()) {
                            if(typeName.endsWith("*")) {
                                typeName = typeName.substring(0, typeName.length()-2);
                            }
                            if(!typeName.endsWith("&")) {
                                typeName += "&";
                            }
                        }
                        QMetaType.Type type = metaType(typeName);
                        if(type==QMetaType.Type.UnknownType || type==QMetaType.Type.User){
                            int t = QtJambiInternal.findMetaType(typeName);
                            if(t==QMetaType.Type.UnknownType.value() || !(genericParameterTypes[j] instanceof Class || new QMetaType(t).name().toString().equals(typeName))) {
                                t = registerMetaType(parameterTypes[j], 
                                        genericParameterTypes[j],
                                        annotatedParameterTypes[j],
                                        false,
                                        false);
                            }
                            if(t!=QMetaType.Type.UnknownType.value())
                                typeName = new QMetaType(t).name().toString();
                            if(!metaData.stringData.contains(typeName)){
                                metaData.stringData.add(typeName);
                            }
                            metaData.metaData.add(0x80000000 | metaData.stringData.indexOf(typeName));
                            metaData.metaTypes.add(t);
                        }else{
                            metaData.metaData.add(type.value());
                            metaData.metaTypes.add(type.value());
                        }
                        intdataComments.add("constructor["+i+"]: parameter["+(j)+"].arg");
                    }
                    for (int j = 0; j < parameterTypes.length; j++) {
                        if(parameters[j].isNamePresent()) {
                            metaData.metaData.add(metaData.stringData.indexOf(parameters[j].getName()));
                        }else {
                            metaData.metaData.add(metaData.stringData.indexOf("arg__"+(j+1)));
                        }
                        intdataComments.add("constructor["+i+"]: parameter["+(j)+"].argName");
                    }
                }
                
                //
                // Build property array
                //
                int metaObjectFlags = 0;
                
                if(!propertyReaders.isEmpty()){
                    if(!QObject.class.isAssignableFrom(clazz)) {
                        metaObjectFlags |= PropertyAccessInStaticMetaCall.value();
                    }
                    metaData.metaData.set(PROPERTY_METADATA_INDEX, metaData.metaData.size());
                    int i=0;
                    for (String propertyName : propertyReaders.keySet()) {
                        Method reader = propertyReaders.get(propertyName);
                        Field qPropertyField = propertyQPropertyFields.get(propertyName);
                        Field propertyMemberField = propertyMembers.get(propertyName);
                        Method writer = null;
                        List<Method> writers = propertyWriters.get(propertyName);
                        Class<?> propertyType;
                        Type genericPropertyType;
                        AnnotatedType annotatedPropertyType;
                        boolean isPointer;
                        boolean isReference;
                        boolean isMemberWritable = false;
                        boolean isMemberReadable = false;
                        if(reader!=null) {
                        	propertyType = reader.getReturnType();
                        	genericPropertyType = reader.getGenericReturnType();
                        	annotatedPropertyType = reader.getAnnotatedReturnType();
                        	isPointer = reader.isAnnotationPresent(QtPointerType.class)
                                    || reader.getAnnotatedReturnType().isAnnotationPresent(QtPointerType.class);
                        	QtReferenceType referenceType = reader.getAnnotation(QtReferenceType.class);
                        	if(referenceType==null)
                        		referenceType = reader.getAnnotatedReturnType().getAnnotation(QtReferenceType.class);
                        	isReference = referenceType!=null && !referenceType.isConst();
                        }else if(propertyMemberField!=null) {
                        	if(isValidQProperty(propertyMemberField)) {
                        		qPropertyField = propertyMemberField;
                        		propertyMemberField = null;
                        		QPropertyTypeInfo info = getQPropertyTypeInfo(qPropertyField);
                            	if(info==null)
                            		continue;
                            	propertyType = info.propertyType;
                            	genericPropertyType = info.genericPropertyType;
                            	annotatedPropertyType = info.annotatedPropertyType;
                            	isPointer = info.isPointer;
                            	isReference = info.isReference;
                            	isMemberWritable = info.isWritable;
                            	isMemberReadable = true;
                        	}else {
                            	propertyType = propertyMemberField.getType();
                            	genericPropertyType = propertyMemberField.getGenericType();
                            	annotatedPropertyType = propertyMemberField.getAnnotatedType();
                            	isPointer = propertyMemberField.isAnnotationPresent(QtPointerType.class)
                                        || propertyMemberField.getAnnotatedType().isAnnotationPresent(QtPointerType.class);
                            	QtReferenceType referenceType = propertyMemberField.getAnnotation(QtReferenceType.class);
                            	if(referenceType==null)
                            		referenceType = propertyMemberField.getAnnotatedType().getAnnotation(QtReferenceType.class);
                            	isReference = referenceType!=null && !referenceType.isConst();
                            	isMemberWritable = !Modifier.isFinal(propertyMemberField.getModifiers());
                            	isMemberReadable = true;
                        	}
                        }else if(qPropertyField!=null){
                        	QPropertyTypeInfo info = getQPropertyTypeInfo(qPropertyField);
                        	if(info==null)
                        		continue;
                        	propertyType = info.propertyType;
                        	genericPropertyType = info.genericPropertyType;
                        	annotatedPropertyType = info.annotatedPropertyType;
                        	isPointer = info.isPointer;
                        	isReference = info.isReference;
                        	isMemberWritable = info.isWritable;
                        	isMemberReadable = true;
                        }else {
                        	propertyType = null;
                        	genericPropertyType = null;
                        	annotatedPropertyType = null;
                        	isPointer = false;
                        	isReference = false;
                        }
                        if(propertyType!=null) {
	                        if(writers!=null) {
	                        	for (Method w : writers) {
									if(w.getParameterCount()==1 && propertyType.isAssignableFrom(w.getParameterTypes()[0])) {
										writer = w;
										break;
									}
								}
	                        	if (!writers.isEmpty() && writer == null) {
	                        		writer = writers.get(0);
	                        	}
	                            if (writer != null && !propertyType.isAssignableFrom(writer.getParameterTypes()[0])) {
	                                Logger.getLogger("io.qt.internal").warning("Writer for property '"
	                                        + clazz.getName() + "::" + propertyName + "' takes a type (" + writer.getParameterTypes()[0].getName() + ") which is incompatible with reader's return type (" + propertyType.getName() + ").");
	                                writer = null;
	                            }
	                        }
                        }else {
                        	if(writers!=null && !writers.isEmpty()) {
                        		writer = writers.get(0);
                        		if(writer!=null) {
                        			propertyType = writer.getParameterTypes()[0];
                        			genericPropertyType = writer.getGenericParameterTypes()[0];
                                	isPointer = writer.getAnnotatedParameterTypes()[0].isAnnotationPresent(QtPointerType.class);
                                	QtReferenceType referenceType = writer.getAnnotatedParameterTypes()[0].getAnnotation(QtReferenceType.class);
                                	isReference = referenceType!=null && !referenceType.isConst();
                        		}else {
                        			continue;
                        		}
                        	}
                        }
                        Method resetter = propertyResetters.get(propertyName);
                        Field notify = propertyNotifies.get(propertyName);
                        Method bindable = propertyBindables.get(propertyName);
                        Object designableVariant = propertyDesignableResolvers.get(propertyName);
                        Object scriptableVariant = propertyScriptableResolvers.get(propertyName);
                        Object editableVariant = propertyEditableResolvers.get(propertyName);
                        Object storedVariant = propertyStoredResolvers.get(propertyName);
                        Object userVariant = propertyUserResolvers.get(propertyName);
                        Boolean requiredVariant = propertyRequiredResolvers.get(propertyName);
                        Boolean constantVariant = propertyConstantResolvers.get(propertyName);
                        Boolean finalVariant = propertyFinalResolvers.get(propertyName);

                        PropertyAttributes flags = PropertyFlags.Invalid.asFlags();
                        // Type (need to special case flags and enums)
                        String typeName = internalTypeNameOfClass(propertyType, genericPropertyType);
                        if(isPointer) {
                            if(!typeName.endsWith("*")) {
                                typeName += "*";
                            }
                        }
                        if(isReference) {
                            if(typeName.endsWith("*")) {
                                typeName = typeName.substring(0, typeName.length()-2);
                            }
                            if(!typeName.endsWith("&")) {
                                typeName += "&";
                            }
                        }
                        
                        if (!isBuiltinType(typeName))
                            flags.set(PropertyFlags.EnumOrFlag);
                        if (writer!=null){
                            flags.set(PropertyFlags.Writable);
                            String s = "set";
                            s += propertyName.toUpperCase().charAt(0);
                            s += propertyName.substring(1);
                            if (s.equals(writer.getName()))
                                flags.set(PropertyFlags.StdCppSet);
                        }else if(isMemberWritable)
                        	flags.set(PropertyFlags.Writable);
                        if (reader!=null || isMemberReadable)
                            flags.set(PropertyFlags.Readable);
                        if (resetter!=null)
                            flags.set(PropertyFlags.Resettable);
                        if ((bindable!=null || isMemberReadable) && Bindable!=null)
                            flags.set(Bindable);
                        
                        if (designableVariant instanceof Boolean) {
                            if ((Boolean) designableVariant)
                                flags.set(PropertyFlags.Designable);
                            metaData.propertyDesignableResolvers.add(null);
                        } else if (designableVariant instanceof Method) {
                            metaData.propertyDesignableResolvers.add((Method) designableVariant);
                            flags.set(ResolveDesignable);
                        }else {
                            metaData.propertyDesignableResolvers.add(null);
                        }
                        
                        if (scriptableVariant instanceof Boolean) {
                            if ((Boolean) scriptableVariant)
                                flags.set(PropertyFlags.Scriptable);
                            metaData.propertyScriptableResolvers.add(null);
                        } else if (scriptableVariant instanceof Method) {
                            flags.set(ResolveScriptable);
                            metaData.propertyScriptableResolvers.add((Method) scriptableVariant);
                        }else {
                            metaData.propertyScriptableResolvers.add(null);
                        }
                        
                        if (editableVariant instanceof Boolean) {
                            if ((Boolean) editableVariant)
                                flags.set(Editable);
                            metaData.propertyEditableResolvers.add(null);
                        } else if (editableVariant instanceof Method) {
                            flags.set(ResolveEditable);
                            metaData.propertyEditableResolvers.add((Method) editableVariant);
                        }else {
                            metaData.propertyEditableResolvers.add(null);
                        }
                        
                        if (storedVariant instanceof Boolean) {
                            if ((Boolean) storedVariant)
                                flags.set(PropertyFlags.Stored);
                            metaData.propertyStoredResolvers.add(null);
                        } else if (storedVariant instanceof Method) {
                                                        
                            flags.set(ResolveStored);
                            metaData.propertyStoredResolvers.add((Method) storedVariant);
                        }else {
                            metaData.propertyStoredResolvers.add(null);
                        }
                               
                                                                       
                         
                        
                        if (userVariant instanceof Boolean) {
                            if ((Boolean) userVariant)
                                flags.set(PropertyFlags.User);
                            metaData.propertyUserResolvers.add(null);
                        } else if (userVariant instanceof Method) {
                            flags.set(ResolveUser);
                            metaData.propertyUserResolvers.add((Method) userVariant);
                        }else {
                            metaData.propertyUserResolvers.add(null);
                        }
                        
                        if (Boolean.TRUE.equals(constantVariant) && writer!=null && notify!=null) {
                            flags.set(PropertyFlags.Constant);
                        }
                        
                        if (Boolean.TRUE.equals(requiredVariant) && Required!=null) {
                            flags.set(Required);
                        }

                        if (Boolean.TRUE.equals(finalVariant))
                            flags.set(PropertyFlags.Final);
                         
                        
                        if (notify!=null && Notify!=null)
                            flags.set(Notify);
                        
                     // properties: name, type, flags
                        int idx = metaData.stringData.indexOf(propertyName);
                        if(idx==-1){
                            metaData.stringData.add(propertyName);
                            idx = metaData.stringData.indexOf(propertyName);
                        }
                        metaData.metaData.add(idx);
                        intdataComments.add("property["+i+"].name");
                        QMetaType.Type type = metaType(typeName);
                        if(type==QMetaType.Type.UnknownType || type==QMetaType.Type.User){
                            int t = QtJambiInternal.findMetaType(typeName);
                            if(t==QMetaType.Type.UnknownType.value() || !(genericPropertyType instanceof Class || new QMetaType(t).name().toString().equals(typeName))) {
                                t = registerMetaType(propertyType, genericPropertyType, annotatedPropertyType, isPointer, isReference);
                            }
                            if(t!=QMetaType.Type.UnknownType.value())
                                typeName = new QMetaType(t).name().toString();
                            idx = metaData.stringData.indexOf(typeName);
                            if(idx==-1){
                                metaData.stringData.add(typeName);
                                idx = metaData.stringData.indexOf(typeName);
                            }
                            metaData.metaData.add(0x80000000 | idx);
                            metaData.metaTypes.set(i, t);
                        }else{
                            metaData.metaData.add(type.value());
                            metaData.metaTypes.set(i, type.value());
                        }
                        intdataComments.add("property["+i+"].type");
                        metaData.metaData.add(flags.value());
                        intdataComments.add("property["+i+"].flags");
                        if(NativeLibraryManager.VERSION_MAJOR>5){
                            Integer signalIndex = signalIndexes.get(notify);
                            metaData.metaData.add(signalIndex!=null ? signalIndex : -1);
                            intdataComments.add("property["+i+"].notifyId");
                            metaData.metaData.add(0);
                            intdataComments.add("property["+i+"].revision");
                        }
                        
                        metaData.propertyReaders.add(reader);
                        metaData.propertyWriters.add(writer);
                        metaData.propertyResetters.add(resetter);
                        metaData.propertyNotifies.add(notify);
                        metaData.propertyBindables.add(bindable);
                        metaData.propertyQPropertyFields.add(qPropertyField);
                        metaData.propertyMemberFields.add(propertyMemberField);
                        ++i;
                    }
                    
                    i=0;
                    for (String propertyName : propertyReaders.keySet()) {
                        Field notify = propertyNotifies.get(propertyName);
                        if(notify!=null) {
                            if(metaData.signalFields.contains(notify)) {
                                metaData.metaData.add(metaData.signalFields.indexOf(notify));
                            }else {
                                metaData.metaData.add(0x70000000);	            			
                            }
                            intdataComments.add("property["+i+"].notify_signal_id");
                        }else {
                            metaData.metaData.add(0);
                            intdataComments.add("property["+i+"].notify_signal_id");
                        }
                        ++i;
                    }
                }
                
                if(metaObjectFlags!=0) {
                    metaData.metaData.set(flagsIndex, metaObjectFlags);
                }
                //
                // Build enums array
                //
                
                if(!enums.isEmpty()){
                    metaData.metaData.set(ENUM_METADATA_INDEX, metaData.metaData.size());
                    List<Class<?>> enumList = new ArrayList<Class<?>>(enums.values());
                    HashMap<Object,Integer> dataIndexOfEnums = new HashMap<Object,Integer>();
                    
                    for (int i = 0; i < enumList.size(); i++) {
                        Class<?> enumClass = enumList.get(i);
                        // enums: name, alias, flags, count, data
                        metaData.stringData.add(enumClass.getSimpleName());
                        if(NativeLibraryManager.VERSION_MAJOR>5){
                            String alias = enumClass.getSimpleName();
                            if(QFlags.class.isAssignableFrom(enumClass)) {
                                Class<?> _enumClass = getEnumForQFlags(enumClass);
                                alias = _enumClass.getSimpleName();
                                metaData.stringData.add(alias);
                            }
                            metaData.metaData.add(metaData.stringData.indexOf(enumClass.getSimpleName()));	intdataComments.add("enum["+i+"].name");
                            metaData.metaData.add(metaData.stringData.indexOf(alias));	intdataComments.add("enum["+i+"].alias");
                        }else{
                            metaData.metaData.add(metaData.stringData.indexOf(enumClass.getSimpleName()));	intdataComments.add("enum["+i+"].name");
                        }
                                                                                                                                               
                                                                                                                            
                        metaData.metaData.add(QFlags.class.isAssignableFrom(enumClass) ? 0x1 : 0x0);	intdataComments.add("enum["+i+"].flags");
                        
                        // Get the enum class
                        Class<?> contentEnumClass = Enum.class.isAssignableFrom(enumClass) ? enumClass : getEnumForQFlags(enumClass);
                        
                        metaData.metaData.add(contentEnumClass.getEnumConstants().length);	intdataComments.add("enum["+i+"].count");
                        dataIndexOfEnums.put(enumClass, metaData.metaData.size());
                        metaData.metaData.add(0);	intdataComments.add("enum["+i+"].data");
                    }
                    
                    for (int i = 0; i < enumList.size(); i++) {
                        Class<?> enumClass = enumList.get(i);
                        @SuppressWarnings("unchecked")
                        Class<Enum<?>> contentEnumClass = (Class<Enum<?>>)(Enum.class.isAssignableFrom(enumClass) ? enumClass : getEnumForQFlags(enumClass));
                        // enum data: key, value
                        int ENUM_DATA_INDEX = dataIndexOfEnums.get(enumClass);
                        metaData.metaData.set(ENUM_DATA_INDEX, metaData.metaData.size());
                        for(Enum<?> enumConstant : contentEnumClass.getEnumConstants()){
                            metaData.stringData.add(enumConstant.name());
                            metaData.metaData.add(metaData.stringData.indexOf(enumConstant.name()));	intdataComments.add("enum["+i+"].data: key");
                            if(enumConstant instanceof QtEnumerator){
                                QtEnumerator enumerator = (QtEnumerator)enumConstant;
                                metaData.metaData.add(enumerator.value());
                            }else if(enumConstant instanceof QtShortEnumerator){
                                QtShortEnumerator enumerator = (QtShortEnumerator)enumConstant;
                                metaData.metaData.add((int)enumerator.value());
                            }else if(enumConstant instanceof QtByteEnumerator){
                                QtByteEnumerator enumerator = (QtByteEnumerator)enumConstant;
                                metaData.metaData.add((int)enumerator.value());
                            }else if(enumConstant instanceof QtLongEnumerator){
                                QtLongEnumerator enumerator = (QtLongEnumerator)enumConstant;
                                metaData.metaData.add((int)enumerator.value());
                            }else{
                                metaData.metaData.add(enumConstant.ordinal());
                            }
                            intdataComments.add("enum["+i+"].data: value");
                        }
                    }
                }
                
                //
                // Terminate data array
                //
                metaData.metaData.add(0); // eod
                intdataComments.add("end of data");
            }

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
                for (int i = 0; i < metaData.metaData.size(); i++) {
                    try {
                        String strg = null;
                        try {
                            if(intdataComments.get(i).endsWith("]: name")) {
                                strg = metaData.stringData.get(metaData.metaData.get(i));
                            }else if(intdataComments.get(i).endsWith("].argName")) {
                                strg = metaData.stringData.get(metaData.metaData.get(i));
                            }else if(intdataComments.get(i).endsWith("].arg")) {
                                int idx = metaData.metaData.get(i);
                                if((idx & 0x80000000) == 0x80000000) {
                                    idx = idx & ~0x80000000;
                                    if(idx>=0 && idx<metaData.stringData.size())
                                        strg = metaData.stringData.get(idx);
                                }else if(idx>=0 && idx<QMetaType.Type.values().length){
                                    for(QMetaType.Type t : QMetaType.Type.values()) {
                                        if(t.value()==idx) {
                                            strg = ""+t;
                                            break;
                                        }
                                    }
                                }
                            }else if(intdataComments.get(i).endsWith("].returnType")) {
                                int idx = metaData.metaData.get(i);
                                if((idx & 0x80000000) == 0x80000000) {
                                    idx = idx & ~0x80000000;
                                    if(idx>=0 && idx<metaData.stringData.size())
                                        strg = metaData.stringData.get(idx);
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
                                System.out.printf("\t%1$s: %3$s=%2$s (%4$s) --> %5$s\n", i, metaData.metaData.get(i), intdataComments.get(i), nms.get(i), strg);
                            }else {
                                System.out.printf("\t%1$s: %3$s=%2$s --> %4$s\n", i, metaData.metaData.get(i), intdataComments.get(i), strg);
                            }
                        }else {
                            if(i<nms.size()) {
                                System.out.printf("\t%1$s: %3$s=%2$s (%4$s)\n", i, metaData.metaData.get(i), intdataComments.get(i), nms.get(i));
                            }else {
                                System.out.printf("\t%1$s: %3$s=%2$s\n", i, metaData.metaData.get(i), intdataComments.get(i));
                            }
                        }
                    } catch (IndexOutOfBoundsException e) {
                        System.out.printf("\t%1$s: %2$s\n", i, metaData.metaData.get(i));
                    }
                }
                System.out.println("}");
            }
            
//			for (int i = 0; i < metaData.stringData.size(); i++) {
//				System.out.printf("string[%1$s]= %2$s\n", i, metaData.stringData.get(i));
//			}
            return metaData;
        } catch (RuntimeException | Error e) {
            throw e;
        } catch (Throwable e) {
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

    // Using a variable to ensure this is changed in all the right places in the
    //  future when higher values are supported.
    public static final int METAOBJECT_REVISION_HIGHEST_SUPPORTED = NativeLibraryManager.VERSION_MAJOR==5 ? 7 : 9;
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
        int major = NativeLibraryManager.VERSION_MAJOR;
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
        else if(major == 6)
            r = 9;  // 6.0.x (Qt6 requires a minimum of this revision)
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
    
    private static boolean isBuiltinType(String type)
    {
        QMetaType.Type id = metaType(type);
       if (id == QMetaType.Type.UnknownType)
           return false;
       return (id.value() < QMetaType.Type.User.value());
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
