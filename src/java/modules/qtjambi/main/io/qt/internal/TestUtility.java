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

package io.qt.internal;

import io.qt.QtObjectInterface;
import io.qt.core.QMetaMethod;
import io.qt.core.QMetaObject;
import io.qt.internal.NativeUtility.NativeLink;

/**
 * @hidden
 */
public abstract class TestUtility {
	static {
		QtJambi_LibraryUtilities.initialize();
	}

	private TestUtility() {
		throw new RuntimeException();
	}

	/**
	 * Testing method...
	 */
	public static boolean tryIsObjectDisposed(QtObjectInterface object) {
		NativeLink lnk = NativeUtility.findInterfaceLink(object, false);
		return lnk == null || lnk.isDisposed();
	}

	private native static boolean isSharedPointer(long nativeId);

	public static boolean isSharedPointer(QtObjectInterface object) {
		return isSharedPointer(NativeUtility.checkedNativeId(object));
	}
    
    public static native boolean hasDeleteLaterEvents();

    public static native boolean hasDebugTools();
    
    public static native int objectCount();

	public static native int objectCacheMode();

	public static native void setObjectCacheMode(int objectCacheMode);
    
    public static native void flushOut();
    
    public static native void flushErr();

	public static final int OBJECT_CACHE_MODE_DISABLE = 0;
	
	public static final int OBJECT_CACHE_MODE_LIMITED = 1;
	
	public static final int OBJECT_CACHE_MODE_DEFAULT = 2;

	public static boolean isDebugBuild() {
		return LibraryUtility.configuration() == LibraryBundle.Configuration.Debug;
	}
	
	public static String processName() {
		return RetroHelper.processName();
	}
	
	public static boolean isMinGWBuilt() {
		return LibraryUtility.isMinGWBuilt();
	}
	
	public static String qtLibraryPath() {
		return LibraryUtility.qtLibraryPath();
	}
	
	public static String qtJambiLibraryPath() {
		return LibraryUtility.qtJambiLibraryPath();
	}
	
	public static String osArchName() {
		return LibraryUtility.osArchName();
	}
	
	public static QMetaMethod signal(QMetaObject.Connection connection) {
		return SignalUtility.signal(connection);
	}
}
