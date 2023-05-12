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

import static org.junit.Assert.fail;

import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.QThreadAffinityException;
import io.qt.core.QEventLoop;
import io.qt.core.QThread;
import io.qt.core.QUrl;
import io.qt.gui.QSurfaceFormat;
import io.qt.quick.QQuickItem;
import io.qt.quick.QQuickView;

public class TestQuickThreadAffinity extends ApplicationInitializer{
	
	static {
		System.setProperty("io.qt.enable-thread-affinity-check", "true");
		System.setProperty("io.qt.enable-event-thread-affinity-check", "true");
	}
	
	@BeforeClass
    public static void testInitialize() throws Exception {
    	ApplicationInitializer.testInitializeWithGui();
    }
	
	private Throwable throwable;
	
	@Test
    public void testQQuick() throws Throwable{
		throwable = null;
		QQuickItem item = new QQuickItem();
		QQuickView component = new QQuickView();
		component.setObjectName("testQuickView");
	    QSurfaceFormat format = component.format();
	    format.setSamples(8);
	    component.setFormat(format);
		QEventLoop loop = new QEventLoop();
		QThread thread = QThread.create(()->{
			try{
				new QQuickItem(item);
				fail("QThreadAffinityException expected to be thrown.");
			}catch(QThreadAffinityException e) {}
			try{
				QQuickItem item2 = new QQuickItem();
				item2.setParentItem(item);
				fail("QThreadAffinityException expected to be thrown.");
			}catch(QThreadAffinityException e) {}
			try{
			    component.setSource(QUrl.fromLocalFile(":io/qt/autotests/qml/RectangleMethodAndSignal.qml"));
				fail("QThreadAffinityException expected to be thrown.");
			}catch(QThreadAffinityException e) {}
		});
		thread.setUncaughtExceptionHandler((Thread t, Throwable e)->throwable = e);
		thread.finished.connect(loop::quit);
		thread.start();
		loop.exec();
		if(throwable!=null)
			throw throwable;
	}
	
    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestQuickThreadAffinity.class.getName());
    }
}
