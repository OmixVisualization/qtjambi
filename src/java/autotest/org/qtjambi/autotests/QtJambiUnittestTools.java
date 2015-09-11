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

import java.lang.reflect.Method;

/**
 * Special debugging methods used by autotests which are only available
 * if Qt Jambi is compiled with QTJAMBI_DEBUG_TOOLS defined.
 *
 */
public class QtJambiUnittestTools {
    static {
        try {
            // Causes QtJambi_LibraryInitializer.<init> to run without needing a compile time
            //  dependency.
            Class.forName("org.qtjambi.autotests.generated.QtJambi_LibraryInitializer");
        } catch(ClassNotFoundException eat) {
        }
    }

    /**
     * Returns true if the debug tools functions are compiled in and
     * false if not.
     */
    public static native boolean hasDebugTools();

    /**
     * Returns values for memory object counts of internal parts of
     * QtJambi.  The main use is in trying to find memory leaks.
     */
    public static native int getObjectCount(int arg0, int arg1);

    public static void clearStaticReferences() {
        // Use reflection to get at the static method QtJambi_LibraryShutdown#clearStaticReferences()
        // QtJambi_LibraryShutdown.class.getName(); can not use due to package visibility
        final String K_org_qtjambi_qt_QtJambi_LibraryShutdown = "org.qtjambi.qt.QtJambi_LibraryShutdown";
        final String K_clearStaticReferences = "clearStaticReferences";
        Boolean accessible = null;
        Method method = null;
        try {
            ClassLoader classLoader = QtJambiUnittestTools.class.getClassLoader();
            Class<?> clazz = Class.forName(K_org_qtjambi_qt_QtJambi_LibraryShutdown, true, classLoader);
            method = clazz.getDeclaredMethod(K_clearStaticReferences, (Class[]) null);
            if(method.isAccessible() == false) {
                accessible = Boolean.valueOf(method.isAccessible());
                method.setAccessible(true);
            }
            method.invoke(clazz);
        } catch(Exception e) {
            e.printStackTrace();
        } finally {
            if(accessible != null)
                method.setAccessible(accessible.booleanValue());
        }
    }
}
