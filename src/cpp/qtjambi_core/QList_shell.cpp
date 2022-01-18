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
#include <qtjambi/qtjambi_registry.h>
#include <qtjambi/qtjambi_repository.h>
#include <qtjambi/qtjambi_containers.h>
#include <QtCore/QList>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtCore/QItemSelection>
#include <qtjambi/qtjambi_cast.h>

using namespace QtJambiPrivate;

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList_initialize)
(JNIEnv * env, jobject _this, jclass elementType, QtJambiNativeID elementMetaType, jobject other)
{
    initialize_QList(env, _this, elementType, elementMetaType, other);
}

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList_elementMetaType)
(JNIEnv * env, jclass, QtJambiNativeID __this_nativeId)
{
    QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
    qtjambi_check_resource(env, container.first, typeid(QList<QVariant>));
    AbstractListAccess* containerAccess = dynamic_cast<AbstractListAccess*>(container.second);
    Q_ASSERT(containerAccess);
    return qtjambi_cast<jobject>(env, containerAccess->elementMetaType());
}

// QList<T>::append(const QList<T> & t)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList__1_1qt_1QList_1appendList__JLjava_util_Collection_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QList<T>::append(const QList<T> & t)")
    try{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QList<QVariant>));
        AbstractListAccess* containerAccess = dynamic_cast<AbstractListAccess*>(container.second);
        Q_ASSERT(containerAccess);
        containerAccess->appendList(__jni_env, container.first, t0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// QList<T>::append(const T & t)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList__1_1qt_1QList_1append__JLjava_lang_Object_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QList<T>::append(const T & t)")
    try{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QList<QVariant>));
        AbstractListAccess* containerAccess = dynamic_cast<AbstractListAccess*>(container.second);
        Q_ASSERT(containerAccess);
        containerAccess->append(__jni_env, container.first, t0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// QList<T>::at(int i) const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList__1_1qt_1QList_1at__JI)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jint i0)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QList<T>::at(int i) const")
    try{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QList<QVariant>));
        AbstractListAccess* containerAccess = dynamic_cast<AbstractListAccess*>(container.second);
        Q_ASSERT(containerAccess);
        return containerAccess->at(__jni_env, container.first, i0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

// QList<T>::begin() const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList__1_1qt_1QList_1begin__J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QList<T>::begin() const")
    try{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QList<QVariant>));
        AbstractListAccess* containerAccess = dynamic_cast<AbstractListAccess*>(container.second);
        Q_ASSERT(containerAccess);
        return containerAccess->begin(__jni_env, __this_nativeId, container.first);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

// QList<T>::clear()
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList__1_1qt_1QList_1clear__J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QList<T>::clear()")
    try{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QList<QVariant>));
        AbstractListAccess* containerAccess = dynamic_cast<AbstractListAccess*>(container.second);
        Q_ASSERT(containerAccess);
        return containerAccess->clear(__jni_env, container.first);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// QList<T>::contains(const T & t) const
extern "C" Q_DECL_EXPORT jboolean JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList__1_1qt_1QList_1contains__JLjava_lang_Object_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QList<T>::contains(const T & t) const")
    try{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QList<QVariant>));
        AbstractListAccess* containerAccess = dynamic_cast<AbstractListAccess*>(container.second);
        Q_ASSERT(containerAccess);
        return containerAccess->contains(__jni_env, container.first, t0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return false;
}

// QList<T>::count(const T & t) const
extern "C" Q_DECL_EXPORT jint JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList__1_1qt_1QList_1count__JLjava_lang_Object_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QList<T>::count(const T & t) const")
    try{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QList<QVariant>));
        AbstractListAccess* containerAccess = dynamic_cast<AbstractListAccess*>(container.second);
        Q_ASSERT(containerAccess);
        return containerAccess->count(__jni_env, container.first, t0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return 0;
}

// QList<T>::end() const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList__1_1qt_1QList_1end__J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QList<T>::end() const")
    try{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QList<QVariant>));
        AbstractListAccess* containerAccess = dynamic_cast<AbstractListAccess*>(container.second);
        Q_ASSERT(containerAccess);
        return containerAccess->end(__jni_env, __this_nativeId, container.first);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

