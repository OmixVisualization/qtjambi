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

QTJAMBI_LIB_PATH = ../../qtjambi/lib
DESTDIR = ../lib
DLLDESTDIR = ../bin

include(../QtJambi/configure.pri)

GENERATOR_PRI = $$clean_path($$PWD/../../../$$VERSION/build/tests/cpp/$$QTJAMBI_PROJECT/generated.pri)
exists($$GENERATOR_PRI): include($$GENERATOR_PRI)

QT = core gui concurrent widgets sql qml network xml quick

INCLUDEPATH += ../

HEADERS += \
    bindableowner.h \
    containers.h \
    enums.h\
    abstractclass.h \
    destruction.h \
    exceptiontest.h \
    global.h \
    injectedcode.h \
    interfaces.h \
    internal.h \
    messagehandler.h \
    metaobjectqtmetacast.h \
    namespace.h \
    nativepointertester.h \
    paintengine.h \
    performancetests.h \
    signalsandslots.h \
    smartpointertest.h \
    testdialog.h \
    threadfactory.h \
    tulip.h \
    variants.h \
    general.h \
    memorymanagement.h \
    flagsandenumtest.h \
    propertyandmethodcalltest.h \
    settingstest.h \
    multisignaltest.h


SOURCES += \
    bindableowner.cpp \
    containers.cpp \
    destruction.cpp \
    exceptiontest.cpp \
    global.cpp \
    injectedcode.cpp \
    interfaces.cpp \
    performancetests.cpp \
    polymorphictype.cpp \
    smartpointertest.cpp \
    testdialog.cpp \
    memorymanagement.cpp \
    flagsandenumtest.cpp \
    propertyandmethodcalltest.cpp \
    settingstest.cpp \
    multisignaltest.cpp \
    threadfactory.cpp \
    tulip.cpp \
    variants.cpp \
    debugtools.cpp
win32-arm64-msvc* | win32-msvc*: {
    QMAKE_CXXFLAGS += /bigobj
}

win32-g++* {
    QMAKE_CXXFLAGS += -Wa,-mbig-obj
    CONFIG(debug, debug|release) {
        QMAKE_CXXFLAGS += -O3
    }
}

macx:{
    OBJECTIVE_SOURCES += mac_util.mm
}

CONFIG += warn_on

RESOURCES += \
    resources.qrc

OTHER_FILES += autotests.qml
