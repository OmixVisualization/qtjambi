/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2020 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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


#ifndef QTJAMBI_CONTAINERS_H
#define QTJAMBI_CONTAINERS_H

#include "qtjambi_global.h"

#include "qtjambi_registry.h"
#include "qtjambi_jobjectwrapper.h"

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QMetaType>
#include <QtCore/QException>

#include <QtCore/QPair>
#include <QtCore/QVariant>
#include <QtCore/QEvent>
#include <QtCore/QModelIndex>
#include <QtCore/QVarLengthArray>
#include <QtCore/QStringList>
#include <functional>
#include <typeinfo>

QTJAMBI_EXPORT bool qtjambi_is_QStack(JNIEnv *env, jobject collection, jclass expectedElementType);
QTJAMBI_EXPORT bool qtjambi_is_QVector(JNIEnv *env, jobject collection, jclass expectedElementType);
QTJAMBI_EXPORT bool qtjambi_is_QLinkedList(JNIEnv *env, jobject collection, jclass expectedElementType);
QTJAMBI_EXPORT bool qtjambi_is_QList(JNIEnv *env, jobject collection, jclass expectedElementType);
QTJAMBI_EXPORT bool qtjambi_is_QSet(JNIEnv *env, jobject collection, jclass expectedElementType);
QTJAMBI_EXPORT bool qtjambi_is_QQueue(JNIEnv *env, jobject collection, jclass expectedElementType);
QTJAMBI_EXPORT bool qtjambi_is_QMultiMap(JNIEnv *env, jobject mapObject, jclass expectedKeyType, jclass expectedValueType);
QTJAMBI_EXPORT bool qtjambi_is_QMultiHash(JNIEnv *env, jobject mapObject, jclass expectedKeyType, jclass expectedValueType);
QTJAMBI_EXPORT bool qtjambi_is_QMap(JNIEnv *env, jobject mapObject, jclass expectedKeyType, jclass expectedValueType);
QTJAMBI_EXPORT bool qtjambi_is_QHash(JNIEnv *env, jobject mapObject, jclass expectedKeyType, jclass expectedValueType);

QTJAMBI_EXPORT bool qtjambi_is_QStack(JNIEnv *env, jobject collection, const std::type_info& expectedElementType);
QTJAMBI_EXPORT bool qtjambi_is_QVector(JNIEnv *env, jobject collection, const std::type_info& expectedElementType);
QTJAMBI_EXPORT bool qtjambi_is_QLinkedList(JNIEnv *env, jobject collection, const std::type_info& expectedElementType);
QTJAMBI_EXPORT bool qtjambi_is_QList(JNIEnv *env, jobject collection, const std::type_info& expectedElementType);
QTJAMBI_EXPORT bool qtjambi_is_QSet(JNIEnv *env, jobject collection, const std::type_info& expectedElementType);
QTJAMBI_EXPORT bool qtjambi_is_QQueue(JNIEnv *env, jobject collection, const std::type_info& expectedElementType);
QTJAMBI_EXPORT bool qtjambi_is_QMultiMap(JNIEnv *env, jobject mapObject, const std::type_info& expectedKeyType, const std::type_info& expectedValueType);
QTJAMBI_EXPORT bool qtjambi_is_QMultiHash(JNIEnv *env, jobject mapObject, const std::type_info& expectedKeyType, const std::type_info& expectedValueType);
QTJAMBI_EXPORT bool qtjambi_is_QMap(JNIEnv *env, jobject mapObject, const std::type_info& expectedKeyType, const std::type_info& expectedValueType);
QTJAMBI_EXPORT bool qtjambi_is_QHash(JNIEnv *env, jobject mapObject, const std::type_info& expectedKeyType, const std::type_info& expectedValueType);

typedef void(* PointerDeleter)(void *);
typedef void*(*PointerGetter)(const void *);
typedef jobject(*QListValueDefaultFunction)(JNIEnv *,const void*,jint,jobject);
typedef jobject(*QListValueFunction)(JNIEnv *,const void*,jint);
typedef jobject(*QListToSetFunction)(JNIEnv *,const void*);
typedef jobject(*QListTakeAtFunction)(JNIEnv *,void*,jint);
typedef void(*QListSwapFunction)(JNIEnv *,void*,jint,jint);
typedef jboolean(*QListStartsWithFunction)(JNIEnv *,const void*,jobject);
typedef jint(*QListSizeFunction)(JNIEnv *,const void*);
typedef void(*QListReserveFunction)(JNIEnv *,void*,jint);
typedef void(*QListReplaceFunction)(JNIEnv *,void*,jint,jobject);
typedef jboolean(*QListRemoveOneFunction)(JNIEnv *,void*,jobject);
typedef void(*QListRemoveAtFunction)(JNIEnv *,void*,jint);
typedef jint(*QListRemoveAllFunction)(JNIEnv *,void*,jobject);
typedef void(*QListPrependFunction)(JNIEnv *,void*,jobject);
typedef jboolean(*QListEqualFunction)(JNIEnv *,const void*,jobject);
typedef void(*QListMoveFunction)(JNIEnv *,void*,jint,jint);
typedef jobject(*QListMidFunction)(JNIEnv *,const void*,jint,jint);
typedef jint(*QListLastIndexOfFunction)(JNIEnv *,const void*,jobject,jint);
typedef void(*QListInsertFunction)(JNIEnv *,void*,jint,jobject);
typedef jint(*QListIndexOfFunction)(JNIEnv *,const void*,jobject,jint);
typedef jboolean(*QListEndsWithFunction)(JNIEnv *,const void*,jobject);
typedef jobject(*QListEndFunction)(JNIEnv *,QtJambiNativeID,const void*);
typedef jint(*QListCountObjectFunction)(JNIEnv *,const void*,jobject);
typedef jboolean(*QListContainsFunction)(JNIEnv *,const void*,jobject);
typedef void(*QListClearFunction)(JNIEnv *,void*);
typedef jobject(*QListBeginFunction)(JNIEnv *,QtJambiNativeID,const void*);
typedef jobject(*QListAtFunction)(JNIEnv *,const void*,jint);
typedef void(*QListAppendFunction)(JNIEnv *,void*,jobject);
typedef void(*QListAppendListFunction)(JNIEnv *,void*,jobject);

typedef jobject(*QLinkedListTakeLastFunction)(JNIEnv *,void*);
typedef jobject(*QLinkedListTakeFirstFunction)(JNIEnv *,void*);
typedef jboolean(*QLinkedListStartsWithFunction)(JNIEnv *,const void*,jobject);
typedef jint(*QLinkedListSizeFunction)(JNIEnv *,const void*);
typedef jboolean(*QLinkedListRemoveOneFunction)(JNIEnv *,void*,jobject);
typedef void(*QLinkedListRemoveFirstFunction)(JNIEnv *,void*);
typedef void(*QLinkedListRemoveLastFunction)(JNIEnv *,void*);
typedef jint(*QLinkedListRemoveAllFunction)(JNIEnv *,void*,jobject);
typedef void(*QLinkedListPrependFunction)(JNIEnv *,void*,jobject);
typedef jboolean(*QLinkedListEqualFunction)(JNIEnv *,const void*,jobject);
typedef jboolean(*QLinkedListEndsWithFunction)(JNIEnv *,const void*,jobject);
typedef jobject(*QLinkedListEndFunction)(JNIEnv *,QtJambiNativeID,const void*);
typedef jint(*QLinkedListCountObjectFunction)(JNIEnv *,const void*,jobject);
typedef jboolean(*QLinkedListContainsFunction)(JNIEnv *,const void*,jobject);
typedef void(*QLinkedListClearFunction)(JNIEnv *,void*);
typedef jobject(*QLinkedListBeginFunction)(JNIEnv *,QtJambiNativeID,const void*);
typedef jobject(*QLinkedListFirstFunction)(JNIEnv *,const void*);
typedef jobject(*QLinkedListLastFunction)(JNIEnv *,const void*);
typedef void(*QLinkedListAppendFunction)(JNIEnv *,void*,jobject);

typedef jobject(*QVectorValueDefaultFunction)(JNIEnv *,const void*,jint,jobject);
typedef jobject(*QVectorValueFunction)(JNIEnv *,const void*,jint);
typedef jobject(*QVectorToListFunction)(JNIEnv *,const void*);
typedef jobject(*QVectorTakeAtFunction)(JNIEnv *,void*,jint);
typedef jboolean(*QVectorStartsWithFunction)(JNIEnv *,const void*,jobject);
typedef jint(*QVectorSizeFunction)(JNIEnv *,const void*);
typedef void(*QVectorShrinkToFitFunction)(JNIEnv *,void*);
typedef void(*QVectorResizeFunction)(JNIEnv *,void*,jint);
typedef void(*QVectorReserveFunction)(JNIEnv *,void*,jint);
typedef void(*QVectorReplaceFunction)(JNIEnv *,void*,jint,jobject);
typedef jboolean(*QVectorRemoveOneFunction)(JNIEnv *,void*,jobject);
typedef void(*QVectorRemoveNFunction)(JNIEnv *,void*,jint,jint);
typedef void(*QVectorRemoveAtFunction)(JNIEnv *,void*,jint);
typedef jint(*QVectorRemoveAllFunction)(JNIEnv *,void*,jobject);
typedef void(*QVectorPrependFunction)(JNIEnv *,void*,jobject);
typedef jboolean(*QVectorEqualFunction)(JNIEnv *,const void*,jobject);
typedef void(*QVectorMoveFunction)(JNIEnv *,void*,jint,jint);
typedef jobject(*QVectorMidFunction)(JNIEnv *,const void*,jint,jint);
typedef jint(*QVectorLastIndexOfFunction)(JNIEnv *,const void*,jobject,jint);
typedef void(*QVectorInsertNFunction)(JNIEnv *,void*,jint,jint,jobject);
typedef void(*QVectorInsertFunction)(JNIEnv *,void*,jint,jobject);
typedef void(*QVectorFillFunction)(JNIEnv *,void*,jobject,jint);
typedef jint(*QVectorIndexOfFunction)(JNIEnv *,const void*,jobject,jint);
typedef jboolean(*QVectorEndsWithFunction)(JNIEnv *,const void*,jobject);
typedef jobject(*QVectorEndFunction)(JNIEnv *,QtJambiNativeID,const void*);
typedef jint(*QVectorCountObjectFunction)(JNIEnv *,const void*,jobject);
typedef jboolean(*QVectorContainsFunction)(JNIEnv *,const void*,jobject);
typedef void(*QVectorClearFunction)(JNIEnv *,void*);
typedef jint(*QVectorCapacityFunction)(JNIEnv *,const void*);
typedef jobject(*QVectorBeginFunction)(JNIEnv *,QtJambiNativeID,const void*);
typedef jobject(*QVectorAtFunction)(JNIEnv *,const void*,jint);
typedef void(*QVectorAppendFunction)(JNIEnv *,void*,jobject);
typedef void(*QVectorAppendVectorFunction)(JNIEnv *,void*,jobject);

typedef jobject(*QSetValuesFunction)(JNIEnv *,const void*);
typedef void(*QSetUniteFunction)(JNIEnv *,void*,jobject);
typedef void(*QSetSubtractFunction)(JNIEnv *,void*,jobject);
typedef jint(*QSetSizeFunction)(JNIEnv *,const void*);
typedef void(*QSetReserveFunction)(JNIEnv *,void*,jint);
typedef jboolean(*QSetRemoveFunction)(JNIEnv *,void*,jobject);
typedef jboolean(*QSetEqualFunction)(JNIEnv *,const void*,jobject);
typedef jboolean(*QSetIntersectsFunction)(JNIEnv *,const void*,jobject);
typedef void(*QSetIntersectFunction)(JNIEnv *,void*,jobject);
typedef void(*QSetInsertFunction)(JNIEnv *,void*,jobject);
typedef jobject(*QSetEndFunction)(JNIEnv *,QtJambiNativeID,const void*);
typedef jboolean(*QSetContainsFunction)(JNIEnv *,const void*,jobject);
typedef void(*QSetClearFunction)(JNIEnv *,void*);
typedef jint(*QSetCapacityFunction)(JNIEnv *,const void*);
typedef jobject(*QSetBeginFunction)(JNIEnv *,QtJambiNativeID,const void*);

typedef jobject(*QMapBeginFunction)(JNIEnv *,QtJambiNativeID,const void*);
typedef void(*QMapClearFunction)(JNIEnv *,void*);
typedef jboolean(*QMapContainsFunction)(JNIEnv *,const void*,jobject);
typedef jint(*QMapCountObjectFunction)(JNIEnv *,const void*,jobject);
typedef jobject(*QMapEndFunction)(JNIEnv *,QtJambiNativeID,const void*);
typedef jobject(*QMapFindFunction)(JNIEnv *,QtJambiNativeID,const void*,jobject);
typedef jobject(*QMapFirstFunction)(JNIEnv *,const void*);
typedef jobject(*QMapFirstKeyFunction)(JNIEnv *,const void*);
typedef void(*QMapInsertFunction)(JNIEnv *,void*,jobject,jobject);
typedef jobject(*QMapKeyFunction)(JNIEnv *,const void*,jobject,jobject);
typedef jobject(*QMapKeysFunction)(JNIEnv *,const void*);
typedef jobject(*QMapKeysForValueFunction)(JNIEnv *,const void*,jobject);
typedef jobject(*QMapLastFunction)(JNIEnv *,const void*);
typedef jobject(*QMapLastKeyFunction)(JNIEnv *,const void*);
typedef jobject(*QMapLowerBoundFunction)(JNIEnv *,QtJambiNativeID,const void*,jobject);
typedef jboolean(*QMapEqualFunction)(JNIEnv *,const void*,jobject);
typedef jint(*QMapRemoveAllFunction)(JNIEnv *,void*,jobject);
typedef jint(*QMapSizeFunction)(JNIEnv *,const void*);
typedef jobject(*QMapTakeFunction)(JNIEnv *,void*,jobject);
typedef jobject(*QMapUniqueKeysFunction)(JNIEnv *,const void*);
typedef void(*QMapUniteFunction)(JNIEnv *,void*,jobject);
typedef jobject(*QMapUpperBoundFunction)(JNIEnv *,QtJambiNativeID,const void*,jobject);
typedef jobject(*QMapValueFunction)(JNIEnv *,const void*,jobject,jobject);
typedef jobject(*QMapValuesFunction)(JNIEnv *,const void*);
typedef jobject(*QMapValuesKeyFunction)(JNIEnv *,const void*,jobject);
typedef jboolean(*QMultiMapContainsPairFunction)(JNIEnv *,const void*,jobject,jobject);
typedef jint(*QMultiMapCountPairFunction)(JNIEnv *,const void*,jobject,jobject);
typedef jobject(*QMultiMapFindPairFunction)(JNIEnv *,QtJambiNativeID,const void*,jobject,jobject);
typedef jint(*QMultiMapRemovePairFunction)(JNIEnv *,void*,jobject,jobject);
typedef void(*QMultiMapReplaceFunction)(JNIEnv *,void*,jobject,jobject);

