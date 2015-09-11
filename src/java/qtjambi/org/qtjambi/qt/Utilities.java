/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2015 Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

package org.qtjambi.qt;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLConnection;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Enumeration;
import java.util.HashSet;
import java.util.List;
import java.util.Properties;
import java.util.SortedMap;
import java.util.TreeMap;
import java.util.jar.Attributes;
import java.util.jar.Manifest;


// !!NOTE!! This class can have no dependencies on Qt since
//          it is used by the NativeLibraryManager
import org.qtjambi.qt.internal.DeploymentSpec;
import org.qtjambi.qt.internal.NativeLibraryManager;
import org.qtjambi.qt.osinfo.OSInfo;
import org.qtjambi.qt.qtjambi.util.RetroTranslatorHelper;

/**
This class contains static members that gives information and performs Qt Jambi
related tasks.
*/
public class Utilities {

    public static final String VERSION_STRING;
    public static final String LIBINFIX;
    public static final String VERSION_MAJOR_STRING;
    public static final int VERSION_MAJOR;
    public static final String QTJAMBI_SONAME_VERSION_MAJOR;
    // We use a List<> to make the collection read-only an array would not be suitable
    private static final List<String> systemLibrariesList;
    private static final List<String> jniLibdirBeforeList;
    private static final List<String> jniLibdirList;
    private static final int[] versionA;

    private static final String K_qtjambi_version              = "qtjambi.version";
    private static final String K_qtjambi_libinfix             = "qtjambi.libinfix";
    private static final String K_qtjambi_soname_version_major = "qtjambi.soname.version.major";
    private static final String K_qtjambi_system_libraries     = "qtjambi.system.libraries";
    private static final String K_qtjambi_jni_libdir_before    = "qtjambi.jni.libdir.before";
    private static final String K_qtjambi_jni_libdir           = "qtjambi.jni.libdir";  // implicit meaning of "after"
    private static final Configuration DEFAULT_CONFIGURATION = Configuration.Release;

    public static final String K_Bundle_SymbolicName = "Bundle-SymbolicName";
    public static final String K_X_QtJambi_Build     = "X-QtJambi-Build";
    public static final String K_org_qtjambi_qt    = "org.qtjambi.qt";
    public static final String K_debug               = "debug";
    public static final String K_test                = "test";
    public static final String K_release             = "release";

