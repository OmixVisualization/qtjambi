/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
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

import java.io.File;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;

import org.apache.tools.ant.BuildException;
import org.apache.tools.ant.Project;
import org.apache.tools.ant.PropertyHelper;
import org.apache.tools.ant.Task;

import io.qt.tools.ant.OSInfo.OS;

public class MakeTask extends Task {

    private String msg = "";
    private String target;
    private String dir = ".";
    private boolean silent = false;
    private boolean failOnError = true;
    private boolean isTools;

    private String compilerName() {
    	String compilerError = (String)PropertyHelper.getProperty(getProject(), "qtjambi.compiler.error");
    	if(compilerError!=null && !compilerError.isEmpty())
    		throw new BuildException(compilerError);
        if(isTools) {
            String make = System.getenv("MAKE");
            if(make !=  null)
                return make;
        	String compilerPathValue = (String)PropertyHelper.getProperty(getProject(), "tools.compiler.path");
        	if(compilerPathValue!=null && !compilerPathValue.isEmpty()) {
	        	switch(OSInfo.os()) {
		        case Windows:
		            String compiler = (String)PropertyHelper.getProperty(getProject(), Constants.TOOLS_COMPILER);
		
		            if(FindCompiler.Compiler.MinGW.toString().equals(compiler)
	            			|| FindCompiler.Compiler.MinGW_W64.toString().equals(compiler))
		                return new File(compilerPathValue, "mingw32-make.exe").getAbsolutePath();
		            return new File(compilerPathValue, "nmake.exe").getAbsolutePath();
				default:
					break;
		        }
	        	return new File(compilerPathValue, "make.exe").getAbsolutePath();
        	}else {
        		switch(OSInfo.os()) {
    	        case Windows:
    	            String compiler = (String)PropertyHelper.getProperty(getProject(), Constants.COMPILER);
    	
    	            if(FindCompiler.Compiler.MinGW.toString().equals(compiler) || FindCompiler.Compiler.MinGW_W64.toString().equals(compiler))
    	                return "mingw32-make";
    	            String qtdir = (String)PropertyHelper.getProperty(getProject(), "qtjambi.qtdir");
    	            if(qtdir!=null) {
    	            	File jom = new File(qtdir);
    	            	jom = new File(jom.getParentFile().getParentFile(), "Tools\\QtCreator\\bin\\jom\\jom.exe");
    	            	if(jom.exists())
    	            		return jom.getAbsolutePath();
    	            }
    	            return "nmake";
    			default:
    				break;
    	        }
    	        return "make";
        	}
        }else {
	        String make_tool = (String)PropertyHelper.getProperty(getProject(), "make.tool");
	        if(make_tool!=null && !make_tool.isEmpty()){
	            return make_tool;
	        }
	
	        if(OSInfo.crossOS()!=OSInfo.os() && OSInfo.crossOS()==OS.Android) {
	        	String ndkRoot = (String)PropertyHelper.getProperty(getProject(), "qtjambi.android.ndk");
	        	if(ndkRoot!=null && !ndkRoot.isEmpty()) {
	        		File nrkRootFile = new File(ndkRoot);
	        		if(nrkRootFile.isDirectory()) {
		    	        switch(OSInfo.os()) {
		    	        case Windows:
		    	        	return nrkRootFile.getAbsolutePath() + "\\prebuilt\\windows-x86_64\\bin\\make.exe";
		    			default:
		    				break;
		    	        }
	        		}
	        	}
	        }
	        String make = System.getenv("MAKE");
	        if(make !=  null)
	            return make;
	        switch(OSInfo.os()) {
	        case Windows:
	            String compiler = (String)PropertyHelper.getProperty(getProject(), Constants.COMPILER);
	
	            if(FindCompiler.Compiler.MinGW.toString().equals(compiler) || FindCompiler.Compiler.MinGW_W64.toString().equals(compiler))
	                return "mingw32-make";
	            String qtdir = (String)PropertyHelper.getProperty(getProject(), "qtjambi.qtdir");
	            if(qtdir!=null) {
	            	File jom = new File(qtdir);
	            	jom = new File(jom.getParentFile().getParentFile(), "Tools\\QtCreator\\bin\\jom\\jom.exe");
	            	if(jom.exists())
	            		return jom.getAbsolutePath();
	            }
	            return "nmake";
			default:
				break;
	        }
	        return "make";
        }
    }

