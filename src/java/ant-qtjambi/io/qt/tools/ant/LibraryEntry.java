/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2021 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

package io.qt.tools.ant;

import java.io.File;

import org.apache.tools.ant.BuildException;
import org.apache.tools.ant.PropertyHelper;
import org.apache.tools.ant.Task;

public class LibraryEntry extends Task {

    public static final String TYPE_DEFAULT            = "user";

    public static final String TYPE_DSO                = "dso";
    public static final String TYPE_PLUGIN             = "plugin";
	public static final String TYPE_DECLARATIVEPLUGIN  = "declarativeplugin";
	public static final String TYPE_QMLPLUGIN          = "qmlplugin";
	public static final String TYPE_FILE               = "file"; // neccessary to map files in the qml imports
	public static final String TYPE_FILESET            = "fileset";
    public static final String TYPE_SYSTEM             = "system";
    public static final String TYPE_QT                 = "qt";
    public static final String TYPE_EXE                = "exe";
	public static final String TYPE_QT_NONVERSIONED    = "qt.nonversioned";
    public static final String TYPE_QTJAMBI            = "qtjambi";
    public static final String TYPE_QTJAMBI_JNI        = "qtjambi-jni";
    public static final String TYPE_QTJAMBI_PLUGIN     = "qtjambi-plugin";
    public static final String TYPE_QTJAMBI_UTILITY    = "qtjambi-utility";
    public static final String TYPE_PLUGIN_JAR         = "plugin-jar";
    public static final String TYPE_UNVERSIONED_PLUGIN = "unversioned-plugin";

    /*
     *  set to specify where the plugins should be saved.
     *  Used to reduce redundancy of build.xml.
     *  TODO:
     *  Other variables could use same kind of solutions, I think.
     *  Whole path system needs to be rewritten to correspond
     *  new libdir, includedir, pluginsdir properties.
     *  Well I'm partially there for now with new destSubdir
     *  setting.
     */
    private String type = TYPE_DEFAULT;
    private String name;         // name from configuration
    private String shortName;
    private String originalname;

	private String targetName;
    private String absolutePath;
    private File rootPath;
    private String subdir;
    private String destSubdir;
    private String srcPath;
    private String conditionProperty;
    private String dsoVersion;
    private Boolean debug;  // inherit
    private FindCompiler.Compiler compiler;
    private boolean isPacked;
    private boolean isPending;

    public boolean isPending() {
		return isPending;
	}

	public void setPending(boolean isPending) {
		this.isPending = isPending;
	}

	public boolean isPacked() {
		return isPacked;
	}

	public void setPacked() {
		this.isPacked = !type.equals(TYPE_QTJAMBI_UTILITY);
	}

	public String getResolvedName() {
        String libraryName = name;
        if(targetName != null && !targetName.isEmpty())
            libraryName = targetName;
        else if(absolutePath != null && absolutePath.isEmpty())
            libraryName = new File(absolutePath).getName();  // override with actual filename used
        return libraryName;
    }

    Boolean getDebug() {
        return debug;
    }
    void setDebug(Boolean debug) {
        this.debug = debug;
    }
    
	FindCompiler.Compiler getCompiler() {
		return compiler;
	}

	void setCompiler(FindCompiler.Compiler compiler) {
		this.compiler = compiler;
	}

	public String getType() {
        return type;
    }
    public void setType(String type) {
        this.type = type;
    }

    public String getShortName() {
        return shortName;
    }
    public String getName() {
        return name;
    }
    public void setName(String name) {
        this.name = name;
        this.originalname = name;
        this.shortName = name;
    }
    
    public String getTargetName() {
        return targetName==null || targetName.isEmpty() ? name : targetName;
    }

    public String getAbsolutePath() {
        return absolutePath;
    }
    public void setAbsolutePath(String absolutePath) {
        this.absolutePath = absolutePath;
    }

    public File getRootPath() {
        return rootPath;
    }
    public void setRootPath(File rootPath) {
        this.rootPath = rootPath;
    }