    static {
        String tmpINFIX_STRING = null;
        String tmpVERSION_STRING = null;
        String tmpVERSION_MAJOR_STRING = null;
        int tmpVERSION_MAJOR = 0;
        String tmpQTJAMBI_SONAME_VERSION_MAJOR = null;
        int[] tmpVERSION_A = null;
        List<String> tmpSystemLibrariesList = null;
        List<String> tmpJniLibdirBeforeList = null;
        List<String> tmpJniLibdirList = null;
        try {
            final Properties props = new Properties();
            final ClassLoader loader = Utilities.class.getClassLoader();
            if (loader == null)
                throw new ExceptionInInitializerError("Could not get classloader!");
            final InputStream in = loader.getResourceAsStream("org/qtjambi/qt/version.properties");
            if (in == null)
                throw new ExceptionInInitializerError("version.properties not found!");
            try {
                props.load(in);
            } catch (Exception ex) {
                throw new ExceptionInInitializerError("Cannot read properties!");
            }
            tmpVERSION_STRING = props.getProperty(K_qtjambi_version);
            if (tmpVERSION_STRING == null)
                throw new ExceptionInInitializerError(K_qtjambi_version + " is not set!");
            tmpINFIX_STRING = props.getProperty(K_qtjambi_libinfix);

            int dotIndex = tmpVERSION_STRING.indexOf(".");	// "4.7.4" => "4"
            if(dotIndex > 0)	// don't allow setting it be empty
                tmpVERSION_MAJOR_STRING = tmpVERSION_STRING.substring(0, dotIndex);
            else
                tmpVERSION_MAJOR_STRING = tmpVERSION_STRING;
            
            tmpVERSION_MAJOR = Integer.parseInt(tmpVERSION_MAJOR_STRING);
            
            tmpQTJAMBI_SONAME_VERSION_MAJOR = props.getProperty(K_qtjambi_soname_version_major);

            SortedMap<String,String> tmpSystemLibrariesMap = new TreeMap<String,String>();
            SortedMap<String,String> tmpJniLibdirBeforeMap = new TreeMap<String,String>();
            SortedMap<String,String> tmpJniLibdirMap = new TreeMap<String,String>();
            Enumeration<? extends Object> e = props.propertyNames();
            while (e.hasMoreElements()) {
                String key = (String) e.nextElement();
                String value = props.getProperty(key);
                if (key.equals(K_qtjambi_system_libraries) || key.startsWith(K_qtjambi_system_libraries + ".")) {
                    tmpSystemLibrariesMap.put(key, value);
                } else if(key.equals(K_qtjambi_jni_libdir_before) || key.startsWith(K_qtjambi_jni_libdir_before + ".")) {
                    tmpJniLibdirBeforeMap.put(key, value);
                } else if(key.equals(K_qtjambi_jni_libdir) || key.startsWith(K_qtjambi_jni_libdir + ".")) {
                   tmpJniLibdirMap.put(key, value);
                }
            }
            tmpVERSION_A = getVersion(tmpVERSION_STRING);
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
            e.printStackTrace();
        } finally {
            VERSION_STRING = tmpVERSION_STRING;
            VERSION_MAJOR = tmpVERSION_MAJOR;
            VERSION_MAJOR_STRING = tmpVERSION_MAJOR_STRING;
            versionA = tmpVERSION_A;
            QTJAMBI_SONAME_VERSION_MAJOR = tmpQTJAMBI_SONAME_VERSION_MAJOR;
            systemLibrariesList = tmpSystemLibrariesList;
            jniLibdirBeforeList = tmpJniLibdirBeforeList;
            jniLibdirList = tmpJniLibdirList;
            LIBINFIX = tmpINFIX_STRING;
        }
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

    /** The operating system Qt Jambi is running on. */
    public static OperatingSystem operatingSystem = decideOperatingSystem();

    /** The configuration of Qt Jambi. */
    public static Configuration configuration = decideConfiguration();

    // What is this used for, since this does not appear to be currently true.  The lib directory works just fine on windows from QtSDK.
    /** The library sub path. */
    public static String libSubPath = decideLibSubPath();

    // The purpose of this method is to resolve the names provided in the list into DSO paths relative to the project.
    public static List<String> resolveSystemLibraries(List<String> tmpSystemLibrariesList) {
        if(tmpSystemLibrariesList == null)
            return null;
        DeploymentSpec[] deploymentSpecA = NativeLibraryManager.unpack();
        if(deploymentSpecA == null)
            return null;
        List<String> resolvedList = new ArrayList<String>();
        for(String original : tmpSystemLibrariesList) {
            String s = original;
            if(OSInfo.isWindows())   // convert "/" into "\"
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

    /**
     * Returns true if the system property name contains any of the specified
     * substrings. If substrings is null or empty the function returns true
     * if the  value is non-null.
     */
    public static Boolean match(String value, Boolean defaultValue, String[] falseA, String[] trueA) {
        for (String s : trueA) {
            if (value.contains(s))
                return true;
        }
        for (String s : falseA) {
            if (value.contains(s))
                return false;
        }
        return defaultValue;
    }
    public static Boolean matchBooleanProperty(String name, Boolean ifNotExistsValue, Boolean ifExistsValue, String[] falseA, String[] trueA) {
        String value = System.getProperty(name);
        if(falseA == null)
            falseA = new String[] { "false", "no" };
        if(trueA == null)
            trueA = new String[] { "true", "yes" };
        if (value == null)
            return ifNotExistsValue;
        return match(value, ifExistsValue, falseA, trueA);
    }
    public static Boolean matchProperty(String name, Boolean ifNotExistsValue, Boolean defaultValue, String[] falseA, String[] trueA) {
        String value = System.getProperty(name);
        if (value == null)
            return ifNotExistsValue;
        return match(value, defaultValue, falseA, trueA);
    }

    public static boolean matchProperty(String name, String ... substrings) {
        String[] substringsA = substrings;
        if(substringsA == null)
            substringsA = new String[] { };
        Boolean ifExistsValue = Boolean.FALSE;
        if(substringsA.length == 0)
            ifExistsValue = Boolean.TRUE;
        return matchProperty(name, Boolean.FALSE, ifExistsValue, new String[] { }, substringsA).booleanValue();
    }

    public static void loadSystemLibraries() {
        if (systemLibrariesList != null) {
            for (String s : systemLibrariesList) {
                // FIXME: We want only append the suffix (no prefix, no Qt version, no debug extra)
                //  currently is does add a prefix (maybe also debug extra).
                NativeLibraryManager.loadSystemLibrary(s);
            }
        }
    }

    public static String[] mergeJniLibdir(String[] middle) {
        List<String> newList = new ArrayList<String>();

        if(jniLibdirBeforeList != null)
            newList.addAll(jniLibdirBeforeList);
        if(middle != null) {
            for(String s : middle)
                newList.add(s);
        }
        if(jniLibdirList != null)
            newList.addAll(jniLibdirList);

        if(newList.size() == 0)
            return middle;   // maybe null or empty
        return newList.toArray(new String[newList.size()]);
    }

    public static boolean isAvailableQtLibrary(String library) {
        return NativeLibraryManager.isAvailableQtLibrary(library, LIBINFIX, VERSION_MAJOR_STRING);
    }
    
    public static boolean isAvailableUtilityLibrary(String library, String versionString) {
        return NativeLibraryManager.isAvailableQtLibrary(library, null, versionString);
    }
    
    public static void loadQtLibrary(String library) {
    	NativeLibraryManager.loadQtLibrary(library, LIBINFIX, VERSION_MAJOR_STRING);
    }

    public static void loadUtilityLibrary(String library, String version) {
        NativeLibraryManager.loadQtLibrary(library, null, version);
    }

    public static void loadJambiLibrary(String library) {
        NativeLibraryManager.loadJambiLibrary(library);
    }

    public static void loadJambiJniLibrary(String library) {
        NativeLibraryManager.loadJambiJniLibrary(library);
    }

    public static boolean loadLibrary(String lib) {
        try {
            NativeLibraryManager.loadJniLibrary(lib);
            return true;
        } catch (Exception e) {
            e.printStackTrace();
            return false;
        }
    }

    public static File jambiTempDir() {
        return NativeLibraryManager.jambiTempDir();
    }

    private static OperatingSystem decideOperatingSystem() {
        String osName = System.getProperty("os.name").toLowerCase();
        if (osName.startsWith("windows")) return OperatingSystem.Windows;
        if (osName.startsWith("mac os x")) return OperatingSystem.MacOSX;
        if (osName.startsWith("freebsd")) return OperatingSystem.FreeBSD;
        if (osName.equals("sunos")) return OperatingSystem.SunOS;	// SunOS
        return OperatingSystem.Linux;
    }

    private static Configuration decideConfiguration() {
        Configuration configuration = null;

        final String K_org_qtjambi_qt_debug = "org.qtjambi.qt.debug";
        String debugString = System.getProperty(K_org_qtjambi_qt_debug);
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
                        System.err.println("-D" + K_org_qtjambi_qt_debug + "=" + Boolean.TRUE + "; is set");
                    }
                }
            }
        } catch(Exception e) {
            e.printStackTrace();
            // only because Configuration.Release is assumed
            System.err.println("-D" + K_org_qtjambi_qt_debug + "=" + Boolean.FALSE + "; is assumed default");
        }

