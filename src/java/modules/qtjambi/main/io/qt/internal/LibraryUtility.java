/****************************************************************************
**
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import java.io.*;
import java.net.*;
import java.nio.*;
import java.nio.charset.*;
import java.nio.file.*;
import java.util.*;
import java.util.concurrent.*;
import java.util.concurrent.atomic.*;
import java.util.function.*;
import java.util.logging.*;
import java.util.prefs.*;
import java.util.zip.*;
import javax.xml.parsers.*;

import org.w3c.dom.Document;
import org.xml.sax.*;
import io.qt.*;
import io.qt.QtUtilities.*;
import io.qt.core.*;
import io.qt.internal.LibraryBundle.*;

/**
 * @hidden
 */
final class LibraryUtility {
	
	private final static Logger logger = Logger.getLogger("io.qt.internal");
	
	enum Architecture{
		x86,
		x86_64,
		arm,
		arm64,
		sparc,
		mips,
		mips64;
		
	    static Architecture decideArchitecture() {
	    	String arch = System.getProperty("os.arch").toLowerCase();
	    	switch(arch) {
	    	case "arm":
	    	case "armv":
	    	case "armv6":
	    	case "armv7":
	    	case "arm32":
	    		return arm;
	    	case "arm64":
	    	case "aarch64":
	    		return arm64;
	    	case "x86_64":
	    	case "x64":
	    	case "amd64":
	    		return x86_64;
	    	case "i386":
	    	case "x86":
	    		return x86;
        	case "sparc":
        		return sparc;
        	case "mips":
        		return mips;
        	case "mips64":
        		return mips64;
	    	default:
	    		if(arch.startsWith("arm-"))
	    			return arm;
	    		else if(arch.startsWith("aarch64-"))
	    			return arm64;
	    		else 
	    			return x86;
	    	}
	    }
	}
	
	enum OperatingSystem {
	    Windows,
	    MacOS,
	    Linux,
	    Android,
	    IOS,
	    FreeBSD,
	    NetBSD,
	    OpenBSD,
	    Solaris
	    ;
		final boolean isAndroid() {
			return this==Android;
		}
		final boolean isUnixLike() {
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

	    static OperatingSystem decideOperatingSystem() {
	        String osName = System.getProperty("os.name").toLowerCase();
	        if (osName.startsWith("windows")) return Windows;
	        else if (osName.startsWith("mac")) return MacOS;
	        else if (osName.startsWith("ios")) return IOS;
	        else if (osName.startsWith("freebsd")) return FreeBSD;
	        else if (osName.startsWith("netbsd")) return NetBSD;
	        else if (osName.startsWith("openbsd")) return OpenBSD;
	        else if (osName.startsWith("solaris") || osName.startsWith("sunos")) return Solaris;
	        else if (osName.startsWith("linux")) {
	        	if(System.getProperty("java.runtime.name").toLowerCase().startsWith("android"))
	        		return OperatingSystem.Android;
	        	else 
	        		return OperatingSystem.Linux;
	        }
	        return OperatingSystem.Linux;
	    }
	}
	
	private LibraryUtility() { throw new RuntimeException();}
	
	static final String DEPLOY_XML = "META-INF/qtjambi-deployment.xml";
	static final String DEPLOY_XML_IN_JAR = "!/"+DEPLOY_XML;
	
	public static final String ICU_VERSION;
    public static final String LIBINFIX;
    private static final List<String> systemLibrariesList;
    private static final List<String> jniLibdirBeforeList;
    private static final List<String> jniLibdirList;
    private static Boolean isMinGWBuilt = null;
    
	static final OperatingSystem operatingSystem = OperatingSystem.decideOperatingSystem();
	static final Architecture architecture = Architecture.decideArchitecture();
    static final String osArchName;
    
    private static LibraryBundle.Configuration configuration = null;
    private static File qtJambiLibraryPath = null;
    private static File qtLibraryPath = null;
	private static Boolean dontUseQtJambiFrameworks = operatingSystem==OperatingSystem.MacOS ? null : Boolean.FALSE;
	private static Boolean dontUseQtFrameworks = operatingSystem==OperatingSystem.MacOS ? null : Boolean.FALSE;
    
    private static boolean dontSearchDeploymentSpec = false;
    private static final boolean useStaticLibs = operatingSystem==OperatingSystem.IOS 
    											 || Boolean.getBoolean("io.qt.staticlibs");
    private static final boolean debugInfoDeployment = Boolean.getBoolean("io.qt.provide-debuginfo");
    private static final boolean noNativeDeployment = Boolean.getBoolean("io.qt.no-native-deployment") 
    		|| Boolean.getBoolean("qtjambi.no-native-deployment")
    		|| Boolean.getBoolean("io.qt.no-deployment-spec") 
    		|| Boolean.getBoolean("qtjambi.no-deployment-spec")
    		|| operatingSystem==OperatingSystem.Android
    		|| useStaticLibs;
    private static final Set<URL> loadedNativeDeploymentUrls = new HashSet<>();
    private static final List<LibraryBundle> nativeDeployments = new ArrayList<>();

    private static final File jambiDeploymentDir;
    private static final File jambiSourcesDir;
    private static final File jambiHeadersDir;
    private static final File jambiJarDir;
    private static final boolean isMavenRepo, isGradleCache, isNativeSubdir, isDebuginfoSubdir;
    private static final List<Library> pluginLibraries = Collections.synchronizedList(new ArrayList<>());

    private static final boolean deleteTmpDeployment;
    
    private static final Map<String,List<String>> javaLibraryPaths = Collections.synchronizedMap(new TreeMap<>());
    private static final Map<String,List<String>> ldLibraryPaths = Collections.synchronizedMap(new TreeMap<>());
    private static final Map<String,List<String>> javaLibraryPathOverrides = Collections.synchronizedMap(new TreeMap<>());
    
    private static final AtomicBoolean isQmlLoaded = new AtomicBoolean(false);
    
    private static class LibVersion{
        LibVersion(int qtMajorVersion, int qtMinorVersion, int qtJambiPatch) {
			this.qtMajorVersion = qtMajorVersion;
			this.qtMinorVersion = qtMinorVersion;
			this.qtJambiPatch = qtJambiPatch;
		}
		final int qtMajorVersion;
        final int qtMinorVersion;
        final int qtJambiPatch;
    }
    private static final Map<Class<?>,LibVersion> qtJambiVersionsByClass = Collections.synchronizedMap(new HashMap<>());
    
    private static final Set<String> loadedLibraries = Collections.synchronizedSet(new TreeSet<>());
    
	private static BiConsumer<Class<?>, String> libraryLoader;
    
