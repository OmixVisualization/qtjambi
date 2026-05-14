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

#if !defined(QTJAMBIAPI_CONTAINER_H) && !defined(QTJAMBI_GENERATOR_RUNNING)
#define QTJAMBIAPI_CONTAINER_H

#include "global.h"

class AbstractListAccess;
class AbstractSetAccess;
class AbstractHashAccess;
class AbstractMultiHashAccess;
class AbstractMapAccess;
class AbstractMultiMapAccess;

namespace QtJambiAPI {
enum class ListType{
    QList, QQueue, QStack
};

QTJAMBI_EXPORT jobject convertQListToJavaObject(JNIEnv *__jni_env,
                                                QtJambiNativeID owner,
                                                const void* listPtr,
                                                CopyFunction copyFunction,
                                                PtrDeleterFunction deleter,
                                                ListType listType,
                                                AbstractListAccess* listAccess
                                                );

QTJAMBI_EXPORT jobject convertQListToJavaObject(JNIEnv *__jni_env,
                                                const QSharedPointer<char>& listPtr,
                                                ListType listType,
                                                AbstractListAccess* listAccess
                                                );
QTJAMBI_EXPORT jobject convertQListToJavaObject(JNIEnv *__jni_env,
                                                const std::shared_ptr<char>& listPtr,
                                                ListType listType,
                                                AbstractListAccess* listAccess
                                                );

QTJAMBI_EXPORT jobject convertQStringListToJavaObject(JNIEnv *__jni_env,
                                                      QtJambiNativeID owner,
                                                      const void* listPtr,
                                                      CopyFunction copyFunction,
                                                      PtrDeleterFunction deleter
                                                      );

QTJAMBI_EXPORT jobject convertQStringListToJavaObject(JNIEnv *__jni_env,
                                                      const QSharedPointer<char>& listPtr
                                                      );
QTJAMBI_EXPORT jobject convertQStringListToJavaObject(JNIEnv *__jni_env,
                                                      const std::shared_ptr<char>& listPtr
                                                      );

QTJAMBI_EXPORT jobject convertQSetToJavaObject(JNIEnv *__jni_env,
                                               QtJambiNativeID owner,
                                               const void* listPtr,
                                               CopyFunction copyFunction,
                                               PtrDeleterFunction deleter,
                                               AbstractSetAccess* setAccess
                                               );

QTJAMBI_EXPORT jobject convertQSetToJavaObject(JNIEnv *__jni_env,
                                               const QSharedPointer<char>& listPtr,
                                               AbstractSetAccess* setAccess
                                               );
QTJAMBI_EXPORT jobject convertQSetToJavaObject(JNIEnv *__jni_env,
                                               const std::shared_ptr<char>& listPtr,
                                               AbstractSetAccess* setAccess
                                               );

QTJAMBI_EXPORT jobject convertQHashToJavaObject(JNIEnv *__jni_env,
                                                QtJambiNativeID owner,
                                                const void* listPtr,
                                                CopyFunction copyFunction,
                                                PtrDeleterFunction deleter,
                                                AbstractHashAccess* hashAccess
                                                );

QTJAMBI_EXPORT jobject convertQHashToJavaObject(JNIEnv *__jni_env,
                                                const QSharedPointer<char>& listPtr,
                                                AbstractHashAccess* hashAccess
                                                );
QTJAMBI_EXPORT jobject convertQHashToJavaObject(JNIEnv *__jni_env,
                                                const std::shared_ptr<char>& listPtr,
                                                AbstractHashAccess* hashAccess
                                                );

QTJAMBI_EXPORT jobject convertQMultiHashToJavaObject(JNIEnv *__jni_env,
                                                     QtJambiNativeID owner,
                                                     const void* listPtr,
                                                     CopyFunction copyFunction,
                                                     PtrDeleterFunction deleter,
                                                     AbstractMultiHashAccess* multiHashAccess
                                                     );

QTJAMBI_EXPORT jobject convertQMultiHashToJavaObject(JNIEnv *__jni_env,
                                                     const QSharedPointer<char>& listPtr,
                                                     AbstractMultiHashAccess* multiHashAccess
                                                     );
QTJAMBI_EXPORT jobject convertQMultiHashToJavaObject(JNIEnv *__jni_env,
                                                     const std::shared_ptr<char>& listPtr,
                                                     AbstractMultiHashAccess* multiHashAccess
                                                     );

QTJAMBI_EXPORT jobject convertQMapToJavaObject(JNIEnv *__jni_env,
                                               QtJambiNativeID owner,
                                               const void* listPtr,
                                               CopyFunction copyFunction,
                                               PtrDeleterFunction deleter,
                                               AbstractMapAccess* mapAccess
                                               );

QTJAMBI_EXPORT jobject convertQMapToJavaObject(JNIEnv *__jni_env,
                                               const QSharedPointer<char>& listPtr,
                                               AbstractMapAccess* mapAccess
                                               );

QTJAMBI_EXPORT jobject convertQMapToJavaObject(JNIEnv *__jni_env,
                                               const std::shared_ptr<char>& listPtr,
                                               AbstractMapAccess* mapAccess
                                               );

QTJAMBI_EXPORT jobject convertQMultiMapToJavaObject(JNIEnv *__jni_env,
                                                    QtJambiNativeID owner,
                                                    const void* listPtr,
                                                    CopyFunction copyFunction,
                                                    PtrDeleterFunction deleter,
                                                    AbstractMultiMapAccess* mapAccess
                                                    );


QTJAMBI_EXPORT jobject convertQMultiMapToJavaObject(JNIEnv *__jni_env,
                                                    const QSharedPointer<char>& listPtr,
                                                    AbstractMultiMapAccess* mapAccess
                                                    );
QTJAMBI_EXPORT jobject convertQMultiMapToJavaObject(JNIEnv *__jni_env,
                                                    const std::shared_ptr<char>& listPtr,
                                                    AbstractMultiMapAccess* mapAccess
                                                    );
}

#endif // QTJAMBIAPI_CONTAINER_H
