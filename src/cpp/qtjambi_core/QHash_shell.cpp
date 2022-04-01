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
#include <qtjambi/qtjambi_cast.h>

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_Qt_qSetGlobalQHashSeed)
(JNIEnv*,jclass, jint newSeed)
{
    qSetGlobalQHashSeed(newSeed);
}

extern "C" Q_DECL_EXPORT jint JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_Qt_qGlobalQHashSeed)
(JNIEnv*,jclass)
{
    return qGlobalQHashSeed();
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QHash_initialize)
(JNIEnv * env, jobject _this, jclass keyType, QtJambiNativeID keyMetaType, jclass valueType, QtJambiNativeID valueMetaType, jobject other)
{
    initialize_QHash(env, _this, keyType, keyMetaType, valueType, valueMetaType, other);
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMultiHash_initialize)
(JNIEnv * env, jobject _this, jclass keyType, QtJambiNativeID keyMetaType, jclass valueType, QtJambiNativeID valueMetaType, jobject other)
{
    initialize_QMultiHash(env, _this, keyType, keyMetaType, valueType, valueMetaType, other);
}

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QHash_keyMetaType)
(JNIEnv * env, jclass, QtJambiNativeID __this_nativeId)
{
    QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
    qtjambi_check_resource(env, container.first, typeid(QMap<QVariant,QVariant>));
    AbstractHashAccess* containerAccess = dynamic_cast<AbstractHashAccess*>(container.second);
    Q_ASSERT(containerAccess);
    return qtjambi_cast<jobject>(env, containerAccess->keyMetaType());
}

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QHash_valueMetaType)
(JNIEnv * env, jclass, QtJambiNativeID __this_nativeId)
{
    QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
    qtjambi_check_resource(env, container.first, typeid(QHash<QVariant,QVariant>));
    AbstractHashAccess* containerAccess = dynamic_cast<AbstractHashAccess*>(container.second);
    Q_ASSERT(containerAccess);
    return qtjambi_cast<jobject>(env, containerAccess->valueMetaType());
}

// emitting  (functionsInTargetLang writeFinalFunction)
// QHash<Key, T>::begin() const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QHash__1_1qt_1QHash_1begin__J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "begin() const")
    try{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QHash<QVariant,QVariant>));
        AbstractHashAccess* containerAccess = dynamic_cast<AbstractHashAccess*>(container.second);
        Q_ASSERT(containerAccess);
        return containerAccess->begin(__jni_env, __this_nativeId, container.first);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

// QHash<Key, T>::capacity() const
extern "C" Q_DECL_EXPORT jint JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QHash__1_1qt_1QHash_1capacity__J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QHash<Key, T>::capacity() const")
    try{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QHash<QVariant,QVariant>));
        AbstractHashAccess* containerAccess = dynamic_cast<AbstractHashAccess*>(container.second);
        Q_ASSERT(containerAccess);
        return containerAccess->capacity(__jni_env, container.first);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return 0;
}

// QHash<Key, T>::clear()
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QHash__1_1qt_1QHash_1clear__J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QHash<Key, T>::clear()")
    try{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QHash<QVariant,QVariant>));
        AbstractHashAccess* containerAccess = dynamic_cast<AbstractHashAccess*>(container.second);
        Q_ASSERT(containerAccess);
        return containerAccess->clear(__jni_env, container.first);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// QHash<Key, T>::contains(const Key & k) const
extern "C" Q_DECL_EXPORT jboolean JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QHash__1_1qt_1QHash_1contains__JLjava_lang_Object_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QHash<Key, T>::contains(const Key & k) const")
    try{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QHash<QVariant,QVariant>));
        AbstractHashAccess* containerAccess = dynamic_cast<AbstractHashAccess*>(container.second);
        Q_ASSERT(containerAccess);
        return containerAccess->contains(__jni_env, container.first, t0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return false;
}