typedef jobject(*QHashBeginFunction)(JNIEnv *,QtJambiNativeID,const void*);
typedef jint(*QHashCapacityFunction)(JNIEnv *,const void*);
typedef void(*QHashClearFunction)(JNIEnv *,void*);
typedef jboolean(*QHashContainsFunction)(JNIEnv *,const void*,jobject);
typedef jint(*QHashCountObjectFunction)(JNIEnv *,const void*,jobject);
typedef jobject(*QHashEndFunction)(JNIEnv *,QtJambiNativeID,const void*);
typedef jobject(*QHashFindFunction)(JNIEnv *,QtJambiNativeID,const void*,jobject);
typedef void(*QHashInsertFunction)(JNIEnv *,void*,jobject,jobject);
typedef jobject(*QHashKeyFunction)(JNIEnv *,const void*,jobject,jobject);
typedef jobject(*QHashKeysFunction)(JNIEnv *,const void*);
typedef jobject(*QHashKeysForValueFunction)(JNIEnv *,const void*,jobject);
typedef jboolean(*QHashEqualFunction)(JNIEnv *,const void*,jobject);
typedef jint(*QHashRemoveAllFunction)(JNIEnv *,void*,jobject);
typedef void(*QHashReserveFunction)(JNIEnv *,void*,jint);
typedef jint(*QHashSizeFunction)(JNIEnv *,const void*);
typedef jobject(*QHashTakeFunction)(JNIEnv *,void*,jobject);
typedef jobject(*QHashUniqueKeysFunction)(JNIEnv *,const void*);
typedef void(*QHashUniteFunction)(JNIEnv *,void*,jobject);
typedef jobject(*QHashValueFunction)(JNIEnv *,const void*,jobject,jobject);
typedef jobject(*QHashValuesFunction)(JNIEnv *,const void*);
typedef jobject(*QHashValuesKeyFunction)(JNIEnv *,const void*,jobject);
typedef jboolean(*QMultiHashContainsPairFunction)(JNIEnv *,const void*,jobject,jobject);
typedef jint(*QMultiHashCountPairFunction)(JNIEnv *,const void*,jobject,jobject);
typedef jobject(*QMultiHashFindPairFunction)(JNIEnv *,QtJambiNativeID,const void*,jobject,jobject);
typedef jint(*QMultiHashRemovePairFunction)(JNIEnv *,void*,jobject,jobject);
typedef void(*QMultiHashReplaceFunction)(JNIEnv *,void*,jobject,jobject);

typedef jboolean(*QIteratorEqualsFunction)(void*,void*);
typedef jboolean(*QIteratorLessThanFunction)(void*,void*);
typedef void(*QIteratorDecrementFunction)(void*);
typedef void(*QIteratorIncrementFunction)(void*);
typedef jobject(*QMapIteratorKeyFunction)(JNIEnv *,void*);
typedef jobject(*QIteratorValueFunction)(JNIEnv *,void*);

QTJAMBI_EXPORT jobject qtjambi_to_constQList(JNIEnv *__jni_env,
                                     QtJambiNativeID owner,
                                     const void* listPtr,
                                     const char* className,
                                     QListAtFunction atFunction,
                                     QListBeginFunction beginFunction,
                                     QListContainsFunction containsFunction,
                                     QListCountObjectFunction countObjectFunction,
                                     QListEndFunction endFunction,
                                     QListEndsWithFunction endsWithFunction,
                                     QListIndexOfFunction indexOfFunction,
                                     QListLastIndexOfFunction lastIndexOfFunction,
                                     QListMidFunction midFunction,
                                     QListEqualFunction equalFunction,
                                     QListSizeFunction sizeFunction,
                                     QListStartsWithFunction startsWithFunction,
                                     QListToSetFunction toSetFunction,
                                     QListValueFunction valueFunction,
                                     QListValueDefaultFunction valueDefaultFunction
                                );

QTJAMBI_EXPORT jobject qtjambi_to_QList(JNIEnv *__jni_env,
                                     QtJambiNativeID owner,
                                     void* listPtr,
                                     const char* className,
                                     QListAppendFunction appendFunction,
                                     QListAppendListFunction appendListFunction,
                                     QListAtFunction atFunction,
                                     QListBeginFunction beginFunction,
                                     QListClearFunction clearFunction,
                                     QListContainsFunction containsFunction,
                                     QListCountObjectFunction countObjectFunction,
                                     QListEndFunction endFunction,
                                     QListEndsWithFunction endsWithFunction,
                                     QListIndexOfFunction indexOfFunction,
                                     QListInsertFunction insertFunction,
                                     QListLastIndexOfFunction lastIndexOfFunction,
                                     QListMidFunction midFunction,
                                     QListMoveFunction moveFunction,
                                     QListEqualFunction equalFunction,
                                     QListPrependFunction prependFunction,
                                     QListRemoveAllFunction removeAllFunction,
                                     QListRemoveAtFunction removeAtFunction,
                                     QListRemoveOneFunction removeOneFunction,
                                     QListReplaceFunction replaceFunction,
                                     QListReserveFunction reserveFunction,
                                     QListSizeFunction sizeFunction,
                                     QListStartsWithFunction startsWithFunction,
                                     QListSwapFunction swapFunction,
                                     QListTakeAtFunction takeAtFunction,
                                     QListToSetFunction toSetFunction,
                                     QListValueFunction valueFunction,
                                     QListValueDefaultFunction valueDefaultFunction
                                );

QTJAMBI_EXPORT jobject qtjambi_to_constQQueue(JNIEnv *__jni_env,
                                     QtJambiNativeID owner,
                                     const void* listPtr,
                                     const char* className,
                                     QListAtFunction atFunction,
                                     QListBeginFunction beginFunction,
                                     QListContainsFunction containsFunction,
                                     QListCountObjectFunction countObjectFunction,
                                     QListEndFunction endFunction,
                                     QListEndsWithFunction endsWithFunction,
                                     QListIndexOfFunction indexOfFunction,
                                     QListLastIndexOfFunction lastIndexOfFunction,
                                     QListMidFunction midFunction,
                                     QListEqualFunction equalFunction,
                                     QListSizeFunction sizeFunction,
                                     QListStartsWithFunction startsWithFunction,
                                     QListToSetFunction toSetFunction,
                                     QListValueFunction valueFunction,
                                     QListValueDefaultFunction valueDefaultFunction
                                );

QTJAMBI_EXPORT jobject qtjambi_to_QQueue(JNIEnv *__jni_env,
                                     QtJambiNativeID owner,
                                     void* listPtr,
                                     const char* className,
                                     QListAppendFunction appendFunction,
                                     QListAppendListFunction appendListFunction,
                                     QListAtFunction atFunction,
                                     QListBeginFunction beginFunction,
                                     QListClearFunction clearFunction,
                                     QListContainsFunction containsFunction,
                                     QListCountObjectFunction countObjectFunction,
                                     QListEndFunction endFunction,
                                     QListEndsWithFunction endsWithFunction,
                                     QListIndexOfFunction indexOfFunction,
                                     QListInsertFunction insertFunction,
                                     QListLastIndexOfFunction lastIndexOfFunction,
                                     QListMidFunction midFunction,
                                     QListMoveFunction moveFunction,
                                     QListEqualFunction equalFunction,
                                     QListPrependFunction prependFunction,
                                     QListRemoveAllFunction removeAllFunction,
                                     QListRemoveAtFunction removeAtFunction,
                                     QListRemoveOneFunction removeOneFunction,
                                     QListReplaceFunction replaceFunction,
                                     QListReserveFunction reserveFunction,
                                     QListSizeFunction sizeFunction,
                                     QListStartsWithFunction startsWithFunction,
                                     QListSwapFunction swapFunction,
                                     QListTakeAtFunction takeAtFunction,
                                     QListToSetFunction toSetFunction,
                                     QListValueFunction valueFunction,
                                     QListValueDefaultFunction valueDefaultFunction
                                );

QTJAMBI_EXPORT jobject qtjambi_to_constQSet(JNIEnv *__jni_env,
                                     QtJambiNativeID owner,
                                     const void* listPtr,
                                     const char* className,
                                     QSetBeginFunction beginFunction,
                                     QSetCapacityFunction capacityFunction,
                                     QSetContainsFunction containsFunction,
                                     QSetEndFunction endFunction,
                                     QSetIntersectsFunction intersectsFunction,
                                     QSetEqualFunction equalFunction,
                                     QSetSizeFunction sizeFunction,
                                     QSetValuesFunction valuesFunction
                                );

QTJAMBI_EXPORT jobject qtjambi_to_QSet(JNIEnv *__jni_env,
                                     QtJambiNativeID owner,
                                     void* listPtr,
                                     const char* className,
                                     QSetBeginFunction beginFunction,
                                     QSetCapacityFunction capacityFunction,
                                     QSetClearFunction clearFunction,
                                     QSetContainsFunction containsFunction,
                                     QSetEndFunction endFunction,
                                     QSetInsertFunction insertFunction,
                                     QSetIntersectFunction intersectFunction,
                                     QSetIntersectsFunction intersectsFunction,
                                     QSetEqualFunction equalFunction,
                                     QSetRemoveFunction removeFunction,
                                     QSetReserveFunction reserveFunction,
                                     QSetSizeFunction sizeFunction,
                                     QSetSubtractFunction subtractFunction,
                                     QSetUniteFunction uniteFunction,
                                     QSetValuesFunction valuesFunction
                                );

QTJAMBI_EXPORT jobject qtjambi_to_constQLinkedList(JNIEnv *__jni_env,
                                     QtJambiNativeID owner,
                                     const void* listPtr,
                                     const char* className,
                                     QLinkedListBeginFunction beginFunction,
                                     QLinkedListContainsFunction containsFunction,
                                     QLinkedListCountObjectFunction countObjectFunction,
                                     QLinkedListEndFunction endFunction,
                                     QLinkedListEndsWithFunction endsWithFunction,
                                     QLinkedListFirstFunction firstFunction,
                                     QLinkedListLastFunction lastFunction,
                                     QLinkedListEqualFunction equalFunction,
                                     QLinkedListSizeFunction sizeFunction,
                                     QLinkedListStartsWithFunction startsWithFunction
                                );

QTJAMBI_EXPORT jobject qtjambi_to_QLinkedList(JNIEnv *__jni_env,
                                     QtJambiNativeID owner,
                                     void* listPtr,
                                     const char* className,
                                     QLinkedListAppendFunction appendFunction,
                                     QLinkedListBeginFunction beginFunction,
                                     QLinkedListClearFunction clearFunction,
                                     QLinkedListContainsFunction containsFunction,
                                     QLinkedListCountObjectFunction countObjectFunction,
                                     QLinkedListEndFunction endFunction,
                                     QLinkedListEndsWithFunction endsWithFunction,
                                     QLinkedListFirstFunction firstFunction,
                                     QLinkedListLastFunction lastFunction,
                                     QLinkedListEqualFunction equalFunction,
                                     QLinkedListPrependFunction prependFunction,
                                     QLinkedListRemoveAllFunction removeAllFunction,
                                     QLinkedListRemoveFirstFunction removeFirstFunction,
                                     QLinkedListRemoveLastFunction removeLastFunction,
                                     QLinkedListRemoveOneFunction removeOneFunction,
                                     QLinkedListSizeFunction sizeFunction,
                                     QLinkedListStartsWithFunction startsWithFunction,
                                     QLinkedListTakeFirstFunction takeFirstFunction,
                                     QLinkedListTakeLastFunction takeLastFunction
                                );

QTJAMBI_EXPORT jobject qtjambi_to_constQVector(JNIEnv *__jni_env,
                                     QtJambiNativeID owner,
                                     const void* listPtr,
                                     const char* className,
                                     QVectorAtFunction atFunction,
                                     QVectorBeginFunction beginFunction,
                                     QVectorCapacityFunction capacityFunction,
                                     QVectorContainsFunction containsFunction,
                                     QVectorCountObjectFunction countObjectFunction,
                                     QVectorEndFunction endFunction,
                                     QVectorEndsWithFunction endsWithFunction,
                                     QVectorIndexOfFunction indexOfFunction,
                                     QVectorLastIndexOfFunction lastIndexOfFunction,
                                     QVectorMidFunction midFunction,
                                     QVectorEqualFunction equalFunction,
                                     QVectorSizeFunction sizeFunction,
                                     QVectorStartsWithFunction startsWithFunction,
                                     QVectorToListFunction toListFunction,
                                     QVectorValueFunction valueFunction,
                                     QVectorValueDefaultFunction valueDefaultFunction
                                );



QTJAMBI_EXPORT jobject qtjambi_to_QVector(JNIEnv *__jni_env,
                                     QtJambiNativeID owner,
                                     void* listPtr,
                                     const char* className,
                                     QVectorAppendFunction appendFunction,
                                     QVectorAppendVectorFunction appendListFunction,
                                     QVectorAtFunction atFunction,
                                     QVectorBeginFunction beginFunction,
                                     QVectorCapacityFunction capacityFunction,
                                     QVectorClearFunction clearFunction,
                                     QVectorContainsFunction containsFunction,
                                     QVectorCountObjectFunction countObjectFunction,
                                     QVectorEndFunction endFunction,
                                     QVectorEndsWithFunction endsWithFunction,
                                     QVectorFillFunction fillFunction,
                                     QVectorIndexOfFunction indexOfFunction,
                                     QVectorInsertFunction insertFunction,
                                     QVectorInsertNFunction insertNFunction,
                                     QVectorLastIndexOfFunction lastIndexOfFunction,
                                     QVectorMidFunction midFunction,
                                     QVectorMoveFunction moveFunction,
                                     QVectorEqualFunction equalFunction,
                                     QVectorPrependFunction prependFunction,
                                     QVectorRemoveAllFunction removeAllFunction,
                                     QVectorRemoveAtFunction removeAtFunction,
                                     QVectorRemoveNFunction removeNFunction,
                                     QVectorRemoveOneFunction removeOneFunction,
                                     QVectorReplaceFunction replaceFunction,
                                     QVectorReserveFunction reserveFunction,
                                     QVectorResizeFunction resizeFunction,
                                     QVectorShrinkToFitFunction shrinkToFitFunction,
                                     QVectorSizeFunction sizeFunction,
                                     QVectorStartsWithFunction startsWithFunction,
                                     QVectorTakeAtFunction takeAtFunction,
                                     QVectorToListFunction toListFunction,
                                     QVectorValueFunction valueFunction,
                                     QVectorValueDefaultFunction valueDefaultFunction
                                );



QTJAMBI_EXPORT jobject qtjambi_to_constQStack(JNIEnv *__jni_env,
                                     QtJambiNativeID owner,
                                     const void* listPtr,
                                     const char* className,
                                     QVectorAtFunction atFunction,
                                     QVectorBeginFunction beginFunction,
                                     QVectorCapacityFunction capacityFunction,
                                     QVectorContainsFunction containsFunction,
                                     QVectorCountObjectFunction countObjectFunction,
                                     QVectorEndFunction endFunction,
                                     QVectorEndsWithFunction endsWithFunction,
                                     QVectorIndexOfFunction indexOfFunction,
                                     QVectorLastIndexOfFunction lastIndexOfFunction,
                                     QVectorMidFunction midFunction,
                                     QVectorEqualFunction equalFunction,
                                     QVectorSizeFunction sizeFunction,
                                     QVectorStartsWithFunction startsWithFunction,
                                     QVectorToListFunction toListFunction,
                                     QVectorValueFunction valueFunction,
                                     QVectorValueDefaultFunction valueDefaultFunction
                                );



QTJAMBI_EXPORT jobject qtjambi_to_QStack(JNIEnv *__jni_env,
                                     QtJambiNativeID owner,
                                     void* listPtr,
                                     const char* className,
                                     QVectorAppendFunction appendFunction,
                                     QVectorAppendVectorFunction appendListFunction,
                                     QVectorAtFunction atFunction,
                                     QVectorBeginFunction beginFunction,
                                     QVectorCapacityFunction capacityFunction,
                                     QVectorClearFunction clearFunction,
                                     QVectorContainsFunction containsFunction,
                                     QVectorCountObjectFunction countObjectFunction,
                                     QVectorEndFunction endFunction,
                                     QVectorEndsWithFunction endsWithFunction,
                                     QVectorFillFunction fillFunction,
                                     QVectorIndexOfFunction indexOfFunction,
                                     QVectorInsertFunction insertFunction,
                                     QVectorInsertNFunction insertNFunction,
                                     QVectorLastIndexOfFunction lastIndexOfFunction,
                                     QVectorMidFunction midFunction,
                                     QVectorMoveFunction moveFunction,
                                     QVectorEqualFunction equalFunction,
                                     QVectorPrependFunction prependFunction,
                                     QVectorRemoveAllFunction removeAllFunction,
                                     QVectorRemoveAtFunction removeAtFunction,
                                     QVectorRemoveNFunction removeNFunction,
                                     QVectorRemoveOneFunction removeOneFunction,
                                     QVectorReplaceFunction replaceFunction,
                                     QVectorReserveFunction reserveFunction,
                                     QVectorResizeFunction resizeFunction,
                                     QVectorShrinkToFitFunction shrinkToFitFunction,
                                     QVectorSizeFunction sizeFunction,
                                     QVectorStartsWithFunction startsWithFunction,
                                     QVectorTakeAtFunction takeAtFunction,
                                     QVectorToListFunction toListFunction,
                                     QVectorValueFunction valueFunction,
                                     QVectorValueDefaultFunction valueDefaultFunction
                                );