    static {
    	libraryLoader = (callerClass, lib) -> {
    		synchronized(LibraryUtility.class) {
				boolean found = false;
    			try {
					if(operatingSystem==OperatingSystem.Android) {
						@SuppressWarnings({ "rawtypes", "unchecked" })
			    		QDeclarableSignals.Signal2<String, ClassLoader> loadLibrary = new QDeclarableSignals.Signal2(String.class, ClassLoader.class);
						QMetaObject.Connection connection = loadLibrary.connect(Runtime.getRuntime(), "load(String, ClassLoader)");
						if(connection.isConnected()) {
							libraryLoader = (_callerClass, _lib)->loadLibrary.emit(_lib, _callerClass.getClassLoader());
							found = true;
						}
					}else {
						@SuppressWarnings({ "rawtypes", "unchecked" })
			    		QDeclarableSignals.Signal2<Class<?>, String> loadLibrary = new QDeclarableSignals.Signal2(Class.class, String.class);
						QMetaObject.Connection connection = loadLibrary.connect(Runtime.getRuntime(), "load0(Class, String)");
						if(connection.isConnected()) {
							libraryLoader = loadLibrary::emit;
							found = true;
						}
					}
				} catch (QNoSuchSlotException e) {
				}
    			if(!found)
    				libraryLoader = (_callerClass, _lib)->Runtime.getRuntime().load(_lib);
    		}
    		libraryLoader.accept(callerClass, lib);
    	};
    	
        String icuVersion = null;
        String libInfix = null;
        List<String> systemLibraries = null;
        List<String> jniLibdirsPrepended = null;
        List<String> jniLibdirs = null;
        try {
            libInfix = QtJambi_LibraryUtilities.properties.getProperty("qtjambi.libinfix");
            if(QtJambi_LibraryUtilities.qtMajorVersion>6 || (Integer.compare(QtJambi_LibraryUtilities.qtMajorVersion, 6)==0 && QtJambi_LibraryUtilities.qtMinorVersion>=7)) {
            	icuVersion = QtJambi_LibraryUtilities.properties.getProperty("qtjambi.icu.version", "73.2");
            }else {
            	icuVersion = QtJambi_LibraryUtilities.properties.getProperty("qtjambi.icu.version", "56.1");
            }
            
            SortedMap<String,String> tmpSystemLibrariesMap = new TreeMap<String,String>();
            SortedMap<String,String> tmpJniLibdirBeforeMap = new TreeMap<String,String>();
            SortedMap<String,String> tmpJniLibdirMap = new TreeMap<String,String>();
            Enumeration<? extends Object> e = QtJambi_LibraryUtilities.properties.propertyNames();
            while (e.hasMoreElements()) {
                String key = (String) e.nextElement();
                String value = QtJambi_LibraryUtilities.properties.getProperty(key);
                if (key.equals("qtjambi.system.libraries") || key.startsWith("qtjambi.system.libraries.")) {
                    tmpSystemLibrariesMap.put(key, value);
                } else if(key.equals("qtjambi.jni.libdir.before") || key.startsWith("qtjambi.jni.libdir.before.")) {
                    tmpJniLibdirBeforeMap.put(key, value);
                } else if(key.equals("qtjambi.jni.libdir") || key.startsWith("qtjambi.jni.libdir.")) {
                   tmpJniLibdirMap.put(key, value);
                }
            }
            // Map will automatically sort the lists { "", ".0", ".01", ".1", ".10", ".2", ".A", ".a" }
            systemLibraries = new ArrayList<String>();
            for (String v : tmpSystemLibrariesMap.values())
                systemLibraries.add(v);

            if (systemLibraries.size() > 0)
                systemLibraries = resolveSystemLibraries(systemLibraries);
            else
                systemLibraries = null;

            jniLibdirsPrepended = new ArrayList<String>();
            for (String v : tmpJniLibdirBeforeMap.values())
                jniLibdirsPrepended.add(v);

            if (jniLibdirsPrepended.size() > 0)
                jniLibdirsPrepended = Collections.unmodifiableList(jniLibdirsPrepended);
            else
                jniLibdirsPrepended = null;

            jniLibdirs = new ArrayList<String>();
            for (String v : tmpJniLibdirMap.values())
                jniLibdirs.add(v);

            if (jniLibdirs.size() > 0)
                jniLibdirs = Collections.unmodifiableList(jniLibdirs);
            else
                jniLibdirs = null;
        } catch(Throwable e) {
            logger.log(java.util.logging.Level.SEVERE, "", e);
        }finally {
        	ICU_VERSION = icuVersion;
            systemLibrariesList = systemLibraries!=null ? Collections.unmodifiableList(systemLibraries) : Collections.emptyList();
            jniLibdirBeforeList = jniLibdirsPrepended!=null ? Collections.unmodifiableList(jniLibdirsPrepended) : Collections.emptyList();
            jniLibdirList = jniLibdirs!=null ? Collections.unmodifiableList(jniLibdirs) : Collections.emptyList();
            LIBINFIX = libInfix;
        }

        boolean isOSNameSpecified = false;
        String _osArchName;
        switch (operatingSystem) {
        case MacOS:
        case IOS:
            _osArchName = operatingSystem.name().toLowerCase();
            break;
        default:
        	switch(architecture) {
        	case x86_64:
        		_osArchName = operatingSystem.name().toLowerCase()+"-x64"; 
        		switch (operatingSystem) {
                case Windows:
    	            	if((isOSNameSpecified = System.getProperty("qtjambi.osname", "").endsWith("mingw-x64")))
    	            		_osArchName = System.getProperty("qtjambi.osname", "");
    	            break;
    	            default:
                }
        		break;
        	default:
        		_osArchName = operatingSystem.name().toLowerCase()+"-"+architecture.name().toLowerCase(); break;
        	}
            break;
        }
        
        if(operatingSystem!=OperatingSystem.Android) {
	        try{
	        	Preferences preferences = Preferences.userNodeForPackage(LibraryUtility.class);
	        	Preferences pids = preferences.node("qtjambi.pids");
	        	for(String pid : pids.keys()) {
	        		if(!RetroHelper.isProcessAlive(pid)) {
	        			String dir = pids.get(pid, "");
	        			File jambiTempDir = new File(dir);
	        			if(jambiTempDir.exists() && jambiTempDir.isDirectory()) {
	        				clearAndDelete(jambiTempDir);
	        			}
        				pids.remove(pid);
        				pids.sync();
	        		}
	        	}
	        	if(pids.keys().length==0) {
	        		preferences.remove("qtjambi.pids");
	        		preferences.sync();
	        	}
	    		String dirs = preferences.get("qtjambi.previous.deployment.dir", null);
	    		if(dirs!=null && !dirs.isEmpty()) {
	    	        preferences.remove("qtjambi.previous.deployment.dir");
	    	        for(String dir : dirs.split("\\"+File.pathSeparator)) {
	        			File jambiTempDir = new File(dir);
	        			if(jambiTempDir.exists() && jambiTempDir.isDirectory())
	        				clearAndDelete(jambiTempDir);
	    	        }
	    		}
	        }catch(Throwable t) {}
        }
        try {
	        boolean loadQtJambiFromLibraryPath = noNativeDeployment;
	        boolean loadQtFromLibraryPath = loadQtJambiFromLibraryPath;
	        File jambiLibraryDir = null;
	        if(!loadQtJambiFromLibraryPath){
	            List<String> libraryPaths = new ArrayList<>();
	            if (System.getProperties().containsKey("io.qt.library-path-override")) {
	            	libraryPaths.addAll(javaLibraryPathOverrides.computeIfAbsent(System.getProperty("io.qt.library-path-override"), LibraryUtility::splitPath));
	        	} else {
	        		libraryPaths.addAll(javaLibraryPaths.computeIfAbsent(System.getProperty("java.library.path"), LibraryUtility::splitPath));
		            switch(operatingSystem) {
					case IOS:
					case MacOS:
						String ldPath = System.getenv("DYLD_FRAMEWORK_PATH");
						if(ldPath!=null)
							libraryPaths.addAll(ldLibraryPaths.computeIfAbsent(ldPath, LibraryUtility::splitPath));
						ldPath = System.getenv("DYLD_LIBRARY_PATH");
						if(ldPath!=null)
							libraryPaths.addAll(ldLibraryPaths.computeIfAbsent(ldPath, LibraryUtility::splitPath));
						break;
					default:
						if(operatingSystem.isUnixLike()) {
							ldPath = System.getenv("LD_LIBRARY_PATH");
							if(ldPath!=null)
								libraryPaths.addAll(ldLibraryPaths.computeIfAbsent(ldPath, LibraryUtility::splitPath));
							break;
						}
						break;
		            }
	    		}
	            libraryPaths = mergeJniLibdir(libraryPaths);
	
		    	List<String> replacements = new ArrayList<>();
		    	configuration = decideConfiguration();
		    	if(configuration!=null) {
		    		String libFormat = qtjambiLibraryName(null, "QtJambi", false, configuration, replacements, QtJambi_LibraryUtilities.qtMajorVersion, QtJambi_LibraryUtilities.qtMinorVersion, QtJambi_LibraryUtilities.qtJambiPatch);
		    		String libFormatNoFw = null;
		    		if(operatingSystem==OperatingSystem.MacOS) {
			    		if(dontUseQtJambiFrameworks==null) {
			    			libFormatNoFw = qtjambiLibraryName(null, "QtJambi", true, configuration, replacements, QtJambi_LibraryUtilities.qtMajorVersion, QtJambi_LibraryUtilities.qtMinorVersion, QtJambi_LibraryUtilities.qtJambiPatch);
			    		}else if(dontUseQtJambiFrameworks) {
			    			libFormat = null;
			    			libFormatNoFw = qtjambiLibraryName(null, "QtJambi", true, configuration, replacements, QtJambi_LibraryUtilities.qtMajorVersion, QtJambi_LibraryUtilities.qtMinorVersion, QtJambi_LibraryUtilities.qtJambiPatch);
			    		}
		    		}
			    	loop1: for (String path : libraryPaths) {
			            Iterator<String> iter = replacements.iterator();
			            do {
			            	String lib = null;
			            	String libNoFw = null;
			            	if(!iter.hasNext()) {
			            		lib = libFormat;
			            		libNoFw = libFormatNoFw;
			            	}else {
			            		if(libFormat!=null)
			            			lib = String.format(libFormat, iter.next());
			            		if(libFormatNoFw!=null)
			            			libNoFw = String.format(libFormatNoFw, iter.next());
			            	}
			            	if(lib!=null) {
					        	File f = new File(path, lib);
					        	if (f.exists()) {
					        		loadQtJambiFromLibraryPath = true;
					        		jambiLibraryDir = f.getParentFile();
					        		if(operatingSystem==OperatingSystem.MacOS) {
					        			jambiLibraryDir = jambiLibraryDir.getParentFile().getParentFile();
					        		}
					        		dontUseQtJambiFrameworks = false;
					        		break loop1;
					        	}
			            	}
			            	if(libNoFw!=null) {
					        	File f = new File(path, libNoFw);
					        	if (f.exists()) {
					        		loadQtJambiFromLibraryPath = true;
					        		jambiLibraryDir = f.getParentFile();
					        		dontUseQtJambiFrameworks = true;
					        		break loop1;
					        	}
			            	}
			        	}while(iter.hasNext());
			    	}
		    	}else {
		    		replacements.clear();
		    		String libFormat = qtjambiLibraryName(null, "QtJambi", false, LibraryBundle.Configuration.Release, replacements, QtJambi_LibraryUtilities.qtMajorVersion, QtJambi_LibraryUtilities.qtMinorVersion, QtJambi_LibraryUtilities.qtJambiPatch);
		    		String libFormatNoFw = null;
		    		if(operatingSystem==OperatingSystem.MacOS) {
			    		if(dontUseQtJambiFrameworks==null) {
			    			libFormatNoFw = qtjambiLibraryName(null, "QtJambi", true, LibraryBundle.Configuration.Release, replacements, QtJambi_LibraryUtilities.qtMajorVersion, QtJambi_LibraryUtilities.qtMinorVersion, QtJambi_LibraryUtilities.qtJambiPatch);
			    		}else if(dontUseQtJambiFrameworks) {
			    			libFormat = null;
			    			libFormatNoFw = qtjambiLibraryName(null, "QtJambi", true, LibraryBundle.Configuration.Release, replacements, QtJambi_LibraryUtilities.qtMajorVersion, QtJambi_LibraryUtilities.qtMinorVersion, QtJambi_LibraryUtilities.qtJambiPatch);
			    		}
		    		}
			    	loop1: for (String path : libraryPaths) {
			            Iterator<String> iter = replacements.iterator();
			            do {
			            	String lib = null;
			            	String libNoFw = null;
			            	if(!iter.hasNext()) {
			            		lib = libFormat;
			            		libNoFw = libFormatNoFw;
			            	}else {
			            		if(libFormat!=null)
			            			lib = String.format(libFormat, iter.next());
			            		if(libFormatNoFw!=null)
			            			libNoFw = String.format(libFormatNoFw, iter.next());
			            	}
			            	if(lib!=null) {
					        	File f = new File(path, lib);
					        	if (f.exists()) {
					        		loadQtJambiFromLibraryPath = true;
					        		jambiLibraryDir = f.getParentFile();
					        		if(operatingSystem==OperatingSystem.MacOS) {
					        			jambiLibraryDir = jambiLibraryDir.getParentFile().getParentFile();
					        		}
					        		configuration = LibraryBundle.Configuration.Release;
					        		dontUseQtJambiFrameworks = false;
					        		break loop1;
					        	}
			            	}else if(libNoFw!=null) {
					        	File f = new File(path, libNoFw);
					        	if (f.exists()) {
					        		loadQtJambiFromLibraryPath = true;
					        		jambiLibraryDir = f.getParentFile();
					        		configuration = LibraryBundle.Configuration.Release;
					        		dontUseQtJambiFrameworks = true;
					        		break loop1;
					        	}
			            	}
			    		}while(iter.hasNext());
			    	}
		    		if(!loadQtJambiFromLibraryPath) {
		    			replacements.clear();
			    		libFormat = qtjambiLibraryName(null, "QtJambi", false, LibraryBundle.Configuration.Debug, replacements, QtJambi_LibraryUtilities.qtMajorVersion, QtJambi_LibraryUtilities.qtMinorVersion, QtJambi_LibraryUtilities.qtJambiPatch);
			    		libFormatNoFw = null;
			    		if(operatingSystem==OperatingSystem.MacOS) {
				    		if(dontUseQtJambiFrameworks==null) {
				    			libFormatNoFw = qtjambiLibraryName(null, "QtJambi", true, LibraryBundle.Configuration.Debug, replacements, QtJambi_LibraryUtilities.qtMajorVersion, QtJambi_LibraryUtilities.qtMinorVersion, QtJambi_LibraryUtilities.qtJambiPatch);
				    		}else if(dontUseQtJambiFrameworks) {
				    			libFormat = null;
				    			libFormatNoFw = qtjambiLibraryName(null, "QtJambi", true, LibraryBundle.Configuration.Debug, replacements, QtJambi_LibraryUtilities.qtMajorVersion, QtJambi_LibraryUtilities.qtMinorVersion, QtJambi_LibraryUtilities.qtJambiPatch);
				    		}
			    		}
				    	loop1: for (String path : libraryPaths) {
				            Iterator<String> iter = replacements.iterator();
				            do {
				            	String lib = null;
				            	String libNoFw = null;
				            	if(!iter.hasNext()) {
				            		lib = libFormat;
				            		libNoFw = libFormatNoFw;
				            	}else {
				            		if(libFormat!=null)
				            			lib = String.format(libFormat, iter.next());
				            		if(libFormatNoFw!=null)
				            			libNoFw = String.format(libFormatNoFw, iter.next());
				            	}
				            	if(lib!=null) {
						        	File f = new File(path, lib);
						        	if (f.exists()) {
						        		loadQtJambiFromLibraryPath = true;
						        		jambiLibraryDir = f.getParentFile();
						        		if(operatingSystem==OperatingSystem.MacOS) {
						        			jambiLibraryDir = jambiLibraryDir.getParentFile().getParentFile();
						        		}
						        		configuration = LibraryBundle.Configuration.Debug;
						        		dontUseQtJambiFrameworks = false;
						        		break loop1;
						        	}
				            	}
				            	if(libNoFw!=null) {
						        	File f = new File(path, libNoFw);
						        	if (f.exists()) {
						        		loadQtJambiFromLibraryPath = true;
						        		jambiLibraryDir = f.getParentFile();
						        		configuration = LibraryBundle.Configuration.Debug;
						        		dontUseQtJambiFrameworks = true;
						        		break loop1;
						        	}
				            	}
				    		}while(iter.hasNext());
				    	}
		    		}
		    		if(configuration==null)
		    			configuration = LibraryBundle.Configuration.Release;
		    	}
		    	replacements.clear();
	    		String libFormat = qtLibraryName("Qt", "Core", null, null, configuration, replacements, QtJambi_LibraryUtilities.qtMajorVersion, QtJambi_LibraryUtilities.qtMinorVersion, -1);
		    	loop1: for (String path : libraryPaths) {
		            Iterator<String> iter = replacements.iterator();
		            do {
		            	String lib;
		            	if(!iter.hasNext()) {
		            		lib = libFormat;
		            	}else {
		            		lib = String.format(libFormat, iter.next());
		            	}
		    			File f = new File(path, lib);
		    			if(operatingSystem.isUnixLike()) {
		    				// libQt5Core.so.5 could point to libQt5Core.so.5.x with x < qtMinorVersion
		    				if(f.exists() && (
		    						lib.endsWith(".so."+QtJambi_LibraryUtilities.qtMajorVersion)
		    						|| lib.endsWith(".so")
		    						) && Files.isSymbolicLink(f.toPath())) {
		    					try {
									File link = f.toPath().toRealPath().toFile();
									if(lib.endsWith(".so")) {
										if(!link.getName().startsWith(lib+"."+QtJambi_LibraryUtilities.qtMajorVersion+"."+QtJambi_LibraryUtilities.qtMinorVersion)) {
											f = null;
											continue;
										}
									}else {
										if(!link.getName().startsWith(lib+"."+QtJambi_LibraryUtilities.qtMinorVersion)) {
											f = null;
											continue;
										}
									}
								} catch (IOException e) {
								}
		    				}
    					}
			        	if (f!=null && f.exists()) {
			        		loadQtFromLibraryPath = true;
			        		if(!loadQtJambiFromLibraryPath && !isOSNameSpecified && _osArchName.equals("windows-x64")) {
			        			// check mingw
			        			File util = new File(path, "libstdc++-6.dll");
			        			if(util.exists()) {
			        				isMinGWBuilt = Boolean.TRUE;
				    				_osArchName = "windows-mingw-x64";
			        			}else {
			        				util = new File(path, "libc++.dll");
				        			if(util.exists()) {
				        				isMinGWBuilt = Boolean.TRUE;
					    				_osArchName = "windows-llvm-mingw-x64";
				        			}else {
				        				
				        			}
			        			}
			        		}
			        		break loop1;
			        	}
		    		}while(iter.hasNext());
		    	}
	        }
	        
	        if(!loadQtJambiFromLibraryPath || !loadQtFromLibraryPath){
		        File tmpDir = new File(System.getProperty("java.io.tmpdir"));
		        String deploymentdir = System.getProperty("io.qt.deploymentdir", "");
		        boolean keepDeployment = Boolean.getBoolean("io.qt.keep-temp-deployment");
		        String dirprefix = "v";
		        if(deploymentdir!=null 
		        		&& !deploymentdir.isEmpty()
		        		&& !"tmp".equalsIgnoreCase(deploymentdir)
		        		&& !"temp".equalsIgnoreCase(deploymentdir)) {
	        		keepDeployment = true;
		        	if("user".equalsIgnoreCase(deploymentdir)) {
		        		switch (operatingSystem) {
		                case Windows:
		                	tmpDir = new File(System.getProperty("user.home"), "AppData\\Local\\QtJambi");
		                	break;
						case MacOS:
							tmpDir = new File(System.getProperty("user.home"), "Library/Application Support/QtJambi");
							break;
						default:
							if(operatingSystem.isUnixLike())
								tmpDir = new File(System.getProperty("user.home"), ".local/share/QtJambi");
							break;
		        		}
		        	}else if("common".equalsIgnoreCase(deploymentdir)) {
		        		switch (operatingSystem) {
		                case Windows:
		                	tmpDir = new File("C:\\ProgramData\\QtJambi");
		                	break;
						case MacOS:
							tmpDir = new File("/Library/Application Support/QtJambi");
							break;
						default:
							if(operatingSystem.isUnixLike())
								tmpDir = new File("/usr/local/share/QtJambi");
							break;
		        		}
		        	}else {
			        	File deploymentDir = new File(deploymentdir);
			        	if(!deploymentDir.isAbsolute()) {
			        		deploymentDir = new File(System.getProperty("user.home"), deploymentdir);
			        	}
		        		if(!deploymentDir.getName().toLowerCase().startsWith("qtjambi"))
	        				dirprefix = "QtJambi";
		        		tmpDir = deploymentDir;
		        	}
	        		jambiDeploymentDir = new File(tmpDir, dirprefix + QtJambi_LibraryUtilities.qtMajorVersion + "." + QtJambi_LibraryUtilities.qtMinorVersion + "." + QtJambi_LibraryUtilities.qtJambiPatch);
		        }else {
			        if(keepDeployment) {
			        	switch (operatingSystem) {
						default:
							if(operatingSystem.isUnixLike()) {
								if(!tmpDir.getAbsolutePath().startsWith(System.getProperty("user.home"))) {
									jambiDeploymentDir = new File(tmpDir, "QtJambi" + QtJambi_LibraryUtilities.qtMajorVersion + "." + QtJambi_LibraryUtilities.qtMinorVersion + "." + QtJambi_LibraryUtilities.qtJambiPatch + "_" + System.getProperty("user.name"));
									break;
								}
							}
				        	jambiDeploymentDir = new File(tmpDir, "QtJambi" + QtJambi_LibraryUtilities.qtMajorVersion + "." + QtJambi_LibraryUtilities.qtMinorVersion + "." + QtJambi_LibraryUtilities.qtJambiPatch);
							break;
			        	}
			        }else {
			        	if(RetroHelper.processName()!=null && !RetroHelper.processName().isEmpty()){
				        	jambiDeploymentDir = new File(tmpDir, "QtJambi" + QtJambi_LibraryUtilities.qtMajorVersion + "." + QtJambi_LibraryUtilities.qtMinorVersion + "." + QtJambi_LibraryUtilities.qtJambiPatch + "_" + RetroHelper.processName());
				        	try{
					        	Preferences preferences = Preferences.userNodeForPackage(LibraryUtility.class);
					        	Preferences pids = preferences.node("qtjambi.pids");
					        	pids.put(RetroHelper.processName(), jambiDeploymentDir.getAbsolutePath());
					        	pids.sync();
					        }catch(Throwable t) {}
				        }else {
				        	jambiDeploymentDir = new File(tmpDir, "QtJambi" + QtJambi_LibraryUtilities.qtMajorVersion + "." + QtJambi_LibraryUtilities.qtMinorVersion + "." + QtJambi_LibraryUtilities.qtJambiPatch + "_" + System.getProperty("user.name"));
				        }
			        }
		        }
		        deleteTmpDeployment = !keepDeployment;
		        if(deleteTmpDeployment)
		        	logger.log(Level.FINEST, ()->"Requires deletion of tmp deployment directory "+jambiDeploymentDir.getAbsolutePath()+" during program termination.");
		        
		        if(Boolean.getBoolean("io.qt.provide-headers")) {
		        	tmpDir = new File(System.getProperty("java.io.tmpdir"));
			        String headersdir = System.getProperty("io.qt.headersdir", "");
			        dirprefix = "v";
			        if(headersdir!=null 
			        		&& !headersdir.isEmpty()
			        		&& !"tmp".equalsIgnoreCase(headersdir)
			        		&& !"temp".equalsIgnoreCase(headersdir)) {
			        	if("user".equalsIgnoreCase(headersdir)) {
			        		switch (operatingSystem) {
			                case Windows:
			                	tmpDir = new File(System.getProperty("user.home"), "AppData\\Local\\QtJambi");
			                	break;
							case MacOS:
								tmpDir = new File(System.getProperty("user.home"), "Library/Application Support/QtJambi");
								break;
							default:
								if(operatingSystem.isUnixLike())
									tmpDir = new File(System.getProperty("user.home"), ".local/share/QtJambi");
								break;
			        		}
			        	}else if("common".equalsIgnoreCase(headersdir)) {
			        		switch (operatingSystem) {
			                case Windows:
			                	tmpDir = new File("C:\\ProgramData\\QtJambi");
			                	break;
							case MacOS:
								tmpDir = new File("/Library/Application Support/QtJambi");
								break;
							default:
								if(operatingSystem.isUnixLike())
									tmpDir = new File("/usr/local/share/QtJambi");
								break;
			        		}
			        	}else {
				        	File deploymentDir = new File(headersdir);
				        	if(!deploymentDir.isAbsolute()) {
				        		deploymentDir = new File(System.getProperty("user.home"), headersdir);
				        	}
			        		if(!deploymentDir.getName().toLowerCase().startsWith("qtjambi"))
		        				dirprefix = "QtJambi";
			        		tmpDir = deploymentDir;
			        	}
		        		jambiHeadersDir = new File(tmpDir, dirprefix + QtJambi_LibraryUtilities.qtMajorVersion + "." + QtJambi_LibraryUtilities.qtMinorVersion + "." + QtJambi_LibraryUtilities.qtJambiPatch);
			        }else if("tmp".equalsIgnoreCase(headersdir)
			        		|| "temp".equalsIgnoreCase(headersdir)){
				        if(deleteTmpDeployment) {
				        	switch (operatingSystem) {
							default:
								if(operatingSystem.isUnixLike()) {
									if(!tmpDir.getAbsolutePath().startsWith(System.getProperty("user.home"))) {
										jambiHeadersDir = new File(tmpDir, "QtJambi" + QtJambi_LibraryUtilities.qtMajorVersion + "." + QtJambi_LibraryUtilities.qtMinorVersion + "." + QtJambi_LibraryUtilities.qtJambiPatch + "_" + System.getProperty("user.name"));
										break;
									}
								}
								jambiHeadersDir = new File(tmpDir, "QtJambi" + QtJambi_LibraryUtilities.qtMajorVersion + "." + QtJambi_LibraryUtilities.qtMinorVersion + "." + QtJambi_LibraryUtilities.qtJambiPatch);
								break;
				        	}
				        }else {
				        	if(RetroHelper.processName()!=null && !RetroHelper.processName().isEmpty()){
				        		jambiHeadersDir = new File(tmpDir, "QtJambi" + QtJambi_LibraryUtilities.qtMajorVersion + "." + QtJambi_LibraryUtilities.qtMinorVersion + "." + QtJambi_LibraryUtilities.qtJambiPatch + "_" + RetroHelper.processName());
					        	try{
						        	Preferences preferences = Preferences.userNodeForPackage(LibraryUtility.class);
						        	Preferences pids = preferences.node("qtjambi.pids");
						        	pids.put(RetroHelper.processName(), jambiDeploymentDir.getAbsolutePath());
						        	pids.sync();
						        }catch(Throwable t) {}
					        }else {
					        	jambiHeadersDir = new File(tmpDir, "QtJambi" + QtJambi_LibraryUtilities.qtMajorVersion + "." + QtJambi_LibraryUtilities.qtMinorVersion + "." + QtJambi_LibraryUtilities.qtJambiPatch + "_" + System.getProperty("user.name"));
					        }
				        }
			        }else {
			        	jambiHeadersDir = null;
			        }
		        }else {
		        	jambiHeadersDir = null;
		        }
	        	if(debugInfoDeployment) {
	        		tmpDir = new File(System.getProperty("java.io.tmpdir"));
			        String sourcesdir = System.getProperty("io.qt.sourcesdir", "");
			        dirprefix = "v";
			        if(sourcesdir!=null 
			        		&& !sourcesdir.isEmpty()
			        		&& !"tmp".equalsIgnoreCase(sourcesdir)
			        		&& !"temp".equalsIgnoreCase(sourcesdir)) {
			        	if("user".equalsIgnoreCase(sourcesdir)) {
			        		switch (operatingSystem) {
			                case Windows:
			                	tmpDir = new File(System.getProperty("user.home"), "AppData\\Local\\QtJambi");
			                	break;
							case MacOS:
								tmpDir = new File(System.getProperty("user.home"), "Library/Application Support/QtJambi");
								break;
							default:
								if(operatingSystem.isUnixLike())
									tmpDir = new File(System.getProperty("user.home"), ".local/share/QtJambi");
								break;
			        		}
			        	}else if("common".equalsIgnoreCase(sourcesdir)) {
			        		switch (operatingSystem) {
			                case Windows:
			                	tmpDir = new File("C:\\ProgramData\\QtJambi");
			                	break;
							case MacOS:
								tmpDir = new File("/Library/Application Support/QtJambi");
								break;
							default:
								if(operatingSystem.isUnixLike())
									tmpDir = new File("/usr/local/share/QtJambi");
								break;
			        		}
			        	}else {
				        	File deploymentDir = new File(sourcesdir);
				        	if(!deploymentDir.isAbsolute()) {
				        		deploymentDir = new File(System.getProperty("user.home"), sourcesdir);
				        	}
			        		if(!deploymentDir.getName().toLowerCase().startsWith("qtjambi"))
		        				dirprefix = "QtJambi";
			        		tmpDir = deploymentDir;
			        	}
		        		jambiSourcesDir = new File(tmpDir, dirprefix + QtJambi_LibraryUtilities.qtMajorVersion + "." + QtJambi_LibraryUtilities.qtMinorVersion + "." + QtJambi_LibraryUtilities.qtJambiPatch);
			        }else if("tmp".equalsIgnoreCase(sourcesdir)
			        		|| "temp".equalsIgnoreCase(sourcesdir)){
				        if(deleteTmpDeployment) {
				        	switch (operatingSystem) {
							default:
								if(operatingSystem.isUnixLike()) {
									if(!tmpDir.getAbsolutePath().startsWith(System.getProperty("user.home"))) {
										jambiSourcesDir = new File(tmpDir, "QtJambi" + QtJambi_LibraryUtilities.qtMajorVersion + "." + QtJambi_LibraryUtilities.qtMinorVersion + "." + QtJambi_LibraryUtilities.qtJambiPatch + "_" + System.getProperty("user.name"));
										break;
									}
								}
								jambiSourcesDir = new File(tmpDir, "QtJambi" + QtJambi_LibraryUtilities.qtMajorVersion + "." + QtJambi_LibraryUtilities.qtMinorVersion + "." + QtJambi_LibraryUtilities.qtJambiPatch);
								break;
				        	}
				        }else {
				        	if(RetroHelper.processName()!=null && !RetroHelper.processName().isEmpty()){
				        		jambiSourcesDir = new File(tmpDir, "QtJambi" + QtJambi_LibraryUtilities.qtMajorVersion + "." + QtJambi_LibraryUtilities.qtMinorVersion + "." + QtJambi_LibraryUtilities.qtJambiPatch + "_" + RetroHelper.processName());
					        	try{
						        	Preferences preferences = Preferences.userNodeForPackage(LibraryUtility.class);
						        	Preferences pids = preferences.node("qtjambi.pids");
						        	pids.put(RetroHelper.processName(), jambiDeploymentDir.getAbsolutePath());
						        	pids.sync();
						        }catch(Throwable t) {}
					        }else {
					        	jambiSourcesDir = new File(tmpDir, "QtJambi" + QtJambi_LibraryUtilities.qtMajorVersion + "." + QtJambi_LibraryUtilities.qtMinorVersion + "." + QtJambi_LibraryUtilities.qtJambiPatch + "_" + System.getProperty("user.name"));
					        }
				        }
			        }else {
			        	jambiSourcesDir = null;
			        }
		        }else {
		        	jambiSourcesDir = null;
	        	}
		    	
	    		boolean maybeMavenRepo = false;
	    		boolean maybeGradleCache = false;
		    	{
		    		File _jambiJarDir = null;
		        	URL _classURL = LibraryUtility.class.getResource(LibraryUtility.class.getSimpleName()+".class");
		        	if(_classURL!=null) {
			        	int index;
			        	String classURL = _classURL.toString();
			    		File jarFile = null;
				    	if(classURL.startsWith("jar:file:") && (index = classURL.indexOf("!/"))>0) {
				    		String jarFileURL = classURL.substring(4, index);
				    		try {
								jarFile = new File(CoreUtility.createURL(jarFileURL).toURI());
							} catch (Exception e) {
								try{
									jarFileURL = jarFileURL.substring(5);
									jarFile = new File(jarFileURL);
								} catch (Throwable e2) {
								}
							}
				    	}
				    	if(jarFile==null || !jarFile.exists()) {
			    			try {
								URLConnection connection = _classURL.openConnection();
								if(connection instanceof JarURLConnection) {
									jarFile = new File(((JarURLConnection) connection).getJarFile().getName());
								}
							} catch (Throwable e) {
							}
			    		}
				    	if(jarFile!=null && jarFile.exists()) {
			    			_jambiJarDir = jarFile.getParentFile();
			    			try {
				    			if(_jambiJarDir.getName().equals(QtJambi_LibraryUtilities.qtMajorVersion + "." + QtJambi_LibraryUtilities.qtMinorVersion + "." + QtJambi_LibraryUtilities.qtJambiPatch)
				    					&& _jambiJarDir.getParentFile().getName().equals("qtjambi")) {
				    				maybeMavenRepo = true;
				    			}
			    			} catch (Throwable e) {
							}
			    			if(!maybeMavenRepo) {
			    				try {
					    			if(_jambiJarDir.getParentFile().getName().equals(QtJambi_LibraryUtilities.qtMajorVersion + "." + QtJambi_LibraryUtilities.qtMinorVersion + "." + QtJambi_LibraryUtilities.qtJambiPatch)
					    					&& _jambiJarDir.getParentFile().getParentFile().getName().equals("qtjambi")) {
					    				maybeGradleCache = true;
					    			}
				    			} catch (Throwable e) {
								}
			    			}
		    			}
		        	}
			    	jambiJarDir = _jambiJarDir;
		    	}
		        
	
	    		ClassLoader loader = classLoader();
	            List<URL> specsFound = new ArrayList<>();
	            try {
					Enumeration<URL> enm = loader.getResources(DEPLOY_XML);
					while(enm.hasMoreElements()) {
						specsFound.add(enm.nextElement());
					}
				} catch (IOException e) {
					logger.log(Level.WARNING, "", e);
				}
	            if(!isOSNameSpecified) {
		            switch (operatingSystem) {
	                case Windows:
	                	DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
	                    factory.setValidating(false);
	                    DocumentBuilder builder = factory.newDocumentBuilder();
			            for(URL _url : specsFound) {
			            	try(InputStream inStream = _url.openStream()){
			            		Document doc = builder.parse(inStream);
			                    String system = doc.getDocumentElement().getAttribute("system");
			                    if("qtjambi".equals(doc.getDocumentElement().getAttribute("module"))
			                    		&& system!=null
			                    		&& system.startsWith("windows-")) {
			                    	_osArchName = system;
			                    	break;
			                    }
			            	}catch(Exception e) {}
			            }
			            break;
			            default:
		            }
	            }
	            dontSearchDeploymentSpec = !specsFound.isEmpty();
            	Map<URL,URL> debuginfosByURL = Collections.emptyMap();
	            if(dontSearchDeploymentSpec && !"debug".equals(System.getProperty("io.qt.debug")) && debugInfoDeployment) {
	            	debuginfosByURL = new HashMap<>();
	            	List<URL> foundURLs = new ArrayList<>();
	            	Map<String,QPair<URL,URL>> urlsByFileName = new TreeMap<>();
		            for(URL _url : specsFound) {
		            	String url = _url.toString();
		            	if(url.startsWith("jar:file:") && url.endsWith(DEPLOY_XML_IN_JAR)) {
		            		url = url.substring(4, url.length()-DEPLOY_XML_IN_JAR.length());
		            		File jar = new File(url);
		            		Function<String,QPair<URL,URL>> pairFactory = n->new QPair<>(null, null);
		            		if(jar.getName().contains("-native-")) {
		            			QPair<URL,URL> pair = urlsByFileName.computeIfAbsent(jar.getName(), pairFactory);
		            			pair.first = _url;
		            			foundURLs.add(_url);
		            		}else if(jar.getName().contains("-debuginfo-")) {
		            			QPair<URL,URL> pair = urlsByFileName.computeIfAbsent(jar.getName().replace("-debuginfo-", "-native-"), pairFactory);
		            			pair.second = _url;
		            		}
		            	}
		            }
		            for(QPair<URL,URL> pair : urlsByFileName.values()) {
		            	if(pair.first!=null) {
		            		if(pair.second!=null) {
		            			debuginfosByURL.put(pair.first, pair.second);
		            		}else {
		            			String url = pair.first.toString();
		            			url = url.substring(4, url.length()-DEPLOY_XML_IN_JAR.length());
			            		File jar = new File(url);
		            			File debuginfo = new File(jar.getParentFile(), jar.getName().replace("-native-", "-debuginfo-"));
			            		if(!debuginfo.exists()) {
			            			debuginfo = new File(jar.getParent().replace("-native-", "-debuginfo-"), jar.getName().replace("-native-", "-debuginfo-"));
			            		}
			            		if(!debuginfo.exists()) {
			            			debuginfo = new File(jar.getParent().replace(File.separator+"native"+File.separator, File.separator+"debuginfo"+File.separator), jar.getName().replace("-native-", "-debuginfo-"));
			            		}
			            		if(debuginfo.exists()) {
			            			debuginfosByURL.put(pair.first, CoreUtility.createURL("jar:"+debuginfo.toURI()+DEPLOY_XML_IN_JAR));
			            		}
		            		}
		            	}
		            }
		            specsFound = foundURLs;
	            }
	            
	            if(!dontSearchDeploymentSpec && jambiJarDir!=null) {
		            boolean _isNativeSubdir = false;
		            boolean _isDebuginfoSubdir = false;
		            boolean _isMavenRepo = false;
		            boolean _isGradleCache = false;
	            	List<URL> foundURLs = new ArrayList<>();
	            	boolean isDebug = false;
	            	debuginfosByURL = new HashMap<>();
	            	if(!loadQtJambiFromLibraryPath) {
	            		String version = String.format("%1$s.%2$s.%3$s", QtJambi_LibraryUtilities.qtMajorVersion, QtJambi_LibraryUtilities.qtMinorVersion, QtJambi_LibraryUtilities.qtJambiPatch);
	            		String nativeModule = String.format("qtjambi-native-%1$s", _osArchName);
	            		String debuginfoModule = String.format("qtjambi-debuginfo-%1$s", _osArchName);
		            	if("debug".equals(System.getProperty("io.qt.debug"))) {
			    			File nativeFile = null;
			    			if(maybeMavenRepo) {
			    				try {
				    				File jarDir = new File(new File(jambiJarDir.getParentFile().getParentFile(), String.format("%1$s-debug", nativeModule)), version);
				    				nativeFile = new File(jarDir, String.format("%1$s-debug-%2$s.jar", nativeModule, version));
				    				_isMavenRepo = nativeFile.exists();
			    				} catch (Throwable e) {}
			    			}
			    			if(maybeGradleCache) {
			    				try {
				    				File jarDir = new File(new File(jambiJarDir.getParentFile().getParentFile().getParentFile(), String.format("%1$s-debug", nativeModule)), version);
				    				for(File subdir : jarDir.listFiles()) {
		        						if(subdir.isDirectory()) {
		        							nativeFile = new File(subdir, String.format("%1$s-debug-%2$s.jar", nativeModule, version));
		        							if(nativeFile.exists())
		        								break;
		        						}
				    				}
				    				_isGradleCache = nativeFile.exists();
			    				} catch (Throwable e) {}
			    			}
			    			if(nativeFile==null || !nativeFile.exists()) {
			    				nativeFile = new File(jambiJarDir, String.format("%1$s-debug-%2$s.jar", nativeModule, version));
			    				if(!nativeFile.exists()) {
				    				nativeFile = new File(new File(jambiJarDir, "native"), String.format("%1$s-debug-%2$s.jar", nativeModule, version));
				    				_isNativeSubdir = nativeFile.exists();
				    			}
			    			}
			    			if(!nativeFile.exists() && "windows-x64".equals(_osArchName)) {
			    				for(String compiler : new String[]{"windows-llvm-mingw-x64", "windows-mingw-x64"}) {
				    				nativeModule = String.format("qtjambi-native-%1$s", compiler);
				            		debuginfoModule = String.format("qtjambi-debuginfo-%1$s", compiler);
				            		nativeFile = null;
				            		if(maybeMavenRepo) {
					    				try {
						    				File jarDir = new File(new File(jambiJarDir.getParentFile().getParentFile(), String.format("%1$s-debug", nativeModule)), version);
						    				nativeFile = new File(jarDir, String.format("%1$s-debug-%2$s.jar", nativeModule, version));
						    				_isMavenRepo = nativeFile.exists();
					    				} catch (Throwable e) {}
					    			}
					    			if(maybeGradleCache) {
					    				try {
						    				File jarDir = new File(new File(jambiJarDir.getParentFile().getParentFile().getParentFile(), String.format("%1$s-debug", nativeModule)), version);
						    				for(File subdir : jarDir.listFiles()) {
				        						if(subdir.isDirectory()) {
				        							nativeFile = new File(subdir, String.format("%1$s-debug-%2$s.jar", nativeModule, version));
				        							if(nativeFile.exists())
				        								break;
				        						}
						    				}
						    				_isGradleCache = nativeFile.exists();
					    				} catch (Throwable e) {}
					    			}
					    			if(nativeFile==null || !nativeFile.exists()) {
					    				nativeFile = new File(jambiJarDir, String.format("%1$s-debug-%2$s.jar", nativeModule, version));
					    				if(!nativeFile.exists()) {
						    				nativeFile = new File(new File(jambiJarDir, "native"), String.format("%1$s-debug-%2$s.jar", nativeModule, version));
						    				_isNativeSubdir = nativeFile.exists();
						    			}
					    			}
					    			if(nativeFile.exists()) {
					    				isMinGWBuilt = Boolean.TRUE;
					    				_osArchName = compiler;
					    				break;
					    			}
			    				}
		            		}
			    			if(nativeFile.exists()) {
			    				isDebug = true;
			    				try {
			    					foundURLs.add(CoreUtility.createURL("jar:"+nativeFile.toURI()+DEPLOY_XML_IN_JAR));
								} catch (IOException e) {
								}
			    			}
			    			osArchName = _osArchName;
		            	}else {
		            		File nativeFile = null;
		            		if(maybeMavenRepo) {
			    				try {
				    				File jarDir = new File(new File(jambiJarDir.getParentFile().getParentFile(), nativeModule), version);
				    				nativeFile = new File(jarDir, String.format("%1$s-%2$s.jar", nativeModule, version));
				    				_isMavenRepo = nativeFile.exists();
			    				} catch (Throwable e) {}
			    			}
		            		if(maybeGradleCache) {
			    				try {
				    				File jarDir = new File(new File(jambiJarDir.getParentFile().getParentFile().getParentFile(), nativeModule), version);
				    				for(File subdir : jarDir.listFiles()) {
		        						if(subdir.isDirectory()) {
		        							nativeFile = new File(subdir, String.format("%1$s-%2$s.jar", nativeModule, version));
		        							if(nativeFile.exists())
		        								break;
		        						}
				    				}
				    				_isGradleCache = nativeFile.exists();
			    				} catch (Throwable e) {}
			    			}
		            		if(nativeFile==null || !nativeFile.exists()) {
		            			nativeFile = new File(jambiJarDir, String.format("%1$s-%2$s.jar", nativeModule, version));
		            			if(!nativeFile.exists()) {
				    				nativeFile = new File(new File(jambiJarDir, "native"), String.format("%1$s-%2$s.jar", nativeModule, version));
				    				_isNativeSubdir = nativeFile.exists();
				    			}
		            		}
		            		if(!nativeFile.exists() && "windows-x64".equals(_osArchName)) {
			    				for(String compiler : new String[]{"windows-llvm-mingw-x64", "windows-mingw-x64"}) {
				    				nativeModule = String.format("qtjambi-native-%1$s", compiler);
				            		debuginfoModule = String.format("qtjambi-debuginfo-%1$s", compiler);
				            		nativeFile = null;
				            		if(maybeMavenRepo) {
					    				try {
						    				File jarDir = new File(new File(jambiJarDir.getParentFile().getParentFile(), nativeModule), version);
						    				nativeFile = new File(jarDir, String.format("%1$s-%2$s.jar", nativeModule, version));
						    				_isMavenRepo = nativeFile.exists();
					    				} catch (Throwable e) {}
					    			}
				            		if(maybeGradleCache) {
					    				try {
						    				File jarDir = new File(new File(jambiJarDir.getParentFile().getParentFile().getParentFile(), nativeModule), version);
						    				for(File subdir : jarDir.listFiles()) {
				        						if(subdir.isDirectory()) {
				        							nativeFile = new File(subdir, String.format("%1$s-%2$s.jar", nativeModule, version));
				        							if(nativeFile.exists())
				        								break;
				        						}
						    				}
						    				_isGradleCache = nativeFile.exists();
					    				} catch (Throwable e) {}
					    			}
				            		if(nativeFile==null || !nativeFile.exists()) {
				            			nativeFile = new File(jambiJarDir, String.format("%1$s-%2$s.jar", nativeModule, version));
				            			if(!nativeFile.exists()) {
						    				nativeFile = new File(new File(jambiJarDir, "native"), String.format("%1$s-%2$s.jar", nativeModule, version));
						    				_isNativeSubdir = nativeFile.exists();
						    			}
				            		}
					    			if(nativeFile.exists()) {
					    				_osArchName = compiler;
					    				isMinGWBuilt = Boolean.TRUE;
					    				break;
					    			}
			    				}
		            		}
		            		osArchName = _osArchName;
			    			if(nativeFile.exists()) {
			    				try {
			    					URL url = CoreUtility.createURL("jar:"+nativeFile.toURI()+DEPLOY_XML_IN_JAR);
			    					foundURLs.add(url);
			    					if(debugInfoDeployment) {
					            		File debuginfoFile = null;
					            		if(_isMavenRepo) {
						    				try {
							    				File jarDir = new File(new File(jambiJarDir.getParentFile().getParentFile(), debuginfoModule), version);
							    				debuginfoFile = new File(jarDir, String.format("%1$s-%2$s.jar", debuginfoModule, version));
						    				} catch (Throwable e) {}
						    			} else if(_isGradleCache) {
						    				try {
							    				File jarDir = new File(new File(jambiJarDir.getParentFile().getParentFile().getParentFile(), debuginfoModule), version);
							    				for(File subdir : jarDir.listFiles()) {
					        						if(subdir.isDirectory()) {
					        							debuginfoFile = new File(subdir, String.format("%1$s-%2$s.jar", debuginfoModule, version));
					        							if(debuginfoFile.exists())
					        								break;
					        						}
							    				}
						    				} catch (Throwable e) {}
						    			}
					            		if(debuginfoFile==null || !debuginfoFile.exists()) {
					            			debuginfoFile = new File(jambiJarDir, String.format("%1$s-%2$s.jar", debuginfoModule, version));
						            		if(!debuginfoFile.exists()) {
						            			debuginfoFile = new File(new File(jambiJarDir, "debuginfo"), String.format("%1$s-%2$s.jar", debuginfoModule, version));
							    				_isDebuginfoSubdir = debuginfoFile.exists();
						            		}
					            		}
					            		if(debuginfoFile.exists()) {
					            			try {
					            				debuginfosByURL.put(url, CoreUtility.createURL("jar:"+debuginfoFile.toURI()+DEPLOY_XML_IN_JAR));
											} catch (IOException e) {
											}
					            		}
				    				}
								} catch (IOException e) {
								}
			    			}else if(!"release".equals(System.getProperty("io.qt.debug"))){
			    				nativeFile = null;
			    				if(maybeMavenRepo) {
			    					try {
					    				File jarDir = new File(new File(jambiJarDir.getParentFile().getParentFile(), String.format("%1$s-debug", nativeModule)), version);
					    				nativeFile = new File(jarDir, String.format("%1$s-debug-%2$s.jar", nativeModule, version));
					    				_isMavenRepo = nativeFile.exists();
				    				} catch (Throwable e) {}
			    				}
			    				if(maybeGradleCache) {
			    					try {
					    				File jarDir = new File(new File(jambiJarDir.getParentFile().getParentFile().getParentFile(), String.format("%1$s-debug", nativeModule)), version);
					    				for(File subdir : jarDir.listFiles()) {
			        						if(subdir.isDirectory()) {
			        							nativeFile = new File(subdir, String.format("%1$s-debug-%2$s.jar", nativeModule, version));
			        							if(nativeFile.exists())
			        								break;
			        						}
					    				}
					    				_isGradleCache = nativeFile.exists();
				    				} catch (Throwable e) {}
			    				}
			    				if(nativeFile==null || !nativeFile.exists()) {
			    					nativeFile = new File(jambiJarDir, String.format("%1$s-debug-%2$s.jar", nativeModule, version));
				    				if(!nativeFile.exists()) {
					    				nativeFile = new File(new File(jambiJarDir, "native"), String.format("%1$s-debug-%2$s.jar", nativeModule, version));
					    				_isNativeSubdir = nativeFile.exists();
					    			}
			    				}
				    			if(nativeFile.exists()) {
				    				isDebug = true;
				    				try {
				    					foundURLs.add(CoreUtility.createURL("jar:"+nativeFile.toURI()+DEPLOY_XML_IN_JAR));
									} catch (IOException e) {
									}
				    			}
			    			}
		            	}
		        		final String suffix;
	        			if(isDebug) {
	        				suffix = String.format("-native-%1$s-debug", _osArchName);
		        		}else {
		        			suffix = String.format("-native-%1$s", _osArchName);
		        		}
		            	final String versionedSuffix = String.format("%1$s-%2$s.%3$s.", suffix, QtJambi_LibraryUtilities.qtMajorVersion, QtJambi_LibraryUtilities.qtMinorVersion);
		        		if(_isMavenRepo) {
		        			try {
			        			for(File dir : jambiJarDir.getParentFile().getParentFile().listFiles()) {
			        				if(dir.isDirectory() && dir.getName().startsWith("qtjambi-plugin-") && dir.getName().endsWith(suffix)) {
			        					dir = new File(dir, version);
			        					for(File f : dir.listFiles()) {
											if(f.getName().startsWith("qtjambi-plugin-") && f.getName().contains(versionedSuffix)) {
												try {
													URL url = CoreUtility.createURL("jar:"+f.toURI()+DEPLOY_XML_IN_JAR);
													foundURLs.add(url);
													if(!isDebug && debugInfoDeployment) {
														String name = f.getName().replace("-native-", "-debuginfo-");
														File debuginfoFile = new File(f.getParentFile(), name);
														if(debuginfoFile.exists()) {
															debuginfosByURL.put(url, CoreUtility.createURL("jar:"+debuginfoFile.toURI()+DEPLOY_XML_IN_JAR));
														}
													}
												} catch (IOException e) {
												}
											}
										}
			        				}
			        			}
		    				} catch (Throwable e) {}
		        		}
		        		if(_isGradleCache) {
		        			try {
			        			for(File dir : jambiJarDir.getParentFile().getParentFile().getParentFile().getParentFile().listFiles()) {
			        				if(dir.isDirectory() && dir.getName().startsWith("qtjambi-plugin-") && dir.getName().endsWith(suffix)) {
			        					dir = new File(dir, version);
			        					for(File subdir : dir.listFiles()) {
			        						if(subdir.isDirectory()) {
					        					for(File f : subdir.listFiles()) {
													if(f.getName().startsWith("qtjambi-plugin-") && f.getName().contains(versionedSuffix)) {
														try {
															URL url = CoreUtility.createURL("jar:"+f.toURI()+DEPLOY_XML_IN_JAR);
															foundURLs.add(url);
															if(!isDebug && debugInfoDeployment) {
																String name = f.getName().replace("-native-", "-debuginfo-");
																File debuginfoFile = new File(f.getParentFile(), name);
																if(debuginfoFile.exists()) {
																	debuginfosByURL.put(url, CoreUtility.createURL("jar:"+debuginfoFile.toURI()+DEPLOY_XML_IN_JAR));
																}
															}
														} catch (IOException e) {
														}
													}
												}
			        						}
			        					}
			        				}
			        			}
		    				} catch (Throwable e) {}
		        		}
		        		try {
			        		for(File f : (_isNativeSubdir ? new File(jambiJarDir, "native") : jambiJarDir).listFiles()) {
								if(f.getName().startsWith("qtjambi-plugin-") && f.getName().contains(versionedSuffix)) {
									try {
										URL url = CoreUtility.createURL("jar:"+f.toURI()+DEPLOY_XML_IN_JAR);
										foundURLs.add(url);
										if(!isDebug && debugInfoDeployment) {
											String name = f.getName().replace("-native-", "-debuginfo-");
											File debuginfoFile = new File((_isNativeSubdir ? new File(jambiJarDir, "debuginfo") : jambiJarDir), name);
											if(debuginfoFile.exists()) {
												debuginfosByURL.put(url, CoreUtility.createURL("jar:"+debuginfoFile.toURI()+DEPLOY_XML_IN_JAR));
											}
										}
									} catch (IOException e) {
									}
								}
							}
	    				} catch (Throwable e) {}
	            	}else {
	            		osArchName = _osArchName;
	            	}
	            	if(!loadQtFromLibraryPath) {
	            		int qtPatchVersion = -1;
	            		String libPrefix = String.format("qt-lib-core-native-%1$s%2$s-%3$s.%4$s.", _osArchName, isDebug && operatingSystem==OperatingSystem.Windows ? "-debug" : "", QtJambi_LibraryUtilities.qtMajorVersion, QtJambi_LibraryUtilities.qtMinorVersion);
	        			for(File lib : (_isNativeSubdir ? new File(jambiJarDir, "native") : jambiJarDir).listFiles()) {
	            			if(lib.isFile() && lib.getName().startsWith(libPrefix) && lib.getName().endsWith(".jar")) {
	            				String version = lib.getName().substring(libPrefix.length(), lib.getName().length()-4);
	            				int v = Integer.parseInt(version);
	            				if(v>qtPatchVersion)
	            					qtPatchVersion = v;
	            			}
	            		}
	            		if(qtPatchVersion>=0) {
	            			String libSuffix = String.format("-native-%1$s%2$s-%3$s.%4$s.%5$s.jar", _osArchName, isDebug && operatingSystem==OperatingSystem.Windows ? "-debug" : "", QtJambi_LibraryUtilities.qtMajorVersion, QtJambi_LibraryUtilities.qtMinorVersion, qtPatchVersion);
	            			for(File lib : (_isNativeSubdir ? new File(jambiJarDir, "native") : jambiJarDir).listFiles()) {
	            				if(lib.isFile() && lib.getName().startsWith("qt-") && lib.getName().endsWith(libSuffix)) {
	            					try {
	            						URL url = CoreUtility.createURL("jar:"+lib.toURI()+DEPLOY_XML_IN_JAR);
				    					foundURLs.add(url);
				    					if(!isDebug && debugInfoDeployment) {
											String name = lib.getName().replace("-native-", "-debuginfo-");
											File debuginfoFile = new File((_isNativeSubdir ? new File(jambiJarDir, "debuginfo") : jambiJarDir), name);
											if(debuginfoFile.exists()) {
												debuginfosByURL.put(url, CoreUtility.createURL("jar:"+debuginfoFile.toURI()+DEPLOY_XML_IN_JAR));
											}
										}
									} catch (IOException e) {
									}
	            				}
	            			}
	            		}
	            	}
	            	if(!foundURLs.isEmpty())
	            		specsFound = foundURLs;
			    	isMavenRepo = _isMavenRepo;
			    	isGradleCache = _isGradleCache;
			    	isNativeSubdir = _isNativeSubdir;
			    	isDebuginfoSubdir = _isDebuginfoSubdir;
	            }else {
	            	isMavenRepo = false;
			    	isGradleCache = false;
	            	isNativeSubdir = false;
	            	isDebuginfoSubdir = false;
	            	osArchName = _osArchName;
	            }
	            
	            for(URL url : specsFound) {
	                if(loadedNativeDeploymentUrls.contains(url))
	                	continue;
	                loadedNativeDeploymentUrls.add(url);
	
	                logger.log(Level.FINEST, ()->String.format("Found %1$s", url.getFile().replace(DEPLOY_XML_IN_JAR, "")));
	
	                LibraryBundle spec = null;
	                String protocol = url.getProtocol();
	                if (protocol.equals("jar")) {
	                    // Using toExternalForm() will convert a space character into %20 which breaks java.lang.File use of the resulting path.
	                    String eform = url.toExternalForm();
	
	                    // Try to decide the name of the .jar file to have a
	                    // reference point for later..
	                    int start = 4; //"jar:".length();
	                    int end = eform.indexOf("!/", start);
	                    // eform has the "jar:url!/entry" format
	                    if (end != -1) {
		                        try {
									URL jarUrl = CoreUtility.createURL(eform.substring(start, end));
									String jarName = new File(jarUrl.getFile()).getName();
									try {
										URL debuginfoURL = debuginfosByURL.get(url);
										logger.log(Level.FINEST, ()->debuginfoURL==null ? String.format("Extracting libraries from %1$s", url.getFile().replace(DEPLOY_XML_IN_JAR, "")) : String.format("Extracting libraries from %1$s and %2$s", url.getFile().replace(DEPLOY_XML_IN_JAR, ""), debuginfoURL.getFile().replace(DEPLOY_XML_IN_JAR, "")));
									    spec = prepareNativeDeployment(url, debuginfoURL, jarName, null);
									} catch (ParserConfigurationException | SAXException | ZipException e) {
										logger.log(Level.WARNING, String.format("Unable to load native libraries from %1$s: %2$s", (jarName==null ? jarUrl : jarName), e.getMessage()), e);
									} catch (IOException e) {
									}
								} catch (MalformedURLException e) {
									logger.log(Level.WARNING, "", e);
								}
	                    }
	                } else if (protocol.equals("file")) {
	                    // No unpack since we presume we are already unpacked
	                    try {
	                    	URL debuginfoURL = debuginfosByURL.get(url);
	                    	logger.log(Level.FINEST, ()->debuginfoURL==null ? String.format("Extracting libraries from %1$s", url.getFile().replace(DEPLOY_XML_IN_JAR, "")) : String.format("Extracting libraries from %1$s and %2$s", url.getFile().replace(DEPLOY_XML_IN_JAR, ""), debuginfoURL.getFile().replace(DEPLOY_XML_IN_JAR, "")));
							spec = prepareNativeDeployment(url, debuginfoURL, null, Boolean.FALSE);
						} catch (ParserConfigurationException | SAXException | ZipException e) {
							logger.log(Level.WARNING, String.format("Unable to load native libraries from %1$s: %2$s", url, e.getMessage()), e);
						} catch (IOException e) {
						}
	                }
	                if(spec != null) {
	                	if("qtjambi".equals(spec.module())) {
	                		nativeDeployments.add(0, spec);
	                	}else {
	                		nativeDeployments.add(spec);
	                	}
	                }
	            }
	            if(!nativeDeployments.isEmpty()) {
	            	LibraryBundle qtjambiSpec = nativeDeployments.get(0);
                	configuration = qtjambiSpec.configuration();
	            	switch (operatingSystem) {
	                case Windows:
	                	qtJambiLibraryPath = new File(qtjambiSpec.extractionDir(), "bin");
	                	break;
	            	default:
	            		qtJambiLibraryPath = new File(qtjambiSpec.extractionDir(), "lib");
	            		break;
	            	}
	            	for(LibraryBundle spec : nativeDeployments) {
	            		if(spec.compiler()!=null && qtjambiSpec.compiler()!=null && !spec.compiler().equals(qtjambiSpec.compiler())) {
	            			if(operatingSystem==OperatingSystem.Windows) {
	            				if((spec.compiler().startsWith("msvc20") && !qtjambiSpec.compiler().startsWith("msvc20"))
	            						|| (qtjambiSpec.compiler().startsWith("msvc20") && !spec.compiler().startsWith("msvc20"))
	            						|| (spec.compiler().endsWith("x64") && !qtjambiSpec.compiler().endsWith("x64"))
	            						|| (qtjambiSpec.compiler().endsWith("x64") && !spec.compiler().endsWith("x64")))
			            			throw new WrongBuildException(String.format("Native deployments of different builts: %1$s (%2$s) and %3$s (%4$s)", 
			            					qtjambiSpec.module(), qtjambiSpec.compiler(),  
			            					spec.module(), spec.compiler()));
	            			}
	            		}else if(!spec.configuration().equals(qtjambiSpec.configuration())) {
	            			if(operatingSystem==OperatingSystem.Windows || spec.module()==null || !spec.module().startsWith("qt.")) {
		            			throw new WrongConfigurationException(String.format("Native deployments of different configurations: %1$s and %2$s", qtjambiSpec.configuration(), spec.configuration()));
	            			}
	            		}
	            	}
	            }else if(configuration==null){
	            	configuration = decideConfiguration();
	            }
	    	}else {
	    		if(configuration==null)
	            	configuration = decideConfiguration();
	    		deleteTmpDeployment = false;
	    		dontSearchDeploymentSpec = true;
	    		jambiDeploymentDir = jambiLibraryDir;
	    		jambiSourcesDir = null;
	    		jambiHeadersDir = null;
	    		jambiJarDir = null;
	    		isMavenRepo = false;
		    	isGradleCache = false;
            	isNativeSubdir = false;
            	isDebuginfoSubdir = false;
            	osArchName = _osArchName;
	    	}
		} catch (RuntimeException | Error e) {
			logger.log(Level.WARNING, "", e);
			throw e;
		} catch (Throwable e) {
			logger.log(Level.WARNING, "", e);
			throw new ExceptionInInitializerError(e);
        }
    }
    
