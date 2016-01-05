/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2015 Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
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
** $END_LICENSE$

**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

package org.qtjambi.tools.ant;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStream;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.Collections;
import java.util.Date;
import java.util.Enumeration;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Properties;
import java.util.Set;
import java.util.StringTokenizer;

import javax.swing.SpringLayout.Constraints;

import org.apache.tools.ant.BuildException;
import org.apache.tools.ant.Project;
import org.apache.tools.ant.PropertyHelper;
import org.qtjambi.qt.osinfo.OSInfo;
import org.qtjambi.tools.ant.FindCompiler.Compiler;

public class InitializeBuildTask extends AbstractInitializeTask {

    private String qtVersion;
    private int qtMajorVersion;
    private int qtMinorVersion;
    private int qtPatchlevelVersion;
    private String qtVersionSource = "";
    private String versionSuffix;       // beta4
    private String libInfix = "";

    private String pathVersionProperties            = "version.properties";
    private String pathVersionPropertiesTemplate    = "version.properties.template";

    private String[] generatorPreProcStageOneA;
    private String[] generatorPreProcStageTwoA;

    // did somebody mention something about methods never being longer than 10 lines?
    public void executeInitialize() throws BuildException {
        String sourceValue;

        String sep = AntUtil.getPropertyAsString(propertyHelper, "sep");
        if(sep == null) {
            sep = File.separator;
            sourceValue = " (auto-detect)";
        } else {
            sourceValue = null;
        }
        mySetProperty(-1, "sep", sourceValue, sep, false);

        String psep = AntUtil.getPropertyAsString(propertyHelper, "psep");
        if(psep == null) {
            psep = File.pathSeparator;
            sourceValue = " (auto-detect)";
        } else {
            sourceValue = null;
        }
        mySetProperty(-1, "psep", sourceValue, psep, false);

        File jambiDirectoryAbspath = null;
        {
            sourceValue = null;
            String value = AntUtil.getPropertyAsString(propertyHelper, Constants.DIRECTORY);
            File dir = null;
            if(value == null) {
                // If this is not set we can auto-configure from $JAMBIDIR
                String JAMBIDIR = System.getenv("JAMBIDIR");
                if(JAMBIDIR != null)
                    getProject().log(this, "JAMBIDIR is set: " + prettyValue(JAMBIDIR), Project.MSG_INFO);
                dir = new File(JAMBIDIR);
                sourceValue = " (from envvar:JAMBIDIR)";
            } else {
                dir = new File(value);
            }
            if(dir != null) {
                if(dir.isDirectory() == false)
                    sourceValue = " (WARNING: path does not exist or is not a directory)";
                else
                    jambiDirectoryAbspath = ResolvePathTask.resolve(dir);
            }
            mySetProperty(-1, Constants.DIRECTORY, sourceValue, value, false);
        }

        {
            int thisVerbose = 0;  // quiet
            sourceValue = null;
            String value = AntUtil.getPropertyAsString(propertyHelper, Constants.DIRECTORY_ABSPATH);
            if(value != null) {
                if(jambiDirectoryAbspath != null) {
                    String path = jambiDirectoryAbspath.getAbsolutePath();
                    if(value.equals(path) == false) {
                        sourceValue = " (WARNING MISMATCH: " + path + ")";
                        thisVerbose = -1;  // noisy (because of WARNING)
                    }
                }
            } else if(jambiDirectoryAbspath != null) {
                value = jambiDirectoryAbspath.getAbsolutePath();
                thisVerbose = -1;  // noisy (because we set it)
            }
            mySetProperty(thisVerbose, Constants.DIRECTORY_ABSPATH, null, value, false);
        }

        String QTDIR = System.getenv("QTDIR");   // used here
        if(QTDIR == null){
			QTDIR = AntUtil.getPropertyAsString(propertyHelper, "QTDIR");
		}
		if(QTDIR != null)
            getProject().log(this, "QTDIR is set: " + prettyValue(QTDIR), Project.MSG_INFO);
        else if(OSInfo.isWindows() || OSInfo.isMacOS())
            getProject().log(this, "WARNING QTDIR is not set, yet this platform usually requires it to be set", Project.MSG_WARN);
        else{
        	String value = AntUtil.getPropertyAsString(propertyHelper, Constants.BINDIR);
        	if(value!=null){
        		File binDir = new File(value);
        		if(binDir.isDirectory()){
        			QTDIR = binDir.getParent();
        		}
        	}
        }

        final String[] emitA = {
            Constants.BINDIR,
            Constants.LIBDIR,
            Constants.INCLUDEDIR,
            Constants.PLUGINSDIR,
            Constants.IMPORTSDIR,
            Constants.QMLMODULESDIR
        };
        for(String emit : emitA) {
            sourceValue = null;
            String value = AntUtil.getPropertyAsString(propertyHelper, emit);
            File dir = null;
            if(value != null) {
                dir = new File(value);
            } else if(value == null && QTDIR != null) {
                dir = resolveDirectoryViaQTDIR(QTDIR, emit);
                if(dir != null) {
                    value = dir.getAbsolutePath();
                    sourceValue = " (from envvar:QTDIR)";
                }
            }
            if(dir != null) {
                if(dir.isDirectory() == false)
                    sourceValue = " (WARNING: path does not exist or is not a directory)";
            }
            mySetProperty(-1, emit, sourceValue, value, false);
        }

        final String[] emitB = {
            Constants.QTJAMBI_PHONON_INCLUDEDIR,
            Constants.QTJAMBI_PHONON_LIBDIR,
            Constants.QTJAMBI_PHONON_PLUGINSDIR
        };
        for(String emit : emitB) {
            sourceValue = null;
            String value = AntUtil.getPropertyAsString(propertyHelper, emit);
            File dir = null;
            if(value != null) {
                dir = new File(value);
            } else if(value == null) {
                String x = null;
                if(Constants.QTJAMBI_PHONON_INCLUDEDIR.equals(emit))
                    x = Constants.INCLUDEDIR;
                else if(Constants.QTJAMBI_PHONON_LIBDIR.equals(emit))
                    x = Constants.LIBDIR;
                else if(Constants.QTJAMBI_PHONON_PLUGINSDIR.equals(emit))
                    x = Constants.PLUGINSDIR;
                if(x != null)
                    value = AntUtil.getPropertyAsString(propertyHelper, x);
                if(value != null)
                    sourceValue = " (inherited from ${" + x + "})";
            }
            if(dir != null) {
                if(dir.isDirectory() == false)
                    sourceValue = " (WARNING: path does not exist or is not a directory)";
            }
            mySetProperty(-1, emit, sourceValue, value, false);
        }

        {
            sourceValue = null;
            // FIXME: we could auto-configure this instead of setenv.xml doing that
            String value = AntUtil.getPropertyAsString(propertyHelper, Constants.QTJAMBI_PHONON_KDEPHONON_PATH);
            if(value == null)
                value = "";   // set this empty-string default here
            mySetProperty(-1, Constants.QTJAMBI_PHONON_KDEPHONON_PATH, sourceValue, value, false);
        }

        {
            sourceValue = null;
            String qtQmake = AntUtil.getPropertyAsString(propertyHelper, Constants.QMAKE);
            String detectedQmake = QMakeTask.executableName();
            if(qtQmake == null) {
                qtQmake = detectedQmake;
                sourceValue = " (default)";
            } else if(qtQmake.equals(detectedQmake) == false) {
                sourceValue = " (WARNING expected " + detectedQmake + ")";
            }
            mySetProperty(-1, Constants.QMAKE, sourceValue, qtQmake, false);

            String detectedQmakeAbsolutePath = QMakeTask.resolveExecutableAbsolutePath(getProject(), qtQmake);

            String qtQmakeAbspath = AntUtil.getPropertyAsString(propertyHelper, Constants.QMAKE_ABSPATH);
            if(qtQmakeAbspath == null) {
                qtQmakeAbspath = detectedQmakeAbsolutePath;
                if(qtQmakeAbspath != null)
                    sourceValue = " (auto-detected)";
                else
                    sourceValue = " (WARNING not found)";
            } else {
                if(qtQmakeAbspath.equals(detectedQmakeAbsolutePath) == false)
                    sourceValue = " (WARNING detected at " + detectedQmakeAbsolutePath + ")";
            }
            mySetProperty(-1, Constants.QMAKE_ABSPATH, sourceValue, qtQmakeAbspath, false);
        }

        FindCompiler finder = new FindCompiler(this);

        String detectedOsname = OSInfo.osArchName();
        String osname = AntUtil.getPropertyAsString(propertyHelper, Constants.OSNAME);
        if(osname == null) {
            sourceValue = " (auto-detected)";
            osname = detectedOsname;
        } else {
            if("help".equals(osname) || "?".equals(osname)) {
                OSInfo.OS[] values = OSInfo.OS.values();
                String s = Arrays.toString(values);
                throw new BuildException(Constants.OSNAME + " valid values: " + s);
            }
            sourceValue = " (detected: " + detectedOsname + ")";
        }
        mySetProperty(-1, Constants.OSNAME, sourceValue, osname, false);  // report value

        Compiler compiler;
        {
            Compiler detectedCompiler = finder.decideCompiler();
            String compilerValue = AntUtil.getPropertyAsString(propertyHelper, Constants.COMPILER);
            if(compilerValue == null) {
                sourceValue = " (available compilers: " + finder.getAvailableCompilers() + "; auto-detected)";
                compilerValue = detectedCompiler.toString();
            } else {
                if("help".equals(compilerValue) || "?".equals(compilerValue)) {
                    Compiler[] values = Compiler.values();
                    String s = Arrays.toString(values);
                    throw new BuildException(Constants.COMPILER + " valid values: " + s);
                }
                sourceValue = " (available compilers: " + finder.getAvailableCompilers() + "; detected: " + detectedCompiler + ")";
            }
            mySetProperty(-1, Constants.COMPILER, sourceValue, compilerValue, false);  // report value
            compiler = Compiler.resolve(compilerValue);
        }

        String vsredistdirValue = AntUtil.getPropertyAsString(propertyHelper, Constants.VSREDISTDIR);
        if(vsredistdirValue != null) {
            sourceValue = null; // " (environment variable $" + "VSREDISTDIR" + ")"; // not strictly true it could be set by ant cmdline
            mySetProperty(-1, Constants.VSREDISTDIR_PACKAGE, sourceValue, vsredistdirValue, false);  // report value

            String vsredistdirPackageValue = AntUtil.getPropertyAsString(propertyHelper, Constants.VSREDISTDIR_PACKAGE);
            if(vsredistdirPackageValue == null) {
                sourceValue = " (default value; feature need explicit enabling)";
                vsredistdirPackageValue = "false";
            } else {
                sourceValue = null;
            }
            mySetProperty(-1, Constants.VSREDISTDIR_PACKAGE, sourceValue, vsredistdirPackageValue, false);  // report value
        } else {
            mySetProperty(0, Constants.VSREDISTDIR_PACKAGE, null, "false", false);  // silently set false
        }

        String CROSS_COMPILE = System.getenv("CROSS_COMPILE");   // used here
        if(CROSS_COMPILE != null)
            getProject().log(this, "CROSS_COMPILE is set: " + prettyValue(CROSS_COMPILE), Project.MSG_INFO);

        if(OSInfo.isMacOS())
            mySetProperty(0, Constants.QTJAMBI_CONFIG_ISMACOSX, " (set by init)", "true", false);

        String s;

        s = null;
        if(OSInfo.isLinux())
            s = OSInfo.K_LINUX;
        else if(OSInfo.isWindows())
            s = OSInfo.K_WINDOWS;
        else if(OSInfo.isMacOS())
            s = OSInfo.K_MACOSX;
        else if(OSInfo.isFreeBSD())
            s = OSInfo.K_FREEBSD;
        else if(OSInfo.isSolaris())
            s = OSInfo.K_SUNOS;
        if(s != null)
            AntUtil.setNewProperty(propertyHelper, Constants.OSPLATFORM, s);

        s = null;

        if(true) {    // FIXME lookup if verbose is set
            Properties props = System.getProperties();
            Enumeration e = props.propertyNames();
            while(e.hasMoreElements()) {
                String k = (String) e.nextElement();
                Object v = System.getProperty(k);
                getProject().log("systemProperty[" + k + "] = " + v, Project.MSG_VERBOSE);
            }
        }

        String javaOsArch = System.getProperty("os.arch");	// arm|
        if("arm".equals(javaOsArch)) {
            // FIXME get LE or BE
            s = "arm";
        } else {
            if(osname.endsWith("64"))
                s = "x86_64";
            else
                s = "i386";
        }
        if(s != null)
            AntUtil.setNewProperty(propertyHelper, Constants.OSCPU, s);
        String osCpu = s;

        finder.checkCompilerDetails();
        
        mySetProperty(-1, Constants.EXEC_STRIP, null, null, false);  // report value

        String JAVA_HOME = System.getenv("JAVA_HOME");   // used here
        if(JAVA_HOME != null)
            getProject().log(this, "JAVA_HOME is set: " + prettyValue(JAVA_HOME), Project.MSG_INFO);
        else
            getProject().log(this, "WARNING JAVA_HOME is not set, this is usually required to be set", Project.MSG_WARN);

        String javaHomeTarget = decideJavaHomeTarget();
        if(javaHomeTarget == null)
            throw new BuildException("Unable to determine JAVA_HOME_TARGET, setup environment variable JAVA_HOME (or JAVA_HOME_TARGET) or edit build.properties");

        String javaOsarchTarget = decideJavaOsarchTarget();
        if(javaOsarchTarget == null) {
            if(OSInfo.isMacOS() == false)  // On MacOSX there is no sub-dir inside the JDK include directory that contains jni.h
                throw new BuildException("Unable to determine JAVA_OSARCH_TARGET, setup environment variable JAVA_OSARCH_TARGET or edit build.properties");
        }

        String javaOscpu = decideJavaOscpu();
        if(javaOscpu == null)
            throw new BuildException("Unable to determine JAVA_OSCPU, setup environment variable JAVA_OSCPU or edit build.properties");

        String javaOscpuTarget = decideJavaOscpuTarget();
        if(javaOscpuTarget  == null)
                throw new BuildException("Unable to determine JAVA_OSCPU_TARGET, setup environment variable JAVA_OSCPU_TARGET or edit build.properties");

        {
            sourceValue = null;
            String toolsBindir = AntUtil.getPropertyAsString(propertyHelper, Constants.TOOLS_BINDIR);
            if(toolsBindir == null) {
                String bindir = AntUtil.getPropertyAsString(propertyHelper, Constants.BINDIR);
                if(bindir != null) {
                   sourceValue = " (inherited from ${" + Constants.BINDIR + "})";
                   toolsBindir = bindir;
                }
            }
            mySetProperty(-1, Constants.TOOLS_BINDIR, sourceValue, toolsBindir, false);  // report value
        }
        {
            sourceValue = null;
            String toolsLibdir = AntUtil.getPropertyAsString(propertyHelper, Constants.TOOLS_LIBDIR);
            if(toolsLibdir == null) {
                String libdir = AntUtil.getPropertyAsString(propertyHelper, Constants.LIBDIR);
                if(libdir != null) {
                    sourceValue = " (inherited from ${" + Constants.LIBDIR + "})";
                    toolsLibdir = libdir;
                }
            }
            mySetProperty(-1, Constants.TOOLS_LIBDIR, sourceValue, toolsLibdir, false);  // report value
        }
        {
            sourceValue = null;
            String toolsQmake = AntUtil.getPropertyAsString(propertyHelper, Constants.TOOLS_QMAKE);
            if(toolsQmake == null) {
                String qmake = AntUtil.getPropertyAsString(propertyHelper, Constants.QMAKE);
                sourceValue = " (inherited from ${" + Constants.QMAKE + "})";
                toolsQmake = qmake;
            }
            mySetProperty(-1, Constants.TOOLS_QMAKE, sourceValue, toolsQmake, false);  // report value

            // This method needs to look in Constants.TOOLS_BINDIR not Constants.BINDIR
            String detectedQmakeAbsolutePath = QMakeTask.resolveExecutableAbsolutePath(getProject(), toolsQmake, Constants.TOOLS_BINDIR);

            String toolsQmakeAbspath = AntUtil.getPropertyAsString(propertyHelper, Constants.TOOLS_QMAKE_ABSPATH);
            if(toolsQmakeAbspath == null) {
                toolsQmakeAbspath = detectedQmakeAbsolutePath;
                if(toolsQmakeAbspath != null)
                    sourceValue = " (auto-detected)";
                else
                    sourceValue = " (WARNING not found)";
            } else {
                if(toolsQmakeAbspath.equals(detectedQmakeAbsolutePath) == false)
                    sourceValue = " (WARNING detected at " + detectedQmakeAbsolutePath + ")";
            }
            mySetProperty(-1, Constants.TOOLS_QMAKE_ABSPATH, sourceValue, toolsQmakeAbspath, false);
        }


        if(!decideQtVersion())
            throw new BuildException("Unable to determine Qt version, try editing: " + pathVersionPropertiesTemplate);
        s = String.valueOf(qtVersion);
        analyzeLibinfix(new File(QTDIR));
        
        if(OSInfo.isMacOS()){
	        String wantedSdk = AntUtil.getPropertyAsString(propertyHelper, Constants.QTJAMBI_MACOSX_MAC_SDK);
	        detectMacosxSdk(wantedSdk);
        }
        if(qtMajorVersion>=5){
        	AntUtil.setProperty(propertyHelper, "qt.feature.QT_NO_STYLE_PLASTIQUE", "true");
        }

        getProject().log(this, Constants.QT_VERSION + " is " + s + qtVersionSource, Project.MSG_VERBOSE);
        AntUtil.setNewProperty(propertyHelper, Constants.QT_VERSION, s);
        AntUtil.setNewProperty(propertyHelper, Constants.VERSION, s); // this won't overwrite existing value

        s = String.valueOf(qtMajorVersion);
        getProject().log(this, Constants.QT_VERSION_MAJOR + " is " + s, Project.MSG_VERBOSE);
        AntUtil.setNewProperty(propertyHelper, Constants.QT_VERSION_MAJOR, s);
        AntUtil.setNewProperty(propertyHelper, Constants.QT_VERSION_MAJOR_NEXT, String.valueOf(qtMajorVersion + 1));
        AntUtil.setNewProperty(propertyHelper, Constants.QT_VERSION_MINOR,      String.valueOf(qtMinorVersion));
        AntUtil.setNewProperty(propertyHelper, Constants.QT_VERSION_MINOR_NEXT, String.valueOf(qtMinorVersion + 1));
        AntUtil.setNewProperty(propertyHelper, Constants.QT_VERSION_PATCHLEVEL, String.valueOf(qtPatchlevelVersion));


        versionSuffix = AntUtil.getPropertyAsString(propertyHelper, Constants.SUFFIX_VERSION);
        mySetProperty(-1, Constants.SUFFIX_VERSION, null, null, false);  // report

        String canonVersionSuffix;
        if(versionSuffix != null)
            canonVersionSuffix = versionSuffix;
        else
            canonVersionSuffix = "";
        String bundleVersionMode = AntUtil.getPropertyAsString(propertyHelper, Constants.BUNDLE_VERSION_MODE);
        if(bundleVersionMode != null) {
            if(bundleVersionMode.equals("auto-suffix-date")) {
                SimpleDateFormat sdf = new SimpleDateFormat("yyyyMMdd");
                s = qtVersion + "." + sdf.format(new Date()) + canonVersionSuffix;
            }
        } else {
            s = qtVersion + canonVersionSuffix;
        }
        mySetProperty(-1, Constants.BUNDLE_VERSION, null, s, false);


        {   // Need to detect the version of Qt we a reworking with before deciding configuration
            String wantedConfiguration = AntUtil.getPropertyAsString(propertyHelper, Constants.CONFIGURATION);
            detectConfiguration(wantedConfiguration);  // all the magic in in here now
        }

        // Moved until after auto-detect of configuration is complete
        s = null;
        if(Constants.CONFIG_RELEASE.equals(getConfiguration()))
            s = "";   // empty
        else if(Constants.CONFIG_DEBUG.equals(getConfiguration()))
            s = "-debug";
        else if(Constants.CONFIG_TEST.equals(getConfiguration()))
            s = "-test";
        else
            s = "";   // empty (debug_and_release)
        if(s != null)
            AntUtil.setNewProperty(propertyHelper, Constants.CONFIGURATION_DASH, s);
        s = null;
        if(Constants.CONFIG_RELEASE.equals(getConfiguration()))
            s = "";   // empty
        else if(Constants.CONFIG_DEBUG.equals(getConfiguration()))
            s = ".debug";
        else if(Constants.CONFIG_TEST.equals(getConfiguration()))
            s = ".test";
        else
            s = "";   // empty (debug_and_release)
        if(s != null)
            AntUtil.setNewProperty(propertyHelper, Constants.CONFIGURATION_OSGI, s);

        {
            sourceValue = null;
            String value = AntUtil.getPropertyAsString(propertyHelper, Constants.QTJAMBI_DEBUG_TOOLS);
            if(value == null) {
                if(isConfigurationDebug() || isConfigurationTest())
                    value = Boolean.TRUE.toString();
                else
                    value = Boolean.FALSE.toString();
                sourceValue = " (auto-configured based on ${" + Constants.CONFIGURATION + "})";
            }
            mySetProperty(-1, Constants.QTJAMBI_DEBUG_TOOLS, sourceValue, value, false);
        }

        {
            sourceValue = null;
            String qmakeTargetDefault = AntUtil.getPropertyAsString(propertyHelper, Constants.QMAKE_TARGET_DEFAULT);
            if(qmakeTargetDefault == null) {
                // We only need to override the default when the Qt SDK is debug_and_release but
                //  we are only building the project for one kind.
//              if(Constants.CONFIG_RELEASE.equals(configuration))
//                  qmakeTargetDefault = configuration;
//              else if(Constants.CONFIG_DEBUG.equals(configuration))
//                  qmakeTargetDefault = configuration;
                if(Constants.CONFIG_DEBUG_AND_RELEASE.equals(getConfiguration()))
                    qmakeTargetDefault = "all";
              else
                    qmakeTargetDefault = "";  // must be set to empty string (avoid ant subst ${qmake.target.default})
                // FIXME: We want ${qtjambi.configuration} to set from QTDIR build kind *.prl data
//                sourceValue = " (set from ${qtjambi.configuration})";
            }
            mySetProperty(-1, Constants.QMAKE_TARGET_DEFAULT, sourceValue, qmakeTargetDefault, false);  // report value
        }


        if(qtMajorVersion<5){
            String sonameVersionMajor = Constants.DEFAULT_QTJAMBI_SONAME_VERSION_MAJOR;
            String sonameSource = " (set by init)";
        	mySetProperty(-1, Constants.QTJAMBI_SONAME_VERSION_MAJOR, sonameSource, sonameVersionMajor, false);
        }else{
        	String sonameVersionMajor = ""+qtMajorVersion;
            String sonameSource = " (taken from qt version)";
        	mySetProperty(-1, Constants.QTJAMBI_SONAME_VERSION_MAJOR, sonameSource, sonameVersionMajor, false);
        }

        String cachekeyVersion = AntUtil.getPropertyAsString(propertyHelper, Constants.CACHEKEY);
        String cachekeyVersionSource = " (already set)";
        if(cachekeyVersion == null) {   // auto-configure
            cachekeyVersionSource = " (set by init)";
            // ${qtjambi.compiler}${qtjambi.configuration.dash}-${DSTAMP}-${TSTAMP}
            cachekeyVersion = propertyHelper.replaceProperties(null, "${qtjambi.compiler}${qtjambi.configuration.dash}-${DSTAMP}-${TSTAMP}", null);
        }
        mySetProperty(-1, Constants.CACHEKEY, cachekeyVersionSource, cachekeyVersion, false);


        if(!decideGeneratorPreProc(osCpu))
            throw new BuildException("Unable to determine generator pre-processor settings");
        s = Util.safeArrayToString(generatorPreProcStageOneA);
        getProject().log(this, Constants.GENERATOR_PREPROC_STAGE1 + " is " + ((s != null) ? s : "<unset>"), Project.MSG_VERBOSE);
        AntUtil.setNewProperty(propertyHelper, Constants.GENERATOR_PREPROC_STAGE1, Util.safeArrayJoinToString(generatorPreProcStageOneA, ","));
        s = Util.safeArrayToString(generatorPreProcStageTwoA);
        getProject().log(this, Constants.GENERATOR_PREPROC_STAGE2 + " is " + ((s != null) ? s : "<unset>"), Project.MSG_VERBOSE);
        AntUtil.setNewProperty(propertyHelper, Constants.GENERATOR_PREPROC_STAGE2, Util.safeArrayJoinToString(generatorPreProcStageTwoA, ","));


        String qtjambiQtLibdir = AntUtil.getPropertyAsString(propertyHelper, Constants.LIBDIR);
        if(qtjambiQtLibdir != null) {
            sourceValue = null;
            s = null;
//            s = AntUtil.getPropertyAsString(propertyHelper, Constants.QTJAMBI_MACOSX_QTMENUNIB_DIR);
            if(OSInfo.isMacOS()) {
                s = doesQtLibExistDir(qtjambiQtLibdir, "Resources/qt_menu.nib");
                if(s == null)
                    s = doesQtLibExistDir(qtjambiQtLibdir, "qt_menu.nib");
                if(s == null)
                    s = doesQtLibExistDir(qtjambiQtLibdir, "../src/gui/mac/qt_menu.nib");
                // FIXME: auto-detect, directory from source, directory from QtSDK on MacOSX, directory from framework on MacOSX
                
                if(s != null)
                    sourceValue = " (auto-detected)";
            }
            if(s == null) {
                if(OSInfo.isMacOS() == false)
                    sourceValue = " (expected for non-MacOSX platform)";
                else
                    sourceValue = " (WARNING you should resolve this for targetting MacOSX)";
                s = "";
            }
            mySetProperty(-1, Constants.QTJAMBI_MACOSX_QTMENUNIB_DIR, sourceValue, s, false);
        }
        if(AntUtil.getPropertyAsString(propertyHelper, Constants.QTJAMBI_MACOSX_QTMENUNIB_DIR) == null)
            AntUtil.setProperty(propertyHelper, Constants.QTJAMBI_MACOSX_QTMENUNIB_DIR, "", false);


        // The 'qreal' Qt C++ type is 'float' on ARM platforms and 'double' on all others
        String qrealtype = "double";
        String qrealSource = " (the default)";
        if("arm".equals(javaOsArch)) {
            qrealtype = "float";
            qrealSource = " (${" + Constants.OSCPU + "} is \"arm\")";
        }
        mySetProperty(-1, Constants.QTJAMBI_QREALTYPE, qrealSource, qrealtype, false);


        detectQtDsoExistAndSetProperty(Constants.CORE, "QtCore", libInfix, String.valueOf(qtMajorVersion), null);
        
        detectQtDsoExistAndSetProperty(Constants.DBUS, "QtDBus", libInfix, String.valueOf(qtMajorVersion), "dbus");

        detectQtDsoExistAndSetProperty(Constants.CLUCENE, "QtCLucene", libInfix, String.valueOf(qtMajorVersion), null);

        String qtDetectDeclarative = detectQtDsoExistAndSetProperty(Constants.DECLARATIVE, "QtDeclarative", libInfix, String.valueOf(qtMajorVersion), "declarative");

        detectQtDsoExistAndSetProperty(Constants.DESIGNER, "QtDesigner", libInfix, String.valueOf(qtMajorVersion), "designer");

        detectQtDsoExistAndSetProperty(Constants.DESIGNERCOMPONENTS, "QtDesignerComponents", libInfix, String.valueOf(qtMajorVersion), null);

        detectQtDsoExistAndSetProperty(Constants.GUI, "QtGui", libInfix, String.valueOf(qtMajorVersion), "gui");
        
        detectQtDsoExistAndSetProperty(Constants.HELP, "QtHelp", libInfix, String.valueOf(qtMajorVersion), "help");

        detectQtDsoExistAndSetProperty(Constants.MULTIMEDIA, "QtMultimedia", libInfix, String.valueOf(qtMajorVersion), "multimedia");

        detectQtDsoExistAndSetProperty(Constants.NETWORK, "QtNetwork", libInfix, String.valueOf(qtMajorVersion), "network");

        detectQtDsoExistAndSetProperty(Constants.OPENGL, "QtOpenGL", libInfix, String.valueOf(qtMajorVersion), "opengl");
        
        String phononLibDir = AntUtil.getPropertyAsString(propertyHelper, Constants.QTJAMBI_PHONON_LIBDIR);  // maybe null or valid
        String qtDetectPhonon = detectQtDsoExistAndSetProperty(Constants.PHONON, "phonon", String.valueOf(qtMajorVersion), "phonon", phononLibDir);

        detectQtDsoExistAndSetProperty(Constants.SCRIPT, "QtScript", libInfix, String.valueOf(qtMajorVersion), "script");

        detectQtDsoExistAndSetProperty(Constants.SCRIPTTOOLS, "QtScriptTools", libInfix, String.valueOf(qtMajorVersion), "scripttools");

        detectQtDsoExistAndSetProperty(Constants.SQL, "QtSql", libInfix, String.valueOf(qtMajorVersion), "sql");

        detectQtDsoExistAndSetProperty(Constants.SVG, "QtSvg", libInfix, String.valueOf(qtMajorVersion), "svg");

        if(qtMajorVersion>=5){
        	detectQtDsoExistAndSetProperty(Constants.TEST, "QtTest", libInfix, String.valueOf(qtMajorVersion), "testlib");
        }else{
        	detectQtDsoExistAndSetProperty(Constants.TEST, "QtTest", libInfix, String.valueOf(qtMajorVersion), "qtestlib");
        }

        String qtDetectWebkit = detectQtDsoExistAndSetProperty(Constants.WEBKIT, "QtWebKit", libInfix, String.valueOf(qtMajorVersion), "webkit");
        // Not sure why this is a problem "ldd libQtWebKit.so.4.7.4" has no dependency on libphonon for me,
        //  if you have headers and DSOs for WebKit then QtJambi should build the support.
        if(qtDetectWebkit != null && qtDetectPhonon == null)
            getProject().log(this, "WARNING: " + Constants.WEBKIT + " is FOUND, but " + Constants.PHONON + " is NOT FOUND", Project.MSG_VERBOSE);
//        DECLARATIVE_WEBKIT

        if(qtDetectWebkit != null && qtDetectDeclarative != null){
        	AntUtil.setNewProperty(propertyHelper, Constants.DECLARATIVE_WEBKIT + ".true", "true"); // true or not-set
        }
        	
        detectQtDsoExistAndSetProperty(Constants.XML, "QtXml", libInfix, String.valueOf(qtMajorVersion), "xml");

        detectQtDsoExistAndSetProperty(Constants.XMLPATTERNS, "QtXmlPatterns", libInfix, String.valueOf(qtMajorVersion), "xmlpatterns");
        
        if(qtMajorVersion>=5){
        	
        	detectQtDsoExistAndSetProperty(Constants.EGLDEVICEINTEGRATION, "QtEglDeviceIntegration", libInfix, String.valueOf(qtMajorVersion), null);
        	
        	detectQtDsoExistAndSetProperty(Constants.WAYLANDCLIENT, "QtWaylandClient", libInfix, String.valueOf(qtMajorVersion), null);
        	
        	detectQtDsoExistAndSetProperty(Constants.XCBQPA, "QtXcbQpa", libInfix, String.valueOf(qtMajorVersion), null);
        	
        	detectQtDsoExistAndSetProperty(Constants.WIDGETS, "QtWidgets", libInfix, String.valueOf(qtMajorVersion), "widgets");
        	
        	detectQtDsoExistAndSetProperty(Constants.QT3DCORE, "Qt3DCore", libInfix, String.valueOf(qtMajorVersion), "3dcore");
        	
        	detectQtDsoExistAndSetProperty(Constants.QT3DQUICK, "Qt3DQuick", libInfix, String.valueOf(qtMajorVersion), "3dquick");
        	
        	detectQtDsoExistAndSetProperty(Constants.QT3DQUICKRENDERER, "Qt3DQuickRenderer", libInfix, String.valueOf(qtMajorVersion), "3dquickrenderer");
        	
        	detectQtDsoExistAndSetProperty(Constants.QT3DRENDERER, "Qt3DRenderer", libInfix, String.valueOf(qtMajorVersion), "3drenderer");
        	
        	detectQtDsoExistAndSetProperty(Constants.WINEXTRAS, "QtWinExtras", libInfix, String.valueOf(qtMajorVersion), "winextras");
        	
        	detectQtDsoExistAndSetProperty(Constants.MACEXTRAS, "QtMacExtras", libInfix, String.valueOf(qtMajorVersion), "macextras");
        	
        	detectQtDsoExistAndSetProperty(Constants.X11EXTRAS, "QtX11Extras", libInfix, String.valueOf(qtMajorVersion), "x11extras");
        	
            detectQtDsoExistAndSetProperty(Constants.CONCURRENT, "QtConcurrent", libInfix, String.valueOf(qtMajorVersion), "concurrent");
            
            detectQtDsoExistAndSetProperty(Constants.LIBEGL, "libEGL", libInfix, null, null);
            
            detectQtDsoExistAndSetProperty(Constants.LIBGLESV2, "libGLESv2", libInfix, null, null);
            
            detectQtDsoExistAndSetProperty(Constants.BLUETOOTH, "QtBluetooth", libInfix, String.valueOf(qtMajorVersion), "bluetooth");
            
            detectQtDsoExistAndSetProperty(Constants.LOCATION, "QtLocation", libInfix, String.valueOf(qtMajorVersion), "location");
            
            detectQtDsoExistAndSetProperty(Constants.WEBSOCKETS, "QtWebSockets", libInfix, String.valueOf(qtMajorVersion), "websocket");
            
            detectQtDsoExistAndSetProperty(Constants.WEBCHANNEL, "QtWebChannel", libInfix, String.valueOf(qtMajorVersion), "webchannel");
            
            detectQtDsoExistAndSetProperty(Constants.SERIALPORT, "QtSerialPort", libInfix, String.valueOf(qtMajorVersion), "serialport");
            
            detectQtDsoExistAndSetProperty(Constants.SENSORS, "QtSensors", libInfix, String.valueOf(qtMajorVersion), "sensors");
            
            detectQtDsoExistAndSetProperty(Constants.QUICKTEST, "QtQuickTest", libInfix, String.valueOf(qtMajorVersion), null);
            
            detectQtDsoExistAndSetProperty(Constants.QUICKWIDGETS, "QtQuickWidgets", libInfix, String.valueOf(qtMajorVersion), null);
            
            detectQtDsoExistAndSetProperty(Constants.QUICKPARTICLES, "QtQuickParticles", libInfix, String.valueOf(qtMajorVersion), null);
            
            detectQtDsoExistAndSetProperty(Constants.QUICK, "QtQuick", libInfix, String.valueOf(qtMajorVersion), "quick");
            
            detectQtDsoExistAndSetProperty(Constants.QML, "QtQml", libInfix, String.valueOf(qtMajorVersion), "qml");
            
            detectQtDsoExistAndSetProperty(Constants.PRINTSUPPORT, "QtPrintSupport", libInfix, String.valueOf(qtMajorVersion), "printsupport");
            
            detectQtDsoExistAndSetProperty(Constants.NFC, "QtNfc", libInfix, String.valueOf(qtMajorVersion), "nfc");
            
            detectQtDsoExistAndSetProperty(Constants.MULTIMEDIAWIDGETS, "QtMultimediaWidgets", libInfix, String.valueOf(qtMajorVersion), null);
            
            detectQtDsoExistAndSetProperty(Constants.MULTIMEDIAQUICK_P, "QtMultimediaQuick_p", libInfix, String.valueOf(qtMajorVersion), null);

            detectQtDsoExistAndSetProperty(Constants.OPENGLEXTENSIONS, "QtOpenGLExtensions", libInfix, String.valueOf(qtMajorVersion), null);
            
            detectQtDsoExistAndSetProperty(Constants.PLATFORMSUPPORT, "QtPlatformSupport", libInfix, String.valueOf(qtMajorVersion), null);

        }

        detectQtPluginExistAndSetProperty(Constants.PLUGINS_ACCESSIBLE_QTACCESSIBLEWIDGETS, "accessible", "qtaccessiblewidgets", null, null, null);

        detectQtPluginExistAndSetProperty(Constants.PLUGINS_BEARER_CONNMANBEARER,    "bearer", "qconnmanbearer", null, null, null);
        detectQtPluginExistAndSetProperty(Constants.PLUGINS_BEARER_GENERICBEARER,    "bearer", "qgenericbearer", null, null, null);
        detectQtPluginExistAndSetProperty(Constants.PLUGINS_BEARER_NATIVEWIFIBEARER, "bearer", "qnativewifibearer", null, null, null);
        detectQtPluginExistAndSetProperty(Constants.PLUGINS_BEARER_NMBEARER,         "bearer", "qnmbearer", null, null, null);

        detectQtPluginExistAndSetProperty(Constants.PLUGINS_CODECS_CNCODECS, "codecs", "qcncodecs", null, null, null);
        detectQtPluginExistAndSetProperty(Constants.PLUGINS_CODECS_JPCODECS, "codecs", "qjpcodecs", null, null, null);
        detectQtPluginExistAndSetProperty(Constants.PLUGINS_CODECS_KRCODECS, "codecs", "qkrcodecs", null, null, null);
        detectQtPluginExistAndSetProperty(Constants.PLUGINS_CODECS_TWCODECS, "codecs", "qtwcodecs", null, null, null);

        detectQtPluginExistAndSetProperty(Constants.PLUGINS_GRAPHICSSYSTEMS_GLGRAPHICSSYSTEM,    "graphicssystems", "qglgraphicssystem", null, null, null);
        detectQtPluginExistAndSetProperty(Constants.PLUGINS_GRAPHICSSYSTEMS_TRACEGRAPHICSSYSTEM, "graphicssystems", "qtracegraphicssystem", null, null, null);

        detectQtPluginExistAndSetProperty(Constants.PLUGINS_ICONENGINES_SVGICON, "iconengines", "qsvgicon", null, null, null);

        // These are only detecting if the plugins exist for these modules,
        // lack of a plugins does not necessarily mean Qt doesn't have support
        // since the implementation might be statically linked in.
        detectQtPluginExistAndSetProperty(Constants.PLUGINS_IMAGEFORMATS_GIF,  "imageformats",  "qgif", null, null, null);

        detectQtPluginExistAndSetProperty(Constants.PLUGINS_IMAGEFORMATS_ICO,  "imageformats",  "qico", null, null, null);

        detectQtPluginExistAndSetProperty(Constants.PLUGINS_IMAGEFORMATS_JPEG, "imageformats", "qjpeg", null, null, null);

        detectQtPluginExistAndSetProperty(Constants.PLUGINS_IMAGEFORMATS_MNG,  "imageformats",  "qmng", null, null, null);

        detectQtPluginExistAndSetProperty(Constants.PLUGINS_IMAGEFORMATS_PNG,  "imageformats",  "qpng", null, "probably a built-in", null);

        detectQtPluginExistAndSetProperty(Constants.PLUGINS_IMAGEFORMATS_SVG,  "imageformats",  "qsvg", null, null, null);

        detectQtPluginExistAndSetProperty(Constants.PLUGINS_IMAGEFORMATS_TIFF, "imageformats", "qtiff", null, null, null);

        detectQtPluginExistAndSetProperty(Constants.PLUGINS_INPUTMETHODS_IMSW_MULTI, "inputmethods",  "qimsw-multi", null, null, null);

        // We now just do plugin detection and emit what we see, regardless of platform
        String sourceValueIfNull;

        sourceValueIfNull = null;
        if(OSInfo.isWindows() == false)
            sourceValueIfNull = "expected for non-Windows platform";
        detectQtPluginExistAndSetProperty(Constants.PHONON_DS9, "phonon_backend", "phonon_ds9", null, sourceValueIfNull, "auto-detected");

        if(OSInfo.isLinux() == false)
            sourceValueIfNull = "expected for non-Linux platform";  // also sunos/freebsd/ete...
        decideQtPluginsPhononBackendPhononGstreamer(Constants.PHONON_GSTREAMER, "phonon_backend", "phonon_gstreamer", sourceValueIfNull);

        sourceValueIfNull = null;
        if(OSInfo.isMacOS() == false)
            sourceValueIfNull = "expected for non-MacOSX platform";
        detectQtPluginExistAndSetProperty(Constants.PHONON_QT7, "phonon_backend", "phonon_qt7", null, sourceValueIfNull, "auto-detected");

        detectQtPluginExistAndSetProperty(Constants.PLUGINS_QMLTOOLING_QMLDBG_TCP,     "qmltooling",   "qmldbg_tcp", null, null, null);
        detectQtPluginExistAndSetProperty(Constants.PLUGINS_SCENEFORMATS_QSCENEAI,     "sceneformats",   "qsceneai", null, null, null);
        detectQtPluginExistAndSetProperty(Constants.PLUGINS_SCENEFORMATS_QSCENEBEZIER, "sceneformats",   "qscenebezier", null, null, null);
        detectQtPluginExistAndSetProperty(Constants.PLUGINS_SCENEFORMATS_QSCENEOBJ,    "sceneformats",   "qsceneobj", null, null, null);

        detectQtPluginExistAndSetProperty(Constants.PLUGINS_SCRIPT_QTSCRIPTDBUS,           "script", "qtscriptdbus", null, null, null);

        // FIXME: Detect the case when this module was compiled into QtSql
        detectQtPluginExistAndSetProperty(Constants.PLUGINS_SQLDRIVERS_SQLITE,   "sqldrivers",   "qsqlite", null, null, null);
        detectQtPluginExistAndSetProperty(Constants.PLUGINS_SQLDRIVERS_SQLITE2,  "sqldrivers",  "qsqlite2", null, null, null);
        detectQtPluginExistAndSetProperty(Constants.PLUGINS_SQLDRIVERS_SQLMYSQL, "sqldrivers", "qsqlmysql", null, null, null);
        detectQtPluginExistAndSetProperty(Constants.PLUGINS_SQLDRIVERS_SQLODBC,  "sqldrivers",  "qsqlodbc", null, null, null);
        detectQtPluginExistAndSetProperty(Constants.PLUGINS_SQLDRIVERS_SQLPSQL,  "sqldrivers",  "qsqlpsql", null, null, null);
        detectQtPluginExistAndSetProperty(Constants.PLUGINS_SQLDRIVERS_SQLTDS,   "sqldrivers",   "qsqltds", null, null, null);
        
        if(qtMajorVersion>=5){
	        detectQtPluginExistAndSetProperty(Constants.PLUGINS_AUDIO_QTAUDIO_WINDOWS,              "audio",          "qtaudio_windows", null, null, null);
	        detectQtPluginExistAndSetProperty(Constants.PLUGINS_GENERIC_QTUIOTOUCHPLUGIN,           "generic",        "qtuiotouchplugin", null, null, null);
	        detectQtPluginExistAndSetProperty(Constants.PLUGINS_IMAGEFORMATS_QJP2,                  "imageformats",   "qjp2", null, null, null);
	        detectQtPluginExistAndSetProperty(Constants.PLUGINS_IMAGEFORMATS_QICNS,                 "imageformats",   "qicns", null, null, null);
	        detectQtPluginExistAndSetProperty(Constants.PLUGINS_IMAGEFORMATS_QDDS,                  "imageformats",   "qdds", null, null, null);
	        detectQtPluginExistAndSetProperty(Constants.PLUGINS_IMAGEFORMATS_QTGA,                  "imageformats",   "qtga", null, null, null);
	        detectQtPluginExistAndSetProperty(Constants.PLUGINS_IMAGEFORMATS_QWBMP,                 "imageformats",   "qwbmp", null, null, null);
	        detectQtPluginExistAndSetProperty(Constants.PLUGINS_IMAGEFORMATS_QWEBP,                 "imageformats",   "qwebp", null, null, null);
	        detectQtPluginExistAndSetProperty(Constants.PLUGINS_MEDIASERVICE_DSENGINE,              "mediaservice",   "dsengine", null, null, null);
	        detectQtPluginExistAndSetProperty(Constants.PLUGINS_MEDIASERVICE_QTMEDIA_AUDIOENGINE,   "mediaservice",   "qtmedia_audioengine", null, null, null);
	        detectQtPluginExistAndSetProperty(Constants.PLUGINS_MEDIASERVICE_WMFENGINE,             "mediaservice",   "wmfengine", null, null, null);
	        detectQtPluginExistAndSetProperty(Constants.PLUGINS_PLATFORMS_QMINIMAL,                 "platforms",      "qminimal", null, null, null);
	        detectQtPluginExistAndSetProperty(Constants.PLUGINS_PLATFORMS_QOFFSCREEN,               "platforms",      "qoffscreen", null, null, null);
	        detectQtPluginExistAndSetProperty(Constants.PLUGINS_PLATFORMS_QWINDOWS,                 "platforms",      "qwindows", null, null, null);
	        detectQtPluginExistAndSetProperty(Constants.PLUGINS_PLATFORMS_QCOCOA,                 	"platforms",      "qcocoa", null, null, null);
	        detectQtPluginExistAndSetProperty(Constants.PLUGINS_PLATFORMS_QEGLFS,                 	"platforms",      "qeglfs", null, null, null);
	        detectQtPluginExistAndSetProperty(Constants.PLUGINS_PLATFORMS_QLINUXFB,                 "platforms",      "qlinuxfb", null, null, null);
	        detectQtPluginExistAndSetProperty(Constants.PLUGINS_PLATFORMS_QMINIMALEGL,              "platforms",      "qminimalegl", null, null, null);
	        detectQtPluginExistAndSetProperty(Constants.PLUGINS_PLATFORMS_QWAYLAND_EGL,             "platforms",      "qwayland-egl", null, null, null);
	        detectQtPluginExistAndSetProperty(Constants.PLUGINS_PLATFORMS_QWAYLAND_GENERIC,         "platforms",      "qwayland-generic", null, null, null);
	        detectQtPluginExistAndSetProperty(Constants.PLUGINS_PLATFORMS_QXCB,         			"platforms",      "qxcb", null, null, null);
	        detectQtPluginExistAndSetProperty(Constants.PLUGINS_PLAYLISTFORMATS_QTMULTIMEDIA_M3U,   "playlistformats","qtmultimedia_m3u", null, null, null);
	        detectQtPluginExistAndSetProperty(Constants.PLUGINS_POSITION_QTPOSITION_POSITIONPOLL,   "position",       "qtposition_positionpoll", null, null, null);
	        detectQtPluginExistAndSetProperty(Constants.PLUGINS_PRINTSUPPORT_WINDOWSPRINTERSUPPORT, "printsupport",   "windowsprintersupport", null, null, null);
	        detectQtPluginExistAndSetProperty(Constants.PLUGINS_QML1TOOLING_QMLDBG_INSPECTOR,       "qml1tooling",    "qmldbg_inspector", null, null, null);
	        detectQtPluginExistAndSetProperty(Constants.PLUGINS_QML1TOOLING_QMLDBG_TCP_QTDECLARATIVE,"qml1tooling",   "qmldbg_tcp_qtdeclarative", null, null, null);
	        detectQtPluginExistAndSetProperty(Constants.PLUGINS_QMLTOOLING_QMLDBG_QTQUICK2,         "qmltooling",     "qmldbg_qtquick2", null, null, null);
	        detectQtPluginExistAndSetProperty(Constants.PLUGINS_SENSORGESTURES_QTSENSORGESTURES_PLUGIN,        "sensorgestures",   "qtsensorgestures_plugin", null, null, null);
	        detectQtPluginExistAndSetProperty(Constants.PLUGINS_SENSORGESTURES_QTSENSORGESTURES_SHAKEPLUGIN,   "sensorgestures",   "qtsensorgestures_shakeplugin", null, null, null);
	        detectQtPluginExistAndSetProperty(Constants.PLUGINS_SENSORS_QTSENSORS_GENERIC,          "sensors",        "qtsensors_generic", null, null, null);
	        detectQtPluginExistAndSetProperty(Constants.PLUGINS_VIDEO_VIDEONODE_EGLVIDEONODE,       "video/videonode","eglvideonode", null, null, null);
	        detectQtPluginExistAndSetProperty(Constants.PLUGINS_GEOSERVICES_QTGEOSERVICES_MAPBOX,   "geoservices",    "qtgeoservices_mapbox", null, null, null);
	        detectQtPluginExistAndSetProperty(Constants.PLUGINS_GEOSERVICES_QTGEOSERVICES_NOKIA,    "geoservices",    "qtgeoservices_nokia", null, null, null);
	        detectQtPluginExistAndSetProperty(Constants.PLUGINS_GEOSERVICES_QTGEOSERVICES_OSM,      "geoservices",    "qtgeoservices_osm", null, null, null);
	
	        detectQtQmlModuleExistAndSetProperty(Constants.QML_QT_LABS_FOLDERLISTMODEL_QMLFOLDERLISTMODELPLUGIN,      	"Qt/labs/folderlistmodel",    	"qmlfolderlistmodelplugin", null, null, null);
	        detectQtQmlModuleExistAndSetProperty(Constants.QML_QT_LABS_SETTINGS_QMLSETTINGSPLUGIN,      				"Qt/labs/settings",    			"qmlsettingsplugin", null, null, null);
	        detectQtQmlModuleExistAndSetProperty(Constants.QML_QT3D_QUICK3DCOREPLUGIN,      							"Qt3D",    						"quick3dcoreplugin", null, null, null);
	        detectQtQmlModuleExistAndSetProperty(Constants.QML_Qt3D_INPUT_QUICK3DINPUTPLUGIN,      						"Qt3D/Input",					"quick3dinputplugin", null, null, null);
	        detectQtQmlModuleExistAndSetProperty(Constants.QML_Qt3D_RENDERER_QUICK3DRENDERERPLUGIN, 					"Qt3D/Renderer",				"quick3drendererplugin", null, null, null);
	        detectQtQmlModuleExistAndSetProperty(Constants.QML_QtBLUETOOTH_DECLARATIVE_BLUETOOTH,      					"QtBluetooth",    				"declarative_bluetooth", null, null, null);
	        detectQtQmlModuleExistAndSetProperty(Constants.QML_QTCANVAS3D_QTCANVAS3D,							      	"QtCanvas3D",			    	"qtcanvas3d", null, null, null);
	        detectQtQmlModuleExistAndSetProperty(Constants.QML_QTGRAPHICALEFFECTS,      								"QtGraphicalEffects",    		null, null, null, null);
	        detectQtQmlModuleExistAndSetProperty(Constants.QML_QTLOCATION_DECLARATIVE_LOCATION,							"QtLocation",			    	"declarative_location", null, null, null);
	        detectQtQmlModuleExistAndSetProperty(Constants.QML_QTMULTIMEDIA_DECLARATIVE_MULTIMEDIA,						"QtMultimedia",			    	"declarative_multimedia", null, null, null);
	        detectQtQmlModuleExistAndSetProperty(Constants.QML_QTNFC_DECLARATIVE_NFC,									"QtNfc",			    		"declarative_nfc", null, null, null);
	        detectQtQmlModuleExistAndSetProperty(Constants.QML_QTPOSITIONING_DECLARATIVE_POSITIONING,					"QtPositioning",			    "declarative_positioning", null, null, null);
	        detectQtQmlModuleExistAndSetProperty(Constants.QML_QTQML_MODELS_2_MODELSPLUGIN,								"QtQml/Models.2",			    "modelsplugin", null, null, null);
	        detectQtQmlModuleExistAndSetProperty(Constants.QML_QTQML_STATEMACHINE_QTQMLSTATEMACHINE,					"QtQml/StateMachine",			"qtqmlstatemachine", null, null, null);
	        detectQtQmlModuleExistAndSetProperty(Constants.QML_QTQUICK_2_QTQUICK2PLUGIN,								"QtQuick.2",			    	"qtquick2plugin", null, null, null);
	        detectQtQmlModuleExistAndSetProperty(Constants.QML_QTQUICK_CONTROLS_QTQUICKCONTROLSPLUGIN,					"QtQuick/Controls",			    "qtquickcontrolsplugin", null, null, null);
	        detectQtQmlModuleExistAndSetProperty(Constants.QML_QTQUICK_CONTROLS_STYLES_FLAT_QTQUICKEXTRASFLATPLUGIN,	"QtQuick/Controls/Styles/Flat", "qtquickextrasflatplugin", null, null, null);
	        detectQtQmlModuleExistAndSetProperty(Constants.QML_QTQUICK_DIALOGS_DIALOGPLUGIN,							"QtQuick/Dialogs",			    "dialogplugin", null, null, null);
	        detectQtQmlModuleExistAndSetProperty(Constants.QML_QTQUICK_DIALOGS_IMAGES,									"QtQuick/Dialogs/images",	    null, null, null, null);
	        detectQtQmlModuleExistAndSetProperty(Constants.QML_QTQUICK_DIALOGS_QML,										"QtQuick/Dialogs/qml",		    null, null, null, null);
	        detectQtQmlModuleExistAndSetProperty(Constants.QML_QTQUICK_DIALOGS_PRIVATE_DIALOGSPRIVATEPLUGIN,			"QtQuick/Dialogs/Private",		"dialogsprivateplugin", null, null, null);
	        detectQtQmlModuleExistAndSetProperty(Constants.QML_QTQUICK_EXTRAS_QTQUICKEXTRASPLUGIN,						"QtQuick/Extras",			    "qtquickextrasplugin", null, null, null);
	        detectQtQmlModuleExistAndSetProperty(Constants.QML_QTQUICK_LAYOUTS_QQUICKLAYOUTSPLUGIN,						"QtQuick/Layouts",			    "qquicklayoutsplugin", null, null, null);
	        detectQtQmlModuleExistAndSetProperty(Constants.QML_QTQUICK_LOCALSTORAGE_QMLLOCALSTORAGEPLUGIN,				"QtQuick/LocalStorage",			"qmllocalstorageplugin", null, null, null);
	        detectQtQmlModuleExistAndSetProperty(Constants.QML_QTQUICK_PARTICLES_2_PARTICLESPLUGIN,						"QtQuick/Particles.2",			"particlesplugin", null, null, null);
	        detectQtQmlModuleExistAndSetProperty(Constants.QML_QTQUICK_PRIVATEWIDGETS_WIDGETSPLUGIN,					"QtQuick/PrivateWidgets",		"widgetsplugin", null, null, null);
	        detectQtQmlModuleExistAndSetProperty(Constants.QML_QTQUICK_SCENE3D_QTQUICKSCENE3DPLUGIN,					"QtQuick/Scene3D",				"qtquickscene3dplugin", null, null, null);
	        detectQtQmlModuleExistAndSetProperty(Constants.QML_QTQUICK_WINDOW_2_WINDOWPLUGIN,							"QtQuick/Window.2",				"windowplugin", null, null, null);
	        detectQtQmlModuleExistAndSetProperty(Constants.QML_QTQUICK_XMLLISTMODEL_QMLXMLLISTMODELPLUGIN,				"QtQuick/XmlListModel",			"qmlxmllistmodelplugin", null, null, null);
	        detectQtQmlModuleExistAndSetProperty(Constants.QML_QTSENSORS_DECLARATIVE_SENSORS,							"QtSensors",					"declarative_sensors", null, null, null);
	        detectQtQmlModuleExistAndSetProperty(Constants.QML_QTWEBCHANNEL_DECLARATIVE_WEBCHANNEL,						"QtWebChannel",					"declarative_webchannel", null, null, null);
	        detectQtQmlModuleExistAndSetProperty(Constants.QML_QTWEBSOCKETS_DECLARATIVE_QMLWEBSOCKETS,					"QtWebSockets",					"declarative_qmlwebsockets", null, null, null);
	        detectQtQmlModuleExistAndSetProperty(Constants.QML_QTWINEXTRAS_QML_WINEXTRAS,								"QtWinExtras",					"qml_winextras", null, null, null);
	        detectQtQmlModuleExistAndSetProperty(Constants.QML_QTAUDIOENGINE_DECLARATIVE_AUDIOENGINE,					"QtQtAudioEngine",				"declarative_audioengine", null, null, null);
	    }
        
        AntUtil.setNewProperty(propertyHelper, Constants.PACKAGING_DSO_LIBSTDC___6,     decideQtBinDso(Constants.PACKAGING_DSO_LIBSTDC___6,     "libstdc++-6", null, null, null));
        AntUtil.setNewProperty(propertyHelper, Constants.PACKAGING_DSO_LIBGCC_S_DW2_1,  decideQtBinDso(Constants.PACKAGING_DSO_LIBGCC_S_DW2_1,  "libgcc_s_dw2-1", null, null, null));
        AntUtil.setNewProperty(propertyHelper, Constants.PACKAGING_DSO_LIBGCC_S_SJLJ_1, decideQtBinDso(Constants.PACKAGING_DSO_LIBGCC_S_SJLJ_1, "libgcc_s_sjlj-1", null, null, null));
        AntUtil.setNewProperty(propertyHelper, Constants.PACKAGING_DSO_MINGWM10,        decideQtBinDso(Constants.PACKAGING_DSO_MINGWM10,        "mingwm10", null, null, null));


        String packagingDsoLibeay32 = decideQtBinDso(Constants.PACKAGING_DSO_LIBEAY32, "libeay32", null, null, false);
        mySetProperty(-1, Constants.PACKAGING_DSO_LIBEAY32, null, packagingDsoLibeay32, false);

        String packagingDsoLibssl32 = decideQtBinDso(Constants.PACKAGING_DSO_LIBSSL32, "libssl32", null, null, false);
        String packagingDsoSsleay32 = decideQtBinDso(Constants.PACKAGING_DSO_SSLEAY32, "ssleay32", null, null, false);
        // When building QtJambi against the official Nokia Qt SDK they appear to provide duplicate
        // DLLs for the two naming variants libssl32.dll ssleay32.dll so we need to resolve this and
        // omit one.
        String packagingDsoLibssl32Message = "";
        String packagingDsoSsleay32Message = "";
        // "true" or a path, also means true.  Only "false" means false.
        if(("false".equals(packagingDsoLibssl32) == false && packagingDsoLibssl32 != null) && ("false".equals(packagingDsoSsleay32) == false && packagingDsoSsleay32 != null)) {
            // FIXME: Compare the files are actually the same
            if(compiler == Compiler.GCC || compiler == Compiler.OldGCC || compiler == Compiler.MinGW || compiler == Compiler.MinGW_W64) {
                packagingDsoSsleay32Message = " (was " + packagingDsoSsleay32 + "; auto-inhibited)";
                packagingDsoSsleay32 = "false";    // favour libssl32.dll
            } else {
                packagingDsoLibssl32Message = " (was " + packagingDsoLibssl32 + "; auto-inhibited)";
                packagingDsoLibssl32 = "false";    // favour ssleay32.dll
            }
        }
        AntUtil.setNewProperty(propertyHelper, Constants.PACKAGING_DSO_LIBSSL32, packagingDsoLibssl32);
        if((packagingDsoLibssl32Message.length() > 0 || ("false".equals(packagingDsoLibssl32) == false) && packagingDsoLibssl32 != null))
            getProject().log(this, Constants.PACKAGING_DSO_LIBSSL32 + ": " + packagingDsoLibssl32 + packagingDsoLibssl32Message, Project.MSG_INFO);

        AntUtil.setNewProperty(propertyHelper, Constants.PACKAGING_DSO_SSLEAY32, packagingDsoSsleay32);
        if((packagingDsoSsleay32Message.length() > 0 || ("false".equals(packagingDsoSsleay32) == false) && packagingDsoSsleay32 != null))
            getProject().log(this, Constants.PACKAGING_DSO_SSLEAY32 + ": " + packagingDsoSsleay32 + packagingDsoSsleay32Message, Project.MSG_INFO);

        if(OSInfo.isWindows()) {
            String packagingDsoZlib1 = decideQtBinDso(Constants.PACKAGING_DSO_ZLIB1, "zlib1", null, null, false);
            mySetProperty(-1, Constants.PACKAGING_DSO_ZLIB1, null, packagingDsoZlib1, false);
        } else {
            // If the lib directory contains "libz.so.1" or "libssl.so" or "libcrypto.so.1.0.0"
            sourceValue = null;
            String packagingDsoLibssl = decideQtBinDso(Constants.PACKAGING_DSO_LIBSSL, "ssl", null, new String[] { null, "1", "0" }, false);
            if(packagingDsoLibssl != null && QTDIR != null && packagingDsoLibssl.startsWith(QTDIR) == false) {
                sourceValue = " (detected: " + packagingDsoLibssl + "; but inhibited as not inside QTDIR)";
                packagingDsoLibssl = null;
            }
            mySetProperty(-1, Constants.PACKAGING_DSO_LIBSSL, sourceValue, packagingDsoLibssl, false);

            // FIXME: Implement file globs and reverse sort
            sourceValue = null;
            String packagingDsoLibcrypto = decideQtBinDso(Constants.PACKAGING_DSO_LIBCRYPTO, "crypto", null, new String[] { "1.0.0h", "1.0.0g", "1.0.0", "0.0.0", null, "10" }, false);
            if(packagingDsoLibcrypto != null && QTDIR != null && packagingDsoLibcrypto.startsWith(QTDIR) == false) {
                sourceValue = " (detected: " + packagingDsoLibcrypto + "; but inhibited as not inside QTDIR)";
                packagingDsoLibcrypto = null;
            }
            mySetProperty(-1, Constants.PACKAGING_DSO_LIBCRYPTO, sourceValue, packagingDsoLibcrypto, false);

            sourceValue = null;
            String packagingDsoLibz = decideQtBinDso(Constants.PACKAGING_DSO_LIBZ, "z", null, new String[] { "1", null }, false);
            if(packagingDsoLibz != null && QTDIR != null && packagingDsoLibz.startsWith(QTDIR) == false) {
                sourceValue = " (detected: " + packagingDsoLibz + "; but inhibited as not inside QTDIR)";
                packagingDsoLibz = null;
            }
            mySetProperty(-1, Constants.PACKAGING_DSO_LIBZ, sourceValue, packagingDsoLibz, false);
        }

        // FIXME: On Macosx when we build and have qtjambi.dbus==true we should WARN when we can not locate libdbus-1.*.dylib
        // FIXME: On Macosx we should also search /usr/local/lib
        String packagingDsoLibdbus = decideQtBinDso(Constants.PACKAGING_DSO_LIBDBUS, "dbus-1", null, new String[] { "3", "2", null }, null);
        AntUtil.setNewProperty(propertyHelper, Constants.PACKAGING_DSO_LIBDBUS, packagingDsoLibdbus);

        // Other build information sanity testing and warning

        String generatorIncludepaths = AntUtil.getPropertyAsString(propertyHelper, Constants.GENERATOR_INCLUDEPATHS);
        if(generatorIncludepaths != null) {
            // Validate the settings
            sourceValue = null;
            mySetProperty(-1, Constants.GENERATOR_INCLUDEPATHS, sourceValue, generatorIncludepaths, false);

            prettifyPathSeparator(generatorIncludepaths, true);
        }
        // On Windows it is usual to configure the compile include directory via envvar
        String INCLUDE = System.getenv("INCLUDE");
        if(INCLUDE != null) {
            getProject().log(this, "INCLUDE is set: " + INCLUDE, Project.MSG_INFO);
            prettifyPathSeparator(INCLUDE, false);
        }

        if(OSInfo.isLinux()) {    // Check we have libQtCore.so.4 in one of the paths in LD_LIBRARY_PATH
            String LD_LIBRARY_PATH = System.getenv("LD_LIBRARY_PATH");
            getProject().log(this, "LD_LIBRARY_PATH is set: " + ((LD_LIBRARY_PATH == null) ? "<notset>" : LD_LIBRARY_PATH), Project.MSG_INFO);
            if(LD_LIBRARY_PATH != null) {
                String[] sA = LD_LIBRARY_PATH.split(File.pathSeparator);  // should regex escape it
                String filename = LibraryEntry.formatQtName("QtCore", libInfix, isConfigurationDebug(), String.valueOf(qtMajorVersion));
                int found = 0;
                for(String element : sA) {
                    File testDir = new File(element);
                    if(testDir.isDirectory() == false)
                        getProject().log(this, " WARNING: LD_LIBRARY_PATH directory does not exit: " + element, Project.MSG_INFO);
                    File testFile = new File(element, filename);
                    if(testFile.isFile()) {
                        getProject().log(this, "   FOUND: LD_LIBRARY_PATH directory contains QtCore: " + testFile.getAbsolutePath(), Project.MSG_INFO);
                        found++;
                    }
                }
                if(found == 0)  // Maybe we should check to see if (QTDIR != null) before warning
                    getProject().log(this, " WARNING: LD_LIBRARY_PATH environment variable is set, but does not contain a valid location for libQtCore.so.*; this is usually needed to allow 'generator' and 'juic' executables to run during the build", Project.MSG_INFO);

                // FIXME: Refactor this duplicate code later (we look for !debug here but don't WARNING is we dont find it)
                filename = LibraryEntry.formatQtName("QtCore", libInfix, !isConfigurationDebug(), String.valueOf(qtMajorVersion));
                found = 0;
                for(String element : sA) {
                    File testDir = new File(element);
                    // we already warned about non-existing directory here
                    File testFile = new File(element, filename);
                    if(testFile.isFile()) {
                        getProject().log(this, "  XFOUND: LD_LIBRARY_PATH directory contains QtCore: " + testFile.getAbsolutePath(), Project.MSG_INFO);
                        found++;
                    }
                }
            } else {   // Maybe we should check to see if (QTDIR != null) before warning
                getProject().log(this, " WARNING: LD_LIBRARY_PATH environment variable is not set; this is usually needed to allow 'generator' and 'juic' executables to run during the build", Project.MSG_INFO);
            }
        }
        if(OSInfo.isWindows()) {    // Check we have QtCore4.dll/Qt5Core.dll in one of the paths in PATH
            String PATH = System.getenv("PATH");
            getProject().log(this, "PATH is set: " + ((PATH == null) ? "<notset>" : PATH), Project.MSG_INFO);
            if(PATH != null) {
                String[] sA = PATH.split(File.pathSeparator);  // should regex escape it
                String filename = LibraryEntry.formatQtName("QtCore", libInfix, isConfigurationDebug(), String.valueOf(qtMajorVersion));
                int found = 0;
                for(String element : sA) {
                    File testDir = new File(element);
                    if(testDir.isDirectory() == false)
                        getProject().log(this, " WARNING: PATH directory does not exit: " + element, Project.MSG_INFO);
                    File testFile = new File(element, filename);
                    if(testFile.isFile()) {
                        getProject().log(this, "   FOUND: PATH directory contains QtCore: " + testFile.getAbsolutePath(), Project.MSG_INFO);
                        found++;
                    }
                }
                if(found == 0)
                    getProject().log(this, " WARNING: PATH environment variable is set, but does not contain a valid location for QtCore*.dll; this is usually needed to allow 'generator' and 'juic' executables to run during the build", Project.MSG_INFO);

                // FIXME: Refactor this duplicate code later (we look for !debug here but don't WARNING is we dont find it)
                filename = LibraryEntry.formatQtName("QtCore", libInfix, !isConfigurationDebug(), String.valueOf(qtMajorVersion));
                found = 0;
                for(String element : sA) {
                    File testDir = new File(element);
                    // we already warned about non-existing directory here
                    File testFile = new File(element, filename);
                    if(testFile.isFile()) {
                        getProject().log(this, "  XFOUND: PATH directory contains QtCore: " + testFile.getAbsolutePath(), Project.MSG_INFO);
                        found++;
                    }
                }
            } else {
                getProject().log(this, " WARNING: PATH environment variable is not set; this is usually needed to allow 'generator' and 'juic' executables to run during the build", Project.MSG_INFO);
            }
        }
        if(OSInfo.isMacOS()) {    // Check we have libQtCore.4.dylib in one of the paths in DYLD_LIBRARY_PATH
            String DYLD_LIBRARY_PATH = System.getenv("DYLD_LIBRARY_PATH");
            getProject().log(this, "DYLD_LIBRARY_PATH is set: " + ((DYLD_LIBRARY_PATH == null) ? "<notset>" : DYLD_LIBRARY_PATH), Project.MSG_INFO);
            if(DYLD_LIBRARY_PATH != null) {
                String[] sA = DYLD_LIBRARY_PATH.split(File.pathSeparator);  // should regex escape it
                String filename = LibraryEntry.formatQtName("QtCore", libInfix, isConfigurationDebug(), String.valueOf(qtMajorVersion));
                int found = 0;
                for(String element : sA) {
                    File testDir = new File(element);
                    if(testDir.isDirectory() == false)
                        getProject().log(this, " WARNING: DYLD_LIBRARY_PATH directory does not exit: " + element, Project.MSG_INFO);
                    File testFile = new File(element, filename);
                    if(testFile.isFile()) {
                        getProject().log(this, "   FOUND: DYLD_LIBRARY_PATH directory contains QtCore: " + testFile.getAbsolutePath(), Project.MSG_INFO);
                        found++;
                    }
                }
                if(found == 0)
                    getProject().log(this, " WARNING: DYLD_LIBRARY_PATH environment variable is set, but does not contain a valid location for libQtCore.*.dylib; this is usually needed to allow 'generator' and 'juic' executables to run during the build", Project.MSG_INFO);

                // FIXME: Refactor this duplicate code later (we look for !debug here but don't WARNING is we dont find it)
                filename = LibraryEntry.formatQtName("QtCore", libInfix, !isConfigurationDebug(), String.valueOf(qtMajorVersion));
                found = 0;
                for(String element : sA) {
                    File testDir = new File(element);
                    // we already warned about non-existing directory here
                    File testFile = new File(element, filename);
                    if(testFile.isFile()) {
                        getProject().log(this, "  XFOUND: DYLD_LIBRARY_PATH directory contains QtCore: " + testFile.getAbsolutePath(), Project.MSG_INFO);
                        found++;
                    }
                }
            } else {
                getProject().log(this, " WARNING: DYLD_LIBRARY_PATH environment variable is not set; this is usually needed to allow 'generator' and 'juic' executables to run during the build", Project.MSG_INFO);
            }
        }

        // Setup some properties to be silently set to an empty string for safe ANT substitution
        {
            final String[] emptyStringIfNotSetA = {
                Constants.QTJAMBI_CONFIG_JUMPTABLE,
                Constants.QTJAMBI_GENERATOR_JUMPTABLE
            };
            for(String emptyStringIfNotSet : emptyStringIfNotSetA) {
                String value = AntUtil.getPropertyAsString(propertyHelper, emptyStringIfNotSet);
                if(value == null)
                    value = "";   // set this empty-string default here
                mySetProperty(0, emptyStringIfNotSet, " (default)", value, false);  // silent
            }
        }

        // Sanity checking, the goal here is to abort the build process early
        //  on so the developer doesn't waste their time getting an error
        //  message later in the build process.
        if(generatorIncludepaths == null)      // ${generator.includepaths}
            throw new BuildException("ERROR: " + Constants.GENERATOR_INCLUDEPATHS + " property is not configured, please ensure you read and edit build.properties");
        String qtCore = AntUtil.getPropertyAsString(propertyHelper, Constants.CORE);
        String qtCoreDebug = AntUtil.getPropertyAsString(propertyHelper, Constants.CORE + ".debug");
        if(qtCore == null && qtCoreDebug == null)
            throw new BuildException("ERROR: " + Constants.CORE + " or " + Constants.CORE + ".debug property is not configured");
    }

