/****************************************************************************
**
** Copyright (C) 2011 Darryl L. Miles.  All rights reserved.
** Copyright (C) 2011 D L Miles Consulting Ltd.  All rights reserved.
**
** This file is part of Qt Jambi.
**
**
** $BEGIN_LICENSE$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html
** 
** In addition, as a special exception, the copyright holders grant you
** certain additional rights. These rights are described in the Nokia Qt
** LGPL Exception version 1.0, included in the file LGPL_EXCEPTION.txt in
** this package.
** 
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 2.0 requirements will be
** met: http://www.gnu.org/licenses/gpl-2.0.html
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL3 included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

package org.qtjambi.autotests;

import static org.junit.Assert.assertEquals;

import org.junit.Test;

import org.qtjambi.qt.qtjambi.util.RetroTranslatorHelper;

public class TestRetroTranslatorHelper extends QApplicationTest {
    @Test
    public void testRetroTranslatorHelper() {
        String[] sA;
        String s;

        s = "";
        sA = RetroTranslatorHelper.split(s, ",", 0);
        assertEquals(0, sA.length);

        s = "QHostInfo";
        sA = RetroTranslatorHelper.split(s, ",", 0);
        assertEquals(1, sA.length);
        assertEquals("QHostInfo", sA[0]);

        s = "QHostInfo,Foo";
        sA = RetroTranslatorHelper.split(s, ",", 0);
        assertEquals(2, sA.length);
        assertEquals("QHostInfo", sA[0]);
        assertEquals("Foo", sA[1]);

        s = "QHostInfo,Foo,Bar";
        sA = RetroTranslatorHelper.split(s, ",", 0);
        assertEquals(3, sA.length);
        assertEquals("QHostInfo", sA[0]);
        assertEquals("Foo", sA[1]);
        assertEquals("Bar", sA[2]);

        s = "QHostInfo,";
        sA = RetroTranslatorHelper.split(s, ",", 0);
        assertEquals(1, sA.length);
        assertEquals("QHostInfo", sA[0]);

        s = ",QHostInfo,";
        sA = RetroTranslatorHelper.split(s, ",", 0);
        assertEquals(2, sA.length);
        assertEquals("", sA[0]);
        assertEquals("QHostInfo", sA[1]);

        s = ",QHostInfo";
        sA = RetroTranslatorHelper.split(s, ",", 0);
        assertEquals(2, sA.length);
        assertEquals("", sA[0]);
        assertEquals("QHostInfo", sA[1]);

        s = ",QHostInfo,,";
        sA = RetroTranslatorHelper.split(s, ",", 0);
        assertEquals(2, sA.length);
        assertEquals("", sA[0]);
        assertEquals("QHostInfo", sA[1]);
    }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestRetroTranslatorHelper.class.getName());
    }
}
