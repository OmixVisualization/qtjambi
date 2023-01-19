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

import static org.junit.Assert.*;

import java.io.File;
import java.io.InputStream;
import java.net.URL;
import java.nio.file.Files;
import java.nio.file.StandardCopyOption;

import org.junit.AfterClass;
import org.junit.Assert;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.QtUtilities;
import io.qt.core.QObject;
import io.qt.core.QResource;
import io.qt.core.QUrl;
import io.qt.gui.QGuiApplication;
import io.qt.qml.QQmlApplicationEngine;

public class TestQmlMultimedia extends ApplicationInitializer {
	
	private static File tmpFile;
	
	@AfterClass
    public static void testDispose() throws Exception {
		ApplicationInitializer.testDispose();
		if(tmpFile!=null) {
			QResource.removeClassPath(tmpFile.getAbsolutePath());
			tmpFile.delete();
		}
	}

    @BeforeClass
    public static void testInitialize() throws Exception {
        ApplicationInitializer.testInitializeWithGui();
        URL url = TestQmlMultimedia.class.getResource("resources.jar");
        if(url.getProtocol().equals("file")) {
        	QResource.addClassPath(new java.io.File(url.toURI()).getAbsolutePath());
        }else {
        	tmpFile = File.createTempFile("resources", ".jar");
        	try (InputStream is = url.openStream()){
        		Files.copy(is, tmpFile.toPath(), StandardCopyOption.REPLACE_EXISTING);
        	}
        	QResource.addClassPath(tmpFile.getAbsolutePath());
        }
    }
    
	@Test
	public void test() {
		QQmlApplicationEngine applicationEngine = new QQmlApplicationEngine();
		io.qt.QtUtilities.initializePackage("io.qt.quick");
		io.qt.QtUtilities.initializePackage("io.qt.multimedia");
		io.qt.QtUtilities.loadQtLibrary("MultimediaQuick");
		if(QtUtilities.qtjambiVersion().majorVersion()==5) {
			io.qt.QtUtilities.loadQtLibrary("Widgets");
			io.qt.QtUtilities.loadQtLibrary("Network");
		}
		Object[] objectCreated = {null,null};
		applicationEngine.objectCreated.connect((object, url)->{objectCreated[0] = object; objectCreated[1] = url;}); // must not crash!
		applicationEngine.load(new QUrl("qrc:/io/qt/qtjambi/resources/multimedia"+QtUtilities.qtjambiVersion().majorVersion()+".qml"));
		Assert.assertFalse(applicationEngine.rootObjects().isEmpty());
		assertTrue(objectCreated[0] instanceof QObject);
		assertTrue(objectCreated[1] instanceof QUrl);
		QGuiApplication.exec();
		Assert.assertEquals(Boolean.TRUE, applicationEngine.rootObjects().get(0).property("wasPlaying"));
	}

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestQmlMultimedia.class.getName());
    }
}
