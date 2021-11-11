/****************************************************************************
**
** Copyright (C) 2009-2021 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef QTJAMBI_THREAD_H
#define QTJAMBI_THREAD_H

#include "qtjambi_global.h"

class QThread;

QTJAMBI_EXPORT jobject qtjambi_get_qthread_for_jthread(JNIEnv *env, jobject thread);

QTJAMBI_EXPORT void qtjambi_initialize_current_thread(JNIEnv *env);

QTJAMBI_EXPORT void qtjambi_initialize_main_thread(JNIEnv *env);

QTJAMBI_EXPORT void qtjambi_initialize_thread(JNIEnv *__jni_env, jobject thread, jobject threadGroup);

QTJAMBI_EXPORT void qtjambi_thread_set_daemon(JNIEnv *__jni_env, jobject thread, bool daemon);

QTJAMBI_EXPORT bool qtjambi_thread_is_daemon(QThread* thread);

QTJAMBI_EXPORT void qtjambi_thread_set_name(JNIEnv *__jni_env, jobject thread, jstring name);

QTJAMBI_EXPORT jstring qtjambi_thread_get_name(JNIEnv *__jni_env, QThread* thread);

QTJAMBI_EXPORT jobject qtjambi_thread_get_group(JNIEnv *__jni_env, QThread* thread);

QTJAMBI_EXPORT void qtjambi_thread_set_UncaughtExceptionHandler(JNIEnv *__jni_env, jobject thread, jobject handler);

QTJAMBI_EXPORT jobject qtjambi_thread_get_UncaughtExceptionHandler(JNIEnv *__jni_env, QThread* thread);

QTJAMBI_EXPORT void qtjambi_thread_set_ContextClassLoader(JNIEnv *__jni_env, jobject thread, jobject cl);

QTJAMBI_EXPORT jobject qtjambi_thread_get_ContextClassLoader(JNIEnv *__jni_env, QThread* thread);

#endif // QTJAMBI_THREAD_H
