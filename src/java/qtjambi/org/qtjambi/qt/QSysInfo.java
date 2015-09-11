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

public class QSysInfo {

    static {
        QtJambi_LibraryInitializer.init();
    }

    // Windows enum values
    public static final int Windows_32s      = 0x0001;
    public static final int Windows_95       = 0x0002;
    public static final int Windows_98       = 0x0003;
    public static final int Windows_Me       = 0x0004;
    public static final int Windows_DOS_based= 0x000f;

    public static final int Windows_NT       = 0x0010;
    public static final int Windows_2000     = 0x0020;
    public static final int Windows_XP       = 0x0030;
    public static final int Windows_2003     = 0x0040;
    public static final int Windows_VISTA    = 0x0080;
    public static final int Windows_NT_based = 0x00f0;

    public static final int Windows_CE       = 0x0100;
    public static final int Windows_CENET    = 0x0200;
    public static final int Windows_CE_based = 0x0f0;


    // Mac OS X enum values
    public static final int MacOS_10_0 = 0x0002;
    public static final int MacOS_10_1 = 0x0003;
    public static final int MacOS_10_2 = 0x0004;
    public static final int MacOS_10_3 = 0x0005;
    public static final int MacOS_10_4 = 0x0006;

    public static final int MacOS_CHEETAH   = MacOS_10_0;
    public static final int MacOS_PUMA      = MacOS_10_1;
    public static final int MacOS_JAGUAR    = MacOS_10_2;
    public static final int MacOS_PANTHER   = MacOS_10_3;
    public static final int MacOS_TIGER     = MacOS_10_4;


    // Operating system enum values.
    public static final int OS_AIX  =  1;
    public static final int OS_BSD4     =  2;
    public static final int OS_BSDI =  3;
    public static final int OS_CYGWIN   =  4;
    public static final int OS_DARWIN   =  5;
    public static final int OS_DGUX =  6;
    public static final int OS_DYNIX    =  7;
    public static final int OS_FREEBSD  =  8;
    public static final int OS_HPUX =  9;
    public static final int OS_HURD = 10;
    public static final int OS_IRIX = 11;
    public static final int OS_LINUX    = 12;
    public static final int OS_LYNX = 13;
    public static final int OS_MSDOS    = 14;
    public static final int OS_NETBSD   = 15;
    public static final int OS_OPENBSD  = 16;
    public static final int OS_OS2  = 17;
    public static final int OS_OS2EMX   = 18;
    public static final int OS_OSF  = 19;
    public static final int OS_QNX  = 20;
    public static final int OS_QNX6 = 21;
    public static final int OS_RELIANT  = 22;
    public static final int OS_SCO  = 23;
    public static final int OS_SOLARIS  = 24;
    public static final int OS_ULTRIX   = 25;
    public static final int OS_UNIXWARE = 26;
    public static final int OS_WIN32    = 27;
    public static final int OS_WIN64    = 28;

    public static native int windowsVersion();
    public static native int macVersion();
    public static native int operatingSystem();
}
