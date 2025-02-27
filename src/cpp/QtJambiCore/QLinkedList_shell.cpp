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

#include <QtCore/qcompilerdetection.h>
QT_WARNING_DISABLE_DEPRECATED
#include <QtCore/QDataStream>
#include <QtCore/QDebug>
#include <QtJambi/QtJambiAPI>
#include <QtJambi/JavaAPI>
#include <QtJambi/ContainerAPI>
#include "utils_p.h"

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include <QtCore/QLinkedList>
#include <QtCore/QVariant>
#include <QtJambi/coreapi.h>
#include <QtJambi/qtjambi_cast.h>

extern "C" JNIEXPORT void JNICALL Java_io_qt_core_QLinkedList_initialize
(JNIEnv * env, jobject _this, jclass elementType, QtJambiNativeID elementMetaType, jobject other)
{
    QTJAMBI_TRY{
        QTJAMBI_NATIVE_METHOD_CALL("QLinkedList<T>::initialize()")
        CoreAPI::initializeQLinkedList(env, _this, elementType, elementMetaType, other);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
}

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_core_QLinkedList_elementMetaType
(JNIEnv * env, jobject, QtJambiNativeID __this_nativeId)
{
    jobject result{nullptr};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(env, container.first, typeid(QLinkedList<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QLinkedList<T>::elementMetaType()", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractLinkedListAccess, containerAccess, container.second);

        result = qtjambi_cast<jobject>(env, containerAccess->elementMetaType());
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return result;
}

// emitting  (functionsInTargetLang writeFinalFunction)
// QLinkedList<T>::append(const T & t)
extern "C" JNIEXPORT void JNICALL Java_io_qt_core_QLinkedList_append__JLjava_lang_Object_2
(JNIEnv *__jni_env,
 jobject _this,
 QtJambiNativeID __this_nativeId,
 jobject t0)
{
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QLinkedList<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QLinkedList<T>::append(const T & t)", container.first)
        dynamic_cast<AbstractLinkedListAccess*>(container.second)->append(__jni_env, ContainerInfo{_this, container.first}, t0);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

// QLinkedList<T>::first() const
extern "C" JNIEXPORT jobject JNICALL Java_io_qt_core_QLinkedList_first__J
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID __this_nativeId)
{
    jobject result{nullptr};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QLinkedList<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QLinkedList<T>::first() const", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractLinkedListAccess, containerAccess, container.second);

        jint size = containerAccess->size(__jni_env, container.first);
        if (size == 0) {
            JavaException::raiseIndexOutOfBoundsException(__jni_env, QString("Accessing container of size 0 at 0") QTJAMBI_STACKTRACEINFO );
        }
        result = containerAccess->first(__jni_env, container.first);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QLinkedList<T>::last() const
extern "C" JNIEXPORT jobject JNICALL Java_io_qt_core_QLinkedList_last__J
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID __this_nativeId)
{
    jobject result{nullptr};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QLinkedList<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QLinkedList<T>::last() const", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractLinkedListAccess, containerAccess, container.second);

        jint size = containerAccess->size(__jni_env, container.first);
        if (size == 0) {
            JavaException::raiseIndexOutOfBoundsException(__jni_env, QString("Accessing container of size 0 at 0") QTJAMBI_STACKTRACEINFO );
        }
        result = containerAccess->last(__jni_env, container.first);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QLinkedList<T>::begin()
extern "C" JNIEXPORT jobject JNICALL Java_io_qt_core_QLinkedList_begin__J
(JNIEnv *__jni_env,
 jobject _this,
 QtJambiNativeID __this_nativeId)
{
    jobject result{nullptr};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QLinkedList<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QLinkedList<T>::begin()", container.first)
        result = dynamic_cast<AbstractLinkedListAccess*>(container.second)->begin(__jni_env, ExtendedContainerInfo{_this, container.first, __this_nativeId});
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QLinkedList<T>::end()
extern "C" JNIEXPORT jobject JNICALL Java_io_qt_core_QLinkedList_end__J
(JNIEnv *__jni_env,
 jobject _this,
 QtJambiNativeID __this_nativeId)
{
    jobject result{nullptr};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QLinkedList<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QLinkedList<T>::end()", container.first)
        result = dynamic_cast<AbstractLinkedListAccess*>(container.second)->end(__jni_env, ExtendedContainerInfo{_this, container.first, __this_nativeId});
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QLinkedList<T>::constBegin() const
extern "C" JNIEXPORT jobject JNICALL Java_io_qt_core_QLinkedList_constBegin__J
(JNIEnv *__jni_env,
 jobject _this,
 QtJambiNativeID __this_nativeId)
{
    jobject result{nullptr};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QLinkedList<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QLinkedList<T>::constBegin() const", container.first)
        result = dynamic_cast<AbstractLinkedListAccess*>(container.second)->constBegin(__jni_env, ConstExtendedContainerInfo{_this, container.first, __this_nativeId});
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QLinkedList<T>::constEnd() const
extern "C" JNIEXPORT jobject JNICALL Java_io_qt_core_QLinkedList_constEnd__J
(JNIEnv *__jni_env,
 jobject _this,
 QtJambiNativeID __this_nativeId)
{
    jobject result{nullptr};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QLinkedList<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QLinkedList<T>::constEnd() const", container.first)
        result = dynamic_cast<AbstractLinkedListAccess*>(container.second)->constEnd(__jni_env, ConstExtendedContainerInfo{_this, container.first, __this_nativeId});
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QLinkedList<T>::clear()
extern "C" JNIEXPORT void JNICALL Java_io_qt_core_QLinkedList_clear__J
(JNIEnv *__jni_env,
 jobject _this,
 QtJambiNativeID __this_nativeId)
{
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QLinkedList<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QLinkedList<T>::clear()", container.first)
        dynamic_cast<AbstractLinkedListAccess*>(container.second)->clear(__jni_env, ContainerInfo{_this, container.first});
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

// QLinkedList<T>::contains(const T & t) const
extern "C" JNIEXPORT jboolean JNICALL Java_io_qt_core_QLinkedList_contains__JLjava_lang_Object_2
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID __this_nativeId,
 jobject t0)
{
    jboolean result{false};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QLinkedList<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QLinkedList<T>::contains(const T & t) const", container.first)
        result = dynamic_cast<AbstractLinkedListAccess*>(container.second)->contains(__jni_env, container.first, t0);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QLinkedList<T>::count(const T & t) const
extern "C" JNIEXPORT jint JNICALL Java_io_qt_core_QLinkedList_count__JLjava_lang_Object_2
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID __this_nativeId,
 jobject t0)
{
    jint result{0};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QLinkedList<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QLinkedList<T>::count(const T & t) const", container.first)
        result = dynamic_cast<AbstractLinkedListAccess*>(container.second)->count(__jni_env, container.first, t0);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QLinkedList<T>::endsWith(const T & t) const
extern "C" JNIEXPORT jboolean JNICALL Java_io_qt_core_QLinkedList_endsWith__JLjava_lang_Object_2
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID __this_nativeId,
 jobject t0)
{
    jboolean result{false};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QLinkedList<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QLinkedList<T>::endsWith(const T & t) const", container.first)
        result = dynamic_cast<AbstractLinkedListAccess*>(container.second)->endsWith(__jni_env, container.first, t0);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QLinkedList<T>::operator==(const QLinkedList & l) const
extern "C" JNIEXPORT jboolean JNICALL Java_io_qt_core_QLinkedList_operator_1equal__JLjava_util_Collection_2
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID __this_nativeId,
 jobject l0)
{
    jboolean result{false};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QLinkedList<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QLinkedList<T>::operator==(const QLinkedList<T> & l) const", container.first)
        result = dynamic_cast<AbstractLinkedListAccess*>(container.second)->equal(__jni_env, container.first, l0);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QLinkedList<T>::prepend(const T & t)
extern "C" JNIEXPORT void JNICALL Java_io_qt_core_QLinkedList_prepend__JLjava_lang_Object_2
(JNIEnv *__jni_env,
 jobject _this,
 QtJambiNativeID __this_nativeId,
 jobject t0)
{
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QLinkedList<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QLinkedList<T>::prepend(const T & t)", container.first)
        dynamic_cast<AbstractLinkedListAccess*>(container.second)->prepend(__jni_env, ContainerInfo{_this, container.first}, t0);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

// QLinkedList<T>::removeAll(const T & t)
extern "C" JNIEXPORT jint JNICALL Java_io_qt_core_QLinkedList_removeAll__JLjava_lang_Object_2
(JNIEnv *__jni_env,
 jobject _this,
 QtJambiNativeID __this_nativeId,
 jobject t0)
{
    jint result{-1};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QLinkedList<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QLinkedList<T>::removeAll(const T & t)", container.first)
        result = dynamic_cast<AbstractLinkedListAccess*>(container.second)->removeAll(__jni_env, ContainerInfo{_this, container.first}, t0);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QLinkedList<T>::removeFirst()
extern "C" JNIEXPORT void JNICALL Java_io_qt_core_QLinkedList_removeFirst__J
(JNIEnv *__jni_env,
 jobject _this,
 QtJambiNativeID __this_nativeId)
{
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QLinkedList<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QLinkedList<T>::removeFirst()", container.first)
        dynamic_cast<AbstractLinkedListAccess*>(container.second)->removeFirst(__jni_env, ContainerInfo{_this, container.first});
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// QLinkedList<T>::removeLast()
extern "C" JNIEXPORT void JNICALL Java_io_qt_core_QLinkedList_removeLast__J
(JNIEnv *__jni_env,
 jobject _this,
 QtJambiNativeID __this_nativeId)
{
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QLinkedList<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QLinkedList<T>::removeLast()", container.first)
        dynamic_cast<AbstractLinkedListAccess*>(container.second)->removeLast(__jni_env, ContainerInfo{_this, container.first});
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

// QLinkedList<T>::removeOne(const T & t)
extern "C" JNIEXPORT jboolean JNICALL Java_io_qt_core_QLinkedList_removeOne__JLjava_lang_Object_2
(JNIEnv *__jni_env,
 jobject _this,
 QtJambiNativeID __this_nativeId,
 jobject t0)
{
    jboolean result{false};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QLinkedList<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QLinkedList<T>::removeOne(const T & t)", container.first)
        result = dynamic_cast<AbstractLinkedListAccess*>(container.second)->removeOne(__jni_env, ContainerInfo{_this, container.first}, t0);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QLinkedList<T>::size() const
extern "C" JNIEXPORT jint JNICALL Java_io_qt_core_QLinkedList_size__J
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID __this_nativeId)
{
    jint result{0};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QLinkedList<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QLinkedList<T>::size() const", container.first)
        result = dynamic_cast<AbstractLinkedListAccess*>(container.second)->size(__jni_env, container.first);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QLinkedList<T>::startsWith(const T & t) const
extern "C" JNIEXPORT jboolean JNICALL Java_io_qt_core_QLinkedList_startsWith__JLjava_lang_Object_2
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID __this_nativeId,
 jobject t0)
{
    jboolean result{false};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QLinkedList<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QLinkedList<T>::startsWith(const T & t) const", container.first)
        result = dynamic_cast<AbstractLinkedListAccess*>(container.second)->startsWith(__jni_env, container.first, t0);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QLinkedList<T>::takeFirst()
extern "C" JNIEXPORT jobject JNICALL Java_io_qt_core_QLinkedList_takeFirst__J
(JNIEnv *__jni_env,
 jobject _this,
 QtJambiNativeID __this_nativeId)
{
    jobject result{nullptr};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QLinkedList<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QLinkedList<T>::takeFirst()", container.first)
        result = dynamic_cast<AbstractLinkedListAccess*>(container.second)->takeFirst(__jni_env, ContainerInfo{_this, container.first});
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QLinkedList<T>::takeLast()
extern "C" JNIEXPORT jobject JNICALL Java_io_qt_core_QLinkedList_takeLast__J
(JNIEnv *__jni_env,
 jobject _this,
 QtJambiNativeID __this_nativeId)
{
    jobject result{nullptr};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QLinkedList<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QLinkedList<T>::takeLast()", container.first)
        result = dynamic_cast<AbstractLinkedListAccess*>(container.second)->takeLast(__jni_env, ContainerInfo{_this, container.first});
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

extern "C" JNIEXPORT void JNICALL Java_io_qt_core_QLinkedList_writeTo
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID __this_nativeId,
 QtJambiNativeID stream0)
{
    QTJAMBI_TRY{
        const QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QDataStream* stream = QtJambiAPI::objectFromNativeId<QDataStream>(stream0);
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("operator << (QDataStream&, QLinkedList<T>)", container.first)
        QtJambiAPI::checkNullPointer(__jni_env, stream);
        QByteArray containerName = "QLinkedList<";
        containerName += dynamic_cast<AbstractLinkedListAccess*>(container.second)->elementMetaType().name();
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

extern "C" JNIEXPORT void JNICALL Java_io_qt_core_QLinkedList_readFrom
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID __this_nativeId,
 QtJambiNativeID stream0)
{
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QLinkedList<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("operator >> (QDataStream&, QLinkedList<T>&)", container.first)
        QDataStream* stream = QtJambiAPI::objectFromNativeId<QDataStream>(stream0);
        QtJambiAPI::checkNullPointer(__jni_env, stream);
        QByteArray containerName = "QLinkedList<";
        containerName += dynamic_cast<AbstractLinkedListAccess*>(container.second)->elementMetaType().name();
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

extern "C" JNIEXPORT jstring JNICALL Java_io_qt_core_QLinkedList_toString
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID __this_nativeId)
{
    jstring result{nullptr};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QLinkedList<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("operator << (QDebug, QLinkedList<T>)", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractLinkedListAccess, containerAccess, container.second);

        QString strg;
        {
            QDebug stream(&strg);
            QByteArray containerName = "QLinkedList<";
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

extern "C" JNIEXPORT jint JNICALL Java_io_qt_core_QLinkedList_hashCode
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID __this_nativeId)
{
    jint result{0};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QLinkedList<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("qHash(QLinkedList<T>)", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractLinkedListAccess, containerAccess, container.second);

        QByteArray containerName = "QLinkedList<";
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

// QLinkedList<T>::operator=(QLinkedList<T>)
extern "C" JNIEXPORT void JNICALL Java_io_qt_core_QLinkedList_assign
    (JNIEnv *__jni_env,
     jobject _this,
     QtJambiNativeID __this_nativeId,
     jobject otherObj,
     QtJambiNativeID other)
{
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container1 = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container1.first, typeid(QLinkedList<QVariant>));
        QPair<void*,AbstractContainerAccess*> container2 = ContainerAPI::fromNativeId(other);
        QtJambiAPI::checkNullPointer(__jni_env, container2.first, typeid(QLinkedList<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QLinkedList<T>::operator=(QLinkedList<T>)", container1.first)
        QTJAMBI_CONTAINER_CAST(AbstractLinkedListAccess, container1Access, container1.second);
        QTJAMBI_CONTAINER_CAST(AbstractLinkedListAccess, container2Access, container2.second);
        if(container1Access->elementMetaType()!=container2Access->elementMetaType())
            JavaException::raiseIllegalArgumentException(__jni_env, QString("Container types mismatch: %1!=%2").arg(container1Access->elementMetaType().name(), container2Access->elementMetaType().name()) QTJAMBI_STACKTRACEINFO );
        container1Access->assign(__jni_env, ContainerInfo{_this, container1.first}, ConstContainerAndAccessInfo(otherObj, container2.first, container2Access));
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

// QLinkedList<T>::swap(QLinkedList<T>&)
extern "C" JNIEXPORT void JNICALL Java_io_qt_core_QLinkedList_swap
    (JNIEnv *__jni_env,
     jobject _this,
     QtJambiNativeID __this_nativeId,
     jobject otherObj,
     QtJambiNativeID other)
{
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container1 = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container1.first, typeid(QLinkedList<QVariant>));
        QPair<void*,AbstractContainerAccess*> container2 = ContainerAPI::fromNativeId(other);
        QtJambiAPI::checkNullPointer(__jni_env, container2.first, typeid(QLinkedList<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QLinkedList<T>::swap(QLinkedList<T>)", container1.first)
        QTJAMBI_CONTAINER_CAST(AbstractLinkedListAccess, container1Access, container1.second);
        QTJAMBI_CONTAINER_CAST(AbstractLinkedListAccess, container2Access, container2.second);
        if(container1Access->elementMetaType()!=container2Access->elementMetaType())
            JavaException::raiseIllegalArgumentException(__jni_env, QString("Container types mismatch: %1!=%2").arg(container1Access->elementMetaType().name(), container2Access->elementMetaType().name()) QTJAMBI_STACKTRACEINFO );
        container1Access->swap(__jni_env, ContainerInfo{_this, container1.first}, ContainerAndAccessInfo(otherObj, container2.first, container2Access));
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

extern "C" JNIEXPORT jboolean JNICALL Java_io_qt_core_QLinkedList_isSharedWith__JJ
    (JNIEnv *__jni_env,
     jobject,
     QtJambiNativeID __this_nativeId,
     QtJambiNativeID other)
{
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container1 = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container1.first, typeid(QLinkedList<QVariant>));
        QPair<void*,AbstractContainerAccess*> container2 = ContainerAPI::fromNativeId(other);
        QtJambiAPI::checkNullPointer(__jni_env, container2.first, typeid(QLinkedList<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QLinkedList<T>::isSharedWith(QLinkedList<T>)", container1.first)
        QTJAMBI_CONTAINER_CAST(AbstractLinkedListAccess, container1Access, container1.second);
        QTJAMBI_CONTAINER_CAST(AbstractLinkedListAccess, container2Access, container2.second);
        if(container1Access && container2Access)
            return reinterpret_cast<QLinkedList<char>*>(container1.first)->isSharedWith(*reinterpret_cast<QLinkedList<char>*>(container2.first));
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
        return false;
}

// emitting (AbstractMetaClass::NormalFunctions|AbstractMetaClass::AbstractFunctions writeFinalFunction)
// emitting Field accessors (writeFieldAccessors)
// emitting (writeInterfaceCastFunction)
// emitting (writeSignalInitialization)
// emitting (writeJavaLangObjectOverrideFunctions)

#endif //QT_VERSION < QT_VERSION_CHECK(6, 0, 0)

