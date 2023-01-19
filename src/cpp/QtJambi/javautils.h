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

#if !defined(QTJAMBI_JAVAUTILS_H) && !defined(QTJAMBI_GENERATOR_RUNNING)
#define QTJAMBI_JAVAUTILS_H

namespace QtJambiPrivate{
QTJAMBI_EXPORT void javaInstanceCheck(JNIEnv* env,jobject object, jclass class_ref, bool isMemberFunction, const char* name);
QTJAMBI_EXPORT void javaExceptionCheck(JNIEnv* env);
}

#define REPOSITORY_EXPORT QTJAMBI_STATIC_EXPORT

#define QTJAMBI_REPOSITORY_DECLARE_CLASS(type_name, members) \
    class QTJAMBI_EXPORT type_name{\
    private:\
        jclass class_ref;\
        explicit type_name(JNIEnv * env);\
        static const type_name& __qt_get_this(JNIEnv *);\
    public:\
        static jclass getClass(JNIEnv* env);\
        static jboolean isInstanceOf(JNIEnv* env,jobject instance);\
        static jboolean isAssignableFrom(JNIEnv* env,jclass otherClass);\
        static jboolean isSameClass(JNIEnv* env,jclass otherClass);\
        static jobjectArray newArray(JNIEnv* env,jsize size);\
        members\
    };

#define QTJAMBI_REPOSITORY_DECLARE_EMPTY_CLASS(type_name) \
    QTJAMBI_REPOSITORY_DECLARE_CLASS(type_name,)

#define QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()\
    private: jmethodID __constructor;\
    public: static inline jobject newInstance(JNIEnv* env,...){\
        auto _this = __qt_get_this(env);\
        if(!_this.class_ref) return nullptr;\
        va_list args;\
        jobject result;\
        va_start(args, env);\
        result = env->NewObjectV(_this.class_ref,_this.__constructor,args);\
        va_end(args);\
        QtJambiPrivate::javaExceptionCheck(env);\
        return result;\
    }

#define QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR2()\
    private: jmethodID __constructor2;\
    public: static inline jobject newInstance2(JNIEnv* env,...){\
        auto _this = __qt_get_this(env);\
        if(!_this.class_ref) return nullptr;\
        va_list args;\
        jobject result;\
        va_start(args, env);\
        result = env->NewObjectV(_this.class_ref,_this.__constructor2,args);\
        va_end(args);\
        QtJambiPrivate::javaExceptionCheck(env);\
        return result;\
    }

#define QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR3()\
    private: jmethodID __constructor3;\
    public: static inline jobject newInstance3(JNIEnv* env,...){\
        auto _this = __qt_get_this(env);\
        if(!_this.class_ref) return nullptr;\
        va_list args;\
        jobject result;\
        va_start(args, env);\
        result = env->NewObjectV(_this.class_ref,_this.__constructor3,args);\
        va_end(args);\
        QtJambiPrivate::javaExceptionCheck(env);\
        return result;\
    }

#define QTJAMBI_REPOSITORY_DECLARE_METHOD_ID(method)\
    public: static inline jmethodID method##_ID(JNIEnv* env){\
        auto _this = __qt_get_this(env);\
        if(!_this.class_ref) return nullptr;\
        return _this.__##method;\
    }