// QHash<Key, T>::count(const Key & k) const
extern "C" Q_DECL_EXPORT jint JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QHash__1_1qt_1QHash_1count__JLjava_lang_Object_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QHash<Key, T>::count(const Key & k) const")
    try{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QHash<QVariant,QVariant>));
        AbstractHashAccess* containerAccess = dynamic_cast<AbstractHashAccess*>(container.second);
        Q_ASSERT(containerAccess);
        return containerAccess->count(__jni_env, container.first, t0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return 0;
}

// QHash<Key, T>::end() const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QHash__1_1qt_1QHash_1end__J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QHash<Key, T>::end() const")
    try{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QHash<QVariant,QVariant>));
        AbstractHashAccess* containerAccess = dynamic_cast<AbstractHashAccess*>(container.second);
        Q_ASSERT(containerAccess);
        return containerAccess->end(__jni_env, __this_nativeId, container.first);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

// QHash<Key, T>::find(const Key & k) const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QHash__1_1qt_1QHash_1find__JLjava_lang_Object_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QHash<Key, T>::find(const Key & k) const")
    try{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QHash<QVariant,QVariant>));
        AbstractHashAccess* containerAccess = dynamic_cast<AbstractHashAccess*>(container.second);
        Q_ASSERT(containerAccess);
        return containerAccess->find(__jni_env, __this_nativeId, container.first, t0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

// QHash<Key, T>::insert(const K & k, const T & t)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QHash__1_1qt_1QHash_1insert__JLjava_lang_Object_2Ljava_lang_Object_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject k0,
 jobject t1)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QHash<Key, T>::insert(const K & k, const T & t)")
    try{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QHash<QVariant,QVariant>));
        AbstractHashAccess* containerAccess = dynamic_cast<AbstractHashAccess*>(container.second);
        Q_ASSERT(containerAccess);
        containerAccess->insert(__jni_env, container.first, k0, t1);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// QHash<Key, T>::key(const V & value, const K & defaultKey) const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QHash__1_1qt_1QHash_1key__JLjava_lang_Object_2Ljava_lang_Object_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jobject k1)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QHash<Key, T>::key(const V & value, const K & defaultKey) const")
    try{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QHash<QVariant,QVariant>));
        AbstractHashAccess* containerAccess = dynamic_cast<AbstractHashAccess*>(container.second);
        Q_ASSERT(containerAccess);
        return containerAccess->key(__jni_env, container.first, t0, k1);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

// QHash<Key, T>::keys(const T & value) const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QHash__1_1qt_1QHash_1keysForValue__JLjava_lang_Object_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QHash<Key, T>::keys(const T & value) const")
    try{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QHash<QVariant,QVariant>));
        AbstractHashAccess* containerAccess = dynamic_cast<AbstractHashAccess*>(container.second);
        Q_ASSERT(containerAccess);
        return containerAccess->keys(__jni_env, container.first, t0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

// QHash<Key, T>::keys() const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QHash__1_1qt_1QHash_1keys__J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QHash<Key, T>::keys() const")
    try{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QHash<QVariant,QVariant>));
        AbstractHashAccess* containerAccess = dynamic_cast<AbstractHashAccess*>(container.second);
        Q_ASSERT(containerAccess);
        return containerAccess->keys(__jni_env, container.first);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

// QHash<Key, T>::operator==(const QHash & l) const
extern "C" Q_DECL_EXPORT jboolean JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QHash__1_1qt_1QHash_1operator_1equal__JLjava_util_Map_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject l0)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QHash<Key, T>::operator==(const QHash<Key, T> & other) const")
    try{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QHash<QVariant,QVariant>));
        AbstractHashAccess* containerAccess = dynamic_cast<AbstractHashAccess*>(container.second);
        Q_ASSERT(containerAccess);
        return containerAccess->equal(__jni_env, container.first, l0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return false;
}

// QHash<Key, T>::remove(const T & t)
extern "C" Q_DECL_EXPORT jint JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QHash__1_1qt_1QHash_1remove__JLjava_lang_Object_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QHash<Key, T>::remove(const T & t)")
    try{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QHash<QVariant,QVariant>));
        AbstractHashAccess* containerAccess = dynamic_cast<AbstractHashAccess*>(container.second);
        Q_ASSERT(containerAccess);
        return containerAccess->remove(__jni_env, container.first, t0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return -1;
}

