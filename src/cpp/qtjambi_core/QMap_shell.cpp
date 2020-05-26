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
// QMap<Key, T>::begin() const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMap__1_1qt_1QMap_1begin__JJ)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jlong beginFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "begin() const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QMap<QVariant,QVariant>));
        return reinterpret_cast<QMapBeginFunction>(beginFunction)(__jni_env, __this_nativeId, __qt_this);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

// QMap<Key, T>::clear()
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMap__1_1qt_1QMap_1clear__JJ)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jlong clearFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QMap<Key, T>::clear()")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QMap<QVariant,QVariant>));
        return reinterpret_cast<QMapClearFunction>(clearFunction)(__jni_env, __qt_this);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// QMap<Key, T>::contains(const Key & k) const
extern "C" Q_DECL_EXPORT jboolean JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMap__1_1qt_1QMap_1contains__JLjava_lang_Object_2J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jlong containsFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QMap<Key, T>::contains(const Key & k) const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QMap<QVariant,QVariant>));
        return reinterpret_cast<QMapContainsFunction>(containsFunction)(__jni_env, __qt_this, t0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return false;
}

// QMap<Key, T>::count(const Key & k) const
extern "C" Q_DECL_EXPORT jint JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMap__1_1qt_1QMap_1count__JLjava_lang_Object_2J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jlong countObjectFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QMap<Key, T>::count(const Key & k) const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QMap<QVariant,QVariant>));
        return reinterpret_cast<QMapCountObjectFunction>(countObjectFunction)(__jni_env, __qt_this, t0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return 0;
}

// QMap<Key, T>::end() const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMap__1_1qt_1QMap_1end__JJ)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jlong endFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QMap<Key, T>::end() const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QMap<QVariant,QVariant>));
        return reinterpret_cast<QMapEndFunction>(endFunction)(__jni_env, __this_nativeId, __qt_this);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

// QMap<Key, T>::find(const Key & k) const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMap__1_1qt_1QMap_1find__JLjava_lang_Object_2J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jlong countObjectFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QMap<Key, T>::find(const Key & k) const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QMap<QVariant,QVariant>));
        return reinterpret_cast<QMapFindFunction>(countObjectFunction)(__jni_env, __this_nativeId, __qt_this, t0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

// QMap<Key, T>::first() const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMap__1_1qt_1QMap_1first__JJ)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jlong firstFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QMap<Key, T>::first() const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QMap<QVariant,QVariant>));
        return reinterpret_cast<QMapFirstFunction>(firstFunction)(__jni_env, __qt_this);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

// QMap<Key, T>::firstKey() const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMap__1_1qt_1QMap_1firstKey__JJ)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jlong firstKeyFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QMap<Key, T>::firstKey() const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QMap<QVariant,QVariant>));
        return reinterpret_cast<QMapFirstKeyFunction>(firstKeyFunction)(__jni_env, __qt_this);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

// QMap<Key, T>::insert(const K & k, const T & t)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMap__1_1qt_1QMap_1insert__JLjava_lang_Object_2Ljava_lang_Object_2J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject k0,
 jobject t1,
 jlong insertFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QMap<Key, T>::insert(const K & k, const T & t)")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QMap<QVariant,QVariant>));
        reinterpret_cast<QMapInsertFunction>(insertFunction)(__jni_env, __qt_this, k0, t1);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// QMap<Key, T>::key(const V & value, const K & defaultKey) const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMap__1_1qt_1QMap_1key__JLjava_lang_Object_2Ljava_lang_Object_2J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jobject k1,
 jlong keyFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QMap<Key, T>::key(const V & value, const K & defaultKey) const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QMap<QVariant,QVariant>));
        return reinterpret_cast<QMapKeyFunction>(keyFunction)(__jni_env, __qt_this, t0, k1);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