#define QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(method)\
    private: jmethodID __##method;\
    public: static inline void method(JNIEnv* env,jobject object,...){\
        auto _this = __qt_get_this(env);\
        if(!_this.class_ref) return;\
        QtJambiPrivate::javaInstanceCheck(env, object, _this.class_ref, true, #method);\
        va_list args;\
        va_start(args, object);\
        env->CallVoidMethodV(object,_this.__##method,args);\
        va_end(args);\
        QtJambiPrivate::javaExceptionCheck(env);\
    }

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_VOID_METHOD(method)\
    private: jmethodID __##method;\
    public: static inline void method(JNIEnv* env,...){\
        auto _this = __qt_get_this(env);\
        if(!_this.class_ref) return;\
        va_list args;\
        va_start(args, env);\
        env->CallStaticVoidMethodV(_this.class_ref,_this.__##method,args);\
        va_end(args);\
        QtJambiPrivate::javaExceptionCheck(env);\
    }

#define QTJAMBI_REPOSITORY_DECLARE_PRIMITIVETYPE_METHOD(jtype,TYPE,method)\
    private: jmethodID __##method;\
    public: static inline jtype method(JNIEnv* env,jobject object,...){\
        auto _this = __qt_get_this(env);\
        if(!_this.class_ref) return jtype{};\
        QtJambiPrivate::javaInstanceCheck(env, object, _this.class_ref, true, #method);\
        va_list args;\
        va_start(args, object);\
        jtype result = jtype(env->Call##TYPE##MethodV(object,_this.__##method,args));\
        va_end(args);\
        QtJambiPrivate::javaExceptionCheck(env);\
        return result;\
    }

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_PRIMITIVETYPE_METHOD(jtype, TYPE, method)\
    private: jmethodID __##method;\
    public: static inline jtype method(JNIEnv* env,...){\
        auto _this = __qt_get_this(env);\
        if(!_this.class_ref) return jtype{};\
        va_list args;\
        va_start(args, env);\
        jtype result = jtype(env->CallStatic##TYPE##MethodV(_this.class_ref,_this.__##method,args));\
        va_end(args);\
        QtJambiPrivate::javaExceptionCheck(env);\
        return result;\
    }

#define QTJAMBI_REPOSITORY_DECLARE_PRIMITIVETYPE_FIELD(jtype,TYPE,field)\
    private: jfieldID __##field;\
    public: static inline jtype field(JNIEnv* env,jobject object){\
        auto _this = __qt_get_this(env);\
        if(!_this.class_ref) return jtype{};\
        QtJambiPrivate::javaInstanceCheck(env, object, _this.class_ref, false, #field);\
        jtype _result = jtype(env->Get##TYPE##Field(object, _this.__##field));\
        QtJambiPrivate::javaExceptionCheck(env);\
        return _result;\
    }\
    public: static inline void set_##field(JNIEnv* env,jobject object, jtype value){\
        auto _this = __qt_get_this(env);\
        if(!_this.class_ref) return;\
        QtJambiPrivate::javaInstanceCheck(env, object, _this.class_ref, false, #field);\
        env->Set##TYPE##Field(object, _this.__##field, value);\
        QtJambiPrivate::javaExceptionCheck(env);\
    }

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_PRIMITIVETYPE_FIELD(jtype,TYPE,field)\
    private: jfieldID __##field;\
    public: static inline jtype field(JNIEnv* env){\
        auto _this = __qt_get_this(env);\
        if(!_this.class_ref) return jtype{};\
        jtype result = jtype(env->GetStatic##TYPE##Field(_this.class_ref, _this.__##field));\
        QtJambiPrivate::javaExceptionCheck(env);\
        return result;\
    }\
    public: static inline void set_##field(JNIEnv* env, jtype value){\
        auto _this = __qt_get_this(env);\
        if(!_this.class_ref) return;\
        env->SetStatic##TYPE##Field(_this.class_ref, _this.__##field, value);\
        QtJambiPrivate::javaExceptionCheck(env);\
    }

#define QTJAMBI_REPOSITORY_DECLARE_TYPED_METHOD(jtype,method)\
    QTJAMBI_REPOSITORY_DECLARE_PRIMITIVETYPE_METHOD(jtype,Object,method)

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_TYPED_METHOD(jtype,method)\
    QTJAMBI_REPOSITORY_DECLARE_STATIC_PRIMITIVETYPE_METHOD(jtype,Object,method)

#define QTJAMBI_REPOSITORY_DECLARE_TYPED_FIELD(jtype,field)\
    QTJAMBI_REPOSITORY_DECLARE_PRIMITIVETYPE_FIELD(jtype,Object,field)

#define QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(method)\
    QTJAMBI_REPOSITORY_DECLARE_TYPED_METHOD(jobject,method)

#define QTJAMBI_REPOSITORY_DECLARE_CLASS_METHOD(method)\
    QTJAMBI_REPOSITORY_DECLARE_TYPED_METHOD(jclass,method)

#define QTJAMBI_REPOSITORY_DECLARE_OBJECTARRAY_METHOD(method)\
    QTJAMBI_REPOSITORY_DECLARE_TYPED_METHOD(jobjectArray,method)

#define QTJAMBI_REPOSITORY_DECLARE_INTARRAY_METHOD(method)\
    QTJAMBI_REPOSITORY_DECLARE_TYPED_METHOD(jintArray,method)

#define QTJAMBI_REPOSITORY_DECLARE_BYTEARRAY_METHOD(method)\
    QTJAMBI_REPOSITORY_DECLARE_TYPED_METHOD(jbyteArray,method)

#define QTJAMBI_REPOSITORY_DECLARE_LONGARRAY_METHOD(method)\
    QTJAMBI_REPOSITORY_DECLARE_TYPED_METHOD(jlongArray,method)

#define QTJAMBI_REPOSITORY_DECLARE_SHORTARRAY_METHOD(method)\
    QTJAMBI_REPOSITORY_DECLARE_TYPED_METHOD(jshortArray,method)

#define QTJAMBI_REPOSITORY_DECLARE_BOOLEANARRAY_METHOD(method)\
    QTJAMBI_REPOSITORY_DECLARE_TYPED_METHOD(jbooleanArray,method)

#define QTJAMBI_REPOSITORY_DECLARE_CHARARRAY_METHOD(method)\
    QTJAMBI_REPOSITORY_DECLARE_TYPED_METHOD(jcharArray,method)

#define QTJAMBI_REPOSITORY_DECLARE_FLOATARRAY_METHOD(method)\
    QTJAMBI_REPOSITORY_DECLARE_TYPED_METHOD(jfloatArray,method)

#define QTJAMBI_REPOSITORY_DECLARE_DOUBLEARRAY_METHOD(method)\
    QTJAMBI_REPOSITORY_DECLARE_TYPED_METHOD(jdoubleArray,method)

#define QTJAMBI_REPOSITORY_DECLARE_STRING_METHOD(method)\
    QTJAMBI_REPOSITORY_DECLARE_TYPED_METHOD(jstring,method)

#define QTJAMBI_REPOSITORY_DECLARE_THROWABLE_METHOD(method)\
    QTJAMBI_REPOSITORY_DECLARE_TYPED_METHOD(jthrowable,method)

#define QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(method)\
    QTJAMBI_REPOSITORY_DECLARE_PRIMITIVETYPE_METHOD(jint,Int,method)

#define QTJAMBI_REPOSITORY_DECLARE_LONG_METHOD(method)\
    QTJAMBI_REPOSITORY_DECLARE_PRIMITIVETYPE_METHOD(jlong,Long,method)

#define QTJAMBI_REPOSITORY_DECLARE_SHORT_METHOD(method)\
    QTJAMBI_REPOSITORY_DECLARE_PRIMITIVETYPE_METHOD(jshort,Short,method)

#define QTJAMBI_REPOSITORY_DECLARE_BYTE_METHOD(method)\
    QTJAMBI_REPOSITORY_DECLARE_PRIMITIVETYPE_METHOD(jbyte,Byte,method)

#define QTJAMBI_REPOSITORY_DECLARE_DOUBLE_METHOD(method)\
    QTJAMBI_REPOSITORY_DECLARE_PRIMITIVETYPE_METHOD(jdouble,Double,method)

#define QTJAMBI_REPOSITORY_DECLARE_FLOAT_METHOD(method)\
    QTJAMBI_REPOSITORY_DECLARE_PRIMITIVETYPE_METHOD(jfloat,Float,method)

#define QTJAMBI_REPOSITORY_DECLARE_CHAR_METHOD(method)\
    QTJAMBI_REPOSITORY_DECLARE_PRIMITIVETYPE_METHOD(jchar,Char,method)

#define QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(method)\
    QTJAMBI_REPOSITORY_DECLARE_PRIMITIVETYPE_METHOD(jboolean,Boolean,method)

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(method)\
    QTJAMBI_REPOSITORY_DECLARE_STATIC_TYPED_METHOD(jobject,method)

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECTARRAY_METHOD(method)\
    QTJAMBI_REPOSITORY_DECLARE_STATIC_TYPED_METHOD(jobjectArray,method)

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_INTARRAY_METHOD(method)\
    QTJAMBI_REPOSITORY_DECLARE_STATIC_TYPED_METHOD(jintArray,method)

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_LONGARRAY_METHOD(method)\
    QTJAMBI_REPOSITORY_DECLARE_STATIC_TYPED_METHOD(jlongArray,method)

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_SHORTARRAY_METHOD(method)\
    QTJAMBI_REPOSITORY_DECLARE_STATIC_TYPED_METHOD(jshortArray,method)

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_BYTEARRAY_METHOD(method)\
    QTJAMBI_REPOSITORY_DECLARE_STATIC_TYPED_METHOD(jbyteArray,method)

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_CHARARRAY_METHOD(method)\
    QTJAMBI_REPOSITORY_DECLARE_STATIC_TYPED_METHOD(jcharArray,method)

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_BOOLEANARRAY_METHOD(method)\
    QTJAMBI_REPOSITORY_DECLARE_STATIC_TYPED_METHOD(jbooleanArray,method)

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_FLOATARRAY_METHOD(method)\
    QTJAMBI_REPOSITORY_DECLARE_STATIC_TYPED_METHOD(jfloatArray,method)

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_DOUBLEARRAY_METHOD(method)\
    QTJAMBI_REPOSITORY_DECLARE_STATIC_TYPED_METHOD(jdoubleArray,method)

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_CLASS_METHOD(method)\
    QTJAMBI_REPOSITORY_DECLARE_STATIC_TYPED_METHOD(jclass,method)

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_STRING_METHOD(method)\
    QTJAMBI_REPOSITORY_DECLARE_STATIC_TYPED_METHOD(jstring,method)

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_THROWABLE_METHOD(method)\
    QTJAMBI_REPOSITORY_DECLARE_STATIC_TYPED_METHOD(jthrowable,method)

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_INT_METHOD(method)\
    QTJAMBI_REPOSITORY_DECLARE_STATIC_PRIMITIVETYPE_METHOD(jint,Int,method)

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_LONG_METHOD(method)\
    QTJAMBI_REPOSITORY_DECLARE_STATIC_PRIMITIVETYPE_METHOD(jlong,Long,method)

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_SHORT_METHOD(method)\
    QTJAMBI_REPOSITORY_DECLARE_STATIC_PRIMITIVETYPE_METHOD(jshort,Short,method)

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_BYTE_METHOD(method)\
    QTJAMBI_REPOSITORY_DECLARE_STATIC_PRIMITIVETYPE_METHOD(jbyte,Byte,method)

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_DOUBLE_METHOD(method)\
    QTJAMBI_REPOSITORY_DECLARE_STATIC_PRIMITIVETYPE_METHOD(jdouble,Double,method)

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_FLOAT_METHOD(method)\
    QTJAMBI_REPOSITORY_DECLARE_STATIC_PRIMITIVETYPE_METHOD(jfloat,Float,method)

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_CHAR_METHOD(method)\
    QTJAMBI_REPOSITORY_DECLARE_STATIC_PRIMITIVETYPE_METHOD(jchar,Char,method)

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_BOOLEAN_METHOD(method)\
    QTJAMBI_REPOSITORY_DECLARE_STATIC_PRIMITIVETYPE_METHOD(jboolean,Boolean,method)

#define QTJAMBI_REPOSITORY_DECLARE_OBJECT_FIELD(field)\
    QTJAMBI_REPOSITORY_DECLARE_TYPED_FIELD(jobject,field)

#define QTJAMBI_REPOSITORY_DECLARE_CLASS_FIELD(field)\
    QTJAMBI_REPOSITORY_DECLARE_TYPED_FIELD(jclass,field)

#define QTJAMBI_REPOSITORY_DECLARE_STRING_FIELD(field)\
    QTJAMBI_REPOSITORY_DECLARE_TYPED_FIELD(jstring,field)

#define QTJAMBI_REPOSITORY_DECLARE_THROWABLE_FIELD(field)\
    QTJAMBI_REPOSITORY_DECLARE_TYPED_FIELD(jthrowabl,field)

#define QTJAMBI_REPOSITORY_DECLARE_INTARRAY_FIELD(field)\
    QTJAMBI_REPOSITORY_DECLARE_TYPED_FIELD(jintArray,field)

#define QTJAMBI_REPOSITORY_DECLARE_BYTEARRAY_FIELD(field)\
    QTJAMBI_REPOSITORY_DECLARE_TYPED_FIELD(jbyteArray,field)

#define QTJAMBI_REPOSITORY_DECLARE_SHORTARRAY_FIELD(field)\
    QTJAMBI_REPOSITORY_DECLARE_TYPED_FIELD(jshortArray,field)

#define QTJAMBI_REPOSITORY_DECLARE_CHARARRAY_FIELD(field)\
    QTJAMBI_REPOSITORY_DECLARE_TYPED_FIELD(jcharArray,field)

#define QTJAMBI_REPOSITORY_DECLARE_BOOLEANARRAY_FIELD(field)\
    QTJAMBI_REPOSITORY_DECLARE_TYPED_FIELD(jbooleanArray,field)

#define QTJAMBI_REPOSITORY_DECLARE_FLOATARRAY_FIELD(field)\
    QTJAMBI_REPOSITORY_DECLARE_TYPED_FIELD(jfloatArray,field)

#define QTJAMBI_REPOSITORY_DECLARE_DOUBLEARRAY_FIELD(field)\
    QTJAMBI_REPOSITORY_DECLARE_TYPED_FIELD(jdoubleArray,field)

#define QTJAMBI_REPOSITORY_DECLARE_OBJECTARRAY_FIELD(field)\
    QTJAMBI_REPOSITORY_DECLARE_TYPED_FIELD(jobjectArray,field)

#define QTJAMBI_REPOSITORY_DECLARE_BYTE_FIELD(field)\
    QTJAMBI_REPOSITORY_DECLARE_PRIMITIVETYPE_FIELD(jbyte,Byte,field)

#define QTJAMBI_REPOSITORY_DECLARE_SHORT_FIELD(field)\
    QTJAMBI_REPOSITORY_DECLARE_PRIMITIVETYPE_FIELD(jshort,Short,field)

#define QTJAMBI_REPOSITORY_DECLARE_INT_FIELD(field)\
    QTJAMBI_REPOSITORY_DECLARE_PRIMITIVETYPE_FIELD(jint,Int,field)

#define QTJAMBI_REPOSITORY_DECLARE_LONG_FIELD(field)\
    QTJAMBI_REPOSITORY_DECLARE_PRIMITIVETYPE_FIELD(jlong,Long,field)

#define QTJAMBI_REPOSITORY_DECLARE_FLOAT_FIELD(field)\
    QTJAMBI_REPOSITORY_DECLARE_PRIMITIVETYPE_FIELD(jfloat,Float,field)

#define QTJAMBI_REPOSITORY_DECLARE_DOUBLE_FIELD(field)\
    QTJAMBI_REPOSITORY_DECLARE_PRIMITIVETYPE_FIELD(jdouble,Double,field)

#define QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_FIELD(field)\
    QTJAMBI_REPOSITORY_DECLARE_PRIMITIVETYPE_FIELD(jboolean,Boolean,field)

#define QTJAMBI_REPOSITORY_DECLARE_CHAR_FIELD(field)\
    QTJAMBI_REPOSITORY_DECLARE_PRIMITIVETYPE_FIELD(jchar,Char,field)

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_TYPED_FIELD(jtype,field)\
    QTJAMBI_REPOSITORY_DECLARE_STATIC_PRIMITIVETYPE_FIELD(jtype,Object,field)

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_FIELD(field)\
    QTJAMBI_REPOSITORY_DECLARE_STATIC_TYPED_FIELD(jobject,field)

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_STRING_FIELD(field)\
    QTJAMBI_REPOSITORY_DECLARE_STATIC_TYPED_FIELD(jstring,field)

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_CLASS_FIELD(field)\
    QTJAMBI_REPOSITORY_DECLARE_STATIC_TYPED_FIELD(jclass,field)

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_THROWABLE_FIELD(field)\
    QTJAMBI_REPOSITORY_DECLARE_STATIC_TYPED_FIELD(jthrowable,field)

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECTARRAY_FIELD(field)\
    QTJAMBI_REPOSITORY_DECLARE_STATIC_TYPED_FIELD(jobjectArray,field)

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_BOOLEANARRAY_FIELD(field)\
    QTJAMBI_REPOSITORY_DECLARE_STATIC_TYPED_FIELD(jbooleanArray,field)

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_CHARARRAY_FIELD(field)\
    QTJAMBI_REPOSITORY_DECLARE_STATIC_TYPED_FIELD(jcharArray,field)

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_BYTEARRAY_FIELD(field)\
    QTJAMBI_REPOSITORY_DECLARE_STATIC_TYPED_FIELD(jbyteArray,field)

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_SHORTARRAY_FIELD(field)\
    QTJAMBI_REPOSITORY_DECLARE_STATIC_TYPED_FIELD(jshortArray,field)

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_INTARRAY_FIELD(field)\
    QTJAMBI_REPOSITORY_DECLARE_STATIC_TYPED_FIELD(jintArray,field)

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_LONGARRAY_FIELD(field)\
    QTJAMBI_REPOSITORY_DECLARE_STATIC_TYPED_FIELD(jlongArray,field)

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_FLOATARRAY_FIELD(field)\
    QTJAMBI_REPOSITORY_DECLARE_STATIC_TYPED_FIELD(jfloatArray,field)

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_DOUBLEARRAY_FIELD(field)\
    QTJAMBI_REPOSITORY_DECLARE_STATIC_TYPED_FIELD(jdoubleArray,field)

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_BYTE_FIELD(field)\
    QTJAMBI_REPOSITORY_DECLARE_STATIC_PRIMITIVETYPE_FIELD(jbyte,Byte,field)

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_SHORT_FIELD(field)\
    QTJAMBI_REPOSITORY_DECLARE_STATIC_PRIMITIVETYPE_FIELD(jshort,Short,field)

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_INT_FIELD(field)\
    QTJAMBI_REPOSITORY_DECLARE_STATIC_PRIMITIVETYPE_FIELD(jint,Int,field)

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_LONG_FIELD(field)\
    QTJAMBI_REPOSITORY_DECLARE_STATIC_PRIMITIVETYPE_FIELD(jlong,Long,field)

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_FLOAT_FIELD(field)\
    QTJAMBI_REPOSITORY_DECLARE_STATIC_PRIMITIVETYPE_FIELD(jfloat,Float,field)

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_DOUBLE_FIELD(field)\
    QTJAMBI_REPOSITORY_DECLARE_STATIC_PRIMITIVETYPE_FIELD(jdouble,Double,field)

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_BOOLEAN_FIELD(field)\
    QTJAMBI_REPOSITORY_DECLARE_STATIC_PRIMITIVETYPE_FIELD(jboolean,Boolean,field)

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_CHAR_FIELD(field)\
    QTJAMBI_REPOSITORY_DECLARE_STATIC_PRIMITIVETYPE_FIELD(jchar,Char,field)

#define DECLARE_CLASS_REF(cls)\
    private: jclass __##cls;\
    public: static inline bool isPrimitiveType(JNIEnv* env, jclass type){\
        auto _this = __qt_get_this(env);\
        if(!_this.class_ref) return false;\
        return env->IsSameObject(type, _this.__##cls);\
    }\
    static inline jclass primitiveType(JNIEnv* env){\
        auto _this = __qt_get_this(env);\
        return jclass(env->NewLocalRef(_this.__##cls));\
    }

#define POST_CLASS_REF_ACTION(class_ref)

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#define QRecursiveMutexLocker QMutexLocker
#else
#define QRecursiveMutexLocker QMutexLocker<QRecursiveMutex>
#endif

#define QTJAMBI_REPOSITORY_DEFINE_CLASS(package, type_name, content)\
const type_name& type_name::__qt_get_this(JNIEnv *env){\
    static const type_name _this = type_name(env);\
    return _this;\
}\
type_name::type_name(JNIEnv *env) : class_ref(nullptr) { \
    this->class_ref = JavaAPI::resolveClass(env, #package"/"#type_name);\
    Q_ASSERT(this->class_ref);\
    POST_CLASS_REF_ACTION(this->class_ref)\
    content\
    Q_ASSERT(this->class_ref);\
}\
jclass type_name::getClass(JNIEnv* env){\
    auto _this = __qt_get_this(env);\
    return jclass(env->NewLocalRef(_this.class_ref));\
}\
jboolean type_name::isInstanceOf(JNIEnv* env,jobject instance){\
    auto _this = __qt_get_this(env);\
    Q_ASSERT(_this.class_ref);\
    jboolean result = !env->IsSameObject(instance, nullptr) && env->IsInstanceOf(instance, _this.class_ref);\
    QtJambiPrivate::javaExceptionCheck(env);\
    return result;\
}\
jboolean type_name::isAssignableFrom(JNIEnv* env,jclass otherClass){\
    auto _this = __qt_get_this(env);\
    Q_ASSERT(_this.class_ref);\
    jboolean result = env->IsAssignableFrom(otherClass, _this.class_ref);\
    QtJambiPrivate::javaExceptionCheck(env);\
    return result;\
}\
jboolean type_name::isSameClass(JNIEnv* env,jclass otherClass){\
    auto _this = __qt_get_this(env);\
    Q_ASSERT(_this.class_ref);\
    jboolean result = env->IsSameObject(otherClass, _this.class_ref);\
    QtJambiPrivate::javaExceptionCheck(env);\
    return result;\
}\
jobjectArray type_name::newArray(JNIEnv* env,jsize size){\
    auto _this = __qt_get_this(env);\
    Q_ASSERT(_this.class_ref);\
    jobjectArray result = env->NewObjectArray(size, _this.class_ref, nullptr);\
    QtJambiPrivate::javaExceptionCheck(env);\
    return result;\
}

#define QTJAMBI_REPOSITORY_DEFINE_FIELD(field_name, signature)\
    this->__##field_name = env->GetFieldID(this->class_ref, #field_name, #signature);\
    QtJambiPrivate::javaExceptionCheck(env);\
    Q_ASSERT(this->__##field_name);

#define QTJAMBI_REPOSITORY_DEFINE_LONG_FIELD(field_name)\
    this->__##field_name = env->GetFieldID(this->class_ref, #field_name, "J");\
    QtJambiPrivate::javaExceptionCheck(env);\
    Q_ASSERT(this->__##field_name);

#define QTJAMBI_REPOSITORY_DEFINE_STATIC_FIELD(field_name, signature)\
    this->__##field_name = env->GetStaticFieldID(this->class_ref, #field_name, #signature);\
    QtJambiPrivate::javaExceptionCheck(env);\
    Q_ASSERT(this->__##field_name);

#define QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(signature)\
    this->__constructor = env->GetMethodID(this->class_ref, "<init>", "("#signature")V");\
    QtJambiPrivate::javaExceptionCheck(env);\
    Q_ASSERT(this->__constructor);

#define QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR2(signature)\
    this->__constructor2 = env->GetMethodID(this->class_ref, "<init>", "("#signature")V");\
    QtJambiPrivate::javaExceptionCheck(env);\
    Q_ASSERT(this->__constructor2);

#define QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR3(signature)\
    this->__constructor3 = env->GetMethodID(this->class_ref, "<init>", "("#signature")V");\
    QtJambiPrivate::javaExceptionCheck(env);\
    Q_ASSERT(this->__constructor3);

#define QTJAMBI_REPOSITORY_DEFINE_RENAMED_METHOD(target, method_name, signature)\
    this->__##target = env->GetMethodID(this->class_ref, #method_name, #signature);\
    QtJambiPrivate::javaExceptionCheck(env);\
    Q_ASSERT(this->__##target);

#define QTJAMBI_REPOSITORY_DEFINE_METHOD(method_name, signature)\
    this->__##method_name = env->GetMethodID(this->class_ref, #method_name, #signature);\
    QtJambiPrivate::javaExceptionCheck(env);\
    Q_ASSERT(this->__##method_name);

#define QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(method_name, signature)\
    this->__##method_name = env->GetStaticMethodID(this->class_ref, #method_name, #signature);\
    QtJambiPrivate::javaExceptionCheck(env);\
    Q_ASSERT(this->__##method_name);

#define QTJAMBI_REPOSITORY_DEFINE_RENAMED_STATIC_METHOD(target, method_name, signature)\
    this->__##target = env->GetStaticMethodID(this->class_ref, #method_name, #signature);\
    QtJambiPrivate::javaExceptionCheck(env);\
    Q_ASSERT(this->__##target);

#define QTJAMBI_REPOSITORY_DEFINE_STANDARD_CONSTRUCTOR()\
    this->__constructor = env->GetMethodID(this->class_ref, "<init>", "()V");\
    QtJambiPrivate::javaExceptionCheck(env);\
    Q_ASSERT(this->__constructor);

#define QTJAMBI_REPOSITORY_DEFINE_EMPTY_CLASS(package, type_name)\
    QTJAMBI_REPOSITORY_DEFINE_CLASS(package, type_name,)

#define QTJAMBI_REPOSITORY_DEFINE_CLASS_SC(package, type_name)\
    QTJAMBI_REPOSITORY_DEFINE_CLASS(package, type_name, QTJAMBI_REPOSITORY_DEFINE_STANDARD_CONSTRUCTOR())

#endif // QTJAMBI_JAVAUTILS_H
