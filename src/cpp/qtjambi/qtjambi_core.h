/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2022 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#if defined __cplusplus

#ifndef QTJAMBI_CORE_H
#define QTJAMBI_CORE_H

// TODO: this doesn't seem to be case... 
// someone should find out is this really the case when QTDIR is set
#ifdef QT3_SUPPORT
#  error "Qt must be configured without Qt 3 support in order to build Qt Jambi"
#endif

#include "qtjambi_global.h"
#include "qtjambi_utils.h"

#include <QtCore/QString>
#include <QtCore/QSharedDataPointer>
#include <QtCore/QException>
#include <QtCore/QMetaEnum>
#include <QtCore/QMetaMethod>
#include <QtCore/QMetaProperty>
#include <functional>
#include <typeinfo>

QT_WARNING_DISABLE_CLANG("-Wshift-count-overflow")

QTJAMBI_EXPORT void qtjambi_assert(const char *assertion, const char *file, int line);

#ifdef QT_NO_DEBUG
#  define QTJAMBI_EXCEPTION_CHECK(env) Q_UNUSED(env)
#  define QTJAMBI_EXCEPTION_CHECK_CLEAR(env) Q_UNUSED(env)
#  define QTJAMBI_DEBUG_TRACE(location)
#  define QTJAMBI_DEBUG_METHOD_PRINT(context, methodname)
#  define QTJAMBI_DEBUG_METHOD_PRINT_WHERE(context, methodname, shell)
#if defined(Q_ASSERT) && defined(QT_FORCE_ASSERTS)
#  undef Q_ASSERT
#  define Q_ASSERT(cond) ((cond) ? static_cast<void>(0) : qtjambi_assert(#cond, __FILE__, __LINE__))
#endif
#else
#  define QTJAMBI_EXCEPTION_CHECK(env) \
      if (env->ExceptionCheck()) { \
          printf("QtJambi: exception pending at %s, %d\n", __FILE__, __LINE__); \
          env->ExceptionDescribe(); \
      }
#  define QTJAMBI_EXCEPTION_CHECK_CLEAR(env) \
      if (env->ExceptionCheck()) { \
          printf("QtJambi: exception pending at %s, %d (will clear now)\n", __FILE__, __LINE__); \
          env->ExceptionDescribe(); \
          env->ExceptionClear(); \
      }
QTJAMBI_EXPORT void qtjambi_debug_trace(const char *location, const char *file, int line);
#  define QTJAMBI_DEBUG_TRACE(location) qtjambi_debug_trace(location, __FILE__, __LINE__);

#if defined(Q_ASSERT)
#  undef Q_ASSERT
#  if defined(QT_NO_DEBUG) && !defined(QT_FORCE_ASSERTS)
#    define Q_ASSERT(cond) static_cast<void>(false && (cond))
#  else
#    define Q_ASSERT(cond) ((cond) ? static_cast<void>(0) : qtjambi_assert(#cond, __FILE__, __LINE__))
#  endif
#endif

class QTJAMBI_EXPORT QtJambiDebugMethodPrint {
public:
    QtJambiDebugMethodPrint(const char* enterMessage, const char* leaveMethod, const char* file, int line);
    ~QtJambiDebugMethodPrint();
private:
    const char* m_enterMessage;
    const char* m_leaveMethod;
    const char* m_file;
    int m_line;
};

#define QTJAMBI_DEBUG_METHOD_PRINT(context, methodname)\
QtJambiDebugMethodPrint __qt_qtjambi_debug_method_print("(" context ") entering: " methodname, "(" context ") leaving:  " methodname, __FILE__, __LINE__);\
Q_UNUSED(__qt_qtjambi_debug_method_print)

class QtJambiShell;

#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME)
class QTJAMBI_EXPORT QtJambiDebugMethodPrintWithType {
public:
    QtJambiDebugMethodPrintWithType(const char* enterMessage, const char* leaveMethod, const QtJambiShell* shell, const char* file, int line);
    ~QtJambiDebugMethodPrintWithType();
private:
    const char* m_enterMessage;
    const char* m_leaveMethod;
    const char* m_type;
    const char* m_file;
    int m_line;
};

#define QTJAMBI_DEBUG_METHOD_PRINT_WHERE(context, methodname, shell)\
QtJambiDebugMethodPrintWithType __qt_qtjambi_debug_method_print("(" context ") entering: " methodname, "(" context ") leaving: " methodname, shell, __FILE__, __LINE__);\
Q_UNUSED(__qt_qtjambi_debug_method_print)

#else
#define QTJAMBI_DEBUG_METHOD_PRINT_WHERE(context, methodname, shell) QTJAMBI_DEBUG_METHOD_PRINT(context, methodname)
#endif

#endif

#ifndef QT_JAMBI_RUN
// during qtjambi generator execution qreal is typedef'ed to
// qtjambireal. this avoids platform dependent typedefs to
// float (arm) or double (x86).
// here during compilation, the wrapper's qtjambireal type is mapped back to qreal.
typedef qreal qtjambireal;
#endif

class QVariant;
class QRect;
class QtJambiFunctionTable;

class QTJAMBI_EXPORT QtJambiShell{
public:
    virtual void destructed(const std::type_info& typeId) = 0;
    virtual void constructed(const std::type_info& typeId) = 0;
    virtual void deleteShell() = 0;
    const QMetaObject* metaObject() const;
    void warnForMethod(const char*) const;
    void warnForMethod(const char*, const QObject* object) const;
    jobject getJavaObjectLocalRef(JNIEnv *env) const;
    jclass javaClass() const;
    jmethodID javaMethod(const std::type_info& typeId, int pos) const;
    void * qt_metacast(const char *className, bool* ok = nullptr);
    const void * qt_metacast(const char *className, bool* ok = nullptr) const;
    int qt_metacall(QMetaObject::Call _c, int _id, void **_a);
private:
    QtJambiShell();
    virtual ~QtJambiShell();
    Q_DISABLE_COPY_MOVE(QtJambiShell)
    friend class QtJambiShellImpl;
};

class FunctorBasePrivate;

class QTJAMBI_EXPORT QtJambiShellInterface{
public:
    static jobject getJavaObjectLocalRef(JNIEnv *env, const QtJambiShellInterface* shellInterface);
protected:
    virtual ~QtJambiShellInterface();
private:
    virtual QtJambiShell* __shell() const = 0;
    friend class FunctorBase;
    friend class FunctorBasePrivate;
};

class QTJAMBI_EXPORT FunctionalBase : public QtJambiShellInterface {
public:
    FunctionalBase();
    virtual ~FunctionalBase() override;
    virtual void getFunctional(JNIEnv *,void*) = 0;
private:
    Q_DISABLE_COPY_MOVE(FunctionalBase)
    QAtomicInt m_ref;
    friend class FunctorBase;
    friend class FunctorBasePrivate;
};

class QTJAMBI_EXPORT FunctorBase{
public:
    FunctorBase(const FunctorBase& functor);
    virtual ~FunctorBase();
    FunctorBase& operator=(const FunctorBase& base) = delete;
protected:
    FunctorBase(const std::type_info& typeId, FunctionalBase& functional);
    jmethodID javaMethod() const;
    jobject getJavaObjectLocalRef(JNIEnv *env) const;
    QtJambiShell* shell() const;
private:
    QSharedDataPointer<FunctorBasePrivate> d;
};

#ifdef QT_DEBUG
#ifndef QTJAMBI_STACKTRACE
#define QTJAMBI_STACKTRACE
#endif
#endif

#ifdef QTJAMBI_STACKTRACE
#define QTJAMBI_STACKTRACEINFO , __FUNCTION__, __FILE__, __LINE__
#define QTJAMBI_STACKTRACEINFO_ENV(env) env QTJAMBI_STACKTRACEINFO
#define QTJAMBI_STACKTRACEINFO_DECL , const char *methodName, const char *fileName, int lineNumber
#else
#define QTJAMBI_STACKTRACEINFO
#define QTJAMBI_STACKTRACEINFO_ENV(env)
#define QTJAMBI_STACKTRACEINFO_DECL
#endif

class JavaExceptionPrivate;

class QTJAMBI_EXPORT JavaException : public QException
{
public:
    JavaException() Q_DECL_NOEXCEPT;
    JavaException(JNIEnv *env, jthrowable obj);
    JavaException(const JavaException& copy) Q_DECL_NOEXCEPT;
    JavaException(JavaException&& copy) Q_DECL_NOEXCEPT;
    ~JavaException() Q_DECL_NOEXCEPT override;
    void raiseInJava(JNIEnv* env) const;
    void raise() const override;
    void report(JNIEnv* env) const;
    JavaException* clone() const override;
#ifdef QTJAMBI_STACKTRACE
    void raise(JNIEnv* env, const char *methodName, const char *fileName, int lineNumber) const;
#endif
    void addSuppressed(JNIEnv* env, const JavaException& exn) const;
    jthrowable object() const;
    JavaException& operator =(const JavaException& other) Q_DECL_NOEXCEPT;
    JavaException& operator =(JavaException&& other) Q_DECL_NOEXCEPT;
    operator bool() const Q_DECL_NOEXCEPT;
    char const* what() const Q_DECL_NOEXCEPT override;

#ifdef QTJAMBI_STACKTRACE
    static void check(JNIEnv* env);
#endif
    static void check(JNIEnv* env QTJAMBI_STACKTRACEINFO_DECL );
    static void raiseQNoNativeResourcesException(JNIEnv* env, const char *message QTJAMBI_STACKTRACEINFO_DECL );
    static void raiseNumberFormatException(JNIEnv* env, const char *message QTJAMBI_STACKTRACEINFO_DECL );
    static void raiseIllegalAccessException(JNIEnv* env, const char *message QTJAMBI_STACKTRACEINFO_DECL );
    static void raiseIllegalArgumentException(JNIEnv* env, const char *message QTJAMBI_STACKTRACEINFO_DECL );
    static void raiseIllegalStateException(JNIEnv* env, const char *message QTJAMBI_STACKTRACEINFO_DECL );
    static void raiseNullPointerException(JNIEnv* env, const char *message QTJAMBI_STACKTRACEINFO_DECL );
    static void raiseIndexOutOfBoundsException(JNIEnv* env, const char *message QTJAMBI_STACKTRACEINFO_DECL );
    static void raiseQNonVirtualOverridingException(JNIEnv* env, const char *message QTJAMBI_STACKTRACEINFO_DECL );
    static void raiseQNoImplementationException(JNIEnv* env, const char *message QTJAMBI_STACKTRACEINFO_DECL );
    static void raiseIllegalAccessError(JNIEnv* env, const char *message QTJAMBI_STACKTRACEINFO_DECL );
    static void raiseError(JNIEnv* env, const char *message QTJAMBI_STACKTRACEINFO_DECL );
    static void raiseRuntimeException(JNIEnv* env, const char *message QTJAMBI_STACKTRACEINFO_DECL );
    static void raiseUnsupportedOperationException(JNIEnv* env, const char *message QTJAMBI_STACKTRACEINFO_DECL );
    static void raiseQThreadAffinityException(JNIEnv* env, const char *message QTJAMBI_STACKTRACEINFO_DECL , jobject t1, QThread* t2, QThread* t3);
private:
    void update(JNIEnv *env);
    QSharedDataPointer<JavaExceptionPrivate> p;
};

