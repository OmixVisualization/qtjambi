/****************************************************************************
**
** Copyright (C) 2009-2022 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import static io.qt.internal.QtJambiInternal.registerMetaType;

import java.lang.reflect.AnnotatedElement;
import java.lang.reflect.AnnotatedParameterizedType;
import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.lang.reflect.Modifier;
import java.lang.reflect.ParameterizedType;
import java.lang.reflect.Type;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import io.qt.QPropertyDeclarationException;
import io.qt.QtObject;
import io.qt.QtPointerType;
import io.qt.QtPrimitiveType;
import io.qt.QtPropertyBindable;
import io.qt.QtReferenceType;
import io.qt.core.QList;
import io.qt.core.QMetaObject;
import io.qt.core.QMetaProperty;
import io.qt.core.QMetaType;
import io.qt.core.QObject;
import io.qt.core.QQueue;
import io.qt.core.QSet;
import io.qt.core.QStack;
import io.qt.core.QUntypedBindable;
import io.qt.core.QUntypedPropertyData;

class AbstractMetaObjectTools {
    static final PropertyFlags ResolveDesignable = null;
    static final PropertyFlags ResolveScriptable = null;
    static final PropertyFlags ResolveEditable = null;
    static final PropertyFlags Editable = null;
    static final PropertyFlags Notify = null;
    static final PropertyFlags ResolveStored = null;
    static final PropertyFlags ResolveUser = null;
    static final PropertyFlags Required = PropertyFlags.Required;
    static final PropertyFlags Bindable = PropertyFlags.Bindable;
    static final PropertyFlags Alias = PropertyFlags.Alias;
    static final MetaObjectFlag PropertyAccessInStaticMetaCall = MetaObjectFlag.PropertyAccessInStaticMetaCall;
    
    static MetaObjectTools.AnnotationInfo analyzeBindableAnnotation(Method method) {
    	QtPropertyBindable bindable = method.getAnnotation(QtPropertyBindable.class);
        return bindable == null ? null : new MetaObjectTools.AnnotationInfo(bindable.name(), bindable.enabled());
    }
    
    static boolean isValidBindable(Method method) {
    	return method.getParameterCount()==0 && QUntypedBindable.class.isAssignableFrom(method.getReturnType());
    }
    
    static boolean isValidQProperty(Field field) {
    	return QObject.class==field.getType().getEnclosingClass()
    			&& QUntypedPropertyData.class.isAssignableFrom(field.getType());
    }
    
    static MetaObjectTools.QPropertyTypeInfo getQPropertyTypeInfo(Field field) {
    	if(field.getType()==QObject.QBooleanProperty.class
    			|| field.getType()==QObject.QComputedBooleanProperty.class) {
    		return new MetaObjectTools.QPropertyTypeInfo(boolean.class, boolean.class, null, false, false, field.getType()==QObject.QBooleanProperty.class);
    	}else if(field.getType()==QObject.QByteProperty.class
    			|| field.getType()==QObject.QComputedByteProperty.class) {
    		return new MetaObjectTools.QPropertyTypeInfo(byte.class, byte.class, null, false, false, field.getType()==QObject.QByteProperty.class);
    	}else if(field.getType()==QObject.QIntProperty.class
    			|| field.getType()==QObject.QComputedIntProperty.class) {
    		return new MetaObjectTools.QPropertyTypeInfo(int.class, int.class, null, false, false, field.getType()==QObject.QIntProperty.class);
    	}else if(field.getType()==QObject.QShortProperty.class
    			|| field.getType()==QObject.QComputedShortProperty.class) {
    		return new MetaObjectTools.QPropertyTypeInfo(short.class, short.class, null, false, false, field.getType()==QObject.QShortProperty.class);
    	}else if(field.getType()==QObject.QLongProperty.class
    			|| field.getType()==QObject.QComputedLongProperty.class) {
    		return new MetaObjectTools.QPropertyTypeInfo(long.class, long.class, null, false, false, field.getType()==QObject.QLongProperty.class);
    	}else if(field.getType()==QObject.QFloatProperty.class
    			|| field.getType()==QObject.QComputedFloatProperty.class) {
    		return new MetaObjectTools.QPropertyTypeInfo(float.class, float.class, null, false, false, field.getType()==QObject.QFloatProperty.class);
    	}else if(field.getType()==QObject.QDoubleProperty.class
    			|| field.getType()==QObject.QComputedDoubleProperty.class) {
    		return new MetaObjectTools.QPropertyTypeInfo(double.class, double.class, null, false, false, field.getType()==QObject.QDoubleProperty.class);
    	}else if(field.getType()==QObject.QCharProperty.class
    			|| field.getType()==QObject.QComputedCharProperty.class) {
    		return new MetaObjectTools.QPropertyTypeInfo(char.class, char.class, null, false, false, field.getType()==QObject.QCharProperty.class);
    	}else if(field.getType()==QObject.QProperty.class
    			|| field.getType()==QObject.QComputedProperty.class) {
    		if(QtJambiInternal.useAnnotatedType) {
	        	AnnotatedElement t = field.getAnnotatedType();
	        	if (t instanceof AnnotatedParameterizedType) {
	            	AnnotatedParameterizedType p = (AnnotatedParameterizedType) t;
	            	AnnotatedElement actualTypes[] = p.getAnnotatedActualTypeArguments();
	            	if(actualTypes.length==1) {
	            		AnnotatedElement actualType = actualTypes[0];
	                	boolean isPrimitive = actualType.isAnnotationPresent(QtPrimitiveType.class);
	                	boolean isPointer = actualType.isAnnotationPresent(QtPointerType.class);
	                	QtReferenceType referenceType = actualType.getAnnotation(QtReferenceType.class);
	                	boolean isReference = !isPointer && referenceType!=null && !referenceType.isConst();
	                	if(!isPrimitive) {
	                		AnnotatedElement annotatedOwnerType = RetroHelper.getAnnotatedOwnerType(actualType);
	                		if(annotatedOwnerType!=null) {
	                			isPrimitive = annotatedOwnerType.isAnnotationPresent(QtPrimitiveType.class);
	                		}
	                	}
	                	Type type = ((java.lang.reflect.AnnotatedType)actualType).getType();
	                    Class<?> rawType;
	                    if (type instanceof Class) {
	                        rawType = (Class<?>) type;
	                    }else if (type instanceof ParameterizedType) {
	                    	ParameterizedType ptype = (ParameterizedType)type;
	                        rawType = (Class<?>)ptype.getRawType();
	                    } else {
	                    	return null;
	                    }
	                    if(isPrimitive) {
	                    	if(rawType==Integer.class) {
	                    		rawType = int.class;
	                    		type = int.class;
	                    	}else if(rawType==Short.class) {
	                    		rawType = short.class;
	                    		type = short.class;
	                    	}else if(rawType==Byte.class) {
	                    		rawType = byte.class;
	                    		type = byte.class;
	                    	}else if(rawType==Long.class) {
	                    		rawType = long.class;
	                    		type = long.class;
	                    	}else if(rawType==Double.class) {
	                    		rawType = double.class;
	                    		type = double.class;
	                    	}else if(rawType==Float.class) {
	                    		rawType = float.class;
	                    		type = float.class;
	                    	}else if(rawType==Boolean.class) {
	                    		rawType = boolean.class;
	                    		type = boolean.class;
	                    	}else if(rawType==Character.class) {
	                    		rawType = char.class;
	                    		type = char.class;
	                    	}
	                    }
	                    return new MetaObjectTools.QPropertyTypeInfo(rawType, type, actualType, isPointer, isReference, field.getType()==QObject.QProperty.class);
	            	}
	        	}
    		}else {
    			Type t = field.getGenericType();
            	if (t instanceof ParameterizedType) {
                	ParameterizedType p = (ParameterizedType) t;
                	Type actualTypes[] = p.getActualTypeArguments();
                	if(actualTypes.length==1) {
                		Type type = actualTypes[0];
                        Class<?> rawType;
                        if (type instanceof Class) {
                            rawType = (Class<?>) type;
                        }else if (type instanceof ParameterizedType) {
                        	ParameterizedType ptype = (ParameterizedType)type;
                            rawType = (Class<?>)ptype.getRawType();
                        } else {
                        	return null;
                        }
                        if(rawType==Integer.class) {
                    		rawType = int.class;
                    		type = int.class;
                    	}else if(rawType==Short.class) {
                    		rawType = short.class;
                    		type = short.class;
                    	}else if(rawType==Byte.class) {
                    		rawType = byte.class;
                    		type = byte.class;
                    	}else if(rawType==Long.class) {
                    		rawType = long.class;
                    		type = long.class;
                    	}else if(rawType==Double.class) {
                    		rawType = double.class;
                    		type = double.class;
                    	}else if(rawType==Float.class) {
                    		rawType = float.class;
                    		type = float.class;
                    	}else if(rawType==Boolean.class) {
                    		rawType = boolean.class;
                    		type = boolean.class;
                    	}else if(rawType==Character.class) {
                    		rawType = char.class;
                    		type = char.class;
                    	}
                        return new MetaObjectTools.QPropertyTypeInfo(rawType, type, null, false, false, field.getType()==QObject.QProperty.class);
                	}
            	}
    		}
    	}
    	return null;
    }
    
    private static class PropertyInfo{
    	private PropertyInfo(Field field, int propertyIndex) {
			super();
			this.field = field;
			this.propertyIndex = propertyIndex;
		}
		final Field field;
		final int propertyIndex;
    }
    
    private final static Map<Class<?>, List<PropertyInfo>> propertyFieldsByClasses = Collections.synchronizedMap(new HashMap<>());
    
    static QtJambiPropertyInfo analyzeProperty(QObject containingObject, QtObject property) {
		List<PropertyInfo> propertyFields = propertyFieldsByClasses.computeIfAbsent(QtJambiInternal.getClass(containingObject), cls->{
			List<PropertyInfo> fields = Collections.emptyList();
			while (QObject.class.isAssignableFrom(cls)) {
				QMetaObject metaObject = QMetaObject.forType(cls);
	            for (Field field : cls.getDeclaredFields()) {
	                if (!Modifier.isStatic(field.getModifiers())
            			&& AbstractMetaObjectTools.isValidQProperty(field)) {
	                	QMetaProperty metaProperty = getPropertyForField(metaObject, field);
	                	PropertyInfo info = new PropertyInfo(field, metaProperty.isValid() ? metaProperty.propertyIndex() : -1);
						if(fields.isEmpty()) {
	        				fields = Collections.singletonList(info);
	        			} else {
	        				if(fields.size()==1)
    	        				fields = new ArrayList<>(fields);
	        				fields.add(info);
	        			}
	                }
	            }
	            cls = cls.getSuperclass();
	        }
			if(fields.isEmpty()) {
				return Collections.emptyList();
			}else if(fields.size()==1){
				return fields;
			}else {
				return Collections.synchronizedList(fields);
			}
		});
		
		Field foundField = null;
		QList<QMetaProperty> remainingProperties = containingObject.metaObject().properties();
		for (PropertyInfo info : propertyFields) {
			try {
				if(QtJambiInternal.fetchField(containingObject, info.field)==property) {
					if(info.propertyIndex>=0) {
						QMetaProperty metaProperty = containingObject.metaObject().properties().at(info.propertyIndex);
						remainingProperties.removeOne(metaProperty);
            			if(!Modifier.isFinal(info.field.getModifiers())) {
            				if(!Boolean.getBoolean("qtjambi.allow-nonfinal-qproperties") && !Boolean.getBoolean("io.qt.allow-nonfinal-qproperties")) {
                    			java.util.logging.Logger.getLogger("io.qt.internal").severe(String.format("Missing modifier 'final' at property field %1$s.%2$s. Specify JVM argument -Dqtjambi.allow-nonfinal-qproperties=true to disable this error.", info.field.getDeclaringClass().getSimpleName(), info.field.getName()));
                    			throw new QPropertyDeclarationException(String.format("Missing modifier 'final' at property field %1$s.%2$s.", info.field.getDeclaringClass().getSimpleName(), info.field.getName()));
                    		}
            			}
	    				return new QtJambiPropertyInfo(info.field, metaProperty);
					}else {
						foundField = info.field;
					}
					break;
				}
			} catch (QPropertyDeclarationException e) {
				throw e;
			} catch (Throwable e) {}
        }
		if(foundField==null)
			throw new QPropertyDeclarationException("Cannot find member field belonging to QProperty instance.");
		MetaObjectTools.QPropertyTypeInfo pinfo = getQPropertyTypeInfo(foundField);
		int t = registerMetaType(
				pinfo.propertyType, 
				pinfo.genericPropertyType, 
				pinfo.annotatedPropertyType,
				pinfo.isPointer,
				pinfo.isReference);
		return new QtJambiPropertyInfo(foundField, new QMetaType(t));
    }
    
    static void registerPropertyField(QMetaProperty metaProperty, java.lang.reflect.Field field) {
    	registerPropertyField(QtJambiInternal.internalAccess.nativeId(metaProperty), field);
    }
    
    private static native void registerPropertyField(long metaPropertyId, java.lang.reflect.Field field);
    
    private static native QMetaProperty getPropertyForField(QMetaObject metaObject, Field field);
    
    static boolean isListType(Class<?> cls) {
    	return cls==QList.class
    			|| cls==QQueue.class
    			|| cls==QStack.class
    			|| cls==QSet.class;
    }
}