    private String detectConfiguration(String wantedConfiguration) {
        String dsoName = "QtCore";
        String version = String.valueOf(qtMajorVersion);

        String dsoPath = doesQtLibExist(dsoName, libInfix, version, null, null, false);
        String prlPath = null;
        if(dsoPath != null)    // Ok not look for PRL
            prlPath = doesQtLibExist(dsoName, libInfix, version, null, null, true); // prl=true
        if(dsoPath == null) {
            dsoPath = doesQtBinExist(dsoName, libInfix, version, null, null, false);
            if(dsoPath != null)    // if not found  look in other directory
                prlPath = doesQtBinExist(dsoName, libInfix, version, null, null, true); // prl=true
            if(prlPath == null)  // on windows using legacy Qt SDK types the *.dll are in bin/ and the *.prl in lib/
                prlPath = doesQtLibExist(dsoName, libInfix, version, null, null, true); // prl=true
        }

        String thisConfiguration = null;
        String defaultConfiguration = null;
        Set<String> foundSet = new HashSet<String>();
        String sourceValue = null;
        // Check *.prl add (WARNING: no *.prl)
        if(prlPath != null) {  // open *.prl and examine fields
            BufferedReader reader = null;
            try {
                reader = new BufferedReader(new FileReader(prlPath));
                String s;
                while((s = reader.readLine()) != null) {
                    String name;
                    String value;
                    // find first "="
                    int indexEquals = s.indexOf("=");
                    if(indexEquals < 0)
                        continue;
                    name = s.substring(0, indexEquals);
                    value = s.substring(indexEquals+1);
                    name = Util.safeFirstWord(name);  // skip leading and tailing on name
                    name = name.toUpperCase();
                    if("QMAKE_PRL_CONFIG".equals(name) == false)
                        continue;
                    StringTokenizer tokenizer = new StringTokenizer(value, " \t\n");
                    while(tokenizer.hasMoreElements()) {
                        String t = tokenizer.nextToken();
                        if(t.length() == 0)
                            continue;
                        t = t.toLowerCase();
                        if(Constants.CONFIG_DEBUG.equals(t)) {
                            foundSet.add(Constants.CONFIG_DEBUG);
                        } else if(Constants.CONFIG_RELEASE.equals(t)) {
                            foundSet.add(Constants.CONFIG_RELEASE);
                        } else if(Constants.CONFIG_DEBUG_AND_RELEASE.equals(t)) {
                            foundSet.add(Constants.CONFIG_DEBUG_AND_RELEASE);
                        }
                    }
                }
                reader.close();
                reader = null;
            } catch(IOException eat) {
            } finally {
                try {
                    if(reader != null)
                        reader.close();
                } catch(IOException eat) {
                }
            }
            sourceValue = "available configurations:";
            if(foundSet.contains(Constants.CONFIG_DEBUG_AND_RELEASE)) {
                if(defaultConfiguration == null)
                    defaultConfiguration = Constants.CONFIG_DEBUG_AND_RELEASE;
                sourceValue += " " + Constants.CONFIG_DEBUG_AND_RELEASE;
            }
            if(foundSet.contains(Constants.CONFIG_RELEASE)) {
                if(defaultConfiguration == null)
                    defaultConfiguration = Constants.CONFIG_RELEASE;
                sourceValue += " " + Constants.CONFIG_RELEASE;
            }
            if(foundSet.contains(Constants.CONFIG_DEBUG)) {
                if(defaultConfiguration == null)
                    defaultConfiguration = Constants.CONFIG_DEBUG;
                sourceValue += " " + Constants.CONFIG_DEBUG;
            }
        } else {
            sourceValue = "WARNING: no *.prl found";
        }

        if(thisConfiguration == null) {
            if(wantedConfiguration != null) {
                // if we parsed *.prl we error on invalid configurations against it
                if(foundSet.isEmpty() == false && foundSet.contains(wantedConfiguration)) {
                    thisConfiguration = wantedConfiguration;
                } else if(Constants.CONFIG_DEBUG_AND_RELEASE.equals(wantedConfiguration)) {
                    thisConfiguration = wantedConfiguration;
                } else if(Constants.CONFIG_RELEASE.equals(wantedConfiguration)) {
                    thisConfiguration = wantedConfiguration;
                } else if(Constants.CONFIG_DEBUG.equals(wantedConfiguration)) {
                    thisConfiguration = wantedConfiguration;
                } else if(Constants.CONFIG_TEST.equals(wantedConfiguration)) {
                    thisConfiguration = wantedConfiguration;
                } else {
                    throw new BuildException(Constants.CONFIGURATION + " could not be set to " + wantedConfiguration + " try one of " +
                        Constants.CONFIG_RELEASE + ", " + Constants.CONFIG_DEBUG + ", " + Constants.CONFIG_DEBUG_AND_RELEASE + ", " + Constants.CONFIG_TEST);
                }
                sourceValue += "; default is " + defaultConfiguration;
            } else if(defaultConfiguration != null) {
                thisConfiguration = defaultConfiguration;  // this is the default
                sourceValue += "; detected default set";
            } else {
                thisConfiguration = Constants.CONFIG_RELEASE;  // this is the configuration of last resort
                sourceValue += "; default of last-resort set";
            }
        }
        setConfiguration(thisConfiguration);
        mySetProperty(-1, Constants.CONFIGURATION,  " (" + sourceValue + ")", thisConfiguration, false);
        return thisConfiguration;
    }

    
    // wantedSdk maybe "MacOSX10.5.sdk" or "/Developer/SDKs/MacOSX10.5.sdk"
    private String detectMacosxSdk(String wantedSdk) {
        if(OSInfo.isMacOS() == false || qtMajorVersion>=5)
            return null;

        //QMAKE_MAC_SDK=/Developer/SDKs/MacOSX10.5.sdk
        String sourceValueAvailable = null;
        Map<String,File> foundMap = new HashMap<String,File>();
        List<String> filenameList = new ArrayList<String>();
        File sdkDir = new File("/Developer/SDKs");
        if(sdkDir.isDirectory()) {
            File[] listFiles = sdkDir.listFiles();
            for(File f : listFiles) {
                if(f.isDirectory()) {
                    String name = f.getName();
                    foundMap.put(name, f);
                    filenameList.add(name);
                }
            }
            Collections.sort(filenameList, Collections.reverseOrder());

            // build available string
            StringBuilder sb = new StringBuilder("available: ");
            boolean first = true;
            for(String n : filenameList) {
                if(first)
                    first = false;
                else
                    sb.append(", ");
                sb.append(n);
            }
            sourceValueAvailable = sb.toString();
        }

        boolean foundValue = false;  // null is a valid value
        String sourceValue = "";
        String thisValue = null;
        File wantedSdkFile = null;
        if(wantedSdk != null) {
            if(wantedSdk.length() > 0) {
                wantedSdkFile = new File(wantedSdk);  // absolute ?
                if(wantedSdkFile.isDirectory() == false) {
                    wantedSdkFile = new File("/Developer/SDKs", wantedSdk);  // relative?
                    if(wantedSdkFile.isDirectory() == false)
                        wantedSdkFile = null;
                }
                if(wantedSdkFile != null) {
                    thisValue = wantedSdkFile.getAbsolutePath(); // invalidate it (maybe we should allow it and warn it does not exist)
                    foundValue = true;
                }
            } else {
                if(wantedSdk.length() == 0)
                    thisValue = null;   // treat empty-string as null
                foundValue = true;  // null is the value we want
            }
        }
        if(foundValue == false) {
            if(foundMap.isEmpty())
                throw new BuildException("Directory \"/Developer/SDKs\" does not exist, did you install XCode on your Macosx ?");
            sourceValue = "picked default";
            thisValue = foundMap.get(filenameList.get(0)).getAbsolutePath();
            foundValue = true;
        }

        if(foundValue) {
            if(sourceValueAvailable != null) {
                if(sourceValue.length() > 0)  // isEmpty() is Java1.6 :(
                    sourceValue += "; ";
                sourceValue += sourceValueAvailable;
            }
            mySetProperty(-1, Constants.QTJAMBI_MACOSX_MAC_SDK,  " (" + sourceValue + ")", thisValue, false);
        }

        return thisValue;
    }

