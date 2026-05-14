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

#if !defined(QTJAMBIAPI_OPTIONAL_H) && !defined(QTJAMBI_GENERATOR_RUNNING)
#define QTJAMBIAPI_OPTIONAL_H

#include "global.h"

namespace QtJambiAPI {
template<class T>
T& checkedAddressOf(JNIEnv *env, T * ptr);

template<typename T>
const T& getDefaultValue();

QTJAMBI_EXPORT jobject newJavaOptional(JNIEnv *env, bool hasValue, jobject object);
QTJAMBI_EXPORT jobject newJavaOptionalInt(JNIEnv *env, bool hasValue, jint value);
QTJAMBI_EXPORT jobject newJavaOptionalLong(JNIEnv *env, bool hasValue, jlong value);
QTJAMBI_EXPORT jobject newJavaOptionalDouble(JNIEnv *env, bool hasValue, jdouble value);
QTJAMBI_EXPORT jobject readJavaOptional(JNIEnv *env, jobject object, bool& isPresent);
QTJAMBI_EXPORT jint readJavaOptionalInt(JNIEnv *env, jobject object, bool& isPresent);
QTJAMBI_EXPORT jlong readJavaOptionalLong(JNIEnv *env, jobject object, bool& isPresent);
QTJAMBI_EXPORT jdouble readJavaOptionalDouble(JNIEnv *env, jobject object, bool& isPresent);
}

#endif // QTJAMBIAPI_OPTIONAL_H
