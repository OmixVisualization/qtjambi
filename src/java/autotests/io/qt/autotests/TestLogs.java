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

import java.util.logging.Handler;
import java.util.logging.Logger;

import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.core.QLogging;
import io.qt.core.QSize;

public class TestLogs extends UnitTestInitializer{
	@BeforeClass
	public static void testInitialize() throws Exception {
		System.setProperty("io.qt.enable-method-logs", "true");
		System.setProperty("io.qt.enable-cleanup-logs", "true");
		Logger rootLogger = Logger.getLogger("");
		for (Handler h : rootLogger.getHandlers()) {
		    rootLogger.removeHandler(h);
		}
		rootLogger.addHandler(new QLogging.Handler());
		QLogging.qInstallMessageHandler((type,context,message)->{
			System.out.println(message);
		});
//		ApplicationInitializer.testInitialize();
	}
	
	@Test
    public void test() {
		QSize size = new QSize();
		size.dispose();
	}
}
