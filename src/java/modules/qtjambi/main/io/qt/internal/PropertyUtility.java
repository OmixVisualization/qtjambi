/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import java.lang.reflect.*;
import java.util.*;
import java.util.function.*;
import io.qt.*;
import io.qt.core.*;
import io.qt.internal.QtMocConstants.*;

/**
 * @hidden
 */
public abstract class PropertyUtility {
	protected PropertyUtility() {throw new RuntimeException();}
    
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
    
    static MetaObjectUtility.AnnotationInfo analyzeBindableAnnotation(Method method) {
    	QtPropertyBindable bindable = method.getAnnotation(QtPropertyBindable.class);
        return bindable == null ? null : new MetaObjectUtility.AnnotationInfo(bindable.name(), bindable.enabled());
    }
    
    static boolean isValidBindable(Method method) {
    	return method.getParameterCount()==0 && QUntypedBindable.class.isAssignableFrom(method.getReturnType());
    }
    
    static boolean isValidQProperty(Field field) {
    	return QObject.class==field.getType().getEnclosingClass()
    			&& QUntypedPropertyData.class.isAssignableFrom(field.getType());
    }
    
    static MetaObjectUtility.QPropertyTypeInfo getQPropertyTypeInfo(Field field) {
    	if(field.getType()==QObject.QBooleanProperty.class
    			|| field.getType()==QObject.QComputedBooleanProperty.class) {
    		return new MetaObjectUtility.QPropertyTypeInfo(boolean.class, boolean.class, null, false, false, field.getType()==QObject.QBooleanProperty.class);
    	}else if(field.getType()==QObject.QByteProperty.class
    			|| field.getType()==QObject.QComputedByteProperty.class) {
    		return new MetaObjectUtility.QPropertyTypeInfo(byte.class, byte.class, null, false, false, field.getType()==QObject.QByteProperty.class);
    	}else if(field.getType()==QObject.QIntProperty.class
    			|| field.getType()==QObject.QComputedIntProperty.class) {
    		return new MetaObjectUtility.QPropertyTypeInfo(int.class, int.class, null, false, false, field.getType()==QObject.QIntProperty.class);
    	}else if(field.getType()==QObject.QShortProperty.class
    			|| field.getType()==QObject.QComputedShortProperty.class) {
    		return new MetaObjectUtility.QPropertyTypeInfo(short.class, short.class, null, false, false, field.getType()==QObject.QShortProperty.class);
    	}else if(field.getType()==QObject.QLongProperty.class
    			|| field.getType()==QObject.QComputedLongProperty.class) {
    		return new MetaObjectUtility.QPropertyTypeInfo(long.class, long.class, null, false, false, field.getType()==QObject.QLongProperty.class);
    	}else if(field.getType()==QObject.QFloatProperty.class
    			|| field.getType()==QObject.QComputedFloatProperty.class) {
    		return new MetaObjectUtility.QPropertyTypeInfo(float.class, float.class, null, false, false, field.getType()==QObject.QFloatProperty.class);
    	}else if(field.getType()==QObject.QDoubleProperty.class
    			|| field.getType()==QObject.QComputedDoubleProperty.class) {
    		return new MetaObjectUtility.QPropertyTypeInfo(double.class, double.class, null, false, false, field.getType()==QObject.QDoubleProperty.class);
    	}else if(field.getType()==QObject.QCharProperty.class
    			|| field.getType()==QObject.QComputedCharProperty.class) {
    		return new MetaObjectUtility.QPropertyTypeInfo(char.class, char.class, null, false, false, field.getType()==QObject.QCharProperty.class);
    	}else if(field.getType()==QObject.QProperty.class
    			|| field.getType()==QObject.QComputedProperty.class) {
    		if(ClassAnalyzerUtility.useAnnotatedType) {
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
	                    return new MetaObjectUtility.QPropertyTypeInfo(rawType, type, actualType, isPointer, isReference, field.getType()==QObject.QProperty.class);
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
                        return new MetaObjectUtility.QPropertyTypeInfo(rawType, type, null, false, false, field.getType()==QObject.QProperty.class);
                	}
            	}
    		}
    	}
    	return null;
    }
    
    static MetaObjectUtility.QPropertyTypeInfo getQPropertyTypeInfo(Method method) {
    	if(method.getReturnType()==QBooleanBindable.class) {
    		return new MetaObjectUtility.QPropertyTypeInfo(boolean.class, boolean.class, null, false, false, method.getReturnType()==QBooleanBindable.class);
    	}else if(method.getReturnType()==QByteBindable.class) {
    		return new MetaObjectUtility.QPropertyTypeInfo(byte.class, byte.class, null, false, false, method.getReturnType()==QByteBindable.class);
    	}else if(method.getReturnType()==QIntBindable.class) {
    		return new MetaObjectUtility.QPropertyTypeInfo(int.class, int.class, null, false, false, method.getReturnType()==QIntBindable.class);
    	}else if(method.getReturnType()==QShortBindable.class) {
    		return new MetaObjectUtility.QPropertyTypeInfo(short.class, short.class, null, false, false, method.getReturnType()==QShortBindable.class);
    	}else if(method.getReturnType()==QLongBindable.class) {
    		return new MetaObjectUtility.QPropertyTypeInfo(long.class, long.class, null, false, false, method.getReturnType()==QLongBindable.class);
    	}else if(method.getReturnType()==QFloatBindable.class) {
    		return new MetaObjectUtility.QPropertyTypeInfo(float.class, float.class, null, false, false, method.getReturnType()==QFloatBindable.class);
    	}else if(method.getReturnType()==QDoubleBindable.class) {
    		return new MetaObjectUtility.QPropertyTypeInfo(double.class, double.class, null, false, false, method.getReturnType()==QDoubleBindable.class);
    	}else if(method.getReturnType()==QCharBindable.class) {
    		return new MetaObjectUtility.QPropertyTypeInfo(char.class, char.class, null, false, false, method.getReturnType()==QCharBindable.class);
    	}else if(method.getReturnType()==QBindable.class) {
    		if(ClassAnalyzerUtility.useAnnotatedType) {
	        	AnnotatedElement t = method.getAnnotatedReturnType();
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
	                    return new MetaObjectUtility.QPropertyTypeInfo(rawType, type, actualType, isPointer, isReference, method.getReturnType()==QBindable.class);
	            	}
	        	}
    		}else {
    			Type t = method.getGenericReturnType();
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
                        return new MetaObjectUtility.QPropertyTypeInfo(rawType, type, null, false, false, method.getReturnType()==QBindable.class);
                	}
            	}
    		}
    	}
    	return null;
    }
    
    private static class PropertyIndex{
    	private PropertyIndex(Field field, int propertyIndex) {
			super();
			this.field = field;
			this.propertyIndex = propertyIndex;
		}
		final Field field;
		final int propertyIndex;
    }
    
    private final static Map<Class<?>, List<PropertyIndex>> propertyFieldsByClasses = Collections.synchronizedMap(new HashMap<>());
    
    protected static <PI> PI analyzeProperty(QObject containingObject, QtObject property, BiFunction<Field, QMetaType, PI> fun1, BiFunction<Field, QMetaProperty, PI> fun2) {
		List<PropertyIndex> propertyFields = propertyFieldsByClasses.computeIfAbsent(AccessUtility.instance.getClass(containingObject), cls->{
			List<PropertyIndex> fields = Collections.emptyList();
			while (QObject.class.isAssignableFrom(cls)) {
				QMetaObject metaObject = QMetaObject.forType(cls);
	            for (Field field : cls.getDeclaredFields()) {
	                if (!Modifier.isStatic(field.getModifiers())
            			&& isValidQProperty(field)) {
	                	QMetaProperty metaProperty = getPropertyForField(metaObject, field);
	                	PropertyIndex info = new PropertyIndex(field, metaProperty!=null ? metaProperty.propertyIndex() : -1);
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
		for (PropertyIndex info : propertyFields) {
			try {
				if(ReflectionUtility.readField(containingObject, info.field)==property) {
					if(info.propertyIndex>=0) {
						QMetaProperty metaProperty = containingObject.metaObject().properties().at(info.propertyIndex);
						remainingProperties.removeOne(metaProperty);
            			if(!Modifier.isFinal(info.field.getModifiers())) {
            				if(!Boolean.getBoolean("qtjambi.allow-nonfinal-qproperties") && !Boolean.getBoolean("io.qt.allow-nonfinal-qproperties")) {
                    			java.util.logging.Logger.getLogger("io.qt.internal").severe(String.format("Missing modifier 'final' at property field %1$s.%2$s. Specify JVM argument -Dqtjambi.allow-nonfinal-qproperties=true to disable this error.", info.field.getDeclaringClass().getSimpleName(), info.field.getName()));
                    			throw new QPropertyDeclarationException(String.format("Missing modifier 'final' at property field %1$s.%2$s.", info.field.getDeclaringClass().getSimpleName(), info.field.getName()));
                    		}
            			}
	    				return fun2.apply(info.field, metaProperty);
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
		MetaObjectUtility.QPropertyTypeInfo pinfo = getQPropertyTypeInfo(foundField);
		int t = MetaTypeUtility.registerMetaType(
				pinfo.propertyType, 
				pinfo.genericPropertyType, 
				pinfo.annotatedPropertyType,
				pinfo.isPointer,
				pinfo.isReference);
		return fun1.apply(foundField, new QMetaType(t));
    }
    
    protected static void registerPropertyField(QMetaProperty metaProperty, java.lang.reflect.Field field) {
    	registerPropertyField(NativeUtility.nativeId(metaProperty), field);
    }
    
    private static native void registerPropertyField(long metaPropertyId, java.lang.reflect.Field field);
    
    private static native @Nullable QMetaProperty getPropertyForField(QMetaObject metaObject, Field field);
}
