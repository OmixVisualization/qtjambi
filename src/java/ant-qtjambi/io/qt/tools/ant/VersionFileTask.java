/****************************************************************************
**
** Copyright (C) 2011 Darryl L. Miles.  All rights reserved.
** Copyright (C) 2011 D L Miles Consulting Ltd.  All rights reserved.
**
** This file is part of Qt Jambi.
**
**
** $BEGIN_LICENSE$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html
** 
** In addition, as a special exception, the copyright holders grant you
** certain additional rights. These rights are described in the Nokia Qt
** LGPL Exception version 1.0, included in the file LGPL_EXCEPTION.txt in
** this package.
** 
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 2.0 requirements will be
** met: http://www.gnu.org/licenses/gpl-2.0.html
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL3 included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

package io.qt.tools.ant;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.PrintStream;
import java.util.Enumeration;
import java.util.Properties;

import org.apache.tools.ant.BuildException;
import org.apache.tools.ant.PropertyHelper;
import org.apache.tools.ant.Task;

public class VersionFileTask extends Task {

    private PropertyHelper propertyHelper;

    private String targetdir;
    private String pathVersionPropertiesTemplate;

    public String getPathVersionPropertiesTemplate() {
        return pathVersionPropertiesTemplate;
    }
    public void setPathVersionPropertiesTemplate(String pathVersionPropertiesTemplate) {
        this.pathVersionPropertiesTemplate = pathVersionPropertiesTemplate;
    }
    public static final String DEFAULT_PATH_VERSION_PROPERTIES_TEMPLATE = "version.properties.template";

    public void execute() throws BuildException {
        propertyHelper = PropertyHelper.getPropertyHelper(getProject());

        File dir = new File((targetdir==null ? AntUtil.getPropertyAsString(propertyHelper, "java.outdir") : targetdir)+"/io/qt/internal");
        dir.mkdirs();
        File fileVersion = new File(dir, "QtJambiVersion.java");
        fileVersion.delete();
        PrintStream ps = null;
        String libInfix = AntUtil.getPropertyAsString(propertyHelper, Constants.QTJAMBI_QT_LIBINFIX);
//        String qtVersion = AntUtil.getPropertyAsString(propertyHelper, Constants.QT_VERSION);
        String qtMajorVersion = AntUtil.getPropertyAsString(propertyHelper, Constants.QT_VERSION_MAJOR);
        String qtMinorVersion = AntUtil.getPropertyAsString(propertyHelper, Constants.QT_VERSION_MINOR);
        String qtJambiPatchVersion = AntUtil.getPropertyAsString(propertyHelper, "qtjambi.patchversion");
        
        String tmpPathVersionPropertiesTemplate = pathVersionPropertiesTemplate;
        if(tmpPathVersionPropertiesTemplate == null)
            tmpPathVersionPropertiesTemplate = AntUtil.getPropertyAsString(propertyHelper, "version.properties.template");
        if(tmpPathVersionPropertiesTemplate == null)
            tmpPathVersionPropertiesTemplate = DEFAULT_PATH_VERSION_PROPERTIES_TEMPLATE;
        
        File fileTemplate = new File(new File(AntUtil.getPropertyAsString(propertyHelper, "jambi.directory")), tmpPathVersionPropertiesTemplate);
        
        try {
        	Integer.parseInt(qtMajorVersion);
        	Integer.parseInt(qtMinorVersion);
        	Integer.parseInt(qtJambiPatchVersion);
			ps = new PrintStream(fileVersion);
			ps.println("package io.qt.internal;");
			ps.println();
			ps.println("final class QtJambiVersion {");
			ps.println("    ");
			ps.println("    private QtJambiVersion(){throw new RuntimeException();}");
			ps.println("    ");
			ps.print("    final static int qtMajorVersion = ");
			ps.print(qtMajorVersion);
			ps.println(";");
			ps.println("    ");
			ps.print("    final static int qtMinorVersion = ");
			ps.print(qtMinorVersion);
			ps.println(";");
			ps.println("    ");
			ps.println("    final static java.util.Properties properties = new java.util.Properties();");
			ps.println("    ");
			ps.println("    static{");
			if(libInfix!=null) {
				ps.print("        properties.setProperty(\"qtjambi.libinfix\", \"");
				ps.print(libInfix);
				ps.println("\");");
			}
			ps.println("        properties.setProperty(\"qtjambi.icu.version\", \"56\");");
			if(fileTemplate.exists()) {
	            Properties props = null;
            	try(InputStream inStream = new FileInputStream(fileTemplate)){
	                props = new Properties();
	                props.load(inStream);        // read in
	                Enumeration<? extends Object> e = props.propertyNames();
	                while (e.hasMoreElements()) {
	                    String key = (String) e.nextElement();
	                    key = key.replace("\\", "\\\\");
	                    key = key.replace("\"", "\\\"");
	                    String value = props.getProperty(key);
	                    value = value.replace("\\", "\\\\");
	                    value = value.replace("\"", "\\\"");
	        			ps.print("        properties.setProperty(\"");
	        			ps.print(key);
	        			ps.print("\", \"");
	        			ps.print(value);
	        			ps.println("\");");
	                }
	            } catch(IOException e) {
	                throw new BuildException(e);
            	}
            }
			ps.println("    }");
			ps.println("    ");
			ps.print("    final static int qtJambiPatch = ");
			ps.print(qtJambiPatchVersion);
			ps.println(";");
			ps.println("}");
		} catch (IOException e) {
			throw new BuildException(e);
		}finally {
			if(ps!=null) {
				ps.close();
			}
		}
    }

	public String getTargetdir() {
		return targetdir;
	}
	public void setTargetdir(String targetdir) {
		this.targetdir = targetdir;
	}
}
