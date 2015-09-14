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

import java.io.UnsupportedEncodingException;
import java.lang.reflect.Constructor;
import java.lang.reflect.Field;
import java.lang.reflect.Member;
import java.lang.reflect.Method;
import java.lang.reflect.Modifier;
import java.lang.reflect.ParameterizedType;
import java.lang.reflect.Type;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Hashtable;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.Queue;
import java.util.Set;
import java.util.Stack;

import org.qtjambi.qt.QFlags;
import org.qtjambi.qt.QPair;
import org.qtjambi.qt.QSignalEmitter;
import org.qtjambi.qt.QtBlockedEnum;
import org.qtjambi.qt.QtBlockedSlot;
import org.qtjambi.qt.QtEnumerator;
import org.qtjambi.qt.QtJambiGeneratedClass;
import org.qtjambi.qt.QtPropertyConstant;
import org.qtjambi.qt.QtPropertyDesignable;
import org.qtjambi.qt.QtPropertyNotify;
import org.qtjambi.qt.QtPropertyReader;
import org.qtjambi.qt.QtPropertyResetter;
import org.qtjambi.qt.QtPropertyScriptable;
import org.qtjambi.qt.QtPropertyStored;
import org.qtjambi.qt.QtPropertyUser;
import org.qtjambi.qt.QtPropertyWriter;
import org.qtjambi.qt.QtScriptable;
import org.qtjambi.qt.Utilities;
import org.qtjambi.qt.core.QByteArray;
import org.qtjambi.qt.core.QObject;
import org.qtjambi.qt.core.Qt;
import org.qtjambi.qt.declarative.QDeclarativeListProperty;
import org.qtjambi.qt.qml.QQmlListProperty;


/**
 * Methods to help construct the fake meta object.
 */
public class MetaObjectTools {
	
	enum VariableContext {
        ReturnType,
        ArgumentType
    }

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

    @SuppressWarnings("unused")
    private static class MetaData {
        public int metaData[];
        public byte stringData[][];

		public Field signalsArray[];
        public Method slotsArray[];
        public Constructor<?> constructorsArray[];

        public Method propertyReadersArray[];
        public Method propertyWritersArray[];
        public Method propertyResettersArray[];
        public Field propertyNotifiesArray[];
        public Method propertyDesignableResolverArray[];
        public Method propertyScriptableResolverArray[];
        public Method propertyEditableResolverArray[];
        public Method propertyStoredResolverArray[];
        public Method propertyUserResolverArray[];
        public Class<?> extraDataArray[] = {};

        public String originalSignatures[];
        
        public boolean hasStaticMembers;
    }

    private final static int MethodAccessPrivate                    = 0x00;
    private final static int MethodAccessProtected                  = 0x01;
    private final static int MethodAccessPublic                     = 0x02;
    private final static int MethodAccessMask                       = 0x03;

    private final static int MethodMethod                           = 0x00;
    private final static int MethodSignal                           = 0x04;
    private final static int MethodSlot                             = 0x08;
    private final static int MethodConstructor                      = 0x0c;
    private final static int MethodTypeMask                         = 0x0c;

    private final static int MethodCompatibility                    = 0x10;
    private final static int MethodCloned                           = 0x20;
    private final static int MethodScriptable                       = 0x40;
    private final static int MethodRevisioned 						= 0x80;

