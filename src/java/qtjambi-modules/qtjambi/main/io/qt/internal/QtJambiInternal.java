/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
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

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.ObjectStreamConstants;
import java.io.Serializable;
import java.lang.Thread.UncaughtExceptionHandler;
import java.lang.invoke.MethodHandle;
import java.lang.invoke.MethodHandleInfo;
import java.lang.invoke.MethodHandles;
import java.lang.invoke.MethodType;
import java.lang.invoke.SerializedLambda;
import java.lang.ref.Reference;
import java.lang.ref.ReferenceQueue;
import java.lang.ref.WeakReference;
import java.lang.reflect.AnnotatedElement;
import java.lang.reflect.Array;
import java.lang.reflect.Constructor;
import java.lang.reflect.Field;
import java.lang.reflect.GenericArrayType;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.lang.reflect.Modifier;
import java.lang.reflect.Parameter;
import java.lang.reflect.ParameterizedType;
import java.lang.reflect.Proxy;
import java.lang.reflect.Type;
import java.lang.reflect.TypeVariable;
import java.lang.reflect.WildcardType;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.Collections;
import java.util.Comparator;
import java.util.Deque;
import java.util.HashMap;
import java.util.HashSet;
import java.util.IdentityHashMap;
import java.util.List;
import java.util.Map;
import java.util.NavigableMap;
import java.util.Queue;
import java.util.Set;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.function.BiConsumer;
import java.util.function.BiFunction;
import java.util.function.Function;
import java.util.function.IntFunction;
import java.util.function.Supplier;
import java.util.logging.LogRecord;

import io.qt.InternalAccess;
import io.qt.InternalAccess.Cleanable;
import io.qt.NativeAccess;
import io.qt.QFlags;
import io.qt.QMissingVirtualOverridingException;
import io.qt.QNoNativeResourcesException;
import io.qt.QNoSuchEnumValueException;
import io.qt.QNonVirtualOverridingException;
import io.qt.QtByteEnumerator;
import io.qt.QtDeclaredFinal;
import io.qt.QtEnumerator;
import io.qt.QtFinalOverride;
import io.qt.QtLongEnumerator;
import io.qt.QtMetaType;
import io.qt.QtObject;
import io.qt.QtObjectInterface;
import io.qt.QtPointerType;
import io.qt.QtReferenceType;
import io.qt.QtShortEnumerator;
import io.qt.QtSignalEmitterInterface;
import io.qt.QtUninvokable;
import io.qt.QtUtilities.LibraryRequirementMode;
import io.qt.core.QByteArray;
import io.qt.core.QDataStream;
import io.qt.core.QDebug;
import io.qt.core.QHash;
import io.qt.core.QList;
import io.qt.core.QMap;
import io.qt.core.QMetaMethod;
import io.qt.core.QMetaObject;
import io.qt.core.QMetaObject.Method3;
import io.qt.core.QMetaObject.Method4;
import io.qt.core.QMetaObject.Method5;
import io.qt.core.QMetaType;
import io.qt.core.QMultiHash;
import io.qt.core.QMultiMap;
import io.qt.core.QObject;
import io.qt.core.QPair;
import io.qt.core.QQueue;
import io.qt.core.QSet;
import io.qt.core.QStack;
import io.qt.core.QVariant;
import io.qt.core.Qt;
import io.qt.internal.QtJambiSignals.AbstractSignal;

public final class QtJambiInternal {

	private QtJambiInternal() {
		throw new RuntimeException();
	}
	
    private static native boolean needsReferenceCounting(long object);

	private static class RCList extends ArrayList<Object> {
		@NativeAccess
		public RCList() {
			super();
		}

		private static final long serialVersionUID = -4010060446825990721L;
		private final QMetaObject.Slot0 checkSlot = this::check;

		private synchronized void check() {
			List<Object> disposedElements = null;
			for (Object o : this) {
				if (o instanceof QtObjectInterface && ((QtObjectInterface) o).isDisposed()) {
					if (disposedElements == null) {
						disposedElements = Collections.singletonList(o);
					} else {
						if (disposedElements.size() == 1) {
							disposedElements = new ArrayList<>(disposedElements);
						}
						disposedElements.add(o);
					}
				}
			}
			if (disposedElements != null) {
				for (Object o : disposedElements) {
					super.remove(o);
				}
			}
		}

		@Override
		@NativeAccess
		public boolean add(Object e) {
			if(e instanceof QtObjectInterface) {
				if(!needsReferenceCounting(internalAccess.nativeId((QtObjectInterface)e)))
					return false;
				boolean result;
				synchronized (this) {
					result = super.add(e);
				}
				if (result) {
					QMetaObject.DisposedSignal disposed = getSignalOnDispose((QtObjectInterface) e, true);
					if (disposed != null)
						disposed.connect(checkSlot);
				}
				return result;
			}else {
				synchronized (this) {
					return super.add(e);
				}
			}
		}

		@Override
		@NativeAccess
		public boolean remove(Object o) {
			boolean result;
			synchronized (this) {
				result = super.remove(o);
			}
			if (result && o instanceof QtObjectInterface) {
				QMetaObject.DisposedSignal disposed = getSignalOnDispose((QtObjectInterface) o, true);
				if (disposed != null)
					disposed.disconnect(checkSlot);
			}
			return result;
		}

		@Override
		@NativeAccess
		public void clear() {
			List<Object> disposedElements;
			synchronized(this) {
				disposedElements = new ArrayList<>(this);
				super.clear();
			}
			for (Object o : disposedElements) {
				if (o instanceof QtObjectInterface) {
					QMetaObject.DisposedSignal disposed = getSignalOnDispose((QtObjectInterface) o, true);
					if (disposed != null)
						disposed.disconnect(checkSlot);
				}
			}
		}

		@Override
		@NativeAccess
		public boolean addAll(Collection<? extends Object> c) {
			boolean result = false;
			for (Object o : c) {
				result |= add(o);
			}
			return result;
		}
	}
	
	@NativeAccess
	private static class RCSet extends HashSet<Object> {
		@NativeAccess
		public RCSet() {
			super();
		}

		private static final long serialVersionUID = -4010060446825990721L;
		private final QMetaObject.Slot0 checkSlot = this::check;

		@SuppressWarnings("unused")
		private synchronized void check() {
			List<Object> disposedElements = null;
			for (Object o : this) {
				if (o instanceof QtObjectInterface && ((QtObjectInterface) o).isDisposed()) {
					if (disposedElements == null) {
						disposedElements = Collections.singletonList(o);
					} else {
						if (disposedElements.size() == 1) {
							disposedElements = new ArrayList<>(disposedElements);
						}
						disposedElements.add(o);
					}
				}
			}
			if (disposedElements != null) {
				for (Object o : disposedElements) {
					super.remove(o);
				}
			}
		}

		@Override
		@NativeAccess
		public boolean add(Object e) {
			if(e instanceof QtObjectInterface) {
				if(!needsReferenceCounting(internalAccess.nativeId((QtObjectInterface)e)))
					return false;
				boolean result;
				synchronized (this) {
					result = super.add(e);
				}
				if (result) {
					QMetaObject.DisposedSignal disposed = getSignalOnDispose((QtObjectInterface) e, true);
					if (disposed != null)
						disposed.connect(checkSlot);
				}
				return result;
			}else {
				synchronized (this) {
					return super.add(e);
				}
			}
		}

		@Override
		@NativeAccess
		public boolean remove(Object o) {
			boolean result;
			synchronized (this) {
				result = super.remove(o);
			}
			if (result && o instanceof QtObjectInterface) {
				QMetaObject.DisposedSignal disposed = getSignalOnDispose((QtObjectInterface) o, true);
				if (disposed != null)
					disposed.disconnect(checkSlot);
			}
			return result;
		}

		@Override
		@NativeAccess
		public void clear() {
			Set<Object> disposedElements;
			synchronized(this) {
				disposedElements = new HashSet<>(this);
				super.clear();
			}
			for (Object o : disposedElements) {
				if (o instanceof QtObjectInterface) {
					QMetaObject.DisposedSignal disposed = getSignalOnDispose((QtObjectInterface) o, true);
					if (disposed != null)
						disposed.disconnect(checkSlot);
				}
			}
		}

		@Override
		@NativeAccess
		public boolean addAll(Collection<? extends Object> c) {
			boolean result = false;
			for (Object o : c) {
				result |= add(o);
			}
			return result;
		}
	}
	
	private static class RCMap extends IdentityHashMap<Object, Object> {
		private static final long serialVersionUID = 3076251074218500284L;
		private final QMetaObject.Slot0 checkSlot = this::check;

		@NativeAccess
		public RCMap() {
			super();
		}
		
		@SuppressWarnings("unused")
		private synchronized void check() {
			List<Object> nulledKeys = null;
			List<Object> disposedKeys = null;
			for (Entry<Object, Object> entry : this.entrySet()) {
				if (entry.getValue() instanceof QtObjectInterface
						&& ((QtObjectInterface) entry.getValue()).isDisposed()) {
					if (entry.getKey() instanceof QtObjectInterface
							&& !((QtObjectInterface) entry.getKey()).isDisposed()) {
						if (nulledKeys == null) {
							nulledKeys = Collections.singletonList(entry.getKey());
						} else {
							if (nulledKeys.size() == 1) {
								nulledKeys = new ArrayList<>(nulledKeys);
							}
							nulledKeys.add(entry.getKey());
						}
					} else {
						if (disposedKeys == null) {
							disposedKeys = Collections.singletonList(entry.getKey());
						} else {
							if (disposedKeys.size() == 1) {
								disposedKeys = new ArrayList<>(disposedKeys);
							}
							disposedKeys.add(entry.getKey());
						}
					}
				}
			}
			if (disposedKeys != null) {
				for (Object key : disposedKeys) {
					super.remove(key);
				}
			}
			if (nulledKeys != null) {
				for (Object key : nulledKeys) {
					super.put(key, null);
				}
			}
		}

		@Override
		public Object put(Object key, Object value) {
			if (key instanceof QtObjectInterface || value instanceof QtObjectInterface) {
				if (key instanceof QtObjectInterface && value instanceof QtObjectInterface) {
					if(!needsReferenceCounting(internalAccess.nativeId((QtObjectInterface)key)) && !needsReferenceCounting(internalAccess.nativeId((QtObjectInterface)value)))
						return false;
				}else if (key instanceof QtObjectInterface) {
					if(!needsReferenceCounting(internalAccess.nativeId((QtObjectInterface)key)))
						return false;
				}else {
					if(!needsReferenceCounting(internalAccess.nativeId((QtObjectInterface)value)))
						return false;
				}
			}

			Object result;
			synchronized (this) {
				result = super.put(key, value);
			}
			if (key instanceof QtObjectInterface) {
				QMetaObject.DisposedSignal disposed = getSignalOnDispose((QtObjectInterface) key, true);
				if (disposed != null)
					disposed.connect(checkSlot);
			}
			if (value instanceof QtObjectInterface) {
				QMetaObject.DisposedSignal disposed = getSignalOnDispose((QtObjectInterface) value, true);
				if (disposed != null)
					disposed.connect(checkSlot);
			}
			if (result instanceof QtObjectInterface) {
				QMetaObject.DisposedSignal disposed = getSignalOnDispose((QtObjectInterface) result, true);
				if (disposed != null)
					disposed.disconnect(checkSlot);
			}
			return result;
		}

		@Override
		public Object remove(Object key) {
			Object result;
			synchronized (this) {
				result = super.remove(key);
			}
			if (key instanceof QtObjectInterface) {
				QMetaObject.DisposedSignal disposed = getSignalOnDispose((QtObjectInterface) key, true);
				if (disposed != null)
					disposed.disconnect(checkSlot);
			}
			if (result instanceof QtObjectInterface) {
				QMetaObject.DisposedSignal disposed = getSignalOnDispose((QtObjectInterface) result, true);
				if (disposed != null)
					disposed.disconnect(checkSlot);
			}
			return result;
		}

		@Override
		public void clear() {
			IdentityHashMap<Object, Object> _this;
			synchronized (this) {
				_this = new IdentityHashMap<>(this);
				super.clear();
			}
			for (Entry<Object, Object> entry : _this.entrySet()) {
				if (entry.getKey() instanceof QtObjectInterface) {
					QMetaObject.DisposedSignal disposed = getSignalOnDispose((QtObjectInterface) entry.getKey(), true);
					if (disposed != null)
						disposed.disconnect(checkSlot);
				}
				if (entry.getValue() instanceof QtObjectInterface) {
					QMetaObject.DisposedSignal disposed = getSignalOnDispose((QtObjectInterface) entry.getValue(), true);
					if (disposed != null)
						disposed.disconnect(checkSlot);
				}
			}
		}

		@Override
		public boolean remove(Object key, Object value) {
			boolean result;
			synchronized (this) {
				result = super.remove(key, value);
			}
			if (result) {
				if (key instanceof QtObjectInterface) {
					QMetaObject.DisposedSignal disposed = getSignalOnDispose((QtObjectInterface) key, true);
					if (disposed != null)
						disposed.disconnect(checkSlot);
				}
				if (value instanceof QtObjectInterface) {
					QMetaObject.DisposedSignal disposed = getSignalOnDispose((QtObjectInterface) value, true);
					if (disposed != null)
						disposed.disconnect(checkSlot);
				}
			}
			return result;
		}
	}

	private static final Map<NativeLink, QMetaObject.DisposedSignal> disposedSignals;
	private static Function<Object, QMetaObject.DisposedSignal> disposedSignalFactory;
	private static final Map<Integer, InterfaceNativeLink> interfaceLinks;
//    private static final Map<Class<? extends QtObjectInterface>, Function<QtObjectInterface, NativeLink>> nativeLinkSuppliers;
	private static final Map<Class<?>, Boolean> isClassGenerated;
	private static final Map<String, Boolean> initializedPackages;
	private static final Map<Class<?>, Function<Object,Object>> lambdaWriteReplaceHandles;
	
	private static final class ReadWriteHandles{
		private ReadWriteHandles(BiConsumer<Object,QDataStream> writeTo, BiConsumer<Object,QDataStream> readFrom, Supplier<?> constructor) {
			super();
			this.writeTo = writeTo;
			this.readFrom = readFrom;
			this.constructor = constructor;
		}
		final BiConsumer<Object,QDataStream> writeTo;
		final BiConsumer<Object,QDataStream> readFrom;
		final Supplier<?> constructor;
	}
	private static final Map<Class<?>, ReadWriteHandles> readWriteHandles;
	
	private static final class DataStreamFunctions{
		@SuppressWarnings("unchecked")
		private DataStreamFunctions(BiConsumer<QDataStream, ?> datastreamInFn,
				Function<QDataStream, ?> datastreamOutFn) {
			super();
			this.datastreamInFn = (java.util.function.BiConsumer<QDataStream, Object>)datastreamInFn;
			this.datastreamOutFn = (java.util.function.Function<QDataStream, Object>)datastreamOutFn;
		}
		final java.util.function.BiConsumer<QDataStream, Object> datastreamInFn;
		final java.util.function.Function<QDataStream, Object> datastreamOutFn;
	}
	private static final Map<Class<?>, DataStreamFunctions> dataStreamFunctions;
	private static final Map<Class<?>, java.util.function.BiConsumer<QDebug, Object>> debugStreamFunctions;
	
	private static final Map<QPair<Class<? extends QtObjectInterface>, Class<?>>, Check> checkedClasses;
	private static final Map<Class<?>, MethodHandle> lambdaSlotHandles;
	static final ReferenceQueue<Object> referenceQueue = new ReferenceQueue<>();
	private static final Thread cleanupRegistrationThread;
	
