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

QTJAMBI_PROJECT = $$basename(_PRO_FILE_)
QTJAMBI_PROJECT = $$section(QTJAMBI_PROJECT, .pro, 0, 0)
TARGET = $$QTJAMBI_PROJECT

isEmpty(QTJAMBI_PATCH_VERSION):{
    error("Please set QTJAMBI_PATCH_VERSION");
}
VERSION = $$section(QT_VERSION, ., 0, 1).$$QTJAMBI_PATCH_VERSION

CONFIG += plugin unversioned_libname skip_target_version_ext

TEMPLATE = lib
DESTDIR = ../lib
DLLDESTDIR = ../bin

CONFIG(debug, debug|release) {
    win32:{
        TARGET = $$member(TARGET, 0)d
    }else{
        TARGET = $$member(TARGET, 0)_debug
    }
}

QT -= gui widgets

contains(QT_CONFIG, release):contains(QT_CONFIG, debug) {
    win32-arm64-msvc* | win32-msvc* | !CONFIG(force_debug_info): {
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

macx:{
    CONFIG+=no_default_rpath no_qt_rpath
    contains(QT_CONFIG, x86):CONFIG += x86
    contains(QT_CONFIG, ppc):CONFIG += ppc
    contains(QT_CONFIG, x86_64):CONFIG += x86_64
    contains(QT_CONFIG, ppc64):CONFIG += ppc64
    contains(QT_CONFIG, arm64):CONFIG += arm64
    CONFIG -= precompile_header
    QMAKE_CXXFLAGS += -fpermissive
    QMAKE_CXXFLAGS += -Wc++14-extensions
    QMAKE_CXXFLAGS_WARN_OFF += -Wdollar-in-identifier-extension -Woverloaded-virtual

    QMAKE_RPATHDIR =  @loader_path/../lib
    QMAKE_RPATHDIR += @loader_path/../../lib
    QMAKE_RPATHDIR += @loader_path/../../../lib
    QMAKE_RPATHDIR += @loader_path/../../../../lib
    QMAKE_RPATHDIR += @loader_path/../../../../../lib

    greaterThan(QT_MAJOR_VERSION, 5):{
        greaterThan(QT_MAJOR_VERSION, 6) | greaterThan(QT_MINOR_VERSION, 1):{
            QMAKE_APPLE_DEVICE_ARCHS = x86_64 arm64
            DEFINES += QTJAMBI_APPLE_MULTI_ARCHS
        }
    }
}

linux-clang* | linux-g++* | freebsd-clang* | freebsd-g++* | netbsd-clang* | netbsd-g++* | openbsd-clang* | openbsd-g++* | solaris-g++* | solaris-cc*{
    QMAKE_RPATHDIR = $ORIGIN/../lib
    QMAKE_RPATHDIR += $ORIGIN/../../lib
    QMAKE_RPATHDIR += $ORIGIN/../../../lib
    QMAKE_RPATHDIR += $ORIGIN/../../../../lib
    QMAKE_RPATHDIR += $ORIGIN/../../../../../lib
}

linux-clang* | linux-g++* | freebsd-clang* | freebsd-g++* | netbsd-clang* | netbsd-g++* | openbsd-clang* | openbsd-g++* | solaris-g++* | solaris-cc* | win32-g++* {
    QMAKE_CXXFLAGS_WARN_OFF += -Wdollar-in-identifier-extension -Woverloaded-virtual
    QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-function
    QMAKE_LFLAGS_NOUNDEF   += -Wl,--no-undefined
    QMAKE_LFLAGS += $$QMAKE_LFLAGS_NOUNDEF
}

contains(QMAKE_CXX, "g++") | contains(QMAKE_CXX, "clang++") : {
    SOURCES_BASE = $$clean_path($$dirname(_PRO_FILE_)/..)
    QMAKE_CXXFLAGS_DEBUG += -g -fdebug-prefix-map=$$SOURCES_BASE=../sources
    QMAKE_CXXFLAGS_RELEASE_WITH_DEBUGINFO += -g -fdebug-prefix-map=$$SOURCES_BASE=../sources
}

android:{
    CONFIG -= android_install
    TARGET = $${TARGET}_$${QT_ARCH}
}

HEADERS += 
SOURCES += plugin.cpp

DEFINES += QT_NO_VERSION_TAGGING