    private void prettifyPathSeparator(String generatorIncludepaths, boolean allowRecurse) {
        StringTokenizer tokenizer = new StringTokenizer(generatorIncludepaths, File.pathSeparator);
        while (tokenizer.hasMoreTokens()) {
            String path = tokenizer.nextToken();
            path = propertyHelper.replaceProperties(null, path, null); // one round of substitution
            if(allowRecurse && path.indexOf(File.pathSeparatorChar) >= 0) {
                prettifyPathSeparator(path, false);   // this is due to ${env.INCLUDE}
                continue;
            }
            File dir = new File(path);
            String msg = dir.getAbsolutePath();
            if (dir.isDirectory()) {
                getProject().log(this, "   FOUND: " + msg, Project.MSG_INFO);
            } else {
                getProject().log(this, " MISSING: " + msg, Project.MSG_WARN);
            }
        }
    }

    private File resolveDirectoryViaQTDIR(String qtdir, String attrName) {
        File tryPath = null;

        if(attrName.equals(Constants.BINDIR)) {
            tryPath = new File(qtdir, "bin");       // $QTDIR/bin
        } else if(attrName.equals(Constants.LIBDIR) || attrName.equals(Constants.QTJAMBI_PHONON_LIBDIR)) {
            tryPath = new File(qtdir, "lib");       // $QTDIR/lib
        } else if(attrName.equals(Constants.INCLUDEDIR) || attrName.equals(Constants.QTJAMBI_PHONON_INCLUDEDIR)) {
            tryPath = new File(qtdir, "include");   // $QTDIR/include
        } else if(attrName.equals(Constants.PLUGINSDIR) || attrName.equals(Constants.QTJAMBI_PHONON_PLUGINSDIR)) {
            tryPath = new File(qtdir, "plugins");   // $QTDIR/plugins
        } else if(attrName.equals(Constants.QMLMODULESDIR)){
        	tryPath = new File(qtdir, "qml");   // $QTDIR/qml
        } else if(attrName.equals(Constants.IMPORTSDIR)) {
            tryPath = new File(qtdir, "imports");   // $QTDIR/imports
        } else if(attrName.equals(Constants.QMAKE)) {
            String qmakeExecName = QMakeTask.executableName();
            tryPath = new File(qtdir, "bin" + File.separator + qmakeExecName);   // $QTDIR/bin
        }

        if(tryPath.isDirectory())
        	return tryPath;
        return null;
    }

