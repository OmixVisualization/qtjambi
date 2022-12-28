/****************************************************************************
**
** Copyright (C) 2009-2022 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#include <QtCore/qcompilerdetection.h>
QT_WARNING_DISABLE_DEPRECATED
#include <QtCore/QDataStream>
#include <QtCore/QDebug>
#include <QtJambi/QtJambiAPI>
#include <QtJambi/JavaAPI>
#include <QtJambi/ContainerAPI>
#include <QtJambi/CoreAPI>
#include "utils_p.h"
#include <QtJambi/qtjambi_cast.h>

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMap_initialize)
(JNIEnv * env, jobject _this, jclass keyType, QtJambiNativeID keyMetaType, jclass valueType, QtJambiNativeID valueMetaType, jobject other)
{
    QTJAMBI_TRY{
        CoreAPI::initializeQMap(env, _this, keyType, keyMetaType, valueType, valueMetaType, other);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMultiMap_initialize)
(JNIEnv * env, jobject _this, jclass keyType, QtJambiNativeID keyMetaType, jclass valueType, QtJambiNativeID valueMetaType, jobject other)
{
    QTJAMBI_TRY{
        CoreAPI::initializeQMultiMap(env, _this, keyType, keyMetaType, valueType, valueMetaType, other);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT jboolean JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMap_lessThan)
(JNIEnv * env, jclass, jobject key1, jobject key2, jlong containerAccess)
{
    jboolean result{false};
    QTJAMBI_TRY{
        result = reinterpret_cast<AbstractMapAccess*>(containerAccess)->keyLessThan(env, key1, key2);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return result;
}

extern "C" Q_DECL_EXPORT jlong JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMap_clonedAccess)
(JNIEnv *env, jclass, QtJambiNativeID __this_nativeId)
{
    jlong result{0};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        AbstractMapAccess* containerAccess = dynamic_cast<AbstractMapAccess*>(container.second);
        Q_ASSERT(containerAccess);
        result = jlong(containerAccess->clone());
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return result;
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMap_disposeAccess)
(JNIEnv *env, jclass, jlong containerAccess)
{
    QTJAMBI_TRY{
        if(containerAccess)
            reinterpret_cast<AbstractContainerAccess*>(containerAccess)->dispose();
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMap_keyMetaType)
(JNIEnv * env, jclass, QtJambiNativeID __this_nativeId)
{
    jobject result{nullptr};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        AbstractMapAccess* containerAccess = dynamic_cast<AbstractMapAccess*>(container.second);
        Q_ASSERT(containerAccess);
        result = qtjambi_cast<jobject>(env, containerAccess->keyMetaType());
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return result;
}

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMap_valueMetaType)
(JNIEnv * env, jclass, QtJambiNativeID __this_nativeId)
{
    jobject result{nullptr};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        AbstractMapAccess* containerAccess = dynamic_cast<AbstractMapAccess*>(container.second);
        Q_ASSERT(containerAccess);
        result = qtjambi_cast<jobject>(env, containerAccess->valueMetaType());
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return result;
}

// emitting  (functionsInTargetLang writeFinalFunction)
// QMap<Key, T>::begin()
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMap_begin__J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId)
{
    jobject result{nullptr};
    QTJAMBI_DEBUG_METHOD_PRINT("native", "begin()")
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkPointer(__jni_env, container.first, typeid(QMap<QVariant,QVariant>));
        AbstractMapAccess* containerAccess = dynamic_cast<AbstractMapAccess*>(container.second);
        Q_ASSERT(containerAccess);
        result = containerAccess->begin(__jni_env, __this_nativeId, container.first);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QMap<Key, T>::end()
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMap_end__J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId)
{
    jobject result{nullptr};
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QMap<Key, T>::end()")
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkPointer(__jni_env, container.first, typeid(QMap<QVariant,QVariant>));
        AbstractMapAccess* containerAccess = dynamic_cast<AbstractMapAccess*>(container.second);
        Q_ASSERT(containerAccess);
        result = containerAccess->end(__jni_env, __this_nativeId, container.first);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QMap<Key, T>::constBegin() const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMap_constBegin__J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId)
{
    jobject result{nullptr};
    QTJAMBI_DEBUG_METHOD_PRINT("native", "constBegin() const")
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkPointer(__jni_env, container.first, typeid(QMap<QVariant,QVariant>));
        AbstractMapAccess* containerAccess = dynamic_cast<AbstractMapAccess*>(container.second);
        Q_ASSERT(containerAccess);
        result = containerAccess->constBegin(__jni_env, __this_nativeId, container.first);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QMap<Key, T>::end() const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMap_constEnd__J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId)
{
    jobject result{nullptr};
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QMap<Key, T>::constEnd() const")
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkPointer(__jni_env, container.first, typeid(QMap<QVariant,QVariant>));
        AbstractMapAccess* containerAccess = dynamic_cast<AbstractMapAccess*>(container.second);
        Q_ASSERT(containerAccess);
        result = containerAccess->constEnd(__jni_env, __this_nativeId, container.first);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QMap<Key, T>::clear()
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMap_clear__J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QMap<Key, T>::clear()")
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkPointer(__jni_env, container.first, typeid(QMap<QVariant,QVariant>));
        AbstractMapAccess* containerAccess = dynamic_cast<AbstractMapAccess*>(container.second);
        Q_ASSERT(containerAccess);
        containerAccess->clear(__jni_env, container.first);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

// QMap<Key, T>::contains(const Key & k) const
extern "C" Q_DECL_EXPORT jboolean JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMap_contains__JLjava_lang_Object_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0)
{
    jboolean result{false};
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QMap<Key, T>::contains(const Key & k) const")
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkPointer(__jni_env, container.first, typeid(QMap<QVariant,QVariant>));
        AbstractMapAccess* containerAccess = dynamic_cast<AbstractMapAccess*>(container.second);
        Q_ASSERT(containerAccess);
        result = containerAccess->contains(__jni_env, container.first, t0);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QMap<Key, T>::count(const Key & k) const
extern "C" Q_DECL_EXPORT jint JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMap_count__JLjava_lang_Object_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0)
{
    jint result{0};
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QMap<Key, T>::count(const Key & k) const")
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkPointer(__jni_env, container.first, typeid(QMap<QVariant,QVariant>));
        AbstractMapAccess* containerAccess = dynamic_cast<AbstractMapAccess*>(container.second);
        Q_ASSERT(containerAccess);
        result = containerAccess->count(__jni_env, container.first, t0);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QMap<Key, T>::find(const Key & k) const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMap_find__JLjava_lang_Object_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0)
{
    jobject result{nullptr};
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QMap<Key, T>::find(const Key & k) const")
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkPointer(__jni_env, container.first, typeid(QMap<QVariant,QVariant>));
        AbstractMapAccess* containerAccess = dynamic_cast<AbstractMapAccess*>(container.second);
        Q_ASSERT(containerAccess);
        result = containerAccess->constFind(__jni_env, __this_nativeId, container.first, t0);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QMap<Key, T>::first() const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMap_first__J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId)
{
    jobject result{nullptr};
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QMap<Key, T>::first() const")
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkPointer(__jni_env, container.first, typeid(QMap<QVariant,QVariant>));
        AbstractMapAccess* containerAccess = dynamic_cast<AbstractMapAccess*>(container.second);
        Q_ASSERT(containerAccess);
        result = containerAccess->first(__jni_env, container.first);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QMap<Key, T>::firstKey() const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMap_firstKey__J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QMap<Key, T>::firstKey() const")
    jobject result{nullptr};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkPointer(__jni_env, container.first, typeid(QMap<QVariant,QVariant>));
        AbstractMapAccess* containerAccess = dynamic_cast<AbstractMapAccess*>(container.second);
        Q_ASSERT(containerAccess);
        result = containerAccess->firstKey(__jni_env, container.first);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QMap<Key, T>::insert(const K & k, const T & t)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMap_insert__JLjava_lang_Object_2Ljava_lang_Object_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject k0,
 jobject t1)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QMap<Key, T>::insert(const K & k, const T & t)")
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkPointer(__jni_env, container.first, typeid(QMap<QVariant,QVariant>));
        AbstractMapAccess* containerAccess = dynamic_cast<AbstractMapAccess*>(container.second);
        Q_ASSERT(containerAccess);
        containerAccess->insert(__jni_env, container.first, k0, t1);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

// QMap<Key, T>::key(const V & value, const K & defaultKey) const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMap_key__JLjava_lang_Object_2Ljava_lang_Object_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jobject k1)
{
    jobject result{nullptr};
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QMap<Key, T>::key(const V & value, const K & defaultKey) const")
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkPointer(__jni_env, container.first, typeid(QMap<QVariant,QVariant>));
        AbstractMapAccess* containerAccess = dynamic_cast<AbstractMapAccess*>(container.second);
        Q_ASSERT(containerAccess);
        result = containerAccess->key(__jni_env, container.first, t0, k1);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QMap<Key, T>::keys(const T & value) const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMap_keysForValue__JLjava_lang_Object_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0)
{
    jobject result{nullptr};
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QMap<Key, T>::keys(const T & value) const")
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkPointer(__jni_env, container.first, typeid(QMap<QVariant,QVariant>));
        AbstractMapAccess* containerAccess = dynamic_cast<AbstractMapAccess*>(container.second);
        Q_ASSERT(containerAccess);
        result = containerAccess->keys(__jni_env, container.first, t0);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QMap<Key, T>::keys() const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMap_keys__J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId)
{
    jobject result{nullptr};
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QMap<Key, T>::keys() const")
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkPointer(__jni_env, container.first, typeid(QMap<QVariant,QVariant>));
        AbstractMapAccess* containerAccess = dynamic_cast<AbstractMapAccess*>(container.second);
        Q_ASSERT(containerAccess);
        result = containerAccess->keys(__jni_env, container.first);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QMap<Key, T>::last() const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMap_last__J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId)
{
    jobject result{nullptr};
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QMap<Key, T>::last() const")
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkPointer(__jni_env, container.first, typeid(QMap<QVariant,QVariant>));
        AbstractMapAccess* containerAccess = dynamic_cast<AbstractMapAccess*>(container.second);
        Q_ASSERT(containerAccess);
        result = containerAccess->last(__jni_env, container.first);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QMap<Key, T>::lastKey() const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMap_lastKey__J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId)
{
    jobject result{nullptr};
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QMap<Key, T>::lastKey() const")
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkPointer(__jni_env, container.first, typeid(QMap<QVariant,QVariant>));
        AbstractMapAccess* containerAccess = dynamic_cast<AbstractMapAccess*>(container.second);
        Q_ASSERT(containerAccess);
        result = containerAccess->lastKey(__jni_env, container.first);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QMap<Key, T>::lowerBound(const Key & k) const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMap_lowerBound__JLjava_lang_Object_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0)
{
    jobject result{nullptr};
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QMap<Key, T>::lowerBound(const Key & k) const")
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkPointer(__jni_env, container.first, typeid(QMap<QVariant,QVariant>));
        AbstractMapAccess* containerAccess = dynamic_cast<AbstractMapAccess*>(container.second);
        Q_ASSERT(containerAccess);
        result = containerAccess->constLowerBound(__jni_env, __this_nativeId, container.first, t0);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QMap<Key, T>::upperBound(const Key & k) const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMap_upperBound__JLjava_lang_Object_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0)
{
    jobject result{nullptr};
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QMap<Key, T>::upperBound(const Key & k) const")
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkPointer(__jni_env, container.first, typeid(QMap<QVariant,QVariant>));
        AbstractMapAccess* containerAccess = dynamic_cast<AbstractMapAccess*>(container.second);
        Q_ASSERT(containerAccess);
        result = containerAccess->constUpperBound(__jni_env, __this_nativeId, container.first, t0);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QMap<Key, T>::operator==(const QMap & l) const
extern "C" Q_DECL_EXPORT jboolean JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMap_operator_1equal__JLjava_util_Map_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject l0)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QMap<Key, T>::operator==(const QMap<Key, T> & other) const")
    jboolean result{false};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkPointer(__jni_env, container.first, typeid(QMap<QVariant,QVariant>));
        AbstractMapAccess* containerAccess = dynamic_cast<AbstractMapAccess*>(container.second);
        Q_ASSERT(containerAccess);
        result = containerAccess->equal(__jni_env, container.first, l0);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QMap<Key, T>::remove(const T & t)
extern "C" Q_DECL_EXPORT jint JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMap_remove__JLjava_lang_Object_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0)
{
    jint result{-1};
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QMap<Key, T>::remove(const T & t)")
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkPointer(__jni_env, container.first, typeid(QMap<QVariant,QVariant>));
        AbstractMapAccess* containerAccess = dynamic_cast<AbstractMapAccess*>(container.second);
        Q_ASSERT(containerAccess);
        result = containerAccess->remove(__jni_env, container.first, t0);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QMap<Key, T>::size() const
extern "C" Q_DECL_EXPORT jint JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMap_size__J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId)
{
    jint result{0};
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QMap<Key, T>::size() const")
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkPointer(__jni_env, container.first, typeid(QMap<QVariant,QVariant>));
        AbstractMapAccess* containerAccess = dynamic_cast<AbstractMapAccess*>(container.second);
        Q_ASSERT(containerAccess);
        result = containerAccess->size(__jni_env, container.first);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QMap<Key, T>::take(const T & t)
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMap_take__JLjava_lang_Object_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0)
{
    jobject result{nullptr};
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QMap<Key, T>::take(const T & t)")
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkPointer(__jni_env, container.first, typeid(QMap<QVariant,QVariant>));
        AbstractMapAccess* containerAccess = dynamic_cast<AbstractMapAccess*>(container.second);
        Q_ASSERT(containerAccess);
        result = containerAccess->take(__jni_env, container.first, t0);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QMap<Key, T>::value(const Key &key, const T &defaultValue) const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMap_value__JLjava_lang_Object_2Ljava_lang_Object_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject k0,
 jobject t1)
{
    jobject result{nullptr};
    QTJAMBI_DEBUG_METHOD_PRINT("native", "value(const Key &key, const T &defaultValue) const")
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkPointer(__jni_env, container.first, typeid(QMap<QVariant,QVariant>));
        AbstractMapAccess* containerAccess = dynamic_cast<AbstractMapAccess*>(container.second);
        Q_ASSERT(containerAccess);
        result = containerAccess->value(__jni_env, container.first, k0, t1);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QMap<Key, T>::values() const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMap_values__J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId)
{
    jobject result{nullptr};
    QTJAMBI_DEBUG_METHOD_PRINT("native", "values() const")
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkPointer(__jni_env, container.first, typeid(QMap<QVariant,QVariant>));
        AbstractMapAccess* containerAccess = dynamic_cast<AbstractMapAccess*>(container.second);
        Q_ASSERT(containerAccess);
        result = containerAccess->values(__jni_env, container.first);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMap_writeTo)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 QtJambiNativeID stream0)
{
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkPointer(__jni_env, container.first, typeid(QMap<QVariant,QVariant>));
        AbstractMapAccess* containerAccess = dynamic_cast<AbstractMapAccess*>(container.second);
        Q_ASSERT(containerAccess);
        QDataStream* stream = QtJambiAPI::objectFromNativeId<QDataStream>(stream0);
        QtJambiAPI::checkPointer(__jni_env, stream, typeid(QDataStream));
        QByteArray containerName = "QMap<";
        containerName += containerAccess->keyMetaType().name();
        containerName += ",";
        containerName += containerAccess->valueMetaType().name();
        containerName += ">";
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
        int metaType = containerAccess->registerContainer(containerName);
        if(!QMetaType::save(*stream, metaType, container.first)){
#else
        QMetaType metaType(containerAccess->registerContainer(containerName));
        if(!metaType.save(*stream, container.first)){
#endif
            containerName.prepend("QDataStream& << ");
            JavaException::raiseQNoImplementationException(__jni_env, containerName.constData() QTJAMBI_STACKTRACEINFO );
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMap_readFrom)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 QtJambiNativeID stream0)
{
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkPointer(__jni_env, container.first, typeid(QMap<QVariant,QVariant>));
        AbstractMapAccess* containerAccess = dynamic_cast<AbstractMapAccess*>(container.second);
        Q_ASSERT(containerAccess);
        QDataStream* stream = QtJambiAPI::objectFromNativeId<QDataStream>(stream0);
        QtJambiAPI::checkPointer(__jni_env, stream, typeid(QDataStream));
        QByteArray containerName = "QMap<";
        containerName += containerAccess->keyMetaType().name();
        containerName += ",";
        containerName += containerAccess->valueMetaType().name();
        containerName += ">";
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
        int metaType = containerAccess->registerContainer(containerName);
        if(!QMetaType::load(*stream, metaType, container.first)){
#else
        QMetaType metaType(containerAccess->registerContainer(containerName));
        if(!metaType.load(*stream, container.first)){
#endif
            containerName.prepend("QDataStream& >> ");
            JavaException::raiseQNoImplementationException(__jni_env, containerName.constData() QTJAMBI_STACKTRACEINFO );
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT jstring JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMap_toString)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId)
{
    jstring result{nullptr};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkPointer(__jni_env, container.first, typeid(QMap<QVariant,QVariant>));
        AbstractMapAccess* containerAccess = dynamic_cast<AbstractMapAccess*>(container.second);
        Q_ASSERT(containerAccess);
        QString strg;
        {
            QDebug stream(&strg);
            QByteArray containerName = "QMap<";
            containerName += containerAccess->keyMetaType().name();
            containerName += ",";
            containerName += containerAccess->valueMetaType().name();
            containerName += ">";
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
            int metaType = containerAccess->registerContainer(containerName);
            CoreAPI::appendToDebugStream(stream.nospace(), metaType, container.first);
            if(strg.isEmpty()){
                containerName.prepend("QDebug >> ");
                JavaException::raiseQNoImplementationException(__jni_env, containerName.constData() QTJAMBI_STACKTRACEINFO );
            }
#else
            QMetaType metaType(containerAccess->registerContainer(containerName));
            if(!metaType.debugStream(stream.nospace(), container.first)){
                containerName.prepend("QDebug >> ");
                JavaException::raiseQNoImplementationException(__jni_env, containerName QTJAMBI_STACKTRACEINFO );
            }
#endif
        }
        result = qtjambi_cast<jstring>(__jni_env, strg);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

extern "C" Q_DECL_EXPORT jint JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMap_hashCode)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId)
{
    jint result{0};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkPointer(__jni_env, container.first, typeid(QMap<QVariant,QVariant>));
        AbstractMapAccess* containerAccess = dynamic_cast<AbstractMapAccess*>(container.second);
        Q_ASSERT(containerAccess);
        QByteArray containerName = "QMap<";
        containerName += containerAccess->keyMetaType().name();
        containerName += ",";
        containerName += containerAccess->valueMetaType().name();
        containerName += ">";
        QMetaType metaType(containerAccess->registerContainer(containerName));
        hash_type h = CoreAPI::computeHash(metaType, container.first);
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
        result = jint(quint64(h) ^ quint64(h) >> 32);
#else
        result = jint(h);
#endif
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QMap<Key, T>::uniqueKeys() const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMultiMap_uniqueKeys__J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId)
{
    jobject result{nullptr};
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QMultiMap<Key, T>::uniqueKeys() const")
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkPointer(__jni_env, container.first, typeid(QMultiMap<QVariant,QVariant>));
        AbstractMultiMapAccess* containerAccess = dynamic_cast<AbstractMultiMapAccess*>(container.second);
        Q_ASSERT(containerAccess);
        result = containerAccess->uniqueKeys(__jni_env, container.first);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QMap<Key, T>::unite(const T & t)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMultiMap_unite__JLjava_lang_Object_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QMultiMap<Key, T>::unite(const T & t)")
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkPointer(__jni_env, container.first, typeid(QMultiMap<QVariant,QVariant>));
        AbstractMultiMapAccess* containerAccess = dynamic_cast<AbstractMultiMapAccess*>(container.second);
        Q_ASSERT(containerAccess);
        containerAccess->unite(__jni_env, container.first, t0);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

// QMap<Key, T>::values(const Key &key) const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMultiMap_valuesKey__JLjava_lang_Object_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject k0)
{
    jobject result{nullptr};
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QMultiMap<Key, T>::values(const Key &key) const")
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkPointer(__jni_env, container.first, typeid(QMultiMap<QVariant,QVariant>));
        AbstractMultiMapAccess* containerAccess = dynamic_cast<AbstractMultiMapAccess*>(container.second);
        Q_ASSERT(containerAccess);
        result = containerAccess->values(__jni_env, container.first, k0);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QMap<Key, T>::contains(const Key & k, const T & value) const
extern "C" Q_DECL_EXPORT jboolean JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMultiMap_contains__JLjava_lang_Object_2Ljava_lang_Object_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jobject k1)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QMultiMap<Key, T>::contains(const Key & k, const T & value) const")
    jboolean result{false};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkPointer(__jni_env, container.first, typeid(QMultiMap<QVariant,QVariant>));
        AbstractMultiMapAccess* containerAccess = dynamic_cast<AbstractMultiMapAccess*>(container.second);
        Q_ASSERT(containerAccess);
        result = containerAccess->contains(__jni_env, container.first, t0, k1);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QMap<Key, T>::count(const Key & k, const T & value) const
extern "C" Q_DECL_EXPORT jint JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMultiMap_count__JLjava_lang_Object_2Ljava_lang_Object_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jobject k1)
{
    jint result{0};
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QMultiMap<Key, T>::contains(const Key & k, const T & value) const")
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkPointer(__jni_env, container.first, typeid(QMultiMap<QVariant,QVariant>));
        AbstractMultiMapAccess* containerAccess = dynamic_cast<AbstractMultiMapAccess*>(container.second);
        Q_ASSERT(containerAccess);
        result = containerAccess->count(__jni_env, container.first, t0, k1);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QMap<Key, T>::find(const Key & k, const T & value) const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMultiMap_find__JLjava_lang_Object_2Ljava_lang_Object_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jobject k1)
{
    jobject result{nullptr};
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QMultiMap<Key, T>::find(const Key & k, const T & value) const")
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkPointer(__jni_env, container.first, typeid(QMultiMap<QVariant,QVariant>));
        AbstractMultiMapAccess* containerAccess = dynamic_cast<AbstractMultiMapAccess*>(container.second);
        Q_ASSERT(containerAccess);
        result = containerAccess->constFind(__jni_env, __this_nativeId, container.first, t0, k1);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QMap<Key, T>::remove(const Key & k, const T & value)
extern "C" Q_DECL_EXPORT jint JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMultiMap_remove__JLjava_lang_Object_2Ljava_lang_Object_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jobject k1)
{
    jint result{0};
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QMultiMap<Key, T>::remove(const Key & k, const T & value)")
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkPointer(__jni_env, container.first, typeid(QMultiMap<QVariant,QVariant>));
        AbstractMultiMapAccess* containerAccess = dynamic_cast<AbstractMultiMapAccess*>(container.second);
        Q_ASSERT(containerAccess);
        result = containerAccess->remove(__jni_env, container.first, t0, k1);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QMap<Key, T>::replace(const Key & k, const T & value)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMultiMap_replace__JLjava_lang_Object_2Ljava_lang_Object_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jobject k1)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QMultiMap<Key, T>::replace(const Key & k, const T & value)")
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkPointer(__jni_env, container.first, typeid(QMultiMap<QVariant,QVariant>));
        AbstractMultiMapAccess* containerAccess = dynamic_cast<AbstractMultiMapAccess*>(container.second);
        Q_ASSERT(containerAccess);
        containerAccess->replace(__jni_env, container.first, t0, k1);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMultiMap_writeTo)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 QtJambiNativeID stream0)
{
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkPointer(__jni_env, container.first, typeid(QMultiMap<QVariant,QVariant>));
        AbstractMultiMapAccess* containerAccess = dynamic_cast<AbstractMultiMapAccess*>(container.second);
        Q_ASSERT(containerAccess);
        QDataStream* stream = QtJambiAPI::objectFromNativeId<QDataStream>(stream0);
        QtJambiAPI::checkPointer(__jni_env, stream, typeid(QDataStream));
        QByteArray containerName = "QMultiMap<";
        containerName += containerAccess->keyMetaType().name();
        containerName += ",";
        containerName += containerAccess->valueMetaType().name();
        containerName += ">";
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
        int metaType = containerAccess->registerContainer(containerName);
        if(!QMetaType::save(*stream, metaType, container.first)){
#else
        QMetaType metaType(containerAccess->registerContainer(containerName));
        if(!metaType.save(*stream, container.first)){
#endif
            containerName.prepend("QDataStream& << ");
            JavaException::raiseQNoImplementationException(__jni_env, containerName.constData() QTJAMBI_STACKTRACEINFO );
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMultiMap_readFrom)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 QtJambiNativeID stream0)
{
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkPointer(__jni_env, container.first, typeid(QMultiMap<QVariant,QVariant>));
        AbstractMultiMapAccess* containerAccess = dynamic_cast<AbstractMultiMapAccess*>(container.second);
        Q_ASSERT(containerAccess);
        QDataStream* stream = QtJambiAPI::objectFromNativeId<QDataStream>(stream0);
        QtJambiAPI::checkPointer(__jni_env, stream, typeid(QDataStream));
        QByteArray containerName = "QMultiMap<";
        containerName += containerAccess->keyMetaType().name();
        containerName += ",";
        containerName += containerAccess->valueMetaType().name();
        containerName += ">";
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
        int metaType = containerAccess->registerContainer(containerName);
        if(!QMetaType::load(*stream, metaType, container.first)){
#else
        QMetaType metaType(containerAccess->registerContainer(containerName));
        if(!metaType.load(*stream, container.first)){
#endif
            containerName.prepend("QDataStream& >> ");
            JavaException::raiseQNoImplementationException(__jni_env, containerName.constData() QTJAMBI_STACKTRACEINFO );
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT jstring JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMultiMap_toString)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId)
{
    jstring result{nullptr};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkPointer(__jni_env, container.first, typeid(QMultiMap<QVariant,QVariant>));
        AbstractMultiMapAccess* containerAccess = dynamic_cast<AbstractMultiMapAccess*>(container.second);
        Q_ASSERT(containerAccess);
        QString strg;
        {
            QDebug stream(&strg);
            QByteArray containerName = "QMultiMap<";
            containerName += containerAccess->keyMetaType().name();
            containerName += ",";
            containerName += containerAccess->valueMetaType().name();
            containerName += ">";
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
            int metaType = containerAccess->registerContainer(containerName);
            CoreAPI::appendToDebugStream(stream.nospace(), metaType, container.first);
#else
            QMetaType metaType(containerAccess->registerContainer(containerName));
            if(!metaType.debugStream(stream.nospace(), container.first)){
                containerName.prepend("QDebug >> ");
                JavaException::raiseQNoImplementationException(__jni_env, containerName.constData() QTJAMBI_STACKTRACEINFO );
            }
#endif
        }
        result = qtjambi_cast<jstring>(__jni_env, strg);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

extern "C" Q_DECL_EXPORT jint JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMultiMap_hashCode)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId)
{
    jint result{0};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkPointer(__jni_env, container.first, typeid(QMultiMap<QVariant,QVariant>));
        AbstractMultiMapAccess* containerAccess = dynamic_cast<AbstractMultiMapAccess*>(container.second);
        Q_ASSERT(containerAccess);
        QByteArray containerName = "QMultiMap<";
        containerName += containerAccess->keyMetaType().name();
        containerName += ",";
        containerName += containerAccess->valueMetaType().name();
        containerName += ">";
        QMetaType metaType(containerAccess->registerContainer(containerName));
        hash_type h = CoreAPI::computeHash(metaType, container.first);
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
        result = jint(quint64(h) ^ quint64(h) >> 32);
#else
        result = jint(h);
#endif
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// emitting (AbstractMetaClass::NormalFunctions|AbstractMetaClass::AbstractFunctions writeFinalFunction)
// emitting Field accessors (writeFieldAccessors)
// emitting (writeInterfaceCastFunction)
// emitting (writeSignalInitialization)
// emitting (writeJavaLangObjectOverrideFunctions)


