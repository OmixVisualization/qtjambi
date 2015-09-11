/****************************************************************************
 **
 ** Copyright (C) 1992-2009 Nokia. All rights reserved.
 **
 ** This file is part of Qt Jambi.
 **
 ** ** $BEGIN_LICENSE$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
** 
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
** 
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
** 
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
** 
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
** $END_LICENSE$

 **
 ** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 ** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 **
 ****************************************************************************/

package org.qtjambi.autotests;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

import org.junit.Test;

import org.qtjambi.qt.widgets.*;
import org.qtjambi.qt.gui.*;

// This class used to be in TestVirtualFunctions.java but due to the requirement for
//  #run_cppCreatedObjects() needing QPlastiqueStyle() which is not available by
//  default Qt builds on windows.
public class TestVirtualFunctionsQPlastiqueStyle extends QApplicationTest {
    /**
     * The purpose of this test is to verify the correct virtual functions are
     * being called for objects created in C++ and Java.
     *
     * This test relies on some hardcoded values in the styles so if those
     * change the tests will break, but wth...
     */
    @Test
    public void run_cppCreatedObjects() throws Exception {
//        QStyle java_plastique = new QPlastiqueStyle();
//        QStyle java_windowsstyle = new QWindowsStyle();
//
//        // Verify that the values are as expected...
//        assertEquals(java_plastique.pixelMetric(QStyle.PixelMetric.PM_SliderThickness), 15);
//        assertEquals(java_windowsstyle.pixelMetric(QStyle.PixelMetric.PM_SliderThickness), 16);
//
//        QStyle cpp_plastique = QStyleFactory.create("plastique");
//        assertTrue(cpp_plastique != null);
//        assertTrue(cpp_plastique instanceof QPlastiqueStyle);
//
//        // The actual test...
//        assertEquals(java_plastique.pixelMetric(QStyle.PixelMetric.PM_SliderThickness), cpp_plastique.pixelMetric(QStyle.PixelMetric.PM_SliderThickness));
//
//        QWindowsStyle windows_style = (QPlastiqueStyle) cpp_plastique;
//        assertEquals(windows_style.pixelMetric(QStyle.PixelMetric.PM_SliderThickness), java_plastique.pixelMetric(QStyle.PixelMetric.PM_SliderThickness));
    }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestVirtualFunctionsQPlastiqueStyle.class.getName());
    }
}
