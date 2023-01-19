/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
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

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.IOException;
import java.io.PrintStream;
import java.util.List;
import java.util.Map;

import org.apache.tools.ant.BuildException;
import org.apache.tools.ant.Project;
import org.apache.tools.ant.PropertyHelper;
import org.apache.tools.ant.Task;

/**
 * @todo Rewrite. This kind of API is PITA to use and maintain.
 */
class Exec {

    /**
     * Convenience method for exec(String, File).
     * @param command Command to be executed.
     */
    public static void exec(Task task, String command, Project project) {
        exec(task, command, null, project);
    }

    /**
     * Execute command command in directory dir.
     * @param command Command to be executed.
     * @param dir Directory where command should be executed.
     * @throws BuildException Thrown if process exit value is not zero or IOException has been occurred.
     */
    public static void exec(Task task, String command, File directory, Project project) throws BuildException {
    	{
        	String log = "Executing: \"" +command+ "\" in directory " + ((directory != null) ? directory.toString() : "<notset>");
        	project.log(task, log, Project.MSG_INFO);
    	}
        try {
            ProcessBuilder builder = new ProcessBuilder(command);
			builder.redirectErrorStream(true);
            if(directory != null)
                builder.directory(directory);
            Process process = builder.start();
            Util.redirectOutput(process);
            if(process.exitValue() != 0) {
                String exitValueAsHex = String.format("0x%1$08x", new Object[] { process.exitValue() });
                String inDirectory = (directory != null) ? " in " + directory.getAbsolutePath() : "";
                throw new BuildException("Running: '" + command.toString() + "'" + inDirectory + " failed.  exitStatus=" + process.exitValue() + " (" + exitValueAsHex + ")");
            }
        } catch(IOException e) {
            throw new BuildException("Running: " + command + " failed with error message: " + e.getMessage(), e);
        }
    }

    /**
     * TODO: this should be merged with above one, repeating code is not that wise.
     *
     * Executes process in more verbose manner.
     * @param cmd Array of command and its arguments to be executed.
     * @param dir Directory where should be executed.
     * @param verbose Whether to be verbose.
     * @throws BuildException Thrown if process exit value is not zero or IOException has been occurred.
     */
    public static void exec(Task task, String cmd[], File directory, Project project, boolean verbose) throws BuildException {
    	if(verbose){
        	String log = "Executing: \"" +join(cmd).trim()+ "\" in directory " + ((directory != null) ? directory.toString() : "<notset>");
        	project.log(task, log, Project.MSG_INFO);
    	}
        try {
            Process process = Runtime.getRuntime().exec(cmd, null, directory);
            Util.redirectOutput(process);
            if(process.exitValue() != 0) {
                String exitValueAsHex = String.format("0x%1$08x", new Object[] { process.exitValue() });
                String inDirectory = (directory != null) ? " in " + directory.getAbsolutePath() : "";
                throw new BuildException("Running: '" + join(cmd) + "'" + inDirectory + " failed.  exitStatus=" + process.exitValue() + " (" + exitValueAsHex + ")");
            }
        } catch(IOException e) {
            throw new BuildException("Running: '" + join(cmd) + "' failed.", e);
        }
    }

    public static void execute(Task task, List<String> command, File directory, Project project) throws BuildException {
        execute(task, command, directory, project, null, null, null);
    }

    public static final String K_LD_LIBRARY_PATH = "LD_LIBRARY_PATH";
    public static final String K_DYLD_LIBRARY_PATH = "DYLD_LIBRARY_PATH";
    public static final String K_DYLD_FRAMEWORK_PATH = "DYLD_FRAMEWORK_PATH";

    /**
     * This prepends a new value to an environment variable that is a list of paths delimited by pathSeparator.
     * @param env
     * @param prependValue
     * @return
     */
    private static String prependEnvironmentWithPathSeparator(Map<String, String> env, String varName, String prependValue) {
        String v = env.get(varName);
        if(v != null)
            prependValue = prependValue + File.pathSeparatorChar + v;
        env.put(varName, prependValue);
        return v;
    }

