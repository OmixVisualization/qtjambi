/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package cz.dalvi.qtjambiplugin;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import org.apache.maven.plugin.AbstractMojo;
import org.apache.maven.plugin.MojoExecutionException;
import org.apache.maven.plugin.MojoFailureException;
import org.apache.maven.project.MavenProject;

/**
 *
 * @author admin
 * @goal generate
 * @phase generate-sources
 */
public class QtJambiMOJO extends AbstractMojo {

    /**
     * Path to sources dir. If not specified, ./src/main/java is used.
     * 
     * @parameter expression="${qtjambi.sourcesDir}"
     */
    private File sourcesDir;
    /**
     * Destination directory. Default the same as sourceDir.
     *
     * @parameter expression="${basedir}/target/generated-sources/qtjambi"
     * @required
     */
    private File destinationDir;
    /**
     * Path to translations dir. If not specified, ./src/main/resources/translations is used.
     *
     * @parameter expression="${qtjambi.translationsDir}"
     */
    private File translationsDir;
    /**
     * Base directory.
     *
     * @parameter expression="${basedir}"
     * @required
     */
    private File baseDir;
    /**
     * Package name of generated files
     * @parameter expression="${qtjambi.packageName}"
     */
    private String packageName;
    /**
     * @parameter expression="${project}"
     * @required
     * @readonly
     * @since 1.0
     */
    private MavenProject project;
    /**
     * @parameter expression="${qtjambi.noObsoleteTranslations}" default-value=true
     */
    private boolean noObsoleteTranslations;


    /*
     * 
     */
    private enum Platforms {

        WIN32, WIN64, LIN32, LIN64, MAC32, MAC64
    };

    private enum Tools {

        JUIC, LUPDATE, LRELEASE;
        private File tool_file = null;

        public void setFile(File f) {
            tool_file = f;
        }

        public File getFile() {
            return tool_file;
        }
    };
    private static final String dotJui = ".jui";
    private static final String dotJava = ".java";
    private static final String dotTs = ".ts";
    private Platforms platform;
    private File binariesDir;

    @Override
    public void execute() throws MojoExecutionException, MojoFailureException {

        // setup platform
        setupPlatform();

        // compile jui files
        compileJuiFiles();

        // process translations
        processTranslations();

        // add created jars to project
        project.addCompileSourceRoot(destinationDir.getAbsolutePath());

        // delete temp dir
        org.apache.commons.io.FileUtils.deleteQuietly(binariesDir);


    }

