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

package org.qtjambi.qt.internal;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.URL;
import java.util.ArrayList;
import java.util.Enumeration;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.xml.parsers.SAXParser;
import javax.xml.parsers.SAXParserFactory;

import org.xml.sax.helpers.DefaultHandler;

import org.qtjambi.qt.Utilities;
import org.qtjambi.qt.Utilities.OperatingSystem;
import org.qtjambi.qt.osinfo.OSInfo;
import org.qtjambi.qt.qtjambi.util.RetroTranslatorHelper;

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
 *   org_qtjambi_qt_core libraries. These are loaded at runtime on
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
 * the <code>-Dorg.qtjambi.qt.verbose-loading</code> system property
 * to the Virtual Machine. It possible to specify that the native
 * library manager should load debug versions of libraries as
 * well. This is done by specifying the system property
 * </code>-Dorg.qtjambi.qt.debug</code>
 *
 */
public class NativeLibraryManager {

    public static String DEPLOY_DESCRIPTOR_NAME = "qtjambi-deployment.xml";

    private static final String DEBUG_SUFFIX = "_debuglib";

    private static final boolean VERBOSE_LOADING = System.getProperty("org.qtjambi.qt.verbose-loading") != null;

    private static final int LOAD_TRUE = 1;
    private static final int LOAD_SYSTEM = 2;
    private static final int LOAD_FALSE = 3;
    private static final int LOAD_NEVER = 4;

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
                } else if (!system.equals(OSInfo.osArchName())) {
                    throw new WrongSystemException("trying to load: '" + system
                                                   + "', expected: '" + OSInfo.osArchName() + "'");
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
        return new File(tmpDir, "QtJambi_" + user + "_" + arch + "_" + Utilities.VERSION_STRING + "_" + key);
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
     * When the system property <code>-Dorg.qtjambi.qt.debug</code>
     * is specified, the suffix <code>_debuglib</code> will be appended
     * to the filename, replacing "qtjambi" above with "qtjambi_debuglib".
     *
     * @param library The name of the library..
     */
    public static void loadJniLibrary(String library) {
        String version;
        if(Utilities.getVersion()[0]>=5){
        	version = "" + Utilities.VERSION_MAJOR_STRING;
        }else{
        	version = Utilities.QTJAMBI_SONAME_VERSION_MAJOR;
        }
        String lib = jniLibraryName(library, version);
        loadNativeLibrary(lib);
    }

    public static void loadJambiJniLibrary(String library) {
        loadJniLibrary(library);
    }

    public static void loadJambiLibrary(String library) {
        String version;
        if(Utilities.getVersion()[0]>=5){
        	version = "" + Utilities.VERSION_MAJOR_STRING;
        }else{
        	version = Utilities.QTJAMBI_SONAME_VERSION_MAJOR;
        }
        String lib = jambiLibraryName(library, version);
        loadNativeLibrary(lib);
    }


    /**
     * Overload which passes the default value of "5" as the version
     */
    public static void loadQtLibrary(String library) {
        loadQtLibrary(library, "5", null);
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
     *
     * @param library The name of the library..
     */
    public static void loadQtLibrary(String library, String libInfix, String version) {
        String lib = qtLibraryName(library, libInfix, version);
        loadNativeLibrary(lib);
    }

    // FIXME: Use proper dependency tree to loadQtLibrary() but this will do for now
    public static boolean isAvailableQtLibrary(String library, String libInfix, String version) {
        unpack();
    	String lib = qtLibraryName(library, libInfix, version);  // "QtDBus" => "libQtDBus.so.4"
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
		                String[] pathA = RetroTranslatorHelper.split(name, "/");
		                if(pathA == null || pathA.length == 0)
		                    continue;
		                String lastPart = pathA[pathA.length - 1];
		                if(lib.equals(lastPart))  // lib=="libQtFoo.so.4"
		                    return true;
		            }
	            }
	        }
        }
    	String libPaths = System.getProperty("org.qtjambi.qt.library-path-override");
        if (libPaths == null || libPaths.length() == 0) {
            libPaths = System.getProperty("java.library.path");
        }

        String[] paths = null;
        if (libPaths != null)
            paths = RetroTranslatorHelper.split(libPaths, File.pathSeparator);
        paths = Utilities.mergeJniLibdir(paths);
        if(paths != null) {
            for (String path : paths) {
                File f = new File(path, lib);
                if (f.exists()) {
                	return true;
                }
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
    private static void loadNativeLibrary(String lib) {
        try {
            loadLibrary_helper(lib);
            if (VERBOSE_LOADING)
                System.out.println(reporter.recentReports());

        } catch (Throwable e) {
            throw new RuntimeException("Loading library failed, progress so far:\n" + reporter, e);
        }
    }


    private static void loadLibrary_helper(String lib) {
        unpack();

        reporter.report("Loading library: '", lib, "'...");

        // First of all verify that we're allowed to load this library...
        LibraryEntry e = neverLoad.get(lib);
        if (e != null) {
            throw new RuntimeException("Library '" + lib + "' cannot be loaded, deploy spec");
        }

        // Try to load via deploy spec...
        e = libraryMap.get(lib);
        if (e != null) {

            if (e.isLoaded()) {
                reporter.report(" - already loaded, skipping...");
                return;
            }

            File libFile = e.getDeploymentSpec().buildPath(e.getName());
            reporter.report(" - using deployment spec at " + libFile.getAbsolutePath());
            Runtime.getRuntime().load(libFile.getAbsolutePath());
            reporter.report(" - ok!");
            e.setLoaded(true);

        // Load via System.load() using default paths..
        } else {
            boolean loaded = false;
            String libPaths = System.getProperty("org.qtjambi.qt.library-path-override");
            if (libPaths != null && libPaths.length() > 0) {
                reporter.report(" - using 'org.qtjambi.qt.library-path-override'");
            } else {
                reporter.report(" - using 'java.library.path'");
                libPaths = System.getProperty("java.library.path");
            }

            String[] paths = null;
            if (libPaths != null)
                paths = RetroTranslatorHelper.split(libPaths, File.pathSeparator);
            paths = Utilities.mergeJniLibdir(paths);
            if(paths != null) {
                for (String path : paths) {
                    File f = new File(path, lib);
                    if (f.exists()) {
                        Runtime.getRuntime().load(f.getAbsolutePath());
                        reporter.report(" - ok, path was: " + f.getAbsolutePath());
                        loaded = true;
                        break;
                    }
                }
            }
            if (!loaded) {
                throw new RuntimeException("Library '" + lib +"' was not found in 'java.library.path="
                                           + libPaths + "'");
            }
        }
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
                spec.setBaseUrl(new URL(Utilities.convertAbsolutePathStringToFileUrlString(tmpDir)));
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
                            throw new FileNotFoundException("Dirent '" + path
                                                            + "' specified in qtjambi-deployment.xml in '"
                                                            + jarName + "' does not exist");
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
                            throw new FileNotFoundException("Library '" + e.getName()
                                                            + "' specified in qtjambi-deployment.xml in '"
                                                            + jarName + "' does not exist");
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
            spec.setBaseUrl(new URL(Utilities.convertAbsolutePathStringToFileUrlString(tmpDir)));
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
        switch (Utilities.operatingSystem) {
        case Windows: 
        	if (Utilities.configuration == Utilities.Configuration.Debug)
                lib += DEBUG_SUFFIX;
        	if(Utilities.VERSION_MAJOR>=5){
        		return lib + version + ".dll";  // "foobar1.dll"
        	}else{
        		return lib + ".dll";  // "foobar1.dll"
        	}
        case MacOSX: 
        	if (Utilities.configuration == Utilities.Configuration.Debug)
                lib += DEBUG_SUFFIX;
        	return "lib" + lib + dotVersion + ".jnilib";  // "libfoobar.1.jnilib"
        case Linux:
        case FreeBSD: return "lib" + lib + ".so" + dotVersion;  // "libfoobar.so.1"
        }
        throw new RuntimeException("Unreachable statement");
    }


    private static String jambiLibraryName(String lib, String version) {
        String dotVersion;
        if(version != null) {
            dotVersion = "." + version;
        } else {
            version = "";
            dotVersion = "";
        }
        switch (Utilities.operatingSystem) {
        case Windows: {
        	if (Utilities.configuration == Utilities.Configuration.Debug)
                lib += DEBUG_SUFFIX;
        	if(Utilities.VERSION_MAJOR>=5){
        		return lib + version + ".dll";  // "foobar1.dll"
        	}else{
        		return lib + ".dll";  // "foobar1.dll"
        	}
        }
        case MacOSX: 
        	if (Utilities.configuration == Utilities.Configuration.Debug)
                lib += DEBUG_SUFFIX;
        	return "lib" + lib + dotVersion + ".dylib";  // "libfoobar.1.dylib"
        case Linux:
        case FreeBSD: return "lib" + lib + ".so" + dotVersion;  // "libfoobar.so.1"
        }
        throw new RuntimeException("Unreachable statement");
    }


    private static String qtLibraryName(String lib, String libInfix, String version) {
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
        if("5".equals(version)){
    		if(lib.startsWith("Qt") && !lib.startsWith("Qt5")){
    			lib = "Qt5"+lib.substring(2);
    		}
    		version = "";
    	}
        String prefix = lib.startsWith("lib") ? "" : "lib";
        switch (Utilities.operatingSystem) {
        case Windows:
            return Utilities.configuration == Utilities.Configuration.Debug
                ? lib + libInfix + "d" + version + ".dll"  // "QtFood4.dll"
                : lib + libInfix + version + ".dll";  // "QtFoo4.dll"
        case MacOSX:
            return Utilities.configuration == Utilities.Configuration.Debug
                ? prefix + lib + libInfix + "_debug" + dotVersion + ".dylib"  // "libQtFoo_debug.4.dylib"
                : prefix + lib + libInfix + dotVersion + ".dylib";  // "libQtFoo.4.dylib"
        case Linux:
        case FreeBSD:
            // Linux doesn't have a dedicated "debug" library since 4.2
            return prefix + lib + libInfix + ".so" + dotVersion;  // "libQtFoo.so.4"
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

    private static boolean unpacked = false;

    public static void main(String args[]) throws Exception {

        unpack();

        loadQtLibrary("QtCore");
        loadQtLibrary("QtGui");
        loadJambiJniLibrary("qtjambi");
        loadJambiJniLibrary("org_qtjambi_qt_core");
        loadJambiJniLibrary("org_qtjambi_qt_gui");
        loadQtLibrary("QtGui");
        loadQtLibrary("QtNetwork");
//         loadJambiJniLibrary("org_qtjambi_qt_network");

        for (String s : pluginPaths())
            System.out.println("PluginPath: " + s);

        for (String s : pluginDesignerPaths())
            System.out.println("PluginDesignerPath: " + s);

    }

}
