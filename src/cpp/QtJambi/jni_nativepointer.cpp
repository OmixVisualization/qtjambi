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

#include "qtjambiapi.h"
#include <QtCore/QtCore>
#include "java_p.h"
#include "typemanager_p.h"
#include "qtjambilink_p.h"
#include "jobjectwrapper.h"
#include "coreapi.h"
#include "utils_p.h"

#include <stdlib.h>
#include <unordered_set>
#include <unordered_map>
#include <set>
#include "qtjambi_cast.h"

size_t getValueSizeForClass(JNIEnv *env, jclass object_class)
{
    if(Java::QtJambi::QFlags::isAssignableFrom(env, object_class)){
        enum E{X};
        return sizeof(QFlags<E>);
    }
    if(Java::QtJambi::QtEnumerator::isAssignableFrom(env, object_class)){
        return sizeof(qint32);
    }
    if(Java::QtJambi::QtShortEnumerator::isAssignableFrom(env, object_class)){
        return sizeof(qint16);
    }
    if(Java::QtJambi::QtByteEnumerator::isAssignableFrom(env, object_class)){
        return sizeof(qint8);
    }
    if(Java::QtJambi::QtLongEnumerator::isAssignableFrom(env, object_class)){
        return sizeof(qint64);
    }
    if(Java::Runtime::Enum::isAssignableFrom(env, object_class)){
        return sizeof(qint32);
    }

    size_t result = 0;
    if(jclass cls = JavaAPI::resolveClosestQtSuperclass(env, object_class, nullptr)){
        QString javaClassName = QtJambiAPI::getClassName(env, cls).replace(".", "/");
        if(const std::type_info* qtClassName = getTypeByJavaName(javaClassName)){
            result = getValueSize(*qtClassName);
        }
    }
    return result;
}

size_t getShellSizeForClass(JNIEnv *env, jclass object_class)
{
    size_t result = 0;
    if(jclass cls = JavaAPI::resolveClosestQtSuperclass(env, object_class, nullptr)){
        QString javaClassName = QtJambiAPI::getClassName(env, cls).replace(".", "/");
        if(const std::type_info* qtClassName = getTypeByJavaName(javaClassName)){
            result = getShellSize(*qtClassName);
        }
    }
    return result;
}

extern "C" Q_DECL_EXPORT jboolean JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QNativePointer_readBoolean)
  (JNIEnv *, jclass, jlong ptr, jlong pos)
{
    return (reinterpret_cast<bool *>(ptr))[pos];
}

extern "C" Q_DECL_EXPORT jbyte JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QNativePointer_readByte)
  (JNIEnv *, jclass, jlong ptr, jlong pos)
{
    return (reinterpret_cast<qint8 *>(ptr))[pos];
}

extern "C" Q_DECL_EXPORT jchar JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QNativePointer_readChar)
  (JNIEnv *, jclass, jlong ptr, jlong pos)
{
    return (reinterpret_cast<quint16 *>(ptr))[pos];
}

extern "C" Q_DECL_EXPORT jshort JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QNativePointer_readShort)
  (JNIEnv *, jclass, jlong ptr, jlong pos)
{
    return (reinterpret_cast<qint16 *>(ptr))[pos];
}

extern "C" Q_DECL_EXPORT jint JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QNativePointer_readInt)
  (JNIEnv *, jclass, jlong ptr, jlong pos)
{
    return (reinterpret_cast<qint32 *>(ptr))[pos];
}

extern "C" Q_DECL_EXPORT jlong JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QNativePointer_readLong)
  (JNIEnv *, jclass, jlong ptr, jlong pos)
{
    return (reinterpret_cast<qint64 *>(ptr))[pos];
}

extern "C" Q_DECL_EXPORT jfloat JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QNativePointer_readFloat)
  (JNIEnv *, jclass, jlong ptr, jlong pos)
{
    return (reinterpret_cast<float *>(ptr))[pos];
}