#if defined(QTJAMBI_CENTRAL_TRY_CATCH)

class TypedTrial{
public:
    template<typename Functor>
    TypedTrial(Functor&& fun)
        : m_functor(&fun),
          m_caller(&caller<Functor>){}
    void operator()();
private:
    template<typename Functor>
    static void caller(void* ptr){(*reinterpret_cast<Functor*>(ptr))();}
    void* m_functor;
    void(*m_caller)(void*);
};

class TypedCatcher{
public:
    template<typename Functor>
    TypedCatcher(Functor&& fun)
        : m_functor(&fun),
          m_caller(&caller<Functor>){}
    void operator()(const JavaException& exn);
private:
    template<typename Functor>
    static void caller(void* ptr, const JavaException& exn){(*reinterpret_cast<Functor*>(ptr))(exn);}
    void* m_functor;
    void(*m_caller)(void*,const JavaException&);
};

QTJAMBI_EXPORT void qtjambi_try_catch(TypedTrial&& fct, TypedCatcher&& handler);
QTJAMBI_EXPORT void qtjambi_try_catch_any(TypedTrial&& fct, TypedTrial&& anyHandler);

#define QTJAMBI_TRY qtjambi_try_catch([&]()->void
#define QTJAMBI_TRY_ANY qtjambi_try_catch_any([&]()->void
#define QTJAMBI_TRY_RETURN(variable,value) variable = value;\
                                           return;
#define QTJAMBI_CATCH(EXN) , [&](EXN)->void
#define QTJAMBI_CATCH_ANY , [&]()->void
#define QTJAMBI_TRY_END );
#else
#define QTJAMBI_TRY try
#define QTJAMBI_TRY_ANY try
#define QTJAMBI_TRY_RETURN(variable,value) return value;
#define QTJAMBI_CATCH catch
#define QTJAMBI_CATCH_ANY catch(...)
#define QTJAMBI_TRY_END
#endif

#define qtjambi_throw_java_exception(env)\
    JavaException::check(env QTJAMBI_STACKTRACEINFO )

class QTJAMBI_EXPORT QtJambiExceptionHandler{
public:
    QtJambiExceptionHandler();
    ~QtJambiExceptionHandler();
    void handle(JNIEnv *env, const JavaException& exn, const char* methodName);
private:
    quint8 data;
    Q_DISABLE_COPY_MOVE(QtJambiExceptionHandler)
};

class QTJAMBI_EXPORT QtJambiExceptionInhibitor{
public:
    QtJambiExceptionInhibitor();
    ~QtJambiExceptionInhibitor();
    void handle(JNIEnv *env, const JavaException& exn, const char* methodName);
private:
    quint8 data;
    Q_DISABLE_COPY_MOVE(QtJambiExceptionInhibitor)
};

class QTJAMBI_EXPORT QtJambiExceptionBlocker{
public:
    QtJambiExceptionBlocker();
    ~QtJambiExceptionBlocker();
    void release(JNIEnv *env);
private:
    quint8 data;
    Q_DISABLE_COPY_MOVE(QtJambiExceptionBlocker)
};

class QTJAMBI_EXPORT QtJambiExceptionRaiser{
public:
    QtJambiExceptionRaiser();
    ~QtJambiExceptionRaiser();
    void raise(JNIEnv *env);
private:
    quint8 data;
    Q_DISABLE_COPY_MOVE(QtJambiExceptionRaiser)
};

QTJAMBI_EXPORT QByteArray qtjambi_type_name(const std::type_info& typeId);

QTJAMBI_EXPORT bool qtjambi_convert_to_native(JNIEnv *env, const std::type_info& typeId, jobject java_object, void * output);

QTJAMBI_EXPORT bool qtjambi_convert_to_native(JNIEnv *env, const std::type_info& typeId, const char* qtName, const char* javaName, jobject java_object, void * output);

QTJAMBI_EXPORT bool qtjambi_convert_from_native(JNIEnv *env, const std::type_info& typeId, const char* qtName, const char* javaName, const void *qt_object, bool makeCopyOfValueTypes, bool cppOwnership, jobject& output);

QTJAMBI_EXPORT bool qtjambi_convert_to_native(JNIEnv *env, const std::type_info& typeId, const char* typeName, jobject java_object, void * output);

QTJAMBI_EXPORT bool qtjambi_convert_from_native(JNIEnv *env, const std::type_info& typeId, const char* typeName, const void *qt_object, bool makeCopyOfValueTypes, bool cppOwnership, jobject& output);

QTJAMBI_EXPORT jobject qtjambi_java_object_reference(QtJambiNativeID nativeId, JNIEnv* env);

QTJAMBI_EXPORT bool qtjambi_is_shell(QtJambiNativeID nativeId);

QTJAMBI_EXPORT bool qtjambi_object_is_shell(JNIEnv *env, jobject object);

QTJAMBI_EXPORT bool qtjambi_interface_is_shell(JNIEnv *env, jobject object);

QTJAMBI_EXPORT QtJambiNativeID qtjambi_get_object_native_id(JNIEnv *env, jobject object);

QTJAMBI_EXPORT QtJambiNativeID qtjambi_get_interface_native_id(JNIEnv *env, jobject object);

QTJAMBI_EXPORT void *qtjambi_from_nativeId(QtJambiNativeID nativeId);

QTJAMBI_EXPORT void *qtjambi_from_nativeId(QtJambiNativeID nativeId, const std::type_info& typeId);

QTJAMBI_EXPORT void qtjambi_invalidate_object(JNIEnv *env, QtJambiNativeID nativeId);

QTJAMBI_EXPORT jclass qtjambi_lambda_return_type(JNIEnv *env, jobject lambdaExpression);

QTJAMBI_EXPORT void qtjambi_register_dependent_interface(JNIEnv *env, jobject dependentObject, jobject owner);

QTJAMBI_EXPORT void qtjambi_register_dependent_object(JNIEnv *env, jobject dependentObject, jobject owner);

QTJAMBI_EXPORT void qtjambi_unregister_dependent_interface(JNIEnv *env, jobject dependentObject, jobject owner);

QTJAMBI_EXPORT void qtjambi_unregister_dependent_object(JNIEnv *env, jobject dependentObject, jobject owner);

QTJAMBI_EXPORT void qtjambi_register_dependent_interface(JNIEnv *env, jobject dependentObject, QtJambiNativeID _this_nativeId);

QTJAMBI_EXPORT jobject qtjambi_get_internal_access(JNIEnv *env, jclass cls);

template<typename T>
T *qtjambi_object_from_nativeId(QtJambiNativeID nativeId)
{
    return reinterpret_cast<T*>( qtjambi_from_nativeId(nativeId));
}

template<typename T>
T *qtjambi_interface_from_nativeId(QtJambiNativeID nativeId)
{
    return reinterpret_cast<T*>( qtjambi_from_nativeId(nativeId, typeid(T)));
}

inline jintArray qtjambi_to_jintArray(JNIEnv *__jni_env, const jint* in, jsize length) {
    jintArray out = __jni_env->NewIntArray(length);
    qtjambi_throw_java_exception(__jni_env);
    __jni_env->SetIntArrayRegion(out, 0, length, in);
    qtjambi_throw_java_exception(__jni_env);
    return out;
}

inline jshortArray qtjambi_to_jshortArray(JNIEnv *__jni_env, const jshort* in, jsize length) {
    jshortArray out = __jni_env->NewShortArray(length);
    qtjambi_throw_java_exception(__jni_env);
    __jni_env->SetShortArrayRegion(out, 0, length, in);
    qtjambi_throw_java_exception(__jni_env);
    return out;
}

inline jbyteArray qtjambi_to_jbyteArray(JNIEnv *__jni_env, const jbyte* in, jsize length) {
    jbyteArray out = __jni_env->NewByteArray(length);
    qtjambi_throw_java_exception(__jni_env);
    __jni_env->SetByteArrayRegion(out, 0, length, in);
    qtjambi_throw_java_exception(__jni_env);
    return out;
}

inline jlongArray qtjambi_to_jlongArray(JNIEnv *__jni_env, const jlong* in, jsize length) {
    jlongArray out = __jni_env->NewLongArray(length);
    qtjambi_throw_java_exception(__jni_env);
    __jni_env->SetLongArrayRegion(out, 0, length, in);
    qtjambi_throw_java_exception(__jni_env);
    return out;
}

