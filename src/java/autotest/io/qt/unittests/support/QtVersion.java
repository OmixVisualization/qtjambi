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
package io.qt.unittests.support;

import io.qt.internal.NativeLibraryManager;

public class QtVersion implements Comparable<QtVersion> {
	public static final QtVersion VERSION_4_5		= new QtVersion("4.5");
	public static final QtVersion VERSION_4_6		= new QtVersion("4.6");
	public static final QtVersion VERSION_4_7		= new QtVersion("4.7");
	public static final QtVersion VERSION_4_8		= new QtVersion("4.8");
	public static final QtVersion VERSION_5			= new QtVersion("5");

	private final String versionString;
	private final int[] versionA;
	
	public QtVersion(String version) {
		if(version == null)
			throw new NullPointerException();
		this.versionString = version;
		this.versionA = NativeLibraryManager.getVersion(version);
		if(versionA == null || versionA.length == 0)
			throw new RuntimeException("invalid version");
	}

	private static volatile QtVersion staticQtVersion;

	/**
	 * Gets the default version based on the runtime execution environment.
	 * @return
	 */
	public static QtVersion getQtVersion() {
		QtVersion tmpQtVersion = staticQtVersion;
		if(tmpQtVersion == null) {
			synchronized (QtVersion.class) {
				tmpQtVersion = staticQtVersion;  // volatile forces reload
				if(tmpQtVersion == null) {
					tmpQtVersion = new QtVersion(NativeLibraryManager.VERSION_STRING);
					staticQtVersion = tmpQtVersion;
				}
			}
		}
		return tmpQtVersion;
	}

	public int[] getVersionA() {
		int[] vA = new int[versionA.length];
		System.arraycopy(versionA, 0, vA, 0, versionA.length);
		return vA;
	}

	public String getVersion() {
		return versionString;
	}

	public boolean equals(Object a) {
		if(a == null)
			return false;
		if((a instanceof QtVersion) == false)
			return false;
		QtVersion aa = (QtVersion) a;
		return versionString.equals(aa.versionString);
	}

	public int compareTo(QtVersion o) {
		int diff = 0;
		int i = 0;
		while(true) {
			if(o.versionA.length <= i && versionA.length <= i)
				break;

			int a;
			if(versionA.length > i)
				a = versionA[i];
			else
				a = 0;

			int b;
			if(o.versionA.length > i)
				b = o.versionA[i];
			else
				b = 0;

			diff += a - b;
			if(diff != 0)
				break;

			i++;
		}
		return diff;
	}

	@Override
	public String toString() {
		return versionString;
	}
}