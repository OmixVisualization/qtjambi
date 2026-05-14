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

#if !defined(QTJAMBIAPI_STRING_H) && !defined(QTJAMBI_GENERATOR_RUNNING)
#define QTJAMBIAPI_STRING_H

#include "global.h"

namespace QtJambiAPI {
QTJAMBI_EXPORT jcharArray toJCharArray(JNIEnv *__jni_env, const jchar* in, jsize length);
QTJAMBI_EXPORT jbyteArray toJByteArray(JNIEnv *__jni_env, const jbyte* in, jsize length);
QTJAMBI_EXPORT jstring toJavaString(JNIEnv *env, jobject object);
QTJAMBI_EXPORT jobject convertQStringToJavaObject(JNIEnv *env, const QString &strg);
QTJAMBI_EXPORT jobject convertQStringToJavaObject(JNIEnv *env, QString &&strg);
QTJAMBI_EXPORT jobject convertQStringToJavaObject(JNIEnv *env, QString *strg);
QTJAMBI_EXPORT jobject convertQStringToJavaObjectAndInvalidateAfterUse(JNIEnv *env, QtJambiScope& scope, QString *strg);
QTJAMBI_EXPORT jobject convertQCharToJavaObject(JNIEnv *env, const QChar &strg);
QTJAMBI_EXPORT jobject convertQCharToJavaObject(JNIEnv *env, QChar *strg);
QTJAMBI_EXPORT bool isQStringObject(JNIEnv *env, jobject obj);
QTJAMBI_EXPORT bool isQCharObject(JNIEnv *env, jobject obj);

QTJAMBI_EXPORT bool isQByteArrayObject(JNIEnv *env, jobject obj);
QTJAMBI_EXPORT bool isQByteArrayViewObject(JNIEnv *env, jobject obj);
}

#endif // QTJAMBIAPI_STRING_H
