/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2020 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
package io.qt.internal;

import java.util.logging.Level;

import io.qt.QtUninvokable;
import io.qt.core.QAbstractEventDispatcher;
import io.qt.core.QCoreApplication;
import io.qt.core.QEvent;

class QtJambi_LibraryShutdown extends Thread {
	
	private final static QtJambi_LibraryShutdown instance = new QtJambi_LibraryShutdown();
	static {
		Runtime.getRuntime().addShutdownHook(instance);
	}
	
	private final java.util.logging.Logger logger = java.util.logging.Logger.getLogger("io.qt.internal");

    @QtUninvokable
	public void run() {
    	logger.log(Level.FINE, "Begin QtJambi_LibraryShutdown...");
        try {
            NativeLibraryManager.resetActiveDeploymentSpec();

            QCoreApplication app = QCoreApplication.instance();

            if(app != null) {
                Object appThread = app.thread();
                QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());  // allow deleteLater() to work some magic
                QCoreApplication.addPostRoutine(()->{
                	Runtime runtime = Runtime.getRuntime();
    	            runtime.gc();
    	            runtime.runFinalization();
    	            logger.log(Level.FINE, "performing native library shutdown");
    	            shutdown();
                });
                QCoreApplication.quit();

                try {
                    QCoreApplication.processEvents();
                    QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());  // allow deleteLater() to work some magic
                    // A fixed time of 500ms is not really ideal.  Better to allow some
                    //  query/configuration include infinite timeout.  Also to warn to
                    //  console when we know the main GUI thread did not terminate on time
                    //  so the resulting errors/crashes can be explained on shutdown.
                    if(appThread != null)
                    	QtJambiThreadUtility.castToThread(appThread).join(500);
                } catch(Throwable e) {
                	logger.log(java.util.logging.Level.SEVERE, "", e);
                } finally {
                    if(appThread == null || QAbstractEventDispatcher.instance(QtJambiThreadUtility.castToThread(appThread))==null) {
                    	logger.log(Level.FINE, "disposing QCoreApplication instance");
                        app.dispose();		// push the delete matter
                    }
                }
                appThread = null;	 // kill hard-reference
                app = null;		// kill hard-reference
            }else {
	            Runtime runtime = Runtime.getRuntime();
	            runtime.gc();
	            runtime.runFinalization();
	            logger.log(Level.FINE, "performing native library shutdown");
	            shutdown();
            }
        } catch(Throwable e) {
        	logger.log(java.util.logging.Level.SEVERE, "", e);
        }finally {
        	logger.log(Level.FINE, "QtJambi_LibraryShutdown finished.");
        }
    }
    
    private QtJambi_LibraryShutdown(){
    	setName("QtJambi_LibraryShutdown");
    }
    
    @QtUninvokable
    private static native void shutdown();

    @QtUninvokable
	public static void initialize() {
	}
}
