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
#include "typetests.h"
#include "qtjambiapi_array.h"

namespace QtJambiPrivate {

template<typename T, typename CType>
static constexpr bool is_compatible(){
    if constexpr(sizeof(T)==sizeof(CType) && !std::is_same_v<T,CType>){
        if constexpr(std::is_integral_v<T>){
            return true;
        }
        if constexpr(sizeof(char)==sizeof(CType)
                      && (std::is_same_v<T, QLatin1Char>
                          || std::is_same_v<T, std::byte>)){
            return true;
        }
        if constexpr(sizeof(char16_t)==sizeof(CType)
                      && std::is_same_v<T, QChar>){
            return true;
        }
    }
    return false;
}

template<typename JArray, bool isCompatible>
struct PointerArrayInfo{
    JArray array;
    jsize size;
    bool isNewArray;
};

template<typename JArray>
struct PointerArrayInfo<JArray,false>{
    JArray array;
    jsize size;
    typename QtJambiPrivate::jni_type<JArray>::ElementType* arrayElements;
    jboolean isCopy;
};

template<typename JArray, typename CType>
PointerArrayInfo<JArray,false> createArray(JNIEnv *env, CType* pointer, jsize size){
    if(pointer){
        JArray array = (env->*QtJambiPrivate::jni_primitive_array_functions<JArray>::NewArray)(size);
        JavaException::check(env QTJAMBI_STACKTRACEINFO );
        jboolean isCopy = false;
        typename QtJambiPrivate::jni_type<JArray>::ElementType* arrayElements = (env->*QtJambiPrivate::jni_primitive_array_functions<JArray>::GetArrayElements)(array, &isCopy);
        JavaException::check(env QTJAMBI_STACKTRACEINFO );
        for(size_t i=0; i<size_t(size); ++i){
            arrayElements[i] = pointer[i];
        }
        return PointerArrayInfo<JArray,false>{array,size,arrayElements,isCopy};
    }else return PointerArrayInfo<JArray,false>{nullptr,0,nullptr,false};
}

QTJAMBI_EXPORT PointerArrayInfo<jintArray,true> findOrCreateArray(JNIEnv *env, const jint* pointer, jsize size);
QTJAMBI_EXPORT PointerArrayInfo<jlongArray,true> findOrCreateArray(JNIEnv *env, const jlong* pointer, jsize size);
QTJAMBI_EXPORT PointerArrayInfo<jbyteArray,true> findOrCreateArray(JNIEnv *env, const jbyte* pointer, jsize size);
QTJAMBI_EXPORT PointerArrayInfo<jshortArray,true> findOrCreateArray(JNIEnv *env, const jshort* pointer, jsize size);
QTJAMBI_EXPORT PointerArrayInfo<jcharArray,true> findOrCreateArray(JNIEnv *env, const jchar* pointer, jsize size);
QTJAMBI_EXPORT PointerArrayInfo<jbooleanArray,true> findOrCreateArray(JNIEnv *env, const jboolean* pointer, jsize size);
QTJAMBI_EXPORT PointerArrayInfo<jfloatArray,true> findOrCreateArray(JNIEnv *env, const jfloat* pointer, jsize size);
QTJAMBI_EXPORT PointerArrayInfo<jdoubleArray,true> findOrCreateArray(JNIEnv *env, const jdouble* pointer, jsize size);

} // namespace QtJambiPrivate

template<bool persistent, typename JArray, bool isConst, typename CType = typename QtJambiPrivate::jni_type<JArray>::ElementType, bool isCompatible = sizeof(CType)==sizeof(typename QtJambiPrivate::jni_type<JArray>::ElementType)>
class PointerArray;

template<typename JArray, bool isConst, typename CType>
class PointerArray<false, JArray, isConst, CType, true>{
public:
    using ArrayType = std::conditional_t<isConst, std::add_const_t<CType>, CType>;
    JArray array() {return m_array;}
    JArray array() const {return const_cast<JArray>(m_array);}
    operator JArray(){return m_array;}
    operator JArray() const {return array();}
    operator void*(){return m_array;}
    operator void*() const {return array();}
    operator jobject(){return m_array;}
    operator jobject() const {return array();}
    operator jvalue() const {
        jvalue v;
        v.l = array();
        return v;
    }
    ArrayType* pointer () const { return m_pointer; }
    jsize size() const {return m_size;}
    PointerArray(JNIEnv *env, ArrayType* pointer, jsize size)
        : PointerArray(env, pointer, QtJambiPrivate::findOrCreateArray(env, pointer, size)) {}

