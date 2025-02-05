/****************************************************************************
**
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import java.io.IOException;

import org.junit.Assume;
import org.junit.Test;

public class TestDeployer extends UnitTestInitializer {
	@org.junit.BeforeClass
	public static void testInitialize() throws Exception {
		Assume.assumeFalse("Cannot run on Android.",
				io.qt.core.QOperatingSystemVersion.current().isAnyOfType(io.qt.core.QOperatingSystemVersion.OSType.Android));
	}
	
    @Test
    public void test() throws InterruptedException, IOException {
    	ApplicationInitializer.testDeployerApp("Deployment");
    }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestDeployer.class.getName());
    }
}
