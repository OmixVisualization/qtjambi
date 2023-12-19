package io.qt.internal;

import java.lang.invoke.MethodHandle;
import java.lang.invoke.MethodHandleProxies;
import java.lang.invoke.MethodHandles.Lookup;
import java.lang.reflect.Constructor;
import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.lang.reflect.Modifier;
import java.util.function.BiConsumer;
import java.util.function.BiFunction;
import java.util.function.Function;
import java.util.function.Supplier;

import io.qt.core.QDataStream;
import io.qt.core.QMetaObject;

/**
 * @hidden
 */
final class JavaMethodHandles implements ReflectionUtility.MethodInvocationHandler{
	public final boolean useMethodHandleProxies;

	JavaMethodHandles() {
		boolean _useMethodHandleProxies = false;
		try {
			java.lang.invoke.MethodHandleProxies.class.hashCode();
			_useMethodHandleProxies = true;
		}catch(Throwable t) {}
		useMethodHandleProxies = _useMethodHandleProxies;
	}

	@Override
	@SuppressWarnings("unchecked")
	public <A,B> Function<A,B> functionFromMethod(Method method){
		if(method==null)
			return null;
		try {
			Lookup lookup = ReflectionUtility.privateLookup(method.getDeclaringClass());
			MethodHandle handle = lookup.unreflect(method);
			if(useMethodHandleProxies) {
				try {
					return MethodHandleProxies.asInterfaceInstance(Function.class, handle);
				} catch (Throwable e) {
				}
			}
			return a->{
				try {
					return (B)handle.invoke(a);
				} catch (RuntimeException | Error e) {
					throw e;
				}catch(Throwable t) {
					throw new RuntimeException(t);
				}
			};
		} catch (IllegalAccessException e) {
			return null;
		}
	}
	
	@Override
	@SuppressWarnings("unchecked")
	public BiConsumer<Object,QDataStream> streamIO(Method method){
		try {
			Lookup lookup = ReflectionUtility.privateLookup(method.getDeclaringClass());
			MethodHandle handle = lookup.unreflect(method);
			if(useMethodHandleProxies) {
				try {
					return MethodHandleProxies.asInterfaceInstance(BiConsumer.class, handle);
				} catch (Throwable e) {
				}
			}
			return (o,s)->{
				try {
					handle.invoke(o, s);
				} catch (RuntimeException | Error e) {
					throw e;
				}catch(Throwable t) {
					throw new RuntimeException(t);
				}
			};
		} catch (IllegalAccessException e) {
			return null;
		}
	}
	
	@Override
	@SuppressWarnings("unchecked")
	public <T> Function<Object[],T> getFactory(Constructor<T> constructor) {
		if(constructor==null)
			return null;
		try {
			java.lang.invoke.MethodHandles.Lookup lookup = ReflectionUtility.privateLookup(constructor.getDeclaringClass());
			MethodHandle handle = lookup.unreflectConstructor(constructor);
			if(useMethodHandleProxies) {
				try {
					MethodHandle spreader = handle.asSpreader(Object[].class, constructor.getParameterCount());
					return MethodHandleProxies.asInterfaceInstance(Function.class, spreader);
				} catch (Throwable e) {
				}
			}
			return args->{
				try {
					if(args!=null) {
						return (T)handle.invokeWithArguments(args);
					}else {
						return (T)handle.invoke();
					}
				}catch(Throwable t) {
					throw new RuntimeException(t);
				}
			};
		} catch (RuntimeException | Error e) {
			throw e;
		} catch (Throwable e) {
			throw new RuntimeException(e);
		}
	}
	