    private boolean parseQtVersion(String versionString) {
        if(versionString == null)
            return false;

        // Remove leading, remove trailing whitespace
        versionString = Util.stripLeadingAndTrailingWhitespace(versionString);

        {
            // Check for valid character set "[0-9\.]+"
            final int len = versionString.length();
            for(int i = 0; i < len; i++) {
                char c = versionString.charAt(i);
                if((c >= '0' && c <= '9') || c == '.')
                    continue;
                return false;
            }
        }

        // Check for non-empty
        final int len = versionString.length();
        if(len == 0)
            return false;

        // Check for [0-9\.] and no double dots, no leading/trailing dots.
        if(versionString.charAt(0) == '.' || versionString.charAt(len - 1) == '.')
            return false;

        if(versionString.indexOf("..") >= 0)
            return false;

        // Split
        String[] versionParts = versionString.split("\\.");

        String tmpQtVersion = null;
        Integer tmpQtMajorVersion = null;
        Integer tmpQtMinorVersion = null;
        Integer tmpQtPatchlevelVersion = null;

        try {
            tmpQtVersion = versionString;
            if(versionParts.length < 1)
                return false;
            tmpQtMajorVersion = Integer.valueOf(versionParts[0]);
            if(versionParts.length < 2)
                return false;
            tmpQtMinorVersion = Integer.valueOf(versionParts[1]);
            if(versionParts.length < 3)
                tmpQtPatchlevelVersion = 0;
            else
                tmpQtPatchlevelVersion = Integer.valueOf(versionParts[2]);
        } catch(NumberFormatException e) {
            return false;
        }

        // Ok we happy
        qtVersion = tmpQtVersion;
        qtMajorVersion = tmpQtMajorVersion.intValue();
        qtMinorVersion = tmpQtMinorVersion.intValue();
        qtPatchlevelVersion = tmpQtPatchlevelVersion.intValue();
        return true;
    }