    private final static int PropertyInvalid                        = 0x00000000;
    private final static int PropertyReadable                       = 0x00000001;
    private final static int PropertyWritable                       = 0x00000002;
    private final static int PropertyResettable                     = 0x00000004;
    private final static int PropertyEnumOrFlag                     = 0x00000008;
    private final static int PropertyStdCppSet                      = 0x00000100;
    private final static int PropertyConstant                      	= 0x00000400;
    private final static int PropertyFinal                      	= 0x00000800;
    private final static int PropertyDesignable                     = 0x00001000;
    private final static int PropertyResolveDesignable              = 0x00002000;
    private final static int PropertyScriptable                     = 0x00004000;
    private final static int PropertyResolveScriptable              = 0x00008000;
    private final static int PropertyStored                         = 0x00010000;
    private final static int PropertyResolveStored                  = 0x00020000;
    private final static int PropertyEditable                       = 0x00040000;
    private final static int PropertyResolveEditable                = 0x00080000;
    private final static int PropertyUser                           = 0x00100000;
    private final static int PropertyResolveUser  					= 0x00200000;
    private final static int PropertyNotify 						= 0x00400000;
    private final static int PropertyRevisioned                     = 0x00800000;

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
        if (((!QObject.class.isAssignableFrom(enclosingClass) && !Qt.class.equals(enclosingClass))
               || enumType.isAnnotationPresent(QtBlockedEnum.class))) {
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
        if (((!QObject.class.isAssignableFrom(enclosingClass) && !Qt.class.equals(enclosingClass))
               || enumType.isAnnotationPresent(QtBlockedEnum.class))) {
            return false;
        }
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
                Method m = clazz.getMethod(value, (Class<?>[]) null);
                if (isBoolean(m.getReturnType()))
                    return m;
                else
                    throw new RuntimeException("Wrong return type of designable method '" + m.getName() + "'");
            } catch (Throwable t) {
                t.printStackTrace();
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
                Method m = clazz.getMethod(value, (Class<?>[]) null);
                if (isBoolean(m.getReturnType()))
                    return m;
                else
                    throw new RuntimeException("Wrong return type of scriptable method '" + m.getName() + "'");
            } catch (Throwable t) {
                t.printStackTrace();
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
                Method m = clazz.getMethod(value, (Class<?>[]) null);
                if (isBoolean(m.getReturnType()))
                    return m;
                else
                    throw new RuntimeException("Wrong return type of scriptable method '" + m.getName() + "'");
            } catch (Throwable t) {
                t.printStackTrace();
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
                Method m = clazz.getMethod(value, (Class<?>[]) null);
                if (isBoolean(m.getReturnType()))
                    return m;
                else
                    throw new RuntimeException("Wrong return type of scriptable method '" + m.getName() + "'");
            } catch (Throwable t) {
                t.printStackTrace();
            }
        }
		*/
        return Boolean.TRUE;
    }

    private static boolean isValidSetter(Method declaredMethod) {
        return (declaredMethod.getParameterTypes().length == 1
                && declaredMethod.getReturnType() == Void.TYPE
                && !internalTypeNameOfParameters(declaredMethod, 1).equals(""));
    }

    private static Method getMethod(Class<?> clazz, String name, Class<?> args[]) {
        try {
            return clazz.getMethod(name, args);
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
                Method m = clazz.getMethod(value, (Class<?>[]) null);
                if (isBoolean(m.getReturnType()))
                    return m;
                else
                    throw new RuntimeException("Wrong return type of scriptable method '" + m.getName() + "'");
            } catch (Throwable t) {
                t.printStackTrace();
            }
        }

        return Boolean.FALSE;
    }

    private static Boolean isFinal(Method declaredMethod) {
    	return Modifier.isFinal(declaredMethod.getModifiers());
    }

    private static Boolean isConstant(Method declaredMethod) {
    	QtPropertyConstant isConstant = declaredMethod.getAnnotation(QtPropertyConstant.class);

        if (isConstant != null) {
            String value = isConstant.value();

            if (value.equals("true")) {
                return Boolean.TRUE;
            } else if (value.equals("false")) {
                return Boolean.FALSE;
            }
        }

        return Boolean.FALSE;
    }

    private static boolean isValidGetter(Method method) {
        return (method.getParameterTypes().length == 0
                && method.getReturnType() != Void.TYPE);
    }


    public static String bunchOfClassNamesInARow(Class<?> classes[]) {
        return bunchOfClassNamesInARow(classes, null);
    }

    public static String bunchOfClassNamesInARow(Class<?> classes[], int arrayDimensions[]) {
        String classNames = "";

        for (int i=0; i<classes.length; ++i) {
            Class<?> clazz = classes[i];

            String className = clazz.getName();
            if (arrayDimensions != null) {
                for (int j=0; j<arrayDimensions[i]; ++j)
                    className = "java.lang.Object";
            }
            if(!clazz.isPrimitive()){
	            try{
					MetaObjectTools.class.getClassLoader().loadClass(className);
				}catch(ClassNotFoundException e){
					if(QObject.class.isAssignableFrom(clazz)){
						className = QObject.class.getName();
					}else{
						className = Object.class.getName();
					}
				}
            }
			classNames += className;
            if (i<classes.length-1)
                classNames += ",";
        }

        return classNames;
    }

    private static String methodParameters(Method m) {
        return bunchOfClassNamesInARow(m.getParameterTypes());
    }
    
    private static String methodParameters(Constructor<?> m) {
        return bunchOfClassNamesInARow(m.getParameterTypes());
    }

    /**
     * Returns the signature of the method m excluding the modifiers and the
     * return type.
     */
    private static String methodSignature(Method m, boolean includeReturnType) {
        return (includeReturnType ? m.getReturnType().getName() + " " : "")
               + m.getName() + "(" + methodParameters(m) + ")";
    }
    
    private static String methodSignature(Constructor<?> m) {
        return m.getName() + "(" + methodParameters(m) + ")";
    }
    
    private static String methodSignature(QtJambiInternal.ResolvedSignal r) {
        return r.name + "(" + bunchOfClassNamesInARow(r.types, r.arrayDimensions) + ")";
    }

    public static String methodSignature(Method m) {
        return methodSignature(m, false);
    }
    
    private static String constructorSignature(Constructor<?> c) {
    	String name = c.getName();
    	int idx = name.lastIndexOf('.');
    	if(idx>0){
    		name = name.substring(idx+1, name.length());
    	}
        return name + "(" + constructorParameters(c) + ")";
    }
    
    private static String constructorParameters(Constructor<?> c) {
        return bunchOfClassNamesInARow(c.getParameterTypes());
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

    public native static void emitNativeSignal(QObject object, String signalSignature, String signalCppSignature, Object args[]);

    public static String cppSignalSignature(QSignalEmitterInternal signalEmitter, String signalName) {
        QSignalEmitter.AbstractSignal signal = (QSignalEmitter.AbstractSignal) QtJambiInternal.lookupSignal(signalEmitter, signalName);
        if (signal != null)
            return cppSignalSignature(signal);
        else
            return "";
    }

    public static String cppSignalSignature(QSignalEmitter.AbstractSignalInternal signal) {
        String signalParameters = QtJambiInternal.signalParameters(signal);
        String params = MetaObjectTools.internalTypeNameOfSignal(signal.resolveSignal(), signalParameters, 1);
        if (signalParameters.length() > 0 && params.length() == 0)
            return "";
        else
            return signal.name() + "(" + params + ")";
    }

    private static String signalParameters(QtJambiInternal.ResolvedSignal resolvedSignal) {
        return MetaObjectTools.bunchOfClassNamesInARow(resolvedSignal.types, resolvedSignal.arrayDimensions);
    }

    private static String signalParameters(Field field, Class<?> declaringClass) {
        QtJambiInternal.ResolvedSignal resolvedSignal = QtJambiInternal.resolveSignal(field, declaringClass);
        return signalParameters(resolvedSignal);
    }


    private native static String internalTypeName(String s, int varContext);
    
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

    public static String internalTypeNameOfSignal(Class<?>[] signals, String s, int varContext) {
        try {
            return internalTypeName(s, varContext);
        } catch (Throwable t) {
            return "";
        }
    }
    
    public static String internalTypeNameOfParameters(Method declaredMethod, int varContext) {
        try {
        	String s = methodParameters(declaredMethod);
            return internalTypeName(s, varContext);
        } catch (Throwable t) {
            return "";
        }
    }
    
    public static String internalTypeNameOfParameters(Constructor<?> declaredConstructor, int varContext) {
        try {
        	String s = constructorParameters(declaredConstructor);
            return internalTypeName(s, varContext);
        } catch (Throwable t) {
            return "";
        }
    }
    
    public static String internalTypeNameOfMethodSignature(Method slot, int varContext) {
        try {
        	String javaMethodSignature = methodSignature(slot);
            return internalTypeName(javaMethodSignature, varContext);
        } catch (Throwable t) {
            return "";
        }
    }
    
    public static String internalTypeNameOfMethodSignature(Constructor<?> constructor, int varContext) {
        try {
        	String javaMethodSignature = constructorSignature(constructor);
            return internalTypeName(javaMethodSignature, varContext);
        } catch (Throwable t) {
            return "";
        }
    }
    
    public static String internalTypeNameOfClass(Class<? extends Object> cls, VariableContext varContext) {
        try {
            String returnType = cls.getName();
            if(!cls.isPrimitive()) {
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
			boolean isEnumOrFlags = Enum.class.isAssignableFrom(cls) || QFlags.class.isAssignableFrom(cls);
            if (isEnumOrFlags && cls.getDeclaringClass() != null && (QObject.class.isAssignableFrom(cls.getDeclaringClass()) || Qt.class.isAssignableFrom(cls.getDeclaringClass()))) {
            	if(cls.getDeclaringClass().isAnnotationPresent(QtJambiGeneratedClass.class)){
//            	if(QFlags.class.isAssignableFrom(cls)){
//            		return "QFlags<"+internalTypeNameOfClass(getEnumForQFlags(cls), varContext) +">";
//            	}else{
	            		return internalTypeNameOfClass(cls.getDeclaringClass(), varContext).replace("*", "") + "::" + cls.getSimpleName();
//	            	}else{
		                // To avoid using JObjectWrapper for enums and flags (which is required in certain cases.)
//		                return cls.getDeclaringClass().getName().replace(".", "::") + "::" + cls.getSimpleName();
	            	}
//            	}
            }
            return internalTypeName(returnType, varContext.ordinal());
        } catch (Throwable t) {
        	t.printStackTrace();
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
     * @author Peter Droste <info@omix-visualization.com>
     */
    private static MetaData buildMetaData(Class<? extends QObject> clazz) {
    	List<String> strings = new StringList();
    	List<Integer> intdata = new ArrayList<Integer>();
    	final String classname = clazz.getName().replace(".", "::");
    	strings.add(classname);
        MetaData metaData = new MetaData();

        Hashtable<String,String> classInfos = new Hashtable<String, String>();
        classInfos.put("__qt__binding_shell_language", "Qt Jambi");
        
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
            if (QtJambiInternal.isSignal(declaredField.getType())) {
                // If we can't convert all the types we don't list the signal
                QtJambiInternal.ResolvedSignal resolvedSignal = QtJambiInternal.resolveSignal(declaredField, declaredField.getDeclaringClass());
                String signalParameters = signalParameters(resolvedSignal);
                if (signalParameters.length() == 0 || internalTypeNameOfSignal(resolvedSignal.types, signalParameters, 1).length() != 0) {
                    signalFields.add(declaredField);
                    resolvedSignals.add(resolvedSignal);
                    originalSignatures.add(methodSignature(resolvedSignal));
                    
                    strings.add(resolvedSignal.name);
                    for (int i = 1; i <= resolvedSignal.types.length; i++) {
                    	String typeName;
                    	if(resolvedSignal.arrayDimensions==null || resolvedSignal.arrayDimensions[i-1]==0){
                    		typeName = internalTypeNameOfClass(resolvedSignal.types[i-1], VariableContext.ArgumentType);
                    	}else{
                    		typeName = internalTypeNameOfClass(Object.class, VariableContext.ArgumentType);
                    	}
                    	QMetaType.Type type = QMetaType.type(typeName);
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
                        && resolvedSignal.types.length <= 1) {
                        propertyNotifies.put(notify.name(), declaredField);
                    }
                }
            }
        }
        
        for(Constructor<?> constructor : clazz.getDeclaredConstructors()){
        	if(constructor.isAnnotationPresent(QtScriptable.class)){
	        	constructors.add(constructor);
	        	metaData.hasStaticMembers = true;
	        	originalSignatures.add(methodSignature(constructor));
	        	for (int i = 1; i <= constructor.getParameterTypes().length; i++) {
	            	String typeName = internalTypeNameOfClass(constructor.getParameterTypes()[i-1], VariableContext.ArgumentType);
	            	QMetaType.Type type = QMetaType.type(typeName);
	            	if(type==QMetaType.Type.UnknownType || type==QMetaType.Type.User){
	            		strings.add(typeName);
	            	}
	            	strings.add("arg__"+i);
				}
        	}
        }
        
        for (Method declaredMethod : clazz.getDeclaredMethods()) {

            if (
            		(
            				(
            						!declaredMethod.isAnnotationPresent(QtBlockedSlot.class) 
            						&& (declaredMethod.getModifiers() & Modifier.STATIC) != Modifier.STATIC
            				) || (
		            				declaredMethod.isAnnotationPresent(QtScriptable.class) 
		            				&& declaredMethod.getAnnotation(QtScriptable.class).value()
            				)
    				)
				) {

                // If we can't convert the type, we don't list it
                String methodParameters = methodParameters(declaredMethod);
                String returnType = declaredMethod.getReturnType().getName();
                String returnTypeName = internalTypeNameOfClass(declaredMethod.getReturnType(), VariableContext.ReturnType);
                if ((methodParameters.equals("") || !internalTypeNameOfParameters(declaredMethod, 1).equals(""))
                    &&(returnType.equals("") || returnType.equals("void") || !returnTypeName.equals(""))) {
                    if((declaredMethod.getModifiers() & Modifier.STATIC) != Modifier.STATIC){
                    	slots.add(declaredMethod);
                    }else{
                    	methods.add(declaredMethod);
                    	metaData.hasStaticMembers = true;
                    }
                    originalSignatures.add(methodSignature(declaredMethod));
                    strings.add(declaredMethod.getName());
                    strings.add(returnTypeName);
                    for (int i = 1; i <= declaredMethod.getParameterTypes().length; i++) {
                    	String typeName = internalTypeNameOfClass(declaredMethod.getParameterTypes()[i-1], VariableContext.ArgumentType);
                    	QMetaType.Type type = QMetaType.type(typeName);
                    	if(type==QMetaType.Type.UnknownType || type==QMetaType.Type.User){
                    		strings.add(typeName);
                    	}
                    	strings.add("arg__"+i);
					}
                    strings.add(""); // for the method tag
                }
            }

            // Rules for readers:
            // 1. Zero arguments
            // 2. Return something other than void
            // 3. We can convert the type
            Container reader = Container.readerAnnotation(declaredMethod);
            {

                if (reader != null
                    && reader.enabled()
                    && isValidGetter(declaredMethod)
                    && !internalTypeNameOfClass(declaredMethod.getReturnType(), VariableContext.ReturnType).equals("")) {

                    // If the return type of the property reader is not registered, then
                    // we need to register the owner class in the meta object (in which case
                    // it has to be a QObject)
                    Class<?> returnType = declaredMethod.getReturnType();

                    if ((QFlags.class.isAssignableFrom(returnType) || Enum.class.isAssignableFrom(returnType)) && !isEnumAllowedForProperty(returnType)) {
                        System.err.println("Problem in property '" + reader.name() + "' in '" + clazz.getName()
                                           + "' with return type '"+returnType.getName()
                                           +"': Only enum types 1. declared inside QObject subclasses or the "
                                           + "Qt interface and 2. declared without the QtBlockedEnum annotation "
                                           + "are supported for properties");
                        continue;
                    }

                    propertyReaders.put(reader.name(), declaredMethod);
                    propertyDesignableResolvers.put(reader.name(), isDesignable(declaredMethod, clazz));
                    propertyScriptableResolvers.put(reader.name(), isScriptable(declaredMethod, clazz));
                    propertyEditableResolvers.put(reader.name(), isEditable(declaredMethod, clazz));
                    propertyStoredResolvers.put(reader.name(), isStored(declaredMethod, clazz));
                    propertyUserResolvers.put(reader.name(), isUser(declaredMethod, clazz));
                    propertyConstantResolvers.put(reader.name(), isConstant(declaredMethod));
                    propertyFinalResolvers.put(reader.name(), isFinal(declaredMethod));
                    strings.add(reader.name());
                }
            }

            // Rules for writers:
            // 1. Takes exactly one argument
            // 2. Return void
            // 3. We can convert the type
            Container writer = Container.writerAnnotation(declaredMethod);
            {

                if (writer != null
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
                    && declaredMethod.getParameterTypes().length == 0
                    && declaredMethod.getReturnType() == Void.TYPE) {
                    propertyResetters.put(resetter.name(), declaredMethod);
                }
            }

            // Check naming convention by looking for setXxx patterns, but only if it hasn't already been
            // annotated as a writer
            String declaredMethodName = declaredMethod.getName();
            if (writer == null
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
                    Method readerMethod = notBogus(getMethod(clazz, propertyName, null), propertyName, paramType);
                    if (readerMethod == null)
                        readerMethod = notBogus(getMethod(clazz, "get" + capitalizeFirst(propertyName), null), propertyName, paramType);
                    if (readerMethod == null && isBoolean(paramType))
                        readerMethod = notBogus(getMethod(clazz, "is" + capitalizeFirst(propertyName), null), propertyName, paramType);
                    if (readerMethod == null && isBoolean(paramType))
                        readerMethod = notBogus(getMethod(clazz, "has" + capitalizeFirst(propertyName), null), propertyName, paramType);

                    if (readerMethod != null) { // yay
                        reader = Container.readerAnnotation(readerMethod);
                        if (reader == null) {
                            propertyReaders.put(propertyName, readerMethod);
                            propertyWriters.put(propertyName, declaredMethod);

                            propertyDesignableResolvers.put(propertyName, isDesignable(readerMethod, clazz));
                            propertyScriptableResolvers.put(propertyName, isScriptable(readerMethod, clazz));
                            propertyUserResolvers.put(propertyName, isUser(readerMethod, clazz));
                        }
                    }
                }
            }
        }
    	ArrayList<Method> slotsMethods = new ArrayList<Method>(slots);
    	slotsMethods.addAll(methods);
        
        metaData.signalsArray = signalFields.toArray(new Field[0]);
        metaData.slotsArray = slotsMethods.toArray(new Method[0]);
        metaData.constructorsArray = constructors.toArray(new Constructor[0]);
        
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
        	intdata.add(resolveMetaDataRevision());
        	// classname
        	intdata.add(strings.indexOf(classname));
        	// classinfo
        	intdata.add(classInfos.size());
        	intdata.add(classInfos.isEmpty() ? 0 : MO_HEADER_LEN);
        	
        	// methods
        	int methodCount = signalFields.size() + slots.size() + methods.size();
        	intdata.add(methodCount);
        	final int METHOD_METADATA_INDEX = intdata.size();
        	intdata.add(0);
        	
            // properties
            intdata.add(propertyReaders.size());
            final int PROPERTY_METADATA_INDEX = intdata.size();
        	intdata.add(0);
        	
        	// enums/sets
        	intdata.add(enums.size());
        	final int ENUM_METADATA_INDEX = intdata.size();
        	intdata.add(0);
        	
        	// constructors
        	intdata.add(!constructors.isEmpty() ? constructors.size() : 0);
        	final int CONSTRUCTOR_METADATA_INDEX = intdata.size();
        	intdata.add(0);
        	
        	// flags
        	intdata.add(0);
        	
        	// signalCount
        	intdata.add(signalFields.size());
        	
        	//
        	// Build classinfo array
        	//
        	for(Map.Entry<String,String> entry : classInfos.entrySet()){
        		// classinfo: key, value
        		intdata.add(strings.indexOf(entry.getKey()));
        		intdata.add(strings.indexOf(entry.getValue()));
        	}
        	
        	HashMap<Object,Integer> paramIndexOfMethods = new HashMap<Object,Integer>();
            
            //
	        // Build constructors array
	        //
            
            if(!constructors.isEmpty()){
            	intdata.set(CONSTRUCTOR_METADATA_INDEX, intdata.size());
	            for (int i = 0; i < constructors.size(); i++) {
					Constructor<?> constructor = constructors.get(i);
					int flags = MethodConstructor;
	        		if (Modifier.isPrivate(constructor.getModifiers()))
	                    flags |= MethodAccessPrivate;
	                else if (Modifier.isPublic(constructor.getModifiers()))
	                    flags |= MethodAccessPublic;
	                else
	                    flags |= MethodAccessProtected;
	                
	        		if (constructor.isAnnotationPresent(QtScriptable.class) && constructor.getAnnotation(QtScriptable.class).value())
	                    flags |= MethodScriptable;
	        		int argc = constructor.getParameterTypes().length;
	        		
	        		// slots: name, argc, parameters, tag, flags
	        		String className = constructor.getDeclaringClass().getName();
	        		if(className.contains(".")){
	        			className = className.substring(className.lastIndexOf('.')+1);
	        		}
	        		strings.add(className);
	        		intdata.add(strings.indexOf(className));
	        		intdata.add(argc);
	        		paramIndexOfMethods.put(constructor, intdata.size());
	        		intdata.add(0);
	        		intdata.add(strings.indexOf(""));
	        		intdata.add(flags);
				}
            }
        	
            //
        	// Build signals array first, otherwise the signal indices would be wrong
        	//
        	if(signalFields.size() + slotsMethods.size() > 0){

        		intdata.set(METHOD_METADATA_INDEX, intdata.size());
        		
            	for (int i = 0; i < signalFields.size(); ++i) {
            		Field signalField = signalFields.get(i);
            		QtJambiInternal.ResolvedSignal resolvedSignal = resolvedSignals.get(i);
            		int flags = MethodSignal;
            		if (Modifier.isPrivate(signalField.getModifiers()))
                        flags |= MethodAccessPrivate;
                    else if (Modifier.isPublic(signalField.getModifiers()))
                        flags |= MethodAccessPublic;
                    else
                        flags |= MethodAccessProtected;
                    
            		if (signalField.isAnnotationPresent(QtScriptable.class))
                        flags |= MethodScriptable;
            		int argc = resolvedSignal.types.length;
            		
            		// signals: name, argc, parameters, tag, flags
            		intdata.add(strings.indexOf(signalField.getName()));
            		intdata.add(argc);
            		paramIndexOfMethods.put(resolvedSignal, intdata.size());
            		intdata.add(0);
            		intdata.add(strings.indexOf(""));
            		intdata.add(flags);
            	}
            	
            	//
            	// Build slots array
            	// Build method array
            	//
            	for (int i = 0; i < slotsMethods.size(); i++) {
    				Method method = slotsMethods.get(i);
    				int flags = slots.contains(method) ? MethodSlot : MethodMethod;
            		if (Modifier.isPrivate(method.getModifiers()))
                        flags |= MethodAccessPrivate;
                    else if (Modifier.isPublic(method.getModifiers()))
                        flags |= MethodAccessPublic;
                    else
                        flags |= MethodAccessProtected;
                    
            		if (!method.isAnnotationPresent(QtScriptable.class) || method.getAnnotation(QtScriptable.class).value())
                        flags |= MethodScriptable;
            		int argc = method.getParameterTypes().length;
            		
            		// slots: name, argc, parameters, tag, flags
            		intdata.add(strings.indexOf(method.getName()));
            		intdata.add(argc);
            		paramIndexOfMethods.put(method, intdata.size());
            		intdata.add(0);
            		intdata.add(strings.indexOf(""));
            		intdata.add(flags);
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
        		intdata.add(QMetaType.Type.Void.value);
        		for (int j = 0; j < resolvedSignal.types.length; j++) {
        			String typeName;
                	if(resolvedSignal.arrayDimensions==null || resolvedSignal.arrayDimensions[j]==0){
                		typeName = internalTypeNameOfClass(resolvedSignal.types[j], VariableContext.ArgumentType);
                	}else{
                		typeName = internalTypeNameOfClass(Object.class, VariableContext.ArgumentType);
                	}
                	QMetaType.Type type = QMetaType.type(typeName);
                	if(type==QMetaType.Type.UnknownType || type==QMetaType.Type.User){
                		if(!strings.contains(typeName)){
                			strings.add(typeName);
                		}
                		intdata.add(0x80000000 | strings.indexOf(typeName));
                	}else{
                		intdata.add(type.value);
                	}
				}
        		for (int j = 1; j <= resolvedSignal.types.length; j++) {
        			intdata.add(strings.indexOf("arg__"+j));
        		}
        	}
        	
        	for (int i = 0; i < slotsMethods.size(); i++) {
				Method method = slotsMethods.get(i);
				// slot/method: parameters
				int METHOD_PARAMETER_INDEX = paramIndexOfMethods.get(method);
        		intdata.set(METHOD_PARAMETER_INDEX, intdata.size());
				String typeName = internalTypeNameOfClass(method.getReturnType(), VariableContext.ReturnType);
        		QMetaType.Type type = QMetaType.type(typeName);
            	if(type==QMetaType.Type.UnknownType || type==QMetaType.Type.User){
            		if(!strings.contains(typeName)){
            			strings.add(typeName);
            		}
            		intdata.add(0x80000000 | strings.indexOf(typeName));
            	}else{
            		intdata.add(type.value);
            	}
        		for (int j = 0; j < method.getParameterTypes().length; j++) {
            		typeName = internalTypeNameOfClass(method.getParameterTypes()[j], VariableContext.ArgumentType);
                	type = QMetaType.type(typeName);
                	if(type==QMetaType.Type.UnknownType || type==QMetaType.Type.User){
                		if(!strings.contains(typeName)){
                			strings.add(typeName);
                		}
                		intdata.add(0x80000000 | strings.indexOf(typeName));
                	}else{
                		intdata.add(type.value);
                	}
				}
        		for (int j = 1; j <= method.getParameterTypes().length; j++) {
        			intdata.add(strings.indexOf("arg__"+j));
        		}
        	}
        	
        	for (int i = 0; i < constructors.size(); i++) {
				Constructor<?> constructor = constructors.get(i);
				// constructors: parameters
				int METHOD_PARAMETER_INDEX = paramIndexOfMethods.get(constructor);
        		intdata.set(METHOD_PARAMETER_INDEX, intdata.size());
//				paramIndexOfMethods.put(constructor, intdata.size());
        		intdata.add(0x80000000 | strings.indexOf(""));
        		for (int j = 0; j < constructor.getParameterTypes().length; j++) {
        			String typeName = internalTypeNameOfClass(constructor.getParameterTypes()[j], VariableContext.ArgumentType);
        			QMetaType.Type type = QMetaType.type(typeName);
                	if(type==QMetaType.Type.UnknownType || type==QMetaType.Type.User){
                		if(!strings.contains(typeName)){
                			strings.add(typeName);
                		}
                		intdata.add(0x80000000 | strings.indexOf(typeName));
                	}else{
                		intdata.add(type.value);
                	}
				}
        		for (int j = 1; j <= constructor.getParameterTypes().length; j++) {
        			intdata.add(strings.indexOf("arg__"+j));
        		}
        	}
        	
        	//
        	// Build property array
        	//
        	
        	String propertyNames[] = propertyReaders.keySet().toArray(new String[0]);
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
	                Boolean constantVariant = propertyConstantResolvers.get(propertyNames[i]);
	                Boolean finalVariant = propertyFinalResolvers.get(propertyNames[i]);
	
	                if (writer != null && !reader.getReturnType().isAssignableFrom(writer.getParameterTypes()[0])) {
	                    System.err.println("QtJambiInternal.buildMetaData: Writer for property "
	                            + propertyNames[i] + " takes a type which is incompatible with reader's return type.");
	                    writer = null;
	                }
	
	                int flags = PropertyInvalid;
	                // Type (need to special case flags and enums)
	                String typeName = internalTypeNameOfClass(reader.getReturnType(), VariableContext.ReturnType);
	                
	                if (!isBuiltinType(typeName))
	                    flags |= PropertyEnumOrFlag;
	                if (writer!=null){
	                    flags |= PropertyWritable;
	                    String s = "set";
	                    s += propertyNames[i].toUpperCase().charAt(0);
	                    s += propertyNames[i].substring(1);
	                    if (s.equals(writer.getName()))
	                        flags |= PropertyStdCppSet;
	
	                }
	                if (reader!=null)
	                    flags |= PropertyReadable;
	                if (resetter!=null)
	                    flags |= PropertyResettable;
	                
	                if (designableVariant instanceof Boolean) {
	                    if ((Boolean) designableVariant)
	                    	flags |= PropertyDesignable;
	                } else if (designableVariant instanceof Method) {
	                    metaData.propertyDesignableResolverArray[i] = (Method) designableVariant;
	                	flags |= PropertyResolveDesignable;
	                }
	                
	                if (scriptableVariant instanceof Boolean) {
	                    if ((Boolean) scriptableVariant)
	                    	flags |= PropertyScriptable;
	                } else if (scriptableVariant instanceof Method) {
	                	flags |= PropertyResolveScriptable;
	                    metaData.propertyScriptableResolverArray[i] = (Method) scriptableVariant;
	                }
	                
	                if (editableVariant instanceof Boolean) {
	                    if ((Boolean) editableVariant)
	                    	flags |= PropertyEditable;
	                } else if (editableVariant instanceof Method) {
	                	flags |= PropertyResolveEditable;
	                    metaData.propertyEditableResolverArray[i] = (Method) editableVariant;
	                }
	                
	                if (storedVariant instanceof Boolean) {
	                    if ((Boolean) storedVariant)
	                    	flags |= PropertyStored;
	                } else if (storedVariant instanceof Method) {
	                	flags |= PropertyResolveStored;
	                    metaData.propertyStoredResolverArray[i] = (Method) storedVariant;
	                }
	                
	                if (userVariant instanceof Boolean) {
	                    if ((Boolean) userVariant)
	                    	flags |= PropertyUser;
	                } else if (userVariant instanceof Method) {
	                	flags |= PropertyResolveUser;
	                    metaData.propertyUserResolverArray[i] = (Method) userVariant;
	                }
	                
	                if (constantVariant instanceof Boolean) {
	                    if ((Boolean) constantVariant && writer!=null && notify!=null)
	                    	flags |= PropertyConstant;
	                }

	                if (finalVariant instanceof Boolean) {
	                    if ((Boolean) finalVariant)
	                    	flags |= PropertyFinal;
	                }
	                
	                if (notify!=null)
	                    flags |= PropertyNotify;
	                
	             // properties: name, type, flags
	                int idx = strings.indexOf(propertyNames[i]);
	                if(idx==-1){
	                	strings.add(propertyNames[i]);
	                	idx = strings.indexOf(propertyNames[i]);
	                }
	                intdata.add(idx);
	                QMetaType.Type type = QMetaType.type(typeName);
	            	if(type==QMetaType.Type.UnknownType || type==QMetaType.Type.User){
	            		idx = strings.indexOf(typeName);
	            		if(idx==-1){
		                	strings.add(typeName);
		                	idx = strings.indexOf(typeName);
		                }
	            		intdata.add(0x80000000 | idx);
	            	}else{
	            		intdata.add(type.value);
	            	}
	                intdata.add(flags);
	                
	                metaData.propertyReadersArray[i] = reader;
	                metaData.propertyWritersArray[i] = writer;
	                metaData.propertyResettersArray[i] = resetter;
	                metaData.propertyNotifiesArray[i] = notify;
	            }
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
	            	intdata.add(strings.indexOf(enumClass.getSimpleName()));
	            	intdata.add(QFlags.class.isAssignableFrom(enumClass) ? 0x1 : 0x0);
	            	
	                // Get the enum class
	                Class<?> contentEnumClass = Enum.class.isAssignableFrom(enumClass) ? enumClass : getEnumForQFlags(enumClass);
	            	
	            	intdata.add(contentEnumClass.getEnumConstants().length);
	            	dataIndexOfEnums.put(enumClass, intdata.size());
	            	intdata.add(0);
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
	            		intdata.add(strings.indexOf(enumConstant.name()));
	                	if(enumConstant instanceof QtEnumerator){
	                		QtEnumerator enumerator = (QtEnumerator)enumConstant;
	                		intdata.add(enumerator.value());
	                	}else{
	                		intdata.add(enumConstant.ordinal());
	                	}
	            	}
	            }
            }
            
            //
	        // Terminate data array
	        //
            intdata.add(0); // eod
        }
        
        metaData.metaData = new int[intdata.size()];
