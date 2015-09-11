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

package org.qtjambi.qt;

import org.qtjambi.qt.core.QObject;

public class GeneratorUtilities {

    private static final boolean threadAsserts;

    static {
        // default is enabled unless "false" or "no" value is specified
        Boolean bf = Utilities.matchBooleanProperty("org.qtjambi.qt.thread-check", Boolean.TRUE, Boolean.TRUE, null, null);
        threadAsserts = bf.booleanValue();
    }

    public static void threadCheck(QObject obj) {
        if (threadAsserts) {
            if (obj.thread() != null && obj.thread() != Thread.currentThread()) {
                throw new QThreadAffinityException("QObject used from outside its own thread",
                                                   obj,
                                                   Thread.currentThread());
            }
        }
    }

    public static Object fetchField(Object owner, Class<?> declaringClass, String fieldName) {
        return org.qtjambi.qt.internal.QtJambiInternal.fetchField(owner, declaringClass, fieldName);
    }

    public static void setField(Object owner, Class<?> declaringClass, String fieldName, Object newValue) {
        org.qtjambi.qt.internal.QtJambiInternal.setField(owner, declaringClass, fieldName, newValue);
    }

    public static void countExpense(Class<?> cl, int cost, int limit) {
        org.qtjambi.qt.internal.QtJambiInternal.countExpense(cl, cost, limit);
    }

    public static Object createExtendedEnum(int value, int ordinal,
            Class<?> cl, String name) {
        return org.qtjambi.qt.internal.QtJambiInternal.createExtendedEnum(value, ordinal, cl, name);
    }
}