    private void setupPlatform() throws MojoExecutionException {


        // platform specific set of tools
        int platform_bits = System.getProperty("os.arch").contains("64") ? 64 : 32;
        String platform_name = System.getProperty("os.name").toLowerCase();

        getLog().debug("platform=" + platform_bits + ", os.name=" + platform_name);

        platform_name = platform_name.toLowerCase();

        // setup propper file names for current platform
        String[] tool_files;
        if (platform_name.contains("windows") && platform_bits == 32) {
            platform = Platforms.WIN32;
            tool_files = new String[]{"juic.exe", "lupdate.exe", "lrelease.exe", "QtCore4.dll", "QtXml4.dll"};
        } else if (platform_name.contains("windows") && platform_bits == 64) {
            platform = Platforms.WIN64;
            tool_files = new String[]{"juic.exe", "lupdate.exe", "lrelease.exe", "QtCore4.dll", "QtXml4.dll"};
        } else if (platform_name.contains("linux") && platform_bits == 32) {
            platform = Platforms.LIN32;
            tool_files = new String[]{"juic", "lupdate", "lrelease", "libQtCore.so.4", "libQtXml.so.4"};
        } else if (platform_name.contains("linux") && platform_bits == 64) {
            platform = Platforms.LIN64;
            tool_files = new String[]{"juic", "lupdate", "lrelease", "libQtCore.so.4", "libQtXml.so.4"};
        } else if (platform_name.contains("mac") && platform_bits == 32) {
            platform = Platforms.MAC32;
            tool_files = new String[]{"juic", "lupdate", "lrelease", "libQtCore.4.dylib", "libQtXml.4.dylib"};
        } else if (platform_name.contains("mac") && platform_bits == 64) {
            throw new MojoExecutionException("Platform mac64 not yet supported");
        } else {
            throw new MojoExecutionException("Uknown platform");
        }

        try {

            if (sourcesDir == null) {
                throw new MojoExecutionException("You must specify sourcesDir");
            }
            if (destinationDir == null) {
                destinationDir = new File(sourcesDir.getAbsolutePath());
            }
            if (translationsDir == null) {
                translationsDir = new File(baseDir, "src/main/resources/translations");
            }

            // create temp directory where qtjambi tools should be extracted
            binariesDir = File.createTempFile("qtjambi_tools", "");
            binariesDir.delete();
            if (!binariesDir.mkdirs()) {
                throw new IOException("Failed to create directory " + binariesDir.getAbsolutePath());
            }

            // extract tools there, setup path to tools
            int idx = 0;
            for (String s : tool_files) {
                File tool = saveResource(s, binariesDir);
                switch (idx++) {
                    case 0:
                        Tools.JUIC.setFile(tool);
                        break;
                    case 1:
                        Tools.LUPDATE.setFile(tool);
                        break;
                    case 2:
                        Tools.LRELEASE.setFile(tool);
                        break;

                }
            }

            // make binaries executable on linux
            // TODO what about mac?
            if (platform_name.toLowerCase().contains("linux")) {
                Tools.JUIC.getFile().setExecutable(true);
                Tools.LRELEASE.getFile().setExecutable(true);
                Tools.LUPDATE.getFile().setExecutable(true);
            }


        } catch (IOException ex) {
            throw new MojoExecutionException("Failed to initialize plugin", ex);
        }

        getLog().debug("QtJambiPlugin initialized successfully");

    }

    private void compileJuiFiles() throws MojoExecutionException {

        Process process;
        int retval;
        FileOutputStream fos;

        try {

            // find jui files in source dir
            getLog().debug("Looking for jui files in " + sourcesDir.getAbsolutePath());
            ArrayList<File> jui_lst = new ArrayList<File>();
            findFiles(sourcesDir, jui_lst, dotJui);


            /*
             * this is a little tricky. It seems that "-a" option doesn't work as I expected,
             * so I must detect modifications myself. I'll do this because all .jui files
             * are re-generated on each build and they are everytime compiled. This should slow down
             * the build process dramatically when developing application.
             * So: i'll find NEWEST file in source and destination dir and
             * if there exists a newer file in source dir, all juis will be recompiled.
             * I must do so because I can't create destination filename by source filename,
             * because destination filename is created by class name _inside_ the .jui file,
             * not by source filename.
             */

            ArrayList<File> dst_lst = new ArrayList<File>();
            findFiles(destinationDir, dst_lst, dotJava);

            long highest_src = 0, highest_dst = 0;
            for (File s : jui_lst) {
                long mtime = s.lastModified();
                if (mtime > highest_src) {
                    highest_src = mtime;
                }
            }
            for (File s : dst_lst) {
                long mtime = s.lastModified();
                if (mtime > highest_dst) {
                    highest_dst = mtime;
                }
            }

            // if no source modifications, return
            if (highest_dst >= highest_src) {
                getLog().debug("No files modified, skipping juic");
                return;
            }

            getLog().debug("Sources modified, compiling all form files");


            // compile all jui files
            for (File jui_file : jui_lst) {

                String[] cmdarray = new String[]{
                    "-d",
                    destinationDir.getAbsolutePath(),
                    "-p",
                    getPackageForJuiFile(jui_file.getAbsolutePath()),
                    jui_file.getAbsolutePath()
                };

                // execute juic
                executeTool(Tools.JUIC, cmdarray);

            }

            // now recode generated files to project sources encoding ( qtjambi 4.5 bug on non-us systems with datetime format in comment )
            ArrayList<File> java_lst = new ArrayList<File>();
            findFiles(destinationDir, java_lst, dotJava);

            for (File f : java_lst) {
                if (f.getName().startsWith("Ui_") && f.getName().endsWith(".java")) {

                    getLog().debug("Recoding " + f.getAbsolutePath() + " to right encoding");

                    BufferedReader r = new BufferedReader(new FileReader(f));
                    StringBuilder b = new StringBuilder();
                    String line;
                    while (null != (line = r.readLine())) {
                        b.append(line + "\n");
                        //getLog().debug("Read: " + line);
                    }
                    r.close();

                    getLog().debug("Readed");

                    // write back in right encoding
                    fos = new FileOutputStream(f);
                    String encoding = project.getProperties().getProperty("project.build.sourceEncoding");
                    if (encoding == null) {
                        encoding = "utf-8";
                    }
                    fos.write(b.toString().getBytes(encoding));
                    fos.close();

                }
            }



        } catch (Exception ex) {
            throw new MojoExecutionException("Failed to convert jui files", ex);
        }

    }