    private static void clearAndDelete(File directory) {
    	if(directory!=null) {
    		if(directory.isDirectory()) {
		    	for(File file : directory.listFiles()) {
		    		if(file.getName().equals(".") || file.getName().equals(".."))
		    			continue;
		    		if(file.isDirectory() && !Files.isSymbolicLink(file.toPath())) {
		    			clearAndDelete(file);
		    		}else {
		    			file.delete();
		    		}
		    	}
    		}
	    	directory.delete();
    	}
    }
    
    private static List<String> resolveSystemLibraries(List<String> tmpSystemLibrariesList) {
        if(tmpSystemLibrariesList == null)
            return null;
        List<String> resolvedList = new ArrayList<String>();
        for(String original : tmpSystemLibrariesList) {
            String s = original;
            if(File.separatorChar!='/')   // convert "/" into "\"
                s = s.replace('/', File.separatorChar);

            String resolvedPath = null;
            synchronized(loadedNativeDeploymentUrls) {
	            for(LibraryBundle deploymentSpec : nativeDeployments) {
	                File f = new File(deploymentSpec.extractionDir(), s);
	                if(f.isFile()) {
	                    resolvedPath = s;
	                    break;
	                }
	
	                File libDir = new File(deploymentSpec.extractionDir(), "lib");
	                f = new File(libDir, s);
	                if(f.isFile()) {
	                    resolvedPath = "lib" + File.separator + s;
	                    break;
	                }
	
	                File binDir = new File(deploymentSpec.extractionDir(), "bin");
	                f = new File(binDir, s);
	                if(f.isFile()) {
	                    resolvedPath = "bin" + File.separator + s;
	                    break;
	                }
	            }
            }

            if(resolvedPath != null)
                resolvedList.add(resolvedPath);
            else
                System.err.println("IGNORED version.properties qtjambi.system.libraries entry \"" + original + "\": file could not be found");
        }
        return Collections.unmodifiableList(resolvedList);
    }

