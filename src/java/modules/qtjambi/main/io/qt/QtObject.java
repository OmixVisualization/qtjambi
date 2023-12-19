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
import java.util.List;
import java.util.Map;
import java.util.function.Supplier;

import io.qt.QtUtilities.LibraryRequirementMode;
import io.qt.core.QMetaObject;

/**
 * Super class of all native types provided by QtJambi.
 * The native counterpart is owned by the object if it has been constructed in Java.
 * This counterpart is assumed to be created during Java constructor call
 * and deleted as soon as garbage collection deletes the Java object.
 * 
 * Call {@link #dispose()} to disconnect the Java object from its the native counterpart.
 * If the native component is owned by by Java it is deleted this way.
 * Otherwise, only the link between native and Java object is removed.
 * To check if the Java object is alive use {@link #isDisposed()}.
 */
public abstract class QtObject extends Utility.Object implements QtObjectInterface{
    
    static {
    	QtJambi_LibraryUtilities.initialize();
    }
    
    /**
     * This type is used as solo argument for internal constructors.
     * @hidden
     */
    protected static final class QPrivateConstructor { private QPrivateConstructor() {} }

    /**
     * Creates a new QtObject.
     */
	public QtObject() {
		super();
	}

    /**
     * Constructor for internal use only.
     * @param p expected to be <code>null</code>.
     * @hidden
     */
	protected QtObject(QPrivateConstructor p) {
		super(p);
	}
	
    /**
     * Explicitly removes the native resources held by the
     * object. Note that though this method does not guarantee that
     * the object will be garbage collected, it is not safe to
     * reference the object after it has been disposed.
     */
	@Override
    public final void dispose() {
    	super.dispose();
    }
    
    /**
     * Is java object connected to native object?
     * @return true if disposed
     */
    @QtUninvokable
	@Override
    public final boolean isDisposed() {
    	return Utility.isObjectDisposed(this);
    }
    
    /**
     * Used to check if this object shares the same native counterpart with the other object,
     * i.e. the objects are equals if their native counterparts are identical.
     * Qt value types override this method to make a value comparison.
     * Otherwise it will return the result of calling the super class implementation.
     *
     * @param other other object
     * @return true if the native counterpart of the two objects are one and the same.
     */
    @Override
    public boolean equals(java.lang.Object other) {
    	return super.equals(other);
    }
}

class Utility extends io.qt.internal.NativeUtility{
	static abstract class Object extends io.qt.internal.NativeUtility.Object{
		Object() {
			super();
		}
		Object(java.lang.Object privateConstructor){
			super(privateConstructor);
		}
	}
	
	protected static void disposeObject(QtObjectInterface object) {
		io.qt.internal.NativeUtility.disposeObject(object);
	}

	protected static boolean isObjectDisposed(QtObjectInterface object) {
		return io.qt.internal.NativeUtility.isObjectDisposed(object);
	}
	
	static Boolean areObjectsEquals(Utility.Object object, java.lang.Object other) {
		return io.qt.internal.NativeUtility.areObjectsEquals(object, other);
	}
		
	protected static void initializeNativeObject(Class<?> declaringClass, QtObjectInterface object, Map<Class<?>, List<?>> arguments) {
		io.qt.internal.NativeUtility.initializeNativeObject(declaringClass, object, arguments);
	}
	
	protected static QMetaObject.DisposedSignal getSignalOnDispose(QtObjectInterface object, boolean forceCreation){
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
	
	protected static Supplier<Class<?>> callerClassProvider() {
		return io.qt.internal.NativeUtility.callerClassProvider();
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
}