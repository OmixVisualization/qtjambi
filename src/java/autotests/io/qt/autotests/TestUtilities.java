/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
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

import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import org.junit.Assert;
import org.junit.Test;

import io.qt.QLibraryNotFoundError;
import io.qt.QtUtilities;
import io.qt.autotests.generated.General;

public class TestUtilities extends UnitTestInitializer {

	private static boolean loadQtJambiLibrary(String lib) {
        try {
            QtUtilities.loadQtJambiLibrary(lib);
            return true;
        } catch (UnsatisfiedLinkError | QLibraryNotFoundError e) {
            java.util.logging.Logger.getLogger("io.qt").log(java.util.logging.Level.SEVERE, "", e);
            return false;
        }
    }

	@org.junit.Test
	public void testLoadLibrary() {
		assertTrue("validLibrary", loadQtJambiLibrary("Core"));
		assertFalse("falseLibrary", loadQtJambiLibrary("Kore"));
	}
    
    @Test
    public void testNullClass() {
    	Assert.assertEquals(null, General.internalAccess.getClass(null));
    }
}