	static LibraryBundle.Configuration configuration() {
		return configuration;
	}
	
    private static LibraryBundle.Configuration decideConfiguration() {
    	LibraryBundle.Configuration configuration = null;

        String debugString = System.getProperty("io.qt.debug");
        try {
            if(debugString != null) {
                // This was added to allow unit tests to specify Configuration (as no MANIFEST.MF is available)
                if(LibraryBundle.Configuration.Release.toString().compareToIgnoreCase(debugString) == 0)
                    configuration = LibraryBundle.Configuration.Release;
                else if(LibraryBundle.Configuration.Debug.toString().compareToIgnoreCase(debugString) == 0)
                    configuration = LibraryBundle.Configuration.Debug;

                if(configuration == null) {
                    Boolean booleanValue = Boolean.valueOf(debugString);
                    if((booleanValue != null && booleanValue.booleanValue()) || debugString.length() == 0) {
                        configuration = LibraryBundle.Configuration.Debug;
                        logger.log(java.util.logging.Level.WARNING, "-Dio.qt.debug=" + Boolean.TRUE + "; is set instead of =debug.");
                    }
                }
            }
        } catch(Exception e) {
            logger.log(java.util.logging.Level.SEVERE, "-Dio.qt.debug=" + Boolean.FALSE + "; is assumed default", e);
            // only because Configuration.Release is assumed
        }

        if(configuration==null)
        	configuration = LibraryBundle.Configuration.Release;
        return configuration;
    }
    
    static File jambiDeploymentDir() {
        return jambiDeploymentDir;
    }
    
    @NativeAccess
	@QtUninvokable
    private static void extractContainerAccessLib(String library) {
    	if(!noNativeDeployment && !useStaticLibs) {
	    	String libraryPlatformName;
	    	switch (operatingSystem) {
	        case Windows: 
	        	libraryPlatformName = "plugins/containeraccess/" + (configuration == LibraryBundle.Configuration.Debug && !isMinGWBuilt()
		                ? library + "d.dll"  // "QtFood4.dll"
		                : library + ".dll");
	        	break;
	        case IOS: 
	        case MacOS: 
				libraryPlatformName = "plugins/containeraccess/lib" + library + ".dylib";
	        	break;
	        case Android:
	        	switch (architecture) {
				case arm:
					libraryPlatformName = "libplugins_containeraccess_" + library + "_armeabi-v7a.so";
					break;
				case arm64:
					libraryPlatformName = "libplugins_containeraccess_" + library + "_arm64-v8a.so";
					break;
				case x86:
					libraryPlatformName = "libplugins_containeraccess_" + library + "_x86.so";
					break;
				case x86_64:
					libraryPlatformName = "libplugins_containeraccess_" + library + "_x86_64.so";
					break;
				default:
					libraryPlatformName = "libplugins_containeraccess_" + library;
					break;
				}
	        	return; // never extract on android
			default:
				if(operatingSystem.isUnixLike()) {
					libraryPlatformName = "plugins/containeraccess/lib" + library + ".so";
					break;
				}
				return;
	        }
	    	LibraryBundle.deploy(libraryPlatformName);
    	}
    }

    @NativeAccess
	@QtUninvokable
	static void clear() {
    	synchronized(loadedNativeDeploymentUrls) {
    		nativeDeployments.clear();
    	}
        if(deleteTmpDeployment) {
        	Preferences preferences = Preferences.userNodeForPackage(LibraryUtility.class);
	        if(jambiDeploymentDir.exists() && jambiDeploymentDir.isDirectory()) {
	        	logger.log(Level.FINEST, ()->"Deleting tmp deployment directory "+jambiDeploymentDir.getAbsolutePath()+".");
	        	clearAndDelete(jambiDeploymentDir);
	        	if(jambiDeploymentDir.exists() && jambiDeploymentDir.isDirectory()) {
	        		logger.log(Level.FINEST, "Preparing pending deletion...");
	        		String dirs = preferences.get("qtjambi.previous.deployment.dir", null);
	        		if(dirs!=null && !dirs.isEmpty()) {
	        			preferences.put("qtjambi.previous.deployment.dir", dirs + File.pathSeparator + jambiDeploymentDir.getAbsolutePath());
	        		}else {
	        			preferences.put("qtjambi.previous.deployment.dir", jambiDeploymentDir.getAbsolutePath());
	        		}
	        	}
	    	}else {
	    		logger.log(Level.FINEST, ()->"Tmp deployment directory "+jambiDeploymentDir.getAbsolutePath()+" does not exist.");
	    	}
        	try{
	        	Preferences pids = preferences.node("qtjambi.pids");
	        	pids.remove(RetroHelper.processName());
	        	if(pids.keys().length==0) {
	        		preferences.remove("qtjambi.pids");
	        	}
	        	pids.sync();
	        }catch(Throwable t) {}
        }
    }

    @NativeAccess
    private static List<String> pluginPaths() {
		pluginLibraries.removeIf( l -> {
			try {
				l.extract();
			}catch(Throwable t) {
				logger.log(java.util.logging.Level.SEVERE, "Error while extracting library", t);
			}
			if(isQmlLoaded.get()) {
				try {
					l.extractQml();
				}catch(Throwable t) {
					logger.log(java.util.logging.Level.SEVERE, "Error while extracting library", t);
				}
			}
			return true;
		});
        List<String> paths = new ArrayList<String>();
        synchronized(loadedNativeDeploymentUrls) {
	        for (LibraryBundle spec : nativeDeployments) {
	            if(spec.hasPluginPaths()) {
		            File root = spec.extractionDir();
	            	String p = new File(root, "plugins").getAbsolutePath();
	            	if(!paths.contains(p))
	            		paths.add(p);
	            }
	        }
        }
        return paths;
    }

    static void loadLibrary(String library) {
    	int idx = library.lastIndexOf('/');
    	String libPath = "";
    	if(idx>0) {
    		libPath = library.substring(0, idx+1);
    		library = library.substring(idx+1);
    	}
    	String libraryPlatformName;
    	if(useStaticLibs) {
    		libraryPlatformName = library;
    	}else {
	    	switch (operatingSystem) {
	        case Windows: 
	        	libraryPlatformName = library + ".dll";
	        	break;
	        case IOS: 
	        case MacOS: 
	        	{
	        		Availability availability = getLibraryAvailability(libPath, library, library + ".framework/" + library, Collections.emptyList(), null);
	        		if(availability.isAvailable()) {
	        			libraryPlatformName = library + ".framework/" + library;
	        		}else {
	        			availability = getLibraryAvailability(libPath, library, "lib" + library + ".dylib", Collections.emptyList(), null);
	        			if(availability.isAvailable()) {
	        				libraryPlatformName = "lib" + library + ".dylib";
	        			}else {
	        				libraryPlatformName = "lib" + library + ".jnilib";
	        			}
	        		}
	        	}
	        	break;
	        case Android:
	        	switch (architecture) {
				case arm:
					libraryPlatformName = library + "_armeabi-v7a";
					break;
				case arm64:
					libraryPlatformName = library + "_arm64-v8a";
					break;
				case x86:
					libraryPlatformName = library + "_x86";
					break;
				case x86_64:
					libraryPlatformName = library + "_x86_64";
					break;
				default:
					libraryPlatformName = library;
					break;
				}
	//        	libraryPlatformName = "lib" + libraryPlatformName + ".so";
	        	break;
			default:
				if(operatingSystem.isUnixLike()) {
					libraryPlatformName = "lib" + library + ".so";
		        	break;
				}
				return;
	        }
    	}
    	loadNativeLibrary(Object.class, getLibraryAvailability(libPath, library, libraryPlatformName, Collections.emptyList(), null), null);
    }
    
    static void loadQtJambiLibrary() {
    	try{
	    	loadSystemLibraries();
        	loadUtilityLibrary("icudata", LibraryUtility.ICU_VERSION, LibraryRequirementMode.Optional);
    		loadUtilityLibrary("icuuc", LibraryUtility.ICU_VERSION, LibraryRequirementMode.Optional);
    		loadUtilityLibrary("icui18n", LibraryUtility.ICU_VERSION, LibraryRequirementMode.Optional);
    	} catch (Throwable e) {
    		logger.log(java.util.logging.Level.SEVERE, "Error while loading system library", e);
    	}
    	Availability qtlibAvailability = getLibraryAvailability("Qt", "Core", LIBINFIX, null, configuration, null, QtJambi_LibraryUtilities.qtMajorVersion, QtJambi_LibraryUtilities.qtMinorVersion);
    	if(!qtlibAvailability.isAvailable()) {
			if(configuration==LibraryBundle.Configuration.Release) {
				// try to run qtjambi with debug libraries instead
				configuration=LibraryBundle.Configuration.Debug;
				Availability _availability = getLibraryAvailability("Qt", "Core", LIBINFIX, null, configuration, null, QtJambi_LibraryUtilities.qtMajorVersion, QtJambi_LibraryUtilities.qtMinorVersion);
				if(_availability.isAvailable())
					qtlibAvailability = _availability;
			}else {
				if(dontUseQtFrameworks==null) {
					dontUseQtFrameworks = Boolean.TRUE;
					Availability _availability = getLibraryAvailability("Qt", "Core", LIBINFIX, null, configuration, null, QtJambi_LibraryUtilities.qtMajorVersion, QtJambi_LibraryUtilities.qtMinorVersion);
					if(!_availability.isAvailable()) {
						configuration=LibraryBundle.Configuration.Debug;
						_availability = getLibraryAvailability("Qt", "Core", LIBINFIX, null, configuration, null, QtJambi_LibraryUtilities.qtMajorVersion, QtJambi_LibraryUtilities.qtMinorVersion);
						if(_availability.isAvailable())
							qtlibAvailability = _availability;
					}
				}else {
					if(isMinGWBuilt==null && operatingSystem==OperatingSystem.Windows) {
						Availability stdCAvailability = getLibraryAvailability(null, "libstdc++-6", null, null, LibraryBundle.Configuration.Release, null);
				    	if(stdCAvailability.entry!=null){
				    		isMinGWBuilt = Boolean.TRUE;
				    	}else if(stdCAvailability.file!=null){
				    		isMinGWBuilt = Boolean.valueOf(new File(stdCAvailability.file.getParentFile(), "Qt"+QtJambi_LibraryUtilities.qtMajorVersion+"Core.dll").isFile());
				    	}else {
				    		isMinGWBuilt = Boolean.FALSE;
				    	}
				    	if(isMinGWBuilt()) {
				    		qtlibAvailability = getLibraryAvailability("Qt", "Core", LIBINFIX, null, configuration, null, QtJambi_LibraryUtilities.qtMajorVersion, QtJambi_LibraryUtilities.qtMinorVersion);
				    	}
			    	}
				}
			}
    	}
    	if(!qtlibAvailability.isAvailable() && operatingSystem.isUnixLike() && !System.getProperties().containsKey("io.qt.library-path-override")) {
    		String key = QtJambi_LibraryUtilities.qtMajorVersion+"."+QtJambi_LibraryUtilities.qtMinorVersion;
    		String qtlibpath;
    		Preferences preferences = Preferences.userNodeForPackage(LibraryUtility.class);
    		Preferences pathsPref = preferences.node("qt.library.path");
    		qtlibpath = pathsPref.get(key, "");
    		boolean setPref = false;
    		if(!qtlibpath.isEmpty()){
    			File libDir = new File(qtlibpath);
				if(!libDir.exists()) {
					qtlibpath = "";
					pathsPref.remove(key);
				}
    		}
    		if(qtlibpath.isEmpty()) {
	    		String exeName = "qmake";
	    		if(Integer.compare(QtJambi_LibraryUtilities.qtMajorVersion, 5)==0) {
	    			exeName += "-qt5";
	    		}else {
	    			exeName += QtJambi_LibraryUtilities.qtMajorVersion;
	    		}
	    		try {
					Process process = Runtime.getRuntime().exec(new String[]{exeName, "-query", "QT_INSTALL_LIBS"});
					byte[] data;
					try(InputStream input = process.getInputStream()){
						process.waitFor();
						data = input.readAllBytes();
					}
					if(data!=null) {
						qtlibpath = new String(data);
						setPref = true;
					}
				} catch (Exception e) {
				}
    		}
    		if(!qtlibpath.isEmpty()){
    			File libDir = new File(qtlibpath);
				if(libDir.exists()) {
					String libPaths = System.getProperty("java.library.path");
					if(libPaths==null)
						libPaths = libDir.getAbsolutePath();
					else
						libPaths += File.pathSeparator + libDir.getAbsolutePath();
					System.setProperty("java.library.path", libPaths);
					if(setPref) {
						pathsPref.put(key, libDir.getAbsolutePath());
						try {
							pathsPref.sync();
							preferences.sync();
						} catch (Exception e) {
						}
					}
				}
    		}
    	}
        if(jambiSourcesDir!=null) {
        	switch (operatingSystem) {
        	case MacOS:
				logger.log(Level.INFO, ()->String.format("Call debugger command: set substitute-path ../../../../../sources/ %1$s/", new File(jambiSourcesDir, "sources").getAbsolutePath()));
				break;
        	case Windows:
        		if(!isMinGWBuilt()) {
        			logger.log(Level.INFO, ()->String.format("Call debugger command: srcpath %1$s", new File(jambiSourcesDir, "sources").getAbsolutePath()));
        			break;
        		}
			default:
				logger.log(Level.INFO, ()->String.format("Call debugger command: set substitute-path ../sources/ %1$s/", new File(jambiSourcesDir, "sources").getAbsolutePath()));
				break;
    		}
        }
    	java.io.File coreLib;
    	try{
    		coreLib = loadNativeLibrary(Object.class, qtlibAvailability, QtJambi_LibraryUtilities.qtMajorVersion, QtJambi_LibraryUtilities.qtMinorVersion);
    		if(coreLib!=null)
    			qtLibraryPath = coreLib.getParentFile();
        } catch(UnsatisfiedLinkError t) {
            LibraryUtility.analyzeUnsatisfiedLinkError(t, qtlibAvailability.file, qtlibAvailability, null);
            throw t;
		}finally {
			if(dontUseQtFrameworks==null) {
				dontUseQtFrameworks = Boolean.FALSE;
			}
		}
    	Availability[] qtjambiOrigin = {null};
    	java.io.File qtjambiLib;
        try{
            qtjambiLib = loadQtJambiLibrary(LibraryUtility.class, null, "QtJambi", qtjambiOrigin, QtJambi_LibraryUtilities.qtMajorVersion, QtJambi_LibraryUtilities.qtMinorVersion, QtJambi_LibraryUtilities.qtJambiPatch);
            if(LibraryUtility.operatingSystem!=OperatingSystem.Android) {
                List<String> paths = new ArrayList<>();
                String path;
                switch(LibraryUtility.operatingSystem) {
                case MacOS:
                    path = QtUtilities.getenv("DYLD_LIBRARY_PATH");
                    String path2 = QtUtilities.getenv("DYLD_FRAMEWORK_PATH");
                    List<String> paths2 = new ArrayList<>();
                	String dir;
                    if(qtjambiLib.getParentFile().getName().equals(""+QtJambi_LibraryUtilities.qtJambiPatch)
                    		&& qtjambiLib.getParentFile().getParentFile().getName().equals("Versions")
                    		&& qtjambiLib.getParentFile().getParentFile().getParentFile().getName().endsWith(".framework")) {
                    	dir = qtjambiLib.getParentFile().getParentFile().getParentFile().getParentFile().getAbsolutePath();
                        paths2.remove(dir);
                        paths2.add(0, dir);
                    	if(debugInfoDeployment) {
                            String sources = new File(qtjambiLib.getParentFile().getParentFile().getParentFile().getParentFile().getParentFile(), "sources").getAbsolutePath();
                            QtUtilities.putenv("SOURCE_PATH", sources);
                            QtUtilities.putenv("SRCROOT", sources);
                        }
                    }else {
                    	dir = qtjambiLib.getParentFile().getAbsolutePath();
                    }
                    paths.remove(dir);
                    paths.add(0, dir);
                    if(coreLib.getParentFile().getName().equals(""+QtJambi_LibraryUtilities.qtJambiPatch)
                    		&& coreLib.getParentFile().getParentFile().getName().equals("Versions")
                    		&& coreLib.getParentFile().getParentFile().getParentFile().getName().endsWith(".framework")) {
                    	dir = coreLib.getParentFile().getParentFile().getParentFile().getParentFile().getAbsolutePath();
                        paths2.remove(dir);
                        paths2.add(0, dir);
                    }else {
                    	dir = coreLib.getParentFile().getAbsolutePath();
                    }
                    paths.remove(dir);
                    paths.add(0, dir);
                    if(path2!=null && !path2.isEmpty()) {
                        for(String p : path2.split("\\"+java.io.File.pathSeparator)) {
                            if(!paths2.contains(p))
                                paths2.add(0, p);
                        }
                    }
                    path2 = String.join(java.io.File.pathSeparator, paths2);
                    QtUtilities.putenv("DYLD_FRAMEWORK_PATH", path2);
                    if(debugInfoDeployment) {
                        QtUtilities.putenv("DWARF_DSYM_FOLDER_PATH", path2);
                    }
                    break;
                case Windows:
                    path = QtUtilities.getenv("PATH");
                    paths.remove(qtjambiLib.getParentFile().getAbsolutePath());
                    paths.add(0, qtjambiLib.getParentFile().getAbsolutePath());
                    paths.remove(coreLib.getParentFile().getAbsolutePath());
                    paths.add(0, coreLib.getParentFile().getAbsolutePath());
                    break;
                default:
                    path = QtUtilities.getenv("LD_LIBRARY_PATH");
                    paths.remove(qtjambiLib.getParentFile().getAbsolutePath());
                    paths.add(0, qtjambiLib.getParentFile().getAbsolutePath());
                    paths.remove(coreLib.getParentFile().getAbsolutePath());
                    paths.add(0, coreLib.getParentFile().getAbsolutePath());
                    break;
                }
                if(path!=null && !path.isEmpty()) {
                    for(String p : path.split("\\"+java.io.File.pathSeparator)) {
                        if(!paths.contains(p))
                            paths.add(p);
                    }
                }
                path = String.join(java.io.File.pathSeparator, paths);
                switch(LibraryUtility.operatingSystem) {
                case MacOS:
                    QtUtilities.putenv("DYLD_LIBRARY_PATH", path);
                    break;
                case Windows:
                    QtUtilities.putenv("PATH", path);
//                    System.out.println("setting PATH="+path);
//                    System.out.println("testing PATH="+System.getenv("PATH"));
                    break;
                default:
                    QtUtilities.putenv("LD_LIBRARY_PATH", path);
                    if(debugInfoDeployment) {
                        QtUtilities.putenv("DEBUGINFODIR", path);
                        String sources = new File(qtjambiLib.getParentFile().getParentFile(), "sources").getAbsolutePath();
                        QtUtilities.putenv("SOURCE_PATH", sources);
                        QtUtilities.putenv("SRCROOT", sources);
                    }
                    break;
                }
            }
        } catch(UnsatisfiedLinkError t) {
            LibraryUtility.analyzeUnsatisfiedLinkError(t, coreLib, qtlibAvailability, qtjambiOrigin[0]);
            throw t;
        }
        int version;
        try {
        	version = qtJambiVersion();
        } catch(UnsatisfiedLinkError t) {
        	final List<String> qtjambiLibraryBuilds = readVersion(qtjambiLib, "QtJambi ".getBytes(StandardCharsets.US_ASCII));
        	if(qtjambiLibraryBuilds.isEmpty()) {
            	throw new UnsatisfiedLinkError("Cannot combine QtJambi " 
    					+ QtJambi_LibraryUtilities.qtMajorVersion + "." 
    					+ QtJambi_LibraryUtilities.qtMinorVersion + "." 
    					+ QtJambi_LibraryUtilities.qtJambiPatch + " with outdated native library.");        		
        	}else {
        		String qtjambiLibraryBuild = qtjambiLibraryBuilds.get(0);
            	throw new UnsatisfiedLinkError("Cannot combine QtJambi " 
    					+ QtJambi_LibraryUtilities.qtMajorVersion + "." 
    					+ QtJambi_LibraryUtilities.qtMinorVersion + "." 
    					+ QtJambi_LibraryUtilities.qtJambiPatch + " with native library "+qtjambiLibraryBuild+".");        		
        	}
        }
        int majorVersion = (version >> 16) & 0xff;
        int minorVersion = (version >> 8) & 0xff;
        int patchVersion = version & 0xff;
        if(majorVersion!=QtJambi_LibraryUtilities.qtMajorVersion || minorVersion!=QtJambi_LibraryUtilities.qtMinorVersion || patchVersion!=QtJambi_LibraryUtilities.qtJambiPatch) {
        	throw new UnsatisfiedLinkError("Cannot combine QtJambi " 
					+ QtJambi_LibraryUtilities.qtMajorVersion + "." 
					+ QtJambi_LibraryUtilities.qtMinorVersion + "." 
					+ QtJambi_LibraryUtilities.qtJambiPatch + " with native library "
					+ majorVersion + "." 
					+ minorVersion + "." 
					+ patchVersion + ".");
        }
        String _qtJambiConfFile = null;
        if(qtLibraryPath!=null && jambiDeploymentDir!=null) {
        	if(jambiDeploymentDir.equals(qtLibraryPath.getParentFile()) || jambiDeploymentDir.getAbsolutePath().contains(qtLibraryPath.getParentFile().getAbsolutePath())) {
	        	File file = new File(new File(qtLibraryPath.getParentFile(), "bin"), "qt.conf");
	        	if(file.exists()) {
	        		_qtJambiConfFile = file.getAbsolutePath();
	        	}
        	}
        }
        ResourceUtility.initialize(_qtJambiConfFile);
    }
    private static native int qtJambiVersion();

