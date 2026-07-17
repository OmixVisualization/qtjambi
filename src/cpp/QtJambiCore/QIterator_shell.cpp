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
#include <QtCore/QByteArrayList>
#include <QtCore/QQueue>
#include <QtCore/QQueue>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

#if QT_VERSION >= QT_VERSION_CHECK(6, 7, 0)
#include <QtCore/QSpan>
#endif

#include <QtJambi/QtJambiAPI>
#include <QtJambi/ContainerAPI>
#include <QtJambi/CoreAPI>
#include <QtJambi/JavaAPI>
#include <QtJambi/Cast>

// emitting (writeExtraFunctions)
// emitting (writeToStringFunction)
// emitting (writeSignalFunction)
// emitting  (functionsInTargetLang writeFinalFunction)

#define QTJAMBI_ITERATOR_CAST(Type, target, source) QTJAMBI_CONTAINER_CAST(Type, target, source)

#define QTJAMBI_SEQUENTIAL_CONST_ITERATOR_CAST(target, source) \
    QTJAMBI_CONTAINER_CAST(SequentialConstIterator, target, source);

#define QTJAMBI_ASSOCIATIVE_CONST_ITERATOR_CAST(target, source) \
    QTJAMBI_CONTAINER_CAST(AssociativeConstIterator, target, source);

extern "C" JNIEXPORT void JNICALL Java_io_qt_core_AbstractIterator_initialize__Lio_qt_core_AbstractIterator_2Lio_qt_core_AbstractIterator_2Z
    (JNIEnv * env, jclass, jobject _this, jobject other, jboolean targetConst)
{
    QTJAMBI_TRY{
        QTJAMBI_NATIVE_METHOD_CALL("AbstractIterator<T>::initialize()")
        CoreAPI::initializeIterator(env, _this, other, targetConst);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
}

// Iterator::operator*() const
extern "C" JNIEXPORT jobject JNICALL Java_io_qt_core_QAssociativeConstIterator_key__J
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId)
{
    jobject result{nullptr};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        Q_ASSERT(container.first);
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("Iterator::key() const", container.first)
        QTJAMBI_ASSOCIATIVE_CONST_ITERATOR_CAST(containerAccess, container.second);
        result = containerAccess->key(__jni_env, container.first);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}
extern "C" JNIEXPORT jobject JNICALL Java_io_qt_core_QAssociativeConstIterator_keyType__J
    (JNIEnv *__jni_env,
     jclass,
     QtJambiNativeID __this_nativeId)
{
    jobject result{nullptr};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        Q_ASSERT(container.first);
        QTJAMBI_ASSOCIATIVE_CONST_ITERATOR_CAST(containerAccess, container.second);
        result = qtjambi_cast<jobject>(__jni_env, containerAccess->keyMetaType());
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
        return result;
}

