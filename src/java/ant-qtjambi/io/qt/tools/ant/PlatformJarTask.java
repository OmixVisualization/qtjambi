/****************************************************************************
**
** Copyright (C) 2009-2022 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintStream;
import java.io.PrintWriter;
import java.io.StringReader;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.AbstractMap.SimpleEntry;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.TreeSet;

import org.apache.tools.ant.BuildException;
import org.apache.tools.ant.Project;
import org.apache.tools.ant.PropertyHelper;
import org.apache.tools.ant.Task;

public class PlatformJarTask extends Task {

    public static final String SYSLIB_AUTO = "auto";
    public static final String SYSLIB_NONE = "none";

    private String cacheKey = "default";
    private File outdir;
    private List<LibraryEntry> libs         = new ArrayList<LibraryEntry>();
    private Set<String> libraryDir          = new HashSet<String>();
    private List<String> unpackLibs         = new ArrayList<String>();
    private List<String> runtimeLibs        = new ArrayList<String>();
    private String systemLibs               = SYSLIB_AUTO;
    private List<PluginPath> pluginPaths    = new ArrayList<PluginPath>();
    private List<PluginDesignerPath> pluginDesignerPaths = new ArrayList<PluginDesignerPath>();
    private List<Directory> directoryList   = new ArrayList<Directory>();
    
    private List<String> additionalFiles    = new ArrayList<>();
    private List<Map.Entry<String,String>> symlinks = new ArrayList<>();
    private Boolean debug                   = null;
    private String javaLibDir               = "";

    private boolean rpath = true;
    private String execStrip;

    private PropertyHelper propertyHelper;
    private FindCompiler.Compiler compiler;

    public void addConfiguredLibrary(LibraryEntry task) {
        try {
        	if(compiler==null) {
        		if(propertyHelper==null) {
        			propertyHelper = PropertyHelper.getPropertyHelper(getProject());
        		}
        		compiler = FindCompiler.Compiler.resolve(AntUtil.getPropertyAsString(propertyHelper, Constants.COMPILER));
        	}
        	task.setCompiler(compiler);
            task.setDebug(debug);
            if(task.isIncluded()){
                task.perform();
                libs.add(task);
            }
        } catch(Exception e) {
            e.printStackTrace();
            throw new BuildException("Failed to add library entry.....");
        }
    }

    public void addConfiguredPlugin(PluginPath path) {
        try {
            path.setDebug(debug);
            path.perform();
            pluginPaths.add(path);
        } catch(Exception e) {
            e.printStackTrace();
            throw new BuildException("Failed to add plugin path.......");
        }
    }

    public void addConfigured(PluginDesignerPath path) {
        try {
            path.setDebug(debug);
            path.perform();
            pluginDesignerPaths.add(path);
        } catch(Exception e) {
            e.printStackTrace();
            throw new BuildException("Failed to add plugin-designer path.......");
        }
    }

    public void addConfigured(Directory directory) {
        try {
            if(directory.isIncluded(debug)){
                directory.perform();
                directoryList.add(directory);
            }
        } catch(Exception e) {
            e.printStackTrace();
            throw new BuildException("Failed to add directory sub-element.......");
        }
    }

    public void setSyslibs(String s) {
        if(s.equals(SYSLIB_NONE) || s.equals(SYSLIB_AUTO))
            systemLibs = s;
        else
            throw new BuildException("Bad 'syslibs' parameter... Only 'auto' or 'none' available, was " + s);
    }
    public String getSyslibs() {
        return systemLibs;
    }

    public String getCacheKey() {
        return cacheKey;
    }
    public void setCacheKey(String cacheKey) {
        this.cacheKey = cacheKey;
    }

    public File getOutdir() {
        return outdir;
    }
    public void setOutdir(File outdir) {
        this.outdir = outdir;
    }

    public Boolean getDebug() {
        return debug;
    }
    public void setDebug(Boolean debug) {
        this.debug = debug;
    }

    public void setRpathenabled(boolean iname) {
        rpath = iname;
    }
    public boolean getRpathenabled() {
        return rpath;
    }

    public void execute() throws BuildException {
        try {
            execute_internal();
        } catch(Exception e) {
            e.printStackTrace();
            throw new BuildException("Failed to create native .jar", e);
        }
    }

    public void execute_internal() throws BuildException {
    	if(propertyHelper==null) {
			propertyHelper = PropertyHelper.getPropertyHelper(getProject());
		}
        javaLibDir = AntUtil.getPropertyAsString(propertyHelper, Constants.JAVALIBDIR);

        if(debug == null) {
            if(Constants.CONFIG_DEBUG.equals(AntUtil.getPropertyAsString(propertyHelper, Constants.CONFIGURATION)))
                debug = Boolean.TRUE;
            else
                debug = Boolean.FALSE;
        }

        execStrip = AntUtil.getPropertyAsString(propertyHelper, Constants.EXEC_STRIP);

        if(outdir == null) {
            throw new BuildException("Missing required attribute 'outdir'. " +
                    "This directory is used for building the .jar file...");
        }

        if(outdir.exists()) {
            outdir.delete();
        }

        outdir.mkdirs();

        Iterator<Directory> it = directoryList.iterator();
        while(it.hasNext()) {
            Directory d = it.next();
            if(processDirectory(d) == false)
                it.remove();    // was found to be inhibited
        }
        for(LibraryEntry e : libs) {
            processLibraryEntry(e);
        }

        if(systemLibs.equals(SYSLIB_AUTO))
            processSystemLibs();

        if(rpath) {
            if(OSInfo.os() == OSInfo.OS.MacOS) {
                processOSXInstallName();
            }
            else if (OSInfo.os() == OSInfo.OS.Linux)
            {
                processRPath();
            }
        }

        writeQtJambiDeployment();
    }

    // Not sure we needed pathFragment, since it is only used by toplevel and it is always null for that.  Ah yes for indent now.
    private void writeDirectoryElementRecurse(PrintWriter writer, Directory root, String pathFragment, Dirent dirent) {
        if(dirent == null)
            return;

        int indentCount = 0;
        {   // Pretty XML
            if(pathFragment != null) {
                String[] pfA = pathFragment.split("/");
                if(pfA != null)
                    indentCount += pfA.length;
            }
        }
        for(int i = indentCount; i > 0; i--)
            writer.print("  ");
        if(dirent.isDirectory()) {
        	if(dirent.getName().endsWith(".dylib.dSYM") && Boolean.FALSE.equals(debug))
        		return;
            if(root == dirent)  // Top  level include full path from getDestSubDir()
                writer.println("  <directory name=\"" + xmlEscape(Util.pathCanon(new String[] { root.getDestSubdir(), pathFragment, dirent.getName() }, "/")) + "\">");
            else
                writer.println("  <directory name=\"" + xmlEscape(Util.pathCanon(new String[] { dirent.getName() }, "/")) + "\">");
        } else {
            writer.println("  <file name=\"" + xmlEscape(Util.pathCanon(new String[] { dirent.getName() }, "/")) + "\"/>");
        }

        List<Dirent> list = dirent.getChildList();
        if(list != null) {
            String subPathFragment = pathFragment;
            if(subPathFragment != null && subPathFragment.length() > 0)
                subPathFragment += "/";
            if(subPathFragment == null)
                subPathFragment = "";   // stops += appending "null" first
            subPathFragment += dirent.getName();

            for(Dirent d : list)
                writeDirectoryElementRecurse(writer, root, subPathFragment, d);
        }

        if(dirent.isDirectory()) {
            for(int i = indentCount; i > 0; i--)
                writer.print("  ");
            writer.println("  </directory>");
        }
    }

    private void writeDirectoryElementTree(PrintWriter writer, List<Directory> list) {
        if(list.size() > 0) {
            writer.println();
            writer.println("  <!-- Directory -->");
        }
        for(Directory root : list)
            writeDirectoryElementRecurse(writer, root, null, root);
    }

    private void writeQtJambiDeployment() {
        PrintWriter writer;
        try {
            writer = new PrintWriter(new BufferedWriter(
                    new FileWriter(new File(outdir, "qtjambi-deployment.xml"))));
        } catch(IOException e) {
            e.printStackTrace();
            throw new BuildException("Failed to open 'qtjambi-deployment.xml' for writing in '" + outdir + "'");
        }

        writer.println("<qtjambi-deploy system=\""
                            + xmlEscape(AntUtil.getPropertyAsString(propertyHelper, Constants.OSNAME).toString()) + "\"");
        writer.println("                version=\"" + xmlEscape(AntUtil.getPropertyAsString(propertyHelper, "qtjambi.version").toString()) + "\"");
        writer.println("                compiler=\"" + xmlEscape(AntUtil.getPropertyAsString(propertyHelper, "qtjambi.compiler").toString()) + "\"");
        if(Boolean.TRUE.equals(this.debug)) {
        	writer.println("                configuration=\"debug\"");
        }else {
        	writer.println("                configuration=\"release\"");
        }
        writer.println("                date=\"" + xmlEscape(AntUtil.getPropertyAsString(propertyHelper, "DSTAMP").toString()) + "\"");
        writer.println("                time=\"" + xmlEscape(AntUtil.getPropertyAsString(propertyHelper, "TSTAMP").toString()) + "\">");
        writeDirectoryElementTree(writer, directoryList);

        // system libraries that must be loaded first of all...
        if(systemLibs.equals(SYSLIB_AUTO)) {
            if(runtimeLibs.size() > 0) {
                writer.println();
                writer.println("  <!-- Runtime libraries, loaded automatically -->");
            }
            for(String rt : runtimeLibs) {
                writer.println("  <syslibrary name=\"" + xmlEscape(rt) + "\"/>");
            }
        }

        writer.println();
        writer.println("  <!-- Qt libraries -->");
        for(LibraryEntry e : libs) {
        	if(e.isPacked()) {
	        	if(e.getAbsolutePath()==null && e.getType().equals(LibraryEntry.TYPE_FILESET)) {
	        		File rootPath = e.getRootPath();
	                if(rootPath == null)
	                    rootPath = new File(".");
	                String subdir = e.getSubdir();
	                File srcDir;
	                if(subdir==null || subdir.isEmpty()){
	                    srcDir = rootPath;        	
	                }else{
	                	srcDir = new File(rootPath, subdir.replace("/", File.separator));
	                }
	                
	                for(String strg : srcDir.list()){
	            		if(strg.matches(e.getName())){
	        	            String destSubdir = e.getDestSubdir();
	        	            writer.println("  <library name=\"" + xmlEscape(Util.pathCanon(new String[] { destSubdir, subdir, strg }, "/")) + "\"/>");
	            		}
	            	}
	        	}else {
		            String resolvedName = e.getResolvedName();
		            String subdir = e.getSubdir();
		            String destSubdir = e.getDestSubdir();
		
		            String path = Util.pathCanon(new String[] { destSubdir, subdir, resolvedName }, "/");
		            writer.println("  <library name=\"" + xmlEscape(path) + "\"/>");
		            if(e.getType().equals(LibraryEntry.TYPE_QT)) {
		            	additionalFiles.remove(path);
		            }
	        	}
        	}
        }

        // Manifests and the like...
        if(systemLibs.equals(SYSLIB_AUTO)) {
            if(unpackLibs.size() > 0) {
                writer.println();
                writer.println("  <!-- Dependency libraries, not loaded... -->");
            }
            for(String unpack : unpackLibs) {
                writer.println("  <library name=\"" + xmlEscape(unpack) + "\"/>");
            }
        }
        
        for(String file : additionalFiles) {
            writer.println("  <library name=\"" + xmlEscape(file) + "\"/>");
        }
        
        for(Map.Entry<String,String> unpack : symlinks) {
            writer.println("  <symlink name=\"" + xmlEscape(unpack.getKey()) + "\" target=\"" + xmlEscape(unpack.getValue()) + "\"/>");
        }

        // plugins...
        if(pluginPaths.size() > 0) {
            writer.println();
            writer.println("  <!-- Plugins... -->");
            for(PluginPath p : pluginPaths) {
                writer.println("  <plugin path=\"" + xmlEscape(p.getPath()) + "\"/>");
            }
        }
        // designer plugins...
        if(pluginDesignerPaths.size() > 0) {
            writer.println();
            writer.println("  <!-- Designer Plugins... -->");
            for(PluginDesignerPath p : pluginDesignerPaths) {
                writer.println("  <plugin-designer path=\"" + xmlEscape(p.getPath()) + "\"/>");
            }
        }

        writer.println();
        writer.println("</qtjambi-deploy>");

        writer.close();
    }

    // This copies one whole directory but does not recurse
    private List<String> processDirectoryOne(File srcDir, File destDir, Set<String> skipSet) throws IOException {
        List<String> dirNameList = new ArrayList<String>();
        // find files (ignore those specified in childList)
        File[] fileA = srcDir.listFiles();
        for(File f : fileA) {
            String name = f.getName();
            if(skipSet != null && skipSet.contains(name))
                continue;
            File thisSrcFile = new File(srcDir, name);
            File thisDestFile = new File(destDir, name);
            if(f.isDirectory()){
                if(!f.getName().endsWith(".dylib.dSYM")){
                    dirNameList.add(name);
                }
            }else{
                if(Boolean.TRUE.equals(debug)){
                    if(name.endsWith(".dll")){
                        String _name = name.substring(0, name.length()-4)+"d.dll";
                        File _thisSrcFile = new File(srcDir, _name);
                        if(_thisSrcFile.isFile()){
                            continue;
                        }
                    }
                    if(name.endsWith(".pdb")){
                        String _name = name.substring(0, name.length()-4)+"d.pdb";
                        File _thisSrcFile = new File(srcDir, _name);
                        if(_thisSrcFile.isFile()){
                            continue;
                        }
                    }
                }else{
                    if(name.endsWith(".pdb")){
                        continue;
                    }
                    if(name.endsWith("d.dll")){
                        String _name = name.substring(0, name.length()-5)+".dll";
                        File _thisSrcFile = new File(srcDir, _name);
                        if(_thisSrcFile.isFile()){
                            continue;
                        }
                    }
                    if(name.endsWith("_debug.dylib")){
                        continue;
                    }
                }
                Util.copy(thisSrcFile, thisDestFile);
            }
        }
        for(String name : dirNameList) {
            File thisDestFile = new File(destDir, name);
            if(thisDestFile.exists() == false)
                thisDestFile.mkdir();
        }
        return dirNameList;
    }

    // recurseDepth = -1 (infinite), 0 only files of given directory, 1 recurse once
    private void processDirectoryInternal(File srcDir, File destDir, int recurseDepth, Set<String> skipSet) throws IOException {
        if(recurseDepth > 0)
            recurseDepth--;

        List<String> dirNameList = processDirectoryOne(srcDir, destDir, skipSet);

        if(recurseDepth == 0)
            return;
        for(String name : dirNameList) {
            File thisSrcFile = new File(srcDir, name);
            File thisDestFile = new File(destDir, name);
            processDirectoryInternal(thisSrcFile, thisDestFile, recurseDepth, null);
        }
    }

    // recurseDepth = -1 (infinite), 0 only files of given directory, 1 recurse once
    private void processDirentInternal(File srcDir, File destDir, int recurseDepth, Directory parent) throws IOException {
        if(recurseDepth > 0)
            recurseDepth--;

        // FIXME: Manage Dirent parent and refactor main code below to use this
        List<String> dirNameList = processDirectoryOne(srcDir, destDir, null);

        if(recurseDepth == 0)
            return;
        for(String name : dirNameList) {
            File thisSrcFile = new File(srcDir, name);
            File thisDestFile = new File(destDir, name);
            processDirectoryInternal(thisSrcFile, thisDestFile, recurseDepth, null);
        }
    }

    private Directory buildDirentDirectory(Directory parent, String rootPath, String name) {
        String thisRootPath = rootPath + "/" + name;
        Directory direntDirectory = new Directory(thisRootPath, name);
        if(parent.getChild(name) != null)
            throw new RuntimeException("duplicate name=" + name);   // should never happen
        parent.getChildList().add(direntDirectory);
        return direntDirectory;
    }

    private io.qt.tools.ant.File buildDirentFile(Directory parent, String name) {
        io.qt.tools.ant.File direntFile = new io.qt.tools.ant.File(name);
        if(parent.getChild(name) != null)
            throw new RuntimeException("duplicate name=" + name);   // should never happen
        parent.getChildList().add(direntFile);
        return direntFile;
    }

    private void buildDirentDirectoryRecurse(Directory parent, File dirParent, String rootPath, String name) {
        Directory subDirectory = buildDirentDirectory(parent, rootPath, name);

        File[] subFileA = dirParent.listFiles();
        for(File f : subFileA) {
            if(f.isDirectory()) {
                File x = new File(dirParent, f.getName());
                buildDirentDirectoryRecurse(subDirectory, x, rootPath, f.getName());   // recursive
            } else {
                @SuppressWarnings("unused")
				io.qt.tools.ant.File direntFile = buildDirentFile(subDirectory, f.getName());
            }
        }
    }

    private boolean processDirectory(Directory d) {
        boolean rv = false;
        File rootPathFile = null;
        String rootPath = null;
        String toplevelName = null;
        String subdir = null;
        String destSubdir = null;
        String outputPath = null;
        boolean recursive = false;
        File srcDir = null;
        File destDir = null;
        File srcTarget = null;
        File destTarget = null;
        try {
            rootPath = d.getRootPath();
            toplevelName = d.getName();
            if(rootPath == null) {
                if(toplevelName == null || toplevelName.length() == 0)
                    throw new IllegalArgumentException("name must be set, when rootPath is not set; name=" + toplevelName);
                rootPathFile = new File(".");
            } else if(rootPath.length() == 0) {
                return rv;  // skip (and have caller remove)
            } else {
                rootPathFile = new File(rootPath);
            }
            if(toplevelName == null || toplevelName.length() == 0) {
                toplevelName = rootPathFile.getName();
                rootPathFile = new File(rootPathFile.getAbsolutePath()).getParentFile();  // converts relative to absolute
                rootPath = rootPathFile.getAbsolutePath();

                // FIXUP the top level so qtjambi-descriptor.xml is emitted correctly
                d.setRootPath(rootPath);
                d.setName(toplevelName);
            }
            //getProject().log(this, "   rootPath " + rootPath, Project.MSG_VERBOSE);
            //getProject().log(this, "       name " + toplevelName, Project.MSG_VERBOSE);
            subdir = d.getSubdir();
            destSubdir = d.getDestSubdir();
            recursive = d.getRecursive();

            if(destSubdir != null) {
                if(destSubdir.startsWith("/")) {   // no subdir
                    outputPath = Util.pathCanon(new String[] { destSubdir }, "/");
                } else {
                    outputPath = Util.pathCanon(new String[] { destSubdir, subdir }, "/");
                }
            } else {
                outputPath = Util.pathCanon(new String[] { subdir }, "/");
            }
            if(subdir != null)
                srcDir = new File(rootPathFile, subdir);
            else
                srcDir = rootPathFile;
            destDir = new File(outdir, outputPath);
            if(!destDir.exists()) {
                //getProject().log(this, "   mkdir " + destDir.getAbsolutePath(), Project.MSG_VERBOSE);
                destDir.mkdir();
            }
            srcTarget = new File(srcDir, toplevelName);
            destTarget = new File(destDir, toplevelName);
            if(srcTarget.isDirectory()) {
                if(!destTarget.exists()) {
                    //getProject().log(this, "   mkdir " + destTarget.getAbsolutePath(), Project.MSG_VERBOSE);
                    destTarget.mkdir();
                }

                List<Directory> direntList = new ArrayList<Directory>();
                List<String> filenameList = new ArrayList<String>();
                List<String> dirnameList = new ArrayList<String>();

                File[] fileA = srcTarget.listFiles();
                for(File f : fileA) {
                    // do files
                    String name = f.getName();
                    Dirent child = d.getChild(name);
                    if(child == null || child.isDirectory() != f.isDirectory()) {
                        File thisSrcFile = new File(srcTarget, name);
                        File thisDestFile = new File(destTarget, name);
                        if(thisSrcFile.isDirectory()) {
                            if(!name.endsWith(".dylib.dSYM") || Boolean.TRUE.equals(debug)){
                                if(!thisDestFile.exists()) {
                                    //getProject().log(this, "   mkdir " + thisDestFile.getAbsolutePath(), Project.MSG_VERBOSE);
                                    thisDestFile.mkdir();
                                }
                                if(recursive)
                                    dirnameList.add(name);
                            }
                        } else {
                            // don't copy pdbs and debug libs in release mode
                            if(Boolean.TRUE.equals(debug)){
                                if(name.endsWith(".dll")){
                                    String _name = name.substring(0, name.length()-4)+"d.dll";
                                    File _thisSrcFile = new File(srcTarget, _name);
                                    if(_thisSrcFile.isFile()){
                                        continue;
                                    }
                                }
                                if(name.endsWith(".pdb")){
                                    String _name = name.substring(0, name.length()-4)+"d.pdb";
                                    File _thisSrcFile = new File(srcTarget, _name);
                                    if(_thisSrcFile.isFile()){
                                        continue;
                                    }
                                }
                            }else{
                                if(name.endsWith(".pdb")){
                                    continue;
                                }
                                if(name.endsWith("d.dll")){
                                    String _name = name.substring(0, name.length()-5)+".dll";
                                    File _thisSrcFile = new File(srcTarget, _name);
                                    if(_thisSrcFile.isFile()){
                                        continue;
                                    }
                                }
                                if(name.endsWith("_debug.dylib")){
                                    continue;
                                }
                            }
                            //getProject().log(this, "Copying " + thisSrcFile + " to " + thisDestFile, Project.MSG_VERBOSE);
                            Util.copy(thisSrcFile, thisDestFile);
                            if(recursive)
                                filenameList.add(name);
                        }
                    } else {
                        if(child.isDirectory()) {
                            if(!child.getName().endsWith(".dylib.dSYM") || Boolean.TRUE.equals(debug)){
                                File thisDestFile = new File(destTarget, name);
                                if(thisDestFile.exists() == false) {
                                    //getProject().log(this, "   mkdir " + thisDestFile.getAbsolutePath(), Project.MSG_VERBOSE);
                                    thisDestFile.mkdir();
                                }
                                if(recursive)
                                    direntList.add((Directory)child);
                            }
                        } else {
                            File thisSrcFile = new File(srcTarget, name);
                            File thisDestFile = new File(destTarget, name);
                            if(Boolean.TRUE.equals(debug)){
                                if(name.endsWith(".dll")){
                                    String _name = name.substring(0, name.length()-4)+"d.dll";
                                    File _thisSrcFile = new File(srcTarget, _name);
                                    if(_thisSrcFile.isFile()){
                                        continue;
                                    }
                                }
                                if(name.endsWith(".pdb")){
                                    String _name = name.substring(0, name.length()-4)+"d.pdb";
                                    File _thisSrcFile = new File(srcTarget, _name);
                                    if(_thisSrcFile.isFile()){
                                        continue;
                                    }
                                }
                            }else{
                                if(name.endsWith(".pdb")){
                                    continue;
                                }
                                if(name.endsWith("d.dll")){
                                    String _name = name.substring(0, name.length()-5)+".dll";
                                    File _thisSrcFile = new File(srcTarget, _name);
                                    if(_thisSrcFile.isFile()){
                                        continue;
                                    }
                                }
                                if(name.endsWith("_debug.dylib")){
                                    continue;
                                }
                            }
                            //getProject().log(this, "Copying " + thisSrcFile + " to " + thisDestFile, Project.MSG_VERBOSE);
                            Util.copy(thisSrcFile, thisDestFile);
                        }
                    }
                }
                for(Directory directory : direntList) {
                    String name = directory.getName();
                    File thisSrcFile = new File(srcTarget, name);
                    File thisDestFile = new File(destTarget, name);
                    processDirentInternal(thisSrcFile, thisDestFile, -1, directory);
                }
                for(String name : dirnameList) {
                    File thisSrcFile = new File(srcTarget, name);
                    File thisDestFile = new File(destTarget, name);
                    processDirectoryInternal(thisSrcFile, thisDestFile, -1, null);
                    // We need to make a Dirent for all these nodes
                    buildDirentDirectoryRecurse(d, thisSrcFile, rootPath, name);
                }
                for(String name : filenameList) {
                    @SuppressWarnings("unused")
					io.qt.tools.ant.File direntFile = buildDirentFile(d, name);
                }
            } else {
                try {
                    //getProject().log(this, "Copying " + srcTarget + " to " + destTarget, Project.MSG_VERBOSE);
                    Util.copy(srcTarget, destTarget);
                } catch(IOException ex) {
                    ex.printStackTrace();
                    throw new BuildException("Failed to copy file '" + toplevelName + "'");
                }
            }
            rv = true;
        } catch(Exception ex) {
            StringBuilder sb = new StringBuilder("DIAGNOSTIC");
            if(rootPathFile != null)
                sb.append("; rootPathFile=" + rootPathFile.getAbsolutePath());
            if(toplevelName != null)
                sb.append("; toplevelName=" + toplevelName);
            if(subdir != null)
                sb.append("; subdir=" + subdir);
            if(destSubdir != null)
                sb.append("; destSubdir=" + destSubdir);
            if(outputPath != null)
                sb.append("; outputPath=" + outputPath);
            if(srcDir != null)
                sb.append("; srcDir=" + srcDir.getAbsolutePath());
            if(destDir != null)
                sb.append("; destDir=" + destDir.getAbsolutePath());
            if(srcTarget != null)
                sb.append("; srcTarget=" + srcTarget.getAbsolutePath());
            if(destTarget != null)
                sb.append("; destTarget=" + destTarget.getAbsolutePath());
            ex.printStackTrace();
            throw new BuildException(ex);
        }
        return rv;
    }

    private void runBinaryStrip(File file) {
        List<String> list = Util.splitStringTokenizer(execStrip);
        list.add(file.getAbsolutePath());
        String[] cmd = list.toArray(new String[list.size()]);
        getProject().log(this, "Stripping binary: " + Arrays.toString(cmd), Project.MSG_VERBOSE);
        Exec.exec(this, cmd, null, getProject(), false);
    }

    private void processLibraryEntry(LibraryEntry e) {
    	if(e.isPending()) {
    		e.setPacked();
    		return;
    	}
        File rootPath = null;
        String absolutePath = null;
        String resolvedName = null;
        String subdir = null;
        String destSubdir = null;
        String outputPath = null;
        File srcDir = null;
        File destDir = null;
        File srcFile = null;
        File destFile = null;
        
        rootPath = e.getRootPath();
        if(rootPath == null)
            rootPath = new File(".");
        subdir = e.getSubdir();
        if(subdir==null || subdir.isEmpty()){
            srcDir = rootPath;        	
        }else{
        	srcDir = new File(rootPath, subdir.replace("/", File.separator));
        }
        
        absolutePath = e.getAbsolutePath();
        
        List<String> names = Collections.singletonList(e.getName());
        
        if(absolutePath == null && e.getType().equals(LibraryEntry.TYPE_FILESET)){
        	names = new ArrayList<String>();
        	if(srcDir!=null && srcDir.isDirectory()) {
	        	for(String strg : srcDir.list()){
	        		if(strg.matches(e.getName())){
	        			names.add(strg);
	        		}
	        	}
        	}
        }

        for(String libraryName : names){
        	try {
        		if(!e.getType().equals(LibraryEntry.TYPE_FILESET)){
        			resolvedName = e.getResolvedName();
        		}else{
        			resolvedName = libraryName;
        		}
	            destSubdir = e.getDestSubdir();
	
	            if(destSubdir != null) {
	                if(destSubdir.startsWith("/")) {   // no subdir
	                    outputPath = Util.pathCanon(new String[] { destSubdir }, "/");
	                } else {
	                    outputPath = Util.pathCanon(new String[] { destSubdir, subdir }, "/");
	                }
	            } else {
	                outputPath = Util.pathCanon(new String[] { subdir }, "/");
	            }
	            destDir = new File(outdir, outputPath);
	            if(!destDir.exists()) {
	                //getProject().log(this, "   mkdir " + destDir.getAbsolutePath(), Project.MSG_VERBOSE);
	                destDir.mkdir();
	            }
	            if(absolutePath == null)
	                srcFile = new File(srcDir, libraryName);
	            else
	                srcFile = new File(absolutePath);
	            destFile = new File(destDir, resolvedName);
	            try {
	            	if(e.getType().equals(LibraryEntry.TYPE_QT) && e.getOriginalname().equals("QtCore")) {
	            		try(PrintStream out = new PrintStream(new File(destDir, "qt.conf"))){
	            			out.println("[Paths]");
	            			out.println("Prefix=..");
	            		}
	            		additionalFiles.add(outputPath.replace(File.separator, "/")+"/qt.conf");
	            	}
	                if(e.getType().equals(LibraryEntry.TYPE_QT)
	            		&& Boolean.valueOf(AntUtil.getPropertyAsString(propertyHelper, Constants.MAC_OS_USE_FRAMEWORK))
	            		&& !Boolean.valueOf(AntUtil.getPropertyAsString(propertyHelper, Constants.MAC_OS_CONVERT_QT_FRAMEWORK))) {
	                	copySubdirs(srcDir.getParentFile(), new File(srcDir, e.getOriginalname()+".framework"), new File(destDir, e.getOriginalname()+".framework"), debug, additionalFiles);
	                	symlinkSubdirs(srcDir.getParentFile(), new File(srcDir, e.getOriginalname()+".framework"), new File(destDir, e.getOriginalname()+".framework"), debug, symlinks);
                    }else if(e.getType().equals(LibraryEntry.TYPE_QTJAMBI_EXE_UTILITY) && OSInfo.crossOS() == OSInfo.OS.MacOS){
                        copySubdirs(srcDir.getParentFile(), new File(srcDir, e.getName()+".app"), new File(destDir, e.getName()+".app"), debug, additionalFiles);
                        symlinkSubdirs(srcDir.getParentFile(), new File(srcDir, e.getName()+".app"), new File(destDir, e.getName()+".app"), debug, symlinks);
	                }else {
		                //getProject().log(this, "Copying " + srcFile + " to " + destFile + " (type: " + e.getType() + ", target file: "+e.getTargetName()+", absolute path: "+e.getAbsolutePath()+")", Project.MSG_INFO);
	                	if(e.getType().equals(LibraryEntry.TYPE_EXE)) {
	                		getProject().log(this, "Copying " + srcFile + " to " + destFile + " (type: " + e.getType() + ", target file: "+e.getTargetName()+", absolute path: "+e.getAbsolutePath()+")", Project.MSG_INFO);
	                	}
		                Util.copy(srcFile, destFile);
		                e.setPacked();
		                if(e.getType().equals(LibraryEntry.TYPE_QT) || e.getType().equals(LibraryEntry.TYPE_QTJAMBI_JNI)) {
			                if(e.getDsoVersion() == null || e.getDsoVersion().compareToIgnoreCase("no-version") != 0) {
			                    int qtMajorVersion = Integer.valueOf(AntUtil.getPropertyAsString(propertyHelper, Constants.QT_VERSION_MAJOR));
			                    int qtMinorVersion = Integer.valueOf(AntUtil.getPropertyAsString(propertyHelper, Constants.QT_VERSION_MINOR));
			                    String libInfix = AntUtil.getPropertyAsString(propertyHelper, Constants.QTJAMBI_QT_LIBINFIX);
			                	switch(OSInfo.crossOS()) {
			                    case MacOS:
			                    case Linux:
			        				Path target = Path.of(destFile.getName());
			                    	File link;
			                    	String name;
			                    	if(e.getType().equals(LibraryEntry.TYPE_QT)) {
			                    		name = LibraryEntry.formatQtName(e.getOriginalname(), libInfix, debug, qtMajorVersion, qtMinorVersion, -1);
			                    	}else {
			                    		name = LibraryEntry.formatQtJambiJniName(e.getOriginalname(), debug, qtMajorVersion, qtMinorVersion, -1);
			                    	}
			        				link = new File(destFile.getParentFile(), name);
			        				if(!link.exists()) {
			        					try{
			        						Files.createSymbolicLink(link.toPath(), target);
			        					}catch(java.nio.file.FileAlreadyExistsException ex) {
				    					} catch (Exception ex) {
				    						ex.printStackTrace();
				    					}
			        				}
		    						symlinks.add(new SimpleEntry<>(outputPath+"/"+name, outputPath+"/"+destFile.getName()));
			                    	if(e.getType().equals(LibraryEntry.TYPE_QT)) {
			                    		name = LibraryEntry.formatQtName(e.getOriginalname(), libInfix, debug, qtMajorVersion, -1, -1);
			                    	}else {
			                    		name = LibraryEntry.formatQtJambiJniName(e.getOriginalname(), debug, qtMajorVersion, -1, -1);
			                    	}
			        				link = new File(destFile.getParentFile(), name);
			        				if(!link.exists()) {
			        					try{
			        						Files.createSymbolicLink(link.toPath(), target);
			        					}catch(java.nio.file.FileAlreadyExistsException ex) {
				    					} catch (Exception ex) {
				    						ex.printStackTrace();
				    					}
			        				}
		    						symlinks.add(new SimpleEntry<>(outputPath+"/"+name, outputPath+"/"+destFile.getName()));
				                	switch(OSInfo.crossOS()) {
				                    case MacOS:
				                    	if(e.getType().equals(LibraryEntry.TYPE_QT)) {
				                    		name = LibraryEntry.formatQtName(e.getOriginalname(), libInfix, debug, -1, -1, -1);
				                    	}else {
				                    		name = LibraryEntry.formatQtJambiJniName(e.getOriginalname(), debug, -1, -1, -1);
				                    	}
				        				link = new File(destFile.getParentFile(), name);
				        				if(!link.exists()) {
				        					try{
				        						Files.createSymbolicLink(link.toPath(), target);
				        					}catch(java.nio.file.FileAlreadyExistsException ex) {
					    					} catch (Exception ex) {
					    						ex.printStackTrace();
					    					}
				        				}
			    						symlinks.add(new SimpleEntry<>(outputPath+"/"+name, outputPath+"/"+destFile.getName()));
			                    	default: break;
				                	}
			                    	break;
			                    	default: break;
			                	}
			                }
		                }
		
		                boolean doStrip = true;
		                if(e.getType().equals(LibraryEntry.TYPE_QT))
		                    doStrip = false;
		                else if(e.getType().equals(LibraryEntry.TYPE_PLUGIN))
		                    doStrip = false;
		                else if(e.getType().equals(LibraryEntry.TYPE_DECLARATIVEPLUGIN))
		                    doStrip = false;
		                else if(e.getType().equals(LibraryEntry.TYPE_QMLPLUGIN))
		                    doStrip = false;
		                else if(e.getType().equals(LibraryEntry.TYPE_DSO))
		                    doStrip = false;
		                else if(e.getType().equals(LibraryEntry.TYPE_SYSTEM))
		                    doStrip = false;
		                else if(e.getType().equals(LibraryEntry.TYPE_UNVERSIONED_PLUGIN))
		                    doStrip = false;
		                if(doStrip && execStrip != null)
		                    runBinaryStrip(destFile);
		                
		                if(e.getType().equals(LibraryEntry.TYPE_EXE) || srcFile.canExecute())
		                	destFile.setExecutable(true);
		
	                }
	                libraryDir.add(outputPath);
	            } catch(IOException ex) {
	                ex.printStackTrace();
	                throw new BuildException("Failed to copy library '" + libraryName + "'");
	            }
	        } catch(Exception ex) {
	            StringBuilder sb = new StringBuilder("DIAGNOSTIC");
	            if(rootPath != null)
	                sb.append("; rootPath=" + rootPath.getAbsolutePath());
	            if(libraryName != null)
	                sb.append("; libraryName=" + libraryName);
	            if(absolutePath != null)
	                sb.append("; absolutePath=" + absolutePath);
	            if(resolvedName != null)
	                sb.append("; resolvedName=" + resolvedName);
	            if(subdir != null)
	                sb.append("; subdir=" + subdir);
	            if(destSubdir != null)
	                sb.append("; destSubdir=" + destSubdir);
	            if(outputPath != null)
	                sb.append("; outputPath=" + outputPath);
	            if(srcDir != null)
	                sb.append("; srcDir=" + srcDir.getAbsolutePath());
	            if(destDir != null)
	                sb.append("; destDir=" + destDir.getAbsolutePath());
	            if(srcFile != null)
	                sb.append("; srcFile=" + srcFile.getAbsolutePath());
	            if(destFile != null)
	                sb.append("; destFile=" + destFile.getAbsolutePath());
	             ex.printStackTrace();
	            throw new BuildException(ex);
	        }
        }
    }
    
    static void copySubdirs(File root, File srcDir, File destDir, boolean debug, List<String> additionalFiles) throws IOException {
    	if(srcDir.isDirectory()) {
	    	for(File content : srcDir.listFiles()) {
	    		if((debug || (!content.getName().endsWith(".prl") && !content.getName().endsWith(".dSYM")))
	    				&& !content.getName().equals("Headers")) {
	    			if(!Files.isSymbolicLink(content.toPath())){
	    				if(content.isDirectory()) {
	        				copySubdirs(root, new File(srcDir, content.getName()), new File(destDir, content.getName()), debug, additionalFiles);
	        			}else {
		    				File destFile = new File(destDir, content.getName());
		    				destDir.mkdirs();
		    				Util.copy(content, destFile);
		    				if(content.canExecute())
			                	destFile.setExecutable(true);
		    				additionalFiles.add(root.toPath().relativize(content.toPath()).toString());
	        			}
	    			}
	    		}
	    	}
    	}else {
    		System.err.println("cannot copy directory "+srcDir);
    	}
    }
    
    static void symlinkSubdirs(File root, File srcDir, File destDir, boolean debug, List<Map.Entry<String,String>> symlinks) throws IOException {
    	if(srcDir.isDirectory()) {
	    	for(File content : srcDir.listFiles()) {
	    		if((debug || (!content.getName().endsWith(".prl") && !content.getName().endsWith(".dSYM")))
	    				&& !content.getName().equals("Headers")) {
	    			if(Files.isSymbolicLink(content.toPath())){
	    				File destFile = new File(destDir, content.getName());
	    				Path target = null;
	    				try {
	    					target = Files.readSymbolicLink(content.toPath());
	    					if(target.isAbsolute())
	    						target = srcDir.toPath().relativize(target);
	    					if(!Files.exists(destFile.toPath())) {
	    						try{
	    							Files.createSymbolicLink(destFile.toPath(), target);
	    						}catch(java.nio.file.FileAlreadyExistsException e) {}
	    					}else if(!Files.isSymbolicLink(destFile.toPath())){
	    						destFile.delete();
	    						try{
		    						Files.createSymbolicLink(destFile.toPath(), target);
								}catch(java.nio.file.FileAlreadyExistsException e) {}
	    					}
	    					if(!target.isAbsolute())
	    						target = srcDir.toPath().resolve(target);
							symlinks.add(new SimpleEntry<>(root.toPath().relativize(content.toPath()).toString(), root.toPath().relativize(target).toString()));
						} catch (Exception e) {
							System.out.println("root: "+root.toPath());
							System.out.println("link: "+content.toPath());
							System.out.println("target: "+target);
							e.printStackTrace();
						}
	    			}else if(content.isDirectory()) {
						symlinkSubdirs(root, new File(srcDir, content.getName()), new File(destDir, content.getName()), debug, symlinks);
	    			}
	    		}
	    	}
    	}
    }

    private void processSystemLibs() {
        String compiler = AntUtil.getPropertyAsString(propertyHelper, Constants.COMPILER);
        FindCompiler.Compiler c = FindCompiler.Compiler.resolve(compiler);

        String vcnumber = null;

        switch(c) {
        // The manifest based ones...
        case MSVC2017:
        case MSVC2017_64:
            if(vcnumber == null)
                vcnumber = "150";
            // fall-thru
        case MSVC2015:
        case MSVC2015_64:
            if(vcnumber == null)
                vcnumber = "140";
            // fall-thru
        case MSVC2013:
        case MSVC2013_64:
            if(vcnumber == null)
                vcnumber = "120";
            // fall-thru
        case MSVC2012:
        case MSVC2012_64:
            if(vcnumber == null)
                vcnumber = "110";
            // fall-thru
        case MSVC2010:
        case MSVC2010_64:
            if(vcnumber == null)
                vcnumber = "100";
            // fall-thru
        case MSVC2008:
        case MSVC2008_64:
            if(vcnumber == null)
                vcnumber = "90";
            // fall-thru
        case MSVC2005:
        case MSVC2005_64:
            if(vcnumber == null)
                vcnumber = "80";

            if(debug) {
                printVisualStudioDebugRuntimeWarning();
                break;
            }
            String vsredistdirPackageEnable = AntUtil.getPropertyAsString(propertyHelper, Constants.VSREDISTDIR_PACKAGE);
            String vsredistdir = AntUtil.getPropertyAsString(propertyHelper, Constants.VSREDISTDIR);
            if(vsredistdirPackageEnable != null && "false".equalsIgnoreCase(vsredistdirPackageEnable) && vsredistdir != null) {
                File crt = new File(vsredistdir, "Microsoft.VC" + vcnumber + ".CRT");

                String files[];
                switch(c) {
                // The manifest based ones...
                case MSVC2010:
                case MSVC2010_64:
	                files = new String[] {	"msvcp" + vcnumber + ".dll",
	                                      	"msvcr" + vcnumber + ".dll"
	                                };
	                break;
                case MSVC2012:
                case MSVC2012_64:
                case MSVC2013:
                case MSVC2013_64:
	                files = new String[] {	"msvcp" + vcnumber + ".dll",
	                                      	"msvcr" + vcnumber + ".dll",
	                                      	"vccorlib" + vcnumber + ".dll"
	                                };
	                break;
                case MSVC2015:
                case MSVC2015_64:
	                files = new String[] {	"msvcp" + vcnumber + ".dll",
	                                      	"vcruntime" + vcnumber + ".dll",
	                                      	"vccorlib" + vcnumber + ".dll"
	                                };
	                break;
                case MSVC2017:
                case MSVC2017_64:
	                files = new String[] {	"msvcp140.dll",
	                                      	"vcruntime140.dll",
	                                      	"vccorlib140.dll"
	                                };
	                break;
                default:
                	files = new String[] { 	"Microsoft.VC" + vcnumber + ".CRT.manifest",
				                            "msvcm" + vcnumber + ".dll",
				                            "msvcp" + vcnumber + ".dll",
				                            "msvcr" + vcnumber + ".dll"
		                            };
                	break;
                }

                for(String libDir : libraryDir) {
                    for(String name : files) {
                        String libdirstring;
                        if("".equals(libDir)) {
                            libdirstring = "lib/";
                        } else {
                            libdirstring = libDir;
                            if(!libdirstring.endsWith("/"))
                                libdirstring += "/";
                        }
                        File srcFile = new File(crt, name);
                        if(srcFile.isFile()) {
                        	File destFile;
                        	switch(c) {
                            case MSVC2012:
                            case MSVC2012_64:
                            case MSVC2013:
                            case MSVC2013_64:
                            case MSVC2015:
                            case MSVC2015_64:
                            case MSVC2017:
                            case MSVC2017_64:
                            	destFile = new File(outdir, libdirstring + name);
                            	break;
                        	default:
                        		String lib = libdirstring + "Microsoft.VC" + vcnumber + ".CRT/" + name;
                                destFile = new File(outdir, lib);
                                unpackLibs.add(lib);
                                break;
                            }

                            try {
                                Util.copy(srcFile, destFile);
                            } catch(Exception e) {
                                e.printStackTrace();
                                throw new BuildException("Failed to copy VS CRT libraries", e);
                            }
                        } else {
                            getProject().log(this, "WARNING: " + Constants.VSREDISTDIR + " " + srcFile.getAbsolutePath() + " does not exist; skipping", Project.MSG_WARN);
                        }
                    }
                }
            } else {
                // You also have to explicitly set -Dqtjambi.vsredistdir.package=true to make this feature work
                getProject().log(this, "WARNING: " + Constants.VSREDISTDIR + " property not set; skipping packaging of Visual C redistributable components.", Project.MSG_WARN);
            }
            break;

        case MSVC1998:
            if(debug) {
                printVisualStudioDebugRuntimeWarning();
                break;
            }
            copyRuntime("msvcr60.dll");
            copyRuntime("msvcp60.dll");
            break;

        case MSVC2002:
            if(debug) {
                printVisualStudioDebugRuntimeWarning();
                break;
            }
            copyRuntime("msvcr70.dll");
            copyRuntime("msvcp70.dll");
            break;

        case MSVC2003:
            if(debug) {
                printVisualStudioDebugRuntimeWarning();
                break;
            }
            copyRuntime("msvcr71.dll");
            copyRuntime("msvcp71.dll");
            break;

        case MinGW:
            // This is auto-detected and emitted in the descriptor now
            break;

        case GCC:
            // This is auto-detected and emitted in the descriptor now
            break;

        case OldGCC:
            // This is auto-detected and emitted in the descriptor now
            break;
		default:
			break;
        }

        // TODO: Make this an arbitrary list of files and provide helper options to
        //  populate with Unix libstdc++.so.5/libstdc++.so.6 values.  Allow each value
        //  to be a full-path to file, filename.
        String cplusplusRuntime = AntUtil.getPropertyAsString(propertyHelper, Constants.PACKAGING_DSO_CPLUSPLUSRUNTIME);
        if(cplusplusRuntime != null)
            copyRuntime(cplusplusRuntime);

    }

    private void copyRuntime(String name) {
        File rt = Util.findInLibraryPath(name, javaLibDir);
        if(rt == null) {
            throw new BuildException("Runtime library '" + name + "' was not found in library path...");
        }

        try {
            //getProject().log(this, "Copying " + rt.toString() + " to " + "lib/" + outdir + ", " + name, Project.MSG_VERBOSE);
            /*
             * "lib" is somewhat of a hack to specify where the files should be copied to.
             */
            Util.copy(rt, new File(outdir + File.separator + "lib", name));
            runtimeLibs.add("lib/" + name);
        } catch(IOException e) {
            e.printStackTrace();
            throw new BuildException("Failed to copy runtime library...", e);
        }
    }

    private void printVisualStudioDebugRuntimeWarning() {
        getProject().log(this, "", Project.MSG_INFO);
        getProject().log(this, "************************************************************************", Project.MSG_INFO);
        getProject().log(this, "", Project.MSG_INFO);
        getProject().log(this, "                              WARNING", Project.MSG_INFO);
        getProject().log(this, "", Project.MSG_INFO);
        getProject().log(this, "The debug runtimes for Visual Studio are not available for", Project.MSG_INFO);
        getProject().log(this, "redistribution by Microsoft, so it is not possible to create a", Project.MSG_INFO);
        getProject().log(this, "platform archive that runs on other machines...", Project.MSG_INFO);
        getProject().log(this, "", Project.MSG_INFO);
        getProject().log(this, "************************************************************************", Project.MSG_INFO);
        getProject().log(this, "", Project.MSG_INFO);
        getProject().log(this, "", Project.MSG_INFO);

    }
    
    private void processOSXInstallName() {
        getProject().log(this, "Processing Mac OS X install_name...", Project.MSG_INFO);
        PropertyHelper propertyHelper = PropertyHelper.getPropertyHelper(getProject());
        boolean useFrameworks = Boolean.valueOf(AntUtil.getPropertyAsString(propertyHelper, Constants.MAC_OS_USE_FRAMEWORK));
        boolean convertQtFrameworks = Boolean.valueOf(AntUtil.getPropertyAsString(propertyHelper, Constants.MAC_OS_CONVERT_QT_FRAMEWORK));
        
        try {
        	String[] out = Exec.executeCaptureOutput(this, Arrays.asList("install_name_tool", "-h"), outdir, getProject(), null, null, false);
        	if(out.length<2 || out[1]==null || !out[1].startsWith("Usage:")) {
                getProject().log(this, "\n********** Warning **********" , Project.MSG_INFO);
                getProject().log(this, "Without install_name_tool, you run the risk that Qt applications and plugins" , Project.MSG_INFO);
                getProject().log(this, "load incorrect Qt libraries, which" , Project.MSG_INFO);
                getProject().log(this, "may result in binary incompatility and crashes." , Project.MSG_INFO);
                getProject().log(this, "*****************************\n" , Project.MSG_INFO);
                return;
        	}
        } catch ( Exception e ) {
            getProject().log(this, " - " + (e.getMessage()!=null && !e.getMessage().isEmpty() ? e.getMessage() : e), e, Project.MSG_INFO);
        }
        boolean no_otool = false;
        try {
        	String[] out = Exec.executeCaptureOutput(this, Arrays.asList("otool", "--version"), outdir, getProject(), null, null, false);
        	if(out==null || out.length<2 || out[0]==null || !out[0].contains("Apple")) {
        		no_otool = true;
        		for (String string : out) {
        			if(string.contains("Apple")) {
        				no_otool = false;
        				break;
        			}
        		}
        	}
        	if(no_otool) {
        		getProject().log(this, "otool unavailable", Project.MSG_INFO);
        	}else
        		getProject().log(this, "otool available", Project.MSG_INFO);
        } catch ( Exception e ) {
        	no_otool = true;
        	getProject().log(this, "otool unavailable", e, Project.MSG_INFO);
        }

        for(LibraryEntry lib : libs) {
            if(LibraryEntry.TYPE_FILE.equals(lib.getType()) 
                    || LibraryEntry.TYPE_PLUGIN_JAR.equals(lib.getType()) 
                    || LibraryEntry.TYPE_FILESET.equals(lib.getType())
                    || (useFrameworks 
                    		&& !convertQtFrameworks 
                    		&& (LibraryEntry.TYPE_QT.equals(lib.getType())
				                    ||  LibraryEntry.TYPE_PLUGIN.equals(lib.getType())
				                    ||  LibraryEntry.TYPE_PLUGIN.equals(lib.getType())
				                    ||  LibraryEntry.TYPE_UNVERSIONED_PLUGIN.equals(lib.getType())))
                    || lib.isPending()){
                // never perform install_name_tool on files like "qmldir"
                continue;
            }
            String targetPath;
            {
                String targetDestSubdir = lib.getDestSubdir();
                String targetSubdir = lib.getSubdir();
                targetPath = Util.pathCanon(new String[] { targetDestSubdir, targetSubdir, lib.getResolvedName() }, "/"); //change.relativePath();
                if(LibraryEntry.TYPE_QTJAMBI_EXE_UTILITY.equals(lib.getType())){
                    targetPath = Util.pathCanon(new String[] { targetDestSubdir, targetSubdir, lib.getName() + ".app/Contents/MacOS/" + lib.getName() }, "/");
                }
            }

            OToolOut otoolOut = no_otool ? null : getOtoolOut(this, targetPath, outdir);
            if(otoolOut==null && !no_otool) {
            	getProject().log(this, "otool does not provide info for " + targetPath, Project.MSG_INFO);
            }
            
            String id = null;
            List<Map.Entry<String, String>> changeCommands = new ArrayList<>();
    		Set<String> addCommands = new TreeSet<>();
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
        	if(LibraryEntry.TYPE_QT.equals(lib.getType())){
        		if(useFrameworks) {
        			if(convertQtFrameworks) {
        				path = "@loader_path/.";
    	        		if(otoolOut==null || !otoolOut.rpaths.contains(path)) {
    	        			addCommands.add(path);
    	        		}
    	        		if(!lib.getOriginalname().equals("QtCore")) {
    	        			path = "@loader_path/../../../";
    	        			if(otoolOut==null || otoolOut.rpaths.contains(path)) {
    	        				deleteCommands.add(path);
    	        			}
    	        		}
            			path = "@loader_path/Frameworks";
            			if(otoolOut==null || otoolOut.rpaths.contains(path)) {
            				deleteCommands.add(path);
            			}
	        			if(otoolOut==null || !lib.getResolvedName().equals(otoolOut.id)) {
	        				id = lib.getResolvedName();
	        			}
        			}
        		}
        	}else if(LibraryEntry.TYPE_QTJAMBI_EXE_UTILITY.equals(lib.getType())) {
                path = "@executable_path/.";
                if(otoolOut==null || otoolOut.rpaths.contains(path)) {
                    deleteCommands.add(path);
                }
                deleteCommands.remove("@executable_path/../Frameworks");
                path = "@executable_path/../../../lib";
                if(otoolOut==null || !otoolOut.rpaths.contains(path)) {
                    addCommands.add(path);
                }
                path = "@executable_path/lib";
                if(otoolOut==null || !otoolOut.rpaths.contains(path)) {
                    addCommands.add(path);
                }
        	}else if(LibraryEntry.TYPE_EXE.equals(lib.getType())) {
        		if(useFrameworks) {
        			if(convertQtFrameworks) {
		    			path = "@loader_path/../../../../../../../";
		    			if(otoolOut==null || otoolOut.rpaths.contains(path)) {
		    				deleteCommands.add(path);
		    			}
		    			path = "@executable_path/../lib";
		        		if(otoolOut==null || !otoolOut.rpaths.contains(path)) {
		        			addCommands.add(path);
		        		}
        			}
        		}
        	}else if(LibraryEntry.TYPE_QTJAMBI_JNI.equals(lib.getType())) {
        	}else if(LibraryEntry.TYPE_QTJAMBI_UTILITY.equals(lib.getType())) {
    			String _id = "lib"+lib.getOriginalname()+".dylib";
    			if(otoolOut==null || !_id.equals(otoolOut.id)) {
    				id = _id;
    			}
            }
            
            for(LibraryEntry referencedLib : libs) {
                if(LibraryEntry.TYPE_PLUGIN.equals(referencedLib.getType()) 
                        || LibraryEntry.TYPE_EXE.equals(referencedLib.getType()) 
                        || LibraryEntry.TYPE_FILE.equals(referencedLib.getType()) 
                        || LibraryEntry.TYPE_PLUGIN_JAR.equals(referencedLib.getType()) 
                        || LibraryEntry.TYPE_DECLARATIVEPLUGIN.equals(referencedLib.getType()) 
                        || LibraryEntry.TYPE_QMLPLUGIN.equals(referencedLib.getType()) 
                        || LibraryEntry.TYPE_QTJAMBI_UTILITY.equals(referencedLib.getType())
                        || LibraryEntry.TYPE_QTJAMBI_EXE_UTILITY.equals(referencedLib.getType())
                        || LibraryEntry.TYPE_QTJAMBI_PLUGIN.equals(referencedLib.getType()) 
                        || LibraryEntry.TYPE_UNVERSIONED_PLUGIN.equals(referencedLib.getType()) 
                        || LibraryEntry.TYPE_FILESET.equals(referencedLib.getType())){
                    // never relink files (e.g. "qmldir") or plugin libraries
                    continue;
                }
                if(otoolOut==null && LibraryEntry.TYPE_QTJAMBI_JNI.equals(referencedLib.getType())) {
                	if(!referencedLib.getOriginalname().equals("QtJambi") && !referencedLib.getOriginalname().equals("QtJambiQml"))
                		continue;
                	if(!LibraryEntry.TYPE_QTJAMBI_JNI.equals(lib.getType())
                    		&& !LibraryEntry.TYPE_QTJAMBI_UTILITY.equals(lib.getType())
                    		&& !LibraryEntry.TYPE_QTJAMBI_PLUGIN.equals(lib.getType())) {
                        continue;                    	
                    }
                }

                String to;
                {
                    String withDestSubdir = referencedLib.getDestSubdir();
                    String withSubdir = referencedLib.getSubdir();
                    if(withSubdir == null)
                        withSubdir = "";
                    if("X".isEmpty()) {
                        getProject().log(this, " change.Name       =  " + lib.getName(), Project.MSG_VERBOSE);
                        getProject().log(this, " change.Subdir     =  " + lib.getSubdir(), Project.MSG_VERBOSE);
                        getProject().log(this, " change.DestSubdir =  " + lib.getDestSubdir(), Project.MSG_VERBOSE);
                        getProject().log(this, " with.destSubdir   =  " + withDestSubdir, Project.MSG_VERBOSE);
                        getProject().log(this, " with.Subdir       =  " + withSubdir, Project.MSG_VERBOSE);
                        getProject().log(this, " with.Name         =  " + referencedLib.getName(), Project.MSG_VERBOSE);
                    }
                    to = "@rpath/" + referencedLib.getShortName();
                }

                String from;
                // only name, when Qt is configured with -no-rpath
                if(convertQtFrameworks && LibraryEntry.TYPE_QT.equals(referencedLib.getType())){
                    from = "@rpath/"+referencedLib.getShortName();
                }else{
                    from = referencedLib.getShortName();
                }
                
                if(otoolOut==null || otoolOut.loadDylibs.contains(from)) {
                	changeCommands.add(new SimpleEntry<>(from, to));
                }

                //getProject().log(this, " exec " + Arrays.toString(cmd) + " in " + outdir, Project.MSG_INFO);
                if(convertQtFrameworks && LibraryEntry.TYPE_QT.equals(referencedLib.getType()) && from.endsWith("_debug")){
                    from = from.substring(0, from.length()-6);
                    if(otoolOut==null || otoolOut.loadDylibs.contains(from)) {
                    	changeCommands.add(new SimpleEntry<>(from, to));
                    }
                }
            }
            
            getProject().log(this, "-  updating: " + targetPath, Project.MSG_INFO);
        	if(id!=null) {
        		try {
    				Exec.exec(this, new String[] {
                            "install_name_tool",
                            "-id",
                            id,
                            targetPath
                        }, outdir, getProject(), false);
        		} catch ( BuildException e )
		        {
		        	getProject().log(this, " - " + (e.getMessage()!=null && !e.getMessage().isEmpty() ? e.getMessage() : e), e, Project.MSG_INFO);
		        }
        	}
//        	getProject().log(this, "-  add rpaths: " + addCommands, Project.MSG_INFO);
    		List<String> adds = new ArrayList<>(addCommands);
        	while(!adds.isEmpty()) {
        		List<String> commands = new ArrayList<>();
    			commands.add("install_name_tool");
        		for (int i = 0; i < 10 && i < adds.size(); i++) 
        		{
                    commands.add("-add_rpath");
					commands.add(adds.remove(0));
				}
				commands.add(targetPath);
				try{
					Exec.exec(this, commands.toArray(new String[commands.size()]), outdir, getProject(), false);
				} catch ( BuildException e )
		        {
		        	getProject().log(this, " - " + (e.getMessage()!=null && !e.getMessage().isEmpty() ? e.getMessage() : e), e, Project.MSG_INFO);
		        }
//                    	print(stream, commands.toArray(new String[commands.size()]));
        	}
//        	getProject().log(this, "-  delete rpaths: " + deleteCommands, Project.MSG_INFO);
        	{
        		List<String> deletes = new ArrayList<>(deleteCommands);
        		while(!deletes.isEmpty()) {
            		List<String> commands = new ArrayList<>();
        			commands.add("install_name_tool");
            		for (int i = 0; i < 10 && i < deletes.size(); i++) 
            		{
                        commands.add("-delete_rpath");
						commands.add(deletes.remove(0));
					}
					commands.add(targetPath);
					try{
						Exec.exec(this, commands.toArray(new String[commands.size()]), outdir, getProject(), false);
					} catch ( BuildException e )
			        {
			        	getProject().log(this, " - " + (e.getMessage()!=null && !e.getMessage().isEmpty() ? e.getMessage() : e), e, Project.MSG_INFO);
			        }
            	}
        	}
    		while(!changeCommands.isEmpty()) {
        		List<String> commands = new ArrayList<>();
    			commands.add("install_name_tool");
    			for (int i = 0; i < 10 && i < changeCommands.size(); i++) 
    			{
        			commands.add("-change");
        			Map.Entry<String, String> e = changeCommands.remove(0);
					commands.add(e.getKey());
					commands.add(e.getValue());
				}
				commands.add(targetPath);
				try {
					Exec.exec(this, commands.toArray(new String[commands.size()]), outdir, getProject(), false);
				} catch ( BuildException e )
		        {
		        	getProject().log(this, " - " + (e.getMessage()!=null && !e.getMessage().isEmpty() ? e.getMessage() : e), e, Project.MSG_INFO);
		        }
        	}
        }
        
        if(!useFrameworks || convertQtFrameworks) {
	        for(Directory dir : directoryList){
	            java.util.ArrayList<File> directories = new java.util.ArrayList<File>();
	            File startDir = new  File(outdir + "/" + dir.getDestSubdir()+"/"+dir.getName());
	            directories.add(startDir);
	            while(!directories.isEmpty()){
	                for(File directory : new java.util.ArrayList<File>(directories)){
	                    directories.remove(directory);
	                    for(File file : directory.listFiles()){
	                        if(file.isDirectory()){
	                            if(!file.getName().endsWith(".dylib.dSYM"))
	                                directories.add(file);
	                        }else if(file.isFile() && file.getName().endsWith(".dylib")){
	                            List<Map.Entry<String, String>> changeCommands = new ArrayList<>();
	                            String targetPath = file.getAbsolutePath();
	                            OToolOut otoolOut = no_otool ? null : getOtoolOut(this, targetPath, outdir);
	                            if(otoolOut==null && !no_otool) {
	                            	getProject().log(this, "otool does not provide info for " + targetPath, Project.MSG_INFO);
	                            }
	                        	for(LibraryEntry referencedLib : libs) {
	                            	if(LibraryEntry.TYPE_QT.equals(referencedLib.getType())){
			                            String from;
			                            if(convertQtFrameworks){
			                            	from = "@rpath/"+referencedLib.getShortName();
			                            }else{
			                            	from = referencedLib.getShortName();
			                            }
			                            if(otoolOut==null || otoolOut.loadDylibs.contains(from)) {
			                            	changeCommands.add(new SimpleEntry<>(from, "@rpath/" + referencedLib.getResolvedName()));
			                            }
			                            if(convertQtFrameworks && from.endsWith("_debug")){
			                            	from = from.substring(0, from.length()-6);
			                            	if(otoolOut==null || otoolOut.loadDylibs.contains(from)) {
			                            		changeCommands.add(new SimpleEntry<>(from, "@rpath/" + referencedLib.getResolvedName()));
			                            	}
			                            }
	                            	}
	                        	}
	                            getProject().log(this, "-  updating: " + targetPath, Project.MSG_INFO);
	                    		while(!changeCommands.isEmpty()) {
	                        		List<String> commands = new ArrayList<>();
	                    			commands.add("install_name_tool");
	                    			for (int i = 0; i < 10 && i < changeCommands.size(); i++) 
	                    			{
	                        			commands.add("-change");
	                        			Map.Entry<String, String> e = changeCommands.remove(0);
	            						commands.add(e.getKey());
	            						commands.add(e.getValue());
	            					}
	            					commands.add(targetPath);
	            					try {
	            						Exec.exec(this, commands.toArray(new String[commands.size()]), outdir, getProject(), false);
	            					} catch ( BuildException e )
	            			        {
	            			        	getProject().log(this, " - " + (e.getMessage()!=null && !e.getMessage().isEmpty() ? e.getMessage() : e), e, Project.MSG_INFO);
	            			        }
	                        	}
	                        }
	                    }
	                }
	            }
	        }
        }
    }
    
    private void processRPath()
    {
		getProject().log(this, "Processing RPATH...", Project.MSG_INFO);
        try
        {
            int qtMajorVersion = Integer.valueOf(AntUtil.getPropertyAsString(propertyHelper, Constants.QT_VERSION_MAJOR));
            int qtMinorVersion = Integer.valueOf(AntUtil.getPropertyAsString(propertyHelper, Constants.QT_VERSION_MINOR));
            int qtPatchlevelVersion = Integer.valueOf(AntUtil.getPropertyAsString(propertyHelper, Constants.QT_VERSION_PATCHLEVEL));
            String sosuffix = ".so."+qtMajorVersion+"."+qtMinorVersion+"."+qtPatchlevelVersion;
			java.util.ArrayList<File> directories = new java.util.ArrayList<File>();
			directories.add(new File(outdir, "lib"));
			String suffix = "/.";
			while(!directories.isEmpty()){
				for(File directory : new java.util.ArrayList<File>(directories)){
					directories.remove(directory);
					for(File file : directory.listFiles()){
						if(file.isDirectory()){
							directories.add(file);
						}else if(file.isFile() && file.getName().startsWith("libQtJambi") && file.getName().endsWith(sosuffix)){
							try{
								Exec.exec (this, new String[]{"chrpath", "--replace", "$ORIGIN"+suffix, file.getAbsolutePath()}, file.getParentFile(), getProject(), true );
							}catch(Exception e){
							}
						}
					}
				}
				if(suffix.length()==2)
					suffix += '.';
				else suffix = "/.." + suffix;
			}
			
			File utilFile = Boolean.TRUE.equals(debug) ? new File(outdir, "utilities/libqtjambiplugin_debug.so") : new File(outdir, "utilities/libqtjambiplugin.so");
			if(utilFile.exists()) {
				try{
					Exec.exec (this, new String[]{"chrpath", "--replace", "$ORIGIN/../../lib", utilFile.getAbsolutePath()}, utilFile.getParentFile(), getProject(), true );
				}catch(Exception e){
				}
			}
			utilFile = Boolean.TRUE.equals(debug) ? new File(outdir, "utilities/libjarimport_debug.so") : new File(outdir, "utilities/libjarimport.so");
			if(utilFile.exists()) {
				try{
					Exec.exec (this, new String[]{"chrpath", "--replace", "$ORIGIN/../lib"
																		+ ":$ORIGIN/../../lib"
																		+ ":$ORIGIN/../../../lib"
																		+ ":$ORIGIN/../../../../lib"
																		+ ":$ORIGIN/../../../../../lib"
																		+ ":$ORIGIN/../../../../../../lib", utilFile.getAbsolutePath()}, utilFile.getParentFile(), getProject(), true );
				}catch(Exception e){
				}
			}
            utilFile = Boolean.TRUE.equals(debug) ? new File(outdir, "utilities/QtJambiLauncher_debug") : new File(outdir, "utilities/QtJambiLauncher");
			if(utilFile.exists()) {
				try{
					Exec.exec (this, new String[]{"chrpath", "--replace", "$ORIGIN/lib"
																		+ ":$ORIGIN/../lib", utilFile.getAbsolutePath()}, utilFile.getParentFile(), getProject(), true );
				}catch(Exception e){
				}
			}
        }
        catch ( Exception e )
        {
            getProject().log(this, " - " + e.getMessage() , Project.MSG_INFO);
            getProject().log(this, "\n********** Warning **********" , Project.MSG_INFO);
            getProject().log(this, "Without rpaths, you run the risk that Qt applications and plugins" , Project.MSG_INFO);
            getProject().log(this, "load incorrect Qt libraries (such as /usr/lib/libQtCore.so), which" , Project.MSG_INFO);
            getProject().log(this, "may result in binary incompatility and crashes." , Project.MSG_INFO);
            getProject().log(this, "*****************************\n" , Project.MSG_INFO);
        }
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

    static String xmlEscape(String s) {
        if(s == null)
            return s;
        StringBuilder sb = new StringBuilder();
        final int len = s.length();
        for(int i = 0; i < len; i++) {
            char c = s.charAt(i);
            switch(c) {
            case '<':
                sb.append("&lt;");
                break;
            case '>':
                sb.append("&gt;");
                break;
            case '&':
                sb.append("&amp;");
                break;
            default:
                sb.append(c);
                break;
            }
        }
        return sb.toString();
    }
}
