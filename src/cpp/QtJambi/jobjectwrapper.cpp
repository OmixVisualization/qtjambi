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

#include <QtCore/qcompilerdetection.h>
QT_WARNING_DISABLE_DEPRECATED

#include "pch_p.h"

QT_WARNING_DISABLE_GCC("-Wattributes")

class JObjectWrapperData : public QSharedData{
protected:
    JObjectWrapperData() = default;
public:
    enum Type{
        None,
        JObjectRef,
        JValueRef,
        JCollectionRef,
        JMapRef,
        JBooleanArrayRef,
        JIntArrayRef,
        JByteArrayRef,
        JShortArrayRef,
        JLongArrayRef,
        JDoubleArrayRef,
        JFloatArrayRef,
        JCharArrayRef,
        JObjectArrayRef
    };
    virtual ~JObjectWrapperData() = default;
    virtual void clear(JNIEnv *env) = 0;
    virtual bool isNull() const = 0;
    virtual jobject data() const = 0;
    virtual jobject data(JNIEnv *env) const = 0;
    virtual const void* array() const = 0;
    virtual void* array() = 0;
    virtual void commitArray() = 0;
    virtual jsize arrayLength() const = 0;
    virtual QString toString(JNIEnv *env, bool * ok = nullptr) const = 0;
    virtual Type type() const = 0;
    virtual bool isRefViaJava() const = 0;
    virtual bool isWeak() const = 0;
    static bool isJObjectValueWrapper(const JObjectWrapper& wrapper){
        return wrapper.m_data->type()==JValueRef;
    }
};

bool isJObjectValueWrapper(const JObjectWrapper& wrapper){
    return JObjectWrapperData::isJObjectValueWrapper(wrapper);
}

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
            }else{
                ThreadPrivate::doInPurgeThread([object](JNIEnv* env){
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
                });
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
    constexpr static bool isGlobal = true;
};

struct JObjectWeakWrapperUtil{
    constexpr static auto NewRef = &JNIEnv::NewWeakGlobalRef;
    constexpr static auto DeleteRef = &JNIEnv::DeleteWeakGlobalRef;
    typedef jweak RefType;
    constexpr static void(*cleanup)(jobject) = &reference_cleanup<JObjectWeakWrapperUtil>;
    constexpr static auto PushRef = &Java::QtJambi::NativeUtility::pushWeakGlobalReference;
    constexpr static auto GetRef = &Java::QtJambi::NativeUtility::getWeakGlobalReference;
    constexpr static auto ReleaseRef = &Java::QtJambi::NativeUtility::releaseWeakGlobalReference;
    constexpr static bool isGlobal = false;
};

template<typename Cleanup, int t = 0>
class JObjectRefWrapperData : public JObjectWrapperData{
public:
    Type type() const override {
        if constexpr(t==1){
            return JObjectWrapperData::JCollectionRef;
        }else if constexpr(t==2){
            return JObjectWrapperData::JMapRef;
        }else if constexpr(t==3){
            return JObjectWrapperData::JObjectArrayRef;
        }else if constexpr(t==4){
            return JObjectWrapperData::JValueRef;
        }else{
            return JObjectWrapperData::JObjectRef;
        }
    };
    bool isRefViaJava() const override { return false; }
    bool isWeak() const override { return std::is_same_v<Cleanup,JObjectWeakWrapperUtil>; }
    JObjectRefWrapperData() = default;
    JObjectRefWrapperData(JNIEnv* env, jobject object)
        : JObjectWrapperData()
        , pointer( !object || env->IsSameObject(object, nullptr) ? nullptr : (env->*Cleanup::NewRef)(object) ){}

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

    bool isNull() const override {
        if constexpr(Cleanup::isGlobal){
            return !pointer.data();
        }else{
            if(pointer.data()){
                if(DefaultJniEnvironment env{16}){
                    return env->IsSameObject(pointer.data(), nullptr);
                }
            }
            return true;
        }
    }

    jobject data() const override {
        return pointer.data();
    }

    jobject data(JNIEnv *env) const override {
        return pointer.data() ? env->NewLocalRef(pointer.data()) : nullptr;
    }
    const void* array() const override {return nullptr;}
    void* array() override {return nullptr;}
    void commitArray() override {}
    jsize arrayLength() const override {return 0;}
    QString toString(JNIEnv *env, bool * ok) const override {
        jobject _object = data(env);
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
private:
    QScopedPointer<_jobject, Cleanup> pointer;
};

template<typename Cleanup>
class JObjectArrayRefWrapperData : public JObjectRefWrapperData<Cleanup,3>{
public:
    JObjectArrayRefWrapperData() = default;
    JObjectArrayRefWrapperData(JNIEnv* env, jarray object)
        : JObjectRefWrapperData<Cleanup,3>(env, object)
        , m_arrayLength( object ? env->GetArrayLength(object) : 0 ){}
    jsize arrayLength() const override {return m_arrayLength;}
    QString toString(JNIEnv *env, bool * ok) const override {
        if(ok)
            *ok = true;
        QString result = QLatin1String("[");
        jobjectArray object = jobjectArray(JObjectRefWrapperData<Cleanup,3>::data(env));
        jsize length = object ? env->GetArrayLength(object) : 0;
        for(jsize i=0; i<length; ++i){
            if(i>0)
                result += QLatin1String(",");
            jobject value = env->GetObjectArrayElement(object, i);
            JavaException::check(env QTJAMBI_STACKTRACEINFO );
            result += JObjectWrapper(env, value).toString(ok);
            if(ok && !*ok)
                return QString();
        }
        result += QLatin1String("]");
        return result;
    }
private:
    jsize m_arrayLength = 0;
};

typedef JObjectRefWrapperData<JObjectGlobalWrapperUtil> JObjectGlobalWrapperData;
typedef JObjectRefWrapperData<JObjectGlobalWrapperUtil,4> JValueGlobalWrapperData;
typedef JObjectRefWrapperData<JObjectWeakWrapperUtil> JObjectWeakWrapperData;
typedef JObjectRefWrapperData<JObjectGlobalWrapperUtil,1> JCollectionGlobalWrapperData;
typedef JObjectRefWrapperData<JObjectWeakWrapperUtil,1> JCollectionWeakWrapperData;
typedef JObjectRefWrapperData<JObjectGlobalWrapperUtil,2> JMapGlobalWrapperData;
typedef JObjectRefWrapperData<JObjectWeakWrapperUtil,2> JMapWeakWrapperData;
typedef JObjectArrayRefWrapperData<JObjectGlobalWrapperUtil> JObjectArrayGlobalWrapperData;
typedef JObjectArrayRefWrapperData<JObjectWeakWrapperUtil> JObjectArrayWeakWrapperData;

template<typename Cleanup, int t = 0>
class JObjectRefViaJavaWrapperData : public JObjectWrapperData{
public:
    Type type() const override {
        if constexpr(t==1){
            return JObjectWrapperData::JCollectionRef;
        }else if constexpr(t==2){
            return JObjectWrapperData::JMapRef;
        }else if constexpr(t==3){
            return JObjectWrapperData::JObjectArrayRef;
        }else if constexpr(t==4){
            return JObjectWrapperData::JValueRef;
        }else{
            return JObjectWrapperData::JObjectRef;
        }
    };
    bool isRefViaJava() const override { return true; }
    bool isWeak() const override { return std::is_same_v<Cleanup,JObjectWeakWrapperUtil>; }
    JObjectRefViaJavaWrapperData() = default;
    JObjectRefViaJavaWrapperData(JNIEnv* env, jobject object)
        : JObjectWrapperData(), m_isNull(!object || env->IsSameObject(object, nullptr))
    {
        if(!m_isNull)
            Cleanup::PushRef(env, jlong(reinterpret_cast<void*>(this)), object);
    }