	@Override
	@SuppressWarnings("unchecked")
	public <T> Supplier<T> getFactory0(Constructor<T> constructor) {
		if(constructor==null)
			return null;
		if(constructor.getParameterCount()!=0)
			throw new RuntimeException("Parameter count mismatch");
		try {
			java.lang.invoke.MethodHandles.Lookup lookup = ReflectionUtility.privateLookup(constructor.getDeclaringClass());
			MethodHandle handle = lookup.unreflectConstructor(constructor);
			if(useMethodHandleProxies) {
				try {
					return MethodHandleProxies.asInterfaceInstance(Supplier.class, handle);
				} catch (Throwable e) {
				}
			}
			return ()->{
				try {
					return (T)handle.invoke();
				}catch(Throwable t) {
					throw new RuntimeException(t);
				}
			};
		} catch (RuntimeException | Error e) {
			throw e;
		} catch (Throwable e) {
			throw new RuntimeException(e);
		}
	}
	
	@Override
	@SuppressWarnings("unchecked")
	public <A,T> Function<A,T> getFactory1(Constructor<T> constructor) {
		if(constructor==null)
			return null;
		if(constructor.getParameterCount()!=1)
			throw new RuntimeException("Parameter count mismatch");
		try {
			java.lang.invoke.MethodHandles.Lookup lookup = ReflectionUtility.privateLookup(constructor.getDeclaringClass());
			MethodHandle handle = lookup.unreflectConstructor(constructor);
			if(useMethodHandleProxies) {
				try {
					return MethodHandleProxies.asInterfaceInstance(Function.class, handle);
				} catch (Throwable e) {
				}
			}
			return a->{
				try {
					return (T)handle.invoke(a);
				}catch(Throwable t) {
					throw new RuntimeException(t);
				}
			};
		} catch (RuntimeException | Error e) {
			throw e;
		} catch (Throwable e) {
			throw new RuntimeException(e);
		}
	}
	
	@Override
	@SuppressWarnings("unchecked")
	public <A,B,T> BiFunction<A,B,T> getFactory2(Constructor<T> constructor) {
		if(constructor==null)
			return null;
		if(constructor.getParameterCount()!=2)
			throw new RuntimeException("Parameter count mismatch");
		try {
			java.lang.invoke.MethodHandles.Lookup lookup = ReflectionUtility.privateLookup(constructor.getDeclaringClass());
			MethodHandle handle = lookup.unreflectConstructor(constructor);
			if(useMethodHandleProxies) {
				try {
					return MethodHandleProxies.asInterfaceInstance(BiFunction.class, handle);
				} catch (Throwable e) {
				}
			}
			return (a,b)->{
				try {
					return (T)handle.invoke(a,b);
				}catch(Throwable t) {
					throw new RuntimeException(t);
				}
			};
		} catch (RuntimeException | Error e) {
			throw e;
		} catch (Throwable e) {
			throw new RuntimeException(e);
		}
	}
	
	@Override
	@SuppressWarnings("unchecked")
	public <A,B,C,T> QMetaObject.Method3<A,B,C,T> getFactory3(Constructor<T> constructor) {
		if(constructor==null)
			return null;
		if(constructor.getParameterCount()!=3)
			throw new RuntimeException("Parameter count mismatch");
		try {
			java.lang.invoke.MethodHandles.Lookup lookup = ReflectionUtility.privateLookup(constructor.getDeclaringClass());
			MethodHandle handle = lookup.unreflectConstructor(constructor);
			if(useMethodHandleProxies) {
				try {
					return MethodHandleProxies.asInterfaceInstance(QMetaObject.Method3.class, handle);
				} catch (Throwable e) {
				}
			}
			return (a,b,c)->{
				try {
					return (T)handle.invoke(a,b,c);
				}catch(Throwable t) {
					throw new RuntimeException(t);
				}
			};
		} catch (RuntimeException | Error e) {
			throw e;
		} catch (Throwable e) {
			throw new RuntimeException(e);
		}
	}
	
