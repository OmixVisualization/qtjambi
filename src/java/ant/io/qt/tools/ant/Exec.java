/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
import java.util.ArrayList;
import java.util.Arrays;
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

	private static String commandLine(OSInfo osInfo, List<String> command) {
		List<String> command2 = new ArrayList<>(command);
		for (int i = 0; i < command2.size(); i++) {
			String arg = command2.get(i);
			if(osInfo.os()==OSInfo.OperationSystem.Windows) {
        		if(arg.contains(" ") && !arg.startsWith("\"") && !arg.endsWith("\""))
        			command2.set(i, "\""+arg+"\"");
        	}else if(arg.contains(" ") || arg.contains("$")){
        		command2.set(i, arg.replace(" ", "\\ ").replace("$", "\\$"));
        	}
		}
		return String.join(" ", command2);
    }
	
	private static String commandLine(OSInfo osInfo, String[] command) {
		List<String> command2 = new ArrayList<>(Arrays.asList(command));
		for (int i = 0; i < command2.size(); i++) {
			String arg = command2.get(i);
			if(osInfo.os()==OSInfo.OperationSystem.Windows) {
        		if(arg.contains(" ") || arg.contains("\\"))
        			command2.set(i, "\""+arg+"\"");
        	}else if(arg.contains(" ") || arg.contains("$")){
        		command2.set(i, arg.replace(" ", "\\ ").replace("$", "\\$"));
        	}
		}
		return String.join(" ", command2);
    }
	
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
        try {
            ProcessBuilder builder = new ProcessBuilder(command);
			builder.redirectErrorStream(true);
            if(directory != null)
                builder.directory(directory);
            Process process = builder.start();
            Runnable wait = Util.redirectOutput(process);
            wait.run();
            if(process.exitValue() != 0) {
                String exitValueAsHex = String.format("0x%1$08x", new Object[] { process.exitValue() });
                String inDirectory = (directory != null) ? " in " + directory.getAbsolutePath() : "";
                project.log(task, "Running: '" + command + "'" + inDirectory + " failed.  exitStatus=" + process.exitValue() + " (" + exitValueAsHex + ")", Project.MSG_ERR);
                throw new BuildException("Running: '" + command + "'" + inDirectory + " failed.  exitStatus=" + process.exitValue() + " (" + exitValueAsHex + ")");
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
    	OSInfo osInfo = OSInfo.instance(project);
    	if(verbose){
        	String log = "Executing: '" +commandLine(osInfo, cmd)+ "' in directory " + ((directory != null) ? directory.toString() : "<notset>");
        	project.log(task, log, Project.MSG_INFO);
    	}
        try {
            Process process = Runtime.getRuntime().exec(cmd, null, directory);
            Runnable wait = Util.redirectOutput(process);
            wait.run();
            if(process.exitValue() != 0) {
                String exitValueAsHex = String.format("0x%1$08x", new Object[] { process.exitValue() });
                String inDirectory = (directory != null) ? " in " + directory.getAbsolutePath() : "";
                throw new BuildException("Running: '" + commandLine(osInfo, cmd) + "'" + inDirectory + " failed.  exitStatus=" + process.exitValue() + " (" + exitValueAsHex + ")");
            }
        } catch(IOException e) {
            throw new BuildException("Running: '" + commandLine(osInfo, cmd) + "' failed.", e);
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

    private static void setupEnvironment(OSInfo osInfo, Map<String, String> env, PropertyHelper props, String path, String ldpath) {
        String s;

        if(osInfo.os().isUnixLike()) {
        	if(ldpath != null) {
                prependEnvironmentWithPathSeparator(env, K_LD_LIBRARY_PATH, ldpath);
            } else {
                s = AntUtil.getPropertyAsString(props, Constants.LIBDIR);
                if(s != null)
                    prependEnvironmentWithPathSeparator(env, K_LD_LIBRARY_PATH, s);
            }
        }else if(osInfo.os()==OSInfo.OperationSystem.MacOS) {
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
    	OSInfo osInfo = OSInfo.instance(project);
    	{
        	project.log(task, "Executing: '" +commandLine(osInfo, command)+ "' in directory " + ((directory != null) ? directory.toString() : "<notset>"), Project.MSG_INFO);
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
            setupEnvironment(osInfo, env, props, path, ldpath);
        }
        
        if(directory != null)
            builder.directory(directory);
        try {
            Process process = builder.start();
            Runnable wait = Util.redirectOutput(process);
            wait.run();
            if(process.exitValue() != 0) {
                String exitValueAsHex = String.format("0x%1$08x", new Object[] { process.exitValue() });
                String inDirectory = (directory != null) ? " in " + directory.getAbsolutePath() : "";
                project.log(task, "Running: '" + commandLine(osInfo, command) + "'" + inDirectory + " failed.  exitStatus=" + process.exitValue() + " (" + exitValueAsHex + ")", Project.MSG_ERR);
                throw new BuildException("Running: '" + commandLine(osInfo, command) + "'" + inDirectory + " failed.  exitStatus=" + process.exitValue() + " (" + exitValueAsHex + ")");
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
            project.log(task, "Running: '" +commandLine(osInfo, command)+ "' in directory " + ((directory != null) ? directory.toString() : "<notset>") + " failed.", e, Project.MSG_ERR);
            throw new BuildException("Running: '" + commandLine(osInfo, command) + "' failed.", e);
        }
    }
    
    public static Runnable executeAsync(Task task, List<String> command, File directory, Project project, String path, String ldpath) throws BuildException {
    	return executeAsync(task, command, directory, project, path, ldpath, null);
    }
    
    public static Runnable executeAsync(Task task, List<String> command, File directory, Project project, String path, String ldpath, Map<String, String> newEnv) throws BuildException {
    	OSInfo osInfo = OSInfo.instance(project);
    	{
        	project.log(task, "Executing: '" +commandLine(osInfo, command)+ "' in directory " + ((directory != null) ? directory.toString() : "<notset>"), Project.MSG_INFO);
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
            setupEnvironment(osInfo, env, props, path, ldpath);
        }
        
        if(directory != null)
            builder.directory(directory);
        try {
            Process process = builder.start();
            Runnable wait = Util.redirectOutput(process);
            return ()->{
            	wait.run();
	            if(process.exitValue() != 0) {
	                String exitValueAsHex = String.format("0x%1$08x", new Object[] { process.exitValue() });
	                String inDirectory = (directory != null) ? " in " + directory.getAbsolutePath() : "";
	                project.log(task, "Running: '" + commandLine(osInfo, command) + "'" + inDirectory + " failed.  exitStatus=" + process.exitValue() + " (" + exitValueAsHex + ")", Project.MSG_ERR);
	                throw new BuildException("Running: '" + commandLine(osInfo, command) + "'" + inDirectory + " failed.  exitStatus=" + process.exitValue() + " (" + exitValueAsHex + ")");
	            }
            };
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
            project.log(task, "Running: '" +commandLine(osInfo, command)+ "' in directory " + ((directory != null) ? directory.toString() : "<notset>") + " failed.", e, Project.MSG_ERR);
            throw new BuildException("Running: '" + commandLine(osInfo, command) + "' failed.", e);
        }
    }

    public static void execute(Task task, List<String> command, File directory, Project project, String path, String ldpath, File outputFile, File errorFile) throws BuildException {
    	OSInfo osInfo = OSInfo.instance(project);
        String inDirectory;
        try {
			inDirectory = (directory != null) ? " in " + directory.getCanonicalPath() : "";
		} catch (IOException e) {
			inDirectory = (directory != null) ? " in " + directory.getAbsolutePath() : "";
		}
    	project.log(task, "Running '" + commandLine(osInfo, command) + "'"+inDirectory+" ...", Project.MSG_INFO);
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
            setupEnvironment(osInfo, env, props, path, ldpath);
        }
        
        if(directory != null)
            builder.directory(directory);
        try {
            Process process = builder.start();
            process.waitFor();
			//Util.redirectOutput(process, outputFile, errorFile);
            if(process.exitValue() != 0) {
                String exitValueAsHex = String.format("0x%1$08x", new Object[] { process.exitValue() });
                project.log(task, "FAILED! exitStatus=" + process.exitValue() + " (" + exitValueAsHex + ")", Project.MSG_ERR);
                throw new BuildException("Running: '" + command.get(0) + "' failed.  exitStatus=" + process.exitValue() + " (" + exitValueAsHex + ")");
            }
            project.log(task, "Finished!", Project.MSG_INFO);
        } catch(InterruptedException e) {
            throw new BuildException("Running: '" + command.get(0) + "' failed.", e);
        } catch(IOException e) {
        	e.printStackTrace();
            project.log(task, "FAILED!", e, Project.MSG_ERR);
            throw new BuildException("Running: '" + commandLine(osInfo, command) + "' failed.", e);
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
    public static String[] executeCaptureOutput(Task task, List<String> command, File directory, Project project, OSInfo osInfo, String path, String ldpath, boolean emitErrorExitStatus) throws BuildException, InterruptedException, IOException {
    	ProcessBuilder builder = new ProcessBuilder(command);

        // NOTE: this is most likely very linux-specific system. For Windows one would use PATH instead,
        // but it should not be needed there in first place... Only if you want to have same kind of building
        // environment one can have for Linux.
        // it shouldn't affect to Windows environment though.
        Map<String, String> env = builder.environment();
        PropertyHelper props = PropertyHelper.getPropertyHelper(project);
        setupEnvironment(osInfo, env, props, path, ldpath);

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
                project.log(task, "Running: '" + commandLine(osInfo, command) + "'" + inDirectory + " failed.  exitStatus=" + process.exitValue() + " (" + exitValueAsHex + ")", Project.MSG_ERR);
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
}
