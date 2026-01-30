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

#if !defined(QTJAMBI_JAVAARRAYS_H) && !defined(QTJAMBI_GENERATOR_RUNNING)
#define QTJAMBI_JAVAARRAYS_H

#include "jnienvironment.h"
#include "javaapi.h"
#include "typetests.h"

namespace QtJambiAPI{

QTJAMBI_EXPORT jobjectArray createObjectArray(JNIEnv *env, const char* componentClass, jsize size);
QTJAMBI_EXPORT jobjectArray createObjectArray(JNIEnv *env, const std::type_info& componentType, jsize size);
template<class Container>
Container createIterable(typename Container::const_iterator begin, typename Container::size_type size);

template<typename T, typename E>
inline jobjectArray toJObjectArray(JNIEnv *__jni_env, const char *className, const T& iterable, std::function<jobject(JNIEnv *,const E&)> convertFunction) {
    jsize length = jsize(iterable.size());
    jobjectArray out = createObjectArray(__jni_env, className, length);
    for (jsize i = 0; i < length; ++i) {
        __jni_env->SetObjectArrayElement(out, i, convertFunction(__jni_env, iterable.begin()[i]));
        JavaException::check(__jni_env QTJAMBI_STACKTRACEINFO );
    }
    return out;
}

QTJAMBI_EXPORT jobjectArray toJObjectArray(JNIEnv *__jni_env, const std::type_info& typeInfo, const void* iterable, jsize length, std::function<jobject(JNIEnv *,const void*,jsize)> convertFunction);

template<template<typename E> class T, typename E>
inline jobjectArray toJObjectArray(JNIEnv *__jni_env, const T<E>& iterable, jsize length, std::function<jobject(JNIEnv *,const T<E>&,jsize)> getFunction) {
    jobjectArray out = createObjectArray(__jni_env, typeid(E), length);
    for (jsize i = 0; i < length; ++i) {
        __jni_env->SetObjectArrayElement(out, i, getFunction(__jni_env, iterable, i));
        JavaException::check(__jni_env QTJAMBI_STACKTRACEINFO );
    }
    return out;
}

template<template<typename E> class T, typename E>
inline jobjectArray toJObjectArray(JNIEnv *__jni_env, const T<E>& iterable, jsize (*lengthFunction)(const T<E>&), std::function<jobject(JNIEnv *,const T<E>&,jsize)> getFunction) {
    jsize length = lengthFunction(iterable);
    jobjectArray out = createObjectArray(__jni_env, typeid(E), length);
    for (jsize i = 0; i < length; ++i) {
        __jni_env->SetObjectArrayElement(out, i, getFunction(__jni_env, iterable, i));
        JavaException::check(__jni_env QTJAMBI_STACKTRACEINFO );
    }
    return out;
}

template<typename T, typename E>
inline jobjectArray toJObjectArray(JNIEnv *__jni_env, const char *className, const T* iterable, std::function<jobject(JNIEnv *,const E&)> convertFunction) {
    jsize length = jsize(iterable->size());
    jobjectArray out = createObjectArray(__jni_env, className, length);
    for (jsize i = 0; i < length; ++i) {
        __jni_env->SetObjectArrayElement(out, i, convertFunction(__jni_env, iterable->begin()[i]));
        JavaException::check(__jni_env QTJAMBI_STACKTRACEINFO );
    }
    return out;
}

template<typename T>
inline jobjectArray toJObjectArray(JNIEnv *__jni_env, const char *className, const T* array, jsize length, std::function<jobject(JNIEnv *,const T&)> convertFunction) {
    jobjectArray out = createObjectArray(__jni_env, className, length);
    for (jsize i = 0; i < length; ++i) {
        __jni_env->SetObjectArrayElement(out, i, convertFunction(__jni_env, array[i]));
        JavaException::check(__jni_env QTJAMBI_STACKTRACEINFO );
    }
    return out;
}

template<typename T>
inline jobjectArray toJObjectArray(JNIEnv *__jni_env, const T* array, jsize length, std::function<jobject(JNIEnv *,const T&)> convertFunction) {
    jobjectArray out = createObjectArray(__jni_env, typeid(T), length);
    for (jsize i = 0; i < length; ++i) {
        __jni_env->SetObjectArrayElement(out, i, convertFunction(__jni_env, array[i]));
        JavaException::check(__jni_env QTJAMBI_STACKTRACEINFO );
    }
    return out;
}

}

template<typename JArray, typename CType>
class PointerArray{
public:
    inline JArray array() {return m_array;}
    inline JArray array() const {return const_cast<JArray>(m_array);}
    inline operator JArray(){return m_array;}
    inline operator JArray() const {return array();}
    inline operator void*(){return m_array;}
    inline operator void*() const {return array();}
    inline operator jobject(){return m_array;}
    inline operator jobject() const {return array();}
    inline operator jvalue() const {
        jvalue v;
        v.l = array();
        return v;
    }
    inline CType* pointer () const { return m_pointer; }
    inline jsize size() const {return m_size;}
    ~PointerArray();
    Q_DISABLE_COPY(PointerArray)
protected:
    PointerArray(JNIEnv *env, CType* pointer, JArray array, jsize size);
    CType* pointer () { return m_pointer; }
    JNIEnv *m_env;
    JArray m_array;
    jsize m_size;
    CType* m_pointer;
private:
    void* operator new(size_t) = delete;
    void* operator new(size_t,size_t) = delete;
    void* operator new[](size_t) = delete;
};

template<typename JArray, typename CType>
inline PointerArray<JArray, CType>::PointerArray(JNIEnv *env, CType* pointer, JArray array, jsize size)
    :
    m_env(env),
    m_array(array),
    m_size(jsize(size)),
    m_pointer(pointer)
{}

template<typename JArray, typename CType>
inline PointerArray<JArray, CType>::~PointerArray(){
}

class QTJAMBI_EXPORT AbstractPersistentPointerArray{
public:
    ~AbstractPersistentPointerArray();
protected:
    AbstractPersistentPointerArray(JNIEnv *env, jarray array, jsize size);
    jarray array() const;
    jarray array(JNIEnv *env) const;
    jsize size() const;
    operator bool() const;
    inline operator jobject() const {return array();}
    inline operator jvalue() const {
        jvalue v;
        v.l = array();
        return v;
    }
    QScopedPointer<struct PersistentPointerArrayPrivate> d;
};

template<typename JArray, typename CType>
class PersistentPointerArray : public AbstractPersistentPointerArray {
public:
    inline JArray array() const {return static_cast<JArray>(AbstractPersistentPointerArray::array());}
    inline operator JArray() const {return array();}
    inline operator void*() const {return const_cast<void*>(reinterpret_cast<const void*>(m_pointer));}
    inline operator CType*() const {return m_pointer;}
    inline CType* pointer () const { return m_pointer; }
    Q_DISABLE_COPY(PersistentPointerArray)
    inline JArray array(JNIEnv *env) const {return static_cast<JArray>(AbstractPersistentPointerArray::array(env));}
protected:
    PersistentPointerArray(JNIEnv *env, CType* pointer, JArray array, jsize size)
      : AbstractPersistentPointerArray(env, array, size),
        m_pointer(pointer){}
    CType* pointer () { return m_pointer; }
    CType* m_pointer;
};

class CharPointerArray : public PointerArray<jbyteArray,char>
{
public:
    QTJAMBI_EXPORT CharPointerArray(JNIEnv *env, char* pointer, jsize size);
    template<typename T>
    CharPointerArray(JNIEnv *env, T* pointer, std::enable_if_t<sizeof(T)==sizeof(char)
                                                                      && (std::is_integral_v<T>
                                                                          || std::is_same_v<T, QLatin1Char>
                                                                          || std::is_same_v<T, std::byte>), jsize> size)
        : CharPointerArray(env, reinterpret_cast<char*>(pointer), size) {}
    QTJAMBI_EXPORT ~CharPointerArray();
};

class Int8PointerArray : public PointerArray<jbyteArray,qint8>
{
public:
    QTJAMBI_EXPORT Int8PointerArray(JNIEnv *env, qint8* pointer, jsize size);
    template<typename T>
    Int8PointerArray(JNIEnv *env, T* pointer, std::enable_if_t<sizeof(T)==sizeof(qint8)
                                                        && (std::is_integral_v<T>), jsize> size)
        : Int8PointerArray(env, reinterpret_cast<qint8*>(pointer), size) {}
    QTJAMBI_EXPORT ~Int8PointerArray();
};

class Int16PointerArray : public PointerArray<jshortArray,qint16>
{
public:
    QTJAMBI_EXPORT Int16PointerArray(JNIEnv *env, qint16* pointer, jsize size);
    template<typename T>
    Int16PointerArray(JNIEnv *env, T* pointer, std::enable_if_t<sizeof(T)==sizeof(qint16)
                                                                  && (std::is_integral_v<T>), jsize> size)
        : Int16PointerArray(env, reinterpret_cast<qint16*>(pointer), size) {}
    QTJAMBI_EXPORT ~Int16PointerArray();
};

class Int32PointerArray : public PointerArray<jintArray,qint32>
{
public:
    QTJAMBI_EXPORT Int32PointerArray(JNIEnv *env, qint32* pointer, jsize size);
    template<typename T>
    Int32PointerArray(JNIEnv *env, T* pointer, std::enable_if_t<sizeof(T)==sizeof(qint32)
                                                        && (std::is_integral_v<T>), jsize> size)
        : Int32PointerArray(env, reinterpret_cast<qint32*>(pointer), size) {}
    QTJAMBI_EXPORT ~Int32PointerArray();
};

class Int64PointerArray : public PointerArray<jlongArray,qint64>
{
public:
    QTJAMBI_EXPORT Int64PointerArray(JNIEnv *env, qint64* pointer, jsize size);
    template<typename T>
    Int64PointerArray(JNIEnv *env, T* pointer, std::enable_if_t<sizeof(T)==sizeof(qint64)
                                                        && (std::is_integral_v<T>), jsize> size)
        : Int64PointerArray(env, reinterpret_cast<qint64*>(pointer), size) {}
    QTJAMBI_EXPORT ~Int64PointerArray();
};

class UCharPointerArray : public PointerArray<jbyteArray,uchar>
{
public:
    QTJAMBI_EXPORT UCharPointerArray(JNIEnv *env, uchar* pointer, jsize size);
    QTJAMBI_EXPORT ~UCharPointerArray();
};

