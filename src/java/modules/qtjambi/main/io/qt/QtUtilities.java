/****************************************************************************
**
** Copyright (C) 2009-2024 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

package io.qt;

import java.io.File;
import java.lang.reflect.Method;
import java.util.Collections;
import java.util.HashMap;
import java.util.Map;
import java.util.Objects;

import io.qt.InternalAccess.CallerContext;
import io.qt.core.QCoreApplication;
import io.qt.core.QMetaObject;
import io.qt.core.QVersionNumber;

/**
 * This class contains static members that gives information and performs QtJambi related tasks.
*/
public final class QtUtilities {
	
	private static final Map<String, Object> initializedPackages = new HashMap<>(Collections.singletonMap("io.qt.internal", Boolean.TRUE));
	static {
		initializePackage("io.qt.internal");
	}
	
	private QtUtilities() {}
	
	/**
	 * Enum for requirement.
	 * @see QtUtilities#loadQtLibrary(String, LibraryRequirementMode, String...)
	 * @see QtUtilities#loadUtilityLibrary(String, LibraryRequirementMode, String...)
	 */
	public enum LibraryRequirementMode{
		/**
		 * Application cannot run without specified library
		 */
        Mandatory,
        /**
         * Specified library is optional
         */
        Optional,
        /**
         * In case of bundled native libraries specified library
         * has to be extracted but not loaded.
         */
        ProvideOnly
    };
	
    public static boolean isAvailableQtLibrary(@NonNull String library) {
        return Utility.isAvailableQtLibrary(Utility.callerClassProvider().get(), library);
    }
    
    public static boolean isAvailableUtilityLibrary(@NonNull String library) {
    	return isAvailableUtilityLibrary(library, null);
    }
    
    public static boolean isAvailableUtilityLibrary(@NonNull String library, @NonNull String versionString) {
        return Utility.isAvailableLibrary(library, versionString);
    }
    
    public static void loadQtLibrary(@NonNull String library) {
    	Utility.loadQtLibrary(Utility.callerClassProvider().get(), library, LibraryRequirementMode.Mandatory);
    }
    
    public static void loadQtLibrary(@NonNull String library, @NonNull LibraryRequirementMode mode, @NonNull String @NonNull...platforms) {
    	Utility.loadQtLibrary(Utility.callerClassProvider().get(), library, mode, platforms);
    }

    public static void loadUtilityLibrary(@NonNull String library) {
    	Utility.loadUtilityLibrary(library, null, LibraryRequirementMode.Mandatory);
    }
    
    public static void loadUtilityLibrary(@NonNull String library, @NonNull LibraryRequirementMode mode, @NonNull String @NonNull...platforms) {
    	Utility.loadUtilityLibrary(library, null, mode, platforms);
    }
    
    public static void loadUtilityLibrary(@NonNull String library, @NonNull String version) {
    	Utility.loadUtilityLibrary(library, version, LibraryRequirementMode.Mandatory);
    }
    
    public static void loadUtilityLibrary(@NonNull String library, @NonNull String version, @NonNull LibraryRequirementMode mode, @NonNull String @NonNull...platforms) {
    	Utility.loadUtilityLibrary(library, version, mode);
    }

    public static void loadQtJambiLibrary(@NonNull String library) {
    	Utility.loadQtJambiLibrary(Utility.callerClassProvider().get(), library);
    }
    
    public static void loadJambiLibrary(@NonNull String library) {
    	Utility.loadJambiLibrary(Utility.callerClassProvider().get(), library);
    }
    
    public static void loadLibrary(@NonNull String lib) {
    	Utility.loadLibrary(lib);
    }
    
    /**
     * Use a class (e.g. from third party library) as gadget.
     * @see io.qt.QtAsGadget
     * @see io.qt.core.QMetaObject#forType(Class)
     */
    public static void useAsGadget(@Nullable Class<?> clazz) {
    	Utility.useAsGadget(clazz);
    }
    
    /**
     * Define a package (e.g. from third party library) to let all its classes considered to be gadgets.
     * @see io.qt.QtAsGadget
     * @see io.qt.core.QMetaObject#forType(Class)
     */
    public static void usePackageContentAsGadgets(@NonNull String _package) {
    	Utility.usePackageContentAsGadgets(_package);
    }