    private boolean decideQtVersion() {
        boolean versionFound = false;

        String tmpQtVersion = null;

        if(!versionFound) {
            tmpQtVersion = AntUtil.getPropertyAsString(propertyHelper, Constants.QT_VERSION);
            if(parseQtVersion(tmpQtVersion)) {
                versionFound = true;
                qtVersionSource = " (${" + Constants.QT_VERSION + "})";
            }
        }

        // If failure, open version.properties.template to get version
        if(!versionFound) {
            tmpQtVersion = null;
            InputStream inStream = null;
            Properties props = null;
            try {
                inStream = new FileInputStream(pathVersionPropertiesTemplate);
                props = new Properties();
                props.load(inStream);
                tmpQtVersion = (String) props.get(Constants.VERSION);
            } catch(FileNotFoundException e) {
                // Acceptable failure
                getProject().log(this, e.getMessage(), Project.MSG_ERR);
            } catch(IOException e) {
                throw new BuildException(e);
            } finally {
                if(inStream != null) {
                    try {
                        inStream.close();
                    } catch(IOException eat) {
                    }
                    inStream = null;
                }
            }

            if(tmpQtVersion != null) {
                if(parseQtVersion(tmpQtVersion)) {
                    versionFound = true;
                    qtVersionSource = " (" + pathVersionPropertiesTemplate + ")";
                }
            }
        }

        if(!versionFound) {
            // Run "qmake -query"
            String qmakeExe = AntUtil.getPropertyAsString(propertyHelper, "qmake.binary");
            qmakeExe = QMakeTask.resolveExecutableAbsolutePath(getProject(), qmakeExe);

            final String K_QT_VERSION = "QT_VERSION";

            List<String> qmakeArgs = new ArrayList<String>();
            qmakeArgs.add(qmakeExe);
            qmakeArgs.add("-query");
            qmakeArgs.add(K_QT_VERSION);

            try {
                File fileDir = new File(".");
                String[] sA = Exec.executeCaptureOutput(qmakeArgs, fileDir, getProject(), null, null, false);
                Util.emitDebugLog(getProject(), sA);
                if(sA != null && sA.length == 2 && sA[0] != null)
                   tmpQtVersion = sA[0];      // stdout
                // Extract QT_VERSION:4.7.4
            } catch(InterruptedException e) {
                e.printStackTrace();
            } catch(IOException e) {
                e.printStackTrace();
            }
            if(tmpQtVersion != null) {
                if(tmpQtVersion.startsWith(K_QT_VERSION + ":"))
                    tmpQtVersion = tmpQtVersion.substring(K_QT_VERSION.length() + 1);
                tmpQtVersion = Util.stripLeadingAndTrailingWhitespace(tmpQtVersion);

                if(parseQtVersion(tmpQtVersion)) {
                    versionFound = true;
                    qtVersionSource = " (" + qmakeExe + " -query " + K_QT_VERSION + ")";
                }
            }
        }

        // This is last method as it is the target we are trying to set and also
        // debatable if it should be here at all.  Maybe the only use is with maybe
        // supporting really older Qt which does not allow: qmake -query
        if(!versionFound) {
            tmpQtVersion = AntUtil.getPropertyAsString(propertyHelper, Constants.VERSION);
            if(parseQtVersion(tmpQtVersion)) {
                versionFound = true;
                qtVersionSource = " (${" + Constants.VERSION + "})";
            }
        }

        return versionFound;
    }

