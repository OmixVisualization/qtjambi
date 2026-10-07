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

import java.io.*;
import java.lang.invoke.*;
import java.lang.reflect.*;
import java.net.*;
import java.util.*;
import java.util.function.*;
import io.qt.*;
import io.qt.core.*;

/**
 * @hidden
 */
public abstract class CoreUtility {
	protected CoreUtility() {throw new RuntimeException();}
	
	/**
	 * @hidden
	 */
	final static class LambdaInfo {
		
		/**
		 * @hidden
		 */
		final static class MethodInfo{
			MethodInfo(Class<?> implClass, 
					boolean hasCapturedArgs, 
					int ownerIndex, 
					int qobjectIndex, 
					String capturingClass, 
					MethodHandle methodHandle, 
					Method reflectiveMethod, 
					boolean isStaticMethod, 
					Constructor<?> reflectiveConstructor, 
					QMetaMethod metaMethod) {
				this.implClass = implClass;
				this.hasCapturedArgs = hasCapturedArgs;
				this.methodHandle = methodHandle;
				this.reflectiveMethod = reflectiveMethod;
				this.isStaticMethod = isStaticMethod;
				this.reflectiveConstructor = reflectiveConstructor;
				this.ownerIndex = ownerIndex;
				this.qobjectIndex = qobjectIndex;
				this.capturingClass = capturingClass;
				if(metaMethod!=null && metaMethod.isValid()) {
					metaObject = metaMethod.enclosingMetaObject();
					methodIndex = metaMethod.methodIndex();
					expectedParameterTypes = metaMethod.parameterCount();
				}else {
					metaObject = null;
					methodIndex = -1;
					expectedParameterTypes = -1;
				}
			}
			public final Class<?> implClass;
			public final boolean hasCapturedArgs;
			public final MethodHandle methodHandle;
			public final Method reflectiveMethod;
			public final boolean isStaticMethod;
			public final Constructor<?> reflectiveConstructor;
			public final QMetaObject metaObject;
			public final int methodIndex;
			public final int expectedParameterTypes;
			final int ownerIndex;
			final int qobjectIndex;
			public final String capturingClass;
			public QMetaMethod metaMethod() {
				return metaObject==null ? new QMetaMethod() : metaObject.method(methodIndex);
			}
		}
		
		LambdaInfo(MethodInfo methodInfo, Object owner, QObject qobject, List<Object> lambdaArgs) {
			super();
			this.methodInfo = methodInfo;
			this.owner = owner;
			this.qobject = qobject;
			this.lambdaArgs = lambdaArgs;
		}

		public final MethodInfo methodInfo;
		public final Object owner;
		public final QObject qobject;
		public final List<Object> lambdaArgs;
	}
	
	/**
	 * @hidden
	 */
	protected static abstract class AbstractSignal extends SignalUtility.AbstractSignal {
		protected AbstractSignal(){
			super();
		}
		
		protected AbstractSignal(Consumer<Object[]> argumentTest){
			super(argumentTest);
		}
	    
		protected AbstractSignal(Class<?> declaringClass) {
            super(declaringClass);
        }
		
		protected AbstractSignal(Class<?> declaringClass, boolean isDisposed) {
            super(declaringClass, isDisposed);
        }
        
		protected AbstractSignal(String signalName, Class<?>[] types) {
            super(signalName, types);
        }
    }
	
	protected static QMetaMethod signalMethod(AbstractSignal signal) {
		return signal.signalMethod();
	}
    
	/**
	 * @hidden
	 */
    protected static abstract class AbstractMultiSignal<Signal extends AbstractSignal> extends SignalUtility.AbstractMultiSignal<Signal> {
    	protected AbstractMultiSignal() {
            super();
        }
    }
    
    protected static void checkConnectionToDisposedSignal(QMetaObject.DisposedSignal signal, Object receiver, boolean slotObject) {
        SignalUtility.checkConnectionToDisposedSignalImpl(signal, receiver, slotObject);
    }
    
    protected static void emitNativeSignal(QObject sender, int methodIndex, long metaObjectId, Object... args) {
    	SignalUtility.emitNativeSignal(null, NativeUtility.checkedNativeId(sender), methodIndex, metaObjectId, 0, args);
    }
    
    protected static boolean disconnectAll(QtSignalEmitterInterface sender, Object receiver) {
    	return SignalUtility.disconnectAll(sender, receiver);
    }
    
    protected static boolean disconnectOne(QMetaObject.Connection connection) {
    	return SignalUtility.disconnectOne(connection);
    }
    
    protected static Object invokeInterfaceDefaultMethod(Method method, Object object, Object... args) throws Throwable {
		return ReflectionUtility.methodInvocationHandler.invokeInterfaceDefaultMethod(method, object, args);
	}
    
