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

#if !defined(QTJAMBI_JAVAINVALIDATE_H) && !defined(QTJAMBI_GENERATOR_RUNNING)
#define QTJAMBI_JAVAINVALIDATE_H

#include "global.h"

enum class QtJambiNativeID : jlong;

struct InvalidateAfterUsePrivate;

class QTJAMBI_EXPORT AbstractInvalidateAfterUse{
public:
    ~AbstractInvalidateAfterUse();
protected:
    AbstractInvalidateAfterUse(InvalidateAfterUsePrivate& p);
private:
    InvalidateAfterUsePrivate* p;
    void* operator new(size_t) = delete;
    void* operator new(size_t,size_t) = delete;
    void* operator new[](size_t) = delete;
    Q_DISABLE_COPY_MOVE(AbstractInvalidateAfterUse)
};

class QTJAMBI_EXPORT InvalidateAfterUse : public AbstractInvalidateAfterUse{
public:
    InvalidateAfterUse(JNIEnv *env, jobject object);
    InvalidateAfterUse(JNIEnv *env, QtJambiNativeID nativeId);
    static void invalidate(JNIEnv *env, jobject java_object);
    static void invalidate(JNIEnv *env, QtJambiNativeID nativeId);
    static void forcedInvalidate(JNIEnv *env, jobject java_object);
    static void forcedInvalidate(JNIEnv *env, QtJambiNativeID nativeId);
    Q_DISABLE_COPY_MOVE(InvalidateAfterUse)
    void* operator new(size_t) = delete;
    void* operator new(size_t,size_t) = delete;
    void* operator new[](size_t) = delete;
};

class QTJAMBI_EXPORT ForcedInvalidateAfterUse : public AbstractInvalidateAfterUse{
public:
    ForcedInvalidateAfterUse(JNIEnv *env, jobject object);
    ForcedInvalidateAfterUse(JNIEnv *env, QtJambiNativeID nativeId);
private:
    Q_DISABLE_COPY_MOVE(ForcedInvalidateAfterUse)
    void* operator new(size_t) = delete;
    void* operator new(size_t,size_t) = delete;
    void* operator new[](size_t) = delete;
};

#define QTJAMBI_INVALIDATE_AFTER_USE(env, object)\
    InvalidateAfterUse __qj_invalidate_##object##_after_use(env, object)

#define QTJAMBI_FORCED_INVALIDATE_AFTER_USE(env, object)\
    ForcedInvalidateAfterUse __qj_invalidate_##object##_after_use(env, object)

class QTJAMBI_EXPORT InvalidateContainerAfterUse : public AbstractInvalidateAfterUse{
public:
    InvalidateContainerAfterUse(JNIEnv *env, jobject object);
private:
    Q_DISABLE_COPY_MOVE(InvalidateContainerAfterUse)
    void* operator new(size_t) = delete;
    void* operator new(size_t,size_t) = delete;
    void* operator new[](size_t) = delete;
};

class QTJAMBI_EXPORT ForcedInvalidateContainerAfterUse : public AbstractInvalidateAfterUse{
public:
    ForcedInvalidateContainerAfterUse(JNIEnv *env, jobject object);
private:
    Q_DISABLE_COPY_MOVE(ForcedInvalidateContainerAfterUse)
    void* operator new(size_t) = delete;
    void* operator new(size_t,size_t) = delete;
    void* operator new[](size_t) = delete;
};

#define QTJAMBI_INVALIDATE_CONTAINER_AFTER_USE(env, object)\
    InvalidateContainerAfterUse __qj_invalidate_##object##_after_use(env, object)
#define QTJAMBI_FORCED_INVALIDATE_CONTAINER_AFTER_USE(env, object)\
    ForcedInvalidateContainerAfterUse __qj_invalidate_##object##_after_use(env, object)

class QTJAMBI_EXPORT InvalidateArrayAfterUse : public AbstractInvalidateAfterUse{
public:
    InvalidateArrayAfterUse(JNIEnv *env, jobjectArray object);
private:
    Q_DISABLE_COPY_MOVE(InvalidateArrayAfterUse)
    void* operator new(size_t) = delete;
    void* operator new(size_t,size_t) = delete;
    void* operator new[](size_t) = delete;
};

class QTJAMBI_EXPORT ForcedInvalidateArrayAfterUse : public AbstractInvalidateAfterUse{
public:
    ForcedInvalidateArrayAfterUse(JNIEnv *env, jobjectArray object);
private:
    Q_DISABLE_COPY_MOVE(ForcedInvalidateArrayAfterUse)
    void* operator new(size_t) = delete;
    void* operator new(size_t,size_t) = delete;
    void* operator new[](size_t) = delete;
};

#define QTJAMBI_INVALIDATE_ARRAY_AFTER_USE(env, object)\
    InvalidateArrayAfterUse __qj_invalidate_##object##_after_use(env, object)
#define QTJAMBI_FORCED_INVALIDATE_ARRAY_AFTER_USE(env, object)\
ForcedInvalidateArrayAfterUse __qj_invalidate_##object##_after_use(env, object)

#endif // QTJAMBI_JAVAINVALIDATE_H