    private boolean decideGeneratorPreProc(String osCpu) {
        List<String> generatorPreProcStageOneList = new ArrayList<String>();
        List<String> generatorPreProcStageTwoList = new ArrayList<String>();

        String compilerString = AntUtil.getPropertyAsString(propertyHelper, Constants.COMPILER);
        if(compilerString == null)
            return false;

        String gccVersionMajor = "";
        if(Compiler.isCompiler(compilerString, Compiler.GCC, Compiler.MinGW, Compiler.MinGW_W64))
            gccVersionMajor = "=4";
        else if(Compiler.isCompiler(compilerString, Compiler.OldGCC))
            gccVersionMajor = "=3";

        Boolean is64bit = OSInfo.is64bit();
        if(OSInfo.isWindows()) {
            if(Compiler.is64Only(compilerString))
                generatorPreProcStageOneList.add("-DWIN64");
            generatorPreProcStageOneList.add("-DWIN32");   // always set this

            if(Compiler.isCompiler(compilerString, Compiler.MSVC2005, Compiler.MSVC2005_64)) {
                generatorPreProcStageOneList.add("-D_MSC_VER=1400");
            } else if(Compiler.isCompiler(compilerString, Compiler.MSVC2008, Compiler.MSVC2008_64)) {
                generatorPreProcStageOneList.add("-D_MSC_VER=1500");
            } else if(Compiler.isCompiler(compilerString, Compiler.MSVC2010, Compiler.MSVC2010_64)) {
                generatorPreProcStageOneList.add("-D_MSC_VER=1600");
            } else if(Compiler.isCompiler(compilerString, Compiler.MSVC2012, Compiler.MSVC2012_64)) {
                generatorPreProcStageOneList.add("-D_MSC_VER=1700");
            } else if(Compiler.isCompiler(compilerString, Compiler.GCC, Compiler.OldGCC, Compiler.MinGW, Compiler.MinGW_W64)) {
                generatorPreProcStageOneList.add("-D__GNUC__" + gccVersionMajor);
            }
        } else if(OSInfo.isLinux()) {
            generatorPreProcStageOneList.add("-D__unix__");
            generatorPreProcStageOneList.add("-D__linux__");
            generatorPreProcStageOneList.add("-D__GNUC__" + gccVersionMajor);
            if("arm".equals(osCpu)) {
                generatorPreProcStageOneList.add("-D__arm__");
            } else {
                if(is64bit != null) {
                    if(is64bit.booleanValue())
                        generatorPreProcStageOneList.add("-D__x86_64__");
                    else
                        generatorPreProcStageOneList.add("-D__i386__");
                }
            }
        } else if(OSInfo.isMacOS()) {
            generatorPreProcStageOneList.add("-D__APPLE__");
            // FIXME: When we detect an alternative compiler is in use (LLVM)
            generatorPreProcStageOneList.add("-D__GNUC__" + gccVersionMajor);
            // if(OSInfo.isMacOSX64())
            //     generatorPreProcStageOneList.add("-D__LP64__");
        } else if(OSInfo.isFreeBSD()) {
            generatorPreProcStageOneList.add("-D__unix__");
            generatorPreProcStageOneList.add("-D__FreeBSD__");
            generatorPreProcStageOneList.add("-D__GNUC__" + gccVersionMajor);
            if("arm".equals(osCpu)) {
                generatorPreProcStageOneList.add("-D__arm__");
            } else {
                if(is64bit != null) {
                    if(is64bit.booleanValue())
                        generatorPreProcStageOneList.add("-D__x86_64__");  // untested
                    else
                        generatorPreProcStageOneList.add("-D__i386__");  // untested
                }
            }
        } else if(OSInfo.isSolaris()) {
            generatorPreProcStageOneList.add("-D__unix__");
            generatorPreProcStageOneList.add("-Dsun");
        }

        if(generatorPreProcStageOneList.size() > 0) {
            generatorPreProcStageOneA = generatorPreProcStageOneList.toArray(new String[generatorPreProcStageOneList.size()]);
        } else {
            generatorPreProcStageOneA = null;
        }

        if(generatorPreProcStageTwoList.size() > 0) {
            generatorPreProcStageTwoA = generatorPreProcStageTwoList.toArray(new String[generatorPreProcStageTwoList.size()]);
        } else {
            generatorPreProcStageTwoA = null;
        }

        return true;
    }