    template<typename T>
    PointerArray(JNIEnv *env, T* pointer, std::enable_if_t<QtJambiPrivate::is_compatible<std::remove_cv_t<T>,CType>(), jsize> size)
        : PointerArray(env, reinterpret_cast<ArrayType*>(pointer), size) {}

    ~PointerArray(){
        if constexpr(!isConst && !std::is_same_v<JArray,jobjectArray>){
            if(m_array && m_isNewArray){
                (m_env->*QtJambiPrivate::jni_primitive_array_functions<JArray>::GetArrayRegion)(m_array, 0, m_size, reinterpret_cast<typename QtJambiPrivate::jni_type<JArray>::ElementType *>(m_pointer));
                JavaException::check(m_env QTJAMBI_STACKTRACEINFO );
            }
        }
    }
    Q_DISABLE_COPY(PointerArray)
protected:
    JNIEnv *m_env;
    JArray m_array;
    jsize m_size;
    bool m_isNewArray;
    ArrayType* m_pointer;
    template<typename JObjectArray>
    PointerArray(JNIEnv *env, ArrayType* pointer, JObjectArray array, std::enable_if_t<std::is_same_v<JObjectArray,jobjectArray> && std::is_same_v<JArray,jobjectArray>, jsize> size)
        :  m_env(env),
        m_array(array),
        m_size(size),
        m_isNewArray(true),
        m_pointer(pointer){}
private:
    inline PointerArray(JNIEnv *env, ArrayType* pointer, const QtJambiPrivate::PointerArrayInfo<JArray,true>& data)
     :  m_env(env),
        m_array(data.array),
        m_size(data.size),
        m_isNewArray(data.isNewArray),
        m_pointer(pointer){}

    void* operator new(size_t) = delete;
    void* operator new(size_t,size_t) = delete;
    void* operator new[](size_t) = delete;
};

template<typename JArray, bool isConst, typename CType>
class PointerArray<false,JArray,isConst,CType,false>{
public:
    using ArrayType = std::conditional_t<isConst, std::add_const_t<CType>, CType>;
    JArray array() {return m_array;}
    JArray array() const {return const_cast<JArray>(m_array);}
    operator JArray(){return m_array;}
    operator JArray() const {return array();}
    operator void*(){return m_array;}
    operator void*() const {return array();}
    operator jobject(){return m_array;}
    operator jobject() const {return array();}
    operator jvalue() const {
        jvalue v;
        v.l = array();
        return v;
    }
    ArrayType* pointer () const { return m_pointer; }
    jsize size() const {return m_size;}
    PointerArray(JNIEnv *env, ArrayType* pointer, jsize size)
        : PointerArray(env, pointer, QtJambiPrivate::createArray<JArray,ArrayType>(env, pointer, size)) {}

    ~PointerArray(){
        if(m_array){
            if constexpr(isConst){
                if(JniEnvironment env{100}){
                    (m_env->*QtJambiPrivate::jni_primitive_array_functions<JArray>::ReleaseArrayElements)(m_array, m_arrayElements, JNI_ABORT);
                    JavaException::check(m_env QTJAMBI_STACKTRACEINFO );
                }
            }else{
                if(JniEnvironment env{100}){
                    for(size_t i=0; i<m_size; ++i){
                        m_pointer[i] = m_arrayElements[i];
                    }
                    (m_env->*QtJambiPrivate::jni_primitive_array_functions<JArray>::ReleaseArrayElements)(m_array, m_arrayElements, JNI_OK);
                    JavaException::check(m_env QTJAMBI_STACKTRACEINFO );
                }
            }
        }
    }
    Q_DISABLE_COPY(PointerArray)
protected:
    JNIEnv *m_env;
    JArray m_array;
    jsize m_size;
    typename QtJambiPrivate::jni_type<JArray>::ElementType* m_arrayElements;
    bool m_isCopy;
    ArrayType* m_pointer;
private:
    PointerArray(JNIEnv *env, ArrayType* pointer, const QtJambiPrivate::PointerArrayInfo<JArray,false>& data)
        :  m_env(env),
        m_array(data.array),
        m_size(data.size),
        m_arrayElements(data.arrayElements),
        m_isCopy(data.isCopy),
        m_pointer(pointer){}
    void* operator new(size_t) = delete;
    void* operator new(size_t,size_t) = delete;
    void* operator new[](size_t) = delete;
};

