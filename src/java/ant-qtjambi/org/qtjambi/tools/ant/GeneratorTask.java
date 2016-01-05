/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
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
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
** $END_LICENSE$

**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

package org.qtjambi.tools.ant;

import org.apache.tools.ant.BuildException;
import org.apache.tools.ant.Project;
import org.apache.tools.ant.Task;
import org.apache.tools.ant.PropertyHelper;

import java.io.File;
import java.util.ArrayList;
import java.util.List;
import java.util.StringTokenizer;

import org.qtjambi.qt.osinfo.OSInfo;

public class GeneratorTask extends Task {
    private String header;
    private String typesystem;
    private String inputDirectory;
    private String outputDirectory;
    private String cppOutputDirectory;
    private String javaOutputDirectory;
    private String inputPreprocessFile;
    private String outputPreprocessFile;
    private String dir;
    private String phononpath;
    private String kdephonon;
    private String options;
    private String qtIncludeDirectory;
    private String qtLibDirectory;
    private String jambiDirectory;
    private String generatorDirectory;
    private String generatorExe;
    private String includePaths;
    private boolean debugTools;
    private List<String> commandList = new ArrayList<String>();

    private List<String> searchPath() {
        List<String> pathList = new ArrayList<String>();

		boolean generator_debug = "debug".equals(getProject().getProperty("generator.configuration"));
		
        if(generatorDirectory != null) {
            File dirGeneratorDirectory = new File(generatorDirectory);
			if(dirGeneratorDirectory.isDirectory()) {
                pathList.add(dirGeneratorDirectory.getAbsolutePath());

                File dir1 = new File(generatorDirectory, generator_debug ? "debug" : "release");
                if(dir1.isDirectory())
                    pathList.add(dir1.getAbsolutePath());

                File dir2 = new File(generatorDirectory, generator_debug ? "release" : "debug");
                if(dir2.isDirectory())
                    pathList.add(dir2.getAbsolutePath());
            }
        }

        if(jambiDirectory != null) {
            File dirJambiDirectory = new File(jambiDirectory);
            if(dirJambiDirectory.isDirectory()) {
                // This is setup at the top of build.xml (of QtJambi project)
                String propGeneratorBuilddir = getProject().getProperty("generator.builddir");
                if(propGeneratorBuilddir != null) {
                    File dirGeneratorDirectory = new File(jambiDirectory, propGeneratorBuilddir);
                    if(dirGeneratorDirectory.isDirectory()) {
                        pathList.add(dirGeneratorDirectory.getAbsolutePath());

                        File dir1 = new File(jambiDirectory, generator_debug ? "debug" : "release");
                        if(dir1.isDirectory())
                            pathList.add(dir1.getAbsolutePath());

                        File dir2 = new File(jambiDirectory, generator_debug ? "release" : "debug");
                        if(dir2.isDirectory())
                            pathList.add(dir2.getAbsolutePath());
                    }
                }
            }
        }

        return pathList;
    }

    private String generatorExecutable() {
        if(generatorExe != null) {
            File fileExe = new File(generatorExe);
            if(fileExe.isFile() /*&& fileExe.isExecutable()*/)
                return fileExe.getAbsolutePath();
            if(OSInfo.os() == OSInfo.OS.Windows) {
                fileExe = new File(generatorExe + ".exe");
                if(fileExe.isFile() /*&& fileExe.isExecutable()*/)
                    return fileExe.getAbsolutePath();
            }
        }

        String exe;
        switch(OSInfo.os()) {
        case Windows:
            exe = "generator.exe";
            break;
        default:
            exe = "generator";
            break;
        }

        return Util.LOCATE_EXEC(exe, searchPath(), null).getAbsolutePath();
    }

    public void setOptions(String options) {
        this.options = options;
    }
    public String getOptions() {
        return options;
    }

    private void parseArgumentFiles(List<String> commandList) {
        File typesystemFile = Util.makeCanonical(typesystem);
        if(typesystemFile == null || !typesystemFile.exists())
            throw new BuildException("Typesystem file '" + typesystem + "' does not exist.");

        File headerFile;
        if(kdephonon == null || kdephonon.length() == 0)
            headerFile = Util.makeCanonical(header);
        else
            headerFile = Util.makeCanonical(kdephonon);

        if(headerFile == null || !headerFile.exists())
            throw new BuildException("Header file '" + headerFile.getAbsolutePath() + "' does not exist.");

        commandList.add(headerFile.getAbsolutePath());
        commandList.add(typesystemFile.getAbsolutePath());
    }