QTJAMBI_EXPORT jobject qtjambi_to_constQHash(JNIEnv *__jni_env,
                                     QtJambiNativeID owner,
                                     const void* listPtr,
                                     const char* keyClassName,
                                     const char* valueClassName,
                                     QHashBeginFunction beginFunction,
                                     QHashCapacityFunction capacityFunction,
                                     QHashContainsFunction containsFunction,
                                     QHashCountObjectFunction countObjectFunction,
                                     QHashEndFunction endFunction,
                                     QHashFindFunction findFunction,
                                     QHashKeyFunction keyFunction,
                                     QHashKeysFunction keysFunction,
                                     QHashKeysForValueFunction keysForValueFunction,
                                     QHashEqualFunction equalFunction,
                                     QHashSizeFunction sizeFunction,
                                     QHashUniqueKeysFunction uniqueKeysFunction,
                                     QHashValueFunction valueFunction,
                                     QHashValuesFunction valuesFunction,
                                     QHashValuesKeyFunction valuesKeyFunction
                                );

QTJAMBI_EXPORT jobject qtjambi_to_QHash(JNIEnv *__jni_env,
                                     QtJambiNativeID owner,
                                     void* listPtr,
                                     const char* keyClassName,
                                     const char* valueClassName,
                                     QHashBeginFunction beginFunction,
                                     QHashCapacityFunction capacityFunction,
                                     QHashClearFunction clearFunction,
                                     QHashContainsFunction containsFunction,
                                     QHashCountObjectFunction countObjectFunction,
                                     QHashEndFunction endFunction,
                                     QHashFindFunction findFunction,
                                     QHashInsertFunction insertFunction,
                                     QHashKeyFunction keyFunction,
                                     QHashKeysFunction keysFunction,
                                     QHashKeysForValueFunction keysForValueFunction,
                                     QHashEqualFunction equalFunction,
                                     QHashRemoveAllFunction removeAllFunction,
                                     QHashReserveFunction reserveFunction,
                                     QHashSizeFunction sizeFunction,
                                     QHashTakeFunction takeFunction,
                                     QHashUniqueKeysFunction uniqueKeysFunction,
                                     QHashUniteFunction uniteFunction,
                                     QHashValueFunction valueFunction,
                                     QHashValuesFunction valuesFunction,
                                     QHashValuesKeyFunction valuesKeyFunction
                                );

QTJAMBI_EXPORT jobject qtjambi_to_constQMultiHash(JNIEnv *__jni_env,
                                     QtJambiNativeID owner,
                                     const void* listPtr,
                                     const char* keyClassName,
                                     const char* valueClassName,
                                     QHashBeginFunction beginFunction,
                                     QHashCapacityFunction capacityFunction,
                                     QHashContainsFunction containsFunction,
                                     QHashCountObjectFunction countObjectFunction,
                                     QHashEndFunction endFunction,
                                     QHashFindFunction findFunction,
                                     QHashKeyFunction keyFunction,
                                     QHashKeysFunction keysFunction,
                                     QHashKeysForValueFunction keysForValueFunction,
                                     QHashEqualFunction equalFunction,
                                     QHashSizeFunction sizeFunction,
                                     QHashUniqueKeysFunction uniqueKeysFunction,
                                     QHashValueFunction valueFunction,
                                     QHashValuesFunction valuesFunction,
                                     QHashValuesKeyFunction valuesKeyFunction,
                                     QMultiHashContainsPairFunction containsPairFunction,
                                     QMultiHashCountPairFunction countPairFunction,
                                     QMultiHashFindPairFunction findPairFunction
                                );

QTJAMBI_EXPORT jobject qtjambi_to_QMultiHash(JNIEnv *__jni_env,
                                     QtJambiNativeID owner,
                                     void* listPtr,
                                     const char* keyClassName,
                                     const char* valueClassName,
                                     QHashBeginFunction beginFunction,
                                     QHashCapacityFunction capacityFunction,
                                     QHashClearFunction clearFunction,
                                     QHashContainsFunction containsFunction,
                                     QHashCountObjectFunction countObjectFunction,
                                     QHashEndFunction endFunction,
                                     QHashFindFunction findFunction,
                                     QHashInsertFunction insertFunction,
                                     QHashKeyFunction keyFunction,
                                     QHashKeysFunction keysFunction,
                                     QHashKeysForValueFunction keysForValueFunction,
                                     QHashEqualFunction equalFunction,
                                     QHashRemoveAllFunction removeAllFunction,
                                     QHashReserveFunction reserveFunction,
                                     QHashSizeFunction sizeFunction,
                                     QHashTakeFunction takeFunction,
                                     QHashUniqueKeysFunction uniqueKeysFunction,
                                     QHashUniteFunction uniteFunction,
                                     QHashValueFunction valueFunction,
                                     QHashValuesFunction valuesFunction,
                                     QHashValuesKeyFunction valuesKeyFunction,
                                     QMultiHashContainsPairFunction containsPairFunction,
                                     QMultiHashCountPairFunction countPairFunction,
                                     QMultiHashFindPairFunction findPairFunction,
                                     QMultiHashRemovePairFunction removeAllPairFunction,
                                     QMultiHashReplaceFunction replaceOneFunction
                                );

QTJAMBI_EXPORT jobject qtjambi_to_constQMap(JNIEnv *__jni_env,
                                     QtJambiNativeID owner,
                                     const void* listPtr,
                                     const char* keyClassName,
                                     const char* valueClassName,
                                     QMapBeginFunction beginFunction,
                                     QMapContainsFunction containsFunction,
                                     QMapCountObjectFunction countObjectFunction,
                                     QMapEndFunction endFunction,
                                     QMapFindFunction findFunction,
                                     QMapFirstFunction firstFunction,
                                     QMapFirstKeyFunction firstKeyFunction,
                                     QMapKeyFunction keyFunction,
                                     QMapKeysFunction keysFunction,
                                     QMapKeysForValueFunction keysForValueFunction,
                                     QMapLastFunction lastFunction,
                                     QMapLastKeyFunction lastKeyFunction,
                                     QMapLowerBoundFunction lowerBoundFunction,
                                     QMapEqualFunction equalFunction,
                                     QMapSizeFunction sizeFunction,
                                     QMapUniqueKeysFunction uniqueKeysFunction,
                                     QMapUpperBoundFunction upperBoundFunction,
                                     QMapValueFunction valueFunction,
                                     QMapValuesFunction valuesFunction,
                                     QMapValuesKeyFunction valuesKeyFunction
                                );

QTJAMBI_EXPORT jobject qtjambi_to_QMap(JNIEnv *__jni_env,
                                     QtJambiNativeID owner,
                                     void* listPtr,
                                     const char* keyClassName,
                                     const char* valueClassName,
                                     QMapBeginFunction beginFunction,
                                     QMapClearFunction clearFunction,
                                     QMapContainsFunction containsFunction,
                                     QMapCountObjectFunction countObjectFunction,
                                     QMapEndFunction endFunction,
                                     QMapFindFunction findFunction,
                                     QMapFirstFunction firstFunction,
                                     QMapFirstKeyFunction firstKeyFunction,
                                     QMapInsertFunction insertFunction,
                                     QMapKeyFunction keyFunction,
                                     QMapKeysFunction keysFunction,
                                     QMapKeysForValueFunction keysForValueFunction,
                                     QMapLastFunction lastFunction,
                                     QMapLastKeyFunction lastKeyFunction,
                                     QMapLowerBoundFunction lowerBoundFunction,
                                     QMapEqualFunction equalFunction,
                                     QMapRemoveAllFunction removeAllFunction,
                                     QMapSizeFunction sizeFunction,
                                     QMapTakeFunction takeFunction,
                                     QMapUniqueKeysFunction uniqueKeysFunction,
                                     QMapUniteFunction uniteFunction,
                                     QMapUpperBoundFunction upperBoundFunction,
                                     QMapValueFunction valueFunction,
                                     QMapValuesFunction valuesFunction,
                                     QMapValuesKeyFunction valuesKeyFunction
                                );

QTJAMBI_EXPORT jobject qtjambi_to_constQMultiMap(JNIEnv *__jni_env,
                                     QtJambiNativeID owner,
                                     const void* listPtr,
                                     const char* keyClassName,
                                     const char* valueClassName,
                                     QMapBeginFunction beginFunction,
                                     QMapContainsFunction containsFunction,
                                     QMapCountObjectFunction countObjectFunction,
                                     QMapEndFunction endFunction,
                                     QMapFindFunction findFunction,
                                     QMapFirstFunction firstFunction,
                                     QMapFirstKeyFunction firstKeyFunction,
                                     QMapKeyFunction keyFunction,
                                     QMapKeysFunction keysFunction,
                                     QMapKeysForValueFunction keysForValueFunction,
                                     QMapLastFunction lastFunction,
                                     QMapLastKeyFunction lastKeyFunction,
                                     QMapLowerBoundFunction lowerBoundFunction,
                                     QMapEqualFunction equalFunction,
                                     QMapSizeFunction sizeFunction,
                                     QMapUniqueKeysFunction uniqueKeysFunction,
                                     QMapUpperBoundFunction upperBoundFunction,
                                     QMapValueFunction valueFunction,
                                     QMapValuesFunction valuesFunction,
                                     QMapValuesKeyFunction valuesKeyFunction,
                                     QMultiMapContainsPairFunction containsPairFunction,
                                     QMultiMapCountPairFunction countPairFunction,
                                     QMultiMapFindPairFunction findPairFunction
                                );

QTJAMBI_EXPORT jobject qtjambi_to_QMultiMap(JNIEnv *__jni_env,
                                     QtJambiNativeID owner,
                                     void* listPtr,
                                     const char* keyClassName,
                                     const char* valueClassName,
                                     QMapBeginFunction beginFunction,
                                     QMapClearFunction clearFunction,
                                     QMapContainsFunction containsFunction,
                                     QMapCountObjectFunction countObjectFunction,
                                     QMapEndFunction endFunction,
                                     QMapFindFunction findFunction,
                                     QMapFirstFunction firstFunction,
                                     QMapFirstKeyFunction firstKeyFunction,
                                     QMapInsertFunction insertFunction,
                                     QMapKeyFunction keyFunction,
                                     QMapKeysFunction keysFunction,
                                     QMapKeysForValueFunction keysForValueFunction,
                                     QMapLastFunction lastFunction,
                                     QMapLastKeyFunction lastKeyFunction,
                                     QMapLowerBoundFunction lowerBoundFunction,
                                     QMapEqualFunction equalFunction,
                                     QMapRemoveAllFunction removeAllFunction,
                                     QMapSizeFunction sizeFunction,
                                     QMapTakeFunction takeFunction,
                                     QMapUniqueKeysFunction uniqueKeysFunction,
                                     QMapUniteFunction uniteFunction,
                                     QMapUpperBoundFunction upperBoundFunction,
                                     QMapValueFunction valueFunction,
                                     QMapValuesFunction valuesFunction,
                                     QMapValuesKeyFunction valuesKeyFunction,
                                     QMultiMapContainsPairFunction containsPairFunction,
                                     QMultiMapCountPairFunction countPairFunction,
                                     QMultiMapFindPairFunction findPairFunction,
                                     QMultiMapRemovePairFunction removeAllPairFunction,
                                     QMultiMapReplaceFunction replaceOneFunction
                                );

QTJAMBI_EXPORT jobject qtjambi_from_constQList_shared_pointer(JNIEnv *__jni_env,
                                     void* listPtr,
                                     PointerDeleter sharedPointerDeleter,
                                     PointerGetter sharedPointerGetter,
                                     const char* className,
                                     QListAtFunction atFunction,
                                     QListBeginFunction beginFunction,
                                     QListContainsFunction containsFunction,
                                     QListCountObjectFunction countObjectFunction,
                                     QListEndFunction endFunction,
                                     QListEndsWithFunction endsWithFunction,
                                     QListIndexOfFunction indexOfFunction,
                                     QListLastIndexOfFunction lastIndexOfFunction,
                                     QListMidFunction midFunction,
                                     QListEqualFunction equalFunction,
                                     QListSizeFunction sizeFunction,
                                     QListStartsWithFunction startsWithFunction,
                                     QListToSetFunction toSetFunction,
                                     QListValueFunction valueFunction,
                                     QListValueDefaultFunction valueDefaultFunction
                                );

QTJAMBI_EXPORT jobject qtjambi_from_QList_shared_pointer(JNIEnv *__jni_env,
                                     void* listPtr,
                                     PointerDeleter sharedPointerDeleter,
                                     PointerGetter sharedPointerGetter,
                                     const char* className,
                                     QListAppendFunction appendFunction,
                                     QListAppendListFunction appendListFunction,
                                     QListAtFunction atFunction,
                                     QListBeginFunction beginFunction,
                                     QListClearFunction clearFunction,
                                     QListContainsFunction containsFunction,
                                     QListCountObjectFunction countObjectFunction,
                                     QListEndFunction endFunction,
                                     QListEndsWithFunction endsWithFunction,
                                     QListIndexOfFunction indexOfFunction,
                                     QListInsertFunction insertFunction,
                                     QListLastIndexOfFunction lastIndexOfFunction,
                                     QListMidFunction midFunction,
                                     QListMoveFunction moveFunction,
                                     QListEqualFunction equalFunction,
                                     QListPrependFunction prependFunction,
                                     QListRemoveAllFunction removeAllFunction,
                                     QListRemoveAtFunction removeAtFunction,
                                     QListRemoveOneFunction removeOneFunction,
                                     QListReplaceFunction replaceFunction,
                                     QListReserveFunction reserveFunction,
                                     QListSizeFunction sizeFunction,
                                     QListStartsWithFunction startsWithFunction,
                                     QListSwapFunction swapFunction,
                                     QListTakeAtFunction takeAtFunction,
                                     QListToSetFunction toSetFunction,
                                     QListValueFunction valueFunction,
                                     QListValueDefaultFunction valueDefaultFunction
                                );

QTJAMBI_EXPORT jobject qtjambi_from_constQQueue_shared_pointer(JNIEnv *__jni_env,
                                     void* listPtr,
                                     PointerDeleter sharedPointerDeleter,
                                     PointerGetter sharedPointerGetter,
                                     const char* className,
                                     QListAtFunction atFunction,
                                     QListBeginFunction beginFunction,
                                     QListContainsFunction containsFunction,
                                     QListCountObjectFunction countObjectFunction,
                                     QListEndFunction endFunction,
                                     QListEndsWithFunction endsWithFunction,
                                     QListIndexOfFunction indexOfFunction,
                                     QListLastIndexOfFunction lastIndexOfFunction,
                                     QListMidFunction midFunction,
                                     QListEqualFunction equalFunction,
                                     QListSizeFunction sizeFunction,
                                     QListStartsWithFunction startsWithFunction,
                                     QListToSetFunction toSetFunction,
                                     QListValueFunction valueFunction,
                                     QListValueDefaultFunction valueDefaultFunction
                                );

QTJAMBI_EXPORT jobject qtjambi_from_QQueue_shared_pointer(JNIEnv *__jni_env,
                                     void* listPtr,
                                     PointerDeleter sharedPointerDeleter,
                                     PointerGetter sharedPointerGetter,
                                     const char* className,
                                     QListAppendFunction appendFunction,
                                     QListAppendListFunction appendListFunction,
                                     QListAtFunction atFunction,
                                     QListBeginFunction beginFunction,
                                     QListClearFunction clearFunction,
                                     QListContainsFunction containsFunction,
                                     QListCountObjectFunction countObjectFunction,
                                     QListEndFunction endFunction,
                                     QListEndsWithFunction endsWithFunction,
                                     QListIndexOfFunction indexOfFunction,
                                     QListInsertFunction insertFunction,
                                     QListLastIndexOfFunction lastIndexOfFunction,
                                     QListMidFunction midFunction,
                                     QListMoveFunction moveFunction,
                                     QListEqualFunction equalFunction,
                                     QListPrependFunction prependFunction,
                                     QListRemoveAllFunction removeAllFunction,
                                     QListRemoveAtFunction removeAtFunction,
                                     QListRemoveOneFunction removeOneFunction,
                                     QListReplaceFunction replaceFunction,
                                     QListReserveFunction reserveFunction,
                                     QListSizeFunction sizeFunction,
                                     QListStartsWithFunction startsWithFunction,
                                     QListSwapFunction swapFunction,
                                     QListTakeAtFunction takeAtFunction,
                                     QListToSetFunction toSetFunction,
                                     QListValueFunction valueFunction,
                                     QListValueDefaultFunction valueDefaultFunction
                                );

