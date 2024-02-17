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

TEMPLATE = subdirs

SUBDIRS = QtJambiCore

for(MOD, QTJAMBI_MODULE) : {
    exists($$member(MOD,0)/$$member(MOD,0).pro):SUBDIRS += $$MOD
}

contains(QTJAMBI_MODULE, QtJambiQml): SUBDIRS += jarimport

contains(QTJAMBI_MODULE, QtJambiAxBase): SUBDIRS += QtJambiActiveX

greaterThan(QT_MAJOR_VERSION, 6):{
    contains(QTJAMBI_MODULE, QtJambiGui-private): SUBDIRS += QtJambiGuiQpa
}

equals(QT_MAJOR_VERSION, 6):greaterThan(QT_MINOR_VERSION, 5):{
    contains(QTJAMBI_MODULE, QtJambiGui-private): SUBDIRS += QtJambiGuiRhi
}
greaterThan(QT_MAJOR_VERSION, 6):{
    contains(QTJAMBI_MODULE, QtJambiGui-private): SUBDIRS += QtJambiGuiRhi
}

!android:!ios:{
    contains(QTJAMBI_MODULE, QtJambiDesigner-private): SUBDIRS += QtJambiUIC
    SUBDIRS += QtJambiLauncher\
    QtJambiGenerator
    QtJambiGenerator.file = QtJambiGenerator/library.pro
}

SUBDIRS += QtJambiPlugin