    public String getSubdir() {
        return subdir;
    }
    public void setSubdir(String subdir) {
        this.subdir = subdir;
    }

    public String getDestSubdir() {
        return destSubdir;
    }
    public void setDestSubdir(String destSubdir) {
        this.destSubdir = destSubdir;
    }

    public void setSrcPath(String srcPath) {
        this.srcPath = srcPath;
    }
    public String getSrcPath() {
        return this.srcPath;
    }

    public void setIf(String conditionProperty) {
        this.absolutePath = null;
        this.conditionProperty = conditionProperty;
    }
    
    public boolean isIncluded() {
        String conditionProperty = this.conditionProperty;
        if(conditionProperty==null)
            return true;
        PropertyHelper propertyHelper = PropertyHelper.getPropertyHelper(getProject());
        if(debug == null) {
            if(Constants.CONFIG_DEBUG.equals(AntUtil.getPropertyAsString(propertyHelper, Constants.CONFIGURATION)))
                debug = Boolean.TRUE;
            else
                debug = Boolean.FALSE;
        }
        if(compiler == null) {
            compiler = FindCompiler.Compiler.resolve(AntUtil.getPropertyAsString(propertyHelper, Constants.COMPILER));
        }
        String condition;
        switch(compiler) {
        case MinGW:
        case MinGW_W64:
        	if(LibraryEntry.TYPE_QT.equals(getType())
        			|| LibraryEntry.TYPE_PLUGIN.equals(getType())
        			|| LibraryEntry.TYPE_QMLPLUGIN.equals(getType())) {
        		debug = Boolean.FALSE;
        	}
        	break;
        	default:
                
        }
    	if(Boolean.TRUE.equals(debug)
        		&& !LibraryEntry.TYPE_PLUGIN_JAR.equals(getType())
        		&& !LibraryEntry.TYPE_EXE.equals(getType())
        		&& !LibraryEntry.TYPE_FILE.equals(getType())
        		&& !LibraryEntry.TYPE_FILESET.equals(getType())){
            if(conditionProperty.endsWith(".true")){
                conditionProperty = conditionProperty.substring(0, conditionProperty.length()-5)+".debug.true";
            }else{
                conditionProperty = conditionProperty+".debug";
            }
        }
        condition = AntUtil.getPropertyAsString(propertyHelper, conditionProperty);
        if(condition==null)
            condition = AntUtil.getPropertyAsString(propertyHelper, conditionProperty+".true");
        if(condition==null){
            File file = new File(conditionProperty);
            if(file.isFile()) {
                if(LibraryEntry.TYPE_QT.equals(getType())
                    || LibraryEntry.TYPE_QTJAMBI.equals(getType())
                    || LibraryEntry.TYPE_PLUGIN_JAR.equals(getType())
                    || LibraryEntry.TYPE_QTJAMBI_JNI.equals(getType())
                    || LibraryEntry.TYPE_QTJAMBI_PLUGIN.equals(getType())
                    || LibraryEntry.TYPE_QTJAMBI_UTILITY.equals(getType())
                    || LibraryEntry.TYPE_UNVERSIONED_PLUGIN.equals(getType())
                    || LibraryEntry.TYPE_QT_NONVERSIONED.equals(getType())
                    || LibraryEntry.TYPE_DECLARATIVEPLUGIN.equals(getType())
                    || LibraryEntry.TYPE_PLUGIN.equals(getType())
                    || LibraryEntry.TYPE_QMLPLUGIN.equals(getType())){
                    this.absolutePath = file.getAbsolutePath();
                }
                return true;
            }
        }
        //getProject().log(this, conditionProperty+" = "+condition, Project.MSG_INFO);
        if(condition != null) {
            if("true".compareToIgnoreCase(condition) == 0){
                return true;
            }
            File file = new File(condition);
            if(file.isFile()) {
                if(LibraryEntry.TYPE_QT.equals(getType())
                    || LibraryEntry.TYPE_QTJAMBI.equals(getType())
                    || LibraryEntry.TYPE_PLUGIN_JAR.equals(getType())
                    || LibraryEntry.TYPE_QTJAMBI_JNI.equals(getType())
                    || LibraryEntry.TYPE_QTJAMBI_PLUGIN.equals(getType())
                    || LibraryEntry.TYPE_QTJAMBI_UTILITY.equals(getType())
                    || LibraryEntry.TYPE_UNVERSIONED_PLUGIN.equals(getType())
                    || LibraryEntry.TYPE_QT_NONVERSIONED.equals(getType())
                    || LibraryEntry.TYPE_DECLARATIVEPLUGIN.equals(getType())
                    || LibraryEntry.TYPE_PLUGIN.equals(getType())
                    || LibraryEntry.TYPE_QMLPLUGIN.equals(getType())){
                    this.absolutePath = file.getAbsolutePath();
                }
                return true;
            }
        }
        return false;
    }