QTJAMBI_EXPORT jobject qtjambi_from_constQSet_shared_pointer(JNIEnv *__jni_env,
                                     void* listPtr,
                                     PointerDeleter sharedPointerDeleter,
                                     PointerGetter sharedPointerGetter,
                                     const char* className,
                                     QSetBeginFunction beginFunction,
                                     QSetCapacityFunction capacityFunction,
                                     QSetContainsFunction containsFunction,
                                     QSetEndFunction endFunction,
                                     QSetIntersectsFunction intersectsFunction,
                                     QSetEqualFunction equalFunction,
                                     QSetSizeFunction sizeFunction,
                                     QSetValuesFunction valuesFunction
                                );

QTJAMBI_EXPORT jobject qtjambi_from_QSet_shared_pointer(JNIEnv *__jni_env,
                                     void* listPtr,
                                     PointerDeleter sharedPointerDeleter,
                                     PointerGetter sharedPointerGetter,
                                     const char* className,
                                     QSetBeginFunction beginFunction,
                                     QSetCapacityFunction capacityFunction,
                                     QSetClearFunction clearFunction,
                                     QSetContainsFunction containsFunction,
                                     QSetEndFunction endFunction,
                                     QSetInsertFunction insertFunction,
                                     QSetIntersectFunction intersectFunction,
                                     QSetIntersectsFunction intersectsFunction,
                                     QSetEqualFunction equalFunction,
                                     QSetRemoveFunction removeFunction,
                                     QSetReserveFunction reserveFunction,
                                     QSetSizeFunction sizeFunction,
                                     QSetSubtractFunction subtractFunction,
                                     QSetUniteFunction uniteFunction,
                                     QSetValuesFunction valuesFunction
                                );

QTJAMBI_EXPORT jobject qtjambi_from_constQLinkedList_shared_pointer(JNIEnv *__jni_env,
                                     void* listPtr,
                                     PointerDeleter sharedPointerDeleter,
                                     PointerGetter sharedPointerGetter,
                                     const char* className,
                                     QLinkedListBeginFunction beginFunction,
                                     QLinkedListContainsFunction containsFunction,
                                     QLinkedListCountObjectFunction countObjectFunction,
                                     QLinkedListEndFunction endFunction,
                                     QLinkedListEndsWithFunction endsWithFunction,
                                     QLinkedListFirstFunction firstFunction,
                                     QLinkedListLastFunction lastFunction,
                                     QLinkedListEqualFunction equalFunction,
                                     QLinkedListSizeFunction sizeFunction,
                                     QLinkedListStartsWithFunction startsWithFunction
                                );

QTJAMBI_EXPORT jobject qtjambi_from_QLinkedList_shared_pointer(JNIEnv *__jni_env,
                                     void* listPtr,
                                     PointerDeleter sharedPointerDeleter,
                                     PointerGetter sharedPointerGetter,
                                     const char* className,
                                     QLinkedListAppendFunction appendFunction,
                                     QLinkedListBeginFunction beginFunction,
                                     QLinkedListClearFunction clearFunction,
                                     QLinkedListContainsFunction containsFunction,
                                     QLinkedListCountObjectFunction countObjectFunction,
                                     QLinkedListEndFunction endFunction,
                                     QLinkedListEndsWithFunction endsWithFunction,
                                     QLinkedListFirstFunction firstFunction,
                                     QLinkedListLastFunction lastFunction,
                                     QLinkedListEqualFunction equalFunction,
                                     QLinkedListPrependFunction prependFunction,
                                     QLinkedListRemoveAllFunction removeAllFunction,
                                     QLinkedListRemoveFirstFunction removeFirstFunction,
                                     QLinkedListRemoveLastFunction removeLastFunction,
                                     QLinkedListRemoveOneFunction removeOneFunction,
                                     QLinkedListSizeFunction sizeFunction,
                                     QLinkedListStartsWithFunction startsWithFunction,
                                     QLinkedListTakeFirstFunction takeFirstFunction,
                                     QLinkedListTakeLastFunction takeLastFunction
                                );

QTJAMBI_EXPORT jobject qtjambi_from_constQVector_shared_pointer(JNIEnv *__jni_env,
                                     void* listPtr,
                                     PointerDeleter sharedPointerDeleter,
                                     PointerGetter sharedPointerGetter,
                                     const char* className,
                                     QVectorAtFunction atFunction,
                                     QVectorBeginFunction beginFunction,
                                     QVectorCapacityFunction capacityFunction,
                                     QVectorContainsFunction containsFunction,
                                     QVectorCountObjectFunction countObjectFunction,
                                     QVectorEndFunction endFunction,
                                     QVectorEndsWithFunction endsWithFunction,
                                     QVectorIndexOfFunction indexOfFunction,
                                     QVectorLastIndexOfFunction lastIndexOfFunction,
                                     QVectorMidFunction midFunction,
                                     QVectorEqualFunction equalFunction,
                                     QVectorSizeFunction sizeFunction,
                                     QVectorStartsWithFunction startsWithFunction,
                                     QVectorToListFunction toListFunction,
                                     QVectorValueFunction valueFunction,
                                     QVectorValueDefaultFunction valueDefaultFunction
                                );



QTJAMBI_EXPORT jobject qtjambi_from_QVector_shared_pointer(JNIEnv *__jni_env,
                                     void* listPtr,
                                     PointerDeleter sharedPointerDeleter,
                                     PointerGetter sharedPointerGetter,
                                     const char* className,
                                     QVectorAppendFunction appendFunction,
                                     QVectorAppendVectorFunction appendListFunction,
                                     QVectorAtFunction atFunction,
                                     QVectorBeginFunction beginFunction,
                                     QVectorCapacityFunction capacityFunction,
                                     QVectorClearFunction clearFunction,
                                     QVectorContainsFunction containsFunction,
                                     QVectorCountObjectFunction countObjectFunction,
                                     QVectorEndFunction endFunction,
                                     QVectorEndsWithFunction endsWithFunction,
                                     QVectorFillFunction fillFunction,
                                     QVectorIndexOfFunction indexOfFunction,
                                     QVectorInsertFunction insertFunction,
                                     QVectorInsertNFunction insertNFunction,
                                     QVectorLastIndexOfFunction lastIndexOfFunction,
                                     QVectorMidFunction midFunction,
                                     QVectorMoveFunction moveFunction,
                                     QVectorEqualFunction equalFunction,
                                     QVectorPrependFunction prependFunction,
                                     QVectorRemoveAllFunction removeAllFunction,
                                     QVectorRemoveAtFunction removeAtFunction,
                                     QVectorRemoveNFunction removeNFunction,
                                     QVectorRemoveOneFunction removeOneFunction,
                                     QVectorReplaceFunction replaceFunction,
                                     QVectorReserveFunction reserveFunction,
                                     QVectorResizeFunction resizeFunction,
                                     QVectorShrinkToFitFunction shrinkToFitFunction,
                                     QVectorSizeFunction sizeFunction,
                                     QVectorStartsWithFunction startsWithFunction,
                                     QVectorTakeAtFunction takeAtFunction,
                                     QVectorToListFunction toListFunction,
                                     QVectorValueFunction valueFunction,
                                     QVectorValueDefaultFunction valueDefaultFunction
                                );



QTJAMBI_EXPORT jobject qtjambi_from_constQStack_shared_pointer(JNIEnv *__jni_env,
                                     void* listPtr,
                                     PointerDeleter sharedPointerDeleter,
                                     PointerGetter sharedPointerGetter,
                                     const char* className,
                                     QVectorAtFunction atFunction,
                                     QVectorBeginFunction beginFunction,
                                     QVectorCapacityFunction capacityFunction,
                                     QVectorContainsFunction containsFunction,
                                     QVectorCountObjectFunction countObjectFunction,
                                     QVectorEndFunction endFunction,
                                     QVectorEndsWithFunction endsWithFunction,
                                     QVectorIndexOfFunction indexOfFunction,
                                     QVectorLastIndexOfFunction lastIndexOfFunction,
                                     QVectorMidFunction midFunction,
                                     QVectorEqualFunction equalFunction,
                                     QVectorSizeFunction sizeFunction,
                                     QVectorStartsWithFunction startsWithFunction,
                                     QVectorToListFunction toListFunction,
                                     QVectorValueFunction valueFunction,
                                     QVectorValueDefaultFunction valueDefaultFunction
                                );



QTJAMBI_EXPORT jobject qtjambi_from_QStack_shared_pointer(JNIEnv *__jni_env,
                                     void* listPtr,
                                     PointerDeleter sharedPointerDeleter,
                                     PointerGetter sharedPointerGetter,
                                     const char* className,
                                     QVectorAppendFunction appendFunction,
                                     QVectorAppendVectorFunction appendListFunction,
                                     QVectorAtFunction atFunction,
                                     QVectorBeginFunction beginFunction,
                                     QVectorCapacityFunction capacityFunction,
                                     QVectorClearFunction clearFunction,
                                     QVectorContainsFunction containsFunction,
                                     QVectorCountObjectFunction countObjectFunction,
                                     QVectorEndFunction endFunction,
                                     QVectorEndsWithFunction endsWithFunction,
                                     QVectorFillFunction fillFunction,
                                     QVectorIndexOfFunction indexOfFunction,
                                     QVectorInsertFunction insertFunction,
                                     QVectorInsertNFunction insertNFunction,
                                     QVectorLastIndexOfFunction lastIndexOfFunction,
                                     QVectorMidFunction midFunction,
                                     QVectorMoveFunction moveFunction,
                                     QVectorEqualFunction equalFunction,
                                     QVectorPrependFunction prependFunction,
                                     QVectorRemoveAllFunction removeAllFunction,
                                     QVectorRemoveAtFunction removeAtFunction,
                                     QVectorRemoveNFunction removeNFunction,
                                     QVectorRemoveOneFunction removeOneFunction,
                                     QVectorReplaceFunction replaceFunction,
                                     QVectorReserveFunction reserveFunction,
                                     QVectorResizeFunction resizeFunction,
                                     QVectorShrinkToFitFunction shrinkToFitFunction,
                                     QVectorSizeFunction sizeFunction,
                                     QVectorStartsWithFunction startsWithFunction,
                                     QVectorTakeAtFunction takeAtFunction,
                                     QVectorToListFunction toListFunction,
                                     QVectorValueFunction valueFunction,
                                     QVectorValueDefaultFunction valueDefaultFunction
                                );

QTJAMBI_EXPORT jobject qtjambi_from_constQHash(JNIEnv *__jni_env,
                                     void* listPtr,
                                     PointerDeleter sharedPointerDeleter,
                                     PointerGetter sharedPointerGetter,
                                     const char* keyClassName,
                                     const char* valueClassName,
                                     QHashBeginFunction beginFunction,
                                     QHashCapacityFunction capacityFunction,
                                     QHashContainsFunction containsFunction,
                                     QHashCountObjectFunction countObjectFunction,
                                     QHashEndFunction endFunction,
                                     QHashFindFunction findFunction,
                                     QHashKeyFunction keyFunction,
                                     QHashKeysFunction keysFunction,
                                     QHashKeysForValueFunction keysForValueFunction,
                                     QHashEqualFunction equalFunction,
                                     QHashSizeFunction sizeFunction,
                                     QHashUniqueKeysFunction uniqueKeysFunction,
                                     QHashValueFunction valueFunction,
                                     QHashValuesFunction valuesFunction,
                                     QHashValuesKeyFunction valuesKeyFunction
                                );

QTJAMBI_EXPORT jobject qtjambi_from_QHash_shared_pointer(JNIEnv *__jni_env,
                                     void* listPtr,
                                     PointerDeleter sharedPointerDeleter,
                                     PointerGetter sharedPointerGetter,
                                     const char* keyClassName,
                                     const char* valueClassName,
                                     QHashBeginFunction beginFunction,
                                     QHashCapacityFunction capacityFunction,
                                     QHashClearFunction clearFunction,
                                     QHashContainsFunction containsFunction,
                                     QHashCountObjectFunction countObjectFunction,
                                     QHashEndFunction endFunction,
                                     QHashFindFunction findFunction,
                                     QHashInsertFunction insertFunction,
                                     QHashKeyFunction keyFunction,
                                     QHashKeysFunction keysFunction,
                                     QHashKeysForValueFunction keysForValueFunction,
                                     QHashEqualFunction equalFunction,
                                     QHashRemoveAllFunction removeAllFunction,
                                     QHashReserveFunction reserveFunction,
                                     QHashSizeFunction sizeFunction,
                                     QHashTakeFunction takeFunction,
                                     QHashUniqueKeysFunction uniqueKeysFunction,
                                     QHashUniteFunction uniteFunction,
                                     QHashValueFunction valueFunction,
                                     QHashValuesFunction valuesFunction,
                                     QHashValuesKeyFunction valuesKeyFunction
                                );

QTJAMBI_EXPORT jobject qtjambi_from_constQMultiHash_shared_pointer(JNIEnv *__jni_env,
                                     void* listPtr,
                                     PointerDeleter sharedPointerDeleter,
                                     PointerGetter sharedPointerGetter,
                                     const char* keyClassName,
                                     const char* valueClassName,
                                     QHashBeginFunction beginFunction,
                                     QHashCapacityFunction capacityFunction,
                                     QHashContainsFunction containsFunction,
                                     QHashCountObjectFunction countObjectFunction,
                                     QHashEndFunction endFunction,
                                     QHashFindFunction findFunction,
                                     QHashKeyFunction keyFunction,
                                     QHashKeysFunction keysFunction,
                                     QHashKeysForValueFunction keysForValueFunction,
                                     QHashEqualFunction equalFunction,
                                     QHashSizeFunction sizeFunction,
                                     QHashUniqueKeysFunction uniqueKeysFunction,
                                     QHashValueFunction valueFunction,
                                     QHashValuesFunction valuesFunction,
                                     QHashValuesKeyFunction valuesKeyFunction,
                                     QMultiHashContainsPairFunction containsPairFunction,
                                     QMultiHashCountPairFunction countPairFunction,
                                     QMultiHashFindPairFunction findPairFunction
                                );

QTJAMBI_EXPORT jobject qtjambi_from_QMultiHash_shared_pointer(JNIEnv *__jni_env,
                                     void* listPtr,
                                     PointerDeleter sharedPointerDeleter,
                                     PointerGetter sharedPointerGetter,
                                     const char* keyClassName,
                                     const char* valueClassName,
                                     QHashBeginFunction beginFunction,
                                     QHashCapacityFunction capacityFunction,
                                     QHashClearFunction clearFunction,
                                     QHashContainsFunction containsFunction,
                                     QHashCountObjectFunction countObjectFunction,
                                     QHashEndFunction endFunction,
                                     QHashFindFunction findFunction,
                                     QHashInsertFunction insertFunction,
                                     QHashKeyFunction keyFunction,
                                     QHashKeysFunction keysFunction,
                                     QHashKeysForValueFunction keysForValueFunction,
                                     QHashEqualFunction equalFunction,
                                     QHashRemoveAllFunction removeAllFunction,
                                     QHashReserveFunction reserveFunction,
                                     QHashSizeFunction sizeFunction,
                                     QHashTakeFunction takeFunction,
                                     QHashUniqueKeysFunction uniqueKeysFunction,
                                     QHashUniteFunction uniteFunction,
                                     QHashValueFunction valueFunction,
                                     QHashValuesFunction valuesFunction,
                                     QHashValuesKeyFunction valuesKeyFunction,
                                     QMultiHashContainsPairFunction containsPairFunction,
                                     QMultiHashCountPairFunction countPairFunction,
                                     QMultiHashFindPairFunction findPairFunction,
                                     QMultiHashRemovePairFunction removeAllPairFunction,
                                     QMultiHashReplaceFunction replaceOneFunction
                                );

