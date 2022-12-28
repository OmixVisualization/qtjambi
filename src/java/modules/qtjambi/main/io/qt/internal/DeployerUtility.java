/****************************************************************************
**
** Copyright (C) 2009-2022 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import java.util.Map;

import io.qt.core.QPair;

public abstract class DeployerUtility {

	private DeployerUtility() {
		throw new RuntimeException();
	}

	public static native Map<String,QPair<Long,Long>> getRegisteredTypeSizesAndAlignments();
	
	public static native String getInterfaceIID(Class<?> cls);

	public static String qtLibraryPath() {
		return LibraryUtility.qtLibraryPath();
	}
	
	public static String qtJambiLibraryPath() {
		return LibraryUtility.qtJambiLibraryPath();
	}
	
	public static String osArchName() {
		return LibraryUtility.osArchName();
	}
	
	public static boolean isMinGWBuilt() {
		return LibraryUtility.isMinGWBuilt();
	}
}
