/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
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

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

import java.util.concurrent.atomic.AtomicInteger;

import org.junit.Assert;
import org.junit.Assume;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.autotests.generated.General;
import io.qt.core.*;
import io.qt.gui.*;
import io.qt.widgets.*;

public class TestReferenceCountingQButtonGroup extends ApplicationInitializer{
	
	@BeforeClass
    public static void testInitialize() throws Exception {
        ApplicationInitializer.testInitializeWithWidgets();
		Assume.assumeTrue("A screen is required to create a window.", QGuiApplication.primaryScreen()!=null);
    }
	
	private static final int COUNT = 200;
	
	static void uncalled(){
		QGuiApplication.platformName();
	}
	
	@Test public void testButtonsNull() {
        QButtonGroup g = new QButtonGroup();
        try {
        	g.addButton((QPushButton)null);
        	Assert.fail("NullPointerException expected to be thrown");
        }catch(NullPointerException t) {}
        try {
        	g.removeButton(null);
	    	Assert.fail("NullPointerException expected to be thrown");
	    }catch(NullPointerException t) {}
	}
	
	@Test public void testAddButtonTwice() throws InterruptedException {
		AtomicInteger counter = new AtomicInteger();
		QButtonGroup g = new QButtonGroup();
        for (int i=0; i<COUNT; ++i) {
            QPushButton act = new QPushButton("button" + i, null);
            General.internalAccess.registerCleaner(act, counter::incrementAndGet);
            g.addButton(act);
            g.addButton(act);
            QApplication.processEvents();
            act = null;
        }
        for (int i = 0; i < 20 && counter.get()==0; i++) {
            ApplicationInitializer.runGC();
            synchronized(getClass()) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(0, counter.get());
        QList<QAbstractButton> buttons = g.buttons();
        assertEquals(COUNT, buttons.size());
        for (int i=0; i<COUNT; ++i) {
        	QAbstractButton button = buttons.removeFirst();
            assertTrue(button != null);
            assertEquals("button" + i, button.text());
            g.removeButton(button);
            assertTrue("QPushButton does not have java ownership", General.internalAccess.isJavaOwnership(button));
            button = null;
            QApplication.processEvents();
        }
        buttons = null;
        for (int i = 0; i < 120 && counter.get()<COUNT; i++) {
            ApplicationInitializer.runGC();
            synchronized(getClass()) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        assertEquals(0, g.buttons().size());
        Assert.assertEquals(COUNT, counter.get());
	}
	
	@Test public void testAddAndRemoveButton() throws InterruptedException {
		AtomicInteger counter = new AtomicInteger();
		QButtonGroup g = new QButtonGroup();
        for (int i=0; i<COUNT; ++i) {
            QPushButton act = new QPushButton("button" + i, null);
            General.internalAccess.registerCleaner(act, counter::incrementAndGet);
            g.addButton(act);
            QApplication.processEvents();
            act = null;
        }
        for (int i = 0; i < 20 && counter.get()==0; i++) {
            ApplicationInitializer.runGC();
            synchronized(getClass()) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        Assert.assertEquals(0, counter.get());
        QList<QAbstractButton> buttons = g.buttons();
        assertEquals(COUNT, buttons.size());
        for (int i=0; i<COUNT; ++i) {
        	QAbstractButton button = buttons.removeFirst();
            assertTrue(button != null);
            assertEquals("button" + i, button.text());
            g.removeButton(button);
            assertTrue("QPushButton does not have java ownership", General.internalAccess.isJavaOwnership(button));
            button = null;
            QApplication.processEvents();
        }
        buttons = null;
        for (int i = 0; i < 120 && counter.get()<COUNT; i++) {
            ApplicationInitializer.runGC();
            synchronized(getClass()) {
            	Thread.sleep(25);
            }
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose.value());
            QCoreApplication.processEvents();
		}
        assertEquals(0, g.buttons().size());
        Assert.assertEquals(COUNT, counter.get());
	}
}
