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

/**
 * This class contains static functions to query information about the
 * Qt library Qt Jambi links against.
 */
public class QtInfo {

    static {
        QtJambi_LibraryInitializer.init();
    }

    /**
     * Returns The Qt version as a string on the form Major.Minor.Patch, e.g 4.1.2
     * @return The Qt version string
     */
    public static String versionString() {
        return String.format("%1$d.%2$d.%3$d", majorVersion(), minorVersion(), patchVersion());
    }


    /**
     * Returns the Qt version as a hexadecimal coded integer on the format
     * 0x00MMmmpp, where MM is major version, mm is minor version and pp is
     * patch version.
     *
     * @return the Qt version as a hexadecimal coded integer
     */
    public static int version() {
        return (majorVersion() << 16) | (minorVersion() << 8) | patchVersion();
    }


    /**
     * @return Qt's major version
     */
    public native static int majorVersion();


    /**
     * @return Qt's minor version
     */
    public native static int minorVersion();


    /**
     * @return Qt's patch version
     */
    public native static int patchVersion();
}
