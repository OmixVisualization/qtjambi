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

import java.util.*;
import java.util.logging.Handler;
import java.util.logging.Logger;

import org.junit.AfterClass;
import org.junit.Assert;
import org.junit.Assume;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.QtUtilities;
import io.qt.core.QCoreApplication;
import io.qt.core.QLogging;
import io.qt.core.QOperatingSystemVersion;
import io.qt.core.QSize;
import io.qt.core.QtMessageHandler;

public class TestLogs extends UnitTestInitializer{

	private static final Set<String> messages = new HashSet<>();
	private static List<Handler> handlers;
	private static Object replace;
	private static Handler logHandler; 
	
	@BeforeClass
	public static void testInitialize() throws Exception {
		System.setProperty("io.qt.enable-method-logs", "true");
		System.setProperty("io.qt.enable-cleanup-logs", "true");
		Logger rootLogger = Logger.getLogger("");
		List<Handler> handlers = new ArrayList<>();
		for (Handler h : rootLogger.getHandlers()) {
			handlers.add(h);
		    rootLogger.removeHandler(h);
		}
		Assume.assumeFalse("Does not work on Android batched tests", QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android) || QCoreApplication.instance()==null);
		TestLogs.handlers = handlers;
		logHandler = new QLogging.Handler();
		rootLogger.addHandler(logHandler);
		replace = QLogging.qInstallMessageHandler((type,context,message)->{
//			System.out.println(message);
			messages.add(message);
		});
	}
	
	@AfterClass
	public static void testShutdown() throws Exception {
		QLogging.qInstallMessageHandler((QtMessageHandler)replace);
		QtUtilities.setMethodLogsEnabled(false);
		Logger rootLogger = Logger.getLogger("");
		rootLogger.removeHandler(logHandler);
		if(handlers!=null) {
			for (Handler h : handlers) {
				rootLogger.addHandler(h);
			}
		}
	}
	
	@Test
    public void test() {
		QSize size = new QSize();
		int hashCode = System.identityHashCode(size);
		size.dispose();
		Assert.assertTrue(messages.contains(String.format("Begin dispose of 0x%1$s@io.qt.core.QSize...", Integer.toHexString(hashCode))));
		Assert.assertTrue(messages.contains(String.format("Dispose of 0x%1$s@io.qt.core.QSize finished.", Integer.toHexString(hashCode))));
	}
}