class QTJAMBI_EXPORT AbstractPersistentPointerArray{
public:
    ~AbstractPersistentPointerArray();
protected:
    AbstractPersistentPointerArray(JNIEnv *env, jarray array, jsize size, bool isNewArray);
    jarray array() const;
    jarray array(JNIEnv *env) const;
    jsize size() const;
    bool isNewArray() const;
    operator bool() const;
    inline operator jobject() const {return array();}
    inline operator jvalue() const {
        jvalue v;
        v.l = array();
        return v;
    }
    QScopedPointer<struct PersistentPointerArrayPrivate> d;
};

template<typename JArray, bool isConst, typename CType>
class PointerArray<true, JArray, isConst, CType, true> : public AbstractPersistentPointerArray {
public:
    using ArrayType = std::conditional_t<isConst, std::add_const_t<CType>, CType>;
    JArray array() const {return static_cast<JArray>(AbstractPersistentPointerArray::array());}
    operator JArray() const {return array();}
    operator void*() const {return const_cast<void*>(reinterpret_cast<const void*>(m_pointer));}
    operator ArrayType*() const {return m_pointer;}
    ArrayType* pointer () const { return m_pointer; }
    Q_DISABLE_COPY(PointerArray)
    JArray array(JNIEnv *env) const {return static_cast<JArray>(AbstractPersistentPointerArray::array(env));}
    PointerArray(JNIEnv *env, ArrayType* pointer, jsize size)
        : PointerArray(env, pointer, QtJambiPrivate::findOrCreateArray(env, pointer, size)) {}

    template<typename T>
    PointerArray(JNIEnv *env, T* pointer, std::enable_if_t<QtJambiPrivate::is_compatible<std::remove_cv_t<T>,CType>(), jsize> size)
        : PointerArray(env, reinterpret_cast<ArrayType*>(pointer), size) {}
    ~PointerArray(){
        if constexpr(!isConst && !std::is_same_v<JArray,jobjectArray>){
            if(JniEnvironment env{100}){
                JArray a = array(env);
                if(a && this->isNewArray()){
                    (env->*QtJambiPrivate::jni_primitive_array_functions<JArray>::GetArrayRegion)(a, 0, this->size(), reinterpret_cast<typename QtJambiPrivate::jni_type<JArray>::ElementType *>(m_pointer));
                    JavaException::check(env QTJAMBI_STACKTRACEINFO );
                }
            }
        }
    }
protected:
    template<typename JObjectArray>
    PointerArray(JNIEnv *env, ArrayType* pointer, JObjectArray array, std::enable_if_t<std::is_same_v<JObjectArray,jobjectArray> && std::is_same_v<JArray,jobjectArray>, jsize> size)
        : AbstractPersistentPointerArray(env, array, size, true),
        m_pointer(pointer){}
private:
    ArrayType* m_pointer;
    inline PointerArray(JNIEnv *env, ArrayType* pointer, const QtJambiPrivate::PointerArrayInfo<JArray,true>& data)
        : AbstractPersistentPointerArray(env, data.array, data.size, data.isNewArray),
        m_pointer(pointer){}
    template<bool, bool, typename>
    friend class ObjectPointerArray;
};