inline jfloatArray qtjambi_to_jfloatArray(JNIEnv *__jni_env, const jfloat* in, jsize length) {
    jfloatArray out = __jni_env->NewFloatArray(length);
    qtjambi_throw_java_exception(__jni_env);
    __jni_env->SetFloatArrayRegion(out, 0, length, in);
    qtjambi_throw_java_exception(__jni_env);
    return out;
}

inline jdoubleArray qtjambi_to_jdoubleArray(JNIEnv *__jni_env, const jdouble* in, jsize length) {
    jdoubleArray out = __jni_env->NewDoubleArray(length);
    qtjambi_throw_java_exception(__jni_env);
    __jni_env->SetDoubleArrayRegion(out, 0, length, in);
    qtjambi_throw_java_exception(__jni_env);
    return out;
}

inline jcharArray qtjambi_to_jcharArray(JNIEnv *__jni_env, const jchar* in, jsize length) {
    jcharArray out = __jni_env->NewCharArray(length);
    qtjambi_throw_java_exception(__jni_env);
    __jni_env->SetCharArrayRegion(out, 0, length, in);
    qtjambi_throw_java_exception(__jni_env);
    return out;
}

inline jbooleanArray qtjambi_to_jbooleanArray(JNIEnv *__jni_env, const jboolean* in, jsize length) {
    jbooleanArray out = __jni_env->NewBooleanArray(length);
    qtjambi_throw_java_exception(__jni_env);
    __jni_env->SetBooleanArrayRegion(out, 0, length, in);
    qtjambi_throw_java_exception(__jni_env);
    return out;
}

QTJAMBI_EXPORT jobjectArray qtjambi_create_array(JNIEnv *env, const char* componentClass, jsize size);
QTJAMBI_EXPORT jobjectArray qtjambi_create_array(JNIEnv *env, const std::type_info& componentType, jsize size);

template<typename T, typename E>
inline jobjectArray qtjambi_to_jobjectArray(JNIEnv *__jni_env, const char *className, const T& iterable, std::function<jobject(JNIEnv *,const E&)> convertFunction) {
    jsize length = jsize(iterable.size());
    jobjectArray out = qtjambi_create_array(__jni_env, className, length);
    for (jsize i = 0; i < length; ++i) {
        __jni_env->SetObjectArrayElement(out, i, convertFunction(__jni_env, iterable.begin()[i]));
        qtjambi_throw_java_exception(__jni_env);
    }
    return out;
}

QTJAMBI_EXPORT jobjectArray qtjambi_to_jobjectArray(JNIEnv *__jni_env, const std::type_info& typeInfo, const void* iterable, jsize length, std::function<jobject(JNIEnv *,const void*,jsize)> convertFunction);

template<template<typename E> class T, typename E>
inline jobjectArray qtjambi_to_jobjectArray(JNIEnv *__jni_env, const T<E>& iterable, jsize length, std::function<jobject(JNIEnv *,const T<E>&,jsize)> getFunction) {
    jobjectArray out = qtjambi_create_array(__jni_env, typeid(E), length);
    for (jsize i = 0; i < length; ++i) {
        __jni_env->SetObjectArrayElement(out, i, getFunction(__jni_env, iterable, i));
        qtjambi_throw_java_exception(__jni_env);
    }
    return out;
}

template<template<typename E> class T, typename E>
inline jobjectArray qtjambi_to_jobjectArray(JNIEnv *__jni_env, const T<E>& iterable, jsize (*lengthFunction)(const T<E>&), std::function<jobject(JNIEnv *,const T<E>&,jsize)> getFunction) {
    jsize length = lengthFunction(iterable);
    jobjectArray out = qtjambi_create_array(__jni_env, typeid(E), length);
    for (jsize i = 0; i < length; ++i) {
        __jni_env->SetObjectArrayElement(out, i, getFunction(__jni_env, iterable, i));
        qtjambi_throw_java_exception(__jni_env);
    }
    return out;
}

template<typename T, typename E>
inline jobjectArray qtjambi_to_jobjectArray(JNIEnv *__jni_env, const char *className, const T* iterable, std::function<jobject(JNIEnv *,const E&)> convertFunction) {
    jsize length = jsize(iterable->size());
    jobjectArray out = qtjambi_create_array(__jni_env, className, length);
    for (jsize i = 0; i < length; ++i) {
        __jni_env->SetObjectArrayElement(out, i, convertFunction(__jni_env, iterable->begin()[i]));
        qtjambi_throw_java_exception(__jni_env);
    }
    return out;
}

template<typename T>
inline jobjectArray qtjambi_to_jobjectArray(JNIEnv *__jni_env, const char *className, const T* array, jsize length, std::function<jobject(JNIEnv *,const T&)> convertFunction) {
    jobjectArray out = qtjambi_create_array(__jni_env, className, length);
    for (jsize i = 0; i < length; ++i) {
        __jni_env->SetObjectArrayElement(out, i, convertFunction(__jni_env, array[i]));
        qtjambi_throw_java_exception(__jni_env);
    }
    return out;
}

template<typename T>
inline jobjectArray qtjambi_to_jobjectArray(JNIEnv *__jni_env, const T* array, jsize length, std::function<jobject(JNIEnv *,const T&)> convertFunction) {
    jobjectArray out = qtjambi_create_array(__jni_env, typeid(T), length);
    for (jsize i = 0; i < length; ++i) {
        __jni_env->SetObjectArrayElement(out, i, convertFunction(__jni_env, array[i]));
        qtjambi_throw_java_exception(__jni_env);
    }
    return out;
}

QTJAMBI_EXPORT QVariant qtjambi_to_qvariant(JNIEnv *env, jobject java_object);

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
class QtJambiScope;
QTJAMBI_EXPORT void qtjambi_to_QModelRoleData(JNIEnv *env, QtJambiScope& scope, jobject java_object, void * &data, qsizetype &length);
#endif

QTJAMBI_EXPORT jobject qtjambi_from_qvariant(JNIEnv *env, const QVariant &qt_variant);

QTJAMBI_EXPORT void *qtjambi_to_object(JNIEnv *env, jobject java_object);

extern "C" QTJAMBI_EXPORT QObject *qtjambi_to_qobject(JNIEnv *env, jobject java_object);

QTJAMBI_EXPORT int qtjambi_to_enum(JNIEnv *env, jobject java_object);

QTJAMBI_EXPORT jstring qtjambi_to_jstring(JNIEnv *env, jobject object);

QTJAMBI_EXPORT QString qtjambi_to_qstring(JNIEnv *env, jobject object);

QTJAMBI_EXPORT QString qtjambi_to_qstring(JNIEnv *env, jstring java_string);

extern "C" QTJAMBI_EXPORT void qtjambi_to_qstring(QString& result, JNIEnv *env, jstring java_string);

template<template <typename> class Pointer, typename Instantiation>
inline void deletePointer(void* pointer,bool){
    Pointer<Instantiation>* _pointer = reinterpret_cast<Pointer<Instantiation>*>(pointer);
    delete _pointer;
}

template<template <typename> class Pointer, typename Instantiation>
inline void* createPointer(void* pointer){
    return new Pointer<Instantiation>(reinterpret_cast<Instantiation*>(pointer));
}

template<template <typename> class Pointer, typename Instantiation>
inline void* getFromPointer(const void* pointer){
    const Pointer<typename std::remove_const<Instantiation>::type>& _pointer = *reinterpret_cast<const Pointer<typename std::remove_const<Instantiation>::type>*>(pointer);
    return &*_pointer;
}

template<template <typename> class Pointer, typename Instantiation>
inline QObject* getQObjectFromPointer(const void* pointer){
    const Pointer<typename std::remove_const<Instantiation>::type>& _pointer = *reinterpret_cast<const Pointer<typename std::remove_const<Instantiation>::type>*>(pointer);
    return &*_pointer;
}

QTJAMBI_EXPORT void *qtjambi_to_interface(JNIEnv *env, jobject java_object,
                                  const char *interface_name, const std::type_info& typeId);

QTJAMBI_EXPORT void *qtjambi_to_interface(JNIEnv *env, jobject java_object,
                                  const std::type_info& typeId);

template<typename T>
T *qtjambi_to_QObject(JNIEnv *env, jobject java_object)
{
    return dynamic_cast<T*>(qtjambi_to_qobject(env, java_object));
}

template<typename T>
T *qtjambi_to_object(JNIEnv *env, jobject java_object)
{
    return reinterpret_cast<T*>(qtjambi_to_object(env, java_object));
}

template<typename T>
T *qtjambi_to_interface(JNIEnv *env, jobject java_object)
{
    return reinterpret_cast<T*>(qtjambi_to_interface(env, java_object, typeid(T)));
}

template<typename T>
T *qtjambi_to_object_deref(JNIEnv *env, jobject java_object)
{
    return checked_deref(env, reinterpret_cast<T*>(qtjambi_to_object(env, java_object)));
}

template<typename T>
T& qtjambi_to_interface_deref(JNIEnv *env, jobject java_object)
{
    return checked_deref(env, reinterpret_cast<T*>(qtjambi_to_interface(env, java_object, typeid(T))));
}

template<typename T>
T *qtjambi_to_interface(JNIEnv *env, jobject java_object, const char *interface_name)
{
    return reinterpret_cast<T*>(qtjambi_to_interface(env, java_object, interface_name, typeid(T)));
}

template<typename T>
T& qtjambi_to_interface_deref(JNIEnv *env, jobject java_object, const char *interface_name)
{
    return checked_deref(env, reinterpret_cast<T*>(qtjambi_to_interface(env, java_object, interface_name, typeid(T))));
}

QTJAMBI_EXPORT
jobject qtjambi_from_object(JNIEnv *env, const void *qt_object, const std::type_info& typeId, bool makeCopyOfValueTypes, bool invalidateAfterUse = false);

QTJAMBI_EXPORT
jobject qtjambi_from_object(JNIEnv *env, const void *qt_object, const char *className, const std::type_info& typeId, bool makeCopyOfValueTypes, bool invalidateAfterUse = false);

