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

import java.util.Collection;
import java.util.Iterator;
import java.util.Vector;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import org.qtjambi.qt.QFlags;
import org.qtjambi.qt.QtEnumerator;

public class TestQFlags extends QApplicationTest {
    private Collection<DataClear> data;

    @Before
    public void setUp() {
        data = new Vector<DataClear>();
        data.add(new DataClear(new MyEnum[] { MyEnum.One, MyEnum.Two, MyEnum.Four }, new MyEnum[] { MyEnum.Three }, 4));
        data.add(new DataClear(new MyEnum[] { MyEnum.One, MyEnum.Two, MyEnum.Four }, new MyEnum[] { MyEnum.Four }, 3));
        data.add(new DataClear(new MyEnum[] { MyEnum.One, MyEnum.Two, MyEnum.Four }, new MyEnum[] { MyEnum.Four, MyEnum.One }, 2));
    }

    @After
    public void tearDown() {
        data = null;
    }

    private enum MyEnum implements QtEnumerator {
        Zero, One, Two, Three, Four;
        public int value() {
            return ordinal();
        }
    }

    private static class Flags extends QFlags<MyEnum> {
        private static final long serialVersionUID = 1L;

        private Flags(MyEnum... flags) {
            super(flags);
        }
    }

    @Test
    public void run_testConstructor() {
        assertEquals(new Flags().value(), 0);

        assertEquals(new Flags(MyEnum.One, MyEnum.Two, MyEnum.Four).value(), 7);
        assertEquals(new Flags(MyEnum.Three).value(), 3);
        assertEquals(new Flags(MyEnum.One, MyEnum.Four).value(), 5);

        Flags flag = new Flags();
        flag.set(new Flags(MyEnum.One, MyEnum.Four));
        assertEquals(flag.value(), 5);

        flag.clearAll();
        flag.set(new Flags(MyEnum.Four));
        assertEquals(flag.value(), 4);
    }

    private static Flags createFlags(Flags other) {
        Flags f = new Flags();
        f.set(other);
        return f;
    }

    private static Flags createFlags(MyEnum... args) {
        Flags f = new Flags();
        for (MyEnum e : args)
            f.set(e);
        return f;
    }

    @Test
    public void run_set() {
        assertEquals(createFlags().value(), 0);

        assertEquals(createFlags(MyEnum.One, MyEnum.Two, MyEnum.Four).value(), 7);
        assertEquals(createFlags(MyEnum.Three).value(), 3);
        assertEquals(createFlags(MyEnum.One, MyEnum.Four).value(), 5);

        assertEquals(createFlags(new Flags(MyEnum.One, MyEnum.Four)).value(), 5);
        assertEquals(createFlags(new Flags(MyEnum.Four)).value(), 4);
    }

    private class DataClear {
        public DataClear(MyEnum[] toSet, MyEnum[] toClear, int result) {
            this.toSet = toSet;
            this.toClear = toClear;
            this.result = result;
        }

        public MyEnum[] toSet;
        public MyEnum[] toClear;
        public int result;
    }

    @Test
    public void run_clear() {
        for (Iterator<DataClear> iter = data.iterator(); iter.hasNext();) {
            DataClear d = iter.next();

            Flags f = new Flags(d.toSet);
            f.clear(d.toClear);
            assertEquals(f.value(), d.result);
        }
    }

    @Test
    public void run_equals() {
        assertEquals(new Flags(MyEnum.One), new Flags(MyEnum.One));
        assertEquals(new Flags(MyEnum.Three, MyEnum.Four), new Flags(MyEnum.Three, MyEnum.Four));
        assertTrue(!new Flags(MyEnum.One).equals(new Flags(MyEnum.Two)));

        assertTrue(!new Flags(MyEnum.One).equals(new QFlags<MyEnum>(MyEnum.One) {
            private static final long serialVersionUID = 1L;
        }));
    }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestQFlags.class.getName());
    }
}