// QHash<Key, T>::reserve(int size)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QHash__1_1qt_1QHash_1reserve__JI)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jint size0)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QHash<Key, T>::reserve(int size)")
    try{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QHash<QVariant,QVariant>));
        AbstractHashAccess* containerAccess = dynamic_cast<AbstractHashAccess*>(container.second);
        Q_ASSERT(containerAccess);
        containerAccess->reserve(__jni_env, container.first, size0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// QHash<Key, T>::size() const
extern "C" Q_DECL_EXPORT jint JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QHash__1_1qt_1QHash_1size__J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QHash<Key, T>::size() const")
    try{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QHash<QVariant,QVariant>));
        AbstractHashAccess* containerAccess = dynamic_cast<AbstractHashAccess*>(container.second);
        Q_ASSERT(containerAccess);
        return containerAccess->size(__jni_env, container.first);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return 0;
}

// QHash<Key, T>::take(const T & t)
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QHash__1_1qt_1QHash_1take__JLjava_lang_Object_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QHash<Key, T>::take(const T & t)")
    try{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QHash<QVariant,QVariant>));
        AbstractHashAccess* containerAccess = dynamic_cast<AbstractHashAccess*>(container.second);
        Q_ASSERT(containerAccess);
        return containerAccess->take(__jni_env, container.first, t0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QHash__1_1qt_1QHash_1writeTo)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 QtJambiNativeID stream0)
{
    try{
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
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QHash__1_1qt_1QHash_1readFrom)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 QtJambiNativeID stream0)
{
    try{
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
        if(!metaType.load(*stream, container.first)){
#endif
            containerName.prepend("QDataStream& >> ");
            JavaException::raiseQNoImplementationException(__jni_env, containerName QTJAMBI_STACKTRACEINFO );
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// QHash<Key, T>::value(const Key &key, const T &defaultValue) const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QHash__1_1qt_1QHash_1value__JLjava_lang_Object_2Ljava_lang_Object_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject k0,
 jobject t1)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "value(const Key &key, const T &defaultValue) const")
    try{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QHash<QVariant,QVariant>));
        AbstractHashAccess* containerAccess = dynamic_cast<AbstractHashAccess*>(container.second);
        Q_ASSERT(containerAccess);
        return containerAccess->value(__jni_env, container.first, k0, t1);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

// QHash<Key, T>::values() const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QHash__1_1qt_1QHash_1values__J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "values() const")
    try{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QHash<QVariant,QVariant>));
        AbstractHashAccess* containerAccess = dynamic_cast<AbstractHashAccess*>(container.second);
        Q_ASSERT(containerAccess);
        return containerAccess->values(__jni_env, container.first);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

// QMultiHash<Key, T>::uniqueKeys() const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMultiHash__1_1qt_1QMultiHash_1uniqueKeys__J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QMultiHash<Key, T>::uniqueKeys() const")
    try{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QHash<QVariant,QVariant>));
        AbstractMultiHashAccess* containerAccess = dynamic_cast<AbstractMultiHashAccess*>(container.second);
        Q_ASSERT(containerAccess);
        return containerAccess->uniqueKeys(__jni_env, container.first);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

// QMultiHash<Key, T>::unite(const T & t)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMultiHash__1_1qt_1QMultiHash_1unite__JLjava_lang_Object_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QMultiHash<Key, T>::unite(const T & t)")
    try{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QHash<QVariant,QVariant>));
        AbstractMultiHashAccess* containerAccess = dynamic_cast<AbstractMultiHashAccess*>(container.second);
        Q_ASSERT(containerAccess);
        containerAccess->unite(__jni_env, container.first, t0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// QMultiHash<Key, T>::values(const Key &key) const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMultiHash__1_1qt_1QMultiHash_1valuesKey__JLjava_lang_Object_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject k0)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "values(const Key &key) const")
    try{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QHash<QVariant,QVariant>));
        AbstractMultiHashAccess* containerAccess = dynamic_cast<AbstractMultiHashAccess*>(container.second);
        Q_ASSERT(containerAccess);
        return containerAccess->values(__jni_env, container.first, k0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

// QMultiHash<Key, T>::contains(const Key & k, const T & value) const
extern "C" Q_DECL_EXPORT jboolean JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMultiHash__1_1qt_1QMultiHash_1contains__JLjava_lang_Object_2Ljava_lang_Object_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jobject k1)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QMultiHash<Key, T>::contains(const Key & k, const T & value) const")
    try{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QMultiHash<QVariant,QVariant>));
        AbstractMultiHashAccess* containerAccess = dynamic_cast<AbstractMultiHashAccess*>(container.second);
        Q_ASSERT(containerAccess);
        return containerAccess->contains(__jni_env, container.first, t0, k1);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return false;
}

