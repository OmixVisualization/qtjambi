###################################################################################################
##
## Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

TEMPLATE = app
TARGET = QtJambiGenerator
CONFIG += console

QT -= core gui

SOURCES  = main.cpp

win32-arm64-msvc* | win32-msvc* {
    CONFIG += embed_manifest_dll force_embed_manifest
}

OTHER_FILES = icon.rc
win32:RC_FILE = icon.rc

DESTDIR = ../bin

contains(QT_CONFIG, release):contains(QT_CONFIG, debug) {
    win32-msvc* | !CONFIG(force_debug_info): {
        # Qt was configued with both debug and release libs
        CONFIG += debug_and_release build_all
    }else{
        # don't compile debug with forced debug info
        CONFIG += release
    }
}

CONFIG(release, debug|release): CONFIG(force_debug_info) {
    CONFIG += separate_debug_info
}

CONFIG(debug, debug|release) {
    win32:{
        TARGET = $$member(TARGET, 0)d
    }else{
        macx:{
            TARGET = $$member(TARGET, 0)_debug
        }else{
            TARGET = $$member(TARGET, 0)_debug
        }
    }
}

lessThan(QT_MAJOR_VERSION, 6):{
    CONFIG += c++17
}

win32*:{
    CONFIG(debug, debug|release) {
        QTJAMBI_LIB_NAME = QtJambiGeneratord$$QT_MAJOR_VERSION
    }else{
        QTJAMBI_LIB_NAME = QtJambiGenerator$$QT_MAJOR_VERSION
    }
}else{
        CONFIG(debug, debug|release) {
        QTJAMBI_LIB_NAME = QtJambiGenerator_debug
    }else{
        QTJAMBI_LIB_NAME = QtJambiGenerator
    }
}

mac:{
    CONFIG -= app_bundle
    if($$QTJAMBI_GENERATE_FRAMEWORKS):{
        LIBS += -F../lib/
        LIBS += -framework $$QTJAMBI_LIB_NAME
    }else{
        equals(QTJAMBI_EXTENSION_SHLIB,dylib) | isEmpty(QTJAMBI_EXTENSION_SHLIB):{
            LIBS += -L../lib
            LIBS += -l$$QTJAMBI_LIB_NAME
        }else{
            QMAKE_EXTENSION_SHLIB = $$QTJAMBI_EXTENSION_SHLIB
            LIBS += ../lib/lib$$member(QTJAMBI_LIB_NAME, 0).$$QTJAMBI_EXTENSION_SHLIB
        }
    }
    QMAKE_SONAME_PREFIX = @rpath
    QMAKE_RPATHDIR = @executable_path/../lib
    greaterThan(QT_MAJOR_VERSION, 5):{
        greaterThan(QT_MAJOR_VERSION, 6) | greaterThan(QT_MINOR_VERSION, 1):{
            QMAKE_APPLE_DEVICE_ARCHS = x86_64 arm64
        }
    }
} else {
    LIBS += -L../lib
    LIBS += -l$$QTJAMBI_LIB_NAME
    linux-clang* | linux-g++* | freebsd-clang* | freebsd-g++* | netbsd-clang* | netbsd-g++* | openbsd-clang* | openbsd-g++* | solaris-g++* | solaris-cc*:{
        QMAKE_RPATHDIR = $ORIGIN/../lib
    }
}

