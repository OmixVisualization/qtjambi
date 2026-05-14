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

#if !defined(QTJAMBIAPI_TIME_H) && !defined(QTJAMBI_GENERATOR_RUNNING)
#define QTJAMBIAPI_TIME_H

#include "global.h"

namespace QtJambiAPI {
QTJAMBI_EXPORT jobject convertDuration(JNIEnv *env, std::chrono::nanoseconds t);
QTJAMBI_EXPORT jobject convertDuration(JNIEnv *env, std::chrono::seconds t);
QTJAMBI_EXPORT jobject convertDuration(JNIEnv *env, std::chrono::milliseconds t);
QTJAMBI_EXPORT std::chrono::nanoseconds convertDuration(JNIEnv *env, jobject t, std::chrono::nanoseconds defaultValue = std::chrono::nanoseconds::zero());
QTJAMBI_EXPORT std::chrono::seconds convertDuration(JNIEnv *env, jobject t, std::chrono::seconds defaultValue = std::chrono::seconds::zero());
QTJAMBI_EXPORT std::chrono::milliseconds convertDuration(JNIEnv *env, jobject t, std::chrono::milliseconds defaultValue = std::chrono::milliseconds::zero());
QTJAMBI_EXPORT jobject convertTimePointFromEpoch(JNIEnv *env, std::chrono::seconds t);
QTJAMBI_EXPORT jobject convertTimePointFromEpoch(JNIEnv *env, std::chrono::nanoseconds t);
QTJAMBI_EXPORT jobject convertTimePointFromEpoch(JNIEnv *env, std::chrono::milliseconds t);
QTJAMBI_EXPORT std::chrono::nanoseconds convertTimePointFromEpoch(JNIEnv *env, jobject t, std::chrono::nanoseconds defaultValue = std::chrono::nanoseconds::zero());
QTJAMBI_EXPORT std::chrono::seconds convertTimePointFromEpoch(JNIEnv *env, jobject t, std::chrono::seconds defaultValue = std::chrono::seconds::zero());
QTJAMBI_EXPORT std::chrono::milliseconds convertTimePointFromEpoch(JNIEnv *env, jobject t, std::chrono::milliseconds defaultValue = std::chrono::milliseconds::zero());
QTJAMBI_EXPORT QPair<std::chrono::seconds, std::chrono::nanoseconds> readDuration(JNIEnv *env, jobject t);
QTJAMBI_EXPORT QPair<std::chrono::seconds, std::chrono::nanoseconds> readTimePoint(JNIEnv *env, jobject t);
}

#endif // QTJAMBIAPI_TIME_H
