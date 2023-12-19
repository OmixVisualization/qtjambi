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

template<typename Cleanup>
void reference_cleanup(jobject object){
    try{
        if(object && !QCoreApplication::closingDown()){
            DEREF_JOBJECT;
            if(DefaultJniEnvironment env{200}){
                jthrowable throwable = nullptr;
                if(env->ExceptionCheck()){
                    throwable = env->ExceptionOccurred();
                    env->ExceptionClear();
                }
                (env->*Cleanup::DeleteRef)(typename Cleanup::RefType(object));
                if(throwable){
                    env->Throw(throwable);
                }
            }
        }
    }catch(...){}
}

struct JObjectGlobalWrapperCleanup{
    constexpr static auto NewRef = &JNIEnv::NewGlobalRef;
    constexpr static auto DeleteRef = &JNIEnv::DeleteGlobalRef;
    typedef jobject RefType;
    constexpr static void(*cleanup)(jobject) = &reference_cleanup<JObjectGlobalWrapperCleanup>;
};

struct JObjectWeakWrapperCleanup{
    constexpr static auto NewRef = &JNIEnv::NewWeakGlobalRef;
    constexpr static auto DeleteRef = &JNIEnv::DeleteWeakGlobalRef;
    typedef jweak RefType;
    constexpr static void(*cleanup)(jobject) = &reference_cleanup<JObjectWeakWrapperCleanup>;
};

template<typename Cleanup>
class JObjectRefWrapperData : public JObjectWrapperData{
public:
    JObjectRefWrapperData() = default;
    JObjectRefWrapperData(JNIEnv* env, jobject object)
        : JObjectWrapperData(), pointer( (env->*Cleanup::NewRef)(object) )
    {
    }

    ~JObjectRefWrapperData() override = default;
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
            }
        }
    }

    jobject data() const override {return pointer.data();}
    const void* array() const override {return nullptr;}
    void* array() override {return nullptr;}
    void commitArray() override {}
    jsize arrayLength() const override {return 0;}
private:
    QScopedPointer<_jobject, Cleanup> pointer;
};

typedef JObjectRefWrapperData<JObjectGlobalWrapperCleanup> JObjectGlobalWrapperData;
typedef JObjectRefWrapperData<JObjectWeakWrapperCleanup> JObjectWeakWrapperData;

template<typename JType, typename Cleanup>
class JArrayWrapperData : public JObjectWrapperData{
protected:
    typedef typename JArray<JType>::Type ArrayType;
public:
    JArrayWrapperData() = default;
    JArrayWrapperData(JNIEnv* env, ArrayType object)
        : pointer( (env->*Cleanup::NewRef)(object) ),
          m_isCopy(false),
          m_length( object ? env->GetArrayLength(object) : 0 ),
          m_array( object ? (env->*JArray<JType>::GetArrayElements)(object, &m_isCopy) : nullptr )
    {}

    ~JArrayWrapperData() override{
        try{
            if(DefaultJniEnvironment env{500}){
                try{
                    (env->*JArray<JType>::ReleaseArrayElements)(JArrayWrapperData::data(), m_array, 0);
                }catch(const JavaException& exn){
                    exn.report(env);
                } catch (...) {}
            }
        } catch (const std::exception& e) {
            qCWarning(DebugAPI::internalCategory, "%s", e.what());
        }catch(...){}
    }
    void clear(JNIEnv *env) override{
        ArrayType array = ArrayType(env->NewLocalRef(JArrayWrapperData::data()));
        if(array){
            (env->*JArray<JType>::ReleaseArrayElements)(array, m_array, 0);
            m_array = nullptr;
            m_length = 0;
            DEREF_JOBJECT;
            jthrowable throwable = nullptr;
            if(env->ExceptionCheck()){
                throwable = env->ExceptionOccurred();
                env->ExceptionClear();
            }
            env->DeleteLocalRef(array);
            if(throwable)
                env->Throw(throwable);
        }
    }