class UInt8PointerArray : public PointerArray<jbyteArray,quint8>
{
public:
    QTJAMBI_EXPORT UInt8PointerArray(JNIEnv *env, quint8* pointer, jsize size);
    template<typename T>
    UInt8PointerArray(JNIEnv *env, T* pointer, std::enable_if_t<sizeof(T)==sizeof(quint8)
                                                         && (std::is_integral_v<T>), jsize> size)
        : UInt8PointerArray(env, reinterpret_cast<quint8*>(pointer), size) {}
    QTJAMBI_EXPORT ~UInt8PointerArray();
};

class UInt16PointerArray : public PointerArray<jshortArray,quint16>
{
public:
    QTJAMBI_EXPORT UInt16PointerArray(JNIEnv *env, quint16* pointer, jsize size);
    template<typename T>
    UInt16PointerArray(JNIEnv *env, T* pointer, std::enable_if_t<sizeof(T)==sizeof(quint16)
                                                                   && (std::is_integral_v<T>), jsize> size)
        : UInt16PointerArray(env, reinterpret_cast<quint16*>(pointer), size) {}
    QTJAMBI_EXPORT ~UInt16PointerArray();
};

class UInt32PointerArray : public PointerArray<jintArray,quint32>
{
public:
    QTJAMBI_EXPORT UInt32PointerArray(JNIEnv *env, quint32* pointer, jsize size);
    template<typename T>
    UInt32PointerArray(JNIEnv *env, T* pointer, std::enable_if_t<sizeof(T)==sizeof(quint32)
                                                         && (std::is_integral_v<T>), jsize> size)
        : UInt32PointerArray(env, reinterpret_cast<quint32*>(pointer), size) {}
    QTJAMBI_EXPORT ~UInt32PointerArray();
};

class UInt64PointerArray : public PointerArray<jlongArray,quint64>
{
public:
    QTJAMBI_EXPORT UInt64PointerArray(JNIEnv *env, quint64* pointer, jsize size);
    template<typename T>
    UInt64PointerArray(JNIEnv *env, T* pointer, std::enable_if_t<sizeof(T)==sizeof(quint64)
                                                         && (std::is_integral_v<T>), jsize> size)
        : UInt64PointerArray(env, reinterpret_cast<quint64*>(pointer), size) {}
    QTJAMBI_EXPORT ~UInt64PointerArray();
};

class BoolPointerArray : public PointerArray<jbooleanArray,bool>
{
public:
    QTJAMBI_EXPORT BoolPointerArray(JNIEnv *env, bool* pointer, jsize size);
    QTJAMBI_EXPORT ~BoolPointerArray();
};

class Bool2PointerArray : public PointerArray<jbooleanArray,uchar>
{
public:
    QTJAMBI_EXPORT Bool2PointerArray(JNIEnv *env, uchar* pointer, jsize size);
    QTJAMBI_EXPORT ~Bool2PointerArray();
};

class DoublePointerArray : public PointerArray<jdoubleArray,double>
{
public:
    QTJAMBI_EXPORT DoublePointerArray(JNIEnv *env, double* pointer, jsize size);
    QTJAMBI_EXPORT ~DoublePointerArray();
};

class FloatPointerArray : public PointerArray<jfloatArray,float>
{
public:
    QTJAMBI_EXPORT FloatPointerArray(JNIEnv *env, float* pointer, jsize size);
    QTJAMBI_EXPORT ~FloatPointerArray();
};

class WCharPointerArray : public PointerArray<jcharArray,ushort>
{
public:
    QTJAMBI_EXPORT WCharPointerArray(JNIEnv *env, ushort* pointer, jsize size);
    QTJAMBI_EXPORT ~WCharPointerArray();
};

class QCharPointerArray : public PointerArray<jcharArray,QChar>
{
public:
    QTJAMBI_EXPORT QCharPointerArray(JNIEnv *env, QChar* pointer, jsize size);
    QTJAMBI_EXPORT ~QCharPointerArray();
};

class ConstCharPointerArray : public PointerArray<jbyteArray,const char>
{
public:
    QTJAMBI_EXPORT ConstCharPointerArray(JNIEnv *env, const char* pointer, jsize size);

    template<typename T>
    ConstCharPointerArray(JNIEnv *env, const T* pointer, std::enable_if_t<sizeof(T)==sizeof(char)
                                                                      && (std::is_integral_v<T>
                                                                          || std::is_same_v<T, QLatin1Char>
                                                                          || std::is_same_v<T, std::byte>), jsize> size)
        : ConstCharPointerArray(env, reinterpret_cast<const char*>(pointer), size) {}
};

class ConstInt8PointerArray : public PointerArray<jbyteArray,const qint8>
{
public:
    QTJAMBI_EXPORT ConstInt8PointerArray(JNIEnv *env, const qint8* pointer, jsize size);
    template<typename T>
    ConstInt8PointerArray(JNIEnv *env, const T* pointer, std::enable_if_t<sizeof(T)==sizeof(qint8)
                                                             && (std::is_integral_v<T>), jsize> size)
        : ConstInt8PointerArray(env, reinterpret_cast<const qint8*>(pointer), size) {}
};

class ConstInt16PointerArray : public PointerArray<jshortArray,const qint16>
{
public:
    QTJAMBI_EXPORT ConstInt16PointerArray(JNIEnv *env, const qint16* pointer, jsize size);
    template<typename T>
    ConstInt16PointerArray(JNIEnv *env, const T* pointer, std::enable_if_t<sizeof(T)==sizeof(qint16)
                                                                  && (std::is_integral_v<T>), jsize> size)
        : ConstInt16PointerArray(env, reinterpret_cast<const qint16*>(pointer), size) {}
};

class ConstInt32PointerArray : public PointerArray<jintArray,const qint32>
{
public:
    QTJAMBI_EXPORT ConstInt32PointerArray(JNIEnv *env, const qint32* pointer, jsize size);
    template<typename T>
    ConstInt32PointerArray(JNIEnv *env, const T* pointer, std::enable_if_t<sizeof(T)==sizeof(qint32)
                                                             && (std::is_integral_v<T>), jsize> size)
        : ConstInt32PointerArray(env, reinterpret_cast<const qint32*>(pointer), size) {}
};

class ConstInt64PointerArray : public PointerArray<jlongArray,const qint64>
{
public:
    QTJAMBI_EXPORT ConstInt64PointerArray(JNIEnv *env, const qint64* pointer, jsize size);
    template<typename T>
    ConstInt64PointerArray(JNIEnv *env, const T* pointer, std::enable_if_t<sizeof(T)==sizeof(qint64)
                                                             && (std::is_integral_v<T>), jsize> size)
        : ConstInt64PointerArray(env, reinterpret_cast<const qint64*>(pointer), size) {}
};

class ConstUInt8PointerArray : public PointerArray<jbyteArray,const quint8>
{
public:
    QTJAMBI_EXPORT ConstUInt8PointerArray(JNIEnv *env, const quint8* pointer, jsize size);
    template<typename T>
    ConstUInt8PointerArray(JNIEnv *env, const T* pointer, std::enable_if_t<sizeof(T)==sizeof(quint8)
                                                             && (std::is_integral_v<T>), jsize> size)
        : ConstUInt8PointerArray(env, reinterpret_cast<const quint8*>(pointer), size) {}
};

class ConstUInt16PointerArray : public PointerArray<jshortArray,const quint16>
{
public:
    QTJAMBI_EXPORT ConstUInt16PointerArray(JNIEnv *env, const quint16* pointer, jsize size);
    template<typename T>
    ConstUInt16PointerArray(JNIEnv *env, const T* pointer, std::enable_if_t<sizeof(T)==sizeof(quint16)
                                                                   && (std::is_integral_v<T>), jsize> size)
        : ConstUInt16PointerArray(env, reinterpret_cast<const quint16*>(pointer), size) {}
};

class ConstUInt32PointerArray : public PointerArray<jintArray,const quint32>
{
public:
    QTJAMBI_EXPORT ConstUInt32PointerArray(JNIEnv *env, const quint32* pointer, jsize size);
    template<typename T>
    ConstUInt32PointerArray(JNIEnv *env, const T* pointer, std::enable_if_t<sizeof(T)==sizeof(quint32)
                                                              && (std::is_integral_v<T>), jsize> size)
        : ConstUInt32PointerArray(env, reinterpret_cast<const quint32*>(pointer), size) {}
};

class ConstUInt64PointerArray : public PointerArray<jlongArray,const quint64>
{
public:
    QTJAMBI_EXPORT ConstUInt64PointerArray(JNIEnv *env, const quint64* pointer, jsize size);
    template<typename T>
    ConstUInt64PointerArray(JNIEnv *env, const T* pointer, std::enable_if_t<sizeof(T)==sizeof(quint64)
                                                              && (std::is_integral_v<T>), jsize> size)
        : ConstUInt64PointerArray(env, reinterpret_cast<const quint64*>(pointer), size) {}
};

class ConstBoolPointerArray : public PointerArray<jbooleanArray,const bool>
{
public:
    QTJAMBI_EXPORT ConstBoolPointerArray(JNIEnv *env, const bool* pointer, jsize size);
};

class ConstBool2PointerArray : public PointerArray<jbooleanArray,const uchar>
{
public:
    QTJAMBI_EXPORT ConstBool2PointerArray(JNIEnv *env, const uchar* pointer, jsize size);
};

class ConstDoublePointerArray : public PointerArray<jdoubleArray,const double>
{
public:
    QTJAMBI_EXPORT ConstDoublePointerArray(JNIEnv *env, const double* pointer, jsize size);
};

class ConstFloatPointerArray : public PointerArray<jfloatArray,const float>
{
public:
    QTJAMBI_EXPORT ConstFloatPointerArray(JNIEnv *env, const float* pointer, jsize size);
};

class ConstWCharPointerArray : public PointerArray<jcharArray,const ushort>
{
public:
    QTJAMBI_EXPORT ConstWCharPointerArray(JNIEnv *env, const ushort* pointer, jsize size);
};

class ConstQCharPointerArray : public PointerArray<jcharArray,const QChar>
{
public:
    QTJAMBI_EXPORT ConstQCharPointerArray(JNIEnv *env, const QChar* pointer, jsize size);
};

