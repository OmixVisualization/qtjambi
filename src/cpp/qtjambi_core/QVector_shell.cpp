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
// QVector<T>::append(const QVector<T> & t)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QVector__1_1qt_1QVector_1appendVector__JLjava_util_Collection_2J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jlong appendVectorFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QVector<T>::append(const QVector<T> & t)")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QVector<QVariant>));
        reinterpret_cast<QVectorAppendVectorFunction>(appendVectorFunction)(__jni_env, __qt_this, t0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// QVector<T>::append(const T & t)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QVector__1_1qt_1QVector_1append__JLjava_lang_Object_2J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jlong appendFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QVector<T>::append(const T & t)")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QVector<QVariant>));
        reinterpret_cast<QVectorAppendFunction>(appendFunction)(__jni_env, __qt_this, t0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// QVector<T>::at(int i) const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QVector__1_1qt_1QVector_1at__JIJ)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jint i0,
 jlong atFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QVector<T>::at(int i) const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QVector<QVariant>));
        return reinterpret_cast<QVectorAtFunction>(atFunction)(__jni_env, __qt_this, i0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

// QVector<T>::begin() const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QVector__1_1qt_1QVector_1begin__JJ)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jlong beginFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QVector<T>::begin() const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QVector<QVariant>));
        return reinterpret_cast<QVectorBeginFunction>(beginFunction)(__jni_env, __this_nativeId, __qt_this);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

// QVector<T>::capacity() const
extern "C" Q_DECL_EXPORT jint JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QVector__1_1qt_1QVector_1capacity__JJ)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jlong capacityFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QVector<T>::capacity() const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QVector<QVariant>));
        return reinterpret_cast<QVectorCapacityFunction>(capacityFunction)(__jni_env, __qt_this);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return 0;
}

// QVector<T>::clear()
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QVector__1_1qt_1QVector_1clear__JJ)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jlong clearFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QVector<T>::clear()")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QVector<QVariant>));
        return reinterpret_cast<QVectorClearFunction>(clearFunction)(__jni_env, __qt_this);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// QVector<T>::contains(const T & t) const
extern "C" Q_DECL_EXPORT jboolean JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QVector__1_1qt_1QVector_1contains__JLjava_lang_Object_2J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jlong containsFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QVector<T>::contains(const T & t) const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QVector<QVariant>));
        return reinterpret_cast<QVectorContainsFunction>(containsFunction)(__jni_env, __qt_this, t0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return false;
}

// QVector<T>::count(const T & t) const
extern "C" Q_DECL_EXPORT jint JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QVector__1_1qt_1QVector_1count__JLjava_lang_Object_2J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jlong countObjectFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QVector<T>::count(const T & t) const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QVector<QVariant>));
        return reinterpret_cast<QVectorCountObjectFunction>(countObjectFunction)(__jni_env, __qt_this, t0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return 0;
}

// QVector<T>::end() const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QVector__1_1qt_1QVector_1end__JJ)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jlong endFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QVector<T>::end() const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QVector<QVariant>));
        return reinterpret_cast<QVectorEndFunction>(endFunction)(__jni_env, __this_nativeId, __qt_this);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

// QVector<T>::endsWith(const T & t) const
extern "C" Q_DECL_EXPORT jboolean JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QVector__1_1qt_1QVector_1endsWith__JLjava_lang_Object_2J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jlong endsWithFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QVector<T>::endsWith(const T & t) const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QVector<QVariant>));
        return reinterpret_cast<QVectorEndsWithFunction>(endsWithFunction)(__jni_env, __qt_this, t0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return false;
}

// QVector<T>::fill(const T &value, int size = ...)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QVector__1_1qt_1QVector_1fill__JLjava_lang_Object_2IJ)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject value0,
 jint size1,
 jlong fillFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QVector<T>::fill(const T &value, int size = ...)")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QVector<QVariant>));
        reinterpret_cast<QVectorFillFunction>(fillFunction)(__jni_env, __qt_this, value0, size1);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// QVector<T>::indexOf(const T & t, int from) const
