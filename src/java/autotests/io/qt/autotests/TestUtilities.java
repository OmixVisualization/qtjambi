/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import org.junit.AfterClass;
import org.junit.Assert;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.QLibraryNotFoundError;
import io.qt.QtUtilities;
import io.qt.autotests.generated.General;
import io.qt.core.QLibraryInfo;
import io.qt.core.QObject;
import io.qt.core.QOperatingSystemVersion;
import io.qt.core.QTimer;
import io.qt.core.Qt;
import io.qt.gui.QAction;
import io.qt.gui.QGuiApplication;
import io.qt.gui.QKeySequence;
import io.qt.gui.QWindow;

public class TestUtilities extends UnitTestInitializer {

	private static boolean loadQtJambiLibrary(String lib) {
        try {
            QtUtilities.loadQtJambiLibrary(lib);
            return true;
        } catch (UnsatisfiedLinkError | QLibraryNotFoundError e) {
            java.util.logging.Logger.getLogger("io.qt").log(java.util.logging.Level.SEVERE, "", e);
            return false;
        }
    }

	@org.junit.Test
	public void testLoadLibrary() {
		assertTrue("validLibrary", loadQtJambiLibrary("Core"));
		assertFalse("falseLibrary", loadQtJambiLibrary("Kore"));
	}
    
    @Test
    public void testNullClass() {
    	Assert.assertEquals(null, General.internalAccess.getClass(null));
    }
	
	@BeforeClass
    public static void testInitialize() throws Exception {
    	ApplicationInitializer.testInitializeWithGui();
    }
	
	@AfterClass
    public static void testDispose() throws Exception {
		if(QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.MacOS) 
    			&& QLibraryInfo.version().majorVersion()==6 
    			&& QLibraryInfo.version().minorVersion()==5) {
	    	QWindow window = new QWindow();
	    	window.show();
	    	QTimer.singleShot(200, QGuiApplication.instance(), QGuiApplication::quit);
	    	QGuiApplication.exec();
	    	window.close();
	    	window.disposeLater();
	    	window = null;
    	}
    	ApplicationInitializer.testDispose();
    }
	
    @Test
    public void testShortcut() {
        QObject obj = new QObject();
        QAction act = new QAction(obj);
        act.setShortcut("Ctrl+A");
        QKeySequence seq = act.shortcut();

        assertEquals(seq.count(), 1);
        assertEquals(seq.at(0).toCombined(), Qt.KeyboardModifier.ControlModifier.value() | Qt.Key.Key_A.value());
    }
}