QTJAMBI_EXPORT
jobject qtjambi_from_object(JNIEnv *env, const void *qt_object, const char *className, bool makeCopyOfValueTypes, bool invalidateAfterUse = false);

QTJAMBI_EXPORT
jobject qtjambi_from_object(JNIEnv *env, const void *qt_object, jclass clazz, bool makeCopyOfValueTypes, bool invalidateAfterUse = false);

QTJAMBI_EXPORT
jobject qtjambi_to_stringobject(JNIEnv *env, const QString &strg);

QTJAMBI_EXPORT
jobject qtjambi_to_stringobject(JNIEnv *env, QString *strg);

QTJAMBI_EXPORT
jobject qtjambi_to_charobject(JNIEnv *env, const QChar &strg);

QTJAMBI_EXPORT
jobject qtjambi_to_charobject(JNIEnv *env, QChar *strg);

QTJAMBI_EXPORT
jobject qtjambi_from_functional(JNIEnv *env, const void *qt_object, const std::type_info& typeId);

template<typename T>
jobject qtjambi_from_object(JNIEnv *env, const T *qt_object, bool makeCopyOfValueTypes, bool invalidateAfterUse = false)
{
    return qtjambi_from_object(env, qt_object, typeid(T), makeCopyOfValueTypes, invalidateAfterUse);
}

template<typename T>
jobject qtjambi_from_object(JNIEnv *env, const T *qt_object, const char *className, bool makeCopyOfValueTypes, bool invalidateAfterUse = false)
{
    return qtjambi_from_object(env, qt_object, className, typeid(T), makeCopyOfValueTypes, invalidateAfterUse);
}

template<typename T>
jobject qtjambi_from_functional(JNIEnv *env, const void *qt_object)
{
    return qtjambi_from_functional(env, qt_object, typeid(T));
}

QTJAMBI_EXPORT
jobject qtjambi_from_functional(JNIEnv *env, const void *qt_object, const char *className, const std::type_info& typeId);

template<typename T>
jobject qtjambi_from_functional(JNIEnv *env, const void *qt_object, const char *className)
{
    return qtjambi_from_functional(env, qt_object, className, typeid(T));
}

QTJAMBI_EXPORT
jobject qtjambi_from_functional(JNIEnv *env, const void *qt_object, const char *className);

typedef void(* PointerDeleter)(void *, bool);
typedef void*(* PointerCreator)(void *);
typedef QObject*(*PointerQObjectGetter)(const void *);
typedef void*(*PointerGetter)(const void *);

QTJAMBI_EXPORT
jobject qtjambi_from_object_shared_pointer(JNIEnv *env, const std::type_info& typeId,
                                           void* ptr_shared_pointer, PointerDeleter sharedPointerDeleter, PointerGetter sharedPointerGetter);

QTJAMBI_EXPORT
jobject qtjambi_from_interface(JNIEnv *env, const void *qt_object, const std::type_info& typeId, bool makeCopyOfValueTypes, bool invalidateAfterUse = false);

QTJAMBI_EXPORT
jobject qtjambi_from_interface(JNIEnv *env, const void *qt_object, const char *className,
                            const std::type_info& typeId, bool makeCopyOfValueTypes, bool invalidateAfterUse = false);

template<typename T>
jobject qtjambi_from_interface(JNIEnv *env, const T *qt_object, bool makeCopyOfValueTypes, bool invalidateAfterUse = false)
{
    return qtjambi_from_interface(env, qt_object, typeid(T), makeCopyOfValueTypes, invalidateAfterUse);
}

template<typename T>
jobject qtjambi_from_interface(JNIEnv *env, const T *qt_object, const char *className,
                               bool makeCopyOfValueTypes, bool invalidateAfterUse = false)
{
    return qtjambi_from_interface(env, qt_object, className, typeid(T), makeCopyOfValueTypes, invalidateAfterUse);
}

extern "C"
QTJAMBI_EXPORT
jobject qtjambi_from_qobject(JNIEnv *env, const QObject *qt_object, const char *className);

QTJAMBI_EXPORT
jobject qtjambi_from_qobject(JNIEnv *env, const QObject *qt_object, jclass clazz);

QTJAMBI_EXPORT
jobject qtjambi_from_qobject(JNIEnv *env, const QObject *qt_object, const std::type_info& typeId);

template<typename O>
jobject qtjambi_from_qobject(JNIEnv *env, const O *qt_object)
{
    return qtjambi_from_qobject(env, qt_object, typeid(O));
}

extern "C"
QTJAMBI_EXPORT
jobject qtjambi_from_qobject_shared_pointer(JNIEnv *env, const char *className, void* ptr_shared_pointer, PointerDeleter shared_pointer_deleter, PointerGetter pointerGetter);

QTJAMBI_EXPORT
jobject qtjambi_from_qobject_shared_pointer(JNIEnv *env, const std::type_info& typeId, void* ptr_shared_pointer, PointerDeleter shared_pointer_deleter, PointerGetter pointerGetter);

extern "C" QTJAMBI_EXPORT jobject qtjambi_from_QObject(JNIEnv *env, const QObject *qt_object);

QTJAMBI_EXPORT jobject qtjambi_from_enum(JNIEnv *env, qint32 qt_enum, const char *className);

QTJAMBI_EXPORT jobject qtjambi_from_enum(JNIEnv *env, qint8 qt_enum, const char *className);

QTJAMBI_EXPORT jobject qtjambi_from_enum(JNIEnv *env, qint16 qt_enum, const char *className);

QTJAMBI_EXPORT jobject qtjambi_from_enum(JNIEnv *env, qint64 qt_enum, const char *className);

QTJAMBI_EXPORT jobject qtjambi_from_enum(JNIEnv *env, quint32 qt_enum, const char *className);

QTJAMBI_EXPORT jobject qtjambi_from_enum(JNIEnv *env, quint8 qt_enum, const char *className);

QTJAMBI_EXPORT jobject qtjambi_from_enum(JNIEnv *env, quint16 qt_enum, const char *className);

QTJAMBI_EXPORT jobject qtjambi_from_enum(JNIEnv *env, quint64 qt_enum, const char *className);

QTJAMBI_EXPORT jobject qtjambi_from_flags(JNIEnv *env, int qt_flags, const char *className);

QTJAMBI_EXPORT jobject qtjambi_from_enum(JNIEnv *env, qint32 qt_enum, jclass cls);

QTJAMBI_EXPORT jobject qtjambi_from_enum(JNIEnv *env, qint8 qt_enum, jclass cls);

QTJAMBI_EXPORT jobject qtjambi_from_enum(JNIEnv *env, qint16 qt_enum, jclass cls);

QTJAMBI_EXPORT jobject qtjambi_from_enum(JNIEnv *env, qint64 qt_enum, jclass cls);

QTJAMBI_EXPORT jobject qtjambi_from_enum(JNIEnv *env, quint32 qt_enum, jclass cls);

QTJAMBI_EXPORT jobject qtjambi_from_enum(JNIEnv *env, quint8 qt_enum, jclass cls);

QTJAMBI_EXPORT jobject qtjambi_from_enum(JNIEnv *env, quint16 qt_enum, jclass cls);

QTJAMBI_EXPORT jobject qtjambi_from_enum(JNIEnv *env, quint64 qt_enum, jclass cls);

QTJAMBI_EXPORT jobject qtjambi_from_flags(JNIEnv *env, int qt_flags, jclass cls);

template<typename E>
jobject qtjambi_from_enum(JNIEnv *env, E qt_enum)
{
    return qtjambi_from_object(env, &qt_enum, typeid(E), true, false);
}

template<typename F>
jobject qtjambi_from_flags(JNIEnv *env, const F& qt_flags)
{
    return qtjambi_from_object(env, &qt_flags, typeid(F), true, false);
}

// QtEnumerator<T> -> int
QTJAMBI_EXPORT QVariant qtjambi_to_enumerator(JNIEnv *env, jobject value);

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
QTJAMBI_EXPORT
jstring qtjambi_from_qstringref(JNIEnv *env, const QStringRef &s);
#else
QTJAMBI_EXPORT
jstring qtjambi_from_qanystringview(JNIEnv *env, QAnyStringView s);

QTJAMBI_EXPORT
jstring qtjambi_from_qutf8stringview(JNIEnv *env, QUtf8StringView s);
#endif

QTJAMBI_EXPORT
jstring qtjambi_from_qstringview(JNIEnv *env, QStringView s);

extern "C"
QTJAMBI_EXPORT
jstring qtjambi_from_qstring(JNIEnv *env, const QString &s);

QTJAMBI_EXPORT
void qtjambi_invalidate_object(JNIEnv *env, jobject java_object, bool checkJavaOwnership = true);

QTJAMBI_EXPORT
void qtjambi_invalidate_collection(JNIEnv *env, jobject java_collection, bool checkJavaOwnership = true);

QTJAMBI_EXPORT
void qtjambi_invalidate_array(JNIEnv *env, jobjectArray java_array, bool checkJavaOwnership = true);

QTJAMBI_EXPORT
void *qtjambi_to_cpointer(JNIEnv *env, jobject java_object, int indirections);

namespace QNativePointer{
    enum class Type{
        /** Java Boolean*/ Boolean = 0,
        /** Java Byte*/ Byte,
        /** Java Char*/ Char,
        /** Java Short*/ Short,
        /** Java Int*/ Int,
        /** Java Long*/ Long,
        /** Java Float*/ Float,
        /** Java Double*/ Double,
        /** Another QNativePointer of any type*/ Pointer,
        /** Java String*/ String
    };
}

QTJAMBI_EXPORT jobject qtjambi_from_cpointer(JNIEnv *env, const void *qt_pointer, QNativePointer::Type type_id, int indirections);

QTJAMBI_EXPORT
jobject qtjambi_array_to_nativepointer(JNIEnv *env, jobjectArray array, size_t elementSize);

