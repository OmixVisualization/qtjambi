/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2022 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
** 
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
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
** 
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

package io.qt.tools.ant;

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
        MacOS ("macos"),
        Android ("android"),
        IOS ("ios");

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

    public static final String K_WIN_X86 = "windows-x86";
    public static final String K_WIN_X64 = "windows-x64";
    public static final String K_WIN_ARM32 = "windows-arm";
    public static final String K_WIN_ARM64 = "windows-arm64";

    public static final String K_LINUX_X86 = "linux-x86";
    public static final String K_LINUX_X64 = "linux-x64";
    public static final String K_LINUX_ARM32 = "linux-arm";
    public static final String K_LINUX_ARM64 = "linux-arm64";
    
    public static final String K_ANDROID_X86 = "android-x86";
    public static final String K_ANDROID_X64 = "android-x64";
    public static final String K_ANDROID_ARM32 = "android-arm";
    public static final String K_ANDROID_ARM64 = "android-arm64";

    public static final String K_WINDOWS = "windows";
    public static final String K_ANDROID = "android";
    public static final String K_LINUX = "linux";
    public static final String K_MACOS = "macos";
    public static final String K_IOS = "ios";

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
            else if (osname.contains("mac os") || osname.contains("macos"))
                os = OS.MacOS;
            else if (osname.contains("android"))
                os = OS.Android;
            else
                os = OS.Unknown;
        }
        return os;
    }
    
    public static OS crossOS() {
    	if(crossOS==null) {
    		return os();
    	}
    	else {
    		return crossOS;
    	}
    }
    
    public static String crossOSArchName() {
    	if(crossOSArchName==null) {
    		return osArchName();
    	}
    	else {
    		return crossOSArchName;
    	}
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
            	switch(System.getProperty("os.arch").toLowerCase()) {
            	case "arm":
            	case "arm32":
            		osArchName = K_WIN_ARM32; break;
            	case "arm64":
            	case "aarch64":
            		osArchName = K_WIN_ARM64; break;
            	case "x86_64":
            	case "x64":
            	case "amd64":
            		osArchName = K_WIN_X64; break;
            	default:
            		osArchName = K_WIN_X86; break;
            	}
                break;
            case Linux:
        		switch(System.getProperty("os.arch").toLowerCase()) {
            	case "arm":
            	case "arm32":
            		osArchName = K_LINUX_ARM32; break;
            	case "arm64":
            	case "aarch64":
            		osArchName = K_LINUX_ARM64; break;
            	case "x86_64":
            	case "x64":
            	case "amd64":
            		osArchName = K_LINUX_X64; break;
            	default:
            		osArchName = K_LINUX_X86; break;            	
            	}
                break;
            case MacOS:
                osArchName = "macos";
                break;
            case Android:
                osArchName = "android";
                break;
            case IOS:
                osArchName = "ios";
                break;
            case Unknown:
                osArchName = "unknown";
                break;
            }
        }
        return osArchName;
    }

    static void setQMakeXSpec(String qmakeXSpec) {
    	if(qmakeXSpec==null) {
    		crossOS = null;
    		crossOSArchName = null;    		
    	}else {
	    	switch(qmakeXSpec) {
	    	case "android-clang":
	    		crossOS = OS.Android;
	    		crossOSArchName = "android";
	    		break;
	    	case "macx-ios-clang":
	    		crossOS = OS.IOS;
	    		crossOSArchName = "ios";
	    		break;
			default:
	    		crossOS = null;
	    		crossOSArchName = null;
				break;
	    	}
    	}
    }
    
    static void setCrossOSArchName(String crossOSArchName) {
		OSInfo.crossOSArchName = crossOSArchName;
	}

    private static OS crossOS;
    private static String crossOSArchName;
    private static OS os;
    private static String osArchName;
}

