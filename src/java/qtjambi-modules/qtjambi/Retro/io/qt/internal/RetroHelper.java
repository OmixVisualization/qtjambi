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
import java.net.MalformedURLException;
import java.util.HashSet;
import java.util.Optional;
import java.util.Set;
import java.util.function.IntFunction;
import java.util.function.Supplier;

import io.qt.InternalAccess.CallerContext;

final class RetroHelper {
	private RetroHelper() {throw new RuntimeException();}
	
	private static interface Implementor{
		Supplier<Class<?>> callerClassProvider();
		IntFunction<io.qt.InternalAccess.CallerContext> classAccessChecker();
		String processName();
		AnnotatedType getAnnotatedOwnerType(AnnotatedType actualType);
	}
	
	private static class DefaultImplementor implements Implementor{
		private final StackWalker stackWalker = StackWalker.getInstance(java.util.Collections.singleton(StackWalker.Option.RETAIN_CLASS_REFERENCE));
	    private final Supplier<Class<?>> callerClassProvider = stackWalker::getCallerClass;
	    private final IntFunction<io.qt.InternalAccess.CallerContext> classAccessChecker = number->{
	    	if(number>0) {
		        Optional<StackWalker.StackFrame> stackFrame = stackWalker.walk(stream->stream.limit(number).skip(number-1).findFirst());
		        if(stackFrame.isPresent()) {
		            return new io.qt.InternalAccess.CallerContext(
		                stackFrame.get().getDeclaringClass(),
		                stackFrame.get().getMethodName(), 
		                stackFrame.get().getLineNumber());
		        }
	        }
	        return null;
	    };
	    private String processName = ""+ProcessHandle.current().pid();
	    
	    @Override
	    public Supplier<Class<?>> callerClassProvider(){
	        return callerClassProvider;
	    }
	    
	    @Override
	    public IntFunction<io.qt.InternalAccess.CallerContext> classAccessChecker(){
	        return classAccessChecker;
	    }
	    
	    @Override
	    public String processName() {
	    	return processName;
	    }
	    
	    @Override
	    public AnnotatedType getAnnotatedOwnerType(AnnotatedType actualType) {
	        try {
				return actualType.getAnnotatedOwnerType();
			} catch (Throwable e) {
				return null;
			}
	    }
	}
	
	private static class Java8Implementor implements Implementor{
	    Java8Implementor() {
			super();
			MethodHandle _getAnnotatedOwnerTypeHandle = null;
	        try {
	            Method getAnnotatedOwnerType = AnnotatedType.class.getMethod("getAnnotatedOwnerType");
	            _getAnnotatedOwnerTypeHandle = QtJambiInternal.getMethodHandle(getAnnotatedOwnerType);
	        } catch (Throwable e2) {
	        }
	        this.getAnnotatedOwnerTypeHandle = _getAnnotatedOwnerTypeHandle;
	        
	        this.classAccessChecker = number->{
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
            Supplier<Class<?>> _callerClassProvider;
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
				_callerClassProvider = ()->{
	                Class<?> result = Object.class;
	                io.qt.InternalAccess.CallerContext info = classAccessChecker().apply(4);
	                if(info!=null && info.declaringClass!=null) {
	                    result = info.declaringClass;
	                }
	                return result;
	            };
			}
			
			this.callerClassProvider = _callerClassProvider;
			
			String pid = "";
			try {
				Class<?> factoryClass = Class.forName("java.lang.management.ManagementFactory");
				Method getRuntimeMXBean = factoryClass.getMethod("getRuntimeMXBean");
				Object runtimeMXBean = getRuntimeMXBean.invoke(null);
				Method getName = runtimeMXBean.getClass().getMethod("getName");
				pid = (String)getName.invoke(runtimeMXBean);
			} catch (Throwable e) {
			}
	        int index = pid.indexOf('@');
	        if(index>0)
	            pid = pid.substring(0, index);
	        this.processName = pid;
		}

		private final IntFunction<io.qt.InternalAccess.CallerContext> classAccessChecker;
	    private final Supplier<Class<?>> callerClassProvider;
	    private final String processName;
	    private final MethodHandle getAnnotatedOwnerTypeHandle;

		@Override
		public Supplier<Class<?>> callerClassProvider() {
			return callerClassProvider;
		}

		@Override
		public IntFunction<CallerContext> classAccessChecker() {
			return classAccessChecker;
		}
		
		@Override
		public String processName() {
	    	return processName;
	    }
		
		@Override
	    public AnnotatedType getAnnotatedOwnerType(AnnotatedType actualType) {
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
	}
	
    private static final Implementor implementor;
    static{
    	Implementor _implementor;
    	try {
			_implementor = new DefaultImplementor();
		} catch (Throwable e) {
			_implementor = new Java8Implementor();
		}
    	implementor = _implementor;
    }
    
    static Supplier<Class<?>> callerClassProvider(){
        return implementor.callerClassProvider();
    }
    
    static IntFunction<io.qt.InternalAccess.CallerContext> classAccessChecker(){
        return implementor.classAccessChecker();
    }
    
    static AnnotatedType getAnnotatedOwnerType(AnnotatedType actualType) {
		return implementor.getAnnotatedOwnerType(actualType);
    }
    
    static String processName() {
    	return implementor.processName();
    }
    
    static Set<ClassLoader> classLoaders(){
    	Set<ClassLoader> result = new HashSet<>();
    	result.add(Thread.currentThread().getContextClassLoader());
    	result.add(RetroHelper.class.getClassLoader());
    	result.add(ClassLoader.getSystemClassLoader());
    	try {
    		result.add(ClassLoader.getPlatformClassLoader());
		} catch (Throwable e) {}
    	return result;
    }
    
    static void findModules(Set<java.net.URL> modules) throws MalformedURLException{
    	try {
	    	for(Module module : ModuleLayer.boot().modules()) {
	    		modules.add(new java.net.URL("jrt:/"+module.getName()+"/"));
	    	}
		} catch (Throwable e) {}
    }
    
    static Thread newShutdownThread(Runnable r, String name) {
    	try {
			return new Thread(null, r, name, 0, false);
		} catch (Throwable e) {
			return new Thread(null, r, name, 0);
		}
    }
}
