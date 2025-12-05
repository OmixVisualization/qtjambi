/****************************************************************************
**
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#include "pch_p.h"

jobject getBufferArray(JNIEnv *env, jobject buffer);

JBufferConstData::JBufferConstData(JNIEnv *env, jobject buffer_object) :
    m_buffer_object(buffer_object),
    m_capacity(0),
    m_position(0),
    m_limit(0),
    m_data(nullptr),
    m_isdirect(true),
    m_readOnly(false),
    m_is_copy(false),
    m_env(env)
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
void JBufferConstData::commit(){}

void JBufferData::commit(){
    if(m_buffer_object && !m_isdirect && m_capacity>0 && m_is_copy){
        jobject bufferArray = getBufferArray(m_env, m_buffer_object);
        if(Java::Runtime::ByteBuffer::isInstanceOf(m_env, m_buffer_object)){
            if(bufferArray && m_env->IsInstanceOf(bufferArray, getArrayClass(m_env, Java::Runtime::Byte::primitiveType(m_env), 1))){
                m_env->ReleaseByteArrayElements(jbyteArray(bufferArray), reinterpret_cast<jbyte*>(m_data), 0);
                m_data = reinterpret_cast<char*>(m_env->GetByteArrayElements(jbyteArray(bufferArray), &m_is_copy));
            }else{
                jbyte* array = reinterpret_cast<jbyte*>(m_data);
                jsize size = jsize(size_t(m_capacity) * sizeof(char) / sizeof(jbyte));
                for(jsize i=0; i<size; ++i){
                    Java::Runtime::ByteBuffer::put(m_env, m_buffer_object, i, array[i]);
                }
            }
        }else if(Java::Runtime::IntBuffer::isInstanceOf(m_env, m_buffer_object)){
            if(bufferArray && m_env->IsInstanceOf(bufferArray, getArrayClass(m_env, Java::Runtime::Integer::primitiveType(m_env), 1))){
                m_env->ReleaseIntArrayElements(jintArray(bufferArray), reinterpret_cast<jint*>(m_data), 0);
                m_data = reinterpret_cast<char*>(m_env->GetIntArrayElements(jintArray(bufferArray), &m_is_copy));
            }else{
                jint* array = reinterpret_cast<jint*>(m_data);
                jsize size = jsize(size_t(m_capacity) * sizeof(char) / sizeof(jint));
                for(jsize i=0; i<size; ++i){
                    Java::Runtime::IntBuffer::put(m_env, m_buffer_object, i, array[i]);
                }
            }
        }else if(Java::Runtime::ShortBuffer::isInstanceOf(m_env, m_buffer_object)){
            if(bufferArray && m_env->IsInstanceOf(bufferArray, getArrayClass(m_env, Java::Runtime::Short::primitiveType(m_env), 1))){
                m_env->ReleaseShortArrayElements(jshortArray(bufferArray), reinterpret_cast<jshort*>(m_data), 0);
                m_data = reinterpret_cast<char*>(m_env->GetShortArrayElements(jshortArray(bufferArray), &m_is_copy));
            }else{
                jshort* array = reinterpret_cast<jshort*>(m_data);
                jsize size = jsize(size_t(m_capacity) * sizeof(char) / sizeof(jshort));
                for(jsize i=0; i<size; ++i){
                    Java::Runtime::ShortBuffer::put(m_env, m_buffer_object, i, array[i]);
                }
            }
        }else if(Java::Runtime::CharBuffer::isInstanceOf(m_env, m_buffer_object)){
            if(bufferArray && m_env->IsInstanceOf(bufferArray, getArrayClass(m_env, Java::Runtime::Character::primitiveType(m_env), 1))){
                m_env->ReleaseCharArrayElements(jcharArray(bufferArray), reinterpret_cast<jchar*>(m_data), 0);
                m_data = reinterpret_cast<char*>(m_env->GetCharArrayElements(jcharArray(bufferArray), &m_is_copy));
            }else{
                jchar* array = reinterpret_cast<jchar*>(m_data);
                jsize size = jsize(size_t(m_capacity) * sizeof(char) / sizeof(jchar));
                for(jsize i=0; i<size; ++i){
                    Java::Runtime::CharBuffer::put(m_env, m_buffer_object, i, array[i]);
                }
            }
        }else if(Java::Runtime::LongBuffer::isInstanceOf(m_env, m_buffer_object)){
            if(bufferArray && m_env->IsInstanceOf(bufferArray, getArrayClass(m_env, Java::Runtime::Long::primitiveType(m_env), 1))){
                m_env->ReleaseLongArrayElements(jlongArray(bufferArray), reinterpret_cast<jlong*>(m_data), 0);
                m_data = reinterpret_cast<char*>(m_env->GetLongArrayElements(jlongArray(bufferArray), &m_is_copy));
            }else{
                jlong* array = reinterpret_cast<jlong*>(m_data);
                m_data = nullptr;
                jsize size = jsize(size_t(m_capacity) * sizeof(char) / sizeof(jlong));
                for(jsize i=0; i<size; ++i){
                    Java::Runtime::LongBuffer::put(m_env, m_buffer_object, i, array[i]);
                }
            }
        }else if(Java::Runtime::FloatBuffer::isInstanceOf(m_env, m_buffer_object)){
            if(bufferArray && m_env->IsInstanceOf(bufferArray, getArrayClass(m_env, Java::Runtime::Float::primitiveType(m_env), 1))){
                m_env->ReleaseFloatArrayElements(jfloatArray(bufferArray), reinterpret_cast<jfloat*>(m_data), 0);
                m_data = reinterpret_cast<char*>(m_env->GetFloatArrayElements(jfloatArray(bufferArray), &m_is_copy));
            }else{
                jfloat* array = reinterpret_cast<jfloat*>(m_data);
                jsize size = jsize(size_t(m_capacity) * sizeof(char) / sizeof(jfloat));
                for(jsize i=0; i<size; ++i){
                    Java::Runtime::FloatBuffer::put(m_env, m_buffer_object, i, double(array[i]));
                }
            }
        }else if(Java::Runtime::DoubleBuffer::isInstanceOf(m_env, m_buffer_object)){
            if(bufferArray && m_env->IsInstanceOf(bufferArray, getArrayClass(m_env, Java::Runtime::Double::primitiveType(m_env), 1))){
                m_env->ReleaseDoubleArrayElements(jdoubleArray(bufferArray), reinterpret_cast<jdouble*>(m_data), 0);
                m_data = reinterpret_cast<char*>(m_env->GetDoubleArrayElements(jdoubleArray(bufferArray), &m_is_copy));
            }else{
                jdouble* array = reinterpret_cast<jdouble*>(m_data);
                jsize size = jsize(size_t(m_capacity) * sizeof(char) / sizeof(jdouble));
                for(jsize i=0; i<size; ++i){
                    Java::Runtime::DoubleBuffer::put(m_env, m_buffer_object, i, array[i]);
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
            if(m_data && !m_isdirect && m_capacity>0){
                jobject bufferArray = nullptr;
                try{
                    bufferArray = getBufferArray(m_env, m_buffer_object);
                }catch(const JavaException&){
                }
                if(Java::Runtime::ByteBuffer::isInstanceOf(m_env, m_buffer_object)){
                    if(bufferArray && m_env->IsInstanceOf(bufferArray, getArrayClass(m_env, Java::Runtime::Byte::primitiveType(m_env), 1))){
                        m_env->ReleaseByteArrayElements(jbyteArray(bufferArray), reinterpret_cast<jbyte*>(m_data), JNI_ABORT);
                    }else
                        delete[] reinterpret_cast<jbyte*>(m_data);
                }else if(Java::Runtime::IntBuffer::isInstanceOf(m_env, m_buffer_object)){
                    if(bufferArray && m_env->IsInstanceOf(bufferArray, getArrayClass(m_env, Java::Runtime::Integer::primitiveType(m_env), 1))){
                        m_env->ReleaseIntArrayElements(jintArray(bufferArray), reinterpret_cast<jint*>(m_data), JNI_ABORT);
                    }else
                        delete[] reinterpret_cast<jint*>(m_data);
                }else if(Java::Runtime::ShortBuffer::isInstanceOf(m_env, m_buffer_object)){
                    if(bufferArray && m_env->IsInstanceOf(bufferArray, getArrayClass(m_env, Java::Runtime::Short::primitiveType(m_env), 1))){
                        m_env->ReleaseShortArrayElements(jshortArray(bufferArray), reinterpret_cast<jshort*>(m_data), JNI_ABORT);
                    }else
                        delete[] reinterpret_cast<jshort*>(m_data);
                }else if(Java::Runtime::CharBuffer::isInstanceOf(m_env, m_buffer_object)){
                    if(bufferArray && m_env->IsInstanceOf(bufferArray, getArrayClass(m_env, Java::Runtime::Character::primitiveType(m_env), 1))){
                        m_env->ReleaseCharArrayElements(jcharArray(bufferArray), reinterpret_cast<jchar*>(m_data), JNI_ABORT);
                    }else
                        delete[] reinterpret_cast<jchar*>(m_data);
                }else if(Java::Runtime::LongBuffer::isInstanceOf(m_env, m_buffer_object)){
                    if(bufferArray && m_env->IsInstanceOf(bufferArray, getArrayClass(m_env, Java::Runtime::Long::primitiveType(m_env), 1))){
                        m_env->ReleaseLongArrayElements(jlongArray(bufferArray), reinterpret_cast<jlong*>(m_data), JNI_ABORT);
                    }else
                        delete[] reinterpret_cast<jlong*>(m_data);
                }else if(Java::Runtime::FloatBuffer::isInstanceOf(m_env, m_buffer_object)){
                    if(bufferArray && m_env->IsInstanceOf(bufferArray, getArrayClass(m_env, Java::Runtime::Float::primitiveType(m_env), 1))){
                        m_env->ReleaseFloatArrayElements(jfloatArray(bufferArray), reinterpret_cast<jfloat*>(m_data), JNI_ABORT);
                    }else
                        delete[] reinterpret_cast<jfloat*>(m_data);
                }else if(Java::Runtime::DoubleBuffer::isInstanceOf(m_env, m_buffer_object)){
                    if(bufferArray && m_env->IsInstanceOf(bufferArray, getArrayClass(m_env, Java::Runtime::Double::primitiveType(m_env), 1))){
                        m_env->ReleaseDoubleArrayElements(jdoubleArray(bufferArray), reinterpret_cast<jdouble*>(m_data), JNI_ABORT);
                    }else
                        delete[] reinterpret_cast<jdouble*>(m_data);
                }
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
            jobject bufferArray = nullptr;
            try{
                bufferArray = getBufferArray(m_env, m_buffer_object);
            }catch(const JavaException&){
            }
            if(Java::Runtime::ByteBuffer::isInstanceOf(m_env, m_buffer_object)){
                if(bufferArray && m_env->IsInstanceOf(bufferArray, getArrayClass(m_env, Java::Runtime::Byte::primitiveType(m_env), 1))){
                    m_env->ReleaseByteArrayElements(jbyteArray(bufferArray), reinterpret_cast<jbyte*>(m_data), 0);
                    m_data = nullptr;
                }else{
                    jbyte* array = reinterpret_cast<jbyte*>(m_data);
                    m_data = nullptr;
                    if(!m_readOnly){
                        jsize size = jsize(size_t(m_capacity) * sizeof(char) / sizeof(jbyte));
                        for(jsize i=0; i<size; ++i){
                            Java::Runtime::ByteBuffer::put(m_env, m_buffer_object, i, array[i]);
                        }
                    }
                    delete[] array;
                }
            }else if(Java::Runtime::IntBuffer::isInstanceOf(m_env, m_buffer_object)){
                if(bufferArray && m_env->IsInstanceOf(bufferArray, getArrayClass(m_env, Java::Runtime::Integer::primitiveType(m_env), 1))){
                    m_env->ReleaseIntArrayElements(jintArray(bufferArray), reinterpret_cast<jint*>(m_data), 0);
                    m_data = nullptr;
                }else{
                    jint* array = reinterpret_cast<jint*>(m_data);
                    m_data = nullptr;
                    if(!m_readOnly){
                        jsize size = jsize(size_t(m_capacity) * sizeof(char) / sizeof(jint));
                        for(jsize i=0; i<size; ++i){
                            Java::Runtime::IntBuffer::put(m_env, m_buffer_object, i, array[i]);
                        }
                    }
                    delete[] array;
                }
            }else if(Java::Runtime::ShortBuffer::isInstanceOf(m_env, m_buffer_object)){
                if(bufferArray && m_env->IsInstanceOf(bufferArray, getArrayClass(m_env, Java::Runtime::Short::primitiveType(m_env), 1))){
                    m_env->ReleaseShortArrayElements(jshortArray(bufferArray), reinterpret_cast<jshort*>(m_data), 0);
                    m_data = nullptr;
                }else{
                    jshort* array = reinterpret_cast<jshort*>(m_data);
                    m_data = nullptr;
                    if(!m_readOnly){
                        jsize size = jsize(size_t(m_capacity) * sizeof(char) / sizeof(jshort));
                        for(jsize i=0; i<size; ++i){
                            Java::Runtime::ShortBuffer::put(m_env, m_buffer_object, i, array[i]);
                        }
                    }
                    delete[] array;
                }
            }else if(Java::Runtime::CharBuffer::isInstanceOf(m_env, m_buffer_object)){
                if(bufferArray && m_env->IsInstanceOf(bufferArray, getArrayClass(m_env, Java::Runtime::Character::primitiveType(m_env), 1))){
                    m_env->ReleaseCharArrayElements(jcharArray(bufferArray), reinterpret_cast<jchar*>(m_data), 0);
                    m_data = nullptr;
                }else{
                    jchar* array = reinterpret_cast<jchar*>(m_data);
                    m_data = nullptr;
                    if(!m_readOnly){
                        jsize size = jsize(size_t(m_capacity) * sizeof(char) / sizeof(jchar));
                        for(jsize i=0; i<size; ++i){
                            Java::Runtime::CharBuffer::put(m_env, m_buffer_object, i, array[i]);
                        }
                    }
                    delete[] array;
                }
            }else if(Java::Runtime::LongBuffer::isInstanceOf(m_env, m_buffer_object)){
                if(bufferArray && m_env->IsInstanceOf(bufferArray, getArrayClass(m_env, Java::Runtime::Long::primitiveType(m_env), 1))){
                    m_env->ReleaseLongArrayElements(jlongArray(bufferArray), reinterpret_cast<jlong*>(m_data), 0);
                    m_data = nullptr;
                }else{
                    jlong* array = reinterpret_cast<jlong*>(m_data);
                    m_data = nullptr;
                    if(!m_readOnly){
                        jsize size = jsize(size_t(m_capacity) * sizeof(char) / sizeof(jlong));
                        for(jsize i=0; i<size; ++i){
                            Java::Runtime::LongBuffer::put(m_env, m_buffer_object, i, array[i]);
                        }
                    }
                    delete[] array;
                }
            }else if(Java::Runtime::FloatBuffer::isInstanceOf(m_env, m_buffer_object)){
                if(bufferArray && m_env->IsInstanceOf(bufferArray, getArrayClass(m_env, Java::Runtime::Float::primitiveType(m_env), 1))){
                    m_env->ReleaseFloatArrayElements(jfloatArray(bufferArray), reinterpret_cast<jfloat*>(m_data), 0);
                    m_data = nullptr;
                }else{
                    jfloat* array = reinterpret_cast<jfloat*>(m_data);
                    m_data = nullptr;
                    if(!m_readOnly){
                        jsize size = jsize(size_t(m_capacity) * sizeof(char) / sizeof(jfloat));
                        for(jsize i=0; i<size; ++i){
                            Java::Runtime::FloatBuffer::put(m_env, m_buffer_object, i, double(array[i]));
                        }
                    }
                    delete[] array;
                }
            }else if(Java::Runtime::DoubleBuffer::isInstanceOf(m_env, m_buffer_object)){
                if(bufferArray && m_env->IsInstanceOf(bufferArray, getArrayClass(m_env, Java::Runtime::Double::primitiveType(m_env), 1))){
                    m_env->ReleaseDoubleArrayElements(jdoubleArray(bufferArray), reinterpret_cast<jdouble*>(m_data), 0);
                    m_data = nullptr;
                }else{
                    jdouble* array = reinterpret_cast<jdouble*>(m_data);
                    m_data = nullptr;
                    if(!m_readOnly){
                        jsize size = jsize(size_t(m_capacity) * sizeof(char) / sizeof(jdouble));
                        for(jsize i=0; i<size; ++i){
                            Java::Runtime::DoubleBuffer::put(m_env, m_buffer_object, i, array[i]);
                        }
                    }
                    delete[] array;
                }
            }
        }
    } catch (const std::exception& e) {
        qCWarning(DebugAPI::internalCategory, "%s", e.what());
    } catch (...) {
    }
}

struct PersistentJBufferDataPrivate{
    JObjectWrapper m_buffer_object;
    qsizetype m_capacity;
    qsizetype m_position;
    qsizetype m_limit;
    char* m_data;
    bool m_isdirect;
    bool m_readOnly;
    jboolean m_is_copy;
};

PersistentJBufferConstData::PersistentJBufferConstData(JNIEnv *env, jobject buffer_object) :
    m_data(new PersistentJBufferDataPrivate{JObjectWrapper(env, buffer_object), 0, 0, 0, nullptr, true, false, false})
{
    if(buffer_object){
        m_data->m_isdirect = Java::Runtime::Buffer::isDirect(env, buffer_object);
        m_data->m_limit = Java::Runtime::Internal::Buffer::limit(env, buffer_object);
        m_data->m_position = Java::Runtime::Internal::Buffer::position(env, buffer_object);
        m_data->m_readOnly = Java::Runtime::Buffer::isReadOnly(env, buffer_object);
        if(m_data->m_isdirect){
            m_data->m_capacity = env->GetDirectBufferCapacity(buffer_object);
            m_data->m_data = reinterpret_cast<char*>(env->GetDirectBufferAddress(buffer_object));
        }else{
            jobject bufferArray = nullptr;
            try{
                bufferArray = getBufferArray(env, buffer_object);
            }catch(const JavaException&){
            }
            if(Java::Runtime::ByteBuffer::isInstanceOf(env, buffer_object)){
                if(bufferArray && env->IsInstanceOf(bufferArray, getArrayClass(env, Java::Runtime::Byte::primitiveType(env), 1))){
                    m_data->m_capacity = env->GetArrayLength(jarray(bufferArray));
                    if(m_data->m_capacity>0){
                        m_data->m_data = reinterpret_cast<char*>(env->GetByteArrayElements(jbyteArray(bufferArray), &m_data->m_is_copy));
                    }
                }else{
                    m_data->m_capacity = Java::Runtime::Internal::Buffer::capacity(env, buffer_object);
                    if(m_data->m_capacity>0){
                        jbyte* array = new jbyte[size_t(m_data->m_capacity)];
                        for(qsizetype i=0; i<m_data->m_capacity; ++i){
                            array[i] = Java::Runtime::ByteBuffer::get(env, buffer_object, jsize(i));
                        }
                        m_data->m_data = reinterpret_cast<char*>(array);
                        m_data->m_is_copy = true;
                    }
                }
            }else if(Java::Runtime::IntBuffer::isInstanceOf(env, buffer_object)){
                if(bufferArray && env->IsInstanceOf(bufferArray, getArrayClass(env, Java::Runtime::Integer::primitiveType(env), 1))){
                    m_data->m_capacity = env->GetArrayLength(jarray(bufferArray));
                    if(m_data->m_capacity>0){
                        m_data->m_data = reinterpret_cast<char*>(env->GetIntArrayElements(jintArray(bufferArray), &m_data->m_is_copy));
                    }
                }else{
                    m_data->m_capacity = Java::Runtime::Internal::Buffer::capacity(env, buffer_object);
                    if(m_data->m_capacity>0){
                        jint* array = new jint[size_t(m_data->m_capacity)];
                        for(qsizetype i=0; i<m_data->m_capacity; ++i){
                            array[i] = Java::Runtime::IntBuffer::get(env, buffer_object, jsize(i));
                        }
                        m_data->m_data = reinterpret_cast<char*>(array);
                        m_data->m_is_copy = true;
                    }
                }
            }else if(Java::Runtime::ShortBuffer::isInstanceOf(env, buffer_object)){
                if(bufferArray && env->IsInstanceOf(bufferArray, getArrayClass(env, Java::Runtime::Short::primitiveType(env), 1))){
                    m_data->m_capacity = env->GetArrayLength(jarray(bufferArray));
                    if(m_data->m_capacity>0){
                        m_data->m_data = reinterpret_cast<char*>(env->GetShortArrayElements(jshortArray(bufferArray), &m_data->m_is_copy));
                    }
                }else{
                    m_data->m_capacity = Java::Runtime::Internal::Buffer::capacity(env, buffer_object);
                    if(m_data->m_capacity>0){
                        jshort* array = new jshort[size_t(m_data->m_capacity)];
                        for(qsizetype i=0; i<m_data->m_capacity; ++i){
                            array[i] = Java::Runtime::ShortBuffer::get(env, buffer_object, jsize(i));
                        }
                        m_data->m_data = reinterpret_cast<char*>(array);
                        m_data->m_is_copy = true;
                    }
                }
            }else if(Java::Runtime::CharBuffer::isInstanceOf(env, buffer_object)){
                if(bufferArray && env->IsInstanceOf(bufferArray, getArrayClass(env, Java::Runtime::Character::primitiveType(env), 1))){
                    m_data->m_capacity = m_data->m_limit = env->GetArrayLength(jarray(bufferArray));
                    if(m_data->m_capacity>0){
                        m_data->m_data = reinterpret_cast<char*>(env->GetCharArrayElements(jcharArray(bufferArray), &m_data->m_is_copy));
                    }
                }else{
                    m_data->m_capacity = Java::Runtime::Internal::Buffer::capacity(env, buffer_object);
                    if(m_data->m_capacity>0){
                        jchar* array = new jchar[size_t(m_data->m_capacity)];
                        for(qsizetype i=0; i<m_data->m_capacity; ++i){
                            array[i] = Java::Runtime::CharBuffer::get(env, buffer_object, jsize(i));
                        }
                        m_data->m_data = reinterpret_cast<char*>(array);
                        m_data->m_is_copy = true;
                    }
                }
            }else if(Java::Runtime::LongBuffer::isInstanceOf(env, buffer_object)){
                if(bufferArray && env->IsInstanceOf(bufferArray, getArrayClass(env, Java::Runtime::Long::primitiveType(env), 1))){
                    m_data->m_capacity = m_data->m_limit = env->GetArrayLength(jarray(bufferArray));
                    if(m_data->m_capacity>0){
                        m_data->m_data = reinterpret_cast<char*>(env->GetLongArrayElements(jlongArray(bufferArray), &m_data->m_is_copy));
                    }
                }else{
                    m_data->m_capacity = Java::Runtime::Internal::Buffer::capacity(env, buffer_object);
                    if(m_data->m_capacity>0){
                        jlong* array = new jlong[size_t(m_data->m_capacity)];
                        for(qsizetype i=0; i<m_data->m_capacity; ++i){
                            array[i] = Java::Runtime::LongBuffer::get(env, buffer_object, jsize(i));
                        }
                        m_data->m_data = reinterpret_cast<char*>(array);
                        m_data->m_is_copy = true;
                    }
                }
            }else if(Java::Runtime::FloatBuffer::isInstanceOf(env, buffer_object)){
                if(bufferArray && env->IsInstanceOf(bufferArray, getArrayClass(env, Java::Runtime::Float::primitiveType(env), 1))){
                    m_data->m_capacity = m_data->m_limit = env->GetArrayLength(jarray(bufferArray));
                    if(m_data->m_capacity>0){
                        m_data->m_data = reinterpret_cast<char*>(env->GetFloatArrayElements(jfloatArray(bufferArray), &m_data->m_is_copy));
                    }
                }else{
                    m_data->m_capacity = Java::Runtime::Internal::Buffer::capacity(env, buffer_object);
                    if(m_data->m_capacity>0){
                        jfloat* array = new jfloat[size_t(m_data->m_capacity)];
                        for(qsizetype i=0; i<m_data->m_capacity; ++i){
                            array[i] = Java::Runtime::FloatBuffer::get(env, buffer_object, jsize(i));
                        }
                        m_data->m_data = reinterpret_cast<char*>(array);
                        m_data->m_is_copy = true;
                    }
                }
            }else if(Java::Runtime::DoubleBuffer::isInstanceOf(env, buffer_object)){
                if(bufferArray && env->IsInstanceOf(bufferArray, getArrayClass(env, Java::Runtime::Double::primitiveType(env), 1))){
                    m_data->m_capacity = m_data->m_limit = env->GetArrayLength(jarray(bufferArray));
                    if(m_data->m_capacity>0){
                        m_data->m_data = reinterpret_cast<char*>(env->GetDoubleArrayElements(jdoubleArray(bufferArray), &m_data->m_is_copy));
                    }
                }else{
                    m_data->m_capacity = Java::Runtime::Internal::Buffer::capacity(env, buffer_object);
                    if(m_data->m_capacity>0){
                        jdouble* array = new jdouble[size_t(m_data->m_capacity)];
                        for(qsizetype i=0; i<m_data->m_capacity; ++i){
                            array[i] = Java::Runtime::DoubleBuffer::get(env, buffer_object, jsize(i));
                        }
                        m_data->m_data = reinterpret_cast<char*>(array);
                        m_data->m_is_copy = true;
                    }
                }
            }
        }
        if(Java::Runtime::IntBuffer::isInstanceOf(env, buffer_object)){
            m_data->m_capacity = m_data->m_capacity * sizeof(jint) / sizeof(char);
            m_data->m_limit = m_data->m_limit * sizeof(jint) / sizeof(char);
            m_data->m_position = m_data->m_position * sizeof(jint) / sizeof(char);
        }else if(Java::Runtime::ShortBuffer::isInstanceOf(env, buffer_object)){
            m_data->m_capacity = m_data->m_capacity * sizeof(jshort) / sizeof(char);
            m_data->m_limit = m_data->m_limit * sizeof(jshort) / sizeof(char);
            m_data->m_position = m_data->m_position * sizeof(jshort) / sizeof(char);
        }else if(Java::Runtime::CharBuffer::isInstanceOf(env, buffer_object)){
            m_data->m_capacity = m_data->m_capacity * sizeof(jchar) / sizeof(char);
            m_data->m_limit = m_data->m_limit * sizeof(jchar) / sizeof(char);
            m_data->m_position = m_data->m_position * sizeof(jchar) / sizeof(char);
        }else if(Java::Runtime::LongBuffer::isInstanceOf(env, buffer_object)){
            m_data->m_capacity = m_data->m_capacity * sizeof(jlong) / sizeof(char);
            m_data->m_limit = m_data->m_limit * sizeof(jlong) / sizeof(char);
            m_data->m_position = m_data->m_position * sizeof(jlong) / sizeof(char);
        }else if(Java::Runtime::FloatBuffer::isInstanceOf(env, buffer_object)){
            m_data->m_capacity = jsize(size_t(m_data->m_capacity) * sizeof(jfloat) / sizeof(char));
            m_data->m_limit = m_data->m_limit * sizeof(jfloat) / sizeof(char);
            m_data->m_position = m_data->m_position * sizeof(jfloat) / sizeof(char);
        }else if(Java::Runtime::DoubleBuffer::isInstanceOf(env, buffer_object)){
            m_data->m_capacity = m_data->m_capacity * sizeof(jdouble) / sizeof(char);
            m_data->m_limit = m_data->m_limit * sizeof(jdouble) / sizeof(char);
            m_data->m_position = m_data->m_position * sizeof(jdouble) / sizeof(char);
        }
    }
}

const void* PersistentJBufferConstData::data() const {return m_data ? m_data->m_data+m_data->m_position : nullptr;}
const void* PersistentJBufferConstData::constData() const {return data();}
void PersistentJBufferConstData::commit(JNIEnv *){}

void PersistentJBufferData::commit(JNIEnv *env){
    if(m_data && m_data->m_buffer_object && !m_data->m_isdirect && m_data->m_capacity>0 && m_data->m_is_copy){
        jobject buffer_object = m_data->m_buffer_object.object(env);
        jobject bufferArray = getBufferArray(env, buffer_object);
        if(Java::Runtime::ByteBuffer::isInstanceOf(env, buffer_object)){
            if(bufferArray && env->IsInstanceOf(bufferArray, getArrayClass(env, Java::Runtime::Byte::primitiveType(env), 1))){
                env->ReleaseByteArrayElements(jbyteArray(bufferArray), reinterpret_cast<jbyte*>(m_data->m_data), 0);
                m_data->m_data = reinterpret_cast<char*>(env->GetByteArrayElements(jbyteArray(bufferArray), &m_data->m_is_copy));
            }else{
                jbyte* array = reinterpret_cast<jbyte*>(m_data->m_data);
                jsize size = jsize(size_t(m_data->m_capacity) * sizeof(char) / sizeof(jbyte));
                for(jsize i=0; i<size; ++i){
                    Java::Runtime::ByteBuffer::put(env, buffer_object, i, array[i]);
                }
            }
        }else if(Java::Runtime::IntBuffer::isInstanceOf(env, buffer_object)){
            if(bufferArray && env->IsInstanceOf(bufferArray, getArrayClass(env, Java::Runtime::Integer::primitiveType(env), 1))){
                env->ReleaseIntArrayElements(jintArray(bufferArray), reinterpret_cast<jint*>(m_data->m_data), 0);
                m_data->m_data = reinterpret_cast<char*>(env->GetIntArrayElements(jintArray(bufferArray), &m_data->m_is_copy));
            }else{
                jint* array = reinterpret_cast<jint*>(m_data->m_data);
                jsize size = jsize(size_t(m_data->m_capacity) * sizeof(char) / sizeof(jint));
                for(jsize i=0; i<size; ++i){
                    Java::Runtime::IntBuffer::put(env, buffer_object, i, array[i]);
                }
            }
        }else if(Java::Runtime::ShortBuffer::isInstanceOf(env, buffer_object)){
            if(bufferArray && env->IsInstanceOf(bufferArray, getArrayClass(env, Java::Runtime::Short::primitiveType(env), 1))){
                env->ReleaseShortArrayElements(jshortArray(bufferArray), reinterpret_cast<jshort*>(m_data->m_data), 0);
                m_data->m_data = reinterpret_cast<char*>(env->GetShortArrayElements(jshortArray(bufferArray), &m_data->m_is_copy));
            }else{
                jshort* array = reinterpret_cast<jshort*>(m_data->m_data);
                jsize size = jsize(size_t(m_data->m_capacity) * sizeof(char) / sizeof(jshort));
                for(jsize i=0; i<size; ++i){
                    Java::Runtime::ShortBuffer::put(env, buffer_object, i, array[i]);
                }
            }
        }else if(Java::Runtime::CharBuffer::isInstanceOf(env, buffer_object)){
            if(bufferArray && env->IsInstanceOf(bufferArray, getArrayClass(env, Java::Runtime::Character::primitiveType(env), 1))){
                env->ReleaseCharArrayElements(jcharArray(bufferArray), reinterpret_cast<jchar*>(m_data->m_data), 0);
                m_data->m_data = reinterpret_cast<char*>(env->GetCharArrayElements(jcharArray(bufferArray), &m_data->m_is_copy));
            }else{
                jchar* array = reinterpret_cast<jchar*>(m_data->m_data);
                jsize size = jsize(size_t(m_data->m_capacity) * sizeof(char) / sizeof(jchar));
                for(jsize i=0; i<size; ++i){
                    Java::Runtime::CharBuffer::put(env, buffer_object, i, array[i]);
                }
            }
        }else if(Java::Runtime::LongBuffer::isInstanceOf(env, buffer_object)){
            if(bufferArray && env->IsInstanceOf(bufferArray, getArrayClass(env, Java::Runtime::Long::primitiveType(env), 1))){
                env->ReleaseLongArrayElements(jlongArray(bufferArray), reinterpret_cast<jlong*>(m_data->m_data), 0);
                m_data->m_data = reinterpret_cast<char*>(env->GetLongArrayElements(jlongArray(bufferArray), &m_data->m_is_copy));
            }else{
                jlong* array = reinterpret_cast<jlong*>(m_data->m_data);
                m_data->m_data = nullptr;
                jsize size = jsize(size_t(m_data->m_capacity) * sizeof(char) / sizeof(jlong));
                for(jsize i=0; i<size; ++i){
                    Java::Runtime::LongBuffer::put(env, buffer_object, i, array[i]);
                }
            }
        }else if(Java::Runtime::FloatBuffer::isInstanceOf(env, buffer_object)){
            if(bufferArray && env->IsInstanceOf(bufferArray, getArrayClass(env, Java::Runtime::Float::primitiveType(env), 1))){
                env->ReleaseFloatArrayElements(jfloatArray(bufferArray), reinterpret_cast<jfloat*>(m_data->m_data), 0);
                m_data->m_data = reinterpret_cast<char*>(env->GetFloatArrayElements(jfloatArray(bufferArray), &m_data->m_is_copy));
            }else{
                jfloat* array = reinterpret_cast<jfloat*>(m_data->m_data);
                jsize size = jsize(size_t(m_data->m_capacity) * sizeof(char) / sizeof(jfloat));
                for(jsize i=0; i<size; ++i){
                    Java::Runtime::FloatBuffer::put(env, buffer_object, i, double(array[i]));
                }
            }
        }else if(Java::Runtime::DoubleBuffer::isInstanceOf(env, buffer_object)){
            if(bufferArray && env->IsInstanceOf(bufferArray, getArrayClass(env, Java::Runtime::Double::primitiveType(env), 1))){
                env->ReleaseDoubleArrayElements(jdoubleArray(bufferArray), reinterpret_cast<jdouble*>(m_data->m_data), 0);
                m_data->m_data = reinterpret_cast<char*>(env->GetDoubleArrayElements(jdoubleArray(bufferArray), &m_data->m_is_copy));
            }else{
                jdouble* array = reinterpret_cast<jdouble*>(m_data->m_data);
                jsize size = jsize(size_t(m_data->m_capacity) * sizeof(char) / sizeof(jdouble));
                for(jsize i=0; i<size; ++i){
                    Java::Runtime::DoubleBuffer::put(env, buffer_object, i, array[i]);
                }
            }
        }
    }
}
void* PersistentJBufferData::data() {return m_data ? m_data->m_data+m_data->m_position : nullptr;}

void* PersistentJBufferConstData::take(){
    if(m_data){
        void* result = m_data->m_data;
        if(!m_data->m_isdirect){
            char* array = new char[size_t(m_data->m_capacity)];
            memcpy(array, m_data->m_data, size_t(m_data->m_capacity));
            result = array;
        }
        m_data->m_data = nullptr;
        return result;
    }
    return nullptr;
}

PersistentJBufferConstData::~PersistentJBufferConstData(){
    try{
        if(m_data && m_data->m_buffer_object){
            if(DefaultJniEnvironment env{500}){
                clear(env);
            }
        }
    } catch (const std::exception& e) {
        qCWarning(DebugAPI::internalCategory, "%s", e.what());
    } catch (...) {
    }
}

void PersistentJBufferConstData::clear(JNIEnv *env){
    try{
        if(m_data){
            if(m_data->m_data && m_data->m_buffer_object && !m_data->m_isdirect && m_data->m_capacity>0){
                jobject buffer_object = m_data->m_buffer_object.object(env);
                jobject bufferArray = nullptr;
                try{
                    bufferArray = getBufferArray(env, buffer_object);
                }catch(const JavaException&){
                }
                if(Java::Runtime::ByteBuffer::isInstanceOf(env, buffer_object)){
                    if(bufferArray && env->IsInstanceOf(bufferArray, getArrayClass(env, Java::Runtime::Byte::primitiveType(env), 1))){
                        env->ReleaseByteArrayElements(jbyteArray(bufferArray), reinterpret_cast<jbyte*>(m_data->m_data), JNI_ABORT);
                    }else
                        delete[] reinterpret_cast<jbyte*>(m_data->m_data);
                }else if(Java::Runtime::IntBuffer::isInstanceOf(env, buffer_object)){
                    if(bufferArray && env->IsInstanceOf(bufferArray, getArrayClass(env, Java::Runtime::Integer::primitiveType(env), 1))){
                        env->ReleaseIntArrayElements(jintArray(bufferArray), reinterpret_cast<jint*>(m_data->m_data), JNI_ABORT);
                    }else
                        delete[] reinterpret_cast<jint*>(m_data->m_data);
                }else if(Java::Runtime::ShortBuffer::isInstanceOf(env, buffer_object)){
                    if(bufferArray && env->IsInstanceOf(bufferArray, getArrayClass(env, Java::Runtime::Short::primitiveType(env), 1))){
                        env->ReleaseShortArrayElements(jshortArray(bufferArray), reinterpret_cast<jshort*>(m_data->m_data), JNI_ABORT);
                    }else
                        delete[] reinterpret_cast<jshort*>(m_data->m_data);
                }else if(Java::Runtime::CharBuffer::isInstanceOf(env, buffer_object)){
                    if(bufferArray && env->IsInstanceOf(bufferArray, getArrayClass(env, Java::Runtime::Character::primitiveType(env), 1))){
                        env->ReleaseCharArrayElements(jcharArray(bufferArray), reinterpret_cast<jchar*>(m_data->m_data), JNI_ABORT);
                    }else
                        delete[] reinterpret_cast<jchar*>(m_data->m_data);
                }else if(Java::Runtime::LongBuffer::isInstanceOf(env, buffer_object)){
                    if(bufferArray && env->IsInstanceOf(bufferArray, getArrayClass(env, Java::Runtime::Long::primitiveType(env), 1))){
                        env->ReleaseLongArrayElements(jlongArray(bufferArray), reinterpret_cast<jlong*>(m_data->m_data), JNI_ABORT);
                    }else
                        delete[] reinterpret_cast<jlong*>(m_data->m_data);
                }else if(Java::Runtime::FloatBuffer::isInstanceOf(env, buffer_object)){
                    if(bufferArray && env->IsInstanceOf(bufferArray, getArrayClass(env, Java::Runtime::Float::primitiveType(env), 1))){
                        env->ReleaseFloatArrayElements(jfloatArray(bufferArray), reinterpret_cast<jfloat*>(m_data->m_data), JNI_ABORT);
                    }else
                        delete[] reinterpret_cast<jfloat*>(m_data->m_data);
                }else if(Java::Runtime::DoubleBuffer::isInstanceOf(env, buffer_object)){
                    if(bufferArray && env->IsInstanceOf(bufferArray, getArrayClass(env, Java::Runtime::Double::primitiveType(env), 1))){
                        env->ReleaseDoubleArrayElements(jdoubleArray(bufferArray), reinterpret_cast<jdouble*>(m_data->m_data), JNI_ABORT);
                    }else
                        delete[] reinterpret_cast<jdouble*>(m_data->m_data);
                }
            }
            m_data->m_buffer_object.clear(env);
        }
    } catch (const std::exception& e) {
        qCWarning(DebugAPI::internalCategory, "%s", e.what());
    } catch (...) {
    }
}

qsizetype PersistentJBufferConstData::size() const{
    return m_data ? m_data->m_limit - m_data->m_position : 0;
}

qsizetype PersistentJBufferConstData::capacity() const{
    return m_data ? m_data->m_capacity : 0;
}

qsizetype PersistentJBufferConstData::position() const{
    return m_data ? m_data->m_position : 0;
}

qsizetype PersistentJBufferConstData::limit() const{
    return m_data ? m_data->m_limit : 0;
}

bool PersistentJBufferConstData::isDirect() const {return m_data ? m_data->m_isdirect : false;}
bool PersistentJBufferConstData::isReadOnly() const {return m_data ? m_data->m_readOnly : false;}
bool PersistentJBufferConstData::isBuffering() const { return m_data ? m_data->m_is_copy : false; }

bool PersistentJBufferConstData::isBuffer(JNIEnv *env, jobject obj){
    return Java::Runtime::Buffer::isInstanceOf(env, obj);
}

bool PersistentJBufferConstData::isReadOnlyBuffer(JNIEnv *env, jobject buffer){
    return Java::Runtime::Buffer::isReadOnly(env, buffer);
}

PersistentJBufferData::PersistentJBufferData(JNIEnv *env, jobject buffer_object) :
    PersistentJBufferConstData(env, buffer_object)
{
    if(m_data->m_readOnly)
        JavaException::raiseIllegalArgumentException(env, "Cannot write on a read-only buffer" QTJAMBI_STACKTRACEINFO );
}

PersistentJBufferData::~PersistentJBufferData(){
    try{
        if(m_data && m_data->m_buffer_object && m_data->m_capacity>0){
            if(DefaultJniEnvironment env{500}){
                clear(env);
            }
        }
    } catch (const std::exception& e) {
        qCWarning(DebugAPI::internalCategory, "%s", e.what());
    } catch (...) {
    }
}

void PersistentJBufferData::clear(JNIEnv *env){
    try{
        if(m_data && m_data->m_buffer_object && !m_data->m_isdirect && m_data->m_capacity>0){
            jobject buffer_object = m_data->m_buffer_object.object(env);
            jobject bufferArray = nullptr;
            try{
                bufferArray = getBufferArray(env, buffer_object);
            }catch(const JavaException&){
            }
            if(Java::Runtime::ByteBuffer::isInstanceOf(env, buffer_object)){
                if(bufferArray && env->IsInstanceOf(bufferArray, getArrayClass(env, Java::Runtime::Byte::primitiveType(env), 1))){
                    env->ReleaseByteArrayElements(jbyteArray(bufferArray), reinterpret_cast<jbyte*>(m_data->m_data), 0);
                    m_data->m_data = nullptr;
                }else{
                    jbyte* array = reinterpret_cast<jbyte*>(m_data->m_data);
                    m_data->m_data = nullptr;
                    if(!m_data->m_readOnly){
                        jsize size = jsize(size_t(m_data->m_capacity) * sizeof(char) / sizeof(jbyte));
                        for(jsize i=0; i<size; ++i){
                            Java::Runtime::ByteBuffer::put(env, buffer_object, i, array[i]);
                        }
                    }
                    delete[] array;
                }
            }else if(Java::Runtime::IntBuffer::isInstanceOf(env, buffer_object)){
                if(bufferArray && env->IsInstanceOf(bufferArray, getArrayClass(env, Java::Runtime::Integer::primitiveType(env), 1))){
                    env->ReleaseIntArrayElements(jintArray(bufferArray), reinterpret_cast<jint*>(m_data->m_data), 0);
                    m_data->m_data = nullptr;
                }else{
                    jint* array = reinterpret_cast<jint*>(m_data->m_data);
                    m_data->m_data = nullptr;
                    if(!m_data->m_readOnly){
                        jsize size = jsize(size_t(m_data->m_capacity) * sizeof(char) / sizeof(jint));
                        for(jsize i=0; i<size; ++i){
                            Java::Runtime::IntBuffer::put(env, buffer_object, i, array[i]);
                        }
                    }
                    delete[] array;
                }
            }else if(Java::Runtime::ShortBuffer::isInstanceOf(env, buffer_object)){
                if(bufferArray && env->IsInstanceOf(bufferArray, getArrayClass(env, Java::Runtime::Short::primitiveType(env), 1))){
                    env->ReleaseShortArrayElements(jshortArray(bufferArray), reinterpret_cast<jshort*>(m_data->m_data), 0);
                    m_data->m_data = nullptr;
                }else{
                    jshort* array = reinterpret_cast<jshort*>(m_data->m_data);
                    m_data->m_data = nullptr;
                    if(!m_data->m_readOnly){
                        jsize size = jsize(size_t(m_data->m_capacity) * sizeof(char) / sizeof(jshort));
                        for(jsize i=0; i<size; ++i){
                            Java::Runtime::ShortBuffer::put(env, buffer_object, i, array[i]);
                        }
                    }
                    delete[] array;
                }
            }else if(Java::Runtime::CharBuffer::isInstanceOf(env, buffer_object)){
                if(bufferArray && env->IsInstanceOf(bufferArray, getArrayClass(env, Java::Runtime::Character::primitiveType(env), 1))){
                    env->ReleaseCharArrayElements(jcharArray(bufferArray), reinterpret_cast<jchar*>(m_data->m_data), 0);
                    m_data->m_data = nullptr;
                }else{
                    jchar* array = reinterpret_cast<jchar*>(m_data->m_data);
                    m_data->m_data = nullptr;
                    if(!m_data->m_readOnly){
                        jsize size = jsize(size_t(m_data->m_capacity) * sizeof(char) / sizeof(jchar));
                        for(jsize i=0; i<size; ++i){
                            Java::Runtime::CharBuffer::put(env, buffer_object, i, array[i]);
                        }
                    }
                    delete[] array;
                }
            }else if(Java::Runtime::LongBuffer::isInstanceOf(env, buffer_object)){
                if(bufferArray && env->IsInstanceOf(bufferArray, getArrayClass(env, Java::Runtime::Long::primitiveType(env), 1))){
                    env->ReleaseLongArrayElements(jlongArray(bufferArray), reinterpret_cast<jlong*>(m_data->m_data), 0);
                    m_data->m_data = nullptr;
                }else{
                    jlong* array = reinterpret_cast<jlong*>(m_data->m_data);
                    m_data->m_data = nullptr;
                    if(!m_data->m_readOnly){
                        jsize size = jsize(size_t(m_data->m_capacity) * sizeof(char) / sizeof(jlong));
                        for(jsize i=0; i<size; ++i){
                            Java::Runtime::LongBuffer::put(env, buffer_object, i, array[i]);
                        }
                    }
                    delete[] array;
                }
            }else if(Java::Runtime::FloatBuffer::isInstanceOf(env, buffer_object)){
                if(bufferArray && env->IsInstanceOf(bufferArray, getArrayClass(env, Java::Runtime::Float::primitiveType(env), 1))){
                    env->ReleaseFloatArrayElements(jfloatArray(bufferArray), reinterpret_cast<jfloat*>(m_data->m_data), 0);
                    m_data->m_data = nullptr;
                }else{
                    jfloat* array = reinterpret_cast<jfloat*>(m_data->m_data);
                    m_data->m_data = nullptr;
                    if(!m_data->m_readOnly){
                        jsize size = jsize(size_t(m_data->m_capacity) * sizeof(char) / sizeof(jfloat));
                        for(jsize i=0; i<size; ++i){
                            Java::Runtime::FloatBuffer::put(env, buffer_object, i, double(array[i]));
                        }
                    }
                    delete[] array;
                }
            }else if(Java::Runtime::DoubleBuffer::isInstanceOf(env, buffer_object)){
                if(bufferArray && env->IsInstanceOf(bufferArray, getArrayClass(env, Java::Runtime::Double::primitiveType(env), 1))){
                    env->ReleaseDoubleArrayElements(jdoubleArray(bufferArray), reinterpret_cast<jdouble*>(m_data->m_data), 0);
                    m_data->m_data = nullptr;
                }else{
                    jdouble* array = reinterpret_cast<jdouble*>(m_data->m_data);
                    m_data->m_data = nullptr;
                    if(!m_data->m_readOnly){
                        jsize size = jsize(size_t(m_data->m_capacity) * sizeof(char) / sizeof(jdouble));
                        for(jsize i=0; i<size; ++i){
                            Java::Runtime::DoubleBuffer::put(env, buffer_object, i, array[i]);
                        }
                    }
                    delete[] array;
                }
            }
        }
    } catch (const std::exception& e) {
        qCWarning(DebugAPI::internalCategory, "%s", e.what());
    } catch (...) {
    }
    PersistentJBufferConstData::clear(env);
}

void truncateBuffer(JNIEnv *env, jobject buffer);

class AbstractDataJBuffer{
public:
    static jobject createBuffer(JNIEnv *env, void* data, qsizetype capacity);
    static jobject createBuffer(JNIEnv *env, const void* data, qsizetype capacity);
    static jobject createBuffer(JNIEnv *env, char* data, qsizetype capacity);
    static jobject createBuffer(JNIEnv *env, const char* data, qsizetype capacity);
    static jobject createBuffer(JNIEnv *env, unsigned char* data, qsizetype capacity);
    static jobject createBuffer(JNIEnv *env, const unsigned char* data, qsizetype capacity);
    static jobject createBuffer(JNIEnv *env, qint16* data, qsizetype capacity);
    static jobject createBuffer(JNIEnv *env, const qint16* data, qsizetype capacity);
    static jobject createBuffer(JNIEnv *env, quint16* data, qsizetype capacity);
    static jobject createBuffer(JNIEnv *env, const quint16* data, qsizetype capacity);
    static jobject createBuffer(JNIEnv *env, qint32* data, qsizetype capacity);
    static jobject createBuffer(JNIEnv *env, const qint32* data, qsizetype capacity);
    static jobject createBuffer(JNIEnv *env, quint32* data, qsizetype capacity);
    static jobject createBuffer(JNIEnv *env, const quint32* data, qsizetype capacity);
    static jobject createBuffer(JNIEnv *env, qint64* data, qsizetype capacity);
    static jobject createBuffer(JNIEnv *env, const qint64* data, qsizetype capacity);
    static jobject createBuffer(JNIEnv *env, quint64* data, qsizetype capacity);
    static jobject createBuffer(JNIEnv *env, const quint64* data, qsizetype capacity);
    static jobject createBuffer(JNIEnv *env, float* data, qsizetype capacity);
    static jobject createBuffer(JNIEnv *env, const float* data, qsizetype capacity);
    static jobject createBuffer(JNIEnv *env, double* data, qsizetype capacity);
    static jobject createBuffer(JNIEnv *env, const double* data, qsizetype capacity);
    static jobject createBuffer(JNIEnv *env, QChar* data, qsizetype capacity);
    static jobject createBuffer(JNIEnv *env, const QChar* data, qsizetype capacity);
    static jobject createBuffer(JNIEnv *env, wchar_t* data, qsizetype capacity);
    static jobject createBuffer(JNIEnv *env, const wchar_t* data, qsizetype capacity);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    static jobject createBuffer(JNIEnv *env, char16_t* data, qsizetype capacity);
    static jobject createBuffer(JNIEnv *env, const char16_t* data, qsizetype capacity);
    static jobject createBuffer(JNIEnv *env, char32_t* data, qsizetype capacity);
    static jobject createBuffer(JNIEnv *env, const char32_t* data, qsizetype capacity);
#endif
};

jobject AbstractDataJBuffer::createBuffer(JNIEnv *env, void* data, qsizetype capacity) {
    jobject buffer_object;
    if(data){
        buffer_object = env->NewDirectByteBuffer(data, jlong(capacity));
    }else{
        buffer_object = Java::Runtime::Internal::ByteBuffer::allocate(env, 0);
    }
    return Java::Runtime::Internal::ByteBuffer::order(env, buffer_object, Java::Runtime::ByteOrder::nativeOrder(env));
}

jobject AbstractDataJBuffer::createBuffer(JNIEnv *env, const void* data, qsizetype capacity) {
    jobject buffer_object;
    if(data){
        buffer_object = env->NewDirectByteBuffer(const_cast<void*>(data), jlong(capacity));
    }else{
        buffer_object = Java::Runtime::Internal::ByteBuffer::allocate(env, 0);
    }
    buffer_object = Java::Runtime::ByteBuffer::asReadOnlyBuffer(env, buffer_object);
    return Java::Runtime::Internal::ByteBuffer::order(env, buffer_object, Java::Runtime::ByteOrder::nativeOrder(env));
}

jobject AbstractDataJBuffer::createBuffer(JNIEnv *env, char* data, qsizetype capacity)
{
    return createBuffer(env, reinterpret_cast<void*>(data), capacity);
}

jobject AbstractDataJBuffer::createBuffer(JNIEnv *env, const char* data, qsizetype capacity)
{
    return createBuffer(env, reinterpret_cast<const void*>(data), capacity);
}

jobject AbstractDataJBuffer::createBuffer(JNIEnv *env, unsigned char* data, qsizetype capacity)
{
    return createBuffer(env, reinterpret_cast<void*>(data), capacity);
}

jobject AbstractDataJBuffer::createBuffer(JNIEnv *env, const unsigned char* data, qsizetype capacity)
{
    return createBuffer(env, reinterpret_cast<const void*>(data), capacity);
}

jobject AbstractDataJBuffer::createBuffer(JNIEnv *env, qint16* data, qsizetype capacity)
{
    jobject buffer_object = createBuffer(env, reinterpret_cast<void*>(data), capacity*sizeof(jshort));
    if(buffer_object)
        buffer_object = Java::Runtime::ByteBuffer::asShortBuffer(env, buffer_object);
    return buffer_object;
}

jobject AbstractDataJBuffer::createBuffer(JNIEnv *env, const qint16* data, qsizetype capacity)
{
    jobject buffer_object = createBuffer(env, reinterpret_cast<const void*>(data), capacity*sizeof(jshort));
    if(buffer_object)
        buffer_object = Java::Runtime::ByteBuffer::asShortBuffer(env, buffer_object);
    return buffer_object;
}

jobject AbstractDataJBuffer::createBuffer(JNIEnv *env, quint16* data, qsizetype capacity)
{
    jobject buffer_object = createBuffer(env, reinterpret_cast<void*>(data), capacity*sizeof(jshort));
    if(buffer_object)
        buffer_object = Java::Runtime::ByteBuffer::asShortBuffer(env, buffer_object);
    return buffer_object;
}

jobject AbstractDataJBuffer::createBuffer(JNIEnv *env, const quint16* data, qsizetype capacity)
{
    jobject buffer_object = createBuffer(env, reinterpret_cast<const void*>(data), capacity*sizeof(jshort));
    if(buffer_object)
        buffer_object = Java::Runtime::ByteBuffer::asShortBuffer(env, buffer_object);
    return buffer_object;
}

jobject AbstractDataJBuffer::createBuffer(JNIEnv *env, qint32* data, qsizetype capacity)
{
    jobject buffer_object = createBuffer(env, reinterpret_cast<void*>(data), capacity*sizeof(jint));
    if(buffer_object)
        buffer_object = Java::Runtime::ByteBuffer::asIntBuffer(env, buffer_object);
    return buffer_object;
}

jobject AbstractDataJBuffer::createBuffer(JNIEnv *env, const qint32* data, qsizetype capacity)
{
    jobject buffer_object = createBuffer(env, reinterpret_cast<const void*>(data), capacity*sizeof(jint));
    if(buffer_object)
        buffer_object = Java::Runtime::ByteBuffer::asIntBuffer(env, buffer_object);
    return buffer_object;
}

jobject AbstractDataJBuffer::createBuffer(JNIEnv *env, quint32* data, qsizetype capacity)
{
    jobject buffer_object = createBuffer(env, reinterpret_cast<void*>(data), capacity*sizeof(jint));
    if(buffer_object)
        buffer_object = Java::Runtime::ByteBuffer::asIntBuffer(env, buffer_object);
    return buffer_object;
}

jobject AbstractDataJBuffer::createBuffer(JNIEnv *env, const quint32* data, qsizetype capacity)
{
    jobject buffer_object = createBuffer(env, reinterpret_cast<const void*>(data), capacity*sizeof(jint));
    if(buffer_object)
        buffer_object = Java::Runtime::ByteBuffer::asIntBuffer(env, buffer_object);
    return buffer_object;
}

jobject AbstractDataJBuffer::createBuffer(JNIEnv *env, qint64* data, qsizetype capacity)
{
    jobject buffer_object = createBuffer(env, reinterpret_cast<void*>(data), capacity*sizeof(jlong));
    if(buffer_object)
        buffer_object = Java::Runtime::ByteBuffer::asLongBuffer(env, buffer_object);
    return buffer_object;
}

jobject AbstractDataJBuffer::createBuffer(JNIEnv *env, const qint64* data, qsizetype capacity)
{
    jobject buffer_object = createBuffer(env, reinterpret_cast<const void*>(data), capacity*sizeof(jlong));
    if(buffer_object)
        buffer_object = Java::Runtime::ByteBuffer::asLongBuffer(env, buffer_object);
    return buffer_object;
}

jobject AbstractDataJBuffer::createBuffer(JNIEnv *env, quint64* data, qsizetype capacity)
{
    jobject buffer_object = createBuffer(env, reinterpret_cast<void*>(data), capacity*sizeof(jlong));
    if(buffer_object)
        buffer_object = Java::Runtime::ByteBuffer::asLongBuffer(env, buffer_object);
    return buffer_object;
}

jobject AbstractDataJBuffer::createBuffer(JNIEnv *env, const quint64* data, qsizetype capacity)
{
    jobject buffer_object = createBuffer(env, reinterpret_cast<const void*>(data), capacity*sizeof(jlong));
    if(buffer_object)
        buffer_object = Java::Runtime::ByteBuffer::asLongBuffer(env, buffer_object);
    return buffer_object;
}

jobject AbstractDataJBuffer::createBuffer(JNIEnv *env, float* data, qsizetype capacity)
{
    jobject buffer_object = createBuffer(env, reinterpret_cast<void*>(data), capacity*sizeof(jfloat));
    if(buffer_object)
        buffer_object = Java::Runtime::ByteBuffer::asFloatBuffer(env, buffer_object);
    return buffer_object;
}

jobject AbstractDataJBuffer::createBuffer(JNIEnv *env, const float* data, qsizetype capacity)
{
    jobject buffer_object = createBuffer(env, reinterpret_cast<const void*>(data), capacity*sizeof(jfloat));
    if(buffer_object)
        buffer_object = Java::Runtime::ByteBuffer::asFloatBuffer(env, buffer_object);
    return buffer_object;
}

jobject AbstractDataJBuffer::createBuffer(JNIEnv *env, double* data, qsizetype capacity)
{
    jobject buffer_object = createBuffer(env, reinterpret_cast<void*>(data), capacity*sizeof(jdouble));
    if(buffer_object)
        buffer_object = Java::Runtime::ByteBuffer::asDoubleBuffer(env, buffer_object);
    return buffer_object;
}

jobject AbstractDataJBuffer::createBuffer(JNIEnv *env, const double* data, qsizetype capacity)
{
    jobject buffer_object = createBuffer(env, reinterpret_cast<const void*>(data), capacity*sizeof(jdouble));
    if(buffer_object)
        buffer_object = Java::Runtime::ByteBuffer::asDoubleBuffer(env, buffer_object);
    return buffer_object;
}

jobject AbstractDataJBuffer::createBuffer(JNIEnv *env, QChar* data, qsizetype capacity)
{
    jobject buffer_object = createBuffer(env, reinterpret_cast<void*>(data), capacity*sizeof(jchar));
    if(buffer_object)
        buffer_object = Java::Runtime::ByteBuffer::asCharBuffer(env, buffer_object);
    return buffer_object;
}
jobject AbstractDataJBuffer::createBuffer(JNIEnv *env, const QChar* data, qsizetype capacity)
{
    jobject buffer_object = createBuffer(env, reinterpret_cast<const void*>(data), capacity*sizeof(jchar));
    if(buffer_object)
        buffer_object = Java::Runtime::ByteBuffer::asCharBuffer(env, buffer_object);
    return buffer_object;
}
jobject AbstractDataJBuffer::createBuffer(JNIEnv *env, wchar_t* data, qsizetype capacity)
{
    jobject buffer_object = createBuffer(env, reinterpret_cast<void*>(data), capacity*sizeof(jchar));
    if(buffer_object)
        buffer_object = Java::Runtime::ByteBuffer::asCharBuffer(env, buffer_object);
    return buffer_object;
}
jobject AbstractDataJBuffer::createBuffer(JNIEnv *env, const wchar_t* data, qsizetype capacity)
{
    jobject buffer_object = createBuffer(env, reinterpret_cast<const void*>(data), capacity*sizeof(jchar));
    if(buffer_object)
        buffer_object = Java::Runtime::ByteBuffer::asCharBuffer(env, buffer_object);
    return buffer_object;
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
jobject AbstractDataJBuffer::createBuffer(JNIEnv *env, char32_t* data, qsizetype capacity)
{
    jobject buffer_object = createBuffer(env, reinterpret_cast<void*>(data), capacity*sizeof(char32_t));
    if(buffer_object)
        buffer_object = Java::Runtime::ByteBuffer::asCharBuffer(env, buffer_object);
    return buffer_object;
}
jobject AbstractDataJBuffer::createBuffer(JNIEnv *env, const char32_t* data, qsizetype capacity)
{
    jobject buffer_object = createBuffer(env, reinterpret_cast<const void*>(data), capacity*sizeof(char32_t));
    if(buffer_object)
        buffer_object = Java::Runtime::ByteBuffer::asCharBuffer(env, buffer_object);
    return buffer_object;
}

jobject AbstractDataJBuffer::createBuffer(JNIEnv *env, char16_t* data, qsizetype capacity)
{
    jobject buffer_object = createBuffer(env, reinterpret_cast<void*>(data), capacity*sizeof(jchar));
    if(buffer_object)
        buffer_object = Java::Runtime::ByteBuffer::asCharBuffer(env, buffer_object);
    return buffer_object;
}
jobject AbstractDataJBuffer::createBuffer(JNIEnv *env, const char16_t* data, qsizetype capacity)
{
    jobject buffer_object = createBuffer(env, reinterpret_cast<const void*>(data), capacity*sizeof(jchar));
    if(buffer_object)
        buffer_object = Java::Runtime::ByteBuffer::asCharBuffer(env, buffer_object);
    return buffer_object;
}
#endif

void DataJBuffer::setLimit(JNIEnv *env, jobject buffer, jsize limit){
    if(buffer)
        Java::Runtime::Internal::Buffer::setLimit(env, buffer, limit);
}

DataJBuffer::DataJBuffer(JNIEnv *env, void* data, qsizetype capacity)
    : m_env(env), m_buffer_object(AbstractDataJBuffer::createBuffer(env, data, capacity))
{
}

DataJBuffer::DataJBuffer(JNIEnv *env, const void* data, qsizetype capacity)
    : m_env(env), m_buffer_object(AbstractDataJBuffer::createBuffer(env, data, capacity))
{
}

DataJBuffer::DataJBuffer(JNIEnv *env, char* data, qsizetype capacity)
    : m_env(env), m_buffer_object(AbstractDataJBuffer::createBuffer(env, data, capacity))
{
}

DataJBuffer::DataJBuffer(JNIEnv *env, const char* data, qsizetype capacity)
    : m_env(env), m_buffer_object(AbstractDataJBuffer::createBuffer(env, data, capacity))
{
}

DataJBuffer::DataJBuffer(JNIEnv *env, unsigned char* data, qsizetype capacity)
    : m_env(env), m_buffer_object(AbstractDataJBuffer::createBuffer(env, data, capacity))
{
}

DataJBuffer::DataJBuffer(JNIEnv *env, const unsigned char* data, qsizetype capacity)
    : m_env(env), m_buffer_object(AbstractDataJBuffer::createBuffer(env, data, capacity))
{
}

DataJBuffer::DataJBuffer(JNIEnv *env, qint16* data, qsizetype capacity)
    : m_env(env), m_buffer_object(AbstractDataJBuffer::createBuffer(env, data, capacity))
{
}

DataJBuffer::DataJBuffer(JNIEnv *env, const qint16* data, qsizetype capacity)
    : m_env(env), m_buffer_object(AbstractDataJBuffer::createBuffer(env, data, capacity))
{
}

DataJBuffer::DataJBuffer(JNIEnv *env, quint16* data, qsizetype capacity)
    : m_env(env), m_buffer_object(AbstractDataJBuffer::createBuffer(env, data, capacity))
{
}

DataJBuffer::DataJBuffer(JNIEnv *env, const quint16* data, qsizetype capacity)
    : m_env(env), m_buffer_object(AbstractDataJBuffer::createBuffer(env, data, capacity))
{
}

DataJBuffer::DataJBuffer(JNIEnv *env, qint32* data, qsizetype capacity)
    : m_env(env), m_buffer_object(AbstractDataJBuffer::createBuffer(env, data, capacity))
{
}

DataJBuffer::DataJBuffer(JNIEnv *env, const qint32* data, qsizetype capacity)
    : m_env(env), m_buffer_object(AbstractDataJBuffer::createBuffer(env, data, capacity))
{
}

DataJBuffer::DataJBuffer(JNIEnv *env, quint32* data, qsizetype capacity)
    : m_env(env), m_buffer_object(AbstractDataJBuffer::createBuffer(env, data, capacity))
{
}

DataJBuffer::DataJBuffer(JNIEnv *env, const quint32* data, qsizetype capacity)
    : m_env(env), m_buffer_object(AbstractDataJBuffer::createBuffer(env, data, capacity))
{
}

DataJBuffer::DataJBuffer(JNIEnv *env, qint64* data, qsizetype capacity)
    : m_env(env), m_buffer_object(AbstractDataJBuffer::createBuffer(env, data, capacity))
{
}

DataJBuffer::DataJBuffer(JNIEnv *env, const qint64* data, qsizetype capacity)
    : m_env(env), m_buffer_object(AbstractDataJBuffer::createBuffer(env, data, capacity))
{
}

DataJBuffer::DataJBuffer(JNIEnv *env, quint64* data, qsizetype capacity)
    : m_env(env), m_buffer_object(AbstractDataJBuffer::createBuffer(env, data, capacity))
{
}

DataJBuffer::DataJBuffer(JNIEnv *env, const quint64* data, qsizetype capacity)
    : m_env(env), m_buffer_object(AbstractDataJBuffer::createBuffer(env, data, capacity))
{
}

DataJBuffer::DataJBuffer(JNIEnv *env, float* data, qsizetype capacity)
    : m_env(env), m_buffer_object(AbstractDataJBuffer::createBuffer(env, data, capacity))
{
}

DataJBuffer::DataJBuffer(JNIEnv *env, const float* data, qsizetype capacity)
    : m_env(env), m_buffer_object(AbstractDataJBuffer::createBuffer(env, data, capacity))
{
}

DataJBuffer::DataJBuffer(JNIEnv *env, double* data, qsizetype capacity)
    : m_env(env), m_buffer_object(AbstractDataJBuffer::createBuffer(env, data, capacity))
{
}

DataJBuffer::DataJBuffer(JNIEnv *env, const double* data, qsizetype capacity)
    : m_env(env), m_buffer_object(AbstractDataJBuffer::createBuffer(env, data, capacity))
{
}

DataJBuffer::DataJBuffer(JNIEnv *env, QChar* data, qsizetype capacity)
    : m_env(env), m_buffer_object(AbstractDataJBuffer::createBuffer(env, data, capacity))
{
}
DataJBuffer::DataJBuffer(JNIEnv *env, const QChar* data, qsizetype capacity)
    : m_env(env), m_buffer_object(AbstractDataJBuffer::createBuffer(env, data, capacity))
{
}

DataJBuffer::DataJBuffer(JNIEnv *env, wchar_t* data, qsizetype capacity)
    : m_env(env), m_buffer_object(AbstractDataJBuffer::createBuffer(env, data, capacity))
{
}
DataJBuffer::DataJBuffer(JNIEnv *env, const wchar_t* data, qsizetype capacity)
    : m_env(env), m_buffer_object(AbstractDataJBuffer::createBuffer(env, data, capacity))
{
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
DataJBuffer::DataJBuffer(JNIEnv *env, char16_t* data, qsizetype capacity)
    : m_env(env), m_buffer_object(AbstractDataJBuffer::createBuffer(env, data, capacity))
{
}
DataJBuffer::DataJBuffer(JNIEnv *env, const char16_t* data, qsizetype capacity)
    : m_env(env), m_buffer_object(AbstractDataJBuffer::createBuffer(env, data, capacity))
{
}
DataJBuffer::DataJBuffer(JNIEnv *env, char32_t* data, qsizetype capacity)
    : m_env(env), m_buffer_object(AbstractDataJBuffer::createBuffer(env, data, capacity))
{
}
DataJBuffer::DataJBuffer(JNIEnv *env, const char32_t* data, qsizetype capacity)
    : m_env(env), m_buffer_object(AbstractDataJBuffer::createBuffer(env, data, capacity))
{
}
#endif

DataJBuffer::~DataJBuffer(){
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

void DataJBuffer::setLimit(jsize limit){
    setLimit(m_env, m_buffer_object, limit);
}

jobject DataJBuffer::buffer() const{
    return m_buffer_object;
}

jobject DataJBuffer::take(){
    jobject result = m_buffer_object;
    m_buffer_object = nullptr;
    return result;
}

struct PersistentDataJBufferPrivate{
    JObjectWrapper m_buffer;
};

PersistentDataJBuffer::PersistentDataJBuffer(JNIEnv *env, void* data, qsizetype capacity)
    : m_data(new PersistentDataJBufferPrivate{AbstractDataJBuffer::createBuffer(env, data, capacity)})
{
}

PersistentDataJBuffer::PersistentDataJBuffer(JNIEnv *env, const void* data, qsizetype capacity)
    : m_data(new PersistentDataJBufferPrivate{AbstractDataJBuffer::createBuffer(env, data, capacity)})
{
}

PersistentDataJBuffer::PersistentDataJBuffer(JNIEnv *env, char* data, qsizetype capacity)
    : m_data(new PersistentDataJBufferPrivate{AbstractDataJBuffer::createBuffer(env, data, capacity)})
{
}

PersistentDataJBuffer::PersistentDataJBuffer(JNIEnv *env, const char* data, qsizetype capacity)
    : m_data(new PersistentDataJBufferPrivate{AbstractDataJBuffer::createBuffer(env, data, capacity)})
{
}

PersistentDataJBuffer::PersistentDataJBuffer(JNIEnv *env, unsigned char* data, qsizetype capacity)
    : m_data(new PersistentDataJBufferPrivate{AbstractDataJBuffer::createBuffer(env, data, capacity)})
{
}

PersistentDataJBuffer::PersistentDataJBuffer(JNIEnv *env, const unsigned char* data, qsizetype capacity)
    : m_data(new PersistentDataJBufferPrivate{AbstractDataJBuffer::createBuffer(env, data, capacity)})
{
}

PersistentDataJBuffer::PersistentDataJBuffer(JNIEnv *env, qint16* data, qsizetype capacity)
    : m_data(new PersistentDataJBufferPrivate{AbstractDataJBuffer::createBuffer(env, data, capacity)})
{
}

PersistentDataJBuffer::PersistentDataJBuffer(JNIEnv *env, const qint16* data, qsizetype capacity)
    : m_data(new PersistentDataJBufferPrivate{AbstractDataJBuffer::createBuffer(env, data, capacity)})
{
}

PersistentDataJBuffer::PersistentDataJBuffer(JNIEnv *env, quint16* data, qsizetype capacity)
    : m_data(new PersistentDataJBufferPrivate{AbstractDataJBuffer::createBuffer(env, data, capacity)})
{
}

PersistentDataJBuffer::PersistentDataJBuffer(JNIEnv *env, const quint16* data, qsizetype capacity)
    : m_data(new PersistentDataJBufferPrivate{AbstractDataJBuffer::createBuffer(env, data, capacity)})
{
}

PersistentDataJBuffer::PersistentDataJBuffer(JNIEnv *env, qint32* data, qsizetype capacity)
    : m_data(new PersistentDataJBufferPrivate{AbstractDataJBuffer::createBuffer(env, data, capacity)})
{
}

PersistentDataJBuffer::PersistentDataJBuffer(JNIEnv *env, const qint32* data, qsizetype capacity)
    : m_data(new PersistentDataJBufferPrivate{AbstractDataJBuffer::createBuffer(env, data, capacity)})
{
}

PersistentDataJBuffer::PersistentDataJBuffer(JNIEnv *env, quint32* data, qsizetype capacity)
    : m_data(new PersistentDataJBufferPrivate{AbstractDataJBuffer::createBuffer(env, data, capacity)})
{
}

PersistentDataJBuffer::PersistentDataJBuffer(JNIEnv *env, const quint32* data, qsizetype capacity)
    : m_data(new PersistentDataJBufferPrivate{AbstractDataJBuffer::createBuffer(env, data, capacity)})
{
}

PersistentDataJBuffer::PersistentDataJBuffer(JNIEnv *env, qint64* data, qsizetype capacity)
    : m_data(new PersistentDataJBufferPrivate{AbstractDataJBuffer::createBuffer(env, data, capacity)})
{
}

PersistentDataJBuffer::PersistentDataJBuffer(JNIEnv *env, const qint64* data, qsizetype capacity)
    : m_data(new PersistentDataJBufferPrivate{AbstractDataJBuffer::createBuffer(env, data, capacity)})
{
}

PersistentDataJBuffer::PersistentDataJBuffer(JNIEnv *env, quint64* data, qsizetype capacity)
    : m_data(new PersistentDataJBufferPrivate{AbstractDataJBuffer::createBuffer(env, data, capacity)})
{
}

PersistentDataJBuffer::PersistentDataJBuffer(JNIEnv *env, const quint64* data, qsizetype capacity)
    : m_data(new PersistentDataJBufferPrivate{AbstractDataJBuffer::createBuffer(env, data, capacity)})
{
}

PersistentDataJBuffer::PersistentDataJBuffer(JNIEnv *env, float* data, qsizetype capacity)
    : m_data(new PersistentDataJBufferPrivate{AbstractDataJBuffer::createBuffer(env, data, capacity)})
{
}

PersistentDataJBuffer::PersistentDataJBuffer(JNIEnv *env, const float* data, qsizetype capacity)
    : m_data(new PersistentDataJBufferPrivate{AbstractDataJBuffer::createBuffer(env, data, capacity)})
{
}

PersistentDataJBuffer::PersistentDataJBuffer(JNIEnv *env, double* data, qsizetype capacity)
    : m_data(new PersistentDataJBufferPrivate{AbstractDataJBuffer::createBuffer(env, data, capacity)})
{
}

PersistentDataJBuffer::PersistentDataJBuffer(JNIEnv *env, const double* data, qsizetype capacity)
    : m_data(new PersistentDataJBufferPrivate{AbstractDataJBuffer::createBuffer(env, data, capacity)})
{
}

PersistentDataJBuffer::PersistentDataJBuffer(JNIEnv *env, QChar* data, qsizetype capacity)
    : m_data(new PersistentDataJBufferPrivate{AbstractDataJBuffer::createBuffer(env, data, capacity)})
{
}
PersistentDataJBuffer::PersistentDataJBuffer(JNIEnv *env, const QChar* data, qsizetype capacity)
    : m_data(new PersistentDataJBufferPrivate{AbstractDataJBuffer::createBuffer(env, data, capacity)})
{
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
PersistentDataJBuffer::PersistentDataJBuffer(JNIEnv *env, char16_t* data, qsizetype capacity)
    : m_data(new PersistentDataJBufferPrivate{AbstractDataJBuffer::createBuffer(env, data, capacity)})
{
}
PersistentDataJBuffer::PersistentDataJBuffer(JNIEnv *env, const char16_t* data, qsizetype capacity)
    : m_data(new PersistentDataJBufferPrivate{AbstractDataJBuffer::createBuffer(env, data, capacity)})
{
}
PersistentDataJBuffer::PersistentDataJBuffer(JNIEnv *env, char32_t* data, qsizetype capacity)
    : m_data(new PersistentDataJBufferPrivate{AbstractDataJBuffer::createBuffer(env, data, capacity)})
{
}
PersistentDataJBuffer::PersistentDataJBuffer(JNIEnv *env, const char32_t* data, qsizetype capacity)
    : m_data(new PersistentDataJBufferPrivate{AbstractDataJBuffer::createBuffer(env, data, capacity)})
{
}
#endif

PersistentDataJBuffer::~PersistentDataJBuffer(){
    if(Q_LIKELY(m_data) && m_data->m_buffer){
        if(DefaultJniEnvironment env{100}){
            try{
                truncateBuffer(env, m_data->m_buffer.object(env));
                m_data->m_buffer.clear(env);
            }catch(const JavaException& exn){
                exn.report(env);
            } catch (const std::exception& e) {
                qCWarning(DebugAPI::internalCategory, "%s", e.what());
            } catch (...) {
            }
        }
    }
}

void PersistentDataJBuffer::setLimit(JNIEnv *env, jsize limit){
    setLimit(env, m_data->m_buffer.object(env), limit);
}

jobject PersistentDataJBuffer::buffer(JNIEnv *env) const{
    return Q_LIKELY(m_data) ? m_data->m_buffer.object(env) : nullptr;
}

jobject PersistentDataJBuffer::take(JNIEnv *env){
    if(Q_LIKELY(m_data)){
        jobject result = m_data->m_buffer.object(env);
        m_data->m_buffer.clear(env);
        return result;
    }else return nullptr;
}

void PersistentDataJBuffer::clear(JNIEnv *env){
    if(Q_LIKELY(m_data)){
        truncateBuffer(env, m_data->m_buffer.object(env));
        m_data->m_buffer.clear(env);
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
