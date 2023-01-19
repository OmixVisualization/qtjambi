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

import static org.junit.Assert.assertNull;
import static org.junit.Assert.assertTrue;

import org.junit.Assume;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.core.QRect;
import io.qt.core.QSize;
import io.qt.gui.QGuiApplication;
import io.qt.widgets.QApplication;
import io.qt.widgets.QLayout;
import io.qt.widgets.QLayoutItem;
import io.qt.widgets.QVBoxLayout;
import io.qt.widgets.QWidget;

public class TestNullPointers extends ApplicationInitializer {
	@BeforeClass
	public static void testInitialize() throws Exception {
    	testInitializeWithWidgets();
    }
	
    @Test
    public void testBoxLayoutAddWidget() {
        QVBoxLayout layout = new QVBoxLayout();

        Exception e = null;
        try {
            layout.addWidget(null);
        } catch (Exception f) {
            e = f;
        }

        assertTrue(e instanceof NullPointerException);
    }

    static class MyLayout extends QLayout {
        @Override
        public QLayoutItem itemAt(int arg__1) {
            return null;
        }

        @Override
        public void addItem(QLayoutItem arg__1) {
            // TODO Auto-generated method stub

        }

        @Override
        public int count() {
            // TODO Auto-generated method stub
            return 0;
        }

        @Override
        public void setGeometry(QRect arg__1) {
            // TODO Auto-generated method stub

        }

        @Override
        public QSize sizeHint() {
            // TODO Auto-generated method stub
            return null;
        }

        @Override
        public QLayoutItem takeAt(int index) {
            return null;
        }
    }

    @Test
    public void testBlah() {
    	Assume.assumeTrue("A screen is required to create a window.", QGuiApplication.primaryScreen()!=null);
        QWidget w = new QWidget();

        MyLayout l = new MyLayout();
        w.setLayout(l);
        l.addWidget(new QWidget());
        assertNull(l.takeAt(0));	// eclEMMA

        w.show();

        QApplication.processEvents();
    }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestNullPointers.class.getName());
    }
}
