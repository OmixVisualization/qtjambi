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
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
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
import java.util.Collections;

import io.qt.core.QMetaObject;
import io.qt.core.QVersionNumber;
import io.qt.internal.QtJambiInternal;

/**
 * This class contains static members that gives information and performs Qt Jambi related tasks.
*/
public final class QtUtilities {
	
	static {
		initializePackage("io.qt.internal");
	}
	
	private QtUtilities() {}
	
    public static boolean isAvailableQtLibrary(String library) {
        return QtJambiInternal.isAvailableQtLibrary(QtJambiInternal.callerClassProvider().get(), library);
    }
    
    public static boolean isAvailableUtilityLibrary(String library) {
    	return isAvailableUtilityLibrary(library, null);
    }
    
    public static boolean isAvailableUtilityLibrary(String library, String versionString) {
        return QtJambiInternal.isAvailableLibrary(library, versionString);
    }
    
    public static void loadQtLibrary(String library) {
    	QtJambiInternal.loadQtLibrary(QtJambiInternal.callerClassProvider().get(), library);
    }

    public static void loadUtilityLibrary(String library) {
        QtJambiInternal.loadUtilityLibrary(library, null);
    }
    
    public static void loadUtilityLibrary(String library, String version) {
        QtJambiInternal.loadUtilityLibrary(library, version);
    }

    public static void loadQtJambiLibrary(String library) {
        QtJambiInternal.loadQtJambiLibrary(QtJambiInternal.callerClassProvider().get(), library);
    }
    
    public static void loadJambiLibrary(String library) {
        QtJambiInternal.loadJambiLibrary(QtJambiInternal.callerClassProvider().get(), library);
    }
    
    public static void loadLibrary(String lib) {
        QtJambiInternal.loadLibrary(lib);
    }

    public static File jambiDeploymentDir() {
        return QtJambiInternal.jambiDeploymentDir();
    }
    
    @Deprecated
    public static File jambiTempDir() {
        return QtJambiInternal.jambiDeploymentDir();
    }
    
    public static boolean initializePackage(String packagePath){
    	Class<?> callerClass = QtJambiInternal.callerClassProvider().get();
    	if(callerClass==null)
    		callerClass = QtUtilities.class;
    	return QtJambiInternal.initializePackage(callerClass.getClassLoader(), packagePath);
    }
    
    public static boolean initializePackage(java.lang.Package pkg){
    	Class<?> callerClass = QtJambiInternal.callerClassProvider().get();
    	if(callerClass==null)
    		callerClass = QtUtilities.class;
    	return QtJambiInternal.initializePackage(callerClass.getClassLoader(), pkg);
    }
    
    public static boolean initializePackage(java.lang.Class<?> cls){
    	return QtJambiInternal.initializePackage(cls);
    }
    
    public static QMetaObject.DisposedSignal getSignalOnDispose(QtObjectInterface object) {
    	return QtJambiInternal.getSignalOnDispose(object, true);
    }
    
    public static void initializeNativeObject(QtObjectInterface object) {
    	QtJambiInternal.initializeNativeObject(object, Collections.emptyMap());
    }
    
    public static void initializeNativeObject(QtObjectInterface object, QtArgument.Stream arguments) {
    	QtJambiInternal.initializeNativeObject(object, arguments.arguments());
    }
    
    @FunctionalInterface
    public interface ByteSupplier{ byte getAsByte(); }
    
    @FunctionalInterface
    public interface ShortSupplier{ short getAsShort(); }
    
    @FunctionalInterface
    public interface FloatSupplier{ float getAsFloat(); }

    @FunctionalInterface
    public interface CharSupplier{ char getAsChar(); }
    
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
     * Return the version of QtJambi
     * @return qtjambi version
     */
    public static QVersionNumber qtjambiVersion() {
    	return new QVersionNumber(QtJambiInternal.majorVersion(), QtJambiInternal.minorVersion(), QtJambiInternal.qtjambiPatchVersion());
    }
}
