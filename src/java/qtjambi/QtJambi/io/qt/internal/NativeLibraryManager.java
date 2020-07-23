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

package io.qt.internal;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.URL;
import java.net.URLConnection;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.Enumeration;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Properties;
import java.util.SortedMap;
import java.util.TreeMap;
import java.util.function.BiConsumer;
import java.util.jar.Attributes;
import java.util.jar.Manifest;

import javax.xml.parsers.SAXParser;
import javax.xml.parsers.SAXParserFactory;

import org.xml.sax.helpers.DefaultHandler;

import io.qt.QNoSuchSlotException;
import io.qt.QtUtilities;
import io.qt.core.QDeclarableSignals;

// !!NOTE!! This class can have no dependencies on Qt since
// it is required for loading the libraries.


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
 * <ll>
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
 * </ll>
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
 * system and via <code>-Djava.library.path<code>
 *
 * To get runtime information about how library loading works, specify
 * the <code>-Dio.qt.verbose-loading</code> system property
 * to the Virtual Machine. It possible to specify that the native
 * library manager should load debug versions of libraries as
 * well. This is done by specifying the system property
 * </code>-Dio.qt.debug</code>
 *
 */
public class NativeLibraryManager {

    public static String DEPLOY_DESCRIPTOR_NAME = "qtjambi-deployment.xml";

    private static final String DEBUG_SUFFIX = "_debug";

    private static final boolean VERBOSE_LOADING = Boolean.getBoolean("io.qt.verbose-loading");

    private static final int LOAD_TRUE = 1;
    private static final int LOAD_SYSTEM = 2;
    private static final int LOAD_FALSE = 3;
    private static final int LOAD_NEVER = 4;

    private static final int[] versionA;
    public static final String VERSION_STRING;
    public static final String ICU_VERSION;
    public static final String LIBINFIX;
    public static final String VERSION_MAJOR_STRING;
    public static final int VERSION_MAJOR;
    public static final String QTJAMBI_SONAME_VERSION_MAJOR;
    // We use a List<> to make the collection read-only an array would not be suitable
    private static final List<String> systemLibrariesList;
    private static final List<String> jniLibdirBeforeList;
    private static final List<String> jniLibdirList;
    
