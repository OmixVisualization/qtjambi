/****************************************************************************
**
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

JBufferConstData::JBufferConstData(JNIEnv *env, jobject buffer_object, bool purgeOnDelete) :
    m_buffer_object(buffer_object ? env->NewGlobalRef(buffer_object) : nullptr),
    m_size(0),
    m_data(nullptr),
    m_purgeOnDelete(purgeOnDelete),
    m_isdirect(true)
{
    if(m_buffer_object){
        m_isdirect = Java::Runtime::Buffer::isDirect(env, m_buffer_object);
        if(m_isdirect){
            m_size = env->GetDirectBufferCapacity(m_buffer_object);
            m_data = env->GetDirectBufferAddress(m_buffer_object);
        }else{
            jobject bufferArray = nullptr;
            try{
                bufferArray = Java::Runtime::Buffer::array(env, m_buffer_object);
            }catch(const JavaException&){}
            if(Java::Runtime::ByteBuffer::isInstanceOf(env, m_buffer_object)){
                if(bufferArray && env->IsInstanceOf(bufferArray, getArrayClass(env, Java::Runtime::Byte::primitiveType(env), 1))){
                    m_size = env->GetArrayLength(jarray(bufferArray));
                    if(m_size>0){
                        if(m_purgeOnDelete){
                            m_data = env->GetByteArrayElements(jbyteArray(bufferArray), nullptr);
                        }else{
                            jbyte* array = new jbyte[size_t(m_size)];
                            env->GetByteArrayRegion(jbyteArray(bufferArray), 0, jsize(m_size), array);
                            m_data = array;
                        }
                    }
                }else{
                    m_size = Java::Runtime::Buffer::capacity(env, m_buffer_object);
                    if(m_size>0){
                        jbyte* array = new jbyte[size_t(m_size)];
                        for(jsize i=0; i<m_size; ++i){
                            array[i] = Java::Runtime::ByteBuffer::get(env, m_buffer_object, i);
                        }
                        m_data = array;
                    }
                }
                //m_size = jsize(size_t(m_size) * sizeof(jbyte) / sizeof(char));
            }else if(Java::Runtime::IntBuffer::isInstanceOf(env, m_buffer_object)){
                if(bufferArray && env->IsInstanceOf(bufferArray, getArrayClass(env, Java::Runtime::Integer::primitiveType(env), 1))){
                    m_size = env->GetArrayLength(jarray(bufferArray));
                    if(m_size>0){
                        if(m_purgeOnDelete){
                            m_data = env->GetIntArrayElements(jintArray(bufferArray), nullptr);
                        }else{
                            jint* array = new jint[size_t(m_size)];
                            env->GetIntArrayRegion(jintArray(bufferArray), 0, jsize(m_size), array);
                            m_data = array;
                        }
                    }
                }else{
                    m_size = Java::Runtime::Buffer::capacity(env, m_buffer_object);
                    if(m_size>0){
                        jint* array = new jint[size_t(m_size)];
                        for(jsize i=0; i<m_size; ++i){
                            array[i] = Java::Runtime::IntBuffer::get(env, m_buffer_object, i);
                        }
                        m_data = array;
                    }
                }
                m_size = jsize(size_t(m_size) * sizeof(jint) / sizeof(char));
            }else if(Java::Runtime::ShortBuffer::isInstanceOf(env, m_buffer_object)){
                if(bufferArray && env->IsInstanceOf(bufferArray, getArrayClass(env, Java::Runtime::Short::primitiveType(env), 1))){
                    m_size = env->GetArrayLength(jarray(bufferArray));
                    if(m_size>0){
                        if(m_purgeOnDelete){
                            m_data = env->GetShortArrayElements(jshortArray(bufferArray), nullptr);
                        }else{
                            jshort* array = new jshort[size_t(m_size)];
                            env->GetShortArrayRegion(jshortArray(bufferArray), 0, jsize(m_size), array);
                            m_data = array;
                        }
                    }
                }else{
                    m_size = Java::Runtime::Buffer::capacity(env, m_buffer_object);
                    if(m_size>0){
                        jshort* array = new jshort[size_t(m_size)];
                        for(jsize i=0; i<m_size; ++i){
                            array[i] = Java::Runtime::ShortBuffer::get(env, m_buffer_object, i);
                        }
                        m_data = array;
                    }
                }
                m_size = jsize(size_t(m_size) * sizeof(jshort) / sizeof(char));
            }else if(Java::Runtime::CharBuffer::isInstanceOf(env, m_buffer_object)){
                if(bufferArray && env->IsInstanceOf(bufferArray, getArrayClass(env, Java::Runtime::Character::primitiveType(env), 1))){
                    m_size = env->GetArrayLength(jarray(bufferArray));
                    if(m_size>0){
                        if(m_purgeOnDelete){
                            m_data = env->GetCharArrayElements(jcharArray(bufferArray), nullptr);
                        }else{
                            jchar* array = new jchar[size_t(m_size)];
                            env->GetCharArrayRegion(jcharArray(bufferArray), 0, jsize(m_size), array);
                            m_data = array;
                        }
                    }
                }else{
                    m_size = Java::Runtime::Buffer::capacity(env, m_buffer_object);
                    if(m_size>0){
                        jchar* array = new jchar[size_t(m_size)];
                        for(jsize i=0; i<m_size; ++i){
                            array[i] = Java::Runtime::CharBuffer::get(env, m_buffer_object, i);
                        }
                        m_data = array;
                    }
                }
                m_size = jsize(size_t(m_size) * sizeof(jchar) / sizeof(char));
            }else if(Java::Runtime::LongBuffer::isInstanceOf(env, m_buffer_object)){
                if(bufferArray && env->IsInstanceOf(bufferArray, getArrayClass(env, Java::Runtime::Long::primitiveType(env), 1))){
                    m_size = env->GetArrayLength(jarray(bufferArray));
                    if(m_size>0){
                        if(m_purgeOnDelete){
                            m_data = env->GetLongArrayElements(jlongArray(bufferArray), nullptr);
                        }else{
                            jlong* array = new jlong[size_t(m_size)];
                            env->GetLongArrayRegion(jlongArray(bufferArray), 0, jsize(m_size), array);
                            m_data = array;
                        }
                    }
                }else{
                    m_size = Java::Runtime::Buffer::capacity(env, m_buffer_object);
                    if(m_size>0){
                        jlong* array = new jlong[size_t(m_size)];
                        for(jsize i=0; i<m_size; ++i){
                            array[i] = Java::Runtime::LongBuffer::get(env, m_buffer_object, i);
                        }
                        m_data = array;
                    }
                }
                m_size = jsize(size_t(m_size) * sizeof(jlong) / sizeof(char));
            }else if(Java::Runtime::FloatBuffer::isInstanceOf(env, m_buffer_object)){
                if(bufferArray && env->IsInstanceOf(bufferArray, getArrayClass(env, Java::Runtime::Float::primitiveType(env), 1))){
                    m_size = env->GetArrayLength(jarray(bufferArray));
                    if(m_size>0){
                        if(m_purgeOnDelete){
                            m_data = env->GetFloatArrayElements(jfloatArray(bufferArray), nullptr);
                        }else{
                            jfloat* array = new jfloat[size_t(m_size)];
                            env->GetFloatArrayRegion(jfloatArray(bufferArray), 0, jsize(m_size), array);
                            m_data = array;
                        }
                    }
                }else{
                    m_size = Java::Runtime::Buffer::capacity(env, m_buffer_object);
                    if(m_size>0){
                        jfloat* array = new jfloat[size_t(m_size)];
                        for(jsize i=0; i<m_size; ++i){
                            array[i] = Java::Runtime::FloatBuffer::get(env, m_buffer_object, i);
                        }
                        m_data = array;
                    }
                }
                m_size = jsize(size_t(m_size) * sizeof(jfloat) / sizeof(char));
            }else if(Java::Runtime::DoubleBuffer::isInstanceOf(env, m_buffer_object)){
                if(bufferArray && env->IsInstanceOf(bufferArray, getArrayClass(env, Java::Runtime::Double::primitiveType(env), 1))){
                    m_size = env->GetArrayLength(jarray(bufferArray));
                    if(m_size>0){
                        if(m_purgeOnDelete){
                            m_data = env->GetDoubleArrayElements(jdoubleArray(bufferArray), nullptr);
                        }else{
                            jdouble* array = new jdouble[size_t(m_size)];
                            env->GetDoubleArrayRegion(jdoubleArray(bufferArray), 0, jsize(m_size), array);
                            m_data = array;
                        }
                    }
                }else{
                    m_size = Java::Runtime::Buffer::capacity(env, m_buffer_object);
                    if(m_size>0){
                        jdouble* array = new jdouble[size_t(m_size)];
                        for(jsize i=0; i<m_size; ++i){
                            array[i] = Java::Runtime::DoubleBuffer::get(env, m_buffer_object, i);
                        }
                        m_data = array;
                    }
                }
                m_size = jsize(size_t(m_size) * sizeof(jdouble) / sizeof(char));
            }
        }
    }
}

JBufferConstData::~JBufferConstData(){
    try{
        if(m_buffer_object && m_data){
            if(DefaultJniEnvironment env{500}){
                if(!m_isdirect && m_size>0 && m_purgeOnDelete){
                    jobject bufferArray = nullptr;
                    try{
                        bufferArray = Java::Runtime::Buffer::array(env, m_buffer_object);
                    }catch(const JavaException&){}
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
        qWarning("%s", e.what());
    } catch (...) {
    }
}

const void* JBufferConstData::data() const {
    return m_data;
}

const void* JBufferConstData::constData() const {
    return m_data;
}

qint64 JBufferConstData::size() const{
    return m_size;
}

JBufferData::JBufferData(JNIEnv *env, jobject buffer_object) :
    JBufferConstData(env, buffer_object, true)
{
}

JBufferData::~JBufferData(){
    try{
        if(m_buffer_object){
            if(DefaultJniEnvironment env{500}){
                jboolean isReadOnly = Java::Runtime::Buffer::isReadOnly(env, m_buffer_object);
                if(!m_isdirect && m_size>0){
                    jobject bufferArray = nullptr;
                    try{
                        bufferArray = Java::Runtime::Buffer::array(env, m_buffer_object);
                    }catch(const JavaException&){}
                    if(Java::Runtime::ByteBuffer::isInstanceOf(env, m_buffer_object)){
                        if(bufferArray && env->IsInstanceOf(bufferArray, getArrayClass(env, Java::Runtime::Byte::primitiveType(env), 1))){
                            env->ReleaseByteArrayElements(jbyteArray(bufferArray), reinterpret_cast<jbyte*>(m_data), 0);
                            m_data = nullptr;
                        }else{
                            jbyte* array = reinterpret_cast<jbyte*>(m_data);
                            m_data = nullptr;
                            if(!isReadOnly){
                                jsize size = jsize(size_t(m_size) * sizeof(char) / sizeof(jbyte));
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
                            if(!isReadOnly){
                                jsize size = jsize(size_t(m_size) * sizeof(char) / sizeof(jint));
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
                            if(!isReadOnly){
                                jsize size = jsize(size_t(m_size) * sizeof(char) / sizeof(jshort));
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
                            if(!isReadOnly){
                                jsize size = jsize(size_t(m_size) * sizeof(char) / sizeof(jchar));
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
                            if(!isReadOnly){
                                jsize size = jsize(size_t(m_size) * sizeof(char) / sizeof(jlong));
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
                            if(!isReadOnly){
                                jsize size = jsize(size_t(m_size) * sizeof(char) / sizeof(jfloat));
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
                            if(!isReadOnly){
                                jsize size = jsize(size_t(m_size) * sizeof(char) / sizeof(jdouble));
                                for(jsize i=0; i<size; ++i){
                                    Java::Runtime::DoubleBuffer::put(env, m_buffer_object, i, array[i]);
                                }
                            }
                            delete[] array;
                        }
                    }
                }
            }
        }
    } catch (const std::exception& e) {
        qWarning("%s", e.what());
    } catch (...) {
    }
}

void* JBufferData::data(){
    return m_data;
}


