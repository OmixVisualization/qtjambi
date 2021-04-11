/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2021 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
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

import org.junit.Test;

public class TestInitializationQt6 {
    @Test
    public void initializeCore() {
    	io.qt.QtUtilities.initializePackage("io.qt.core");
    }
    @Test
    public void initializeGui() {
    	io.qt.QtUtilities.initializePackage("io.qt.gui");
    }
    @Test
    public void initializeWidgets() {
    	io.qt.QtUtilities.initializePackage("io.qt.widgets");
    }
    @Test
    public void initializeNetwork() {
    	io.qt.QtUtilities.initializePackage("io.qt.network");
    }
    @Test
    public void initializeXml() {
    	io.qt.QtUtilities.initializePackage("io.qt.xml");
    }
    @Test
    public void initializeQml() {
    	io.qt.QtUtilities.initializePackage("io.qt.qml");
    }
    @Test
    public void initializeDBus() {
    	io.qt.QtUtilities.initializePackage("io.qt.dbus");
    }
    @Test
    public void initializeConcurrent() {
    	io.qt.QtUtilities.initializePackage("io.qt.concurrent");
    }
    @Test
    public void initializeHelp() {
    	io.qt.QtUtilities.initializePackage("io.qt.help");
    }
    @Test
    public void initializeOpenGL() {
    	io.qt.QtUtilities.initializePackage("io.qt.opengl");
    }
    @Test
    public void initializeOpenGLWidgets() {
    	io.qt.QtUtilities.initializePackage("io.qt.opengl.widgets");
    }
    @Test
    public void initializePrintSupport() {
    	io.qt.QtUtilities.initializePackage("io.qt.printsupport");
    }
    @Test
    public void initializeQuick() {
    	io.qt.QtUtilities.initializePackage("io.qt.quick");
    }
    @Test
    public void initializeQuickControls() {
    	io.qt.QtUtilities.initializePackage("io.qt.quick.controls");
    }
    @Test
    public void initializeQuickWidgets() {
    	io.qt.QtUtilities.initializePackage("io.qt.quick.widgets");
    }
    @Test
    public void initializeSql() {
    	io.qt.QtUtilities.initializePackage("io.qt.sql");
    }
    @Test
    public void initializeSvg() {
    	io.qt.QtUtilities.initializePackage("io.qt.svg");
    }
    @Test
    public void initializeSvgWidgets() {
    	io.qt.QtUtilities.initializePackage("io.qt.svg.widgets");
    }
    @Test
    public void initializeTest() {
    	io.qt.QtUtilities.initializePackage("io.qt.test");
    }
}