    static{
        int[] tmpVERSION_A = null;
        String tmpVERSION_STRING = null;
        String tmpICUVERSION_STRING = null;
        String tmpINFIX_STRING = "5";
        String tmpVERSION_MAJOR_STRING = null;
        int tmpVERSION_MAJOR = 0;
        String tmpQTJAMBI_SONAME_VERSION_MAJOR = null;
        List<String> tmpSystemLibrariesList = null;
        List<String> tmpJniLibdirBeforeList = null;
        List<String> tmpJniLibdirList = null;
        try {
            final Properties props = new Properties();
            final InputStream in = NativeLibraryManager.class.getResourceAsStream("version.properties");
            if (in == null)
                throw new ExceptionInInitializerError("version.properties not found!");
            try {
                props.load(in);
            } catch (Exception ex) {
                throw new ExceptionInInitializerError("Cannot read properties!");
            }
            tmpINFIX_STRING = props.getProperty("qtjambi.libinfix");
            tmpICUVERSION_STRING = props.getProperty("qtjambi.icu.version", "56");
            
            tmpQTJAMBI_SONAME_VERSION_MAJOR = props.getProperty("qtjambi.soname.version.major");

            tmpVERSION_STRING = props.getProperty("qtjambi.version");
            if (tmpVERSION_STRING == null)
                throw new ExceptionInInitializerError("qtjambi.version is not set!");
            int dotIndex = tmpVERSION_STRING.indexOf(".");	// "4.7.4" => "4"
            if(dotIndex > 0)	// don't allow setting it be empty
                tmpVERSION_MAJOR_STRING = tmpVERSION_STRING.substring(0, dotIndex);
            else
                tmpVERSION_MAJOR_STRING = tmpVERSION_STRING;

            tmpVERSION_MAJOR = Integer.parseInt(tmpVERSION_MAJOR_STRING);
            
            tmpVERSION_A = getVersion(tmpVERSION_STRING);        	
            SortedMap<String,String> tmpSystemLibrariesMap = new TreeMap<String,String>();
            SortedMap<String,String> tmpJniLibdirBeforeMap = new TreeMap<String,String>();
            SortedMap<String,String> tmpJniLibdirMap = new TreeMap<String,String>();
            Enumeration<? extends Object> e = props.propertyNames();
            while (e.hasMoreElements()) {
                String key = (String) e.nextElement();
                String value = props.getProperty(key);
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
            versionA = tmpVERSION_A;
            VERSION_STRING = tmpVERSION_STRING;
            VERSION_MAJOR = tmpVERSION_MAJOR;
            VERSION_MAJOR_STRING = tmpVERSION_MAJOR_STRING;
            QTJAMBI_SONAME_VERSION_MAJOR = tmpQTJAMBI_SONAME_VERSION_MAJOR;
            systemLibrariesList = tmpSystemLibrariesList;
            jniLibdirBeforeList = tmpJniLibdirBeforeList;
            jniLibdirList = tmpJniLibdirList;
            LIBINFIX = tmpINFIX_STRING;
        }
    }
    
 // The purpose of this method is to resolve the names provided in the list into DSO paths relative to the project.
    private static List<String> resolveSystemLibraries(List<String> tmpSystemLibrariesList) {
        if(tmpSystemLibrariesList == null)
            return null;
        DeploymentSpec[] deploymentSpecA = NativeLibraryManager.unpack();
        if(deploymentSpecA == null)
            return null;
        List<String> resolvedList = new ArrayList<String>();
        for(String original : tmpSystemLibrariesList) {
            String s = original;
            if(operatingSystem==OperatingSystem.Windows)   // convert "/" into "\"
                s = stringCharReplace(s, "/", File.separator);

            String resolvedPath = null;
            for(DeploymentSpec deploymentSpec : deploymentSpecA) {
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

    /** Enum for defining the operation system. */
    public enum OperatingSystem {
        Windows("Windows"),
        MacOSX("MacOSX"),
        Linux("Linux"),
        FreeBSD("FreeBSD"),
        SunOS("SunOS");

        private OperatingSystem(String label) {
            this.label = label;
        }
        public String toString() {
            return label;
        }
        private String label;
    }

    /** The operating system Qt Jambi is running on. */
    private static final OperatingSystem operatingSystem = decideOperatingSystem();
    private static final String osArchName;
    static {
        final String K_SUNOS32 = "sunos32";
        final String K_SUNOS64 = "sunos64";
        final String K_WIN32 = "win32";
        final String K_WIN64 = "win64";
        final String K_LINUX32 = "linux32";
        final String K_LINUX64 = "linux64";
        final String K_FREEBSD32 = "freebsd32";
        final String K_FREEBSD64 = "freebsd64";
    	
        switch (operatingSystem) {
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
        case MacOSX:
            osArchName = "macosx";
            break;
        case SunOS:
            // sparc || sparcv9
            osArchName = System.getProperty("os.arch").equalsIgnoreCase("sparcv9")
                         ? K_SUNOS32
                         : K_SUNOS64;
            break;
        default:
            osArchName = "unknown";
            break;
        }
    }

    private static OperatingSystem decideOperatingSystem() {
        String osName = System.getProperty("os.name").toLowerCase();
        if (osName.startsWith("windows")) return OperatingSystem.Windows;
        if (osName.startsWith("mac")) return OperatingSystem.MacOSX;
        if (osName.startsWith("freebsd")) return OperatingSystem.FreeBSD;
        if (osName.equals("sunos")) return OperatingSystem.SunOS;	// SunOS
        return OperatingSystem.Linux;
    }
    
    /** Defines whether Qt is build in Release or Debug. */
    public enum Configuration {
        Release("Release"),
        Debug("Debug"),
        Test("Test");

        private Configuration(String label) {
            this.label = label;
        }
        public String toString() {
            return label;
        }
        private String label;
    }

    /** The configuration of Qt Jambi. */
    private static Configuration configuration = decideConfiguration();
	private static final Configuration DEFAULT_CONFIGURATION = Configuration.Release;
	
	public static Configuration configuration() {
		return configuration;
	}
	
	public static String osArchName() {
		return osArchName;
	}
    
    private static Configuration decideConfiguration() {
        Configuration configuration = null;

        final String K_io_qt_debug = "io.qt.debug";
        String debugString = System.getProperty(K_io_qt_debug);
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
                        java.util.logging.Logger.getLogger("io.qt.internal").log(java.util.logging.Level.WARNING, "-D" + K_io_qt_debug + "=" + Boolean.TRUE + "; is set instead of =debug.");
                    }
                }
            }
        } catch(Exception e) {
            java.util.logging.Logger.getLogger("io.qt.internal").log(java.util.logging.Level.SEVERE, "-D" + K_io_qt_debug + "=" + Boolean.FALSE + "; is assumed default", e);
            // only because Configuration.Release is assumed
        }

        if(configuration == null)
            configuration = decideDefaultConfiguration();

        if(configuration == null)
            configuration = DEFAULT_CONFIGURATION;

        return configuration;
    }
    
