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

import org.qtjambi.qt.QtProperty;
import org.qtjambi.qt.QtPropertyDesignable;
import org.qtjambi.qt.QtPropertyReader;
import org.qtjambi.qt.QtPropertyResetter;
import org.qtjambi.qt.QtPropertyUser;
import org.qtjambi.qt.QtPropertyWriter;
import org.qtjambi.qt.core.QObject;
import org.qtjambi.qt.widgets.QAbstractButton;
import org.qtjambi.qt.widgets.QCheckBox;

public class TestProperties extends QApplicationTest {
    private static class FullOfProperties extends QObject {
        private boolean isDesignableTest;
        private FullOfProperties(boolean isDesignableTest) {
            this.isDesignableTest = isDesignableTest;
        }

        public final int ordinaryProperty() { return 0; }
        public final void setOrdinaryProperty(int i) { }

        @QtPropertyReader(name="annotatedProperty")
        public final int fooBar() { return 0; }

        @QtPropertyWriter(name="annotatedProperty")
        public final void fooBarSetIt(int i) { }

        @QtPropertyReader()
        public final int ordinaryReadOnlyProperty() { return 0; }

        @QtPropertyReader()
        public final int readOnlyProperty() { return 0; }

        @QtPropertyWriter(enabled=false)
        public final void setReadOnlyProperty(int i) { }

        @QtPropertyDesignable(value="false")
        public final int ordinaryNonDesignableProperty() { return 0; }

        public final void setOrdinaryNonDesignableProperty(int i) { }

        @QtPropertyDesignable(value="false")
        @QtPropertyReader(name="annotatedNonDesignableProperty")
        public final int fooBarXyz() { return 0; }
        @QtPropertyWriter()
        public final void setAnnotatedNonDesignableProperty(int i) { }

        public final boolean hasBooleanProperty() { return false; }
        public final void setBooleanProperty(boolean b) { }

        public final boolean isOtherBooleanProperty() { return false; }
        public final void setOtherBooleanProperty(boolean b) { }

        @QtPropertyReader
        public final int resettableProperty() { return 0; }

        @QtPropertyWriter
        public final void setResettableProperty(int i) { }

        @QtPropertyResetter(name="resettableProperty")
        public final void resetResettableProperty() { }

        @QtPropertyDesignable(value="test")
        public final int testDesignableProperty() { return 0; }
        public final void setTestDesignableProperty(int i) { }

        @QtPropertyReader
        @QtPropertyUser
        public final int annotatedUserProperty() { return 0; }

        @QtPropertyWriter
        public final void setAnnotatedUserProperty(int i) {}

        @QtPropertyUser
        public final int myUserProperty() { return 0; }
        public final void setMyUserProperty(int i) {}


        public boolean test() {
            return isDesignableTest;
        }
    }

    private static class ExpectedValues {
        private boolean writable;
        private boolean resettable;
        private boolean designable;
        private boolean user;
        private String name;

        private ExpectedValues(String name, boolean writable, boolean resettable, boolean designable, boolean user) {
            this.name = name;
            this.writable = writable;
            this.resettable = resettable;
            this.designable = designable;
            this.user = user;
        }
    }

    @Test
    public void testProperties() {
        ExpectedValues expectedValues[] = {
                new ExpectedValues("ordinaryProperty", true, false, true, false),
                new ExpectedValues("annotatedProperty", true, false, true, false),
                new ExpectedValues("ordinaryReadOnlyProperty", false, false, true, false),
                new ExpectedValues("readOnlyProperty", false, false, true, false),
                new ExpectedValues("ordinaryNonDesignableProperty", true, false, false, false),
                new ExpectedValues("annotatedNonDesignableProperty", true, false, false, false),
                new ExpectedValues("booleanProperty", true, false, true, false),
                new ExpectedValues("otherBooleanProperty", true, false, true, false),
                new ExpectedValues("resettableProperty", true, true, true, false),
                new ExpectedValues("objectName", true, false, true, false),
                new ExpectedValues("testDesignableProperty", true, false, true, false),
                new ExpectedValues("myUserProperty", true, false, true, true),
                new ExpectedValues("annotatedUserProperty", true, false, true, true)
        };

        FullOfProperties fop = new FullOfProperties(true);
        List<QtProperty> properties = fop.properties();

        for (ExpectedValues e : expectedValues) {
            Utils.println(1, "Current property: " + e.name);
            boolean found = false;
            for (QtProperty property : properties) {
                if (property.name().equals(e.name)) {
                    assertEquals(e.writable, property.isWritable());
                    assertEquals(e.resettable, property.isResettable());
                    assertEquals(e.designable, property.isDesignable());
                    assertEquals(e.user, property.isUser());
                    found = true;
                    break;
                }
            }
            assertTrue(found);
        }
        assertEquals(expectedValues.length, properties.size());
    }

    @Test
    public void testFunctionDesignableProperty() {
        {
            FullOfProperties fop = new FullOfProperties(true);
            int idx = fop.indexOfProperty("testDesignableProperty");
            assertTrue(idx >= 0);

            List<QtProperty> properties = fop.properties();
            QtProperty property = properties.get(idx);

            assertTrue(property.isDesignable());
        }

        {
            FullOfProperties fop = new FullOfProperties(false);
            int idx = fop.indexOfProperty("testDesignableProperty");
            assertTrue(idx >= 0);

            List<QtProperty> properties = fop.properties();
            QtProperty property = properties.get(idx);

            assertFalse(property.isDesignable());
        }
    }

    @Test
    public void testUserPropertyInQt() {
        QAbstractButton b = new QCheckBox();

        QtProperty property = b.userProperty();
        assertEquals("checked", property.name());
        assertEquals(true, property.isUser());
    }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestProperties.class.getName());
    }
}
