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
import static org.junit.Assume.assumeTrue;

import org.junit.BeforeClass;
import org.junit.Test;
import org.junit.experimental.categories.Category;

import org.qtjambi.qt.gui.QColor;
import org.qtjambi.qt.opengl.QGLColormap;
import org.qtjambi.unittests.support.CategoryOpenGL;
import org.qtjambi.unittests.support.FilterOpenGL;

// OpenGL support is an optional part of API:
// 1) The javac has to compile this package (this is the usual way the
//    test is deselected by having javac just not compile it)
// 2) The ANT testrunner looks over the source code folder for tests the
//    problem is that this class won't load in environment where OpenGL
//    package does not exist.  FIXME
@Category(CategoryOpenGL.class)
public class TestInjectedCodeOpenGL extends QApplicationTest {

    @BeforeClass
    public static void testInitialize() throws Exception {
        assumeTrue(FilterOpenGL.detectStatic());
        QApplicationTest.testInitialize(null);
    }

    @Test
    public void testQGLColorMapSetEntries() {
        int firstColors[] = { new QColor(org.qtjambi.qt.core.Qt.GlobalColor.red).rgba(), new QColor(org.qtjambi.qt.core.Qt.GlobalColor.green).rgba() };
        int secondColors[] = { new QColor(org.qtjambi.qt.core.Qt.GlobalColor.blue).rgba(), new QColor(org.qtjambi.qt.core.Qt.GlobalColor.yellow).rgba() };

        QGLColormap map = new QGLColormap();
        map.setEntries(firstColors);
        assertEquals(new QColor(org.qtjambi.qt.core.Qt.GlobalColor.red).rgba(), map.entryRgb(0));
        assertEquals(new QColor(org.qtjambi.qt.core.Qt.GlobalColor.green).rgba(), map.entryRgb(1));

        map.setEntries(secondColors, 1);
        assertEquals(new QColor(org.qtjambi.qt.core.Qt.GlobalColor.red).rgba(), map.entryRgb(0));
        assertEquals(new QColor(org.qtjambi.qt.core.Qt.GlobalColor.blue).rgba(), map.entryRgb(1));
    }


    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestInjectedCodeOpenGL.class.getName());
    }
}