        if(configuration == null)
            configuration = decideDefaultConfiguration();

        if(configuration == null)
            configuration = DEFAULT_CONFIGURATION;

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
                    String tmpBundleSymbolicName = attributes.getValue(K_Bundle_SymbolicName);
                    String tmpXQtJambiBuild = attributes.getValue(K_X_QtJambi_Build);

                    Configuration tmpConfiguration = null;
                    if(bundleSymbolicNameMatch(K_org_qtjambi_qt, tmpBundleSymbolicName)) {
                        // We found the right bundle
                        if(K_release.equals(tmpXQtJambiBuild)) {
                            tmpConfiguration = Configuration.Release;
                        } else if(K_debug.equals(tmpXQtJambiBuild)) {
                            tmpConfiguration = Configuration.Debug;
                        } else if(K_test.equals(tmpXQtJambiBuild)) {
                            tmpConfiguration = Configuration.Test;
                        } else {
                            if(tmpXQtJambiBuild == null)
                                tmpXQtJambiBuild = "<notset>";
                            // FIXME: Only JARs with/relating-to platform specific DSO parts will have X-QtJambi-Build setting.
                            //System.out.println(Utilities.class.getName() + "#decideDefaultConfiguration()  " + url.toString() + " invalid " + K_X_QtJambi_Build + ": " + tmpXQtJambiBuild);
                        }

                        // We keep checking them all
                        // If we find 2 matches this is a failure case right now, until we have resolution strategy implemented
                        if(configuration != null && tmpConfiguration != null && tmpConfiguration != configuration) {
                            // Multiple matches, ah well...
                            System.err.println(Utilities.class.getName() + "#decideDefaultConfiguration()  " +
                                configurationSource + "(" + configuration.toString() + ") != " +
                                url.toString() + "(" + tmpConfiguration.toString() + ") multiple different " +
                                K_X_QtJambi_Build + ": found in ClassPath JARs, this is currently not supported");
                            configuration = null;
                            configurationSource = null;
                            error = true;
                        } else if(tmpConfiguration != null && !error) {
                            configuration = tmpConfiguration;  // found
                            configurationSource = url.toString();
                        }
                    }
                } catch(IOException e) {
                    e.printStackTrace();
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
            e.printStackTrace();
        } catch(SecurityException e) {
            e.printStackTrace();
        }
        return configuration;
    }

    private static String decideLibSubPath() {
        return operatingSystem == OperatingSystem.Windows
                                ? "bin"
                                : "lib";
    }


    //not used
    /*private static String stripLibraryName(String lib) {
        // Strip away "lib" prefix
        if (operatingSystem != OperatingSystem.Windows)
            lib = lib.substring(3);

        int dot = -1;

        switch (operatingSystem) {
        case Windows:
            dot = lib.indexOf(".dll");
            break;
        case Linux:
            dot = lib.indexOf(".so");
            break;
        case MacOSX:
            dot = lib.indexOf("."); // makes a fair attemt at matching /.[0-9]*.(jni)|(dy)lib/
            break;
        }

        // Strip away the library postfix...
        return lib.substring(0, dot);
    }*/

    public static List<String> unpackPlugins() {
        // FIXME: The logic of this method is broken.  We should be reconfiguring the Qt libraryPath's
        //  based on the active deployment spec and putting the runtime location of that at the start
        //  of the paths list.
        // This method should be renamed if it does not actually do anything about the "unpacking"
        //  process.  Maybe it should be "resolvePluginLocations()"
        List<String> paths = NativeLibraryManager.unpackPlugins();
        if(paths != null)
            return paths;

        // FIXME: enumerate ClassPath look for qtjambi-deployment.xml
        // FIXME: Use qtjambi-deployment.xml is one exists
        // FIXME: Make a resolver method (that produces a list of automatic things found) and another method to set from list
        // FIXME: The last resort should be to find a plugins/ directory (when we have no qtjambi-deployment.xml)
        paths = new ArrayList<String>();

        String classPath = System.getProperty("java.class.path");
        String[] classPathElements = RetroTranslatorHelper.split(classPath, File.pathSeparator);
        for(String element : classPathElements) {
            File base = new File(element);
            if(base.isDirectory()) {
                File descriptorFile = new File(base, "qtjambi-deployment.xml");
                if(descriptorFile.isFile()) {
                    if(configuration == Configuration.Debug)
                        System.out.println("resolve Plugin Locations: found qtjambi-deployment.xml at " + descriptorFile.getAbsolutePath());
                }
                // Assume a default plugins layout
                File pluginsDir = new File(base, "plugins");
                if(pluginsDir.isDirectory()) {
                    if(configuration == Configuration.Debug)
                        System.out.println("resolve Plugin Locations: found plugins/ at " + pluginsDir.getAbsolutePath());
                    paths.add(pluginsDir.getAbsolutePath());
                } else {
                    if(configuration == Configuration.Debug)
                        System.out.println("resolve Plugin Locations: found DIRECTORY at " + base.getAbsolutePath());
                }
            } else if(element.toLowerCase().endsWith(".jar")) {
                // FIXME: We should only load MANIFEST.MF qtjambi-deployment.xml from the JAR we activated
                if(configuration == Configuration.Debug)
                    System.out.println("resolve Plugin Locations: found JAR at " + base.getAbsolutePath());
                if(base.exists()){
                	try {
						URL url = base.toURI().toURL();
						url = new URL("jar:"+url.toString()+"!/plugins");
						paths.add(url.toString());
					} catch (MalformedURLException e) {
						if(configuration == Configuration.Debug)
							e.printStackTrace();
					}
                }
            } else {
                if(configuration == Configuration.Debug)
                    System.out.println("resolve Plugin Locations: found FILE at " + base.getAbsolutePath());
            }
        }

        if(paths.isEmpty())
            return null;
        return paths;
    }

    // This is implemented so that String#replace(char, char) is not used as this is regexp based
    //  and sounds heavy weight for us.  Maybe I should performance test this point before just
    //  implementing this but bleh!
    public static String stringCharReplace(String s, char fromChar, char toChar) {
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

    // a - haystack, b - needle
    public static boolean stringCompareAt(CharSequence a, int aOffset, CharSequence b, int bOffset) {
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

    public static String stringCharReplace(String s, CharSequence fromCharSequence, CharSequence toCharSequence) {
        final int len = s.length();
        char[] cA = new char[fromCharSequence.length()];
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
        int[] vA = new int[versionA.length];
        System.arraycopy(versionA, 0, vA, 0, versionA.length);
        return vA;
    }

    /**
     * Converts the string version number into an array of
     * @param versionString Version string such as "1.2.3"
     * @return Array of <code>new int[] { 1, 2, 3 };</code>
     * @throws NumberFormatException
     */
    public static int[] getVersion(String versionString) {
        String[] sA = RetroTranslatorHelper.split(versionString, ".");
        int[] vA = new int[sA.length];
        int i = 0;
        for(String s : sA)
            vA[i++] = Integer.valueOf(s);
        return vA;
    }
    
    public static boolean initializePackage(String packagePath){
    	if(initializedPackages.contains(packagePath)){
    		return true;
    	}
    	try {
			java.lang.reflect.Method init = Class.forName(packagePath + ".QtJambi_LibraryInitializer").getDeclaredMethod("init");
			init.setAccessible(true);
			init.invoke(null);
			initializedPackages.add(packagePath);
			return true;
		} catch (Throwable e) {
	    	return false;
		}
    }
    
    private static HashSet<String> initializedPackages = new HashSet<String>();
}