// QList<T>::endsWith(const T & t) const
extern "C" Q_DECL_EXPORT jboolean JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList__1_1qt_1QList_1endsWith__JLjava_lang_Object_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QList<T>::endsWith(const T & t) const")
    try{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QList<QVariant>));
        AbstractListAccess* containerAccess = dynamic_cast<AbstractListAccess*>(container.second);
        Q_ASSERT(containerAccess);
        return containerAccess->endsWith(__jni_env, container.first, t0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return false;
}

// QList<T>::indexOf(const T & t, int from) const
extern "C" Q_DECL_EXPORT jint JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList__1_1qt_1QList_1indexOf__JLjava_lang_Object_2I)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jint from1)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QList<T>::indexOf(const T & t, int from) const")
    try{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QList<QVariant>));
        AbstractListAccess* containerAccess = dynamic_cast<AbstractListAccess*>(container.second);
        Q_ASSERT(containerAccess);
        return containerAccess->indexOf(__jni_env, container.first, t0, from1);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return -1;
}

// QList<T>::insert(int i, const T & t)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList__1_1qt_1QList_1insert__JILjava_lang_Object_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jint i0,
 jobject t1)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QList<T>::insert(int i, const T & t)")
    try{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QList<QVariant>));
        AbstractListAccess* containerAccess = dynamic_cast<AbstractListAccess*>(container.second);
        Q_ASSERT(containerAccess);
        containerAccess->insert(__jni_env, container.first, i0, t1);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// QList<T>::lastIndexOf(const T & t, int from) const
extern "C" Q_DECL_EXPORT jint JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList__1_1qt_1QList_1lastIndexOf__JLjava_lang_Object_2I)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jint from1)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QList<T>::lastIndexOf(const T & t, int from) const")
    try{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QList<QVariant>));
        AbstractListAccess* containerAccess = dynamic_cast<AbstractListAccess*>(container.second);
        Q_ASSERT(containerAccess);
        return containerAccess->lastIndexOf(__jni_env, container.first, t0, from1);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return -1;
}

// QList<T>::mid(int pos, int length) const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList__1_1qt_1QList_1mid__JII)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jint pos0,
 jint length1)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QList<T>::mid(int pos, int length) const")
    try{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QList<QVariant>));
        AbstractListAccess* containerAccess = dynamic_cast<AbstractListAccess*>(container.second);
        Q_ASSERT(containerAccess);
        return containerAccess->mid(__jni_env, container.first, pos0, length1);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

// QList<T>::move(int from, int to)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList__1_1qt_1QList_1move__JII)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jint from0,
 jint to1)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QList<T>::move(int from, int to)")
    try{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QList<QVariant>));
        AbstractListAccess* containerAccess = dynamic_cast<AbstractListAccess*>(container.second);
        Q_ASSERT(containerAccess);
        containerAccess->move(__jni_env, container.first, from0, to1);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// QList<T>::operator==(const QList & l) const
extern "C" Q_DECL_EXPORT jboolean JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList__1_1qt_1QList_1operator_1equal__JLjava_util_Collection_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject l0)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QList<T>::operator==(const QList<T> & l) const")
    try{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QList<QVariant>));
        AbstractListAccess* containerAccess = dynamic_cast<AbstractListAccess*>(container.second);
        Q_ASSERT(containerAccess);
        return containerAccess->equal(__jni_env, container.first, l0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return false;
}

// QList<T>::prepend(const T & t)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList__1_1qt_1QList_1prepend__JLjava_lang_Object_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QList<T>::prepend(const T & t)")
    try{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QList<QVariant>));
        AbstractListAccess* containerAccess = dynamic_cast<AbstractListAccess*>(container.second);
        Q_ASSERT(containerAccess);
        containerAccess->prepend(__jni_env, container.first, t0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// QList<T>::removeAll(const T & t)
extern "C" Q_DECL_EXPORT jint JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList__1_1qt_1QList_1removeAll__JLjava_lang_Object_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QList<T>::removeAll(const T & t)")
    try{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QList<QVariant>));
        AbstractListAccess* containerAccess = dynamic_cast<AbstractListAccess*>(container.second);
        Q_ASSERT(containerAccess);
        return containerAccess->removeAll(__jni_env, container.first, t0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return -1;
}

