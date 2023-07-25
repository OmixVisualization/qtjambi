/****************************************************************************
**
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import io.qt.InternalAccess.CallerContext;
import io.qt.core.QCoreApplication;
import io.qt.core.QMetaObject;
import io.qt.core.QVersionNumber;

/**
 * This class contains static members that gives information and performs QtJambi related tasks.
*/
public final class QtUtilities {
	
	private static final Map<String, Boolean> initializedPackages;
	static {
		initializedPackages = Collections.synchronizedMap(new HashMap<>());
		initializedPackages.put("io.qt.internal", Boolean.TRUE);
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
	
    public static boolean isAvailableQtLibrary(String library) {
        return Utility.isAvailableQtLibrary(Utility.callerClassProvider().get(), library);
    }
    
    public static boolean isAvailableUtilityLibrary(String library) {
    	return isAvailableUtilityLibrary(library, null);
    }
    
    public static boolean isAvailableUtilityLibrary(String library, String versionString) {
        return Utility.isAvailableLibrary(library, versionString);
    }
    
    public static void loadQtLibrary(String library) {
    	Utility.loadQtLibrary(Utility.callerClassProvider().get(), library, LibraryRequirementMode.Mandatory);
    }
    
    public static void loadQtLibrary(String library, LibraryRequirementMode mode, String...platforms) {
    	Utility.loadQtLibrary(Utility.callerClassProvider().get(), library, mode, platforms);
    }

    public static void loadUtilityLibrary(String library) {
    	Utility.loadUtilityLibrary(library, null, LibraryRequirementMode.Mandatory);
    }
    
    public static void loadUtilityLibrary(String library, LibraryRequirementMode mode, String...platforms) {
    	Utility.loadUtilityLibrary(library, null, mode, platforms);
    }
    
    public static void loadUtilityLibrary(String library, String version) {
    	Utility.loadUtilityLibrary(library, version, LibraryRequirementMode.Mandatory);
    }
    
    public static void loadUtilityLibrary(String library, String version, LibraryRequirementMode mode, String...platforms) {
    	Utility.loadUtilityLibrary(library, version, mode);
    }

    public static void loadQtJambiLibrary(String library) {
    	Utility.loadQtJambiLibrary(Utility.callerClassProvider().get(), library);
    }
    
    public static void loadJambiLibrary(String library) {
    	Utility.loadJambiLibrary(Utility.callerClassProvider().get(), library);
    }
    
    public static void loadLibrary(String lib) {
    	Utility.loadLibrary(lib);
    }
    
    /**
     * Use a class (e.g. from third party library) as gadget.
     * @see io.qt.QtAsGadget
     * @see io.qt.core.QMetaObject#forType(Class)
     */
    public static void useAsGadget(Class<?> clazz) {
    	Utility.useAsGadget(clazz);
    }
    
    /**
     * Define a package (e.g. from third party library) to let all its classes considered to be gadgets.
     * @see io.qt.QtAsGadget
     * @see io.qt.core.QMetaObject#forType(Class)
     */
    public static void usePackageContentAsGadgets(String _package) {
    	Utility.usePackageContentAsGadgets(_package);
    }

    public static File jambiDeploymentDir() {
        return Utility.jambiDeploymentDir();
    }
    
    /**
     * @deprecated Use {@link #jambiDeploymentDir()} instead.
     * @return
     */
    @Deprecated
    public static File jambiTempDir() {
        return Utility.jambiDeploymentDir();
    }
    
    public static boolean initializePackage(String packagePath){
    	Class<?> callerClass = Utility.callerClassProvider().get();
    	if(callerClass==null)
    		callerClass = QtUtilities.class;
    	return initializePackage(callerClass.getClassLoader(), packagePath);
    }
    
    public static boolean initializePackage(java.lang.Package pkg){
    	Class<?> callerClass = Utility.callerClassProvider().get();
    	if(callerClass==null)
    		callerClass = QtUtilities.class;
    	return initializePackage(callerClass.getClassLoader(), pkg);
    }
    
	private static boolean initializePackage(ClassLoader classLoader, java.lang.Package pkg) {
		return pkg != null && initializePackage(classLoader, pkg.getName());
	}

	@NativeAccess
	public static boolean initializePackage(java.lang.Class<?> cls) {
		return cls != null && cls.getPackage() != null && initializePackage(cls.getClassLoader(), cls.getPackage().getName());
	}
	
	private static boolean initializePackage(ClassLoader classLoader, String packagePath) {
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
				initializedPackages.put(packagePath, Boolean.FALSE);
				return false;
			}
			try {
				Method initialize = cls.getDeclaredMethod("initialize");
				QtJambi_LibraryUtilities.internal.invokeMethod(initialize, null);
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
	
    public static QMetaObject.DisposedSignal getSignalOnDispose(QtObjectInterface object) {
    	return Utility.getSignalOnDispose(object, true);
    }
    
    public static void initializeNativeObject(QtObjectInterface object) {
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
    
    public static void initializeNativeObject(QtObjectInterface object, QtArgument.Stream arguments) {
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
     * This function sets the value of the environment variable named varName.
     * @see https://doc.qt.io/qt/qtglobal.html#qputenv
     * @param varName
     * @param value
     */
    public static native boolean putenv(String varName, String value);

    /**
     * This function deletes the variable varName from the environment.
     * @see https://doc.qt.io/qt/qtglobal.html#qunsetenv
     * @param varName
     */
    public static native boolean unsetenv(String varName);
    
    /**
     * This function gets the value of the environment variable named varName.
     * @see https://doc.qt.io/qt/qtglobal.html#	qEnvironmentVariable
     * @param varName
     * @return value
     */
    public static native String getenv(String varName);
    
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
    public static QVersionNumber qtjambiVersion() {
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
    public static void threadCheck(io.qt.core.QObject object) throws QThreadAffinityException{
    	threadCheck(QtJambi_LibraryUtilities.internal.checkedNativeId(object));
    }
    
    private static native void threadCheck(long objectId) throws QThreadAffinityException;
}