    public void setDsoVersion(String dsoVersion) {
        // canonicalize empty string to null, due to usage pattern in Ant XML
        if(dsoVersion != null && dsoVersion.length() == 0)
            dsoVersion = null;
        this.dsoVersion = dsoVersion;
    }
    public String getDsoVersion() {
        return dsoVersion;
    }

    @Override
    public void execute() throws BuildException {
        if(name == null || name.length() == 0)
            throw new BuildException("Required attribute 'name' missing");

        PropertyHelper propertyHelper = PropertyHelper.getPropertyHelper(getProject());

        if(debug == null) {
            if(Constants.CONFIG_DEBUG.equals(AntUtil.getPropertyAsString(propertyHelper, Constants.CONFIGURATION)))
                debug = Boolean.TRUE;
            else
                debug = Boolean.FALSE;
        }

        // On windows the Qt plugins are versioned
        String qtVersion = AntUtil.getPropertyAsString(propertyHelper, Constants.QT_VERSION);
        int qtMajorVersion = Integer.valueOf(AntUtil.getPropertyAsString(propertyHelper, Constants.QT_VERSION_MAJOR));
        int qtMinorVersion = Integer.valueOf(AntUtil.getPropertyAsString(propertyHelper, Constants.QT_VERSION_MINOR));
        int qtPatchlevelVersion = Integer.valueOf(AntUtil.getPropertyAsString(propertyHelper, Constants.QT_VERSION_PATCHLEVEL));
        boolean useFrameworks = Boolean.valueOf(AntUtil.getPropertyAsString(propertyHelper, Constants.MAC_OS_USE_FRAMEWORK));
        boolean convertQtFrameworks = Boolean.valueOf(AntUtil.getPropertyAsString(propertyHelper, Constants.MAC_OS_CONVERT_QT_FRAMEWORK));
        String libInfix = AntUtil.getPropertyAsString(propertyHelper, Constants.QTJAMBI_QT_LIBINFIX);

        boolean resolved = false;

        if(!resolved && srcPath != null) {
            File srcPathFile = new File(srcPath);
            if(srcPathFile.exists()) {
                // FIXME this should override everything
                //name = srcPathFile.getAbsolutePath();
                name = srcPathFile.getName();
                shortName = srcPathFile.getName();
                resolved = true;
            }
        }

        if(!resolved) {
            // Fix name
        	switch(type) {
        	case TYPE_PLUGIN: {
                // MacOSX: uses *.dylib and _debug suffix
                String useDsoVersion = dsoVersion;
                if(dsoVersion != null && dsoVersion.compareToIgnoreCase("no-version") == 0) {
                    useDsoVersion = null;
                } else if(dsoVersion != null && dsoVersion.compareToIgnoreCase("use-qt-major-version") == 0) {
                    useDsoVersion = ""+qtMajorVersion;
                } else if(dsoVersion != null && dsoVersion.compareToIgnoreCase("use-qt-version") == 0) {
                    useDsoVersion = qtVersion;
                } else if(dsoVersion != null && dsoVersion.compareToIgnoreCase("use-soname-version") == 0) {
                    useDsoVersion = ""+qtMajorVersion;
                } else if(dsoVersion == null) {  // the default stratagy
                    if(OSInfo.crossOS() == OSInfo.OS.Windows)
                        useDsoVersion = ""+qtMajorVersion;
                    else
                        useDsoVersion = null;
                }
                if(useDsoVersion==null || useDsoVersion.isEmpty()){
                    name = formatPluginName(name, debug, qtMajorVersion, qtMinorVersion, qtPatchlevelVersion);
                }else{
                    name = formatPluginName(name, debug, -1, -1, -1);
                }
            } 
        	break;
        	case TYPE_PLUGIN_JAR: 
        		name += ".jar";
//        		this.setAbsolutePath(libInfix);
        		break;
        	case TYPE_QTJAMBI_UTILITY:
                // MacOSX: uses *.dylib and _debug suffix
                name = formatQtJambiQmlPluginName(name, debug, dsoVersion);
                break;
        	case TYPE_QTJAMBI_PLUGIN:
                // MacOSX: uses *.dylib and _debug suffix
                name = formatQtJambiPluginName(name, debug, dsoVersion);
                break;
        	case TYPE_DECLARATIVEPLUGIN: case TYPE_QMLPLUGIN:
        		name = formatQmlPluginName(name, debug, qtMajorVersion, qtMinorVersion, qtPatchlevelVersion);
        		break;
        	case TYPE_QT: {
	        		if(dsoVersion != null && dsoVersion.compareToIgnoreCase("no-version") == 0) {
	            		if(useFrameworks) {
	            			shortName = name = formatQtNameAsFramework(name, libInfix, debug, -1, -1, -1);
	            		}else {
	    	                String _name = name;
	    	                shortName = name = formatQtName(_name, libInfix, debug, -1, -1, -1);
	    	                if(convertQtFrameworks){
	    	                    targetName = name;
	    	                    shortName = name = formatQtNameAsFramework(_name, libInfix, debug, -1, -1, -1);
	    	                }
	            		}
	                }else {
		                // MacOSX: uses *.dylib and _debug suffix
		        		if(useFrameworks) {
		        			shortName = name = formatQtNameAsFramework(name, libInfix, debug, qtMajorVersion, qtMinorVersion, qtPatchlevelVersion);
		        		}else {
			                String _name = name;
		                	name = formatQtName(_name, libInfix, debug, qtMajorVersion, qtMinorVersion, qtPatchlevelVersion);
		                	shortName = formatQtName(_name, libInfix, debug, qtMajorVersion, -1, -1);
			                if(convertQtFrameworks){
			                    targetName = name;
			                    name = formatQtNameAsFramework(_name, libInfix, debug, qtMajorVersion, qtMinorVersion, qtPatchlevelVersion);
			                }
		        		}
	                }
	            } 
	        	break;
        	case TYPE_QT_NONVERSIONED:
    			name = formatQtName(name, libInfix, debug, -1, -1, -1);
    			break;
        	case TYPE_QTJAMBI_JNI: // JNI
                // MacOSX: uses *.jnilib and _debug suffix
        		String _name = name;
    			name = formatQtJambiJniName(_name, debug, qtMajorVersion, qtMinorVersion, qtPatchlevelVersion);
    			shortName = formatQtJambiJniName(_name, debug, qtMajorVersion, -1, -1);
                break;
        	case TYPE_QTJAMBI: // non-JNI base library
                // MacOSX: uses *.dylib and _debug suffix
    			name = formatQtJambiName(name, debug, dsoVersion);
                break;
        	case TYPE_DSO: 
    		case TYPE_SYSTEM:
                // name as-iz
                if(dsoVersion!=null && !dsoVersion.isEmpty()){
                    try{
                        name = formatQtName(name, libInfix, false, Integer.parseInt(dsoVersion), -1, -1);
                    }catch(Throwable t){
                        name = formatQtName(name, libInfix, false, -1, -1, -1);
                    }
                }else{
                    name = formatQtName(name, libInfix, false, -1, -1, -1);
                }
                break;
        	case TYPE_UNVERSIONED_PLUGIN:
                // MacOSX: uses *.dylib and _debug any suffix
                name = formatUnversionedPluginName(name, debug, qtMajorVersion, qtMinorVersion, qtPatchlevelVersion);
                break;
        	case TYPE_EXE:
            	if(OSInfo.crossOS() == OSInfo.OS.Windows) {
            		if(debug) {
            			name = name+"d.exe";
            		}else {
            			name = name+".exe";
            		}
            	}
	        	break;
        	case TYPE_FILE: 
    		case TYPE_FILESET:
            	// do nothing
    			break;
            default: 
            	break;
        	}
        }
    }