template<typename JArray, bool isConst, typename CType>
class PointerArray<true,JArray,isConst, CType, false> : public AbstractPersistentPointerArray {
public:
    using ArrayType = std::conditional_t<isConst, std::add_const_t<CType>, CType>;
    JArray array() const {return static_cast<JArray>(AbstractPersistentPointerArray::array());}
    operator JArray() const {return array();}
    operator void*() const {return const_cast<void*>(reinterpret_cast<const void*>(m_pointer));}
    operator ArrayType*() const {return m_pointer;}
    ArrayType* pointer () const { return m_pointer; }
    Q_DISABLE_COPY(PointerArray)
    JArray array(JNIEnv *env) const {return static_cast<JArray>(AbstractPersistentPointerArray::array(env));}
    PointerArray(JNIEnv *env, ArrayType* pointer, jsize size)
        : PointerArray(env, pointer, QtJambiPrivate::createArray<JArray,ArrayType>(env, pointer, size)) {}

    ~PointerArray(){
        if constexpr(!std::is_same_v<JArray,jobjectArray>){
            if constexpr(isConst){
                if(JniEnvironment env{100}){
                    JArray a = array(env);
                    if(a){
                        (env->*QtJambiPrivate::jni_primitive_array_functions<JArray>::ReleaseArrayElements)(a, m_arrayElements, JNI_ABORT);
                        JavaException::check(env QTJAMBI_STACKTRACEINFO );
                    }
                }
            }else if constexpr(!isConst){
                if(JniEnvironment env{100}){
                    JArray a = array(env);
                    if(a){
                        for(size_t i=0; i<this->size(); ++i){
                            m_pointer[i] = m_arrayElements[i];
                        }
                        (env->*QtJambiPrivate::jni_primitive_array_functions<JArray>::ReleaseArrayElements)(a, m_arrayElements, JNI_OK);
                        JavaException::check(env QTJAMBI_STACKTRACEINFO );
                    }
                }
            }
        }
    }
protected:
    inline ArrayType* pointer () { return m_pointer; }
private:
    typename QtJambiPrivate::jni_type<JArray>::ElementType* m_arrayElements;
    bool m_isCopy;
    ArrayType* m_pointer;
    PointerArray(JNIEnv *env, ArrayType* pointer, const QtJambiPrivate::PointerArrayInfo<JArray,false>& data)
        : AbstractPersistentPointerArray(env, data.array, data.size, true),
        m_arrayElements(data.arrayElements),
        m_isCopy(data.isCopy),
        m_pointer(pointer){}
    template<bool, bool, typename>
    friend class ObjectPointerArray;
};

extern template class PointerArray<false,jbyteArray,false,jbyte,true>;
extern template class PointerArray<false,jshortArray,false,jshort,true>;
extern template class PointerArray<false,jintArray,false,jint,true>;
extern template class PointerArray<false,jlongArray,false,jlong,true>;
extern template class PointerArray<false,jcharArray,false,jchar,true>;
extern template class PointerArray<false,jfloatArray,false,jfloat,true>;
extern template class PointerArray<false,jdoubleArray,false,jdouble,true>;
extern template class PointerArray<false,jbooleanArray,false,jboolean,true>;
extern template class PointerArray<false,jbyteArray,true,jbyte,true>;
extern template class PointerArray<false,jshortArray,true,jshort,true>;
extern template class PointerArray<false,jintArray,true,jint,true>;
extern template class PointerArray<false,jlongArray,true,jlong,true>;
extern template class PointerArray<false,jcharArray,true,jchar,true>;
extern template class PointerArray<false,jfloatArray,true,jfloat,true>;
extern template class PointerArray<false,jdoubleArray,true,jdouble,true>;
extern template class PointerArray<false,jbooleanArray,true,jboolean,true>;

extern template class PointerArray<true,jbyteArray,false,jbyte,true>;
extern template class PointerArray<true,jshortArray,false,jshort,true>;
extern template class PointerArray<true,jintArray,false,jint,true>;
extern template class PointerArray<true,jlongArray,false,jlong,true>;
extern template class PointerArray<true,jcharArray,false,jchar,true>;
extern template class PointerArray<true,jfloatArray,false,jfloat,true>;
extern template class PointerArray<true,jdoubleArray,false,jdouble,true>;
extern template class PointerArray<true,jbooleanArray,false,jboolean,true>;
extern template class PointerArray<true,jbyteArray,true,jbyte,true>;
extern template class PointerArray<true,jshortArray,true,jshort,true>;
extern template class PointerArray<true,jintArray,true,jint,true>;
extern template class PointerArray<true,jlongArray,true,jlong,true>;
extern template class PointerArray<true,jcharArray,true,jchar,true>;
extern template class PointerArray<true,jfloatArray,true,jfloat,true>;
extern template class PointerArray<true,jdoubleArray,true,jdouble,true>;
extern template class PointerArray<true,jbooleanArray,true,jboolean,true>;

