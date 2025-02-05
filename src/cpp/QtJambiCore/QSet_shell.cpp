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
#include <QtCore/QSet>
#include <QtCore/QVariant>
#include <QtJambi/CoreAPI>
#include "utils_p.h"
#include <QtJambi/qtjambi_cast.h>

// emitting  (functionsInTargetLang writeFinalFunction)

extern "C" JNIEXPORT void JNICALL Java_io_qt_core_QSet_initialize
(JNIEnv * env, jobject _this, jclass elementType, QtJambiNativeID elementMetaType, jobject other)
{
    QTJAMBI_TRY{
        QTJAMBI_NATIVE_METHOD_CALL("QSet<T>::initialize()")
        CoreAPI::initializeQSet(env, _this, elementType, elementMetaType, other);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
}

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_core_QSet_elementMetaType
(JNIEnv * env, jobject, QtJambiNativeID __this_nativeId)
{
    jobject result{nullptr};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(env, container.first, typeid(QSet<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QSet<T>::elementMetaType()", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractSetAccess, containerAccess, container.second);

        result = qtjambi_cast<jobject>(env, containerAccess->elementMetaType());
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return result;
}

// QSet<T>::insert(const T & t)
extern "C" JNIEXPORT void JNICALL Java_io_qt_core_QSet_insert__JLjava_lang_Object_2
(JNIEnv *__jni_env,
 jobject _this,
 QtJambiNativeID __this_nativeId,
 jobject t0)
{
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QSet<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QSet<T>::insert(const T & t)", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractSetAccess, containerAccess, container.second);

        containerAccess->insert(__jni_env, ContainerInfo{_this, container.first}, t0);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

// QSet<T>::constBegin() const
extern "C" JNIEXPORT jobject JNICALL Java_io_qt_core_QSet_constBegin__J
(JNIEnv *__jni_env,
 jobject _this,
 QtJambiNativeID __this_nativeId)
{
    jobject result{nullptr};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QSet<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QSet<T>::constEnd() const", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractSetAccess, containerAccess, container.second);

        result = containerAccess->constBegin(__jni_env, ConstExtendedContainerInfo{_this, container.first, __this_nativeId});
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QSet<T>::capacity() const
extern "C" JNIEXPORT jint JNICALL Java_io_qt_core_QSet_capacity__J
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID __this_nativeId)
{
    jint result{0};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QSet<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QSet<T>::capacity() const", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractSetAccess, containerAccess, container.second);

        result = containerAccess->capacity(__jni_env, container.first);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QSet<T>::clear()
extern "C" JNIEXPORT void JNICALL Java_io_qt_core_QSet_clear__J
(JNIEnv *__jni_env,
 jobject _this,
 QtJambiNativeID __this_nativeId)
{
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QSet<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QSet<T>::clear()", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractSetAccess, containerAccess, container.second);

        containerAccess->clear(__jni_env, ContainerInfo{_this, container.first});
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

// QSet<T>::contains(const T & t) const
extern "C" JNIEXPORT jboolean JNICALL Java_io_qt_core_QSet_contains__JLjava_lang_Object_2
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID __this_nativeId,
 jobject t0)
{
    jboolean result{false};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QSet<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QSet<T>::contains(const T & t) const", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractSetAccess, containerAccess, container.second);

        result = containerAccess->contains(__jni_env, container.first, t0);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QSet<T>::intersect(const QSet<T> &other)
extern "C" JNIEXPORT void JNICALL Java_io_qt_core_QSet_intersect__JLjava_util_Collection_2
(JNIEnv *__jni_env,
 jobject _this,
 QtJambiNativeID __this_nativeId,
 jobject t0)
{
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QSet<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QSet<T>::intersect(const QSet<T> &other)", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractSetAccess, containerAccess, container.second);
        ContainerAndAccessInfo containerInfo(t0);
        containerAccess->intersect(__jni_env, ContainerInfo{_this, container.first}, containerInfo);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

// QSet<T>::unite(const QSet<T> &other)
extern "C" JNIEXPORT void JNICALL Java_io_qt_core_QSet_unite__JLjava_util_Collection_2
(JNIEnv *__jni_env,
 jobject _this,
 QtJambiNativeID __this_nativeId,
 jobject t0)
{
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QSet<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QSet<T>::unite(const QSet<T> &other)", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractSetAccess, containerAccess, container.second);
        ContainerAndAccessInfo containerInfo(t0);
        containerAccess->unite(__jni_env, ContainerInfo{_this, container.first}, containerInfo);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

// QSet<T>::subtract(const QSet<T> &other)
extern "C" JNIEXPORT void JNICALL Java_io_qt_core_QSet_subtract__JLjava_util_Collection_2
(JNIEnv *__jni_env,
 jobject _this,
 QtJambiNativeID __this_nativeId,
 jobject t0)
{
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QSet<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QSet<T>::subtract(const QSet<T> &other)", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractSetAccess, containerAccess, container.second);
        ContainerAndAccessInfo containerInfo(t0);
        containerAccess->subtract(__jni_env, ContainerInfo{_this, container.first}, containerInfo);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

// QSet<T>::intersects(const QSet<T> &other) const
extern "C" JNIEXPORT jboolean JNICALL Java_io_qt_core_QSet_intersects__JLjava_util_Collection_2
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID __this_nativeId,
 jobject t0)
{
    jboolean result{false};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QSet<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QSet<T>::intersects(const QSet<T> &other) const", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractSetAccess, containerAccess, container.second);

        result = containerAccess->intersects(__jni_env, container.first, t0);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QSet<T>::constEnd() const
extern "C" JNIEXPORT jobject JNICALL Java_io_qt_core_QSet_constEnd__J
(JNIEnv *__jni_env,
 jobject _this,
 QtJambiNativeID __this_nativeId)
{
    jobject result{nullptr};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QSet<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QSet<T>::constEnd() const", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractSetAccess, containerAccess, container.second);

        result = containerAccess->constEnd(__jni_env, ConstExtendedContainerInfo{_this, container.first, __this_nativeId});
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QSet<T>::operator==(const QSet & l) const
extern "C" JNIEXPORT jboolean JNICALL Java_io_qt_core_QSet_operator_1equal__JLjava_util_Collection_2
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID __this_nativeId,
 jobject l0)
{
    jboolean result{false};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QSet<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QSet<T>::operator==(const QSet<T> & l) const", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractSetAccess, containerAccess, container.second);

        result = containerAccess->equal(__jni_env, container.first, l0);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QSet<T>::remove(const T & t)
extern "C" JNIEXPORT jboolean JNICALL Java_io_qt_core_QSet_remove__JLjava_lang_Object_2
(JNIEnv *__jni_env,
 jobject _this,
 QtJambiNativeID __this_nativeId,
 jobject t0)
{
    jboolean result{false};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QSet<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QSet<T>::remove(const T & t)", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractSetAccess, containerAccess, container.second);

        result = containerAccess->remove(__jni_env, ContainerInfo{_this, container.first}, t0);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QSet<T>::reserve(int size)
extern "C" JNIEXPORT void JNICALL Java_io_qt_core_QSet_reserve__JI
(JNIEnv *__jni_env,
 jobject _this,
 QtJambiNativeID __this_nativeId,
 jint size0)
{
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QSet<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QSet<T>::reserve(int size)", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractSetAccess, containerAccess, container.second);

        containerAccess->reserve(__jni_env, ContainerInfo{_this, container.first}, size0);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

// QSet<T>::size() const
extern "C" JNIEXPORT jint JNICALL Java_io_qt_core_QSet_size__J
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID __this_nativeId)
{
    jint result{0};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QSet<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QSet<T>::size() const", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractSetAccess, containerAccess, container.second);

        result = containerAccess->size(__jni_env, container.first);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QSet<T>::values() const
extern "C" JNIEXPORT jobject JNICALL Java_io_qt_core_QSet_values__J
(JNIEnv *__jni_env,
 jobject _this,
 QtJambiNativeID __this_nativeId)
{
    jobject result{nullptr};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QSet<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QSet<T>::values() const", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractSetAccess, containerAccess, container.second);

        result = containerAccess->values(__jni_env, ConstContainerInfo{_this, container.first}).object;
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}



extern "C" JNIEXPORT void JNICALL Java_io_qt_core_QSet_writeTo
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID __this_nativeId,
 QtJambiNativeID stream0)
{
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QSet<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("operator << (QDataStream&, QSet<T>)", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractSetAccess, containerAccess, container.second);

        QDataStream* stream = QtJambiAPI::objectFromNativeId<QDataStream>(stream0);
        QtJambiAPI::checkNullPointer(__jni_env, stream);
        QByteArray containerName = "QSet<";
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

extern "C" JNIEXPORT void JNICALL Java_io_qt_core_QSet_readFrom
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID __this_nativeId,
 QtJambiNativeID stream0)
{
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QSet<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("operator >> (QDataStream&, QSet<T>&)", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractSetAccess, containerAccess, container.second);

        QDataStream* stream = QtJambiAPI::objectFromNativeId<QDataStream>(stream0);
        QtJambiAPI::checkNullPointer(__jni_env, stream);
        QByteArray containerName = "QSet<";
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

extern "C" JNIEXPORT jstring JNICALL Java_io_qt_core_QSet_toString
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID __this_nativeId)
{
    jstring result = nullptr;
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QSet<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("operator << (QDebug, QSet<T>)", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractSetAccess, containerAccess, container.second);

        QString strg;
        {
            QDebug stream(&strg);
            QByteArray containerName = "QSet<";
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

extern "C" JNIEXPORT jint JNICALL Java_io_qt_core_QSet_hashCode
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID __this_nativeId)
{
    jint result{0};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QSet<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("qHash(QSet<T>)", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractSetAccess, containerAccess, container.second);

        QByteArray containerName = "QSet<";
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

// QSet<T>::operator=(QSet<T>)
extern "C" JNIEXPORT void JNICALL Java_io_qt_core_QSet_assign
    (JNIEnv *__jni_env,
     jobject _this,
     QtJambiNativeID __this_nativeId,
     jobject otherObj,
     QtJambiNativeID other)
{
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container1 = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container1.first, typeid(QSet<QVariant>));
        QPair<void*,AbstractContainerAccess*> container2 = ContainerAPI::fromNativeId(other);
        QtJambiAPI::checkNullPointer(__jni_env, container2.first, typeid(QSet<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QSet<T>::operator=(QSet<T>)", container1.first)
        QTJAMBI_CONTAINER_CAST(AbstractSetAccess, container1Access, container1.second);
        QTJAMBI_CONTAINER_CAST(AbstractSetAccess, container2Access, container2.second);
        if(container1Access->elementMetaType()!=container2Access->elementMetaType())
            JavaException::raiseIllegalArgumentException(__jni_env, QString("Container types mismatch: %1!=%2").arg(container1Access->elementMetaType().name(), container2Access->elementMetaType().name()) QTJAMBI_STACKTRACEINFO );
        container1Access->assign(__jni_env, ContainerInfo{_this, container1.first}, ConstContainerAndAccessInfo(otherObj, container2.first, container2Access));
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

// QSet<T>::swap(QSet<T>&)
extern "C" JNIEXPORT void JNICALL Java_io_qt_core_QSet_swap
    (JNIEnv *__jni_env,
     jobject _this,
     QtJambiNativeID __this_nativeId,
     jobject otherObj,
     QtJambiNativeID other)
{
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container1 = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container1.first, typeid(QSet<QVariant>));
        QPair<void*,AbstractContainerAccess*> container2 = ContainerAPI::fromNativeId(other);
        QtJambiAPI::checkNullPointer(__jni_env, container2.first, typeid(QSet<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QSet<T>::swap(QSet<T>)", container1.first)
        QTJAMBI_CONTAINER_CAST(AbstractSetAccess, container1Access, container1.second);
        QTJAMBI_CONTAINER_CAST(AbstractSetAccess, container2Access, container2.second);
        if(container1Access->elementMetaType()!=container2Access->elementMetaType())
            JavaException::raiseIllegalArgumentException(__jni_env, QString("Container types mismatch: %1!=%2").arg(container1Access->elementMetaType().name(), container2Access->elementMetaType().name()) QTJAMBI_STACKTRACEINFO );
        container1Access->swap(__jni_env, ContainerInfo{_this, container1.first}, ContainerAndAccessInfo(otherObj, container2.first, container2Access));
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

extern "C" JNIEXPORT jboolean JNICALL Java_io_qt_core_QSet_isSharedWith__JJ
    (JNIEnv *__jni_env,
     jobject,
     QtJambiNativeID __this_nativeId,
     QtJambiNativeID other)
{
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container1 = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container1.first, typeid(QSet<QVariant>));
        QPair<void*,AbstractContainerAccess*> container2 = ContainerAPI::fromNativeId(other);
        QtJambiAPI::checkNullPointer(__jni_env, container2.first, typeid(QSet<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QSet<T>::isSharedWith(QSet<T>)", container1.first)
        QTJAMBI_CONTAINER_CAST(AbstractSetAccess, container1Access, container1.second);
        QTJAMBI_CONTAINER_CAST(AbstractSetAccess, container2Access, container2.second);
        if(container1Access && container2Access)
            return container1Access->isSharedWith(container1.first, container2.first);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
        return false;
}

extern "C" JNIEXPORT jboolean JNICALL Java_io_qt_core_QSet_isDetached__J
    (JNIEnv *__jni_env,
     jobject,
     QtJambiNativeID __this_nativeId)
{
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QSet<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QSet<T>::isDetached()", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractSetAccess, containerAccess, container.second);
        if(containerAccess)
            return containerAccess->isDetached(container.first);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
        return false;
}

extern "C" JNIEXPORT void JNICALL Java_io_qt_core_QSet_detach__J
    (JNIEnv *__jni_env,
     jobject _this,
     QtJambiNativeID __this_nativeId)
{
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, container.first, typeid(QSet<QVariant>));
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QSet<T>::detach()", container.first)
        QTJAMBI_CONTAINER_CAST(AbstractSetAccess, containerAccess, container.second);
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