	@Override
	@SuppressWarnings("unchecked")
	public <A,B,C,D,T> QMetaObject.Method4<A,B,C,D,T> getFactory4(Constructor<T> constructor) {
		if(constructor==null)
			return null;
		if(constructor.getParameterCount()!=4)
			throw new RuntimeException("Parameter count mismatch");
		try {
			java.lang.invoke.MethodHandles.Lookup lookup = ReflectionUtility.privateLookup(constructor.getDeclaringClass());
			MethodHandle handle = lookup.unreflectConstructor(constructor);
			if(useMethodHandleProxies) {
				try {
					return MethodHandleProxies.asInterfaceInstance(QMetaObject.Method4.class, handle);
				} catch (Throwable e) {
				}
			}
			return (a,b,c,d)->{
				try {
					return (T)handle.invoke(a,b,c,d);
				}catch(Throwable t) {
					throw new RuntimeException(t);
				}
			};
		} catch (RuntimeException | Error e) {
			throw e;
		} catch (Throwable e) {
			throw new RuntimeException(e);
		}
	}
	
	@Override
	@SuppressWarnings("unchecked")
	public <A,B,C,D,E,T> QMetaObject.Method5<A,B,C,D,E,T> getFactory5(Constructor<T> constructor) {
		if(constructor==null)
			return null;
		if(constructor.getParameterCount()!=5)
			throw new RuntimeException("Parameter count mismatch");
		try {
			java.lang.invoke.MethodHandles.Lookup lookup = ReflectionUtility.privateLookup(constructor.getDeclaringClass());
			MethodHandle handle = lookup.unreflectConstructor(constructor);
			if(useMethodHandleProxies) {
				try {
					return MethodHandleProxies.asInterfaceInstance(QMetaObject.Method5.class, handle);
				} catch (Throwable e) {
				}
			}
			return (a,b,c,d,e)->{
				try {
					return (T)handle.invoke(a,b,c,d,e);
				}catch(Throwable t) {
					throw new RuntimeException(t);
				}
			};
		} catch (RuntimeException | Error e) {
			throw e;
		} catch (Throwable e) {
			throw new RuntimeException(e);
		}
	}
	
	@Override
	@SuppressWarnings("unchecked")
	public <T> T invokeContructor(Constructor<T> constructor, Object... args) throws Throwable {
		java.lang.invoke.MethodHandles.Lookup lookup = ReflectionUtility.privateLookup(constructor.getDeclaringClass());
		MethodHandle handle = lookup.unreflectConstructor(constructor);
		if(args!=null && args.length>0) {
			return (T)handle.invokeWithArguments(args);
		}else {
			return (T)handle.invoke();
		}
	}

	@Override
	public Object invokeMethod(Method method, Object object, Object... args) throws Throwable {
		java.lang.invoke.MethodHandles.Lookup lookup = ReflectionUtility.privateLookup(method.getDeclaringClass());
		MethodHandle handle = lookup.unreflect(method);
		if(Modifier.isStatic(method.getModifiers())) {
			if(args!=null && args.length>0) {
				return handle.invokeWithArguments(args);
			}else {
				return handle.invoke();
			}
		}else {
			if(args!=null && args.length>0) {
				Object[] _arguments = new Object[args.length + 1];
				_arguments[0] = object;
				System.arraycopy(args, 0, _arguments, 1, args.length);
				return handle.invokeWithArguments(_arguments);
			}else {
				return handle.invokeWithArguments(new Object[] {object});
			}
		}
	}
	
	@Override
	public Object invokeInterfaceDefaultMethod(Method method, Object object, Object... args) throws Throwable {
		java.lang.invoke.MethodHandles.Lookup lookup = ReflectionUtility.privateLookup(method.getDeclaringClass());
		MethodHandle handle = lookup.unreflectSpecial(method, method.getDeclaringClass());
		if(args!=null) {
			Object[] _arguments = new Object[args.length + 1];
			_arguments[0] = object;
			System.arraycopy(args, 0, _arguments, 1, args.length);
			return handle.invokeWithArguments(_arguments);
		}else {
			return handle.invokeWithArguments(new Object[] {object});
		}
	}
	
	@Override
	public void setField(Object owner, Field f, Object newValue) throws Throwable {
		java.lang.invoke.MethodHandles.Lookup lookup = ReflectionUtility.privateLookup(f.getDeclaringClass());
		if (Modifier.isStatic(f.getModifiers())) {
			MethodHandle setter = lookup.unreflectSetter(f);
			setter.invoke(newValue);
		}else {
			if(owner==null)
				throw new NullPointerException();
			MethodHandle setter = lookup.unreflectSetter(f);
			setter.invoke(owner, newValue);
		}
	}
	
