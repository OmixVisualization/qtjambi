/****************************************************************************
**
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import java.lang.invoke.MethodHandle;
import java.lang.reflect.AnnotatedElement;
import java.lang.reflect.GenericArrayType;
import java.lang.reflect.InvocationHandler;
import java.lang.reflect.Method;
import java.lang.reflect.Modifier;
import java.lang.reflect.Proxy;
import java.lang.reflect.Type;
import java.nio.Buffer;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import io.qt.NativeAccess;
import io.qt.QNativePointer;
import io.qt.QtMetaType;
import io.qt.QtObjectInterface;
import io.qt.QtPointerType;
import io.qt.QtReferenceType;
import io.qt.QtUtilities;

final class QFunctionPointerUtil {
	private QFunctionPointerUtil() {}
	
	@NativeAccess
	private static <T extends io.qt.QtObjectInterface> T createProxy(Class<T> functionalInterface) {
		if(!functionalInterface.isInterface())
			throw new IllegalArgumentException(functionalInterface.getName().replace('$', '.')+" is not an interface.");
		if(!functionalInterface.isAnnotationPresent(FunctionalInterface.class))
			throw new IllegalArgumentException(functionalInterface.getName().replace('$', '.')+" is not a functional interface. Annotation @FunctionalInterface expected.");
		Method functionMethod = null;
		for(Method method : functionalInterface.getDeclaredMethods()) {
			if(!Modifier.isStatic(method.getModifiers()) && !method.isDefault()) {
				if(functionMethod!=null)
					throw new IllegalArgumentException(functionalInterface.getName().replace('$', '.')+" is not a functional interface.");
				functionMethod = method;
			}
		}
		if(functionMethod==null)
			throw new IllegalArgumentException(functionalInterface.getName().replace('$', '.')+" is not a functional interface.");
		return functionalInterface.cast(Proxy.newProxyInstance(
				functionalInterface.getClassLoader(), 
				new Class[] {functionalInterface},
				invocationHandlers.computeIfAbsent(functionMethod, JavaToCppInvocationHandler::new)
			));
	}
	
	private static final Map<Method, InvocationHandler> invocationHandlers = Collections.synchronizedMap(new HashMap<>());
	
	private static abstract class AbstractInvocationHandler implements InvocationHandler{
		final QGenericReturnType<?> returnType;
		final QGenericArgumentType<?>[] argumentTypes;
		
		AbstractInvocationHandler(Method functionMethod) {
			super();
			QtMetaType metaTypeDecl = null;
			QtPointerType pointerType = null;
			QtReferenceType referenceType = null;
			if(QtJambi_LibraryUtilities.internal.useAnnotatedType()) {
				if(functionMethod.getAnnotatedReturnType()!=null) {
					metaTypeDecl = functionMethod.getAnnotatedReturnType().getAnnotation(QtMetaType.class);
					pointerType = functionMethod.getAnnotatedReturnType().getAnnotation(QtPointerType.class);
					referenceType = functionMethod.getAnnotatedReturnType().getAnnotation(QtReferenceType.class);
				}
			}
			if(metaTypeDecl==null)
				metaTypeDecl = functionMethod.getAnnotation(QtMetaType.class);
			QGenericReturnType<?> returnType;
			if(metaTypeDecl!=null) {
				QMetaType metaType;
				if(metaTypeDecl.id()!=0) {
					metaType = new QMetaType(metaTypeDecl.id());
				}else if(metaTypeDecl.type()!=QMetaType.Type.UnknownType){
					metaType = new QMetaType(metaTypeDecl.type());
				}else {
					int id = QtJambi_LibraryUtilities.internal.findMetaType(metaTypeDecl.name());
					if(id==0) {
						if(metaTypeDecl.name().isEmpty())
							throw new IllegalArgumentException("Incomplete @QtMetaType declaration. Either use type, id or name to specify meta type.");
						else
							throw new IllegalArgumentException("Unable to detect meta type "+metaTypeDecl.name());
					}
					metaType = new QMetaType(id);
				}
				returnType = new QGenericReturnType<>(functionMethod.getReturnType(), metaType, 0);
				if(pointerType!=null)
					returnType = returnType.asPointer();
				else if(referenceType!=null) {
					if(referenceType.isConst())
						returnType = returnType.asConstRef();
					else
						returnType = returnType.asRef();
				}
			}else if(functionMethod.getReturnType().isArray() 
					|| (functionMethod.getReturnType().isPrimitive() && pointerType==null && referenceType==null)
					|| Buffer.class.isAssignableFrom(functionMethod.getReturnType())
					|| functionMethod.getReturnType().getName().startsWith("com.sun.jna.")) {
				returnType = new QGenericReturnType<>(functionMethod.getReturnType(), null, 0);
			}else {
				if(functionMethod.getReturnType().isInterface() 
						&& QtObjectInterface.class.isAssignableFrom(functionMethod.getReturnType())
						&& functionMethod.getReturnType().isAnnotationPresent(FunctionalInterface.class)
						&& QtJambi_LibraryUtilities.internal.findGeneratedSuperclass(functionMethod.getReturnType())==null) {
					returnType = new QGenericReturnType<>(functionMethod.getReturnType(), null, 2);
				}else {
					AnnotatedElement annotatedReturnType = null;
					if(QtJambi_LibraryUtilities.internal.useAnnotatedType())
						annotatedReturnType = functionMethod.getAnnotatedReturnType();
					int metaType = QtJambi_LibraryUtilities.internal.registerMetaType(functionMethod.getReturnType(), functionMethod.getGenericReturnType(), annotatedReturnType, false, false);
					if(metaType!=0) {
						returnType = new QGenericReturnType<>(functionMethod.getReturnType(), new QMetaType(metaType), 0);
						if(referenceType!=null && referenceType.isConst()) {
							returnType = returnType.asConstRef();
						}
					}else {
						returnType = new QGenericReturnType<>(functionMethod.getReturnType(), null, 0);
						if(pointerType!=null)
							returnType = returnType.asPointer();
						else if(referenceType!=null) {
							if(referenceType.isConst())
								returnType = returnType.asConstRef();
							else
								returnType = returnType.asRef();
						}
					}
				}
			}
			QGenericArgumentType<?>[] argumentTypes = new QGenericArgumentType[functionMethod.getParameterCount()];
            Class<?>[] parameterTypes = functionMethod.getParameterTypes();
            Type[] genericParameterTypes = functionMethod.getGenericParameterTypes();
            AnnotatedElement[] annotatedParameterTypes = null;
            if(QtJambi_LibraryUtilities.internal.useAnnotatedType())
            	annotatedParameterTypes = functionMethod.getAnnotatedParameterTypes();
			for (int i = 0; i < argumentTypes.length; i++) {
				if(annotatedParameterTypes!=null && annotatedParameterTypes[i]!=null) {
					pointerType = annotatedParameterTypes[i].getAnnotation(QtPointerType.class);
					referenceType = annotatedParameterTypes[i].getAnnotation(QtReferenceType.class);
					metaTypeDecl = annotatedParameterTypes[i].getAnnotation(QtMetaType.class);
				}
				QGenericArgumentType<?> argumentType;
				Class<?> parameterType = parameterTypes[i];
				Type genericParameterType = genericParameterTypes[i];
				if(functionMethod.isVarArgs() && parameterType.isArray()) {
					parameterType = parameterType.getComponentType();
					if(genericParameterType instanceof GenericArrayType) {
						genericParameterType = ((GenericArrayType)genericParameterType).getGenericComponentType();
					}else {
						genericParameterType = parameterType;
					}
				}
				if(metaTypeDecl!=null) {
					QMetaType metaType;
					if(metaTypeDecl.id()!=0) {
						metaType = new QMetaType(metaTypeDecl.id());
					}else if(metaTypeDecl.type()!=QMetaType.Type.UnknownType){
						metaType = new QMetaType(metaTypeDecl.type());
					}else {
						int id = QtJambi_LibraryUtilities.internal.findMetaType(metaTypeDecl.name());
						if(id==0) {
							if(metaTypeDecl.name().isEmpty())
								throw new IllegalArgumentException("Incomplete @QtMetaType declaration. Either use type, id or name to specify meta type.");
							else
								throw new IllegalArgumentException("Unable to detect meta type "+metaTypeDecl.name());
						}
						metaType = new QMetaType(id);
					}
					argumentType = new QGenericReturnType<>(parameterType, metaType, 0);
					if(pointerType!=null)
						argumentType = argumentType.asPointer();
					else if(referenceType!=null) {
						if(referenceType.isConst())
							argumentType = argumentType.asConstRef();
						else
							argumentType = argumentType.asRef();
					}
				}else if(parameterType.isArray() 
						|| (parameterType.isPrimitive() && pointerType==null && referenceType==null)
						|| Buffer.class.isAssignableFrom(parameterType)
						|| parameterType.getName().startsWith("com.sun.jna.")) {
					argumentType = new QGenericReturnType<>(parameterType, null, 0);
				}else {
					if(parameterType.isInterface() 
							&& QtObjectInterface.class.isAssignableFrom(parameterType)
							&& parameterType.isAnnotationPresent(FunctionalInterface.class)
							&& QtJambi_LibraryUtilities.internal.findGeneratedSuperclass(parameterType)==null) {
						argumentType = new QGenericReturnType<>(parameterType, null, 2);
					}else {
						int metaType = QtJambi_LibraryUtilities.internal.registerMetaType(parameterType, genericParameterType, annotatedParameterTypes==null ? null : annotatedParameterTypes[i], false, false);
						if(metaType!=0) {
							argumentType = new QGenericReturnType<>(parameterType, new QMetaType(metaType), 0);
							if(referenceType!=null && referenceType.isConst()) {
								argumentType = argumentType.asConstRef();
							}
						}else {
							argumentType = new QGenericReturnType<>(parameterType, null, 0);
							if(pointerType!=null)
								argumentType = argumentType.asPointer();
							else if(referenceType!=null) {
								if(referenceType.isConst())
									argumentType = argumentType.asConstRef();
								else
									argumentType = argumentType.asRef();
							}
						}
					}
				}
				argumentTypes[i] = argumentType;
			}
			this.returnType = returnType;
			this.argumentTypes = argumentTypes;
		}
	}
	
	private static final class JavaToCppInvocationHandler extends AbstractInvocationHandler{
		
		private final Class<?> declaringClass;
		
		JavaToCppInvocationHandler(Method functionMethod) {
			super(functionMethod);
			declaringClass = functionMethod.getDeclaringClass();
		}
		
		@Override
		public Object invoke(Object proxy, Method method, Object[] args) throws Throwable {
			if(proxy instanceof QtObjectInterface) {
				QtObjectInterface iface = (QtObjectInterface)proxy;
				if(Modifier.isAbstract(method.getModifiers())) {
					if(method.isVarArgs()) {
						List<QGenericArgument> arguments = new ArrayList<>();
						int j = argumentTypes.length-1;
						for (int i = 0; i < (args==null ? 0 : args.length); i++) {
							if(i<j) {
								arguments.add(new QGenericArgument(argumentTypes[i].classType, argumentTypes[i].metaType, argumentTypes[i].pointerOrReference, args[i]));
							}else {
								Object[] varArg = (Object[])args[i];
								for (int k = 0; k < varArg.length; k++) {
									arguments.add(new QGenericArgument(argumentTypes[j].classType, argumentTypes[j].metaType, argumentTypes[j].pointerOrReference, varArg[k]));
								}
							}
						}
						return QFunctionPointer.invoke_native(iface, returnType, arguments.toArray(new Object[arguments.size()]));
					}else {
						Object[] arguments = new Object[argumentTypes.length];
						for (int i = 0; i < arguments.length; i++) {
							arguments[i] = new QGenericArgument(argumentTypes[i].classType, argumentTypes[i].metaType, argumentTypes[i].pointerOrReference, args[i]);
						}
						return QFunctionPointer.invoke_native(iface, returnType, arguments);
					}
				}else if(method.getDeclaringClass()==Object.class){
					switch(method.getName()) {
					case "hashCode":
						return Long.hashCode(QNativePointer.fromObject(iface).pointerAt(0).pointer());
					case "equals":
						if(args[0] instanceof QtObjectInterface) {
							if(Proxy.isProxyClass(args[0].getClass())) {
								InvocationHandler ih = Proxy.getInvocationHandler(args[0]);
								return ih==this && QNativePointer.fromObject(iface).pointerAt(0).pointer()
										== QNativePointer.fromObject((QtObjectInterface)args[0]).pointerAt(0).pointer();
							}
						}
						return false;
					case "toString":
						return String.format("%1$s@%2$s", declaringClass.getName(), Long.toHexString(QNativePointer.fromObject(iface).pointerAt(0).pointer()));
					}
				}else if(method.isDefault()) {
					return CoreUtility.invokeInterfaceDefaultMethod(method, proxy, args);
				}
			}
			return null;
		}
	}
	
	private static class CoreUtility extends io.qt.internal.CoreUtility{
		protected static Object invokeInterfaceDefaultMethod(Method method, Object object, Object... args) throws Throwable {
			return io.qt.internal.CoreUtility.invokeInterfaceDefaultMethod(method, object, args);
		}
		protected static MethodHandle getMethodHandle(Method method) throws IllegalAccessException {
			return io.qt.internal.CoreUtility.getMethodHandle(method);
		}
	}
	
	private static native Class<?> returnType(QGenericReturnType<?> returnType);
	private static native void getParameterTypes(QGenericArgumentType<?>[] argumentTypes, Class<?>[] parameterTypes, boolean isVarArgs);
	private static native void convertParameters(QGenericArgumentType<?>[] argumentTypes, Object[] args, Object[] convertedArgs, boolean isVarArgs, boolean forward);
	private static native Object convertReturn(QGenericReturnType<?> returnType, Object result);
	private static native void dispose(long callback);
	
	private static final class CppToJavaInvocationHandler extends AbstractInvocationHandler{
		
		private final boolean isVarArgs;
		private final MethodHandle functionMethod;
		private final QtObjectInterface functionalInterface;
		@NativeAccess Object proxy;
		@NativeAccess private long peer;
		private final Class<?> returnType;
		private final Class<?>[] parameterTypes;

		public CppToJavaInvocationHandler(Method functionMethod, QtObjectInterface functionalInterface) throws IllegalAccessException {
			super(functionMethod);
			isVarArgs = functionMethod.isVarArgs();
			this.functionMethod = CoreUtility.getMethodHandle(functionMethod);
			this.functionalInterface = functionalInterface;
			returnType = returnType(super.returnType);
			parameterTypes = new Class[argumentTypes.length];
			getParameterTypes(argumentTypes, parameterTypes, isVarArgs);
		}

		@Override
		public Object invoke(Object proxy, Method method, Object[] args) throws Throwable {
			if(method.getDeclaringClass()==Object.class){
				return method.invoke(functionalInterface, args);
			}else {
				switch(method.getName()) {
				case "callback":
					Object[] newArgs = new Object[argumentTypes.length + 1];
					newArgs[0] = functionalInterface;
					args = (Object[])args[0];
					convertParameters(argumentTypes, args, newArgs, isVarArgs, true);
					try {
						Object result = functionMethod.invokeWithArguments(newArgs);
						result = convertReturn(super.returnType, result);
						//convertParameters(argumentTypes, args, newArgs, functionMethod.isVarArgs(), false);
						return result;
					} catch (Exception e) {
						e.printStackTrace();
						throw e;
					}
				case "getParameterTypes":
					return Arrays.copyOf(parameterTypes, parameterTypes.length);
				case "getReturnType":
					return returnType;
				}
			}
			return null;
		}
	}
	
	@NativeAccess
	private static CppToJavaInvocationHandler createCallbackProxy(Class<?> callbackProxyInterface, Class<?> functionalInterfaceClass, QtObjectInterface functionalInterface) throws IllegalAccessException {
		if(!functionalInterfaceClass.isInterface())
			throw new IllegalArgumentException(functionalInterfaceClass.getName().replace('$', '.')+" is not an interface.");
		if(!functionalInterfaceClass.isAnnotationPresent(FunctionalInterface.class))
			throw new IllegalArgumentException(functionalInterfaceClass.getName().replace('$', '.')+" is not a functional interface. Annotation @FunctionalInterface expected.");
		Method functionMethod = null;
		for(Method method : functionalInterfaceClass.getDeclaredMethods()) {
			if(!Modifier.isStatic(method.getModifiers()) && !method.isDefault()) {
				if(functionMethod!=null)
					throw new IllegalArgumentException(functionalInterfaceClass.getName().replace('$', '.')+" is not a functional interface.");
				functionMethod = method;
			}
		}
		if(functionMethod==null)
			throw new IllegalArgumentException(functionalInterfaceClass.getName().replace('$', '.')+" is not a functional interface.");
		CppToJavaInvocationHandler invocationHandler = new CppToJavaInvocationHandler(functionMethod, functionalInterface);
		invocationHandler.proxy = Proxy.newProxyInstance(callbackProxyInterface.getClassLoader(), new Class[] {callbackProxyInterface}, invocationHandler);
		return invocationHandler;
	}
	
	@NativeAccess
	private static void registerCleanup(QtObjectInterface functionalInterface, CppToJavaInvocationHandler invocationHandler) {
		QtUtilities.getSignalOnDispose(functionalInterface).connect(()->{
			dispose(invocationHandler.peer);
		});		
	}
}