	public static final boolean useAnnotatedType;
	
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
		QtJambiSignals.SlotInvoker getSlotInvoker(Method slot, MethodHandle slotHandle);
	}
	
	private static final MethodInvocationHandler methodInvocationHandler;
	private static final Map<Class<?>, Function<Object,QtJambiIteratorObject<?>>> endMethodHandles;

	static {
		interfaceLinks = Collections.synchronizedMap(new HashMap<>());
		disposedSignals = Collections.synchronizedMap(new HashMap<>());
		isClassGenerated = Collections.synchronizedMap(new HashMap<>());
		initializedPackages = Collections.synchronizedMap(new HashMap<>());
		initializedPackages.put("io.qt.internal", Boolean.TRUE);
		lambdaWriteReplaceHandles = Collections.synchronizedMap(new HashMap<>());
		readWriteHandles = Collections.synchronizedMap(new HashMap<>());
		dataStreamFunctions = Collections.synchronizedMap(new HashMap<>());
		debugStreamFunctions = Collections.synchronizedMap(new HashMap<>());
		checkedClasses = Collections.synchronizedMap(new HashMap<>());
		lambdaSlotHandles = Collections.synchronizedMap(new HashMap<>());

		QtJambi_LibraryUtilities.initialize();
		cleanupRegistrationThread = new Thread(() -> {
			while (true) {
				if(Thread.interrupted())
					break;
				try {
					Reference<?> ref = referenceQueue.remove();
					try {
						if(Thread.interrupted())
							break;
						if (ref instanceof Cleanable) {
							((Cleanable) ref).clean();
							if(Thread.interrupted())
								break;
						}
					} finally {
						ref = null;
					}
				} catch (InterruptedException e) {
					break;
				} catch (Throwable e) {
					e.printStackTrace();
				}
				if(Thread.interrupted())
					break;
			}
		});
		cleanupRegistrationThread.setName("QtJambiCleanupThread");
		cleanupRegistrationThread.setDaemon(true);
		cleanupRegistrationThread.start();
		
		boolean _useAnnotatedType = false;
		try {
			java.lang.reflect.AnnotatedType.class.hashCode();
			_useAnnotatedType = true;
		}catch(Throwable t) {}
		useAnnotatedType = _useAnnotatedType;
		
		MethodInvocationHandler _methodInvoker = null;
		try {
			_methodInvoker = new QtJambiMethodHandles();
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
				public QtJambiSignals.SlotInvoker getSlotInvoker(Method slot, MethodHandle slotHandle){
					return null;
				}
			};
		}
		methodInvocationHandler = _methodInvoker;
		endMethodHandles = Collections.synchronizedMap(new HashMap<>());
	}
	
	static Function<Object,QtJambiIteratorObject<?>> findEndSupplier(Object beginOwner){
		if(beginOwner!=null){
			return endMethodHandles.computeIfAbsent(getClass(beginOwner), cls -> {
				Method endMethod = null;
				while (endMethod == null && cls!=null && cls != QtJambiObject.class) {
					Method methods[] = cls.getDeclaredMethods();
					for (Method method : methods) {
						if (method.getParameterCount() == 0 && method.getName().equals("end")
								&& QtJambiIteratorObject.class.isAssignableFrom(method.getReturnType())) {
							endMethod = method;
							break;
						}
					}
					cls = cls.getSuperclass();
				}
				if (endMethod != null)
					return functionFromMethod(endMethod);
				else return null;
			});
		}else {
			return null;
		}
	}

	@NativeAccess
	@QtUninvokable
	private static void terminateCleanupThread() throws Throwable {
		switch (cleanupRegistrationThread.getState()) {
		case TERMINATED:
			break;
		default:
			cleanupRegistrationThread.interrupt();
			cleanupRegistrationThread.join();
			break;
		}
	}

	private static final Map<Cleaner, Runnable> cleaners = new HashMap<>();

	private static class Cleaner extends WeakReference<Object> implements Cleanable {

		public Cleaner(Object r) {
			super(r, referenceQueue);
		}

		@Override
		public void clean() {
			synchronized (cleaners) {
				Runnable runnable = cleaners.remove(this);
				if (runnable != null) {
					try {
						runnable.run();
					} catch (Throwable e) {
						e.printStackTrace();
					}
				}
			}
		}
	}

	public static final char SlotPrefix = '1';
	public static final char SignalPrefix = '2';

	static Class<?> getComplexType(Class<?> primitiveType) {
		if (primitiveType == int.class)
			return Integer.class;
		else if (primitiveType == double.class)
			return Double.class;
		else if (primitiveType == long.class)
			return Long.class;
		else if (primitiveType == float.class)
			return Float.class;
		else if (primitiveType == short.class)
			return Short.class;
		else if (primitiveType == boolean.class)
			return Boolean.class;
		else if (primitiveType == char.class)
			return Character.class;
		else if (primitiveType == byte.class)
			return Byte.class;
		else
			return primitiveType;
	}
	
	static QtJambiSignals.SlotInvoker getSlotInvoker(Method slot, MethodHandle slotHandle){
		return methodInvocationHandler.getSlotInvoker(slot, slotHandle);
	}
	
	static <A,B> Function<A,B> functionFromMethod(Method method){
		return methodInvocationHandler.functionFromMethod(method);
	}
	
	@SuppressWarnings("unchecked")
	@NativeAccess
	private static List<Class<? extends QtObjectInterface>> getImplementedInterfaces(Class<?> cls) {
		if (cls == null) {
			return null;
		} else {
			initializePackage(cls);
			if (isGeneratedClass(cls) || cls.isInterface())
				return null;
			List<Class<? extends QtObjectInterface>> result = new ArrayList<>();
			Class<?> generatedSuperClass = findGeneratedSuperclass(cls);
			for (Class<?> _interface : cls.getInterfaces()) {
				initializePackage(_interface);
				if (isGeneratedClass(_interface) && QtObjectInterface.class.isAssignableFrom(_interface)) {
					Class<? extends QtObjectInterface> __interface = (Class<? extends QtObjectInterface>) _interface;
					Class<?> defaultImplementationClass = findDefaultImplementation(__interface);
					if (defaultImplementationClass != null && defaultImplementationClass.isAssignableFrom(cls)) {
						continue;
					}
					if (generatedSuperClass != null && __interface.isAssignableFrom(generatedSuperClass)) {
						continue;
					}
					if (!result.contains(__interface)) {
						result.add(0, __interface);
						initializePackage(__interface);
					}
				}
			}
			List<Class<? extends QtObjectInterface>> superInterfaces = getImplementedInterfaces(cls.getSuperclass());
			if (superInterfaces != null) {
				for (Class<? extends QtObjectInterface> _interface : superInterfaces) {
					if (!result.contains(_interface)) {
						result.add(0, _interface);
					}
				}
			}
			if (result.isEmpty() && !Proxy.isProxyClass(cls)) {
				result = null;
			} else {
				result = Collections.unmodifiableList(result);
			}
			return result;
		}
	}

	@SuppressWarnings("unchecked")
	@NativeAccess
	private static List<Class<? extends QtObjectInterface>> getAllImplementedInterfaces(Class<?> cls) {
		if (cls == null) {
			return null;
		} else {
			initializePackage(cls);
			if (cls.isInterface())
				return null;
			List<Class<? extends QtObjectInterface>> result = new ArrayList<>();
			for (Class<?> _interface : cls.getInterfaces()) {
				initializePackage(_interface);
				if (QtObjectInterface.class.isAssignableFrom(_interface)) {
					Class<? extends QtObjectInterface> __interface = (Class<? extends QtObjectInterface>) _interface;
					Class<?> defaultImplementationClass = findDefaultImplementation(__interface);
					if (defaultImplementationClass != null && defaultImplementationClass.isAssignableFrom(cls)) {
						continue;
					}
					if (!result.contains(__interface)) {
						result.add(0, __interface);
						initializePackage(__interface);
					}
				}
			}
			List<Class<? extends QtObjectInterface>> superInterfaces = getAllImplementedInterfaces(cls.getSuperclass());
			if (superInterfaces != null) {
				for (Class<? extends QtObjectInterface> _interface : superInterfaces) {
					if (!result.contains(_interface)) {
						result.add(0, _interface);
					}
				}
			}
			if (result.isEmpty()) {
				result = null;
			} else {
				result = Collections.unmodifiableList(result);
			}
			return result;
		}
	}

	@NativeAccess
	private static Method findQmlAttachedProperties(Class<?> cls) {
		try {
			Method qmlAttachedProperties = cls.getDeclaredMethod("qmlAttachedProperties", QObject.class);
			if (Modifier.isStatic(qmlAttachedProperties.getModifiers()))
				return qmlAttachedProperties;
		} catch (Throwable e) {
		}
		return null;
	}
	
	public static native long objectHash(Object object);

	static native java.lang.invoke.MethodHandles.Lookup privateLookup(Class<?> targetClass);

	public static SerializedLambda serializeLambdaExpression(Serializable slotObject) {
		Class<?> slotClass = QtJambiInternal.getClass(slotObject);
		if (slotClass.isSynthetic()) {
			Function<Object,Object> writeReplaceHandle = lambdaWriteReplaceHandles.computeIfAbsent(slotClass, cls -> {
				Method writeReplace = null;
				try {
					writeReplace = cls.getMethod("writeReplace");
				} catch (Throwable e) {}
				if(writeReplace == null) try {
					writeReplace = cls.getDeclaredMethod("writeReplace");
				} catch (Throwable e) {}
				return writeReplace==null ? null : QtJambiInternal.functionFromMethod(writeReplace);
			});
			if (writeReplaceHandle != null) {
				try {
					Object serializedResult = writeReplaceHandle.apply(slotObject);
					if (serializedResult instanceof SerializedLambda) {
						return (SerializedLambda) serializedResult;
					}
				} catch (Throwable e) {
					java.util.logging.Logger.getLogger("io.qt.internal").log(java.util.logging.Level.WARNING,
							"Exception caught while analyzing lambda expression", e);
				}
			}
		}
		return null;
	}

	public static MethodHandle getConstructorHandle(Constructor<?> constructor) throws IllegalAccessException {
		java.lang.invoke.MethodHandles.Lookup lookup = QtJambiInternal.privateLookup(constructor.getDeclaringClass());
		return lookup.unreflectConstructor(constructor);
	}

	public static MethodHandle getMethodHandle(Method method) throws IllegalAccessException {
		java.lang.invoke.MethodHandles.Lookup lookup = QtJambiInternal.privateLookup(method.getDeclaringClass());
		return lookup.unreflect(method);
	}

	static MethodHandle getFieldGetterHandle(Field field) throws IllegalAccessException {
		java.lang.invoke.MethodHandles.Lookup lookup = QtJambiInternal.privateLookup(field.getDeclaringClass());
		return lookup.unreflectGetter(field);
	}
	
	public static <T> Function<Object[],T> getFactory(Constructor<T> constructor) {
		return methodInvocationHandler.getFactory(constructor);
	}
	
	public static <T> Supplier<T> getFactory0(Constructor<T> constructor) {
		return methodInvocationHandler.getFactory0(constructor);
	}
	
	public static <A,T> Function<A,T> getFactory1(Constructor<T> constructor) {
		return methodInvocationHandler.getFactory1(constructor);
	}
	
	public static <A,B,T> BiFunction<A,B,T> getFactory2(Constructor<T> constructor) {
		return methodInvocationHandler.getFactory2(constructor);
	}
	
	public static <A,B,C,T> QMetaObject.Method3<A,B,C,T> getFactory3(Constructor<T> constructor) {
		return methodInvocationHandler.getFactory3(constructor);
	}
	
	public static <A,B,C,D,T> QMetaObject.Method4<A,B,C,D,T> getFactory4(Constructor<T> constructor) {
		return methodInvocationHandler.getFactory4(constructor);
	}
	
	public static <A,B,C,D,E,T> QMetaObject.Method5<A,B,C,D,E,T> getFactory5(Constructor<T> constructor) {
		return methodInvocationHandler.getFactory5(constructor);
	}

	public static <T> T invokeContructor(Constructor<T> constructor, Object... args) throws Throwable {
		return methodInvocationHandler.invokeContructor(constructor, args);
	}

	public static Object invokeMethod(Method method, Object object, Object... args) throws Throwable {
		return methodInvocationHandler.invokeMethod(method, object, args);
	}
	
	public static Object invokeInterfaceDefaultMethod(Method method, Object object, Object... args) throws Throwable {
		return methodInvocationHandler.invokeInterfaceDefaultMethod(method, object, args);
	}

	static native Object invokeMethod(Object receiver, Method method, boolean isStatic, byte returnType, Object args[], byte slotTypes[]);

	/**
	 * Searches the object's class and its superclasses for a method of the given
	 * name and returns its signature.
	 */
	static private HashMap<String, String> signalMethodSignatureCash = new HashMap<String, String>();

	static String findSignalMethodSignature(QtSignalEmitterInterface signalEmitter, String name)
			throws NoSuchFieldException, IllegalAccessException {

		Class<?> cls = QtJambiInternal.getClass(signalEmitter);
		String fullName = cls + "." + name;
		String found = signalMethodSignatureCash.get(fullName);

		if (found != null) {
			return found;
		}

		while (cls != null) {
			Method methods[] = cls.getDeclaredMethods();
			for (Method method : methods) {
				if (method.getName().equals(name)) {
					found = name + "(";

					Class<?> params[] = method.getParameterTypes();
					for (int j = 0; j < params.length; ++j) {
						if (j > 0) {
							found += ",";
						}
						found += params[j].getName();
					}
					found = found + ")";
					break;
				}
			}

			cls = cls.getSuperclass();
		}
		signalMethodSignatureCash.put(fullName, found);
		return found;
	}

	private native static <E extends Enum<E> & QtEnumerator> E resolveIntEnum(int hashCode, Class<E> cl, int value,
			String name) throws Throwable;

	private native static <E extends Enum<E> & QtByteEnumerator> E resolveByteEnum(int hashCode, Class<E> cl,
			byte value, String name) throws Throwable;

	private native static <E extends Enum<E> & QtShortEnumerator> E resolveShortEnum(int hashCode, Class<E> cl,
			short value, String name) throws Throwable;

	private native static <E extends Enum<E> & QtLongEnumerator> E resolveLongEnum(int hashCode, Class<E> cl,
			long value, String name) throws Throwable;

	public static void setField(Object owner, Class<?> declaringClass, String fieldName, Object newValue) {
		try {
			Field f = declaringClass.getDeclaredField(fieldName);
			setField(owner, f, newValue);
		} catch (RuntimeException e) {
			throw e;
		} catch (Throwable e) {
			throw new RuntimeException("Cannot set field '" + fieldName + "'", e);
		}
	}

	static void setField(Object owner, Field f, Object newValue) {
		try {
			methodInvocationHandler.setField(owner, f, newValue);
		} catch (Throwable e) {
			if (Modifier.isStatic(f.getModifiers())) {
				if (!setFieldNative(f.getDeclaringClass(), f, true, newValue)) {
					throw new RuntimeException("Cannot set field '" + f.getName() + "'", e);
				}
			}else if(owner!=null){
				if (!setFieldNative(owner, f, false, newValue)) {
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

	private static native boolean setFieldNative(Object owner, Field field, boolean isStatic, Object newValue);
	
	private static native boolean setFieldByName(Object owner, String field, String signature, boolean isStatic, Object newValue);

	@SuppressWarnings("unchecked")
	public static <V> V fetchField(Object owner, Class<?> declaringClass, String fieldName, Class<V> fieldType) {
		if(declaringClass==null)
			declaringClass = QtJambiInternal.getClass(owner);
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
				return (V) fetchField(owner,field);
		} catch (Throwable e) {
		}
		throw new RuntimeException("Cannot fetch field '" + fieldName + "'");
	}

	public static Object fetchField(Object owner, Field f) {
		try {
			return methodInvocationHandler.fetchField(owner, f);
		} catch (Throwable e) {
			if (Modifier.isStatic(f.getModifiers())) {
				return fetchFieldNative(f.getDeclaringClass(), f, true);
			}else {
				return fetchFieldNative(owner, f, false);
			}
		}
	}
	
	public static native Map<String,QPair<Long,Long>> getRegisteredTypeSizesAndAlignments();
	
	private static native Object fetchFieldNative(Object owner, Field field, boolean isStatic);

	/**
	 * Returns wether a class is an actual implementor of a function or if the
	 * function is simply a shell around a native implementation provided by default
	 * by the Qt Jambi bindings.
	 *
	 * @param method The function to match.
	 * @return wether the implements the function or not.
	 */
	@NativeAccess
	private static boolean isImplementedInJava(boolean isAbstract, Method method, Class<?> expectedClass) {
		Class<?> declaringClass = method.getDeclaringClass();
		if (expectedClass.isInterface() && !declaringClass.isInterface()
				&& !expectedClass.isAssignableFrom(declaringClass)) {
			return true;
		}
		if (!expectedClass.isInterface() && declaringClass.isInterface() && isAbstract && method.isDefault()
				&& !declaringClass.isAssignableFrom(expectedClass)) {
			return true;
		}
		return !isGeneratedClass(declaringClass);
	}

	/**
	 * Returns the closest superclass of <code>obj's</code> class which is a
	 * generated class, or null if no such class is found.
	 */
	@NativeAccess
	public static Class<?> findGeneratedSuperclass(Class<?> clazz) {
		while (clazz != null && !isGeneratedClass(clazz)) {
			clazz = clazz.getSuperclass();
		}
		return clazz;
	}
	
	private static final int RWH_MAGIC = 0x01010101;
	private static final int NULL_MAGIC = 0x02020202;
	private static final int DSF_MAGIC = 0x03030303;
	private static final int BYTE_ARRAY_MAGIC = 0x04040400;
	private static final int SHORT_ARRAY_MAGIC = 0x04040401;
	private static final int INT_ARRAY_MAGIC = 0x04040402;
	private static final int LONG_ARRAY_MAGIC = 0x04040403;
	private static final int FLOAT_ARRAY_MAGIC = 0x04040404;
	private static final int DOUBLE_ARRAY_MAGIC = 0x04040405;
	private static final int BOOLEAN_ARRAY_MAGIC = 0x04040406;
	private static final int CHAR_ARRAY_MAGIC = 0x04040407;
	private static final int OBJECT_ARRAY_MAGIC = 0x04040408;

	private static ReadWriteHandles getReadWriteHandles(Class<?> _cls) {
		return readWriteHandles.computeIfAbsent(_cls, cls ->{
			try {
				Constructor<?> constructor = cls.getDeclaredConstructor();
				Method writeTo;
				try {
					writeTo = cls.getMethod("writeTo", QDataStream.class);
				} catch (Throwable e) {
					writeTo = cls.getDeclaredMethod("writeTo", QDataStream.class);
				}
				Method readFrom;
				try {
					readFrom = cls.getMethod("readFrom", QDataStream.class);
				} catch (Throwable e) {
					readFrom = cls.getDeclaredMethod("readFrom", QDataStream.class);
				}
				if(!Modifier.isStatic(writeTo.getModifiers()) && !Modifier.isStatic(readFrom.getModifiers())) {
					return new ReadWriteHandles(methodInvocationHandler.streamIO(writeTo), methodInvocationHandler.streamIO(readFrom), methodInvocationHandler.getFactory0(constructor));
				}
			} catch (Throwable e) {
			}
			return null;
		});
	}

	@NativeAccess
	private static Object readSerializableJavaObject(final QDataStream s) throws ClassNotFoundException, IOException {
		Object res = null;
		InputStream is = new InputStream() {
			@Override
			public int read() throws IOException {
				return s.readByte();
			}
		};
		boolean isSerialized = true;
		s.startTransaction();
		{
			int ch1 = is.read();
	        int ch2 = is.read();
			int ch3 = is.read();
	        int ch4 = is.read();
	        short magic = (short)((ch1 << 8) + (ch2 & 0xFF));
	        short version = (short)((ch3 << 8) + (ch4 & 0xFF));
			if(magic!=ObjectStreamConstants.STREAM_MAGIC
					|| version!=ObjectStreamConstants.STREAM_VERSION) {
				isSerialized = false;
			}
		}
		s.rollbackTransaction();
		
		if(isSerialized) {
			try (ObjectInputStream in = new ObjectInputStream(is)) {
				res = in.readObject();
			}
		}else {
			Class<?> cls;
			int length;
			switch(s.readInt()) {
			case BYTE_ARRAY_MAGIC:
				length = s.readInt();
				{
					byte[] array = new byte[length];
					for (int i = 0; i < array.length; i++) {
						array[i] = s.readByte();
					}
					res = array;
				}
				break;
			case SHORT_ARRAY_MAGIC:
				length = s.readInt();
				{
					short[] array = new short[length];
					for (int i = 0; i < array.length; i++) {
						array[i] = s.readShort();
					}
					res = array;
				}
				break;
			case INT_ARRAY_MAGIC:
				length = s.readInt();
				{
					int[] array = new int[length];
					for (int i = 0; i < array.length; i++) {
						array[i] = s.readInt();
					}
					res = array;
				}
				break;
			case LONG_ARRAY_MAGIC:
				length = s.readInt();
				{
					long[] array = new long[length];
					for (int i = 0; i < array.length; i++) {
						array[i] = s.readLong();
					}
					res = array;
				}
				break;
			case FLOAT_ARRAY_MAGIC:
				length = s.readInt();
				{
					float[] array = new float[length];
					for (int i = 0; i < array.length; i++) {
						array[i] = s.readFloat();
					}
					res = array;
				}
				break;
			case DOUBLE_ARRAY_MAGIC:
				length = s.readInt();
				{
					double[] array = new double[length];
					for (int i = 0; i < array.length; i++) {
						array[i] = s.readDouble();
					}
					res = array;
				}
				break;
			case BOOLEAN_ARRAY_MAGIC:
				length = s.readInt();
				{
					boolean[] array = new boolean[length];
					for (int i = 0; i < array.length; i++) {
						array[i] = s.readBoolean();
					}
					res = array;
				}
				break;
			case CHAR_ARRAY_MAGIC:
				length = s.readInt();
				{
					char[] array = new char[length];
					for (int i = 0; i < array.length; i++) {
						array[i] = s.readChar();
					}
					res = array;
				}
				break;
			case OBJECT_ARRAY_MAGIC:
				length = s.readInt();
				{
					cls = Class.forName(s.readString());
					res = Array.newInstance(cls, length);
					for (int i = 0; i < length; i++) {
						Array.set(res, i, QVariant.loadObject(s));
					}
				}
				break;
			case RWH_MAGIC:
				cls = Class.forName(s.readString());
				ReadWriteHandles readWriteHandles = getReadWriteHandles(cls);
				if(readWriteHandles!=null) {
					try {
						res = readWriteHandles.constructor.get();
						readWriteHandles.readFrom.accept(res, s);
					} catch (Error | Exception e) {
						throw e;
					} catch (Throwable e) {
						throw new IOException(e);
					}
				}
				break;
			case DSF_MAGIC:
				cls = Class.forName(s.readString());
				DataStreamFunctions functions = dataStreamFunctions.get(cls);
				if(functions!=null) {
					try {
						res = functions.datastreamOutFn.apply(s);
					} catch (RuntimeException | Error e) {
						throw e;
					} catch (Throwable e) {
						throw new IOException(e);
					}
				}
				break;
			case NULL_MAGIC:
				break;
			default:
				break;
			}
		}
		return res;
	}
	
	@NativeAccess
	private static void writeSerializableJavaObject(QDataStream s, Object o) throws IOException {
		if(o==null) {
			s.writeInt(NULL_MAGIC);
			return;
		}
		Class<?> objectClass = QtJambiInternal.getClass(o);
		if(objectClass.isArray()) {
			if(objectClass==byte[].class) {
				byte[] array = (byte[])o;
				s.writeInt(BYTE_ARRAY_MAGIC);
				s.writeInt(array.length);
				for (int i = 0; i < array.length; i++) {
					s.writeByte(array[i]);
				}
			}else if(objectClass==short[].class) {
				short[] array = (short[])o;
				s.writeInt(SHORT_ARRAY_MAGIC);
				s.writeInt(array.length);
				for (int i = 0; i < array.length; i++) {
					s.writeShort(array[i]);
				}
			}else if(objectClass==int[].class) {
				int[] array = (int[])o;
				s.writeInt(INT_ARRAY_MAGIC);
				s.writeInt(array.length);
				for (int i = 0; i < array.length; i++) {
					s.writeInt(array[i]);
				}
			}else if(objectClass==long[].class) {
				long[] array = (long[])o;
				s.writeInt(LONG_ARRAY_MAGIC);
				s.writeInt(array.length);
				for (int i = 0; i < array.length; i++) {
					s.writeLong(array[i]);
				}
			}else if(objectClass==float[].class) {
				float[] array = (float[])o;
				s.writeInt(FLOAT_ARRAY_MAGIC);
				s.writeInt(array.length);
				for (int i = 0; i < array.length; i++) {
					s.writeFloat(array[i]);
				}
			}else if(objectClass==double[].class) {
				double[] array = (double[])o;
				s.writeInt(DOUBLE_ARRAY_MAGIC);
				s.writeInt(array.length);
				for (int i = 0; i < array.length; i++) {
					s.writeDouble(array[i]);
				}
			}else if(objectClass==boolean[].class) {
				boolean[] array = (boolean[])o;
				s.writeInt(BOOLEAN_ARRAY_MAGIC);
				s.writeInt(array.length);
				for (int i = 0; i < array.length; i++) {
					s.writeBoolean(array[i]);
				}
			}else if(objectClass==char[].class) {
				char[] array = (char[])o;
				s.writeInt(CHAR_ARRAY_MAGIC);
				s.writeInt(array.length);
				for (int i = 0; i < array.length; i++) {
					s.writeChar(array[i]);
				}
			}else {
				Object[] array = (Object[])o;
				s.writeInt(OBJECT_ARRAY_MAGIC);
				s.writeString(objectClass.getComponentType().getName());
				s.writeInt(array.length);
				for (int i = 0; i < array.length; i++) {
					QVariant.saveObject(s, array[i]);
				}
			}
		}else {
			DataStreamFunctions functions = dataStreamFunctions.get(objectClass);
			if(functions!=null) {
				try {
					s.writeInt(DSF_MAGIC);
					s.writeString(objectClass.getName());
					functions.datastreamInFn.accept(s, o);
				} catch (RuntimeException | Error e) {
					throw e;
				} catch (Throwable e) {
					throw new IOException(e);
				}
			}else {
				ReadWriteHandles readWriteHandles = getReadWriteHandles(objectClass);
				if(readWriteHandles!=null) {
					try {
						s.writeInt(RWH_MAGIC);
						s.writeString(objectClass.getName());
						readWriteHandles.writeTo.accept(o, s);
					} catch (Error | Exception e) {
						throw e;
					} catch (Throwable e) {
						throw new IOException(e);
					}
				}else {
					ByteArrayOutputStream bos = new ByteArrayOutputStream();
					try(ObjectOutputStream out = new ObjectOutputStream(bos)) {
						out.writeObject(o);
					}
					s.writeBytes(bos.toByteArray());
				}
			}
		}
	}
	
	@NativeAccess
	private static void debugObject(QDebug s, Object o) {
		if(o==null) {
			s.append("null");
		}else{
			Class<?> objectClass = QtJambiInternal.getClass(o);
			if(objectClass.isArray()) {
				if(objectClass==byte[].class) {
					byte[] array = (byte[])o;
					s.append(Arrays.toString(array));
				}else if(objectClass==short[].class) {
					short[] array = (short[])o;
					s.append(Arrays.toString(array));
				}else if(objectClass==int[].class) {
					int[] array = (int[])o;
					s.append(Arrays.toString(array));
				}else if(objectClass==long[].class) {
					long[] array = (long[])o;
					s.append(Arrays.toString(array));
				}else if(objectClass==float[].class) {
					float[] array = (float[])o;
					s.append(Arrays.toString(array));
				}else if(objectClass==double[].class) {
					double[] array = (double[])o;
					s.append(Arrays.toString(array));
				}else if(objectClass==boolean[].class) {
					boolean[] array = (boolean[])o;
					s.append(Arrays.toString(array));
				}else if(objectClass==char[].class) {
					char[] array = (char[])o;
					s.append(Arrays.toString(array));
				}else {
					Object[] array = (Object[])o;
					s.append(Arrays.toString(array));
				}
			}else {
				java.util.function.BiConsumer<QDebug,Object> function = debugStreamFunctions.get(objectClass);
				if(function!=null) {
					try {
						function.accept(s, o);
					} catch (RuntimeException | Error e) {
						throw e;
					} catch (Throwable e) {
						throw new RuntimeException(e);
					}
				}else {
					s.append(o.toString());
				}
			}
		}
	}
	
	static native String libraryFilePath();
	
	public static <T> void registerDataStreamOperators(int metaType, Class<?> classType, java.util.function.BiConsumer<QDataStream, T> datastreamInFn, java.util.function.Function<QDataStream, T> datastreamOutFn) {
		if(!isObjectWrapperType(metaType)) {
			if(classType.isArray())
				throw new IllegalArgumentException(String.format("Unable to register datastream operators for type %1$s.", classType.getTypeName()));
			else
				throw new IllegalArgumentException(String.format("Unable to register datastream operators for type %1$s (%2$s).", classType.getTypeName(), new QMetaType(metaType).name()));
		}
		DataStreamFunctions functions = dataStreamFunctions.computeIfAbsent(classType, cls->new DataStreamFunctions(datastreamInFn, datastreamOutFn));
		if(functions.datastreamInFn!=datastreamInFn || functions.datastreamOutFn!=datastreamOutFn)
			throw new RuntimeException(String.format("Datastream operators already exist for type %1$s.", classType.getTypeName()));
	}
	
	@SuppressWarnings("unchecked")
	public static <T> void registerDebugStreamOperator(int metaType, Class<?> classType, java.util.function.BiConsumer<QDebug, T> debugstreamFn) {
		if(!isObjectWrapperType(metaType)) {
			if(classType.isArray())
				throw new IllegalArgumentException(String.format("Unable to register debug stream operator for type %1$s.", classType.getTypeName()));
			else
				throw new IllegalArgumentException(String.format("Unable to register debug stream operator for type %1$s (%2$s).", classType.getTypeName(), classType.getTypeName()));
		}
		java.util.function.BiConsumer<QDebug,Object> fun = debugStreamFunctions.computeIfAbsent(classType, c->(java.util.function.BiConsumer<QDebug,Object>)debugstreamFn);
		if(fun!=debugstreamFn)
			throw new RuntimeException(String.format("Debug stream operator already exists for type %1$s.", classType.getTypeName()));
	}
	
	private static native boolean isObjectWrapperType(int metaType);

	@NativeAccess
	static boolean isGeneratedClass(Class<?> clazz) {
		return isClassGenerated.computeIfAbsent(clazz, cls -> {
			initializePackage(cls);
			if (QtObjectInterface.class.isAssignableFrom(cls)) {
				if (isGeneratedClass(cls.getName())) {
					return true;
				} else if (cls.getSimpleName().equals("ConcreteWrapper") && cls.getEnclosingClass() != null) {
					return isGeneratedClass(cls.getEnclosingClass().getName());
				} else if (cls.getSimpleName().equals("Impl") && cls.getEnclosingClass() != null) {
					return isGeneratedClass(cls.getEnclosingClass().getName());
				}
			}
			return false;
		});
	}

	private static native boolean isGeneratedClass(String className);

	@NativeAccess
	private static boolean putMultiMap(Map<Object, List<Object>> map, Object key, Object value) {
		map.computeIfAbsent(key, k -> new ArrayList<>()).add(value);
		return true;
	}

	@NativeAccess
	private static Comparator<Object> createComparator(long compareFunction) {
		return (Object k1, Object k2) -> {
			if (lessThan(compareFunction, k1, k2)) {
				return -1;
			} else if (lessThan(compareFunction, k2, k1)) {
				return 1;
			} else {
				return 0;
			}
		};
	}

	private static native boolean lessThan(long compareFunction, Object k1, Object k2);

	public static Monitor synchronizedNativeId(QtObjectInterface object) {
		return new Monitor(findInterfaceLink(object, true));
	}

	private static native boolean monitorEnter(Object object);

	private static native boolean monitorExit(Object object);

	public static class Monitor implements AutoCloseable {

		public Monitor(Object object) {
			super();
			if (object != null && monitorEnter(object)) {
				this.object = object;
			} else {
				this.object = null;
			}
		}

		private final Object object;

		@Override
		public void close() throws Exception {
			if (object != null)
				monitorExit(object);
		}

	}

	public enum Ownership implements QtByteEnumerator {
		Invalid(0), 
		Java(0x001), // Weak ref to java object, deleteNativeObject deletes c++ object
		Cpp(0x002), // Strong ref to java object until c++ object is deleted, deleteNativeObject
					// does *not* delete c++ obj.
		Split(0x004); // Weak ref to java object, deleteNativeObject does *not* delete c++ object.
						// Only for objects not created by Java.
		private Ownership(int value) {
			this.value = (byte) value;
		}

		private final byte value;

		@Override
		public byte value() {
			return value;
		}

		public static Ownership resolve(byte value) {
			switch (value) {
			case 0x001:
				return Java;
			case 0x002:
				return Cpp;
			case 0x004:
				return Split;
			default:
				return Invalid;
			}
		}
	};

	public static Ownership ownership(QtObject object) {
		try {
			return Ownership.resolve(ownership(internalAccess.nativeId(object)));
		} catch (QNoSuchEnumValueException e) {
			return Ownership.Invalid;
		}
	}

	public static Ownership ownership(QtObjectInterface object) {
		try {
			return Ownership.resolve(ownership(internalAccess.nativeId(object)));
		} catch (QNoSuchEnumValueException e) {
			return Ownership.Invalid;
		}
	}

	private static native void invalidateObject(long native__id);

	private static native void setCppOwnership(long native__id);

	private static native void setDefaultOwnership(long native__id);

	private static native void setJavaOwnership(long native__id);

	private static native byte ownership(long native__id);
	
	private static native QObject owner(long native__id);

	private static native boolean hasOwnerFunction(long native__id);

	/**
	 * Emitted either as the native resources that belong to the object are being
	 * cleaned up or directly before the object is finalized. Connect to this signal
	 * to do clean up when the object is destroyed. The signal will never be emitted
	 * more than once per object, and the object is guaranteed to be unusable after
	 * this signal has returned.
	 */
	public static QMetaObject.DisposedSignal getSignalOnDispose(QtObjectInterface object, boolean forceCreation) {
		return getSignalOnDispose(findInterfaceLink(object, forceCreation), forceCreation);
	}
	public static QMetaObject.DisposedSignal getSignalOnDispose(QtJambiObject object, boolean forceCreation) {
		return getSignalOnDispose(findInterfaceLink(object, forceCreation), forceCreation);
	}

	private static QMetaObject.DisposedSignal getSignalOnDispose(NativeLink nativeLink, boolean forceCreation) {
		if (nativeLink != null) {
			if (forceCreation) {
				boolean isDisposed;
				synchronized (nativeLink) {
					isDisposed = nativeLink.native__id == 0;
					if(!isDisposed)
						NativeLink.setHasDisposedSignal(nativeLink.native__id);
				}
				if (!isDisposed) {
					try {
						return disposedSignals.computeIfAbsent(nativeLink, lnk -> {
							QtObjectInterface object = lnk.get();
							if(disposedSignalFactory==null)
								disposedSignalFactory = QtJambiSignals.<QMetaObject.DisposedSignal>getSignalFactory(QMetaObject.DisposedSignal.class);
							return disposedSignalFactory.apply(QtJambiInternal.getClass(object));
						});
					} catch (NullPointerException e) {
					}
				}
			} else
				return disposedSignals.get(nativeLink);
		}
		return null;
	}

	private static QMetaObject.DisposedSignal takeSignalOnDispose(NativeLink nativeLink) {
		return disposedSignals.remove(nativeLink);
	}

	@SuppressWarnings("unlikely-arg-type")
	public static QMetaObject.AbstractSignal findSignal(QObject sender, QMetaMethod method) {
		int methodIndex = method.methodIndex();
		Class<?> cls = method.enclosingMetaObject().type();
		if(cls==null)
			cls = getClass(sender);
		if(cls!=null) {
			while(cls!=null && cls!=Object.class) {
				try {
					Field f = cls.getDeclaredField(method.name().toString());
					if (!java.lang.reflect.Modifier.isStatic(f.getModifiers())) {
						if (AbstractSignal.class.isAssignableFrom(f.getType())) {
							try {
								AbstractSignal signal = (AbstractSignal) fetchField(sender, f);
								if (signal != null) {
									if(signal.methodIndex()==methodIndex)
										return (QMetaObject.AbstractSignal) signal;
									if(method.name().equals(signal.name()) && signal.argumentTypes().equals(method.parameterClassTypes())) {
										return (QMetaObject.AbstractSignal) signal;
									}
								}
							}catch(Throwable t) {}
						} else if (QtJambiSignals.AbstractMultiSignal.class.isAssignableFrom(f.getType())) {
							try {
								QtJambiSignals.AbstractMultiSignal<?> multiSignal = (QtJambiSignals.AbstractMultiSignal<?>) fetchField(sender, f);
								for (AbstractSignal signal : multiSignal.signals()) {
									if (signal.methodIndex()==methodIndex) {
										return (QMetaObject.AbstractSignal) signal;
									}
								}
							}catch(Throwable t) {}
						}
					}
				}catch(Throwable t) {}
				cls = cls.getSuperclass();
			}
		}
		return getExtraSignal(internalAccess.nativeId(sender), internalAccess.nativeId(method));
	}
	
	native static QMetaObject.AbstractSignal getExtraSignal(long sender__id, long method__id);

	private static interface Check {
		void check() throws Exception;
	}

	private final static class Throwing implements Check {
		public Throwing(Exception exn) {
			super();
			this.exn = exn;
		}

		public void check() throws Exception {
			StackTraceElement[] st = Thread.currentThread().getStackTrace();
			st = Arrays.copyOfRange(st, 2, st.length - 2);
			exn.setStackTrace(st);
			throw exn;
		}

		private final Exception exn;
	}

	private static void checkImplementation(Class<? extends QtObjectInterface> originalType, Class<?> subType) throws Exception {
		QPair<Class<? extends QtObjectInterface>, Class<?>> _pair = new QPair<>(originalType, subType);
		checkedClasses.computeIfAbsent(_pair, pair -> {
			Class<?> cls = pair.first;
			if(pair.first.isInterface()) {
				cls = findDefaultImplementation(pair.first);
				if (cls == null) {
					return new Throwing(new ClassNotFoundException("Implementation of " + pair.first.getName()));
				}
			}
			List<Method> virtualProtectedMethods = new ArrayList<>();
			List<Method> finalMethods = new ArrayList<>();
			if(pair.first.isInterface()) {
				while (cls != null) {
					for (Method method : cls.getDeclaredMethods()) {
						int mod = method.getModifiers();
						if (!Modifier.isStatic(mod)) {
							boolean isFinal = method.isAnnotationPresent(QtDeclaredFinal.class) || Modifier.isFinal(mod);
							if (!isFinal && Modifier.isProtected(mod) && Modifier.isAbstract(mod) && !virtualProtectedMethods.contains(method)) {
								virtualProtectedMethods.add(method);
							} else if (isFinal && !Modifier.isPrivate(mod) && !finalMethods.contains(method)) {
								finalMethods.add(method);
							}
						}
					}
					if (cls == QtObject.class)
						break;
					cls = cls.getSuperclass();
				}
			}else if(pair.first!=pair.second && !isGeneratedClass(pair.second)){
				while (cls != null) {
					for (Method method : cls.getDeclaredMethods()) {
						int mod = method.getModifiers();
						if (!Modifier.isStatic(mod)) {
							boolean isFinal = method.isAnnotationPresent(QtDeclaredFinal.class);
							if (isFinal && !finalMethods.contains(method)) {
								finalMethods.add(method);
							}
						}
					}
					cls = cls.getSuperclass();
					if (cls == QtObject.class)
						break;
				}
			}
			if(!virtualProtectedMethods.isEmpty() || !finalMethods.isEmpty()) {
				List<Method> missingMethods = new ArrayList<>();
				List<Method> nonFinalMethods = new ArrayList<>();
				List<Method> nonOverridableMethods = new ArrayList<>();
				for (Method method : virtualProtectedMethods) {
					if (findAccessibleMethod(method, pair.second) == null) {
						missingMethods.add(method);
					}
				}
				for (Method method : finalMethods) {
					Method implMethod = findAccessibleMethod(method, pair.second);
					if (Modifier.isProtected(method.getModifiers())) {
						if (implMethod != null && implMethod.getDeclaringClass() != pair.first
								&& implMethod.getDeclaringClass() != QtObjectInterface.class
								&& !Modifier.isFinal(implMethod.getModifiers())
								&& !nonFinalMethods.contains(implMethod)) {
							nonFinalMethods.add(implMethod);
						}
					} else if (implMethod != null) {
						if (implMethod.getDeclaringClass() != pair.first
								&& implMethod.getDeclaringClass() != QtObject.class
								&& !isGeneratedClass(implMethod.getDeclaringClass())
								&& !implMethod.isAnnotationPresent(QtFinalOverride.class)
								&& !nonOverridableMethods.contains(implMethod)) {
							nonOverridableMethods.add(implMethod);
						}
					}
				}
				if (missingMethods.size() == 1) {
					Method method = missingMethods.get(0);
					StringBuilder builder = new StringBuilder();
					if (Modifier.isPublic(method.getModifiers())) {
						builder.append("public ");
					} else if (Modifier.isProtected(method.getModifiers())) {
						builder.append("protected ");
					}
					builder.append(method.getReturnType().getName().replace('$', '.'));
					builder.append(' ');
					builder.append(method.getName());
					builder.append('(');
					Parameter[] ptypes = method.getParameters();
					for (int i = 0; i < ptypes.length; i++) {
						if (i > 0)
							builder.append(',');
						builder.append(ptypes[i].getType().getName().replace('$', '.'));
						builder.append(' ');
						builder.append(ptypes[i].getName());
					}
					builder.append(')');
					return new Throwing(new QMissingVirtualOverridingException(String.format(
							"Cannot convert %2$s to %1$s because class is required to implement virtual method: %3$s",
							pair.first.getSimpleName(), pair.second.getName(), builder)));
				} else if (missingMethods.size() > 1) {
					StringBuilder builder = new StringBuilder();
					for (Method method : missingMethods) {
						if (builder.length() != 0) {
							builder.append(',');
							builder.append(' ');
						}
						if (Modifier.isPublic(method.getModifiers())) {
							builder.append("public ");
						} else if (Modifier.isProtected(method.getModifiers())) {
							builder.append("protected ");
						}
						builder.append(method.getReturnType().getName().replace('$', '.'));
						builder.append(' ');
						builder.append(method.getName());
						builder.append('(');
						Parameter[] ptypes = method.getParameters();
						for (int i = 0; i < ptypes.length; i++) {
							if (i > 0)
								builder.append(',');
							builder.append(ptypes[i].getType().getName().replace('$', '.'));
							builder.append(' ');
							builder.append(ptypes[i].getName());
						}
						builder.append(')');
					}
					return new Throwing(new QMissingVirtualOverridingException(String.format(
							"Cannot convert %2$s to %1$s because class is required to implement following virtual methods: %3$s",
							pair.first.getSimpleName(), pair.second.getName(), builder)));
				}
	
				if (nonFinalMethods.size() == 1) {
					Method method = nonFinalMethods.get(0);
					StringBuilder builder = new StringBuilder();
					if (Modifier.isPublic(method.getModifiers())) {
						builder.append("public ");
					} else if (Modifier.isProtected(method.getModifiers())) {
						builder.append("protected ");
					}
					builder.append("final ");
					builder.append(method.getReturnType().getName().replace('$', '.'));
					builder.append(' ');
					builder.append(method.getName());
					builder.append('(');
					Parameter[] ptypes = method.getParameters();
					for (int i = 0; i < ptypes.length; i++) {
						if (i > 0)
							builder.append(',');
						builder.append(ptypes[i].getType().getName().replace('$', '.'));
						builder.append(' ');
						builder.append(ptypes[i].getName());
					}
					builder.append(')');
					return new Throwing(new QNonVirtualOverridingException(String.format(
							"Cannot convert %2$s to %1$s because following method has to be declared final: %3$s",
							pair.first.getSimpleName(), pair.second.getName(), builder), true));
				} else if (nonFinalMethods.size() > 1) {
					StringBuilder builder = new StringBuilder();
					for (Method method : nonFinalMethods) {
						if (builder.length() != 0) {
							builder.append(',');
							builder.append(' ');
						}
						if (Modifier.isPublic(method.getModifiers())) {
							builder.append("public ");
						} else if (Modifier.isProtected(method.getModifiers())) {
							builder.append("protected ");
						}
						builder.append("final ");
						builder.append(method.getReturnType().getName().replace('$', '.'));
						builder.append(' ');
						builder.append(method.getName());
						builder.append('(');
						Parameter[] ptypes = method.getParameters();
						for (int i = 0; i < ptypes.length; i++) {
							if (i > 0)
								builder.append(',');
							builder.append(ptypes[i].getType().getName().replace('$', '.'));
							builder.append(' ');
							builder.append(ptypes[i].getName());
						}
						builder.append(')');
					}
					return new Throwing(new QNonVirtualOverridingException(String.format(
							"Cannot convert %2$s to %1$s because following methods have to be declared final: %3$s",
							pair.first.getSimpleName(), pair.second.getName(), builder), true));
				}
	
				if (nonOverridableMethods.size() == 1) {
					Method method = nonOverridableMethods.get(0);
					StringBuilder builder = new StringBuilder();
					if (Modifier.isPublic(method.getModifiers())) {
						builder.append("public ");
					} else if (Modifier.isProtected(method.getModifiers())) {
						builder.append("protected ");
					}
					builder.append("final ");
					builder.append(method.getReturnType().getName().replace('$', '.'));
					builder.append(' ');
					builder.append(method.getName());
					builder.append('(');
					Parameter[] ptypes = method.getParameters();
					for (int i = 0; i < ptypes.length; i++) {
						if (i > 0)
							builder.append(',');
						builder.append(ptypes[i].getType().getName().replace('$', '.'));
						builder.append(' ');
						builder.append(ptypes[i].getName());
					}
					builder.append(')');
					return new Throwing(new QNonVirtualOverridingException(
							String.format("Cannot convert %2$s to %1$s because it overrides following final method: %3$s",
									pair.first.getSimpleName(), pair.second.getName(), builder),
							true));
				} else if (nonOverridableMethods.size() > 1) {
					StringBuilder builder = new StringBuilder();
					for (Method method : nonOverridableMethods) {
						if (builder.length() != 0) {
							builder.append(',');
							builder.append(' ');
						}
						if (Modifier.isPublic(method.getModifiers())) {
							builder.append("public ");
						} else if (Modifier.isProtected(method.getModifiers())) {
							builder.append("protected ");
						}
						builder.append("final ");
						builder.append(method.getReturnType().getName().replace('$', '.'));
						builder.append(' ');
						builder.append(method.getName());
						builder.append('(');
						Parameter[] ptypes = method.getParameters();
						for (int i = 0; i < ptypes.length; i++) {
							if (i > 0)
								builder.append(',');
							builder.append(ptypes[i].getType().getName().replace('$', '.'));
							builder.append(' ');
							builder.append(ptypes[i].getName());
						}
						builder.append(')');
					}
					return new Throwing(new QNonVirtualOverridingException(
							String.format("Cannot convert %2$s to %1$s because it overrides following final methods: %3$s",
									pair.first.getSimpleName(), pair.second.getName(), builder),
							true));
				}
			}
			return () -> {};
		}).check();
	}

	private static Method findAccessibleMethod(Method virtualProtectedMethod, Class<?> implementationClass) {
		if (implementationClass == null || isGeneratedClass(implementationClass))
			return null;
		Method method = null;
		try {
			method = implementationClass.getDeclaredMethod(virtualProtectedMethod.getName(),
					virtualProtectedMethod.getParameterTypes());
		} catch (NoSuchMethodException e) {
		}
		if (method == null) {
			return findAccessibleMethod(virtualProtectedMethod, implementationClass.getSuperclass());
		} else {
			if (virtualProtectedMethod.getReturnType() != method.getReturnType()) {
				return null;
			}
			int mod = method.getModifiers();
			return (Modifier.isPublic(mod) || Modifier.isProtected(mod)) && !Modifier.isStatic(mod) ? method : null;
		}
	}

	private static Class<?> findDefaultImplementation(Class<? extends QtObjectInterface> interfaceClass) {
		for (Class<?> cls : interfaceClass.getClasses()) {
			if (interfaceClass.isAssignableFrom(cls) && "Impl".equals(cls.getSimpleName())) {
				return cls;
			}
		}
		return null;
	}

	public static boolean initializePackage(ClassLoader classLoader, java.lang.Package pkg) {
		return pkg != null && initializePackage(classLoader, pkg.getName());
	}

	public static boolean initializePackage(java.lang.Class<?> cls) {
		return cls != null && cls.getPackage() != null && initializePackage(cls.getClassLoader(), cls.getPackage().getName());
	}
	
	public static boolean initializePackage(ClassLoader classLoader, String packagePath) {
		synchronized (initializedPackages) {
			Boolean b = initializedPackages.get(packagePath);
			if (b != null) {
				return b;
			}
			Class<?> cls;
			try {
				try {
					cls = Class.forName(packagePath + ".QtJambi_LibraryUtilities");
				} catch (ClassNotFoundException e) {
					if(classLoader!=null && classLoader!=QtJambiInternal.class.getClassLoader()) {
						cls = Class.forName(packagePath + ".QtJambi_LibraryUtilities", true, classLoader);
					}else {
						throw e;
					}
				}
			} catch (NoClassDefFoundError t) {
				if (t.getCause() instanceof Error && t.getCause() != t)
					throw (Error) t.getCause();
				else if (t.getCause() instanceof RuntimeException)
					throw (RuntimeException) t.getCause();
				throw t;
			} catch (ExceptionInInitializerError t) {
				if (t.getCause() instanceof Error && t.getCause() != t)
					throw (Error) t.getCause();
				else if (t.getCause() instanceof RuntimeException)
					throw (RuntimeException) t.getCause();
				throw t;
			} catch (ClassNotFoundException e1) {
				initializedPackages.put(packagePath, Boolean.FALSE);
				return false;
			}
			try {
				Method initialize = cls.getDeclaredMethod("initialize");
				methodInvocationHandler.invokeMethod(initialize, null);
				initializedPackages.put(packagePath, Boolean.TRUE);
				return true;
			} catch (NoSuchMethodException | NoSuchMethodError t) {
				return true;
			} catch (NoClassDefFoundError t) {
				if (t.getCause() instanceof Error && t.getCause() != t)
					throw (Error) t.getCause();
				else if (t.getCause() instanceof RuntimeException)
					throw (RuntimeException) t.getCause();
				throw t;
			} catch (RuntimeException | Error t) {
				throw t;
			} catch (Throwable t) {
				java.util.logging.Logger.getLogger("io.qt.internal").log(java.util.logging.Level.WARNING,
						"initializePackage", t);
				throw new RuntimeException(t);
			}
		}
	}

	static MethodHandle lambdaSlotHandles(Class<?> slotClass, SerializedLambda serializedLambda) {
		return lambdaSlotHandles.computeIfAbsent(slotClass, cls -> {
			try {
				Class<?> implClass = slotClass.getClassLoader()
						.loadClass(serializedLambda.getImplClass().replace('/', '.'));
				MethodHandles.Lookup lookup = QtJambiInternal.privateLookup(implClass);
				if (serializedLambda.getImplMethodKind() == MethodHandleInfo.REF_invokeVirtual
						|| serializedLambda.getImplMethodKind() == MethodHandleInfo.REF_invokeInterface) {
					return lookup.findVirtual(implClass, serializedLambda.getImplMethodName(),
							MethodType.fromMethodDescriptorString(serializedLambda.getImplMethodSignature(),
									implClass.getClassLoader()));
				} else if (serializedLambda.getImplMethodKind() == MethodHandleInfo.REF_invokeSpecial) {
					return lookup.findSpecial(implClass, serializedLambda.getImplMethodName(),
							MethodType.fromMethodDescriptorString(serializedLambda.getImplMethodSignature(),
									implClass.getClassLoader()),
							implClass);
				} else if (serializedLambda.getImplMethodKind() == MethodHandleInfo.REF_invokeStatic) {
					return lookup.findStatic(implClass, serializedLambda.getImplMethodName(),
							MethodType.fromMethodDescriptorString(serializedLambda.getImplMethodSignature(),
									implClass.getClassLoader()));
				} else if (serializedLambda.getImplMethodKind() == MethodHandleInfo.REF_newInvokeSpecial) {
					return lookup.findConstructor(implClass,
							MethodType.fromMethodDescriptorString(serializedLambda.getImplMethodSignature(),
									implClass.getClassLoader()));
				}
			} catch (ClassNotFoundException | NoSuchMethodException | IllegalAccessException | IllegalArgumentException
					| TypeNotPresentException e) {
				java.util.logging.Logger.getLogger("io.qt.internal").log(java.util.logging.Level.WARNING,
						"Exception caught while analyzing slot", e);
			}
			return null;
		});
	}

	public static final class LambdaInfo {
		public LambdaInfo(Class<?> ownerClass, Object owner, QObject qobject, boolean isStatic,
				MethodHandle methodHandle, Method reflectiveMethod, Constructor<?> reflectiveConstructor, List<Object> lambdaArgs) {
			super();
			this.ownerClass = ownerClass;
			this.owner = owner;
			this.qobject = qobject;
			this.isStatic = isStatic;
			this.methodHandle = methodHandle;
			this.reflectiveMethod = reflectiveMethod;
			this.reflectiveConstructor = reflectiveConstructor;
			this.lambdaArgs = lambdaArgs;
		}

		public final Class<?> ownerClass;
		public final Object owner;
		public final QObject qobject;
		public final boolean isStatic;
		public final MethodHandle methodHandle;
		public final Method reflectiveMethod;
		public final Constructor<?> reflectiveConstructor;
		public final List<Object> lambdaArgs;
	}

	public static LambdaInfo lambdaInfo(Serializable slotObject) {
		//String className = slotObject.getClass().getName();
		Class<?> slotClass = QtJambiInternal.getClass(slotObject);
		if (slotClass.isSynthetic()
				//&& className.contains("Lambda$") && className.contains("/")
				) {
			SerializedLambda serializedLambda = serializeLambdaExpression(slotObject);
			if(serializedLambda == null)
				return null;
			MethodHandle methodHandle = lambdaSlotHandles(slotClass, serializedLambda);
			Method reflectiveMethod = null;
			Constructor<?> reflectiveConstructor = null;
			Class<?> ownerClass = null;
			Object owner = null;
			QObject qobject = null;
			List<Object> lambdaArgsList = Collections.emptyList();
			if (methodHandle != null) {
				if(serializedLambda.getImplMethodKind()==MethodHandleInfo.REF_newInvokeSpecial)
					reflectiveConstructor = MethodHandles.reflectAs(Constructor.class, methodHandle);
				else
					reflectiveMethod = MethodHandles.reflectAs(Method.class, methodHandle);
				if (methodHandle.isVarargsCollector()) {
					methodHandle = methodHandle.asFixedArity();
				}
				if (reflectiveConstructor != null || reflectiveMethod != null) {
					ownerClass = reflectiveMethod==null ? reflectiveConstructor.getDeclaringClass() : reflectiveMethod.getDeclaringClass();
					if (Modifier.isStatic(reflectiveMethod==null ? reflectiveConstructor.getModifiers() : reflectiveMethod.getModifiers())) {
						if (serializedLambda.getCapturedArgCount() > 0) {
							if (serializedLambda.getCapturedArgCount() > 0)
								lambdaArgsList = new ArrayList<>();
							for (int i = 0; i < serializedLambda.getCapturedArgCount(); i++) {
								if (qobject == null && serializedLambda.getCapturedArg(i) instanceof QObject) {
									qobject = (QObject) serializedLambda.getCapturedArg(i);
								} else {
									lambdaArgsList.add(serializedLambda.getCapturedArg(i));
								}
							}
						}
						return new LambdaInfo(ownerClass, owner, qobject, true, methodHandle, reflectiveMethod,
								reflectiveConstructor, lambdaArgsList == Collections.emptyList() ? lambdaArgsList
										: Collections.unmodifiableList(lambdaArgsList));
					} else if (serializedLambda.getCapturedArgCount() > 0
							&& ownerClass.isInstance(serializedLambda.getCapturedArg(0))) {
						if (serializedLambda.getCapturedArg(0) instanceof QObject)
							qobject = (QObject) serializedLambda.getCapturedArg(0);
						owner = serializedLambda.getCapturedArg(0);
						if (serializedLambda.getCapturedArgCount() > 1)
							lambdaArgsList = new ArrayList<>();
						for (int i = 1; i < serializedLambda.getCapturedArgCount(); i++) {
							lambdaArgsList.add(serializedLambda.getCapturedArg(i));
						}
						return new LambdaInfo(ownerClass, owner, qobject, false, methodHandle, reflectiveMethod,
								reflectiveConstructor, lambdaArgsList == Collections.emptyList() ? lambdaArgsList
										: Collections.unmodifiableList(lambdaArgsList));
					} else if (serializedLambda.getCapturedArgCount() == 0) {
						return new LambdaInfo(ownerClass, owner, qobject, false, methodHandle, reflectiveMethod,
								reflectiveConstructor, lambdaArgsList == Collections.emptyList() ? lambdaArgsList
										: Collections.unmodifiableList(lambdaArgsList));
					}
				}
			}
		}
		return null;
	}

	public static void disposeObject(QtObjectInterface object) {
		NativeLink lnk = findInterfaceLink(object, false);
		if (lnk != null) {
			lnk.dispose();
		}
	}

	public static boolean isObjectDisposed(QtObjectInterface object) {
		NativeLink lnk = findInterfaceLink(object, true);
		return lnk == null || lnk.isDisposed();
	}

	public static boolean tryIsObjectDisposed(QtObjectInterface object) {
		NativeLink lnk = findInterfaceLink(object, false);
		return lnk == null || lnk.isDisposed();
	}

	public static void disposeObject(QtJambiObject object) {
		object.nativeLink.dispose();
	}

	public static boolean isObjectDisposed(QtJambiObject object) {
		return object.nativeLink.isDisposed();
	}

	public static Boolean areObjectsEquals(QtJambiObject object, Object other) {
		if (other instanceof QtJambiObject)
			return ((QtJambiObject) other).nativeLink.native__id == object.nativeLink.native__id;
		else
			return null;
	}

	private static Class<?> qmlListPropertiesClass;
	private static boolean qmlListPropertiesClassResolved;

	private static boolean isQQmlListProperty(Class<? extends Object> cls) {
		if (!qmlListPropertiesClassResolved) {
			qmlListPropertiesClassResolved = true;
			Class<?> _qmlListPropertiesClass = null;
			try {
				_qmlListPropertiesClass = Class.forName("io.qt.qml.QQmlListProperty");
			} catch (Exception e) {
			}
			qmlListPropertiesClass = _qmlListPropertiesClass;
		}
		return qmlListPropertiesClass != null && qmlListPropertiesClass == cls;
	}

	static native int registerQmlListProperty(String type);

	public static String internalNameOfArgumentType(Class<? extends Object> cls) {
		return internalTypeNameOfClass(cls, cls);
	}

	static String internalTypeNameOfClass(Class<? extends Object> cls, Type genericType) {
		try {
			if (isQQmlListProperty(cls) && genericType instanceof ParameterizedType) {
				ParameterizedType ptype = (ParameterizedType) genericType;
				Type actualTypes[] = ptype.getActualTypeArguments();
				if (actualTypes.length == 1 && actualTypes[0] instanceof Class<?>) {
					String argumentName = internalTypeNameOfClass((Class<?>) actualTypes[0], actualTypes[0]);
					if (argumentName.endsWith("*")) {
						argumentName = argumentName.substring(0, argumentName.length() - 1);
					}
					return "QQmlListProperty<" + argumentName + ">";
				}
			}else if (( cls==QMap.class
						|| cls==QHash.class
						|| cls==QMultiMap.class
						|| cls==QMultiHash.class
						|| cls==QPair.class
						|| cls==Map.class
						|| cls==NavigableMap.class) && genericType instanceof ParameterizedType) {
				ParameterizedType ptype = (ParameterizedType) genericType;
				Type actualTypes[] = ptype.getActualTypeArguments();
				if (actualTypes.length == 2) {
					Type keyType = actualTypes[0];
					if(actualTypes[0] instanceof ParameterizedType)
						keyType = ((ParameterizedType) actualTypes[0]).getRawType();
					else if(actualTypes[0] instanceof TypeVariable) {
						Type[] bounds = ((TypeVariable<?>) actualTypes[0]).getBounds();
						if(bounds.length>0) {
							if(bounds[0] instanceof ParameterizedType)
								keyType = ((ParameterizedType) bounds[0]).getRawType();
							else
								keyType = bounds[0];
						}
					}
					Type valueType = actualTypes[1];
					if(actualTypes[1] instanceof ParameterizedType)
						valueType = ((ParameterizedType) actualTypes[1]).getRawType();
					else if(actualTypes[1] instanceof TypeVariable) {
						Type[] bounds = ((TypeVariable<?>) actualTypes[1]).getBounds();
						if(bounds.length>0) {
							if(bounds[0] instanceof ParameterizedType)
								valueType = ((ParameterizedType) bounds[0]).getRawType();
							else
								valueType = bounds[0];
						}
					}
					if(keyType instanceof Class && valueType instanceof Class) {
						String keyName = internalTypeNameOfClass((Class<?>) keyType, actualTypes[0]);
						String valueName = internalTypeNameOfClass((Class<?>) valueType, actualTypes[1]);
						if(keyType==actualTypes[0])
							QMetaType.registerMetaType((Class<?>) keyType);
						if(valueType==actualTypes[1])
							QMetaType.registerMetaType((Class<?>) valueType);
						if(cls==NavigableMap.class) {
							return String.format("QMap<%1$s,%2$s>", keyName, valueName);
						}else if(cls==Map.class) {
							return String.format("QHash<%1$s,%2$s>", keyName, valueName);
						}else {
							return String.format("%1$s<%2$s,%3$s>", cls.getSimpleName(), keyName, valueName);
						}
					}
				}
			}else if ((
					AbstractMetaObjectTools.isListType(cls)
					|| cls==Collection.class
					|| cls==Queue.class
					|| cls==Deque.class
					|| cls==List.class
					|| cls==Set.class
					) && genericType instanceof ParameterizedType) {
				ParameterizedType ptype = (ParameterizedType) genericType;
				Type actualTypes[] = ptype.getActualTypeArguments();
				if (actualTypes.length == 1) {
					Type elementType = actualTypes[0];
					if(actualTypes[0] instanceof ParameterizedType)
						elementType = ((ParameterizedType) actualTypes[0]).getRawType();
					else if(actualTypes[0] instanceof TypeVariable) {
						Type[] bounds = ((TypeVariable<?>) actualTypes[0]).getBounds();
						if(bounds.length>0) {
							if(bounds[0] instanceof ParameterizedType)
								elementType = ((ParameterizedType) bounds[0]).getRawType();
							else
								elementType = bounds[0];
						}
					}
					if(elementType instanceof Class) {
						String elementName = internalTypeNameOfClass((Class<?>) elementType, actualTypes[0]);
						if(elementType==actualTypes[0])
							QMetaType.registerMetaType((Class<?>) elementType);
						if(cls==Collection.class
								|| cls==Queue.class
								|| cls==Deque.class
								|| cls==List.class
								|| cls==Set.class) {
							if(cls==Set.class) {
								return String.format("QSet<%1$s>", elementName);
							}else if(cls==Queue.class) {
								return String.format("QQueue<%1$s>", elementName);								
							}else if(cls==Deque.class) {
								return String.format("QStack<%1$s>", elementName);
							}else {
								return String.format("QList<%1$s>", elementName);
							}
						}else {
							return String.format("%1$s<%2$s>", cls.getSimpleName(), elementName);
						}
					}
				}
			}
			String result = internalTypeNameByClass(cls);
			boolean isEnumOrFlags = Enum.class.isAssignableFrom(cls) || QFlags.class.isAssignableFrom(cls);
			if (isEnumOrFlags && "JObjectWrapper".equals(result) && cls.getDeclaringClass() != null
					&& (QObject.class.isAssignableFrom(cls.getDeclaringClass())
							|| Qt.class.isAssignableFrom(cls.getDeclaringClass()))) {
				if (QtJambiInternal.isGeneratedClass(cls.getDeclaringClass())) {
					result = internalTypeNameOfClass(cls.getDeclaringClass(), null).replace("*", "")
																		+ "::" + cls.getSimpleName();
				}
			}
			return result;
		} catch (Throwable t) {
			java.util.logging.Logger.getLogger("io.qt.internal").log(java.util.logging.Level.SEVERE, "", t);
			return "";
		}
	}
	
	native static String internalTypeNameByClass(Class<?> cls);

	private static native int __qt_registerMetaType(Class<?> clazz, boolean isPointer, boolean isReference);
	
	private static native int __qt_registerMetaType2(int id, boolean isPointer, boolean isReference);

	private static native int __qt_registerContainerMetaType(Class<?> containerType, int... instantiations);
	
	static int registerRefMetaType(int id, boolean isPointer, boolean isReference) {
		if(!isPointer && !isReference)
			return id;
		return __qt_registerMetaType2(id, isPointer, isReference);
	}
	

	public static int registerMetaType(Class<?> clazz) {
		return registerMetaType(clazz, clazz, null, false, false);
	}

	public static int registerMetaType(Class<?> clazz, int[] instantiations) {
		return __qt_registerContainerMetaType(clazz, instantiations);
	}
	
	public static native int findMetaType(String name);
	
	static int registerMetaType(Parameter parameter) {
		AnnotatedElement annotatedParameterType = null;
    	if(useAnnotatedType)
    		annotatedParameterType = parameter.getAnnotatedType();
    	return registerMetaType(parameter.getType(), parameter.getParameterizedType(), annotatedParameterType, false, false);
	}

	public static int registerMetaType(Class<?> clazz, Type genericType, AnnotatedElement annotatedType, boolean isPointer, boolean isReference) {
		initializePackage(clazz);
		QtReferenceType referenceType = null;
		QtPointerType pointerType = null;
		if(annotatedType!=null) {
			referenceType = annotatedType.getAnnotation(QtReferenceType.class);
			if(referenceType!=null)
				isReference = !referenceType.isConst();
			pointerType = annotatedType.getAnnotation(QtPointerType.class);
			if(pointerType!=null && !isPointer)
				isPointer = true;
			QtMetaType metaTypeDecl = annotatedType.getAnnotation(QtMetaType.class);
			if(metaTypeDecl!=null) {
				int id;
				if(metaTypeDecl.id()!=0) {
					id = metaTypeDecl.id();
				}else if(metaTypeDecl.type()!=QMetaType.Type.UnknownType){
					id = metaTypeDecl.type().value();
				}else {
					id = QtJambiInternal.findMetaType(metaTypeDecl.name());
					if(id==0) {
						if(metaTypeDecl.name().isEmpty())
							throw new IllegalArgumentException("Incomplete @QtMetaType declaration. Either use type, id or name to specify meta type.");
						else
							throw new IllegalArgumentException("Unable to detect meta type "+metaTypeDecl.name());
					}
				}
				if(isPointer) {
					id = __qt_registerMetaType2(id, true, false);
				}else if(isReference){
					id = __qt_registerMetaType2(id, false, true);
				}
				return id;
			}
		}
		if (genericType instanceof ParameterizedType) {
			ParameterizedType parameterizedType = (ParameterizedType) genericType;
			Type[] actualTypeArguments = parameterizedType.getActualTypeArguments();
			AnnotatedElement[] actualAnnotatedTypeArguments = null;
			if(QtJambiInternal.useAnnotatedType) {
				if(annotatedType instanceof java.lang.reflect.AnnotatedParameterizedType) {
					actualAnnotatedTypeArguments = ((java.lang.reflect.AnnotatedParameterizedType)annotatedType).getAnnotatedActualTypeArguments();
				}
			}
			if (parameterizedType.getRawType() instanceof Class<?>) {
				if (isQQmlListProperty((Class<?>) parameterizedType.getRawType())) {
					String listPropertyName = internalTypeNameOfClass(clazz, genericType);
					int id = findMetaType(listPropertyName);
					if (id != QMetaType.Type.UnknownType.value()) {
						return id;
					} else {
						return registerQmlListProperty(listPropertyName);
					}
				} else if ((AbstractMetaObjectTools.isListType(clazz)
						|| List.class==parameterizedType.getRawType()
						|| Collection.class==parameterizedType.getRawType()
						|| Deque.class==parameterizedType.getRawType()
						|| Queue.class==parameterizedType.getRawType()
						|| Set.class==parameterizedType.getRawType())
							&& actualTypeArguments.length == 1) {
					if(List.class==parameterizedType.getRawType()) {
						if (actualTypeArguments[0] == String.class) {
							return QMetaType.Type.QStringList.value();
						} else if (actualTypeArguments[0] == QByteArray.class) {
							return QMetaType.Type.QByteArrayList.value();
						} else if (actualTypeArguments[0] instanceof Class<?>) {
							if (metaTypeId((Class<?>) actualTypeArguments[0]) == QMetaType.Type.QVariant.value()) {
								return QMetaType.Type.QVariantList.value();
							}
						}
					}
					int elementType = 0;
					if(actualTypeArguments[0] instanceof Class)
						elementType = registerMetaType((Class<?>)actualTypeArguments[0], actualTypeArguments[0], actualAnnotatedTypeArguments==null ? null : actualAnnotatedTypeArguments[0], false, false);
					else if(actualTypeArguments[0] instanceof ParameterizedType 
							&& ((ParameterizedType)actualTypeArguments[0]).getRawType() instanceof Class)
						elementType = registerMetaType((Class<?>)((ParameterizedType)actualTypeArguments[0]).getRawType(), actualTypeArguments[0], actualAnnotatedTypeArguments==null ? null : actualAnnotatedTypeArguments[0], false, false);
					else if(actualTypeArguments[0] instanceof TypeVariable) {
						Type[] bounds = ((TypeVariable<?>)actualTypeArguments[0]).getBounds();
						if(bounds.length>0) {
							if(bounds[0] instanceof Class)
								elementType = registerMetaType((Class<?>)bounds[0], actualTypeArguments[0], actualAnnotatedTypeArguments==null ? null : actualAnnotatedTypeArguments[0], false, false);
						}
					}
					if(elementType!=0) {
						int cotainerMetaType;
						if(AbstractMetaObjectTools.isListType(clazz)) {
							cotainerMetaType = __qt_registerContainerMetaType(clazz, elementType);
						}else if(Deque.class==parameterizedType.getRawType()) {
							cotainerMetaType = __qt_registerContainerMetaType(QStack.class, elementType);
						}else if(Queue.class==parameterizedType.getRawType()) {
							cotainerMetaType = __qt_registerContainerMetaType(QQueue.class, elementType);
						}else if(Set.class==parameterizedType.getRawType()) {
							cotainerMetaType = __qt_registerContainerMetaType(QSet.class, elementType);
						}else {
							cotainerMetaType = __qt_registerContainerMetaType(QList.class, elementType);
						}
						if(isPointer || isReference)
							cotainerMetaType = __qt_registerMetaType2(cotainerMetaType, isPointer, isReference);
						return cotainerMetaType;
					}
				} else if ((
							NavigableMap.class==parameterizedType.getRawType()
							|| Map.class==parameterizedType.getRawType()
							|| QMap.class==parameterizedType.getRawType()
							|| QMultiMap.class==parameterizedType.getRawType()
							|| QHash.class==parameterizedType.getRawType()
							|| QMultiHash.class==parameterizedType.getRawType()
						)
						&& actualTypeArguments.length == 2) {
					if (actualTypeArguments[0] == String.class
							&& actualTypeArguments[1] instanceof Class<?>) {
						if(NavigableMap.class==parameterizedType.getRawType()) {
							if (metaTypeId((Class<?>) actualTypeArguments[1]) == QMetaType.Type.QVariant.value()) {
								return QMetaType.Type.QVariantMap.value();
							}
						}
						if(Map.class==parameterizedType.getRawType()) {
							if (metaTypeId((Class<?>) actualTypeArguments[1]) == QMetaType.Type.QVariant.value()) {
								return QMetaType.Type.QVariantHash.value();
							}
						}
					}
					int keyType = 0;
					if(actualTypeArguments[0] instanceof Class)
						keyType = registerMetaType((Class<?>)actualTypeArguments[0], actualTypeArguments[0], actualAnnotatedTypeArguments==null ? null : actualAnnotatedTypeArguments[0], false, false);
					else if(actualTypeArguments[0] instanceof ParameterizedType 
							&& ((ParameterizedType)actualTypeArguments[0]).getRawType() instanceof Class)
						keyType = registerMetaType((Class<?>)((ParameterizedType)actualTypeArguments[0]).getRawType(), actualTypeArguments[0], actualAnnotatedTypeArguments==null ? null : actualAnnotatedTypeArguments[0], false, false);
					else if(actualTypeArguments[0] instanceof TypeVariable) {
						Type[] bounds = ((TypeVariable<?>)actualTypeArguments[0]).getBounds();
						if(bounds.length>0) {
							if(bounds[0] instanceof Class)
								keyType = registerMetaType((Class<?>)bounds[0], actualTypeArguments[0], actualAnnotatedTypeArguments==null ? null : actualAnnotatedTypeArguments[0], false, false);
						}
					}
					int valueType = 0;
					if(actualTypeArguments[1] instanceof Class)
						valueType = registerMetaType((Class<?>)actualTypeArguments[1], actualTypeArguments[1], actualAnnotatedTypeArguments==null ? null : actualAnnotatedTypeArguments[0], false, false);
					else if(actualTypeArguments[1] instanceof ParameterizedType 
							&& ((ParameterizedType)actualTypeArguments[1]).getRawType() instanceof Class)
						valueType = registerMetaType((Class<?>)((ParameterizedType)actualTypeArguments[1]).getRawType(), actualTypeArguments[1], actualAnnotatedTypeArguments==null ? null : actualAnnotatedTypeArguments[0], false, false);
					else if(actualTypeArguments[1] instanceof TypeVariable) {
						Type[] bounds = ((TypeVariable<?>)actualTypeArguments[1]).getBounds();
						if(bounds.length>0) {
							if(bounds[0] instanceof Class)
								valueType = registerMetaType((Class<?>)bounds[0], actualTypeArguments[1], actualAnnotatedTypeArguments==null ? null : actualAnnotatedTypeArguments[0], false, false);
						}
					}
					if(keyType!=0 && valueType!=0) {
						int cotainerMetaType;
						if(NavigableMap.class==parameterizedType.getRawType()) {
							cotainerMetaType = __qt_registerContainerMetaType(QMap.class, keyType, valueType);
						}else if(Map.class==parameterizedType.getRawType()) {
							cotainerMetaType = __qt_registerContainerMetaType(QHash.class, keyType, valueType);
						}else {
							cotainerMetaType = __qt_registerContainerMetaType(clazz, keyType, valueType);
						}
						if(isPointer || isReference)
							cotainerMetaType = __qt_registerMetaType2(cotainerMetaType, isPointer, isReference);
						return cotainerMetaType;
					}
				}
			}
		}
		return __qt_registerMetaType(clazz, isPointer, isReference);
	}

	private static native int __qt_metaTypeId(Class<?> clazz);

	public static int metaTypeId(Class<?> clazz) {
		initializePackage(clazz);
		return __qt_metaTypeId(clazz);
	}

	public static native Class<?> javaTypeForMetaTypeId(int metaTypeId);

	public static int objectMetaTypeId(Object o) {
		if (o == null) {
			return QMetaType.Type.Nullptr.value();
		} else {
			return metaTypeId(QtJambiInternal.getClass(o));
		}
	}

	public static int nextMetaTypeId(Class<?> clazz) {
		int id = QMetaType.Type.UnknownType.value();
		if (QtObjectInterface.class.isAssignableFrom(clazz)) {
			initializePackage(clazz);
			id = __qt_metaTypeId(clazz);
			if (QMetaType.Type.UnknownType.value() == id) {
				if (!clazz.isInterface())
					id = nextMetaTypeId(clazz.getSuperclass());
				if (QMetaType.Type.UnknownType.value() == id) {
					for (Class<?> iclass : clazz.getInterfaces()) {
						id = nextMetaTypeId(iclass);
						if (QMetaType.Type.UnknownType.value() != id) {
							break;
						}
					}
				}
			}
		} else {
			id = __qt_metaTypeId(clazz);
		}
		return id;
	}
	
	public static boolean isDebugBuild() {
		return NativeLibraryManager.configuration() == NativeLibraryManager.Configuration.Debug;
	}
	
	public static String processName() {
		return RetroHelper.processName();
	}

	@NativeAccess
	private static List<String> getLibraryPaths() {
		List<String> result = new ArrayList<String>();
		for(String path : System.getProperty("io.qt.pluginpath", "").split(File.pathSeparator)) {
			if(path!=null && !path.isEmpty() && !result.contains(path))
				result.add(path);
		}
		for(String path : System.getProperty("qtjambi.pluginpath", "").split(File.pathSeparator)) {
			if(path!=null && !path.isEmpty() && !result.contains(path))
				result.add(path);
		}

		try {
			if (io.qt.internal.NativeLibraryManager.isUsingDeploymentSpec()) {

				List<String> pluginPaths = io.qt.internal.NativeLibraryManager.pluginPaths();
				for (int i = pluginPaths.size()-1; i >= 0; --i) {
					String p = pluginPaths.get(i);
					if(p!=null && !p.isEmpty() && !result.contains(p))
						result.add(p);
				}
			}
		} catch (Exception e) {
			java.util.logging.Logger.getLogger("io.qt.internal").log(java.util.logging.Level.SEVERE, "", e);
		}
		return result;
	}

	@NativeAccess
	private static void extendStackTrace(Throwable e, String methodName, String fileName, int lineNumber) {
		if (fileName == null)
			return;
		fileName = new java.io.File(fileName).getName();
		StackTraceElement[] threadStackTrace = Thread.currentThread().getStackTrace();
		StackTraceElement[] stackTrace = e.getStackTrace();
		StackTraceElement[] newStackTrace = new StackTraceElement[stackTrace.length + 1];
		int cursor = 0;
		for (; cursor < stackTrace.length && cursor < threadStackTrace.length; ++cursor) {
			if (!stackTrace[stackTrace.length - cursor - 1]
					.equals(threadStackTrace[threadStackTrace.length - cursor - 1])) {
				break;
			}
		}
		StackTraceElement newElement = new StackTraceElement("<native>",
				methodName == null ? "<unknown_method>" : methodName, fileName, lineNumber);
		if (cursor > 0) {
			System.arraycopy(stackTrace, 0, newStackTrace, 0, stackTrace.length - cursor);
			newStackTrace[stackTrace.length - cursor] = newElement;
			System.arraycopy(stackTrace, stackTrace.length - cursor, newStackTrace, stackTrace.length - cursor + 1,
					cursor);
		} else {
			System.arraycopy(stackTrace, 0, newStackTrace, 0, stackTrace.length);
			newStackTrace[stackTrace.length] = newElement;
		}
		e.setStackTrace(newStackTrace);
	}

	@NativeAccess
	private static void reportException(String methodName, Throwable e) {
		if(!(e instanceof ThreadDeath)) try {
			UncaughtExceptionHandler handler = Thread.currentThread().getUncaughtExceptionHandler();
			while(handler != null && (Object)QtJambiInternal.getClass(handler)==ThreadGroup.class) {
				try {
					ThreadGroup tg = (ThreadGroup)handler;
					handler = tg.getParent();
				}catch(Throwable e1) {break;}
			}
			if(handler==null)
                handler = Thread.getDefaultUncaughtExceptionHandler();
			if (handler != null 
					&& (Object)QtJambiInternal.getClass(handler)!=ThreadGroup.class
					&& !QtJambiInternal.getClass(handler).getName().startsWith("com.android")) {
				handler.uncaughtException(Thread.currentThread(), e);
			} else{
				LogRecord logRecord = new LogRecord(java.util.logging.Level.SEVERE, QtJambiInternal.getClass(e).getTypeName()+(methodName==null ? " has been thrown" : " has been thrown in "+methodName));
				logRecord.setThrown(e);
				if(methodName!=null) {
					logRecord.setSourceClassName(methodName);
		    		logRecord.setSourceMethodName("");
				}
	    		java.util.logging.Logger.getLogger("io.qt").log(logRecord);
			}
		} catch (Throwable e1) {
			e1.addSuppressed(e);
			e1.printStackTrace();
		}
	}

	public static void initializeNativeObject(QtObjectInterface object, Map<Class<?>, List<?>> arguments)
			throws IllegalArgumentException {
		Class<?> cls = getClass(object);
		io.qt.InternalAccess.CallerContext callerInfo = RetroHelper.classAccessChecker().apply(2);
		if (callerInfo.declaringClass == null || !callerInfo.declaringClass.isAssignableFrom(cls)
				|| !"<init>".equals(callerInfo.methodName)) {
			throw new RuntimeException(new IllegalAccessException(
					"QtUtilities.initializeNativeObject(...) can only be called from inside the given object's constructor. Expected: "
							+ cls.getName() + ".<init>, found: "
							+ (callerInfo.declaringClass == null ? "null" : callerInfo.declaringClass.getName()) + "."
							+ callerInfo.methodName));
		}
		__qt_initializeNativeObject(callerInfo.declaringClass, object, findInterfaceLink(object, true, false), arguments);
	}

	static void initializeNativeObject(QtObjectInterface object, NativeLink link) throws IllegalArgumentException {
		Class<?> cls = getClass(object);
		initializePackage(cls);
		__qt_initializeNativeObject(cls, object, link, Collections.emptyMap());
	}

	private native static void __qt_initializeNativeObject(Class<?> callingClass, QtObjectInterface object,
			NativeLink link, Map<Class<?>, List<?>> arguments) throws IllegalArgumentException;

	static class NativeLink extends WeakReference<QtObjectInterface> implements Cleanable {

		private NativeLink(QtObjectInterface object) {
			super(object, referenceQueue);
//			cls = object == null ? null : object.getClass();
		}

//		Class<?> cls;
		private @NativeAccess long native__id = 0;

		@NativeAccess
		private final void detach(long native__id, boolean hasDisposedSignal) {
			QMetaObject.DisposedSignal disposed = hasDisposedSignal ? takeSignalOnDispose(this) : null;
			boolean detached = false;
			synchronized (this) {
				if (this.native__id == native__id) {
					this.native__id = 0;
					detached = true;
				}
			}
			if (disposed != null) {
				try {
					if (detached)
						disposed.emitSignal();
					disposed.disconnect();
				} catch (Throwable e) {
					e.printStackTrace();
				}
			}
			if (detached)
				enqueue();
		}

		@Override
		public synchronized void clean() {
			if (native__id != 0) {
				clean(native__id);
			} else {
				QMetaObject.DisposedSignal disposed = takeSignalOnDispose(this);
				if (disposed != null)
					disposed.disconnect();
			}
		}

		@NativeAccess
		private final synchronized void reset() {
			if (native__id != 0 && hasDisposedSignal(native__id)) {
				QMetaObject.DisposedSignal disposed = takeSignalOnDispose(this);
				if (disposed != null)
					disposed.disconnect();
			}
			this.native__id = 0;
		}

		private static native void clean(long native__id);
		private static native boolean hasDisposedSignal(long native__id);
		private static native void setHasDisposedSignal(long native__id);

		final synchronized void dispose() {
			if (native__id != 0) {
				dispose(native__id);
			}
		}

		final synchronized boolean isDisposed() {
			return native__id == 0;
		}

		private static native void dispose(long native__id);

		private static native String qtTypeName(long native__id);

		Object getMemberAccess(Class<?> interfaceClass) {
			throw new RuntimeException("Requesting member access of non-interface object is not permitted.");
		}

		void initialize(QtJambiObject obj) {
		}

		@Override
		public final String toString() {
			QtObjectInterface o = get();
			if (o != null) {
				return QtJambiInternal.getClass(o).getName() + "@" + Integer.toHexString(System.identityHashCode(o));
			} else {
				String qtTypeName = null;
				synchronized (this) {
					if (native__id != 0) {
						qtTypeName = qtTypeName(native__id);
					}
				}
				if (qtTypeName != null)
					return qtTypeName + "[disposed]";
				return super.toString();
			}
		}
	}

	private static class InterfaceNativeLink extends NativeLink {

		private static final Map<Class<?>, Function<Object,Object>> memberAccessConstructorHandles = new HashMap<>();

		private final Map<Class<?>, Object> memberAccesses;

		private InterfaceNativeLink(QtObjectInterface object, List<Class<? extends QtObjectInterface>> interfaces) {
			super(object);
			Map<Class<?>, Object> memberAccesses = new HashMap<>();
			synchronized (memberAccessConstructorHandles) {
				for (Class<? extends QtObjectInterface> _iface : interfaces) {
					@SuppressWarnings("unchecked")
					Function<Object,Object> constructorHandle = memberAccessConstructorHandles.computeIfAbsent(_iface, iface -> {
						for (Class<?> innerClass : iface.getClasses()) {
							if (io.qt.MemberAccess.class.isAssignableFrom(innerClass)) {
								try {
									return (Function<Object,Object>)methodInvocationHandler.getFactory1(innerClass.getDeclaredConstructor(iface));
								} catch (Exception e) {
									e.printStackTrace();
								}
							}
						}
						return null;
					});
					if (constructorHandle != null) {
						try {
							Object memberAccess = constructorHandle.apply(object);
							memberAccesses.put(_iface, memberAccess);
						} catch (Throwable e) {
							e.printStackTrace();
						}
					}
				}
			}
			this.memberAccesses = Collections.unmodifiableMap(memberAccesses);
		}

		Object getMemberAccess(Class<?> interfaceClass) {
			return memberAccesses.get(interfaceClass);
		}

		private Map<Class<?>, Map<String, Object>> referenceCounts;

		public void setReferenceCount(Class<? extends QtObjectInterface> declaringClass, String fieldName,
				Object newValue) {
			if (referenceCounts == null) {
				referenceCounts = Collections.synchronizedMap(new HashMap<>());
			}
			Map<String, Object> referenceCountsVariables = referenceCounts.computeIfAbsent(declaringClass,
					c -> Collections.synchronizedMap(new HashMap<>()));
			referenceCountsVariables.put(fieldName, newValue);
		}

		public Object getReferenceCountCollection(Class<? extends QtObjectInterface> declaringClass, String fieldName,
				Supplier<Object> collectionSupplier) {
			if (referenceCounts == null) {
				if (collectionSupplier != null) {
					referenceCounts = Collections.synchronizedMap(new HashMap<>());
					Map<String, Object> referenceCountsVariables = referenceCounts.computeIfAbsent(declaringClass,
							c -> Collections.synchronizedMap(new HashMap<>()));
					Object result = collectionSupplier.get();
					referenceCountsVariables.put(fieldName, result);
					return result;
				} else {
					return null;
				}
			} else {
				if (collectionSupplier != null) {
					return referenceCounts
							.computeIfAbsent(declaringClass, c -> Collections.synchronizedMap(new HashMap<>()))
							.computeIfAbsent(fieldName, _fieldName -> collectionSupplier.get());
				} else {
					return referenceCounts
							.computeIfAbsent(declaringClass, c -> Collections.synchronizedMap(new HashMap<>()))
							.get(fieldName);
				}
			}
		}

		@Override
		public synchronized void clean() {
			referenceCounts = null;
			super.clean();
		}

		void initialize(QtJambiObject obj) {
			initializeNativeObject(obj, this);
		}
	}

	private static final class InterfaceBaseNativeLink extends InterfaceNativeLink {
		final int ownerHashCode;

		private InterfaceBaseNativeLink(QtObjectInterface object, List<Class<? extends QtObjectInterface>> interfaces) {
			super(object, interfaces);
			ownerHashCode = System.identityHashCode(object);
			interfaceLinks.put(ownerHashCode, this);
		}

		@Override
		public synchronized void clean() {
			super.clean();
			interfaceLinks.remove(ownerHashCode);
		}
	}

	static NativeLink findInterfaceLink(QtObjectInterface iface, boolean forceCreation) {
		return findInterfaceLink(iface, forceCreation, forceCreation);
	}

	@NativeAccess
	private static NativeLink findInterfaceLink(QtObjectInterface iface, boolean forceCreation, boolean initialize) {
		if (iface instanceof QtJambiObject) {
			return ((QtJambiObject) iface).nativeLink;
		} else if (iface!=null){
			NativeLink link = interfaceLinks.get(System.identityHashCode(iface));
			if (link == null && forceCreation) {
				link = createNativeLink(iface);
				if (link == null) {
					link = new InterfaceBaseNativeLink(iface, Collections.emptyList());
				}else if (initialize) {
					initializeNativeObject(iface, link);
				}
			}
			return link;
		}else return null;
	}

	private native static List<Class<? extends QtObjectInterface>> getInterfaces(
			Class<? extends QtObjectInterface> cls);

	@NativeAccess
	static NativeLink createNativeLink(QtJambiObject object) {
		List<Class<? extends QtObjectInterface>> interfaces = getInterfaces(getClass(object));
		if (interfaces != null) {
			return new InterfaceNativeLink(object, interfaces);
		} else {
			return new NativeLink(object);
		}
	}
	
	public native static <T> Class<T> getClass(T object);//Class.getClass() lead to recursive calls on android when using inside of interface default methods.

	@NativeAccess
	private static NativeLink createNativeLink(QtObjectInterface iface) {
		List<Class<? extends QtObjectInterface>> interfaces = getInterfaces(getClass(iface));
		if (interfaces != null) {
			return new InterfaceBaseNativeLink(iface, interfaces);
		} else {
			return null;
		}
	}

	private native static void registerDependentObject(long dependentObject, long owner);

	private native static void unregisterDependentObject(long dependentObject, long owner);

	/**
	 * This method converts a native std::exception to it's causing java exception
	 * if any.
	 * 
	 * @param exception
	 */
	@NativeAccess
	private native static Throwable convertNativeException(long exception);

	private native static boolean isSharedPointer(long nativeId);

	public static boolean isSharedPointer(QtObjectInterface object) {
		return isSharedPointer(internalAccess.nativeId(object));
	}

	@SuppressWarnings("unchecked")
	public static <T> Class<T> getFactoryClass(QMetaObject.Method1<T, ?> method) {
		return (Class<T>) getFactoryClass((Serializable) method);
	}

	@SuppressWarnings("unchecked")
	public static <T> Class<T> getFactoryClass(QMetaObject.Method2<T, ?, ?> method) {
		return (Class<T>) getFactoryClass((Serializable) method);
	}

	@SuppressWarnings("unchecked")
	public static <T> Class<T> getFactoryClass(QMetaObject.Method3<T, ?, ?, ?> method) {
		return (Class<T>) getFactoryClass((Serializable) method);
	}

	@SuppressWarnings("unchecked")
	public static <T> Class<T> getFactoryClass(QMetaObject.Method4<T, ?, ?, ?, ?> method) {
		return (Class<T>) getFactoryClass((Serializable) method);
	}

	@SuppressWarnings("unchecked")
	public static <T> Class<T> getFactoryClass(QMetaObject.Method5<T, ?, ?, ?, ?, ?> method) {
		return (Class<T>) getFactoryClass((Serializable) method);
	}

	@SuppressWarnings("unchecked")
	public static <T> Class<T> getFactoryClass(QMetaObject.Method6<T, ?, ?, ?, ?, ?, ?> method) {
		return (Class<T>) getFactoryClass((Serializable) method);
	}

	@SuppressWarnings("unchecked")
	public static <T> Class<T> getFactoryClass(QMetaObject.Method7<T, ?, ?, ?, ?, ?, ?, ?> method) {
		return (Class<T>) getFactoryClass((Serializable) method);
	}

	@SuppressWarnings("unchecked")
	public static <T> Class<T> getFactoryClass(QMetaObject.Method8<T, ?, ?, ?, ?, ?, ?, ?, ?> method) {
		return (Class<T>) getFactoryClass((Serializable) method);
	}

	@SuppressWarnings("unchecked")
	public static <T> Class<T> getFactoryClass(QMetaObject.Method9<T, ?, ?, ?, ?, ?, ?, ?, ?, ?> method) {
		return (Class<T>) getFactoryClass((Serializable) method);
	}

	private static Class<?> getFactoryClass(Serializable method) {
		LambdaInfo lamdaInfo = lambdaInfo(method);
		if(lamdaInfo!=null) {
			if (lamdaInfo.reflectiveMethod != null && (lamdaInfo.lambdaArgs == null || lamdaInfo.lambdaArgs.isEmpty())
					&& !lamdaInfo.reflectiveMethod.isSynthetic() && !lamdaInfo.reflectiveMethod.isBridge()
					&& !Modifier.isStatic(lamdaInfo.reflectiveMethod.getModifiers())) {
				return lamdaInfo.reflectiveMethod.getDeclaringClass();
			}else if (lamdaInfo.reflectiveConstructor != null && (lamdaInfo.lambdaArgs == null || lamdaInfo.lambdaArgs.isEmpty())
					&& !lamdaInfo.reflectiveConstructor.isSynthetic()
					&& !Modifier.isStatic(lamdaInfo.reflectiveConstructor.getModifiers())) {
				return lamdaInfo.reflectiveConstructor.getDeclaringClass();
			}
		}
		return null;
	}

	@SuppressWarnings("unchecked")
	public static <R> Class<R> getReturnType(QMetaObject.Method1<?, R> method) {
		LambdaInfo lamdaInfo = lambdaInfo(method);
		if (lamdaInfo!=null && lamdaInfo.methodHandle != null) {
			return (Class<R>) lamdaInfo.methodHandle.type().returnType();
		} else {
			return null;
		}
	}
	
	public static Class<?> toClass(Type type){
		if (type instanceof Class) {
			return (Class<?>) type;
		} else if (type instanceof GenericArrayType) {
			GenericArrayType gtype = (GenericArrayType) type;
			Class<?> classType = toClass(gtype.getGenericComponentType());
			return Array.newInstance(classType, 0).getClass();
		} else if (type instanceof WildcardType) {
			WildcardType wt = (WildcardType) type;
			Type[] lbounds = wt.getLowerBounds();
			Type[] ubounds = wt.getUpperBounds();
			if(lbounds==null || lbounds.length==0)
				if(ubounds==null || ubounds.length==0)
					return Object.class;
				else
					return toClass(ubounds[0]);
			else
				return toClass(lbounds[0]);
		} else if (type instanceof ParameterizedType) {
			ParameterizedType ptype = (ParameterizedType) type;
			return toClass(ptype.getRawType());
		} else if (type instanceof TypeVariable<?>) {
			TypeVariable<?> tv = (TypeVariable<?>)type;
			Type[] bounds = tv.getBounds();
			if(bounds==null || bounds.length==0)
				return Object.class;
			else
				return toClass(bounds[0]);
		} else {
			throw new RuntimeException("Unable to find raw type for " + type.getTypeName()+"; type: "+getClass(type));
		}
	}
	
	private static final AtomicBoolean canSetThreadInterruptible = new AtomicBoolean(true);

	@NativeAccess
	private static boolean setThreadInterruptible(Thread thread, Object interruptible) {
		if(canSetThreadInterruptible.get()) {
			try {
				Field blockerField = Thread.class.getDeclaredField("blocker");
				setField(thread, blockerField, interruptible);
				return true;
			} catch (Throwable e) {
				if(setFieldByName(thread, "blocker", "Lsun/nio/ch/Interruptible;", false, interruptible))
					return true;
				try {
					Field blockOnField = Thread.class.getDeclaredField("blockOn");
					setField(thread, blockOnField, interruptible);
					return true;
				} catch (Throwable e1) {
					if(setFieldByName(thread, "blockOn", "Lsun/nio/ch/Interruptible;", false, interruptible))
						return true;
					canSetThreadInterruptible.set(false);
				}
			}
		}
		return false;
	}

	private static class AssociativeReference extends WeakReference<Object> implements Cleanable {

		public AssociativeReference(Object r) {
			super(r, referenceQueue);
		}

		@Override
		public void clean() {
			synchronized (object2ObjectAssociations) {
				object2ObjectAssociations.remove(this);
			}
		}
	}

	private static final Map<AssociativeReference, Object> object2ObjectAssociations = new HashMap<>();

	@NativeAccess
	private static void createAssociation(Object o1, Object o2) {
		synchronized (object2ObjectAssociations) {
			AssociativeReference reference = new AssociativeReference(o1);
			object2ObjectAssociations.put(reference, o2);
			if(o2 instanceof QtObjectInterface) {
				QMetaObject.DisposedSignal disposed = getSignalOnDispose((QtObjectInterface)o2, true);
				if (disposed != null)
					disposed.connect(()->object2ObjectAssociations.remove(reference));
			}
		}
	}

	@NativeAccess
	private static boolean deleteAssociation(Object o1) {
		AssociativeReference matchingReference = null;
		synchronized (object2ObjectAssociations) {
			for (AssociativeReference ref : object2ObjectAssociations.keySet()) {
				if (ref.get() == o1) {
					matchingReference = ref;
					break;
				}
			}
			if (matchingReference != null)
				object2ObjectAssociations.remove(matchingReference);
		}
		if (matchingReference != null) {
			matchingReference.enqueue();
			return true;
		} else
			return false;
	}

	@NativeAccess
	private static Object findAssociation(Object o1) {
		synchronized (object2ObjectAssociations) {
			AssociativeReference matchingReference = null;
			for (AssociativeReference ref : object2ObjectAssociations.keySet()) {
				if (ref.get() == o1) {
					matchingReference = ref;
					break;
				}
			}
			return matchingReference == null ? null : object2ObjectAssociations.get(matchingReference);
		}
	}

	public static Supplier<Class<?>> callerClassProvider() {
		return RetroHelper.callerClassProvider();
	}
	
	private native static String internalTypeName(String s, ClassLoader classLoader);

	public static String cppNormalizedSignature(String signalName, Class<?> classType) {
		int idx = signalName.indexOf("(");
        if(idx>=0) {
            String parameters = signalName.substring(idx).trim();
            String name = signalName.substring(0, idx);
            if(parameters.startsWith("(") && parameters.endsWith(")")) {
                parameters = parameters.substring(1, parameters.length()-1).trim();
                if(parameters.isEmpty()) {
                    return name+"()";
                }else {
                    String[] argumentTypes = parameters.split("\\,");
                    List<Parameter[]> possibleMethods = new ArrayList<>();
                    while(classType!=null) {
                    	for(Method method : classType.getDeclaredMethods()) {
                    		if(method.getParameterCount()==argumentTypes.length)
                    			possibleMethods.add(method.getParameters());
                    	}
                    	classType = classType.getSuperclass();
                    }
                    name += "(";
                    for (int i = 0; i < argumentTypes.length; ++i) {
                        if(i>0) {
                            name += ",";
                        }
                        Class<?> paramType = null;
                        Type genericParamType = null;
                        int arrayDimension = 0;
                        argumentTypes[i] = argumentTypes[i].replace(" ", "");
                        while(argumentTypes[i].endsWith("[]")) {
                        	++arrayDimension;
                        	argumentTypes[i] = argumentTypes[i].substring(0, argumentTypes[i].length()-2);
                        }
                        try {
                        	if(arrayDimension==0)
                        		paramType = Class.forName(argumentTypes[i].replace(" ", ""));
                        	else {
                        		String predix = "";
                        		for (int j = 0; j < arrayDimension; j++) {
                        			predix += '[';
								}
                        		switch(argumentTypes[i]) {
                        		case "int":
                            		paramType = Class.forName(predix + "I");
                        			break;
                        		case "long":
                            		paramType = Class.forName(predix + "J");
                        			break;
                        		case "short":
                            		paramType = Class.forName(predix + "S");
                        			break;
                        		case "byte":
                            		paramType = Class.forName(predix + "B");
                        			break;
                        		case "char":
                            		paramType = Class.forName(predix + "C");
                        			break;
                        		case "float":
                            		paramType = Class.forName(predix + "F");
                        			break;
                        		case "double":
                            		paramType = Class.forName(predix + "D");
                        			break;
                        		case "boolean":
                            		paramType = Class.forName(predix + "Z");
                        			break;
                    			default:
                            		paramType = Class.forName(predix + "L" + argumentTypes[i] + ";");
                    				break;
                        		}
                        	}
						} catch (Throwable e) {
						}
                        if(paramType==null && classType!=null) {
                        	try {
                        		if(arrayDimension==0)
                        			paramType = classType.getClassLoader().loadClass(argumentTypes[i].replace(" ", ""));
                        		else {
                        			String predix = "";
                            		for (int j = 0; j < arrayDimension; j++) {
                            			predix += '[';
    								}
                            		paramType = Class.forName(predix + "L" + argumentTypes[i] + ";");
                        		}
    						} catch (Throwable e) {
    						}
                        }
                        if(paramType==null) {
                        	if(arrayDimension==0) {
	                        	for(Parameter[] method : possibleMethods) {
	                        		if(method[i].getType().getName().equals(argumentTypes[i])
	                        				|| method[i].getType().getSimpleName().equals(argumentTypes[i])
	                        				|| method[i].getParameterizedType().getTypeName().equals(argumentTypes[i])) {
	                        			paramType = method[i].getType();
	                        			genericParamType = method[i].getParameterizedType();
	                        			break;
	                        		}
	                        	}
                        	}else {
                        		for(Parameter[] method : possibleMethods) {
                        			Class<?> type = method[i].getType();
                        			int _arrayDimension = 0;
                        			while(type.isArray()) {
                        				type = type.getComponentType();
                        				++_arrayDimension;
                        			}
                        			
                        			if(arrayDimension==_arrayDimension 
                        					&& (type.getName().equals(argumentTypes[i])
	                        				 || type.getSimpleName().equals(argumentTypes[i]))) {
	                        			paramType = method[i].getType();
	                        			genericParamType = method[i].getParameterizedType();
	                        			break;
	                        		}
                        		}
                        	}
                        }
                        if(arrayDimension>0) {
                        	String predix = "";
                    		for (int j = 0; j < arrayDimension; j++) {
                    			predix += '[';
							}
                    		argumentTypes[i] = predix + "L" + argumentTypes[i] + ";";
                        }
                        String cpptype = 
                        		paramType==null
                        		? internalTypeName(argumentTypes[i], MetaObjectTools.class.getClassLoader())
                        				: internalTypeNameOfClass(paramType, genericParamType==null ? paramType : genericParamType);
                        if(cpptype.isEmpty())
                        	cpptype = argumentTypes[i];
                        name += cpptype;
                    }
                    name += ")";
                }
                return name;
            }
            return signalName;
        }else {
            return signalName+"()";
        }
	}

	public static boolean isAvailableQtLibrary(Class<?> callerClass, String library) {
		return NativeLibraryManager.isAvailableQtLibrary(callerClass, library);
	}

	public static boolean isAvailableLibrary(String library, String version) {
		return NativeLibraryManager.isAvailableLibrary(library, version);
	}

	public static void loadQtLibrary(Class<?> callerClass, String library, LibraryRequirementMode libraryRequirementMode, String...platforms) {
		NativeLibraryManager.loadQtLibrary(callerClass, library, libraryRequirementMode, platforms);
	}

	public static void loadUtilityLibrary(String library, String version, LibraryRequirementMode libraryRequirementMode, String...platforms) {
		NativeLibraryManager.loadUtilityLibrary(library, version, libraryRequirementMode, platforms);
	}

	public static void loadLibrary(String lib) {
		NativeLibraryManager.loadLibrary(lib);
	}

	public static File jambiDeploymentDir() {
		return NativeLibraryManager.jambiDeploymentDir();
	}

	public static void loadQtJambiLibrary(Class<?> callerClass, String library) {
		NativeLibraryManager.loadQtJambiLibrary(callerClass, library);
	}
	
	public static void loadJambiLibrary(Class<?> callerClass, String library) {
		NativeLibraryManager.loadJambiLibrary(callerClass, library);
	}

	public static int majorVersion() {
		return QtJambi_LibraryUtilities.qtMajorVersion;
	}
	
	public static int minorVersion() {
		return QtJambi_LibraryUtilities.qtMinorVersion;
	}
	
	public static int qtjambiPatchVersion() {
		return QtJambi_LibraryUtilities.qtJambiPatch;
	}
	
	public static String qtJambiLibraryPath() {
		return NativeLibraryManager.qtJambiLibraryPath();
	}

	public static String qtLibraryPath() {
		return NativeLibraryManager.qtLibraryPath();
	}
	
	public static boolean isMinGWBuilt() {
		return NativeLibraryManager.isMinGWBuilt();
	}
	
	public static String osArchName() {
		return NativeLibraryManager.osArchName();
	}

	public static Object createMetaType(int id, Class<?> javaType, Object copy) {
		if (copy != null && javaType != null) {
			if (javaType.isPrimitive()) {
				copy = getComplexType(javaType).cast(copy);
			} else {
				if (Collection.class.isAssignableFrom(javaType)) {
					copy = Collection.class.cast(copy);
				} else if (Map.class.isAssignableFrom(javaType)) {
					copy = Map.class.cast(copy);
				} else {
					copy = javaType.cast(copy);
				}
			}
		}
		return __qt_createMetaType(id, copy);
	}

	private native static Object __qt_createMetaType(int id, Object copy);
	
	@NativeAccess
	private static IntFunction<io.qt.InternalAccess.CallerContext> invocationInfoProvider(){
		return RetroHelper.classAccessChecker();
	}
	
	public static <K,V> Function<? super K, ArrayList<V>> getArrayListFactory(){
		return key->new ArrayList<>();
	}
	
	@NativeAccess
	private static String objectToString(Object object) {
		if (object != null) {
			try {
				Method toStringMethod = QtJambiInternal.getClass(object).getMethod("toString");
				if (toStringMethod.getDeclaringClass() != Object.class) {
					return object.toString();
				}
			} catch (Exception e) {
			}
		}
		return null;
	}
	
	static final InternalAccess internalAccess = new InternalAccess() {

		@Override
		public <E extends Enum<E> & QtEnumerator> E resolveEnum(Class<E> cl, int value, String name) {
			if (name != null) {
				if (name.isEmpty())
					name = null;
				else {
					E enm = null;
					try {
						enm = Enum.valueOf(cl, name);
					} catch (Exception e) {
					}
					if (enm != null) {
						if (enm.value() == value) {
							return enm;
						} else {
							throw new io.qt.QNoSuchEnumValueException(value, name);
						}
					}
				}
			}
			try {
				E enm = resolveIntEnum(cl.hashCode(), cl, value, name);
				if (enm == null) {
					if (name == null)
						throw new QNoSuchEnumValueException(value);
					else
						throw new QNoSuchEnumValueException(value, name);
				}
				return enm;
			} catch (QNoSuchEnumValueException e) {
				throw e;
			} catch (Throwable e) {
				throw new QNoSuchEnumValueException(value, e);
			}
		}

		@Override
		public <E extends Enum<E> & QtByteEnumerator> E resolveEnum(Class<E> cl, byte value, String name) {
			if (name != null) {
				if (name.isEmpty())
					name = null;
				else {
					E enm = null;
					try {
						enm = Enum.valueOf(cl, name);
					} catch (Exception e) {
					}
					if (enm != null) {
						if (enm.value() == value) {
							return enm;
						} else {
							throw new io.qt.QNoSuchEnumValueException(value, name);
						}
					}
				}
			}
			try {
				E enm = resolveByteEnum(cl.hashCode(), cl, value, name);
				if (enm == null) {
					if (name == null)
						throw new QNoSuchEnumValueException(value);
					else
						throw new QNoSuchEnumValueException(value, name);
				}
				return enm;
			} catch (QNoSuchEnumValueException e) {
				throw e;
			} catch (Throwable e) {
				throw new QNoSuchEnumValueException(value, e);
			}
		}

		@Override
		public <E extends Enum<E> & QtShortEnumerator> E resolveEnum(Class<E> cl, short value, String name) {
			if (name != null) {
				if (name.isEmpty())
					name = null;
				else {
					E enm = null;
					try {
						enm = Enum.valueOf(cl, name);
					} catch (Exception e) {
					}
					if (enm != null) {
						if (enm.value() == value) {
							return enm;
						} else {
							throw new io.qt.QNoSuchEnumValueException(value, name);
						}
					}
				}
			}
			try {
				E enm = resolveShortEnum(cl.hashCode(), cl, value, name);
				if (enm == null) {
					if (name == null)
						throw new QNoSuchEnumValueException(value);
					else
						throw new QNoSuchEnumValueException(value, name);
				}
				return enm;
			} catch (QNoSuchEnumValueException e) {
				throw e;
			} catch (Throwable e) {
				throw new QNoSuchEnumValueException(value, e);
			}
		}

		@Override
		public <E extends Enum<E> & QtLongEnumerator> E resolveEnum(Class<E> cl, long value, String name) {
			if (name != null) {
				if (name.isEmpty())
					name = null;
				else {
					E enm = null;
					try {
						enm = Enum.valueOf(cl, name);
					} catch (Exception e) {
					}
					if (enm != null) {
						if (enm.value() == value) {
							return enm;
						} else {
							throw new io.qt.QNoSuchEnumValueException(value, name);
						}
					}
				}
			}
			try {
				E enm = resolveLongEnum(cl.hashCode(), cl, value, name);
				if (enm == null) {
					if (name == null)
						throw new QNoSuchEnumValueException(value);
					else
						throw new QNoSuchEnumValueException(value, name);
				}
				return enm;
			} catch (QNoSuchEnumValueException e) {
				throw e;
			} catch (Throwable e) {
				throw new QNoSuchEnumValueException(value, e);
			}
		}
		


		/**
		 * This is an internal function. Calling it can have unexpected results.
		 *
		 * Disables garbage collection for this object. This should be used when objects
		 * created in java are passed to C++ functions that take ownership of the
		 * objects. Both the Java and C++ part of the object will then be cleaned up by
		 * C++.
		 */
		@Override
		public void setCppOwnership(QtObjectInterface object) {
			QtJambiInternal.setCppOwnership(nativeId(object));
		}

		/**
		 * This is an internal function. Calling it can have unexpected results.
		 *
		 * Forces Java ownership of both the Java object and its C++ resources. The C++
		 * resources will be cleaned up when the Java object is finalized.
		 */
		@Override
		public void setJavaOwnership(QtObjectInterface object) {
			QtJambiInternal.setJavaOwnership(nativeId(object));
		}

		/**
		 * This is an internal function. Calling it can have unexpected results.
		 *
		 * Reenables garbage collection for this object. Should be used on objects for
		 * which disableGarbageCollection() has previously been called. After calling
		 * this function, the object ownership will be reset to default.
		 */
		@Override
		public void setDefaultOwnership(QtObjectInterface object) {
			QtJambiInternal.setDefaultOwnership(nativeId(object));
		}

		/**
		 * This is an internal function. Calling it can have unexpected results.
		 *
		 * Disables garbage collection for this object. This should be used when objects
		 * created in java are passed to C++ functions that take ownership of the
		 * objects. Both the Java and C++ part of the object will then be cleaned up by
		 * C++.
		 */
		@Override
		public void setCppOwnership(QtObject object) {
			QtJambiInternal.setCppOwnership(nativeId(object));
		}

		/**
		 * This is an internal function. Calling it can have unexpected results.
		 *
		 * Forces Java ownership of both the Java object and its C++ resources. The C++
		 * resources will be cleaned up when the Java object is finalized.
		 */
		@Override
		public void setJavaOwnership(QtObject object) {
			QtJambiInternal.setJavaOwnership(nativeId(object));
		}

		/**
		 * This is an internal function. Calling it can have unexpected results.
		 *
		 * Reenables garbage collection for this object. Should be used on objects for
		 * which disableGarbageCollection() has previously been called. After calling
		 * this function, the object ownership will be reset to default.
		 */
		@Override
		public void setDefaultOwnership(QtObject object) {
			QtJambiInternal.setDefaultOwnership(nativeId(object));
		}

		@Override
		public void invalidateObject(QtObjectInterface object) {
			QtJambiInternal.invalidateObject(nativeId(object));
		}

		@Override
		public void invalidateObject(QtObject object) {
			QtJambiInternal.invalidateObject(nativeId(object));
		}

		@Override
		public boolean isJavaOwnership(QtObject object) {
			return QtJambiInternal.ownership(nativeId(object))==QtJambiInternal.Ownership.Java.value;
		}

		@Override
		public boolean isJavaOwnership(QtObjectInterface object) {
			return QtJambiInternal.ownership(nativeId(object))==QtJambiInternal.Ownership.Java.value;
		}

		@Override
		public boolean isSplitOwnership(QtObject object) {
			return QtJambiInternal.ownership(nativeId(object))==QtJambiInternal.Ownership.Split.value;
		}

		@Override
		public boolean isSplitOwnership(QtObjectInterface object) {
			return QtJambiInternal.ownership(nativeId(object))==QtJambiInternal.Ownership.Split.value;
		}

		@Override
		public boolean isCppOwnership(QtObject object) {
			return QtJambiInternal.ownership(nativeId(object))==QtJambiInternal.Ownership.Cpp.value;
		}

		@Override
		public boolean isCppOwnership(QtObjectInterface object) {
			return QtJambiInternal.ownership(nativeId(object))==QtJambiInternal.Ownership.Cpp.value;
		}

		@Override
		public long nativeId(QtObject object) {
			QtJambiObject obj = object;
			if (obj != null && obj.nativeLink != null) {
				synchronized (obj.nativeLink) {
					return obj.nativeLink.native__id;
				}
			}
			return 0;
		}

		@Override
		public long nativeId(QtObjectInterface object) {
			NativeLink nativeLink = findInterfaceLink(object, true);
			if (nativeLink != null) {
				synchronized (nativeLink) {
					return nativeLink.native__id;
				}
			}
			return 0;
		}

		@Override
		public long checkedNativeId(QtObject object) {
			if(object==null)
				return 0;
			try {
				long nid = nativeId(object);
				if (nid == 0) {
					QNoNativeResourcesException e = new QNoNativeResourcesException(
							"Function call on incomplete object of type: " + QtJambiInternal.getClass(object).getName());
					StackTraceElement[] st = e.getStackTrace();
					st = Arrays.copyOfRange(st, 1, st.length);
					e.setStackTrace(st);
					throw e;
				}
				return nid;
			} catch (NullPointerException e) {
				StackTraceElement[] st = e.getStackTrace();
				st = Arrays.copyOfRange(st, 1, st.length);
				e.setStackTrace(st);
				throw e;
			}
		}

		@Override
		public long checkedNativeId(QtObjectInterface object) {
			if(object==null)
				return 0;
			long nid = nativeId(object);
			if (nid == 0) {
				QNoNativeResourcesException e = new QNoNativeResourcesException(
						"Function call on incomplete object of type: " + QtJambiInternal.getClass(object).getName());
				StackTraceElement[] st = e.getStackTrace();
				st = Arrays.copyOfRange(st, 1, st.length);
				e.setStackTrace(st);
				throw e;
			}
			return nid;
		}

		@Override
		public void removeFromMapReferenceCount(QtObjectInterface owner,
				Class<? extends QtObjectInterface> declaringClass, String fieldName, boolean isStatic, Object value) {
			Object collection = null;
			boolean got = false;
			if (declaringClass.isInterface() && !isStatic) {
				NativeLink link = findInterfaceLink(owner, false);
				if (link instanceof InterfaceNativeLink) {
					collection = ((InterfaceNativeLink) link).getReferenceCountCollection(declaringClass, fieldName, null);
					got = true;
				}
			}
			if (!got) {
				Field field = null;
				try {
					field = declaringClass.getDeclaredField(fieldName);
				} catch (NoSuchFieldException | SecurityException e2) {
				}
				if (field == null && owner != null) {
					Class<?> objectClass = QtJambiInternal.getClass(owner);
					do {
						try {
							field = objectClass.getDeclaredField(fieldName);
						} catch (NoSuchFieldException | SecurityException e2) {
						}
						objectClass = objectClass.getSuperclass();
					} while (objectClass != null && field == null);
				}
				if (field != null) {
					collection = fetchField(owner, field);
				}
			}
			if (collection instanceof Map) {
				((Map<?, ?>) collection).remove(value);
			}
		}

		@Override
		public void removeFromCollectionReferenceCount(QtObjectInterface owner,
				Class<? extends QtObjectInterface> declaringClass, String fieldName, boolean isStatic, Object value) {
			Object collection = null;
			boolean got = false;
			if (declaringClass.isInterface() && !isStatic) {
				NativeLink link = findInterfaceLink(owner, false);
				if (link instanceof InterfaceNativeLink) {
					collection = ((InterfaceNativeLink) link).getReferenceCountCollection(declaringClass, fieldName, null);
					got = true;
				}
			}
			if (!got) {
				Field field = null;
				try {
					field = declaringClass.getDeclaredField(fieldName);
				} catch (NoSuchFieldException | SecurityException e2) {
				}
				if (field == null && owner != null) {
					Class<?> objectClass = QtJambiInternal.getClass(owner);
					do {
						try {
							field = objectClass.getDeclaredField(fieldName);
						} catch (NoSuchFieldException | SecurityException e2) {
						}
						objectClass = objectClass.getSuperclass();
					} while (objectClass != null && field == null);
				}
				if (field != null) {
					collection = fetchField(owner, field);
				}
			}
			if (collection instanceof Collection) {
				((Collection<?>) collection).remove(value);
			}
		}

		@SuppressWarnings("unchecked")
		@Override
		public void addAllReferenceCount(QtObjectInterface owner, Class<? extends QtObjectInterface> declaringClass,
				String fieldName, boolean isThreadSafe, boolean isStatic, Collection<?> values) {
			Object collection = null;
			boolean got = false;
			if (declaringClass.isInterface() && !isStatic) {
				NativeLink link = findInterfaceLink(owner, false);
				if (link instanceof InterfaceNativeLink) {
					collection = ((InterfaceNativeLink) link).getReferenceCountCollection(declaringClass, fieldName, RCList::new);
					got = true;
				}
			}
			if (!got) {
				Field field = null;
				try {
					field = declaringClass.getDeclaredField(fieldName);
				} catch (NoSuchFieldException | SecurityException e2) {
				}
				if (field == null && owner != null) {
					Class<?> objectClass = QtJambiInternal.getClass(owner);
					do {
						try {
							field = objectClass.getDeclaredField(fieldName);
						} catch (NoSuchFieldException | SecurityException e2) {
						}
						objectClass = objectClass.getSuperclass();
					} while (objectClass != null && field == null);
				}
				if (field != null) {
					collection = fetchField(owner, field);
					if (collection == null) {
						if (isThreadSafe) {
							collection = java.util.Collections.synchronizedList(new java.util.ArrayList<>());
						} else {
							collection = new java.util.ArrayList<>();
						}
						setField(owner, field, collection);
					}
				}
			}
			if (collection instanceof Collection) {
				((Collection<Object>) collection).addAll(values);
			}
		}

		@SuppressWarnings("unchecked")
		@Override
		public void putReferenceCount(QtObjectInterface owner, Class<? extends QtObjectInterface> declaringClass,
				String fieldName, boolean isThreadSafe, boolean isStatic, Object key, Object value) {
			Object map = null;
			boolean got = false;
			if (declaringClass.isInterface() && !isStatic) {
				NativeLink link = findInterfaceLink(owner, false);
				if (link instanceof InterfaceNativeLink) {
					map = ((InterfaceNativeLink) link).getReferenceCountCollection(declaringClass, fieldName, () -> {
						if (isThreadSafe) {
							return java.util.Collections.synchronizedMap(new RCMap());
						} else {
							return new RCMap();
						}
					});
					got = true;
				}
			}
			if (!got) {
				Field field = null;
				try {
					field = declaringClass.getDeclaredField(fieldName);
				} catch (NoSuchFieldException | SecurityException e2) {
				}
				if (field == null && owner != null) {
					Class<?> objectClass = QtJambiInternal.getClass(owner);
					do {
						try {
							field = objectClass.getDeclaredField(fieldName);
						} catch (NoSuchFieldException | SecurityException e2) {
						}
						objectClass = objectClass.getSuperclass();
					} while (objectClass != null && field == null);
				}
				if (field != null) {
					map = fetchField(owner, field);
					if (map == null) {
						if (isThreadSafe) {
							map = java.util.Collections.synchronizedMap(new java.util.HashMap<>());
						} else {
							map = new java.util.HashMap<>();
						}
						setField(owner, field, map);
					}
				}
			}
			if (map instanceof Map) {
				((Map<Object, Object>) map).put(key, value);
			}
		}

		@Override
		public void clearReferenceCount(QtObjectInterface owner, Class<? extends QtObjectInterface> declaringClass,
				String fieldName, boolean isStatic) {
			Object collection = null;
			boolean got = false;
			if (declaringClass.isInterface() && !isStatic) {
				NativeLink link = findInterfaceLink(owner, false);
				if (link instanceof InterfaceNativeLink) {
					collection = ((InterfaceNativeLink) link).getReferenceCountCollection(declaringClass, fieldName, null);
					got = true;
				}
			}
			if (!got) {
				Field field = null;
				try {
					field = declaringClass.getDeclaredField(fieldName);
				} catch (NoSuchFieldException | SecurityException e2) {
				}
				if (field == null && owner != null) {
					Class<?> objectClass = QtJambiInternal.getClass(owner);
					do {
						try {
							field = objectClass.getDeclaredField(fieldName);
						} catch (NoSuchFieldException | SecurityException e2) {
						}
						objectClass = objectClass.getSuperclass();
					} while (objectClass != null && field == null);
				}
				if (field != null) {
					collection = fetchField(owner, field);
				}
			}
			if (collection instanceof Map) {
				((Map<?, ?>) collection).clear();
			} else if (collection instanceof Collection) {
				((Collection<?>) collection).clear();
			}
		}

		@SuppressWarnings("unchecked")
		@Override
		public void addReferenceCount(QtObjectInterface owner, Class<? extends QtObjectInterface> declaringClass,
				String fieldName, boolean isThreadSafe, boolean isStatic, Object value) {
			Object collection = null;
			boolean got = false;
			if (declaringClass.isInterface() && !isStatic) {
				NativeLink link = findInterfaceLink(owner, false);
				if (link instanceof InterfaceNativeLink) {
					collection = ((InterfaceNativeLink) link).getReferenceCountCollection(declaringClass, fieldName, RCList::new);
					got = true;
				}
			}
			if (!got) {
				Field field = null;
				try {
					field = declaringClass.getDeclaredField(fieldName);
				} catch (NoSuchFieldException | SecurityException e2) {
				}
				if (field == null && owner != null) {
					Class<?> objectClass = QtJambiInternal.getClass(owner);
					do {
						try {
							field = objectClass.getDeclaredField(fieldName);
						} catch (NoSuchFieldException | SecurityException e2) {
						}
						objectClass = objectClass.getSuperclass();
					} while (objectClass != null && field == null);
				}
				if (field != null) {
					collection = fetchField(owner, field);
					if (collection == null) {
						if (isThreadSafe) {
							collection = java.util.Collections.synchronizedList(new java.util.ArrayList<>());
						} else {
							collection = new java.util.ArrayList<>();
						}
						setField(owner, field, collection);
					}
				}
			}
			if (collection instanceof Collection) {
				((Collection<Object>) collection).add(value);
			}
		}
		
		@Override
		public void setReferenceCount(QtObjectInterface owner, Class<? extends QtObjectInterface> declaringClass,
				String fieldName, boolean threadSafe, boolean isStatic, Object newValue) {
			if (threadSafe) {
				synchronized (isStatic ? declaringClass : owner) {
					setReferenceCount(owner, declaringClass, fieldName, isStatic, newValue);
				}
			} else {
				setReferenceCount(owner, declaringClass, fieldName, isStatic, newValue);
			}
		}

		public void setReferenceCount(QtObjectInterface owner, Class<? extends QtObjectInterface> declaringClass,
				String fieldName, boolean isStatic, Object newValue) {
			if (declaringClass.isInterface() && !isStatic) {
				NativeLink link = findInterfaceLink(owner, true);
				if (link instanceof InterfaceNativeLink) {
					((InterfaceNativeLink) link).setReferenceCount(declaringClass, fieldName, newValue);
					return;
				}
			}
			Field field = null;
			try {
				field = declaringClass.getDeclaredField(fieldName);
			} catch (NoSuchFieldException | SecurityException e2) {
			}
			if (field == null && owner != null) {
				Class<?> objectClass = QtJambiInternal.getClass(owner);
				do {
					try {
						field = objectClass.getDeclaredField(fieldName);
					} catch (NoSuchFieldException | SecurityException e2) {
					}
					objectClass = objectClass.getSuperclass();
				} while (objectClass != null && field == null);
			}
			if (field != null) {
				setField(owner, field, newValue);
			}
		}

		@Override
		public Supplier<Class<?>> callerClassProvider() {
			return RetroHelper.callerClassProvider();
		}

		@Override
		public Map<Object, Object> newRCMap() {
			return new RCMap();
		}

		@Override
		public List<Object> newRCList() {
			return new RCList();
		}

		@Override
		public void registerDependentObject(QtObjectInterface dependentObject, QtObjectInterface owner) {
			QtJambiInternal.registerDependentObject(nativeId(dependentObject), nativeId(owner));
		}

		public void registerDependentObject(QtObject dependentObject, QtObjectInterface owner) {
			QtJambiInternal.registerDependentObject(nativeId(dependentObject), nativeId(owner));
		}

		@Override
		public void registerDependentObject(QtObjectInterface dependentObject, QtObject owner) {
			QtJambiInternal.registerDependentObject(nativeId(dependentObject), nativeId(owner));
		}

		@Override
		public void registerDependentObject(QtObject dependentObject, QtObject owner) {
			QtJambiInternal.registerDependentObject(nativeId(dependentObject), nativeId(owner));
		}

		@Override
		public void unregisterDependentObject(QtObjectInterface dependentObject, QtObjectInterface owner) {
			QtJambiInternal.unregisterDependentObject(nativeId(dependentObject), nativeId(owner));
		}

		@Override
		public void unregisterDependentObject(QtObject dependentObject, QtObjectInterface owner) {
			QtJambiInternal.unregisterDependentObject(nativeId(dependentObject), nativeId(owner));
		}

		@Override
		public void unregisterDependentObject(QtObjectInterface dependentObject, QtObject owner) {
			QtJambiInternal.unregisterDependentObject(nativeId(dependentObject), nativeId(owner));
		}

		@Override
		public void unregisterDependentObject(QtObject dependentObject, QtObject owner) {
			QtJambiInternal.unregisterDependentObject(nativeId(dependentObject), nativeId(owner));
		}

		@Override
		public InternalAccess.Cleanable registerCleaner(Object object, Runnable action) {
			synchronized (cleaners) {
				Cleaner cleanable = new Cleaner(object);
				cleaners.put(cleanable, action);
				return cleanable;
			}
		}

		@Override
		public <S extends java.io.Serializable> io.qt.core.QObject lambdaContext(S lambdaExpression) {
			if(lambdaExpression instanceof AbstractSignal) {
				QtSignalEmitterInterface containingObject = ((AbstractSignal) lambdaExpression).containingObject();
				if(containingObject instanceof QObject)
					return (QObject)containingObject;
			}
			LambdaInfo lambdaInfo = lambdaInfo(lambdaExpression);
			if (lambdaInfo != null) {
				if(lambdaInfo.owner instanceof AbstractSignal) {
					QtSignalEmitterInterface containingObject = ((AbstractSignal) lambdaInfo.owner).containingObject();
					if(containingObject instanceof QObject)
						return (QObject)containingObject;
				}
				return lambdaInfo.qobject;
			} else {
				return null;
			}
		}

		@Override
		public <S extends java.io.Serializable> Class<?> lambdaReturnType(Class<S> type, S lambdaExpression) {
			LambdaInfo lamdaInfo = lambdaInfo(lambdaExpression);
			if (lamdaInfo!=null && lamdaInfo.methodHandle != null) {
				return lamdaInfo.methodHandle.type().returnType();
			} else {
				Class<?> objectType = QtJambiInternal.getClass(lambdaExpression);
				if(type.isInterface() && !objectType.isSynthetic()) {
					Method functionalMethod = null;
					for(Method method : type.getMethods()) {
						if(!method.isDefault() && method.getDeclaringClass()!=Object.class) {
							functionalMethod = method;
							break;
						}
					}
					if(functionalMethod!=null) {
						try {
							Method mtd = objectType.getMethod(functionalMethod.getName(), functionalMethod.getParameterTypes());
							if(mtd==null || mtd.isBridge() || mtd.isDefault() || Modifier.isAbstract(mtd.getModifiers())) {
								for(Method _mtd : objectType.getMethods()) {
									if(!_mtd.isBridge()
											&& !_mtd.isDefault()
											&& _mtd.getName().equals(functionalMethod.getName())
											&& _mtd.getParameterCount()==functionalMethod.getParameterCount()) {
										mtd = _mtd;
										break;
									}
								}
							}
							if(mtd!=null && !mtd.isBridge() && !mtd.isDefault() && !Modifier.isAbstract(mtd.getModifiers()))
								return mtd.getReturnType();
						} catch (Exception e) {
						}
					}
				}
				return null;
			}
		}
		
		@Override
		public <S extends java.io.Serializable> int[] lambdaMetaTypes(Class<S> type, S lambdaExpression) {
			LambdaInfo lamdaInfo = lambdaInfo(lambdaExpression);
			if (lamdaInfo!=null && lamdaInfo.reflectiveMethod != null) {
				int[] metaTypes = new int[1+lamdaInfo.reflectiveMethod.getParameterCount()];
				AnnotatedElement rt = null;
				if(useAnnotatedType)
					rt = lamdaInfo.reflectiveMethod.getAnnotatedReturnType();
				metaTypes[0] = QtJambiInternal.registerMetaType(lamdaInfo.reflectiveMethod.getReturnType(), lamdaInfo.reflectiveMethod.getGenericReturnType(), rt, false, false);
				Parameter[] parameters = lamdaInfo.reflectiveMethod.getParameters();
				for (int i = 0; i < parameters.length; i++) {
					metaTypes[i+1] = registerMetaType(parameters[i]);
				}
				return metaTypes;
			}else {
				Class<?> objectType = QtJambiInternal.getClass(lambdaExpression);
				if(type.isInterface() && !objectType.isSynthetic()) {
					Method functionalMethod = null;
					for(Method method : type.getMethods()) {
						if(!method.isDefault() && method.getDeclaringClass()!=Object.class) {
							functionalMethod = method;
							break;
						}
					}
					if(functionalMethod!=null) {
						try {
							Method mtd = objectType.getMethod(functionalMethod.getName(), functionalMethod.getParameterTypes());
							if(mtd==null || mtd.isBridge() || mtd.isDefault() || Modifier.isAbstract(mtd.getModifiers())) {
								for(Method _mtd : objectType.getMethods()) {
									if(!_mtd.isBridge()
											&& !_mtd.isDefault()
											&& _mtd.getName().equals(functionalMethod.getName())
											&& _mtd.getParameterCount()==functionalMethod.getParameterCount()) {
										mtd = _mtd;
										break;
									}
								}
							}
							if(mtd!=null && !mtd.isBridge() && !mtd.isDefault() && !Modifier.isAbstract(mtd.getModifiers())) {
								int[] metaTypes = new int[mtd.getParameterCount()+1];
								if(mtd.getReturnType()==void.class) {
									metaTypes[0] = QMetaType.Type.Void.value();
								}else {
									AnnotatedElement ae = null;
									if(useAnnotatedType) {
										ae = mtd.getAnnotatedReturnType();
									}
									metaTypes[0] = QtJambiInternal.registerMetaType(mtd.getReturnType(), mtd.getGenericReturnType(), ae, false, false);
								}
								Parameter[] params = mtd.getParameters();
								for (int i = 0; i < params.length; i++) {
									metaTypes[i+1] = registerMetaType(params[i]);
								}
								return metaTypes;
							}
						} catch (Exception e) {
						}
					}
				}
				return null;
			}
		}
		
		@Override
		public <S extends java.io.Serializable> Class<?>[] lambdaClassTypes(Class<S> type, S lambdaExpression) {
			LambdaInfo lamdaInfo = lambdaInfo(lambdaExpression);
			if (lamdaInfo!=null && lamdaInfo.reflectiveMethod != null) {
				Class<?>[] classTypes = new Class[1+lamdaInfo.reflectiveMethod.getParameterCount()];
				classTypes[0] = lamdaInfo.reflectiveMethod.getReturnType();
				Parameter[] parameters = lamdaInfo.reflectiveMethod.getParameters();
				for (int i = 0; i < parameters.length; i++) {
					classTypes[i+1] = parameters[i].getType();
				}
				return classTypes;
			}else {
				Class<?> objectType = QtJambiInternal.getClass(lambdaExpression);
				if(type.isInterface() && !objectType.isSynthetic()) {
					Method functionalMethod = null;
					for(Method method : type.getMethods()) {
						if(!method.isDefault() && method.getDeclaringClass()!=Object.class) {
							functionalMethod = method;
							break;
						}
					}
					if(functionalMethod!=null) {
						try {
							Method mtd = objectType.getMethod(functionalMethod.getName(), functionalMethod.getParameterTypes());
							if(mtd==null || mtd.isBridge() || mtd.isDefault() || Modifier.isAbstract(mtd.getModifiers())) {
								for(Method _mtd : objectType.getMethods()) {
									if(!_mtd.isBridge()
											&& !_mtd.isDefault()
											&& _mtd.getName().equals(functionalMethod.getName())
											&& _mtd.getParameterCount()==functionalMethod.getParameterCount()) {
										mtd = _mtd;
										break;
									}
								}
							}
							if(mtd!=null && !mtd.isBridge() && !mtd.isDefault() && !Modifier.isAbstract(mtd.getModifiers())) {
								Class<?>[] metaTypes = new Class[mtd.getParameterCount()+1];
								if(mtd.getReturnType()==void.class) {
									metaTypes[0] = void.class;
								}else {
									metaTypes[0] = mtd.getReturnType();
								}
								Parameter[] params = mtd.getParameters();
								for (int i = 0; i < params.length; i++) {
									metaTypes[i+1] = params[i].getType();
								}
								return metaTypes;
							}
						} catch (Exception e) {
						}
					}
				}
				return null;
			}
		}
		
		public <S extends java.io.Serializable> java.lang.reflect.Executable lambdaExecutable(S lambdaExpression){
			LambdaInfo lamdaInfo = lambdaInfo(lambdaExpression);
			if (lamdaInfo!=null) {
				if(lamdaInfo.reflectiveMethod != null)
					return lamdaInfo.reflectiveMethod;
				else
					return lamdaInfo.reflectiveConstructor;
			}else 
				return null;
		}
		
		@Override
		public Class<?> findGeneratedSuperclass(Class<?> clazz){
			return QtJambiInternal.findGeneratedSuperclass(clazz);
		}

		@Override
		public Supplier<CallerContext> callerContextProvider() {
			return RetroHelper.callerContextProvider();
		}
		
		@Override
		public QObject owner(QtObjectInterface object) {
			return QtJambiInternal.owner(nativeId(object));
		}

		@Override
		public boolean hasOwnerFunction(QtObjectInterface object) {
			return QtJambiInternal.hasOwnerFunction(nativeId(object));
		}
		
		@Override
		public QObject owner(QtObject object) {
			return QtJambiInternal.owner(nativeId(object));
		}

		@Override
		public boolean hasOwnerFunction(QtObject object) {
			return QtJambiInternal.hasOwnerFunction(nativeId(object));
		}
		
		@Override
		public <Q extends QtObjectInterface,M> M findMemberAccess(Q ifc, Class<Q> interfaceClass, Class<M> accessClass) {
			QtJambiInternal.NativeLink link = QtJambiInternal.findInterfaceLink(ifc, true);
			return accessClass.cast(link.getMemberAccess(interfaceClass));
		}

		@Override
		public <T> Class<T> getClass(T object) {
			return QtJambiInternal.getClass(object);
		}

		@Override
		public int registerMetaType(Parameter parameter) {
			return QtJambiInternal.registerMetaType(parameter);
		}

		@Override
		public <T> Supplier<T> getFactory0(Constructor<T> constructor) {
			return QtJambiInternal.getFactory0(constructor);
		}

		@Override
		public Package getDefinedPackage(ClassLoader cl, String pkg) {
			return RetroHelper.getDefinedPackage(cl, pkg);
		}
	};
}
