/****************************************************************************
**
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
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
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

package io.qt.tools.ant;

import java.io.BufferedReader;
import java.io.File;
import java.io.StringReader;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Set;
import java.util.TreeSet;

import org.apache.tools.ant.BuildException;
import org.apache.tools.ant.Project;
import org.apache.tools.ant.PropertyHelper;
import org.apache.tools.ant.Task;

public class InstallNameToolTask extends Task {

    private String msg = "";
    private String dir = ".";
    private String libname = "";
    private String suffix = "dylib";

	private boolean debug = false;

    @Override
    public void execute() throws NullPointerException {
        getProject().log(this, msg, Project.MSG_INFO);
        
        int qtMajorVersion = Integer.parseInt(""+PropertyHelper.getProperty(getProject(), "qtjambi.soname.version.major"));
        int qtMinorVersion = Integer.parseInt(""+PropertyHelper.getProperty(getProject(), "qtjambi.soname.version.minor"));
        String libInfix = (String)PropertyHelper.getProperty(getProject(), "qtjambi.qt.libinfix");
        String qtlibdir = (String)PropertyHelper.getProperty(getProject(), "qtjambi.qt.libdir");
        
        String debugLib1 = debug ? "_debug" : "";
        String debugQtLib = debugLib1;
        if(qtMajorVersion==5 && qtMinorVersion>=14){
        	debugQtLib = "";
        }
        String[] libraries = {"Network", "Core", "Gui", "Widgets", "Quick", "Sql", "Xml", "Qml", "QmlModels"};
        File dirExecute = null;
        String libraryName = "lib"+libname+debugLib1+"."+suffix;
        if(dir != null){
            dirExecute = new File(dir);
            if(!new File(dirExecute, libraryName).exists()){
            	System.err.println("install_name_tool: Unable to find file "+new File(dirExecute, libraryName).getAbsolutePath());
                return;
            }
        }
        boolean no_otool = false;
        try {
        	String[] out = Exec.executeCaptureOutput(this, Arrays.asList("otool", "--version"), dirExecute, getProject(), null, null, false);
        	if(out==null || out.length<2 || out[0]==null || !out[0].contains("Apple")) {
        		if(out==null || out.length<=2 || out[1]==null || !out[1].contains("Apple")) {
        			no_otool = true;
        		}
        	}
        } catch ( Exception e ) {
        	no_otool = true;
        }
        OToolOut otoolOut = no_otool ? null : getOtoolOut(this, libraryName, dirExecute);
        if(otoolOut==null && !no_otool) {
        	getProject().log(this, "otool does not provide info for " + libraryName, Project.MSG_INFO);
        }
        final List<String> command =  new ArrayList<String>();
        command.add("install_name_tool");
        command.add("-change");
        command.add("");
        command.add("@rpath/");
        command.add(libraryName);
        for (String library : new String[]{
            "libQtJambi"+debugLib1+"."+qtMajorVersion+".jnilib"
        }) {
            command.set(2, library);
            command.set(3, "@rpath/"+library);
            Exec.execute(this, command, dirExecute, getProject());
        }
        
        PropertyHelper propertyHelper = PropertyHelper.getPropertyHelper(getProject());
        boolean useFrameworks = Boolean.valueOf(AntUtil.getPropertyAsString(propertyHelper, Constants.MAC_OS_USE_FRAMEWORK));
        boolean convertQtFrameworks = Boolean.valueOf(AntUtil.getPropertyAsString(propertyHelper, Constants.MAC_OS_CONVERT_QT_FRAMEWORK));
        if(convertQtFrameworks){
        	if(qtMajorVersion<6) {
	            for (String library : libraries) {
	                command.set(2, "@rpath/Qt"+library+".framework/Versions/"+qtMajorVersion+"/Qt"+library);
	                command.set(3, "@rpath/libQt"+qtMajorVersion+library+libInfix+debugQtLib+"."+qtMajorVersion+".dylib");
	                Exec.execute(this, command, dirExecute, getProject());
	            }
        	}else {
        		for (String library : libraries) {
	                command.set(2, "@rpath/Qt"+library+".framework/Versions/A/Qt"+library);
	                command.set(3, "@rpath/libQt"+qtMajorVersion+library+libInfix+debugQtLib+"."+qtMajorVersion+".dylib");
	                Exec.execute(this, command, dirExecute, getProject());
	            }
        	}
        }else {
        	if(!useFrameworks) {
	        	for (String library : libraries) {
	                command.set(2, "libQt"+qtMajorVersion+library+libInfix+debugLib1+"."+qtMajorVersion+".dylib");
	                command.set(3, "@rpath/libQt"+qtMajorVersion+library+libInfix+debugQtLib+"."+qtMajorVersion+".dylib");
	                Exec.execute(this, command, dirExecute, getProject());
	            }
        	}
            if(qtlibdir!=null){
                for (String library : new String[]{
                    "libQtJambi"+debugLib1+"."+qtMajorVersion+".jnilib"
                }) {
                    command.set(2, qtlibdir+"/"+library);
                    command.set(3, "@rpath/"+library);
                    Exec.execute(this, command, dirExecute, getProject());
                }
                for (String library : libraries) {
                    command.set(2, qtlibdir+"/libQt"+qtMajorVersion+library+libInfix+debugLib1+"."+qtMajorVersion+".dylib");
                    command.set(3, "@rpath/libQt"+qtMajorVersion+library+libInfix+debugLib1+"."+qtMajorVersion+".dylib");
                    Exec.execute(this, command, dirExecute, getProject());
                }
            }
        }
        Set<String> deleteCommands = new TreeSet<>();
        String path = "@executable_path/../Frameworks";
        if(otoolOut==null || otoolOut.rpaths.contains(path)) {
        	deleteCommands.add(path);
        }
        path = "@loader_path/Frameworks";
        if(otoolOut==null || otoolOut.rpaths.contains(path)) {
        	deleteCommands.add(path);
        }
        path = AntUtil.getPropertyAsString(propertyHelper, Constants.LIBDIR);
		if(otoolOut==null || otoolOut.rpaths.contains(path)) {
			deleteCommands.add(path);
		}
		List<String> deletes = new ArrayList<>(deleteCommands);
		while(!deletes.isEmpty()) {
    		List<String> commands = new ArrayList<>();
			commands.add("install_name_tool");
    		for (int i = 0; i < 10 && i < deletes.size(); i++) 
    		{
                commands.add("-delete_rpath");
				commands.add(deletes.remove(0));
			}
			commands.add(libraryName);
			try{
				Exec.exec(this, commands.toArray(new String[commands.size()]), dirExecute, getProject(), false);
			} catch ( BuildException e )
	        {
	        	getProject().log(this, " - " + (e.getMessage()!=null && !e.getMessage().isEmpty() ? e.getMessage() : e), e, Project.MSG_INFO);
	        }
    	}
    }

    public void setMessage(String msg) {
        this.msg = msg;
    }

    public void setDir(String dir) {
        this.dir = dir;
    }

	public String getLibname() {
		return libname;
	}

	public void setLibname(String libname) {
		this.libname = libname;
	}

	public boolean isDebug() {
		return debug;
	}

	public void setDebug(boolean debug) {
		this.debug = debug;
	}
    public String getSuffix() {
		return suffix;
	}

	public void setSuffix(String suffix) {
		this.suffix = suffix;
	}
    
    public static class OToolOut{
		String id = null;
		Set<String> loadDylibs = new TreeSet<>();
		Set<String> rpaths = new TreeSet<>();    	
    }
    
    public static OToolOut getOtoolOut(Task task, String libpath, File outdir) {
    	task.getProject().log(task, "- analyzing: " + libpath, Project.MSG_INFO);
    	OToolOut result = null;
    	try {
    		String[] out = Exec.executeCaptureOutput(task, Arrays.asList("otool", "-l", libpath), outdir, task.getProject(), null, null, false);
    		if(out.length==2 && out[0]!=null) {
        		result = new OToolOut();
    			try(BufferedReader reader = new BufferedReader(new StringReader(out[0]))){
    				while(true) {
    					String line = reader.readLine();
    					if(line==null) {
    						break;
    					}else {
    						line = line.trim();
    						if(line.equals("cmd LC_ID_DYLIB")) {
    							line = reader.readLine(); // cmdsize line
    							if(line!=null && line.trim().startsWith("cmdsize ")) {
    								line = reader.readLine(); // name line
        							if(line!=null && (line = line.trim()).startsWith("name ")) {
        								line = line.substring(5);
        								int idx = line.indexOf(" (");
        								if(idx>0) {
        									result.id = line.substring(0, idx).trim();
        								}
        							}
    							}
    						}else if(line.equals("cmd LC_LOAD_DYLIB")) {
    							line = reader.readLine(); // cmdsize line
    							if(line!=null && line.trim().startsWith("cmdsize ")) {
    								line = reader.readLine(); // name line
        							if(line!=null && (line = line.trim()).startsWith("name ")) {
        								line = line.substring(5);
        								int idx = line.indexOf(" (");
        								if(idx>0) {
        									result.loadDylibs.add(line.substring(0, idx).trim());
        								}
        							}
    							}
    						}else if(line.equals("cmd LC_RPATH")) {
    							line = reader.readLine(); // cmdsize line
    							if(line!=null && line.trim().startsWith("cmdsize ")) {
    								line = reader.readLine(); // path line
        							if(line!=null && (line = line.trim()).startsWith("path ")) {
        								line = line.substring(5);
        								int idx = line.indexOf(" (");
        								if(idx>0) {
        									result.rpaths.add(line.substring(0, idx).trim());
        								}
        							}
    							}
    						}
    					}
    				}
    			}
    			if(result.id==null && result.loadDylibs.isEmpty() && result.rpaths.isEmpty()) {
    				result = null;
    			}
    		}
    	}
        catch ( Exception e )
        {
        	task.getProject().log(task, " - " + e.getMessage() , Project.MSG_INFO);
        }
//    	if(result!=null)
//    		task.getProject().log(task, "  rpaths: " + result.rpaths, Project.MSG_INFO);
    	return result;
    }
}
