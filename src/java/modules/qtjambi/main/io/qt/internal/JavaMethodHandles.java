package io.qt.internal;

import java.lang.invoke.CallSite;
import java.lang.invoke.LambdaConversionException;
import java.lang.invoke.MethodHandle;
import java.lang.invoke.MethodHandles;
import java.lang.invoke.MethodHandles.Lookup;
import java.lang.invoke.MethodType;
import java.lang.reflect.Constructor;
import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.lang.reflect.Modifier;
import java.util.ArrayList;
import java.util.List;
import java.util.function.BiConsumer;
import java.util.function.BiFunction;
import java.util.function.Consumer;
import java.util.function.Function;
import java.util.function.Supplier;

import io.qt.QtUtilities;
import io.qt.core.QDataStream;
import io.qt.core.QMetaObject;
import io.qt.internal.LibraryUtility.OperatingSystem;

/**
 * @hidden
 */
final class JavaMethodHandles implements ReflectionUtility.MethodInvocationHandler{
	interface ProxyFactory{
		public <T> T asInterfaceInstance(final Class<T> intfc, final MethodHandle target);
	}
	public final ProxyFactory resolvedProxyFactory;
	interface MetaFactory{
		public CallSite metafactory(MethodHandles.Lookup caller,
                String interfaceMethodName,
                MethodType factoryType,
                MethodType interfaceMethodType,
                MethodHandle implementation,
                MethodType dynamicMethodType) throws LambdaConversionException;
	}
	public final MetaFactory resolvedMetaFactory;
	interface AccessClass{
		public Class<?> accessClass(MethodHandles.Lookup lookup, Class<?> targetClass) throws IllegalAccessException;
	}
	public AccessClass lookupAccessClass;

	JavaMethodHandles() {
		ProxyFactory _resolvedProxyFactory = null;
//		#### MethodHandleProxies are slower than lambdas ####
//		try {
//			_resolvedProxyFactory = java.lang.invoke.MethodHandleProxies::asInterfaceInstance;
//		}catch(Throwable t) {}
		resolvedProxyFactory = _resolvedProxyFactory;
		MetaFactory _resolvedMetaFactory = null;
		try {
			if(LibraryUtility.operatingSystem==OperatingSystem.Android) {
				java.lang.invoke.LambdaMetafactory.class.getMethod("metafactory",   MethodHandles.Lookup.class,
																                    String.class,
																                    MethodType.class,
																                    MethodType.class,
																                    MethodHandle.class,
																                    MethodType.class);
			}
			_resolvedMetaFactory = java.lang.invoke.LambdaMetafactory::metafactory;
		}catch(Throwable t) {}
		resolvedMetaFactory = _resolvedMetaFactory;
		AccessClass _lookupAccessClass = null;
		try {
			if(LibraryUtility.operatingSystem==OperatingSystem.Android) {
				MethodHandles.Lookup.class.getMethod("accessClass", Class.class);
			}
			_lookupAccessClass = MethodHandles.Lookup::accessClass;
		}catch(Throwable e) {
			_lookupAccessClass = (_lookup,_cls)->_cls;
		}
		lookupAccessClass = _lookupAccessClass;
	}
	
	static MethodType wrap(MethodType mt) {
		mt = mt.wrap().changeReturnType(void.class);
		return mt;
	}

