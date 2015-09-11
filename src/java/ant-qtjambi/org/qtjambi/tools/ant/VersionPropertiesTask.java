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

package org.qtjambi.tools.ant;

import org.apache.tools.ant.BuildException;
import org.apache.tools.ant.Project;
import org.apache.tools.ant.Task;
import org.apache.tools.ant.PropertyHelper;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.IOException;

import java.util.Properties;

public class VersionPropertiesTask extends Task {

    private PropertyHelper propertyHelper;

    private String pathVersionProperties;
    private String pathVersionPropertiesTemplate;

    public static final String DEFAULT_PATH_VERSION_PROPERTIES          = "version.properties";
    public static final String DEFAULT_PATH_VERSION_PROPERTIES_TEMPLATE = "version.properties.template";

    public String getPathVersionProperties() {
        return pathVersionProperties;
    }
    public void setPathVersionProperties(String pathVersionProperties) {
        this.pathVersionProperties = pathVersionProperties;
    }

    public String getPathVersionPropertiesTemplate() {
        return pathVersionPropertiesTemplate;
    }
    public void setPathVersionPropertiesTemplate(String pathVersionPropertiesTemplate) {
        this.pathVersionPropertiesTemplate = pathVersionPropertiesTemplate;
    }

    public void execute() throws BuildException {
        propertyHelper = PropertyHelper.getPropertyHelper(getProject());

        String tmpPathVersionProperties = pathVersionProperties;
        if(tmpPathVersionProperties == null)
            tmpPathVersionProperties = AntUtil.getPropertyAsString(propertyHelper, Constants.QT_VERSION_PROPERTIES);
        if(tmpPathVersionProperties == null)
            tmpPathVersionProperties = DEFAULT_PATH_VERSION_PROPERTIES;
        String basedir = AntUtil.getPropertyAsString(propertyHelper, "basedir");
        
        File fileVersion = new File(new File(basedir), tmpPathVersionProperties);

        String tmpPathVersionPropertiesTemplate = pathVersionPropertiesTemplate;
        if(tmpPathVersionPropertiesTemplate == null)
            tmpPathVersionPropertiesTemplate = AntUtil.getPropertyAsString(propertyHelper, Constants.QT_VERSION_PROPERTIES_TEMPLATE);
        if(tmpPathVersionPropertiesTemplate == null)
            tmpPathVersionPropertiesTemplate = DEFAULT_PATH_VERSION_PROPERTIES_TEMPLATE;
        
        File fileTemplate = new File(new File(basedir), tmpPathVersionPropertiesTemplate);

        String libInfix = AntUtil.getPropertyAsString(propertyHelper, Constants.QTJAMBI_QT_LIBINFIX);
        String qtVersion = AntUtil.getPropertyAsString(propertyHelper, Constants.QT_VERSION);
        if(qtVersion == null)
            throw new BuildException("Unable to determine Qt version, try editing: " + fileTemplate.getAbsolutePath());

        String qtjambiSonameVersionMajor = AntUtil.getPropertyAsString(propertyHelper, Constants.QTJAMBI_SONAME_VERSION_MAJOR);
        if(Integer.parseInt(AntUtil.getPropertyAsString(propertyHelper, Constants.QT_VERSION_MAJOR))>=5){
        	qtjambiSonameVersionMajor = null;
        }

        buildNewVersionProperties(fileVersion, fileTemplate, qtVersion, libInfix, qtjambiSonameVersionMajor);
    }

    private boolean buildNewVersionProperties(File fileVersion, File fileTemplate, String qtVersion, String libInfix, String qtjambiSonameVersionMajor) {
        boolean allOk = false;
        // FIXME: This part below should really be a sub-operation / new Ant Task that
        //  modifies the version.properties file

        // If detected, open version.properties to set version (or warning mismatch)
        if(true /*&& !foundInVersionPropertiesTemplate*/) {
            // Build a version.properties file.
            File fileOut = fileVersion.getAbsoluteFile();
            if(fileOut.exists()) {
                if(fileOut.delete() == false)
                    throw new BuildException("Unable to delete file: " + fileOut.getAbsolutePath());
                getProject().log(this, "Deleted file to rebuild " + fileOut.getAbsolutePath(), Project.MSG_VERBOSE);
            }

            InputStream inStream = null;
            OutputStream outStream = null;
            Properties props = null;
            try {
                inStream = new FileInputStream(fileTemplate);

                props = new Properties();
                props.load(inStream);        // read in
                props.put(Constants.VERSION, qtVersion);    // set version
                props.put(Constants.QTJAMBI_LIBINFIX, libInfix);    // set version
                if(qtjambiSonameVersionMajor != null)
                    props.put(Constants.QTJAMBI_SONAME_VERSION_MAJOR, qtjambiSonameVersionMajor);  // set version

                inStream.close();
                inStream = null;

                outStream = new FileOutputStream(fileOut);
                String header = "Auto-generated by " + getClass().getName();
                props.store(outStream, header);    // write out

                outStream.close();
                outStream = null;

                allOk = true;
            } catch(IOException e) {
                throw new BuildException(e);
            } finally {
                if(outStream != null) {
                    try {
                        outStream.close();
                    } catch(IOException eat) {
                    }
                    outStream = null;
                }
                if(inStream != null) {
                    try {
                        inStream.close();
                    } catch(IOException eat) {
                    }
                    inStream = null;
                }
            }

            getProject().log(this, "File rebuilt " + fileOut.getAbsolutePath() + " from template file " + fileTemplate.getAbsolutePath(), Project.MSG_VERBOSE);
        }

        return allOk;
    }
}
