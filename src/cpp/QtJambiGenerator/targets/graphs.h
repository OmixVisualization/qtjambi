/****************************************************************************
**
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#define QTJAMBI_CORE_H
#define QQMLPRIVATE_H
#define QML_GETTYPENAMES
#define QGENERICMATRIX_H
#define QTJAMBI_UTILS_H
#define QTJAMBI_REGISTRY_H
#define QT_KEYPAD_NAVIGATION

#define QT_NO_STL
#define Q_BYTE_ORDER Q_BYTE_ORDER

//qtjambi preprocessor does not understand properly
#define GL_APIENTRY
#define Q_COMPILER_CONSTEXPR
#define Q_COMPILER_UNIFORM_INIT

#undef Q_STATIC_ASSERT
#undef Q_STATIC_ASSERT_X

#include <QtCore/qcompilerdetection.h>

#define seed_seq initializer_list<uint>const

#include <QtCore/qstringliteral.h>
#undef QStringLiteral
#define QStringLiteral QString
typedef struct __GLsync *GLsync;

#if QT_VERSION >= QT_VERSION_CHECK(6, 6, 0)

#ifndef QTJAMBI_NO_GRAPHS
#include <QtGraphs/QtGraphs>
#include <QtJambiGraphs/hashes.h>
#endif

#endif
