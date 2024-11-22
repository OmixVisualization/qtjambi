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

#include <QtCore/qcompilerdetection.h>
QT_WARNING_DISABLE_DEPRECATED

#include <QtCore/QCoreApplication>
#include "qtjambiapi.h"
#include "jobjectwrapper.h"
#include "java_p.h"
#include "registryutil_p.h"
#include "typemanager_p.h"
#include "qtjambi_cast.h"

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
QT_WARNING_DISABLE_GCC("-Wdeprecated-declarations")
QT_WARNING_DISABLE_DEPRECATED

template<>
inline bool qMapLessThanKey<QVariant>(const QVariant& v1, const QVariant& v2){
    return v1 < v2;
}
#endif

class JObjectWrapperData : public QSharedData{
protected:
    JObjectWrapperData() = default;
public:
    virtual ~JObjectWrapperData() = default;
    virtual void clear(JNIEnv *env) = 0;
    virtual jobject data() const = 0;
    virtual jobject data(JNIEnv *env) const = 0;
    virtual const void* array() const = 0;
    virtual void* array() = 0;
    virtual void commitArray() = 0;
    virtual jsize arrayLength() const = 0;
};

template<typename Cleanup>
void reference_cleanup(jobject object){
    try{
        if(object && !QCoreApplication::closingDown()){
            if(DefaultJniEnvironment env{0}){
                jthrowable throwable = nullptr;
                if(env->ExceptionCheck()){
                    throwable = env->ExceptionOccurred();
                    env->ExceptionClear();
                }
                (env->*Cleanup::DeleteRef)(typename Cleanup::RefType(object));
                if(throwable){
                    env->Throw(throwable);
                    env->DeleteLocalRef(throwable);
                }
            }
        }
    }catch(...){}
}

struct JObjectGlobalWrapperUtil{
    constexpr static auto NewRef = &JNIEnv::NewGlobalRef;
    constexpr static auto DeleteRef = &JNIEnv::DeleteGlobalRef;
    typedef jobject RefType;
    constexpr static void(*cleanup)(jobject) = &reference_cleanup<JObjectGlobalWrapperUtil>;
    constexpr static auto PushRef = &Java::QtJambi::NativeUtility::pushGlobalReference;
    constexpr static auto GetRef = &Java::QtJambi::NativeUtility::getGlobalReference;
    constexpr static auto ReleaseRef = &Java::QtJambi::NativeUtility::releaseGlobalReference;
};

struct JObjectWeakWrapperUtil{
    constexpr static auto NewRef = &JNIEnv::NewWeakGlobalRef;
    constexpr static auto DeleteRef = &JNIEnv::DeleteWeakGlobalRef;
    typedef jweak RefType;
    constexpr static void(*cleanup)(jobject) = &reference_cleanup<JObjectWeakWrapperUtil>;
    constexpr static auto PushRef = &Java::QtJambi::NativeUtility::pushWeakGlobalReference;
    constexpr static auto GetRef = &Java::QtJambi::NativeUtility::getWeakGlobalReference;
    constexpr static auto ReleaseRef = &Java::QtJambi::NativeUtility::releaseWeakGlobalReference;
};

template<typename Cleanup>
class JObjectRefWrapperData : public JObjectWrapperData{
public:
    JObjectRefWrapperData() = default;
    JObjectRefWrapperData(JNIEnv* env, jobject object)
        : JObjectWrapperData()
        , pointer( (env->*Cleanup::NewRef)(object) ){}

    ~JObjectRefWrapperData() override{
    }
    void clear(JNIEnv *env) override{
        if(pointer.data()){
            jthrowable throwable = nullptr;
            if(env->ExceptionCheck()){
                throwable = env->ExceptionOccurred();
                env->ExceptionClear();
            }
            (env->*Cleanup::DeleteRef)(pointer.take());
            if(throwable){
                env->Throw(throwable);
                env->DeleteLocalRef(throwable);
            }
        }
    }

    jobject data() const override {
        return pointer.data();
    }

    jobject data(JNIEnv *env) const override {
        return env->NewLocalRef(pointer.data());
    }
    const void* array() const override {return nullptr;}
    void* array() override {return nullptr;}
    void commitArray() override {}
    jsize arrayLength() const override {return 0;}
private:
    QScopedPointer<_jobject, Cleanup> pointer;
};

typedef JObjectRefWrapperData<JObjectGlobalWrapperUtil> JObjectGlobalWrapperData;
typedef JObjectRefWrapperData<JObjectWeakWrapperUtil> JObjectWeakWrapperData;

template<typename Cleanup>
class JObjectRefViaJavaWrapperData : public JObjectWrapperData{
public:
    JObjectRefViaJavaWrapperData() = default;
    JObjectRefViaJavaWrapperData(JNIEnv* env, jobject object)
        : JObjectWrapperData()
    {
        Cleanup::PushRef(env, jlong(reinterpret_cast<void*>(this)), object);
    }

    ~JObjectRefViaJavaWrapperData() override{
        try{
            if(!QCoreApplication::closingDown()){
                if(DefaultJniEnvironment env{0}){
                    JObjectRefViaJavaWrapperData<Cleanup>::clear(env);
                }
            }
        }catch(...){}
    }
    void clear(JNIEnv *env) override{
        jthrowable throwable = nullptr;
        if(env->ExceptionCheck()){
            throwable = env->ExceptionOccurred();
            env->ExceptionClear();
        }
        Cleanup::ReleaseRef(env, jlong(reinterpret_cast<void*>(this)));
        if(throwable){
            env->Throw(throwable);
            env->DeleteLocalRef(throwable);
        }
    }

    jobject data() const override {
        if(DefaultJniEnvironment env{0}){
            return Cleanup::GetRef(env, jlong(reinterpret_cast<const void*>(this)));
        }
        return nullptr;
    }

    jobject data(JNIEnv *env) const override {
        return Cleanup::GetRef(env, jlong(reinterpret_cast<const void*>(this)));
    }
    const void* array() const override {return nullptr;}
    void* array() override {return nullptr;}
    void commitArray() override {}
    jsize arrayLength() const override {return 0;}
};

typedef JObjectRefViaJavaWrapperData<JObjectGlobalWrapperUtil> JObjectGlobalViaJavaWrapperData;
typedef JObjectRefViaJavaWrapperData<JObjectWeakWrapperUtil> JObjectWeakViaJavaWrapperData;

template<typename JType, typename Cleanup>
class JArrayWrapperData : public JObjectWrapperData{
protected:
    typedef typename JArray<JType>::Type ArrayType;
public:
    JArrayWrapperData() = default;
    JArrayWrapperData(JNIEnv* env, ArrayType object)
        :
          pointer( (env->*Cleanup::NewRef)(object) ),
          m_isCopy(false),
          m_length( object ? env->GetArrayLength(object) : 0 ),
          m_array( object ? (env->*JArray<JType>::GetArrayElements)(object, &m_isCopy) : nullptr )
    {
    }

    ~JArrayWrapperData() override{
        try{
            if(DefaultJniEnvironment env{500}){
                JArrayWrapperData<JType,Cleanup>::clear(env);
            }
        } catch (const std::exception& e) {
            qCWarning(DebugAPI::internalCategory, "%s", e.what());
        }catch(...){}
    }
    void clear(JNIEnv *env) override{
        ArrayType array = JArrayWrapperData<JType,Cleanup>::data(env);
        if(array){
            (env->*JArray<JType>::ReleaseArrayElements)(array, m_array, 0);
            m_array = nullptr;
            m_length = 0;
            jthrowable throwable = nullptr;
            if(env->ExceptionCheck()){
                throwable = env->ExceptionOccurred();
                env->ExceptionClear();
            }
            (env->*Cleanup::DeleteRef)(pointer.take());
            env->DeleteLocalRef(array);
            if(throwable)
                env->Throw(throwable);
        }
    }

    ArrayType data(JNIEnv* env) const override {
        return ArrayType(env->NewLocalRef(pointer.data()));
    }

    ArrayType data() const override {
        return ArrayType(pointer.data());
    }
    const void* array() const override  {return m_array;}
    void* array() override  {return m_array;}
    jsize arrayLength() const override {return m_length;}
    void commitArray() override {
        if(m_isCopy){
            if(JniEnvironment env{500}){
                ArrayType array = JArrayWrapperData<JType,Cleanup>::data(env);
                if(array){
                    (env->*JArray<JType>::ReleaseArrayElements)(array, m_array, 0);
                    m_array = (env->*JArray<JType>::GetArrayElements)(array, &m_isCopy);
                }
            }
        }
    }
private:
    QScopedPointer<_jobject, Cleanup> pointer;
    jboolean m_isCopy = false;
    jsize m_length = 0;
    JType* m_array = nullptr;
};

template<typename JType, typename Cleanup>
class JArrayViaJavaWrapperData : public JObjectWrapperData{
protected:
    typedef typename JArray<JType>::Type ArrayType;
public:
    JArrayViaJavaWrapperData() = default;
    JArrayViaJavaWrapperData(JNIEnv* env, ArrayType object)
        :
        m_isCopy(false),
        m_length( object ? env->GetArrayLength(object) : 0 ),
        m_array( object ? (env->*JArray<JType>::GetArrayElements)(object, &m_isCopy) : nullptr )
    {
        Cleanup::PushRef(env, jlong(reinterpret_cast<void*>(this)), object);
    }

    ~JArrayViaJavaWrapperData() override{
        try{
            if(DefaultJniEnvironment env{500}){
                JArrayViaJavaWrapperData<JType,Cleanup>::clear(env);
            }
        } catch (const std::exception& e) {
            qCWarning(DebugAPI::internalCategory, "%s", e.what());
        }catch(...){}
    }
    void clear(JNIEnv *env) override{
        ArrayType array = JArrayViaJavaWrapperData<JType,Cleanup>::data(env);
        if(array){
            (env->*JArray<JType>::ReleaseArrayElements)(array, m_array, 0);
            m_array = nullptr;
            m_length = 0;
            jthrowable throwable = nullptr;
            if(env->ExceptionCheck()){
                throwable = env->ExceptionOccurred();
                env->ExceptionClear();
            }
            Cleanup::ReleaseRef(env, jlong(reinterpret_cast<void*>(this)));
            env->DeleteLocalRef(array);
            if(throwable)
                env->Throw(throwable);
        }
    }

    ArrayType data(JNIEnv* env) const override {
        return ArrayType(Cleanup::GetRef(env, jlong(reinterpret_cast<const void*>(this))));
    }

