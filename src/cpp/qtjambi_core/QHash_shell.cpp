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
// QHash<Key, T>::begin() const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QHash__1_1qt_1QHash_1begin__JJ)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jlong beginFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "begin() const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QHash<QVariant,QVariant>));
        return reinterpret_cast<QHashBeginFunction>(beginFunction)(__jni_env, __this_nativeId, __qt_this);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

// QHash<Key, T>::capacity() const
extern "C" Q_DECL_EXPORT jint JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QHash__1_1qt_1QHash_1capacity__JJ)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jlong capacityFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QHash<Key, T>::capacity() const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QHash<QVariant,QVariant>));
        return reinterpret_cast<QHashCapacityFunction>(capacityFunction)(__jni_env, __qt_this);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return 0;
}

// QHash<Key, T>::clear()
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QHash__1_1qt_1QHash_1clear__JJ)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jlong clearFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QHash<Key, T>::clear()")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QHash<QVariant,QVariant>));
        return reinterpret_cast<QHashClearFunction>(clearFunction)(__jni_env, __qt_this);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// QHash<Key, T>::contains(const Key & k) const
extern "C" Q_DECL_EXPORT jboolean JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QHash__1_1qt_1QHash_1contains__JLjava_lang_Object_2J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jlong containsFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QHash<Key, T>::contains(const Key & k) const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QHash<QVariant,QVariant>));
        return reinterpret_cast<QHashContainsFunction>(containsFunction)(__jni_env, __qt_this, t0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return false;
}

// QHash<Key, T>::count(const Key & k) const
extern "C" Q_DECL_EXPORT jint JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QHash__1_1qt_1QHash_1count__JLjava_lang_Object_2J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jlong countObjectFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QHash<Key, T>::count(const Key & k) const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QHash<QVariant,QVariant>));
        return reinterpret_cast<QHashCountObjectFunction>(countObjectFunction)(__jni_env, __qt_this, t0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return 0;
}

// QHash<Key, T>::end() const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QHash__1_1qt_1QHash_1end__JJ)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jlong endFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QHash<Key, T>::end() const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QHash<QVariant,QVariant>));
        return reinterpret_cast<QHashEndFunction>(endFunction)(__jni_env, __this_nativeId, __qt_this);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

// QHash<Key, T>::find(const Key & k) const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QHash__1_1qt_1QHash_1find__JLjava_lang_Object_2J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jlong countObjectFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QHash<Key, T>::find(const Key & k) const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QHash<QVariant,QVariant>));
        return reinterpret_cast<QHashFindFunction>(countObjectFunction)(__jni_env, __this_nativeId, __qt_this, t0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

// QHash<Key, T>::insert(const K & k, const T & t)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QHash__1_1qt_1QHash_1insert__JLjava_lang_Object_2Ljava_lang_Object_2J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject k0,
 jobject t1,
 jlong insertFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QHash<Key, T>::insert(const K & k, const T & t)")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QHash<QVariant,QVariant>));
        reinterpret_cast<QHashInsertFunction>(insertFunction)(__jni_env, __qt_this, k0, t1);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// QHash<Key, T>::key(const V & value, const K & defaultKey) const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QHash__1_1qt_1QHash_1key__JLjava_lang_Object_2Ljava_lang_Object_2J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jobject k1,
 jlong keyFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QHash<Key, T>::key(const V & value, const K & defaultKey) const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QHash<QVariant,QVariant>));
        return reinterpret_cast<QHashKeyFunction>(keyFunction)(__jni_env, __qt_this, t0, k1);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

// QHash<Key, T>::keys(const T & value) const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QHash__1_1qt_1QHash_1keysForValue__JLjava_lang_Object_2J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jlong keysForValueFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QHash<Key, T>::keys(const T & value) const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QHash<QVariant,QVariant>));
        return reinterpret_cast<QHashKeysForValueFunction>(keysForValueFunction)(__jni_env, __qt_this, t0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

// QHash<Key, T>::keys() const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QHash__1_1qt_1QHash_1keys__JJ)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jlong keysFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QHash<Key, T>::keys() const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QHash<QVariant,QVariant>));
        return reinterpret_cast<QHashKeysFunction>(keysFunction)(__jni_env, __qt_this);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

// QHash<Key, T>::operator==(const QHash & l) const
extern "C" Q_DECL_EXPORT jboolean JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QHash__1_1qt_1QHash_1operator_1equal__JLjava_util_Map_2J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject l0,
 jlong equalFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QHash<Key, T>::operator==(const QHash<Key, T> & other) const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QHash<QVariant,QVariant>));
        return reinterpret_cast<QHashEqualFunction>(equalFunction)(__jni_env, __qt_this, l0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return false;
}

// QHash<Key, T>::remove(const T & t)
extern "C" Q_DECL_EXPORT jint JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QHash__1_1qt_1QHash_1remove__JLjava_lang_Object_2J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jlong removeAllFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QHash<Key, T>::remove(const T & t)")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QHash<QVariant,QVariant>));
        return reinterpret_cast<QHashRemoveAllFunction>(removeAllFunction)(__jni_env, __qt_this, t0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return -1;
}