template<bool persistent, bool isConst, typename T>
class ObjectPointerArray;

template<typename T>
class ObjectPointerArray<false,false,T> : public PointerArray<false,jobjectArray,false,T,true>
{
public:
    ObjectPointerArray(JNIEnv *env, T* pointer, jsize _size,
                       const char* javaClass,
                       std::function<jobject(JNIEnv *,const T&)> getter,
                       std::function<void(T&,JNIEnv *,jobject)> setter)
        : PointerArray<false,jobjectArray,false,T,true>(env, pointer, QtJambiAPI::createObjectArray(env, javaClass, pointer ? _size : 0), pointer ? _size : 0),
        m_setter(setter)
    {
        if(pointer){
            JavaException::check(env QTJAMBI_STACKTRACEINFO );
            for(jsize i=0; i<this->size(); ++i){
                env->SetObjectArrayElement(this->array(), i, getter(env, pointer[i]));
            }
            JavaException::check(env QTJAMBI_STACKTRACEINFO );
        }
    }

    ObjectPointerArray(JNIEnv *env, T* pointer, jsize _size,
                       std::function<jobject(JNIEnv *,const T&)> getter,
                       std::function<void(T&,JNIEnv *,jobject)> setter)
        : PointerArray<false,jobjectArray,false,T,true>(env, pointer, QtJambiAPI::createObjectArray(env, typeid(std::remove_pointer_t<T>), pointer ? _size : 0), pointer ? _size : 0), m_setter(setter) {
        if(pointer){
            JavaException::check(env QTJAMBI_STACKTRACEINFO );
            for(jsize i=0; i<this->size(); ++i){
                env->SetObjectArrayElement(this->array(), i, getter(env, pointer[i]));
            }
            JavaException::check(env QTJAMBI_STACKTRACEINFO );
        }
    }
    ~ObjectPointerArray(){
        if(this->array()){
            for(jsize i=0; i<this->size(); ++i){
                m_setter(this->pointer()[i], this->m_env, this->m_env->GetObjectArrayElement(this->array(), i));
            }
        }
    }
private:
    std::function<void(T&,JNIEnv *,jobject)> m_setter;
private:
    void* operator new(size_t) = delete;
    void* operator new(size_t,size_t) = delete;
    void* operator new[](size_t) = delete;
};

template<typename T>
class ObjectPointerArray<false,true,T> : public PointerArray<false,jobjectArray,true,T,true>
{
public:
    ObjectPointerArray(JNIEnv *env, const T* pointer, jsize _size,
                            const char* javaClass,
                            std::function<jobject(JNIEnv *,const T&)> getter)
        : PointerArray<false,jobjectArray,true,T,true>(env, pointer, QtJambiAPI::createObjectArray(env, javaClass, pointer ? _size : 0), pointer ? _size : 0)
    {
        if(pointer){
            JavaException::check(env QTJAMBI_STACKTRACEINFO );
            for(jsize i=0; i<this->size(); ++i){
                env->SetObjectArrayElement(this->array(), i, getter(env, pointer[i]));
            }
            JavaException::check(env QTJAMBI_STACKTRACEINFO );
        }
    }
    ObjectPointerArray(JNIEnv *env, const T* pointer, jsize _size,
                            std::function<jobject(JNIEnv *,const T&)> getter)
        : PointerArray<false,jobjectArray,true,T,true>(env, pointer, QtJambiAPI::createObjectArray(env, typeid(std::remove_pointer_t<T>), pointer ? _size : 0), pointer ? _size : 0)
    {
        if(pointer){
            JavaException::check(env QTJAMBI_STACKTRACEINFO );
            for(jsize i=0; i<this->size(); ++i){
                env->SetObjectArrayElement(this->array(), i, getter(env, pointer[i]));
            }
            JavaException::check(env QTJAMBI_STACKTRACEINFO );
        }
    }

private:
    void* operator new(size_t) = delete;
    void* operator new(size_t,size_t) = delete;
    void* operator new[](size_t) = delete;
};