    ArrayType data() const override {
        if(DefaultJniEnvironment env{0}){
            return data(env);
        }
        return nullptr;
    }
    const void* array() const override  {return m_array;}
    void* array() override  {return m_array;}
    jsize arrayLength() const override {return m_length;}
    void commitArray() override {
        if(m_isCopy){
            if(JniEnvironment env{500}){
                ArrayType array = JArrayViaJavaWrapperData<JType,Cleanup>::data(env);
                if(array){
                    (env->*JArray<JType>::ReleaseArrayElements)(array, m_array, 0);
                    m_array = (env->*JArray<JType>::GetArrayElements)(array, &m_isCopy);
                }
            }
        }
    }
private:
    jboolean m_isCopy = false;
    jsize m_length = 0;
    JType* m_array = nullptr;
};

template<typename JType>
struct JArrayGlobalWrapperData : JArrayWrapperData<JType,JObjectGlobalWrapperUtil>{
    JArrayGlobalWrapperData() = default;
    JArrayGlobalWrapperData(JNIEnv* env, typename JArrayWrapperData<JType,JObjectGlobalWrapperUtil>::ArrayType object) : JArrayWrapperData<JType,JObjectGlobalWrapperUtil>(env, object){}
};

template<typename JType>
struct JArrayWeakWrapperData : JArrayWrapperData<JType,JObjectWeakWrapperUtil>{
    JArrayWeakWrapperData() = default;
    JArrayWeakWrapperData(JNIEnv* env, typename JArrayWrapperData<JType,JObjectWeakWrapperUtil>::ArrayType object) : JArrayWrapperData<JType,JObjectWeakWrapperUtil>(env, object){}
};

template<typename JType>
struct JArrayGlobalViaJavaWrapperData : JArrayViaJavaWrapperData<JType,JObjectGlobalWrapperUtil>{
    JArrayGlobalViaJavaWrapperData() = default;
    JArrayGlobalViaJavaWrapperData(JNIEnv* env, typename JArrayViaJavaWrapperData<JType,JObjectGlobalWrapperUtil>::ArrayType object) : JArrayViaJavaWrapperData<JType,JObjectGlobalWrapperUtil>(env, object){}
};

template<typename JType>
struct JArrayWeakViaJavaWrapperData : JArrayViaJavaWrapperData<JType,JObjectWeakWrapperUtil>{
    JArrayWeakViaJavaWrapperData() = default;
    JArrayWeakViaJavaWrapperData(JNIEnv* env, typename JArrayViaJavaWrapperData<JType,JObjectWeakWrapperUtil>::ArrayType object) : JArrayViaJavaWrapperData<JType,JObjectWeakWrapperUtil>(env, object){}
};

bool JObjectWrapper::compareEqual(JNIEnv *env, const JObjectWrapper &other) const{
    return (*this)==other.object(env);
}

bool JObjectWrapper::compareEqual(JNIEnv *env, jobject otherObject) const{
    jobject myObject = object(env);
    if (!myObject && !otherObject){
        return true;
    }else if (!myObject || !otherObject){
        return false;
    }else{
        Q_ASSERT(Java::Runtime::Object::isInstanceOf(env, myObject));  // check the java object is right type (objects on JVM don't have to inherit java.lang.Object)
        return Java::Runtime::Object::equals(env, myObject, otherObject);
    }
}

bool JObjectWrapper::operator==(const JObjectWrapper &other) const
{
    if(JniEnvironment env{500}){
        return compareEqual(env, other);
    }
    return false;
}

bool JObjectWrapper::operator==(jobject otherObject) const
{
    if(JniEnvironment env{500}){
        return compareEqual(env, otherObject);
    }
    return false;
}

bool JObjectWrapper::compareLess(JNIEnv *env, const JObjectWrapper &other) const{
    return (*this)<other.object(env);
}

bool JObjectWrapper::operator<(const JObjectWrapper &other) const
{
    if(JniEnvironment env{500}){
        return compareLess(env, other);
    }
    return false;
}

bool JObjectWrapper::compareLess(JNIEnv *env, jobject otherObject) const{
    jobject myObject = object(env);
    if (!myObject && !otherObject){
        return false;
    }else if (!myObject || !otherObject){
        return false;
    }else{
        if(Java::QtJambi::QFlags::isInstanceOf(env, myObject) && otherObject && env->IsInstanceOf(otherObject, env->GetObjectClass(myObject))){
            jint h1 = Java::QtJambi::QFlags::value(env, myObject);
            jint h2 = Java::QtJambi::QFlags::value(env, otherObject);
            return h1<h2;
        }else if(Java::QtJambi::QtEnumerator::isInstanceOf(env, myObject) && otherObject && env->IsInstanceOf(otherObject, env->GetObjectClass(myObject))){
            jint h1 = Java::QtJambi::QtEnumerator::value(env, myObject);
            jint h2 = Java::QtJambi::QtEnumerator::value(env, otherObject);
            return h1<h2;
        }else if(Java::QtJambi::QtShortEnumerator::isInstanceOf(env, myObject) && otherObject && env->IsInstanceOf(otherObject, env->GetObjectClass(myObject))){
            jshort h1 = Java::QtJambi::QtShortEnumerator::value(env, myObject);
            jshort h2 = Java::QtJambi::QtShortEnumerator::value(env, otherObject);
            return h1<h2;
        }else if(Java::QtJambi::QtByteEnumerator::isInstanceOf(env, myObject) && otherObject && env->IsInstanceOf(otherObject, env->GetObjectClass(myObject))){
            jbyte h1 = Java::QtJambi::QtByteEnumerator::value(env, myObject);
            jbyte h2 = Java::QtJambi::QtByteEnumerator::value(env, otherObject);
            return h1<h2;
        }else if(Java::QtJambi::QtLongEnumerator::isInstanceOf(env, myObject) && otherObject && env->IsInstanceOf(otherObject, env->GetObjectClass(myObject))){
            jlong h1 = Java::QtJambi::QtLongEnumerator::value(env, myObject);
            jlong h2 = Java::QtJambi::QtLongEnumerator::value(env, otherObject);
            return h1<h2;
        }else if(Java::Runtime::Enum::isInstanceOf(env, myObject) && otherObject && env->IsInstanceOf(otherObject, env->GetObjectClass(myObject))){
            jint h1 = Java::Runtime::Enum::ordinal(env, myObject);
            jint h2 = Java::Runtime::Enum::ordinal(env, otherObject);
            return h1<h2;
        }
        if(Java::Runtime::Comparable::isInstanceOf(env, myObject)){
            try{
                return Java::Runtime::Comparable::compareTo(env, myObject, otherObject)<0;
            }catch(const JavaException&){
            }
        }
        jint h1 = Java::Runtime::Object::hashCode(env, myObject);
        jint h2 = Java::Runtime::Object::hashCode(env, otherObject);
        return h1<h2;
    }
}

bool JObjectWrapper::operator<(jobject otherObject) const
{
    if(JniEnvironment env{200}){
        return compareLess(env, otherObject);
    }
    return false;
}

JObjectWrapper::JObjectWrapper() : m_data()
{
}

JObjectWrapper::JObjectWrapper(const JObjectWrapper &wrapper) : m_data(wrapper.m_data)
{
}

JObjectWrapper::JObjectWrapper(JObjectWrapper &&wrapper) : m_data(std::move(wrapper.m_data))
{
}

JObjectWrapper::JObjectWrapper(jobject obj)
    : m_data()
{
    if(obj){
        if(JniEnvironment env{500}){
            if(env->ExceptionCheck()){
                env->ExceptionDescribe();
                env->ExceptionClear();
            }
            if(!env->IsSameObject(obj, nullptr)){
                m_data = static_cast<JObjectWrapperData*>(new JObjectGlobalWrapperData(env, obj));
            }
        }
    }
}

JObjectWrapper::JObjectWrapper(JNIEnv *env, jobject obj, bool globalRefs)
    : m_data()
{
    if(env->ExceptionCheck()){
        env->ExceptionDescribe();
        env->ExceptionClear();
    }
    if(!env->IsSameObject(obj, nullptr)){
        if(avoidJNIGlobalRefs(env)){
            m_data = globalRefs ? static_cast<JObjectWrapperData*>(new JObjectGlobalViaJavaWrapperData(env, obj)) : static_cast<JObjectWrapperData*>(new JObjectWeakViaJavaWrapperData(env, obj));
        }else{
            m_data = globalRefs ? static_cast<JObjectWrapperData*>(new JObjectGlobalWrapperData(env, obj)) : static_cast<JObjectWrapperData*>(new JObjectWeakWrapperData(env, obj));
        }
    }
}

