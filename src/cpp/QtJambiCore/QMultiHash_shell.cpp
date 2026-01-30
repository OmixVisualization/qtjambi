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

#include <QtCore/qcompilerdetection.h>
QT_WARNING_DISABLE_DEPRECATED
#include "pch_p.h"
#include <QtCore/QDataStream>
#include <QtCore/QDebug>
#include <QtJambi/QtJambiAPI>
#include <QtJambi/ContainerAPI>
#include <QtJambi/JavaAPI>
#include "utils_p.h"
#include <QtJambi/CoreAPI>
#include <QtJambi/qtjambi_cast.h>

extern "C" JNIEXPORT void JNICALL Java_io_qt_core_QMultiHash_initialize
(JNIEnv * env, jobject _this, jclass keyType, QtJambiNativeID keyMetaType, jclass valueType, QtJambiNativeID valueMetaType, jobject other)
{
    QTJAMBI_TRY{
        QTJAMBI_NATIVE_METHOD_CALL("QMultiHash<K,V>::initialize()")
        CoreAPI::initializeQMultiHash(env, _this, keyType, keyMetaType, valueType, valueMetaType, other);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
}

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_core_QMultiHash_keyMetaType
(JNIEnv * env, jobject, QtJambiNativeID __this_nativeId)
{
    jobject result{nullptr};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(env, container.first, typeid(QMultiHash<QVariant,QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QMultiHash<K,V>::keyMetaType()", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractMultiHashAccess, containerAccess, container.second);

        result = qtjambi_cast<jobject>(env, containerAccess->keyMetaType());
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return result;
}

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_core_QMultiHash_valueMetaType
(JNIEnv * env, jobject, QtJambiNativeID __this_nativeId)
{
    jobject result{nullptr};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(env, container.first, typeid(QMultiHash<QVariant,QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QMultiHash<K,V>::valueMetaType()", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractMultiHashAccess, containerAccess, container.second);

        result = qtjambi_cast<jobject>(env, containerAccess->valueMetaType());
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return result;
}

// emitting  (functionsInTargetLang writeFinalFunction)
// QMultiHash<Key, T>::begin()
extern "C" JNIEXPORT jobject JNICALL Java_io_qt_core_QMultiHash_begin__J
(JNIEnv *__jni_env,
 jobject _this,
 QtJambiNativeID __this_nativeId)
{
    jobject result{nullptr};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QMultiHash<QVariant,QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("begin()", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractMultiHashAccess, containerAccess, container.second);

        result = containerAccess->begin(__jni_env, ExtendedContainerInfo{_this, container.first, __this_nativeId});
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QMultiHash<Key, T>::end()
extern "C" JNIEXPORT jobject JNICALL Java_io_qt_core_QMultiHash_end__J
(JNIEnv *__jni_env,
 jobject _this,
 QtJambiNativeID __this_nativeId)
{
    jobject result{nullptr};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QMultiHash<QVariant,QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QMultiHash<Key, T>::end()", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractMultiHashAccess, containerAccess, container.second);

        result = containerAccess->end(__jni_env, ExtendedContainerInfo{_this, container.first, __this_nativeId});
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QMultiHash<Key, T>::constBegin() const
extern "C" JNIEXPORT jobject JNICALL Java_io_qt_core_QMultiHash_constBegin__J
(JNIEnv *__jni_env,
 jobject _this,
 QtJambiNativeID __this_nativeId)
{
    jobject result{nullptr};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QMultiHash<QVariant,QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QMultiHash<Key, T>::constBegin() const", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractMultiHashAccess, containerAccess, container.second);

        result = containerAccess->constBegin(__jni_env, ConstExtendedContainerInfo{_this, container.first, __this_nativeId});
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QMultiHash<Key, T>::constEnd() const
extern "C" JNIEXPORT jobject JNICALL Java_io_qt_core_QMultiHash_constEnd__J
(JNIEnv *__jni_env,
 jobject _this,
 QtJambiNativeID __this_nativeId)
{
    jobject result{nullptr};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QMultiHash<QVariant,QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QMultiHash<Key, T>::constEnd() const", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractMultiHashAccess, containerAccess, container.second);

        result = containerAccess->constEnd(__jni_env, ConstExtendedContainerInfo{_this, container.first, __this_nativeId});
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QMultiHash<Key, T>::capacity() const
extern "C" JNIEXPORT jint JNICALL Java_io_qt_core_QMultiHash_capacity__J
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID __this_nativeId)
{
    jint result{0};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QMultiHash<QVariant,QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QMultiHash<Key, T>::capacity() const", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractMultiHashAccess, containerAccess, container.second);

        result = containerAccess->capacity(__jni_env, container.first);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QMultiHash<Key, T>::clear()
extern "C" JNIEXPORT void JNICALL Java_io_qt_core_QMultiHash_clear__J
(JNIEnv *__jni_env,
 jobject _this,
 QtJambiNativeID __this_nativeId)
{
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QMultiHash<QVariant,QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QMultiHash<Key, T>::clear()", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractMultiHashAccess, containerAccess, container.second);

        containerAccess->clear(__jni_env, ContainerInfo{_this, container.first});
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

// QMultiHash<Key, T>::contains(const Key & k) const
extern "C" JNIEXPORT jboolean JNICALL Java_io_qt_core_QMultiHash_contains__JLjava_lang_Object_2
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID __this_nativeId,
 jobject t0)
{
    jboolean result{false};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QMultiHash<QVariant,QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QMultiHash<Key, T>::contains(const Key & k) const", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractMultiHashAccess, containerAccess, container.second);

        result = containerAccess->contains(__jni_env, container.first, t0);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QMultiHash<Key, T>::count(const Key & k) const
extern "C" JNIEXPORT jint JNICALL Java_io_qt_core_QMultiHash_count__JLjava_lang_Object_2
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID __this_nativeId,
 jobject t0)
{
    jint result{0};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QMultiHash<QVariant,QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QMultiHash<Key, T>::count(const Key & k) const", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractMultiHashAccess, containerAccess, container.second);

        result = containerAccess->count(__jni_env, container.first, t0);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QMultiHash<Key, T>::find(const Key & k) const
extern "C" JNIEXPORT jobject JNICALL Java_io_qt_core_QMultiHash_find__JLjava_lang_Object_2
(JNIEnv *__jni_env,
 jobject _this,
 QtJambiNativeID __this_nativeId,
 jobject t0)
{
    jobject result{nullptr};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QMultiHash<QVariant,QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QMultiHash<Key, T>::find(const Key & k) const", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractMultiHashAccess, containerAccess, container.second);

        result = containerAccess->constFind(__jni_env, ConstExtendedContainerInfo{_this, container.first, __this_nativeId}, t0);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QMultiHash<Key, T>::insert(const K & k, const T & t)
extern "C" JNIEXPORT void JNICALL Java_io_qt_core_QMultiHash_insert__JLjava_lang_Object_2Ljava_lang_Object_2
(JNIEnv *__jni_env,
 jobject _this,
 QtJambiNativeID __this_nativeId,
 jobject k0,
 jobject t1)
{
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QMultiHash<QVariant,QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QMultiHash<Key, T>::insert(const K & k, const T & t)", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractMultiHashAccess, containerAccess, container.second);

        containerAccess->insert(__jni_env, ContainerInfo{_this, container.first}, k0, t1);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

// QMultiHash<Key, T>::key(const V & value, const K & defaultKey) const
extern "C" JNIEXPORT jobject JNICALL Java_io_qt_core_QMultiHash_key__JLjava_lang_Object_2Ljava_lang_Object_2
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jobject k1)
{
    jobject result{nullptr};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QMultiHash<QVariant,QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QMultiHash<Key, T>::key(const V & value, const K & defaultKey) const", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractMultiHashAccess, containerAccess, container.second);

        result = containerAccess->key(__jni_env, container.first, t0, k1);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QMultiHash<Key, T>::keys(const T & value) const
extern "C" JNIEXPORT jobject JNICALL Java_io_qt_core_QMultiHash_keysForValue__JLjava_lang_Object_2
(JNIEnv *__jni_env,
 jobject _this,
 QtJambiNativeID __this_nativeId,
 jobject t0)
{
    jobject result{nullptr};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QMultiHash<QVariant,QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QMultiHash<Key, T>::keys(const T & value) const", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractMultiHashAccess, containerAccess, container.second);

        result = containerAccess->keys(__jni_env, ConstContainerInfo{_this, container.first}, t0).object;
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QMultiHash<Key, T>::keys() const
extern "C" JNIEXPORT jobject JNICALL Java_io_qt_core_QMultiHash_keys__J
(JNIEnv *__jni_env,
 jobject _this,
 QtJambiNativeID __this_nativeId)
{
    jobject result{nullptr};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QMultiHash<QVariant,QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QMultiHash<Key, T>::keys() const", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractMultiHashAccess, containerAccess, container.second);

        result = containerAccess->keys(__jni_env, ConstContainerInfo{_this, container.first}).object;
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QMultiHash<Key, T>::operator==(const QHash & l) const
extern "C" JNIEXPORT jboolean JNICALL Java_io_qt_core_QMultiHash_operator_1equal__JLjava_util_Map_2
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID __this_nativeId,
 jobject l0)
{
    jboolean result{false};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QMultiHash<QVariant,QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QMultiHash<Key, T>::operator==(const QMultiHash<Key, T> & other) const", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractMultiHashAccess, containerAccess, container.second);

        result = containerAccess->equal(__jni_env, container.first, l0);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QMultiHash<Key, T>::remove(const T & t)
extern "C" JNIEXPORT jint JNICALL Java_io_qt_core_QMultiHash_remove__JLjava_lang_Object_2
(JNIEnv *__jni_env,
 jobject _this,
 QtJambiNativeID __this_nativeId,
 jobject t0)
{
    jint result{-1};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QMultiHash<QVariant,QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QMultiHash<Key, T>::remove(const T & t)", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractMultiHashAccess, containerAccess, container.second);

        result = containerAccess->remove(__jni_env, ContainerInfo{_this, container.first}, t0);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QMultiHash<Key, T>::reserve(int size)
extern "C" JNIEXPORT void JNICALL Java_io_qt_core_QMultiHash_reserve__JI
(JNIEnv *__jni_env,
 jobject _this,
 QtJambiNativeID __this_nativeId,
 jint size0)
{
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QMultiHash<QVariant,QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QMultiHash<Key, T>::reserve(int size)", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractMultiHashAccess, containerAccess, container.second);

        containerAccess->reserve(__jni_env, ContainerInfo{_this, container.first}, size0);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

// QMultiHash<Key, T>::size() const
extern "C" JNIEXPORT jint JNICALL Java_io_qt_core_QMultiHash_size__J
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID __this_nativeId)
{
    jint result{0};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QMultiHash<QVariant,QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QMultiHash<Key, T>::size() const", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractMultiHashAccess, containerAccess, container.second);

        result = containerAccess->size(__jni_env, container.first);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QMultiHash<Key, T>::take(const T & t)
extern "C" JNIEXPORT jobject JNICALL Java_io_qt_core_QMultiHash_take__JLjava_lang_Object_2
(JNIEnv *__jni_env,
 jobject _this,
 QtJambiNativeID __this_nativeId,
 jobject t0)
{
    jobject result{nullptr};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QMultiHash<QVariant,QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QMultiHash<Key, T>::take(const T & t)", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractMultiHashAccess, containerAccess, container.second);

        result = containerAccess->take(__jni_env, ContainerInfo{_this, container.first}, t0);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QMultiHash<Key, T>::value(const Key &key, const T &defaultValue) const
extern "C" JNIEXPORT jobject JNICALL Java_io_qt_core_QMultiHash_value__JLjava_lang_Object_2Ljava_lang_Object_2
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID __this_nativeId,
 jobject k0,
 jobject t1)
{
    jobject result{nullptr};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QMultiHash<QVariant,QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("value(const Key &key, const T &defaultValue) const", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractMultiHashAccess, containerAccess, container.second);

        result = containerAccess->value(__jni_env, container.first, k0, t1);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QMultiHash<Key, T>::values() const
extern "C" JNIEXPORT jobject JNICALL Java_io_qt_core_QMultiHash_values__J
(JNIEnv *__jni_env,
 jobject _this,
 QtJambiNativeID __this_nativeId)
{
    jobject result{nullptr};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QMultiHash<QVariant,QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("values() const", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractMultiHashAccess, containerAccess, container.second);

        result = containerAccess->values(__jni_env, ConstContainerInfo{_this, container.first}).object;
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QMultiHash<Key, T>::uniqueKeys() const
extern "C" JNIEXPORT jobject JNICALL Java_io_qt_core_QMultiHash_uniqueKeys__J
(JNIEnv *__jni_env,
 jobject _this,
 QtJambiNativeID __this_nativeId)
{
    jobject result{nullptr};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QMultiHash<QVariant,QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QMultiHash<Key, T>::uniqueKeys() const", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractMultiHashAccess, containerAccess, container.second);

        result = containerAccess->uniqueKeys(__jni_env, ConstContainerInfo{_this, container.first}).object;
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QMultiHash<Key, T>::unite(const T & t)
extern "C" JNIEXPORT void JNICALL Java_io_qt_core_QMultiHash_unite__JLjava_lang_Object_2
(JNIEnv *__jni_env,
 jobject _this,
 QtJambiNativeID __this_nativeId,
 jobject t0)
{
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QMultiHash<QVariant,QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QMultiHash<Key, T>::unite(const T & t)", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractMultiHashAccess, containerAccess, container.second);
        ContainerAndAccessInfo containerInfo(t0);
        containerAccess->unite(__jni_env, ContainerInfo{_this, container.first}, containerInfo);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

// QMultiHash<Key, T>::values(const Key &key) const
extern "C" JNIEXPORT jobject JNICALL Java_io_qt_core_QMultiHash_valuesKey__JLjava_lang_Object_2
(JNIEnv *__jni_env,
 jobject _this,
 QtJambiNativeID __this_nativeId,
 jobject k0)
{
    jobject result{nullptr};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QMultiHash<QVariant,QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("values(const Key &key) const", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractMultiHashAccess, containerAccess, container.second);

        result = containerAccess->values(__jni_env, ConstContainerInfo{_this, container.first}, k0).object;
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QMultiHash<Key, T>::contains(const Key & k, const T & value) const
extern "C" JNIEXPORT jboolean JNICALL Java_io_qt_core_QMultiHash_contains__JLjava_lang_Object_2Ljava_lang_Object_2
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jobject k1)
{
    jboolean result{false};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QMultiHash<QVariant,QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QMultiHash<Key, T>::contains(const Key & k, const T & value) const", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractMultiHashAccess, containerAccess, container.second);

        result = containerAccess->contains(__jni_env, container.first, t0, k1);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QMultiHash<Key, T>::count(const Key & k, const T & value) const
extern "C" JNIEXPORT jint JNICALL Java_io_qt_core_QMultiHash_count__JLjava_lang_Object_2Ljava_lang_Object_2
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jobject k1)
{
    jint result{0};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QMultiHash<QVariant,QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QMultiHash<Key, T>::contains(const Key & k, const T & value) const", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractMultiHashAccess, containerAccess, container.second);

        result = containerAccess->count(__jni_env, container.first, t0, k1);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QMultiHash<Key, T>::find(const Key & k, const T & value) const
extern "C" JNIEXPORT jobject JNICALL Java_io_qt_core_QMultiHash_find__JLjava_lang_Object_2Ljava_lang_Object_2
(JNIEnv *__jni_env,
 jobject _this,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jobject k1)
{
    jobject result{nullptr};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QMultiHash<QVariant,QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QMultiHash<Key, T>::find(const Key & k, const T & value) const", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractMultiHashAccess, containerAccess, container.second);

        result = containerAccess->constFind(__jni_env, ConstExtendedContainerInfo{_this, container.first, __this_nativeId}, t0, k1);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QMultiHash<Key, T>::remove(const Key & k, const T & value)
extern "C" JNIEXPORT jint JNICALL Java_io_qt_core_QMultiHash_remove__JLjava_lang_Object_2Ljava_lang_Object_2
(JNIEnv *__jni_env,
 jobject _this,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jobject k1)
{
    jint result{0};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QMultiHash<QVariant,QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QMultiHash<Key, T>::remove(const Key & k, const T & value)", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractMultiHashAccess, containerAccess, container.second);

        result = containerAccess->remove(__jni_env, ContainerInfo{_this, container.first}, t0, k1);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QMultiHash<Key, T>::replace(const Key & k, const T & value)
extern "C" JNIEXPORT void JNICALL Java_io_qt_core_QMultiHash_replace__JLjava_lang_Object_2Ljava_lang_Object_2
(JNIEnv *__jni_env,
 jobject _this,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jobject k1)
{
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QMultiHash<QVariant,QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QMultiHash<Key, T>::replace(const Key & k, const T & value)", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractMultiHashAccess, containerAccess, container.second);

        containerAccess->replace(__jni_env, ContainerInfo{_this, container.first}, t0, k1);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

extern "C" JNIEXPORT void JNICALL Java_io_qt_core_QMultiHash_writeTo
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID __this_nativeId,
 QtJambiNativeID stream0)
{
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QMultiHash<QVariant,QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("operator << (QDataStream&, QMultiHash<K,V>)", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractMultiHashAccess, containerAccess, container.second);

        QDataStream* stream = QtJambiAPI::objectFromNativeId<QDataStream>(stream0);
        QtJambiAPI::checkNullPointer(__jni_env, stream);
        QByteArray containerName = "QMultiHash<";
        containerName += containerAccess->keyMetaType().name();
        containerName += ",";
        containerName += containerAccess->valueMetaType().name();
        containerName += ">";
        QMetaType metaType(containerAccess->registerContainer(containerName));
        if(!metaType.save(*stream, container.first)){
            containerName.prepend("QDataStream& << ");
            JavaException::raiseQNoImplementationException(__jni_env, containerName.constData() QTJAMBI_STACKTRACEINFO );
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

extern "C" JNIEXPORT void JNICALL Java_io_qt_core_QMultiHash_readFrom
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID __this_nativeId,
 QtJambiNativeID stream0)
{
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QMultiHash<QVariant,QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("operator >> (QDataStream&, QMultiHash<K,V>&)", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractMultiHashAccess, containerAccess, container.second);

        QDataStream* stream = QtJambiAPI::objectFromNativeId<QDataStream>(stream0);
        QtJambiAPI::checkNullPointer(__jni_env, stream);
        QByteArray containerName = "QMultiHash<";
        containerName += containerAccess->keyMetaType().name();
        containerName += ",";
        containerName += containerAccess->valueMetaType().name();
        containerName += ">";
        QMetaType metaType(containerAccess->registerContainer(containerName));
        if(!metaType.load(*stream, container.first)){
            containerName.prepend("QDataStream& >> ");
            JavaException::raiseQNoImplementationException(__jni_env, containerName.constData() QTJAMBI_STACKTRACEINFO );
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

extern "C" JNIEXPORT jstring JNICALL Java_io_qt_core_QMultiHash_toString
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID __this_nativeId)
{
    jstring result{nullptr};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QMultiHash<QVariant,QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("operator << (QDebug, QMultiHash<K,V>)", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractMultiHashAccess, containerAccess, container.second);

        QString strg;
        {
            QDebug stream(&strg);
            QByteArray containerName = "QMultiHash<";
            containerName += containerAccess->keyMetaType().name();
            containerName += ",";
            containerName += containerAccess->valueMetaType().name();
            containerName += ">";
            QMetaType metaType(containerAccess->registerContainer(containerName));
            if(!metaType.debugStream(stream.nospace(), container.first)){
                containerName.prepend("QDebug >> ");
                JavaException::raiseQNoImplementationException(__jni_env, containerName QTJAMBI_STACKTRACEINFO );
            }
        }
        result = qtjambi_cast<jstring>(__jni_env, strg);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

extern "C" JNIEXPORT jint JNICALL Java_io_qt_core_QMultiHash_hashCode
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID __this_nativeId)
{
    jint result{0};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QMultiHash<QVariant,QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("qHash(QMultiHash<K,V>)", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractMultiHashAccess, containerAccess, container.second);

        QByteArray containerName = "QMultiHash<";
        containerName += containerAccess->keyMetaType().name();
        containerName += ",";
        containerName += containerAccess->valueMetaType().name();
        containerName += ">";
        QMetaType metaType(containerAccess->registerContainer(containerName));
        size_t h = CoreAPI::computeHash(metaType, container.first);
        result = jint(quint64(h) ^ quint64(h) >> 32);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QMultiHash<Key,T>::operator=(QMultiHash<Key,T>)
extern "C" JNIEXPORT void JNICALL Java_io_qt_core_QMultiHash_assign
    (JNIEnv *__jni_env,
     jobject _this,
     QtJambiNativeID __this_nativeId,
     jobject otherObj,
     QtJambiNativeID other)
{
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container1 = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container1.first, typeid(QMultiHash<QVariant,QVariant>));
        QPair<void*,AbstractContainerAccess*> container2 = ContainerAPI::fromNativeId(other);
        QtJambiAPI::checkNullPointer(__jni_env, container2.first, typeid(QMultiHash<QVariant,QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QMultiHash<Key,T>::operator=(QMultiHash<Key,T>)", container1.first)
        QTJAMBI_CONTAINER_CAST(AbstractMultiHashAccess, container1Access, container1.second);
        QTJAMBI_CONTAINER_CAST(AbstractMultiHashAccess, container2Access, container2.second);
        if(container1Access->keyMetaType()!=container2Access->keyMetaType())
            JavaException::raiseIllegalArgumentException(__jni_env, QString("Container's key types mismatch: %1!=%2").arg(container1Access->keyMetaType().name(), container2Access->keyMetaType().name()) QTJAMBI_STACKTRACEINFO );
        if(container1Access->valueMetaType()!=container2Access->valueMetaType())
            JavaException::raiseIllegalArgumentException(__jni_env, QString("Container's value types mismatch: %1!=%2").arg(container1Access->valueMetaType().name(), container2Access->valueMetaType().name()) QTJAMBI_STACKTRACEINFO );
        container1Access->assign(__jni_env, ContainerInfo{_this, container1.first}, ConstContainerAndAccessInfo(otherObj, container2.first, container2Access));
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

// QMultiHash<Key,T>::swap(QMultiHash<Key,T>&)
extern "C" JNIEXPORT void JNICALL Java_io_qt_core_QMultiHash_swap
    (JNIEnv *__jni_env,
     jobject _this,
     QtJambiNativeID __this_nativeId,
     jobject otherObj,
     QtJambiNativeID other)
{
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container1 = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container1.first, typeid(QMultiHash<QVariant,QVariant>));
        QPair<void*,AbstractContainerAccess*> container2 = ContainerAPI::fromNativeId(other);
        QtJambiAPI::checkNullPointer(__jni_env, container2.first, typeid(QMultiHash<QVariant,QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QMultiHash<Key,T>::swap(QMultiHash<Key,T>&)", container1.first)
        QTJAMBI_CONTAINER_CAST(AbstractMultiHashAccess, container1Access, container1.second);
        QTJAMBI_CONTAINER_CAST(AbstractMultiHashAccess, container2Access, container2.second);
        if(container1Access->keyMetaType()!=container2Access->keyMetaType())
            JavaException::raiseIllegalArgumentException(__jni_env, QString("Container's key types mismatch: %1!=%2").arg(container1Access->keyMetaType().name(), container2Access->keyMetaType().name()) QTJAMBI_STACKTRACEINFO );
        if(container1Access->valueMetaType()!=container2Access->valueMetaType())
            JavaException::raiseIllegalArgumentException(__jni_env, QString("Container's value types mismatch: %1!=%2").arg(container1Access->valueMetaType().name(), container2Access->valueMetaType().name()) QTJAMBI_STACKTRACEINFO );
        container1Access->swap(__jni_env, ContainerInfo{_this, container1.first}, ContainerAndAccessInfo(otherObj, container2.first, container2Access));
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

extern "C" JNIEXPORT jboolean JNICALL Java_io_qt_core_QMultiHash_isSharedWith__JJ
    (JNIEnv *__jni_env,
     jobject,
     QtJambiNativeID __this_nativeId,
     QtJambiNativeID other)
{
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container1 = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container1.first, typeid(QMultiHash<QVariant,QVariant>));
        QPair<void*,AbstractContainerAccess*> container2 = ContainerAPI::fromNativeId(other);
        QtJambiAPI::checkNullPointer(__jni_env, container2.first, typeid(QMultiHash<QVariant,QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QMultiHash<K,T>::isSharedWith(QMultiHash<K,T>)", container1.first)
        QTJAMBI_CONTAINER_CAST(AbstractMultiHashAccess, container1Access, container1.second);
        QTJAMBI_CONTAINER_CAST(AbstractMultiHashAccess, container2Access, container2.second);
        if(container1Access && container2Access)
            return container1Access->isSharedWith(container1.first, container2.first);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
        return false;
}

extern "C" JNIEXPORT jboolean JNICALL Java_io_qt_core_QMultiHash_isDetached__J
    (JNIEnv *__jni_env,
     jobject,
     QtJambiNativeID __this_nativeId)
{
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QMultiHash<QVariant,QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QMultiHash<T>::isDetached()", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractMultiHashAccess, containerAccess, container.second);
        if(containerAccess)
            return containerAccess->isDetached(container.first);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
        return false;
}

extern "C" JNIEXPORT void JNICALL Java_io_qt_core_QMultiHash_detach__J
    (JNIEnv *__jni_env,
     jobject _this,
     QtJambiNativeID __this_nativeId)
{
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QMultiHash<QVariant,QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QMultiHash<T>::detach()", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractMultiHashAccess, containerAccess, container.second);
        if(containerAccess)
            containerAccess->detach(ContainerInfo{_this, container.first});
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

// emitting (AbstractMetaClass::NormalFunctions|AbstractMetaClass::AbstractFunctions writeFinalFunction)
// emitting Field accessors (writeFieldAccessors)
// emitting (writeInterfaceCastFunction)
// emitting (writeSignalInitialization)
// emitting (writeJavaLangObjectOverrideFunctions)


