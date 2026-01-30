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

import java.lang.invoke.MethodHandle;
import java.lang.reflect.Constructor;
import java.lang.reflect.Field;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.lang.reflect.Modifier;
import java.util.Arrays;
import java.util.function.BiConsumer;
import java.util.function.BiFunction;
import java.util.function.Function;
import java.util.function.IntFunction;
import java.util.function.Supplier;

import io.qt.NativeAccess;
import io.qt.core.QDataStream;
import io.qt.core.QMetaObject;
import io.qt.core.QMetaObject.Method3;
import io.qt.core.QMetaObject.Method4;
import io.qt.core.QMetaObject.Method5;

/**
 * @hidden
 */
abstract class ReflectionUtility {
	private ReflectionUtility() {
		throw new RuntimeException();
	}

	static interface MethodInvocationHandler{
		BiConsumer<Object,QDataStream> streamIO(Method method);
		<A,B> Function<A,B> functionFromMethod(Method method);
		<T> Supplier<T> getFactory0(Constructor<T> constructor);
		<A,T> Function<A,T> getFactory1(Constructor<T> constructor);
		<A,B,T> BiFunction<A,B,T> getFactory2(Constructor<T> constructor);
		<A,B,C,T> QMetaObject.Method3<A,B,C,T> getFactory3(Constructor<T> constructor);
		<A,B,C,D,T> QMetaObject.Method4<A,B,C,D,T> getFactory4(Constructor<T> constructor);
		<A,B,C,D,E,T> QMetaObject.Method5<A,B,C,D,E,T> getFactory5(Constructor<T> constructor);
		<T> Function<Object[],T> getFactory(Constructor<T> constructor);
		<T> T invokeContructor(Constructor<T> constructor, Object... args) throws Throwable;
		Object invokeMethod(Method method, Object object, Object... args) throws Throwable;
		Object invokeInterfaceDefaultMethod(Method method, Object object, Object... args) throws Throwable;
		void setField(Object owner, Field f, Object newValue)throws Throwable;
		Object fetchField(Object owner, Field f)throws Throwable;
		
		public default SignalUtility.SlotInvoker getSlotInvoker(Method slot, MethodHandle _slotHandle){
        	try {
        		slot.setAccessible(true);
				return (instance, _arguments)->slot.invoke(instance, _arguments);
			} catch (Throwable e) {
			}
			return null;
		}
		
		public default SignalUtility.StaticSlotInvoker getStaticSlotInvoker(Method slot, MethodHandle _slotHandle){
        	try {
        		slot.setAccessible(true);
				return _arguments->slot.invoke(null, _arguments);
			} catch (Throwable e) {
			}
			return null;
		}
		
		public default SignalUtility.LambdaArgsSlotInvoker getSlotInvoker(Method slot, MethodHandle _slotHandle, int lambdaArgsCount){
			try {
				slot.setAccessible(true);
				return (instance, args, _arguments)->{
					int destPos = args.length;
					args = Arrays.copyOf(args, args.length + _arguments.length);
					System.arraycopy(_arguments, 0, args, destPos, _arguments.length);
					slot.invoke(instance, args);
				};
			} catch (Throwable e) {
			}
			return null;
		}
		
		public default SignalUtility.StaticLambdaArgsSlotInvoker getStaticSlotInvoker(Method slot, MethodHandle _slotHandle, int lambdaArgsCount){
			try {
				slot.setAccessible(true);
				return (args, _arguments)->{
					int destPos = args.length;
					args = Arrays.copyOf(args, args.length + _arguments.length);
					System.arraycopy(_arguments, 0, args, destPos, _arguments.length);
					slot.invoke(null, args);
				};
			} catch (Throwable e) {
			}
			return null;
		}
		
		<T,V> BiConsumer<T,V> getFieldSetter(Field f);
	}
	
	static final MethodInvocationHandler methodInvocationHandler;