QTJAMBI_EXPORT jobject qtjambi_from_constQMap_shared_pointer(JNIEnv *__jni_env,
                                     void* listPtr,
                                     PointerDeleter sharedPointerDeleter,
                                     PointerGetter sharedPointerGetter,
                                     const char* keyClassName,
                                     const char* valueClassName,
                                     QMapBeginFunction beginFunction,
                                     QMapContainsFunction containsFunction,
                                     QMapCountObjectFunction countObjectFunction,
                                     QMapEndFunction endFunction,
                                     QMapFindFunction findFunction,
                                     QMapFirstFunction firstFunction,
                                     QMapFirstKeyFunction firstKeyFunction,
                                     QMapKeyFunction keyFunction,
                                     QMapKeysFunction keysFunction,
                                     QMapKeysForValueFunction keysForValueFunction,
                                     QMapLastFunction lastFunction,
                                     QMapLastKeyFunction lastKeyFunction,
                                     QMapLowerBoundFunction lowerBoundFunction,
                                     QMapEqualFunction equalFunction,
                                     QMapSizeFunction sizeFunction,
                                     QMapUniqueKeysFunction uniqueKeysFunction,
                                     QMapUpperBoundFunction upperBoundFunction,
                                     QMapValueFunction valueFunction,
                                     QMapValuesFunction valuesFunction,
                                     QMapValuesKeyFunction valuesKeyFunction
                                );

QTJAMBI_EXPORT jobject qtjambi_from_QMap_shared_pointer(JNIEnv *__jni_env,
                                     void* listPtr,
                                     PointerDeleter sharedPointerDeleter,
                                     PointerGetter sharedPointerGetter,
                                     const char* keyClassName,
                                     const char* valueClassName,
                                     QMapBeginFunction beginFunction,
                                     QMapClearFunction clearFunction,
                                     QMapContainsFunction containsFunction,
                                     QMapCountObjectFunction countObjectFunction,
                                     QMapEndFunction endFunction,
                                     QMapFindFunction findFunction,
                                     QMapFirstFunction firstFunction,
                                     QMapFirstKeyFunction firstKeyFunction,
                                     QMapInsertFunction insertFunction,
                                     QMapKeyFunction keyFunction,
                                     QMapKeysFunction keysFunction,
                                     QMapKeysForValueFunction keysForValueFunction,
                                     QMapLastFunction lastFunction,
                                     QMapLastKeyFunction lastKeyFunction,
                                     QMapLowerBoundFunction lowerBoundFunction,
                                     QMapEqualFunction equalFunction,
                                     QMapRemoveAllFunction removeAllFunction,
                                     QMapSizeFunction sizeFunction,
                                     QMapTakeFunction takeFunction,
                                     QMapUniqueKeysFunction uniqueKeysFunction,
                                     QMapUniteFunction uniteFunction,
                                     QMapUpperBoundFunction upperBoundFunction,
                                     QMapValueFunction valueFunction,
                                     QMapValuesFunction valuesFunction,
                                     QMapValuesKeyFunction valuesKeyFunction
                                );

QTJAMBI_EXPORT jobject qtjambi_from_constQMultiMap_shared_pointer(JNIEnv *__jni_env,
                                     void* listPtr,
                                     PointerDeleter sharedPointerDeleter,
                                     PointerGetter sharedPointerGetter,
                                     const char* keyClassName,
                                     const char* valueClassName,
                                     QMapBeginFunction beginFunction,
                                     QMapContainsFunction containsFunction,
                                     QMapCountObjectFunction countObjectFunction,
                                     QMapEndFunction endFunction,
                                     QMapFindFunction findFunction,
                                     QMapFirstFunction firstFunction,
                                     QMapFirstKeyFunction firstKeyFunction,
                                     QMapKeyFunction keyFunction,
                                     QMapKeysFunction keysFunction,
                                     QMapKeysForValueFunction keysForValueFunction,
                                     QMapLastFunction lastFunction,
                                     QMapLastKeyFunction lastKeyFunction,
                                     QMapLowerBoundFunction lowerBoundFunction,
                                     QMapEqualFunction equalFunction,
                                     QMapSizeFunction sizeFunction,
                                     QMapUniqueKeysFunction uniqueKeysFunction,
                                     QMapUpperBoundFunction upperBoundFunction,
                                     QMapValueFunction valueFunction,
                                     QMapValuesFunction valuesFunction,
                                     QMapValuesKeyFunction valuesKeyFunction,
                                     QMultiMapContainsPairFunction containsPairFunction,
                                     QMultiMapCountPairFunction countPairFunction,
                                     QMultiMapFindPairFunction findPairFunction
                                );

QTJAMBI_EXPORT jobject qtjambi_from_QMultiMap_shared_pointer(JNIEnv *__jni_env,
                                     void* listPtr,
                                     PointerDeleter sharedPointerDeleter,
                                     PointerGetter sharedPointerGetter,
                                     const char* keyClassName,
                                     const char* valueClassName,
                                     QMapBeginFunction beginFunction,
                                     QMapClearFunction clearFunction,
                                     QMapContainsFunction containsFunction,
                                     QMapCountObjectFunction countObjectFunction,
                                     QMapEndFunction endFunction,
                                     QMapFindFunction findFunction,
                                     QMapFirstFunction firstFunction,
                                     QMapFirstKeyFunction firstKeyFunction,
                                     QMapInsertFunction insertFunction,
                                     QMapKeyFunction keyFunction,
                                     QMapKeysFunction keysFunction,
                                     QMapKeysForValueFunction keysForValueFunction,
                                     QMapLastFunction lastFunction,
                                     QMapLastKeyFunction lastKeyFunction,
                                     QMapLowerBoundFunction lowerBoundFunction,
                                     QMapEqualFunction equalFunction,
                                     QMapRemoveAllFunction removeAllFunction,
                                     QMapSizeFunction sizeFunction,
                                     QMapTakeFunction takeFunction,
                                     QMapUniqueKeysFunction uniqueKeysFunction,
                                     QMapUniteFunction uniteFunction,
                                     QMapUpperBoundFunction upperBoundFunction,
                                     QMapValueFunction valueFunction,
                                     QMapValuesFunction valuesFunction,
                                     QMapValuesKeyFunction valuesKeyFunction,
                                     QMultiMapContainsPairFunction containsPairFunction,
                                     QMultiMapCountPairFunction countPairFunction,
                                     QMultiMapFindPairFunction findPairFunction,
                                     QMultiMapRemovePairFunction removeAllPairFunction,
                                     QMultiMapReplaceFunction replaceOneFunction
                                );

QTJAMBI_EXPORT jobject qtjambi_to_QIterator(JNIEnv *__jni_env,
                                            QtJambiNativeID owner,
                                           void* iteratorPtr,
                                           QIteratorValueFunction valueFunction,
                                           QIteratorIncrementFunction incrementFunction,
                                           QIteratorDecrementFunction decrementFunction,
                                           QIteratorLessThanFunction lessThanFunction,
                                           QIteratorEqualsFunction equalsFunction,
                                           PtrDeleterFunction destructor_function
                                );

QTJAMBI_EXPORT jobject qtjambi_to_QMapIterator(JNIEnv *__jni_env,
                                           QtJambiNativeID owner,
                                           void* iteratorPtr,
                                           QMapIteratorKeyFunction keyFunction,
                                           QIteratorValueFunction valueFunction,
                                           QIteratorIncrementFunction incrementFunction,
                                           QIteratorDecrementFunction decrementFunction,
                                           QIteratorLessThanFunction lessThanFunction,
                                           QIteratorEqualsFunction equalsFunction,
                                           PtrDeleterFunction destructor_function
                                );

QTJAMBI_EXPORT jobject qtjambi_to_constQList(JNIEnv *__jni_env,
                                     QtJambiNativeID owner,
                                     const void* listPtr,
                                     const std::type_info& typeId,
                                     QListAtFunction atFunction,
                                     QListBeginFunction beginFunction,
                                     QListContainsFunction containsFunction,
                                     QListCountObjectFunction countObjectFunction,
                                     QListEndFunction endFunction,
                                     QListEndsWithFunction endsWithFunction,
                                     QListIndexOfFunction indexOfFunction,
                                     QListLastIndexOfFunction lastIndexOfFunction,
                                     QListMidFunction midFunction,
                                     QListEqualFunction equalFunction,
                                     QListSizeFunction sizeFunction,
                                     QListStartsWithFunction startsWithFunction,
                                     QListToSetFunction toSetFunction,
                                     QListValueFunction valueFunction,
                                     QListValueDefaultFunction valueDefaultFunction
                                );

QTJAMBI_EXPORT jobject qtjambi_to_QList(JNIEnv *__jni_env,
                                     QtJambiNativeID owner,
                                     void* listPtr,
                                     const std::type_info& typeId,
                                     QListAppendFunction appendFunction,
                                     QListAppendListFunction appendListFunction,
                                     QListAtFunction atFunction,
                                     QListBeginFunction beginFunction,
                                     QListClearFunction clearFunction,
                                     QListContainsFunction containsFunction,
                                     QListCountObjectFunction countObjectFunction,
                                     QListEndFunction endFunction,
                                     QListEndsWithFunction endsWithFunction,
                                     QListIndexOfFunction indexOfFunction,
                                     QListInsertFunction insertFunction,
                                     QListLastIndexOfFunction lastIndexOfFunction,
                                     QListMidFunction midFunction,
                                     QListMoveFunction moveFunction,
                                     QListEqualFunction equalFunction,
                                     QListPrependFunction prependFunction,
                                     QListRemoveAllFunction removeAllFunction,
                                     QListRemoveAtFunction removeAtFunction,
                                     QListRemoveOneFunction removeOneFunction,
                                     QListReplaceFunction replaceFunction,
                                     QListReserveFunction reserveFunction,
                                     QListSizeFunction sizeFunction,
                                     QListStartsWithFunction startsWithFunction,
                                     QListSwapFunction swapFunction,
                                     QListTakeAtFunction takeAtFunction,
                                     QListToSetFunction toSetFunction,
                                     QListValueFunction valueFunction,
                                     QListValueDefaultFunction valueDefaultFunction
                                );

QTJAMBI_EXPORT jobject qtjambi_to_constQQueue(JNIEnv *__jni_env,
                                     QtJambiNativeID owner,
                                     const void* listPtr,
                                     const std::type_info& typeId,
                                     QListAtFunction atFunction,
                                     QListBeginFunction beginFunction,
                                     QListContainsFunction containsFunction,
                                     QListCountObjectFunction countObjectFunction,
                                     QListEndFunction endFunction,
                                     QListEndsWithFunction endsWithFunction,
                                     QListIndexOfFunction indexOfFunction,
                                     QListLastIndexOfFunction lastIndexOfFunction,
                                     QListMidFunction midFunction,
                                     QListEqualFunction equalFunction,
                                     QListSizeFunction sizeFunction,
                                     QListStartsWithFunction startsWithFunction,
                                     QListToSetFunction toSetFunction,
                                     QListValueFunction valueFunction,
                                     QListValueDefaultFunction valueDefaultFunction
                                );

QTJAMBI_EXPORT jobject qtjambi_to_QQueue(JNIEnv *__jni_env,
                                     QtJambiNativeID owner,
                                     void* listPtr,
                                     const std::type_info& typeId,
                                     QListAppendFunction appendFunction,
                                     QListAppendListFunction appendListFunction,
                                     QListAtFunction atFunction,
                                     QListBeginFunction beginFunction,
                                     QListClearFunction clearFunction,
                                     QListContainsFunction containsFunction,
                                     QListCountObjectFunction countObjectFunction,
                                     QListEndFunction endFunction,
                                     QListEndsWithFunction endsWithFunction,
                                     QListIndexOfFunction indexOfFunction,
                                     QListInsertFunction insertFunction,
                                     QListLastIndexOfFunction lastIndexOfFunction,
                                     QListMidFunction midFunction,
                                     QListMoveFunction moveFunction,
                                     QListEqualFunction equalFunction,
                                     QListPrependFunction prependFunction,
                                     QListRemoveAllFunction removeAllFunction,
                                     QListRemoveAtFunction removeAtFunction,
                                     QListRemoveOneFunction removeOneFunction,
                                     QListReplaceFunction replaceFunction,
                                     QListReserveFunction reserveFunction,
                                     QListSizeFunction sizeFunction,
                                     QListStartsWithFunction startsWithFunction,
                                     QListSwapFunction swapFunction,
                                     QListTakeAtFunction takeAtFunction,
                                     QListToSetFunction toSetFunction,
                                     QListValueFunction valueFunction,
                                     QListValueDefaultFunction valueDefaultFunction
                                );

QTJAMBI_EXPORT jobject qtjambi_to_constQSet(JNIEnv *__jni_env,
                                     QtJambiNativeID owner,
                                     const void* listPtr,
                                     const std::type_info& typeId,
                                     QSetBeginFunction beginFunction,
                                     QSetCapacityFunction capacityFunction,
                                     QSetContainsFunction containsFunction,
                                     QSetEndFunction endFunction,
                                     QSetIntersectsFunction intersectsFunction,
                                     QSetEqualFunction equalFunction,
                                     QSetSizeFunction sizeFunction,
                                     QSetValuesFunction valuesFunction
                                );

QTJAMBI_EXPORT jobject qtjambi_to_QSet(JNIEnv *__jni_env,
                                     QtJambiNativeID owner,
                                     void* listPtr,
                                     const std::type_info& typeId,
                                     QSetBeginFunction beginFunction,
                                     QSetCapacityFunction capacityFunction,
                                     QSetClearFunction clearFunction,
                                     QSetContainsFunction containsFunction,
                                     QSetEndFunction endFunction,
                                     QSetInsertFunction insertFunction,
                                     QSetIntersectFunction intersectFunction,
                                     QSetIntersectsFunction intersectsFunction,
                                     QSetEqualFunction equalFunction,
                                     QSetRemoveFunction removeFunction,
                                     QSetReserveFunction reserveFunction,
                                     QSetSizeFunction sizeFunction,
                                     QSetSubtractFunction subtractFunction,
                                     QSetUniteFunction uniteFunction,
                                     QSetValuesFunction valuesFunction
                                );

QTJAMBI_EXPORT jobject qtjambi_to_constQLinkedList(JNIEnv *__jni_env,
                                     QtJambiNativeID owner,
                                     const void* listPtr,
                                     const std::type_info& typeId,
                                     QLinkedListBeginFunction beginFunction,
                                     QLinkedListContainsFunction containsFunction,
                                     QLinkedListCountObjectFunction countObjectFunction,
                                     QLinkedListEndFunction endFunction,
                                     QLinkedListEndsWithFunction endsWithFunction,
                                     QLinkedListFirstFunction firstFunction,
                                     QLinkedListLastFunction lastFunction,
                                     QLinkedListEqualFunction equalFunction,
                                     QLinkedListSizeFunction sizeFunction,
                                     QLinkedListStartsWithFunction startsWithFunction
                                );

QTJAMBI_EXPORT jobject qtjambi_to_QLinkedList(JNIEnv *__jni_env,
                                     QtJambiNativeID owner,
                                     void* listPtr,
                                     const std::type_info& typeId,
                                     QLinkedListAppendFunction appendFunction,
                                     QLinkedListBeginFunction beginFunction,
                                     QLinkedListClearFunction clearFunction,
                                     QLinkedListContainsFunction containsFunction,
                                     QLinkedListCountObjectFunction countObjectFunction,
                                     QLinkedListEndFunction endFunction,
                                     QLinkedListEndsWithFunction endsWithFunction,
                                     QLinkedListFirstFunction firstFunction,
                                     QLinkedListLastFunction lastFunction,
                                     QLinkedListEqualFunction equalFunction,
                                     QLinkedListPrependFunction prependFunction,
                                     QLinkedListRemoveAllFunction removeAllFunction,
                                     QLinkedListRemoveFirstFunction removeFirstFunction,
                                     QLinkedListRemoveLastFunction removeLastFunction,
                                     QLinkedListRemoveOneFunction removeOneFunction,
                                     QLinkedListSizeFunction sizeFunction,
                                     QLinkedListStartsWithFunction startsWithFunction,
                                     QLinkedListTakeFirstFunction takeFirstFunction,
                                     QLinkedListTakeLastFunction takeLastFunction
                                );