    public static @Nullable File jambiDeploymentDir() {
        return Utility.jambiDeploymentDir();
    }
    
    /**
     * @deprecated Use {@link #jambiDeploymentDir()} instead.
     * @return
     */
    @Deprecated(forRemoval=true)
    public static @Nullable File jambiTempDir() {
        return Utility.jambiDeploymentDir();
    }
    
    public static boolean initializePackage(@NonNull String packagePath){
    	Class<?> callerClass = Utility.callerClassProvider().get();
    	if(callerClass==null)
    		callerClass = QtUtilities.class;
    	return initializePackage(callerClass.getClassLoader(), packagePath);
    }
    
    public static boolean initializePackage(java.lang.@StrictNonNull Package pkg){
    	Class<?> callerClass = Utility.callerClassProvider().get();
    	if(callerClass==null)
    		callerClass = QtUtilities.class;
    	return initializePackage(callerClass.getClassLoader(), pkg);
    }
    
	private static boolean initializePackage(@Nullable ClassLoader classLoader, java.lang.@StrictNonNull Package pkg) {
		return pkg != null && initializePackage(classLoader, pkg.getName());
	}

	@NativeAccess
	public static boolean initializePackage(java.lang.@Nullable Class<?> cls) {
		return cls != null && cls.getPackage() != null && initializePackage(cls.getClassLoader(), cls.getPackage().getName());
	}
	