    public String absoluteSourcePath() {
    	if(isPending)
    		return null;
    	if(getSubdir()!=null && !getSubdir().isEmpty()){
    		return getRootPath() + "/" + getSubdir() + "/" + getName();
    	}else{
    		return getRootPath() + "/" + getName();
    	}
    }

    public String relativePath() {
    	if(getSubdir()!=null && !getSubdir().isEmpty()){
    		return getSubdir() + "/" + getName();
    	}else{
    		return getName();
    	}
    }

    public static String formatPluginName(String name, boolean debug, int qtMajorVersion, int qtMinorVersion, int qtPatchlevelVersion) {
        if(debug) {
            switch(OSInfo.crossOS()) {
            case Windows: return name + "d.dll";
            case MacOS:
                if(qtMajorVersion==5 && qtMinorVersion<14){
                    return "lib" + name + "_debug.dylib";
                }else{
                    return "lib" + name + ".dylib";
                }
            case Android:
            case Solaris:
            case Linux:
            case FreeBSD:
                return "lib" + name + ".so";
            case IOS:
                return "lib" + name + "_debug.a";
			default:
				break;
            }
        } else {
            switch(OSInfo.crossOS()) {
            case Windows:
                return name + ".dll";
            case MacOS:
                return "lib" + name + ".dylib";
            case Android:
            case Solaris:
            case Linux:
            case FreeBSD:
                return "lib" + name + ".so";
            case IOS:
                return "lib" + name + ".a";
			default:
				break;
            }
        }
        throw new BuildException("unhandled case...");
    }