// QList<T>::removeAt(int i)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList__1_1qt_1QList_1removeAt__JI)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jint i0)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QList<T>::removeAt(int i)")
    try{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QList<QVariant>));
        AbstractListAccess* containerAccess = dynamic_cast<AbstractListAccess*>(container.second);
        Q_ASSERT(containerAccess);
        containerAccess->removeAt(__jni_env, container.first, i0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// QList<T>::removeOne(const T & t)
extern "C" Q_DECL_EXPORT jboolean JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList__1_1qt_1QList_1removeOne__JLjava_lang_Object_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QList<T>::removeOne(const T & t)")
    try{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QList<QVariant>));
        AbstractListAccess* containerAccess = dynamic_cast<AbstractListAccess*>(container.second);
        Q_ASSERT(containerAccess);
        return containerAccess->removeOne(__jni_env, container.first, t0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return false;
}

// QList<T>::replace(int i, const T & t)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList__1_1qt_1QList_1replace__JILjava_lang_Object_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jint i0,
 jobject t1)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QList<T>::replace(int i, const T & t)")
    try{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QList<QVariant>));
        AbstractListAccess* containerAccess = dynamic_cast<AbstractListAccess*>(container.second);
        Q_ASSERT(containerAccess);
        containerAccess->replace(__jni_env, container.first, i0, t1);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// QList<T>::reserve(int size)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList__1_1qt_1QList_1reserve__JI)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jint size0)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QList<T>::reserve(int size)")
    try{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QList<QVariant>));
        AbstractListAccess* containerAccess = dynamic_cast<AbstractListAccess*>(container.second);
        Q_ASSERT(containerAccess);
        containerAccess->reserve(__jni_env, container.first, size0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// QList<T>::size() const
extern "C" Q_DECL_EXPORT jint JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList__1_1qt_1QList_1size__J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QList<T>::size() const")
    try{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QList<QVariant>));
        AbstractListAccess* containerAccess = dynamic_cast<AbstractListAccess*>(container.second);
        Q_ASSERT(containerAccess);
        return containerAccess->size(__jni_env, container.first);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return 0;
}

// QList<T>::startsWith(const T & t) const
extern "C" Q_DECL_EXPORT jboolean JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList__1_1qt_1QList_1startsWith__JLjava_lang_Object_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QList<T>::startsWith(const T & t) const")
    try{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QList<QVariant>));
        AbstractListAccess* containerAccess = dynamic_cast<AbstractListAccess*>(container.second);
        Q_ASSERT(containerAccess);
        return containerAccess->startsWith(__jni_env, container.first, t0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return false;
}

// QList<T>::swapItemsAt(int i, int j)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList__1_1qt_1QList_1swapItemsAt__JII)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jint i0,
 jint j1)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QList<T>::swap(int i, int j)")
    try{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QList<QVariant>));
        AbstractListAccess* containerAccess = dynamic_cast<AbstractListAccess*>(container.second);
        Q_ASSERT(containerAccess);
        containerAccess->swapItemsAt(__jni_env, container.first, i0, j1);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// QList<T>::takeAt(int i)
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList__1_1qt_1QList_1takeAt__JI)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jint i0)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QList<T>::takeAt(int i)")
    try{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QList<QVariant>));
        AbstractListAccess* containerAccess = dynamic_cast<AbstractListAccess*>(container.second);
        Q_ASSERT(containerAccess);
        return containerAccess->takeAt(__jni_env, container.first, i0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
// QList<T>::toSet() const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList__1_1qt_1QList_1toSet__J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QList<T>::toSet() const")
    try{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QList<QVariant>));
        AbstractListAccess* containerAccess = dynamic_cast<AbstractListAccess*>(container.second);
        Q_ASSERT(containerAccess);
        return containerAccess->toSet(__jni_env, container.first);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}