    private static Configuration decideDefaultConfiguration() {
        Configuration configuration = null;
        String configurationSource = null;
        try {
            boolean error = false;
            Enumeration<URL> enumUrls = Thread.currentThread().getContextClassLoader().getResources("META-INF/MANIFEST.MF");
            while(enumUrls.hasMoreElements()) {
                URL url = enumUrls.nextElement();
                InputStream inStream = null;
                try {
                    URLConnection urlConnection = url.openConnection();
                    inStream = urlConnection.getInputStream();
                    Manifest manifest = new Manifest(inStream);
                    inStream.close();
                    inStream = null;

                    Attributes attributes = manifest.getMainAttributes();
                    String tmpBundleSymbolicName = attributes.getValue("Bundle-SymbolicName");
                    String tmpXQtJambiBuild = attributes.getValue("X-QtJambi-Build");

                    Configuration tmpConfiguration = null;
                    if(bundleSymbolicNameMatch("io.qt", tmpBundleSymbolicName)) {
                        // We found the right bundle
                        if("release".equals(tmpXQtJambiBuild)) {
                            tmpConfiguration = Configuration.Release;
                        } else if("debug".equals(tmpXQtJambiBuild)) {
                            tmpConfiguration = Configuration.Debug;
                        } else if("test".equals(tmpXQtJambiBuild)) {
                            tmpConfiguration = Configuration.Test;
                        } else {
                            if(tmpXQtJambiBuild == null)
                                tmpXQtJambiBuild = "<notset>";
                            // FIXME: Only JARs with/relating-to platform specific DSO parts will have X-QtJambi-Build setting.
                            //java.util.logging.Logger.getLogger("io.qt").log(java.util.logging.Level.INFO, Utilities.class.getName() + "#decideDefaultConfiguration()  " + url.toString() + " invalid " + "X-QtJambi-Build" + ": " + tmpXQtJambiBuild);
                        }

                        // We keep checking them all
                        // If we find 2 matches this is a failure case right now, until we have resolution strategy implemented
                        if(configuration != null && tmpConfiguration != null && tmpConfiguration != configuration) {
                            // Multiple matches, ah well...
                            System.err.println(QtUtilities.class.getName() + "#decideDefaultConfiguration()  " +
                                configurationSource + "(" + configuration.toString() + ") != " +
                                url.toString() + "(" + tmpConfiguration.toString() + ") multiple different " +
                                "X-QtJambi-Build" + ": found in ClassPath JARs, this is currently not supported");
                            configuration = null;
                            configurationSource = null;
                            error = true;
                        } else if(tmpConfiguration != null && !error) {
                            configuration = tmpConfiguration;  // found
                            configurationSource = url.toString();
                        }
                    }
                } catch(IOException e) {
                    java.util.logging.Logger.getLogger("io.qt.internal").log(java.util.logging.Level.SEVERE, "", e);
                } finally {
                    if(inStream != null) {
                        try {
                            inStream.close();
                        } catch(IOException eat) {
                        }
                        inStream = null;
                    }
                }
            }
        } catch(IOException e) {
            java.util.logging.Logger.getLogger("io.qt.internal").log(java.util.logging.Level.SEVERE, "", e);
        } catch(SecurityException e) {
            java.util.logging.Logger.getLogger("io.qt.internal").log(java.util.logging.Level.SEVERE, "", e);
        }
        return configuration;
    }

    private static boolean bundleSymbolicNameMatch(String a, String b) {
        if(a == null || b == null)
            return false;
        int i = a.indexOf(';');
        if(i >= 0)
            a = a.substring(0, i);
        i = a.indexOf(';');
        if(i >= 0)
            b = b.substring(0, i);
        if(a.equals(b))
            return true;
        // FIXME later for now we don't care, we need to remove this and properly
        //   find the bundle we expect, not almost anything with X-QtJambi-Build set
        if(b.startsWith(a))
            return true;
        return false;
    }

    private static DeploymentSpec[] activeDeploymentSpecA;

    public static String loadToString(int load) {
        switch(load) {
        case LOAD_TRUE:
            return "load-true";
        case LOAD_SYSTEM:
            return "load-system";
        case LOAD_NEVER:
            return "load-never";
        case LOAD_FALSE:
            return "load-false";
        }
        return null;
    }

    // FIXME: Remove this from public API
    public static File jambiTempDir() {
        if(activeDeploymentSpecA == null)
            return null;
        // FIXME: move this into each DeploymentSpec making the path unique per DeploymentSpec
        return activeDeploymentSpecA[0].buildPath("");
    }

    // FIXME: This should be a read-only object
    public static DeploymentSpec[] getActiveDeploymentSpec() {
        return activeDeploymentSpecA;
    }

    /**
     * Called during shutdown of the deployment spec to help clear
     * the active from static values.
     * @return
     */
    public static DeploymentSpec[] resetActiveDeploymentSpec() {
        DeploymentSpec[] rv = activeDeploymentSpecA;
        activeDeploymentSpecA = null;
        return rv;
    }

    private static class XMLHandler extends DefaultHandler {
        public DeploymentSpec spec;
        private List<String> direntCurrentPathList = new ArrayList<String>();