	static {
		QtJambi_LibraryUtilities.initialize();
		MethodInvocationHandler _methodInvoker = null;
		try {
			_methodInvoker = new JavaMethodHandles();
		} catch (Throwable e) {
			_methodInvoker = new MethodInvocationHandler() {
				@Override
				public BiConsumer<Object, QDataStream> streamIO(Method method) {
					method.setAccessible(true);
					return (o,s)->{
						try {
							method.invoke(o,s);
						} catch (IllegalAccessException | InvocationTargetException e) {
							e.printStackTrace();
						}
					};
				}

				@SuppressWarnings("unchecked")
				@Override
				public <A, B> Function<A, B> functionFromMethod(Method method) {
					if(method==null)
						return null;
					method.setAccessible(true);
					return a->{
						try {
							return (B)method.invoke(a);
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
				}

				@Override
				public <T> Supplier<T> getFactory0(Constructor<T> constructor) {
					constructor.setAccessible(true);
					return ()->{
						try {
							return constructor.newInstance();
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
				}

				@Override
				public <A, T> Function<A, T> getFactory1(Constructor<T> constructor) {
					constructor.setAccessible(true);
					return (a)->{
						try {
							return constructor.newInstance(a);
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
				}

				@Override
				public <A, B, T> BiFunction<A, B, T> getFactory2(Constructor<T> constructor) {
					constructor.setAccessible(true);
					return (a,b)->{
						try {
							return constructor.newInstance(a,b);
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
				}

				@Override
				public <A, B, C, T> Method3<A, B, C, T> getFactory3(Constructor<T> constructor) {
					constructor.setAccessible(true);
					return (a,b,c)->{
						try {
							return constructor.newInstance(a,b,c);
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
				}

				@Override
				public <A, B, C, D, T> Method4<A, B, C, D, T> getFactory4(Constructor<T> constructor) {
					constructor.setAccessible(true);
					return (a,b,c,d)->{
						try {
							return constructor.newInstance(a,b,c,d);
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
				}

				@Override
				public <A, B, C, D, E, T> Method5<A, B, C, D, E, T> getFactory5(Constructor<T> constructor) {
					constructor.setAccessible(true);
					return (a,b,c,d,e)->{
						try {
							return constructor.newInstance(a,b,c,d,e);
						} catch (RuntimeException | Error e1) {
							throw e1;
						} catch (InvocationTargetException e1) {
							try {
								throw e1.getTargetException();
							} catch (RuntimeException | Error e2) {
								throw e2;
							}catch(Throwable t) {
								throw new RuntimeException(t);
							}
						}catch(Throwable t) {
							throw new RuntimeException(t);
						}
					};
				}

				@Override
				public <T> Function<Object[], T> getFactory(Constructor<T> constructor) {
					constructor.setAccessible(true);
					return (a)->{
						try {
							return constructor.newInstance(a);
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
				}

				@Override
				public <T> T invokeContructor(Constructor<T> constructor, Object... args) throws Throwable {
					try {
						constructor.setAccessible(true);
						return constructor.newInstance(args);
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
				}

				@Override
				public Object invokeMethod(Method method, Object object, Object... args) throws Throwable {
					try {
						method.setAccessible(true);
						if(Modifier.isStatic(method.getModifiers()))
							return method.invoke(null, args);
						else
							return method.invoke(object, args);
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
				}

				@Override
				public Object invokeInterfaceDefaultMethod(Method method, Object object, Object... args) throws Throwable {
					return invokeMethod(method, object, args);
				}

				@Override
				public void setField(Object owner, Field f, Object newValue) throws Throwable {
					try {
						f.setAccessible(true);
						if(Modifier.isStatic(f.getModifiers()))
							f.set(null, newValue);
						else
							f.set(owner, newValue);
					} catch (RuntimeException | Error e) {
						throw e;
					}catch(Throwable t) {
						throw new RuntimeException(t);
					}
				}

				@Override
				public Object fetchField(Object owner, Field f) throws Throwable {
					try {
						f.setAccessible(true);
						if(Modifier.isStatic(f.getModifiers()))
							return f.get(null);
						else
							return f.get(owner);
					} catch (RuntimeException | Error e) {
						throw e;
					}catch(Throwable t) {
						throw new RuntimeException(t);
					}
				}
				
				@Override
				public <T,V> BiConsumer<T,V> getFieldSetter(Field f) {
					try {
						f.setAccessible(true);
						return (T owner, V value) -> {
							try {
								f.set(owner, value);
							} catch (RuntimeException | Error e) {
								throw e;
							}catch(Throwable t) {
								throw new RuntimeException(t);
							}
						};
					} catch (RuntimeException | Error e) {
						throw e;
					}catch(Throwable t) {
						throw new RuntimeException(t);
					}
				}
			};
		}
		methodInvocationHandler = _methodInvoker;
	}

	static SignalUtility.SlotInvoker getSlotInvoker(Method slot, MethodHandle slotHandle){
		return methodInvocationHandler.getSlotInvoker(slot, slotHandle);
	}
	
	static SignalUtility.StaticSlotInvoker getStaticSlotInvoker(Method slot, MethodHandle slotHandle){
		return methodInvocationHandler.getStaticSlotInvoker(slot, slotHandle);
	}
	
	static SignalUtility.LambdaArgsSlotInvoker getSlotInvoker(Method slot, MethodHandle slotHandle, int lambdaArgsCount){
		return methodInvocationHandler.getSlotInvoker(slot, slotHandle, lambdaArgsCount);
	}
	
	static SignalUtility.StaticLambdaArgsSlotInvoker getStaticSlotInvoker(Method slot, MethodHandle slotHandle, int lambdaArgsCount){
		return methodInvocationHandler.getStaticSlotInvoker(slot, slotHandle, lambdaArgsCount);
	}
	
	static <A,B> Function<A,B> functionFromMethod(Method method){
		return methodInvocationHandler.functionFromMethod(method);
	}

	static MethodHandle getConstructorHandle(Constructor<?> constructor) throws IllegalAccessException {
		java.lang.invoke.MethodHandles.Lookup lookup = ReflectionUtility.privateLookup(constructor.getDeclaringClass());
		return lookup.unreflectConstructor(constructor);
	}

	static MethodHandle getMethodHandle(Method method) throws IllegalAccessException {
		java.lang.invoke.MethodHandles.Lookup lookup = ReflectionUtility.privateLookup(method.getDeclaringClass());
		return lookup.unreflect(method);
	}

	static MethodHandle getFieldGetterHandle(Field field) throws IllegalAccessException {
		java.lang.invoke.MethodHandles.Lookup lookup = ReflectionUtility.privateLookup(field.getDeclaringClass());
		return lookup.unreflectGetter(field);
	}
	
	static <T> Function<Object[],T> getFactory(Constructor<T> constructor) {
		return methodInvocationHandler.getFactory(constructor);
	}
	
	static <T> Supplier<T> getFactory0(Constructor<T> constructor) {
		return methodInvocationHandler.getFactory0(constructor);
	}
	
	static <A,T> Function<A,T> getFactory1(Constructor<T> constructor) {
		return methodInvocationHandler.getFactory1(constructor);
	}
	
	static <A,B,T> BiFunction<A,B,T> getFactory2(Constructor<T> constructor) {
		return methodInvocationHandler.getFactory2(constructor);
	}
	
	static <A,B,C,T> QMetaObject.Method3<A,B,C,T> getFactory3(Constructor<T> constructor) {
		return methodInvocationHandler.getFactory3(constructor);
	}
	
	static <A,B,C,D,T> QMetaObject.Method4<A,B,C,D,T> getFactory4(Constructor<T> constructor) {
		return methodInvocationHandler.getFactory4(constructor);
	}
	
	static <A,B,C,D,E,T> QMetaObject.Method5<A,B,C,D,E,T> getFactory5(Constructor<T> constructor) {
		return methodInvocationHandler.getFactory5(constructor);
	}

	static <T> T invokeContructor(Constructor<T> constructor, Object... args) throws Throwable {
		return methodInvocationHandler.invokeContructor(constructor, args);
	}

	static Object invokeMethod(Method method, Object object, Object... args) throws Throwable {
		return methodInvocationHandler.invokeMethod(method, object, args);
	}
	
	static void writeField(Object owner, Field f, Object newValue) {
		try {
			methodInvocationHandler.setField(owner, f, newValue);
		} catch (Throwable e) {
			if (Modifier.isStatic(f.getModifiers())) {
				if (!writeField(f.getDeclaringClass(), f, true, newValue)) {
					throw new RuntimeException("Cannot set field '" + f.getName() + "'", e);
				}
			}else if(owner!=null){
				if (!writeField(owner, f, false, newValue)) {
					throw new RuntimeException("Cannot set field '" + f.getName() + "'", e);
				}
			}else {
				try {
					throw e;
				} catch (RuntimeException e1) {
					throw e1;
				} catch (Throwable e1) {
					throw new RuntimeException("Cannot set field '" + f.getName() + "'", e1);
				}
			}
		}
	}
	
	static <T,V> BiConsumer<T,V> getFieldSetter(Field f){
		return methodInvocationHandler.getFieldSetter(f);
	}

	private static native boolean writeField(Object owner, Field field, boolean isStatic, Object newValue);
	
	@SuppressWarnings("unchecked")
	static <V> V readField(Object owner, Class<?> declaringClass, String fieldName, Class<V> fieldType) {
		if(declaringClass==null)
			declaringClass = AccessUtility.instance.getClass(owner);
		try {
			Field field = null;
			while(declaringClass!=null) {
				try {
					field = declaringClass.getDeclaredField(fieldName);
				} catch (Exception e) {
				}
				if(field==null)
					declaringClass = declaringClass.getSuperclass();
				else
					break;
			}
			if(field!=null)
				return (V) readField(owner,field);
		} catch (Throwable e) {
		}
		throw new RuntimeException("Cannot fetch field '" + fieldName + "'");
	}

	static Object readField(Object owner, Field f) {
		try {
			return methodInvocationHandler.fetchField(owner, f);
		} catch (Throwable e) {
			if (Modifier.isStatic(f.getModifiers())) {
				return readField(f.getDeclaringClass(), f, true);
			}else {
				return readField(owner, f, false);
			}
		}
	}

	static void writeField(Object owner, Class<?> declaringClass, String fieldName, Object newValue) {
		try {
			Field f = declaringClass.getDeclaredField(fieldName);
			writeField(owner, f, newValue);
		} catch (RuntimeException e) {
			throw e;
		} catch (Throwable e) {
			throw new RuntimeException("Cannot set field '" + fieldName + "'", e);
		}
	}
	
	private static native Object readField(Object owner, Field field, boolean isStatic);

	static native java.lang.invoke.MethodHandles.Lookup privateLookup(Class<?> targetClass);
	
	@NativeAccess
	private static IntFunction<io.qt.InternalAccess.CallerContext> invocationInfoProvider(){
		return RetroHelper.classAccessChecker();
	}
}