//        System.out.println("{");
        for (int i = 0; i < metaData.metaData.length; i++) {
//        	System.out.printf("\t%1$s: %2$s\n", i, intdata.get(i));
        	metaData.metaData[i] = intdata.get(i);
		}
//        System.out.println("}");
        
        metaData.stringData = new byte[strings.size()][];
        for (int i = 0; i < metaData.stringData.length; i++) {
//        	System.out.printf("%1$s: %2$s\n", i, strings.get(i));
        	try {
				metaData.stringData[i] = strings.get(i).getBytes("UTF-8");
			} catch (UnsupportedEncodingException e) {
				metaData.stringData[i] = strings.get(i).getBytes();
			}
		}
        metaData.originalSignatures = originalSignatures.toArray(new String[originalSignatures.size()]);
        return metaData;
    }

    // Using a variable to ensure this is changed in all the right places in the
    //  future when higher values are supported.
    public static final int METAOBJECT_REVISION_HIGHEST_SUPPORTED = 7;
    // This property allows you to override the QMetaObject revision number for
    //  QtJambi to use.
    public static final String K_org_qtjambi_qt_qtjambi_metadata_revision = "org.qtjambi.qt.qtjambi.metadata.revision";
    private static int revision;
    // This should be updated as the code-base supports the correct data layout
    //  for each new revision.  We don't necessarily have to support the features
    //  that new revision brings as well.
    private static int resolveMetaDataRevision() {
        int r = revision;
        if(r != 0)
            return r;

        int[] versionA = Utilities.getVersion();
        int major = -1;
        if(versionA.length > 0)
            major = versionA[0];
        int minor = -1;
        if(versionA.length > 1)
            minor = versionA[1];
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
            s = System.getProperty(K_org_qtjambi_qt_qtjambi_metadata_revision);
            if(s != null) {
                r = Integer.parseInt(s);
                if(r <= 0 || r > METAOBJECT_REVISION_HIGHEST_SUPPORTED)
                    r = -1;  // invalidate causing the value to be ignored
            }
        } catch(NumberFormatException e) {
            r = -1;
        } catch(SecurityException e) {
            e.printStackTrace();
        }
        if(r < 0)
            System.err.println("System Property " + K_org_qtjambi_qt_qtjambi_metadata_revision + " invalid value: " + s + " using default: " + defaultRevision);
        return r;
    }
    
    private static int generateTypeInfo(List<String> strings, String typeName){
        if (isBuiltinType(typeName)) {
        	QMetaType.Type type = nameToBuiltinType(typeName);
        	return type.value;
        } else {
        	return 0x80000000 | strings.indexOf(typeName);
        }
    }
    
    private static boolean isBuiltinType(String type)
    {
    	QMetaType.Type id = QMetaType.type(type);
       if (id == QMetaType.Type.UnknownType)
           return false;
       return (id.value < QMetaType.Type.User.value);
    }
    
    private static QMetaType.Type nameToBuiltinType(String name)
    {
        if (name.isEmpty())
            return QMetaType.Type.UnknownType;

        QMetaType.Type tp = QMetaType.type(name);
        return tp.value < QMetaType.Type.User.value ? tp : QMetaType.Type.UnknownType;
    }
    
    private static native int __qt_registerMetaType(Class<?> clazz);
    
    public static int registerMetaType(Class<?> clazz){
    	String packageName = clazz.getName();
        int idx = packageName.lastIndexOf('.');
        if(idx>0){
            packageName = packageName.substring(0, idx);
        }
        Utilities.initializePackage(packageName);
        return __qt_registerMetaType(clazz);
    }
    
    private static int aggregateParameterCount(List<?> list)
    {
        int sum = 0;
        for (int i = 0; i < list.size(); ++i){
        	Object method = list.get(i);
        	if(method instanceof QtJambiInternal.ResolvedSignal){
        		sum += ((QtJambiInternal.ResolvedSignal)method).types.length + 1; // +1 for return type
        	}else if(method instanceof Method){
        		sum += ((Method)method).getGenericParameterTypes().length + 1; // +1 for return type
        	}else if(method instanceof Constructor){
        		sum += ((Constructor<?>)method).getGenericParameterTypes().length + 1; // +1 for return type
        	}
        }
        return sum;
    }
}
