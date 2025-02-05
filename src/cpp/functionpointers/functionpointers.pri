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

isEmpty(TARGET):{
    PRO_FILE = $$basename(_PRO_FILE_)
    PRO_FILE = $$section(PRO_FILE, .pro, 0, 0)
    TARGET = PRO_FILE;
}
TEMPLATE = lib

QT -= core gui

DEFINES += QT_NO_VERSION_TAGGING
CONFIG -= staticlib static force_debug_info separate_debug_info
CONFIG += unversioned_libname skip_target_version_ext shared

INCLUDEPATH += $$[QT_INSTALL_HEADERS]

SOURCES = impl.cpp

CONFIG(debug, debug|release) {
    QTJAMBICONF = debug
}else{
    QTJAMBICONF = release
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

contains(QT_CONFIG, release):contains(QT_CONFIG, debug) {
    # Qt was configued with both debug and release libs
    CONFIG += debug_and_release build_all
}

android:{
    CONFIG += rtti
    CONFIG += exceptions
    CONFIG -= android_install
    TARGET = $${TARGET}_$${QT_ARCH}
    QMAKE_CXXFLAGS_EXCEPTIONS_ON += -fexceptions
    QMAKE_CXXFLAGS += -fexceptions -frtti -funwind-tables
    QMAKE_LFLAGS += -Wl,--export-dynamic -Wl,--exclude-libs,libgcc_real.a -Wl,--exclude-libs,libunwind.a -Wl,--exclude-libs,libgcc.a -lunwind

#    greaterThan(QT_MAJOR_VERSION, 5):{
#        DESTDIR = ../../QtJambi/$$QTJAMBICONF
#    }else{
#        armeabi-v7a: DESTDIR = ../../QtJambi/$$QTJAMBICONF-armeabi-v7a
#        arm64-v8a: DESTDIR = ../../QtJambi/$$QTJAMBICONF-arm64-v8a
#        x86: DESTDIR = ../../QtJambi/$$QTJAMBICONF-x86
#        x86_64: DESTDIR = ../../QtJambi/$$QTJAMBICONF-x86_64
#    }
}else{
    macx:{
        CONFIG -= precompile_header
        CONFIG += no_default_rpath no_qt_rpath
        QMAKE_CXXFLAGS += -Wc++14-extensions
        QMAKE_CXXFLAGS += -F$$[QT_INSTALL_LIBS]
        QMAKE_CXXFLAGS_WARN_OFF += -Wdollar-in-identifier-extension -Woverloaded-virtual
        greaterThan(QT_MAJOR_VERSION, 6) | greaterThan(QT_MINOR_VERSION, 1):{
            QMAKE_APPLE_DEVICE_ARCHS = x86_64 arm64
        }
    }
    linux-clang* | linux-g++* | freebsd-clang* | freebsd-g++* | netbsd-clang* | netbsd-g++* | openbsd-clang* | openbsd-g++* | solaris-g++* | solaris-cc* | win32-g++*:{
        QMAKE_CXXFLAGS_WARN_OFF += -Wdollar-in-identifier-extension -Woverloaded-virtual
        QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-function
        QMAKE_LFLAGS_NOUNDEF   += -Wl,--no-undefined
        QMAKE_LFLAGS += $$QMAKE_LFLAGS_NOUNDEF
        CONFIG += rtti exceptions
        QMAKE_CXXFLAGS += -fexceptions -frtti -funwind-tables
    }

#    DESTDIR = ../../QtJambi/$$QTJAMBICONF
#    DLLDESTDIR = $$DESTDIR
}

DESTDIR = ../../lib
DLLDESTDIR = ../../lib
