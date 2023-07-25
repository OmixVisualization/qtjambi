/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
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

import org.junit.BeforeClass;

import io.qt.core.QCoreApplication;
import io.qt.core.QEventLoop;
import io.qt.core.QThread;
import io.qt.core.QThreadPool;
import io.qt.core.QTimer;

public class TestQThreadPool extends ApplicationInitializer {
	@BeforeClass
	public static void testInitialize() throws Exception {
		ApplicationInitializer.testInitialize();
    }
	
	@org.junit.Test
	public void test(){
		QThreadPool pool = new QThreadPool();
		pool.setMaxThreadCount(100);
		pool.start(()->{
			QEventLoop loop = new QEventLoop();
			QTimer timer = new QTimer();
			int[] counter = {0};
			timer.timeout.connect(()->{
				System.out.println("timeout "+counter[0]+" "+Thread.currentThread());
				if(++counter[0]==50)
					loop.quit();
			});
			timer.setSingleShot(false);
			timer.setInterval(200);
			timer.start();
			loop.exec();
			QCoreApplication.quit();
		});
		QCoreApplication.exec();
	}
}