// QMap<Key, T>::keys(const T & value) const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMap__1_1qt_1QMap_1keysForValue__JLjava_lang_Object_2J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jlong keysForValueFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QMap<Key, T>::keys(const T & value) const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QMap<QVariant,QVariant>));
        return reinterpret_cast<QMapKeysForValueFunction>(keysForValueFunction)(__jni_env, __qt_this, t0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

// QMap<Key, T>::keys() const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMap__1_1qt_1QMap_1keys__JJ)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jlong keysFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QMap<Key, T>::keys() const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QMap<QVariant,QVariant>));
        return reinterpret_cast<QMapKeysFunction>(keysFunction)(__jni_env, __qt_this);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

// QMap<Key, T>::last() const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMap__1_1qt_1QMap_1last__JJ)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jlong lastFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QMap<Key, T>::last() const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QMap<QVariant,QVariant>));
        return reinterpret_cast<QMapLastFunction>(lastFunction)(__jni_env, __qt_this);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

// QMap<Key, T>::lastKey() const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMap__1_1qt_1QMap_1lastKey__JJ)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jlong lastKeyFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QMap<Key, T>::lastKey() const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QMap<QVariant,QVariant>));
        return reinterpret_cast<QMapLastKeyFunction>(lastKeyFunction)(__jni_env, __qt_this);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

// QMap<Key, T>::lowerBound(const Key & k) const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMap__1_1qt_1QMap_1lowerBound__JLjava_lang_Object_2J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jlong lowerBoundFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QMap<Key, T>::lowerBound(const Key & k) const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QMap<QVariant,QVariant>));
        return reinterpret_cast<QMapLowerBoundFunction>(lowerBoundFunction)(__jni_env, __this_nativeId, __qt_this, t0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

// QMap<Key, T>::upperBound(const Key & k) const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMap__1_1qt_1QMap_1upperBound__JLjava_lang_Object_2J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jlong upperBoundFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QMap<Key, T>::upperBound(const Key & k) const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QMap<QVariant,QVariant>));
        return reinterpret_cast<QMapUpperBoundFunction>(upperBoundFunction)(__jni_env, __this_nativeId, __qt_this, t0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

// QMap<Key, T>::operator==(const QMap & l) const
extern "C" Q_DECL_EXPORT jboolean JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMap__1_1qt_1QMap_1operator_1equal__JLjava_util_Map_2J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject l0,
 jlong equalFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QMap<Key, T>::operator==(const QMap<Key, T> & other) const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QMap<QVariant,QVariant>));
        return reinterpret_cast<QMapEqualFunction>(equalFunction)(__jni_env, __qt_this, l0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return false;
}

// QMap<Key, T>::remove(const T & t)
extern "C" Q_DECL_EXPORT jint JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMap__1_1qt_1QMap_1remove__JLjava_lang_Object_2J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jlong removeAllFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QMap<Key, T>::remove(const T & t)")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QMap<QVariant,QVariant>));
        return reinterpret_cast<QMapRemoveAllFunction>(removeAllFunction)(__jni_env, __qt_this, t0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return -1;
}

// QMap<Key, T>::size() const
extern "C" Q_DECL_EXPORT jint JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMap__1_1qt_1QMap_1size__JJ)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jlong sizeFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QMap<Key, T>::size() const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QMap<QVariant,QVariant>));
        return reinterpret_cast<QMapSizeFunction>(sizeFunction)(__jni_env, __qt_this);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return 0;
}

// QMap<Key, T>::take(const T & t)
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMap__1_1qt_1QMap_1take__JLjava_lang_Object_2J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jlong takeFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QMap<Key, T>::take(const T & t)")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QMap<QVariant,QVariant>));
        return reinterpret_cast<QMapTakeFunction>(takeFunction)(__jni_env, __qt_this, t0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

// QMap<Key, T>::uniqueKeys() const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMap__1_1qt_1QMap_1uniqueKeys__JJ)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jlong uniqueKeysFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QMap<Key, T>::uniqueKeys() const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QMap<QVariant,QVariant>));
        return reinterpret_cast<QMapUniqueKeysFunction>(uniqueKeysFunction)(__jni_env, __qt_this);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

// QMap<Key, T>::unite(const T & t)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMap__1_1qt_1QMap_1unite__JLjava_lang_Object_2J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jlong uniteFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QMap<Key, T>::unite(const T & t)")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QMap<QVariant,QVariant>));
        reinterpret_cast<QMapUniteFunction>(uniteFunction)(__jni_env, __qt_this, t0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// QMap<Key, T>::value(const Key &key, const T &defaultValue) const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMap__1_1qt_1QMap_1value__JLjava_lang_Object_2Ljava_lang_Object_2J)
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
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QMap<QVariant,QVariant>));
        return reinterpret_cast<QMapValueFunction>(valueFunction)(__jni_env, __qt_this, k0, t1);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

