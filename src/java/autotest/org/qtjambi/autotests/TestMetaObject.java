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
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import java.util.List;

import org.junit.Test;

import org.qtjambi.autotests.generated.SignalsAndSlots;

import org.qtjambi.qt.QtEnumerator;

/**
 * Testing the fake meta object.
 * @author eblomfel
 */
public class TestMetaObject extends QApplicationTest {
    private static class SignalsAndSlotsSubclass extends SignalsAndSlots {
        enum YoYoYo {
            Yo,
            YoYo,
            YoYoYo
        }

        enum HeyYo implements QtEnumerator {
            Hey(0x4),
            Yo(1123);

            HeyYo(int value) { this.value = value; }
            public int value() { return value; }

            private final int value;
        }

        enum FlipModeSquad implements QtEnumerator {
            Flip(1),
            Mode(2),
            Squad(4);

            FlipModeSquad(int value) { this.value = value; }
            public int value() { return value; }

            private final int value;
        }

        public static class FlipModeSquads extends org.qtjambi.qt.QFlags<FlipModeSquad> {
            private static final long serialVersionUID = 1L;
            public FlipModeSquads(FlipModeSquad ... args) { super(args); }
            public FlipModeSquads(int value) { setValue(value); }
        }
    }

    @Test public void regularEnumDeclarations() {
        SignalsAndSlotsSubclass sass = new SignalsAndSlotsSubclass();

        assertEquals(4, sass.numberOfEnumTypes());

        assertFalse(sass.isFlagsType("YoYoYo"));

        {
            List<String> names = sass.namesOfEnumType("YoYoYo");
            assertEquals(3, names.size());

            assertEquals("Yo", names.get(0));
            assertEquals("YoYo", names.get(1));
            assertEquals("YoYoYo", names.get(2));
        }

        {
            List<Integer> values = sass.valuesOfEnumType("YoYoYo");
            assertEquals(3, values.size());

            assertEquals((Integer)0, values.get(0));
            assertEquals((Integer)1, values.get(1));
            assertEquals((Integer)2, values.get(2));
        }
    }

    @Test public void qtEnumeratorDeclarations() {
        SignalsAndSlotsSubclass sass = new SignalsAndSlotsSubclass();

        assertFalse(sass.isFlagsType("HeyYo"));

        {
            List<String> names = sass.namesOfEnumType("HeyYo");
            assertEquals(2, names.size());

            assertEquals("Hey", names.get(0));
            assertEquals("Yo", names.get(1));
        }

        {
            List<Integer> values = sass.valuesOfEnumType("HeyYo");
            assertEquals(2, values.size());

            assertEquals((Integer)0x4, values.get(0));
            assertEquals((Integer)1123, values.get(1));
        }
    }

    @Test public void flagsDeclarations() {
        SignalsAndSlotsSubclass sass = new SignalsAndSlotsSubclass();

        assertTrue(sass.isFlagsType("FlipModeSquads"));

        {
            List<String> names = sass.namesOfEnumType("FlipModeSquad");
            assertEquals(3, names.size());

            assertEquals("Flip", names.get(0));
            assertEquals("Mode", names.get(1));
            assertEquals("Squad", names.get(2));
        }

        {
            List<Integer> values = sass.valuesOfEnumType("FlipModeSquad");
            assertEquals(3, values.size());

            assertEquals((Integer)1, values.get(0));
            assertEquals((Integer)2, values.get(1));
            assertEquals((Integer)4, values.get(2));
        }
    }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestMetaObject.class.getName());
    }
}
