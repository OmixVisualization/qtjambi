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

#include <qtjambi/qtjambi_core.h>
#include <qtjambi/qtjambi_repository.h>

// emitting  (functionsInTargetLang writeFinalFunction)
// QList<T>::append(const QList<T> & t)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList__1_1qt_1QList_1appendList__JLjava_util_Collection_2J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jlong appendListFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QList<T>::append(const QList<T> & t)")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QList<QVariant>));
        reinterpret_cast<QListAppendListFunction>(appendListFunction)(__jni_env, __qt_this, t0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// QList<T>::append(const T & t)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList__1_1qt_1QList_1append__JLjava_lang_Object_2J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jlong appendFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QList<T>::append(const T & t)")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QList<QVariant>));
        reinterpret_cast<QListAppendFunction>(appendFunction)(__jni_env, __qt_this, t0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// QList<T>::at(int i) const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList__1_1qt_1QList_1at__JIJ)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jint i0,
 jlong atFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QList<T>::at(int i) const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QList<QVariant>));
        return reinterpret_cast<QListAtFunction>(atFunction)(__jni_env, __qt_this, i0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

// QList<T>::begin() const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList__1_1qt_1QList_1begin__JJ)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jlong beginFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QList<T>::begin() const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QList<QVariant>));
        return reinterpret_cast<QListBeginFunction>(beginFunction)(__jni_env, __this_nativeId, __qt_this);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

// QList<T>::clear()
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList__1_1qt_1QList_1clear__JJ)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jlong clearFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QList<T>::clear()")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QList<QVariant>));
        return reinterpret_cast<QListClearFunction>(clearFunction)(__jni_env, __qt_this);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// QList<T>::contains(const T & t) const
extern "C" Q_DECL_EXPORT jboolean JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList__1_1qt_1QList_1contains__JLjava_lang_Object_2J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jlong containsFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QList<T>::contains(const T & t) const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QList<QVariant>));
        return reinterpret_cast<QListContainsFunction>(containsFunction)(__jni_env, __qt_this, t0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return false;
}

// QList<T>::count(const T & t) const
extern "C" Q_DECL_EXPORT jint JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList__1_1qt_1QList_1count__JLjava_lang_Object_2J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jlong countObjectFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QList<T>::count(const T & t) const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QList<QVariant>));
        return reinterpret_cast<QListCountObjectFunction>(countObjectFunction)(__jni_env, __qt_this, t0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return 0;
}

// QList<T>::end() const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList__1_1qt_1QList_1end__JJ)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jlong endFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QList<T>::end() const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QList<QVariant>));
        return reinterpret_cast<QListEndFunction>(endFunction)(__jni_env, __this_nativeId, __qt_this);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

// QList<T>::endsWith(const T & t) const
extern "C" Q_DECL_EXPORT jboolean JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList__1_1qt_1QList_1endsWith__JLjava_lang_Object_2J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jlong endsWithFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QList<T>::endsWith(const T & t) const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QList<QVariant>));
        return reinterpret_cast<QListEndsWithFunction>(endsWithFunction)(__jni_env, __qt_this, t0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return false;
}

// QList<T>::indexOf(const T & t, int from) const
extern "C" Q_DECL_EXPORT jint JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList__1_1qt_1QList_1indexOf__JLjava_lang_Object_2IJ)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jint from1,
 jlong indexOfFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QList<T>::indexOf(const T & t, int from) const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QList<QVariant>));
        return reinterpret_cast<QListIndexOfFunction>(indexOfFunction)(__jni_env, __qt_this, t0, from1);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return -1;
}

// QList<T>::insert(int i, const T & t)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList__1_1qt_1QList_1insert__JILjava_lang_Object_2J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jint i0,
 jobject t1,
 jlong insertFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QList<T>::insert(int i, const T & t)")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QList<QVariant>));
        reinterpret_cast<QListInsertFunction>(insertFunction)(__jni_env, __qt_this, i0, t1);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// QList<T>::lastIndexOf(const T & t, int from) const
extern "C" Q_DECL_EXPORT jint JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList__1_1qt_1QList_1lastIndexOf__JLjava_lang_Object_2IJ)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jint from1,
 jlong lastIndexOfFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QList<T>::lastIndexOf(const T & t, int from) const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QList<QVariant>));
        return reinterpret_cast<QListLastIndexOfFunction>(lastIndexOfFunction)(__jni_env, __qt_this, t0, from1);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return -1;
}

// QList<T>::mid(int pos, int length) const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList__1_1qt_1QList_1mid__JIIJ)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jint pos0,
 jint length1,
 jlong midFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QList<T>::mid(int pos, int length) const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QList<QVariant>));
        return reinterpret_cast<QListMidFunction>(midFunction)(__jni_env, __qt_this, pos0, length1);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

// QList<T>::move(int from, int to)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList__1_1qt_1QList_1move__JIIJ)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jint from0,
 jint to1,
 jlong moveFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QList<T>::move(int from, int to)")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QList<QVariant>));
        reinterpret_cast<QListMoveFunction>(moveFunction)(__jni_env, __qt_this, from0, to1);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// QList<T>::operator==(const QList & l) const
