###################################################################################################
##
## Copyright (C) 2009-2024 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
##
## This file is part of Qt Jambi.
##
## $BEGIN_LICENSE$
##
## GNU Lesser General Public License Usage
## This file may be used under the terms of the GNU Lesser
## General Public License version 2.1 as published by the Free Software
## Foundation and appearing in the file LICENSE.LGPL included in the
## packaging of this file.  Please review the following information to
## ensure the GNU Lesser General Public License version 2.1 requirements
## will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
##
## GNU General Public License Usage
## Alternatively, this file may be used under the terms of the GNU
## General Public License version 3.0 as published by the Free Software
## Foundation and appearing in the file LICENSE.GPL included in the
## packaging of this file.  Please review the following information to
## ensure the GNU General Public License version 3.0 requirements will be
## met: http://www.gnu.org/copyleft/gpl.html.
##
## $END_LICENSE$
##
## This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
## WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
##
###################################################################################################

include(../QtJambi/configure.pri)
TEMPLATE = app

DESTDIR = ../bin
QT = core

SOURCES = main.cpp

win32-arm64-msvc* | win32-msvc* {
    CONFIG += embed_manifest_dll force_embed_manifest
}

OTHER_FILES = icon.rc
win32:RC_FILE = icon.rc

macx:QMAKE_SONAME_PREFIX = @rpath
macx:QMAKE_RPATHDIR = @executable_path/. @executable_path/lib @executable_path/../lib @executable_path/../../lib @executable_path/Frameworks @executable_path/../Frameworks @executable_path/../../Frameworks
linux-clang* | linux-g++* | freebsd-clang* | freebsd-g++* | netbsd-clang* | netbsd-g++* | openbsd-clang* | openbsd-g++* | solaris-g++* | solaris-cc*:{
    QMAKE_RPATHDIR = $ORIGIN/.
    QMAKE_RPATHDIR += $ORIGIN/lib
    QMAKE_RPATHDIR += $ORIGIN/../lib
}
