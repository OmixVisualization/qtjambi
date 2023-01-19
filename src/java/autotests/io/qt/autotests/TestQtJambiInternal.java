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

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

import org.junit.BeforeClass;

import io.qt.autotests.generated.General;
import io.qt.core.QFile;
import io.qt.core.QObject;
import io.qt.gui.QImage;

public class TestQtJambiInternal extends UnitTestInitializer {
	
	@BeforeClass
    public static void testInitialize() throws Exception {
    	ApplicationInitializer.testInitializeWithWidgets();
    }
	
	private FetchableTestClass ftc;
	private QImage image;

	@org.junit.AfterClass
	public static void testDispose() throws Exception {
		ApplicationInitializer.testDispose();
	}

	@org.junit.Before
	public void setUp() {
		ftc = new FetchableTestClass(new QObject());
		image = new QImage();
	}

	@org.junit.After
	public void tearDown() {
		ftc = null;
		image = null;
	}

	@org.junit.Test
	public void testFetchField() {
		/*
		 * Access the class variable with it's getter method, then with
		 * GeneratorUtilities.fetchfield(QObject, Class, String); Compare the
		 * references.
		 */
		assertTrue("fakeVar1", ftc.getFakeVar1().equals(General.internalAccess.readField(ftc, FetchableTestClass.class, "fakeVar1", QObject.class)));
	}

	@org.junit.Test
	public void testSetField() {
		General.internalAccess.writeField(ftc, FetchableTestClass.class, "str1", "some text");
		assertEquals("ftc.getStr1()", "some text", ftc.getStr1());
	}

	@org.junit.Test
	public void testCountExpense() {
		final String path = ":/qt-project.org/qmessagebox/images/qtlogo-64.png";
		QFile qfile = new QFile(path);
		boolean bf = qfile.exists();
		assertTrue(bf);

		image.load(path);
		assertEquals("height*bytesPerLine", 4096, image.height() * image.bytesPerLine());
		qfile.dispose();
	}

	// TODO implement testCreateExtendedEnum(), example from
	// QAbstractFileEngine.java
	/*
	 * @org.junit.Test public void testCreateExtendedEnum() { }
	 */

}

class FetchableTestClass {

	private QObject fakeVar1;
	private String str1;

	public FetchableTestClass(QObject fakeVar1) {
		this.fakeVar1 = fakeVar1;
	}

	public QObject getFakeVar1() {
		return fakeVar1;
	}

	public String getStr1() {
		return str1;
	}

}
