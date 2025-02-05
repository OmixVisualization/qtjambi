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

SOURCES += \
    $$RXXPATH/rpp/preprocessor.cpp \
	$$RXXPATH/rpp/pp-environment.cpp \
	$$RXXPATH/rpp/pp-engine-bits.cpp \
	$$RXXPATH/rpp/pp-macro-expander.cpp

HEADERS += \
    $$RXXPATH/rpp/pp-cctype.h \
    $$RXXPATH/rpp/pp-engine-bits.h \
    $$RXXPATH/rpp/pp-environment.h \
    $$RXXPATH/rpp/pp-fwd.h \
    $$RXXPATH/rpp/pp-internal.h \
    $$RXXPATH/rpp/pp-iterator.h \
    $$RXXPATH/rpp/pp-macro-expander.h \
    $$RXXPATH/rpp/pp-macro.h \
    $$RXXPATH/rpp/pp-scanner.h \
    $$RXXPATH/rpp/pp-string.h \
    $$RXXPATH/rpp/pp-symbol.h \
    $$RXXPATH/rpp/pp.h \
    $$RXXPATH/rpp/preprocessor.h

INCLUDEPATH += $$PWD $$RXXPATH/rpp
