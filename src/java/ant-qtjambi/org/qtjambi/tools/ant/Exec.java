package org.qtjambi.tools.ant;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.IOException;
import java.io.PrintStream;
import java.util.List;
import java.util.Map;

import org.apache.tools.ant.BuildException;
import org.apache.tools.ant.Project;
import org.apache.tools.ant.PropertyHelper;

import org.qtjambi.qt.osinfo.OSInfo;

/**
 * @todo Rewrite. This kind of API is PITA to use and maintain.
 */
class Exec {

    /**
     * Convenience method for exec(String, File).
     * @param command Command to be executed.
     */
    public static void exec(String command, Project project) {
        exec(command, null, project);
    }

    /**
     * Execute command command in directory dir.
     * @param command Command to be executed.
     * @param dir Directory where command should be executed.
     * @throws BuildException Thrown if process exit value is not zero or IOException has been occurred.
     */
    public static void exec(String command, File directory, Project project) throws BuildException {
        String directoryString = ((directory != null) ? "(" + Util.makeCanonical(directory) + ")" : "");
        project.log("Running : " + directoryString + " " + command, Project.MSG_INFO);
        try {
            ProcessBuilder builder = new ProcessBuilder(command);
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
    public static void exec(String cmd[], File directory, Project project, boolean verbose) throws BuildException {
        if(verbose) {
            StringBuilder b = new StringBuilder();
            for(String s : cmd)
                b.append(s).append(' ');
            project.log("Running : " + ((directory != null)? "(" + Util.makeCanonical(directory) + ")" : "") + " " + b, Project.MSG_INFO);
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

    public static void execute(List<String> command, File directory, Project project) throws BuildException {
        execute(command, directory, project, null, null);
    }

    public static final String K_LD_LIBRARY_PATH = "LD_LIBRARY_PATH";
    public static final String K_DYLD_LIBRARY_PATH = "DYLD_LIBRARY_PATH";

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

    private static String whichLDEnvironmentVariable() {
        if(OSInfo.isLinux())
            return K_LD_LIBRARY_PATH;
        else if(OSInfo.isMacOS())
            return K_DYLD_LIBRARY_PATH;
        return null;
    }

    private static void setupEnvironment(Map<String, String> env, PropertyHelper props, String path, String ldpath) {
        String s;

        String envName = whichLDEnvironmentVariable();
        if(envName != null) {
            if(ldpath != null) {
                prependEnvironmentWithPathSeparator(env, envName, ldpath);
            } else {
                s = AntUtil.getPropertyAsString(props, Constants.LIBDIR);
                if(s != null)
                    prependEnvironmentWithPathSeparator(env, envName, s);
            }
        }
        
		String key;
		if(env.containsKey("Path")){
			key = "Path";
		}else if(env.containsKey("path")){
			key = "path";
		}else{
			key = "PATH";
		}
        if(path != null) {
        	prependEnvironmentWithPathSeparator(env, key, path);
        }
		
		{
			s = AntUtil.getPropertyAsString(props, Constants.BINDIR);
			if(s != null)
				prependEnvironmentWithPathSeparator(env, key, s);
		}
        if(!System.getenv().containsKey("QTDIR")){
	        s = AntUtil.getPropertyAsString(props, Constants.BINDIR);
	        if(s != null)
	            env.put("QTDIR", new File(s).getParent());
        }
        s = AntUtil.getPropertyAsString(props, Constants.JAVA_HOME_TARGET);
        if(s != null)
            env.put("JAVA_HOME_TARGET", s);
        s = AntUtil.getPropertyAsString(props, Constants.JAVA_OSARCH_TARGET);
        if(s != null)
            env.put("JAVA_OSARCH_TARGET", s);

        //something extra?
        s = AntUtil.getPropertyAsString(props, Constants.QTJAMBI_PHONON_INCLUDEDIR);
        if(s != null && s.length() > 0)
            env.put("PHONON_INCLUDEPATH", s);

        s = AntUtil.getPropertyAsString(props, Constants.QTJAMBI_PHONON_LIBDIR);
        if(s != null && s.length() > 0)
            env.put("PHONON_LIBS", s);
    }

    public static void execute(List<String> command, File directory, Project project, String path, String ldpath) throws BuildException {
        project.log("Executing: " + command.toString() + " in directory " + ((directory != null) ? directory.toString() : "<notset>"), Project.MSG_INFO);
        ProcessBuilder builder = new ProcessBuilder(command);

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
            Util.redirectOutput(process);
            if(process.exitValue() != 0) {
                String exitValueAsHex = String.format("0x%1$08x", new Object[] { process.exitValue() });
                String inDirectory = (directory != null) ? " in " + directory.getAbsolutePath() : "";
                throw new BuildException("Running: '" + command.toString() + "'" + inDirectory + " failed.  exitStatus=" + process.exitValue() + " (" + exitValueAsHex + ")");
            }
        } catch(IOException e) {
        	System.out.println("directory="+directory.getAbsolutePath());
        	e.printStackTrace();
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
    public static String[] executeCaptureOutput(List<String> command, File directory, Project project, String path, String ldpath, boolean emitErrorExitStatus) throws BuildException, InterruptedException, IOException {
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
                project.log("Running: '" + command.toString() + "'" + inDirectory + " failed.  exitStatus=" + process.exitValue() + " (" + exitValueAsHex + ")", Project.MSG_ERR);
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