	@Override
	public Object fetchField(Object owner, Field f) throws Throwable {
		java.lang.invoke.MethodHandles.Lookup lookup = ReflectionUtility.privateLookup(f.getDeclaringClass());
		if (Modifier.isStatic(f.getModifiers())) {
			MethodHandle getter = lookup.unreflectGetter(f);
			return getter.invoke();
		}else {
			if(owner==null)
				throw new NullPointerException();
			MethodHandle getter = lookup.unreflectGetter(f);
			return getter.invoke(owner);
		}
	}
	
	@Override
	public SignalUtility.SlotInvoker getSlotInvoker(Method slot, MethodHandle _slotHandle){
		if(_slotHandle==null) {
        	try {
				_slotHandle = ReflectionUtility.getMethodHandle(slot);
			} catch (Exception e) {
			}
        }
		MethodHandle slotHandle = _slotHandle;
		if(slotHandle!=null) {
			return _arguments->{
					switch(_arguments.length){
					case 0:
						slotHandle.invoke();
						break;
					case 1:
						slotHandle.invoke(_arguments[0]);
						break;
					case 2:
						slotHandle.invoke(_arguments[0], _arguments[1]);
						break;
					case 3:
						slotHandle.invoke(
								_arguments[0], _arguments[1], _arguments[2]
							);
						break;
					case 4:
						slotHandle.invoke(
								_arguments[0], _arguments[1], _arguments[2],
								_arguments[3]
							);
						break;
					case 5:
						slotHandle.invoke(
								_arguments[0], _arguments[1], _arguments[2],
								_arguments[3], _arguments[4]
							);
						break;
					case 6:
						slotHandle.invoke(
								_arguments[0], _arguments[1], _arguments[2],
								_arguments[3], _arguments[4], _arguments[5]
							);
						break;
					case 7:
						slotHandle.invoke(
								_arguments[0], _arguments[1], _arguments[2],
								_arguments[3], _arguments[4], _arguments[5],
								_arguments[6]
							);
						break;
					case 8:
						slotHandle.invoke(
								_arguments[0], _arguments[1], _arguments[2],
								_arguments[3], _arguments[4], _arguments[5],
								_arguments[6], _arguments[7]
							);
						break;
					case 9:
						slotHandle.invoke(
								_arguments[0], _arguments[1], _arguments[2],
								_arguments[3], _arguments[4], _arguments[5],
								_arguments[6], _arguments[7], _arguments[8]
							);
						break;
					case 10:
						slotHandle.invoke(
								_arguments[0], _arguments[1], _arguments[2],
								_arguments[3], _arguments[4], _arguments[5],
								_arguments[6], _arguments[7], _arguments[8],
								_arguments[9]
							);
						break;
					default:
						slotHandle.asSpreader(Object[].class, _arguments.length).invoke(_arguments);
						break;
				}
			};
		}
		return null;
	}

	@SuppressWarnings("unchecked")
	@Override
	public <T,V> BiConsumer<T,V> getFieldSetter(Field f) {
		java.lang.invoke.MethodHandles.Lookup lookup = ReflectionUtility.privateLookup(f.getDeclaringClass());
		try {
			MethodHandle setter = lookup.unreflectSetter(f);
			if(useMethodHandleProxies) {
				try {
					return (BiConsumer<T,V>)MethodHandleProxies.asInterfaceInstance(BiConsumer.class, setter);
				} catch (Throwable e) {
				}
			}
			return (T owner, V value) -> {
				if(owner==null)
					throw new NullPointerException();
				try{
					setter.invoke(owner, value);
				} catch (RuntimeException | Error e) {
					throw e;
				} catch (Throwable e) {
					throw new RuntimeException(e);
				}
			};
		} catch (RuntimeException | Error e) {
			throw e;
		} catch (Throwable e) {
			throw new RuntimeException(e);
		}
	}
}