    /**
     * Searches recursively the whole directory tree pointed by
     * <code>srcDir</code>.
     *
     * @param srcDir
     *            source directory to search under.
     * @param files
     *            list to which all found files will be put.
     * @param extension
     *            filter files by extension
     */
    private void findFiles(File srcDir, List<File> files, String extension) {
        if (srcDir != null && srcDir.isDirectory()) {
            for (File f : srcDir.listFiles()) {
                if (f.isDirectory()) {
                    findFiles(f, files, extension);
                } else if (f.getName().endsWith(extension)) {
                    getLog().debug("found file: " + f.getName());
                    files.add(f);
                }
            }
        }
    }

    /**
     * Derives the package name for the given <code>juiFile</code>.
     *
     * @param juiFile
     *            the file for which the package is being derived
     * @return
     */
    private String getPackageForJuiFile(String juiFile) {

        if (packageName != null && packageName.trim().length() > 0) {
            return packageName;
        }

        /* removing the sourceDirectory path */
        String pkgName = juiFile.substring(sourcesDir.getAbsolutePath().length() + 1);
        /* removing the file name with preceding separator */
        pkgName = pkgName.substring(0, pkgName.lastIndexOf(File.separatorChar));
        /* replacing file separators with dots */
        pkgName = pkgName.replace(File.separatorChar, '.');
        getLog().debug("derived package name for: " + juiFile + " is: " + pkgName);

        return pkgName;
    }

    private void executeTool(Tools tool, String[] arguments) throws IOException, InterruptedException, MojoExecutionException {

        LinkedList<String> cmd = new LinkedList<String>();
        cmd.add(tool.getFile().getAbsolutePath());
        for (String arg : arguments) {
            if (arg != null) {
                cmd.add(arg);
            }
        }

        getLog().debug("Executing: " + cmd.toString());

        ProcessBuilder b = new ProcessBuilder(cmd);

        // export LD_LIBRARY_PATH on linux
        // TODO what about mac?
        if (platform.equals(Platforms.LIN32) || platform.equals(Platforms.LIN64)) {
            b.environment().put("LD_LIBRARY_PATH", binariesDir.getAbsolutePath());
        }

        if (platform.equals(Platforms.MAC32) || platform.equals(Platforms.MAC64)) {
            b.environment().put("DYLD_LIBRARY_PATH", binariesDir.getAbsolutePath());
        }

        // run process
        Process process = b.start();
        int retval = process.waitFor();

        // log output
        for (String s : ((List<String>) org.apache.commons.io.IOUtils.readLines(process.getInputStream()))) {
            getLog().debug(s);
        }

        // check exit code
        if (retval != 0) {
            System.err.println("\n\n");
            org.apache.commons.io.IOUtils.copy(process.getInputStream(), System.out);
            org.apache.commons.io.IOUtils.copy(process.getErrorStream(), System.err);
            System.err.println("\n\n");
            throw new MojoExecutionException(tool.name() + " execution failed!");
        }

    }

