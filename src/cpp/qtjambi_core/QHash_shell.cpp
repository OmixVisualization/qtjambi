/****************************************************************************
**
** Copyright (C) 2009-2022 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#include <QtCore/qcompilerdetection.h>
QT_WARNING_DISABLE_DEPRECATED
#include <QtCore/QDataStream>
#include <QtCore/QDebug>
#include <qtjambi/qtjambi_core.h>
#include <qtjambi/qtjambi_containers.h>
#include <qtjambi/qtjambi_repository.h>
#include "qtjambi_core_repository.h"
#include <qtjambi/qtjambi_application.h>
#include <qtjambi/qtjambi_cast.h>

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QHash_initialize)
(JNIEnv * env, jobject _this, jclass keyType, QtJambiNativeID keyMetaType, jclass valueType, QtJambiNativeID valueMetaType, jobject other)
{
    QTJAMBI_TRY{
        initialize_QHash(env, _this, keyType, keyMetaType, valueType, valueMetaType, other);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMultiHash_initialize)
(JNIEnv * env, jobject _this, jclass keyType, QtJambiNativeID keyMetaType, jclass valueType, QtJambiNativeID valueMetaType, jobject other)
{
    QTJAMBI_TRY{
        initialize_QMultiHash(env, _this, keyType, keyMetaType, valueType, valueMetaType, other);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QHash_keyMetaType)
(JNIEnv * env, jclass, QtJambiNativeID __this_nativeId)
{
    jobject result{nullptr};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(env, container.first, typeid(QMap<QVariant,QVariant>));
        AbstractHashAccess* containerAccess = dynamic_cast<AbstractHashAccess*>(container.second);
        Q_ASSERT(containerAccess);
        result = qtjambi_cast<jobject>(env, containerAccess->keyMetaType());
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return result;
}

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QHash_valueMetaType)
(JNIEnv * env, jclass, QtJambiNativeID __this_nativeId)
{
    jobject result{nullptr};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(env, container.first, typeid(QHash<QVariant,QVariant>));
        AbstractHashAccess* containerAccess = dynamic_cast<AbstractHashAccess*>(container.second);
        Q_ASSERT(containerAccess);
        result = qtjambi_cast<jobject>(env, containerAccess->valueMetaType());
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return result;
}

// emitting  (functionsInTargetLang writeFinalFunction)
// QHash<Key, T>::begin() const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QHash_begin__J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId)
{
    jobject result{nullptr};
    QTJAMBI_DEBUG_METHOD_PRINT("native", "begin() const")
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QHash<QVariant,QVariant>));
        AbstractHashAccess* containerAccess = dynamic_cast<AbstractHashAccess*>(container.second);
        Q_ASSERT(containerAccess);
        result = containerAccess->begin(__jni_env, __this_nativeId, container.first);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QHash<Key, T>::capacity() const
extern "C" Q_DECL_EXPORT jint JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QHash_capacity__J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId)
{
    jint result{0};
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QHash<Key, T>::capacity() const")
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QHash<QVariant,QVariant>));
        AbstractHashAccess* containerAccess = dynamic_cast<AbstractHashAccess*>(container.second);
        Q_ASSERT(containerAccess);
        result = containerAccess->capacity(__jni_env, container.first);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QHash<Key, T>::clear()
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QHash_clear__J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QHash<Key, T>::clear()")
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QHash<QVariant,QVariant>));
        AbstractHashAccess* containerAccess = dynamic_cast<AbstractHashAccess*>(container.second);
        Q_ASSERT(containerAccess);
        containerAccess->clear(__jni_env, container.first);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

// QHash<Key, T>::contains(const Key & k) const
extern "C" Q_DECL_EXPORT jboolean JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QHash_contains__JLjava_lang_Object_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0)
{
    jboolean result{false};
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QHash<Key, T>::contains(const Key & k) const")
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QHash<QVariant,QVariant>));
        AbstractHashAccess* containerAccess = dynamic_cast<AbstractHashAccess*>(container.second);
        Q_ASSERT(containerAccess);
        result = containerAccess->contains(__jni_env, container.first, t0);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QHash<Key, T>::count(const Key & k) const
extern "C" Q_DECL_EXPORT jint JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QHash_count__JLjava_lang_Object_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0)
{
    jint result{0};
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QHash<Key, T>::count(const Key & k) const")
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QHash<QVariant,QVariant>));
        AbstractHashAccess* containerAccess = dynamic_cast<AbstractHashAccess*>(container.second);
        Q_ASSERT(containerAccess);
        result = containerAccess->count(__jni_env, container.first, t0);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QHash<Key, T>::end() const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QHash_end__J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId)
{
    jobject result{nullptr};
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QHash<Key, T>::end() const")
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QHash<QVariant,QVariant>));
        AbstractHashAccess* containerAccess = dynamic_cast<AbstractHashAccess*>(container.second);
        Q_ASSERT(containerAccess);
        result = containerAccess->end(__jni_env, __this_nativeId, container.first);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QHash<Key, T>::find(const Key & k) const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QHash_find__JLjava_lang_Object_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0)
{
    jobject result{nullptr};
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QHash<Key, T>::find(const Key & k) const")
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QHash<QVariant,QVariant>));
        AbstractHashAccess* containerAccess = dynamic_cast<AbstractHashAccess*>(container.second);
        Q_ASSERT(containerAccess);
        result = containerAccess->find(__jni_env, __this_nativeId, container.first, t0);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QHash<Key, T>::insert(const K & k, const T & t)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QHash_insert__JLjava_lang_Object_2Ljava_lang_Object_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject k0,
 jobject t1)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QHash<Key, T>::insert(const K & k, const T & t)")
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QHash<QVariant,QVariant>));
        AbstractHashAccess* containerAccess = dynamic_cast<AbstractHashAccess*>(container.second);
        Q_ASSERT(containerAccess);
        containerAccess->insert(__jni_env, container.first, k0, t1);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

// QHash<Key, T>::key(const V & value, const K & defaultKey) const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QHash_key__JLjava_lang_Object_2Ljava_lang_Object_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jobject k1)
{
    jobject result{nullptr};
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QHash<Key, T>::key(const V & value, const K & defaultKey) const")
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QHash<QVariant,QVariant>));
        AbstractHashAccess* containerAccess = dynamic_cast<AbstractHashAccess*>(container.second);
        Q_ASSERT(containerAccess);
        result = containerAccess->key(__jni_env, container.first, t0, k1);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QHash<Key, T>::keys(const T & value) const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QHash_keysForValue__JLjava_lang_Object_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0)
{
    jobject result{nullptr};
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QHash<Key, T>::keys(const T & value) const")
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QHash<QVariant,QVariant>));
        AbstractHashAccess* containerAccess = dynamic_cast<AbstractHashAccess*>(container.second);
        Q_ASSERT(containerAccess);
        result = containerAccess->keys(__jni_env, container.first, t0);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QHash<Key, T>::keys() const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QHash_keys__J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId)
{
    jobject result{nullptr};
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QHash<Key, T>::keys() const")
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QHash<QVariant,QVariant>));
        AbstractHashAccess* containerAccess = dynamic_cast<AbstractHashAccess*>(container.second);
        Q_ASSERT(containerAccess);
        result = containerAccess->keys(__jni_env, container.first);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QHash<Key, T>::operator==(const QHash & l) const
extern "C" Q_DECL_EXPORT jboolean JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QHash_operator_1equal__JLjava_util_Map_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject l0)
{
    jboolean result{false};
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QHash<Key, T>::operator==(const QHash<Key, T> & other) const")
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QHash<QVariant,QVariant>));
        AbstractHashAccess* containerAccess = dynamic_cast<AbstractHashAccess*>(container.second);
        Q_ASSERT(containerAccess);
        result = containerAccess->equal(__jni_env, container.first, l0);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QHash<Key, T>::remove(const T & t)
extern "C" Q_DECL_EXPORT jint JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QHash_remove__JLjava_lang_Object_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0)
{
    jint result{-1};
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QHash<Key, T>::remove(const T & t)")
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QHash<QVariant,QVariant>));
        AbstractHashAccess* containerAccess = dynamic_cast<AbstractHashAccess*>(container.second);
        Q_ASSERT(containerAccess);
        result = containerAccess->remove(__jni_env, container.first, t0);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QHash<Key, T>::reserve(int size)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QHash_reserve__JI)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jint size0)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QHash<Key, T>::reserve(int size)")
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QHash<QVariant,QVariant>));
        AbstractHashAccess* containerAccess = dynamic_cast<AbstractHashAccess*>(container.second);
        Q_ASSERT(containerAccess);
        containerAccess->reserve(__jni_env, container.first, size0);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

// QHash<Key, T>::size() const
extern "C" Q_DECL_EXPORT jint JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QHash_size__J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId)
{
    jint result{0};
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QHash<Key, T>::size() const")
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QHash<QVariant,QVariant>));
        AbstractHashAccess* containerAccess = dynamic_cast<AbstractHashAccess*>(container.second);
        Q_ASSERT(containerAccess);
        result = containerAccess->size(__jni_env, container.first);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QHash<Key, T>::take(const T & t)
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QHash_take__JLjava_lang_Object_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0)
{
    jobject result{nullptr};
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QHash<Key, T>::take(const T & t)")
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QHash<QVariant,QVariant>));
        AbstractHashAccess* containerAccess = dynamic_cast<AbstractHashAccess*>(container.second);
        Q_ASSERT(containerAccess);
        result = containerAccess->take(__jni_env, container.first, t0);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QHash_writeTo)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 QtJambiNativeID stream0)
{
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QHash<QVariant,QVariant>));
        AbstractHashAccess* containerAccess = dynamic_cast<AbstractHashAccess*>(container.second);
        Q_ASSERT(containerAccess);
        QDataStream* stream = qtjambi_object_from_nativeId<QDataStream>(stream0);
        qtjambi_check_resource(__jni_env, stream, typeid(QDataStream));
        QByteArray containerName = "QHash<";
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
            JavaException::raiseQNoImplementationException(__jni_env, containerName QTJAMBI_STACKTRACEINFO );
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QHash_readFrom)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 QtJambiNativeID stream0)
{
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QHash<QVariant,QVariant>));
        AbstractHashAccess* containerAccess = dynamic_cast<AbstractHashAccess*>(container.second);
        Q_ASSERT(containerAccess);
        QDataStream* stream = qtjambi_object_from_nativeId<QDataStream>(stream0);
        qtjambi_check_resource(__jni_env, stream, typeid(QDataStream));
        QByteArray containerName = "QHash<";
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
            JavaException::raiseQNoImplementationException(__jni_env, containerName QTJAMBI_STACKTRACEINFO );
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

// QHash<Key, T>::value(const Key &key, const T &defaultValue) const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QHash_value__JLjava_lang_Object_2Ljava_lang_Object_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject k0,
 jobject t1)
{
    jobject result{nullptr};
    QTJAMBI_DEBUG_METHOD_PRINT("native", "value(const Key &key, const T &defaultValue) const")
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QHash<QVariant,QVariant>));
        AbstractHashAccess* containerAccess = dynamic_cast<AbstractHashAccess*>(container.second);
        Q_ASSERT(containerAccess);
        result = containerAccess->value(__jni_env, container.first, k0, t1);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QHash<Key, T>::values() const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QHash_values__J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId)
{
    jobject result{nullptr};
    QTJAMBI_DEBUG_METHOD_PRINT("native", "values() const")
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QHash<QVariant,QVariant>));
        AbstractHashAccess* containerAccess = dynamic_cast<AbstractHashAccess*>(container.second);
        Q_ASSERT(containerAccess);
        result = containerAccess->values(__jni_env, container.first);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

extern "C" Q_DECL_EXPORT jstring JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QHash_toString)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId)
{
    jstring result{nullptr};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QHash<QVariant,QVariant>));
        AbstractHashAccess* containerAccess = dynamic_cast<AbstractHashAccess*>(container.second);
        Q_ASSERT(containerAccess);
        QString strg;
        {
            QDebug stream(&strg);
            QByteArray containerName = "QHash<";
            containerName += containerAccess->keyMetaType().name();
            containerName += ",";
            containerName += containerAccess->valueMetaType().name();
            containerName += ">";
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
            int metaType = containerAccess->registerContainer(containerName);
            debug_stream(stream.nospace(), metaType, container.first);
#else
            QMetaType metaType(containerAccess->registerContainer(containerName));
            if(!metaType.debugStream(stream.nospace(), container.first)){
                containerName.prepend("QDebug >> ");
                JavaException::raiseQNoImplementationException(__jni_env, containerName QTJAMBI_STACKTRACEINFO );
            }
#endif
        }
        result = qtjambi_from_qstring(__jni_env, strg);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

extern "C" Q_DECL_EXPORT jint JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QHash_hashCode)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId)
{
    jint result{0};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QHash<QVariant,QVariant>));
        AbstractHashAccess* containerAccess = dynamic_cast<AbstractHashAccess*>(container.second);
        Q_ASSERT(containerAccess);
        QByteArray containerName = "QHash<";
        containerName += containerAccess->keyMetaType().name();
        containerName += ",";
        containerName += containerAccess->valueMetaType().name();
        containerName += ">";
        QMetaType metaType(containerAccess->registerContainer(containerName));
        hash_type h = qHash(metaType, container.first);
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

// QMultiHash<Key, T>::uniqueKeys() const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMultiHash_uniqueKeys__J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId)
{
    jobject result{nullptr};
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QMultiHash<Key, T>::uniqueKeys() const")
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QHash<QVariant,QVariant>));
        AbstractMultiHashAccess* containerAccess = dynamic_cast<AbstractMultiHashAccess*>(container.second);
        Q_ASSERT(containerAccess);
        result = containerAccess->uniqueKeys(__jni_env, container.first);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QMultiHash<Key, T>::unite(const T & t)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMultiHash_unite__JLjava_lang_Object_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QMultiHash<Key, T>::unite(const T & t)")
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QHash<QVariant,QVariant>));
        AbstractMultiHashAccess* containerAccess = dynamic_cast<AbstractMultiHashAccess*>(container.second);
        Q_ASSERT(containerAccess);
        containerAccess->unite(__jni_env, container.first, t0);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

// QMultiHash<Key, T>::values(const Key &key) const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMultiHash_valuesKey__JLjava_lang_Object_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject k0)
{
    jobject result{nullptr};
    QTJAMBI_DEBUG_METHOD_PRINT("native", "values(const Key &key) const")
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QHash<QVariant,QVariant>));
        AbstractMultiHashAccess* containerAccess = dynamic_cast<AbstractMultiHashAccess*>(container.second);
        Q_ASSERT(containerAccess);
        result = containerAccess->values(__jni_env, container.first, k0);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QMultiHash<Key, T>::contains(const Key & k, const T & value) const
extern "C" Q_DECL_EXPORT jboolean JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMultiHash_contains__JLjava_lang_Object_2Ljava_lang_Object_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jobject k1)
{
    jboolean result{false};
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QMultiHash<Key, T>::contains(const Key & k, const T & value) const")
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QMultiHash<QVariant,QVariant>));
        AbstractMultiHashAccess* containerAccess = dynamic_cast<AbstractMultiHashAccess*>(container.second);
        Q_ASSERT(containerAccess);
        result = containerAccess->contains(__jni_env, container.first, t0, k1);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QMultiHash<Key, T>::count(const Key & k, const T & value) const
extern "C" Q_DECL_EXPORT jint JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMultiHash_count__JLjava_lang_Object_2Ljava_lang_Object_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jobject k1)
{
    jint result{0};
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QMultiHash<Key, T>::contains(const Key & k, const T & value) const")
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QMultiHash<QVariant,QVariant>));
        AbstractMultiHashAccess* containerAccess = dynamic_cast<AbstractMultiHashAccess*>(container.second);
        Q_ASSERT(containerAccess);
        result = containerAccess->count(__jni_env, container.first, t0, k1);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QMultiHash<Key, T>::find(const Key & k, const T & value) const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMultiHash_find__JLjava_lang_Object_2Ljava_lang_Object_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jobject k1)
{
    jobject result{nullptr};
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QMultiHash<Key, T>::find(const Key & k, const T & value) const")
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QMultiHash<QVariant,QVariant>));
        AbstractMultiHashAccess* containerAccess = dynamic_cast<AbstractMultiHashAccess*>(container.second);
        Q_ASSERT(containerAccess);
        result = containerAccess->find(__jni_env, __this_nativeId, container.first, t0, k1);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QMultiHash<Key, T>::remove(const Key & k, const T & value)
extern "C" Q_DECL_EXPORT jint JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMultiHash_remove__JLjava_lang_Object_2Ljava_lang_Object_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jobject k1)
{
    jint result{0};
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QMultiHash<Key, T>::remove(const Key & k, const T & value)")
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QMultiHash<QVariant,QVariant>));
        AbstractMultiHashAccess* containerAccess = dynamic_cast<AbstractMultiHashAccess*>(container.second);
        Q_ASSERT(containerAccess);
        result = containerAccess->remove(__jni_env, container.first, t0, k1);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QMultiHash<Key, T>::replace(const Key & k, const T & value)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMultiHash_replace__JLjava_lang_Object_2Ljava_lang_Object_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jobject k1)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QMultiHash<Key, T>::replace(const Key & k, const T & value)")
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QMultiHash<QVariant,QVariant>));
        AbstractMultiHashAccess* containerAccess = dynamic_cast<AbstractMultiHashAccess*>(container.second);
        Q_ASSERT(containerAccess);
        containerAccess->replace(__jni_env, container.first, t0, k1);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMultiHash_writeTo)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 QtJambiNativeID stream0)
{
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QMultiHash<QVariant,QVariant>));
        AbstractMultiHashAccess* containerAccess = dynamic_cast<AbstractMultiHashAccess*>(container.second);
        Q_ASSERT(containerAccess);
        QDataStream* stream = qtjambi_object_from_nativeId<QDataStream>(stream0);
        qtjambi_check_resource(__jni_env, stream, typeid(QDataStream));
        QByteArray containerName = "QMultiHash<";
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
            JavaException::raiseQNoImplementationException(__jni_env, containerName QTJAMBI_STACKTRACEINFO );
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMultiHash_readFrom)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 QtJambiNativeID stream0)
{
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QMultiHash<QVariant,QVariant>));
        AbstractMultiHashAccess* containerAccess = dynamic_cast<AbstractMultiHashAccess*>(container.second);
        Q_ASSERT(containerAccess);
        QDataStream* stream = qtjambi_object_from_nativeId<QDataStream>(stream0);
        qtjambi_check_resource(__jni_env, stream, typeid(QDataStream));
        QByteArray containerName = "QMultiHash<";
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
            JavaException::raiseQNoImplementationException(__jni_env, containerName QTJAMBI_STACKTRACEINFO );
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT jstring JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMultiHash_toString)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId)
{
    jstring result{nullptr};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QMultiHash<QVariant,QVariant>));
        AbstractMultiHashAccess* containerAccess = dynamic_cast<AbstractMultiHashAccess*>(container.second);
        Q_ASSERT(containerAccess);
        QString strg;
        {
            QDebug stream(&strg);
            QByteArray containerName = "QMultiHash<";
            containerName += containerAccess->keyMetaType().name();
            containerName += ",";
            containerName += containerAccess->valueMetaType().name();
            containerName += ">";
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
            int metaType = containerAccess->registerContainer(containerName);
            debug_stream(stream.nospace(), metaType, container.first);
            if(strg.isEmpty()){
                containerName.prepend("QDebug >> ");
                JavaException::raiseQNoImplementationException(__jni_env, containerName QTJAMBI_STACKTRACEINFO );
            }
#else
            QMetaType metaType(containerAccess->registerContainer(containerName));
            if(!metaType.debugStream(stream.nospace(), container.first)){
                containerName.prepend("QDebug >> ");
                JavaException::raiseQNoImplementationException(__jni_env, containerName QTJAMBI_STACKTRACEINFO );
            }
#endif
        }
        result = qtjambi_from_qstring(__jni_env, strg);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

extern "C" Q_DECL_EXPORT jint JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMultiHash_hashCode)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId)
{
    jint result{0};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QMultiHash<QVariant,QVariant>));
        AbstractMultiHashAccess* containerAccess = dynamic_cast<AbstractMultiHashAccess*>(container.second);
        Q_ASSERT(containerAccess);
        QByteArray containerName = "QMultiHash<";
        containerName += containerAccess->keyMetaType().name();
        containerName += ",";
        containerName += containerAccess->valueMetaType().name();
        containerName += ">";
        QMetaType metaType(containerAccess->registerContainer(containerName));
        hash_type h = qHash(metaType, container.first);
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