#endif

// QList<T>::value(int i) const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList__1_1qt_1QList_1value__JI)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jint i0)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QList<T>::value(int i) const")
    try{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QList<QVariant>));
        AbstractListAccess* containerAccess = dynamic_cast<AbstractListAccess*>(container.second);
        Q_ASSERT(containerAccess);
        return containerAccess->value(__jni_env, container.first, i0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

// QList<T>::value(int i, const T & defaultValue) const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList__1_1qt_1QList_1valueDefault__JILjava_lang_Object_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jint i0,
 jobject defaultValue1)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QList<T>::value(int i, const T & defaultValue) const")
    try{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QList<QVariant>));
        AbstractListAccess* containerAccess = dynamic_cast<AbstractListAccess*>(container.second);
        Q_ASSERT(containerAccess);
        return containerAccess->value(__jni_env, container.first, i0, defaultValue1);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList__1_1qt_1QList_1writeTo)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 QtJambiNativeID stream0)
{
    try{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QList<QVariant>));
        AbstractListAccess* containerAccess = dynamic_cast<AbstractListAccess*>(container.second);
        Q_ASSERT(containerAccess);
        QDataStream* stream = qtjambi_object_from_nativeId<QDataStream>(stream0);
        qtjambi_check_resource(__jni_env, stream, typeid(QDataStream));
        QByteArray containerName = "QList<";
        containerName += containerAccess->elementMetaType().name();
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

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList__1_1qt_1QList_1readFrom)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 QtJambiNativeID stream0)
{
    try{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QList<QVariant>));
        AbstractListAccess* containerAccess = dynamic_cast<AbstractListAccess*>(container.second);
        Q_ASSERT(containerAccess);
        QDataStream* stream = qtjambi_object_from_nativeId<QDataStream>(stream0);
        qtjambi_check_resource(__jni_env, stream, typeid(QDataStream));
        QByteArray containerName = "QList<";
        containerName += containerAccess->elementMetaType().name();
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

#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
// QList<T>::squeeze()
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList__1_1qt_1QList_1squeeze__J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QList<T>::squeeze()")
    try{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QList<QVariant>));
        AbstractListAccess* containerAccess = dynamic_cast<AbstractListAccess*>(container.second);
        Q_ASSERT(containerAccess);
        containerAccess->squeeze(__jni_env, container.first);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// QList<T>::capacity() const
extern "C" Q_DECL_EXPORT jint JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList__1_1qt_1QList_1capacity__J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QList<T>::capacity() const")
    try{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QList<QVariant>));
        AbstractListAccess* containerAccess = dynamic_cast<AbstractListAccess*>(container.second);
        Q_ASSERT(containerAccess);
        return containerAccess->capacity(__jni_env, container.first);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return 0;
}

