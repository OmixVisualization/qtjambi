/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2022 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import java.util.ArrayList;
import java.util.List;

// Package private as we don't expose it outside the package.
class LibraryEntry {
    private String name;
    private DeploymentSpec deploymentSpec;
    private boolean loaded;
    
    public String getName() {
        return name;
    }
    void setName(String name) {
        this.name = name;
    }

    public DeploymentSpec getDeploymentSpec() {
        return deploymentSpec;
    }
    void setDeploymentSpec(DeploymentSpec deploymentSpec) {
        this.deploymentSpec = deploymentSpec;
    }

    public boolean isExtracting() {
    	synchronized(this.extractionFunctions) {
    		return !extractionFunctions.isEmpty();
    	}
    }
    
    public boolean isQmlExtracting() {
    	synchronized(this.qmlExtractionFunctions) {
    		return !qmlExtractionFunctions.isEmpty();
    	}
    }
    
    public boolean isLoaded() {
        return loaded;
    }
    void setLoaded(boolean loaded) {
        this.loaded = loaded;
        if(loaded) {
        	synchronized(this.extractionFunctions) {
        		extractionFunctions.clear();
        	}
        	synchronized(this.qmlExtractionFunctions) {
        		qmlExtractionFunctions.clear();
        	}
        }
    }
    
    interface ExtractionFunction{
    	void extract() throws Throwable;
    }

    private final List<ExtractionFunction> extractionFunctions = new ArrayList<>();
    private final List<ExtractionFunction> qmlExtractionFunctions = new ArrayList<>();

	public void addExtractionFunction(ExtractionFunction loadFunction) {
		synchronized(this.extractionFunctions) {
			this.extractionFunctions.add(loadFunction);
		}
	}
	
	public void addQmlExtractionFunction(ExtractionFunction loadFunction) {
		synchronized(this.qmlExtractionFunctions) {
			this.qmlExtractionFunctions.add(loadFunction);
		}
	}
	
	public void extract() throws Throwable {
		NativeLibraryManager.reporter.report("extracting ", name);
		List<ExtractionFunction> extractionFunctions;
		synchronized(this.extractionFunctions) {
			extractionFunctions = new ArrayList<>(this.extractionFunctions);
			this.extractionFunctions.clear();
		}
		if(!extractionFunctions.isEmpty()) {
			for(ExtractionFunction f : extractionFunctions) {
				f.extract();
			}
		}
	}
	
	public void extractQml() throws Throwable {
		List<ExtractionFunction> extractionFunctions;
		synchronized(this.qmlExtractionFunctions) {
			extractionFunctions = new ArrayList<>(this.qmlExtractionFunctions);
			this.qmlExtractionFunctions.clear();
		}
		if(!extractionFunctions.isEmpty()) {
			for(ExtractionFunction f : extractionFunctions) {
				f.extract();
			}
		}
	}
}
