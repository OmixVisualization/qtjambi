/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2020 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import io.qt.core.QMetaObject;
import io.qt.core.QObject;
import io.qt.internal.NativeLibraryManager;
import io.qt.internal.QtJambiInternal;
import io.qt.widgets.QWidget;

/**
This class contains static members that gives information and performs Qt Jambi
related tasks.
*/
public class QtUtilities {
	
	static {
		initializePackage("io.qt.internal");
	}
	
	private final static StackWalker stackWalker = StackWalker.getInstance(java.util.Collections.singleton(StackWalker.Option.RETAIN_CLASS_REFERENCE));
	
    public static boolean isAvailableQtLibrary(String library) {
        return NativeLibraryManager.isAvailableQtLibrary(library);
    }
    
    public static boolean isAvailableUtilityLibrary(String library, String versionString) {
        return NativeLibraryManager.isAvailableLibrary(null, library, null, versionString);
    }
    
    public static void loadQtLibrary(String library) {
    	NativeLibraryManager.loadQtLibrary(library);
    }

    public static void loadUtilityLibrary(String library, String version) {
        NativeLibraryManager.loadUtilityLibrary(library, version);
    }

    public static void loadQtJambiLibrary(String library) {
        NativeLibraryManager.loadQtJambiLibrary(stackWalker.getCallerClass(), library);
    }
    
    public static void loadJambiLibrary(String library) {
        NativeLibraryManager.loadJambiLibrary(stackWalker.getCallerClass(), null, library);
    }

    public static void loadLibrary(String lib) {
        NativeLibraryManager.loadLibrary(lib);
    }

    public static File jambiTempDir() {
        return NativeLibraryManager.jambiTempDir();
    }
    
    public static String qtPrefix() {
    	return QtJambiInternal.qtPrefix();
    }
    
    public static boolean initializePackage(String packagePath){
    	return QtJambiInternal.initializePackage(packagePath);
    }
    
    public static boolean initializePackage(java.lang.Package pkg){
    	return QtJambiInternal.initializePackage(pkg.getName());
    }
    
    public static boolean initializePackage(java.lang.Class<?> cls){
    	return QtJambiInternal.initializePackage(cls.getPackageName());
    }
    
    public static QMetaObject.DisposedSignal getSignalOnDispose(QtObjectInterface object) {
    	return QtJambiInternal.getSignalOnDispose(object, true);
    }
    
    public static void threadCheck(QObject object) {
    	QtJambiInternal.threadCheck(object);
    }
    
    public static void threadCheck(QWidget object) {
    	QtJambiInternal.threadCheck(object);
    }
    
    public static void initializeNativeObject(QtObjectInterface object, QtArgument.Stream arguments) {
    	QtJambiInternal.initializeNativeObject(object, arguments.arguments());
    }
}
