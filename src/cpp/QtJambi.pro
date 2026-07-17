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

TEMPLATE = subdirs

SUBDIRS = functionpointers QtJambi
QtJambi.depends = functionpointers

contains(QTJAMBI_MODULE, QtJambiCore) {
    for(MOD, QTJAMBI_MODULE) {
        exists($$member(MOD,0)/$$member(MOD,0).pro):{
            SUBDIRS += $$MOD
            equals(MOD, QtJambiCore) {
                eval($${MOD}.depends = QtJambi)
            }else{
                eval($${MOD}.depends = QtJambiCore)
            }
        }
    }

    contains(QTJAMBI_MODULE, QtJambiQml) {
        SUBDIRS += jarimport
        jarimport.depends = QtJambiQml
    }

    contains(QTJAMBI_MODULE, QtJambiAxBase) {
        SUBDIRS += QtJambiActiveX
        QtJambiActiveX.depends = QtJambiWidgets
    }

    greaterThan(QT_MAJOR_VERSION, 5) {
        greaterThan(QT_MAJOR_VERSION, 6) | greaterThan(QT_MINOR_VERSION, 5){
        contains(QTJAMBI_MODULE, QtJambiGui-private) {
            SUBDIRS += QtJambiGuiRhi
            QtJambiGuiRhi.depends = QtJambiGui
            SUBDIRS += QtJambiGuiVulkan
            QtJambiGuiVulkan.depends = QtJambiGui
        }
        }
    }

    greaterThan(QT_MAJOR_VERSION, 6) {
        contains(QTJAMBI_MODULE, QtJambiGui-private) {
            SUBDIRS += QtJambiGuiQpa
            QtJambiGuiQpa.depends = QtJambiGui
        }
    }

    !android:!ios {
        contains(QTJAMBI_MODULE, QtJambiDesigner-private) {
            SUBDIRS += QtJambiUIC
            QtJambiUIC.depends = QtJambiWidgets
        }
        SUBDIRS += QtJambiLauncher
        QtJambiLauncher.depends = QtJambiCore

        SUBDIRS += QtJambiGenerator
        QtJambiGenerator.file = QtJambiGenerator/QtJambiGenerator.pro
        contains(QTJAMBI_MODULE, QtJambiGeneratorExec) {
            SUBDIRS += QtJambiGeneratorExec
            QtJambiGeneratorExec.file = QtJambiGenerator/QtJambiGeneratorExec.pro
            QtJambiGeneratorExec.depends = QtJambiGenerator
        }
    }

    SUBDIRS += QtJambiPlugin
    QtJambiPlugin.depends = QtJambiCore
}

contains(QT_CONFIG, release):contains(QT_CONFIG, debug) {
    # Qt was configued with both debug and release libs
    CONFIG += debug_and_release build_all
}
