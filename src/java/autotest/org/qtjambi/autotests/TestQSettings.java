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
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertNull;

import java.io.Serializable;
import java.util.List;
import java.util.Vector;

import org.junit.Test;

import org.qtjambi.qt.QVariant;
import org.qtjambi.qt.core.QSettings;

public class TestQSettings extends QApplicationTest implements Serializable {
    private static final long serialVersionUID = 1L;

    @Test
    public void writeSettingsSimple() {
        QSettings settings = new QSettings("Trolltech", "Test");
        settings.sync();

        settings.setValue("int", 5);
        settings.setValue("double", 5.000001d);
        settings.setValue("String", "String");

        settings.sync();
    }

    @Test
    public void readSettingsSimple() {
        QSettings settings = new QSettings("Trolltech", "Test");
        settings.sync();

        assertEquals(5, QVariant.toInt(settings.value("int")));
        assertEquals(5.000001d, QVariant.toDouble(settings.value("double").toString()), 0.0);
        assertEquals("String", QVariant.toString(settings.value("String")));
    }

    @Test
    public void writeSettingsCollection() {
        QSettings settings = new QSettings("Trolltech", "Test");
        settings.sync();

        List<String> list = new Vector<String>();
        for (int i = 0; i < 10; i++) {
            list.add("entry-" + i);
        }
        settings.setValue("test", list);
        settings.sync();
    }

    @Test
    public void readSettingsCollection() {
        QSettings settings = new QSettings("Trolltech", "Test");
        settings.sync();

        List<?> list = (List<?>) settings.value("test", new Vector<String>());

        for (int i = 0; i < 10; i++) {
            assertEquals("entry-" + i, list.get(i));
        }
    }

    @Test
    public void readSettingsEmpty() {
        QSettings settings = new QSettings("Trolltech", "Test");
        settings.sync();

        String res = (String) settings.value("empty", "ok");
        assertEquals("ok", res);

        res = (String) settings.value("empty");
        assertNull(res);
    }

    public class Custom implements Serializable {
        private static final long serialVersionUID = 1L;

        String name;
        int integer;
        Custom object;
    }

    @Test
    public void writeSettingsCustomClass() {
        QSettings settings = new QSettings("Trolltech", "Test");
        settings.sync();

        Custom custom = new Custom();
        custom.name = "abc";
        custom.integer = 123;
        custom.object = new Custom();

        settings.setValue("custom", custom);
        settings.sync();
    }

    @Test
    public void readSettingsCustomClass() {
        QSettings settings = new QSettings("Trolltech", "Test");
        settings.sync();

        Custom custom = (Custom) settings.value("custom");
        assertNotNull(custom);
        assertEquals(custom.name, "abc");
        assertEquals(custom.integer, 123);
        assertEquals(custom.object.getClass(), Custom.class);
    }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestQSettings.class.getName());
    }
}