template<typename T>
class ObjectPointerArray<true,false,T> : public PointerArray<true,jobjectArray,false,T,true>
{
public:
    ObjectPointerArray(JNIEnv *env, T* pointer, jsize _size,
                       const char* javaClass,
                       std::function<jobject(JNIEnv *,const T&)> getter,
                       std::function<void(T&,JNIEnv *,jobject)> setter)
        : PointerArray<true,jobjectArray,false,T,true>(env, pointer, QtJambiAPI::createObjectArray(env, javaClass, pointer ? _size : 0), pointer ? _size : 0),
        m_setter(setter)
    {
        if(pointer && this->isNewArray()){
            JavaException::check(env QTJAMBI_STACKTRACEINFO );
            for(jsize i=0; i<this->size(); ++i){
                env->SetObjectArrayElement(this->array(), i, getter(env, pointer[i]));
            }
            JavaException::check(env QTJAMBI_STACKTRACEINFO );
        }
    }

    ObjectPointerArray(JNIEnv *env, T* pointer, jsize _size,
                       std::function<jobject(JNIEnv *,const T&)> getter,
                       std::function<void(T&,JNIEnv *,jobject)> setter)
        : PointerArray<true,jobjectArray,false,T,true>(env, pointer, QtJambiAPI::createObjectArray(env, typeid(std::remove_pointer_t<T>), pointer ? _size : 0), pointer ? _size : 0), m_setter(setter) {
        if(pointer && this->isNewArray()){
            JavaException::check(env QTJAMBI_STACKTRACEINFO );
            for(jsize i=0; i<this->size(); ++i){
                env->SetObjectArrayElement(this->array(), i, getter(env, pointer[i]));
            }
            JavaException::check(env QTJAMBI_STACKTRACEINFO );
        }
    }

    ~ObjectPointerArray(){
        if(this->array() && this->isNewArray()){
            if(JniEnvironment env{300}){
                for(jsize i=0; i<this->size(); ++i){
                    m_setter(this->pointer()[i], env, env->GetObjectArrayElement(this->array(), i));
                }
            }
        }
    }
private:
    std::function<void(T&,JNIEnv *,jobject)> m_setter;
};

template<typename T>
class ObjectPointerArray<true,true,T> : public PointerArray<true,jobjectArray,true,T,true>
{
public:
    ObjectPointerArray(JNIEnv *env, const T* pointer, jsize _size,
                            const char* javaClass,
                            std::function<jobject(JNIEnv *,const T&)> getter)
        : PointerArray<true,jobjectArray,true,T,true>(env, pointer, QtJambiAPI::createObjectArray(env, javaClass, pointer ? _size : 0), pointer ? _size : 0)
    {
        if(pointer && this->isNewArray()){
            JavaException::check(env QTJAMBI_STACKTRACEINFO );
            for(jsize i=0; i<this->size(); ++i){
                env->SetObjectArrayElement(this->array(), i, getter(env, pointer[i]));
            }
            JavaException::check(env QTJAMBI_STACKTRACEINFO );
        }
    }

    ObjectPointerArray(JNIEnv *env, const T* pointer, jsize _size,
                            std::function<jobject(JNIEnv *,const T&)> getter)
        : PointerArray<true,jobjectArray,true,T,true>(env, pointer, QtJambiAPI::createObjectArray(env, typeid(std::remove_pointer_t<T>), pointer ? _size : 0), pointer ? _size : 0)
    {
        if(pointer && this->isNewArray()){
            JavaException::check(env QTJAMBI_STACKTRACEINFO );
            for(jsize i=0; i<this->size(); ++i){
                env->SetObjectArrayElement(this->array(), i, getter(env, pointer[i]));
            }
            JavaException::check(env QTJAMBI_STACKTRACEINFO );
        }
    }
};