    private static void setupEnvironment(Map<String, String> env, PropertyHelper props, String path, String ldpath) {
        String s;

        if(OSInfo.os()==OSInfo.OS.Linux) {
        	if(ldpath != null) {
                prependEnvironmentWithPathSeparator(env, K_LD_LIBRARY_PATH, ldpath);
            } else {
                s = AntUtil.getPropertyAsString(props, Constants.LIBDIR);
                if(s != null)
                    prependEnvironmentWithPathSeparator(env, K_LD_LIBRARY_PATH, s);
            }
        }else if(OSInfo.os()==OSInfo.OS.MacOS) {
        	if(ldpath != null) {
                prependEnvironmentWithPathSeparator(env, K_DYLD_LIBRARY_PATH, ldpath);
                prependEnvironmentWithPathSeparator(env, K_DYLD_FRAMEWORK_PATH, ldpath);
            } else {
                s = AntUtil.getPropertyAsString(props, Constants.LIBDIR);
                if(s != null) {
                    prependEnvironmentWithPathSeparator(env, K_DYLD_LIBRARY_PATH, s);
                    prependEnvironmentWithPathSeparator(env, K_DYLD_FRAMEWORK_PATH, s);
                }
            }
        }
        
		String key;
		if(env.containsKey("PATH")){
			key = "PATH";
		}else if(env.containsKey("Path")){
			key = "Path";
		}else if(env.containsKey("path")){
			key = "path";
		}else{
			key = "PATH";
		}
        if(path != null) {
        	prependEnvironmentWithPathSeparator(env, key, path);
        }else{
			s = AntUtil.getPropertyAsString(props, Constants.BINDIR);
			if(s != null && !s.isEmpty())
				prependEnvironmentWithPathSeparator(env, key, s);
		}
        if(!System.getenv().containsKey("QTDIR")){
	        s = AntUtil.getPropertyAsString(props, Constants.BINDIR);
	        if(s != null && !s.isEmpty())
	            env.put("QTDIR", new File(s).getParent());
        }
        s = AntUtil.getPropertyAsString(props, Constants.JAVA_HOME_TARGET);
        if(s != null && !s.isEmpty())
            env.put("JAVA_HOME_TARGET", s);
        s = AntUtil.getPropertyAsString(props, Constants.JAVA_OSARCH_TARGET);
        if(s != null && !s.isEmpty())
            env.put("JAVA_OSARCH_TARGET", s);
    }
    
    public static void execute(Task task, List<String> command, File directory, Project project, String path, String ldpath) throws BuildException {
    	execute(task, command, directory, project, path, ldpath, null);
    }

    public static void execute(Task task, List<String> command, File directory, Project project, String path, String ldpath, Map<String, String> newEnv) throws BuildException {
    	{
        	String log = "";
        	for (String string : command) {
    			log += " " + string;
    		}
        	log = "Executing: \"" +log.trim()+ "\" in directory " + ((directory != null) ? directory.toString() : "<notset>");
        	project.log(task, log, Project.MSG_INFO);
    	}
        ProcessBuilder builder = new ProcessBuilder(command);
		//builder = builder.redirectErrorStream(true);

        // NOTE: this is most likely very linux-specific system. For Windows one would use PATH instead,
        // but it should not be needed there in first place... Only if you want to have same kind of building
        // environment one can have for Linux.
        // it shouldn't affect to Windows environment though.
        Map<String, String> env = builder.environment();
        if(newEnv!=null) {
        	env.putAll(newEnv);
        }else if(project != null) {
            PropertyHelper props = PropertyHelper.getPropertyHelper(project);
            setupEnvironment(env, props, path, ldpath);
        }
        
        if(directory != null)
            builder.directory(directory);
        try {
            Process process = builder.start();
            Util.redirectOutput(process);
            if(process.exitValue() != 0) {
                String exitValueAsHex = String.format("0x%1$08x", new Object[] { process.exitValue() });
                String inDirectory = (directory != null) ? " in " + directory.getAbsolutePath() : "";
                throw new BuildException("Running: '" + command.toString() + "'" + inDirectory + " failed.  exitStatus=" + process.exitValue() + " (" + exitValueAsHex + ")");
            }
        } catch(IOException e) {
        	e.printStackTrace();
            if(directory != null)
            	System.err.println("directory="+directory.getAbsolutePath());
            if(env.containsKey("path"))
            	System.err.println("path="+env.get("path"));
            if(env.containsKey("Path"))
            	System.err.println("Path="+env.get("Path"));
            if(env.containsKey("PATH"))
            	System.err.println("PATH="+env.get("PATH"));
            throw new BuildException("Running: '" + command.toString() + "' failed.", e);
        }
    }

