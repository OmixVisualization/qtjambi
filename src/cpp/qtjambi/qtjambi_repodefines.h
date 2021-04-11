/****************************************************************************
**
** Copyright (C) 2009-2021 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
        qtjambi_throw_java_exception_no_stackinfo(env)\
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
        qtjambi_throw_java_exception_no_stackinfo(env)\
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
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return result;\
    }

#ifdef QT_QTJAMBI_PORT
#define QTJAMBI_REPOSITORY_DECLARE_VOID_AMETHOD(method)\
    private: jmethodID __##method;\
    public: static inline void method(JNIEnv* env,jobject object,jvalue* arguments){\
        auto _this = __qt_get_this(env);\
        env->CallVoidMethodA(object,_this.__##method,arguments);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
    }
#endif

#define QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(method)\
    private: jmethodID __##method;\
    public: static inline void method(JNIEnv* env,jobject object,...){\
        auto _this = __qt_get_this(env);\
        if(!_this.class_ref) return;\
        if(env->IsSameObject(object, nullptr))\
            JavaException::raiseNullPointerException(env, nullptr QTJAMBI_STACKTRACEINFO );\
        if(!env->IsInstanceOf(object, _this.class_ref))\
            JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Cannot invoke member function %1." #method "(...) on object of type %2.").arg(qtjambi_class_name(env, _this.class_ref)).arg(qtjambi_object_class_name(env, object))) QTJAMBI_STACKTRACEINFO );\
        va_list args;\
        va_start(args, object);\
        env->CallVoidMethodV(object,_this.__##method,args);\
        va_end(args);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
    }

#define QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(method)\
    private: jmethodID __##method;\
    public: static inline jobject method(JNIEnv* env,jobject object,...){\
        auto _this = __qt_get_this(env);\
        if(!_this.class_ref) return nullptr;\
        va_list args;\
        if(env->IsSameObject(object, nullptr))\
            JavaException::raiseNullPointerException(env, nullptr QTJAMBI_STACKTRACEINFO );\
        if(!env->IsInstanceOf(object, _this.class_ref))\
            JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Cannot invoke member function %1." #method "(...) on object of type %2.").arg(qtjambi_class_name(env, _this.class_ref)).arg(qtjambi_object_class_name(env, object))) QTJAMBI_STACKTRACEINFO );\
        jobject result;\
        va_start(args, object);\
        result = env->CallObjectMethodV(object,_this.__##method,args);\
        va_end(args);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return result;\
    }

#define QTJAMBI_REPOSITORY_DECLARE_METHOD_ID(method)\
    public: static inline jmethodID method##_ID(JNIEnv* env){\
        auto _this = __qt_get_this(env);\
        if(!_this.class_ref) return nullptr;\
        return _this.__##method;\
    }

#define QTJAMBI_REPOSITORY_DECLARE_CLASS_METHOD(method)\
    private: jmethodID __##method;\
    public: static inline jclass method(JNIEnv* env,jobject object,...){\
        auto _this = __qt_get_this(env);\
        if(!_this.class_ref) return nullptr;\
        if(env->IsSameObject(object, nullptr))\
            JavaException::raiseNullPointerException(env, nullptr QTJAMBI_STACKTRACEINFO );\
        if(!env->IsInstanceOf(object, _this.class_ref))\
            JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Cannot invoke member function %1." #method "(...) on object of type %2.").arg(qtjambi_class_name(env, _this.class_ref)).arg(qtjambi_object_class_name(env, object))) QTJAMBI_STACKTRACEINFO );\
        va_list args;\
        jobject result;\
        va_start(args, object);\
        result = env->CallObjectMethodV(object,_this.__##method,args);\
        va_end(args);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return jclass(result);\
    }

#define QTJAMBI_REPOSITORY_DECLARE_OBJECTARRAY_METHOD(method)\
    private: jmethodID __##method;\
    public: static inline jobjectArray method(JNIEnv* env,jobject object,...){\
        auto _this = __qt_get_this(env);\
        if(!_this.class_ref) return nullptr;\
        if(env->IsSameObject(object, nullptr))\
            JavaException::raiseNullPointerException(env, nullptr QTJAMBI_STACKTRACEINFO );\
        if(!env->IsInstanceOf(object, _this.class_ref))\
            JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Cannot invoke member function %1." #method "(...) on object of type %2.").arg(qtjambi_class_name(env, _this.class_ref)).arg(qtjambi_object_class_name(env, object))) QTJAMBI_STACKTRACEINFO );\
        va_list args;\
        jobject result;\
        va_start(args, object);\
        result = env->CallObjectMethodV(object,_this.__##method,args);\
        va_end(args);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return jobjectArray(result);\
    }

#define QTJAMBI_REPOSITORY_DECLARE_STRING_METHOD(method)\
    private: jmethodID __##method;\
    public: static inline jstring method(JNIEnv* env,jobject object,...){\
        auto _this = __qt_get_this(env);\
        if(!_this.class_ref) return nullptr;\
        if(env->IsSameObject(object, nullptr))\
            JavaException::raiseNullPointerException(env, nullptr QTJAMBI_STACKTRACEINFO );\
        if(!env->IsInstanceOf(object, _this.class_ref))\
            JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Cannot invoke member function %1." #method "(...) on object of type %2.").arg(qtjambi_class_name(env, _this.class_ref)).arg(qtjambi_object_class_name(env, object))) QTJAMBI_STACKTRACEINFO );\
        va_list args;\
        jobject result;\
        va_start(args, object);\
        result = env->CallObjectMethodV(object,_this.__##method,args);\
        va_end(args);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return jstring(result);\
    }

#define QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(method)\
    private: jmethodID __##method;\
    public: static inline jint method(JNIEnv* env,jobject object,...){\
        auto _this = __qt_get_this(env);\
        if(!_this.class_ref) return 0;\
        if(env->IsSameObject(object, nullptr))\
            JavaException::raiseNullPointerException(env, nullptr QTJAMBI_STACKTRACEINFO );\
        if(!env->IsInstanceOf(object, _this.class_ref))\
            JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Cannot invoke member function %1." #method "(...) on object of type %2.").arg(qtjambi_class_name(env, _this.class_ref)).arg(qtjambi_object_class_name(env, object))) QTJAMBI_STACKTRACEINFO );\
        va_list args;\
        jint result;\
        va_start(args, object);\
        result = env->CallIntMethodV(object,_this.__##method,args);\
        va_end(args);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return result;\
    }

#define QTJAMBI_REPOSITORY_DECLARE_LONG_METHOD(method)\
    private: jmethodID __##method;\
    public: static inline jlong method(JNIEnv* env,jobject object,...){\
        auto _this = __qt_get_this(env);\
        if(!_this.class_ref) return 0;\
        if(env->IsSameObject(object, nullptr))\
            JavaException::raiseNullPointerException(env, nullptr QTJAMBI_STACKTRACEINFO );\
        if(!env->IsInstanceOf(object, _this.class_ref))\
            JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Cannot invoke member function %1." #method "(...) on object of type %2.").arg(qtjambi_class_name(env, _this.class_ref)).arg(qtjambi_object_class_name(env, object))) QTJAMBI_STACKTRACEINFO );\
        va_list args;\
        jlong result;\
        va_start(args, object);\
        result = env->CallLongMethodV(object,_this.__##method,args);\
        va_end(args);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return result;\
    }

#define QTJAMBI_REPOSITORY_DECLARE_SHORT_METHOD(method)\
    private: jmethodID __##method;\
    public: static inline jshort method(JNIEnv* env,jobject object,...){\
        auto _this = __qt_get_this(env);\
        if(!_this.class_ref) return 0;\
        if(env->IsSameObject(object, nullptr))\
            JavaException::raiseNullPointerException(env, nullptr QTJAMBI_STACKTRACEINFO );\
        if(!env->IsInstanceOf(object, _this.class_ref))\
            JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Cannot invoke member function %1." #method "(...) on object of type %2.").arg(qtjambi_class_name(env, _this.class_ref)).arg(qtjambi_object_class_name(env, object))) QTJAMBI_STACKTRACEINFO );\
        va_list args;\
        jshort result;\
        va_start(args, object);\
        result = env->CallShortMethodV(object,_this.__##method,args);\
        va_end(args);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return result;\
    }

#define QTJAMBI_REPOSITORY_DECLARE_BYTE_METHOD(method)\
    private: jmethodID __##method;\
    public: static inline jbyte method(JNIEnv* env,jobject object,...){\
        auto _this = __qt_get_this(env);\
        if(!_this.class_ref) return 0;\
        if(env->IsSameObject(object, nullptr))\
            JavaException::raiseNullPointerException(env, nullptr QTJAMBI_STACKTRACEINFO );\
        if(!env->IsInstanceOf(object, _this.class_ref))\
            JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Cannot invoke member function %1." #method "(...) on object of type %2.").arg(qtjambi_class_name(env, _this.class_ref)).arg(qtjambi_object_class_name(env, object))) QTJAMBI_STACKTRACEINFO );\
        va_list args;\
        jbyte result;\
        va_start(args, object);\
        result = env->CallByteMethodV(object,_this.__##method,args);\
        va_end(args);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return result;\
    }

#define QTJAMBI_REPOSITORY_DECLARE_DOUBLE_METHOD(method)\
    private: jmethodID __##method;\
    public: static inline jdouble method(JNIEnv* env,jobject object,...){\
        auto _this = __qt_get_this(env);\
        if(!_this.class_ref) return 0;\
        if(env->IsSameObject(object, nullptr))\
            JavaException::raiseNullPointerException(env, nullptr QTJAMBI_STACKTRACEINFO );\
        if(!env->IsInstanceOf(object, _this.class_ref))\
            JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Cannot invoke member function %1." #method "(...) on object of type %2.").arg(qtjambi_class_name(env, _this.class_ref)).arg(qtjambi_object_class_name(env, object))) QTJAMBI_STACKTRACEINFO );\
        va_list args;\
        jdouble result;\
        va_start(args, object);\
        result = env->CallDoubleMethodV(object,_this.__##method,args);\
        va_end(args);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return result;\
    }

#define QTJAMBI_REPOSITORY_DECLARE_FLOAT_METHOD(method)\
    private: jmethodID __##method;\
    public: static inline jfloat method(JNIEnv* env,jobject object,...){\
        auto _this = __qt_get_this(env);\
        if(!_this.class_ref) return 0;\
        if(env->IsSameObject(object, nullptr))\
            JavaException::raiseNullPointerException(env, nullptr QTJAMBI_STACKTRACEINFO );\
        if(!env->IsInstanceOf(object, _this.class_ref))\
            JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Cannot invoke member function %1." #method "(...) on object of type %2.").arg(qtjambi_class_name(env, _this.class_ref)).arg(qtjambi_object_class_name(env, object))) QTJAMBI_STACKTRACEINFO );\
        va_list args;\
        jfloat result;\
        va_start(args, object);\
        result = env->CallFloatMethodV(object,_this.__##method,args);\
        va_end(args);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return result;\
    }

#define QTJAMBI_REPOSITORY_DECLARE_CHAR_METHOD(method)\
    private: jmethodID __##method;\
    public: static inline jchar method(JNIEnv* env,jobject object,...){\
        auto _this = __qt_get_this(env);\
        if(!_this.class_ref) return 0;\
        if(env->IsSameObject(object, nullptr))\
            JavaException::raiseNullPointerException(env, nullptr QTJAMBI_STACKTRACEINFO );\
        if(!env->IsInstanceOf(object, _this.class_ref))\
            JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Cannot invoke member function %1." #method "(...) on object of type %2.").arg(qtjambi_class_name(env, _this.class_ref)).arg(qtjambi_object_class_name(env, object))) QTJAMBI_STACKTRACEINFO );\
        va_list args;\
        jchar result;\
        va_start(args, object);\
        result = env->CallCharMethodV(object,_this.__##method,args);\
        va_end(args);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return result;\
    }

#define QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(method)\
    private: jmethodID __##method;\
    public: static inline jboolean method(JNIEnv* env,jobject object,...){\
        auto _this = __qt_get_this(env);\
        if(!_this.class_ref) return false;\
        if(env->IsSameObject(object, nullptr))\
            JavaException::raiseNullPointerException(env, nullptr QTJAMBI_STACKTRACEINFO );\
        if(!env->IsInstanceOf(object, _this.class_ref))\
            JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Cannot invoke member function %1." #method "(...) on object of type %2.").arg(qtjambi_class_name(env, _this.class_ref)).arg(qtjambi_object_class_name(env, object))) QTJAMBI_STACKTRACEINFO );\
        va_list args;\
        jboolean result;\
        va_start(args, object);\
        result = env->CallBooleanMethodV(object,_this.__##method,args);\
        va_end(args);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return result;\
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
        qtjambi_throw_java_exception_no_stackinfo(env)\
    }

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(method)\
    private: jmethodID __##method;\
    public: static inline jobject method(JNIEnv* env,...){\
        auto _this = __qt_get_this(env);\
        if(!_this.class_ref) return nullptr;\
        va_list args;\
        jobject result;\
        va_start(args, env);\
        result = env->CallStaticObjectMethodV(_this.class_ref,_this.__##method,args);\
        va_end(args);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return result;\
    }

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECTARRAY_METHOD(method)\
    private: jmethodID __##method;\
    public: static inline jobjectArray method(JNIEnv* env,...){\
        auto _this = __qt_get_this(env);\
        if(!_this.class_ref) return nullptr;\
        va_list args;\
        jobject result;\
        va_start(args, env);\
        result = env->CallStaticObjectMethodV(_this.class_ref,_this.__##method,args);\
        va_end(args);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return jobjectArray(result);\
    }

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_CLASS_METHOD(method)\
    private: jmethodID __##method;\
    public: static inline jclass method(JNIEnv* env,...){\
        auto _this = __qt_get_this(env);\
        if(!_this.class_ref) return nullptr;\
        va_list args;\
        jobject result;\
        va_start(args, env);\
        result = env->CallStaticObjectMethodV(_this.class_ref,_this.__##method,args);\
        va_end(args);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return jclass(result);\
    }

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_STRING_METHOD(method)\
    private: jmethodID __##method;\
    public: static inline jstring method(JNIEnv* env,...){\
        auto _this = __qt_get_this(env);\
        if(!_this.class_ref) return nullptr;\
        va_list args;\
        jobject result;\
        va_start(args, env);\
        result = env->CallStaticObjectMethodV(_this.class_ref,_this.__##method,args);\
        va_end(args);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return jstring(result);\
    }

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_INT_METHOD(method)\
    private: jmethodID __##method;\
    public: static inline jint method(JNIEnv* env,...){\
        auto _this = __qt_get_this(env);\
        if(!_this.class_ref) return 0;\
        va_list args;\
        jint result;\
        va_start(args, env);\
        result = env->CallStaticIntMethodV(_this.class_ref,_this.__##method,args);\
        va_end(args);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return result;\
    }

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_LONG_METHOD(method)\
    private: jmethodID __##method;\
    public: static inline jlong method(JNIEnv* env,...){\
        auto _this = __qt_get_this(env);\
        if(!_this.class_ref) return 0;\
        va_list args;\
        jlong result;\
        va_start(args, env);\
        result = env->CallStaticLongMethodV(_this.class_ref,_this.__##method,args);\
        va_end(args);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return result;\
    }

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_SHORT_METHOD(method)\
    private: jmethodID __##method;\
    public: static inline jshort method(JNIEnv* env,...){\
        auto _this = __qt_get_this(env);\
        if(!_this.class_ref) return 0;\
        va_list args;\
        jshort result;\
        va_start(args, env);\
        result = env->CallStaticShortMethodV(_this.class_ref,_this.__##method,args);\
        va_end(args);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return result;\
    }

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_BYTE_METHOD(method)\
    private: jmethodID __##method;\
    public: static inline jbyte method(JNIEnv* env,...){\
        auto _this = __qt_get_this(env);\
        if(!_this.class_ref) return 0;\
        va_list args;\
        jbyte result;\
        va_start(args, env);\
        result = env->CallStaticByteMethodV(_this.class_ref,_this.__##method,args);\
        va_end(args);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return result;\
    }

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_DOUBLE_METHOD(method)\
    private: jmethodID __##method;\
    public: static inline jdouble method(JNIEnv* env,...){\
        auto _this = __qt_get_this(env);\
        if(!_this.class_ref) return 0;\
        va_list args;\
        jdouble result;\
        va_start(args, env);\
        result = env->CallStaticDoubleMethodV(_this.class_ref,_this.__##method,args);\
        va_end(args);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return result;\
    }

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_FLOAT_METHOD(method)\
    private: jmethodID __##method;\
    public: static inline jfloat method(JNIEnv* env,...){\
        auto _this = __qt_get_this(env);\
        if(!_this.class_ref) return 0;\
        va_list args;\
        jfloat result;\
        va_start(args, env);\
        result = env->CallStaticFloatMethodV(_this.class_ref,_this.__##method,args);\
        va_end(args);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return result;\
    }

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_CHAR_METHOD(method)\
    private: jmethodID __##method;\
    public: static inline jchar method(JNIEnv* env,...){\
        auto _this = __qt_get_this(env);\
        if(!_this.class_ref) return 0;\
        va_list args;\
        jchar result;\
        va_start(args, env);\
        result = env->CallStaticCharMethodV(_this.class_ref,_this.__##method,args);\
        va_end(args);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return result;\
    }

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_BOOLEAN_METHOD(method)\
    private: jmethodID __##method;\
    public: static inline jboolean method(JNIEnv* env,...){\
        auto _this = __qt_get_this(env);\
        if(!_this.class_ref) return false;\
        va_list args;\
        jboolean result;\
        va_start(args, env);\
        result = env->CallStaticBooleanMethodV(_this.class_ref,_this.__##method,args);\
        va_end(args);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return result;\
    }

#define QTJAMBI_REPOSITORY_DECLARE_OBJECT_FIELD(field)\
    private: jfieldID __##field;\
    public: static inline jobject field(JNIEnv* env,jobject object){\
        auto _this = __qt_get_this(env);\
        if(!_this.class_ref) return nullptr;\
        if(env->IsSameObject(object, nullptr))\
            JavaException::raiseNullPointerException(env, nullptr QTJAMBI_STACKTRACEINFO );\
        if(!env->IsInstanceOf(object, _this.class_ref))\
            JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Cannot access member field %1." #field " on object of type %2.").arg(qtjambi_class_name(env, _this.class_ref)).arg(qtjambi_object_class_name(env, object))) QTJAMBI_STACKTRACEINFO );\
        jobject _result = env->GetObjectField(object, _this.__##field);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return _result;\
    }\
    public: static inline void set_##field(JNIEnv* env,jobject object, jobject _value){\
        auto _this = __qt_get_this(env);\
        if(!_this.class_ref) return;\
        if(env->IsSameObject(object, nullptr))\
            JavaException::raiseNullPointerException(env, nullptr QTJAMBI_STACKTRACEINFO );\
        if(!env->IsInstanceOf(object, _this.class_ref))\
            JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Cannot access member field %1." #field " on object of type %2.").arg(qtjambi_class_name(env, _this.class_ref)).arg(qtjambi_object_class_name(env, object))) QTJAMBI_STACKTRACEINFO );\
        env->SetObjectField(object, _this.__##field, _value);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
    }

#define QTJAMBI_REPOSITORY_DECLARE_CLASS_FIELD(field)\
    private: jfieldID __##field;\
    public: static inline jclass field(JNIEnv* env,jobject object){\
        auto _this = __qt_get_this(env);\
        if(!_this.class_ref) return nullptr;\
        if(env->IsSameObject(object, nullptr))\
            JavaException::raiseNullPointerException(env, nullptr QTJAMBI_STACKTRACEINFO );\
        if(!env->IsInstanceOf(object, _this.class_ref))\
            JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Cannot access member field %1." #field " on object of type %2.").arg(qtjambi_class_name(env, _this.class_ref)).arg(qtjambi_object_class_name(env, object))) QTJAMBI_STACKTRACEINFO );\
        jobject _result = env->GetObjectField(object, _this.__##field);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return jclass(_result);\
    }\
    public: static inline void set_##field(JNIEnv* env,jobject object, jclass value){\
        auto _this = __qt_get_this(env);\
        if(!_this.class_ref) return;\
        if(env->IsSameObject(object, nullptr))\
            JavaException::raiseNullPointerException(env, nullptr QTJAMBI_STACKTRACEINFO );\
        if(!env->IsInstanceOf(object, _this.class_ref))\
            JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Cannot access member field %1." #field " on object of type %2.").arg(qtjambi_class_name(env, _this.class_ref)).arg(qtjambi_object_class_name(env, object))) QTJAMBI_STACKTRACEINFO );\
        env->SetObjectField(object, _this.__##field, value);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
    }

#define QTJAMBI_REPOSITORY_DECLARE_STRING_FIELD(field)\
    private: jfieldID __##field;\
    public: static inline jstring field(JNIEnv* env,jobject object){\
        auto _this = __qt_get_this(env);\
        if(!_this.class_ref) return nullptr;\
        if(env->IsSameObject(object, nullptr))\
            JavaException::raiseNullPointerException(env, nullptr QTJAMBI_STACKTRACEINFO );\
        if(!env->IsInstanceOf(object, _this.class_ref))\
            JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Cannot access member field %1." #field " on object of type %2.").arg(qtjambi_class_name(env, _this.class_ref)).arg(qtjambi_object_class_name(env, object))) QTJAMBI_STACKTRACEINFO );\
        jobject _result = env->GetObjectField(object, _this.__##field);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return jstring(_result);\
    }\
    public: static inline void set_##field(JNIEnv* env,jobject object, jstring value){\
        auto _this = __qt_get_this(env);\
        if(!_this.class_ref) return;\
        if(env->IsSameObject(object, nullptr))\
            JavaException::raiseNullPointerException(env, nullptr QTJAMBI_STACKTRACEINFO );\
        if(!env->IsInstanceOf(object, _this.class_ref))\
            JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Cannot access member field %1." #field " on object of type %2.").arg(qtjambi_class_name(env, _this.class_ref)).arg(qtjambi_object_class_name(env, object))) QTJAMBI_STACKTRACEINFO );\
        env->SetObjectField(object, _this.__##field, value);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
    }

#define QTJAMBI_REPOSITORY_DECLARE_INTARRAY_FIELD(field)\
    private: jfieldID __##field;\
    public: static inline jintArray field(JNIEnv* env,jobject object){\
        auto _this = __qt_get_this(env);\
        if(!_this.class_ref) return nullptr;\
        if(env->IsSameObject(object, nullptr))\
            JavaException::raiseNullPointerException(env, nullptr QTJAMBI_STACKTRACEINFO );\
        if(!env->IsInstanceOf(object, _this.class_ref))\
            JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Cannot access member field %1." #field " on object of type %2.").arg(qtjambi_class_name(env, _this.class_ref)).arg(qtjambi_object_class_name(env, object))) QTJAMBI_STACKTRACEINFO );\
        jobject _result = env->GetObjectField(object, _this.__##field);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return jintArray(_result);\
    }\
    public: static inline void set_##field(JNIEnv* env,jobject object, jintArray value){\
        auto _this = __qt_get_this(env);\
        if(!_this.class_ref) return;\
        if(env->IsSameObject(object, nullptr))\
            JavaException::raiseNullPointerException(env, nullptr QTJAMBI_STACKTRACEINFO );\
        if(!env->IsInstanceOf(object, _this.class_ref))\
            JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Cannot access member field %1." #field " on object of type %2.").arg(qtjambi_class_name(env, _this.class_ref)).arg(qtjambi_object_class_name(env, object))) QTJAMBI_STACKTRACEINFO );\
        env->SetObjectField(object, _this.__##field, value);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
    }

#define QTJAMBI_REPOSITORY_DECLARE_OBJECTARRAY_FIELD(field)\
    private: jfieldID __##field;\
    public: static inline jobjectArray field(JNIEnv* env,jobject object){\
        auto _this = __qt_get_this(env);\
        if(!_this.class_ref) return nullptr;\
        if(env->IsSameObject(object, nullptr))\
            JavaException::raiseNullPointerException(env, nullptr QTJAMBI_STACKTRACEINFO );\
        if(!env->IsInstanceOf(object, _this.class_ref))\
            JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Cannot access member field %1." #field " on object of type %2.").arg(qtjambi_class_name(env, _this.class_ref)).arg(qtjambi_object_class_name(env, object))) QTJAMBI_STACKTRACEINFO );\
        jobject _result = env->GetObjectField(object, _this.__##field);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return jobjectArray(_result);\
    }\
    public: static inline void set_##field(JNIEnv* env,jobject object, jobjectArray value){\
        auto _this = __qt_get_this(env);\
        if(!_this.class_ref) return;\
        if(env->IsSameObject(object, nullptr))\
            JavaException::raiseNullPointerException(env, nullptr QTJAMBI_STACKTRACEINFO );\
        if(!env->IsInstanceOf(object, _this.class_ref))\
            JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Cannot access member field %1." #field " on object of type %2.").arg(qtjambi_class_name(env, _this.class_ref)).arg(qtjambi_object_class_name(env, object))) QTJAMBI_STACKTRACEINFO );\
        env->SetObjectField(object, _this.__##field, value);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
    }

#define QTJAMBI_REPOSITORY_DECLARE_BYTE_FIELD(field)\
    private: jfieldID __##field;\
    public: static inline jbyte field(JNIEnv* env,jobject object){\
        auto _this = __qt_get_this(env);\
        if(!_this.class_ref) return 0;\
        if(env->IsSameObject(object, nullptr))\
            JavaException::raiseNullPointerException(env, nullptr QTJAMBI_STACKTRACEINFO );\
        if(!env->IsInstanceOf(object, _this.class_ref))\
            JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Cannot access member field %1." #field " on object of type %2.").arg(qtjambi_class_name(env, _this.class_ref)).arg(qtjambi_object_class_name(env, object))) QTJAMBI_STACKTRACEINFO );\
        jbyte _result = env->GetByteField(object, _this.__##field);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return _result;\
    }\
    public: static inline void set_##field(JNIEnv* env,jobject object, jbyte value){\
        auto _this = __qt_get_this(env);\
    if(!_this.class_ref) return;\
        if(env->IsSameObject(object, nullptr))\
            JavaException::raiseNullPointerException(env, nullptr QTJAMBI_STACKTRACEINFO );\
        if(!env->IsInstanceOf(object, _this.class_ref))\
            JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Cannot access member field %1." #field " on object of type %2.").arg(qtjambi_class_name(env, _this.class_ref)).arg(qtjambi_object_class_name(env, object))) QTJAMBI_STACKTRACEINFO );\
        env->SetByteField(object, _this.__##field, value);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
    }

#define QTJAMBI_REPOSITORY_DECLARE_SHORT_FIELD(field)\
    private: jfieldID __##field;\
    public: static inline jshort field(JNIEnv* env,jobject object){\
        auto _this = __qt_get_this(env);\
        if(!_this.class_ref) return 0;\
        if(env->IsSameObject(object, nullptr))\
            JavaException::raiseNullPointerException(env, nullptr QTJAMBI_STACKTRACEINFO );\
        if(!env->IsInstanceOf(object, _this.class_ref))\
            JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Cannot access member field %1." #field " on object of type %2.").arg(qtjambi_class_name(env, _this.class_ref)).arg(qtjambi_object_class_name(env, object))) QTJAMBI_STACKTRACEINFO );\
        jshort result = env->GetShortField(object, _this.__##field);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return result;\
    }\
    public: static inline void set_##field(JNIEnv* env,jobject object, jshort value){\
        auto _this = __qt_get_this(env);\
        if(!_this.class_ref) return;\
        if(env->IsSameObject(object, nullptr))\
            JavaException::raiseNullPointerException(env, nullptr QTJAMBI_STACKTRACEINFO );\
        if(!env->IsInstanceOf(object, _this.class_ref))\
            JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Cannot access member field %1." #field " on object of type %2.").arg(qtjambi_class_name(env, _this.class_ref)).arg(qtjambi_object_class_name(env, object))) QTJAMBI_STACKTRACEINFO );\
        env->SetShortField(object, _this.__##field, value);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
    }

#define QTJAMBI_REPOSITORY_DECLARE_INT_FIELD(field)\
    private: jfieldID __##field;\
    public: static inline jint field(JNIEnv* env,jobject object){\
        auto _this = __qt_get_this(env);\
        if(!_this.class_ref) return 0;\
        if(env->IsSameObject(object, nullptr))\
            JavaException::raiseNullPointerException(env, nullptr QTJAMBI_STACKTRACEINFO );\
        if(!env->IsInstanceOf(object, _this.class_ref))\
            JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Cannot access member field %1." #field " on object of type %2.").arg(qtjambi_class_name(env, _this.class_ref)).arg(qtjambi_object_class_name(env, object))) QTJAMBI_STACKTRACEINFO );\
        jint result = env->GetIntField(object, _this.__##field);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return result;\
    }\
    public: static inline void set_##field(JNIEnv* env,jobject object, jint value){\
        auto _this = __qt_get_this(env);\
        if(!_this.class_ref) return;\
        if(env->IsSameObject(object, nullptr))\
            JavaException::raiseNullPointerException(env, nullptr QTJAMBI_STACKTRACEINFO );\
        if(!env->IsInstanceOf(object, _this.class_ref))\
            JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Cannot access member field %1." #field " on object of type %2.").arg(qtjambi_class_name(env, _this.class_ref)).arg(qtjambi_object_class_name(env, object))) QTJAMBI_STACKTRACEINFO );\
        env->SetIntField(object, _this.__##field, value);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
    }

#define QTJAMBI_REPOSITORY_DECLARE_LONG_FIELD(field)\
    private: jfieldID __##field;\
    public: static inline jlong field(JNIEnv* env,jobject object){\
        auto _this = __qt_get_this(env);\
        if(!_this.class_ref) return 0;\
        if(env->IsSameObject(object, nullptr))\
            JavaException::raiseNullPointerException(env, nullptr QTJAMBI_STACKTRACEINFO );\
        if(!env->IsInstanceOf(object, _this.class_ref))\
            JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Cannot access member field %1." #field " on object of type %2.").arg(qtjambi_class_name(env, _this.class_ref)).arg(qtjambi_object_class_name(env, object))) QTJAMBI_STACKTRACEINFO );\
        jlong result = env->GetLongField(object, _this.__##field);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return result;\
    }\
    public: static inline void set_##field(JNIEnv* env,jobject object, jlong value){\
        auto _this = __qt_get_this(env);\
        if(!_this.class_ref) return;\
        if(env->IsSameObject(object, nullptr))\
            JavaException::raiseNullPointerException(env, nullptr QTJAMBI_STACKTRACEINFO );\
        if(!env->IsInstanceOf(object, _this.class_ref))\
            JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Cannot access member field %1." #field " on object of type %2.").arg(qtjambi_class_name(env, _this.class_ref)).arg(qtjambi_object_class_name(env, object))) QTJAMBI_STACKTRACEINFO );\
        env->SetLongField(object, _this.__##field, value);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
    }

#define QTJAMBI_REPOSITORY_DECLARE_FLOAT_FIELD(field)\
    private: jfieldID __##field;\
    public: static inline jfloat field(JNIEnv* env,jobject object){\
        auto _this = __qt_get_this(env);\
        if(!_this.class_ref) return 0;\
        if(env->IsSameObject(object, nullptr))\
            JavaException::raiseNullPointerException(env, nullptr QTJAMBI_STACKTRACEINFO );\
        if(!env->IsInstanceOf(object, _this.class_ref))\
            JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Cannot access member field %1." #field " on object of type %2.").arg(qtjambi_class_name(env, _this.class_ref)).arg(qtjambi_object_class_name(env, object))) QTJAMBI_STACKTRACEINFO );\
        jfloat result = env->GetFloatField(object, _this.__##field);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return result;\
    }\
    public: static inline void set_##field(JNIEnv* env,jobject object, jfloat value){\
        auto _this = __qt_get_this(env);\
        if(!_this.class_ref) return;\
        if(env->IsSameObject(object, nullptr))\
            JavaException::raiseNullPointerException(env, nullptr QTJAMBI_STACKTRACEINFO );\
        if(!env->IsInstanceOf(object, _this.class_ref))\
            JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Cannot access member field %1." #field " on object of type %2.").arg(qtjambi_class_name(env, _this.class_ref)).arg(qtjambi_object_class_name(env, object))) QTJAMBI_STACKTRACEINFO );\
        env->SetFloatField(object, _this.__##field, value);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
    }

#define QTJAMBI_REPOSITORY_DECLARE_DOUBLE_FIELD(field)\
    private: jfieldID __##field;\
    public: static inline jdouble field(JNIEnv* env,jobject object){\
        auto _this = __qt_get_this(env);\
        if(!_this.class_ref) return 0;\
        if(env->IsSameObject(object, nullptr))\
            JavaException::raiseNullPointerException(env, nullptr QTJAMBI_STACKTRACEINFO );\
        if(!env->IsInstanceOf(object, _this.class_ref))\
            JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Cannot access member field %1." #field " on object of type %2.").arg(qtjambi_class_name(env, _this.class_ref)).arg(qtjambi_object_class_name(env, object))) QTJAMBI_STACKTRACEINFO );\
        jdouble result = env->GetDoubleField(object, _this.__##field);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return result;\
    }\
    public: static inline void set_##field(JNIEnv* env,jobject object, jdouble value){\
        auto _this = __qt_get_this(env);\
        if(!_this.class_ref) return;\
        if(env->IsSameObject(object, nullptr))\
            JavaException::raiseNullPointerException(env, nullptr QTJAMBI_STACKTRACEINFO );\
        if(!env->IsInstanceOf(object, _this.class_ref))\
            JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Cannot access member field %1." #field " on object of type %2.").arg(qtjambi_class_name(env, _this.class_ref)).arg(qtjambi_object_class_name(env, object))) QTJAMBI_STACKTRACEINFO );\
        env->SetDoubleField(object, _this.__##field, value);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
    }

#define QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_FIELD(field)\
    private: jfieldID __##field;\
    public: static inline jboolean field(JNIEnv* env,jobject object){\
        auto _this = __qt_get_this(env);\
        if(!_this.class_ref) return false;\
        if(env->IsSameObject(object, nullptr))\
            JavaException::raiseNullPointerException(env, nullptr QTJAMBI_STACKTRACEINFO );\
        if(!env->IsInstanceOf(object, _this.class_ref))\
            JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Cannot access member field %1." #field " on object of type %2.").arg(qtjambi_class_name(env, _this.class_ref)).arg(qtjambi_object_class_name(env, object))) QTJAMBI_STACKTRACEINFO );\
        jboolean result = env->GetBooleanField(object, _this.__##field);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return result;\
    }\
    public: static inline void set_##field(JNIEnv* env,jobject object, jboolean value){\
        auto _this = __qt_get_this(env);\
        if(!_this.class_ref) return;\
        if(env->IsSameObject(object, nullptr))\
            JavaException::raiseNullPointerException(env, nullptr QTJAMBI_STACKTRACEINFO );\
        if(!env->IsInstanceOf(object, _this.class_ref))\
            JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Cannot access member field %1." #field " on object of type %2.").arg(qtjambi_class_name(env, _this.class_ref)).arg(qtjambi_object_class_name(env, object))) QTJAMBI_STACKTRACEINFO );\
        env->SetBooleanField(object, _this.__##field, value);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
    }

#define QTJAMBI_REPOSITORY_DECLARE_CHAR_FIELD(field)\
    private: jfieldID __##field;\
    public: static inline jchar field(JNIEnv* env,jobject object){\
        auto _this = __qt_get_this(env);\
        if(!_this.class_ref) return 0;\
        if(env->IsSameObject(object, nullptr))\
            JavaException::raiseNullPointerException(env, nullptr QTJAMBI_STACKTRACEINFO );\
        if(!env->IsInstanceOf(object, _this.class_ref))\
            JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Cannot access member field %1." #field " on object of type %2.").arg(qtjambi_class_name(env, _this.class_ref)).arg(qtjambi_object_class_name(env, object))) QTJAMBI_STACKTRACEINFO );\
        jchar result = env->GetCharField(object, _this.__##field);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return result;\
    }\
    public: static inline void set_##field(JNIEnv* env,jobject object, jchar value){\
        auto _this = __qt_get_this(env);\
        if(!_this.class_ref) return;\
        if(env->IsSameObject(object, nullptr))\
            JavaException::raiseNullPointerException(env, nullptr QTJAMBI_STACKTRACEINFO );\
        if(!env->IsInstanceOf(object, _this.class_ref))\
            JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Cannot access member field %1." #field " on object of type %2.").arg(qtjambi_class_name(env, _this.class_ref)).arg(qtjambi_object_class_name(env, object))) QTJAMBI_STACKTRACEINFO );\
        env->SetCharField(object, _this.__##field, value);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
    }

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_FIELD(field)\
    private: jfieldID __##field;\
    public: static inline jobject field(JNIEnv* env){\
        auto _this = __qt_get_this(env);\
        if(!_this.class_ref) return nullptr;\
        jobject result = env->GetStaticObjectField(_this.class_ref, _this.__##field);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return result;\
    }\
    public: static inline void set_##field(JNIEnv* env, jobject value){\
        auto _this = __qt_get_this(env);\
        if(!_this.class_ref) return;\
        env->SetStaticObjectField(_this.class_ref, _this.__##field, value);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
    }

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_BYTE_FIELD(field)\
    private: jfieldID __##field;\
    public: static inline jbyte field(JNIEnv* env){\
        auto _this = __qt_get_this(env);\
        if(!_this.class_ref) return 0;\
        jbyte result = env->GetStaticByteField(_this.class_ref, _this.__##field);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return result;\
    }\
    public: static inline void set_##field(JNIEnv* env, jbyte value){\
        auto _this = __qt_get_this(env);\
        if(!_this.class_ref) return;\
        env->SetStaticByteField(_this.class_ref, _this.__##field, value);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
    }

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_SHORT_FIELD(field)\
    private: jfieldID __##field;\
    public: static inline jshort field(JNIEnv* env){\
        auto _this = __qt_get_this(env);\
        if(!_this.class_ref) return 0;\
        jshort result = env->GetStaticShortField(_this.class_ref, _this.__##field);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return result;\
    }\
    public: static inline void set_##field(JNIEnv* env, jshort value){\
        auto _this = __qt_get_this(env);\
        if(!_this.class_ref) return;\
        env->SetStaticShortField(_this.class_ref, _this.__##field, value);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
    }

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_INT_FIELD(field)\
    private: jfieldID __##field;\
    public: static inline jint field(JNIEnv* env){\
        auto _this = __qt_get_this(env);\
        if(!_this.class_ref) return 0;\
        jint result = env->GetStaticIntField(_this.class_ref, _this.__##field);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return result;\
    }\
    public: static inline void set_##field(JNIEnv* env, jint value){\
        auto _this = __qt_get_this(env);\
        if(!_this.class_ref) return;\
        env->SetStaticIntField(_this.class_ref, _this.__##field, value);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
    }

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_LONG_FIELD(field)\
    private: jfieldID __##field;\
    public: static inline jlong field(JNIEnv* env){\
        auto _this = __qt_get_this(env);\
        if(!_this.class_ref) return 0;\
        jlong result = env->GetStaticLongField(_this.class_ref, _this.__##field);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return result;\
    }\
    public: static inline void set_##field(JNIEnv* env, jlong value){\
        auto _this = __qt_get_this(env);\
        if(!_this.class_ref) return;\
        env->SetStaticLongField(_this.class_ref, _this.__##field, value);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
    }

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_FLOAT_FIELD(field)\
    private: jfieldID __##field;\
    public: static inline jfloat field(JNIEnv* env){\
        auto _this = __qt_get_this(env);\
        if(!_this.class_ref) return 0;\
        jfloat result = env->GetStaticFloatField(_this.class_ref, _this.__##field);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return result;\
    }\
    public: static inline void set_##field(JNIEnv* env, jfloat value){\
        auto _this = __qt_get_this(env);\
        if(!_this.class_ref) return;\
        env->SetStaticFloatField(_this.class_ref, _this.__##field, value);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
    }

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_DOUBLE_FIELD(field)\
    private: jfieldID __##field;\
    public: static inline jdouble field(JNIEnv* env){\
        auto _this = __qt_get_this(env);\
        if(!_this.class_ref) return 0;\
        jdouble result = env->GetStaticDoubleField(_this.class_ref, _this.__##field);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return result;\
    }\
    public: static inline void set_##field(JNIEnv* env, jdouble value){\
        auto _this = __qt_get_this(env);\
        if(!_this.class_ref) return;\
        env->SetStaticDoubleField(_this.class_ref, _this.__##field, value);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
    }

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_BOOLEAN_FIELD(field)\
    private: jfieldID __##field;\
    public: static inline jboolean field(JNIEnv* env){\
        auto _this = __qt_get_this(env);\
        if(!_this.class_ref) return false;\
        jboolean result = env->GetStaticBooleanField(_this.class_ref, _this.__##field);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return result;\
    }\
    public: static inline void set_##field(JNIEnv* env, jboolean value){\
        auto _this = __qt_get_this(env);\
        if(!_this.class_ref) return;\
        env->SetStaticBooleanField(_this.class_ref, _this.__##field, value);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
    }

#define QTJAMBI_REPOSITORY_DECLARE_STATIC_CHAR_FIELD(field)\
    private: jfieldID __##field;\
    public: static inline jchar field(JNIEnv* env){\
        auto _this = __qt_get_this(env);\
        if(!_this.class_ref) return 0;\
        jchar result = env->GetStaticCharField(_this.class_ref, _this.__##field);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
        return result;\
    }\
    public: static inline void set_##field(JNIEnv* env, jchar value){\
        auto _this = __qt_get_this(env);\
        if(!_this.class_ref) return;\
        env->SetStaticCharField(_this.class_ref, _this.__##field, value);\
        qtjambi_throw_java_exception_no_stackinfo(env)\
    }

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
    QRecursiveMutexLocker locker(gMutex()); \
    Q_UNUSED(locker)\
    this->class_ref = resolveClass(env, #package"/"#type_name);\
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
    qtjambi_throw_java_exception_no_stackinfo(env)\
    return result;\
}\
jboolean type_name::isAssignableFrom(JNIEnv* env,jclass otherClass){\
    auto _this = __qt_get_this(env);\
    Q_ASSERT(_this.class_ref);\
    jboolean result = env->IsAssignableFrom(otherClass, _this.class_ref);\
    qtjambi_throw_java_exception_no_stackinfo(env)\
    return result;\
}\
jboolean type_name::isSameClass(JNIEnv* env,jclass otherClass){\
    auto _this = __qt_get_this(env);\
    Q_ASSERT(_this.class_ref);\
    jboolean result = env->IsSameObject(otherClass, _this.class_ref);\
    qtjambi_throw_java_exception_no_stackinfo(env)\
    return result;\
}\
jobjectArray type_name::newArray(JNIEnv* env,jsize size){\
    auto _this = __qt_get_this(env);\
    Q_ASSERT(_this.class_ref);\
    jobjectArray result = env->NewObjectArray(size, _this.class_ref, nullptr);\
    qtjambi_throw_java_exception_no_stackinfo(env)\
    return result;\
}

#define QTJAMBI_REPOSITORY_DEFINE_FIELD(field_name, signature)\
    this->__##field_name = env->GetFieldID(this->class_ref, #field_name, #signature);\
    qtjambi_throw_java_exception_no_stackinfo(env)\
    Q_ASSERT(this->__##field_name);

#define QTJAMBI_REPOSITORY_DEFINE_LONG_FIELD(field_name)\
    this->__##field_name = env->GetFieldID(this->class_ref, #field_name, "J");\
    qtjambi_throw_java_exception_no_stackinfo(env)\
    Q_ASSERT(this->__##field_name);

#define QTJAMBI_REPOSITORY_DEFINE_STATIC_FIELD(field_name, signature)\
    this->__##field_name = env->GetStaticFieldID(this->class_ref, #field_name, #signature);\
    qtjambi_throw_java_exception_no_stackinfo(env)\
    Q_ASSERT(this->__##field_name);

#define QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(signature)\
    this->__constructor = env->GetMethodID(this->class_ref, "<init>", "("#signature")V");\
    qtjambi_throw_java_exception_no_stackinfo(env)\
    Q_ASSERT(this->__constructor);

#define QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR2(signature)\
    this->__constructor2 = env->GetMethodID(this->class_ref, "<init>", "("#signature")V");\
    qtjambi_throw_java_exception_no_stackinfo(env)\
    Q_ASSERT(this->__constructor2);

#define QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR3(signature)\
    this->__constructor3 = env->GetMethodID(this->class_ref, "<init>", "("#signature")V");\
    qtjambi_throw_java_exception_no_stackinfo(env)\
    Q_ASSERT(this->__constructor3);

#define QTJAMBI_REPOSITORY_DEFINE_RENAMED_METHOD(target, method_name, signature)\
    this->__##target = env->GetMethodID(this->class_ref, #method_name, #signature);\
    qtjambi_throw_java_exception_no_stackinfo(env)\
    Q_ASSERT(this->__##target);

#define QTJAMBI_REPOSITORY_DEFINE_METHOD(method_name, signature)\
    this->__##method_name = env->GetMethodID(this->class_ref, #method_name, #signature);\
    qtjambi_throw_java_exception_no_stackinfo(env)\
    Q_ASSERT(this->__##method_name);

#define QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(method_name, signature)\
    this->__##method_name = env->GetStaticMethodID(this->class_ref, #method_name, #signature);\
    qtjambi_throw_java_exception_no_stackinfo(env)\
    Q_ASSERT(this->__##method_name);

#define QTJAMBI_REPOSITORY_DEFINE_RENAMED_STATIC_METHOD(target, method_name, signature)\
    this->__##target = env->GetStaticMethodID(this->class_ref, #method_name, #signature);\
    qtjambi_throw_java_exception_no_stackinfo(env)\
    Q_ASSERT(this->__##target);

#define QTJAMBI_REPOSITORY_DEFINE_STANDARD_CONSTRUCTOR()\
    this->__constructor = env->GetMethodID(this->class_ref, "<init>", "()V");\
    qtjambi_throw_java_exception_no_stackinfo(env)\
    Q_ASSERT(this->__constructor);

#define QTJAMBI_REPOSITORY_DEFINE_EMPTY_CLASS(package, type_name)\
    QTJAMBI_REPOSITORY_DEFINE_CLASS(package, type_name,)

#define QTJAMBI_REPOSITORY_DEFINE_CLASS_SC(package, type_name)\
    QTJAMBI_REPOSITORY_DEFINE_CLASS(package, type_name, QTJAMBI_REPOSITORY_DEFINE_STANDARD_CONSTRUCTOR())

#endif // QTJAMBI_REPODEFINES_H