namespace QtJambiPrivate {

template<typename JArray>
struct ElementForArray{

};

template<>
struct ElementForArray<jbyteArray>{
    typedef jbyte type;
};

template<>
struct ElementForArray<jshortArray>{
    typedef jshort type;
};

template<>
struct ElementForArray<jintArray>{
    typedef jint type;
};

template<>
struct ElementForArray<jlongArray>{
    typedef jlong type;
};

template<>
struct ElementForArray<jfloatArray>{
    typedef jfloat type;
};

template<>
struct ElementForArray<jdoubleArray>{
    typedef jdouble type;
};

template<>
struct ElementForArray<jcharArray>{
    typedef jchar type;
};

template<>
struct ElementForArray<jbooleanArray>{
    typedef bool type;
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
    bool isNull() const;
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

template<class Type, bool isJObject = QtJambiPrivate::is_jni_object_type_v<Type>>
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
        return QtJambiAPI::initializer_list<Type>(m_array_elements, m_size);
    }
    operator std::initializer_list<const Type> () const {
        return QtJambiAPI::initializer_list<const Type>(m_array_elements, m_size);
    }
    void* operator new(size_t) = delete;
    void* operator new(size_t,size_t) = delete;
    void* operator new[](size_t) = delete;
};

template<class JObject>
class JConstObjectArrayPointer<JObject,true> : public JArrayPointer<jobjectArray, JObject>
{
public:
    using JArrayPointer<jobjectArray, JObject>::size;
    using JArrayPointer<jobjectArray, JObject>::array;
    using JArrayPointer<jobjectArray, JObject>::m_size;
    using JArrayPointer<jobjectArray, JObject>::m_array;
    using JArrayPointer<jobjectArray, JObject>::m_env;
    using JArrayPointer<jobjectArray, JObject>::m_array_elements;
    using JArrayPointer<jobjectArray, JObject>::m_is_copy;

    JConstObjectArrayPointer(JNIEnv *env, jobjectArray array, std::function<void(jobject&,JNIEnv *,JObject)> = {})
        : JArrayPointer<jobjectArray, JObject>(env, array)
    {
        if(m_array && m_size>0){
            m_is_copy = true;
        }
    }

    ~JConstObjectArrayPointer() {
    }

    inline JObject operator[](jsize index) const{
        return JObject(m_env->GetObjectArrayElement(m_array, index));
    }
    void* operator new(size_t) = delete;
    void* operator new(size_t,size_t) = delete;
    void* operator new[](size_t) = delete;
};

template<class Type, bool isJObject = QtJambiPrivate::is_jni_object_type_v<Type>>
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
        return QtJambiAPI::initializer_list<Type>(m_array_elements, m_size);
    }
    operator std::initializer_list<const Type> () const {
        return QtJambiAPI::initializer_list<const Type>(m_array_elements, m_size);
    }
    void* operator new(size_t) = delete;
    void* operator new(size_t,size_t) = delete;
    void* operator new[](size_t) = delete;
private:
    std::function<jobject(JNIEnv *,const Type&)> m_getter;
};

template<class JObject>
class JObjectArrayPointer<JObject,true> : public JArrayPointer<jobjectArray, JObject>
{
public:
    using JArrayPointer<jobjectArray, JObject>::size;
    using JArrayPointer<jobjectArray, JObject>::array;
    using JArrayPointer<jobjectArray, JObject>::m_size;
    using JArrayPointer<jobjectArray, JObject>::m_array;
    using JArrayPointer<jobjectArray, JObject>::m_is_copy;
    using JArrayPointer<jobjectArray, JObject>::m_env;

    JObjectArrayPointer(JNIEnv *env, jobjectArray array, std::function<void(JObject&,JNIEnv *,jobject)> = {}, std::function<jobject(JNIEnv *,const JObject&)> = {})
        : JArrayPointer<jobjectArray, JObject>(env, array)
    {
        if(m_array && m_size>0){
            m_is_copy = true;
        }
    }

    ~JObjectArrayPointer() {
    }

    inline jobject operator[](jsize index) const{
        return m_env->GetObjectArrayElement(m_array, index);
    }

