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

import org.junit.Test;

import org.qtjambi.autotests.generated.NonPolymorphicObjectType;
import org.qtjambi.autotests.generated.InvalidatorNonPolymorphicObjectType;

import org.qtjambi.qt.internal.QtJambiObject;
import org.qtjambi.qt.internal.QtJambiRuntime;

public class TestMemoryManagementNonPolymorphicObject extends TestMemoryManagement {
    @Override
    protected void initialize() {
        QtJambiRuntime.setObjectCacheMode(QtJambiRuntime.OBJECT_CACHE_MODE_DISABLE);
    }

    @Override
    protected void uninitialize() {
        // We need to undo this unusual setting to give any test cases that run after
        // within the same JVM a change of succeeding.
        QtJambiRuntime.setObjectCacheMode(QtJambiRuntime.OBJECT_CACHE_MODE_DEFAULT);
    }

    protected QtJambiObject createInstanceInJava() {
        QtJambiObject o = new NonPolymorphicObjectType();
        accountingForObject(o);
        return o;
    }

    protected QtJambiObject createInstanceInNative() {
        return NonPolymorphicObjectType.newInstance();
    }

    protected void deleteLastInstance() {
        NonPolymorphicObjectType.deleteLastInstance();
    }

    QtJambiObject temporaryObject = null;
    protected QtJambiObject invalidateObject(QtJambiObject obj, final boolean returnReference) {
        new InvalidatorNonPolymorphicObjectType() {
            @Override
            public void overrideMe(NonPolymorphicObjectType t) {
                if (returnReference) {
                    temporaryObject = t;
                    temporaryObject.setJavaOwnership();
                }
            }
        }.invalidateObject((NonPolymorphicObjectType) obj);

        QtJambiObject tmp = temporaryObject;
        temporaryObject = null;
        return tmp;
    }

    protected String className() {
        return "NonPolymorphicObjectType";
    }

    protected boolean hasShellDestructor() {
        return false;
    }

    @Override
    protected boolean hasVirtualDestructor() {
        return false;
    }

    /*
        Required for IntelliJ to realize that the method is a junit test
        since it doesn't check super classes.
    */
    @Test
    public void dummy() {}

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestMemoryManagementNonPolymorphicObject.class.getName());
    }
}
