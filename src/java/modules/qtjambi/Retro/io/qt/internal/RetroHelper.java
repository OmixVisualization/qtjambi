/****************************************************************************
**
** Copyright (C) 2009-2022 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import java.io.File;
import java.lang.invoke.MethodType;
import java.lang.reflect.AnnotatedElement;
import java.lang.reflect.Field;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.net.MalformedURLException;
import java.util.HashSet;
import java.util.Optional;
import java.util.Set;
import java.util.function.Function;
import java.util.function.IntFunction;
import java.util.function.Supplier;
import java.util.logging.Logger;

import io.qt.InternalAccess.CallerContext;

final class RetroHelper {
	private RetroHelper() {throw new RuntimeException();}
	
	private static interface Implementor{
		Supplier<Class<?>> callerClassProvider();
		IntFunction<io.qt.InternalAccess.CallerContext> classAccessChecker();
		String processName();
		boolean isProcessAlive(String pid);
		<T extends AnnotatedElement> T getAnnotatedOwnerType(T actualType);
	}
	
	private static class DefaultImplementor implements Implementor{
		private final StackWalker stackWalker = StackWalker.getInstance(java.util.Collections.singleton(StackWalker.Option.RETAIN_CLASS_REFERENCE));
	    private final Supplier<Class<?>> callerClassProvider = stackWalker::getCallerClass;
	    private final IntFunction<io.qt.InternalAccess.CallerContext> classAccessChecker = number->{
	    	if(number>=0) {
		        Optional<StackWalker.StackFrame> stackFrame = stackWalker.walk(stream->stream.limit(number+2).skip(number+1).findFirst());
		        if(stackFrame.isPresent() && stackFrame.get().getDeclaringClass()==RetroHelper.class) {
		        	stackFrame = stackWalker.walk(stream->stream.limit(number+3).skip(number+2).findFirst());
		        }
		        if(stackFrame.isPresent()) {
		        	String methodDescriptor = null;
		        	MethodType methodType = null;
		        	try {
						methodDescriptor = stackFrame.get().getDescriptor();
					} catch (UnsupportedOperationException e) {}
		        	try{
		        		methodType = stackFrame.get().getMethodType();
		        	} catch (UnsupportedOperationException e) {}
		            return new io.qt.InternalAccess.CallerContext(
		                stackFrame.get().getDeclaringClass(),
		                stackFrame.get().getMethodName(),
		                methodDescriptor,
		                methodType,
		                stackFrame.get().getLineNumber());
		        }
	        }
	        return null;
	    };
	    private String processName = Long.toUnsignedString(ProcessHandle.current().pid());
	    
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
	    
	    @SuppressWarnings("unchecked")
		@Override
	    public <T extends AnnotatedElement> T getAnnotatedOwnerType(T actualType){
	        try {
	        	if(actualType instanceof java.lang.reflect.AnnotatedType)
	        		return (T)((java.lang.reflect.AnnotatedType)actualType).getAnnotatedOwnerType();
			} catch (Throwable e) {
			}
			return null;
	    }

		@Override
		public boolean isProcessAlive(String pid) {
			ProcessHandle process = ProcessHandle.of(Long.parseUnsignedLong(pid)).orElse(null);
			return process!=null && process.isAlive();
		}
	}
	
	private static class Java8Implementor implements Implementor{
	    Java8Implementor() {
			super();
			Function<AnnotatedElement, AnnotatedElement> getAnnotatedOwnerTypeSupplier = null;
	        try {
	        	Method getAnnotatedOwnerType = java.lang.reflect.AnnotatedType.class.getMethod("getAnnotatedOwnerType");
	        	getAnnotatedOwnerTypeSupplier = a->{
					try {
						return (AnnotatedElement)getAnnotatedOwnerType.invoke(a);
					} catch (RuntimeException | Error e) {
						throw e;
					} catch (InvocationTargetException e) {
						try {
							throw e.getTargetException();
						} catch (RuntimeException | Error e2) {
							throw e2;
						}catch(Throwable t) {
							throw new RuntimeException(t);
						}
					}catch(Throwable t) {
						throw new RuntimeException(t);
					}
				};
	        } catch (Throwable e2) {
	        }
	        this.getAnnotatedOwnerTypeSupplier = getAnnotatedOwnerTypeSupplier;
	        
	        this.classAccessChecker = number->{
                StackTraceElement[] stackTrace = Thread.currentThread().getStackTrace();
                boolean postStackTrace = false;
                for (StackTraceElement element : stackTrace) {
                	if(!postStackTrace) {
                		if(element.getClassName().equals(Thread.class.getName())
                				&& element.getMethodName().equals("getStackTrace")) {
                			postStackTrace = true;
                			--number;
                		}
                	}else {
	                    if(!element.getClassName().startsWith("jdk.internal.reflect")
	                            && !element.getClassName().startsWith("java.lang.reflect")
	                            && !element.getClassName().startsWith("io.qt.internal.RetroHelper")) {
		                    if(number==0){
		                        try {
		                            return new io.qt.InternalAccess.CallerContext(Class.forName(element.getClassName()), element.getMethodName(), null, null, element.getLineNumber());
		                        } catch (ClassNotFoundException e) {
		                            ++number;
		                        }
		                    }
	                        --number;
	                    }
                	}
                }
                return null;
            };
            Supplier<Class<?>> _callerClassProvider;
			try {
				Class<?> reflectionClass = Class.forName("sun.reflect.Reflection");
				Method getCallerClass = reflectionClass.getMethod("getCallerClass", int.class);
				_callerClassProvider = ()->{
					try {
						return (Class<?>)getCallerClass.invoke(null, 4);
					} catch (RuntimeException | Error e) {
						throw e;
					} catch (InvocationTargetException e) {
						try {
							throw e.getTargetException();
						} catch (RuntimeException | Error e2) {
							throw e2;
						}catch(Throwable t) {
							throw new RuntimeException(t);
						}
					}catch(Throwable t) {
						throw new RuntimeException(t);
					}
				};
			} catch (Throwable e) {
				_callerClassProvider = ()->{
	                Class<?> result = Object.class;
	                io.qt.InternalAccess.CallerContext info = classAccessChecker().apply(2);
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
				try {
					Field jvm = runtimeMXBean.getClass().getDeclaredField("jvm");
					jvm.setAccessible(true);
					Object management = jvm.get(runtimeMXBean);
					Method method = management.getClass().getDeclaredMethod("getProcessId");
					method.setAccessible(true);
					pid = ""+method.invoke(management);
				} catch (Throwable e) {
					Method getName = runtimeMXBean.getClass().getMethod("getName");
					getName.setAccessible(true);
					pid = (String)getName.invoke(runtimeMXBean);
			        int index = pid.indexOf('@');
			        if(index>0)
			            pid = pid.substring(0, index);
				}
			} catch (Throwable e) {
				Logger.getLogger("io.qt.internal").throwing("Java8Implementor", "<init>", e);
			}
	        this.processName = pid;
		}

		private final IntFunction<io.qt.InternalAccess.CallerContext> classAccessChecker;
	    private final Supplier<Class<?>> callerClassProvider;
	    private final String processName;
	    private final Function<AnnotatedElement, AnnotatedElement> getAnnotatedOwnerTypeSupplier;

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
		
		@SuppressWarnings("unchecked")
		@Override
		public <T extends AnnotatedElement> T getAnnotatedOwnerType(T actualType) {
			if(getAnnotatedOwnerTypeSupplier!=null) {
	            try{
	            	if(actualType instanceof java.lang.reflect.AnnotatedType)
	            		return (T)getAnnotatedOwnerTypeSupplier.apply(actualType);
	            } catch (RuntimeException | Error e) {
	                throw e;
	            } catch (Throwable e) {
	                throw new RuntimeException(e);
	            }
	        }
	        return null;
		}

		@Override
		public boolean isProcessAlive(String pid) {
			switch (LibraryUtility.operatingSystem) {
			case Linux:
				return new File("/proc/"+pid+"/exe").exists();
			case MacOS:
				try {
					Process process = Runtime.getRuntime().exec(new String[]{"ps", "-p", pid});
					return process.waitFor()==0;
				} catch (Throwable e) {
				}
				break;
			case Windows:
				try {
					Process process = Runtime.getRuntime().exec(new String[]{"tasklist", "/FI", "PID eq "+pid});
					return process.waitFor()==0;
				} catch (Throwable e) {
				}
				break;
			default:
				break;
			}
			return true; //...because unknown
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
    
    static Supplier<CallerContext> callerContextProvider() {
    	return ()->implementor.classAccessChecker().apply(2);
    }
    
    static <T extends AnnotatedElement> T getAnnotatedOwnerType(T actualType) {
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
    
    static Package getDefinedPackage(ClassLoader cl, String pkg) {
        try {
			return cl.getDefinedPackage(pkg);
		} catch (Throwable e) {}
        try {
            return (Package)ReflectionUtility.invokeMethod(ClassLoader.class.getDeclaredMethod("getPackage", String.class), cl, pkg);
		} catch (Throwable e) {}
        try {
        	for(java.lang.Package _pkg : java.lang.Package.getPackages()) {
        		if(_pkg.getName().equals(pkg))
        			return _pkg;
        	}
		} catch (Throwable e) {}
        try {
			return cl.loadClass(pkg.replace("/", ".")+".package-info").getPackage();
		} catch (Throwable e) {}
		return null;
    }

	static boolean isProcessAlive(String pid) {
		return implementor.isProcessAlive(pid);
	}
}
