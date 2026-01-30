/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import java.util.ArrayList;
import java.util.List;

import org.junit.Assume;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.Nullable;
import io.qt.autotests.generated.General;
import io.qt.core.QCoreApplication;
import io.qt.core.QEvent;
import io.qt.core.QLogging;
import io.qt.core.QObject;
import io.qt.core.QSize;
import io.qt.core.QThread;
import io.qt.core.QTimer;
import io.qt.widgets.QApplication;
import io.qt.widgets.QMainWindow;

public class TestTerminate extends ApplicationInitializer {
	
	@BeforeClass
    public static void testInitialize() throws Exception {
    	ApplicationInitializer.testInitializeWithWidgets();
    	General.canCreateVulkanInstance();
    	QLogging.qInstallMessageHandler((t, c, m)->System.out.println(m));
    }
	
	void crash(int terminationMode) {
		switch(terminationMode) {
    	case 1:
    		General.std_terminate();
    		break;
    	case 2:
    		General.std_abort();
    		break;
    	case 3:
    		General.std_exit(1);
    		break;
    	case 4:
    		General.c_kill();
    		break;
    	case 5:
    		General.c_abort();
    		break;
    	case 6:
    		General.c_exit(1);
    		break;
    	case 7:
    		General.badAccess();
    		break;
    	case 8:
    		General.badVirtual();
    		break;
    	case 9:
    		General.uncaughtException();
    		break;
    	}
	}
	
    @Test
    public void test() {
    	int terminationMode = Integer.getInteger("termination.mode", 0);
    	Assume.assumeTrue(terminationMode!=0);
    	QMainWindow mainWindow = new QMainWindow();
    	List<QObject> objects = new ArrayList<>();
    	List<QThread> threads = new ArrayList<>();
    	long t1 = System.currentTimeMillis();
    	for (int i = 0; i < 25; i++) {
    		QObject object = new QObject(){
				@Override
				public boolean event(@Nullable QEvent event) {
					for (int j = 0; j < 10; j++) {
						new QSize();
						QSize s = new QSize();
						s.dispose();
					}
					if(System.currentTimeMillis()-t1>5000) {
	            		System.gc();
						crash(terminationMode);
					}
					QCoreApplication.postEvent(this, new QEvent(QEvent.Type.Close));
					return super.event(event);
				}
    		};
    		objects.add(object);
			QThread thread = new QThread();
			thread.setObjectName("QThread-Worker-"+i);
			thread.setName("QThread-Worker-"+i);
			thread.setDaemon(true);
			thread.start();
			threads.add(thread);
			object.moveToThread(thread);
    		QCoreApplication.postEvent(object, new QEvent(QEvent.Type.Close));
		}
    	try {
        	QApplication.instance().aboutToQuit.connect(()->{
        		crash(terminationMode);
        	});
        	QTimer.singleShot(15000, mainWindow::close);
        	mainWindow.show();
        	QApplication.exec();
    	}finally {
    		for (QThread thread : threads) {
    			thread.quit();
			}
    	}
    }
    
    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestTerminate.class.getName());
    }
}