JObjectWrapper::JObjectWrapper(JNIEnv *env, jobject obj, bool globalRefs, const std::type_info& typeId)
 : m_data()
{
    if(env->ExceptionCheck()){
        env->ExceptionDescribe();
        env->ExceptionClear();
    }
    if(!env->IsSameObject(obj, nullptr)){
        if(avoidJNIGlobalRefs(env)){
            if(globalRefs){
                if(typeid_equals(typeId, typeid(jint))){
                    m_data = static_cast<JObjectWrapperData*>(new JArrayGlobalViaJavaWrapperData<jint>(env, jintArray(obj)));
                }else if(typeid_equals(typeId, typeid(jbyte))){
                    m_data = static_cast<JObjectWrapperData*>(new JArrayGlobalViaJavaWrapperData<jbyte>(env, jbyteArray(obj)));
                }else if(typeid_equals(typeId, typeid(jshort))){
                    m_data = static_cast<JObjectWrapperData*>(new JArrayGlobalViaJavaWrapperData<jshort>(env, jshortArray(obj)));
                }else if(typeid_equals(typeId, typeid(jlong))){
                    m_data = static_cast<JObjectWrapperData*>(new JArrayGlobalViaJavaWrapperData<jlong>(env, jlongArray(obj)));
                }else if(typeid_equals(typeId, typeid(jchar))){
                    m_data = static_cast<JObjectWrapperData*>(new JArrayGlobalViaJavaWrapperData<jchar>(env, jcharArray(obj)));
                }else if(typeid_equals(typeId, typeid(jboolean))){
                    m_data = static_cast<JObjectWrapperData*>(new JArrayGlobalViaJavaWrapperData<jboolean>(env, jbooleanArray(obj)));
                }else if(typeid_equals(typeId, typeid(jfloat))){
                    m_data = static_cast<JObjectWrapperData*>(new JArrayGlobalViaJavaWrapperData<jfloat>(env, jfloatArray(obj)));
                }else if(typeid_equals(typeId, typeid(jdouble))){
                    m_data = static_cast<JObjectWrapperData*>(new JArrayGlobalViaJavaWrapperData<jdouble>(env, jdoubleArray(obj)));
                }
            }else{
                if(typeid_equals(typeId, typeid(jint))){
                    m_data = static_cast<JObjectWrapperData*>(new JArrayWeakViaJavaWrapperData<jint>(env, jintArray(obj)));
                }else if(typeid_equals(typeId, typeid(jbyte))){
                    m_data = static_cast<JObjectWrapperData*>(new JArrayWeakViaJavaWrapperData<jbyte>(env, jbyteArray(obj)));
                }else if(typeid_equals(typeId, typeid(jshort))){
                    m_data = static_cast<JObjectWrapperData*>(new JArrayWeakViaJavaWrapperData<jshort>(env, jshortArray(obj)));
                }else if(typeid_equals(typeId, typeid(jlong))){
                    m_data = static_cast<JObjectWrapperData*>(new JArrayWeakViaJavaWrapperData<jlong>(env, jlongArray(obj)));
                }else if(typeid_equals(typeId, typeid(jchar))){
                    m_data = static_cast<JObjectWrapperData*>(new JArrayWeakViaJavaWrapperData<jchar>(env, jcharArray(obj)));
                }else if(typeid_equals(typeId, typeid(jboolean))){
                    m_data = static_cast<JObjectWrapperData*>(new JArrayWeakViaJavaWrapperData<jboolean>(env, jbooleanArray(obj)));
                }else if(typeid_equals(typeId, typeid(jfloat))){
                    m_data = static_cast<JObjectWrapperData*>(new JArrayWeakViaJavaWrapperData<jfloat>(env, jfloatArray(obj)));
                }else if(typeid_equals(typeId, typeid(jdouble))){
                    m_data = static_cast<JObjectWrapperData*>(new JArrayWeakViaJavaWrapperData<jdouble>(env, jdoubleArray(obj)));
                }
            }
        }else{
            if(globalRefs){
                if(typeid_equals(typeId, typeid(jint))){
                    m_data = static_cast<JObjectWrapperData*>(new JArrayGlobalWrapperData<jint>(env, jintArray(obj)));
                }else if(typeid_equals(typeId, typeid(jbyte))){
                    m_data = static_cast<JObjectWrapperData*>(new JArrayGlobalWrapperData<jbyte>(env, jbyteArray(obj)));
                }else if(typeid_equals(typeId, typeid(jshort))){
                    m_data = static_cast<JObjectWrapperData*>(new JArrayGlobalWrapperData<jshort>(env, jshortArray(obj)));
                }else if(typeid_equals(typeId, typeid(jlong))){
                    m_data = static_cast<JObjectWrapperData*>(new JArrayGlobalWrapperData<jlong>(env, jlongArray(obj)));
                }else if(typeid_equals(typeId, typeid(jchar))){
                    m_data = static_cast<JObjectWrapperData*>(new JArrayGlobalWrapperData<jchar>(env, jcharArray(obj)));
                }else if(typeid_equals(typeId, typeid(jboolean))){
                    m_data = static_cast<JObjectWrapperData*>(new JArrayGlobalWrapperData<jboolean>(env, jbooleanArray(obj)));
                }else if(typeid_equals(typeId, typeid(jfloat))){
                    m_data = static_cast<JObjectWrapperData*>(new JArrayGlobalWrapperData<jfloat>(env, jfloatArray(obj)));
                }else if(typeid_equals(typeId, typeid(jdouble))){
                    m_data = static_cast<JObjectWrapperData*>(new JArrayGlobalWrapperData<jdouble>(env, jdoubleArray(obj)));
                }
            }else{
                if(typeid_equals(typeId, typeid(jint))){
                    m_data = static_cast<JObjectWrapperData*>(new JArrayWeakWrapperData<jint>(env, jintArray(obj)));
                }else if(typeid_equals(typeId, typeid(jbyte))){
                    m_data = static_cast<JObjectWrapperData*>(new JArrayWeakWrapperData<jbyte>(env, jbyteArray(obj)));
                }else if(typeid_equals(typeId, typeid(jshort))){
                    m_data = static_cast<JObjectWrapperData*>(new JArrayWeakWrapperData<jshort>(env, jshortArray(obj)));
                }else if(typeid_equals(typeId, typeid(jlong))){
                    m_data = static_cast<JObjectWrapperData*>(new JArrayWeakWrapperData<jlong>(env, jlongArray(obj)));
                }else if(typeid_equals(typeId, typeid(jchar))){
                    m_data = static_cast<JObjectWrapperData*>(new JArrayWeakWrapperData<jchar>(env, jcharArray(obj)));
                }else if(typeid_equals(typeId, typeid(jboolean))){
                    m_data = static_cast<JObjectWrapperData*>(new JArrayWeakWrapperData<jboolean>(env, jbooleanArray(obj)));
                }else if(typeid_equals(typeId, typeid(jfloat))){
                    m_data = static_cast<JObjectWrapperData*>(new JArrayWeakWrapperData<jfloat>(env, jfloatArray(obj)));
                }else if(typeid_equals(typeId, typeid(jdouble))){
                    m_data = static_cast<JObjectWrapperData*>(new JArrayWeakWrapperData<jdouble>(env, jdoubleArray(obj)));
                }
            }
        }
    }
}

void JObjectWrapper::assign(JNIEnv* env, const JObjectWrapper& wrapper, const std::type_info& typeId)
{
    if(QExplicitlySharedDataPointer<JObjectWrapperData> wrappersData = QExplicitlySharedDataPointer<JObjectWrapperData>(wrapper.m_data)){
        JObjectWrapperData* wd = wrappersData.data();
        const std::type_info& dataTypeId = typeid(*wd);
        if(typeid_equals(typeId, typeid(jint))){
            if(typeid_equals(dataTypeId, typeid(JObjectGlobalWrapperData))){
                m_data = static_cast<JObjectWrapperData*>(new JArrayGlobalWrapperData<jint>(env, jintArray(wrapper.object(env))));
            }else if(typeid_equals(dataTypeId, typeid(JObjectWeakWrapperData))){
                m_data = static_cast<JObjectWrapperData*>(new JArrayWeakWrapperData<jint>(env, jintArray(wrapper.object(env))));
            }else if(typeid_equals(dataTypeId, typeid(JArrayGlobalWrapperData<jint>)) || typeid_equals(dataTypeId, typeid(JArrayWeakWrapperData<jint>))){
                m_data = wrappersData;
            }else{
                m_data.reset();
            }
        }else if(typeid_equals(typeId, typeid(jbyte))){
            if(typeid_equals(dataTypeId, typeid(JObjectGlobalWrapperData))){
                m_data = static_cast<JObjectWrapperData*>(new JArrayGlobalWrapperData<jbyte>(env, jbyteArray(wrapper.object(env))));
            }else if(typeid_equals(dataTypeId, typeid(JObjectWeakWrapperData))){
                m_data = static_cast<JObjectWrapperData*>(new JArrayWeakWrapperData<jbyte>(env, jbyteArray(wrapper.object(env))));
            }else if(typeid_equals(dataTypeId, typeid(JArrayGlobalWrapperData<jbyte>)) || typeid_equals(dataTypeId, typeid(JArrayWeakWrapperData<jbyte>))){
                m_data = wrappersData;
            }else{
                m_data.reset();
            }
        }else if(typeid_equals(typeId, typeid(jshort))){
            if(typeid_equals(dataTypeId, typeid(JObjectGlobalWrapperData))){
                m_data = static_cast<JObjectWrapperData*>(new JArrayGlobalWrapperData<jshort>(env, jshortArray(wrapper.object(env))));
            }else if(typeid_equals(dataTypeId, typeid(JObjectWeakWrapperData))){
                m_data = static_cast<JObjectWrapperData*>(new JArrayWeakWrapperData<jshort>(env, jshortArray(wrapper.object(env))));
            }else if(typeid_equals(dataTypeId, typeid(JArrayGlobalWrapperData<jshort>)) || typeid_equals(dataTypeId, typeid(JArrayWeakWrapperData<jshort>))){
                m_data = wrappersData;
            }else{
                m_data.reset();
            }
        }else if(typeid_equals(typeId, typeid(jlong))){
            if(typeid_equals(dataTypeId, typeid(JObjectGlobalWrapperData))){
                m_data = static_cast<JObjectWrapperData*>(new JArrayGlobalWrapperData<jlong>(env, jlongArray(wrapper.object(env))));
            }else if(typeid_equals(dataTypeId, typeid(JObjectWeakWrapperData))){
                m_data = static_cast<JObjectWrapperData*>(new JArrayWeakWrapperData<jlong>(env, jlongArray(wrapper.object(env))));
            }else if(typeid_equals(dataTypeId, typeid(JArrayGlobalWrapperData<jlong>)) || typeid_equals(dataTypeId, typeid(JArrayWeakWrapperData<jlong>))){
                m_data = wrappersData;
            }else{
                m_data.reset();
            }
        }else if(typeid_equals(typeId, typeid(jchar))){
            if(typeid_equals(dataTypeId, typeid(JObjectGlobalWrapperData))){
                m_data = static_cast<JObjectWrapperData*>(new JArrayGlobalWrapperData<jchar>(env, jcharArray(wrapper.object(env))));
            }else if(typeid_equals(dataTypeId, typeid(JObjectWeakWrapperData))){
                m_data = static_cast<JObjectWrapperData*>(new JArrayWeakWrapperData<jchar>(env, jcharArray(wrapper.object(env))));
            }else if(typeid_equals(dataTypeId, typeid(JArrayGlobalWrapperData<jchar>)) || typeid_equals(dataTypeId, typeid(JArrayWeakWrapperData<jchar>))){
                m_data = wrappersData;
            }else{
                m_data.reset();
            }
        }else if(typeid_equals(typeId, typeid(jboolean))){
            if(typeid_equals(dataTypeId, typeid(JObjectGlobalWrapperData))){
                m_data = static_cast<JObjectWrapperData*>(new JArrayGlobalWrapperData<jboolean>(env, jbooleanArray(wrapper.object(env))));
            }else if(typeid_equals(dataTypeId, typeid(JObjectWeakWrapperData))){
                m_data = static_cast<JObjectWrapperData*>(new JArrayWeakWrapperData<jboolean>(env, jbooleanArray(wrapper.object(env))));
            }else if(typeid_equals(dataTypeId, typeid(JArrayGlobalWrapperData<jboolean>)) || typeid_equals(dataTypeId, typeid(JArrayWeakWrapperData<jboolean>))){
                m_data = wrappersData;
            }else{
                m_data.reset();
            }
        }else if(typeid_equals(typeId, typeid(jfloat))){
            if(typeid_equals(dataTypeId, typeid(JObjectGlobalWrapperData))){
                m_data = static_cast<JObjectWrapperData*>(new JArrayGlobalWrapperData<jfloat>(env, jfloatArray(wrapper.object(env))));
            }else if(typeid_equals(dataTypeId, typeid(JObjectWeakWrapperData))){
                m_data = static_cast<JObjectWrapperData*>(new JArrayWeakWrapperData<jfloat>(env, jfloatArray(wrapper.object(env))));
            }else if(typeid_equals(dataTypeId, typeid(JArrayGlobalWrapperData<jfloat>)) || typeid_equals(dataTypeId, typeid(JArrayWeakWrapperData<jfloat>))){
                m_data = wrappersData;
            }else{
                m_data.reset();
            }
        }else if(typeid_equals(typeId, typeid(jdouble))){
            if(typeid_equals(dataTypeId, typeid(JObjectGlobalWrapperData))){
                m_data = static_cast<JObjectWrapperData*>(new JArrayGlobalWrapperData<jdouble>(env, jdoubleArray(wrapper.object(env))));
            }else if(typeid_equals(dataTypeId, typeid(JObjectWeakWrapperData))){
                m_data = static_cast<JObjectWrapperData*>(new JArrayWeakWrapperData<jdouble>(env, jdoubleArray(wrapper.object(env))));
            }else if(typeid_equals(dataTypeId, typeid(JArrayGlobalWrapperData<jdouble>)) || typeid_equals(dataTypeId, typeid(JArrayWeakWrapperData<jdouble>))){
                m_data = wrappersData;
            }else{
                m_data.reset();
            }
        }else{
            m_data.reset();
        }
    }else{
        m_data.reset();
    }
}