// QMultiHash<Key, T>::count(const Key & k, const T & value) const
extern "C" Q_DECL_EXPORT jint JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMultiHash__1_1qt_1QMultiHash_1count__JLjava_lang_Object_2Ljava_lang_Object_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jobject k1)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QMultiHash<Key, T>::contains(const Key & k, const T & value) const")
    try{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QMultiHash<QVariant,QVariant>));
        AbstractMultiHashAccess* containerAccess = dynamic_cast<AbstractMultiHashAccess*>(container.second);
        Q_ASSERT(containerAccess);
        return containerAccess->count(__jni_env, container.first, t0, k1);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return false;
}

// QMultiHash<Key, T>::find(const Key & k, const T & value) const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMultiHash__1_1qt_1QMultiHash_1find__JLjava_lang_Object_2Ljava_lang_Object_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jobject k1)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QMultiHash<Key, T>::find(const Key & k, const T & value) const")
    try{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QMultiHash<QVariant,QVariant>));
        AbstractMultiHashAccess* containerAccess = dynamic_cast<AbstractMultiHashAccess*>(container.second);
        Q_ASSERT(containerAccess);
        return containerAccess->find(__jni_env, __this_nativeId, container.first, t0, k1);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

// QMultiHash<Key, T>::remove(const Key & k, const T & value)
extern "C" Q_DECL_EXPORT jint JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMultiHash__1_1qt_1QMultiHash_1remove__JLjava_lang_Object_2Ljava_lang_Object_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jobject k1)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QMultiHash<Key, T>::remove(const Key & k, const T & value)")
    try{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QMultiHash<QVariant,QVariant>));
        AbstractMultiHashAccess* containerAccess = dynamic_cast<AbstractMultiHashAccess*>(container.second);
        Q_ASSERT(containerAccess);
        return containerAccess->remove(__jni_env, container.first, t0, k1);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return false;
}

// QMultiHash<Key, T>::replace(const Key & k, const T & value)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMultiHash__1_1qt_1QMultiHash_1replace__JLjava_lang_Object_2Ljava_lang_Object_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jobject k1)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QMultiHash<Key, T>::replace(const Key & k, const T & value)")
    try{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QMultiHash<QVariant,QVariant>));
        AbstractMultiHashAccess* containerAccess = dynamic_cast<AbstractMultiHashAccess*>(container.second);
        Q_ASSERT(containerAccess);
        containerAccess->replace(__jni_env, container.first, t0, k1);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMultiHash__1_1qt_1QMultiHash_1writeTo)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 QtJambiNativeID stream0)
{
    try{
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
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMultiHash__1_1qt_1QMultiHash_1readFrom)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 QtJambiNativeID stream0)
{
    try{
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
        if(!metaType.load(*stream, container.first)){
#endif
            containerName.prepend("QDataStream& >> ");
            JavaException::raiseQNoImplementationException(__jni_env, containerName QTJAMBI_STACKTRACEINFO );
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// emitting (AbstractMetaClass::NormalFunctions|AbstractMetaClass::AbstractFunctions writeFinalFunction)
// emitting Field accessors (writeFieldAccessors)
// emitting (writeInterfaceCastFunction)
// emitting (writeSignalInitialization)
// emitting (writeJavaLangObjectOverrideFunctions)


