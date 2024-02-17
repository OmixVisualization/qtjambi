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

isEmpty(RXXPATH):RXXPATH = $$PWD

INCLUDEPATH += $$RXXPATH

DEFINES += RXX_ALLOCATOR_INIT_0

HEADERS += $$RXXPATH/ast.h \
           $$RXXPATH/lexer.h \
           $$RXXPATH/list.h \
           $$RXXPATH/parser.h \
           $$RXXPATH/rxx_allocator.h \
           $$RXXPATH/rpp-allocator.h \
           $$RXXPATH/smallobject.h \
           $$RXXPATH/tokens.h \
           $$RXXPATH/symbol.h \
           $$RXXPATH/control.h \
           $$RXXPATH/visitor.h \
           $$RXXPATH/default_visitor.h \
           $$RXXPATH/dumptree.h \
           $$RXXPATH/binder.h \
           $$RXXPATH/codemodel.h \
           $$RXXPATH/codemodel_fwd.h \
           $$RXXPATH/type_compiler.h \
           $$RXXPATH/name_compiler.h \
           $$RXXPATH/declarator_compiler.h \
           $$RXXPATH/class_compiler.h \
           $$RXXPATH/codemodel_finder.h \
           $$RXXPATH/compiler_utils.h
SOURCES += $$RXXPATH/ast.cpp \
           $$RXXPATH/lexer.cpp \
           $$RXXPATH/list.cpp \
           $$RXXPATH/parser.cpp \
           $$RXXPATH/smallobject.cpp \
           $$RXXPATH/control.cpp \
           $$RXXPATH/visitor.cpp \
           $$RXXPATH/default_visitor.cpp \
           $$RXXPATH/dumptree.cpp \
           $$RXXPATH/tokens.cpp \
           $$RXXPATH/binder.cpp \
           $$RXXPATH/codemodel.cpp \
           $$RXXPATH/type_compiler.cpp \
           $$RXXPATH/name_compiler.cpp \
           $$RXXPATH/declarator_compiler.cpp \
           $$RXXPATH/class_compiler.cpp \
           $$RXXPATH/codemodel_finder.cpp \
           $$RXXPATH/compiler_utils.cpp
