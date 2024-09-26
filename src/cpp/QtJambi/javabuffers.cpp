/****************************************************************************
**
** Copyright (C) 2009-2024 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
**
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
**
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "qtjambiapi.h"
#include "javabuffers.h"
#include "java_p.h"
#include "registryutil_p.h"

jobject getBufferArray(JNIEnv *env, jobject buffer);

JBufferConstData::JBufferConstData(JNIEnv *env, jobject buffer_object) :
    m_buffer_object(buffer_object ? env->NewGlobalRef(buffer_object) : nullptr),
    m_capacity(0),
    m_position(0),
    m_limit(0),
    m_data(nullptr),
    m_isdirect(true),
    m_readOnly(false),
    m_is_copy(false)
{
    if(m_buffer_object){
        m_isdirect = Java::Runtime::Buffer::isDirect(env, m_buffer_object);
        m_limit = Java::Runtime::Internal::Buffer::limit(env, m_buffer_object);
        m_position = Java::Runtime::Internal::Buffer::position(env, m_buffer_object);
        m_readOnly = Java::Runtime::Buffer::isReadOnly(env, m_buffer_object);
        if(m_isdirect){
            m_capacity = env->GetDirectBufferCapacity(m_buffer_object);
            m_data = reinterpret_cast<char*>(env->GetDirectBufferAddress(m_buffer_object));
        }else{
            jobject bufferArray = nullptr;
            try{
                bufferArray = getBufferArray(env, m_buffer_object);
            }catch(const JavaException&){
            }
            if(Java::Runtime::ByteBuffer::isInstanceOf(env, m_buffer_object)){
                if(bufferArray && env->IsInstanceOf(bufferArray, getArrayClass(env, Java::Runtime::Byte::primitiveType(env), 1))){
                    m_capacity = env->GetArrayLength(jarray(bufferArray));
                    if(m_capacity>0){
                        m_data = reinterpret_cast<char*>(env->GetByteArrayElements(jbyteArray(bufferArray), &m_is_copy));
                    }
                }else{
                    m_capacity = Java::Runtime::Internal::Buffer::capacity(env, m_buffer_object);
                    if(m_capacity>0){
                        jbyte* array = new jbyte[size_t(m_capacity)];
                        for(qsizetype i=0; i<m_capacity; ++i){
                            array[i] = Java::Runtime::ByteBuffer::get(env, m_buffer_object, jsize(i));
                        }
                        m_data = reinterpret_cast<char*>(array);
                        m_is_copy = true;
                    }
                }
            }else if(Java::Runtime::IntBuffer::isInstanceOf(env, m_buffer_object)){
                if(bufferArray && env->IsInstanceOf(bufferArray, getArrayClass(env, Java::Runtime::Integer::primitiveType(env), 1))){
                    m_capacity = env->GetArrayLength(jarray(bufferArray));
                    if(m_capacity>0){
                        m_data = reinterpret_cast<char*>(env->GetIntArrayElements(jintArray(bufferArray), &m_is_copy));
                    }
                }else{
                    m_capacity = Java::Runtime::Internal::Buffer::capacity(env, m_buffer_object);
                    if(m_capacity>0){
                        jint* array = new jint[size_t(m_capacity)];
                        for(qsizetype i=0; i<m_capacity; ++i){
                            array[i] = Java::Runtime::IntBuffer::get(env, m_buffer_object, jsize(i));
                        }
                        m_data = reinterpret_cast<char*>(array);
                        m_is_copy = true;
                    }
                }
            }else if(Java::Runtime::ShortBuffer::isInstanceOf(env, m_buffer_object)){
                if(bufferArray && env->IsInstanceOf(bufferArray, getArrayClass(env, Java::Runtime::Short::primitiveType(env), 1))){
                    m_capacity = env->GetArrayLength(jarray(bufferArray));
                    if(m_capacity>0){
                        m_data = reinterpret_cast<char*>(env->GetShortArrayElements(jshortArray(bufferArray), &m_is_copy));
                    }
                }else{
                    m_capacity = Java::Runtime::Internal::Buffer::capacity(env, m_buffer_object);
                    if(m_capacity>0){
                        jshort* array = new jshort[size_t(m_capacity)];
                        for(qsizetype i=0; i<m_capacity; ++i){
                            array[i] = Java::Runtime::ShortBuffer::get(env, m_buffer_object, jsize(i));
                        }
                        m_data = reinterpret_cast<char*>(array);
                        m_is_copy = true;
                    }
                }
            }else if(Java::Runtime::CharBuffer::isInstanceOf(env, m_buffer_object)){
                if(bufferArray && env->IsInstanceOf(bufferArray, getArrayClass(env, Java::Runtime::Character::primitiveType(env), 1))){
                    m_capacity = m_limit = env->GetArrayLength(jarray(bufferArray));
                    if(m_capacity>0){
                        m_data = reinterpret_cast<char*>(env->GetCharArrayElements(jcharArray(bufferArray), &m_is_copy));
                    }
                }else{
                    m_capacity = Java::Runtime::Internal::Buffer::capacity(env, m_buffer_object);
                    if(m_capacity>0){
                        jchar* array = new jchar[size_t(m_capacity)];
                        for(qsizetype i=0; i<m_capacity; ++i){
                            array[i] = Java::Runtime::CharBuffer::get(env, m_buffer_object, jsize(i));
                        }
                        m_data = reinterpret_cast<char*>(array);
                        m_is_copy = true;
                    }
                }
            }else if(Java::Runtime::LongBuffer::isInstanceOf(env, m_buffer_object)){
                if(bufferArray && env->IsInstanceOf(bufferArray, getArrayClass(env, Java::Runtime::Long::primitiveType(env), 1))){
                    m_capacity = m_limit = env->GetArrayLength(jarray(bufferArray));
                    if(m_capacity>0){
                        m_data = reinterpret_cast<char*>(env->GetLongArrayElements(jlongArray(bufferArray), &m_is_copy));
                    }
                }else{
                    m_capacity = Java::Runtime::Internal::Buffer::capacity(env, m_buffer_object);
                    if(m_capacity>0){
                        jlong* array = new jlong[size_t(m_capacity)];
                        for(qsizetype i=0; i<m_capacity; ++i){
                            array[i] = Java::Runtime::LongBuffer::get(env, m_buffer_object, jsize(i));
                        }
                        m_data = reinterpret_cast<char*>(array);
                        m_is_copy = true;
                    }
                }
            }else if(Java::Runtime::FloatBuffer::isInstanceOf(env, m_buffer_object)){
                if(bufferArray && env->IsInstanceOf(bufferArray, getArrayClass(env, Java::Runtime::Float::primitiveType(env), 1))){
                    m_capacity = m_limit = env->GetArrayLength(jarray(bufferArray));
                    if(m_capacity>0){
                        m_data = reinterpret_cast<char*>(env->GetFloatArrayElements(jfloatArray(bufferArray), &m_is_copy));
                    }
                }else{
                    m_capacity = Java::Runtime::Internal::Buffer::capacity(env, m_buffer_object);
                    if(m_capacity>0){
                        jfloat* array = new jfloat[size_t(m_capacity)];
                        for(qsizetype i=0; i<m_capacity; ++i){
                            array[i] = Java::Runtime::FloatBuffer::get(env, m_buffer_object, jsize(i));
                        }
                        m_data = reinterpret_cast<char*>(array);
                        m_is_copy = true;
                    }
                }
            }else if(Java::Runtime::DoubleBuffer::isInstanceOf(env, m_buffer_object)){
                if(bufferArray && env->IsInstanceOf(bufferArray, getArrayClass(env, Java::Runtime::Double::primitiveType(env), 1))){
                    m_capacity = m_limit = env->GetArrayLength(jarray(bufferArray));
                    if(m_capacity>0){
                        m_data = reinterpret_cast<char*>(env->GetDoubleArrayElements(jdoubleArray(bufferArray), &m_is_copy));
                    }
                }else{
                    m_capacity = Java::Runtime::Internal::Buffer::capacity(env, m_buffer_object);
                    if(m_capacity>0){
                        jdouble* array = new jdouble[size_t(m_capacity)];
                        for(qsizetype i=0; i<m_capacity; ++i){
                            array[i] = Java::Runtime::DoubleBuffer::get(env, m_buffer_object, jsize(i));
                        }
                        m_data = reinterpret_cast<char*>(array);
                        m_is_copy = true;
                    }
                }
            }
        }
        if(Java::Runtime::IntBuffer::isInstanceOf(env, m_buffer_object)){
            m_capacity = m_capacity * sizeof(jint) / sizeof(char);
            m_limit = m_limit * sizeof(jint) / sizeof(char);
            m_position = m_position * sizeof(jint) / sizeof(char);
        }else if(Java::Runtime::ShortBuffer::isInstanceOf(env, m_buffer_object)){
            m_capacity = m_capacity * sizeof(jshort) / sizeof(char);
            m_limit = m_limit * sizeof(jshort) / sizeof(char);
            m_position = m_position * sizeof(jshort) / sizeof(char);
        }else if(Java::Runtime::CharBuffer::isInstanceOf(env, m_buffer_object)){
            m_capacity = m_capacity * sizeof(jchar) / sizeof(char);
            m_limit = m_limit * sizeof(jchar) / sizeof(char);
            m_position = m_position * sizeof(jchar) / sizeof(char);
        }else if(Java::Runtime::LongBuffer::isInstanceOf(env, m_buffer_object)){
            m_capacity = m_capacity * sizeof(jlong) / sizeof(char);
            m_limit = m_limit * sizeof(jlong) / sizeof(char);
            m_position = m_position * sizeof(jlong) / sizeof(char);
        }else if(Java::Runtime::FloatBuffer::isInstanceOf(env, m_buffer_object)){
            m_capacity = jsize(size_t(m_capacity) * sizeof(jfloat) / sizeof(char));
            m_limit = m_limit * sizeof(jfloat) / sizeof(char);
            m_position = m_position * sizeof(jfloat) / sizeof(char);
        }else if(Java::Runtime::DoubleBuffer::isInstanceOf(env, m_buffer_object)){
            m_capacity = m_capacity * sizeof(jdouble) / sizeof(char);
            m_limit = m_limit * sizeof(jdouble) / sizeof(char);
            m_position = m_position * sizeof(jdouble) / sizeof(char);
        }
    }
}

const void* JBufferConstData::data() const {return m_data+m_position;}
const void* JBufferConstData::constData() const {return m_data+m_position;}
void JBufferConstData::commit(JNIEnv *){}

void JBufferData::commit(JNIEnv *env){
    if(m_buffer_object && !m_isdirect && m_capacity>0 && m_is_copy){
        jobject bufferArray = getBufferArray(env, m_buffer_object);
        if(Java::Runtime::ByteBuffer::isInstanceOf(env, m_buffer_object)){
            if(bufferArray && env->IsInstanceOf(bufferArray, getArrayClass(env, Java::Runtime::Byte::primitiveType(env), 1))){
                env->ReleaseByteArrayElements(jbyteArray(bufferArray), reinterpret_cast<jbyte*>(m_data), 0);
                m_data = reinterpret_cast<char*>(env->GetByteArrayElements(jbyteArray(bufferArray), &m_is_copy));
            }else{
                jbyte* array = reinterpret_cast<jbyte*>(m_data);
                jsize size = jsize(size_t(m_capacity) * sizeof(char) / sizeof(jbyte));
                for(jsize i=0; i<size; ++i){
                    Java::Runtime::ByteBuffer::put(env, m_buffer_object, i, array[i]);
                }
            }
        }else if(Java::Runtime::IntBuffer::isInstanceOf(env, m_buffer_object)){
            if(bufferArray && env->IsInstanceOf(bufferArray, getArrayClass(env, Java::Runtime::Integer::primitiveType(env), 1))){
                env->ReleaseIntArrayElements(jintArray(bufferArray), reinterpret_cast<jint*>(m_data), 0);
                m_data = reinterpret_cast<char*>(env->GetIntArrayElements(jintArray(bufferArray), &m_is_copy));
            }else{
                jint* array = reinterpret_cast<jint*>(m_data);
                jsize size = jsize(size_t(m_capacity) * sizeof(char) / sizeof(jint));
                for(jsize i=0; i<size; ++i){
                    Java::Runtime::IntBuffer::put(env, m_buffer_object, i, array[i]);
                }
            }
        }else if(Java::Runtime::ShortBuffer::isInstanceOf(env, m_buffer_object)){
            if(bufferArray && env->IsInstanceOf(bufferArray, getArrayClass(env, Java::Runtime::Short::primitiveType(env), 1))){
                env->ReleaseShortArrayElements(jshortArray(bufferArray), reinterpret_cast<jshort*>(m_data), 0);
                m_data = reinterpret_cast<char*>(env->GetShortArrayElements(jshortArray(bufferArray), &m_is_copy));
            }else{
                jshort* array = reinterpret_cast<jshort*>(m_data);
                jsize size = jsize(size_t(m_capacity) * sizeof(char) / sizeof(jshort));
                for(jsize i=0; i<size; ++i){
                    Java::Runtime::ShortBuffer::put(env, m_buffer_object, i, array[i]);
                }
            }
        }else if(Java::Runtime::CharBuffer::isInstanceOf(env, m_buffer_object)){
            if(bufferArray && env->IsInstanceOf(bufferArray, getArrayClass(env, Java::Runtime::Character::primitiveType(env), 1))){
                env->ReleaseCharArrayElements(jcharArray(bufferArray), reinterpret_cast<jchar*>(m_data), 0);
                m_data = reinterpret_cast<char*>(env->GetCharArrayElements(jcharArray(bufferArray), &m_is_copy));
            }else{
                jchar* array = reinterpret_cast<jchar*>(m_data);
                jsize size = jsize(size_t(m_capacity) * sizeof(char) / sizeof(jchar));
                for(jsize i=0; i<size; ++i){
                    Java::Runtime::CharBuffer::put(env, m_buffer_object, i, array[i]);
                }
            }
        }else if(Java::Runtime::LongBuffer::isInstanceOf(env, m_buffer_object)){
            if(bufferArray && env->IsInstanceOf(bufferArray, getArrayClass(env, Java::Runtime::Long::primitiveType(env), 1))){
                env->ReleaseLongArrayElements(jlongArray(bufferArray), reinterpret_cast<jlong*>(m_data), 0);
                m_data = reinterpret_cast<char*>(env->GetLongArrayElements(jlongArray(bufferArray), &m_is_copy));
            }else{
                jlong* array = reinterpret_cast<jlong*>(m_data);
                m_data = nullptr;
                jsize size = jsize(size_t(m_capacity) * sizeof(char) / sizeof(jlong));
                for(jsize i=0; i<size; ++i){
                    Java::Runtime::LongBuffer::put(env, m_buffer_object, i, array[i]);
                }
            }
        }else if(Java::Runtime::FloatBuffer::isInstanceOf(env, m_buffer_object)){
            if(bufferArray && env->IsInstanceOf(bufferArray, getArrayClass(env, Java::Runtime::Float::primitiveType(env), 1))){
                env->ReleaseFloatArrayElements(jfloatArray(bufferArray), reinterpret_cast<jfloat*>(m_data), 0);
                m_data = reinterpret_cast<char*>(env->GetFloatArrayElements(jfloatArray(bufferArray), &m_is_copy));
            }else{
                jfloat* array = reinterpret_cast<jfloat*>(m_data);
                jsize size = jsize(size_t(m_capacity) * sizeof(char) / sizeof(jfloat));
                for(jsize i=0; i<size; ++i){
                    Java::Runtime::FloatBuffer::put(env, m_buffer_object, i, double(array[i]));
                }
            }
        }else if(Java::Runtime::DoubleBuffer::isInstanceOf(env, m_buffer_object)){
            if(bufferArray && env->IsInstanceOf(bufferArray, getArrayClass(env, Java::Runtime::Double::primitiveType(env), 1))){
                env->ReleaseDoubleArrayElements(jdoubleArray(bufferArray), reinterpret_cast<jdouble*>(m_data), 0);
                m_data = reinterpret_cast<char*>(env->GetDoubleArrayElements(jdoubleArray(bufferArray), &m_is_copy));
            }else{
                jdouble* array = reinterpret_cast<jdouble*>(m_data);
                jsize size = jsize(size_t(m_capacity) * sizeof(char) / sizeof(jdouble));
                for(jsize i=0; i<size; ++i){
                    Java::Runtime::DoubleBuffer::put(env, m_buffer_object, i, array[i]);
                }
            }
        }
    }
}
void* JBufferData::data() {return m_data+m_position;}

void* JBufferConstData::take(){
    void* result = m_data;
    if(!m_isdirect){
        char* array = new char[size_t(m_capacity)];
        memcpy(array, m_data, size_t(m_capacity));
        result = array;
    }
    m_data = nullptr;
    return result;
}

JBufferConstData::~JBufferConstData(){
    try{
        if(m_buffer_object){
            if(DefaultJniEnvironment env{500}){
                if(m_data && !m_isdirect && m_capacity>0){
                    jobject bufferArray = nullptr;
                    try{
                        bufferArray = getBufferArray(env, m_buffer_object);
                    }catch(const JavaException&){
                    }
                    if(Java::Runtime::ByteBuffer::isInstanceOf(env, m_buffer_object)){
                        if(bufferArray && env->IsInstanceOf(bufferArray, getArrayClass(env, Java::Runtime::Byte::primitiveType(env), 1))){
                            env->ReleaseByteArrayElements(jbyteArray(bufferArray), reinterpret_cast<jbyte*>(m_data), JNI_ABORT);
                        }else
                            delete[] reinterpret_cast<jbyte*>(m_data);
                    }else if(Java::Runtime::IntBuffer::isInstanceOf(env, m_buffer_object)){
                        if(bufferArray && env->IsInstanceOf(bufferArray, getArrayClass(env, Java::Runtime::Integer::primitiveType(env), 1))){
                            env->ReleaseIntArrayElements(jintArray(bufferArray), reinterpret_cast<jint*>(m_data), JNI_ABORT);
                        }else
                            delete[] reinterpret_cast<jint*>(m_data);
                    }else if(Java::Runtime::ShortBuffer::isInstanceOf(env, m_buffer_object)){
                        if(bufferArray && env->IsInstanceOf(bufferArray, getArrayClass(env, Java::Runtime::Short::primitiveType(env), 1))){
                            env->ReleaseShortArrayElements(jshortArray(bufferArray), reinterpret_cast<jshort*>(m_data), JNI_ABORT);
                        }else
                            delete[] reinterpret_cast<jshort*>(m_data);
                    }else if(Java::Runtime::CharBuffer::isInstanceOf(env, m_buffer_object)){
                        if(bufferArray && env->IsInstanceOf(bufferArray, getArrayClass(env, Java::Runtime::Character::primitiveType(env), 1))){
                            env->ReleaseCharArrayElements(jcharArray(bufferArray), reinterpret_cast<jchar*>(m_data), JNI_ABORT);
                        }else
                            delete[] reinterpret_cast<jchar*>(m_data);
                    }else if(Java::Runtime::LongBuffer::isInstanceOf(env, m_buffer_object)){
                        if(bufferArray && env->IsInstanceOf(bufferArray, getArrayClass(env, Java::Runtime::Long::primitiveType(env), 1))){
                            env->ReleaseLongArrayElements(jlongArray(bufferArray), reinterpret_cast<jlong*>(m_data), JNI_ABORT);
                        }else
                            delete[] reinterpret_cast<jlong*>(m_data);
                    }else if(Java::Runtime::FloatBuffer::isInstanceOf(env, m_buffer_object)){
                        if(bufferArray && env->IsInstanceOf(bufferArray, getArrayClass(env, Java::Runtime::Float::primitiveType(env), 1))){
                            env->ReleaseFloatArrayElements(jfloatArray(bufferArray), reinterpret_cast<jfloat*>(m_data), JNI_ABORT);
                        }else
                            delete[] reinterpret_cast<jfloat*>(m_data);
                    }else if(Java::Runtime::DoubleBuffer::isInstanceOf(env, m_buffer_object)){
                        if(bufferArray && env->IsInstanceOf(bufferArray, getArrayClass(env, Java::Runtime::Double::primitiveType(env), 1))){
                            env->ReleaseDoubleArrayElements(jdoubleArray(bufferArray), reinterpret_cast<jdouble*>(m_data), JNI_ABORT);
                        }else
                            delete[] reinterpret_cast<jdouble*>(m_data);
                    }
                }
                env->DeleteGlobalRef(m_buffer_object);
            }
        }
    } catch (const std::exception& e) {
        qCWarning(DebugAPI::internalCategory, "%s", e.what());
    } catch (...) {
    }
}

qsizetype JBufferConstData::size() const{
    return m_limit - m_position;
}

qsizetype JBufferConstData::capacity() const{
    return m_capacity;
}

qsizetype JBufferConstData::position() const{
    return m_position;
}

qsizetype JBufferConstData::limit() const{
    return m_limit;
}

bool JBufferConstData::isDirect() const {return m_isdirect;}
bool JBufferConstData::isReadOnly() const {return m_readOnly;}
bool JBufferConstData::isBuffering() const { return m_is_copy; }

bool JBufferConstData::isBuffer(JNIEnv *env, jobject obj){
    return Java::Runtime::Buffer::isInstanceOf(env, obj);
}

bool JBufferConstData::isReadOnlyBuffer(JNIEnv *env, jobject buffer){
    return Java::Runtime::Buffer::isReadOnly(env, buffer);
}

JBufferData::JBufferData(JNIEnv *env, jobject buffer_object) :
    JBufferConstData(env, buffer_object)
{
    if(m_readOnly)
        JavaException::raiseIllegalArgumentException(env, "Cannot write on a read-only buffer" QTJAMBI_STACKTRACEINFO );
}

JBufferData::~JBufferData(){
    try{
        if(m_buffer_object && !m_isdirect && m_capacity>0){
            if(DefaultJniEnvironment env{500}){
                jobject bufferArray = nullptr;
                try{
                    bufferArray = getBufferArray(env, m_buffer_object);
                }catch(const JavaException&){
                }
                if(Java::Runtime::ByteBuffer::isInstanceOf(env, m_buffer_object)){
                    if(bufferArray && env->IsInstanceOf(bufferArray, getArrayClass(env, Java::Runtime::Byte::primitiveType(env), 1))){
                        env->ReleaseByteArrayElements(jbyteArray(bufferArray), reinterpret_cast<jbyte*>(m_data), 0);
                        m_data = nullptr;
                    }else{
                        jbyte* array = reinterpret_cast<jbyte*>(m_data);
                        m_data = nullptr;
                        if(!m_readOnly){
                                jsize size = jsize(size_t(m_capacity) * sizeof(char) / sizeof(jbyte));
                            for(jsize i=0; i<size; ++i){
                                Java::Runtime::ByteBuffer::put(env, m_buffer_object, i, array[i]);
                            }
                        }
                        delete[] array;
                    }
                }else if(Java::Runtime::IntBuffer::isInstanceOf(env, m_buffer_object)){
                    if(bufferArray && env->IsInstanceOf(bufferArray, getArrayClass(env, Java::Runtime::Integer::primitiveType(env), 1))){
                        env->ReleaseIntArrayElements(jintArray(bufferArray), reinterpret_cast<jint*>(m_data), 0);
                        m_data = nullptr;
                    }else{
                        jint* array = reinterpret_cast<jint*>(m_data);
                        m_data = nullptr;
                        if(!m_readOnly){
                                jsize size = jsize(size_t(m_capacity) * sizeof(char) / sizeof(jint));
                            for(jsize i=0; i<size; ++i){
                                Java::Runtime::IntBuffer::put(env, m_buffer_object, i, array[i]);
                            }
                        }
                        delete[] array;
                    }
                }else if(Java::Runtime::ShortBuffer::isInstanceOf(env, m_buffer_object)){
                    if(bufferArray && env->IsInstanceOf(bufferArray, getArrayClass(env, Java::Runtime::Short::primitiveType(env), 1))){
                        env->ReleaseShortArrayElements(jshortArray(bufferArray), reinterpret_cast<jshort*>(m_data), 0);
                        m_data = nullptr;
                    }else{
                        jshort* array = reinterpret_cast<jshort*>(m_data);
                        m_data = nullptr;
                        if(!m_readOnly){
                            jsize size = jsize(size_t(m_capacity) * sizeof(char) / sizeof(jshort));
                            for(jsize i=0; i<size; ++i){
                                Java::Runtime::ShortBuffer::put(env, m_buffer_object, i, array[i]);
                            }
                        }
                        delete[] array;
                    }
                }else if(Java::Runtime::CharBuffer::isInstanceOf(env, m_buffer_object)){
                    if(bufferArray && env->IsInstanceOf(bufferArray, getArrayClass(env, Java::Runtime::Character::primitiveType(env), 1))){
                        env->ReleaseCharArrayElements(jcharArray(bufferArray), reinterpret_cast<jchar*>(m_data), 0);
                        m_data = nullptr;
                    }else{
                        jchar* array = reinterpret_cast<jchar*>(m_data);
                        m_data = nullptr;
                        if(!m_readOnly){
                            jsize size = jsize(size_t(m_capacity) * sizeof(char) / sizeof(jchar));
                            for(jsize i=0; i<size; ++i){
                                Java::Runtime::CharBuffer::put(env, m_buffer_object, i, array[i]);
                            }
                        }
                        delete[] array;
                    }
                }else if(Java::Runtime::LongBuffer::isInstanceOf(env, m_buffer_object)){
                    if(bufferArray && env->IsInstanceOf(bufferArray, getArrayClass(env, Java::Runtime::Long::primitiveType(env), 1))){
                        env->ReleaseLongArrayElements(jlongArray(bufferArray), reinterpret_cast<jlong*>(m_data), 0);
                        m_data = nullptr;
                    }else{
                        jlong* array = reinterpret_cast<jlong*>(m_data);
                        m_data = nullptr;
                        if(!m_readOnly){
                            jsize size = jsize(size_t(m_capacity) * sizeof(char) / sizeof(jlong));
                            for(jsize i=0; i<size; ++i){
                                Java::Runtime::LongBuffer::put(env, m_buffer_object, i, array[i]);
                            }
                        }
                        delete[] array;
                    }
                }else if(Java::Runtime::FloatBuffer::isInstanceOf(env, m_buffer_object)){
                    if(bufferArray && env->IsInstanceOf(bufferArray, getArrayClass(env, Java::Runtime::Float::primitiveType(env), 1))){
                        env->ReleaseFloatArrayElements(jfloatArray(bufferArray), reinterpret_cast<jfloat*>(m_data), 0);
                        m_data = nullptr;
                    }else{
                        jfloat* array = reinterpret_cast<jfloat*>(m_data);
                        m_data = nullptr;
                        if(!m_readOnly){
                            jsize size = jsize(size_t(m_capacity) * sizeof(char) / sizeof(jfloat));
                            for(jsize i=0; i<size; ++i){
                                Java::Runtime::FloatBuffer::put(env, m_buffer_object, i, double(array[i]));
                            }
                        }
                        delete[] array;
                    }
                }else if(Java::Runtime::DoubleBuffer::isInstanceOf(env, m_buffer_object)){
                    if(bufferArray && env->IsInstanceOf(bufferArray, getArrayClass(env, Java::Runtime::Double::primitiveType(env), 1))){
                        env->ReleaseDoubleArrayElements(jdoubleArray(bufferArray), reinterpret_cast<jdouble*>(m_data), 0);
                        m_data = nullptr;
                    }else{
                        jdouble* array = reinterpret_cast<jdouble*>(m_data);
                        m_data = nullptr;
                        if(!m_readOnly){
                            jsize size = jsize(size_t(m_capacity) * sizeof(char) / sizeof(jdouble));
                            for(jsize i=0; i<size; ++i){
                                Java::Runtime::DoubleBuffer::put(env, m_buffer_object, i, array[i]);
                            }
                        }
                        delete[] array;
                    }
                }
            }
        }
    } catch (const std::exception& e) {
        qCWarning(DebugAPI::internalCategory, "%s", e.what());
    } catch (...) {
    }
}

void truncateBuffer(JNIEnv *env, jobject buffer);

AbstractDataJBuffer::AbstractDataJBuffer(JNIEnv *env, void* data, qsizetype capacity) : m_buffer_object(nullptr) {
    if(data){
        m_buffer_object = env->NewDirectByteBuffer(data, jlong(capacity));
    }else{
        m_buffer_object = Java::Runtime::Internal::ByteBuffer::allocate(env, 0);
    }
    m_buffer_object = Java::Runtime::Internal::ByteBuffer::order(env, m_buffer_object, Java::Runtime::ByteOrder::nativeOrder(env));
}

AbstractDataJBuffer::AbstractDataJBuffer(JNIEnv *env, const void* data, qsizetype capacity) : m_buffer_object(nullptr) {
    if(data){
        m_buffer_object = env->NewDirectByteBuffer(const_cast<void*>(data), jlong(capacity));
    }else{
        m_buffer_object = Java::Runtime::Internal::ByteBuffer::allocate(env, 0);
    }
    m_buffer_object = Java::Runtime::ByteBuffer::asReadOnlyBuffer(env, m_buffer_object);
    m_buffer_object = Java::Runtime::Internal::ByteBuffer::order(env, m_buffer_object, Java::Runtime::ByteOrder::nativeOrder(env));
}

AbstractDataJBuffer::AbstractDataJBuffer(JNIEnv *env, char* data, qsizetype capacity)
    : AbstractDataJBuffer(env, reinterpret_cast<void*>(data), capacity)
{
}

AbstractDataJBuffer::AbstractDataJBuffer(JNIEnv *env, const char* data, qsizetype capacity)
    : AbstractDataJBuffer(env, reinterpret_cast<const void*>(data), capacity)
{
}

AbstractDataJBuffer::AbstractDataJBuffer(JNIEnv *env, unsigned char* data, qsizetype capacity)
    : AbstractDataJBuffer(env, reinterpret_cast<void*>(data), capacity)
{
}

AbstractDataJBuffer::AbstractDataJBuffer(JNIEnv *env, const unsigned char* data, qsizetype capacity)
    : AbstractDataJBuffer(env, reinterpret_cast<const void*>(data), capacity)
{
}

AbstractDataJBuffer::AbstractDataJBuffer(JNIEnv *env, qint16* data, qsizetype capacity)
    : AbstractDataJBuffer(env, reinterpret_cast<void*>(data), capacity*sizeof(jshort))
{
    if(m_buffer_object)
        m_buffer_object = Java::Runtime::ByteBuffer::asShortBuffer(env, m_buffer_object);
}

AbstractDataJBuffer::AbstractDataJBuffer(JNIEnv *env, const qint16* data, qsizetype capacity)
    : AbstractDataJBuffer(env, reinterpret_cast<const void*>(data), capacity*sizeof(jshort))
{
    if(m_buffer_object)
        m_buffer_object = Java::Runtime::ByteBuffer::asShortBuffer(env, m_buffer_object);
}

AbstractDataJBuffer::AbstractDataJBuffer(JNIEnv *env, quint16* data, qsizetype capacity)
    : AbstractDataJBuffer(env, reinterpret_cast<void*>(data), capacity*sizeof(jshort))
{
    if(m_buffer_object)
        m_buffer_object = Java::Runtime::ByteBuffer::asShortBuffer(env, m_buffer_object);
}

AbstractDataJBuffer::AbstractDataJBuffer(JNIEnv *env, const quint16* data, qsizetype capacity)
    : AbstractDataJBuffer(env, reinterpret_cast<const void*>(data), capacity*sizeof(jshort))
{
    if(m_buffer_object)
        m_buffer_object = Java::Runtime::ByteBuffer::asShortBuffer(env, m_buffer_object);
}

AbstractDataJBuffer::AbstractDataJBuffer(JNIEnv *env, qint32* data, qsizetype capacity)
    : AbstractDataJBuffer(env, reinterpret_cast<void*>(data), capacity*sizeof(jint))
{
    if(m_buffer_object)
        m_buffer_object = Java::Runtime::ByteBuffer::asIntBuffer(env, m_buffer_object);
}

AbstractDataJBuffer::AbstractDataJBuffer(JNIEnv *env, const qint32* data, qsizetype capacity)
    : AbstractDataJBuffer(env, reinterpret_cast<const void*>(data), capacity*sizeof(jint))
{
    if(m_buffer_object)
        m_buffer_object = Java::Runtime::ByteBuffer::asIntBuffer(env, m_buffer_object);
}

AbstractDataJBuffer::AbstractDataJBuffer(JNIEnv *env, quint32* data, qsizetype capacity)
    : AbstractDataJBuffer(env, reinterpret_cast<void*>(data), capacity*sizeof(jint))
{
    if(m_buffer_object)
        m_buffer_object = Java::Runtime::ByteBuffer::asIntBuffer(env, m_buffer_object);
}

AbstractDataJBuffer::AbstractDataJBuffer(JNIEnv *env, const quint32* data, qsizetype capacity)
    : AbstractDataJBuffer(env, reinterpret_cast<const void*>(data), capacity*sizeof(jint))
{
    if(m_buffer_object)
        m_buffer_object = Java::Runtime::ByteBuffer::asIntBuffer(env, m_buffer_object);
}

AbstractDataJBuffer::AbstractDataJBuffer(JNIEnv *env, qint64* data, qsizetype capacity)
    : AbstractDataJBuffer(env, reinterpret_cast<void*>(data), capacity*sizeof(jlong))
{
    if(m_buffer_object)
        m_buffer_object = Java::Runtime::ByteBuffer::asLongBuffer(env, m_buffer_object);
}

AbstractDataJBuffer::AbstractDataJBuffer(JNIEnv *env, const qint64* data, qsizetype capacity)
    : AbstractDataJBuffer(env, reinterpret_cast<const void*>(data), capacity*sizeof(jlong))
{
    if(m_buffer_object)
        m_buffer_object = Java::Runtime::ByteBuffer::asLongBuffer(env, m_buffer_object);
}

AbstractDataJBuffer::AbstractDataJBuffer(JNIEnv *env, quint64* data, qsizetype capacity)
    : AbstractDataJBuffer(env, reinterpret_cast<void*>(data), capacity*sizeof(jlong))
{
    if(m_buffer_object)
        m_buffer_object = Java::Runtime::ByteBuffer::asLongBuffer(env, m_buffer_object);
}

AbstractDataJBuffer::AbstractDataJBuffer(JNIEnv *env, const quint64* data, qsizetype capacity)
    : AbstractDataJBuffer(env, reinterpret_cast<const void*>(data), capacity*sizeof(jlong))
{
    if(m_buffer_object)
        m_buffer_object = Java::Runtime::ByteBuffer::asLongBuffer(env, m_buffer_object);
}

AbstractDataJBuffer::AbstractDataJBuffer(JNIEnv *env, float* data, qsizetype capacity)
    : AbstractDataJBuffer(env, reinterpret_cast<void*>(data), capacity*sizeof(jfloat))
{
    if(m_buffer_object)
        m_buffer_object = Java::Runtime::ByteBuffer::asFloatBuffer(env, m_buffer_object);
}

AbstractDataJBuffer::AbstractDataJBuffer(JNIEnv *env, const float* data, qsizetype capacity)
    : AbstractDataJBuffer(env, reinterpret_cast<const void*>(data), capacity*sizeof(jfloat))
{
    if(m_buffer_object)
        m_buffer_object = Java::Runtime::ByteBuffer::asFloatBuffer(env, m_buffer_object);
}

AbstractDataJBuffer::AbstractDataJBuffer(JNIEnv *env, double* data, qsizetype capacity)
    : AbstractDataJBuffer(env, reinterpret_cast<void*>(data), capacity*sizeof(jdouble))
{
    if(m_buffer_object)
        m_buffer_object = Java::Runtime::ByteBuffer::asDoubleBuffer(env, m_buffer_object);
}

AbstractDataJBuffer::AbstractDataJBuffer(JNIEnv *env, const double* data, qsizetype capacity)
    : AbstractDataJBuffer(env, reinterpret_cast<const void*>(data), capacity*sizeof(jdouble))
{
    if(m_buffer_object)
        m_buffer_object = Java::Runtime::ByteBuffer::asDoubleBuffer(env, m_buffer_object);
}

AbstractDataJBuffer::AbstractDataJBuffer(JNIEnv *env, QChar* data, qsizetype capacity)
    : AbstractDataJBuffer(env, reinterpret_cast<void*>(data), capacity*sizeof(jchar))
{
    if(m_buffer_object)
        m_buffer_object = Java::Runtime::ByteBuffer::asCharBuffer(env, m_buffer_object);
}
AbstractDataJBuffer::AbstractDataJBuffer(JNIEnv *env, const QChar* data, qsizetype capacity)
    : AbstractDataJBuffer(env, reinterpret_cast<const void*>(data), capacity*sizeof(jchar))
{
    if(m_buffer_object)
        m_buffer_object = Java::Runtime::ByteBuffer::asCharBuffer(env, m_buffer_object);
}

AbstractDataJBuffer::AbstractDataJBuffer(JNIEnv *env, wchar_t* data, qsizetype capacity)
    : AbstractDataJBuffer(env, reinterpret_cast<void*>(data), capacity*sizeof(jchar))
{
    if(m_buffer_object)
        m_buffer_object = Java::Runtime::ByteBuffer::asCharBuffer(env, m_buffer_object);
}
AbstractDataJBuffer::AbstractDataJBuffer(JNIEnv *env, const wchar_t* data, qsizetype capacity)
    : AbstractDataJBuffer(env, reinterpret_cast<const void*>(data), capacity*sizeof(jchar))
{
    if(m_buffer_object)
        m_buffer_object = Java::Runtime::ByteBuffer::asCharBuffer(env, m_buffer_object);
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
AbstractDataJBuffer::AbstractDataJBuffer(JNIEnv *env, char32_t* data, qsizetype capacity)
    : AbstractDataJBuffer(env, reinterpret_cast<void*>(data), capacity*sizeof(char32_t))
{
    if(m_buffer_object)
        m_buffer_object = Java::Runtime::ByteBuffer::asCharBuffer(env, m_buffer_object);
}
AbstractDataJBuffer::AbstractDataJBuffer(JNIEnv *env, const char32_t* data, qsizetype capacity)
    : AbstractDataJBuffer(env, reinterpret_cast<const void*>(data), capacity*sizeof(char32_t))
{
    if(m_buffer_object)
        m_buffer_object = Java::Runtime::ByteBuffer::asCharBuffer(env, m_buffer_object);
}

AbstractDataJBuffer::AbstractDataJBuffer(JNIEnv *env, char16_t* data, qsizetype capacity)
    : AbstractDataJBuffer(env, reinterpret_cast<void*>(data), capacity*sizeof(jchar))
{
    if(m_buffer_object)
        m_buffer_object = Java::Runtime::ByteBuffer::asCharBuffer(env, m_buffer_object);
}
AbstractDataJBuffer::AbstractDataJBuffer(JNIEnv *env, const char16_t* data, qsizetype capacity)
    : AbstractDataJBuffer(env, reinterpret_cast<const void*>(data), capacity*sizeof(jchar))
{
    if(m_buffer_object)
        m_buffer_object = Java::Runtime::ByteBuffer::asCharBuffer(env, m_buffer_object);
}
#endif

AbstractDataJBuffer::~AbstractDataJBuffer(){}

void AbstractDataJBuffer::setLimit(JNIEnv *env, jobject buffer, jsize limit){
    if(buffer)
        Java::Runtime::Internal::Buffer::setLimit(env, buffer, limit);
}

LocalDataJBuffer::LocalDataJBuffer(JNIEnv *env, void* data, qsizetype capacity)
    : AbstractDataJBuffer(env, data, capacity), m_env(env)
{
}

LocalDataJBuffer::LocalDataJBuffer(JNIEnv *env, const void* data, qsizetype capacity)
    : AbstractDataJBuffer(env, data, capacity), m_env(env)
{
}

LocalDataJBuffer::LocalDataJBuffer(JNIEnv *env, char* data, qsizetype capacity)
    : AbstractDataJBuffer(env, data, capacity), m_env(env)
{
}

LocalDataJBuffer::LocalDataJBuffer(JNIEnv *env, const char* data, qsizetype capacity)
    : AbstractDataJBuffer(env, data, capacity), m_env(env)
{
}

LocalDataJBuffer::LocalDataJBuffer(JNIEnv *env, unsigned char* data, qsizetype capacity)
    : AbstractDataJBuffer(env, data, capacity), m_env(env)
{
}

LocalDataJBuffer::LocalDataJBuffer(JNIEnv *env, const unsigned char* data, qsizetype capacity)
    : AbstractDataJBuffer(env, data, capacity), m_env(env)
{
}

LocalDataJBuffer::LocalDataJBuffer(JNIEnv *env, qint16* data, qsizetype capacity)
    : AbstractDataJBuffer(env, data, capacity), m_env(env)
{
}

LocalDataJBuffer::LocalDataJBuffer(JNIEnv *env, const qint16* data, qsizetype capacity)
    : AbstractDataJBuffer(env, data, capacity), m_env(env)
{
}

LocalDataJBuffer::LocalDataJBuffer(JNIEnv *env, quint16* data, qsizetype capacity)
    : AbstractDataJBuffer(env, data, capacity), m_env(env)
{
}

LocalDataJBuffer::LocalDataJBuffer(JNIEnv *env, const quint16* data, qsizetype capacity)
    : AbstractDataJBuffer(env, data, capacity), m_env(env)
{
}

LocalDataJBuffer::LocalDataJBuffer(JNIEnv *env, qint32* data, qsizetype capacity)
    : AbstractDataJBuffer(env, data, capacity), m_env(env)
{
}

LocalDataJBuffer::LocalDataJBuffer(JNIEnv *env, const qint32* data, qsizetype capacity)
    : AbstractDataJBuffer(env, data, capacity), m_env(env)
{
}

LocalDataJBuffer::LocalDataJBuffer(JNIEnv *env, quint32* data, qsizetype capacity)
    : AbstractDataJBuffer(env, data, capacity), m_env(env)
{
}

LocalDataJBuffer::LocalDataJBuffer(JNIEnv *env, const quint32* data, qsizetype capacity)
    : AbstractDataJBuffer(env, data, capacity), m_env(env)
{
}

LocalDataJBuffer::LocalDataJBuffer(JNIEnv *env, qint64* data, qsizetype capacity)
    : AbstractDataJBuffer(env, data, capacity), m_env(env)
{
}

LocalDataJBuffer::LocalDataJBuffer(JNIEnv *env, const qint64* data, qsizetype capacity)
    : AbstractDataJBuffer(env, data, capacity), m_env(env)
{
}

LocalDataJBuffer::LocalDataJBuffer(JNIEnv *env, quint64* data, qsizetype capacity)
    : AbstractDataJBuffer(env, data, capacity), m_env(env)
{
}

LocalDataJBuffer::LocalDataJBuffer(JNIEnv *env, const quint64* data, qsizetype capacity)
    : AbstractDataJBuffer(env, data, capacity), m_env(env)
{
}

LocalDataJBuffer::LocalDataJBuffer(JNIEnv *env, float* data, qsizetype capacity)
    : AbstractDataJBuffer(env, data, capacity), m_env(env)
{
}

LocalDataJBuffer::LocalDataJBuffer(JNIEnv *env, const float* data, qsizetype capacity)
    : AbstractDataJBuffer(env, data, capacity), m_env(env)
{
}

LocalDataJBuffer::LocalDataJBuffer(JNIEnv *env, double* data, qsizetype capacity)
    : AbstractDataJBuffer(env, data, capacity), m_env(env)
{
}

LocalDataJBuffer::LocalDataJBuffer(JNIEnv *env, const double* data, qsizetype capacity)
    : AbstractDataJBuffer(env, data, capacity), m_env(env)
{
}

LocalDataJBuffer::LocalDataJBuffer(JNIEnv *env, QChar* data, qsizetype capacity)
    : AbstractDataJBuffer(env, data, capacity), m_env(env)
{
}
LocalDataJBuffer::LocalDataJBuffer(JNIEnv *env, const QChar* data, qsizetype capacity)
    : AbstractDataJBuffer(env, data, capacity), m_env(env)
{
}

LocalDataJBuffer::LocalDataJBuffer(JNIEnv *env, wchar_t* data, qsizetype capacity)
    : AbstractDataJBuffer(env, data, capacity), m_env(env)
{
}
LocalDataJBuffer::LocalDataJBuffer(JNIEnv *env, const wchar_t* data, qsizetype capacity)
    : AbstractDataJBuffer(env, data, capacity), m_env(env)
{
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
LocalDataJBuffer::LocalDataJBuffer(JNIEnv *env, char16_t* data, qsizetype capacity)
    : AbstractDataJBuffer(env, data, capacity), m_env(env)
{
}
LocalDataJBuffer::LocalDataJBuffer(JNIEnv *env, const char16_t* data, qsizetype capacity)
    : AbstractDataJBuffer(env, data, capacity), m_env(env)
{
}
LocalDataJBuffer::LocalDataJBuffer(JNIEnv *env, char32_t* data, qsizetype capacity)
    : AbstractDataJBuffer(env, data, capacity), m_env(env)
{
}
LocalDataJBuffer::LocalDataJBuffer(JNIEnv *env, const char32_t* data, qsizetype capacity)
    : AbstractDataJBuffer(env, data, capacity), m_env(env)
{
}
#endif

LocalDataJBuffer::~LocalDataJBuffer(){
    try{
        if(m_buffer_object)
            truncateBuffer(m_env, m_buffer_object);
    }catch(const JavaException& exn){
        exn.report(m_env);
    } catch (const std::exception& e) {
        qCWarning(DebugAPI::internalCategory, "%s", e.what());
    } catch (...) {
    }
}

void LocalDataJBuffer::setLimit(jsize limit){
    setLimit(m_env, m_buffer_object, limit);
}

jobject LocalDataJBuffer::buffer() const{
    return m_buffer_object;
}

jobject LocalDataJBuffer::take(){
    jobject result = m_buffer_object;
    m_buffer_object = nullptr;
    return result;
}

DataJBuffer::DataJBuffer(JNIEnv *env, void* data, qsizetype capacity)
    : AbstractDataJBuffer(env, data, capacity)
{
    if(m_buffer_object)
        m_buffer_object = env->NewGlobalRef(m_buffer_object);
}

DataJBuffer::DataJBuffer(JNIEnv *env, const void* data, qsizetype capacity)
    : AbstractDataJBuffer(env, data, capacity)
{
    if(m_buffer_object)
        m_buffer_object = env->NewGlobalRef(m_buffer_object);
}

DataJBuffer::DataJBuffer(JNIEnv *env, char* data, qsizetype capacity)
    : AbstractDataJBuffer(env, data, capacity)
{
    if(m_buffer_object)
        m_buffer_object = env->NewGlobalRef(m_buffer_object);
}

DataJBuffer::DataJBuffer(JNIEnv *env, const char* data, qsizetype capacity)
    : AbstractDataJBuffer(env, data, capacity)
{
    if(m_buffer_object)
        m_buffer_object = env->NewGlobalRef(m_buffer_object);
}

DataJBuffer::DataJBuffer(JNIEnv *env, unsigned char* data, qsizetype capacity)
    : AbstractDataJBuffer(env, data, capacity)
{
    if(m_buffer_object)
        m_buffer_object = env->NewGlobalRef(m_buffer_object);
}

DataJBuffer::DataJBuffer(JNIEnv *env, const unsigned char* data, qsizetype capacity)
    : AbstractDataJBuffer(env, data, capacity)
{
    if(m_buffer_object)
        m_buffer_object = env->NewGlobalRef(m_buffer_object);
}

DataJBuffer::DataJBuffer(JNIEnv *env, qint16* data, qsizetype capacity)
    : AbstractDataJBuffer(env, data, capacity)
{
    if(m_buffer_object)
        m_buffer_object = env->NewGlobalRef(m_buffer_object);
}

DataJBuffer::DataJBuffer(JNIEnv *env, const qint16* data, qsizetype capacity)
    : AbstractDataJBuffer(env, data, capacity)
{
    if(m_buffer_object)
        m_buffer_object = env->NewGlobalRef(m_buffer_object);
}

DataJBuffer::DataJBuffer(JNIEnv *env, quint16* data, qsizetype capacity)
    : AbstractDataJBuffer(env, data, capacity)
{
    if(m_buffer_object)
        m_buffer_object = env->NewGlobalRef(m_buffer_object);
}

DataJBuffer::DataJBuffer(JNIEnv *env, const quint16* data, qsizetype capacity)
    : AbstractDataJBuffer(env, data, capacity)
{
    if(m_buffer_object)
        m_buffer_object = env->NewGlobalRef(m_buffer_object);
}

DataJBuffer::DataJBuffer(JNIEnv *env, qint32* data, qsizetype capacity)
    : AbstractDataJBuffer(env, data, capacity)
{
    if(m_buffer_object)
        m_buffer_object = env->NewGlobalRef(m_buffer_object);
}

DataJBuffer::DataJBuffer(JNIEnv *env, const qint32* data, qsizetype capacity)
    : AbstractDataJBuffer(env, data, capacity)
{
    if(m_buffer_object)
        m_buffer_object = env->NewGlobalRef(m_buffer_object);
}

DataJBuffer::DataJBuffer(JNIEnv *env, quint32* data, qsizetype capacity)
    : AbstractDataJBuffer(env, data, capacity)
{
    if(m_buffer_object)
        m_buffer_object = env->NewGlobalRef(m_buffer_object);
}

DataJBuffer::DataJBuffer(JNIEnv *env, const quint32* data, qsizetype capacity)
    : AbstractDataJBuffer(env, data, capacity)
{
    if(m_buffer_object)
        m_buffer_object = env->NewGlobalRef(m_buffer_object);
}

DataJBuffer::DataJBuffer(JNIEnv *env, qint64* data, qsizetype capacity)
    : AbstractDataJBuffer(env, data, capacity)
{
    if(m_buffer_object)
        m_buffer_object = env->NewGlobalRef(m_buffer_object);
}

DataJBuffer::DataJBuffer(JNIEnv *env, const qint64* data, qsizetype capacity)
    : AbstractDataJBuffer(env, data, capacity)
{
    if(m_buffer_object)
        m_buffer_object = env->NewGlobalRef(m_buffer_object);
}

DataJBuffer::DataJBuffer(JNIEnv *env, quint64* data, qsizetype capacity)
    : AbstractDataJBuffer(env, data, capacity)
{
    if(m_buffer_object)
        m_buffer_object = env->NewGlobalRef(m_buffer_object);
}

DataJBuffer::DataJBuffer(JNIEnv *env, const quint64* data, qsizetype capacity)
    : AbstractDataJBuffer(env, data, capacity)
{
    if(m_buffer_object)
        m_buffer_object = env->NewGlobalRef(m_buffer_object);
}

DataJBuffer::DataJBuffer(JNIEnv *env, float* data, qsizetype capacity)
    : AbstractDataJBuffer(env, data, capacity)
{
    if(m_buffer_object)
        m_buffer_object = env->NewGlobalRef(m_buffer_object);
}

DataJBuffer::DataJBuffer(JNIEnv *env, const float* data, qsizetype capacity)
    : AbstractDataJBuffer(env, data, capacity)
{
    if(m_buffer_object)
        m_buffer_object = env->NewGlobalRef(m_buffer_object);
}

DataJBuffer::DataJBuffer(JNIEnv *env, double* data, qsizetype capacity)
    : AbstractDataJBuffer(env, data, capacity)
{
    if(m_buffer_object)
        m_buffer_object = env->NewGlobalRef(m_buffer_object);
}

DataJBuffer::DataJBuffer(JNIEnv *env, const double* data, qsizetype capacity)
    : AbstractDataJBuffer(env, data, capacity)
{
    if(m_buffer_object)
        m_buffer_object = env->NewGlobalRef(m_buffer_object);
}

DataJBuffer::DataJBuffer(JNIEnv *env, QChar* data, qsizetype capacity)
    : AbstractDataJBuffer(env, data, capacity)
{
    if(m_buffer_object)
        m_buffer_object = env->NewGlobalRef(m_buffer_object);
}
DataJBuffer::DataJBuffer(JNIEnv *env, const QChar* data, qsizetype capacity)
    : AbstractDataJBuffer(env, data, capacity)
{
    if(m_buffer_object)
        m_buffer_object = env->NewGlobalRef(m_buffer_object);
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
DataJBuffer::DataJBuffer(JNIEnv *env, char16_t* data, qsizetype capacity)
    : AbstractDataJBuffer(env, data, capacity)
{
    if(m_buffer_object)
        m_buffer_object = env->NewGlobalRef(m_buffer_object);
}
DataJBuffer::DataJBuffer(JNIEnv *env, const char16_t* data, qsizetype capacity)
    : AbstractDataJBuffer(env, data, capacity)
{
    if(m_buffer_object)
        m_buffer_object = env->NewGlobalRef(m_buffer_object);
}
DataJBuffer::DataJBuffer(JNIEnv *env, char32_t* data, qsizetype capacity)
    : AbstractDataJBuffer(env, data, capacity)
{
    if(m_buffer_object)
        m_buffer_object = env->NewGlobalRef(m_buffer_object);
}
DataJBuffer::DataJBuffer(JNIEnv *env, const char32_t* data, qsizetype capacity)
    : AbstractDataJBuffer(env, data, capacity)
{
    if(m_buffer_object)
        m_buffer_object = env->NewGlobalRef(m_buffer_object);
}
#endif

DataJBuffer::~DataJBuffer(){
    if(m_buffer_object){
        if(DefaultJniEnvironment env{100}){
            try{
                truncateBuffer(env, m_buffer_object);
                env->DeleteGlobalRef(m_buffer_object);
                m_buffer_object = nullptr;
            }catch(const JavaException& exn){
                exn.report(env);
            } catch (const std::exception& e) {
                qCWarning(DebugAPI::internalCategory, "%s", e.what());
            } catch (...) {
            }
        }
    }
}

void DataJBuffer::setLimit(JNIEnv *env, jsize limit){
    setLimit(env, m_buffer_object, limit);
}

jobject DataJBuffer::buffer(JNIEnv *env) const{
    return m_buffer_object ? env->NewLocalRef(m_buffer_object) : nullptr;
}

jobject DataJBuffer::take(JNIEnv *env){
    jobject result = m_buffer_object ? env->NewLocalRef(m_buffer_object) : nullptr;
    m_buffer_object = nullptr;
    return result;
}

void DataJBuffer::clear(JNIEnv *env){
    if(m_buffer_object){
        truncateBuffer(env, m_buffer_object);
        env->DeleteGlobalRef(m_buffer_object);
        m_buffer_object = nullptr;
    }
}

namespace BufferBase{
QTJAMBI_REPOSITORY_DECLARE_CLASS(Buffer,
                                 QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(base))
QTJAMBI_REPOSITORY_DEFINE_CLASS(java/nio,Buffer,
                                QTJAMBI_REPOSITORY_DEFINE_METHOD(base,()Ljava/lang/Object;)
                                )
}
jobject getBufferArray(JNIEnv *env, jobject buffer){
    typedef jobject (*GetBufferArrayImpl)(JNIEnv *env, jobject buffer);
    static GetBufferArrayImpl getBufferArrayImpl = [](JNIEnv *env) -> bool {
                                                        try{
                                                            BufferBase::Buffer::getClass(env);
                                                            return true;
                                                        }catch(const JavaException&){}
                                                        return false;
                                                    }(env)
                                                  ? GetBufferArrayImpl(&BufferBase::Buffer::base)
                                                  : GetBufferArrayImpl([](JNIEnv *env, jobject buffer) -> jobject {
                                                        if(Java::Runtime::Internal::Buffer::hasArray(env, buffer)){
                                                            return Java::Runtime::Internal::Buffer::array(env, buffer);
                                                        }
                                                        return nullptr;
                                                    });
    return getBufferArrayImpl(env, buffer);
}
