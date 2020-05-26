/****************************************************************************
**
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

#include "qtjambi_repository_p.h"
#include "qtjambi_containers.h"
#include "qtjambi_cast.h"
#include "qtjambi_jobjectwrapper.h"
#include "qtjambi_registry_p.h"
#include "qtjambifunctiontable_p.h"
#include "qtjambilink_p.h"
#include "qtjambitypemanager_p.h"
#include "qtdynamicmetaobject_p.h"
#include "qtjambivariant_p.h"
#include "qtjambi_thread.h"
#include "qtjambi_interfaces.h"
#include "qtjambi_functionpointer.h"
#include "qtjambi_application.h"

#include <qglobal.h>
#include <cstring>

#include <QtCore/QAbstractItemModel>
#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QEvent>
#include <QtCore/QFileInfo>
#include <QtCore/QLibrary>
#include <QtCore/QMetaMethod>
#include <QtCore/QMetaObject>
#include <QtCore/QSettings>
#include <QtCore/QStringList>
#include <QtCore/QThread>
#include <QtCore/private/qthread_p.h>
#include <QtCore/QVariant>
#include <QtCore/QTextStream>
#include <QtCore/QMutex>
#include <QtCore/QAbstractEventDispatcher>
#include <QtWidgets/QWidget>
#include "qtjambi_templates.h"
#if QT_VERSION < 0x050000
#include <QtGui/QStyleOption>
#endif
#include <stdio.h>
#include <string.h>
#ifdef Q_OS_WIN
#include <Windows.h>
#endif

jobject qtjambi_to_constQList(JNIEnv *env,
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
                             QListValueDefaultFunction valueDefaultFunction)
{

    if(qtjambi_from_nativeId(owner)==listPtr){
        jobject obj = qtjambi_java_object_reference(owner, env);
        if(obj && Java::Private::QtCore::QList.isInstanceOf(env, obj)){
            return obj;
        }
    }
    jclass clazz = resolveClass(env, className);
    jobject returned = nullptr;
    if (clazz) {

        returned = Java::Private::QtCore::QList.newInstance2(env,
                                  clazz,
                                  jlong(atFunction),
                                  jlong(beginFunction),
                                  jlong(containsFunction),
                                  jlong(countObjectFunction),
                                  jlong(endFunction),
                                  jlong(endsWithFunction),
                                  jlong(indexOfFunction),
                                  jlong(lastIndexOfFunction),
                                  jlong(midFunction),
                                  jlong(equalFunction),
                                  jlong(sizeFunction),
                                  jlong(startsWithFunction),
                                  jlong(toSetFunction),
                                  jlong(valueFunction),
                                  jlong(valueDefaultFunction));
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForOwnedObject(env, returned, const_cast<void*>(listPtr),
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                                            "QList",
#endif
                                                                                            owner, nullptr);
        if (!link) {
            returned = nullptr;
        }
    }
    return returned;
}

jobject qtjambi_to_QList(JNIEnv *env,
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
                             QListValueDefaultFunction valueDefaultFunction)
{

    if(qtjambi_from_nativeId(owner)==listPtr){
        jobject obj = qtjambi_java_object_reference(owner, env);
        if(obj && Java::Private::QtCore::QList.isInstanceOf(env, obj)){
            return obj;
        }
    }
    jclass clazz = resolveClass(env, className);
    jobject returned = nullptr;
    if (clazz) {

        returned = Java::Private::QtCore::QList.newInstance(env,
                                  clazz,
                                  jlong(appendFunction),
                                  jlong(appendListFunction),
                                  jlong(atFunction),
                                  jlong(beginFunction),
                                  jlong(clearFunction),
                                  jlong(containsFunction),
                                  jlong(countObjectFunction),
                                  jlong(endFunction),
                                  jlong(endsWithFunction),
                                  jlong(indexOfFunction),
                                  jlong(insertFunction),
                                  jlong(lastIndexOfFunction),
                                  jlong(midFunction),
                                  jlong(moveFunction),
                                  jlong(equalFunction),
                                  jlong(prependFunction),
                                  jlong(removeAllFunction),
                                  jlong(removeAtFunction),
                                  jlong(removeOneFunction),
                                  jlong(replaceFunction),
                                  jlong(reserveFunction),
                                  jlong(sizeFunction),
                                  jlong(startsWithFunction),
                                  jlong(swapFunction),
                                  jlong(takeAtFunction),
                                  jlong(toSetFunction),
                                  jlong(valueFunction),
                                  jlong(valueDefaultFunction));
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForOwnedObject(env, returned, listPtr,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                                            "QList",
#endif
                                                                                            owner, nullptr);
        if (!link) {
            returned = nullptr;
        }
    }
    return returned;
}

jobject qtjambi_to_constQQueue(JNIEnv *env,
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
                             QListValueDefaultFunction valueDefaultFunction)
{

    if(qtjambi_from_nativeId(owner)==listPtr){
        jobject obj = qtjambi_java_object_reference(owner, env);
        if(obj && Java::Private::QtCore::QQueue.isInstanceOf(env, obj)){
            return obj;
        }
    }
    jclass clazz = resolveClass(env, className);
    jobject returned = nullptr;
    if (clazz) {
        returned = Java::Private::QtCore::QQueue.newInstance2(env,
                                  clazz,
                                  jlong(atFunction),
                                  jlong(beginFunction),
                                  jlong(containsFunction),
                                  jlong(countObjectFunction),
                                  jlong(endFunction),
                                  jlong(endsWithFunction),
                                  jlong(indexOfFunction),
                                  jlong(lastIndexOfFunction),
                                  jlong(midFunction),
                                  jlong(equalFunction),
                                  jlong(sizeFunction),
                                  jlong(startsWithFunction),
                                  jlong(toSetFunction),
                                  jlong(valueFunction),
                                  jlong(valueDefaultFunction));
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForOwnedObject(env, returned, const_cast<void*>(listPtr),
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                                            "QQueue",
#endif
                                                                                            owner, nullptr);
        if (!link) {
            returned = nullptr;
        }
    }
    return returned;
}


jobject qtjambi_to_QQueue(JNIEnv *env,
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
                             QListValueDefaultFunction valueDefaultFunction)
{

    if(qtjambi_from_nativeId(owner)==listPtr){
        jobject obj = qtjambi_java_object_reference(owner, env);
        if(obj && Java::Private::QtCore::QQueue.isInstanceOf(env, obj)){
            return obj;
        }
    }
    jclass clazz = resolveClass(env, className);
    jobject returned = nullptr;
    if (clazz) {
        returned = Java::Private::QtCore::QQueue.newInstance(env,
                                  clazz,
                                  jlong(appendFunction),
                                  jlong(appendListFunction),
                                  jlong(atFunction),
                                  jlong(beginFunction),
                                  jlong(clearFunction),
                                  jlong(containsFunction),
                                  jlong(countObjectFunction),
                                  jlong(endFunction),
                                  jlong(endsWithFunction),
                                  jlong(indexOfFunction),
                                  jlong(insertFunction),
                                  jlong(lastIndexOfFunction),
                                  jlong(midFunction),
                                  jlong(moveFunction),
                                  jlong(equalFunction),
                                  jlong(prependFunction),
                                  jlong(removeAllFunction),
                                  jlong(removeAtFunction),
                                  jlong(removeOneFunction),
                                  jlong(replaceFunction),
                                  jlong(reserveFunction),
                                  jlong(sizeFunction),
                                  jlong(startsWithFunction),
                                  jlong(swapFunction),
                                  jlong(takeAtFunction),
                                  jlong(toSetFunction),
                                  jlong(valueFunction),
                                  jlong(valueDefaultFunction));
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForOwnedObject(env, returned, listPtr,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                                            "QQueue",
#endif
                                                                                            owner, nullptr);
        if (!link) {
            returned = nullptr;
        }
    }
    return returned;
}

jobject qtjambi_to_constQSet(JNIEnv *env,
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
                                )
{

    if(qtjambi_from_nativeId(owner)==listPtr){
        jobject obj = qtjambi_java_object_reference(owner, env);
        if(obj && Java::Private::QtCore::QSet.isInstanceOf(env, obj)){
            return obj;
        }
    }
    jclass clazz = resolveClass(env, className);
    jobject returned = nullptr;
    if (clazz) {
        returned = Java::Private::QtCore::QSet.newInstance(env,
                                  clazz,
                                  jlong(beginFunction),
                                  jlong(capacityFunction),
                                  jlong(containsFunction),
                                  jlong(endFunction),
                                  jlong(intersectsFunction),
                                  jlong(equalFunction),
                                  jlong(sizeFunction),
                                  jlong(valuesFunction));
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForOwnedObject(env, returned, const_cast<void*>(listPtr),
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                                            "QSet",
#endif
                                                                                            owner, nullptr);
        if (!link) {
            returned = nullptr;
        }
    }
    return returned;
}

jobject qtjambi_to_QSet(JNIEnv *env,
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
                                )
{

    if(qtjambi_from_nativeId(owner)==listPtr){
        jobject obj = qtjambi_java_object_reference(owner, env);
        if(obj && Java::Private::QtCore::QSet.isInstanceOf(env, obj)){
            return obj;
        }
    }
    jclass clazz = resolveClass(env, className);
    jobject returned = nullptr;
    if (clazz) {
        returned = Java::Private::QtCore::QSet.newInstance2(env,
                                  clazz,
                                  jlong(beginFunction),
                                  jlong(capacityFunction),
                                  jlong(clearFunction),
                                  jlong(containsFunction),
                                  jlong(endFunction),
                                  jlong(insertFunction),
                                  jlong(intersectFunction),
                                  jlong(intersectsFunction),
                                  jlong(equalFunction),
                                  jlong(removeFunction),
                                  jlong(reserveFunction),
                                  jlong(sizeFunction),
                                  jlong(subtractFunction),
                                  jlong(uniteFunction),
                                  jlong(valuesFunction));
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForOwnedObject(env, returned, listPtr,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                                            "QSet",
#endif
                                                                                            owner, nullptr);
        if (!link) {
            returned = nullptr;
        }
    }
    return returned;
}

jobject qtjambi_to_constQLinkedList(JNIEnv *env,
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
                                )
{

    if(qtjambi_from_nativeId(owner)==listPtr){
        jobject obj = qtjambi_java_object_reference(owner, env);
        if(obj && Java::Private::QtCore::QLinkedList.isInstanceOf(env, obj)){
            return obj;
        }
    }
    jclass clazz = resolveClass(env, className);
    jobject returned = nullptr;
    if (clazz) {
        returned = Java::Private::QtCore::QLinkedList.newInstance2(env,
                                  clazz,
                                  jlong(beginFunction),
                                  jlong(containsFunction),
                                  jlong(countObjectFunction),
                                  jlong(endFunction),
                                  jlong(endsWithFunction),
                                  jlong(firstFunction),
                                  jlong(lastFunction),
                                  jlong(equalFunction),
                                  jlong(sizeFunction),
                                  jlong(startsWithFunction));
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForOwnedObject(env, returned, const_cast<void*>(listPtr),
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                                            "QSet",
#endif
                                                                                            owner, nullptr);
        if (!link) {
            returned = nullptr;
        }
    }
    return returned;
}

jobject qtjambi_to_QLinkedList(JNIEnv *env,
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
                                )
{

    if(qtjambi_from_nativeId(owner)==listPtr){
        jobject obj = qtjambi_java_object_reference(owner, env);
        if(obj && Java::Private::QtCore::QLinkedList.isInstanceOf(env, obj)){
            return obj;
        }
    }
    jclass clazz = resolveClass(env, className);
    jobject returned = nullptr;
    if (clazz) {
        returned = Java::Private::QtCore::QLinkedList.newInstance(env,
                                  clazz,
                                  jlong(appendFunction),
                                  jlong(beginFunction),
                                  jlong(clearFunction),
                                  jlong(containsFunction),
                                  jlong(countObjectFunction),
                                  jlong(endFunction),
                                  jlong(endsWithFunction),
                                  jlong(firstFunction),
                                  jlong(lastFunction),
                                  jlong(equalFunction),
                                  jlong(prependFunction),
                                  jlong(removeAllFunction),
                                  jlong(removeFirstFunction),
                                  jlong(removeLastFunction),
                                  jlong(removeOneFunction),
                                  jlong(sizeFunction),
                                  jlong(startsWithFunction),
                                  jlong(takeFirstFunction),
                                  jlong(takeLastFunction));
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForOwnedObject(env, returned, listPtr,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                                            "QLinkedList",
#endif
                                                                                            owner, nullptr);
        if (!link) {
            returned = nullptr;
        }
    }
    return returned;
}

jobject qtjambi_to_constQVector(JNIEnv *env,
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
                                )
{

    if(qtjambi_from_nativeId(owner)==listPtr){
        jobject obj = qtjambi_java_object_reference(owner, env);
        if(obj && Java::Private::QtCore::QVector.isInstanceOf(env, obj)){
            return obj;
        }
    }
    jclass clazz = resolveClass(env, className);
    jobject returned = nullptr;
    if (clazz) {
        returned = Java::Private::QtCore::QVector.newInstance2(env,
                                  clazz,
                                  jlong(atFunction),
                                  jlong(beginFunction),
                                  jlong(capacityFunction),
                                  jlong(containsFunction),
                                  jlong(countObjectFunction),
                                  jlong(endFunction),
                                  jlong(endsWithFunction),
                                  jlong(indexOfFunction),
                                  jlong(lastIndexOfFunction),
                                  jlong(midFunction),
                                  jlong(equalFunction),
                                  jlong(sizeFunction),
                                  jlong(startsWithFunction),
                                  jlong(toListFunction),
                                  jlong(valueFunction),
                                  jlong(valueDefaultFunction));
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForOwnedObject(env, returned, const_cast<void*>(listPtr),
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                                            "QVector",
#endif
                                                                                            owner, nullptr);
        if (!link) {
            returned = nullptr;
        }
    }
    return returned;
}

jobject qtjambi_to_QVector(JNIEnv *env,
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
                                )
{

    if(qtjambi_from_nativeId(owner)==listPtr){
        jobject obj = qtjambi_java_object_reference(owner, env);
        if(obj && Java::Private::QtCore::QVector.isInstanceOf(env, obj)){
            return obj;
        }
    }
    jclass clazz = resolveClass(env, className);
    jobject returned = nullptr;
    if (clazz) {
        returned = Java::Private::QtCore::QVector.newInstance(env,
                                  clazz,
                                  jlong(appendFunction),
                                  jlong(appendListFunction),
                                  jlong(atFunction),
                                  jlong(beginFunction),
                                  jlong(capacityFunction),
                                  jlong(clearFunction),
                                  jlong(containsFunction),
                                  jlong(countObjectFunction),
                                  jlong(endFunction),
                                  jlong(endsWithFunction),
                                  jlong(fillFunction),
                                  jlong(indexOfFunction),
                                  jlong(insertFunction),
                                  jlong(insertNFunction),
                                  jlong(lastIndexOfFunction),
                                  jlong(midFunction),
                                  jlong(moveFunction),
                                  jlong(equalFunction),
                                  jlong(prependFunction),
                                  jlong(removeAllFunction),
                                  jlong(removeAtFunction),
                                  jlong(removeNFunction),
                                  jlong(removeOneFunction),
                                  jlong(replaceFunction),
                                  jlong(reserveFunction),
                                  jlong(resizeFunction),
                                  jlong(shrinkToFitFunction),
                                  jlong(sizeFunction),
                                  jlong(startsWithFunction),
                                  jlong(takeAtFunction),
                                  jlong(toListFunction),
                                  jlong(valueFunction),
                                  jlong(valueDefaultFunction));
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForOwnedObject(env, returned, listPtr,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                                            "QVector",
#endif
                                                                                            owner, nullptr);
        if (!link) {
            returned = nullptr;
        }
    }
    return returned;
}

jobject qtjambi_to_constQStack(JNIEnv *env,
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
                                )
{

    if(qtjambi_from_nativeId(owner)==listPtr){
        jobject obj = qtjambi_java_object_reference(owner, env);
        if(obj && Java::Private::QtCore::QStack.isInstanceOf(env, obj)){
            return obj;
        }
    }
    jclass clazz = resolveClass(env, className);

    jobject returned = nullptr;
    if (clazz) {
        returned = Java::Private::QtCore::QStack.newInstance2(env,
                                  clazz,
                                  jlong(atFunction),
                                  jlong(beginFunction),
                                  jlong(capacityFunction),
                                  jlong(containsFunction),
                                  jlong(countObjectFunction),
                                  jlong(endFunction),
                                  jlong(endsWithFunction),
                                  jlong(indexOfFunction),
                                  jlong(lastIndexOfFunction),
                                  jlong(midFunction),
                                  jlong(equalFunction),
                                  jlong(sizeFunction),
                                  jlong(startsWithFunction),
                                  jlong(toListFunction),
                                  jlong(valueFunction),
                                  jlong(valueDefaultFunction));
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForOwnedObject(env, returned, const_cast<void*>(listPtr),
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                                            "QStack",
#endif
                                                                                            owner, nullptr);
        if (!link) {
            returned = nullptr;
        }
    }
    return returned;
}

jobject qtjambi_to_QStack(JNIEnv *env,
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
                                )
{

    if(qtjambi_from_nativeId(owner)==listPtr){
        jobject obj = qtjambi_java_object_reference(owner, env);
        if(obj && Java::Private::QtCore::QStack.isInstanceOf(env, obj)){
            return obj;
        }
    }
    jclass clazz = resolveClass(env, className);

    jobject returned = nullptr;
    if (clazz) {
        returned = Java::Private::QtCore::QStack.newInstance(env,
                                  clazz,
                                  jlong(appendFunction),
                                  jlong(appendListFunction),
                                  jlong(atFunction),
                                  jlong(beginFunction),
                                  jlong(capacityFunction),
                                  jlong(clearFunction),
                                  jlong(containsFunction),
                                  jlong(countObjectFunction),
                                  jlong(endFunction),
                                  jlong(endsWithFunction),
                                  jlong(fillFunction),
                                  jlong(indexOfFunction),
                                  jlong(insertFunction),
                                  jlong(insertNFunction),
                                  jlong(lastIndexOfFunction),
                                  jlong(midFunction),
                                  jlong(moveFunction),
                                  jlong(equalFunction),
                                  jlong(prependFunction),
                                  jlong(removeAllFunction),
                                  jlong(removeAtFunction),
                                  jlong(removeNFunction),
                                  jlong(removeOneFunction),
                                  jlong(replaceFunction),
                                  jlong(reserveFunction),
                                  jlong(resizeFunction),
                                  jlong(shrinkToFitFunction),
                                  jlong(sizeFunction),
                                  jlong(startsWithFunction),
                                  jlong(takeAtFunction),
                                  jlong(toListFunction),
                                  jlong(valueFunction),
                                  jlong(valueDefaultFunction));
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForOwnedObject(env, returned, listPtr,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                                        "QStack",
#endif
                                                                                        owner, nullptr);
        if (!link) {
            returned = nullptr;
        }
    }
    return returned;
}

jobject qtjambi_to_constQHash(JNIEnv *env,
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
                                )
{

    if(qtjambi_from_nativeId(owner)==listPtr){
        jobject obj = qtjambi_java_object_reference(owner, env);
        if(obj && Java::Private::QtCore::QHash.isInstanceOf(env, obj)){
            return obj;
        }
    }
    jclass keyClazz = resolveClass(env, keyClassName);
    jclass valueClazz = resolveClass(env, valueClassName);

    jobject returned = nullptr;
    if (keyClazz && valueClazz) {
        returned = Java::Private::QtCore::QHash.newInstance2(env,
                                  keyClazz,
                                  valueClazz,
                                  jlong(beginFunction),
                                  jlong(capacityFunction),
                                  jlong(containsFunction),
                                  jlong(countObjectFunction),
                                  jlong(endFunction),
                                  jlong(findFunction),
                                  jlong(keyFunction),
                                  jlong(keysFunction),
                                  jlong(keysForValueFunction),
                                  jlong(equalFunction),
                                  jlong(sizeFunction),
                                  jlong(uniqueKeysFunction),
                                  jlong(valueFunction),
                                  jlong(valuesFunction),
                                  jlong(valuesKeyFunction));
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForOwnedObject(env, returned, const_cast<void*>(listPtr),
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                                            "QHash",
#endif
                                                                                            owner, nullptr);
        if (!link) {
            returned = nullptr;
        }
    }
    return returned;
}

jobject qtjambi_to_QHash(JNIEnv *env,
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
                                )
{

    if(qtjambi_from_nativeId(owner)==listPtr){
        jobject obj = qtjambi_java_object_reference(owner, env);
        if(obj && Java::Private::QtCore::QHash.isInstanceOf(env, obj)){
            return obj;
        }
    }
    jclass keyClazz = resolveClass(env, keyClassName);
    jclass valueClazz = resolveClass(env, valueClassName);

    jobject returned = nullptr;
    if (keyClazz && valueClazz) {
        returned = Java::Private::QtCore::QHash.newInstance(env,
                                  keyClazz,
                                  valueClazz,
                                  jlong(beginFunction),
                                  jlong(capacityFunction),
                                  jlong(clearFunction),
                                  jlong(containsFunction),
                                  jlong(countObjectFunction),
                                  jlong(endFunction),
                                  jlong(findFunction),
                                  jlong(insertFunction),
                                  jlong(keyFunction),
                                  jlong(keysFunction),
                                  jlong(keysForValueFunction),
                                  jlong(equalFunction),
                                  jlong(removeAllFunction),
                                  jlong(reserveFunction),
                                  jlong(sizeFunction),
                                  jlong(takeFunction),
                                  jlong(uniqueKeysFunction),
                                  jlong(uniteFunction),
                                  jlong(valueFunction),
                                  jlong(valuesFunction),
                                  jlong(valuesKeyFunction));
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForOwnedObject(env, returned, listPtr,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                                            "QHash",
#endif
                                                                                            owner, nullptr);
        if (!link) {
            returned = nullptr;
        }
    }
    return returned;
}

jobject qtjambi_to_constQMultiHash(JNIEnv *env,
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
                                )
{

    if(qtjambi_from_nativeId(owner)==listPtr){
        jobject obj = qtjambi_java_object_reference(owner, env);
        if(obj && Java::Private::QtCore::QMultiHash.isInstanceOf(env, obj)){
            return obj;
        }
    }
    jclass keyClazz = resolveClass(env, keyClassName);
    jclass valueClazz = resolveClass(env, valueClassName);

    jobject returned = nullptr;
    if (keyClazz && valueClazz) {
        returned = Java::Private::QtCore::QMultiHash.newInstance2(env,
                                  keyClazz,
                                  valueClazz,
                                  jlong(beginFunction),
                                  jlong(capacityFunction),
                                  jlong(containsFunction),
                                  jlong(countObjectFunction),
                                  jlong(endFunction),
                                  jlong(findFunction),
                                  jlong(keyFunction),
                                  jlong(keysFunction),
                                  jlong(keysForValueFunction),
                                  jlong(equalFunction),
                                  jlong(sizeFunction),
                                  jlong(uniqueKeysFunction),
                                  jlong(valueFunction),
                                  jlong(valuesFunction),
                                  jlong(valuesKeyFunction),
                                  jlong(containsPairFunction),
                                  jlong(countPairFunction),
                                  jlong(findPairFunction));
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForOwnedObject(env, returned, const_cast<void*>(listPtr),
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                                            "QMultiHash",
#endif
                                                                                            owner, nullptr);
        if (!link) {
            returned = nullptr;
        }
    }
    return returned;
}

jobject qtjambi_to_QMultiHash(JNIEnv *env,
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
                                )
{

    if(qtjambi_from_nativeId(owner)==listPtr){
        jobject obj = qtjambi_java_object_reference(owner, env);
        if(obj && Java::Private::QtCore::QMultiHash.isInstanceOf(env, obj)){
            return obj;
        }
    }
    jclass keyClazz = resolveClass(env, keyClassName);
    jclass valueClazz = resolveClass(env, valueClassName);

    jobject returned = nullptr;
    if (keyClazz && valueClazz) {
        returned = Java::Private::QtCore::QMultiHash.newInstance(env,
                                  keyClazz,
                                  valueClazz,
                                  jlong(beginFunction),
                                  jlong(capacityFunction),
                                  jlong(clearFunction),
                                  jlong(containsFunction),
                                  jlong(countObjectFunction),
                                  jlong(endFunction),
                                  jlong(findFunction),
                                  jlong(insertFunction),
                                  jlong(keyFunction),
                                  jlong(keysFunction),
                                  jlong(keysForValueFunction),
                                  jlong(equalFunction),
                                  jlong(removeAllFunction),
                                  jlong(reserveFunction),
                                  jlong(sizeFunction),
                                  jlong(takeFunction),
                                  jlong(uniqueKeysFunction),
                                  jlong(uniteFunction),
                                  jlong(valueFunction),
                                  jlong(valuesFunction),
                                  jlong(valuesKeyFunction),
                                  jlong(containsPairFunction),
                                  jlong(countPairFunction),
                                  jlong(findPairFunction),
                                  jlong(removeAllPairFunction),
                                  jlong(replaceOneFunction));
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForOwnedObject(env, returned, listPtr,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                                            "QMultiHash",
#endif
                                                                                            owner, nullptr);
        if (!link) {
            returned = nullptr;
        }
    }
    return returned;
}

jobject qtjambi_to_constQMap(JNIEnv *env,
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
                                )
{

    if(qtjambi_from_nativeId(owner)==listPtr){
        jobject obj = qtjambi_java_object_reference(owner, env);
        if(obj && Java::Private::QtCore::QMap.isInstanceOf(env, obj)){
            return obj;
        }
    }
    jclass keyClazz = resolveClass(env, keyClassName);
    jclass valueClazz = resolveClass(env, valueClassName);

    jobject returned = nullptr;
    if (keyClazz && valueClazz) {
        returned = Java::Private::QtCore::QMap.newInstance2(env,
                                  keyClazz,
                                  valueClazz,
                                  jlong(beginFunction),
                                  jlong(containsFunction),
                                  jlong(countObjectFunction),
                                  jlong(endFunction),
                                  jlong(findFunction),
                                  jlong(firstFunction),
                                  jlong(firstKeyFunction),
                                  jlong(keyFunction),
                                  jlong(keysFunction),
                                  jlong(keysForValueFunction),
                                  jlong(lastFunction),
                                  jlong(lastKeyFunction),
                                  jlong(lowerBoundFunction),
                                  jlong(equalFunction),
                                  jlong(sizeFunction),
                                  jlong(uniqueKeysFunction),
                                  jlong(upperBoundFunction),
                                  jlong(valueFunction),
                                  jlong(valuesFunction),
                                  jlong(valuesKeyFunction));
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForOwnedObject(env, returned, const_cast<void*>(listPtr),
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                                            "QMap",
#endif
                                                                                            owner, nullptr);
        if (!link) {
            returned = nullptr;
        }
    }
    return returned;
}

jobject qtjambi_to_QMap(JNIEnv *env,
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
                                )
{

    if(qtjambi_from_nativeId(owner)==listPtr){
        jobject obj = qtjambi_java_object_reference(owner, env);
        if(obj && Java::Private::QtCore::QMap.isInstanceOf(env, obj)){
            return obj;
        }
    }
    jclass keyClazz = resolveClass(env, keyClassName);
    jclass valueClazz = resolveClass(env, valueClassName);

    jobject returned = nullptr;
    if (keyClazz && valueClazz) {
        returned = Java::Private::QtCore::QMap.newInstance(env,
                                  keyClazz,
                                  valueClazz,
                                  jlong(beginFunction),
                                  jlong(clearFunction),
                                  jlong(containsFunction),
                                  jlong(countObjectFunction),
                                  jlong(endFunction),
                                  jlong(findFunction),
                                  jlong(firstFunction),
                                  jlong(firstKeyFunction),
                                  jlong(insertFunction),
                                  jlong(keyFunction),
                                  jlong(keysFunction),
                                  jlong(keysForValueFunction),
                                  jlong(lastFunction),
                                  jlong(lastKeyFunction),
                                  jlong(lowerBoundFunction),
                                  jlong(equalFunction),
                                  jlong(removeAllFunction),
                                  jlong(sizeFunction),
                                  jlong(takeFunction),
                                  jlong(uniqueKeysFunction),
                                  jlong(uniteFunction),
                                  jlong(upperBoundFunction),
                                  jlong(valueFunction),
                                  jlong(valuesFunction),
                                  jlong(valuesKeyFunction));
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForOwnedObject(env, returned, listPtr,
    #if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                                                "QMap",
    #endif
                                                                                                owner, nullptr);
        if (!link) {
            returned = nullptr;
        }
    }
    return returned;
}

jobject qtjambi_to_constQMultiMap(JNIEnv *env,
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
                                )
{

    if(qtjambi_from_nativeId(owner)==listPtr){
        jobject obj = qtjambi_java_object_reference(owner, env);
        if(obj && Java::Private::QtCore::QMultiMap.isInstanceOf(env, obj)){
            return obj;
        }
    }
    jclass keyClazz = resolveClass(env, keyClassName);
    jclass valueClazz = resolveClass(env, valueClassName);

    jobject returned = nullptr;
    if (keyClazz && valueClazz) {
        returned = Java::Private::QtCore::QMultiMap.newInstance2(env,
                                  keyClazz,
                                  valueClazz,
                                  jlong(beginFunction),
                                  jlong(containsFunction),
                                  jlong(countObjectFunction),
                                  jlong(endFunction),
                                  jlong(findFunction),
                                  jlong(firstFunction),
                                  jlong(firstKeyFunction),
                                  jlong(keyFunction),
                                  jlong(keysFunction),
                                  jlong(keysForValueFunction),
                                  jlong(lastFunction),
                                  jlong(lastKeyFunction),
                                  jlong(lowerBoundFunction),
                                  jlong(equalFunction),
                                  jlong(sizeFunction),
                                  jlong(uniqueKeysFunction),
                                  jlong(upperBoundFunction),
                                  jlong(valueFunction),
                                  jlong(valuesFunction),
                                  jlong(valuesKeyFunction),
                                  jlong(containsPairFunction),
                                  jlong(countPairFunction),
                                  jlong(findPairFunction));
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForOwnedObject(env, returned, const_cast<void*>(listPtr),
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                                            "QMultiMap",
#endif
                                                                                            owner, nullptr);
        if (!link) {
            returned = nullptr;
        }
    }
    return returned;
}

jobject qtjambi_to_QMultiMap(JNIEnv *env,
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
                                )
{

    if(qtjambi_from_nativeId(owner)==listPtr){
        jobject obj = qtjambi_java_object_reference(owner, env);
        if(obj && Java::Private::QtCore::QMultiMap.isInstanceOf(env, obj)){
            return obj;
        }
    }
    jclass keyClazz = resolveClass(env, keyClassName);
    jclass valueClazz = resolveClass(env, valueClassName);

    jobject returned = nullptr;
    if (keyClazz && valueClazz) {
        returned = Java::Private::QtCore::QMultiMap.newInstance(env,
                                  keyClazz,
                                  valueClazz,
                                  jlong(beginFunction),
                                  jlong(clearFunction),
                                  jlong(containsFunction),
                                  jlong(countObjectFunction),
                                  jlong(endFunction),
                                  jlong(findFunction),
                                  jlong(firstFunction),
                                  jlong(firstKeyFunction),
                                  jlong(insertFunction),
                                  jlong(keyFunction),
                                  jlong(keysFunction),
                                  jlong(keysForValueFunction),
                                  jlong(lastFunction),
                                  jlong(lastKeyFunction),
                                  jlong(lowerBoundFunction),
                                  jlong(equalFunction),
                                  jlong(removeAllFunction),
                                  jlong(sizeFunction),
                                  jlong(takeFunction),
                                  jlong(uniqueKeysFunction),
                                  jlong(uniteFunction),
                                  jlong(upperBoundFunction),
                                  jlong(valueFunction),
                                  jlong(valuesFunction),
                                  jlong(valuesKeyFunction),
                                  jlong(containsPairFunction),
                                  jlong(countPairFunction),
                                  jlong(findPairFunction),
                                  jlong(removeAllPairFunction),
                                  jlong(replaceOneFunction));
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForOwnedObject(env, returned, listPtr,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                                            "QMultiMap",
#endif
                                                                                            owner, nullptr);
        if (!link) {
            returned = nullptr;
        }
    }
    return returned;
}

jobject qtjambi_from_constQList_shared_pointer(JNIEnv *env,
                             void* ptr_shared_pointer,
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
                             QListValueDefaultFunction valueDefaultFunction)
{
    Q_ASSERT(sharedPointerGetter);
    Q_ASSERT(sharedPointerDeleter);
    if (!ptr_shared_pointer || !sharedPointerDeleter || !sharedPointerGetter)
        return nullptr;
    void* listPtr = sharedPointerGetter(ptr_shared_pointer);
    if (!listPtr)
        return nullptr;


    jclass clazz = resolveClass(env, className);

    jobject returned = nullptr;
    if (clazz) {
        returned = Java::Private::QtCore::QList.newInstance2(env,
                                  clazz,
                                  jlong(atFunction),
                                  jlong(beginFunction),
                                  jlong(containsFunction),
                                  jlong(countObjectFunction),
                                  jlong(endFunction),
                                  jlong(endsWithFunction),
                                  jlong(indexOfFunction),
                                  jlong(lastIndexOfFunction),
                                  jlong(midFunction),
                                  jlong(equalFunction),
                                  jlong(sizeFunction),
                                  jlong(startsWithFunction),
                                  jlong(toSetFunction),
                                  jlong(valueFunction),
                                  jlong(valueDefaultFunction));
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForSharedPointerToObject(env, returned, QMetaType::UnknownType,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                                          "QList",
#endif
                                                                                          false, false, nullptr, nullptr, ptr_shared_pointer, sharedPointerDeleter, sharedPointerGetter);
        if (!link) {
            returned = nullptr;
        }
    }
    return returned;
}


jobject qtjambi_from_QList_shared_pointer(JNIEnv *env,
                             void* ptr_shared_pointer,
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
                             QListValueDefaultFunction valueDefaultFunction)
{
    Q_ASSERT(sharedPointerGetter);
    Q_ASSERT(sharedPointerDeleter);
    if (!ptr_shared_pointer || !sharedPointerDeleter || !sharedPointerGetter)
        return nullptr;
    void* listPtr = sharedPointerGetter(ptr_shared_pointer);
    if (!listPtr)
        return nullptr;


    jclass clazz = resolveClass(env, className);

    jobject returned = nullptr;
    if (clazz) {
        returned = Java::Private::QtCore::QList.newInstance(env,
                                  clazz,
                                  jlong(appendFunction),
                                  jlong(appendListFunction),
                                  jlong(atFunction),
                                  jlong(beginFunction),
                                  jlong(clearFunction),
                                  jlong(containsFunction),
                                  jlong(countObjectFunction),
                                  jlong(endFunction),
                                  jlong(endsWithFunction),
                                  jlong(indexOfFunction),
                                  jlong(insertFunction),
                                  jlong(lastIndexOfFunction),
                                  jlong(midFunction),
                                  jlong(moveFunction),
                                  jlong(equalFunction),
                                  jlong(prependFunction),
                                  jlong(removeAllFunction),
                                  jlong(removeAtFunction),
                                  jlong(removeOneFunction),
                                  jlong(replaceFunction),
                                  jlong(reserveFunction),
                                  jlong(sizeFunction),
                                  jlong(startsWithFunction),
                                  jlong(swapFunction),
                                  jlong(takeAtFunction),
                                  jlong(toSetFunction),
                                  jlong(valueFunction),
                                  jlong(valueDefaultFunction));
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForSharedPointerToObject(env, returned, QMetaType::UnknownType,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                                          "QList",
#endif
                                                                                          false, false, nullptr, nullptr, ptr_shared_pointer, sharedPointerDeleter, sharedPointerGetter);
        if (!link) {
            returned = nullptr;
        }
    }
    return returned;
}

jobject qtjambi_from_constQQueue_shared_pointer(JNIEnv *env,
                             void* ptr_shared_pointer,
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
                             QListValueDefaultFunction valueDefaultFunction)
{
    Q_ASSERT(sharedPointerGetter);
    Q_ASSERT(sharedPointerDeleter);
    if (!ptr_shared_pointer || !sharedPointerDeleter || !sharedPointerGetter)
        return nullptr;
    void* listPtr = sharedPointerGetter(ptr_shared_pointer);
    if (!listPtr)
        return nullptr;


    jclass clazz = resolveClass(env, className);

    jobject returned = nullptr;
    if (clazz) {
        returned = Java::Private::QtCore::QQueue.newInstance2(env,
                                  clazz,
                                  jlong(atFunction),
                                  jlong(beginFunction),
                                  jlong(containsFunction),
                                  jlong(countObjectFunction),
                                  jlong(endFunction),
                                  jlong(endsWithFunction),
                                  jlong(indexOfFunction),
                                  jlong(lastIndexOfFunction),
                                  jlong(midFunction),
                                  jlong(equalFunction),
                                  jlong(sizeFunction),
                                  jlong(startsWithFunction),
                                  jlong(toSetFunction),
                                  jlong(valueFunction),
                                  jlong(valueDefaultFunction));
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForSharedPointerToObject(env, returned, QMetaType::UnknownType,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                                          "QQueue",
#endif
                                                                                          false, false, nullptr, nullptr, ptr_shared_pointer, sharedPointerDeleter, sharedPointerGetter);
        if (!link) {
            returned = nullptr;
        }
    }
    return returned;
}


jobject qtjambi_from_QQueue_shared_pointer(JNIEnv *env,
                             void* ptr_shared_pointer,
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
                             QListValueDefaultFunction valueDefaultFunction)
{
    Q_ASSERT(sharedPointerGetter);
    Q_ASSERT(sharedPointerDeleter);
    if (!ptr_shared_pointer || !sharedPointerDeleter || !sharedPointerGetter)
        return nullptr;
    void* listPtr = sharedPointerGetter(ptr_shared_pointer);
    if (!listPtr)
        return nullptr;


    jclass clazz = resolveClass(env, className);

    jobject returned = nullptr;
    if (clazz) {
        returned = Java::Private::QtCore::QQueue.newInstance(env,
                                  clazz,
                                  jlong(appendFunction),
                                  jlong(appendListFunction),
                                  jlong(atFunction),
                                  jlong(beginFunction),
                                  jlong(clearFunction),
                                  jlong(containsFunction),
                                  jlong(countObjectFunction),
                                  jlong(endFunction),
                                  jlong(endsWithFunction),
                                  jlong(indexOfFunction),
                                  jlong(insertFunction),
                                  jlong(lastIndexOfFunction),
                                  jlong(midFunction),
                                  jlong(moveFunction),
                                  jlong(equalFunction),
                                  jlong(prependFunction),
                                  jlong(removeAllFunction),
                                  jlong(removeAtFunction),
                                  jlong(removeOneFunction),
                                  jlong(replaceFunction),
                                  jlong(reserveFunction),
                                  jlong(sizeFunction),
                                  jlong(startsWithFunction),
                                  jlong(swapFunction),
                                  jlong(takeAtFunction),
                                  jlong(toSetFunction),
                                  jlong(valueFunction),
                                  jlong(valueDefaultFunction));
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForSharedPointerToObject(env, returned, QMetaType::UnknownType,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                                          "QQueue",
#endif
                                                                                          false, false, nullptr, nullptr, ptr_shared_pointer, sharedPointerDeleter, sharedPointerGetter);
        if (!link) {
            returned = nullptr;
        }
    }
    return returned;
}

jobject qtjambi_from_constQSet_shared_pointer(JNIEnv *env,
                                     void* ptr_shared_pointer,
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
                                )
{
    Q_ASSERT(sharedPointerGetter);
    Q_ASSERT(sharedPointerDeleter);
    if (!ptr_shared_pointer || !sharedPointerDeleter || !sharedPointerGetter)
        return nullptr;
    void* listPtr = sharedPointerGetter(ptr_shared_pointer);
    if (!listPtr)
        return nullptr;


    jclass clazz = resolveClass(env, className);

    jobject returned = nullptr;
    if (clazz) {
        returned = Java::Private::QtCore::QSet.newInstance(env,
                                  clazz,
                                  jlong(beginFunction),
                                  jlong(capacityFunction),
                                  jlong(containsFunction),
                                  jlong(endFunction),
                                  jlong(intersectsFunction),
                                  jlong(equalFunction),
                                  jlong(sizeFunction),
                                  jlong(valuesFunction));
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForSharedPointerToObject(env, returned, QMetaType::UnknownType,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                                          "QSet",
#endif
                                                                                          false, false, nullptr, nullptr, ptr_shared_pointer, sharedPointerDeleter, sharedPointerGetter);
        if (!link) {
            returned = nullptr;
        }
    }
    return returned;
}

jobject qtjambi_from_QSet_shared_pointer(JNIEnv *env,
                                     void* ptr_shared_pointer,
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
                                )
{
    Q_ASSERT(sharedPointerGetter);
    Q_ASSERT(sharedPointerDeleter);
    if (!ptr_shared_pointer || !sharedPointerDeleter || !sharedPointerGetter)
        return nullptr;
    void* listPtr = sharedPointerGetter(ptr_shared_pointer);
    if (!listPtr)
        return nullptr;


    jclass clazz = resolveClass(env, className);

    jobject returned = nullptr;
    if (clazz) {
        returned = Java::Private::QtCore::QSet.newInstance2(env,
                                  clazz,
                                  jlong(beginFunction),
                                  jlong(capacityFunction),
                                  jlong(clearFunction),
                                  jlong(containsFunction),
                                  jlong(endFunction),
                                  jlong(insertFunction),
                                  jlong(intersectFunction),
                                  jlong(intersectsFunction),
                                  jlong(equalFunction),
                                  jlong(removeFunction),
                                  jlong(reserveFunction),
                                  jlong(sizeFunction),
                                  jlong(subtractFunction),
                                  jlong(uniteFunction),
                                  jlong(valuesFunction));
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForSharedPointerToObject(env, returned, QMetaType::UnknownType,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                                          "QSet",
#endif
                                                                                          false, false, nullptr, nullptr, ptr_shared_pointer, sharedPointerDeleter, sharedPointerGetter);
        if (!link) {
            returned = nullptr;
        }
    }
    return returned;
}

jobject qtjambi_from_constQLinkedList_shared_pointer(JNIEnv *env,
                                     void* ptr_shared_pointer,
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
                                )
{
    Q_ASSERT(sharedPointerGetter);
    Q_ASSERT(sharedPointerDeleter);
    if (!ptr_shared_pointer || !sharedPointerDeleter || !sharedPointerGetter)
        return nullptr;
    void* listPtr = sharedPointerGetter(ptr_shared_pointer);
    if (!listPtr)
        return nullptr;


    jclass clazz = resolveClass(env, className);

    jobject returned = nullptr;
    if (clazz) {
        returned = Java::Private::QtCore::QLinkedList.newInstance2(env,
                                  clazz,
                                  jlong(beginFunction),
                                  jlong(containsFunction),
                                  jlong(countObjectFunction),
                                  jlong(endFunction),
                                  jlong(endsWithFunction),
                                  jlong(firstFunction),
                                  jlong(lastFunction),
                                  jlong(equalFunction),
                                  jlong(sizeFunction),
                                  jlong(startsWithFunction));
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForSharedPointerToObject(env, returned, QMetaType::UnknownType,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                              "QLinkedList",
#endif
                                                                              false, false, nullptr, nullptr, ptr_shared_pointer, sharedPointerDeleter, sharedPointerGetter);
        if (!link) {
            returned = nullptr;
        }
    }
    return returned;
}

jobject qtjambi_from_QLinkedList_shared_pointer(JNIEnv *env,
                                     void* ptr_shared_pointer,
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
                                )
{
    Q_ASSERT(sharedPointerGetter);
    Q_ASSERT(sharedPointerDeleter);
    if (!ptr_shared_pointer || !sharedPointerDeleter || !sharedPointerGetter)
        return nullptr;
    void* listPtr = sharedPointerGetter(ptr_shared_pointer);
    if (!listPtr)
        return nullptr;


    jclass clazz = resolveClass(env, className);

    jobject returned = nullptr;
    if (clazz) {
        returned = Java::Private::QtCore::QLinkedList.newInstance(env,
                                  clazz,
                                  jlong(appendFunction),
                                  jlong(beginFunction),
                                  jlong(clearFunction),
                                  jlong(containsFunction),
                                  jlong(countObjectFunction),
                                  jlong(endFunction),
                                  jlong(endsWithFunction),
                                  jlong(firstFunction),
                                  jlong(lastFunction),
                                  jlong(equalFunction),
                                  jlong(prependFunction),
                                  jlong(removeAllFunction),
                                  jlong(removeFirstFunction),
                                  jlong(removeLastFunction),
                                  jlong(removeOneFunction),
                                  jlong(sizeFunction),
                                  jlong(startsWithFunction),
                                  jlong(takeFirstFunction),
                                  jlong(takeLastFunction));
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForSharedPointerToObject(env, returned, QMetaType::UnknownType,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                                          "QLinkedList",
#endif
                                                                                          false, false, nullptr, nullptr, ptr_shared_pointer, sharedPointerDeleter, sharedPointerGetter);
        if (!link) {
            returned = nullptr;
        }
    }
    return returned;
}

jobject qtjambi_from_constQVector_shared_pointer(JNIEnv *env,
                                     void* ptr_shared_pointer,
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
                                )
{
    Q_ASSERT(sharedPointerGetter);
    Q_ASSERT(sharedPointerDeleter);
    if (!ptr_shared_pointer || !sharedPointerDeleter || !sharedPointerGetter)
        return nullptr;
    void* listPtr = sharedPointerGetter(ptr_shared_pointer);
    if (!listPtr)
        return nullptr;


    jclass clazz = resolveClass(env, className);

    jobject returned = nullptr;
    if (clazz) {
        returned = Java::Private::QtCore::QVector.newInstance2(env,
                                  clazz,
                                  jlong(atFunction),
                                  jlong(beginFunction),
                                  jlong(capacityFunction),
                                  jlong(containsFunction),
                                  jlong(countObjectFunction),
                                  jlong(endFunction),
                                  jlong(endsWithFunction),
                                  jlong(indexOfFunction),
                                  jlong(lastIndexOfFunction),
                                  jlong(midFunction),
                                  jlong(equalFunction),
                                  jlong(sizeFunction),
                                  jlong(startsWithFunction),
                                  jlong(toListFunction),
                                  jlong(valueFunction),
                                  jlong(valueDefaultFunction));
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForSharedPointerToObject(env, returned, QMetaType::UnknownType,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                                          "QVector",
#endif
                                                                                          false, false, nullptr, nullptr, ptr_shared_pointer, sharedPointerDeleter, sharedPointerGetter);
        if (!link) {
            returned = nullptr;
        }
    }
    return returned;
}

jobject qtjambi_from_QVector_shared_pointer(JNIEnv *env,
                                     void* ptr_shared_pointer,
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
                                )
{
    Q_ASSERT(sharedPointerGetter);
    Q_ASSERT(sharedPointerDeleter);
    if (!ptr_shared_pointer || !sharedPointerDeleter || !sharedPointerGetter)
        return nullptr;
    void* listPtr = sharedPointerGetter(ptr_shared_pointer);
    if (!listPtr)
        return nullptr;


    jclass clazz = resolveClass(env, className);

    jobject returned = nullptr;
    if (clazz) {
        returned = Java::Private::QtCore::QVector.newInstance(env,
                                  clazz,
                                  jlong(appendFunction),
                                  jlong(appendListFunction),
                                  jlong(atFunction),
                                  jlong(beginFunction),
                                  jlong(capacityFunction),
                                  jlong(clearFunction),
                                  jlong(containsFunction),
                                  jlong(countObjectFunction),
                                  jlong(endFunction),
                                  jlong(endsWithFunction),
                                  jlong(fillFunction),
                                  jlong(indexOfFunction),
                                  jlong(insertFunction),
                                  jlong(insertNFunction),
                                  jlong(lastIndexOfFunction),
                                  jlong(midFunction),
                                  jlong(moveFunction),
                                  jlong(equalFunction),
                                  jlong(prependFunction),
                                  jlong(removeAllFunction),
                                  jlong(removeAtFunction),
                                  jlong(removeNFunction),
                                  jlong(removeOneFunction),
                                  jlong(replaceFunction),
                                  jlong(reserveFunction),
                                  jlong(resizeFunction),
                                  jlong(shrinkToFitFunction),
                                  jlong(sizeFunction),
                                  jlong(startsWithFunction),
                                  jlong(takeAtFunction),
                                  jlong(toListFunction),
                                  jlong(valueFunction),
                                  jlong(valueDefaultFunction));
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForSharedPointerToObject(env, returned, QMetaType::UnknownType,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                                          "QVector",
#endif
                                                                                          false, false, nullptr, nullptr, ptr_shared_pointer, sharedPointerDeleter, sharedPointerGetter);
        if (!link) {
            returned = nullptr;
        }
    }
    return returned;
}

jobject qtjambi_from_constQStack_shared_pointer(JNIEnv *env,
                                     void* ptr_shared_pointer,
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
                                )
{
    Q_ASSERT(sharedPointerGetter);
    Q_ASSERT(sharedPointerDeleter);
    if (!ptr_shared_pointer || !sharedPointerDeleter || !sharedPointerGetter)
        return nullptr;
    void* listPtr = sharedPointerGetter(ptr_shared_pointer);
    if (!listPtr)
        return nullptr;


    jclass clazz = resolveClass(env, className);

    jobject returned = nullptr;
    if (clazz) {
        returned = Java::Private::QtCore::QStack.newInstance2(env,
                                  clazz,
                                  jlong(atFunction),
                                  jlong(beginFunction),
                                  jlong(capacityFunction),
                                  jlong(containsFunction),
                                  jlong(countObjectFunction),
                                  jlong(endFunction),
                                  jlong(endsWithFunction),
                                  jlong(indexOfFunction),
                                  jlong(lastIndexOfFunction),
                                  jlong(midFunction),
                                  jlong(equalFunction),
                                  jlong(sizeFunction),
                                  jlong(startsWithFunction),
                                  jlong(toListFunction),
                                  jlong(valueFunction),
                                  jlong(valueDefaultFunction));
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForSharedPointerToObject(env, returned, QMetaType::UnknownType,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                                          "QStack",
#endif
                                                                                          false, false, nullptr, nullptr, ptr_shared_pointer, sharedPointerDeleter, sharedPointerGetter);
        if (!link) {
            returned = nullptr;
        }
    }
    return returned;
}

jobject qtjambi_from_QStack_shared_pointer(JNIEnv *env,
                                     void* ptr_shared_pointer,
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
                                )
{
    Q_ASSERT(sharedPointerGetter);
    Q_ASSERT(sharedPointerDeleter);
    if (!ptr_shared_pointer || !sharedPointerDeleter || !sharedPointerGetter)
        return nullptr;
    void* listPtr = sharedPointerGetter(ptr_shared_pointer);
    if (!listPtr)
        return nullptr;


    jclass clazz = resolveClass(env, className);

    jobject returned = nullptr;
    if (clazz) {
        returned = Java::Private::QtCore::QStack.newInstance(env,
                                  clazz,
                                  jlong(appendFunction),
                                  jlong(appendListFunction),
                                  jlong(atFunction),
                                  jlong(beginFunction),
                                  jlong(capacityFunction),
                                  jlong(clearFunction),
                                  jlong(containsFunction),
                                  jlong(countObjectFunction),
                                  jlong(endFunction),
                                  jlong(endsWithFunction),
                                  jlong(fillFunction),
                                  jlong(indexOfFunction),
                                  jlong(insertFunction),
                                  jlong(insertNFunction),
                                  jlong(lastIndexOfFunction),
                                  jlong(midFunction),
                                  jlong(moveFunction),
                                  jlong(equalFunction),
                                  jlong(prependFunction),
                                  jlong(removeAllFunction),
                                  jlong(removeAtFunction),
                                  jlong(removeNFunction),
                                  jlong(removeOneFunction),
                                  jlong(replaceFunction),
                                  jlong(reserveFunction),
                                  jlong(resizeFunction),
                                  jlong(shrinkToFitFunction),
                                  jlong(sizeFunction),
                                  jlong(startsWithFunction),
                                  jlong(takeAtFunction),
                                  jlong(toListFunction),
                                  jlong(valueFunction),
                                  jlong(valueDefaultFunction));
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForSharedPointerToObject(env, returned, QMetaType::UnknownType,
  #if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                                              "QStack",
  #endif
                                                                                              false, false, nullptr, nullptr, ptr_shared_pointer, sharedPointerDeleter, sharedPointerGetter);
        if (!link) {
            returned = nullptr;
        }
    }
    return returned;
}

jobject qtjambi_from_constQHash_shared_pointer(JNIEnv *env,
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
                                )
{
    Q_ASSERT(sharedPointerGetter);
    Q_ASSERT(sharedPointerDeleter);
    if (!ptr_shared_pointer || !sharedPointerDeleter || !sharedPointerGetter)
        return nullptr;
    void* listPtr = sharedPointerGetter(ptr_shared_pointer);
    if (!listPtr)
        return nullptr;


    jclass keyClazz = resolveClass(env, keyClassName);
    jclass valueClazz = resolveClass(env, valueClassName);

    jobject returned = nullptr;
    if (keyClazz && valueClazz) {
        returned = Java::Private::QtCore::QHash.newInstance2(env,
                                  keyClazz,
                                  valueClazz,
                                  jlong(beginFunction),
                                  jlong(capacityFunction),
                                  jlong(containsFunction),
                                  jlong(countObjectFunction),
                                  jlong(endFunction),
                                  jlong(findFunction),
                                  jlong(keyFunction),
                                  jlong(keysFunction),
                                  jlong(keysForValueFunction),
                                  jlong(equalFunction),
                                  jlong(sizeFunction),
                                  jlong(uniqueKeysFunction),
                                  jlong(valueFunction),
                                  jlong(valuesFunction),
                                  jlong(valuesKeyFunction));
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForSharedPointerToObject(env, returned, QMetaType::UnknownType,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                                          "QHash",
#endif
                                                                                          false, false, nullptr, nullptr, ptr_shared_pointer, sharedPointerDeleter, sharedPointerGetter);
        if (!link) {
            returned = nullptr;
        }
    }
    return returned;
}

jobject qtjambi_from_QHash_shared_pointer(JNIEnv *env,
                                     void* ptr_shared_pointer,
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
                                )
{
    Q_ASSERT(sharedPointerGetter);
    Q_ASSERT(sharedPointerDeleter);
    if (!ptr_shared_pointer || !sharedPointerDeleter || !sharedPointerGetter)
        return nullptr;
    void* listPtr = sharedPointerGetter(ptr_shared_pointer);
    if (!listPtr)
        return nullptr;


    jclass keyClazz = resolveClass(env, keyClassName);
    jclass valueClazz = resolveClass(env, valueClassName);

    jobject returned = nullptr;
    if (keyClazz && valueClazz) {
        returned = Java::Private::QtCore::QHash.newInstance(env,
                                  keyClazz,
                                  valueClazz,
                                  jlong(beginFunction),
                                  jlong(capacityFunction),
                                  jlong(clearFunction),
                                  jlong(containsFunction),
                                  jlong(countObjectFunction),
                                  jlong(endFunction),
                                  jlong(findFunction),
                                  jlong(insertFunction),
                                  jlong(keyFunction),
                                  jlong(keysFunction),
                                  jlong(keysForValueFunction),
                                  jlong(equalFunction),
                                  jlong(removeAllFunction),
                                  jlong(reserveFunction),
                                  jlong(sizeFunction),
                                  jlong(takeFunction),
                                  jlong(uniqueKeysFunction),
                                  jlong(uniteFunction),
                                  jlong(valueFunction),
                                  jlong(valuesFunction),
                                  jlong(valuesKeyFunction));
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForSharedPointerToObject(env, returned, QMetaType::UnknownType,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                                          "QHash",
#endif
                                                                                          false, false, nullptr, nullptr, ptr_shared_pointer, sharedPointerDeleter, sharedPointerGetter);
        if (!link) {
            returned = nullptr;
        }
    }
    return returned;
}

jobject qtjambi_from_constQMultiHash_shared_pointer(JNIEnv *env,
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
                                     QHashValuesKeyFunction valuesKeyFunction,
                                     QMultiHashContainsPairFunction containsPairFunction,
                                     QMultiHashCountPairFunction countPairFunction,
                                     QMultiHashFindPairFunction findPairFunction
                                )
{
    Q_ASSERT(sharedPointerGetter);
    Q_ASSERT(sharedPointerDeleter);
    if (!ptr_shared_pointer || !sharedPointerDeleter || !sharedPointerGetter)
        return nullptr;
    void* listPtr = sharedPointerGetter(ptr_shared_pointer);
    if (!listPtr)
        return nullptr;


    jclass keyClazz = resolveClass(env, keyClassName);
    jclass valueClazz = resolveClass(env, valueClassName);

    jobject returned = nullptr;
    if (keyClazz && valueClazz) {
        returned = Java::Private::QtCore::QMultiHash.newInstance2(env,
                                  keyClazz,
                                  valueClazz,
                                  jlong(beginFunction),
                                  jlong(capacityFunction),
                                  jlong(containsFunction),
                                  jlong(countObjectFunction),
                                  jlong(endFunction),
                                  jlong(findFunction),
                                  jlong(keyFunction),
                                  jlong(keysFunction),
                                  jlong(keysForValueFunction),
                                  jlong(equalFunction),
                                  jlong(sizeFunction),
                                  jlong(uniqueKeysFunction),
                                  jlong(valueFunction),
                                  jlong(valuesFunction),
                                  jlong(valuesKeyFunction),
                                  jlong(containsPairFunction),
                                  jlong(countPairFunction),
                                  jlong(findPairFunction));
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForSharedPointerToObject(env, returned, QMetaType::UnknownType,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                                          "QMultiHash",
#endif
                                                                                          false, false, nullptr, nullptr, ptr_shared_pointer, sharedPointerDeleter, sharedPointerGetter);
        if (!link) {
            returned = nullptr;
        }
    }
    return returned;
}

jobject qtjambi_from_QMultiHash_shared_pointer(JNIEnv *env,
                                     void* ptr_shared_pointer,
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
                                )
{
    Q_ASSERT(sharedPointerGetter);
    Q_ASSERT(sharedPointerDeleter);
    if (!ptr_shared_pointer || !sharedPointerDeleter || !sharedPointerGetter)
        return nullptr;
    void* listPtr = sharedPointerGetter(ptr_shared_pointer);
    if (!listPtr)
        return nullptr;


    jclass keyClazz = resolveClass(env, keyClassName);
    jclass valueClazz = resolveClass(env, valueClassName);

    jobject returned = nullptr;
    if (keyClazz && valueClazz) {
        returned = Java::Private::QtCore::QMultiHash.newInstance(env,
                                  keyClazz,
                                  valueClazz,
                                  jlong(beginFunction),
                                  jlong(capacityFunction),
                                  jlong(clearFunction),
                                  jlong(containsFunction),
                                  jlong(countObjectFunction),
                                  jlong(endFunction),
                                  jlong(findFunction),
                                  jlong(insertFunction),
                                  jlong(keyFunction),
                                  jlong(keysFunction),
                                  jlong(keysForValueFunction),
                                  jlong(equalFunction),
                                  jlong(removeAllFunction),
                                  jlong(reserveFunction),
                                  jlong(sizeFunction),
                                  jlong(takeFunction),
                                  jlong(uniqueKeysFunction),
                                  jlong(uniteFunction),
                                  jlong(valueFunction),
                                  jlong(valuesFunction),
                                  jlong(valuesKeyFunction),
                                  jlong(containsPairFunction),
                                  jlong(countPairFunction),
                                  jlong(findPairFunction),
                                  jlong(removeAllPairFunction),
                                  jlong(replaceOneFunction));
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForSharedPointerToObject(env, returned, QMetaType::UnknownType,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                                          "QMultiHash",
#endif
                                                                                          false, false, nullptr, nullptr, ptr_shared_pointer, sharedPointerDeleter, sharedPointerGetter);
        if (!link) {
            returned = nullptr;
        }
    }
    return returned;
}

jobject qtjambi_from_constQMap_shared_pointer(JNIEnv *env,
                                     void* ptr_shared_pointer,
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
                                )
{
    Q_ASSERT(sharedPointerGetter);
    Q_ASSERT(sharedPointerDeleter);
    if (!ptr_shared_pointer || !sharedPointerDeleter || !sharedPointerGetter)
        return nullptr;
    void* listPtr = sharedPointerGetter(ptr_shared_pointer);
    if (!listPtr)
        return nullptr;


    jclass keyClazz = resolveClass(env, keyClassName);
    jclass valueClazz = resolveClass(env, valueClassName);

    jobject returned = nullptr;
    if (keyClazz && valueClazz) {
        returned = Java::Private::QtCore::QMap.newInstance2(env,
                                  keyClazz,
                                  valueClazz,
                                  jlong(beginFunction),
                                  jlong(containsFunction),
                                  jlong(countObjectFunction),
                                  jlong(endFunction),
                                  jlong(findFunction),
                                  jlong(firstFunction),
                                  jlong(firstKeyFunction),
                                  jlong(keyFunction),
                                  jlong(keysFunction),
                                  jlong(keysForValueFunction),
                                  jlong(lastFunction),
                                  jlong(lastKeyFunction),
                                  jlong(lowerBoundFunction),
                                  jlong(equalFunction),
                                  jlong(sizeFunction),
                                  jlong(uniqueKeysFunction),
                                  jlong(upperBoundFunction),
                                  jlong(valueFunction),
                                  jlong(valuesFunction),
                                  jlong(valuesKeyFunction));
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForSharedPointerToObject(env, returned, QMetaType::UnknownType,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                                          "QMap",
#endif
                                                                                          false, false, nullptr, nullptr, ptr_shared_pointer, sharedPointerDeleter, sharedPointerGetter);
        if (!link) {
            returned = nullptr;
        }
    }
    return returned;
}

jobject qtjambi_from_QMap_shared_pointer(JNIEnv *env,
                                     void* ptr_shared_pointer,
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
                                )
{
    Q_ASSERT(sharedPointerGetter);
    Q_ASSERT(sharedPointerDeleter);
    if (!ptr_shared_pointer || !sharedPointerDeleter || !sharedPointerGetter)
        return nullptr;
    void* listPtr = sharedPointerGetter(ptr_shared_pointer);
    if (!listPtr)
        return nullptr;


    jclass keyClazz = resolveClass(env, keyClassName);
    jclass valueClazz = resolveClass(env, valueClassName);

    jobject returned = nullptr;
    if (keyClazz && valueClazz) {
        returned = Java::Private::QtCore::QMap.newInstance(env,
                                  keyClazz,
                                  valueClazz,
                                  jlong(beginFunction),
                                  jlong(clearFunction),
                                  jlong(containsFunction),
                                  jlong(countObjectFunction),
                                  jlong(endFunction),
                                  jlong(findFunction),
                                  jlong(firstFunction),
                                  jlong(firstKeyFunction),
                                  jlong(insertFunction),
                                  jlong(keyFunction),
                                  jlong(keysFunction),
                                  jlong(keysForValueFunction),
                                  jlong(lastFunction),
                                  jlong(lastKeyFunction),
                                  jlong(lowerBoundFunction),
                                  jlong(equalFunction),
                                  jlong(removeAllFunction),
                                  jlong(sizeFunction),
                                  jlong(takeFunction),
                                  jlong(uniqueKeysFunction),
                                  jlong(uniteFunction),
                                  jlong(upperBoundFunction),
                                  jlong(valueFunction),
                                  jlong(valuesFunction),
                                  jlong(valuesKeyFunction));
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForSharedPointerToObject(env, returned, QMetaType::UnknownType,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                                          "QMap",
#endif
                                                                                          false, false, nullptr, nullptr, ptr_shared_pointer, sharedPointerDeleter, sharedPointerGetter);
        if (!link) {
            returned = nullptr;
        }
    }
    return returned;
}

jobject qtjambi_from_constQMultiMap_shared_pointer(JNIEnv *env,
                                     void* ptr_shared_pointer,
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
                                )
{
    Q_ASSERT(sharedPointerGetter);
    Q_ASSERT(sharedPointerDeleter);
    if (!ptr_shared_pointer || !sharedPointerDeleter || !sharedPointerGetter)
        return nullptr;
    void* listPtr = sharedPointerGetter(ptr_shared_pointer);
    if (!listPtr)
        return nullptr;


    jclass keyClazz = resolveClass(env, keyClassName);
    jclass valueClazz = resolveClass(env, valueClassName);

    jobject returned = nullptr;
    if (keyClazz && valueClazz) {
        returned = Java::Private::QtCore::QMultiMap.newInstance2(env,
                                  keyClazz,
                                  valueClazz,
                                  jlong(beginFunction),
                                  jlong(containsFunction),
                                  jlong(countObjectFunction),
                                  jlong(endFunction),
                                  jlong(findFunction),
                                  jlong(firstFunction),
                                  jlong(firstKeyFunction),
                                  jlong(keyFunction),
                                  jlong(keysFunction),
                                  jlong(keysForValueFunction),
                                  jlong(lastFunction),
                                  jlong(lastKeyFunction),
                                  jlong(lowerBoundFunction),
                                  jlong(equalFunction),
                                  jlong(sizeFunction),
                                  jlong(uniqueKeysFunction),
                                  jlong(upperBoundFunction),
                                  jlong(valueFunction),
                                  jlong(valuesFunction),
                                  jlong(valuesKeyFunction),
                                  jlong(containsPairFunction),
                                  jlong(countPairFunction),
                                  jlong(findPairFunction));
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForSharedPointerToObject(env, returned, QMetaType::UnknownType,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                                          "QMultiMap",
#endif
                                                                                          false, false, nullptr, nullptr, ptr_shared_pointer, sharedPointerDeleter, sharedPointerGetter);
        if (!link) {
            returned = nullptr;
        }
    }
    return returned;
}

jobject qtjambi_from_QMultiMap_shared_pointer(JNIEnv *env,
                                     void* ptr_shared_pointer,
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
                                )
{
    Q_ASSERT(sharedPointerGetter);
    Q_ASSERT(sharedPointerDeleter);
    if (!ptr_shared_pointer || !sharedPointerDeleter || !sharedPointerGetter)
        return nullptr;
    void* listPtr = sharedPointerGetter(ptr_shared_pointer);
    if (!listPtr)
        return nullptr;


    jclass keyClazz = resolveClass(env, keyClassName);
    jclass valueClazz = resolveClass(env, valueClassName);

    jobject returned = nullptr;
    if (keyClazz && valueClazz) {
        returned = Java::Private::QtCore::QMultiMap.newInstance(env,
                                  keyClazz,
                                  valueClazz,
                                  jlong(beginFunction),
                                  jlong(clearFunction),
                                  jlong(containsFunction),
                                  jlong(countObjectFunction),
                                  jlong(endFunction),
                                  jlong(findFunction),
                                  jlong(firstFunction),
                                  jlong(firstKeyFunction),
                                  jlong(insertFunction),
                                  jlong(keyFunction),
                                  jlong(keysFunction),
                                  jlong(keysForValueFunction),
                                  jlong(lastFunction),
                                  jlong(lastKeyFunction),
                                  jlong(lowerBoundFunction),
                                  jlong(equalFunction),
                                  jlong(removeAllFunction),
                                  jlong(sizeFunction),
                                  jlong(takeFunction),
                                  jlong(uniqueKeysFunction),
                                  jlong(uniteFunction),
                                  jlong(upperBoundFunction),
                                  jlong(valueFunction),
                                  jlong(valuesFunction),
                                  jlong(valuesKeyFunction),
                                  jlong(containsPairFunction),
                                  jlong(countPairFunction),
                                  jlong(findPairFunction),
                                  jlong(removeAllPairFunction),
                                  jlong(replaceOneFunction));
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForSharedPointerToObject(env, returned, QMetaType::UnknownType,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                                          "QMultiMap",
#endif
                                                                                          false, false, nullptr, nullptr, ptr_shared_pointer, sharedPointerDeleter, sharedPointerGetter);
        if (!link) {
            returned = nullptr;
        }
    }
    return returned;
}

jobject qtjambi_to_QIterator(JNIEnv *env,
                           QtJambiNativeID owner,
                           void* iteratorPtr,
                           QIteratorValueFunction valueFunction,
                           QIteratorIncrementFunction incrementFunction,
                           QIteratorDecrementFunction decrementFunction,
                           QIteratorLessThanFunction lessThanFunction,
                           QIteratorEqualsFunction equalsFunction,
                           PtrDeleterFunction destructor_function)
{
    jobject returned = nullptr;

    jobject obj = qtjambi_java_object_reference(owner, env);
    returned = Java::Private::QtCore::QIterator.newInstance(env,
                              obj,
                              jlong(valueFunction),
                              jlong(incrementFunction),
                              jlong(decrementFunction),
                              jlong(lessThanFunction),
                              jlong(equalsFunction));
    const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForOwnedObject(env, returned, iteratorPtr,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                                            "QIterator",
#endif
                                                                                            owner, destructor_function);
    if (!link) {
        returned = nullptr;
    }
    return returned;
}

jobject qtjambi_to_QMapIterator(JNIEnv *env,
                                           QtJambiNativeID owner,
                                           void* iteratorPtr,
                                           QMapIteratorKeyFunction keyFunction,
                                           QIteratorValueFunction valueFunction,
                                           QIteratorIncrementFunction incrementFunction,
                                           QIteratorDecrementFunction decrementFunction,
                                           QIteratorLessThanFunction lessThanFunction,
                                           QIteratorEqualsFunction equalsFunction,
                                           PtrDeleterFunction destructor_function)
{
    jobject returned = nullptr;

    jobject obj = qtjambi_java_object_reference(owner, env);
    returned = Java::Private::QtCore::QMapIterator.newInstance(env,
                              obj,
                              jlong(keyFunction),
                              jlong(valueFunction),
                              jlong(incrementFunction),
                              jlong(decrementFunction),
                              jlong(lessThanFunction),
                              jlong(equalsFunction));
    const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForOwnedObject(env, returned, iteratorPtr,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                                            "QMapIterator",
#endif
                                                                                            owner, destructor_function);
    if (!link) {
        returned = nullptr;
    }
    return returned;
}

jobject qtjambi_to_constQList(JNIEnv *env,
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
                             QListValueDefaultFunction valueDefaultFunction)
{

    if(qtjambi_from_nativeId(owner)==listPtr){
        jobject obj = qtjambi_java_object_reference(owner, env);
        if(obj && Java::Private::QtCore::QList.isInstanceOf(env, obj)){
            return obj;
        }
    }
    jclass clazz = resolveClass(env, getJavaName(typeId));

    jobject returned = nullptr;
    if (clazz) {

        returned = Java::Private::QtCore::QList.newInstance2(env,
                                  clazz,
                                  jlong(atFunction),
                                  jlong(beginFunction),
                                  jlong(containsFunction),
                                  jlong(countObjectFunction),
                                  jlong(endFunction),
                                  jlong(endsWithFunction),
                                  jlong(indexOfFunction),
                                  jlong(lastIndexOfFunction),
                                  jlong(midFunction),
                                  jlong(equalFunction),
                                  jlong(sizeFunction),
                                  jlong(startsWithFunction),
                                  jlong(toSetFunction),
                                  jlong(valueFunction),
                                  jlong(valueDefaultFunction));
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForOwnedObject(env, returned, const_cast<void*>(listPtr),
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                                            "QList",
#endif
                                                                                            owner, nullptr);
        if (!link) {
            returned = nullptr;
        }
    }
    return returned;
}

jobject qtjambi_to_QList(JNIEnv *env,
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
                             QListValueDefaultFunction valueDefaultFunction)
{

    if(qtjambi_from_nativeId(owner)==listPtr){
        jobject obj = qtjambi_java_object_reference(owner, env);
        if(obj && Java::Private::QtCore::QList.isInstanceOf(env, obj)){
            return obj;
        }
    }
    jclass clazz = resolveClass(env, getJavaName(typeId));

    jobject returned = nullptr;
    if (clazz) {

        returned = Java::Private::QtCore::QList.newInstance(env,
                                  clazz,
                                  jlong(appendFunction),
                                  jlong(appendListFunction),
                                  jlong(atFunction),
                                  jlong(beginFunction),
                                  jlong(clearFunction),
                                  jlong(containsFunction),
                                  jlong(countObjectFunction),
                                  jlong(endFunction),
                                  jlong(endsWithFunction),
                                  jlong(indexOfFunction),
                                  jlong(insertFunction),
                                  jlong(lastIndexOfFunction),
                                  jlong(midFunction),
                                  jlong(moveFunction),
                                  jlong(equalFunction),
                                  jlong(prependFunction),
                                  jlong(removeAllFunction),
                                  jlong(removeAtFunction),
                                  jlong(removeOneFunction),
                                  jlong(replaceFunction),
                                  jlong(reserveFunction),
                                  jlong(sizeFunction),
                                  jlong(startsWithFunction),
                                  jlong(swapFunction),
                                  jlong(takeAtFunction),
                                  jlong(toSetFunction),
                                  jlong(valueFunction),
                                  jlong(valueDefaultFunction));
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForOwnedObject(env, returned, listPtr,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                                            "QList",
#endif
                                                                                            owner, nullptr);
        if (!link) {
            returned = nullptr;
        }
    }
    return returned;
}

jobject qtjambi_to_constQQueue(JNIEnv *env,
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
                             QListValueDefaultFunction valueDefaultFunction)
{

    if(qtjambi_from_nativeId(owner)==listPtr){
        jobject obj = qtjambi_java_object_reference(owner, env);
        if(obj && Java::Private::QtCore::QQueue.isInstanceOf(env, obj)){
            return obj;
        }
    }
    jclass clazz = resolveClass(env, getJavaName(typeId));

    jobject returned = nullptr;
    if (clazz) {
        returned = Java::Private::QtCore::QQueue.newInstance2(env,
                                  clazz,
                                  jlong(atFunction),
                                  jlong(beginFunction),
                                  jlong(containsFunction),
                                  jlong(countObjectFunction),
                                  jlong(endFunction),
                                  jlong(endsWithFunction),
                                  jlong(indexOfFunction),
                                  jlong(lastIndexOfFunction),
                                  jlong(midFunction),
                                  jlong(equalFunction),
                                  jlong(sizeFunction),
                                  jlong(startsWithFunction),
                                  jlong(toSetFunction),
                                  jlong(valueFunction),
                                  jlong(valueDefaultFunction));
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForOwnedObject(env, returned, const_cast<void*>(listPtr),
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                                            "QQueue",
#endif
                                                                                            owner, nullptr);
        if (!link) {
            returned = nullptr;
        }
    }
    return returned;
}


jobject qtjambi_to_QQueue(JNIEnv *env,
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
                             QListValueDefaultFunction valueDefaultFunction)
{

    if(qtjambi_from_nativeId(owner)==listPtr){
        jobject obj = qtjambi_java_object_reference(owner, env);
        if(obj && Java::Private::QtCore::QQueue.isInstanceOf(env, obj)){
            return obj;
        }
    }
    jclass clazz = resolveClass(env, getJavaName(typeId));

    jobject returned = nullptr;
    if (clazz) {
        returned = Java::Private::QtCore::QQueue.newInstance(env,
                                  clazz,
                                  jlong(appendFunction),
                                  jlong(appendListFunction),
                                  jlong(atFunction),
                                  jlong(beginFunction),
                                  jlong(clearFunction),
                                  jlong(containsFunction),
                                  jlong(countObjectFunction),
                                  jlong(endFunction),
                                  jlong(endsWithFunction),
                                  jlong(indexOfFunction),
                                  jlong(insertFunction),
                                  jlong(lastIndexOfFunction),
                                  jlong(midFunction),
                                  jlong(moveFunction),
                                  jlong(equalFunction),
                                  jlong(prependFunction),
                                  jlong(removeAllFunction),
                                  jlong(removeAtFunction),
                                  jlong(removeOneFunction),
                                  jlong(replaceFunction),
                                  jlong(reserveFunction),
                                  jlong(sizeFunction),
                                  jlong(startsWithFunction),
                                  jlong(swapFunction),
                                  jlong(takeAtFunction),
                                  jlong(toSetFunction),
                                  jlong(valueFunction),
                                  jlong(valueDefaultFunction));
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForOwnedObject(env, returned, listPtr,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                                            "QQueue",
#endif
                                                                                            owner, nullptr);
        if (!link) {
            returned = nullptr;
        }
    }
    return returned;
}

jobject qtjambi_to_constQSet(JNIEnv *env,
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
                                )
{

    if(qtjambi_from_nativeId(owner)==listPtr){
        jobject obj = qtjambi_java_object_reference(owner, env);
        if(obj && Java::Private::QtCore::QSet.isInstanceOf(env, obj)){
            return obj;
        }
    }
    jclass clazz = resolveClass(env, getJavaName(typeId));

    jobject returned = nullptr;
    if (clazz) {
        returned = Java::Private::QtCore::QSet.newInstance(env,
                                  clazz,
                                  jlong(beginFunction),
                                  jlong(capacityFunction),
                                  jlong(containsFunction),
                                  jlong(endFunction),
                                  jlong(intersectsFunction),
                                  jlong(equalFunction),
                                  jlong(sizeFunction),
                                  jlong(valuesFunction));
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForOwnedObject(env, returned, const_cast<void*>(listPtr),
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                                            "QSet",
#endif
                                                                                            owner, nullptr);
        if (!link) {
            returned = nullptr;
        }
    }
    return returned;
}

jobject qtjambi_to_QSet(JNIEnv *env,
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
                                )
{

    if(qtjambi_from_nativeId(owner)==listPtr){
        jobject obj = qtjambi_java_object_reference(owner, env);
        if(obj && Java::Private::QtCore::QSet.isInstanceOf(env, obj)){
            return obj;
        }
    }
    jclass clazz = resolveClass(env, getJavaName(typeId));

    jobject returned = nullptr;
    if (clazz) {
        returned = Java::Private::QtCore::QSet.newInstance2(env,
                                  clazz,
                                  jlong(beginFunction),
                                  jlong(capacityFunction),
                                  jlong(clearFunction),
                                  jlong(containsFunction),
                                  jlong(endFunction),
                                  jlong(insertFunction),
                                  jlong(intersectFunction),
                                  jlong(intersectsFunction),
                                  jlong(equalFunction),
                                  jlong(removeFunction),
                                  jlong(reserveFunction),
                                  jlong(sizeFunction),
                                  jlong(subtractFunction),
                                  jlong(uniteFunction),
                                  jlong(valuesFunction));
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForOwnedObject(env, returned, listPtr,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                                            "QSet",
#endif
                                                                                            owner, nullptr);
        if (!link) {
            returned = nullptr;
        }
    }
    return returned;
}

jobject qtjambi_to_constQLinkedList(JNIEnv *env,
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
                                )
{

    if(qtjambi_from_nativeId(owner)==listPtr){
        jobject obj = qtjambi_java_object_reference(owner, env);
        if(obj && Java::Private::QtCore::QLinkedList.isInstanceOf(env, obj)){
            return obj;
        }
    }
    jclass clazz = resolveClass(env, getJavaName(typeId));

    jobject returned = nullptr;
    if (clazz) {
        returned = Java::Private::QtCore::QLinkedList.newInstance2(env,
                                  clazz,
                                  jlong(beginFunction),
                                  jlong(containsFunction),
                                  jlong(countObjectFunction),
                                  jlong(endFunction),
                                  jlong(endsWithFunction),
                                  jlong(firstFunction),
                                  jlong(lastFunction),
                                  jlong(equalFunction),
                                  jlong(sizeFunction),
                                  jlong(startsWithFunction));
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForOwnedObject(env, returned, const_cast<void*>(listPtr),
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                                            "QLinkedList",
#endif
                                                                                            owner, nullptr);
        if (!link) {
            returned = nullptr;
        }
    }
    return returned;
}

jobject qtjambi_to_QLinkedList(JNIEnv *env,
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
                                )
{

    if(qtjambi_from_nativeId(owner)==listPtr){
        jobject obj = qtjambi_java_object_reference(owner, env);
        if(obj && Java::Private::QtCore::QLinkedList.isInstanceOf(env, obj)){
            return obj;
        }
    }
    jclass clazz = resolveClass(env, getJavaName(typeId));

    jobject returned = nullptr;
    if (clazz) {
        returned = Java::Private::QtCore::QLinkedList.newInstance(env,
                                  clazz,
                                  jlong(appendFunction),
                                  jlong(beginFunction),
                                  jlong(clearFunction),
                                  jlong(containsFunction),
                                  jlong(countObjectFunction),
                                  jlong(endFunction),
                                  jlong(endsWithFunction),
                                  jlong(firstFunction),
                                  jlong(lastFunction),
                                  jlong(equalFunction),
                                  jlong(prependFunction),
                                  jlong(removeAllFunction),
                                  jlong(removeFirstFunction),
                                  jlong(removeLastFunction),
                                  jlong(removeOneFunction),
                                  jlong(sizeFunction),
                                  jlong(startsWithFunction),
                                  jlong(takeFirstFunction),
                                  jlong(takeLastFunction));
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForOwnedObject(env, returned, listPtr,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                                            "QLinkedList",
#endif
                                                                                            owner, nullptr);
        if (!link) {
            returned = nullptr;
        }
    }
    return returned;
}

jobject qtjambi_to_constQVector(JNIEnv *env,
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
                                )
{

    if(qtjambi_from_nativeId(owner)==listPtr){
        jobject obj = qtjambi_java_object_reference(owner, env);
        if(obj && Java::Private::QtCore::QVector.isInstanceOf(env, obj)){
            return obj;
        }
    }
    jclass clazz = resolveClass(env, getJavaName(typeId));

    jobject returned = nullptr;
    if (clazz) {
        returned = Java::Private::QtCore::QVector.newInstance2(env,
                                  clazz,
                                  jlong(atFunction),
                                  jlong(beginFunction),
                                  jlong(capacityFunction),
                                  jlong(containsFunction),
                                  jlong(countObjectFunction),
                                  jlong(endFunction),
                                  jlong(endsWithFunction),
                                  jlong(indexOfFunction),
                                  jlong(lastIndexOfFunction),
                                  jlong(midFunction),
                                  jlong(equalFunction),
                                  jlong(sizeFunction),
                                  jlong(startsWithFunction),
                                  jlong(toListFunction),
                                  jlong(valueFunction),
                                  jlong(valueDefaultFunction));
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForOwnedObject(env, returned, const_cast<void*>(listPtr),
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                                            "QVector",
#endif
                                                                                            owner, nullptr);
        if (!link) {
            returned = nullptr;
        }
    }
    return returned;
}

jobject qtjambi_to_QVector(JNIEnv *env,
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
                                )
{

    if(qtjambi_from_nativeId(owner)==listPtr){
        jobject obj = qtjambi_java_object_reference(owner, env);
        if(obj && Java::Private::QtCore::QVector.isInstanceOf(env, obj)){
            return obj;
        }
    }
    jclass clazz = resolveClass(env, getJavaName(typeId));

    jobject returned = nullptr;
    if (clazz) {
        returned = Java::Private::QtCore::QVector.newInstance(env,
                                  clazz,
                                  jlong(appendFunction),
                                  jlong(appendListFunction),
                                  jlong(atFunction),
                                  jlong(beginFunction),
                                  jlong(capacityFunction),
                                  jlong(clearFunction),
                                  jlong(containsFunction),
                                  jlong(countObjectFunction),
                                  jlong(endFunction),
                                  jlong(endsWithFunction),
                                  jlong(fillFunction),
                                  jlong(indexOfFunction),
                                  jlong(insertFunction),
                                  jlong(insertNFunction),
                                  jlong(lastIndexOfFunction),
                                  jlong(midFunction),
                                  jlong(moveFunction),
                                  jlong(equalFunction),
                                  jlong(prependFunction),
                                  jlong(removeAllFunction),
                                  jlong(removeAtFunction),
                                  jlong(removeNFunction),
                                  jlong(removeOneFunction),
                                  jlong(replaceFunction),
                                  jlong(reserveFunction),
                                  jlong(resizeFunction),
                                  jlong(shrinkToFitFunction),
                                  jlong(sizeFunction),
                                  jlong(startsWithFunction),
                                  jlong(takeAtFunction),
                                  jlong(toListFunction),
                                  jlong(valueFunction),
                                  jlong(valueDefaultFunction));
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForOwnedObject(env, returned, listPtr,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                                            "QVector",
#endif
                                                                                            owner, nullptr);
        if (!link) {
            returned = nullptr;
        }
    }
    return returned;
}

jobject qtjambi_to_constQStack(JNIEnv *env,
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
                                )
{

    if(qtjambi_from_nativeId(owner)==listPtr){
        jobject obj = qtjambi_java_object_reference(owner, env);
        if(obj && Java::Private::QtCore::QStack.isInstanceOf(env, obj)){
            return obj;
        }
    }
    jclass clazz = resolveClass(env, getJavaName(typeId));

    jobject returned = nullptr;
    if (clazz) {
        returned = Java::Private::QtCore::QStack.newInstance2(env,
                                  clazz,
                                  jlong(atFunction),
                                  jlong(beginFunction),
                                  jlong(capacityFunction),
                                  jlong(containsFunction),
                                  jlong(countObjectFunction),
                                  jlong(endFunction),
                                  jlong(endsWithFunction),
                                  jlong(indexOfFunction),
                                  jlong(lastIndexOfFunction),
                                  jlong(midFunction),
                                  jlong(equalFunction),
                                  jlong(sizeFunction),
                                  jlong(startsWithFunction),
                                  jlong(toListFunction),
                                  jlong(valueFunction),
                                  jlong(valueDefaultFunction));
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForOwnedObject(env, returned, const_cast<void*>(listPtr),
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                                            "QStack",
#endif
                                                                                            owner, nullptr);
        if (!link) {
            returned = nullptr;
        }
    }
    return returned;
}

jobject qtjambi_to_QStack(JNIEnv *env,
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
                                )
{

    if(qtjambi_from_nativeId(owner)==listPtr){
        jobject obj = qtjambi_java_object_reference(owner, env);
        if(obj && Java::Private::QtCore::QStack.isInstanceOf(env, obj)){
            return obj;
        }
    }
    jclass clazz = resolveClass(env, getJavaName(typeId));

    jobject returned = nullptr;
    if (clazz) {
        returned = Java::Private::QtCore::QStack.newInstance(env,
                                  clazz,
                                  jlong(appendFunction),
                                  jlong(appendListFunction),
                                  jlong(atFunction),
                                  jlong(beginFunction),
                                  jlong(capacityFunction),
                                  jlong(clearFunction),
                                  jlong(containsFunction),
                                  jlong(countObjectFunction),
                                  jlong(endFunction),
                                  jlong(endsWithFunction),
                                  jlong(fillFunction),
                                  jlong(indexOfFunction),
                                  jlong(insertFunction),
                                  jlong(insertNFunction),
                                  jlong(lastIndexOfFunction),
                                  jlong(midFunction),
                                  jlong(moveFunction),
                                  jlong(equalFunction),
                                  jlong(prependFunction),
                                  jlong(removeAllFunction),
                                  jlong(removeAtFunction),
                                  jlong(removeNFunction),
                                  jlong(removeOneFunction),
                                  jlong(replaceFunction),
                                  jlong(reserveFunction),
                                  jlong(resizeFunction),
                                  jlong(shrinkToFitFunction),
                                  jlong(sizeFunction),
                                  jlong(startsWithFunction),
                                  jlong(takeAtFunction),
                                  jlong(toListFunction),
                                  jlong(valueFunction),
                                  jlong(valueDefaultFunction));
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForOwnedObject(env, returned, listPtr,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                                            "QStack",
#endif
                                                                                            owner, nullptr);
        if (!link) {
            returned = nullptr;
        }
    }
    return returned;
}

jobject qtjambi_to_constQHash(JNIEnv *env,
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
                                )
{

    if(qtjambi_from_nativeId(owner)==listPtr){
        jobject obj = qtjambi_java_object_reference(owner, env);
        if(obj && Java::Private::QtCore::QHash.isInstanceOf(env, obj)){
            return obj;
        }
    }
    jclass keyClazz = resolveClass(env, getJavaName(keyTypeId));
    jclass valueClazz = resolveClass(env, getJavaName(valueTypeId));

    jobject returned = nullptr;
    if (keyClazz && valueClazz) {
        returned = Java::Private::QtCore::QHash.newInstance2(env,
                                  keyClazz,
                                  valueClazz,
                                  jlong(beginFunction),
                                  jlong(capacityFunction),
                                  jlong(containsFunction),
                                  jlong(countObjectFunction),
                                  jlong(endFunction),
                                  jlong(findFunction),
                                  jlong(keyFunction),
                                  jlong(keysFunction),
                                  jlong(keysForValueFunction),
                                  jlong(equalFunction),
                                  jlong(sizeFunction),
                                  jlong(uniqueKeysFunction),
                                  jlong(valueFunction),
                                  jlong(valuesFunction),
                                  jlong(valuesKeyFunction));
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForOwnedObject(env, returned, const_cast<void*>(listPtr),
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                                            "QHash",
#endif
                                                                                            owner, nullptr);
        if (!link) {
            returned = nullptr;
        }
    }
    return returned;
}

jobject qtjambi_to_QHash(JNIEnv *env,
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
                                )
{

    if(qtjambi_from_nativeId(owner)==listPtr){
        jobject obj = qtjambi_java_object_reference(owner, env);
        if(obj && Java::Private::QtCore::QHash.isInstanceOf(env, obj)){
            return obj;
        }
    }
    jclass keyClazz = resolveClass(env, getJavaName(keyTypeId));
    jclass valueClazz = resolveClass(env, getJavaName(valueTypeId));

    jobject returned = nullptr;
    if (keyClazz && valueClazz) {
        returned = Java::Private::QtCore::QHash.newInstance(env,
                                  keyClazz,
                                  valueClazz,
                                  jlong(beginFunction),
                                  jlong(capacityFunction),
                                  jlong(clearFunction),
                                  jlong(containsFunction),
                                  jlong(countObjectFunction),
                                  jlong(endFunction),
                                  jlong(findFunction),
                                  jlong(insertFunction),
                                  jlong(keyFunction),
                                  jlong(keysFunction),
                                  jlong(keysForValueFunction),
                                  jlong(equalFunction),
                                  jlong(removeAllFunction),
                                  jlong(reserveFunction),
                                  jlong(sizeFunction),
                                  jlong(takeFunction),
                                  jlong(uniqueKeysFunction),
                                  jlong(uniteFunction),
                                  jlong(valueFunction),
                                  jlong(valuesFunction),
                                  jlong(valuesKeyFunction));
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForOwnedObject(env, returned, listPtr,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                                            "QHash",
#endif
                                                                                            owner, nullptr);
        if (!link) {
            returned = nullptr;
        }
    }
    return returned;
}

jobject qtjambi_to_constQMultiHash(JNIEnv *env,
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
                                )
{

    if(qtjambi_from_nativeId(owner)==listPtr){
        jobject obj = qtjambi_java_object_reference(owner, env);
        if(obj && Java::Private::QtCore::QMultiHash.isInstanceOf(env, obj)){
            return obj;
        }
    }
    jclass keyClazz = resolveClass(env, getJavaName(keyTypeId));
    jclass valueClazz = resolveClass(env, getJavaName(valueTypeId));

    jobject returned = nullptr;
    if (keyClazz && valueClazz) {
        returned = Java::Private::QtCore::QMultiHash.newInstance2(env,
                                  keyClazz,
                                  valueClazz,
                                  jlong(beginFunction),
                                  jlong(capacityFunction),
                                  jlong(containsFunction),
                                  jlong(countObjectFunction),
                                  jlong(endFunction),
                                  jlong(findFunction),
                                  jlong(keyFunction),
                                  jlong(keysFunction),
                                  jlong(keysForValueFunction),
                                  jlong(equalFunction),
                                  jlong(sizeFunction),
                                  jlong(uniqueKeysFunction),
                                  jlong(valueFunction),
                                  jlong(valuesFunction),
                                  jlong(valuesKeyFunction),
                                  jlong(containsPairFunction),
                                  jlong(countPairFunction),
                                  jlong(findPairFunction));
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForOwnedObject(env, returned, const_cast<void*>(listPtr),
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                                            "QMultiHash",
#endif
                                                                                            owner, nullptr);
        if (!link) {
            returned = nullptr;
        }
    }
    return returned;
}

jobject qtjambi_to_QMultiHash(JNIEnv *env,
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
                                )
{

    if(qtjambi_from_nativeId(owner)==listPtr){
        jobject obj = qtjambi_java_object_reference(owner, env);
        if(obj && Java::Private::QtCore::QMultiHash.isInstanceOf(env, obj)){
            return obj;
        }
    }
    jclass keyClazz = resolveClass(env, getJavaName(keyTypeId));
    jclass valueClazz = resolveClass(env, getJavaName(valueTypeId));

    jobject returned = nullptr;
    if (keyClazz && valueClazz) {
        returned = Java::Private::QtCore::QMultiHash.newInstance(env,
                                  keyClazz,
                                  valueClazz,
                                  jlong(beginFunction),
                                  jlong(capacityFunction),
                                  jlong(clearFunction),
                                  jlong(containsFunction),
                                  jlong(countObjectFunction),
                                  jlong(endFunction),
                                  jlong(findFunction),
                                  jlong(insertFunction),
                                  jlong(keyFunction),
                                  jlong(keysFunction),
                                  jlong(keysForValueFunction),
                                  jlong(equalFunction),
                                  jlong(removeAllFunction),
                                  jlong(reserveFunction),
                                  jlong(sizeFunction),
                                  jlong(takeFunction),
                                  jlong(uniqueKeysFunction),
                                  jlong(uniteFunction),
                                  jlong(valueFunction),
                                  jlong(valuesFunction),
                                  jlong(valuesKeyFunction),
                                  jlong(containsPairFunction),
                                  jlong(countPairFunction),
                                  jlong(findPairFunction),
                                  jlong(removeAllPairFunction),
                                  jlong(replaceOneFunction));
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForOwnedObject(env, returned, listPtr,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                                            "QMultiHash",
#endif
                                                                                            owner, nullptr);
        if (!link) {
            returned = nullptr;
        }
    }
    return returned;
}

jobject qtjambi_to_constQMap(JNIEnv *env,
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
                                )
{

    if(qtjambi_from_nativeId(owner)==listPtr){
        jobject obj = qtjambi_java_object_reference(owner, env);
        if(obj && Java::Private::QtCore::QMap.isInstanceOf(env, obj)){
            return obj;
        }
    }
    jclass keyClazz = resolveClass(env, getJavaName(keyTypeId));
    jclass valueClazz = resolveClass(env, getJavaName(valueTypeId));

    jobject returned = nullptr;
    if (keyClazz && valueClazz) {
        returned = Java::Private::QtCore::QMap.newInstance2(env,
                                  keyClazz,
                                  valueClazz,
                                  jlong(beginFunction),
                                  jlong(containsFunction),
                                  jlong(countObjectFunction),
                                  jlong(endFunction),
                                  jlong(findFunction),
                                  jlong(firstFunction),
                                  jlong(firstKeyFunction),
                                  jlong(keyFunction),
                                  jlong(keysFunction),
                                  jlong(keysForValueFunction),
                                  jlong(lastFunction),
                                  jlong(lastKeyFunction),
                                  jlong(lowerBoundFunction),
                                  jlong(equalFunction),
                                  jlong(sizeFunction),
                                  jlong(uniqueKeysFunction),
                                  jlong(upperBoundFunction),
                                  jlong(valueFunction),
                                  jlong(valuesFunction),
                                  jlong(valuesKeyFunction));
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForOwnedObject(env, returned, const_cast<void*>(listPtr),
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                                            "QMap",
#endif
                                                                                            owner, nullptr);
        if (!link) {
            returned = nullptr;
        }
    }
    return returned;
}

jobject qtjambi_to_QMap(JNIEnv *env,
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
                                )
{

    if(qtjambi_from_nativeId(owner)==listPtr){
        jobject obj = qtjambi_java_object_reference(owner, env);
        if(obj && Java::Private::QtCore::QMap.isInstanceOf(env, obj)){
            return obj;
        }
    }
    jclass keyClazz = resolveClass(env, getJavaName(keyTypeId));
    jclass valueClazz = resolveClass(env, getJavaName(valueTypeId));

    jobject returned = nullptr;
    if (keyClazz && valueClazz) {
        returned = Java::Private::QtCore::QMap.newInstance(env,
                                  keyClazz,
                                  valueClazz,
                                  jlong(beginFunction),
                                  jlong(clearFunction),
                                  jlong(containsFunction),
                                  jlong(countObjectFunction),
                                  jlong(endFunction),
                                  jlong(findFunction),
                                  jlong(firstFunction),
                                  jlong(firstKeyFunction),
                                  jlong(insertFunction),
                                  jlong(keyFunction),
                                  jlong(keysFunction),
                                  jlong(keysForValueFunction),
                                  jlong(lastFunction),
                                  jlong(lastKeyFunction),
                                  jlong(lowerBoundFunction),
                                  jlong(equalFunction),
                                  jlong(removeAllFunction),
                                  jlong(sizeFunction),
                                  jlong(takeFunction),
                                  jlong(uniqueKeysFunction),
                                  jlong(uniteFunction),
                                  jlong(upperBoundFunction),
                                  jlong(valueFunction),
                                  jlong(valuesFunction),
                                  jlong(valuesKeyFunction));
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForOwnedObject(env, returned, listPtr,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                                            "QMap",
#endif
                                                                                           owner, nullptr);
        if (!link) {
            returned = nullptr;
        }
    }
    return returned;
}

jobject qtjambi_to_constQMultiMap(JNIEnv *env,
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
                                )
{

    if(qtjambi_from_nativeId(owner)==listPtr){
        jobject obj = qtjambi_java_object_reference(owner, env);
        if(obj && Java::Private::QtCore::QMultiMap.isInstanceOf(env, obj)){
            return obj;
        }
    }
    jclass keyClazz = resolveClass(env, getJavaName(keyTypeId));
    jclass valueClazz = resolveClass(env, getJavaName(valueTypeId));

    jobject returned = nullptr;
    if (keyClazz && valueClazz) {
        returned = Java::Private::QtCore::QMultiMap.newInstance2(env,
                                  keyClazz,
                                  valueClazz,
                                  jlong(beginFunction),
                                  jlong(containsFunction),
                                  jlong(countObjectFunction),
                                  jlong(endFunction),
                                  jlong(findFunction),
                                  jlong(firstFunction),
                                  jlong(firstKeyFunction),
                                  jlong(keyFunction),
                                  jlong(keysFunction),
                                  jlong(keysForValueFunction),
                                  jlong(lastFunction),
                                  jlong(lastKeyFunction),
                                  jlong(lowerBoundFunction),
                                  jlong(equalFunction),
                                  jlong(sizeFunction),
                                  jlong(uniqueKeysFunction),
                                  jlong(upperBoundFunction),
                                  jlong(valueFunction),
                                  jlong(valuesFunction),
                                  jlong(valuesKeyFunction),
                                  jlong(containsPairFunction),
                                  jlong(countPairFunction),
                                  jlong(findPairFunction));
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForOwnedObject(env, returned, const_cast<void*>(listPtr),
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                                        "QMultiMap",
#endif
                                                                                        owner, nullptr);
        if (!link) {
            returned = nullptr;
        }
    }
    return returned;
}

jobject qtjambi_to_QMultiMap(JNIEnv *env,
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
                                )
{

    if(qtjambi_from_nativeId(owner)==listPtr){
        jobject obj = qtjambi_java_object_reference(owner, env);
        if(obj && Java::Private::QtCore::QMultiMap.isInstanceOf(env, obj)){
            return obj;
        }
    }
    jclass keyClazz = resolveClass(env, getJavaName(keyTypeId));
    jclass valueClazz = resolveClass(env, getJavaName(valueTypeId));
    jobject returned = nullptr;
    if (keyClazz && valueClazz) {
        returned = Java::Private::QtCore::QMultiMap.newInstance(env,
                                  keyClazz,
                                  valueClazz,
                                  jlong(beginFunction),
                                  jlong(clearFunction),
                                  jlong(containsFunction),
                                  jlong(countObjectFunction),
                                  jlong(endFunction),
                                  jlong(findFunction),
                                  jlong(firstFunction),
                                  jlong(firstKeyFunction),
                                  jlong(insertFunction),
                                  jlong(keyFunction),
                                  jlong(keysFunction),
                                  jlong(keysForValueFunction),
                                  jlong(lastFunction),
                                  jlong(lastKeyFunction),
                                  jlong(lowerBoundFunction),
                                  jlong(equalFunction),
                                  jlong(removeAllFunction),
                                  jlong(sizeFunction),
                                  jlong(takeFunction),
                                  jlong(uniqueKeysFunction),
                                  jlong(uniteFunction),
                                  jlong(upperBoundFunction),
                                  jlong(valueFunction),
                                  jlong(valuesFunction),
                                  jlong(valuesKeyFunction),
                                  jlong(containsPairFunction),
                                  jlong(countPairFunction),
                                  jlong(findPairFunction),
                                  jlong(removeAllPairFunction),
                                  jlong(replaceOneFunction));
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForOwnedObject(env, returned, listPtr,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                                            "QMultiMap",
#endif
                                                                                            owner, nullptr);
        if (!link) {
            returned = nullptr;
        }
    }
    return returned;
}

jobject qtjambi_from_constQList_shared_pointer(JNIEnv *env,
                             void* ptr_shared_pointer,
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
                             QListValueDefaultFunction valueDefaultFunction)
{
    Q_ASSERT(sharedPointerGetter);
    Q_ASSERT(sharedPointerDeleter);
    if (!ptr_shared_pointer || !sharedPointerDeleter || !sharedPointerGetter)
        return nullptr;
    void* listPtr = sharedPointerGetter(ptr_shared_pointer);
    if (!listPtr)
        return nullptr;


    jclass clazz = resolveClass(env, getJavaName(typeId));

    jobject returned = nullptr;
    if (clazz) {
        returned = Java::Private::QtCore::QList.newInstance2(env,
                                  clazz,
                                  jlong(atFunction),
                                  jlong(beginFunction),
                                  jlong(containsFunction),
                                  jlong(countObjectFunction),
                                  jlong(endFunction),
                                  jlong(endsWithFunction),
                                  jlong(indexOfFunction),
                                  jlong(lastIndexOfFunction),
                                  jlong(midFunction),
                                  jlong(equalFunction),
                                  jlong(sizeFunction),
                                  jlong(startsWithFunction),
                                  jlong(toSetFunction),
                                  jlong(valueFunction),
                                  jlong(valueDefaultFunction));
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForSharedPointerToObject(env, returned, QMetaType::UnknownType,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                                          "QList",
#endif
                                                                                          false, false, nullptr, nullptr, ptr_shared_pointer, sharedPointerDeleter, sharedPointerGetter);
        if (!link) {
            returned = nullptr;
        }
    }
    return returned;
}


jobject qtjambi_from_QList_shared_pointer(JNIEnv *env,
                             void* ptr_shared_pointer,
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
                             QListValueDefaultFunction valueDefaultFunction)
{
    Q_ASSERT(sharedPointerGetter);
    Q_ASSERT(sharedPointerDeleter);
    if (!ptr_shared_pointer || !sharedPointerDeleter || !sharedPointerGetter)
        return nullptr;
    void* listPtr = sharedPointerGetter(ptr_shared_pointer);
    if (!listPtr)
        return nullptr;


    jclass clazz = resolveClass(env, getJavaName(typeId));

    jobject returned = nullptr;
    if (clazz) {
        returned = Java::Private::QtCore::QList.newInstance(env,
                                  clazz,
                                  jlong(appendFunction),
                                  jlong(appendListFunction),
                                  jlong(atFunction),
                                  jlong(beginFunction),
                                  jlong(clearFunction),
                                  jlong(containsFunction),
                                  jlong(countObjectFunction),
                                  jlong(endFunction),
                                  jlong(endsWithFunction),
                                  jlong(indexOfFunction),
                                  jlong(insertFunction),
                                  jlong(lastIndexOfFunction),
                                  jlong(midFunction),
                                  jlong(moveFunction),
                                  jlong(equalFunction),
                                  jlong(prependFunction),
                                  jlong(removeAllFunction),
                                  jlong(removeAtFunction),
                                  jlong(removeOneFunction),
                                  jlong(replaceFunction),
                                  jlong(reserveFunction),
                                  jlong(sizeFunction),
                                  jlong(startsWithFunction),
                                  jlong(swapFunction),
                                  jlong(takeAtFunction),
                                  jlong(toSetFunction),
                                  jlong(valueFunction),
                                  jlong(valueDefaultFunction));
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForSharedPointerToObject(env, returned, QMetaType::UnknownType,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                                          "QList",
#endif
                                                                                          false, false, nullptr, nullptr, ptr_shared_pointer, sharedPointerDeleter, sharedPointerGetter);
        if (!link) {
            returned = nullptr;
        }
    }
    return returned;
}

jobject qtjambi_from_constQQueue_shared_pointer(JNIEnv *env,
                             void* ptr_shared_pointer,
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
                             QListValueDefaultFunction valueDefaultFunction)
{
    Q_ASSERT(sharedPointerGetter);
    Q_ASSERT(sharedPointerDeleter);
    if (!ptr_shared_pointer || !sharedPointerDeleter || !sharedPointerGetter)
        return nullptr;
    void* listPtr = sharedPointerGetter(ptr_shared_pointer);
    if (!listPtr)
        return nullptr;


    jclass clazz = resolveClass(env, getJavaName(typeId));

    jobject returned = nullptr;
    if (clazz) {
        returned = Java::Private::QtCore::QQueue.newInstance(env,
                                  clazz,
                                  jlong(atFunction),
                                  jlong(beginFunction),
                                  jlong(containsFunction),
                                  jlong(countObjectFunction),
                                  jlong(endFunction),
                                  jlong(endsWithFunction),
                                  jlong(indexOfFunction),
                                  jlong(lastIndexOfFunction),
                                  jlong(midFunction),
                                  jlong(equalFunction),
                                  jlong(sizeFunction),
                                  jlong(startsWithFunction),
                                  jlong(toSetFunction),
                                  jlong(valueFunction),
                                  jlong(valueDefaultFunction));
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForSharedPointerToObject(env, returned, QMetaType::UnknownType,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                                          "QQueue",
#endif
                                                                                          false, false, nullptr, nullptr, ptr_shared_pointer, sharedPointerDeleter, sharedPointerGetter);
        if (!link) {
            returned = nullptr;
        }
    }
    return returned;
}


jobject qtjambi_from_QQueue_shared_pointer(JNIEnv *env,
                             void* ptr_shared_pointer,
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
                             QListValueDefaultFunction valueDefaultFunction)
{
    Q_ASSERT(sharedPointerGetter);
    Q_ASSERT(sharedPointerDeleter);
    if (!ptr_shared_pointer || !sharedPointerDeleter || !sharedPointerGetter)
        return nullptr;
    void* listPtr = sharedPointerGetter(ptr_shared_pointer);
    if (!listPtr)
        return nullptr;


    jclass clazz = resolveClass(env, getJavaName(typeId));

    jobject returned = nullptr;
    if (clazz) {
        returned = Java::Private::QtCore::QQueue.newInstance(env,
                                  clazz,
                                  jlong(appendFunction),
                                  jlong(appendListFunction),
                                  jlong(atFunction),
                                  jlong(beginFunction),
                                  jlong(clearFunction),
                                  jlong(containsFunction),
                                  jlong(countObjectFunction),
                                  jlong(endFunction),
                                  jlong(endsWithFunction),
                                  jlong(indexOfFunction),
                                  jlong(insertFunction),
                                  jlong(lastIndexOfFunction),
                                  jlong(midFunction),
                                  jlong(moveFunction),
                                  jlong(equalFunction),
                                  jlong(prependFunction),
                                  jlong(removeAllFunction),
                                  jlong(removeAtFunction),
                                  jlong(removeOneFunction),
                                  jlong(replaceFunction),
                                  jlong(reserveFunction),
                                  jlong(sizeFunction),
                                  jlong(startsWithFunction),
                                  jlong(swapFunction),
                                  jlong(takeAtFunction),
                                  jlong(toSetFunction),
                                  jlong(valueFunction),
                                  jlong(valueDefaultFunction));
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForSharedPointerToObject(env, returned, QMetaType::UnknownType,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                                          "QQueue",
#endif
                                                                                          false, false, nullptr, nullptr, ptr_shared_pointer, sharedPointerDeleter, sharedPointerGetter);
        if (!link) {
            returned = nullptr;
        }
    }
    return returned;
}

jobject qtjambi_from_constQSet_shared_pointer(JNIEnv *env,
                                     void* ptr_shared_pointer,
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
                                )
{
    Q_ASSERT(sharedPointerGetter);
    Q_ASSERT(sharedPointerDeleter);
    if (!ptr_shared_pointer || !sharedPointerDeleter || !sharedPointerGetter)
        return nullptr;
    void* listPtr = sharedPointerGetter(ptr_shared_pointer);
    if (!listPtr)
        return nullptr;


    jclass clazz = resolveClass(env, getJavaName(typeId));

    jobject returned = nullptr;
    if (clazz) {
        returned = Java::Private::QtCore::QSet.newInstance(env,
                                  clazz,
                                  jlong(beginFunction),
                                  jlong(capacityFunction),
                                  jlong(containsFunction),
                                  jlong(endFunction),
                                  jlong(intersectsFunction),
                                  jlong(equalFunction),
                                  jlong(sizeFunction),
                                  jlong(valuesFunction));
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForSharedPointerToObject(env, returned, QMetaType::UnknownType,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                                          "QSet",
#endif
                                                                                          false, false, nullptr, nullptr, ptr_shared_pointer, sharedPointerDeleter, sharedPointerGetter);
        if (!link) {
            returned = nullptr;
        }
    }
    return returned;
}

jobject qtjambi_from_QSet_shared_pointer(JNIEnv *env,
                                     void* ptr_shared_pointer,
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
                                )
{
    Q_ASSERT(sharedPointerGetter);
    Q_ASSERT(sharedPointerDeleter);
    if (!ptr_shared_pointer || !sharedPointerDeleter || !sharedPointerGetter)
        return nullptr;
    void* listPtr = sharedPointerGetter(ptr_shared_pointer);
    if (!listPtr)
        return nullptr;


    jclass clazz = resolveClass(env, getJavaName(typeId));

    jobject returned = nullptr;
    if (clazz) {
        returned = Java::Private::QtCore::QSet.newInstance2(env,
                                  clazz,
                                  jlong(beginFunction),
                                  jlong(capacityFunction),
                                  jlong(clearFunction),
                                  jlong(containsFunction),
                                  jlong(endFunction),
                                  jlong(insertFunction),
                                  jlong(intersectFunction),
                                  jlong(intersectsFunction),
                                  jlong(equalFunction),
                                  jlong(removeFunction),
                                  jlong(reserveFunction),
                                  jlong(sizeFunction),
                                  jlong(subtractFunction),
                                  jlong(uniteFunction),
                                  jlong(valuesFunction));
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForSharedPointerToObject(env, returned, QMetaType::UnknownType,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                                          "QSet",
#endif
                                                                                          false, false, nullptr, nullptr, ptr_shared_pointer, sharedPointerDeleter, sharedPointerGetter);
        if (!link) {
            returned = nullptr;
        }
    }
    return returned;
}

jobject qtjambi_from_constQLinkedList_shared_pointer(JNIEnv *env,
                                     void* ptr_shared_pointer,
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
                                )
{
    Q_ASSERT(sharedPointerGetter);
    Q_ASSERT(sharedPointerDeleter);
    if (!ptr_shared_pointer || !sharedPointerDeleter || !sharedPointerGetter)
        return nullptr;
    void* listPtr = sharedPointerGetter(ptr_shared_pointer);
    if (!listPtr)
        return nullptr;


    jclass clazz = resolveClass(env, getJavaName(typeId));

    jobject returned = nullptr;
    if (clazz) {
        returned = Java::Private::QtCore::QLinkedList.newInstance2(env,
                                  clazz,
                                  jlong(beginFunction),
                                  jlong(containsFunction),
                                  jlong(countObjectFunction),
                                  jlong(endFunction),
                                  jlong(endsWithFunction),
                                  jlong(firstFunction),
                                  jlong(lastFunction),
                                  jlong(equalFunction),
                                  jlong(sizeFunction),
                                  jlong(startsWithFunction));
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForSharedPointerToObject(env, returned, QMetaType::UnknownType,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                                          "QLinkedList",
#endif
                                                                                          false, false, nullptr, nullptr, ptr_shared_pointer, sharedPointerDeleter, sharedPointerGetter);
        if (!link) {
            returned = nullptr;
        }
    }
    return returned;
}

jobject qtjambi_from_QLinkedList_shared_pointer(JNIEnv *env,
                                     void* ptr_shared_pointer,
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
                                )
{
    Q_ASSERT(sharedPointerGetter);
    Q_ASSERT(sharedPointerDeleter);
    if (!ptr_shared_pointer || !sharedPointerDeleter || !sharedPointerGetter)
        return nullptr;
    void* listPtr = sharedPointerGetter(ptr_shared_pointer);
    if (!listPtr)
        return nullptr;


    jclass clazz = resolveClass(env, getJavaName(typeId));

    jobject returned = nullptr;
    if (clazz) {
        returned = Java::Private::QtCore::QLinkedList.newInstance(env,
                                  clazz,
                                  jlong(appendFunction),
                                  jlong(beginFunction),
                                  jlong(clearFunction),
                                  jlong(containsFunction),
                                  jlong(countObjectFunction),
                                  jlong(endFunction),
                                  jlong(endsWithFunction),
                                  jlong(firstFunction),
                                  jlong(lastFunction),
                                  jlong(equalFunction),
                                  jlong(prependFunction),
                                  jlong(removeAllFunction),
                                  jlong(removeFirstFunction),
                                  jlong(removeLastFunction),
                                  jlong(removeOneFunction),
                                  jlong(sizeFunction),
                                  jlong(startsWithFunction),
                                  jlong(takeFirstFunction),
                                  jlong(takeLastFunction));
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForSharedPointerToObject(env, returned, QMetaType::UnknownType,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                                  "QLinkedList",
#endif
                                                                                  false, false, nullptr, nullptr, ptr_shared_pointer, sharedPointerDeleter, sharedPointerGetter);
        if (!link) {
            returned = nullptr;
        }
    }
    return returned;
}

jobject qtjambi_from_constQVector_shared_pointer(JNIEnv *env,
                                     void* ptr_shared_pointer,
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
                                )
{
    Q_ASSERT(sharedPointerGetter);
    Q_ASSERT(sharedPointerDeleter);
    if (!ptr_shared_pointer || !sharedPointerDeleter || !sharedPointerGetter)
        return nullptr;
    void* listPtr = sharedPointerGetter(ptr_shared_pointer);
    if (!listPtr)
        return nullptr;


    jclass clazz = resolveClass(env, getJavaName(typeId));

    jobject returned = nullptr;
    if (clazz) {
        returned = Java::Private::QtCore::QVector.newInstance2(env,
                                  clazz,
                                  jlong(atFunction),
                                  jlong(beginFunction),
                                  jlong(capacityFunction),
                                  jlong(containsFunction),
                                  jlong(countObjectFunction),
                                  jlong(endFunction),
                                  jlong(endsWithFunction),
                                  jlong(indexOfFunction),
                                  jlong(lastIndexOfFunction),
                                  jlong(midFunction),
                                  jlong(equalFunction),
                                  jlong(sizeFunction),
                                  jlong(startsWithFunction),
                                  jlong(toListFunction),
                                  jlong(valueFunction),
                                  jlong(valueDefaultFunction));
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForSharedPointerToObject(env, returned, QMetaType::UnknownType,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                                          "QVector",
#endif
                                                                                          false, false, nullptr, nullptr, ptr_shared_pointer, sharedPointerDeleter, sharedPointerGetter);
        if (!link) {
            returned = nullptr;
        }
    }
    return returned;
}

jobject qtjambi_from_QVector_shared_pointer(JNIEnv *env,
                                     void* ptr_shared_pointer,
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
                                )
{
    Q_ASSERT(sharedPointerGetter);
    Q_ASSERT(sharedPointerDeleter);
    if (!ptr_shared_pointer || !sharedPointerDeleter || !sharedPointerGetter)
        return nullptr;
    void* listPtr = sharedPointerGetter(ptr_shared_pointer);
    if (!listPtr)
        return nullptr;


    jclass clazz = resolveClass(env, getJavaName(typeId));

    jobject returned = nullptr;
    if (clazz) {
        returned = Java::Private::QtCore::QVector.newInstance(env,
                                  clazz,
                                  jlong(appendFunction),
                                  jlong(appendListFunction),
                                  jlong(atFunction),
                                  jlong(beginFunction),
                                  jlong(capacityFunction),
                                  jlong(clearFunction),
                                  jlong(containsFunction),
                                  jlong(countObjectFunction),
                                  jlong(endFunction),
                                  jlong(endsWithFunction),
                                  jlong(fillFunction),
                                  jlong(indexOfFunction),
                                  jlong(insertFunction),
                                  jlong(insertNFunction),
                                  jlong(lastIndexOfFunction),
                                  jlong(midFunction),
                                  jlong(moveFunction),
                                  jlong(equalFunction),
                                  jlong(prependFunction),
                                  jlong(removeAllFunction),
                                  jlong(removeAtFunction),
                                  jlong(removeNFunction),
                                  jlong(removeOneFunction),
                                  jlong(replaceFunction),
                                  jlong(reserveFunction),
                                  jlong(resizeFunction),
                                  jlong(shrinkToFitFunction),
                                  jlong(sizeFunction),
                                  jlong(startsWithFunction),
                                  jlong(takeAtFunction),
                                  jlong(toListFunction),
                                  jlong(valueFunction),
                                  jlong(valueDefaultFunction));
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForSharedPointerToObject(env, returned, QMetaType::UnknownType,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                                          "QVector",
#endif
                                                                                          false, false, nullptr, nullptr, ptr_shared_pointer, sharedPointerDeleter, sharedPointerGetter);
        if (!link) {
            returned = nullptr;
        }
    }
    return returned;
}

jobject qtjambi_from_constQStack_shared_pointer(JNIEnv *env,
                                     void* ptr_shared_pointer,
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
                                )
{
    Q_ASSERT(sharedPointerGetter);
    Q_ASSERT(sharedPointerDeleter);
    if (!ptr_shared_pointer || !sharedPointerDeleter || !sharedPointerGetter)
        return nullptr;
    void* listPtr = sharedPointerGetter(ptr_shared_pointer);
    if (!listPtr)
        return nullptr;


    jclass clazz = resolveClass(env, getJavaName(typeId));

    jobject returned = nullptr;
    if (clazz) {
        returned = Java::Private::QtCore::QStack.newInstance2(env,
                                  clazz,
                                  jlong(atFunction),
                                  jlong(beginFunction),
                                  jlong(capacityFunction),
                                  jlong(containsFunction),
                                  jlong(countObjectFunction),
                                  jlong(endFunction),
                                  jlong(endsWithFunction),
                                  jlong(indexOfFunction),
                                  jlong(lastIndexOfFunction),
                                  jlong(midFunction),
                                  jlong(equalFunction),
                                  jlong(sizeFunction),
                                  jlong(startsWithFunction),
                                  jlong(toListFunction),
                                  jlong(valueFunction),
                                  jlong(valueDefaultFunction));
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForSharedPointerToObject(env, returned, QMetaType::UnknownType,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                                          "QStack",
#endif
                                                                                          false, false, nullptr, nullptr, ptr_shared_pointer, sharedPointerDeleter, sharedPointerGetter);
        if (!link) {
            returned = nullptr;
        }
    }
    return returned;
}

jobject qtjambi_from_QStack_shared_pointer(JNIEnv *env,
                                     void* ptr_shared_pointer,
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
                                )
{
    Q_ASSERT(sharedPointerGetter);
    Q_ASSERT(sharedPointerDeleter);
    if (!ptr_shared_pointer || !sharedPointerDeleter || !sharedPointerGetter)
        return nullptr;
    void* listPtr = sharedPointerGetter(ptr_shared_pointer);
    if (!listPtr)
        return nullptr;


    jclass clazz = resolveClass(env, getJavaName(typeId));

    jobject returned = nullptr;
    if (clazz) {
        returned = Java::Private::QtCore::QStack.newInstance(env,
                                  clazz,
                                  jlong(appendFunction),
                                  jlong(appendListFunction),
                                  jlong(atFunction),
                                  jlong(beginFunction),
                                  jlong(capacityFunction),
                                  jlong(clearFunction),
                                  jlong(containsFunction),
                                  jlong(countObjectFunction),
                                  jlong(endFunction),
                                  jlong(endsWithFunction),
                                  jlong(fillFunction),
                                  jlong(indexOfFunction),
                                  jlong(insertFunction),
                                  jlong(insertNFunction),
                                  jlong(lastIndexOfFunction),
                                  jlong(midFunction),
                                  jlong(moveFunction),
                                  jlong(equalFunction),
                                  jlong(prependFunction),
                                  jlong(removeAllFunction),
                                  jlong(removeAtFunction),
                                  jlong(removeNFunction),
                                  jlong(removeOneFunction),
                                  jlong(replaceFunction),
                                  jlong(reserveFunction),
                                  jlong(resizeFunction),
                                  jlong(shrinkToFitFunction),
                                  jlong(sizeFunction),
                                  jlong(startsWithFunction),
                                  jlong(takeAtFunction),
                                  jlong(toListFunction),
                                  jlong(valueFunction),
                                  jlong(valueDefaultFunction));
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForSharedPointerToObject(env, returned, QMetaType::UnknownType,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                                          "QStack",
#endif
                                                                                          false, false, nullptr, nullptr, ptr_shared_pointer, sharedPointerDeleter, sharedPointerGetter);
        if (!link) {
            returned = nullptr;
        }
    }
    return returned;
}

jobject qtjambi_from_constQHash_shared_pointer(JNIEnv *env,
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
                                )
{
    Q_ASSERT(sharedPointerGetter);
    Q_ASSERT(sharedPointerDeleter);
    if (!ptr_shared_pointer || !sharedPointerDeleter || !sharedPointerGetter)
        return nullptr;
    void* listPtr = sharedPointerGetter(ptr_shared_pointer);
    if (!listPtr)
        return nullptr;


    jclass keyClazz = resolveClass(env, getJavaName(keyTypeId));
    jclass valueClazz = resolveClass(env, getJavaName(valueTypeId));

    jobject returned = nullptr;
    if (keyClazz && valueClazz) {
        returned = Java::Private::QtCore::QHash.newInstance2(env,
                                  keyClazz,
                                  valueClazz,
                                  jlong(beginFunction),
                                  jlong(capacityFunction),
                                  jlong(containsFunction),
                                  jlong(countObjectFunction),
                                  jlong(endFunction),
                                  jlong(findFunction),
                                  jlong(keyFunction),
                                  jlong(keysFunction),
                                  jlong(keysForValueFunction),
                                  jlong(equalFunction),
                                  jlong(sizeFunction),
                                  jlong(uniqueKeysFunction),
                                  jlong(valueFunction),
                                  jlong(valuesFunction),
                                  jlong(valuesKeyFunction));
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForSharedPointerToObject(env, returned, QMetaType::UnknownType,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                                          "QHash",
#endif
                                                                                          false, false, nullptr, nullptr, ptr_shared_pointer, sharedPointerDeleter, sharedPointerGetter);
        if (!link) {
            returned = nullptr;
        }
    }
    return returned;
}

jobject qtjambi_from_QHash_shared_pointer(JNIEnv *env,
                                     void* ptr_shared_pointer,
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
                                )
{
    Q_ASSERT(sharedPointerGetter);
    Q_ASSERT(sharedPointerDeleter);
    if (!ptr_shared_pointer || !sharedPointerDeleter || !sharedPointerGetter)
        return nullptr;
    void* listPtr = sharedPointerGetter(ptr_shared_pointer);
    if (!listPtr)
        return nullptr;


    jclass keyClazz = resolveClass(env, getJavaName(keyTypeId));
    jclass valueClazz = resolveClass(env, getJavaName(valueTypeId));

    jobject returned = nullptr;
    if (keyClazz && valueClazz) {
        returned = Java::Private::QtCore::QHash.newInstance(env,
                                  keyClazz,
                                  valueClazz,
                                  jlong(beginFunction),
                                  jlong(capacityFunction),
                                  jlong(clearFunction),
                                  jlong(containsFunction),
                                  jlong(countObjectFunction),
                                  jlong(endFunction),
                                  jlong(findFunction),
                                  jlong(insertFunction),
                                  jlong(keyFunction),
                                  jlong(keysFunction),
                                  jlong(keysForValueFunction),
                                  jlong(equalFunction),
                                  jlong(removeAllFunction),
                                  jlong(reserveFunction),
                                  jlong(sizeFunction),
                                  jlong(takeFunction),
                                  jlong(uniqueKeysFunction),
                                  jlong(uniteFunction),
                                  jlong(valueFunction),
                                  jlong(valuesFunction),
                                  jlong(valuesKeyFunction));
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForSharedPointerToObject(env, returned, QMetaType::UnknownType,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                                                    "QHash",
#endif
                                                                                                    false, false, nullptr, nullptr, ptr_shared_pointer, sharedPointerDeleter, sharedPointerGetter);
        if (!link) {
            returned = nullptr;
        }
    }
    return returned;
}

jobject qtjambi_from_constQMultiHash_shared_pointer(JNIEnv *env,
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
                                     QHashValuesKeyFunction valuesKeyFunction,
                                     QMultiHashContainsPairFunction containsPairFunction,
                                     QMultiHashCountPairFunction countPairFunction,
                                     QMultiHashFindPairFunction findPairFunction
                                )
{
    Q_ASSERT(sharedPointerGetter);
    Q_ASSERT(sharedPointerDeleter);
    if (!ptr_shared_pointer || !sharedPointerDeleter || !sharedPointerGetter)
        return nullptr;
    void* listPtr = sharedPointerGetter(ptr_shared_pointer);
    if (!listPtr)
        return nullptr;


    jclass keyClazz = resolveClass(env, getJavaName(keyTypeId));
    jclass valueClazz = resolveClass(env, getJavaName(valueTypeId));

    jobject returned = nullptr;
    if (keyClazz && valueClazz) {
        returned = Java::Private::QtCore::QMultiHash.newInstance2(env,
                                  keyClazz,
                                  valueClazz,
                                  jlong(beginFunction),
                                  jlong(capacityFunction),
                                  jlong(containsFunction),
                                  jlong(countObjectFunction),
                                  jlong(endFunction),
                                  jlong(findFunction),
                                  jlong(keyFunction),
                                  jlong(keysFunction),
                                  jlong(keysForValueFunction),
                                  jlong(equalFunction),
                                  jlong(sizeFunction),
                                  jlong(uniqueKeysFunction),
                                  jlong(valueFunction),
                                  jlong(valuesFunction),
                                  jlong(valuesKeyFunction),
                                  jlong(containsPairFunction),
                                  jlong(countPairFunction),
                                  jlong(findPairFunction));
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForSharedPointerToObject(env, returned, QMetaType::UnknownType,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                                          "QMultiHash",
#endif
                                                                                          false, false, nullptr, nullptr, ptr_shared_pointer, sharedPointerDeleter, sharedPointerGetter);
        if (!link) {
            returned = nullptr;
        }
    }
    return returned;
}

jobject qtjambi_from_QMultiHash_shared_pointer(JNIEnv *env,
                                     void* ptr_shared_pointer,
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
                                )
{
    Q_ASSERT(sharedPointerGetter);
    Q_ASSERT(sharedPointerDeleter);
    if (!ptr_shared_pointer || !sharedPointerDeleter || !sharedPointerGetter)
        return nullptr;
    void* listPtr = sharedPointerGetter(ptr_shared_pointer);
    if (!listPtr)
        return nullptr;


    jclass keyClazz = resolveClass(env, getJavaName(keyTypeId));
    jclass valueClazz = resolveClass(env, getJavaName(valueTypeId));

    jobject returned = nullptr;
    if (keyClazz && valueClazz) {
        returned = Java::Private::QtCore::QMultiHash.newInstance(env,
                                  keyClazz,
                                  valueClazz,
                                  jlong(beginFunction),
                                  jlong(capacityFunction),
                                  jlong(clearFunction),
                                  jlong(containsFunction),
                                  jlong(countObjectFunction),
                                  jlong(endFunction),
                                  jlong(findFunction),
                                  jlong(insertFunction),
                                  jlong(keyFunction),
                                  jlong(keysFunction),
                                  jlong(keysForValueFunction),
                                  jlong(equalFunction),
                                  jlong(removeAllFunction),
                                  jlong(reserveFunction),
                                  jlong(sizeFunction),
                                  jlong(takeFunction),
                                  jlong(uniqueKeysFunction),
                                  jlong(uniteFunction),
                                  jlong(valueFunction),
                                  jlong(valuesFunction),
                                  jlong(valuesKeyFunction),
                                  jlong(containsPairFunction),
                                  jlong(countPairFunction),
                                  jlong(findPairFunction),
                                  jlong(removeAllPairFunction),
                                  jlong(replaceOneFunction));
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForSharedPointerToObject(env, returned, QMetaType::UnknownType,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                                          "QMultiHash",
#endif
                                                                                          false, false, nullptr, nullptr, ptr_shared_pointer, sharedPointerDeleter, sharedPointerGetter);
        if (!link) {
            returned = nullptr;
        }
    }
    return returned;
}

jobject qtjambi_from_constQMap_shared_pointer(JNIEnv *env,
                                     void* ptr_shared_pointer,
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
                                )
{
    Q_ASSERT(sharedPointerGetter);
    Q_ASSERT(sharedPointerDeleter);
    if (!ptr_shared_pointer || !sharedPointerDeleter || !sharedPointerGetter)
        return nullptr;
    void* listPtr = sharedPointerGetter(ptr_shared_pointer);
    if (!listPtr)
        return nullptr;


    jclass keyClazz = resolveClass(env, getJavaName(keyTypeId));
    jclass valueClazz = resolveClass(env, getJavaName(valueTypeId));

    jobject returned = nullptr;
    if (keyClazz && valueClazz) {
        returned = Java::Private::QtCore::QMap.newInstance(env,
                                  keyClazz,
                                  valueClazz,
                                  jlong(beginFunction),
                                  jlong(containsFunction),
                                  jlong(countObjectFunction),
                                  jlong(endFunction),
                                  jlong(findFunction),
                                  jlong(firstFunction),
                                  jlong(firstKeyFunction),
                                  jlong(keyFunction),
                                  jlong(keysFunction),
                                  jlong(keysForValueFunction),
                                  jlong(lastFunction),
                                  jlong(lastKeyFunction),
                                  jlong(lowerBoundFunction),
                                  jlong(equalFunction),
                                  jlong(sizeFunction),
                                  jlong(uniqueKeysFunction),
                                  jlong(upperBoundFunction),
                                  jlong(valueFunction),
                                  jlong(valuesFunction),
                                  jlong(valuesKeyFunction));
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForSharedPointerToObject(env, returned, QMetaType::UnknownType,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                                          "QMap",
#endif
                                                                                          false, false, nullptr, nullptr, ptr_shared_pointer, sharedPointerDeleter, sharedPointerGetter);
        if (!link) {
            returned = nullptr;
        }
    }
    return returned;
}

jobject qtjambi_from_QMap_shared_pointer(JNIEnv *env,
                                     void* ptr_shared_pointer,
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
                                )
{
    Q_ASSERT(sharedPointerGetter);
    Q_ASSERT(sharedPointerDeleter);
    if (!ptr_shared_pointer || !sharedPointerDeleter || !sharedPointerGetter)
        return nullptr;
    void* listPtr = sharedPointerGetter(ptr_shared_pointer);
    if (!listPtr)
        return nullptr;


    jclass keyClazz = resolveClass(env, getJavaName(keyTypeId));
    jclass valueClazz = resolveClass(env, getJavaName(valueTypeId));

    jobject returned = nullptr;
    if (keyClazz && valueClazz) {
        returned = Java::Private::QtCore::QMap.newInstance(env,
                                  keyClazz,
                                  valueClazz,
                                  jlong(beginFunction),
                                  jlong(clearFunction),
                                  jlong(containsFunction),
                                  jlong(countObjectFunction),
                                  jlong(endFunction),
                                  jlong(findFunction),
                                  jlong(firstFunction),
                                  jlong(firstKeyFunction),
                                  jlong(insertFunction),
                                  jlong(keyFunction),
                                  jlong(keysFunction),
                                  jlong(keysForValueFunction),
                                  jlong(lastFunction),
                                  jlong(lastKeyFunction),
                                  jlong(lowerBoundFunction),
                                  jlong(equalFunction),
                                  jlong(removeAllFunction),
                                  jlong(sizeFunction),
                                  jlong(takeFunction),
                                  jlong(uniqueKeysFunction),
                                  jlong(uniteFunction),
                                  jlong(upperBoundFunction),
                                  jlong(valueFunction),
                                  jlong(valuesFunction),
                                  jlong(valuesKeyFunction));
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForSharedPointerToObject(env, returned, QMetaType::UnknownType,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                                          "QMap",
#endif
                                                                                          false, false, nullptr, nullptr, ptr_shared_pointer, sharedPointerDeleter, sharedPointerGetter);
        if (!link) {
            returned = nullptr;
        }
    }
    return returned;
}

jobject qtjambi_from_constQMultiMap_shared_pointer(JNIEnv *env,
                                     void* ptr_shared_pointer,
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
                                )
{
    Q_ASSERT(sharedPointerGetter);
    Q_ASSERT(sharedPointerDeleter);
    if (!ptr_shared_pointer || !sharedPointerDeleter || !sharedPointerGetter)
        return nullptr;
    void* listPtr = sharedPointerGetter(ptr_shared_pointer);
    if (!listPtr)
        return nullptr;


    jclass keyClazz = resolveClass(env, getJavaName(keyTypeId));
    jclass valueClazz = resolveClass(env, getJavaName(valueTypeId));

    jobject returned = nullptr;
    if (keyClazz && valueClazz) {
        returned = Java::Private::QtCore::QMultiMap.newInstance2(env,
                                  keyClazz,
                                  valueClazz,
                                  jlong(beginFunction),
                                  jlong(containsFunction),
                                  jlong(countObjectFunction),
                                  jlong(endFunction),
                                  jlong(findFunction),
                                  jlong(firstFunction),
                                  jlong(firstKeyFunction),
                                  jlong(keyFunction),
                                  jlong(keysFunction),
                                  jlong(keysForValueFunction),
                                  jlong(lastFunction),
                                  jlong(lastKeyFunction),
                                  jlong(lowerBoundFunction),
                                  jlong(equalFunction),
                                  jlong(sizeFunction),
                                  jlong(uniqueKeysFunction),
                                  jlong(upperBoundFunction),
                                  jlong(valueFunction),
                                  jlong(valuesFunction),
                                  jlong(valuesKeyFunction),
                                  jlong(containsPairFunction),
                                  jlong(countPairFunction),
                                  jlong(findPairFunction));
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForSharedPointerToObject(env, returned, QMetaType::UnknownType,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                                          "QMultiMap",
#endif
                                                                                          false, false, nullptr, nullptr, ptr_shared_pointer, sharedPointerDeleter, sharedPointerGetter);
        if (!link) {
            returned = nullptr;
        }
    }
    return returned;
}

jobject qtjambi_from_QMultiMap_shared_pointer(JNIEnv *env,
                                     void* ptr_shared_pointer,
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
                                )
{
    Q_ASSERT(sharedPointerGetter);
    Q_ASSERT(sharedPointerDeleter);
    if (!ptr_shared_pointer || !sharedPointerDeleter || !sharedPointerGetter)
        return nullptr;
    void* listPtr = sharedPointerGetter(ptr_shared_pointer);
    if (!listPtr)
        return nullptr;


    jclass keyClazz = resolveClass(env, getJavaName(keyTypeId));
    jclass valueClazz = resolveClass(env, getJavaName(valueTypeId));

    jobject returned = nullptr;
    if (keyClazz && valueClazz) {
        returned = Java::Private::QtCore::QMultiMap.newInstance(env,
                                  keyClazz,
                                  valueClazz,
                                  jlong(beginFunction),
                                  jlong(clearFunction),
                                  jlong(containsFunction),
                                  jlong(countObjectFunction),
                                  jlong(endFunction),
                                  jlong(findFunction),
                                  jlong(firstFunction),
                                  jlong(firstKeyFunction),
                                  jlong(insertFunction),
                                  jlong(keyFunction),
                                  jlong(keysFunction),
                                  jlong(keysForValueFunction),
                                  jlong(lastFunction),
                                  jlong(lastKeyFunction),
                                  jlong(lowerBoundFunction),
                                  jlong(equalFunction),
                                  jlong(removeAllFunction),
                                  jlong(sizeFunction),
                                  jlong(takeFunction),
                                  jlong(uniqueKeysFunction),
                                  jlong(uniteFunction),
                                  jlong(upperBoundFunction),
                                  jlong(valueFunction),
                                  jlong(valuesFunction),
                                  jlong(valuesKeyFunction),
                                  jlong(containsPairFunction),
                                  jlong(countPairFunction),
                                  jlong(findPairFunction),
                                  jlong(removeAllPairFunction),
                                  jlong(replaceOneFunction));
        const QSharedPointer<QtJambiLink>& link = QtJambiLink::createLinkForSharedPointerToObject(env, returned, QMetaType::UnknownType,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                                                          "QMultiMap",
#endif
                                                                                          false, false, nullptr, nullptr, ptr_shared_pointer, sharedPointerDeleter, sharedPointerGetter);
        if (!link) {
            returned = nullptr;
        }
    }
    return returned;
}