QTJAMBI_EXPORT QThread *qtjambi_to_thread(JNIEnv *env, jobject thread);
QTJAMBI_EXPORT jobject qtjambi_from_thread(JNIEnv *env, QThread *thread);

QTJAMBI_EXPORT QModelIndex qtjambi_to_QModelIndex(JNIEnv *env, jobject index);
QTJAMBI_EXPORT jobject qtjambi_from_QModelIndex(JNIEnv *env, const QModelIndex &index);

QTJAMBI_EXPORT QString qtjambi_class_name(JNIEnv *env, jclass java_class);
QTJAMBI_EXPORT QString qtjambi_object_class_name(JNIEnv *env, jobject java_object);

// Boxing functions
QTJAMBI_EXPORT jobject qtjambi_from_int(JNIEnv *env, jint int_value);
QTJAMBI_EXPORT jint qtjambi_to_int(JNIEnv *env, jobject int_object);
QTJAMBI_EXPORT jobject qtjambi_from_double(JNIEnv *env, jdouble double_value);
QTJAMBI_EXPORT jdouble qtjambi_to_double(JNIEnv *env, jobject double_object);
QTJAMBI_EXPORT jobject qtjambi_from_boolean(JNIEnv *env, jboolean bool_value);
QTJAMBI_EXPORT bool qtjambi_to_boolean(JNIEnv *env, jobject bool_object);
QTJAMBI_EXPORT jlong qtjambi_to_long(JNIEnv *env, jobject long_object);
QTJAMBI_EXPORT jobject qtjambi_from_long(JNIEnv *env, jlong long_value);
QTJAMBI_EXPORT jobject qtjambi_from_short(JNIEnv *env, jshort short_value);
QTJAMBI_EXPORT jobject qtjambi_from_float(JNIEnv *env, jfloat float_value);
QTJAMBI_EXPORT jobject qtjambi_from_byte(JNIEnv *env, jbyte byte_value);
QTJAMBI_EXPORT jchar qtjambi_to_char(JNIEnv *env, jobject char_object);
QTJAMBI_EXPORT jobject qtjambi_from_char(JNIEnv *env, jchar char_value);
QTJAMBI_EXPORT jfloat qtjambi_to_float(JNIEnv *env, jobject float_object);
QTJAMBI_EXPORT jshort qtjambi_to_short(JNIEnv *env, jobject short_object);
QTJAMBI_EXPORT jchar qtjambi_to_jchar(JNIEnv *env, jobject char_object);
QTJAMBI_EXPORT jbyte qtjambi_to_byte(JNIEnv *env, jobject byte_object);
// Container helpers...
QTJAMBI_EXPORT jobject qtjambi_pair_new(JNIEnv *env, jobject first, jobject second);
QTJAMBI_EXPORT jobject qtjambi_pair_get(JNIEnv *env, jobject pair, int pos);
QTJAMBI_EXPORT jobject qtjambi_hashset_new(JNIEnv *env);
QTJAMBI_EXPORT jobject qtjambi_hashmap_new(JNIEnv *env, int size);
QTJAMBI_EXPORT jobject qtjambi_treemap_new(JNIEnv *env);

typedef bool (*LessThanFunction)(JNIEnv *env, jobject k1, jobject k2);
QTJAMBI_EXPORT jobject qtjambi_treemap_new(JNIEnv *env, LessThanFunction lessThanFct);
QTJAMBI_EXPORT jobject qtjambi_treeset_new(JNIEnv *env, LessThanFunction lessThanFct);

QTJAMBI_EXPORT void qtjambi_map_put(JNIEnv *env, jobject map, jobject key, jobject val);
QTJAMBI_EXPORT void qtjambi_multimap_put(JNIEnv *env, jobject map, jobject key, jobject val);
QTJAMBI_EXPORT int qtjambi_map_size(JNIEnv *env, jobject map);
QTJAMBI_EXPORT void qtjambi_map_clear(JNIEnv *env, jobject map);
QTJAMBI_EXPORT jobject qtjambi_map_entryset_iterator(JNIEnv *env, jobject map);
QTJAMBI_EXPORT jobjectArray qtjambi_map_entryset_array(JNIEnv *env, jobject map);

QT_WARNING_DISABLE_CLANG("-Wdollar-in-identifier-extension")
QTJAMBI_EXPORT jobject qtjambi_map$entry_key(JNIEnv *env, jobject entry);
QTJAMBI_EXPORT jobject qtjambi_map$entry_value(JNIEnv *env, jobject entry);
QTJAMBI_EXPORT QPair<jobject, jobject> qtjambi_entryset_array_get(JNIEnv *env, jobjectArray array, jint idx);
QTJAMBI_EXPORT jobject qtjambi_arraylist_new(JNIEnv *env, jint size);
QTJAMBI_EXPORT jobject qtjambi_linkedlist_new(JNIEnv *env);
QTJAMBI_EXPORT jobject qtjambi_deque_new(JNIEnv *env);
QTJAMBI_EXPORT jobject qtjambi_list_get(JNIEnv *env, jobject list, jint index);
QTJAMBI_EXPORT void qtjambi_collection_add(JNIEnv *env, jobject list, jobject obj);
QTJAMBI_EXPORT jobject qtjambi_collection_replace(JNIEnv *env, jobject list, int idx, jobject obj);
QTJAMBI_EXPORT jobject qtjambi_collection_remove_last(JNIEnv *env, jobject list);
QTJAMBI_EXPORT void qtjambi_collection_clear(JNIEnv *env, jobject collection);
QTJAMBI_EXPORT jobjectArray qtjambi_collection_toArray(JNIEnv *env, jobject col);
QTJAMBI_EXPORT int qtjambi_collection_size(JNIEnv *env, jobject col);
QTJAMBI_EXPORT jobject qtjambi_collection_iterator(JNIEnv *env, jobject col);
QTJAMBI_EXPORT jobject qtjambi_iterator_next(JNIEnv *env, jobject col);
QTJAMBI_EXPORT bool qtjambi_iterator_has_next(JNIEnv *env, jobject col);

template<typename T>
const void* value_creator(){
    return new T();
}

typedef const void* (*DefaultValueCreator)();
QTJAMBI_EXPORT const void* qtjambi_get_default_value(const std::type_info& type_info, DefaultValueCreator creator);

template<typename T>
const T& qtjambi_get_default_value(){
    return *reinterpret_cast<const T*>(qtjambi_get_default_value(typeid(T), &value_creator<T>));
}

template<typename T>
const T& qtjambi_value_from_nativeId(QtJambiNativeID nativeId){
    if(!!nativeId){
        if(const T* value = qtjambi_object_from_nativeId<T>(nativeId)){
            return *value;
        }
    }
    return qtjambi_get_default_value<T>();
}

template<typename T>
const T& qtjambi_to_value(JNIEnv *env, jobject java_object)
{
    T* v = reinterpret_cast<T*>(qtjambi_to_object(env, java_object));
    return v ? *v : qtjambi_get_default_value<T>();
}

template <template<typename I> class V, class I>
inline jobject qtjambi_to_ArrayList(JNIEnv *__jni_env, const V<I>& data, const std::function<jobject(JNIEnv *,I const &)>& converter){
    jobject list = qtjambi_arraylist_new(__jni_env, jint(data.size()));
    for(typename V<I>::const_iterator i = data.constBegin(); i!=data.constEnd(); ++i){
        qtjambi_collection_add(__jni_env, list, converter(__jni_env,*i));
    }
    return list;
}

template <template<typename I> class V, class I>
inline jobject qtjambi_to_ArrayList(JNIEnv *__jni_env, const V<I>& data, const std::function<jsize(const V<I>&)> sizeFunction, const std::function<jobject(JNIEnv *,I const &)>& converter){
    jobject list = qtjambi_arraylist_new(__jni_env, sizeFunction(data));
    for(typename V<I>::const_iterator i = data.constBegin(); i!=data.constEnd(); ++i){
        qtjambi_collection_add(__jni_env, list, converter(__jni_env,*i));
    }
    return list;
}

template <template<typename I> class V, class I>
inline jobject qtjambi_to_LinkedList(JNIEnv *__jni_env, const V<I>& data, const std::function<jobject(JNIEnv *,I const &)>& converter){
    jobject list = qtjambi_linkedlist_new(__jni_env);
    for(typename V<I>::const_iterator i = data.constBegin(); i!=data.constEnd(); ++i){
        qtjambi_collection_add(__jni_env, list, converter(__jni_env,*i));
    }
    return list;
}

template <template<typename I> class V, class I>
inline jobject qtjambi_to_Deque(JNIEnv *__jni_env, const V<I>& data, const std::function<jobject(JNIEnv *,I const &)>& converter){
    jobject list = qtjambi_deque_new(__jni_env);
    for(typename V<I>::const_iterator i = data.constBegin(); i!=data.constEnd(); ++i){
        qtjambi_collection_add(__jni_env, list, converter(__jni_env,*i));
    }
    return list;
}

template <template<typename I> class V, class I>
inline jobject qtjambi_to_HashSet(JNIEnv *__jni_env, const V<I>& data, const std::function<jobject(JNIEnv *,I const &)>& converter){
    jobject list = qtjambi_hashset_new(__jni_env);
    for(typename V<I>::const_iterator i = data.constBegin(); i!=data.constEnd(); ++i){
        qtjambi_collection_add(__jni_env, list, converter(__jni_env,*i));
    }
    return list;
}

template <template<typename K,typename V> class M, class K, class V>
inline jobject qtjambi_to_HashMap(JNIEnv *__jni_env, const M<K,V>& map,
                                  const std::function<jobject(JNIEnv *,K const &)>& keyConverter,
                                  const std::function<jobject(JNIEnv *,V const &)>& valueConverter){
    jobject java_map = qtjambi_hashmap_new(__jni_env, map.size());
    for(typename M<K,V>::const_iterator i = map.constBegin(); i!=map.constEnd(); ++i){
        qtjambi_map_put(__jni_env, java_map, keyConverter(__jni_env,i.key()), valueConverter(__jni_env,i.value()));
    }
    return java_map;
}