	public static String formatQmlPluginName(String name, boolean debug, int qtMajorVersion, int qtMinorVersion, int qtPatchlevelVersion) {
        if (debug) {
            switch (OSInfo.crossOS()) {
            case Windows: return name + "d.dll";
            case MacOS: 
                if(qtMajorVersion==5 && qtMinorVersion<14)
                    return "lib" + name + "_debug.dylib";
                else return "lib" + name + ".dylib";
            case Android:
            case Solaris:
            case Linux: return "lib" + name + ".so";
            case IOS: return "lib" + name + "_debug.a";
			default:
				break;
            }
        } else {
            switch (OSInfo.crossOS()) {
            case Windows: return name + ".dll";
            case MacOS: return "lib" + name + ".dylib";
            case Android:
            case Solaris:
            case Linux: return "lib" + name + ".so";
            case IOS: return "lib" + name + ".a";
			default:
				break;
            }
        }
        throw new BuildException("unhandled case...");
    }
    
    public static String formatQtNameAsFramework(String name, String infix, boolean debug, int qtMajorVersion, int qtMinorVersion, int qtPatchlevelVersion) {
        String result = name + ".framework/Versions/" + qtMajorVersion + "/" + name;
        if(qtMajorVersion==6) {
        	result = name + ".framework/Versions/A/" + name;
        }
        if(qtMajorVersion==5 && qtMinorVersion<14){
            if(debug)
                result += "_debug";
        }
        return result;
    }