        public void startElement(String uri,
                                 String localName,
                                 String name,
                                 org.xml.sax.Attributes attributes) {
            if (name.equals("cache")) {
                String key = attributes.getValue("key");
                if (key == null) {
                    throw new DeploymentSpecException("<cache> element missing required attribute \"key\"");
                }
                spec.setKey(key);
                reporter.report(" - cache key='", spec.getKey(), "'");

            } else if (name.equals("library")) {
                LibraryEntry e = new LibraryEntry();
                e.setName(attributes.getValue("name"));
                if (e.getName() == null) {
                    throw new DeploymentSpecException("<library> element missing required attribute \"name\"");
                }

                String load = attributes.getValue("load");
                e.setLoad(LOAD_FALSE);
                if (load != null) {
                    if (load.equals("true"))
                        e.setLoad(LOAD_TRUE);
                    else if (load.equals("never"))
                        e.setLoad(LOAD_NEVER);
                    else if (load.equals("system"))
                        e.setLoad(LOAD_SYSTEM);
                }

                e.setDeploymentSpec(spec);

                String fileName = new File(e.getName()).getName();
                if (e.getLoad() == LOAD_NEVER) {
                    neverLoad.put(fileName, e);

                } else {
                    // Add library name to the global map of libraries...
                    LibraryEntry old = libraryMap.get(fileName);
                    if (old != null) {
                        throw new DeploymentSpecException("<library> '" + e.getName()
                                                          + "' is duplicated. Present in both '"
                                                          + spec.getSourceUrl() + "' and '"
                                                          + old.getDeploymentSpec().getSourceUrl() + "'.");
                    }
                    reporter.report(" - adding '", fileName, "' to library map");
                    libraryMap.put(fileName, e);
                }

                spec.addLibraryEntry(e);
                reporter.report(" - library: name='", e.getName(), "', ", loadToString(e.getLoad()));

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
    private static File jambiTempDirBase(String key) {
        File tmpDir = new File(System.getProperty("java.io.tmpdir"));
        String user = System.getProperty("user.name");
        String arch = System.getProperty("os.arch");
        return new File(tmpDir, "QtJambi_" + user + "_" + arch + "_" + VERSION_STRING + "_" + key);
    }


    /**
     * Returns the list of all plugin paths that are specified in the
     * deployment descriptors. If deployment descriptors are not used,
     * this list will be an empty list.
     *
     * @return The list of plugin paths
     */
    public static List<String> pluginPaths() {
        List<String> paths = new ArrayList<String>();
        for (DeploymentSpec spec : deploymentSpecs) {
            File root = spec.getBaseDir();
            List<String> pluginPaths = spec.getPluginPaths();
            if (pluginPaths != null)
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
    public static List<String> pluginDesignerPaths() {
        List<String> paths = new ArrayList<String>();
        for (DeploymentSpec spec : deploymentSpecs) {
            File root = spec.getBaseDir();
            List<String> pluginDesignerPaths = spec.getPluginDesignerPaths();
            if (pluginDesignerPaths != null)
                for (String path : pluginDesignerPaths)
                    paths.add(new File(root, path).getAbsolutePath());
        }
        return paths;
    }


    /**
     * Loads a library with name specified in <code>library</code>.
     * The library name will be expanded to the JNI shared library
     * name for a given platform, so the name "qtjambi" will be
     * expanded like this:
     *
     * <ll>
     *   <li> Windows: qtjambi.dll
     *   <li> Linux / Unix: libqtjambi.so
     *   <li> Mac OS X: libqtjambi.jnilib
     * </ll>
     *
     * When using loading libraries from the filesystem, this method
     * simply calls <code>System.loadLibrary</code>.
     *
     * When the system property <code>-Dio.qt.debug</code>
     * is specified, the suffix <code>_debuglib</code> will be appended
     * to the filename, replacing "qtjambi" above with "qtjambi_debuglib".
     *
     * @param library The name of the library..
     */
    public static void loadJniLibrary(Class<?> callerClass, String library, String version) {
        String lib = jniLibraryName(library, version);
        loadNativeLibrary(callerClass, lib);
    }
    
    public static void loadLibrary(String library) {
    	switch (operatingSystem()) {
        case Windows: 
        	loadNativeLibrary(Object.class, library + ".dll");
        	break;
        case MacOSX: 
        	try {
				loadNativeLibrary(Object.class, "lib" + library + ".jnilib");
			} catch (Throwable e) {
				loadNativeLibrary(Object.class, "lib" + library + ".dylib");
			}
        	break;
        case Linux:
        case FreeBSD: 
        	loadNativeLibrary(Object.class, "lib" + library + ".so");
        	break;
		case SunOS:
			break;
		default:
			break;
        }
    }
    
    public static void loadQtJambiLibrary() {
    	loadJambiLibrary(NativeLibraryManager.class, null, "QtJambi");
    }

    public static void loadQtJambiLibrary(Class<?> callerClass, String library) {
    	loadJambiLibrary(callerClass, "QtJambi", library);
    }

    public static void loadJambiLibrary(Class<?> callerClass, String prefix, String library) {
        String version;
        if(versionA[0]>=5){
        	version = "" + VERSION_MAJOR_STRING;
        }else{
        	version = QTJAMBI_SONAME_VERSION_MAJOR;
        }
        String lib = jambiLibraryName(prefix, library, version);
        loadNativeLibrary(callerClass, lib);
    }

    /**
     * Loads a library with name specified in <code>library</code>.
     * The library name will be expanded to the default shared library
     * name for a given platform, so the name "QtCore" and version "4" and "5" will be
     * expanded like this:
     *
     * <ll>
     *   <li> Windows: QtCore4.dll
     *   <li> Linux / Unix: libQtCore.so.4
     *   <li> Mac OS X: libQtCore.4.dylib
     * </ll>
     * <ll>
     *   <li> Windows: Qt5Core.dll
     *   <li> Linux / Unix: libQt5Core.so.5
     *   <li> Mac OS X: libQt5Core.5.dylib
     * </ll>
     *
     * When using loading libraries from the filesystem, this method
     * simply calls <code>System.loadLibrary</code>.
     * @param library The name of the library..
     */
    public static void loadQtLibrary(String library) {
    	findAndLoadLibrary(Object.class, "Qt", library, LIBINFIX, VERSION_MAJOR_STRING);
    }
    
    public static void loadUtilityLibrary(String library, String version) {
    	findAndLoadLibrary(Object.class, null, library, null, version);
    }
    
    static File loadQtCore() {
    	return findAndLoadLibrary(Object.class, "Qt", "Core", LIBINFIX, VERSION_MAJOR_STRING);
    }
    
    public static void loadLibrary(Class<?> callerClass, String qtprefix, String library, String libInfix, String version) {
    	findAndLoadLibrary(callerClass, qtprefix, library, libInfix, version);
    }
    
    private static File findAndLoadLibrary(Class<?> callerClass, String qtprefix, String library, String libInfix, String version) {
        String lib = qtLibraryName(qtprefix, library, libInfix, version);
        try {
			return loadNativeLibrary(callerClass, lib);
		} catch (RuntimeException e) {
			if("Qt".equals(qtprefix) && "Core".equals(library) && configuration==Configuration.Release) {
				// try to run qtjambi with debug libraries instead
				configuration=Configuration.Debug;
				lib = qtLibraryName(qtprefix, library, libInfix, version);
				try {
					return loadNativeLibrary(callerClass, lib);
				} catch (Throwable e1) {
					configuration=Configuration.Release;
					throw e;
				}
			}else {
				throw e;
			}
		}
    }
    
    public static boolean isAvailableQtLibrary(String library) {
    	return isAvailableLibrary("Qt", library, LIBINFIX, VERSION_MAJOR_STRING);
    }

    // FIXME: Use proper dependency tree to loadQtLibrary() but this will do for now
    public static boolean isAvailableLibrary(String qtprefix, String library, String libInfix, String version) {
        unpack();
    	String lib = qtLibraryName(qtprefix, library, libInfix, version);  // "QtDBus" => "libQtDBus.so.4"
        // search active deploymentSpec for existance of library
        DeploymentSpec[] deploymentSpecA = getActiveDeploymentSpec();
        if(deploymentSpecA != null){
	        for(DeploymentSpec deploymentSpec : deploymentSpecA) {
	            List<LibraryEntry> libraries = deploymentSpec.getLibraries();
	            if(libraries!=null){
		            for(LibraryEntry libraryEntry : libraries) {
		                String name = libraryEntry.getName();  // name="lib/libQtFoo.so.4"
		                if(name == null)
		                    continue;
		                if(lib.equals(name))      // lib=="lib/libQtFoo.so.4"
		                    return true;
		                String[] pathA = name.split("\\/");
		                if(pathA == null || pathA.length == 0)
		                    continue;
		                String lastPart = pathA[pathA.length - 1];
		                if(lib.equals(lastPart))  // lib=="libQtFoo.so.4"
		                    return true;
		            }
	            }
	        }
        }
    	String libPaths = System.getProperty("io.qt.library-path-override");
        if (libPaths == null || libPaths.length() == 0) {
            libPaths = System.getProperty("java.library.path");
        }

        List<String> paths = new ArrayList<>();
        if (libPaths != null)
        	paths.addAll(Arrays.asList(libPaths.split("\\"+File.pathSeparator)));
        switch (operatingSystem()) {
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
        for (String path : paths) {
            File f = new File(path, lib);
            if (f.exists()) {
            	return true;
            }
        }
        return false;
    }

    public static List<String> unpackPlugins() {
        if(activeDeploymentSpecA != null) {
            List<String> paths = new ArrayList<String>();

            // Merge pluginPaths
            for(DeploymentSpec spec : activeDeploymentSpecA) {
                List<String> pluginPaths = spec.getPluginPaths();
                if(pluginPaths != null) {
                    for(String p : pluginPaths) {
                        File resolvedFile = spec.buildPath(p);
                        // Checking it really exists as a directory (before adding it to the list)
                        //  is kind of a security check.
                        if(resolvedFile != null && resolvedFile.isDirectory()) {
                            paths.add(resolvedFile.getAbsolutePath());
                        }
                    }
                }
            }

            // Merge pluginDesignerPaths
            for(DeploymentSpec spec : activeDeploymentSpecA) {
                List<String> pluginDesignerPaths = spec.getPluginDesignerPaths();
                if(pluginDesignerPaths != null) {
                    for(String p : pluginDesignerPaths) {
                        File resolvedFile = spec.buildPath(p);
                        // Checking it really exists as a directory (before adding it to the list)
                        //  is kind of a security check.
                        if(resolvedFile != null && resolvedFile.isDirectory()) {
                            paths.add(resolvedFile.getAbsolutePath());
                        }
                    }
                }
            }

            return paths;
        }
        return null;
    }

    // Made this more vissible to allow unpacking earlier on the startup process
    public static DeploymentSpec[] unpack() {
        if (unpacked)
            return activeDeploymentSpecA;
        try {
            synchronized(NativeLibraryManager.class) {
                if (unpacked)
                    return activeDeploymentSpecA;
                activeDeploymentSpecA = unpack_helper();
                unpacked = true;
            }
        } catch (Throwable t) {
            throw new RuntimeException("Failed to unpack native libraries, progress so far:\n"
                                       + reporter, t);
        }
        return activeDeploymentSpecA;
    }

    private static DeploymentSpec[] unpack_helper() throws Exception {
        List<DeploymentSpec> specList = new ArrayList<DeploymentSpec>();
        ClassLoader loader = classLoader();
        // Multiple descriptors maybe found
        Enumeration<URL> specsFound = loader.getResources(DEPLOY_DESCRIPTOR_NAME);
        // FIXME: Want searchForDescriptors/parse/resolve/unpack/load phases separated
        while (specsFound.hasMoreElements()) {
            URL url = specsFound.nextElement();

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
                    URL jarUrl = new URL(eform.substring(start, end));
                    String jarName = new File(jarUrl.getFile()).getName();
                    if (VERBOSE_LOADING)
                        reporter.report("Loading ", jarName, " from ", eform);
                    spec = unpackDeploymentSpec(url, jarName, null);
                }
            } else if (protocol.equals("file")) {
                // No unpack since we presume we are already unpacked
                spec = unpackDeploymentSpec(url, null, Boolean.FALSE);
            }
            if(spec != null)
                specList.add(spec);
        }

        if (specList.size() == 0) {
            reporter.report("No '", DEPLOY_DESCRIPTOR_NAME,
                            "' found in classpath, loading libraries via 'java.library.path'");
            return null;
        }

        return specList.toArray(new DeploymentSpec[specList.size()]);
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
    private static File loadNativeLibrary(Class<?> callerClass, String lib) {
    	File result = null;
        try {
        	result = loadLibrary_helper(callerClass, lib);
            if (VERBOSE_LOADING)
                System.out.println(reporter.recentReports());

        } catch (Throwable e) {
            throw new RuntimeException("Loading library failed, progress so far:\n" + reporter, e);
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

    private static File loadLibrary_helper(Class<?> callerClass, String lib) {
    	File result = null;
        unpack();

        reporter.report("Loading library: '", lib, "'...");

        // First of all verify that we're allowed to load this library...
        LibraryEntry e = neverLoad.get(lib);
        if (e != null) {
            throw new RuntimeException("Library '" + lib + "' cannot be loaded, deploy spec");
        }

        // Try to load via deploy spec...
        e = libraryMap.get(lib);
        ClassLoader callerClassLoader = callerClass.getClassLoader();
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

        // Load via System.load() using default paths..
        } else {
            boolean loaded = false;
            String libPaths = System.getProperty("io.qt.library-path-override");
            if (libPaths != null && libPaths.length() > 0) {
                reporter.report(" - using 'io.qt.library-path-override'");
            } else {
                reporter.report(" - using 'java.library.path'");
                libPaths = System.getProperty("java.library.path");
            }

            List<String> paths = new ArrayList<>();
            if (libPaths != null)
                paths.addAll(Arrays.asList(libPaths.split("\\"+File.pathSeparator)));
        	switch (operatingSystem()) {
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
            for (String path : paths) {
                File f = new File(path, lib);
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
            if (!loaded) {
                throw new RuntimeException("Library '" + lib +"' was not found in 'java.library.path="
                                           + libPaths + "'");
            }
        }
        return result;
    }

    /**
     * 
     * @param url This maybe "file" or "jar" protocol.  "http" is untested.
     * @return
     * @throws Exception
     */
    private static DeploymentSpec readDeploySpec(URL url) throws Exception {
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
            if (spec.getKey() == null) {
                throw new DeploymentSpecException("Deployment Specification doesn't include required <cache key='...'/>");
            }

            deploymentSpecs.add(spec);

            return spec;
        } catch (WrongSystemException e) {
            reporter.report(" - skipping because of wrong system: " + e.getMessage());
            return null;
        } finally {
            if(inStream != null) {
                inStream.close();
                inStream = null;
            }
        }
    }

    private static DeploymentSpec unpackDeploymentSpec(URL deploymentSpec, String jarName, Boolean shouldUnpack) throws Exception {
        if(jarName!=null)
        	reporter.report("Unpacking .jar file: '", jarName, "'");

        DeploymentSpec spec = readDeploySpec(deploymentSpec);
        if (spec == null)
            return spec;

        File tmpDir = jambiTempDirBase(spec.getKey());

        reporter.report(" - using cache directory: '", tmpDir.getAbsolutePath(), "'");

        File dummyFile = null;
        if(shouldUnpack == null) {
            // If the dir exists and contains .dummy, sanity check the contents...
            dummyFile = new File(tmpDir, ".dummy");
            if (dummyFile.exists()) {
                reporter.report(" - cache directory exists");
                shouldUnpack = Boolean.FALSE;
                spec.setBaseDir(tmpDir);
                spec.setBaseUrl(new URL(convertAbsolutePathStringToFileUrlString(tmpDir)));
            } else {
                shouldUnpack = Boolean.TRUE;
            }
        }

        // If the dir doesn't exists or it was only half completed, copy the files over...
        // FIXME: This should be a separate API call to cause loading, we want to be able to load up multiple DeploymentSpec
        //  so we can choose which one we are going to activate (and allow enumeration of them).
        if (shouldUnpack.booleanValue()) {
            reporter.report(" - starting to copy content to cache directory...");

            List<String> dirents = spec.getDirents();
            if (dirents != null) {
                for (String path : dirents) {
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

                        File outFile = new File(tmpDir, path);
                        File outFileDir = outFile.getParentFile();
                        if (!outFileDir.exists()) {
                            reporter.report(" - creating directory: ", outFileDir.getAbsolutePath());
                            outFileDir.mkdirs();
                        }

                        out = new FileOutputStream(new File(tmpDir, path));
                        try {
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

            List<LibraryEntry> libraries = spec.getLibraries();
            if (libraries != null) {
                for (LibraryEntry e : libraries) {
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

                        File outFile = new File(tmpDir, e.getName());
                        File outFileDir = outFile.getParentFile();
                        if (!outFileDir.exists()) {
                            reporter.report(" - creating directory: ", outFileDir.getAbsolutePath());
                            outFileDir.mkdirs();
                        }

                        out = new FileOutputStream(new File(tmpDir, e.getName()));
                        try {
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

        // Load the libraries tagged for loading...
        // FIXME: Like the unpack operation, this should be a separate API call to cause loading 
        List<LibraryEntry> libraries = spec.getLibraries();
        for (LibraryEntry e : libraries) {
            if (e.getLoad() == LOAD_TRUE)
                loadSystemLibrary(e.getName());
        }

        return spec;
    }

    public static boolean loadSystemLibrary(String name) {
        DeploymentSpec[] activeSpecA = getActiveDeploymentSpec();
        if(activeSpecA == null)
            throw new RuntimeException("No active deployment spec is set");

        reporter.report(" - trying to load: ", name);

        File foundFile = null;
        for(DeploymentSpec deploymentSpec : activeSpecA) {
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

    private static String jniLibraryName(String lib, String version) {
        String dotVersion;
        if(version != null && !version.isEmpty()) {
            dotVersion = "." + version;
        } else {
            version = "";
            dotVersion = "";
        }
        switch (operatingSystem()) {
        case Windows: 
        	if (configuration == Configuration.Debug)
                lib += DEBUG_SUFFIX;
        	if(VERSION_MAJOR>=5){
        		return lib + version + ".dll";  // "foobar1.dll"
        	}else{
        		return lib + ".dll";  // "foobar1.dll"
        	}
        case MacOSX: 
        	if (configuration == Configuration.Debug)
                lib += DEBUG_SUFFIX;
        	return "lib" + lib + dotVersion + ".jnilib";  // "libfoobar.1.jnilib"
        case Linux:
        case FreeBSD: 
        	if (configuration == Configuration.Debug)
                lib += DEBUG_SUFFIX;
			return "lib" + lib + ".so" + dotVersion;  // "libfoobar.so.1"
		case SunOS:
			break;
		default:
			break;
        }
        throw new RuntimeException("Unreachable statement");
    }

    private static String jambiLibraryName(String prefix, String lib, String version) {
        String dotVersion;
        if(version != null) {
            dotVersion = "." + version;
        } else {
            version = "";
            dotVersion = "";
        }
        if(prefix!=null)
        	lib = prefix + lib;
        switch (operatingSystem()) {
        case Windows: {
        	if (configuration == Configuration.Debug)
                lib += DEBUG_SUFFIX;
        	if(VERSION_MAJOR>=5){
        		return lib + version + ".dll";  // "foobar1.dll"
        	}else{
        		return lib + ".dll";  // "foobar1.dll"
        	}
        }
        case MacOSX: 
        	if (configuration == Configuration.Debug)
                lib += DEBUG_SUFFIX;
        	return "lib" + lib + dotVersion + ".jnilib";  // "libfoobar.1.dylib"
        case Linux:
		case FreeBSD: 
        	if (configuration == Configuration.Debug)
                lib += DEBUG_SUFFIX;
        return "lib" + lib + ".so" + dotVersion;  // "libfoobar.so.1"
		case SunOS:
			break;
		default:
			break;
        }
        throw new RuntimeException("Unreachable statement");
    }


    private static String qtLibraryName(String qtprefix, String lib, String libInfix, String version) {
        String dotVersion;
        if(libInfix==null){
        	libInfix = "";
        }
        if(version != null && !version.isEmpty()) {
            dotVersion = "." + version;
        } else {
            version = "";
            dotVersion = "";
        }
        if(qtprefix!=null){
        	lib = qtprefix + version + lib;
    		version = "";
    	}
        String prefix = lib.startsWith("lib") ? "" : "lib";
        switch (operatingSystem()) {
        case Windows:
            return configuration == Configuration.Debug
                ? lib + libInfix + "d" + version + ".dll"  // "QtFood4.dll"
                : lib + libInfix + version + ".dll";  // "QtFoo4.dll"
        case MacOSX:
        	if(versionA!=null && versionA.length>1 && versionA[0]>=5 && versionA[1]>=14) {
        		return prefix + lib + libInfix + dotVersion + ".dylib";
        	}else {
	            return configuration == Configuration.Debug
	                ? prefix + lib + libInfix + "_debug" + dotVersion + ".dylib"  // "libQtFoo_debug.4.dylib"
	                : prefix + lib + libInfix + dotVersion + ".dylib";  // "libQtFoo.4.dylib"
        	}
        case Linux:
        case FreeBSD:
            // Linux doesn't have a dedicated "debug" library since 4.2
            return prefix + lib + libInfix + ".so" + dotVersion;  // "libQtFoo.so.4"
		case SunOS:
			break;
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

    public static boolean isUsingDeploymentSpec() {
        unpack();
        return deploymentSpecs != null && deploymentSpecs.size() != 0;
    }

    private static Map<String, LibraryEntry> libraryMap = new HashMap<String, LibraryEntry>();
    private static Map<String, LibraryEntry> neverLoad = new HashMap<String, LibraryEntry>();
    private static List<DeploymentSpec> deploymentSpecs = new ArrayList<DeploymentSpec>();
    private static Reporter reporter = new Reporter();
    static {
    	reporter.setReportEnabled(VERBOSE_LOADING);
    }

    private static boolean unpacked = false;

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

    public static String convertAbsolutePathStringToFileUrlString(String pathString) {
        if(File.separatorChar == '\\')
            pathString = stringCharReplace(pathString, '\\', '/');  // windows
        String schemePrefix;
        if(pathString.length() > 0 && pathString.charAt(0) != '/')
            schemePrefix = "file:///";  // empty authority part, ensure leading / in path part
        else
            schemePrefix = "file://";   // empty authority part, pathString already has leading /
        return schemePrefix + pathString;
    }

    public static String convertAbsolutePathStringToFileUrlString(File file) {
        return convertAbsolutePathStringToFileUrlString(file.getAbsolutePath());
    }

    public static int[] getVersion() {
        return Arrays.copyOf(versionA, versionA.length);
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
    public static int[] getVersion(String versionString) {
        String[] sA = versionString.split("\\.");
        int[] vA = new int[sA.length];
        int i = 0;
        for(String s : sA)
            vA[i++] = Integer.valueOf(s);
        return vA;
    }

    public static void main(String args[]) throws Exception {

        unpack();

        loadQtLibrary("QtCore");
        loadQtLibrary("QtGui");
        loadQtJambiLibrary(NativeLibraryManager.class, "qtjambi");
        loadQtJambiLibrary(NativeLibraryManager.class, "io_qt_core");
        loadQtJambiLibrary(NativeLibraryManager.class, "io_qt_gui");
        loadQtLibrary("QtGui");
        loadQtLibrary("QtNetwork");

        for (String s : pluginPaths())
            System.out.println("PluginPath: " + s);

        for (String s : pluginDesignerPaths())
            System.out.println("PluginDesignerPath: " + s);

    }

	public static OperatingSystem operatingSystem() {
		return operatingSystem;
	}

}
