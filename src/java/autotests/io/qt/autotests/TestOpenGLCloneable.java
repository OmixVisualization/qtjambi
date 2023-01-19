/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
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
**
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

package io.qt.autotests;

import static org.junit.Assert.assertEquals;

import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.opengl.*;

public class TestOpenGLCloneable extends ApplicationInitializer {
	
	@BeforeClass
	public static void testInitialize() throws Exception {
		ApplicationInitializer.testInitializeWithGui();
	}

    // this test does not make sense because QGLBuffer does not have an equals operator
    //@Test
	public void run_clone_QOpenGLBuffer() {
		QOpenGLBuffer org = new QOpenGLBuffer();
		QOpenGLBuffer clone = org.clone();
		org.dispose();
		QOpenGLBuffer clone2 = clone.clone();
		assertEquals(clone, clone2);
	}

	@Test
	public void run_clone_QOpenGLFramebufferObjectFormat() {
		QOpenGLFramebufferObjectFormat org = new QOpenGLFramebufferObjectFormat();
		QOpenGLFramebufferObjectFormat clone = org.clone();
		org.dispose();
		QOpenGLFramebufferObjectFormat clone2 = clone.clone();
		assertEquals(clone, clone2);
	}
}