class Char16PointerArray : public PointerArray<jcharArray,char16_t>
{
public:
    QTJAMBI_EXPORT Char16PointerArray(JNIEnv *env, char16_t* pointer, jsize size);
    QTJAMBI_EXPORT ~Char16PointerArray();
};

class ConstChar16PointerArray : public PointerArray<jcharArray,const char16_t>
{
public:
    QTJAMBI_EXPORT ConstChar16PointerArray(JNIEnv *env, const char16_t* pointer, jsize size);
};

class Char32PointerArray : public PointerArray<jintArray,char32_t>
{
public:
    QTJAMBI_EXPORT Char32PointerArray(JNIEnv *env, char32_t* pointer, jsize size);
    QTJAMBI_EXPORT ~Char32PointerArray();
};

class ConstChar32PointerArray : public PointerArray<jintArray,const char32_t>
{
public:
    QTJAMBI_EXPORT ConstChar32PointerArray(JNIEnv *env, const char32_t* pointer, jsize size);
};

class PersistentCharPointerArray : public PersistentPointerArray<jbyteArray,char>
{
public:
    QTJAMBI_EXPORT PersistentCharPointerArray(JNIEnv *env, char* pointer, jsize size);
    template<typename T>
    PersistentCharPointerArray(JNIEnv *env, T* pointer, std::enable_if_t<sizeof(T)==sizeof(char)
                                                                      && (std::is_integral_v<T>
                                                                          || std::is_same_v<T, QLatin1Char>
                                                                          || std::is_same_v<T, std::byte>), jsize> size)
        : PersistentCharPointerArray(env, reinterpret_cast<char*>(pointer), size) {}
    QTJAMBI_EXPORT ~PersistentCharPointerArray();
};

class PersistentInt8PointerArray : public PersistentPointerArray<jbyteArray,qint8>
{
public:
    QTJAMBI_EXPORT PersistentInt8PointerArray(JNIEnv *env, qint8* pointer, jsize size);
    template<typename T>
    PersistentInt8PointerArray(JNIEnv *env, T* pointer, std::enable_if_t<sizeof(T)==sizeof(qint8)
                                                                  && (std::is_integral_v<T>), jsize> size)
        : PersistentInt8PointerArray(env, reinterpret_cast<qint8*>(pointer), size) {}
    QTJAMBI_EXPORT ~PersistentInt8PointerArray();
};

class PersistentInt16PointerArray : public PersistentPointerArray<jshortArray,qint16>
{
public:
    QTJAMBI_EXPORT PersistentInt16PointerArray(JNIEnv *env, qint16* pointer, jsize size);
    template<typename T>
    PersistentInt16PointerArray(JNIEnv *env, T* pointer, std::enable_if_t<sizeof(T)==sizeof(qint16)
                                                                   && (std::is_integral_v<T>), jsize> size)
        : PersistentInt16PointerArray(env, reinterpret_cast<qint16*>(pointer), size) {}
    QTJAMBI_EXPORT ~PersistentInt16PointerArray();
};

class PersistentInt32PointerArray : public PersistentPointerArray<jintArray,qint32>
{
public:
    QTJAMBI_EXPORT PersistentInt32PointerArray(JNIEnv *env, qint32* pointer, jsize size);
    template<typename T>
    PersistentInt32PointerArray(JNIEnv *env, T* pointer, std::enable_if_t<sizeof(T)==sizeof(qint32)
                                                                  && (std::is_integral_v<T>), jsize> size)
        : PersistentInt32PointerArray(env, reinterpret_cast<qint32*>(pointer), size) {}
    QTJAMBI_EXPORT ~PersistentInt32PointerArray();
};

class PersistentInt64PointerArray : public PersistentPointerArray<jlongArray,qint64>
{
public:
    QTJAMBI_EXPORT PersistentInt64PointerArray(JNIEnv *env, qint64* pointer, jsize size);
    template<typename T>
    PersistentInt64PointerArray(JNIEnv *env, T* pointer, std::enable_if_t<sizeof(T)==sizeof(qint64)
                                                                  && (std::is_integral_v<T>), jsize> size)
        : PersistentInt64PointerArray(env, reinterpret_cast<qint64*>(pointer), size) {}
    QTJAMBI_EXPORT ~PersistentInt64PointerArray();
};

class PersistentUCharPointerArray : public PersistentPointerArray<jbyteArray,uchar>
{
public:
    QTJAMBI_EXPORT PersistentUCharPointerArray(JNIEnv *env, uchar* pointer, jsize size);
    QTJAMBI_EXPORT ~PersistentUCharPointerArray();
};

class PersistentUInt8PointerArray : public PersistentPointerArray<jbyteArray,quint8>
{
public:
    QTJAMBI_EXPORT PersistentUInt8PointerArray(JNIEnv *env, quint8* pointer, jsize size);
    template<typename T>
    PersistentUInt8PointerArray(JNIEnv *env, T* pointer, std::enable_if_t<sizeof(T)==sizeof(quint8)
                                                                   && (std::is_integral_v<T>), jsize> size)
        : PersistentUInt8PointerArray(env, reinterpret_cast<quint8*>(pointer), size) {}
    QTJAMBI_EXPORT ~PersistentUInt8PointerArray();
};

class PersistentUInt16PointerArray : public PersistentPointerArray<jshortArray,quint16>
{
public:
    QTJAMBI_EXPORT PersistentUInt16PointerArray(JNIEnv *env, quint16* pointer, jsize size);
    template<typename T>
    PersistentUInt16PointerArray(JNIEnv *env, T* pointer, std::enable_if_t<sizeof(T)==sizeof(quint16)
                                                                       && (std::is_integral_v<T>), jsize> size)
        : PersistentUInt16PointerArray(env, reinterpret_cast<quint16*>(pointer), size) {}
    QTJAMBI_EXPORT ~PersistentUInt16PointerArray();
};

class PersistentUInt32PointerArray : public PersistentPointerArray<jintArray,quint32>
{
public:
    QTJAMBI_EXPORT PersistentUInt32PointerArray(JNIEnv *env, quint32* pointer, jsize size);
    template<typename T>
    PersistentUInt32PointerArray(JNIEnv *env, T* pointer, std::enable_if_t<sizeof(T)==sizeof(quint32)
                                                                   && (std::is_integral_v<T>), jsize> size)
        : PersistentUInt32PointerArray(env, reinterpret_cast<quint32*>(pointer), size) {}
    QTJAMBI_EXPORT ~PersistentUInt32PointerArray();
};

class PersistentUInt64PointerArray : public PersistentPointerArray<jlongArray,quint64>
{
public:
    QTJAMBI_EXPORT PersistentUInt64PointerArray(JNIEnv *env, quint64* pointer, jsize size);
    template<typename T>
    PersistentUInt64PointerArray(JNIEnv *env, T* pointer, std::enable_if_t<sizeof(T)==sizeof(quint64)
                                                                   && (std::is_integral_v<T>), jsize> size)
        : PersistentUInt64PointerArray(env, reinterpret_cast<quint64*>(pointer), size) {}
    QTJAMBI_EXPORT ~PersistentUInt64PointerArray();
};

class PersistentBoolPointerArray : public PersistentPointerArray<jbooleanArray,bool>
{
public:
    QTJAMBI_EXPORT PersistentBoolPointerArray(JNIEnv *env, bool* pointer, jsize size);
    QTJAMBI_EXPORT ~PersistentBoolPointerArray();
};

class PersistentBool2PointerArray : public PersistentPointerArray<jbooleanArray,uchar>
{
public:
    QTJAMBI_EXPORT PersistentBool2PointerArray(JNIEnv *env, uchar* pointer, jsize size);
    QTJAMBI_EXPORT ~PersistentBool2PointerArray();
};

class PersistentDoublePointerArray : public PersistentPointerArray<jdoubleArray,double>
{
public:
    QTJAMBI_EXPORT PersistentDoublePointerArray(JNIEnv *env, double* pointer, jsize size);
    QTJAMBI_EXPORT ~PersistentDoublePointerArray();
};

class PersistentFloatPointerArray : public PersistentPointerArray<jfloatArray,float>
{
public:
    QTJAMBI_EXPORT PersistentFloatPointerArray(JNIEnv *env, float* pointer, jsize size);
    QTJAMBI_EXPORT ~PersistentFloatPointerArray();
};

class PersistentWCharPointerArray : public PersistentPointerArray<jcharArray,ushort>
{
public:
    QTJAMBI_EXPORT PersistentWCharPointerArray(JNIEnv *env, ushort* pointer, jsize size);
    QTJAMBI_EXPORT ~PersistentWCharPointerArray();
};

class PersistentQCharPointerArray : public PersistentPointerArray<jcharArray,QChar>
{
public:
    QTJAMBI_EXPORT PersistentQCharPointerArray(JNIEnv *env, QChar* pointer, jsize size);
    QTJAMBI_EXPORT ~PersistentQCharPointerArray();
};

class PersistentConstCharPointerArray : public PersistentPointerArray<jbyteArray,const char>
{
public:
    QTJAMBI_EXPORT PersistentConstCharPointerArray(JNIEnv *env, const char* pointer, jsize size);
    template<typename T>
    PersistentConstCharPointerArray(JNIEnv *env, const T* pointer, std::enable_if_t<sizeof(T)==sizeof(char)
                                                                                        && (std::is_integral_v<T>
                                                                                            || std::is_same_v<T, QLatin1Char>
                                                                                            || std::is_same_v<T, std::byte>), jsize> size)
        : PersistentConstCharPointerArray(env, reinterpret_cast<const char*>(pointer), size) {}
};

class PersistentConstInt8PointerArray : public PersistentPointerArray<jbyteArray,const qint8>
{
public:
    QTJAMBI_EXPORT PersistentConstInt8PointerArray(JNIEnv *env, const qint8* pointer, jsize size);
};

class PersistentConstInt16PointerArray : public PersistentPointerArray<jshortArray,const qint16>
{
public:
    QTJAMBI_EXPORT PersistentConstInt16PointerArray(JNIEnv *env, const qint16* pointer, jsize size);
    template<typename T>
    PersistentConstInt16PointerArray(JNIEnv *env, const T* pointer, std::enable_if_t<sizeof(T)==sizeof(qint16)
                                                                      && (std::is_integral_v<T>), jsize> size)
        : PersistentConstInt16PointerArray(env, reinterpret_cast<const qint16*>(pointer), size) {}
};

