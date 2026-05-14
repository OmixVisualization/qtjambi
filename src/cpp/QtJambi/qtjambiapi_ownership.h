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

#if !defined(QTJAMBIAPI_OWNERSHIP_H) && !defined(QTJAMBI_GENERATOR_RUNNING)
#define QTJAMBIAPI_OWNERSHIP_H

#include "global.h"

enum class QtJambiNativeID : jlong;

namespace QtJambiAPI {
template<class T>
T& checkedAddressOf(JNIEnv *env, T * ptr);

template<typename T>
const T& getDefaultValue();

QTJAMBI_EXPORT void setJavaOwnershipForTopLevelObject(JNIEnv *env, QObject* qobject);

QTJAMBI_EXPORT void setCppOwnershipForTopLevelObject(JNIEnv *env, QObject* qobject);

QTJAMBI_EXPORT void setDefaultOwnershipForTopLevelObject(JNIEnv *env, QObject* qobject);

QTJAMBI_EXPORT void setJavaOwnership(JNIEnv *env, jobject object);

QTJAMBI_EXPORT void setCppOwnership(JNIEnv *env, jobject object);

QTJAMBI_EXPORT bool isSplitOwnership(JNIEnv *env, jobject object);

QTJAMBI_EXPORT bool isCppOwnership(JNIEnv *env, jobject object);

QTJAMBI_EXPORT bool isJavaOwnership(JNIEnv *env, jobject object);

QTJAMBI_EXPORT void setCppOwnershipAndInvalidate(JNIEnv *env, jobject object);

QTJAMBI_EXPORT void setDefaultOwnership(JNIEnv *env, jobject object);

QTJAMBI_EXPORT void changeSplitToCppOwnership(JNIEnv *env, jobject object);

QTJAMBI_EXPORT bool isSplitOwnership(const QObject* object);

QTJAMBI_EXPORT bool isCppOwnership(const QObject* object);

QTJAMBI_EXPORT bool isJavaOwnership(const QObject* object);

QTJAMBI_EXPORT bool isSplitOwnership(const void* object);

QTJAMBI_EXPORT bool isCppOwnership(const void* object);

QTJAMBI_EXPORT bool isJavaOwnership(const void* object);

QTJAMBI_EXPORT void setJavaOwnership(JNIEnv *env, QtJambiNativeID objectId);

QTJAMBI_EXPORT void setCppOwnership(JNIEnv *env, QtJambiNativeID objectId);

QTJAMBI_EXPORT void changeSplitToCppOwnership(JNIEnv *env, QtJambiNativeID objectId);

QTJAMBI_EXPORT void setCppOwnershipAndInvalidate(JNIEnv *env, QtJambiNativeID objectId);

QTJAMBI_EXPORT void setDefaultOwnership(JNIEnv *env, QtJambiNativeID objectId);

QTJAMBI_EXPORT bool isSplitOwnership(QtJambiNativeID objectId);

QTJAMBI_EXPORT bool isCppOwnership(QtJambiNativeID objectId);

QTJAMBI_EXPORT bool isJavaOwnership(QtJambiNativeID objectId);

QTJAMBI_EXPORT void registerDependency(JNIEnv *env, jobject dependentObject, QtJambiNativeID _this_nativeId);

}

#endif // QTJAMBIAPI_OWNERSHIP_H