extern "C" JNIEXPORT jboolean JNICALL Java_io_qt_core_AbstractIterator_setValue__JLjava_lang_Object_2
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject newValue)
{
    jboolean result{false};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        Q_ASSERT(container.first);
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("Iterator::operator*()", container.first)
        if(container.second->isAssociativeIterator()){
            static_cast<AbstractAssociativeIteratorAccess*>(container.second)->setValue(__jni_env, container.first, newValue);
            result = true;
        }else if(container.second->isSequentialIterator()){
            static_cast<AbstractSequentialIteratorAccess*>(container.second)->setValue(__jni_env, container.first, newValue);
            result = true;
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_core_AbstractIterator_valueType__J
    (JNIEnv *__jni_env,
     jclass,
     QtJambiNativeID __this_nativeId)
{
    jobject result{nullptr};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        Q_ASSERT(container.first);
        QTJAMBI_SEQUENTIAL_CONST_ITERATOR_CAST(containerAccess, container.second);
        result = qtjambi_cast<jobject>(__jni_env, containerAccess->valueMetaType());
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
        return result;
}

extern "C" JNIEXPORT jboolean JNICALL Java_io_qt_core_AbstractIterator_isAssociative__J
    (JNIEnv *__jni_env,
     jclass,
     QtJambiNativeID __this_nativeId)
{
    jboolean result{false};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        Q_ASSERT(container.first);
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("Iterator::isEnd() const", container.first)
        QTJAMBI_SEQUENTIAL_CONST_ITERATOR_CAST(containerAccess, container.second);
        result = containerAccess->isAssociativeConstIterator();
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
        return result;
}

extern "C" JNIEXPORT jboolean JNICALL Java_io_qt_core_AbstractIterator_isEnd__J
    (JNIEnv *__jni_env,
     jclass,
     QtJambiNativeID __this_nativeId)
{
    jboolean result{false};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        Q_ASSERT(container.first);
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("Iterator::isEnd() const", container.first)
        QTJAMBI_SEQUENTIAL_CONST_ITERATOR_CAST(containerAccess, container.second);
        result = containerAccess->isEnd(__jni_env, container.first);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
        return result;
}

extern "C" JNIEXPORT jboolean JNICALL Java_io_qt_core_AbstractIterator_isBegin__J
    (JNIEnv *__jni_env,
     jclass,
     QtJambiNativeID __this_nativeId)
{
    jboolean result{false};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        Q_ASSERT(container.first);
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("Iterator::isBegin() const", container.first)
        QTJAMBI_SEQUENTIAL_CONST_ITERATOR_CAST(containerAccess, container.second);
        result = containerAccess->isBegin(__jni_env, container.first);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
        return result;
}

extern "C" JNIEXPORT jboolean JNICALL Java_io_qt_core_AbstractIterator_isValid__J
    (JNIEnv *__jni_env,
     jclass,
     QtJambiNativeID __this_nativeId)
{
    jboolean result{false};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        Q_ASSERT(container.first);
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("Iterator::isValid() const", container.first)
        QTJAMBI_SEQUENTIAL_CONST_ITERATOR_CAST(containerAccess, container.second);
        std::optional<bool> opt = containerAccess->isValid(container.first);
        result = !opt.has_value() || opt.value();
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
        return result;
}

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_core_AbstractIterator_value__J
    (JNIEnv *__jni_env,
     jclass,
     QtJambiNativeID __this_nativeId)
{
    jobject result{nullptr};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        Q_ASSERT(container.first);
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("Iterator::operator*() const", container.first)
        QTJAMBI_SEQUENTIAL_CONST_ITERATOR_CAST(containerAccess, container.second);
        result = containerAccess->value(__jni_env, container.first);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// Iterator::operator++()
extern "C" JNIEXPORT void JNICALL Java_io_qt_core_AbstractIterator_increment__J
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId)
{
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        Q_ASSERT(container.first);
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("Iterator::operator++()", container.first)
        QTJAMBI_SEQUENTIAL_CONST_ITERATOR_CAST(containerAccess, container.second);
        containerAccess->increment(__jni_env, container.first);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

// Iterator::advance()
extern "C" JNIEXPORT void JNICALL Java_io_qt_core_AbstractIterator_advance__JJ
    (JNIEnv *__jni_env,
     jclass,
     QtJambiNativeID __this_nativeId, jlong n)
{
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        Q_ASSERT(container.first);
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("Iterator::advance(n)", container.first)
        QTJAMBI_SEQUENTIAL_CONST_ITERATOR_CAST(containerAccess, container.second);
        containerAccess->advance(__jni_env, container.first, n);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

// Iterator::operator--()
extern "C" JNIEXPORT void JNICALL Java_io_qt_core_AbstractIterator_decrement__J
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId)
{
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        Q_ASSERT(container.first);
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("Iterator::operator--()", container.first)
        QTJAMBI_SEQUENTIAL_CONST_ITERATOR_CAST(containerAccess, container.second);
        containerAccess->decrement(__jni_env, container.first);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

// Iterator::operator--()
extern "C" JNIEXPORT jboolean JNICALL Java_io_qt_core_AbstractIterator_isBidirectionalIterator__J
    (JNIEnv *__jni_env,
     jclass,
     QtJambiNativeID __this_nativeId)
{
    jboolean result = false;
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        Q_UNUSED(container.first)
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("Iterator::isBidirectionalIterator()", container.first)
        QTJAMBI_SEQUENTIAL_CONST_ITERATOR_CAST(containerAccess, container.second);
        result = containerAccess->isBidirectionalIterator();
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

extern "C" JNIEXPORT jboolean JNICALL Java_io_qt_core_AbstractIterator_isContiguous__J
    (JNIEnv *env,
     jclass,
     QtJambiNativeID __this_nativeId)
{
    jboolean result{false};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QTJAMBI_SEQUENTIAL_CONST_ITERATOR_CAST(containerAccess, container.second);
        result = containerAccess->isContiguousIterator();
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
        return result;
}

extern "C" JNIEXPORT jboolean JNICALL Java_io_qt_core_AbstractIterator_canLess__J
    (JNIEnv *env,
     jclass,
     QtJambiNativeID __this_nativeId)
{
    jboolean result{false};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QTJAMBI_SEQUENTIAL_CONST_ITERATOR_CAST(containerAccess, container.second);
        result = containerAccess->canLess();
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
        return result;
}

extern "C" JNIEXPORT jboolean JNICALL Java_io_qt_core_AbstractIterator_canCopy__J
    (JNIEnv *env,
     jclass,
     QtJambiNativeID __this_nativeId)
{
    jboolean result{false};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        QTJAMBI_SEQUENTIAL_CONST_ITERATOR_CAST(containerAccess, container.second);
        result = containerAccess->canCopy();
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
        return result;
}

// Iterator::operator<(const Iterator & other) const
extern "C" JNIEXPORT jboolean JNICALL Java_io_qt_core_AbstractIterator_lessThan__JJ
    (JNIEnv *__jni_env,
     jclass,
     QtJambiNativeID __this_nativeId,
     QtJambiNativeID other0)
{
    jboolean result{false};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        Q_ASSERT(container.first);
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("Iterator::operator<(const Iterator & other) const", container.first)
        QTJAMBI_SEQUENTIAL_CONST_ITERATOR_CAST(containerAccess, container.second);
        QPair<void*,AbstractContainerAccess*> other = ContainerAPI::fromNativeId(other0);
        QtJambiAPI::checkNullPointer(__jni_env, other.first, typeid(QList<QVariant>::const_iterator));
        result = containerAccess->lessThan(__jni_env, container.first, other.first);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
        return result;
}

// Iterator::operator==(const Iterator & o) const
extern "C" JNIEXPORT jboolean JNICALL Java_io_qt_core_AbstractIterator_equals__JJLio_qt_core_AbstractIterator_2
    (JNIEnv *__jni_env,
     jclass,
     QtJambiNativeID __this_nativeId,
     QtJambiNativeID o0, jobject otherObj)
{
    bool result{false};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        Q_ASSERT(container.first);
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("Iterator::operator==(const Iterator & o) const", container.first)
        QTJAMBI_SEQUENTIAL_CONST_ITERATOR_CAST(containerAccess, container.second);
        QPair<void*,AbstractContainerAccess*> other = ContainerAPI::fromNativeId(o0);
        QtJambiAPI::checkNullPointer(__jni_env, other.first, typeid(QList<QVariant>::const_iterator));
        result = containerAccess->equals(__jni_env, container.first, ConstContainerAndAccessInfo{otherObj, other.first, other.second});
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
        return result;
}

extern "C" JNIEXPORT jstring JNICALL Java_io_qt_core_AbstractIterator_toString
    (JNIEnv *env,
     jclass,
     QtJambiNativeID __this_nativeId)
{
    jstring result{nullptr};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        Q_ASSERT(container.first);
        if(container.second->isSequentialConstIterator()){
            AbstractSequentialConstIteratorAccess* containerAccess = static_cast<AbstractSequentialConstIteratorAccess*>(container.second);
            bool isConst = !container.second->isSequentialIterator()
                           && !container.second->isAssociativeIterator();
            bool isAssociative = container.second->isAssociativeConstIterator();
            result = qtjambi_cast<jstring>(env, QString::asprintf("Q%s%sIterator<%s>(%p)",
                                                                  isAssociative ? "Associative" : "Sequential",
                                                                  isConst ? "Const" : "",
                                                                  containerAccess->valueMetaType().name()
                                                                  ,container.first));
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return result;
}

extern "C" JNIEXPORT jint JNICALL Java_io_qt_core_AbstractIterator_hashCode
    (JNIEnv *env,
     jclass,
     QtJambiNativeID __this_nativeId)
{
    jint result{0};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        Q_ASSERT(container.first);
        if(container.second->isSequentialConstIterator()){
            bool isConst = !container.second->isSequentialIterator()
                           && !container.second->isAssociativeIterator();
            bool isAssociative = container.second->isAssociativeConstIterator();
#if QT_VERSION < QT_VERSION_CHECK(6, 10, 0)
            QtPrivate::QHashCombine hash;
#else
            QtPrivate::QHashCombine hash(0);
#endif
            size_t hashValue = 0;
            hashValue = hash(hashValue, container.first);
            hashValue = hash(hashValue, isConst);
            hashValue = hash(hashValue, isAssociative);
            result = jint(quint64(hashValue) ^ quint64(hashValue) >> 32);
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return result;
}

// emitting (AbstractMetaClass::NormalFunctions|AbstractMetaClass::AbstractFunctions writeFinalFunction)
// emitting Field accessors (writeFieldAccessors)
// emitting (writeInterfaceCastFunction)
// emitting (writeSignalInitialization)
// emitting (writeJavaLangObjectOverrideFunctions)