extern "C" Q_DECL_EXPORT jint JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QVector__1_1qt_1QVector_1indexOf__JLjava_lang_Object_2IJ)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jint from1,
 jlong indexOfFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QVector<T>::indexOf(const T & t, int from) const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QVector<QVariant>));
        return reinterpret_cast<QVectorIndexOfFunction>(indexOfFunction)(__jni_env, __qt_this, t0, from1);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return -1;
}

// QVector<T>::insert(int i, const T & t)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QVector__1_1qt_1QVector_1insert__JILjava_lang_Object_2J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jint i0,
 jobject t1,
 jlong insertFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QVector<T>::insert(int i, const T & t)")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QVector<QVariant>));
        reinterpret_cast<QVectorInsertFunction>(insertFunction)(__jni_env, __qt_this, i0, t1);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// QVector<T>::insert(int i, int count, const T & t)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QVector__1_1qt_1QVector_1insert__JIILjava_lang_Object_2J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jint i0,
 jint count1,
 jobject t2,
 jlong insertNFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QVector<T>::insert(int i, int count, const T & t)")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QVector<QVariant>));
        reinterpret_cast<QVectorInsertNFunction>(insertNFunction)(__jni_env, __qt_this, i0, count1, t2);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// QVector<T>::lastIndexOf(const T & t, int from) const
extern "C" Q_DECL_EXPORT jint JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QVector__1_1qt_1QVector_1lastIndexOf__JLjava_lang_Object_2IJ)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jint from1,
 jlong lastIndexOfFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QVector<T>::lastIndexOf(const T & t, int from) const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QVector<QVariant>));
        return reinterpret_cast<QVectorLastIndexOfFunction>(lastIndexOfFunction)(__jni_env, __qt_this, t0, from1);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return -1;
}

// QVector<T>::mid(int pos, int length) const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QVector__1_1qt_1QVector_1mid__JIIJ)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jint pos0,
 jint length1,
 jlong midFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QVector<T>::mid(int pos, int length) const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QVector<QVariant>));
        return reinterpret_cast<QVectorMidFunction>(midFunction)(__jni_env, __qt_this, pos0, length1);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

// QVector<T>::move(int from, int to)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QVector__1_1qt_1QVector_1move__JIIJ)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jint from0,
 jint to1,
 jlong moveFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QVector<T>::move(int from, int to)")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QVector<QVariant>));
        reinterpret_cast<QVectorMoveFunction>(moveFunction)(__jni_env, __qt_this, from0, to1);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// QVector<T>::operator==(const QVector & l) const
extern "C" Q_DECL_EXPORT jboolean JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QVector__1_1qt_1QVector_1operator_1equal__JLjava_util_Collection_2J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject l0,
 jlong equalFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QVector<T>::operator==(const QVector<T> & l) const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QVector<QVariant>));
        return reinterpret_cast<QVectorEqualFunction>(equalFunction)(__jni_env, __qt_this, l0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return false;
}

