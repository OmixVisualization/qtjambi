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

#if !defined(QTJAMBIAPI_MODEL_H) && !defined(QTJAMBI_GENERATOR_RUNNING)
#define QTJAMBIAPI_MODEL_H

#include "global.h"

class QModelIndex;
class QModelRoleData;
class QModelRoleDataSpan;

namespace QtJambiAPI {
QTJAMBI_EXPORT jobject convertModelIndexToJavaObject(JNIEnv *env, QModelIndex&& index);
QTJAMBI_EXPORT jobject convertModelIndexToJavaObject(JNIEnv *env, const QModelIndex& index);
QTJAMBI_EXPORT jobject convertModelIndexToJavaObject(JNIEnv *env, const QModelIndex* index);
QTJAMBI_EXPORT jobject convertModelIndexToEphemeralJavaObject(JNIEnv *env, QtJambiScope& scope, const QModelIndex* index);
QTJAMBI_EXPORT jobject convertModelIndexToEphemeralJavaObject(JNIEnv *env, QtJambiScope& scope, const QModelIndex& index);
QTJAMBI_EXPORT void convertJavaObjectToQModelRoleData(JNIEnv *env, QtJambiScope& scope, jobject java_object, QModelRoleData * &data, qsizetype &length);
QTJAMBI_EXPORT bool convertJavaToModelIndex(JNIEnv *env, jobject java_object, QModelIndex& output);
QTJAMBI_EXPORT bool convertJavaToModelIndex(JNIEnv *env, jobject java_object, QModelIndex*& output
#if defined(QTJAMBI_LIGHTWEIGHT_MODELINDEX)
                                            , QtJambiScope& scope
#endif
                                            );
}

#endif // QTJAMBIAPI_MODEL_H
