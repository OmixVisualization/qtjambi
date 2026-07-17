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
#include <QtJambi/BufferAPI>
#include <QtJambi/Cast>
#include <QtJambi/ArrayCast>
#include <QtJambi/ContainerCast>
#include <QtJambi/ArithmeticCast>
#include <QtJambi/QList>

// emitting (writeExtraFunctions)
// emitting (writeToStringFunction)
// emitting (writeSignalFunction)
// emitting  (functionsInTargetLang writeFinalFunction)

#if QT_VERSION >= QT_VERSION_CHECK(6, 7, 0)
extern "C" JNIEXPORT jobject JNICALL Java_io_qt_core_QConstSpan_constBegin
    (JNIEnv *env,
     jobject _this,
     QtJambiNativeID __this_nativeId)
{
    jobject result{nullptr};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        Q_ASSERT(container.first);
        if(container.second->isSpan()){
            result = static_cast<AbstractSpanAccess*>(container.second)->constBegin(env, {_this, container.first, __this_nativeId});
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return result;
}

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_core_QConstSpan_constEnd
    (JNIEnv *env,
     jobject _this,
     QtJambiNativeID __this_nativeId)
{
    jobject result{nullptr};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        Q_ASSERT(container.first);
        if(container.second->isSpan()){
            result = static_cast<AbstractSpanAccess*>(container.second)->constEnd(env, {_this, container.first, __this_nativeId});
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
        return result;
}

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_core_QSpan_begin
    (JNIEnv *env,
     jobject _this,
     QtJambiNativeID __this_nativeId)
{
    jobject result{nullptr};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        Q_ASSERT(container.first);
        if(container.second->isSpan()){
            result = static_cast<AbstractSpanAccess*>(container.second)->begin(env, {_this, container.first, __this_nativeId});
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
        return result;
}

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_core_QSpan_end
    (JNIEnv *env,
     jobject _this,
     QtJambiNativeID __this_nativeId)
{
    jobject result{nullptr};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        Q_ASSERT(container.first);
        if(container.second->isSpan()){
            result = static_cast<AbstractSpanAccess*>(container.second)->end(env, {_this, container.first, __this_nativeId});
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
        return result;
}

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_core_QConstSpan_constReverseBegin
    (JNIEnv *env,
     jobject _this,
     QtJambiNativeID __this_nativeId)
{
    jobject result{nullptr};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        Q_ASSERT(container.first);
        if(container.second->isSpan()){
            result = static_cast<AbstractSpanAccess*>(container.second)->constReverseBegin(env, {_this, container.first, __this_nativeId});
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
        return result;
}

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_core_QConstSpan_constReverseEnd
    (JNIEnv *env,
     jobject _this,
     QtJambiNativeID __this_nativeId)
{
    jobject result{nullptr};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        Q_ASSERT(container.first);
        if(container.second->isSpan()){
            result = static_cast<AbstractSpanAccess*>(container.second)->constReverseEnd(env, {_this, container.first, __this_nativeId});
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
        return result;
}

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_core_QSpan_reverseBegin
    (JNIEnv *env,
     jobject _this,
     QtJambiNativeID __this_nativeId)
{
    jobject result{nullptr};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        Q_ASSERT(container.first);
        if(container.second->isSpan()){
            result = static_cast<AbstractSpanAccess*>(container.second)->reverseBegin(env, {_this, container.first, __this_nativeId});
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
        return result;
}

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_core_QSpan_reverseEnd
    (JNIEnv *env,
     jobject _this,
     QtJambiNativeID __this_nativeId)
{
    jobject result{nullptr};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        Q_ASSERT(container.first);
        if(container.second->isSpan()){
            result = static_cast<AbstractSpanAccess*>(container.second)->reverseEnd(env, {_this, container.first, __this_nativeId});
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
        return result;
}

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_core_AbstractSpan_elementType
    (JNIEnv *__jni_env,
     jclass,
     QtJambiNativeID __this_nativeId)
{
    jobject result{nullptr};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        Q_ASSERT(container.first);
        QTJAMBI_CONTAINER_CAST(Span, containerAccess, container.second);
        result = qtjambi_cast<jobject>(__jni_env, containerAccess->elementMetaType());
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
        return result;
}

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_core_AbstractSpan_asBuffer
    (JNIEnv *env,
     jclass,
     jclass bufferClass,
     QtJambiNativeID __this_nativeId,
     QtJambiNativeID ownerId)
{
    jobject result{nullptr};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        Q_ASSERT(container.first);
        if(container.second->isSpan() && static_cast<AbstractSpanAccess*>(container.second)->elementMetaType().isValid()){
            AbstractSpanAccess* containerAccess = static_cast<AbstractSpanAccess*>(container.second);
            QtJambiSpan* span = static_cast<QtJambiSpan*>(container.first);
            qsizetype elementSize = containerAccess->elementMetaType().sizeOf() + (containerAccess->elementMetaType().alignOf() > 0 ? containerAccess->elementMetaType().sizeOf() % containerAccess->elementMetaType().alignOf() : 0);
            qsizetype size_bytes = span->size * elementSize;
            if(containerAccess->isConst()){
                result = DataJBuffer(env, reinterpret_cast<char*>(const_cast<void*>(span->begin)), size_bytes).take();
            }else{
                result = DataJBuffer(env, reinterpret_cast<const char*>(span->begin), size_bytes).take();
            }
        }else{
            ownerId = InvalidNativeID;
            result = DataJBuffer(env, reinterpret_cast<char*>(0), 0).take();
        }
        if(!!ownerId)
            QtJambiAPI::registerDependency(env, result, ownerId);
        if(Java::Runtime::IntBuffer::isSameClass(env, bufferClass)){
            result = Java::Runtime::ByteBuffer::asIntBuffer(env, result);
        }else if(Java::Runtime::ShortBuffer::isSameClass(env, bufferClass)){
            result = Java::Runtime::ByteBuffer::asShortBuffer(env, result);
        }else if(Java::Runtime::LongBuffer::isSameClass(env, bufferClass)){
            result = Java::Runtime::ByteBuffer::asLongBuffer(env, result);
        }else if(Java::Runtime::FloatBuffer::isSameClass(env, bufferClass)){
            result = Java::Runtime::ByteBuffer::asFloatBuffer(env, result);
        }else if(Java::Runtime::DoubleBuffer::isSameClass(env, bufferClass)){
            result = Java::Runtime::ByteBuffer::asDoubleBuffer(env, result);
        }else if(Java::Runtime::CharBuffer::isSameClass(env, bufferClass)){
            result = Java::Runtime::ByteBuffer::asCharBuffer(env, result);
        }
        if(!!ownerId)
            QtJambiAPI::registerDependency(env, result, ownerId);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return result;
}

extern "C" JNIEXPORT jarray JNICALL Java_io_qt_core_AbstractSpan_asArray
    (JNIEnv *env,
     jclass,
     QtJambiNativeID __this_nativeId,
     jchar type)
{
    jarray result{nullptr};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        Q_ASSERT(container.first);
        QtJambiSpan* span = static_cast<QtJambiSpan*>(container.first);
        union{
            const void* pointer;
            char* b;
            qint16* s;
            qint32* i;
            qint64* j;
            QChar* c;
            float* f;
            double* d;
            bool* z;
        }pointer;
        pointer.pointer = span->begin;
        qsizetype size = span->size;
        switch(type){
        case 'B':
            result = qtjambi_cast<jbyteArray>(env, pointer.b, std::move(size));
            break;
        case 'S':
            result = qtjambi_cast<jshortArray>(env, pointer.s, std::move(size));
            break;
        case 'I':
            result = qtjambi_cast<jintArray>(env, pointer.i, std::move(size));
            break;
        case 'J':
            result = qtjambi_cast<jlongArray>(env, pointer.j, std::move(size));
            break;
        case 'C':
            result = qtjambi_cast<jcharArray>(env, pointer.c, std::move(size));
            break;
        case 'F':
            result = qtjambi_cast<jfloatArray>(env, pointer.f, std::move(size));
            break;
        case 'D':
            result = qtjambi_cast<jdoubleArray>(env, pointer.d, std::move(size));
            break;
        case 'Z':
            result = qtjambi_cast<jbooleanArray>(env, pointer.z, std::move(size));
            break;
        default:
            JavaException::raiseIllegalArgumentException(env, "Unable to create QSpan" QTJAMBI_STACKTRACEINFO );
            return nullptr;
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return result;
}

extern "C" JNIEXPORT jint JNICALL Java_io_qt_core_AbstractSpan_size
    (JNIEnv *env,
     jclass,
     QtJambiNativeID __this_nativeId)
{
    jint result{0};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        Q_ASSERT(container.first);
        if(container.second->isSpan()){
            result = jint(static_cast<QtJambiSpan*>(container.first)->size);
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
        return result;
}

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_core_AbstractSpan_get
    (JNIEnv *env,
     jobject,
     QtJambiNativeID __this_nativeId,
     jint index)
{
    jobject result{nullptr};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        Q_ASSERT(container.first);
        if(container.second->isSpan() && static_cast<AbstractSpanAccess*>(container.second)->elementMetaType().isValid()){
            result = static_cast<AbstractSpanAccess*>(container.second)->get(env, container.first, index);
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return result;
}

extern "C" JNIEXPORT jboolean JNICALL Java_io_qt_core_AbstractSpan_set
    (JNIEnv *env,
     jobject _this,
     QtJambiNativeID __this_nativeId,
     jint index,
     jobject value)
{
    jboolean result = false;
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        Q_ASSERT(container.first);
        if(container.second->isSpan() && static_cast<AbstractSpanAccess*>(container.second)->elementMetaType().isValid()){
            result = static_cast<AbstractSpanAccess*>(container.second)->set(env, {_this, container.first}, index, value);
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
        return result;
}

extern "C" JNIEXPORT jboolean JNICALL Java_io_qt_core_AbstractSpan_isConst
    (JNIEnv *env,
     jclass,
     QtJambiNativeID __this_nativeId)
{
    jboolean result = true;
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        Q_ASSERT(container.first);
        result = container.second->isSpan() && static_cast<AbstractSpanAccess*>(container.second)->isConst();
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
        return result;
}

void __qt_construct_QSpan_cref_Iterator(void* __qtjambi_ptr, JNIEnv*, jobject, jvalue* __java_arguments, QtJambiAPI::ConstructorOptions)
{
    QTJAMBI_NATIVE_METHOD_CALL("construct QSpan::QSpan(Iterator)")
    new(__qtjambi_ptr) QtJambiSpan{reinterpret_cast<void*>(__java_arguments[0].j), qsizetype(__java_arguments[1].j)};
}

extern "C" JNIEXPORT void JNICALL Java_io_qt_core_AbstractSpan_initializeFromListBegin
    (JNIEnv *__jni_env,
     jclass,
     jobject __jni_object,
     QtJambiNativeID list,
     QtJambiNativeID begin,
     jint size,
     QtJambiNativeID owner)
{
    QTJAMBI_NATIVE_METHOD_CALL("QSpan::QSpan(Iterator)")
    QTJAMBI_TRY {
        jvalue arguments[2];
        arguments[0].j = 0;
        arguments[1].j = 0;
        if(!!begin){
            if(!!list){
                QPair<void*,AbstractContainerAccess*> listPair = ContainerAPI::fromNativeId(list);
                QtJambiAPI::checkNullPointer(__jni_env, listPair.second, typeid(QList<QVariant>));
                QTJAMBI_CONTAINER_CAST(List, listAccess, listPair.second);
                QPair<void*,AbstractContainerAccess*> beginPair = ContainerAPI::fromNativeId(begin);
                QtJambiAPI::checkNullPointer(__jni_env, beginPair.second, typeid(QList<QVariant>::const_iterator));
                QTJAMBI_CONTAINER_CAST(SequentialConstIterator, iterAccess, beginPair.second);
                std::optional<const void*> value = iterAccess->value(beginPair.first);
                if(value.has_value()){
                    arguments[0].j = jlong(value.value());
                    arguments[1].j = size;
                    QtJambiShell::initialize(__jni_env, __jni_env->GetObjectClass(__jni_object), __jni_object, &__qt_construct_QSpan_cref_Iterator, sizeof(QtJambiSpan), alignof(QtJambiSpan), typeid(QSpan<QVariant>), 0, false, listAccess->createSpanAccess(!iterAccess->isSequentialIterator()), &QtJambiAPI::deletePointer<QtJambiSpan>, arguments, owner);
                    return;
                }
            }else{
                QPair<void*,AbstractContainerAccess*> beginPair = ContainerAPI::fromNativeId(begin);
                QtJambiAPI::checkNullPointer(__jni_env, beginPair.second, typeid(QList<QVariant>::const_iterator));
                QTJAMBI_CONTAINER_CAST(SequentialConstIterator, iterAccess, beginPair.second);
                std::optional<const void*> value = iterAccess->value(beginPair.first);
                if(value.has_value()){
                    arguments[0].j = jlong(value.value());
                    arguments[1].j = size;
                    QtJambiShell::initialize(__jni_env, __jni_env->GetObjectClass(__jni_object), __jni_object, &__qt_construct_QSpan_cref_Iterator, sizeof(QtJambiSpan), alignof(QtJambiSpan), typeid(QSpan<QVariant>), 0, false, iterAccess->createSpanAccess(), &QtJambiAPI::deletePointer<QtJambiSpan>, arguments, owner);
                    return;
                }
            }
        }
        QtJambiShell::initialize(__jni_env, __jni_env->GetObjectClass(__jni_object), __jni_object, &__qt_construct_QSpan_cref_Iterator, sizeof(QtJambiSpan), alignof(QtJambiSpan), typeid(QSpan<QVariant>), 0, false, &QtJambiAPI::deletePointer<QtJambiSpan>, arguments);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

extern "C" JNIEXPORT void JNICALL Java_io_qt_core_AbstractSpan_initializeFromBeginEnd
    (JNIEnv *__jni_env,
     jclass,
     jobject __jni_object,
     QtJambiNativeID begin,
     QtJambiNativeID end,
     QtJambiNativeID owner)
{
    QTJAMBI_NATIVE_METHOD_CALL("QSpan::QSpan(Iterator)")
    QTJAMBI_TRY {
        jvalue arguments[2];
        arguments[0].j = 0;
        arguments[1].j = 0;
        if(begin && end){
            QPair<void*,AbstractContainerAccess*> beginPair = ContainerAPI::fromNativeId(begin);
            QtJambiAPI::checkNullPointer(__jni_env, beginPair.second, typeid(QList<QVariant>::const_iterator));
            QTJAMBI_CONTAINER_CAST(SequentialConstIterator, iterAccess, beginPair.second);
            QPair<void*,AbstractContainerAccess*> endPair = ContainerAPI::fromNativeId(end);
            QtJambiAPI::checkNullPointer(__jni_env, endPair.second, typeid(QList<QVariant>::const_iterator));
            if(iterAccess->isContiguousIterator()){
                std::optional<size_t> size = iterAccess->distance(beginPair.first, endPair.first);
                std::optional<const void*> value = iterAccess->value(beginPair.first);
                if(value.has_value() && size.has_value()){
                    arguments[0].j = jlong(value.value());
                    arguments[1].j = size.value();
                    QtJambiShell::initialize(__jni_env, __jni_env->GetObjectClass(__jni_object), __jni_object, &__qt_construct_QSpan_cref_Iterator, sizeof(QtJambiSpan), alignof(QtJambiSpan), typeid(QSpan<QVariant>), 0, false, iterAccess->createSpanAccess(), &QtJambiAPI::deletePointer<QtJambiSpan>, arguments, owner);
                    return;
                }
            }else{
                JavaException::raiseIllegalArgumentException(__jni_env, "Unable to create QSpan from non-pointer iterator." QTJAMBI_STACKTRACEINFO );
            }
        }
        QtJambiShell::initialize(__jni_env, __jni_env->GetObjectClass(__jni_object), __jni_object, &__qt_construct_QSpan_cref_Iterator, sizeof(QtJambiSpan), alignof(QtJambiSpan), typeid(QSpan<QVariant>), 0, false, &QtJambiAPI::deletePointer<QtJambiSpan>, arguments);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

template<bool isConst>
class BufferArraySpan : public ManagedSpan{
    typedef std::conditional_t<isConst, PersistentJBufferConstData, PersistentJBufferData> BufferAccess;

    struct Data : ManagedSpanData{
        BufferAccess bufferAccess;
        Data(JNIEnv *env, jobject buffer_object)
            : ManagedSpanData{&commit}, bufferAccess(env, buffer_object){}
        static void commit(ManagedSpanData* data,JNIEnv* env){
            static_cast<Data*>(data)->bufferAccess.commit(env);
        }
    };

    BufferArraySpan(JNIEnv *env, jobject buffer_object, jlong valueSize)
        : ManagedSpan(QSharedPointer<Data>(new Data(env, buffer_object)))
    {
        begin = data<Data>()->bufferAccess.data();
        size = data<Data>()->bufferAccess.size()/valueSize;
    }
    static void construct(void* __qtjambi_ptr, JNIEnv* env, jobject, jvalue* __java_arguments, QtJambiAPI::ConstructorOptions)
    {
        QTJAMBI_NATIVE_METHOD_CALL("construct QSpan::QSpan(Buffer)")
        BufferArraySpan<isConst>* access = new(__qtjambi_ptr) BufferArraySpan<isConst>(env, __java_arguments[0].l, __java_arguments[1].j);
        __java_arguments[2].z = access->data<Data>()->bufferAccess.isBuffering();
    }
public:
    static bool initialize(JNIEnv *__jni_env, jobject __jni_object, jobject buffer, QtJambiNativeID owner, AbstractSpanAccess* containerAccess){
        jvalue arguments[3];
        arguments[0].l = buffer;
        auto sz = containerAccess->elementMetaType().sizeOf() + (containerAccess->elementMetaType().alignOf() > 0 ? containerAccess->elementMetaType().sizeOf() % containerAccess->elementMetaType().alignOf() : 0);
        arguments[1].j = sz;
        arguments[2].z = false;
        QtJambiShell::initialize(__jni_env, __jni_env->GetObjectClass(__jni_object),
                                 __jni_object,
                                 &construct, sizeof(ManagedSpan), alignof(ManagedSpan), typeid(QSpan<QVariant>), 0, false,
                                 containerAccess,
                                 &QtJambiAPI::deletePointer<ManagedSpan>, arguments, owner);
        return arguments[2].z;
    }
};

extern "C" JNIEXPORT jboolean JNICALL Java_io_qt_core_AbstractSpan_initializeFromBuffer
    (JNIEnv *__jni_env,
     jclass,
     jobject __jni_object,
     jobject buffer,
     jchar type,
     QtJambiNativeID owner,
     jboolean isConst)
{
    jboolean result = false;
    QTJAMBI_NATIVE_METHOD_CALL("QSpan::QSpan(Buffer)")
    QTJAMBI_TRY {
        AbstractSpanAccess* containerAccess;
        switch(type){
        case 'B':
            if(isConst)
                containerAccess = QSpanAccess<const char>::newInstance();
            else containerAccess = QSpanAccess<char>::newInstance();
            break;
        case 'S':
            if(isConst)
                containerAccess = QSpanAccess<const qint16>::newInstance();
            else containerAccess = QSpanAccess<qint16>::newInstance();
            break;
        case 'I':
            if(isConst)
                containerAccess = QSpanAccess<const qint32>::newInstance();
            else containerAccess = QSpanAccess<qint32>::newInstance();
            break;
        case 'J':
            if(isConst)
                containerAccess = QSpanAccess<const qint64>::newInstance();
            else containerAccess = QSpanAccess<qint64>::newInstance();
            break;
        case 'C':
            if(isConst)
                containerAccess = QSpanAccess<const QChar>::newInstance();
            else containerAccess = QSpanAccess<QChar>::newInstance();
            break;
        case 'F':
            if(isConst)
                containerAccess = QSpanAccess<const float>::newInstance();
            else containerAccess = QSpanAccess<float>::newInstance();
            break;
        case 'D':
            if(isConst)
                containerAccess = QSpanAccess<const double>::newInstance();
            else containerAccess = QSpanAccess<double>::newInstance();
            break;
        default:
            JavaException::raiseIllegalArgumentException(__jni_env, "Unable to create QSpan" QTJAMBI_STACKTRACEINFO );
            return result;
        }
        if(isConst){
            BufferArraySpan<true>::initialize(__jni_env, __jni_object, buffer, owner, containerAccess);
        }else{
            result = BufferArraySpan<false>::initialize(__jni_env, __jni_object, buffer, owner, containerAccess);
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

template<typename NativeType>
class ArraySpan : public ManagedSpan{
    using JArray = QtJambiPrivate::jni_array_type_t<NativeType>;
    using ArrayAccess = QtJambiPrivate::jni_native_to_java_array_converter_t<JArray, true, std::remove_cv_t<NativeType>, std::is_const_v<NativeType>>;

    struct Data : ManagedSpanData{
        ArrayAccess arrayAccess;
        Data(JNIEnv *env, JArray array)
            : ManagedSpanData{&commit}, arrayAccess(env, array){}
        static void commit(ManagedSpanData* data,JNIEnv* env){
            static_cast<Data*>(data)->arrayAccess.commit(env);
        }
    };

    ArraySpan(JNIEnv *env, JArray array)
       : ManagedSpan(QSharedPointer<Data>(new Data(env, array)))
    {
        begin = data<Data>()->arrayAccess.pointer();
        size = data<Data>()->arrayAccess.size();
    }
    static void construct(void* __qtjambi_ptr, JNIEnv* env, jobject, jvalue* __java_arguments, QtJambiAPI::ConstructorOptions)
    {
        QTJAMBI_NATIVE_METHOD_CALL("construct QSpan::QSpan(Array)")
        ArraySpan<NativeType>* access = new(__qtjambi_ptr) ArraySpan<NativeType>(env, JArray(__java_arguments[0].l));
        __java_arguments[1].z = access->data<Data>()->arrayAccess.isBuffering();
    }
public:
    static bool initialize(JNIEnv *__jni_env, jobject __jni_object, JArray array){
        if(!array || __jni_env->GetArrayLength(array)==0){
            jvalue arguments[2];
            arguments[0].j = 0;
            arguments[1].j = 0;
            QtJambiShell::initialize(__jni_env, __jni_env->GetObjectClass(__jni_object),
                                     __jni_object,
                                     &__qt_construct_QSpan_cref_Iterator,
                                     sizeof(QtJambiSpan), alignof(QtJambiSpan), typeid(QSpan<QVariant>), 0, false,
                                     &QtJambiAPI::deletePointer<QtJambiSpan>, arguments);
            return false;
        }else{
            jvalue arguments[2];
            arguments[0].l = array;
            arguments[1].z = false;
            QtJambiShell::initialize(__jni_env, __jni_env->GetObjectClass(__jni_object),
                                     __jni_object,
                                     &construct, sizeof(ManagedSpan), alignof(ManagedSpan), typeid(QSpan<QVariant>), 0, false,
                                     QSpanAccess<NativeType>::newInstance(),
                                     &QtJambiAPI::deletePointer<ManagedSpan>, arguments, InvalidNativeID);
            return arguments[1].z;
        }
    }
};

extern "C" JNIEXPORT jboolean JNICALL Java_io_qt_core_AbstractSpan_initializeFromBooleanArray
    (JNIEnv *__jni_env,
     jclass,
     jobject __jni_object,
     jbooleanArray array,
     jboolean isConst)
{
    jboolean result = false;
    QTJAMBI_NATIVE_METHOD_CALL("QSpan::QSpan(boolean[])")
    QTJAMBI_TRY {
        if(isConst){
            ArraySpan<const bool>::initialize(__jni_env, __jni_object, array);
        }else{
            result = ArraySpan<bool>::initialize(__jni_env, __jni_object, array);
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

extern "C" JNIEXPORT jboolean JNICALL Java_io_qt_core_AbstractSpan_initializeFromByteArray
    (JNIEnv *__jni_env,
     jclass,
     jobject __jni_object,
     jbyteArray array,
     jboolean isConst)
{
    jboolean result = false;
    QTJAMBI_NATIVE_METHOD_CALL("QSpan::QSpan(byte[])")
    QTJAMBI_TRY {
        if(isConst){
            ArraySpan<const char>::initialize(__jni_env, __jni_object, array);
        }else{
            result = ArraySpan<char>::initialize(__jni_env, __jni_object, array);
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

extern "C" JNIEXPORT jboolean JNICALL Java_io_qt_core_AbstractSpan_initializeFromShortArray
    (JNIEnv *__jni_env,
     jclass,
     jobject __jni_object,
     jshortArray array,
     jboolean isConst)
{
    jboolean result = false;
    QTJAMBI_NATIVE_METHOD_CALL("QSpan::QSpan(short[])")
    QTJAMBI_TRY {
        if(isConst){
            ArraySpan<const qint16>::initialize(__jni_env, __jni_object, array);
        }else{
            result = ArraySpan<qint16>::initialize(__jni_env, __jni_object, array);
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

extern "C" JNIEXPORT jboolean JNICALL Java_io_qt_core_AbstractSpan_initializeFromIntArray
    (JNIEnv *__jni_env,
     jclass,
     jobject __jni_object,
     jintArray array,
     jboolean isConst)
{
    jboolean result = false;
    QTJAMBI_NATIVE_METHOD_CALL("QSpan::QSpan(int[])")
    QTJAMBI_TRY {
        if(isConst){
            ArraySpan<const qint32>::initialize(__jni_env, __jni_object, array);
        }else{
            result = ArraySpan<qint32>::initialize(__jni_env, __jni_object, array);
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

extern "C" JNIEXPORT jboolean JNICALL Java_io_qt_core_AbstractSpan_initializeFromLongArray
    (JNIEnv *__jni_env,
     jclass,
     jobject __jni_object,
     jlongArray array,
     jboolean isConst)
{
    jboolean result = false;
    QTJAMBI_NATIVE_METHOD_CALL("QSpan::QSpan(long[])")
    QTJAMBI_TRY {
        if(isConst){
            ArraySpan<const qint64>::initialize(__jni_env, __jni_object, array);
        }else{
            result = ArraySpan<qint64>::initialize(__jni_env, __jni_object, array);
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

extern "C" JNIEXPORT jboolean JNICALL Java_io_qt_core_AbstractSpan_initializeFromFloatArray
    (JNIEnv *__jni_env,
     jclass,
     jobject __jni_object,
     jfloatArray array,
     jboolean isConst)
{
    jboolean result = false;
    QTJAMBI_NATIVE_METHOD_CALL("QSpan::QSpan(float[])")
    QTJAMBI_TRY {
        if(isConst){
            ArraySpan<const float>::initialize(__jni_env, __jni_object, array);
        }else{
            result = ArraySpan<float>::initialize(__jni_env, __jni_object, array);
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

extern "C" JNIEXPORT jboolean JNICALL Java_io_qt_core_AbstractSpan_initializeFromDoubleArray
    (JNIEnv *__jni_env,
     jclass,
     jobject __jni_object,
     jdoubleArray array,
     jboolean isConst)
{
    jboolean result = false;
    QTJAMBI_NATIVE_METHOD_CALL("QSpan::QSpan(double[])")
    QTJAMBI_TRY {
        if(isConst){
            ArraySpan<const double>::initialize(__jni_env, __jni_object, array);
        }else{
            result = ArraySpan<double>::initialize(__jni_env, __jni_object, array);
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

extern "C" JNIEXPORT jboolean JNICALL Java_io_qt_core_AbstractSpan_initializeFromCharArray
    (JNIEnv *__jni_env,
     jclass,
     jobject __jni_object,
     jcharArray array,
     jboolean isConst)
{
    jboolean result = false;
    QTJAMBI_NATIVE_METHOD_CALL("QSpan::QSpan(char[])")
    QTJAMBI_TRY {
        if(isConst){
            ArraySpan<const QChar>::initialize(__jni_env, __jni_object, array);
        }else{
            result = ArraySpan<QChar>::initialize(__jni_env, __jni_object, array);
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

void __qt_construct_QSpan_cref_Clone(void* __qtjambi_ptr, JNIEnv*, jobject, jvalue* __java_arguments, QtJambiAPI::ConstructorOptions)
{
    QTJAMBI_NATIVE_METHOD_CALL("construct QSpan::QSpan(Iterator)")
    QtJambiSpan* newManager = new(__qtjambi_ptr) QtJambiSpan(*reinterpret_cast<QtJambiSpan*>(__java_arguments[0].j));
    if(__java_arguments[1].j){
        char* ptr = reinterpret_cast<char*>(const_cast<void*>(newManager->begin));
        newManager->begin = ptr + __java_arguments[1].j;
    }
    if(__java_arguments[2].j){
        newManager->size -= __java_arguments[2].j;
    }
}

void __qt_construct_QSpan_cref_ManagedClone(void* __qtjambi_ptr, JNIEnv*, jobject, jvalue* __java_arguments, QtJambiAPI::ConstructorOptions)
{
    QTJAMBI_NATIVE_METHOD_CALL("construct QSpan::QSpan(Iterator)")
    ManagedSpan* newManager = new(__qtjambi_ptr) ManagedSpan(*reinterpret_cast<ManagedSpan*>(__java_arguments[0].j));
    if(__java_arguments[1].j){
        char* ptr = reinterpret_cast<char*>(const_cast<void*>(newManager->begin));
        newManager->begin = ptr + __java_arguments[1].j;
    }
    if(__java_arguments[2].j){
        newManager->size -= __java_arguments[2].j;
    }
}

extern "C" JNIEXPORT void JNICALL Java_io_qt_core_AbstractSpan_initializeFromClone
    (JNIEnv *__jni_env,
     jclass,
     jobject __jni_object,
     QtJambiNativeID other,
     QtJambiNativeID owner,
     jboolean isArrayOrNondirectBuffer,
     jint offset,
     jint n)
{
    QTJAMBI_NATIVE_METHOD_CALL("QSpan::QSpan(QSpan)")
    QTJAMBI_TRY {
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(other);
        Q_ASSERT(container.first);
        jlong byte_offset = 0;
        if(offset>0){
            if(container.second->isSpan() && static_cast<AbstractSpanAccess*>(container.second)->elementMetaType().isValid()){
                AbstractSpanAccess* containerAccess = static_cast<AbstractSpanAccess*>(container.second);
                auto sz = containerAccess->elementMetaType().sizeOf() + (containerAccess->elementMetaType().alignOf() > 0 ? containerAccess->elementMetaType().sizeOf() % containerAccess->elementMetaType().alignOf() : 0);
                byte_offset = offset * sz;
            }else{
                offset = 0;
            }
        }
        jvalue arguments[3];
        arguments[0].j = jlong(container.first);
        arguments[1].j = byte_offset;
        arguments[2].j = offset + n;
        if(container.second){
            if(isArrayOrNondirectBuffer){
                QtJambiShell::initialize(__jni_env, __jni_env->GetObjectClass(__jni_object), __jni_object, &__qt_construct_QSpan_cref_ManagedClone, sizeof(ManagedSpan), alignof(ManagedSpan), typeid(QSpan<QVariant>), 0, false, container.second->clone(), &QtJambiAPI::deletePointer<ManagedSpan>, arguments, owner);
            }else{
                QtJambiShell::initialize(__jni_env, __jni_env->GetObjectClass(__jni_object), __jni_object, &__qt_construct_QSpan_cref_Clone, sizeof(QtJambiSpan), alignof(QtJambiSpan), typeid(QSpan<QVariant>), 0, false, container.second->clone(), &QtJambiAPI::deletePointer<QtJambiSpan>, arguments, owner);
            }
        }else{
            if(isArrayOrNondirectBuffer){
                QtJambiShell::initialize(__jni_env, __jni_env->GetObjectClass(__jni_object), __jni_object, &__qt_construct_QSpan_cref_ManagedClone, sizeof(ManagedSpan), alignof(ManagedSpan), typeid(QSpan<QVariant>), 0, false, &QtJambiAPI::deletePointer<ManagedSpan>, arguments);
            }else{
                QtJambiShell::initialize(__jni_env, __jni_env->GetObjectClass(__jni_object), __jni_object, &__qt_construct_QSpan_cref_Clone, sizeof(QtJambiSpan), alignof(QtJambiSpan), typeid(QSpan<QVariant>), 0, false, &QtJambiAPI::deletePointer<QtJambiSpan>, arguments);
            }
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

extern "C" JNIEXPORT void JNICALL Java_io_qt_core_AbstractSpan_commit
    (JNIEnv *env,
     jclass,
     QtJambiNativeID __this_nativeId)
{
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        Q_ASSERT(container.first);
        reinterpret_cast<ManagedSpan*>(container.first)->commit(env);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
}

extern "C" JNIEXPORT jstring JNICALL Java_io_qt_core_AbstractSpan_toString
    (JNIEnv *env,
     jclass,
     QtJambiNativeID __this_nativeId)
{
    jstring result{nullptr};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        Q_ASSERT(container.first);
        if(container.second->isSpan()){
            AbstractSpanAccess* containerAccess = static_cast<AbstractSpanAccess*>(container.second);
            bool isConst = containerAccess->isConst();
            QtJambiSpan* span = reinterpret_cast<QtJambiSpan*>(container.first);
            if(span->size && span->begin)
                result = qtjambi_cast<jstring>(env, QString::asprintf("QSpan<%s%s>(%p,%lld)", isConst ? "const " : "", containerAccess->elementMetaType().name(), span->begin, quint64(span->size)));
            else
                result = qtjambi_cast<jstring>(env, QString::asprintf("QSpan<%s%s>()", isConst ? "const " : "", containerAccess->elementMetaType().name()));
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
        return result;
}

extern "C" JNIEXPORT jint JNICALL Java_io_qt_core_AbstractSpan_hashCode
    (JNIEnv *env,
     jclass,
     QtJambiNativeID __this_nativeId)
{
    jint result{0};
    QTJAMBI_TRY{
        QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
        Q_ASSERT(container.first);
        if(container.second->isSpan()){
            AbstractSpanAccess* containerAccess = static_cast<AbstractSpanAccess*>(container.second);
            bool isConst = containerAccess->isConst();
            QtJambiSpan* span = reinterpret_cast<QtJambiSpan*>(container.first);
            size_t hashValue;
            if(span->size && span->begin)
                hashValue = qHashMulti(0, span->begin, span->size, isConst);
            else
                hashValue = qHashMulti(0, nullptr, 0, isConst);
            result = jint(quint64(hashValue) ^ quint64(hashValue) >> 32);
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return result;
}

extern "C" JNIEXPORT void JNICALL Java_io_qt_core_AbstractSpan_toList
    (JNIEnv *,
     jclass,
     QtJambiNativeID __this_nativeId,
     QtJambiNativeID list_nativeId)
{
    QPair<void*,AbstractContainerAccess*> container = ContainerAPI::fromNativeId(__this_nativeId);
    Q_ASSERT(container.first);

    QPair<void*,AbstractContainerAccess*> list = ContainerAPI::fromNativeId(list_nativeId);
    Q_ASSERT(list.first);

    if(container.second->isSpan() && list.second->isList()){
        AbstractSpanAccess* containerAccess = static_cast<AbstractSpanAccess*>(container.second);
        const QMetaType& valueType = containerAccess->elementMetaType();
        QtJambiSpan* span = reinterpret_cast<QtJambiSpan*>(container.first);
        char* target = &*reinterpret_cast<QList<char>*>(list.first)->begin();
        const char* pointer = reinterpret_cast<const char*>(span->begin);
        auto sz = valueType.sizeOf() + (valueType.alignOf() > 0 ? valueType.sizeOf() % valueType.alignOf() : 0);
        for(qsizetype i = 0; i<span->size; ++i){
            valueType.destruct(target + i*sz);
            valueType.construct(target + i*sz, pointer + i*sz);
        }
    }
}
#endif

// emitting (AbstractMetaClass::NormalFunctions|AbstractMetaClass::AbstractFunctions writeFinalFunction)
// emitting Field accessors (writeFieldAccessors)
// emitting (writeInterfaceCastFunction)
// emitting (writeSignalInitialization)
// emitting (writeJavaLangObjectOverrideFunctions)


