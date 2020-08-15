/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2020 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import java.lang.ref.WeakReference;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

import org.hamcrest.BaseMatcher;
import org.hamcrest.Description;
import org.hamcrest.Matcher;
import org.junit.AfterClass;
import org.junit.BeforeClass;

import io.qt.QNoNativeResourcesException;
import io.qt.QtObjectInterface;
import io.qt.core.QCoreApplication;
import io.qt.core.QEvent;
import io.qt.core.QMessageHandler;
import io.qt.core.QThread;
import io.qt.internal.QtJambiDebugTools;
import io.qt.internal.QtJambiInternal;
import io.qt.widgets.QApplication;

public abstract class QApplicationTest {
	static {
		io.qt.QtUtilities.initializePackage("io.qt.autotests.generated");
	}
    
    public static Matcher<Boolean> trueMatcher(final String message) {
		return new BaseMatcher<Boolean>() {
			//@Override // Java6 only
			public boolean matches(final Object actualObject) {
				return Boolean.TRUE.equals(actualObject); 
			}
			//@Override // Java6 only
			public void describeTo(final Description description) {
				description.appendText(message);
			}
			//@Override // Java6 only
			public void describeMismatch(final Object actualObject, final Description description) {
				description.appendText(message);
			}
		};
	}
	
	protected static final List<WeakReference<QtObjectInterface>> instances = Collections.synchronizedList(new ArrayList<>());
	
    public static class MyQMessageHandler extends QMessageHandler {
        @Override
        public void debug(String message) {
            System.out.println("DEBUG: " + message);
            System.out.flush();
        }

        @Override
        public void warning(String message) {
            System.out.println("WARN: " + message);
            System.out.flush();
        }

        @Override
        public void critical(String message) {
            System.out.println("CRIT: " + message);
            System.out.flush();
        }

        @Override
        public void fatal(String message) {
            System.out.println("FATAL: " + message);
            System.out.flush();
        }
    }

    private static QMessageHandler myQMessageHandler;

    @BeforeClass
	public static void testInitialize() throws Exception {
        try {
			if(QCoreApplication.instance()==null) {
				Utils.println(2, "QApplicationTest.testInitialize(): begin");
			    io.qt.QtResources.addSearchPath(".");
			    QCoreApplication.setApplicationName("QtJambiUnitTest");
			    QApplication.initialize(new String[]{"arg1", "arg2", "arg3"});
			    Utils.println(2, "QApplicationTest.testInitialize(): done");
		        QThread.currentThread().setObjectName("main");
			}else if(myQMessageHandler==null) {
				Utils.println(2, "QApplicationTest.testInitialize(): begin");
			    myQMessageHandler = new MyQMessageHandler();
			    QMessageHandler.installMessageHandler(myQMessageHandler);        		
			    Utils.println(2, "QApplicationTest.testInitialize(): done");
			}
		} catch (Throwable e) {
			e.printStackTrace();
			throw e;
		}
    }

