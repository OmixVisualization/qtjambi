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
// QLinkedList<T>::append(const T & t)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QLinkedList__1_1qt_1QLinkedList_1append__JLjava_lang_Object_2J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jlong appendFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QLinkedList<T>::append(const T & t)")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QLinkedList<QVariant>));
        reinterpret_cast<QLinkedListAppendFunction>(appendFunction)(__jni_env, __qt_this, t0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// QLinkedList<T>::first() const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QLinkedList__1_1qt_1QLinkedList_1first__JJ)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jlong firstFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QLinkedList<T>::first() const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QLinkedList<QVariant>));
        return reinterpret_cast<QLinkedListFirstFunction>(firstFunction)(__jni_env, __qt_this);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

// QLinkedList<T>::last() const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QLinkedList__1_1qt_1QLinkedList_1last__JJ)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jlong lastFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QLinkedList<T>::last() const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QLinkedList<QVariant>));
        return reinterpret_cast<QLinkedListLastFunction>(lastFunction)(__jni_env, __qt_this);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

// QLinkedList<T>::begin() const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QLinkedList__1_1qt_1QLinkedList_1begin__JJ)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jlong beginFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QLinkedList<T>::begin() const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QLinkedList<QVariant>));
        return reinterpret_cast<QLinkedListBeginFunction>(beginFunction)(__jni_env, __this_nativeId, __qt_this);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

// QLinkedList<T>::clear()
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QLinkedList__1_1qt_1QLinkedList_1clear__JJ)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jlong clearFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QLinkedList<T>::clear()")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QLinkedList<QVariant>));
        return reinterpret_cast<QLinkedListClearFunction>(clearFunction)(__jni_env, __qt_this);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// QLinkedList<T>::contains(const T & t) const
extern "C" Q_DECL_EXPORT jboolean JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QLinkedList__1_1qt_1QLinkedList_1contains__JLjava_lang_Object_2J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jlong containsFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QLinkedList<T>::contains(const T & t) const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QLinkedList<QVariant>));
        return reinterpret_cast<QLinkedListContainsFunction>(containsFunction)(__jni_env, __qt_this, t0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return false;
}

// QLinkedList<T>::count(const T & t) const
extern "C" Q_DECL_EXPORT jint JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QLinkedList__1_1qt_1QLinkedList_1count__JLjava_lang_Object_2J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jlong countObjectFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QLinkedList<T>::count(const T & t) const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QLinkedList<QVariant>));
        return reinterpret_cast<QLinkedListCountObjectFunction>(countObjectFunction)(__jni_env, __qt_this, t0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return 0;
}

// QLinkedList<T>::end() const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QLinkedList__1_1qt_1QLinkedList_1end__JJ)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jlong endFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QLinkedList<T>::end() const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QLinkedList<QVariant>));
        return reinterpret_cast<QLinkedListEndFunction>(endFunction)(__jni_env, __this_nativeId, __qt_this);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

// QLinkedList<T>::endsWith(const T & t) const
extern "C" Q_DECL_EXPORT jboolean JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QLinkedList__1_1qt_1QLinkedList_1endsWith__JLjava_lang_Object_2J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jlong endsWithFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QLinkedList<T>::endsWith(const T & t) const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QLinkedList<QVariant>));
        return reinterpret_cast<QLinkedListEndsWithFunction>(endsWithFunction)(__jni_env, __qt_this, t0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return false;
}

// QLinkedList<T>::operator==(const QLinkedList & l) const
extern "C" Q_DECL_EXPORT jboolean JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QLinkedList__1_1qt_1QLinkedList_1operator_1equal__JLjava_util_Collection_2J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject l0,
 jlong equalFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QLinkedList<T>::operator==(const QLinkedList<T> & l) const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QLinkedList<QVariant>));
        return reinterpret_cast<QLinkedListEqualFunction>(equalFunction)(__jni_env, __qt_this, l0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return false;
}

// QLinkedList<T>::prepend(const T & t)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QLinkedList__1_1qt_1QLinkedList_1prepend__JLjava_lang_Object_2J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jlong prependFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QLinkedList<T>::prepend(const T & t)")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QLinkedList<QVariant>));
        reinterpret_cast<QLinkedListPrependFunction>(prependFunction)(__jni_env, __qt_this, t0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// QLinkedList<T>::removeAll(const T & t)
extern "C" Q_DECL_EXPORT jint JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QLinkedList__1_1qt_1QLinkedList_1removeAll__JLjava_lang_Object_2J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jlong removeAllFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QLinkedList<T>::removeAll(const T & t)")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QLinkedList<QVariant>));
        return reinterpret_cast<QLinkedListRemoveAllFunction>(removeAllFunction)(__jni_env, __qt_this, t0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return -1;
}

// QLinkedList<T>::removeFirst()
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QLinkedList__1_1qt_1QLinkedList_1removeFirst__JJ)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jlong removeFirstFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QLinkedList<T>::removeFirst()")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QLinkedList<QVariant>));
        reinterpret_cast<QLinkedListRemoveFirstFunction>(removeFirstFunction)(__jni_env, __qt_this);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// QLinkedList<T>::removeLast()
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QLinkedList__1_1qt_1QLinkedList_1removeLast__JJ)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jlong removeLastFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QLinkedList<T>::removeLast()")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QLinkedList<QVariant>));
        reinterpret_cast<QLinkedListRemoveLastFunction>(removeLastFunction)(__jni_env, __qt_this);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// QLinkedList<T>::removeOne(const T & t)
extern "C" Q_DECL_EXPORT jboolean JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QLinkedList__1_1qt_1QLinkedList_1removeOne__JLjava_lang_Object_2J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jlong removeOneFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QLinkedList<T>::removeOne(const T & t)")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QLinkedList<QVariant>));
        return reinterpret_cast<QLinkedListRemoveOneFunction>(removeOneFunction)(__jni_env, __qt_this, t0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return false;
}

// QLinkedList<T>::size() const
extern "C" Q_DECL_EXPORT jint JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QLinkedList__1_1qt_1QLinkedList_1size__JJ)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jlong sizeFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QLinkedList<T>::size() const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QLinkedList<QVariant>));
        return reinterpret_cast<QLinkedListSizeFunction>(sizeFunction)(__jni_env, __qt_this);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return 0;
}

// QLinkedList<T>::startsWith(const T & t) const
extern "C" Q_DECL_EXPORT jboolean JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QLinkedList__1_1qt_1QLinkedList_1startsWith__JLjava_lang_Object_2J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jlong startsWithFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QLinkedList<T>::startsWith(const T & t) const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QLinkedList<QVariant>));
        return reinterpret_cast<QLinkedListStartsWithFunction>(startsWithFunction)(__jni_env, __qt_this, t0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return false;
}

// QLinkedList<T>::takeFirst()
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QLinkedList__1_1qt_1QLinkedList_1takeFirst__JJ)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jlong takeFirstFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QLinkedList<T>::takeFirst()")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QLinkedList<QVariant>));
        return reinterpret_cast<QLinkedListTakeFirstFunction>(takeFirstFunction)(__jni_env, __qt_this);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

// QLinkedList<T>::takeLast()
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QLinkedList__1_1qt_1QLinkedList_1takeLast__JJ)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jlong takeLastFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QLinkedList<T>::takeLast()")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QLinkedList<QVariant>));
        return reinterpret_cast<QLinkedListTakeLastFunction>(takeLastFunction)(__jni_env, __qt_this);
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