    public static String formatQtName(String name, String infix, boolean debug, int qtMajorVersion, int qtMinorVersion, int qtPatchlevelVersion) {
        String tmpVersionString;
        if(qtMajorVersion>=5){
    		if(name.startsWith("Qt") && !name.startsWith("Qt"+qtMajorVersion)){
    			name = "Qt"+qtMajorVersion+name.substring(2);
    		}
    		tmpVersionString = "";
    	}else if(qtMajorVersion>=0){
            tmpVersionString = ""+qtMajorVersion;
        }else{
            tmpVersionString = "";
        }
        if(infix==null || name.startsWith("lib")){
    		infix = "";
    	}
        String libPrefix = name.startsWith("lib") ? "" : "lib";
        if(debug) {
            String tmpDotVersionString;
            if(qtMajorVersion>=0){
                if(qtMinorVersion>=0){
                    if(qtPatchlevelVersion>=0){
                        tmpDotVersionString = String.format(".%1$s.%2$s.%3$s", qtMajorVersion, qtMinorVersion, qtPatchlevelVersion);
                    }else{
                        tmpDotVersionString = String.format(".%1$s.%2$s", qtMajorVersion, qtMinorVersion);
                    }
                }else{
                    tmpDotVersionString = String.format(".%1$s", qtMajorVersion);
                }
            }else{
                tmpDotVersionString = "";
            }
            switch(OSInfo.crossOS()) {
            case Windows:
                return name + infix + "d" + tmpVersionString + ".dll";
            case MacOS:
                if(qtMajorVersion==5 && qtMinorVersion<14){
                    return libPrefix + name + infix + "_debug" + tmpDotVersionString + ".dylib";
                }else{
                    return libPrefix + name + infix + tmpDotVersionString + ".dylib";
                }
            case Solaris:
            case Linux:
            case FreeBSD:
                return libPrefix + name + infix + ".so" + tmpDotVersionString;
            case Android:
                return libPrefix + name + infix + ".so";
            case IOS: return "lib" + name + infix + "_debug.a";
			default:
				break;
            }
        } else {
            String tmpDotVersionString;
            if(qtMajorVersion>=0){
                if(qtMinorVersion>=0){
                    if(qtPatchlevelVersion>=0){
                        tmpDotVersionString = String.format(".%1$s.%2$s.%3$s", qtMajorVersion, qtMinorVersion, qtPatchlevelVersion);
                    }else{
                        tmpDotVersionString = String.format(".%1$s.%2$s", qtMajorVersion, qtMinorVersion);
                    }
                }else{
                    tmpDotVersionString = String.format(".%1$s", qtMajorVersion);
                }
            }else{
                tmpDotVersionString = "";
            }
            switch(OSInfo.crossOS()) {
            case Windows:
                return name + infix + tmpVersionString + ".dll";
            case MacOS:
                return libPrefix + name + infix + tmpDotVersionString + ".dylib";
            case Solaris:
            case Linux:
            case FreeBSD:
                return libPrefix + name + infix + ".so" + tmpDotVersionString;
            case Android:
                return libPrefix + name + infix + ".so";
            case IOS: return "lib" + name + infix + ".a";
			default:
				break;
            }
        }
        throw new BuildException("unhandled case...");
    }
    
    public static String formatQtPrlNameAsFramework(String name, String infix, boolean debug) {
        String result = name + ".framework/" + name;
        if(debug)
            result += "_debug";
        result += ".prl";
        return result;
    }