extern "C" Q_DECL_EXPORT jboolean JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList__1_1qt_1QList_1operator_1equal__JLjava_util_Collection_2J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject l0,
 jlong equalFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QList<T>::operator==(const QList<T> & l) const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QList<QVariant>));
        return reinterpret_cast<QListEqualFunction>(equalFunction)(__jni_env, __qt_this, l0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return false;
}

// QList<T>::prepend(const T & t)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList__1_1qt_1QList_1prepend__JLjava_lang_Object_2J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jlong prependFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QList<T>::prepend(const T & t)")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QList<QVariant>));
        reinterpret_cast<QListPrependFunction>(prependFunction)(__jni_env, __qt_this, t0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// QList<T>::removeAll(const T & t)
extern "C" Q_DECL_EXPORT jint JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList__1_1qt_1QList_1removeAll__JLjava_lang_Object_2J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jlong removeAllFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QList<T>::removeAll(const T & t)")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QList<QVariant>));
        return reinterpret_cast<QListRemoveAllFunction>(removeAllFunction)(__jni_env, __qt_this, t0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return -1;
}

// QList<T>::removeAt(int i)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList__1_1qt_1QList_1removeAt__JIJ)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jint i0,
 jlong removeAtFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QList<T>::removeAt(int i)")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QList<QVariant>));
        reinterpret_cast<QListRemoveAtFunction>(removeAtFunction)(__jni_env, __qt_this, i0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// QList<T>::removeOne(const T & t)
extern "C" Q_DECL_EXPORT jboolean JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList__1_1qt_1QList_1removeOne__JLjava_lang_Object_2J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jlong removeOneFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QList<T>::removeOne(const T & t)")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QList<QVariant>));
        return reinterpret_cast<QListRemoveOneFunction>(removeOneFunction)(__jni_env, __qt_this, t0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return false;
}

// QList<T>::replace(int i, const T & t)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList__1_1qt_1QList_1replace__JILjava_lang_Object_2J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jint i0,
 jobject t1,
 jlong replaceFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QList<T>::replace(int i, const T & t)")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QList<QVariant>));
        reinterpret_cast<QListReplaceFunction>(replaceFunction)(__jni_env, __qt_this, i0, t1);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// QList<T>::reserve(int size)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList__1_1qt_1QList_1reserve__JIJ)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jint size0,
 jlong reserveFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QList<T>::reserve(int size)")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QList<QVariant>));
        reinterpret_cast<QListReserveFunction>(reserveFunction)(__jni_env, __qt_this, size0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// QList<T>::size() const
extern "C" Q_DECL_EXPORT jint JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList__1_1qt_1QList_1size__JJ)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jlong sizeFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QList<T>::size() const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QList<QVariant>));
        return reinterpret_cast<QListSizeFunction>(sizeFunction)(__jni_env, __qt_this);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return 0;
}

// QList<T>::startsWith(const T & t) const
extern "C" Q_DECL_EXPORT jboolean JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList__1_1qt_1QList_1startsWith__JLjava_lang_Object_2J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jlong startsWithFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QList<T>::startsWith(const T & t) const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QList<QVariant>));
        return reinterpret_cast<QListStartsWithFunction>(startsWithFunction)(__jni_env, __qt_this, t0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return false;
}

// QList<T>::swap(int i, int j)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList__1_1qt_1QList_1swap__JIIJ)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jint i0,
 jint j1,
 jlong swapFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QList<T>::swap(int i, int j)")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QList<QVariant>));
        reinterpret_cast<QListSwapFunction>(swapFunction)(__jni_env, __qt_this, i0, j1);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// QList<T>::takeAt(int i)
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList__1_1qt_1QList_1takeAt__JIJ)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jint i0,
 jlong takeAtFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QList<T>::takeAt(int i)")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QList<QVariant>));
        return reinterpret_cast<QListTakeAtFunction>(takeAtFunction)(__jni_env, __qt_this, i0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

// QList<T>::toSet() const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList__1_1qt_1QList_1toSet__JJ)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jlong toSetFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QList<T>::toSet() const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QList<QVariant>));
        return reinterpret_cast<QListToSetFunction>(toSetFunction)(__jni_env, __qt_this);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

// QList<T>::value(int i) const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList__1_1qt_1QList_1value__JIJ)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jint i0,
 jlong valueFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QList<T>::value(int i) const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QList<QVariant>));
        return reinterpret_cast<QListValueFunction>(valueFunction)(__jni_env, __qt_this, i0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

// QList<T>::value(int i, const T & defaultValue) const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList__1_1qt_1QList_1valueDefault__JILjava_lang_Object_2J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jint i0,
 jobject defaultValue1,
 jlong valueDefaultFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QList<T>::value(int i, const T & defaultValue) const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QList<QVariant>));
        return reinterpret_cast<QListValueDefaultFunction>(valueDefaultFunction)(__jni_env, __qt_this, i0, defaultValue1);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

// emitting (AbstractMetaClass::NormalFunctions|AbstractMetaClass::AbstractFunctions writeFinalFunction)
// emitting Field accessors (writeFieldAccessors)
// emitting (writeInterfaceCastFunction)
// emitting (writeSignalInitialization)
// emitting (writeJavaLangObjectOverrideFunctions)


