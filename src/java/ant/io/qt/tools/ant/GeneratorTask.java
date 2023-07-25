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
import java.util.ArrayList;
import java.util.List;
import java.util.StringTokenizer;

import org.apache.tools.ant.BuildException;
import org.apache.tools.ant.PropertyHelper;
import org.apache.tools.ant.Task;

public class GeneratorTask extends Task {
    private String header;
    private String typesystem;
    private String importdirectories;
    private String typesystemDirectories;
	private String outputDirectory;
    private String cppOutputDirectory;
    private String javaOutputDirectory;
    private String outputPreprocessFile;
    private String dir;
    private String options;
    private String qtDocDirectory;
    private String qtDocUrl;
	private String qtLibDirectory;
    private String qtBinDirectory;
    private String generatorDirectory;
    private String generatorExe;
    private String includeDirectories;
	private String qtjambiVersion;
	private String nullness;
	private String kotlinPropertyDelegates;
    private List<String> commandList = new ArrayList<String>();

    private List<String> searchPath(boolean generator_debug) {
        List<String> pathList = new ArrayList<String>();
		
        if(generatorDirectory != null) {
            File dirGeneratorDirectory = new File(generatorDirectory);
			if(dirGeneratorDirectory.isDirectory()) {
                pathList.add(dirGeneratorDirectory.getAbsolutePath());
                File dir1 = new File(generatorDirectory, "bin");
                if(dir1.isDirectory())
                    pathList.add(dir1.getAbsolutePath());
            }
        }

        return pathList;
    }

    private String generatorExecutable() {
    	boolean generator_debug = "true".equals(getProject().getProperty("generator.debug.run"));
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
            exe = generator_debug ? "QtJambiGeneratord.exe" : "QtJambiGenerator.exe";
            break;
        default:
            exe = generator_debug ? "QtJambiGenerator_debug" : "QtJambiGenerator";
            break;
        }

