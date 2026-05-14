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

#if !defined(QTJAMBIAPI_BOXED_H) && !defined(QTJAMBI_GENERATOR_RUNNING)
#define QTJAMBIAPI_BOXED_H

#include "global.h"

namespace QtJambiAPI {
QTJAMBI_EXPORT jobject toJavaIntegerObject(JNIEnv *env, jint int_value);
QTJAMBI_EXPORT jobject toJavaDoubleObject(JNIEnv *env, jdouble double_value);
QTJAMBI_EXPORT jobject toJavaBooleanObject(JNIEnv *env, jboolean bool_value);
QTJAMBI_EXPORT jobject toJavaLongObject(JNIEnv *env, jlong long_value);
QTJAMBI_EXPORT jobject toJavaShortObject(JNIEnv *env, jshort short_value);
QTJAMBI_EXPORT jobject toJavaFloatObject(JNIEnv *env, jfloat float_value);
QTJAMBI_EXPORT jobject toJavaByteObject(JNIEnv *env, jbyte byte_value);
QTJAMBI_EXPORT jobject toJavaCharacterObject(JNIEnv *env, jchar char_value);

QTJAMBI_EXPORT jdouble fromJavaDoubleObject(JNIEnv *env, jobject double_object);
QTJAMBI_EXPORT jint fromJavaIntegerObject(JNIEnv *env, jobject int_object);
QTJAMBI_EXPORT bool fromJavaBooleanObject(JNIEnv *env, jobject bool_object);
QTJAMBI_EXPORT jlong fromJavaLongObject(JNIEnv *env, jobject long_object);
QTJAMBI_EXPORT jchar fromJavaCharacterObject(JNIEnv *env, jobject char_object);
QTJAMBI_EXPORT jfloat fromJavaFloatObject(JNIEnv *env, jobject float_object);
QTJAMBI_EXPORT jshort fromJavaShortObject(JNIEnv *env, jobject short_object);
QTJAMBI_EXPORT jbyte fromJavaByteObject(JNIEnv *env, jobject byte_object);
}

#endif // QTJAMBIAPI_BOXED_H