QTJAMBI_EXPORT jobject qtjambi_to_constQVector(JNIEnv *__jni_env,
                                     QtJambiNativeID owner,
                                     const void* listPtr,
                                     const std::type_info& typeId,
                                     QVectorAtFunction atFunction,
                                     QVectorBeginFunction beginFunction,
                                     QVectorCapacityFunction capacityFunction,
                                     QVectorContainsFunction containsFunction,
                                     QVectorCountObjectFunction countObjectFunction,
                                     QVectorEndFunction endFunction,
                                     QVectorEndsWithFunction endsWithFunction,
                                     QVectorIndexOfFunction indexOfFunction,
                                     QVectorLastIndexOfFunction lastIndexOfFunction,
                                     QVectorMidFunction midFunction,
                                     QVectorEqualFunction equalFunction,
                                     QVectorSizeFunction sizeFunction,
                                     QVectorStartsWithFunction startsWithFunction,
                                     QVectorToListFunction toListFunction,
                                     QVectorValueFunction valueFunction,
                                     QVectorValueDefaultFunction valueDefaultFunction
                                );



QTJAMBI_EXPORT jobject qtjambi_to_QVector(JNIEnv *__jni_env,
                                     QtJambiNativeID owner,
                                     void* listPtr,
                                     const std::type_info& typeId,
                                     QVectorAppendFunction appendFunction,
                                     QVectorAppendVectorFunction appendListFunction,
                                     QVectorAtFunction atFunction,
                                     QVectorBeginFunction beginFunction,
                                     QVectorCapacityFunction capacityFunction,
                                     QVectorClearFunction clearFunction,
                                     QVectorContainsFunction containsFunction,
                                     QVectorCountObjectFunction countObjectFunction,
                                     QVectorEndFunction endFunction,
                                     QVectorEndsWithFunction endsWithFunction,
                                     QVectorFillFunction fillFunction,
                                     QVectorIndexOfFunction indexOfFunction,
                                     QVectorInsertFunction insertFunction,
                                     QVectorInsertNFunction insertNFunction,
                                     QVectorLastIndexOfFunction lastIndexOfFunction,
                                     QVectorMidFunction midFunction,
                                     QVectorMoveFunction moveFunction,
                                     QVectorEqualFunction equalFunction,
                                     QVectorPrependFunction prependFunction,
                                     QVectorRemoveAllFunction removeAllFunction,
                                     QVectorRemoveAtFunction removeAtFunction,
                                     QVectorRemoveNFunction removeNFunction,
                                     QVectorRemoveOneFunction removeOneFunction,
                                     QVectorReplaceFunction replaceFunction,
                                     QVectorReserveFunction reserveFunction,
                                     QVectorResizeFunction resizeFunction,
                                     QVectorShrinkToFitFunction shrinkToFitFunction,
                                     QVectorSizeFunction sizeFunction,
                                     QVectorStartsWithFunction startsWithFunction,
                                     QVectorTakeAtFunction takeAtFunction,
                                     QVectorToListFunction toListFunction,
                                     QVectorValueFunction valueFunction,
                                     QVectorValueDefaultFunction valueDefaultFunction
                                );



QTJAMBI_EXPORT jobject qtjambi_to_constQStack(JNIEnv *__jni_env,
                                     QtJambiNativeID owner,
                                     const void* listPtr,
                                     const std::type_info& typeId,
                                     QVectorAtFunction atFunction,
                                     QVectorBeginFunction beginFunction,
                                     QVectorCapacityFunction capacityFunction,
                                     QVectorContainsFunction containsFunction,
                                     QVectorCountObjectFunction countObjectFunction,
                                     QVectorEndFunction endFunction,
                                     QVectorEndsWithFunction endsWithFunction,
                                     QVectorIndexOfFunction indexOfFunction,
                                     QVectorLastIndexOfFunction lastIndexOfFunction,
                                     QVectorMidFunction midFunction,
                                     QVectorEqualFunction equalFunction,
                                     QVectorSizeFunction sizeFunction,
                                     QVectorStartsWithFunction startsWithFunction,
                                     QVectorToListFunction toListFunction,
                                     QVectorValueFunction valueFunction,
                                     QVectorValueDefaultFunction valueDefaultFunction
                                );



QTJAMBI_EXPORT jobject qtjambi_to_QStack(JNIEnv *__jni_env,
                                     QtJambiNativeID owner,
                                     void* listPtr,
                                     const std::type_info& typeId,
                                     QVectorAppendFunction appendFunction,
                                     QVectorAppendVectorFunction appendListFunction,
                                     QVectorAtFunction atFunction,
                                     QVectorBeginFunction beginFunction,
                                     QVectorCapacityFunction capacityFunction,
                                     QVectorClearFunction clearFunction,
                                     QVectorContainsFunction containsFunction,
                                     QVectorCountObjectFunction countObjectFunction,
                                     QVectorEndFunction endFunction,
                                     QVectorEndsWithFunction endsWithFunction,
                                     QVectorFillFunction fillFunction,
                                     QVectorIndexOfFunction indexOfFunction,
                                     QVectorInsertFunction insertFunction,
                                     QVectorInsertNFunction insertNFunction,
                                     QVectorLastIndexOfFunction lastIndexOfFunction,
                                     QVectorMidFunction midFunction,
                                     QVectorMoveFunction moveFunction,
                                     QVectorEqualFunction equalFunction,
                                     QVectorPrependFunction prependFunction,
                                     QVectorRemoveAllFunction removeAllFunction,
                                     QVectorRemoveAtFunction removeAtFunction,
                                     QVectorRemoveNFunction removeNFunction,
                                     QVectorRemoveOneFunction removeOneFunction,
                                     QVectorReplaceFunction replaceFunction,
                                     QVectorReserveFunction reserveFunction,
                                     QVectorResizeFunction resizeFunction,
                                     QVectorShrinkToFitFunction shrinkToFitFunction,
                                     QVectorSizeFunction sizeFunction,
                                     QVectorStartsWithFunction startsWithFunction,
                                     QVectorTakeAtFunction takeAtFunction,
                                     QVectorToListFunction toListFunction,
                                     QVectorValueFunction valueFunction,
                                     QVectorValueDefaultFunction valueDefaultFunction
                                );

QTJAMBI_EXPORT jobject qtjambi_to_constQHash(JNIEnv *__jni_env,
                                     QtJambiNativeID owner,
                                     const void* listPtr,
                                     const std::type_info& keyTypeId,
                                     const std::type_info& valueTypeId,
                                     QHashBeginFunction beginFunction,
                                     QHashCapacityFunction capacityFunction,
                                     QHashContainsFunction containsFunction,
                                     QHashCountObjectFunction countObjectFunction,
                                     QHashEndFunction endFunction,
                                     QHashFindFunction findFunction,
                                     QHashKeyFunction keyFunction,
                                     QHashKeysFunction keysFunction,
                                     QHashKeysForValueFunction keysForValueFunction,
                                     QHashEqualFunction equalFunction,
                                     QHashSizeFunction sizeFunction,
                                     QHashUniqueKeysFunction uniqueKeysFunction,
                                     QHashValueFunction valueFunction,
                                     QHashValuesFunction valuesFunction,
                                     QHashValuesKeyFunction valuesKeyFunction
                                );

QTJAMBI_EXPORT jobject qtjambi_to_QHash(JNIEnv *__jni_env,
                                     QtJambiNativeID owner,
                                     void* listPtr,
                                     const std::type_info& keyTypeId,
                                     const std::type_info& valueTypeId,
                                     QHashBeginFunction beginFunction,
                                     QHashCapacityFunction capacityFunction,
                                     QHashClearFunction clearFunction,
                                     QHashContainsFunction containsFunction,
                                     QHashCountObjectFunction countObjectFunction,
                                     QHashEndFunction endFunction,
                                     QHashFindFunction findFunction,
                                     QHashInsertFunction insertFunction,
                                     QHashKeyFunction keyFunction,
                                     QHashKeysFunction keysFunction,
                                     QHashKeysForValueFunction keysForValueFunction,
                                     QHashEqualFunction equalFunction,
                                     QHashRemoveAllFunction removeAllFunction,
                                     QHashReserveFunction reserveFunction,
                                     QHashSizeFunction sizeFunction,
                                     QHashTakeFunction takeFunction,
                                     QHashUniqueKeysFunction uniqueKeysFunction,
                                     QHashUniteFunction uniteFunction,
                                     QHashValueFunction valueFunction,
                                     QHashValuesFunction valuesFunction,
                                     QHashValuesKeyFunction valuesKeyFunction
                                );

QTJAMBI_EXPORT jobject qtjambi_to_constQMultiHash(JNIEnv *__jni_env,
                                     QtJambiNativeID owner,
                                     const void* listPtr,
                                     const std::type_info& keyTypeId,
                                     const std::type_info& valueTypeId,
                                     QHashBeginFunction beginFunction,
                                     QHashCapacityFunction capacityFunction,
                                     QHashContainsFunction containsFunction,
                                     QHashCountObjectFunction countObjectFunction,
                                     QHashEndFunction endFunction,
                                     QHashFindFunction findFunction,
                                     QHashKeyFunction keyFunction,
                                     QHashKeysFunction keysFunction,
                                     QHashKeysForValueFunction keysForValueFunction,
                                     QHashEqualFunction equalFunction,
                                     QHashSizeFunction sizeFunction,
                                     QHashUniqueKeysFunction uniqueKeysFunction,
                                     QHashValueFunction valueFunction,
                                     QHashValuesFunction valuesFunction,
                                     QHashValuesKeyFunction valuesKeyFunction,
                                     QMultiHashContainsPairFunction containsPairFunction,
                                     QMultiHashCountPairFunction countPairFunction,
                                     QMultiHashFindPairFunction findPairFunction
                                );

QTJAMBI_EXPORT jobject qtjambi_to_QMultiHash(JNIEnv *__jni_env,
                                     QtJambiNativeID owner,
                                     void* listPtr,
                                     const std::type_info& keyTypeId,
                                     const std::type_info& valueTypeId,
                                     QHashBeginFunction beginFunction,
                                     QHashCapacityFunction capacityFunction,
                                     QHashClearFunction clearFunction,
                                     QHashContainsFunction containsFunction,
                                     QHashCountObjectFunction countObjectFunction,
                                     QHashEndFunction endFunction,
                                     QHashFindFunction findFunction,
                                     QHashInsertFunction insertFunction,
                                     QHashKeyFunction keyFunction,
                                     QHashKeysFunction keysFunction,
                                     QHashKeysForValueFunction keysForValueFunction,
                                     QHashEqualFunction equalFunction,
                                     QHashRemoveAllFunction removeAllFunction,
                                     QHashReserveFunction reserveFunction,
                                     QHashSizeFunction sizeFunction,
                                     QHashTakeFunction takeFunction,
                                     QHashUniqueKeysFunction uniqueKeysFunction,
                                     QHashUniteFunction uniteFunction,
                                     QHashValueFunction valueFunction,
                                     QHashValuesFunction valuesFunction,
                                     QHashValuesKeyFunction valuesKeyFunction,
                                     QMultiHashContainsPairFunction containsPairFunction,
                                     QMultiHashCountPairFunction countPairFunction,
                                     QMultiHashFindPairFunction findPairFunction,
                                     QMultiHashRemovePairFunction removeAllPairFunction,
                                     QMultiHashReplaceFunction replaceOneFunction
                                );

QTJAMBI_EXPORT jobject qtjambi_to_constQMap(JNIEnv *__jni_env,
                                     QtJambiNativeID owner,
                                     const void* listPtr,
                                     const std::type_info& keyTypeId,
                                     const std::type_info& valueTypeId,
                                     QMapBeginFunction beginFunction,
                                     QMapContainsFunction containsFunction,
                                     QMapCountObjectFunction countObjectFunction,
                                     QMapEndFunction endFunction,
                                     QMapFindFunction findFunction,
                                     QMapFirstFunction firstFunction,
                                     QMapFirstKeyFunction firstKeyFunction,
                                     QMapKeyFunction keyFunction,
                                     QMapKeysFunction keysFunction,
                                     QMapKeysForValueFunction keysForValueFunction,
                                     QMapLastFunction lastFunction,
                                     QMapLastKeyFunction lastKeyFunction,
                                     QMapLowerBoundFunction lowerBoundFunction,
                                     QMapEqualFunction equalFunction,
                                     QMapSizeFunction sizeFunction,
                                     QMapUniqueKeysFunction uniqueKeysFunction,
                                     QMapUpperBoundFunction upperBoundFunction,
                                     QMapValueFunction valueFunction,
                                     QMapValuesFunction valuesFunction,
                                     QMapValuesKeyFunction valuesKeyFunction
                                );

QTJAMBI_EXPORT jobject qtjambi_to_QMap(JNIEnv *__jni_env,
                                     QtJambiNativeID owner,
                                     void* listPtr,
                                     const std::type_info& keyTypeId,
                                     const std::type_info& valueTypeId,
                                     QMapBeginFunction beginFunction,
                                     QMapClearFunction clearFunction,
                                     QMapContainsFunction containsFunction,
                                     QMapCountObjectFunction countObjectFunction,
                                     QMapEndFunction endFunction,
                                     QMapFindFunction findFunction,
                                     QMapFirstFunction firstFunction,
                                     QMapFirstKeyFunction firstKeyFunction,
                                     QMapInsertFunction insertFunction,
                                     QMapKeyFunction keyFunction,
                                     QMapKeysFunction keysFunction,
                                     QMapKeysForValueFunction keysForValueFunction,
                                     QMapLastFunction lastFunction,
                                     QMapLastKeyFunction lastKeyFunction,
                                     QMapLowerBoundFunction lowerBoundFunction,
                                     QMapEqualFunction equalFunction,
                                     QMapRemoveAllFunction removeAllFunction,
                                     QMapSizeFunction sizeFunction,
                                     QMapTakeFunction takeFunction,
                                     QMapUniqueKeysFunction uniqueKeysFunction,
                                     QMapUniteFunction uniteFunction,
                                     QMapUpperBoundFunction upperBoundFunction,
                                     QMapValueFunction valueFunction,
                                     QMapValuesFunction valuesFunction,
                                     QMapValuesKeyFunction valuesKeyFunction
                                );

QTJAMBI_EXPORT jobject qtjambi_to_constQMultiMap(JNIEnv *__jni_env,
                                     QtJambiNativeID owner,
                                     const void* listPtr,
                                     const std::type_info& keyTypeId,
                                     const std::type_info& valueTypeId,
                                     QMapBeginFunction beginFunction,
                                     QMapContainsFunction containsFunction,
                                     QMapCountObjectFunction countObjectFunction,
                                     QMapEndFunction endFunction,
                                     QMapFindFunction findFunction,
                                     QMapFirstFunction firstFunction,
                                     QMapFirstKeyFunction firstKeyFunction,
                                     QMapKeyFunction keyFunction,
                                     QMapKeysFunction keysFunction,
                                     QMapKeysForValueFunction keysForValueFunction,
                                     QMapLastFunction lastFunction,
                                     QMapLastKeyFunction lastKeyFunction,
                                     QMapLowerBoundFunction lowerBoundFunction,
                                     QMapEqualFunction equalFunction,
                                     QMapSizeFunction sizeFunction,
                                     QMapUniqueKeysFunction uniqueKeysFunction,
                                     QMapUpperBoundFunction upperBoundFunction,
                                     QMapValueFunction valueFunction,
                                     QMapValuesFunction valuesFunction,
                                     QMapValuesKeyFunction valuesKeyFunction,
                                     QMultiMapContainsPairFunction containsPairFunction,
                                     QMultiMapCountPairFunction countPairFunction,
                                     QMultiMapFindPairFunction findPairFunction
                                );