void JObjectWrapper::assign(JNIEnv* env, JObjectWrapper&& wrapper, const std::type_info& typeId)
{
    if(QExplicitlySharedDataPointer<JObjectWrapperData> wrappersData = QExplicitlySharedDataPointer<JObjectWrapperData>(std::move(wrapper.m_data))){
        JObjectWrapperData* wd = wrappersData.data();
        const std::type_info& dataTypeId = typeid(*wd);
        if(typeid_equals(typeId, typeid(jint))){
            if(typeid_equals(dataTypeId, typeid(JObjectGlobalWrapperData))){
                m_data = static_cast<JObjectWrapperData*>(new JArrayGlobalWrapperData<jint>(env, jintArray(wrapper.object(env))));
            }else if(typeid_equals(dataTypeId, typeid(JObjectWeakWrapperData))){
                m_data = static_cast<JObjectWrapperData*>(new JArrayWeakWrapperData<jint>(env, jintArray(wrapper.object(env))));
            }else if(typeid_equals(dataTypeId, typeid(JArrayGlobalWrapperData<jint>)) || typeid_equals(dataTypeId, typeid(JArrayWeakWrapperData<jint>))){
                m_data = std::move(wrappersData);
            }else{
                m_data.reset();
            }
        }else if(typeid_equals(typeId, typeid(jbyte))){
            if(typeid_equals(dataTypeId, typeid(JObjectGlobalWrapperData))){
                m_data = static_cast<JObjectWrapperData*>(new JArrayGlobalWrapperData<jbyte>(env, jbyteArray(wrapper.object(env))));
            }else if(typeid_equals(dataTypeId, typeid(JObjectWeakWrapperData))){
                m_data = static_cast<JObjectWrapperData*>(new JArrayWeakWrapperData<jbyte>(env, jbyteArray(wrapper.object(env))));
            }else if(typeid_equals(dataTypeId, typeid(JArrayGlobalWrapperData<jbyte>)) || typeid_equals(dataTypeId, typeid(JArrayWeakWrapperData<jbyte>))){
                m_data = std::move(wrappersData);
            }else{
                m_data.reset();
            }
        }else if(typeid_equals(typeId, typeid(jshort))){
            if(typeid_equals(dataTypeId, typeid(JObjectGlobalWrapperData))){
                m_data = static_cast<JObjectWrapperData*>(new JArrayGlobalWrapperData<jshort>(env, jshortArray(wrapper.object(env))));
            }else if(typeid_equals(dataTypeId, typeid(JObjectWeakWrapperData))){
                m_data = static_cast<JObjectWrapperData*>(new JArrayWeakWrapperData<jshort>(env, jshortArray(wrapper.object(env))));
            }else if(typeid_equals(dataTypeId, typeid(JArrayGlobalWrapperData<jshort>)) || typeid_equals(dataTypeId, typeid(JArrayWeakWrapperData<jshort>))){
                m_data = std::move(wrappersData);
            }else{
                m_data.reset();
            }
        }else if(typeid_equals(typeId, typeid(jlong))){
            if(typeid_equals(dataTypeId, typeid(JObjectGlobalWrapperData))){
                m_data = static_cast<JObjectWrapperData*>(new JArrayGlobalWrapperData<jlong>(env, jlongArray(wrapper.object(env))));
            }else if(typeid_equals(dataTypeId, typeid(JObjectWeakWrapperData))){
                m_data = static_cast<JObjectWrapperData*>(new JArrayWeakWrapperData<jlong>(env, jlongArray(wrapper.object(env))));
            }else if(typeid_equals(dataTypeId, typeid(JArrayGlobalWrapperData<jlong>)) || typeid_equals(dataTypeId, typeid(JArrayWeakWrapperData<jlong>))){
                m_data = std::move(wrappersData);
            }else{
                m_data.reset();
            }
        }else if(typeid_equals(typeId, typeid(jchar))){
            if(typeid_equals(dataTypeId, typeid(JObjectGlobalWrapperData))){
                m_data = static_cast<JObjectWrapperData*>(new JArrayGlobalWrapperData<jchar>(env, jcharArray(wrapper.object(env))));
            }else if(typeid_equals(dataTypeId, typeid(JObjectWeakWrapperData))){
                m_data = static_cast<JObjectWrapperData*>(new JArrayWeakWrapperData<jchar>(env, jcharArray(wrapper.object(env))));
            }else if(typeid_equals(dataTypeId, typeid(JArrayGlobalWrapperData<jchar>)) || typeid_equals(dataTypeId, typeid(JArrayWeakWrapperData<jchar>))){
                m_data = std::move(wrappersData);
            }else{
                m_data.reset();
            }
        }else if(typeid_equals(typeId, typeid(jboolean))){
            if(typeid_equals(dataTypeId, typeid(JObjectGlobalWrapperData))){
                m_data = static_cast<JObjectWrapperData*>(new JArrayGlobalWrapperData<jboolean>(env, jbooleanArray(wrapper.object(env))));
            }else if(typeid_equals(dataTypeId, typeid(JObjectWeakWrapperData))){
                m_data = static_cast<JObjectWrapperData*>(new JArrayWeakWrapperData<jboolean>(env, jbooleanArray(wrapper.object(env))));
            }else if(typeid_equals(dataTypeId, typeid(JArrayGlobalWrapperData<jboolean>)) || typeid_equals(dataTypeId, typeid(JArrayWeakWrapperData<jboolean>))){
                m_data = std::move(wrappersData);
            }else{
                m_data.reset();
            }
        }else if(typeid_equals(typeId, typeid(jfloat))){
            if(typeid_equals(dataTypeId, typeid(JObjectGlobalWrapperData))){
                m_data = static_cast<JObjectWrapperData*>(new JArrayGlobalWrapperData<jfloat>(env, jfloatArray(wrapper.object(env))));
            }else if(typeid_equals(dataTypeId, typeid(JObjectWeakWrapperData))){
                m_data = static_cast<JObjectWrapperData*>(new JArrayWeakWrapperData<jfloat>(env, jfloatArray(wrapper.object(env))));
            }else if(typeid_equals(dataTypeId, typeid(JArrayGlobalWrapperData<jfloat>)) || typeid_equals(dataTypeId, typeid(JArrayWeakWrapperData<jfloat>))){
                m_data = std::move(wrappersData);
            }else{
                m_data.reset();
            }
        }else if(typeid_equals(typeId, typeid(jdouble))){
            if(typeid_equals(dataTypeId, typeid(JObjectGlobalWrapperData))){
                m_data = static_cast<JObjectWrapperData*>(new JArrayGlobalWrapperData<jdouble>(env, jdoubleArray(wrapper.object(env))));
            }else if(typeid_equals(dataTypeId, typeid(JObjectWeakWrapperData))){
                m_data = static_cast<JObjectWrapperData*>(new JArrayWeakWrapperData<jdouble>(env, jdoubleArray(wrapper.object(env))));
            }else if(typeid_equals(dataTypeId, typeid(JArrayGlobalWrapperData<jdouble>)) || typeid_equals(dataTypeId, typeid(JArrayWeakWrapperData<jdouble>))){
                m_data = std::move(wrappersData);
            }else{
                m_data.reset();
            }
        }else{
            m_data.reset();
        }
    }else{
        m_data.reset();
    }
}

const void* JObjectWrapper::array() const
{
    if(QExplicitlySharedDataPointer<JObjectWrapperData> data{m_data})
        return data->array();
    return nullptr;
}

void* JObjectWrapper::array()
{
    if(QExplicitlySharedDataPointer<JObjectWrapperData> data{m_data})
        return data->array();
    return nullptr;
}

void JObjectWrapper::commitArray(){
    if(QExplicitlySharedDataPointer<JObjectWrapperData> data{m_data})
        data->commitArray();
}

void JObjectWrapper::swap(JObjectWrapper& other){
    m_data.swap(other.m_data);
}

jsize JObjectWrapper::arrayLength() const {
    if(QExplicitlySharedDataPointer<JObjectWrapperData> data{m_data})
        return data->arrayLength();
    return 0;
}

JObjectWrapper& JObjectWrapper::operator=(jobject object) {
    if(JniEnvironment env{500}){
        assign(env, object);
    }
    return *this;
}