        return Util.LOCATE_EXEC(exe, searchPath(generator_debug), null).getAbsolutePath();
    }

    public void setOptions(String options) {
        this.options = options;
    }
    public String getOptions() {
        return options;
    }

    private boolean parseArguments() {
        if(options != null && options.length() > 0) {
            List<String> optionArgsList = Util.safeSplitStringTokenizer(options);
            for(String s : optionArgsList) {
                if(s != null && s.length() > 0)
                    commandList.add(s);
            }
        }

        if(includeDirectories != null){
        	// replace path separator since linux/mac does not accept semicolon
        	commandList.add("--include-directories=" + includeDirectories.replace(";", File.pathSeparator));
        }
        
        if("false".equalsIgnoreCase(nullness)) {
        	commandList.add("--no-nullness");
        }
        
        if("false".equalsIgnoreCase(kotlinPropertyDelegates)) {
        	commandList.add("--no-kotlin-property-delegates");
        }

        if(qtDocDirectory != null){
        	// replace path separator since linux/mac does not accept semicolon
        	commandList.add("--qt-doc-directory=" + qtDocDirectory);
        }
        
        if(qtDocUrl != null) {
			commandList.add("--qt-doc-url=" + qtDocUrl);
        }
        if(typesystemDirectories!=null) {
        	commandList.add("--typesystem-directories=" + typesystemDirectories);
        }

        // --input-directory: Don't test the value exists, since it might be a pathSeparator
        // spec, or just test each part and warn (not fail) when something does not exist
        handleArgumentDirectory(importdirectories, "--import-directories", "Input directory", false);
        handleArgumentDirectory(outputDirectory, "--output-directory", "Output directory", true);
        handleArgumentDirectory(cppOutputDirectory, "--cpp-output-directory", "CPP output directory", true);
        handleArgumentDirectory(javaOutputDirectory, "--java-output-directory", "Java output directory", true);

        if(outputPreprocessFile != null)
            commandList.add("--output-preprocess-file=" + outputPreprocessFile);
        
        if(qtjambiVersion!=null)
        	commandList.add("--qtjambi-version=" + qtjambiVersion);

        PropertyHelper props = PropertyHelper.getPropertyHelper(getProject());

        Object o;
		o = AntUtil.getProperty(props, Constants.GENERATOR_PREPROC_DEFINES);
        handlePreprocArgument(o);
		o = AntUtil.getProperty(props, Constants.GENERATOR_STATICLIBS);
		if(o instanceof String && !o.toString().isEmpty())
			commandList.add("--static="+o);
		o = AntUtil.getProperty(props, Constants.GENERATOR_PREPROC_FILE);
		if(o instanceof String && !o.toString().isEmpty())
			commandList.add("--output-preprocess-file="+o);

        if(typesystem!=null && !typesystem.isEmpty()) {
        	if(typesystem.startsWith(":")) {
        		commandList.add("--typesystem="+typesystem);
        	}else {
		        File typesystemFile = Util.makeCanonical(typesystem);
		        if(typesystemFile == null || !typesystemFile.exists())
		            throw new BuildException("Typesystem file '" + typesystem + "' does not exist.");
		        commandList.add("--typesystem="+typesystemFile.getAbsolutePath());
        	}
        }

        if(header!=null && !header.isEmpty()) {
        	if(header.startsWith(":")) {
        		commandList.add(header);
        	}else {
	        	File headerFile = Util.makeCanonical(header);
		        if(headerFile == null || !headerFile.exists())
		            throw new BuildException("Header file '" + headerFile.getAbsolutePath() + "' does not exist.");
		        commandList.add(headerFile.getAbsolutePath());
        	}
        }

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
    private void handlePreprocArgument(Object o) {
        if(o != null) {
            if(o instanceof String[]) {
                String[] sA = (String[]) o;
                for(String s : sA)
                    commandList.add("-D"+s);
            } else {
                StringTokenizer st = new StringTokenizer(o.toString(), ",");
                while(st.hasMoreTokens())
                    commandList.add("-D"+st.nextToken());
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

        File dirExecute = null;
        if(dir != null)
            dirExecute = new File(dir);
        Exec.execute(this, thisCommandList, dirExecute, getProject(), qtBinDirectory, qtLibDirectory, new File(outputDirectory+"/generator.out.txt"), new File(outputDirectory+"/generator.err.txt"));
    }

    public void setHeader(String header) {
        this.header = header;
    }

    public void setTypesystem(String typesystem) {
        this.typesystem = typesystem;
    }

    public void setIncludeDirectories(String includePaths) {
        // HACK - We need (recursive) expansion of ${properties} this appears to do the trick
        PropertyHelper props = PropertyHelper.getPropertyHelper(getProject());
        String x = props.replaceProperties(null, includePaths, null);
        this.includeDirectories = x;
    }

    public void setGeneratorDirectory(String generatorDirectory) {
        this.generatorDirectory = generatorDirectory;
    }

    public void setQtDocDirectory(String dir) {
        this.qtDocDirectory = dir;
    }

	public void setQtDocUrl(String qtDocUrl) {
		this.qtDocUrl = qtDocUrl;
	}
	
    /**
     * Used for LD_LIBRARY_PATH assurance only.
     */
    public void setQtLibDirectory(String dir) {
        this.qtLibDirectory = dir;
    }
    
    public void setQtBinDirectory(String dir) {
        this.qtBinDirectory = dir;
    }

    public void setImportDirectories(String importdirectories) {
        this.importdirectories = importdirectories;
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

    public void setOutputPreprocessFile(String outputPreprocessFile) {
        this.outputPreprocessFile = outputPreprocessFile;
    }

    public void setDir(String dir) {
        this.dir = dir;
    }

    public void setGeneratorExe(String generatorExe) {
        this.generatorExe = generatorExe;
    }
	
    public String getTypesystemsDirectory() {
		return typesystemDirectories;
	}

	public void setTypesystemsDirectory(String typesystemsDirectory) {
		this.typesystemDirectories = typesystemsDirectory;
	}

	public String getQtjambiVersion() {
		return qtjambiVersion;
	}

	public void setQtjambiVersion(String qtjambiVersion) {
		this.qtjambiVersion = qtjambiVersion;
	}

	public String getNullness() {
		return nullness;
	}

	public void setNullness(String nullness) {
		this.nullness = nullness;
	}

	public String getKotlinPropertyDelegates() {
		return kotlinPropertyDelegates;
	}

	public void setKotlinPropertyDelegates(String kotlinPropertyDelegates) {
		this.kotlinPropertyDelegates = kotlinPropertyDelegates;
	}
}