    ~JObjectRefViaJavaWrapperData() override{
        try{
            if(!m_isNull && !QCoreApplication::closingDown()){
                if(DefaultJniEnvironment env{0}){
                    JObjectRefViaJavaWrapperData<Cleanup,t>::clear(env);
                }
            }
        }catch(...){}
    }
    void clear(JNIEnv *env) override{
        if(!m_isNull){
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
    }

    bool isNull() const override {
        if constexpr(Cleanup::isGlobal){
            return m_isNull;
        }else{
            if(!m_isNull){
                if(DefaultJniEnvironment env{16}){
                    jthrowable throwable = nullptr;
                    if(env->ExceptionCheck()){
                        throwable = env->ExceptionOccurred();
                        env->ExceptionClear();
                    }
                    bool nul = env->IsSameObject(Cleanup::GetRef(env, jlong(reinterpret_cast<const void*>(this))), nullptr);
                    if(throwable){
                        env->Throw(throwable);
                        env->DeleteLocalRef(throwable);
                    }
                    return nul;
                }
            }
            return true;
        }
    }

    jobject data() const override {
        if(!m_isNull){
            if(DefaultJniEnvironment env{0}){
                env->PushLocalFrame(24);
                jthrowable throwable = nullptr;
                if(env->ExceptionCheck()){
                    throwable = env->ExceptionOccurred();
                    env->ExceptionClear();
                }
                jobject obj = Cleanup::GetRef(env, jlong(reinterpret_cast<const void*>(this)));
                if(throwable){
                    env->Throw(throwable);
                    env->DeleteLocalRef(throwable);
                }
                return env->PopLocalFrame(obj);
            }
        }
        return nullptr;
    }

    jobject data(JNIEnv *env) const override {
        if(m_isNull)
            return  nullptr;
        jthrowable throwable = nullptr;
        if(env->ExceptionCheck()){
            throwable = env->ExceptionOccurred();
            env->ExceptionClear();
        }
        jobject obj = m_isNull ? nullptr : Cleanup::GetRef(env, jlong(reinterpret_cast<const void*>(this)));
        if(throwable){
            env->Throw(throwable);
            env->DeleteLocalRef(throwable);
        }
        return obj;
    }
    const void* array() const override {return nullptr;}
    void* array() override {return nullptr;}
    void commitArray() override {}
    jsize arrayLength() const override {return 0;}
    QString toString(JNIEnv *env, bool * ok) const override {
        jobject _object = data(env);
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
    bool m_isNull = true;
};

template<typename Cleanup>
class JObjectArrayRefViaJavaWrapperData : public JObjectRefViaJavaWrapperData<Cleanup,3>{
public:
    JObjectArrayRefViaJavaWrapperData() = default;
    JObjectArrayRefViaJavaWrapperData(JNIEnv* env, jarray object)
        : JObjectRefViaJavaWrapperData<Cleanup,3>(env, object)
        , m_arrayLength( object ? env->GetArrayLength(object) : 0 ){}
    jsize arrayLength() const override {return m_arrayLength;}
    QString toString(JNIEnv *env, bool * ok) const override {
        if(ok)
            *ok = true;
        QString result = QLatin1String("[");
        jobjectArray object = jobjectArray(JObjectRefViaJavaWrapperData<Cleanup,3>::data(env));
        jsize length = object ? env->GetArrayLength(object) : 0;
        for(jsize i=0; i<length; ++i){
            if(i>0)
                result += QLatin1String(",");
            jobject value = env->GetObjectArrayElement(object, i);
            JavaException::check(env QTJAMBI_STACKTRACEINFO );
            result += JObjectWrapper(env, value).toString(ok);
            if(ok && !*ok)
                return QString();
        }
        result += QLatin1String("]");
        return result;
    }
private:
    jsize m_arrayLength = 0;
};

typedef JObjectRefViaJavaWrapperData<JObjectGlobalWrapperUtil> JObjectGlobalViaJavaWrapperData;
typedef JObjectRefViaJavaWrapperData<JObjectGlobalWrapperUtil,4> JValueGlobalViaJavaWrapperData;
typedef JObjectRefViaJavaWrapperData<JObjectWeakWrapperUtil> JObjectWeakViaJavaWrapperData;
typedef JObjectRefViaJavaWrapperData<JObjectGlobalWrapperUtil,1> JCollectionGlobalViaJavaWrapperData;
typedef JObjectRefViaJavaWrapperData<JObjectWeakWrapperUtil,1> JCollectionWeakViaJavaWrapperData;
typedef JObjectRefViaJavaWrapperData<JObjectGlobalWrapperUtil,2> JMapGlobalViaJavaWrapperData;
typedef JObjectRefViaJavaWrapperData<JObjectWeakWrapperUtil,2> JMapWeakViaJavaWrapperData;
typedef JObjectArrayRefViaJavaWrapperData<JObjectGlobalWrapperUtil> JObjectArrayGlobalViaJavaWrapperData;
typedef JObjectArrayRefViaJavaWrapperData<JObjectWeakWrapperUtil> JObjectArrayWeakViaJavaWrapperData;

template<typename ArrayType>
struct JArrayRuntime{
};

template<>
struct JArrayRuntime<jintArray>{
    constexpr static auto isPrimitiveType = Java::Runtime::Integer::isPrimitiveType;
    constexpr static auto GetArrayElements = &JNIEnv::GetIntArrayElements;
    constexpr static auto ReleaseArrayElements = &JNIEnv::ReleaseIntArrayElements;
    constexpr static auto ArrayRef = JObjectWrapperData::JIntArrayRef;
};

template<>
struct JArrayRuntime<jbyteArray>{
    constexpr static auto isPrimitiveType = Java::Runtime::Byte::isPrimitiveType;
    constexpr static auto GetArrayElements = &JNIEnv::GetByteArrayElements;
    constexpr static auto ReleaseArrayElements = &JNIEnv::ReleaseByteArrayElements;
    constexpr static auto ArrayRef = JObjectWrapperData::JByteArrayRef;
};

template<>
struct JArrayRuntime<jlongArray>{
    constexpr static auto isPrimitiveType = Java::Runtime::Long::isPrimitiveType;
    constexpr static auto GetArrayElements = &JNIEnv::GetLongArrayElements;
    constexpr static auto ReleaseArrayElements = &JNIEnv::ReleaseLongArrayElements;
    constexpr static auto ArrayRef = JObjectWrapperData::JLongArrayRef;
};

template<>
struct JArrayRuntime<jshortArray>{
    constexpr static auto isPrimitiveType = Java::Runtime::Short::isPrimitiveType;
    constexpr static auto GetArrayElements = &JNIEnv::GetShortArrayElements;
    constexpr static auto ReleaseArrayElements = &JNIEnv::ReleaseShortArrayElements;
    constexpr static auto ArrayRef = JObjectWrapperData::JShortArrayRef;
};

template<>
struct JArrayRuntime<jcharArray>{
    constexpr static auto isPrimitiveType = Java::Runtime::Character::isPrimitiveType;
    constexpr static auto GetArrayElements = &JNIEnv::GetCharArrayElements;
    constexpr static auto ReleaseArrayElements = &JNIEnv::ReleaseCharArrayElements;
    constexpr static auto ArrayRef = JObjectWrapperData::JCharArrayRef;
};

template<>
struct JArrayRuntime<jbooleanArray>{
    constexpr static auto isPrimitiveType = Java::Runtime::Boolean::isPrimitiveType;
    constexpr static auto GetArrayElements = &JNIEnv::GetBooleanArrayElements;
    constexpr static auto ReleaseArrayElements = &JNIEnv::ReleaseBooleanArrayElements;
    constexpr static auto ArrayRef = JObjectWrapperData::JBooleanArrayRef;
};

template<>
struct JArrayRuntime<jfloatArray>{
    constexpr static auto isPrimitiveType = Java::Runtime::Float::isPrimitiveType;
    constexpr static auto GetArrayElements = &JNIEnv::GetFloatArrayElements;
    constexpr static auto ReleaseArrayElements = &JNIEnv::ReleaseFloatArrayElements;
    constexpr static auto ArrayRef = JObjectWrapperData::JFloatArrayRef;
};

template<>
struct JArrayRuntime<jdoubleArray>{
    constexpr static auto isPrimitiveType = Java::Runtime::Double::isPrimitiveType;
    constexpr static auto GetArrayElements = &JNIEnv::GetDoubleArrayElements;
    constexpr static auto ReleaseArrayElements = &JNIEnv::ReleaseDoubleArrayElements;
    constexpr static auto ArrayRef = JObjectWrapperData::JDoubleArrayRef;
};

template<typename ArrayType, typename Cleanup>
class JArrayWrapperData : public JObjectWrapperData{
protected:
    typedef typename QtJambiPrivate::jni_type<ArrayType>::ElementType ElementType;
public:
    Type type() const override { return JArrayRuntime<ArrayType>::ArrayRef; };
    bool isRefViaJava() const override { return false; }
    bool isWeak() const override { return std::is_same_v<Cleanup,JObjectWeakWrapperUtil>; }
    JArrayWrapperData() = default;
    JArrayWrapperData(JNIEnv* env, ArrayType object)
        :
          pointer( !object || env->IsSameObject(object, nullptr) ? nullptr : (env->*Cleanup::NewRef)(object) ),
          m_isCopy(false),
          m_length( !object || env->IsSameObject(object, nullptr) ? 0 : env->GetArrayLength(object) ),
          m_array( !object || env->IsSameObject(object, nullptr) ? nullptr : (env->*JArrayRuntime<ArrayType>::GetArrayElements)(object, &m_isCopy) )
    {
    }

    ~JArrayWrapperData() override{
        if(pointer.data()){
            try{
                if(DefaultJniEnvironment env{500}){
                    JArrayWrapperData<ArrayType,Cleanup>::clear(env);
                }
            } catch (const std::exception& e) {
                qCWarning(DebugAPI::internalCategory, "%s", e.what());
            }catch(...){}
        }
    }
    void clear(JNIEnv *env) override{
        if(pointer.data()){
            ArrayType array = JArrayWrapperData<ArrayType,Cleanup>::data(env);
            if(array){
                (env->*JArrayRuntime<ArrayType>::ReleaseArrayElements)(array, m_array, 0);
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
    }

    ArrayType data(JNIEnv* env) const override {
        return pointer.data() ? ArrayType(env->NewLocalRef(pointer.data())) : nullptr;
    }

    bool isNull() const override {
        if constexpr(Cleanup::isGlobal){
            return !pointer.data();
        }else{
            if(pointer.data()){
                if(DefaultJniEnvironment env{16}){
                    return env->IsSameObject(pointer.data(), nullptr);
                }
            }
            return true;
        }
    }

    ArrayType data() const override {
        return pointer.data() ? ArrayType(pointer.data()) : nullptr;
    }
    const void* array() const override  {return m_array;}
    void* array() override  {return m_array;}
    jsize arrayLength() const override {return m_length;}
    void commitArray() override {
        if(m_isCopy){
            if(JniEnvironment env{500}){
                ArrayType array = JArrayWrapperData<ArrayType,Cleanup>::data(env);
                if(array){
                    (env->*JArrayRuntime<ArrayType>::ReleaseArrayElements)(array, m_array, 0);
                    m_array = (env->*JArrayRuntime<ArrayType>::GetArrayElements)(array, &m_isCopy);
                }
            }
        }
    }
    QString toString(JNIEnv*, bool * ok) const override {
        QString result = QLatin1String("[");
        for(jsize i=0; i<m_length; ++i){
            if(i>0)
                result += QLatin1String(",");
            ElementType value = m_array[i];
            result += QString::number(value);
        }
        result += QLatin1String("]");
        if(ok)
            *ok = true;
        return result;
    }
private:
    QScopedPointer<_jobject, Cleanup> pointer;
    jboolean m_isCopy = false;
    jsize m_length = 0;
    ElementType* m_array = nullptr;
};

template<typename ArrayType, typename Cleanup>
class JArrayViaJavaWrapperData : public JObjectWrapperData{
protected:
    typedef typename QtJambiPrivate::jni_type<ArrayType>::ElementType ElementType;
public:
    Type type() const override { return JArrayRuntime<ArrayType>::ArrayRef; };
    bool isRefViaJava() const override { return true; }
    bool isWeak() const override { return std::is_same_v<Cleanup,JObjectWeakWrapperUtil>; }
    JArrayViaJavaWrapperData() = default;
    JArrayViaJavaWrapperData(JNIEnv* env, ArrayType object)
        :
        m_isNull(!object || env->IsSameObject(object, nullptr)),
        m_isCopy(false),
        m_length( m_isNull ? 0 : env->GetArrayLength(object) ),
        m_array( m_isNull ? nullptr : (env->*JArrayRuntime<ArrayType>::GetArrayElements)(object, &m_isCopy) )
    {
        Cleanup::PushRef(env, jlong(reinterpret_cast<void*>(this)), object);
    }

    ~JArrayViaJavaWrapperData() override{
        if(!m_isNull){
            try{
                if(DefaultJniEnvironment env{500}){
                    JArrayViaJavaWrapperData<ArrayType,Cleanup>::clear(env);
                }
            } catch (const std::exception& e) {
                qCWarning(DebugAPI::internalCategory, "%s", e.what());
            }catch(...){}
        }
    }
    void clear(JNIEnv *env) override{
        if(!m_isNull){
            ArrayType array = JArrayViaJavaWrapperData<ArrayType,Cleanup>::data(env);
            if(array){
                (env->*JArrayRuntime<ArrayType>::ReleaseArrayElements)(array, m_array, 0);
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
    }

    bool isNull() const override {
        if constexpr(Cleanup::isGlobal){
            return m_isNull;
        }else{
            if(!m_isNull){
                if(DefaultJniEnvironment env{16}){
                    jthrowable throwable = nullptr;
                    if(env->ExceptionCheck()){
                        throwable = env->ExceptionOccurred();
                        env->ExceptionClear();
                    }
                    bool nul = env->IsSameObject(Cleanup::GetRef(env, jlong(reinterpret_cast<const void*>(this))), nullptr);
                    if(throwable){
                        env->Throw(throwable);
                        env->DeleteLocalRef(throwable);
                    }
                    return nul;
                }
            }
            return true;
        }
    }

    ArrayType data() const override {
        if(!m_isNull){
            if(DefaultJniEnvironment env{0}){
                env->PushLocalFrame(24);
                jthrowable throwable = nullptr;
                if(env->ExceptionCheck()){
                    throwable = env->ExceptionOccurred();
                    env->ExceptionClear();
                }
                jobject obj = Cleanup::GetRef(env, jlong(reinterpret_cast<const void*>(this)));
                if(throwable){
                    env->Throw(throwable);
                    env->DeleteLocalRef(throwable);
                }
                return ArrayType(env->PopLocalFrame(obj));
            }
        }
        return nullptr;
    }

    ArrayType data(JNIEnv *env) const override {
        if(m_isNull)
            return nullptr;
        jthrowable throwable = nullptr;
        if(env->ExceptionCheck()){
            throwable = env->ExceptionOccurred();
            env->ExceptionClear();
        }
        jobject obj = Cleanup::GetRef(env, jlong(reinterpret_cast<const void*>(this)));
        if(throwable){
            env->Throw(throwable);
            env->DeleteLocalRef(throwable);
        }
        return ArrayType(obj);
    }

    const void* array() const override  {return m_array;}
    void* array() override  {return m_array;}
    jsize arrayLength() const override {return m_length;}
    void commitArray() override {
        if(m_isCopy){
            if(JniEnvironment env{500}){
                ArrayType array = JArrayViaJavaWrapperData<ArrayType,Cleanup>::data(env);
                if(array){
                    (env->*JArrayRuntime<ArrayType>::ReleaseArrayElements)(array, m_array, 0);
                    m_array = (env->*JArrayRuntime<ArrayType>::GetArrayElements)(array, &m_isCopy);
                }
            }
        }
    }
    QString toString(JNIEnv*, bool * ok) const override {
        QString result = QLatin1String("[");
        for(jsize i=0; i<m_length; ++i){
            if(i>0)
                result += QLatin1String(",");
            ElementType value = m_array[i];
            result += QString::number(value);
        }
        result += QLatin1String("]");
        if(ok)
            *ok = true;
        return result;
    }
private:
    bool m_isNull = true;
    jboolean m_isCopy = false;
    jsize m_length = 0;
    ElementType* m_array = nullptr;
};

template<typename ArrayType>
struct JArrayGlobalWrapperData : JArrayWrapperData<ArrayType,JObjectGlobalWrapperUtil>{
    JArrayGlobalWrapperData() = default;
    JArrayGlobalWrapperData(JNIEnv* env, ArrayType object) : JArrayWrapperData<ArrayType,JObjectGlobalWrapperUtil>(env, object){}
};

template<typename ArrayType>
struct JArrayWeakWrapperData : JArrayWrapperData<ArrayType,JObjectWeakWrapperUtil>{
    JArrayWeakWrapperData() = default;
    JArrayWeakWrapperData(JNIEnv* env, ArrayType object) : JArrayWrapperData<ArrayType,JObjectWeakWrapperUtil>(env, object){}
};

template<typename ArrayType>
struct JArrayGlobalViaJavaWrapperData : JArrayViaJavaWrapperData<ArrayType,JObjectGlobalWrapperUtil>{
    JArrayGlobalViaJavaWrapperData() = default;
    JArrayGlobalViaJavaWrapperData(JNIEnv* env, ArrayType object) : JArrayViaJavaWrapperData<ArrayType,JObjectGlobalWrapperUtil>(env, object){}
};

template<typename ArrayType>
struct JArrayWeakViaJavaWrapperData : JArrayViaJavaWrapperData<ArrayType,JObjectWeakWrapperUtil>{
    JArrayWeakViaJavaWrapperData() = default;
    JArrayWeakViaJavaWrapperData(JNIEnv* env, ArrayType object) : JArrayViaJavaWrapperData<ArrayType,JObjectWeakWrapperUtil>(env, object){}
};

template<typename ArrayType>
ArrayType filterPrimitiveArray(JNIEnv *env, jobject object){
    if(object){
        jclass cls = env->GetObjectClass(object);
        if(Java::Runtime::Class::isArray(env, cls)){
            jclass componentType = Java::Runtime::Class::getComponentType(env, cls);
            if(JArrayRuntime<ArrayType>::isPrimitiveType(env, componentType)){
                return ArrayType(object);
            }
        }
    }
    return nullptr;
}

template<typename ArrayType>
JObjectWrapperData* createJArrayWrapperData(JNIEnv *env, jobject obj, bool globalRefs = true){
    if(env && env->ExceptionCheck()){
        env->ExceptionDescribe();
        env->ExceptionClear();
    }
    ArrayType objArray = filterPrimitiveArray<ArrayType>(env, obj);
    if(avoidJNIGlobalRefs(env)){
        if(globalRefs){
            return new JArrayGlobalViaJavaWrapperData<ArrayType>(env, objArray);
        }else{
            return new JArrayWeakViaJavaWrapperData<ArrayType>(env, objArray);
        }
    }else{
        if(globalRefs){
            return new JArrayGlobalWrapperData<ArrayType>(env, objArray);
        }else{
            return new JArrayWeakWrapperData<ArrayType>(env, objArray);
        }
    }
}

template<typename ArrayType>
JObjectWrapperData* createJArrayWrapperData(JNIEnv *env, ArrayType objArray, bool globalRefs = true){
    if(env && env->ExceptionCheck()){
        env->ExceptionDescribe();
        env->ExceptionClear();
    }
    if(avoidJNIGlobalRefs(env)){
        if(globalRefs){
            return new JArrayGlobalViaJavaWrapperData<ArrayType>(env, objArray);
        }else{
            return new JArrayWeakViaJavaWrapperData<ArrayType>(env, objArray);
        }
    }else{
        if(globalRefs){
            return new JArrayGlobalWrapperData<ArrayType>(env, objArray);
        }else{
            return new JArrayWeakWrapperData<ArrayType>(env, objArray);
        }
    }
}

template<typename ArrayType>
JObjectWrapperData* createJArrayWrapperData(bool globalRefs = true){
    if(avoidJNIGlobalRefs()){
        if(globalRefs){
            return new JArrayGlobalViaJavaWrapperData<ArrayType>();
        }else{
            return new JArrayWeakViaJavaWrapperData<ArrayType>();
        }
    }else{
        if(globalRefs){
            return new JArrayGlobalWrapperData<ArrayType>();
        }else{
            return new JArrayWeakWrapperData<ArrayType>();
        }
    }
}

jobjectArray filterObjectArray(JNIEnv *env, jobject object){
    if(object){
        jclass cls = env->GetObjectClass(object);
        if(Java::Runtime::Class::isArray(env, cls)){
            jclass componentType = Java::Runtime::Class::getComponentType(env, cls);
            if(!Java::Runtime::Class::isPrimitive(env, componentType)){
                return jobjectArray(object);
            }
        }
    }
    return nullptr;
}

JObjectWrapperData* createJObjectArrayWrapperData(JNIEnv *env, jobject obj, bool globalRefs = true){
    if(env && env->ExceptionCheck()){
        env->ExceptionDescribe();
        env->ExceptionClear();
    }
    jobjectArray objArray = filterObjectArray(env, obj);
    if(avoidJNIGlobalRefs(env)){
        if(globalRefs){
            return new JObjectArrayGlobalViaJavaWrapperData(env, objArray);
        }else{
            return new JObjectArrayWeakViaJavaWrapperData(env, objArray);
        }
    }else{
        if(globalRefs){
            return new JObjectArrayGlobalWrapperData(env, objArray);
        }else{
            return new JObjectArrayWeakWrapperData(env, objArray);
        }
    }
}

JObjectWrapperData* createJObjectArrayWrapperData(JNIEnv *env, jobjectArray objArray, bool globalRefs = true){
    if(env && env->ExceptionCheck()){
        env->ExceptionDescribe();
        env->ExceptionClear();
    }
    if(avoidJNIGlobalRefs(env)){
        if(globalRefs){
            return new JObjectArrayGlobalViaJavaWrapperData(env, objArray);
        }else{
            return new JObjectArrayWeakViaJavaWrapperData(env, objArray);
        }
    }else{
        if(globalRefs){
            return new JObjectArrayGlobalWrapperData(env, objArray);
        }else{
            return new JObjectArrayWeakWrapperData(env, objArray);
        }
    }
}

JObjectWrapperData* createJObjectArrayWrapperData(bool globalRefs = true){
    if(avoidJNIGlobalRefs()){
        if(globalRefs){
            return new JObjectArrayGlobalViaJavaWrapperData();
        }else{
            return new JObjectArrayWeakViaJavaWrapperData();
        }
    }else{
        if(globalRefs){
            return new JObjectArrayGlobalWrapperData();
        }else{
            return new JObjectArrayWeakWrapperData();
        }
    }
}

jobject filterMap(JNIEnv *env, jobject object){
    if(object && Java::Runtime::Map::isInstanceOf(env, object)){
        return object;
    }
    return nullptr;
}

JObjectWrapperData* createJMapWrapperData(JNIEnv *env, jobject obj, bool globalRefs = true){
    if(env && env->ExceptionCheck()){
        env->ExceptionDescribe();
        env->ExceptionClear();
    }
    obj = filterMap(env, obj);
    if(avoidJNIGlobalRefs(env)){
        if(globalRefs){
            return new JMapGlobalViaJavaWrapperData(env, obj);
        }else{
            return new JMapWeakViaJavaWrapperData(env, obj);
        }
    }else{
        if(globalRefs){
            return new JMapGlobalWrapperData(env, obj);
        }else{
            return new JMapWeakWrapperData(env, obj);
        }
    }
}

JObjectWrapperData* createJMapWrapperData(bool globalRefs = true){
    if(avoidJNIGlobalRefs()){
        if(globalRefs){
            return new JMapGlobalViaJavaWrapperData();
        }else{
            return new JMapWeakViaJavaWrapperData();
        }
    }else{
        if(globalRefs){
            return new JMapGlobalWrapperData();
        }else{
            return new JMapWeakWrapperData();
        }
    }
}

jobject filterCollection(JNIEnv *env, jobject object){
    if(object && Java::Runtime::Collection::isInstanceOf(env, object)){
        return object;
    }
    return nullptr;
}

JObjectWrapperData* createJCollectionWrapperData(JNIEnv *env, jobject obj, bool globalRefs = true){
    if(env && env->ExceptionCheck()){
        env->ExceptionDescribe();
        env->ExceptionClear();
    }
    obj = filterCollection(env, obj);
    if(avoidJNIGlobalRefs(env)){
        if(globalRefs){
            return new JCollectionGlobalViaJavaWrapperData(env, obj);
        }else{
            return new JCollectionWeakViaJavaWrapperData(env, obj);
        }
    }else{
        if(globalRefs){
            return new JCollectionGlobalWrapperData(env, obj);
        }else{
            return new JCollectionWeakWrapperData(env, obj);
        }
    }
}

JObjectWrapperData* createJCollectionWrapperData(bool globalRefs = true){
    if(avoidJNIGlobalRefs()){
        if(globalRefs){
            return new JCollectionGlobalViaJavaWrapperData();
        }else{
            return new JCollectionWeakViaJavaWrapperData();
        }
    }else{
        if(globalRefs){
            return new JCollectionGlobalWrapperData();
        }else{
            return new JCollectionWeakWrapperData();
        }
    }
}

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
    if(otherObject==nullptr && m_data.data()==nullptr){
        return true;
    }else if(JniEnvironment env{500}){
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
#if QT_VERSION >= QT_VERSION_CHECK(6, 9, 0)
        if(Java::QtJambi::QLongFlags::isInstanceOf(env, myObject) && otherObject && env->IsInstanceOf(otherObject, env->GetObjectClass(myObject))){
            jlong h1 = Java::QtJambi::QLongFlags::longValue(env, myObject);
            jlong h2 = Java::QtJambi::QLongFlags::longValue(env, otherObject);
            return h1<h2;
        }else
#endif
        if(Java::QtJambi::QFlags::isInstanceOf(env, myObject) && otherObject && env->IsInstanceOf(otherObject, env->GetObjectClass(myObject))){
            jint h1 = Java::QtJambi::QFlags::intValue(env, myObject);
            jint h2 = Java::QtJambi::QFlags::intValue(env, otherObject);
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
                if(avoidJNIGlobalRefs(env)){
                    m_data = static_cast<JObjectWrapperData*>(new JObjectGlobalViaJavaWrapperData(env, obj));
                }else{
                    m_data = static_cast<JObjectWrapperData*>(new JObjectGlobalWrapperData(env, obj));
                }
            }
        }
    }
}

JObjectWrapperData* createJObjectValueWrapperData(JNIEnv *env, jobject obj){
    if(avoidJNIGlobalRefs(env)){
        return new JObjectGlobalViaJavaWrapperData(env, obj);
    }else{
        return new JObjectGlobalWrapperData(env, obj);
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

JObjectWrapper::JObjectWrapper(JObjectWrapperData* data)
    : m_data(data)
{}

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
        assign(env, object, true);
    }
    return *this;
}

void JObjectWrapper::assign(JNIEnv *env, jobject object, bool globalRefs) {
    if(env->ExceptionCheck()){
        env->ExceptionDescribe();
        env->ExceptionClear();
    }
    JObjectWrapperData::Type type = JObjectWrapperData::None;
    if(m_data)
        type = m_data->type();
    switch(type){
    case JObjectWrapperData::JCollectionRef:
        m_data.reset(createJCollectionWrapperData(env, object, globalRefs));
        break;
    case JObjectWrapperData::JMapRef:
        m_data.reset(createJMapWrapperData(env, object, globalRefs));
        break;
    case JObjectWrapperData::JBooleanArrayRef:
        m_data.reset(createJArrayWrapperData<jbooleanArray>(env, object, globalRefs));
        break;
    case JObjectWrapperData::JIntArrayRef:
        m_data.reset(createJArrayWrapperData<jintArray>(env, object, globalRefs));
        break;
    case JObjectWrapperData::JByteArrayRef:
        m_data.reset(createJArrayWrapperData<jbyteArray>(env, object, globalRefs));
        break;
    case JObjectWrapperData::JShortArrayRef:
        m_data.reset(createJArrayWrapperData<jshortArray>(env, object, globalRefs));
        break;
    case JObjectWrapperData::JLongArrayRef:
        m_data.reset(createJArrayWrapperData<jlongArray>(env, object, globalRefs));
        break;
    case JObjectWrapperData::JDoubleArrayRef:
        m_data.reset(createJArrayWrapperData<jdoubleArray>(env, object, globalRefs));
        break;
    case JObjectWrapperData::JFloatArrayRef:
        m_data.reset(createJArrayWrapperData<jfloatArray>(env, object, globalRefs));
        break;
    case JObjectWrapperData::JCharArrayRef:
        m_data.reset(createJArrayWrapperData<jcharArray>(env, object, globalRefs));
        break;
    case JObjectWrapperData::JObjectArrayRef:
        m_data.reset(createJObjectArrayWrapperData(env, object, globalRefs));
        break;
    default:
        if(env->IsSameObject(object, nullptr)){
            m_data.reset();
        }else{
            if(avoidJNIGlobalRefs(env)){
                if(globalRefs){
                    m_data.reset(new JObjectGlobalViaJavaWrapperData(env, object));
                }else{
                    m_data.reset(new JObjectWeakViaJavaWrapperData(env, object));
                }
            }else{
                if(globalRefs){
                    m_data.reset(new JObjectGlobalWrapperData(env, object));
                }else{
                    m_data.reset(new JObjectWeakWrapperData(env, object));
                }
            }
        }
        break;
    }
}

JObjectWrapper& JObjectWrapper::operator=(const JObjectWrapper &wrapper) {
    assign(wrapper);
    return *this;
}

void JObjectWrapper::assign(const JObjectWrapper &wrapper) {
    JObjectWrapperData::Type type1 = m_data ? m_data->type() : JObjectWrapperData::None;
    JObjectWrapperData::Type type2 = wrapper.m_data ? wrapper.m_data->type() : JObjectWrapperData::None;
    if(type1==type2
        || (type1==JObjectWrapperData::None && type2==JObjectWrapperData::JObjectRef)
        || (type2==JObjectWrapperData::None && type1==JObjectWrapperData::JObjectRef)){
        m_data = wrapper.m_data;
    }else{
        m_data.reset();
        if(wrapper.m_data){
            if(JniEnvironment env{500}){
                assign(env, wrapper.object(env), !wrapper.m_data->isWeak());
            }
        }
    }
}

JObjectWrapper& JObjectWrapper::operator=(JObjectWrapper &&wrapper) {
    assign(std::move(wrapper));
    return *this;
}

void JObjectWrapper::assign(JObjectWrapper &&wrapper) {
    JObjectWrapperData::Type type1 = m_data ? m_data->type() : JObjectWrapperData::None;
    JObjectWrapperData::Type type2 = wrapper.m_data ? wrapper.m_data->type() : JObjectWrapperData::None;
    if(type1==type2
        || (type1==JObjectWrapperData::None && type2==JObjectWrapperData::JObjectRef)
        || (type2==JObjectWrapperData::None && type1==JObjectWrapperData::JObjectRef)){
        m_data = std::move(wrapper.m_data);
    }else{
        m_data.reset();
        if(wrapper.m_data){
            if(JniEnvironment env{500}){
                assign(env, wrapper.object(env), !wrapper.m_data->isWeak());
                wrapper.m_data.reset();
            }
        }
    }
}

bool JObjectWrapper::isInstanceOf(JNIEnv *env, jclass cls) const{
    if(QExplicitlySharedDataPointer<JObjectWrapperData> data{m_data}){
        jobject obj = data->data(env);
        return obj!=nullptr && env->IsInstanceOf(obj, cls);
    }
    return false;
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

bool JObjectWrapper::isNull() const {
    if(QExplicitlySharedDataPointer<JObjectWrapperData> data{m_data}){
        return data->isNull();
    }
    return true;
}

void JObjectWrapper::clear(JNIEnv *env){
    QExplicitlySharedDataPointer<JObjectWrapperData> data(std::move(m_data));
    if(data && data->ref.loadRelaxed()==1)
        data->clear(env);
}

JObjectWrapper::~JObjectWrapper(){
}

QString JObjectWrapper::toString(JNIEnv *env, bool * ok) const {
    return m_data->toString(env, ok);
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

JCollectionWrapper::JCollectionWrapper(JNIEnv *env, jobject obj, bool globalRefs)
    : JObjectWrapper(createJCollectionWrapperData(env, obj, globalRefs)) {}

JCollectionWrapper::JCollectionWrapper(jobject obj)
    : JCollectionWrapper(JniEnvironment{128}, obj) {}

JCollectionWrapper::JCollectionWrapper()
    : JObjectWrapper(createJCollectionWrapperData()) {}

void JCollectionWrapper::assign(JNIEnv *env, jobject object, bool globalRefs) {
    m_data.reset(createJCollectionWrapperData(env, object, globalRefs));
}

void JCollectionWrapper::assign(const JObjectWrapper &wrapper) {
    switch(wrapper.m_data->type()){
    case JObjectWrapperData::JCollectionRef:
        m_data = wrapper.m_data;
        break;
    case JObjectWrapperData::JObjectRef:
    case JObjectWrapperData::JValueRef:
        if(JniEnvironment env{500}){
            assign(env, wrapper.object(env), wrapper.m_data->isWeak());
        }
        break;
    default:
        m_data.reset();
        break;
    }
}

void JCollectionWrapper::assign(JObjectWrapper &&wrapper) {
    switch(wrapper.m_data->type()){
    case JObjectWrapperData::JCollectionRef:
        m_data = std::move(wrapper.m_data);
        break;
    case JObjectWrapperData::JObjectRef:
    case JObjectWrapperData::JValueRef:
        if(JniEnvironment env{500}){
            assign(env, wrapper.object(env), wrapper.m_data->isWeak());
        }
        break;
    default:
        m_data.reset();
        break;
    }
}

JCollectionWrapper& JCollectionWrapper::operator=(const JObjectWrapper &wrapper) {
    assign(wrapper);
    return *this;
}

JCollectionWrapper& JCollectionWrapper::operator=(JObjectWrapper &&wrapper) {
    assign(std::move(wrapper));
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

jobject JCollectionWrapper::at(JNIEnv *env, jsize index) const{
    if(jobject obj = object(env)){
        jobject value = nullptr;
        if(index<0 || index>=Java::Runtime::Collection::size(env, obj)){
            JavaException::raiseIndexOutOfBoundsException(env, QString::number(index) QTJAMBI_STACKTRACEINFO);
        }else if(Java::Runtime::List::isInstanceOf(env, obj)){
            value = Java::Runtime::List::get(env, obj, index);
        }else{
            jobject iterator = QtJambiAPI::iteratorOfJavaIterable(env, obj);
            for(int i=0; i<index; ++i){
                QtJambiAPI::nextOfJavaIterator(env, iterator);
            }
            value = QtJambiAPI::nextOfJavaIterator(env, iterator);
        }
        JavaException::check(env QTJAMBI_STACKTRACEINFO );
        return value;
    }else{
        JavaException::raiseNullPointerException(env, "collection is null" QTJAMBI_STACKTRACEINFO );
    }
    return nullptr;
}

JObjectWrapper JCollectionWrapper::operator[](jsize index) const{
    if(JniEnvironment env{500}){
        jobject value = at(env, index);
        return JObjectWrapper(env, value);
    }
    return JObjectWrapper();
}

JObjectWrapperRef JCollectionWrapper::operator[](jsize index){
    if(JniEnvironment env{500}){
        if(jobject obj = object(env)){
            if(index<0 || index>=Java::Runtime::Collection::size(env, obj)){
                JavaException::raiseIndexOutOfBoundsException(env, QString::number(index) QTJAMBI_STACKTRACEINFO);
            }
        }
    }
    return JObjectWrapperRef(*this, index);
}

QList<QVariant> JCollectionWrapper::toList() const {
    if(JniEnvironment env{200}){
        return JCollectionWrapper::toList(env);
    }
    return {};
}

QList<QVariant> JCollectionWrapper::toList(JNIEnv *env) const {
    QList<QVariant> list;
    jobject iter = Java::Runtime::Collection::iterator(env, object(env));
    while(Java::Runtime::Iterator::hasNext(env, iter)){
        jobject o = Java::Runtime::Iterator::next(env, iter);
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
    jobject iter = Java::Runtime::Collection::iterator(env, object(env));
    while(Java::Runtime::Iterator::hasNext(env, iter)){
        jobject o = Java::Runtime::Iterator::next(env, iter);
        list << qtjambi_cast<QString>(env, o);
    }
    if(ok) ok[0] = true;
    return list;
}

JMapWrapper::JMapWrapper()
    : JObjectWrapper(createJMapWrapperData()) {}

JMapWrapper::JMapWrapper(JNIEnv *env, jobject obj, bool globalRefs)
    : JObjectWrapper(createJMapWrapperData(env, obj, globalRefs)) {}

JMapWrapper::JMapWrapper(jobject obj)
    : JMapWrapper(JniEnvironment{128}, obj) {}

void JMapWrapper::assign(JNIEnv *env, jobject object, bool globalRefs) {
    m_data.reset(createJMapWrapperData(env, object, globalRefs));
}

void JMapWrapper::assign(const JObjectWrapper &wrapper) {
    switch(wrapper.m_data->type()){
    case JObjectWrapperData::JMapRef:
        m_data = wrapper.m_data;
        break;
    case JObjectWrapperData::JObjectRef:
    case JObjectWrapperData::JValueRef:
        if(JniEnvironment env{500}){
            assign(env, wrapper.object(env), wrapper.m_data->isWeak());
        }
        break;
    default:
        m_data.reset();
        break;
    }
}

void JMapWrapper::assign(JObjectWrapper &&wrapper) {
    switch(wrapper.m_data->type()){
    case JObjectWrapperData::JMapRef:
        m_data = std::move(wrapper.m_data);
        break;
    case JObjectWrapperData::JObjectRef:
    case JObjectWrapperData::JValueRef:
        if(JniEnvironment env{500}){
            assign(env, wrapper.object(env), wrapper.m_data->isWeak());
        }
        break;
    default:
        m_data.reset();
        break;
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
    assign(wrapper);
    return *this;
}

JMapWrapper& JMapWrapper::operator=(JObjectWrapper &&wrapper) {
    assign(std::move(wrapper));
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
        jobject iter = Java::Runtime::Collection::iterator(env, _entrySet(env));
        while(Java::Runtime::Iterator::hasNext(env, iter)){
            jobject entry = Java::Runtime::Iterator::next(env, iter);
            jobject key = Java::Runtime::Map$Entry::getKey(env, entry);
            jobject value = Java::Runtime::Map$Entry::getValue(env, entry);
            QVariant k(QtJambiAPI::convertJavaObjectToQVariant(env, key));
            QVariant v(QtJambiAPI::convertJavaObjectToQVariant(env, value));
            if(k.userType()!=QMetaType::UnknownType){
                if(type==QMetaType::UnknownType){
                    type = k.userType();
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
        jobject iter = Java::Runtime::Collection::iterator(env, _entrySet(env));
        while(Java::Runtime::Iterator::hasNext(env, iter)){
            jobject entry = Java::Runtime::Iterator::next(env, iter);
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
        jobject iter = Java::Runtime::Collection::iterator(env, _entrySet(env));
        while(Java::Runtime::Iterator::hasNext(env, iter)){
            jobject entry = Java::Runtime::Iterator::next(env, iter);
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

JObjectWrapperRef::JObjectWrapperRef(const JObjectWrapper& containerWrapper, jsize index) : m_containerWrapper(containerWrapper), m_index(index) {}

JObjectWrapperRef& JObjectWrapperRef::operator=(jobject newValue)
{
    if(JniEnvironment env{200}){
        assign(env, newValue);
    }
    return *this;
}

JObjectWrapperRef& JObjectWrapperRef::operator=(const JObjectWrapper &newValue)
{
    if(JniEnvironment env{200}){
        assign(env, newValue);
    }
    return *this;
}

void JObjectWrapperRef::assign(JNIEnv *env, jobject newValue)
{
    if(jobject object = m_containerWrapper.object(env)){
        switch(m_containerWrapper.m_data->type()){
        case JObjectWrapperData::JObjectArrayRef:
            env->SetObjectArrayElement(jobjectArray(object), m_index, newValue);
            JavaException::check(env QTJAMBI_STACKTRACEINFO );
            break;
        case JObjectWrapperData::JCollectionRef:
            Java::Runtime::List::set(env, object, m_index, newValue);
            break;
        default: break;
        }
    }
}

void JObjectWrapperRef::assign(JNIEnv *env, const JObjectWrapper &newValue)
{
    if(jobject object = m_containerWrapper.object(env)){
        switch(m_containerWrapper.m_data->type()){
        case JObjectWrapperData::JObjectArrayRef:
            env->SetObjectArrayElement(jobjectArray(object), m_index, newValue.object(env));
            JavaException::check(env QTJAMBI_STACKTRACEINFO );
            break;
        case JObjectWrapperData::JCollectionRef:
            Java::Runtime::List::set(env, object, m_index, newValue.object(env));
            break;
        default: break;
        }
    }
}

JObjectWrapperRef::operator JObjectWrapper() const
{
    if(JniEnvironment env{200}){
        if(jobject object = m_containerWrapper.object(env)){
            jobject value = nullptr;
            switch(m_containerWrapper.m_data->type()){
            case JObjectWrapperData::JObjectArrayRef:
                value = env->GetObjectArrayElement(jobjectArray(object), m_index);
                JavaException::check(env QTJAMBI_STACKTRACEINFO );
                break;
            case JObjectWrapperData::JCollectionRef:
                if(Java::Runtime::List::isInstanceOf(env, object)){
                    value = Java::Runtime::List::get(env, object, m_index);
                }else{
                    jobject iterator = QtJambiAPI::iteratorOfJavaIterable(env, object);
                    for(int i=0; i<m_index; ++i){
                        QtJambiAPI::nextOfJavaIterator(env, iterator);
                    }
                    value = QtJambiAPI::nextOfJavaIterator(env, iterator);
                }
                break;
            default: break;
            }
            return JObjectWrapper(env, value);
        }
    }
    return JObjectWrapper();
}

JObjectWrapperRef::operator jobject() const
{
    if(JniEnvironment env{0}){
        jobject value{nullptr};
        env->PushLocalFrame(200);
        if(jobject object = m_containerWrapper.object(env)){
            switch(m_containerWrapper.m_data->type()){
            case JObjectWrapperData::JObjectArrayRef:
                value = env->GetObjectArrayElement(jobjectArray(object), m_index);
                JavaException::check(env QTJAMBI_STACKTRACEINFO );
            break;
            case JObjectWrapperData::JCollectionRef:
                if(Java::Runtime::List::isInstanceOf(env, object)){
                    value = Java::Runtime::List::get(env, object, m_index);
                }else{
                    jobject iterator = QtJambiAPI::iteratorOfJavaIterable(env, object);
                    for(int i=0; i<m_index; ++i){
                        QtJambiAPI::nextOfJavaIterator(env, iterator);
                    }
                    value = QtJambiAPI::nextOfJavaIterator(env, iterator);
                }
                break;
            default: break;
            }
        }
        return env->PopLocalFrame(value);
    }
    return nullptr;
}

JObjectArrayWrapper::JObjectArrayWrapper()
    : JObjectWrapper(createJObjectArrayWrapperData()) {}

JObjectArrayWrapper::JObjectArrayWrapper(JNIEnv *env, jobjectArray obj, bool globalRefs)
    : JObjectWrapper(createJObjectArrayWrapperData(env, obj, globalRefs)) {}

JObjectArrayWrapper::JObjectArrayWrapper(jobjectArray obj)
    : JObjectArrayWrapper(JniEnvironment{128}, obj, true) {}

void JObjectArrayWrapper::assign(JNIEnv *env, jobject object, bool globalRefs) {
    m_data.reset(createJObjectArrayWrapperData(env, object, globalRefs));
}

void JObjectArrayWrapper::assign(JNIEnv *env, jobjectArray object, bool globalRefs) {
    m_data.reset(createJObjectArrayWrapperData(env, object, globalRefs));
}

void JObjectArrayWrapper::assign(const JObjectArrayWrapper &wrapper) {
    m_data = wrapper.m_data;
}

void JObjectArrayWrapper::assign(JObjectArrayWrapper &&wrapper) {
    m_data = std::move(wrapper.m_data);
}

void JObjectArrayWrapper::assign(const JObjectWrapper &wrapper) {
    if(wrapper.m_data){
        switch(wrapper.m_data->type()){
        case JObjectWrapperData::JObjectArrayRef:
            m_data = wrapper.m_data;
            break;
        case JObjectWrapperData::JObjectRef:
        case JObjectWrapperData::JValueRef:
            if(JniEnvironment env{500}){
                assign(env, wrapper.object(env), !wrapper.m_data->isWeak());
            }
            break;
        default:
            m_data.reset(createJObjectArrayWrapperData());
            break;
        }
    }else{
        m_data.reset(createJObjectArrayWrapperData());
    }
}

void JObjectArrayWrapper::assign(JObjectWrapper &&wrapper) {
    if(wrapper.m_data){
        switch(wrapper.m_data->type()){
        case JObjectWrapperData::JObjectArrayRef:
            m_data = std::move(wrapper.m_data);
            wrapper.m_data.reset(createJObjectArrayWrapperData());
            break;
        case JObjectWrapperData::JObjectRef:
        case JObjectWrapperData::JValueRef:
            if(JniEnvironment env{500}){
                assign(env, wrapper.object(env), !wrapper.m_data->isWeak());
                wrapper.m_data.reset();
            }
            break;
        default:
            m_data.reset(createJObjectArrayWrapperData());
            break;
        }
    }else{
        m_data.reset(createJObjectArrayWrapperData());
    }
}

JObjectArrayWrapper& JObjectArrayWrapper::operator=(const JObjectWrapper &wrapper) {
    assign(wrapper);
    return *this;
}

JObjectArrayWrapper& JObjectArrayWrapper::operator=(JObjectWrapper &&wrapper) {
    assign(std::move(wrapper));
    return *this;
}

JObjectArrayWrapper& JObjectArrayWrapper::operator=(const JObjectArrayWrapper &wrapper) {
    assign(wrapper);
    return *this;
}

JObjectArrayWrapper& JObjectArrayWrapper::operator=(JObjectArrayWrapper &&wrapper) {
    assign(std::move(wrapper));
    return *this;
}

JObjectArrayWrapper& JObjectArrayWrapper::operator=(jobject object) {
    if(JniEnvironment env{500}){
        assign(env, object);
    }
    return *this;
}

JObjectArrayWrapper& JObjectArrayWrapper::operator=(jobjectArray object) {
    if(JniEnvironment env{500}){
        assign(env, object);
    }
    return *this;
}

jsize JObjectArrayWrapper::length() const
{
    return m_data ? m_data->arrayLength() : 0;
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
        if(jobjectArray obj = object(env)){
            jobject value = env->GetObjectArrayElement(obj, index);
            JavaException::check(env QTJAMBI_STACKTRACEINFO );
            return JObjectWrapper(env, value);
        }else{
            JavaException::raiseIndexOutOfBoundsException(env, QString::number(index) QTJAMBI_STACKTRACEINFO);
        }
    }
    return JObjectWrapper();
}

JObjectWrapperRef JObjectArrayWrapper::operator[](jsize index){
    if(index>=0 && index < m_data->arrayLength()){
        return JObjectWrapperRef(*this, index);
    }else if(JniEnvironment env{500}){
        JavaException::raiseIndexOutOfBoundsException(env, QString::number(index) QTJAMBI_STACKTRACEINFO);
    }
    return JObjectWrapperRef(JObjectWrapper(), 0);
}

QDataStream &operator<<(QDataStream &out, const JObjectWrapper &myObj){
    if(JniEnvironment env{200}){
        QtJambiExceptionHandler __exnhandler;
        try{
            jobject jstream = QtJambiAPI::convertNativeToJavaObjectAsWrapper(env, &out, typeid(QDataStream));
            QTJAMBI_INVALIDATE_AFTER_USE(env, jstream);
            Java::QtJambi::MetaTypeUtility::writeSerializableJavaObject(env, jstream, myObj.object(env));
        }catch(const JavaException& exn){
            if(noExceptionForwarding(env)){
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
                myObj = JObjectWrapper();
            }else if(Java::Runtime::Collection::isInstanceOf(env, res)){
                myObj = JCollectionWrapper(env, res);
            }else if(Java::Runtime::Map::isInstanceOf(env, res)){
                myObj = JMapWrapper(env, res);
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
            if(noExceptionForwarding(env)){
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
            if(noExceptionForwarding(env)){
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

size_t qHash(const JObjectWrapper &value, size_t seed)
{
    if(JniEnvironment env{200}){
        if(value.object(env)){
            if(seed)
                return qHash(Java::Runtime::Object::hashCode(env, value.object(env)), seed);
            else return size_t(Java::Runtime::Object::hashCode(env, value.object(env)));
        }
    }
    return seed;
}



template<typename ArrayType>
JArrayWrapper<ArrayType>::JArrayWrapper()
    : JObjectWrapper(createJArrayWrapperData<ArrayType>()) {}

template<typename ArrayType>
JArrayWrapper<ArrayType>::JArrayWrapper(JNIEnv *env, ArrayType obj, bool globalRefs)
    : JObjectWrapper(createJArrayWrapperData<ArrayType>(env, obj, globalRefs)) {}

template<typename ArrayType>
JArrayWrapper<ArrayType>::JArrayWrapper(ArrayType obj)
    : JArrayWrapper(JniEnvironment{500}, obj, true) {}

template<typename ArrayType>
JArrayWrapper<ArrayType>::JArrayWrapper(const JArrayWrapper& other): JObjectWrapper(other) {}

template<typename ArrayType>
JArrayWrapper<ArrayType>::JArrayWrapper(JArrayWrapper&& other): JObjectWrapper(std::move(other)) {}

template<typename ArrayType>
JArrayWrapper<ArrayType>::JArrayWrapper(const JObjectWrapper& wrapper): JObjectWrapper() {
    JArrayWrapper<ArrayType>::assign(wrapper);
}

template<typename ArrayType>
JArrayWrapper<ArrayType>::JArrayWrapper(JObjectWrapper&& wrapper): JObjectWrapper() {
    JArrayWrapper<ArrayType>::assign(std::move(wrapper));
}

template<typename ArrayType>
JArrayWrapper<ArrayType>::~JArrayWrapper() {}

template<typename ArrayType>
jsize JArrayWrapper<ArrayType>::length() const{
    return JObjectWrapper::arrayLength();
}

template<typename ArrayType>
typename JArrayWrapper<ArrayType>::ElementType JArrayWrapper<ArrayType>::operator[](jsize index) const{
    if(JniEnvironment env{500}){
        ArrayType obj = object(env);
        if(index>=0 && index < env->GetArrayLength(obj)){
            const ElementType* _array = array();
            ElementType result;
            if(!_array){
                ElementType* array = (env->*JArrayRuntime<ArrayType>::GetArrayElements)(obj, nullptr);
                result = array[index];
                (env->*JArrayRuntime<ArrayType>::ReleaseArrayElements)(obj, array, 0);
            }else{
                result = _array[index];
            }
            return result;
        }
    }
    return 0;
}

template<typename ArrayType>
JArrayAccessRef<ArrayType> JArrayWrapper<ArrayType>::operator[](jsize index){
    if(index>=0 && index < length()){
        return JArrayAccessRef<ArrayType>(*this, index);
    }
    return JArrayAccessRef<ArrayType>(JObjectWrapper(), 0);
}

template<typename ArrayType>
void JArrayWrapper<ArrayType>::assign(JNIEnv* env, jobject object, bool globalRefs){
    m_data.reset(createJArrayWrapperData<ArrayType>(env, object, globalRefs));
}

template<typename ArrayType>
void JArrayWrapper<ArrayType>::assign(JNIEnv* env, ArrayType object, bool globalRefs){
    m_data.reset(createJArrayWrapperData<ArrayType>(env, object, globalRefs));
}

template<typename ArrayType>
void JArrayWrapper<ArrayType>::assign(const JObjectWrapper &wrapper){
    if(wrapper.m_data){
        switch(wrapper.m_data->type()){
        case JArrayRuntime<ArrayType>::ArrayRef:
            m_data = wrapper.m_data;
            break;
        case JObjectWrapperData::JObjectRef:
        case JObjectWrapperData::JValueRef:
            if(JniEnvironment env{500}){
                assign(env, wrapper.object(env), !wrapper.m_data->isWeak());
            }
            break;
        default:
            m_data.reset(createJArrayWrapperData<ArrayType>());
            break;
        }
    }else{
        m_data.reset(createJArrayWrapperData<ArrayType>());
    }
}

template<typename ArrayType>
void JArrayWrapper<ArrayType>::assign(JObjectWrapper &&wrapper){
    if(wrapper.m_data){
        switch(wrapper.m_data->type()){
        case JArrayRuntime<ArrayType>::ArrayRef:
            m_data = std::move(wrapper.m_data);
            wrapper.m_data.reset(createJArrayWrapperData<ArrayType>());
            break;
        case JObjectWrapperData::JObjectRef:
        case JObjectWrapperData::JValueRef:
            if(JniEnvironment env{500}){
                assign(env, wrapper.object(env), !wrapper.m_data->isWeak());
                wrapper.m_data.reset();
            }
            break;
        default:
            m_data.reset(createJArrayWrapperData<ArrayType>());
            break;
        }
    }else{
        m_data.reset(createJArrayWrapperData<ArrayType>());
    }
}

template<typename ArrayType>
void JArrayWrapper<ArrayType>::assign(const JArrayWrapper &wrapper){
    m_data = wrapper.m_data;
}

template<typename ArrayType>
void JArrayWrapper<ArrayType>::assign(JArrayWrapper &&wrapper){
    m_data = std::move(wrapper.m_data);
}

template<typename ArrayType>
const typename JArrayWrapper<ArrayType>::ElementType* JArrayWrapper<ArrayType>::array() const
{
    return reinterpret_cast<const typename JArrayWrapper<ArrayType>::ElementType*>(JObjectWrapper::array());
}

template<typename ArrayType>
typename JArrayWrapper<ArrayType>::ElementType* JArrayWrapper<ArrayType>::array()
{
    return reinterpret_cast<typename JArrayWrapper<ArrayType>::ElementType*>(JObjectWrapper::array());
}

template<typename ArrayType>
JArrayWrapper<ArrayType>::operator const typename JArrayWrapper<ArrayType>::ElementType*() const{return array();}

template<typename ArrayType>
JArrayWrapper<ArrayType>::operator typename JArrayWrapper<ArrayType>::ElementType*(){return array();}

template<typename ArrayType>
ArrayType JArrayWrapper<ArrayType>::object(JNIEnv *env) const { return ArrayType(JObjectWrapper::object(env)); }

template<typename ArrayType>
JArrayWrapper<ArrayType>::operator ArrayType() const{ return ArrayType(JObjectWrapper::operator jobject()); }

template<typename ArrayType>
JArrayWrapper<ArrayType>& JArrayWrapper<ArrayType>::operator=(const JArrayWrapper &wrapper){
    assign(wrapper);
    return *this;
}

template<typename ArrayType>
JArrayWrapper<ArrayType>& JArrayWrapper<ArrayType>::operator=(JArrayWrapper &&wrapper){
    assign(std::move(wrapper));
    return *this;
}

template<typename ArrayType>
JArrayWrapper<ArrayType>& JArrayWrapper<ArrayType>::operator=(const JObjectWrapper &wrapper) {
    assign(wrapper);
    return *this;
}

template<typename ArrayType>
JArrayWrapper<ArrayType>& JArrayWrapper<ArrayType>::operator=(JObjectWrapper &&wrapper) {
    assign(std::move(wrapper));
    return *this;
}

template<typename ArrayType>
JArrayWrapper<ArrayType>& JArrayWrapper<ArrayType>::operator=(jobject object) {
    if(JniEnvironment env{500}){
        assign(env, object);
    }
    return *this;
}

template<typename ArrayType>
JArrayWrapper<ArrayType>& JArrayWrapper<ArrayType>::operator=(ArrayType object) {
    if(JniEnvironment env{500}){
        assign(env, object);
    }
    return *this;
}

template class QTJAMBI_EXPORT JArrayWrapper<jintArray>;

template class QTJAMBI_EXPORT JArrayWrapper<jlongArray>;

template class QTJAMBI_EXPORT JArrayWrapper<jshortArray>;

template class QTJAMBI_EXPORT JArrayWrapper<jbyteArray>;

template class QTJAMBI_EXPORT JArrayWrapper<jbooleanArray>;

template class QTJAMBI_EXPORT JArrayWrapper<jcharArray>;

template class QTJAMBI_EXPORT JArrayWrapper<jfloatArray>;

template class QTJAMBI_EXPORT JArrayWrapper<jdoubleArray>;
