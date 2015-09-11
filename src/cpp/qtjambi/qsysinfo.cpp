/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2015 Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
** 
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
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
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "qtjambi_core.h"


#define QTJAMBI_OS_AIX          1
#define QTJAMBI_OS_BSD4          2
#define QTJAMBI_OS_BSDI          3
#define QTJAMBI_OS_CYGWIN        4
#define QTJAMBI_OS_DARWIN        5
#define QTJAMBI_OS_DGUX          6
#define QTJAMBI_OS_DYNIX         7
#define QTJAMBI_OS_FREEBSD       8
#define QTJAMBI_OS_HPUX          9
#define QTJAMBI_OS_HURD         10
#define QTJAMBI_OS_IRIX         11
#define QTJAMBI_OS_LINUX        12
#define QTJAMBI_OS_LYNX         13
#define QTJAMBI_OS_MSDOS        14
#define QTJAMBI_OS_NETBSD       15
#define QTJAMBI_OS_OPENBSD      16
#define QTJAMBI_OS_OS2          17
#define QTJAMBI_OS_OS2EMX       18
#define QTJAMBI_OS_OSF          19
#define QTJAMBI_OS_QNX          20
#define QTJAMBI_OS_QNX6         21
#define QTJAMBI_OS_RELIANT      22
#define QTJAMBI_OS_SCO          23
#define QTJAMBI_OS_SOLARIS      24
#define QTJAMBI_OS_ULTRIX       25
#define QTJAMBI_OS_UNIXWARE     26
#define QTJAMBI_OS_WIN32        27
#define QTJAMBI_OS_WIN64        28

extern "C" Q_DECL_EXPORT jint JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_QSysInfo_windowsVersion(JNIEnv *, jclass))
{
    return
#if defined(Q_OS_WIN32) || defined(Q_OS_WIN64)
        QSysInfo::WindowsVersion;
#else
        0;
#endif
}

extern "C" Q_DECL_EXPORT jint JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_QSysInfo_macVersion(JNIEnv *, jclass))
{
    return
#if defined(Q_OS_MAC)
        QSysInfo::MacintoshVersion;
#else
        0;
#endif
}

extern "C" Q_DECL_EXPORT jint JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_QSysInfo_operatingSystem(JNIEnv *, jclass))
{
#if defined(Q_OS_AIX)
    return QTJAMBI_OS_AIX;
#elif defined(Q_OS_BSD4)
    return QTJAMBI_OS_BSD4;
#elif defined(Q_OS_BSDI)
    return QTJAMBI_OS_BSDI;
#elif defined(Q_OS_CYGWIN)
    return QTJAMBI_OS_CYGWIN;
#elif defined(Q_OS_DARWIN)
    return QTJAMBI_OS_DARWIN;
#elif defined(Q_OS_DGUX)
    return QTJAMBI_OS_DGUX;
#elif defined(Q_OS_DYNIX)
    return QTJAMBI_OS_DYNIX;
#elif defined(Q_OS_FREEBSD)
    return QTJAMBI_OS_FREEBSD;
#elif defined(Q_OS_HPUX)
    return QTJAMBI_OS_HPUX;
#elif defined(Q_OS_HURD)
    return QTJAMBI_OS_HURD;
#elif defined(Q_OS_IRIX)
    return QTJAMBI_OS_IRIX;
#elif defined(Q_OS_LINUX)
    return QTJAMBI_OS_LINUX;
#elif defined(Q_OS_LYNX)
    return QTJAMBI_OS_LYNX;
#elif defined(Q_OS_MSDOS)
    return OS;
#elif defined(Q_OS_NETBSD)
    return QTJAMBI_OS_NETBSD;
#elif defined(Q_OS_OPENBSD)
    return QTJAMBI_OS_OPENBSD;
#elif defined(Q_OS_OS2)
    return OS2;
#elif defined(Q_OS_OS2EMX)
    return OS2EMX;
#elif defined(Q_OS_OSF)
    return OSF;
#elif defined(Q_OS_QNX)
    return QTJAMBI_OS_QNX;
#elif defined(Q_OS_QNX6)
    return QTJAMBI_OS_QNX6;
#elif defined(Q_OS_RELIANT)
    return QTJAMBI_OS_RELIANT;
#elif defined(Q_OS_SCO)
    return QTJAMBI_OS_SCO;
#elif defined(Q_OS_SOLARIS)
    return QTJAMBI_OS_SOLARIS;
#elif defined(Q_OS_ULTRIX)
    return QTJAMBI_OS_ULTRIX;
#elif defined(Q_OS_UNIXWARE)
    return QTJAMBI_OS_UNIXWARE;
#elif defined(Q_OS_WIN64)
    return QTJAMBI_OS_WIN64;
#elif defined(Q_OS_WIN32)
    return QTJAMBI_OS_WIN32;
#else
    qWarning("Unknown OS version");
    return 0;
#endif
}
