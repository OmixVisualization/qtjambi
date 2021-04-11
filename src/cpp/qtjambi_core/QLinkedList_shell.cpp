/****************************************************************************
**
** Copyright (C) 2009-2021 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#include <QtCore/QDataStream>
#include <QtCore/QDebug>
#include <qtjambi/qtjambi_core.h>
#include <qtjambi/qtjambi_repository.h>
#include <qtjambi/qtjambi_containers.h>

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include <QtCore/QLinkedList>
#include <QtCore/QVariant>
#include <qtjambi/qtjambi_cast.h>

QT_WARNING_DISABLE_DEPRECATED

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QLinkedList_initialize)
(JNIEnv * env, jobject _this, jclass elementType, QtJambiNativeID elementMetaType, jobject other)
{
    initialize_QLinkedList(env, _this, elementType, elementMetaType, other);
}

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QLinkedList_elementMetaType)
(JNIEnv * env, jclass, QtJambiNativeID __this_nativeId)
{
    QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
    AbstractLinkedListAccess* containerAccess = dynamic_cast<AbstractLinkedListAccess*>(container.second);
    Q_ASSERT(containerAccess);
    return qtjambi_cast<jobject>(env, containerAccess->elementMetaType());
}

// emitting  (functionsInTargetLang writeFinalFunction)
// QLinkedList<T>::append(const T & t)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QLinkedList__1_1qt_1QLinkedList_1append__JLjava_lang_Object_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QLinkedList<T>::append(const T & t)")
    try{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QLinkedList<QVariant>));
        dynamic_cast<AbstractLinkedListAccess*>(container.second)->append(__jni_env, container.first, t0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// QLinkedList<T>::first() const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QLinkedList__1_1qt_1QLinkedList_1first__J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QLinkedList<T>::first() const")
    try{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QLinkedList<QVariant>));
        return dynamic_cast<AbstractLinkedListAccess*>(container.second)->first(__jni_env, container.first);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

// QLinkedList<T>::last() const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QLinkedList__1_1qt_1QLinkedList_1last__J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QLinkedList<T>::last() const")
    try{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QLinkedList<QVariant>));
        return dynamic_cast<AbstractLinkedListAccess*>(container.second)->last(__jni_env, container.first);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

// QLinkedList<T>::begin() const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QLinkedList__1_1qt_1QLinkedList_1begin__J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QLinkedList<T>::begin() const")
    try{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QLinkedList<QVariant>));
        return dynamic_cast<AbstractLinkedListAccess*>(container.second)->begin(__jni_env, __this_nativeId, container.first);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

// QLinkedList<T>::clear()
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QLinkedList__1_1qt_1QLinkedList_1clear__J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QLinkedList<T>::clear()")
    try{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QLinkedList<QVariant>));
        return dynamic_cast<AbstractLinkedListAccess*>(container.second)->clear(__jni_env, container.first);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// QLinkedList<T>::contains(const T & t) const
extern "C" Q_DECL_EXPORT jboolean JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QLinkedList__1_1qt_1QLinkedList_1contains__JLjava_lang_Object_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QLinkedList<T>::contains(const T & t) const")
    try{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QLinkedList<QVariant>));
        return dynamic_cast<AbstractLinkedListAccess*>(container.second)->contains(__jni_env, container.first, t0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return false;
}

// QLinkedList<T>::count(const T & t) const
extern "C" Q_DECL_EXPORT jint JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QLinkedList__1_1qt_1QLinkedList_1count__JLjava_lang_Object_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QLinkedList<T>::count(const T & t) const")
    try{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QLinkedList<QVariant>));
        return dynamic_cast<AbstractLinkedListAccess*>(container.second)->count(__jni_env, container.first, t0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return 0;
}

// QLinkedList<T>::end() const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QLinkedList__1_1qt_1QLinkedList_1end__J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QLinkedList<T>::end() const")
    try{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QLinkedList<QVariant>));
        return dynamic_cast<AbstractLinkedListAccess*>(container.second)->end(__jni_env, __this_nativeId, container.first);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

// QLinkedList<T>::endsWith(const T & t) const
extern "C" Q_DECL_EXPORT jboolean JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QLinkedList__1_1qt_1QLinkedList_1endsWith__JLjava_lang_Object_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QLinkedList<T>::endsWith(const T & t) const")
    try{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QLinkedList<QVariant>));
        return dynamic_cast<AbstractLinkedListAccess*>(container.second)->endsWith(__jni_env, container.first, t0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return false;
}

// QLinkedList<T>::operator==(const QLinkedList & l) const
extern "C" Q_DECL_EXPORT jboolean JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QLinkedList__1_1qt_1QLinkedList_1operator_1equal__JLjava_util_Collection_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject l0)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QLinkedList<T>::operator==(const QLinkedList<T> & l) const")
    try{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QLinkedList<QVariant>));
        return dynamic_cast<AbstractLinkedListAccess*>(container.second)->equal(__jni_env, container.first, l0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return false;
}

// QLinkedList<T>::prepend(const T & t)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QLinkedList__1_1qt_1QLinkedList_1prepend__JLjava_lang_Object_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QLinkedList<T>::prepend(const T & t)")
    try{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QLinkedList<QVariant>));
        dynamic_cast<AbstractLinkedListAccess*>(container.second)->prepend(__jni_env, container.first, t0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// QLinkedList<T>::removeAll(const T & t)
extern "C" Q_DECL_EXPORT jint JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QLinkedList__1_1qt_1QLinkedList_1removeAll__JLjava_lang_Object_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QLinkedList<T>::removeAll(const T & t)")
    try{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QLinkedList<QVariant>));
        return dynamic_cast<AbstractLinkedListAccess*>(container.second)->removeAll(__jni_env, container.first, t0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return -1;
}

// QLinkedList<T>::removeFirst()
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QLinkedList__1_1qt_1QLinkedList_1removeFirst__J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QLinkedList<T>::removeFirst()")
    try{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QLinkedList<QVariant>));
        dynamic_cast<AbstractLinkedListAccess*>(container.second)->removeFirst(__jni_env, container.first);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// QLinkedList<T>::removeLast()
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QLinkedList__1_1qt_1QLinkedList_1removeLast__J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QLinkedList<T>::removeLast()")
    try{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QLinkedList<QVariant>));
        dynamic_cast<AbstractLinkedListAccess*>(container.second)->removeLast(__jni_env, container.first);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// QLinkedList<T>::removeOne(const T & t)
extern "C" Q_DECL_EXPORT jboolean JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QLinkedList__1_1qt_1QLinkedList_1removeOne__JLjava_lang_Object_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QLinkedList<T>::removeOne(const T & t)")
    try{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QLinkedList<QVariant>));
        return dynamic_cast<AbstractLinkedListAccess*>(container.second)->removeOne(__jni_env, container.first, t0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return false;
}

// QLinkedList<T>::size() const
extern "C" Q_DECL_EXPORT jint JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QLinkedList__1_1qt_1QLinkedList_1size__J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QLinkedList<T>::size() const")
    try{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QLinkedList<QVariant>));
        return dynamic_cast<AbstractLinkedListAccess*>(container.second)->size(__jni_env, container.first);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return 0;
}

// QLinkedList<T>::startsWith(const T & t) const
extern "C" Q_DECL_EXPORT jboolean JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QLinkedList__1_1qt_1QLinkedList_1startsWith__JLjava_lang_Object_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QLinkedList<T>::startsWith(const T & t) const")
    try{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QLinkedList<QVariant>));
        return dynamic_cast<AbstractLinkedListAccess*>(container.second)->startsWith(__jni_env, container.first, t0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return false;
}

// QLinkedList<T>::takeFirst()
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QLinkedList__1_1qt_1QLinkedList_1takeFirst__J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QLinkedList<T>::takeFirst()")
    try{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QLinkedList<QVariant>));
        return dynamic_cast<AbstractLinkedListAccess*>(container.second)->takeFirst(__jni_env, container.first);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

// QLinkedList<T>::takeLast()
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QLinkedList__1_1qt_1QLinkedList_1takeLast__J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QLinkedList<T>::takeLast()")
    try{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QLinkedList<QVariant>));
        return dynamic_cast<AbstractLinkedListAccess*>(container.second)->takeLast(__jni_env, container.first);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QLinkedList__1_1qt_1QLinkedList_1writeTo)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 QtJambiNativeID stream0)
{
    try{
        const QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        QDataStream* stream = qtjambi_object_from_nativeId<QDataStream>(stream0);
        qtjambi_check_resource(__jni_env, stream, typeid(QDataStream));
        QByteArray containerName = "QLinkedList<";
        containerName += dynamic_cast<AbstractLinkedListAccess*>(container.second)->elementMetaType().name();
        containerName += ">";
        int metaType = container.second->registerContainer(containerName);
        if(!QMetaType::save(*stream, metaType, container.first)){
            containerName.prepend("QDataStream& << ");
            JavaException::raiseQNoImplementationException(__jni_env, containerName QTJAMBI_STACKTRACEINFO );
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QLinkedList__1_1qt_1QLinkedList_1readFrom)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 QtJambiNativeID stream0)
{
    try{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QLinkedList<QVariant>));
        QDataStream* stream = qtjambi_object_from_nativeId<QDataStream>(stream0);
        qtjambi_check_resource(__jni_env, stream, typeid(QDataStream));
        QByteArray containerName = "QLinkedList<";
        containerName += dynamic_cast<AbstractLinkedListAccess*>(container.second)->elementMetaType().name();
        containerName += ">";
        int metaType = container.second->registerContainer(containerName);
        if(!QMetaType::save(*stream, metaType, container.first)){
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

#endif //QT_VERSION < QT_VERSION_CHECK(6, 0, 0)