class PersistentConstInt32PointerArray : public PersistentPointerArray<jintArray,const qint32>
{
public:
    QTJAMBI_EXPORT PersistentConstInt32PointerArray(JNIEnv *env, const qint32* pointer, jsize size);
    template<typename T>
    PersistentConstInt32PointerArray(JNIEnv *env, const T* pointer, std::enable_if_t<sizeof(T)==sizeof(qint32)
                                                                       && (std::is_integral_v<T>), jsize> size)
        : PersistentConstInt32PointerArray(env, reinterpret_cast<const qint32*>(pointer), size) {}
};

class PersistentConstInt64PointerArray : public PersistentPointerArray<jlongArray,const qint64>
{
public:
    QTJAMBI_EXPORT PersistentConstInt64PointerArray(JNIEnv *env, const qint64* pointer, jsize size);
    template<typename T>
    PersistentConstInt64PointerArray(JNIEnv *env, const T* pointer, std::enable_if_t<sizeof(T)==sizeof(qint64)
                                                                       && (std::is_integral_v<T>), jsize> size)
        : PersistentConstInt64PointerArray(env, reinterpret_cast<const qint64*>(pointer), size) {}
};

class PersistentConstUInt8PointerArray : public PersistentPointerArray<jbyteArray,const quint8>
{
public:
    QTJAMBI_EXPORT PersistentConstUInt8PointerArray(JNIEnv *env, const quint8* pointer, jsize size);
    template<typename T>
    PersistentConstUInt8PointerArray(JNIEnv *env, const T* pointer, std::enable_if_t<sizeof(T)==sizeof(quint8)
                                                                        && (std::is_integral_v<T>), jsize> size)
        : PersistentConstUInt8PointerArray(env, reinterpret_cast<const quint8*>(pointer), size) {}
};

class PersistentConstUInt16PointerArray : public PersistentPointerArray<jshortArray,const quint16>
{
public:
    QTJAMBI_EXPORT PersistentConstUInt16PointerArray(JNIEnv *env, const quint16* pointer, jsize size);
    template<typename T>
    PersistentConstUInt16PointerArray(JNIEnv *env, const T* pointer, std::enable_if_t<sizeof(T)==sizeof(quint16)
                                                                       && (std::is_integral_v<T>), jsize> size)
        : PersistentConstUInt16PointerArray(env, reinterpret_cast<const quint16*>(pointer), size) {}
};

class PersistentConstUInt32PointerArray : public PersistentPointerArray<jintArray,const quint32>
{
public:
    QTJAMBI_EXPORT PersistentConstUInt32PointerArray(JNIEnv *env, const quint32* pointer, jsize size);
    template<typename T>
    PersistentConstUInt32PointerArray(JNIEnv *env, const T* pointer, std::enable_if_t<sizeof(T)==sizeof(quint32)
                                                                        && (std::is_integral_v<T>), jsize> size)
        : PersistentConstUInt32PointerArray(env, reinterpret_cast<const quint32*>(pointer), size) {}
};

class PersistentConstUInt64PointerArray : public PersistentPointerArray<jlongArray,const quint64>
{
public:
    QTJAMBI_EXPORT PersistentConstUInt64PointerArray(JNIEnv *env, const quint64* pointer, jsize size);
    template<typename T>
    PersistentConstUInt64PointerArray(JNIEnv *env, const T* pointer, std::enable_if_t<sizeof(T)==sizeof(quint64)
                                                                        && (std::is_integral_v<T>), jsize> size)
        : PersistentConstUInt64PointerArray(env, reinterpret_cast<const quint64*>(pointer), size) {}
};

class PersistentConstBoolPointerArray : public PersistentPointerArray<jbooleanArray,const bool>
{
public:
    QTJAMBI_EXPORT PersistentConstBoolPointerArray(JNIEnv *env, const bool* pointer, jsize size);
};

class PersistentConstBool2PointerArray : public PersistentPointerArray<jbooleanArray,const uchar>
{
public:
    QTJAMBI_EXPORT PersistentConstBool2PointerArray(JNIEnv *env, const uchar* pointer, jsize size);
};

class PersistentConstDoublePointerArray : public PersistentPointerArray<jdoubleArray,const double>
{
public:
    QTJAMBI_EXPORT PersistentConstDoublePointerArray(JNIEnv *env, const double* pointer, jsize size);
};

class PersistentConstFloatPointerArray : public PersistentPointerArray<jfloatArray,const float>
{
public:
    QTJAMBI_EXPORT PersistentConstFloatPointerArray(JNIEnv *env, const float* pointer, jsize size);
};

class PersistentConstWCharPointerArray : public PersistentPointerArray<jcharArray,const ushort>
{
public:
    QTJAMBI_EXPORT PersistentConstWCharPointerArray(JNIEnv *env, const ushort* pointer, jsize size);
};

class PersistentConstQCharPointerArray : public PersistentPointerArray<jcharArray,const QChar>
{
public:
    QTJAMBI_EXPORT PersistentConstQCharPointerArray(JNIEnv *env, const QChar* pointer, jsize size);
};

class PersistentChar16PointerArray : public PersistentPointerArray<jcharArray,char16_t>
{
public:
    QTJAMBI_EXPORT PersistentChar16PointerArray(JNIEnv *env, char16_t* pointer, jsize size);
    QTJAMBI_EXPORT ~PersistentChar16PointerArray();
};

class PersistentConstChar16PointerArray : public PersistentPointerArray<jcharArray,const char16_t>
{
public:
    QTJAMBI_EXPORT PersistentConstChar16PointerArray(JNIEnv *env, const char16_t* pointer, jsize size);
};

class PersistentChar32PointerArray : public PersistentPointerArray<jintArray,char32_t>
{
public:
    QTJAMBI_EXPORT PersistentChar32PointerArray(JNIEnv *env, char32_t* pointer, jsize size);
    QTJAMBI_EXPORT ~PersistentChar32PointerArray();
};

class PersistentConstChar32PointerArray : public PersistentPointerArray<jintArray,const char32_t>
{
public:
    QTJAMBI_EXPORT PersistentConstChar32PointerArray(JNIEnv *env, const char32_t* pointer, jsize size);
};

template<typename T>
class ObjectPointerArray : public PointerArray<jobjectArray,T>
{
public:
    ObjectPointerArray(JNIEnv *env, T* pointer, jsize _size,
                       const char* javaClass,
                       std::function<jobject(JNIEnv *,const T&)> getter,
                       std::function<void(T&,JNIEnv *,jobject)> setter);
    ObjectPointerArray(JNIEnv *env, T* pointer, jsize _size,
                       std::function<jobject(JNIEnv *,const T&)> getter,
                       std::function<void(T&,JNIEnv *,jobject)> setter);
    ~ObjectPointerArray();
private:
    using PointerArray<jobjectArray,T>::m_env;
    std::function<void(T&,JNIEnv *,jobject)> m_setter;
private:
    void* operator new(size_t) = delete;
    void* operator new(size_t,size_t) = delete;
    void* operator new[](size_t) = delete;
};

template<typename T>
inline ObjectPointerArray<T>::ObjectPointerArray(JNIEnv *env, T* pointer, jsize _size,
                   const char* javaClass,
                   std::function<jobject(JNIEnv *,const T&)> getter,
                   std::function<void(T&,JNIEnv *,jobject)> setter)
    : PointerArray<jobjectArray,T>(env, pointer, QtJambiAPI::createObjectArray(env, javaClass, pointer ? _size : 0), pointer ? _size : 0),
      m_setter(setter)
{
    if(pointer){
        JavaException::check(env QTJAMBI_STACKTRACEINFO );
        for(jsize i=0; i<PointerArray<jobjectArray,T>::size(); ++i){
            env->SetObjectArrayElement(PointerArray<jobjectArray,T>::array(), i, getter(env, pointer[i]));
        }
        JavaException::check(env QTJAMBI_STACKTRACEINFO );
    }
}

template<typename T>
inline ObjectPointerArray<T>::ObjectPointerArray(JNIEnv *env, T* pointer, jsize _size,
                   std::function<jobject(JNIEnv *,const T&)> getter,
                   std::function<void(T&,JNIEnv *,jobject)> setter)
    : PointerArray<jobjectArray,T>(env, pointer, QtJambiAPI::createObjectArray(env, typeid(typename std::remove_pointer<T>::type), pointer ? _size : 0), pointer ? _size : 0), m_setter(setter) {
    if(pointer){
        JavaException::check(env QTJAMBI_STACKTRACEINFO );
        for(jsize i=0; i<PointerArray<jobjectArray,T>::size(); ++i){
            env->SetObjectArrayElement(PointerArray<jobjectArray,T>::array(), i, getter(env, pointer[i]));
        }
        JavaException::check(env QTJAMBI_STACKTRACEINFO );
    }
}

template<typename T>
inline ObjectPointerArray<T>::~ObjectPointerArray(){
    if(PointerArray<jobjectArray,T>::array()){
        for(jsize i=0; i<PointerArray<jobjectArray,T>::size(); ++i){
            m_setter(PointerArray<jobjectArray,T>::pointer()[i], m_env, m_env->GetObjectArrayElement(PointerArray<jobjectArray,T>::array(), i));
        }
    }
}

template<typename T>
class ConstObjectPointerArray : public PointerArray<jobjectArray,const T>
{
public:
    ConstObjectPointerArray(JNIEnv *env, const T* pointer, jsize _size,
                            const char* javaClass,
                            std::function<jobject(JNIEnv *,const T&)> getter);
    ConstObjectPointerArray(JNIEnv *env, const T* pointer, jsize _size,
                            std::function<jobject(JNIEnv *,const T&)> getter);
private:
    void* operator new(size_t) = delete;
    void* operator new(size_t,size_t) = delete;
    void* operator new[](size_t) = delete;
};

template<typename T>
inline ConstObjectPointerArray<T>::ConstObjectPointerArray(JNIEnv *env, const T* pointer, jsize _size,
                        const char* javaClass,
                        std::function<jobject(JNIEnv *,const T&)> getter)
    : PointerArray<jobjectArray,const T>(env, pointer, QtJambiAPI::createObjectArray(env, javaClass, pointer ? _size : 0), pointer ? _size : 0)
{
    if(pointer){
        JavaException::check(env QTJAMBI_STACKTRACEINFO );
        for(jsize i=0; i<PointerArray<jobjectArray,const T>::size(); ++i){
            env->SetObjectArrayElement(PointerArray<jobjectArray,const T>::array(), i, getter(env, pointer[i]));
        }
        JavaException::check(env QTJAMBI_STACKTRACEINFO );
    }
}