void JObjectWrapper::assign(JNIEnv *env, jobject object) {
    if(env->ExceptionCheck()){
        env->ExceptionDescribe();
        env->ExceptionClear();
    }
    if(env->IsSameObject(object, nullptr)){
        m_data.reset();
    }else if(Java::Runtime::Enum::isInstanceOf(env, object)
       || Java::QtJambi::QtEnumerator::isInstanceOf(env, object)
       || Java::QtJambi::QtShortEnumerator::isInstanceOf(env, object)
       || Java::QtJambi::QtByteEnumerator::isInstanceOf(env, object)
       || Java::QtJambi::QtLongEnumerator::isInstanceOf(env, object)){
       *this = JEnumWrapper(env, object);
    }else if(Java::Runtime::Collection::isInstanceOf(env, object)){
        *this = JCollectionWrapper(env, object);
    }else if(Java::Runtime::Map::isInstanceOf(env, object)){
        *this = JMapWrapper(env, object);
    }else if(Java::Runtime::Iterator::isInstanceOf(env, object)){
        *this = JIteratorWrapper(env, object);
    }else{
        jclass cls = env->GetObjectClass(object);
        if(Java::Runtime::Class::isArray(env, cls)){
            jclass componentType = Java::Runtime::Class::getComponentType(env, cls);
            if(Java::Runtime::Integer::isPrimitiveType(env, componentType)){
                *this = JIntArrayWrapper(env, jintArray(object));
            }else if(Java::Runtime::Byte::isPrimitiveType(env, componentType)){
                *this = JByteArrayWrapper(env, jbyteArray(object));
            }else if(Java::Runtime::Short::isPrimitiveType(env, componentType)){
                *this = JShortArrayWrapper(env, jshortArray(object));
            }else if(Java::Runtime::Long::isPrimitiveType(env, componentType)){
                *this = JLongArrayWrapper(env, jlongArray(object));
            }else if(Java::Runtime::Character::isPrimitiveType(env, componentType)){
                *this = JCharArrayWrapper(env, jcharArray(object));
            }else if(Java::Runtime::Boolean::isPrimitiveType(env, componentType)){
                *this = JBooleanArrayWrapper(env, jbooleanArray(object));
            }else if(Java::Runtime::Float::isPrimitiveType(env, componentType)){
                *this = JFloatArrayWrapper(env, jfloatArray(object));
            }else if(Java::Runtime::Double::isPrimitiveType(env, componentType)){
                *this = JDoubleArrayWrapper(env, jdoubleArray(object));
            }else{
                *this = JObjectArrayWrapper(env, jobjectArray(object));
            }
        }else{
            *this = JObjectWrapper(env, object);
        }
    }
}

JObjectWrapper& JObjectWrapper::operator=(const JObjectWrapper &wrapper) {
    assign(wrapper);
    return *this;
}

void JObjectWrapper::assign(const JObjectWrapper &wrapper) {
    m_data = wrapper.m_data;
}

JObjectWrapper& JObjectWrapper::operator=(JObjectWrapper &&wrapper) {
    if(JniEnvironment env{200}){
        assign(env, std::move(wrapper));
    }
    return *this;
}

void JObjectWrapper::assign(JNIEnv *env, JObjectWrapper &&wrapper) {
    if(JObjectWrapper::isEquals(typeid(*this), typeid(wrapper)) || !wrapper.object(env)){
        m_data = std::move(wrapper.m_data);
    }
}

jobject JObjectWrapper::object(JNIEnv *env) const{
    if(QExplicitlySharedDataPointer<JObjectWrapperData> data{m_data})
        return data->data(env);
    return nullptr;
}

JObjectWrapper::operator jobject() const {
    if(QExplicitlySharedDataPointer<JObjectWrapperData> data{m_data}){
#if !defined(QTJAMBI_NO_GLOBALREFS)
        return data->data();
#else
        if(DefaultJniEnvironment env{0}){
            return data->data(env);
        }
#endif
    }
    return nullptr;
}

void JObjectWrapper::clear(JNIEnv *env){
    QExplicitlySharedDataPointer<JObjectWrapperData> data(std::move(m_data));
    if(data && data->ref.loadRelaxed()==1)
        data->clear(env);
}

JObjectWrapper::~JObjectWrapper(){
}

QString JObjectWrapper::toString(JNIEnv *env, bool * ok) const {
    jobject _object = object(env);
    if(_object){
        if(ok){
            if (Java::QtCore::QMetaType$GenericTypeInterface::isInstanceOf(env, _object)) {
                QVariant variant = QtJambiAPI::convertJavaObjectToQVariant(env, _object);
                *ok = true;
                return variant.toString();
            }else if (Java::QtJambi::QtObjectInterface::isInstanceOf(env, _object)) {
                jstring strg = Java::QtJambi::ClassAnalyzerUtility::objectToString(env, _object);
                *ok = strg;
                return qtjambi_cast<QString>(env, strg);
            }else{
                return qtjambi_cast<QString>(env, Java::Runtime::Object::toString(env, _object));
            }
        }else{
            return qtjambi_cast<QString>(env, Java::Runtime::Object::toString(env, _object));
        }
    }
    if(ok)
        *ok = true;
    return "null";
}

QString JObjectWrapper::toString(bool * ok) const {
    if(JniEnvironment env{200}){
        return toString(env, ok);
    }
    return {};
}

bool JObjectWrapper::isEquals(const std::type_info& typeId1, const std::type_info& typeId2){
    return typeid_equals(typeId1, typeId2);
}

jobject JObjectWrapper::filterPrimitiveArray(JNIEnv *env, jobject object, const std::type_info& typeId){
    if(object){
        jclass cls = env->GetObjectClass(object);
        if(Java::Runtime::Class::isArray(env, cls)){
            jclass componentType = Java::Runtime::Class::getComponentType(env, cls);
            if(typeid_equals(typeId, typeid(jint)) && Java::Runtime::Integer::isPrimitiveType(env, componentType)){
                return object;
            }else if(typeid_equals(typeId, typeid(jbyte)) && Java::Runtime::Byte::isPrimitiveType(env, componentType)){
                return object;
            }else if(typeid_equals(typeId, typeid(jshort)) && Java::Runtime::Short::isPrimitiveType(env, componentType)){
                return object;
            }else if(typeid_equals(typeId, typeid(jlong)) && Java::Runtime::Long::isPrimitiveType(env, componentType)){
                return object;
            }else if(typeid_equals(typeId, typeid(jchar)) && Java::Runtime::Character::isPrimitiveType(env, componentType)){
                return object;
            }else if(typeid_equals(typeId, typeid(jboolean)) && Java::Runtime::Boolean::isPrimitiveType(env, componentType)){
                return object;
            }else if(typeid_equals(typeId, typeid(jfloat)) && Java::Runtime::Float::isPrimitiveType(env, componentType)){
                return object;
            }else if(typeid_equals(typeId, typeid(jdouble)) && Java::Runtime::Double::isPrimitiveType(env, componentType)){
                return object;
            }
        }
    }
    return nullptr;
}

jobject filterObjectArray(JNIEnv *env, jobject object){
    if(object){
        jclass cls = env->GetObjectClass(object);
        if(Java::Runtime::Class::isArray(env, cls)){
            jclass componentType = Java::Runtime::Class::getComponentType(env, cls);
            if(!Java::Runtime::Class::isPrimitive(env, componentType)){
               return object;
            }
        }
    }
    return nullptr;
}

jobject filterObjectArray(jobject object){
    if(JniEnvironment env{500}){
        return filterObjectArray(env, object);
    }
    return nullptr;
}

jobject filterIterator(JNIEnv *env, jobject object){
    if(Java::Runtime::Iterator::isInstanceOf(env, object)){
       return object;
    }
    return nullptr;
}

jobject filterIterator(jobject object){
    if(JniEnvironment env{500}){
        return filterIterator(env, object);
    }
    return nullptr;
}

jobject filterCollection(JNIEnv *env, jobject object){
    if(Java::Runtime::Collection::isInstanceOf(env, object)){
       return object;
    }
    return nullptr;
}

jobject filterCollection(jobject object){
    if(JniEnvironment env{500}){
        return filterCollection(env, object);
    }
    return nullptr;
}

jobject filterMap(JNIEnv *env, jobject object){
    if(Java::Runtime::Map::isInstanceOf(env, object)){
       return object;
    }
    return nullptr;
}

jobject filterMap(jobject object){
    if(JniEnvironment env{500}){
        return filterMap(env, object);
    }
    return nullptr;
}

jobject filterEnum(JNIEnv *env, jobject object){
    if(Java::Runtime::Enum::isInstanceOf(env, object)
        || Java::QtJambi::QtEnumerator::isInstanceOf(env, object)
        || Java::QtJambi::QtShortEnumerator::isInstanceOf(env, object)
        || Java::QtJambi::QtByteEnumerator::isInstanceOf(env, object)
        || Java::QtJambi::QtLongEnumerator::isInstanceOf(env, object)){
       return object;
    }
    return nullptr;
}

jobject filterEnum(jobject object){
    if(JniEnvironment env{500}){
        return filterEnum(env, object);
    }
    return nullptr;
}

JEnumWrapper::JEnumWrapper(JNIEnv *env, jobject obj, bool globalRefs)
    : JObjectWrapper(env, filterEnum(env, obj), globalRefs) {}

JEnumWrapper::JEnumWrapper(jobject obj)
    : JObjectWrapper(filterEnum(obj)) {}

void JEnumWrapper::assign(JNIEnv *env, jobject object) {
    object = filterEnum(env, object);
    if(object){
        m_data = static_cast<JObjectWrapperData*>(new JObjectGlobalWrapperData(env, object));
    }else{
        m_data.reset();
    }
}

void JEnumWrapper::assign(const JObjectWrapper &wrapper) {
    if(JniEnvironment env{500}){
        if(filterEnum(env, wrapper.object(env))){
            m_data = wrapper.m_data;
        }else{
            m_data.reset();
        }
    }
}

void JEnumWrapper::assign(JNIEnv *env, JObjectWrapper &&wrapper) {
    if(filterEnum(env, wrapper.object(env))){
        m_data = std::move(wrapper.m_data);
    }else{
        m_data.reset();
    }
}

JEnumWrapper& JEnumWrapper::operator=(const JObjectWrapper &wrapper) {
    if(JniEnvironment env{500}){
        if(filterEnum(env, wrapper.object(env))){
            m_data = wrapper.m_data;
        }else{
            m_data.reset();
        }
    }
    return *this;
}

JEnumWrapper& JEnumWrapper::operator=(JObjectWrapper &&wrapper) {
    if(JniEnvironment env{500}){
        if(filterEnum(env, wrapper.object(env))){
            m_data = std::move(wrapper.m_data);
        }else{
            m_data.reset();
        }
    }
    return *this;
}