    @AfterClass
    public static void testDispose() throws Exception {
    	try {
    		while(!instances.isEmpty()) {
    			WeakReference<QtObjectInterface> weak = instances.remove(0);
    			QtObjectInterface o = weak.get();
    			if(o!=null && !o.isDisposed())
    				o.dispose();
    		}
	        Utils.println(2, "QApplicationTest.testDispose(): BEGIN");
	        System.gc();
	        System.runFinalization();
	
	        // We are attempting to reach a point here where memory should be reclaimed
	        // except for anything linked to QApplication.
	
	        QApplication app = QApplication.instance();
	        try {
	            Utils.println(3, "QApplicationTest.testDispose(): QApplication-only  app="+app);
	        } catch(QNoNativeResourcesException e) {
	            Utils.println(3, "QApplicationTest.testDispose(): QApplication-only  io.qt.QNoNativeResourcesException: app="+e.getMessage());
	        }
	
	        System.err.flush();
	        System.out.flush();
	
	        if(app != null) {
	            try {
	        		Utils.println(3, "QApplicationTest.testDispose(): processEvents() 1 PRE");
	                QApplication.processEvents();	// NPE
	        		Utils.println(3, "QApplicationTest.testDispose(): processEvents() 1 POST");
	            } catch(Throwable t) {
	                t.printStackTrace();
	            }
	    		Utils.println(3, "QApplicationTest.testDispose(): sendPostedEvents(DeferredDelete) PRE");
	            QApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	    		Utils.println(3, "QApplicationTest.testDispose(): sendPostedEvents(DeferredDelete) POST");
	        }
    		Utils.println(3, "QApplicationTest.testDispose(): QApplication.quit() PRE");
//	        QApplication.quit();
    		Utils.println(3, "QApplicationTest.testDispose(): QApplication.quit() POST");
	        app = QApplication.instance();
	        if(app != null) {
				Utils.println(3, "QApplicationTest.testDispose(): garbage PRE");
		        for (int i = 0; i < 4; i++) {
		        	System.gc();
		            System.runFinalization();
		            Thread.sleep(50);
		            QApplication.processEvents();
		            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
		        	System.gc();
		            System.runFinalization();
				}
		        while(QtJambiDebugTools.hasDeleteLaterEvents()) {
		            QApplication.processEvents();
		            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());	        	
		        }
				Utils.println(3, "QApplicationTest.testDispose(): garbage POST");
		        
	    		Utils.println(3, "QApplicationTest.testDispose(): processEvents() 2 PRE");
	            QApplication.processEvents();
	    		Utils.println(3, "QApplicationTest.testDispose(): processEvents() 2 POST");
	    		Utils.println(3, "QApplicationTest.testDispose(): processEvents() 3 PRE");
	            QApplication.processEvents(/*QEventLoop.ProcessEventsFlag.DeferredDeletion*/);
	    		Utils.println(3, "QApplicationTest.testDispose(): processEvents() 3 POST");
	    		Utils.println(3, "QApplicationTest.testDispose(): QApplication.dispose() PRE");
	            app.dispose();
	    		Utils.println(3, "QApplicationTest.testDispose(): QApplication.dispose() POST");
	            try {
	            	if(QtJambiInternal.nativeId(app)==0)
	            		Utils.println(3, "QApplicationTest.testDispose(): done  app="+app.getClass()+"@"+app.hashCode());
	            	else
	            		Utils.println(3, "QApplicationTest.testDispose(): not done  app="+app);
	            } catch(QNoNativeResourcesException e) {
	                Utils.println(3, "QApplicationTest.testDispose(): done  io.qt.QNoNativeResourcesException: app="+e.getMessage());
	            }
	            app = null;		// kill hard-reference
	        }
	        if(myQMessageHandler != null) {
	    		Utils.println(3, "QApplicationTest.testDispose(): QMessageHandler.removeMessageHandler() PRE");
	            QMessageHandler.removeMessageHandler(myQMessageHandler);
	    		Utils.println(3, "QApplicationTest.testDispose(): QMessageHandler.removeMessageHandler() POST");
	            myQMessageHandler = null;
	        }
			Utils.println(3, "QApplicationTest.testDispose(): garbage PRE");
	        System.gc();
	        System.runFinalization();
	        QApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	        System.gc();
	        System.runFinalization();
	        QApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	        Thread.sleep(50);
			Utils.println(3, "QApplicationTest.testDispose(): garbage POST");
	
	        // We are attempting to reach a point here where memory should be reclaimed
	        // including that which was linked to QApplication
	
	        Utils.println(3, "QApplicationTest.testDispose(): shutdown PRE");
	        QApplication.shutdown();
	        Utils.println(3, "QApplicationTest.testDispose(): shutdown POST");
	        
	        System.gc();
	        System.runFinalization();
	        QApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	        Thread.sleep(50);
	        
	        QtJambiDebugTools.flushOut();  // fflush(stdout)
	        QtJambiDebugTools.flushErr();  // fflush(stderr)
	        int objectCount = QtJambiDebugTools.objectCount();  // QtJambiLink::QtJambiLink_dump()
	        QtJambiDebugTools.flushErr();  // fflush(stderr)
	        Utils.println(3, "QApplicationTest.testDispose(): end objectCount="+objectCount);
	
	        if(objectCount == 0)
	            return;  // optimization due to class loading causing some references to be set
	
	        System.gc();
	        System.runFinalization();
	        QApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
	        QApplication.processEvents();
	        System.gc();
	        System.runFinalization();
	        Thread.sleep(50);
	
	        QtJambiDebugTools.flushOut();  // fflush(stdout)
	        QtJambiDebugTools.flushErr();  // fflush(stderr)
	        objectCount = QtJambiDebugTools.objectCount();  // QtJambiLink::QtJambiLink_dump()
	        QtJambiDebugTools.flushErr();  // fflush(stderr)
	        Utils.println(3, "QApplicationTest.testDispose(): end objectCount="+objectCount);
    	}finally {
            Utils.println(2, "QApplicationTest.testDispose(): DONE");
    	}
    }
}