    private String doesQtLibExistDir(String librarydir, String name) {
        File dir = new File(librarydir, name);
        if(dir.exists() && dir.isDirectory())
            return dir.getAbsolutePath();
        return null;
    }

    private String doesQtDsoExist(String name, String infix, String version, String librarydir, Boolean debugValue, boolean prl, String propName) {
        StringBuilder path = new StringBuilder();

        if(librarydir != null) {
            path.append(librarydir);
            path.append(File.separator);
        } else if(propName != null) {
            String  propValue = AntUtil.getPropertyAsString(propertyHelper, propName);
            if(propValue != null)
                path.append(propValue);
            path.append(File.separator);
        }

        boolean thisDebug = isConfigurationDebug();
        if(debugValue != null)
            thisDebug = debugValue.booleanValue();
        String filename;
        if(prl)
            filename = LibraryEntry.formatQtPrlName(name, infix, thisDebug);
        else
            filename = LibraryEntry.formatQtName(name, infix, thisDebug, version);
        path.append(filename); // was LibraryEntry.formatQtJambiName
        File testForFile = new File(path.toString());
        getProject().log(this, "Checking " + path + " " + testForFile.exists(), Project.MSG_VERBOSE);
        if(testForFile.exists())
            return testForFile.getAbsolutePath();
        return null;
    }

    private String doesQtBinExist(String name, String infix, String version, String librarydir, Boolean debugValue, boolean prl) {
        return doesQtDsoExist(name, infix, version, librarydir, debugValue, prl, Constants.BINDIR);
    }
    private String doesQtLibExist(String name, String infix, String version, String librarydir, Boolean debugValue, boolean prl) {
        return doesQtDsoExist(name, infix, version, librarydir, debugValue, prl, Constants.LIBDIR);
    }

    private String doesQtLibOrBinExist(String name, String infix, String version, Boolean debugValue) {
        String path = doesQtLibExist(name, infix, version, null, debugValue, false);
        if(path == null)
            path = doesQtBinExist(name, infix, version, null, debugValue, false);
        return path;
    }

    private String detectQtDsoExistAndSetProperty(String propName, String name, String infix, String version, String addQtConfig, String altLibdir) {
        // We detect both non-debug and debug forms manually
        // On Linux the debug and the release are the same DSO name, so we have to look at configuration before detecting which one to detect first
        String dsoPath;
        String dsoDebugPath;
        if(Constants.CONFIG_DEBUG.equals(getConfiguration())) {
            dsoDebugPath = doesQtLibOrBinExist(name, infix, version, Boolean.TRUE);
            dsoPath = doesQtLibOrBinExist(name, infix, version, Boolean.FALSE);
            if(dsoDebugPath != null && dsoDebugPath.equals(dsoPath))
                dsoPath = null;  // same name on linux it can only be one kind
        } else {
            dsoPath = doesQtLibOrBinExist(name, infix, version, Boolean.FALSE);
            dsoDebugPath = doesQtLibOrBinExist(name, infix, version, Boolean.TRUE);
            if(dsoPath != null && dsoPath.equals(dsoDebugPath))
                dsoDebugPath = null;  // same name on linux it can only be one kind
        }

        String dsoDebugFilename = null;
        if(dsoDebugPath != null)  // ideally we want to resolve and display the relative path difference
            dsoDebugFilename = new File(dsoDebugPath).getName();

        String sourceValue = null;
        if(dsoDebugPath != null)
            sourceValue = " (found debug DSO: " + dsoDebugFilename + ")";
        mySetProperty(((dsoPath != null || dsoDebugPath == null) ? -1 : 0), propName, sourceValue, dsoPath, false);  // silent if null
        if(dsoPath != null) {  // we do this because the setting is used for both the Qt library itself and the JNI linkage DSO
            AntUtil.setNewProperty(propertyHelper, propName + ".true", "true"); // true or not-set
            AntUtil.setNewProperty(propertyHelper, propName + ".any.true", "true"); // true or not-set
        }

        sourceValue = null;
        if(Constants.CONFIG_DEBUG_AND_RELEASE.equals(getConfiguration()) && dsoPath == null && dsoDebugPath != null)
            sourceValue = " (no release DSO found)";
        mySetProperty(((dsoPath != null || dsoDebugPath == null) ? 0 : -1), propName + ".debug", sourceValue, dsoDebugPath, false);  // silent unless dsoPath==null
        if(dsoDebugPath != null) {  // we do this because the setting is used for both the Qt library itself and the JNI linkage DSO
            AntUtil.setNewProperty(propertyHelper, propName + ".debug.true", "true"); // true or not-set
            AntUtil.setNewProperty(propertyHelper, propName + ".any.true", "true"); // true or not-set
        }

        if(dsoPath != null || dsoDebugPath != null) {
            if(addQtConfig != null)
                addToQtConfig(addQtConfig);
        }

        if(dsoPath != null)
            return dsoPath;
        return dsoDebugPath;
    }

    private String detectQtDsoExistAndSetProperty(String propName, String name, String infix, String version, String addQtConfig) {
        return detectQtDsoExistAndSetProperty(propName, name, infix, version, addQtConfig, null);
    }

    private String detectQtPluginExistAndSetProperty(String propName, String pluginSubdir, String name, String addQtConfig, String sourceValueIfNull, String sourceValueIfNotNull) {
        // We detect both non-debug and debug forms manually
        // On Linux the debug and the release are the same DSO name, so we have to look at configuration before detecting which one to detect first
        String pluginPath;
        String pluginDebugPath;
        if(Constants.CONFIG_DEBUG.equals(getConfiguration())) {
            pluginDebugPath = doesQtPluginExist(pluginSubdir, name, false, Boolean.TRUE);
            pluginPath = doesQtPluginExist(pluginSubdir, name, false, Boolean.FALSE);
            if(pluginDebugPath != null && pluginDebugPath.equals(pluginPath))
                pluginPath = null;  // same name on linux it can only be one kind
        } else {
            pluginPath = doesQtPluginExist(pluginSubdir, name, false, Boolean.FALSE);
            pluginDebugPath = doesQtPluginExist(pluginSubdir, name, false, Boolean.TRUE);
            if(pluginPath != null && pluginPath.equals(pluginDebugPath))
                pluginDebugPath = null;  // same name on linux it can only be one kind
        }

        String pluginDebugFilename = null;
        if(pluginDebugPath != null)  // ideally we want to resolve and display the relative path difference
            pluginDebugFilename = new File(pluginDebugPath).getName();

        String sourceValue;
        String sourceValuePrefix;

        sourceValue = null;
        sourceValuePrefix = null;
        if(pluginDebugPath != null)
            sourceValuePrefix = "found debug DSO: " + pluginDebugFilename + "";
        if(pluginPath == null && sourceValueIfNull != null)
            sourceValue = " (" + ((sourceValuePrefix != null) ? sourceValuePrefix + "; " : "") + sourceValueIfNull + ")";
        else if(pluginPath != null && sourceValueIfNotNull != null)
            sourceValue = " (" + ((sourceValuePrefix != null) ? sourceValuePrefix + "; " : "") + sourceValueIfNotNull + ")";
        else if(sourceValuePrefix != null)
            sourceValue = " (" + sourceValuePrefix + ")";
        mySetProperty(((pluginPath != null || pluginDebugPath == null) ? -1 : 0), propName, sourceValue, pluginPath, false);  // silent if null
        if(pluginPath != null) {  // we do this because the setting is used for both the Qt library itself and the JNI linkage DSO
            AntUtil.setNewProperty(propertyHelper, propName + ".true", "true"); // true or not-set
            AntUtil.setNewProperty(propertyHelper, propName + ".any.true", "true"); // true or not-set
        }

        sourceValue = null;
        sourceValuePrefix = null;
        if(Constants.CONFIG_DEBUG_AND_RELEASE.equals(getConfiguration()) && pluginPath == null && pluginDebugPath != null)
            sourceValuePrefix = "no release DSO found";
        if(pluginPath == null && sourceValueIfNull != null)
            sourceValue = " (" + ((sourceValuePrefix != null) ? sourceValuePrefix + "; " : "") + sourceValueIfNull + ")";
        else if(pluginPath != null && sourceValueIfNotNull != null)
            sourceValue = " (" + ((sourceValuePrefix != null) ? sourceValuePrefix + "; " : "") + sourceValueIfNotNull + ")";
        else if(sourceValuePrefix != null)
            sourceValue = " (" + sourceValuePrefix + ")";
        mySetProperty(((pluginPath != null || pluginDebugPath == null) ? 0 : -1), propName + ".debug", sourceValue, pluginDebugPath, false);  // silent unless dsoPath==null
        if(pluginDebugPath != null) {  // we do this because the setting is used for both the Qt library itself and the JNI linkage DSO
            AntUtil.setNewProperty(propertyHelper, propName + ".debug.true", "true"); // true or not-set
            AntUtil.setNewProperty(propertyHelper, propName + ".any.true", "true"); // true or not-set
        }

        if(pluginPath != null || pluginDebugPath != null) {
            if(addQtConfig != null)
                addToQtConfig(addQtConfig);
        }

        if(pluginPath != null)
            return pluginPath;
        return pluginDebugPath;
    }
    
