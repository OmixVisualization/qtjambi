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

import org.junit.*;

import io.qt.core.*;
import io.qt.gui.QGuiApplication;
import io.qt.positioning.*;

public class TestPositioning extends ApplicationInitializer{
	
	@BeforeClass
    public static void testInitialize() throws Exception {
		QGuiApplication.setDesktopFileName("QtJambi");
    	ApplicationInitializer.testInitializeWithWidgets();
	}
	
	@AfterClass
    public static void testDispose() throws Exception {
    	ApplicationInitializer.testDispose();
    }
	
	@Test
    public void test() throws InterruptedException {
		System.out.println("QGeoPositionInfoSource.availableSources: "+QGeoPositionInfoSource.availableSources());
		QGeoPositionInfo result = new QGeoPositionInfo();
		Assert.assertFalse(result.isValid());
		for(String name : QGeoPositionInfoSource.availableSources()) {
			QGeoPositionInfoSource source = QGeoPositionInfoSource.createSource(name, null);
			if(source!=null) {
				try {
					source.errorOccurred.connect(e->System.out.println(name+" "+e));
					source.positionUpdated.connect(info->{
						if(info.isValid()) {
							result.assign(info);
							QCoreApplication.quit();
						}
					});
					source.startUpdates();
					QTimer.singleShot(5000, QCoreApplication::quit);
					QCoreApplication.exec();
				}finally {
					source.stopUpdates();
					source.dispose();
				}
			}else {
				System.out.println("no source available for "+name);
			}
			if(result.isValid())
				break;
		}
//		Assert.assertTrue(result.isValid());
	}

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestPositioning.class.getName());
    }
}
