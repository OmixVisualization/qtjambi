/****************************************************************************
**
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
#include <QtJambi/RegistryAPI>
#include <QtJambi/JavaAPI>
#include <QtJambi/ContainerAPI>
#include <QtCore/QList>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtCore/QItemSelection>
#include <QtJambi/CoreAPI>
#include "utils_p.h"
#include <QtJambi/qtjambi_cast.h>

using namespace QtJambiPrivate;

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList_initialize)
(JNIEnv * env, jobject _this, jclass elementType, QtJambiNativeID elementMetaType, jobject other)
{
    QTJAMBI_TRY{
        QTJAMBI_NATIVE_METHOD_CALL("QList<T>::initialize()")
        CoreAPI::initializeQList(env, _this, elementType, elementMetaType, other);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList_elementMetaType)
(JNIEnv * env, jclass, QtJambiNativeID __this_nativeId)
{
    jobject result{nullptr};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(env, container.first, typeid(QList<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QList<T>::elementMetaType()", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractListAccess, containerAccess, container.second);
        result = qtjambi_cast<jobject>(env, containerAccess->elementMetaType());
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return result;
}

// QList<T>::append(const QList<T> & t)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList_appendList__JLjava_util_Collection_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0)
{
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QList<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QList<T>::append(const QList<T> & t)", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractListAccess, containerAccess, container.second);
        containerAccess->appendList(__jni_env, container.first, t0);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

// QList<T>::at(int i) const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList_at__JI)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jint i0)
{
    jobject result{nullptr};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QList<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QList<T>::at(int i) const", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractListAccess, containerAccess, container.second);
        jint size = containerAccess->size(__jni_env, container.first);
        if (i0 >= size || i0 < 0) {
            JavaException::raiseIndexOutOfBoundsException(__jni_env, QString("Accessing container of size %1 at %2").arg(size).arg(i0) QTJAMBI_STACKTRACEINFO );
        }
        result = containerAccess->at(__jni_env, container.first, i0);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QList<T>::last() const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList_last__J)
    (JNIEnv *__jni_env,
     jclass,
     QtJambiNativeID __this_nativeId)
{
    jobject result{nullptr};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QList<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QList<T>::last() const", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractListAccess, containerAccess, container.second);
        jint size = containerAccess->size(__jni_env, container.first);
        if (size == 0) {
            JavaException::raiseIndexOutOfBoundsException(__jni_env, QString("Accessing container of size 0 at 0") QTJAMBI_STACKTRACEINFO );
        }
        result = containerAccess->at(__jni_env, container.first, size-1);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
        return result;
}

// QList<T>::begin()
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList_begin__J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId)
{
    jobject result{nullptr};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QList<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QList<T>::begin()", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractListAccess, containerAccess, container.second);
        result = containerAccess->begin(__jni_env, __this_nativeId, container.first);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QList<T>::end()
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList_end__J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId)
{
    jobject result{nullptr};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QList<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QList<T>::end()", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractListAccess, containerAccess, container.second);
        result = containerAccess->end(__jni_env, __this_nativeId, container.first);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QList<T>::constBegin()const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList_constBegin__J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId)
{
    jobject result{nullptr};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QList<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QList<T>::constBegin()const", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractListAccess, containerAccess, container.second);
        result = containerAccess->constBegin(__jni_env, __this_nativeId, container.first);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QList<T>::constEnd()const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList_constEnd__J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId)
{
    jobject result{nullptr};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QList<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QList<T>::constEnd()const", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractListAccess, containerAccess, container.second);
        result = containerAccess->constEnd(__jni_env, __this_nativeId, container.first);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QList<T>::clear()
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList_clear__J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId)
{
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QList<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QList<T>::clear()", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractListAccess, containerAccess, container.second);
        containerAccess->clear(__jni_env, container.first);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

// QList<T>::contains(const T & t) const
extern "C" Q_DECL_EXPORT jboolean JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList_contains__JLjava_lang_Object_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0)
{
    jboolean result{false};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QList<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QList<T>::contains(const T & t) const", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractListAccess, containerAccess, container.second);
        result = containerAccess->contains(__jni_env, container.first, t0);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QList<T>::count(const T & t) const
extern "C" Q_DECL_EXPORT jint JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList_count__JLjava_lang_Object_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0)
{
    jint result{0};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QList<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QList<T>::count(const T & t) const", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractListAccess, containerAccess, container.second);
        result = containerAccess->count(__jni_env, container.first, t0);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QList<T>::endsWith(const T & t) const
extern "C" Q_DECL_EXPORT jboolean JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList_endsWith__JLjava_lang_Object_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0)
{
    jboolean result{false};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QList<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QList<T>::endsWith(const T & t) const", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractListAccess, containerAccess, container.second);
        result = containerAccess->endsWith(__jni_env, container.first, t0);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QList<T>::indexOf(const T & t, int from) const
extern "C" Q_DECL_EXPORT jint JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList_indexOf__JLjava_lang_Object_2I)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jint from1)
{
    jint result{-1};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QList<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QList<T>::indexOf(const T & t, int from) const", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractListAccess, containerAccess, container.second);
        result = containerAccess->indexOf(__jni_env, container.first, t0, from1);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QList<T>::insert(int i, const T & t)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList_insert__JILjava_lang_Object_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jint i0,
 jobject t1)
{
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QList<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QList<T>::insert(int i, const T & t)", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractListAccess, containerAccess, container.second);
        containerAccess->insert(__jni_env, container.first, i0, 1, t1);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

// QList<T>::lastIndexOf(const T & t, int from) const
extern "C" Q_DECL_EXPORT jint JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList_lastIndexOf__JLjava_lang_Object_2I)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jint from1)
{
    jint result{-1};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QList<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QList<T>::lastIndexOf(const T & t, int from) const", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractListAccess, containerAccess, container.second);
        result = containerAccess->lastIndexOf(__jni_env, container.first, t0, from1);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QList<T>::mid(int pos, int length) const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList_mid__JII)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jint pos0,
 jint length1)
{
    jobject result{nullptr};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QList<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QList<T>::mid(int pos, int length) const", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractListAccess, containerAccess, container.second);
        jint size = containerAccess->size(__jni_env, container.first);
        if (pos0 >= size || pos0 < 0) {
            JavaException::raiseIndexOutOfBoundsException(__jni_env, QString("Accessing container of size %1 at %2").arg(size).arg(pos0) QTJAMBI_STACKTRACEINFO );
        }
        if (length1>=0 && pos0+length1 > size) {
            JavaException::raiseIndexOutOfBoundsException(__jni_env, QString("Accessing container of size %1 at %2 to %3").arg(size).arg(pos0).arg(pos0+length1) QTJAMBI_STACKTRACEINFO );
        }
        result = containerAccess->mid(__jni_env, container.first, pos0, length1);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QList<T>::move(int from, int to)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList_move__JII)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jint from0,
 jint to1)
{
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QList<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QList<T>::move(int from, int to)", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractListAccess, containerAccess, container.second);
        jint size = containerAccess->size(__jni_env, container.first);
        if (from0 >= size || from0 < 0) {
            JavaException::raiseIndexOutOfBoundsException(__jni_env, QString("Accessing container of size %1 at %2").arg(size).arg(from0) QTJAMBI_STACKTRACEINFO );
        }
        if (to1 >= size && to1 < 0) {
            JavaException::raiseIndexOutOfBoundsException(__jni_env, QString("Accessing container of size %1 at %2").arg(size).arg(to1) QTJAMBI_STACKTRACEINFO );
        }
        containerAccess->move(__jni_env, container.first, from0, to1);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

// QList<T>::operator==(const QList & l) const
extern "C" Q_DECL_EXPORT jboolean JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList_operator_1equal__JLjava_util_Collection_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject l0)
{
    jboolean result{false};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QList<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QList<T>::operator==(const QList<T> & l) const", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractListAccess, containerAccess, container.second);
        result = containerAccess->equal(__jni_env, container.first, l0);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QList<T>::removeAll(const T & t)
extern "C" Q_DECL_EXPORT jint JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList_removeAll__JLjava_lang_Object_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0)
{
    jint result{-1};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QList<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QList<T>::removeAll(const T & t)", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractListAccess, containerAccess, container.second);

        result = containerAccess->removeAll(__jni_env, container.first, t0);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QList<T>::replace(int i, const T & t)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList_replace__JILjava_lang_Object_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jint i0,
 jobject t1)
{
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QList<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QList<T>::replace(int i, const T & t)", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractListAccess, containerAccess, container.second);

        jint size = containerAccess->size(__jni_env, container.first);
        if (i0 >= size || i0 < 0) {
            JavaException::raiseIndexOutOfBoundsException(__jni_env, QString("Accessing container of size %1 at %2").arg(size).arg(i0) QTJAMBI_STACKTRACEINFO );
        }
        containerAccess->replace(__jni_env, container.first, i0, t1);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

// QList<T>::removeOne(const T & t)
extern "C" Q_DECL_EXPORT jboolean JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList_removeOne__JLjava_lang_Object_2)
    (JNIEnv *__jni_env,
     jclass,
     QtJambiNativeID __this_nativeId,
     jobject t1)
{
    jboolean result = false;
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QList<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QList<T>::removeOne(const T & t)", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractListAccess, containerAccess, container.second);

        jint i0 = containerAccess->indexOf(__jni_env, container.first, t1, 0);
        if ((result = i0 >= 0))
            containerAccess->remove(__jni_env, container.first, i0, 1);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
        return result;
}

// QList<T>::takeAt(int i)
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList_takeAt__JI)
    (JNIEnv *__jni_env,
     jclass,
     QtJambiNativeID __this_nativeId,
     jint i0)
{
        jobject result = nullptr;
        QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QList<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QList<T>::reserve(int size)", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractListAccess, containerAccess, container.second);

        jint size = containerAccess->size(__jni_env, container.first);
        if (i0 >= size || i0 < 0) {
            JavaException::raiseIndexOutOfBoundsException(__jni_env, QString("Accessing container of size %1 at %2").arg(size).arg(i0) QTJAMBI_STACKTRACEINFO );
        }
        result = containerAccess->at(__jni_env, container.first, i0);
        containerAccess->remove(__jni_env, container.first, i0, 1);
        }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
        }QTJAMBI_TRY_END
        return result;
}

// QList<T>::takeLast()
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList_takeLast__J)
    (JNIEnv *__jni_env,
     jclass,
     QtJambiNativeID __this_nativeId)
{
        jobject result = nullptr;
        QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QList<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QList<T>::reserve(int size)", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractListAccess, containerAccess, container.second);

        jint size = containerAccess->size(__jni_env, container.first);
        if (size == 0) {
            JavaException::raiseIndexOutOfBoundsException(__jni_env, QString("Accessing container of size 0 at 0") QTJAMBI_STACKTRACEINFO );
        }
        result = containerAccess->at(__jni_env, container.first, size-1);
        containerAccess->remove(__jni_env, container.first, size-1, 1);
        }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
        }QTJAMBI_TRY_END
            return result;
}

// QList<T>::reserve(int size)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList_reserve__JI)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jint size0)
{
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QList<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QList<T>::reserve(int size)", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractListAccess, containerAccess, container.second);

        containerAccess->reserve(__jni_env, container.first, size0);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

// QList<T>::size() const
extern "C" Q_DECL_EXPORT jint JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList_size__J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId)
{
    jint result{0};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QList<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QList<T>::size() const", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractListAccess, containerAccess, container.second);

        result = containerAccess->size(__jni_env, container.first);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QList<T>::startsWith(const T & t) const
extern "C" Q_DECL_EXPORT jboolean JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList_startsWith__JLjava_lang_Object_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject t0)
{
    jboolean result{false};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QList<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QList<T>::startsWith(const T & t) const", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractListAccess, containerAccess, container.second);

        result = containerAccess->startsWith(__jni_env, container.first, t0);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QList<T>::swapItemsAt(int i, int j)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList_swapItemsAt__JII)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jint i0,
 jint j1)
{
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QList<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QList<T>::swap(int i, int j)", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractListAccess, containerAccess, container.second);

        jint size = containerAccess->size(__jni_env, container.first);
        if (i0 >= size || i0 < 0) {
            JavaException::raiseIndexOutOfBoundsException(__jni_env, QString("Accessing container of size %1 at %2").arg(size).arg(i0) QTJAMBI_STACKTRACEINFO );
        }
        if (j1 >= size || j1 < 0) {
            JavaException::raiseIndexOutOfBoundsException(__jni_env, QString("Accessing container of size %1 at %2").arg(size).arg(j1) QTJAMBI_STACKTRACEINFO );
        }
        containerAccess->swapItemsAt(__jni_env, container.first, i0, j1);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

// QList<T>::value(int i) const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList_value__JI)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jint i0)
{
    jobject result{nullptr};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QList<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QList<T>::value(int i) const", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractListAccess, containerAccess, container.second);

        result = containerAccess->value(__jni_env, container.first, i0);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QList<T>::value(int i, const T & defaultValue) const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList_valueDefault__JILjava_lang_Object_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jint i0,
 jobject defaultValue1)
{
    jobject result{nullptr};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QList<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QList<T>::value(int i, const T & defaultValue) const", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractListAccess, containerAccess, container.second);

        result = containerAccess->value(__jni_env, container.first, i0, defaultValue1);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList_writeTo)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 QtJambiNativeID stream0)
{
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QList<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("operator << (QDataStream&, QList<T>)", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractListAccess, containerAccess, container.second);

        QDataStream* stream = QtJambiAPI::objectFromNativeId<QDataStream>(stream0);
        QtJambiAPI::checkNullPointer(__jni_env, stream);
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
            JavaException::raiseQNoImplementationException(__jni_env, containerName.constData() QTJAMBI_STACKTRACEINFO );
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList_readFrom)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 QtJambiNativeID stream0)
{
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QList<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("operator >> (QDataStream&, QList<T>&)", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractListAccess, containerAccess, container.second);

        QDataStream* stream = QtJambiAPI::objectFromNativeId<QDataStream>(stream0);
        QtJambiAPI::checkNullPointer(__jni_env, stream);
        QByteArray containerName = "QList<";
        containerName += containerAccess->elementMetaType().name();
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

extern "C" Q_DECL_EXPORT jstring JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList_toString)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId)
{
    jstring result{nullptr};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QList<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("operator << (QDebug, QList<T>)", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractListAccess, containerAccess, container.second);

        QString strg;
        {
            QDebug stream(&strg);
            QByteArray containerName = "QList<";
            containerName += containerAccess->elementMetaType().name();
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

extern "C" Q_DECL_EXPORT jint JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList_hashCode)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId)
{
    jint result{0};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QList<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("qHash(QList<T>)", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractListAccess, containerAccess, container.second);

        QByteArray containerName = "QList<";
        containerName += containerAccess->elementMetaType().name();
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

// QList<T>::remove(int i, int count)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList_remove__JII)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jint i0,
 jint count1)
{
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QList<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QList<T>::remove(int i, int count)", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractListAccess, containerAccess, container.second);

        jint size = containerAccess->size(__jni_env, container.first);
        if (i0+count1 > size || i0 < 0) {
            JavaException::raiseIndexOutOfBoundsException(__jni_env, QString("Accessing container of size %1 at %2").arg(size).arg(i0 < 0 ? i0 : i0+count1) QTJAMBI_STACKTRACEINFO );
        }
        containerAccess->remove(__jni_env, container.first, i0, count1);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

// QList<T>::insert(int i, int count, const T & t)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList_insert__JIILjava_lang_Object_2)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jint i0,
 jint count1,
 jobject t2)
{
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QList<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QList<T>::insert(int i, int count, const T & t)", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractListAccess, containerAccess, container.second);

        jint size = containerAccess->size(__jni_env, container.first);
        if (i0 > size || i0 < 0) {
            JavaException::raiseIndexOutOfBoundsException(__jni_env, QString("Accessing container of size %1 at %2").arg(size).arg(i0) QTJAMBI_STACKTRACEINFO );
        }
        containerAccess->insert(__jni_env, container.first, i0, count1, t2);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

// QList<T>::append(const T & t)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList_append__JLjava_lang_Object_2)
    (JNIEnv *__jni_env,
     jclass,
     QtJambiNativeID __this_nativeId,
     jobject t2)
{
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QList<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QList<T>::append(const T & t)", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractListAccess, containerAccess, container.second);

        jint size = containerAccess->size(__jni_env, container.first);
        containerAccess->insert(__jni_env, container.first, size, 1, t2);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
// QList<T>::squeeze()
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList_squeeze__J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId)
{
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QList<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QList<T>::squeeze()", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractListAccess, containerAccess, container.second);

        containerAccess->squeeze(__jni_env, container.first);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

// QList<T>::capacity() const
extern "C" Q_DECL_EXPORT jint JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList_capacity__J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId)
{
    jint result{0};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QList<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QList<T>::capacity() const", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractListAccess, containerAccess, container.second);

        result = containerAccess->capacity(__jni_env, container.first);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QList<T>::fill(const T &value, int size = ...)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList_fill__JLjava_lang_Object_2I)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject value0,
 jint size1)
{
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QList<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QList<T>::fill(const T &value, int size = ...)", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractListAccess, containerAccess, container.second);

        containerAccess->fill(__jni_env, container.first, value0, size1);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

// QList<T>::resize(int size)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QList_resize__JI)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jint size0)
{
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QList<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QList<T>::resize(int size)", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractListAccess, containerAccess, container.second);

        containerAccess->resize(__jni_env, container.first, size0);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

// new QItemSelection(QItemSelection)
#if QT_CONFIG(itemmodel)
void __qt_construct_QItemSelection_3(void* __qtjambi_ptr, JNIEnv* __jni_env, jobject __jni_object, jvalue* __java_arguments)
{
    QTJAMBI_NATIVE_METHOD_CALL("construct QItemSelection(const QModelIndex & topLeft, const QModelIndex & bottomRight)")
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
    QTJAMBI_NATIVE_METHOD_CALL("QItemSelection::QItemSelection(QItemSelection)")
    QTJAMBI_TRY{
#if QT_CONFIG(itemmodel)
        jvalue arguments;
        arguments.l = other;
        QtJambiShell::initialize(__jni_env, __jni_class, __jni_object, &__qt_construct_QItemSelection_3, sizeof(QItemSelection), typeid(QItemSelection), false, QtJambiPrivate::QListAccess<QItemSelectionRange>::newInstance(), &arguments);
#else
        Q_UNUSED(__jni_object)
        Q_UNUSED(__jni_class)
        Q_UNUSED(topLeft0)
        Q_UNUSED(bottomRight1)
        JavaException::raiseQNoImplementationException(__jni_env, "The method has no implementation on this platform." QTJAMBI_STACKTRACEINFO );
#endif // QT_CONFIG(itemmodel)
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}
#endif



