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

#if !defined(QTJAMBI_JAVAARRAYS_H) && !defined(QTJAMBI_GENERATOR_RUNNING)
#define QTJAMBI_JAVAARRAYS_H

#include "jnienvironment.h"

namespace QtJambiAPI{

QTJAMBI_EXPORT jobjectArray createObjectArray(JNIEnv *env, const char* componentClass, jsize size);
QTJAMBI_EXPORT jobjectArray createObjectArray(JNIEnv *env, const std::type_info& componentType, jsize size);

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
    virtual ~PointerArray();
    Q_DISABLE_COPY(PointerArray)
    inline JArray array(JNIEnv *env) {return JArray(env->NewLocalRef(m_array));}
protected:
    PointerArray(JNIEnv *env, CType* pointer, JArray array, jsize size);
    CType* pointer () { return m_pointer; }
    JArray m_array;
    jsize m_size;
    CType* m_pointer;
};

template<typename JArray, typename CType>
inline PointerArray<JArray, CType>::PointerArray(JNIEnv *env, CType* pointer, JArray array, jsize size)
    : m_array(JArray(env->NewGlobalRef(array))),
      m_size(jsize(size)),
      m_pointer(pointer)
{}

template<typename JArray, typename CType>
inline PointerArray<JArray, CType>::~PointerArray(){
    if(m_array){
        if(JniEnvironment env{300}){
            env->DeleteGlobalRef(m_array);
        }
    }
}

class QTJAMBI_EXPORT CharPointerArray : public PointerArray<jbyteArray,char>
{
public:
    CharPointerArray(JNIEnv *env, char* pointer, jsize size);
    ~CharPointerArray() override;
};

class QTJAMBI_EXPORT Int8PointerArray : public PointerArray<jbyteArray,qint8>
{
public:
    Int8PointerArray(JNIEnv *env, qint8* pointer, jsize size);
    ~Int8PointerArray() override;
};

class QTJAMBI_EXPORT Int16PointerArray : public PointerArray<jshortArray,qint16>
{
public:
    Int16PointerArray(JNIEnv *env, qint16* pointer, jsize size);
    ~Int16PointerArray() override;
};

class QTJAMBI_EXPORT Int32PointerArray : public PointerArray<jintArray,qint32>
{
public:
    Int32PointerArray(JNIEnv *env, qint32* pointer, jsize size);
    ~Int32PointerArray() override;
};

class QTJAMBI_EXPORT Int64PointerArray : public PointerArray<jlongArray,qint64>
{
public:
    Int64PointerArray(JNIEnv *env, qint64* pointer, jsize size);
    ~Int64PointerArray() override;
};

class QTJAMBI_EXPORT BoolPointerArray : public PointerArray<jbooleanArray,bool>
{
public:
    BoolPointerArray(JNIEnv *env, bool* pointer, jsize size);
    ~BoolPointerArray() override;
};

class QTJAMBI_EXPORT Bool2PointerArray : public PointerArray<jbooleanArray,uchar>
{
public:
    Bool2PointerArray(JNIEnv *env, uchar* pointer, jsize size);
    ~Bool2PointerArray() override;
};

class QTJAMBI_EXPORT DoublePointerArray : public PointerArray<jdoubleArray,double>
{
public:
    DoublePointerArray(JNIEnv *env, double* pointer, jsize size);
    ~DoublePointerArray() override;
};

class QTJAMBI_EXPORT FloatPointerArray : public PointerArray<jfloatArray,float>
{
public:
    FloatPointerArray(JNIEnv *env, float* pointer, jsize size);
    ~FloatPointerArray() override;
};

class QTJAMBI_EXPORT WCharPointerArray : public PointerArray<jcharArray,ushort>
{
public:
    WCharPointerArray(JNIEnv *env, ushort* pointer, jsize size);
    ~WCharPointerArray() override;
};

class QTJAMBI_EXPORT QCharPointerArray : public PointerArray<jcharArray,QChar>
{
public:
    QCharPointerArray(JNIEnv *env, QChar* pointer, jsize size);
    ~QCharPointerArray() override;
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
    ~ObjectPointerArray() override;
private:
    std::function<void(T&,JNIEnv *,jobject)> m_setter;
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
        if(JniEnvironment env{300}){
            for(jsize i=0; i<PointerArray<jobjectArray,T>::size(); ++i){
                m_setter(PointerArray<jobjectArray,T>::pointer()[i], env, env->GetObjectArrayElement(PointerArray<jobjectArray,T>::array(), i));
            }
        }
    }
}

class QTJAMBI_EXPORT ConstCharPointerArray : public PointerArray<jbyteArray,const char>
{
public:
    ConstCharPointerArray(JNIEnv *env, const char* pointer, jsize size);
};

