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

#if !defined(QTJAMBIAPI_ITERATOR_H) && !defined(QTJAMBI_GENERATOR_RUNNING)
#define QTJAMBIAPI_ITERATOR_H

#include "global.h"
#include "typetests.h"

class AbstractSequentialConstIteratorAccess;
class AbstractAssociativeConstIteratorAccess;
class AbstractSequentialIteratorAccess;
class AbstractAssociativeIteratorAccess;
enum class QtJambiNativeID : jlong;

namespace QtJambiAPI {
QTJAMBI_EXPORT jobject convertIteratorToJavaObject(JNIEnv *env,
                                                   jobject owner,
                                                   void* iteratorPtr,
                                                   PtrDeleterFunction destructor_function,
                                                   AbstractSequentialConstIteratorAccess* access);
QTJAMBI_EXPORT jobject convertIteratorToJavaObject(JNIEnv *env,
                                                   jobject owner,
                                                   void* iteratorPtr,
                                                   PtrDeleterFunction destructor_function,
                                                   AbstractAssociativeConstIteratorAccess* access);
QTJAMBI_EXPORT jobject convertIteratorToJavaObject(JNIEnv *env,
                                                   QtJambiNativeID owner,
                                                   void* iteratorPtr,
                                                   PtrDeleterFunction destructor_function,
                                                   AbstractSequentialConstIteratorAccess* access);
QTJAMBI_EXPORT jobject convertIteratorToJavaObject(JNIEnv *env,
                                                   QtJambiNativeID owner,
                                                   void* iteratorPtr,
                                                   PtrDeleterFunction destructor_function,
                                                   AbstractAssociativeConstIteratorAccess* access);
QTJAMBI_EXPORT jobject convertIteratorToJavaObject(JNIEnv *env,
                                                   void* iteratorPtr,
                                                   PtrDeleterFunction destructor_function,
                                                   AbstractSequentialConstIteratorAccess* access);
QTJAMBI_EXPORT jobject convertIteratorToJavaObject(JNIEnv *env,
                                                   void* iteratorPtr,
                                                   PtrDeleterFunction destructor_function,
                                                   AbstractAssociativeConstIteratorAccess* access);

QTJAMBI_EXPORT jobject convertListIteratorToJavaObject(JNIEnv *env,
                                                       void* iteratorPtr,
                                                       PtrDeleterFunction destructor_function,
                                                       AbstractSequentialConstIteratorAccess* access);
QTJAMBI_EXPORT jobject convertListReverseIteratorToJavaObject(JNIEnv *env,
                                                       void* iteratorPtr,
                                                       PtrDeleterFunction destructor_function,
                                                       AbstractSequentialConstIteratorAccess* access);
QTJAMBI_EXPORT jobject convertSpanIteratorToJavaObject(JNIEnv *env,
                                                       void* iteratorPtr,
                                                       PtrDeleterFunction destructor_function,
                                                       AbstractSequentialConstIteratorAccess* access);
QTJAMBI_EXPORT jobject convertSpanReverseIteratorToJavaObject(JNIEnv *env,
                                                       void* iteratorPtr,
                                                       PtrDeleterFunction destructor_function,
                                                       AbstractSequentialConstIteratorAccess* access);
QTJAMBI_EXPORT jobject convertSetIteratorToJavaObject(JNIEnv *env,
                                                      void* iteratorPtr,
                                                      PtrDeleterFunction destructor_function,
                                                      AbstractSequentialConstIteratorAccess* access);
QTJAMBI_EXPORT jobject convertMapIteratorToJavaObject(JNIEnv *env,
                                                      void* iteratorPtr,
                                                      PtrDeleterFunction destructor_function,
                                                      AbstractAssociativeConstIteratorAccess* access);
QTJAMBI_EXPORT jobject convertHashIteratorToJavaObject(JNIEnv *env,
                                                       void* iteratorPtr,
                                                       PtrDeleterFunction destructor_function,
                                                       AbstractAssociativeConstIteratorAccess* access);
QTJAMBI_EXPORT jobject convertMultiMapIteratorToJavaObject(JNIEnv *env,
                                                      void* iteratorPtr,
                                                      PtrDeleterFunction destructor_function,
                                                      AbstractAssociativeConstIteratorAccess* access);
QTJAMBI_EXPORT jobject convertMultiHashIteratorToJavaObject(JNIEnv *env,
                                                       void* iteratorPtr,
                                                       PtrDeleterFunction destructor_function,
                                                       AbstractAssociativeConstIteratorAccess* access);
QTJAMBI_EXPORT jobject convertMapKeyIteratorToJavaObject(JNIEnv *env,
                                                         void* iteratorPtr,
                                                         PtrDeleterFunction destructor_function,
                                                         AbstractSequentialConstIteratorAccess* access);
QTJAMBI_EXPORT jobject convertHashKeyIteratorToJavaObject(JNIEnv *env,
                                                       void* iteratorPtr,
                                                       PtrDeleterFunction destructor_function,
                                                       AbstractSequentialConstIteratorAccess* access);
QTJAMBI_EXPORT jobject convertMultiMapKeyIteratorToJavaObject(JNIEnv *env,
                                                           void* iteratorPtr,
                                                           PtrDeleterFunction destructor_function,
                                                           AbstractSequentialConstIteratorAccess* access);
QTJAMBI_EXPORT jobject convertMultiHashKeyIteratorToJavaObject(JNIEnv *env,
                                                            void* iteratorPtr,
                                                            PtrDeleterFunction destructor_function,
                                                            AbstractSequentialConstIteratorAccess* access);
QTJAMBI_EXPORT jobject convertMapKeyValueIteratorToJavaObject(JNIEnv *env,
                                                              void* iteratorPtr,
                                                              PtrDeleterFunction destructor_function,
                                                              AbstractSequentialConstIteratorAccess* access);
QTJAMBI_EXPORT jobject convertHashKeyValueIteratorToJavaObject(JNIEnv *env,
                                                       void* iteratorPtr,
                                                       PtrDeleterFunction destructor_function,
                                                       AbstractSequentialConstIteratorAccess* access);
QTJAMBI_EXPORT jobject convertMultiMapKeyValueIteratorToJavaObject(JNIEnv *env,
                                                           void* iteratorPtr,
                                                           PtrDeleterFunction destructor_function,
                                                           AbstractSequentialConstIteratorAccess* access);
QTJAMBI_EXPORT jobject convertMultiHashKeyValueIteratorToJavaObject(JNIEnv *env,
                                                            void* iteratorPtr,
                                                            PtrDeleterFunction destructor_function,
                                                            AbstractSequentialConstIteratorAccess* access);

QTJAMBI_EXPORT jobject convertIteratorToJavaObject(JNIEnv *env,
                                                   const std::type_info& containerTypeId,
                                                   const std::type_info& iteratorTypeId,
                                                   jobject owner,
                                                   void* iteratorPtr,
                                                   PtrDeleterFunction destructor_function,
                                                   AbstractSequentialConstIteratorAccess* access);
QTJAMBI_EXPORT jobject convertIteratorToJavaObject(JNIEnv *env,
                                                   const std::type_info& containerTypeId,
                                                   const std::type_info& iteratorTypeId,
                                                   jobject owner,
                                                   void* iteratorPtr,
                                                   PtrDeleterFunction destructor_function,
                                                   AbstractAssociativeConstIteratorAccess* access);
QTJAMBI_EXPORT jobject convertIteratorToJavaObject(JNIEnv *env,
                                                   const std::type_info& containerTypeId,
                                                   const std::type_info& iteratorTypeId,
                                                   QtJambiNativeID owner,
                                                   void* iteratorPtr,
                                                   PtrDeleterFunction destructor_function,
                                                   AbstractSequentialConstIteratorAccess* access);
QTJAMBI_EXPORT jobject convertIteratorToJavaObject(JNIEnv *env,
                                                   const std::type_info& containerTypeId,
                                                   const std::type_info& iteratorTypeId,
                                                   QtJambiNativeID owner,
                                                   void* iteratorPtr,
                                                   PtrDeleterFunction destructor_function,
                                                   AbstractAssociativeConstIteratorAccess* access);
QTJAMBI_EXPORT jobject convertIteratorToJavaObject(JNIEnv *env,
                                                   const std::type_info& containerTypeId,
                                                   const std::type_info& iteratorTypeId,
                                                   void* iteratorPtr,
                                                   PtrDeleterFunction destructor_function,
                                                   AbstractSequentialConstIteratorAccess* access);
QTJAMBI_EXPORT jobject convertIteratorToJavaObject(JNIEnv *env,
                                                   const std::type_info& containerTypeId,
                                                   const std::type_info& iteratorTypeId,
                                                   void* iteratorPtr,
                                                   PtrDeleterFunction destructor_function,
                                                   AbstractAssociativeConstIteratorAccess* access);


}

#endif // QTJAMBIAPI_ITERATOR_H