	private static boolean initializePackage(@Nullable ClassLoader classLoader, @NonNull String packagePath) {
		synchronized (initializedPackages) {
			Object obj = initializedPackages.get(packagePath);
			if (obj instanceof Boolean) {
				return (Boolean)obj;
			}else if (obj instanceof Error) {
				throw (Error)obj;
			}else if (obj instanceof RuntimeException) {
				throw (RuntimeException)obj;
			}
		}
		Class<?> cls;
		try {
			try {
				try {
					cls = Class.forName(packagePath + ".QtJambi_LibraryUtilities");
				} catch (ClassNotFoundException e) {
					if(classLoader!=null && classLoader!=QtUtilities.class.getClassLoader()) {
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
				synchronized (initializedPackages) {
					initializedPackages.put(packagePath, Boolean.FALSE);
				}
				return false;
			}
			try {
				Method initialize = cls.getDeclaredMethod("initialize");
				QtJambi_LibraryUtilities.internal.invokeMethod(initialize, null);
				synchronized (initializedPackages) {
					initializedPackages.put(packagePath, Boolean.TRUE);
				}
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
		} catch (RuntimeException | Error t) {
			synchronized (initializedPackages) {
				initializedPackages.put(packagePath, t);
			}
			throw t;
		}
	}
	
    public static QMetaObject.@NonNull DisposedSignal getSignalOnDispose(@NonNull QtObjectInterface object) {
    	return Utility.getSignalOnDispose(object, true);
    }
    
    public static void initializeNativeObject(@NonNull QtObjectInterface object) {
    	Class<?> cls = QtJambi_LibraryUtilities.internal.getClass(object);
    	CallerContext callerInfo = QtJambi_LibraryUtilities.internal.callerContextProvider().get();
    	if (callerInfo.declaringClass == null || !callerInfo.declaringClass.isAssignableFrom(cls)
				|| !"<init>".equals(callerInfo.methodName)) {
			throw new RuntimeException(new IllegalAccessException(
					"QtUtilities.initializeNativeObject(...) can only be called from inside the given object's constructor. Expected: "
							+ cls.getName() + ".<init>, found: "
							+ (callerInfo.declaringClass == null ? "null" : callerInfo.declaringClass.getName()) + "."
							+ callerInfo.methodName));
		}
    	Utility.initializeNativeObject(callerInfo.declaringClass, object, Collections.emptyMap());
    }
    
    public static void initializeNativeObject(@NonNull QtObjectInterface object, QtArgument.@NonNull Stream arguments) {
    	Class<?> cls = QtJambi_LibraryUtilities.internal.getClass(object);
    	CallerContext callerInfo = QtJambi_LibraryUtilities.internal.callerContextProvider().get();
    	if (callerInfo.declaringClass == null || !callerInfo.declaringClass.isAssignableFrom(cls)
				|| !"<init>".equals(callerInfo.methodName)) {
			throw new RuntimeException(new IllegalAccessException(
					"QtUtilities.initializeNativeObject(...) can only be called from inside the given object's constructor. Expected: "
							+ cls.getName() + ".<init>, found: "
							+ (callerInfo.declaringClass == null ? "null" : callerInfo.declaringClass.getName()) + "."
							+ callerInfo.methodName));
		}
    	Utility.initializeNativeObject(callerInfo.declaringClass, object, arguments.arguments());
    }
    
    /**
     * Equivalent to {@link java.util.function.IntSupplier} with {@code byte} result.
     */
    @FunctionalInterface
    public interface ByteSupplier{ byte getAsByte(); }
    
    /**
     * Equivalent to {@link java.util.function.IntSupplier} with {@code short} result.
     */
    @FunctionalInterface
    public interface ShortSupplier{ short getAsShort(); }
    
    /**
     * Equivalent to {@link java.util.function.DoubleSupplier} with {@code float} result.
     */
    @FunctionalInterface
    public interface FloatSupplier{ float getAsFloat(); }

    /**
     * Equivalent to {@link java.util.function.IntSupplier} with {@code char} result.
     */
    @FunctionalInterface
    public interface CharSupplier{ char getAsChar(); }
    
    /**
     * Equivalent to {@link java.util.function.Supplier}.
     */
    @FunctionalInterface
    public interface Supplier<T> extends java.util.function.Supplier<T>, java.io.Serializable { }
    
    /**
     * Equivalent to {@link java.util.function.Consumer} with three arguments.
     */
    @FunctionalInterface
    public static interface Consumer3<A,B,C>{
        public void accept(A a, B b, C c);
    }
    
    /**
     * Equivalent to {@link java.util.function.Consumer} with four arguments.
     */
    @FunctionalInterface
    public static interface Consumer4<A,B,C,D>{
        public void accept(A a, B b, C c, D d);
    }
    
    /**
     * Equivalent to {@link java.util.function.Consumer} with five arguments.
     */
    @FunctionalInterface
    public static interface Consumer5<A,B,C,D,E>{
        public void accept(A a, B b, C c, D d, E e);
    }
    
    /**
     * Equivalent to {@link java.util.function.Consumer} with six arguments.
     */
    @FunctionalInterface
    public static interface Consumer6<A,B,C,D,E,F>{
        public void accept(A a, B b, C c, D d, E e, F f);
    }
    
    /**
     * Equivalent to {@link java.util.function.Consumer} with seven arguments.
     */
    @FunctionalInterface
    public static interface Consumer7<A,B,C,D,E,F,G>{
        public void accept(A a, B b, C c, D d, E e, F f, G g);
    }
    
    /**
     * Equivalent to {@link java.util.function.Consumer} with eight arguments.
     */
    @FunctionalInterface
    public static interface Consumer8<A,B,C,D,E,F,G,H>{
        public void accept(A a, B b, C c, D d, E e, F f, G g, H h);
    }
    
    /**
     * Equivalent to {@link java.util.function.Consumer} with nine arguments.
     */
    @FunctionalInterface
    public static interface Consumer9<A,B,C,D,E,F,G,H,I>{
        public void accept(A a, B b, C c, D d, E e, F f, G g, H h, I i);
    }
    
    /**
     * Equivalent to {@link java.util.function.Consumer} with ten arguments.
     */
    @FunctionalInterface
    public static interface Consumer10<A,B,C,D,E,F,G,H,I,J>{
        public void accept(A a, B b, C c, D d, E e, F f, G g, H h, I i, J j);
    }
    
    /**
     * This function sets the value of the environment variable named varName.
     * @see https://doc.qt.io/qt/qtglobal.html#qputenv
     * @param varName
     * @param value
     */
    public static native boolean putenv(@NonNull String varName, @NonNull String value);

    /**
     * This function deletes the variable varName from the environment.
     * @see https://doc.qt.io/qt/qtglobal.html#qunsetenv
     * @param varName
     */
    public static native boolean unsetenv(@NonNull String varName);
    
    /**
     * This function gets the value of the environment variable named varName.
     * @see https://doc.qt.io/qt/qtglobal.html#	qEnvironmentVariable
     * @param varName
     * @return value
     */
    public static native @Nullable String getenv(@NonNull String varName);
    
    /**
     * This method allows to reinstall the QtJambi event notify callback as the last callback.
     * If other libraries register their event notify callback to Qt,
     * it is necessary to make sure QtJambi's callback is called at last.
     * This method can only be called from inside the main thread and before calling {@link QCoreApplication#exec()}.
     */
    public static native boolean reinstallEventNotifyCallback();
    
    /**
     * Third-party libraries must not overwrite Java's installed signal handlers. 
     * Several of these signal handlers are crucial for JVM to work.
     * This method saves the installed signal handlers for later restoring by {@link #restoreUnixSignalHandlers()}.
     */
    public static native boolean saveUnixSignalHandlers();
    
    /**
     * Third-party libraries must not overwrite Java's installed signal handlers. 
     * Several of these signal handlers are crucial for JVM to work.
     * This method restores the signal handlers previously saved by {@link #saveUnixSignalHandlers()}.
     */
    public static native boolean restoreUnixSignalHandlers();
    
    /**
     * Return the version of QtJambi
     * @return qtjambi version
     */
    public static @NonNull QVersionNumber qtjambiVersion() {
    	return new QVersionNumber(Utility.majorVersion(), Utility.minorVersion(), Utility.qtjambiPatchVersion());
    }
    
    /**
     * Checks if the current thread is UI thread and throws QThreadAffinityException otherwise.
     * @throws QThreadAffinityException
     */
    public static native void uiThreadCheck() throws QThreadAffinityException;
    
    /**
     * Checks if the current thread is the given object's thread and throws QThreadAffinityException otherwise.
     * @throws QThreadAffinityException
     */
    public static native void threadCheck(io.qt.core.@Nullable QObject object) throws QThreadAffinityException;
    
    /**
     * Reduces the number of native-to-java conversions by pre-filtering the events according to the given event types.
     * @param eventFilter
     * @param eventType
     * @param eventTypes
     * @return selective event filter
     */
    public static io.qt.core.@NonNull QObject asSelectiveEventFilter(io.qt.core.@StrictNonNull QObject eventFilter, io.qt.core.QEvent.@NonNull Type eventType, io.qt.core.QEvent.@NonNull Type @NonNull... eventTypes) {
    	return asSelectiveEventFilter(QtJambi_LibraryUtilities.internal.checkedNativeId(Objects.requireNonNull(eventFilter, "Argument 'eventFilter': null not expected.")), eventType, eventTypes);
    }
    
    private static native io.qt.core.@NonNull QObject asSelectiveEventFilter(long objectId, io.qt.core.QEvent.@NonNull Type eventType, io.qt.core.QEvent.@NonNull Type @NonNull... eventTypes);
    
    /**
     * Enables or disables the dangling pointer checks at runtime.
     * <p><b>CAUTION:</b> This function is experimental and may have unexpected behavior.</p>
     * @param enabled
     */
    public static native void setDanglingPointerCheckEnabled(boolean enabled);
    
    /**
     * <p>Enables or disables the thread affinity checks at runtime.</p>
     * <p>This function is not thread-safe, i.e. calling this method parallel to usage of Qtjambi's API in other threads may lead to crash.</p>
     * <p><b>CAUTION:</b> This function is experimental and may have unexpected behavior.</p>
     * @param enabled
     */
    public static native void setThreadAffinityCheckEnabled(boolean enabled);
    
    /**
     * <p>Enables or disables the thread affinity checks on events at runtime.</p>
     * <p>This function is not thread-safe, i.e. calling this method parallel to running event loops in other threads may lead to crash.</p>
     * <p><b>CAUTION:</b> This function is experimental and may have unexpected behavior.</p>
     * @param enabled
     */
    public static native void setEventThreadAffinityCheckEnabled(boolean enabled);

    /**
     * Enables or disables method logs at runtime.
     * <p><b>CAUTION:</b> This function is experimental and may have unexpected behavior.</p>
     * @param enabled
     */
    public static native void setMethodLogsEnabled(boolean enabled);
    
    /**
     * Enables or disables event logs checks at runtime.
     * <p><b>CAUTION:</b> This function is experimental and may have unexpected behavior.</p>
     * @param enabled
     */
    public static native void setEventLogsEnabled(boolean enabled);
}