class QTJAMBI_EXPORT ConstInt8PointerArray : public PointerArray<jbyteArray,const qint8>
{
public:
    ConstInt8PointerArray(JNIEnv *env, const qint8* pointer, jsize size);
};

class QTJAMBI_EXPORT ConstInt16PointerArray : public PointerArray<jshortArray,const qint16>
{
public:
    ConstInt16PointerArray(JNIEnv *env, const qint16* pointer, jsize size);
};

class QTJAMBI_EXPORT ConstInt32PointerArray : public PointerArray<jintArray,const qint32>
{
public:
    ConstInt32PointerArray(JNIEnv *env, const qint32* pointer, jsize size);
};

class QTJAMBI_EXPORT ConstInt64PointerArray : public PointerArray<jlongArray,const qint64>
{
public:
    ConstInt64PointerArray(JNIEnv *env, const qint64* pointer, jsize size);
};

class QTJAMBI_EXPORT ConstBoolPointerArray : public PointerArray<jbooleanArray,const bool>
{
public:
    ConstBoolPointerArray(JNIEnv *env, const bool* pointer, jsize size);
};

class QTJAMBI_EXPORT ConstBool2PointerArray : public PointerArray<jbooleanArray,const uchar>
{
public:
    ConstBool2PointerArray(JNIEnv *env, const uchar* pointer, jsize size);
};

class QTJAMBI_EXPORT ConstDoublePointerArray : public PointerArray<jdoubleArray,const double>
{
public:
    ConstDoublePointerArray(JNIEnv *env, const double* pointer, jsize size);
};

class QTJAMBI_EXPORT ConstFloatPointerArray : public PointerArray<jfloatArray,const float>
{
public:
    ConstFloatPointerArray(JNIEnv *env, const float* pointer, jsize size);
};

class QTJAMBI_EXPORT ConstWCharPointerArray : public PointerArray<jcharArray,const ushort>
{
public:
    ConstWCharPointerArray(JNIEnv *env, const ushort* pointer, jsize size);
};

class QTJAMBI_EXPORT ConstQCharPointerArray : public PointerArray<jcharArray,const QChar>
{
public:
    ConstQCharPointerArray(JNIEnv *env, const QChar* pointer, jsize size);
};

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
class QTJAMBI_EXPORT Char16PointerArray : public PointerArray<jcharArray,char16_t>
{
public:
    Char16PointerArray(JNIEnv *env, char16_t* pointer, jsize size);
    ~Char16PointerArray() override;
};

class QTJAMBI_EXPORT ConstChar16PointerArray : public PointerArray<jcharArray,const char16_t>
{
public:
    ConstChar16PointerArray(JNIEnv *env, const char16_t* pointer, jsize size);
};

class QTJAMBI_EXPORT Char32PointerArray : public PointerArray<jintArray,char32_t>
{
public:
    Char32PointerArray(JNIEnv *env, char32_t* pointer, jsize size);
    ~Char32PointerArray() override;
};

class QTJAMBI_EXPORT ConstChar32PointerArray : public PointerArray<jintArray,const char32_t>
{
public:
    ConstChar32PointerArray(JNIEnv *env, const char32_t* pointer, jsize size);
};
#endif

template<typename T>
class ConstObjectPointerArray : public PointerArray<jobjectArray,const T>
{
public:
    ConstObjectPointerArray(JNIEnv *env, const T* pointer, jsize _size,
                            const char* javaClass,
                            std::function<jobject(JNIEnv *,const T&)> getter);
    ConstObjectPointerArray(JNIEnv *env, const T* pointer, jsize _size,
                            std::function<jobject(JNIEnv *,const T&)> getter);
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

template<typename JArray, typename JType>
class QTJAMBI_EXPORT JArrayPointer{
public:
    inline JArrayPointer(JNIEnv *env, JArray array, bool writable)
        : m_array(array ? JArray(env->NewGlobalRef(array)) : nullptr),
          m_size(array ? env->GetArrayLength(m_array) : 0),
          m_is_copy(false),
          m_array_elements(nullptr),
          m_writable(writable) {
        JavaException::check(env QTJAMBI_STACKTRACEINFO );
    }

    inline virtual ~JArrayPointer()
    {
        if(m_array){
            if(JniEnvironment env{16}){
                env->DeleteGlobalRef(m_array);
            }
        }
    }

    inline JType* pointer(){
        return m_array_elements;
    }

    inline const JType* pointer() const {
        return m_array_elements;
    }

    inline operator JType*(){
        return m_array_elements;
    }

    inline operator const JType*() const {
        return m_array_elements;
    }