// QMap<Key, T>::values(const Key &key) const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMap__1_1qt_1QMap_1valuesKey__JLjava_lang_Object_2J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject k0,
 jlong valuesKeyFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "values(const Key &key) const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QMap<QVariant,QVariant>));
        return reinterpret_cast<QMapValuesKeyFunction>(valuesKeyFunction)(__jni_env, __qt_this, k0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

// QMap<Key, T>::values() const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMap__1_1qt_1QMap_1values__JJ)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jlong valuesFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "values() const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QMap<QVariant,QVariant>));
        return reinterpret_cast<QMapValuesFunction>(valuesFunction)(__jni_env, __qt_this);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

// QMap<Key, T>::contains(const Key & k, const T & value) const
extern "C" Q_DECL_EXPORT jboolean JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMultiMap__1_1qt_1QMultiMap_1contains__JLjava_lang_Object_2Ljava_lang_Object_2J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jobject k1,
 jlong containsFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QMap<Key, T>::contains(const Key & k, const T & value) const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QMap<QVariant,QVariant>));
        return reinterpret_cast<QMultiMapContainsPairFunction>(containsFunction)(__jni_env, __qt_this, t0, k1);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return false;
}

// QMap<Key, T>::count(const Key & k, const T & value) const
extern "C" Q_DECL_EXPORT jint JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMultiMap__1_1qt_1QMultiMap_1count__JLjava_lang_Object_2Ljava_lang_Object_2J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jobject k1,
 jlong countFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QMap<Key, T>::contains(const Key & k, const T & value) const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QMap<QVariant,QVariant>));
        return reinterpret_cast<QMultiMapCountPairFunction>(countFunction)(__jni_env, __qt_this, t0, k1);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return false;
}

// QMap<Key, T>::find(const Key & k, const T & value) const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMultiMap__1_1qt_1QMultiMap_1find__JLjava_lang_Object_2Ljava_lang_Object_2J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jobject k1,
 jlong findFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QMap<Key, T>::find(const Key & k, const T & value) const")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QMap<QVariant,QVariant>));
        return reinterpret_cast<QMultiMapFindPairFunction>(findFunction)(__jni_env, __this_nativeId, __qt_this, t0, k1);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

// QMap<Key, T>::remove(const Key & k, const T & value)
extern "C" Q_DECL_EXPORT jint JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMultiMap__1_1qt_1QMultiMap_1remove__JLjava_lang_Object_2Ljava_lang_Object_2J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jobject k1,
 jlong removeFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QMap<Key, T>::remove(const Key & k, const T & value)")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QMap<QVariant,QVariant>));
        return reinterpret_cast<QMultiMapRemovePairFunction>(removeFunction)(__jni_env, __qt_this, t0, k1);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return false;
}

// QMap<Key, T>::replace(const Key & k, const T & value)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMultiMap__1_1qt_1QMultiMap_1replace__JLjava_lang_Object_2Ljava_lang_Object_2J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jobject k1,
 jlong replaceFunction)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QMap<Key, T>::replace(const Key & k, const T & value)")
    try{
        void *__qt_this = qtjambi_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this, typeid(QMap<QVariant,QVariant>));
        reinterpret_cast<QMultiMapReplaceFunction>(replaceFunction)(__jni_env, __qt_this, t0, k1);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// emitting (AbstractMetaClass::NormalFunctions|AbstractMetaClass::AbstractFunctions writeFinalFunction)
// emitting Field accessors (writeFieldAccessors)
// emitting (writeInterfaceCastFunction)
// emitting (writeSignalInitialization)
// emitting (writeJavaLangObjectOverrideFunctions)


