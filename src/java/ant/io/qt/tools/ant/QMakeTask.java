/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.StringTokenizer;
import java.util.TreeMap;

import org.apache.tools.ant.Project;
import org.apache.tools.ant.PropertyHelper;
import org.apache.tools.ant.Task;

import io.qt.tools.ant.OSInfo.OS;

public class QMakeTask extends Task {

    private String msg = "";
	private String config = "";
    private String dir = ".";
    private String pro = "";
    private boolean isTools;

    //parameters
    private String qtjambiModules;
    private String qtjambiConfig;
    private String includepath;
    private String qmakebinary;

    private boolean recursive;

    public static String executableName() {
        String exe;
        switch(OSInfo.os()) {
        case Windows:
            exe = "qmake.exe";
            break;
        default:
            exe = "qmake";
            break;
        }
        return exe;
    }

    public static String resolveExecutableAbsolutePath(Project project, String executableName, String propertyName) {
        if(executableName == null)
            executableName = executableName();

        // If qmakebinary is already absolute use it "/somedir/bin/qmake"
        if(executableName != null) {
            File file = new File(executableName);
            if(file.isAbsolute())
                return file.getAbsolutePath();

            if(propertyName != null) {
                // try in ${qtjambi.qt.bindir} if it is configured (caller supplies propertyName=Constants.BINDIR)
                // try in ${tools.qt.bindir} if it is configured (caller supplies propertyName=Constants.TOOLS_BINDIR)
                String binDir = project.getProperty(propertyName);
                if(binDir != null && !binDir.isEmpty()) {
                    File exeFile = new File(binDir, executableName);
                    if(exeFile.isFile()) {
                        // FIXME: Prepend 'binDir' to $PATH (so qmake can find moc)
                        return exeFile.getAbsolutePath();
                    }
                }
            }

            // try in $QTDIR/bin if it is configured
            try {
                String qtDir = AntUtil.getPropertyAsString(PropertyHelper.getPropertyHelper(project), "qtjambi.qtdir");
                if(qtDir != null && !qtDir.isEmpty()) {
                    File exeFile = new File(qtDir, "bin" + File.separator + executableName);
                    if(exeFile.isFile()) {
                        // FIXME: Prepend '$QTDIR/bin' to $PATH (so qmake can find moc)
                        return exeFile.getAbsolutePath();
                    }else {
                    	if(executableName.endsWith(".exe")) {
                    		exeFile = new File(qtDir, "bin" + File.separator + executableName.replace(".exe", ".bat"));
                    		if(exeFile.isFile()) {
                                // FIXME: Prepend '$QTDIR/bin' to $PATH (so qmake can find moc)
                                return exeFile.getAbsolutePath();
                            }
            			}
                    }
                }
            } catch(SecurityException eat) {
            }
        }

        // otherwise search $PATH
        return Util.LOCATE_EXEC(executableName, (String) null, null).getAbsolutePath();
    }

    public static String resolveExecutableAbsolutePath(Project project, String executableName) {
        return resolveExecutableAbsolutePath(project, executableName, Constants.BINDIR);
    }

    public String resolveExecutableAbsolutePath() {
        return resolveExecutableAbsolutePath(getProject(), qmakebinary);
    }

    private List<String> parseArguments() {
        List<String> arguments = new ArrayList<String>();

        StringTokenizer tokenizer = new StringTokenizer(config, " ");
        
        boolean forceDbg = Boolean.parseBoolean(getProject().getProperty(Constants.QTJAMBI_FORCE_DEBUG_INFO));

        while(tokenizer.hasMoreTokens()) {
            arguments.add("-config");
            String cf = tokenizer.nextToken();
            if("debug_and_release".equals(cf)) {
            	String compiler = (String)PropertyHelper.getProperty(getProject(), isTools ? Constants.TOOLS_COMPILER : Constants.COMPILER);
            	if(compiler!=null && !compiler.startsWith("msvc") && forceDbg) {
                	cf = "release";
            	}else{
                	arguments.add("build_all");
            		arguments.add("-config");
            	}
            }
            arguments.add(cf);
        }
        
        if(forceDbg) {
        	arguments.add("-config");
        	arguments.add("force_debug_info");
        }

        if(recursive)
            arguments.add("-r");
        return arguments;
    }