    public static String formatQtPrlName(String name, String infix, boolean debug, int qtMajorVersion, int qtMinorVersion, int qtPatchlevelVersion) {
        // Windows: QtCore.prl QtCored.prl
        //   Linux: libQtCore.prl ??????
        //  MacOSX: libQtCore.prl libQtCore_debug.prl
    	if(infix==null){
    		infix = "";
    	}
        if(debug) {
            String tmpDebugSuffix = "_debug";
            switch(OSInfo.crossOS()) {
            case Windows:
                return name + infix + "d" + ".prl";
            case MacOS:
            case IOS:
                if(qtMajorVersion==5 && qtMinorVersion<14)
                    return "lib" + name + infix + ".prl";
                else return "lib" + name + infix + tmpDebugSuffix + ".prl";
            case Android:
            case Solaris:
            case Linux:
            case FreeBSD:
                return "lib" + name + infix + ".prl";
			default:
				break;
            }
        } else {
            switch(OSInfo.crossOS()) {
            case Windows:
                return name + ".prl";
            case MacOS:
            case IOS:
                return "lib" + name + infix + ".prl";
            case Android:
            case Solaris:
            case Linux:
            case FreeBSD:
                return "lib" + name + infix + ".prl";
			default:
				break;
            }
        }
        throw new BuildException("unhandled case...");
    }

    public static String formatUnversionedPluginName(String name, boolean debug, int qtMajorVersion, int qtMinorVersion, int qtPatchlevelVersion) {
        if(debug) {
            String tmpDebugSuffix = "_debug";
            switch(OSInfo.crossOS()) {
            case Windows:
                return name + "d.dll";
            case MacOS:
                if(qtMajorVersion==5 && qtMinorVersion<14)
                    return "lib" + name + tmpDebugSuffix + ".dylib";
                else return "lib" + name + ".dylib";
            case Android:
            case Solaris:
            case Linux:
            case FreeBSD:
                return "lib" + name + ".so";
            case IOS:
                return "lib" + name + ".a";
			default:
				break;
            }
        } else {
            switch(OSInfo.crossOS()) {
            case Windows:
                return name + ".dll";
            case MacOS:
                return "lib" + name + ".dylib";
            case Android:
            case Solaris:
            case Linux:
            case FreeBSD:
                return "lib" + name + ".so";
            case IOS:
                return "lib" + name + ".a";
			default:
				break;
            }
        }
        throw new BuildException("unhandled case...");
    }
    
    public static String formatQtJambiJniName(String name, boolean debug, int qtMajorVersion, int qtMinorVersion, int qtPatchlevelVersion) {
        String tmpVersionString = (qtMajorVersion>=0) ? ""+qtMajorVersion : "";
    	String tmpDotVersionString;
        if(qtMajorVersion>=0){
            if(qtMinorVersion>=0){
                if(qtPatchlevelVersion>=0){
                    tmpDotVersionString = String.format(".%1$s.%2$s.%3$s", qtMajorVersion, qtMinorVersion, qtPatchlevelVersion);
                }else{
                    tmpDotVersionString = String.format(".%1$s.%2$s", qtMajorVersion, qtMinorVersion);
                }
            }else{
                tmpDotVersionString = String.format(".%1$s", qtMajorVersion);
            }
        }else{
            tmpDotVersionString = "";
        }
        if(debug) {
            String tmpDebugSuffix = "_debug";
            switch(OSInfo.crossOS()) {
            case Windows:
        		return name + "d" + tmpVersionString + ".dll";
            case MacOS:
                return "lib" + name + tmpDebugSuffix + tmpDotVersionString + ".jnilib";
            case Android:
                return "lib" + name + tmpDebugSuffix + ".so";
            case Solaris:
            case Linux:
            case FreeBSD:
                return "lib" + name + tmpDebugSuffix + ".so" + tmpDotVersionString;
            case IOS: return "lib" + name + tmpDebugSuffix + ".a";
			default:
				break;
            }
        } else {
            switch(OSInfo.crossOS()) {
            case Windows:
        		return name + tmpVersionString + ".dll";
            case MacOS:
                return "lib" + name + tmpDotVersionString + ".jnilib";
            case Android:
                return "lib" + name + ".so";
            case Solaris:
            case Linux:
            case FreeBSD:
                return "lib" + name + ".so" + tmpDotVersionString;
            case IOS: return "lib" + name + ".a";
			default:
				break;
            }
        }
        throw new BuildException("unhandled case...");
    }