    static void loadQtJambiLibrary(Class<?> callerClass, String library) {
    	RetroHelper.enableNativeAccessOnModule(callerClass);
    	LibVersion libVersion = getLibVersion(callerClass);
    	try {
    		loadQtJambiLibrary(callerClass, "QtJambi", library, null, libVersion.qtMajorVersion, libVersion.qtMinorVersion, libVersion.qtJambiPatch);
    	} catch (RuntimeException | Error e) {
			if(libVersion.qtMajorVersion!=QtJambi_LibraryUtilities.qtMajorVersion 
					|| libVersion.qtMinorVersion!=QtJambi_LibraryUtilities.qtMinorVersion
					|| libVersion.qtJambiPatch!=QtJambi_LibraryUtilities.qtJambiPatch) {
				throw new LinkageError("Cannot combine QtJambi" + library + " " 
					+ libVersion.qtMajorVersion + "." 
					+ libVersion.qtMinorVersion + "." 
					+ libVersion.qtJambiPatch + " with QtJambi " 
					+ QtJambi_LibraryUtilities.qtMajorVersion + "." 
					+ QtJambi_LibraryUtilities.qtMinorVersion + "." 
					+ QtJambi_LibraryUtilities.qtJambiPatch + ".", e);
			}
			throw e;
		}
    }
    
    static void loadJambiLibrary(Class<?> callerClass, String library) {
    	LibVersion libVersion = getLibVersion(callerClass);
    	try {
    		loadQtJambiLibrary(callerClass, null, library, null, libVersion.qtMajorVersion, libVersion.qtMinorVersion, libVersion.qtJambiPatch);
    	} catch (RuntimeException | Error e) {
			if(libVersion.qtMajorVersion!=QtJambi_LibraryUtilities.qtMajorVersion 
					|| libVersion.qtMinorVersion!=QtJambi_LibraryUtilities.qtMinorVersion
					|| libVersion.qtJambiPatch!=QtJambi_LibraryUtilities.qtJambiPatch) {
				throw new LinkageError("Cannot combine " + library + " " 
					+ libVersion.qtMajorVersion + "." 
					+ libVersion.qtMinorVersion + "." 
					+ libVersion.qtJambiPatch + " with QtJambi " 
					+ QtJambi_LibraryUtilities.qtMajorVersion + "." 
					+ QtJambi_LibraryUtilities.qtMinorVersion + "." 
					+ QtJambi_LibraryUtilities.qtJambiPatch + ".", e);
			}
			throw e;
		}
    }
    
    private static File loadQtJambiLibrary(Class<?> callerClass, String prefix, String library, Availability[] originOut, int... versionArray) {
    	Availability availability = getQtJambiLibraryAvailability(prefix, library, null, null, configuration, null, versionArray);
    	if(!availability.isAvailable()
    			&& versionArray!=null 
        		&& versionArray.length==3 
        		&& !noNativeDeployment) {
        	String className = callerClass.getName();
        	int idx = className.lastIndexOf('.');
        	className = className.substring(idx+1);
        	URL _classURL = callerClass.getResource(className+".class");
        	if(_classURL!=null) {
	        	int index;
	        	String classURL = _classURL.toString();
	    		File jarFile = null;
		    	if(classURL.startsWith("jar:file:") && (index = classURL.indexOf("!/"))>0) {
		    		String jarFileURL = classURL.substring(4, index);
		    		try {
						jarFile = new File(CoreUtility.createURL(jarFileURL).toURI());
					} catch (Exception e) {
						jarFile = new File(jarFileURL.substring(5));
					}
		    	}
		    	if(jarFile==null || !jarFile.exists()) {
		    		try {
						URLConnection connection = _classURL.openConnection();
						if(connection instanceof JarURLConnection) {
							jarFile = new File(((JarURLConnection) connection).getJarFile().getName());
						}
					} catch (Throwable e) {
					}
		    	}
	    		if(jarFile!=null && jarFile.exists()) {
	    			final File directory;
	    			if(isNativeSubdir) {
	    				directory = new File(jarFile.getParentFile(), "native");
	    			}else {
	    				directory = jarFile.getParentFile();
	    			}
	    			final File debuginfoDirectory;
	    			if(isDebuginfoSubdir) {
	    				debuginfoDirectory = new File(jarFile.getParentFile(), "debuginfo");
	    			}else {
	    				debuginfoDirectory = jarFile.getParentFile();
	    			}
	    			String fileName = jarFile.getName();
	    			String qtjambiVersion = String.format("-%1$s.%2$s.%3$s", versionArray[0], versionArray[1], versionArray[2]);
    				index = fileName.indexOf(qtjambiVersion+".jar");
	    			if(index>0) {
	    				String moduleName = fileName.substring(0, index);
		    			String infix = "-native-" + osArchName;
		    			String dinfix = "-debuginfo-" + osArchName;
		    			if(configuration==LibraryBundle.Configuration.Debug)
		    				infix += "-debug";
		    			final File _directory;
		    			final File _debuginfoDirectory;
		    			if(isGradleCache) {
		    				File __directory = directory;
		    				File dir = new File(new File(directory.getParentFile().getParentFile().getParentFile(), moduleName + infix), String.format("%1$s.%2$s.%3$s", versionArray[0], versionArray[1], versionArray[2]));
		    				if(dir.isDirectory()) {
		    					for(File subdir : dir.listFiles()) {
		    						if(subdir.isDirectory() && new File(subdir, moduleName + infix + qtjambiVersion+".jar").exists()) {
		    							__directory = subdir;
		    							break;
		    						}
		    					}
		    				}
		    				File __debuginfoDirectory = directory;
		    				dir = new File(new File(directory.getParentFile().getParentFile().getParentFile(), moduleName + dinfix), String.format("%1$s.%2$s.%3$s", versionArray[0], versionArray[1], versionArray[2]));
		    				if(dir.isDirectory()) {
		    					for(File subdir : dir.listFiles()) {
		    						if(subdir.isDirectory() && new File(subdir, moduleName + dinfix + qtjambiVersion+".jar").exists()) {
		    							__debuginfoDirectory = subdir;
		    							break;
		    						}
		    					}
		    				}
		    				_directory = __directory;
		    				_debuginfoDirectory = __debuginfoDirectory;
		    			}else if(isMavenRepo) {
		    				_directory = new File(new File(directory.getParentFile().getParentFile(), moduleName + infix), String.format("%1$s.%2$s.%3$s", versionArray[0], versionArray[1], versionArray[2]));
		    				_debuginfoDirectory = new File(new File(directory.getParentFile().getParentFile(), moduleName + dinfix), String.format("%1$s.%2$s.%3$s", versionArray[0], versionArray[1], versionArray[2]));
		    			}else {
		    				_directory = directory;
		    				_debuginfoDirectory = debuginfoDirectory;
		    			}
		    			final File nativeFile = new File(_directory, moduleName + infix + qtjambiVersion+".jar");
		    			if(nativeFile.exists() && !dontSearchDeploymentSpec) {
		    				try {
								URL nativeFileURL = CoreUtility.createURL("jar:"+nativeFile.toURI()+DEPLOY_XML_IN_JAR);
								LibraryBundle deployment = null;
								synchronized(loadedNativeDeploymentUrls) {
									if(!loadedNativeDeploymentUrls.contains(nativeFileURL)) {
										loadedNativeDeploymentUrls.add(nativeFileURL);
										URL debuginfoFileURL = null;
										File debuginfoFile = null;
										if(debugInfoDeployment && configuration!=LibraryBundle.Configuration.Debug) {
											debuginfoFile = new File(_debuginfoDirectory, moduleName + dinfix + qtjambiVersion+".jar");
											if(debuginfoFile.exists()) {
												debuginfoFileURL = CoreUtility.createURL("jar:"+debuginfoFile.toURI()+DEPLOY_XML_IN_JAR);
											}else {
												debuginfoFile = null;
											}
										}
										File _debuginfoFile = debuginfoFile;
										logger.log(Level.FINEST, ()->_debuginfoFile==null ? String.format("Extracting libraries from %1$s", nativeFile.getAbsolutePath()) : String.format("Extracting libraries from %1$s and %2$s", nativeFile.getAbsolutePath(), _debuginfoFile.getAbsolutePath()));
										deployment = prepareNativeDeployment(nativeFileURL, debuginfoFileURL, nativeFile.getName(), null);
										if(deployment != null 
												&& String.format("%1$s.%2$s.%3$s", versionArray[0], versionArray[1], versionArray[2]).equals(deployment.version())
												&& deployment.configuration()==configuration
												&& (nativeDeployments.isEmpty() || deployment.compiler().equals(nativeDeployments.get(0).compiler()))) {
						                	if("qtjambi".equals(deployment.module()))
						                		nativeDeployments.add(0, deployment);
						                	else
						                		nativeDeployments.add(deployment);
						                	if(qtJambiLibraryPath==null) {
							                	switch (operatingSystem) {
							                    case Windows:
							                    	qtJambiLibraryPath = new File(deployment.extractionDir(), "bin");
							                    	break;
							                	default:
							                		qtJambiLibraryPath = new File(deployment.extractionDir(), "lib");
							                		break;
							                	}
						                	}
						                }else {
						                	deployment = null;
						                }
									}
								}
								if(deployment!=null) {
				                	availability = getQtJambiLibraryAvailability(prefix, library, null, null, configuration, moduleName + infix, versionArray);
								}
							} catch (ParserConfigurationException | SAXException | ZipException e) {
								logger.log(Level.WARNING, String.format("Unable to load native libraries from %1$s: %2$s", nativeFile.getName(), e.getMessage()), e);
							} catch (IOException e) {
							}
		    			}else {
		    				availability = getQtJambiLibraryAvailability(prefix, library, null, null, configuration, moduleName + infix, versionArray);
		    			}
	    			}
	    		}
        	}
        }
    	if(originOut!=null && originOut.length>0) {
    		originOut[0] = availability;
    	}
        return loadNativeLibrary(callerClass, availability, versionArray);
    }

	static void loadQtLibrary(Class<?> callerClass, String library, LibraryRequirementMode libraryRequirementMode, String...platforms) {
    	if(platforms!=null && platforms.length>0) {
			boolean skip = true;
			for(String platform : platforms) {
				if(osArchName.startsWith(platform)) {
					skip = false;
					break;
				}
			}
			if(skip)
				return;
		}
    	LibVersion libVersion = getLibVersion(callerClass);
    	Availability availability = getLibraryAvailability("Qt", library, LIBINFIX, null, configuration, null, libVersion.qtMajorVersion, libVersion.qtMinorVersion);
    	switch(libraryRequirementMode) {
		case ProvideOnly:
			availability.extractLibrary();
			return;
		case Optional:
			if(!availability.isAvailable()) {
				logger.log(Level.FINEST, ()->String.format("Library unavailable: %1$s", library));
				return;
			}
			break;
		default:
			break;
    	}
    	try {
    		switch(library) {
    		case "Qml":
				isQmlLoaded.set(true);
				LibraryBundle.deployQml();
				break;
    		}
			loadNativeLibrary(Object.class, availability, libVersion.qtMajorVersion, libVersion.qtMinorVersion);
		} catch (RuntimeException | Error e) {
			switch(libraryRequirementMode) {
			case Optional:
	        	logger.log(Level.FINEST, ()->e.getMessage());
				return;
			default:
				break;
			}
			if(libVersion.qtMajorVersion!=QtJambi_LibraryUtilities.qtMajorVersion
					|| libVersion.qtMinorVersion!=QtJambi_LibraryUtilities.qtMinorVersion) {
				throw new LinkageError("Cannot combine Qt" + library + " " + libVersion.qtMajorVersion + "." + libVersion.qtMinorVersion + " with Qt " + QtJambi_LibraryUtilities.qtMajorVersion + "." + QtJambi_LibraryUtilities.qtMinorVersion + ".", e);
			}
			throw e;
		}
    }
    
    static void loadUtilityLibrary(String library, String version, LibraryRequirementMode libraryRequirementMode, String...platforms) {
    	if(platforms!=null && platforms.length>0) {
			boolean skip = true;
			for(String platform : platforms) {
				if(osArchName.startsWith(platform)) {
					skip = false;
					break;
				}
			}
			if(skip)
				return;
		}
    	Availability availability = getLibraryAvailability(null, library, LIBINFIX, version, configuration, null);
    	switch(libraryRequirementMode) {
		case ProvideOnly:
			availability.extractLibrary();
			return;
		case Optional:
			if(!availability.isAvailable()) {
				if(library.startsWith("icu")) {
					if(LIBINFIX==null || LIBINFIX.isEmpty()) {
						availability = getLibraryAvailability(null, library, null, "", configuration, null);
					}else {
						availability = getLibraryAvailability(null, library, null, version, configuration, null);
						if(!availability.isAvailable())
							availability = getLibraryAvailability(null, library, null, "", configuration, null);
					}
				}
				return;
			}
			try {
				loadNativeLibrary(Object.class, availability, null);
			}catch(RuntimeException | Error t){
				logger.log(Level.FINEST, "Unable to load optional library "+library, t);
			}
			break;
		default:
	    	loadNativeLibrary(Object.class, availability, null);
			break;
    	}
    }
    
    private static LibVersion getLibVersion(Class<?> callerClass) {
    	return qtJambiVersionsByClass.computeIfAbsent(callerClass, cls -> {
    		if(callerClass.getSimpleName().equals("QtJambi_LibraryUtilities")) {
    			io.qt.ModuleVersion moduleVersion = callerClass.getAnnotation(io.qt.ModuleVersion.class);
    			if(moduleVersion!=null) {
    				return new LibVersion(moduleVersion.major(), moduleVersion.minor(), moduleVersion.patch());
    			}
	    		try {
	    			int qtMajorVersion = ReflectionUtility.readField(null, callerClass, "qtMajorVersion", int.class);
	    			int qtMinorVersion = ReflectionUtility.readField(null, callerClass, "qtMinorVersion", int.class);
	    			int qtJambiPatch = ReflectionUtility.readField(null, callerClass, "qtJambiPatch", int.class);
	    			return new LibVersion(qtMajorVersion, qtMinorVersion, qtJambiPatch);
	    		}catch(Throwable t) {}
	    		
	        	URL _classURL = callerClass.getResource(callerClass.getSimpleName()+".class");
	        	if(_classURL!=null) {
		        	int index;
		        	String classURL = _classURL.toString();
		    		File jarFile = null;
			    	if(classURL.startsWith("jar:file:") && (index = classURL.indexOf("!/"))>0) {
			    		String jarFileURL = classURL.substring(4, index);
			    		try {
							jarFile = new File(CoreUtility.createURL(jarFileURL).toURI());
						} catch (Exception e) {
							jarFile = new File(jarFileURL.substring(5));
						}
			    	}
			    	if(jarFile==null || !jarFile.exists()) {
			    		try {
							URLConnection connection = _classURL.openConnection();
							if(connection instanceof JarURLConnection) {
								jarFile = new File(((JarURLConnection) connection).getJarFile().getName());
							}
						} catch (Throwable e) {
						}
			    	}
			    	if(jarFile!=null && jarFile.exists()) {
			    		String fileName = jarFile.getName();
			    		if(fileName.endsWith(".jar") && (index = fileName.lastIndexOf('-'))>0) {
			    			fileName = fileName.substring(index+1);
			    			String[] versionString = fileName.split("\\.");
			    			return new LibVersion(Integer.parseInt(versionString[0]), Integer.parseInt(versionString[1]), Integer.parseInt(versionString[2]));
			    		}
			    	}
	        	}
    		}
    		return new LibVersion(QtJambi_LibraryUtilities.qtMajorVersion, QtJambi_LibraryUtilities.qtMinorVersion, QtJambi_LibraryUtilities.qtJambiPatch);
    	});
    }
    
    public static boolean isMinGWBuilt() {
		return Boolean.TRUE.equals(isMinGWBuilt);
	}
    
    static boolean isAvailableQtLibrary(Class<?> callerClass, String library) {
    	LibVersion version = getLibVersion(callerClass);
    	return getLibraryAvailability("Qt", library, LIBINFIX, null, configuration, null, version.qtMajorVersion, version.qtMinorVersion).isAvailable();
    }
    
    static Availability getQtLibraryAvailability(Class<?> callerClass, String library) {
    	LibVersion version = getLibVersion(callerClass);
    	return getLibraryAvailability("Qt", library, LIBINFIX, null, configuration, null, version.qtMajorVersion, version.qtMinorVersion);
    }
    
    static boolean isAvailableLibrary(String library, String version) {
    	if(version!=null && !version.isEmpty()) {
	    	String[] parts = version.split("\\.");
	    	int[] iparts = new int[parts.length];
	    	for (int i = 0; i < iparts.length; i++) {
	    		try {
					iparts[i] = Integer.parseInt(parts[i]);
				} catch (Exception e) {
					iparts = null;
					break;
				}
			}
	    	if(iparts!=null)
	    		return getLibraryAvailability(null, library, null, null, configuration, null, iparts).isAvailable();
    	}
    	return getLibraryAvailability(null, library, null, version, configuration, null).isAvailable();
    }
    
    private static class Availability{
    	public Availability(String libraryRawName, File file, String libFormat, List<String> replacements, String expectedModuleName) {
			super();
			this.libraryRawName = libraryRawName;
			this.file = file;
			this.entry = null;
			this.libFormat = libFormat;
			this.replacements = replacements;
			this.expectedModuleName = expectedModuleName;
		}
    	public Availability(String libraryRawName, String libFormat, List<String> replacements, String expectedModuleName) {
			super();
			this.libraryRawName = libraryRawName;
			this.file = null;
			this.entry = null;
			this.libFormat = libFormat;
			this.replacements = replacements;
			this.expectedModuleName = expectedModuleName;
		}
    	public Availability(String libraryRawName, Library entry, File file, String libFormat, Iterable<String> replacements, String expectedModuleName) {
			super();
			this.libraryRawName = libraryRawName;
			this.file = file;
			this.entry = entry;
			this.libFormat = libFormat;
			this.replacements = replacements;
			this.expectedModuleName = expectedModuleName;
		}
    	final String libraryRawName;
		final File file;
		final Library entry;
		final Iterable<String> replacements;
		final String libFormat;
		final String expectedModuleName;
		boolean isAvailable() {
			return (file!=null && file.exists()) || entry!=null;
		}
		
		void extractLibrary() {
			if(entry!=null && entry.isExtracting() && !entry.isLoaded()) {
				try {
        			entry.extract();
				} catch (Throwable e1) {
					throw new QLibraryNotLoadedError("Unable to extract library "+entry.getName(), e1);
				}
			}
		}
    }

    private static Availability getQtJambiLibraryAvailability(String qtprefix, String library, String libInfix, String versionStrg, LibraryBundle.Configuration configuration, String expectedModuleName, int... version) {
    	List<String> replacements = new ArrayList<>();
    	int idx = library.lastIndexOf('/');
    	String libPath = "";
    	if(idx>0) {
    		libPath = library.substring(0, idx+1);
    		library = library.substring(idx+1);
    	}
    	String libFormat = qtjambiLibraryName(qtprefix, library, dontUseQtJambiFrameworks!=null && dontUseQtJambiFrameworks, configuration, replacements, version);
    	Availability av = getLibraryAvailability(libPath, qtprefix==null ? library : qtprefix+library, libFormat, replacements, expectedModuleName);
    	if(!av.isAvailable() && operatingSystem==OperatingSystem.MacOS && dontUseQtJambiFrameworks!=null && dontUseQtJambiFrameworks) {
			libFormat = qtjambiLibraryName(qtprefix, library, false, configuration, replacements, version);
			Availability av2 = getLibraryAvailability(libPath, qtprefix==null ? library : qtprefix+library, libFormat, replacements, expectedModuleName);
			if(av2.isAvailable())
				av = av2;
    	}
    	return av;
    }

    private static Availability getLibraryAvailability(String qtprefix, String library, String libInfix, String versionStrg, LibraryBundle.Configuration configuration, String expectedModuleName, int... version) {
    	List<String> replacements = new ArrayList<>();
    	int idx = library.lastIndexOf('/');
    	String libPath = "";
    	if(idx>0) {
    		libPath = library.substring(0, idx+1);
    		library = library.substring(idx+1);
    	}
    	String libFormat = qtLibraryName(qtprefix, library, libInfix, versionStrg, configuration, replacements, version);  // "QtDBus" => "libQtDBus.so.4"
    	return getLibraryAvailability(libPath, qtprefix==null ? library : qtprefix+library, libFormat, replacements, expectedModuleName);
    }
    