    private boolean parseArguments() {
        if(options != null && options.length() > 0) {
            List<String> optionArgsList = Util.safeSplitStringTokenizer(options);
            for(String s : optionArgsList) {
                if(s != null && s.length() > 0)
                    commandList.add(s);
            }
        }

        if(includePaths != null){
        	// replace path separator since linux/mac does not accept semicolon
        	commandList.add("--include-paths=" + includePaths.replace(";", File.pathSeparator));
        }

        if(phononpath != null && phononpath.length() > 0){
        	// replace path separator since linux/mac does not accept semicolon
        	commandList.add("--phonon-include=" + phononpath.replace(";", File.pathSeparator));
        }

        if(qtIncludeDirectory != null){
        	// replace path separator since linux/mac does not accept semicolon
        	commandList.add("--qt-include-directory=" + qtIncludeDirectory.replace(";", File.pathSeparator));
        }

        // --input-directory: Don't test the value exists, since it might be a pathSeparator
        // spec, or just test each part and warn (not fail) when something does not exist
        handleArgumentDirectory(inputDirectory, "--input-directory", "Input directory", false);
        handleArgumentDirectory(outputDirectory, "--output-directory", "Output directory", true);
        handleArgumentDirectory(cppOutputDirectory, "--cpp-output-directory", "CPP output directory", true);
        handleArgumentDirectory(javaOutputDirectory, "--java-output-directory", "Java output directory", true);

        if(inputPreprocessFile != null)
            commandList.add("--input-preprocess-file=" + inputPreprocessFile);

        if(outputPreprocessFile != null)
            commandList.add("--output-preprocess-file=" + outputPreprocessFile);

        if(debugTools)
            commandList.add("--qtjambi-debug-tools");


        PropertyHelper props = PropertyHelper.getPropertyHelper(getProject());

        String javaOsarchTarget = AntUtil.getPropertyAsString(props, Constants.JAVA_OSCPU_TARGET);
        if("arm".equals(javaOsarchTarget))
            commandList.add("--target-platform-arm-cpu");

        Object o;
        o = AntUtil.getProperty(props, Constants.GENERATOR_PREPROC_STAGE1);
        handlePreprocArgument(o, "--preproc-stage1");

        o = AntUtil.getProperty(props, Constants.GENERATOR_PREPROC_STAGE2);
        handlePreprocArgument(o, "--preproc-stage2");

        parseArgumentFiles(commandList);

        return true;
    }


    /**
     * Helper for parseArguments().
     *
     * @see parseArguments()
     */
    private void handleArgumentDirectory(String directory, String argumentName, String name, boolean mustExistIfSpecified) {
        if(directory == null || directory.length() <= 0)
            return;

        // The 'directory' maybe a File.pathSeparator delimited list of directories and on 
        //  windows both : and ; can appear which makeCanonical() doesn't like.
        StringBuilder sb = new StringBuilder();
        String[] directoryElementA = directory.split(File.pathSeparator);
        for(String directoryElement : directoryElementA) {
            File file = Util.makeCanonical(directoryElement);
            if(mustExistIfSpecified && !file.exists())
                throw new BuildException(name + " '" + directoryElement + "' does not exist.");
            if(sb.length() > 0)
                sb.append(File.pathSeparator);
            sb.append(file.getAbsolutePath());
        }

        commandList.add(argumentName + "=" + sb.toString());
    }

    /**
     * Helper for parseArguments().
     *
     * @see parseArguments()
     */
    private void handlePreprocArgument(Object o, String argument) {
        if(o != null) {
            if(o instanceof String[]) {
                String[] sA = (String[]) o;
                commandList.add(argument);
                for(String s : sA)
                    commandList.add(s);
            } else {
                StringTokenizer st = new StringTokenizer(o.toString(), ",");
                commandList.add(argument);
                while(st.hasMoreTokens())
                    commandList.add(st.nextToken());
            }
        }
    }

    @Override
    public void execute() throws BuildException {
        parseArguments();

        String generator = generatorExecutable();

        List<String> thisCommandList = new ArrayList<String>();
        thisCommandList.add(generator);
        thisCommandList.addAll(commandList);
        getProject().log(this, "Arguments: " + thisCommandList.toString(), Project.MSG_INFO);

        File dirExecute = null;
        if(dir != null)
            dirExecute = new File(dir);
        String qtBinDirectory = null;
        if(OSInfo.isWindows()){
        	qtBinDirectory = qtLibDirectory;
        }
        Exec.execute(thisCommandList, dirExecute, getProject(), qtBinDirectory, qtLibDirectory);
    }

    public void setHeader(String header) {
        this.header = header;
    }

    public void setTypesystem(String typesystem) {
        this.typesystem = typesystem;
    }

    public void setPhononpath(String phononpath) {
        this.phononpath = phononpath;
    }

    public void setKdephonon(String kdephonon) {
        this.kdephonon = kdephonon;
    }

    public void setIncludePaths(String includePaths) {
        // HACK - We need (recursive) expansion of ${properties} this appears to do the trick
        PropertyHelper props = PropertyHelper.getPropertyHelper(getProject());
        String x = props.replaceProperties(null, includePaths, null);
        this.includePaths = x;
    }

    public void setJambiDirectory(String jambiDirectory) {
        this.jambiDirectory = jambiDirectory;
    }

    public void setGeneratorDirectory(String generatorDirectory) {
        this.generatorDirectory = generatorDirectory;
    }

    public void setQtIncludeDirectory(String dir) {
        this.qtIncludeDirectory = dir;
    }

    /**
     * Used for LD_LIBRARY_PATH assurance only.
     */
    public void setQtLibDirectory(String dir) {
        this.qtLibDirectory = dir;
    }

    public void setInputDirectory(String inputDirectory) {
        this.inputDirectory = inputDirectory;
    }

    public void setOutputDirectory(String outputDirectory) {
        this.outputDirectory = outputDirectory;
    }

    public void setCppOutputDirectory(String cppOutputDirectory) {
        this.cppOutputDirectory = cppOutputDirectory;
    }

    public void setJavaOutputDirectory(String javaOutputDirectory) {
        this.javaOutputDirectory = javaOutputDirectory;
    }

    public void setInputPreprocessFile(String inputPreprocessFile) {
        this.inputPreprocessFile = inputPreprocessFile;
    }

    public void setOutputPreprocessFile(String outputPreprocessFile) {
        this.outputPreprocessFile = outputPreprocessFile;
    }

    public void setDir(String dir) {
        this.dir = dir;
    }

    public void setGeneratorExe(String generatorExe) {
        this.generatorExe = generatorExe;
    }

    /**
     * Enable code generation with additional debugging output, this
     *  output may inhibit runtime performance.
     */
    public void setDebugTools(boolean debugTools) {
        this.debugTools = debugTools;
    }
}