// QHash<Key, T>::reserve(int size)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QHash__1_1qt_1QHash_1reserve__JIJ)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jint size0,
 jlong reserveFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QHash<Key, T>::reserve(int size)")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QHash<QVariant,QVariant>));
        reinterpret_cast<QHashReserveFunction>(reserveFunction)(__jni_env, __qt_this, size0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// QHash<Key, T>::size() const
extern "C" Q_DECL_EXPORT jint JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QHash__1_1qt_1QHash_1size__JJ)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jlong sizeFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QHash<Key, T>::size() const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QHash<QVariant,QVariant>));
        return reinterpret_cast<QHashSizeFunction>(sizeFunction)(__jni_env, __qt_this);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return 0;
}

// QHash<Key, T>::take(const T & t)
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QHash__1_1qt_1QHash_1take__JLjava_lang_Object_2J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jlong takeFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QHash<Key, T>::take(const T & t)")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QHash<QVariant,QVariant>));
        return reinterpret_cast<QHashTakeFunction>(takeFunction)(__jni_env, __qt_this, t0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

// QHash<Key, T>::uniqueKeys() const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QHash__1_1qt_1QHash_1uniqueKeys__JJ)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jlong uniqueKeysFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QHash<Key, T>::uniqueKeys() const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QHash<QVariant,QVariant>));
        return reinterpret_cast<QHashUniqueKeysFunction>(uniqueKeysFunction)(__jni_env, __qt_this);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

// QHash<Key, T>::unite(const T & t)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QHash__1_1qt_1QHash_1unite__JLjava_lang_Object_2J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jlong uniteFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QHash<Key, T>::unite(const T & t)")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QHash<QVariant,QVariant>));
        reinterpret_cast<QHashUniteFunction>(uniteFunction)(__jni_env, __qt_this, t0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// QHash<Key, T>::value(const Key &key, const T &defaultValue) const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QHash__1_1qt_1QHash_1value__JLjava_lang_Object_2Ljava_lang_Object_2J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject k0,
 jobject t1,
 jlong valueFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "value(const Key &key, const T &defaultValue) const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QHash<QVariant,QVariant>));
        return reinterpret_cast<QHashValueFunction>(valueFunction)(__jni_env, __qt_this, k0, t1);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

// QHash<Key, T>::values(const Key &key) const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QHash__1_1qt_1QHash_1valuesKey__JLjava_lang_Object_2J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject k0,
 jlong valuesKeyFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "values(const Key &key) const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QHash<QVariant,QVariant>));
        return reinterpret_cast<QHashValuesKeyFunction>(valuesKeyFunction)(__jni_env, __qt_this, k0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

// QHash<Key, T>::values() const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QHash__1_1qt_1QHash_1values__JJ)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jlong valuesFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "values() const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QHash<QVariant,QVariant>));
        return reinterpret_cast<QHashValuesFunction>(valuesFunction)(__jni_env, __qt_this);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

// QHash<Key, T>::contains(const Key & k, const T & value) const
extern "C" Q_DECL_EXPORT jboolean JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMultiHash__1_1qt_1QMultiHash_1contains__JLjava_lang_Object_2Ljava_lang_Object_2J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jobject k1,
 jlong containsFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QHash<Key, T>::contains(const Key & k, const T & value) const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QMultiHash<QVariant,QVariant>));
        return reinterpret_cast<QMultiHashContainsPairFunction>(containsFunction)(__jni_env, __qt_this, t0, k1);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return false;
}

// QHash<Key, T>::count(const Key & k, const T & value) const
extern "C" Q_DECL_EXPORT jint JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMultiHash__1_1qt_1QMultiHash_1count__JLjava_lang_Object_2Ljava_lang_Object_2J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jobject k1,
 jlong countFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QHash<Key, T>::contains(const Key & k, const T & value) const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QMultiHash<QVariant,QVariant>));
        return reinterpret_cast<QMultiHashCountPairFunction>(countFunction)(__jni_env, __qt_this, t0, k1);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return false;
}

// QHash<Key, T>::find(const Key & k, const T & value) const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMultiHash__1_1qt_1QMultiHash_1find__JLjava_lang_Object_2Ljava_lang_Object_2J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jobject k1,
 jlong findFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QHash<Key, T>::find(const Key & k, const T & value) const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QMultiHash<QVariant,QVariant>));
        return reinterpret_cast<QMultiHashFindPairFunction>(findFunction)(__jni_env, __this_nativeId, __qt_this, t0, k1);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

// QHash<Key, T>::remove(const Key & k, const T & value)
extern "C" Q_DECL_EXPORT jint JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMultiHash__1_1qt_1QMultiHash_1remove__JLjava_lang_Object_2Ljava_lang_Object_2J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jobject k1,
 jlong removeFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QHash<Key, T>::remove(const Key & k, const T & value)")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QMultiHash<QVariant,QVariant>));
        return reinterpret_cast<QMultiHashRemovePairFunction>(removeFunction)(__jni_env, __qt_this, t0, k1);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return false;
}

// QHash<Key, T>::replace(const Key & k, const T & value)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMultiHash__1_1qt_1QMultiHash_1replace__JLjava_lang_Object_2Ljava_lang_Object_2J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jobject k1,
 jlong replaceFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QHash<Key, T>::replace(const Key & k, const T & value)")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QMultiHash<QVariant,QVariant>));
        reinterpret_cast<QMultiHashReplaceFunction>(replaceFunction)(__jni_env, __qt_this, t0, k1);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// emitting (AbstractMetaClass::NormalFunctions|AbstractMetaClass::AbstractFunctions writeFinalFunction)
// emitting Field accessors (writeFieldAccessors)
// emitting (writeInterfaceCastFunction)
// emitting (writeSignalInitialization)
// emitting (writeJavaLangObjectOverrideFunctions)


