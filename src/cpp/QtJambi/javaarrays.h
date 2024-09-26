/****************************************************************************
**
** Copyright (C) 2009-2024 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

class QTJAMBI_EXPORT UCharPointerArray : public PointerArray<jbyteArray,uchar>
{
public:
    UCharPointerArray(JNIEnv *env, uchar* pointer, jsize size);
    ~UCharPointerArray() override;
};

class QTJAMBI_EXPORT UInt8PointerArray : public PointerArray<jbyteArray,quint8>
{
public:
    UInt8PointerArray(JNIEnv *env, quint8* pointer, jsize size);
    ~UInt8PointerArray() override;
};

class QTJAMBI_EXPORT UInt16PointerArray : public PointerArray<jshortArray,quint16>
{
public:
    UInt16PointerArray(JNIEnv *env, quint16* pointer, jsize size);
    ~UInt16PointerArray() override;
};

class QTJAMBI_EXPORT UInt32PointerArray : public PointerArray<jintArray,quint32>
{
public:
    UInt32PointerArray(JNIEnv *env, quint32* pointer, jsize size);
    ~UInt32PointerArray() override;
};

class QTJAMBI_EXPORT UInt64PointerArray : public PointerArray<jlongArray,quint64>
{
public:
    UInt64PointerArray(JNIEnv *env, quint64* pointer, jsize size);
    ~UInt64PointerArray() override;
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
    ConstCharPointerArray(JNIEnv *env, const uchar* pointer, jsize size);
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

class QTJAMBI_EXPORT ConstUInt8PointerArray : public PointerArray<jbyteArray,const quint8>
{
public:
    ConstUInt8PointerArray(JNIEnv *env, const quint8* pointer, jsize size);
};

class QTJAMBI_EXPORT ConstUInt16PointerArray : public PointerArray<jshortArray,const quint16>
{
public:
    ConstUInt16PointerArray(JNIEnv *env, const quint16* pointer, jsize size);
};

class QTJAMBI_EXPORT ConstUInt32PointerArray : public PointerArray<jintArray,const quint32>
{
public:
    ConstUInt32PointerArray(JNIEnv *env, const quint32* pointer, jsize size);
};

class QTJAMBI_EXPORT ConstUInt64PointerArray : public PointerArray<jlongArray,const quint64>
{
public:
    ConstUInt64PointerArray(JNIEnv *env, const quint64* pointer, jsize size);
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
        : m_array(array ? JArray(env->NewGlobalRef(array)) : nullptr),
          m_size(array ? env->GetArrayLength(m_array) : 0),
          m_is_copy(false),
          m_array_elements(nullptr) {
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
    inline bool isBuffering() const { return m_is_copy; }
    virtual void commit(JNIEnv *){}
protected:
    typedef JArray JArrayType;
    typedef typename QtJambiPrivate::ElementForArray<JArrayType>::type ElementType;
    inline JArray array() const { return m_array; }
    JArray m_array;
    jsize m_size;
    jboolean m_is_copy;
    JType* m_array_elements;
    Q_DISABLE_COPY(JArrayPointer)
};

#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)
#define QTJAMBI_POINTER_ARRAY_CONST_OPERATOR(type)\
operator QSpan<const type> () const;\
operator const type* () const;\
operator std::initializer_list<type> () const;\
operator std::initializer_list<const type> () const;
#define QTJAMBI_POINTER_ARRAY_OPERATOR(type)\
operator QSpan<type> ();\
operator type* ();
#else
#define QTJAMBI_POINTER_ARRAY_CONST_OPERATOR(type)\
operator const type* () const;\
operator std::initializer_list<type> () const;\
operator std::initializer_list<const type> () const;
#define QTJAMBI_POINTER_ARRAY_OPERATOR(type)\
operator type* ();
#endif

#define QTJAMBI_TYPED_ARRAY_POINTER(Type,jArray,const_operators,operators)\
class QTJAMBI_EXPORT JConst##Type##ArrayPointer : public JArrayPointer<jArray>\
{\
public:\
    JConst##Type##ArrayPointer(JNIEnv *env, jArray array);\
    ~JConst##Type##ArrayPointer() override;\
    ElementType operator[](int index) const;\
    const_operators\
    static bool isValidArray(JNIEnv *env, jobject object);\
private:\
    using JArrayPointer<jArray>::JArrayType;\
    using JArrayPointer<jArray>::ElementType;\
};\
\
class QTJAMBI_EXPORT J##Type##ArrayPointer : public JArrayPointer<jArray>\
{\
public:\
    J##Type##ArrayPointer(JNIEnv *env, jArray array);\
    ~J##Type##ArrayPointer() override;\
    void commit(JNIEnv *env) override;\
    ElementType operator[](int index) const;\
    ElementType& operator[](int index);\
    const_operators\
    operators\
    static bool isValidArray(JNIEnv *env, jobject object);\
private:\
    using JArrayPointer<jArray>::JArrayType;\
    using JArrayPointer<jArray>::ElementType;\
}

#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
#define QTJAMBI_BYTEARRAY_OPERATOR() \
    operator QByteArrayView() const;\
    operator QByteArray() const;
#define QTJAMBI_STRING_OPERATOR() \
operator QStringView() const;\
operator QString() const;
#else
#define QTJAMBI_BYTEARRAY_OPERATOR() \
    operator QByteArray() const;
#define QTJAMBI_STRING_OPERATOR() \
    operator QString() const;
#endif

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
    jboolean* booleanArray();
#define QTJAMBI_CONST_BOOLEAN_ARRAY() \
    const jboolean* booleanArray() const;

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
    using JArrayPointer<jobjectArray, Type>::pointer;
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

    ~JConstObjectArrayPointer() override {
        if(m_array){
            delete[] m_array_elements;
        }
    }

    operator const Type*() const {return m_array_elements;}
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
};

template<class Type>
class JObjectArrayPointer : public JArrayPointer<jobjectArray, Type>
{
public:
    using JArrayPointer<jobjectArray, Type>::pointer;
    using JArrayPointer<jobjectArray, Type>::size;
    using JArrayPointer<jobjectArray, Type>::array;
    using JArrayPointer<jobjectArray, Type>::m_size;
    using JArrayPointer<jobjectArray, Type>::m_array;
    using JArrayPointer<jobjectArray, Type>::m_array_elements;
    using JArrayPointer<jobjectArray, Type>::m_is_copy;

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

    ~JObjectArrayPointer() override {
        if(m_array){
            if(JniEnvironment env{300}){
                for(int i=0; i<m_size; i++){
                    env->SetObjectArrayElement(m_array, i, m_getter(env, m_array_elements[i]));
                }
            }
            delete[] m_array_elements;
        }
    }

    operator Type*() {return m_array_elements;}
    operator const Type*() const {return m_array_elements;}
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
private:
    std::function<jobject(JNIEnv *,const Type&)> m_getter;
};

#endif // QTJAMBI_JAVAARRAYS_H
