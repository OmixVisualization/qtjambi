/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2021 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
import java.lang.reflect.AnnotatedArrayType;
import java.lang.reflect.AnnotatedParameterizedType;
import java.lang.reflect.AnnotatedType;
import java.lang.reflect.Constructor;
import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.lang.reflect.Modifier;
import java.lang.reflect.Parameter;
import java.lang.reflect.ParameterizedType;
import java.lang.reflect.Type;
import java.lang.reflect.TypeVariable;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLClassLoader;
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
import java.util.function.Function;
import java.util.function.IntFunction;
import java.util.function.Supplier;
import java.util.jar.JarFile;
import java.util.logging.Logger;
import java.util.zip.ZipEntry;

import io.qt.QFlags;
import io.qt.QMissingVirtualOverridingException;
import io.qt.QNoNativeResourcesException;
import io.qt.QNoSuchEnumValueException;
import io.qt.QNonVirtualOverridingException;
import io.qt.QtAbstractFlagEnumerator;
import io.qt.QtByteEnumerator;
import io.qt.QtByteFlagEnumerator;
import io.qt.QtDeclaredFinal;
import io.qt.QtEnumerator;
import io.qt.QtExtensibleEnum;
import io.qt.QtFinalOverride;
import io.qt.QtFlagEnumerator;
import io.qt.QtLongEnumerator;
import io.qt.QtLongFlagEnumerator;
import io.qt.QtObject;
import io.qt.QtObjectInterface;
import io.qt.QtPointerType;
import io.qt.QtPrimitiveType;
import io.qt.QtReferenceType;
import io.qt.QtResources;
import io.qt.QtShortEnumerator;
import io.qt.QtShortFlagEnumerator;
import io.qt.QtSignalEmitterInterface;
import io.qt.core.QByteArray;
import io.qt.core.QCoreApplication;
import io.qt.core.QDataStream;
import io.qt.core.QDeclarableSignals;
import io.qt.core.QDir;
import io.qt.core.QFile;
import io.qt.core.QFileInfo;
import io.qt.core.QHash;
import io.qt.core.QIODevice;
import io.qt.core.QJsonDocument;
import io.qt.core.QJsonDocument.FromJsonResult;
import io.qt.core.QJsonObject;
import io.qt.core.QJsonParseError.ParseError;
import io.qt.core.QJsonValue;
import io.qt.core.QList;
import io.qt.core.QMap;
import io.qt.core.QMetaMethod;
import io.qt.core.QMetaObject;
import io.qt.core.QMetaType;
import io.qt.core.QMultiHash;
import io.qt.core.QMultiMap;
import io.qt.core.QObject;
import io.qt.core.QPair;
import io.qt.core.QQueue;
import io.qt.core.QSet;
import io.qt.core.QStack;
import io.qt.core.QStaticMemberSignals;
import io.qt.core.QStringList;
import io.qt.core.QThread;
import io.qt.core.Qt;
import io.qt.core.internal.QtPluginMetaData;
import io.qt.internal.QtJambiSignals.AbstractSignal;
import io.qt.internal.QtJambiSignals.SignalParameterType;

public final class QtJambiInternal {

	private QtJambiInternal() {
		throw new RuntimeException();
	}

	public static class RCList extends ArrayList<Object> {
		@NativeAccess
		public RCList() {
			super();
		}

		private static final long serialVersionUID = -4010060446825990721L;

		private void check() {
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
				if(!needsReferenceCounting((QtObjectInterface)e))
					return false;
				boolean result = super.add(e);
				if (result) {
					QMetaObject.DisposedSignal disposed = getSignalOnDispose((QtObjectInterface) e, true);
					if (disposed != null)
						disposed.connect(this::check);
				}
				return result;
			}else {
				return super.add(e);
			}
		}

		@Override
		@NativeAccess
		public boolean remove(Object o) {
			boolean result = super.remove(o);
			if (result && o instanceof QtObjectInterface) {
				QMetaObject.DisposedSignal disposed = getSignalOnDispose((QtObjectInterface) o, true);
				if (disposed != null)
					disposed.disconnect(this::check);
			}
			return result;
		}

		@Override
		@NativeAccess
		public void clear() {
			for (Object o : this) {
				if (o instanceof QtObjectInterface) {
					QMetaObject.DisposedSignal disposed = getSignalOnDispose((QtObjectInterface) o, true);
					if (disposed != null)
						disposed.disconnect(this::check);
				}
			}
			super.clear();
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
	
	public static class RCSet extends HashSet<Object> {
		@NativeAccess
		public RCSet() {
			super();
		}

		private static final long serialVersionUID = -4010060446825990721L;

		private void check() {
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
				if(!needsReferenceCounting((QtObjectInterface)e))
					return false;
				boolean result = super.add(e);
				if (result) {
					QMetaObject.DisposedSignal disposed = getSignalOnDispose((QtObjectInterface) e, true);
					if (disposed != null)
						disposed.connect(this::check);
				}
				return result;
			}else {
				return super.add(e);
			}
		}

		@Override
		@NativeAccess
		public boolean remove(Object o) {
			boolean result = super.remove(o);
			if (result && o instanceof QtObjectInterface) {
				QMetaObject.DisposedSignal disposed = getSignalOnDispose((QtObjectInterface) o, true);
				if (disposed != null)
					disposed.disconnect(this::check);
			}
			return result;
		}

		@Override
		@NativeAccess
		public void clear() {
			for (Object o : this) {
				if (o instanceof QtObjectInterface) {
					QMetaObject.DisposedSignal disposed = getSignalOnDispose((QtObjectInterface) o, true);
					if (disposed != null)
						disposed.disconnect(this::check);
				}
			}
			super.clear();
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

	public static class RCMap extends IdentityHashMap<Object, Object> {
		private static final long serialVersionUID = 3076251074218500284L;

		@NativeAccess
		public RCMap() {
			super();
		}
		
		private void check() {
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
					if(!needsReferenceCounting((QtObjectInterface)key) && !needsReferenceCounting((QtObjectInterface)value))
						return false;
				}else if (key instanceof QtObjectInterface) {
					if(!needsReferenceCounting((QtObjectInterface)key))
						return false;
				}else {
					if(!needsReferenceCounting((QtObjectInterface)value))
						return false;
				}
			}

			Object result = super.put(key, value);
			if (key instanceof QtObjectInterface) {
				QMetaObject.DisposedSignal disposed = getSignalOnDispose((QtObjectInterface) key, true);
				if (disposed != null)
					disposed.connect(this::check);
			}
			if (value instanceof QtObjectInterface) {
				QMetaObject.DisposedSignal disposed = getSignalOnDispose((QtObjectInterface) value, true);
				if (disposed != null)
					disposed.connect(this::check);
			}
			if (result instanceof QtObjectInterface) {
				QMetaObject.DisposedSignal disposed = getSignalOnDispose((QtObjectInterface) result, true);
				if (disposed != null)
					disposed.disconnect(this::check);
			}
			return result;
		}

		@Override
		public Object remove(Object key) {
			Object result = super.remove(key);
			if (key instanceof QtObjectInterface) {
				QMetaObject.DisposedSignal disposed = getSignalOnDispose((QtObjectInterface) key, true);
				if (disposed != null)
					disposed.disconnect(this::check);
			}
			if (result instanceof QtObjectInterface) {
				QMetaObject.DisposedSignal disposed = getSignalOnDispose((QtObjectInterface) result, true);
				if (disposed != null)
					disposed.disconnect(this::check);
			}
			return result;
		}

		@Override
		public void clear() {
			for (Entry<Object, Object> entry : this.entrySet()) {
				if (entry.getKey() instanceof QtObjectInterface) {
					QMetaObject.DisposedSignal disposed = getSignalOnDispose((QtObjectInterface) entry.getKey(), true);
					if (disposed != null)
						disposed.disconnect(this::check);
				}
				if (entry.getValue() instanceof QtObjectInterface) {
					QMetaObject.DisposedSignal disposed = getSignalOnDispose((QtObjectInterface) entry.getValue(),
							true);
					if (disposed != null)
						disposed.disconnect(this::check);
				}
			}
			super.clear();
		}

		@Override
		public boolean remove(Object key, Object value) {
			boolean result = super.remove(key, value);
			if (result) {
				if (key instanceof QtObjectInterface) {
					QMetaObject.DisposedSignal disposed = getSignalOnDispose((QtObjectInterface) key, true);
					if (disposed != null)
						disposed.disconnect(this::check);
				}
				if (value instanceof QtObjectInterface) {
					QMetaObject.DisposedSignal disposed = getSignalOnDispose((QtObjectInterface) value, true);
					if (disposed != null)
						disposed.disconnect(this::check);
				}
			}
			return result;
		}
	}

	private static class PluginClassLoader extends URLClassLoader {
		public PluginClassLoader() {
			super(new URL[0]);
		}

		protected void addURLs(Collection<URL> urls) {
			for (URL url : urls) {
				super.addURL(url);
				QtResources.addSearchPath(url.toString());
			}
		}
	}

	private static final Map<NativeLink, QMetaObject.DisposedSignal> disposedSignals;
	private static Function<Class<?>, QMetaObject.DisposedSignal> disposedSignalFactory;
	private static final Map<Integer, InterfaceNativeLink> interfaceLinks;
//    private static final Map<Class<? extends QtObjectInterface>, Function<QtObjectInterface, NativeLink>> nativeLinkSuppliers;
	private static final Map<Class<? extends QtAbstractFlagEnumerator>, MethodHandle> flagsConstructorsByEnumType;
	private static final Map<Class<? extends QtAbstractFlagEnumerator>, QtAbstractFlagEnumerator[]> flagsConstants;
	private static final Map<Class<?>, Boolean> isClassGenerated;
	private static final Map<String, Boolean> initializedPackages;
	private static final Map<Class<?>, List<Class<? extends QtObjectInterface>>> implementingInterfaces;
	private static final Map<Class<?>, List<Class<? extends QtObjectInterface>>> allImplementingInterfaces;
	private static final Map<Integer, Method> lookupSlots;
	private static final Map<Class<?>, MethodHandle> lambdaWriteReplaceHandles;
	private static final Map<QPair<Class<? extends QtObjectInterface>, Class<?>>, Check> checkedClasses;
	private static final Map<Class<?>, MethodHandle> lambdaSlotHandles;
	static final ReferenceQueue<Object> referenceQueue = new ReferenceQueue<>();
	private static final Thread cleanupRegistrationThread;
	private static boolean noJarPlugins = Boolean.parseBoolean(System.getProperty("io.qt.no-plugins", "false"));
	private static final Set<String> analyzedPaths = new HashSet<>();
	private static final PluginClassLoader pluginClassLoader = new PluginClassLoader();
	static {
		interfaceLinks = Collections.synchronizedMap(new HashMap<>());
		disposedSignals = Collections.synchronizedMap(new HashMap<>());
//    	nativeLinkSuppliers = Collections.synchronizedMap(new HashMap<>());
		flagsConstructorsByEnumType = Collections.synchronizedMap(new HashMap<>());
		flagsConstants = Collections.synchronizedMap(new HashMap<>());
		isClassGenerated = Collections.synchronizedMap(new HashMap<>());
		initializedPackages = Collections.synchronizedMap(new HashMap<>());
		initializedPackages.put("io.qt.internal", Boolean.TRUE);
		implementingInterfaces = new HashMap<>();
		allImplementingInterfaces = new HashMap<>();
		lookupSlots = Collections.synchronizedMap(new HashMap<>());
		lambdaWriteReplaceHandles = Collections.synchronizedMap(new HashMap<>());
		checkedClasses = Collections.synchronizedMap(new HashMap<>());
		lambdaSlotHandles = Collections.synchronizedMap(new HashMap<>());

		QtJambi_LibraryInitializer.init();
		cleanupRegistrationThread = new Thread(() -> {
			while (true) {
				try {
					Reference<?> ref = referenceQueue.remove();
					try {
						if (ref instanceof Cleanable) {
							try {
								((Cleanable) ref).clean();
							} catch (Throwable e) {
								e.printStackTrace();
							}
						}
					} finally {
						ref = null;
					}
				} catch (InterruptedException e) {
					break;
				} catch (Throwable e) {
					e.printStackTrace();
				}
			}
		});
		cleanupRegistrationThread.setName("QtJambiCleanupThread");
		cleanupRegistrationThread.setDaemon(true);
		cleanupRegistrationThread.start();
	}

	@NativeAccess
	public static void shutdown() {
		switch (cleanupRegistrationThread.getState()) {
		case TERMINATED:
			break;
		default:
			cleanupRegistrationThread.interrupt();
			try {
				cleanupRegistrationThread.join();
			} catch (InterruptedException e) {
				break;
			} catch (Throwable e) {
				e.printStackTrace();
			}
			break;
		}
	}