    public static void execute(Task task, List<String> command, File directory, Project project, String path, String ldpath, File outputFile, File errorFile) throws BuildException {
    	{
        	String log = "";
        	for (String string : command) {
    			log += " " + string;
    		}
        	log = "Executing: \"" +log.trim()+ "\" in directory " + ((directory != null) ? directory.toString() : "<notset>");
        	project.log(task, log, Project.MSG_INFO);
    	}
        ProcessBuilder builder = new ProcessBuilder(command);
		//builder = builder.redirectErrorStream(true);
		builder.redirectError(errorFile);
		builder.redirectOutput(outputFile);

        // NOTE: this is most likely very linux-specific system. For Windows one would use PATH instead,
        // but it should not be needed there in first place... Only if you want to have same kind of building
        // environment one can have for Linux.
        // it shouldn't affect to Windows environment though.
        Map<String, String> env = builder.environment();
        if(project != null) {
            PropertyHelper props = PropertyHelper.getPropertyHelper(project);
            setupEnvironment(env, props, path, ldpath);
        }
        
        if(directory != null)
            builder.directory(directory);
        try {
            Process process = builder.start();
            process.waitFor();
			//Util.redirectOutput(process, outputFile, errorFile);
            if(process.exitValue() != 0) {
                String exitValueAsHex = String.format("0x%1$08x", new Object[] { process.exitValue() });
                String inDirectory = (directory != null) ? " in " + directory.getAbsolutePath() : "";
                if(directory != null)
                	project.log(task, "directory="+directory.getAbsolutePath(), Project.MSG_WARN);
                if(env.containsKey("path"))
                	project.log(task, "path="+env.get("path"), Project.MSG_WARN);
                if(env.containsKey("Path"))
                	project.log(task, "Path="+env.get("Path"), Project.MSG_WARN);
                if(env.containsKey("PATH"))
                	project.log(task, "PATH="+env.get("PATH"), Project.MSG_WARN);
                throw new BuildException("Running: '" + command.toString() + "'" + inDirectory + " failed.  exitStatus=" + process.exitValue() + " (" + exitValueAsHex + ")");
            }
        } catch(InterruptedException e) {
            throw new BuildException("Running: '" + command.toString() + "' failed.", e);
        } catch(IOException e) {
        	e.printStackTrace();
            if(directory != null)
            	project.log(task, "directory="+directory.getAbsolutePath(), Project.MSG_WARN);
            if(env.containsKey("path"))
            	project.log(task, "path="+env.get("path"), Project.MSG_WARN);
            if(env.containsKey("Path"))
            	project.log(task, "Path="+env.get("Path"), Project.MSG_WARN);
            if(env.containsKey("PATH"))
            	project.log(task, "PATH="+env.get("PATH"), Project.MSG_WARN);
            throw new BuildException("Running: '" + command.toString() + "' failed.", e);
        }
    }

    /**
     * 
     * @param command
     * @param directory
     * @param project  May never be null.
     * @param ldpath
     * @param emitErrorExitStatus
     * @return
     * @throws BuildException
     * @throws InterruptedException
     * @throws IOException
     */
    public static String[] executeCaptureOutput(Task task, List<String> command, File directory, Project project, String path, String ldpath, boolean emitErrorExitStatus) throws BuildException, InterruptedException, IOException {
        ProcessBuilder builder = new ProcessBuilder(command);

        // NOTE: this is most likely very linux-specific system. For Windows one would use PATH instead,
        // but it should not be needed there in first place... Only if you want to have same kind of building
        // environment one can have for Linux.
        // it shouldn't affect to Windows environment though.
        Map<String, String> env = builder.environment();
        PropertyHelper props = PropertyHelper.getPropertyHelper(project);
        setupEnvironment(env, props, path, ldpath);

        if(directory != null)
            builder.directory(directory);
        PrintStream out = null;
        PrintStream err = null;
        try {
            Process process = builder.start();
            
            ByteArrayOutputStream outdata = new ByteArrayOutputStream();
            out = new PrintStream(outdata);

            ByteArrayOutputStream errdata = new ByteArrayOutputStream();
            err = new PrintStream(errdata);

            StreamConsumer stdoutReader = new StreamConsumer(process.getInputStream(), out);
            StreamConsumer stderrReader = new StreamConsumer(process.getErrorStream(), err);
            stdoutReader.start();
            stderrReader.start();
            process.waitFor();

            stdoutReader.join();
            stderrReader.join();

            err.close();
            err = null;
            out.close();
            out = null;

            if(emitErrorExitStatus && process.exitValue() != 0) {
                String exitValueAsHex = String.format("0x%1$08x", new Object[] { process.exitValue() });
                String inDirectory = (directory != null) ? " in " + directory.getAbsolutePath() : "";
                project.log(task, "Running: '" + command.toString() + "'" + inDirectory + " failed.  exitStatus=" + process.exitValue() + " (" + exitValueAsHex + ")", Project.MSG_ERR);
            }

            return new String[] { outdata.toString(), errdata.toString() };
        } finally {
            if(err != null) {
                try {
                    err.close();
                } catch(Exception eat) {
                }
                err = null;
            }
            if(out != null) {
                try {
                    out.close();
                } catch(Exception eat) {
                }
                out = null;
            }
        }
    }

    /**
     * Internal helper of Exec.
     * @param ar What to join
     * @return array joined together to form "foo1, foo2, .."
     */
    private static String join(String ar[]) {
        String s = "";
        for(int i = 0; i<ar.length; ++i) {
            s += ar[i];
            if(i < ar.length - 1)
                s += " ";
        }
        return s;
    }
}