    private static List<String> computeLibraryPaths(){
    	String libPaths = System.getProperty("io.qt.library-path-override");
    	String libPaths2 = null;
    	if (libPaths == null || libPaths.length() == 0) {
            libPaths = System.getProperty("java.library.path");
        }
    	List<String> libraryPaths = new ArrayList<>();
        if (libPaths != null)
        	libraryPaths.addAll(Arrays.asList(libPaths.split("\\"+File.pathSeparator)));
        switch (operatingSystem) {
        case Windows:
    		libPaths = System.getenv("PATH");
        	break;
        case MacOS: 
    		libPaths = System.getenv("DYLD_FRAMEWORK_PATH");
    		libPaths2 = System.getenv("DYLD_LIBRARY_PATH");
        	break;
    	default:
    		libPaths = System.getenv("LD_LIBRARY_PATH");
    	}
        if (libPaths != null)
            libraryPaths.addAll(ldLibraryPaths.computeIfAbsent(libPaths, LibraryUtility::splitPath));
        if (libPaths2 != null)
            libraryPaths.addAll(ldLibraryPaths.computeIfAbsent(libPaths2, LibraryUtility::splitPath));
        synchronized(loadedNativeDeploymentUrls) {
	        if(qtJambiLibraryPath!=null)
	        	libraryPaths.add(qtJambiLibraryPath.getAbsolutePath());
        }
        switch (operatingSystem) {
        case Windows:
        	libraryPaths.add(new File(jambiDeploymentDir, "bin").getAbsolutePath());
        	if(jambiJarDir!=null && new File(jambiJarDir, "bin").isDirectory())
        		libraryPaths.add(new File(jambiJarDir, "bin").getAbsolutePath());
        	break;
    	default:
    		libraryPaths.add(new File(jambiDeploymentDir, "lib").getAbsolutePath());
        	if(jambiJarDir!=null && new File(jambiJarDir, "lib").isDirectory())
        		libraryPaths.add(new File(jambiJarDir, "lib").getAbsolutePath());
    		break;
    	}
        
        if (System.getProperties().containsKey("io.qt.library-path-override")) {
//            reporter.report(" - using 'io.qt.library-path-override'");
        	libraryPaths.addAll(javaLibraryPathOverrides.computeIfAbsent(System.getProperty("io.qt.library-path-override"), LibraryUtility::splitPath));
        } else {
//            reporter.report(" - using 'java.library.path'");
        	libraryPaths.addAll(javaLibraryPaths.computeIfAbsent(System.getProperty("java.library.path"), LibraryUtility::splitPath));
        }
        
        return mergeJniLibdir(libraryPaths);
    }
    
    private static Availability getLibraryAvailability(String libPath, String libraryRawName, String libFormat, List<String> replacements, String expectedModuleName) {
    	if(operatingSystem!=OperatingSystem.Android
    			&& !useStaticLibs) {
	    	List<String> libraryPaths = null;
	        // search active deploymentSpec for existance of library
	        Iterator<String> iter = replacements.iterator();
	        do {
	        	String lib;
	        	if(!iter.hasNext()) {
	        		lib = libFormat;
	        	}else {
	        		lib = String.format(libFormat, iter.next());
	        	}
	        	if(!noNativeDeployment) {
		        	Library entry = LibraryBundle.findLibrary(libPath, lib);
		        	if(entry!=null) {
		        		return new Availability(libraryRawName, entry, entry.extractionPath(), libFormat, replacements, expectedModuleName);
		        	}
	        	}
	        	if(libraryPaths==null)
	        		libraryPaths = computeLibraryPaths();
		        for (String path : libraryPaths) {
		        	File dir;
		        	if(!libPath.isEmpty()) {
		        		dir = new File(path, File.separatorChar=='/' ? libPath : libPath.replace('/', '\\'));
		        	}else {
		        		dir = new File(path);
		        	}
		            File f = new File(dir, lib);
		            if (f.exists()) {
		            	return new Availability(libraryRawName, f, libFormat, replacements, expectedModuleName);
		            }
		        }
	        }while(iter.hasNext());
    	}else if(operatingSystem==OperatingSystem.Android){
    		String libraryFilePath = libraryFilePath();
    		File dir = new File(libraryFilePath).getParentFile();
    		if(!libPath.isEmpty()) {
    			dir = new File(dir, libPath);
    		}
	        Iterator<String> iter = replacements.iterator();
	        do {
	        	String lib;
	        	if(!iter.hasNext()) {
	        		lib = libFormat;
	        	}else {
	        		lib = String.format(libFormat, iter.next());
	        	}
	    		File f = new File(dir, "lib"+lib+".so");
	    		if (f.exists()) {
	            	return new Availability(libraryRawName, f, libFormat, replacements, expectedModuleName);
	            }
	        }while(iter.hasNext());
    	}
        return new Availability(libraryRawName, libFormat, replacements, expectedModuleName);
    }
    
    private static native String libraryFilePath();

    /**
     * Returns a classloader for current context...
     * @return The classloader
     */
    private static ClassLoader classLoader() {
        ClassLoader loader = Thread.currentThread().getContextClassLoader();
        if (loader == null) {
            loader = LibraryUtility.class.getClassLoader();
            assert(loader != null);
        }
        return loader;
    }

    private static File loadNativeLibrary(Class<?> callerClass, Availability availability, int... versionArray) {
        try {
        	if(loadedLibraries.contains(availability.libFormat))
        		return null;
        	if(operatingSystem==OperatingSystem.Android || useStaticLibs) {
    	        ClassLoader callerClassLoader = callerClass.getClassLoader();
        		if(callerClassLoader==LibraryUtility.class.getClassLoader()
                		|| callerClassLoader==Object.class.getClassLoader()) {
        			Runtime.getRuntime().loadLibrary(availability.libFormat);
    			}else {
                	libraryLoader.accept(callerClass, availability.libFormat);
                }
                loadedLibraries.add(availability.libFormat);
        		return null;
        	}else {
    	    	if(availability.entry!=null && availability.entry.isLoaded()) {
    				File result = availability.entry.extractionPath();
    		        if(operatingSystem==OperatingSystem.MacOS && (dontUseQtFrameworks==null || !dontUseQtFrameworks) && qtLibraryPath==null
    		        		&& !result.getName().endsWith(".dylib") && !result.getName().endsWith(".jnilib")) {
    		        	result = result.getParentFile().getParentFile().getParentFile();
    		        }
    	            return result;
    			}
    	        ClassLoader callerClassLoader = callerClass.getClassLoader();
    	        boolean qmlLoaded = isQmlLoaded.get();
    	        File libFile = availability.file;
    	        if(libFile==null && availability.entry!=null) {
    	    		libFile = availability.entry.extractionPath();
    	        }
    	        if(libFile==null) {
    	        	List<String> fileNames = new ArrayList<>();
    	        	Iterator<String> iter = availability.replacements.iterator();
    		        do {
    		        	if(!iter.hasNext()) {
    		        		fileNames.add(availability.libFormat);
    		        	}else {
    		        		fileNames.add(String.format(availability.libFormat, iter.next()));
    		        	}
    		        }while(iter.hasNext());
    	    		String message;
    		        if(availability.entry!=null) {
    		        	message = String.format("Library %1$s (%2$s) was not found in %3$s", availability.libraryRawName, String.join(", ", fileNames), availability.entry.bundle().origin());
    		        }else {
    		        	if(availability.expectedModuleName!=null) {
    		        		message = String.format("Library %1$s (%2$s) was not found due to missing module '%3$s'", availability.libraryRawName, String.join(", ", fileNames), availability.expectedModuleName);
    		        	}else {
		    	    		if (System.getProperties().containsKey("io.qt.library-path-override")) {
		    	    			message = String.format("Library %1$s (%2$s) was not found in 'io.qt.library-path-override=%3$s%4$s%5$s'", availability.libraryRawName, String.join(", ", fileNames), System.getProperty("io.qt.library-path-override"), File.pathSeparator, jambiDeploymentDir);
		    				}else {
		    	    			message = String.format("Library %1$s (%2$s) was not found in 'java.library.path=%3$s%4$s%5$s'", availability.libraryRawName, String.join(", ", fileNames), System.getProperty("java.library.path"), File.pathSeparator, jambiDeploymentDir);
		    				}
    		        	}
    		        }
    				throw new QLibraryNotFoundError(message);
    	        }
    	        if(availability.entry!=null && (availability.entry.isExtracting() || (qmlLoaded && availability.entry.isQmlExtracting()))) { // might be just writing
    	        	if(availability.entry.isExtracting()) {
    	        		try {
    	        			availability.entry.extract();
    					} catch (Throwable e1) {
    						throw new QLibraryNotLoadedError("Unable to extract library "+availability.entry.getName(), e1);
    					}
    	        	}
    	        	if(qmlLoaded && availability.entry.isQmlExtracting()) {
    	        		try {
    	        			availability.entry.extractQml();
    					} catch (Throwable e1) {
    						throw new QLibraryNotLoadedError("Unable to extract library "+availability.entry.getName(), e1);
    					}
    	        	}
    	        }
    	    	if(libFile.exists()) {
    	    		File _libFile = libFile;
    	    		logger.log(Level.FINEST, ()->String.format("Loading library: '%1$s' from location: %2$s", availability.libraryRawName, _libFile.getAbsolutePath()));
    	            if(callerClassLoader==LibraryUtility.class.getClassLoader()
    	            		|| callerClassLoader==Object.class.getClassLoader()) {
    	            	Runtime.getRuntime().load(availability.file.getAbsolutePath());
    	            }else {
    	            	libraryLoader.accept(callerClass, availability.file.getAbsolutePath());
    	            }
    	            loadedLibraries.add(availability.libFormat);
    	            File result = libFile;
    		        if(operatingSystem==OperatingSystem.MacOS && (dontUseQtFrameworks==null || !dontUseQtFrameworks) && qtLibraryPath==null
    		        		&& !result.getName().endsWith(".dylib") && !result.getName().endsWith(".jnilib")) {
    		        	result = result.getParentFile().getParentFile().getParentFile();
    		        }
    		        if(availability.entry!=null)
    		        	availability.entry.setLoaded(true);
    	            return result;
    	    	}else {
    	    		String message;
    	    		if(availability.entry!=null) {
    	    			message = String.format("Library %1$s (%2$s) was not found in %3$s", availability.libraryRawName, libFile.getName(), availability.entry.bundle().origin());
    	    		}else {
    	    			if (System.getProperties().containsKey("io.qt.library-path-override")) {
    	        			message = String.format("Library %1$s (%2$s) was not found in 'io.qt.library-path-override=%3$s'", availability.libraryRawName, libFile.getName(), System.getProperty("io.qt.library-path-override"));
    	    			}else {
    	        			message = String.format("Library %1$s (%2$s) was not found in 'java.library.path=%3$s'", availability.libraryRawName, libFile.getName(), System.getProperty("java.library.path"));
    	    			}
    	    		}
    				throw new QLibraryNotFoundError(message);
    	    	}
        	}
        } catch (RuntimeException | Error e) {
        	if(availability.entry!=null) {
    			throw new QLibraryNotLoadedError(String.format(e instanceof LinkageError && e.getMessage()!=null && !e.getMessage().isEmpty() 
						    					? "%1$s extracted from %2$s" 
												: "Unable to load %3$s extracted from %2$s", 
												e.getMessage(), availability.entry.bundle().origin(), availability.file.getAbsolutePath()), e);
        	}
        	throw e;
        } catch (Throwable e) {
    		if(e.getMessage().isEmpty()) {
        		throw new QLibraryNotLoadedError("Loading library failed.", e);
        	}else {
        		throw new QLibraryNotLoadedError(e.getMessage(), e);
        	}
        }
    }
    
    private static List<String> splitPath(String path){
    	if(path!=null) {
    		return Arrays.asList(path.split("\\"+File.pathSeparator));
    	}else {
    		return Collections.emptyList();
    	}
    }

    private static class ExecutorThread extends Thread implements Executor{
    	
    	private final List<Runnable> commands = new LinkedList<>();
    	private boolean noMoreExpected = false;
    	
		private ExecutorThread(String module) {
			super();
			this.setDaemon(true);
			this.setName("QtJambi_LibraryExtractor_"+module);
		}

		@Override
		public void execute(Runnable command) {
			synchronized(this.commands) {
				this.commands.add(command);
				this.commands.notifyAll();
			}
		}

		@Override
		public void run() {
			try {
				while(true) {
					Runnable first;
					synchronized(this.commands) {
						if(this.commands.isEmpty()) {
							if(noMoreExpected)
								break;
							this.commands.wait();
							if(!this.commands.isEmpty()) {
								first = this.commands.remove(0);
							}else {
								if(noMoreExpected)
									break;
								continue;
							}
						}else {
							first = this.commands.remove(0);
						}
					}
					try {
						first.run();
					} catch (Throwable e) {
						if(e instanceof InterruptedException)
							throw e;
						logger.log(java.util.logging.Level.SEVERE, "", e);
					}
				}
			} catch (InterruptedException e) {
			}
		}

		void setNoMoreExpected() {
			synchronized(this.commands) {
				noMoreExpected = true;
				this.commands.notifyAll();
			}
		}
    }
    
    private static Library.ExtractionFunction getLibraryExtractor(String urlBase, String libName, File outFile, boolean executable, boolean isDebug, boolean isQtLib, int... version){
    	String qtLibName = null;
    	if(isQtLib) {
	    	qtLibName = outFile.getName();
	        switch(operatingSystem) {
			case Android:
				String prefix = "libQt"+QtJambi_LibraryUtilities.qtMajorVersion;
				if(qtLibName.startsWith(prefix)) {
					qtLibName = qtLibName.substring(prefix.length());
		        	String suffix;
					switch (architecture) {
					case arm:
						suffix = "_armeabi-v7a.so";
						break;
					case arm64:
						suffix = "_arm64-v8a.so";
						break;
					case x86:
						suffix = "_x86.so";
						break;
					case x86_64:
						suffix = "_x86_64.so";
						break;
					default:
						suffix = ".so";
						break;
					}
					if(qtLibName.endsWith(suffix)) {
						qtLibName = qtLibName.substring(0, qtLibName.length() - suffix.length());
					}else {
						qtLibName = null;
					}
				}
				break;
			case MacOS:
				if(!libName.contains(".framework/")) {
					prefix = "libQt"+QtJambi_LibraryUtilities.qtMajorVersion;
					int dot = qtLibName.indexOf('.');
					if(dot>0) {
						qtLibName = qtLibName.substring(0, dot);
					}else {
						qtLibName = null;
					}
				}
				break;
			case Windows:
				prefix = "Qt"+QtJambi_LibraryUtilities.qtMajorVersion;
				String suffix = isDebug ? "d.dll" : ".dll";
				if(qtLibName.endsWith(suffix)) {
					qtLibName = qtLibName.substring(0, qtLibName.length() - suffix.length());
				}else {
					qtLibName = null;
				}
				break;
			default:
				if(operatingSystem.isUnixLike()) {
					prefix = "libQt"+QtJambi_LibraryUtilities.qtMajorVersion;
					int dot = qtLibName.indexOf('.');
					if(dot>0) {
						qtLibName = qtLibName.substring(0, dot);
					}else {
						qtLibName = null;
					}
				}
				break;
	        }
    	}
		List<Dependency> dependencies = qtLibName==null ? Collections.emptyList() : QtJambi_LibraryUtilities.dependencies.getOrDefault("Qt"+qtLibName, Collections.emptyList());
		
    	return ()->{
	    	URL entryURL = CoreUtility.createURL(urlBase+libName);
	        try(InputStream in = entryURL.openStream()){
	        	File outFileDir = outFile.getParentFile();
	        	logger.log(Level.FINEST, ()->String.format(" - copying '%1$s' to %2$s", libName, outFile.getAbsolutePath()));
	            if (!outFileDir.exists()) {
	            	logger.log(Level.FINEST, ()->String.format(" - creating directory: %1$s", outFileDir.getAbsolutePath()));
	                outFileDir.mkdirs();
	            }
	            Files.copy(in, outFile.toPath(), StandardCopyOption.REPLACE_EXISTING);
                if(executable)
                	outFile.setExecutable(true);
                
        		if(!dependencies.isEmpty()) {
        			boolean qmlExtract = isQmlLoaded.get();
        			for(Dependency dep : dependencies) {
        				List<String> replacements = new ArrayList<>();
        				String libFormat = qtLibraryName("Qt", dep.library, LIBINFIX, null, configuration, replacements, version);
        		        Iterator<String> iter = replacements.iterator();
        		        do {
        		        	String library;
        		        	if(!iter.hasNext()) {
        		        		library = libFormat;
        		        	}else {
        		        		library = String.format(libFormat, iter.next());
        		        	}
        		        	Library qmlLibrary = LibraryBundle.findLibrary("", library);
        					if(qmlLibrary!=null) {
        						if(!qmlLibrary.isLoaded() && qmlLibrary.isExtracting()) {
	        						qmlLibrary.extract();
	        						if(qmlExtract && qmlLibrary.isQmlExtracting()) {
	    		            			qmlLibrary.extractQml();
	        		            	}
        						}
        						break;
        					}
        		        }while(iter.hasNext());
        			}
    			}
			} catch (Exception e1) {
				return;
			}
		};
    }
    