template<typename T>
inline ConstObjectPointerArray<T>::ConstObjectPointerArray(JNIEnv *env, const T* pointer, jsize _size,
                        std::function<jobject(JNIEnv *,const T&)> getter)
    : PointerArray<jobjectArray,const T>(env, pointer, QtJambiAPI::createObjectArray(env, typeid(typename std::remove_pointer<T>::type), pointer ? _size : 0), pointer ? _size : 0)
{
    if(pointer){
        JavaException::check(env QTJAMBI_STACKTRACEINFO );
        for(jsize i=0; i<PointerArray<jobjectArray,const T>::size(); ++i){
            env->SetObjectArrayElement(PointerArray<jobjectArray,const T>::array(), i, getter(env, pointer[i]));
        }
        JavaException::check(env QTJAMBI_STACKTRACEINFO );
    }
}

template<typename T>
class PersistentObjectPointerArray : public PersistentPointerArray<jobjectArray,T>
{
public:
    PersistentObjectPointerArray(JNIEnv *env, T* pointer, jsize _size,
                       const char* javaClass,
                       std::function<jobject(JNIEnv *,const T&)> getter,
                       std::function<void(T&,JNIEnv *,jobject)> setter);
    PersistentObjectPointerArray(JNIEnv *env, T* pointer, jsize _size,
                       std::function<jobject(JNIEnv *,const T&)> getter,
                       std::function<void(T&,JNIEnv *,jobject)> setter);
    ~PersistentObjectPointerArray();
private:
    std::function<void(T&,JNIEnv *,jobject)> m_setter;
};

template<typename T>
inline PersistentObjectPointerArray<T>::PersistentObjectPointerArray(JNIEnv *env, T* pointer, jsize _size,
                                                 const char* javaClass,
                                                 std::function<jobject(JNIEnv *,const T&)> getter,
                                                 std::function<void(T&,JNIEnv *,jobject)> setter)
    : PersistentPointerArray<jobjectArray,T>(env, pointer, QtJambiAPI::createObjectArray(env, javaClass, pointer ? _size : 0), pointer ? _size : 0),
    m_setter(setter)
{
    if(pointer){
        JavaException::check(env QTJAMBI_STACKTRACEINFO );
        for(jsize i=0; i<PersistentPointerArray<jobjectArray,T>::size(); ++i){
            env->SetObjectArrayElement(PersistentPointerArray<jobjectArray,T>::array(), i, getter(env, pointer[i]));
        }
        JavaException::check(env QTJAMBI_STACKTRACEINFO );
    }
}

template<typename T>
inline PersistentObjectPointerArray<T>::PersistentObjectPointerArray(JNIEnv *env, T* pointer, jsize _size,
                                                 std::function<jobject(JNIEnv *,const T&)> getter,
                                                 std::function<void(T&,JNIEnv *,jobject)> setter)
    : PersistentPointerArray<jobjectArray,T>(env, pointer, QtJambiAPI::createObjectArray(env, typeid(typename std::remove_pointer<T>::type), pointer ? _size : 0), pointer ? _size : 0), m_setter(setter) {
    if(pointer){
        JavaException::check(env QTJAMBI_STACKTRACEINFO );
        for(jsize i=0; i<PersistentPointerArray<jobjectArray,T>::size(); ++i){
            env->SetObjectArrayElement(PersistentPointerArray<jobjectArray,T>::array(), i, getter(env, pointer[i]));
        }
        JavaException::check(env QTJAMBI_STACKTRACEINFO );
    }
}

template<typename T>
inline PersistentObjectPointerArray<T>::~PersistentObjectPointerArray(){
    if(PersistentPointerArray<jobjectArray,T>::array()){
        if(JniEnvironment env{300}){
            for(jsize i=0; i<PersistentPointerArray<jobjectArray,T>::size(); ++i){
                m_setter(PersistentPointerArray<jobjectArray,T>::pointer()[i], env, env->GetObjectArrayElement(PersistentPointerArray<jobjectArray,T>::array(), i));
            }
        }
    }
}

template<typename T>
class PersistentConstObjectPointerArray : public PersistentPointerArray<jobjectArray,const T>
{
public:
    PersistentConstObjectPointerArray(JNIEnv *env, const T* pointer, jsize _size,
                            const char* javaClass,
                            std::function<jobject(JNIEnv *,const T&)> getter);
    PersistentConstObjectPointerArray(JNIEnv *env, const T* pointer, jsize _size,
                            std::function<jobject(JNIEnv *,const T&)> getter);
};

template<typename T>
inline PersistentConstObjectPointerArray<T>::PersistentConstObjectPointerArray(JNIEnv *env, const T* pointer, jsize _size,
                                                           const char* javaClass,
                                                           std::function<jobject(JNIEnv *,const T&)> getter)
    : PersistentPointerArray<jobjectArray,const T>(env, pointer, QtJambiAPI::createObjectArray(env, javaClass, pointer ? _size : 0), pointer ? _size : 0)
{
    if(pointer){
        JavaException::check(env QTJAMBI_STACKTRACEINFO );
        for(jsize i=0; i<PersistentPointerArray<jobjectArray,const T>::size(); ++i){
            env->SetObjectArrayElement(PersistentPointerArray<jobjectArray,const T>::array(), i, getter(env, pointer[i]));
        }
        JavaException::check(env QTJAMBI_STACKTRACEINFO );
    }
}

template<typename T>
inline PersistentConstObjectPointerArray<T>::PersistentConstObjectPointerArray(JNIEnv *env, const T* pointer, jsize _size,
                                                           std::function<jobject(JNIEnv *,const T&)> getter)
    : PersistentPointerArray<jobjectArray,const T>(env, pointer, QtJambiAPI::createObjectArray(env, typeid(typename std::remove_pointer<T>::type), pointer ? _size : 0), pointer ? _size : 0)
{
    if(pointer){
        JavaException::check(env QTJAMBI_STACKTRACEINFO );
        for(jsize i=0; i<PersistentPointerArray<jobjectArray,const T>::size(); ++i){
            env->SetObjectArrayElement(PersistentPointerArray<jobjectArray,const T>::array(), i, getter(env, pointer[i]));
        }
        JavaException::check(env QTJAMBI_STACKTRACEINFO );
    }
}

namespace QtJambiPrivate {

template<typename JArray>
struct ElementForArray{

};

template<>
struct ElementForArray<jbyteArray>{
    typedef jbyte type;
    typedef Java::Runtime::Byte RuntimeType;
};

template<>
struct ElementForArray<jshortArray>{
    typedef jshort type;
    typedef Java::Runtime::Short RuntimeType;
};

template<>
struct ElementForArray<jintArray>{
    typedef jint type;
    typedef Java::Runtime::Integer RuntimeType;
};

template<>
struct ElementForArray<jlongArray>{
    typedef jlong type;
    typedef Java::Runtime::Long RuntimeType;
};

template<>
struct ElementForArray<jfloatArray>{
    typedef jfloat type;
    typedef Java::Runtime::Float RuntimeType;
};

template<>
struct ElementForArray<jdoubleArray>{
    typedef jdouble type;
    typedef Java::Runtime::Double RuntimeType;
};

template<>
struct ElementForArray<jcharArray>{
    typedef jchar type;
    typedef Java::Runtime::Character RuntimeType;
};

template<>
struct ElementForArray<jbooleanArray>{
    typedef bool type;
    typedef Java::Runtime::Boolean RuntimeType;
};

template<>
struct ElementForArray<jobjectArray>{
    typedef jobject type;
};

}

template<typename JArray, typename JType = typename QtJambiPrivate::ElementForArray<JArray>::type>
class JArrayPointer{
public:
    inline JArrayPointer(JNIEnv *env, JArray array)
        : m_array(array),
        m_size(array ? env->GetArrayLength(m_array) : 0),
        m_is_copy(false),
        m_array_elements(nullptr),
        m_env(env) {
        JavaException::check(env QTJAMBI_STACKTRACEINFO );
    }

    inline ~JArrayPointer(){}

    inline jsize size() const { return m_size; }
    inline jsize length() const { return m_size; }
    inline bool isBuffering() const { return m_is_copy; }
    void commit(){}
protected:
    typedef JArray JArrayType;
    typedef typename QtJambiPrivate::ElementForArray<JArrayType>::type ElementType;
    inline JArray array() const { return m_array; }
    JArray m_array;
    jsize m_size;
    jboolean m_is_copy;
    JType* m_array_elements;
    JNIEnv *m_env;
    Q_DISABLE_COPY(JArrayPointer)
};

class QTJAMBI_EXPORT AbstractPersistentJArrayPointer{
public:
    AbstractPersistentJArrayPointer(JNIEnv *env, jarray array);
    ~AbstractPersistentJArrayPointer();
    jsize size() const;
    jarray array() const;
protected:
    QScopedPointer<struct PersistentJArrayPointerPrivate> m_data;
};

template<typename JArray, typename JType = typename QtJambiPrivate::ElementForArray<JArray>::type>
class PersistentJArrayPointer : public AbstractPersistentJArrayPointer{
public:
    inline PersistentJArrayPointer(JNIEnv *env, JArray array)
        : AbstractPersistentJArrayPointer(env, array),
          m_is_copy(false),
          m_array_elements(nullptr) {
    }

    inline bool isBuffering() const { return m_is_copy; }
    void commit(JNIEnv *){}
    inline JArray array(JNIEnv *env) const { return static_cast<JArray>(env->NewLocalRef(AbstractPersistentJArrayPointer::array())); }
protected:
    typedef JArray JArrayType;
    typedef typename QtJambiPrivate::ElementForArray<JArrayType>::type ElementType;
    inline JArray array() const { return static_cast<JArray>(AbstractPersistentJArrayPointer::array()); }
    jboolean m_is_copy;
    JType* m_array_elements;
    Q_DISABLE_COPY(PersistentJArrayPointer)
};

