/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import java.util.Collections;
import java.util.Map;
import java.util.TreeMap;

import org.apache.tools.ant.Project;

import io.qt.tools.ant.FindCompiler.Compiler;

/**
 * The OSInfo class contains some basic information about the current
 * running system. The information is mostly retreived from
 * System.getProperty() and similar.
 */
public class OSInfo
{
    public enum OperationSystem
    {
        Unknown,
        Windows,
        Linux,
        MacOS,
        Android,
        IOS,
        FreeBSD,
        NetBSD,
        OpenBSD,
        Solaris
        ;
    	boolean isAndroid() {
			return this==Android;
		}
    	boolean isUnixLike() {
			switch (this) {
			case FreeBSD:
			case NetBSD:
			case OpenBSD:
			case Solaris:
			case Linux:
				return true;
			default:
				return false;
			}
		}
    }
    
    enum Architecture{
		x86,
		x86_64,
		arm,
		arm64,
		sparc,
		mips,
		mips64,
		riscv64
	}

    /**
     * Returns the operating system
     */
    public OperationSystem os() {
        if (os == null) {
            String osname = System.getProperty("os.name").toLowerCase();
            if (osname.contains("linux")) {
            	if(System.getProperty("java.runtime.name").toLowerCase().startsWith("android"))
            		os = OperationSystem.Android;
            	else
            		os = OperationSystem.Linux;
            }else if (osname.contains("windows"))
                os = OperationSystem.Windows;
            else if (osname.startsWith("freebsd")) 
            	os = OperationSystem.FreeBSD;
            else if (osname.startsWith("netbsd")) 
            	os = OperationSystem.NetBSD;
            else if (osname.startsWith("openbsd")) 
            	os = OperationSystem.OpenBSD;
            else if (osname.startsWith("solaris") || osname.startsWith("sunos")) 
            	os = OperationSystem.Solaris;
            else if (osname.contains("mac os") || osname.contains("macos") || osname.contains("darwin"))
                os = OperationSystem.MacOS;
            else
                os = OperationSystem.Unknown;
        }
        return os;
    }

	public Architecture arch() {
		if(arch == null) {
	    	String osarch = System.getProperty("os.arch").toLowerCase();
	    	switch(osarch) {
        	case "arm":
        	case "armv":
        	case "armv6":
        	case "armv7":
        	case "arm32":
        		arch = Architecture.arm; break;
        	case "arm64":
        	case "aarch64":
        		arch = Architecture.arm64; break;
        	case "x86_64":
        	case "x64":
        	case "amd64":
        		arch = Architecture.x86_64; break;
        	case "x86":
        	case "i386":
        		arch = Architecture.x86; break;
        	case "sparc":
        		arch = Architecture.sparc; break;
        	case "mips":
        		arch = Architecture.mips; break;
        	case "mips64":
        		arch = Architecture.mips64; break;
        	case "riscv64":
        		arch = Architecture.riscv64; break;
        	default:
        		if(osarch.startsWith("arm-"))
        			arch = Architecture.arm;
        		else if(osarch.startsWith("aarch64-"))
        			arch = Architecture.arm64;
        		else
        			arch = Architecture.x86_64;
        		break;
        	}
		}
		return arch;
	}
    
    public OperationSystem crossOS() {
    	if(crossOS==null) {
    		return os();
    	}
    	else {
    		return crossOS;
    	}
    }
    
    public String targetPlatform(Compiler compiler) {
    	if(crossOS==null) {
    		switch (os()) {
            case Windows:
            	switch(compiler) {
            	case MSVC2019_arm64:
            	case MSVC2022_arm64:
            	case MSVC20XX_arm64:
            		return os().name().toLowerCase()+"-arm64";
            	case MinGW:
            	case MinGW_W64:
	            	switch(arch()) {
	            	case x86_64:
	            		return os().name().toLowerCase()+"-mingw-x64";
	            	default:
	            		return os().name().toLowerCase()+"-mingw-"+arch().name().toLowerCase();
	            	}
            	case LLVM_MinGW_W64:
	            	switch(arch()) {
	            	case x86_64:
	            		return os().name().toLowerCase()+"-llvm-mingw-x64";
	            	default:
	            		return os().name().toLowerCase()+"-llvm-mingw-"+arch().name().toLowerCase();
	            	}
	            	default:
            	}
            case Linux:
            case FreeBSD:
            case NetBSD:
            case OpenBSD:
            case Solaris:
            case Android:
            	switch(arch()) {
            	case x86_64:
            		return os().name().toLowerCase()+"-x64";
            	default:
            		return os().name().toLowerCase()+"-"+arch().name().toLowerCase();
            	}
            case MacOS:
                return "macos";
            case IOS:
            	return "ios";
            default:
            	return "unknown";
            }
    	}else {
	    	switch (crossOS) {
	        case Windows:
	        	switch(compiler) {
            	case MinGW:
            	case MinGW_W64:
	            	switch(arch()) {
	            	case x86_64:
	            		return os().name().toLowerCase()+"-mingw-x64";
	            	default:
	            		return os().name().toLowerCase()+"-mingw-"+arch().name().toLowerCase();
	            	}
            	case LLVM_MinGW_W64:
	            	switch(arch()) {
	            	case x86_64:
	            		return os().name().toLowerCase()+"-llvm-mingw-x64";
	            	default:
	            		return os().name().toLowerCase()+"-llvm-mingw-"+arch().name().toLowerCase();
	            	}
	            	default:
            	}
	        case Linux:
	        case FreeBSD:
	        case NetBSD:
	        case OpenBSD:
            case Solaris:
	        case Android:
	        	if(crossArch!=null) {
	            	switch(crossArch) {
	            	case x86_64:
	            		return crossOS.name().toLowerCase()+"-x64";
	            	default:
	            		return crossOS.name().toLowerCase()+"-"+crossArch.name().toLowerCase();
	            	}
	        	}else {
	        		return crossOS.name().toLowerCase();
	        	}
	        case MacOS:
	            return "macos";
	        case IOS:
	        	return "ios";
        	default:
	        	return "unknown";
	        }
    	}
    }
    