    protected static MethodHandle getMethodHandle(Method method) throws IllegalAccessException {
    	return ReflectionUtility.getMethodHandle(method);
    }
    
    protected static void addClassPath(String path) {
    	ResourceUtility.addSearchPath(path);
    }
    
    protected static void removeClassPath(String path) {
    	ResourceUtility.removeSearchPath(path);
    }
    
    protected static void addClassPath(URL path) {
    	ResourceUtility.addSearchPath(path);
    }
    
    protected static <A,B> Function<A,B> functionFromMethod(Method method){
    	return ReflectionUtility.functionFromMethod(method);
    }
    
    protected static void invokeMethod(QObject context, Runnable runnable, boolean blocking) {
    	SignalUtility.invokeMethod(NativeUtility.checkedNativeId(context), runnable, blocking);
    }
    
    protected static LambdaInfo lambdaInfo(Serializable slotObject) {
    	return ClassAnalyzerUtility.lambdaInfo(slotObject);
    }
    
    protected static LambdaInfo lambdaInfo(Serializable slotObject, Object owner) {
    	return ClassAnalyzerUtility.lambdaInfo(slotObject, owner);
    }
    
    protected static LambdaInfo lambdaInfo(Serializable slotObject, QObject qobject) {
    	return ClassAnalyzerUtility.lambdaInfo(slotObject, qobject);
    }
    
    private static <L> L lambdaInfo(LambdaInfo lambdaInfo, QMetaObject.Method8<Object,QObject,QMetaObject,Integer,Integer,Boolean,List<Object>,java.lang.reflect.Method,L> constructor) {
    	if(lambdaInfo!=null) {
	    	try {
	    		return constructor.invoke(lambdaInfo.owner, 
											lambdaInfo.qobject, 
											lambdaInfo.methodInfo.metaObject, 
											lambdaInfo.methodInfo.methodIndex, 
											lambdaInfo.methodInfo.expectedParameterTypes,
											lambdaInfo.methodInfo.isStaticMethod,
											lambdaInfo.lambdaArgs,
											lambdaInfo.methodInfo.reflectiveMethod);
			} catch (Throwable e) {
			}
    	}
    	return null;
    }
    
    protected static <L> L lambdaInfo(Serializable slotObject, Object object, QMetaObject.Method8<Object,QObject,QMetaObject,Integer,Integer,Boolean,List<Object>,java.lang.reflect.Method,L> constructor) {
    	return lambdaInfo(ClassAnalyzerUtility.lambdaInfo(slotObject, object), constructor);
    }
    
    protected static <L> L lambdaInfo(Serializable slotObject, QObject object, QMetaObject.Method8<Object,QObject,QMetaObject,Integer,Integer,Boolean,List<Object>,java.lang.reflect.Method,L> constructor) {
    	return lambdaInfo(ClassAnalyzerUtility.lambdaInfo(slotObject, object), constructor);
    }
    
    protected static <L> L lambdaInfo(Serializable slotObject, QMetaObject.Method8<Object,QObject,QMetaObject,Integer,Integer,Boolean,List<Object>,java.lang.reflect.Method,L> constructor) {
    	return lambdaInfo(slotObject, (Object)null, constructor);
    }
    
    protected static <PI> PI analyzeProperty(QObject containingObject, QtObject property, BiFunction<Field, QMetaType, PI> fun1, BiFunction<Field, QMetaProperty, PI> fun2) {
		return MetaObjectUtility.analyzeProperty(containingObject, property, fun1, fun2);
	}
    
    protected static void registerPropertyField(QMetaProperty metaProperty, java.lang.reflect.Field field) {
    	MetaObjectUtility.registerPropertyField(NativeUtility.nativeId(metaProperty), field);
    }
    
    protected static String internalNameOfArgumentType(Class<? extends Object> cls) {
    	return MetaTypeUtility.internalNameOfArgumentType(cls);
    }
    
    protected static String internalTypeNameOfClass(Class<? extends Object> cls, Type genericType, AnnotatedElement annotatedType) {
    	return MetaTypeUtility.internalTypeNameOfClass(cls, genericType, annotatedType);
    }
    
    protected static String internalTypeName(String s, ClassLoader classLoader) {
    	return MetaTypeUtility.internalTypeName(s, classLoader);
    }
    
    protected static QMetaType[] findSuperInstantiations(Class<?> clazz){
    	return MetaTypeUtility.findSuperInstantiations(clazz);
    }
    
    protected static <T> void registerDataStreamOperators(int metaType, Class<?> classType, java.util.function.BiConsumer<QDataStream, T> datastreamInFn, java.util.function.Function<QDataStream, T> datastreamOutFn){
    	MetaTypeUtility.registerDataStreamOperators(metaType, classType, datastreamInFn, datastreamOutFn);
    }
    
