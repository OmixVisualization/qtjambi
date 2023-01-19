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

import io.qt.core.QByteArray;
import io.qt.core.QEvent;
import io.qt.core.QFile;
import io.qt.core.QIODevice;
import io.qt.core.QObject;
import io.qt.widgets.QApplication;

public class Deployment {
    public static void main(String args[]) {
		try {
			DeploymentFileWriter.main(args);
    	}catch(java.lang.NoClassDefFoundError e) {
    		System.err.println("java.class.path="+System.getProperty("java.class.path"));
    		throw e;
    	}catch(java.lang.ExceptionInInitializerError e) {
    		System.err.println("java.library.path="+System.getProperty("java.library.path"));
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

class DeploymentFileWriter extends QObject {
	static void main(String args[]) {
    	QApplication.initialize(args);
    	try {
			if(QApplication.arguments().size()>=3) {
		    	QObject object = new DeploymentFileWriter();
		    	object.disposeLater();
		    	QApplication.exec();
	    	}else {
	    		throw new RuntimeException("Two arguments expected.");
	    	}
    	}finally {
    		QApplication.shutdown();
    	}
	}
	@Override
	public boolean event(QEvent event) {
		if(event.type()==QEvent.Type.DeferredDispose) {
			QFile file = new QFile(QApplication.arguments().get(1));
			if(file.open(QIODevice.OpenModeFlag.WriteOnly)) {
				file.write(new QByteArray(QApplication.arguments().get(2)));
				file.close();
			}
			file.dispose();
			QApplication.quit();
		}
		return super.event(event);
	}
}