    inline auto operator[](jsize index) {
        struct jobjectRef{
            JObjectArrayPointer<JObject,true> *_this;
            jsize index;
            operator JObject() const{
                return JObject(_this->m_env->GetObjectArrayElement(_this->m_array, index));
            }
            jobjectRef& operator=(JObject obj){
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

template<class Type, bool isJObject = QtJambiPrivate::is_jni_object_type_v<Type>>
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
        return QtJambiAPI::initializer_list<Type>(m_array_elements, size());
    }
    operator std::initializer_list<const Type> () const {
        return QtJambiAPI::initializer_list<const Type>(m_array_elements, size());
    }
};

template<class Type, bool isJObject = QtJambiPrivate::is_jni_object_type_v<Type>>
class PersistentJObjectArrayPointer : public PersistentJArrayPointer<jobjectArray, Type>
{
public:
    using PersistentJArrayPointer<jobjectArray, Type>::size;
    using PersistentJArrayPointer<jobjectArray, Type>::array;
    using PersistentJArrayPointer<jobjectArray, Type>::isNull;
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
        if(!isNull()){
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
        return QtJambiAPI::initializer_list<Type>(m_array_elements, size());
    }
    operator std::initializer_list<const Type> () const {
        return QtJambiAPI::initializer_list<const Type>(m_array_elements, size());
    }
private:
    std::function<jobject(JNIEnv *,const Type&)> m_getter;
};

template<class JObject>
class PersistentJConstObjectArrayPointer<JObject,true> : public PersistentJArrayPointer<jobjectArray, JObject>
{
public:
    using PersistentJArrayPointer<jobjectArray, JObject>::size;
    using PersistentJArrayPointer<jobjectArray, JObject>::array;
    using PersistentJArrayPointer<jobjectArray, JObject>::m_array_elements;
    using PersistentJArrayPointer<jobjectArray, JObject>::m_is_copy;

    PersistentJConstObjectArrayPointer(JNIEnv *env, jobjectArray array, std::function<void(JObject&,JNIEnv *,jobject)> = {})
        : PersistentJArrayPointer<jobjectArray, JObject>(env, array)
    {
    }

    ~PersistentJConstObjectArrayPointer() {
    }
    inline jobject operator[](jsize index) const{
        if(JniEnvironment env{}){
            env->PushLocalFrame(128);
            return JObject(env->PopLocalFrame(env->GetObjectArrayElement(array(env), index)));
        }else return nullptr;
    }
};

template<class JObject>
class PersistentJObjectArrayPointer<JObject,true> : public PersistentJArrayPointer<jobjectArray, JObject>
{
public:
    using PersistentJArrayPointer<jobjectArray, JObject>::size;
    using PersistentJArrayPointer<jobjectArray, JObject>::array;
    using PersistentJArrayPointer<jobjectArray, JObject>::isNull;
    using PersistentJArrayPointer<jobjectArray, JObject>::m_array_elements;
    using PersistentJArrayPointer<jobjectArray, JObject>::m_is_copy;

    PersistentJObjectArrayPointer(JNIEnv *env, jobjectArray array, std::function<void(JObject&,JNIEnv *,jobject)> = {}, std::function<jobject(JNIEnv *,const JObject&)> = {})
        : PersistentJArrayPointer<jobjectArray, JObject>(env, array)
    {
    }

    ~PersistentJObjectArrayPointer() {
    }
    inline JObject operator[](jsize index) const{
        if(JniEnvironment env{}){
            env->PushLocalFrame(128);
            return JObject(env->PopLocalFrame(env->GetObjectArrayElement(array(env), index)));
        }else return nullptr;
    }

    inline auto operator[](jsize index) {
        struct jobjectRef{
            PersistentJObjectArrayPointer<JObject,true> *_this;
            jsize index;
            operator JObject() const{
                if(JniEnvironment env{}){
                    env->PushLocalFrame(128);
                    return JObject(env->PopLocalFrame(env->GetObjectArrayElement(array(env), index)));
                }else return nullptr;
            }
            jobjectRef& operator=(JObject obj){
                if(JniEnvironment env{128}){
                    env->SetObjectArrayElement(array(env), index, obj);
                }
                return *this;
            }
        };
        return jobjectRef{this, index};
    }
};

#endif // QTJAMBI_JAVAARRAYS_H
