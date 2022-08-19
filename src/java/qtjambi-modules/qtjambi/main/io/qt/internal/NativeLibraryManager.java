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

package io.qt.internal;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.net.MalformedURLException;
import java.net.URISyntaxException;
import java.net.URL;
import java.nio.file.Files;
import java.nio.file.StandardCopyOption;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.Enumeration;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.SortedMap;
import java.util.TreeMap;
import java.util.TreeSet;
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.Executor;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.function.BiConsumer;
import java.util.logging.Level;
import java.util.logging.Logger;
import java.util.prefs.Preferences;
import java.util.zip.ZipException;

import javax.xml.parsers.ParserConfigurationException;
import javax.xml.parsers.SAXParser;
import javax.xml.parsers.SAXParserFactory;

import org.xml.sax.SAXException;
import org.xml.sax.helpers.DefaultHandler;

import io.qt.NativeAccess;
import io.qt.QLibraryNotFoundError;
import io.qt.QLibraryNotLoadedError;
import io.qt.QNoSuchSlotException;
import io.qt.QtUninvokable;
import io.qt.QtUtilities.LibraryRequirementMode;
import io.qt.core.QDeclarableSignals;
import io.qt.core.QPair;

/**
 * The NativeLibraryManager class is responsible for handling native
 * libraries in Qt Jambi. Native libraries can be loaded either
 * directly from the file system using
 * <code>-Djava.library.path</code> or indirectly JAR-file that
 * contain a deployment descriptor. For normal deployment, the JAR
 * approach is recommended.
 *
 * Loading libraries is done by calling the methods
 * <code>loadQtLibrary</code> and <code>loadLibrary</code>.
 *
 * When the indirect .jar file approach is taken, the .jar file will
 * be opened and the native library manager will search for a file
 * named "qtjambi-deployment.xml". This file contains a list of native
 * libraries that should unpacked to a temporary directory and loaded,
 * either right away or at a later time. There are three types of
 * libraries.
 *
 * <ul>
 *
 *   <li> System libraries; such as the system runtime
 *   libraries. These libraries are usually loaded automatically by
 *   the native library loader.
 *
 *   <li> Normal libraries; such as the QtCore and
 *   io_qt_core libraries. These are loaded at runtime on
 *   demand.
 *
 *   <li> Plugins; such as qjpeg. These are never loaded explicitly by
 *   the native library manager, but are unpacked into the temporary
 *   folder so that Qt can find and load them from the file system.
 *
 * </ul>
 *
 * There are three possible deployment scenarios. The simplest and
 * most straightforward approach is when deploying a Pure Java
 * application based on Qt Jambi. In this case the prebuilt binaries
 * from the binary package can just be deployed as part of the
 * classpath and the rest will solve itself.
 *
 * When deploying a Qt Jambi application that is using native code
 * other than Qt Jambi, we recommend building a new .jar file with a
 * custom qtjambi-deployment.xml which contais the Qt Jambi libraries
 * and the custom native libraries. Deployment can then be done by
 * making sure that this new .jar file is available in the classpath.
 *
 * The final option for deployment is when users have a C++
 * application which starts and makes use of Qt Jambi. In this case we
 * suggest that all dependent libraries are available in the file
 * system and via <code>-Djava.library.path</code>
 *
 * To get runtime information about how library loading works, specify
 * the <code>-Dio.qt.verbose-loading</code> system property
 * to the Virtual Machine. It possible to specify that the native
 * library manager should load debug versions of libraries as
 * well. This is done by specifying the system property
 * <code>-Dio.qt.debug=debug</code>
 *
 */
final class NativeLibraryManager {
	
	private NativeLibraryManager() { throw new RuntimeException();}
	
	final static class Dependency {
        Dependency(String library) {
            this(library, LibraryRequirementMode.Mandatory);
        }
        Dependency(String library, LibraryRequirementMode mode, String... platforms) {
            this.library = library;
            this.mode = mode;
            this.platforms = java.util.Arrays.asList(platforms);
        }
        final String library;
        final LibraryRequirementMode mode;
        final java.util.List<String> platforms;
    }

    private static final boolean VERBOSE_LOADING = Boolean.getBoolean("io.qt.verbose-loading") || Boolean.getBoolean("qtjambi.verbose-loading");

    public static final String ICU_VERSION;
    public static final String LIBINFIX;
    // We use a List<> to make the collection read-only an array would not be suitable
    private static final List<String> systemLibrariesList;
    private static final List<String> jniLibdirBeforeList;
    private static final List<String> jniLibdirList;
    private static boolean isMinGWBuilt = false;
    
	static final OperatingSystem operatingSystem = decideOperatingSystem();
	static final Architecture architecture = decideArchitecture();
    private static final String osArchName;
    
    private static Configuration configuration = null;
    private static String qtJambiLibraryPath = null;
    private static String qtLibraryPath = null;
	private static Boolean dontUseFrameworks = operatingSystem==OperatingSystem.MacOSX ? null : Boolean.FALSE;
    
    private static boolean dontSearchDeploymentSpec = false;
    private static final boolean noDeploymentSpec = Boolean.getBoolean("io.qt.no-deployment-spec") 
    		|| Boolean.getBoolean("qtjambi.no-deployment-spec")
    		|| operatingSystem==OperatingSystem.Android;
    private static final Set<URL> loadedDeploymentSpecUrls = Collections.synchronizedSet(new HashSet<>());
    private static final List<DeploymentSpec> deploymentSpec = Collections.synchronizedList(new ArrayList<>());

    private static final Map<String, LibraryEntry> libraryMap = Collections.synchronizedMap(new HashMap<String, LibraryEntry>());
    static final Reporter reporter = new Reporter();
    private static final File jambiDeploymentDir;
    private static final File jambiJarDir;
    private static final List<LibraryEntry> pluginLibraries = Collections.synchronizedList(new ArrayList<>());

    public static final String K_SUNOS_X86 = "sunos-x86";
    public static final String K_SUNOS_X64 = "sunos-x64";

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

    public static final String K_FREEBSD_X86 = "freebsd-x86";
    public static final String K_FREEBSD_X64 = "freebsd-x64";
    private static final boolean deleteTmpDeployment;
    
    private static final Map<String,List<String>> javaLibraryPaths = new HashMap<>();
    private static final Map<String,List<String>> ldLibraryPaths = new HashMap<>();
    private static final Map<String,List<String>> javaLibraryPathOverrides = new HashMap<>();
    
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
    
    private static final Set<String> loadedLibraries = new TreeSet<>();
    
