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

# because QActionEvent refers to qaction.h
macx:{
    INCLUDEPATH += $$[QT_INSTALL_LIBS]/QtGui.framework/Headers/qpa
    INCLUDEPATH += $$[QT_INSTALL_LIBS]/QtWidgets.framework/Headers
    INCLUDEPATH += $$[QT_INSTALL_HEADERS]
}
INCLUDEPATH += $$[QT_INSTALL_HEADERS]/QtGui/qpa
INCLUDEPATH += $$[QT_INSTALL_HEADERS]/QtWidgets
INCLUDEPATH += $$[QT_INSTALL_HEADERS]/QtPlatformHeaders

QT = core core-private gui gui-private
CONFIG+=no_private_qt_headers_warning

SOURCES += \
    impl.cpp

HEADERS += \
    classes_p.h \
    hashes.h \
    utils_p.h

OBJECTIVE_SOURCES += utils.mm

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
    QMAKE_CXXFLAGS += -Wa,-mbig-obj
    CONFIG(debug, debug|release) {
        QMAKE_CXXFLAGS += -O3
    }
}