template <template<typename K,typename V> class M, class K, class V>
inline jobject qtjambi_to_MultiHashMap(JNIEnv *__jni_env, const M<K,V>& map,
                                  const std::function<jobject(JNIEnv *,K const &)>& keyConverter,
                                  const std::function<jobject(JNIEnv *,V const &)>& valueConverter){
    jobject java_map = qtjambi_hashmap_new(__jni_env, map.size());
    for(typename M<K,V>::const_iterator i = map.constBegin(); i!=map.constEnd(); ++i){
        qtjambi_multimap_put(__jni_env, java_map, keyConverter(__jni_env,i.key()), valueConverter(__jni_env,i.value()));
    }
    return java_map;
}

template <template<typename K,typename V> class M, class K, class V>
inline jobject qtjambi_to_TreeMap(JNIEnv *__jni_env, const M<K,V>& map, LessThanFunction lessThanFct,
                                  const std::function<jobject(JNIEnv *,K const&)>& keyConverter,
                                  const std::function<jobject(JNIEnv *,V const&)>& valueConverter){
    jobject java_map = qtjambi_treemap_new(__jni_env, lessThanFct);
    for(typename M<K,V>::const_iterator i = map.constBegin(); i!=map.constEnd(); ++i){
        qtjambi_map_put(__jni_env, java_map, keyConverter(__jni_env,i.key()), valueConverter(__jni_env,i.value()));
    }
    return java_map;
}

template <template<typename K,typename V> class M, class K, class V>
inline jobject qtjambi_to_MultiTreeMap(JNIEnv *__jni_env, const M<K,V>& map, LessThanFunction lessThanFct,
                                  const std::function<jobject(JNIEnv *,K const&)>& keyConverter,
                                  const std::function<jobject(JNIEnv *,V const&)>& valueConverter){
    jobject java_map = qtjambi_treemap_new(__jni_env, lessThanFct);
    for(typename M<K,V>::const_iterator i = map.constBegin(); i!=map.constEnd(); ++i){
        qtjambi_multimap_put(__jni_env, java_map, keyConverter(__jni_env,i.key()), valueConverter(__jni_env,i.value()));
    }
    return java_map;
}

inline jobject qtjambi_to_ArrayList(JNIEnv *__jni_env, int size, const std::function<jobject(JNIEnv *,int)>& at){
    jobject list = qtjambi_arraylist_new(__jni_env, size);
    for(int i=0; i<size; ++i){
        qtjambi_collection_add(__jni_env, list, at(__jni_env, i));
    }
    return list;
}

inline jobject qtjambi_to_LinkedList(JNIEnv *__jni_env, int size, const std::function<jobject(JNIEnv *,int)>& at){
    jobject list = qtjambi_linkedlist_new(__jni_env);
    for(int i=0; i<size; ++i){
        qtjambi_collection_add(__jni_env, list, at(__jni_env, i));
    }
    return list;
}

inline jobject qtjambi_to_Deque(JNIEnv *__jni_env, int size, const std::function<jobject(JNIEnv *,int)>& at){
    jobject list = qtjambi_deque_new(__jni_env);
    for(int i=0; i<size; ++i){
        qtjambi_collection_add(__jni_env, list, at(__jni_env, i));
    }
    return list;
}

inline jobject qtjambi_to_HashSet(JNIEnv *__jni_env, int size, const std::function<jobject(JNIEnv *,int)>& at){
    jobject list = qtjambi_hashset_new(__jni_env);
    for(int i=0; i<size; ++i){
        qtjambi_collection_add(__jni_env, list, at(__jni_env, i));
    }
    return list;
}

template <class P>
inline jobject qtjambi_from_QPair(JNIEnv *__jni_env,
                                  const P& pair,
                                  const std::function<jobject(JNIEnv *,const P&)>& first,
                                  const std::function<jobject(JNIEnv *,const P&)>& second){
    return qtjambi_pair_new(__jni_env, first(__jni_env,pair), second(__jni_env,pair));
}

extern "C" QTJAMBI_EXPORT void qtjambi_shutdown();

QTJAMBI_EXPORT const void *qtjambi_to_object_from_shared_pointer(JNIEnv *env, jobject java_object, PointerCreator pointerCreator, PointerDeleter pointerDeleter, PointerGetter pointerGetter);

QTJAMBI_EXPORT const void *qtjambi_to_qobject_from_shared_pointer(JNIEnv *env, jobject java_object, PointerCreator pointerCreator, PointerDeleter pointerDeleter, PointerQObjectGetter pointerGetter);

template<template <typename> class Pointer, typename Instantiation>
inline jobject qtjambi_from_qobject_pointer(JNIEnv *env, const Pointer<Instantiation> & qt_shared_pointer, const char *className){
    return qtjambi_from_object_shared_pointer(env,
                                className,
                                new Pointer<Instantiation>(qt_shared_pointer),
                                &deletePointer<Pointer,Instantiation>,
                                &getFromPointer<Pointer,Instantiation>);
}

template<template<typename> class Pointer, typename O>
inline jobject qtjambi_from_qobject_pointer(JNIEnv *env, const Pointer<O> & qt_shared_pointer){
    return qtjambi_from_qobject_shared_pointer(env,
                                typeid(O),
                                new Pointer<O>(qt_shared_pointer),
                                &deletePointer<Pointer, O>,
                                &getFromPointer<Pointer, O>);
}

template<template<typename> class Pointer, typename O>
inline jobject qtjambi_from_object_pointer(JNIEnv *env, const Pointer<O> & qt_shared_pointer){
    return qtjambi_from_object_shared_pointer(env,
                                typeid(O),
                                new Pointer<O>(qt_shared_pointer),
                                &deletePointer<Pointer, O>,
                                &getFromPointer<Pointer, O>);
}

class QTJAMBI_EXPORT JBufferConstData
{
public:
    JBufferConstData(JNIEnv *env, jobject buffer_object, bool purgeOnDelete = true);

    ~JBufferConstData();

    const void* data() const;

    const void* constData() const;

    qint64 size() const;

    inline operator const void*() const {return constData();}

    template<class T>
    inline operator const T*() const {return reinterpret_cast<const T*>(constData());}
protected:
    jobject m_buffer_object;
    qint64 m_size;
    void* m_data;
    bool m_purgeOnDelete;
    bool m_isdirect;
    Q_DISABLE_COPY(JBufferConstData)
};

class QTJAMBI_EXPORT JBufferData: public JBufferConstData
{
public:
    JBufferData(JNIEnv *env, jobject buffer_object);

    ~JBufferData();

    void* data();

    inline operator void*(){return data();}

