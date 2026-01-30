###################################################################################################
##
## Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

HEADERS += \
    future_p.h \
    hashes.h \
    range_p.h \
    range_p_p.h \
    utils.h \
    utils_p.h

SOURCES +=\
    QHash_shell.cpp \
    QIterator_shell.cpp \
    QMultiHash_shell.cpp \
    QMultiMap_shell.cpp \
    QSet_shell.cpp \
    QMap_shell.cpp \
    QList_shell.cpp \
    QSpan_shell.cpp \
    future.cpp \
    plugins.cpp \
    impl.cpp \
    range.cpp \
    rangetree.cpp \
    rangetable.cpp \
    utils.cpp

lessThan(QT_MAJOR_VERSION, 6):{
    SOURCES +=
}
QT = core core-private
CONFIG+=no_private_qt_headers_warning

!ios:{
    PRECOMPILED_HEADER = pch_p.h
    CONFIG += precompile_header
}else{
    HEADERS += pch_p.h
}

win32-arm64-msvc* | win32-msvc*: {
    QMAKE_CXXFLAGS += /bigobj
}

win32-g++* {
    QMAKE_CXXFLAGS += -Wa,-mbig-obj -g0
    CONFIG(debug, debug|release) {
        QMAKE_CXXFLAGS += -O3
    }
}