#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)
#define QTJAMBI_POINTER_ARRAY_CONST_OPERATOR(type)\
QTJAMBI_EXPORT operator QSpan<const type> () const;\
QTJAMBI_EXPORT operator const type* () const;\
QTJAMBI_EXPORT operator std::initializer_list<type> () const;\
QTJAMBI_EXPORT operator std::initializer_list<const type> () const;
#define QTJAMBI_POINTER_ARRAY_OPERATOR(type)\
QTJAMBI_EXPORT operator QSpan<type> ();\
QTJAMBI_EXPORT operator type* ();
#else
#define QTJAMBI_POINTER_ARRAY_CONST_OPERATOR(type)\
QTJAMBI_EXPORT operator const type* () const;\
QTJAMBI_EXPORT operator std::initializer_list<type> () const;\
QTJAMBI_EXPORT operator std::initializer_list<const type> () const;
#define QTJAMBI_POINTER_ARRAY_OPERATOR(type)\
QTJAMBI_EXPORT operator type* ();
#endif

#define QTJAMBI_TYPED_ARRAY_POINTER(Type,jArray,const_operators,operators)\
class JConst##Type##ArrayPointer : public JArrayPointer<jArray>\
{\
public:\
    QTJAMBI_EXPORT JConst##Type##ArrayPointer(JNIEnv *env, jArray array);\
    QTJAMBI_EXPORT ~JConst##Type##ArrayPointer();\
    QTJAMBI_EXPORT const ElementType& operator[](int index) const;\
    QTJAMBI_EXPORT const ElementType* pointer() const;\
    const_operators\
    QTJAMBI_EXPORT static bool isValidArray(JNIEnv *env, jobject object);\
private:\
    using JArrayPointer<jArray>::JArrayType;\
    using JArrayPointer<jArray>::ElementType;\
    void* operator new(size_t) = delete;\
    void* operator new(size_t,size_t) = delete;\
    void* operator new[](size_t) = delete;\
};\
\
class J##Type##ArrayPointer : public JArrayPointer<jArray>\
{\
public:\
    QTJAMBI_EXPORT J##Type##ArrayPointer(JNIEnv *env, jArray array);\
    QTJAMBI_EXPORT ~J##Type##ArrayPointer();\
    QTJAMBI_EXPORT void commit();\
    QTJAMBI_EXPORT const ElementType& operator[](int index) const;\
    QTJAMBI_EXPORT ElementType& operator[](int index);\
    QTJAMBI_EXPORT ElementType* pointer();\
    QTJAMBI_EXPORT const ElementType* pointer() const;\
    const_operators\
    operators\
    QTJAMBI_EXPORT static bool isValidArray(JNIEnv *env, jobject object);\
private:\
    using JArrayPointer<jArray>::JArrayType;\
    using JArrayPointer<jArray>::ElementType;\
    void* operator new(size_t) = delete;\
    void* operator new(size_t,size_t) = delete;\
    void* operator new[](size_t) = delete;\
};\
\
class PersistentJConst##Type##ArrayPointer : public PersistentJArrayPointer<jArray>\
{\
public:\
    QTJAMBI_EXPORT PersistentJConst##Type##ArrayPointer(JNIEnv *env, jArray array);\
    QTJAMBI_EXPORT ~PersistentJConst##Type##ArrayPointer();\
    QTJAMBI_EXPORT const ElementType& operator[](int index) const;\
    QTJAMBI_EXPORT const ElementType* pointer() const;\
    const_operators\
    QTJAMBI_EXPORT static bool isValidArray(JNIEnv *env, jobject object);\
private:\
    using PersistentJArrayPointer<jArray>::JArrayType;\
    using PersistentJArrayPointer<jArray>::ElementType;\
};\
\
class PersistentJ##Type##ArrayPointer : public PersistentJArrayPointer<jArray>\
{\
public:\
    QTJAMBI_EXPORT PersistentJ##Type##ArrayPointer(JNIEnv *env, jArray array);\
    QTJAMBI_EXPORT ~PersistentJ##Type##ArrayPointer();\
    QTJAMBI_EXPORT void commit(JNIEnv *env);\
    QTJAMBI_EXPORT const ElementType& operator[](int index) const;\
    QTJAMBI_EXPORT ElementType& operator[](int index);\
    QTJAMBI_EXPORT ElementType* pointer();\
    QTJAMBI_EXPORT const ElementType* pointer() const;\
    const_operators\
    operators\
    QTJAMBI_EXPORT static bool isValidArray(JNIEnv *env, jobject object);\
private:\
    using PersistentJArrayPointer<jArray>::JArrayType;\
    using PersistentJArrayPointer<jArray>::ElementType;\
};

#define QTJAMBI_BYTEARRAY_OPERATOR() \
    QTJAMBI_EXPORT operator QByteArrayView() const;\
    QTJAMBI_EXPORT operator QByteArray() const;
#define QTJAMBI_STRING_OPERATOR() \
    QTJAMBI_EXPORT operator QStringView() const;\
    QTJAMBI_EXPORT operator QString() const;

QTJAMBI_TYPED_ARRAY_POINTER(Byte,
                            jbyteArray,
                            QTJAMBI_POINTER_ARRAY_CONST_OPERATOR(char)
                            QTJAMBI_POINTER_ARRAY_CONST_OPERATOR(qint8)
                            QTJAMBI_POINTER_ARRAY_CONST_OPERATOR(quint8)
                            MIN_QT6(QTJAMBI_POINTER_ARRAY_CONST_OPERATOR,std::byte)
                            QTJAMBI_BYTEARRAY_OPERATOR(),
                            QTJAMBI_POINTER_ARRAY_OPERATOR(char)
                            QTJAMBI_POINTER_ARRAY_OPERATOR(qint8)
                            QTJAMBI_POINTER_ARRAY_OPERATOR(quint8)
                            MIN_QT6(QTJAMBI_POINTER_ARRAY_OPERATOR,std::byte)
                            );

QTJAMBI_TYPED_ARRAY_POINTER(Int,
                            jintArray,
                            QTJAMBI_POINTER_ARRAY_CONST_OPERATOR(int)
                            QTJAMBI_POINTER_ARRAY_CONST_OPERATOR(uint)
                            MIN_QT6(QTJAMBI_POINTER_ARRAY_CONST_OPERATOR,char32_t),
                            QTJAMBI_POINTER_ARRAY_OPERATOR(int)
                            QTJAMBI_POINTER_ARRAY_OPERATOR(uint)
                            MIN_QT6(QTJAMBI_POINTER_ARRAY_OPERATOR,char32_t)
                            );

QTJAMBI_TYPED_ARRAY_POINTER(Long,
                            jlongArray,
                            QTJAMBI_POINTER_ARRAY_CONST_OPERATOR(qint64)
                            QTJAMBI_POINTER_ARRAY_CONST_OPERATOR(quint64),
                            QTJAMBI_POINTER_ARRAY_OPERATOR(qint64)
                            QTJAMBI_POINTER_ARRAY_OPERATOR(quint64)
                            );

QTJAMBI_TYPED_ARRAY_POINTER(Float,
                            jfloatArray,
                            QTJAMBI_POINTER_ARRAY_CONST_OPERATOR(float),
                            QTJAMBI_POINTER_ARRAY_OPERATOR(float)
                            );

QTJAMBI_TYPED_ARRAY_POINTER(Double,
                            jdoubleArray,
                            QTJAMBI_POINTER_ARRAY_CONST_OPERATOR(double),
                            QTJAMBI_POINTER_ARRAY_OPERATOR(double)
                            );

QTJAMBI_TYPED_ARRAY_POINTER(Short,
                            jshortArray,
                            QTJAMBI_POINTER_ARRAY_CONST_OPERATOR(qint16)
                            QTJAMBI_POINTER_ARRAY_CONST_OPERATOR(quint16),
                            QTJAMBI_POINTER_ARRAY_OPERATOR(qint16)
                            QTJAMBI_POINTER_ARRAY_OPERATOR(quint16)
                            );

QTJAMBI_TYPED_ARRAY_POINTER(Char,
                            jcharArray,
                            QTJAMBI_POINTER_ARRAY_CONST_OPERATOR(qint16)
                            QTJAMBI_POINTER_ARRAY_CONST_OPERATOR(quint16)
                            QTJAMBI_POINTER_ARRAY_CONST_OPERATOR(wchar_t)
                            QTJAMBI_POINTER_ARRAY_CONST_OPERATOR(QChar)
                            MIN_QT6(QTJAMBI_POINTER_ARRAY_CONST_OPERATOR,char16_t)
                            QTJAMBI_STRING_OPERATOR(),
                            QTJAMBI_POINTER_ARRAY_OPERATOR(qint16)
                            QTJAMBI_POINTER_ARRAY_OPERATOR(quint16)
                            QTJAMBI_POINTER_ARRAY_OPERATOR(wchar_t)
                            QTJAMBI_POINTER_ARRAY_OPERATOR(QChar)
                            MIN_QT6(QTJAMBI_POINTER_ARRAY_OPERATOR,char16_t)
                            );

#define QTJAMBI_BOOLEAN_ARRAY() \
    QTJAMBI_EXPORT jboolean* booleanArray();
#define QTJAMBI_CONST_BOOLEAN_ARRAY() \
    QTJAMBI_EXPORT const jboolean* booleanArray() const;

QTJAMBI_TYPED_ARRAY_POINTER(Boolean,
                            jbooleanArray,
                            QTJAMBI_POINTER_ARRAY_CONST_OPERATOR(bool)
                            QTJAMBI_CONST_BOOLEAN_ARRAY()
                            private: jboolean* m_boolean_array;
                            public:
                            ,
                            QTJAMBI_POINTER_ARRAY_OPERATOR(bool)
                            QTJAMBI_BOOLEAN_ARRAY()
                            );

#undef QTJAMBI_BOOLEAN_ARRAY
#undef QTJAMBI_CONST_BOOLEAN_ARRAY
#undef QTJAMBI_TYPED_ARRAY_POINTER
#undef QTJAMBI_STRING_OPERATOR
#undef QTJAMBI_BYTEARRAY_OPERATOR
#undef QTJAMBI_POINTER_ARRAY_OPERATOR
#undef QTJAMBI_POINTER_ARRAY_OPERATOR_QT6

template<class Type>
class JConstObjectArrayPointer : public JArrayPointer<jobjectArray, Type>
{
public:
    using JArrayPointer<jobjectArray, Type>::size;
    using JArrayPointer<jobjectArray, Type>::array;
    using JArrayPointer<jobjectArray, Type>::m_size;
    using JArrayPointer<jobjectArray, Type>::m_array;
    using JArrayPointer<jobjectArray, Type>::m_array_elements;
    using JArrayPointer<jobjectArray, Type>::m_is_copy;

