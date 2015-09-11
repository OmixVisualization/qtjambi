/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
** 
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
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
** 
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
** $END_LICENSE$

**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

package org.qtjambi.qt.osinfo;

/**
 * The OSInfo class contains some basic information about the current
 * running system. The information is mostly retreived from
 * System.getProperty() and similar.
 */
public class OSInfo
{
    public enum OS
    {
        Unknown ("unkwnown"),
        Windows ("windows"),
        Linux ("linux"),
        MacOS ("macosx"),
        Solaris ("sunos"),
        FreeBSD ("freebsd");

        private String name;

        OS(final String name)
        {
            this.name = name;
        }

        @Override
        public String toString()
        {
            return name;
        }
    }

    public static final String K_SUNOS32 = "sunos32";
    public static final String K_SUNOS64 = "sunos64";

    public static final String K_WIN32 = "win32";
    public static final String K_WIN64 = "win64";

    public static final String K_LINUX32 = "linux32";
    public static final String K_LINUX64 = "linux64";

    public static final String K_FREEBSD32 = "freebsd32";
    public static final String K_FREEBSD64 = "freebsd64";

    public static final String K_SUNOS = "sunos";
    public static final String K_WINDOWS = "windows";
    public static final String K_LINUX = "linux";
    public static final String K_FREEBSD = "freebsd";
    public static final String K_MACOSX = "macosx";

    /**
     * Returns the operating system
     */
    public static OS os() {
        if (os == null) {
            String osname = System.getProperty("os.name").toLowerCase();
            if (osname.contains("linux"))
                os = OS.Linux;
            else if (osname.contains("windows"))
                os = OS.Windows;
            else if (osname.contains("mac os x"))
                os = OS.MacOS;
            else if (osname.contains("sunos"))
                os = OS.Solaris;
            else if (osname.contains("freebsd"))
                os = OS.FreeBSD;
            else
                os = OS.Unknown;
        }
        return os;
    }


    /**
     * Returns a string containing the operating system and
     * architecture name
     *
     * @return e.g. "win32" or "linux64"..
     */
    public static String osArchName() {
        if (osArchName == null) {
            switch (os()) {
            case Windows:
                osArchName = System.getProperty("os.arch").equalsIgnoreCase("amd64")
                             ? K_WIN64
                             : K_WIN32;
                break;
            case Linux:
                osArchName = System.getProperty("os.arch").equalsIgnoreCase("amd64")
                             ? K_LINUX64
                             : K_LINUX32;
                break;
            case FreeBSD:
                osArchName = System.getProperty("os.arch").equalsIgnoreCase("amd64")
                             ? K_FREEBSD64
                             : K_FREEBSD32;
                break;
            case MacOS:
                osArchName = "macosx";
                break;
            case Solaris:
                // sparc || sparcv9
                osArchName = System.getProperty("os.arch").equalsIgnoreCase("sparcv9")
                             ? K_SUNOS32
                             : K_SUNOS64;
                break;
            case Unknown:
                osArchName = "unknown";
                break;
            }
        }
        return osArchName;
    }

    public static boolean isWindows() {
        if(os() == OS.Windows)
            return true;
        return false;
    }

    public static boolean isLinux() {
        if(os() == OS.Linux)
            return true;
        return false;
    }

    public static boolean isMacOS() {
        if(os() == OS.MacOS)
            return true;
        return false;
    }

    public static boolean isFreeBSD() {
        if(os() == OS.FreeBSD)
            return true;
        return false;
    }

    public static boolean isSolaris() {
        if(os() == OS.Solaris)
            return true;
        return false;
    }

    public static Boolean is64bit() {
        String s = osArchName();
        if(K_LINUX64.equals(s))
            return Boolean.TRUE;
        if(K_WIN64.equals(s))
           return Boolean.TRUE;
        if(K_MACOSX.equals(s))
            return Boolean.TRUE;
        if(K_FREEBSD64.equals(s))
            return Boolean.TRUE;
        if(K_SUNOS64.equals(s))
            return Boolean.TRUE;
        return Boolean.FALSE;
    }


    private static OS os;
    private static String osArchName;
}

