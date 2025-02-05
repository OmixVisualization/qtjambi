/****************************************************************************
**
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#include <QtCore/QDataStream>
#include <QtCore/QDebug>
#include <QtJambi/QtJambiAPI>
#include <QtJambi/JavaAPI>
#include <QtJambi/ContainerAPI>
#include <QtJambi/CoreAPI>
#include "utils_p.h"
#include <QtJambi/qtjambi_cast.h>

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)

extern "C" JNIEXPORT void JNICALL Java_io_qt_core_QVector_initialize
(JNIEnv * env, jobject _this, jclass elementType, QtJambiNativeID elementMetaType, jobject other)
{
    QTJAMBI_TRY{
        QTJAMBI_NATIVE_METHOD_CALL("QVector<T>::initialize()")
        CoreAPI::initializeQVector(env, _this, elementType, elementMetaType, other);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
}

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_core_QVector_elementMetaType
(JNIEnv * env, jobject, QtJambiNativeID __this_nativeId)
{
    jobject result{nullptr};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QTJAMBI_CONTAINER_CAST(AbstractVectorAccess, containerAccess, container.second);

        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QVector<T>::elementMetaType()", container.first)
        result = qtjambi_cast<jobject>(env, containerAccess->elementMetaType());
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return result;
}

// emitting  (functionsInTargetLang writeFinalFunction)

// QVector<T>::append(const QVector<T> & t)
extern "C" JNIEXPORT void JNICALL Java_io_qt_core_QVector_appendVector__JLjava_util_Collection_2
(JNIEnv *__jni_env,
 jobject _this,
 QtJambiNativeID __this_nativeId,
 jobject t0)
{
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QVector<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QVector<T>::append(const QVector<T> & t)", container.first)
        ContainerAndAccessInfo containerInfo(t0);
        dynamic_cast<AbstractVectorAccess*>(container.second)->appendVector(__jni_env, ContainerInfo{_this, container.first}, containerInfo);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

// QVector<T>::append(const T & t)
extern "C" JNIEXPORT void JNICALL Java_io_qt_core_QVector_append__JLjava_lang_Object_2
    (JNIEnv *__jni_env,
     jobject _this,
     QtJambiNativeID __this_nativeId,
     jobject t2)
{
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QVector<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QVector<T>::append(const T & t)", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractVectorAccess, containerAccess, container.second);

        jint size = containerAccess->size(__jni_env, container.first);
        containerAccess->insert(__jni_env, ContainerInfo{_this, container.first}, size, 1, t2);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

// QVector<T>::takeAt(int i)
extern "C" JNIEXPORT jobject JNICALL Java_io_qt_core_QVector_takeAt__JI
    (JNIEnv *__jni_env,
     jobject _this,
     QtJambiNativeID __this_nativeId,
     jint i0)
{
    jobject result = nullptr;
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QVector<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QVector<T>::reserve(int size)", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractVectorAccess, containerAccess, container.second);

        jint size = containerAccess->size(__jni_env, container.first);
        if (i0 >= size || i0 < 0) {
            JavaException::raiseIndexOutOfBoundsException(__jni_env, QString("Accessing container of size %1 at %2").arg(size).arg(i0) QTJAMBI_STACKTRACEINFO );
        }
        result = containerAccess->at(__jni_env, container.first, i0);
        containerAccess->remove(__jni_env, ContainerInfo{_this, container.first}, i0, 1);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
        return result;
}

// QVector<T>::takeLast()
extern "C" JNIEXPORT jobject JNICALL Java_io_qt_core_QVector_takeLast__J
    (JNIEnv *__jni_env,
     jobject _this,
     QtJambiNativeID __this_nativeId)
{
        jobject result = nullptr;
        QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QVector<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QVector<T>::reserve(int size)", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractVectorAccess, containerAccess, container.second);

        jint size = containerAccess->size(__jni_env, container.first);
        if (size == 0) {
            JavaException::raiseIndexOutOfBoundsException(__jni_env, QString("Accessing container of size 0 at 0") QTJAMBI_STACKTRACEINFO );
        }
        result = containerAccess->at(__jni_env, container.first, size-1);
        containerAccess->remove(__jni_env, ContainerInfo{_this, container.first}, size-1, 1);
        }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
        }QTJAMBI_TRY_END
            return result;
}

// QVector<T>::last() const
extern "C" JNIEXPORT jobject JNICALL Java_io_qt_core_QVector_last__J
    (JNIEnv *__jni_env,
     jobject,
     QtJambiNativeID __this_nativeId)
{
    jobject result{nullptr};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QVector<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QVector<T>::last() const", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractVectorAccess, containerAccess, container.second);

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

// QVector<T>::at(int i) const
extern "C" JNIEXPORT jobject JNICALL Java_io_qt_core_QVector_at__JI
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID __this_nativeId,
 jint i0)
{
    jobject result{nullptr};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QVector<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QVector<T>::at(int i) const", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractVectorAccess, containerAccess, container.second);

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

// QVector<T>::begin()
extern "C" JNIEXPORT jobject JNICALL Java_io_qt_core_QVector_begin__J
(JNIEnv *__jni_env,
 jobject _this,
 QtJambiNativeID __this_nativeId)
{
    jobject result{nullptr};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QVector<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QVector<T>::begin()", container.first)
        result = dynamic_cast<AbstractVectorAccess*>(container.second)->begin(__jni_env, ExtendedContainerInfo{_this, container.first, __this_nativeId});
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QVector<T>::end()
extern "C" JNIEXPORT jobject JNICALL Java_io_qt_core_QVector_end__J
(JNIEnv *__jni_env,
 jobject _this,
 QtJambiNativeID __this_nativeId)
{
    jobject result{nullptr};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QVector<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QVector<T>::end()", container.first)
        result = dynamic_cast<AbstractVectorAccess*>(container.second)->end(__jni_env, ExtendedContainerInfo{_this, container.first, __this_nativeId});
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QVector<T>::constBegin() const
extern "C" JNIEXPORT jobject JNICALL Java_io_qt_core_QVector_constBegin__J
(JNIEnv *__jni_env,
 jobject _this,
 QtJambiNativeID __this_nativeId)
{
    jobject result{nullptr};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QVector<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QVector<T>::constBegin() const", container.first)
        result = dynamic_cast<AbstractVectorAccess*>(container.second)->constBegin(__jni_env, ConstExtendedContainerInfo{_this, container.first, __this_nativeId});
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QVector<T>::constEnd() const
extern "C" JNIEXPORT jobject JNICALL Java_io_qt_core_QVector_constEnd__J
(JNIEnv *__jni_env,
 jobject _this,
 QtJambiNativeID __this_nativeId)
{
    jobject result{nullptr};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QVector<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QVector<T>::constEnd() const", container.first)
        result = dynamic_cast<AbstractVectorAccess*>(container.second)->constEnd(__jni_env, ConstExtendedContainerInfo{_this, container.first, __this_nativeId});
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QVector<T>::capacity() const
extern "C" JNIEXPORT jint JNICALL Java_io_qt_core_QVector_capacity__J
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID __this_nativeId)
{
    jint result{0};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QVector<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QVector<T>::capacity() const", container.first)
        result = dynamic_cast<AbstractVectorAccess*>(container.second)->capacity(__jni_env, container.first);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QVector<T>::clear()
extern "C" JNIEXPORT void JNICALL Java_io_qt_core_QVector_clear__J
(JNIEnv *__jni_env,
 jobject _this,
 QtJambiNativeID __this_nativeId)
{
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QVector<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QVector<T>::clear()", container.first)
        dynamic_cast<AbstractVectorAccess*>(container.second)->clear(__jni_env, ContainerInfo{_this, container.first});
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

// QVector<T>::contains(const T & t) const
extern "C" JNIEXPORT jboolean JNICALL Java_io_qt_core_QVector_contains__JLjava_lang_Object_2
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID __this_nativeId,
 jobject t0)
{
    jboolean result{false};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QVector<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QVector<T>::contains(const T & t) const", container.first)
        result = dynamic_cast<AbstractVectorAccess*>(container.second)->contains(__jni_env, container.first, t0);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QVector<T>::count(const T & t) const
extern "C" JNIEXPORT jint JNICALL Java_io_qt_core_QVector_count__JLjava_lang_Object_2
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID __this_nativeId,
 jobject t0)
{
    jint result{0};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QVector<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QVector<T>::count(const T & t) const", container.first)
        result = dynamic_cast<AbstractVectorAccess*>(container.second)->count(__jni_env, container.first, t0);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QVector<T>::endsWith(const T & t) const
extern "C" JNIEXPORT jboolean JNICALL Java_io_qt_core_QVector_endsWith__JLjava_lang_Object_2
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID __this_nativeId,
 jobject t0)
{
    jboolean result{false};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QVector<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QVector<T>::endsWith(const T & t) const", container.first)
        result = dynamic_cast<AbstractVectorAccess*>(container.second)->endsWith(__jni_env, container.first, t0);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QVector<T>::fill(const T &value, int size = ...)
extern "C" JNIEXPORT void JNICALL Java_io_qt_core_QVector_fill__JLjava_lang_Object_2I
(JNIEnv *__jni_env,
 jobject _this,
 QtJambiNativeID __this_nativeId,
 jobject value0,
 jint size1)
{
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QVector<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QVector<T>::fill(const T &value, int size = ...)", container.first)
        dynamic_cast<AbstractVectorAccess*>(container.second)->fill(__jni_env, ContainerInfo{_this, container.first}, value0, size1);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

// QVector<T>::indexOf(const T & t, int from) const
extern "C" JNIEXPORT jint JNICALL Java_io_qt_core_QVector_indexOf__JLjava_lang_Object_2I
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jint from1)
{
    jint result{-1};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QVector<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QVector<T>::indexOf(const T & t, int from) const", container.first)
        result = dynamic_cast<AbstractVectorAccess*>(container.second)->indexOf(__jni_env, container.first, t0, from1);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QVector<T>::insert(int i, int count, const T & t)
extern "C" JNIEXPORT void JNICALL Java_io_qt_core_QVector_insert__JIILjava_lang_Object_2
(JNIEnv *__jni_env,
 jobject _this,
 QtJambiNativeID __this_nativeId,
 jint i0,
 jint count1,
 jobject t2)
{
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QVector<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QVector<T>::insert(int i, int count, const T & t)", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractVectorAccess, containerAccess, container.second);

        jint size = containerAccess->size(__jni_env, container.first);
        if (i0 >= size || i0 < 0) {
            JavaException::raiseIndexOutOfBoundsException(__jni_env, QString("Accessing container of size %1 at %2").arg(size).arg(i0) QTJAMBI_STACKTRACEINFO );
        }
        containerAccess->insert(__jni_env, ContainerInfo{_this, container.first}, i0, count1, t2);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

// QVector<T>::lastIndexOf(const T & t, int from) const
extern "C" JNIEXPORT jint JNICALL Java_io_qt_core_QVector_lastIndexOf__JLjava_lang_Object_2I
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID __this_nativeId,
 jobject t0,
 jint from1)
{
    jint result{-1};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QVector<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QVector<T>::lastIndexOf(const T & t, int from) const", container.first)
        result = dynamic_cast<AbstractVectorAccess*>(container.second)->lastIndexOf(__jni_env, container.first, t0, from1);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QVector<T>::mid(int pos, int length) const
extern "C" JNIEXPORT jobject JNICALL Java_io_qt_core_QVector_mid__JII
(JNIEnv *__jni_env,
 jobject _this,
 QtJambiNativeID __this_nativeId,
 jint pos0,
 jint length1)
{
    jobject result{nullptr};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QVector<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QVector<T>::mid(int pos, int length) const", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractVectorAccess, containerAccess, container.second);

        jint size = containerAccess->size(__jni_env, container.first);
        if (pos0 >= size || pos0 < 0) {
            JavaException::raiseIndexOutOfBoundsException(__jni_env, QString("Accessing container of size %1 at %2").arg(size).arg(pos0) QTJAMBI_STACKTRACEINFO );
        }
        if (length1>=0 && pos0+length1 > size) {
            JavaException::raiseIndexOutOfBoundsException(__jni_env, QString("Accessing container of size %1 at %2 to %3").arg(size).arg(pos0).arg(pos0+length1) QTJAMBI_STACKTRACEINFO );
        }
        result = containerAccess->mid(__jni_env, ConstContainerAndAccessInfo{_this, container.first, containerAccess}, pos0, length1).object;
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QVector<T>::move(int from, int to)
extern "C" JNIEXPORT void JNICALL Java_io_qt_core_QVector_move__JII
(JNIEnv *__jni_env,
 jobject _this,
 QtJambiNativeID __this_nativeId,
 jint from0,
 jint to1)
{
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QVector<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QVector<T>::move(int from, int to)", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractVectorAccess, containerAccess, container.second);

        jint size = containerAccess->size(__jni_env, container.first);
        if (from0 >= size || from0 < 0) {
            JavaException::raiseIndexOutOfBoundsException(__jni_env, QString("Accessing container of size %1 at %2").arg(size).arg(from0) QTJAMBI_STACKTRACEINFO );
        }
        if (to1 >= size && to1 < 0) {
            JavaException::raiseIndexOutOfBoundsException(__jni_env, QString("Accessing container of size %1 at %2").arg(size).arg(to1) QTJAMBI_STACKTRACEINFO );
        }
        containerAccess->move(__jni_env, ContainerInfo{_this, container.first}, from0, to1);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

// QVector<T>::operator==(const QVector & l) const
extern "C" JNIEXPORT jboolean JNICALL Java_io_qt_core_QVector_operator_1equal__JLjava_util_Collection_2
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID __this_nativeId,
 jobject l0)
{
    jboolean result{false};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QVector<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QVector<T>::operator==(const QVector<T> & l) const", container.first)
        result = dynamic_cast<AbstractVectorAccess*>(container.second)->equal(__jni_env, container.first, l0);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QVector<T>::removeAll(const T & t)
extern "C" JNIEXPORT jint JNICALL Java_io_qt_core_QVector_removeAll__JLjava_lang_Object_2
(JNIEnv *__jni_env,
 jobject _this,
 QtJambiNativeID __this_nativeId,
 jobject t0)
{
    jint result{-1};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QVector<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QVector<T>::removeAll(const T & t)", container.first)
        result = dynamic_cast<AbstractVectorAccess*>(container.second)->removeAll(__jni_env, ContainerInfo{_this, container.first}, t0);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QVector<T>::removeOne(const T & t)
extern "C" JNIEXPORT jboolean JNICALL Java_io_qt_core_QVector_removeOne__JLjava_lang_Object_2
    (JNIEnv *__jni_env,
     jobject _this,
     QtJambiNativeID __this_nativeId,
     jobject t1)
{
    jboolean result = false;
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QVector<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QVector<T>::replace(int i, const T & t)", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractVectorAccess, containerAccess, container.second);

        jint i0 = containerAccess->indexOf(__jni_env, container.first, t1, 0);
        if ((result = i0 >= 0))
            containerAccess->remove(__jni_env, ContainerInfo{_this, container.first}, i0, 1);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
        return result;
}

// QVector<T>::remove(int i, int count)
extern "C" JNIEXPORT void JNICALL Java_io_qt_core_QVector_remove__JII
(JNIEnv *__jni_env,
 jobject _this,
 QtJambiNativeID __this_nativeId,
 jint i0,
 jint count1)
{
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QVector<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QVector<T>::remove(int i, int count)", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractVectorAccess, containerAccess, container.second);

        jint size = containerAccess->size(__jni_env, container.first);
        if (i0+count1 > size || i0 < 0) {
            JavaException::raiseIndexOutOfBoundsException(__jni_env, QString("Accessing container of size %1 at %2").arg(size).arg(i0 < 0 ? i0 : i0+count1) QTJAMBI_STACKTRACEINFO );
        }
        containerAccess->remove(__jni_env, ContainerInfo{_this, container.first}, i0, count1);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

// QVector<T>::replace(int i, const T & t)
extern "C" JNIEXPORT void JNICALL Java_io_qt_core_QVector_replace__JILjava_lang_Object_2
(JNIEnv *__jni_env,
 jobject _this,
 QtJambiNativeID __this_nativeId,
 jint i0,
 jobject t1)
{
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QVector<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QVector<T>::replace(int i, const T & t)", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractVectorAccess, containerAccess, container.second);

        jint size = containerAccess->size(__jni_env, container.first);
        if (i0 >= size || i0 < 0) {
            JavaException::raiseIndexOutOfBoundsException(__jni_env, QString("Accessing container of size %1 at %2").arg(size).arg(i0) QTJAMBI_STACKTRACEINFO );
        }
        containerAccess->replace(__jni_env, ContainerInfo{_this, container.first}, i0, t1);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

// QVector<T>::reserve(int size)
extern "C" JNIEXPORT void JNICALL Java_io_qt_core_QVector_reserve__JI
(JNIEnv *__jni_env,
 jobject _this,
 QtJambiNativeID __this_nativeId,
 jint size0)
{
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QVector<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QVector<T>::reserve(int size)", container.first)
        dynamic_cast<AbstractVectorAccess*>(container.second)->reserve(__jni_env, ContainerInfo{_this, container.first}, size0);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

// QVector<T>::resize(int size)
extern "C" JNIEXPORT void JNICALL Java_io_qt_core_QVector_resize__JI
(JNIEnv *__jni_env,
 jobject _this,
 QtJambiNativeID __this_nativeId,
 jint size0)
{
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QVector<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QVector<T>::resize(int size)", container.first)
        dynamic_cast<AbstractVectorAccess*>(container.second)->resize(__jni_env, ContainerInfo{_this, container.first}, size0);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

// QVector<T>::squeeze()
extern "C" JNIEXPORT void JNICALL Java_io_qt_core_QVector_squeeze__J
(JNIEnv *__jni_env,
 jobject _this,
 QtJambiNativeID __this_nativeId)
{
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QVector<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QVector<T>::squeeze()", container.first)
        dynamic_cast<AbstractVectorAccess*>(container.second)->squeeze(__jni_env, ContainerInfo{_this, container.first});
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

// QVector<T>::size() const
extern "C" JNIEXPORT jint JNICALL Java_io_qt_core_QVector_size__J
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID __this_nativeId)
{
    jint result{0};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QVector<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QVector<T>::size() const", container.first)
        result = dynamic_cast<AbstractVectorAccess*>(container.second)->size(__jni_env, container.first);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QVector<T>::startsWith(const T & t) const
extern "C" JNIEXPORT jboolean JNICALL Java_io_qt_core_QVector_startsWith__JLjava_lang_Object_2
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID __this_nativeId,
 jobject t0)
{
    jboolean result{false};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QVector<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QVector<T>::startsWith(const T & t) const", container.first)
        result = dynamic_cast<AbstractVectorAccess*>(container.second)->startsWith(__jni_env, container.first, t0);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QVector<T>::value(int i) const
extern "C" JNIEXPORT jobject JNICALL Java_io_qt_core_QVector_value__JI
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID __this_nativeId,
 jint i0)
{
    jobject result{nullptr};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QVector<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QVector<T>::value(int i) const", container.first)
        result = dynamic_cast<AbstractVectorAccess*>(container.second)->value(__jni_env, container.first, i0);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QVector<T>::value(int i, const T & defaultValue) const
extern "C" JNIEXPORT jobject JNICALL Java_io_qt_core_QVector_valueDefault__JILjava_lang_Object_2
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID __this_nativeId,
 jint i0,
 jobject defaultValue1)
{
    jobject result{nullptr};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QVector<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QVector<T>::value(int i, const T & defaultValue) const", container.first)
        result = dynamic_cast<AbstractVectorAccess*>(container.second)->value(__jni_env, container.first, i0, defaultValue1);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QVector<T>::swapItemsAt(int i, int j)
extern "C" JNIEXPORT void JNICALL Java_io_qt_core_QVector_swapItemsAt__JII
(JNIEnv *__jni_env,
 jobject _this,
 QtJambiNativeID __this_nativeId,
 jint i0,
 jint j1)
{
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QVector<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QVector<T>::swapItemsAt(int i, int j)", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractVectorAccess, containerAccess, container.second);

        jint size = containerAccess->size(__jni_env, container.first);
        if (i0 >= size || i0 < 0) {
            JavaException::raiseIndexOutOfBoundsException(__jni_env, QString("Accessing container of size %1 at %2").arg(size).arg(i0) QTJAMBI_STACKTRACEINFO );
        }
        if (j1 >= size || j1 < 0) {
            JavaException::raiseIndexOutOfBoundsException(__jni_env, QString("Accessing container of size %1 at %2").arg(size).arg(j1) QTJAMBI_STACKTRACEINFO );
        }
        containerAccess->swapItemsAt(__jni_env, ContainerInfo{_this, container.first}, i0, j1);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

extern "C" JNIEXPORT void JNICALL Java_io_qt_core_QVector_writeTo
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID __this_nativeId,
 QtJambiNativeID stream0)
{
    QTJAMBI_TRY{
        const QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QVector<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("operator << (QDataStream&, QVector<T>)", container.first)
        QDataStream* stream = QtJambiAPI::objectFromNativeId<QDataStream>(stream0);
        QtJambiAPI::checkNullPointer(__jni_env, stream);
        QByteArray containerName = "QVector<";
        containerName += dynamic_cast<AbstractVectorAccess*>(container.second)->elementMetaType().name();
        containerName += ">";
        int metaType = container.second->registerContainer(containerName);
        if(!QMetaType::save(*stream, metaType, container.first)){
            containerName.prepend("QDataStream& << ");
            JavaException::raiseQNoImplementationException(__jni_env, containerName.constData() QTJAMBI_STACKTRACEINFO );
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

extern "C" JNIEXPORT void JNICALL Java_io_qt_core_QVector_readFrom
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID __this_nativeId,
 QtJambiNativeID stream0)
{
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QVector<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("operator >> (QDataStream&, QVector<T>&)", container.first)
        QDataStream* stream = QtJambiAPI::objectFromNativeId<QDataStream>(stream0);
        QtJambiAPI::checkNullPointer(__jni_env, stream);
        QByteArray containerName = "QVector<";
        containerName += dynamic_cast<AbstractVectorAccess*>(container.second)->elementMetaType().name();
        containerName += ">";
        int metaType = container.second->registerContainer(containerName);
        if(!QMetaType::load(*stream, metaType, container.first)){
            containerName.prepend("QDataStream& >> ");
            JavaException::raiseQNoImplementationException(__jni_env, containerName.constData() QTJAMBI_STACKTRACEINFO );
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

extern "C" JNIEXPORT jstring JNICALL Java_io_qt_core_QVector_toString
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID __this_nativeId)
{
    jstring result{nullptr};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QVector<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("operator << (QDebug, QVector<T>)", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractVectorAccess, containerAccess, container.second);

        QString strg;
        {
            QDebug stream(&strg);
            QByteArray containerName = "QVector<";
            containerName += containerAccess->elementMetaType().name();
            containerName += ">";
            int metaType = containerAccess->registerContainer(containerName);
            CoreAPI::appendToDebugStream(stream.nospace(), metaType, container.first);
            if(strg.isEmpty()){
                containerName.prepend("QDebug >> ");
                JavaException::raiseQNoImplementationException(__jni_env, containerName.constData() QTJAMBI_STACKTRACEINFO );
            }
        }
        result = qtjambi_cast<jstring>(__jni_env, strg);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

extern "C" JNIEXPORT jint JNICALL Java_io_qt_core_QVector_hashCode
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID __this_nativeId)
{
    jint result{0};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QVector<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("qHash(QVector<T>)", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractVectorAccess, containerAccess, container.second);

        QByteArray containerName = "QVector<";
        containerName += containerAccess->elementMetaType().name();
        containerName += ">";
        QMetaType metaType(containerAccess->registerContainer(containerName));
        hash_type h = CoreAPI::computeHash(metaType, container.first);
        result = jint(h);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QVector<T>::operator=(QVector<T>)
extern "C" JNIEXPORT void JNICALL Java_io_qt_core_QVector_assign
    (JNIEnv *__jni_env,
     jobject _this,
     QtJambiNativeID __this_nativeId,
     jobject otherObj,
     QtJambiNativeID other)
{
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container1 = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container1.first, typeid(QVector<QVariant>));
        QPair<void*,AbstractContainerAccess*> container2 = ContainerAPI::fromNativeId(other);
        QtJambiAPI::checkNullPointer(__jni_env, container2.first, typeid(QVector<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QVector<T>::operator=(QVector<T>)", container1.first)
        QTJAMBI_CONTAINER_CAST(AbstractVectorAccess, container1Access, container1.second);
        QTJAMBI_CONTAINER_CAST(AbstractVectorAccess, container2Access, container2.second);
        if(container1Access->elementMetaType()!=container2Access->elementMetaType())
            JavaException::raiseIllegalArgumentException(__jni_env, QString("Container types mismatch: %1!=%2").arg(container1Access->elementMetaType().name(), container2Access->elementMetaType().name()) QTJAMBI_STACKTRACEINFO );
        container1Access->assign(__jni_env, ContainerInfo{_this, container1.first}, ConstContainerAndAccessInfo(otherObj, container2.first, container2Access));
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

// QVector<T>::swap(QVector<T>&)
extern "C" JNIEXPORT void JNICALL Java_io_qt_core_QVector_swap
    (JNIEnv *__jni_env,
     jobject _this,
     QtJambiNativeID __this_nativeId,
     jobject otherObj,
     QtJambiNativeID other)
{
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container1 = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container1.first, typeid(QVector<QVariant>));
        QPair<void*,AbstractContainerAccess*> container2 = ContainerAPI::fromNativeId(other);
        QtJambiAPI::checkNullPointer(__jni_env, container2.first, typeid(QVector<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QVector<T>::swap(QVector<T>)", container1.first)
        QTJAMBI_CONTAINER_CAST(AbstractVectorAccess, container1Access, container1.second);
        QTJAMBI_CONTAINER_CAST(AbstractVectorAccess, container2Access, container2.second);
        if(container1Access->elementMetaType()!=container2Access->elementMetaType())
            JavaException::raiseIllegalArgumentException(__jni_env, QString("Container types mismatch: %1!=%2").arg(container1Access->elementMetaType().name(), container2Access->elementMetaType().name()) QTJAMBI_STACKTRACEINFO );
        container1Access->swap(__jni_env, ContainerInfo{_this, container1.first}, ContainerAndAccessInfo(otherObj, container2.first, container2Access));
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

extern "C" JNIEXPORT jboolean JNICALL Java_io_qt_core_QVector_isSharedWith__JJ
    (JNIEnv *__jni_env,
     jobject,
     QtJambiNativeID __this_nativeId,
     QtJambiNativeID other)
{
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container1 = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container1.first, typeid(QVector<QVariant>));
        QPair<void*,AbstractContainerAccess*> container2 = ContainerAPI::fromNativeId(other);
        QtJambiAPI::checkNullPointer(__jni_env, container2.first, typeid(QVector<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QVector<T>::isSharedWith(QVector<T>)", container1.first)
        QTJAMBI_CONTAINER_CAST(AbstractVectorAccess, container1Access, container1.second);
        QTJAMBI_CONTAINER_CAST(AbstractVectorAccess, container2Access, container2.second);
        if(container1Access && container2Access)
            return container1Access->isSharedWith(container1.first, container2.first);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return false;
}

extern "C" JNIEXPORT jboolean JNICALL Java_io_qt_core_QVector_isDetached__J
    (JNIEnv *__jni_env,
     jobject,
     QtJambiNativeID __this_nativeId)
{
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QVector<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QVector<T>::isDetached()", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractVectorAccess, containerAccess, container.second);
        if(containerAccess)
            return containerAccess->isDetached(container.first);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return false;
}

extern "C" JNIEXPORT void JNICALL Java_io_qt_core_QVector_detach__J
    (JNIEnv *__jni_env,
     jobject _this,
     QtJambiNativeID __this_nativeId)
{
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QVector<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QVector<T>::detach()", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractVectorAccess, containerAccess, container.second);
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

#endif
