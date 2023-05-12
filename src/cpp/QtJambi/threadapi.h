/****************************************************************************
**
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#ifndef QTJAMBI_THREADAPI_H
#define QTJAMBI_THREADAPI_H

#include "global.h"

class QThread;

namespace ThreadAPI{

QTJAMBI_EXPORT jobject findJThreadForQThread(JNIEnv *env, jobject qt_thread);

QTJAMBI_EXPORT jobject findQThreadForJThread(JNIEnv *env, jobject thread);

QTJAMBI_EXPORT void initializeCurrentThread(JNIEnv *env);

QTJAMBI_EXPORT void initializeMainThread(JNIEnv *env);

QTJAMBI_EXPORT void initializeThread(JNIEnv *__jni_env, jobject thread, jobject threadGroup);

QTJAMBI_EXPORT void setDaemon(JNIEnv *__jni_env, jobject thread, bool daemon);

QTJAMBI_EXPORT bool isDaemon(QThread* thread);

QTJAMBI_EXPORT void setName(JNIEnv *__jni_env, jobject thread, jstring name);

QTJAMBI_EXPORT jstring getName(JNIEnv *__jni_env, QThread* thread);

QTJAMBI_EXPORT jobject getThreadGroup(JNIEnv *__jni_env, QThread* thread);

QTJAMBI_EXPORT void setUncaughtExceptionHandler(JNIEnv *__jni_env, jobject thread, jobject handler);

QTJAMBI_EXPORT jobject getUncaughtExceptionHandler(JNIEnv *__jni_env, QThread* thread);

QTJAMBI_EXPORT void setContextClassLoader(JNIEnv *__jni_env, jobject thread, jobject cl);

QTJAMBI_EXPORT jobject getContextClassLoader(JNIEnv *__jni_env, QThread* thread);

}

#endif // QTJAMBI_THREADAPI_H