    protected static <T> void registerDebugStreamOperator(int metaType, Class<?> classType, java.util.function.BiConsumer<QDebug, T> debugstreamFn) {
    	MetaTypeUtility.registerDebugStreamOperator(metaType, classType, debugstreamFn);
    }
    
    protected static boolean registerConverter(int metaType1, Class<?> classType1, int metaType2, Class<?> classType2, java.util.function.Function<?,?> converterFn) {
    	return MetaTypeUtility.registerConverter(metaType1, classType1, metaType2, classType2, converterFn);
	}
    
    protected static QMetaMethod fromMethod(java.io.Serializable method) {
        var info = lambdaInfo(method);
        if (info != null)
            return info.methodInfo.metaMethod();
        return new QMetaMethod();
    }
    
    @SuppressWarnings("deprecation")
	protected static URL createURL(String url) throws MalformedURLException {
    	try {
			return new URL(url);
		} catch (NoSuchMethodError e) {
			return URI.create(url).toURL();
		}
    }
    
    protected static Class<?> getFactoryClass(Serializable method) {
    	LambdaInfo lamdaInfo = lambdaInfo(method);
        if (lamdaInfo != null) {
            if (lamdaInfo.methodInfo.reflectiveMethod != null
                    && (lamdaInfo.lambdaArgs == null || lamdaInfo.lambdaArgs.isEmpty())
                    && !lamdaInfo.methodInfo.reflectiveMethod.isSynthetic()
                    && !lamdaInfo.methodInfo.reflectiveMethod.isBridge()
                    && !Modifier.isStatic(lamdaInfo.methodInfo.reflectiveMethod.getModifiers())) {
                return lamdaInfo.methodInfo.reflectiveMethod.getDeclaringClass();
            } else if (lamdaInfo.methodInfo.reflectiveConstructor != null
                    && (lamdaInfo.lambdaArgs == null || lamdaInfo.lambdaArgs.isEmpty())
                    && !lamdaInfo.methodInfo.reflectiveConstructor.isSynthetic()
                    && !Modifier.isStatic(lamdaInfo.methodInfo.reflectiveConstructor.getModifiers())) {
                return lamdaInfo.methodInfo.reflectiveConstructor.getDeclaringClass();
            }
        }
        return null;
    }
    
    protected static <S extends Serializable, Bindable> @NonNull Bindable fromProperty(Class<S> type, S propertyGetter, BiFunction<QObject, QMetaProperty, Bindable> constr){
    	var info = CoreUtility.lambdaInfo(propertyGetter);
		if(info!=null && info.qobject!=null && info.methodInfo.reflectiveMethod!=null && !info.methodInfo.reflectiveMethod.isSynthetic()) {
			QtPropertyReader pr = info.methodInfo.reflectiveMethod.getAnnotation(QtPropertyReader.class);
			if(pr!=null) {
				if(pr.enabled() && !pr.name().isEmpty()) {
					QMetaProperty prp = info.qobject.metaObject().property(pr.name());
					if(prp!=null && prp.isValid()) {
						return constr.apply(info.qobject, prp);
					}
				}
			}else{
				int[] lambdaMetaTypes = ClassAnalyzerUtility.lambdaMetaTypes(type, propertyGetter);
				if(lambdaMetaTypes!=null && lambdaMetaTypes.length==1) {
					QMetaProperty prp = info.qobject.metaObject().property(info.methodInfo.reflectiveMethod.getName());
					if(prp!=null && prp.isValid() && lambdaMetaTypes[0]==prp.typeId()) {
						return constr.apply(info.qobject, prp);
					}
					boolean isIs = false;
					if(info.methodInfo.reflectiveMethod.getName().startsWith("get") || info.methodInfo.reflectiveMethod.getName().startsWith("has") || (isIs = info.methodInfo.reflectiveMethod.getName().startsWith("is"))) {
						String name = info.methodInfo.reflectiveMethod.getName().substring(isIs ? 2 : 3);
						if(name.length()>1) {
							name = Character.toLowerCase(name.charAt(0)) + name.substring(1);
							prp = info.qobject.metaObject().property(name);
							if(prp!=null && prp.isValid() && lambdaMetaTypes[0]==prp.typeId()) {
								return constr.apply(info.qobject, prp);
							}
						}
					}
				}
			}
			throw new IllegalArgumentException(String.format("Unable to determine property from method %1$s.", info.methodInfo.reflectiveMethod.toGenericString()));
		}else {
			throw new IllegalArgumentException("Unable to determine property from given method.");
		}
    }
}