	private static final Map<Cleaner, Runnable> cleaners = new HashMap<>();

	public static interface Cleanable {
		public void clean();
	}

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

	public static Cleanable registerCleaner(Object obj, Runnable action) {
		synchronized (cleaners) {
			Cleaner cleanable = new Cleaner(obj);
			cleaners.put(cleanable, action);
			return cleanable;
		}
	}

	private static Boolean matchBooleanProperty(String name, Boolean ifNotExistsValue, Boolean ifExistsValue,
			String[] falseA, String[] trueA) {
		String value = System.getProperty(name);
		if ("yes".equalsIgnoreCase(value)) {
			return Boolean.TRUE;
		} else if ("true".equalsIgnoreCase(value)) {
			return Boolean.TRUE;
		} else if ("no".equalsIgnoreCase(value)) {
			return Boolean.FALSE;
		}
		return Boolean.parseBoolean(value);
	}

	public static final char SlotPrefix = '1';
	public static final char SignalPrefix = '2';

	static byte[] resolveConversionSchema(Class<?> outputParameterTypes[]) {
		byte returned[] = new byte[outputParameterTypes.length];
		for (int i = 0; i < returned.length; ++i) {
			returned[i] = typeConversionCode(outputParameterTypes[i]);
		}

		return returned;
	}

	static byte typeConversionCode(Class<?> cls) {
		if (cls.isPrimitive()) {
			if (cls == int.class) {
				return 'I';
			} else if (cls == long.class) {
				return 'J';
			} else if (cls == short.class) {
				return 'S';
			} else if (cls == boolean.class) {
				return 'Z';
			} else if (cls == byte.class) {
				return 'B';
			} else if (cls == float.class) {
				return 'F';
			} else if (cls == double.class) {
				return 'D';
			} else if (cls == char.class) {
				return 'C';
			} else if (cls == void.class) {
				return 'V';
			} else {
				throw new RuntimeException("Error in conversion to primitive for complex type " + cls.getName());
			}
		}else return 'L';
	}

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

	@NativeAccess
	static QtJambiSignals.AbstractSignal lookupSignal(QtSignalEmitterInterface signalEmitter, String name,
			Class<?>[] requiredParameters) {
		try {
			if (name == null || signalEmitter == null) {
				java.util.logging.Logger.getLogger("io.qt.internal").log(java.util.logging.Level.WARNING,
						"lookupSignal: Name or object is null");
				return null;
			}

			String parameters;
			if (requiredParameters != null) {
				return findSignal((QtSignalEmitterInterface) signalEmitter, name, requiredParameters);
			} else {
				parameters = "";
				int idx = name.indexOf("(");
				if (idx >= 0) {
					parameters = name.substring(idx).trim();
					name = name.substring(0, idx);
				}
			}

			String[] argumentTypes = {};
			if (parameters != null && !parameters.isEmpty()) {
				if (parameters.startsWith("(") && parameters.endsWith(")")) {
					parameters = parameters.substring(1, parameters.length() - 1).trim();
					if (!parameters.isEmpty()) {
						argumentTypes = parameters.split("\\,");
						for (int i = 0; i < argumentTypes.length; ++i)
							argumentTypes[i] = argumentTypes[i].replace(" ", "");
					}
				}
			}

			Set<AbstractSignal> visitedSignals = new HashSet<>();

			for (Class<?> cls = signalEmitter.getClass(); QtSignalEmitterInterface.class
					.isAssignableFrom(cls); cls = cls.getSuperclass()) {

				try {
					Field f = cls.getDeclaredField(name);
					if (!java.lang.reflect.Modifier.isStatic(f.getModifiers())) {
						List<? extends QtJambiSignals.AbstractSignal> signals = Collections.emptyList();
						if (QtJambiSignals.AbstractSignal.class.isAssignableFrom(f.getType())) {
							signals = Collections
									.singletonList((QtJambiSignals.AbstractSignal) fetchField(signalEmitter, f));
						} else if (QtJambiSignals.MultiSignal.class.isAssignableFrom(f.getType())) {
							QtJambiSignals.MultiSignal multiSignal = (QtJambiSignals.MultiSignal) fetchField(
									signalEmitter, f);
							signals = multiSignal.signals();
						}
						visitedSignals.addAll(signals);
						for (QtJambiSignals.AbstractSignal signal : signals) {
							if (argumentTypes.length == 0 && signal.signalTypes().isEmpty()) {
								return signal;
							} else if (argumentTypes.length == signal.signalTypes().size()) {
								List<SignalParameterType> signalTypes = signal.signalTypes();

								boolean matches = true;
								for (int i = 0; i < signalTypes.size(); ++i) {
									Class<?> t = signalTypes.get(i).type;
									String arg = t.getName();

									if (t.isArray()) {
										String brackets = "";

										do {
											t = t.getComponentType();
											brackets += "[]";
										} while (t.isArray());

										arg = t.getName() + brackets;
									}

									if (argumentTypes[i].indexOf('.') < 0) {
										arg = arg.substring(arg.lastIndexOf('.') + 1);
									}

									String argLenient = t.isMemberClass() ? arg.replace('$', '.') : null;
									if (!arg.equals(argumentTypes[i])
											&& (argLenient == null || !argLenient.equals(argumentTypes[i]))) {
										matches = false;
										break;
									}
								}
								if (matches) {
									return signal;
								}
							}
						}
					}
				} catch (NoSuchFieldException | SecurityException t) {
				}
			}

			if (signalEmitter instanceof QObject) {
				List<AbstractSignal> listOfExtraSignal = getListOfExtraSignal(
						io.qt.internal.QtJambiInternal.nativeId((QObject) signalEmitter));
				visitedSignals.addAll(listOfExtraSignal);
				for (AbstractSignal signal : listOfExtraSignal) {
					if (signal.name().equals(name)) {
						if (argumentTypes.length == 0 && signal.signalTypes().isEmpty()) {
							return signal;
						} else if (argumentTypes.length == signal.signalTypes().size()) {
							List<SignalParameterType> signalTypes = signal.signalTypes();

							boolean matches = true;
							for (int i = 0; i < signalTypes.size(); ++i) {
								Class<?> t = signalTypes.get(i).type;
								String arg = t.getName();

								if (t.isArray()) {
									String brackets = "";

									do {
										t = t.getComponentType();
										brackets += "[]";
									} while (t.isArray());

									arg = t.getName() + brackets;
								}

								if (argumentTypes[i].indexOf('.') < 0) {
									arg = arg.substring(arg.lastIndexOf('.') + 1);
								}

								String argLenient = t.isMemberClass() ? arg.replace('$', '.') : null;
								if (!arg.equals(argumentTypes[i])
										&& (argLenient == null || !argLenient.equals(argumentTypes[i]))) {
									matches = false;
									break;
								}
							}
							if (matches) {
								return signal;
							}
						}
					}
				}
				for (QMetaMethod method : QMetaObject.forType(signalEmitter.getClass()).methods()) {
					if (method.methodType() == QMetaMethod.MethodType.Signal) {
						AbstractSignal signal = method.toSignal((QObject) signalEmitter);
						if (signal != null && signal.name().equals(name) && !visitedSignals.contains(signal)) {
							if (argumentTypes.length == 0 && signal.signalTypes().isEmpty()) {
								return signal;
							} else if (argumentTypes.length == signal.signalTypes().size()) {
								List<SignalParameterType> signalTypes = signal.signalTypes();

								boolean matches = true;
								for (int i = 0; i < signalTypes.size(); ++i) {
									Class<?> t = signalTypes.get(i).type;
									String arg = t.getName();

									if (t.isArray()) {
										String brackets = "";

										do {
											t = t.getComponentType();
											brackets += "[]";
										} while (t.isArray());

										arg = t.getName() + brackets;
									}

									if (argumentTypes[i].indexOf('.') < 0) {
										arg = arg.substring(arg.lastIndexOf('.') + 1);
									}

									String argLenient = t.isMemberClass() ? arg.replace('$', '.') : null;
									if (!arg.equals(argumentTypes[i])
											&& (argLenient == null || !argLenient.equals(argumentTypes[i]))) {
										matches = false;
										break;
									}
								}
								if (matches) {
									return signal;
								}
							}
						}
					}
				}
			}

			return null;
		} catch (RuntimeException | Error e) {
			e.printStackTrace();
			throw e;
		} catch (Throwable e) {
			e.printStackTrace();
			throw new RuntimeException(e);
		}
	}

	@SuppressWarnings("unchecked")
	static Class<? extends QtObject> getQtSuperType(Class<?> cls) {
		if (cls == QtObject.class || !QtObject.class.isAssignableFrom(cls) || cls == null) {
			return null;
		} else if (isGeneratedClass(cls)) {
			return (Class<? extends QtObject>) cls;
		} else {
			return getQtSuperType(cls.getSuperclass());
		}
	}