    inline jsize size() const { return m_size; }
protected:
    inline JArray array() const { return m_array; }
    JArray m_array;
    jsize m_size;
    jboolean m_is_copy;
    JType* m_array_elements;
    bool m_writable;
    Q_DISABLE_COPY(JArrayPointer)
};

class QTJAMBI_EXPORT JByteArrayPointer : public JArrayPointer<jbyteArray, jbyte>
{
public:
    JByteArrayPointer(JNIEnv *env, jbyteArray array, bool writable = true);
    ~JByteArrayPointer() override;
    operator char* () { return reinterpret_cast<char*>(m_array_elements); }
    operator const char* () const { return reinterpret_cast<const char*>(m_array_elements); }
    operator qint8* () { return reinterpret_cast<qint8*>(m_array_elements); }
    operator const qint8* () const { return reinterpret_cast<const qint8*>(m_array_elements); }
    operator quint8* () { return reinterpret_cast<quint8*>(m_array_elements); }
    operator const quint8* () const { return reinterpret_cast<const quint8*>(m_array_elements); }
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    operator QByteArrayView() const { return QByteArrayView(reinterpret_cast<const char*>(m_array_elements), size()); }
#endif
    operator QByteArray() const { return QByteArray(reinterpret_cast<const char*>(m_array_elements), size()); }
    static bool isValidArray(JNIEnv *env, jobject object);
private:
};

class QTJAMBI_EXPORT JIntArrayPointer : public JArrayPointer<jintArray, jint>
{
public:
    JIntArrayPointer(JNIEnv *env, jintArray array, bool writable = true);
    ~JIntArrayPointer() override;
    operator int* () { return reinterpret_cast<int*>(m_array_elements); }
    operator const int* () const { return reinterpret_cast<int*>(m_array_elements); }
    operator uint* () { return reinterpret_cast<uint*>(m_array_elements); }
    operator const uint* () const { return reinterpret_cast<uint*>(m_array_elements); }
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    operator char32_t* () { return reinterpret_cast<char32_t*>(m_array_elements); }
    operator const char32_t* () const { return reinterpret_cast<char32_t*>(m_array_elements); }
#endif
    static bool isValidArray(JNIEnv *env, jobject object);
};

class QTJAMBI_EXPORT JLongArrayPointer : public JArrayPointer<jlongArray, jlong>
{
public:
    JLongArrayPointer(JNIEnv *env, jlongArray array, bool writable = true);
    ~JLongArrayPointer() override;
    operator qint64* () { return reinterpret_cast<qint64*>(m_array_elements); }
    operator const qint64* () const { return reinterpret_cast<qint64*>(m_array_elements); }
    operator quint64* () { return reinterpret_cast<quint64*>(m_array_elements); }
    operator const quint64* () const { return reinterpret_cast<quint64*>(m_array_elements); }
    static bool isValidArray(JNIEnv *env, jobject object);
};

class QTJAMBI_EXPORT JFloatArrayPointer : public JArrayPointer<jfloatArray, jfloat>
{
public:
    JFloatArrayPointer(JNIEnv *env, jfloatArray array, bool writable = true);
    ~JFloatArrayPointer() override;
    operator float* () { return reinterpret_cast<float*>(m_array_elements); }
    operator const float* () const { return reinterpret_cast<float*>(m_array_elements); }
    static bool isValidArray(JNIEnv *env, jobject object);
};

class QTJAMBI_EXPORT JDoubleArrayPointer : public JArrayPointer<jdoubleArray, jdouble>
{
public:
    JDoubleArrayPointer(JNIEnv *env, jdoubleArray array, bool writable = true);
    ~JDoubleArrayPointer() override;
    operator double* () { return reinterpret_cast<double*>(m_array_elements); }
    operator const double* () const { return reinterpret_cast<double*>(m_array_elements); }
    static bool isValidArray(JNIEnv *env, jobject object);
};

class QTJAMBI_EXPORT JShortArrayPointer : public JArrayPointer<jshortArray, jshort>
{
public:
    JShortArrayPointer(JNIEnv *env, jshortArray array, bool writable = true);
    ~JShortArrayPointer() override;
    operator qint16* () { return reinterpret_cast<qint16*>(m_array_elements); }
    operator const qint16* () const { return reinterpret_cast<qint16*>(m_array_elements); }
    operator quint16* () { return reinterpret_cast<quint16*>(m_array_elements); }
    operator const quint16* () const { return reinterpret_cast<quint16*>(m_array_elements); }
    static bool isValidArray(JNIEnv *env, jobject object);
};

class QTJAMBI_EXPORT JCharArrayPointer : public JArrayPointer<jcharArray, jchar>
{
public:
    JCharArrayPointer(JNIEnv *env, jcharArray array, bool writable = true);
    ~JCharArrayPointer() override;
    operator qint16* () { return reinterpret_cast<qint16*>(m_array_elements); }
    operator const qint16* () const { return reinterpret_cast<qint16*>(m_array_elements); }
    operator quint16* () { return reinterpret_cast<quint16*>(m_array_elements); }
    operator const quint16* () const { return reinterpret_cast<quint16*>(m_array_elements); }
    operator wchar_t* () { return reinterpret_cast<wchar_t*>(m_array_elements); }
    operator const wchar_t* () const { return reinterpret_cast<wchar_t*>(m_array_elements); }
    operator QChar* () { return reinterpret_cast<QChar*>(m_array_elements); }
    operator const QChar* () const { return reinterpret_cast<QChar*>(m_array_elements); }
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    operator QStringView() const { return QStringView(reinterpret_cast<const QChar*>(m_array_elements), size()); }
    operator char16_t*() const { return reinterpret_cast<char16_t*>(m_array_elements); }
    operator const char16_t*() const { return reinterpret_cast<const char16_t*>(m_array_elements); }
#endif
    operator QString() const { return QString(reinterpret_cast<const QChar*>(m_array_elements), size()); }
    static bool isValidArray(JNIEnv *env, jobject object);
};

class QTJAMBI_EXPORT JBooleanArrayPointer : public JArrayPointer<jbooleanArray, bool>
{
public:
    JBooleanArrayPointer(JNIEnv *env, jbooleanArray array, bool writable = true);
    ~JBooleanArrayPointer() override;
    static bool isValidArray(JNIEnv *env, jobject object);
    jboolean* booleanArray();
    const jboolean* booleanArray() const;
private:
    jboolean* m_boolean_array;
};

template<class Type>
class QTJAMBI_EXPORT JObjectArrayPointer : public JArrayPointer<jobjectArray, Type>
{
public:
    JObjectArrayPointer(JNIEnv *env, jobjectArray array, std::function<void(Type&,JNIEnv *,jobject)> setter, std::function<jobject(JNIEnv *,const Type&)> getter)
        : JArrayPointer<jobjectArray, Type>(env, array, true), m_getter(getter)
    {
        if(JArrayPointer<jobjectArray, Type>::m_array && JArrayPointer<jobjectArray, Type>::size()>0){
            JArrayPointer<jobjectArray, Type>::m_array_elements = new Type[JArrayPointer<jobjectArray, Type>::size()];
            for(int i=0; i<JArrayPointer<jobjectArray, Type>::size(); i++){
                setter(JArrayPointer<jobjectArray, Type>::m_array_elements[i], env, env->GetObjectArrayElement(JArrayPointer<jobjectArray, Type>::array(), i));
            }
        }
    }

