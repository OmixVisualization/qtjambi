/****************************************************************************
**
** Copyright (C) 2009-2020 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
**
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
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

#ifndef QTJAMBI_REPODEFINES_H
#define QTJAMBI_REPODEFINES_H

#define REPOSITORY_EXPORT QTJAMBI_STATIC_EXPORT

#define QTJAMBI_REPOSITORY_DECLARE_CLASS(container,type_name, members) \
public:\
    class QTJAMBI_EXPORT _##type_name{\
    private:\
        jclass class_ref;\
        void (*__qt_resolve)(JNIEnv *, const _##type_name*);\
        _##type_name();\
        friend container;\
    public:\
        jclass getClass(JNIEnv* env) const;\
        jboolean isInstanceOf(JNIEnv* env,jobject instance) const;\
        jboolean isAssignableFrom(JNIEnv* env,jclass otherClass) const;\
        jboolean isSameClass(JNIEnv* env,jclass otherClass) const;\
        jobjectArray newArray(JNIEnv* env,jsize size) const;\
        members\
    };\
    static REPOSITORY_EXPORT const _##type_name type_name;

#define QTJAMBI_REPOSITORY_DECLARE_EMPTY_CLASS(ns,type_name) \
    QTJAMBI_REPOSITORY_DECLARE_CLASS(ns,type_name,)

#define QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()\
    private: jmethodID __constructor;\
    public: inline jobject newInstance(JNIEnv* env,...) const {\
        __qt_resolve(env, this);\
        va_list args;\
        jobject result;\
        va_start(args, env);\
        result = env->NewObjectV(class_ref,__constructor,args);\
        va_end(args);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return result;\
    }

#define QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR2()\
    private: jmethodID __constructor2;\
    public: inline jobject newInstance2(JNIEnv* env,...) const {\
        __qt_resolve(env, this);\
        va_list args;\
        jobject result;\
        va_start(args, env);\
        result = env->NewObjectV(class_ref,__constructor2,args);\
        va_end(args);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return result;\
    }

#define QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR3()\
    private: jmethodID __constructor3;\
    public: inline jobject newInstance3(JNIEnv* env,...) const {\
        __qt_resolve(env, this);\
        va_list args;\
        jobject result;\
        va_start(args, env);\
        result = env->NewObjectV(class_ref,__constructor3,args);\
        va_end(args);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return result;\
    }

#ifdef QT_QTJAMBI_PORT
#define QTJAMBI_REPOSITORY_DECLARE_VOID_AMETHOD(method)\
    private: jmethodID __##method;\
    public: inline void method(JNIEnv* env,jobject object,jvalue* arguments) const {\
        __qt_resolve(env, this);\
        env->CallVoidMethodA(object,__##method,arguments);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
    }
#endif

#define QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(method)\
    private: jmethodID __##method;\
    public: inline void method(JNIEnv* env,jobject object,...) const {\
        __qt_resolve(env, this);\
        if(env->IsSameObject(object, nullptr))\
            JavaException::raiseNullPointerException(env, nullptr QTJAMBI_STACKTRACEINFO );\
        if(!isInstanceOf(env, object))\
            JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Cannot invoke member function %1." #method "(...) on object of type %2.").arg(qtjambi_class_name(env, class_ref)).arg(qtjambi_object_class_name(env, object))) QTJAMBI_STACKTRACEINFO );\
        va_list args;\
        va_start(args, object);\
        env->CallVoidMethodV(object,__##method,args);\
        va_end(args);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
    }

#define QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(method)\
    private: jmethodID __##method;\
    public: inline jobject method(JNIEnv* env,jobject object,...) const {\
        __qt_resolve(env, this);\
        va_list args;\
        if(env->IsSameObject(object, nullptr))\
            JavaException::raiseNullPointerException(env, nullptr QTJAMBI_STACKTRACEINFO );\
        if(!isInstanceOf(env, object))\
            JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Cannot invoke member function %1." #method "(...) on object of type %2.").arg(qtjambi_class_name(env, class_ref)).arg(qtjambi_object_class_name(env, object))) QTJAMBI_STACKTRACEINFO );\
        jobject result;\
        va_start(args, object);\
        result = env->CallObjectMethodV(object,__##method,args);\
        va_end(args);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return result;\
    }

#define QTJAMBI_REPOSITORY_DECLARE_CLASS_METHOD(method)\
    private: jmethodID __##method;\
    public: inline jclass method(JNIEnv* env,jobject object,...) const {\
        __qt_resolve(env, this);\
        if(env->IsSameObject(object, nullptr))\
            JavaException::raiseNullPointerException(env, nullptr QTJAMBI_STACKTRACEINFO );\
        if(!isInstanceOf(env, object))\
            JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Cannot invoke member function %1." #method "(...) on object of type %2.").arg(qtjambi_class_name(env, class_ref)).arg(qtjambi_object_class_name(env, object))) QTJAMBI_STACKTRACEINFO );\
        va_list args;\
        jobject result;\
        va_start(args, object);\
        result = env->CallObjectMethodV(object,__##method,args);\
        va_end(args);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return jclass(result);\
    }

#define QTJAMBI_REPOSITORY_DECLARE_OBJECTARRAY_METHOD(method)\
    private: jmethodID __##method;\
    public: inline jobjectArray method(JNIEnv* env,jobject object,...) const {\
        __qt_resolve(env, this);\
        if(env->IsSameObject(object, nullptr))\
            JavaException::raiseNullPointerException(env, nullptr QTJAMBI_STACKTRACEINFO );\
        if(!isInstanceOf(env, object))\
            JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Cannot invoke member function %1." #method "(...) on object of type %2.").arg(qtjambi_class_name(env, class_ref)).arg(qtjambi_object_class_name(env, object))) QTJAMBI_STACKTRACEINFO );\
        va_list args;\
        jobject result;\
        va_start(args, object);\
        result = env->CallObjectMethodV(object,__##method,args);\
        va_end(args);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return jobjectArray(result);\
    }

#define QTJAMBI_REPOSITORY_DECLARE_STRING_METHOD(method)\
    private: jmethodID __##method;\
    public: inline jstring method(JNIEnv* env,jobject object,...) const {\
        __qt_resolve(env, this);\
        if(env->IsSameObject(object, nullptr))\
            JavaException::raiseNullPointerException(env, nullptr QTJAMBI_STACKTRACEINFO );\
        if(!isInstanceOf(env, object))\
            JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Cannot invoke member function %1." #method "(...) on object of type %2.").arg(qtjambi_class_name(env, class_ref)).arg(qtjambi_object_class_name(env, object))) QTJAMBI_STACKTRACEINFO );\
        va_list args;\
        jobject result;\
        va_start(args, object);\
        result = env->CallObjectMethodV(object,__##method,args);\
        va_end(args);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return jstring(result);\
    }

#define QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(method)\
    private: jmethodID __##method;\
    public: inline jint method(JNIEnv* env,jobject object,...) const {\
        __qt_resolve(env, this);\
        if(env->IsSameObject(object, nullptr))\
            JavaException::raiseNullPointerException(env, nullptr QTJAMBI_STACKTRACEINFO );\
        if(!isInstanceOf(env, object))\
            JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Cannot invoke member function %1." #method "(...) on object of type %2.").arg(qtjambi_class_name(env, class_ref)).arg(qtjambi_object_class_name(env, object))) QTJAMBI_STACKTRACEINFO );\
        va_list args;\
        jint result;\
        va_start(args, object);\
        result = env->CallIntMethodV(object,__##method,args);\
        va_end(args);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return result;\
    }

#define QTJAMBI_REPOSITORY_DECLARE_LONG_METHOD(method)\
    private: jmethodID __##method;\
    public: inline jlong method(JNIEnv* env,jobject object,...) const {\
        __qt_resolve(env, this);\
        if(env->IsSameObject(object, nullptr))\
            JavaException::raiseNullPointerException(env, nullptr QTJAMBI_STACKTRACEINFO );\
        if(!isInstanceOf(env, object))\
            JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Cannot invoke member function %1." #method "(...) on object of type %2.").arg(qtjambi_class_name(env, class_ref)).arg(qtjambi_object_class_name(env, object))) QTJAMBI_STACKTRACEINFO );\
        va_list args;\
        jlong result;\
        va_start(args, object);\
        result = env->CallLongMethodV(object,__##method,args);\
        va_end(args);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return result;\
    }

#define QTJAMBI_REPOSITORY_DECLARE_SHORT_METHOD(method)\
    private: jmethodID __##method;\
    public: inline jshort method(JNIEnv* env,jobject object,...) const {\
        __qt_resolve(env, this);\
        if(env->IsSameObject(object, nullptr))\
            JavaException::raiseNullPointerException(env, nullptr QTJAMBI_STACKTRACEINFO );\
        if(!isInstanceOf(env, object))\
            JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Cannot invoke member function %1." #method "(...) on object of type %2.").arg(qtjambi_class_name(env, class_ref)).arg(qtjambi_object_class_name(env, object))) QTJAMBI_STACKTRACEINFO );\
        va_list args;\
        jshort result;\
        va_start(args, object);\
        result = env->CallShortMethodV(object,__##method,args);\
        va_end(args);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return result;\
    }

#define QTJAMBI_REPOSITORY_DECLARE_BYTE_METHOD(method)\
    private: jmethodID __##method;\
    public: inline jbyte method(JNIEnv* env,jobject object,...) const {\
        __qt_resolve(env, this);\
        if(env->IsSameObject(object, nullptr))\
            JavaException::raiseNullPointerException(env, nullptr QTJAMBI_STACKTRACEINFO );\
        if(!isInstanceOf(env, object))\
            JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Cannot invoke member function %1." #method "(...) on object of type %2.").arg(qtjambi_class_name(env, class_ref)).arg(qtjambi_object_class_name(env, object))) QTJAMBI_STACKTRACEINFO );\
        va_list args;\
        jbyte result;\
        va_start(args, object);\
        result = env->CallByteMethodV(object,__##method,args);\
        va_end(args);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return result;\
    }

#define QTJAMBI_REPOSITORY_DECLARE_DOUBLE_METHOD(method)\
    private: jmethodID __##method;\
    public: inline jdouble method(JNIEnv* env,jobject object,...) const {\
        __qt_resolve(env, this);\
        if(env->IsSameObject(object, nullptr))\
            JavaException::raiseNullPointerException(env, nullptr QTJAMBI_STACKTRACEINFO );\
        if(!isInstanceOf(env, object))\
            JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Cannot invoke member function %1." #method "(...) on object of type %2.").arg(qtjambi_class_name(env, class_ref)).arg(qtjambi_object_class_name(env, object))) QTJAMBI_STACKTRACEINFO );\
        va_list args;\
        jdouble result;\
        va_start(args, object);\
        result = env->CallDoubleMethodV(object,__##method,args);\
        va_end(args);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return result;\
    }

#define QTJAMBI_REPOSITORY_DECLARE_FLOAT_METHOD(method)\
    private: jmethodID __##method;\
    public: inline jfloat method(JNIEnv* env,jobject object,...) const {\
        __qt_resolve(env, this);\
        if(env->IsSameObject(object, nullptr))\
            JavaException::raiseNullPointerException(env, nullptr QTJAMBI_STACKTRACEINFO );\
        if(!isInstanceOf(env, object))\
            JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Cannot invoke member function %1." #method "(...) on object of type %2.").arg(qtjambi_class_name(env, class_ref)).arg(qtjambi_object_class_name(env, object))) QTJAMBI_STACKTRACEINFO );\
        va_list args;\
        jfloat result;\
        va_start(args, object);\
        result = env->CallFloatMethodV(object,__##method,args);\
        va_end(args);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return result;\
    }

#define QTJAMBI_REPOSITORY_DECLARE_CHAR_METHOD(method)\
    private: jmethodID __##method;\
    public: inline jchar method(JNIEnv* env,jobject object,...) const {\
        __qt_resolve(env, this);\
        if(env->IsSameObject(object, nullptr))\
            JavaException::raiseNullPointerException(env, nullptr QTJAMBI_STACKTRACEINFO );\
        if(!isInstanceOf(env, object))\
            JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Cannot invoke member function %1." #method "(...) on object of type %2.").arg(qtjambi_class_name(env, class_ref)).arg(qtjambi_object_class_name(env, object))) QTJAMBI_STACKTRACEINFO );\
        va_list args;\
        jchar result;\
        va_start(args, object);\
        result = env->CallCharMethodV(object,__##method,args);\
        va_end(args);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return result;\
    }

#define QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(method)\
    private: jmethodID __##method;\
    public: inline jboolean method(JNIEnv* env,jobject object,...) const {\
        __qt_resolve(env, this);\
        if(env->IsSameObject(object, nullptr))\
            JavaException::raiseNullPointerException(env, nullptr QTJAMBI_STACKTRACEINFO );\
        if(!isInstanceOf(env, object))\
            JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Cannot invoke member function %1." #method "(...) on object of type %2.").arg(qtjambi_class_name(env, class_ref)).arg(qtjambi_object_class_name(env, object))) QTJAMBI_STACKTRACEINFO );\
        va_list args;\
        jboolean result;\
        va_start(args, object);\
        result = env->CallBooleanMethodV(object,__##method,args);\
        va_end(args);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return result;\
    }

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_VOID_METHOD(method)\
    private: jmethodID __##method;\
    public: inline void method(JNIEnv* env,...) const {\
        __qt_resolve(env, this);\
        va_list args;\
        va_start(args, env);\
        env->CallStaticVoidMethodV(class_ref,__##method,args);\
        va_end(args);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
    }

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(method)\
    private: jmethodID __##method;\
    public: inline jobject method(JNIEnv* env,...) const {\
        __qt_resolve(env, this);\
        va_list args;\
        jobject result;\
        va_start(args, env);\
        result = env->CallStaticObjectMethodV(class_ref,__##method,args);\
        va_end(args);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return result;\
    }

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECTARRAY_METHOD(method)\
    private: jmethodID __##method;\
    public: inline jobjectArray method(JNIEnv* env,...) const {\
        __qt_resolve(env, this);\
        va_list args;\
        jobject result;\
        va_start(args, env);\
        result = env->CallStaticObjectMethodV(class_ref,__##method,args);\
        va_end(args);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return jobjectArray(result);\
    }

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_CLASS_METHOD(method)\
    private: jmethodID __##method;\
    public: inline jclass method(JNIEnv* env,...) const {\
        __qt_resolve(env, this);\
        va_list args;\
        jobject result;\
        va_start(args, env);\
        result = env->CallStaticObjectMethodV(class_ref,__##method,args);\
        va_end(args);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return jclass(result);\
    }

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_STRING_METHOD(method)\
    private: jmethodID __##method;\
    public: inline jstring method(JNIEnv* env,...) const {\
        __qt_resolve(env, this);\
        va_list args;\
        jobject result;\
        va_start(args, env);\
        result = env->CallStaticObjectMethodV(class_ref,__##method,args);\
        va_end(args);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return jstring(result);\
    }

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_INT_METHOD(method)\
    private: jmethodID __##method;\
    public: inline jint method(JNIEnv* env,...) const {\
        __qt_resolve(env, this);\
        va_list args;\
        jint result;\
        va_start(args, env);\
        result = env->CallStaticIntMethodV(class_ref,__##method,args);\
        va_end(args);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return result;\
    }

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_LONG_METHOD(method)\
    private: jmethodID __##method;\
    public: inline jlong method(JNIEnv* env,...) const {\
        __qt_resolve(env, this);\
        va_list args;\
        jlong result;\
        va_start(args, env);\
        result = env->CallStaticLongMethodV(class_ref,__##method,args);\
        va_end(args);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return result;\
    }

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_SHORT_METHOD(method)\
    private: jmethodID __##method;\
    public: inline jshort method(JNIEnv* env,...) const {\
        __qt_resolve(env, this);\
        va_list args;\
        jshort result;\
        va_start(args, env);\
        result = env->CallStaticShortMethodV(class_ref,__##method,args);\
        va_end(args);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return result;\
    }

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_BYTE_METHOD(method)\
    private: jmethodID __##method;\
    public: inline jbyte method(JNIEnv* env,...) const {\
        __qt_resolve(env, this);\
        va_list args;\
        jbyte result;\
        va_start(args, env);\
        result = env->CallStaticByteMethodV(class_ref,__##method,args);\
        va_end(args);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return result;\
    }

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_DOUBLE_METHOD(method)\
    private: jmethodID __##method;\
    public: inline jdouble method(JNIEnv* env,...) const {\
        __qt_resolve(env, this);\
        va_list args;\
        jdouble result;\
        va_start(args, env);\
        result = env->CallStaticDoubleMethodV(class_ref,__##method,args);\
        va_end(args);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return result;\
    }

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_FLOAT_METHOD(method)\
    private: jmethodID __##method;\
    public: inline jfloat method(JNIEnv* env,...) const {\
        __qt_resolve(env, this);\
        va_list args;\
        jfloat result;\
        va_start(args, env);\
        result = env->CallStaticFloatMethodV(class_ref,__##method,args);\
        va_end(args);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return result;\
    }

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_CHAR_METHOD(method)\
    private: jmethodID __##method;\
    public: inline jchar method(JNIEnv* env,...) const {\
        __qt_resolve(env, this);\
        va_list args;\
        jchar result;\
        va_start(args, env);\
        result = env->CallStaticCharMethodV(class_ref,__##method,args);\
        va_end(args);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return result;\
    }

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_BOOLEAN_METHOD(method)\
    private: jmethodID __##method;\
    public: inline jboolean method(JNIEnv* env,...) const {\
        __qt_resolve(env, this);\
        va_list args;\
        jboolean result;\
        va_start(args, env);\
        result = env->CallStaticBooleanMethodV(class_ref,__##method,args);\
        va_end(args);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return result;\
    }

#define QTJAMBI_REPOSITORY_DECLARE_OBJECT_FIELD(field)\
    private: jfieldID __##field;\
    public: inline jobject field(JNIEnv* env,jobject object) const {\
        __qt_resolve(env, this);\
        if(env->IsSameObject(object, nullptr))\
            JavaException::raiseNullPointerException(env, nullptr QTJAMBI_STACKTRACEINFO );\
        if(!isInstanceOf(env, object))\
            JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Cannot access member field %1." #field " on object of type %2.").arg(qtjambi_class_name(env, class_ref)).arg(qtjambi_object_class_name(env, object))) QTJAMBI_STACKTRACEINFO );\
        jobject _result = env->GetObjectField(object, __##field);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return _result;\
    }\
    public: inline void set_##field(JNIEnv* env,jobject object, jobject _value) const {\
        __qt_resolve(env, this);\
        if(env->IsSameObject(object, nullptr))\
            JavaException::raiseNullPointerException(env, nullptr QTJAMBI_STACKTRACEINFO );\
        if(!isInstanceOf(env, object))\
            JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Cannot access member field %1." #field " on object of type %2.").arg(qtjambi_class_name(env, class_ref)).arg(qtjambi_object_class_name(env, object))) QTJAMBI_STACKTRACEINFO );\
        env->SetObjectField(object, __##field, _value);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
    }

#define QTJAMBI_REPOSITORY_DECLARE_CLASS_FIELD(field)\
    private: jfieldID __##field;\
    public: inline jclass field(JNIEnv* env,jobject object) const {\
        __qt_resolve(env, this);\
        if(env->IsSameObject(object, nullptr))\
            JavaException::raiseNullPointerException(env, nullptr QTJAMBI_STACKTRACEINFO );\
        if(!isInstanceOf(env, object))\
            JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Cannot access member field %1." #field " on object of type %2.").arg(qtjambi_class_name(env, class_ref)).arg(qtjambi_object_class_name(env, object))) QTJAMBI_STACKTRACEINFO );\
        jobject _result = env->GetObjectField(object, __##field);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return jclass(_result);\
    }\
    public: inline void set_##field(JNIEnv* env,jobject object, jclass value) const {\
        __qt_resolve(env, this);\
        if(env->IsSameObject(object, nullptr))\
            JavaException::raiseNullPointerException(env, nullptr QTJAMBI_STACKTRACEINFO );\
        if(!isInstanceOf(env, object))\
            JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Cannot access member field %1." #field " on object of type %2.").arg(qtjambi_class_name(env, class_ref)).arg(qtjambi_object_class_name(env, object))) QTJAMBI_STACKTRACEINFO );\
        env->SetObjectField(object, __##field, value);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
    }

#define QTJAMBI_REPOSITORY_DECLARE_STRING_FIELD(field)\
    private: jfieldID __##field;\
    public: inline jstring field(JNIEnv* env,jobject object) const {\
        __qt_resolve(env, this);\
        if(env->IsSameObject(object, nullptr))\
            JavaException::raiseNullPointerException(env, nullptr QTJAMBI_STACKTRACEINFO );\
        if(!isInstanceOf(env, object))\
            JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Cannot access member field %1." #field " on object of type %2.").arg(qtjambi_class_name(env, class_ref)).arg(qtjambi_object_class_name(env, object))) QTJAMBI_STACKTRACEINFO );\
        jobject _result = env->GetObjectField(object, __##field);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return jstring(_result);\
    }\
    public: inline void set_##field(JNIEnv* env,jobject object, jstring value) const {\
        __qt_resolve(env, this);\
        if(env->IsSameObject(object, nullptr))\
            JavaException::raiseNullPointerException(env, nullptr QTJAMBI_STACKTRACEINFO );\
        if(!isInstanceOf(env, object))\
            JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Cannot access member field %1." #field " on object of type %2.").arg(qtjambi_class_name(env, class_ref)).arg(qtjambi_object_class_name(env, object))) QTJAMBI_STACKTRACEINFO );\
        env->SetObjectField(object, __##field, value);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
    }

#define QTJAMBI_REPOSITORY_DECLARE_INTARRAY_FIELD(field)\
    private: jfieldID __##field;\
    public: inline jintArray field(JNIEnv* env,jobject object) const {\
        __qt_resolve(env, this);\
        if(env->IsSameObject(object, nullptr))\
            JavaException::raiseNullPointerException(env, nullptr QTJAMBI_STACKTRACEINFO );\
        if(!isInstanceOf(env, object))\
            JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Cannot access member field %1." #field " on object of type %2.").arg(qtjambi_class_name(env, class_ref)).arg(qtjambi_object_class_name(env, object))) QTJAMBI_STACKTRACEINFO );\
        jobject _result = env->GetObjectField(object, __##field);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return jintArray(_result);\
    }\
    public: inline void set_##field(JNIEnv* env,jobject object, jintArray value) const {\
        __qt_resolve(env, this);\
        if(env->IsSameObject(object, nullptr))\
            JavaException::raiseNullPointerException(env, nullptr QTJAMBI_STACKTRACEINFO );\
        if(!isInstanceOf(env, object))\
            JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Cannot access member field %1." #field " on object of type %2.").arg(qtjambi_class_name(env, class_ref)).arg(qtjambi_object_class_name(env, object))) QTJAMBI_STACKTRACEINFO );\
        env->SetObjectField(object, __##field, value);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
    }

#define QTJAMBI_REPOSITORY_DECLARE_OBJECTARRAY_FIELD(field)\
    private: jfieldID __##field;\
    public: inline jobjectArray field(JNIEnv* env,jobject object) const {\
        __qt_resolve(env, this);\
        if(env->IsSameObject(object, nullptr))\
            JavaException::raiseNullPointerException(env, nullptr QTJAMBI_STACKTRACEINFO );\
        if(!isInstanceOf(env, object))\
            JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Cannot access member field %1." #field " on object of type %2.").arg(qtjambi_class_name(env, class_ref)).arg(qtjambi_object_class_name(env, object))) QTJAMBI_STACKTRACEINFO );\
        jobject _result = env->GetObjectField(object, __##field);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return jobjectArray(_result);\
    }\
    public: inline void set_##field(JNIEnv* env,jobject object, jobjectArray value) const {\
        __qt_resolve(env, this);\
        if(env->IsSameObject(object, nullptr))\
            JavaException::raiseNullPointerException(env, nullptr QTJAMBI_STACKTRACEINFO );\
        if(!isInstanceOf(env, object))\
            JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Cannot access member field %1." #field " on object of type %2.").arg(qtjambi_class_name(env, class_ref)).arg(qtjambi_object_class_name(env, object))) QTJAMBI_STACKTRACEINFO );\
        env->SetObjectField(object, __##field, value);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
    }

#define QTJAMBI_REPOSITORY_DECLARE_BYTE_FIELD(field)\
    private: jfieldID __##field;\
    public: inline jbyte field(JNIEnv* env,jobject object) const {\
        __qt_resolve(env, this);\
        if(env->IsSameObject(object, nullptr))\
            JavaException::raiseNullPointerException(env, nullptr QTJAMBI_STACKTRACEINFO );\
        if(!isInstanceOf(env, object))\
            JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Cannot access member field %1." #field " on object of type %2.").arg(qtjambi_class_name(env, class_ref)).arg(qtjambi_object_class_name(env, object))) QTJAMBI_STACKTRACEINFO );\
        jbyte _result = env->GetByteField(object, __##field);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return _result;\
    }\
    public: inline void set_##field(JNIEnv* env,jobject object, jbyte value) const {\
        __qt_resolve(env, this);\
        if(env->IsSameObject(object, nullptr))\
            JavaException::raiseNullPointerException(env, nullptr QTJAMBI_STACKTRACEINFO );\
        if(!isInstanceOf(env, object))\
            JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Cannot access member field %1." #field " on object of type %2.").arg(qtjambi_class_name(env, class_ref)).arg(qtjambi_object_class_name(env, object))) QTJAMBI_STACKTRACEINFO );\
        env->SetByteField(object, __##field, value);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
    }

#define QTJAMBI_REPOSITORY_DECLARE_SHORT_FIELD(field)\
    private: jfieldID __##field;\
    public: inline jshort field(JNIEnv* env,jobject object) const {\
        __qt_resolve(env, this);\
        if(env->IsSameObject(object, nullptr))\
            JavaException::raiseNullPointerException(env, nullptr QTJAMBI_STACKTRACEINFO );\
        if(!isInstanceOf(env, object))\
            JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Cannot access member field %1." #field " on object of type %2.").arg(qtjambi_class_name(env, class_ref)).arg(qtjambi_object_class_name(env, object))) QTJAMBI_STACKTRACEINFO );\
        jshort result = env->GetShortField(object, __##field);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return result;\
    }\
    public: inline void set_##field(JNIEnv* env,jobject object, jshort value) const {\
        __qt_resolve(env, this);\
        if(env->IsSameObject(object, nullptr))\
            JavaException::raiseNullPointerException(env, nullptr QTJAMBI_STACKTRACEINFO );\
        if(!isInstanceOf(env, object))\
            JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Cannot access member field %1." #field " on object of type %2.").arg(qtjambi_class_name(env, class_ref)).arg(qtjambi_object_class_name(env, object))) QTJAMBI_STACKTRACEINFO );\
        env->SetShortField(object, __##field, value);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
    }

#define QTJAMBI_REPOSITORY_DECLARE_INT_FIELD(field)\
    private: jfieldID __##field;\
    public: inline jint field(JNIEnv* env,jobject object) const {\
        __qt_resolve(env, this);\
        if(env->IsSameObject(object, nullptr))\
            JavaException::raiseNullPointerException(env, nullptr QTJAMBI_STACKTRACEINFO );\
        if(!isInstanceOf(env, object))\
            JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Cannot access member field %1." #field " on object of type %2.").arg(qtjambi_class_name(env, class_ref)).arg(qtjambi_object_class_name(env, object))) QTJAMBI_STACKTRACEINFO );\
        jint result = env->GetIntField(object, __##field);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return result;\
    }\
    public: inline void set_##field(JNIEnv* env,jobject object, jint value) const {\
        __qt_resolve(env, this);\
        if(env->IsSameObject(object, nullptr))\
            JavaException::raiseNullPointerException(env, nullptr QTJAMBI_STACKTRACEINFO );\
        if(!isInstanceOf(env, object))\
            JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Cannot access member field %1." #field " on object of type %2.").arg(qtjambi_class_name(env, class_ref)).arg(qtjambi_object_class_name(env, object))) QTJAMBI_STACKTRACEINFO );\
        env->SetIntField(object, __##field, value);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
    }

#define QTJAMBI_REPOSITORY_DECLARE_LONG_FIELD(field)\
    private: jfieldID __##field;\
    public: inline jlong field(JNIEnv* env,jobject object) const {\
        __qt_resolve(env, this);\
        if(env->IsSameObject(object, nullptr))\
            JavaException::raiseNullPointerException(env, nullptr QTJAMBI_STACKTRACEINFO );\
        if(!isInstanceOf(env, object))\
            JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Cannot access member field %1." #field " on object of type %2.").arg(qtjambi_class_name(env, class_ref)).arg(qtjambi_object_class_name(env, object))) QTJAMBI_STACKTRACEINFO );\
        jlong result = env->GetLongField(object, __##field);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return result;\
    }\
    public: inline void set_##field(JNIEnv* env,jobject object, jlong value) const {\
        __qt_resolve(env, this);\
        if(env->IsSameObject(object, nullptr))\
            JavaException::raiseNullPointerException(env, nullptr QTJAMBI_STACKTRACEINFO );\
        if(!isInstanceOf(env, object))\
            JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Cannot access member field %1." #field " on object of type %2.").arg(qtjambi_class_name(env, class_ref)).arg(qtjambi_object_class_name(env, object))) QTJAMBI_STACKTRACEINFO );\
        env->SetLongField(object, __##field, value);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
    }

#define QTJAMBI_REPOSITORY_DECLARE_FLOAT_FIELD(field)\
    private: jfieldID __##field;\
    public: inline jfloat field(JNIEnv* env,jobject object) const {\
        __qt_resolve(env, this);\
        if(env->IsSameObject(object, nullptr))\
            JavaException::raiseNullPointerException(env, nullptr QTJAMBI_STACKTRACEINFO );\
        if(!isInstanceOf(env, object))\
            JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Cannot access member field %1." #field " on object of type %2.").arg(qtjambi_class_name(env, class_ref)).arg(qtjambi_object_class_name(env, object))) QTJAMBI_STACKTRACEINFO );\
        jfloat result = env->GetFloatField(object, __##field);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return result;\
    }\
    public: inline void set_##field(JNIEnv* env,jobject object, jfloat value) const {\
        __qt_resolve(env, this);\
        if(env->IsSameObject(object, nullptr))\
            JavaException::raiseNullPointerException(env, nullptr QTJAMBI_STACKTRACEINFO );\
        if(!isInstanceOf(env, object))\
            JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Cannot access member field %1." #field " on object of type %2.").arg(qtjambi_class_name(env, class_ref)).arg(qtjambi_object_class_name(env, object))) QTJAMBI_STACKTRACEINFO );\
        env->SetFloatField(object, __##field, value);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
    }

#define QTJAMBI_REPOSITORY_DECLARE_DOUBLE_FIELD(field)\
    private: jfieldID __##field;\
    public: inline jdouble field(JNIEnv* env,jobject object) const {\
        __qt_resolve(env, this);\
        if(env->IsSameObject(object, nullptr))\
            JavaException::raiseNullPointerException(env, nullptr QTJAMBI_STACKTRACEINFO );\
        if(!isInstanceOf(env, object))\
            JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Cannot access member field %1." #field " on object of type %2.").arg(qtjambi_class_name(env, class_ref)).arg(qtjambi_object_class_name(env, object))) QTJAMBI_STACKTRACEINFO );\
        jdouble result = env->GetDoubleField(object, __##field);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return result;\
    }\
    public: inline void set_##field(JNIEnv* env,jobject object, jdouble value) const {\
        __qt_resolve(env, this);\
        if(env->IsSameObject(object, nullptr))\
            JavaException::raiseNullPointerException(env, nullptr QTJAMBI_STACKTRACEINFO );\
        if(!isInstanceOf(env, object))\
            JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Cannot access member field %1." #field " on object of type %2.").arg(qtjambi_class_name(env, class_ref)).arg(qtjambi_object_class_name(env, object))) QTJAMBI_STACKTRACEINFO );\
        env->SetDoubleField(object, __##field, value);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
    }

#define QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_FIELD(field)\
    private: jfieldID __##field;\
    public: inline jboolean field(JNIEnv* env,jobject object) const {\
        __qt_resolve(env, this);\
        if(env->IsSameObject(object, nullptr))\
            JavaException::raiseNullPointerException(env, nullptr QTJAMBI_STACKTRACEINFO );\
        if(!isInstanceOf(env, object))\
            JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Cannot access member field %1." #field " on object of type %2.").arg(qtjambi_class_name(env, class_ref)).arg(qtjambi_object_class_name(env, object))) QTJAMBI_STACKTRACEINFO );\
        jboolean result = env->GetBooleanField(object, __##field);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return result;\
    }\
    public: inline void set_##field(JNIEnv* env,jobject object, jboolean value) const {\
        __qt_resolve(env, this);\
        if(env->IsSameObject(object, nullptr))\
            JavaException::raiseNullPointerException(env, nullptr QTJAMBI_STACKTRACEINFO );\
        if(!isInstanceOf(env, object))\
            JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Cannot access member field %1." #field " on object of type %2.").arg(qtjambi_class_name(env, class_ref)).arg(qtjambi_object_class_name(env, object))) QTJAMBI_STACKTRACEINFO );\
        env->SetBooleanField(object, __##field, value);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
    }

#define QTJAMBI_REPOSITORY_DECLARE_CHAR_FIELD(field)\
    private: jfieldID __##field;\
    public: inline jchar field(JNIEnv* env,jobject object) const {\
        __qt_resolve(env, this);\
        if(env->IsSameObject(object, nullptr))\
            JavaException::raiseNullPointerException(env, nullptr QTJAMBI_STACKTRACEINFO );\
        if(!isInstanceOf(env, object))\
            JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Cannot access member field %1." #field " on object of type %2.").arg(qtjambi_class_name(env, class_ref)).arg(qtjambi_object_class_name(env, object))) QTJAMBI_STACKTRACEINFO );\
        jchar result = env->GetCharField(object, __##field);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return result;\
    }\
    public: inline void set_##field(JNIEnv* env,jobject object, jchar value) const {\
        __qt_resolve(env, this);\
        if(env->IsSameObject(object, nullptr))\
            JavaException::raiseNullPointerException(env, nullptr QTJAMBI_STACKTRACEINFO );\
        if(!isInstanceOf(env, object))\
            JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Cannot access member field %1." #field " on object of type %2.").arg(qtjambi_class_name(env, class_ref)).arg(qtjambi_object_class_name(env, object))) QTJAMBI_STACKTRACEINFO );\
        env->SetCharField(object, __##field, value);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
    }

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_FIELD(field)\
    private: jfieldID __##field;\
    public: inline jobject field(JNIEnv* env) const {\
        __qt_resolve(env, this);\
        jobject result = env->GetStaticObjectField(class_ref, __##field);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return result;\
    }\
    public: inline void set_##field(JNIEnv* env, jobject value) const {\
        __qt_resolve(env, this);\
        env->SetStaticObjectField(class_ref, __##field, value);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
    }

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_BYTE_FIELD(field)\
    private: jfieldID __##field;\
    public: inline jbyte field(JNIEnv* env) const {\
        __qt_resolve(env, this);\
        jbyte result = env->GetStaticByteField(class_ref, __##field);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return result;\
    }\
    public: inline void set_##field(JNIEnv* env, jbyte value) const {\
        __qt_resolve(env, this);\
        env->SetStaticByteField(class_ref, __##field, value);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
    }

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_SHORT_FIELD(field)\
    private: jfieldID __##field;\
    public: inline jshort field(JNIEnv* env) const {\
        __qt_resolve(env, this);\
        jshort result = env->GetStaticShortField(class_ref, __##field);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return result;\
    }\
    public: inline void set_##field(JNIEnv* env, jshort value) const {\
        __qt_resolve(env, this);\
        env->SetStaticShortField(class_ref, __##field, value);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
    }

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_INT_FIELD(field)\
    private: jfieldID __##field;\
    public: inline jint field(JNIEnv* env) const {\
        __qt_resolve(env, this);\
        jint result = env->GetStaticIntField(class_ref, __##field);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return result;\
    }\
    public: inline void set_##field(JNIEnv* env, jint value) const {\
        __qt_resolve(env, this);\
        env->SetStaticIntField(class_ref, __##field, value);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
    }

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_LONG_FIELD(field)\
    private: jfieldID __##field;\
    public: inline jlong field(JNIEnv* env) const {\
        __qt_resolve(env, this);\
        jlong result = env->GetStaticLongField(class_ref, __##field);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return result;\
    }\
    public: inline void set_##field(JNIEnv* env, jlong value) const {\
        __qt_resolve(env, this);\
        env->SetStaticLongField(class_ref, __##field, value);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
    }

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_FLOAT_FIELD(field)\
    private: jfieldID __##field;\
    public: inline jfloat field(JNIEnv* env) const {\
        __qt_resolve(env, this);\
        jfloat result = env->GetStaticFloatField(class_ref, __##field);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return result;\
    }\
    public: inline void set_##field(JNIEnv* env, jfloat value) const {\
        __qt_resolve(env, this);\
        env->SetStaticFloatField(class_ref, __##field, value);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
    }

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_DOUBLE_FIELD(field)\
    private: jfieldID __##field;\
    public: inline jdouble field(JNIEnv* env) const {\
        __qt_resolve(env, this);\
        jdouble result = env->GetStaticDoubleField(class_ref, __##field);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return result;\
    }\
    public: inline void set_##field(JNIEnv* env, jdouble value) const {\
        __qt_resolve(env, this);\
        env->SetStaticDoubleField(class_ref, __##field, value);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
    }

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_BOOLEAN_FIELD(field)\
    private: jfieldID __##field;\
    public: inline jboolean field(JNIEnv* env) const {\
        __qt_resolve(env, this);\
        jboolean result = env->GetStaticBooleanField(class_ref, __##field);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return result;\
    }\
    public: inline void set_##field(JNIEnv* env, jboolean value) const {\
        __qt_resolve(env, this);\
        env->SetStaticBooleanField(class_ref, __##field, value);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
    }

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_CHAR_FIELD(field)\
    private: jfieldID __##field;\
    public: inline jchar field(JNIEnv* env) const {\
        __qt_resolve(env, this);\
        jchar result = env->GetStaticCharField(class_ref, __##field);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return result;\
    }\
    public: inline void set_##field(JNIEnv* env, jchar value) const {\
        __qt_resolve(env, this);\
        env->SetStaticCharField(class_ref, __##field, value);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
    }

#define DECLARE_CLASS_REF(cls)\
    private: jclass __##cls;\
    public: inline bool isPrimitiveType(JNIEnv* env, jclass type) const{\
        __qt_resolve(env, this);\
        return env->IsSameObject(type, __##cls);\
    }\
    inline jclass primitiveType(JNIEnv* env) const{\
        __qt_resolve(env, this);\
        return jclass(env->NewLocalRef(__##cls));\
    }

#define POST_CLASS_REF_ACTION(class_ref)

#define QTJAMBI_REPOSITORY_DEFINE_CLASS(container,package, type_name, content)\
const container::_##type_name container::type_name;\
void empty_resolver_##type_name(JNIEnv *, const container::_##type_name*){}\
container::_##type_name::_##type_name() : class_ref(nullptr), __qt_resolve([](JNIEnv *env, const _##type_name* const_this) { \
        QMutexLocker locker(gMutex()); \
        Q_UNUSED(locker)\
        container::_##type_name* _this = const_cast<container::_##type_name*>(const_this);\
        if (!_this->class_ref){\
            {\
                _this->class_ref = resolveClass(env, #package"/"#type_name);\
                Q_ASSERT(_this->class_ref);\
                POST_CLASS_REF_ACTION(_this->class_ref)\
            }\
            content\
        }\
        Q_ASSERT(_this->class_ref);\
        _this->__qt_resolve = &empty_resolver_##type_name;\
    }){}\
jclass container::_##type_name::getClass(JNIEnv* env) const {\
    __qt_resolve(env, this);\
    return jclass(env->NewLocalRef(class_ref));\
}\
jboolean container::_##type_name::isInstanceOf(JNIEnv* env,jobject instance) const {\
    __qt_resolve(env, this);\
    jboolean result = !env->IsSameObject(instance, nullptr) && env->IsInstanceOf(instance, class_ref);\
    qtjambi_throw_java_exception_no_stackinfo(env)\
    return result;\
}\
jboolean container::_##type_name::isAssignableFrom(JNIEnv* env,jclass otherClass) const {\
    __qt_resolve(env, this);\
    jboolean result = env->IsAssignableFrom(otherClass, class_ref);\
    qtjambi_throw_java_exception_no_stackinfo(env)\
    return result;\
}\
jboolean container::_##type_name::isSameClass(JNIEnv* env,jclass otherClass) const {\
    __qt_resolve(env, this);\
    jboolean result = env->IsSameObject(otherClass, class_ref);\
    qtjambi_throw_java_exception_no_stackinfo(env)\
    return result;\
}\
jobjectArray container::_##type_name::newArray(JNIEnv* env,jsize size) const {\
    __qt_resolve(env, this);\
    jobjectArray result = env->NewObjectArray(size, class_ref, nullptr);\
    qtjambi_throw_java_exception_no_stackinfo(env)\
    return result;\
}

#define QTJAMBI_REPOSITORY_DEFINE_FIELD(field_name, signature)\
    _this->__##field_name = env->GetFieldID(_this->class_ref, #field_name, #signature);\
    qtjambi_throw_java_exception_no_stackinfo(env)\
    Q_ASSERT(_this->__##field_name);

#define QTJAMBI_REPOSITORY_DEFINE_STATIC_FIELD(field_name, signature)\
    _this->__##field_name = env->GetStaticFieldID(_this->class_ref, #field_name, #signature);\
    qtjambi_throw_java_exception_no_stackinfo(env)\
    Q_ASSERT(_this->__##field_name);

#define QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(signature)\
    _this->__constructor = env->GetMethodID(_this->class_ref, "<init>", "("#signature")V");\
    qtjambi_throw_java_exception_no_stackinfo(env)\
    Q_ASSERT(_this->__constructor);

#define QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR2(signature)\
    _this->__constructor2 = env->GetMethodID(_this->class_ref, "<init>", "("#signature")V");\
    qtjambi_throw_java_exception_no_stackinfo(env)\
    Q_ASSERT(_this->__constructor2);

#define QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR3(signature)\
    _this->__constructor3 = env->GetMethodID(_this->class_ref, "<init>", "("#signature")V");\
    qtjambi_throw_java_exception_no_stackinfo(env)\
    Q_ASSERT(_this->__constructor3);

#define QTJAMBI_REPOSITORY_DEFINE_RENAMED_METHOD(target, method_name, signature)\
    _this->__##target = env->GetMethodID(_this->class_ref, #method_name, #signature);\
    qtjambi_throw_java_exception_no_stackinfo(env)\
    Q_ASSERT(_this->__##target);

#define QTJAMBI_REPOSITORY_DEFINE_METHOD(method_name, signature)\
    _this->__##method_name = env->GetMethodID(_this->class_ref, #method_name, #signature);\
    qtjambi_throw_java_exception_no_stackinfo(env)\
    Q_ASSERT(_this->__##method_name);

#define QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(method_name, signature)\
    _this->__##method_name = env->GetStaticMethodID(_this->class_ref, #method_name, #signature);\
    qtjambi_throw_java_exception_no_stackinfo(env)\
    Q_ASSERT(_this->__##method_name);

#define QTJAMBI_REPOSITORY_DEFINE_RENAMED_STATIC_METHOD(target, method_name, signature)\
    _this->__##target = env->GetStaticMethodID(_this->class_ref, #method_name, #signature);\
    qtjambi_throw_java_exception_no_stackinfo(env)\
    Q_ASSERT(_this->__##target);

#define QTJAMBI_REPOSITORY_DEFINE_STANDARD_CONSTRUCTOR()\
    _this->__constructor = env->GetMethodID(_this->class_ref, "<init>", "()V");\
    qtjambi_throw_java_exception_no_stackinfo(env)\
    Q_ASSERT(_this->__constructor);

#define QTJAMBI_REPOSITORY_DEFINE_EMPTY_CLASS(container,package, type_name)\
    QTJAMBI_REPOSITORY_DEFINE_CLASS(container,package, type_name,)

#define QTJAMBI_REPOSITORY_DEFINE_CLASS_SC(container,package, type_name)\
    QTJAMBI_REPOSITORY_DEFINE_CLASS(container,package, type_name, QTJAMBI_REPOSITORY_DEFINE_STANDARD_CONSTRUCTOR())

#endif // QTJAMBI_REPODEFINES_H