    public Architecture crossArch() {
    	if(crossOS==null) {
    		return null;
    	}else if(crossOS==OperationSystem.Android) {
    		return crossArch;
    	}else {
	    	if(crossArch==null) {
	    		return arch();
	    	}
	    	else {
	    		return crossArch;
	    	}
    	}
    }

    void setQMakeXSpec(String qmakeXSpec) {
    	if(qmakeXSpec==null) {
    		crossOS = null;
    	}else {
	    	switch(qmakeXSpec) {
	    	case "linux-arm-gnueabi-g++":
	    		crossOS = OperationSystem.Linux;
	    		crossArch = Architecture.arm;
	    		break;
	    	case "linux-aarch64-gnu-g++":
	    		crossOS = OperationSystem.Linux;
	    		crossArch = Architecture.arm64;
	    		break;
	    	case "linux-g++-32":
	    		crossOS = OperationSystem.Linux;
	    		crossArch = Architecture.x86;
	    		break;
	    	case "linux-g++-64":
	    		crossOS = OperationSystem.Linux;
	    		crossArch = Architecture.x86_64;
	    		break;
	    	case "win32-arm64-msvc":
	    		crossOS = OperationSystem.Windows;
	    		crossArch = Architecture.arm64;
	    		break;
	    	case "win32-x64-msvc":
	    		crossOS = OperationSystem.Windows;
	    		crossArch = Architecture.x86_64;
	    		break;
	    	case "freebsd-clang":
	    	case "freebsd-g++":
	    		crossOS = OperationSystem.FreeBSD;
	    		crossArch = Architecture.x86_64;
	    		break;
	    	case "netbsd-clang":
	    	case "netbsd-g++":
	    		crossOS = OperationSystem.NetBSD;
	    		crossArch = Architecture.x86_64;
	    		break;
	    	case "openbsd-clang":
	    	case "openbsd-g++":
	    		crossOS = OperationSystem.OpenBSD;
	    		crossArch = Architecture.x86_64;
	    		break;
	    	case "solaris-cc-64":
	    	case "solaris-cc-64-stlport":
	    	case "solaris-g++-64":
	    		crossOS = OperationSystem.Solaris;
	    		crossArch = Architecture.x86_64;
	    		break;
	    	case "android-clang":
	    		crossOS = OperationSystem.Android;
	    		crossArch = null;
	    		break;
	    	case "macx-ios-clang":
	    	case "macx-tvos-clang":
	    	case "macx-watchos-clang":
	    		crossOS = OperationSystem.IOS;
	    		crossArch = null;
	    		break;
			default:
	    		crossOS = null;
	    		crossArch = null;
				break;
	    	}
    	}
    }
    
    void setCrossCompilation(OperationSystem _crossOS, Architecture _crossArch) {
    	crossOS = _crossOS;
    	crossArch = _crossArch;
    }
    
    private OperationSystem crossOS;
    private Architecture crossArch;
    private OperationSystem os;
    private Architecture arch;
    
    public final String id = "qtjambi.osinfo-"+Integer.toHexString(System.identityHashCode(this));
    
    private static final Map<String,OSInfo> osInfos = Collections.synchronizedMap(new TreeMap<>());
    
    public static OSInfo instance(Project project) {
    	return osInfos.get(project.getProperty("qtjambi.osinfo"));
	}
    
    public static void cleanupOSInfo(Project project) {
    	osInfos.remove(project.getProperty("qtjambi.osinfo"));
    }
    
    public static String initializeNewOSInfo(Project project) {
    	OSInfo localOSInfo = new OSInfo();
    	osInfos.put(localOSInfo.id, localOSInfo);
		return localOSInfo.id;
    }
}

