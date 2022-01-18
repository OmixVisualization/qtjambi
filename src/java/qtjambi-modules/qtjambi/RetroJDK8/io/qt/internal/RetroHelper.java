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

import java.lang.invoke.MethodHandle;
import java.lang.reflect.AnnotatedType;
import java.lang.reflect.Method;
import java.lang.management.ManagementFactory;
import java.util.function.Supplier;
import java.util.stream.Stream;
import java.util.Optional;
import java.util.function.Function;
import java.util.function.IntFunction;

final class RetroHelper {
    private RetroHelper() {throw new RuntimeException();}
    
    static AnnotatedType getAnnotatedOwnerType(AnnotatedType actualType) {
    	if(getAnnotatedOwnerTypeHandle!=null) {
    		try{
    			return (AnnotatedType)getAnnotatedOwnerTypeHandle.invoke(actualType);
			} catch (RuntimeException | Error e) {
				throw e;
			} catch (Throwable e) {
				throw new RuntimeException(e);
    		}
    	}
        return null;
    }
    
    private static IntFunction<io.qt.InternalAccess.CallerContext> classAccessChecker;
    private static Supplier<Class<?>> callerClassProvider;
    private static MethodHandle getAnnotatedOwnerTypeHandle;
    
    static {
    	MethodHandle _getAnnotatedOwnerTypeHandle = null;
    	try {
			Method getAnnotatedOwnerType = AnnotatedType.class.getMethod("getAnnotatedOwnerType");
			_getAnnotatedOwnerTypeHandle = QtJambiInternal.getMethodHandle(getAnnotatedOwnerType);
		} catch (Throwable e2) {
		}
    	Supplier<Class<?>> _callerClassProvider = null;
    	IntFunction<io.qt.InternalAccess.CallerContext> _classAccessChecker = null;
    	try {
			Class<?> stackWalkerClass = Class.forName("java.lang.StackWalker");
			Method getInstance = stackWalkerClass.getMethod("getInstance", java.util.Set.class);
			@SuppressWarnings("rawtypes")
			Class stackWalkerOptionClass = (Class)Class.forName("java.lang.StackWalker$Option");
			@SuppressWarnings("unchecked")
			Object RETAIN_CLASS_REFERENCE = java.util.Collections.singleton(Enum.valueOf(stackWalkerOptionClass, "RETAIN_CLASS_REFERENCE"));
			Object stackWalker = getInstance.invoke(null, RETAIN_CLASS_REFERENCE);
			Class<?> stackWalkerStackFrameClass = Class.forName("java.lang.StackWalker$StackFrame");
			Method getDeclaringClass = stackWalkerStackFrameClass.getMethod("getDeclaringClass");
			MethodHandle getDeclaringClassHandle = QtJambiInternal.getMethodHandle(getDeclaringClass);
			Method getMethodName = stackWalkerStackFrameClass.getMethod("getMethodName");
			MethodHandle getMethodNameHandle = QtJambiInternal.getMethodHandle(getMethodName);
			Method getLineNumber = stackWalkerStackFrameClass.getMethod("getLineNumber");
			MethodHandle getLineNumberHandle = QtJambiInternal.getMethodHandle(getLineNumber);
			Method walk = stackWalkerClass.getMethod("walk", Function.class);
			MethodHandle walkHandle = QtJambiInternal.getMethodHandle(walk);
			_classAccessChecker = number->{
				try {
					if(number>0) {
						Function<Stream<?>, Optional<?>> fun = stream->stream.limit(number).skip(number-1).findFirst();
				        Optional<?> stackFrameOpt = (Optional<?>)walkHandle.invoke(stackWalker, fun);
				        if(stackFrameOpt.isPresent()) {
				        	Object stackFrame = stackFrameOpt.get();
				            return new io.qt.InternalAccess.CallerContext(
								            		(Class<?>)getDeclaringClassHandle.invoke(stackFrame),
								            		(String)getMethodNameHandle.invoke(stackFrame), 
								            		(int)getLineNumberHandle.invoke(stackFrame));
				        }
			        }
		            return null;
				} catch (RuntimeException | Error e) {
					throw e;
				} catch (Throwable e) {
					throw new RuntimeException(e);
				}
		    };
		} catch (Throwable e1) {
		}
    	
    	if(_callerClassProvider==null) {
	    	try {
				Class<?> reflectionClass = Class.forName("sun.reflect.Reflection");
				Method getCallerClass = reflectionClass.getMethod("getCallerClass", int.class);
				MethodHandle getCallerClassHandle = QtJambiInternal.getMethodHandle(getCallerClass).bindTo(4);
				_callerClassProvider = ()->{
					try {
						return (Class<?>)getCallerClassHandle.invoke();
					} catch (RuntimeException | Error e) {
						throw e;
					} catch (Throwable e) {
						throw new RuntimeException(e);
					}
				};
			} catch (Throwable e) {
			}
    	}
    	if(_callerClassProvider==null) {
    		_callerClassProvider = ()->{
    			Class<?> result = Object.class;
    			io.qt.InternalAccess.CallerContext info = classAccessChecker().apply(4);
    			if(info!=null && info.declaringClass!=null) {
    				result = info.declaringClass;
    			}
    			return result;
    		};
    	}
    	if(_classAccessChecker==null) 
    	{
    		_classAccessChecker = number->{
    	        StackTraceElement[] stackTrace = Thread.currentThread().getStackTrace();
    	        ++number; // for getStackTrace()
    	        for (StackTraceElement element : stackTrace) {
					if(!element.getClassName().startsWith("jdk.internal.reflect")
							&& !element.getClassName().startsWith("java.lang.reflect")
                            && !element.getClassName().startsWith("io.qt.internal.RetroHelper$$Lambda$"))
						--number;
					if(number==0){
	    	            try {
	    	                return new io.qt.InternalAccess.CallerContext(Class.forName(element.getClassName()), element.getMethodName(), element.getLineNumber());
	    	            } catch (ClassNotFoundException e) {
	    	                //e.printStackTrace();
                            ++number;
	    	            }
	    	        }
				}
    	        return null;
    	    };
    	}
    	callerClassProvider = _callerClassProvider;
    	classAccessChecker = _classAccessChecker;
    	getAnnotatedOwnerTypeHandle = _getAnnotatedOwnerTypeHandle;
    }
    
    static IntFunction<io.qt.InternalAccess.CallerContext> classAccessChecker(){
        return classAccessChecker;
    }
    
    static Supplier<Class<?>> callerClassProvider(){
        return callerClassProvider;
    }
    
    static String processName() {
    	return ManagementFactory.getRuntimeMXBean().getName();
    }
    
    static void onExit(Runnable runnable) {
    	Runtime.getRuntime().addShutdownHook(new Thread(runnable));
    }
}