QTJAMBI_EXPORT jobject qtjambi_to_QMultiMap(JNIEnv *__jni_env,
                                     QtJambiNativeID owner,
                                     void* listPtr,
                                     const std::type_info& keyTypeId,
                                     const std::type_info& valueTypeId,
                                     QMapBeginFunction beginFunction,
                                     QMapClearFunction clearFunction,
                                     QMapContainsFunction containsFunction,
                                     QMapCountObjectFunction countObjectFunction,
                                     QMapEndFunction endFunction,
                                     QMapFindFunction findFunction,
                                     QMapFirstFunction firstFunction,
                                     QMapFirstKeyFunction firstKeyFunction,
                                     QMapInsertFunction insertFunction,
                                     QMapKeyFunction keyFunction,
                                     QMapKeysFunction keysFunction,
                                     QMapKeysForValueFunction keysForValueFunction,
                                     QMapLastFunction lastFunction,
                                     QMapLastKeyFunction lastKeyFunction,
                                     QMapLowerBoundFunction lowerBoundFunction,
                                     QMapEqualFunction equalFunction,
                                     QMapRemoveAllFunction removeAllFunction,
                                     QMapSizeFunction sizeFunction,
                                     QMapTakeFunction takeFunction,
                                     QMapUniqueKeysFunction uniqueKeysFunction,
                                     QMapUniteFunction uniteFunction,
                                     QMapUpperBoundFunction upperBoundFunction,
                                     QMapValueFunction valueFunction,
                                     QMapValuesFunction valuesFunction,
                                     QMapValuesKeyFunction valuesKeyFunction,
                                     QMultiMapContainsPairFunction containsPairFunction,
                                     QMultiMapCountPairFunction countPairFunction,
                                     QMultiMapFindPairFunction findPairFunction,
                                     QMultiMapRemovePairFunction removeAllPairFunction,
                                     QMultiMapReplaceFunction replaceOneFunction
                                );

QTJAMBI_EXPORT jobject qtjambi_from_constQList_shared_pointer(JNIEnv *__jni_env,
                                     void* listPtr,
                                     PointerDeleter sharedPointerDeleter,
                                     PointerGetter sharedPointerGetter,
                                     const std::type_info& typeId,
                                     QListAtFunction atFunction,
                                     QListBeginFunction beginFunction,
                                     QListContainsFunction containsFunction,
                                     QListCountObjectFunction countObjectFunction,
                                     QListEndFunction endFunction,
                                     QListEndsWithFunction endsWithFunction,
                                     QListIndexOfFunction indexOfFunction,
                                     QListLastIndexOfFunction lastIndexOfFunction,
                                     QListMidFunction midFunction,
                                     QListEqualFunction equalFunction,
                                     QListSizeFunction sizeFunction,
                                     QListStartsWithFunction startsWithFunction,
                                     QListToSetFunction toSetFunction,
                                     QListValueFunction valueFunction,
                                     QListValueDefaultFunction valueDefaultFunction
                                );

QTJAMBI_EXPORT jobject qtjambi_from_QList_shared_pointer(JNIEnv *__jni_env,
                                     void* listPtr,
                                     PointerDeleter sharedPointerDeleter,
                                     PointerGetter sharedPointerGetter,
                                     const std::type_info& typeId,
                                     QListAppendFunction appendFunction,
                                     QListAppendListFunction appendListFunction,
                                     QListAtFunction atFunction,
                                     QListBeginFunction beginFunction,
                                     QListClearFunction clearFunction,
                                     QListContainsFunction containsFunction,
                                     QListCountObjectFunction countObjectFunction,
                                     QListEndFunction endFunction,
                                     QListEndsWithFunction endsWithFunction,
                                     QListIndexOfFunction indexOfFunction,
                                     QListInsertFunction insertFunction,
                                     QListLastIndexOfFunction lastIndexOfFunction,
                                     QListMidFunction midFunction,
                                     QListMoveFunction moveFunction,
                                     QListEqualFunction equalFunction,
                                     QListPrependFunction prependFunction,
                                     QListRemoveAllFunction removeAllFunction,
                                     QListRemoveAtFunction removeAtFunction,
                                     QListRemoveOneFunction removeOneFunction,
                                     QListReplaceFunction replaceFunction,
                                     QListReserveFunction reserveFunction,
                                     QListSizeFunction sizeFunction,
                                     QListStartsWithFunction startsWithFunction,
                                     QListSwapFunction swapFunction,
                                     QListTakeAtFunction takeAtFunction,
                                     QListToSetFunction toSetFunction,
                                     QListValueFunction valueFunction,
                                     QListValueDefaultFunction valueDefaultFunction
                                );

QTJAMBI_EXPORT jobject qtjambi_from_constQQueue_shared_pointer(JNIEnv *__jni_env,
                                     void* listPtr,
                                     PointerDeleter sharedPointerDeleter,
                                     PointerGetter sharedPointerGetter,
                                     const std::type_info& typeId,
                                     QListAtFunction atFunction,
                                     QListBeginFunction beginFunction,
                                     QListContainsFunction containsFunction,
                                     QListCountObjectFunction countObjectFunction,
                                     QListEndFunction endFunction,
                                     QListEndsWithFunction endsWithFunction,
                                     QListIndexOfFunction indexOfFunction,
                                     QListLastIndexOfFunction lastIndexOfFunction,
                                     QListMidFunction midFunction,
                                     QListEqualFunction equalFunction,
                                     QListSizeFunction sizeFunction,
                                     QListStartsWithFunction startsWithFunction,
                                     QListToSetFunction toSetFunction,
                                     QListValueFunction valueFunction,
                                     QListValueDefaultFunction valueDefaultFunction
                                );

QTJAMBI_EXPORT jobject qtjambi_from_QQueue_shared_pointer(JNIEnv *__jni_env,
                                     void* listPtr,
                                     PointerDeleter sharedPointerDeleter,
                                     PointerGetter sharedPointerGetter,
                                     const std::type_info& typeId,
                                     QListAppendFunction appendFunction,
                                     QListAppendListFunction appendListFunction,
                                     QListAtFunction atFunction,
                                     QListBeginFunction beginFunction,
                                     QListClearFunction clearFunction,
                                     QListContainsFunction containsFunction,
                                     QListCountObjectFunction countObjectFunction,
                                     QListEndFunction endFunction,
                                     QListEndsWithFunction endsWithFunction,
                                     QListIndexOfFunction indexOfFunction,
                                     QListInsertFunction insertFunction,
                                     QListLastIndexOfFunction lastIndexOfFunction,
                                     QListMidFunction midFunction,
                                     QListMoveFunction moveFunction,
                                     QListEqualFunction equalFunction,
                                     QListPrependFunction prependFunction,
                                     QListRemoveAllFunction removeAllFunction,
                                     QListRemoveAtFunction removeAtFunction,
                                     QListRemoveOneFunction removeOneFunction,
                                     QListReplaceFunction replaceFunction,
                                     QListReserveFunction reserveFunction,
                                     QListSizeFunction sizeFunction,
                                     QListStartsWithFunction startsWithFunction,
                                     QListSwapFunction swapFunction,
                                     QListTakeAtFunction takeAtFunction,
                                     QListToSetFunction toSetFunction,
                                     QListValueFunction valueFunction,
                                     QListValueDefaultFunction valueDefaultFunction
                                );

QTJAMBI_EXPORT jobject qtjambi_from_constQSet_shared_pointer(JNIEnv *__jni_env,
                                     void* listPtr,
                                     PointerDeleter sharedPointerDeleter,
                                     PointerGetter sharedPointerGetter,
                                     const std::type_info& typeId,
                                     QSetBeginFunction beginFunction,
                                     QSetCapacityFunction capacityFunction,
                                     QSetContainsFunction containsFunction,
                                     QSetEndFunction endFunction,
                                     QSetIntersectsFunction intersectsFunction,
                                     QSetEqualFunction equalFunction,
                                     QSetSizeFunction sizeFunction,
                                     QSetValuesFunction valuesFunction
                                );

QTJAMBI_EXPORT jobject qtjambi_from_QSet_shared_pointer(JNIEnv *__jni_env,
                                     void* listPtr,
                                     PointerDeleter sharedPointerDeleter,
                                     PointerGetter sharedPointerGetter,
                                     const std::type_info& typeId,
                                     QSetBeginFunction beginFunction,
                                     QSetCapacityFunction capacityFunction,
                                     QSetClearFunction clearFunction,
                                     QSetContainsFunction containsFunction,
                                     QSetEndFunction endFunction,
                                     QSetInsertFunction insertFunction,
                                     QSetIntersectFunction intersectFunction,
                                     QSetIntersectsFunction intersectsFunction,
                                     QSetEqualFunction equalFunction,
                                     QSetRemoveFunction removeFunction,
                                     QSetReserveFunction reserveFunction,
                                     QSetSizeFunction sizeFunction,
                                     QSetSubtractFunction subtractFunction,
                                     QSetUniteFunction uniteFunction,
                                     QSetValuesFunction valuesFunction
                                );

QTJAMBI_EXPORT jobject qtjambi_from_constQLinkedList_shared_pointer(JNIEnv *__jni_env,
                                     void* listPtr,
                                     PointerDeleter sharedPointerDeleter,
                                     PointerGetter sharedPointerGetter,
                                     const std::type_info& typeId,
                                     QLinkedListBeginFunction beginFunction,
                                     QLinkedListContainsFunction containsFunction,
                                     QLinkedListCountObjectFunction countObjectFunction,
                                     QLinkedListEndFunction endFunction,
                                     QLinkedListEndsWithFunction endsWithFunction,
                                     QLinkedListFirstFunction firstFunction,
                                     QLinkedListLastFunction lastFunction,
                                     QLinkedListEqualFunction equalFunction,
                                     QLinkedListSizeFunction sizeFunction,
                                     QLinkedListStartsWithFunction startsWithFunction
                                );

QTJAMBI_EXPORT jobject qtjambi_from_QLinkedList_shared_pointer(JNIEnv *__jni_env,
                                     void* listPtr,
                                     PointerDeleter sharedPointerDeleter,
                                     PointerGetter sharedPointerGetter,
                                     const std::type_info& typeId,
                                     QLinkedListAppendFunction appendFunction,
                                     QLinkedListBeginFunction beginFunction,
                                     QLinkedListClearFunction clearFunction,
                                     QLinkedListContainsFunction containsFunction,
                                     QLinkedListCountObjectFunction countObjectFunction,
                                     QLinkedListEndFunction endFunction,
                                     QLinkedListEndsWithFunction endsWithFunction,
                                     QLinkedListFirstFunction firstFunction,
                                     QLinkedListLastFunction lastFunction,
                                     QLinkedListEqualFunction equalFunction,
                                     QLinkedListPrependFunction prependFunction,
                                     QLinkedListRemoveAllFunction removeAllFunction,
                                     QLinkedListRemoveFirstFunction removeFirstFunction,
                                     QLinkedListRemoveLastFunction removeLastFunction,
                                     QLinkedListRemoveOneFunction removeOneFunction,
                                     QLinkedListSizeFunction sizeFunction,
                                     QLinkedListStartsWithFunction startsWithFunction,
                                     QLinkedListTakeFirstFunction takeFirstFunction,
                                     QLinkedListTakeLastFunction takeLastFunction
                                );

QTJAMBI_EXPORT jobject qtjambi_from_constQVector_shared_pointer(JNIEnv *__jni_env,
                                     void* listPtr,
                                     PointerDeleter sharedPointerDeleter,
                                     PointerGetter sharedPointerGetter,
                                     const std::type_info& typeId,
                                     QVectorAtFunction atFunction,
                                     QVectorBeginFunction beginFunction,
                                     QVectorCapacityFunction capacityFunction,
                                     QVectorContainsFunction containsFunction,
                                     QVectorCountObjectFunction countObjectFunction,
                                     QVectorEndFunction endFunction,
                                     QVectorEndsWithFunction endsWithFunction,
                                     QVectorIndexOfFunction indexOfFunction,
                                     QVectorLastIndexOfFunction lastIndexOfFunction,
                                     QVectorMidFunction midFunction,
                                     QVectorEqualFunction equalFunction,
                                     QVectorSizeFunction sizeFunction,
                                     QVectorStartsWithFunction startsWithFunction,
                                     QVectorToListFunction toListFunction,
                                     QVectorValueFunction valueFunction,
                                     QVectorValueDefaultFunction valueDefaultFunction
                                );



QTJAMBI_EXPORT jobject qtjambi_from_QVector_shared_pointer(JNIEnv *__jni_env,
                                     void* listPtr,
                                     PointerDeleter sharedPointerDeleter,
                                     PointerGetter sharedPointerGetter,
                                     const std::type_info& typeId,
                                     QVectorAppendFunction appendFunction,
                                     QVectorAppendVectorFunction appendListFunction,
                                     QVectorAtFunction atFunction,
                                     QVectorBeginFunction beginFunction,
                                     QVectorCapacityFunction capacityFunction,
                                     QVectorClearFunction clearFunction,
                                     QVectorContainsFunction containsFunction,
                                     QVectorCountObjectFunction countObjectFunction,
                                     QVectorEndFunction endFunction,
                                     QVectorEndsWithFunction endsWithFunction,
                                     QVectorFillFunction fillFunction,
                                     QVectorIndexOfFunction indexOfFunction,
                                     QVectorInsertFunction insertFunction,
                                     QVectorInsertNFunction insertNFunction,
                                     QVectorLastIndexOfFunction lastIndexOfFunction,
                                     QVectorMidFunction midFunction,
                                     QVectorMoveFunction moveFunction,
                                     QVectorEqualFunction equalFunction,
                                     QVectorPrependFunction prependFunction,
                                     QVectorRemoveAllFunction removeAllFunction,
                                     QVectorRemoveAtFunction removeAtFunction,
                                     QVectorRemoveNFunction removeNFunction,
                                     QVectorRemoveOneFunction removeOneFunction,
                                     QVectorReplaceFunction replaceFunction,
                                     QVectorReserveFunction reserveFunction,
                                     QVectorResizeFunction resizeFunction,
                                     QVectorShrinkToFitFunction shrinkToFitFunction,
                                     QVectorSizeFunction sizeFunction,
                                     QVectorStartsWithFunction startsWithFunction,
                                     QVectorTakeAtFunction takeAtFunction,
                                     QVectorToListFunction toListFunction,
                                     QVectorValueFunction valueFunction,
                                     QVectorValueDefaultFunction valueDefaultFunction
                                );



QTJAMBI_EXPORT jobject qtjambi_from_constQStack_shared_pointer(JNIEnv *__jni_env,
                                     void* listPtr,
                                     PointerDeleter sharedPointerDeleter,
                                     PointerGetter sharedPointerGetter,
                                     const std::type_info& typeId,
                                     QVectorAtFunction atFunction,
                                     QVectorBeginFunction beginFunction,
                                     QVectorCapacityFunction capacityFunction,
                                     QVectorContainsFunction containsFunction,
                                     QVectorCountObjectFunction countObjectFunction,
                                     QVectorEndFunction endFunction,
                                     QVectorEndsWithFunction endsWithFunction,
                                     QVectorIndexOfFunction indexOfFunction,
                                     QVectorLastIndexOfFunction lastIndexOfFunction,
                                     QVectorMidFunction midFunction,
                                     QVectorEqualFunction equalFunction,
                                     QVectorSizeFunction sizeFunction,
                                     QVectorStartsWithFunction startsWithFunction,
                                     QVectorToListFunction toListFunction,
                                     QVectorValueFunction valueFunction,
                                     QVectorValueDefaultFunction valueDefaultFunction
                                );