    template<class T>
    operator T*(){return reinterpret_cast<T*>(data());}
private:
    Q_DISABLE_COPY(JBufferData)
};

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
        if(JNIEnv* env = qtjambi_current_environment()){
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
    : PointerArray<jobjectArray,T>(env, pointer, qtjambi_create_array(env, javaClass, pointer ? _size : 0), pointer ? _size : 0),
      m_setter(setter)
{
    if(pointer){
        qtjambi_throw_java_exception(env);
        for(jsize i=0; i<PointerArray<jobjectArray,T>::size(); ++i){
            env->SetObjectArrayElement(PointerArray<jobjectArray,T>::array(), i, getter(env, pointer[i]));
        }
        qtjambi_throw_java_exception(env);
    }
}

template<typename T>
inline ObjectPointerArray<T>::ObjectPointerArray(JNIEnv *env, T* pointer, jsize _size,
                   std::function<jobject(JNIEnv *,const T&)> getter,
                   std::function<void(T&,JNIEnv *,jobject)> setter)
    : PointerArray<jobjectArray,T>(env, pointer, qtjambi_create_array(env, typeid(typename std::remove_pointer<T>::type), pointer ? _size : 0), pointer ? _size : 0), m_setter(setter) {
    if(pointer){
        qtjambi_throw_java_exception(env);
        for(jsize i=0; i<PointerArray<jobjectArray,T>::size(); ++i){
            env->SetObjectArrayElement(PointerArray<jobjectArray,T>::array(), i, getter(env, pointer[i]));
        }
        qtjambi_throw_java_exception(env);
    }
}

template<typename T>
inline ObjectPointerArray<T>::~ObjectPointerArray(){
    if(PointerArray<jobjectArray,T>::array()){
        if(JNIEnv* env = qtjambi_current_environment()){
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
    : PointerArray<jobjectArray,const T>(env, pointer, qtjambi_create_array(env, javaClass, pointer ? _size : 0), pointer ? _size : 0)
{
    if(pointer){
        qtjambi_throw_java_exception(env);
        for(jsize i=0; i<PointerArray<jobjectArray,const T>::size(); ++i){
            env->SetObjectArrayElement(PointerArray<jobjectArray,const T>::array(), i, getter(env, pointer[i]));
        }
        qtjambi_throw_java_exception(env);
    }
}

template<typename T>
inline ConstObjectPointerArray<T>::ConstObjectPointerArray(JNIEnv *env, const T* pointer, jsize _size,
                        std::function<jobject(JNIEnv *,const T&)> getter)
    : PointerArray<jobjectArray,const T>(env, pointer, qtjambi_create_array(env, typeid(typename std::remove_pointer<T>::type), pointer ? _size : 0), pointer ? _size : 0)
{
    if(pointer){
        qtjambi_throw_java_exception(env);
        for(jsize i=0; i<PointerArray<jobjectArray,const T>::size(); ++i){
            env->SetObjectArrayElement(PointerArray<jobjectArray,const T>::array(), i, getter(env, pointer[i]));
        }
        qtjambi_throw_java_exception(env);
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
        qtjambi_throw_java_exception(env);
    }

    inline virtual ~JArrayPointer()
    {
        if(m_array){
            if(JNIEnv* env = qtjambi_current_environment()){
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
                if(JNIEnv* env = qtjambi_current_environment()){
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

bool QTJAMBI_EXPORT qtjambi_is_valid_array(JNIEnv *env, jobject object, const std::type_info& typeId);

class QTJAMBI_EXPORT J2CStringBuffer{
public:
    J2CStringBuffer(JNIEnv* env, jstring strg);
    ~J2CStringBuffer();
    const char* constData() const;
    inline operator const char*() const { return constData(); }
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    inline QByteArrayView toByteArrayView() const { return QByteArrayView(constData(), length()); }
    inline operator QByteArrayView() const { return toByteArrayView(); }
    inline QUtf8StringView toUtf8StringView() const { return QUtf8StringView(constData(), length()); }
    inline operator QUtf8StringView() const { return toUtf8StringView(); }
#endif
#if QT_VERSION >= QT_VERSION_CHECK(7,0,0)
    inline QLatin1StringView toLatin1StringView() const { return QLatin1StringView(constData(), length()); }
#endif
    inline QLatin1String toLatin1String() const { return QLatin1String(constData(), length()); }
    inline operator QLatin1String() const { return toLatin1String(); }
    inline const char* data() const { return constData(); }
    inline const char* data() { return constData(); }
    int length() const;
private:
    const jstring m_strg;
    const jsize m_length;
    const char* m_data;
    Q_DISABLE_COPY_MOVE(J2CStringBuffer)
};

class QTJAMBI_EXPORT JString2QChars{
public:
    JString2QChars(JNIEnv* env, jstring strg);
    ~JString2QChars();
    const QChar* constData() const;
    inline operator const QChar*() const { return constData(); }
    inline QStringView toStringView() const { return QStringView(constData(), length()); }
    inline operator QStringView() const { return toStringView(); }
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    inline QAnyStringView toAnyStringView() const { return QAnyStringView(constData(), length()); }
    inline operator QAnyStringView() const { return toAnyStringView(); }
#endif
    inline const QChar* data() const { return constData(); }
    inline const QChar* data() { return constData(); }
    int length() const;
private:
    const jstring m_strg;
    const jsize m_length;
    const jchar* m_data;
    Q_DISABLE_COPY_MOVE(JString2QChars)
};

QTJAMBI_EXPORT void qtjambi_report_qmlobject_destruction(QObject * obj);

QTJAMBI_EXPORT void qtjambi_initialize_native_object(JNIEnv *env, jclass callingClass, jobject object, ConstructorFunction constructorFunction,
                                                     size_t size, const std::type_info& typeId, bool isShell,
                                                     PtrDeleterFunction delete_function,
                                                     jvalue* arguments);

QTJAMBI_EXPORT void qtjambi_initialize_native_object(JNIEnv *env, jclass callingClass, jobject object, ConstructorFunction constructorFunction,
                                                     size_t size, const std::type_info& typeId, bool isShell,
                                                     PtrDeleterFunction delete_function, PtrOwnerFunction ownerFunction,
                                                     jvalue* arguments);

QTJAMBI_EXPORT void qtjambi_initialize_native_value(JNIEnv *env, jclass callingClass, jobject object, ConstructorFunction constructorFunction,
                                                    size_t size, const std::type_info& typeId,
                                                    bool isShell, jvalue* arguments);

QTJAMBI_EXPORT void qtjambi_initialize_native_value(JNIEnv *env, jclass callingClass, jobject object, ConstructorFunction constructorFunction,
                                                    size_t size, const std::type_info& typeId,
                                                    bool isShell, PtrOwnerFunction ownerFunction, jvalue* arguments);

QTJAMBI_EXPORT void qtjambi_initialize_native_functional(JNIEnv *env, jclass callingClass, jobject object, ConstructorFunction constructorFunction,
                                                     size_t size, const std::type_info& typeId, PtrDeleterFunction delete_function);

QTJAMBI_EXPORT void qtjambi_initialize_native_qobject(JNIEnv *env, jclass callingClass, jobject object, ConstructorFunction constructorFunction,
                                                      size_t size, const std::type_info& typeId, const QMetaObject& originalMetaObject, bool isShell, bool hasCustomMetaObject, bool isDeclarativeCall, jvalue* arguments);

template<typename T>
class QTJAMBI_EXPORT QtJambiQmlShellElement final : public T
{
public:
    QtJambiQmlShellElement() : T() {}
    ~QtJambiQmlShellElement() override {
        qtjambi_report_qmlobject_destruction(this);
    }
    static void operator delete(void * ptr) Q_DECL_NOTHROW{
        T::operator delete(ptr);
    }
};

template<typename T>
class QTJAMBI_EXPORT QtJambiQmlElement final : public T
{
public:
    QtJambiQmlElement() : T() {}
    ~QtJambiQmlElement() override {
        qtjambi_report_qmlobject_destruction(this);
    }
};

void QTJAMBI_EXPORT qtjambi_check_resource(JNIEnv *env, const void* ptr, const std::type_info& typeId);

template<typename T>
void qtjambi_check_resource(JNIEnv *env, const T* ptr)
{
    qtjambi_check_resource(env, ptr, typeid(T));
}

class QTJAMBI_EXPORT InvalidateAfterUse{
public:
    InvalidateAfterUse(JNIEnv *env, jobject object, bool checkJavaOwnership = true);
    ~InvalidateAfterUse();
private:
    JNIEnv *m_env;
    jobject m_object;
    bool m_checkJavaOwnership;
    Q_DISABLE_COPY_MOVE(InvalidateAfterUse)
};

#define QTJAMBI_INVALIDATE_AFTER_USE(env, object)\
    InvalidateAfterUse __invalidate_##object##_after_use(env, object);\
    Q_UNUSED(__invalidate_##object##_after_use)

#define QTJAMBI_INVALIDATE_AFTER_USE_UNCHECKED(env, object)\
    InvalidateAfterUse __invalidate_##object##_after_use(env, object, false);\
    Q_UNUSED(__invalidate_##object##_after_use)

class QTJAMBI_EXPORT InvalidateContainerAfterUse{
public:
    InvalidateContainerAfterUse(JNIEnv *env, jobject object, bool checkJavaOwnership = true);
    ~InvalidateContainerAfterUse();
private:
    JNIEnv *m_env;
    jobject m_object;
    bool m_checkJavaOwnership;
    Q_DISABLE_COPY_MOVE(InvalidateContainerAfterUse)
};

#define QTJAMBI_INVALIDATE_CONTAINER_AFTER_USE(env, object)\
    InvalidateContainerAfterUse __invalidate_##object##_after_use(env, object);\
    Q_UNUSED(__invalidate_##object##_after_use)

class QTJAMBI_EXPORT InvalidateArrayAfterUse{
public:
    InvalidateArrayAfterUse(JNIEnv *env, jobjectArray object, bool checkJavaOwnership = true);
    ~InvalidateArrayAfterUse();
private:
    JNIEnv *m_env;
    jobjectArray m_object;
    bool m_checkJavaOwnership;
    Q_DISABLE_COPY_MOVE(InvalidateArrayAfterUse)
};

#define QTJAMBI_INVALIDATE_ARRAY_AFTER_USE(env, object)\
    InvalidateArrayAfterUse __invalidate_##object##_after_use(env, object);\
    Q_UNUSED(__invalidate_##object##_after_use)

class QTJAMBI_EXPORT AboutToPaint{
public:
    AboutToPaint(JNIEnv *env, jobject object);
    ~AboutToPaint();
private:
    JNIEnv *m_env;
    jobject m_object;
    Q_DISABLE_COPY_MOVE(AboutToPaint)
};

#define QTJAMBI_ABOUT_TO_PAINT(env, object)\
    AboutToPaint __about_to_paint_##object(env, object);\
    Q_UNUSED(__about_to_paint_##object)

class QTJAMBI_EXPORT JniLocalFrame{
public:
    JniLocalFrame(JNIEnv *env, int capacity);
    ~JniLocalFrame();
private:
    JNIEnv *m_env;
    Q_DISABLE_COPY_MOVE(JniLocalFrame)
};

#define QTJAMBI_JNI_LOCAL_FRAME(env, capacity)\
    JniLocalFrame __jniLocalFrame(env, capacity);\
    Q_UNUSED(__jniLocalFrame)

class QtJambiScopePrivate;

class QTJAMBI_EXPORT QtJambiScope{
public:
    QtJambiScope();
    QtJambiScope(QtJambiShell* shell);
    QtJambiScope(QtJambiNativeID nativeId);
    QtJambiScope(JNIEnv *env, jobject object);
    ~QtJambiScope();
    void addFinalAction(std::function<void()>&& action);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    void addDeletion(int metaTypeId, void* pointer);
#else
    void addDeletion(QMetaType metaType, void* pointer);
#endif
    template<typename T>
    void addDeletion(T* pointer){
        addFinalAction([pointer](){delete pointer;});
    }
    template<typename T>
    void addArrayDeletion(T* pointer){
        addFinalAction([pointer](){delete[] pointer;});
    }
    void addObjectInvalidation(JNIEnv *env, jobject object, bool checkJavaOwnership, bool persistent = true);
    QtJambiNativeID relatedNativeID() const;
protected:
    QtJambiScope(QtJambiScopePrivate&);
private:
    QtJambiScopePrivate* d;
    Q_DISABLE_COPY_MOVE(QtJambiScope)
};

class QTJAMBI_EXPORT DoFinally{
public:
    DoFinally(std::function<void()> action);
    ~DoFinally();
private:
    std::function<void()> action;
    Q_DISABLE_COPY_MOVE(DoFinally)
};

#define DO_FINALLY(expr)\
    DoFinally __qt_do_finally([=](){expr});\
    Q_UNUSED(__qt_do_finally)\


template<class T>
T& checked_deref(JNIEnv *env, T * ptr)
{
    qtjambi_check_resource<T>(env, ptr);
    return *ptr;
}

template<class T>
const T& reinterpret_value_cast(const void * ptr)
{
    const T* _ptr = reinterpret_cast<const T*>(ptr);
    if(_ptr)
    return *_ptr;
    return qtjambi_get_default_value<T>();
}

template<class T>
const T& reinterpret_deref_cast(JNIEnv *env, const void * ptr)
{
    return checked_deref<const T>(env, reinterpret_cast<const T*>(ptr));
}

template<class T>
T& reinterpret_deref_cast(JNIEnv *env, void * ptr)
{
    return checked_deref<T>(env, reinterpret_cast<T*>(ptr));
}

template<class T>
T qtjambi_to_functional(JNIEnv *env, jobject java_object, const char *className)
{
    T fct = nullptr;
    T* result = &fct;
    if(!qtjambi_convert_to_native(env, typeid(T), nullptr, className, java_object, &result)){
        JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Cannot cast object of type %1 to %2").arg(java_object ? qtjambi_object_class_name(env, java_object) : QString("null")).arg(QLatin1String(className))) QTJAMBI_STACKTRACEINFO );
    }
    return result ? *result : nullptr;
}

template<class T>
T qtjambi_to_functional(JNIEnv *env, jobject java_object)
{
    T fct = nullptr;
    T* result = &fct;
    if(!qtjambi_convert_to_native(env, typeid(T), nullptr, qtjambi_class_name(env, env->GetObjectClass(java_object)).replace(".", "/"), java_object, &result)){
        JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Cannot cast object of type %1 to %2").arg(java_object ? qtjambi_object_class_name(env, java_object) : QString("null")).arg(QLatin1String(qtjambi_type_name(typeid(T))))) QTJAMBI_STACKTRACEINFO );
    }
    return result ? *result : nullptr;
}

template<class T>
T qtjambi_to_functional(JNIEnv *env, QtJambiNativeID native_id)
{
    T result;
    if(!!native_id){
        if(qtjambi_is_shell(native_id)){
            if(FunctionalBase* functionalBase = reinterpret_cast<FunctionalBase *>( qtjambi_from_nativeId(native_id, typeid(T)) )){
                functionalBase->getFunctional(&result);
            }
        }else{
            result = reinterpret_deref_cast<T>(env, qtjambi_from_nativeId(native_id) );
        }
    }
    return result;
}

template<typename T>
T& qtjambi_object_from_nativeId_deref(JNIEnv *env, QtJambiNativeID nativeId)
{
    return checked_deref<T>(env, qtjambi_object_from_nativeId<T>(nativeId));
}

template<typename T>
T& qtjambi_interface_from_nativeId_deref(JNIEnv *env, QtJambiNativeID nativeId)
{
    return checked_deref<T>(env, qtjambi_interface_from_nativeId<T>(nativeId));
}

QTJAMBI_EXPORT void qtjambi_register_pointer_deletion(void* ptr);

QTJAMBI_EXPORT bool qtjambi_is_java_ownership(JNIEnv *env, jobject object);

QTJAMBI_EXPORT bool qtjambi_is_cpp_ownership(JNIEnv *env, jobject object);

QTJAMBI_EXPORT bool qtjambi_is_split_ownership(JNIEnv *env, jobject object);

QTJAMBI_EXPORT bool qtjambi_is_java_ownership(QtJambiNativeID objectId);

QTJAMBI_EXPORT bool qtjambi_is_cpp_ownership(QtJambiNativeID objectId);

QTJAMBI_EXPORT bool qtjambi_is_split_ownership(QtJambiNativeID objectId);

QTJAMBI_EXPORT void qtjambi_set_java_ownership(JNIEnv *env, jobject object);

QTJAMBI_EXPORT void qtjambi_set_cpp_ownership(JNIEnv *env, jobject object);

QTJAMBI_EXPORT void qtjambi_set_default_ownership(JNIEnv *env, jobject object);

QTJAMBI_EXPORT void qtjambi_set_java_ownership(JNIEnv *env, QtJambiNativeID objectId);

QTJAMBI_EXPORT void qtjambi_set_cpp_ownership(JNIEnv *env, QtJambiNativeID objectId);

QTJAMBI_EXPORT void qtjambi_set_default_ownership(JNIEnv *env, QtJambiNativeID objectId);

QTJAMBI_EXPORT void qtjambi_set_java_ownership_for_toplevel_object(JNIEnv *env, QObject* qobject);

QTJAMBI_EXPORT void qtjambi_set_cpp_ownership_for_toplevel_object(JNIEnv *env, QObject* qobject);

QTJAMBI_EXPORT void qtjambi_set_default_ownership_for_toplevel_object(JNIEnv *env, QObject* qobject);

QTJAMBI_EXPORT uint qtjambi_java_object_identity(JNIEnv *env, jobject object);

QTJAMBI_EXPORT uint qtjambi_java_object_hashcode(JNIEnv *env, jobject object);

QTJAMBI_EXPORT jobject qtjambi_make_optional(JNIEnv *env, bool hasValue, jobject object);

QTJAMBI_EXPORT jobject qtjambi_make_optionalint(JNIEnv *env, bool hasValue, jint value);

QTJAMBI_EXPORT jobject qtjambi_make_optionallong(JNIEnv *env, bool hasValue, jlong value);

QTJAMBI_EXPORT jobject qtjambi_make_optionaldouble(JNIEnv *env, bool hasValue, jdouble value);

QTJAMBI_EXPORT jobject qtjambi_read_optional(JNIEnv *env, jobject object, bool& isPresent);

QTJAMBI_EXPORT jint qtjambi_read_optionalint(JNIEnv *env, jobject object, bool& isPresent);

QTJAMBI_EXPORT jlong qtjambi_read_optionallong(JNIEnv *env, jobject object, bool& isPresent);

QTJAMBI_EXPORT jdouble qtjambi_read_optionaldouble(JNIEnv *env, jobject object, bool& isPresent);

QTJAMBI_EXPORT void qtjambi_thread_check(JNIEnv *env, const QObject* object);

QTJAMBI_EXPORT void qtjambi_ui_thread_check(JNIEnv *env, const std::type_info& typeId);

QTJAMBI_EXPORT void qtjambi_pixmap_thread_check(JNIEnv *env, const std::type_info& typeId);

QTJAMBI_EXPORT void qtjambi_argument_thread_check(JNIEnv *env, const char* argumentName, const QObject* argument);

QTJAMBI_EXPORT void qtjambi_argument_ui_thread_check(JNIEnv *env, const char* argumentName, const std::type_info& argumentType);

QTJAMBI_EXPORT void qtjambi_argument_pixmap_thread_check(JNIEnv *env, const char* argumentName, const std::type_info& argumentType);

QTJAMBI_EXPORT void qtjambi_argument_thread_check(JNIEnv *env, const char* argumentName, const std::type_info& argumentType, const QObject* argumentOwner);

QTJAMBI_EXPORT void qtjambi_argument_thread_check(JNIEnv *env, const char* argumentName, const std::type_info& argumentType, const void* argument);

QTJAMBI_EXPORT void qtjambi_constructor_thread_check(JNIEnv *env, const QObject* parent);

QTJAMBI_EXPORT void qtjambi_constructor_thread_check(JNIEnv *env, const std::type_info& parentType, const void* parent);

QTJAMBI_EXPORT void qtjambi_constructor_thread_check(JNIEnv *env, const std::type_info& parentType, const QObject* parentOwner);

QTJAMBI_EXPORT void qtjambi_constructor_ui_thread_check(JNIEnv *env, const std::type_info& constructedType);

QTJAMBI_EXPORT void qtjambi_constructor_pixmap_thread_check(JNIEnv *env, const std::type_info& constructedType);

QTJAMBI_EXPORT void qtjambi_constructor_widget_thread_check(JNIEnv *env, const std::type_info& constructedType, const QObject* parent);

QTJAMBI_EXPORT void qtjambi_constructor_window_thread_check(JNIEnv *env, const std::type_info& constructedType, const QObject* parent);

QTJAMBI_EXPORT void qtjambi_constructor_application_thread_check(JNIEnv *env, const std::type_info& constructedType);

QTJAMBI_EXPORT const QObject* qtjambi_pixmap_owner_function(const void *);

class QFutureInterfaceBase;
class QFutureWatcherBase;

typedef void(*ResultTranslator)(JNIEnv *, const QSharedPointer<QFutureInterfaceBase>&, const QSharedPointer<QFutureInterfaceBase>&, int, int);

QTJAMBI_EXPORT QFutureInterfaceBase* qtjambi_translate_futureinterfaces(QSharedPointer<QFutureInterfaceBase>&& sourceFuture, QSharedPointer<QFutureInterfaceBase>&& targetFuture, const char* translatedType, ResultTranslator resultTranslator, ResultTranslator resultRetranslator);

typedef void(*FutureSetter)(JNIEnv *, QFutureWatcherBase*, jobject);
typedef jobject(*FutureResult)(JNIEnv *, QFutureWatcherBase*, int);
typedef jobject(*FutureGetter)(JNIEnv *, QFutureWatcherBase*);
QTJAMBI_EXPORT jobject qtjambi_from_QFutureWatcher(JNIEnv* env, const QFutureWatcherBase* futureWatcher,
                                                   FutureSetter futureSetter, FutureResult futureResult, FutureGetter futureGetter);

QTJAMBI_EXPORT jobject qtjambi_create_QFuture(JNIEnv* env, jobject futureInterface);

QTJAMBI_EXPORT jobject qtjambi_get_QFutureInterface(JNIEnv* env, jobject future);

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
template <typename T> class QFuture;
QTJAMBI_EXPORT QFutureInterfaceBase& qtjambi_get_QFutureInterface(const QFuture<void>& future);
#else
QTJAMBI_EXPORT jobject qtjambi_create_QPromise(JNIEnv* env, jobject futureInterface, const void* promise, QtJambiScope* scope);

QTJAMBI_EXPORT void* qtjambi_get_native_QPromise(JNIEnv* env, jobject promise);
#endif

#endif // QTJAMBI_CORE_H

#endif //defined __cplusplus