// QVector<T>::prepend(const T & t)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QVector__1_1qt_1QVector_1prepend__JLjava_lang_Object_2J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jlong prependFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QVector<T>::prepend(const T & t)")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QVector<QVariant>));
        reinterpret_cast<QVectorPrependFunction>(prependFunction)(__jni_env, __qt_this, t0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// QVector<T>::removeAll(const T & t)
extern "C" Q_DECL_EXPORT jint JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QVector__1_1qt_1QVector_1removeAll__JLjava_lang_Object_2J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jlong removeAllFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QVector<T>::removeAll(const T & t)")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QVector<QVariant>));
        return reinterpret_cast<QVectorRemoveAllFunction>(removeAllFunction)(__jni_env, __qt_this, t0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return -1;
}

// QVector<T>::remove(int i, int count)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QVector__1_1qt_1QVector_1removeN__JIJ)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jint i0,
 jint count1,
 jlong removeNFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QVector<T>::remove(int i, int count)")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QVector<QVariant>));
        reinterpret_cast<QVectorRemoveNFunction>(removeNFunction)(__jni_env, __qt_this, i0, count1);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// QVector<T>::removeAt(int i)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QVector__1_1qt_1QVector_1removeAt__JIJ)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jint i0,
 jlong removeAtFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QVector<T>::removeAt(int i)")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QVector<QVariant>));
        reinterpret_cast<QVectorRemoveAtFunction>(removeAtFunction)(__jni_env, __qt_this, i0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// QVector<T>::removeOne(const T & t)
extern "C" Q_DECL_EXPORT jboolean JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QVector__1_1qt_1QVector_1removeOne__JLjava_lang_Object_2J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jlong removeOneFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QVector<T>::removeOne(const T & t)")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QVector<QVariant>));
        return reinterpret_cast<QVectorRemoveOneFunction>(removeOneFunction)(__jni_env, __qt_this, t0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return false;
}

// QVector<T>::replace(int i, const T & t)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QVector__1_1qt_1QVector_1replace__JILjava_lang_Object_2J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jint i0,
 jobject t1,
 jlong replaceFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QVector<T>::replace(int i, const T & t)")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QVector<QVariant>));
        reinterpret_cast<QVectorReplaceFunction>(replaceFunction)(__jni_env, __qt_this, i0, t1);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// QVector<T>::reserve(int size)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QVector__1_1qt_1QVector_1reserve__JIJ)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jint size0,
 jlong reserveFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QVector<T>::reserve(int size)")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QVector<QVariant>));
        reinterpret_cast<QVectorReserveFunction>(reserveFunction)(__jni_env, __qt_this, size0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// QVector<T>::resize(int size)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QVector__1_1qt_1QVector_1resize__JIJ)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jint size0,
 jlong resizeFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QVector<T>::resize(int size)")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QVector<QVariant>));
        reinterpret_cast<QVectorResizeFunction>(resizeFunction)(__jni_env, __qt_this, size0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// QVector<T>::shrink_to_fit()
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QVector__1_1qt_1QVector_1shrink_1to_1fit__JJ)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jlong shrinkToFitFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QVector<T>::shrink_to_fit()")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QVector<QVariant>));
        reinterpret_cast<QVectorShrinkToFitFunction>(shrinkToFitFunction)(__jni_env, __qt_this);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// QVector<T>::size() const
extern "C" Q_DECL_EXPORT jint JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QVector__1_1qt_1QVector_1size__JJ)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jlong sizeFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QVector<T>::size() const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QVector<QVariant>));
        return reinterpret_cast<QVectorSizeFunction>(sizeFunction)(__jni_env, __qt_this);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return 0;
}

// QVector<T>::startsWith(const T & t) const
extern "C" Q_DECL_EXPORT jboolean JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QVector__1_1qt_1QVector_1startsWith__JLjava_lang_Object_2J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jlong startsWithFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QVector<T>::startsWith(const T & t) const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QVector<QVariant>));
        return reinterpret_cast<QVectorStartsWithFunction>(startsWithFunction)(__jni_env, __qt_this, t0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return false;
}

// QVector<T>::takeAt(int i)
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QVector__1_1qt_1QVector_1takeAt__JIJ)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jint i0,
 jlong takeAtFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QVector<T>::takeAt(int i)")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QVector<QVariant>));
        return reinterpret_cast<QVectorTakeAtFunction>(takeAtFunction)(__jni_env, __qt_this, i0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

// QVector<T>::toList() const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QVector__1_1qt_1QVector_1toList__JJ)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jlong toSetFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QVector<T>::toList() const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QVector<QVariant>));
        return reinterpret_cast<QVectorToListFunction>(toSetFunction)(__jni_env, __qt_this);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

// QVector<T>::value(int i) const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QVector__1_1qt_1QVector_1value__JIJ)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jint i0,
 jlong valueFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QVector<T>::value(int i) const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QVector<QVariant>));
        return reinterpret_cast<QVectorValueFunction>(valueFunction)(__jni_env, __qt_this, i0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

// QVector<T>::value(int i, const T & defaultValue) const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QVector__1_1qt_1QVector_1valueDefault__JILjava_lang_Object_2J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jint i0,
 jobject defaultValue1,
 jlong valueDefaultFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QVector<T>::value(int i, const T & defaultValue) const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QVector<QVariant>));
        return reinterpret_cast<QVectorValueDefaultFunction>(valueDefaultFunction)(__jni_env, __qt_this, i0, defaultValue1);
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


