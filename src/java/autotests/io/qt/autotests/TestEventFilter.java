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

import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.core.QEvent;
import io.qt.core.QObject;
import io.qt.core.QTimer;
import io.qt.gui.QGuiApplication;
import io.qt.widgets.QApplication;
import io.qt.widgets.QWidget;

public class TestEventFilter extends ApplicationInitializer {
	
	@BeforeClass
	public static void testInitialize() throws Exception {
		ApplicationInitializer.testInitializeWithWidgets();
    }
    
    @Test
    public void test() {
    	// must not lead to crash
    	QGuiApplication.instance().sessionId();
    	QGuiApplication.instance().installEventFilter(new QObject(QGuiApplication.instance()){
    		@Override
    		public boolean eventFilter(QObject obj, QEvent evt) {
    			return false;
    		}
    	});
    	QWidget widget = new QWidget();
    	widget.show();
    	QTimer.singleShot(1000, QApplication::quit);
    	QApplication.exec();
    	widget.dispose();
    }
    
    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestEventFilter.class.getName());
    }
}
