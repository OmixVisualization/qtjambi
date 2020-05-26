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
// QSet<T>::insert(const T & t)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QSet__1_1qt_1QSet_1insert__JLjava_lang_Object_2J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jlong insertFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QSet<T>::insert(const T & t)")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QSet<QVariant>));
        reinterpret_cast<QSetInsertFunction>(insertFunction)(__jni_env, __qt_this, t0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// QSet<T>::begin() const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QSet__1_1qt_1QSet_1begin__JJ)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jlong beginFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QSet<T>::begin() const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QSet<QVariant>));
        return reinterpret_cast<QSetBeginFunction>(beginFunction)(__jni_env, __this_nativeId, __qt_this);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

// QSet<T>::capacity() const
extern "C" Q_DECL_EXPORT jint JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QSet__1_1qt_1QSet_1capacity__JJ)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jlong capacityFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QSet<T>::begin() const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QSet<QVariant>));
        return reinterpret_cast<QSetCapacityFunction>(capacityFunction)(__jni_env, __qt_this);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return 0;
}

// QSet<T>::clear()
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QSet__1_1qt_1QSet_1clear__JJ)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jlong clearFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QSet<T>::clear()")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QSet<QVariant>));
        return reinterpret_cast<QSetClearFunction>(clearFunction)(__jni_env, __qt_this);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// QSet<T>::contains(const T & t) const
extern "C" Q_DECL_EXPORT jboolean JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QSet__1_1qt_1QSet_1contains__JLjava_lang_Object_2J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jlong containsFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QSet<T>::contains(const T & t) const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QSet<QVariant>));
        return reinterpret_cast<QSetContainsFunction>(containsFunction)(__jni_env, __qt_this, t0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return false;
}

// QSet<T>::intersect(const QSet<T> &other)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QSet__1_1qt_1QSet_1intersect__JLjava_util_Collection_2J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jlong intersectFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QSet<T>::intersect(const QSet<T> &other)")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QSet<QVariant>));
        reinterpret_cast<QSetIntersectFunction>(intersectFunction)(__jni_env, __qt_this, t0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// QSet<T>::unite(const QSet<T> &other)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QSet__1_1qt_1QSet_1unite__JLjava_util_Collection_2J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jlong uniteFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QSet<T>::unite(const QSet<T> &other)")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QSet<QVariant>));
        reinterpret_cast<QSetUniteFunction>(uniteFunction)(__jni_env, __qt_this, t0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// QSet<T>::subtract(const QSet<T> &other)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QSet__1_1qt_1QSet_1subtract__JLjava_util_Collection_2J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jlong subtractFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QSet<T>::subtract(const QSet<T> &other)")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QSet<QVariant>));
        reinterpret_cast<QSetSubtractFunction>(subtractFunction)(__jni_env, __qt_this, t0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// QSet<T>::intersects(const QSet<T> &other) const
extern "C" Q_DECL_EXPORT jboolean JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QSet__1_1qt_1QSet_1intersects__JLjava_util_Collection_2J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jlong intersectsFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QSet<T>::intersects(const QSet<T> &other) const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QSet<QVariant>));
        return reinterpret_cast<QSetIntersectsFunction>(intersectsFunction)(__jni_env, __qt_this, t0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return false;
}

// QSet<T>::end() const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QSet__1_1qt_1QSet_1end__JJ)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jlong endFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QSet<T>::end() const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QSet<QVariant>));
        return reinterpret_cast<QSetEndFunction>(endFunction)(__jni_env, __this_nativeId, __qt_this);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

// QSet<T>::operator==(const QSet & l) const
extern "C" Q_DECL_EXPORT jboolean JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QSet__1_1qt_1QSet_1operator_1equal__JLjava_util_Collection_2J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject l0,
 jlong equalFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QSet<T>::operator==(const QSet<T> & l) const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QSet<QVariant>));
        return reinterpret_cast<QSetEqualFunction>(equalFunction)(__jni_env, __qt_this, l0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return false;
}

// QSet<T>::remove(const T & t)
extern "C" Q_DECL_EXPORT jboolean JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QSet__1_1qt_1QSet_1remove__JLjava_lang_Object_2J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jlong removeFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QSet<T>::remove(const T & t)")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QSet<QVariant>));
        return reinterpret_cast<QSetRemoveFunction>(removeFunction)(__jni_env, __qt_this, t0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return false;
}

// QSet<T>::reserve(int size)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QSet__1_1qt_1QSet_1reserve__JIJ)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jint size0,
 jlong reserveFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QSet<T>::reserve(int size)")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QSet<QVariant>));
        reinterpret_cast<QSetReserveFunction>(reserveFunction)(__jni_env, __qt_this, size0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// QSet<T>::size() const
extern "C" Q_DECL_EXPORT jint JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QSet__1_1qt_1QSet_1size__JJ)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jlong sizeFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QSet<T>::size() const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QSet<QVariant>));
        return reinterpret_cast<QSetSizeFunction>(sizeFunction)(__jni_env, __qt_this);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return 0;
}

// QSet<T>::values() const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QSet__1_1qt_1QSet_1values__JJ)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jlong valuesFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QSet<T>::values() const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QSet<QVariant>));
        return reinterpret_cast<QSetValuesFunction>(valuesFunction)(__jni_env, __qt_this);
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


