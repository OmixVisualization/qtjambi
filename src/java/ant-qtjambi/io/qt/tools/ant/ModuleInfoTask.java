/****************************************************************************
**
** Copyright (C) 2009-2021 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
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

import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.HashSet;
import java.util.Set;

import org.apache.tools.ant.BuildException;
import org.apache.tools.ant.Project;
import org.apache.tools.ant.Task;

public class ModuleInfoTask extends Task {
    private String outputDirectory;
    private String javaDirectory;

    private String msg = "";
    @Override
    public void execute() throws BuildException {
        getProject().log(this, msg, Project.MSG_INFO);
		java.io.File directory = new java.io.File(outputDirectory);
		Set<String> modules = new HashSet<>();
		for(java.io.File subdir : new java.io.File(javaDirectory).listFiles()) {
			if(subdir.isDirectory() && subdir.getName().startsWith("qtjambi.")) {
				modules.add(subdir.getName());
			}
		}
		try(PrintWriter writer = new PrintWriter(new FileWriter(new java.io.File(directory, "module-info.java")));){
			writer.println("module qtjambi.autotest{");
			writer.println("\trequires java.base;");
			writer.println("\trequires java.logging;");
			writer.println("\trequires java.desktop;");
			writer.println("\trequires junit;");
			writer.println("\trequires org.hamcrest.core;");
			writer.println("\trequires transitive qtjambi;");
			
			for(String pkg : modules){
				if(!pkg.equals("io.qt.tools.designer")){
					writer.print("\trequires transitive ");
					writer.print(pkg);
					writer.println(";");
				}
			}
			writer.println("\texports io.qt.autotests;");
			writer.println("\texports io.qt.unittests;");
			writer.println("\texports io.qt.unittests.support;");
			writer.println("\texports io.qt.generatortests;");
			writer.println("}");
		}catch(IOException e){
			throw new BuildException(e);
		}
    }
	
    public void setOutputDirectory(String outputDirectory) {
        this.outputDirectory = outputDirectory;
    }
    
    public void setJavaDirectory(String javaDirectory) {
        this.javaDirectory = javaDirectory;
    }
}