    JConstObjectArrayPointer(JNIEnv *env, jobjectArray array, std::function<void(Type&,JNIEnv *,jobject)> setter)
        : JArrayPointer<jobjectArray, Type>(env, array)
    {
        if(m_array && m_size>0){
            m_is_copy = true;
            m_array_elements = new Type[m_size];
            for(int i=0; i<m_size; i++){
                setter(m_array_elements[i], env, env->GetObjectArrayElement(m_array, i));
            }
        }
    }

    ~JConstObjectArrayPointer() {
        if(m_array){
            delete[] m_array_elements;
        }
    }

    inline const Type* pointer() const {
        return m_array_elements;
    }

    inline operator const Type*() const {
        return m_array_elements;
    }

    inline const Type& operator[](jsize index) const{
        return m_array_elements[index];
    }

#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)
    QSpan<const Type> span() const { return m_array_elements ? QSpan<const Type>(m_array_elements, m_array_elements+m_size) : QSpan<const Type>(); }
    operator QSpan<const Type> () const { return span(); }
#endif
    operator std::initializer_list<Type> () const {
        return QtJambiAPI::createIterable<std::initializer_list<Type>>(m_array_elements, m_size);
    }
    operator std::initializer_list<const Type> () const {
        return QtJambiAPI::createIterable<std::initializer_list<const Type>>(m_array_elements, m_size);
    }
    void* operator new(size_t) = delete;
    void* operator new(size_t,size_t) = delete;
    void* operator new[](size_t) = delete;
};

template<>
class JConstObjectArrayPointer<jobject> : public JArrayPointer<jobjectArray, jobject>
{
public:
    using JArrayPointer<jobjectArray, jobject>::size;
    using JArrayPointer<jobjectArray, jobject>::array;
    using JArrayPointer<jobjectArray, jobject>::m_size;
    using JArrayPointer<jobjectArray, jobject>::m_array;
    using JArrayPointer<jobjectArray, jobject>::m_array_elements;
    using JArrayPointer<jobjectArray, jobject>::m_is_copy;

    JConstObjectArrayPointer(JNIEnv *env, jobjectArray array)
        : JArrayPointer<jobjectArray, jobject>(env, array)
    {
        if(m_array && m_size>0){
            m_is_copy = true;
        }
    }

    JConstObjectArrayPointer(JNIEnv *env, jobjectArray array, std::function<void(jobject&,JNIEnv *,jobject)>)
        : JConstObjectArrayPointer(env, array)
    {
    }

    ~JConstObjectArrayPointer() {
    }

    inline jobject operator[](jsize index) const{
        return m_env->GetObjectArrayElement(m_array, index);
    }
    void* operator new(size_t) = delete;
    void* operator new(size_t,size_t) = delete;
    void* operator new[](size_t) = delete;
};

template<>
class JConstObjectArrayPointer<jclass> : public JArrayPointer<jobjectArray, jclass>
{
public:
    using JArrayPointer<jobjectArray, jclass>::size;
    using JArrayPointer<jobjectArray, jclass>::array;
    using JArrayPointer<jobjectArray, jclass>::m_size;
    using JArrayPointer<jobjectArray, jclass>::m_array;
    using JArrayPointer<jobjectArray, jclass>::m_array_elements;
    using JArrayPointer<jobjectArray, jclass>::m_is_copy;

    JConstObjectArrayPointer(JNIEnv *env, jobjectArray array)
        : JArrayPointer<jobjectArray, jclass>(env, array)
    {
        if(m_array && m_size>0){
            m_is_copy = true;
        }
    }

    JConstObjectArrayPointer(JNIEnv *env, jobjectArray array, std::function<void(jclass&,JNIEnv *,jobject)>)
        : JConstObjectArrayPointer(env, array)
    {
    }

    ~JConstObjectArrayPointer() {
    }

    inline jclass operator[](jsize index) const{
        return jclass(m_env->GetObjectArrayElement(m_array, index));
    }
    void* operator new(size_t) = delete;
    void* operator new(size_t,size_t) = delete;
    void* operator new[](size_t) = delete;
};

template<>
class JConstObjectArrayPointer<jobjectArray> : public JArrayPointer<jobjectArray, jobjectArray>
{
public:
    using JArrayPointer<jobjectArray, jobjectArray>::size;
    using JArrayPointer<jobjectArray, jobjectArray>::array;
    using JArrayPointer<jobjectArray, jobjectArray>::m_size;
    using JArrayPointer<jobjectArray, jobjectArray>::m_array;
    using JArrayPointer<jobjectArray, jobjectArray>::m_array_elements;
    using JArrayPointer<jobjectArray, jobjectArray>::m_is_copy;

    JConstObjectArrayPointer(JNIEnv *env, jobjectArray array)
        : JArrayPointer<jobjectArray, jobjectArray>(env, array)
    {
        if(m_array && m_size>0){
            m_is_copy = true;
        }
    }

    JConstObjectArrayPointer(JNIEnv *env, jobjectArray array, std::function<void(jobjectArray&,JNIEnv *,jobject)>)
        : JConstObjectArrayPointer(env, array)
    {
    }

    ~JConstObjectArrayPointer() {
    }

    inline jobjectArray operator[](jsize index) const{
        return jobjectArray(m_env->GetObjectArrayElement(m_array, index));
    }
    void* operator new(size_t) = delete;
    void* operator new(size_t,size_t) = delete;
    void* operator new[](size_t) = delete;
};

template<>
class JConstObjectArrayPointer<jstring> : public JArrayPointer<jobjectArray, jstring>
{
public:
    using JArrayPointer<jobjectArray, jstring>::size;
    using JArrayPointer<jobjectArray, jstring>::array;
    using JArrayPointer<jobjectArray, jstring>::m_size;
    using JArrayPointer<jobjectArray, jstring>::m_array;
    using JArrayPointer<jobjectArray, jstring>::m_array_elements;
    using JArrayPointer<jobjectArray, jstring>::m_is_copy;

    JConstObjectArrayPointer(JNIEnv *env, jobjectArray array)
        : JArrayPointer<jobjectArray, jstring>(env, array)
    {
        if(m_array && m_size>0){
            m_is_copy = true;
        }
    }

    JConstObjectArrayPointer(JNIEnv *env, jobjectArray array, std::function<void(jstring&,JNIEnv *,jobject)>)
        : JConstObjectArrayPointer(env, array)
    {
    }

    ~JConstObjectArrayPointer() {
    }

    inline jstring operator[](jsize index) const{
        return jstring(m_env->GetObjectArrayElement(m_array, index));
    }
    void* operator new(size_t) = delete;
    void* operator new(size_t,size_t) = delete;
    void* operator new[](size_t) = delete;
};

template<class Type>
class JObjectArrayPointer : public JArrayPointer<jobjectArray, Type>
{
public:
    using JArrayPointer<jobjectArray, Type>::size;
    using JArrayPointer<jobjectArray, Type>::array;
    using JArrayPointer<jobjectArray, Type>::m_size;
    using JArrayPointer<jobjectArray, Type>::m_array;
    using JArrayPointer<jobjectArray, Type>::m_array_elements;
    using JArrayPointer<jobjectArray, Type>::m_is_copy;
    using JArrayPointer<jobjectArray, Type>::m_env;

    JObjectArrayPointer(JNIEnv *env, jobjectArray array, std::function<void(Type&,JNIEnv *,jobject)> setter, std::function<jobject(JNIEnv *,const Type&)> getter)
        : JArrayPointer<jobjectArray, Type>(env, array), m_getter(getter)
    {
        if(m_array && m_size>0){
            m_is_copy = true;
            m_array_elements = new Type[m_size];
            for(int i=0; i<m_size; i++){
                setter(m_array_elements[i], env, env->GetObjectArrayElement(m_array, i));
            }
        }
    }

    ~JObjectArrayPointer() {
        if(m_array){
            for(int i=0; i<m_size; i++){
                m_env->SetObjectArrayElement(m_array, i, m_getter(m_env, m_array_elements[i]));
            }
            delete[] m_array_elements;
        }
    }

    inline const Type* pointer() const {
        return m_array_elements;
    }

    inline operator const Type*() const {
        return m_array_elements;
    }

    inline const Type& operator[](jsize index) const{
        return m_array_elements[index];
    }

    inline Type* pointer() {
        return m_array_elements;
    }

    inline operator Type*() {
        return m_array_elements;
    }

    inline Type& operator[](jsize index) {
        return m_array_elements[index];
    }

#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)
    operator QSpan<Type> () { return m_array_elements ? QSpan<Type>(m_array_elements, m_array_elements+m_size) : QSpan<Type>(); }
    operator QSpan<const Type> () const { return m_array_elements ? QSpan<const Type>(m_array_elements, m_array_elements+m_size) : QSpan<const Type>(); }
#endif
    operator std::initializer_list<Type> () const {
        return QtJambiAPI::createIterable<std::initializer_list<Type>>(m_array_elements, m_size);
    }
    operator std::initializer_list<const Type> () const {
        return QtJambiAPI::createIterable<std::initializer_list<const Type>>(m_array_elements, m_size);
    }
    void* operator new(size_t) = delete;
    void* operator new(size_t,size_t) = delete;
    void* operator new[](size_t) = delete;
private:
    std::function<jobject(JNIEnv *,const Type&)> m_getter;
};

template<>
class JObjectArrayPointer<jobject> : public JArrayPointer<jobjectArray, jobject>
{
public:
    using JArrayPointer<jobjectArray, jobject>::size;
    using JArrayPointer<jobjectArray, jobject>::array;
    using JArrayPointer<jobjectArray, jobject>::m_size;
    using JArrayPointer<jobjectArray, jobject>::m_array;
    using JArrayPointer<jobjectArray, jobject>::m_is_copy;
    using JArrayPointer<jobjectArray, jobject>::m_env;

    JObjectArrayPointer(JNIEnv *env, jobjectArray array)
        : JArrayPointer<jobjectArray, jobject>(env, array)
    {
        if(m_array && m_size>0){
            m_is_copy = true;
        }
    }

    JObjectArrayPointer(JNIEnv *env, jobjectArray array, std::function<void(jobject&,JNIEnv *,jobject)>, std::function<jobject(JNIEnv *,const jobject&)>)
        : JObjectArrayPointer(env, array)
    {
    }

    ~JObjectArrayPointer() {
    }

