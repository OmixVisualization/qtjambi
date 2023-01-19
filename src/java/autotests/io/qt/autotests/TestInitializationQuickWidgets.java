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

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

import org.junit.Assert;
import org.junit.Test;

import io.qt.core.QCoreApplication;
import io.qt.core.QLibraryInfo;
import io.qt.core.Qt;
import io.qt.quick.QQuickWindow;
import io.qt.quick.QSGRendererInterface;
import io.qt.quick.widgets.QQuickWidget;
import io.qt.widgets.QApplication;

public class TestInitializationQuickWidgets extends UnitTestInitializer {
    @Test
    public void initialize() throws IllegalAccessException, IllegalArgumentException, InvocationTargetException, NoSuchMethodException, SecurityException {
    	Assert.assertTrue(io.qt.QtUtilities.initializePackage("io.qt.quick.widgets"));
    	QCoreApplication.setAttribute(Qt.ApplicationAttribute.AA_ShareOpenGLContexts);
		Method mtd = QQuickWindow.class.getMethod(QLibraryInfo.version().majorVersion()>5 ? "setGraphicsApi" : "setSceneGraphBackend", QSGRendererInterface.GraphicsApi.class);
		mtd.invoke(null, QSGRendererInterface.GraphicsApi.OpenGL);
    	QApplication.initialize(new String[0]);
    	QQuickWidget window = new QQuickWidget();
    	window.show();
    	QApplication.processEvents();
    	window.hide();
    	QApplication.processEvents();
    	window.dispose();
    	QApplication.shutdown();
    }
}