    private static LibraryBundle prepareNativeDeployment(URL deploymentSpec, URL debuginfoFileURL, String jarName, Boolean shouldUnpack) throws ParserConfigurationException, SAXException, IOException{
        LibraryBundle spec = null;
        try {
			spec = LibraryBundle.read(deploymentSpec);
        } catch (java.util.zip.ZipException e1) {
		} catch (SpecificationException e1) {
			logger.warning(String.format("Unable to load native libraries from %1$s: %2$s", (jarName==null ? deploymentSpec : jarName), e1.getMessage()));
		}
        if (spec==null)
            return null;
        LibraryBundle debuginfoSpec = null;
        if(debuginfoFileURL!=null) {
	        try {
	        	debuginfoSpec = LibraryBundle.read(debuginfoFileURL);
	        	if(!debuginfoSpec.isDebuginfo()
	        			|| !spec.compiler().equals(debuginfoSpec.compiler())
	        			|| !spec.configuration().equals(debuginfoSpec.configuration())
	        			|| !spec.module().equals(debuginfoSpec.module())
	        			|| !spec.version().equals(debuginfoSpec.version()))
	        		debuginfoSpec = null;
	        } catch (java.util.zip.ZipException e1) {
			} catch (SpecificationException e1) {
				logger.warning(String.format("Unable to load native libraries from %1$s: %2$s", (jarName==null ? deploymentSpec : jarName), e1.getMessage()));
			}
        }
        boolean isDebug = spec.configuration()==LibraryBundle.Configuration.Debug;
        boolean isQtLib = spec.module()!=null && spec.module().startsWith("qt.lib.");
        boolean isQtQml = spec.module()!=null && spec.module().startsWith("qt.qml.");
        boolean isQtPlugin = spec.module()!=null && spec.module().startsWith("qt.plugin.");
        boolean isQtJambiPlugin = spec.module()!=null && spec.module().startsWith("qtjambi.plugin.");
        
        final File tmpDir;
        if((isQtLib || isQtQml || isQtPlugin) && !deleteTmpDeployment) {
    		tmpDir = new File(jambiDeploymentDir, "Qt"+spec.version());
        }else {
        	tmpDir = jambiDeploymentDir;
        }
        spec.setExtractionDir(tmpDir);
        if(debuginfoSpec!=null)
        	debuginfoSpec.setExtractionDir(tmpDir);
        File dummyFile = null;
        if(shouldUnpack == null) {
        	if(spec.module()!=null) {
        		shouldUnpack = Boolean.TRUE;
        	}else {
	            // If the dir exists and contains .dummy, sanity check the contents...
	            dummyFile = new File(tmpDir, ".dummy");
	            if(spec.module()!=null)
	            	dummyFile = new File(tmpDir, "."+spec.module()+".dummy");
	            if (dummyFile.exists()) {
	            	logger.log(Level.FINEST, " - cache directory exists");
	                shouldUnpack = Boolean.FALSE;
	            } else {
	                shouldUnpack = Boolean.TRUE;
	            }
        	}
        }

        if (shouldUnpack.booleanValue()) {
        	logger.log(Level.FINEST, ()->String.format(" - prepare library extraction for %1$s...", (jarName==null ? deploymentSpec : new File(jarName).getName())));
            
            LibVersion specVersion;
            try {
				String[] versionString = spec.version().split("\\.");
				specVersion = new LibVersion(Integer.parseInt(versionString[0]), Integer.parseInt(versionString[1]), Integer.parseInt(versionString[2]));
			} catch (Exception e2) {
				specVersion = new LibVersion(QtJambi_LibraryUtilities.qtMajorVersion, QtJambi_LibraryUtilities.qtMinorVersion, QtJambi_LibraryUtilities.qtJambiPatch);
			}
        	
            List<Library.ExtractionFunction> qmlExtractionFunctions = new LinkedList<>();
            List<Library.ExtractionFunction> utilExtractionFunctions = new LinkedList<>();
            {
            	String urlBase = deploymentSpec.toString();
                int idx = urlBase.indexOf("!/");
                if(idx>0) {
                	urlBase = urlBase.substring(0, idx+2);
                }
	            for(QPair<String,Boolean> pair : spec.files()) {
	            	final File outFile;
	            	if(pair.first.startsWith("sources/")) {
	            		if(jambiSourcesDir!=null) {
	            			outFile = new File(jambiSourcesDir, pair.first.replace('/', File.separatorChar));
	            		}else {
	            			continue;
	            		}
                	}else if(pair.first.startsWith("include/")
                			|| (operatingSystem==OperatingSystem.MacOS && pair.first.contains("/Headers/"))) {
	            		if(jambiHeadersDir!=null) {
	            			outFile = new File(jambiHeadersDir, pair.first.replace('/', File.separatorChar));
	            		}else {
	            			continue;
	            		}
                	}else {
                		outFile = new File(tmpDir, pair.first.replace('/', File.separatorChar));
                	}
	                if(!outFile.exists()) {
	                	logger.log(Level.FINEST, ()->String.format(" - adding extractor for %1$s", pair.first));
	                	Library.ExtractionFunction extractor = getLibraryExtractor(urlBase, pair.first, outFile, Boolean.TRUE.equals(pair.second), isDebug, false, specVersion.qtMajorVersion, specVersion.qtMinorVersion, specVersion.qtJambiPatch);
	                	if(pair.first.startsWith("qml/")) {
	                		qmlExtractionFunctions.add(extractor);
	                	}else {
	                		utilExtractionFunctions.add(extractor);
	                	}
	                }
	            }
            }
            if(debuginfoSpec!=null) {
            	String debuginfoUrlBase = debuginfoFileURL.toString();
                int idx = debuginfoUrlBase.indexOf("!/");
                if(idx>0) {
                	debuginfoUrlBase = debuginfoUrlBase.substring(0, idx+2);
                }
            	for(QPair<String,Boolean> pair : debuginfoSpec.files()) {
            		final File outFile;
	            	if(pair.first.startsWith("sources/")) {
	            		if(jambiSourcesDir!=null) {
	            			outFile = new File(jambiSourcesDir, pair.first.replace('/', File.separatorChar));
	            		}else {
	            			continue;
	            		}
                	}else if(pair.first.startsWith("include/")
                			|| (operatingSystem==OperatingSystem.MacOS && pair.first.contains("/Headers/"))) {
	            		if(jambiHeadersDir!=null) {
	            			outFile = new File(jambiHeadersDir, pair.first.replace('/', File.separatorChar));
	            		}else {
	            			continue;
	            		}
                	}else {
                		outFile = new File(tmpDir, pair.first.replace('/', File.separatorChar));
                	}
                    if(!outFile.exists()) {
                    	Library.ExtractionFunction extractor = getLibraryExtractor(debuginfoUrlBase, pair.first, outFile, Boolean.TRUE.equals(pair.second), isDebug, false, specVersion.qtMajorVersion, specVersion.qtMinorVersion, specVersion.qtJambiPatch);
                    	if(pair.first.startsWith("qml/")) {
                    		qmlExtractionFunctions.add(extractor);
                    	}else {
                    		utilExtractionFunctions.add(extractor);
                    	}
                    }
                }
            }
            
            Map<String,Library> entries = new TreeMap<>();
            
            ExecutorThread executor = null;
            if("qtjambi".equals(spec.module()) || "qt.lib.core".equals(spec.module())) {
            	executor = new ExecutorThread(spec.module());
            	executor.start();
            }
            if(isQtLib && "qt.lib.core".equals(spec.module()) && !spec.isDebuginfo()) {
            	switch(operatingSystem) {
				case Android:
				case IOS:
				case MacOS:
					break;
				default:
            			if((tmpDir.isDirectory() || tmpDir.mkdirs())) {
            				File binFolder = new File(tmpDir, "bin");
            				binFolder.mkdirs();
            				File confFile = new File(binFolder, "qt.conf");
            				if(!confFile.exists()) {
	            				try {
		            				try(PrintStream out = new PrintStream(new FileOutputStream(confFile), false, "UTF-8")){
				            			out.println("[Paths]");
				            			out.println("Prefix=..");
				            			out.flush();
		            				}
	        					}catch(Throwable t){
	        						confFile.delete();
	        					}
	            				if(confFile.exists()) {
		            				if(operatingSystem!=OperatingSystem.Windows) {
		            					File libFolder = new File(tmpDir, "libexec");
		                				libFolder.mkdirs();
		                				Files.copy(confFile.toPath(), new File(libFolder, "qt.conf").toPath());
		            				}
	            				}
            				}
            			}
					break;
            	}
            }
            try {
            	List<Library> libraries = spec.libraries();
            	if(debuginfoSpec!=null) {
            		libraries = new ArrayList<>(libraries);
            		libraries.addAll(debuginfoSpec.libraries());
    	        }
            	Library library = null;
            	if(isQtLib) {
            		if(!libraries.isEmpty() && !(libraries.get(0) instanceof Symlink)) {
            			libraries = new ArrayList<>(libraries);
            			library = libraries.remove(0);
            			entries.put(library.getName(), library);
            			if(operatingSystem==OperatingSystem.MacOS) {
            				if(library.getName().contains(".framework/") && !library.getName().endsWith(".framework/")) {
            					String path = library.getName();
            					int idx = path.lastIndexOf(File.separatorChar);
            					while(idx>0) {
            						path = path.substring(0, idx);
            						if(path.endsWith(".framework")) {
            							entries.put(path, library);
            							break;
            						}
            						idx = path.lastIndexOf(File.separatorChar);
            					}
            				}else if(library.getName().contains(".framework.dSYM/") && !library.getName().endsWith(".framework.dSYM/")) {
            					String path = library.getName();
            					int idx = path.lastIndexOf(File.separatorChar);
            					while(idx>0) {
            						path = path.substring(0, idx);
            						if(path.endsWith(".framework.dSYM")) {
            							entries.put(path, library);
            							break;
            						}
            						idx = path.lastIndexOf(File.separatorChar);
            					}
            				}
            			}
	            		String libName = library.getName();
	            		String urlBase = library.source().toString();
	                    int idx = urlBase.indexOf("!/");
	                    if(idx>0) {
	                    	urlBase = urlBase.substring(0, idx+2);
	                    }
	                    File outFile = new File(tmpDir, libName.replace('/', File.separatorChar));
	                    
	                    if(!outFile.exists()) {
	                		Library.ExtractionFunction extractLibrary = getLibraryExtractor(urlBase, libName, outFile, false, isDebug, isQtLib, specVersion.qtMajorVersion, specVersion.qtMinorVersion, specVersion.qtJambiPatch);
	                		if(executor!=null) {
	                    		CompletableFuture<Throwable> future = CompletableFuture.supplyAsync(()->{
	        	                    try {
		                    			extractLibrary.extract();
	                    				Library.ExtractionFunction first = null;
		                    			while(true) {
		                    				synchronized(utilExtractionFunctions) {
		                    					utilExtractionFunctions.remove(first);
			                    				if(utilExtractionFunctions.isEmpty())
			                    					break;
			                    				else
			                    					first = utilExtractionFunctions.get(0);
		                    				}
		                    				first.extract();
		                    			}
	        	                    } catch (Throwable e1) {
	        	                    	return e1;
	        	                    }
	    	                        return null;
	                        	}, executor);
	                    		library.addExtractionFunction(()->{
	                        		Throwable exn = future.get();
	                        		if(exn!=null)
	                        			throw exn;
	                        	});
	                    	}else {
	                    		library.addExtractionFunction(extractLibrary);
                    			library.addExtractionFunctions(utilExtractionFunctions);
	                    	}
	                    	List<String> qmlLibraries = library.bundle().qmlLibraries();
                    		if(!qmlLibraries.isEmpty()) {
                    			library.addQmlExtractionFunction(()->{
                    				for(String lib : qmlLibraries) {
                                    	if(lib.startsWith("lib/") || lib.startsWith("bin/"))
                                    		lib = lib.substring(4);
                    					Library qmlLibrary = LibraryBundle.findLibrary("", lib);
                    					if(qmlLibrary!=null && !qmlLibrary.isLoaded() && qmlLibrary.isExtracting()) {
                    						qmlLibrary.extract();
                    						if(qmlLibrary.isQmlExtracting()) {
                		            			qmlLibrary.extractQml();
                    		            	}
                    					}
                    				}
                    			});
                    		}
                			library.addQmlExtractionFunctions(qmlExtractionFunctions);
	                    }
            		}
            	}
	            for (Library e : libraries) {
	            	entries.put(e.getName(), e);
	            	if(operatingSystem==OperatingSystem.MacOS) {
        				if(e.getName().contains(".framework/") && !e.getName().endsWith(".framework/")) {
        					String path = e.getName();
        					int idx = path.lastIndexOf(File.separatorChar);
        					while(idx>0) {
        						path = path.substring(0, idx);
        						if(path.endsWith(".framework")) {
        							entries.put(path, e);
        							break;
        						}
        						idx = path.lastIndexOf(File.separatorChar);
        					}
        				}else if(e.getName().contains(".framework.dSYM/") && !e.getName().endsWith(".framework.dSYM/")) {
        					String path = e.getName();
        					int idx = path.lastIndexOf(File.separatorChar);
        					while(idx>0) {
        						path = path.substring(0, idx);
        						if(path.endsWith(".framework.dSYM")) {
        							entries.put(path, e);
        							break;
        						}
        						idx = path.lastIndexOf(File.separatorChar);
        					}
        				}
        			}
	            	if(!(e instanceof Symlink)) {
	            		String libName = e.getName();
	            		final File outFile;
		            	if(libName.startsWith("sources/")) {
		            		if(jambiSourcesDir!=null) {
		            			outFile = new File(jambiSourcesDir, libName.replace('/', File.separatorChar));
		            		}else {
		            			continue;
		            		}
	                	}else if(libName.startsWith("include/")
	                			|| (operatingSystem==OperatingSystem.MacOS && libName.contains("/Headers/"))) {
		            		if(jambiHeadersDir!=null) {
		            			outFile = new File(jambiHeadersDir, libName.replace('/', File.separatorChar));
		            		}else {
		            			continue;
		            		}
	                	}else {
	                		outFile = new File(tmpDir, libName.replace('/', File.separatorChar));
	                	}
	                    if(!outFile.exists()) {
	                		if(isQtJambiPlugin || isQtPlugin) {
	                			pluginLibraries.add(e);
	                		}
                			outFile.getParentFile().mkdirs();
	                		String urlBase = e.source().toString();
		                    int idx = urlBase.indexOf("!/");
		                    if(idx>0) {
		                    	urlBase = urlBase.substring(0, idx+2);
		                    }
	                		Library.ExtractionFunction extractLibrary = getLibraryExtractor(urlBase, libName, outFile, false, isDebug, 
	                				libName.startsWith("lib/libQt"+QtJambi_LibraryUtilities.qtMajorVersion)
	                				|| (libName.startsWith("lib/Qt") && !libName.startsWith("lib/QtJambi"))
	                				|| libName.startsWith("bin/Qt"+QtJambi_LibraryUtilities.qtMajorVersion), 
	                				specVersion.qtMajorVersion, specVersion.qtMinorVersion, specVersion.qtJambiPatch);
	                		if(library!=null) {
	                			logger.log(Level.FINEST, ()->String.format(" - adding extractor for %1$s", e.getName()));
	                			if(libName.startsWith("qml/")) {
	                				library.addQmlExtractionFunction(extractLibrary);
	                			}else {
	                				library.addExtractionFunction(extractLibrary);
	                			}
	                			e.addExtractionFunction(extractLibrary);
                    			e.addExtractionFunctions(utilExtractionFunctions);
	                		}else if(executor!=null
	                    			&& ((!libName.contains("QtJambiGui")
	                            			&& !libName.contains("QtJambiWidgets")
	                            			&& !(libName.startsWith("include/")
	        	                        			|| (operatingSystem==OperatingSystem.MacOS && libName.contains("/Headers/"))))
	                            			|| isQtJambiPlugin)) {
	                    		CompletableFuture<Throwable> future = CompletableFuture.supplyAsync(()->{
	        	                    try {
		                    			extractLibrary.extract();
	                    				Library.ExtractionFunction first = null;
		                    			while(true) {
		                    				synchronized(utilExtractionFunctions) {
		                    					utilExtractionFunctions.remove(first);
			                    				if(utilExtractionFunctions.isEmpty())
			                    					break;
			                    				else
			                    					first = utilExtractionFunctions.get(0);
		                    				}
		                    				first.extract();
		                    			}
	        	                    } catch (Throwable e1) {
	        	                    	return e1;
	        	                    }
	    	                        return null;
	                        	}, executor);
	                        	e.addExtractionFunction(()->{
	                        		Throwable exn = future.get();
	                        		if(exn!=null)
	                        			throw exn;
	                        	});
	                    	}else {
	                    		if(libName.startsWith("qml/")) {
	                    			qmlExtractionFunctions.add(extractLibrary);
	                    			qmlExtractionFunctions.addAll(utilExtractionFunctions);
	                			}else {
	                				e.addExtractionFunction(extractLibrary);
	                    			e.addExtractionFunctions(utilExtractionFunctions);
	                			}
	                    	}
                			e.addQmlExtractionFunctions(qmlExtractionFunctions);
		            	}
	            	}
	            }
	            List<Symlink> shiftedLinks = new ArrayList<>();
	            for (Library e : libraries) {
	            	if(e instanceof Symlink) {
	            		final File outFile;
		            	if(e.getName().startsWith("sources/")) {
		            		if(jambiSourcesDir!=null) {
		            			outFile = new File(jambiSourcesDir, e.getName().replace('/', File.separatorChar));
		            		}else {
		            			continue;
		            		}
	                	}else if(e.getName().startsWith("include/")
	                			|| (operatingSystem==OperatingSystem.MacOS && e.getName().contains("/Headers/"))) {
		            		if(jambiHeadersDir!=null) {
		            			outFile = new File(jambiHeadersDir, e.getName().replace('/', File.separatorChar));
		            		}else {
		            			continue;
		            		}
	                	}else {
	                		outFile = new File(tmpDir, e.getName().replace('/', File.separatorChar));
	                	}
		        		if(!outFile.exists()) {
		            		Symlink s = (Symlink)e;
		                    File outFileDir = outFile.getParentFile();
		                    File target = new File(tmpDir, s.getTarget().replace('/', File.separatorChar));
		                    if(target.exists()) {
			                    if (!outFileDir.exists()) {
			                    	logger.log(Level.FINEST, ()->String.format(" - creating directory: %1$s", outFileDir.getAbsolutePath()));
			                        outFileDir.mkdirs();
			                    }
			                    logger.log(Level.FINEST, ()->String.format(" - creating symbolic link %1$s pointing to %2$s", outFile.getAbsolutePath(), target.getAbsolutePath()));
		                    	Files.createSymbolicLink(outFile.toPath(), outFile.getParentFile().toPath().relativize(target.toPath()));
		                    }else {
		                    	Library.ExtractionFunction linker = ()->{
	                    			if(!outFile.exists()) {
		    		                    if (!outFileDir.exists()) {
		    		                    	logger.log(Level.FINEST, ()->String.format(" - creating directory: %1$s", outFileDir.getAbsolutePath()));
		    		                        outFileDir.mkdirs();
		    		                    }
		    		                    logger.log(Level.FINEST, ()->String.format(" - creating symbolic link %1$s pointing to %2$s", outFile.getAbsolutePath(), target.getAbsolutePath()));
				                    	Files.createSymbolicLink(outFile.toPath(), outFile.getParentFile().toPath().relativize(target.toPath()));
	                    			}
			                    	s.extractQml();
		                    	};
		                    	Library linkedEntry = entries.get(s.getTarget());
		                    	if(linkedEntry!=null) {
		                    		if(e.getName().startsWith("qml/")) {
                						linkedEntry.addQmlExtractionFunction(linker);
                						if(library!=null)
                							library.addQmlExtractionFunction(linker);
    	                			}else {
    	                				linkedEntry.addExtractionFunction(linker);
    	                				if(library!=null)
    	                					library.addExtractionFunction(linker);
    	                			}
		                    	}else if(operatingSystem==OperatingSystem.MacOS) {
	                				if(s.getTarget().contains(".framework/") && !s.getTarget().endsWith(".framework/")) {
	                					String path = s.getTarget();
	                					int idx = path.lastIndexOf(File.separatorChar);
	                					while(idx>0) {
	                						path = path.substring(0, idx);
	                						if(path.endsWith(".framework")) {
	                							linkedEntry = entries.get(path);
	                							break;
	                						}
	                						idx = path.lastIndexOf(File.separatorChar);
	                					}
	                				}else if(s.getTarget().contains(".framework.dSYM/") && !s.getTarget().endsWith(".framework.dSYM/")) {
	                					String path = s.getTarget();
	                					int idx = path.lastIndexOf(File.separatorChar);
	                					while(idx>0) {
	                						path = path.substring(0, idx);
	                						if(path.endsWith(".framework.dSYM")) {
	                							linkedEntry = entries.get(path);
	                							break;
	                						}
	                						idx = path.lastIndexOf(File.separatorChar);
	                					}
	                				}
	                				if(linkedEntry!=null) {
	                					if(e.getName().startsWith("qml/")) {
	                						linkedEntry.addQmlExtractionFunction(linker);
	                						if(library!=null)
	                							library.addQmlExtractionFunction(linker);
	    	                			}else {
	    	                				linkedEntry.addExtractionFunction(linker);
	    	                				if(library!=null)
	    	                					library.addExtractionFunction(linker);
	    	                			}
			                    	}else {
			                    		shiftedLinks.add(s);
			                    	}
	                			}else {
	                				shiftedLinks.add(s);
	                			}
		                    }
	            		}
	            	}
	            }
	            if(!shiftedLinks.isEmpty()) {
	            	List<Symlink> _shiftedLinks = new ArrayList<>();
	                while(!shiftedLinks.isEmpty()) {
	                	_shiftedLinks.clear();
	                	for (Symlink s : shiftedLinks) {
		            		final File outFile;
			            	if(s.getName().startsWith("sources/")) {
			            		if(jambiSourcesDir!=null) {
			            			outFile = new File(jambiSourcesDir, s.getName().replace('/', File.separatorChar));
			            		}else {
			            			continue;
			            		}
		                	}else if(s.getName().startsWith("include/")
		                			|| (operatingSystem==OperatingSystem.MacOS && s.getName().contains("/Headers/"))) {
			            		if(jambiHeadersDir!=null) {
			            			outFile = new File(jambiHeadersDir, s.getName().replace('/', File.separatorChar));
			            		}else {
			            			continue;
			            		}
		                	}else {
		                		outFile = new File(tmpDir, s.getName().replace('/', File.separatorChar));
		                	}
	                		if(!outFile.exists()) {
		                        File outFileDir = outFile.getParentFile();
		                        File target = new File(tmpDir, s.getTarget().replace('/', File.separatorChar));
		                        if(target.exists()) {
			                        if (!outFileDir.exists()) {
			                        	logger.log(Level.FINEST, ()->String.format(" - creating directory: %1$s", outFileDir.getAbsolutePath()));
			                            outFileDir.mkdirs();
			                        }
			                        logger.log(Level.FINEST, ()->String.format(" - creating symbolic link %1$s pointing to %2$s", outFile.getAbsolutePath(), target.getAbsolutePath()));
		                        	Files.createSymbolicLink(outFile.toPath(), outFile.getParentFile().toPath().relativize(target.toPath()));
		                        }else {
		                        	Library linkedEntry = entries.get(s.getTarget());
		                        	if(linkedEntry==null && operatingSystem==OperatingSystem.MacOS) {
		                				if(s.getTarget().contains(".framework/") && !s.getTarget().endsWith(".framework/")) {
		                					String path = s.getTarget();
		                					int idx = path.lastIndexOf(File.separatorChar);
		                					while(idx>0) {
		                						path = path.substring(0, idx);
		                						if(path.endsWith(".framework")) {
		                							linkedEntry = entries.get(path);
		                							break;
		                						}
		                						idx = path.lastIndexOf(File.separatorChar);
		                					}
		                				}else if(s.getTarget().contains(".framework.dSYM/") && !s.getTarget().endsWith(".framework.dSYM/")) {
		                					String path = s.getTarget();
		                					int idx = path.lastIndexOf(File.separatorChar);
		                					while(idx>0) {
		                						path = path.substring(0, idx);
		                						if(path.endsWith(".framework.dSYM")) {
		                							linkedEntry = entries.get(path);
		                							break;
		                						}
		                						idx = path.lastIndexOf(File.separatorChar);
		                					}
		                				}
		                			}
			                    	if(linkedEntry!=null) {
			                    		Library.ExtractionFunction linker = ()->{
			                    			if(!outFile.exists()) {
						                        if (!outFileDir.exists()) {
						                        	logger.log(Level.FINEST, ()->String.format(" - creating directory: %1$s", outFileDir.getAbsolutePath()));
						                            outFileDir.mkdirs();
						                        }
						                        logger.log(Level.FINEST, ()->String.format(" - creating symbolic link %1$s pointing to %2$s", outFile.getAbsolutePath(), target.getAbsolutePath()));
						                    	Files.createSymbolicLink(outFile.toPath(), outFile.getParentFile().toPath().relativize(target.toPath()));
			                    			}
					                    	s.extractQml();
				                    	};
			                    		if(s.getName().startsWith("qml/")) {
			                				linkedEntry.addQmlExtractionFunction(linker);
			                				if(library!=null)
			                					library.addQmlExtractionFunction(linker);
			                			}else {
			                				linkedEntry.addExtractionFunction(linker);
			                				if(library!=null)
			                					library.addExtractionFunction(linker);
			                			}
			                    	}else {
			                    		if(library!=null) {
			                    			Library.ExtractionFunction linker = ()->{
				                    			if(!outFile.exists()) {
							                        if (!outFileDir.exists()) {
							                        	logger.log(Level.FINEST, ()->String.format(" - creating directory: %1$s", outFileDir.getAbsolutePath()));
							                            outFileDir.mkdirs();
							                        }
							                        logger.log(Level.FINEST, ()->String.format(" - creating symbolic link %1$s pointing to %2$s", outFile.getAbsolutePath(), target.getAbsolutePath()));
							                    	Files.createSymbolicLink(outFile.toPath(), outFile.getParentFile().toPath().relativize(target.toPath()));
				                    			}
						                    	s.extractQml();
					                    	};
				                    		if(s.getName().startsWith("qml/")) {
				                    			library.addQmlExtractionFunction(linker);
		    	                			}else {
		    	                				library.addExtractionFunction(linker);
		    	                			}
				                    	}else {
				                    		_shiftedLinks.add(s);
				                    	}
			                    	}
		                        }
	                		}
						}
	                	if(_shiftedLinks.size()==shiftedLinks.size()) {
	                		break;
	                	}
	                	shiftedLinks.clear();
	                	shiftedLinks.addAll(_shiftedLinks);
	                }
	            }
	
	            if (dummyFile != null && !dummyFile.createNewFile()) {
	                throw new SpecificationException("Can't create dummy file in cache directory");
	            }
	            spec.setExtractionDir(tmpDir);
	            if(debuginfoSpec!=null) {
            		debuginfoSpec.setExtractionDir(tmpDir);
    	        }
            }finally {
            	if(executor!=null)
            		executor.setNoMoreExpected();
            }
        } else if(spec.extractionDir() == null) {
            String path = deploymentSpec.getPath();
            int i = path.lastIndexOf('/');  // URL path
            if(i >= 0)
                path = path.substring(0, i);
            spec.setExtractionDir(new File(path));
            if(debuginfoSpec!=null) {
        		debuginfoSpec.setExtractionDir(new File(path));
	        }
//            spec.setExtractionUrl(CoreUtility.createURL(deploymentSpec, path));
        }

        return spec;
    }

    private static boolean loadSystemLibrary(String name) {
    	logger.log(Level.FINEST, ()->String.format(" - trying to load: %1$s", name));

        File foundFile = null;
        synchronized(loadedNativeDeploymentUrls) {
	        for(LibraryBundle deploymentSpec : nativeDeployments) {
	            File f = new File(deploymentSpec.extractionDir(), name);
	            if(f.isFile()) {
	                foundFile = f;
	                break;
	            }
	        }
        }
        if(foundFile == null)
            return false;

        Runtime rt = Runtime.getRuntime();
        rt.load(foundFile.getAbsolutePath());
        logger.log(Level.FINEST, " - ok!");
        return true;
    }

