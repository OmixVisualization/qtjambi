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

import static io.qt.internal.EnumUtility.getEnumForQFlags;
import static io.qt.internal.MetaTypeUtility.findMetaType;
import static io.qt.internal.MetaTypeUtility.internalTypeNameOfClass;
import static io.qt.internal.MetaTypeUtility.registerMetaType;
import static io.qt.internal.MetaTypeUtility.registerRefMetaType;

import java.lang.reflect.AccessibleObject;
import java.lang.reflect.AnnotatedElement;
import java.lang.reflect.Constructor;
import java.lang.reflect.Field;
import java.lang.reflect.Member;
import java.lang.reflect.Method;
import java.lang.reflect.Modifier;
import java.lang.reflect.Parameter;
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
import java.util.TreeSet;
import java.util.function.Function;
import java.util.logging.Logger;

import io.qt.NativeAccess;
import io.qt.QFlags;
import io.qt.QPropertyDeclarationException;
import io.qt.QSignalDeclarationException;
import io.qt.QSignalInitializationException;
import io.qt.QtAsGadget;
import io.qt.QtByteEnumerator;
import io.qt.QtClassInfo;
import io.qt.QtEnumerator;
import io.qt.QtInvokable;
import io.qt.QtLongEnumerator;
import io.qt.QtMetaType;
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
import io.qt.QtUninvokable;
import io.qt.QtUnlistedEnum;
import io.qt.core.QDeclarableSignals;
import io.qt.core.QMetaObject;
import io.qt.core.QMetaType;
import io.qt.core.QObject;
import io.qt.core.QPair;
import io.qt.core.QStaticMemberSignals;
import io.qt.internal.SignalUtility.SignalParameterType;


/**
 * Methods to help construct the fake meta object.
 * @hidden
 */
final class MetaObjectUtility extends AbstractMetaObjectUtility{
    
    private MetaObjectUtility() { throw new RuntimeException();}
    
    private final static boolean isQt6 = QtJambi_LibraryUtilities.qtMajorVersion>5;
    
    static <K,V> Function<? super K, ArrayList<V>> arrayListFactory(){
		return key->new ArrayList<>();
	}
	