    private void processTranslations() throws MojoExecutionException {

        try {

            // ts files will be in src/main/resources/translations
            if (translationsDir == null) {
                translationsDir = new File(this.baseDir, "src/main/resources/translations");
            }


            if (!translationsDir.isDirectory()) {
                getLog().info("No translations found in " + translationsDir.getAbsolutePath());
                return;
            }
            Process process;
            int retval;

            // get list of sources and forms
            //ArrayList<File> lst = new ArrayList<File>();
            //findFiles(sourcesDir, lst, dotJava);
            //findFiles(sourcesDir, lst, dotJava);

            // for all ts files
            for (File ts : translationsDir.listFiles()) {

                if (ts.getName().endsWith(dotTs)) {

                    // if file is zero-length, delete it before calling lupdate
                    if (ts.isFile() && ts.length() == 0) {
                        ts.delete();
                    }

                    getLog().debug("Updating translation " + ts.getAbsolutePath());

                    // [-noobsolete] -extensions java %sourcedir %dstdir -ts test.ts
                    String[] args = new String[]{
                        noObsoleteTranslations ? "-noobsolete" : null,
                        "-extensions",
                        dotJava.replace(".", ""),
                        sourcesDir.getAbsolutePath(),
                        destinationDir.getAbsolutePath(),
                        "-ts",
                        ts.getAbsolutePath()
                    };

                    getLog().debug("Executing: lupdate " + java.util.Arrays.toString(args));


                    executeTool(Tools.LUPDATE, args);
                }
            }

        } catch (Exception ex) {
            throw new MojoExecutionException("Translation failed", ex);
        }


        /*

        // ts will reside in the same dir where pm
        File ts = new File(qmDirectory.getAbsolutePath(), "app.ts");
        File qm = new File(qmDirectory.getAbsolutePath(), "app.qm");


        try {

        List<String> javaFiles = new LinkedList<String>();
        findFiles(sourceDirectory, javaFiles, ".java");

        List<String> cmd = new ArrayList<String>();
        cmd.add(lupdatePath.getAbsolutePath());

        for (String f : javaFiles) {
        cmd.add(f);
        }

        cmd.add("-ts");
        cmd.add(ts.getAbsolutePath());

        getLog().debug(cmd.toString());

        Runtime.getRuntime().exec(cmd.toArray(new String[0]));



        cmd.clear();

        cmd.add(lreleasePath.getAbsolutePath());
        cmd.add(ts.getAbsolutePath());
        cmd.add("-qm");
        cmd.add(qm.getAbsolutePath());

        getLog().debug(cmd.toString());

        Runtime.getRuntime().exec(cmd.toArray(new String[0]));


        } catch (IOException ex) {
        Logger.getLogger(JUICMojo.class.getName()).log(Level.SEVERE, null, ex);
        throw new MojoExecutionException("Error has occured", ex);
        }
         */
    }

    private File saveResource(String resourceName, File outputDir) throws MojoExecutionException {

        InputStream in = QtJambiMOJO.class.getResourceAsStream("/" + resourceName);

        if (in == null) {
            throw new MojoExecutionException("Can't find resource named '" + resourceName + "'");
        }
        File outfile = new File(outputDir, resourceName);

        outfile.deleteOnExit();
        try {

            FileOutputStream out = new FileOutputStream(outfile);
            org.apache.commons.io.IOUtils.copy(in, out);

            out.close();
            in.close();

            return outfile;

        } catch (IOException ex) {
            throw new MojoExecutionException("Can't write to " + outfile.getAbsolutePath());
        }
    }
}