    static {
        String tmpICUVERSION_STRING = null;
        String tmpINFIX_STRING = null;
        List<String> tmpSystemLibrariesList = null;
        List<String> tmpJniLibdirBeforeList = null;
        List<String> tmpJniLibdirList = null;
        try {
            tmpINFIX_STRING = QtJambi_LibraryUtilities.properties.getProperty("qtjambi.libinfix");
            tmpICUVERSION_STRING = QtJambi_LibraryUtilities.properties.getProperty("qtjambi.icu.version", "56.1");
            
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
            tmpSystemLibrariesList = new ArrayList<String>();
            for (String v : tmpSystemLibrariesMap.values())
                tmpSystemLibrariesList.add(v);

            if (tmpSystemLibrariesList.size() > 0)
                tmpSystemLibrariesList = resolveSystemLibraries(tmpSystemLibrariesList);
            else
                tmpSystemLibrariesList = null;

            tmpJniLibdirBeforeList = new ArrayList<String>();
            for (String v : tmpJniLibdirBeforeMap.values())
                tmpJniLibdirBeforeList.add(v);

            if (tmpJniLibdirBeforeList.size() > 0)
                tmpJniLibdirBeforeList = Collections.unmodifiableList(tmpJniLibdirBeforeList);
            else
                tmpJniLibdirBeforeList = null;

            tmpJniLibdirList = new ArrayList<String>();
            for (String v : tmpJniLibdirMap.values())
                tmpJniLibdirList.add(v);

            if (tmpJniLibdirList.size() > 0)
                tmpJniLibdirList = Collections.unmodifiableList(tmpJniLibdirList);
            else
                tmpJniLibdirList = null;
        } catch(Throwable e) {
            java.util.logging.Logger.getLogger("io.qt.internal").log(java.util.logging.Level.SEVERE, "", e);
        }finally {
        	ICU_VERSION = tmpICUVERSION_STRING;
            systemLibrariesList = tmpSystemLibrariesList!=null ? Collections.unmodifiableList(tmpSystemLibrariesList) : Collections.emptyList();
            jniLibdirBeforeList = tmpJniLibdirBeforeList!=null ? Collections.unmodifiableList(tmpJniLibdirBeforeList) : Collections.emptyList();
            jniLibdirList = tmpJniLibdirList!=null ? Collections.unmodifiableList(tmpJniLibdirList) : Collections.emptyList();
            LIBINFIX = tmpINFIX_STRING;
        }

        switch (operatingSystem) {
        case Windows:
        	switch(architecture) {
        	case arm:
        		osArchName = K_WIN_ARM32; break;
        	case arm64:
        		osArchName = K_WIN_ARM64; break;
        	case x86_64:
        		osArchName = K_WIN_X64; break;
        	default:
        		osArchName = K_WIN_X86; break;
        	}
            break;
        case Linux:
        	switch(architecture) {
        	case arm:
        		osArchName = K_LINUX_ARM32; break;
        	case arm64:
        		osArchName = K_LINUX_ARM64; break;
        	case x86_64:
        		osArchName = K_LINUX_X64; break;
        	default:
        		osArchName = K_LINUX_X86; break;            	
        	}
            break;
        case Android:
        	switch(architecture) {
        	case arm:
        		osArchName = K_ANDROID_ARM32; break;
        	case arm64:
        		osArchName = K_ANDROID_ARM64; break;
        	case x86_64:
        		osArchName = K_ANDROID_X64; break;
        	default:
        		osArchName = K_ANDROID_X86; break;
        	}
            break;
        case MacOSX:
            osArchName = "macos";
            break;
        default:
            osArchName = "unknown";
            break;
        }
        
        reporter.setReportEnabled(VERBOSE_LOADING);
        if(operatingSystem!=OperatingSystem.Android) {
	        try{
	        	Preferences preferences = Preferences.userNodeForPackage(NativeLibraryManager.class);
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
                
        boolean loadQtJambiFromLibraryPath = noDeploymentSpec;
        boolean loadQtFromLibraryPath = loadQtJambiFromLibraryPath;
        @SuppressWarnings("unused")
		File _qtDeploymentDir = null;
        File _jambiDeploymentDir = null;
        if(!loadQtJambiFromLibraryPath){
            List<String> libraryPaths = new ArrayList<>();
            if (System.getProperties().contains("io.qt.library-path-override")) {
            	libraryPaths.addAll(javaLibraryPathOverrides.computeIfAbsent(System.getProperty("io.qt.library-path-override"), NativeLibraryManager::splitPath));
        	} else {
        		libraryPaths.addAll(javaLibraryPaths.computeIfAbsent(System.getProperty("java.library.path"), NativeLibraryManager::splitPath));
    		}
            libraryPaths = mergeJniLibdir(libraryPaths);

	    	List<String> replacements = new ArrayList<>();
	    	configuration = decideConfiguration();
	    	if(configuration!=null) {
	    		String libFormat = qtjambiLibraryName(null, "QtJambi", configuration, replacements, QtJambi_LibraryUtilities.qtMajorVersion, QtJambi_LibraryUtilities.qtMinorVersion, QtJambi_LibraryUtilities.qtJambiPatch);
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
			        	if (f.exists()) {
			        		loadQtJambiFromLibraryPath = true;
			        		_jambiDeploymentDir = f.getParentFile();
			        		break loop1;
			        	}
		        	}while(iter.hasNext());
		    	}
	    	}else {
	    		replacements.clear();
	    		String libFormat = qtjambiLibraryName(null, "QtJambi", Configuration.Release, replacements, QtJambi_LibraryUtilities.qtMajorVersion, QtJambi_LibraryUtilities.qtMinorVersion, QtJambi_LibraryUtilities.qtJambiPatch);
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
			        	if (f.exists()) {
			        		loadQtJambiFromLibraryPath = true;
			        		_jambiDeploymentDir = f.getParentFile();
			        		configuration = Configuration.Release;
			        		break loop1;
			        	}
		    		}while(iter.hasNext());
		    	}
	    		if(!loadQtJambiFromLibraryPath) {
	    			replacements.clear();
		    		libFormat = qtjambiLibraryName(null, "QtJambi", Configuration.Debug, replacements, QtJambi_LibraryUtilities.qtMajorVersion, QtJambi_LibraryUtilities.qtMinorVersion, QtJambi_LibraryUtilities.qtJambiPatch);
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
				        	if (f.exists()) {
				        		loadQtJambiFromLibraryPath = true;
				        		_jambiDeploymentDir = f.getParentFile();
				        		configuration = Configuration.Debug;
				        		break loop1;
				        	}
			    		}while(iter.hasNext());
			    	}
	    		}
	    		if(configuration==null)
	    			configuration = Configuration.Release;
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
	    			switch(operatingSystem) {
	    			case Linux:
	    				// libQt5Core.so.5 could point to libQt5Core.so.5.x with x < qtMinorVersion
	    				if(f.exists() && (
	    						lib.endsWith(".so."+QtJambi_LibraryUtilities.qtMajorVersion)
	    						|| lib.endsWith(".so")
	    						) && Files.isSymbolicLink(f.toPath())) {
	    					try {
								File link = Files.readSymbolicLink(f.toPath()).toFile();
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
	    				break;
	    				default:
	    					break;
	    			}
		        	if (f!=null && f.exists()) {
		        		loadQtFromLibraryPath = true;
		        		_qtDeploymentDir = f.getParentFile();
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
					case Linux:
						tmpDir = new File(System.getProperty("user.home"), ".local/share/QtJambi");
						break;
					case MacOSX:
						tmpDir = new File(System.getProperty("user.home"), "Library/Application Support/QtJambi");
						break;
					default:
						break;
	        		}
	        	}else if("common".equalsIgnoreCase(deploymentdir)) {
	        		switch (operatingSystem) {
	                case Windows:
	                	tmpDir = new File("C:\\ProgramData\\QtJambi");
	                	break;
					case Linux:
						tmpDir = new File("/usr/local/share/QtJambi");
						break;
					case MacOSX:
						tmpDir = new File("/Library/Application Support/QtJambi");
						break;
					default:
						break;
	        		}
	        	}else {
		        	File deploymentDir = new File(deploymentdir);
		        	if(!deploymentDir.isAbsolute()) {
		        		deploymentDir = new File(System.getProperty("user.home"), deploymentdir);
		        	}
	        		if(deploymentDir.getName().toLowerCase().startsWith("qtjambi"))
        				dirprefix = "QtJambi";
	        		tmpDir = deploymentDir;
	        	}
        		jambiDeploymentDir = new File(tmpDir, dirprefix + QtJambi_LibraryUtilities.qtMajorVersion + "." + QtJambi_LibraryUtilities.qtMinorVersion + "." + QtJambi_LibraryUtilities.qtJambiPatch);
	        }else {
		        if(keepDeployment) {
		        	jambiDeploymentDir = new File(tmpDir, "QtJambi" + QtJambi_LibraryUtilities.qtMajorVersion + "." + QtJambi_LibraryUtilities.qtMinorVersion + "." + QtJambi_LibraryUtilities.qtJambiPatch + "_" + System.getProperty("user.name"));
		        }else {
		        	jambiDeploymentDir = new File(tmpDir, "QtJambi" + QtJambi_LibraryUtilities.qtMajorVersion + "." + QtJambi_LibraryUtilities.qtMinorVersion + "." + QtJambi_LibraryUtilities.qtJambiPatch + "_" + System.getProperty("user.name") + "_" + RetroHelper.processName());
		        }
	        }
	        deleteTmpDeployment = !keepDeployment;
	        if(deleteTmpDeployment)
	        	Logger.getLogger("io.qt.internal").log(Level.FINEST, ()->"Requires deletion of tmp deployment directory "+jambiDeploymentDir.getAbsolutePath()+" during pogram termination.");
	        

	    	
	    	{
	    		File _jambiJarDir = null;
	        	String classURL = ""+NativeLibraryManager.class.getResource("NativeLibraryManager.class");
	        	int index;
		    	if(classURL.startsWith("jar:file:") && (index = classURL.indexOf("!/"))>0) {
		    		String jarFileURL = classURL.substring(4, index);
		    		File jarFile = null;
		    		try {
						jarFile = new File(new URL(jarFileURL).toURI());
					} catch (URISyntaxException | MalformedURLException e) {
					}
		    		if(jarFile!=null && jarFile.exists()) {
		    			_jambiJarDir = jarFile.getParentFile();
		    		}
		    	}
		    	jambiJarDir = _jambiJarDir;
	    	}
	        

    		ClassLoader loader = classLoader();
            // Multiple descriptors maybe found
            Enumeration<URL> specsFound = Collections.emptyEnumeration();
            try {
				specsFound = loader.getResources("qtjambi-deployment.xml");
			} catch (IOException e) {
				Logger.getLogger("io.qt.internal").log(Level.WARNING, "", e);
			}
            // FIXME: Want searchForDescriptors/parse/resolve/unpack/load phases separated
            
            dontSearchDeploymentSpec = specsFound.hasMoreElements();
            
            if(!dontSearchDeploymentSpec && jambiJarDir!=null) {
            	List<URL> foundURLs = new ArrayList<>();
            	boolean isDebug = false;
            	if(!loadQtJambiFromLibraryPath) {
	            	if("debug".equals(System.getProperty("io.qt.debug"))) {
		    			File nativeFile = new File(jambiJarDir, String.format("qtjambi-native-%1$s-debug-%2$s.%3$s.%4$s.jar", osArchName, QtJambi_LibraryUtilities.qtMajorVersion, QtJambi_LibraryUtilities.qtMinorVersion, QtJambi_LibraryUtilities.qtJambiPatch));
		    			if(nativeFile.exists()) {
		    				isDebug = true;
		    				try {
		    					foundURLs.add(new URL("jar:"+nativeFile.toURI()+"!/qtjambi-deployment.xml"));
							} catch (IOException e) {
							}
		    			}
	            	}else {
	            		File nativeFile = new File(jambiJarDir, String.format("qtjambi-native-%1$s-%2$s.%3$s.%4$s.jar", osArchName, QtJambi_LibraryUtilities.qtMajorVersion, QtJambi_LibraryUtilities.qtMinorVersion, QtJambi_LibraryUtilities.qtJambiPatch));
		    			if(nativeFile.exists()) {
		    				try {
		    					foundURLs.add(new URL("jar:"+nativeFile.toURI()+"!/qtjambi-deployment.xml"));
							} catch (IOException e) {
							}
		    			}else {
		    				nativeFile = new File(jambiJarDir, String.format("qtjambi-native-%1$s-debug-%2$s.%3$s.%4$s.jar", osArchName, QtJambi_LibraryUtilities.qtMajorVersion, QtJambi_LibraryUtilities.qtMinorVersion, QtJambi_LibraryUtilities.qtJambiPatch));
			    			if(nativeFile.exists()) {
			    				isDebug = true;
			    				try {
			    					foundURLs.add(new URL("jar:"+nativeFile.toURI()+"!/qtjambi-deployment.xml"));
								} catch (IOException e) {
								}
			    			}
		    			}
	            	}
	            	String infix;
	        		if(isDebug) {
	        			infix = String.format("-native-%1$s-debug-%2$s.%3$s.", osArchName, QtJambi_LibraryUtilities.qtMajorVersion, QtJambi_LibraryUtilities.qtMinorVersion);
	        		}else {
	        			infix = String.format("-native-%1$s-%2$s.%3$s.", osArchName, QtJambi_LibraryUtilities.qtMajorVersion, QtJambi_LibraryUtilities.qtMinorVersion);
	        		}
	            	for(String f : jambiJarDir.list()) {
						if(f.startsWith("qtjambi-plugin-") && f.contains(infix)) {
							try {
								foundURLs.add(new URL("jar:"+new File(jambiJarDir, f).toURI()+"!/qtjambi-deployment.xml"));
							} catch (IOException e) {
							}
						}
					}
            	}
            	if(!loadQtFromLibraryPath) {
            		int qtPatchVersion = -1;
            		String libPrefix = String.format("qt-lib-core-native-%1$s%2$s-%3$s.%4$s.", osArchName, isDebug && operatingSystem!=OperatingSystem.Linux ? "-debug" : "", QtJambi_LibraryUtilities.qtMajorVersion, QtJambi_LibraryUtilities.qtMinorVersion);
        			for(File lib : jambiJarDir.listFiles()) {
            			if(lib.isFile() && lib.getName().startsWith(libPrefix) && lib.getName().endsWith(".jar")) {
            				String version = lib.getName().substring(libPrefix.length(), lib.getName().length()-4);
            				int v = Integer.parseInt(version);
            				if(v>qtPatchVersion)
            					qtPatchVersion = v;
            			}
            		}
            		if(qtPatchVersion>=0) {
            			String libSuffix = String.format("-native-%1$s%2$s-%3$s.%4$s.%5$s.jar", osArchName, isDebug && operatingSystem!=OperatingSystem.Linux ? "-debug" : "", QtJambi_LibraryUtilities.qtMajorVersion, QtJambi_LibraryUtilities.qtMinorVersion, qtPatchVersion);
            			for(File lib : jambiJarDir.listFiles()) {
            				if(lib.isFile() && lib.getName().startsWith("qt-") && lib.getName().endsWith(libSuffix)) {
            					try {
			    					foundURLs.add(new URL("jar:"+lib.toURI()+"!/qtjambi-deployment.xml"));
								} catch (IOException e) {
								}
            				}
            			}
            		}
            	}
            	if(!foundURLs.isEmpty())
            		specsFound = Collections.enumeration(foundURLs);
            }
            
            while (specsFound.hasMoreElements()) {
                URL url = specsFound.nextElement();
                
                if(loadedDeploymentSpecUrls.contains(url))
                	continue;
                loadedDeploymentSpecUrls.add(url);

                reporter.report("Found ", url.toString());

                DeploymentSpec spec = null;
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
								URL jarUrl = new URL(eform.substring(start, end));
								String jarName = new File(jarUrl.getFile()).getName();
								try {
								    reporter.report("Loading ", jarName, " from ", eform);
								    spec = unpackDeploymentSpec(url, jarName, null);
								} catch (ParserConfigurationException | SAXException | ZipException e) {
									Logger.getLogger("io.qt.internal").log(Level.WARNING, String.format("Unable to load native libraries from %1$s: %2$s", (jarName==null ? jarUrl : jarName), e.getMessage()), e);
								} catch (IOException e) {
								}
							} catch (MalformedURLException e) {
								Logger.getLogger("io.qt.internal").log(Level.WARNING, "", e);
							}
                    }
                } else if (protocol.equals("file")) {
                    // No unpack since we presume we are already unpacked
                    try {
						spec = unpackDeploymentSpec(url, null, Boolean.FALSE);
					} catch (ParserConfigurationException | SAXException | ZipException e) {
						Logger.getLogger("io.qt.internal").log(Level.WARNING, String.format("Unable to load native libraries from %1$s: %2$s", url, e.getMessage()), e);
					} catch (IOException e) {
					}
                }
                if(spec != null) {
                	if("qtjambi".equals(spec.getModule()))
                		deploymentSpec.add(0, spec);
                	else
                		deploymentSpec.add(spec);
                }
            }
            if(!deploymentSpec.isEmpty()) {
            	DeploymentSpec qtjambiSpec = deploymentSpec.get(0);
            	if(Configuration.Release.toString().compareToIgnoreCase(qtjambiSpec.getConfiguration()) == 0)
                    configuration = Configuration.Release;
                else if(Configuration.Debug.toString().compareToIgnoreCase(qtjambiSpec.getConfiguration()) == 0)
                    configuration = Configuration.Debug;
                else if(Configuration.Test.toString().compareToIgnoreCase(qtjambiSpec.getConfiguration()) == 0)
                    configuration = Configuration.Test;
                else
                	configuration = Configuration.Release;
            	switch (operatingSystem) {
                case Windows:
                	qtJambiLibraryPath = new File(qtjambiSpec.getBaseDir(), "bin").getAbsolutePath();
                	break;
            	default:
            		qtJambiLibraryPath = new File(qtjambiSpec.getBaseDir(), "lib").getAbsolutePath();
            		break;
            	}
            	for(DeploymentSpec spec : deploymentSpec) {
            		if(spec.getCompiler()!=null && qtjambiSpec.getCompiler()!=null && !spec.getCompiler().equals(qtjambiSpec.getCompiler())) {
            			throw new DeploymentSpecException(String.format("Native deployments of different builts: %1$s and %2$s", qtjambiSpec.getCompiler(), spec.getCompiler()));
            		}else if(!spec.getConfiguration().equals(qtjambiSpec.getConfiguration())) {
            			if(operatingSystem==OperatingSystem.Windows || spec.getModule()==null || !spec.getModule().startsWith("qt.lib.")) {
	            			throw new DeploymentSpecException(String.format("Native deployments of different configurations: %1$s and %2$s", qtjambiSpec.getConfiguration(), spec.getConfiguration()));
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
    		jambiDeploymentDir = _jambiDeploymentDir;
    		jambiJarDir = null;
    	}
    	
    	if(VERBOSE_LOADING)
    		System.out.println(reporter.recentReports());
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
    
 // The purpose of this method is to resolve the names provided in the list into DSO paths relative to the project.
    private static List<String> resolveSystemLibraries(List<String> tmpSystemLibrariesList) {
        if(tmpSystemLibrariesList == null)
            return null;
        List<String> resolvedList = new ArrayList<String>();
        for(String original : tmpSystemLibrariesList) {
            String s = original;
            if(File.separatorChar!='/')   // convert "/" into "\"
                s = s.replace('/', File.separatorChar);

            String resolvedPath = null;
            for(DeploymentSpec deploymentSpec : deploymentSpec) {
                File f = new File(deploymentSpec.getBaseDir(), s);
                if(f.isFile()) {
                    resolvedPath = s;
                    break;
                }

                File libDir = new File(deploymentSpec.getBaseDir(), "lib");
                f = new File(libDir, s);
                if(f.isFile()) {
                    resolvedPath = "lib" + File.separator + s;
                    break;
                }

                File binDir = new File(deploymentSpec.getBaseDir(), "bin");
                f = new File(binDir, s);
                if(f.isFile()) {
                    resolvedPath = "bin" + File.separator + s;
                    break;
                }
            }

            if(resolvedPath != null)
                resolvedList.add(resolvedPath);
            else
                System.err.println("IGNORED version.properties qtjambi.system.libraries entry \"" + original + "\": file could not be found");
        }
        return Collections.unmodifiableList(resolvedList);
    }
    
    private static Architecture decideArchitecture() {
    	switch(System.getProperty("os.arch").toLowerCase()) {
    	case "arm":
    	case "arm32":
    		return Architecture.arm;
    	case "arm64":
    	case "aarch64":
    		return Architecture.arm64;
    	case "x86_64":
    	case "x64":
    	case "amd64":
    		return Architecture.x86_64;
    	default:
    		return Architecture.x86;
    	}
    }

    private static OperatingSystem decideOperatingSystem() {
        String osName = System.getProperty("os.name").toLowerCase();
        if (osName.startsWith("windows")) return OperatingSystem.Windows;
        else if (osName.startsWith("mac")) return OperatingSystem.MacOSX;
        else if (osName.startsWith("linux")) {
        	if(System.getProperty("java.runtime.name").toLowerCase().startsWith("android"))
        		return OperatingSystem.Android;
        	else 
        		return OperatingSystem.Linux;
        }
        return OperatingSystem.Linux;
    }

	static Configuration configuration() {
		return configuration;
	}
	
    private static Configuration decideConfiguration() {
    	Configuration configuration = null;

        String debugString = System.getProperty("io.qt.debug");
        try {
            if(debugString != null) {
                // This was added to allow unit tests to specify Configuration (as no MANIFEST.MF is available)
                if(Configuration.Release.toString().compareToIgnoreCase(debugString) == 0)
                    configuration = Configuration.Release;
                else if(Configuration.Debug.toString().compareToIgnoreCase(debugString) == 0)
                    configuration = Configuration.Debug;
                else if(Configuration.Test.toString().compareToIgnoreCase(debugString) == 0)
                    configuration = Configuration.Test;

                if(configuration == null) {
                    Boolean booleanValue = Boolean.valueOf(debugString);
                    if((booleanValue != null && booleanValue.booleanValue()) || debugString.length() == 0) {
                        configuration = Configuration.Debug;
                        // FIXME: When we can unambigiously auto-detect this from the MANIFEST we don't need to
                        //  emit this, we'd only emit it when both non-debug and debug were found and we selected
                        //  the debug kind.
                        java.util.logging.Logger.getLogger("io.qt.internal").log(java.util.logging.Level.WARNING, "-Dio.qt.debug=" + Boolean.TRUE + "; is set instead of =debug.");
                    }
                }
            }
        } catch(Exception e) {
            java.util.logging.Logger.getLogger("io.qt.internal").log(java.util.logging.Level.SEVERE, "-Dio.qt.debug=" + Boolean.FALSE + "; is assumed default", e);
            // only because Configuration.Release is assumed
        }

        if(configuration==null)
        	configuration = Configuration.Release;
        return configuration;
    }
    
    static File jambiDeploymentDir() {
        return jambiDeploymentDir;
    }
    
    @NativeAccess
	@QtUninvokable
    private static void deployContainerAccess(String library) {
    	if(!noDeploymentSpec) {
	    	String libraryPlatformName;
	    	switch (operatingSystem) {
	        case Windows: 
	        	libraryPlatformName = "plugins/containeraccess/" + (configuration == Configuration.Debug && !isMinGWBuilt
		                ? library + "d.dll"  // "QtFood4.dll"
		                : library + ".dll");
	        	break;
	        case MacOSX: 
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
	        case Linux:
	        	libraryPlatformName = "plugins/containeraccess/lib" + library + ".so";
	        	break;
			default:
				return;
	        }
        	LibraryEntry entry = libraryMap.get(libraryPlatformName);
        	if(entry!=null) {
        		try {
					entry.extract();
				} catch (Throwable e) {
					Logger.getLogger("io.qt.internal").throwing(NativeLibraryManager.class.getName(), "deployContainerAccess", e);
				}
        	}
    	}
    }

    @NativeAccess
	@QtUninvokable
	static void resetDeploymentSpecs() {
        deploymentSpec.clear();
        if(deleteTmpDeployment) {
	        if(jambiDeploymentDir.exists() && jambiDeploymentDir.isDirectory()) {
	        	Logger.getLogger("io.qt.internal").log(Level.FINEST, ()->"Deleting tmp deployment directory "+jambiDeploymentDir.getAbsolutePath()+".");
	        	clearAndDelete(jambiDeploymentDir);
	        	if(jambiDeploymentDir.exists() && jambiDeploymentDir.isDirectory()) {
	        		Logger.getLogger("io.qt.internal").log(Level.FINEST, "Preparing pending deletion...");
	        		Preferences preferences = Preferences.userNodeForPackage(NativeLibraryManager.class);
	        		String dirs = preferences.get("qtjambi.previous.deployment.dir", null);
	        		if(dirs!=null && !dirs.isEmpty()) {
	        			preferences.put("qtjambi.previous.deployment.dir", dirs + File.pathSeparator + jambiDeploymentDir.getAbsolutePath());
	        		}else {
	        			preferences.put("qtjambi.previous.deployment.dir", jambiDeploymentDir.getAbsolutePath());
	        		}
	        	}
	    	}else {
	    		Logger.getLogger("io.qt.internal").log(Level.FINEST, ()->"Tmp deployment directory "+jambiDeploymentDir.getAbsolutePath()+" does not exist.");
	    	}
        }
    }
    
    private static class XMLHandler extends DefaultHandler {
    	XMLHandler() {
		}

        public DeploymentSpec spec;
        private final List<String> direntCurrentPathList = new ArrayList<String>();

        public void startElement(String uri,
                                 String localName,
                                 String name,
                                 org.xml.sax.Attributes attributes) {
            if (name.equals("symlink")) {
            	SymlinkEntry e = new SymlinkEntry();
                e.setName(attributes.getValue("name"));
                e.setTarget(attributes.getValue("target"));
                if (e.getName() == null) {
                    throw new DeploymentSpecException("<symlink> element missing required attribute \"name\"");
                }
                if (e.getTarget() == null) {
                    throw new DeploymentSpecException("<symlink> element missing required attribute \"target\"");
                }

                e.setDeploymentSpec(spec);
                
                if(e.getName().startsWith("qml/")) {
                	spec.setHasQmlPaths(true);
                }else if(e.getName().startsWith("plugins/")) {
                	spec.setHasPluginPaths(true);
                }else if(e.getName().startsWith("translations/")) {
                	spec.setHasTrPaths(true);
                }else{
	                String fileName = e.getName();
                	if(fileName.startsWith("lib/") || fileName.startsWith("bin/"))
                		fileName = fileName.substring(4);
	                // Add library name to the global map of libraries...
	                LibraryEntry old = libraryMap.get(fileName);
	                if (old == null) {
		                reporter.report(" - adding '", fileName, "' to library map");
		                libraryMap.put(fileName, e);
	                }
                }

                spec.addLibraryEntry(e);
                reporter.report(" - symlink: name='", e.getName(), "'");
            	
            } else if (name.equals("library") || name.equals("syslibrary")) {
                LibraryEntry e = new LibraryEntry();
                e.setName(attributes.getValue("name"));
                if (e.getName() == null) {
                    throw new DeploymentSpecException("<library> element missing required attribute \"name\"");
                }

                e.setDeploymentSpec(spec);

                if(e.getName().startsWith("qml/")) {
                	spec.setHasQmlPaths(true);
                }else if(e.getName().startsWith("plugins/")) {
                	spec.setHasPluginPaths(true);
                	if(e.getName().startsWith("plugins/containeraccess/")) {
                		LibraryEntry old = libraryMap.get(e.getName());
    	                if (old == null) {
    		                reporter.report(" - adding '", e.getName(), "' to library map");
    		                libraryMap.put(e.getName(), e);
    	                }
                	}
                }else if(e.getName().startsWith("translations/")) {
                	spec.setHasTrPaths(true);
                }else{
	                String fileName = e.getName();
                	if(fileName.startsWith("lib/") || fileName.startsWith("bin/"))
                		fileName = fileName.substring(4);
	                // Add library name to the global map of libraries...
	                LibraryEntry old = libraryMap.get(fileName);
	                if (old != null && !old.getDeploymentSpec().getSourceUrl().equals(spec.getSourceUrl())) {
	                    throw new DeploymentSpecException("<library> '" + e.getName()
	                                                      + "' is duplicated. Present in both '"
	                                                      + spec.getSourceUrl() + "' and '"
	                                                      + old.getDeploymentSpec().getSourceUrl() + "'.");
	                }
	                reporter.report(" - adding '", fileName, "' to library map");
	                libraryMap.put(fileName, e);
                }

                spec.addLibraryEntry(e);
                reporter.report(" - library: name='", e.getName(), "'");
            } else if (name.equals("qtjambi-deploy")) {
                String system = attributes.getValue("system");
                if (system == null || system.length() == 0) {
                    throw new DeploymentSpecException("<qtjambi-deploy> element missing required attribute 'system'");
                } else if (!system.equals(osArchName)) {
                    throw new WrongSystemException(String.format("Expected version: %1$s, found: %2$s.", osArchName, system));
                }
                spec.setSystem(system);
                String version = attributes.getValue("version");
                if (version == null || version.length() == 0)
                    throw new DeploymentSpecException("<qtjambi-deploy> element missing required attribute 'version'");
                spec.setVersion(version);
                spec.setModule(attributes.getValue("module"));
                spec.setCompiler(attributes.getValue("compiler"));
                spec.setConfiguration(attributes.getValue("configuration"));
                spec.setDate(attributes.getValue("date"));
                spec.setTime(attributes.getValue("time"));
            } else if (name.equals("directory")) {
                String attrName = attributes.getValue("name");
                if (attrName == null) {
                    throw new DeploymentSpecException("<directory> element missing required attribute \"name\"");
                }
                while(attrName.length() > 0 && attrName.charAt(0) == '/')
                    attrName = attrName.substring(1);
                while(attrName.length() > 0 && attrName.charAt(attrName.length() - 1) == '/')
                    attrName = attrName.substring(0, attrName.length() - 1 - 1);
                direntCurrentPathList.add(attrName);
            } else if (name.equals("file")) {
                String attrName = attributes.getValue("name");
                if (attrName == null) {
                    throw new DeploymentSpecException("<file> element missing required attribute \"name\"");
                }
                StringBuilder sb = new StringBuilder();
                for(String s : direntCurrentPathList) {
                    if(sb.length() > 0)
                        sb.append('/');
                    sb.append(s);
                }
                if(sb.length() > 0)
                    sb.append('/');
                sb.append(attrName);
                spec.addDirentPath(sb.toString(), "true".equals(attributes.getValue("executable")));
                reporter.report(" - dirent path='", sb.toString(), "'");
            } else if (name.equals("qmllib")) {
                String attrName = attributes.getValue("name");
                if (attrName == null) {
                    throw new DeploymentSpecException("<qmllib> element missing required attribute \"name\"");
                }
                spec.addQmlLibrary(attrName);
            }
        }

        public void endElement(String uri,
                                 String localName,
                                 String name) {
            if (name.equals("directory")) {
                if (direntCurrentPathList.isEmpty() == false)
                    direntCurrentPathList.remove(direntCurrentPathList.size() - 1);  // remove last
            }
        }
    }

    //no-one used this so I commented it out
   /*private static class ChecksumFileFilter implements FilenameFilter {
        public boolean accept(File dir, String name) {
            return name.endsWith(".chk");
        }
    }*/


    /**
     * Returns a file that is used for caching native libraries. The
     * path is a subdirectory of <code>java.io.tmpdir</code>, named
     * <code>QtJambi_{user}_{architecture}_{version}_{key}</code>. The
     * key is the same as the cache key used in the deployment
     * descriptor. The purpose of using different keys is to avoid
     * binary compatibility when various configurations of Qt and Qt
     * Jambi are used on the same system.
     *
     * When deployment descriptors are not used, this location will
     * not be used.
     *
     * @param key The cache key to.
     * @return A File representing the location directory for
     * temporary content. The directory is not explicitly created in
     * this here.
     */
    @SuppressWarnings("unused")
	private static File jambiTempDirBase(DeploymentSpec spec) {
        File tmpDir = new File(System.getProperty("java.io.tmpdir"));
        String user = System.getProperty("user.name");
        return new File(tmpDir, "QtJambi_" + spec.getVersion() + "_" + spec.getSystem() + "_" + spec.getCompiler() + "_" + spec.getConfiguration() + "_" + spec.getDate() + "_" + spec.getTime() + "_" + user);
    }


    /**
     * Returns the list of all plugin paths that are specified in the
     * deployment descriptors. If deployment descriptors are not used,
     * this list will be an empty list.
     *
     * @return The list of plugin paths
     */
    static List<String> pluginPaths() {
		pluginLibraries.removeIf( l -> {
			try {
				l.extract();
			}catch(Throwable t) {
				java.util.logging.Logger.getLogger("io.qt.internal").log(java.util.logging.Level.SEVERE, "Error while extracting library", t);
			}
			if(isQmlLoaded.get()) {
				try {
					l.extractQml();
				}catch(Throwable t) {
					java.util.logging.Logger.getLogger("io.qt.internal").log(java.util.logging.Level.SEVERE, "Error while extracting library", t);
				}
			}
			return true;
		});
        List<String> paths = new ArrayList<String>();
        for (DeploymentSpec spec : deploymentSpec) {
            File root = spec.getBaseDir();
            if(spec.isHasPluginPaths())
                paths.add(new File(root, "plugins").getAbsolutePath());
        }
        return paths;
    }

    static void loadLibrary(String library) {
    	String libraryPlatformName;
    	switch (operatingSystem) {
        case Windows: 
        	libraryPlatformName = library + ".dll";
        	break;
        case MacOSX: 
        	{
        		Availability availability = getLibraryAvailability(library, "lib" + library + ".jnilib", Collections.emptyList());
        		if(availability.isAvailable()) {
        			libraryPlatformName = "lib" + library + ".jnilib";
        		}else {
        			availability = getLibraryAvailability(library, "lib" + library + ".dylib", Collections.emptyList());
        			if(availability.isAvailable()) {
        				libraryPlatformName = "lib" + library + ".dylib";
        			}else {
        				libraryPlatformName = library + ".framework/" + library;
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
        case Linux:
        	libraryPlatformName = "lib" + library + ".so";
        	break;
		default:
			return;
        }
    	loadNativeLibrary(Object.class, getLibraryAvailability(library, libraryPlatformName, Collections.emptyList()), false, null);
    }
    
    static File loadQtJambiLibrary() {
		return loadQtJambiLibrary(NativeLibraryManager.class, null, "QtJambi", QtJambi_LibraryUtilities.qtMajorVersion, QtJambi_LibraryUtilities.qtMinorVersion, QtJambi_LibraryUtilities.qtJambiPatch);
    }

    static void loadQtJambiLibrary(Class<?> callerClass, String library) {
    	LibVersion libVersion = getLibVersion(callerClass);
    	try {
    		loadQtJambiLibrary(callerClass, "QtJambi", library, libVersion.qtMajorVersion, libVersion.qtMinorVersion, libVersion.qtJambiPatch);
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
    		loadQtJambiLibrary(callerClass, null, library, libVersion.qtMajorVersion, libVersion.qtMinorVersion, libVersion.qtJambiPatch);
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
    
    private static File loadQtJambiLibrary(Class<?> callerClass, String prefix, String library, int... versionArray) {
    	Availability availability = getLibraryAvailability(true, prefix, library, null, null, configuration, versionArray);
    	if(!availability.isAvailable()
    			&& versionArray!=null 
        		&& versionArray.length==3 
        		&& !dontSearchDeploymentSpec 
        		&& !noDeploymentSpec) {
        	String className = callerClass.getName();
        	int idx = className.lastIndexOf('.');
        	className = className.substring(idx+1);
        	String classURL = ""+callerClass.getResource(className+".class");
        	int index;
	    	if(classURL.startsWith("jar:file:") && (index = classURL.indexOf("!/"))>0) {
	    		String jarFileURL = classURL.substring(4, index);
	    		File jarFile = null;
	    		try {
					jarFile = new File(new URL(jarFileURL).toURI());
				} catch (URISyntaxException | MalformedURLException e) {
				}
	    		if(jarFile!=null && jarFile.exists()) {
	    			File directory = jarFile.getParentFile();
	    			String fileName = jarFile.getName();
	    			String qtjambiVersion = String.format("-%1$s.%2$s.%3$s", versionArray[0], versionArray[1], versionArray[2]);
    				index = fileName.indexOf(qtjambiVersion+".jar");
	    			if(index>0) {
	    				String moduleName = fileName.substring(0, index);
		    			String postfix = "-native-" + osArchName;
		    			if(configuration==Configuration.Debug)
		    				postfix += "-debug";
		    			postfix += qtjambiVersion+".jar";
		    			File nativeFile = new File(directory, moduleName + postfix);
		    			if(nativeFile.exists()) {
		    				try {
								URL nativeFileURL = new URL("jar:"+nativeFile.toURI()+"!/qtjambi-deployment.xml");
								if(!loadedDeploymentSpecUrls.contains(nativeFileURL)) {
									loadedDeploymentSpecUrls.add(nativeFileURL);
									DeploymentSpec spec = unpackDeploymentSpec(nativeFileURL, nativeFile.getName(), null);
									if(spec != null 
											&& String.format("%1$s.%2$s.%3$s", versionArray[0], versionArray[1], versionArray[2]).equals(spec.getVersion())
											&& spec.getConfiguration().equalsIgnoreCase(configuration.name())
											&& (deploymentSpec.isEmpty() || spec.getCompiler().equals(deploymentSpec.get(0).getCompiler()))) {
					                	if("qtjambi".equals(spec.getModule()))
					                		deploymentSpec.add(0, spec);
					                	else
					                		deploymentSpec.add(spec);
					                	if(qtJambiLibraryPath==null) {
						                	switch (operatingSystem) {
						                    case Windows:
						                    	qtJambiLibraryPath = new File(spec.getBaseDir(), "bin").getAbsolutePath();
						                    	break;
						                	default:
						                		qtJambiLibraryPath = new File(spec.getBaseDir(), "lib").getAbsolutePath();
						                		break;
						                	}
					                	}
					                	availability = getLibraryAvailability(true, prefix, library, null, null, configuration, versionArray);
					                }
								}
							} catch (ParserConfigurationException | SAXException | ZipException e) {
								Logger.getLogger("io.qt.internal").log(Level.WARNING, String.format("Unable to load native libraries from %1$s: %2$s", nativeFile.getName(), e.getMessage()), e);
							} catch (IOException e) {
							}
		    			}
	    			}
	    		}
	    	}
        }
        return loadNativeLibrary(callerClass, availability, true, versionArray);
    }

    /**
     * Loads a library with name specified in <code>library</code>.
     * The library name will be expanded to the default shared library
     * name for a given platform, so the name "QtCore" and version "4" and "5" will be
     * expanded like this:
     *
     * <ul>
     *   <li> Windows: QtCore4.dll
     *   <li> Linux / Unix: libQtCore.so.4
     *   <li> Mac OS X: libQtCore.4.dylib
     * </ul>
     * <ul>
     *   <li> Windows: Qt5Core.dll
     *   <li> Linux / Unix: libQt5Core.so.5
     *   <li> Mac OS X: libQt5Core.5.dylib
     * </ul>
     *
     * When using loading libraries from the filesystem, this method
     * simply calls <code>System.loadLibrary</code>.
     * @param library The name of the library..
     */
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
    	Availability availability = getLibraryAvailability(false, "Qt", library, LIBINFIX, null, configuration, libVersion.qtMajorVersion, libVersion.qtMinorVersion);
    	switch(libraryRequirementMode) {
		case ProvideOnly:
			availability.extractLibrary();
			return;
		case Optional:
			if(!availability.isAvailable())
				return;
			break;
		default:
			break;
    	}
    	try {
    		switch(library) {
    		case "Qml":
				isQmlLoaded.set(true);
				for(LibraryEntry e : new ArrayList<>(libraryMap.values())) {
	            	if(e.isLoaded() && e.isQmlExtracting()) {
	            		try {
							e.extractQml();
						} catch (Throwable e1) {
							throw new QLibraryNotLoadedError("Unable to extract library "+e.getName(), e1);
						}
	            	}
				}
				break;
    		}
			loadNativeLibrary(Object.class, availability, true, libVersion.qtMajorVersion, libVersion.qtMinorVersion);
		} catch (RuntimeException | Error e) {
			switch(libraryRequirementMode) {
			case Optional:
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
    	Availability availability = getLibraryAvailability(false, null, library, LIBINFIX, version, configuration, null);
    	switch(libraryRequirementMode) {
		case ProvideOnly:
			availability.extractLibrary();
			break;
		case Optional:
			if(!availability.isAvailable())
				return;
			break;
		default:
			break;
    	}
    	loadNativeLibrary(Object.class, availability, true, null);
    }
    
    static File loadQtCore() {
    	try{
	    	loadSystemLibraries();
        	loadUtilityLibrary("icudata", NativeLibraryManager.ICU_VERSION, LibraryRequirementMode.Optional);
    		loadUtilityLibrary("icuuc", NativeLibraryManager.ICU_VERSION, LibraryRequirementMode.Optional);
    		loadUtilityLibrary("icui18n", NativeLibraryManager.ICU_VERSION, LibraryRequirementMode.Optional);
    	} catch (Throwable e) {
    		java.util.logging.Logger.getLogger("io.qt.internal").log(java.util.logging.Level.SEVERE, "Error while loading system library", e);
    	}
    	Availability availability = getLibraryAvailability(false, "Qt", "Core", LIBINFIX, null, configuration, QtJambi_LibraryUtilities.qtMajorVersion, QtJambi_LibraryUtilities.qtMinorVersion);
    	if(!availability.isAvailable()) {
			if(configuration==Configuration.Release) {
				// try to run qtjambi with debug libraries instead
				configuration=Configuration.Debug;
				Availability _availability = getLibraryAvailability(false, "Qt", "Core", LIBINFIX, null, configuration, QtJambi_LibraryUtilities.qtMajorVersion, QtJambi_LibraryUtilities.qtMinorVersion);
				if(_availability.isAvailable())
					availability = _availability;
			}else {
				if(dontUseFrameworks==null) {
					dontUseFrameworks = Boolean.TRUE;
					Availability _availability = getLibraryAvailability(false, "Qt", "Core", LIBINFIX, null, configuration, QtJambi_LibraryUtilities.qtMajorVersion, QtJambi_LibraryUtilities.qtMinorVersion);
					if(!_availability.isAvailable()) {
						configuration=Configuration.Debug;
						_availability = getLibraryAvailability(false, "Qt", "Core", LIBINFIX, null, configuration, QtJambi_LibraryUtilities.qtMajorVersion, QtJambi_LibraryUtilities.qtMinorVersion);
						if(_availability.isAvailable())
							availability = _availability;
					}
				}else {
					if(!isMinGWBuilt && operatingSystem==OperatingSystem.Windows) {
						Availability stdCAvailability = getLibraryAvailability(false, null, "libstdc++-6", null, null, Configuration.Release, null);
				    	if(stdCAvailability.entry!=null){
				    		isMinGWBuilt = true;
				    	}else if(stdCAvailability.file!=null){
				    		isMinGWBuilt = new File(stdCAvailability.file.getParentFile(), "Qt"+QtJambi_LibraryUtilities.qtMajorVersion+"Core.dll").isFile();
				    	}
				    	if(isMinGWBuilt) {
				    		availability = getLibraryAvailability(false, "Qt", "Core", LIBINFIX, null, configuration, QtJambi_LibraryUtilities.qtMajorVersion, QtJambi_LibraryUtilities.qtMinorVersion);
				    	}
			    	}
				}
			}
    	}
    	try{
    		File lib = loadNativeLibrary(Object.class, availability, true, QtJambi_LibraryUtilities.qtMajorVersion, QtJambi_LibraryUtilities.qtMinorVersion);
    		if(lib!=null)
    			qtLibraryPath = lib.getParentFile().getAbsolutePath();
    		return lib;
		}finally {
			if(dontUseFrameworks==null) {
				dontUseFrameworks = Boolean.FALSE;
			}
		}
    }
    
    private static LibVersion getLibVersion(Class<?> callerClass) {
    	return qtJambiVersionsByClass.computeIfAbsent(callerClass, cls -> {
    		if(callerClass.getSimpleName().equals("QtJambi_LibraryUtilities")) {
	    		try {
	    			int qtMajorVersion = QtJambiInternal.fetchField(null, callerClass, "qtMajorVersion", int.class);
	    			int qtMinorVersion = QtJambiInternal.fetchField(null, callerClass, "qtMinorVersion", int.class);
	    			int qtJambiPatch = QtJambiInternal.fetchField(null, callerClass, "qtJambiPatch", int.class);
	    			return new LibVersion(qtMajorVersion, qtMinorVersion, qtJambiPatch);
	    		}catch(Throwable t) {}
	    		
	        	String classURL = ""+callerClass.getResource(callerClass.getSimpleName()+".class");
	        	int index;
		    	if(classURL.startsWith("jar:file:") && (index = classURL.indexOf("!/"))>0) {
		    		classURL = classURL.substring(4, index);
		    		if(classURL.endsWith(".jar") && (index = classURL.lastIndexOf('-'))>0) {
		    			classURL = classURL.substring(index+1);
		    			String[] versionString = classURL.split("\\.");
		    			return new LibVersion(Integer.parseInt(versionString[0]), Integer.parseInt(versionString[1]), Integer.parseInt(versionString[2]));
		    		}
		    	}
    		}
    		return new LibVersion(QtJambi_LibraryUtilities.qtMajorVersion, QtJambi_LibraryUtilities.qtMinorVersion, QtJambi_LibraryUtilities.qtJambiPatch);
    	});
    }
    
    public static boolean isMinGWBuilt() {
		return isMinGWBuilt;
	}
    
    static boolean isAvailableQtLibrary(Class<?> callerClass, String library) {
    	LibVersion version = getLibVersion(callerClass);
    	return getLibraryAvailability(false, "Qt", library, LIBINFIX, null, configuration, version.qtMajorVersion, version.qtMinorVersion).isAvailable();
    }
    
    static Availability getQtLibraryAvailability(Class<?> callerClass, String library) {
    	LibVersion version = getLibVersion(callerClass);
    	return getLibraryAvailability(false, "Qt", library, LIBINFIX, null, configuration, version.qtMajorVersion, version.qtMinorVersion);
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
	    		return getLibraryAvailability(false, null, library, null, null, configuration, iparts).isAvailable();
    	}
    	return getLibraryAvailability(false, null, library, null, version, configuration, null).isAvailable();
    }
    
    private static class Availability{
    	public Availability(String libraryRawName, File file, String libFormat, List<String> replacements) {
			super();
			this.libraryRawName = libraryRawName;
			this.file = file;
			this.entry = null;
			this.libFormat = libFormat;
			this.replacements = replacements;
		}
    	public Availability(String libraryRawName, String libFormat, List<String> replacements) {
			super();
			this.libraryRawName = libraryRawName;
			this.file = null;
			this.entry = null;
			this.libFormat = libFormat;
			this.replacements = replacements;
		}
    	public Availability(String libraryRawName, LibraryEntry entry, File file, String libFormat, Iterable<String> replacements) {
			super();
			this.libraryRawName = libraryRawName;
			this.file = file;
			this.entry = entry;
			this.libFormat = libFormat;
			this.replacements = replacements;
		}
    	final String libraryRawName;
		final File file;
		final LibraryEntry entry;
		@SuppressWarnings("unused")
		final Iterable<String> replacements;
		final String libFormat;
		boolean isAvailable() {
			return (file!=null && file.exists()) || entry!=null;
		}
		
		void extractLibrary() {
			if(entry!=null && entry.isExtracting() && !file.exists() && !entry.isLoaded()) {
				try {
        			entry.extract();
				} catch (Throwable e1) {
					throw new QLibraryNotLoadedError("Unable to extract library "+entry.getName(), e1);
				}
			}
		}
    }

    private static Availability getLibraryAvailability(boolean qtjambiLib, String qtprefix, String library, String libInfix, String versionStrg, Configuration configuration, int... version) {
    	List<String> replacements = new ArrayList<>();
    	String libFormat = 
    			qtjambiLib 
    			? qtjambiLibraryName(qtprefix, library, configuration, replacements, version)
    			: qtLibraryName(qtprefix, library, libInfix, versionStrg, configuration, replacements, version);  // "QtDBus" => "libQtDBus.so.4"
    	return getLibraryAvailability(qtprefix==null ? library : qtprefix+library, libFormat, replacements);
    }
    
    private static List<String> computeLibraryPaths(){
    	String libPaths = System.getProperty("io.qt.library-path-override");
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
        case MacOSX: 
    		libPaths = System.getenv("DYLD_LIBRARY_PATH");
        	break;
    	default:
    		libPaths = System.getenv("LD_LIBRARY_PATH");
    	}
        if (libPaths != null)
            libraryPaths.addAll(ldLibraryPaths.computeIfAbsent(libPaths, NativeLibraryManager::splitPath));
        if(qtJambiLibraryPath!=null)
        	libraryPaths.add(qtJambiLibraryPath);
        switch (operatingSystem) {
        case Windows:
        	libraryPaths.add(new File(jambiDeploymentDir, "bin").getAbsolutePath());
        	if(jambiJarDir!=null)
        		libraryPaths.add(new File(jambiJarDir, "bin").getAbsolutePath());
        	break;
    	default:
    		libraryPaths.add(new File(jambiDeploymentDir, "lib").getAbsolutePath());
        	if(jambiJarDir!=null)
        		libraryPaths.add(new File(jambiJarDir, "lib").getAbsolutePath());
    		break;
    	}
        
        if (System.getProperties().contains("io.qt.library-path-override")) {
//            reporter.report(" - using 'io.qt.library-path-override'");
        	libraryPaths.addAll(javaLibraryPathOverrides.computeIfAbsent(System.getProperty("io.qt.library-path-override"), NativeLibraryManager::splitPath));
        } else {
//            reporter.report(" - using 'java.library.path'");
        	libraryPaths.addAll(javaLibraryPaths.computeIfAbsent(System.getProperty("java.library.path"), NativeLibraryManager::splitPath));
        }
        
        return mergeJniLibdir(libraryPaths);
    }
    
    private static Availability getLibraryAvailability(String libraryRawName, String libFormat, List<String> replacements) {
    	if(operatingSystem!=OperatingSystem.Android) {
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
	        	if(!noDeploymentSpec) {
		        	LibraryEntry entry = libraryMap.get(lib);
		        	while(entry instanceof SymlinkEntry) {
			        	String target = ((SymlinkEntry)entry).getTarget();
			        	if(target.startsWith("lib/") || target.startsWith("bin/")) {
			        		target = target.substring(4);
			        	}
			        	LibraryEntry eTarget = libraryMap.get(target);
			        	if(eTarget==null)
			        		break;
			        	else
			        		entry = eTarget;
			        }
		        	if(entry!=null) {
		        		return new Availability(libraryRawName, entry, entry.getDeploymentSpec().buildPath(entry.getName()), libFormat, replacements);
		        	}
	        	}
	        	if(libraryPaths==null)
	        		libraryPaths = computeLibraryPaths();
		        for (String path : libraryPaths) {
		            File f = new File(path, lib);
		            if (f.exists()) {
		            	return new Availability(libraryRawName, f, libFormat, replacements);
		            }
		        }
	        }while(iter.hasNext());
    	}else {
    		String libraryFilePath = QtJambiInternal.libraryFilePath();
    		File dir = new File(libraryFilePath).getParentFile();
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
	            	return new Availability(libraryRawName, f, libFormat, replacements);
	            }
	        }while(iter.hasNext());
    	}
        return new Availability(libraryRawName, libFormat, replacements);
    }

    /**
     * Returns a classloader for current context...
     * @return The classloader
     */
    private static ClassLoader classLoader() {
        ClassLoader loader = Thread.currentThread().getContextClassLoader();
        if (loader == null) {
            loader = NativeLibraryManager.class.getClassLoader();
            assert(loader != null);
        }
        return loader;
    }

    /**
     * Tries to load the specified library. It will first try to load
     * the library using the deploy spec, and if that fails, it will
     * try to load the library using a standard System.loadLibrary()
     * call.
     * @param lib The full name of the library to load, such as libQtCore.so.4
     */
    private static File loadNativeLibrary(Class<?> callerClass, Availability availability, boolean searchSpec, int... versionArray) {
    	File result = null;
        try {
        	result = loadLibrary_helper(callerClass, availability);
            if (VERBOSE_LOADING)
                System.out.println(reporter.recentReports());
        } catch (RuntimeException | Error e) {
        	if(VERBOSE_LOADING)
        		System.err.println(reporter.recentReports());
        	throw e;
        } catch (Throwable e) {
    		if(reporter.toString().isEmpty()) {
        		if(e.getMessage().isEmpty()) {
	        		throw new QLibraryNotLoadedError("Loading library failed.", e);
            	}else {
            		throw new QLibraryNotLoadedError(e.getMessage(), e);
	        	}
        	}else {
    			throw new QLibraryNotLoadedError("Loading library failed. Progress so far: " + reporter, e);
        	}
        }
        return result;
    }

	private static BiConsumer<Class<?>, String> libraryLoader;
    static{
    	libraryLoader = (callerClass, lib) -> {
    		synchronized(NativeLibraryManager.class) {
	    		@SuppressWarnings({ "rawtypes", "unchecked" })
	    		QDeclarableSignals.Signal2<Class<?>, String> loadLibrary = new QDeclarableSignals.Signal2(Class.class, String.class);
				try {
					if(operatingSystem==OperatingSystem.Android)
						loadLibrary.connect(Runtime.getRuntime(), "loadLibrary0(Class, String)");
					else
						loadLibrary.connect(Runtime.getRuntime(), "load0(Class, String)");
					libraryLoader = loadLibrary::emit;
				} catch (QNoSuchSlotException e) {
					libraryLoader = (_callerClass, _lib)->Runtime.getRuntime().load(_lib);
				}
    		}
    		libraryLoader.accept(callerClass, lib);
    	};
    }
    
    private static List<String> splitPath(String path){
    	if(path!=null) {
    		return Arrays.asList(path.split("\\"+File.pathSeparator));
    	}else {
    		return Collections.emptyList();
    	}
    }

    private static File loadLibrary_helper(Class<?> callerClass, Availability availability) {
    	if(loadedLibraries.contains(availability.libFormat))
    		return null;
    	if(operatingSystem==OperatingSystem.Android) {
	        ClassLoader callerClassLoader = callerClass.getClassLoader();
    		if(callerClassLoader==NativeLibraryManager.class.getClassLoader()
            		|| callerClassLoader==Object.class.getClassLoader()) {
    			Runtime.getRuntime().loadLibrary(availability.libFormat);
			}else {
            	libraryLoader.accept(callerClass, availability.libFormat);
            }
            loadedLibraries.add(availability.libFormat);
    		return null;
    	}else {
	    	if(availability.entry!=null && availability.entry.isLoaded()) {
				File result = availability.entry.getDeploymentSpec().buildPath(availability.entry.getName());
		        if(operatingSystem==OperatingSystem.MacOSX && (dontUseFrameworks==null || !dontUseFrameworks) && qtLibraryPath==null
		        		&& !result.getName().endsWith(".dylib") && !result.getName().endsWith(".jnilib")) {
		        	result = result.getParentFile().getParentFile().getParentFile();
		        }
	            return result;
			}
	        ClassLoader callerClassLoader = callerClass.getClassLoader();
	        boolean qmlLoaded = isQmlLoaded.get();
	        File libFile = availability.file;
	        if(libFile==null && availability.entry!=null) {
	    		libFile = availability.entry.getDeploymentSpec().buildPath(availability.entry.getName());
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
	    		if (System.getProperties().contains("io.qt.library-path-override")) {
	    			message = String.format("Library %1$s (%2$s) was not found in 'io.qt.library-path-override=%3$s'", availability.libraryRawName, String.join(", ", fileNames), System.getProperty("io.qt.library-path-override"));
				}else {
	    			message = String.format("Library %1$s (%2$s) was not found in 'java.library.path=%3$s'", availability.libraryRawName, String.join(", ", fileNames), System.getProperty("java.library.path"));
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
	        	reporter.report("Loading library: '", availability.libraryRawName, "' from location: ", libFile.getAbsolutePath());
	            if(callerClassLoader==NativeLibraryManager.class.getClassLoader()
	            		|| callerClassLoader==Object.class.getClassLoader()) {
	            	Runtime.getRuntime().load(availability.file.getAbsolutePath());
	            }else {
	            	libraryLoader.accept(callerClass, availability.file.getAbsolutePath());
	            }
	            loadedLibraries.add(availability.libFormat);
	            File result = libFile;
		        if(operatingSystem==OperatingSystem.MacOSX && (dontUseFrameworks==null || !dontUseFrameworks) && qtLibraryPath==null
		        		&& !result.getName().endsWith(".dylib") && !result.getName().endsWith(".jnilib")) {
		        	result = result.getParentFile().getParentFile().getParentFile();
		        }
		        if(availability.entry!=null)
		        	availability.entry.setLoaded(true);
	            return result;
	    	}else {
	    		String message;
	    		if(availability.entry!=null) {
	    			message = String.format("Library %1$s (%2$s) was not found in %3$s", availability.libraryRawName, libFile.getName(), availability.entry.getDeploymentSpec().getSourceUrl());
	    		}else {
	    			if (System.getProperties().contains("io.qt.library-path-override")) {
	        			message = String.format("Library %1$s (%2$s) was not found in 'io.qt.library-path-override=%3$s'", availability.libraryRawName, libFile.getName(), System.getProperty("io.qt.library-path-override"));
	    			}else {
	        			message = String.format("Library %1$s (%2$s) was not found in 'java.library.path=%3$s'", availability.libraryRawName, libFile.getName(), System.getProperty("java.library.path"));
	    			}
	    		}
				throw new QLibraryNotFoundError(message);
	    	}
    	}
    }

    /**
     * 
     * @param url This maybe "file" or "jar" protocol.  "http" is untested.
     * @return
     * @throws SAXException 
     * @throws ParserConfigurationException 
     * @throws IOException 
     * @throws Exception
     */
    private static DeploymentSpec readDeploySpec(URL url) throws ParserConfigurationException, SAXException, IOException {
        reporter.report("Checking Archive '", url.toString(), "'");

        DeploymentSpec spec = new DeploymentSpec();
        spec.setSourceUrl(url);

        SAXParserFactory fact = SAXParserFactory.newInstance();
        SAXParser parser = fact.newSAXParser();

        XMLHandler handler = new XMLHandler();
        handler.spec = spec;

        InputStream inStream = null;
        try {
            inStream = url.openStream();
            parser.parse(inStream, handler);
            return spec;
        } finally {
            if(inStream != null) {
                inStream.close();
                inStream = null;
            }
        }
    }
    
    private static class ExecutorThread extends Thread implements Executor{
    	
    	private final List<Runnable> commands = new ArrayList<>();
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
					synchronized(this.commands) {
						if(this.commands.isEmpty()) {
							if(noMoreExpected)
								break;
							this.commands.wait();
						}
					}
					List<Runnable> commands = new ArrayList<>();
					synchronized(this.commands) {
						commands.addAll(this.commands);
						this.commands.clear();
					}
					for (Runnable runnable : commands) {
						try {
							runnable.run();
						} catch (Throwable e) {
							if(e instanceof InterruptedException)
								throw e;
							java.util.logging.Logger.getLogger("io.qt.internal").log(java.util.logging.Level.SEVERE, "", e);
						}
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
    
    private static LibraryEntry.ExtractionFunction getLibraryExtractor(String urlBase, String libName, File outFile, boolean executable, boolean isDebug, boolean isQtLib, int... version){
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
			case Linux:
				prefix = "libQt"+QtJambi_LibraryUtilities.qtMajorVersion;
				int dot = qtLibName.indexOf('.');
				if(dot>0) {
					qtLibName = qtLibName.substring(0, dot);
				}else {
					qtLibName = null;
				}
				break;
			case MacOSX:
				if(!libName.contains(".framework/")) {
					prefix = "libQt"+QtJambi_LibraryUtilities.qtMajorVersion;
					dot = qtLibName.indexOf('.');
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
				break;
	        }
    	}
		List<Dependency> dependencies = qtLibName==null ? Collections.emptyList() : QtJambi_LibraryUtilities.getDependencies().getOrDefault("Qt"+qtLibName, Collections.emptyList());
		
    	return ()->{
	    	URL entryURL = new URL(urlBase+libName);
	        try(InputStream in = entryURL.openStream()){
	        	File outFileDir = outFile.getParentFile();
	            reporter.report(" - copying '", libName, "' to ", outFile.getAbsolutePath());
	            if (!outFileDir.exists()) {
	                reporter.report(" - creating directory: ", outFileDir.getAbsolutePath());
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
        		        	LibraryEntry qmlLibrary = libraryMap.get(library);
        		        	while(qmlLibrary instanceof SymlinkEntry) {
        			        	String target = ((SymlinkEntry)qmlLibrary).getTarget();
        			        	if(target.startsWith("lib/") || target.startsWith("bin/")) {
        			        		target = target.substring(4);
        			        	}
        			        	LibraryEntry eTarget = libraryMap.get(target);
        			        	if(eTarget==null)
        			        		break;
        			        	else
        			        		qmlLibrary = eTarget;
        			        }
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
    
    private static DeploymentSpec unpackDeploymentSpec(URL deploymentSpec, String jarName, Boolean shouldUnpack) throws ParserConfigurationException, SAXException, IOException{
        if(jarName!=null)
        	reporter.report("Unpacking .jar file: '", (jarName==null ? deploymentSpec.toString() : jarName), "'");

        DeploymentSpec spec = null;
        try {
			spec = readDeploySpec(deploymentSpec);
        } catch (java.util.zip.ZipException e1) {
		} catch (DeploymentSpecException e1) {
			Logger.getLogger("io.qt.internal").warning(String.format("Unable to load native libraries from %1$s: %2$s", (jarName==null ? deploymentSpec : jarName), e1.getMessage()));
		}
        if (spec==null)
            return null;
        
        boolean isDebug = "debug".equalsIgnoreCase(spec.getConfiguration());
        boolean isQtLib = spec.getModule()!=null && spec.getModule().startsWith("qt.lib.");
        boolean isQtQml = spec.getModule()!=null && spec.getModule().startsWith("qt.qml.");
        boolean isQtPlugin = spec.getModule()!=null && spec.getModule().startsWith("qt.plugin.");
        
        File tmpDir = jambiDeploymentDir;
        if((isQtLib || isQtQml || isQtPlugin) && !deleteTmpDeployment) {
    		tmpDir = new File(jambiDeploymentDir, "Qt"+spec.getVersion());
        }
        spec.setBaseDir(tmpDir);
        spec.setBaseUrl(new URL(convertAbsolutePathStringToFileUrlString(tmpDir)));

        File dummyFile = null;
        if(shouldUnpack == null) {
        	if(spec.getModule()!=null) {
        		shouldUnpack = Boolean.TRUE;
        	}else {
	            // If the dir exists and contains .dummy, sanity check the contents...
	            dummyFile = new File(tmpDir, ".dummy");
	            if(spec.getModule()!=null)
	            	dummyFile = new File(tmpDir, "."+spec.getModule()+".dummy");
	            if (dummyFile.exists()) {
	                reporter.report(" - cache directory exists");
	                shouldUnpack = Boolean.FALSE;
	            } else {
	                shouldUnpack = Boolean.TRUE;
	            }
        	}
        }

        // If the dir doesn't exists or it was only half completed, copy the files over...
        // FIXME: This should be a separate API call to cause loading, we want to be able to load up multiple DeploymentSpec
        //  so we can choose which one we are going to activate (and allow enumeration of them).
        if (shouldUnpack.booleanValue()) {
            reporter.report(" - prepare library extraction...");
            
            String urlBase = deploymentSpec.toString();
            int idx = urlBase.indexOf("!/");
            if(idx>0) {
            	urlBase = urlBase.substring(0, idx+2);
            }

            DeploymentSpec _spec = spec;
            final String _urlBase = urlBase;
            LibVersion specVersion;
            try {
				String[] versionString = _spec.getVersion().split("\\.");
				specVersion = new LibVersion(Integer.parseInt(versionString[0]), Integer.parseInt(versionString[1]), Integer.parseInt(versionString[2]));
			} catch (Exception e2) {
				specVersion = new LibVersion(QtJambi_LibraryUtilities.qtMajorVersion, QtJambi_LibraryUtilities.qtMinorVersion, QtJambi_LibraryUtilities.qtJambiPatch);
			}
        	
            List<LibraryEntry.ExtractionFunction> qmlExtractionFunctions = new ArrayList<>();
            List<LibraryEntry.ExtractionFunction> utilExtractionFunctions = new ArrayList<>();
            for(QPair<String,Boolean> pair : _spec.getDirents()) {
                File outFile = new File(tmpDir, pair.first.replace('/', File.separatorChar));
                if(!outFile.exists()) {
                	LibraryEntry.ExtractionFunction extractor = getLibraryExtractor(_urlBase, pair.first, outFile, Boolean.TRUE.equals(pair.second), isDebug, false, specVersion.qtMajorVersion, specVersion.qtMinorVersion, specVersion.qtJambiPatch);
                	if(pair.first.startsWith("qml/")) {
                		qmlExtractionFunctions.add(extractor);
                	}else {
                		utilExtractionFunctions.add(extractor);
                	}
                }
            }
            
            Map<String,LibraryEntry> entries = new TreeMap<>();
            
            ExecutorThread executor = null;
            if("qtjambi".equals(spec.getModule()) || "qt.lib.core".equals(spec.getModule())) {
            	executor = new ExecutorThread(spec.getModule());
            	executor.start();
            }
            
            try {
            	List<LibraryEntry> libraries = spec.getLibraries();
            	LibraryEntry libraryEntry = null;
            	if(isQtLib) {
            		if(!libraries.isEmpty() && !(libraries.get(0) instanceof SymlinkEntry)) {
            			libraries = new ArrayList<>(libraries);
            			libraryEntry = libraries.remove(0);
            			entries.put(libraryEntry.getName(), libraryEntry);
	            		String libName = libraryEntry.getName();
	                    File outFile = new File(tmpDir, libName.replace('/', File.separatorChar));
	                    
	                    if(!outFile.exists()) {
	                		LibraryEntry.ExtractionFunction extractLibrary = getLibraryExtractor(_urlBase, libName, outFile, false, isDebug, isQtLib, specVersion.qtMajorVersion, specVersion.qtMinorVersion, specVersion.qtJambiPatch);
	                		if(executor!=null) {
	                    		CompletableFuture<Throwable> future = CompletableFuture.supplyAsync(()->{
	        	                    try {
		                    			extractLibrary.extract();
		                    			List<LibraryEntry.ExtractionFunction> _utilExtractionFunctions;
		                    			synchronized(utilExtractionFunctions) {
			                    			_utilExtractionFunctions = new ArrayList<>(utilExtractionFunctions);
			                    			utilExtractionFunctions.clear();
		                    			}
		                    			for(LibraryEntry.ExtractionFunction extr : _utilExtractionFunctions) {
		                    				extr.extract();
		                    			}
	        	                    } catch (Throwable e1) {
	        	                    	return e1;
	        	                    }
	    	                        return null;
	                        	}, executor);
	                    		libraryEntry.addExtractionFunction(()->{
	                        		Throwable exn = future.get();
	                        		if(exn!=null)
	                        			throw exn;
	                        	});
	                    	}else {
	                    		libraryEntry.addExtractionFunction(extractLibrary);
	                    		for(LibraryEntry.ExtractionFunction extr : utilExtractionFunctions) {
	                    			libraryEntry.addExtractionFunction(extr);
	                    		}
	                    	}
	                    	List<String> qmlLibraries = spec.qmlLibraries();
                    		if(!qmlLibraries.isEmpty()) {
                    			libraryEntry.addQmlExtractionFunction(()->{
                    				for(String lib : qmlLibraries) {
                                    	if(lib.startsWith("lib/") || lib.startsWith("bin/"))
                                    		lib = lib.substring(4);
                    					LibraryEntry qmlLibrary = libraryMap.get(lib);
                    					while(qmlLibrary instanceof SymlinkEntry) {
                    			        	String target = ((SymlinkEntry)qmlLibrary).getTarget();
                    			        	if(target.startsWith("lib/") || target.startsWith("bin/")) {
                    			        		target = target.substring(4);
                    			        	}
                    			        	LibraryEntry eTarget = libraryMap.get(target);
                    			        	if(eTarget==null)
                    			        		break;
                    			        	else
                    			        		qmlLibrary = eTarget;
                    			        }
                    					if(qmlLibrary!=null && !qmlLibrary.isLoaded() && qmlLibrary.isExtracting()) {
                    						qmlLibrary.extract();
                    						if(qmlLibrary.isQmlExtracting()) {
                		            			qmlLibrary.extractQml();
                    		            	}
                    					}
                    				}
                    			});
                    		}
                    		for(LibraryEntry.ExtractionFunction extr : qmlExtractionFunctions) {
                    			libraryEntry.addQmlExtractionFunction(extr);
                    		}
	                    }
            		}
            	}
	            for (LibraryEntry e : libraries) {
	            	entries.put(e.getName(), e);
	            	if(!(e instanceof SymlinkEntry)) {
	            		String libName = e.getName();
	                    File outFile = new File(tmpDir, libName.replace('/', File.separatorChar));
	                    if(!outFile.exists()) {
	                    	boolean isQtJambiPlugin = spec.getModule()!=null && spec.getModule().startsWith("qtjambi.plugin.") && libName.startsWith("plugins/");
	                		if(isQtJambiPlugin || isQtPlugin || isQtQml) {
	                			outFile.getParentFile().mkdirs();
	                			pluginLibraries.add(e);
	                		}
	                		LibraryEntry.ExtractionFunction extractLibrary = getLibraryExtractor(_urlBase, libName, outFile, false, isDebug, 
	                				libName.startsWith("lib/libQt"+QtJambi_LibraryUtilities.qtMajorVersion)
	                				|| (libName.startsWith("lib/Qt") && !libName.startsWith("lib/QtJambi"))
	                				|| libName.startsWith("bin/Qt"+QtJambi_LibraryUtilities.qtMajorVersion), 
	                				specVersion.qtMajorVersion, specVersion.qtMinorVersion, specVersion.qtJambiPatch);
	                		if(libraryEntry!=null) {
	                			if(libName.startsWith("qml/")) {
	                				libraryEntry.addQmlExtractionFunction(extractLibrary);
	                			}else {
	                				libraryEntry.addExtractionFunction(extractLibrary);
	                			}
	                			e.addExtractionFunction(extractLibrary);
	                		}else if(executor!=null
	                    			&& ((!libName.contains("QtJambiGui")
	                            			&& !libName.contains("QtJambiWidgets"))
	                            			|| isQtJambiPlugin)) {
	                    		CompletableFuture<Throwable> future = CompletableFuture.supplyAsync(()->{
	        	                    try {
		                    			extractLibrary.extract();
		                    			List<LibraryEntry.ExtractionFunction> _utilExtractionFunctions;
		                    			synchronized(utilExtractionFunctions) {
			                    			_utilExtractionFunctions = new ArrayList<>(utilExtractionFunctions);
			                    			utilExtractionFunctions.clear();
		                    			}
		                    			for(LibraryEntry.ExtractionFunction extr : _utilExtractionFunctions) {
		                    				extr.extract();
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
	                    		e.addExtractionFunction(extractLibrary);
	                    		for(LibraryEntry.ExtractionFunction extr : utilExtractionFunctions) {
	                    			e.addExtractionFunction(extr);
	                    		}
	                    	}
                    		for(LibraryEntry.ExtractionFunction extr : qmlExtractionFunctions) {
                    			e.addQmlExtractionFunction(extr);
                    		}
		            	}
	            	}
	            }
	            List<SymlinkEntry> shiftedLinks = new ArrayList<>();
	            for (LibraryEntry e : libraries) {
	            	if(e instanceof SymlinkEntry) {
		        		File outFile = new File(tmpDir, e.getName().replace('/', File.separatorChar));
		        		if(!outFile.exists()) {
		            		SymlinkEntry s = (SymlinkEntry)e;
		                    File outFileDir = outFile.getParentFile();
		                    File target = new File(tmpDir, s.getTarget().replace('/', File.separatorChar));
		                    if(target.exists()) {
			                    if (!outFileDir.exists()) {
			                        reporter.report(" - creating directory: ", outFileDir.getAbsolutePath());
			                        outFileDir.mkdirs();
			                    }
		                    	reporter.report(" - creating symbolic link ", outFile.getAbsolutePath());
		                    	Files.createSymbolicLink(outFile.toPath(), outFile.getParentFile().toPath().relativize(target.toPath()));
		                    }else {
	                    		if(e.getName().startsWith("qml/")) {
		                    		LibraryEntry.ExtractionFunction linker = ()->{
		                    			if(!outFile.exists()) {
			    		                    if (!outFileDir.exists()) {
			    		                        reporter.report(" - creating directory: ", outFileDir.getAbsolutePath());
			    		                        outFileDir.mkdirs();
			    		                    }
				                    		reporter.report(" - creating symbolic link ", outFile.getAbsolutePath());
					                    	Files.createSymbolicLink(outFile.toPath(), outFile.getParentFile().toPath().relativize(target.toPath()));
		                    			}
				                    	s.extractQml();
			                    	};
			                    	if(libraryEntry!=null) {
		                				libraryEntry.addQmlExtractionFunction(linker);
			                    	}else {
				                    	LibraryEntry linkedEntry = entries.get(s.getTarget());
				                    	if(linkedEntry!=null) {
			                    			linkedEntry.addQmlExtractionFunction(linker);
				                    	}else {
				                    		shiftedLinks.add(s);
				                    	}
			                    	}
	                			}else {
		                    		LibraryEntry.ExtractionFunction linker = ()->{
		                    			if(!outFile.exists()) {
			    		                    if (!outFileDir.exists()) {
			    		                        reporter.report(" - creating directory: ", outFileDir.getAbsolutePath());
			    		                        outFileDir.mkdirs();
			    		                    }
				                    		reporter.report(" - creating symbolic link ", outFile.getAbsolutePath());
					                    	Files.createSymbolicLink(outFile.toPath(), outFile.getParentFile().toPath().relativize(target.toPath()));
		                    			}
				                    	s.extract();
			                    	};
			                    	if(libraryEntry!=null) {
		                				libraryEntry.addExtractionFunction(linker);
			                    	}else {
				                    	LibraryEntry linkedEntry = entries.get(s.getTarget());
				                    	if(linkedEntry!=null) {
			                				linkedEntry.addExtractionFunction(linker);
				                    	}else {
				                    		shiftedLinks.add(s);
				                    	}
			                    	}
	                			}
		                    }
	            		}
	            	}
	            }
	            if(!shiftedLinks.isEmpty()) {
	            	List<SymlinkEntry> _shiftedLinks = new ArrayList<>();
	                while(!shiftedLinks.isEmpty()) {
	                	_shiftedLinks.clear();
	                	for (SymlinkEntry s : shiftedLinks) {
	                		File outFile = new File(tmpDir, s.getName().replace('/', File.separatorChar));
	                		if(!outFile.exists()) {
		                        File outFileDir = outFile.getParentFile();
		                        File target = new File(tmpDir, s.getTarget().replace('/', File.separatorChar));
		                        if(target.exists()) {
			                        if (!outFileDir.exists()) {
			                            reporter.report(" - creating directory: ", outFileDir.getAbsolutePath());
			                            outFileDir.mkdirs();
			                        }
		                        	reporter.report(" - creating symbolic link ", outFile.getAbsolutePath());
		                        	Files.createSymbolicLink(outFile.toPath(), outFile.getParentFile().toPath().relativize(target.toPath()));
		                        }else {
		                        	LibraryEntry linkedEntry = entries.get(s.getTarget());
			                    	if(linkedEntry!=null) {
			                    		if(s.getName().startsWith("qml/")) {
				                    		LibraryEntry.ExtractionFunction linker = ()->{
				                    			if(!outFile.exists()) {
							                        if (!outFileDir.exists()) {
							                            reporter.report(" - creating directory: ", outFileDir.getAbsolutePath());
							                            outFileDir.mkdirs();
							                        }
						                    		reporter.report(" - creating symbolic link ", outFile.getAbsolutePath());
							                    	Files.createSymbolicLink(outFile.toPath(), outFile.getParentFile().toPath().relativize(target.toPath()));
				                    			}
						                    	s.extractQml();
					                    	};
			                				linkedEntry.addQmlExtractionFunction(linker);
			                			}else {
				                    		LibraryEntry.ExtractionFunction linker = ()->{
				                    			if(!outFile.exists()) {
					                    			if (!outFileDir.exists()) {
							                            reporter.report(" - creating directory: ", outFileDir.getAbsolutePath());
							                            outFileDir.mkdirs();
							                        }
						                    		reporter.report(" - creating symbolic link ", outFile.getAbsolutePath());
							                    	Files.createSymbolicLink(outFile.toPath(), outFile.getParentFile().toPath().relativize(target.toPath()));
				                    			}
						                    	s.extract();
					                    	};
			                				linkedEntry.addExtractionFunction(linker);
			                			}
			                    	}else {
			                    		_shiftedLinks.add(s);
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
	                throw new DeploymentSpecException("Can't create dummy file in cache directory");
	            }
	            spec.setBaseDir(tmpDir);
	            spec.setBaseUrl(new URL(convertAbsolutePathStringToFileUrlString(tmpDir)));
            }finally {
            	if(executor!=null)
            		executor.setNoMoreExpected();
            }
        } else if(spec.getBaseUrl() == null) {
            String path = deploymentSpec.getPath();
            int i = path.lastIndexOf('/');  // URL path
            if(i >= 0)
                path = path.substring(0, i);
            spec.setBaseDir(new File(path));
            spec.setBaseUrl(new URL(deploymentSpec, path));
        }

        return spec;
    }

    private static boolean loadSystemLibrary(String name) {
        reporter.report(" - trying to load: ", name);

        File foundFile = null;
        for(DeploymentSpec deploymentSpec : deploymentSpec) {
            File f = new File(deploymentSpec.getBaseDir(), name);
            if(f.isFile()) {
                foundFile = f;
                break;
            }
        }
        if(foundFile == null)
            return false;

        Runtime rt = Runtime.getRuntime();
        rt.load(foundFile.getAbsolutePath());
        reporter.report(" - ok!");
        return true;
    }

    private static String qtjambiLibraryName(String prefix, String lib, Configuration configuration, List<String> replacements, int... version) {
        if(prefix!=null)
        	lib = prefix + lib;
    	replacements.clear();
        switch (operatingSystem) {
        case Windows: {
        	if(version.length>0)
        		replacements.add(""+version[0]);
        	replacements.add("");
        	if (configuration == Configuration.Debug)
                lib += "d";
    		return lib + "%1$s.dll";  // "foobar1.dll"
        }
        case MacOSX: 
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
        	if (configuration == Configuration.Debug)
                lib += "_debug";
        	return "lib" + lib + "%1$s.jnilib";
        case Linux:
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
        	if (configuration == Configuration.Debug)
        		return "lib" + lib + "_debug.so%1$s";
        	else return "lib" + lib + ".so%1$s";
		case Android: 
			if (configuration == Configuration.Debug)
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
			break;
        }
        throw new RuntimeException("Unreachable statement");
    }


    private static String qtLibraryName(String qtprefix, String libraryName, String libInfix, String versionStrg, Configuration configuration, List<String> replacements, int... version) {
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
        switch (operatingSystem) {
        case Windows:
        	if(version!=null && version.length>0 && !isQt){
        		replacements.add(""+version[0]);
        		replacements.add("");
	            return configuration == Configuration.Debug && !isMinGWBuilt
		                ? qtXlibName + "d%1$s.dll"  // "QtFood4.dll"
		                : qtXlibName + "%1$s.dll";  // "QtFoo4.dll"
        	}else {
	            return configuration == Configuration.Debug && !isMinGWBuilt
	                ? qtXlibName + "d.dll"  // "QtFood4.dll"
	                : qtXlibName + ".dll";  // "QtFoo4.dll"
        	}
        case MacOSX:
        	if(dontUseFrameworks==null || !dontUseFrameworks) {
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
		            return configuration == Configuration.Debug
		                ? prefix + qtXlibName + "_debug%1$s.dylib"  // "libQtFoo_debug.4.dylib"
		                : prefix + qtXlibName + "%1$s.dylib";  // "libQtFoo.4.dylib"
	        	}
        	}
        case Linux:
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
			break;
        }
        throw new RuntimeException("Unreachable statement");
    }

    static boolean isUsingDeploymentSpec() {
        return !deploymentSpec.isEmpty();
    }

    private static String convertAbsolutePathStringToFileUrlString(String pathString) {
        if(File.separatorChar != '/')
            pathString = pathString.replace(File.separatorChar, '/');  // windows
        String schemePrefix;
        if(pathString.length() > 0 && pathString.charAt(0) != '/')
            schemePrefix = "file:///";  // empty authority part, ensure leading / in path part
        else
            schemePrefix = "file://";   // empty authority part, pathString already has leading /
        return schemePrefix + pathString;
    }

    private static String convertAbsolutePathStringToFileUrlString(File file) {
        return convertAbsolutePathStringToFileUrlString(file.getAbsolutePath());
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
                // FIXME: We want only append the suffix (no prefix, no Qt version, no debug extra)
                //  currently is does add a prefix (maybe also debug extra).
                NativeLibraryManager.loadSystemLibrary(s);
            }
        }
    }

    /**
     * Converts the string version number into an array of
     * @param versionString Version string such as "1.2.3"
     * @return Array of <code>new int[] { 1, 2, 3 };</code>
     * @throws NumberFormatException
     */
    static int[] getVersion(String versionString) {
        String[] sA = versionString.split("\\.");
        int[] vA = new int[sA.length];
        int i = 0;
        for(String s : sA)
            vA[i++] = Integer.valueOf(s);
        return vA;
    }

    /** 
     * Enum for defining the operation system.
     */
    enum OperatingSystem {
        Windows,
        MacOSX,
        Linux,
        Android
    }
    
    enum Architecture{
    	x86,
    	x86_64,
    	arm,
    	arm64
    }
    
    /** 
     * Enum for defining whether Qt is build in Release or Debug. 
     */
    enum Configuration {
        Release,
        Debug,
        Test;
    }
    
    static String qtJambiLibraryPath() {
		return qtJambiLibraryPath;
	}

	static String qtLibraryPath() {
		return qtLibraryPath;
	}

	static String osArchName() {
		return osArchName;
	}
}

//!!NOTE!! This class can have no dependencies on Qt since
//it is required for loading the libraries.


class DeploymentSpecException extends RuntimeException {
 private static final long serialVersionUID = 1L;

 public DeploymentSpecException(String msg) {
     super(msg);
 }
}

class WrongSystemException extends DeploymentSpecException {
 private static final long serialVersionUID = 1L;

 public WrongSystemException(String msg) {
     super(msg);
 }
}

class WrongVersionException extends DeploymentSpecException {
	 private static final long serialVersionUID = 1L;

	 public WrongVersionException(String msg) {
	     super(msg);
	 }
}