// QList<T>::fill(const T &value, int size = ...)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList__1_1qt_1QList_1fill__JLjava_lang_Object_2I)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject value0,
 jint size1)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QList<T>::fill(const T &value, int size = ...)")
    try{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QList<QVariant>));
        AbstractListAccess* containerAccess = dynamic_cast<AbstractListAccess*>(container.second);
        Q_ASSERT(containerAccess);
        containerAccess->fill(__jni_env, container.first, value0, size1);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// QList<T>::remove(int i, int count)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList__1_1qt_1QList_1removeN__JII)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jint i0,
 jint count1)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QList<T>::remove(int i, int count)")
    try{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QList<QVariant>));
        AbstractListAccess* containerAccess = dynamic_cast<AbstractListAccess*>(container.second);
        Q_ASSERT(containerAccess);
        containerAccess->remove(__jni_env, container.first, i0, count1);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// QList<T>::insert(int i, int count, const T & t)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList__1_1qt_1QList_1insert__JIILjava_lang_Object_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jint i0,
 jint count1,
 jobject t2)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QList<T>::insert(int i, int count, const T & t)")
    try{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QList<QVariant>));
        AbstractListAccess* containerAccess = dynamic_cast<AbstractListAccess*>(container.second);
        Q_ASSERT(containerAccess);
        containerAccess->insert(__jni_env, container.first, i0, count1, t2);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// QList<T>::resize(int size)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList__1_1qt_1QList_1resize__JI)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jint size0)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QList<T>::resize(int size)")
    try{
        QPair<void*,AbstractContainerAccess*> container = qtjambi_container_from_nativeId(__this_nativeId);
        qtjambi_check_resource(__jni_env, container.first, typeid(QList<QVariant>));
        AbstractListAccess* containerAccess = dynamic_cast<AbstractListAccess*>(container.second);
        Q_ASSERT(containerAccess);
        containerAccess->resize(__jni_env, container.first, size0);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// new QItemSelection(QItemSelection)
#if QT_CONFIG(itemmodel)
void __qt_construct_QItemSelection_1(void* __qtjambi_ptr, JNIEnv* __jni_env, jobject __jni_object, jvalue* __java_arguments)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "new QItemSelection(const QModelIndex & topLeft, const QModelIndex & bottomRight)")
    Q_ASSERT(__java_arguments);
    QItemSelection *__qt_this = new(__qtjambi_ptr) QItemSelection();
    if(__java_arguments->l){
        QtJambiScope scope;
        __qt_this->append(qtjambi_cast<const QList<QItemSelectionRange>&>(__jni_env, scope, __java_arguments->l));
    }
    Q_UNUSED(__jni_object)
    Q_UNUSED(__qt_this)
}
#endif // QT_CONFIG(itemmodel)

// emitting  (functionsInTargetLang writeFinalFunction)
// QItemSelection::QItemSelection(QItemSelection)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QItemSelection_initialize_1native__Lio_qt_core_QItemSelection_2Ljava_util_Collection_2)
(JNIEnv *__jni_env,
 jclass __jni_class,
 jobject __jni_object,
 jobject other)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QItemSelection::QItemSelection(QItemSelection)")
    try{
#if QT_CONFIG(itemmodel)
        jvalue arguments;
        arguments.l = other;
        qtjambi_initialize_native_container(__jni_env, __jni_class, __jni_object, &__qt_construct_QItemSelection_1, sizeof(QItemSelection), typeid(QItemSelection), false, QtJambiPrivate::QListAccess<QItemSelectionRange>::newInstance(), &arguments);
#else
        Q_UNUSED(__jni_object)
        Q_UNUSED(__jni_class)
        Q_UNUSED(topLeft0)
        Q_UNUSED(bottomRight1)
        JavaException::raiseQNoImplementationException(__jni_env, "The method has no implementation on this platform." QTJAMBI_STACKTRACEINFO );
#endif // QT_CONFIG(itemmodel)
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// new QStringList(QStringList)
void __qt_construct_QStringList_1(void* __qtjambi_ptr, JNIEnv* __jni_env, jobject __jni_object, jvalue* __java_arguments)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "new QStringList()")
    Q_ASSERT(__java_arguments);
    QtJambiScope scope;
    QStringList *__qt_this = new(__qtjambi_ptr) QStringList(qtjambi_cast<const QStringList&>(__jni_env, scope, __java_arguments->l));
    Q_UNUSED(__jni_object)
    Q_UNUSED(__jni_env)
    Q_UNUSED(__qt_this)
}

// emitting  (functionsInTargetLang writeFinalFunction)
// QStringList::QStringList(QStringList)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QStringList_initialize_1native__Lio_qt_core_QStringList_2Ljava_util_Collection_2)
(JNIEnv *__jni_env,
 jclass __jni_class,
 jobject __jni_object,
 jobject __jni_other)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QStringList::QStringList(QStringList)")
    try{
        jvalue argument;
        argument.l = __jni_other;
        qtjambi_initialize_native_container(__jni_env, __jni_class, __jni_object, &__qt_construct_QStringList_1, sizeof(QStringList), typeid(QStringList), false, QtJambiPrivate::QListAccess<QString>::newInstance(), &argument);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }

}
#endif