QTJAMBI_EXPORT jobject qtjambi_from_QStack_shared_pointer(JNIEnv *__jni_env,
                                     void* listPtr,
                                     PointerDeleter sharedPointerDeleter,
                                     PointerGetter sharedPointerGetter,
                                     const std::type_info& typeId,
                                     QVectorAppendFunction appendFunction,
                                     QVectorAppendVectorFunction appendListFunction,
                                     QVectorAtFunction atFunction,
                                     QVectorBeginFunction beginFunction,
                                     QVectorCapacityFunction capacityFunction,
                                     QVectorClearFunction clearFunction,
                                     QVectorContainsFunction containsFunction,
                                     QVectorCountObjectFunction countObjectFunction,
                                     QVectorEndFunction endFunction,
                                     QVectorEndsWithFunction endsWithFunction,
                                     QVectorFillFunction fillFunction,
                                     QVectorIndexOfFunction indexOfFunction,
                                     QVectorInsertFunction insertFunction,
                                     QVectorInsertNFunction insertNFunction,
                                     QVectorLastIndexOfFunction lastIndexOfFunction,
                                     QVectorMidFunction midFunction,
                                     QVectorMoveFunction moveFunction,
                                     QVectorEqualFunction equalFunction,
                                     QVectorPrependFunction prependFunction,
                                     QVectorRemoveAllFunction removeAllFunction,
                                     QVectorRemoveAtFunction removeAtFunction,
                                     QVectorRemoveNFunction removeNFunction,
                                     QVectorRemoveOneFunction removeOneFunction,
                                     QVectorReplaceFunction replaceFunction,
                                     QVectorReserveFunction reserveFunction,
                                     QVectorResizeFunction resizeFunction,
                                     QVectorShrinkToFitFunction shrinkToFitFunction,
                                     QVectorSizeFunction sizeFunction,
                                     QVectorStartsWithFunction startsWithFunction,
                                     QVectorTakeAtFunction takeAtFunction,
                                     QVectorToListFunction toListFunction,
                                     QVectorValueFunction valueFunction,
                                     QVectorValueDefaultFunction valueDefaultFunction
                                );

QTJAMBI_EXPORT jobject qtjambi_from_constQHash(JNIEnv *__jni_env,
                                     void* listPtr,
                                     PointerDeleter sharedPointerDeleter,
                                     PointerGetter sharedPointerGetter,
                                     const std::type_info& keyTypeId,
                                     const std::type_info& valueTypeId,
                                     QHashBeginFunction beginFunction,
                                     QHashCapacityFunction capacityFunction,
                                     QHashContainsFunction containsFunction,
                                     QHashCountObjectFunction countObjectFunction,
                                     QHashEndFunction endFunction,
                                     QHashFindFunction findFunction,
                                     QHashKeyFunction keyFunction,
                                     QHashKeysFunction keysFunction,
                                     QHashKeysForValueFunction keysForValueFunction,
                                     QHashEqualFunction equalFunction,
                                     QHashSizeFunction sizeFunction,
                                     QHashUniqueKeysFunction uniqueKeysFunction,
                                     QHashValueFunction valueFunction,
                                     QHashValuesFunction valuesFunction,
                                     QHashValuesKeyFunction valuesKeyFunction
                                );

QTJAMBI_EXPORT jobject qtjambi_from_constQHash_shared_pointer(JNIEnv *env,
                                     void* ptr_shared_pointer,
                                     PointerDeleter sharedPointerDeleter,
                                     PointerGetter sharedPointerGetter,
                                     const char* keyClassName,
                                     const char* valueClassName,
                                     QHashBeginFunction beginFunction,
                                     QHashCapacityFunction capacityFunction,
                                     QHashContainsFunction containsFunction,
                                     QHashCountObjectFunction countObjectFunction,
                                     QHashEndFunction endFunction,
                                     QHashFindFunction findFunction,
                                     QHashKeyFunction keyFunction,
                                     QHashKeysFunction keysFunction,
                                     QHashKeysForValueFunction keysForValueFunction,
                                     QHashEqualFunction equalFunction,
                                     QHashSizeFunction sizeFunction,
                                     QHashUniqueKeysFunction uniqueKeysFunction,
                                     QHashValueFunction valueFunction,
                                     QHashValuesFunction valuesFunction,
                                     QHashValuesKeyFunction valuesKeyFunction
                                );

QTJAMBI_EXPORT jobject qtjambi_from_constQHash_shared_pointer(JNIEnv *env,
                                     void* ptr_shared_pointer,
                                     PointerDeleter sharedPointerDeleter,
                                     PointerGetter sharedPointerGetter,
                                     const std::type_info& keyTypeId,
                                     const std::type_info& valueTypeId,
                                     QHashBeginFunction beginFunction,
                                     QHashCapacityFunction capacityFunction,
                                     QHashContainsFunction containsFunction,
                                     QHashCountObjectFunction countObjectFunction,
                                     QHashEndFunction endFunction,
                                     QHashFindFunction findFunction,
                                     QHashKeyFunction keyFunction,
                                     QHashKeysFunction keysFunction,
                                     QHashKeysForValueFunction keysForValueFunction,
                                     QHashEqualFunction equalFunction,
                                     QHashSizeFunction sizeFunction,
                                     QHashUniqueKeysFunction uniqueKeysFunction,
                                     QHashValueFunction valueFunction,
                                     QHashValuesFunction valuesFunction,
                                     QHashValuesKeyFunction valuesKeyFunction
                                );

QTJAMBI_EXPORT jobject qtjambi_from_QHash_shared_pointer(JNIEnv *__jni_env,
                                     void* listPtr,
                                     PointerDeleter sharedPointerDeleter,
                                     PointerGetter sharedPointerGetter,
                                     const std::type_info& keyTypeId,
                                     const std::type_info& valueTypeId,
                                     QHashBeginFunction beginFunction,
                                     QHashCapacityFunction capacityFunction,
                                     QHashClearFunction clearFunction,
                                     QHashContainsFunction containsFunction,
                                     QHashCountObjectFunction countObjectFunction,
                                     QHashEndFunction endFunction,
                                     QHashFindFunction findFunction,
                                     QHashInsertFunction insertFunction,
                                     QHashKeyFunction keyFunction,
                                     QHashKeysFunction keysFunction,
                                     QHashKeysForValueFunction keysForValueFunction,
                                     QHashEqualFunction equalFunction,
                                     QHashRemoveAllFunction removeAllFunction,
                                     QHashReserveFunction reserveFunction,
                                     QHashSizeFunction sizeFunction,
                                     QHashTakeFunction takeFunction,
                                     QHashUniqueKeysFunction uniqueKeysFunction,
                                     QHashUniteFunction uniteFunction,
                                     QHashValueFunction valueFunction,
                                     QHashValuesFunction valuesFunction,
                                     QHashValuesKeyFunction valuesKeyFunction
                                );

QTJAMBI_EXPORT jobject qtjambi_from_constQMultiHash_shared_pointer(JNIEnv *__jni_env,
                                     void* listPtr,
                                     PointerDeleter sharedPointerDeleter,
                                     PointerGetter sharedPointerGetter,
                                     const std::type_info& keyTypeId,
                                     const std::type_info& valueTypeId,
                                     QHashBeginFunction beginFunction,
                                     QHashCapacityFunction capacityFunction,
                                     QHashContainsFunction containsFunction,
                                     QHashCountObjectFunction countObjectFunction,
                                     QHashEndFunction endFunction,
                                     QHashFindFunction findFunction,
                                     QHashKeyFunction keyFunction,
                                     QHashKeysFunction keysFunction,
                                     QHashKeysForValueFunction keysForValueFunction,
                                     QHashEqualFunction equalFunction,
                                     QHashSizeFunction sizeFunction,
                                     QHashUniqueKeysFunction uniqueKeysFunction,
                                     QHashValueFunction valueFunction,
                                     QHashValuesFunction valuesFunction,
                                     QHashValuesKeyFunction valuesKeyFunction,
                                     QMultiHashContainsPairFunction containsPairFunction,
                                     QMultiHashCountPairFunction countPairFunction,
                                     QMultiHashFindPairFunction findPairFunction
                                );

QTJAMBI_EXPORT jobject qtjambi_from_QMultiHash_shared_pointer(JNIEnv *__jni_env,
                                     void* listPtr,
                                     PointerDeleter sharedPointerDeleter,
                                     PointerGetter sharedPointerGetter,
                                     const std::type_info& keyTypeId,
                                     const std::type_info& valueTypeId,
                                     QHashBeginFunction beginFunction,
                                     QHashCapacityFunction capacityFunction,
                                     QHashClearFunction clearFunction,
                                     QHashContainsFunction containsFunction,
                                     QHashCountObjectFunction countObjectFunction,
                                     QHashEndFunction endFunction,
                                     QHashFindFunction findFunction,
                                     QHashInsertFunction insertFunction,
                                     QHashKeyFunction keyFunction,
                                     QHashKeysFunction keysFunction,
                                     QHashKeysForValueFunction keysForValueFunction,
                                     QHashEqualFunction equalFunction,
                                     QHashRemoveAllFunction removeAllFunction,
                                     QHashReserveFunction reserveFunction,
                                     QHashSizeFunction sizeFunction,
                                     QHashTakeFunction takeFunction,
                                     QHashUniqueKeysFunction uniqueKeysFunction,
                                     QHashUniteFunction uniteFunction,
                                     QHashValueFunction valueFunction,
                                     QHashValuesFunction valuesFunction,
                                     QHashValuesKeyFunction valuesKeyFunction,
                                     QMultiHashContainsPairFunction containsPairFunction,
                                     QMultiHashCountPairFunction countPairFunction,
                                     QMultiHashFindPairFunction findPairFunction,
                                     QMultiHashRemovePairFunction removeAllPairFunction,
                                     QMultiHashReplaceFunction replaceOneFunction
                                );

QTJAMBI_EXPORT jobject qtjambi_from_constQMap_shared_pointer(JNIEnv *__jni_env,
                                     void* listPtr,
                                     PointerDeleter sharedPointerDeleter,
                                     PointerGetter sharedPointerGetter,
                                     const std::type_info& keyTypeId,
                                     const std::type_info& valueTypeId,
                                     QMapBeginFunction beginFunction,
                                     QMapContainsFunction containsFunction,
                                     QMapCountObjectFunction countObjectFunction,
                                     QMapEndFunction endFunction,
                                     QMapFindFunction findFunction,
                                     QMapFirstFunction firstFunction,
                                     QMapFirstKeyFunction firstKeyFunction,
                                     QMapKeyFunction keyFunction,
                                     QMapKeysFunction keysFunction,
                                     QMapKeysForValueFunction keysForValueFunction,
                                     QMapLastFunction lastFunction,
                                     QMapLastKeyFunction lastKeyFunction,
                                     QMapLowerBoundFunction lowerBoundFunction,
                                     QMapEqualFunction equalFunction,
                                     QMapSizeFunction sizeFunction,
                                     QMapUniqueKeysFunction uniqueKeysFunction,
                                     QMapUpperBoundFunction upperBoundFunction,
                                     QMapValueFunction valueFunction,
                                     QMapValuesFunction valuesFunction,
                                     QMapValuesKeyFunction valuesKeyFunction
                                );

QTJAMBI_EXPORT jobject qtjambi_from_QMap_shared_pointer(JNIEnv *__jni_env,
                                     void* listPtr,
                                     PointerDeleter sharedPointerDeleter,
                                     PointerGetter sharedPointerGetter,
                                     const std::type_info& keyTypeId,
                                     const std::type_info& valueTypeId,
                                     QMapBeginFunction beginFunction,
                                     QMapClearFunction clearFunction,
                                     QMapContainsFunction containsFunction,
                                     QMapCountObjectFunction countObjectFunction,
                                     QMapEndFunction endFunction,
                                     QMapFindFunction findFunction,
                                     QMapFirstFunction firstFunction,
                                     QMapFirstKeyFunction firstKeyFunction,
                                     QMapInsertFunction insertFunction,
                                     QMapKeyFunction keyFunction,
                                     QMapKeysFunction keysFunction,
                                     QMapKeysForValueFunction keysForValueFunction,
                                     QMapLastFunction lastFunction,
                                     QMapLastKeyFunction lastKeyFunction,
                                     QMapLowerBoundFunction lowerBoundFunction,
                                     QMapEqualFunction equalFunction,
                                     QMapRemoveAllFunction removeAllFunction,
                                     QMapSizeFunction sizeFunction,
                                     QMapTakeFunction takeFunction,
                                     QMapUniqueKeysFunction uniqueKeysFunction,
                                     QMapUniteFunction uniteFunction,
                                     QMapUpperBoundFunction upperBoundFunction,
                                     QMapValueFunction valueFunction,
                                     QMapValuesFunction valuesFunction,
                                     QMapValuesKeyFunction valuesKeyFunction
                                );

QTJAMBI_EXPORT jobject qtjambi_from_constQMultiMap_shared_pointer(JNIEnv *__jni_env,
                                     void* listPtr,
                                     PointerDeleter sharedPointerDeleter,
                                     PointerGetter sharedPointerGetter,
                                     const std::type_info& keyTypeId,
                                     const std::type_info& valueTypeId,
                                     QMapBeginFunction beginFunction,
                                     QMapContainsFunction containsFunction,
                                     QMapCountObjectFunction countObjectFunction,
                                     QMapEndFunction endFunction,
                                     QMapFindFunction findFunction,
                                     QMapFirstFunction firstFunction,
                                     QMapFirstKeyFunction firstKeyFunction,
                                     QMapKeyFunction keyFunction,
                                     QMapKeysFunction keysFunction,
                                     QMapKeysForValueFunction keysForValueFunction,
                                     QMapLastFunction lastFunction,
                                     QMapLastKeyFunction lastKeyFunction,
                                     QMapLowerBoundFunction lowerBoundFunction,
                                     QMapEqualFunction equalFunction,
                                     QMapSizeFunction sizeFunction,
                                     QMapUniqueKeysFunction uniqueKeysFunction,
                                     QMapUpperBoundFunction upperBoundFunction,
                                     QMapValueFunction valueFunction,
                                     QMapValuesFunction valuesFunction,
                                     QMapValuesKeyFunction valuesKeyFunction,
                                     QMultiMapContainsPairFunction containsPairFunction,
                                     QMultiMapCountPairFunction countPairFunction,
                                     QMultiMapFindPairFunction findPairFunction
                                );

QTJAMBI_EXPORT jobject qtjambi_from_QMultiMap_shared_pointer(JNIEnv *__jni_env,
                                     void* listPtr,
                                     PointerDeleter sharedPointerDeleter,
                                     PointerGetter sharedPointerGetter,
                                     const std::type_info& keyTypeId,
                                     const std::type_info& valueTypeId,
                                     QMapBeginFunction beginFunction,
                                     QMapClearFunction clearFunction,
                                     QMapContainsFunction containsFunction,
                                     QMapCountObjectFunction countObjectFunction,
                                     QMapEndFunction endFunction,
                                     QMapFindFunction findFunction,
                                     QMapFirstFunction firstFunction,
                                     QMapFirstKeyFunction firstKeyFunction,
                                     QMapInsertFunction insertFunction,
                                     QMapKeyFunction keyFunction,
                                     QMapKeysFunction keysFunction,
                                     QMapKeysForValueFunction keysForValueFunction,
                                     QMapLastFunction lastFunction,
                                     QMapLastKeyFunction lastKeyFunction,
                                     QMapLowerBoundFunction lowerBoundFunction,
                                     QMapEqualFunction equalFunction,
                                     QMapRemoveAllFunction removeAllFunction,
                                     QMapSizeFunction sizeFunction,
                                     QMapTakeFunction takeFunction,
                                     QMapUniqueKeysFunction uniqueKeysFunction,
                                     QMapUniteFunction uniteFunction,
                                     QMapUpperBoundFunction upperBoundFunction,
                                     QMapValueFunction valueFunction,
                                     QMapValuesFunction valuesFunction,
                                     QMapValuesKeyFunction valuesKeyFunction,
                                     QMultiMapContainsPairFunction containsPairFunction,
                                     QMultiMapCountPairFunction countPairFunction,
                                     QMultiMapFindPairFunction findPairFunction,
                                     QMultiMapRemovePairFunction removeAllPairFunction,
                                     QMultiMapReplaceFunction replaceOneFunction
                                );

#endif // QTJAMBI_CONTAINERS_H