	@Override
	@SuppressWarnings("unchecked")
	public <A,B> Function<A,B> functionFromMethod(Method method){
		if(method==null)
			return null;
		try {
			Lookup lookup = ReflectionUtility.privateLookup(method.getDeclaringClass());
			MethodHandle handle = lookup.unreflect(method);
			if(resolvedMetaFactory!=null) {
				try {
					CallSite site = resolvedMetaFactory.metafactory(ReflectionUtility.privateLookup(method.getDeclaringClass()),
							"apply",
			                MethodType.methodType(Function.class),
			                MethodType.methodType(Object.class, Object.class),
			                handle,
			                wrap(handle.type()));
					return (Function<A,B>)site.getTarget().invokeExact();
				} catch (Throwable e) {
				}
			}
			if(resolvedProxyFactory!=null) {
				try {
					return resolvedProxyFactory.asInterfaceInstance(Function.class, handle);
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
			if(resolvedProxyFactory!=null) {
				try {
					return resolvedProxyFactory.asInterfaceInstance(BiConsumer.class, handle);
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
			if(resolvedProxyFactory!=null) {
				try {
					MethodHandle spreader = handle.asSpreader(Object[].class, constructor.getParameterCount());
					return resolvedProxyFactory.asInterfaceInstance(Function.class, spreader);
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
				} catch (RuntimeException | Error e) {
					throw e;
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
			if(resolvedMetaFactory!=null) {
				try {
					CallSite site = resolvedMetaFactory.metafactory(ReflectionUtility.privateLookup(constructor.getDeclaringClass()),
							"get",
			                MethodType.methodType(Supplier.class),
			                MethodType.methodType(Object.class),
			                handle,
			                wrap(handle.type()));
					return (Supplier<T>)site.getTarget().invokeExact();
				} catch (Throwable e) {
				}
			}
			if(resolvedProxyFactory!=null) {
				try {
					return resolvedProxyFactory.asInterfaceInstance(Supplier.class, handle);
				} catch (Throwable e) {
				}
			}
			return ()->{
				try {
					return (T)handle.invoke();
				} catch (RuntimeException | Error e) {
					throw e;
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
			if(resolvedMetaFactory!=null) {
				try {
					CallSite site = resolvedMetaFactory.metafactory(ReflectionUtility.privateLookup(constructor.getDeclaringClass()),
							"apply",
			                MethodType.methodType(Function.class),
			                MethodType.methodType(Object.class, Object.class),
			                handle,
			                wrap(handle.type()));
					return (Function<A,T>)site.getTarget().invokeExact();
				} catch (Throwable e) {
				}
			}
			if(resolvedProxyFactory!=null) {
				try {
					return resolvedProxyFactory.asInterfaceInstance(Function.class, handle);
				} catch (Throwable e) {
				}
			}
			return a->{
				try {
					return (T)handle.invoke(a);
				} catch (RuntimeException | Error e) {
					throw e;
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
			if(resolvedMetaFactory!=null) {
				try {
					CallSite site = resolvedMetaFactory.metafactory(ReflectionUtility.privateLookup(constructor.getDeclaringClass()),
							"apply",
			                MethodType.methodType(BiFunction.class),
			                MethodType.methodType(Object.class, Object.class, Object.class),
			                handle,
			                wrap(handle.type()));
					return (BiFunction<A,B,T>)site.getTarget().invokeExact();
				} catch (Throwable e) {
				}
			}
			if(resolvedProxyFactory!=null) {
				try {
					return resolvedProxyFactory.asInterfaceInstance(BiFunction.class, handle);
				} catch (Throwable e) {
				}
			}
			return (a,b)->{
				try {
					return (T)handle.invoke(a,b);
				} catch (RuntimeException | Error e) {
					throw e;
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
			if(resolvedMetaFactory!=null) {
				try {
					CallSite site = resolvedMetaFactory.metafactory(ReflectionUtility.privateLookup(constructor.getDeclaringClass()),
							"invoke",
			                MethodType.methodType(QMetaObject.Method3.class),
			                MethodType.methodType(Object.class, Object.class, Object.class, Object.class),
			                handle,
			                wrap(handle.type()));
					return (QMetaObject.Method3<A,B,C,T>)site.getTarget().invokeExact();
				} catch (Throwable e) {
				}
			}
			if(resolvedProxyFactory!=null) {
				try {
					return resolvedProxyFactory.asInterfaceInstance(QMetaObject.Method3.class, handle);
				} catch (Throwable e) {
				}
			}
			return (a,b,c)->{
				try {
					return (T)handle.invoke(a,b,c);
				} catch (RuntimeException | Error e) {
					throw e;
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
			if(resolvedMetaFactory!=null) {
				try {
					CallSite site = resolvedMetaFactory.metafactory(ReflectionUtility.privateLookup(constructor.getDeclaringClass()),
							"invoke",
			                MethodType.methodType(QMetaObject.Method4.class),
			                MethodType.methodType(Object.class, Object.class, Object.class, Object.class, Object.class),
			                handle,
			                wrap(handle.type()));
					return (QMetaObject.Method4<A,B,C,D,T>)site.getTarget().invokeExact();
				} catch (Throwable e) {
				}
			}
			if(resolvedProxyFactory!=null) {
				try {
					return resolvedProxyFactory.asInterfaceInstance(QMetaObject.Method4.class, handle);
				} catch (Throwable e) {
				}
			}
			return (a,b,c,d)->{
				try {
					return (T)handle.invoke(a,b,c,d);
				} catch (RuntimeException | Error e) {
					throw e;
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
			if(resolvedMetaFactory!=null) {
				try {
					CallSite site = resolvedMetaFactory.metafactory(ReflectionUtility.privateLookup(constructor.getDeclaringClass()),
							"invoke",
			                MethodType.methodType(QMetaObject.Method5.class),
			                MethodType.methodType(Object.class, Object.class, Object.class, Object.class, Object.class, Object.class),
			                handle,
			                wrap(handle.type()));
					return (QMetaObject.Method5<A,B,C,D,E,T>)site.getTarget().invokeExact();
				} catch (Throwable e) {
				}
			}
			if(resolvedProxyFactory!=null) {
				try {
					return resolvedProxyFactory.asInterfaceInstance(QMetaObject.Method5.class, handle);
				} catch (Throwable e) {
				}
			}
			return (a,b,c,d,e)->{
				try {
					return (T)handle.invoke(a,b,c,d,e);
				} catch (RuntimeException | Error t) {
					throw t;
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
		if(resolvedMetaFactory!=null) {
			do try {
				CallSite site;
				List<Class<?>> parameters = new ArrayList<>();
				for(int i=0; i<_slotHandle.type().parameterCount(); ++i) {
					parameters.add(Object.class);
				}
				MethodType invokeType = MethodType.methodType(void.class, parameters);
				switch(_slotHandle.type().parameterCount()) {
				case 1:
					site = resolvedMetaFactory.metafactory(ReflectionUtility.privateLookup(slot.getDeclaringClass()),
							"accept",
			                MethodType.methodType(Consumer.class),
			                invokeType,
			                _slotHandle,
			                wrap(_slotHandle.type()));
					Consumer<Object> slot1 = (Consumer<Object>)site.getTarget().invokeExact();
					return (instance, _arguments)->slot1.accept(instance);
				case 2:
					site = resolvedMetaFactory.metafactory(ReflectionUtility.privateLookup(slot.getDeclaringClass()),
							"accept",
			                MethodType.methodType(BiConsumer.class),
			                invokeType,
			                _slotHandle,
			                wrap(_slotHandle.type()));
					BiConsumer<Object,Object> slot2 = (BiConsumer<Object,Object>)site.getTarget().invokeExact();
					return (instance, _arguments)->slot2.accept(instance, _arguments[0]);
				}
				MethodHandles.Lookup lookup;
				lookup = ReflectionUtility.privateLookup(slot.getDeclaringClass());
				lookupAccessClass.accessClass(lookup, QtUtilities.class);
				switch(_slotHandle.type().parameterCount()) {
				case 3:
					site = resolvedMetaFactory.metafactory(lookup,
							"accept",
			                MethodType.methodType(QtUtilities.Consumer3.class),
			                invokeType,
			                _slotHandle,
			                wrap(_slotHandle.type()));
					QtUtilities.Consumer3<Object,Object,Object> slot3 = (QtUtilities.Consumer3<Object,Object,Object>)site.getTarget().invokeExact();
					return (instance, _arguments)->slot3.accept(instance, _arguments[0], _arguments[1]);
				case 4:
					site = resolvedMetaFactory.metafactory(lookup,
							"accept",
			                MethodType.methodType(QtUtilities.Consumer4.class),
			                invokeType,
			                _slotHandle,
			                wrap(_slotHandle.type()));
					QtUtilities.Consumer4<Object,Object,Object,Object> slot4 = (QtUtilities.Consumer4<Object,Object,Object,Object>)site.getTarget().invokeExact();
					return (instance, _arguments)->slot4.accept(instance, _arguments[0], _arguments[1], _arguments[2]);
				case 5:
					site = resolvedMetaFactory.metafactory(lookup,
							"accept",
			                MethodType.methodType(QtUtilities.Consumer5.class),
			                invokeType,
			                _slotHandle,
			                wrap(_slotHandle.type()));
					QtUtilities.Consumer5<Object,Object,Object,Object,Object> slot5 = (QtUtilities.Consumer5<Object,Object,Object,Object,Object>)site.getTarget().invokeExact();
					return (instance, _arguments)->slot5.accept(instance, _arguments[0], _arguments[1], _arguments[2], _arguments[3]);
				case 6:
					site = resolvedMetaFactory.metafactory(lookup,
							"accept",
			                MethodType.methodType(QtUtilities.Consumer6.class),
			                invokeType,
			                _slotHandle,
			                wrap(_slotHandle.type()));
					QtUtilities.Consumer6<Object,Object,Object,Object,Object,Object> slot6 = (QtUtilities.Consumer6<Object,Object,Object,Object,Object,Object>)site.getTarget().invokeExact();
					return (instance, _arguments)->slot6.accept(instance, _arguments[0], _arguments[1], _arguments[2], _arguments[3], _arguments[4]);
				case 7:
					site = resolvedMetaFactory.metafactory(lookup,
							"accept",
			                MethodType.methodType(QtUtilities.Consumer7.class),
			                invokeType,
			                _slotHandle,
			                wrap(_slotHandle.type()));
					QtUtilities.Consumer7<Object,Object,Object,Object,Object,Object,Object> slot7 = (QtUtilities.Consumer7<Object,Object,Object,Object,Object,Object,Object>)site.getTarget().invokeExact();
					return (instance, _arguments)->slot7.accept(instance, _arguments[0], _arguments[1], _arguments[2], _arguments[3], _arguments[4], _arguments[5]);
				case 8:
					site = resolvedMetaFactory.metafactory(lookup,
							"accept",
			                MethodType.methodType(QtUtilities.Consumer8.class),
			                invokeType,
			                _slotHandle,
			                wrap(_slotHandle.type()));
					QtUtilities.Consumer8<Object,Object,Object,Object,Object,Object,Object,Object> slot8 = (QtUtilities.Consumer8<Object,Object,Object,Object,Object,Object,Object,Object>)site.getTarget().invokeExact();
					return (instance, _arguments)->slot8.accept(instance, _arguments[0], _arguments[1], _arguments[2], _arguments[3], _arguments[4], _arguments[5], _arguments[6]);
				case 9:
					site = resolvedMetaFactory.metafactory(lookup,
							"accept",
			                MethodType.methodType(QtUtilities.Consumer9.class),
			                invokeType,
			                _slotHandle,
			                wrap(_slotHandle.type()));
					QtUtilities.Consumer9<Object,Object,Object,Object,Object,Object,Object,Object,Object> slot9 = (QtUtilities.Consumer9<Object,Object,Object,Object,Object,Object,Object,Object,Object>)site.getTarget().invokeExact();
					return (instance, _arguments)->slot9.accept(instance, _arguments[0], _arguments[1], _arguments[2], _arguments[3], _arguments[4], _arguments[5], _arguments[6], _arguments[7]);
				case 10:
					site = resolvedMetaFactory.metafactory(lookup,
							"accept",
			                MethodType.methodType(QtUtilities.Consumer10.class),
			                invokeType,
			                _slotHandle,
			                wrap(_slotHandle.type()));
					QtUtilities.Consumer10<Object,Object,Object,Object,Object,Object,Object,Object,Object,Object> slot10 = (QtUtilities.Consumer10<Object,Object,Object,Object,Object,Object,Object,Object,Object,Object>)site.getTarget().invokeExact();
					return (instance, _arguments)->slot10.accept(instance, _arguments[0], _arguments[1], _arguments[2], _arguments[3], _arguments[4], _arguments[5], _arguments[6], _arguments[7], _arguments[8]);
				}
			} catch (IllegalAccessException e) {
			} catch (Throwable e) {
				if(Boolean.getBoolean("io.qt.internal.enable-lambda-factory-log"))
					java.util.logging.Logger.getLogger("io.qt.internal").log(java.util.logging.Level.SEVERE, "", e);
			}
			while(false);
		}
		if(resolvedProxyFactory!=null) {
			try {
				MethodHandle slotHandle = _slotHandle.asSpreader(1, Object[].class, _slotHandle.type().parameterCount()-1);
				return resolvedProxyFactory.asInterfaceInstance(SignalUtility.SlotInvoker.class, slotHandle);
			} catch (Throwable e) {
			}
		}
		return ReflectionUtility.MethodInvocationHandler.super.getSlotInvoker(slot, _slotHandle);
	}
	
	@Override
	public SignalUtility.StaticSlotInvoker getStaticSlotInvoker(Method slot, MethodHandle _slotHandle){
		if(resolvedMetaFactory!=null) {
			do try {
				CallSite site;
				List<Class<?>> parameters = new ArrayList<>();
				for(int i=0; i<_slotHandle.type().parameterCount(); ++i) {
					parameters.add(Object.class);
				}
				MethodType invokeType = MethodType.methodType(void.class, parameters);
				switch(_slotHandle.type().parameterCount()) {
				case 0:
					site = resolvedMetaFactory.metafactory(ReflectionUtility.privateLookup(slot.getDeclaringClass()),
							"run",
			                MethodType.methodType(Runnable.class),
			                invokeType,
			                _slotHandle,
			                wrap(_slotHandle.type()));
					Runnable slot0 = (Runnable)site.getTarget().invokeExact();
					return (_arguments)->slot0.run();
				case 1:
					site = resolvedMetaFactory.metafactory(ReflectionUtility.privateLookup(slot.getDeclaringClass()),
							"accept",
			                MethodType.methodType(Consumer.class),
			                invokeType,
			                _slotHandle,
			                wrap(_slotHandle.type()));
					Consumer<Object> slot1 = (Consumer<Object>)site.getTarget().invokeExact();
					return (_arguments)->slot1.accept(_arguments[0]);
				case 2:
					site = resolvedMetaFactory.metafactory(ReflectionUtility.privateLookup(slot.getDeclaringClass()),
							"accept",
			                MethodType.methodType(BiConsumer.class),
			                invokeType,
			                _slotHandle,
			                wrap(_slotHandle.type()));
					BiConsumer<Object,Object> slot2 = (BiConsumer<Object,Object>)site.getTarget().invokeExact();
					return (_arguments)->slot2.accept(_arguments[0], _arguments[1]);
				}
				MethodHandles.Lookup lookup;
				lookup = ReflectionUtility.privateLookup(slot.getDeclaringClass());
				lookupAccessClass.accessClass(lookup, QtUtilities.class);
				switch(_slotHandle.type().parameterCount()) {
				case 3:
					site = resolvedMetaFactory.metafactory(lookup,
							"accept",
			                MethodType.methodType(QtUtilities.Consumer3.class),
			                invokeType,
			                _slotHandle,
			                wrap(_slotHandle.type()));
					QtUtilities.Consumer3<Object,Object,Object> slot3 = (QtUtilities.Consumer3<Object,Object,Object>)site.getTarget().invokeExact();
					return (_arguments)->slot3.accept(_arguments[0], _arguments[1], _arguments[2]);
				case 4:
					site = resolvedMetaFactory.metafactory(lookup,
							"accept",
			                MethodType.methodType(QtUtilities.Consumer4.class),
			                invokeType,
			                _slotHandle,
			                wrap(_slotHandle.type()));
					QtUtilities.Consumer4<Object,Object,Object,Object> slot4 = (QtUtilities.Consumer4<Object,Object,Object,Object>)site.getTarget().invokeExact();
					return (_arguments)->slot4.accept(_arguments[0], _arguments[1], _arguments[2], _arguments[3]);
				case 5:
					site = resolvedMetaFactory.metafactory(lookup,
							"accept",
			                MethodType.methodType(QtUtilities.Consumer5.class),
			                invokeType,
			                _slotHandle,
			                wrap(_slotHandle.type()));
					QtUtilities.Consumer5<Object,Object,Object,Object,Object> slot5 = (QtUtilities.Consumer5<Object,Object,Object,Object,Object>)site.getTarget().invokeExact();
					return (_arguments)->slot5.accept(_arguments[0], _arguments[1], _arguments[2], _arguments[3], _arguments[4]);
				case 6:
					site = resolvedMetaFactory.metafactory(lookup,
							"accept",
			                MethodType.methodType(QtUtilities.Consumer6.class),
			                invokeType,
			                _slotHandle,
			                wrap(_slotHandle.type()));
					QtUtilities.Consumer6<Object,Object,Object,Object,Object,Object> slot6 = (QtUtilities.Consumer6<Object,Object,Object,Object,Object,Object>)site.getTarget().invokeExact();
					return (_arguments)->slot6.accept(_arguments[0], _arguments[1], _arguments[2], _arguments[3], _arguments[4], _arguments[5]);
				case 7:
					site = resolvedMetaFactory.metafactory(lookup,
							"accept",
			                MethodType.methodType(QtUtilities.Consumer7.class),
			                invokeType,
			                _slotHandle,
			                wrap(_slotHandle.type()));
					QtUtilities.Consumer7<Object,Object,Object,Object,Object,Object,Object> slot7 = (QtUtilities.Consumer7<Object,Object,Object,Object,Object,Object,Object>)site.getTarget().invokeExact();
					return (_arguments)->slot7.accept(_arguments[0], _arguments[1], _arguments[2], _arguments[3], _arguments[4], _arguments[5], _arguments[6]);
				case 8:
					site = resolvedMetaFactory.metafactory(lookup,
							"accept",
			                MethodType.methodType(QtUtilities.Consumer8.class),
			                invokeType,
			                _slotHandle,
			                wrap(_slotHandle.type()));
					QtUtilities.Consumer8<Object,Object,Object,Object,Object,Object,Object,Object> slot8 = (QtUtilities.Consumer8<Object,Object,Object,Object,Object,Object,Object,Object>)site.getTarget().invokeExact();
					return (_arguments)->slot8.accept(_arguments[0], _arguments[1], _arguments[2], _arguments[3], _arguments[4], _arguments[5], _arguments[6], _arguments[7]);
				case 9:
					site = resolvedMetaFactory.metafactory(lookup,
							"accept",
			                MethodType.methodType(QtUtilities.Consumer9.class),
			                invokeType,
			                _slotHandle,
			                wrap(_slotHandle.type()));
					QtUtilities.Consumer9<Object,Object,Object,Object,Object,Object,Object,Object,Object> slot9 = (QtUtilities.Consumer9<Object,Object,Object,Object,Object,Object,Object,Object,Object>)site.getTarget().invokeExact();
					return (_arguments)->slot9.accept(_arguments[0], _arguments[1], _arguments[2], _arguments[3], _arguments[4], _arguments[5], _arguments[6], _arguments[7], _arguments[8]);
				}
			} catch (IllegalAccessException e) {
			} catch (Throwable e) {
				e.printStackTrace();
			} while(true);
		}
		if(resolvedProxyFactory!=null) {
			try {
					try {
						MethodHandle slotHandle = _slotHandle.asSpreader(0, Object[].class, _slotHandle.type().parameterCount());
						return resolvedProxyFactory.asInterfaceInstance(SignalUtility.StaticSlotInvoker.class, slotHandle);
					} catch (Throwable e) {
					}
			} catch (Throwable e) {
			}
		}
		return ReflectionUtility.MethodInvocationHandler.super.getStaticSlotInvoker(slot, _slotHandle);
	}
	
	@Override
	public SignalUtility.LambdaArgsSlotInvoker getSlotInvoker(Method slot, MethodHandle _slotHandle, int lambdaArgsCount){
		if(resolvedProxyFactory!=null) {
			try {
				MethodType mt = _slotHandle.type();
				for(int i=0; i<mt.parameterCount(); ++i){
					mt = mt.changeParameterType(i, Object.class);
				}
				_slotHandle = _slotHandle.asType(mt);
				MethodHandle slotHandle = _slotHandle.asSpreader(1, Object[].class, lambdaArgsCount).asSpreader(2, Object[].class, _slotHandle.type().parameterCount() - 1 - lambdaArgsCount);
				return resolvedProxyFactory.asInterfaceInstance(SignalUtility.LambdaArgsSlotInvoker.class, slotHandle);
			} catch (Throwable e) {
			}
		}
		return ReflectionUtility.MethodInvocationHandler.super.getSlotInvoker(slot, _slotHandle, lambdaArgsCount);
	}
	
	@Override
	public SignalUtility.StaticLambdaArgsSlotInvoker getStaticSlotInvoker(Method slot, MethodHandle _slotHandle, int lambdaArgsCount){
		if(resolvedProxyFactory!=null) {
			try {
				MethodType mt = _slotHandle.type();
				for(int i=0; i<mt.parameterCount(); ++i){
					mt = mt.changeParameterType(i, Object.class);
				}
				_slotHandle = _slotHandle.asType(mt);
				MethodHandle slotHandle = _slotHandle.asSpreader(0, Object[].class, lambdaArgsCount).asSpreader(1, Object[].class, _slotHandle.type().parameterCount() - lambdaArgsCount);
				return resolvedProxyFactory.asInterfaceInstance(SignalUtility.StaticLambdaArgsSlotInvoker.class, slotHandle);
			} catch (Throwable e) {
			}
		}
		return ReflectionUtility.MethodInvocationHandler.super.getStaticSlotInvoker(slot, _slotHandle, lambdaArgsCount);
	}

	@SuppressWarnings("unchecked")
	@Override
	public <T,V> BiConsumer<T,V> getFieldSetter(Field f) {
		java.lang.invoke.MethodHandles.Lookup lookup = ReflectionUtility.privateLookup(f.getDeclaringClass());
		try {
			MethodHandle setter = lookup.unreflectSetter(f);
			if(resolvedMetaFactory!=null) {
				try {
					CallSite site = resolvedMetaFactory.metafactory(ReflectionUtility.privateLookup(f.getDeclaringClass()),
							"accept",
			                MethodType.methodType(BiConsumer.class),
			                MethodType.methodType(void.class, Object.class, Object.class),
			                setter,
			                wrap(setter.type()));
					return (BiConsumer<T,V>)site.getTarget().invokeExact();
				} catch (Throwable e) {
				}
			}
			if(resolvedProxyFactory!=null) {
				try {
					return (BiConsumer<T,V>)resolvedProxyFactory.asInterfaceInstance(BiConsumer.class, setter);
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
