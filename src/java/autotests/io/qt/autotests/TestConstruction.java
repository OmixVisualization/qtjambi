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

import static org.junit.Assert.*;

import java.lang.ref.WeakReference;

import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.core.QEvent;
import io.qt.core.QObject;
import io.qt.widgets.QMdiArea;
import io.qt.widgets.QMdiSubWindow;
import io.qt.widgets.QWidget;

public class TestConstruction extends ApplicationInitializer {
	
	@BeforeClass
    public static void testInitialize() throws Exception {
    	ApplicationInitializer.testInitializeWithWidgets();
    }
	
    @Test
    public void testQObjectConversionInConstructor() {
    	QObject[] array = {null};
        QMdiArea area = new QMdiArea() {
            @Override
            public boolean eventFilter(QObject o, QEvent e) {
            	array[0] = o;
            	return false;
            }
        };
        instances.add(new WeakReference<>(area));
        
        QMdiSubWindow subWindow = area.addSubWindow(new QWidget());
        // Although it seems unlikely, this may actually be false, trust me.
        try{
        	assertFalse(array[0] instanceof QMdiSubWindow);
        	assertTrue(subWindow instanceof QMdiSubWindow);
        }finally {
        	area.hide();
        	area.dispose();
        }
    }
    
    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestConstruction.class.getName());
    }
}