JEnumWrapper& JEnumWrapper::operator=(const JEnumWrapper &wrapper) {
    m_data = wrapper.m_data;
    return *this;
}

JEnumWrapper& JEnumWrapper::operator=(JEnumWrapper &&wrapper) {
    m_data = std::move(wrapper.m_data);
    return *this;
}

#undef filterEnum

qint32 JEnumWrapper::ordinal() const {
    if(JniEnvironment env{500}){
        return JEnumWrapper::ordinal(env);
    }
    return -1;
}

qint32 JEnumWrapper::ordinal(JNIEnv *env) const {
    jobject _object = object(env);
    if(_object){
        return Java::Runtime::Enum::ordinal(env, _object);
    }
    return -1;
}

JIteratorWrapper::JIteratorWrapper(JNIEnv *env, jobject obj, bool globalRefs)
    : JObjectWrapper(env, filterIterator(env, obj), globalRefs) {}

JIteratorWrapper::JIteratorWrapper(jobject obj)
    : JObjectWrapper(filterIterator(obj)) {}

void JIteratorWrapper::assign(JNIEnv *env, jobject object) {
    object = filterIterator(env, object);
    if(object){
        m_data = static_cast<JObjectWrapperData*>(new JObjectGlobalWrapperData(env, object));
    }else{
        m_data.reset();
    }
}

void JIteratorWrapper::assign(const JObjectWrapper &wrapper) {
    if(JniEnvironment env{500}){
        if(filterIterator(env, wrapper.object(env))){
            m_data = wrapper.m_data;
        }else{
            m_data.reset();
        }
    }
}

void JIteratorWrapper::assign(JNIEnv *env, JObjectWrapper &&wrapper) {
    if(filterIterator(env, wrapper.object(env))){
        m_data = std::move(wrapper.m_data);
    }else{
        m_data.reset();
    }
}

JIteratorWrapper& JIteratorWrapper::operator=(const JIteratorWrapper &wrapper) {
    m_data = wrapper.m_data;
    return *this;
}

JIteratorWrapper& JIteratorWrapper::operator=(JIteratorWrapper &&wrapper) {
    m_data = std::move(wrapper.m_data);
    return *this;
}

JIteratorWrapper& JIteratorWrapper::operator=(const JObjectWrapper &wrapper) {
    if(JniEnvironment env{500}){
        if(filterIterator(env, wrapper.object(env))){
            m_data = wrapper.m_data;
        }else{
            m_data.reset();
        }
    }
    return *this;
}

JIteratorWrapper& JIteratorWrapper::operator=(JObjectWrapper &&wrapper) {
    if(JniEnvironment env{500}){
        if(filterIterator(env, wrapper.object(env))){
            m_data = std::move(wrapper.m_data);
        }else{
            m_data.reset();
        }
    }
    return *this;
}

bool JIteratorWrapper::hasNext() const {
    if(JniEnvironment env{500}){
        JIteratorWrapper::hasNext(env);
    }
    return false;
}

bool JIteratorWrapper::hasNext(JNIEnv *env) const {
    jobject _object = object(env);
    if(_object){
        return Java::Runtime::Iterator::hasNext(env, _object);
    }
    return false;
}

JObjectWrapper JIteratorWrapper::next() const {
    if(JniEnvironment env{500}){
        return JIteratorWrapper::next(env);
    }else{
        return JObjectWrapper();
    }
}

JObjectWrapper JIteratorWrapper::next(JNIEnv *env) const {
    return JObjectWrapper(env, _next(env));
}

jobject JIteratorWrapper::_next(JNIEnv *env) const {
    jobject _object = object(env);
    if(_object){
        return Java::Runtime::Iterator::next(env, _object);
    }else{
        return nullptr;
    }
}

JCollectionWrapper::JCollectionWrapper(JNIEnv *env, jobject obj, bool globalRefs)
    : JObjectWrapper(env, filterCollection(env, obj), globalRefs) {}

JCollectionWrapper::JCollectionWrapper(jobject obj)
    : JObjectWrapper(filterCollection(obj)) {}

void JCollectionWrapper::assign(JNIEnv *env, jobject object) {
    object = filterCollection(env, object);
    if(object){
        m_data = static_cast<JObjectWrapperData*>(new JObjectGlobalWrapperData(env, object));
    }else{
        m_data.reset();
    }
}

void JCollectionWrapper::assign(const JObjectWrapper &wrapper) {
    if(JniEnvironment env{500}){
        if(filterCollection(env, wrapper.object(env))){
            m_data = wrapper.m_data;
        }else{
            m_data.reset();
        }
    }
}

void JCollectionWrapper::assign(JNIEnv *env, JObjectWrapper &&wrapper) {
    if(filterCollection(env, wrapper.object(env))){
        m_data = std::move(wrapper.m_data);
    }else{
        m_data.reset();
    }
}

JCollectionWrapper& JCollectionWrapper::operator=(const JObjectWrapper &wrapper) {
    if(JniEnvironment env{500}){
        if(filterCollection(env, wrapper.object(env))){
            m_data = wrapper.m_data;
        }else{
            m_data.reset();
        }
    }
    return *this;
}

JCollectionWrapper& JCollectionWrapper::operator=(JObjectWrapper &&wrapper) {
    if(JniEnvironment env{500}){
        if(filterCollection(env, wrapper.object(env))){
            m_data = std::move(wrapper.m_data);
        }else{
            m_data.reset();
        }
    }
    return *this;
}

JCollectionWrapper& JCollectionWrapper::operator=(const JCollectionWrapper &wrapper) {
    m_data = wrapper.m_data;
    return *this;
}

JCollectionWrapper& JCollectionWrapper::operator=(JCollectionWrapper &&wrapper) {
    m_data = std::move(wrapper.m_data);
    return *this;
}

int JCollectionWrapper::size() const {
    if(JniEnvironment env{200}){
        jobject _object = object(env);
        if(_object){
           return Java::Runtime::Collection::size(env, _object);
        }
    }
    return 0;
}

JIteratorWrapper JCollectionWrapper::iterator() const {
    if(JniEnvironment env{200}){
        JCollectionWrapper::iterator(env);
    }
    return JIteratorWrapper();
}

JIteratorWrapper JCollectionWrapper::iterator(JNIEnv *env) const {
    jobject _object = object(env);
    if(_object){
        jobject iter = Java::Runtime::Collection::iterator(env, _object);
        return JIteratorWrapper(env, iter);
    }
    return JIteratorWrapper();
}

QList<QVariant> JCollectionWrapper::toList() const {
    if(JniEnvironment env{200}){
        return JCollectionWrapper::toList(env);
    }
    return {};
}

QList<QVariant> JCollectionWrapper::toList(JNIEnv *env) const {
    QList<QVariant> list;
    for (JIteratorWrapper iter = iterator(env); iter.hasNext(env);) {
        jobject o = iter._next(env);
        list << QtJambiAPI::convertJavaObjectToQVariant(env, o);
    }
    return list;
}

QStringList JCollectionWrapper::toStringList(bool * ok) const {
    if(JniEnvironment env{200}){
        return JCollectionWrapper::toStringList(env, ok);
    }
    if(ok) ok[0] = false;
    return {};
}

QStringList JCollectionWrapper::toStringList(JNIEnv *env, bool * ok) const {
    QStringList list;
    for (JIteratorWrapper iter = iterator(env); iter.hasNext(env);) {
        list << qtjambi_cast<QString>(env, iter._next(env));
    }
    if(ok) ok[0] = true;
    return list;
}

JMapWrapper::JMapWrapper(JNIEnv *env, jobject obj, bool globalRefs)
    : JObjectWrapper(env, filterMap(env, obj), globalRefs) {}

JMapWrapper::JMapWrapper(jobject obj)
    : JObjectWrapper(filterMap(obj)) {}

void JMapWrapper::assign(JNIEnv *env, jobject object) {
    object = filterMap(env, object);
    if(object){
        m_data = static_cast<JObjectWrapperData*>(new JObjectGlobalWrapperData(env, object));
    }else{
        m_data.reset();
    }
}

void JMapWrapper::assign(const JObjectWrapper &wrapper) {
    if(JniEnvironment env{500}){
        if(filterMap(env, wrapper.object(env))){
            m_data = wrapper.m_data;
        }else{
            m_data.reset();
        }
    }
}

void JMapWrapper::assign(JNIEnv *env, JObjectWrapper &&wrapper) {
    if(filterMap(env, wrapper.object(env))){
        m_data = std::move(wrapper.m_data);
    }else{
        m_data.reset();
    }
}

JMapWrapper& JMapWrapper::operator=(const JMapWrapper &wrapper) {
    m_data = wrapper.m_data;
    return *this;
}

JMapWrapper& JMapWrapper::operator=(JMapWrapper &&wrapper) {
    m_data = std::move(wrapper.m_data);
    return *this;
}

JMapWrapper& JMapWrapper::operator=(const JObjectWrapper &wrapper) {
    if(JniEnvironment env{500}){
        if(filterMap(env, wrapper.object(env))){
            m_data = wrapper.m_data;
        }else{
            m_data.reset();
        }
    }
    return *this;
}

JMapWrapper& JMapWrapper::operator=(JObjectWrapper &&wrapper) {
    if(JniEnvironment env{500}){
        if(filterMap(env, wrapper.object(env))){
            m_data = std::move(wrapper.m_data);
        }else{
            m_data.reset();
        }
    }
    return *this;
}

jobject JMapWrapper::_entrySet(JNIEnv* env) const {
    jobject _object = object(env);
    if(_object){
        return Java::Runtime::Map::entrySet(env, _object);
    }
    return nullptr;
}

JCollectionWrapper JMapWrapper::entrySet() const {
    if(JniEnvironment env{16})
        return JCollectionWrapper(env, _entrySet(env));
    return JCollectionWrapper();
}

QMap<QVariant,QVariant> JMapWrapper::toMap() const {
    QMap<QVariant,QVariant> map;
    if(JniEnvironment env{200}){
        int type = QMetaType::UnknownType;
        for(JIteratorWrapper iter = entrySet().iterator(); iter.hasNext();){
            jobject entry = iter._next(env);
            jobject key = Java::Runtime::Map$Entry::getKey(env, entry);
            jobject value = Java::Runtime::Map$Entry::getValue(env, entry);
            QVariant k(QtJambiAPI::convertJavaObjectToQVariant(env, key));
            QVariant v(QtJambiAPI::convertJavaObjectToQVariant(env, value));
            if(k.userType()!=QMetaType::UnknownType){
                if(type==QMetaType::UnknownType){
                    type = k.userType();
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                    if(!QtJambiTypeManager::hasRegisteredComparators(type)){
                        break;
                    }
#endif //QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                }else if(type != k.userType()){
                    continue;
                }
                map.insert(k, v);
            }
        }
    }
    return map;
}

