/****************************************************************************
**
** Copyright (C) 2009-2024 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import java.util.List;

import static org.junit.Assert.*;

import org.junit.AfterClass;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.core.QLibraryInfo;
import io.qt.core.QOperatingSystemVersion;
import io.qt.core.QTimer;
import io.qt.core.Qt.*;
import io.qt.gui.QGuiApplication;
import io.qt.gui.QWindow;
import io.qt.keyboard.*;
import io.qt.keyboard.QVirtualKeyboardInputEngine.*;

public class TestVirtualKeyboard extends ApplicationInitializer {
	
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
    public void test() {
    	QVirtualKeyboardInputContext context = new QVirtualKeyboardInputContext();
    	QVirtualKeyboardInputEngine engine = context.inputEngine();
    	QVirtualKeyboardAbstractInputMethod inputMethod = new QVirtualKeyboardAbstractInputMethod() {
			@Override
			public boolean setTextCase(TextCase textCase) {
				return false;
			}
			
			@Override
			public boolean setInputMode(String locale, InputMode inputMode) {
				return false;
			}
			
			@Override
			public boolean keyEvent(Key key, String text, KeyboardModifiers modifiers) {
				return false;
			}
			
			@Override
			public List<InputMode> inputModes(String locale) {
				return null;
			}
		};
    	engine.setInputMethod(inputMethod);
    	assertEquals(null, inputMethod.parent());
    }
    
}
