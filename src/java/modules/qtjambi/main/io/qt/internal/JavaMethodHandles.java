package io.qt.internal;

import java.lang.invoke.CallSite;
import java.lang.invoke.LambdaConversionException;
import java.lang.invoke.MethodHandle;
import java.lang.invoke.MethodHandles;
import java.lang.invoke.MethodHandles.Lookup;
import java.lang.invoke.MethodType;
import java.lang.reflect.Constructor;
import java.lang.reflect.Executable;
import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.lang.reflect.Modifier;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Objects;
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
	
	static class MethodInfo{
		MethodInfo(Method slot, MethodHandle slotHandle) {
			this.slot = Objects.requireNonNull(slot);
			if(slotHandle==null) {
	        	try {
	        		slotHandle = ReflectionUtility.getMethodHandle(slot);
				} catch (Exception e) {
				}
	        }
			this.slotHandle = slotHandle;
		}
		final Method slot;
		final MethodHandle slotHandle;
		@Override
		public int hashCode() {
			final int prime = 31;
			int result = 1;
			result = prime * result + slot.getDeclaringClass().hashCode();
			result = prime * result + slot.getName().hashCode();
			if(slotHandle != null) {
				result = prime * result + slotHandle.type().hashCode();
			}else {
				result = prime * result + Arrays.hashCode(slot.getParameterTypes());
			}
			return result;
		}
		@Override
		public boolean equals(Object obj) {
			if (this == obj)
				return true;
			if (obj == null)
				return false;
			if (getClass() != obj.getClass())
				return false;
			MethodInfo other = (MethodInfo) obj;
			if (!slot.equals(other.slot))
				return false;
			return true;
		}
	}
	
	static class LambdaInfo extends MethodInfo{

		LambdaInfo(Method slot, MethodHandle slotHandle, int lambdaArgsCount) {
			super(slot, slotHandle);
			this.lambdaArgsCount = lambdaArgsCount;
		}
		
		final int lambdaArgsCount;
		
		@Override
		public int hashCode() {
			final int prime = 31;
			int result = super.hashCode();
			result = prime * result + lambdaArgsCount;
			return result;
		}
	}
	
	interface MetaFactory{
		public CallSite metafactory(MethodHandles.Lookup caller,
                String interfaceMethodName,
                MethodType factoryType,
                MethodType interfaceMethodType,
                MethodHandle implementation,
                MethodType dynamicMethodType) throws LambdaConversionException;
	}
	private final MetaFactory resolvedMetaFactory;
	interface AccessClass{
		public Class<?> accessClass(MethodHandles.Lookup lookup, Class<?> targetClass) throws IllegalAccessException;
	}
	private final AccessClass lookupAccessClass;

	JavaMethodHandles() {
		MetaFactory _resolvedMetaFactory = null;
		AccessClass _lookupAccessClass = null;
		if(!Boolean.getBoolean("io.qt.internal.disable-metalambdas")) {
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
			try {
				if(LibraryUtility.operatingSystem==OperatingSystem.Android) {
					MethodHandles.Lookup.class.getMethod("accessClass", Class.class);
				}
				_lookupAccessClass = MethodHandles.Lookup::accessClass;
			}catch(Throwable e) {
				_lookupAccessClass = (_lookup,_cls)->_cls;
			}
		}
		resolvedMetaFactory = _resolvedMetaFactory;
		lookupAccessClass = _lookupAccessClass;
	}
	
	static MethodType wrap(MethodType mt) {
		mt = mt.wrap().changeReturnType(void.class);
		return mt;
	}
	
	static class ExecutableHash<Member extends Executable>{
		ExecutableHash(Member member) {
			this.member = member;
		}

		final Member member;
		
		@Override
		public int hashCode() {
			final int prime = 31;
			int result = 1;
			if(member != null) {
				result = prime * result + member.getDeclaringClass().hashCode();
				result = prime * result + Arrays.hashCode(member.getParameterTypes());
			}
			return result;
		}
		@Override
		public boolean equals(Object obj) {
			if (this == obj)
				return true;
			if (obj == null)
				return false;
			if (getClass() != obj.getClass())
				return false;
			ExecutableHash<?> other = (ExecutableHash<?>) obj;
			if (member == null) {
				if (other.member != null)
					return false;
			} else if (!member.equals(other.member))
				return false;
			return true;
		}
	}
	
	static class MethodHash extends ExecutableHash<Method>{
		MethodHash(Method method) {
			super(method);
		}
		
		@Override
		public int hashCode() {
			final int prime = 31;
			int result = super.hashCode();
			if(member != null) {
				result = prime * result + member.getName().hashCode();
			}
			return result;
		}
	}
	
	/**
	 * This method needs to be called in cached context only!
	 */
	@Override
	public <A,B> Function<A,B> functionFromMethod(Method method){
		if(method==null)
			return null;
		try {
			Lookup lookup = ReflectionUtility.privateLookup(method.getDeclaringClass());
			MethodHandle handle = lookup.unreflect(method);
			if(resolvedMetaFactory!=null && !method.getDeclaringClass().isSynthetic()) {
				try {
					CallSite site = resolvedMetaFactory.metafactory(lookup,
							"apply",
			                MethodType.methodType(Function.class),
			                MethodType.methodType(Object.class, Object.class),
			                handle,
			                handle.type().wrap());
					return (Function<A,B>)site.getTarget().invokeExact();
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
	public BiConsumer<Object,QDataStream> streamIO(Method method){
		java.lang.invoke.MethodHandles.Lookup lookup = ReflectionUtility.privateLookup(method.getDeclaringClass());
		try {
			MethodHandle slotHandle = lookup.unreflect(method);
			try {
				if(resolvedMetaFactory!=null && !method.getDeclaringClass().isSynthetic()) {
					try {
						CallSite site = resolvedMetaFactory.metafactory(lookup,
								"accept",
				                MethodType.methodType(BiConsumer.class),
				                MethodType.methodType(void.class, Object.class, Object.class),
				                slotHandle,
				                wrap(slotHandle.type()));
						return (BiConsumer<Object,QDataStream>)site.getTarget().invokeExact();
					} catch (Throwable e) {
					}
				}
			} catch (RuntimeException | Error e) {
				throw e;
			} catch (Throwable e) {
				throw new RuntimeException(e);
			}
			return (o,s)->{
				try {
					slotHandle.invoke(o, s);
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
	
	private final Map<ExecutableHash<Constructor<?>>,Function<?,?>> genericFactories = Collections.synchronizedMap(new HashMap<>());
	
	@Override
	@SuppressWarnings("unchecked")
	public <T> Function<Object[],T> getFactory(Constructor<T> constructor) {
		if(constructor==null)
			return null;
		return (Function<Object[],T>)genericFactories.computeIfAbsent(new ExecutableHash<>(constructor), info->{
			try {
				java.lang.invoke.MethodHandles.Lookup lookup = ReflectionUtility.privateLookup(info.member.getDeclaringClass());
				MethodHandle handle = lookup.unreflectConstructor(info.member);
				return (Function<Object[],?>)args->{
					try {
						if(args!=null) {
							return handle.invokeWithArguments(args);
						}else {
							return handle.invoke();
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
		});
	}
	
	private final Map<ExecutableHash<Constructor<?>>,Object> factories = Collections.synchronizedMap(new HashMap<>());
	
	@Override
	@SuppressWarnings("unchecked")
	public <T> Supplier<T> getFactory0(Constructor<T> constructor) {
		if(constructor==null)
			return null;
		if(constructor.getParameterCount()!=0)
			throw new RuntimeException("Parameter count mismatch");
		return (Supplier<T>)factories.computeIfAbsent(new ExecutableHash<>(constructor), info->{
			try {
				java.lang.invoke.MethodHandles.Lookup lookup = ReflectionUtility.privateLookup(info.member.getDeclaringClass());
				MethodHandle handle = lookup.unreflectConstructor(info.member);
				if(resolvedMetaFactory!=null && !info.member.getDeclaringClass().isSynthetic()) {
					try {
						CallSite site = resolvedMetaFactory.metafactory(lookup,
								"get",
				                MethodType.methodType(Supplier.class),
				                MethodType.methodType(Object.class),
				                handle,
				                wrap(handle.type()));
						return site.getTarget().invokeExact();
					} catch (Throwable e) {
					}
				}
				return (Supplier<?>)()->{
					try {
						return handle.invoke();
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
		});
	}
	
	@Override
	@SuppressWarnings("unchecked")
	public <A,T> Function<A,T> getFactory1(Constructor<T> constructor) {
		if(constructor==null)
			return null;
		if(constructor.getParameterCount()!=1)
			throw new RuntimeException("Parameter count mismatch");
		return (Function<A,T>)factories.computeIfAbsent(new ExecutableHash<>(constructor), info->{
			try {
				java.lang.invoke.MethodHandles.Lookup lookup = ReflectionUtility.privateLookup(info.member.getDeclaringClass());
				MethodHandle handle = lookup.unreflectConstructor(info.member);
				if(resolvedMetaFactory!=null && !info.member.getDeclaringClass().isSynthetic()) {
					try {
						CallSite site = resolvedMetaFactory.metafactory(lookup,
								"apply",
				                MethodType.methodType(Function.class),
				                MethodType.methodType(Object.class, Object.class),
				                handle,
				                wrap(handle.type()));
						return site.getTarget().invokeExact();
					} catch (Throwable e) {
					}
				}
				return (Function<?,?>)a->{
					try {
						return handle.invoke(a);
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
		});
	}
	
	@Override
	@SuppressWarnings("unchecked")
	public <A,B,T> BiFunction<A,B,T> getFactory2(Constructor<T> constructor) {
		if(constructor==null)
			return null;
		if(constructor.getParameterCount()!=2)
			throw new RuntimeException("Parameter count mismatch");
		return (BiFunction<A,B,T>)factories.computeIfAbsent(new ExecutableHash<>(constructor), info->{
			try {
				java.lang.invoke.MethodHandles.Lookup lookup = ReflectionUtility.privateLookup(info.member.getDeclaringClass());
				MethodHandle handle = lookup.unreflectConstructor(info.member);
				if(resolvedMetaFactory!=null && !info.member.getDeclaringClass().isSynthetic()) {
					try {
						CallSite site = resolvedMetaFactory.metafactory(lookup,
								"apply",
				                MethodType.methodType(BiFunction.class),
				                MethodType.methodType(Object.class, Object.class, Object.class),
				                handle,
				                wrap(handle.type()));
						return site.getTarget().invokeExact();
					} catch (Throwable e) {
					}
				}
				return (BiFunction<?,?,?>)(a,b)->{
					try {
						return handle.invoke(a,b);
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
		});
	}
	
	@Override
	@SuppressWarnings("unchecked")
	public <A,B,C,T> QMetaObject.Method3<A,B,C,T> getFactory3(Constructor<T> constructor) {
		if(constructor==null)
			return null;
		if(constructor.getParameterCount()!=3)
			throw new RuntimeException("Parameter count mismatch");
		return (QMetaObject.Method3<A,B,C,T>)factories.computeIfAbsent(new ExecutableHash<>(constructor), info->{
			try {
				java.lang.invoke.MethodHandles.Lookup lookup = ReflectionUtility.privateLookup(info.member.getDeclaringClass());
				MethodHandle handle = lookup.unreflectConstructor(info.member);
				if(resolvedMetaFactory!=null && !info.member.getDeclaringClass().isSynthetic()) {
					try {
						lookupAccessClass.accessClass(lookup, QMetaObject.class);
						CallSite site = resolvedMetaFactory.metafactory(lookup,
								"invoke",
				                MethodType.methodType(QMetaObject.Method3.class),
				                MethodType.methodType(Object.class, Object.class, Object.class, Object.class),
				                handle,
				                wrap(handle.type()));
						return site.getTarget().invokeExact();
					} catch (Throwable e) {
					}
				}
				return (QMetaObject.Method3<?,?,?,?>)(a,b,c)->{
					try {
						return handle.invoke(a,b,c);
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
		});
	}
	
	@Override
	@SuppressWarnings("unchecked")
	public <A,B,C,D,T> QMetaObject.Method4<A,B,C,D,T> getFactory4(Constructor<T> constructor) {
		if(constructor==null)
			return null;
		if(constructor.getParameterCount()!=4)
			throw new RuntimeException("Parameter count mismatch");
		return (QMetaObject.Method4<A,B,C,D,T>)factories.computeIfAbsent(new ExecutableHash<>(constructor), info->{
			try {
				java.lang.invoke.MethodHandles.Lookup lookup = ReflectionUtility.privateLookup(info.member.getDeclaringClass());
				MethodHandle handle = lookup.unreflectConstructor(info.member);
				if(resolvedMetaFactory!=null && !info.member.getDeclaringClass().isSynthetic()) {
					try {
						lookupAccessClass.accessClass(lookup, QMetaObject.class);
						CallSite site = resolvedMetaFactory.metafactory(lookup,
								"invoke",
				                MethodType.methodType(QMetaObject.Method4.class),
				                MethodType.methodType(Object.class, Object.class, Object.class, Object.class, Object.class),
				                handle,
				                wrap(handle.type()));
						return site.getTarget().invokeExact();
					} catch (Throwable e) {
					}
				}
				return (QMetaObject.Method4<?,?,?,?,?>)(a,b,c,d)->{
					try {
						return handle.invoke(a,b,c,d);
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
		});
	}
	
	@Override
	@SuppressWarnings("unchecked")
	public <A,B,C,D,E,T> QMetaObject.Method5<A,B,C,D,E,T> getFactory5(Constructor<T> constructor) {
		if(constructor==null)
			return null;
		if(constructor.getParameterCount()!=5)
			throw new RuntimeException("Parameter count mismatch");
		return (QMetaObject.Method5<A,B,C,D,E,T>)factories.computeIfAbsent(new ExecutableHash<>(constructor), info->{
			try {
				java.lang.invoke.MethodHandles.Lookup lookup = ReflectionUtility.privateLookup(info.member.getDeclaringClass());
				MethodHandle handle = lookup.unreflectConstructor(info.member);
				if(resolvedMetaFactory!=null && !info.member.getDeclaringClass().isSynthetic()) {
					try {
						lookupAccessClass.accessClass(lookup, QMetaObject.class);
						CallSite site = resolvedMetaFactory.metafactory(lookup,
								"invoke",
				                MethodType.methodType(QMetaObject.Method5.class),
				                MethodType.methodType(Object.class, Object.class, Object.class, Object.class, Object.class, Object.class),
				                handle,
				                wrap(handle.type()));
						return site.getTarget().invokeExact();
					} catch (Throwable e) {
					}
				}
				return (QMetaObject.Method5<?,?,?,?,?,?>)(a,b,c,d,e)->{
					try {
						return handle.invoke(a,b,c,d,e);
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
		});
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
	
	private final Map<MethodInfo,SignalUtility.SlotInvoker> slotInvokers = Collections.synchronizedMap(new HashMap<>());
	
	@Override
	public SignalUtility.SlotInvoker getSlotInvoker(Method slot, MethodHandle slotHandle){
		return slotInvokers.computeIfAbsent(new MethodInfo(slot, slotHandle), methodInfo -> {
			if(methodInfo.slotHandle!=null) {
				if(resolvedMetaFactory!=null && !methodInfo.slot.getDeclaringClass().isSynthetic()) {
					try {
						CallSite site;
						List<Class<?>> parameters = new ArrayList<>();
						for(int i=0; i<methodInfo.slotHandle.type().parameterCount(); ++i) {
							parameters.add(Object.class);
						}
						MethodType invokeType = MethodType.methodType(void.class, parameters);
						MethodHandles.Lookup lookup = ReflectionUtility.privateLookup(methodInfo.slot.getDeclaringClass());
						switch(methodInfo.slotHandle.type().parameterCount()) {
						case 1:
							site = resolvedMetaFactory.metafactory(lookup,
									"accept",
					                MethodType.methodType(Consumer.class),
					                invokeType,
					                methodInfo.slotHandle,
					                wrap(methodInfo.slotHandle.type()));
							Consumer<Object> slot1 = (Consumer<Object>)site.getTarget().invokeExact();
							return (instance, _arguments)->slot1.accept(instance);
						case 2:
							site = resolvedMetaFactory.metafactory(lookup,
									"accept",
					                MethodType.methodType(BiConsumer.class),
					                invokeType,
					                methodInfo.slotHandle,
					                wrap(methodInfo.slotHandle.type()));
							BiConsumer<Object,Object> slot2 = (BiConsumer<Object,Object>)site.getTarget().invokeExact();
							return (instance, _arguments)->slot2.accept(instance, _arguments[0]);
						default: break;
						}
						lookupAccessClass.accessClass(lookup, QtUtilities.class);
						switch(methodInfo.slotHandle.type().parameterCount()) {
						case 3:
							site = resolvedMetaFactory.metafactory(lookup,
									"accept",
					                MethodType.methodType(QtUtilities.Consumer3.class),
					                invokeType,
					                methodInfo.slotHandle,
					                wrap(methodInfo.slotHandle.type()));
							QtUtilities.Consumer3<Object,Object,Object> slot3 = (QtUtilities.Consumer3<Object,Object,Object>)site.getTarget().invokeExact();
							return (instance, _arguments)->slot3.accept(instance, _arguments[0], _arguments[1]);
						case 4:
							site = resolvedMetaFactory.metafactory(lookup,
									"accept",
					                MethodType.methodType(QtUtilities.Consumer4.class),
					                invokeType,
					                methodInfo.slotHandle,
					                wrap(methodInfo.slotHandle.type()));
							QtUtilities.Consumer4<Object,Object,Object,Object> slot4 = (QtUtilities.Consumer4<Object,Object,Object,Object>)site.getTarget().invokeExact();
							return (instance, _arguments)->slot4.accept(instance, _arguments[0], _arguments[1], _arguments[2]);
						case 5:
							site = resolvedMetaFactory.metafactory(lookup,
									"accept",
					                MethodType.methodType(QtUtilities.Consumer5.class),
					                invokeType,
					                methodInfo.slotHandle,
					                wrap(methodInfo.slotHandle.type()));
							QtUtilities.Consumer5<Object,Object,Object,Object,Object> slot5 = (QtUtilities.Consumer5<Object,Object,Object,Object,Object>)site.getTarget().invokeExact();
							return (instance, _arguments)->slot5.accept(instance, _arguments[0], _arguments[1], _arguments[2], _arguments[3]);
						case 6:
							site = resolvedMetaFactory.metafactory(lookup,
									"accept",
					                MethodType.methodType(QtUtilities.Consumer6.class),
					                invokeType,
					                methodInfo.slotHandle,
					                wrap(methodInfo.slotHandle.type()));
							QtUtilities.Consumer6<Object,Object,Object,Object,Object,Object> slot6 = (QtUtilities.Consumer6<Object,Object,Object,Object,Object,Object>)site.getTarget().invokeExact();
							return (instance, _arguments)->slot6.accept(instance, _arguments[0], _arguments[1], _arguments[2], _arguments[3], _arguments[4]);
						case 7:
							site = resolvedMetaFactory.metafactory(lookup,
									"accept",
					                MethodType.methodType(QtUtilities.Consumer7.class),
					                invokeType,
					                methodInfo.slotHandle,
					                wrap(methodInfo.slotHandle.type()));
							QtUtilities.Consumer7<Object,Object,Object,Object,Object,Object,Object> slot7 = (QtUtilities.Consumer7<Object,Object,Object,Object,Object,Object,Object>)site.getTarget().invokeExact();
							return (instance, _arguments)->slot7.accept(instance, _arguments[0], _arguments[1], _arguments[2], _arguments[3], _arguments[4], _arguments[5]);
						case 8:
							site = resolvedMetaFactory.metafactory(lookup,
									"accept",
					                MethodType.methodType(QtUtilities.Consumer8.class),
					                invokeType,
					                methodInfo.slotHandle,
					                wrap(methodInfo.slotHandle.type()));
							QtUtilities.Consumer8<Object,Object,Object,Object,Object,Object,Object,Object> slot8 = (QtUtilities.Consumer8<Object,Object,Object,Object,Object,Object,Object,Object>)site.getTarget().invokeExact();
							return (instance, _arguments)->slot8.accept(instance, _arguments[0], _arguments[1], _arguments[2], _arguments[3], _arguments[4], _arguments[5], _arguments[6]);
						case 9:
							site = resolvedMetaFactory.metafactory(lookup,
									"accept",
					                MethodType.methodType(QtUtilities.Consumer9.class),
					                invokeType,
					                methodInfo.slotHandle,
					                wrap(methodInfo.slotHandle.type()));
							QtUtilities.Consumer9<Object,Object,Object,Object,Object,Object,Object,Object,Object> slot9 = (QtUtilities.Consumer9<Object,Object,Object,Object,Object,Object,Object,Object,Object>)site.getTarget().invokeExact();
							return (instance, _arguments)->slot9.accept(instance, _arguments[0], _arguments[1], _arguments[2], _arguments[3], _arguments[4], _arguments[5], _arguments[6], _arguments[7]);
						case 10:
							site = resolvedMetaFactory.metafactory(lookup,
									"accept",
					                MethodType.methodType(QtUtilities.Consumer10.class),
					                invokeType,
					                methodInfo.slotHandle,
					                wrap(methodInfo.slotHandle.type()));
							QtUtilities.Consumer10<Object,Object,Object,Object,Object,Object,Object,Object,Object,Object> slot10 = (QtUtilities.Consumer10<Object,Object,Object,Object,Object,Object,Object,Object,Object,Object>)site.getTarget().invokeExact();
							return (instance, _arguments)->slot10.accept(instance, _arguments[0], _arguments[1], _arguments[2], _arguments[3], _arguments[4], _arguments[5], _arguments[6], _arguments[7], _arguments[8]);
						default: break;
						}
					} catch (IllegalAccessException e) {
					} catch (Throwable e) {
						if(Boolean.getBoolean("io.qt.internal.enable-lambda-factory-log"))
							java.util.logging.Logger.getLogger("io.qt.internal").log(java.util.logging.Level.SEVERE, "", e);
					}
				}
				MethodHandle _slotHandle = methodInfo.slotHandle;
				switch(methodInfo.slotHandle.type().parameterCount()) {
				case 1:
					return (instance, _arguments)->_slotHandle.invoke(instance);
				case 2:
					return (instance, _arguments)->_slotHandle.invoke(instance, _arguments[0]);
				case 3:
					return (instance, _arguments)->_slotHandle.invoke(instance, _arguments[0], _arguments[1]);
				case 4:
					return (instance, _arguments)->_slotHandle.invoke(instance, _arguments[0], _arguments[1], _arguments[2]);
				case 5:
					return (instance, _arguments)->_slotHandle.invoke(instance, _arguments[0], _arguments[1], _arguments[2], _arguments[3]);
				case 6:
					return (instance, _arguments)->_slotHandle.invoke(instance, _arguments[0], _arguments[1], _arguments[2], _arguments[3], _arguments[4]);
				case 7:
					return (instance, _arguments)->_slotHandle.invoke(instance, _arguments[0], _arguments[1], _arguments[2], _arguments[3], _arguments[4], _arguments[5]);
				case 8:
					return (instance, _arguments)->_slotHandle.invoke(instance, _arguments[0], _arguments[1], _arguments[2], _arguments[3], _arguments[4], _arguments[5], _arguments[6]);
				case 9:
					return (instance, _arguments)->_slotHandle.invoke(instance, _arguments[0], _arguments[1], _arguments[2], _arguments[3], _arguments[4], _arguments[5], _arguments[6], _arguments[7]);
				case 10:
					return (instance, _arguments)->_slotHandle.invoke(instance, _arguments[0], _arguments[1], _arguments[2], _arguments[3], _arguments[4], _arguments[5], _arguments[6], _arguments[7], _arguments[8]);
				default:
					try {
						MethodHandle __slotHandle = methodInfo.slotHandle.asSpreader(1, Object[].class, methodInfo.slotHandle.type().parameterCount()-1);
						return (instance, _arguments)->__slotHandle.invoke(instance, _arguments);
					} catch (Throwable e) {
					}
				}
			}
			return ReflectionUtility.MethodInvocationHandler.super.getSlotInvoker(methodInfo.slot, methodInfo.slotHandle);
		});
	}
	
	private final Map<MethodInfo,SignalUtility.StaticSlotInvoker> staticSlotInvokers = Collections.synchronizedMap(new HashMap<>());
	
	@Override
	public SignalUtility.StaticSlotInvoker getStaticSlotInvoker(Method slot, MethodHandle slotHandle){
		return staticSlotInvokers.computeIfAbsent(new MethodInfo(slot, slotHandle), methodInfo -> {
				if(methodInfo.slotHandle!=null) {
					if(resolvedMetaFactory!=null && !methodInfo.slot.getDeclaringClass().isSynthetic()) {
						try {
							CallSite site;
							List<Class<?>> parameters = new ArrayList<>();
							for(int i=0; i<methodInfo.slotHandle.type().parameterCount(); ++i) {
								parameters.add(Object.class);
							}
							MethodType invokeType = MethodType.methodType(void.class, parameters);
							MethodHandles.Lookup lookup = ReflectionUtility.privateLookup(methodInfo.slot.getDeclaringClass());
							switch(methodInfo.slotHandle.type().parameterCount()) {
							case 0:
								site = resolvedMetaFactory.metafactory(lookup,
										"run",
						                MethodType.methodType(Runnable.class),
						                invokeType,
						                methodInfo.slotHandle,
						                wrap(methodInfo.slotHandle.type()));
								Runnable slot0 = (Runnable)site.getTarget().invokeExact();
								return (_arguments)->slot0.run();
							case 1:
								site = resolvedMetaFactory.metafactory(lookup,
										"accept",
						                MethodType.methodType(Consumer.class),
						                invokeType,
						                methodInfo.slotHandle,
						                wrap(methodInfo.slotHandle.type()));
								Consumer<Object> slot1 = (Consumer<Object>)site.getTarget().invokeExact();
								return (_arguments)->slot1.accept(_arguments[0]);
							case 2:
								site = resolvedMetaFactory.metafactory(lookup,
										"accept",
						                MethodType.methodType(BiConsumer.class),
						                invokeType,
						                methodInfo.slotHandle,
						                wrap(methodInfo.slotHandle.type()));
								BiConsumer<Object,Object> slot2 = (BiConsumer<Object,Object>)site.getTarget().invokeExact();
								return (_arguments)->slot2.accept(_arguments[0], _arguments[1]);
							}
							lookupAccessClass.accessClass(lookup, QtUtilities.class);
							switch(methodInfo.slotHandle.type().parameterCount()) {
							case 3:
								site = resolvedMetaFactory.metafactory(lookup,
										"accept",
						                MethodType.methodType(QtUtilities.Consumer3.class),
						                invokeType,
						                methodInfo.slotHandle,
						                wrap(methodInfo.slotHandle.type()));
								QtUtilities.Consumer3<Object,Object,Object> slot3 = (QtUtilities.Consumer3<Object,Object,Object>)site.getTarget().invokeExact();
								return (_arguments)->slot3.accept(_arguments[0], _arguments[1], _arguments[2]);
							case 4:
								site = resolvedMetaFactory.metafactory(lookup,
										"accept",
						                MethodType.methodType(QtUtilities.Consumer4.class),
						                invokeType,
						                methodInfo.slotHandle,
						                wrap(methodInfo.slotHandle.type()));
								QtUtilities.Consumer4<Object,Object,Object,Object> slot4 = (QtUtilities.Consumer4<Object,Object,Object,Object>)site.getTarget().invokeExact();
								return (_arguments)->slot4.accept(_arguments[0], _arguments[1], _arguments[2], _arguments[3]);
							case 5:
								site = resolvedMetaFactory.metafactory(lookup,
										"accept",
						                MethodType.methodType(QtUtilities.Consumer5.class),
						                invokeType,
						                methodInfo.slotHandle,
						                wrap(methodInfo.slotHandle.type()));
								QtUtilities.Consumer5<Object,Object,Object,Object,Object> slot5 = (QtUtilities.Consumer5<Object,Object,Object,Object,Object>)site.getTarget().invokeExact();
								return (_arguments)->slot5.accept(_arguments[0], _arguments[1], _arguments[2], _arguments[3], _arguments[4]);
							case 6:
								site = resolvedMetaFactory.metafactory(lookup,
										"accept",
						                MethodType.methodType(QtUtilities.Consumer6.class),
						                invokeType,
						                methodInfo.slotHandle,
						                wrap(methodInfo.slotHandle.type()));
								QtUtilities.Consumer6<Object,Object,Object,Object,Object,Object> slot6 = (QtUtilities.Consumer6<Object,Object,Object,Object,Object,Object>)site.getTarget().invokeExact();
								return (_arguments)->slot6.accept(_arguments[0], _arguments[1], _arguments[2], _arguments[3], _arguments[4], _arguments[5]);
							case 7:
								site = resolvedMetaFactory.metafactory(lookup,
										"accept",
						                MethodType.methodType(QtUtilities.Consumer7.class),
						                invokeType,
						                methodInfo.slotHandle,
						                wrap(methodInfo.slotHandle.type()));
								QtUtilities.Consumer7<Object,Object,Object,Object,Object,Object,Object> slot7 = (QtUtilities.Consumer7<Object,Object,Object,Object,Object,Object,Object>)site.getTarget().invokeExact();
								return (_arguments)->slot7.accept(_arguments[0], _arguments[1], _arguments[2], _arguments[3], _arguments[4], _arguments[5], _arguments[6]);
							case 8:
								site = resolvedMetaFactory.metafactory(lookup,
										"accept",
						                MethodType.methodType(QtUtilities.Consumer8.class),
						                invokeType,
						                methodInfo.slotHandle,
						                wrap(methodInfo.slotHandle.type()));
								QtUtilities.Consumer8<Object,Object,Object,Object,Object,Object,Object,Object> slot8 = (QtUtilities.Consumer8<Object,Object,Object,Object,Object,Object,Object,Object>)site.getTarget().invokeExact();
								return (_arguments)->slot8.accept(_arguments[0], _arguments[1], _arguments[2], _arguments[3], _arguments[4], _arguments[5], _arguments[6], _arguments[7]);
							case 9:
								site = resolvedMetaFactory.metafactory(lookup,
										"accept",
						                MethodType.methodType(QtUtilities.Consumer9.class),
						                invokeType,
						                methodInfo.slotHandle,
						                wrap(methodInfo.slotHandle.type()));
								QtUtilities.Consumer9<Object,Object,Object,Object,Object,Object,Object,Object,Object> slot9 = (QtUtilities.Consumer9<Object,Object,Object,Object,Object,Object,Object,Object,Object>)site.getTarget().invokeExact();
								return (_arguments)->slot9.accept(_arguments[0], _arguments[1], _arguments[2], _arguments[3], _arguments[4], _arguments[5], _arguments[6], _arguments[7], _arguments[8]);
							}
						} catch (IllegalAccessException e) {
						} catch (Throwable e) {
							e.printStackTrace();
						}
					}
					MethodHandle _slotHandle = methodInfo.slotHandle;
					switch(methodInfo.slotHandle.type().parameterCount()) {
					case 0:
						return _arguments->_slotHandle.invoke();
					case 1:
						return _arguments->_slotHandle.invoke(_arguments[0]);
					case 2:
						return _arguments->_slotHandle.invoke(_arguments[0], _arguments[1]);
					case 3:
						return _arguments->_slotHandle.invoke(_arguments[0], _arguments[1], _arguments[2]);
					case 4:
						return _arguments->_slotHandle.invoke(_arguments[0], _arguments[1], _arguments[2], _arguments[3]);
					case 5:
						return _arguments->_slotHandle.invoke(_arguments[0], _arguments[1], _arguments[2], _arguments[3], _arguments[4]);
					case 6:
						return _arguments->_slotHandle.invoke(_arguments[0], _arguments[1], _arguments[2], _arguments[3], _arguments[4], _arguments[5]);
					case 7:
						return _arguments->_slotHandle.invoke(_arguments[0], _arguments[1], _arguments[2], _arguments[3], _arguments[4], _arguments[5], _arguments[6]);
					case 8:
						return _arguments->_slotHandle.invoke(_arguments[0], _arguments[1], _arguments[2], _arguments[3], _arguments[4], _arguments[5], _arguments[6], _arguments[7]);
					case 9:
						return _arguments->_slotHandle.invoke(_arguments[0], _arguments[1], _arguments[2], _arguments[3], _arguments[4], _arguments[5], _arguments[6], _arguments[7], _arguments[8]);
					default:
						try {
							MethodHandle __slotHandle = methodInfo.slotHandle.asSpreader(0, Object[].class, methodInfo.slotHandle.type().parameterCount());
							return _arguments->__slotHandle.invoke(_arguments);
						} catch (Throwable e) {
						}
					}
				}
				return ReflectionUtility.MethodInvocationHandler.super.getStaticSlotInvoker(methodInfo.slot, methodInfo.slotHandle);
		});
	}
	
	private final Map<LambdaInfo,SignalUtility.LambdaArgsSlotInvoker> lambdaArgsSlotInvokers = Collections.synchronizedMap(new HashMap<>());
	
	@Override
	public SignalUtility.LambdaArgsSlotInvoker getSlotInvoker(Method slot, MethodHandle slotHandle, int lambdaArgsCount){
		return lambdaArgsSlotInvokers.computeIfAbsent(new LambdaInfo(slot, slotHandle, lambdaArgsCount), info->{
			if(info.slotHandle!=null) {
				try {
					MethodType mt = info.slotHandle.type();
					for(int i=0; i<mt.parameterCount(); ++i){
						mt = mt.changeParameterType(i, Object.class);
					}
					MethodHandle tmpSlotHandle = info.slotHandle.asType(mt);
					MethodHandle _slotHandle = tmpSlotHandle.asSpreader(1, Object[].class, lambdaArgsCount).asSpreader(2, Object[].class, tmpSlotHandle.type().parameterCount() - 1 - lambdaArgsCount);
					return (instance, args, _arguments)->_slotHandle.invoke(instance, args, _arguments);
				} catch (Throwable e) {
				}
			}
			return ReflectionUtility.MethodInvocationHandler.super.getSlotInvoker(slot, info.slotHandle, lambdaArgsCount);
		});
	}
	
	private final Map<LambdaInfo,SignalUtility.StaticLambdaArgsSlotInvoker> staticLambdaArgsSlotInvokers = Collections.synchronizedMap(new HashMap<>());
	
	@Override
	public SignalUtility.StaticLambdaArgsSlotInvoker getStaticSlotInvoker(Method slot, MethodHandle slotHandle, int lambdaArgsCount){
		return staticLambdaArgsSlotInvokers.computeIfAbsent(new LambdaInfo(slot, slotHandle, lambdaArgsCount), info->{
			if(info.slotHandle!=null) {
				try {
					MethodType mt = info.slotHandle.type();
					for(int i=0; i<mt.parameterCount(); ++i){
						mt = mt.changeParameterType(i, Object.class);
					}
					MethodHandle tmpSlotHandle = info.slotHandle.asType(mt);
					MethodHandle _slotHandle = tmpSlotHandle.asSpreader(0, Object[].class, lambdaArgsCount).asSpreader(1, Object[].class, tmpSlotHandle.type().parameterCount() - lambdaArgsCount);
					return (args, _arguments)->_slotHandle.invoke(args, _arguments);
				} catch (Throwable e) {
				}
			}
			return ReflectionUtility.MethodInvocationHandler.super.getStaticSlotInvoker(slot, info.slotHandle, lambdaArgsCount);
		});
	}
	
	private final Map<Field,BiConsumer<?,?>> fieldAccessors = Collections.synchronizedMap(new HashMap<>());

	@SuppressWarnings("unchecked")
	@Override
	public <T,V> BiConsumer<T,V> getFieldSetter(Field f) {
		if(f==null)
			return null;
		return (BiConsumer<T,V>)fieldAccessors.computeIfAbsent(f, field->{
			java.lang.invoke.MethodHandles.Lookup lookup = ReflectionUtility.privateLookup(field.getDeclaringClass());
			try {
				MethodHandle setter = lookup.unreflectSetter(field);
				if(resolvedMetaFactory!=null && !field.getDeclaringClass().isSynthetic()) {
					try {
						CallSite site = resolvedMetaFactory.metafactory(lookup,
								"accept",
				                MethodType.methodType(BiConsumer.class),
				                MethodType.methodType(void.class, Object.class, Object.class),
				                setter,
				                wrap(setter.type()));
						return (BiConsumer<T,V>)site.getTarget().invokeExact();
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
		});
	}
}
