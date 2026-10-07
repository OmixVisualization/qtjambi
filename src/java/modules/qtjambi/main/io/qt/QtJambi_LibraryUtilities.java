package io.qt;

import java.io.*;
import java.util.*;
import java.util.function.*;

import io.qt.QtUtilities.*;
import io.qt.core.*;

final class QtJambi_LibraryUtilities {
	
    static {
		Utility.initialize();
    }
    
    static final io.qt.InternalAccess internal = internalAccess();

    private static native io.qt.InternalAccess internalAccess();

    static void initialize() { };

    private QtJambi_LibraryUtilities() throws java.lang.InstantiationError { throw new java.lang.InstantiationError("Cannot instantiate QtJambi_LibraryUtilities."); }
}

/**
 * @hidden
 */
class Utility extends io.qt.internal.NativeUtility{
	/**
	 * @hidden
	 */
	interface ObjectInterface extends io.qt.internal.NativeUtility.ObjectInterface{
	}
	
	/**
	 * @hidden
	 */
	static abstract class Object extends io.qt.internal.NativeUtility.Object implements ObjectInterface{
		Object() {
			super();
		}
		Object(java.lang.Object privateConstructor){
			super(privateConstructor);
		}
	}
		
	protected static void initializeNativeObject(Class<?> declaringClass, QtObjectInterface object, Map<Class<?>, List<Map.Entry<java.lang.Object,java.lang.Object>>> arguments) {
		io.qt.internal.NativeUtility.initializeNativeObject(declaringClass, object, arguments);
	}
	
	protected static QMetaObject.DisposedSignal getSignalOnDispose(QtObjectInterface object, boolean forceCreation){
		return io.qt.internal.NativeUtility.getSignalOnDispose(object, forceCreation);
	}
	
	protected static QMetaObject.DisposedSignal getSignalOnDispose(QtObject object, boolean forceCreation){
		return io.qt.internal.NativeUtility.getSignalOnDispose(object, forceCreation);
	}
	
	protected static void loadQtJambiLibrary(Class<?> callerClass, String library) {
		io.qt.internal.NativeUtility.loadQtJambiLibrary(callerClass, library);
	}
	
	protected static void loadJambiLibrary(Class<?> callerClass, String library) {
		io.qt.internal.NativeUtility.loadJambiLibrary(callerClass, library);
	}

	protected static boolean isAvailableQtLibrary(Class<?> callerClass, String library) {
		return io.qt.internal.NativeUtility.isAvailableQtLibrary(callerClass, library);
	}

	protected static boolean isAvailableLibrary(String library, String version) {
		return io.qt.internal.NativeUtility.isAvailableLibrary(library, version);
	}

	protected static void loadQtLibrary(Class<?> callerClass, String library, LibraryRequirementMode libraryRequirementMode, String...platforms) {
		io.qt.internal.NativeUtility.loadQtLibrary(callerClass, library, libraryRequirementMode, platforms);
	}

	protected static void loadUtilityLibrary(String library, String version, LibraryRequirementMode libraryRequirementMode, String...platforms) {
		io.qt.internal.NativeUtility.loadUtilityLibrary(library, version, libraryRequirementMode, platforms);
	}

	protected static void loadLibrary(String lib) {
		io.qt.internal.NativeUtility.loadLibrary(lib);
	}
	
	protected static void useAsGadget(Class<?> clazz) {
		io.qt.internal.NativeUtility.useAsGadget(clazz);
    }
    
	protected static void usePackageContentAsGadgets(String _package) {
		io.qt.internal.NativeUtility.usePackageContentAsGadgets(_package);
    }

	protected static File jambiDeploymentDir() {
		return io.qt.internal.NativeUtility.jambiDeploymentDir();
	}
	
	protected static int majorVersion() {
		return io.qt.internal.NativeUtility.majorVersion();
	}
	
	protected static int minorVersion() {
		return io.qt.internal.NativeUtility.minorVersion();
	}
	
	protected static int qtjambiPatchVersion() {
		return io.qt.internal.NativeUtility.qtjambiPatchVersion();
	}
	
	protected static boolean initializePackage(ClassLoader classLoader, String packagePath) {
		return io.qt.internal.NativeUtility.initializePackage(classLoader, packagePath);
	}
	
	protected static void initialize() {
	}
}