    private String detectQtQmlModuleExistAndSetProperty(String propName, String qmlSubdir, String name, String addQtConfig, String sourceValueIfNull, String sourceValueIfNotNull) {
        // We detect both non-debug and debug forms manually
        // On Linux the debug and the release are the same DSO name, so we have to look at configuration before detecting which one to detect first
        String qmlLibPath;
        String qmlLibDebugPath;
        if(Constants.CONFIG_DEBUG.equals(getConfiguration())) {
            qmlLibDebugPath = doesQtQmlModuleExist(qmlSubdir, name, Boolean.TRUE);
            qmlLibPath = doesQtQmlModuleExist(qmlSubdir, name, Boolean.FALSE);
            if(qmlLibDebugPath != null && qmlLibDebugPath.equals(qmlLibPath))
                qmlLibPath = null;  // same name on linux it can only be one kind
        } else {
            qmlLibPath = doesQtQmlModuleExist(qmlSubdir, name, Boolean.FALSE);
            qmlLibDebugPath = doesQtQmlModuleExist(qmlSubdir, name, Boolean.TRUE);
            if(qmlLibPath != null && qmlLibPath.equals(qmlLibDebugPath))
                qmlLibDebugPath = null;  // same name on linux it can only be one kind
        }

        String qmlLibDebugFilename = null;
        if(qmlLibDebugPath != null)  // ideally we want to resolve and display the relative path difference
            qmlLibDebugFilename = new File(qmlLibDebugPath).getName();

        String sourceValue;
        String sourceValuePrefix;

        sourceValue = null;
        sourceValuePrefix = null;
        if(qmlLibDebugPath != null)
            sourceValuePrefix = "found debug DSO: " + qmlLibDebugFilename + "";
        if(qmlLibPath == null && sourceValueIfNull != null)
            sourceValue = " (" + ((sourceValuePrefix != null) ? sourceValuePrefix + "; " : "") + sourceValueIfNull + ")";
        else if(qmlLibPath != null && sourceValueIfNotNull != null)
            sourceValue = " (" + ((sourceValuePrefix != null) ? sourceValuePrefix + "; " : "") + sourceValueIfNotNull + ")";
        else if(sourceValuePrefix != null)
            sourceValue = " (" + sourceValuePrefix + ")";
        mySetProperty(((qmlLibPath != null || qmlLibDebugPath == null) ? -1 : 0), propName, sourceValue, qmlLibPath, false);  // silent if null
        if(qmlLibPath != null) {  // we do this because the setting is used for both the Qt library itself and the JNI linkage DSO
            AntUtil.setNewProperty(propertyHelper, propName + ".true", "true"); // true or not-set
            AntUtil.setNewProperty(propertyHelper, propName + ".any.true", "true"); // true or not-set
        }

        sourceValue = null;
        sourceValuePrefix = null;
        if(Constants.CONFIG_DEBUG_AND_RELEASE.equals(getConfiguration()) && qmlLibPath == null && qmlLibDebugPath != null)
            sourceValuePrefix = "no release DSO found";
        if(qmlLibPath == null && sourceValueIfNull != null)
            sourceValue = " (" + ((sourceValuePrefix != null) ? sourceValuePrefix + "; " : "") + sourceValueIfNull + ")";
        else if(qmlLibPath != null && sourceValueIfNotNull != null)
            sourceValue = " (" + ((sourceValuePrefix != null) ? sourceValuePrefix + "; " : "") + sourceValueIfNotNull + ")";
        else if(sourceValuePrefix != null)
            sourceValue = " (" + sourceValuePrefix + ")";
        mySetProperty(((qmlLibPath != null || qmlLibDebugPath == null) ? 0 : -1), propName + ".debug", sourceValue, qmlLibDebugPath, false);  // silent unless dsoPath==null
        if(qmlLibDebugPath != null) {  // we do this because the setting is used for both the Qt library itself and the JNI linkage DSO
            AntUtil.setNewProperty(propertyHelper, propName + ".debug.true", "true"); // true or not-set
            AntUtil.setNewProperty(propertyHelper, propName + ".any.true", "true"); // true or not-set
        }

        if(qmlLibPath != null || qmlLibDebugPath != null) {
            if(addQtConfig != null)
                addToQtConfig(addQtConfig);
        }

        if(qmlLibPath != null)
            return qmlLibPath;
        return qmlLibDebugPath;
    }

    private String doesQtPluginExist(String subdir, String name, boolean noLibPrefix, Boolean debugValue) {
        String pluginsDirPropertyName;
        if(noLibPrefix)
            pluginsDirPropertyName = Constants.QTJAMBI_PHONON_PLUGINSDIR;
        else
            pluginsDirPropertyName = Constants.PLUGINSDIR;
        String pluginsPath = AntUtil.getPropertyAsString(propertyHelper, pluginsDirPropertyName);
        if(pluginsPath == null)
            return null;
        File filePluginsPath = new File(pluginsPath);
        if(filePluginsPath.isDirectory() == false)
            return null;
        // We construct a real path
        StringBuilder path = new StringBuilder();
        path.append(pluginsPath);
        path.append(File.separator);
        if(subdir !=  null) {
            path.append(subdir);
            path.append(File.separator);
        }

        boolean thisDebug = isConfigurationDebug();
        if(debugValue != null)
            thisDebug = debugValue.booleanValue();
        // Why do we sent qtMajorVersion
        String filename = LibraryEntry.formatPluginName(name, noLibPrefix, thisDebug, String.valueOf(qtMajorVersion));
        path.append(filename);

        File file = new File(path.toString());
        if(!file.exists())
            return null;
        return file.getAbsolutePath();
    }

    private String doesQtPluginExist(String subdir, String name) {
        return doesQtPluginExist(subdir, name, false, null);
    }
    
    private String doesQtQmlModuleExist(String subdir, String name, Boolean debugValue) {
        String qmlModulePath = AntUtil.getPropertyAsString(propertyHelper, Constants.QMLMODULESDIR);
        if(qmlModulePath == null)
            return null;
        File fileQmlPath = new File(qmlModulePath);
        if(!fileQmlPath.isDirectory()){
        	getProject().log("Not a directory: "+fileQmlPath);
        	return null;
        }
        // We construct a real path
        StringBuilder path = new StringBuilder();
        path.append(qmlModulePath);
        path.append(File.separator);
        if(subdir !=  null) {
            path.append(subdir);
            path.append(File.separator);
        }

        boolean thisDebug = isConfigurationDebug();
        if(debugValue != null)
            thisDebug = debugValue.booleanValue();
        if(name!=null){
	        String filename = LibraryEntry.formatQmlPluginName(name, thisDebug);
	        path.append(filename);
        }

        File file = new File(path.toString());
        if(!file.exists()){
        	getProject().log("Module "+name+" not available: "+file);
        	return null;
        }
        return file.getAbsolutePath();
    }
    
    private String doesQtQmlModuleExist(String subdir, String name) {
        return doesQtQmlModuleExist(subdir, name, null);
    }

    // FIXME: Should really have release/debug as well, but this is only relevant for Linux and that
    //  doesn't support debug_and_release like windows or macosx does.  So we save ourselves this headache
    //  of shoehorning that matter into this method.
    private String decideQtPluginsPhononBackendPhononGstreamer(String propName, String pluginSubdir, String name, String sourceValueIfNull) {
        String path;
        String sourceValue = null;
        Boolean autodetectKdePhonon = null;

        String kdephonon = AntUtil.getPropertyAsString(propertyHelper, Constants.QTJAMBI_PHONON_KDEPHONON);
        if(kdephonon != null && "true".equals(kdephonon)) {
            // build configuration states use KDE phonon
            path = doesQtPluginExist(pluginSubdir, name, true, null);  // auto-detect Kde phonon
            if(path == null) {  // not found
                path = doesQtPluginExist(pluginSubdir, name);  // try for Qt phonon anyway?
                if(path != null)  // found
                    sourceValue = " (WARNING auto-detected qt phonon; but " + Constants.QTJAMBI_PHONON_KDEPHONON + "=\"" + kdephonon +"\"; so turning this off)";
                autodetectKdePhonon = Boolean.FALSE;
            } else {
                sourceValue = " (auto-detected kind:kdephonon)";
            }
        } else {
            // build configuration states use Qt phonon
            path = doesQtPluginExist(pluginSubdir, name);   // auto-detect Qt phonon
            if(path != null) {
                if(kdephonon != null && "true".equals(kdephonon))  // but user has setup kde phonon as well?
                    sourceValue = " (auto-detected; WARNING " + Constants.QTJAMBI_PHONON_KDEPHONON + "=\"" + kdephonon + "\"; so \"lib\" prefix removed)";
            } else {
                path = doesQtPluginExist(pluginSubdir, name, true, null);  // try for KDE phonon anyway?
                if(path != null) {
                    if(kdephonon == null)
                        sourceValue = " (auto-detected as kind:kdephonon)";
                    else
                        sourceValue = " (auto-detected kde phonon; but " + Constants.QTJAMBI_PHONON_KDEPHONON + "=\"" + kdephonon + "\"; so \"lib\" prefix removed)";
                    autodetectKdePhonon = Boolean.TRUE;
                } else {
                    autodetectKdePhonon = Boolean.FALSE;  // FWIW
                }
            }
        }
        if(sourceValue == null) {
            if(path != null)
                sourceValue = " (auto-detected)";
            else
                sourceValue = " (" + sourceValueIfNull + ")";
        }
        mySetProperty(-1, propName, sourceValue, path, false);
        if(autodetectKdePhonon != null) {
            if(kdephonon == null) {  // we can only set this if it was not already set
                if(autodetectKdePhonon.booleanValue())
                    mySetProperty(((OSInfo.isLinux()) ? -1 : 0), Constants.QTJAMBI_PHONON_KDEPHONON, " (auto-detected)", "true", false);
                else  // FIXME: This else clause should probably be removed
                    mySetProperty(((OSInfo.isLinux()) ? -1 : 0), Constants.QTJAMBI_PHONON_KDEPHONON, null, "false", true);  // force off
            }
        }
        return path;
    }

    public static boolean findInString(String haystack, String needle, char delimChar) {
        final int nLen = needle.length();
        final int hLen = haystack.length();
        int o = 0;
        boolean found = false;
        while(o < hLen) {
            int stringOffset = haystack.indexOf(needle, o);
            if(stringOffset < 0)
                break;
            if(stringOffset == 0 || haystack.charAt(stringOffset - 1) == delimChar) {
                if(hLen <= stringOffset + nLen || haystack.charAt(stringOffset + nLen) == delimChar) {
                   // found
                   found = true;
                   break;
                }
            }
            o = stringOffset + nLen;
        }
        return found;
    }

    /**
     * Adds new library to qtjambi.qtconfig property, which is used
     * to specify additional qt libraries compiled.
     * @param config Library to add
     */
    private void addToQtConfig(String config) {
        String oldConfig = AntUtil.getPropertyAsString(propertyHelper, Constants.QTCONFIG);
        String newConfig = null;

        if(oldConfig != null) {
            final char delimChar = ' ';
            // Check it doesn't already exist (and don't add it again), this
            //  happens when InitializeTask runs twice due to multiple ant
            //  targets being set.
            boolean found = findInString(oldConfig, config, delimChar);
            if(!found) {
                newConfig = oldConfig + delimChar + config;
            }
        } else {
            newConfig = config;
        }

        if(newConfig != null)
            AntUtil.setProperty(propertyHelper, Constants.QTCONFIG, newConfig, false);
    }

    private String decideQtLibDso(String attrName, String name, String infix, String version, boolean verboseFlag) {
        String path = doesQtLibExist(name, infix, version, null, Boolean.FALSE, false);
        if(verboseFlag && path != null)
            getProject().log(this, attrName + ": " + path, Project.MSG_INFO);
        return path;
    }

    // unused
    private String decideQtLibDso(String attrName, String name, String infix, String version) {
        boolean verbose = getVerboseLevel() > 0;
        return decideQtLibDso(attrName, name, infix, version, verbose);
    }

    // unused
    private String decideQtLibDso(String attrName, String name, String infix, String[] tryVersionA, Boolean verboseBoolean) {
        boolean thisVerbose = getVerboseLevel() > 0;
        if(verboseBoolean != null)
            thisVerbose = verboseBoolean.booleanValue();

        if(tryVersionA == null)
            return decideQtLibDso(attrName, name, infix, null, thisVerbose);  // run at least once

        String rv = null;
        for(String tryVersion : tryVersionA) {
            if(tryVersion != null)
                rv = decideQtLibDso(attrName, name, infix, tryVersion, thisVerbose);
            else
                rv = decideQtLibDso(attrName, name, infix, null, thisVerbose);
            if(rv != null)
                return rv;
        }
        return rv;
    }

    private String decideQtBinDso(String attrName, String name, String infix, String version, boolean verboseFlag) {
        String path = doesQtBinExist(name, infix, version, null, Boolean.FALSE, false);
        if(verboseFlag && path != null)
            getProject().log(this, attrName + ": " + path, Project.MSG_INFO);
        return path;
    }

    private String decideQtBinDso(String attrName, String name, String infix, String[] tryVersionA, Boolean verboseBoolean) {
        boolean thisVerbose = getVerboseLevel() > 0;
        if(verboseBoolean != null)
            thisVerbose = verboseBoolean.booleanValue();

        if(tryVersionA == null)
            return decideQtBinDso(attrName, name, infix, null, thisVerbose);  // run at least once

        String rv = null;
        for(String tryVersion : tryVersionA) {
            if(tryVersion != null)
                rv = decideQtBinDso(attrName, name, infix, tryVersion, thisVerbose);
            else
                rv = decideQtBinDso(attrName, name, infix, null, thisVerbose);
            if(rv != null)
                return rv;
        }
        return rv;
    }

    private void analyzeLibinfix(File qtdir) {
		if(qtMajorVersion>4){
			File libraryPath;
			if(OSInfo.os()==OSInfo.OS.Windows){
				libraryPath = new File(qtdir, "bin");
			}else{
				libraryPath = new File(qtdir, "lib");
			}
			
			String libName = "Qt"+qtMajorVersion+"Gui";
			
			String[] libSuffixes;
			switch(OSInfo.os()){
			case Windows:
				libSuffixes = new String[]{"d.dll", ".dll"};
				break;
			case MacOS:
				libSuffixes = new String[]{"_debug."+qtVersion+".dylib", "."+qtVersion+".dylib"};
				libName = "lib"+libName;
				break;
			case FreeBSD:
			case Solaris:
			case Linux:
			case Unknown:
			default:
				libSuffixes = new String[]{".so."+qtVersion};
				libName = "lib"+libName;
				break;
			}
			
			for (String libSuffix : libSuffixes) {
				if(!new File(libraryPath, libName + libSuffix).exists()){
					for(File libfile : libraryPath.listFiles()){
						String fileName = libfile.getName();
						if(libfile.isFile() && fileName.startsWith(libName) && fileName.endsWith(libSuffix)){
							fileName = fileName.substring(0, fileName.length() - libSuffix.length());
							fileName = fileName.substring(libName.length());
							libInfix = fileName;
							break;
						}
					}
				}
			}
		}
		mySetProperty(-1, Constants.QTJAMBI_QT_LIBINFIX, " (taken from library name)", libInfix, false);
	}
}