    @Override
    public void execute() throws BuildException {
        getProject().log(this, msg, Project.MSG_INFO);

        String compilerName = compilerName();
        List<String> commandArray = new ArrayList<String>();
        commandArray.add(compilerName);

        if(silent && OSInfo.os() != OSInfo.OS.Windows) {
            commandArray.add("-s");
        }

        try {
            final String makeOptions = System.getenv("MAKEOPTS");
            List<String> list = Util.safeSplitStringTokenizer(makeOptions);
            if(list != null)
                commandArray.addAll(list);
        } catch(SecurityException e) {
        }
        try {
	        String makeOptions = (String)PropertyHelper.getProperty(getProject(), "make.args");
            List<String> list = Util.safeSplitStringTokenizer(makeOptions);
            if(list != null)
                commandArray.addAll(list);
        } catch(SecurityException e) {
        }

        PropertyHelper propertyHelper = PropertyHelper.getPropertyHelper(getProject());
        if (!"false".equalsIgnoreCase(AntUtil.getPropertyAsString(propertyHelper, "parallel"))
        		&& !compilerName.endsWith("nmake.exe")
        		&& !compilerName.endsWith("jom.exe")) {
        	boolean hasJArg = false;
        	for(String arg : commandArray) {
        		if(arg.startsWith("-j")) {
        			hasJArg = true;
        			break;
        		}
        	}
        	if(!hasJArg && Runtime.getRuntime().availableProcessors()>1) {
        		commandArray.add("-j"+Runtime.getRuntime().availableProcessors());
        	}
        }

        if(target != null && !target.isEmpty())
            commandArray.add(target);

        String ldpath = AntUtil.getPropertyAsString(propertyHelper, Constants.LIBDIR);
        try {
            File dirExecute = null;
            if(dir != null)
                dirExecute = new File(dir);
            if(isTools) {
            	String compilerPathValue = (String)PropertyHelper.getProperty(getProject(), "tools.compiler.path");
            	Exec.execute(this, commandArray, dirExecute, getProject(), 
            			compilerPathValue!=null && !compilerPathValue.isEmpty() ? new File(compilerPathValue).getAbsolutePath() : null, ldpath);
            }else {
            	String path = null;
            	Map<String, String> newEnv = null;
            	if(OSInfo.crossOS()!=OSInfo.os() && OSInfo.crossOS()==OS.Android) {
    	        	String ndkRoot = (String)PropertyHelper.getProperty(getProject(), "qtjambi.android.ndk");
    	        	if(ndkRoot!=null) {
    	        		newEnv = new TreeMap<>();
    	        		newEnv.put("ANDROID_NDK_ROOT", ndkRoot.replace('\\', '/'));
    	        		File nrkRootFile = new File(ndkRoot);
    	    	        switch(OSInfo.os()) {
    	    	        case Windows:
    	    	        	path = nrkRootFile.getAbsolutePath() + "\\prebuilt\\windows-x86_64\\bin;"
    	    	        			+ nrkRootFile.getAbsolutePath() + "\\toolchains\\llvm\\prebuilt\\windows-x86_64\\bin";
    	    			default:
    	    				break;
    	    	        }
    	        	}
    	        }
            	Exec.execute(this, commandArray, dirExecute, getProject(), path, ldpath, newEnv);
            }
        } catch(BuildException e) {
            if(failOnError)
                throw e;
            else
                getProject().log(this, e.getMessage() + "; failOnError=" + failOnError + "; continuing", e, Project.MSG_ERR);
        }
    }

    public void setMessage(String msg) {
        this.msg = msg;
    }

    public void setTarget(String target) {
        this.target = target;
    }

    public void setSilent(boolean silent) {
        this.silent = silent;
    }

    public void setDir(String dir) {
        this.dir = dir;
    }

    public void setFailOnError(boolean failOnError) {
        this.failOnError = failOnError;
    }
    
    public void setTools(boolean tools) {
    	isTools = tools;
    }
}