    private List<String> parseParameters() {
    	PropertyHelper propertyHelper = PropertyHelper.getPropertyHelper(getProject());
        List<String> parameters = new ArrayList<String>();

        if(qtjambiModules != null) {
            if(qtjambiModules.indexOf('{') >= 0) // windows name does not like (unescaped) "{" character this creates
                getProject().log(this, "QTJAMBI_MODULE not exported as value is: " + qtjambiModules, Project.MSG_INFO);
            else {
            	for(String cfg : qtjambiModules.split(","))
            		parameters.add("QTJAMBI_MODULE+=" + cfg);
            }
        }
        if("true".equals(AntUtil.getPropertyAsString(propertyHelper, Constants.QTJAMBI_DEBUG_TOOLS)))
        	parameters.add("DEFINES+=QTJAMBI_DEBUG_TOOLS");
        if(OSInfo.crossOS()==OSInfo.OS.MacOS) {
        	parameters.add("DEFINES+=_XOPEN_SOURCE");
        	if(Boolean.valueOf(AntUtil.getPropertyAsString(propertyHelper, Constants.MAC_OS_GENERATE_FRAMEWORKS))) {
            	parameters.add("QTJAMBI_GENERATE_FRAMEWORKS=true");
        	}else{
        		parameters.add("QTJAMBI_EXTENSION_SHLIB=jnilib");
        	}
        }
        
        parameters.add("QTJAMBI_PATCH_VERSION=" + getProject().getProperty("qtjambi.patchversion"));
        parameters.add("QTJAMBI_PLATFORM_BUILDDIR=" + getProject().getProperty("qtjambi.builddir"));
        parameters.add("QTJAMBI_GENERATOR_OUTPUT_DIR=" + getProject().getProperty("generator.outputdir"));

        String macSdk = getProject().getProperty(Constants.QTJAMBI_MACOSX_MAC_SDK);
        if(macSdk != null && macSdk.length() > 0)
            parameters.add("QMAKE_MAC_SDK=" + macSdk);

        if(qtjambiConfig != null && qtjambiConfig.length() > 0)
            parameters.add("QTJAMBI_CONFIG=" + qtjambiConfig);

        if(includepath != null && includepath.length() > 0)
            parameters.add("INCLUDEPATH+=" + includepath);
        
//        if(Boolean.parseBoolean(getProject().getProperty(Constants.QTJAMBI_FORCE_DEBUG_INFO)))
//        	parameters.add("CONFIG+=force_debug_info");
//        else
//        	parameters.add("CONFIG-=force_debug_info");
		
        /*
		String javaVersion = getProject().getProperty("target.java.version");
        if(javaVersion != null){
            if(javaVersion.startsWith("1.")){
            	parameters.add("DEFINES+=\"TARGET_JAVA_VERSION=" + javaVersion.substring(2) + "\"");
            }else{
                parameters.add("DEFINES+=\"TARGET_JAVA_VERSION=" + javaVersion + "\"");
            }
		}
        */
        if(OSInfo.os()==OSInfo.OS.MacOS) {
        	if(Boolean.valueOf(AntUtil.getPropertyAsString(propertyHelper, Constants.MAC_OS_CONVERT_QT_FRAMEWORK))) {
        		parameters.add("DEFINES+=\"QTJAMBI_NO_DEBUG_PLUGINS\"");
        	}
        }

        return parameters;
    }