    JObjectArrayPointer(JNIEnv *env, jobjectArray array, std::function<void(Type&,JNIEnv *,jobject)> setter)
        : JArrayPointer<jobjectArray, Type>(env, array, false), m_getter()
    {
        if(JArrayPointer<jobjectArray, Type>::m_array && JArrayPointer<jobjectArray, Type>::m_size>0){
            JArrayPointer<jobjectArray, Type>::m_array_elements = new Type[JArrayPointer<jobjectArray, Type>::m_size];
            for(int i=0; i<JArrayPointer<jobjectArray, Type>::m_size; i++){
                setter(JArrayPointer<jobjectArray, Type>::m_array_elements[i], env, env->GetObjectArrayElement(JArrayPointer<jobjectArray, Type>::m_array, i));
            }
        }
    }

    ~JObjectArrayPointer() override {
        if(JArrayPointer<jobjectArray, Type>::m_array){
            if(JArrayPointer<jobjectArray, Type>::m_writable){
                if(JniEnvironment env{300}){
                    for(int i=0; i<JArrayPointer<jobjectArray, Type>::m_size; i++){
                        env->SetObjectArrayElement(JArrayPointer<jobjectArray, Type>::m_array, i, m_getter(env, JArrayPointer<jobjectArray, Type>::m_array_elements[i]));
                    }
                }
            }
            delete[] JArrayPointer<jobjectArray, Type>::m_array_elements;
        }
    }

    operator Type*(){return reinterpret_cast<Type*>(JArrayPointer<jobjectArray, Type>::pointer());}
    operator const Type*() const {return reinterpret_cast<const Type*>(JArrayPointer<jobjectArray, Type>::pointer());}
private:
    std::function<jobject(JNIEnv *,const Type&)> m_getter;
};

#endif // QTJAMBI_JAVAARRAYS_H