    ArrayType data() const override {return ArrayType(pointer.data());}
    const void* array() const override  {return m_array;}
    void* array() override  {return m_array;}
    jsize arrayLength() const override {return m_length;}
    void commitArray() override {
        if(m_isCopy){
            if(JniEnvironment env{500}){
                ArrayType array = JArrayWrapperData::data();
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

template<typename JType>
struct JArrayGlobalWrapperData : JArrayWrapperData<JType,JObjectGlobalWrapperCleanup>{
    JArrayGlobalWrapperData() = default;
    JArrayGlobalWrapperData(JNIEnv* env, typename JArrayWrapperData<JType,JObjectGlobalWrapperCleanup>::ArrayType object) : JArrayWrapperData<JType,JObjectGlobalWrapperCleanup>(env, object){}
};

template<typename JType>
struct JArrayWeakWrapperData : JArrayWrapperData<JType,JObjectWeakWrapperCleanup>{
    JArrayWeakWrapperData() = default;
    JArrayWeakWrapperData(JNIEnv* env, typename JArrayWrapperData<JType,JObjectWeakWrapperCleanup>::ArrayType object) : JArrayWrapperData<JType,JObjectWeakWrapperCleanup>(env, object){}
};

bool JObjectWrapper::operator==(const JObjectWrapper &other) const
{
    return (*this)==other.object();
}

bool JObjectWrapper::operator==(jobject otherObject) const
{
    jobject myObject = object();
    if (!myObject && !otherObject){
        return true;
    }else if (!myObject || !otherObject){
        return false;
    }else if(JniEnvironment env{500}){
        Q_ASSERT(Java::Runtime::Object::isInstanceOf(env, myObject));  // check the java object is right type (objects on JVM don't have to inherit java.lang.Object)
        return Java::Runtime::Object::equals(env, myObject, otherObject);
    }else{
        return false;
    }
}

bool JObjectWrapper::operator<(const JObjectWrapper &other) const
{
    return (*this)<other.object();
}

bool JObjectWrapper::operator<(jobject otherObject) const
{
    jobject myObject = object();
    if (!myObject && !otherObject){
        return false;
    }else if (!myObject || !otherObject){
        return false;
    }else if(JniEnvironment env{200}){
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
    }else{
        return false;
    }
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
            if(!env->IsSameObject(obj, nullptr)){
                REF_JOBJECT;
                m_data = static_cast<JObjectWrapperData*>(new JObjectGlobalWrapperData(env, obj));
            }
        }
    }
}

JObjectWrapper::JObjectWrapper(JNIEnv *env, jobject obj, bool globalRefs)
    : m_data()
{
    if(!env->IsSameObject(obj, nullptr)){
        REF_JOBJECT;
        m_data = globalRefs ? static_cast<JObjectWrapperData*>(new JObjectGlobalWrapperData(env, obj)) : static_cast<JObjectWrapperData*>(new JObjectWeakWrapperData(env, obj));
    }
}

JObjectWrapper::JObjectWrapper(JNIEnv *env, jobject obj, bool globalRefs, const std::type_info& typeId)
 : m_data()
{
    if(!env->IsSameObject(obj, nullptr)){
        REF_JOBJECT;
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

void JObjectWrapper::assign(JNIEnv* env, const JObjectWrapper& wrapper, const std::type_info& typeId)
{
    if(JObjectWrapperData* wrappersData = wrapper.m_data.data()){
        const std::type_info& dataTypeId = typeid(*wrappersData);
        if(typeid_equals(typeId, typeid(jint))){
            if(typeid_equals(dataTypeId, typeid(JObjectGlobalWrapperData))){
                m_data = static_cast<JObjectWrapperData*>(new JArrayGlobalWrapperData<jint>(env, jintArray(wrapper.object())));
            }else if(typeid_equals(dataTypeId, typeid(JObjectWeakWrapperData))){
                m_data = static_cast<JObjectWrapperData*>(new JArrayWeakWrapperData<jint>(env, jintArray(wrapper.object())));
            }else if(typeid_equals(dataTypeId, typeid(JArrayGlobalWrapperData<jint>)) || typeid_equals(dataTypeId, typeid(JArrayWeakWrapperData<jint>))){
                m_data = wrapper.m_data;
            }else{
                m_data.reset();
            }
        }else if(typeid_equals(typeId, typeid(jbyte))){
            if(typeid_equals(dataTypeId, typeid(JObjectGlobalWrapperData))){
                m_data = static_cast<JObjectWrapperData*>(new JArrayGlobalWrapperData<jbyte>(env, jbyteArray(wrapper.object())));
            }else if(typeid_equals(dataTypeId, typeid(JObjectWeakWrapperData))){
                m_data = static_cast<JObjectWrapperData*>(new JArrayWeakWrapperData<jbyte>(env, jbyteArray(wrapper.object())));
            }else if(typeid_equals(dataTypeId, typeid(JArrayGlobalWrapperData<jbyte>)) || typeid_equals(dataTypeId, typeid(JArrayWeakWrapperData<jbyte>))){
                m_data = wrapper.m_data;
            }else{
                m_data.reset();
            }
        }else if(typeid_equals(typeId, typeid(jshort))){
            if(typeid_equals(dataTypeId, typeid(JObjectGlobalWrapperData))){
                m_data = static_cast<JObjectWrapperData*>(new JArrayGlobalWrapperData<jshort>(env, jshortArray(wrapper.object())));
            }else if(typeid_equals(dataTypeId, typeid(JObjectWeakWrapperData))){
                m_data = static_cast<JObjectWrapperData*>(new JArrayWeakWrapperData<jshort>(env, jshortArray(wrapper.object())));
            }else if(typeid_equals(dataTypeId, typeid(JArrayGlobalWrapperData<jshort>)) || typeid_equals(dataTypeId, typeid(JArrayWeakWrapperData<jshort>))){
                m_data = wrapper.m_data;
            }else{
                m_data.reset();
            }
        }else if(typeid_equals(typeId, typeid(jlong))){
            if(typeid_equals(dataTypeId, typeid(JObjectGlobalWrapperData))){
                m_data = static_cast<JObjectWrapperData*>(new JArrayGlobalWrapperData<jlong>(env, jlongArray(wrapper.object())));
            }else if(typeid_equals(dataTypeId, typeid(JObjectWeakWrapperData))){
                m_data = static_cast<JObjectWrapperData*>(new JArrayWeakWrapperData<jlong>(env, jlongArray(wrapper.object())));
            }else if(typeid_equals(dataTypeId, typeid(JArrayGlobalWrapperData<jlong>)) || typeid_equals(dataTypeId, typeid(JArrayWeakWrapperData<jlong>))){
                m_data = wrapper.m_data;
            }else{
                m_data.reset();
            }
        }else if(typeid_equals(typeId, typeid(jchar))){
            if(typeid_equals(dataTypeId, typeid(JObjectGlobalWrapperData))){
                m_data = static_cast<JObjectWrapperData*>(new JArrayGlobalWrapperData<jchar>(env, jcharArray(wrapper.object())));
            }else if(typeid_equals(dataTypeId, typeid(JObjectWeakWrapperData))){
                m_data = static_cast<JObjectWrapperData*>(new JArrayWeakWrapperData<jchar>(env, jcharArray(wrapper.object())));
            }else if(typeid_equals(dataTypeId, typeid(JArrayGlobalWrapperData<jchar>)) || typeid_equals(dataTypeId, typeid(JArrayWeakWrapperData<jchar>))){
                m_data = wrapper.m_data;
            }else{
                m_data.reset();
            }
        }else if(typeid_equals(typeId, typeid(jboolean))){
            if(typeid_equals(dataTypeId, typeid(JObjectGlobalWrapperData))){
                m_data = static_cast<JObjectWrapperData*>(new JArrayGlobalWrapperData<jboolean>(env, jbooleanArray(wrapper.object())));
            }else if(typeid_equals(dataTypeId, typeid(JObjectWeakWrapperData))){
                m_data = static_cast<JObjectWrapperData*>(new JArrayWeakWrapperData<jboolean>(env, jbooleanArray(wrapper.object())));
            }else if(typeid_equals(dataTypeId, typeid(JArrayGlobalWrapperData<jboolean>)) || typeid_equals(dataTypeId, typeid(JArrayWeakWrapperData<jboolean>))){
                m_data = wrapper.m_data;
            }else{
                m_data.reset();
            }
        }else if(typeid_equals(typeId, typeid(jfloat))){
            if(typeid_equals(dataTypeId, typeid(JObjectGlobalWrapperData))){
                m_data = static_cast<JObjectWrapperData*>(new JArrayGlobalWrapperData<jfloat>(env, jfloatArray(wrapper.object())));
            }else if(typeid_equals(dataTypeId, typeid(JObjectWeakWrapperData))){
                m_data = static_cast<JObjectWrapperData*>(new JArrayWeakWrapperData<jfloat>(env, jfloatArray(wrapper.object())));
            }else if(typeid_equals(dataTypeId, typeid(JArrayGlobalWrapperData<jfloat>)) || typeid_equals(dataTypeId, typeid(JArrayWeakWrapperData<jfloat>))){
                m_data = wrapper.m_data;
            }else{
                m_data.reset();
            }
        }else if(typeid_equals(typeId, typeid(jdouble))){
            if(typeid_equals(dataTypeId, typeid(JObjectGlobalWrapperData))){
                m_data = static_cast<JObjectWrapperData*>(new JArrayGlobalWrapperData<jdouble>(env, jdoubleArray(wrapper.object())));
            }else if(typeid_equals(dataTypeId, typeid(JObjectWeakWrapperData))){
                m_data = static_cast<JObjectWrapperData*>(new JArrayWeakWrapperData<jdouble>(env, jdoubleArray(wrapper.object())));
            }else if(typeid_equals(dataTypeId, typeid(JArrayGlobalWrapperData<jdouble>)) || typeid_equals(dataTypeId, typeid(JArrayWeakWrapperData<jdouble>))){
                m_data = wrapper.m_data;
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
    if(JObjectWrapperData* wrappersData = wrapper.m_data.data()){
        const std::type_info& dataTypeId = typeid(*wrappersData);
        if(typeid_equals(typeId, typeid(jint))){
            if(typeid_equals(dataTypeId, typeid(JObjectGlobalWrapperData))){
                m_data = static_cast<JObjectWrapperData*>(new JArrayGlobalWrapperData<jint>(env, jintArray(wrapper.object())));
            }else if(typeid_equals(dataTypeId, typeid(JObjectWeakWrapperData))){
                m_data = static_cast<JObjectWrapperData*>(new JArrayWeakWrapperData<jint>(env, jintArray(wrapper.object())));
            }else if(typeid_equals(dataTypeId, typeid(JArrayGlobalWrapperData<jint>)) || typeid_equals(dataTypeId, typeid(JArrayWeakWrapperData<jint>))){
                m_data = std::move(wrapper.m_data);
            }else{
                m_data.reset();
            }
        }else if(typeid_equals(typeId, typeid(jbyte))){
            if(typeid_equals(dataTypeId, typeid(JObjectGlobalWrapperData))){
                m_data = static_cast<JObjectWrapperData*>(new JArrayGlobalWrapperData<jbyte>(env, jbyteArray(wrapper.object())));
            }else if(typeid_equals(dataTypeId, typeid(JObjectWeakWrapperData))){
                m_data = static_cast<JObjectWrapperData*>(new JArrayWeakWrapperData<jbyte>(env, jbyteArray(wrapper.object())));
            }else if(typeid_equals(dataTypeId, typeid(JArrayGlobalWrapperData<jbyte>)) || typeid_equals(dataTypeId, typeid(JArrayWeakWrapperData<jbyte>))){
                m_data = std::move(wrapper.m_data);
            }else{
                m_data.reset();
            }
        }else if(typeid_equals(typeId, typeid(jshort))){
            if(typeid_equals(dataTypeId, typeid(JObjectGlobalWrapperData))){
                m_data = static_cast<JObjectWrapperData*>(new JArrayGlobalWrapperData<jshort>(env, jshortArray(wrapper.object())));
            }else if(typeid_equals(dataTypeId, typeid(JObjectWeakWrapperData))){
                m_data = static_cast<JObjectWrapperData*>(new JArrayWeakWrapperData<jshort>(env, jshortArray(wrapper.object())));
            }else if(typeid_equals(dataTypeId, typeid(JArrayGlobalWrapperData<jshort>)) || typeid_equals(dataTypeId, typeid(JArrayWeakWrapperData<jshort>))){
                m_data = std::move(wrapper.m_data);
            }else{
                m_data.reset();
            }
        }else if(typeid_equals(typeId, typeid(jlong))){
            if(typeid_equals(dataTypeId, typeid(JObjectGlobalWrapperData))){
                m_data = static_cast<JObjectWrapperData*>(new JArrayGlobalWrapperData<jlong>(env, jlongArray(wrapper.object())));
            }else if(typeid_equals(dataTypeId, typeid(JObjectWeakWrapperData))){
                m_data = static_cast<JObjectWrapperData*>(new JArrayWeakWrapperData<jlong>(env, jlongArray(wrapper.object())));
            }else if(typeid_equals(dataTypeId, typeid(JArrayGlobalWrapperData<jlong>)) || typeid_equals(dataTypeId, typeid(JArrayWeakWrapperData<jlong>))){
                m_data = std::move(wrapper.m_data);
            }else{
                m_data.reset();
            }
        }else if(typeid_equals(typeId, typeid(jchar))){
            if(typeid_equals(dataTypeId, typeid(JObjectGlobalWrapperData))){
                m_data = static_cast<JObjectWrapperData*>(new JArrayGlobalWrapperData<jchar>(env, jcharArray(wrapper.object())));
            }else if(typeid_equals(dataTypeId, typeid(JObjectWeakWrapperData))){
                m_data = static_cast<JObjectWrapperData*>(new JArrayWeakWrapperData<jchar>(env, jcharArray(wrapper.object())));
            }else if(typeid_equals(dataTypeId, typeid(JArrayGlobalWrapperData<jchar>)) || typeid_equals(dataTypeId, typeid(JArrayWeakWrapperData<jchar>))){
                m_data = std::move(wrapper.m_data);
            }else{
                m_data.reset();
            }
        }else if(typeid_equals(typeId, typeid(jboolean))){
            if(typeid_equals(dataTypeId, typeid(JObjectGlobalWrapperData))){
                m_data = static_cast<JObjectWrapperData*>(new JArrayGlobalWrapperData<jboolean>(env, jbooleanArray(wrapper.object())));
            }else if(typeid_equals(dataTypeId, typeid(JObjectWeakWrapperData))){
                m_data = static_cast<JObjectWrapperData*>(new JArrayWeakWrapperData<jboolean>(env, jbooleanArray(wrapper.object())));
            }else if(typeid_equals(dataTypeId, typeid(JArrayGlobalWrapperData<jboolean>)) || typeid_equals(dataTypeId, typeid(JArrayWeakWrapperData<jboolean>))){
                m_data = std::move(wrapper.m_data);
            }else{
                m_data.reset();
            }
        }else if(typeid_equals(typeId, typeid(jfloat))){
            if(typeid_equals(dataTypeId, typeid(JObjectGlobalWrapperData))){
                m_data = static_cast<JObjectWrapperData*>(new JArrayGlobalWrapperData<jfloat>(env, jfloatArray(wrapper.object())));
            }else if(typeid_equals(dataTypeId, typeid(JObjectWeakWrapperData))){
                m_data = static_cast<JObjectWrapperData*>(new JArrayWeakWrapperData<jfloat>(env, jfloatArray(wrapper.object())));
            }else if(typeid_equals(dataTypeId, typeid(JArrayGlobalWrapperData<jfloat>)) || typeid_equals(dataTypeId, typeid(JArrayWeakWrapperData<jfloat>))){
                m_data = std::move(wrapper.m_data);
            }else{
                m_data.reset();
            }
        }else if(typeid_equals(typeId, typeid(jdouble))){
            if(typeid_equals(dataTypeId, typeid(JObjectGlobalWrapperData))){
                m_data = static_cast<JObjectWrapperData*>(new JArrayGlobalWrapperData<jdouble>(env, jdoubleArray(wrapper.object())));
            }else if(typeid_equals(dataTypeId, typeid(JObjectWeakWrapperData))){
                m_data = static_cast<JObjectWrapperData*>(new JArrayWeakWrapperData<jdouble>(env, jdoubleArray(wrapper.object())));
            }else if(typeid_equals(dataTypeId, typeid(JArrayGlobalWrapperData<jdouble>)) || typeid_equals(dataTypeId, typeid(JArrayWeakWrapperData<jdouble>))){
                m_data = std::move(wrapper.m_data);
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
    if(m_data)
        return m_data->array();
    return nullptr;
}

void* JObjectWrapper::array()
{
    if(m_data)
        return m_data->array();
    return nullptr;
}

void JObjectWrapper::commitArray(){
    m_data->commitArray();
}

jsize JObjectWrapper::arrayLength() const {
    return m_data->arrayLength();
}

JObjectWrapper& JObjectWrapper::operator=(jobject object) {
    if(JniEnvironment env{500}){
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
    return *this;
}

JObjectWrapper& JObjectWrapper::operator=(const JObjectWrapper &wrapper) {
    m_data = wrapper.m_data;
    return *this;
}

JObjectWrapper& JObjectWrapper::operator=(JObjectWrapper &&wrapper) {
    if(JObjectWrapper::isEquals(typeid(*this), typeid(wrapper)) || !wrapper.object()){
        m_data = std::move(wrapper.m_data);
    }
    return *this;
}

jobject JObjectWrapper::object() const{
    return m_data ? m_data->data() : nullptr;
}

void JObjectWrapper::clear(JNIEnv *env){
    if(m_data){
        if(m_data->ref.loadRelaxed()==1){
            m_data->clear(env);
        }
        m_data.reset();
    }
}

JObjectWrapper::~JObjectWrapper(){
}

QString JObjectWrapper::toString(bool * ok) const {
    jobject _object = object();
    if(_object){
        if(JniEnvironment env{200}){
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
    }
    return "null";
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

JEnumWrapper& JEnumWrapper::operator=(jobject object) {
    if(JniEnvironment env{500}){
        object = filterEnum(env, object);
        if(object){
            m_data = static_cast<JObjectWrapperData*>(new JObjectGlobalWrapperData(env, object));
        }else{
            m_data.reset();
        }
    }
    return *this;
}

JEnumWrapper& JEnumWrapper::operator=(const JObjectWrapper &wrapper) {
    if(JniEnvironment env{500}){
        if(filterEnum(env, wrapper.object())){
            m_data = wrapper.m_data;
        }else{
            m_data.reset();
        }
    }
    return *this;
}

JEnumWrapper& JEnumWrapper::operator=(JObjectWrapper &&wrapper) {
    if(JniEnvironment env{500}){
        if(filterEnum(env, wrapper.object())){
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
    jobject _object = object();
    if(_object){
        if(JniEnvironment env{500}){
            return Java::Runtime::Enum::ordinal(env, _object);
        }
    }
    return -1;
}

JIteratorWrapper::JIteratorWrapper(JNIEnv *env, jobject obj, bool globalRefs)
    : JObjectWrapper(env, filterIterator(env, obj), globalRefs) {}

JIteratorWrapper::JIteratorWrapper(jobject obj)
    : JObjectWrapper(filterIterator(obj)) {}

JIteratorWrapper& JIteratorWrapper::operator=(jobject object) {
    if(JniEnvironment env{500}){
        object = filterIterator(env, object);
        if(object){
            m_data = static_cast<JObjectWrapperData*>(new JObjectGlobalWrapperData(env, object));
        }else{
            m_data.reset();
        }
    }
    return *this;
}

JIteratorWrapper& JIteratorWrapper::operator=(const JObjectWrapper &wrapper) {
    if(JniEnvironment env{500}){
        if(filterIterator(env, wrapper.object())){
            m_data = wrapper.m_data;
        }else{
            m_data.reset();
        }
    }
    return *this;
}

JIteratorWrapper& JIteratorWrapper::operator=(JObjectWrapper &&wrapper) {
    if(JniEnvironment env{500}){
        if(filterIterator(env, wrapper.object())){
            m_data = std::move(wrapper.m_data);
        }else{
            m_data.reset();
        }
    }
    return *this;
}

JIteratorWrapper& JIteratorWrapper::operator=(const JIteratorWrapper &wrapper) {
    m_data = wrapper.m_data;
    return *this;
}

JIteratorWrapper& JIteratorWrapper::operator=(JIteratorWrapper &&wrapper) {
    m_data = std::move(wrapper.m_data);
    return *this;
}

bool JIteratorWrapper::hasNext() const {
    jobject _object = object();
    if(_object){
        if(JniEnvironment env{500}){
            return Java::Runtime::Iterator::hasNext(env, _object);
        }
    }
    return false;
}

JObjectWrapper JIteratorWrapper::next() const {
    if(JniEnvironment env{500}){
        return JObjectWrapper(env, _next(env));
    }else{
        return JObjectWrapper();
    }
}

jobject JIteratorWrapper::_next(JNIEnv *env) const {
    jobject _object = object();
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

JCollectionWrapper& JCollectionWrapper::operator=(jobject object) {
    if(JniEnvironment env{500}){
        object = filterCollection(env, object);
        if(object){
            m_data = static_cast<JObjectWrapperData*>(new JObjectGlobalWrapperData(env, object));
        }else{
            m_data.reset();
        }
    }
    return *this;
}

JCollectionWrapper& JCollectionWrapper::operator=(const JObjectWrapper &wrapper) {
    if(JniEnvironment env{500}){
        if(filterCollection(env, wrapper.object())){
            m_data = wrapper.m_data;
        }else{
            m_data.reset();
        }
    }
    return *this;
}

JCollectionWrapper& JCollectionWrapper::operator=(JObjectWrapper &&wrapper) {
    if(JniEnvironment env{500}){
        if(filterCollection(env, wrapper.object())){
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
    jobject _object = object();
    if(_object){
        if(JniEnvironment env{200}){
           return Java::Runtime::Collection::size(env, _object);
        }
    }
    return 0;
}

JIteratorWrapper JCollectionWrapper::iterator() const {
    jobject _object = object();
    if(_object){
        if(JniEnvironment env{200}){
            jobject iter = Java::Runtime::Collection::iterator(env, _object);
            return JIteratorWrapper(env, iter);
        }
    }
    return JIteratorWrapper();
}

QList<QVariant> JCollectionWrapper::toList() const {
    QList<QVariant> list;
    if(object()){
        if(JniEnvironment env{200}){
            for (JIteratorWrapper iter = iterator(); iter.hasNext();) {
                jobject o = iter._next(env);
                list << QtJambiAPI::convertJavaObjectToQVariant(env, o);
            }
        }
    }
    return list;
}

QStringList JCollectionWrapper::toStringList(bool * ok) const {
    QStringList list;
    if(object()){
        if(JniEnvironment env{200}){
            for (JIteratorWrapper iter = iterator(); iter.hasNext();) {
                list << qtjambi_cast<QString>(env, iter._next(env));
            }
        }
    }
    if(ok) ok[0] = true;
    return list;
}

JMapWrapper::JMapWrapper(JNIEnv *env, jobject obj, bool globalRefs)
    : JObjectWrapper(env, filterMap(env, obj), globalRefs) {}

JMapWrapper::JMapWrapper(jobject obj)
    : JObjectWrapper(filterMap(obj)) {}

JMapWrapper& JMapWrapper::operator=(jobject object) {
    if(JniEnvironment env{500}){
        object = filterMap(env, object);
        if(object){
            m_data = static_cast<JObjectWrapperData*>(new JObjectGlobalWrapperData(env, object));
        }else{
            m_data.reset();
        }
    }
    return *this;
}

JMapWrapper& JMapWrapper::operator=(const JObjectWrapper &wrapper) {
    if(JniEnvironment env{500}){
        if(filterMap(env, wrapper.object())){
            m_data = wrapper.m_data;
        }else{
            m_data.reset();
        }
    }
    return *this;
}

JMapWrapper& JMapWrapper::operator=(JObjectWrapper &&wrapper) {
    if(JniEnvironment env{500}){
        if(filterMap(env, wrapper.object())){
            m_data = std::move(wrapper.m_data);
        }else{
            m_data.reset();
        }
    }
    return *this;
}

JMapWrapper& JMapWrapper::operator=(const JMapWrapper &wrapper) {
    m_data = wrapper.m_data;
    return *this;
}

JMapWrapper& JMapWrapper::operator=(JMapWrapper &&wrapper) {
    m_data = std::move(wrapper.m_data);
    return *this;
}

jobject JMapWrapper::_entrySet(JNIEnv* env) const {
    jobject _object = object();
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
    if(m_arrayWrapper.object()){
        if(JniEnvironment env{200}){
            env->SetObjectArrayElement(jobjectArray(m_arrayWrapper.object()), m_index, newValue);
            JavaException::check(env QTJAMBI_STACKTRACEINFO );
        }
    }
    return *this;
}

JObjectWrapperRef& JObjectWrapperRef::operator=(const JObjectWrapper &newValue)
{
    if(m_arrayWrapper.object()){
        if(JniEnvironment env{200}){
            env->SetObjectArrayElement(jobjectArray(m_arrayWrapper.object()), m_index, newValue.object());
            JavaException::check(env QTJAMBI_STACKTRACEINFO );
        }
    }
    return *this;
}

JObjectWrapperRef::operator JObjectWrapper() const
{
    if(m_arrayWrapper.object()){
        if(JniEnvironment env{200}){
            jobject value = env->GetObjectArrayElement(jobjectArray(m_arrayWrapper.object()), m_index);
            JavaException::check(env QTJAMBI_STACKTRACEINFO );
            return JObjectWrapper(env, value);
        }
    }
    return JObjectWrapper();
}

JObjectWrapperRef::operator jobject() const
{
    if(m_arrayWrapper.object()){
        if(JniEnvironment env{0}){
            env->PushLocalFrame(200);
            jobject value = env->GetObjectArrayElement(jobjectArray(m_arrayWrapper.object()), m_index);
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

JObjectArrayWrapper& JObjectArrayWrapper::operator=(jobject object) {
    if(JniEnvironment env{500}){
        object = filterObjectArray(env, object);
        if(object){
            m_data = static_cast<JObjectWrapperData*>(new JObjectGlobalWrapperData(env, object));
        }else{
            m_data.reset();
        }
    }
    return *this;
}

JObjectArrayWrapper& JObjectArrayWrapper::operator=(const JObjectWrapper &wrapper) {
    if(JniEnvironment env{500}){
        if(filterObjectArray(env, wrapper.object())){
            m_data = wrapper.m_data;
        }else{
            m_data.reset();
        }
    }
    return *this;
}

JObjectArrayWrapper& JObjectArrayWrapper::operator=(JObjectWrapper &&wrapper) {
    if(JniEnvironment env{500}){
        if(filterObjectArray(env, wrapper.object())){
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
        return env->GetArrayLength(object());
    }
    return 0;
}

jobject JObjectArrayWrapper::at(JNIEnv *env, jsize index) const{
    jobject value = env->GetObjectArrayElement(object(), index);
    JavaException::check(env QTJAMBI_STACKTRACEINFO );
    return value;
}

JObjectWrapper JObjectArrayWrapper::operator[](jsize index) const{
    if(JniEnvironment env{500}){
        jobject value = env->GetObjectArrayElement(object(), index);
        JavaException::check(env QTJAMBI_STACKTRACEINFO );
        return JObjectWrapper(env, value);
    }
    return JObjectWrapper();
}

JObjectWrapperRef JObjectArrayWrapper::operator[](jsize index){
    if(JniEnvironment env{500}){
        if(index>=0 && index < env->GetArrayLength(object())){
            return JObjectWrapperRef(*this, index);
        }else{
            JavaException::raiseIndexOutOfBoundsException(env, QString::number(index) QTJAMBI_STACKTRACEINFO);
        }
    }
    return JObjectWrapperRef(JObjectWrapper(), 0);
}

QString JObjectArrayWrapper::toString(bool * ok) const{
    if(ok)
        *ok = true;
    QString result = QLatin1String("[");
    if(JniEnvironment env{500}){
        jsize length = env->GetArrayLength(object());
        for(jsize i=0; i<length; ++i){
            if(i>0)
                result += QLatin1String(",");
            jobject value = env->GetObjectArrayElement(object(), i);
            JavaException::check(env QTJAMBI_STACKTRACEINFO );
            result += JObjectWrapper(env, value).toString(ok);
            if(ok && !*ok)
                return QString();
        }
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
            Java::QtJambi::MetaTypeUtility::writeSerializableJavaObject(env, jstream, myObj.object());
        }catch(const JavaException& exn){
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
            Java::QtJambi::MetaTypeUtility::debugObject(env, dstream, myObj.object());
        }catch(const JavaException& exn){
            __exnhandler.handle(env, exn, "operator<<(QDebug &, const JObjectWrapper &)");
        }
    }
    return out;
}

hash_type qHash(const JObjectWrapper &value, hash_type seed)
{
    if(JniEnvironment env{200}){
        if(value.object()){
            if(seed)
                return qHash(Java::Runtime::Object::hashCode(env, value.object()), seed);
            else return hash_type(Java::Runtime::Object::hashCode(env, value.object()));
        }
    }
    return seed;
}