    /**
     * this method analyzes the given class for meta object data.
     * It is based upon code of the moc tool.
     */
    @NativeAccess
    private static MetaObjectData analyze(Class<?> clazz) {
        try {
            if(clazz.isPrimitive()) {
                throw new RuntimeException("Cannot analyze meta object from primitive type");
            }
            else if(clazz.isArray()) {
            	throw new RuntimeException("Cannot analyze meta object from array type");
            }
            MetaObjectData metaObjectData = new MetaObjectData();
            metaObjectData.addStringDataAndReturnIndex("Reserving the first string for QDynamicMetaObject identification.");
            List<String> intdataComments = /*new ArrayList<>();*/new AbstractList<String>() {
                @Override
                public boolean add(String e) { return false; }
                @Override
                public String get(int index) {return null;}
                @Override
                public int size() {return 0; }
            };
            final String classname = clazz.getName().replace(".", "::").replace("$", "::");
            
            Hashtable<String,String> classInfos = new Hashtable<String, String>();
            
            for(QtClassInfo info : clazz.getAnnotationsByType(QtClassInfo.class)) {
                classInfos.put(info.key(), info.value());
            }
            if(qmlClassInfoGeneratorFunction!=null) {
            	classInfos.putAll(qmlClassInfoGeneratorFunction.apply(clazz));
            }
            
            Map<Method, MethodFlags> methodFlags = new HashMap<>();
            Map<String, Method> propertyReaders = new TreeMap<>();
            Map<String, List<Method>> propertyWriters = new TreeMap<>();
            Map<String, Object> propertyDesignableResolvers = new TreeMap<>();
            Map<String, Object> propertyScriptableResolvers = new TreeMap<>();
            Map<String, Object> propertyEditableResolvers = new TreeMap<>();
            Map<String, Object> propertyStoredResolvers = new TreeMap<>();
            Map<String, Object> propertyUserResolvers = new TreeMap<>();
            Map<String, Boolean> propertyRequiredResolvers = new TreeMap<>();
            Map<String, Boolean> propertyConstantResolvers = new TreeMap<>();
            Map<String, Boolean> propertyFinalResolvers = new TreeMap<>();
                                                                                         
            Map<String, Method> propertyResetters = new TreeMap<>();
            Map<String, Field> propertyNotifies = new TreeMap<>();
            Map<String, Method> propertyBindables = new TreeMap<>();
            Map<String, Field> propertyMembers = new TreeMap<>();
            Map<String, Field> propertyQPropertyFields = new TreeMap<>();
            Map<String, Field> signals = new TreeMap<>();
            
            // First we get all enums actually declared in the class
            Map<String, Class<?>> enums = findFlagsAndEnums(clazz);
            
            class ParameterInfo{
            	public ParameterInfo(int metaTypeId, String typeName) {
					super();
					this.type = null;
					this.metaTypeId = metaTypeId;
					this.typeName = typeName;
				}
            	public ParameterInfo(io.qt.core.QMetaType.Type type) {
					super();
					this.type = type;
					this.metaTypeId = 0;
					this.typeName = null;
				}
				final QMetaType.Type type;
            	final int metaTypeId;
            	final String typeName;
            }

            Set<String> addedMethodSignatures = new TreeSet<>();
            List<Boolean> signalIsClone = new ArrayList<>();
            List<List<ParameterInfo>> allSignalParameterInfos = new ArrayList<>();
            boolean isQObject = QObject.class.isAssignableFrom(clazz);
            boolean isGadget = clazz.isAnnotationPresent(QtAsGadget.class);
            if(isQObject && isGadget){
            	throw new IllegalStateException("Must not annotate QObject type '"+clazz.getTypeName()+"' with @QtAsGadget.");
            }
            if(!isQObject && !isGadget){
            	isGadget = gadgetClasses.contains(clazz) || gadgetPackages.contains(clazz.getPackage().getName());
            }
            {
            	TreeSet<Field> declaredFields = new TreeSet<>((m1, m2)->{
                	return m1.getName().compareTo(m2.getName());
                });
            	declaredFields.addAll(Arrays.asList(clazz.getDeclaredFields()));
signalLoop:	    for (Field declaredField : declaredFields) {
					Class<?> fieldType = declaredField.getType();
	            	if(isQObjectSignalType(fieldType)) {
	            		if (!Modifier.isStatic(declaredField.getModifiers())) {
	            			if(!isQObject && fieldType.getEnclosingClass() == QObject.class)
	            				throw new QSignalDeclarationException(String.format("Declaration error at signal %1$s.%2$s: do not use QObject signals within non-QObjects.", clazz.getSimpleName(), declaredField.getName()));
		                    // If we can't convert all the types we don't list the signal
	            			List<Class<?>> emitParameterTypes = new ArrayList<>();
	            			List<SignalParameterType> signalTypes = new ArrayList<>(SignalUtility.resolveSignal(declaredField));
	                    	List<String> cppTypes = new ArrayList<>();
	                        List<ParameterInfo> signalParameterInfos = new ArrayList<>();
	                        for (int j = 0; j < signalTypes.size(); j++) {
	                        	emitParameterTypes.add(Object.class);
	                            SignalUtility.SignalParameterType signalType = signalTypes.get(j);
	                            QtMetaType metaTypeDecl = signalType.annotatedType!=null ? signalType.annotatedType.getAnnotation(QtMetaType.class) : null;
	                            int metaTypeId = 0;
	                            String typeName;
	                            if(metaTypeDecl!=null) {
	                				if(metaTypeDecl.id()!=0) {
	                					metaTypeId = metaTypeDecl.id();
                						metaTypeId = registerRefMetaType(metaTypeId, signalType.isPointer, signalType.isReference);
	                					typeName = new QMetaType(metaTypeId).name().toString();
	                				}else if(metaTypeDecl.type()!=QMetaType.Type.UnknownType){
	                					metaTypeId = metaTypeDecl.type().value();
                						metaTypeId = registerRefMetaType(metaTypeId, signalType.isPointer, signalType.isReference);
	                					typeName = new QMetaType(metaTypeId).name().toString();
	                				}else {
	            						if(metaTypeDecl.name().isEmpty())
	            							throw new IllegalArgumentException("Incomplete @QtMetaType declaration. Either use type, id or name to specify meta type.");
	                					typeName = metaTypeDecl.name();
	                					if(signalType.isPointer && !typeName.endsWith("*")) {
	                                        typeName += "*";
	                                    }
	                                    if(signalType.isReference) {
	                                    	if(typeName.endsWith("*")) {
	                                            typeName = typeName.substring(0, typeName.length()-2);
	                                        }
	                                        if(!typeName.endsWith("&")) {
	                                            typeName += "&";
	                                        }
	                                    }
	                				}
	                			}else{
	                				typeName = internalTypeNameOfClass(signalType.type, signalType.genericType);
	                				if(signalType.isPointer) {
	                                    if(!typeName.isEmpty() && !typeName.endsWith("*")) {
	                                        typeName += "*";
	                                    }
	                                }
	                                if(signalType.isReference) {
	                                    if(typeName.endsWith("*")) {
	                                        typeName = typeName.substring(0, typeName.length()-2);
	                                    }
	                                    if(!typeName.isEmpty() && !typeName.endsWith("&")) {
	                                        typeName += "&";
	                                    }
	                                }
	                			}
	                            QMetaType.Type type = metaType(typeName);
	                            if(type==QMetaType.Type.UnknownType || type==QMetaType.Type.User){
	                                if(metaTypeId==QMetaType.Type.UnknownType.value()) {
	                                	metaTypeId = findMetaType(typeName);
	                                	QMetaType metaType = new QMetaType(metaTypeId);
	    	                            if(metaTypeId==QMetaType.Type.UnknownType.value() 
	    	                            		|| !(signalType.genericType instanceof Class 
	    	                            				|| metaType.name().toString().equals(typeName))
	    	                            		|| metaType.javaType()!=signalType.type) {
	    	                                metaTypeId = registerMetaType(signalType.type, signalType.genericType, null, signalType.isPointer, signalType.isReference);
	    	                                metaType = new QMetaType(metaTypeId);
	    	                            }
	    	                            if(metaTypeId!=QMetaType.Type.UnknownType.value())
	    	                                typeName = metaType.name().toString();
	    	                            else
	    	                            	continue signalLoop;
	                                }
	                                cppTypes.add(typeName);
	                                signalParameterInfos.add(new ParameterInfo(metaTypeId, typeName));
	                            }else{
	                                cppTypes.add(new QMetaType(type).name().toString());
	                                signalParameterInfos.add(new ParameterInfo(type));
	                            }
	                        }
	                        long methodId = findEmitMethodId(fieldType, emitParameterTypes);
	                        String methodSignature = String.format("%1$s(%2$s)", declaredField.getName(), String.join(", ", cppTypes));
	                        if(!addedMethodSignatures.contains(methodSignature) && methodId!=0) {
	                        	if (!Modifier.isFinal(declaredField.getModifiers())) {
		                    		if(!Boolean.getBoolean("qtjambi.allow-nonfinal-signals") && !Boolean.getBoolean("io.qt.allow-nonfinal-signals")) {
		                    			java.util.logging.Logger.getLogger("io.qt.internal").severe(String.format("Missing modifier 'final' at signal %1$s.%2$s. Specify JVM argument -Dqtjambi.allow-nonfinal-signals=true to disable this error.", clazz.getSimpleName(), declaredField.getName()));
		                    			throw new QSignalDeclarationException(String.format("Missing modifier 'final' at signal %1$s.%2$s.", clazz.getSimpleName(), declaredField.getName()));
		                    		}
		                    	}
			                    // Rules for resetters:
			                    // 1. Zero or one argument
			                    if(signalTypes.size() <= 1){
			                        PropertyAnnotation notify = PropertyAnnotation.notifyAnnotation(declaredField);
			
			                        if (notify != null) {
			                        	for(String name : notify.name().split(",")) {
			                        		name = name.trim();
			                        		if(!name.isEmpty())
			                        			propertyNotifies.put(name, declaredField);
			                        	}
			                        }
			                    }
	                        	addedMethodSignatures.add(methodSignature);
	                        	signalIsClone.add(Boolean.FALSE);
	                        	allSignalParameterInfos.add(new ArrayList<>(signalParameterInfos));
                        		metaObjectData.signalInfos.add(new MetaObjectData.SignalInfo(declaredField, new ArrayList<>(signalTypes), fieldType, new int[signalTypes.size()], methodId));
                        		signals.put(declaredField.getName(), declaredField);
		                        Runnable addDefaultSignal = ()->{
		                        	signalTypes.remove(signalTypes.size()-1);
	                        		signalParameterInfos.remove(signalParameterInfos.size()-1);
	                        		cppTypes.remove(cppTypes.size()-1);
	                        		emitParameterTypes.remove(emitParameterTypes.size()-1);
		                        	long _methodId = findEmitMethodId(fieldType, emitParameterTypes);
	                        		String _methodSignature = String.format("%1$s(%2$s)", declaredField.getName(), String.join(", ", cppTypes));
	                        		if(!addedMethodSignatures.contains(_methodSignature) && _methodId!=0) {
	                        			addedMethodSignatures.add(_methodSignature);
	                        			signalIsClone.add(Boolean.TRUE);
	    	                        	allSignalParameterInfos.add(new ArrayList<>(signalParameterInfos));
	    	                        	metaObjectData.signalInfos.add(new MetaObjectData.SignalInfo(declaredField, new ArrayList<>(signalTypes), fieldType, new int[signalTypes.size()], _methodId));
	                        		}
		                        };
		                        switch(signalTypes.size()) {
		                        case 9:
		                        	if(QMetaObject.Emitable8.class.isAssignableFrom(fieldType)) {
		                        		addDefaultSignal.run();
		                        	}else break;
		                        case 8:
		                        	if(QMetaObject.Emitable7.class.isAssignableFrom(fieldType)) {
		                        		addDefaultSignal.run();
		                        	}else break;
		                        case 7:
		                        	if(QMetaObject.Emitable6.class.isAssignableFrom(fieldType)) {
		                        		addDefaultSignal.run();
		                        	}else break;
		                        case 6:
		                        	if(QMetaObject.Emitable5.class.isAssignableFrom(fieldType)) {
		                        		addDefaultSignal.run();
		                        	}else break;
		                        case 5:
		                        	if(QMetaObject.Emitable4.class.isAssignableFrom(fieldType)) {
		                        		addDefaultSignal.run();
		                        	}else break;
	                        	case 4:
		                        	if(QMetaObject.Emitable3.class.isAssignableFrom(fieldType)) {
		                        		addDefaultSignal.run();
		                        	}else break;
	                        	case 3:
		                        	if(QMetaObject.Emitable2.class.isAssignableFrom(fieldType)) {
		                        		addDefaultSignal.run();
		                        	}else break;
	                        	case 2:
		                        	if(QMetaObject.Emitable1.class.isAssignableFrom(fieldType)) {
		                        		addDefaultSignal.run();
		                        	}else break;
	                        	case 1:
		                        	if(QMetaObject.Emitable0.class.isAssignableFrom(fieldType)) {
		                        		addDefaultSignal.run();
		                        	}else break;
		                        }
	                        }
	                	}else {
	                		throw new QSignalDeclarationException(String.format("Modifier 'static' not allowed for signal %1$s.%2$s. Use QStaticMemberSignals instead to declare a static signal.", clazz.getSimpleName(), declaredField.getName()));
	                	}
	            	}else if(isQObject && SignalUtility.AbstractMultiSignal.class.isAssignableFrom(fieldType) && QObject.class!=fieldType.getEnclosingClass()) {
	            		if(Modifier.isStatic(declaredField.getModifiers()))
	                		throw new QSignalDeclarationException(String.format("Modifier 'static' not allowed for signal %1$s.%2$s. Use QStaticMemberSignals instead to declare a static signal.", clazz.getSimpleName(), declaredField.getName()));
            			if(declaredField.getDeclaringClass()!=fieldType.getEnclosingClass())
            				throw new QSignalDeclarationException(String.format("Declaration error at signal %1$s.%2$s: Multi signal class has to be declared in the class using it.", clazz.getSimpleName(), declaredField.getName()));
            			if(!Modifier.isFinal(fieldType.getModifiers()))
            				throw new QSignalDeclarationException(String.format("Missing modifier 'final' at signal class %1$s.", fieldType.getTypeName()));
                    	if (!Modifier.isFinal(declaredField.getModifiers()))
                			throw new QSignalDeclarationException(String.format("Missing modifier 'final' at signal %1$s.%2$s.", clazz.getSimpleName(), declaredField.getName()));
                    	Map<List<Class<?>>,SignalUtility.EmitMethodInfo> emitMethods = SignalUtility.findEmitMethods(fieldType);
        				if(emitMethods.keySet().isEmpty())
        					throw new QSignalDeclarationException(String.format("Missing modifier emit methods at signal class %1$s.", fieldType.getTypeName()));
                        for (SignalUtility.EmitMethodInfo emitMethodInfo : emitMethods.values()) {
                        	if(emitMethodInfo.methodId==0)
                        		continue;
                        	List<SignalParameterType> signalTypes = emitMethodInfo.parameterTypes;
	                    	List<String> cppTypes = new ArrayList<>();
	                        List<ParameterInfo> signalParameterInfos = new ArrayList<>();
	                        List<Class<?>> signalClassTypes = new ArrayList<>();
	                        for (int j = 0; j < signalTypes.size(); j++) {
	                            SignalUtility.SignalParameterType signalType = signalTypes.get(j);
	                            signalClassTypes.add(signalType.type);
	                            QtMetaType metaTypeDecl = signalType.annotatedType==null ? null : signalType.annotatedType.getAnnotation(QtMetaType.class);
	                            int metaTypeId = 0;
	                            String typeName;
	                            if(metaTypeDecl!=null) {
	                				if(metaTypeDecl.id()!=0) {
	                					metaTypeId = metaTypeDecl.id();
                						metaTypeId = registerRefMetaType(metaTypeId, signalType.isPointer, signalType.isReference);
	                					typeName = new QMetaType(metaTypeId).name().toString();
	                				}else if(metaTypeDecl.type()!=QMetaType.Type.UnknownType){
	                					metaTypeId = metaTypeDecl.type().value();
                						metaTypeId = registerRefMetaType(metaTypeId, signalType.isPointer, signalType.isReference);
	                					typeName = new QMetaType(metaTypeId).name().toString();
	                				}else {
	            						if(metaTypeDecl.name().isEmpty())
	            							throw new IllegalArgumentException("Incomplete @QtMetaType declaration. Either use type, id or name to specify meta type.");
	                					typeName = metaTypeDecl.name();
	                					if(signalType.isPointer && !typeName.endsWith("*")) {
	                                        typeName += "*";
	                                    }
	                                    if(signalType.isReference) {
	                                    	if(typeName.endsWith("*")) {
	                                            typeName = typeName.substring(0, typeName.length()-2);
	                                        }
	                                        if(!typeName.endsWith("&")) {
	                                            typeName += "&";
	                                        }
	                                    }
	                				}
	                			}else{
	                				typeName = internalTypeNameOfClass(signalType.type, signalType.genericType);
	                				if(signalType.isPointer) {
	                                    if(!typeName.isEmpty() && !typeName.endsWith("*")) {
	                                        typeName += "*";
	                                    }
	                                }
	                                if(signalType.isReference) {
	                                    if(typeName.endsWith("*")) {
	                                        typeName = typeName.substring(0, typeName.length()-2);
	                                    }
	                                    if(!typeName.isEmpty() && !typeName.endsWith("&")) {
	                                        typeName += "&";
	                                    }
	                                }
	                			}
	                            QMetaType.Type type = metaType(typeName);
	                            if(type==QMetaType.Type.UnknownType || type==QMetaType.Type.User){
	                                if(metaTypeId==QMetaType.Type.UnknownType.value()) {
	                                	metaTypeId = findMetaType(typeName);
	                                	QMetaType metaType = new QMetaType(metaTypeId);
	    	                            if(metaTypeId==QMetaType.Type.UnknownType.value() 
	    	                            		|| !(signalType.genericType instanceof Class 
	    	                            				|| metaType.name().toString().equals(typeName))
	    	                            		|| metaType.javaType()!=signalType.type) {
	    	                                metaTypeId = registerMetaType(signalType.type, signalType.genericType, null, signalType.isPointer, signalType.isReference);
	    	                                metaType = new QMetaType(metaTypeId);
	    	                            }
	    	                            if(metaTypeId!=QMetaType.Type.UnknownType.value())
	    	                                typeName = metaType.name().toString();
	    	                            else
	    	                            	continue signalLoop;
	                                }
	                                cppTypes.add(typeName);
	                                signalParameterInfos.add(new ParameterInfo(metaTypeId, typeName));
	                            }else{
	                                cppTypes.add(new QMetaType(type).name().toString());
	                                signalParameterInfos.add(new ParameterInfo(type));
	                            }
	                        }
	                        String methodSignature = String.format("%1$s(%2$s)", declaredField.getName(), String.join(", ", cppTypes));
	                        if(!addedMethodSignatures.contains(methodSignature)) {
	                        	addedMethodSignatures.add(methodSignature);
	                        	signalIsClone.add(Boolean.FALSE);
	                        	allSignalParameterInfos.add(new ArrayList<>(signalParameterInfos));
	                        	Class<?> _signalClass;
	                        	SignalUtility.SignalClasses signalClasses = SignalUtility.signalClasses(QObject.class);
                        		if(signalClasses==null)
                        			throw new QSignalInitializationException("Unable to resolve multi signal.");
	                        	if(Modifier.isPublic(emitMethodInfo.method.getModifiers())) {
            						_signalClass = signalClasses.getPublicSignal(emitMethodInfo.parameterTypes.size());
            					}else {
            						_signalClass = signalClasses.getPrivateSignal(emitMethodInfo.parameterTypes.size());
            					}
	                        	metaObjectData.signalInfos.add(new MetaObjectData.SignalInfo(declaredField, new ArrayList<>(signalTypes), _signalClass, new int[signalTypes.size()], emitMethodInfo.methodId));
	                        	signals.put(declaredField.getName(), declaredField);
	                        }
            			}
	            	}else{
	            		PropertyAnnotation member = PropertyAnnotation.memberAnnotation(declaredField);
	            		if(member!=null) {
	            			if(member.enabled()) {
    	            			String property = member.name();
	            				if(isQObject && isValidQProperty(declaredField)) {
	            					if (!Modifier.isFinal(declaredField.getModifiers())) {
	            						if(!Boolean.getBoolean("qtjambi.allow-nonfinal-qproperties") && !Boolean.getBoolean("io.qt.allow-nonfinal-qproperties")) {
	                            			java.util.logging.Logger.getLogger("io.qt.internal").severe(String.format("Missing modifier 'final' at property field %1$s.%2$s. Specify JVM argument -Dqtjambi.allow-nonfinal-qproperties=true to disable this error.", declaredField.getDeclaringClass().getSimpleName(), declaredField.getName()));
	                            			throw new QPropertyDeclarationException(String.format("Missing modifier 'final' at property field %1$s.%2$s.", clazz.getSimpleName(), declaredField.getName()));
	                            		}
	            					}
	    	                		propertyQPropertyFields.put(property, declaredField);
	    	                	}else {
	    	                		propertyMembers.put(property, declaredField);
	    	                	}
            					propertyDesignableResolvers.put(property, isDesignable(declaredField, clazz));
                                propertyScriptableResolvers.put(property, isScriptable(declaredField, clazz));
                                propertyEditableResolvers.put(property, isEditable(declaredField, clazz));
                                propertyStoredResolvers.put(property, isStored(declaredField, clazz));
                                propertyUserResolvers.put(property, isUser(declaredField, clazz));
                                propertyRequiredResolvers.put(property, isRequired(declaredField, clazz));
                                propertyConstantResolvers.put(property, isConstant(declaredField));
                                propertyFinalResolvers.put(property, false);
	            			}
	            		}else if(isQObject && isValidQProperty(declaredField)) {
        					if (!Modifier.isFinal(declaredField.getModifiers())) {
        						if(!Boolean.getBoolean("qtjambi.allow-nonfinal-qproperties") && !Boolean.getBoolean("io.qt.allow-nonfinal-qproperties")) {
                        			java.util.logging.Logger.getLogger("io.qt.internal").severe(String.format("Missing modifier 'final' at property field %1$s.%2$s. Specify JVM argument -Dqtjambi.allow-nonfinal-qproperties=true to disable this error.", declaredField.getDeclaringClass().getSimpleName(), declaredField.getName()));
                        			throw new QPropertyDeclarationException(String.format("Missing modifier 'final' at property field %1$s.%2$s.", clazz.getSimpleName(), declaredField.getName()));
                        		}
        					}
	            			String property = declaredField.getName();
	                		propertyQPropertyFields.put(property, declaredField);
        					propertyDesignableResolvers.put(property, isDesignable(declaredField, clazz));
                            propertyScriptableResolvers.put(property, isScriptable(declaredField, clazz));
                            propertyEditableResolvers.put(property, isEditable(declaredField, clazz));
                            propertyStoredResolvers.put(property, isStored(declaredField, clazz));
                            propertyUserResolvers.put(property, isUser(declaredField, clazz));
                            propertyRequiredResolvers.put(property, isRequired(declaredField, clazz));
                            propertyConstantResolvers.put(property, isConstant(declaredField));
                            propertyFinalResolvers.put(property, false);
	            		}else if((isQObject || isGadget) && Modifier.isPublic(declaredField.getModifiers())) {
	            			String property = declaredField.getName();
	            			if(!(Modifier.isStatic(declaredField.getModifiers()) && property.equals("staticMetaObject"))) {
		                		propertyMembers.put(property, declaredField);
	        					propertyDesignableResolvers.put(property, isDesignable(declaredField, clazz));
	                            propertyScriptableResolvers.put(property, isScriptable(declaredField, clazz));
	                            propertyEditableResolvers.put(property, isEditable(declaredField, clazz));
	                            propertyStoredResolvers.put(property, isStored(declaredField, clazz));
	                            propertyUserResolvers.put(property, isUser(declaredField, clazz));
	                            propertyRequiredResolvers.put(property, false);
	                            propertyConstantResolvers.put(property, true);
	                            propertyFinalResolvers.put(property, false);
	            			}
	                	}
	                }
	            }
            }
            
            List<List<ParameterInfo>> allConstructorParameterInfos = new ArrayList<>();
            {
            	TreeSet<Constructor<?>> declaredConstructors = new TreeSet<>((m1, m2)->{
                	return m1.toGenericString().compareTo(m2.toGenericString());
                });
                declaredConstructors.addAll(Arrays.asList(clazz.getDeclaredConstructors()));
cloop: 		    for(Constructor<?> constructor : declaredConstructors){
                    if(!constructor.isSynthetic() && constructor.isAnnotationPresent(QtInvokable.class)) {
                        Class<?>[] parameterTypes = constructor.getParameterTypes();
                        for (Class<?> parameterType : parameterTypes) {
                            if(parameterType==Classes.QPrivateConstructor()
                            		|| parameterType==Classes.QDeclarativeConstructor()) {
                                continue cloop;
                            }
                        }
                        
                    	List<String> cppTypes = new ArrayList<>();
                        List<ParameterInfo> constructorParameterInfos = new ArrayList<>();
                        Type[] genericParameterTypes = constructor.getGenericParameterTypes();
                        AnnotatedElement[] annotatedParameterTypes = null;
                        if(ClassAnalyzerUtility.useAnnotatedType) {
                        	annotatedParameterTypes = constructor.getAnnotatedParameterTypes();
                        }
                        for (int j = 0; j < parameterTypes.length; j++) {
                            boolean isPointer = false;
                            boolean isReference = false;
                            if(annotatedParameterTypes!=null && annotatedParameterTypes[j]!=null) {
	                            if(annotatedParameterTypes[j].isAnnotationPresent(QtPointerType.class)) {
	                            	isPointer = true;
	                            }
	                            QtReferenceType referenceType = annotatedParameterTypes[j].getAnnotation(QtReferenceType.class);
	                            if(referenceType!=null && !referenceType.isConst()) {
	                            	isReference = true;
	                            }
                            }
                            String typeName;
                            QtMetaType metaTypeDecl = annotatedParameterTypes==null || annotatedParameterTypes[j]==null ? null : annotatedParameterTypes[j].getAnnotation(QtMetaType.class);
                            int metaTypeId = 0;
                            if(metaTypeDecl!=null) {
                				if(metaTypeDecl.id()!=0) {
                					metaTypeId = metaTypeDecl.id();
            						metaTypeId = registerRefMetaType(metaTypeId, isPointer, isReference);
                					typeName = new QMetaType(metaTypeId).name().toString();
                				}else if(metaTypeDecl.type()!=QMetaType.Type.UnknownType){
                					metaTypeId = metaTypeDecl.type().value();
            						metaTypeId = registerRefMetaType(metaTypeId, isPointer, isReference);
                					typeName = new QMetaType(metaTypeId).name().toString();
                				}else {
            						if(metaTypeDecl.name().isEmpty())
            							throw new IllegalArgumentException("Incomplete @QtMetaType declaration. Either use type, id or name to specify meta type.");
                					typeName = metaTypeDecl.name();
                					if(isPointer && !typeName.endsWith("*")) {
                                        typeName += "*";
                                    }
                                    if(isReference) {
                                    	if(typeName.endsWith("*")) {
                                            typeName = typeName.substring(0, typeName.length()-2);
                                        }
                                        if(!typeName.endsWith("&")) {
                                            typeName += "&";
                                        }
                                    }
                				}
                			}else {
                				typeName = internalTypeNameOfClass(parameterTypes[j], genericParameterTypes[j]);
                				if(isPointer) {
                                    if(!typeName.isEmpty() && !typeName.endsWith("*")) {
                                        typeName += "*";
                                    }
                                }
                                if(isReference) {
                                    if(typeName.endsWith("*")) {
                                        typeName = typeName.substring(0, typeName.length()-2);
                                    }
                                    if(!typeName.isEmpty() && !typeName.endsWith("&")) {
                                        typeName += "&";
                                    }
                                }
                			}
                        	QMetaType.Type type = metaType(typeName);
                            if(type==QMetaType.Type.UnknownType || type==QMetaType.Type.User){
                                if(metaTypeId==QMetaType.Type.UnknownType.value()) {
    	                        	metaTypeId = findMetaType(typeName);
    	                        	QMetaType metaType = new QMetaType(metaTypeId);
    	                            if(metaTypeId==QMetaType.Type.UnknownType.value() 
    	                            		|| !(genericParameterTypes[j] instanceof Class 
    	                            				|| new QMetaType(metaTypeId).name().toString().equals(typeName))
    	                            		|| metaType.javaType()!=parameterTypes[j]) {
    	                                metaTypeId = registerMetaType(parameterTypes[j], 
    	                                        genericParameterTypes[j],
    	                                        annotatedParameterTypes!=null ? annotatedParameterTypes[j] : null,
    	                                        isPointer,
    	                                        isReference);
    	                                metaType = new QMetaType(metaTypeId);
    	                            }
    	                            if(metaTypeId!=QMetaType.Type.UnknownType.value())
    	                                typeName = metaType.name().toString();
    	                            else continue cloop;
                                }
                                cppTypes.add(typeName);
                                constructorParameterInfos.add(new ParameterInfo(metaTypeId, typeName));
                            }else{
                                cppTypes.add(new QMetaType(type).name().toString());
                                constructorParameterInfos.add(new ParameterInfo(type));
                            }
                        }
                        
                        String name = constructor.getName();
                        String methodSignature = String.format("%1$s(%2$s)", name, String.join(", ", cppTypes));
                        if(!addedMethodSignatures.contains(methodSignature)) {
                        	addedMethodSignatures.add(methodSignature);
	                        allConstructorParameterInfos.add(constructorParameterInfos);
	                        metaObjectData.constructors.add(constructor);
	                        metaObjectData.constructorMetaTypes.add(new MetaObjectData.MetaTypeInfo[parameterTypes.length+1]);
	                        metaObjectData.hasStaticMembers = true;
                        }
                    }
                }
            }
            
            List<List<ParameterInfo>> allMethodParameterInfos = new ArrayList<>();
            List<Method> possibleBindables = Collections.emptyList();
            TreeSet<Method> declaredMethods = new TreeSet<>((m1, m2)->{
            	return m1.toGenericString().compareTo(m2.toGenericString());
            });
            declaredMethods.addAll(Arrays.asList(clazz.getDeclaredMethods()));
            List<QPair<String,Method>> possibleReaders = Collections.emptyList();
            Set<Method> usedGetters = new HashSet<>();
            for (Method declaredMethod : declaredMethods) {
                if(declaredMethod.isSynthetic() 
                        || declaredMethod.isBridge()) {
                    continue;
                }
                if (
                        (
                                (
                                        (isQObject || isGadget)
                                        && !declaredMethod.isAnnotationPresent(QtUninvokable.class) 
                                        && !Modifier.isStatic(declaredMethod.getModifiers())
                                        && (declaredMethod.getReturnType()==void.class || Modifier.isPublic(declaredMethod.getModifiers()))
                                        && !(declaredMethod.getParameterCount()==0 && "clone".equals(declaredMethod.getName()))
                                ) || (
                                        declaredMethod.isAnnotationPresent(QtInvokable.class) 
                                        && declaredMethod.getAnnotation(QtInvokable.class).value()
                                )
                        )
                        && !overridesGeneratedSlot(declaredMethod, clazz)
                    ) {
                	List<ParameterInfo> methodParameterInfos = new ArrayList<>();
                	boolean isPointer = false;
                    boolean isReference = false;
                    if(ClassAnalyzerUtility.useAnnotatedType
                    		&& (declaredMethod.getAnnotatedReturnType().isAnnotationPresent(QtPointerType.class)
                            		|| declaredMethod.isAnnotationPresent(QtPointerType.class))) {
                    	isPointer = true;
                    }
                    QtReferenceType referenceType = null;
                    if(ClassAnalyzerUtility.useAnnotatedType) {
                    	if(declaredMethod.getAnnotatedReturnType()!=null)
                    		referenceType = declaredMethod.getAnnotatedReturnType().getAnnotation(QtReferenceType.class);
                    }
                    if(referenceType==null)
                    	referenceType = declaredMethod.getAnnotation(QtReferenceType.class);
                    if(referenceType!=null && !referenceType.isConst()) {
                    	isReference = true;
                    }
                    QtMetaType metaTypeDecl = null;
                    if(ClassAnalyzerUtility.useAnnotatedType) {
                    	if(declaredMethod.getAnnotatedReturnType()!=null)
                    		metaTypeDecl = declaredMethod.getAnnotatedReturnType().getAnnotation(QtMetaType.class);
                    }
                    int metaTypeId = 0;
                    String typeName;
                    if(metaTypeDecl!=null) {
        				if(metaTypeDecl.id()!=0) {
        					metaTypeId = metaTypeDecl.id();
    						metaTypeId = registerRefMetaType(metaTypeId, isPointer, isReference);
        					typeName = new QMetaType(metaTypeId).name().toString();
        				}else if(metaTypeDecl.type()!=QMetaType.Type.UnknownType){
        					metaTypeId = metaTypeDecl.type().value();
    						metaTypeId = registerRefMetaType(metaTypeId, isPointer, isReference);
        					typeName = new QMetaType(metaTypeId).name().toString();
        				}else {
    						if(metaTypeDecl.name().isEmpty())
    							throw new IllegalArgumentException("Incomplete @QtMetaType declaration. Either use type, id or name to specify meta type.");
        					typeName = metaTypeDecl.name();
        					if(isPointer && !typeName.endsWith("*")) {
                                typeName += "*";
                            }
                            if(isReference) {
                            	if(typeName.endsWith("*")) {
                                    typeName = typeName.substring(0, typeName.length()-2);
                                }
                                if(!typeName.endsWith("&")) {
                                    typeName += "&";
                                }
                            }
        				}
        			}else {
        				typeName = internalTypeNameOfClass(declaredMethod.getReturnType(), declaredMethod.getGenericReturnType());
        				if(isPointer) {
                            if(!typeName.isEmpty() && !typeName.endsWith("*")) {
                                typeName += "*";
                            }
                        }
                        if(isReference) {
                            if(typeName.endsWith("*")) {
                                typeName = typeName.substring(0, typeName.length()-2);
                            }
                            if(!typeName.isEmpty() && !typeName.endsWith("&")) {
                                typeName += "&";
                            }
                        }
        			}
                    QMetaType.Type type = metaType(typeName);
                    if(type==QMetaType.Type.UnknownType || type==QMetaType.Type.User){
                    	if(metaTypeId==QMetaType.Type.UnknownType.value()) {
	                    	metaTypeId = findMetaType(typeName);
	                    	QMetaType metaType = new QMetaType(metaTypeId);
	                        if(metaTypeId==QMetaType.Type.UnknownType.value() 
	                        		|| !(declaredMethod.getGenericReturnType() instanceof Class 
	                        				|| metaType.name().toString().equals(typeName))
	                        		|| metaType.javaType()!=declaredMethod.getReturnType()) {
	                        	AnnotatedElement ae = null;
	                        	if(ClassAnalyzerUtility.useAnnotatedType)
	                        		ae = declaredMethod.getAnnotatedReturnType();
	                        	metaTypeId = registerMetaType(
	                                    declaredMethod.getReturnType(), 
	                                    declaredMethod.getGenericReturnType(), 
	                                    ae,
	                                    isPointer,
	                                    isReference);
	                        	metaType = new QMetaType(metaTypeId);
	                        }
	                        if(metaTypeId!=QMetaType.Type.UnknownType.value())
	                            typeName = metaType.name().toString();
                    	}
                    	methodParameterInfos.add(new ParameterInfo(metaTypeId, typeName));
                    }else{
                    	methodParameterInfos.add(new ParameterInfo(type));
                    }
                    
                    List<String> cppTypes = new ArrayList<>();
                    Class<?>[] parameterTypes = declaredMethod.getParameterTypes();
                    Type[] genericParameterTypes = declaredMethod.getGenericParameterTypes();
                    AnnotatedElement[] annotatedParameterTypes = null;
                    if(ClassAnalyzerUtility.useAnnotatedType) {
                    	annotatedParameterTypes = declaredMethod.getAnnotatedParameterTypes();
                    }
                    for (int j = 0; j < parameterTypes.length; j++) {
                    	metaTypeId = 0;
                        isPointer = false;
                        isReference = false;
                        if(annotatedParameterTypes!=null && annotatedParameterTypes[j]!=null) {
	                        if(annotatedParameterTypes[j].isAnnotationPresent(QtPointerType.class)) {
	                        	isPointer = true;
	                        	
	                        }
	                        referenceType = annotatedParameterTypes[j].getAnnotation(QtReferenceType.class);
	                        if(referenceType!=null && !referenceType.isConst()) {
	                        	isReference = true;
	                        }
	                        metaTypeDecl = annotatedParameterTypes[j].getAnnotation(QtMetaType.class);
                        }
                    	if(metaTypeDecl!=null) {
            				if(metaTypeDecl.id()!=0) {
            					metaTypeId = metaTypeDecl.id();
            					if(isPointer || isReference) {
            						metaTypeId = registerRefMetaType(metaTypeId, isPointer, isReference);
            					}
            					typeName = new QMetaType(metaTypeId).name().toString();
            				}else if(metaTypeDecl.type()!=QMetaType.Type.UnknownType){
            					metaTypeId = metaTypeDecl.type().value();
            					if(isPointer || isReference) {
            						metaTypeId = registerRefMetaType(metaTypeId, isPointer, isReference);
            					}
            					typeName = new QMetaType(metaTypeId).name().toString();
            				}else {
        						if(metaTypeDecl.name().isEmpty())
        							throw new IllegalArgumentException("Incomplete @QtMetaType declaration. Either use type, id or name to specify meta type.");
            					typeName = metaTypeDecl.name();
            					if(isPointer && !typeName.endsWith("*")) {
                                    typeName += "*";
                                }
                                if(isReference) {
                                	if(typeName.endsWith("*")) {
                                        typeName = typeName.substring(0, typeName.length()-2);
                                    }
                                    if(!typeName.endsWith("&")) {
                                        typeName += "&";
                                    }
                                }
            				}
            			}else {
            				typeName = internalTypeNameOfClass(parameterTypes[j], genericParameterTypes[j]);
            				if(isPointer) {
                                if(!typeName.isEmpty() && !typeName.endsWith("*")) {
                                    typeName += "*";
                                }
                            }
                            if(isReference) {
                                if(typeName.endsWith("*")) {
                                    typeName = typeName.substring(0, typeName.length()-2);
                                }
                                if(!typeName.isEmpty() && !typeName.endsWith("&")) {
                                    typeName += "&";
                                }
                            }
            			}
                        type = metaType(typeName);
                        if(type==QMetaType.Type.UnknownType || type==QMetaType.Type.User){
                            if(metaTypeId==QMetaType.Type.UnknownType.value()) {
	                        	metaTypeId = findMetaType(typeName);
	                        	QMetaType metaType = new QMetaType(metaTypeId);
	                            if(metaTypeId==QMetaType.Type.UnknownType.value() 
	                            		|| !(genericParameterTypes[j] instanceof Class 
	                            				|| metaType.name().toString().equals(typeName))
	                            		|| metaType.javaType()!=parameterTypes[j]) {
	                                metaTypeId = registerMetaType(parameterTypes[j], 
	                                        genericParameterTypes[j],
	                                        annotatedParameterTypes==null ? null : annotatedParameterTypes[j],
	                                        isPointer,
	                                        isReference);
	                                metaType = new QMetaType(metaTypeId);
	                            }
	                            if(metaTypeId!=QMetaType.Type.UnknownType.value())
	                                typeName = metaType.name().toString();
                            }
                            cppTypes.add(typeName);
                            methodParameterInfos.add(new ParameterInfo(metaTypeId, typeName));
                        }else{
                            cppTypes.add(new QMetaType(type).name().toString());
                        	methodParameterInfos.add(new ParameterInfo(type));
                        }
                    }
                    String methodSignature = String.format("%1$s(%2$s)", declaredMethod.getName(), String.join(", ", cppTypes));
                    if(!addedMethodSignatures.contains(methodSignature)) {
                    	allMethodParameterInfos.add(methodParameterInfos);
                    	metaObjectData.methods.add(declaredMethod);
                        if(Modifier.isStatic(declaredMethod.getModifiers())){
                            methodFlags.put(declaredMethod, MethodFlags.MethodMethod);
                            metaObjectData.hasStaticMembers = true;
                        }else{
                            if(!isQObject) {
                                // we need to make sure that static_metacall is set
                                metaObjectData.hasStaticMembers = true;
                            	methodFlags.put(declaredMethod, MethodFlags.MethodMethod);
                            }else {
                            	methodFlags.put(declaredMethod, MethodFlags.MethodSlot);
                            }
                        }
                        metaObjectData.methodMetaTypes.add(new MetaObjectData.MetaTypeInfo[declaredMethod.getParameterCount()+1]);
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
                            && isValidGetter(declaredMethod)) {

                        String name = reader.name();
                        // If the return type of the property reader is not registered, then
                        // we need to register the owner class in the meta object (in which case
                        // it has to be a QObject)
                        Class<?> returnType = declaredMethod.getReturnType();

                        if ( (QFlags.class.isAssignableFrom(returnType) || Enum.class.isAssignableFrom(returnType))
                                && !isEnumAllowedForProperty(returnType) ) {
                            int type = QMetaType.qRegisterMetaType(returnType);
                            if(type==QMetaType.Type.UnknownType.value()) {
                                System.err.println("Problem in property '" + name + "' in '" + clazz.getName()
                                                   + "' with return type '"+returnType.getName()
                                                   +"': Unable to register meta type.");
                                continue;
                            }
                        }
                        Field signalField = signals.get(name);
                        if(signalField!=null && SignalUtility.AbstractSignal.class.isAssignableFrom(returnType))
                        	continue;

                        propertyReaders.put(name, declaredMethod);
                        usedGetters.add(declaredMethod);
                        propertyDesignableResolvers.put(name, isDesignable(declaredMethod, clazz));
                                                                                                   
                        propertyScriptableResolvers.put(name, isScriptable(declaredMethod, clazz));
                        propertyEditableResolvers.put(name, isEditable(declaredMethod, clazz));
                        propertyStoredResolvers.put(name, isStored(declaredMethod, clazz));
                        propertyUserResolvers.put(name, isUser(declaredMethod, clazz));
                        propertyRequiredResolvers.put(name, isRequired(declaredMethod, clazz));
                        propertyConstantResolvers.put(name, isConstant(declaredMethod));
                        propertyFinalResolvers.put(name, isFinal(declaredMethod));
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
                        propertyWriters.computeIfAbsent(writer.name(), arrayListFactory()).add(declaredMethod);
                    }
                }

                // Check naming convention by looking for setXxx patterns, but only if it hasn't already been
                // annotated as a writer
                if ((isQObject || isGadget)
                    && writer == null
                    && reader == null // reader can't be a writer, cause the signature doesn't match, just an optimization
                    ) {
            		if(declaredMethodName.startsWith("set")
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
                            Method readerMethod = findPropertyReader(getDeclaredMethod(clazz, propertyName), propertyName, paramType);
                            if (readerMethod == null)
                                readerMethod = findPropertyReader(getDeclaredMethod(clazz, "get" + capitalizeFirst(propertyName)), propertyName, paramType);
                            if (readerMethod == null && isBoolean(paramType))
                                readerMethod = findPropertyReader(getDeclaredMethod(clazz, "is" + capitalizeFirst(propertyName)), propertyName, paramType);
                            if (readerMethod == null && isBoolean(paramType))
                                readerMethod = findPropertyReader(getDeclaredMethod(clazz, "has" + capitalizeFirst(propertyName)), propertyName, paramType);

                            if (readerMethod != null) { // yay
                                reader = PropertyAnnotation.readerAnnotation(readerMethod);
                                if (reader == null) {
                                	usedGetters.add(readerMethod);
                                    propertyReaders.put(propertyName, readerMethod);
                                    propertyWriters.computeIfAbsent(propertyName, arrayListFactory()).add(declaredMethod);

                                    propertyDesignableResolvers.put(propertyName, isDesignable(readerMethod, clazz));
                                    propertyScriptableResolvers.put(propertyName, isScriptable(readerMethod, clazz));
                                    propertyUserResolvers.put(propertyName, isUser(readerMethod, clazz));
                                    propertyRequiredResolvers.put(propertyName, isRequired(readerMethod, clazz));
                                }
                            }
                        }
            		}else if(isValidGetter(declaredMethod)){
            			int offset = 3;
            			if((   declaredMethodName.startsWith("get") 
            				|| (isBoolean(declaredMethod.getReturnType()) && declaredMethodName.startsWith("has"))
            				|| (isBoolean(declaredMethod.getReturnType()) && (--offset==2) && declaredMethodName.startsWith("is"))
            			   ) && declaredMethodName.length() > offset
                             && Character.isUpperCase(declaredMethodName.charAt(offset))) {
	            			String propertyName = Character.toLowerCase(declaredMethodName.charAt(offset))
	                                + declaredMethodName.substring(offset+1);
	            			if(possibleReaders.isEmpty())
	            				possibleReaders = new ArrayList<>();
	            			possibleReaders.add(new QPair<>(propertyName, declaredMethod));
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
            
            for(QPair<String,Method> pair : possibleReaders) {
            	if (!propertyReaders.containsKey(pair.first) && !usedGetters.contains(pair.second)) {
            		Field signalField = signals.get(pair.first);
                    if(signalField!=null && SignalUtility.AbstractSignal.class.isAssignableFrom(pair.second.getReturnType()))
                    	continue;
            		propertyReaders.put(pair.first, pair.second);
            		propertyDesignableResolvers.put(pair.first, isDesignable(pair.second, clazz));
                    propertyScriptableResolvers.put(pair.first, isScriptable(pair.second, clazz));
                    propertyUserResolvers.put(pair.first, isUser(pair.second, clazz));
                    propertyRequiredResolvers.put(pair.first, isRequired(pair.second, clazz));
            	}
            }
            
            for(Method possibleBindable : possibleBindables) {
            	String propertyName = possibleBindable.getName();
            	if(propertyName.startsWith("bindable") && propertyName.length() > 8) {
            		propertyName = PropertyAnnotation.removeAndLowercaseFirst(propertyName, 8);
            		if (propertyReaders.containsKey(propertyName) || propertyWriters.containsKey(propertyName)) {
            			propertyBindables.put(propertyName, possibleBindable);
            		}else {
            			QPropertyTypeInfo typeInfo = getQPropertyTypeInfo(possibleBindable);
            			if(typeInfo!=null) {
	            			Class<?> paramType = typeInfo.propertyType;
	            			Method readerMethod = findPropertyReader(getDeclaredMethod(clazz, propertyName), propertyName, paramType);
	                        if (readerMethod == null)
	                            readerMethod = findPropertyReader(getDeclaredMethod(clazz, "get" + capitalizeFirst(propertyName)), propertyName, paramType);
	                        if (readerMethod == null && isBoolean(paramType))
	                            readerMethod = findPropertyReader(getDeclaredMethod(clazz, "is" + capitalizeFirst(propertyName)), propertyName, paramType);
	                        if (readerMethod == null && isBoolean(paramType))
	                            readerMethod = findPropertyReader(getDeclaredMethod(clazz, "has" + capitalizeFirst(propertyName)), propertyName, paramType);
	
	                        if (readerMethod != null) { // yay
	                            propertyReaders.put(propertyName, readerMethod);
	                            propertyBindables.put(propertyName, possibleBindable);
	
	                            propertyDesignableResolvers.put(propertyName, isDesignable(readerMethod, clazz));
	                            propertyScriptableResolvers.put(propertyName, isScriptable(readerMethod, clazz));
	                            propertyUserResolvers.put(propertyName, isUser(readerMethod, clazz));
	                            propertyRequiredResolvers.put(propertyName, isRequired(readerMethod, clazz));
	                        }else {
	                        	Method writerMethod = findPropertyWriter(getDeclaredMethod(clazz, "set"+propertyName.toUpperCase().charAt(0)+propertyName.substring(1), paramType), propertyName, paramType);
	                        	if(writerMethod != null) {
	                        		PropertyAnnotation writer = PropertyAnnotation.writerAnnotation(readerMethod);
	                                if (writer == null) {
	                                	propertyWriters.computeIfAbsent(propertyName, arrayListFactory()).add(writerMethod);
	                                	propertyDesignableResolvers.put(propertyName, isDesignable(writerMethod, clazz));
	    	                            propertyScriptableResolvers.put(propertyName, isScriptable(writerMethod, clazz));
	    	                            propertyUserResolvers.put(propertyName, isUser(writerMethod, clazz));
	    	                            propertyRequiredResolvers.put(propertyName, isRequired(writerMethod, clazz));
	                                }
	                        	}
	                        }
            			}
            		}
            	}
            }
            
            for(MetaObjectData.SignalInfo signalInfo : metaObjectData.signalInfos) {
            	if(signalInfo.field.getName().endsWith("Changed")) {
            		String propertyName = signalInfo.field.getName();
            		propertyName = propertyName.substring(0, propertyName.length()-7);
            		if (!propertyReaders.containsKey(propertyName)) {
            			Class<?> paramType = signalInfo.signalTypes.size()==1 ? signalInfo.signalTypes.get(0).type : null;
            			Method readerMethod = findPropertyReader(getDeclaredMethod(clazz, propertyName), propertyName, paramType);
                        if (readerMethod == null)
                            readerMethod = findPropertyReader(getDeclaredMethod(clazz, "get" + capitalizeFirst(propertyName)), propertyName, paramType);
                        if (readerMethod == null && isBoolean(paramType))
                            readerMethod = findPropertyReader(getDeclaredMethod(clazz, "is" + capitalizeFirst(propertyName)), propertyName, paramType);
                        if (readerMethod == null && isBoolean(paramType))
                            readerMethod = findPropertyReader(getDeclaredMethod(clazz, "has" + capitalizeFirst(propertyName)), propertyName, paramType);

                        if (readerMethod != null) { // yay
                            propertyReaders.put(propertyName, readerMethod);
                            propertyNotifies.put(propertyName, signalInfo.field);

                            propertyDesignableResolvers.put(propertyName, isDesignable(readerMethod, clazz));
                            propertyScriptableResolvers.put(propertyName, isScriptable(readerMethod, clazz));
                            propertyUserResolvers.put(propertyName, isUser(readerMethod, clazz));
                            propertyRequiredResolvers.put(propertyName, isRequired(readerMethod, clazz));
                        }else {
                        	Method writerMethod;
                        	if(paramType==null) {
                        		
                        	}else{
                        		
                        	}
                        	writerMethod = findPropertyWriter(getDeclaredMethod(clazz, "set"+propertyName.toUpperCase().charAt(0)+propertyName.substring(1), paramType), propertyName, paramType);
                        	if(writerMethod != null) {
                        		PropertyAnnotation writer = PropertyAnnotation.writerAnnotation(readerMethod);
                                if (writer == null) {
                                	propertyWriters.computeIfAbsent(propertyName, arrayListFactory()).add(writerMethod);
                                	propertyDesignableResolvers.put(propertyName, isDesignable(writerMethod, clazz));
    	                            propertyScriptableResolvers.put(propertyName, isScriptable(writerMethod, clazz));
    	                            propertyUserResolvers.put(propertyName, isUser(writerMethod, clazz));
    	                            propertyRequiredResolvers.put(propertyName, isRequired(writerMethod, clazz));
                                }
                        	}
                        }
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
	        		PropertyAnnotation member;
	            	if(!propertyReaders.containsKey(entry.getKey()) 
	            			&& !propertyReaders.containsKey(name)
	            			&& !Modifier.isStatic(entry.getValue().getModifiers())
	            			&& Modifier.isFinal(entry.getValue().getModifiers())
	            			&& (Modifier.isPublic(entry.getValue().getModifiers())
	            					|| ((member = PropertyAnnotation.memberAnnotation(entry.getValue()))!=null && member.enabled()))) {
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
            		for(MetaObjectData.SignalInfo signalInfo : metaObjectData.signalInfos) {
            			if(signalInfo.field.getName().equals(property+"Changed")) {
                			if(signalInfo.signalTypes.isEmpty()) {
                				propertyNotifies.put(property, signalInfo.field);
                			}else {
                				if(propertyReaders.get(property)!=null) {
                					if(signalInfo.signalTypes.get(0).type.isAssignableFrom(getBoxedType(propertyReaders.get(property).getReturnType()))) {
                        				propertyNotifies.put(property, signalInfo.field);
                        			}
                    			}else if(propertyMembers.get(property)!=null) {
                    				Field propertyField = propertyMembers.get(property);
                    				if(isValidQProperty(propertyField)) {
                    					MetaObjectUtility.QPropertyTypeInfo pinfo = getQPropertyTypeInfo(propertyField);
                    					if(signalInfo.signalTypes.get(0).type.isAssignableFrom(getBoxedType(pinfo.propertyType))) {
                            				propertyNotifies.put(property, signalInfo.field);
                            			}
                    				}else if(signalInfo.signalTypes.get(0).type.isAssignableFrom(getBoxedType(propertyField.getType()))) {
                        				propertyNotifies.put(property, signalInfo.field);
                        			}
                    			}else if(propertyQPropertyFields.get(property)!=null) {
                    				MetaObjectUtility.QPropertyTypeInfo pinfo = getQPropertyTypeInfo(propertyQPropertyFields.get(property));
                					if(signalInfo.signalTypes.get(0).type.isAssignableFrom(getBoxedType(pinfo.propertyType))) {
                        				propertyNotifies.put(property, signalInfo.field);
                        			}
                    			}
                			}
                			break;
                		}
            		}
            	}
            }
            
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
                metaObjectData.intData.add(resolveMetaDataRevision());		intdataComments.add("revision");
                // classname
                metaObjectData.intData.add(metaObjectData.addStringDataAndReturnIndex(classname));		intdataComments.add("className");
                // classinfo
                metaObjectData.intData.add(classInfos.size());		intdataComments.add("classInfoCount");
                metaObjectData.intData.add(classInfos.isEmpty() ? 0 : MO_HEADER_LEN);		intdataComments.add("classInfoData");
                
                // methods
                int methodCount = metaObjectData.signalInfos.size() + metaObjectData.methods.size();
                metaObjectData.intData.add(methodCount);		intdataComments.add("methodCount");
                final int METHOD_METADATA_INDEX = metaObjectData.intData.size();
                metaObjectData.intData.add(0);		intdataComments.add("methodData");
                
                // properties
                metaObjectData.intData.add(propertyReaders.size());		intdataComments.add("propertyCount");
                final int PROPERTY_METADATA_INDEX = metaObjectData.intData.size();
                metaObjectData.intData.add(0);		intdataComments.add("propertyData");
                
                // enums/sets
                metaObjectData.intData.add(enums.size());		intdataComments.add("enumeratorCount");
                final int ENUM_METADATA_INDEX = metaObjectData.intData.size();
                metaObjectData.intData.add(0);		intdataComments.add("enumeratorData");
                
                // constructors
                metaObjectData.intData.add(!metaObjectData.constructors.isEmpty() ? metaObjectData.constructors.size() : 0);		intdataComments.add("constructorCount");
                final int CONSTRUCTOR_METADATA_INDEX = metaObjectData.intData.size();
                metaObjectData.intData.add(0);		intdataComments.add("constructorData");
                
                // flags
                flagsIndex = metaObjectData.intData.size();
                metaObjectData.intData.add(0);		intdataComments.add("flags");
                
                // signalCount
                metaObjectData.intData.add(metaObjectData.signalInfos.size());		intdataComments.add("signalCount");
                
                //
                // Build classinfo array
                //
                for(Map.Entry<String,String> entry : classInfos.entrySet()){
                    // classinfo: key, value
                    metaObjectData.intData.add(metaObjectData.addStringDataAndReturnIndex(entry.getKey()));		intdataComments.add("classinfo: key");
                    metaObjectData.intData.add(metaObjectData.addStringDataAndReturnIndex(entry.getValue()));		intdataComments.add("classinfo: value");
                }
                
                HashMap<Object,Integer> paramIndexOfMethods = new HashMap<Object,Integer>();
                HashMap<Field,Integer> signalIndexes = new HashMap<>();
                
                //
                // Build signals array first, otherwise the signal indices would be wrong
                //
                if(metaObjectData.signalInfos.size() + metaObjectData.methods.size() > 0){

                    metaObjectData.intData.set(METHOD_METADATA_INDEX, metaObjectData.intData.size());
                    
                    for (int i = 0; i < metaObjectData.signalInfos.size(); ++i) {
                    	MetaObjectData.SignalInfo signalInfo = metaObjectData.signalInfos.get(i);
                        if(!signalIndexes.containsKey(signalInfo.field))
                        	signalIndexes.put(signalInfo.field, i);
                        MethodAttributes flags = MethodFlags.MethodSignal.asFlags();
                        if (Modifier.isPrivate(signalInfo.field.getModifiers()))
                            flags.set(MethodFlags.AccessPrivate);
                        else if (Modifier.isPublic(signalInfo.field.getModifiers()))
                            flags.set(MethodFlags.AccessPublic);
                        else
                            flags.set(MethodFlags.AccessProtected);
                        
                        if (signalInfo.field.isAnnotationPresent(QtInvokable.class))
                            flags.set(MethodFlags.MethodScriptable);
                        if(Boolean.TRUE.equals(signalIsClone.get(i)))
                        	flags.set(MethodFlags.MethodCloned);
                        int argc = signalInfo.signalTypes.size();
                        
                        // signals: name, argc, parameters, tag, flags, initial metatype offsets
                        metaObjectData.intData.add(metaObjectData.addStringDataAndReturnIndex(signalInfo.field.getName()));		intdataComments.add("signal["+i+"]: name");
                        metaObjectData.intData.add(argc);		intdataComments.add("signal["+i+"]: argc");
                    	paramIndexOfMethods.put(new QPair<>(signalInfo.field, argc), metaObjectData.intData.size());
                        metaObjectData.intData.add(0);		intdataComments.add("signal["+i+"]: parameters");
                        metaObjectData.intData.add(metaObjectData.addStringDataAndReturnIndex(""));		intdataComments.add("signal["+i+"]: tag");
                        metaObjectData.intData.add(flags.value());		intdataComments.add("signal["+i+"]: flags");
                        if(isQt6) {
                            metaObjectData.intData.add(0);		intdataComments.add("signal["+i+"]: initial metatype offsets");
                        }
                    }
                    
                    //
                    // Build method array
                    //
                    for (int i = 0; i < metaObjectData.methods.size(); i++) {
                        Method method = metaObjectData.methods.get(i);
                        MethodAttributes flags = methodFlags.get(method).asFlags();
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
                        metaObjectData.intData.add(metaObjectData.addStringDataAndReturnIndex(method.getName()));
                        intdataComments.add("slot["+i+"]: name");
                        metaObjectData.intData.add(argc);
                        intdataComments.add("slot["+i+"]: argc");
                        paramIndexOfMethods.put(method, metaObjectData.intData.size());
                        metaObjectData.intData.add(0);
                        intdataComments.add("slot["+i+"]: parameters");
                        metaObjectData.intData.add(metaObjectData.addStringDataAndReturnIndex(""));
                        intdataComments.add("slot["+i+"]: tag");
                        metaObjectData.intData.add(flags.value());
                        intdataComments.add("slot["+i+"]: flags");
                        if(isQt6) {
                            metaObjectData.intData.add(0);		intdataComments.add("slot["+i+"]: initial metatype offsets");
                        }
                    }
                }
                
                //
                // Build method parameters array
                //
                for(int i=0; i<propertyReaders.size(); ++i) {
                    metaObjectData.metaTypes.add(0);
                }
                
                for (int i = 0; i < metaObjectData.signalInfos.size(); ++i) {
                	MetaObjectData.SignalInfo signalInfo = metaObjectData.signalInfos.get(i);
                	List<ParameterInfo> signalParameterInfos = allSignalParameterInfos.get(i);
                    // signals: parameters
                    int METHOD_PARAMETER_INDEX = paramIndexOfMethods.get(new QPair<>(signalInfo.field, signalInfo.signalMetaTypes.length));
                    metaObjectData.intData.set(METHOD_PARAMETER_INDEX, metaObjectData.intData.size());
                    if(isQt6) {
                        metaObjectData.intData.set(METHOD_PARAMETER_INDEX+3, metaObjectData.metaTypes.size());
                    }
                    metaObjectData.intData.add(QMetaType.Type.Void.value());		intdataComments.add("signal["+i+"].returnType");
                    metaObjectData.metaTypes.add(QMetaType.Type.Void.value());
                    for (int j = 0; j < signalParameterInfos.size(); j++) {
                    	ParameterInfo info = signalParameterInfos.get(j);
                        if(info.type==null){
                        	signalInfo.signalMetaTypes[j] = info.metaTypeId;
                            metaObjectData.intData.add(0x80000000 | metaObjectData.addStringDataAndReturnIndex(info.typeName));
                            metaObjectData.metaTypes.add(info.metaTypeId);
                        }else{
                        	signalInfo.signalMetaTypes[j] = info.type.value();
                        	metaObjectData.intData.add(info.type.value());
                            metaObjectData.metaTypes.add(info.type.value());
                        }
                        intdataComments.add("signal["+i+"]: parameter["+j+"].arg");
                    }
                    for (int j = 0; j < signalParameterInfos.size(); j++) {
                        metaObjectData.intData.add(metaObjectData.addStringDataAndReturnIndex("arg__"+(j+1)));		intdataComments.add("signal["+i+"]: parameter["+j+"].argName");
                    }
                }
                
                //
                // Build constructors array
                //
                
                if(!metaObjectData.constructors.isEmpty()){
                    metaObjectData.intData.set(CONSTRUCTOR_METADATA_INDEX, metaObjectData.intData.size());
                    for (int i = 0; i < metaObjectData.constructors.size(); i++) {
                        Constructor<?> constructor = metaObjectData.constructors.get(i);
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
                        if(className.contains("$")){
                            className = className.substring(className.lastIndexOf('$')+1);
                        }
                        metaObjectData.intData.add(metaObjectData.addStringDataAndReturnIndex(className));		intdataComments.add("constructor["+i+"]: name");
                        metaObjectData.intData.add(argc);		intdataComments.add("constructor["+i+"]: argc");
                        paramIndexOfMethods.put(constructor, metaObjectData.intData.size());
                        metaObjectData.intData.add(0);		intdataComments.add("constructor["+i+"]: parameters");
                        metaObjectData.intData.add(metaObjectData.addStringDataAndReturnIndex(""));		intdataComments.add("constructor["+i+"]: tag");
                        metaObjectData.intData.add(flags.value());		intdataComments.add("constructor["+i+"]: flags");
                        if(isQt6) {
                            metaObjectData.intData.add(0);		intdataComments.add("slot["+i+"]: initial metatype offsets");
                        }
                    }
                }
                
                for (int i = 0; i < metaObjectData.methods.size(); i++) {
                    Method method = metaObjectData.methods.get(i);
                    MetaObjectData.MetaTypeInfo[] metaTypes = metaObjectData.methodMetaTypes.get(i);
                    List<ParameterInfo> methodParameterInfos = allMethodParameterInfos.get(i);
                    // slot/method: parameters
                    int METHOD_PARAMETER_INDEX = paramIndexOfMethods.get(method);
                    metaObjectData.intData.set(METHOD_PARAMETER_INDEX, metaObjectData.intData.size());
                    if(isQt6) {
                        metaObjectData.intData.set(METHOD_PARAMETER_INDEX+3, metaObjectData.metaTypes.size());
                    }
                    ParameterInfo info = methodParameterInfos.get(0);
                    if(info.type==null){
                        metaObjectData.intData.add(0x80000000 | metaObjectData.addStringDataAndReturnIndex(info.typeName));
                        metaObjectData.metaTypes.add(info.metaTypeId);
                        metaTypes[0] = new MetaObjectData.MetaTypeInfo(info.metaTypeId, info.typeName);
                    }else{
                        metaObjectData.intData.add(info.type.value());
                        metaObjectData.metaTypes.add(info.type.value());
                        metaTypes[0] = new MetaObjectData.MetaTypeInfo(info.type.value(), info.typeName);
                    }
                    intdataComments.add("slot["+i+"].returnType");
                    for (int j = 1; j < methodParameterInfos.size(); j++) {
                    	info = methodParameterInfos.get(j);
                        if(info.type==null){
                            metaObjectData.intData.add(0x80000000 | metaObjectData.addStringDataAndReturnIndex(info.typeName));
                            metaObjectData.metaTypes.add(info.metaTypeId);
                            metaTypes[j] = new MetaObjectData.MetaTypeInfo(info.metaTypeId, info.typeName);
                        }else{
                            metaObjectData.intData.add(info.type.value());
                            metaObjectData.metaTypes.add(info.type.value());
                            metaTypes[j] = new MetaObjectData.MetaTypeInfo(info.type.value(), info.typeName);
                        }
                        intdataComments.add("slot["+i+"]: parameter["+(j-1)+"].arg");
                    }
                    Parameter[] parameters = method.getParameters();
                    for (int j = 0; j < parameters.length; j++) {
                        if(parameters[j].isNamePresent()) {
                            metaObjectData.intData.add(metaObjectData.addStringDataAndReturnIndex(parameters[j].getName()));
                        }else {
                            metaObjectData.intData.add(metaObjectData.addStringDataAndReturnIndex("arg__"+(j+1)));
                        }
                        intdataComments.add("slot["+i+"]: parameter["+j+"].argName");
                    }
                }
                
                for (int i = 0; i < metaObjectData.constructors.size(); i++) {
                    Constructor<?> constructor = metaObjectData.constructors.get(i);
                    List<ParameterInfo> constructorParameterInfos = allConstructorParameterInfos.get(i);
                    MetaObjectData.MetaTypeInfo[] metaTypes = metaObjectData.constructorMetaTypes.get(i);
                    metaTypes[0] = new MetaObjectData.MetaTypeInfo(QMetaType.Type.UnknownType.value(), null);
                    // constructors: parameters
                    int METHOD_PARAMETER_INDEX = paramIndexOfMethods.get(constructor);
                    metaObjectData.intData.set(METHOD_PARAMETER_INDEX, metaObjectData.intData.size());
                    if(isQt6) {
                        metaObjectData.intData.set(METHOD_PARAMETER_INDEX+3, metaObjectData.metaTypes.size());
                    }
                    metaObjectData.intData.add(0x80000000 | metaObjectData.addStringDataAndReturnIndex(""));
                    intdataComments.add("constructor["+i+"].returnType");
                    for (int j = 0; j < constructorParameterInfos.size(); j++) {
                    	ParameterInfo info = constructorParameterInfos.get(j);
                        if(info.type==null){
                            metaObjectData.intData.add(0x80000000 | metaObjectData.addStringDataAndReturnIndex(info.typeName));
                            metaObjectData.metaTypes.add(info.metaTypeId);
                            metaTypes[j+1] = new MetaObjectData.MetaTypeInfo(info.metaTypeId, info.typeName);
                        }else{
                            metaObjectData.intData.add(info.type.value());
                            metaObjectData.metaTypes.add(info.type.value());
                            metaTypes[j+1] = new MetaObjectData.MetaTypeInfo(info.type.value(), info.typeName);
                        }
                        intdataComments.add("constructor["+i+"]: parameter["+(j)+"].arg");
                    }
                    Parameter[] parameters = constructor.getParameters();
                    for (int j = 0; j < parameters.length; j++) {
                        if(parameters[j].isNamePresent()) {
                            metaObjectData.intData.add(metaObjectData.addStringDataAndReturnIndex(parameters[j].getName()));
                        }else {
                            metaObjectData.intData.add(metaObjectData.addStringDataAndReturnIndex("arg__"+(j+1)));
                        }
                        intdataComments.add("constructor["+i+"]: parameter["+(j)+"].argName");
                    }
                }
                
                //
                // Build property array
                //
                int metaObjectFlags = 0;
                
                if(!propertyReaders.isEmpty()){
                    if(!isQObject) {
                        metaObjectFlags |= PropertyAccessInStaticMetaCall.value();
                    }
                    metaObjectData.intData.set(PROPERTY_METADATA_INDEX, metaObjectData.intData.size());
                    int i=0;
                    for (String propertyName : propertyReaders.keySet()) {
                        Method reader = propertyReaders.get(propertyName);
                        Field qPropertyField = propertyQPropertyFields.get(propertyName);
                        Field propertyMemberField = propertyMembers.get(propertyName);
                        Method writer = null;
                        List<Method> writers = propertyWriters.get(propertyName);
                        Class<?> propertyType;
                        Type genericPropertyType;
                        AnnotatedElement annotatedPropertyType = null;
                        boolean isPointer;
                        boolean isReference;
                        boolean isMemberWritable = false;
                        boolean isMemberReadable = false;
                        boolean isMemberBindable = false;
                        QtMetaType metaTypeDecl;
                        if(reader!=null) {
                        	propertyType = reader.getReturnType();
                        	genericPropertyType = reader.getGenericReturnType();
                        	if(ClassAnalyzerUtility.useAnnotatedType) {
                        		annotatedPropertyType = reader.getAnnotatedReturnType();
                        	}
                        	isPointer = reader.isAnnotationPresent(QtPointerType.class)
                                    || (annotatedPropertyType!=null && annotatedPropertyType.isAnnotationPresent(QtPointerType.class));
                        	QtReferenceType referenceType = reader.getAnnotation(QtReferenceType.class);
                        	if(referenceType==null && annotatedPropertyType!=null)
                        		referenceType = annotatedPropertyType.getAnnotation(QtReferenceType.class);
                        	isReference = referenceType!=null && !referenceType.isConst();
                        	metaTypeDecl = annotatedPropertyType==null ? null : annotatedPropertyType.getAnnotation(QtMetaType.class);
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
                            	isMemberBindable = true;
                            	if(info.annotatedPropertyType!=null)
                            		metaTypeDecl = info.annotatedPropertyType.getAnnotation(QtMetaType.class);
                            	else
                            		metaTypeDecl = null;
                        	}else {
                            	propertyType = propertyMemberField.getType();
                            	genericPropertyType = propertyMemberField.getGenericType();
                            	if(ClassAnalyzerUtility.useAnnotatedType) {
                            		annotatedPropertyType = propertyMemberField.getAnnotatedType();
                            	}
                            	isPointer = propertyMemberField.isAnnotationPresent(QtPointerType.class)
                                        || (annotatedPropertyType!=null && annotatedPropertyType.isAnnotationPresent(QtPointerType.class));
                            	QtReferenceType referenceType = propertyMemberField.getAnnotation(QtReferenceType.class);
                            	if(referenceType==null && annotatedPropertyType!=null)
                            		referenceType = annotatedPropertyType.getAnnotation(QtReferenceType.class);
                            	isReference = referenceType!=null && !referenceType.isConst();
                            	isMemberWritable = !Modifier.isFinal(propertyMemberField.getModifiers());
                            	isMemberReadable = true;
                            	metaTypeDecl = annotatedPropertyType!=null ? annotatedPropertyType.getAnnotation(QtMetaType.class) : null;
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
                        	isMemberBindable = true;
                        	if(info.annotatedPropertyType!=null)
                        		metaTypeDecl = info.annotatedPropertyType.getAnnotation(QtMetaType.class);
                        	else
                        		metaTypeDecl = null;
                        }else {
                        	propertyType = null;
                        	genericPropertyType = null;
                        	annotatedPropertyType = null;
                        	isPointer = false;
                        	isReference = false;
                        	metaTypeDecl = null;
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
                        			if(ClassAnalyzerUtility.useAnnotatedType) {
                        				isPointer = writer.getAnnotatedParameterTypes()[0].isAnnotationPresent(QtPointerType.class);
                        				QtReferenceType referenceType = writer.getAnnotatedParameterTypes()[0].getAnnotation(QtReferenceType.class);
	                                	isReference = referenceType!=null && !referenceType.isConst();
	                                	metaTypeDecl = writer.getAnnotatedParameterTypes()[0].getAnnotation(QtMetaType.class);
                        			}
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
                        int metaTypeId = 0;
                        String typeName;
                        if(isReference) {
                        	throw new IllegalStateException("Type of property "+ clazz.getTypeName()+"."+propertyName +" must not be reference type");
                        }
                        if(metaTypeDecl!=null) {
            				if(metaTypeDecl.id()!=0) {
            					metaTypeId = metaTypeDecl.id();
            					if(isPointer || isReference) {
            						metaTypeId = registerRefMetaType(metaTypeId, isPointer, isReference);
            					}
            					typeName = new QMetaType(metaTypeId).name().toString();
            				}else if(metaTypeDecl.type()!=QMetaType.Type.UnknownType){
            					metaTypeId = metaTypeDecl.type().value();
            					if(isPointer || isReference) {
            						metaTypeId = registerRefMetaType(metaTypeId, isPointer, isReference);
            					}
            					typeName = new QMetaType(metaTypeId).name().toString();
            				}else {
        						if(metaTypeDecl.name().isEmpty())
        							throw new IllegalArgumentException("Incomplete @QtMetaType declaration. Either use type, id or name to specify meta type.");
            					typeName = metaTypeDecl.name();
            					if(isPointer && !typeName.endsWith("*")) {
                                    typeName += "*";
                                }
                                if(isReference) {
                                	if(typeName.endsWith("*")) {
                                        typeName = typeName.substring(0, typeName.length()-2);
                                    }
                                    if(!typeName.endsWith("&")) {
                                        typeName += "&";
                                    }
                                }
            				}
            			}else{
            				typeName = internalTypeNameOfClass(propertyType, genericPropertyType);
            				if(isPointer) {
                                if(!typeName.isEmpty() && !typeName.endsWith("*")) {
                                    typeName += "*";
                                }
                            }
                            if(isReference) {
                                if(typeName.endsWith("*")) {
                                    typeName = typeName.substring(0, typeName.length()-2);
                                }
                                if(!typeName.isEmpty() && !typeName.endsWith("&")) {
                                    typeName += "&";
                                }
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
                        if ((bindable!=null || isMemberBindable) && Bindable!=null)
                            flags.set(Bindable);
                        
                        if (designableVariant instanceof Boolean) {
                            if ((Boolean) designableVariant)
                                flags.set(PropertyFlags.Designable);
                            metaObjectData.propertyDesignableResolvers.add(null);
                        } else if (designableVariant instanceof Method) {
                            metaObjectData.propertyDesignableResolvers.add((Method) designableVariant);
                            flags.set(ResolveDesignable);
                        }else {
                            metaObjectData.propertyDesignableResolvers.add(null);
                        }
                        
                        if (scriptableVariant instanceof Boolean) {
                            if ((Boolean) scriptableVariant)
                                flags.set(PropertyFlags.Scriptable);
                            metaObjectData.propertyScriptableResolvers.add(null);
                        } else if (scriptableVariant instanceof Method) {
                            flags.set(ResolveScriptable);
                            metaObjectData.propertyScriptableResolvers.add((Method) scriptableVariant);
                        }else {
                            metaObjectData.propertyScriptableResolvers.add(null);
                        }
                        
                        if (editableVariant instanceof Boolean) {
                            if ((Boolean) editableVariant)
                                flags.set(Editable);
                            metaObjectData.propertyEditableResolvers.add(null);
                        } else if (editableVariant instanceof Method) {
                            flags.set(ResolveEditable);
                            metaObjectData.propertyEditableResolvers.add((Method) editableVariant);
                        }else {
                            metaObjectData.propertyEditableResolvers.add(null);
                        }
                        
                        if (storedVariant instanceof Boolean) {
                            if ((Boolean) storedVariant)
                                flags.set(PropertyFlags.Stored);
                            metaObjectData.propertyStoredResolvers.add(null);
                        } else if (storedVariant instanceof Method) {
                                                        
                            flags.set(ResolveStored);
                            metaObjectData.propertyStoredResolvers.add((Method) storedVariant);
                        }else {
                            metaObjectData.propertyStoredResolvers.add(null);
                        }
                               
                        if (userVariant instanceof Boolean) {
                            if ((Boolean) userVariant)
                                flags.set(PropertyFlags.User);
                            metaObjectData.propertyUserResolvers.add(null);
                        } else if (userVariant instanceof Method) {
                            flags.set(ResolveUser);
                            metaObjectData.propertyUserResolvers.add((Method) userVariant);
                        }else {
                            metaObjectData.propertyUserResolvers.add(null);
                        }
                        
                        if (Boolean.TRUE.equals(constantVariant)) {
                        	if(writer!=null || notify!=null)
                        		throw new QPropertyDeclarationException(String.format("Property %1$s in class %2$s: Must not specify @QtPropertyConstant in combination with writer and/or notifier signal.", propertyName, clazz.getTypeName()));
                            flags.set(PropertyFlags.Constant);
                        }else if (writer==null 
                        		&& notify==null 
                        		&& qPropertyField==null
                        		&& (propertyMemberField==null || Modifier.isFinal(propertyMemberField.getModifiers()))) {
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
                        metaObjectData.intData.add(metaObjectData.addStringDataAndReturnIndex(propertyName));
                        intdataComments.add("property["+i+"].name");
                        QMetaType.Type type = metaType(typeName);
                        if(type==QMetaType.Type.UnknownType || type==QMetaType.Type.User){
                        	if(metaTypeId==QMetaType.Type.UnknownType.value()) {
	                        	metaTypeId = findMetaType(typeName);
	                        	QMetaType metaType = new QMetaType(metaTypeId);
	                            if(metaTypeId==QMetaType.Type.UnknownType.value() 
	                            		|| !(genericPropertyType instanceof Class 
	                            				|| metaType.name().toString().equals(typeName))
	                            		|| metaType.javaType()!=propertyType) {
	                                metaTypeId = registerMetaType(propertyType, genericPropertyType, annotatedPropertyType, isPointer, isReference);
	                                metaType = new QMetaType(metaTypeId);
	                            }
	                            if(metaTypeId!=QMetaType.Type.UnknownType.value())
	                                typeName = new QMetaType(metaTypeId).name().toString();
                        	}
                            metaObjectData.intData.add(0x80000000 | metaObjectData.addStringDataAndReturnIndex(typeName));
                            metaObjectData.metaTypes.set(i, metaTypeId);
                            metaObjectData.propertyMetaTypes.add(new MetaObjectData.MetaTypeInfo[]{new MetaObjectData.MetaTypeInfo(metaTypeId, typeName),new MetaObjectData.MetaTypeInfo(metaTypeId, typeName)});
                        }else{
                            metaObjectData.intData.add(type.value());
                            metaObjectData.metaTypes.set(i, type.value());
                            metaObjectData.propertyMetaTypes.add(new MetaObjectData.MetaTypeInfo[]{new MetaObjectData.MetaTypeInfo(type.value(), typeName),new MetaObjectData.MetaTypeInfo(type.value(), typeName)});
                        }
                        metaObjectData.propertyClassTypes.add(propertyType);
                        intdataComments.add("property["+i+"].type");
                        metaObjectData.intData.add(flags.value());
                        intdataComments.add("property["+i+"].flags");
                        Integer signalIndex = signalIndexes.get(notify);
                        if(isQt6){
                            metaObjectData.intData.add(signalIndex!=null ? signalIndex : -1);
                            intdataComments.add("property["+i+"].notifyId");
                            metaObjectData.intData.add(0);
                            intdataComments.add("property["+i+"].revision");
                        }
                        
                        metaObjectData.propertyReaders.add(reader);
                        metaObjectData.propertyWriters.add(writer);
                        metaObjectData.propertyResetters.add(resetter);
                        metaObjectData.propertyNotifies.add(signalIndex);
                        metaObjectData.propertyBindables.add(bindable);
                        metaObjectData.propertyQPropertyFields.add(qPropertyField);
                        metaObjectData.propertyMemberFields.add(propertyMemberField);
                        ++i;
                    }
                    
                    i=0;
                    for (String propertyName : propertyReaders.keySet()) {
                        Field notify = propertyNotifies.get(propertyName);
                        if(notify!=null) {
                        	int idx = 0x70000000;
                        	for(int j=0; j<metaObjectData.signalInfos.size(); ++j) {
                        		if(metaObjectData.signalInfos.get(j).field==notify) {
                        			idx = j;
                        			break;
                        		}
                        	}
                            metaObjectData.intData.add(idx);
                            intdataComments.add("property["+i+"].notify_signal_id");
                        }else {
                            metaObjectData.intData.add(0);
                            intdataComments.add("property["+i+"].notify_signal_id");
                        }
                        ++i;
                    }
                }
                
                if(metaObjectFlags!=0) {
                    metaObjectData.intData.set(flagsIndex, metaObjectFlags);
                }
                //
                // Build enums array
                //
                
                if(!enums.isEmpty()){
                    metaObjectData.intData.set(ENUM_METADATA_INDEX, metaObjectData.intData.size());
                    List<Class<?>> enumList = new ArrayList<Class<?>>(enums.values());
                    HashMap<Object,Integer> dataIndexOfEnums = new HashMap<Object,Integer>();
                    
                    for (int i = 0; i < enumList.size(); i++) {
                        Class<?> enumClass = enumList.get(i);
                        // enums: name, alias, flags, count, data
                        if(isQt6){
                            String alias = enumClass.getSimpleName();
                            if(QFlags.class.isAssignableFrom(enumClass)) {
                                Class<?> _enumClass = getEnumForQFlags(enumClass);
                                alias = _enumClass.getSimpleName();
                            }
                            metaObjectData.intData.add(metaObjectData.addStringDataAndReturnIndex(enumClass.getSimpleName()));	intdataComments.add("enum["+i+"].name");
                            metaObjectData.intData.add(metaObjectData.addStringDataAndReturnIndex(alias));	intdataComments.add("enum["+i+"].alias");
                        }else{
                            metaObjectData.intData.add(metaObjectData.addStringDataAndReturnIndex(enumClass.getSimpleName()));	intdataComments.add("enum["+i+"].name");
                        }
                                                                                                                                               
                                                                                                                            
                        metaObjectData.intData.add(QFlags.class.isAssignableFrom(enumClass) ? 0x1 : 0x0);	intdataComments.add("enum["+i+"].flags");
                        
                        // Get the enum class
                        Class<?> contentEnumClass = Enum.class.isAssignableFrom(enumClass) ? enumClass : getEnumForQFlags(enumClass);
                        
                        if(contentEnumClass==null) {
                        	metaObjectData.intData.add(0);	intdataComments.add("enum["+i+"].count");
                        }else {
                        	Object[] enumConstants = contentEnumClass.getEnumConstants();
                        	metaObjectData.intData.add(enumConstants==null ? 0 : enumConstants.length);	intdataComments.add("enum["+i+"].count");
                        }
                        dataIndexOfEnums.put(enumClass, metaObjectData.intData.size());
                        metaObjectData.intData.add(0);	intdataComments.add("enum["+i+"].data");
                    }
                    
                    for (int i = 0; i < enumList.size(); i++) {
                        Class<?> enumClass = enumList.get(i);
                        @SuppressWarnings("unchecked")
                        Class<Enum<?>> contentEnumClass = (Class<Enum<?>>)(Enum.class.isAssignableFrom(enumClass) ? enumClass : getEnumForQFlags(enumClass));
                        // enum data: key, value
                        int ENUM_DATA_INDEX = dataIndexOfEnums.get(enumClass);
                        metaObjectData.intData.set(ENUM_DATA_INDEX, metaObjectData.intData.size());
                        if(contentEnumClass!=null) {
                        	Enum<?>[] enumConstants = contentEnumClass.getEnumConstants();
                        	if(enumConstants!=null) {
		                        for(Enum<?> enumConstant : enumConstants){
		                            metaObjectData.intData.add(metaObjectData.addStringDataAndReturnIndex(enumConstant.name()));	intdataComments.add("enum["+i+"].data: key");
		                            if(enumConstant instanceof QtEnumerator){
		                                QtEnumerator enumerator = (QtEnumerator)enumConstant;
		                                metaObjectData.intData.add(enumerator.value());
		                            }else if(enumConstant instanceof QtShortEnumerator){
		                                QtShortEnumerator enumerator = (QtShortEnumerator)enumConstant;
		                                metaObjectData.intData.add((int)enumerator.value());
		                            }else if(enumConstant instanceof QtByteEnumerator){
		                                QtByteEnumerator enumerator = (QtByteEnumerator)enumConstant;
		                                metaObjectData.intData.add((int)enumerator.value());
		                            }else if(enumConstant instanceof QtLongEnumerator){
		                                QtLongEnumerator enumerator = (QtLongEnumerator)enumConstant;
		                                metaObjectData.intData.add((int)enumerator.value());
		                            }else{
		                                metaObjectData.intData.add(enumConstant.ordinal());
		                            }
		                            intdataComments.add("enum["+i+"].data: value");
		                        }
                        	}
                        }
                    }
                }
                
                //
                // Terminate data array
                //
                metaObjectData.intData.add(0); // eod
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
                for (int i = 0; i < metaObjectData.intData.size(); i++) {
                    try {
                        String strg = null;
                        try {
                            if(intdataComments.get(i).endsWith("]: name")) {
                                strg = metaObjectData.getStringData(metaObjectData.intData.get(i));
                            }else if(intdataComments.get(i).endsWith("].argName")) {
                                strg = metaObjectData.getStringData(metaObjectData.intData.get(i));
                            }else if(intdataComments.get(i).endsWith("].arg")) {
                                int idx = metaObjectData.intData.get(i);
                                if((idx & 0x80000000) == 0x80000000) {
                                    idx = idx & ~0x80000000;
                                    if(idx>=0 && idx<metaObjectData.stringDataSize())
                                        strg = metaObjectData.getStringData(idx);
                                }else if(idx>=0 && idx<QMetaType.Type.values().length){
                                    for(QMetaType.Type t : QMetaType.Type.values()) {
                                        if(t.value()==idx) {
                                            strg = ""+t;
                                            break;
                                        }
                                    }
                                }
                            }else if(intdataComments.get(i).endsWith("].returnType")) {
                                int idx = metaObjectData.intData.get(i);
                                if((idx & 0x80000000) == 0x80000000) {
                                    idx = idx & ~0x80000000;
                                    if(idx>=0 && idx<metaObjectData.stringDataSize())
                                        strg = metaObjectData.getStringData(idx);
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
                                System.out.printf("\t%1$s: %3$s=%2$s (%4$s) --> %5$s\n", i, metaObjectData.intData.get(i), intdataComments.get(i), nms.get(i), strg);
                            }else {
                                System.out.printf("\t%1$s: %3$s=%2$s --> %4$s\n", i, metaObjectData.intData.get(i), intdataComments.get(i), strg);
                            }
                        }else {
                            if(i<nms.size()) {
                                System.out.printf("\t%1$s: %3$s=%2$s (%4$s)\n", i, metaObjectData.intData.get(i), intdataComments.get(i), nms.get(i));
                            }else {
                                System.out.printf("\t%1$s: %3$s=%2$s\n", i, metaObjectData.intData.get(i), intdataComments.get(i));
                            }
                        }
                    } catch (IndexOutOfBoundsException e) {
                        System.out.printf("\t%1$s: %2$s\n", i, metaObjectData.intData.get(i));
                    }
                }
                System.out.println("}");
            }
            
//			for (int i = 0; i < metaData.stringData.size(); i++) {
//				System.out.printf("string[%1$s]= %2$s\n", i, metaData.getStringData(i));
//			}
            return metaObjectData;
        } catch (RuntimeException | Error e) {
            throw e;
        } catch (Throwable e) {
            throw new RuntimeException(e);
        }
    }

    private static abstract class Classes extends io.qt.core.QObject{
		static Class<?> QPrivateConstructor(){
    		return QPrivateConstructor.class;
    	}
		static Class<?> QDeclarativeConstructor(){
    		return QDeclarativeConstructor.class;
    	}
    }
    
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
		QPropertyTypeInfo(Class<?> propertyType, Type genericPropertyType, AnnotatedElement annotatedPropertyType, boolean isPointer,
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
    	final AnnotatedElement annotatedPropertyType;
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
            	if((name.startsWith("clear") || name.startsWith("reset")) && name.length() > 5) {
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
    
    private static Method findPropertyReader(Method method, String propertyName, Class<?> paramType) {
        if (method == null)
            return null;

        PropertyAnnotation reader = PropertyAnnotation.readerAnnotation(method);
        if (reader != null
            && (!reader.name().equals(propertyName)
                || !reader.enabled()
                || (paramType!=null && !method.getReturnType().isAssignableFrom(paramType)))) {
            return null;
        } else {
            return method;
        }
    }
    
    private static Method findPropertyWriter(Method method, String propertyName, Class<?> paramType) {
        if (method == null)
            return null;

        PropertyAnnotation reader = PropertyAnnotation.writerAnnotation(method);
        if (reader != null
            && (!reader.name().equals(propertyName)
        		|| method.getParameterCount()!=1
        		|| method.getReturnType()!=void.class
                || !reader.enabled()
                || (paramType!=null && !method.getParameterTypes()[0].isAssignableFrom(paramType)))) {
            return null;
        } else {
            return method;
        }
    }

    private static Map<String, Class<?>> findFlagsAndEnums(Class<?> clazz) {
    	Map<String, Class<?>> enums = new TreeMap<>();
        Class<?> declaredClasses[] = clazz.getDeclaredClasses();
        for (Class<?> declaredClass : declaredClasses)
            putEnumOrFlag(declaredClass, enums);

        return enums;
    }

    private static int putEnumOrFlag(Class<?> type, Map<String, Class<?>> enums) {
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
        if (!enums.containsKey(enumType.getName())) {
            enums.put(enumType.getName(), enumType);
        	Object[] enumConstants = enumType.getEnumConstants();
            enumConstantCount = enumConstants==null ? 0 : enumConstants.length;
        }

        if (flagsType != null && !enums.containsKey(flagsType.getName()))
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

    private static Object isDesignable(AccessibleObject member, Class<?> clazz) {
        QtPropertyDesignable designable = member.getAnnotation(QtPropertyDesignable.class);

        if (designable != null) {
            String value = designable.value();

            if (value.equalsIgnoreCase("true")) {
                return Boolean.TRUE;
            } else if (value.equalsIgnoreCase("false")) {
                return Boolean.FALSE;
            } else if(!isQt6) { 
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
    
    private static Object isScriptable(AccessibleObject member, Class<?> clazz) {
        QtPropertyScriptable scriptable = member.getAnnotation(QtPropertyScriptable.class);

        if (scriptable != null) {
            String value = scriptable.value();

            if (value.equalsIgnoreCase("true")) {
                return Boolean.TRUE;
            } else if (value.equalsIgnoreCase("false")) {
                return Boolean.FALSE;
            } else if(!isQt6) {
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
    
    private static Object isStored(AccessibleObject member, Class<?> clazz) {
        QtPropertyStored stored = member.getAnnotation(QtPropertyStored.class);

        if (stored != null) {
            String value = stored.value();

            if (value.equalsIgnoreCase("true")) {
                return Boolean.TRUE;
            } else if (value.equalsIgnoreCase("false")) {
                return Boolean.FALSE;
            } else if(!isQt6) {
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
    
    private static Object isEditable(AccessibleObject member, Class<?> clazz) {
        return Boolean.TRUE;
    }

    private static boolean isValidSetter(Method declaredMethod) {
        return (declaredMethod.getParameterCount() == 1
                && declaredMethod.getReturnType() == Void.TYPE);
    }

    private static Method getDeclaredMethod(Class<?> clazz, String name, Class<?>... args) {
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

    private static Object isUser(AccessibleObject member, Class<?> clazz) {
        QtPropertyUser user = member.getAnnotation(QtPropertyUser.class);

        if (user != null) {
            String value = user.value();

            if (value.equalsIgnoreCase("true")) {
                return Boolean.TRUE;
            } else if (value.equalsIgnoreCase("false")) {
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

    private static Boolean isRequired(AccessibleObject member, Class<?> clazz) {
        QtPropertyRequired required = member.getAnnotation(QtPropertyRequired.class);
        if (required != null) {
            return required.value();
        }
        return Boolean.FALSE;
    }

    private static Boolean isFinal(Method declaredMethod) {
        return Modifier.isFinal(declaredMethod.getModifiers());
    }

    private static Boolean isConstant(AccessibleObject member) {
        QtPropertyConstant isConstant = member.getAnnotation(QtPropertyConstant.class);
        if (isConstant != null) {
            return isConstant.value();
        }
        return Boolean.FALSE;
    }

    private static boolean isValidGetter(Method method) {
        return (method.getParameterCount() == 0
                && method.getReturnType() != Void.TYPE);
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

    private static boolean overridesGeneratedSlot(Method declaredMethod, Class<?> clazz) {
        if(!Modifier.isPrivate(declaredMethod.getModifiers()) && !Modifier.isStatic(declaredMethod.getModifiers()) && clazz.getSuperclass()!=null) {
            try {
                Method declaredSuperMethod = clazz.getSuperclass().getDeclaredMethod(declaredMethod.getName(), declaredMethod.getParameterTypes());
                if(declaredSuperMethod!=null) {
                    Class<?> declaringClass = declaredSuperMethod.getDeclaringClass();
                    if(ClassAnalyzerUtility.isGeneratedClass(declaringClass)) {
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

    // This property allows you to override the QMetaObject revision number for
    //  QtJambi to use.
    private static int revision;
    // This should be updated as the code-base supports the correct data layout
    //  for each new revision.  We don't necessarily have to support the features
    //  that new revision brings as well.
    private static int resolveMetaDataRevision() {
        int r = revision;
        if(r != 0)
            return r;

        int major = QtJambi_LibraryUtilities.qtMajorVersion;
//        int minor = QtJambi_LibraryUtilities.qtMinorVersion;
        if(major == 5)
            r = 7;  // 5.0.x (Qt5 requires a minimum of this revision)
        else if(major == 6)
            r = 9;  // 6.0.x (Qt6 requires a minimum of this revision)
        else  // All future versions
            r = 9;
        revision = r;
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
	
	private static long findEmitMethodId(Class<?> signalClass, List<Class<?>> signalTypes) {
		Map<List<Class<?>>,SignalUtility.EmitMethodInfo> emitMethods = SignalUtility.findEmitMethods(signalClass);
		SignalUtility.EmitMethodInfo result;
		if(emitMethods.isEmpty())
			result = null;
		else if(emitMethods.size()==1) {
			if(signalTypes==null) {
				result = emitMethods.entrySet().iterator().next().getValue();
			}else {
				result = emitMethods.get(signalTypes);
			}
		}else {
			if(signalTypes==null) {
				result = emitMethods.get(emitMethods.keySet().stream().max((t1, t2)->t1.size() > t2.size() ? 1 : -1).orElse(null));
			}else {
				result = emitMethods.get(signalTypes);
			}
		}
		return result==null ? 0 : result.methodId;
	}

	/**
	 * Returns true if the class cl represents a Signal.
	 * 
	 * @return True if the class is a signal
	 * @param cl The class to check
	 */
	public static boolean isQObjectSignalType(Class<?> cl) {
		return SignalUtility.AbstractSignal.class.isAssignableFrom(cl)
				&& !Modifier.isAbstract(cl.getModifiers())
				&& cl.getEnclosingClass() != QMetaObject.class
				&& cl.getEnclosingClass() != QStaticMemberSignals.class
				&& cl.getEnclosingClass() != QDeclarableSignals.class && !SignalUtility.findEmitMethods(cl).isEmpty();
	}
	
	private final static Set<Class<?>> gadgetClasses = Collections.synchronizedSet(new HashSet<>());
	private final static Set<String> gadgetPackages = Collections.synchronizedSet(new HashSet<>());
	
	public static void useAsGadget(Class<?> clazz) {
		gadgetClasses.add(clazz);
    }
    
    public static void usePackageContentAsGadgets(String _package) {
    	gadgetPackages.add(_package);
    }
    
    private static java.util.function.Function<Class<?>,Map<String,String>> qmlClassInfoGeneratorFunction;
    
	public static void setQmlClassInfoGeneratorFunction(
			java.util.function.Function<Class<?>, Map<String, String>> qmlClassInfogeneratorFunction) {
		MetaObjectUtility.qmlClassInfoGeneratorFunction = qmlClassInfogeneratorFunction;
	}
}

@NativeAccess
class MetaObjectData {
	
	@NativeAccess
	static class MetaTypeInfo{
		final @NativeAccess int metaTypeId;
		final @NativeAccess String typeName;
		MetaTypeInfo(int metaTypeId, String typeName) {
			this.metaTypeId = metaTypeId;
			this.typeName = typeName;
		}
	}
	
	@NativeAccess
	static class IntArray{
		@NativeAccess
		private int[] array = new int[1024];
		private int size = 0;
		void set(int i, int data) {
			array[i] = data;
		}
		int get(int i) {
			return array[i];
		}
		void add(int data) {
			array[size++] = data;
			if(size==array.length) {
				array = Arrays.copyOf(array, array.length+1024);
			}
		}
		
		int size() {
			return size;
		}
	}
	
    @NativeAccess
	static class SignalInfo{
		SignalInfo(Field field, List<SignalParameterType> signalTypes, Class<?> signalClass, int[] signalMetaTypes, long methodId) {
			super();
			this.field = field;
			this.signalTypes = Collections.unmodifiableList(signalTypes);
			this.signalClass = signalClass;
			this.signalMetaTypes = signalMetaTypes;
			this.methodId = methodId;
		}
		
		final @NativeAccess Field field;
		final @NativeAccess List<SignalParameterType> signalTypes;
		final @NativeAccess Class<?> signalClass;
		final @NativeAccess int[] signalMetaTypes;
		final @NativeAccess long methodId;
	}
    
	int addStringDataAndReturnIndex(String data) {
		int index = stringData.indexOf(data);
		if(index<0) {
			index = stringData.size();
			stringData.add(data);
		}
		return index;
	}
	
	int stringDataSize() {
		return stringData.size();
	}
	
	String getStringData(int index) {
		return stringData.get(index);
	}
	
    final @NativeAccess IntArray intData = new IntArray();
    private final @NativeAccess List<String>  stringData = new ArrayList<>();

    final @NativeAccess List<SignalInfo>  signalInfos = new ArrayList<>();
    final @NativeAccess List<Method>  methods = new ArrayList<>();
    final @NativeAccess List<MetaTypeInfo[]>   methodMetaTypes = new ArrayList<>();
    final @NativeAccess List<Constructor<?>> constructors = new ArrayList<>();
    final @NativeAccess List<MetaTypeInfo[]>   constructorMetaTypes = new ArrayList<>();

    final @NativeAccess List<Method>  propertyReaders = new ArrayList<>();
    final @NativeAccess List<Method>  propertyWriters = new ArrayList<>();
    final @NativeAccess List<Method>  propertyResetters = new ArrayList<>();
    final @NativeAccess List<Integer> propertyNotifies = new ArrayList<>();
    final @NativeAccess List<Method>  propertyBindables = new ArrayList<>();
    final @NativeAccess List<Field>   propertyQPropertyFields = new ArrayList<>();
    final @NativeAccess List<Field>   propertyMemberFields = new ArrayList<>();
    final @NativeAccess List<Method>  propertyDesignableResolvers = new ArrayList<>();
    final @NativeAccess List<Method>  propertyScriptableResolvers = new ArrayList<>();
    final @NativeAccess List<Method>  propertyEditableResolvers = new ArrayList<>();
    final @NativeAccess List<Method>  propertyStoredResolvers = new ArrayList<>();
    final @NativeAccess List<Method>  propertyUserResolvers = new ArrayList<>();
    final @NativeAccess List<MetaTypeInfo[]>   propertyMetaTypes = new ArrayList<>();
    final @NativeAccess List<Class<?>>   propertyClassTypes = new ArrayList<>();
    final @NativeAccess List<Class<?>> relatedMetaObjects = new ArrayList<>();

    @NativeAccess boolean hasStaticMembers;
    final @NativeAccess List<Integer> metaTypes = new ArrayList<>();
}