/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2021 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.URL;
import java.nio.file.Files;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.Enumeration;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.SortedMap;
import java.util.TreeMap;
import java.util.function.BiConsumer;
import java.util.logging.Level;
import java.util.logging.Logger;
import java.util.prefs.Preferences;

import javax.xml.parsers.ParserConfigurationException;
import javax.xml.parsers.SAXParser;
import javax.xml.parsers.SAXParserFactory;

import org.xml.sax.SAXException;
import org.xml.sax.helpers.DefaultHandler;

import io.qt.QNoSuchSlotException;
import io.qt.core.QDeclarableSignals;

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

    private static final boolean VERBOSE_LOADING = Boolean.getBoolean("io.qt.verbose-loading") || Boolean.getBoolean("qtjambi.verbose-loading");

    private static final int[] versionA = {QtJambiVersion.qtMajorVersion, QtJambiVersion.qtMinorVersion};
    public static final String ICU_VERSION;
    public static final String LIBINFIX;
    // We use a List<> to make the collection read-only an array would not be suitable
    private static final List<String> systemLibrariesList;
    private static final List<String> jniLibdirBeforeList;
    private static final List<String> jniLibdirList;
    private static boolean isMinGWBuilt = false;
    
    private static final OperatingSystem operatingSystem = decideOperatingSystem();
    private static final String osArchName;
    
    private static Configuration configuration = null;
    private static Boolean dontUseFrameworks = operatingSystem==OperatingSystem.MacOSX ? null : Boolean.FALSE;
    
    private static final List<DeploymentSpec> deploymentSpec = new ArrayList<>();

    private static final Map<String, LibraryEntry> libraryMap = new HashMap<String, LibraryEntry>();
    private static final Reporter reporter = new Reporter();
    private static final File jambiTempDir;

    public static final String K_SUNOS_X86 = "sunos-x86";
    public static final String K_SUNOS_X64 = "sunos-x64";

    public static final String K_WIN_X86 = "windows-x86";
    public static final String K_WIN_X64 = "windows-x64";
    public static final String K_WIN_ARM32 = "windows-arm32";
    public static final String K_WIN_ARM64 = "windows-arm64";

    public static final String K_LINUX_X86 = "linux-x86";
    public static final String K_LINUX_X64 = "linux-x64";
    public static final String K_LINUX_ARM32 = "linux-arm32";
    public static final String K_LINUX_ARM64 = "linux-arm64";

    public static final String K_FREEBSD_X86 = "freebsd-x86";
    public static final String K_FREEBSD_X64 = "freebsd-x64";
    
    static {
        File tmpDir = new File(System.getProperty("java.io.tmpdir"));
        jambiTempDir = new File(tmpDir, "QtJambi_" + System.getProperty("user.name") + "_" + RetroHelper.processName());
		
    	reporter.setReportEnabled(VERBOSE_LOADING);
        String tmpICUVERSION_STRING = null;
        String tmpINFIX_STRING = null;
        List<String> tmpSystemLibrariesList = null;
        List<String> tmpJniLibdirBeforeList = null;
        List<String> tmpJniLibdirList = null;
        try {
            tmpINFIX_STRING = QtJambiVersion.properties.getProperty("qtjambi.libinfix");
            tmpICUVERSION_STRING = QtJambiVersion.properties.getProperty("qtjambi.icu.version", "56");
            
            SortedMap<String,String> tmpSystemLibrariesMap = new TreeMap<String,String>();
            SortedMap<String,String> tmpJniLibdirBeforeMap = new TreeMap<String,String>();
            SortedMap<String,String> tmpJniLibdirMap = new TreeMap<String,String>();
            Enumeration<? extends Object> e = QtJambiVersion.properties.propertyNames();
            while (e.hasMoreElements()) {
                String key = (String) e.nextElement();
                String value = QtJambiVersion.properties.getProperty(key);
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
            java.util.logging.Logger.getLogger("io.qt").log(java.util.logging.Level.SEVERE, "", e);
        }finally {
        	ICU_VERSION = tmpICUVERSION_STRING;
            systemLibrariesList = tmpSystemLibrariesList;
            jniLibdirBeforeList = tmpJniLibdirBeforeList;
            jniLibdirList = tmpJniLibdirList;
            LIBINFIX = tmpINFIX_STRING;
        }

        switch (operatingSystem) {
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
        case Android:
            osArchName = "android";
            break;
        case MacOSX:
            osArchName = "macos";
            break;
        default:
            osArchName = "unknown";
            break;
        }
        
    	if(!(Boolean.getBoolean("io.qt.no-deployment-spec") || Boolean.getBoolean("qtjambi.no-deployment-spec"))) {
    		ClassLoader loader = classLoader();
            // Multiple descriptors maybe found
            Set<URL> specsUrls = new HashSet<>();
            Enumeration<URL> specsFound = Collections.emptyEnumeration();
            try {
				specsFound = loader.getResources("qtjambi-deployment.xml");
			} catch (IOException e) {
				Logger.getLogger("io.qt").log(Level.WARNING, "", e);
			}
            if(specsFound.hasMoreElements()) {
            	Preferences preferences = Preferences.userNodeForPackage(NativeLibraryManager.class);
        		String dir = preferences.get("qtjambi.previous.deployment.dir", null);
        		if(dir!=null) {
        	        preferences.remove("qtjambi.previous.deployment.dir");
        			File jambiTempDir = new File(dir);
        			if(jambiTempDir.exists() && jambiTempDir.isDirectory())
        				clearAndDelete(jambiTempDir);
        		}
            }
            // FIXME: Want searchForDescriptors/parse/resolve/unpack/load phases separated
            while (specsFound.hasMoreElements()) {
                URL url = specsFound.nextElement();
                
                if(specsUrls.contains(url))
                	continue;
                specsUrls.add(url);

                if (VERBOSE_LOADING)
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
	                        if (VERBOSE_LOADING)
	                            reporter.report("Loading ", jarName, " from ", eform);
	                        spec = unpackDeploymentSpec(url, jarName, null);
    					} catch (ParserConfigurationException | SAXException | IOException e) {
            				Logger.getLogger("io.qt").log(Level.WARNING, "", e);
            			}
                    }
                } else if (protocol.equals("file")) {
                    // No unpack since we presume we are already unpacked
                    try {
						spec = unpackDeploymentSpec(url, null, Boolean.FALSE);
					} catch (ParserConfigurationException | SAXException | IOException e) {
        				Logger.getLogger("io.qt").log(Level.WARNING, "", e);
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
            	for(DeploymentSpec spec : deploymentSpec) {
            		if(!spec.getCompiler().equals(qtjambiSpec.getCompiler())) {
            			throw new DeploymentSpecException(String.format("Native deployments of different builts: %1$s and %2$s", qtjambiSpec.getCompiler(), spec.getCompiler()));
            		}else if(!spec.getConfiguration().equals(qtjambiSpec.getConfiguration())) {
            			throw new DeploymentSpecException(String.format("Native deployments of different configurations: %1$s and %2$s", qtjambiSpec.getConfiguration(), spec.getConfiguration()));
            		}
            	}
            }else {
            	configuration = decideConfiguration();
            }
    	}else {
    		configuration = decideConfiguration();
    	}
    }
    
    private static void clearAndDelete(File directory) {
    	for(File file : directory.listFiles()) {
    		if(file.getName().equals(".") || file.getName().equals(".."))
    			continue;
    		if(file.isDirectory()) {
    			clearAndDelete(file);
    		}else {
    			file.delete();
    		}
    	}
    	directory.delete();
    }
    
 // The purpose of this method is to resolve the names provided in the list into DSO paths relative to the project.
    private static List<String> resolveSystemLibraries(List<String> tmpSystemLibrariesList) {
        if(tmpSystemLibrariesList == null)
            return null;
        List<String> resolvedList = new ArrayList<String>();
        for(String original : tmpSystemLibrariesList) {
            String s = original;
            if(operatingSystem==OperatingSystem.Windows)   // convert "/" into "\"
                s = stringCharReplace(s, "/", File.separator);

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
    
    private static String stringCharReplace(String s, CharSequence fromCharSequence, CharSequence toCharSequence) {
        final int len = s.length();
        StringBuilder sb = new StringBuilder();
        int i = 0;
        while(i < len) {
            char c = s.charAt(i);
            if(c == fromCharSequence.charAt(0) && stringCompareAt(s, i, fromCharSequence, 0)) {  // FIXME needle/haystack
                sb.append(toCharSequence);
                i += fromCharSequence.length();  // skip input sequence
            } else {
                sb.append(c);
                i++;
            }
        }
        return sb.toString();
    }

    // a - haystack, b - needle
    private static boolean stringCompareAt(CharSequence a, int aOffset, CharSequence b, int bOffset) {
        final int aLen = a.length();  // used as end range
        final int bLen = b.length();  // used as end range
        while(aOffset < aLen && bOffset < bLen) {
            char ca = a.charAt(aOffset++);
            char ba = b.charAt(bOffset++);
            if(ca != ba)
                return false;
        }
        // we are only allowed to run out of chars on 'a' side
        if(bOffset < bLen)
            return false;  // ran out of chars on 'b' side
        return true;
    }

    private static OperatingSystem decideOperatingSystem() {
        String osName = System.getProperty("os.name").toLowerCase();
        if (osName.startsWith("windows")) return OperatingSystem.Windows;
        if (osName.startsWith("mac")) return OperatingSystem.MacOSX;
        if (osName.startsWith("android")) return OperatingSystem.Android;
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
    
    static File jambiTempDir() {
        return jambiTempDir;
    }

    /**
     * Called during shutdown of the deployment spec to help clear
     * the active from static values.
     * @return
     */
    static void resetDeploymentSpecs() {
        deploymentSpec.clear();
        if(jambiTempDir.exists() && jambiTempDir.isDirectory()) {
        	clearAndDelete(jambiTempDir);
        	if(jambiTempDir.exists() && jambiTempDir.isDirectory()) {
        		Preferences preferences = Preferences.userNodeForPackage(NativeLibraryManager.class);
        		preferences.put("qtjambi.previous.deployment.dir", jambiTempDir.getAbsolutePath());
        	}
    	}
    }

    private static class XMLHandler extends DefaultHandler {
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
                
                if(!e.getName().startsWith("qml/") 
                		&& !e.getName().startsWith("translations/")
                		&& !e.getName().startsWith("plugins/")) {
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

                if(!e.getName().startsWith("qml/") 
                		&& !e.getName().startsWith("translations/")
                		&& !e.getName().startsWith("plugins/")) {
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

            } else if (name.equals("plugin")) {
                String path = attributes.getValue("path");
                if (path == null) {
                    throw new DeploymentSpecException("<plugin> element missing required attribute \"path\"");
                }
                spec.addPluginPath(path);
                reporter.report(" - plugin path='", path, "'");
            } else if (name.equals("plugin-designer")) {
                String path = attributes.getValue("path");
                if (path == null) {
                    throw new DeploymentSpecException("<plugin-designer> element missing required attribute \"path\"");
                }
                spec.addPluginDesignerPath(path);
                reporter.report(" - plugin-designer path='", path, "'");
            } else if (name.equals("qtjambi-deploy")) {
                String system = attributes.getValue("system");
                if (system == null || system.length() == 0) {
                    throw new DeploymentSpecException("<qtjambi-deploy> element missing required attribute 'system'");
                } else if (!system.equals(osArchName)) {
                    throw new WrongSystemException("trying to load: '" + system
                                                   + "', expected: '" + osArchName + "'");
                }
                spec.setSystem(system);
                String version = attributes.getValue("version");
                if(!version.equals(QtJambiVersion.qtMajorVersion + "." + QtJambiVersion.qtMinorVersion + "." + QtJambiVersion.qtJambiPatch)) {
                    throw new WrongVersionException("trying to load: '" + version
                            + "', expected: '" + QtJambiVersion.qtMajorVersion + "." + QtJambiVersion.qtMinorVersion + "." + QtJambiVersion.qtJambiPatch + "'");                	
                }
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
                spec.addDirentPath(sb.toString());
                reporter.report(" - dirent path='", sb.toString(), "'");
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
        List<String> paths = new ArrayList<String>();
        for (DeploymentSpec spec : deploymentSpec) {
            File root = spec.getBaseDir();
            List<String> pluginPaths = spec.getPluginPaths();
            for (String path : pluginPaths)
                paths.add(new File(root, path).getAbsolutePath());
        }
        return paths;
    }


    /**
     * Returns the list of all plugin-designer paths that are specified in the
     * deployment descriptors. If deployment descriptors are not used,
     * this list will be an empty list.
     *
     * @return The list of plugin paths
     */
    static List<String> pluginDesignerPaths() {
        List<String> paths = new ArrayList<String>();
        for (DeploymentSpec spec : deploymentSpec) {
            File root = spec.getBaseDir();
            List<String> pluginDesignerPaths = spec.getPluginDesignerPaths();
            for (String path : pluginDesignerPaths)
                paths.add(new File(root, path).getAbsolutePath());
        }
        return paths;
    }
    
    static void loadLibrary(String library) {
    	switch (operatingSystem) {
        case Windows: 
        	loadNativeLibrary(Object.class, library + ".dll", Arrays.asList(""));
        	break;
        case MacOSX: 
        	try {
				loadNativeLibrary(Object.class, "lib" + library + ".jnilib", Arrays.asList(""));
			} catch (Throwable e) {
				loadNativeLibrary(Object.class, "lib" + library + ".dylib", Arrays.asList(""));
			}
        	break;
        case Android:
        case Linux:
        	loadNativeLibrary(Object.class, "lib" + library + ".so", Arrays.asList(""));
        	break;
		default:
			break;
        }
    }
    
    static void loadQtJambiLibrary() {
    	loadQtJambiLibrary(NativeLibraryManager.class, null, "QtJambi");
    }

    static void loadQtJambiLibrary(Class<?> callerClass, String library) {
    	loadQtJambiLibrary(callerClass, "QtJambi", library);
    }
    
    static void loadJambiLibrary(Class<?> callerClass, String library) {
    	loadQtJambiLibrary(callerClass, null, library);
    }

    private static void loadQtJambiLibrary(Class<?> callerClass, String prefix, String library) {
    	List<String> replacements = new ArrayList<>();
    	String lib = jambiLibraryName(prefix, library, versionA, replacements);
        loadNativeLibrary(callerClass, lib, replacements);
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
    static void loadQtLibrary(String library) {
    	findAndLoadLibrary(Object.class, "Qt", library, LIBINFIX, versionA);
    }
    
    static void loadUtilityLibrary(String library, String version) {
    	findAndLoadLibrary(Object.class, library, version);
    }
    
    static File loadQtCore() {
    	loadSystemLibraries();
        if(isAvailableLibrary("icudata", NativeLibraryManager.ICU_VERSION)){
        	findAndLoadLibrary(Object.class, "icudata", NativeLibraryManager.ICU_VERSION);
    	}
    	if(isAvailableLibrary("icuuc", NativeLibraryManager.ICU_VERSION)){
    		findAndLoadLibrary(Object.class, "icuuc", NativeLibraryManager.ICU_VERSION);
    	}
    	if(isAvailableLibrary("icui18n", NativeLibraryManager.ICU_VERSION)){
    		findAndLoadLibrary(Object.class, "icui18n", NativeLibraryManager.ICU_VERSION);
    	}
    	try{
    		return findAndLoadLibrary(Object.class, "Qt", "Core", LIBINFIX, versionA);
		} catch (RuntimeException e) {
			if(configuration==Configuration.Release && deploymentSpec.isEmpty()) {
				// try to run qtjambi with debug libraries instead
				configuration=Configuration.Debug;
				try {
					return findAndLoadLibrary(Object.class, "Qt", "Core", LIBINFIX, versionA);
				} catch (Throwable e1) {
					e.addSuppressed(e1);
					configuration=Configuration.Release;
					if(dontUseFrameworks==null) {
						dontUseFrameworks = Boolean.TRUE;
						try {
							return findAndLoadLibrary(Object.class, "Qt", "Core", LIBINFIX, versionA);
						} catch (Throwable e2) {
							e.addSuppressed(e2);
							configuration=Configuration.Debug;
							try {
								return findAndLoadLibrary(Object.class, "Qt", "Core", LIBINFIX, versionA);
							} catch (Throwable e3) {
								e.addSuppressed(e3);
								configuration=Configuration.Release;
								throw e;
							}
						}
					}else{
						throw e;
					}
				}
			}else {
				if(dontUseFrameworks==null) {
					dontUseFrameworks = Boolean.TRUE;
					try {
						return findAndLoadLibrary(Object.class, "Qt", "Core", LIBINFIX, versionA);
					} catch (Throwable e1) {
						e.addSuppressed(e1);
						throw e;
					}
				}else{
					if(!isMinGWBuilt && operatingSystem==OperatingSystem.Windows) {
						Availability availability = getLibraryAvailability(null, "libstdc++-6", null, null, null, Configuration.Release);
				    	if(availability.entry!=null){
				    		isMinGWBuilt = true;
				    	}else if(availability.file!=null){
				    		isMinGWBuilt = new File(availability.file.getParentFile(), "Qt5Core.dll").isFile();
				    	}
				    	if(isMinGWBuilt) {
				    		return findAndLoadLibrary(Object.class, "Qt", "Core", LIBINFIX, versionA);
				    	}
			    	}
					throw e;
				}
			}
		}finally {
			if(dontUseFrameworks==null) {
				dontUseFrameworks = Boolean.FALSE;
			}
		}
    }
    
    private static File findAndLoadLibrary(Class<?> callerClass, String library, String version) {
    	List<String> replacements = new ArrayList<>();
        String lib = qtLibraryName(null, library, null, null, version, configuration, replacements);
		return loadNativeLibrary(callerClass, lib, replacements);
    }
    
    private static File findAndLoadLibrary(Class<?> callerClass, String qtprefix, String library, String libInfix, int[] version) {
    	List<String> replacements = new ArrayList<>();
        String lib = qtLibraryName(qtprefix, library, libInfix, version, null, configuration, replacements);
		return loadNativeLibrary(callerClass, lib, replacements);
    }
    
    static boolean isAvailableQtLibrary(String library) {
    	return getLibraryAvailability("Qt", library, LIBINFIX, versionA, null, configuration).isAvailable();
    }
    
    static boolean isAvailableLibrary(String library, String version) {
    	return getLibraryAvailability(null, library, null, null, version, configuration).isAvailable();
    }
    
    private static class Availability{
    	public Availability(File file) {
			super();
			this.file = file;
			this.entry = null;
		}
    	public Availability() {
			super();
			this.file = null;
			this.entry = null;
		}
    	public Availability(LibraryEntry entry) {
			super();
			this.file = null;
			this.entry = entry;
		}
		final File file;
		final LibraryEntry entry;
		boolean isAvailable() {
			return file!=null || entry!=null;
		}
    }

    private static Availability getLibraryAvailability(String qtprefix, String library, String libInfix, int[] version, String versionStrg, Configuration configuration) {
    	List<String> replacements = new ArrayList<>();
    	String libFormat = qtLibraryName(qtprefix, library, libInfix, version, versionStrg, configuration, replacements);  // "QtDBus" => "libQtDBus.so.4"
    	String libPaths = System.getProperty("io.qt.library-path-override");
        if (libPaths == null || libPaths.length() == 0) {
            libPaths = System.getProperty("java.library.path");
        }

        List<String> paths = new ArrayList<>();
        if (libPaths != null)
        	paths.addAll(Arrays.asList(libPaths.split("\\"+File.pathSeparator)));
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
            paths.addAll(Arrays.asList(libPaths.split("\\"+File.pathSeparator)));
        paths = mergeJniLibdir(paths);
        // search active deploymentSpec for existance of library
        for(String replacement : replacements) {
        	String lib = String.format(libFormat, replacement);
	        for(DeploymentSpec deploymentSpec : deploymentSpec) {
	            List<LibraryEntry> libraries = deploymentSpec.getLibraries();
	            for(LibraryEntry libraryEntry : libraries) {
	                String name = libraryEntry.getName();  // name="lib/libQtFoo.so.4"
	                if(name == null)
	                    continue;
	                if(lib.equals(name))      // lib=="lib/libQtFoo.so.4"
	                    return new Availability(libraryEntry);
	                String[] pathA = name.split("\\/");
	                if(pathA == null || pathA.length == 0)
	                    continue;
	                String lastPart = pathA[pathA.length - 1];
	                if(lib.equals(lastPart))  // lib=="libQtFoo.so.4"
	                	return new Availability(libraryEntry);
	            }
	        }
	        for (String path : paths) {
	            File f = new File(path, lib);
	            if (f.exists()) {
	            	return new Availability(f);
	            }
	        }
        }
        return new Availability();
    }

    static List<String> unpackPlugins() {
        if(deploymentSpec != null) {
            List<String> paths = new ArrayList<String>();

            // Merge pluginPaths
            for(DeploymentSpec spec : deploymentSpec) {
                List<String> pluginPaths = spec.getPluginPaths();
                for(String p : pluginPaths) {
                    File resolvedFile = spec.buildPath(p);
                    // Checking it really exists as a directory (before adding it to the list)
                    //  is kind of a security check.
                    if(resolvedFile != null && resolvedFile.isDirectory()) {
                        paths.add(resolvedFile.getAbsolutePath());
                    }
                }
            }

            // Merge pluginDesignerPaths
            for(DeploymentSpec spec : deploymentSpec) {
                List<String> pluginDesignerPaths = spec.getPluginDesignerPaths();
                for(String p : pluginDesignerPaths) {
                    File resolvedFile = spec.buildPath(p);
                    // Checking it really exists as a directory (before adding it to the list)
                    //  is kind of a security check.
                    if(resolvedFile != null && resolvedFile.isDirectory()) {
                        paths.add(resolvedFile.getAbsolutePath());
                    }
                }
            }

            return paths;
        }
        return null;
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
    private static File loadNativeLibrary(Class<?> callerClass, String lib, List<String> replacements) {
    	File result = null;
        try {
        	result = loadLibrary_helper(callerClass, lib, replacements);
            if (VERBOSE_LOADING)
                System.out.println(reporter.recentReports());
        } catch (RuntimeException | Error e) {
    		throw e;
        } catch (Throwable e) {
    		if(reporter.toString().isEmpty()) {
        		if(e.getMessage().isEmpty()) {
	        		throw new RuntimeException("Loading library failed.", e);
            	}else {
            		throw new RuntimeException(e.getMessage(), e);
	        	}
        	}else {
    			throw new RuntimeException("Loading library failed. Progress so far: " + reporter, e);
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
					loadLibrary.connect(Runtime.getRuntime(), "load0(Class, String)");
					libraryLoader = loadLibrary::emit;
				} catch (QNoSuchSlotException e) {
					libraryLoader = (_callerClass, _lib)->Runtime.getRuntime().load(_lib);
				}
    		}
    		libraryLoader.accept(callerClass, lib);
    	};
    }

    private static File loadLibrary_helper(Class<?> callerClass, String libFormat, List<String> replacements) {
    	File result = null;
        ClassLoader callerClassLoader = callerClass.getClassLoader();
        String libPaths = System.getProperty("io.qt.library-path-override");
        if (libPaths != null && libPaths.length() > 0) {
            reporter.report(" - using 'io.qt.library-path-override'");
        } else {
            reporter.report(" - using 'java.library.path'");
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
            libraryPaths.addAll(Arrays.asList(libPaths.split("\\"+File.pathSeparator)));
        libraryPaths = mergeJniLibdir(libraryPaths);
        
        boolean loaded = false;
        for(String replacement : replacements) {
        	String library = String.format(libFormat, replacement);
	        reporter.report("Loading library: '", library, "'...");
	
	        LibraryEntry e;
	        // Try to load via deploy spec...
	        e = libraryMap.get(library);
	        if (e != null) {
	            if (e.isLoaded()) {
	                reporter.report(" - already loaded, skipping...");
	                return result;
	            }
	
	            File libFile = e.getDeploymentSpec().buildPath(e.getName());
	            reporter.report(" - using deployment spec at ", libFile.getAbsolutePath());
	            if(callerClassLoader==NativeLibraryManager.class.getClassLoader()
	            		|| callerClassLoader==Object.class.getClassLoader()) {
	            	Runtime.getRuntime().load(libFile.getAbsolutePath());
	            }else {
	            	libraryLoader.accept(callerClass, libFile.getAbsolutePath());
	            }
	            result = libFile;
	            reporter.report(" - ok!");
	            e.setLoaded(true);
                loaded = true;
                break;
	
	        // Load via System.load() using default paths..
	        } else {
	            for (String path : libraryPaths) {
	                File f = new File(path, library);
	                if (f.exists()) {
	                	if(callerClassLoader==NativeLibraryManager.class.getClassLoader()
	                    		|| callerClassLoader==Object.class.getClassLoader()) {
	                		Runtime.getRuntime().load(f.getAbsolutePath());
	                	}else {
	                		libraryLoader.accept(callerClass, f.getAbsolutePath());
	                	}
	                    reporter.report(" - ok, path was: ", f.getAbsolutePath());
	                    result = f;
	                    loaded = true;
	                    break;
	                }
	            }
	        }
	        if(result!=null && operatingSystem==OperatingSystem.MacOSX && (dontUseFrameworks==null || !dontUseFrameworks)) {
	        	result = result.getParentFile().getParentFile();
	        }
	        if(loaded)
	        	break;
        }
        if (!loaded) {
        	String lib = String.format(libFormat, replacements.get(0));
            throw new RuntimeException("Library '" + lib +"' was not found in 'java.library.path=" + libraryPaths + "'");
        }
        return result;
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
        } catch (WrongSystemException e) {
            reporter.report(" - skipping because of wrong system: " + e.getMessage());
            return null;
        } catch (WrongVersionException e) {
            reporter.report(" - skipping because of wrong version: " + e.getMessage());
            return null;
        } finally {
            if(inStream != null) {
                inStream.close();
                inStream = null;
            }
        }
    }

    private static DeploymentSpec unpackDeploymentSpec(URL deploymentSpec, String jarName, Boolean shouldUnpack) throws ParserConfigurationException, SAXException, IOException{
        if(jarName!=null)
        	reporter.report("Unpacking .jar file: '", jarName, "'");

        DeploymentSpec spec = readDeploySpec(deploymentSpec);
        if (spec == null)
            return spec;
        
        File tmpDir = spec.getModule()==null ? jambiTempDirBase(spec) : jambiTempDir;

        reporter.report(" - using cache directory: '", tmpDir.getAbsolutePath(), "'");

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
	                spec.setBaseDir(tmpDir);
	                spec.setBaseUrl(new URL(convertAbsolutePathStringToFileUrlString(tmpDir)));
	            } else {
	                shouldUnpack = Boolean.TRUE;
	            }
        	}
        }

        // If the dir doesn't exists or it was only half completed, copy the files over...
        // FIXME: This should be a separate API call to cause loading, we want to be able to load up multiple DeploymentSpec
        //  so we can choose which one we are going to activate (and allow enumeration of them).
        if (shouldUnpack.booleanValue()) {
            reporter.report(" - starting to copy content to cache directory...");

            for (String path : spec.getDirents()) {
                reporter.report(" - copying over: '", path, "'...");

                InputStream in = null;
                OutputStream out = null;
                try {
                    // Why do we use the ClassLoader to lookup the resource?  The DeploymentSpec
                    //  needs to tightly bind the location of the sub-parts with the location of the spec.
                    // This means we use the containing JAR url, + path to deployment.xml + path to file we
                    //  are looking for to always find the path.  This way we will never get any surprised
                    //  where the classloader chose to provide us with another file instead of the one we expected.
                    // FIXME
                    Enumeration<URL> resources = classLoader().getResources(path);
                    while (resources.hasMoreElements()) {
                        URL url = resources.nextElement();
                        String eform = url.toExternalForm();
                        if (eform.contains(jarName)) {
                            in = url.openStream();
                            reporter.report("    - matched url: ", url.toExternalForm());
                        } else if (VERBOSE_LOADING) {
                            reporter.report("    - unmatched .jar file: ", eform);
                        }
                    }

                    if (in == null) {
//                            throw new FileNotFoundException("Dirent '" + path
//                                                            + "' specified in qtjambi-deployment.xml in '"
//                                                            + jarName + "' does not exist");
                        continue;
                    }

                    File outFile = new File(tmpDir, path.replace('/', File.separatorChar));
                    if(spec.getModule()!=null)
                    	outFile.deleteOnExit();
                    File outFileDir = outFile.getParentFile();
                    if (!outFileDir.exists()) {
                        reporter.report(" - creating directory: ", outFileDir.getAbsolutePath());
                        outFileDir.mkdirs();
                        if(spec.getModule()!=null)
                        	outFileDir.deleteOnExit();
                    }

                    out = new FileOutputStream(outFile);
                    try {
                    	reporter.report(" - copying to ", outFile.getAbsolutePath());
                        copy(in, out);
                    } finally {
                        in = null;    // copy() ALWAYS closes it for us
                        out = null;   // copy() ALWAYS closes it for us
                    }
                } finally {
                    if(in != null) {
                        try {
                            in.close();
                        } catch(IOException eat) {
                        }
                    }
                }
            }

            for (LibraryEntry e : spec.getLibraries()) {
            	if(!(e instanceof SymlinkEntry)) {
                    reporter.report(" - copying over: '", e.getName(), "'...");

                    InputStream in = null;
                    OutputStream out = null;
                    try {
                        Enumeration<URL> resources = classLoader().getResources(e.getName());
                        while (resources.hasMoreElements()) {
                            URL url = resources.nextElement();
                            String eform = url.toExternalForm();
                            if (eform.contains(jarName)) {
                                in = url.openStream();
                                reporter.report("    - matched url: ", url.toExternalForm());
                            } else if (VERBOSE_LOADING) {
                                reporter.report("    - unmatched .jar file: ", eform);
                            }
                        }

                        if (in == null) {
//                            throw new FileNotFoundException("Library '" + e.getName()
//                                                            + "' specified in qtjambi-deployment.xml in '"
//                                                            + jarName + "' does not exist");
                            continue;
                        }

                        File outFile = new File(tmpDir, e.getName().replace('/', File.separatorChar));
                        if(spec.getModule()!=null)
                        	outFile.deleteOnExit();
                        File outFileDir = outFile.getParentFile();
                        if (!outFileDir.exists()) {
                            reporter.report(" - creating directory: ", outFileDir.getAbsolutePath());
                            outFileDir.mkdirs();
                            if(spec.getModule()!=null)
                            	outFileDir.deleteOnExit();
                        }

                        out = new FileOutputStream(outFile);
                        try {
                        	reporter.report(" - copying to ", outFile.getAbsolutePath());
                            copy(in, out);
                        } finally {
                            in = null;    // copy() ALWAYS closes it for us
                            out = null;   // copy() ALWAYS closes it for us
                        }
                    } finally {
                        if(in != null) {
                            try {
                                in.close();
                            } catch(IOException eat) {
                            }
                        }
                    }
            	}
            }
            List<SymlinkEntry> shiftedLinks = new ArrayList<>();
            for (LibraryEntry e : spec.getLibraries()) {
            	if(e instanceof SymlinkEntry) {
            		SymlinkEntry s = (SymlinkEntry)e;
            		File outFile = new File(tmpDir, s.getName().replace('/', File.separatorChar));
            		if(spec.getModule()!=null)
            			outFile.deleteOnExit();
                    File outFileDir = outFile.getParentFile();
                    if (!outFileDir.exists()) {
                        reporter.report(" - creating directory: ", outFileDir.getAbsolutePath());
                        outFileDir.mkdirs();
                        if(spec.getModule()!=null)
                        	outFileDir.deleteOnExit();
                    }
                    File target = new File(tmpDir, s.getTarget().replace('/', File.separatorChar));
                    if(target.exists()) {
                    	reporter.report(" - creating symbolic link ", outFile.getAbsolutePath());
                    	Files.createSymbolicLink(outFile.toPath(), outFile.getParentFile().toPath().relativize(target.toPath()));
                    }else {
                    	shiftedLinks.add(s);
                    }
            	}
            }
            if(!shiftedLinks.isEmpty()) {
            	List<SymlinkEntry> _shiftedLinks = new ArrayList<>();
                while(!shiftedLinks.isEmpty()) {
                	_shiftedLinks.clear();
                	for (SymlinkEntry s : shiftedLinks) {
                		File outFile = new File(tmpDir, s.getName().replace('/', File.separatorChar));
                		if(spec.getModule()!=null)
                			outFile.deleteOnExit();
                        File outFileDir = outFile.getParentFile();
                        if (!outFileDir.exists()) {
                            reporter.report(" - creating directory: ", outFileDir.getAbsolutePath());
                            outFileDir.mkdirs();
                            if(spec.getModule()!=null)
                            	outFileDir.deleteOnExit();
                        }
                        File target = new File(tmpDir, s.getTarget().replace('/', File.separatorChar));
                        if(target.exists()) {
                        	reporter.report(" - creating symbolic link ", outFile.getAbsolutePath());
                        	Files.createSymbolicLink(outFile.toPath(), outFile.getParentFile().toPath().relativize(target.toPath()));
                        }else {
                        	_shiftedLinks.add(s);
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

    private static String jambiLibraryName(String prefix, String lib, int[] version, List<String> replacements) {
        if(prefix!=null)
        	lib = prefix + lib;
        switch (operatingSystem) {
        case Windows:
        	if(version.length>0)
        		replacements.add(""+version[0]);
        	replacements.add("");
        	break;
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
        	break;
    	default:
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
        	break;
        }
        switch (operatingSystem) {
        case Windows: {
        	if (configuration == Configuration.Debug)
                lib += "d";
    		return lib + "%1$s.dll";  // "foobar1.dll"
        }
        case MacOSX: 
        	if (configuration == Configuration.Debug)
                lib += "_debug";
        	return "lib" + lib + "%1$s.jnilib";
        case Linux:
		case Android: 
        	if (configuration == Configuration.Debug)
                lib += "_debug";
        	return "lib" + lib + ".so%1$s";
		default:
			break;
        }
        throw new RuntimeException("Unreachable statement");
    }


    private static String qtLibraryName(String qtprefix, String lib, String libInfix, int[] version, String versionStrg, Configuration configuration, List<String> replacements) {
        if(libInfix==null){
        	libInfix = "";
        }
        String _lib = lib;
        if(qtprefix!=null && version!=null && version.length>0){
        	_lib = qtprefix + version[0] + lib;
    	}
        String prefix = _lib.startsWith("lib") ? "" : "lib";
        switch (operatingSystem) {
        case Windows:
        	replacements.add("");
        	break;
    	default:
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
    		}else if(versionStrg!=null) {
    			replacements.add("."+versionStrg);
    			replacements.add("");
    		}else {
    			replacements.add("");
    		}
        	break;
        }
        switch (operatingSystem) {
        case Windows:
            return configuration == Configuration.Debug && !isMinGWBuilt
                ? _lib + libInfix + "d.dll"  // "QtFood4.dll"
                : _lib + libInfix + ".dll";  // "QtFoo4.dll"
        case MacOSX:
        	if(dontUseFrameworks==null || !dontUseFrameworks) {
        		if(version!=null && version.length>0 && version[0]<6)
        			return String.format("%1$s%2$s.framework/Versions/%3$s/%1$s%2$s", qtprefix, lib, version[0]);
        		else
        			return String.format("%1$s%2$s.framework/Versions/A/%1$s%2$s", qtprefix, lib);
        	}else {
	        	if(version!=null && version.length>1 && version[0]>=5 && version[1]>=14) {
	        		return prefix + _lib + libInfix + "%1$s.dylib";
	        	}else {
		            return configuration == Configuration.Debug
		                ? prefix + _lib + libInfix + "_debug%1$s.dylib"  // "libQtFoo_debug.4.dylib"
		                : prefix + _lib + libInfix + "%1$s.dylib";  // "libQtFoo.4.dylib"
	        	}
        	}
        case Linux:
        case Android:
            // Linux doesn't have a dedicated "debug" library since 4.2
            return prefix + _lib + libInfix + ".so%1$s";  // "libQtFoo.so.4"
		default:
			break;
        }
        throw new RuntimeException("Unreachable statement");
    }

    /**
     * Copies the data in the inputstream into the output stream.
     * @param in The source.
     * @param out The destination.
     *
     * @throws IOException when there is a problem...
     */
    private static boolean copy(InputStream in, OutputStream out) throws IOException {
        boolean bf = false;

        try {
            byte buffer[] = new byte[1024 * 8];
            int n;
            while((n = in.read(buffer)) > 0) {
                out.write(buffer, 0, n);
            }

            out.close();
            out = null;
            in.close();
            in = null;

            bf = true;
        } finally {
            if(out != null) {
                try {
                    out.close();
                } catch(IOException eat) {
                }
                out = null;
            }
            if(in != null) {
                try {
                    in.close();
                } catch(IOException eat) {
                }
                in = null;
            }
        }
        return bf;
    }

    static boolean isUsingDeploymentSpec() {
        return !deploymentSpec.isEmpty();
    }

    // This is implemented so that String#replace(char, char) is not used as this is regexp based
    //  and sounds heavy weight for us.  Maybe I should performance test this point before just
    //  implementing this but bleh!
    private static String stringCharReplace(String s, char fromChar, char toChar) {
        final int len = s.length();
        StringBuilder sb = new StringBuilder();
        for(int i = 0; i < len; i++) {
            char c = s.charAt(i);
            if(c == fromChar)
                sb.append(toChar);
            else
                sb.append(c);
        }
        return sb.toString();
    }

    private static String convertAbsolutePathStringToFileUrlString(String pathString) {
        if(File.separatorChar == '\\')
            pathString = stringCharReplace(pathString, '\\', '/');  // windows
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
    private enum OperatingSystem {
        Windows,
        MacOSX,
        Linux,
        Android
    }
    
    /** 
     * Enum for defining whether Qt is build in Release or Debug. 
     */
    enum Configuration {
        Release,
        Debug,
        Test;
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