extern "C" Q_DECL_EXPORT jdouble JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QNativePointer_readDouble)
  (JNIEnv *, jclass, jlong ptr, jlong pos)
{
    return (reinterpret_cast<double *>(ptr))[pos];
}

extern "C" Q_DECL_EXPORT jlong JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QNativePointer_readPointer)
  (JNIEnv *, jclass, jlong ptr, jlong pos)
{
    return reinterpret_cast<jlong>((reinterpret_cast<void **>(ptr))[pos]);
}

bool isValueType(JNIEnv * __jni_env, jclass valueType, int* metaTypeId = nullptr);

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QNativePointer_readObject)
  (JNIEnv * __jni_env, jclass, jlong ptr, jclass valueType, jlong pos, jboolean readOnly, jint type, jlong size)
{
    try{
        if(QNativePointer::Type(type)==QNativePointer::Type::Pointer && size==-1){
            if (QSharedPointer<QtJambiLink> link = QtJambiLink::findLinksForPointer(&reinterpret_cast<void **>(ptr)[pos]).value(0)){
                return link->getJavaObjectLocalRef(__jni_env);
            }
            return QtJambiAPI::convertNativeToJavaObjectAsWrapper(__jni_env, &reinterpret_cast<void **>(ptr)[pos], valueType);
        }else if(size>0){
            void* _ptr = reinterpret_cast<char*>(ptr) + size_t(pos) * size;
            int metaTypeId;
            bool isValue = isValueType(__jni_env, valueType, &metaTypeId);
            if(readOnly && !isValue){
                JavaException::raiseIllegalArgumentException(__jni_env, "Cannot read non-value typed object from read-only buffer." QTJAMBI_STACKTRACEINFO);
            }
            if(isValue){
                return QtJambiAPI::convertNativeToJavaObjectAsCopy(__jni_env, &_ptr, valueType);
            }else{
                return QtJambiAPI::convertNativeToJavaObjectAsWrapper(__jni_env, &_ptr, valueType);
            }
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QNativePointer_fromArray)
  (JNIEnv * env, jclass, jclass valueType, jobjectArray array)
{
    try{
        jsize len = array ? env->GetArrayLength(array) : 0;
        if (len == 0)
            return nullptr;
        int metaTypeId;
        size_t size = getValueSizeForClass(env, valueType);
        bool isValue = isValueType(env, valueType, &metaTypeId);
        if(size<=0 || !isValue || metaTypeId==QMetaType::UnknownType){
            JavaException::raiseIllegalArgumentException(env, "Array does not contain clonable values." QTJAMBI_STACKTRACEINFO );
        }

        jobject nativePointer = Java::QtJambi::QNativePointer::newInstance(env,
                                               jint(QNativePointer::Type::Byte), size * size_t(len), 1, false);
        char *buf = reinterpret_cast<char *>(QtJambiAPI::convertQNativePointerToNative(env, nativePointer, 1));
        for (jsize i=0; i<len; ++i) {
            jobject java_object = env->GetObjectArrayElement(array, i);

            void *ptr = nullptr;
            if (java_object) {
                if (QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(env, java_object))
                    ptr = link->pointer();
                else if(Java::QtJambi::QtObjectInterface::isInstanceOf(env, java_object))
                    Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassName(env, java_object).replace("$", ".")) QTJAMBI_STACKTRACEINFO );

            }

            if (ptr){
                QMetaType::construct(metaTypeId, buf + size_t(i) * size, ptr);
            }
        }

        return nativePointer;
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QNativePointer_copyFromB)
  (JNIEnv * env, jclass, jlong ptr, jbyteArray array)
{
    if(jsize size = env->GetArrayLength(array))
        env->GetByteArrayRegion(array, 0, size, reinterpret_cast<jbyte*>(ptr));
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QNativePointer_copyFromS)
  (JNIEnv * env, jclass, jlong ptr, jshortArray array)
{
    if(jsize size = env->GetArrayLength(array))
        env->GetShortArrayRegion(array, 0, size, reinterpret_cast<jshort*>(ptr));
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QNativePointer_copyFromI)
  (JNIEnv * env, jclass, jlong ptr, jintArray array)
{
    if(jsize size = env->GetArrayLength(array))
        env->GetIntArrayRegion(array, 0, size, reinterpret_cast<jint*>(ptr));
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QNativePointer_copyFromL)
  (JNIEnv * env, jclass, jlong ptr, jlongArray array)
{
    if(jsize size = env->GetArrayLength(array))
        env->GetLongArrayRegion(array, 0, size, reinterpret_cast<jlong*>(ptr));
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QNativePointer_copyFromF)
  (JNIEnv * env, jclass, jlong ptr, jfloatArray array)
{
    if(jsize size = env->GetArrayLength(array))
        env->GetFloatArrayRegion(array, 0, size, reinterpret_cast<jfloat*>(ptr));
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QNativePointer_copyFromD)
  (JNIEnv * env, jclass, jlong ptr, jdoubleArray array)
{
    if(jsize size = env->GetArrayLength(array))
        env->GetDoubleArrayRegion(array, 0, size, reinterpret_cast<jdouble*>(ptr));
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QNativePointer_copyBackB)
  (JNIEnv * env, jclass, jlong ptr, jbyteArray array)
{
    if(jsize size = env->GetArrayLength(array))
        env->SetByteArrayRegion(array, 0, size, reinterpret_cast<const jbyte*>(ptr));
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QNativePointer_copyBackS)
  (JNIEnv * env, jclass, jlong ptr, jshortArray array)
{
    if(jsize size = env->GetArrayLength(array))
        env->SetShortArrayRegion(array, 0, size, reinterpret_cast<const jshort*>(ptr));
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QNativePointer_copyBackI)
  (JNIEnv * env, jclass, jlong ptr, jintArray array)
{
    if(jsize size = env->GetArrayLength(array))
        env->SetIntArrayRegion(array, 0, size, reinterpret_cast<const jint*>(ptr));
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QNativePointer_copyBackL)
  (JNIEnv * env, jclass, jlong ptr, jlongArray array)
{
    if(jsize size = env->GetArrayLength(array))
        env->SetLongArrayRegion(array, 0, size, reinterpret_cast<const jlong*>(ptr));
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QNativePointer_copyBackF)
  (JNIEnv * env, jclass, jlong ptr, jfloatArray array)
{
    if(jsize size = env->GetArrayLength(array))
        env->SetFloatArrayRegion(array, 0, size, reinterpret_cast<const jfloat*>(ptr));
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QNativePointer_copyBackD)
  (JNIEnv * env, jclass, jlong ptr, jdoubleArray array)
{
    if(jsize size = env->GetArrayLength(array))
        env->SetDoubleArrayRegion(array, 0, size, reinterpret_cast<const jdouble*>(ptr));
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QNativePointer_fromObject)
  (JNIEnv * __jni_env, jclass, jobject object)
{
    try{
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(__jni_env, object))
            return QtJambiAPI::convertNativeToQNativePointer(__jni_env, link->pointer(), QNativePointer::Type::Pointer, 1);
        else if(Java::QtJambi::QtObjectInterface::isInstanceOf(__jni_env, object))
            Java::QtJambi::QNoNativeResourcesException::throwNew(__jni_env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassName(__jni_env, object).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QNativePointer_toByteBuffer)
  (JNIEnv * env, jclass, jlong ptr, jlong capacity, jboolean readOnly)
{
	jobject buffer{nullptr};
    try{
        if(readOnly){
            buffer = DataJBuffer(env, reinterpret_cast<const void*>(ptr), capacity).take();
        }else{
            buffer = DataJBuffer(env, reinterpret_cast<void*>(ptr), capacity).take();
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return buffer;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QNativePointer_fromBuffer)
  (JNIEnv * env, jclass, jobject buffer)
{
    try{
        if(Java::Runtime::Buffer::isReadOnly(env, buffer)){
            JBufferConstData* d = new JBufferConstData(env, buffer);
            if(d->isDirect()){
                jlong ptr = jlong(d->take());
                delete d;
                return Java::QtJambi::QNativePointer::newInstance2(env, buffer, ptr, jlong(0));
            }else{
                return Java::QtJambi::QNativePointer::newInstance2(env, buffer, jlong(d->data()), jlong(d));
            }
        }else{
            JBufferData* d = new JBufferData(env, buffer);
            if(d->isDirect()){
                jlong ptr = jlong(d->take());
                delete d;
                return Java::QtJambi::QNativePointer::newInstance2(env, buffer, ptr, jlong(0));
            }else{
                return Java::QtJambi::QNativePointer::newInstance2(env, buffer, jlong(d->data()), jlong(d));
            }
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QNativePointer_deleteBufferAccess)
    (JNIEnv *, jclass, jlong ptr, jboolean isReadonly)
{
    if(isReadonly){
        delete reinterpret_cast<JBufferConstData*>(ptr);
    }else{
        delete reinterpret_cast<JBufferData*>(ptr);
    }
}

extern "C" Q_DECL_EXPORT jstring JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QNativePointer_readString)
  (JNIEnv *env, jclass, jlong ptr, jlong pos)
{
    try{
        return qtjambi_cast<jstring>(env, reinterpret_cast<QString *>(ptr)[pos]);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QNativePointer_writeBoolean)
  (JNIEnv *, jclass, jlong ptr, jlong pos, jboolean value)
{
    ((reinterpret_cast<bool *>(ptr))[pos]) = value;
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QNativePointer_writeByte)
  (JNIEnv *, jclass, jlong ptr, jlong pos, jbyte value)
{
    (((reinterpret_cast<qint8 *>(ptr)))[pos]) = value;
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QNativePointer_writeChar)
  (JNIEnv *, jclass, jlong ptr, jlong pos, jchar value)
{
    (((reinterpret_cast<quint16 *>(ptr)))[pos]) = value;
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QNativePointer_writeShort)
  (JNIEnv *, jclass, jlong ptr, jlong pos, jshort value)
{
    (((reinterpret_cast<qint16 *>(ptr)))[pos]) = value;
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QNativePointer_writeInt)
  (JNIEnv *, jclass, jlong ptr, jlong pos, jint value)
{
    (((reinterpret_cast<qint32 *>(ptr)))[pos]) = value;
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QNativePointer_writeLong)
  (JNIEnv *, jclass, jlong ptr, jlong pos, jlong value)
{
    (((reinterpret_cast<qint64 *>(ptr)))[pos]) = value;
}
extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QNativePointer_writeFloat)
  (JNIEnv *, jclass, jlong ptr, jlong pos, jfloat value)
{
    (((reinterpret_cast<float *>(ptr)))[pos]) = value;
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QNativePointer_writeDouble)
  (JNIEnv *, jclass, jlong ptr, jlong pos, jdouble value)
{
    (((reinterpret_cast<double *>(ptr)))[pos]) = value;
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QNativePointer_writePointer)
  (JNIEnv *, jclass, jlong ptr, jlong pos, jlong value)
{
    (reinterpret_cast<void **>(ptr))[pos] = reinterpret_cast<void *>(value);
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QNativePointer_writeObject)
  (JNIEnv * __jni_env, jclass, jlong buf, jlong _knownSize, jclass valueType, jlong pos, jobject value)
{
    try{
        size_t knownSize = size_t(_knownSize);
        size_t size = getValueSizeForClass(__jni_env, valueType);
        if(size<=0){
            JavaException::raiseIllegalArgumentException(__jni_env, "Object type is not clonable." QTJAMBI_STACKTRACEINFO );
            return;
        }
        if(knownSize % size != 0){
            JavaException::raiseIllegalArgumentException(__jni_env, "Object type does not fit." QTJAMBI_STACKTRACEINFO );
            return;
        }
        if(size*size_t(pos+1) > knownSize){
            JavaException::raiseIllegalArgumentException(__jni_env, "Object type does not fit." QTJAMBI_STACKTRACEINFO );
            return;
        }
        void *ptr = nullptr;
        if (value) {
            if (QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(__jni_env, value))
                ptr = link->pointer();
            else if(Java::QtJambi::QtObjectInterface::isInstanceOf(__jni_env, value))
                Java::QtJambi::QNoNativeResourcesException::throwNew(__jni_env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassName(__jni_env, value).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
        }

        if (buf != 0)
            memcpy(reinterpret_cast<char*>(buf) + size_t(pos) * size, ptr, size);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QNativePointer_writeString)
  (JNIEnv *env, jclass, jlong ptr, jlong pos, jstring value)
{
    try{
        reinterpret_cast<QString *>(ptr)[pos] = qtjambi_cast<QString>(env, value);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

extern "C" Q_DECL_EXPORT jlong JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QNativePointer_createPointer)
  (JNIEnv *, jclass, jint type, jlong _size, jint indirections)
{
    size_t size = size_t(_size);
    Q_ASSERT(indirections > 0);
    Q_ASSERT(size > 0);

    if (indirections != 1) {
        return reinterpret_cast<jlong>(new void*[size]);
    }

    if (size == 1) {
        switch (type) {
        case jint(QNativePointer::Type::Boolean): return reinterpret_cast<jlong>(new bool);
        case jint(QNativePointer::Type::Byte): return reinterpret_cast<jlong>(new qint8);
        case jint(QNativePointer::Type::Char): return reinterpret_cast<jlong>(new quint16);
        case jint(QNativePointer::Type::Short): return reinterpret_cast<jlong>(new qint16);
        case jint(QNativePointer::Type::Int): return reinterpret_cast<jlong>(new qint32);
        case jint(QNativePointer::Type::Long): return reinterpret_cast<jlong>(new qint64);
        case jint(QNativePointer::Type::Float): return reinterpret_cast<jlong>(new float);
        case jint(QNativePointer::Type::Double): return reinterpret_cast<jlong>(new double);
        case jint(QNativePointer::Type::Pointer): return reinterpret_cast<jlong>(new void *);
        case jint(QNativePointer::Type::String): return reinterpret_cast<jlong>(new QString());
        }
    } else if (size > 1) {
        switch (type) {
        case jint(QNativePointer::Type::Boolean): return reinterpret_cast<jlong>(new bool[size]);
        case jint(QNativePointer::Type::Byte): return reinterpret_cast<jlong>(new qint8[size]);
        case jint(QNativePointer::Type::Char): return reinterpret_cast<jlong>(new quint16[size]);
        case jint(QNativePointer::Type::Short): return reinterpret_cast<jlong>(new qint16[size]);
        case jint(QNativePointer::Type::Int): return reinterpret_cast<jlong>(new qint32[size]);
        case jint(QNativePointer::Type::Long): return reinterpret_cast<jlong>(new qint64[size]);
        case jint(QNativePointer::Type::Float): return reinterpret_cast<jlong>(new float[size]);
        case jint(QNativePointer::Type::Double): return reinterpret_cast<jlong>(new double[size]);
        case jint(QNativePointer::Type::Pointer): return reinterpret_cast<jlong>(new void *[size]);
        case jint(QNativePointer::Type::String): return reinterpret_cast<jlong>(new QString[size]);
        }
    }
    return 0;
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QNativePointer_deletePointer)
  (JNIEnv *, jclass, jlong ptr, jint type, jint deleteMode)
{
    if (deleteMode == 0) { // free()
        switch (type) {
        case jint(QNativePointer::Type::Boolean):       free((reinterpret_cast<bool *>(ptr))); break;
        case jint(QNativePointer::Type::Byte):          free((reinterpret_cast<qint8 *>(ptr))); break;
        case jint(QNativePointer::Type::Char):          free((reinterpret_cast<quint16 *>(ptr))); break;
        case jint(QNativePointer::Type::Short):         free((reinterpret_cast<qint16 *>(ptr))); break;
        case jint(QNativePointer::Type::Int):           free((reinterpret_cast<int *>(ptr))); break;
        case jint(QNativePointer::Type::Long):          free((reinterpret_cast<qint64 *>(ptr))); break;
        case jint(QNativePointer::Type::Float):         free((reinterpret_cast<float *>(ptr))); break;
        case jint(QNativePointer::Type::Double):        free((reinterpret_cast<double *>(ptr))); break;
        default:
            qCWarning(DebugAPI::internalCategory, "Unhandled free of type: %d\n", int(type));
            break;
        }

    } else if (deleteMode == 1) { // delete
        switch (type) {
        case jint(QNativePointer::Type::Boolean):       delete ((reinterpret_cast<bool *>(ptr))); break;
        case jint(QNativePointer::Type::Byte):          delete ((reinterpret_cast<qint8 *>(ptr))); break;
        case jint(QNativePointer::Type::Char):          delete ((reinterpret_cast<quint16 *>(ptr))); break;
        case jint(QNativePointer::Type::Short):         delete ((reinterpret_cast<qint16 *>(ptr))); break;
        case jint(QNativePointer::Type::Int):           delete ((reinterpret_cast<int *>(ptr))); break;
        case jint(QNativePointer::Type::Long):          delete ((reinterpret_cast<qint64 *>(ptr))); break;
        case jint(QNativePointer::Type::Float):         delete ((reinterpret_cast<float *>(ptr))); break;
        case jint(QNativePointer::Type::Double):        delete ((reinterpret_cast<double *>(ptr))); break;
        case jint(QNativePointer::Type::String):        delete ((reinterpret_cast<QString *>(ptr))); break;
        default:
            qCWarning(DebugAPI::internalCategory, "Unhandled delete of type: %d\n", int(type));
            break;
        }

    } else if (deleteMode == 2) { // delete []
        switch (type) {
        case jint(QNativePointer::Type::Boolean):       delete [] ((reinterpret_cast<bool *>(ptr))); break;
        case jint(QNativePointer::Type::Byte):          delete [] ((reinterpret_cast<qint8 *>(ptr))); break;
        case jint(QNativePointer::Type::Char):          delete [] ((reinterpret_cast<quint16 *>(ptr))); break;
        case jint(QNativePointer::Type::Short):         delete [] ((reinterpret_cast<qint16 *>(ptr))); break;
        case jint(QNativePointer::Type::Int):           delete [] ((reinterpret_cast<int *>(ptr))); break;
        case jint(QNativePointer::Type::Long):          delete [] ((reinterpret_cast<qint64 *>(ptr))); break;
        case jint(QNativePointer::Type::Float):         delete [] ((reinterpret_cast<float *>(ptr))); break;
        case jint(QNativePointer::Type::Double):        delete [] ((reinterpret_cast<double *>(ptr))); break;
        case jint(QNativePointer::Type::String):        delete [] ((reinterpret_cast<QString *>(ptr))); break;
        default:
            qCWarning(DebugAPI::internalCategory, "Unhandled delete [] of type: %d\n", int(type));
            break;
        }
    }
}

class DirectAddressIODevice : public QIODevice{
public:
    DirectAddressIODevice(qint64 capacity, char* address, bool readOnly, JNIEnv* env, jobject buffer, QObject *parent = nullptr);
    bool open(OpenMode mode);
protected:
    qint64 readData(char *data, qint64 maxlen);
    qint64 writeData(const char *data, qint64 len);
private:
    const qint64 m_capacity;
    char* const m_address;
    const bool m_readOnly;
    JObjectWrapper m_buffer;
};

DirectAddressIODevice::DirectAddressIODevice(qint64 capacity, char* address, bool readOnly, JNIEnv* env, jobject object, QObject *parent)
    : QIODevice(parent),
      m_capacity(capacity),
      m_address(address),
      m_readOnly(readOnly),
      m_buffer(env, object) {}

bool DirectAddressIODevice::open(OpenMode mode)
{
    if(mode & QIODevice::Append)
        return false;
    if(bool(mode & QIODevice::WriteOnly) == m_readOnly || m_address==nullptr){
        return false;
    }
    return QIODevice::open(mode);
}

qint64 DirectAddressIODevice::readData(char *data, qint64 maxlen)
{
    if((openMode() & QIODevice::ReadOnly) && maxlen>0){
        qint64 len = qMin(maxlen, m_capacity-pos());
        if(len>0)
            memcpy(data, m_address+pos(), size_t(len));
        return qint64(len);
    }
    return -1;
}

qint64 DirectAddressIODevice::writeData(const char *data, qint64 len)
{
    if((openMode() & QIODevice::WriteOnly)){
        qint64 _len = qMin(len, m_capacity-pos());
        if(_len>0)
            memcpy(m_address+pos(), data, size_t(_len));
        return qint64(_len);
    }
    return -1;
}

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QNativePointer_ioDevice)
(JNIEnv *env,
 jobject buffer)
{
    try{
        jint indirections = Java::QtJambi::QNativePointer::indirections(env,buffer);
        if(indirections==1){
            jlong capacity = Java::QtJambi::QNativePointer::byteSize(env,buffer);
            char* address = reinterpret_cast<char*>(Java::QtJambi::QNativePointer::pointer(env,buffer));
            bool readOnly = Java::QtJambi::QNativePointer::isReadOnly(env, buffer);
            if(address && capacity>0){
                QIODevice* device = new DirectAddressIODevice(capacity, address, readOnly, env, buffer);
                return qtjambi_cast<jobject>(env, device);
            }
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

class BufferIODevice : public QIODevice{
public:
    BufferIODevice(JNIEnv* env, jobject buffer, QObject *parent = nullptr);
    ~BufferIODevice();
    bool open(OpenMode mode);
protected:
    qint64 readData(char *data, qint64 maxlen);
    qint64 writeData(const char *data, qint64 len);
private:
    const bool m_readOnly;
    JBufferConstData* d;
};

BufferIODevice::BufferIODevice(JNIEnv* env, jobject object, QObject *parent)
    : QIODevice(parent),
    m_readOnly(object && Java::Runtime::Buffer::isReadOnly(env, object)),
    d(m_readOnly ? new JBufferConstData(env, object) : new JBufferData(env, object)) {}

BufferIODevice::~BufferIODevice(){
    if(m_readOnly){
        delete d;
    }else{
        delete reinterpret_cast<JBufferData*>(d);
    }
    d = nullptr;
}

bool BufferIODevice::open(OpenMode mode)
{
    if(mode & QIODevice::Append)
        return false;
    if(bool(mode & QIODevice::WriteOnly) == m_readOnly){
        return false;
    }
    return QIODevice::open(mode);
}

qint64 BufferIODevice::readData(char *data, qint64 maxlen)
{
    if((openMode() & QIODevice::ReadOnly) && maxlen>0){
        qint64 len = qMin(maxlen, d->limit()-pos());
        if(len>0)
            memcpy(data, d->data<char>()+pos(), size_t(len));
        return qint64(len);
    }
    return -1;
}

qint64 BufferIODevice::writeData(const char *data, qint64 len)
{
    if((openMode() & QIODevice::WriteOnly)){
        qint64 _len = qMin(len, d->limit()-pos());
        if(_len>0)
            memcpy(reinterpret_cast<JBufferData*>(d)->data<char>()+pos(), data, size_t(_len));
        return qint64(_len);
    }
    return -1;
}

QIODevice* CoreAPI::newDirectAddressDevice(JNIEnv* env, jobject buffer, QObject *parent){
    return env->IsSameObject(buffer, nullptr) ? nullptr : new BufferIODevice(env, buffer, parent);
}
