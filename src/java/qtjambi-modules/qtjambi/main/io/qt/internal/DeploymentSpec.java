/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2021 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
package io.qt.internal;

import java.io.File;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

// NOTE the use of package level security on this class for any methods that may cause modification 
class DeploymentSpec {
    private String compiler;
	private String configuration;
    private String date;
    private String time;
    private String system;
    private String version;
    private String module;
	private URL sourceUrl;
    private File baseDir;
    private URL baseUrl;
    private List<LibraryEntry> libraries;
    private List<String> pluginPaths;
    private List<String> pluginDesignerPaths;
    private List<String> dirents;

    void addPluginPath(String path) {
        if (pluginPaths == null)
            pluginPaths = new ArrayList<String>();
        pluginPaths.add(path);
    }

    void addPluginDesignerPath(String path) {
        if (pluginDesignerPaths == null)
            pluginDesignerPaths = new ArrayList<String>();
        pluginDesignerPaths.add(path);
    }

    void addLibraryEntry(LibraryEntry e) {
        if (libraries == null)
            libraries = new ArrayList<LibraryEntry>();
        libraries.add(e);
    }

    void addDirentPath(String direntAsString) {
        if (dirents == null)
            dirents = new ArrayList<String>();
        dirents.add(direntAsString);
    }

    public File buildPath(String relativePath) {
        if(baseDir == null)
            return null;  // we're loaded via non file:/// method ?  Java Web Start anyone ?
        return new File(baseDir, relativePath);
    }

    public URL buildUrl(String relativeUrl) throws MalformedURLException {
        return new URL(baseUrl, relativeUrl);
    }

    public URL getSourceUrl() {
        return sourceUrl;
    }
    void setSourceUrl(URL sourceUrl) {
        this.sourceUrl = sourceUrl;
    }

    public File getBaseDir() {
        return baseDir;
    }
    void setBaseDir(File baseDir) {
        this.baseDir = baseDir;
    }

    public URL getBaseUrl() {
        return baseUrl;
    }
    void setBaseUrl(URL baseUrl) {
        this.baseUrl = baseUrl;
    }

    // Package private getter as no need to expose outside the package
    List<LibraryEntry> getLibraries() {
        if(libraries == null)
                return Collections.emptyList();
        return Collections.unmodifiableList(libraries);
    }
    void setLibraries(List<LibraryEntry> libraries) {
        this.libraries = libraries;
    }

    public List<String> getPluginPaths() {
        if(pluginPaths == null)
                return Collections.emptyList();
        return Collections.unmodifiableList(pluginPaths);
    }
    void setPluginPaths(List<String> pluginPaths) {
        this.pluginPaths = pluginPaths;
    }

    public List<String> getPluginDesignerPaths() {
        if(pluginDesignerPaths == null)
                return Collections.emptyList();
        return Collections.unmodifiableList(pluginDesignerPaths);
    }
    void setPluginDesignerPaths(List<String> pluginDesignerPaths) {
        this.pluginDesignerPaths = pluginDesignerPaths;
    }

    public List<String> getDirents() {
        if(dirents == null)
                return Collections.emptyList();
        return Collections.unmodifiableList(dirents);
    }
    void setDirents(List<String> dirents) {
        this.dirents = dirents;
    }
    public String getCompiler() {
		return compiler;
	}

	public void setCompiler(String compiler) {
		this.compiler = compiler;
	}

	public String getConfiguration() {
		return configuration;
	}

	public void setConfiguration(String configuration) {
		this.configuration = configuration;
	}

	public String getDate() {
		return date;
	}

	public void setDate(String date) {
		this.date = date;
	}

	public String getTime() {
		return time;
	}

	public void setTime(String time) {
		this.time = time;
	}

	public String getSystem() {
		return system;
	}

	public void setSystem(String system) {
		this.system = system;
	}
	
    public String getVersion() {
		return version;
	}

	public void setVersion(String version) {
		this.version = version;
	}

	public String getModule() {
		return module;
	}

	public void setModule(String module) {
		this.module = module;
	}
}