    public static String formatQtJambiName(String name, boolean debug, String versionString) {
        String tmpVersionString = (versionString != null) ? versionString : "";
        int iversion = 0;
    	if(!tmpVersionString.isEmpty()){
    		try {
				iversion = Integer.parseInt(tmpVersionString);
			} catch (Exception e) {
			}
    	}
        
        String tmpDotVersionString = (versionString != null && versionString.length() > 0) ? "." + versionString : "";
        if(debug) {
            String tmpDebugSuffix = "_debug";
            switch(OSInfo.crossOS()) {
            case Windows:
                if(iversion>=5){
            		return name + "d" + tmpVersionString + ".dll";
            	}else{
            		return name + "d.dll";
            	}
            case MacOS:
                return "lib" + name + tmpDebugSuffix + tmpDotVersionString + ".dylib";
            case Android:
                return "lib" + name + tmpDebugSuffix + ".so";
            case Solaris:
            case Linux:
            case FreeBSD:
                return "lib" + name + tmpDebugSuffix + ".so" + tmpDotVersionString;
            case IOS: return "lib" + name + tmpDebugSuffix + ".a";
			default:
				break;
            }
        } else {
            switch(OSInfo.crossOS()) {
            case Windows:
            	if(iversion>=5){
            		return name + tmpVersionString + ".dll";
            	}else{
            		return name + ".dll";
            	}
            case MacOS:
                return "lib" + name + tmpDotVersionString + ".dylib";
            case Solaris:
            case Android:
            case Linux:
            case FreeBSD:
                return "lib" + name + ".so" + tmpDotVersionString;
            case IOS: return "lib" + name + ".a";
			default:
				break;
            }
        }
        throw new BuildException("unhandled case...");
    }


    public static String formatQtJambiPluginName(String name, boolean debug, String versionString) {
    	if(debug) {
            switch(OSInfo.crossOS()) {
            case Windows: return name + "d.dll";
            case MacOS:
                return "lib" + name + "_debug.dylib";
            case Android:
            case Solaris:
            case Linux:
            case FreeBSD:
                return "lib" + name + "_debug.so";
            case IOS:
                return "lib" + name + "_debug.a";
			default:
				break;
            }
        } else {
            switch(OSInfo.crossOS()) {
            case Windows:
                return name + ".dll";
            case MacOS:
                return "lib" + name + ".dylib";
            case Android:
            case Solaris:
            case Linux:
            case FreeBSD:
                return "lib" + name + ".so";
            case IOS:
                return "lib" + name + ".a";
			default:
				break;
            }
        }
        throw new BuildException("unhandled case...");
    }

    public static String formatQtJambiQmlPluginName(String name, boolean debug, String versionString) {
//        String tmpDotVersionString = (versionString != null && versionString.length() > 0) ? "." + versionString : "";
         if(debug) {
            String tmpDebugSuffix = "_debug";
             switch(OSInfo.crossOS()) {
             case Windows:
        		 return name + "d.dll";
             case MacOS:
                return "lib" + name + tmpDebugSuffix + ".dylib";
             case Android:
             case Solaris:
             case Linux:
             case FreeBSD:
                return "lib" + name + tmpDebugSuffix + ".so";
             case IOS: return "lib" + name + tmpDebugSuffix + ".a";
 			default:
				break;
             }
         } else {
             switch(OSInfo.crossOS()) {
             case Windows:
        		 return name + ".dll";
             case MacOS:
                return "lib" + name + ".dylib";
             case Android:
             case Solaris:
             case Linux:
             case FreeBSD:
                return "lib" + name + ".so";
             case IOS: return "lib" + name + ".a";
 			default:
				break;
             }
         }
         throw new BuildException("unhandled case...");
    }
    
    String getOriginalname() {
		return originalname;
	}
}