QVariantMap JMapWrapper::toStringMap(bool* ok) const {
    QVariantMap map;
    if(JniEnvironment env{200}){
        for(JIteratorWrapper iter = entrySet().iterator(); iter.hasNext();){
            jobject entry = iter._next(env);
            jobject key = Java::Runtime::Map$Entry::getKey(env, entry);
            if(ok && key && !Java::Runtime::String::isInstanceOf(env, key)){
                ok[0] = false;
                map.clear();
                return map;
            }
            jobject value = Java::Runtime::Map$Entry::getValue(env, entry);
            map.insert(QtJambiAPI::convertJavaObjectToQVariant(env, key).toString(), QtJambiAPI::convertJavaObjectToQVariant(env, value));
        }
        if(ok) ok[0] = true;
    }else{
        ok[0] = false;
    }
    return map;
}

QVariantHash JMapWrapper::toStringHash(bool* ok) const {
    QVariantHash map;
    if(JniEnvironment env{200}){
        for(JIteratorWrapper iter = entrySet().iterator(); iter.hasNext();){
            jobject entry = iter._next(env);
            jobject key = Java::Runtime::Map$Entry::getKey(env, entry);
            if(ok && key && !Java::Runtime::String::isInstanceOf(env, key)){
                ok[0] = false;
                map.clear();
                return map;
            }
            jobject value = Java::Runtime::Map$Entry::getValue(env, entry);
            map.insert(QtJambiAPI::convertJavaObjectToQVariant(env, key).toString(), QtJambiAPI::convertJavaObjectToQVariant(env, value));
        }
        if(ok) ok[0] = true;
    }else{
        ok[0] = false;
    }
    return map;
}

JObjectWrapperRef::JObjectWrapperRef(const JObjectWrapper& arrayWrapper, jsize index) : m_arrayWrapper(arrayWrapper), m_index(index) {}

JObjectWrapperRef& JObjectWrapperRef::operator=(jobject newValue)
{
    if(JniEnvironment env{200}){
        if(m_arrayWrapper.object(env)){
            env->SetObjectArrayElement(jobjectArray(m_arrayWrapper.object(env)), m_index, newValue);
            JavaException::check(env QTJAMBI_STACKTRACEINFO );
        }
    }
    return *this;
}

JObjectWrapperRef& JObjectWrapperRef::operator=(const JObjectWrapper &newValue)
{
    if(JniEnvironment env{200}){
        if(m_arrayWrapper.object(env)){
            env->SetObjectArrayElement(jobjectArray(m_arrayWrapper.object(env)), m_index, newValue.object(env));
            JavaException::check(env QTJAMBI_STACKTRACEINFO );
        }
    }
    return *this;
}

JObjectWrapperRef::operator JObjectWrapper() const
{
    if(JniEnvironment env{200}){
        if(m_arrayWrapper.object(env)){
            jobject value = env->GetObjectArrayElement(jobjectArray(m_arrayWrapper.object(env)), m_index);
            JavaException::check(env QTJAMBI_STACKTRACEINFO );
            return JObjectWrapper(env, value);
        }
    }
    return JObjectWrapper();
}

JObjectWrapperRef::operator jobject() const
{
    if(JniEnvironment env{0}){
        if(m_arrayWrapper.object(env)){
            env->PushLocalFrame(200);
            jobject value = env->GetObjectArrayElement(jobjectArray(m_arrayWrapper.object(env)), m_index);
            JavaException::check(env QTJAMBI_STACKTRACEINFO );
            return env->PopLocalFrame(value);
        }
    }
    return nullptr;
}

JObjectArrayWrapper::JObjectArrayWrapper(JNIEnv *env, jobjectArray obj, bool globalRefs)
    : JObjectWrapper(env, filterObjectArray(env, obj), globalRefs) {}

JObjectArrayWrapper::JObjectArrayWrapper(jobjectArray obj)
    : JObjectWrapper(filterObjectArray(obj)) {}

void JObjectArrayWrapper::assign(JNIEnv *env, jobject object) {
    object = filterObjectArray(env, object);
    if(object){
        m_data = static_cast<JObjectWrapperData*>(new JObjectGlobalWrapperData(env, object));
    }else{
        m_data.reset();
    }
}

void JObjectArrayWrapper::assign(const JObjectWrapper &wrapper) {
    if(JniEnvironment env{500}){
        if(filterObjectArray(env, wrapper.object(env))){
            m_data = wrapper.m_data;
        }else{
            m_data.reset();
        }
    }
}

void JObjectArrayWrapper::assign(JNIEnv *env, JObjectWrapper &&wrapper) {
    if(filterObjectArray(env, wrapper.object(env))){
        m_data = std::move(wrapper.m_data);
    }else{
        m_data.reset();
    }
}

JObjectArrayWrapper& JObjectArrayWrapper::operator=(const JObjectWrapper &wrapper) {
    if(JniEnvironment env{500}){
        if(filterObjectArray(env, wrapper.object(env))){
            m_data = wrapper.m_data;
        }else{
            m_data.reset();
        }
    }
    return *this;
}

JObjectArrayWrapper& JObjectArrayWrapper::operator=(JObjectWrapper &&wrapper) {
    if(JniEnvironment env{500}){
        if(filterObjectArray(env, wrapper.object(env))){
            m_data = std::move(wrapper.m_data);
        }else{
            m_data.reset();
        }
    }
    return *this;
}

JObjectArrayWrapper& JObjectArrayWrapper::operator=(const JObjectArrayWrapper &wrapper) {
    m_data = wrapper.m_data;
    return *this;
}

JObjectArrayWrapper& JObjectArrayWrapper::operator=(JObjectArrayWrapper &&wrapper) {
    m_data = std::move(wrapper.m_data);
    return *this;
}

jsize JObjectArrayWrapper::length() const
{
    if(JniEnvironment env{16}){
        return object(env) ? env->GetArrayLength(object(env)) : 0;
    }
    return 0;
}

jobject JObjectArrayWrapper::at(JNIEnv *env, jsize index) const{
    if(!object(env))
        JavaException::raiseIndexOutOfBoundsException(env, QString::number(index) QTJAMBI_STACKTRACEINFO);
    jobject value = env->GetObjectArrayElement(object(env), index);
    JavaException::check(env QTJAMBI_STACKTRACEINFO );
    return value;
}

JObjectWrapper JObjectArrayWrapper::operator[](jsize index) const{
    if(JniEnvironment env{500}){
        if(!object(env))
            JavaException::raiseIndexOutOfBoundsException(env, QString::number(index) QTJAMBI_STACKTRACEINFO);
        jobject value = env->GetObjectArrayElement(object(env), index);
        JavaException::check(env QTJAMBI_STACKTRACEINFO );
        return JObjectWrapper(env, value);
    }
    return JObjectWrapper();
}

JObjectWrapperRef JObjectArrayWrapper::operator[](jsize index){
    if(JniEnvironment env{500}){
        if(index>=0 && object(env) && index < env->GetArrayLength(object(env))){
            return JObjectWrapperRef(*this, index);
        }else{
            JavaException::raiseIndexOutOfBoundsException(env, QString::number(index) QTJAMBI_STACKTRACEINFO);
        }
    }
    return JObjectWrapperRef(JObjectWrapper(), 0);
}

QString JObjectArrayWrapper::toString(JNIEnv *env, bool * ok) const{
    if(ok)
        *ok = true;
    QString result = QLatin1String("[");
    jsize length = object(env) ? env->GetArrayLength(object(env)) : 0;
    for(jsize i=0; i<length; ++i){
        if(i>0)
            result += QLatin1String(",");
        jobject value = env->GetObjectArrayElement(object(env), i);
        JavaException::check(env QTJAMBI_STACKTRACEINFO );
        result += JObjectWrapper(env, value).toString(ok);
        if(ok && !*ok)
            return QString();
    }
    result += QLatin1String("]");
    return result;
}

QDataStream &operator<<(QDataStream &out, const JObjectWrapper &myObj){
    if(JniEnvironment env{200}){
        QtJambiExceptionHandler __exnhandler;
        try{
            jobject jstream = QtJambiAPI::convertNativeToJavaObjectAsWrapper(env, &out, typeid(QDataStream));
            QTJAMBI_INVALIDATE_AFTER_USE(env, jstream);
            Java::QtJambi::MetaTypeUtility::writeSerializableJavaObject(env, jstream, myObj.object(env));
        }catch(const JavaException& exn){
            if(noExceptionForwarding()){
                QtJambiExceptionBlocker __blocker;
                {
                    QtJambiExceptionHandler __handler;
                    __handler.handle(env, exn, "operator<<(QDataStream &, const JObjectWrapper &)");
                }
                __blocker.release(env);
            }else
                __exnhandler.handle(env, exn, "operator<<(QDataStream &, const JObjectWrapper &)");
        }
    }
    return out;
}

