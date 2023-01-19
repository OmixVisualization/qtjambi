/****************************************************************************
**
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
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
package io.qt.autotests;

import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.StandardCopyOption;

import io.qt.core.QByteArray;
import io.qt.core.QCoreApplication;
import io.qt.core.QFile;
import io.qt.core.QIODevice;
import io.qt.core.internal.QFactoryLoader;
import io.qt.sql.QSqlDriver;
import io.qt.sql.QSqlDriverPlugin;

public class DeploymentSql {
    public static void main(String args[]) {
		try {
			DeploymentSqlImpl.main(args);
    	}catch(java.lang.NoClassDefFoundError e) {
    		System.err.println("java.class.path="+System.getProperty("java.class.path"));
    		throw e;
    	}catch(java.lang.ExceptionInInitializerError e) {
    		System.err.println("java.class.path="+System.getProperty("java.class.path"));
    		System.err.println("java.library.path="+System.getProperty("java.library.path"));
    		if(io.qt.QtUtilities.jambiDeploymentDir()!=null && io.qt.QtUtilities.jambiDeploymentDir().isDirectory()) {
    			System.err.println("qtjambi.deplspec.dir="+io.qt.QtUtilities.jambiDeploymentDir().getAbsolutePath());
    		}
        	String osName = System.getProperty("os.name").toLowerCase();
    		if(osName.startsWith("windows")) {
    			if(System.getenv("PATH")!=null)
    				System.err.println("PATH="+System.getenv("PATH"));
        	}else if(osName.startsWith("mac")) {
        		if(System.getenv("DYLD_LIBRARY_PATH")!=null)
        			System.err.println("DYLD_LIBRARY_PATH="+System.getenv("DYLD_LIBRARY_PATH"));
        	}else if(osName.startsWith("linux")) {
    			if(System.getenv("LD_LIBRARY_PATH")!=null)
    				System.err.println("LD_LIBRARY_PATH="+System.getenv("LD_LIBRARY_PATH"));
    		}
    		throw e;
    	}
    }
}

class DeploymentSqlImpl {
	static void copyDir(File file, File toDir) throws IOException {
		File newFile = new File(toDir, file.getName());
		if(file.isDirectory()) {
			newFile.mkdirs();
			for(File subFile : file.listFiles()) {
				copyDir(subFile, newFile);
			}
		}else {
			Files.copy(file.toPath(), newFile.toPath(), StandardCopyOption.REPLACE_EXISTING);
		}
	}
	
	static void main(String args[]) {
    	QCoreApplication.initialize(args);
    	try {
    		QFactoryLoader loader = new QFactoryLoader(QSqlDriverPlugin.class, "/sqldrivers");
    		QSqlDriver plugin = loader.loadPlugin(QSqlDriverPlugin::create, "QJDBC");
    		if(plugin==null) {
    			throw new AssertionError("plugin==null. LibraryPath="+QCoreApplication.libraryPaths()+", java.class.path="+System.getProperty("java.class.path"));
    		}
    		try {
	    		if(!"io::qt::sql::jdbc::QJdbcSqlDriver".equals(plugin.metaObject().className()))
	    			throw new AssertionError("Expected: io::qt::sql::jdbc::QJdbcSqlDriver, found: "+plugin.metaObject().className());
    		}finally {
    			plugin.dispose();
    		}
			if(QCoreApplication.arguments().size()>=3) {
				QFile file = new QFile(QCoreApplication.arguments().get(1));
				if(file.open(QIODevice.OpenModeFlag.WriteOnly)) {
					file.write(new QByteArray(QCoreApplication.arguments().get(2)));
					file.close();
				}else {
					System.err.println("File "+QCoreApplication.arguments().get(1)+" not open because: "+file.errorString());
				}
				file.dispose();
	    	}else {
	    		throw new RuntimeException("Two arguments expected.");
	    	}
    	}finally {
    		QCoreApplication.shutdown();
    	}
	}
}