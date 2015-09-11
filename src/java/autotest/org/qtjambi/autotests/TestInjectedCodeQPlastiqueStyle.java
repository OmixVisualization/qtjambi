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

import org.junit.Test;

import org.qtjambi.qt.widgets.*;
import org.qtjambi.qt.gui.*;

// This class used to be in TestInjectedCode.java but due to the requirement for
//  #testQApplicationSetStyleOverridePolish() and #testQApplicationSetStyleNotOverridePolish()
//  needing QPlastiqueStyle() which is not available by default Qt builds on windows.
public class TestInjectedCodeQPlastiqueStyle extends QApplicationTest {
    /*
	private QPalette savedPalette = null;
    @Test
    public void testQApplicationSetStyleOverridePolish() {
        savedPalette = null;

        QApplication.setStyle(new QPlastiqueStyle() {
            @Override
            public void polish(QPalette palette) {
                savedPalette = palette;
                super.polish(palette);
            }
        });

        // The palette is automatically invalidated after the virtual
        // call to polish
        assertEquals(0l, savedPalette.nativeId());
    }

    @Test
    public void testQApplicationSetStyleNotOverridePolish() {
        QApplication.setStyle(new QPlastiqueStyle());
        // got here, didn't crash
    }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestInjectedCodeQPlastiqueStyle.class.getName());
    }
    */
}