    private static String qtjambiLibraryName(String prefix, String lib, boolean dontUseQtJambiFrameworks, LibraryBundle.Configuration configuration, List<String> replacements, int... version) {
        if(prefix!=null)
        	lib = prefix + lib;
    	replacements.clear();
    	if(useStaticLibs) {
    		return lib;
    	}else {
	        switch (operatingSystem) {
	        case Windows: {
	        	if(version.length>0)
	        		replacements.add(""+version[0]);
	        	replacements.add("");
	        	if (configuration == LibraryBundle.Configuration.Debug)
	                lib += "d";
	    		return lib + "%1$s.dll";  // "foobar1.dll"
	        }
			case IOS: 
	        case MacOS: 
	        	if(dontUseQtJambiFrameworks) {
	            	switch(version.length) {
	            	default:
	            		replacements.add("."+version[0]);
	            		replacements.add("."+version[0]+"."+version[1]);
	        			replacements.add("."+version[0]+"."+version[1]+"."+version[2]);
	            		replacements.add("");
	            		break;
	            	case 2:
	            		replacements.add("."+version[0]);
	            		replacements.add("."+version[0]+"."+version[1]);
	            		replacements.add("");
	            		break;
	            	case 1:
	            		replacements.add("."+version[0]);
	            	case 0: 
	            		replacements.add("");
	            		break;
	            	}
	            	if (configuration == LibraryBundle.Configuration.Debug)
	                    lib += "_debug";
	            	return "lib" + lib + "%1$s.jnilib";
	        	}else {
	            	switch(version.length) {
	            	default:
	            		replacements.add(""+version[0]);
	            	case 0: 
	            		replacements.add("6");
	            		break;
	            	}
	            	if (configuration == LibraryBundle.Configuration.Debug)
	                    lib += "_debug";
	            	return lib + ".framework/Versions/%1$s/"+lib;
	        	}
			case Android: 
				if (configuration == LibraryBundle.Configuration.Debug)
					switch(architecture) {
		        	case arm:
		        		return lib + "_debug_armeabi-v7a";
		        	case arm64:
		        		return lib + "_debug_arm64-v8a";
		        	case x86_64:
		        		return lib + "_debug_x86_64";
		        	default:
		        		return lib + "_debug_x86";
		        	}
				else
		        	switch(architecture) {
		        	case arm:
		        		return lib + "_armeabi-v7a";
		        	case arm64:
		        		return lib + "_arm64-v8a";
		        	case x86_64:
		        		return lib + "_x86_64";
		        	default:
		        		return lib + "_x86";
		        	}
			default:
				if(operatingSystem.isUnixLike()) {
					switch(version.length) {
		        	default:
		    			replacements.add("."+version[0]+"."+version[1]+"."+version[2]);
		        	case 2:
		        		replacements.add("."+version[0]+"."+version[1]);
		        	case 1:
		        		replacements.add("."+version[0]);
		        	case 0: 
		        		replacements.add("");
		        		break;
		        	}
		        	if (configuration == LibraryBundle.Configuration.Debug)
		        		return "lib" + lib + "_debug.so%1$s";
		        	else return "lib" + lib + ".so%1$s";
				}
				break;
	        }
	        throw new RuntimeException("Unreachable statement");
    	}
    }


    private static String qtLibraryName(String qtprefix, String libraryName, String libInfix, String versionStrg, LibraryBundle.Configuration configuration, List<String> replacements, int... version) {
        if(libInfix==null)
        	libInfix = "";
        if(qtprefix==null)
        	qtprefix = "";
        boolean isQt = "Qt".equals(qtprefix);
        String qtXlibName;
        if(version!=null && version.length>0 && isQt){
    		qtXlibName = qtprefix + version[0] + libraryName + libInfix;
    	}else {
    		qtXlibName = qtprefix + libraryName + libInfix;
    	}
        String prefix = qtXlibName.startsWith("lib") ? "" : "lib";
        replacements.clear();
    	if(useStaticLibs) {
    		return qtXlibName;
    	}else {
	        switch (operatingSystem) {
	        case Windows:
	        	if(version!=null && version.length>0 && !isQt){
	        		replacements.add(""+version[0]);
	        		replacements.add("");
		            return configuration == LibraryBundle.Configuration.Debug && !isMinGWBuilt()
			                ? qtXlibName + "d%1$s.dll"  // "QtFood4.dll"
			                : qtXlibName + "%1$s.dll";  // "QtFoo4.dll"
	        	}else {
		            return configuration == LibraryBundle.Configuration.Debug && !isMinGWBuilt()
		                ? qtXlibName + "d.dll"  // "QtFood4.dll"
		                : qtXlibName + ".dll";  // "QtFoo4.dll"
	        	}
	        case IOS:
	        case MacOS:
	        	if(dontUseQtFrameworks==null || !dontUseQtFrameworks) {
	        		if(version!=null && version.length>0 && version[0]<6)
	        			return String.format("%1$s%2$s.framework/Versions/%3$s/%1$s%2$s", qtprefix, libraryName, version[0]);
	        		else
	        			return String.format("%1$s%2$s.framework/Versions/A/%1$s%2$s", qtprefix, libraryName);
	        	}else {
	        		if(version!=null) {
	    	        	switch(version.length) {
	    	        	default:
	            			replacements.add("."+version[0]+"."+version[1]+"."+version[2]);
	    	        	case 2:
	    	        		replacements.add("."+version[0]+"."+version[1]);
	    	        	case 1:
	    	        		replacements.add("."+version[0]);
	    	        	case 0: 
	    	        		replacements.add("");
	    	        		break;
	    	        	}
	        		}else if(versionStrg!=null && !versionStrg.isEmpty()) {
	        			List<String> parts = new ArrayList<>();
	        			parts.addAll(Arrays.asList(versionStrg.split("\\.")));
	        			while(!parts.isEmpty()) {
	        				replacements.add("."+String.join(".", parts));
	        				parts.remove(parts.size()-1);
	        			}
	        			replacements.add("");
	        		}else {
	        			replacements.add("");
	        		}
		        	if(version!=null && version.length>1 && version[0]>=5 && version[1]>=14) {
		        		return prefix + qtXlibName + "%1$s.dylib";
		        	}else {
			            return configuration == LibraryBundle.Configuration.Debug
			                ? prefix + qtXlibName + "_debug%1$s.dylib"  // "libQtFoo_debug.4.dylib"
			                : prefix + qtXlibName + "%1$s.dylib";  // "libQtFoo.4.dylib"
		        	}
	        	}
	        case Android:
	        	switch (architecture) {
				case arm:
					return qtXlibName + "_armeabi-v7a";
				case arm64:
					return qtXlibName + "_arm64-v8a";
				case x86:
					return qtXlibName + "_x86";
				case x86_64:
					return qtXlibName + "_x86_64";
				default:
					break;
				}
	            // Linux doesn't have a dedicated "debug" library since 4.2
	            return prefix + qtXlibName + ".so";  // "libQtFoo.so.4"
			default:
				if(operatingSystem.isUnixLike()) {
					if(version!=null) {
			        	switch(version.length) {
			        	default:
		        			replacements.add("."+version[0]+"."+version[1]+"."+version[2]);
			        	case 2:
			        		replacements.add("."+version[0]+"."+version[1]);
			        	case 1:
			        		replacements.add("."+version[0]);
			        	case 0: 
			        		replacements.add("");
			        		break;
			        	}
		    		}else if(versionStrg!=null && !versionStrg.isEmpty()) {
		    			List<String> parts = new ArrayList<>();
		    			parts.addAll(Arrays.asList(versionStrg.split("\\.")));
		    			while(!parts.isEmpty()) {
		    				replacements.add("."+String.join(".", parts));
		    				parts.remove(parts.size()-1);
		    			}
		    			replacements.add("");
		    		}else {
		    			replacements.add("");
		    		}
		        	return prefix + qtXlibName + ".so%1$s";
				}
				break;
	        }
    	}
        throw new RuntimeException("Unreachable statement");
    }

    @NativeAccess
    private static boolean isNativeDeployment() {
    	synchronized(loadedNativeDeploymentUrls) {
    		return !nativeDeployments.isEmpty();
    	}
    }

    private static List<String> mergeJniLibdir(List<String> middle) {
    	if((jniLibdirBeforeList != null && !jniLibdirBeforeList.isEmpty()) 
    			|| (jniLibdirList != null && !jniLibdirList.isEmpty())) {
	    	List<String> newList = new ArrayList<String>();
	        if(jniLibdirBeforeList != null)
	            newList.addAll(jniLibdirBeforeList);
	        newList.addAll(middle);
	        if(jniLibdirList != null)
	            newList.addAll(jniLibdirList);
	        middle = newList;
    	}
    	Set<String> set = new TreeSet<>();
    	for (int i = 0; i < middle.size(); ++i) {
    		String a = middle.get(i);
			if(!set.contains(a))
				set.add(a);
			else {
				middle.remove(i);
				--i;
			}
		}
        return middle;
    }
    
    static void loadSystemLibraries() {
        if (systemLibrariesList != null) {
            for (String s : systemLibrariesList) {
                LibraryUtility.loadSystemLibrary(s);
            }
        }
    }

    static File qtJambiLibraryPath() {
    	synchronized(loadedNativeDeploymentUrls) {
    		return qtJambiLibraryPath;
    	}
	}

	static File qtLibraryPath() {
		return qtLibraryPath;
	}

	static String osArchName() {
		return osArchName;
	}
	
	private static int findSequence(int offset, byte[] array, byte[] sequence) {
	    for (int i = offset; i < array.length - sequence.length + 1; ++i) {
	        boolean found = true;
	        for (int j = 0; j < sequence.length; j++) {
	            if (array[i + j] != sequence[j]) {
	                found = false;
	                break;
	            }
	        }
	        if (found) {
	            return i;
	        }
	    }
	    return -1;
	}
	
	private static int reverseFindSequence(int offset, byte[] array, byte[] sequence) {
	    for (int i = offset-sequence.length; i >= 0; --i) {
	        boolean found = true;
	        for (int j = 0; j < sequence.length; j++) {
	            if (array[i + j] != sequence[j]) {
	                found = false;
	                break;
	            }
	        }
	        if (found) {
	            return i;
	        }
	    }
	    return -1;
	}
	
	private static List<String> readVersion(java.io.File file, final byte[] prefix) {
		switch(LibraryUtility.operatingSystem) {
        case MacOS:
        	if(file.isDirectory() && file.getName().endsWith(".framework")) {
        		String name = file.getName().substring(0, file.getName().length()-10);
        		File library = new File(file, name);
        		if(!library.exists()) {
        			library = new File(file, "Versions/Current/"+name);
        			if(!library.exists()) {
            			library = new File(file, "Versions/A/"+name);
            			if(!library.exists()) {
                			library = new File(file, "Versions/"+QtJambi_LibraryUtilities.qtMajorVersion+"/"+name);
                		}
            		}
        		}
        		if(library.exists()) {
        			file = library;
        		}
        	}
			break;
		default:
			break;
		}
		final boolean canUniversalBuild = LibraryUtility.operatingSystem==OperatingSystem.MacOS
				|| LibraryUtility.operatingSystem==OperatingSystem.IOS;
		final List<String> qtLibraryBuilds = new ArrayList<>();
		final Set<Integer> indexes = new HashSet<>();
		final byte[] buildBy = "build; by".getBytes(StandardCharsets.US_ASCII);
		if(file.isFile()) try {
			byte[] data = Files.readAllBytes(file.toPath());
			int idx = 0;
			while(true) {
				idx = findSequence(idx, data, buildBy);
				if(idx<0)
					break;
				idx = reverseFindSequence(idx, data, prefix);
				if(idx<0 || indexes.contains(idx))
					break;
				indexes.add(idx);
				ByteArrayOutputStream os = new ByteArrayOutputStream();
				int parenCount = 0;
				for(int i=idx; i<data.length; ++i){
					byte c = data[i];
					switch(c) {
					case (byte)'(':
						++parenCount;
						os.write(c);
						break;
					case (byte)')':
						--parenCount;
						os.write(c);
						if(parenCount==0)
							i = data.length;
						break;
					case (byte)'<':
					case (byte)'>':
					case (byte)'-':
					case (byte)'_':
					case (byte)' ':
					case (byte)';':
					case (byte)':':
					case (byte)'/':
					case (byte)'.':
						os.write(c);
						break;
					default:
						if(Character.isLetterOrDigit(c)) {
							os.write(c);
						}else {
							i = data.length;
						}
						break;
					}
				}
				String qtLibraryBuild = new String(os.toByteArray(), StandardCharsets.US_ASCII);
				if(qtLibraryBuild.contains("build; by")) {
					qtLibraryBuilds.add(qtLibraryBuild);
					if(!canUniversalBuild)
						break;
				}
				idx += prefix.length+1;
			}
		}catch(Throwable t2){}
		return qtLibraryBuilds;
	}

	private static void analyzeUnsatisfiedLinkError(UnsatisfiedLinkError t, java.io.File coreLib, Availability qtlibAvailability, Availability availability) {
		final String architecture;
		if(LibraryUtility.architecture==Architecture.x86) {
			architecture = "i386-"+ByteOrder.nativeOrder().toString().toLowerCase();
		}else {
			architecture = LibraryUtility.architecture.name()+"-"+ByteOrder.nativeOrder().toString().toLowerCase();
		}
		final List<String> qtLibraryBuilds = readVersion(coreLib, "Qt ".getBytes(StandardCharsets.US_ASCII));
		List<String> qtjambiLibraryBuilds = Collections.emptyList();
		if(availability==null){
			 // error when loading QtCore:
			String qtLibraryBuild = null;
			switch(qtLibraryBuilds.size()) {
			case 0: throw t;
			case 1:
				qtLibraryBuild = qtLibraryBuilds.get(0);
				if(qtLibraryBuild.contains(architecture)) {
					throw t;
				}
				throw new LinkageError("Cannot load " + qtLibraryBuild + " on " + architecture + " architecture.", t);
			default:
				for(String s : qtLibraryBuilds) {
					if(s.contains(architecture)) {
						qtLibraryBuild = s;
						break;
					}
				}
				if(qtLibraryBuild==null) {
					throw new LinkageError("Cannot load one of " + qtLibraryBuilds + " on " + architecture + " architecture.", t);
				}else {
					throw new LinkageError("Cannot load " + qtLibraryBuild + " on " + architecture + " architecture.", t);
				}
			}
		}else {
			if(availability.file!=null) {
				qtjambiLibraryBuilds = readVersion(availability.file, "QtJambi ".getBytes(StandardCharsets.US_ASCII));
			}
			if(!qtLibraryBuilds.isEmpty() && !qtjambiLibraryBuilds.isEmpty()) {
				String qtLibraryBuild = qtLibraryBuilds.get(0);
				String qtjambiLibraryBuild = qtjambiLibraryBuilds.get(0);
				String qtcoreFile = coreLib!=null ? coreLib.getAbsolutePath() : "";
				String qtjambiFile = availability.file!=null ? availability.file.getAbsolutePath() : "";
				if(coreLib!=null && qtlibAvailability!=null && qtlibAvailability.entry!=null) {
					qtcoreFile += " extracted from " + qtlibAvailability.entry.bundle().origin();
				}
				if(!qtcoreFile.isEmpty()) {
					qtcoreFile = " (" + qtcoreFile + ")";
				}
				if(availability.file!=null && availability!=null && availability.entry!=null) {
					qtjambiFile += " extracted from " + availability.entry.bundle().origin();
				}
				if(!qtjambiFile.isEmpty()) {
					qtjambiFile = " (" + qtjambiFile + ")";
				}
				switch(LibraryUtility.operatingSystem) {
				case Windows:
					if(LibraryUtility.architecture==Architecture.x86_64 && qtjambiLibraryBuild.contains(architecture)) {
						if(qtjambiLibraryBuild.contains("MSVC") && (qtLibraryBuild.contains("GCC") || qtLibraryBuild.contains("Clang"))) {
							throw new LinkageError("Cannot run " + qtjambiLibraryBuild + qtjambiFile + " with " + qtLibraryBuild + qtcoreFile + ". Please install and use Qt (MSVC x64) instead.", t);
						}else if(qtjambiLibraryBuild.contains("GCC") && (qtLibraryBuild.contains("MSVC") || qtLibraryBuild.contains("Clang"))){
							throw new LinkageError("Cannot run " + qtjambiLibraryBuild + qtjambiFile + " with " + qtLibraryBuild + qtcoreFile + ". Please install and use Qt (MinGW x64) instead.", t);
						}else if(qtjambiLibraryBuild.contains("Clang") && (qtLibraryBuild.contains("GCC") || qtLibraryBuild.contains("MSVC"))){
							throw new LinkageError("Cannot run " + qtjambiLibraryBuild + qtjambiFile + " with " + qtLibraryBuild + qtcoreFile + ". Please install and use Qt (LLVM-MinGW x64) instead.", t);
						}
					}else if(LibraryUtility.architecture==Architecture.x86 && qtjambiLibraryBuild.contains(architecture)) {
						if(qtjambiLibraryBuild.contains("MSVC") && qtLibraryBuild.contains("GCC")) {
							throw new LinkageError("Cannot run " + qtjambiLibraryBuild + qtjambiFile + " with " + qtLibraryBuild + qtcoreFile + ". Please install and use Qt (MSVC x86) instead.", t);
						}else if(qtjambiLibraryBuild.contains("GCC") && qtLibraryBuild.contains("MSVC")){
							throw new LinkageError("Cannot run " + qtjambiLibraryBuild + qtjambiFile + " with " + qtLibraryBuild + qtcoreFile + ". Please install and use Qt (MinGW x86) instead.", t);
						}
					}
				default:
					if(!qtLibraryBuild.contains(String.format("Qt %1$s.%2$s.", QtJambi_LibraryUtilities.qtMajorVersion, QtJambi_LibraryUtilities.qtMinorVersion))) {
						if(coreLib!=null)
							throw new LinkageError(String.format("Mismatching versions: Cannot run Qtjambi %1$s.%2$s with %3$s (%4%s).", QtJambi_LibraryUtilities.qtMajorVersion, QtJambi_LibraryUtilities.qtMinorVersion, qtLibraryBuild, coreLib.getAbsolutePath()), t);
						else
							throw new LinkageError(String.format("Mismatching versions: Cannot run Qtjambi %1$s.%2$s with %3$s.", QtJambi_LibraryUtilities.qtMajorVersion, QtJambi_LibraryUtilities.qtMinorVersion, qtLibraryBuild), t);
					}else {
						String message = "";
						if(LibraryUtility.operatingSystem.isUnixLike()) {
							if(t.getMessage()!=null && t.getMessage().contains("_PRIVATE_API' not found")) {
								message = "Incompatible Qt builds: ";
							}
						}
						if(qtLibraryBuilds.size()==1) {
							if(qtjambiLibraryBuilds.size()==1) {
								throw new LinkageError(message+"Cannot run " + qtjambiLibraryBuild + qtjambiFile + " with " + qtLibraryBuild + qtcoreFile + ".", t);
							}else {
								throw new LinkageError(message+"Cannot run one of " + qtjambiLibraryBuilds + qtjambiFile + " with " + qtLibraryBuild + qtcoreFile + ".", t);
							}
						}else {
							if(qtjambiLibraryBuilds.size()==1) {
								throw new LinkageError(message+"Cannot run " + qtjambiLibraryBuild + qtjambiFile + " with one of " + qtLibraryBuilds + qtcoreFile + ".", t);
							}else {
								throw new LinkageError(message+"Cannot run one of " + qtjambiLibraryBuilds + qtjambiFile + " with one of " + qtLibraryBuilds + qtcoreFile + ".", t);
							}
						}
					}
				}
			}else {
				switch(LibraryUtility.operatingSystem) {
		        case MacOS:
		            if(coreLib!=null) {
		                java.io.File prl = new java.io.File(coreLib.getParentFile(), "Resources/QtCore.prl");
		                if(prl.exists()) {
		                    Properties prlProp = new Properties();
		                    try(java.io.FileInputStream inStream = new java.io.FileInputStream(prl)){
		                        prlProp.load(inStream);
		                    } catch(Throwable t2) {}
		                    String version = prlProp.getProperty("QMAKE_PRL_VERSION", "");
		                    if(!version.isEmpty()) {
		                        if(!version.startsWith(QtJambi_LibraryUtilities.qtMajorVersion + "." + QtJambi_LibraryUtilities.qtMinorVersion + ".")) {
		                            throw new LinkageError("Cannot combine QtJambi " + QtJambi_LibraryUtilities.qtMajorVersion + "." + QtJambi_LibraryUtilities.qtMinorVersion + " with Qt " + version + ".", t);
		                        }
		                    }
		                }
		            }
		            break;
		        case Windows:
		            if(coreLib!=null) {
		                java.io.File prl = new java.io.File(coreLib.getParentFile(), "Qt"+QtJambi_LibraryUtilities.qtMajorVersion+"Core.prl");
		                if(!prl.exists()) {
		                    prl = new java.io.File(coreLib.getParentFile().getParentFile(), "lib\\Qt"+QtJambi_LibraryUtilities.qtMajorVersion+"Core.prl");
		                }
		                if(prl.exists()) {
		                    Properties prlProp = new Properties();
		                    try(java.io.FileInputStream inStream = new java.io.FileInputStream(prl)){
		                        prlProp.load(inStream);
		                    } catch(Throwable t2) {}
		                    String version = prlProp.getProperty("QMAKE_PRL_VERSION", "");
		                    if(!version.isEmpty()) {
		                        if(!version.startsWith(QtJambi_LibraryUtilities.qtMajorVersion + "." + QtJambi_LibraryUtilities.qtMinorVersion + ".")) {
		                            throw new LinkageError("Cannot combine QtJambi " + QtJambi_LibraryUtilities.qtMajorVersion + "." + QtJambi_LibraryUtilities.qtMinorVersion + " with Qt " + version + ".", t);
		                        }
		                    }
		                }
		                if(new java.io.File(coreLib.getParentFile(), "libc++.dll").exists() || new java.io.File(coreLib.getParentFile(), "libstdc++-6.dll").exists() || LibraryUtility.isMinGWBuilt()) {
		                    throw new LinkageError("Cannot combine msvc-based QtJambi with mingw-based Qt library. Please install and use Qt (MSVC x64) instead.", t);
		                }else {
		                	switch(osArchName) {
		                	case "windows-llvm-mingw-x64":
		                		throw new LinkageError("Cannot combine mingw-based QtJambi with msvc-based Qt library. Please install and use Qt (LLVM MinGW x64) instead.", t);
		                	case "windows-mingw-x64":
		                		throw new LinkageError("Cannot combine mingw-based QtJambi with msvc-based Qt library. Please install and use Qt (MinGW x64) instead.", t);
		                	case "windows-x64":
		                		throw new LinkageError("Cannot combine msvc-based QtJambi with mingw-based Qt library. Please install and use Qt (MSVC x64) instead.", t);
		                	}
		                }
		            }
		            break;
		        default:
		            if(coreLib!=null) {
		                java.io.File prl = new java.io.File(coreLib.getParentFile(), "Qt"+QtJambi_LibraryUtilities.qtMajorVersion+"Core.prl");
		                if(prl.exists()) {
		                    Properties prlProp = new Properties();
		                    try(java.io.FileInputStream inStream = new java.io.FileInputStream(prl)){
		                        prlProp.load(inStream);
		                    } catch(Throwable t2) {}
		                    String version = prlProp.getProperty("QMAKE_PRL_VERSION", "");
		                    if(!version.isEmpty()) {
		                        if(!version.startsWith(QtJambi_LibraryUtilities.qtMajorVersion + "." + QtJambi_LibraryUtilities.qtMinorVersion + ".")) {
		                            throw new LinkageError("Cannot combine QtJambi " + QtJambi_LibraryUtilities.qtMajorVersion + "." + QtJambi_LibraryUtilities.qtMinorVersion + " with Qt " + version + ".", t);
		                        }
		                    }
		                }
		            }
		            break;
		        }
			}
	        throw t;
		}
	}
}
