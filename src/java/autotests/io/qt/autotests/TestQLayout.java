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

import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.widgets.QGraphicsGridLayout;
import io.qt.widgets.QGraphicsWidget;
import io.qt.widgets.QGridLayout;
import io.qt.widgets.QWidget;

public class TestQLayout extends ApplicationInitializer {
	
	@BeforeClass
    public static void testInitialize() throws Exception {
    	ApplicationInitializer.testInitializeWithWidgets();
    }
	
//    @Test
    public void testLayout() {
    	QWidget parent = new QWidget();
    	parent.setObjectName("parent");
    	QWidget widget = new QWidget();
    	widget.setObjectName("widget");
    	QGridLayout layout = new QGridLayout(parent);
    	widget.setLayout(layout);
    	System.out.println("parent = "+layout.parent());
    	System.out.println("parentWidget = "+layout.parentWidget());
    }
    
    @Test
    public void testGraphicsLayout() {
    	QGraphicsWidget parent = new QGraphicsWidget();
    	parent.setObjectName("parent");
    	QGraphicsWidget widget = new QGraphicsWidget();
    	widget.setObjectName("widget");
    	QGraphicsGridLayout layout = new QGraphicsGridLayout();
    	widget.setLayout(layout);
    	System.out.println("parentLayoutItem = "+layout.parentLayoutItem());
//    	widget.dispose();
//    	System.out.println(layout.isDisposed());
    }
    
    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestQLayout.class.getName());
    }
}
