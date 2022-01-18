/****************************************************************************
**
** Copyright (C) 2009-2022 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import org.junit.Assert;
import org.junit.Test;

import io.qt.InternalAccess;
import io.qt.autotests.generated.General;

public class TestRetroHelper extends QApplicationTest {
	
	private void testCallerClassImpl() {
		Assert.assertEquals(TestRetroHelper.class, General.internalAccess.callerClassProvider().get());
	}
	
    @Test
    public void testCallerClass() {
    	testCallerClassImpl();
    }
    
    @Test
    public void testInvocationInfo() {
    	InternalAccess.CallerContext info = General.internalAccess.callerContextProvider().get();
//    	IntFunction<QtJambiInternal.InvocationInfo> invocationInfoProvider = QtJambiInternal.invocationInfoProvider();
//    	QtJambiInternal.InvocationInfo info = invocationInfoProvider.apply(2);
    	Assert.assertTrue(info!=null);
    	Assert.assertEquals(TestRetroHelper.class, info.declaringClass);
    	Assert.assertEquals("testInvocationInfo", info.methodName);
    }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestRetroHelper.class.getName());
    }
}