    @Override
    public void execute() throws NullPointerException {
        getProject().log(this, msg, Project.MSG_INFO);

        PropertyHelper propertyHelper = PropertyHelper.getPropertyHelper(getProject());
        if(qtjambiConfig == null) {
            String thisQtjambiConfig = AntUtil.getPropertyAsString(propertyHelper, Constants.CONFIG);
            if(thisQtjambiConfig != null) {
                if(Constants.CONFIG_RELEASE.equals(thisQtjambiConfig))
                    qtjambiConfig = thisQtjambiConfig;
                else if(Constants.CONFIG_DEBUG.equals(thisQtjambiConfig))
                    qtjambiConfig = thisQtjambiConfig;
                else if(Constants.CONFIG_TEST.equals(thisQtjambiConfig))
                    qtjambiConfig = thisQtjambiConfig;
                else
                    getProject().log(this, "WARNING: QTJAMBI_CONFIG will not be exported as value " + thisQtjambiConfig + " is not recognised (from " + Constants.CONFIG + ")", Project.MSG_INFO);
                if(thisQtjambiConfig != null)
                    getProject().log(this, "QTJAMBI_CONFIG will be exported as " + qtjambiConfig + " (from " + Constants.CONFIG + ")", Project.MSG_INFO);
            }
        }

        String proFile = "";
        if(!pro.equals(""))
            proFile = Util.makeCanonical(pro).getAbsolutePath();

        final List<String> command =  new ArrayList<String>();

        if(proFile!=null && !proFile.isEmpty())
        	command.add(proFile);

        List<String> arguments = parseArguments();
        if(!arguments.isEmpty())
            command.addAll(arguments);

        List<String> parameters = parseParameters();
        if(!parameters.isEmpty())
            command.addAll(parameters);

        File dirExecute = null;
        if(dir != null)
            dirExecute = new File(dir);
        
        if(isTools) {
        	command.add(0, AntUtil.getPropertyAsString(propertyHelper, Constants.TOOLS_QMAKE_ABSPATH));
//        	command.add("-spec");
//        	command.add(AntUtil.getPropertyAsString(propertyHelper, Constants.TOOLS_QMAKESPEC));
        	String binpath = AntUtil.getPropertyAsString(propertyHelper, Constants.TOOLS_BINDIR);
        	String compilerbinpath = AntUtil.getPropertyAsString(propertyHelper, "tools.compiler.path");
        	String pathKey = "PATH";
        	String path = System.getenv(pathKey);
        	if(path==null || path.isEmpty()) {
        		pathKey = "Path";
        		path = System.getenv(pathKey);
        	}
        	if(path==null || path.isEmpty()) {
        		pathKey = "path";
        		path = System.getenv(pathKey);
        	}
        	if(path==null || path.isEmpty()) {
        		pathKey = "PATH";
        	}
        	if(path==null) {
        		path = "";
        	}
        	if(!path.isEmpty()) {
        		path += File.pathSeparator;
        	}
        	if(binpath!=null) {
        		path += binpath + File.pathSeparator;
        	}
        	if(compilerbinpath!=null) {
        		path += new File(compilerbinpath).getAbsolutePath() + File.pathSeparator;
        	}
        	Map<String,String> env = new HashMap<>();
        	env.put(pathKey, path);
        	Exec.execute(this, command, dirExecute, getProject(), null, null, env);
        }else {
        	command.add(0, AntUtil.getPropertyAsString(propertyHelper, Constants.QMAKE_ABSPATH));
//        	command.add("-spec");
//        	command.add(AntUtil.getPropertyAsString(propertyHelper, Constants.QT_SPEC));
        	String binpath = AntUtil.getPropertyAsString(propertyHelper, Constants.BINDIR);
        	Map<String,String> env = null;
        	if(OSInfo.crossOS()==OS.Android) {
        		String ndkRoot = AntUtil.getPropertyAsString(propertyHelper, "qtjambi.android.ndk");
        		if(ndkRoot!=null) {
	            	env = new HashMap<>();
	            	File nrkRootFile = new File(ndkRoot);
	            	env.put("ANDROID_NDK_ROOT", nrkRootFile.getAbsolutePath().replace('\\', '/'));
	            	command.add("\"ANDROID_ABIS="+AntUtil.getPropertyAsString(propertyHelper, Constants.QTJAMBI_ABIS)+"\"");
        		}
        	}
        	Exec.execute(this, command, dirExecute, getProject(), binpath, null, env);
        }
    }

    public void setMessage(String msg) {
        this.msg = msg;
    }

    public void setConfig(String config) {
        this.config = config;
    }

    public void setQtjambiModules(String qtconfig) {
        this.qtjambiModules = qtconfig;
    }

    public void setQtjambiConfig(String qtjambiConfig) {
        this.qtjambiConfig = qtjambiConfig;
    }

    public void setDir(String dir) {
        this.dir = dir;
    }

    public void setQmakebinary(String binary) {
        this.qmakebinary = binary;
    }

    public void setIncludepath(String path) {
        this.includepath = path;
    }

    public void setRecursive(boolean recursive) {
        this.recursive = recursive;
    }

    public void setPro(String pro) {
        this.pro = pro;
    }

    public void setTools(boolean tools) {
    	isTools = tools;
    }
	
    private final static Map<String,Map<String,String>> queries = Collections.synchronizedMap(new TreeMap<>());
    
	// Run "qmake -query"
	public static Map<String,String> query(Task task, String _qmakeExe) {
		return queries.computeIfAbsent(_qmakeExe, qmakeExe->{
			Map<String,String> result = new TreeMap<>();
			if(qmakeExe==null) {
				qmakeExe = AntUtil.getPropertyAsString(PropertyHelper.getPropertyHelper(task.getProject()), "qmake.binary");
				qmakeExe = QMakeTask.resolveExecutableAbsolutePath(task.getProject(), qmakeExe);
			}
			try {
				String[] sA = Exec.executeCaptureOutput(task, Arrays.asList(qmakeExe, "-query"), new File("."), task.getProject(), null, null, false);
				Util.emitDebugLog(task.getProject(), sA);
				if (sA != null && sA.length == 2 && sA[0] != null) {
					for(String line : sA[0].split(sA[0].contains("\r\n") ? "\\\r\\\n" : "\\\n")) {
						int idx = line.indexOf(':');
						if(idx>0) {
							result.put(line.substring(0, idx), line.substring(idx+1).trim());
						}
					}
				}
			} catch (InterruptedException e) {
				e.printStackTrace();
			} catch (IOException e) {
				e.printStackTrace();
			}
			return Collections.unmodifiableMap(result);
		});
	}
}