	@SuppressWarnings("unchecked")
	@NativeAccess
	static List<Class<? extends QtObjectInterface>> getImplementedInterfaces(Class<?> cls) {
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
			if (result.isEmpty()) {
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

	static Method lookupSlot(Object object, String signature) {
		return lookupSlot(object, signature, null);
	}

	@NativeAccess
	private static Method lookupSlot(Object object, String signature, Class<?>[] requiredParameters) {
		Class<?> cls = object.getClass();

		if (requiredParameters == null) {
			int pos = signature.indexOf('(');
			if (pos < 0) {
				throw new RuntimeException("Wrong syntax in slot signature: '" + signature + "'");
			}
			int spacePos = signature.trim().lastIndexOf(' ', pos);
			if (pos > spacePos && spacePos > 0) {
				throw new RuntimeException("Do not specify return type in slot signature: '" + signature + "'");
			}

			String name = signature.substring(0, pos).trim();

			int pos2 = signature.indexOf(')', pos);
			if (pos2 < 0) {
				throw new RuntimeException("Wrong syntax in slot signature: '" + signature + "'");
			}
			String strTypes = signature.substring(pos + 1, pos2).trim();

			String argumentTypes[] = {};
			if (!strTypes.isEmpty())
				argumentTypes = strTypes.split("\\,");

//            if (strTypes.length() == 0)
//                argumentTypes = new String[0];
//            else
//                argumentTypes = RetroTranslatorHelper.split(strTypes, ",");

			for (int i = 0; i < argumentTypes.length; ++i)
				argumentTypes[i] = argumentTypes[i].replace(" ", "");

			Method result = findFunctionRecursive(cls, name, argumentTypes);
			if (result == null && object instanceof Class) {
				result = findFunctionRecursive((Class<?>) object, name, argumentTypes);
			}
			return result;
		} else {
			Method result = findFunctionRecursive(cls, signature, requiredParameters);
			if (result == null && object instanceof Class) {
				result = findFunctionRecursive((Class<?>) object, signature, requiredParameters);
			}
			return result;
		}
	}

	private static Method findFunctionRecursive(Class<?> cls, String functionName, String argumentTypes[]) {
		Method result = lookupSlots.computeIfAbsent(argumentsHashCode(cls, functionName, argumentTypes),
				argumentsHashCode -> {
					for (Method m : cls.getDeclaredMethods()) {
						boolean found;
						if (!m.getName().equals(functionName))
							continue;

						Class<?> a[] = m.getParameterTypes();
						if (a.length != argumentTypes.length)
							continue;

						found = true;
						for (int i = 0; i < a.length; ++i) {
							String arg = a[i].getName();

							Class<?> t = a[i];

							if (t.isArray()) {
								String brackets = "";

								do {
									t = t.getComponentType();
									brackets += "[]";
								} while (t.isArray());

								arg = t.getName() + brackets;
							}

							if (argumentTypes[i].indexOf('.') < 0) {
								arg = arg.substring(arg.lastIndexOf('.') + 1);
							}

							String argLenient = t.isMemberClass() ? arg.replace('$', '.') : null;
							if (!arg.equals(argumentTypes[i])
									&& (argLenient == null || !argLenient.equals(argumentTypes[i]))) {
								if (m.isVarArgs() && argumentTypes[i].endsWith("...")) {
									if (arg.endsWith("[]")) {
										arg = arg.substring(0, arg.length() - 2) + "...";
										if (argLenient != null && argLenient.endsWith("[]")) {
											argLenient = argLenient.substring(0, argLenient.length() - 2) + "...";
										}
									}
									if (!arg.equals(argumentTypes[i])
											&& (argLenient == null || !argLenient.equals(argumentTypes[i]))) {
										found = false;
										break;
									}
								} else {
									found = false;
									break;
								}
							}
						}

						if (found) {
							return m;
						}
					}
					return null;
				});
		if (result == null) {
			if (cls.getSuperclass() == null)
				return null;
			else
				return findFunctionRecursive(cls.getSuperclass(), functionName, argumentTypes);
		}
		return result;
	}

	private static Method findFunctionRecursive(Class<?> cls, String functionName, Class<?> argumentTypes[]) {
		Method result = lookupSlots.computeIfAbsent(argumentsHashCode(cls, functionName, argumentTypes),
				argumentsHashCode -> {
					for (Method m : cls.getDeclaredMethods()) {
						if (m.getName().equals(functionName) && Arrays.deepEquals(argumentTypes, m.getParameterTypes()))
							return m;
					}
					return null;
				});
		if (result == null) {
			if (cls.getSuperclass() == null)
				return null;
			else
				return findFunctionRecursive(cls.getSuperclass(), functionName, argumentTypes);
		}
		return result;
	}

	/**
	 * Generates a hash from arguments
	 */
	private static int argumentsHashCode(Class<?> cls, String functionName, Object argumentTypes[]) {
		final int prime = 31;
		int result = 1;
		result = prime * result + System.identityHashCode(cls);
		result = prime * result + ((functionName == null) ? 0 : functionName.hashCode());
		result = prime * result + argumentTypes.getClass().hashCode();
		result = prime * result + Arrays.deepHashCode(argumentTypes);
		return result;
	}

	/**
	 * Sets the current QObject sender for receiver to sender. A function call to
	 * setQObjectSender must always be followed by a call to resetQObjectSender
	 * where the data returned from set is passed to reset. Package visibility due
	 * to handling of raw memory pointers in long type.
	 * 
	 * @param receiver The receiver QObject
	 * @param sender   The sender QObject
	 * @return A magic data to be used in the following reset call.
	 */
	static native long setQObjectSender(long receiver, long sender);

	/**
	 * Resets the current sender for the object in receiver. Package visibility due
	 * to handling of raw memory pointers in long type.
	 * 
	 * @param receiver The receiver QObject.
	 * @param data     A magic value which must be the return value from the recent
	 *                 setQObjectSender call.
	 */
	static native void resetQObjectSender(long receiver, long data);

	protected static final ThreadLocal<QtSignalEmitterInterface> currentSender = new ThreadLocal<>();

	static void setCurrentSender(QtSignalEmitterInterface sender) {
		currentSender.set(sender);
	}

	/**
	 * If a signal is currently being emitted (e.g. if this method is called from
	 * within a slot that has been invoked by a signal), then this function will
	 * return the object containing the signal that was emitted.
	 * 
	 * @return Current sender, or null if a signal is not currently being emitted.
	 */
	static QtSignalEmitterInterface getCurrentSender() {
		return currentSender.get();
	}

	/**
	 * Paranoid cleanup method for the current Thread's ThreadLocal data.
	 */
	static void currentSenderRemove() {
		currentSender.remove();
	}

	static native java.lang.invoke.MethodHandles.Lookup privateLookup(Class<?> targetClass);

	public static SerializedLambda serializeLambdaExpression(Serializable slotObject) {
		String className = slotObject.getClass().getName();
		if (slotObject.getClass().isSynthetic() && className.contains("Lambda$") && className.contains("/")) {
			MethodHandle writeReplaceHandle = lambdaWriteReplaceHandles.computeIfAbsent(slotObject.getClass(), cls -> {
				try {
					return QtJambiInternal.privateLookup(cls).findVirtual(cls, "writeReplace",
							MethodType.methodType(Object.class));
				} catch (Throwable e) {
					return null;
				}
			});
			if (writeReplaceHandle != null) {
				try {
					Object serializedResult = writeReplaceHandle.invoke(slotObject);
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

	@NativeAccess
	private static boolean signalMatchesSlot(String signal, String slot) {
		// void slots always match...
		if (slot.contains("()"))
			return true;

		int signalIndex = signal.indexOf('<');

		// Match only if () slot which is covered above already...
		if (signalIndex < 0)
			return false;

		int slotIndex = slot.indexOf('(');
		if (slotIndex < 0) {
			throw new IllegalArgumentException("slot doesn't contain () as expected, '" + slot + "'");
		}

		String substr = signal.substring(signalIndex + 1, signal.length() - 1);
		String signalArguments[] = substr.split("\\,");
		substr = slot.substring(slotIndex + 1, slot.length() - 1);
		String slotArguments[] = substr.split("\\,");

		if (slotArguments.length > signalArguments.length)
			return false;

		for (int i = 0; i < slotArguments.length; ++i) {
			if (!matchTypes(signalArguments[i], slotArguments[i]))
				return false;
		}

		return true;
	}

	private static HashMap<String, String> typeMap;
	static {
		typeMap = new HashMap<String, String>();
		typeMap.put("java.lang.Boolean", "boolean");
		typeMap.put("java.lang.Byte", "byte");
		typeMap.put("java.lang.Char", "char");
		typeMap.put("java.lang.Short", "short");
		typeMap.put("java.lang.Integer", "int");
		typeMap.put("java.lang.Long", "long");
		typeMap.put("java.lang.Float", "float");
		typeMap.put("java.lang.Double", "double");
	}

	private static boolean matchTypes(String a, String b) {
		return (a.equals(b) || (typeMap.get(a) != null && typeMap.get(a).equals(b)));
	}

	@NativeAccess
	private static Method findEmitMethod(Class<?> signalClass) {
		Class<?> cls = signalClass;
		Method slotMethod = null;
		if (QtJambiSignals.AbstractSignal.class.isAssignableFrom(signalClass)) {
			while (slotMethod == null && cls != QtJambiSignals.AbstractSignal.class) {
				Method methods[] = cls.getDeclaredMethods();

				for (Method method : methods) {
					if (method.getName().equals("emit")) {
						slotMethod = method;
						break;
					}
				}
				cls = cls.getSuperclass();
			}
		}
		return slotMethod;
	}

	private static List<Field> findField(Field[] fields, String fieldName) {
		List<Field> found = new ArrayList<>();
		for (Field field : fields) {
			if (field.getName().equals(fieldName)) {
				found.add(field);
				break;
			}
		}
		return found;
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

	public static Object invokeMethod(Method method, Object object, Object... args) throws Throwable {
		java.lang.invoke.MethodHandles.Lookup lookup = QtJambiInternal.privateLookup(method.getDeclaringClass());
		MethodHandle handle = lookup.unreflect(method);
		Object[] _arguments = new Object[args.length + 1];
		_arguments[0] = object;
		System.arraycopy(args, 0, _arguments, 1, args.length);
		return handle.invokeWithArguments(_arguments);
	}

	/**
	 * Returns true if the class cl represents a Signal.
	 * 
	 * @return True if the class is a signal
	 * @param cl The class to check
	 */
	public static boolean isSignalType(Class<?> cl) {
		return QtJambiSignals.AbstractSignal.class.isAssignableFrom(cl)
				&& cl.getEnclosingClass() != QStaticMemberSignals.class
				&& cl.getEnclosingClass() != QDeclarableSignals.class && findEmitMethod(cl) != null;
	}

	static QtJambiInternal.ResolvedSignal resolveSignal(Field field, Class<?> declaringClass) {
		AnnotatedType t = field.getAnnotatedType();

		List<SignalParameterType> typeList = Collections.emptyList();

		// either t is a parameterized type, or it is Signal0
		if (t instanceof AnnotatedParameterizedType) {
			AnnotatedParameterizedType p = (AnnotatedParameterizedType) t;
			AnnotatedType actualTypes[] = p.getAnnotatedActualTypeArguments();

			for (int j = 0; j < actualTypes.length; ++j) {

				AnnotatedType actualType = actualTypes[j];
				boolean isPrimitive = actualType.isAnnotationPresent(QtPrimitiveType.class);
				boolean isPointer = actualType.isAnnotationPresent(QtPointerType.class);
				boolean isReference = !isPointer && actualType.isAnnotationPresent(QtReferenceType.class);
				if (!isPrimitive) {
					AnnotatedType annotatedOwnerType = RetroHelper.getAnnotatedOwnerType(actualType);
					if (annotatedOwnerType != null) {
						isPrimitive = annotatedOwnerType.isAnnotationPresent(QtPrimitiveType.class);
					}
				}
				int arrayDims = 0;
				while (actualType instanceof AnnotatedArrayType) {
					actualType = ((AnnotatedArrayType) actualType).getAnnotatedGenericComponentType();
					++arrayDims;
				}

				Type type = actualTypes[j].getType();
				Class<?> originalTypeClass;
				Class<?> actualTypeClass;
				if (type instanceof Class) {
					originalTypeClass = (Class<?>) type;
					actualTypeClass = (Class<?>) actualType.getType();
				} else if (type instanceof ParameterizedType) {
					ParameterizedType ptype = (ParameterizedType) type;
					originalTypeClass = (Class<?>) ptype.getRawType();
					actualTypeClass = (Class<?>) ptype.getRawType();
				} else {
					throw new RuntimeException("Signals of generic types not supported: " + actualTypes[j].toString());
				}
				while (actualTypeClass.isArray()) {
					arrayDims++;
					actualTypeClass = actualTypeClass.getComponentType();
				}
				if (arrayDims == 0 && isPrimitive) {
					if (originalTypeClass == Integer.class) {
						originalTypeClass = int.class;
						actualTypeClass = int.class;
					} else if (originalTypeClass == Short.class) {
						originalTypeClass = short.class;
						actualTypeClass = short.class;
					} else if (originalTypeClass == Byte.class) {
						originalTypeClass = byte.class;
						actualTypeClass = byte.class;
					} else if (originalTypeClass == Long.class) {
						originalTypeClass = long.class;
						actualTypeClass = long.class;
					} else if (originalTypeClass == Double.class) {
						originalTypeClass = double.class;
						actualTypeClass = double.class;
					} else if (originalTypeClass == Float.class) {
						originalTypeClass = float.class;
						actualTypeClass = float.class;
					} else if (originalTypeClass == Boolean.class) {
						originalTypeClass = boolean.class;
						actualTypeClass = boolean.class;
					} else if (originalTypeClass == Character.class) {
						originalTypeClass = char.class;
						actualTypeClass = char.class;
					}
				}
				// If we do not do this assignment here, we need to uncomment the section in
				// QSignalEmitterInternal#matchTwoTypes()
				// to unwrap things there as well (or unit tests continue to fail).
				SignalParameterType signalType = new SignalParameterType(originalTypeClass, actualTypeClass, type,
						arrayDims, isPointer, isReference);
				if (j == 0) {
					if (actualTypes.length > 1) {
						typeList = new ArrayList<>();
						typeList.add(signalType);
					} else {
						typeList = Collections.singletonList(signalType);
					}
				} else {
					typeList.add(signalType);
				}
			}
			typeList = actualTypes.length > 1 ? Collections.unmodifiableList(typeList) : typeList;
		}
		return new ResolvedSignal(field, typeList);
	}

	static native Object invokeMethod(Object receiver, Method method, boolean isStatic, byte returnType, Object args[], byte slotTypes[]);

	/**
	 * Searches the object's class and its superclasses for a method of the given
	 * name and returns its signature.
	 */
	static private HashMap<String, String> signalMethodSignatureCash = new HashMap<String, String>();

	static String findSignalMethodSignature(QtSignalEmitterInterface signalEmitter, String name)
			throws NoSuchFieldException, IllegalAccessException {

		Class<?> cls = signalEmitter.getClass();
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

	public static <E extends Enum<E> & QtEnumerator> E resolveEnum(Class<E> cl, int value)
			throws io.qt.QNoSuchEnumValueException {
		return resolveEnum(cl, value, null);
	}

	public static <E extends Enum<E> & QtEnumerator> E resolveEnum(Class<E> cl, int value, String name)
			throws io.qt.QNoSuchEnumValueException {
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

	public static <E extends Enum<E> & QtByteEnumerator> E resolveEnum(Class<E> cl, byte value)
			throws io.qt.QNoSuchEnumValueException {
		return resolveEnum(cl, value, null);
	}

	public static <E extends Enum<E> & QtByteEnumerator> E resolveEnum(Class<E> cl, byte value, String name)
			throws io.qt.QNoSuchEnumValueException {
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

	public static <E extends Enum<E> & QtShortEnumerator> E resolveEnum(Class<E> cl, short value)
			throws io.qt.QNoSuchEnumValueException {
		return resolveEnum(cl, value, null);
	}

	public static <E extends Enum<E> & QtShortEnumerator> E resolveEnum(Class<E> cl, short value, String name)
			throws io.qt.QNoSuchEnumValueException {
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

	public static <E extends Enum<E> & QtLongEnumerator> E resolveEnum(Class<E> cl, long value)
			throws io.qt.QNoSuchEnumValueException {
		return resolveEnum(cl, value, null);
	}

	public static <E extends Enum<E> & QtLongEnumerator> E resolveEnum(Class<E> cl, long value, String name)
			throws io.qt.QNoSuchEnumValueException {
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

	private native static <E extends Enum<E> & QtEnumerator> E resolveIntEnum(int hashCode, Class<E> cl, int value,
			String name) throws Throwable;

	private native static <E extends Enum<E> & QtByteEnumerator> E resolveByteEnum(int hashCode, Class<E> cl,
			byte value, String name) throws Throwable;

	private native static <E extends Enum<E> & QtShortEnumerator> E resolveShortEnum(int hashCode, Class<E> cl,
			short value, String name) throws Throwable;

	private native static <E extends Enum<E> & QtLongEnumerator> E resolveLongEnum(int hashCode, Class<E> cl,
			long value, String name) throws Throwable;

	private static class ExpenseCounter {
		long value;
	}

	private final static Map<Class<?>, ExpenseCounter> expensesTable = new HashMap<>();

	public static void countExpense(Class<?> cl, long cost, long limit) {
		ExpenseCounter mi = expensesTable.get(cl);
		if (mi == null) {
			mi = new ExpenseCounter();
			expensesTable.put(cl, mi);
		}

		mi.value += cost;
		if (mi.value > limit) {
			mi.value = 0;
			System.gc();
		}
	}

	@Deprecated
	public static void setReferenceCount(Thread owner, Class<? extends QThread> declaringClass, String fieldName,
			boolean threadSafe, boolean isStatic, Object newValue) {
	}

	public static void setReferenceCount(QtObjectInterface owner, Class<? extends QtObjectInterface> declaringClass,
			String fieldName, boolean threadSafe, boolean isStatic, Object newValue) {
		if (threadSafe) {
			synchronized (isStatic ? declaringClass : owner) {
				setReferenceCount(owner, declaringClass, fieldName, threadSafe, newValue);
			}
		} else {
			setReferenceCount(owner, declaringClass, fieldName, threadSafe, newValue);
		}
	}

	private static void setReferenceCount(QtObjectInterface owner, Class<? extends QtObjectInterface> declaringClass,
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
			Class<?> objectClass = owner.getClass();
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

	private static void setField(Object owner, Field f, Object newValue) {
		setField(QtJambiInternal.privateLookup(f.getDeclaringClass()), owner, f, newValue);
	}

	private static void setField(MethodHandles.Lookup lookup, Object owner, Field f, Object newValue) {
		try {
			MethodHandle setter = lookup.unreflectSetter(f);
			if (Modifier.isStatic(f.getModifiers()))
				setter.invoke(newValue);
			else
				setter.invoke(owner, newValue);
		} catch (Throwable e) {
			if (Modifier.isStatic(f.getModifiers())) {
				if (!setFieldNative(f.getDeclaringClass(), f, true, newValue)) {
					throw new RuntimeException("Cannot set field '" + f.getName() + "'", e);
				}
			} else {
				if (!setFieldNative(owner, f, false, newValue)) {
					throw new RuntimeException("Cannot set field '" + f.getName() + "'", e);
				}
			}
		}
	}

	private static native boolean setFieldNative(Object owner, Field field, boolean isStatic, Object newValue);

	@SuppressWarnings("unchecked")
	public static <V> V fetchField(Object owner, Class<?> declaringClass, String fieldName, Class<V> fieldType) {
		try {
			Field f = declaringClass.getDeclaredField(fieldName);
			return (V) fetchField(owner, f);
		} catch (Throwable e) {
			throw new RuntimeException("Cannot fetch field '" + fieldName + "'");
		}
	}

	static Object fetchField(Object owner, Field f) {
		MethodHandles.Lookup lookup = QtJambiInternal.privateLookup(f.getDeclaringClass());
		return fetchField(lookup, owner, f);
	}

	static Object fetchField(MethodHandles.Lookup lookup, Object owner, Field f) {
		try {
			MethodHandle getter = lookup.unreflectGetter(f);
			if (Modifier.isStatic(f.getModifiers()))
				return getter.invoke();
			else
				return getter.invoke(owner);
		} catch (Throwable e) {
			if (Modifier.isStatic(f.getModifiers())) {
				return fetchFieldNative(f.getDeclaringClass(), f, true);
			} else {
				return fetchFieldNative(owner, f, false);
			}
		}
	}

	public static void clearReferenceCount(QtObjectInterface owner, Class<? extends QtObjectInterface> declaringClass,
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
				Class<?> objectClass = owner.getClass();
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
	public static void addReferenceCount(QtObjectInterface owner, Class<? extends QtObjectInterface> declaringClass,
			String fieldName, boolean isThreadSafe, boolean isStatic, Object value) {
		Object collection = null;
		boolean got = false;
		if (declaringClass.isInterface() && !isStatic) {
			NativeLink link = findInterfaceLink(owner, false);
			if (link instanceof InterfaceNativeLink) {
				collection = ((InterfaceNativeLink) link).getReferenceCountCollection(declaringClass, fieldName, () -> {
					if (isThreadSafe) {
						return java.util.Collections.synchronizedList(new RCList());
					} else {
						return new RCList();
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
				Class<?> objectClass = owner.getClass();
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

	public static void removeFromMapReferenceCount(QtObjectInterface owner,
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
				Class<?> objectClass = owner.getClass();
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

	public static void removeFromCollectionReferenceCount(QtObjectInterface owner,
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
				Class<?> objectClass = owner.getClass();
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
	public static void addAllReferenceCount(QtObjectInterface owner, Class<? extends QtObjectInterface> declaringClass,
			String fieldName, boolean isThreadSafe, boolean isStatic, Collection<?> values) {
		Object collection = null;
		boolean got = false;
		if (declaringClass.isInterface() && !isStatic) {
			NativeLink link = findInterfaceLink(owner, false);
			if (link instanceof InterfaceNativeLink) {
				collection = ((InterfaceNativeLink) link).getReferenceCountCollection(declaringClass, fieldName, () -> {
					if (isThreadSafe) {
						return java.util.Collections.synchronizedList(new RCList());
					} else {
						return new RCList();
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
				Class<?> objectClass = owner.getClass();
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
	public static void putReferenceCount(QtObjectInterface owner, Class<? extends QtObjectInterface> declaringClass,
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
				Class<?> objectClass = owner.getClass();
				do {
					try {
						field = objectClass.getDeclaredField(fieldName);
					} catch (NoSuchFieldException | SecurityException e2) {
					}
					objectClass = objectClass.getSuperclass();
				} while (objectClass != null && field == null);
			}
			if (field != null) {
				MethodHandles.Lookup lookup = QtJambiInternal.privateLookup(field.getDeclaringClass());
				map = fetchField(lookup, owner, field);
				if (map == null) {
					if (isThreadSafe) {
						map = java.util.Collections.synchronizedMap(new java.util.HashMap<>());
					} else {
						map = new java.util.HashMap<>();
					}
					setField(lookup, owner, field, map);
				}
			}
		}
		if (map instanceof Map) {
			((Map<Object, Object>) map).put(key, value);
		}
	}

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
	 * Returns the field entry for all declared signales in o and its base classes.
	 */
	private static List<Field> findSignals(QObject o) {
		Class<?> c = o.getClass();
		List<Field> fields = new ArrayList<Field>();
		while (c != null) {
			Field declared[] = c.getDeclaredFields();
			for (Field f : declared) {
				if (QtJambiInternal.isSignalType(f.getType())) {
					fields.add(f);
				}
			}
			c = c.getSuperclass();
		}
		return fields;
	}

	private static Class<?> objectClass(Class<?> cl) {
		if (cl == boolean.class)
			return java.lang.Boolean.class;
		if (cl == byte.class)
			return java.lang.Byte.class;
		if (cl == char.class)
			return java.lang.Character.class;
		if (cl == short.class)
			return java.lang.Short.class;
		if (cl == int.class)
			return java.lang.Integer.class;
		if (cl == long.class)
			return java.lang.Long.class;
		if (cl == float.class)
			return java.lang.Float.class;
		if (cl == double.class)
			return java.lang.Double.class;
		return cl;
	}

	/**
	 * Compares the signatures and does a connect if the signatures match up.
	 */
	private static void tryConnect(QObject receiver, Method method, QObject sender, Field signal) {
		Class<?> params[] = method.getParameterTypes();
		Type type = signal.getGenericType();

		if (type instanceof ParameterizedType) {
			ParameterizedType pt = (ParameterizedType) type;
			Type types[] = pt.getActualTypeArguments();

			// If the signal has too few arguments, we abort...
			if (types.length < params.length)
				return;

			for (int i = 0; i < params.length; ++i) {
				Class<?> signal_type = (Class<?>) types[i];
				Class<?> param_type = params[i];

				if (signal_type.isPrimitive())
					signal_type = objectClass(signal_type);

				if (param_type.isPrimitive())
					param_type = objectClass(param_type);

				// Parameter types don't match.
				if (signal_type != param_type)
					return;
			}

		} else if (params.length != 0) {
			throw new RuntimeException("Don't know how to autoconnect to: " + signal.getDeclaringClass().getName() + "."
					+ signal.getName());
		}

		// Do the connection...
		Object signal_object = null;
		try {
			signal_object = signal.get(sender);
		} catch (IllegalAccessException e) {
			java.util.logging.Logger.getLogger("io.qt.internal").log(java.util.logging.Level.SEVERE, "", e);
			return;
		}

		((QtJambiSignals.AbstractSignal) signal_object).addConnection(receiver, method);
	}

	public static void connectSlotsByName(QObject object) {
		List<QObject> children = object.findChildren();
		Class<?> objectClass = object.getClass();
		while (objectClass != null) {
			Method methods[] = objectClass.getDeclaredMethods();
			for (QObject child : children) {
				String prefix = "on_" + child.objectName() + "_";
				List<Field> fields = findSignals(child);
				for (Field f : fields) {
					String slot_name = prefix + f.getName();
					for (int i = 0; i < methods.length; ++i) {
						if (methods[i].getName().equals(slot_name)) {
							tryConnect(object, methods[i], child, f);
						}
					}
				}
			}
			objectClass = objectClass.getSuperclass();
		}
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

	@NativeAccess
	private static void writeSerializableJavaObject(QDataStream s, Object o) throws IOException {
		ByteArrayOutputStream bos = new ByteArrayOutputStream();
		ObjectOutputStream out = null;
		try {
			out = new ObjectOutputStream(bos);
			out.writeObject(o);
			ObjectOutputStream tmpOut = out;
			out = null; // don't call close() twice
			tmpOut.close();
		} finally {
			if (out != null) {
				try {
					out.close();
				} catch (IOException eat) {
				}
				out = null;
			}
		}
		s.writeBytes(bos.toByteArray());
	}

	@NativeAccess
	static boolean isGeneratedClass(Class<?> clazz) {
		return isClassGenerated.computeIfAbsent(clazz, cls -> {
			initializePackage(cls);
			if (QtObjectInterface.class.isAssignableFrom(cls)) {
				if (isGeneratedClass(cls.getName())) {
					return true;
				} else if (cls.getSimpleName().equals("ConcreteWrapper") && cls.getEnclosingClass() != null) {
					return isGeneratedClass(cls.getEnclosingClass());
				} else if (cls.getSimpleName().equals("Impl") && cls.getEnclosingClass() != null) {
					return isGeneratedClass(cls.getEnclosingClass());
				}
			}
			return false;
		});
	}

	private static native boolean isGeneratedClass(String className);

	static class ResolvedSignal {
		ResolvedSignal(Field field, List<SignalParameterType> signalTypes) {
			super();
			this.field = field;
			this.signalTypes = signalTypes;
		}

		public final Field field;
		public final List<SignalParameterType> signalTypes;
	}

	static String signalParameters(QtJambiSignals.AbstractSignal signal) {
		return MetaObjectTools.bunchOfClassNamesInARow(signal.signalTypes(), false);
	}

	static String signalCppParameters(QtJambiSignals.AbstractSignal signal) {
		return MetaObjectTools.bunchOfClassNamesInARow(signal.signalTypes(), true);
	}

	@NativeAccess
	private static Object readSerializableJavaObject(final QDataStream s) throws ClassNotFoundException, IOException {
		Object res = null;
		try (ObjectInputStream in = new ObjectInputStream(new InputStream() {
			@Override
			public int read() throws IOException {
				return s.readByte();
			}
		});) {
			res = in.readObject();
		}
		return res;
	}

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

	private static Class<?> privateConstructorClass;

	static Class<?> privateConstructorClass() {
		if (privateConstructorClass == null) {
//        	QPrivateConstructor;
		}
		return privateConstructorClass;
	}

	/**
	 * Internal function which fetches the native id of the current object.
	 * 
	 * @return A long value which uniquely define the native resources held by this
	 *         object during their life time.
	 */
	public static long nativeId(QtJambiObject object) {
		if (object != null && object.nativeLink != null) {
			synchronized (object.nativeLink) {
				return object.nativeLink.native__id;
			}
		}
		return 0;
	}

	public static long checkedNativeId(QtJambiObject object) {
		if(object==null)
			return 0;
		try {
			long nid = object.nativeLink.native__id;
			if (nid == 0) {
				QNoNativeResourcesException e = new QNoNativeResourcesException(
						"Function call on incomplete object of type: " + object.getClass().getName());
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

	public static long checkedNativeId(QtObjectInterface object) {
		if(object==null)
			return 0;
		long nid = nativeId(object);
		if (nid == 0) {
			QNoNativeResourcesException e = new QNoNativeResourcesException(
					"Function call on incomplete object of type: " + object.getClass().getName());
			StackTraceElement[] st = e.getStackTrace();
			st = Arrays.copyOfRange(st, 1, st.length);
			e.setStackTrace(st);
			throw e;
		}
		return nid;
	}

	/**
	 * Internal function which fetches the native id of the current object.
	 * 
	 * @return A long value which uniquely define the native resources held by this
	 *         object during their life time.
	 */
	public static long nativeId(QtObjectInterface object) {
		NativeLink nativeLink = findInterfaceLink(object, true);
		if (nativeLink != null) {
			synchronized (nativeLink) {
				return nativeLink.native__id;
			}
		}
		return 0;
	}

	/**
	 * This is an internal function. Calling it can have unexpected results.
	 *
	 * Disables garbage collection for this object. This should be used when objects
	 * created in java are passed to C++ functions that take ownership of the
	 * objects. Both the Java and C++ part of the object will then be cleaned up by
	 * C++.
	 */
	public static void setCppOwnership(QtObjectInterface object) {
		setCppOwnership(nativeId(object));
	}

	/**
	 * This is an internal function. Calling it can have unexpected results.
	 *
	 * Forces Java ownership of both the Java object and its C++ resources. The C++
	 * resources will be cleaned up when the Java object is finalized.
	 */
	public static void setJavaOwnership(QtObjectInterface object) {
		setJavaOwnership(nativeId(object));
	}

	/**
	 * This is an internal function. Calling it can have unexpected results.
	 *
	 * Reenables garbage collection for this object. Should be used on objects for
	 * which disableGarbageCollection() has previously been called. After calling
	 * this function, the object ownership will be reset to default.
	 */
	public static void setDefaultOwnership(QtObjectInterface object) {
		setDefaultOwnership(nativeId(object));
	}

	/**
	 * This is an internal function. Calling it can have unexpected results.
	 *
	 * Disables garbage collection for this object. This should be used when objects
	 * created in java are passed to C++ functions that take ownership of the
	 * objects. Both the Java and C++ part of the object will then be cleaned up by
	 * C++.
	 */
	public static void setCppOwnership(QtObject object) {
		setCppOwnership(nativeId(object));
	}

	/**
	 * This is an internal function. Calling it can have unexpected results.
	 *
	 * Forces Java ownership of both the Java object and its C++ resources. The C++
	 * resources will be cleaned up when the Java object is finalized.
	 */
	public static void setJavaOwnership(QtObject object) {
		setJavaOwnership(nativeId(object));
	}

	/**
	 * This is an internal function. Calling it can have unexpected results.
	 *
	 * Reenables garbage collection for this object. Should be used on objects for
	 * which disableGarbageCollection() has previously been called. After calling
	 * this function, the object ownership will be reset to default.
	 */
	public static void setDefaultOwnership(QtObject object) {
		setDefaultOwnership(nativeId(object));
	}

	public static void invalidateObject(QtObjectInterface object) {
		invalidateObject(nativeId(object));
	}

	public static void invalidateObject(QtObject object) {
		invalidateObject(nativeId(object));
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
			return Ownership.resolve(ownership(nativeId(object)));
		} catch (QNoSuchEnumValueException e) {
			return Ownership.Invalid;
		}
	}

	public static Ownership ownership(QtObjectInterface object) {
		try {
			return Ownership.resolve(ownership(nativeId(object)));
		} catch (QNoSuchEnumValueException e) {
			return Ownership.Invalid;
		}
	}

	public static QObject owner(QtObjectInterface object) {
		return owner(nativeId(object));
	}

	public static boolean hasOwnerFunction(QtObjectInterface object) {
		return hasOwnerFunction(nativeId(object));
	}

	private static native void invalidateObject(long native__id);

	private static native void setCppOwnership(long native__id);

	private static native void setDefaultOwnership(long native__id);

	private static native void setJavaOwnership(long native__id);

	private static native byte ownership(long native__id);

	private static native QObject owner(long native__id);

	private static native boolean hasOwnerFunction(long native__id);
	
	private static boolean needsReferenceCounting(QtObjectInterface object) {
		return needsReferenceCounting(nativeId(object));
	}
	
	private static native boolean needsReferenceCounting(long native__id);

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
							return disposedSignalFactory.apply(object.getClass());
						});
					} catch (NullPointerException e) {
					}
				}
			} else
				return disposedSignals.get(nativeLink);
		}
		return null;
	}

	static void registerDisposedSignalFactory(Function<Class<?>, QMetaObject.DisposedSignal> factory) {
		if (disposedSignalFactory == null)
			disposedSignalFactory = factory;
	}

	private static QMetaObject.DisposedSignal takeSignalOnDispose(NativeLink nativeLink) {
		return disposedSignals.remove(nativeLink);
	}

	public static QMetaObject.AbstractSignal findSignal(QtSignalEmitterInterface sender, String name,
			Class<?>... types) {
		for (Class<?> cls = sender.getClass(); QtSignalEmitterInterface.class
				.isAssignableFrom(cls); cls = cls.getSuperclass()) {
			MethodHandles.Lookup lookup = QtJambiInternal.privateLookup(cls);
			try {
				Field f = cls.getDeclaredField(name);
				if (!java.lang.reflect.Modifier.isStatic(f.getModifiers())) {
					if (QMetaObject.AbstractSignal.class.isAssignableFrom(f.getType())) {
						AbstractSignal signal = (AbstractSignal) fetchField(lookup, sender, f);
						if (signal != null) {
							if (signal.matchTypes(types)) {
								return (QMetaObject.AbstractSignal) signal;
							}
						}
					} else if (QtJambiSignals.MultiSignal.class.isAssignableFrom(f.getType())) {
						QtJambiSignals.MultiSignal multiSignal = (QtJambiSignals.MultiSignal) fetchField(lookup, sender,
								f);
						for (QMetaObject.AbstractSignal signal : multiSignal.signals()) {
							if (((AbstractSignal) signal).matchTypes(types)) {
								return (QMetaObject.AbstractSignal) signal;
							}
						}
					}
				}
			} catch (NoSuchFieldException | SecurityException t) {
			}
		}
		if (sender instanceof QObject) {
			List<AbstractSignal> listOfExtraSignal = getListOfExtraSignal(
					io.qt.internal.QtJambiInternal.nativeId((QObject) sender));
			for (AbstractSignal signal : listOfExtraSignal) {
				if (signal.name().equals(name) && signal.matchTypes(types)) {
					return (QMetaObject.AbstractSignal) signal;
				}
			}
		}
		return null;
	}

	private native static List<AbstractSignal> getListOfExtraSignal(long sender__id);

	/**
	 * Disconnect all connections made from the given object to a specific object.
	 *
	 * @param sender   The sender to disconnect from receiver
	 * @param receiver The receiver to disconnect, or null to disconnect all
	 *                 receivers
	 */
	@NativeAccess
	public static boolean disconnectAll(QtSignalEmitterInterface sender, Object receiver) {
		boolean result = false;
		if (sender != null) {
			Class<?> cls = sender.getClass();
			while (cls != null && cls != Object.class) {
				Field fields[] = cls.getDeclaredFields();
				MethodHandles.Lookup lookup = QtJambiInternal.privateLookup(cls);

				for (Field f : fields) {
					if (!java.lang.reflect.Modifier.isStatic(f.getModifiers())
							&& AbstractSignal.class.isAssignableFrom(f.getType())) {
						AbstractSignal signal = (AbstractSignal) fetchField(lookup, sender, f);
						if (signal != null) {
							if (receiver == null) {
								try {
									result |= signal.removeConnectionToMethod(null, null, null, true);
								} catch (io.qt.QNoNativeResourcesException e) {
								}
							} else if (receiver instanceof QMetaObject.Connection) {
								result |= signal.removeConnection((QMetaObject.Connection) receiver);
							} else if (receiver instanceof QMetaObject.AbstractSlot) {
								try {
									result |= signal.removeConnectionToSlotObject((QMetaObject.AbstractSlot) receiver);
								} catch (io.qt.QNoNativeResourcesException e) {
								}
							} else {
								try {
									result |= signal.removeConnectionToMethod(receiver, null, null, true);
								} catch (io.qt.QNoNativeResourcesException e) {
								}
							}
						}
					}
				}

				cls = cls.getSuperclass();
			}
			if (sender instanceof QtObjectInterface) {
				NativeLink nativeLink = findInterfaceLink((QtObjectInterface) sender, true);
				QMetaObject.DisposedSignal disposed = getSignalOnDispose(nativeLink, false);
				if (disposed != null) {
					result |= disposed.disconnect(receiver);
				}
			}
		}
		return result;
	}

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

	public static boolean disconnect(QMetaObject.Connection connection) {
		if (connection instanceof QtJambiSignals.JavaConnection) {
			QtJambiSignals.JavaConnection javaConnection = (QtJambiSignals.JavaConnection) connection;
			QtJambiSignals.AbstractSignal signal = javaConnection.signal();
			if (signal != null) {
				return signal.removeConnection(connection);
			}
		} else if (connection instanceof QtJambiSignals.NativeConnection) {
			QtJambiSignals.NativeConnection nativeConnection = (QtJambiSignals.NativeConnection) connection;
			long nativeId = nativeId(nativeConnection);
			if (nativeId != 0)
				return disconnectConnection(nativeId);
		}
		return false;
	}

	static native boolean disconnectConnection(long connection__id);

	@NativeAccess
	private static Class<?> findDefaultImplementation(Class<? extends QtObjectInterface> interfaceClass) {
		for (Class<?> cls : interfaceClass.getClasses()) {
			if (interfaceClass.isAssignableFrom(cls) && "Impl".equals(cls.getSimpleName())) {
				return cls;
			}
		}
		return null;
	}

	public static boolean initializePackage(java.lang.Package pkg) {
		return pkg != null && initializePackage(pkg.getName());
	}

	public static boolean initializePackage(java.lang.Class<?> cls) {
		return cls != null && initializePackage(cls.getPackage());
	}

	public static boolean initializePackage(String packagePath) {
		synchronized (initializedPackages) {
			Boolean b = initializedPackages.get(packagePath);
			if (b != null) {
				return b;
			}
			Class<?> QtJambi_LibraryInitializer;
			try {
				QtJambi_LibraryInitializer = Class.forName(packagePath + ".QtJambi_LibraryInitializer");
			} catch (NoClassDefFoundError t) {
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
				privateLookup(QtJambi_LibraryInitializer)
						.findStatic(QtJambi_LibraryInitializer, "init", MethodType.methodType(void.class)).invoke();
				initializedPackages.put(packagePath, Boolean.TRUE);
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

	public static LambdaInfo lamdaInfo(Serializable slotObject) {
		String className = slotObject.getClass().getName();
		if (slotObject.getClass().isSynthetic() && className.contains("Lambda$") && className.contains("/")) {
			SerializedLambda serializedLambda = serializeLambdaExpression(slotObject);
			if(serializedLambda == null)
				return null;
			MethodHandle methodHandle = lambdaSlotHandles(slotObject.getClass(), serializedLambda);
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

	public static QObject lambdaContext(Serializable slotObject) {
		LambdaInfo lamdaInfo = lamdaInfo(slotObject);
		if (lamdaInfo != null) {
			return lamdaInfo.qobject;
		} else {
			return null;
		}
	}

	public static void disposedCheck(QtObjectInterface obj) {
		if (obj.isDisposed()) {
			QNoNativeResourcesException e = new QNoNativeResourcesException(
					"Function call on incomplete object of type: " + obj.getClass().getName());
			StackTraceElement[] st = e.getStackTrace();
			st = Arrays.copyOfRange(st, 1, st.length - 1);
			e.setStackTrace(st);
			throw e;
		}
	}

	public static Class<?> signalDeclaringClass(QMetaObject.AbstractSignal signal) {
		return ((QtJambiSignals.AbstractSignal) signal).getDeclaringClass();
	}

	public static void initializeInterface(QtObjectInterface iface) {
		findInterfaceLink(iface, true);
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

	native static String internalTypeName(String s, ClassLoader classLoader);
	
	native static String internalTypeNameByClass(Class<?> cls);

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

	private static native int __qt_registerMetaType(Class<?> clazz, boolean isPointer, boolean isReference);

	private static native int __qt_registerContainerMetaType(Class<?> containerType, int... instantiations);

	public static int registerMetaType(Class<?> clazz) {
		return registerMetaType(clazz, clazz, false, false);
	}

	public static int registerMetaType(Class<?> clazz, int[] instantiations) {
		return __qt_registerContainerMetaType(clazz, instantiations);
	}

	public static int registerMetaType(Class<?> clazz, Type genericType, boolean isPointer, boolean isReference) {
		initializePackage(clazz);
		if (genericType instanceof ParameterizedType) {
			ParameterizedType parameterizedType = (ParameterizedType) genericType;
			Type[] actualTypeArguments = parameterizedType.getActualTypeArguments();
			if (parameterizedType.getRawType() instanceof Class<?>) {
				if (isQQmlListProperty((Class<?>) parameterizedType.getRawType())) {
					String listPropertyName = internalTypeNameOfClass(clazz, genericType);
					int id = MetaObjectTools.metaTypeIdForName(listPropertyName);
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
						elementType = registerMetaType((Class<?>)actualTypeArguments[0], actualTypeArguments[0], false, false);
					else if(actualTypeArguments[0] instanceof ParameterizedType 
							&& ((ParameterizedType)actualTypeArguments[0]).getRawType() instanceof Class)
						elementType = registerMetaType((Class<?>)((ParameterizedType)actualTypeArguments[0]).getRawType(), actualTypeArguments[0], false, false);
					else if(actualTypeArguments[0] instanceof TypeVariable) {
						Type[] bounds = ((TypeVariable<?>)actualTypeArguments[0]).getBounds();
						if(bounds.length>0) {
							if(bounds[0] instanceof Class)
								elementType = registerMetaType((Class<?>)bounds[0], actualTypeArguments[0], false, false);
						}
					}
					if(elementType!=0) {
						if(AbstractMetaObjectTools.isListType(clazz)) {
							return __qt_registerContainerMetaType(clazz, elementType);
						}else if(Deque.class==parameterizedType.getRawType()) {
							return __qt_registerContainerMetaType(QStack.class, elementType);
						}else if(Queue.class==parameterizedType.getRawType()) {
							return __qt_registerContainerMetaType(QQueue.class, elementType);
						}else if(Set.class==parameterizedType.getRawType()) {
							return __qt_registerContainerMetaType(QSet.class, elementType);
						}else {
							return __qt_registerContainerMetaType(QList.class, elementType);
						}
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
						keyType = registerMetaType((Class<?>)actualTypeArguments[0], actualTypeArguments[0], false, false);
					else if(actualTypeArguments[0] instanceof ParameterizedType 
							&& ((ParameterizedType)actualTypeArguments[0]).getRawType() instanceof Class)
						keyType = registerMetaType((Class<?>)((ParameterizedType)actualTypeArguments[0]).getRawType(), actualTypeArguments[0], false, false);
					else if(actualTypeArguments[0] instanceof TypeVariable) {
						Type[] bounds = ((TypeVariable<?>)actualTypeArguments[0]).getBounds();
						if(bounds.length>0) {
							if(bounds[0] instanceof Class)
								keyType = registerMetaType((Class<?>)bounds[0], actualTypeArguments[0], false, false);
						}
					}
					int valueType = 0;
					if(actualTypeArguments[1] instanceof Class)
						valueType = registerMetaType((Class<?>)actualTypeArguments[1], actualTypeArguments[1], false, false);
					else if(actualTypeArguments[1] instanceof ParameterizedType 
							&& ((ParameterizedType)actualTypeArguments[1]).getRawType() instanceof Class)
						valueType = registerMetaType((Class<?>)((ParameterizedType)actualTypeArguments[1]).getRawType(), actualTypeArguments[1], false, false);
					else if(actualTypeArguments[1] instanceof TypeVariable) {
						Type[] bounds = ((TypeVariable<?>)actualTypeArguments[1]).getBounds();
						if(bounds.length>0) {
							if(bounds[0] instanceof Class)
								valueType = registerMetaType((Class<?>)bounds[0], actualTypeArguments[1], false, false);
						}
					}
					if(keyType!=0 && valueType!=0) {
						if(NavigableMap.class==parameterizedType.getRawType()) {
							return __qt_registerContainerMetaType(QMap.class, keyType, valueType);
						}else if(Map.class==parameterizedType.getRawType()) {
							return __qt_registerContainerMetaType(QHash.class, keyType, valueType);
						}else {
							return __qt_registerContainerMetaType(clazz, keyType, valueType);
						}
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
			return metaTypeId(o.getClass());
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

	private static List<String> unpackPlugins() {
		// FIXME: The logic of this method is broken. We should be reconfiguring the Qt
		// libraryPath's
		// based on the active deployment spec and putting the runtime location of that
		// at the start
		// of the paths list.
		// This method should be renamed if it does not actually do anything about the
		// "unpacking"
		// process. Maybe it should be "resolvePluginLocations()"
		List<String> paths = NativeLibraryManager.unpackPlugins();
		if (paths != null)
			return paths;

		// FIXME: enumerate ClassPath look for qtjambi-deployment.xml
		// FIXME: Use qtjambi-deployment.xml is one exists
		// FIXME: Make a resolver method (that produces a list of automatic things
		// found) and another method to set from list
		// FIXME: The last resort should be to find a plugins/ directory (when we have
		// no qtjambi-deployment.xml)
		paths = new ArrayList<String>();

		String classPath = System.getProperty("java.class.path");
		String[] classPathElements = classPath.split("\\" + File.pathSeparator);
		for (String element : classPathElements) {
			File base = new File(element);
			if (base.isDirectory()) {
				File descriptorFile = new File(base, "qtjambi-deployment.xml");
				if (descriptorFile.isFile()) {
					if (NativeLibraryManager.configuration() == NativeLibraryManager.Configuration.Debug)
						java.util.logging.Logger.getLogger("io.qt").log(java.util.logging.Level.FINEST,
								"resolve Plugin Locations: found qtjambi-deployment.xml at "
										+ descriptorFile.getAbsolutePath());
				}
				// Assume a default plugins layout
				File pluginsDir = new File(base, "plugins");
				if (pluginsDir.isDirectory()) {
					if (NativeLibraryManager.configuration() == NativeLibraryManager.Configuration.Debug)
						java.util.logging.Logger.getLogger("io.qt").log(java.util.logging.Level.FINEST,
								"resolve Plugin Locations: found plugins/ at " + pluginsDir.getAbsolutePath());
					paths.add(pluginsDir.getAbsolutePath());
				} else {
					if (NativeLibraryManager.configuration() == NativeLibraryManager.Configuration.Debug)
						java.util.logging.Logger.getLogger("io.qt").log(java.util.logging.Level.FINEST,
								"resolve Plugin Locations: found DIRECTORY at " + base.getAbsolutePath());
				}
			} else if (element.toLowerCase().endsWith(".jar")) {
				// FIXME: We should only load MANIFEST.MF qtjambi-deployment.xml from the JAR we
				// activated
				if (NativeLibraryManager.configuration() == NativeLibraryManager.Configuration.Debug)
					java.util.logging.Logger.getLogger("io.qt").log(java.util.logging.Level.FINEST,
							"resolve Plugin Locations: found JAR at " + base.getAbsolutePath());
				if (base.exists()) {
					try {
						URL url = base.toURI().toURL();
						url = new URL("jar:" + url.toString() + "!/plugins");
						paths.add(url.toString());
					} catch (MalformedURLException e) {
						if (NativeLibraryManager.configuration() == NativeLibraryManager.Configuration.Debug)
							java.util.logging.Logger.getLogger("io.qt").log(java.util.logging.Level.SEVERE, "", e);
					}
				}
			} else {
				if (NativeLibraryManager.configuration() == NativeLibraryManager.Configuration.Debug)
					java.util.logging.Logger.getLogger("io.qt").log(java.util.logging.Level.FINEST,
							"resolve Plugin Locations: found FILE at " + base.getAbsolutePath());
			}
		}

		if (paths.isEmpty())
			return null;
		return paths;
	}

	@NativeAccess
	private static List<String> getLibraryPaths() {
		List<String> result = new ArrayList<String>();
		List<String> paths = unpackPlugins();
		if (paths != null) {
			Collections.reverse(paths); // Qt prepends but our list is in highest priority first order
			for (String p : paths)
				result.add(p);
		}

		try {
			if (io.qt.internal.NativeLibraryManager.isUsingDeploymentSpec()) {
				paths = new ArrayList<String>();

				List<String> pluginPaths = io.qt.internal.NativeLibraryManager.pluginPaths();
				if (pluginPaths != null)
					paths.addAll(pluginPaths);

				List<String> pluginDesignerPaths = io.qt.internal.NativeLibraryManager.pluginDesignerPaths();
				if (pluginDesignerPaths != null)
					paths.addAll(pluginDesignerPaths);

				// We don't override the existing values (which are based on envvars)
				// as envvars should continue to take priority even for Java Qt use.
				if (paths.size() > 0) {
					Collections.reverse(paths);
					for (String p : paths)
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
	private static void reportException(String message, Throwable e) {
		try {
			UncaughtExceptionHandler handler = Thread.currentThread().getUncaughtExceptionHandler();
			if (handler != null) {
				handler.uncaughtException(Thread.currentThread(), e);
			} else {
				java.util.logging.Logger.getLogger("io.qt").log(java.util.logging.Level.SEVERE, message, e);
			}
		} catch (Throwable e1) {
			e.printStackTrace();
			e1.printStackTrace();
		}
	}

	public static class InvocationInfo {
		public InvocationInfo(Class<?> declaringClass, String methodName, int lineNumber) {
			super();
			this.declaringClass = declaringClass;
			this.methodName = methodName;
			this.lineNumber = lineNumber;
		}

		public final Class<?> declaringClass;
		public final String methodName;
		public final int lineNumber;
	}

	public static IntFunction<QtJambiInternal.InvocationInfo> invocationInfoProvider() {
		return RetroHelper.classAccessChecker();
	}

	public static void initializeNativeObject(QtObjectInterface object, Map<Class<?>, List<?>> arguments)
			throws IllegalArgumentException {
		InvocationInfo callerInfo = RetroHelper.classAccessChecker().apply(4);
		if (callerInfo.declaringClass == null || !callerInfo.declaringClass.isAssignableFrom(object.getClass())
				|| !"<init>".equals(callerInfo.methodName)) {
			throw new RuntimeException(new IllegalAccessException(
					"QtUtilities.initializeNativeObject(...) can only be called from inside the given object's constructor. Expected: "
							+ object.getClass().getName() + ".<init>, found: "
							+ (callerInfo.declaringClass == null ? "null" : callerInfo.declaringClass.getName()) + "."
							+ callerInfo.methodName));
		}
		__qt_initializeNativeObject(callerInfo.declaringClass, object, findInterfaceLink(object, true, false), arguments);
	}

	static void initializeNativeObject(QtObjectInterface object, NativeLink link) throws IllegalArgumentException {
		initializePackage(object.getClass());
		__qt_initializeNativeObject(object.getClass(), object, link, Collections.emptyMap());
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

		QtJambiMemberAccess<?> getMemberAccess(Class<?> interfaceClass) {
			throw new RuntimeException("Requesting member access of non-interface object is not permitted.");
		}

		void initialize(QtJambiObject obj) {
		}

		@Override
		public final String toString() {
			QtObjectInterface o = get();
			if (o != null) {
				return o.getClass().getName() + "@" + Integer.toHexString(System.identityHashCode(o));
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

		private static final Map<Class<?>, MethodHandle> memberAccessConstructorHandles = new HashMap<>();

		private final Map<Class<?>, QtJambiMemberAccess<?>> memberAccesses;

		private InterfaceNativeLink(QtObjectInterface object, List<Class<? extends QtObjectInterface>> interfaces) {
			super(object);
			Map<Class<?>, QtJambiMemberAccess<?>> memberAccesses = new HashMap<>();
			synchronized (memberAccessConstructorHandles) {
				for (Class<? extends QtObjectInterface> _iface : interfaces) {
					MethodHandle constructorHandle = memberAccessConstructorHandles.computeIfAbsent(_iface, iface -> {
						for (Class<?> innerClass : iface.getClasses()) {
							if (QtJambiMemberAccess.class.isAssignableFrom(innerClass)) {
								java.lang.invoke.MethodHandles.Lookup lookup = QtJambiInternal
										.privateLookup(innerClass);
								try {
									return lookup.findConstructor(innerClass, MethodType.methodType(void.class, iface));
								} catch (NoSuchMethodException | IllegalAccessException e) {
									e.printStackTrace();
								}
							}
						}
						return null;
					});
					if (constructorHandle != null) {
						try {
							QtJambiMemberAccess<?> memberAccess = (QtJambiMemberAccess<?>) constructorHandle
									.invoke(object);
							memberAccesses.put(_iface, memberAccess);
						} catch (Throwable e) {
							e.printStackTrace();
						}
					}
				}
			}
			this.memberAccesses = Collections.unmodifiableMap(memberAccesses);
		}

		QtJambiMemberAccess<?> getMemberAccess(Class<?> interfaceClass) {
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
		} else {
			NativeLink link = interfaceLinks.get(System.identityHashCode(iface));
			if (link == null && forceCreation) {
				link = createNativeLink(iface);
				if (initialize) {
					initializeNativeObject(iface, link);
				}
			}
			return link;
		}
	}

	private native static List<Class<? extends QtObjectInterface>> getInterfaces(
			Class<? extends QtObjectInterface> cls);

	@NativeAccess
	static NativeLink createNativeLink(QtJambiObject object) {
		List<Class<? extends QtObjectInterface>> interfaces = getInterfaces(object.getClass());
		if (interfaces != null) {
			return new InterfaceNativeLink(object, interfaces);
		} else {
			return new NativeLink(object);
		}
//		return nativeLinkSuppliers.computeIfAbsent(object.getClass(), cls->{
//    		List<Class<? extends QtObjectInterface>> interfaces = getInterfaces(cls);
//    		if(interfaces!=null) {
//    			return _this -> new InterfaceNativeLink(_this, interfaces);
//    		}else{
//    			return _this -> new NativeLink(_this);
//    		}
//    	}).apply(object);
	}

	@NativeAccess
	private static NativeLink createNativeLink(QtObjectInterface iface) {
		List<Class<? extends QtObjectInterface>> interfaces = getInterfaces(iface.getClass());
		if (interfaces != null) {
			return new InterfaceBaseNativeLink(iface, interfaces);
		} else {
			return null;
		}
//		return nativeLinkSuppliers.computeIfAbsent(iface.getClass(), cls->{
//    		List<Class<? extends QtObjectInterface>> interfaces = getInterfaces(cls);
//    		if(interfaces!=null) {
//    			return _this -> new InterfaceBaseNativeLink(_this, interfaces);
//    		}else{
//    			return _this -> null;
//    		}
//    	}).apply(iface);
	}

	public native static boolean isQtPatched();

	public static void registerDependentObject(QtObjectInterface dependentObject, QtObjectInterface owner) {
		registerDependentObject(nativeId(dependentObject), nativeId(owner));
	}

	public static void registerDependentObject(QtObject dependentObject, QtObjectInterface owner) {
		registerDependentObject(nativeId(dependentObject), nativeId(owner));
	}

	public static void registerDependentObject(QtObjectInterface dependentObject, QtObject owner) {
		registerDependentObject(nativeId(dependentObject), nativeId(owner));
	}

	public static void registerDependentObject(QtObject dependentObject, QtObject owner) {
		registerDependentObject(nativeId(dependentObject), nativeId(owner));
	}

	private native static void registerDependentObject(long dependentObject, long owner);

	public static void unregisterDependentObject(QtObjectInterface dependentObject, QtObjectInterface owner) {
		unregisterDependentObject(nativeId(dependentObject), nativeId(owner));
	}

	public static void unregisterDependentObject(QtObject dependentObject, QtObjectInterface owner) {
		unregisterDependentObject(nativeId(dependentObject), nativeId(owner));
	}

	public static void unregisterDependentObject(QtObjectInterface dependentObject, QtObject owner) {
		unregisterDependentObject(nativeId(dependentObject), nativeId(owner));
	}

	public static void unregisterDependentObject(QtObject dependentObject, QtObject owner) {
		unregisterDependentObject(nativeId(dependentObject), nativeId(owner));
	}

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
		return isSharedPointer(nativeId(object));
	}

	public static native void qRegisterPluginInterface(Class<? extends QtObjectInterface> iface);

	public static Class<? extends QtObjectInterface> qRegisteredPluginInterface(QByteArray iid) {
		return qRegisteredPluginInterface(nativeId(iid));
	}

	public static native Class<? extends QtObjectInterface> qRegisteredPluginInterface(long iid);

	public static void qRegisterStaticPluginFunction(QObject instance, QJsonObject metaData) {
		if (metaData == null) {
			metaData = loadMetaDataFromClass(instance.getClass());
		}
		qRegisterStaticPluginFunctionInstance(nativeId(instance), nativeId(metaData));
	}

	public static void qRegisterStaticPluginFunction(Class<? extends QObject> pluginClass, QJsonObject metaData) {
		try {
			if (metaData == null) {
				metaData = loadMetaDataFromClass(pluginClass);
			}
			qRegisterStaticPluginFunctionClass(pluginClass, nativeId(metaData));
		} catch (RuntimeException | Error e) {
			throw e;
		} catch (Exception e) {
			throw new RuntimeException("Unable to register plugin " + pluginClass.getName(), e);
		}
	}

	private static QJsonObject loadMetaDataFromClass(Class<? extends QObject> pluginClass) {
		QtPluginMetaData pluginMetaData = pluginClass.getAnnotation(QtPluginMetaData.class);
		if (pluginMetaData != null) {
			if (!pluginMetaData.file().isEmpty()) {
				QFile pluginMetaDataFile = new QFile("classpath:" + pluginClass.getPackage().getName().replace('.', '/')
						+ "/" + pluginMetaData.file());
				if (pluginMetaDataFile.exists() && pluginMetaDataFile.open(QIODevice.OpenModeFlag.ReadOnly)) {
					FromJsonResult result = QJsonDocument.fromJson(pluginMetaDataFile.readAll());
					if (result.document != null)
						return result.document.object();
				} else {
					try {
						InputStream stream = pluginClass.getClassLoader().getResourceAsStream(
								pluginClass.getPackage().getName().replace('.', '/') + "/" + pluginMetaData.file());
						if (stream != null) {
							ByteArrayOutputStream bas = new ByteArrayOutputStream();
							try {
								byte[] buffer = new byte[1024];
								int length = stream.read(buffer);
								while (length > 0) {
									bas.write(buffer, 0, length);
									length = stream.read(buffer);
								}
							} finally {
								stream.close();
							}
							FromJsonResult result = QJsonDocument.fromJson(new QByteArray(bas.toByteArray()));
							if (result.error == null || result.error.error() == ParseError.NoError)
								return result.document.object();
							else
								throw new IOException(result.error.errorString());
						}
					} catch (IOException t) {
						Logger.getLogger("internal").throwing(QtJambiInternal.class.getName(), "qRegisterPlugin", t);
					}
				}
			} else if (!pluginMetaData.json().isEmpty()) {
				FromJsonResult result = QJsonDocument.fromJson(new QByteArray(pluginMetaData.json()));
				if (result.document != null)
					return result.document.object();
			}
		}
		return new QJsonObject();
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
		LambdaInfo lamdaInfo = lamdaInfo(method);
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
		LambdaInfo lamdaInfo = lamdaInfo(method);
		if (lamdaInfo!=null && lamdaInfo.methodHandle != null) {
			return (Class<R>) lamdaInfo.methodHandle.type().returnType();
		} else {
			return null;
		}
	}

	private static native void qRegisterStaticPluginFunctionClass(Class<? extends QObject> pluginClass, long metaData);

	private static native void qRegisterStaticPluginFunctionInstance(long instance, long metaData);

	private static native void qRegisterStaticPluginFunction(Supplier<Class<? extends QObject>> classSupplier,
			String className, String iid, long metaData, long pluginInfo);

	@SuppressWarnings("unchecked")
	public static void findPlugins(String location) {
		if (!noJarPlugins) {
			QDir pluginDir = new QDir(location + "/qtjambi");
			if (!analyzedPaths.contains(pluginDir.absolutePath()) && pluginDir.exists()) {
				analyzedPaths.add(pluginDir.absolutePath());
				for (String plDir : pluginDir.entryList(QDir.Filter.Dirs, QDir.Filter.NoDotAndDotDot)) {
					QDir pluginBundle = new QDir(pluginDir.absoluteFilePath(plDir));
					QFile pluginMetaDataFile = new QFile(pluginBundle.absoluteFilePath("qtjambiplugin.json"));
					if (pluginMetaDataFile.exists() && pluginMetaDataFile.open(QIODevice.OpenModeFlag.ReadOnly)) {
						FromJsonResult result = QJsonDocument.fromJson(pluginMetaDataFile.readAll());
						if (result.error == null || result.error.error() == ParseError.NoError) {
							QJsonObject pluginInfo = result.document.object();
							QJsonValue clsValue = pluginInfo.value("Class");
							QJsonValue iidValue = pluginInfo.value("IID");
							QJsonValue metaDataValue = pluginInfo.value("MetaData");
							pluginInfo.remove("Class");
							pluginInfo.remove("IID");
							pluginInfo.remove("MetaData");
							if (clsValue.isString() && iidValue.isString()) {
								String className = clsValue.toString();
								String iid = iidValue.toString();
								QJsonObject metaData = metaDataValue.toObject();
								if (!className.isEmpty() && !iid.isEmpty()) {
									Class<? extends QObject> cls = null;
									try {
										// check if plugin jar is already loaded
										Class<?> foundClass = Class.forName(className);
										if (QObject.class.isAssignableFrom(foundClass)) {
											cls = (Class<? extends QObject>) foundClass;
										}
									} catch (Throwable e) {
									}
									if (cls == null) {
										try {
											List<URL> urls = new ArrayList<URL>();
											for (QJsonValue val : pluginInfo.value("ClassPaths").toArray()) {
												File file = new File(QDir.toNativeSeparators(
														location + "/qtjambi/" + plDir + "/" + val));
												urls.add(file.toURI().toURL());
											}
											QStringList libraryPaths = new QStringList();
											for (QJsonValue val : pluginInfo.value("LibraryPaths").toArray()) {
												libraryPaths.add(QDir.toNativeSeparators(
														location + "/qtjambi/" + plDir + "/" + val));
											}
											String appendLibPath = libraryPaths.join(File.pathSeparatorChar);
											qRegisterStaticPluginFunction(() -> {
												try {
													if (!appendLibPath.isEmpty()) {
														System.setProperty("java.library.path",
																System.getProperty("java.library.path", "")
																		+ File.pathSeparatorChar + appendLibPath);
													}
													Class<? extends QObject> _cls = null;
													pluginClassLoader.addURLs(urls);
													Class<?> foundClass = pluginClassLoader.loadClass(className);
													if (QObject.class.isAssignableFrom(foundClass)) {
														_cls = (Class<? extends QObject>) foundClass;
													}
													return _cls;
												} catch (Throwable e) {
													Logger.getLogger("internal").throwing(
															QtJambiInternal.class.getName(),
															"findPlugins(String location)", e);
													return null;
												}
											}, className, iid, nativeId(metaData), nativeId(pluginInfo));
										} catch (Throwable e) {
											Logger.getLogger("internal").throwing(QtJambiInternal.class.getName(),
													"findPlugins(String location)", e);
										}
									} else {
										qRegisterStaticPluginFunctionClass(cls, nativeId(metaData));
									}
								}
							}
						} else {
							Logger.getLogger("internal").warning(result.error.errorString());
						}
					}
				}
				for (String jar : pluginDir.entryList(Arrays.asList("*.jar"), QDir.Filter.Files)) {
					ByteArrayOutputStream bas = new ByteArrayOutputStream();
					File file = new File(QDir.toNativeSeparators(pluginDir.absoluteFilePath(jar)));
					try (JarFile jarFile = new JarFile(file)) {
						ZipEntry entry = jarFile.getEntry("qtjambiplugin.json");
						try (InputStream stream = jarFile.getInputStream(entry)) {
							byte[] buffer = new byte[1024];
							int length = stream.read(buffer);
							while (length > 0) {
								bas.write(buffer, 0, length);
								length = stream.read(buffer);
							}
						}
					} catch (Throwable e) {
						Logger.getLogger("internal").throwing(QtJambiInternal.class.getName(),
								"findPlugins(String location)", e);
					}
					if (bas.size() > 0) {
						FromJsonResult result = QJsonDocument.fromJson(new QByteArray(bas.toByteArray()));
						if (result.error == null || result.error.error() == ParseError.NoError) {
							QJsonObject pluginInfo = result.document.object();
							QJsonValue clsValue = pluginInfo.value("Class");
							QJsonValue interfaceValue = pluginInfo.value("IID");
							QJsonValue metaDataValue = pluginInfo.value("MetaData");
							pluginInfo.remove("Class");
							pluginInfo.remove("IID");
							pluginInfo.remove("MetaData");
							if (clsValue.isString() && interfaceValue.isString()) {
								String className = clsValue.toString();
								String iid = interfaceValue.toString();
								QJsonObject metaData = metaDataValue.toObject();
								if (!className.isEmpty() && !iid.isEmpty()) {
									Class<? extends QObject> cls = null;
									try {
										// check if plugin jar is already loaded
										Class<?> foundClass = Class.forName(className);
										if (QObject.class.isAssignableFrom(foundClass)) {
											cls = (Class<? extends QObject>) foundClass;
										}
									} catch (Throwable e) {
									}
									if (cls == null) {
										try {
											URL url = file.toURI().toURL();
											qRegisterStaticPluginFunction(() -> {
												try {
													Class<? extends QObject> _cls = null;
													pluginClassLoader.addURLs(Collections.singletonList(url));
													Class<?> foundClass = pluginClassLoader.loadClass(className);
													if (QObject.class.isAssignableFrom(foundClass)) {
														_cls = (Class<? extends QObject>) foundClass;
													}
													return _cls;
												} catch (Throwable e) {
													Logger.getLogger("internal").throwing(
															QtJambiInternal.class.getName(),
															"findPlugins(String location)", e);
													return null;
												}
											}, className, iid, nativeId(metaData), nativeId(pluginInfo));
										} catch (Throwable e) {
											Logger.getLogger("internal").throwing(QtJambiInternal.class.getName(),
													"findPlugins(String location)", e);
										}
									} else {
										qRegisterStaticPluginFunctionClass(cls, nativeId(metaData));
									}
								}
							}
						} else {
							Logger.getLogger("internal").warning(result.error.errorString());
						}
					}
				}
			}
		}
	}

	public static void findPlugins() {
		for (String location : QCoreApplication.libraryPaths()) {
			findPlugins(location);
		}
	}

	@NativeAccess
	private static QObject loadPluginInstance(String libPath, String className, String pluginName) throws Throwable {
		QFileInfo libFile = new QFileInfo(QDir.fromNativeSeparators(libPath));
		if (libFile.exists()) {
			List<URL> urls = new ArrayList<URL>();
			QDir dir = libFile.dir();
			if (dir.exists(pluginName + ".jar")) {
				QFileInfo jarFile = new QFileInfo(dir.filePath(pluginName + ".jar"));
				if (jarFile.isFile()) {
					urls.add(new File(QDir.toNativeSeparators(jarFile.absoluteFilePath())).toURI().toURL());
				}
			} else {
				QDir subdir = new QDir(dir.filePath(pluginName));
				if (subdir.exists()) {
					for (String jar : subdir.entryList(Arrays.asList("*.jar"), QDir.Filter.Files)) {
						urls.add(new File(QDir.toNativeSeparators(subdir.absoluteFilePath(jar))).toURI().toURL());
					}
					if (!urls.isEmpty()) {
						System.setProperty("java.library.path", System.getProperty("java.library.path", "")
								+ File.pathSeparatorChar + QDir.toNativeSeparators(subdir.absolutePath()));
					}
				}
			}
			if (!urls.isEmpty()) {
				pluginClassLoader.addURLs(urls);
				Class<?> foundClass = pluginClassLoader.loadClass(className);
				MethodHandle constructor = QtJambiInternal.privateLookup(foundClass)
						.unreflectConstructor(foundClass.getDeclaredConstructor());
				QObject result = (QObject) constructor.invoke();
				setCppOwnership(result);
				return result;
			}
		}
		return null;
	}

	public static int countEventLoops(QThread thread) {
		return countEventLoops(nativeId(thread));
	}

	public static native int countEventLoops(long thread);

	public static QFlags<?> asFlags(QtAbstractFlagEnumerator flag, Function<Integer, QFlags<?>> constructor) {
		MethodHandle flagsConstructor = flagsConstructorsByEnumType.computeIfAbsent(flag.getClass(), cls -> {
			Class<?> declClass = cls.getDeclaringClass();
			if (declClass != null) {
				for (Class<?> flagClass : declClass.getDeclaredClasses()) {
					if (flagClass.getSuperclass() == QFlags.class) {
						if (flagClass.getGenericSuperclass() instanceof ParameterizedType) {
							ParameterizedType p = (ParameterizedType) flagClass.getGenericSuperclass();
							Type[] args = p.getActualTypeArguments();
							if (args.length == 1 && args[0] == cls) {
								try {
									Constructor<?> constr = flagClass.getDeclaredConstructor(int.class);
									return privateLookup(flagClass).unreflectConstructor(constr);
								} catch (Throwable e) {
								}
								try {
									Constructor<?> constr = flagClass.getDeclaredConstructor();
									return privateLookup(flagClass).unreflectConstructor(constr);
								} catch (Throwable e) {
								}
								try {
									Constructor<?> constr = flagClass.getConstructor();
									return privateLookup(flagClass).unreflectConstructor(constr);
								} catch (Throwable e) {
								}
							}
						}
					}
				}
			}
			return null;
		});
		int value = 0;
		if (flag instanceof QtFlagEnumerator) {
			value |= ((QtFlagEnumerator) flag).value();
		} else if (flag instanceof QtByteFlagEnumerator) {
			value |= ((QtByteFlagEnumerator) flag).value();
		} else if (flag instanceof QtShortFlagEnumerator) {
			value |= ((QtShortFlagEnumerator) flag).value();
		} else if (flag instanceof QtLongFlagEnumerator) {
			value |= ((QtLongFlagEnumerator) flag).value();
		}
		if (flagsConstructor != null) {
			if (flagsConstructor.type().parameterCount() == 1) {
				try {
					return (QFlags<?>) flagsConstructor.invoke(value);
				} catch (Throwable e) {
				}
			} else {
				try {
					QFlags<?> flags = (QFlags<?>) flagsConstructor.invoke();
					flags.setValue(flags.value() | value);
					return flags;
				} catch (Throwable e) {
				}
			}
		}
		return constructor.apply(value);
	}

	public static boolean isSmallEnum(QtAbstractFlagEnumerator enm) {
		try {
			QtAbstractFlagEnumerator[] values;
			if (enm.getClass().isAnnotationPresent(QtExtensibleEnum.class)) {
				values = enm.getClass().getEnumConstants();
			} else {
				values = flagsConstants.computeIfAbsent(enm.getClass(), cls -> {
					return cls.getEnumConstants();
				});
			}
			return values != null && values.length <= 33;
		} catch (Exception e) {
		}
		return false;
	}

	@SuppressWarnings("unchecked")
	public static <T extends QtAbstractFlagEnumerator> T[] flagConstants(QFlags<T> flags) {
		if (flags.getClass().getGenericSuperclass() instanceof ParameterizedType) {
			ParameterizedType superType = (ParameterizedType) flags.getClass().getGenericSuperclass();
			if (superType.getRawType() == QFlags.class) {
				Type[] typeArguments = superType.getActualTypeArguments();
				if (typeArguments.length == 1 && typeArguments[0] instanceof Class) {
					Class<T> enumFlagType = (Class<T>) typeArguments[0];
					T[] values;
					if (enumFlagType.isAnnotationPresent(QtExtensibleEnum.class)) {
						values = enumFlagType.getEnumConstants();
					} else {
						values = (T[]) flagsConstants.computeIfAbsent(enumFlagType, cls -> {
							return cls.getEnumConstants();
						});
					}
					return values;
				}
			}
		}
		return (T[]) new QtFlagEnumerator[0];
	}

	@NativeAccess
	private static <T extends Enum<T>> boolean extendEnum(Class<T> enumClass, T[] array, T enumEntry) {
		java.lang.invoke.MethodHandles.Lookup pl = privateLookup(Class.class);
		try {
			Field enumConstantsField = Class.class.getDeclaredField("enumConstants");
			Field enumConstantDirectoryField = Class.class.getDeclaredField("enumConstantDirectory");
			pl.unreflectSetter(enumConstantsField).invoke(enumClass, array);
			Map<String, T> directory = (Map<String, T>) pl.unreflectGetter(enumConstantDirectoryField)
					.invoke(enumClass);
			if (directory != null) {
				directory.put(enumEntry.name(), enumEntry);
			}
			return true;
		} catch (Throwable e) {
			try {
				Field enumVarsField = Class.class.getDeclaredField("enumVars");
				Object enumVars = pl.unreflectGetter(enumVarsField).invoke(enumClass);
				java.lang.invoke.MethodHandles.Lookup pl2 = privateLookup(enumVarsField.getType());
				if (enumVars == null) {
					enumVars = pl2.findConstructor(enumVarsField.getType(), MethodType.methodType(void.class)).invoke();
					pl.unreflectSetter(enumVarsField).invoke(enumClass, enumVars);
				}
				Field enumConstantsField = enumVarsField.getType().getDeclaredField("cachedEnumConstants");
				Field enumConstantDirectoryField = enumVarsField.getType()
						.getDeclaredField("cachedEnumConstantDirectory");
				pl2.unreflectSetter(enumConstantsField).invoke(enumVars, array);
				Map<String, T> directory = (Map<String, T>) pl2.unreflectGetter(enumConstantDirectoryField)
						.invoke(enumVars);
				if (directory != null) {
					directory.put(enumEntry.name(), enumEntry);
				}
				return true;
			} catch (Throwable e1) {
				e1.printStackTrace();
			}
		}
		return false;
	}

	@NativeAccess
	private static boolean setThreadInterruptible(Thread thread, Object interruptible) {
		java.lang.invoke.MethodHandles.Lookup pl = privateLookup(Thread.class);
		try {
			Field blockerField = Thread.class.getDeclaredField("blocker");
			pl.unreflectSetter(blockerField).invoke(thread, interruptible);
			return true;
		} catch (Throwable e) {
			try {
				Field blockOnField = Thread.class.getDeclaredField("blockOn");
				pl.unreflectSetter(blockOnField).invoke(thread, interruptible);
				return true;
			} catch (Throwable e1) {
				e1.printStackTrace();
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

	public static native String getInterfaceIID(Class<?> cls);

	public static String cppSlotSignature(Object receiver, String slot) {
		return MetaObjectTools.cppSlotSignature(receiver, slot);
	}

	public static String cppSlotSignature(String slot) {
		return MetaObjectTools.cppSlotSignature(slot);
	}

	public static String cppSignalSignature(QMetaObject.AbstractSignal signal) {
		return MetaObjectTools.cppSignalSignature(signal);
	}

	public static String cppSignalSignature(QtSignalEmitterInterface signalEmitter, String signalName) {
		return MetaObjectTools.cppSignalSignature(signalEmitter, signalName);
	}

	public static String cppNormalizedSignature(String signalName) {
		return MetaObjectTools.cppNormalizedSignature(signalName);
	}

	public static QByteArray cppSignalSignature(QByteArray signal) {
		return MetaObjectTools.cppSignalSignature(signal);
	}

	public static boolean isAvailableQtLibrary(String library) {
		return NativeLibraryManager.isAvailableQtLibrary(library);
	}

	public static boolean isAvailableLibrary(String library, String version) {
		return NativeLibraryManager.isAvailableLibrary(library, version);
	}

	public static void loadQtLibrary(String library) {
		NativeLibraryManager.loadQtLibrary(library);
	}

	public static void loadUtilityLibrary(String library, String version) {
		NativeLibraryManager.loadUtilityLibrary(library, version);
	}

	public static void loadLibrary(String lib) {
		NativeLibraryManager.loadLibrary(lib);
	}

	public static File jambiTempDir() {
		return NativeLibraryManager.jambiTempDir();
	}

	public static void loadQtJambiLibrary(Class<?> callerClass, String library) {
		NativeLibraryManager.loadQtJambiLibrary(callerClass, library);
	}
	
	public static void loadJambiLibrary(Class<?> callerClass, String library) {
		NativeLibraryManager.loadJambiLibrary(callerClass, library);
	}

	public static int majorVersion() {
		return NativeLibraryManager.VERSION_MAJOR;
	}

	public static Object createMetaType(int id, Class<?> javaType, Object copy) {
		if (copy != null && javaType != null) {
			if (javaType.isPrimitive()) {
				copy = getComplexType(javaType).cast(copy);
			} else {
				if (copy instanceof QtObjectInterface) {
					QtJambiInternal.checkedNativeId((QtObjectInterface) copy);
				}
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
	private static String objectToString(Object object) {
		if (object != null) {
			try {
				Method toStringMethod = object.getClass().getMethod("toString");
				if (toStringMethod.getDeclaringClass() != Object.class) {
					return object.toString();
				}
			} catch (Exception e) {
			}
		}
		return null;
	}
}
