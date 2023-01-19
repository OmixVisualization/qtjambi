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

import static org.junit.Assert.*;
import java.util.Arrays;
import org.junit.Test;
import io.qt.gui.*;

public class TestGLVersionFunctionsQt5 extends ApplicationInitializer {

	@Test
	public void test() {
		QOpenGLContext context = new QOpenGLContext();
		for(Class<? extends QAbstractOpenGLFunctions> cls : Arrays.asList(
				QOpenGLFunctions_ES2.class,
				QOpenGLFunctions_1_0.class,
				QOpenGLFunctions_1_1.class,
				QOpenGLFunctions_1_2.class,
				QOpenGLFunctions_1_3.class,
				QOpenGLFunctions_1_4.class,
				QOpenGLFunctions_1_5.class,
				QOpenGLFunctions_2_0.class,
				QOpenGLFunctions_2_1.class,
				QOpenGLFunctions_3_0.class,
				QOpenGLFunctions_3_1.class,
				QOpenGLFunctions_3_2_Compatibility.class,
				QOpenGLFunctions_3_2_Core.class,
				QOpenGLFunctions_3_3_Compatibility.class,
				QOpenGLFunctions_3_3_Core.class,
				QOpenGLFunctions_4_0_Compatibility.class,
				QOpenGLFunctions_4_0_Core.class,
				QOpenGLFunctions_4_1_Compatibility.class,
				QOpenGLFunctions_4_1_Core.class,
				QOpenGLFunctions_4_2_Compatibility.class,
				QOpenGLFunctions_4_2_Core.class,
				QOpenGLFunctions_4_3_Compatibility.class,
				QOpenGLFunctions_4_3_Core.class,
				QOpenGLFunctions_4_4_Compatibility.class,
				QOpenGLFunctions_4_4_Core.class,
				QOpenGLFunctions_4_5_Compatibility.class,
				QOpenGLFunctions_4_5_Core.class)) {
			QAbstractOpenGLFunctions versionFunctions = context.versionFunctions(cls);
			System.out.println(cls.getName()+": "+versionFunctions);
			assertTrue("Expected "+cls.getName()+" but was "+(versionFunctions==null ? "null" : versionFunctions.getClass().getName()), versionFunctions==null || versionFunctions.getClass()==cls);
		}
	}
}