QDataStream &operator>>(QDataStream &in, JObjectWrapper &myObj){
    if(JniEnvironment env{200}){
        QtJambiExceptionHandler __exnhandler;
        try{
            jobject jstream = QtJambiAPI::convertNativeToJavaObjectAsWrapper(env, &in, typeid(QDataStream));
            QTJAMBI_INVALIDATE_AFTER_USE(env, jstream);
            jobject res = Java::QtJambi::MetaTypeUtility::readSerializableJavaObject(env, jstream);
            if(!res){
                myObj = JCollectionWrapper();
            }else if(Java::Runtime::Collection::isInstanceOf(env, res)){
                myObj = JCollectionWrapper(env, res);
            }else if(Java::Runtime::Iterator::isInstanceOf(env, res)){
                myObj = JIteratorWrapper(env, res);
            }else if(Java::Runtime::Map::isInstanceOf(env, res)){
                myObj = JMapWrapper(env, res);
            }else if(Java::Runtime::Enum::isInstanceOf(env, res)
                     || Java::QtJambi::QtEnumerator::isInstanceOf(env, res)
                     || Java::QtJambi::QtShortEnumerator::isInstanceOf(env, res)
                     || Java::QtJambi::QtByteEnumerator::isInstanceOf(env, res)
                     || Java::QtJambi::QtLongEnumerator::isInstanceOf(env, res)){
                myObj = JEnumWrapper(env, res);
            }else{
                jclass cls = env->GetObjectClass(res);
                if(Java::Runtime::Class::isArray(env, cls)){
                    jclass componentType = Java::Runtime::Class::getComponentType(env, cls);
                    if(Java::Runtime::Integer::isPrimitiveType(env, componentType)){
                        myObj = JIntArrayWrapper(env, jintArray(res));
                    }else if(Java::Runtime::Byte::isPrimitiveType(env, componentType)){
                        myObj = JByteArrayWrapper(env, jbyteArray(res));
                    }else if(Java::Runtime::Short::isPrimitiveType(env, componentType)){
                        myObj = JShortArrayWrapper(env, jshortArray(res));
                    }else if(Java::Runtime::Long::isPrimitiveType(env, componentType)){
                        myObj = JLongArrayWrapper(env, jlongArray(res));
                    }else if(Java::Runtime::Character::isPrimitiveType(env, componentType)){
                        myObj = JCharArrayWrapper(env, jcharArray(res));
                    }else if(Java::Runtime::Boolean::isPrimitiveType(env, componentType)){
                        myObj = JBooleanArrayWrapper(env, jbooleanArray(res));
                    }else if(Java::Runtime::Float::isPrimitiveType(env, componentType)){
                        myObj = JFloatArrayWrapper(env, jfloatArray(res));
                    }else if(Java::Runtime::Double::isPrimitiveType(env, componentType)){
                        myObj = JDoubleArrayWrapper(env, jdoubleArray(res));
                    }else{
                        myObj = JObjectArrayWrapper(env, jobjectArray(res));
                    }
                }else{
                    myObj = JObjectWrapper(env, res);
                }
            }
        }catch(const JavaException& exn){
            if(noExceptionForwarding()){
                QtJambiExceptionBlocker __blocker;
                {
                    QtJambiExceptionHandler __handler;
                    __handler.handle(env, exn, "operator>>(QDataStream &, JObjectWrapper &)");
                }
                __blocker.release(env);
            }else
                __exnhandler.handle(env, exn, "operator>>(QDataStream &, JObjectWrapper &)");
        }
    }
    return in;
}

QDebug operator<<(QDebug out, const JObjectWrapper &myObj){
    if(JniEnvironment env{200}){
        QtJambiExceptionHandler __exnhandler;
        try{
            jobject dstream = QtJambiAPI::convertNativeToJavaObjectAsWrapper(env, &out, typeid(QDebug));
            QTJAMBI_INVALIDATE_AFTER_USE(env, dstream);
            Java::QtJambi::MetaTypeUtility::debugObject(env, dstream, myObj.object(env));
        }catch(const JavaException& exn){
            if(noExceptionForwarding()){
                QtJambiExceptionBlocker __blocker;
                {
                    QtJambiExceptionHandler __handler;
                    __handler.handle(env, exn, "operator<<(QDataStream &, const JObjectWrapper &)");
                }
                __blocker.release(env);
            }else
                __exnhandler.handle(env, exn, "operator<<(QDataStream &, const JObjectWrapper &)");
        }
    }
    return out;
}

hash_type qHash(const JObjectWrapper &value, hash_type seed)
{
    if(JniEnvironment env{200}){
        if(value.object(env)){
            if(seed)
                return qHash(Java::Runtime::Object::hashCode(env, value.object(env)), seed);
            else return hash_type(Java::Runtime::Object::hashCode(env, value.object(env)));
        }
    }
    return seed;
}



template<typename JType>
JArrayWrapper<JType>::JArrayWrapper(): JObjectWrapper() {}

template<typename JType>
JArrayWrapper<JType>::JArrayWrapper(JNIEnv *env, ArrayType obj, bool globalRefs)
    : JObjectWrapper(env, obj, globalRefs, typeid(JType)) {
      };

template<typename JType>
JArrayWrapper<JType>::JArrayWrapper(ArrayType obj)
    : JObjectWrapper() {
    if(JniEnvironment env{500}){
        JArrayWrapper<JType>::assign(env, obj);
    }
}

template<typename JType>
JArrayWrapper<JType>::JArrayWrapper(const JArrayWrapper& other): JObjectWrapper(other) {}

template<typename JType>
JArrayWrapper<JType>::JArrayWrapper(JArrayWrapper&& other): JObjectWrapper(std::move(other)) {}

template<typename JType>
JArrayWrapper<JType>::JArrayWrapper(const JObjectWrapper& wrapper): JObjectWrapper() {
    JArrayWrapper<JType>::assign(wrapper);
}

template<typename JType>
JArrayWrapper<JType>::JArrayWrapper(JObjectWrapper&& wrapper): JObjectWrapper() {
    if(JniEnvironment env{500}){
        JArrayWrapper<JType>::assign(env, std::move(wrapper));
    }
}

template<typename JType>
JArrayWrapper<JType>::~JArrayWrapper() {}

template<typename JType>
jsize JArrayWrapper<JType>::length() const{
    return JObjectWrapper::arrayLength();
}

template<typename JType>
JType JArrayWrapper<JType>::operator[](jsize index) const{
    if(JniEnvironment env{500}){
        typename JArrayWrapper<JType>::ArrayType obj = object(env);
        if(index>=0 && index < env->GetArrayLength(obj)){
            const JType* _array = array();
            JType result;
            if(!_array){
                JType* array = (env->*JArray<JType>::GetArrayElements)(obj, nullptr);
                result = array[index];
                (env->*JArray<JType>::ReleaseArrayElements)(obj, array, 0);
            }else{
                result = _array[index];
            }
            return result;
        }
    }
    return 0;
}

template<typename JType>
JArrayAccessRef<JType> JArrayWrapper<JType>::operator[](jsize index){
    if(index>=0 && index < length()){
        return JArrayAccessRef<JType>(*this, index);
    }
    return JArrayAccessRef<JType>(JObjectWrapper(), 0);
}

template<typename JType>
QString JArrayWrapper<JType>::toString(JNIEnv*, bool * ok) const{
    QString result = QLatin1String("[");
    jsize _length = length();
    for(jsize i=0; i<_length; ++i){
        if(i>0)
            result += QLatin1String(",");
        JType value = (*this)[i];
        result += QString::number(value);
    }
    result += QLatin1String("]");
    if(ok)
        *ok = true;
    return result;
}

template<typename JType>
void JArrayWrapper<JType>::assign(JNIEnv* env, jobject object){
        object = filterPrimitiveArray(env, object, typeid(JType));
        if(object){
            m_data = std::move(JArrayWrapper<JType>(env, ArrayType(object)).m_data);
        }else{
            m_data.reset();
        }
}

template<typename JType>
void JArrayWrapper<JType>::assign(const JObjectWrapper &wrapper){
    if(JniEnvironment env{500}){
        if(filterPrimitiveArray(env, wrapper.object(env), typeid(JType))){
            if(JObjectWrapper::isEquals(typeid(wrapper), typeid(*this))){
                m_data = wrapper.m_data;
            }else if(JObjectWrapper::isEquals(typeid(wrapper), typeid(JObjectWrapper))){
                JObjectWrapper::assign(env, wrapper, typeid(JType));
            }else{
                m_data.reset();
            }
        }else{
            m_data.reset();
        }
    }
}

template<typename JType>
void JArrayWrapper<JType>::assign(JNIEnv *env, JObjectWrapper &&wrapper){
    if(filterPrimitiveArray(env, wrapper.object(env), typeid(JType))){
        if(JObjectWrapper::isEquals(typeid(wrapper), typeid(*this))){
            m_data = std::move(wrapper.m_data);
        }else if(JObjectWrapper::isEquals(typeid(wrapper), typeid(JObjectWrapper))){
            JObjectWrapper::assign(env, wrapper, typeid(JType));
        }else{
            m_data.reset();
        }
    }else{
        m_data.reset();
    }
}

template<typename JType>
const JType* JArrayWrapper<JType>::array() const
{
    return reinterpret_cast<const JType*>(JObjectWrapper::array());
}

template<typename JType>
JType* JArrayWrapper<JType>::array()
{
    return reinterpret_cast<JType*>(JObjectWrapper::array());
}

template<typename JType>
JArrayWrapper<JType>::operator const JType*() const{return array();}

template<typename JType>
JArrayWrapper<JType>::operator JType*(){return array();}

template<typename JType>
typename JArrayWrapper<JType>::ArrayType JArrayWrapper<JType>::object(JNIEnv *env) const { return ArrayType(JObjectWrapper::object(env)); }

template<typename JType>
JArrayWrapper<JType>::operator ArrayType() const{ return ArrayType(JObjectWrapper::operator jobject()); }

template<typename JType>
JArrayWrapper<JType>& JArrayWrapper<JType>::operator=(const JArrayWrapper &wrapper){
    m_data = wrapper.m_data;
    return *this;
}

template<typename JType>
JArrayWrapper<JType>& JArrayWrapper<JType>::operator=(JArrayWrapper &&wrapper){
    m_data = std::move(wrapper.m_data);
    return *this;
}

template<typename JType>
JArrayWrapper<JType>& JArrayWrapper<JType>::operator=(const JObjectWrapper &wrapper) {
    if(JniEnvironment env{500}){
        if(filterPrimitiveArray(env, wrapper.object(env), typeid(JType))){
            m_data = wrapper.m_data;
        }else{
            m_data.reset();
        }
    }
    return *this;
}

template<typename JType>
JArrayWrapper<JType>& JArrayWrapper<JType>::operator=(JObjectWrapper &&wrapper) {
    if(JniEnvironment env{500}){
        if(filterPrimitiveArray(env, wrapper.object(env), typeid(JType))){
            m_data = std::move(wrapper.m_data);
        }else{
            m_data.reset();
        }
    }
    return *this;
}

template class QTJAMBI_EXPORT JArrayWrapper<jint>;

template class QTJAMBI_EXPORT JArrayWrapper<jlong>;

template class QTJAMBI_EXPORT JArrayWrapper<jshort>;

template class QTJAMBI_EXPORT JArrayWrapper<jbyte>;

template class QTJAMBI_EXPORT JArrayWrapper<jboolean>;

template class QTJAMBI_EXPORT JArrayWrapper<jchar>;

template class QTJAMBI_EXPORT JArrayWrapper<jfloat>;

template class QTJAMBI_EXPORT JArrayWrapper<jdouble>;
