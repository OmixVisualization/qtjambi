/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
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
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#if !defined(QTJAMBIAPI_THREAD_H) && !defined(QTJAMBI_GENERATOR_RUNNING)
#define QTJAMBIAPI_THREAD_H

#include "global.h"

namespace QtJambiAPI {
QTJAMBI_EXPORT void checkThread(JNIEnv *env, const QObject* object);

QTJAMBI_EXPORT void checkThread(JNIEnv *env, const std::type_info& argumentType, const void* object);

QTJAMBI_EXPORT void checkMainThread(JNIEnv *env, const std::type_info& typeId);

QTJAMBI_EXPORT void checkThreadOnArgument(JNIEnv *env, const char* argumentName, const QObject* argument);

QTJAMBI_EXPORT void checkThreadOnArgument(JNIEnv *env, const char* argumentName, const std::type_info& argumentType, const QObject* argumentOwner);

QTJAMBI_EXPORT void checkThreadOnArgument(JNIEnv *env, const char* argumentName, const std::type_info& argumentType, const void* argument);

QTJAMBI_EXPORT void checkMainThreadOnArgument(JNIEnv *env, const char* argumentName, const std::type_info& argumentType);

QTJAMBI_EXPORT void checkMainThreadConstructing(JNIEnv *env, const std::type_info& constructedType);

QTJAMBI_EXPORT void checkThreadOnParent(JNIEnv *env, const QObject* parent);

QTJAMBI_EXPORT void checkThreadOnParent(JNIEnv *env, const std::type_info& parentType, const void* parent);

QTJAMBI_EXPORT void checkThreadOnParent(JNIEnv *env, const std::type_info& parentType, const QObject* parentOwner);

QTJAMBI_EXPORT void checkThreadConstructingQWindow(JNIEnv *env, const std::type_info& constructedType, const QObject* parent);

QTJAMBI_EXPORT void checkThreadConstructingQPixmap(JNIEnv *env, const std::type_info& constructedType);

QTJAMBI_EXPORT void checkThreadConstructingQWidget(JNIEnv *env, const std::type_info& constructedType, const QObject* parent);

QTJAMBI_EXPORT void checkThreadOnArgumentQPixmap(JNIEnv *env, const char* argumentName, const std::type_info& argumentType);

QTJAMBI_EXPORT void checkThreadQPixmap(JNIEnv *env, const std::type_info& typeId);
}

#endif // QTJAMBIAPI_THREAD_H
