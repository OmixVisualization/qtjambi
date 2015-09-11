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

import org.qtjambi.autotests.generated.ValueType;
import org.qtjambi.autotests.generated.InvalidatorValueType;

import org.qtjambi.qt.QNativePointer;
import org.qtjambi.qt.internal.QtJambiObject;

public class TestMemoryManagementValueType extends TestMemoryManagement {
    @Override
    protected void initialize() {
    }

    @Override
    protected void uninitialize() {
    }

    @Override
    protected QtJambiObject createInstanceInJava() {
        QtJambiObject o = new ValueType();
        accountingForObject(o);
        return o;
    }

    @Override
    protected QtJambiObject createInstanceInNative() {
        return ValueType.fromNativePointer(ValueType.newInstance());
    }

    @Override
    protected void deleteLastInstance() {
        ValueType.deleteLastInstance();
    }

    @Override
    protected boolean isValueType() {
        return true;
    }

    QtJambiObject temporaryObject = null;
    @Override
    protected QtJambiObject invalidateObject(QtJambiObject obj, final boolean returnReference) {
        new InvalidatorValueType() {
            @Override
            public void overrideMe(QNativePointer t) {
                if (returnReference) {
                    temporaryObject = ValueType.fromNativePointer(t);
                    temporaryObject.setJavaOwnership();
                }
            }
        }.invalidateObject(obj.nativePointer());

        QtJambiObject tmp = temporaryObject;
        temporaryObject = null;
        return tmp;
    }

    @Override
    protected boolean hasShellDestructor() {
        return false;
    }

    @Override
    protected boolean hasVirtualDestructor() {
        return false;
    }

    @Override
    protected String className() {
        return "ValueType";
    }

    @Override
    protected boolean supportsSplitOwnership() {
        return false;
    }

    /*
        Required for IntelliJ to realize that the method is a junit test
        since it doesn't check super classes.
    */
    @Test
    public void dummy() {
    }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestMemoryManagementValueType.class.getName());
    }
}
