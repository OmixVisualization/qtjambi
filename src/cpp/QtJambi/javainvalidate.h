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

#if !defined(QTJAMBI_JAVAINVALIDATE_H) && !defined(QTJAMBI_GENERATOR_RUNNING)
#define QTJAMBI_JAVAINVALIDATE_H

#include "global.h"

enum class QtJambiNativeID : jlong;

class QTJAMBI_EXPORT InvalidateAfterUse{
public:
    InvalidateAfterUse(JNIEnv *env, jobject object, bool checkJavaOwnership = true);
    InvalidateAfterUse(JNIEnv *env, QtJambiNativeID nativeId);
    ~InvalidateAfterUse();
    static void invalidate(JNIEnv *env, jobject java_object, bool checkJavaOwnership = true);
    static void invalidate(JNIEnv *env, QtJambiNativeID nativeId);
private:
    struct InvalidateAfterUsePrivate* p;
    Q_DISABLE_COPY_MOVE(InvalidateAfterUse)
};

#define QTJAMBI_INVALIDATE_AFTER_USE(env, object)\
    InvalidateAfterUse __qj_invalidate_##object##_after_use(env, object)

#define QTJAMBI_INVALIDATE_AFTER_USE_UNCHECKED(env, object)\
    InvalidateAfterUse __qj_invalidate_##object##_after_use(env, object, false)

class QTJAMBI_EXPORT InvalidateContainerAfterUse{
public:
    InvalidateContainerAfterUse(JNIEnv *env, jobject object, bool checkJavaOwnership = true);
    ~InvalidateContainerAfterUse();
private:
    JNIEnv *m_env;
    jobject m_object;
    bool m_checkJavaOwnership;
    Q_DISABLE_COPY_MOVE(InvalidateContainerAfterUse)
};

#define QTJAMBI_INVALIDATE_CONTAINER_AFTER_USE(env, object)\
    InvalidateContainerAfterUse __qj_invalidate_##object##_after_use(env, object)

class QTJAMBI_EXPORT InvalidateArrayAfterUse{
public:
    InvalidateArrayAfterUse(JNIEnv *env, jobjectArray object, bool checkJavaOwnership = true);
    ~InvalidateArrayAfterUse();
private:
    JNIEnv *m_env;
    jobjectArray m_object;
    bool m_checkJavaOwnership;
    Q_DISABLE_COPY_MOVE(InvalidateArrayAfterUse)
};

#define QTJAMBI_INVALIDATE_ARRAY_AFTER_USE(env, object)\
    InvalidateArrayAfterUse __qj_invalidate_##object##_after_use(env, object)

#endif // QTJAMBI_JAVAINVALIDATE_H