    inline jobject operator[](jsize index) const{
        return m_env->GetObjectArrayElement(m_array, index);
    }

    inline auto operator[](jsize index) {
        struct jobjectRef{
            JObjectArrayPointer<jobject> *_this;
            jsize index;
            operator jobject() const{
                return _this->m_env->GetObjectArrayElement(_this->m_array, index);
            }
            jobjectRef& operator=(jobject obj){
                _this->m_env->SetObjectArrayElement(_this->m_array, index, obj);
                return *this;
            }
        };
        return jobjectRef{this, index};
    }
    void* operator new(size_t) = delete;
    void* operator new(size_t,size_t) = delete;
    void* operator new[](size_t) = delete;
};

template<>
class JObjectArrayPointer<jclass> : public JArrayPointer<jobjectArray, jclass>
{
public:
    using JArrayPointer<jobjectArray, jclass>::size;
    using JArrayPointer<jobjectArray, jclass>::array;
    using JArrayPointer<jobjectArray, jclass>::m_size;
    using JArrayPointer<jobjectArray, jclass>::m_array;
    using JArrayPointer<jobjectArray, jclass>::m_is_copy;
    using JArrayPointer<jobjectArray, jclass>::m_env;

    JObjectArrayPointer(JNIEnv *env, jobjectArray array)
        : JArrayPointer<jobjectArray, jclass>(env, array)
    {
        if(m_array && m_size>0){
            m_is_copy = true;
        }
    }

    JObjectArrayPointer(JNIEnv *env, jobjectArray array, std::function<void(jclass&,JNIEnv *,jobject)>, std::function<jobject(JNIEnv *,const jclass&)>)
        : JObjectArrayPointer(env, array)
    {
    }

    ~JObjectArrayPointer() {
    }

    inline jclass operator[](jsize index) const{
        return jclass(m_env->GetObjectArrayElement(m_array, index));
    }

    inline auto operator[](jsize index) {
        struct jobjectRef{
            JObjectArrayPointer<jclass> *_this;
            jsize index;
            operator jclass() const{
                return jclass(_this->m_env->GetObjectArrayElement(_this->m_array, index));
            }
            jobjectRef& operator=(jclass obj){
                _this->m_env->SetObjectArrayElement(_this->m_array, index, obj);
                return *this;
            }
        };
        return jobjectRef{this, index};
    }
    void* operator new(size_t) = delete;
    void* operator new(size_t,size_t) = delete;
    void* operator new[](size_t) = delete;
};

template<>
class JObjectArrayPointer<jstring> : public JArrayPointer<jobjectArray, jstring>
{
public:
    using JArrayPointer<jobjectArray, jstring>::size;
    using JArrayPointer<jobjectArray, jstring>::array;
    using JArrayPointer<jobjectArray, jstring>::m_size;
    using JArrayPointer<jobjectArray, jstring>::m_array;
    using JArrayPointer<jobjectArray, jstring>::m_is_copy;
    using JArrayPointer<jobjectArray, jstring>::m_env;

    JObjectArrayPointer(JNIEnv *env, jobjectArray array)
        : JArrayPointer<jobjectArray, jstring>(env, array)
    {
        if(m_array && m_size>0){
            m_is_copy = true;
        }
    }

    JObjectArrayPointer(JNIEnv *env, jobjectArray array, std::function<void(jstring&,JNIEnv *,jobject)>, std::function<jobject(JNIEnv *,const jstring&)>)
        : JObjectArrayPointer(env, array)
    {
    }

    ~JObjectArrayPointer() {
    }

    inline jstring operator[](jsize index) const{
        return jstring(m_env->GetObjectArrayElement(m_array, index));
    }

    inline auto operator[](jsize index) {
        struct jobjectRef{
            JObjectArrayPointer<jstring> *_this;
            jsize index;
            operator jstring() const{
                return jstring(_this->m_env->GetObjectArrayElement(_this->m_array, index));
            }
            jobjectRef& operator=(jstring obj){
                _this->m_env->SetObjectArrayElement(_this->m_array, index, obj);
                return *this;
            }
        };
        return jobjectRef{this, index};
    }
    void* operator new(size_t) = delete;
    void* operator new(size_t,size_t) = delete;
    void* operator new[](size_t) = delete;
};

template<>
class JObjectArrayPointer<jobjectArray> : public JArrayPointer<jobjectArray, jobjectArray>
{
public:
    using JArrayPointer<jobjectArray, jobjectArray>::size;
    using JArrayPointer<jobjectArray, jobjectArray>::array;
    using JArrayPointer<jobjectArray, jobjectArray>::m_size;
    using JArrayPointer<jobjectArray, jobjectArray>::m_array;
    using JArrayPointer<jobjectArray, jobjectArray>::m_is_copy;
    using JArrayPointer<jobjectArray, jobjectArray>::m_env;

    JObjectArrayPointer(JNIEnv *env, jobjectArray array)
        : JArrayPointer<jobjectArray, jobjectArray>(env, array)
    {
        if(m_array && m_size>0){
            m_is_copy = true;
        }
    }

    JObjectArrayPointer(JNIEnv *env, jobjectArray array, std::function<void(jobjectArray&,JNIEnv *,jobject)>, std::function<jobject(JNIEnv *,const jobjectArray&)>)
        : JObjectArrayPointer(env, array)
    {
    }

    ~JObjectArrayPointer() {
    }

    inline jobjectArray operator[](jsize index) const{
        return jobjectArray(m_env->GetObjectArrayElement(m_array, index));
    }

    inline auto operator[](jsize index) {
        struct jobjectRef{
            JObjectArrayPointer<jobjectArray> *_this;
            jsize index;
            operator jobjectArray() const{
                return jobjectArray(_this->m_env->GetObjectArrayElement(_this->m_array, index));
            }
            jobjectRef& operator=(jobjectArray obj){
                _this->m_env->SetObjectArrayElement(_this->m_array, index, obj);
                return *this;
            }
        };
        return jobjectRef{this, index};
    }
    void* operator new(size_t) = delete;
    void* operator new(size_t,size_t) = delete;
    void* operator new[](size_t) = delete;
};

template<class Type>
class PersistentJConstObjectArrayPointer : public PersistentJArrayPointer<jobjectArray, Type>
{
public:
    using PersistentJArrayPointer<jobjectArray, Type>::size;
    using PersistentJArrayPointer<jobjectArray, Type>::array;
    using PersistentJArrayPointer<jobjectArray, Type>::m_array_elements;
    using PersistentJArrayPointer<jobjectArray, Type>::m_is_copy;

    PersistentJConstObjectArrayPointer(JNIEnv *env, jobjectArray array, std::function<void(Type&,JNIEnv *,jobject)> setter)
        : PersistentJArrayPointer<jobjectArray, Type>(env, array)
    {
        if(array && size()>0){
            m_is_copy = true;
            m_array_elements = new Type[size()];
            for(int i=0; i<size(); i++){
                setter(m_array_elements[i], env, env->GetObjectArrayElement(array, i));
            }
        }
    }

    ~PersistentJConstObjectArrayPointer() {
        if(array()){
            delete[] m_array_elements;
        }
    }

    inline const Type* pointer() const {
        return m_array_elements;
    }

    inline operator const Type*() const {
        return m_array_elements;
    }

    inline const Type& operator[](jsize index) const{
        return m_array_elements[index];
    }
#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)
    QSpan<const Type> span() const { return m_array_elements ? QSpan<const Type>(m_array_elements, m_array_elements+size()) : QSpan<const Type>(); }
    operator QSpan<const Type> () const { return span(); }
#endif
    operator std::initializer_list<Type> () const {
        return QtJambiAPI::createIterable<std::initializer_list<Type>>(m_array_elements, size());
    }
    operator std::initializer_list<const Type> () const {
        return QtJambiAPI::createIterable<std::initializer_list<const Type>>(m_array_elements, size());
    }
};

template<class Type>
class PersistentJObjectArrayPointer : public PersistentJArrayPointer<jobjectArray, Type>
{
public:
    using PersistentJArrayPointer<jobjectArray, Type>::size;
    using PersistentJArrayPointer<jobjectArray, Type>::array;
    using PersistentJArrayPointer<jobjectArray, Type>::m_array_elements;
    using PersistentJArrayPointer<jobjectArray, Type>::m_is_copy;

    PersistentJObjectArrayPointer(JNIEnv *env, jobjectArray array, std::function<void(Type&,JNIEnv *,jobject)> setter, std::function<jobject(JNIEnv *,const Type&)> getter)
        : PersistentJArrayPointer<jobjectArray, Type>(env, array), m_getter(getter)
    {
        if(array && size()>0){
            m_is_copy = true;
            m_array_elements = new Type[size()];
            for(int i=0; i<size(); i++){
                setter(m_array_elements[i], env, env->GetObjectArrayElement(array, i));
            }
        }
    }

    ~PersistentJObjectArrayPointer() {
        if(array()){
            if(JniEnvironment env{16+size()}){
                auto _array = array(env);
                for(int i=0; i<size(); i++){
                    env->SetObjectArrayElement(_array, i, m_getter(env, m_array_elements[i]));
                }
            }
            delete[] m_array_elements;
        }
    }

    inline const Type* pointer() const {
        return m_array_elements;
    }

    inline operator const Type*() const {
        return m_array_elements;
    }

    inline const Type& operator[](jsize index) const{
        return m_array_elements[index];
    }

    inline Type* pointer() {
        return m_array_elements;
    }

    inline operator Type*() {
        return m_array_elements;
    }

    inline Type& operator[](jsize index) {
        return m_array_elements[index];
    }

#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)
    operator QSpan<Type> () { return m_array_elements ? QSpan<Type>(m_array_elements, m_array_elements+size()) : QSpan<Type>(); }
    operator QSpan<const Type> () const { return m_array_elements ? QSpan<const Type>(m_array_elements, m_array_elements+size()) : QSpan<const Type>(); }
#endif
    operator std::initializer_list<Type> () const {
        return QtJambiAPI::createIterable<std::initializer_list<Type>>(m_array_elements, size());
    }
    operator std::initializer_list<const Type> () const {
        return QtJambiAPI::createIterable<std::initializer_list<const Type>>(m_array_elements, size());
    }
private:
    std::function<jobject(JNIEnv *,const Type&)> m_getter;
};

#endif // QTJAMBI_JAVAARRAYS_H
